/*
 * Low-level and POST tests.
 *
 * Copyright (C) 2014 Baikal Electronics.
 *
 * Author: Alexey Malahov <Alexey.Malahov@baikalelectronics.com>
 *
 * Description: Running handlers.
 */

#include "llenv.h"
#include <asm/arch/sysreg.h>
#include <asm/mipsregs.h>
#include "mmu.h"

#define READ_SYSREG(a...)	__read_32bit_c0_register(a)

//#define EXC_DEBUG
#define debug_msg	debug

llenv_interrupt_handler_t llenv_test_irq_trap;
llenv_interrupt_handler_t llenv_test_exc_trap;

extern llenv_tsuite_t *llenv_tsuites_list [];

int llenv_run_all(void)
{
	return llenv_run_ts(0, 0);
}

int llenv_run_ts(char *ts_name, char *tc_name)
{
	llenv_tsuite_t *ts;
	int i = 0, st = 0;

	while ((ts = (llenv_tsuites_list[i]))) {
		i++;
		if ((ts_name) && llenv_strcmp(ts_name, ts->testlink_ts_name)) {
			continue;
		}
#ifdef DEBUG
		llenv_print("Run "); llenv_print(ts->testlink_ts_name); llenv_print(" test suite" NL);
#endif /* DEBUG */
		st |= ts->ts_run(tc_name);
	}

	return st;
}

int llenv_default_run_testsuite(llenv_tsuite_t *ts, llenv_tcase_t **tc_list, char *tc_name)
{
	llenv_tcase_t *tc;
	int i = 0;
	int ret, status = OK;

	if (!ts) {
		return ERROR;
	}
	/* Run the init() function. */
	ts->ts_init(0);

	/* Run all test cases of the test suite. */
	while ((tc = tc_list[i])) {
		i++;
		if ((tc_name) && llenv_strcmp(tc_name, tc->testlink_tc_name)) {
			continue;
		}

		llenv_print(tc->testlink_tc_name);
		llenv_print("\n");
		if ((ret = tc->tc_run()) == 0) {
			llenv_print(" : PASS" NL);
		} else if (ret == ERROR_UNIMPLEMENTED) {
			status |= ret;
			llenv_print(" : TEST CASE UNIMPLEMENTED" NL);
		} else {
			status |= ret;
			llenv_print(" : FAIL" NL);
		}
	}

	/* Run the teardown() function. */
	ts->ts_teardown(0);

	return status;
}

void llenv_irq_handle (unsigned int iar, unsigned int aiar,
			unsigned int sp, unsigned int cpsr)
{
	debug_msg("IRQ iar=%d aiar=0x%x sp=0x%x cpsr=0x%x\n", iar, aiar, sp, cpsr);

	if (llenv_test_irq_trap) {
		llenv_test_irq_trap(iar, aiar);
	}
	debug_msg("IRQ ret\n");
}

void llenv_exc_handle (unsigned int iar, unsigned int excode, 
			unsigned int cause, unsigned int status)
{
#ifdef EXC_DEBUG
	debug_msg("EXC irq=%d excode=0x%x cause=0x%x status=0x%x\n",
			iar, excode, cause, status);
	debug_msg("BadVaddr %x\n",READ_SYSREG(MIPS_CP0_BAD_VADDR));
	debug_msg("EPC %x\n",READ_SYSREG(MIPS_CP0_EPC));
#endif
	if ((iar) && (llenv_test_irq_trap)) {
		debug_msg("EXC: call irq handler\n");
		llenv_test_irq_trap(iar, excode);
	}

			llenv_general_exc_handler(iar, excode);
#ifdef EXC_DEBUG
	debug_msg("EXC ret\n");
#endif
}
