/*
 * File:        pll.c
 * Description: PLL infornation and manipulation for Baikal-T
 * Created:     29 Apr 2015
 * Author:      Dmitry Dunaev <dmitry.dunaev@baikalelectronics.ru>
 * Copyright 2015 Baikal Electronics
 *
 */

#include <common.h>
#include <command.h>
#include <asm/io.h>
#include <asm/arch/clock.h>
#include <asm/arch/clock_manager.h>
#include <clk.h>

DECLARE_GLOBAL_DATA_PTR;

static clk_t be_clk[] = {
	{BK_OSC,        BK_OSC,       1, BK_VIRT_OFS, "External OSC", BK_OSC_DATA},
	{BK_PLL_CORE,   BK_OSC,       1, 0x0000,      "Core PLL"},
	{BK_PLL_SATA,   BK_OSC,       1, 0x0008,      "SATA PLL"},
	{BK_PLL_DDR,    BK_OSC,       1, 0x0010,      "DDR PLL"},
	{BK_PLL_PCI,    BK_OSC,       1, 0x0018,      "PCI PLL"},
	{BK_PLL_ETH,    BK_OSC,       1, 0x0020,      "ETH PLL"},
	{0, BK_NONE, 0, BK_VIRT_OFS, ""},
};

/*
static clk_t be_clk_perih[] = {
	{BK_CLK_PCI,    BK_PLL_PCI,   1, 0x005C,      "PCI PHY Clock"},
	{BK_CLK_SATA,   BK_PLL_SATA,  1, 0x0060,      "SATA Clock"},
	{BK_CLK_APB,    BK_PLL_CORE,  1, 0x0064,      "APB Clock"},
	{BK_CLK_GMAC1,  BK_PLL_ETH,   5, 0x0068,      "GMAC1 Clock"},
	{BK_CLK_GMAC2,  BK_PLL_ETH,   5, 0x006C,      "GMAC2 Clock"},
	{BK_CLK_XGMAC,  BK_PLL_ETH,   4, 0x0070,      "XGMAC Clock"},
	{BK_CLK_USB,    BK_PLL_CORE, 10, 0x0074,      "USB Clock"},
	{BK_CLK_WDT,    BK_OSC,      25, 0x0080,      "WDT Clock"},
	{BK_CLK_UART,   BK_PLL_ETH, 400, 0x0084,      "UART Clock"},
	{BK_CLK_TIMER0, BK_OSC,       1, 0x0088,      "Timer0 Clock"},
	{BK_CLK_TIMER1, BK_OSC,       1, 0x008C,      "Timer1 Clock"},
	{BK_CLK_TIMER2, BK_OSC,       1, 0x0090,      "Timer2 Clock"},
	{BK_CLK_I2C,    BK_PLL_ETH,   5, BK_VIRT_OFS, "I2C Clock"},
	{BK_CLK_SPI,    BK_CLK_APB,   1, BK_VIRT_OFS, "SPI Clock"},
	{0, BK_NONE, 0, BK_VIRT_OFS, ""},
};

static clk_t be_clk_axi[] = {
	{0, BK_NONE, 0, BK_VIRT_OFS, ""},
};
*/

static inline unsigned int pll_read(unsigned int ofs)
{
	return __raw_readl(CONFIG_BAIKAL_PLL_BASE + ofs);
}

static inline void pll_write(unsigned int val, unsigned int ofs)
{
	__raw_writel(val, CONFIG_BAIKAL_PLL_BASE + ofs);
}

static inline clk_t * __clk_get(int id)
{
	clk_t *clk;

	for (clk = &be_clk[0]; clk->id && (clk->id != id); clk++);

	return clk->id ? clk : NULL;
}

#define __FL(V) ((V) ? '*' : '.')
static void __clk_print(clk_t *clk, const char *indent)
{
	clk_data_t *cd;

	if (!clk)
		goto __error;

	cd = &clk->data;

	if (baikal_clk_get(clk))
		goto __error;

	printf("%s%02d %-19s ", indent, clk->id, clk->name);

	if (cd->freq < 1000000)
		printf("%4d KHz", (int)(cd->freq / 1000));
	else
		printf("%4d MHz", (int)(cd->freq / 1000000));

	printf("   %2u  %3u  %2u    %c  %c  %c\n", cd->nr, cd->nf, cd->od,
		__FL(cd->bypass), __FL(cd->lock), __FL(cd->enable));

	return;

__error:
	printf("Invalid clock\n");
}

int baikal_clk_get(clk_t *clk)
{
	unsigned int val;
	clk_t *parent;
	clk_data_t *cd;

	if (!clk)
		return -1;

	if (clk->id == BK_OSC)
		return 0;

	cd = &clk->data;

	clk->data.nr = 1;
	clk->data.nf = 1;
	clk->data.od = 1;
	clk->data.lock   = (clk->ofs == BK_VIRT_OFS) ? 1 : 0;
	clk->data.bypass = ((clk->ofs == BK_VIRT_OFS) && (clk->divf == 1)) ? 1 : 0;

	if (clk->ofs != BK_VIRT_OFS) {
		val = pll_read(clk->ofs);
		cd->nr += BE_RD_CLKR(val);
		cd->nf += BE_RD_CLKF(val);
		cd->od += BE_RD_CLKOD(val);
		cd->bypass = val & BE_CLK_BYPASS_MASK;
		cd->lock   = val & BE_CLK_LOCK_MASK;
		cd->enable = baikal_clk_state(clk);
	}

	parent = __clk_get(clk->parent);

	if (baikal_clk_get(parent))
		return -1;

	cd->freq = (parent->data.freq * cd->nf) / cd->nr / cd->od/ clk->divf;

	return 0;
}

int baikal_clk_set(clk_t *clk)
{
	if (!clk)
		return -1;

	puts("Not implemented.\n");
	return 0;
}


int baikal_clk_div(clk_t *clk, unsigned int div)
{
	unsigned int val;

	if (!clk)
		return -1;

	if ((clk->id == BK_OSC) || (clk->ofs == BK_VIRT_OFS))
		return 0;

	val = pll_read(clk->ofs);

	val &= ~BE_CLK_DIV_MASK;
	val |= (div << BE_DIV_SHFT) | BE_CLK_SET_MASK;
	pll_write(val, clk->ofs);

	return 0;
}

int baikal_clk_state(clk_t *clk)
{
	unsigned int val;

	if (clk->id == BK_OSC)
		return 1;

	if (clk->ofs == BK_VIRT_OFS)
		return baikal_clk_state(__clk_get(clk->parent));

	 val = pll_read(clk->ofs);

	return (val & BE_CLK_ENABLE_MASK) ? 1 : 0;
}

void baikal_clk_enable(clk_t *clk)
{
	unsigned int val;

	if (clk->ofs == BK_VIRT_OFS)
		return;

	 val = pll_read(clk->ofs) | BE_CLK_ENABLE_MASK;

	 pll_write(val, clk->ofs);
}

void baikal_clk_disable(clk_t *clk)
{
	unsigned int val;

	if (clk->ofs == BK_VIRT_OFS)
		return;

	 val = pll_read(clk->ofs) & ~BE_CLK_ENABLE_MASK;

	 pll_write(val, clk->ofs);
}


long baikal_pll_get(int id)
{
	unsigned int val, nr, nf, od;
	clk_t *clk = __clk_get(id);

	if (clk->ofs == BK_VIRT_OFS)
		return 0;

	val = pll_read(clk->ofs);

	nr = BE_RD_CLKR(val) + 1;
	nf = BE_RD_CLKF(val) + 1;
	od = BE_RD_CLKOD(val) + 1;

	return (CONFIG_BAIKAL_OSC_CLOCK / nr / od) * nf;
}

void baikal_clock_init(void)
{
	const unsigned int ofs[] = {
		BK_PMU_SATAPLL_CTL,   BK_PMU_AXI_SATA_CTL,
		BK_PMU_AXI_GMAC1_CTL, BK_PMU_AXI_GMAC2_CTL,
		BK_PMU_AXI_XGMAC_CTL, BK_PMU_AXI_USB_CTL,
		BK_PMU_XGMAC_CTL,     BK_PMU_WDT_CTL,
		BK_PMU_SATA_REF_CTL,  BK_PMU_PCIE_REF_CTL,
		0x000, /* Senitel */
	};
	clk_t clk;
	int i;

	/* Turn on AXI/APB clocks */
	for (i = 0; ofs[i]; i++) {
		clk.ofs = ofs[i];
		baikal_clk_enable(&clk);
	}
#ifndef CONFIG_BAIKAL_T1
	/* UART ReInit for engineering Baikal-T CPU. */
	pll_write(0xa4, BK_PMU_UART_CTL);
#endif /* CONFIG_BAIKAL_T1 */
}

int baikal_clock_core(unsigned long freq)
{
	clk_t *clk = __clk_get(BK_PLL_CORE);
	unsigned int val;
	int tout = 1000;

	val = (unsigned int)BE_CORE_PLL_VAL(freq);

	debug("CORE: pll=%dMHz reg=0x%08x\n", (unsigned int)(freq / 1000000), val);

	pll_write(val, clk->ofs);
	sync();

	do {
		__udelay(1000);
		val = pll_read(clk->ofs);;
		tout--;
	} while ((!(val & BE_CLK_LOCK_MASK)) && tout);

	return tout ? 0 : -1;
}

int soc_clk_dump(void)
{
	clk_t *clk;

	puts("\nClocks for " CONFIG_BAIKAL_NAME "\n\n");
	puts("  ID Clock Name            Freq     NR  NF   OD    B  L  E\n"
		 "  --------------------------------------------------------\n");
	for (clk = &be_clk[0]; clk->id; clk++)
		__clk_print(clk, "  ");

	putc('\n');

	return 0;
}

int soc_clk_show(int id)
{
	clk_t *clk = __clk_get(id);

	if (!clk) {
		printf("Invalid clock\n");
		return 1;
	}

	__clk_print(clk, "Clock #");

	return 0;
}

int soc_clk_enable(int id)
{
	clk_t *clk = __clk_get(id);

	if (!clk) {
		printf("Invalid clock\n");
		return 1;
	}

	baikal_clk_enable(clk);

	return 0;
}

int soc_clk_disable(int id)
{
	clk_t *clk = __clk_get(id);

	if (!clk) {
		printf("Invalid clock\n");
		return 1;
	}

	baikal_clk_disable(clk);

	return 0;
}

int soc_clock_set(int id,
	unsigned int od, unsigned int nf, unsigned int nr)
{
	clk_t *clk = __clk_get(id);

	if (!clk) {
		printf("Invalid clock\n");
		return 1;
	}

	baikal_clk_get(clk);

	clk->data.od = od;
	clk->data.nf = nf;
	clk->data.nr = nr;

	return baikal_clk_set(clk);
}
/* End of file pll.c */

