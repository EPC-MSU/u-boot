/*
 * cpu.h: Values of the PRId register used to match up
 *	  various MIPS cpu types.
 *
 * Copyright (C) 1996 David S. Miller (davem@davemloft.net)
 * Copyright (C) 2004, 2013  Maciej W. Rozycki
 */
#ifndef _ASM_CPU_H
#define _ASM_CPU_H

/*
   As of the MIPS32 and MIPS64 specs from MTI, the PRId register (CP0
   register 15, select 0) is defined in this (backwards compatible) way:

  +----------------+----------------+----------------+----------------+
  | Company Options| Company ID	    | Processor ID   | Revision	      |
  +----------------+----------------+----------------+----------------+
   31		 24 23		  16 15		    8 7

   I don't have docs for all the previous processors, but my impression is
   that bits 16-23 have been 0 for all MIPS processors before the MIPS32/64
   spec.
*/

#define PRID_OPT_MASK		0xff000000

/*
 * Assigned Company values for bits 23:16 of the PRId register.
 */

#define PRID_COMP_MASK		0xff0000

#define PRID_COMP_LEGACY	0x000000
#define PRID_COMP_MIPS		0x010000
#define PRID_COMP_BROADCOM	0x020000
#define PRID_COMP_ALCHEMY	0x030000
#define PRID_COMP_SIBYTE	0x040000
#define PRID_COMP_SANDCRAFT	0x050000
#define PRID_COMP_NXP		0x060000
#define PRID_COMP_TOSHIBA	0x070000
#define PRID_COMP_LSI		0x080000
#define PRID_COMP_LEXRA		0x0b0000
#define PRID_COMP_NETLOGIC	0x0c0000
#define PRID_COMP_CAVIUM	0x0d0000
#define PRID_COMP_INGENIC_D0	0xd00000	/* JZ4740, JZ4750 */
#define PRID_COMP_INGENIC_D1	0xd10000	/* JZ4770, JZ4775 */
#define PRID_COMP_INGENIC_E1	0xe10000	/* JZ4780 */

/*
 * Assigned Processor ID (implementation) values for bits 15:8 of the PRId
 * register.  In order to detect a certain CPU type exactly eventually
 * additional registers may need to be examined.
 */

#define PRID_IMP_MASK		0xff00

/*
 * These are valid when 23:16 == PRID_COMP_LEGACY
 */

#define PRID_IMP_R2000		0x0100
#define PRID_IMP_AU1_REV1	0x0100
#define PRID_IMP_AU1_REV2	0x0200
#define PRID_IMP_R3000		0x0200		/* Same as R2000A  */
#define PRID_IMP_R6000		0x0300		/* Same as R3000A  */
#define PRID_IMP_R4000		0x0400
#define PRID_IMP_R6000A		0x0600
#define PRID_IMP_R10000		0x0900
#define PRID_IMP_R4300		0x0b00
#define PRID_IMP_VR41XX		0x0c00
#define PRID_IMP_R12000		0x0e00
#define PRID_IMP_R14000		0x0f00		/* R14K && R16K */
#define PRID_IMP_R8000		0x1000
#define PRID_IMP_PR4450		0x1200
#define PRID_IMP_R4600		0x2000
#define PRID_IMP_R4700		0x2100
#define PRID_IMP_TX39		0x2200
#define PRID_IMP_R4640		0x2200
#define PRID_IMP_R4650		0x2200		/* Same as R4640 */
#define PRID_IMP_R5000		0x2300
#define PRID_IMP_TX49		0x2d00
#define PRID_IMP_SONIC		0x2400
#define PRID_IMP_MAGIC		0x2500
#define PRID_IMP_RM7000		0x2700
#define PRID_IMP_NEVADA		0x2800		/* RM5260 ??? */
#define PRID_IMP_RM9000		0x3400
#define PRID_IMP_LOONGSON_32	0x4200  /* Loongson-1 */
#define PRID_IMP_R5432		0x5400
#define PRID_IMP_R5500		0x5500
#define PRID_IMP_LOONGSON_64	0x6300  /* Loongson-2/3 */

#define PRID_IMP_UNKNOWN	0xff00

/*
 * These are the PRID's for when 23:16 == PRID_COMP_MIPS
 */
#define PRID_MIPS_MASK		0x00ff0000

#define PRID_IMP_QEMU_GENERIC	0x0000
#define PRID_IMP_4KC		0x8000
#define PRID_IMP_5KC		0x8100
#define PRID_IMP_20KC		0x8200
#define PRID_IMP_4KEC		0x8400
#define PRID_IMP_4KSC		0x8600
#define PRID_IMP_25KF		0x8800
#define PRID_IMP_5KE		0x8900
#define PRID_IMP_4KECR2		0x9000
#define PRID_IMP_4KEMPR2	0x9100
#define PRID_IMP_4KSD		0x9200
#define PRID_IMP_24K		0x9300
#define PRID_IMP_34K		0x9500
#define PRID_IMP_24KE		0x9600
#define PRID_IMP_74K		0x9700
#define PRID_IMP_1004K		0x9900
#define PRID_IMP_1074K		0x9a00
#define PRID_IMP_M14KC		0x9c00
#define PRID_IMP_M14KEC		0x9e00
#define PRID_IMP_INTERAPTIV_UP	0xa000
#define PRID_IMP_INTERAPTIV_MP	0xa100
#define PRID_IMP_PROAPTIV_UP	0xa200
#define PRID_IMP_PROAPTIV_MP	0xa300
#define PRID_IMP_M5150		0xa700
#define PRID_IMP_P5600		0xa800
#define PRID_IMP_I6400		0xa900

/*
 * These are the PRID's for when 23:16 == PRID_COMP_SIBYTE
 */

#define PRID_IMP_SB1		0x0100
#define PRID_IMP_SB1A		0x1100

/*
 * These are the PRID's for when 23:16 == PRID_COMP_SANDCRAFT
 */

#define PRID_IMP_SR71000	0x0400

/*
 * These are the PRID's for when 23:16 == PRID_COMP_BROADCOM
 */

#define PRID_IMP_BMIPS32_REV4	0x4000
#define PRID_IMP_BMIPS32_REV8	0x8000
#define PRID_IMP_BMIPS3300	0x9000
#define PRID_IMP_BMIPS3300_ALT	0x9100
#define PRID_IMP_BMIPS3300_BUG	0x0000
#define PRID_IMP_BMIPS43XX	0xa000
#define PRID_IMP_BMIPS5000	0x5a00
#define PRID_IMP_BMIPS5200	0x5b00

#define PRID_REV_BMIPS4380_LO	0x0040
#define PRID_REV_BMIPS4380_HI	0x006f

/*
 * These are the PRID's for when 23:16 == PRID_COMP_CAVIUM
 */

#define PRID_IMP_CAVIUM_CN38XX 0x0000
#define PRID_IMP_CAVIUM_CN31XX 0x0100
#define PRID_IMP_CAVIUM_CN30XX 0x0200
#define PRID_IMP_CAVIUM_CN58XX 0x0300
#define PRID_IMP_CAVIUM_CN56XX 0x0400
#define PRID_IMP_CAVIUM_CN50XX 0x0600
#define PRID_IMP_CAVIUM_CN52XX 0x0700
#define PRID_IMP_CAVIUM_CN63XX 0x9000
#define PRID_IMP_CAVIUM_CN68XX 0x9100
#define PRID_IMP_CAVIUM_CN66XX 0x9200
#define PRID_IMP_CAVIUM_CN61XX 0x9300
#define PRID_IMP_CAVIUM_CNF71XX 0x9400
#define PRID_IMP_CAVIUM_CN78XX 0x9500
#define PRID_IMP_CAVIUM_CN70XX 0x9600

/*
 * These are the PRID's for when 23:16 == PRID_COMP_INGENIC_*
 */

#define PRID_IMP_JZRISC	       0x0200

/*
 * These are the PRID's for when 23:16 == PRID_COMP_NETLOGIC
 */
#define PRID_IMP_NETLOGIC_XLR732	0x0000
#define PRID_IMP_NETLOGIC_XLR716	0x0200
#define PRID_IMP_NETLOGIC_XLR532	0x0900
#define PRID_IMP_NETLOGIC_XLR308	0x0600
#define PRID_IMP_NETLOGIC_XLR532C	0x0800
#define PRID_IMP_NETLOGIC_XLR516C	0x0a00
#define PRID_IMP_NETLOGIC_XLR508C	0x0b00
#define PRID_IMP_NETLOGIC_XLR308C	0x0f00
#define PRID_IMP_NETLOGIC_XLS608	0x8000
#define PRID_IMP_NETLOGIC_XLS408	0x8800
#define PRID_IMP_NETLOGIC_XLS404	0x8c00
#define PRID_IMP_NETLOGIC_XLS208	0x8e00
#define PRID_IMP_NETLOGIC_XLS204	0x8f00
#define PRID_IMP_NETLOGIC_XLS108	0xce00
#define PRID_IMP_NETLOGIC_XLS104	0xcf00
#define PRID_IMP_NETLOGIC_XLS616B	0x4000
#define PRID_IMP_NETLOGIC_XLS608B	0x4a00
#define PRID_IMP_NETLOGIC_XLS416B	0x4400
#define PRID_IMP_NETLOGIC_XLS412B	0x4c00
#define PRID_IMP_NETLOGIC_XLS408B	0x4e00
#define PRID_IMP_NETLOGIC_XLS404B	0x4f00
#define PRID_IMP_NETLOGIC_AU13XX	0x8000

#define PRID_IMP_NETLOGIC_XLP8XX	0x1000
#define PRID_IMP_NETLOGIC_XLP3XX	0x1100
#define PRID_IMP_NETLOGIC_XLP2XX	0x1200
#define PRID_IMP_NETLOGIC_XLP9XX	0x1500
#define PRID_IMP_NETLOGIC_XLP5XX	0x1300

/*
 * Particular Revision values for bits 7:0 of the PRId register.
 */

#define PRID_REV_MASK		0x00ff

/*
 * Definitions for 7:0 on legacy processors
 */

#define PRID_REV_TX4927		0x0022
#define PRID_REV_TX4937		0x0030
#define PRID_REV_R4400		0x0040
#define PRID_REV_R3000A		0x0030
#define PRID_REV_R3000		0x0020
#define PRID_REV_R2000A		0x0010
#define PRID_REV_TX3912		0x0010
#define PRID_REV_TX3922		0x0030
#define PRID_REV_TX3927		0x0040
#define PRID_REV_VR4111		0x0050
#define PRID_REV_VR4181		0x0050	/* Same as VR4111 */
#define PRID_REV_VR4121		0x0060
#define PRID_REV_VR4122		0x0070
#define PRID_REV_VR4181A	0x0070	/* Same as VR4122 */
#define PRID_REV_VR4130		0x0080
#define PRID_REV_34K_V1_0_2	0x0022
#define PRID_REV_LOONGSON1B	0x0020
#define PRID_REV_LOONGSON2E	0x0002
#define PRID_REV_LOONGSON2F	0x0003
#define PRID_REV_LOONGSON3A	0x0005
#define PRID_REV_LOONGSON3B_R1	0x0006
#define PRID_REV_LOONGSON3B_R2	0x0007


#endif /* _ASM_CPU_H */
