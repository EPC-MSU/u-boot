/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Inline assembly cache operations.
 *
 * Copyright (C) 1996 David S. Miller (davem@davemloft.net)
 * Copyright (C) 1997 - 2002 Ralf Baechle (ralf@gnu.org)
 * Copyright (C) 2004 Ralf Baechle (ralf@linux-mips.org)
 */
#ifndef _ASM_R4KCACHE_H
#define _ASM_R4KCACHE_H

#include <asm/asm.h>
#include <asm/cacheops.h>
#include <asm/addrspace.h>

#define INDEX_BASE	CKSEG0

#ifndef cpu_scache_line_size
#define cpu_scache_line_size(X) scache.linesz
#endif

typedef struct {
    unsigned int sets;
    unsigned int linesz;
    unsigned int ways;
    unsigned int waysize;
    unsigned int waybit;
} scache_t;

extern scache_t scache;

static inline void flush_icache_line_indexed(unsigned long addr)
{
	mips_cache(INDEX_INVALIDATE_I, (volatile void *)addr);
}

static inline void flush_dcache_line_indexed(unsigned long addr)
{
	mips_cache(INDEX_WRITEBACK_INV_D, (volatile void *)addr);
}

static inline void flush_scache_line_indexed(unsigned long addr)
{
	mips_cache(INDEX_WRITEBACK_INV_SD, (volatile void *)addr);
}

static inline void flush_icache_line(unsigned long addr)
{
	mips_cache(HIT_INVALIDATE_I, (volatile void *)addr);
}

static inline void flush_dcache_line(unsigned long addr)
{
	mips_cache(HIT_WRITEBACK_INV_D, (volatile void *)addr);
}

static inline void invalidate_dcache_line(unsigned long addr)
{
	mips_cache(HIT_INVALIDATE_D, (volatile void *)addr);
}

static inline void invalidate_scache_line(unsigned long addr)
{
	mips_cache(HIT_INVALIDATE_SD, (volatile void *)addr);
}

static inline void flush_scache_line(unsigned long addr)
{
	mips_cache(HIT_WRITEBACK_INV_SD, (volatile void *)addr);
}

static inline void cache16_unroll32(unsigned long base,
		unsigned long op)
{
	__asm__ __volatile__(
	"	.set push					\n"
	"	.set noreorder					\n"
	"	.set mips3					\n"
	"	cache %1, 0x000(%0); cache %1, 0x010(%0)	\n"
	"	cache %1, 0x020(%0); cache %1, 0x030(%0)	\n"
	"	cache %1, 0x040(%0); cache %1, 0x050(%0)	\n"
	"	cache %1, 0x060(%0); cache %1, 0x070(%0)	\n"
	"	cache %1, 0x080(%0); cache %1, 0x090(%0)	\n"
	"	cache %1, 0x0a0(%0); cache %1, 0x0b0(%0)	\n"
	"	cache %1, 0x0c0(%0); cache %1, 0x0d0(%0)	\n"
	"	cache %1, 0x0e0(%0); cache %1, 0x0f0(%0)	\n"
	"	cache %1, 0x100(%0); cache %1, 0x110(%0)	\n"
	"	cache %1, 0x120(%0); cache %1, 0x130(%0)	\n"
	"	cache %1, 0x140(%0); cache %1, 0x150(%0)	\n"
	"	cache %1, 0x160(%0); cache %1, 0x170(%0)	\n"
	"	cache %1, 0x180(%0); cache %1, 0x190(%0)	\n"
	"	cache %1, 0x1a0(%0); cache %1, 0x1b0(%0)	\n"
	"	cache %1, 0x1c0(%0); cache %1, 0x1d0(%0)	\n"
	"	cache %1, 0x1e0(%0); cache %1, 0x1f0(%0)	\n"
	"	.set pop					\n"
		:
		: "r" (base),
		  "i" (op));
}

static inline void cache32_unroll32(unsigned long base,
		unsigned long op)
{
	__asm__ __volatile__(
	"	.set push					\n"
	"	.set noreorder					\n"
	"	.set mips3					\n"
	"	cache %1, 0x000(%0); cache %1, 0x020(%0)	\n"
	"	cache %1, 0x040(%0); cache %1, 0x060(%0)	\n"
	"	cache %1, 0x080(%0); cache %1, 0x0a0(%0)	\n"
	"	cache %1, 0x0c0(%0); cache %1, 0x0e0(%0)	\n"
	"	cache %1, 0x100(%0); cache %1, 0x120(%0)	\n"
	"	cache %1, 0x140(%0); cache %1, 0x160(%0)	\n"
	"	cache %1, 0x180(%0); cache %1, 0x1a0(%0)	\n"
	"	cache %1, 0x1c0(%0); cache %1, 0x1e0(%0)	\n"
	"	cache %1, 0x200(%0); cache %1, 0x220(%0)	\n"
	"	cache %1, 0x240(%0); cache %1, 0x260(%0)	\n"
	"	cache %1, 0x280(%0); cache %1, 0x2a0(%0)	\n"
	"	cache %1, 0x2c0(%0); cache %1, 0x2e0(%0)	\n"
	"	cache %1, 0x300(%0); cache %1, 0x320(%0)	\n"
	"	cache %1, 0x340(%0); cache %1, 0x360(%0)	\n"
	"	cache %1, 0x380(%0); cache %1, 0x3a0(%0)	\n"
	"	cache %1, 0x3c0(%0); cache %1, 0x3e0(%0)	\n"
	"	.set pop					\n"
		:
		: "r" (base),
		  "i" (op));
}

static inline void cache64_unroll32(unsigned long base,
		unsigned long op)
{
	__asm__ __volatile__(
	"	.set push					\n"
	"	.set noreorder					\n"
	"	.set mips3					\n"
	"	cache %1, 0x000(%0); cache %1, 0x040(%0)	\n"
	"	cache %1, 0x080(%0); cache %1, 0x0c0(%0)	\n"
	"	cache %1, 0x100(%0); cache %1, 0x140(%0)	\n"
	"	cache %1, 0x180(%0); cache %1, 0x1c0(%0)	\n"
	"	cache %1, 0x200(%0); cache %1, 0x240(%0)	\n"
	"	cache %1, 0x280(%0); cache %1, 0x2c0(%0)	\n"
	"	cache %1, 0x300(%0); cache %1, 0x340(%0)	\n"
	"	cache %1, 0x380(%0); cache %1, 0x3c0(%0)	\n"
	"	cache %1, 0x400(%0); cache %1, 0x440(%0)	\n"
	"	cache %1, 0x480(%0); cache %1, 0x4c0(%0)	\n"
	"	cache %1, 0x500(%0); cache %1, 0x540(%0)	\n"
	"	cache %1, 0x580(%0); cache %1, 0x5c0(%0)	\n"
	"	cache %1, 0x600(%0); cache %1, 0x640(%0)	\n"
	"	cache %1, 0x680(%0); cache %1, 0x6c0(%0)	\n"
	"	cache %1, 0x700(%0); cache %1, 0x740(%0)	\n"
	"	cache %1, 0x780(%0); cache %1, 0x7c0(%0)	\n"
	"	.set pop					\n"
		:
		: "r" (base),
		  "i" (op));
}

static inline void cache128_unroll32(unsigned long base,
		unsigned long op)
{
	__asm__ __volatile__(
	"	.set push					\n"
	"	.set noreorder					\n"
	"	.set mips3					\n"
	"	cache %1, 0x000(%0); cache %1, 0x080(%0)	\n"
	"	cache %1, 0x100(%0); cache %1, 0x180(%0)	\n"
	"	cache %1, 0x200(%0); cache %1, 0x280(%0)	\n"
	"	cache %1, 0x300(%0); cache %1, 0x380(%0)	\n"
	"	cache %1, 0x400(%0); cache %1, 0x480(%0)	\n"
	"	cache %1, 0x500(%0); cache %1, 0x580(%0)	\n"
	"	cache %1, 0x600(%0); cache %1, 0x680(%0)	\n"
	"	cache %1, 0x700(%0); cache %1, 0x780(%0)	\n"
	"	cache %1, 0x800(%0); cache %1, 0x880(%0)	\n"
	"	cache %1, 0x900(%0); cache %1, 0x980(%0)	\n"
	"	cache %1, 0xa00(%0); cache %1, 0xa80(%0)	\n"
	"	cache %1, 0xb00(%0); cache %1, 0xb80(%0)	\n"
	"	cache %1, 0xc00(%0); cache %1, 0xc80(%0)	\n"
	"	cache %1, 0xd00(%0); cache %1, 0xd80(%0)	\n"
	"	cache %1, 0xe00(%0); cache %1, 0xe80(%0)	\n"
	"	cache %1, 0xf00(%0); cache %1, 0xf80(%0)	\n"
	"	.set pop					\n"
		:
		: "r" (base),
		  "i" (op));
}

static inline void cache_noop(unsigned long base,
		unsigned long op) { }

static void (* cache_unroll32)(unsigned long base,
		unsigned long op) = NULL;

static void cache_unroll_setup(void)
{
	unsigned long scache_size = cpu_scache_line_size();

	if (scache_size == 0)
		cache_unroll32 = (void *)cache_noop;
	else if (scache_size == 16)
		cache_unroll32 = cache16_unroll32;
	else if (scache_size == 32)
		cache_unroll32 = cache32_unroll32;
	else if (scache_size == 64)
		cache_unroll32 = cache64_unroll32;
	else if (scache_size == 128)
		cache_unroll32 = cache128_unroll32;
}

static inline void blast_scache(void)
{
	unsigned long lsize = cpu_scache_line_size();
	unsigned long start = INDEX_BASE;
	unsigned long end = start + scache.waysize;
	unsigned long ws_inc = 1UL << scache.waybit;
	unsigned long ws_end = scache.ways << scache.waybit;
	unsigned long ws, addr;

	if (!cache_unroll32)
		cache_unroll_setup();

	for (ws = 0; ws < ws_end; ws += ws_inc)
		for (addr = start; addr < end; addr += lsize * 32)
			cache_unroll32(addr|ws, INDEX_WRITEBACK_INV_SD);
}

static inline void blast_inv_scache(void)
{
	unsigned long lsize = cpu_scache_line_size();
	unsigned long start = INDEX_BASE;
	unsigned long end = start + scache.waysize;
	unsigned long ws_inc = 1UL << scache.waybit;
	unsigned long ws_end = scache.ways << scache.waybit;
	unsigned long ws, addr;

	if (!cache_unroll32)
		cache_unroll_setup();

	for (ws = 0; ws < ws_end; ws += ws_inc)
		for (addr = start; addr < end; addr += lsize * 32)
			cache_unroll32(addr|ws, HIT_INVALIDATE_SD);
}

static inline void blast_scache_range(unsigned long start,
						    unsigned long end)
{
	unsigned long lsize = cpu_scache_line_size();
	unsigned long addr = start & ~(lsize - 1);
	unsigned long aend = (end - 1) & ~(lsize - 1);

	while (1) {
		mips_cache(HIT_WRITEBACK_INV_SD, (volatile void *)addr);
		if (addr == aend)
			break;
		addr += lsize;
	}
}

static inline void blast_inv_scache_range(unsigned long start,
						    unsigned long end)
{
	unsigned long lsize = cpu_scache_line_size();
	unsigned long addr = start & ~(lsize - 1);
	unsigned long aend = (end - 1) & ~(lsize - 1);

	while (1) {
		mips_cache(HIT_INVALIDATE_SD, (volatile void *)addr);
		if (addr == aend)
			break;
		addr += lsize;
	}
}

/*
 * Exported functions
 */

void mips_sc_wback_inv(unsigned long addr, unsigned long size);
void mips_sc_inv(unsigned long addr, unsigned long size);
void mips_sc_enable(void);
void mips_sc_disable(void);
void mips_sc_prefetch_enable(void);
void mips_sc_prefetch_disable(void);
bool mips_sc_prefetch_is_enabled(void);
int mips_sc_init(void);

#endif /* _ASM_R4KCACHE_H */
