/*
 * Low-level and POST tests.
 *
 * Copyright (C) 2015-2017 Baikal Electronics.
 *
 * Author: Ekaterina Skachko <Ekaterina.Skachko@baikalelectronics.ru>
 * Contributor: Georgy Vlasov <Georgy.Vlasov@baikalelectronics.ru> {addrcheck}
 *
 * Description: Memory tests.
 */

#define DEBUG

#include <config.h>
#include "llenv.h"
#include <asm/arch/sysreg.h>
#include "mmu.h"
#include "ddr.h"


/* Declare test suite. */
#define TSUITE memtest
DEC_TESTSUITE(TSUITE, "memtest");

/* Declare all test cases: */
DEC_TESTCASE(TSUITE, movinv_0, "movinv0");
DEC_TESTCASE(TSUITE, movinv_1, "movinv1");
DEC_TESTCASE(TSUITE, movinv_walk, "movinv_walk");
DEC_TESTCASE(TSUITE, addrcheck, "addrcheck");
DEC_TESTCASE(TSUITE, solidbits, "solidbits");

/*
 * Add test cases to run. All test cases should be specified
 * between BEGIN_TCASE_LIST and END_TCASE_LIST.
 */
BEGIN_TCASE_LIST(TSUITE)
ADD_TCASE_TO_LIST(TSUITE, movinv_0)
ADD_TCASE_TO_LIST(TSUITE, movinv_1)
ADD_TCASE_TO_LIST(TSUITE, movinv_walk)
ADD_TCASE_TO_LIST(TSUITE, addrcheck)
ADD_TCASE_TO_LIST(TSUITE, solidbits)

END_TCASE_LIST;

#define REPEAT8(com)	com; com; com; com; \
			com; com; com; com;

int movinv(llenv_arch_t start_addr, int size, llenv_arch_t pattern, uint32_t paddr);
int walk_test(llenv_arch_t start_addr, int size, llenv_arch_t pattern, uint32_t paddr);
int solidbits_test(llenv_arch_t start_addr, int size, llenv_arch_t pattern, uint32_t paddr);
int modulo(int offset, llenv_arch_t start_addr, int size, llenv_arch_t pattern);
static int writememory(llenv_arch_t start_addr, int size, llenv_arch_t pattern, uint32_t highphy_addr);
static int checkmemory(llenv_arch_t start_addr, int size, llenv_arch_t pattern, uint32_t highphy_addr);

int memtest_common(int (*do_test)(llenv_arch_t start_addr, int size, llenv_arch_t pattern, uint32_t paddr),
		llenv_arch_t pattern, const char *title);

extern llenv_memory_map_t llenv_memory_map;

#define LIM_4GB	0x100000000ULL

#define READ_DDRC_REG(r)	(*(volatile uint32_t *)(CONFIG_DDR_CTRL_BASE + r))

int memtest_debug;
#define debug_msg(v...) do { if(memtest_debug) printf(v); } while(0)
#define err_msg(v...) printf(v)
#define MAX_ERRORS	10	/* max number of errors to print per region */

int which_rank(llenv_arch_t addr)
{
	int bit_num = READ_DDRC_REG(DDRC_ADDRMAP0) + 6;


	if (addr & (1 << bit_num))
		return 1;
	else
		return 0;
}

void parse_error(llenv_arch_t data, llenv_arch_t pattern)
{

	int i;
	llenv_arch_t mask;

	err_msg("Faulty bits : ");
	for (i = 0; i < 32; i++) {
		mask = (1 << i);
		if ((data & mask) != (pattern & mask))
			err_msg("%d ", i);
	}
	err_msg("\n");

}

DEC_TESTSUITE_INIT(TSUITE)
{
	return 0;
}

DEC_TESTSUITE_RUN(TSUITE)
{
	/* Use default handler. */
	return llenv_default_run_testsuite(&TSUITE_STRUCT(memtest), TSUITE_LIST(memtest), tc_name);
}

DEC_TESTSUITE_INFO(TSUITE)
{
	return;
}

DEC_TESTSUITE_TEARDOWN(TSUITE)
{
	debug_msg("\n TEARDOWN \n");
	return 0;
}

uint64_t get_ddr_highmem_size(void);
uint64_t ddr_high_size;
uint64_t high_phyaddr;
uint64_t high_psize;
uint32_t low_phyaddr;
uint32_t low_psize;


uint32_t check_memory_range(uint64_t phyaddr, uint64_t psize, uint64_t def_phyaddr, uint64_t def_psize){
	/* Size of high addr should be same or smaller than value from ddr spd flash */
        if (((phyaddr + psize) > (def_phyaddr + (def_psize))) ||
	    (psize && (phyaddr < def_phyaddr))) {
		err_msg("\nEntered range is bigger than available memory space.\
		\nPlease input correct value. Default start addr of high memory region is 0x20000000, \
		\ndefault start addr of low memory region is 0x00000000, \
		\nsize of high memory automatically set at the start of system, \
		\nand save in memory_high_size uboot env variable.\
		\nSize of low memory is 0x08000000, you can choose different values in 0x0 - 0x08000000. \n");
		return 1;
	} else {
		return 0;
	}
}

uint32_t init_vars(void){
	memtest_debug = getenv_ulong("memtest_debug", 10, 0);
	debug_msg("\nMemory ranges:");
	ddr_high_size = get_ddr_highmem_size(); /* B */
	high_phyaddr = getenv_ulong("memory_high_paddr", 16, MEMORY_HIGH_PADDR);
	debug_msg("\nmemory_high_paddr = %llx", high_phyaddr);
	high_psize = (uint64_t)getenv_ulong("memory_high_size", 16, MEMORY_HIGH_SIZE); /* B */
	if (high_psize > ddr_high_size)
		high_psize = ddr_high_size;
	if (high_phyaddr + high_psize > LIM_4GB)
		high_psize = LIM_4GB - high_phyaddr;
	debug_msg("\nmemory_high_size = %llx", high_psize);
	low_phyaddr = getenv_ulong("memory_low_paddr", 16, 0);
	debug_msg("\nmemory_low_paddr = %x", low_phyaddr);
	low_psize = getenv_ulong("memory_low_size", 16, MEMORY_LOW_SIZE);
	debug_msg("\nmemory_low_size = %x\n", low_psize);
	return 0;
}

DEC_TESTCASE_RUN(TSUITE, movinv_0)
{
	return memtest_common(movinv, 0xAAAAAAAA,
			"Test inspired by MEMTEST86 MOVING INVERSION algorithm");
}

DEC_TESTCASE_RUN(TSUITE, movinv_1)
{
	return memtest_common(movinv, 0x55555555,
			"Test inspired by MEMTEST86 MOVING INVERSION algorithm");
}

DEC_TESTCASE_RUN(TSUITE, movinv_walk)
{
	return memtest_common(walk_test, 0,
			"Test inspired by MEMTEST86 MOVING INVERSION algotithm with a walking one");
}

DEC_TESTCASE_RUN(TSUITE, addrcheck)
{
	int res = 0;
	res = memtest_common(writememory, 0, "Address test of DDR (fill mem)");
	res += memtest_common(checkmemory, 0, "Address test of DDR (check mem)");
	return res;
}

DEC_TESTCASE_RUN(TSUITE, solidbits)
{
	llenv_arch_t pattern;
	pattern = getenv_ulong("solidbits_pattern", 16, 0xffffffff);

	return memtest_common(solidbits_test, pattern, "Test inspired by MEMTESTER solidbits algorithm");
}

int memtest_common(int (*do_test)(llenv_arch_t start_addr, int size, llenv_arch_t pattern, uint32_t paddr),
		llenv_arch_t pattern, const char *title)
{
	int res = 0;
	uint32_t mem;
	int size;
	int full_size;
	const llenv_memory_map_t tmp_map  = llenv_memory_map;
	uint32_t high_addr = MEMORY_HIGH_VADDR;
	uint32_t high_size = MEMORY_HIGH_SIZE;

	init_vars();
	if (check_memory_range(high_phyaddr, high_psize, MEMORY_HIGH_PADDR, ddr_high_size))
		return 1;

	if (check_memory_range(low_phyaddr, low_psize,GET_PADDR(MEMORY_LOW_VADDR), MEMORY_LOW_SIZE))
		return 1;

	llenv_memory_map_t exc1_map = {high_phyaddr ,   high_addr,  high_size - sizeof(llenv_arch_t),  MMU_FLAGS};;

	llenv_memory_map_t * ptr_map = &llenv_memory_map;
	mem = MEMORY_LOW_VADDR;
       /* For now the lowest part u-boot relocates to DRAM is STACK
          so let's just take address of the lowest stack variable and
          check DRAM up to it (save another 4K for the next func)*/

        void *stack_pointer;
	mem = LEGACY_KSEG3 | low_phyaddr;
	size = (LEGACY_KSEG3 | (int)&stack_pointer) - mem - 0x1000;
	if (low_psize && size > low_psize)
		size = low_psize;

	debug_msg("\n*** %s ***\n", title);

	printf("\nPhysical range:         0x%x - 0x%x ", GET_PADDR(mem), GET_PADDR(mem + size));
	printf("rank #%d\n", which_rank(GET_PADDR(mem)));
	res = do_test(mem, size, pattern, low_phyaddr);
	/*
	 * This tests the same memory range, but mapped into KSEG0.
	 * Enable it if you want to compare TLB processing performance.
	 */
#if 0
	mem = LEGACY_KSEG0 | low_phyaddr;
	res += do_test(mem, size, pattern, low_phyaddr);
#endif
	full_size = size / 1024;
	while (((exc1_map.paddr_start + MEMORY_HIGH_SIZE) < (high_phyaddr + high_psize)) && exc1_map.paddr_start) {
		llenv_flush_tlb();
		*ptr_map = exc1_map;
		printf("Physical range:         0x%llx - 0x%llx ", exc1_map.paddr_start, \
			exc1_map.paddr_start + high_size);
		printf("rank #%d\n", which_rank(exc1_map.paddr_start));
		res += do_test(high_addr, high_size, pattern, exc1_map.paddr_start);
		exc1_map.paddr_start += high_size;
		full_size += high_size / 1024;
	}
	/* Check rest of memory */
	if (exc1_map.paddr_start){
		llenv_flush_tlb();
		*ptr_map = exc1_map;
		printf("Physical range:         0x%llx - 0x%llx ", exc1_map.paddr_start, \
			exc1_map.paddr_start + (high_phyaddr + high_psize - exc1_map.paddr_start));
		printf("rank #%d\n", which_rank(exc1_map.paddr_start));
		res += do_test(high_addr, (high_phyaddr + high_psize - exc1_map.paddr_start), pattern, exc1_map.paddr_start);
		full_size += high_size / 1024;
	}
	/* End of checking */
	*ptr_map = tmp_map;
	llenv_flush_tlb();

	printf("Fully checked:		%dKB\n", full_size);
        printf("Damaged cells:             %d\n", res);
	return res;
}

int movinv(llenv_arch_t start_addr, int size, llenv_arch_t pattern, llenv_arch_t paddr)
{
	llenv_arch_t *addr;
	short st = OK;

	debug_msg("set memory              pattern 0x%x\n", pattern);

	addr = (llenv_arch_t *)start_addr;
	while ((addr < (llenv_arch_t *)(start_addr + size))) {
		REPEAT8(*(addr++) = pattern);
	}

	debug_msg("backwards check mem for pattern 0x%x\n", pattern);
	debug_msg("set memory              pattern 0x%x\n",  ~pattern);

	for (addr = (llenv_arch_t *)(start_addr + size - sizeof(llenv_arch_t)); \
	     addr >= (llenv_arch_t *)start_addr; addr--) {
		if (*(addr) != pattern) {
			if (st < MAX_ERRORS) {
				err_msg("Error at addr 0x%x value = 0x%x\n",
					(uint32_t)addr, *addr);
				parse_error(*addr, pattern);
			}
			st++;
		}
		*(addr) = ~pattern;
	}

	debug_msg("straight check mem for  pattern 0x%x\n", ~pattern);

	for (addr = (llenv_arch_t *)start_addr; \
		addr <= (llenv_arch_t *)(start_addr + size - sizeof(llenv_arch_t)) && addr; \
		addr++) {
		if (*(addr) != ~pattern) {
			if (st < MAX_ERRORS) {
				err_msg("Error at addr 0x%x 0x%x\n",
					(uint32_t)addr, *addr);
				parse_error(*addr, pattern);
			}
			st++;
		}
	}

	debug_msg("Memory area has %d damaged cells\n", st);

	return st;
}

int walk_test(llenv_arch_t start_addr, int size, llenv_arch_t pattern, uint32_t paddr)
{
	int res = 0;
	int ptrn, word;

	for(ptrn = 0x80; ptrn; ptrn >>= 1) {
		word = (ptrn << 24) | (ptrn << 16) | (ptrn << 8) | ptrn;

		/* Try walking ones */
		res = movinv(start_addr, size, word, paddr);
		/* Try walking zeroes */
		res += movinv(start_addr, size, ~word, paddr);
	}

	return res;
}

int solidbits_test(llenv_arch_t start_addr, int size, llenv_arch_t pattern, uint32_t paddr)
{
	short st = OK;
	llenv_arch_t *bufa, *bufb, q;
	llenv_arch_t *p1, *p2;
	int i, j, count;

	i = size / 2;
	i &= ~0xffff;
	bufa = (llenv_arch_t *)start_addr;
	bufb = (llenv_arch_t *)(start_addr + i);
	count = i / sizeof(llenv_arch_t);

	for (j = 0; j < 4; j++) {
		q = (j & 1) ? ~pattern : pattern;
		debug_msg("set memory buffers (pattern 0x%x-0x%x)\n", q, ~q);

		p1 = bufa;
		p2 = bufb;
		for (i = 0; i < count; i++) {
			*p1++ = *p2++ = (i & 1) ? ~q : q;
		}

		debug_msg("compare mem buffers\n");

		p1 = bufa;
		p2 = bufb;
		for (i = 0; i < count; i++, p1++, p2++) {
			if (*p1 != *p2) {
				if (st < MAX_ERRORS)
					err_msg("Error at %x: %x != %x\n",
						(uint32_t)p1, *p1, *p2);
				st++;
			}
		}
	}

	debug_msg("Memory area has %d damaged cells\n", st);

	return st;
}

static int writememory(llenv_arch_t start_addr, int size, llenv_arch_t pattern, uint32_t highphy_addr)
{
	llenv_arch_t *addr;
	debug_msg("\n highphy_addr = %x", highphy_addr);
	int st = 0;
	addr = (llenv_arch_t *)start_addr;
	while ((addr < (llenv_arch_t *)(start_addr + size))) {
		for(int k = 0; k < 8; k++){
			*(addr) = highphy_addr;
			addr++;
			highphy_addr++;
		}
	}
	return st;
}

static int checkmemory(llenv_arch_t start_addr, int size, llenv_arch_t pattern, uint32_t highphy_addr)
{
	llenv_arch_t *addr;
	int st = 0;
	debug_msg("\n highphy_addr = %x", highphy_addr);
	addr = (llenv_arch_t *)start_addr;
	while ((addr < (llenv_arch_t *)(start_addr + size))) {
		for (int k = 0; k < 8; k++){
			if (*(addr) != highphy_addr) {
				st++;
				if (st < MAX_ERRORS)
					err_msg("Error addr = %x val = 0x%x\n",(int) addr, *(addr));
			}
			addr++;
			highphy_addr++;
		}
	}
	debug_msg("\nMemory area has %d damaged cells\n", st);

	return st;
}

