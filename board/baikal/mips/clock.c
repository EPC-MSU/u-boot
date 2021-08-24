/*
 * File:        clock.c
 * Description: Clock tuning module
 * Created:     19 Nov 2013
 * Author:      Dmitry Dunaev <dmitry.dunaev@baikalelectronics.ru>
 * Copyright 2013 Baikal Electronics
 *
 */

#include <common.h>
#include <asm/arch/clock.h>

#define BAIKAL_PCLK_FREQ 600    /* 600MHz */
#define BAIKAL_HCLK_FREQ 50     /* 50MHz */
#define BAIKAL_GMII_FREQ 125	/* 125MHz */

static inline uint32_t mxs_get_pclk(void)
{
        return BAIKAL_PCLK_FREQ;
}

static inline uint32_t mxs_get_hclk(void)
{
        return BAIKAL_HCLK_FREQ;
}

static inline uint32_t mxs_get_gmiiclk(void)
{
        return BAIKAL_GMII_FREQ;
}

uint32_t mxc_get_clock(enum mxc_clock clk)
{
        switch (clk) {
        case MXC_ARM_CLK:
                return mxs_get_pclk() * 1000000;
        case MXC_SATA_CLK:
                return mxs_get_pclk() * 1000000 / 5;
        case MXC_GMII_CLK:
                return mxs_get_gmiiclk() * 1000000;
        case MXC_AHB_CLK:
                return mxs_get_hclk() * 1000000;
        }

        return 0;
}
/* End of file clock.c */

