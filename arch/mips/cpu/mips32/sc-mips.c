/*
 * Copyright (C) 2006 Chris Dearman (chris@mips.com)
 * Copyright (C) 2016 Dmitry Dunaev (dmitry.dunaev@baikalelectronics.ru)
 *
 * SPDX-License-Identifier:GPL-2.0+
 */
#include <common.h>
#include <asm/cpu.h>
#include <asm/mipsregs.h>
#include <asm/bitops.h>
#include <asm/cacheops.h>
#include <asm/r4kcache.h>
#include <asm/mips-cm.h>
#include <linux/types.h>
#include <asm/system.h>

/*
 * MIPS32/MIPS64 L2 cache handling
 */

static char *way_string[] = { NULL, "direct mapped", "2-way",
	"3-way", "4-way", "5-way", "6-way", "7-way", "8-way",
	"9-way", "10-way", "11-way", "12-way",
	"13-way", "14-way", "15-way", "16-way",
};

scache_t scache;

/*
 * Writeback and invalidate the secondary cache before DMA.
 */
void mips_sc_wback_inv(unsigned long addr, unsigned long size)
{
	blast_scache_range(addr, addr + size);
}

/*
 * Invalidate the secondary cache before DMA.
 */
void mips_sc_inv(unsigned long addr, unsigned long size)
{
	unsigned long lsize = cpu_scache_line_size();
	unsigned long almask = ~(lsize - 1);

	flush_scache_line(addr & almask);
	flush_scache_line((addr + size - 1)	& almask);
	blast_inv_scache_range(addr, addr + size);
}

void mips_sc_enable(void)
{
	/* L2 cache is permanently enabled */
}

void mips_sc_disable(void)
{
	/* L2 cache is permanently enabled */
}

void mips_sc_prefetch_enable(void)
{
	unsigned long pftctl;

	if (mips_cm_revision() < CM_REV_CM2_5)
		return;

	/*
	 * If there is one or more L2 prefetch unit present then enable
	 * prefetching for both code & data, for all ports.
	 */
	pftctl = read_gcr_l2_pft_control();
	if (pftctl & CM_GCR_L2_PFT_CONTROL_NPFT_MSK) {
		pftctl &= ~CM_GCR_L2_PFT_CONTROL_PAGEMASK_MSK;
		pftctl |= PAGE_MASK & CM_GCR_L2_PFT_CONTROL_PAGEMASK_MSK;
		pftctl |= CM_GCR_L2_PFT_CONTROL_PFTEN_MSK;
		write_gcr_l2_pft_control(pftctl);

		pftctl = read_gcr_l2_pft_control_b();
		pftctl |= CM_GCR_L2_PFT_CONTROL_B_PORTID_MSK;
		pftctl |= CM_GCR_L2_PFT_CONTROL_B_CEN_MSK;
		write_gcr_l2_pft_control_b(pftctl);
	}
}

void mips_sc_prefetch_disable(void)
{
	unsigned long pftctl;

	if (mips_cm_revision() < CM_REV_CM2_5)
		return;

	pftctl = read_gcr_l2_pft_control();
	pftctl &= ~CM_GCR_L2_PFT_CONTROL_PFTEN_MSK;
	write_gcr_l2_pft_control(pftctl);

	pftctl = read_gcr_l2_pft_control_b();
	pftctl &= ~CM_GCR_L2_PFT_CONTROL_B_PORTID_MSK;
	pftctl &= ~CM_GCR_L2_PFT_CONTROL_B_CEN_MSK;
	write_gcr_l2_pft_control_b(pftctl);
}

bool mips_sc_prefetch_is_enabled(void)
{
	unsigned long pftctl;

	if (mips_cm_revision() < CM_REV_CM2_5)
		return false;

	pftctl = read_gcr_l2_pft_control();
	if (!(pftctl & CM_GCR_L2_PFT_CONTROL_NPFT_MSK))
		return false;
	return !!(pftctl & CM_GCR_L2_PFT_CONTROL_PFTEN_MSK);
}

/*
 * Check if the L2 cache controller is activated on a particular platform.
 * MTI's L2 controller and the L2 cache controller of Broadcom's BMIPS
 * cores both use c0_config2's bit 12 as "L2 Bypass" bit, that is the
 * cache being disabled.  However there is no guarantee for this to be
 * true on all platforms.  In an act of stupidity the spec defined bits
 * 12..15 as implementation defined so below function will eventually have
 * to be replaced by a platform specific probe.
 */
static inline int mips_sc_is_activated(void)
{
	unsigned int config2 = read_c0_config2();
	unsigned int tmp;

	/* Check the bypass bit (L2B) */
	switch (read_c0_prid() & PRID_MIPS_MASK) {
	case PRID_IMP_34K:
	case PRID_IMP_74K:
	case PRID_IMP_1004K:
	case PRID_IMP_1074K:
	case PRID_IMP_INTERAPTIV_UP:
	case PRID_IMP_INTERAPTIV_MP:
	case PRID_IMP_PROAPTIV_UP:
	case PRID_IMP_PROAPTIV_MP:
	case PRID_IMP_P5600:
	case PRID_IMP_QEMU_GENERIC:
		if (config2 & (1 << 12))
			return 0;
	}

	tmp = (config2 >> 4) & 0x0f;
	if (0 < tmp && tmp <= 7)
		scache.linesz = 2 << tmp;
	else
		return 0;
	return 1;
}

static int mips_sc_probe_cm3(void)
{
	unsigned long cfg = read_gcr_l2_config();
	unsigned long sets, line_sz, assoc;

	if (cfg & CM_GCR_L2_CONFIG_BYPASS_MSK)
		return 0;

	sets = cfg & CM_GCR_L2_CONFIG_SET_SIZE_MSK;
	sets >>= CM_GCR_L2_CONFIG_SET_SIZE_SHF;
	if (sets)
		scache.sets = 64 << sets;

	line_sz = cfg & CM_GCR_L2_CONFIG_LINE_SIZE_MSK;
	line_sz >>= CM_GCR_L2_CONFIG_LINE_SIZE_SHF;
	if (line_sz)
		scache.linesz = 2 << line_sz;

	assoc = cfg & CM_GCR_L2_CONFIG_ASSOC_MSK;
	assoc >>= CM_GCR_L2_CONFIG_ASSOC_SHF;
	scache.ways = assoc + 1;
	scache.waysize = scache.sets * scache.linesz;
	scache.waybit = __ffs(scache.waysize);

	return !!(scache.linesz);
}

static inline int mips_sc_probe(void)
{
	unsigned int config1, config2;
	unsigned int tmp;

	if (mips_cm_revision() >= CM_REV_CM3)
		return mips_sc_probe_cm3();

	/* Does this MIPS32/MIPS64 CPU have a config2 register? */
	config1 = read_c0_config1();
	if (!(config1 & MIPS_CONF_M))
		return 0;

	config2 = read_c0_config2();

	if (!mips_sc_is_activated())
		return 0;

	tmp = (config2 >> 8) & 0x0f;
	if (tmp <= 7)
		scache.sets = 64 << tmp;
	else
		return 0;

	tmp = (config2 >> 0) & 0x0f;
	if (tmp <= 7)
		scache.ways = tmp + 1;
	else
		return 0;

	scache.waysize = scache.sets * scache.linesz;
	scache.waybit = __ffs(scache.waysize);

	return 1;
}

int mips_sc_init(void)
{
	unsigned int scache_size;
	int found = mips_sc_probe();

	if (found) {
		mips_sc_enable();
		mips_sc_prefetch_enable();
		scache_size = scache.ways * scache.sets * scache.linesz;
		printf("MIPS:  secondary cache %dkB, %s, linesize %d bytes.\n",
		       scache_size >> 10,
		       way_string[scache.ways], scache.linesz);
	}
	return found;
}
