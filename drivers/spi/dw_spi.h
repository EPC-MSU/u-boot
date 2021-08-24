/*
 * Synopsis DesignWire SPI driver header file
 *
 * (C) Copyright 2013-2015 Baikal Electronics
 * http://www.baikalelectronics.ru
 *
 * Authors:
 *   Dmitry Dunaev <dmitry.dunaev@baikalelectronics.ru>
 *   Alexey.Malahov <alexey.malahov@baikalelectronics.ru>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef _DW_SPI_H_
#define _DW_SPI_H_

struct dw_spi_regs {
	u32	ctrl0;		/* 0x00 */
	u32	ctrl1;		/* 0x04 */
	u32	ssien;		/* 0x08 */
	u32	mwcr;		/* 0x0c */
	u32	ser;		/* 0x10 */
	u32	baud;		/* 0x14 */
	u32	txftl;		/* 0x18 */
	u32	rxftl;		/* 0x1c */
	u32	txfl;		/* 0x20 */
	u32	rxfl;		/* 0x24 */
	u32	sr;			/* 0x28 */
	u32	imr;		/* 0x2C */
	u32	isr;		/* 0x30 */
	u32	risr;		/* 0x34 */
	u32	txoicr;		/* 0x38 */
	u32	rxoicr;		/* 0x3C */
	u32	rxuicr;		/* 0x40 */
	u32	msticr;		/* 0x44 */
	u32	icr;		/* 0x48 */
	u32	dmacr;		/* 0x4C */
	u32	dmatdlr;	/* 0x50 */
	u32	dmardlr;	/* 0x54 */
	u32	idr;		/* 0x58 */
	u32	ver;		/* 0x5C */
	u32	dr;			/* 0x60 */
	u8 align[16];	/* 0x64-0xEC */
	u8 rx_smpl_dly;	/* 0xF0 */
	u32 rsvd[3];	/* 0xF4 - 0xFC */
};

#define BIT(x)			(1 << (x))
#define MASK(x)			((1 << (x)) - 1)

/* Bit fields in CTRLR0 */
#define SPI_DFS_OFFSET			0
#define SPI_DFS_DEFAULT			0x07

#define SPI_FRF_OFFSET			4
#define SPI_FRF_SPI				0x0
#define SPI_FRF_SSP				0x1
#define SPI_FRF_MICROWIRE		0x2
#define SPI_FRF_RESV			0x3

#define SPI_MODE_OFFSET			6
#define SPI_SCPHA_OFFSET		6
#define SPI_SCPOL_OFFSET		7
#define SPI_SLVOE_OFFSET		10
#define SPI_SRL_OFFSET			11
#define SPI_CFS_OFFSET			12

/* Bit fields in SSIEN */
#define SPI_DISABLE				0x0
#define SPI_ENABLE				0x1

/* Bit fields in SR */
#define SPI_SR_DCOL				BIT(6)
#define SPI_SR_TXE				BIT(5)
#define SPI_SR_RFF				BIT(4)
#define SPI_SR_RFNE				BIT(3)
#define SPI_SR_TFE				BIT(2)
#define SPI_SR_TFNF				BIT(1)
#define SPI_SR_BUSY				BIT(0)

#define SPI_TMOD_OFFSET			8
#define	SPI_TMOD_TR				0x0		/* xmit & recv */
#define SPI_TMOD_TO				0x1		/* xmit only */
#define SPI_TMOD_RO				0x2		/* recv only */
#define SPI_TMOD_EPROMREAD		0x3		/* eeprom read mode */
#define SPI_TMOD_MASK			(0x3 << SPI_TMOD_OFFSET)

//#define SPI_DR_IDLE_VAL			0x00
#define SPI_DR_IDLE_VAL			0xff
#define SPI_TICK_BLOCK_SIZE		(8 * 1024) /* 8 KiB */

/* Enable bit. */
#define SSIENR_SSI_EN (1 << 0)

/* Transfer mode bits */
#define SPI_TMOD_OFFSET                 8
#define SPI_TMOD_MASK                   (0x3 << SPI_TMOD_OFFSET)
#define SPI_TMOD_TR                     0x0 /* transmit and receive */
#define SPI_TMOD_TO                     0x1 /* transmit only */
#define SPI_TMOD_RO                     0x2 /* receive only */
#define SPI_TMOD_EPROMREAD              0x3 /* EEPROM read */

#define OK                      0
#define ERROR                   -1

#define SPI_DUMMY 0
#define MAX_SPI_TRIES   10000
#define SPI_FIFO_SIZE   CONFIG_SPI_FIFO_SIZE

/* Slave output enable. */
#define SPI_SLV_OE_OFFSET               10
#define SPI_SLV_OE                      1

/* Serial Clock Polarity. */
#define SPI_SCPOL_OFFSET                7
#define SPI_SCPOL_LOW                   0
#define SPI_SCPOL_HIGH                  1

/* Serial Clock Phase. */
#define SPI_SCPH_OFFSET                 6
#define SPI_SCPH_MIDDLE                 0
#define SPI_SCPH_START                  1

/* Frame format. */
#define SPI_FRF_OFFSET                  4
#define SPI_FRF_SPI                     0x0
#define SPI_FRF_SSP                     0x1
#define SPI_FRF_MICROWIRE               0x2
#define SPI_FRF_RESV                    0x3

/* Data Frame Size */
#define SPI_DFS(x)                      (x - 1)


#if 0



/* Bit fields in ISR, IMR, RISR, 7 bits */
#define SPI_INT_TXEI			BIT(0)
#define SPI_INT_TXOI			BIT(1)
#define SPI_INT_RXUI			BIT(2)
#define SPI_INT_RXOI			BIT(3)
#define SPI_INT_RXFI			BIT(4)
#define SPI_INT_MSTI			BIT(5)

/* TX RX interrupt level threshold, max can be 256 */
#define SPI_INT_THRESHOLD		32

#endif

#define SPI_IN_BUF_SIZE                (2 * SPI_FIFO_SIZE)

struct dw_spi_slave {
	struct spi_slave slave;
	struct dw_spi_regs *regs;
	unsigned int freq;
	unsigned int mode;
	unsigned int fifo;
	unsigned char in[SPI_IN_BUF_SIZE];
	int in_len;
	void *out;
	int out_len;
};

static inline struct dw_spi_slave *to_dw_spi(struct spi_slave *slave)
{
	return container_of(slave, struct dw_spi_slave, slave);
}

#endif /* _DW_SPI_H_ */
