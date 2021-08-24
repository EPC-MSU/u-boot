#ifndef	_ASM_ARCH_CLOCK_H_
#define	_ASM_ARCH_CLOCK_H_

enum mxc_clock {
	MXC_ARM_CLK = 0,
	MXC_AHB_CLK,
	MXC_GMII_CLK,
	MXC_SATA_CLK,
};

uint32_t mxc_get_clock(enum mxc_clock clk);

/* Main PLL clocks */
#define BK_PMU_COREPLL_CTL		0x0000
#define BK_PMU_COREPLL_CTL1		0x0004
#define BK_PMU_SATAPLL_CTL		0x0008
#define BK_PMU_SATAPLL_CTL1		0x000C
#define BK_PMU_DDRPLL_CTL		0x0010
#define BK_PMU_DDRPLL_CTL1		0x0014
#define BK_PMU_PCIEPLL_CTL		0x0018
#define BK_PMU_PCIEPLL_CTL1		0x001C
#define BK_PMU_ETHPLL_CTL		0x0020
#define BK_PMU_ETHPLL_CTL1		0x0024
#define BK_PMU_CORE_CTL			0x0028
#define BK_PMU_DDR_CTL			0x002C
enum baikal_clock_pll {
    /* External OSC */
    BK_OSC = 1,
    /* Internal PLL */
    BK_PLL_CORE = 10,
    BK_PLL_SATA,
    BK_PLL_DDR,
    BK_PLL_PCI,
    BK_PLL_ETH,
};

/* Interconnect side clocks */
#define BK_PMU_AXI_XBAR_CTL		0x0030
#define BK_PMU_AXI_DDR_CTL		0x0034
#define BK_PMU_AXI_SATA_CTL		0x0038
#define BK_PMU_AXI_GMAC1_CTL		0x003C
#define BK_PMU_AXI_GMAC2_CTL		0x0040
#define BK_PMU_AXI_XGMAC_CTL		0x0044
#define BK_PMU_AXI_PCIE_M_CTL		0x0048
#define BK_PMU_AXI_PCIE_S_CTL		0x004C
#define BK_PMU_AXI_USB_CTL		0x0050
#define BK_PMU_AXI_BSRAM_CTL		0x0058
enum baikal_clock_axi {
    BK_AXI_XBAR = 20,
    BK_AXI_DDR,
    BK_AXI_SATA,
    BK_AXI_GMAC1,
    BK_AXI_GMAC2,
    BK_AXI_XGMAC,
    BK_AXI_PCIE_M,
    BK_AXI_PCIE_S,
    BK_AXI_USB,
    BK_AXI_BSRAM,
};

/* Peripherial clocks */
#define BK_PMU_PCIE_REF_CTL		0x005C
#define BK_PMU_SATA_REF_CTL		0x0060
#define BK_PMU_APB_CTL			0x0064
#define BK_PMU_GMAC1_CTL		0x0068
#define BK_PMU_GMAC2_CTL		0x006C
#define BK_PMU_XGMAC_CTL		0x0070
#define BK_PMU_USB_CTL			0x0074
#define BK_PMU_WDT_CTL			0x0080
#define BK_PMU_UART_CTL			0x0084
#define BK_PMU_TIMER1_CTL		0x0088
#define BK_PMU_TIMER2_CTL		0x008C
#define BK_PMU_TIMER3_CTL		0x0090
enum baikal_clock_periph {
    BK_CLK_PCI = 40,
    BK_CLK_SATA,
    BK_CLK_APB,
    BK_CLK_GMAC1,
    BK_CLK_GMAC2,
    BK_CLK_XGMAC,
    BK_CLK_USB,
    BK_CLK_WDT,
    BK_CLK_UART,
    BK_CLK_TIMER0,
    BK_CLK_TIMER1,
    BK_CLK_TIMER2,
    BK_CLK_I2C,
    BK_CLK_SPI,
};

/* Clocks and PLL functions */
int  baikal_clock_simple(int id);

#endif /* _ASM_ARCH_CLOCK_H_ */

