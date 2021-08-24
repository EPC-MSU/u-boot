
/*
 * Low-level and POST tests.
 *
 * Copyright (C) 2014-2015 Baikal Electronics.
 *
 * Author: Ekaterina Skachko <Ekaterina.Skachko@baikalelectronics.ru>
 *
 * Description: The PMU misc functions.
 */

#include <config.h>
#include <stdint.h>
#include "pmu.h"

#define READ_MEMORY_REG(r)       (*((volatile uint32_t *) (r)))
#define WRITE_MEMORY_REG(r, v)   (*((volatile uint32_t *) (r)) = v)


void llenv_set_axi_ctl(uint32_t pmu_reg, int clkdiv, int en)
{
	uint32_t reg;

	reg = READ_PMU_REG(pmu_reg);
	reg &= ~PMU_CLKDIV_MASK;
	reg |= (PMU_SET_CLKDIV | (clkdiv << PMU_CLKDIV_SHIFT) | en);
	WRITE_PMU_REG(pmu_reg, reg);
}

#define PLL_WAIT_RETRIES 1000
int llenv32_init_pll(const unsigned int pmu_register)
{
	uint32_t reg;
	int i = 0;

	/* Check if PLL already up */
	if (READ_PMU_REG(pmu_register) & BK_PMU_EN_BIT)
		return 0;

	/* Wait for LOCK bit in BK_PMU_COREPLL_CTL */
	while(!(READ_PMU_REG(BK_PMU_COREPLL_CTL) & BK_PMU_LOCK_BIT)) {
		if((i++) == PLL_WAIT_RETRIES) {
			return -1;
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
			return -1;
		}
	}

	return 0;
}

int llenv32_deinit_pll(const unsigned int pmu_register)
{
	uint32_t reg;
	int i = 0;

	/* Wait for LOCK bit in BK_PMU_COREPLL_CTL */
	while(!(READ_PMU_REG(BK_PMU_COREPLL_CTL) & BK_PMU_LOCK_BIT)) {
		if((i++) == PLL_WAIT_RETRIES) {
			return -1;
		}
	}
	/* Set EN & RST bit in pmu_register */
	reg = READ_PMU_REG(pmu_register);
	reg |= BK_PMU_OFF_BIT | BK_PMU_RST_BIT;
	WRITE_PMU_REG(pmu_register, reg);

	/* Wait for LOCK bit in pmu_register */
	i = 0;
	while(!(READ_PMU_REG(pmu_register) & BK_PMU_LOCK_BIT)) {
		if((i++) == PLL_WAIT_RETRIES) {
			return -1;
		}
	}

	return 0;
}

int llenv_get_cpu_freq(void)
{
	uint32_t freq, nf, nr, od, reg;

	reg = READ_PMU_REG(BK_PMU_COREPLL_CTL);

	nf = ((reg & BK_PMU_CLKF_MASK) >> BK_PMU_CLKF_SHIFT) + 1;
	nr = ((reg & BK_PMU_CLKR_MASK) >> BK_PMU_CLKR_SHIFT) + 1;
	od = ((reg & BK_PMU_CLKOD_MASK) >> BK_PMU_CLKOD_SHIFT) + 1;

	freq = REFERENCE_FREQ * nf / nr / od;

	return freq;
}

void llenv_set_cpu_freq(uint32_t freq)
{
	uint32_t nf, nr, od, reg;

	if (freq <= 850)
		od = 4;
	else
		od = 2;

	nr = 1;

	nf = (freq * nr * od) / REFERENCE_FREQ;

	reg = READ_PMU_REG(BK_PMU_COREPLL_CTL);

	reg &= ~(BK_PMU_CLKF_MASK) & \
	       ~(BK_PMU_CLKR_MASK) & \
	       ~(BK_PMU_CLKOD_MASK);

	reg |= ((nf - 1) << BK_PMU_CLKF_SHIFT) | \
	       ((nr - 1) << BK_PMU_CLKR_SHIFT) | \
	       ((od - 1) << BK_PMU_CLKOD_SHIFT);

	/* printf("Set frequency = %d\n  ", freq); */

	WRITE_PMU_REG(BK_PMU_COREPLL_CTL, reg | BK_PMU_RST_BIT);

	return;
}
