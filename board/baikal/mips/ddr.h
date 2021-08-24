/*
 * Low-level and POST tests.
 *
 * Copyright (C) 2014 Baikal Electronics.
 *
 * Author: Alexey Malahov <Alexey.Malahov@baikalelectronics.com>
 *
 * Description: DDR registers.
 */

#ifndef __DDR_H__
#define __DDR_H__

#define DDRC_MSTR		(0x0)	/* Master Register */
#define DDRC_STAT		(0x4)	/* Operating Mode Status Register */
#define DDRC_MRCTRL0		(0x10)	/* Mode Register Read/Write Control Register 0. */
#define DDRC_MRCTRL1		(0x14)	/* Mode Register Read/Write Control Register 1 */
#define DDRC_MRSTAT		(0x18)	/* Mode Register Read/Write Status Register */
#define DDRC_MRCTRL2		(0x1c)	/* Mode Register Read/Write Control Register 2 */
#define DDRC_DERATEEN		(0x20)	/* Temperature Derate Enable Register */
#define DDRC_DERATEINT		(0x24)	/* Temperature Derate Interval Register */
#define DDRC_PWRCTL		(0x30)	/* Low Power Control Register */
#define DDRC_PWRTMG		(0x34)	/* Low Power Timing Register */
#define DDRC_HWLPCTL		(0x38)	/* Hardware Low Power Control Register */
#define DDRC_RFSHCTL0		(0x50)	/* Refresh Control Register 0 */
#define DDRC_RFSHCTL1		(0x54)	/* Refresh Control Register 1 */
#define DDRC_RFSHCTL2		(0x58)	/* Refresh Control Register 2 */
#define DDRC_RFSHCTL3		(0x60)	/* Refresh Control Register 0 */
#define DDRC_RFSHTMG		(0x64)	/* Refresh Timing Register */
#define DDRC_ECCCFG0		(0x70)	/* ECC Configuration Register */
#define DDRC_ECCCFG1		(0x74)	/* ECC Configuration Register */
#define DDRC_ECCSTAT		(0x78)	/* ECC Status Register */
#define DDRC_ECCCLR		(0x7c)	/* ECC Clear Register */
#define DDRC_ECCERRCNT		(0x80)	/* ECC Error Counter Register */
#define DDRC_ECCCADDR0		(0x84)	/* ECC Corrected Error Address Register 0 */
#define DDRC_ECCCADDR1		(0x88)	/* ECC Corrected Error Address Register 1 */
#define DDRC_ECCCSYN0		(0x8c)	/* ECC Corrected Syndrome Register 0 */
#define DDRC_ECCCSYN1		(0x90)	/* ECC Corrected Syndrome Register 1 */
#define DDRC_ECCCSYN2		(0x94)	/* ECC Corrected Syndrome Register 2 */
#define DDRC_ECCBITMASK0	(0x98)	/* ECC Corrected Data Bit Mask Register 0 */
#define DDRC_ECCBITMASK1	(0x9c)	/* ECC Corrected Data Bit Mask Register 1 */
#define DDRC_ECCBITMASK2	(0xa0)	/* ECC Corrected Data Bit Mask Register 2 */
#define DDRC_ECCUADDR0		(0xa4)	/* ECC Uncorrected Error Address Register 0 */
#define DDRC_ECCUADDR1		(0xa8)	/* ECC Uncorrected Error Address Register 1 */
#define DDRC_ECCUSYN0		(0xac)	/* ECC Uncorrected Syndrome Register 0 */
#define DDRC_ECCUSYN1		(0xb0)	/* ECC Uncorrected Syndrome Register 1 */
#define DDRC_ECCUSYN2		(0xb4)	/* ECC Uncorrected Syndrome Register 2 */
#define DDRC_ECCPOISONADDR0	(0xb8)	/* ECC Data Poisoning Address Register 0 */
#define DDRC_ECCPOISONADDR1	(0xbc)	/* ECC Data Poisoning Address Register 1 */
#define DDRC_CRCPARCTL0		(0xc0)	/* CRC Parity Control Register0 */
#define DDRC_CRCPARCTL1		(0xc4)	/* CRC Parity Control Register1 */
#define DDRC_CRCPARCTL2		(0xc8)	/* CRC Parity Control Register2 */
#define DDRC_CRCPARSTAT		(0xcc)	/* CRC Parity Status Register */
#define DDRC_INIT0		(0xd0)	/* SDRAM Initialization Register 0 */
#define DDRC_INIT1		(0xd4)	/* SDRAM Initialization Register 1 */
#define DDRC_INIT2		(0xd8)	/* SDRAM Initialization Register 2 */
#define DDRC_INIT3		(0xdc)	/* SDRAM Initialization Register 3 */
#define DDRC_INIT4		(0xe0)	/* SDRAM Initialization Register 4 */
#define DDRC_INIT5		(0xe4)	/* SDRAM Initialization Register 5 */
#define DDRC_INIT6		(0xe8)	/* SDRAM Initialization Register 6 */
#define DDRC_INIT7		(0xec)	/* SDRAM Initialization Register 7 */
#define DDRC_DIMMCTL		(0xf0)	/* DIMM Control Register */
#define DDRC_RANKCTL		(0xf4)	/* Rank Control Register */
#define DDRC_DRAMTMG0		(0x100)	/* SDRAM Timing Register 0 */
#define DDRC_DRAMTMG1		(0x104)	/* SDRAM Timing Register 1 */
#define DDRC_DRAMTMG2		(0x108)	/* SDRAM Timing Register 2 */
#define DDRC_DRAMTMG3		(0x10c)	/* SDRAM Timing Register 3 */
#define DDRC_DRAMTMG4		(0x110)	/* SDRAM Timing Register 4 */
#define DDRC_DRAMTMG5		(0x114)	/* SDRAM Timing Register 5 */
#define DDRC_DRAMTMG6		(0x118)	/* SDRAM Timing Register 6 */
#define DDRC_DRAMTMG7		(0x11c)	/* SDRAM Timing Register 7 */
#define DDRC_DRAMTMG8		(0x120)	/* SDRAM Timing Register 8 */
#define DDRC_DRAMTMG9		(0x124)	/* SDRAM Timing Register 9 */
#define DDRC_DRAMTMG10		(0x128)	/* SDRAM Timing Register 10 */
#define DDRC_DRAMTMG11		(0x12c)	/* SDRAM Timing Register 11 */
#define DDRC_DRAMTMG12		(0x130)	/* SDRAM Timing Register 12 */
#define DDRC_ZQCTL0		(0x180)	/* ZQ Control Register 0 */
#define DDRC_ZQCTL1		(0x184)	/* ZQ Control Register 1 */
#define DDRC_ZQCTL2		(0x188)	/* ZQ Control Register 2 */
#define DDRC_ZQSTAT		(0x18c)	/* ZQ Status Register */
#define DDRC_DFITMG0		(0x190)	/* DFI Timing Register 0 */
#define DDRC_DFITMG1		(0x194)	/* DFI Timing Register 1 */
#define DDRC_DFILPCFG0		(0x198)	/* DFI Low Power Configuration Register 0 */
#define DDRC_DFILPCFG1		(0x19c)	/* DFI Low Power Configuration Register 1 */
#define DDRC_DFIUPD0		(0x1a0)	/* DFI Update Register 0 */
#define DDRC_DFIUPD1		(0x1a4)	/* DFI Update Register 1 */
#define DDRC_DFIUPD2		(0x1a8)	/* DFI Update Register 2 */
#define DDRC_DFIUPD3		(0x1ac)	/* DFI Update Register 3 */
#define DDRC_DFIMISC		(0x1b0)	/* DFI Miscellaneous Control Register */
#define DDRC_DFITMG2		(0x1b4)	/* DFI Timing Register 2 */
#define DDRC_DFIUPD4		(0x1b8)	/* DFI Update Register 4 */
#define DDRC_DBICTL		(0x1c0)	/* DM/DBI Control Register */
#define DDRC_TRAINCTL0		(0x1d0)	/* PHY Eval Training Control Register 0 */
#define DDRC_TRAINCTL1		(0x1d4)	/* PHY Eval Training Control Register 1 */
#define DDRC_TRAINCTL2		(0x1d8)	/* PHY Eval Training Control Register 2 */
#define DDRC_TRAINSTAT		(0x1dc)	/* PHY Eval Training Status Register */
#define DDRC_ADDRMAP0		(0x200)	/* Address Map Register 0 */
#define DDRC_ADDRMAP1		(0x204)	/* Address Map Register 1 */
#define DDRC_ADDRMAP2		(0x208)	/* Address Map Register 2 */
#define DDRC_ADDRMAP3		(0x20c)	/* Address Map Register 3 */
#define DDRC_ADDRMAP4		(0x210)	/* Address Map Register 4 */
#define DDRC_ADDRMAP5		(0x214)	/* Address Map Register 5 */
#define DDRC_ADDRMAP6		(0x218)	/* Address Map Register 6 */
#define DDRC_ADDRMAP7		(0x21c)	/* Address Map Register 7 */
#define DDRC_ADDRMAP8		(0x220)	/* Address Map Register 8 */
#define DDRC_ODTCFG		(0x240)	/* ODT Configuration Register */
#define DDRC_ODTMAP		(0x244)	/* ODT/Rank Map Register */
#define DDRC_SCHED		(0x250)	/* Scheduler Control Register */
#define DDRC_SCHED1		(0x254)	/* Scheduler Control Register 1 */
#define DDRC_PERFHPR1		(0x25c)	/* High Priority Read CAM Register 1 */
#define DDRC_PERFLPR1		(0x264)	/* Low Priority Read CAM Register 1 */
#define DDRC_PERFWR1		(0x26c)	/* Write CAM Register 1 */
#define DDRC_PERFVPR1		(0x274)	/* Variable Priority Read CAM Register 1 */
#define DDRC_PERFVPW1		(0x278)	/* Variable Priority Write CAM Register 1 */
#define DDRC_DQMAP0		(0x280)	/* DQ Map Register 0 */
#define DDRC_DQMAP1		(0x284)	/* DQ Map Register 1 */
#define DDRC_DQMAP2		(0x288)	/* DQ Map Register 2 */
#define DDRC_DQMAP3		(0x28c)	/* DQ Map Register 3 */
#define DDRC_DQMAP4		(0x290)	/* DQ Map Register 4 */
#define DDRC_DQMAP5		(0x294)	/* DQ Map Register 5 */
#define DDRC_DBG0		(0x300)	/* Debug Register 0 */
#define DDRC_DBG1		(0x304)	/* Debug Register 1 */
#define DDRC_DBGCAM		(0x308)	/* CAM Debug Register */
#define DDRC_DBGCMD		(0x30c)	/* Command Debug Register */
#define DDRC_DBGSTAT		(0x310)	/* Status Debug Register */
#define DDRC_SWCTL		(0x320)	/* Software Register Programming Control Enable */
#define DDRC_SWSTAT		(0x324)	/* Software Register Programming Control Status */
#define DDRC_OCPARCFG0		(0x330)	/* On-Chip Parity Configuration Register 0 */
#define DDRC_OCPARCFG1		(0x334)	/* On-Chip Parity Configuration Register 1 */
#define DDRC_OCPARCFG2		(0x338)	/* On-Chip Parity Configuration Register 2 */
#define DDRC_OCPARCFG3		(0x33c)	/* On-Chip Parity Configuration Register 3 */
#define DDRC_OCPARSTAT0		(0x340)	/* On-Chip Parity Status Register 0 */
#define DDRC_OCPARSTAT1		(0x344)	/* On-Chip Parity Status Register 1 */
#define DDRC_OCPARWLOG0		(0x348)	/* On-Chip Parity Write Data Log Register 0 */
#define DDRC_OCPARWLOG1		(0x34c)	/* On-Chip Parity Write Data Log Register 1 */
#define DDRC_OCPARWLOG2		(0x350)	/* On-Chip Parity Write Data Log Register 2 */
#define DDRC_OCPARAWLOG0	(0x354)	/* On-Chip Parity Write Address Log 0 Register */
#define DDRC_OCPARAWLOG1	(0x358)	/* On-Chip Parity Write Address Log 1 Register */
#define DDRC_OCPARRLOG0		(0x35c)	/* On-Chip Parity Read Data Log 0 Register */
#define DDRC_OCPARRLOG1		(0x360)	/* On-Chip Parity Read Data Log 1 Register */
#define DDRC_OCPARARLOG0	(0x364)	/* On-Chip Parity Read Address Log 0 Register */
#define DDRC_OCPARARLOG1	(0x368)	/* On-Chip Parity Read Address Log 1 Register */
#define DDRC_PSTAT		(0x3fc)	/* Port Status Register */
#define DDRC_PCCFG		(0x400)	/* Port Common Configuration Register */
#define DDRC_PCFGR(n)		(0x404 + n * 0xb0)	/* Port n Configuration Read Register (for n = 0; n <= 15) */
#define DDRC_PCFGW(n)		(0x408 + n * 0xb0)	/* Port n Configuration Write Register (for n = 0; n <= 15) */
#define DDRC_PCFGC(n)		(0x40c + n * 0xb0)	/* Port n Common Configuration Register (for n = 0; n <= 15) */
#define DDRC_PCFGIDMASKCH(m, n)	(0x410 + m * 0x08 + n * 0xb0)	/* Port n Channel m Configuration ID Mask Register (for m,n = 0; m,n <= 15) */
#define DDRC_PCFGIDVALUECH(m, n) (0x414 + m * 0x08 + n * 0xb0) 	/* Port n Channel m Configuration ID Value Register (for m,n = 0; m,n <= 15) */
#define DDRC_PCTRL(n)		(0x490 + n * 0xb0)	/* Port n Control Register (for n = 0; n <= 15) */
#define DDRC_PCFGQOS0(n)	(0x494 + n * 0xb0)	/* Port n Read QoS Configuration Register 0 (for n = 0; n <= 15) */
#define DDRC_PCFGQOS1(n)	(0x498 + n * 0xb0)	/* Port n Read QoS Configuration Register 1 (for n = 0; n <= 15) */
#define DDRC_PCFGWQOS0(n)	(0x49c + n * 0xb0)	/* Port n Write QoS Configuration Register 0 (for n = 0; n <= 15) */
#define DDRC_PCFGWQOS1(n) 	(0x4a0 + n * 0xb0)	/* Port n Write QoS Configuration Register 1 (for n = 0; n <= 15) */
#define DDRC_SARBASE(n)		(0xf04 + n * 0x08)	/* SAR Base Address Register n, (for n = 0; n <= 3) */
#define DDRC_SARSIZE(n)		(0xf08 + n * 0x08)	/* SAR Size Register n, (for n = 0; n <= 3) */
#define DDRC_SBRCTL		(0xf24)	/* Scrubber Control Register */
#define DDRC_SBRSTAT		(0xf28)	/* Scrubber Status Register */
#define DDRC_SBRWDATA0		(0xf2c)	/* Scrubber Write Data Pattern0 */
#define DDRC_SBRWDATA1		(0xf30)	/* Scrubber Write Data Pattern1 */

/* Operating Mode Status Register Bits. */
#define DDRC_STAT_MODE_MASK	0x7
#define DDRC_STAT_MODE_POS	0
#define DDRC_STAT_MODE_BITS	3
#define DDRC_STAT_MODE_INIT	0x00	/* Init Mode. */
#define DDRC_STAT_MODE_NORMAL	0x01	/* Normal Mode. */
#define DDRC_STAT_MODE_PW_DOWN	0x10	/* Power-down Mode. */
#define DDRC_STAT_MODE_SELF_RF	0x11	/* Self-refresh Mode. */

/* SCRUBBER */
#define SCRUB_EN		(1 << 0) /* Enable ECC scrubber. */
#define SCRUB_EN_SHIFT		(0)
#define SCRUB_LP		(1 << 1) /* Continue scrubbing during low power. */
#define SCRUB_MODE_WR	(1 << 2) /* 0: ECC scrubber performs reads. */
#define SCRUB_MODE_SHIFT	(2)
#define SCRUB_BURST_SHIFT	(4) /* Scrub burst count. */
#define SCRUB_INTVAL_SHIFT	(8) /* Scrub interval. */
#define SCRUB_INTVAL_LEN	(16)
#define SCRUB_INTVAL_DEF	(0xFF)
#define SCRUB_PATTERN0		(0xC001CAFE)
#define SCRUB_PATTERN1		(0xC001BABE)

/* SCRUBBER status */
#define SCRUB_BUSY		(1 << 0) /* Scrubber busy */
#define SCRUB_DONE		(1 << 1) /* Scrubber done */

/* DDR4 multiPHY Utility Block (PUB) registers. */
#define	DDR_PUB_RIDR		(0x0) /* R Revision Identification Register, register number = 0 */
#define	DDR_PUB_PIR		(0x4) /* R/W PHY Initialization Register, register number = 1 */
#define	DDR_PUB_PGCR0		(0x10) /* R/W PHY General Configuration Register 0-7, register number = 4 */
#define	DDR_PUB_PGCR1		(0x14) /* R/W PHY General Configuration Register 0-7, register number = 5 */
#define	DDR_PUB_PGCR2		(0x18) /* R/W PHY General Configuration Register 0-7, register number = 6 */
#define	DDR_PUB_PGCR3		(0x1c) /* R/W PHY General Configuration Register 0-7, register number = 7 */
#define	DDR_PUB_PGCR4		(0x20) /* R/W PHY General Configuration Register 0-7, register number = 8 */
#define	DDR_PUB_PGCR5		(0x24) /* R/W PHY General Configuration Register 0-7, register number = 9 */
#define	DDR_PUB_PGCR6		(0x28) /* R/W PHY General Configuration Register 0-7, register number = 10 */
#define	DDR_PUB_PGCR7		(0x2c) /* R/W PHY General Configuration Register 0-7, register number = 11 */
#define	DDR_PUB_PGSR0		(0x30) /* R PHY General Status Register 0-1, register number = 12 */
#define	DDR_PUB_PGSR1		(0x34) /* R PHY General Status Register 0-1, register number = 13 */
#define	DDR_PUB_PTR0		(0x40) /* R/W PHY Timing Register 0-4, register number = 16 */
#define	DDR_PUB_PTR1		(0x44) /* R/W PHY Timing Register 0-4, register number = 17 */
#define	DDR_PUB_PTR2		(0x48) /* R/W PHY Timing Register 0-4, register number = 18 */
#define	DDR_PUB_PTR3		(0x4c) /* R/W PHY Timing Register 0-4, register number = 19 */
#define	DDR_PUB_PTR4		(0x50) /* R/W PHY Timing Register 0-4, register number = 20 */
#define	DDR_PUB_PLLCR0		(0x68) /* R/W PLL Control Register 0-5 (Type B PLL only), register number = 26 */
#define	DDR_PUB_PLLCR1		(0x6c) /* R/W PLL Control Register 0-5 (Type B PLL only), register number = 27 */
#define	DDR_PUB_PLLCR2		(0x70) /* R/W PLL Control Register 0-5 (Type B PLL only), register number = 28 */
#define	DDR_PUB_PLLCR3		(0x74) /* R/W PLL Control Register 0-5 (Type B PLL only), register number = 29 */
#define	DDR_PUB_PLLCR4		(0x78) /* R/W PLL Control Register 0-5 (Type B PLL only), register number = 30 */
#define	DDR_PUB_PLLCR5		(0x7c) /* R/W PLL Control Register 0-5 (Type B PLL only), register number = 31 */
#define	DDR_PUB_PLLCR		(0x80) /* R/W PLL Control Register (Type A PLL only), register number = 32 */
#define	DDR_PUB_DXCCR		(0x88) /* R/W DATX8 Common Configuration Register, register number = 34 */
#define	DDR_PUB_DSGCR		(0x90) /* R/W DDR System General Configuration Register, register number = 36 */
#define	DDR_PUB_ODTCR		(0x98) /* R/W ODT Configuration Register, register number = 38 */
#define	DDR_PUB_AACR		(0xa0) /* R/W Anti-Aging Control Register, register number = 40 */
#define	DDR_PUB_GPR0		(0xc0) /* R/W General Purpose Register 0-1, register number = 48 */
#define	DDR_PUB_GPR1		(0xc4) /* R/W General Purpose Register 0-1, register number = 49 */
#define	DDR_PUB_DCR		(0x100) /* R/W DRAM Configuration Register, register number = 64 */
#define	DDR_PUB_DTPR0		(0x110) /* R/W DRAM Timing Parameters Register 0-5, register number = 68 */
#define	DDR_PUB_DTPR1		(0x114) /* R/W DRAM Timing Parameters Register 0-5, register number = 69 */
#define	DDR_PUB_DTPR2		(0x118) /* R/W DRAM Timing Parameters Register 0-5, register number = 70 */
#define	DDR_PUB_DTPR3		(0x11c) /* R/W DRAM Timing Parameters Register 0-5, register number = 71 */
#define	DDR_PUB_DTPR4		(0x120) /* R/W DRAM Timing Parameters Register 0-5, register number = 72 */
#define	DDR_PUB_DTPR5		(0x124) /* R/W DRAM Timing Parameters Register 0-5, register number = 73 */
#define	DDR_PUB_DTPR6		(0x128) /* R/W DRAM Timing Parameters Register 0-5, register number = 74 */
#define	DDR_PUB_RDIMMGCR0	(0x140) /* R/W RDIMM General Configuration Register 0-2, register number = 80 */
#define	DDR_PUB_RDIMMGCR1	(0x144) /* R/W RDIMM General Configuration Register 0-2, register number = 81 */
#define	DDR_PUB_RDIMMGCR2	(0x148) /* R/W RDIMM General Configuration Register 0-2, register number = 82 */
#define	DDR_PUB_RDIMMCR0	(0x150) /* R/W RDIMM Control Register 0-4, register number = 84 */
#define	DDR_PUB_RDIMMCR1	(0x154) /* R/W RDIMM Control Register 0-4, register number = 85 */
#define	DDR_PUB_RDIMMCR2	(0x158) /* R/W RDIMM Control Register 0-4, register number = 86 */
#define	DDR_PUB_RDIMMCR3	(0x15c) /* R/W RDIMM Control Register 0-4, register number = 87 */
#define	DDR_PUB_RDIMMCR4	(0x160) /* R/W RDIMM Control Register 0-4, register number = 88 */
#define	DDR_PUB_SCHCR0		(0x168) /* RW Scheduler Command Register 0-1, register number = 90 */
#define	DDR_PUB_SCHCR1		(0x16c) /* RW Scheduler Command Register 0-1, register number = 91 */
#define	DDR_PUB_MR0		(0x180) /* R/W Mode Register 0, register number = 96 */
#define	DDR_PUB_MR1		(0x184) /* R/W Mode Register 1, register number = 97 */
#define	DDR_PUB_MR2		(0x188) /* R/W Mode Register 2, register number = 98 */
#define	DDR_PUB_MR3		(0x18c) /* R/W Mode Register 3, register number = 99 */
#define	DDR_PUB_MR4		(0x168) /* RW Mode Register 4, register number = 90 */
#define	DDR_PUB_MR5		(0x194) /* RW Mode Register 5, register number = 101 */
#define	DDR_PUB_MR6		(0x198) /* RW Mode Register 6, register number = 102 */
#define	DDR_PUB_MR7		(0x19c) /* RW Mode Register 7, register number = 103 */
#define	DDR_PUB_MR11		(0x1ac) /* RW Mode Register 11, register number = 107 */
#define	DDR_PUB_DTCR0		(0x200) /* R/W Data Training Configuration Register 0-1, register number = 128 */
#define	DDR_PUB_DTCR1		(0x204) /* R/W Data Training Configuration Register 0-1, register number = 129 */
#define	DDR_PUB_DTAR0		(0x208) /* R/W Data Training Address Register 0-2, register number = 130 */
#define	DDR_PUB_DTAR1		(0x20c) /* R/W Data Training Address Register 0-2, register number = 131 */
#define	DDR_PUB_DTAR2		(0x210) /* R/W Data Training Address Register 0-2, register number = 132 */
#define	DDR_PUB_DTDR0		(0x218) /* R/W Data Training Data Register 0-1, register number = 134 */
#define	DDR_PUB_DTDR1		(0x21c) /* R/W Data Training Data Register 0-1, register number = 135 */
#define	DDR_PUB_DTEDR0		(0x230) /* R Data Training Eye Data Register 0-2, register number = 140 */
#define	DDR_PUB_DTEDR1		(0x234) /* R Data Training Eye Data Register 0-2, register number = 141 */
#define	DDR_PUB_DTEDR2		(0x238) /* R Data Training Eye Data Register 0-2, register number = 142 */
#define	DDR_PUB_VTDR		(0x23c) /* R VREF Training Data Register, register number = 143 */
#define	DDR_PUB_CATR0		(0x240) /* R/W CA Training Register 0-1, register number = 144 */
#define	DDR_PUB_CATR1		(0x244) /* R/W CA Training Register 0-1, register number = 145 */
#define	DDR_PUB_DQSDR0		(0x250) /* R/W DQS Drift Register 0-2, register number = 148 */
#define	DDR_PUB_DQSDR1		(0x254) /* R/W DQS Drift Register 0-2, register number = 149 */
#define	DDR_PUB_DQSDR2		(0x258) /* R/W DQS Drift Register 0-2, register number = 150 */
#define	DDR_PUB_DCUAR		(0x304) /* R/W DCU Address Register, register number = 193 */
#define	DDR_PUB_DCUDR		(0x308) /* R/W DCU Data Register, register number = 194 */
#define	DDR_PUB_DCURR		(0x30c) /* R/W DCU Run Register, register number = 195 */
#define	DDR_PUB_DCULR		(0x310) /* R/W DCU Loop Register, register number = 196 */
#define	DDR_PUB_DCUGCR		(0x314) /* R/W DCU General Configuration Register, register number = 197 */
#define	DDR_PUB_DCUTPR		(0x318) /* R/W DCU Timing Parameters Register, register number = 198 */
#define	DDR_PUB_DCUSR0		(0x31c) /* R DCU Status Register 0-1, register number = 199 */
#define	DDR_PUB_DCUSR1		(0x320) /* R DCU Status Register 0-1, register number = 200 */
#define	DDR_PUB_BISTRR		(0x400) /* R/W BIST Run Register, register number = 256 */
#define	DDR_PUB_BISTWCR		(0x404) /* R/W BIST Word Count Register, register number = 257 */
#define	DDR_PUB_BISTMSKR0	(0x408) /* R/W BIST Mask Register 0-2, register number = 258 */
#define	DDR_PUB_BISTMSKR1	(0x40c) /* R/W BIST Mask Register 0-2, register number = 259 */
#define	DDR_PUB_BISTMSKR2	(0x410) /* R/W BIST Mask Register 0-2, register number = 260 */
#define	DDR_PUB_BISTLSR		(0x414) /* R/W BIST LFSR Seed Register, register number = 261 */
#define	DDR_PUB_BISTAR0		(0x418) /* R/W BIST Address Register 0-4, register number = 262 */
#define	DDR_PUB_BISTAR1		(0x41c) /* R/W BIST Address Register 0-4, register number = 263 */
#define	DDR_PUB_BISTAR2		(0x420) /* R/W BIST Address Register 0-4, register number = 264 */
#define	DDR_PUB_BISTAR3		(0x424) /* R/W BIST Address Register 0-4, register number = 265 */
#define	DDR_PUB_BISTAR4		(0x428) /* R/W BIST Address Register 0-4, register number = 266 */
#define	DDR_PUB_BISTUDPR	(0x42c) /* R/W BIST User Data Pattern Register, register number = 267 */
#define	DDR_PUB_BISTGSR		(0x430) /* R BIST General Status Register, register number = 268 */
#define	DDR_PUB_BISTWER0	(0x434) /* R BIST Word Error Register 0-1, register number = 269 */
#define	DDR_PUB_BISTWER1	(0x438) /* R BIST Word Error Register 0-1, register number = 270 */
#define	DDR_PUB_BISTBER0	(0x43c) /* R BIST Bit Error Register 0-4, register number = 271 */
#define	DDR_PUB_BISTBER1	(0x440) /* R BIST Bit Error Register 0-4, register number = 272 */
#define	DDR_PUB_BISTBER2	(0x444) /* R BIST Bit Error Register 0-4, register number = 273 */
#define	DDR_PUB_BISTBER3	(0x448) /* R BIST Bit Error Register 0-4, register number = 274 */
#define	DDR_PUB_BISTBER4	(0x44c) /* R BIST Bit Error Register 0-4, register number = 275 */
#define	DDR_PUB_BISTWCSR	(0x450) /* R BIST Word Count Status Register, register number = 276 */
#define	DDR_PUB_BISTFWR0	(0x454) /* R BIST Fail Word Register 0-2, register number = 277 */
#define	DDR_PUB_BISTFWR1	(0x458) /* R BIST Fail Word Register 0-2, register number = 278 */
#define	DDR_PUB_BISTFWR2	(0x45c) /* R BIST Fail Word Register 0-2, register number = 279 */
#define	DDR_PUB_BISTBER5	(0x460) /* R BIST Bit Error Register 5, register number = 280 */
#define	DDR_PUB_RANKIDR		(0x4dc) /* R/W Rank ID Register, register number = 311 */
#define	DDR_PUB_RIOCR0		(0x4e0) /* R/W Rank I/O Configuration Registers 0-5, register number = 312 */
#define	DDR_PUB_RIOCR1		(0x4e4) /* R/W Rank I/O Configuration Registers 0-5, register number = 313 */
#define	DDR_PUB_RIOCR2		(0x4e8) /* R/W Rank I/O Configuration Registers 0-5, register number = 314 */
#define	DDR_PUB_RIOCR3		(0x4ec) /* R/W Rank I/O Configuration Registers 0-5, register number = 315 */
#define	DDR_PUB_RIOCR4		(0x4f0) /* R/W Rank I/O Configuration Registers 0-5, register number = 316 */
#define	DDR_PUB_RIOCR5		(0x4f4) /* R/W Rank I/O Configuration Registers 0-5, register number = 317 */
#define	DDR_PUB_ACIOCR0		(0x500) /* R/W AC I/O Configuration Register 0-5, register number = 320 */
#define	DDR_PUB_ACIOCR1		(0x504) /* R/W AC I/O Configuration Register 0-5, register number = 321 */
#define	DDR_PUB_ACIOCR2		(0x508) /* R/W AC I/O Configuration Register 0-5, register number = 322 */
#define	DDR_PUB_ACIOCR3		(0x50c) /* R/W AC I/O Configuration Register 0-5, register number = 323 */
#define	DDR_PUB_ACIOCR4		(0x510) /* R/W AC I/O Configuration Register 0-5, register number = 324 */
#define	DDR_PUB_ACIOCR5		(0x514) /* R/W AC I/O Configuration Register 0-5, register number = 325 */
#define	DDR_PUB_IOVCR0		(0x520) /* R/W IO VREF Control Register 0-1, register number = 328 */
#define	DDR_PUB_IOVCR1		(0x524) /* R/W IO VREF Control Register 0-1, register number = 329 */
#define	DDR_PUB_VTCR0		(0x528) /* R/W VREF Training Control Register 0-1, register number = 330 */
#define	DDR_PUB_VTCR1		(0x52c) /* R/W VREF Training Control Register 0-1, register number = 331 */
#define	DDR_PUB_346		(0x540) /* 0x150 - 0x15A ACBDLR0-10 R/W AC Bit Delay Line Register 0-10, register number = 336 */
#define	DDR_PUB_ACLCDLR		(0x580) /* R/W AC Local Calibrated Delay Line Register, register number = 352 */
#define	DDR_PUB_ACMDLR0		(0x5a0) /* R/W AC Master Delay Line Register 0-1, register number = 360 */
#define	DDR_PUB_ACMDLR1		(0x5a4) /* R/W AC Master Delay Line Register 0-1, register number = 361 */
#define	DDR_PUB_ZQCR		(0x680) /* R/W ZQ Impedance Control Register, register number = 416 */
#define	DDR_PUB_ZQ0PR		(0x684) /* R/W ZQ 0 Impedance Control Program Register, register number = 417 */
#define	DDR_PUB_ZQ0DR		(0x688) /* R/W ZQ 0 Impedance Control Data Register, register number = 418 */
#define	DDR_PUB_ZQ0SR		(0x68c) /* R ZQ 0 Impedance Control Status Register, register number = 419 */
#define	DDR_PUB_ZQ1PR		(0x694) /* R/W ZQ 1 Impedance Control Program Register, register number = 421 */
#define	DDR_PUB_ZQ1DR		(0x698) /* R/W ZQ 1 Impedance Control Data Register, register number = 422 */
#define	DDR_PUB_ZQ1SR		(0x69c) /* R ZQ 1 Impedance Control Status Register, register number = 423 */
#define	DDR_PUB_ZQ2PR		(0x6a4) /* R/W ZQ 2 Impedance Control Program Register, register number = 425 */
#define	DDR_PUB_ZQ2DR		(0x6a8) /* R/W ZQ 2 Impedance Control Data Register, register number = 426 */
#define	DDR_PUB_ZQ2SR		(0x6ac) /* R ZQ 2 Impedance Control Status Register, register number = 427 */
#define	DDR_PUB_ZQ3PR		(0x6b4) /* R/W ZQ 3 Impedance Control Program Register, register number = 429 */
#define	DDR_PUB_ZQ3DR		(0x6b8) /* R/W ZQ 3 Impedance Control Data Register, register number = 430 */
#define	DDR_PUB_ZQ3SR		(0x6bc) /* R ZQ 3 Impedance Control Status Register, register number = 431 */
#define	DDR_PUB_DX0GCR0		(0x700) /* R/W DATX8 0 General Configuration Register 0-9, register number = 448 */
#define	DDR_PUB_DX0GCR1		(0x704) /* R/W DATX8 0 General Configuration Register 0-9, register number = 449 */
#define	DDR_PUB_DX0GCR2		(0x708) /* R/W DATX8 0 General Configuration Register 0-9, register number = 450 */
#define	DDR_PUB_DX0GCR3		(0x70c) /* R/W DATX8 0 General Configuration Register 0-9, register number = 451 */
#define	DDR_PUB_DX0GCR4		(0x710) /* R/W DATX8 0 General Configuration Register 0-9, register number = 452 */
#define	DDR_PUB_DX0GCR5		(0x714) /* R/W DATX8 0 General Configuration Register 0-9, register number = 453 */
#define	DDR_PUB_DX0GCR6		(0x718) /* R/W DATX8 0 General Configuration Register 0-9, register number = 454 */
#define	DDR_PUB_DX0GCR7		(0x71c) /* R/W DATX8 0 General Configuration Register 0-9, register number = 455 */
#define	DDR_PUB_DX0GCR8		(0x720) /* R/W DATX8 0 General Configuration Register 0-9, register number = 456 */
#define	DDR_PUB_DX0GCR9		(0x724) /* R/W DATX8 0 General Configuration Register 0-9, register number = 457 */
#define	DDR_PUB_DX0BDLR0	(0x740) /* R/W DATX8 0 Bit Delay Line Register 0-2, register number = 464 */
#define	DDR_PUB_DX0BDLR1	(0x744) /* R/W DATX8 0 Bit Delay Line Register 0-2, register number = 465 */
#define	DDR_PUB_DX0BDLR2	(0x748) /* R/W DATX8 0 Bit Delay Line Register 0-2, register number = 466 */
#define	DDR_PUB_DX0BDLR3	(0x750) /* R/W DATX8 0 Bit Delay Line Register 3-5, register number = 468 */
#define	DDR_PUB_DX0BDLR4	(0x754) /* R/W DATX8 0 Bit Delay Line Register 3-5, register number = 469 */
#define	DDR_PUB_DX0BDLR5	(0x758) /* R/W DATX8 0 Bit Delay Line Register 3-5, register number = 470 */
#define	DDR_PUB_DX0BDLR6	(0x760) /* R/W DATX8 0 Bit Delay Line Register 6-9, register number = 472 */
#define	DDR_PUB_DX0BDLR7	(0x764) /* R/W DATX8 0 Bit Delay Line Register 6-9, register number = 473 */
#define	DDR_PUB_DX0BDLR8	(0x768) /* R/W DATX8 0 Bit Delay Line Register 6-9, register number = 474 */
#define	DDR_PUB_DX0BDLR9	(0x76c) /* R/W DATX8 0 Bit Delay Line Register 6-9, register number = 475 */
#define	DDR_PUB_DX0LCDLR0	(0x780) /* R/W DATX8 0 Local Calibrated Delay Line Register 0-5, register number = 480 */
#define	DDR_PUB_DX0LCDLR1	(0x784) /* R/W DATX8 0 Local Calibrated Delay Line Register 0-5, register number = 481 */
#define	DDR_PUB_DX0LCDLR2	(0x788) /* R/W DATX8 0 Local Calibrated Delay Line Register 0-5, register number = 482 */
#define	DDR_PUB_DX0LCDLR3	(0x78c) /* R/W DATX8 0 Local Calibrated Delay Line Register 0-5, register number = 483 */
#define	DDR_PUB_DX0LCDLR4	(0x790) /* R/W DATX8 0 Local Calibrated Delay Line Register 0-5, register number = 484 */
#define	DDR_PUB_DX0LCDLR5	(0x794) /* R/W DATX8 0 Local Calibrated Delay Line Register 0-5, register number = 485 */
#define	DDR_PUB_DX0MDLR0	(0x7a0) /* R/W DATX8 0 Master Delay Line Register 0-1, register number = 488 */
#define	DDR_PUB_DX0MDLR1	(0x7a4) /* R/W DATX8 0 Master Delay Line Register 0-1, register number = 489 */
#define	DDR_PUB_DX0GTR0		(0x7c0) /* R/W DATX8 0 General Timing Register 0, register number = 496 */
#define	DDR_PUB_DX0RSR0		(0x7d0) /* R/W DATX8 0 Rank Status Register 0-3, register number = 500 */
#define	DDR_PUB_DX0RSR1		(0x7d4) /* R/W DATX8 0 Rank Status Register 0-3, register number = 501 */
#define	DDR_PUB_DX0RSR2		(0x7d8) /* R/W DATX8 0 Rank Status Register 0-3, register number = 502 */
#define	DDR_PUB_DX0RSR3		(0x7dc) /* R/W DATX8 0 Rank Status Register 0-3, register number = 503 */
#define	DDR_PUB_DX0GSR0		(0x7e0) /* R DATX8 0 General Status Register 0-6, register number = 504 */
#define	DDR_PUB_DX0GSR1		(0x7e4) /* R DATX8 0 General Status Register 0-6, register number = 505 */
#define	DDR_PUB_DX0GSR2		(0x7e8) /* R DATX8 0 General Status Register 0-6, register number = 506 */
#define	DDR_PUB_DX0GSR3		(0x7ec) /* R DATX8 0 General Status Register 0-6, register number = 507 */
#define	DDR_PUB_DX0GSR4		(0x7f0) /* R DATX8 0 General Status Register 0-6, register number = 508 */
#define	DDR_PUB_DX0GSR5		(0x7f4) /* R DATX8 0 General Status Register 0-6, register number = 509 */
#define	DDR_PUB_DX0GSR6		(0x7f8) /* R DATX8 0 General Status Register 0-6, register number = 510 */
#define	DDR_PUB_DX1GCR0		(0x800) /* R/W DATX8 1 General Configuration Register 0-9, register number = 512 */
#define	DDR_PUB_DX1GCR1		(0x804) /* R/W DATX8 1 General Configuration Register 0-9, register number = 513 */
#define	DDR_PUB_DX1GCR2		(0x808) /* R/W DATX8 1 General Configuration Register 0-9, register number = 514 */
#define	DDR_PUB_DX1GCR3		(0x80c) /* R/W DATX8 1 General Configuration Register 0-9, register number = 515 */
#define	DDR_PUB_DX1GCR4		(0x810) /* R/W DATX8 1 General Configuration Register 0-9, register number = 516 */
#define	DDR_PUB_DX1GCR5		(0x814) /* R/W DATX8 1 General Configuration Register 0-9, register number = 517 */
#define	DDR_PUB_DX1GCR6		(0x818) /* R/W DATX8 1 General Configuration Register 0-9, register number = 518 */
#define	DDR_PUB_DX1GCR7		(0x81c) /* R/W DATX8 1 General Configuration Register 0-9, register number = 519 */
#define	DDR_PUB_DX1GCR8		(0x820) /* R/W DATX8 1 General Configuration Register 0-9, register number = 520 */
#define	DDR_PUB_DX1GCR9		(0x824) /* R/W DATX8 1 General Configuration Register 0-9, register number = 521 */
#define	DDR_PUB_DX1BDLR0	(0x840) /* R/W DATX8 1 Bit Delay Line Register 0-2, register number = 528 */
#define	DDR_PUB_DX1BDLR1	(0x844) /* R/W DATX8 1 Bit Delay Line Register 0-2, register number = 529 */
#define	DDR_PUB_DX1BDLR2	(0x848) /* R/W DATX8 1 Bit Delay Line Register 0-2, register number = 530 */
#define	DDR_PUB_DX1BDLR3	(0x850) /* R/W DATX8 1 Bit Delay Line Register 3-5, register number = 532 */
#define	DDR_PUB_DX1BDLR4	(0x854) /* R/W DATX8 1 Bit Delay Line Register 3-5, register number = 533 */
#define	DDR_PUB_DX1BDLR5	(0x858) /* R/W DATX8 1 Bit Delay Line Register 3-5, register number = 534 */
#define	DDR_PUB_DX1BDLR6	(0x860) /* R/W DATX8 1 Bit Delay Line Register 6-9, register number = 536 */
#define	DDR_PUB_DX1BDLR7	(0x864) /* R/W DATX8 1 Bit Delay Line Register 6-9, register number = 537 */
#define	DDR_PUB_DX1BDLR8	(0x868) /* R/W DATX8 1 Bit Delay Line Register 6-9, register number = 538 */
#define	DDR_PUB_DX1BDLR9	(0x86c) /* R/W DATX8 1 Bit Delay Line Register 6-9, register number = 539 */
#define	DDR_PUB_DX1LCDLR0	(0x880) /* R/W DATX8 1 Local Calibrated Delay Line Register 0-5, register number = 544 */
#define	DDR_PUB_DX1LCDLR1	(0x884) /* R/W DATX8 1 Local Calibrated Delay Line Register 0-5, register number = 545 */
#define	DDR_PUB_DX1LCDLR2	(0x888) /* R/W DATX8 1 Local Calibrated Delay Line Register 0-5, register number = 546 */
#define	DDR_PUB_DX1LCDLR3	(0x88c) /* R/W DATX8 1 Local Calibrated Delay Line Register 0-5, register number = 547 */
#define	DDR_PUB_DX1LCDLR4	(0x890) /* R/W DATX8 1 Local Calibrated Delay Line Register 0-5, register number = 548 */
#define	DDR_PUB_DX1LCDLR5	(0x894) /* R/W DATX8 1 Local Calibrated Delay Line Register 0-5, register number = 549 */
#define	DDR_PUB_DX1MDLR0	(0x8a0) /* R/W DATX8 1 Master Delay Line Register 0-1, register number = 552 */
#define	DDR_PUB_DX1MDLR1	(0x8a4) /* R/W DATX8 1 Master Delay Line Register 0-1, register number = 553 */
#define	DDR_PUB_DX1GTR0		(0x8c0) /* R/W DATX8 1 General Timing Register 0, register number = 560 */
#define	DDR_PUB_DX1RSR0		(0x8d0) /* R/W DATX8 1 Rank Status Register 0-3, register number = 564 */
#define	DDR_PUB_DX1RSR1		(0x8d4) /* R/W DATX8 1 Rank Status Register 0-3, register number = 565 */
#define	DDR_PUB_DX1RSR2		(0x8d8) /* R/W DATX8 1 Rank Status Register 0-3, register number = 566 */
#define	DDR_PUB_DX1RSR3		(0x8dc) /* R/W DATX8 1 Rank Status Register 0-3, register number = 567 */
#define	DDR_PUB_DX1GSR0		(0x8e0) /* R DATX8 1 General Status Register 0-6, register number = 568 */
#define	DDR_PUB_DX1GSR1		(0x8e4) /* R DATX8 1 General Status Register 0-6, register number = 569 */
#define	DDR_PUB_DX1GSR2		(0x8e8) /* R DATX8 1 General Status Register 0-6, register number = 570 */
#define	DDR_PUB_DX1GSR3		(0x8ec) /* R DATX8 1 General Status Register 0-6, register number = 571 */
#define	DDR_PUB_DX1GSR4		(0x8f0) /* R DATX8 1 General Status Register 0-6, register number = 572 */
#define	DDR_PUB_DX1GSR5		(0x8f4) /* R DATX8 1 General Status Register 0-6, register number = 573 */
#define	DDR_PUB_DX1GSR6		(0x8f8) /* R DATX8 1 General Status Register 0-6, register number = 574 */
#define	DDR_PUB_DX2GCR0		(0x900) /* R/W DATX8 2 General Configuration Register 0-9, register number = 576 */
#define	DDR_PUB_DX2GCR1		(0x904) /* R/W DATX8 2 General Configuration Register 0-9, register number = 577 */
#define	DDR_PUB_DX2GCR2		(0x908) /* R/W DATX8 2 General Configuration Register 0-9, register number = 578 */
#define	DDR_PUB_DX2GCR3		(0x90c) /* R/W DATX8 2 General Configuration Register 0-9, register number = 579 */
#define	DDR_PUB_DX2GCR4		(0x910) /* R/W DATX8 2 General Configuration Register 0-9, register number = 580 */
#define	DDR_PUB_DX2GCR5		(0x914) /* R/W DATX8 2 General Configuration Register 0-9, register number = 581 */
#define	DDR_PUB_DX2GCR6		(0x918) /* R/W DATX8 2 General Configuration Register 0-9, register number = 582 */
#define	DDR_PUB_DX2GCR7		(0x91c) /* R/W DATX8 2 General Configuration Register 0-9, register number = 583 */
#define	DDR_PUB_DX2GCR8		(0x920) /* R/W DATX8 2 General Configuration Register 0-9, register number = 584 */
#define	DDR_PUB_DX2GCR9		(0x924) /* R/W DATX8 2 General Configuration Register 0-9, register number = 585 */
#define	DDR_PUB_DX2BDLR0	(0x940) /* R/W DATX8 2 Bit Delay Line Register 0-2, register number = 592 */
#define	DDR_PUB_DX2BDLR1	(0x944) /* R/W DATX8 2 Bit Delay Line Register 0-2, register number = 593 */
#define	DDR_PUB_DX2BDLR2	(0x948) /* R/W DATX8 2 Bit Delay Line Register 0-2, register number = 594 */
#define	DDR_PUB_DX2BDLR3	(0x950) /* R/W DATX8 2 Bit Delay Line Register 3-5, register number = 596 */
#define	DDR_PUB_DX2BDLR4	(0x954) /* R/W DATX8 2 Bit Delay Line Register 3-5, register number = 597 */
#define	DDR_PUB_DX2BDLR5	(0x958) /* R/W DATX8 2 Bit Delay Line Register 3-5, register number = 598 */
#define	DDR_PUB_DX2BDLR6	(0x960) /* R/W DATX8 2 Bit Delay Line Register 6-9, register number = 600 */
#define	DDR_PUB_DX2BDLR7	(0x964) /* R/W DATX8 2 Bit Delay Line Register 6-9, register number = 601 */
#define	DDR_PUB_DX2BDLR8	(0x968) /* R/W DATX8 2 Bit Delay Line Register 6-9, register number = 602 */
#define	DDR_PUB_DX2BDLR9	(0x96c) /* R/W DATX8 2 Bit Delay Line Register 6-9, register number = 603 */
#define	DDR_PUB_DX2LCDLR0	(0x980) /* R/W DATX8 2 Local Calibrated Delay Line Register 0-5, register number = 608 */
#define	DDR_PUB_DX2LCDLR1	(0x984) /* R/W DATX8 2 Local Calibrated Delay Line Register 0-5, register number = 609 */
#define	DDR_PUB_DX2LCDLR2	(0x988) /* R/W DATX8 2 Local Calibrated Delay Line Register 0-5, register number = 610 */
#define	DDR_PUB_DX2LCDLR3	(0x98c) /* R/W DATX8 2 Local Calibrated Delay Line Register 0-5, register number = 611 */
#define	DDR_PUB_DX2LCDLR4	(0x990) /* R/W DATX8 2 Local Calibrated Delay Line Register 0-5, register number = 612 */
#define	DDR_PUB_DX2LCDLR5	(0x994) /* R/W DATX8 2 Local Calibrated Delay Line Register 0-5, register number = 613 */
#define	DDR_PUB_DX2MDLR0	(0x9a0) /* R/W DATX8 2 Master Delay Line Register 0-1, register number = 616 */
#define	DDR_PUB_DX2MDLR1	(0x9a4) /* R/W DATX8 2 Master Delay Line Register 0-1, register number = 617 */
#define	DDR_PUB_DX2GTR0		(0x9c0) /* R/W DATX8 2 General Timing Register 0, register number = 624 */
#define	DDR_PUB_DX2RSR0		(0x9d0) /* R/W DATX8 2 Rank Status Register 0-3, register number = 628 */
#define	DDR_PUB_DX2RSR1		(0x9d4) /* R/W DATX8 2 Rank Status Register 0-3, register number = 629 */
#define	DDR_PUB_DX2RSR2		(0x9d8) /* R/W DATX8 2 Rank Status Register 0-3, register number = 630 */
#define	DDR_PUB_DX2RSR3		(0x9dc) /* R/W DATX8 2 Rank Status Register 0-3, register number = 631 */
#define	DDR_PUB_DX2GSR0		(0x9e0) /* R DATX8 2 General Status Register 0-6, register number = 632 */
#define	DDR_PUB_DX2GSR1		(0x9e4) /* R DATX8 2 General Status Register 0-6, register number = 633 */
#define	DDR_PUB_DX2GSR2		(0x9e8) /* R DATX8 2 General Status Register 0-6, register number = 634 */
#define	DDR_PUB_DX2GSR3		(0x9ec) /* R DATX8 2 General Status Register 0-6, register number = 635 */
#define	DDR_PUB_DX2GSR4		(0x9f0) /* R DATX8 2 General Status Register 0-6, register number = 636 */
#define	DDR_PUB_DX2GSR5		(0x9f4) /* R DATX8 2 General Status Register 0-6, register number = 637 */
#define	DDR_PUB_DX2GSR6		(0x9f8) /* R DATX8 2 General Status Register 0-6, register number = 638 */
#define	DDR_PUB_DX3GCR0		(0xa00) /* R/W DATX8 3 General Configuration Register 0-9, register number = 640 */
#define	DDR_PUB_DX3GCR1		(0xa04) /* R/W DATX8 3 General Configuration Register 0-9, register number = 641 */
#define	DDR_PUB_DX3GCR2		(0xa08) /* R/W DATX8 3 General Configuration Register 0-9, register number = 642 */
#define	DDR_PUB_DX3GCR3		(0xa0c) /* R/W DATX8 3 General Configuration Register 0-9, register number = 643 */
#define	DDR_PUB_DX3GCR4		(0xa10) /* R/W DATX8 3 General Configuration Register 0-9, register number = 644 */
#define	DDR_PUB_DX3GCR5		(0xa14) /* R/W DATX8 3 General Configuration Register 0-9, register number = 645 */
#define	DDR_PUB_DX3GCR6		(0xa18) /* R/W DATX8 3 General Configuration Register 0-9, register number = 646 */
#define	DDR_PUB_DX3GCR7		(0xa1c) /* R/W DATX8 3 General Configuration Register 0-9, register number = 647 */
#define	DDR_PUB_DX3GCR8		(0xa20) /* R/W DATX8 3 General Configuration Register 0-9, register number = 648 */
#define	DDR_PUB_DX3GCR9		(0xa24) /* R/W DATX8 3 General Configuration Register 0-9, register number = 649 */
#define	DDR_PUB_DX3BDLR0	(0xa40) /* R/W DATX8 3 Bit Delay Line Register 0-2, register number = 656 */
#define	DDR_PUB_DX3BDLR1	(0xa44) /* R/W DATX8 3 Bit Delay Line Register 0-2, register number = 657 */
#define	DDR_PUB_DX3BDLR2	(0xa48) /* R/W DATX8 3 Bit Delay Line Register 0-2, register number = 658 */
#define	DDR_PUB_DX3BDLR3	(0xa50) /* R/W DATX8 3 Bit Delay Line Register 3-5, register number = 660 */
#define	DDR_PUB_DX3BDLR4	(0xa54) /* R/W DATX8 3 Bit Delay Line Register 3-5, register number = 661 */
#define	DDR_PUB_DX3BDLR5	(0xa58) /* R/W DATX8 3 Bit Delay Line Register 3-5, register number = 662 */
#define	DDR_PUB_DX3BDLR6	(0xa60) /* R/W DATX8 3 Bit Delay Line Register 6-9, register number = 664 */
#define	DDR_PUB_DX3BDLR7	(0xa64) /* R/W DATX8 3 Bit Delay Line Register 6-9, register number = 665 */
#define	DDR_PUB_DX3BDLR8	(0xa68) /* R/W DATX8 3 Bit Delay Line Register 6-9, register number = 666 */
#define	DDR_PUB_DX3BDLR9	(0xa6c) /* R/W DATX8 3 Bit Delay Line Register 6-9, register number = 667 */
#define	DDR_PUB_DX3LCDLR0	(0xa80) /* R/W DATX8 3 Local Calibrated Delay Line Register 0-5, register number = 672 */
#define	DDR_PUB_DX3LCDLR1	(0xa84) /* R/W DATX8 3 Local Calibrated Delay Line Register 0-5, register number = 673 */
#define	DDR_PUB_DX3LCDLR2	(0xa88) /* R/W DATX8 3 Local Calibrated Delay Line Register 0-5, register number = 674 */
#define	DDR_PUB_DX3LCDLR3	(0xa8c) /* R/W DATX8 3 Local Calibrated Delay Line Register 0-5, register number = 675 */
#define	DDR_PUB_DX3LCDLR4	(0xa90) /* R/W DATX8 3 Local Calibrated Delay Line Register 0-5, register number = 676 */
#define	DDR_PUB_DX3LCDLR5	(0xa94) /* R/W DATX8 3 Local Calibrated Delay Line Register 0-5, register number = 677 */
#define	DDR_PUB_DX3MDLR0	(0xaa0) /* R/W DATX8 3 Master Delay Line Register 0-1, register number = 680 */
#define	DDR_PUB_DX3MDLR1	(0xaa4) /* R/W DATX8 3 Master Delay Line Register 0-1, register number = 681 */
#define	DDR_PUB_DX3GTR0		(0xac0) /* R/W DATX8 3 General Timing Register 0, register number = 688 */
#define	DDR_PUB_DX3RSR0		(0xad0) /* R/W DATX8 3 Rank Status Register 0-3, register number = 692 */
#define	DDR_PUB_DX3RSR1		(0xad4) /* R/W DATX8 3 Rank Status Register 0-3, register number = 693 */
#define	DDR_PUB_DX3RSR2		(0xad8) /* R/W DATX8 3 Rank Status Register 0-3, register number = 694 */
#define	DDR_PUB_DX3RSR3		(0xadc) /* R/W DATX8 3 Rank Status Register 0-3, register number = 695 */
#define	DDR_PUB_DX3GSR0		(0xae0) /* R DATX8 3 General Status Register 0-6, register number = 696 */
#define	DDR_PUB_DX3GSR1		(0xae4) /* R DATX8 3 General Status Register 0-6, register number = 697 */
#define	DDR_PUB_DX3GSR2		(0xae8) /* R DATX8 3 General Status Register 0-6, register number = 698 */
#define	DDR_PUB_DX3GSR3		(0xaec) /* R DATX8 3 General Status Register 0-6, register number = 699 */
#define	DDR_PUB_DX3GSR4		(0xaf0) /* R DATX8 3 General Status Register 0-6, register number = 700 */
#define	DDR_PUB_DX3GSR5		(0xaf4) /* R DATX8 3 General Status Register 0-6, register number = 701 */
#define	DDR_PUB_DX3GSR6		(0xaf8) /* R DATX8 3 General Status Register 0-6, register number = 702 */
#define	DDR_PUB_DX4GCR0		(0xb00) /* R/W DATX8 4 General Configuration Register 0-9, register number = 704 */
#define	DDR_PUB_DX4GCR1		(0xb04) /* R/W DATX8 4 General Configuration Register 0-9, register number = 705 */
#define	DDR_PUB_DX4GCR2		(0xb08) /* R/W DATX8 4 General Configuration Register 0-9, register number = 706 */
#define	DDR_PUB_DX4GCR3		(0xb0c) /* R/W DATX8 4 General Configuration Register 0-9, register number = 707 */
#define	DDR_PUB_DX4GCR4		(0xb10) /* R/W DATX8 4 General Configuration Register 0-9, register number = 708 */
#define	DDR_PUB_DX4GCR5		(0xb14) /* R/W DATX8 4 General Configuration Register 0-9, register number = 709 */
#define	DDR_PUB_DX4GCR6		(0xb18) /* R/W DATX8 4 General Configuration Register 0-9, register number = 710 */
#define	DDR_PUB_DX4GCR7		(0xb1c) /* R/W DATX8 4 General Configuration Register 0-9, register number = 711 */
#define	DDR_PUB_DX4GCR8		(0xb20) /* R/W DATX8 4 General Configuration Register 0-9, register number = 712 */
#define	DDR_PUB_DX4GCR9		(0xb24) /* R/W DATX8 4 General Configuration Register 0-9, register number = 713 */
#define	DDR_PUB_DX4BDLR0	(0xb40) /* R/W DATX8 4 Bit Delay Line Register 0-2, register number = 720 */
#define	DDR_PUB_DX4BDLR1	(0xb44) /* R/W DATX8 4 Bit Delay Line Register 0-2, register number = 721 */
#define	DDR_PUB_DX4BDLR2	(0xb48) /* R/W DATX8 4 Bit Delay Line Register 0-2, register number = 722 */
#define	DDR_PUB_DX4BDLR3	(0xb50) /* R/W DATX8 4 Bit Delay Line Register 3-5, register number = 724 */
#define	DDR_PUB_DX4BDLR4	(0xb54) /* R/W DATX8 4 Bit Delay Line Register 3-5, register number = 725 */
#define	DDR_PUB_DX4BDLR5	(0xb58) /* R/W DATX8 4 Bit Delay Line Register 3-5, register number = 726 */
#define	DDR_PUB_DX4BDLR6	(0xb60) /* R/W DATX8 4 Bit Delay Line Register 6-9, register number = 728 */
#define	DDR_PUB_DX4BDLR7	(0xb64) /* R/W DATX8 4 Bit Delay Line Register 6-9, register number = 729 */
#define	DDR_PUB_DX4BDLR8	(0xb68) /* R/W DATX8 4 Bit Delay Line Register 6-9, register number = 730 */
#define	DDR_PUB_DX4BDLR9	(0xb6c) /* R/W DATX8 4 Bit Delay Line Register 6-9, register number = 731 */
#define	DDR_PUB_DX4LCDLR0	(0xb80) /* R/W DATX8 4 Local Calibrated Delay Line Register 0-5, register number = 736 */
#define	DDR_PUB_DX4LCDLR1	(0xb84) /* R/W DATX8 4 Local Calibrated Delay Line Register 0-5, register number = 737 */
#define	DDR_PUB_DX4LCDLR2	(0xb88) /* R/W DATX8 4 Local Calibrated Delay Line Register 0-5, register number = 738 */
#define	DDR_PUB_DX4LCDLR3	(0xb8c) /* R/W DATX8 4 Local Calibrated Delay Line Register 0-5, register number = 739 */
#define	DDR_PUB_DX4LCDLR4	(0xb90) /* R/W DATX8 4 Local Calibrated Delay Line Register 0-5, register number = 740 */
#define	DDR_PUB_DX4LCDLR5	(0xb94) /* R/W DATX8 4 Local Calibrated Delay Line Register 0-5, register number = 741 */
#define	DDR_PUB_DX4MDLR0	(0xba0) /* R/W DATX8 4 Master Delay Line Register 0-1, register number = 744 */
#define	DDR_PUB_DX4MDLR1	(0xba4) /* R/W DATX8 4 Master Delay Line Register 0-1, register number = 745 */
#define	DDR_PUB_DX4GTR0		(0xbc0) /* R/W DATX8 4 General Timing Register 0, register number = 752 */
#define	DDR_PUB_DX4RSR0		(0xbd0) /* R/W DATX8 4 Rank Status Register 0-3, register number = 756 */
#define	DDR_PUB_DX4RSR1		(0xbd4) /* R/W DATX8 4 Rank Status Register 0-3, register number = 757 */
#define	DDR_PUB_DX4RSR2		(0xbd8) /* R/W DATX8 4 Rank Status Register 0-3, register number = 758 */
#define	DDR_PUB_DX4RSR3		(0xbdc) /* R/W DATX8 4 Rank Status Register 0-3, register number = 759 */
#define	DDR_PUB_DX4GSR0		(0xbe0) /* R DATX8 4 General Status Register 0-6, register number = 760 */
#define	DDR_PUB_DX4GSR1		(0xbe4) /* R DATX8 4 General Status Register 0-6, register number = 761 */
#define	DDR_PUB_DX4GSR2		(0xbe8) /* R DATX8 4 General Status Register 0-6, register number = 762 */
#define	DDR_PUB_DX4GSR3		(0xbec) /* R DATX8 4 General Status Register 0-6, register number = 763 */
#define	DDR_PUB_DX4GSR4		(0xbf0) /* R DATX8 4 General Status Register 0-6, register number = 764 */
#define	DDR_PUB_DX4GSR5		(0xbf4) /* R DATX8 4 General Status Register 0-6, register number = 765 */
#define	DDR_PUB_DX4GSR6		(0xbf8) /* R DATX8 4 General Status Register 0-6, register number = 766 */
#define	DDR_PUB_DX5GCR0		(0xc00) /* R/W DATX8 5 General Configuration Register 0-9, register number = 768 */
#define	DDR_PUB_DX5GCR1		(0xc04) /* R/W DATX8 5 General Configuration Register 0-9, register number = 769 */
#define	DDR_PUB_DX5GCR2		(0xc08) /* R/W DATX8 5 General Configuration Register 0-9, register number = 770 */
#define	DDR_PUB_DX5GCR3		(0xc0c) /* R/W DATX8 5 General Configuration Register 0-9, register number = 771 */
#define	DDR_PUB_DX5GCR4		(0xc10) /* R/W DATX8 5 General Configuration Register 0-9, register number = 772 */
#define	DDR_PUB_DX5GCR5		(0xc14) /* R/W DATX8 5 General Configuration Register 0-9, register number = 773 */
#define	DDR_PUB_DX5GCR6		(0xc18) /* R/W DATX8 5 General Configuration Register 0-9, register number = 774 */
#define	DDR_PUB_DX5GCR7		(0xc1c) /* R/W DATX8 5 General Configuration Register 0-9, register number = 775 */
#define	DDR_PUB_DX5GCR8		(0xc20) /* R/W DATX8 5 General Configuration Register 0-9, register number = 776 */
#define	DDR_PUB_DX5GCR9		(0xc24) /* R/W DATX8 5 General Configuration Register 0-9, register number = 777 */
#define	DDR_PUB_DX5BDLR0	(0xc40) /* R/W DATX8 5 Bit Delay Line Register 0-2, register number = 784 */
#define	DDR_PUB_DX5BDLR1	(0xc44) /* R/W DATX8 5 Bit Delay Line Register 0-2, register number = 785 */
#define	DDR_PUB_DX5BDLR2	(0xc48) /* R/W DATX8 5 Bit Delay Line Register 0-2, register number = 786 */
#define	DDR_PUB_DX5BDLR3	(0xc50) /* R/W DATX8 5 Bit Delay Line Register 3-5, register number = 788 */
#define	DDR_PUB_DX5BDLR4	(0xc54) /* R/W DATX8 5 Bit Delay Line Register 3-5, register number = 789 */
#define	DDR_PUB_DX5BDLR5	(0xc58) /* R/W DATX8 5 Bit Delay Line Register 3-5, register number = 790 */
#define	DDR_PUB_DX5BDLR6	(0xc60) /* R/W DATX8 5 Bit Delay Line Register 6-9, register number = 792 */
#define	DDR_PUB_DX5BDLR7	(0xc64) /* R/W DATX8 5 Bit Delay Line Register 6-9, register number = 793 */
#define	DDR_PUB_DX5BDLR8	(0xc68) /* R/W DATX8 5 Bit Delay Line Register 6-9, register number = 794 */
#define	DDR_PUB_DX5BDLR9	(0xc6c) /* R/W DATX8 5 Bit Delay Line Register 6-9, register number = 795 */
#define	DDR_PUB_DX5LCDLR0	(0xc80) /* R/W DATX8 5 Local Calibrated Delay Line Register 0-5, register number = 800 */
#define	DDR_PUB_DX5LCDLR1	(0xc84) /* R/W DATX8 5 Local Calibrated Delay Line Register 0-5, register number = 801 */
#define	DDR_PUB_DX5LCDLR2	(0xc88) /* R/W DATX8 5 Local Calibrated Delay Line Register 0-5, register number = 802 */
#define	DDR_PUB_DX5LCDLR3	(0xc8c) /* R/W DATX8 5 Local Calibrated Delay Line Register 0-5, register number = 803 */
#define	DDR_PUB_DX5LCDLR4	(0xc90) /* R/W DATX8 5 Local Calibrated Delay Line Register 0-5, register number = 804 */
#define	DDR_PUB_DX5LCDLR5	(0xc94) /* R/W DATX8 5 Local Calibrated Delay Line Register 0-5, register number = 805 */
#define	DDR_PUB_DX5MDLR0	(0xca0) /* R/W DATX8 5 Master Delay Line Register 0-1, register number = 808 */
#define	DDR_PUB_DX5MDLR1	(0xca4) /* R/W DATX8 5 Master Delay Line Register 0-1, register number = 809 */
#define	DDR_PUB_DX5GTR0		(0xcc0) /* R/W DATX8 5 General Timing Register 0, register number = 816 */
#define	DDR_PUB_DX5RSR0		(0xcd0) /* R/W DAXT8 5 Rank Status Register 0-3, register number = 820 */
#define	DDR_PUB_DX5RSR1		(0xcd4) /* R/W DAXT8 5 Rank Status Register 0-3, register number = 821 */
#define	DDR_PUB_DX5RSR2		(0xcd8) /* R/W DAXT8 5 Rank Status Register 0-3, register number = 822 */
#define	DDR_PUB_DX5RSR3		(0xcdc) /* R/W DAXT8 5 Rank Status Register 0-3, register number = 823 */
#define	DDR_PUB_DX5GSR0		(0xce0) /* R DATX8 5 General Status Register 0-6, register number = 824 */
#define	DDR_PUB_DX5GSR1		(0xce4) /* R DATX8 5 General Status Register 0-6, register number = 825 */
#define	DDR_PUB_DX5GSR2		(0xce8) /* R DATX8 5 General Status Register 0-6, register number = 826 */
#define	DDR_PUB_DX5GSR3		(0xcec) /* R DATX8 5 General Status Register 0-6, register number = 827 */
#define	DDR_PUB_DX5GSR4		(0xcf0) /* R DATX8 5 General Status Register 0-6, register number = 828 */
#define	DDR_PUB_DX5GSR5		(0xcf4) /* R DATX8 5 General Status Register 0-6, register number = 829 */
#define	DDR_PUB_DX5GSR6		(0xcf8) /* R DATX8 5 General Status Register 0-6, register number = 830 */
#define	DDR_PUB_DX6GCR0		(0xd00) /* R/W DATX8 6 General Configuration Register 0-9, register number = 832 */
#define	DDR_PUB_DX6GCR1		(0xd04) /* R/W DATX8 6 General Configuration Register 0-9, register number = 833 */
#define	DDR_PUB_DX6GCR2		(0xd08) /* R/W DATX8 6 General Configuration Register 0-9, register number = 834 */
#define	DDR_PUB_DX6GCR3		(0xd0c) /* R/W DATX8 6 General Configuration Register 0-9, register number = 835 */
#define	DDR_PUB_DX6GCR4		(0xd10) /* R/W DATX8 6 General Configuration Register 0-9, register number = 836 */
#define	DDR_PUB_DX6GCR5		(0xd14) /* R/W DATX8 6 General Configuration Register 0-9, register number = 837 */
#define	DDR_PUB_DX6GCR6		(0xd18) /* R/W DATX8 6 General Configuration Register 0-9, register number = 838 */
#define	DDR_PUB_DX6GCR7		(0xd1c) /* R/W DATX8 6 General Configuration Register 0-9, register number = 839 */
#define	DDR_PUB_DX6GCR8		(0xd20) /* R/W DATX8 6 General Configuration Register 0-9, register number = 840 */
#define	DDR_PUB_DX6GCR9		(0xd24) /* R/W DATX8 6 General Configuration Register 0-9, register number = 841 */
#define	DDR_PUB_DX6BDLR0	(0xd40) /* R/W DATX8 6 Bit Delay Line Register 0-2, register number = 848 */
#define	DDR_PUB_DX6BDLR1	(0xd44) /* R/W DATX8 6 Bit Delay Line Register 0-2, register number = 849 */
#define	DDR_PUB_DX6BDLR2	(0xd48) /* R/W DATX8 6 Bit Delay Line Register 0-2, register number = 850 */
#define	DDR_PUB_DX6BDLR3	(0xd50) /* R/W DATX8 6 Bit Delay Line Register 3-5, register number = 852 */
#define	DDR_PUB_DX6BDLR4	(0xd54) /* R/W DATX8 6 Bit Delay Line Register 3-5, register number = 853 */
#define	DDR_PUB_DX6BDLR5	(0xd58) /* R/W DATX8 6 Bit Delay Line Register 3-5, register number = 854 */
#define	DDR_PUB_DX6BDLR6	(0xd60) /* R/W DATX8 6 Bit Delay Line Register 6-9, register number = 856 */
#define	DDR_PUB_DX6BDLR7	(0xd64) /* R/W DATX8 6 Bit Delay Line Register 6-9, register number = 857 */
#define	DDR_PUB_DX6BDLR8	(0xd68) /* R/W DATX8 6 Bit Delay Line Register 6-9, register number = 858 */
#define	DDR_PUB_DX6BDLR9	(0xd6c) /* R/W DATX8 6 Bit Delay Line Register 6-9, register number = 859 */
#define	DDR_PUB_DX6LCDLR0	(0xd80) /* R/W DATX8 6 Local Calibrated Delay Line Register 0-5, register number = 864 */
#define	DDR_PUB_DX6LCDLR1	(0xd84) /* R/W DATX8 6 Local Calibrated Delay Line Register 0-5, register number = 865 */
#define	DDR_PUB_DX6LCDLR2	(0xd88) /* R/W DATX8 6 Local Calibrated Delay Line Register 0-5, register number = 866 */
#define	DDR_PUB_DX6LCDLR3	(0xd8c) /* R/W DATX8 6 Local Calibrated Delay Line Register 0-5, register number = 867 */
#define	DDR_PUB_DX6LCDLR4	(0xd90) /* R/W DATX8 6 Local Calibrated Delay Line Register 0-5, register number = 868 */
#define	DDR_PUB_DX6LCDLR5	(0xd94) /* R/W DATX8 6 Local Calibrated Delay Line Register 0-5, register number = 869 */
#define	DDR_PUB_DX6MDLR0	(0xda0) /* R/W DATX8 6 Master Delay Line Register 0-1, register number = 872 */
#define	DDR_PUB_DX6MDLR1	(0xda4) /* R/W DATX8 6 Master Delay Line Register 0-1, register number = 873 */
#define	DDR_PUB_DX6GTR0		(0xdc0) /* R/W DATX8 6 General Timing Register 0, register number = 880 */
#define	DDR_PUB_DX6RSR0		(0xdd0) /* R/W DAXT8 6 Rank Status Register 0-3, register number = 884 */
#define	DDR_PUB_DX6RSR1		(0xdd4) /* R/W DAXT8 6 Rank Status Register 0-3, register number = 885 */
#define	DDR_PUB_DX6RSR2		(0xdd8) /* R/W DAXT8 6 Rank Status Register 0-3, register number = 886 */
#define	DDR_PUB_DX6RSR3		(0xddc) /* R/W DAXT8 6 Rank Status Register 0-3, register number = 887 */
#define	DDR_PUB_DX6GSR0		(0xde0) /* R DATX8 6 General Status Register 0-6, register number = 888 */
#define	DDR_PUB_DX6GSR1		(0xde4) /* R DATX8 6 General Status Register 0-6, register number = 889 */
#define	DDR_PUB_DX6GSR2		(0xde8) /* R DATX8 6 General Status Register 0-6, register number = 890 */
#define	DDR_PUB_DX6GSR3		(0xdec) /* R DATX8 6 General Status Register 0-6, register number = 891 */
#define	DDR_PUB_DX6GSR4		(0xdf0) /* R DATX8 6 General Status Register 0-6, register number = 892 */
#define	DDR_PUB_DX6GSR5		(0xdf4) /* R DATX8 6 General Status Register 0-6, register number = 893 */
#define	DDR_PUB_DX6GSR6		(0xdf8) /* R DATX8 6 General Status Register 0-6, register number = 894 */
#define	DDR_PUB_DX7GCR0		(0xe00) /* R/W DATX8 7 General Configuration Register 0-9, register number = 896 */
#define	DDR_PUB_DX7GCR1		(0xe04) /* R/W DATX8 7 General Configuration Register 0-9, register number = 897 */
#define	DDR_PUB_DX7GCR2		(0xe08) /* R/W DATX8 7 General Configuration Register 0-9, register number = 898 */
#define	DDR_PUB_DX7GCR3		(0xe0c) /* R/W DATX8 7 General Configuration Register 0-9, register number = 899 */
#define	DDR_PUB_DX7GCR4		(0xe10) /* R/W DATX8 7 General Configuration Register 0-9, register number = 900 */
#define	DDR_PUB_DX7GCR5		(0xe14) /* R/W DATX8 7 General Configuration Register 0-9, register number = 901 */
#define	DDR_PUB_DX7GCR6		(0xe18) /* R/W DATX8 7 General Configuration Register 0-9, register number = 902 */
#define	DDR_PUB_DX7GCR7		(0xe1c) /* R/W DATX8 7 General Configuration Register 0-9, register number = 903 */
#define	DDR_PUB_DX7GCR8		(0xe20) /* R/W DATX8 7 General Configuration Register 0-9, register number = 904 */
#define	DDR_PUB_DX7GCR9		(0xe24) /* R/W DATX8 7 General Configuration Register 0-9, register number = 905 */
#define	DDR_PUB_DX7BDLR0	(0xe40) /* R/W DATX8 7 Bit Delay Line Register 0-2, register number = 912 */
#define	DDR_PUB_DX7BDLR1	(0xe44) /* R/W DATX8 7 Bit Delay Line Register 0-2, register number = 913 */
#define	DDR_PUB_DX7BDLR2	(0xe48) /* R/W DATX8 7 Bit Delay Line Register 0-2, register number = 914 */
#define	DDR_PUB_DX7BDLR3	(0xe50) /* R/W DATX8 7 Bit Delay Line Register 3-5, register number = 916 */
#define	DDR_PUB_DX7BDLR4	(0xe54) /* R/W DATX8 7 Bit Delay Line Register 3-5, register number = 917 */
#define	DDR_PUB_DX7BDLR5	(0xe58) /* R/W DATX8 7 Bit Delay Line Register 3-5, register number = 918 */
#define	DDR_PUB_DX7BDLR6	(0xe60) /* R/W DATX8 7 Bit Delay Line Register 6-9, register number = 920 */
#define	DDR_PUB_DX7BDLR7	(0xe64) /* R/W DATX8 7 Bit Delay Line Register 6-9, register number = 921 */
#define	DDR_PUB_DX7BDLR8	(0xe68) /* R/W DATX8 7 Bit Delay Line Register 6-9, register number = 922 */
#define	DDR_PUB_DX7BDLR9	(0xe6c) /* R/W DATX8 7 Bit Delay Line Register 6-9, register number = 923 */
#define	DDR_PUB_DX7LCDLR0	(0xe80) /* R/W DATX8 7 Local Calibrated Delay Line Register 0-5, register number = 928 */
#define	DDR_PUB_DX7LCDLR1	(0xe84) /* R/W DATX8 7 Local Calibrated Delay Line Register 0-5, register number = 929 */
#define	DDR_PUB_DX7LCDLR2	(0xe88) /* R/W DATX8 7 Local Calibrated Delay Line Register 0-5, register number = 930 */
#define	DDR_PUB_DX7LCDLR3	(0xe8c) /* R/W DATX8 7 Local Calibrated Delay Line Register 0-5, register number = 931 */
#define	DDR_PUB_DX7LCDLR4	(0xe90) /* R/W DATX8 7 Local Calibrated Delay Line Register 0-5, register number = 932 */
#define	DDR_PUB_DX7LCDLR5	(0xe94) /* R/W DATX8 7 Local Calibrated Delay Line Register 0-5, register number = 933 */
#define	DDR_PUB_DX7MDLR0	(0xea0) /* R/W DATX8 7 Master Delay Line Register 0-1, register number = 936 */
#define	DDR_PUB_DX7MDLR1	(0xea4) /* R/W DATX8 7 Master Delay Line Register 0-1, register number = 937 */
#define	DDR_PUB_DX7GTR0		(0xec0) /* R/W DATX8 7 General Timing Register 0, register number = 944 */
#define	DDR_PUB_DX7RSR0		(0xed0) /* R/W DATX8 7 Rank Status Register 0-3, register number = 948 */
#define	DDR_PUB_DX7RSR1		(0xed4) /* R/W DATX8 7 Rank Status Register 0-3, register number = 949 */
#define	DDR_PUB_DX7RSR2		(0xed8) /* R/W DATX8 7 Rank Status Register 0-3, register number = 950 */
#define	DDR_PUB_DX7RSR3		(0xedc) /* R/W DATX8 7 Rank Status Register 0-3, register number = 951 */
#define	DDR_PUB_DX7GSR0		(0xee0) /* R DATX8 7 General Status Register 0-6, register number = 952 */
#define	DDR_PUB_DX7GSR1		(0xee4) /* R DATX8 7 General Status Register 0-6, register number = 953 */
#define	DDR_PUB_DX7GSR2		(0xee8) /* R DATX8 7 General Status Register 0-6, register number = 954 */
#define	DDR_PUB_DX7GSR3		(0xeec) /* R DATX8 7 General Status Register 0-6, register number = 955 */
#define	DDR_PUB_DX7GSR4		(0xef0) /* R DATX8 7 General Status Register 0-6, register number = 956 */
#define	DDR_PUB_DX7GSR5		(0xef4) /* R DATX8 7 General Status Register 0-6, register number = 957 */
#define	DDR_PUB_DX7GSR6		(0xef8) /* R DATX8 7 General Status Register 0-6, register number = 958 */
#define	DDR_PUB_DX8GCR0		(0xf00) /* R/W DATX8 8 General Configuration Register 0-9, register number = 960 */
#define	DDR_PUB_DX8GCR1		(0xf04) /* R/W DATX8 8 General Configuration Register 0-9, register number = 961 */
#define	DDR_PUB_DX8GCR2		(0xf08) /* R/W DATX8 8 General Configuration Register 0-9, register number = 962 */
#define	DDR_PUB_DX8GCR3		(0xf0c) /* R/W DATX8 8 General Configuration Register 0-9, register number = 963 */
#define	DDR_PUB_DX8GCR4		(0xf10) /* R/W DATX8 8 General Configuration Register 0-9, register number = 964 */
#define	DDR_PUB_DX8GCR5		(0xf14) /* R/W DATX8 8 General Configuration Register 0-9, register number = 965 */
#define	DDR_PUB_DX8GCR6		(0xf18) /* R/W DATX8 8 General Configuration Register 0-9, register number = 966 */
#define	DDR_PUB_DX8GCR7		(0xf1c) /* R/W DATX8 8 General Configuration Register 0-9, register number = 967 */
#define	DDR_PUB_DX8GCR8		(0xf20) /* R/W DATX8 8 General Configuration Register 0-9, register number = 968 */
#define	DDR_PUB_DX8GCR9		(0xf24) /* R/W DATX8 8 General Configuration Register 0-9, register number = 969 */
#define	DDR_PUB_DX8BDLR0	(0xf40) /* R/W DATX8 8 Bit Delay Line Register 0-2, register number = 976 */
#define	DDR_PUB_DX8BDLR1	(0xf44) /* R/W DATX8 8 Bit Delay Line Register 0-2, register number = 977 */
#define	DDR_PUB_DX8BDLR2	(0xf48) /* R/W DATX8 8 Bit Delay Line Register 0-2, register number = 978 */
#define	DDR_PUB_DX8BDLR3	(0xf50) /* R/W DATX8 8 Bit Delay Line Register 3-5, register number = 980 */
#define	DDR_PUB_DX8BDLR4	(0xf54) /* R/W DATX8 8 Bit Delay Line Register 3-5, register number = 981 */
#define	DDR_PUB_DX8BDLR5	(0xf58) /* R/W DATX8 8 Bit Delay Line Register 3-5, register number = 982 */
#define	DDR_PUB_DX8BDLR6	(0xf60) /* R/W DATX8 8 Bit Delay Line Register 6-9, register number = 984 */
#define	DDR_PUB_DX8BDLR7	(0xf64) /* R/W DATX8 8 Bit Delay Line Register 6-9, register number = 985 */
#define	DDR_PUB_DX8BDLR8	(0xf68) /* R/W DATX8 8 Bit Delay Line Register 6-9, register number = 986 */
#define	DDR_PUB_DX8BDLR9	(0xf6c) /* R/W DATX8 8 Bit Delay Line Register 6-9, register number = 987 */
#define	DDR_PUB_DX8LCDLR0	(0xf80) /* R/W DATX8 8 Local Calibrated Delay Line Register 0-5, register number = 992 */
#define	DDR_PUB_DX8LCDLR1	(0xf84) /* R/W DATX8 8 Local Calibrated Delay Line Register 0-5, register number = 993 */
#define	DDR_PUB_DX8LCDLR2	(0xf88) /* R/W DATX8 8 Local Calibrated Delay Line Register 0-5, register number = 994 */
#define	DDR_PUB_DX8LCDLR3	(0xf8c) /* R/W DATX8 8 Local Calibrated Delay Line Register 0-5, register number = 995 */
#define	DDR_PUB_DX8LCDLR4	(0xf90) /* R/W DATX8 8 Local Calibrated Delay Line Register 0-5, register number = 996 */
#define	DDR_PUB_DX8LCDLR5	(0xf94) /* R/W DATX8 8 Local Calibrated Delay Line Register 0-5, register number = 997 */
#define	DDR_PUB_DX8MDLR0	(0xfa0) /* R/W DATX8 8 Master Delay Line Register 0-1, register number = 1000 */
#define	DDR_PUB_DX8MDLR1	(0xfa4) /* R/W DATX8 8 Master Delay Line Register 0-1, register number = 1001 */
#define	DDR_PUB_DX8GTR0		(0xfc0) /* R/W DATX8 8 General Timing Register 0, register number = 1008 */
#define	DDR_PUB_DX8RSR0		(0xfd0) /* R/W DATX8 8 Rank Status Register 0-3, register number = 1012 */
#define	DDR_PUB_DX8RSR1		(0xfd4) /* R/W DATX8 8 Rank Status Register 0-3, register number = 1013 */
#define	DDR_PUB_DX8RSR2		(0xfd8) /* R/W DATX8 8 Rank Status Register 0-3, register number = 1014 */
#define	DDR_PUB_DX8RSR3		(0xfdc) /* R/W DATX8 8 Rank Status Register 0-3, register number = 1015 */
#define	DDR_PUB_DX8GSR0		(0xfe0) /* R DATX8 8 General Status Register 0-6, register number = 1016 */
#define	DDR_PUB_DX8GSR1		(0xfe4) /* R DATX8 8 General Status Register 0-6, register number = 1017 */
#define	DDR_PUB_DX8GSR2		(0xfe8) /* R DATX8 8 General Status Register 0-6, register number = 1018 */
#define	DDR_PUB_DX8GSR3		(0xfec) /* R DATX8 8 General Status Register 0-6, register number = 1019 */
#define	DDR_PUB_DX8GSR4		(0xff0) /* R DATX8 8 General Status Register 0-6, register number = 1020 */
#define	DDR_PUB_DX8GSR5		(0xff4) /* R DATX8 8 General Status Register 0-6, register number = 1021 */
#define	DDR_PUB_DX8GSR6		(0xff8) /* R DATX8 8 General Status Register 0-6, register number = 1022 */


/* PHY General Status Register Bits, register number = 12 */
#define DDR_PUB_PGSR_IDONE	(1 << 0)	/* Initialization Done */
#define DDR_PUB_PGSR_ZCERR	(1 << 20)	/* Impedance Calibration Error */
#define DDR_PUB_PGSR_TRAIN_ERRBIT_START	(19)	/* Start error training bit. (VERR) */
#define DDR_PUB_PGSR_TRAIN_ERRBIT_LEN	(12)	/* Length of the error bit field */




#endif /* __DDR_H__ */
