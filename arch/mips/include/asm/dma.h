/*
 * DMA operations for cached / uncached regions
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * (C) Copyright 2015,2016 Dmitry Dunaev <dmitry.dunaev@baikalelectronics.ru>
 */
#ifndef	__ASM_DMA_H
#define	__ASM_DMA_H

#include <malloc.h>
#include <asm/addrspace.h>
#include <linux/stddef.h>

#ifndef __ASSEMBLY__

#if CONFIG_SYS_SDRAM_BASE < KSEG1
inline void * dmalloc(size_t n)
{
	void *p = malloc(n);
	if (p)
		return (void *)KSEG1ADDR((ulong)p);
	return NULL;
}

inline void * dmalign(size_t alignment, size_t n)
{
	void *p = memalign(alignment, n);
	if (p)
		return (void *)KSEG1ADDR((ulong)p);
	return NULL;
}

inline void dfree(void *p)
{
	if ((ulong)p < KSEG1)
		free(p);
	free((void *)KSEG0ADDR((ulong)p));
}
#else
inline void * dmalloc(size_t n)
{
	return malloc(n);
}

inline void * dmalign(size_t alignment, size_t n)
{
	return memalign(alignment, n);
}

inline void dfree(void *p)
{
	free(p);
}
#endif

#endif /* !__ASSEMBLY__ */

#endif	/* __ASM_DMA_H */