/*
 * File:        clock_manager.h
 * Description: PLL infornation and manipulation for Baikal-T
 * Created:     01 Jun 2015
 * Author:      Dmitry Dunaev
 * Copyright 2015 Baikal Electronics
 *
 */

#ifndef _ASM_CLOCK_MANAGER_H
#define _ASM_CLOCK_MANAGER_H


/* Common macros */

#define HZ_TO_MHZ(HZ)       (HZ / 1000000)
#define HZ_TO_KHZ(HZ)       (HZ / 1000)
#define KHZ_TO_MHZ(HZ)      (HZ / 1000)
#define KHZ_TO_HZ(HZ)       (HZ * 1000)
#define MHZ_TO_HZ(HZ)       (HZ * 1000000)
#define MHZ_TO_KHZ(HZ)      (HZ * 1000)

#ifndef CONFIG_BAIKAL_OSC_CLOCK
#define CONFIG_BAIKAL_OSC_CLOCK 25000000 /* 25 MHz */
#endif

#define BE_CLK_ENABLE_BIT		0
#define BE_CLK_RESET_BIT		1
#define BE_CLK_SET_BIT			2
#define BE_CLK_BYPASS_BIT		30
#define BE_CLK_LOCK_BIT			31

#define BE_CLK_ENABLE_MASK		(1 << BE_CLK_ENABLE_BIT)
#define BE_CLK_RESET_MASK		(1 << BE_CLK_RESET_BIT)
#define BE_CLK_SET_MASK			(1 << BE_CLK_SET_BIT)
#define BE_CLK_BYPASS_MASK		(1 << BE_CLK_BYPASS_BIT)
#define BE_CLK_LOCK_MASK		(1 << BE_CLK_LOCK_BIT)

#define BE_CLKR_SHFT			2
#define BE_DIV_SHFT			4
#define BE_CLKF_SHFT			8
#define BE_CLKOD_SHFT			21

#define BE_CLKR_VAL(NR)			((NR - 1) << BE_CLKR_SHFT)
#define BE_CLKF_VAL(NF)			((NF - 1) << BE_CLKF_SHFT)
#define BE_CLKOD_VAL(OD)		((OD - 1) << BE_CLKOD_SHFT)

#define BE_PLL_CLK_VAL(NR, NF, OD)	\
	(BE_CLK_ENABLE_MASK | BE_CLK_RESET_MASK | \
	BE_CLKR_VAL(NR) | BE_CLKF_VAL(NF) | BE_CLKOD_VAL(OD))

#define BE_CORE_PLL_MUL(F)	((F / CONFIG_BAIKAL_OSC_CLOCK) & 0x7f)
#define BE_CORE_PLL_OD(F)	(((68 / BE_CORE_PLL_MUL(F)) << 1) & 0x3f)
#define BE_CORE_PLL_NF(F)	((BE_CORE_PLL_MUL(F) * BE_CORE_PLL_OD(F)) & 0x1fff)
#define BE_CORE_PLL_VAL(F)	BE_PLL_CLK_VAL(1, BE_CORE_PLL_NF(F), BE_CORE_PLL_OD(F))

#define BE_CORE_PLL_1200MHZ		BE_PLL_CLK_VAL(1, 96, 2)
#define BE_CORE_PLL_1100MHZ		BE_PLL_CLK_VAL(1, 88, 2)
#define BE_CORE_PLL_1000MHZ		BE_PLL_CLK_VAL(1, 80, 2)
#define BE_CORE_PLL_900MHZ		BE_PLL_CLK_VAL(1, 72, 2)
#define BE_CORE_PLL_800MHZ		BE_PLL_CLK_VAL(1, 128, 4)
#define BE_CORE_PLL_700MHZ		BE_PLL_CLK_VAL(1, 112, 4)
#define BE_CORE_PLL_600MHZ		BE_PLL_CLK_VAL(1, 96, 4)

#if CONFIG_SYS_CPU_CLOCK_RATE > 1500000000
# error "Core PLL clock higher than 1500 MHz is not supported"
#elif CONFIG_SYS_CPU_CLOCK_RATE < 100000000
# error "Core PLL clock less than 100 MHz is not supported"
#else
#define BE_CORE_PLL BE_CORE_PLL_VAL(CONFIG_SYS_CPU_CLOCK_RATE)
#endif

#define BE_CLK_DIV_MAX_WIDTH	17
#define BE_CLK_DIV_MASK		(((1 << BE_CLK_DIV_MAX_WIDTH) - 1) << BE_DIV_SHFT)
#define BE_CLK_PARAM_MASK	(0x3FFFFFFC)

#define BE_RD_CLKR(src)			(((src) & 0x000000fc) >> BE_CLKR_SHFT)
#define BE_RD_CLKF(src)			(((src) & 0x001FFF00) >> BE_CLKF_SHFT)
#define BE_RD_CLKOD(src)		(((src) & 0x01E00000) >> BE_CLKOD_SHFT)

#ifndef __ASSEMBLER__

#define BK_VIRT_OFS	0x10000
#define BK_NONE		0x00000
#define BK_OSC_DATA {CONFIG_BAIKAL_OSC_CLOCK, 1, 1, 1, 1, 1, 1}

typedef struct {
    unsigned long freq;
    unsigned int  nr;
    unsigned int  nf;
    unsigned int  od;
    unsigned int  enable;
    unsigned int  bypass;
    unsigned int  lock;
} clk_data_t;

typedef struct {
    int id;
    int parent;
    unsigned int divf;
    unsigned int ofs;
    char name[16];
    clk_data_t data;
} clk_t;

long baikal_pll_get(int id);

int baikal_clk_get(clk_t *clk);
int baikal_clk_set(clk_t *clk);
int baikal_clk_div(clk_t *clk, unsigned int div);
int baikal_clk_state(clk_t *clk);

void baikal_clk_enable(clk_t *clk);
void baikal_clk_disable(clk_t *clk);

void baikal_clock_init(void);
int baikal_clock_core(unsigned long freq);

static inline int baikal_clock_core_mhz(unsigned long freq) {
	return baikal_clock_core(freq * 1000000);
}

#endif /* __ASSEMBLER__ */

#endif /* _ASM_CLOCK_MANAGER_H */
