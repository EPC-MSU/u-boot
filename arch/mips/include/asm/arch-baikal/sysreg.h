/*
 * Low-level and POST tests.
 *
 * Copyright (C) 2014 Baikal Electronics.
 *
 * Author: Alexey Malahov <Alexey.Malahov@baikalelectronics.com>
 *
 * Description: CP0 registers.
 */

#ifndef __SYSREG_MIPS_H__
#define __SYSREG_MIPS_H__

#define zero    $0
#define at      $1
#define v0      $2
#define v1      $3
#define a0      $4
#define a1      $5
#define a2      $6
#define a3      $7
#define t0      $8
#define t1      $9
#define t2      $10
#define t3      $11
#define t4      $12
#define t5      $13
#define t6      $14
#define t7      $15
#define s0      $16
#define s1      $17
#define s2      $18
#define s3      $19
#define s4      $20
#define s5      $21
#define s6      $22
#define s7      $23
#define t8      $24
#define t9      $25
#define k0      $26 /* kernel private register 0 */
#define k1      $27 /* kernel private register 1 */
#define gp      $28 /* global data pointer */
#define sp      $29 /* stack-pointer */
#define fp      $30 /* frame-pointer */
#define ra      $31 /* return address */

/* Define Coprocessor0 registers: Name, Register, Select. */

#define MIPS_CP0_TLB_INDEX          $0, 0
#define MIPS_CP0_TLB_RANDOM         $1, 0
#define MIPS_CP0_TLB_LO0            $2, 0
#define MIPS_CP0_TLB_LO1            $3, 0
#define MIPS_CP0_TLB_CONTEXT        $4, 0
#define MIPS_CP0_TLB_CONTEXT_CFG    $4, 1
#define MIPS_CP0_TLB_USER_LOCAL     $4, 2
#define MIPS_CP0_TLB_PG_MASK        $5, 0
#define MIPS_CP0_TLB_PG_GRAIN       $5, 1
#define MIPS_CP0_SEG_CTL0           $5, 2
#define MIPS_CP0_SEG_CTL1           $5, 3
#define MIPS_CP0_SEG_CTL2           $5, 4
#define MIPS_CP0_TLB_PW_BASE        $5, 5
#define MIPS_CP0_TLB_PW_FIELD       $5, 6
#define MIPS_CP0_TLB_PW_SIZE        $5, 7
#define MIPS_CP0_TLB_WIRED          $6, 0
#define MIPS_CP0_TLB_PW_CTL         $6, 6
#define MIPS_CP0_HWRENA             $7, 0
#define MIPS_CP0_BAD_VADDR          $8, 0
#define MIPS_CP0_BAD_INSTR          $8, 1
#define MIPS_CP0_BAD_INSTRP         $8, 2
#define MIPS_CP0_COUNT              $9, 0
#define MIPS_CP0_TLB_ENTRY_HI       $10, 0
#define MIPS_CP0_GUEST_CTL1         $10, 4
#define MIPS_CP0_GUEST_CTL2         $10, 5
#define MIPS_CP0_COMPARE            $11, 0
#define MIPS_CP0_GUEST_CTL0_EXT     $11, 4
#define MIPS_CP0_STATUS             $12, 0
#define MIPS_CP0_INTCTL             $12, 1
#define MIPS_CP0_SRSCTL             $12, 2
#define MIPS_CP0_GUEST_CTL0         $12, 6
#define MIPS_CP0_GT_OFFSET          $12, 7
#define MIPS_CP0_CAUSE              $13, 0
#define MIPS_CP0_EXC_PC             $14, 0
#define MIPS_CP0_EPC                MIPS_CP0_EXC_PC
#define MIPS_CP0_PRID               $15, 0
#define MIPS_CP0_EBASE              $15, 1
#define MIPS_CP0_CDMMBASE           $15, 2
#define MIPS_CP0_CMGCRBASE          $15, 3
#define MIPS_CP0_CONFIG0            $16, 0
#define MIPS_CP0_CONFIG1            $16, 1
#define MIPS_CP0_CONFIG2            $16, 2
#define MIPS_CP0_CONFIG3            $16, 3
#define MIPS_CP0_CONFIG4            $16, 4
#define MIPS_CP0_CONFIG5            $16, 5
#define MIPS_CP0_CONFIG6            $16, 6
#define MIPS_CP0_CONFIG7            $16, 7
#define MIPS_CP0_MAAR               $17, 1
#define MIPS_CP0_MAARI              $17, 2
#define MIPS_CP0_WATCH_LO           $18, 0
#define MIPS_CP0_WATCH_LO_1         $18, 1
#define MIPS_CP0_WATCH_LO_2         $18, 2
#define MIPS_CP0_WATCH_LO_3         $18, 3
#define MIPS_CP0_WATCH_HI           $19, 0
#define MIPS_CP0_WATCH_HI_1         $19, 1
#define MIPS_CP0_WATCH_HI_2         $19, 2
#define MIPS_CP0_WATCH_HI_3         $19, 3
#define MIPS_CP0_DEBUG              $23, 0
#define MIPS_CP0_TRACE_CONTROL      $23, 1
#define MIPS_CP0_TRACE_CONTROL2     $23, 2
#define MIPS_CP0_TRACE_DATA1        $23, 3
#define MIPS_CP0_TRACE_IBPC         $23, 4
#define MIPS_CP0_TRACE_DBPC         $23, 5
#define MIPS_CP0_DEPC               $24, 0
#define MIPS_CP0_TRACE_CONTROL3     $24, 2
#define MIPS_CP0_TRACE_DATA2        $24, 3
#define MIPS_CP0_PERF0_CTL          $25, 0
#define MIPS_CP0_PERF0_CNT          $25, 1
#define MIPS_CP0_PERF1_CTL          $25, 2
#define MIPS_CP0_PERF1_CNT          $25, 3
#define MIPS_CP0_PERF2_CTL          $25, 4
#define MIPS_CP0_PERF2_CNT          $25, 5
#define MIPS_CP0_PERF3_CTL          $25, 6
#define MIPS_CP0_PERF3_CNT          $25, 7
#define MIPS_CP0_ERRCTL             $26, 0
#define MIPS_CP0_CACHE_ERR          $27, 0
#define MIPS_CP0_TAG_LO1            $28, 0
#define MIPS_CP0_TAG_LO2            $28, 2
#define MIPS_CP0_DATA_LO1           $28, 1
#define MIPS_CP0_DATA_LO2           $28, 3
#define MIPS_CP0_L23_TAG_LO         $28, 4
#define MIPS_CP0_L23_DATA_LO        $28, 5
#define MIPS_CP0_TAG_HI1            $29, 0
#define MIPS_CP0_DATA_HI1           $29, 1
#define MIPS_CP0_L23_DATA_HI        $29, 5
#define MIPS_CP0_ITAG_LO            MIPS_CP0_TAG_LO1
#define MIPS_CP0_ITAG_HI            MIPS_CP0_TAG_HI1
#define MIPS_CP0_IDATA_LO           MIPS_CP0_DATA_LO1
#define MIPS_CP0_IDATA_HI           MIPS_CP0_DATA_HI1
#define MIPS_CP0_DTAG_LO            MIPS_CP0_TAG_LO2	/* the P5600 core does not implement the DTagHi register */
#define MIPS_CP0_DDATA_LO           MIPS_CP0_DATA_LO2	/* The P5600 core does not implement the DDataHi register. */
#define MIPS_CP0_ERROR_EPC          $30, 0
#define MIPS_CP0_DESAVE             $31, 0
#define MIPS_CP0_KSCRATCH1          $31, 2
#define MIPS_CP0_KSCRATCH2          $31, 3
#define MIPS_CP0_KSCRATCH3          $31, 4

/* Reset values of CP0 registers */

#define CP0_TLB_INDEX_VAL	0x0
#define CP0_TLB_RANDOM_VAL	0x0
#define CP0_TLB_CONTEXT_CFG_VAL	0x7FFFF0
#define CP0_TLB_PG_MASK_VAL	0x0
#define CP0_TLB_PG_GRAIN_VAL	0x0
#define CP0_TLB_PW_BASE_VAL	0x0
#define CP0_TLB_PW_FIELD_VAL	0x2
#define CP0_TLB_PW_SIZE_VAL	0x0
#define CP0_TLB_WIRED_VAL	0x0
#define CP0_TLB_PW_CTL_VAL	0x0
#define CP0_HWRENA_VAL		0x0
#define CP0_BAD_INSTR_VAL	0x1
#define CP0_TLB_ENTRY_HI_VAL_H	0x0
#define CP0_TLB_ENTRY_HI_VAL_L	0x0
#define CP0_GUEST_CTL1_VAL	0x0
#define CP0_GUEST_CTL2_VAL	0x0
#define CP0_GUEST_CTL0_EXT_VAL	0x40
#define CP0_STATUS_VAL		0x400004
#define CP0_STATUS_EXC_TEST_DEF	0x30000000
#define CP0_INTCTL_VAL		0x0
#define CP0_SRSCTL_VAL		0x0
#define CP0_GUEST_CTL0_VAL	0xC4C0080
#define CP0_GT_OFFSET_VAL	0x0
#define CP0_CAUSE_VAL		0x0
#define CP0_CAUSE_EXC_TEST_DEF	0x8800000
#define CP0_PRID_VAL		0x1A800
#define CP0_CDMMBASE_VAL	0x2
#define CP0_CMGCRBASE_VAL	0x2
#define CP0_MAAR_VAL_H		0x0
#define CP0_MAAR_VAL_L		0x0
#define CP0_MAARI_VAL		0x0
#define CP0_WATCH_LO_VAL	0x0
#define CP0_WATCH_HI_VAL_02	0x80000000
#define CP0_WATCH_HI_VAL_3	0x0
#define CP0_DEBUG_VAL		0x2030000
#define CP0_TRACE_CONTROL_VAL	0x0
#define CP0_TRACE_CONTROL2_VAL	0x0
#define CP0_TRACE_DATA1_VAL	0x0
#define CP0_TRACE_IBPC_VAL	0x0
#define CP0_TRACE_DBPC_VAL	0x0
#define CP0_TRACE_CONTROL3_VAL	0x20000100
#define CP0_TRACE_DATA2_VAL	0x0
#define CP0_PERF02_CTL_VAL	0x80000000
#define CP0_PERF3_CTL_VAL	0x0
#define CP0_ERRCTL_VAL		0x0
#define CP0_TAG_LO1_H_VAL	0x0
#define CP0_TAG_LO1_L_VAL	0x0
#define CP0_TAG_LO2_H_VAL	0x0
#define CP0_TAG_LO2_L_VAL	0x0
#define CP0_CONFIG0_VAL		0x80040602
#define CP0_CONFIG1_VAL		0xFEE3719B
#define CP0_CONFIG2_VAL		0x80000647
#define CP0_CONFIG3_VAL		0xBF8032E9
#define CP0_CONFIG4_VAL		0xC01CC127
#define CP0_CONFIG5_VAL		0x10000038
#define CP0_CONFIG6_VAL		0x0
#define CP0_CONFIG7_VAL		0x80014F20

/* Masks for defined reset values of CP0 registers */

#define CP0_TLB_INDEX_MASK	0x7FFFFC00
#define CP0_TLB_RANDOM_MASK	0xFFFFFFC0
#define CP0_TLB_PG_MASK_MASK	0xE0001FFF
#define CP0_TLB_ENTRY_HI_MASK_H	0xFFFFFF00
#define CP0_TLB_ENTRY_HI_MASK_L	0x1FFF
#define CP0_GUEST_CTL2_MASK	0xFFFF0000
#define CP0_STATUS_MASK		0xCFF500E4
#define CP0_INTCTL_MASK		0x7FFFFF
#define CP0_GUEST_CTL0_MASK	0xFFFFFF83
#define CP0_CAUSE_MASK		0xB1F0083
#define CP0_CDMMBASE_MASK	0xF00007FF
#define CP0_CMGCRBASE_MASK	0xF00007FF
#define CP0_MAAR_MASK_H		0x7F000000
#define CP0_MAAR_MASK_L		0xFFD
#define CP0_MAARI_MASK		0xFFFFFFC0
#define CP0_WATCH_LO_MASK	0x7
#define CP0_WATCH_HI_MASK	0xBF00F000
#define CP0_DEBUG_MASK		0x73FF83C0
#define CP0_TRACE_CONTROL_MASK	0xF0000001
#define CP0_TRACE_CONTROL2_MASK	0xFFFFF000
#define CP0_PERF_CTL_MASK	0xCE7F6010
#define CP0_ERRCTL_MASK		0xFAFFF00F
#define CP0_TAG_LO1_H_MASK	0xFFFF00
#define CP0_TAG_LO1_L_MASK	0x357
#define CP0_TAG_LO2_H_MASK	0x0
#define CP0_TAG_LO2_L_MASK	0x317

/* CP0_STATUS bits. */
#define CP0_STATUS_IE		(1 << 0)	/* Interrupt enable */
#define CP0_STATUS_EXL		(1 << 1)	/* Exception level */
#define CP0_STATUS_ERL		(1 << 2)	/* Error level */
#define CP0_STATUS_IM0		(1 << 8)	/* Mask SW0 */
#define CP0_STATUS_IM1		(1 << 9)	/* Mask SW1 */
#define CP0_STATUS_IM2		(1 << 10)	/* Mask HW0 */
#define CP0_STATUS_IM3		(1 << 11)	/* Mask HW1 */
#define CP0_STATUS_IM4		(1 << 12)	/* Mask HW2 */
#define CP0_STATUS_IM5		(1 << 13)	/* Mask HW3 */
#define CP0_STATUS_IM6		(1 << 14)	/* Mask HW4 */
#define CP0_STATUS_IM7		(1 << 15)	/* Mask HW5 */
#define CP0_STATUS_IM_HW_SHIFT	10
#define CP0_STATUS_IM_HW_MASK	(0x3F << CP0_STATUS_IM_HW_SHIFT)
#define CP0_STATUS_IM_SW_SHIFT	8
#define CP0_STATUS_IM_SW_MASK	(0x3 << CP0_STATUS_IM_SW_SHIFT)
#define CP0_STATUS_BEV		(1 << 22)	/* Boot Exception Vector */
#define CP0_STATUS_CU_MASK	(0xF << 28)	/* Allow access to all the CPs */
#define CP0_STATUS_CU_CP0_ONLY	(0x1 << 28)	/* Allow access only to CP0 the CPs */
#define CP0_STATUS_MX          	(1 << 24)       /* MIPS DSP Extension. Enables access to DSP ASE resources. */

/* MIPS_CP0_CAUSE bits. */
#define CP0_CAUSE_EXCODE_SHIFT	2		/* Shift of the ExcCode field.  */
#define CP0_CAUSE_EXCODE_POS	CP0_CAUSE_EXCODE_SHIFT
#define CP0_CAUSE_EXCODE_MASK	0x1F
#define CP0_CAUSE_EXCODE_BITS	5		/* Number of bits in the ExcCode field. */
#define CP0_CAUSE_IP0		(1 << 8)	/* Cause SW0 */
#define CP0_CAUSE_IP1		(1 << 9)	/* Cause SW1 */
#define CP0_CAUSE_IP2		(1 << 10)	/* Pending HW0 */
#define CP0_CAUSE_IP3		(1 << 11)	/* Pending HW1 */
#define CP0_CAUSE_IP4		(1 << 12)	/* Pending HW2 */
#define CP0_CAUSE_IP5		(1 << 13)	/* Pending HW3 */
#define CP0_CAUSE_IP6		(1 << 14)	/* Pending HW4 */
#define CP0_CAUSE_IP7		(1 << 15)	/* Pending HW5 */
#define CP0_CAUSE_IP_HW_SHIFT	10
#define CP0_CAUSE_IP_HW_POS	CP0_CAUSE_IP_HW_SHIFT
#define CP0_CAUSE_IP_HW_MASK	0x3F
#define CP0_CAUSE_IP_HW_BITS	6		/* Number of bits in the IP_HW field. */
#define CP0_CAUSE_IP_SW_SHIFT	8
#define CP0_CAUSE_IP_SW_POS	CP0_CAUSE_IP_SW_SHIFT
#define CP0_CAUSE_IP_SW_MASK	0x3
#define CP0_CAUSE_IP_SW_BITS	2		/* Number of bits in the IP_SW field. */
#define CP0_CAUSE_WP		(1 << 22)	/* Watch exception. */
#define CP0_CAUSE_WP_POS	22
#define CP0_CAUSE_WP_BITS	1
#define CP0_CAUSE_IV		(1 << 23)	/* Use the special interrupt vector. */
#define CP0_CAUSE_DC		(1 << 27)	/* Disable count register. */
#define CP0_CAUSE_TI		(1 << 30)	/* Timer interrupt is pending. */

/* MIPS_CP0_CONFIG0 bits. */
#define CP0_CONFIG0_MT_SHIFT	7		/* MMU type. */
#define CP0_CONFIG0_MT_SIZE	3		/* 3 bits */
#define CP0_CONFIG0_MT_POS	CP0_CONFIG0_MT_SHIFT
#define	CP0_CONFIG0_MT_NONE	0		/* None */
#define	CP0_CONFIG0_MT_TLB	1		/* Standard TLB. */
#define	CP0_CONFIG0_MT_BAT	2		/* Block Address Translation */
#define	CP0_CONFIG0_MT_FIX_MAP	3		/* Fixed Mapping. */
#define	CP0_CONFIG0_MT_DUAL	4		/* Dual VTLB and FTLB. */
#define	CP0_CONFIG0_VI		(1 << 3)	/* Virtual instruction cache. */
#define	CP0_CONFIG0_K0_UNCACHE	2		/* Kseg0 cacheability and coherency attribute */
#define	CP0_CONFIG0_K0_WB	3		/* Cacheable, non-coherent, write-back, write allocate. */
#define	CP0_CONFIG0_K0_CWBE	4		/* Cacheable, coherent, write-back, write-allocate, read misses request Exclusive */
#define	CP0_CONFIG0_K0_CWB	5		/* Cacheable, coherent, write-back, write-allocate, read misses request Shared */
#define	CP0_CONFIG0_K0_UCA	7		/* Uncached Accelerated, non-coherent */
#define	CP0_CONFIG0_K0_POS	0
#define	CP0_CONFIG0_K0_BITS	3
#define	CP0_CONFIG0_K0_MASK	7

/* MIPS_CP0_CONFIG1 bits. */
#define CP0_CONFIG1_MMUSIZE_SHIFT	25	/* Number of entries in the TLB minus 1. */
#define CP0_CONFIG1_MMUSIZE_BITS	6	/* Number of bits in the register. */
#define CP0_CONFIG1_MMUSIZE_POS		CP0_CONFIG1_MMUSIZE_SHIFT
#define CP0_CONFIG1_MMUSIZE_MASK	0x3F
#define CP0_CONFIG1_IS_SHIFT		22	/* L1 Instruction cache number of sets per way. */
#define CP0_CONFIG1_IS_POS		CP0_CONFIG1_IS_SHIFT
#define CP0_CONFIG1_IS_BITS		3
#define CP0_CONFIG1_IS_256_SETS		2	/* 256 sets per way, 32 KByte instruction cache */
#define CP0_CONFIG1_IS_512_SETS		3	/* 512 sets per way, 64 KByte instruction cache */
#define CP0_CONFIG1_IL_SHIFT		19	/* L1 Instruction cache line size. */
#define CP0_CONFIG1_IL_POS		CP0_CONFIG1_IL_SHIFT
#define CP0_CONFIG1_IL_BITS		3
#define CP0_CONFIG1_IL_32_BYTE		4	/* 32 byte line size. */
#define CP0_CONFIG1_IA_SHIFT		16	/* L1 Instruction cache associativity. */
#define CP0_CONFIG1_IA_POS		CP0_CONFIG1_IA_SHIFT
#define CP0_CONFIG1_IA_BITS		3
#define CP0_CONFIG1_IA_4_WAYS		3
#define CP0_CONFIG1_DS_SHIFT		13	/* L1 Data cache number of sets per way. */
#define CP0_CONFIG1_DS_POS		CP0_CONFIG1_DS_SHIFT
#define CP0_CONFIG1_DS_BITS		3
#define CP0_CONFIG1_DS_256_SETS		2	/* 256 sets per way, 32 KByte data cache */
#define CP0_CONFIG1_DS_512_SETS		3	/* 512 sets per way, 64 KByte data cache */
#define CP0_CONFIG1_DL_SHIFT		10	/* L1 Data cache line size. */
#define CP0_CONFIG1_DL_POS		CP0_CONFIG1_DL_SHIFT
#define CP0_CONFIG1_DL_BITS		3
#define CP0_CONFIG1_DL_32_BYTE		4	/* 32 byte line size. */
#define CP0_CONFIG1_DA_SHIFT		7	/* L1 Data cache associativity. */
#define CP0_CONFIG1_DA_POS		CP0_CONFIG1_DA_SHIFT
#define CP0_CONFIG1_DA_BITS		3
#define CP0_CONFIG1_DA_4_WAYS		3

 /* MIPS_CP0_CONFIG2 bits. */
#define CP0_CONFIG2_L2B	(1 << 12)	/* L2 cache bybass */
#define CP0_CONFIG2_L2B_POS	12
#define CP0_CONFIG2_L2B_BITS	1
#define CP0_CONFIG2_SS_SHIFT	8	/* L2 cache number of sets per way */
#define CP0_CONFIG2_SS_POS	CP0_CONFIG2_SS_SHIFT
#define CP0_CONFIG2_SS_BITS	4
#define CP0_CONFIG2_SS_MASK	(0xF << CP0_CONFIG2_SS_SHIFT)
#define CP0_CONFIG2_SS_512_SETS	3	/* 512 sets per way */
#define CP0_CONFIG2_SS_1024_SETS	4	/* 1024 sets per way */
#define CP0_CONFIG2_SS_2048_SETS	5	/* 2048 sets per way */
#define CP0_CONFIG2_SS_4096_SETS	6	/* 4096 sets per way */
#define CP0_CONFIG2_SS_8192_SETS	7	/* 8192 sets per way */
#define CP0_CONFIG2_SS_16384_SETS	8	/* 16384 sets per way */
#define CP0_CONFIG2_SS_32768_SETS	9	/* 32768 sets per way */
#define CP0_CONFIG2_SL_SHIFT	4	/* L2 data cache line size */
#define CP0_CONFIG2_SL_POS	CP0_CONFIG2_SL_SHIFT
#define CP0_CONFIG2_SL_BITS	4
#define CP0_CONFIG2_SL_MASK	(0xF << CP0_CONFIG2_SL_SHIFT)
#define CP0_CONFIG2_SL_32_BYTE	4	/* 32 byte line size. */
#define CP0_CONFIG2_SL_64_BYTE	5	/* 64 byte line size. */
#define CP0_CONFIG2_SA_SHIFT	0	/* L2 cache associativity */
#define CP0_CONFIG2_SA_POS	CP0_CONFIG2_SA_SHIFT
#define CP0_CONFIG2_SA_BITS	4
#define CP0_CONFIG2_SA_MASK	(0xF << CP0_CONFIG2_SA_SHIFT)
#define CP0_CONFIG2_SA_8_WAY	7	/* 8-way associative L2 cache. */


/* MIPS_CP0_CONFIG3 bits. */
#define CP0_CONFIG3_VEIC_POS		6
#define CP0_CONFIG3_VEIC_BITS		1
#define CP0_CONFIG3_VEIC		(1 << CP0_CONFIG3_VEIC_POS)
#define CP0_CONFIG3_LPA_POS     	7
#define CP0_CONFIG3_LPA        	(1 << CP0_CONFIG3_LPA_POS)


/* MIPS_CP0_CONFIG4 bits. */
#define CP0_CONFIG4_MMU_EXTDEF_POS	14
#define CP0_CONFIG4_MMU_EXTDEF_BITS	2
#define CP0_CONFIG4_MMU_EXTDEF_MASK	3
#define CP0_CONFIG4_MMU_EXTDEF_SIZE	1	/* Config4[7:0] used as MMUSizeExt. */
#define CP0_CONFIG4_MMU_EXTDEF_FTLB	3	/* Config4[3:0] - FTLB ways Config4[7:4] - FTLB sets */
#define CP0_CONFIG4_MMU_SIZE_EXT_POS	0
#define CP0_CONFIG4_MMU_SIZE_EXT_BITS	8
#define CP0_CONFIG4_MMU_SIZE_EXT_MASK	0xFF
#define CP0_CONFIG4_FTLB_PS_POS		8	/* FTLB Page Size. */
#define CP0_CONFIG4_FTLB_PS_BITS	5
#define CP0_CONFIG4_FTLB_WAYS_POS	4	/* FTLB Ways. */
#define CP0_CONFIG4_FTLB_WAYS_BITS	4
#define CP0_CONFIG4_FTLB_SETS_POS	0	/* FTLB Sets. */
#define CP0_CONFIG4_FTLB_SETS_BITS	4
#define CP0_CONFIG4_P5600_FTLB_SIZE	512	/* FTLB is organized as 128 sets and 4-ways */

/* MIPS_CP0_CONFIG5 bits. */
#define CP0_CONFIG5_K			(1 << 30)

/* MIPS_CP0_CONFIG6 bits. */
#define CP0_CONFIG6_FTLBEN		(1 << 15)   /* FTLB enable. */
#define CP0_CONFIG6_DLSB		(1 << 21)   /* Disable load/store bonding. */
#define CP0_CONFIG6_JRCD		(1 << 0)    /* Jump register cache prediction disable. */

/* MIPS_CP0_CONFIG7 bits. */
#define CP0_CONFIG7_HCI_POS		18	/* Hardware Cache Initialization */
#define CP0_CONFIG7_HCI_BITS		1
#define CP0_CONFIG7_RPS			(1 << 2)    /* Return prediction stack. */

/* MIPS_CP0_PRID bits */
#define CP0_PRID_CoID_SHIFT     16
#define CP0_PRID_CoID_MASK      (0xFF << CP0_PRID_CoID_SHIFT)
#define CP0_PRID_CoID_Imagination   1
#define CP0_PRID_ProcType_SHIFT     8
#define CP0_PRID_ProcType_MASK      (0xFF << CP0_PRID_ProcType_SHIFT)
#define CP0_PRID_ProcType_P5600     0xA8

#define CACHED_KSEG0            0x80000000
#define CACHED_KSEG3            0xE0000000
#define LEGACY_KSEG0            CACHED_KSEG0
#define LEGACY_KSEG1            0xA0000000
#define LEGACY_KSEG2            0xC0000000
#define LEGACY_KSEG3            CACHED_KSEG3
#define LEGACY_BOOT_EXP_ADDR        0xBFC00000
#define CACHED_ADDR(a)          (CACHED_KSEG0 | ((0x1FFFFFFF) & (a)))
#define KSEG1_ADDR(a)           (LEGACY_KSEG1 | ((0x1FFFFFFF) & (a)))
#define CACHED_FUNC(type, func)          (type (*)())(CACHED_ADDR((unsigned int)func))

#define EVA_CFG0            0xE0000000
#define EVA_CFG5            0x00000000

#define L1_ICACHE           0   /* Primary instruction cache. */
#define L1_DCACHE           1   /* Primary data cache. */
#define L2_CACHE            3   /* Secondary cache. */

/* Operations with L1/L2 caches. */
#define CACHE_INDEX_INV         0   /* Index Invalidate */
#define CACHE_INDEX_LOAD_TAG        1   /* Index Load Tag */
#define CACHE_INDEX_STORE_TAG       2   /* Index Store Tag */
#define CACHE_INDEX_RESERVED        3
#define CACHE_INDEX_STORE_DATA      CACHE_INDEX_RESERVED
#define CACHE_INDEX_HIT_INV     4   /* Hit Invalidate */
#define CACHE_INDEX_HIT_WR_INV      5   /* Hit Writeback Invalidate */
#define CACHE_INDEX_HIT_WR      6   /* Hit Writeback */
#define CACHE_INDEX_LOCK        7   /* Fetch and Lock */

#define CACHE_OP_SHIFT          2
#define MIPS_CACHE_OP(c, o)     (((o) << CACHE_OP_SHIFT) | (c))
#define MIPS_CACHE(op, off) do {                     \
    asm volatile(                   \
    "cache %0, %1\n"                    \
    :: "i" (op), "R" (*((unsigned char *)(off))));     \
} while (0)


/* MIPS_CP0_TAG */
#define CP0_DTAG_LO_PTAG_SHIFT  12
#define CP0_DTAG_LO_PTAG_MASK   0x1FFFF000  /* The cache address tag */
#define CP0_DTAG_LO_V11     (1 << 11)   /* This bit always gets the virtual address bit [11] */
#define CP0_DTAG_LO_PTAG_V  (1 << 7)    /* Valid entry. */
#define CP0_TAG_LRU_SHIFT   10      /* LRU bits. */
#define CP0_TAG_LRU_MASK    0x0000FC00  /* LRU mask. */

/* MIPS_CP0_ERRCTL */
#define CP0_ERRCTL_PO       (1 << 30)   /* Parity Overwrite. */
#define CP0_ERRCTL_WST      (1 << 29)   /* Test mode, way-selection bit. */
#define CP0_ERRCTL_DYT      (1 << 21)   /* Allows cache load/store data operations on the "dirty array" */
#define CP0_ERRCTL_PO       (1 << 30)   /* Parity Overwrite */

/* MIPS_CP0_EBASE */
#define CP0_EBASE_CPUNUM_SHIFT      0   /* Number of CPUs. */
#define CP0_EBASE_CPUNUM_POS        CP0_EBASE_CPUNUM_SHIFT
#define CP0_EBASE_CPUNUM_BITS       10  /* Number of bits in the register. */
#define CP0_EBASE_CPUNUM_MASK       0x3FF
#define CP0_EBASE_EXCBASE_SHIFT     12
#define CP0_EBASE_EXCBASE_POS       CP0_EBASE_EXCBASE_SHIFT
#define CP0_EBASE_EXCBASE_BITS      20

/* MIPS_CP0_SEG_CTL{0, 1, 2} */
#define CP0_SEG_CFG_C_SHIFT     0   /* Cache coherency attribute. */
#define CP0_SEG_CFG_EU_SHIFT        3   /* Error condition behavior. */
#define CP0_SEG_CFG_AM_SHIFT        4   /* Access control mode. */
#define CP0_SEG_CFG_PA_SHIFT        9   /* Physical address. */

#define CP0_SEG_CFG_NEXT_SHIFT      ( CP0_SEG_CFG_C_SHIFT + CP0_SEG_CFG_EU_SHIFT + \
                    CP0_SEG_CFG_AM_SHIFT + CP0_SEG_CFG_PA_SHIFT)

#define SET_SEG_CFG(pa, am, eu, c)  ((pa << CP0_SEG_CFG_PA_SHIFT) | (am << CP0_SEG_CFG_AM_SHIFT) | \
                    (eu << CP0_SEG_CFG_EU_SHIFT) | (c << CP0_SEG_CFG_C_SHIFT))
#define SEG_CFG_X           0

/* Define the default values for SegCtl registers:  PA, AM, EU, C */
#define SEG_CFG0_LEGACY_DFLT        SET_SEG_CFG(0, 1, 0, SEG_CFG_X)
#define SEG_CFG1_LEGACY_DFLT        SET_SEG_CFG(0, 2, 0, SEG_CFG_X)
#define SEG_CFG2_LEGACY_DFLT        SET_SEG_CFG(0, 0, 0, 2)
#define SEG_CFG3_LEGACY_DFLT        SET_SEG_CFG(0, 0, 0, 5)
#define SEG_CFG4_LEGACY_DFLT        SET_SEG_CFG(2, 3, 1, SEG_CFG_X)
#define SEG_CFG5_LEGACY_DFLT        SET_SEG_CFG(0, 3, 1, SEG_CFG_X)

#define CP0_SEG_CTL0_LEGACY_MASK    0xFFF8FFF8
#define CP0_SEG_CTL0_LEGACY_VAL     (SEG_CFG0_LEGACY_DFLT | \
                    (SEG_CFG1_LEGACY_DFLT << CP0_SEG_CFG_NEXT_SHIFT))
#define CP0_SEG_CTL1_LEGACY_VAL     (SEG_CFG2_LEGACY_DFLT | \
                    (SEG_CFG3_LEGACY_DFLT << CP0_SEG_CFG_NEXT_SHIFT))
#define CP0_SEG_CTL2_LEGACY_MASK    0xFFF8FFF8
#define CP0_SEG_CTL2_LEGACY_VAL     (SEG_CFG4_LEGACY_DFLT | \
                    (SEG_CFG5_LEGACY_DFLT << CP0_SEG_CFG_NEXT_SHIFT))


#define SEG_CFG0_EVA_DFLT       SET_SEG_CFG(SEG_CFG_X, 1, 0, SEG_CFG_X)
#define SEG_CFG1_EVA_DFLT       SET_SEG_CFG(SEG_CFG_X, 1, 0, SEG_CFG_X)
#define SEG_CFG2_EVA_DFLT       SET_SEG_CFG(5, 4, 1, 3)
#define SEG_CFG3_EVA_DFLT       SET_SEG_CFG(4, 4, 1, 3)
#define SEG_CFG4_EVA_DFLT       SET_SEG_CFG(2, 4, 1, 3)
#define SEG_CFG5_EVA_DFLT       SET_SEG_CFG(0, 4, 1, 3)

#define SEG_CFG0_EVA_TEMP       SET_SEG_CFG(0, 0, 0, 2)
#define SEG_CFG_EVA_MK          SET_SEG_CFG(SEG_CFG_X, 1, 1, 3) /* Mapped Kernel region */
#define SEG_CFG_EVA_MUSUK       SET_SEG_CFG(SEG_CFG_X, 4, 1, 3) /* Mapped user/supervisor, unmapped kernel region. */


/* MIPS_CP0_L23_TAG */
#define CP0_L23_PTAG_SHIFT      14
#define CP0_L23_8M_PTAG_MASK        0x1FFFC000  /* The cache address tag - [31:14], HUM p5600. */
#define CP0_L23_1M_PTAG_MASK        0x1FFE0000  /* The cache address tag - [31:17] */
#define CP0_L23_TP_SHIFT        8
#define CP0_L23_TP          (1 << CP0_DTAG_L23_SHIFT)   /* Total parity. */
#define CP0_L23_V           (1 << 7)    /* Valid entry. */
#define CP0_L23_UNDEFINED       0x3E00      /* Undefined bits. */

/* CP0_TLB_ENTRY_HI */
#define CP0_TLB_ENTRY_HI_EHINV      0x400   /* TLB HW Invalidate */
#define CP0_TLB_ENTRY_HI_VPN2_SHIFT 13

/* CP0_TLB_LO0/1 */
#define CP0_TLB_LO_PFN_SHIFT        6
#define CP0_TLB_LO_CACHE_SHIFT      3
#define CP0_TLB_LO_UNCACHED     2
#define CP0_TLB_LO_CACHEABLE        3

#define CP0_TLB_C_UNCACHE       2   /* Uncached, non-coherent attribute */
#define CP0_TLB_C_WB            3   /* Cacheable, non-coherent, write-back, write allocate. */
#define CP0_TLB_C_CWBE          4   /* Cacheable, coherent, write-back, write-allocate, read misses request Exclusive */
#define CP0_TLB_C_CWB           5   /* Cacheable, coherent, write-back, write-allocate, read misses request Shared */
#define CP0_TLB_C_UCA           7   /* Uncached Accelerated, non-coherent */

#define CP0_TLB_LO_DIRTY        (1 << 2)
#define CP0_TLB_LO_VALID        (1 << 1)
#define CP0_TLB_LO_GLOBAL       (1 << 0)

#define CP0_TLB_LO_RI       (1 << 31)
#define CP0_TLB_LO_XI       (1 << 30)

#define TLB_MASK_UNCACHED       (CP0_TLB_LO_DIRTY | CP0_TLB_LO_VALID | \
                    (CP0_TLB_C_UNCACHE << CP0_TLB_LO_CACHE_SHIFT))

#define TLB_MASK_CACHED_NONCOH      (CP0_TLB_LO_DIRTY | CP0_TLB_LO_VALID | \
                    (CP0_TLB_C_WB << CP0_TLB_LO_CACHE_SHIFT))

#define TLB_MASK_CACHED_COH     (CP0_TLB_LO_DIRTY | CP0_TLB_LO_VALID | \
                    (CP0_TLB_C_CWB << CP0_TLB_LO_CACHE_SHIFT))

/* CP0_TLB_PG_GRAIN */
#define CP0_TLB_PG_GRAIN_RIE    (1 << 31)
#define CP0_TLB_PG_GRAIN_XIE    (1 << 30)
#define CP0_TLB_PG_GRAIN_ELPA   (1 << 29)
#define CP0_TLB_PG_GRAIN_IEC    (1 << 27)

/* CP0_TLB_PG_MASK */
#define CP0_PAGE_SIZE           (4 * 1024)   /* 4kB */
#define CP0_TLB_PG_MASK         (((CP0_PAGE_SIZE * 2) - 1) & ~0x1FFF)
#define CP0_TLB_PFN_SHIFT       12

/* MIPS_CP0_INTCTL */
#define CP0_INTCTL_VS_SHIFT     5   /* Vector spacing. */
#define CP0_INTCTL_VS_POS       CP0_INTCTL_VS_SHIFT
#define CP0_INTCTL_VS_BITS      5
#define CP0_INTCTL_VS_0         0x00    /* Spacing between vectors. */
#define CP0_INTCTL_VS_32        0x01
#define CP0_INTCTL_VS_64        0x02
#define CP0_INTCTL_VS_128       0x04
#define CP0_INTCTL_VS_256       0x08
#define CP0_INTCTL_VS_512       0x10

#define CP0_INTCTL_VS           CP0_INTCTL_VS_32
#define CP0_INTCTL_VS_SPACE     (CP0_INTCTL_VS << CP0_INTCTL_VS_SHIFT)

/* MIPS_CP0_WATCH HI/LO */
#define MIPS_CP0_WATCH_SHIFT        3
#define MIPS_CP0_WATCH_W        (1 << 0)
#define MIPS_CP0_WATCH_R        (1 << 1)
#define MIPS_CP0_WATCH_I        (1 << 2)
#define MIPS_CP0_WATCH_IRW      (0x7 )
#define MIPS_CP0_WATCH_HI_G     (1 << 30)

/* MIPS_CP0_DEBUG_DM */
#define MIPS_CP0_DEBUG_DM       (1<<30)

/*  List of FP control registers*/
#define MIPS_CP1_FIR        $0
#define CP1_FIR_RESETVAL    0x30f30320
#define MIPS_CP1_FCSR       $f31
#define MIPS_CP1_DEFAULT    (0xC<<16)

/*  List of FP exception in FCSR_CP1 register*/
#define MIPS_CP1_FCSR_EX_Z  (MIPS_CP1_DEFAULT |  0x8020 )
#define MIPS_CP1_FCSR_EX_V  (MIPS_CP1_DEFAULT | 0x10040 )
#define MIPS_CP1_FCSR_EX_OI (MIPS_CP1_DEFAULT |  0x5014 )
#define MIPS_CP1_FCSR_EX_UI (MIPS_CP1_DEFAULT |  0x300C )
#define MIPS_CP1_FCSR_EX_I  (MIPS_CP1_DEFAULT |  0x1004 )

/* Exception codes. */
#define EXCODE_INT  0   /* 0x00 Interrupt */
#define EXCODE_TLBM 1   /* 0x01 TLB modification exception */
#define EXCODE_TLBL 2   /* 0x02 TLB exception (load or instruction fetch) */
#define EXCODE_TLBS 3   /* 0x03 TLB exception (store) */
#define EXCODE_ADEL 4   /* 0x04 Address error exception (load or instruction fetch) */
#define EXCODE_ADES 5   /* 0x05 Address error exception (store) */
#define EXCODE_IBE  6   /* 0x06 Bus error exception (instruction fetch) */
#define EXCODE_DBE  7   /* 0x07 Bus error exception (data reference: load or store) */
#define EXCODE_SYS  8   /* 0x08 Syscall exception */
#define EXCODE_BP   9   /* 0x09 Breakpoint exception. */
#define EXCODE_RI   10  /* 0x0a Reserved instruction exception */
#define EXCODE_CPU  11  /* 0x0b Coprocessor Unusable exception */
#define EXCODE_OV   12  /* 0x0c Arithmetic Overflow exception */
#define EXCODE_TR   13  /* 0x0d Trap exception */
#define EXCODE_MSAFPE   14  /* 0x0e MSA Floating Point exception */
#define EXCODE_FPE  15  /* 0x0f Floating point exception */
#define EXCODE_IMP1 16  /* 0x10 Available for implementation dependent use */
#define EXCODE_IMP2 17  /* 0x11 Available for implementation dependent use */
#define EXCODE_C2E  18  /* 0x12 Reserved for precise Coprocessor 2 exceptions */
#define EXCODE_TLBRI    19  /* 0x13 TLB Read-Inhibit exception */
#define EXCODE_TLBXI    20  /* 0x14 TLB Execution-Inhibit exception */
#define EXCODE_MSADIS   21  /* 0x15 MSA Disabled exception */
#define EXCODE_MDMX 22  /* 0x16 Previously MDMX Unusable Exception (MDMX ASE). MDMX deprecated with Revision 5. */
#define EXCODE_WATCH    23  /* 0x17 Reference to WatchHi/WatchLo address */
#define EXCODE_MC   24  /* 0x18 Machine check */
#define EXCODE_THREAD   25  /* 0x19 Thread Allocation, Deallocation, or Scheduling Exceptions (MIPS MT Module) */
#define EXCODE_DSP  26  /* 0x1a DSP Module State Disabled exception (MIPS DSP Module) */
#define EXCODE_GE   27  /* 0x1b Virtualized Guest Exception */
#define EXCODE_RES1 28  /* 0x1c Reserved */
#define EXCODE_RES2 29  /* 0x1d Reserved */
#define EXCODE_CERR 30  /* 0x1e Cache error. */
#define EXCODE_RES3 31  /* 0x1f Reserved */

/* MSA reg codes */
#define MSAIR                   $0
#define MSAIR_RESETVAL          0x320
#define MSACSR                  $1
#define MSASave                 $3

#define MSAIR_VRP               (1 << 16)

#define MSACSR_NX               (1 << 18)
#define MSACSR_CAUSE_SHIFT      12
#define MSACSR_CAUSE_MASK       0x3F
#define MSACSR_ENABLE_SHIFT     7
#define MSACSR_ENABLE_MASK      0x1F
#define MSACSR_FLAGS_SHIFT      2
#define MSACSR_FLAGS_MASK       0x1F

#define MSACSR_CAUSE_E          (1 << 17)
#define MSACSR_CAUSE_V          (1 << 16)
#define MSACSR_CAUSE_Z          (1 << 15)
#define MSACSR_CAUSE_O          (1 << 14)
#define MSACSR_CAUSE_U          (1 << 13)
#define MSACSR_CAUSE_I          (1 << 12)

#define MSACSR_ENABLE_V         (1 << 11)
#define MSACSR_ENABLE_Z         (1 << 10)
#define MSACSR_ENABLE_O         (1 << 9)
#define MSACSR_ENABLE_U         (1 << 8)
#define MSACSR_ENABLE_I         (1 << 7)

#define MSACSR_FLAGS_V          (1 << 6)
#define MSACSR_FLAGS_Z          (1 << 5)
#define MSACSR_FLAGS_O          (1 << 4)
#define MSACSR_FLAGS_U          (1 << 3)
#define MSACSR_FLAGS_I          (1 << 2)

#define NAN                     0x7F800000

#define FLOAT_EXC_V             (1 << 4)
#define FLOAT_EXC_Z             (1 << 3)
#define FLOAT_EXC_O             (1 << 2)
#define FLOAT_EXC_U             (1 << 1)
#define FLOAT_EXC_I             (1 << 0)


/* Processor Identification */
#define CP0_PRID_PID_POS    8
#define CP0_PRID_PID_BITS   8

/* CPU type of p5600. */
#define CPU_P5600   0xA8




#endif /* __SYSREG_MIPS_H__ */
