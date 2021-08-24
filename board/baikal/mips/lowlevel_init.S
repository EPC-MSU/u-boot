/*
 * File:        lowlevel_init.c
 * Description: Low Level initialization code
 * Authors:
 *   Dmitry Dunaev <dmitry.dunaev@baikalelectronics.ru>
 *   Alexey.Malahov <alexey.malahov@baikalelectronics.ru>
 *   Ekaterina.Skachko <ekaterina.skachko@baikalelectronics.ru>
 *
 * Copyright 2014-2016 Baikal Electronics JSC
 *
 * On BFK board the 24C02 EEPROM is used as FRU device and as
 * storage for several variables.
 */

/* Memory sub-system initialization code */

#include <config.h>
#include <version.h>
#include <asm/regdef.h>
#include <asm/mipsregs.h>
#include <asm/addrspace.h>
#include <asm/arch/p5600.h>
#include <asm/arch/segcfg.h>
#include <asm/arch/sysreg.h>
#include <asm/arch/clock_manager.h>


#ifdef CONFIG_SYS_BOOT_DELAY_MS
#define BOOT_TIMER_FREQ_MS	(_ULCAST_(CONFIG_SYS_INIT_TIMER_FREQ) / 1000)
#define BOOT_DELAY_TICKS	(CONFIG_SYS_BOOT_DELAY_MS * BOOT_TIMER_FREQ_MS)
#endif


.set noreorder
.set mips0
.globl lowlevel_init
.text



/* --------------------- */
/*   init                */
/* --------------------- */
lowlevel_init:
	/* Boot controller barrier */
#ifdef CONFIG_SYS_BOOT_DELAY_MS
#ifdef CONFIG_MIPS_GIC_BASE
	/* Disable GIC */
	li	t0, KSEG1ADDR(CONFIG_MIPS_GCR_BASE)
	lw	t1, MIPS_GCR_GIC_BASE(t0)
	and	t1, t1, 0xfffffffe
	sw	t1, MIPS_GCR_GIC_BASE(t0)
#endif /* CONFIG_MIPS_GIC_BASE */
	/* Disable CPU IRQ */
	mfc0	t1, CP0_STATUS, 0
	and	t1, t1, ~ST0_IE
	and	t1, t1, ~ST0_IM
	mtc0	t1, CP0_STATUS, 0
	/* Setup timer */
	li	t1, BOOT_DELAY_TICKS
	mtc0	t1, CP0_COMPARE, 0
	mtc0	zero, CP0_COUNT, 0
	mfc0	t1, CP0_STATUS, 0
	or	t1, t1, IE_IRQ5
	mtc0	t1, CP0_STATUS, 0

	/* Wait for internal timer */
	.set	mips3
	nop
	wait
	nop
	.set	mips0
#endif /* CONFIG_SYS_BOOT_DELAY_MS */

	/* Check CPU Number */
	mfc0	a0, CP0_EBASE, 1
	.set	mips2
	andi	a0, a0, 0xFF
	bgtz	a0, secondary_cpu
	nop

	/* Fix DDR auto page close */
	li	t0, CONFIG_DDR_CTRL_BASE
	lw	t1, CONFIG_DDR_SCHED_OFF(t0)
	and	t1, t1, CONFIG_DDR_SCHED_MSK
	sw	t1, CONFIG_DDR_SCHED_OFF(t0)
	sync

	.set	mips0
#ifdef CONFIG_BAIKAL_PLL
	/* Base PMU address */
	li	t0, CONFIG_BAIKAL_PLL_BASE
#ifndef CONFIG_BAIKAL_T1
	/* Set L2$ DATASTALL 2 cycle access, 1 stall for engineering Baikal-T CPU.*/
	lw	t1, 0x28(t0)
	and	t1, t1, 0xFFFFFFC0
	or	t1, t1, 0x00002001
	sw	t1, 0x28(t0)
	sync
#endif /* CONFIG_BAIKAL_T1 */
#ifdef BE_CORE_PLL
	/* Core PLL control value */
	li	t1, BE_CORE_PLL
	/* Timeout */
	li	t2, 0x1000
	/* Store new value for Core PLL */
	sw	t1, 0x00(t0)
	sync
	/* Wait for PLL LOCK bit */
1:	lw	t1, 0x00(t0)
	ext	t1, t1, BE_CLK_LOCK_BIT, 1
	bnel	t1, zero, 2f
	addi	t2, t2, -1
	bne	t2, zero, 1b
	nop
	wait
2:
#endif /* BE_CORE_PLL */
#endif /* CONFIG_BAIKAL_PLL */
	/*
	 * Set the stack pointer to SRAM.
	 */
	li	sp, CONFIG_SRAM_STACK_BASE

	/* Save ra */
	addiu	sp, sp, -4
	sw	ra, 4(sp)

#ifdef CONFIG_TARGET_BAIKAL_MIPS
	la t0, __start_baikal_fw
#else
	la t0, __lowlevel_init_cp0
#endif /* CONFIG_TARGET_BAIKAL_MIPS */


	jalr	t0
	nop	/* Delay slot */

	/* Setup Global Configuration Register (GCR) */
#ifdef CONFIG_MIPS_GCR_BASE
	/* Set GCR base addresses */
	li	t0, KSEG1ADDR(CONFIG_MIPS_GCR_BASE)
#ifdef CONFIG_MIPS_GIC_BASE
	/* Set GIC base addresses, block disabled */
	li	t1, CPHYSADDR(CONFIG_MIPS_GIC_BASE)
	sw	t1, MIPS_GCR_GIC_BASE(t0)
#endif /* CONFIG_MIPS_GIC_BASE */
#ifdef CONFIG_MIPS_CPC_BASE
	/* Set CPC base addresses, block enabled */
	li	t1, CPHYSADDR(CONFIG_MIPS_CPC_BASE) | MIPS_GCR_BLOCK_ENABLE
	sw	t1, MIPS_GCR_CPC_BASE(t0)
#endif /* CONFIG_MIPS_CPC_BASE */
#endif /* CONFIG_MIPS_GCR_BASE */
	nop
	/* Restore ra */
	lw	ra, 4(sp)
	jr	ra
	addiu	sp, sp, 4	/* Delay slot */






/* --------------------- */
/*   cp0                 */
/* --------------------- */
__lowlevel_init_cp0:

	/* __start_baikal_fw duplicates these settings.  */

	/*
	 * Step 2) Establish Status Register
	 * (set BEV, clear ERL, clear EXL, clear IE)
	 */
	li	t1, 0x00400000
	mtc0	t1, CP0_STATUS

	/*
	 * Step 3) Establish CP0 Config0
	 * (set K0=3)
	 */
	li	t1, 0x00000003
	mtc0	t1, CP0_CONFIG

	/*
	 * Step 7) Establish Cause
	 * (set IV bit)
	 */
	li	t1, 0x00800000
	mtc0	t1, CP0_CAUSE

	/* Establish Wired (and Random) */
	mtc0	zero, CP0_WIRED
	nop
	li	t0, MIPS_LEGACY_SEGCFG0
	mtc0	t0, CP0_PAGEMASK, 2

	li	t0, MIPS_LEGACY_SEGCFG1
	mtc0	t0, CP0_PAGEMASK, 3

	li	t0, MIPS_LEGACY_SEGCFG2
	mtc0	t0, CP0_PAGEMASK, 4

	mfc0	t0, CP0_CONFIG, 5
	li	t1, 1
	ins	t0, t1, 0, 30
	mtc0	t0, CP0_CONFIG, 5

	jr	ra
	nop




/* --------------------- */
/*   cpu1                */
/* --------------------- */
secondary_cpu:
	.set	mips3
	wait

	.set	mips0
	b secondary_cpu
	nop	/* Delay slot */




/* --------------------- */
/*   version             */
/* --------------------- */
	.long 0x27051956	/* U-Boot Magic Number */
	.globl version_string
version_string:
	.ascii U_BOOT_VERSION_STRING, "\0"
