/*
 * Low-level and POST tests.
 *
 * Copyright (C) 2014-2016 Baikal Electronics.
 *
 * Author: Alexey Malahov <Alexey.Malahov@baikalelectronics.com>
 *
 * Description: PMU header file.
 */

#ifndef __PMU_H__
#define __PMU_H__

#include <config.h>

#ifndef __ASSEMBLER__
#include <stdint.h>
extern int llenv32_init_pll(const unsigned int pmu_register);
extern int llenv32_deinit_pll(const unsigned int pmu_register);
extern void llenv_set_axi_ctl(uint32_t pmu_reg, int clkdiv, int en);
#endif /*__ASSEMBLER__ */

/* PLL output frequencies (MHz)*/
#define REFERENCE_FREQ			25

/* Core PLL Control register bits. */
#define PMU_COREPLL_CTL_LOCK		(1 << 31)	/* Synchronized Lock detect. */
#define PMU_COREPLL_CTL_LOCK_SHIFT	(31)
#define PMU_COREPLL_CTL_LOCK_POS	PMU_COREPLL_CTL_LOCK_SHIFT
#define PMU_COREPLL_CTL_LOCK_BITS	1

/* DDR PLL control register bits. */
#define PMU_DDRPLL_CTL_EN		(1 << 0)	/* PLL enable bit. */
#define PMU_DDRPLL_CTL_RST		(1 << 1)	/* PLL reset bit. */
#define PMU_DDRPLL_CTL_RST_POS		1
#define PMU_DDRPLL_CTL_RST_BITS		1
#define PMU_DDRPLL_CTL_LOCK		(1 << 31)	/* Synchronized Lock detect. */
#define PMU_DDRPLL_CTL_LOCK_SHIFT	31
#define PMU_DDRPLL_CTL_LOCK_POS		PMU_DDRPLL_CTL_LOCK_SHIFT
#define PMU_DDRPLL_CTL_LOCK_BITS	1

#define PMU_DDRPLL_CTL_CLKOD_SHIFT	21
#define PMU_DDRPLL_CTL_CLKOD_POS	PMU_DDRPLL_CTL_CLKOD_SHIFT
#define PMU_DDRPLL_CTL_CLKOD_BITS	4
#define PMU_DDRPLL_CTL_CLKOD_VAL	11 /* */

#define PMU_DDRPLL_OD_NF_NR_POS		2
#define PMU_DDRPLL_OD_NF_NR_BITS	23
#define PMU_DDRPLL_1600_OD_NF_NR	( (7 << 19) | (127 << 6) | (0 << 0))
#define PMU_DDRPLL_1333_OD_NF_NR	( (9 << 19) | (399 << 6) | (2 << 0))
#define PMU_DDRPLL_1066_OD_NF_NR	((11 << 19) | (383 << 6) | (2 << 0))

/* DDR control register bits. */
#define PMU_DDR_CTL_EN			(1 << 0)	/* Clock enable for DDR clock */
#define PMU_DDR_CTL_RST			(1 << 1)	/* Software reset for DDR Controller */
#define PMU_DDR_CTL_INIT		(1 << 2)	/* Initialization bit */
#define PMU_DDR_CTL_INIT_SHIFT		2
#define PMU_DDR_CTL_INIT_POS		PMU_DDR_CTL_INIT_SHIFT
#define PMU_DDR_CTL_INIT_BITS		1


/* AXI DDR Control register bits. */
#define PMU_AXI_DDR_CTL_EN		(1 << 0)	/* Enable AXI DDR clock. */
#define PMU_AXI_DDR_CTL_INIT		(1 << 8)	/* Initialization bit. When this bit is active, */
#define PMU_AXI_DDR_CTL_INIT_SHIFT	(8)		/* the DDR remains in the reset state and can be configured. */
#define PMU_AXI_DDR_CTL_INIT_POS	PMU_AXI_DDR_CTL_INIT_SHIFT
#define PMU_AXI_DDR_CTL_INIT_BITS	1

/* BK_PMU_AXI_BSRAM_CTL */
#define PMU_AXI_BSRAM_CTL_EN		(1 << 0)	/* Enable AXI clock. */
#define PMU_AXI_BSRAM_CTL_RST		(1 << 1)	/* Software AXI clock domain reset. */

/* BK_PMU_AXI_CRYPTO_CTL */
#define PMU_AXI_CRYPTO_CTL_EN		(1 << 0)	/* Enable AXI clock. */
#define PMU_AXI_CRYPTO_CTL_RST		(1 << 1)	/* Software AXI Crypto clock domain reset. */

/* APB divider register bits */
#define PMU_APB_CTL_CLKDIV_SHIFT	4
#define PMU_APB_CTL_CLKDIV_POS		PMU_APB_CTL_CLKDIV_SHIFT
#define PMU_APB_CTL_CLKDIV_BITS		5
#define PMU_APB_CTL_CLKDIV_MASK		0x1F0


/* BK_PMU_AXI_PCIE_M_CTL */
#define PMU_AXI_PCIE_M_CTL_EN		(1 << 0)	/* Enable AXI PCIe Master clock. */
#define PMU_AXI_PCIE_M_CTL_RST		(1 << 1)	/* Software AXI PCIe Master clock domain reset. */

/* BK_PMU_AXI_PCIE_S_CTL */
#define PMU_AXI_PCIE_S_CTL_EN		(1 << 0)	/* Enable AXI PCIe Slave clock. */
#define PMU_AXI_PCIE_S_CTL_RST		(1 << 1)	/* Software AXI PCIe Slave clock domain reset. */

/* BK_PMU_PCIE_RSTC */
#define PMU_PCIE_RSTC_PHY_RESET		(1 << 0)	/* PCIe PHY phy_rts_n reset control bit. */
#define PMU_PCIE_RSTC_PIPE_RESET	(1 << 4)	/* PCIe PHY PCS pipe_reset_n reset control bit. */
#define PMU_PCIE_RSTC_CORE_RST		(1 << 8)	/* PCIe core core_rst_n reset control bit. */
#define PMU_PCIE_RSTC_PWR_RST		(1 << 9)	/* PCIe core pwr_rst_n reset control bit. */
#define PMU_PCIE_RSTC_STICKY_RST	(1 << 10)	/* PCIe core sticky_rst_n reset control bit. */
#define PMU_PCIE_RSTC_NONSTICKY_RST	(1 << 11)	/* PCIe core nonsticky_rst_n reset control bit. */
#define PMU_PCIE_RSTC_HOT_RESET		(1 << 12)	/* Hot Reset control bit. */
#define PMU_PCIE_RSTC_REQ_RESET		(1 << 13)	/* PCIe core link_req_rst_not ready for reset signal status bit */
#define PMU_PCIE_RSTC_BRIDGE_FLUSH	(1 << 19)	/* PCIe AXI bridge bridge_flush_not signal status bit. */

/* BK_PMU_PCIE_GENC */
#define PMU_PCIE_GENC_LTSSM_ENABLE	(1 << 1)	/* LTSSM enable bit. */
#define PMU_PCIE_GENC_DBI2_MODE		(1 << 2)	/* PCIe core registers access mode bit: DBI(=0) / DBI2(=1) */
#define PMU_PCIE_GENC_MGMT_ENABLE	(1 << 3)	/* PCIe PHY management interface enable bit. */

/* BK_PMU_PCIE_PMSC */
#define PMU_PCIE_PMSC_LTSSM_STATE_SHIFT	(0)		/* LTSSM state (smlh_ltssm_state[5:0] signal) */
#define PMU_PCIE_PMSC_LTSSM_STATE_MASK	(0x3F)
#define LTSSM_L0			0x11
#define PMU_PCIE_PMSC_SMLH_LINKUP	(1 << 6)	/* Physical level (PL) state bit (smlh_link_up signal) */
#define PMU_PCIE_PMSC_RDLH_LINKUP	(1 << 7)	/* Channel level (DLL) state bit (rdlh_link_up signal) */

/* Macros to read/write PMU registers. */
#define READ_PMU_REG(r)		READ_MEMORY_REG(r)
#define WRITE_PMU_REG(r, v)	WRITE_MEMORY_REG(r, v)

/* Register map */

#define BK_COREPLL_CTL_OFFSET		0x000
#define BK_COREPLL_CTL1_OFFSET		0x004
#define BK_SATAPLL_CTL_OFFSET		0x008
#define BK_SATAPLL_CTL1_OFFSET		0x00C
#define BK_DDRPLL_CTL_OFFSET		0x010
#define BK_DDRPLL_CTL1_OFFSET		0x014
#define BK_PCIEPLL_CTL_OFFSET		0x018
#define BK_PCIEPLL_CTL1_OFFSET		0x01C
#define BK_ETHPLL_CTL_OFFSET		0x020
#define BK_ETHPLL_CTL1_OFFSET		0x024
#define BK_CORE_CTL_OFFSET		0x028
#define BK_DDR_CTL_OFFSET		0x02C
#define BK_AXI_XBAR_CTL_OFFSET		0x030
#define BK_AXI_DDR_CTL_OFFSET 		0x034
#define BK_AXI_SATA_CTL_OFFSET		0x038
#define BK_AXI_GMAC1_CTL_OFFSET		0x03C
#define BK_AXI_GMAC2_CTL_OFFSET		0x040
#define BK_AXI_XGMAC_CTL_OFFSET		0x044
#define BK_AXI_PCIE_M_CTL_OFFSET	0x048
#define BK_AXI_PCIE_S_CTL_OFFSET	0x04C
#define BK_AXI_USB_CTL_OFFSET		0x050
#define BK_AXI_CRYPTO_CTL_OFFSET	0x054
#define BK_AXI_BSRAM_CTL_OFFSET		0x058
#define BK_SATA_REF_CTL_OFFSET		0x060
#define BK_APB_CTL_OFFSET		0x064
#define BK_GMAC1_CTL_OFFSET		0x068
#define BK_GMAC2_CTL_OFFSET		0x06C
#define BK_XGMAC_CTL_OFFSET		0x070
#define BK_USB_CTL_OFFSET		0x074
#define BK_PVT_CTL_OFFSET		0x078
#define BK_CRYPTO_CTL_OFFSET		0x07C
#define BK_WDT_CTL_OFFSET		0x080
#define BK_UART_CTL_OFFSET		0x084
#define BK_TIMER1_CTL_OFFSET		0x088
#define BK_TIMER2_CTL_OFFSET		0x08C
#define BK_TIMER3_CTL_OFFSET		0x090
#define BK_PMU_I2C_CTL_OFFSET		0x100
#define BK_PMU_I2C_DI_OFFSET		0x104
#define BK_PMU_I2C_DO_OFFSET		0x108
#define BK_PLL_STS_OFFSET		0x10C
#define BK_CORE_WERRL_OFFSET		0x110
#define BK_CORE_WERRH_OFFSET		0x114
#define BK_CORE_DEBUG_OFFSET		0x118
#define BK_PCIE_CLKC_OFFSET		0x140
#define BK_PCIE_RSTC_OFFSET		0x144
#define BK_PCIE_PMSC_OFFSET		0x148
#define BK_PCIE_GENC_OFFSET		0x14C


#define BK_PMU_COREPLL_CTL	(CONFIG_BAIKAL_PLL_BASE + BK_COREPLL_CTL_OFFSET)
#define BK_PMU_COREPLL_CTL1	(CONFIG_BAIKAL_PLL_BASE + BK_COREPLL_CTL1_OFFSET)
#define BK_PMU_SATAPLL_CTL	(CONFIG_BAIKAL_PLL_BASE + BK_SATAPLL_CTL_OFFSET)
#define BK_PMU_SATAPLL_CTL1	(CONFIG_BAIKAL_PLL_BASE + BK_SATAPLL_CTL1_OFFSET)
#define BK_PMU_DDRPLL_CTL	(CONFIG_BAIKAL_PLL_BASE + BK_DDRPLL_CTL_OFFSET)
#define BK_PMU_DDRPLL_CTL1	(CONFIG_BAIKAL_PLL_BASE + BK_DDRPLL_CTL1_OFFSET)
#define BK_PMU_PCIEPLL_CTL	(CONFIG_BAIKAL_PLL_BASE + BK_PCIEPLL_CTL_OFFSET)
#define BK_PMU_PCIEPLL_CTL1	(CONFIG_BAIKAL_PLL_BASE + BK_PCIEPLL_CTL1_OFFSET)
#define BK_PMU_ETHPLL_CTL	(CONFIG_BAIKAL_PLL_BASE + BK_ETHPLL_CTL_OFFSET)
#define BK_PMU_ETHPLL_CTL1	(CONFIG_BAIKAL_PLL_BASE + BK_ETHPLL_CTL1_OFFSET)
#define BK_PMU_CORE_CTL		(CONFIG_BAIKAL_PLL_BASE + BK_CORE_CTL_OFFSET)
#define BK_PMU_DDR_CTL		(CONFIG_BAIKAL_PLL_BASE + BK_DDR_CTL_OFFSET)
#define BK_PMU_AXI_XBAR_CTL	(CONFIG_BAIKAL_PLL_BASE + BK_AXI_XBAR_CTL_OFFSET)
#define BK_PMU_AXI_DDR_CTL	(CONFIG_BAIKAL_PLL_BASE + BK_AXI_DDR_CTL_OFFSET)
#define BK_PMU_AXI_SATA_CTL	(CONFIG_BAIKAL_PLL_BASE + BK_AXI_SATA_CTL_OFFSET)
#define BK_PMU_AXI_GMAC1_CTL	(CONFIG_BAIKAL_PLL_BASE + BK_AXI_GMAC1_CTL_OFFSET)
#define BK_PMU_AXI_GMAC2_CTL	(CONFIG_BAIKAL_PLL_BASE + BK_AXI_GMAC2_CTL_OFFSET)
#define BK_PMU_AXI_XGMAC_CTL	(CONFIG_BAIKAL_PLL_BASE + BK_AXI_XGMAC_CTL_OFFSET)
#define BK_PMU_AXI_PCIE_M_CTL	(CONFIG_BAIKAL_PLL_BASE + BK_AXI_PCIE_M_CTL_OFFSET)
#define BK_PMU_AXI_PCIE_S_CTL	(CONFIG_BAIKAL_PLL_BASE + BK_AXI_PCIE_S_CTL_OFFSET)
#define BK_PMU_AXI_USB_CTL	(CONFIG_BAIKAL_PLL_BASE + BK_AXI_USB_CTL_OFFSET)
#define BK_PMU_AXI_CRYPTO_CTL	(CONFIG_BAIKAL_PLL_BASE + BK_AXI_CRYPTO_CTL_OFFSET)
#define BK_PMU_AXI_BSRAM_CTL	(CONFIG_BAIKAL_PLL_BASE + BK_AXI_BSRAM_CTL_OFFSET)
#define BK_PMU_SATA_REF_CTL	(CONFIG_BAIKAL_PLL_BASE + BK_SATA_REF_CTL_OFFSET)
#define BK_PMU_APB_CTL		(CONFIG_BAIKAL_PLL_BASE + BK_APB_CTL_OFFSET)
#define BK_PMU_GMAC1_CTL	(CONFIG_BAIKAL_PLL_BASE + BK_GMAC1_CTL_OFFSET)
#define BK_PMU_GMAC2_CTL	(CONFIG_BAIKAL_PLL_BASE + BK_GMAC2_CTL_OFFSET)
#define BK_PMU_XGMAC_CTL	(CONFIG_BAIKAL_PLL_BASE + BK_XGMAC_CTL_OFFSET)
#define BK_PMU_USB_CTL		(CONFIG_BAIKAL_PLL_BASE + BK_USB_CTL_OFFSET)
#define BK_PMU_PVT_CTL		(CONFIG_BAIKAL_PLL_BASE + BK_PVT_CTL_OFFSET)
#define BK_PMU_CRYPTO_CTL	(CONFIG_BAIKAL_PLL_BASE + BK_CRYPTO_CTL_OFFSET)
#define BK_PMU_WDT_CTL		(CONFIG_BAIKAL_PLL_BASE + BK_WDT_CTL_OFFSET)
#define BK_PMU_UART_CTL		(CONFIG_BAIKAL_PLL_BASE + BK_UART_CTL_OFFSET)
#define BK_PMU_TIMER1_CTL	(CONFIG_BAIKAL_PLL_BASE + BK_TIMER1_CTL_OFFSET)
#define BK_PMU_TIMER2_CTL	(CONFIG_BAIKAL_PLL_BASE + BK_TIMER2_CTL_OFFSET)
#define BK_PMU_TIMER3_CTL	(CONFIG_BAIKAL_PLL_BASE + BK_TIMER3_CTL_OFFSET)
#define BK_PMU_PMU_I2C_CTL	(CONFIG_BAIKAL_PLL_BASE + BK_PMU_I2C_CTL_OFFSET)
#define BK_PMU_PMU_I2C_DI	(CONFIG_BAIKAL_PLL_BASE + BK_PMU_I2C_DI_OFFSET)
#define BK_PMU_PMU_I2C_DO	(CONFIG_BAIKAL_PLL_BASE + BK_PMU_I2C_DO_OFFSET)
#define BK_PMU_PLL_STS		(CONFIG_BAIKAL_PLL_BASE + BK_PLL_STS_OFFSET)
#define BK_PMU_CORE_WERRL	(CONFIG_BAIKAL_PLL_BASE + BK_CORE_WERRL_OFFSET)
#define BK_PMU_CORE_WERRH	(CONFIG_BAIKAL_PLL_BASE + BK_CORE_WERRH_OFFSET)
#define BK_PMU_CORE_DEBUG	(CONFIG_BAIKAL_PLL_BASE + BK_CORE_DEBUG_OFFSET)
#define BK_PMU_PCIE_CLKC	(CONFIG_BAIKAL_PLL_BASE + BK_PCIE_CLKC_OFFSET)
#define BK_PMU_PCIE_RSTC	(CONFIG_BAIKAL_PLL_BASE + BK_PCIE_RSTC_OFFSET)
#define BK_PMU_PCIE_PMSC	(CONFIG_BAIKAL_PLL_BASE + BK_PCIE_PMSC_OFFSET)
#define BK_PMU_PCIE_GENC	(CONFIG_BAIKAL_PLL_BASE + BK_PCIE_GENC_OFFSET)


#define BK_PMU_COREPLL_CTL_VAL		0x80605f01
#define BK_PMU_COREPLL_CTL1_VAL		0x17a
#define BK_PMU_SATAPLL_CTL_VAL		0x205700
#define BK_PMU_SATAPLL_CTL1_VAL		0x15a
#define BK_PMU_DDRPLL_CTL_VAL		0xe07f00
#define BK_PMU_DDRPLL_CTL1_VAL		0x1fa
#define BK_PMU_PCIEPLL_CTL_VAL		0x204f00
#define BK_PMU_PCIEPLL_CTL1_VAL		0x13a
#define BK_PMU_ETHPLL_CTL_VAL		0x606300
#define BK_PMU_ETHPLL_CTL1_VAL		0x18a
#define BK_PMU_CORE_CTL_VAL		0xC0004040
#define BK_PMU_DDR_CTL_VAL		0x0
#define BK_PMU_AXI_XBAR_CTL_VAL		0x80000021
#define BK_PMU_AXI_DDR_CTL_VAL		0x80000021
#define BK_PMU_AXI_SATA_CTL_VAL		0x80000050
#define BK_PMU_AXI_GMAC1_CTL_VAL	0x80000040
#define BK_PMU_AXI_GMAC2_CTL_VAL	0x80000040
#define BK_PMU_AXI_XGMAC_CTL_VAL	0x80000020
#define BK_PMU_AXI_PCIE_M_CTL_VAL	0x80000030
#define BK_PMU_AXI_PCIE_S_CTL_VAL	0x80000030
#define BK_PMU_AXI_USB_CTL_VAL		0x800000a0
#define BK_PMU_AXI_CRYPTO_CTL_VAL	0x80000020
#define BK_PMU_AXI_BSRAM_CTL_VAL	0x80000081
#define BK_PMU_PCIE_REF_CTL_VAL		0xa0
#define BK_PMU_SATA_REF_CTL_VAL		0xb0
#define BK_PMU_APB_CTL_VAL		0x180
#define BK_PMU_GMAC1_CTL_VAL		0x1
#define BK_PMU_GMAC2_CTL_VAL		0x1
#define BK_PMU_XGMAC_CTL_VAL		0x1
#define BK_PMU_USB_CTL_VAL		0x1
#define BK_PMU_PVT_CTL_VAL		0x140
#define BK_PMU_CRYPTO_CTL_VAL 		0x20
#define BK_PMU_WDT_CTL_VAL		0x1
#define BK_PMU_UART_CTL_VAL		0xa0
#define BK_PMU_TIMER1_CTL_VAL		0x11
#define BK_PMU_TIMER2_CTL_VAL		0x11
#define BK_PMU_TIMER3_CTL_VAL		0x11
#define BK_PMU_PMU_I2C_CTL_VAL		0x0
#define BK_PMU_PMU_I2C_DI_VAL		0x0
#define BK_PMU_PMU_I2C_DO_VAL		0x0
#define BK_PMU_PLL_STS_VAL		0x0
#define BK_PMU_CORE_WERRL_VAL		0x0
#define BK_PMU_CORE_WERRH_VAL		0x0
#define BK_PMU_CORE_DEBUG_VAL		0x400
#define BK_PMU_PCIE_CLKC_VAL		0x30000
#define BK_PMU_PCIE_RSTC_VAL		0x82F11
#define BK_PMU_PCIE_PMSC_VAL		0x0
#define BK_PMU_PCIE_GENC_VAL		0x0

#define BK_PMU_LOCK_BIT			(1 << 31)
#define BK_PMU_EN_BIT			(1 << 0)
#define BK_PMU_OFF_BIT			(0 << 0)
#define BK_PMU_RST_BIT			(1 << 1)
#define BK_PMU_INIT_BIT			(1 << 2)
#define BK_PMU_AXI_SEL_BIT		(1 << 3) /* 0 - COREPLL; 1- SATAPLL */
#define BK_PMU_SET_CLKDIV_BIT		(1 << 2)
#define BK_PMU_CLKDIV_SHIFT		4
#define BK_PMU_APB_CLKDIV_MASK		(0x1F << BK_PMU_CLKDIV_SHIFT)
#define BK_PMU_AXI_CLKDIV_MASK		(0xF << BK_PMU_CLKDIV_SHIFT)
#define BK_PMU_CLKOD_SHIFT		21
#define BK_PMU_CLKOD_MASK		(0xF << BK_PMU_CLKOD_SHIFT)
#define BK_PMU_CLKOD_BITS		4
#define BK_PMU_CLKF_SHIFT		8
#define BK_PMU_CLKF_MASK		(0x1FFF << BK_PMU_CLKF_SHIFT)
#define BK_PMU_CLKF_BITS		13
#define BK_PMU_CLKR_SHIFT		2
#define BK_PMU_CLKR_MASK		(0x3F << BK_PMU_CLKR_SHIFT)
#define BK_PMU_CLKR_BITS		6
#define BK_PMU_I2C_GO			(1 << 31)
#define BK_PMU_I2C_RW			(1 << 8)
/* BK_PMU_I2C_RW sets PMU I2C read/write mode
 * 0 - PMU_I2C_CTL register in read mode
 * 1 - PMU_I2C_CTL register in write mode */
#define BK_PMU_CLKOD_2		1
#define BK_PMU_CLKOD_4		3
#define BK_PMU_CLKOD_6		5
#define BK_PMU_CLKF_128		127
#define BK_PMU_CLKF_112		111
#define BK_PMU_CLKF_50		49
#define BK_PMU_AXI_CLKDIV_2	2
#define BK_PMU_AXI_CLKDIV_3	3
#define BK_PMU_APB_CLKDIV_20	20


/* PMU settings for HAPS  */
#define HAPS_BK_PMU_SATAPLL_CTL_EN_VAL			1
#define HAPS_BK_PMU_DDRPLL_CTL_EN_VAL			1
#define HAPS_BK_PMU_ETHPLL_CTL_EN_VAL			1
#define HAPS_BK_PMU_PCIEPLL_CTL_EN_VAL			1
#define HAPS_BK_PMU_DDR_CTL_EN_VAL			1
#define HAPS_BK_PMU_AXI_XBAR_CTL_CLKDIV_VAL		1
#define HAPS_BK_PMU_AXI_DDR_CTL_CLKDIV_VAL		1
#define HAPS_BK_PMU_AXI_SATA_CTL_EN_VAL			1
#define HAPS_BK_PMU_AXI_SATA_CTL_CLKDIV_VAL		4
#define HAPS_BK_PMU_AXI_GMAC1_CTL_EN_VAL		1
#define HAPS_BK_PMU_AXI_GMAC1_CTL_CLKDIV_VAL		5
#define HAPS_BK_PMU_AXI_GMAC2_CTL_EN_VAL		1
#define HAPS_BK_PMU_AXI_GMAC2_CTL_CLKDIV_VAL		5
#define HAPS_BK_PMU_AXI_USB_CTL_EN_VAL			1
#define HAPS_BK_PMU_AXI_USB_CTL_CLKDIV_VAL		1
#define HAPS_BK_PMU_AXI_CRYPTO_CTL_EN_VAL		1
#define HAPS_BK_PMU_AXI_CRYPTO_CTL_CLKDIV_VAL		1
#define HAPS_BK_PMU_AXI_BSRAM_CTL_CLKDIV_VAL		1
#define HAPS_BK_PMU_AXI_PCIE_M_CTL_EN_VAL		1
#define HAPS_BK_PMU_AXI_PCIE_M_CTL_CLKDIV_VAL		4
#define HAPS_BK_PMU_AXI_PCIE_S_CTL_EN_VAL		1
#define HAPS_BK_PMU_AXI_PCIE_S_CTL_CLKDIV_VAL		4
#define HAPS_BK_PMU_PCIE_REF_CTL_CLKDIV_VAL		1
#define HAPS_BK_PMU_SATA_REF_CTL_CLKDIV_VAL		1
#define HAPS_BK_PMU_APB_CTL_CLKDIV_VAL			2 /* According to BT_HAPS_Init_sequence_v2.odt */
#define HAPS_BK_PMU_PVT_CTL_CLKDIV_VAL			1 /* TBD: to be precise. */
#define HAPS_BK_PMU_CRYPTO_CTL_CLKDIV_VAL		1


/* BK_PMU_CRYPTO_DMA_CTL */
#define PMU_SET_CLKDIV			(1 << 2)
#define PMU_CLKDIV_SHIFT		(4)
#define PMU_CLKDIV_POS			PMU_CLKDIV_SHIFT
#define PMU_CLKDIV_BITS			4
#define PMU_CLKDIV_MASK			(0xF0)
#define PMU_CRYPTO_DMA_CTL_SET_CLKDIV	PMU_SET_CLKDIV
#define PMU_CRYPTO_DMA_CTL_CLKDIV_SHIFT	PMU_CLKDIV_SHIFT


/* Map of offsets for PMU_I2C registers */
#define BK_PMU_IC_CON			0x0 /* I2C Control  */
#define BK_PMU_IC_TAR			0x4 /* I2C Target Address  */
#define BK_PMU_IC_SAR			0x8 /* I2C Slave Address  */
#define BK_PMU_IC_HS_MADDR		0xc /* I2C High Speed Master Mode Code Address */
#define BK_PMU_IC_DATA_CMD		0x10 /* I2C Rx/Tx Data Buffer and Command Register */
#define BK_PMU_IC_SS_SCL_HCNT		0x14 /* Standard Speed I2C Clock SCL High Count */
#define BK_PMU_IC_SS_SCL_LCNT		0x18 /* Standard Speed I2C Clock SCL Low Count */
#define BK_PMU_IC_FS_SCL_HCNT		0x1c /* Fast Speed I2C Clock SCL High Count */
#define BK_PMU_IC_FS_SCL_LCNT		0x20 /* Fast Speed I2C Clock SCL Low Count */
#define BK_PMU_IC_HS_SCL_HCNT		0x24 /* High Speed I2C Clock SCL High Count */
#define BK_PMU_IC_HS_SCL_LCNT		0x28 /* High Speed I2C Clock SCL Low Count */
#define BK_PMU_IC_INTR_STAT		0x2c /* I2C Interrupt Status  */
#define BK_PMU_IC_INTR_MASK		0x30 /* I2C Interrupt Mask  */
#define BK_PMU_IC_RAW_INTR_STAT		0x34 /* I2C Raw Interrupt Status  */
#define BK_PMU_IC_RX_TL			0x38 /* I2C Receive FIFO Threshold  */
#define BK_PMU_IC_TX_TL			0x3c /* I2C Transmit FIFO Threshold  */
#define BK_PMU_IC_CLR_INTR		0x40 /* Clear Combined and Individual Interrupt */
#define BK_PMU_IC_CLR_RX_UNDER		0x44 /* Clear RX_UNDER Interrupt  */
#define BK_PMU_IC_CLR_RX_OVER		0x48 /* Clear RX_OVER Interrupt  */
#define BK_PMU_IC_CLR_TX_OVER		0x4c /* Clear TX_OVER Interrupt  */
#define BK_PMU_IC_CLR_RD_REQ		0x50 /* Clear RD_REQ Interrupt  */
#define BK_PMU_IC_CLR_TX_ABRT		0x54 /* Clear TX_ABRT Interrupt  */
#define BK_PMU_IC_CLR_RX_DONE		0x58 /* Clear RX_DONE Interrupt  */
#define BK_PMU_IC_CLR_ACTIVITY		0x5c /* Clear ACTIVITY Interrupt  */
#define BK_PMU_IC_CLR_STOP_DET		0x60 /* Clear STOP_DET Interrupt  */
#define BK_PMU_IC_CLR_START_DET		0x64 /* Clear START_DET Interrupt  */
#define BK_PMU_IC_CLR_GEN_CALL		0x68 /* Clear GEN_CALL Interrupt  */
#define BK_PMU_IC_ENABLE		0x6c /* I2C Enable  */
#define BK_PMU_IC_STATUS		0x70 /* I2C Status  */
#define BK_PMU_IC_TXFLR			0x74 /* I2C Transmit FIFO Level  */
#define BK_PMU_IC_RXFLR			0x78 /* I2C Receive FIFO Level  */
#define BK_PMU_IC_SDA_HOLD		0x7c /* I2C SDA Hold  */
#define BK_PMU_IC_TX_ABRT_SOURCE	0x80 /* I2C Transmit Abort Source  */
#define BK_PMU_IC_DMA_CR		0x88 /* DMA Control  */
#define BK_PMU_IC_DMA_TDLR		0x8c /* DMA Transmit Data Level  */
#define BK_PMU_IC_DMA_RDLR		0x90 /* I2C Receive Data Level  */
#define BK_PMU_IC_SDA_SETUP		0x94 /* I2C SDA Setup  */
#define BK_PMU_IC_ACK_GENERAL_CALL	0x98 /* I2C ACK General Call  */
#define BK_PMU_IC_ENABLE_STATUS		0x9c /* I2C Enable Status  */
#define BK_PMU_IC_FS_SPKLEN		0xa0 /* I2C SS and FS spike suppression limit  */
#define BK_PMU_IC_HS_SPKLEN		0xa4 /* I2C HS spike suppression limit  */
#define BK_PMU_IC_COMP_PARAM_1		0xf4 /* Component Parameter  */
#define BK_PMU_IC_COMP_VERSION		0xf8 /* I2C Component Version  */
#define BK_PMU_IC_COMP_TYPE		0xfc /* I2C Component Type  */


/* BK_PMU_UART_CTL bits */
#define BK_PMU_UART_CTL_SET_CLKDIV	(1 << 2)
#define BK_PMU_UART_CTL_CLKDIV_POS	4
#define BK_PMU_UART_CTL_CLKDIV_BITS	4
#define BK_PMU_UART_CTL_CLKDIV_1200	5

#endif /* __PMU_H__ */
