/*
 * Low-level and POST tests.
 *
 * Copyright (C) 2014 Baikal Electronics.
 *
 * Author: Alexey Malahov <Alexey.Malahov@baikalelectronics.com>
 *
 * Description: MMU header file.
 */

#ifndef __MMU_H__
#define __MMU_H__

extern volatile unsigned int tlb_miss_counter;
extern volatile unsigned int test_buf[4];

typedef struct {
	uint64_t paddr_start;
	uint32_t vaddr_start;
	uint32_t size;
	int flags;
} llenv_memory_map_t;

/* Prototypes */
void llenv_flush_tlb(void);

#define DUMMY_ENTRY	0xFFFFFFFF
#define DUMMY_ENTRY_P	0xFFFFFFFFFFFFFFFF

#define MEMORY_LOW_PADDR	0x00000000
#define MEMORY_LOW_VADDR	0xE0000000 /* Define memory from kseg3 (kernel mapped) */
#define MEMORY_LOW_SIZE		0x08000000
#define MEMORY_BASE_UNMAP	0xA0000000
#define MEMORY_HIGH_PADDR	0x20000000
#define MEMORY_HIGH_VADDR	0xC0000000
#define MEMORY_HIGH_SIZE	0x20000000
#define MEMORY_BASE_MAP		MEMORY_LOW_VADDR /* Use low memory */

#define MEMORY_BASE		MEMORY_BASE_MAP
#define MEMORY_SIZE		MEMORY_LOW_SIZE
#define GET_PADDR(v)		((0x1FFFFFFF) & (v))

#define STACK_TOP		(MEMORY_BASE_UNMAP + MEMORY_SIZE - 0x200)
#define STACK_CORE_SIZE		0x10000 /* 64 KBytes. */

#define MMU_CACHEABLE
#define COHERENCE_ENABLE
#define USE_FTLB

#ifdef MMU_CACHEABLE
#ifdef COHERENCE_ENABLE
#define MMU_FLAGS	TLB_MASK_CACHED_COH
#else /* COHERENCE_ENABLE */
#define MMU_FLAGS	TLB_MASK_CACHED_NONCOH
#endif /* COHERENCE_ENABLE */
#else /* MMU_CACHEABLE */
#define MMU_FLAGS	TLB_MASK_UNCACHED
#endif /* MMU_CACHEABLE */

#endif /* __MMU_H__ */
