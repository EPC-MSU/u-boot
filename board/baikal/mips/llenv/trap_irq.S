/*
 * Low-level and POST tests.
 *
 * Copyright (C) 2014 Baikal Electronics.
 *
 * Author: Alexey Malahov <Alexey.Malahov@baikalelectronics.com>
 *
 * Description: Interrupt and exception handlers.
 */

// vvv #include "gic.h"
#include <asm/arch/sysreg.h>

.set	noreorder
.set	noat

.global llenv32_trap_irq
.global llenv32_trap_exc
.global do_irq
.global arch_interrupt_init

.extern IRQ_STACK_START
.extern IRQ_STACK_START_IN

#ifdef BROM
.extern brom_exc_handle
#endif /* BROM */

#ifndef BROM
#define STACK_UBOOT_TOP	IRQ_STACK_START
#define STACK_UBOOT_SIZE_IRQ	(16 * 1024)
#define STACK_UBOOT_SIZE_SYSTEM	(16 * 1024)

do_irq:
llenv32_trap_irq:
	di
	addiu	sp, sp, -136
	/* Reserve 16 bytes to store incoming function arguments. */
	sw	at, 16(sp)
	sw	v0, 20(sp)
	sw	v1, 24(sp)	
	sw	a1, 28(sp)
	sw	a2, 32(sp)
	sw	a3, 36(sp)
	sw	t0, 40(sp)
	sw	t1, 44(sp)
	sw	t2, 48(sp)
	sw	t3, 52(sp)
	sw	t4, 56(sp)
	sw	t5, 60(sp)
	sw	t6, 64(sp)
	sw	t7, 68(sp)
	sw	t8, 72(sp)
	sw	t9, 76(sp)
	sw	s0, 80(sp)
	sw	s1, 84(sp)
	sw	s2, 88(sp)
	sw	s3, 92(sp)
	sw	s4, 96(sp)
	sw	s5, 100(sp)
	sw	s6, 104(sp)
	sw	s7, 108(sp)
	sw	fp, 112(sp)
	sw	k0, 116(sp)
	sw	k1, 120(sp)
	sw	gp, 124(sp)
	sw	ra, 128(sp)
	sw	a0, 132(sp)

	mfc0	t0, MIPS_CP0_CONFIG3
	ext	t0, t0, CP0_CONFIG3_VEIC_POS, CP0_CONFIG3_VEIC_BITS
	li	t1, 1	

	mfc0	a3, MIPS_CP0_STATUS
	beq	t0, t1, eic_mode
	mfc0	a0, MIPS_CP0_CAUSE /* Delay slot. */
	and	a0, a0, a3 	/* mask HW IRQs */
eic_mode:
	ext	a0, a0, CP0_CAUSE_IP_HW_POS, CP0_CAUSE_IP_HW_BITS

	bne	a0, zero, next_arg
	mfc0	a1, MIPS_CP0_CAUSE /* Delay slot. */
	and	a1, a1, a3 	/* mask SW IRQs */
	ext	a1, a1, CP0_CAUSE_IP_SW_POS, CP0_CAUSE_IP_SW_BITS


next_arg:
	move	a2, sp
	jal	llenv_irq_handle	
	mfc0	a3, MIPS_CP0_STATUS /* Delay slot. */

	/* Restore registers and return. */
	lw	at, 16(sp)
	lw	v0, 20(sp)
	lw	v1, 24(sp)	
	lw	a1, 28(sp)
	lw	a2, 32(sp)
	lw	a3, 36(sp)
	lw	t0, 40(sp)
	lw	t1, 44(sp)
	lw	t2, 48(sp)
	lw	t3, 52(sp)
	lw	t4, 56(sp)
	lw	t5, 60(sp)
	lw	t6, 64(sp)
	lw	t7, 68(sp)
	lw	t8, 72(sp)
	lw	t9, 76(sp)
	lw	s0, 80(sp)
	lw	s1, 84(sp)
	lw	s2, 88(sp)
	lw	s3, 92(sp)
	lw	s4, 96(sp)
	lw	s5, 100(sp)
	lw	s6, 104(sp)
	lw	s7, 108(sp)
	lw	fp, 112(sp)
	lw	k0, 116(sp)
	lw	k1, 120(sp)
	lw	gp, 124(sp)
	lw	ra, 128(sp)
	lw	a0, 132(sp)

	addiu   sp, sp, 136
	ei
	ehb
	eret
#endif /* BROM */

llenv32_trap_exc:
	di
	addiu	sp, sp, -136
	/* Reserve 16 bytes to store incoming function arguments. */
	sw	at, 16(sp)
	sw	v0, 20(sp)
	sw	v1, 24(sp)	
	sw	a1, 28(sp)
	sw	a2, 32(sp)
	sw	a3, 36(sp)
	sw	t0, 40(sp)
	sw	t1, 44(sp)
	sw	t2, 48(sp)
	sw	t3, 52(sp)
	sw	t4, 56(sp)
	sw	t5, 60(sp)
	sw	t6, 64(sp)
	sw	t7, 68(sp)
	sw	t8, 72(sp)
	sw	t9, 76(sp)
	sw	s0, 80(sp)
	sw	s1, 84(sp)
	sw	s2, 88(sp)
	sw	s3, 92(sp)
	sw	s4, 96(sp)
	sw	s5, 100(sp)
	sw	s6, 104(sp)
	sw	s7, 108(sp)
	sw	fp, 112(sp)
	sw	k0, 116(sp)
	sw	k1, 120(sp)
	sw	gp, 124(sp)
	sw	ra, 128(sp)
	sw	a0, 132(sp)

	mfc0	t0, MIPS_CP0_CONFIG3
	ext	t0, t0, CP0_CONFIG3_VEIC_POS, CP0_CONFIG3_VEIC_BITS
	li	t1, 1	

	mfc0	a3, MIPS_CP0_STATUS
	beq	t0, t1, eic_exc_mode
	mfc0	a0, MIPS_CP0_CAUSE	/* Delay slot */
	and	a0, a0, a3 // mask IRQ
eic_exc_mode:
	ext	a0, a0, CP0_CAUSE_IP_HW_POS, CP0_CAUSE_IP_HW_BITS

	mfc0	a1, MIPS_CP0_CAUSE
	ext	a1, a1, CP0_CAUSE_EXCODE_POS, CP0_CAUSE_EXCODE_BITS

	mfc0	a2, MIPS_CP0_CAUSE
#ifdef BROM
	jal	brom_exc_handle
#else /* BROM */
	jal	llenv_exc_handle
#endif /* BROM */
	mfc0	a3, MIPS_CP0_STATUS /* Delay slot. */

	/* Restore registers and return. */

	lw	at, 16(sp)
	lw	v0, 20(sp)
	lw	v1, 24(sp)	
	lw	a1, 28(sp)
	lw	a2, 32(sp)
	lw	a3, 36(sp)
	lw	t0, 40(sp)
	lw	t1, 44(sp)
	lw	t2, 48(sp)
	lw	t3, 52(sp)
	lw	t4, 56(sp)
	lw	t5, 60(sp)
	lw	t6, 64(sp)
	lw	t7, 68(sp)
	lw	t8, 72(sp)
	lw	t9, 76(sp)
	lw	s0, 80(sp)
	lw	s1, 84(sp)
	lw	s2, 88(sp)
	lw	s3, 92(sp)
	lw	s4, 96(sp)
	lw	s5, 100(sp)
	lw	s6, 104(sp)
	lw	s7, 108(sp)
	lw	fp, 112(sp)
	lw	k0, 116(sp)
	lw	k1, 120(sp)
	lw	gp, 124(sp)
	lw	ra, 128(sp)
	lw	a0, 132(sp)

	ehb
	addiu   sp, sp, 136
	ei
	eret

#ifdef UBOOT
arch_interrupt_init:
	/* TBD: set stack. */
	nop

#endif /* UBOOT */


#ifdef RTL

#define NOPS nop; nop; nop; nop; nop; nop; nop; nop; nop; nop

/*
 * Define llenv32_install_hdl function for timer/brom RTL test.
 * See following for details
 * http://ubsrv1.baikal.int/bugzilla-4.2.5/show_bug.cgi?id=6#c8
 */
.global llenv32_install_hdl
llenv32_install_hdl:
	move	v0,zero
	j	hdl_18
	li      v1,-1
hdl_c:
	lbu	t0,0(t0)
	addiu	v0,v0,1
	sb	t0,0(a3)
hdl_18:
	addiu	a2,a2,-1
	addu	t0,a1,v0
	bne	a2,v1,hdl_c
	addu	a3,a0,v0
	jr	ra
	nop
#endif /* RTL */
	
