/*
 * Low-level and POST tests.
 *
 * Copyright (C) 2014-2016 Baikal Electronics.
 *
 * Author: Alexey Malahov <Alexey.Malahov@baikalelectronics.com>
 *
 * Description: Standalone init file.
 */

#include <asm/arch/sysreg.h>
#include <config.h>

#define MMU_FLAGS	TLB_MASK_CACHED_NONCOH

#define CHECK		\
	sync;		\
	bnez	v0, _ddr_init_finish;	\
	 nop

#define CALL(x)		\
	la	t9, x;	\
	jalr	t9;	\
	 nop;		\
	CHECK




.set noat
.set noreorder
.global llenv_init_regs
.global llenv_init_tlb
.global llenv_init_caches
.global llenv_init_ddr
.text


/* --------------------- */
/* regs                  */
/* --------------------- */
llenv_init_regs:

	/* Init GPR registers. */
	move	at, zero
	move	v0, zero
	move	v1, zero
	move	a0, zero
	move	a1, zero
	move	a2, zero
	move	a3, zero
	move	t0, zero
	move	t1, zero
	move	t2, zero
	move	t3, zero
	move	t4, zero
	move	t5, zero
	move	t6, zero
	move	t7, zero
	move	s0, zero
	move	s1, zero
	move	s2, zero
	move	s3, zero
	move	s4, zero
	move	s5, zero
	move	s6, zero
	move	s7, zero
	move	t8, zero
	move	t9, zero
	move	k0, zero
	move	k1, zero

	/* Clear SWs and WP. */
	mtc0	zero, MIPS_CP0_CAUSE
	li	t0, (CP0_STATUS_BEV)
	mtc0	t0, MIPS_CP0_STATUS

	/* Clear internal timer. */
	li	t0, 0xffffffff
	mtc0	t0, MIPS_CP0_COMPARE
	mtc0	zero, MIPS_CP0_COUNT

	/* Init SegCtl registers. */
	lui	t0, SEG_CFG1_LEGACY_DFLT
	ori	t0, t0, SEG_CFG0_LEGACY_DFLT
	mtc0	t0, MIPS_CP0_SEG_CTL0

	lui	t0, SEG_CFG3_LEGACY_DFLT
	ori	t0, t0, SEG_CFG2_LEGACY_DFLT
	mtc0	t0, MIPS_CP0_SEG_CTL1

	lui	t0, SEG_CFG5_LEGACY_DFLT
	ori	t0, t0, SEG_CFG4_LEGACY_DFLT
	mtc0	t0, MIPS_CP0_SEG_CTL2

	/* Set WB cached attribute for kseg0 */
	mfc0	k0, MIPS_CP0_CONFIG0
	li	k1, CP0_CONFIG0_K0_WB
	ins	k0, k1, CP0_CONFIG0_K0_POS, CP0_CONFIG0_K0_BITS
	mtc0	k0, MIPS_CP0_CONFIG0


#if !defined(CONFIG_BAIKAL_T1)
	/* Errata E#26. Disable load/store bonding. */
	mfc0    t0, MIPS_CP0_CONFIG6
	lui t1, (CP0_CONFIG6_DLSB >> 16)
	or  t0, t0, t1

	/* Errata E#29, CONFIG6. This disables all JR prediction other than JR $31. */
	ori t0, t0, CP0_CONFIG6_JRCD
	mtc0    t0, MIPS_CP0_CONFIG6

	/* Errata E#29, CONFIG7. This disables all JR $31 prediction through return prediction stack. */
	mfc0    t0, MIPS_CP0_CONFIG7
	ori t0, t0, CP0_CONFIG7_RPS
	mtc0    t0, MIPS_CP0_CONFIG7
#endif /* CONFIG_BAIKAL_T1 */

	jr	ra
	nop	/* Delay slot */





/* --------------------- */
/* ddr                   */
/* --------------------- */
/* v0 - On fail v0 contains ERROR_DDR_INIT or ERROR_DDR_CKSUM_SPD. */
/* v1 - Save DDR init results. */

llenv_init_ddr:

	/* Save ra */
	addiu	sp, sp, -4
	sw	ra, 4(sp)

	/* Only core0 initializes DDR. */
	mfc0	t0, MIPS_CP0_EBASE
	ext	t0, t0, CP0_EBASE_CPUNUM_POS, CP0_EBASE_CPUNUM_BITS
	bnez	t0, _ddr_init_finish
	nop	/* Delay slot. */

	CALL	(llenv_prepare_buffer0)
	CALL	(llenv_prepare_buffer1)
	CALL	(init_ddr_spd)

_ddr_init_finish:
	/* Restore ra */
	lw	ra, 4(sp)
	jr	ra
	addiu	sp, sp, 4	/* Delay slot */






/* --------------------- */
/* tlb                   */
/* --------------------- */
llenv_init_tlb:

	/* Enable FTLB in the Config6 register. */
	mfc0	t3, MIPS_CP0_CONFIG6
	ori	t3, t3, CP0_CONFIG6_FTLBEN
	mtc0	t3, MIPS_CP0_CONFIG6
	sync
	ehb

	/* Read the Config0 register to identify the MMU type. */
	mfc0	t0, MIPS_CP0_CONFIG0
	ext	t3, t0, CP0_CONFIG0_MT_POS, CP0_CONFIG0_MT_SIZE	/* Get MMU type. */
	li	t1, CP0_CONFIG0_MT_TLB
	beq	t3, t1, _vtlb
	move	t4, zero /* Set the number of FTLB entries to 0. Delay slot. */

	li	t1, CP0_CONFIG0_MT_DUAL
	bne	t3, t1, no_tlb
	nop	/* Delay slot. */

	/* Clear context. */
	mtc0	zero, MIPS_CP0_TLB_CONTEXT

	/* Initialize TLB according to the user guide, MIPS32PRA-AFP-05, page 34. */


/* Enable/disable FTLB. */
_vtlb_ftlb:

	/* Calculate the number of the FTLB entries. */
	mfc0	t3, MIPS_CP0_CONFIG4
	ext	t5, t3, CP0_CONFIG4_MMU_EXTDEF_POS, CP0_CONFIG4_MMU_EXTDEF_BITS
	li	t1, CP0_CONFIG4_MMU_EXTDEF_SIZE
	beq	t1, t5, _vtlb
	/* Delay slot. Config4[7:0] used as MMUSizeExt. */
	ext	t4, t3, CP0_CONFIG4_MMU_SIZE_EXT_POS, CP0_CONFIG4_MMU_SIZE_EXT_BITS

	/* Config4[3:0] - FTLB ways Config4[7:4] - FTLB sets */
	li	t4, CP0_CONFIG4_P5600_FTLB_SIZE	/* FTLB is organized as 128 sets and 4-ways */

_vtlb:
	/* Read the number of entries. */
	mfc0	t0, MIPS_CP0_CONFIG1
	ext	t0, t0, CP0_CONFIG1_MMUSIZE_POS, CP0_CONFIG1_MMUSIZE_BITS
	add	t0, t0, t4	/* Add FTLB entries. */

	/* Clear PageMask, EntryLo0/Lo1 */
	mtc0	zero, MIPS_CP0_TLB_LO0		/* Clear out PFN and valid bits */
	mtc0	zero, MIPS_CP0_TLB_LO1
	li	t6, CP0_TLB_PG_MASK
	mtc0	t6, MIPS_CP0_TLB_PG_MASK

	/* Set the EHINV bit (TLB HW Invalidate), clear the VPN2 bits. */
	ori	t3, zero, CP0_TLB_ENTRY_HI_EHINV
	mtc0	t3, MIPS_CP0_TLB_ENTRY_HI

tlb_entries:
	mtc0	t0, MIPS_CP0_TLB_INDEX	/* Use this as next TLB index */
	ehb				/* Clear hazard */
	tlbwi				/* Write the TLB entry */
	bne	t0, zero, tlb_entries	/* Branch if more TLB entries to do */
	addiu	t0, -1			/* Decrement the TLB index, Delay slot. */

	/* Clear Index and EntryHi */
	mtc0	zero, MIPS_CP0_TLB_INDEX
	mtc0	zero, MIPS_CP0_TLB_ENTRY_HI

no_tlb:
	jr	ra
	nop	/* Delay slot */




/* --------------------- */
/* init_caches           */
/* --------------------- */
llenv_init_caches:
    /* Enable parity check fo L1 & L2 */
    mfc0    t0, MIPS_CP0_ERRCTL
    lui t1, 0x8080
    or  t0, t0, t1
    mtc0    t0, MIPS_CP0_ERRCTL

    /* Invalidate L1 Instruction cache. */
    mfc0    t0, MIPS_CP0_CONFIG1
    /* Get number of sets. */
    ext t1, t0, CP0_CONFIG1_IS_POS, CP0_CONFIG1_IS_BITS
    li  t2, CP0_CONFIG1_IS_256_SETS
    beq t1, t2, get_instruction_il
    li  k0, 256     /* Delay slot */
    li  k0, 512

get_instruction_il:
    /* Get the line size. */
    ext t1, t0, CP0_CONFIG1_IL_POS, CP0_CONFIG1_IL_BITS
    li  t2, CP0_CONFIG1_IL_32_BYTE
    bne t1, t2, init_L1_data /* Go to init L1 data cache if size is not equal to 32 */
    li  k1, 32      /* Delay slot */

get_instruction_ia:
    /* Get the cache associativity. */
    ext t1, t0, CP0_CONFIG1_IA_POS, CP0_CONFIG1_IA_BITS
    li  t2, CP0_CONFIG1_IA_4_WAYS
    bne t1, t2, init_L1_data /* Go to init L1 data cache if associativity is not equal to 4 ways */
    li  t2, 4   /* Delay slot */
    mul k0, k0, t2 /* Total number of sets, considering 4-way associativity */

    li  t0, CACHED_KSEG0
    mtc0    zero, MIPS_CP0_TAG_HI1  /* Clear parity bits. */
    mtc0    zero, MIPS_CP0_TAG_LO1  /* Invalidate tag. */

icache_loop:
    cache   MIPS_CACHE_OP(L1_ICACHE, CACHE_INDEX_STORE_TAG), 0(t0)
    addi    k0, k0, -1
    bne k0, zero, icache_loop
    add t0, t0, k1  /* Delay slot */

init_L1_data:
    /* Invalidate L1 Data cache. */
    mfc0    t0, MIPS_CP0_CONFIG1
    /* Get number of sets. */
    ext t1, t0, CP0_CONFIG1_DS_POS, CP0_CONFIG1_DS_BITS
    li  t2, CP0_CONFIG1_DS_256_SETS
    beq t1, t2, get_data_dl
    li  k0, 256     /* Delay slot */
    li  k0, 512

get_data_dl:
    /* Get the line size. */
    ext t1, t0, CP0_CONFIG1_DL_POS, CP0_CONFIG1_DL_BITS
    li  t2, CP0_CONFIG1_DL_32_BYTE
    bne t1, t2, init_l2 /* Go to init_l2  */
    li  k1, 32      /* Delay slot */

get_data_da:
    /* Get the cache associativity. */
    ext t1, t0, CP0_CONFIG1_DA_POS, CP0_CONFIG1_DA_BITS
    li  t2, CP0_CONFIG1_DA_4_WAYS
    bne t1, t2, init_l2 /* Go to init_l2 */
    li  t3, 4   /* Delay slot */
    mul k0, k0, t3 /* Total number of sets, considering 4-way associativity */

    li  t0, CACHED_KSEG0
    mtc0    zero, MIPS_CP0_TAG_LO2  /* Invalidate tag. */

dcache_loop:
    cache   MIPS_CACHE_OP(L1_DCACHE, CACHE_INDEX_STORE_TAG), 0(t0)
    addi    k0, k0, -1
    bne k0, zero, dcache_loop
    add t0, t0, k1  /* Delay slot */

    /* Only Core 0 initializes L2 cache. */
    li  k0, 0
    mfc0    k1, MIPS_CP0_EBASE
    ext k1, k1, CP0_EBASE_CPUNUM_POS, CP0_EBASE_CPUNUM_BITS
    bne k1, k0, init_caches_finish
    nop
    /* Core0-specific code. */

    /* Init L2. */
init_l2:
    mfc0    t0, MIPS_CP0_CONFIG2    /* Read C0_Config2 */
get_l2_sl:
    ext t1, t0, CP0_CONFIG2_SL_POS, CP0_CONFIG2_SL_BITS     /* extract L2 line size */
    li  t2, CP0_CONFIG2_SL_32_BYTE
    beq t1, t2, get_l2_ss
    li  k0, 32      /* Delay slot */
    li  k0, 64

get_l2_ss:
    ext t1, t0, CP0_CONFIG2_SS_POS, CP0_CONFIG2_SS_BITS /* extract L2 sets per way */
    li  t2, 64                      /* base shift to determine the real number of sets per way */
    sllv    k1, t2, t1                  /* L2 Sets per way */

get_l2_sa:
    ext t1, t0, CP0_CONFIG2_SA_POS, CP0_CONFIG2_SA_BITS /* extract L2 associativity */
    li  t2, CP0_CONFIG2_SA_8_WAY
    bne t1, t2, init_caches_finish
    li  t0, 8   /* Delay slot */

    mul k1, k1, t0          /* Get total number of sets (sets per way * number of ways) */

    li  t0, CACHED_KSEG0        /* Get a KSeg0 address for cacheops */
    mtc0    zero, MIPS_CP0_L23_TAG_LO   /* Invalidate tag. */

l2_cache_loop:
    cache   MIPS_CACHE_OP(L2_CACHE, CACHE_INDEX_STORE_TAG), 0(t0)
    add k1, -1
    bne k1, zero, l2_cache_loop
    add t0, t0, k0      /* Delay slot */

init_caches_finish:
    jr  ra
    nop /* Delay slot. */
