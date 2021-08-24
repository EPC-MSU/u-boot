/*
 * Synopsis DesignWire SPI driver
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

#include <common.h>
#include <spi.h>
#include <malloc.h>
#include <asm/io.h>
#include "dw_spi.h"

#ifndef CONFIG_DW_MAX_CS
#define CONFIG_DW_MAX_CS	8
#endif

static const unsigned int spi_bus_regs[] = {
#ifdef CONFIG_SPI0_BASE
	CONFIG_SPI0_BASE,
#endif
#ifdef CONFIG_SPI1_BASE
	CONFIG_SPI1_BASE,
#endif
#ifdef CONFIG_SPI2_BASE
	CONFIG_SPI2_BASE,
#endif
#ifdef CONFIG_SPI3_BASE
	CONFIG_SPI3_BASE,
#endif
};

struct spi_slave *spi_setup_slave(unsigned int bus, unsigned int cs,
			unsigned int max_hz, unsigned int mode)
{
	struct dw_spi_slave	*dws;

	/* Check bus exists */
	if (bus > ARRAY_SIZE(spi_bus_regs) - 1)
		return NULL;

	/* Check chip select line */
	if (!spi_cs_is_valid(bus, cs))
		return NULL;

	/* Try to allocate SPI slave device */
	dws = spi_alloc_slave(struct dw_spi_slave, bus, cs);
	if (!dws)
		return NULL;

	/* Set base address for hardware registers */
//	dws->regs = (struct dw_spi_regs *) ((int) SPI_BASE + SPI_CHUNK * (bus));
	dws->regs = (struct dw_spi_regs *) spi_bus_regs[bus];

	/* Save mode and max freq */
	dws->freq = CONFIG_DW_SPI_FREQ;
	dws->mode = mode;
	dws->in_len = 0;

	printf("%s: bus:%i cs:%i base:%p freq=%d\n", __func__,
		bus, cs, dws->regs, dws->freq);

	/*
	 * Try to detect the FIFO depth if not set by interface driver,
	 * the depth could be from 2 to 256 from HW spec
	 */
	if (!dws->fifo) {
		u32 fifo;

		for (fifo = 1; fifo < 256; fifo++) {
			writel(fifo, &dws->regs->txftl);
			if (fifo != readl(&dws->regs->txftl))
				break;
		}

		dws->fifo = (fifo == 1) ? 0 : fifo;
		writel(0, &dws->regs->txftl);
	}
	printf("%s: fifo:%d\n", __func__, dws->fifo);

	return &dws->slave;
}

void spi_free_slave(struct spi_slave *slave)
{
	struct dw_spi_slave *dws = to_dw_spi(slave);

	/* Free allocated memory. Nothing else. */
	free(dws);
}

int spi_claim_bus(struct spi_slave *slave)
{
	struct dw_spi_slave *dws = to_dw_spi(slave);
	uint32_t reg;

	printf("%s: bus:%i cs:%i\n", __func__, slave->bus, slave->cs);

	/* Disable device. */
	writel(0, &dws->regs->ssien);

	/* Mask all interrupts. */
	writel(0, &dws->regs->imr);

	/* Set CTRLR0 */
	reg = (SPI_TMOD_TR << SPI_TMOD_OFFSET); /* transmit and receive */
	reg |= (SPI_SCPOL_LOW << SPI_SCPOL_OFFSET);
	reg |= (SPI_SCPH_MIDDLE << SPI_SCPH_OFFSET);
	reg |= (SPI_FRF_SPI << SPI_FRF_OFFSET);
	reg |= SPI_DFS(8);
	writel(reg, &dws->regs->ctrl0);

	/* Specify the baud rate */
	spi_set_speed(slave, dws->freq);

	/* Set thresholds. */
	writel(48, &dws->regs->txftl);
	writel(48, &dws->regs->rxftl);

	sync();

	/* Enable device. */
	writel(SSIENR_SSI_EN, &dws->regs->ssien);

	return 0;
}

void spi_release_bus(struct spi_slave *slave)
{
	struct dw_spi_slave *dws = to_dw_spi(slave);
	/* Disable SPI bus */
	writel(SPI_DISABLE, &dws->regs->ssien);
}

void spi_set_speed(struct spi_slave *slave, uint hz)
{
	struct dw_spi_slave *dws = to_dw_spi(slave);
	unsigned int val;

	val = (CONFIG_DW_CLOCK_FREQ / hz);

	/* SPI divider should be even. */
	if (val & 0x1) {
		val++;
	}
	val &= 0xFFFF;
	writel(val, &dws->regs->baud);
	printf("%s: baud:%u\n", __func__, readl(&dws->regs->baud));
}

void dw_set_cs(struct spi_slave *slave)
{
	struct dw_spi_slave *dws = to_dw_spi(slave);
	uint32_t reg;

	reg = readl(&dws->regs->ser);

	writel((reg | BIT(slave->cs)), &dws->regs->ser);
}

void dw_clear_cs(struct spi_slave *slave)
{
	struct dw_spi_slave *dws = to_dw_spi(slave);
	uint32_t reg;

	reg = readl(&dws->regs->ser);

	writel((reg & (~BIT(slave->cs))), &dws->regs->ser);
}

void dw_spi_send(struct spi_slave *slave, uint8_t *buf, int tlen, int dummy)
{
	struct dw_spi_slave *dws = to_dw_spi(slave);
	int i;

	for (i = 0; ((i < tlen) && (buf)); i++) {
		writel(buf[i], &dws->regs->dr);
	}

	for (i = 0; i < dummy; i++) {
		writel(SPI_DUMMY, &dws->regs->dr);
	}
}

void dw_spi_recv(struct spi_slave *slave, uint8_t *buf, int tlen, int dummy)
{
	struct dw_spi_slave *dws = to_dw_spi(slave);

	int i;
	for (i = 0; i < dummy; i++) {
		buf[0] = readl(&dws->regs->dr);  /* dummy read */
	}

	for (i = 0; ((i < tlen) && (buf)); i++) {
		buf[i] = readl(&dws->regs->dr);
	}
}

#define SPI_ARRANGE_LEN(cnt1, sz1, cnt2, sz2)	\
if (cnt1 <= sz1) {				\
	cnt2 = 0;				\
        sz1 -= cnt1;				\
} else {					\
	cnt2 = cnt1 - sz1;			\
	cnt1 -= cnt2;				\
	sz1 = 0;				\
	sz2 -= cnt2;				\
}      						\


int dw_spi_transfer_cs(struct spi_slave *slave, uint8_t *in, int in_len, uint8_t *out, int out_len, unsigned long flags)
{
	int i;
	uint32_t reg;
	int spi_mode;
	int dummy_tx = 0, dummy_rx = 0;
	struct dw_spi_slave *dws = to_dw_spi(slave);
	int cs = 0;
	int tx_count = 0, dtx_count = 0, delta;
	int rx_count = 0, drx_count = 0;
	int ret = OK;
	int tout = 4;

	printf("%s: in=%p in_len=%d out=%p out_len=%d flags = 0x%lx\n", __FUNCTION__, in, in_len, out, out_len, flags);

	/* Get SPI mode */
	reg = readl(&dws->regs->ctrl0);
	spi_mode = ((reg & SPI_TMOD_MASK) >> SPI_TMOD_OFFSET);

	printf("%s: SPY MODE=%d\n", __func__, spi_mode);

	if (((spi_mode == SPI_TMOD_EPROMREAD) || (spi_mode == SPI_TMOD_RO)) && (out_len)) {
		/* Disable device. CTRLR1 should be set when spi_port is disabled. */
		writel(0, &dws->regs->ssien);

		/* Set Number of Data Frames that should be out_len-1 */
		writel((out_len - 1), &dws->regs->ctrl1);

		/* Enable device. */
		writel(SSIENR_SSI_EN, &dws->regs->ssien);
	}

	if (spi_mode != SPI_TMOD_EPROMREAD) {
		dummy_rx = in_len;
		dummy_tx = out_len;
	}

	while ((((in_len + dummy_tx) != 0) || ((out_len + dummy_rx) != 0)) && tout) {

		/* Calculate send bytes. */
		tx_count = SPI_FIFO_SIZE - readl(&dws->regs->txfl);

		printf("%s: tx_count[1]=%d\n", __func__, tx_count);

		reg = readl(&dws->regs->sr); // TX_EMPTY
		if ((cs) && ((in_len + dummy_tx) != 0) && (reg & SPI_SR_TFE)) {
			printf ("%s: remain tx len=%d SR=0x%x\n", __FUNCTION__, (in_len + dummy_tx), reg);
		}

		if (tx_count > (in_len + dummy_tx)) {
			tx_count = (in_len + dummy_tx);
		}
		printf("%s: tx_count[2]=%d\n", __func__, tx_count);

		delta = SPI_FIFO_SIZE - ((out_len + dummy_rx) - (in_len + dummy_tx));
		printf("%s: delta=%d\n", __func__, delta);
		if (tx_count > delta) {
			tx_count = delta;
		}
		printf("%s: tx_count[3]=%d\n", __func__, tx_count);

		SPI_ARRANGE_LEN(tx_count, in_len, dtx_count, dummy_tx);

		/* Write incomming buffer to TX FIFO and the dummy data to shift outcomming data. */
		dw_spi_send(slave, in, tx_count, dtx_count);

		/*
		 * Chip select. Enable slave right after filling TX FIFO to avoid breaking
		 * a SPI transfer.
		 */
		if (!cs) {
			dw_set_cs(slave);
			cs = 1;
		}

		/* Calculate recv bytes. */
		drx_count = readl(&dws->regs->rxfl);
		printf("%s: drx_count[1]=%d\n", __func__, drx_count);

		if (drx_count > (out_len + dummy_rx)) {
			drx_count = (out_len + dummy_rx);
		}
		printf("%s: drx_count[2]=%d\n", __func__, drx_count);

		SPI_ARRANGE_LEN(drx_count, dummy_rx, rx_count, out_len);

		/* Read dummy data and the actual data. */
		dw_spi_recv(slave, out, rx_count, drx_count);

		if (in) {
			in += tx_count;
		}

		if (out) {
			out += rx_count;
		}
		printf("%s: timeout=%d  (in_len + dummy_tx)=%d   (out_len + dummy_rx)=%d\n", __func__, tout, (in_len + dummy_tx), (out_len + dummy_rx));
		tout--;
	} // while

	if (!tout) {
		printf("%s: TIMEOUT!!!\n", __func__);
		return -1;
	}

	for (i = 0; i < MAX_SPI_TRIES; i++) {
		/* Get status register */
		reg = readl(&dws->regs->sr);

		if ((reg & SPI_SR_TFE) && ((reg & SPI_SR_BUSY) == 0)) {
			break;
		}
	}

	if (i == MAX_SPI_TRIES) {
		printf("%s: transer is not completed SR=0x%x\n", __FUNCTION__, reg);
		ret = ERROR;
	}

	/* Clear chip select. */
	dw_clear_cs(slave);

	return ret;
}

int spi_xfer(struct spi_slave *slave, unsigned int bitlen, const void *dout,
	     void *din, unsigned long flags)
{
	int rlen = bitlen >> 3;

	struct dw_spi_slave *dws = to_dw_spi(slave);

	printf("%s: bitlen=%d dout=0x%x din=0x%x flags=0x%lx\n", __FUNCTION__, bitlen, (int)dout, (int)din, flags);

	if ((bitlen == 0) && (flags & SPI_XFER_END)) {
		/* Just return, clear cs. */
		return OK;
	}

	if (flags & SPI_XFER_BEGIN) {
		dws->in_len = 0;
		dws->out_len = 0;
		dws->out = 0;
	}

	if (dout) {
		if ((dws->in_len + rlen) < SPI_IN_BUF_SIZE) {
			memcpy((char *)&dws->in[0] + dws->in_len, dout, rlen);
			dws->in_len += rlen;
		} else {
			printf("Cannot store incomming data\n");
			return ERROR;
		}
	}

	if (din) {
		dws->out = din;
		dws->out_len = rlen;
	}

	if ((flags & SPI_XFER_END) || (flags == 0)) {
		return dw_spi_transfer_cs(slave, (uint8_t *)dws->in, dws->in_len,
						(uint8_t *)dws->out, dws->out_len, flags);
	}

	return 0;
}

#if 0
int spi_xfer_prev(struct spi_slave *slave, unsigned int bitlen, const void *dout,
	     void *din, unsigned long flags)
{
	struct dw_spi_slave *dws = to_dw_spi(slave);
	unsigned int i;
	uchar data;

	/* Internal pointers */
//	const uchar *txptr = dout;
//	uchar *rxptr = din;
	const uchar _txptr[] = {0x9f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	uchar _rxptr[16];

	const uchar *txptr = &_txptr[0];
	uchar *rxptr = &_rxptr[0];


	/* Assume spi core configured to do 8 bit transfers */
	uint bytes = bitlen / sizeof(uchar) / 8;
	bytes = 7;

	/* Activate chip select on start */
	if (flags & SPI_XFER_BEGIN)
//		spi_cs_activate(slave);
		writel(SPI_ENABLE, &dws->regs->ssien);

	/* Transmit / receive count */
	uint rx_count = bytes;
	uint tx_count = bytes;

	printf("%s: bus:%i cs:%i bitlen:%i bytes:%i flags:%lx\n", __func__,
		slave->bus, slave->cs, bitlen, bytes, flags);

	/* Nothing to transfer */
	if (bitlen == 0)
		goto done;

	/* Not aligned to 8 bit transfer frame */
	if (bitlen % 8) {
		flags |= SPI_XFER_END;
		goto done;
	}

	data = (dws->mode & SPI_TMOD_MASK) >> SPI_TMOD_OFFSET;

	if (((data == SPI_TMOD_RO) || (data == SPI_TMOD_EPROMREAD)) && din) {
		/* Set rx count */
		writel(SPI_DISABLE, &dws->regs->ssien);
		writel(bytes, &dws->regs->ctrl1);
		writel(SPI_ENABLE, &dws->regs->ssien);
	}

	printf("%s: filling fifo\n", __func__);
	for (i = 0; i < dws->fifo - 1; i++) {
		if (txptr)
			data = *txptr++;
		else
			data = SPI_DR_IDLE_VAL;
		writel(data, &dws->regs->dr);
		printf("%s: fifo_tx:%x\n", __func__, data);
		/* Decrement counter */
		tx_count--;
		if (!tx_count)
			break;
	}

#if 0
	/* Flush receive buffer fifo */
	while (readl(&dws->regs->sr) & SPI_SR_RFNE) {
		data = readl(&dws->regs->dr);
		printf("%s: missed: %02x\n", __func__, data);
	}
#endif
	/* Transfer data (while not complete) */
	while (rx_count || tx_count) {
		/* There are bytes for transfer and tx fifo is not full */
		if (tx_count && (readl(&dws->regs->sr) & SPI_SR_TFNF)) {
			/* If exists data to send */
			if (txptr)
				data = *txptr++;
			else
				data = SPI_DR_IDLE_VAL;
			writel(data, &dws->regs->dr);
			printf("%s: tx:%x\n", __func__, data);
			/* Decrement counter */
			tx_count--;
			/* Update activity counter */
			count--;
		}

		/* There are bytes is being received and rx fifo is not empty */
		if (rx_count && (readl(&dws->regs->sr) & SPI_SR_RFNE)) {
			data = readl(&dws->regs->dr);
			if (rxptr)
				*rxptr++ = data;
			printf("%s: rx:%x\n", __func__, data);
			/* Decrement counter */
			rx_count--;
			/* Update activity counter */
			count--;
		}
	}

 done:
	if (flags & SPI_XFER_END)
		writel(SPI_DISABLE, &dws->regs->ssien);
	return 0;
}
#endif /* 0/1 */

__attribute__((weak))
int spi_cs_is_valid(unsigned int bus, unsigned int cs)
{
	/* Valid only for registered buses */
	if (bus > ARRAY_SIZE(spi_bus_regs))
		return 0;
	/* Valid only for cs=0..CONFIG_DW_MAX_CS */
	return !!(cs < CONFIG_DW_MAX_CS);
}

__attribute__((weak))
void spi_cs_activate(struct spi_slave *slave)
{
	struct dw_spi_slave *dws = to_dw_spi(slave);
	/* Wait for idle state */
	while (readl(&dws->regs->sr) & SPI_SR_BUSY);
		;
	/* Set bit N for CS_N in SER */
	writel(BIT(slave->cs), &dws->regs->ser);
	printf("%s: cs:%i ser:%u\n", __func__, slave->cs, readl(&dws->regs->ser));
}

__attribute__((weak))
void spi_cs_deactivate(struct spi_slave *slave)
{
	struct dw_spi_slave *dws = to_dw_spi(slave);
	/* Wait for idle state */
	while (readl(&dws->regs->sr) & SPI_SR_BUSY)
		;
	/* Clear Serial Enable Register */
	writel(0, &dws->regs->ser);
	printf("%s: cs:%i ser:%u\n", __func__, slave->cs, readl(&dws->regs->ser));
}
