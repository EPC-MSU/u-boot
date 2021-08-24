/*
 * File:        pcie_dw.c
 * Description: Designware PCI support
 * Author:      Alex Matveev
 * Copyright (C) 2017-2018 T-Platforms JSC
 *
 * PCI support - ported from Linux driver
 */

#include <common.h>
#include <linux/compat.h>
#include <linux/types.h>
#include <pci.h>
#include <asm/io.h>
#include <asm/system.h>
#include <asm/arch/pci.h>
#include <asm/gpio.h>

#define PCIE_PHY_RETRIES        1000000
#define PCIE_ERROR_VALUE        0xFFFFFFFF
#define PHY_ALL_LANES           0xF
#define PHY_LANE0               0x1

#define OK      0
#define ERROR  -1
#define ERROR_MISMATCH1         0x0010
#define ERROR_MISMATCH2         0x0020
#define ERROR_MISMATCH3         0x0040
#define ERROR_MISMATCH4         0x0080
#define ERROR_MISMATCH5         0x0100
#define ERROR_MISMATCH6         0x0200
#define ERROR_MISMATCH7         0x0400
#define ERROR_MISMATCH8         0x0800

static uint32_t dw_pcie_phy_read(uint32_t phy_addr)
{
	uint32_t reg;
	int i;

	/* Set lane0 for reading values. */
	WRITE_PCIE_REG(PCIE_BK_MGMT_SEL_LANE, PHY_LANE0);

	/* Write the address of the PHY register. */
	WRITE_PCIE_REG(PCIE_BK_MGMT_CTRL, (phy_addr & BK_MGMT_CTRL_ADDR_MASK) | BK_MGMT_CTRL_READ);

	for (i = 0; i < PCIE_PHY_RETRIES; i++) {
		reg = READ_PCIE_REG(PCIE_BK_MGMT_CTRL);
		if (reg & BK_MGMT_CTRL_DONE) {
			/* Read data register. */
			reg = READ_PCIE_REG(PCIE_BK_MGMT_READ_DATA);
			return reg;
		}
	}

	debug("%s: timeout expired for phy_addr=0x%x\n", __FUNCTION__, phy_addr);
	return PCIE_ERROR_VALUE;
}

static uint32_t dw_pcie_phy_write(uint32_t phy_addr, uint32_t val)
{
	uint32_t reg;
	int i;

	/* Set line. */
	WRITE_PCIE_REG(PCIE_BK_MGMT_SEL_LANE, PHY_ALL_LANES);

	/* Write value to data register. */
	WRITE_PCIE_REG(PCIE_BK_MGMT_WRITE_DATA, val);

	/* Write the address of the PHY register. */
	WRITE_PCIE_REG(PCIE_BK_MGMT_CTRL, (phy_addr & BK_MGMT_CTRL_ADDR_MASK) | BK_MGMT_CTRL_WRITE);

	for (i = 0; i < PCIE_PHY_RETRIES; i++) {
		reg = READ_PCIE_REG(PCIE_BK_MGMT_CTRL);
		if (reg & BK_MGMT_CTRL_DONE) {
			return OK;
		}
	}

	debug("%s: timeout expired for phy_addr=0x%x\n", __FUNCTION__, phy_addr);
	return PCIE_ERROR_VALUE;
}

static void dw_set_iatu_region(int dir, int index, int base_addr, int limit_addr, int target_addr, int tlp_type)
{
	WRITE_PCIE_REG(PCIE_IATU_VIEWPORT_OFF, ((index << REGION_INDEX_SHIFT) | (dir << REGION_DIR_SHIFT)));
	WRITE_PCIE_REG(PCIE_IATU_LWR_BASE_ADDR_OFF_OUTBOUND_0, (base_addr << LWR_BASE_RW_SHIFT));
	WRITE_PCIE_REG(PCIE_IATU_UPR_BASE_ADDR_OFF_OUTBOUND_0, 0);
	WRITE_PCIE_REG(PCIE_IATU_LIMIT_ADDR_OFF_OUTBOUND_0, (limit_addr << LIMIT_ADDR_RW_SHIFT));
	WRITE_PCIE_REG(PCIE_IATU_LWR_TARGET_ADDR_OFF_OUTBOUND_0, (target_addr << LWR_TARGET_RW_SHIFT));
	WRITE_PCIE_REG(PCIE_IATU_UPR_TARGET_ADDR_OFF_OUTBOUND_0, 0);
	WRITE_PCIE_REG(PCIE_IATU_REGION_CTRL_1_OFF_OUTBOUND_0, (tlp_type << IATU_REGION_CTRL_1_OFF_OUTBOUND_0_TYPE_SHIFT));
	WRITE_PCIE_REG(PCIE_IATU_REGION_CTRL_2_OFF_OUTBOUND_0, IATU_REGION_CTRL_2_OFF_OUTBOUND_0_REGION_EN);

	smp_mb();
}

#ifndef CONFIG_BAIKAL_T1
#define PLL_WAIT_RETRIES 1000
static int dw_init_pll(const unsigned int pmu_register)
{
	uint32_t reg;
	int i = 0;

	/* Wait for LOCK bit in BK_PMU_COREPLL_CTL */
	while(!(READ_PMU_REG(BK_PMU_COREPLL_CTL) & BK_PMU_LOCK_BIT)) {
		if((i++) == PLL_WAIT_RETRIES) {
			return ERROR;
		}
	}
	/* Set EN & RST bit in pmu_register */
	reg = READ_PMU_REG(pmu_register);
	reg |= BK_PMU_EN_BIT | BK_PMU_RST_BIT;
	WRITE_PMU_REG(pmu_register, reg);

	/* Wait for LOCK bit in pmu_register */
	i = 0;
	while(!(READ_PMU_REG(pmu_register) & BK_PMU_LOCK_BIT)) {
		if((i++) == PLL_WAIT_RETRIES) {
			return ERROR;
		}
	}

	return OK;
}
#endif

static int dw_pcie_init(void)
{
	volatile uint32_t reg;
	int i, st = 0;
	uint32_t rstc_mask = 0;

	/* PMU PCIe init. */

#ifndef CONFIG_BAIKAL_T1
	/* 1., 2. Start BK_PMU_PCIEPLL_CTL. */
	dw_init_pll(BK_PMU_PCIEPLL_CTL);
#endif

	/* 3. Read value of BK_PMU_AXI_PCIE_M_CTL, set EN bit. */
	reg = READ_PMU_REG(BK_PMU_AXI_PCIE_M_CTL);
	reg |= PMU_AXI_PCIE_M_CTL_EN;
	WRITE_PMU_REG(BK_PMU_AXI_PCIE_M_CTL, reg);

	/* 4. Read value of BK_PMU_AXI_PCIE_S_CTL, set EN bit. */
	reg = READ_PMU_REG(BK_PMU_AXI_PCIE_S_CTL);
	reg |= PMU_AXI_PCIE_S_CTL_EN;
	WRITE_PMU_REG(BK_PMU_AXI_PCIE_S_CTL, reg);

	/*
	 * 5. Manage RESET* bits
	 * (PHY_RESET, PIPE_RESET, CORE_RST, PWR_RST, STICKY_RST, NONSTICKY_RST)
	 */
	reg = READ_PMU_REG(BK_PMU_PCIE_RSTC);
#ifdef CONFIG_BAIKAL_T1
	/* we have Baikal-T1 chip, perform enhanced reset procedure */
	if (reg & PMU_PCIE_RSTC_REQ_PHY_RST)
		rstc_mask |= PMU_PCIE_RSTC_PHY_RESET;
	if (reg & PMU_PCIE_RSTC_REQ_CORE_RST)
		rstc_mask |= PMU_PCIE_RSTC_CORE_RST;
	if (reg & PMU_PCIE_RSTC_REQ_STICKY_RST)
		rstc_mask |= PMU_PCIE_RSTC_STICKY_RST;
	if (reg & PMU_PCIE_RSTC_REQ_NON_STICKY_RST)
		rstc_mask |= PMU_PCIE_RSTC_NONSTICKY_RST;
#else /* BAIKAL-T */
	/* we have Baikal-T chip, perform simplified reset procedure */
	rstc_mask = (PMU_PCIE_RSTC_PHY_RESET | PMU_PCIE_RSTC_PIPE_RESET |
		     PMU_PCIE_RSTC_CORE_RST|  PMU_PCIE_RSTC_PWR_RST |
		     PMU_PCIE_RSTC_STICKY_RST | PMU_PCIE_RSTC_NONSTICKY_RST);
#endif
	WRITE_PMU_REG(BK_PMU_PCIE_RSTC, reg | rstc_mask);
	udelay(10);
	reg = READ_PMU_REG(BK_PMU_PCIE_RSTC);
	reg &= ~rstc_mask;
	reg = READ_PMU_REG(BK_PMU_PCIE_RSTC);
	if (reg & 0x3f11) {
		reg &= ~0x3f11;
		WRITE_PMU_REG(BK_PMU_PCIE_RSTC, reg);
		udelay(10);
		reg = READ_PMU_REG(BK_PMU_PCIE_RSTC);
	}

	/* 3.2 Set writing to RO Registers Using DBI */
	WRITE_PCIE_REG(PCIE_MISC_CONTROL_1_OFF, DBI_RO_WR_EN);

	/* 3.1 Set DBI2 mode, dbi2_cs = 0x1 */
	reg = READ_PMU_REG(BK_PMU_PCIE_GENC);
	reg &= ~PMU_PCIE_GENC_DBI2_MODE;
	WRITE_PMU_REG(BK_PMU_PCIE_GENC, reg);
	/* set PCI bridge class (subtractive decode) */
	WRITE_PCIE_REG(PCI_CLASS_REVISION, (PCI_CLASS_CODE_BRIDGE << 24) |
					   (PCI_CLASS_SUB_CODE_BRIDGE_PCI << 16) |
					   (0x1 << 8) | 0x01); /* transparent, rev.1 */
	reg |= PMU_PCIE_GENC_DBI2_MODE;
	WRITE_PMU_REG(BK_PMU_PCIE_GENC, reg);

	/* 4.1 Allow access to the PHY registers, phy0_mgmt_pcs_reg_sel = 0x1. */
	reg = READ_PMU_REG(BK_PMU_PCIE_GENC);
	reg |= PMU_PCIE_GENC_MGMT_ENABLE;
	WRITE_PMU_REG(BK_PMU_PCIE_GENC, reg);

	/* 4.2 All lanes can read/write PHY registers using the management interface. */
	WRITE_PCIE_REG(PCIE_BK_MGMT_SEL_LANE, 0xF);

	/*
	 * 7. Wait for stable clocks: SDS_PCS_CLOCK_READY bit in
	 * DWC_GLBL_PLL_MONITOR register of PCIe PHY.
	 */
	for (i = 0; i < PCIE_PHY_RETRIES; i++) {
		reg = dw_pcie_phy_read(PCIE_PHY_DWC_GLBL_PLL_MONITOR);

		if (reg == PCIE_ERROR_VALUE) {
			return ERROR;
		}
		if ((reg & SDS_PCS_CLOCK_READY) == SDS_PCS_CLOCK_READY) {
			break;
		}
	}

	if (i == PCIE_PHY_RETRIES) {
		/* Return an error if the timeout expired. */
		return ERROR;
	}

	/*
	 * 8. Read value of BK_PMU_PCIE_RSTC, reset bits: PIPE_RESET, CORE_RST,
	 * PWR_RST, STICKY_RST, NONSTICKY_RST, HOT_RESET.
	 */
	reg = READ_PMU_REG(BK_PMU_PCIE_RSTC);
	reg &= ~(PMU_PCIE_RSTC_PIPE_RESET | PMU_PCIE_RSTC_CORE_RST | PMU_PCIE_RSTC_PWR_RST |
		 PMU_PCIE_RSTC_STICKY_RST | PMU_PCIE_RSTC_NONSTICKY_RST | PMU_PCIE_RSTC_HOT_RESET);
	WRITE_PMU_REG(BK_PMU_PCIE_RSTC, reg);

	debug("%s: DEV_ID_VEND_ID=0x%x CLASS_CODE_REV_ID=0x%x\n", __FUNCTION__,
		READ_PCIE_REG(PCI_VENDOR_ID),
		READ_PCIE_REG(PCI_CLASS_REVISION));

	/* 6. Set number of lanes. */
	reg = READ_PCIE_REG(PCIE_GEN2_CTRL_OFF);
	reg &= ~NUM_OF_LANES_MASK;
	reg |= (0x4 << NUM_OF_LANES_SHIFT);
	WRITE_PCIE_REG(PCIE_GEN2_CTRL_OFF, reg);

	reg = READ_PCIE_REG(PCIE_PORT_LINK_CTRL_OFF);
	reg &= ~LINK_CAPABLE_MASK;
	reg |= (0x7 << LINK_CAPABLE_SHIFT);
	WRITE_PCIE_REG(PCIE_PORT_LINK_CTRL_OFF, reg);

	/* 7.1 Disable entire DFE */
	reg = dw_pcie_phy_read(PCIE_PHY_DWC_RX_LOOP_CTRL);
	reg |= 0x2;
	dw_pcie_phy_write(PCIE_PHY_DWC_RX_LOOP_CTRL, reg);

	reg = 0x3F;
	dw_pcie_phy_write(PCIE_PHY_DWC_RX_AEQ_VALBBD_2, reg);

	reg = 0;
	dw_pcie_phy_write(PCIE_PHY_DWC_RX_AEQ_VALBBD_1, reg);

	/* Configure bus. */
	reg = READ_PCIE_REG(PCI_PRIMARY_BUS);
	reg &= 0xff000000;	/* keep latency */
	reg |= ((0xff << 16) | ((PCIE_ROOT_BUS_NUM + 1) << 8) | PCIE_ROOT_BUS_NUM);
	WRITE_PCIE_REG(PCI_PRIMARY_BUS, reg);

	/* Setup memory base. */
	reg = ((PCIMEM_BUS_LIMIT & 0xfff00000) | ((PCIMEM_BUS_ADDR & 0xfff00000) >> 16));
	WRITE_PCIE_REG(PCI_MEMORY_BASE, reg);

	/* Setup IO base. */
	reg = (0 << 8) | 0xf0;	/* I/O base is 0, limit 0xffff */
	WRITE_PCIE_REG(PCI_IO_BASE, reg);
	reg = 0;	/* upper base/limit = 0 */
	WRITE_PCIE_REG(PCI_IO_BASE_UPPER16, reg);

	/* 8. Set master for PCIe EP. */
	reg = READ_PCIE_REG(PCI_COMMAND);
	reg |= (PCI_COMMAND_MASTER | PCI_COMMAND_MEMORY | PCI_COMMAND_IO);
	reg |= (PCI_COMMAND_PARITY | PCI_COMMAND_SERR); // Add check error.
	WRITE_PCIE_REG(PCI_COMMAND, reg);

	/* AER */
	reg =  READ_PCIE_REG(PCIE_DEVICE_CONTROL_DEVICE_STATUS);
	reg |= PCI_EXP_DEVCTL_CERE; /* Correctable Error Reporting */
	reg |= PCI_EXP_DEVCTL_NFERE; /* Non-Fatal Error Reporting */
	reg |= PCI_EXP_DEVCTL_FERE;     /* Fatal Error Reporting */
	reg |= PCI_EXP_DEVCTL_URRE;     /* Unsupported Request */
	WRITE_PCIE_REG(PCIE_DEVICE_CONTROL_DEVICE_STATUS, reg);

	/* Unmask Uncorrectable Errors. */
	reg = READ_PCIE_REG(PCIE_UNCORR_ERR_STATUS_OFF);
	WRITE_PCIE_REG(PCIE_UNCORR_ERR_STATUS_OFF, reg);
	WRITE_PCIE_REG(PCIE_UNCORR_ERR_MASK_OFF, 0);

	/* Unmask Correctable Errors. */
	reg = READ_PCIE_REG(PCIE_CORR_ERR_STATUS_OFF);
	WRITE_PCIE_REG(PCIE_CORR_ERR_STATUS_OFF, reg);
	WRITE_PCIE_REG(PCIE_CORR_ERR_MASK_OFF, 0);

	/* 9. Set Inbound/Outbound iATU regions. */

	/* dw_set_iatu_region(dir,  index, base_addr, limit_addr, target_addr, tlp_type) */

	dw_set_iatu_region(REGION_DIR_OUTBOUND, IATU_RD0_INDEX, PHYS_PCI_RD0_BASE_ADDR >> 16,
			   PHYS_PCI_RD0_LIMIT_ADDR >> 16, 0x0000, TLP_TYPE_CFGRD0);

	dw_set_iatu_region(REGION_DIR_OUTBOUND, IATU_MEM_INDEX, PHYS_PCIMEM_BASE_ADDR >> 16,
			   PHYS_PCIMEM_LIMIT_ADDR >> 16, PCIMEM_BUS_ADDR >> 16, TLP_TYPE_MEM);

	dw_set_iatu_region(REGION_DIR_OUTBOUND, IATU_IO_INDEX, PHYS_PCIIO_BASE_ADDR >> 16,
			   PHYS_PCIIO_LIMIT_ADDR >> 16, 0x0000,  TLP_TYPE_IO);

	smp_mb();

	/* Set GEN3/GEN2 speed */
	reg = READ_PCIE_REG(PCIE_LINK_CONTROL2_LINK_STATUS2_REG);
	reg &= ~PCIE_LINK_CONTROL2_GEN_MASK;
#ifdef CONFIG_BAIKAL_T1
	reg |= PCIE_LINK_CONTROL2_GEN3;
#else
	reg |= PCIE_LINK_CONTROL2_GEN2;
#endif
	WRITE_PCIE_REG(PCIE_LINK_CONTROL2_LINK_STATUS2_REG, reg);
	smp_mb();
	
	/* 10. Set LTSSM enable, app_ltssm_enable=0x1 */
	reg = READ_PMU_REG(BK_PMU_PCIE_GENC);
	reg |= PMU_PCIE_GENC_LTSSM_ENABLE;
	WRITE_PMU_REG(BK_PMU_PCIE_GENC, reg);

	/* 11-12 Analyze BK_PMU_PCIE_PMSC */
	for (i = 0; i < PCIE_PHY_RETRIES; i++) {
		reg = READ_PMU_REG(BK_PMU_PCIE_PMSC);
		st = 0;
		if ((reg & PMU_PCIE_PMSC_SMLH_LINKUP) == 0) {
			st |= ERROR_MISMATCH3;
		}

		if ((reg & PMU_PCIE_PMSC_RDLH_LINKUP) == 0) {
			st |= ERROR_MISMATCH4;
		}

		if ((reg & PMU_PCIE_PMSC_LTSSM_STATE_MASK) != LTSSM_L0) {
			st |= ERROR_MISMATCH5;
		}

		if (!st) {
			break;
		}
	}

	if (st)
		printf("%s: PCIe error core = 0x%x\n", __FUNCTION__, st);

	/* Check that GEN3 is set in PCIE_LINK_CONTROL_LINK_STATUS_REG. */
	reg = READ_PCIE_REG(PCIE_LINK_CONTROL_LINK_STATUS_REG);
	reg = ((reg & PCIE_CAP_LINK_SPEED_MASK) >> PCIE_CAP_LINK_SPEED_SHIFT);
	debug("%s: PCIe link speed GEN%d\n", __FUNCTION__, reg);

	smp_mb();

	return st;
}

static int dw_pcibios_read(struct pci_controller *hose, pci_dev_t d,
			   int where, u32 *val)
{
	u32 addr;
	u16 target;

	target = (d >> 8) & 0xffff;

	if (PCI_BUS(d) == PCIE_ROOT_BUS_NUM) {
		if (PCI_DEV(d) != 0) {
			*val = 0xffffffff;
			return -EINVAL;
		}
		addr = PCIE_CFG_BASE;
	} else if (PCI_BUS(d) == (PCIE_ROOT_BUS_NUM + 1)) {
		if (PCI_DEV(d) != 0) {
			*val = 0xffffffff;
			return -EINVAL;
		}
		/* dw_set_iatu_region(dir, index, base_addr, limit_addr, target_addr, tlp_type) */
		dw_set_iatu_region(REGION_DIR_OUTBOUND, IATU_RD0_INDEX, PHYS_PCI_RD0_BASE_ADDR >> 16,
			                 PHYS_PCI_RD0_LIMIT_ADDR >> 16, target, TLP_TYPE_CFGRD0);
		addr = PCI_RD0_BASE_ADDR;
	} else {
		dw_set_iatu_region(REGION_DIR_OUTBOUND, IATU_RD0_INDEX, PHYS_PCI_RD0_BASE_ADDR >> 16,
			                 PHYS_PCI_RD0_LIMIT_ADDR >> 16, target, TLP_TYPE_CFGRD1);
		addr = PCI_RD0_BASE_ADDR;
	}

	smp_mb();

	addr += where & ~0x3;
	*val = readl(addr);

	return 0;
}

static int dw_pcibios_write(struct pci_controller *hose, pci_dev_t d,
			    int where, u32 val)
{
	u32 addr;
	u16 target;

	target = (d >> 8) & 0xffff;

	if (PCI_BUS(d) == PCIE_ROOT_BUS_NUM) {
		if (PCI_DEV(d) != 0)
			return -EINVAL;
		addr = PCIE_CFG_BASE;
	} else if (PCI_BUS(d) == (PCIE_ROOT_BUS_NUM + 1)) {
		if (PCI_DEV(d) != 0)
			return -EINVAL;
		/* dw_set_iatu_region(dir, index, base_addr, limit_addr, target_addr, tlp_type) */
		dw_set_iatu_region(REGION_DIR_OUTBOUND, IATU_RD0_INDEX, PHYS_PCI_RD0_BASE_ADDR >> 16,
				   PHYS_PCI_RD0_LIMIT_ADDR >> 16, target, TLP_TYPE_CFGRD0);
		addr = PCI_RD0_BASE_ADDR;
	} else {
		dw_set_iatu_region(REGION_DIR_OUTBOUND, IATU_RD0_INDEX, PHYS_PCI_RD0_BASE_ADDR >> 16,
				   PHYS_PCI_RD0_LIMIT_ADDR >> 16, target, TLP_TYPE_CFGRD1);
		addr = PCI_RD0_BASE_ADDR;
	}

	smp_mb();

	addr += where & ~0x3;
	writel(val, addr);

	return 0;
}

__weak int board_pci_reset(void)
{
#ifdef CONFIG_PCIE_RST_PIN
	int ret, delay;

	delay = getenv_ulong("pci_delay", 10, 0);
	if (delay > 1000)
		delay = 1000;
	ret = gpio_request(CONFIG_PCIE_RST_PIN, "pcie_rst");
	if (ret) {
		printf("PCIe:  failed to request GPIO %d (ret %d)\n", CONFIG_PCIE_RST_PIN, ret);
		return ret;
	}
	gpio_direction_output(CONFIG_PCIE_RST_PIN, 1);
	mdelay(delay);
#endif

	return 0;
}

/* Probe function. */
void pci_init_board(void)
{
	/* Static instance of the controller. */
	static struct pci_controller    pcc;
	struct pci_controller           *hose = &pcc;
	int rc, region = 0;
	unsigned long bus_addr, phys_addr, size;

	memset(&pcc, 0, sizeof(pcc));

	bus_addr = PCIMEM_BUS_ADDR;
	phys_addr = KSEG1ADDR(PHYS_PCIMEM_BASE_ADDR);
	size = PHYS_PCIMEM_LIMIT_ADDR - PHYS_PCIMEM_BASE_ADDR + 1;

#ifdef CONFIG_PCIPREF_SIZE
	pci_set_region(&hose->regions[region], bus_addr, phys_addr,
			CONFIG_PCIPREF_SIZE, PCI_REGION_PREFETCH);
	bus_addr += CONFIG_PCIPREF_SIZE;
	phys_addr += CONFIG_PCIPREF_SIZE;
	size -= CONFIG_PCIPREF_SIZE;
	region++;
#endif
	/* PCI memory space */
	pci_set_region(&hose->regions[region], bus_addr, phys_addr,
			size, PCI_REGION_MEM);
	region++;

	pci_set_region(&hose->regions[region], 0, KSEG1ADDR(PHYS_PCIIO_BASE_ADDR),
			PHYS_PCIIO_LIMIT_ADDR - PHYS_PCIIO_BASE_ADDR + 1, PCI_REGION_IO);

	hose->region_count = region + 1;

	pci_set_ops(hose, pci_hose_read_config_byte_via_dword,
			  pci_hose_read_config_word_via_dword,
			  dw_pcibios_read,
			  pci_hose_write_config_byte_via_dword,
			  pci_hose_write_config_word_via_dword,
			  dw_pcibios_write);

	/* Release 'reset' from devices (board specific) */
	board_pci_reset();

	/* Start the controller. */
	rc = dw_pcie_init();

	pci_register_hose(hose);
	hose->first_busno = PCIE_ROOT_BUS_NUM;
	if (rc == 0)
		hose->last_busno = pci_hose_scan(hose);
}

int pci_skip_dev(struct pci_controller *hose, pci_dev_t dev)
{
	return 0;
}

#ifdef CONFIG_PCI_SCAN_SHOW
int pci_print_dev(struct pci_controller *hose, pci_dev_t dev)
{
	return 1;
}
#endif /* CONFIG_PCI_SCAN_SHOW */
