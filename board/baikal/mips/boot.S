/*
 * Low-level and POST tests.
 *
 * Copyright (C) 2015 Baikal Electronics.
 *
 * Author: Alexey Malahov <Alexey.Malahov@baikalelectronics.com>
 *
 * Description: U-boot init file.
 */

#include <asm/arch/sysreg.h>
#include <config.h>
#include "pmu.h"


#define CALL(x)		\
	la	t9, x;	\
	jalr	t9;	\
	nop /* Delay slot */

.set noat
.set noreorder
.global __start_baikal_fw
.global llenv_uboot_pmu
.global llenv_uboot_ddr
.text


/* --------------------- */
/* __start_baikal_fw     */
/* --------------------- */
__start_baikal_fw:

	/* Save ra */
	addiu	sp, sp, -4
	sw	ra, 4(sp)

	lui	t0, (CP0_CAUSE_DC >> 16)
	mtc0	t0, MIPS_CP0_CAUSE

	CALL	(llenv_init_tlb)
	CALL	(llenv_init_caches)
#if !defined(CONFIG_BAIKAL_T1)
	CALL	(llenv_uboot_pmu)
#endif
	CALL	(llenv_uboot_ddr)
	CALL	(llenv_init_regs)

	/* Restore ra */
	lw	ra, 4(sp)
	jr	ra
	addiu	sp, sp, 4 /* Delay slot */


/* --------------------- */
/* llenv_uboot_pmu       */
/* --------------------- */
llenv_uboot_pmu:
	/* Save ra */
	addiu	sp, sp, -8
	sw	ra, 4(sp)
	sw	a0, 8(sp)

	li	a0, BK_PMU_ETHPLL_CTL
	bal	llenv32_init_pll
	nop /* Delay slot */

	/* Restore ra */
	lw	a0, 8(sp)
	lw	ra, 4(sp)
	jr	ra
	addiu	sp, sp, 8 /* Delay slot */




/* --------------------- */
/* llenv_uboot_ddr       */
/* --------------------- */
llenv_uboot_ddr:

	/* Save ra, llenv_init_ddr uses k0-k1, save it also. */
	addiu	sp, sp, -12
	sw	ra, 4(sp)
	sw	k0, 8(sp)
	sw	k1, 12(sp)

	CALL(llenv_init_ddr)

	/* save result */
	li	k0, CONFIG_SRAM_DDR_INIT_RESULT
	sw	v0, CONFIG_DDR_INIT_RESULT_OFF_v0(k0)
	sw	v1, CONFIG_DDR_INIT_RESULT_OFF_v1(k0)
	sw	t7, CONFIG_DDR_INIT_RESULT_OFF_t7(k0)

	/* Restore ra */
	lw	k1, 12(sp)
	lw	k0, 8(sp)
	lw	ra, 4(sp)
	jr	ra
	addiu	sp, sp, 12 /* Delay slot */
