/*
 * (C) Copyright 2010 - 2015
 * Vipin Kumar, ST Micoelectronics, vipin.kumar@st.com.
 * Alexey Malahov, Baikal Electronics, Alexey.Malahov@baikalelectronics.ru
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

/*
 * Designware ethernet IP driver for u-boot
 */

#include <common.h>
#include <miiphy.h>
#include <malloc.h>
#include <linux/compiler.h>
#include <linux/err.h>
#include <asm/io.h>

#include "designware.h"

#if (defined(CONFIG_PHY_MICREL) && defined(CONFIG_TARGET_BAIKAL_MIPS))
#include <micrel.h>
#endif /* CONFIG_PHY_MICREL && CONFIG_TARGET_BAIKAL_MIPS */

//#define DEBUG

#if !defined(CONFIG_PHYLIB)
# error "DesignWare Ether MAC requires PHYLIB - missing CONFIG_PHYLIB"
#endif

static int dw_mdio_read(struct mii_dev *bus, int addr, int devad, int reg)
{
	struct eth_mac_regs *mac_p = bus->priv;
	ulong start;
	u16 miiaddr;
	int timeout = CONFIG_MDIO_TIMEOUT;

	miiaddr = ((addr << MIIADDRSHIFT) & MII_ADDRMSK) |
		  ((reg << MIIREGSHIFT) & MII_REGMSK);

	writel(miiaddr | MII_CLKRANGE_150_250M | MII_BUSY, &mac_p->miiaddr);

	start = get_timer(0);
	while (get_timer(start) < timeout) {
		if (!(readl(&mac_p->miiaddr) & MII_BUSY))
			return readl(&mac_p->miidata);
		udelay(10);
	};

	return -1;
}

static int dw_mdio_write(struct mii_dev *bus, int addr, int devad, int reg,
			u16 val)
{
	struct eth_mac_regs *mac_p = bus->priv;
	ulong start;
	u16 miiaddr;
	int ret = -1, timeout = CONFIG_MDIO_TIMEOUT;

	writel(val, &mac_p->miidata);
	miiaddr = ((addr << MIIADDRSHIFT) & MII_ADDRMSK) |
		  ((reg << MIIREGSHIFT) & MII_REGMSK) | MII_WRITE;

	writel(miiaddr | MII_CLKRANGE_150_250M | MII_BUSY, &mac_p->miiaddr);

	start = get_timer(0);
	while (get_timer(start) < timeout) {
		if (!(readl(&mac_p->miiaddr) & MII_BUSY)) {
			ret = 0;
			break;
		}
		udelay(10);
	};

	return ret;
}

#ifdef CONFIG_TARGET_BAIKAL_MIPS
static int dw_mdio_reset(struct mii_dev *bus)
{
	struct eth_mac_regs *mac_p = bus->priv;
	u32 val;

	val = readl(&mac_p->gpio);
	writel(val & ~MAC_GPIO_GPO0, &mac_p->gpio);
	udelay(10);
	writel(val | MAC_GPIO_GPO0, &mac_p->gpio);
	udelay(15000);

	return 0;
}
#endif

static int dw_mdio_init(char *name, struct eth_mac_regs *mac_regs_p)
{
	struct mii_dev *bus = mdio_alloc();

	if (!bus) {
		printf("Failed to allocate MDIO bus\n");
		return -1;
	}

	bus->read = dw_mdio_read;
	bus->write = dw_mdio_write;
#ifdef CONFIG_TARGET_BAIKAL_MIPS
	bus->reset = dw_mdio_reset;
#endif
	sprintf(bus->name, name);

	bus->priv = (void *)mac_regs_p;

	return mdio_register(bus);
}

static void tx_descs_init(struct eth_device *dev)
{
	struct dw_eth_dev *priv = dev->priv;
	struct eth_dma_regs *dma_p = priv->dma_regs_p;
	struct dmamacdescr *desc_table_p = &priv->tx_mac_descrtable[0];
	char *txbuffs = &priv->txbuffs[0];
	struct dmamacdescr *desc_p;
	u32 idx;

	memset(desc_table_p, 0, (sizeof(struct dmamacdescr) * CONFIG_TX_DESCR_NUM));
	memset(txbuffs, 0, TX_TOTAL_BUFSIZE);

	for (idx = 0; idx < CONFIG_TX_DESCR_NUM; idx++) {
		desc_p = &desc_table_p[idx];
		desc_p->dmamac_addr = (char *)virt_to_phys(&txbuffs[idx * CONFIG_ETH_BUFSIZE]);
		desc_p->dmamac_next = (struct dmamacdescr *)virt_to_phys(&desc_table_p[idx + 1]);

#if defined(CONFIG_DW_ALTDESCRIPTOR)
		desc_p->txrx_status &= ~(DESC_TXSTS_TXINT | DESC_TXSTS_TXLAST |
				DESC_TXSTS_TXFIRST | DESC_TXSTS_TXCRCDIS | \
				DESC_TXSTS_TXCHECKINSCTRL | DESC_TXSTS_TXPADDIS);

		desc_p->txrx_status |= DESC_TXSTS_TXCHAIN;
		desc_p->dmamac_cntl = 0;
		desc_p->txrx_status &= ~(DESC_TXSTS_MSK | DESC_TXSTS_OWNBYDMA);
#else
		desc_p->dmamac_cntl = DESC_TXCTRL_TXCHAIN;
		desc_p->txrx_status = 0;
#endif
	}

	/* Correcting the last pointer of the chain */
	desc_p->dmamac_next = (struct dmamacdescr *)virt_to_phys(&desc_table_p[0]);

#ifdef DEBUG
		printf("## %s: TX Descriptors\n", dev->name);
		for (idx = 0; idx < CONFIG_TX_DESCR_NUM; idx++) {
			desc_p = &desc_table_p[idx];
			printf("  %d: %x %x %p %p\n", idx, desc_p->txrx_status,
				desc_p->dmamac_cntl, desc_p->dmamac_addr, desc_p->dmamac_next);
			puts("\n");
		}
#endif

	flush_dcache_range((ulong)&desc_table_p[0], (ulong)desc_p + sizeof(*desc_p));
	writel(virt_to_phys(&desc_table_p[0]), &dma_p->txdesclistaddr);

	priv->tx_currdescnum = 0;
}

static void rx_descs_init(struct eth_device *dev)
{
	struct dw_eth_dev *priv = dev->priv;
	struct eth_dma_regs *dma_p = priv->dma_regs_p;
	struct dmamacdescr *desc_table_p = &priv->rx_mac_descrtable[0];
	char *rxbuffs = &priv->rxbuffs[0];
	struct dmamacdescr *desc_p;
	u32 idx;

	memset(desc_table_p, 0, (sizeof(struct dmamacdescr) * CONFIG_RX_DESCR_NUM));
	memset(rxbuffs, 0, RX_TOTAL_BUFSIZE);

	for (idx = 0; idx < CONFIG_RX_DESCR_NUM; idx++) {
		desc_p = &desc_table_p[idx];

		desc_p->dmamac_addr = (char *)virt_to_phys(&rxbuffs[idx * CONFIG_ETH_BUFSIZE]);
		desc_p->dmamac_next = (struct dmamacdescr *)virt_to_phys(&desc_table_p[idx + 1]);

		desc_p->dmamac_cntl =
			(MAC_MAX_FRAME_SZ & DESC_RXCTRL_SIZE1MASK) | \
				      DESC_RXCTRL_RXCHAIN;

		desc_p->txrx_status = DESC_RXSTS_OWNBYDMA;
	}

	/* Correcting the last pointer of the chain */
	desc_p->dmamac_next = (struct dmamacdescr *)virt_to_phys(&desc_table_p[0]);

#ifdef DEBUG
		printf("## %s: RX Descriptors\n", dev->name);
		for (idx = 0; idx < CONFIG_RX_DESCR_NUM; idx++) {
			desc_p = &desc_table_p[idx];
			printf("  %d: %x %x %p %p\n", idx, desc_p->txrx_status,
				desc_p->dmamac_cntl, desc_p->dmamac_addr, desc_p->dmamac_next);
			puts("\n");
		}
#endif

	flush_dcache_range((ulong)&desc_table_p[0], (ulong)desc_p + sizeof(*desc_p));
	writel(virt_to_phys(&desc_table_p[0]), &dma_p->rxdesclistaddr);

	priv->rx_currdescnum = 0;
}

static int dw_write_hwaddr(struct eth_device *dev)
{
	struct dw_eth_dev *priv = dev->priv;
	struct eth_mac_regs *mac_p = priv->mac_regs_p;
	u32 macid_lo, macid_hi;
	u8 *mac_id = &dev->enetaddr[0];

	macid_lo = mac_id[0] + (mac_id[1] << 8) + (mac_id[2] << 16) +
		   (mac_id[3] << 24);
	macid_hi = mac_id[4] + (mac_id[5] << 8);

	writel(macid_hi, &mac_p->macaddr0hi);
	writel(macid_lo, &mac_p->macaddr0lo);

	return 0;
}

static void dw_adjust_link(struct eth_mac_regs *mac_p,
			   struct phy_device *phydev)
{
	u32 conf = readl(&mac_p->conf) | FRAMEBURSTENABLE | DISABLERXOWN;

	if (!phydev->link) {
		printf("%s: No link.\n", phydev->dev->name);
		return;
	}

	if (phydev->speed != 1000)
		conf |= MII_PORTSELECT;

#if defined(CONFIG_TARGET_BAIKAL_MIPS) && !defined(CONFIG_BAIKAL_T1)
	if (phydev->speed != 100) {
#else
	if (phydev->speed != 10) {
#endif
		conf |= FES_100;	/* Set 100Mb */
	} else {
		conf &= ~FES_100;	/* Set 10Mb */
	}

	if (phydev->duplex)
		conf |= FULLDPLXMODE;

#if 0
	/* Set the loopback mode. */
	conf |= LOOPBACKMODE; 
#endif  /* 0/1 */

	writel(conf, &mac_p->conf);

	printf("Speed: %d, %s duplex%s\n", phydev->speed,
	       (phydev->duplex) ? "full" : "half",
	       (phydev->port == PORT_FIBRE) ? ", fiber mode" : "");
}

static void dw_eth_halt(struct eth_device *dev)
{
	struct dw_eth_dev *priv = dev->priv;
	struct eth_mac_regs *mac_p = priv->mac_regs_p;
	struct eth_dma_regs *dma_p = priv->dma_regs_p;

	writel(readl(&mac_p->conf) & ~(RXENABLE | TXENABLE), &mac_p->conf);
	writel(readl(&dma_p->opmode) & ~(RXSTART | TXSTART), &dma_p->opmode);

	phy_shutdown(priv->phydev);
}

#ifdef CONFIG_TARGET_BAIKAL_PHY_ERRATA
#define MMD_RGMII_TX_SKEW_ADDR		2
#define TX_DATA_PAD_SKEW_REG		6
#define TX_CLOCK_PAD_SKEW_REG		8
static void micrel_set_skew(struct phy_device *phydev)
{
        int reg;
	
	ksz9031_phy_extended_write(phydev, MMD_RGMII_TX_SKEW_ADDR,
					TX_DATA_PAD_SKEW_REG, 0x4000, 0);
	reg = ksz9031_phy_extended_read(phydev, MMD_RGMII_TX_SKEW_ADDR,
					TX_CLOCK_PAD_SKEW_REG, 0x4000);
	reg |= (0x1F << 5);
	ksz9031_phy_extended_write(phydev, MMD_RGMII_TX_SKEW_ADDR,
					TX_CLOCK_PAD_SKEW_REG, 0x4000, reg);

	return;
}
#endif /* CONFIG_TARGET_BAIKAL_PHY_ERRATA */

void designware_clear_phy_reset(ulong base)
{
	struct eth_mac_regs *mac_p = (struct eth_mac_regs *)base;

	/* Make sure device has had enough time being under reset then clear
	 * the pin state */
	udelay(10);
	writel(readl(&mac_p->gpio) | MAC_GPIO_GPO0, &mac_p->gpio);
}

static int dw_eth_init(struct eth_device *dev, bd_t *bis)
{
	volatile struct dw_eth_dev *priv = dev->priv;
	volatile struct eth_mac_regs *mac_p = priv->mac_regs_p;
	volatile struct eth_dma_regs *dma_p = priv->dma_regs_p;
	unsigned int start;

	writel(readl(&dma_p->busmode) | DMAMAC_SRST, &dma_p->busmode);

#ifdef CONFIG_TARGET_BAIKAL_MIPS
	/* Clear PHY reset. */
	designware_clear_phy_reset((ulong)mac_p);
#endif /* CONFIG_TARGET_BAIKAL_MIPS */

	start = get_timer(0);
	while (readl(&dma_p->busmode) & DMAMAC_SRST) {
		if (get_timer(start) >= CONFIG_MACRESET_TIMEOUT) {
			printf("Could not reset PHY %s\n", priv->phydev->dev->name);
			return -1;
		}

		mdelay(100);
	};

	/* Soft reset above clears HW address registers.
	 * So we have to set it here once again */
	dw_write_hwaddr(dev);

	rx_descs_init(dev);
	tx_descs_init(dev);

	writel(FIXEDBURST | PRIORXTX_41 | DMA_PBL | ALTERDESC, &dma_p->busmode);

	writel(readl(&dma_p->opmode) | FLUSHTXFIFO /* | STOREFORWARD */,
	       &dma_p->opmode);

	/* Enable transmition and recive operations in the opmode register. */
	writel(readl(&dma_p->opmode) | RXSTART | TXSTART, &dma_p->opmode);

	/* Configure data/clock skew for Micrel RGMII. */
#if (defined(CONFIG_PHY_MICREL) && defined(CONFIG_TARGET_BAIKAL_PHY_ERRATA))
	micrel_set_skew(priv->phydev);
#endif /* CONFIG_PHY_MICREL && CONFIG_TARGET_BAIKAL_PHY_ERRATA */

	/* Start up the PHY */
	if (phy_startup(priv->phydev)) {
		printf("Could not initialize PHY %s\n",
		       priv->phydev->dev->name);
		return -1;
	}

	dw_adjust_link((struct eth_mac_regs *)mac_p, priv->phydev);

	if (!priv->phydev->link)
		return -1;

	/* Enable transmition operations in the MAC conf register. */
	writel(readl(&mac_p->conf) | TXENABLE, &mac_p->conf);

	/* Enable receive operations in the MAC conf register. */
	writel(readl(&mac_p->conf) | RXENABLE, &mac_p->conf);

#ifdef DEBUG
	printf("mac conf =0x%x\n", readl(&mac_p->conf));
#endif /* DEBUG */

	phy_config(priv->phydev);

	return 0;
}

static int dw_eth_send(struct eth_device *dev, void *packet, int length)
{
	volatile struct dw_eth_dev *priv = dev->priv;
	volatile struct eth_dma_regs *dma_p = priv->dma_regs_p;
	u32 desc_num = priv->tx_currdescnum;
	volatile struct dmamacdescr *desc_p = &priv->tx_mac_descrtable[desc_num];
	void *data;

	/* Check if the descriptor is owned by CPU */
	invalidate_dcache_range((ulong)desc_p, (ulong)desc_p + sizeof(*desc_p));
	if (desc_p->txrx_status & DESC_TXSTS_OWNBYDMA) {
		printf("CPU not owner of tx frame txrx_status = 0x%x\n", desc_p->txrx_status);
		return -1;
	}

	data = phys_to_virt((ulong)desc_p->dmamac_addr);
	memcpy(data, packet, length);
	flush_dcache_range((ulong)data, (ulong)data + length);

#ifdef PRINT_PACKETS
	print_buffer((ulong)data, data, 1, length, 16);
#endif

	/* Test the wrap-around condition. */
	if (++desc_num >= CONFIG_TX_DESCR_NUM ) {
		desc_num = 0;
	}

#if defined(CONFIG_DW_ALTDESCRIPTOR)
	desc_p->txrx_status |= DESC_TXSTS_TXFIRST | DESC_TXSTS_TXLAST | DESC_TXSTS_TXCHECKINSCTRL;
	desc_p->dmamac_cntl |= (length << DESC_TXCTRL_SIZE1SHFT) & \
					DESC_TXCTRL_SIZE1MASK;

	desc_p->txrx_status |= DESC_TXSTS_TXCHAIN;
	desc_p->txrx_status &= ~(DESC_TXSTS_MSK);

	flush_dcache_range((ulong)desc_p, (ulong)desc_p + sizeof(*desc_p));
	desc_p->txrx_status |= DESC_TXSTS_OWNBYDMA;
#else
	desc_p->dmamac_cntl |= ((length << DESC_TXCTRL_SIZE1SHFT) & \
			       DESC_TXCTRL_SIZE1MASK) | DESC_TXCTRL_TXLAST | \
			       DESC_TXCTRL_TXFIRST | DESC_TXCTRL_TXCHECKINSCTRL;

	flush_dcache_range((ulong)desc_p, (ulong)desc_p + sizeof(*desc_p));
	desc_p->txrx_status = DESC_TXSTS_OWNBYDMA;
#endif

	priv->tx_currdescnum = desc_num;

	/* Start the transmission */
	flush_dcache_range((ulong)desc_p, (ulong)desc_p + sizeof(*desc_p));
	writel(0, &dma_p->txpolldemand);

#define WAIT_TX_COMPLETE
#ifdef WAIT_TX_COMPLETE
	do {
		invalidate_dcache_range((ulong)desc_p, (ulong)desc_p + sizeof(*desc_p));
	} while (desc_p->txrx_status & DESC_TXSTS_OWNBYDMA);
#endif /* WAIT_TX_COMPLETE */

	return 0;
}


static int dw_eth_recv(struct eth_device *dev)
{
	volatile struct dw_eth_dev *priv = dev->priv;
	uint32_t desc_num = priv->rx_currdescnum;
	volatile struct dmamacdescr *desc_p = &priv->rx_mac_descrtable[desc_num];
	uint32_t length = 0;
	void *data;

	/* Check  if the owner is the CPU */
	invalidate_dcache_range((ulong)desc_p, (ulong)desc_p + sizeof(*desc_p));
	if (!(desc_p->txrx_status & DESC_RXSTS_OWNBYDMA)) {

		length = ((desc_p->txrx_status >> DESC_RXSTS_FRMLENSHFT) & DESC_RXSTS_FRMLENMSK);
		data = phys_to_virt((ulong)desc_p->dmamac_addr);

		invalidate_dcache_range((ulong)data, (ulong)data + length);
#ifdef PRINT_PACKETS
		print_buffer((ulong)data, data, 1, length, 16);
#endif
		NetReceive(data, length);

		/*
		 * Make the current descriptor valid again and go to
		 * the next one
		 */
		desc_p->dmamac_cntl = (MAC_MAX_FRAME_SZ & DESC_RXCTRL_SIZE1MASK) | \
				      DESC_RXCTRL_RXCHAIN;

		desc_p->txrx_status = DESC_RXSTS_OWNBYDMA;
		flush_dcache_range((ulong)desc_p, (ulong)desc_p + sizeof(*desc_p));

		/* Test the wrap-around condition. */
		if (++desc_num >= CONFIG_RX_DESCR_NUM)
			desc_num = 0;
	}

	priv->rx_currdescnum = desc_num;

	return length;
}

static int dw_phy_init(struct eth_device *dev)
{
	volatile struct dw_eth_dev *priv = dev->priv;
	struct phy_device *phydev;
	int mask = 0xffffffff;

#ifdef CONFIG_PHY_ADDR
	mask = 1 << CONFIG_PHY_ADDR;
#endif
	phydev = phy_find_by_mask(priv->bus, mask, priv->interface);
	if (!phydev)
		return -1;

	phy_connect_dev(phydev, dev);

	phydev->supported = PHY_BASIC_FEATURES | PHY_GBIT_FEATURES;
	phydev->advertising = phydev->supported;

	priv->phydev = phydev;
	phy_config(phydev);

	return 1;
}

int designware_initialize(ulong base_addr, u32 interface)
{
	struct eth_device *dev;
	struct dw_eth_dev *priv;

	dev = (struct eth_device *) malloc(sizeof(struct eth_device));
	if (!dev)
		return -ENOMEM;

#if 0
	dev = (struct eth_device *) KSEG1ADDR(dev);
#endif /* CONFIG_TARGET_BAIKAL_MIPS */

	/*
	 * Since the priv structure contains the descriptors which need a strict
	 * buswidth alignment, memalign is used to allocate memory
	 */
	priv = (struct dw_eth_dev *) memalign(ARCH_DMA_MINALIGN,
					      sizeof(struct dw_eth_dev));
	if (!priv) {
		free(dev);
		return -ENOMEM;
	}

#if 0
	priv = (struct dw_eth_dev *) KSEG1ADDR(priv);
#endif /* CONFIG_TARGET_BAIKAL_MIPS */

	memset(dev, 0, sizeof(struct eth_device));
	memset(priv, 0, sizeof(struct dw_eth_dev));

	sprintf(dev->name, "dwmac.%lx", base_addr);
	dev->iobase = (int)base_addr;
	dev->priv = priv;

	priv->dev = dev;
	priv->mac_regs_p = (struct eth_mac_regs *)base_addr;
	priv->dma_regs_p = (struct eth_dma_regs *)(base_addr +
			DW_DMA_BASE_OFFSET);

	dev->init = dw_eth_init;
	dev->send = dw_eth_send;
	dev->recv = dw_eth_recv;
	dev->halt = dw_eth_halt;
	dev->write_hwaddr = dw_write_hwaddr;

	eth_register(dev);

	priv->interface = interface;

	dw_mdio_init(dev->name, priv->mac_regs_p);
	priv->bus = miiphy_get_dev_by_name(dev->name);

	return dw_phy_init(dev);
}
