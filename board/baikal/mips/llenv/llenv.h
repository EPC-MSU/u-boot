/*
 * Low-level and POST tests.
 *
 * Copyright (C) 2014-2016 Baikal Electronics.
 *
 * Author: Alexey Malahov <Alexey.Malahov@baikalelectronics.com>
 *
 * Description: Main header file.
 */

#ifndef __LLENV_H__
#define __LLENV_H__

#include <common.h>

#define UBOOT
#define ARCH_MIPS

#define OK			0
#define ERROR			-1
#define ERROR_UNIMPLEMENTED	100

typedef struct
{
	int (*ts_init) (int tc_num);
	int  (*ts_run) (char *tc_name);
	void (*ts_info) (void);
	int (*ts_teardown) (int tc_num);
	char *testlink_ts_name;
} llenv_tsuite_t; /* type of test suite. */

typedef struct
{
	int (*tc_run) (void);
	char *testlink_tc_name;
} llenv_tcase_t;

typedef struct
{
	volatile int lock;
} llenv_spinlock_t;

#define TCASE_RUN_HNDL(x, y)	llenv_##x##_##y##_run
#define INIT_HNDL(x)		llenv_##x##_init
#define RUN_HNDL(x)		llenv_##x##_run
#define INFO_HNDL(x)		llenv_##x##_info
#define TEARDOWN_HNDL(x)	llenv_##x##_teardown

#define TSUITE_STRUCT(x)	llenv_##x##_tsuite
#define TCASE_STRUCT(x, y)	llenv_##x##_##y##_tcase
#define TSUITE_LIST(x)		llenv_##x##_list

#define	DEC_TESTSUITE_INIT(ts)	int INIT_HNDL(ts) (int tc_num)
#define DEC_TESTSUITE_RUN(ts)	int RUN_HNDL(ts) (char *tc_name)
#define DEC_TESTSUITE_INFO(ts)	void INFO_HNDL(ts) (void)
#define DEC_TESTSUITE_TEARDOWN(ts)	int TEARDOWN_HNDL(ts) (int tc_num)

#define DEC_TESTCASE_RUN(ts, tc)	int TCASE_RUN_HNDL(ts, tc) (void)

#define DEC_TESTSUITE(ts, name) \
extern DEC_TESTSUITE_INIT(ts); \
extern DEC_TESTSUITE_RUN(ts); \
extern DEC_TESTSUITE_INFO(ts); \
extern DEC_TESTSUITE_TEARDOWN(ts); \
llenv_tsuite_t TSUITE_STRUCT(ts) = { &INIT_HNDL(ts), &RUN_HNDL(ts), \
				&INFO_HNDL(ts), &TEARDOWN_HNDL(ts), name }

#define DEC_TSUITE(x) \
extern llenv_tsuite_t TSUITE_STRUCT(x)

#define DEC_TESTCASE(ts, tc, name)	\
extern DEC_TESTCASE_RUN(ts, tc);		\
llenv_tcase_t TCASE_STRUCT(ts, tc) = { TCASE_RUN_HNDL(ts, tc), name }

#define DEC_TCASE(ts, tc) \
extern llenv_tcase_t TCASE_STRUCT(ts, tc)

#define BEGIN_TSUITES_LIST	\
llenv_tsuite_t *llenv_tsuites_list [] = {

#define ADD_TSUITE_TO_LIST(ts)  \
	(&TSUITE_STRUCT(ts)),

#define END_TSUITE_LIST	\
	(0)		\
}



#define BEGIN_TCASE_LIST(ts)	\
llenv_tcase_t * TSUITE_LIST(ts) [] = {

#define ADD_TCASE_TO_LIST(ts, tc)  \
	(&TCASE_STRUCT(ts, tc)),

#define END_TCASE_LIST	\
	(0)		\
}


/* New line symbols. */
#define NL	"\n\r"

int llenv_default_run_testsuite(llenv_tsuite_t *ts, llenv_tcase_t **tc, char *tc_name);
int llenv_run_all(void);
int llenv_run_ts(char *ts_name, char *tc_name);
int llenv_run_ts_index(int index);
void llenv_init_irq(void);
void llenv_disable_irq(void);

void llenv32_mem_map_info(void);
void llenv32_install_hdl(void *addr_dst, void *addr_src, int size);
void llenv_general_exc_handler(unsigned int iar, unsigned int excode);

typedef void (*llenv_interrupt_handler_t) (unsigned int iar, int unsigned aiar);
typedef uint32_t llenv_arch_t;

#define MIPS_EI asm volatile("ei; nop; ehb; nop")
#define MIPS_DI asm volatile("di; nop; ehb; nop")
#define MIPS_EHB	asm volatile("ehb")
#define MIPS_TLBWI	asm volatile("tlbwi")

#if 1 //vvv ???
#define PAGE_SIZE	0x1000
#define TLB_PG_MASK	0x00000000
#define PFN_PG_MASK	0xffffe000
#else /* vvv: 256MB page */
#define PAGE_SIZE	0x10000000
#define TLB_PG_MASK	0x1fffe000
#define PFN_PG_MASK	0xe0000000
#endif

#define llenv_strcmp	strcmp
#define llenv_print	printf
//vvv #define debug_msg	debug

#endif /* __LLENV_H__ */

