/*
 * (C) Copyright 2015
 * Dmitry Dunaev <dmitry.dunaev@baikalelectronics.ru>
 *
 * HW Temperature sensor support for Abracon
 * AB-RTCMC-32.768Khz-EOZ9-S3 I2C RTC chip.
 *
 * Detailed datasheet of the chip is available here:
 * http://www.abracon.com/realtimeclock/AB-RTCMC-32.768kHz-EOZ9-S3-Application-Manual.pdf
 *
 * SPDX-License-Identifier:GPL-2.0+
 */

#include <common.h>
#include <i2c.h>
#include <dtt.h>

/* Address guard */
#ifndef CONFIG_SYS_I2C_DTT_ADDR
#define CONFIG_SYS_I2C_DTT_ADDR		0x56
#endif /* CONFIG_SYS_I2C_DDT_ADDR */

#define ABEOZ9S3_ADDR			CONFIG_SYS_I2C_DTT_ADDR
#define ABEOZ9S3_TEMP			0x20
#define ABEOZ9S3_TEMP_MAX_POS		0xC2
#define ABEOZ9S3_TEMP_MAX_NEG		0x3C
#define ABEOZ9S3_EECTRL			0x30
#define ABEOZ9S3_EECTRL_TEMP_MSK	0x03
#define ABEOZ9S3_EECTRL_THP_1S		0x00
#define ABEOZ9S3_EECTRL_THP_16S		0x01
#define ABEOZ9S3_EECTRL_TH_EN		0x00
#define ABEOZ9S3_EECTRL_TH_DIS		0x02
#define ABEOZ9S3_INVALID		0xFF

int dtt_read(int sensor, int reg)
{
	u8 dir = reg;
	u8 data;

	i2c_set_bus_num(CONFIG_SYS_DTT_BUS_NUM);

	if (i2c_read(ABEOZ9S3_ADDR, dir, 1, &data, 1) == -1)
		return -1;
	if (data == ABEOZ9S3_INVALID)
		return -1;

	return data;
}

int dtt_write(int sensor, int reg, int val)
{
	u8 dir = reg;
	u8 data = val;

	i2c_set_bus_num(CONFIG_SYS_DTT_BUS_NUM);

	if (i2c_write(ABEOZ9S3_ADDR, dir, 1, &data, 1) == -1)
		return -1;

	return 0;
}

int dtt_init_one(int sensor)
{
	u8 data;

	/* Set bus bumber */
	i2c_set_bus_num(CONFIG_SYS_DTT_BUS_NUM);
	/* Read control value */
	if (i2c_read(ABEOZ9S3_ADDR, ABEOZ9S3_EECTRL, 1, &data, 1) == -1)
		return -1;
	/* Thermosensor detected */
	printf("ABEOZ9S3 at I2C address 0x%2x\n", ABEOZ9S3_ADDR);
	/* Termosensor control bits reset */
	data &= ~ABEOZ9S3_EECTRL_TEMP_MSK;
	/* Termosensor enable and update every second */
	data |= ABEOZ9S3_EECTRL_THP_1S | ABEOZ9S3_EECTRL_TH_EN;
	/* Set new control value */
	if (dtt_write(0, ABEOZ9S3_EECTRL, data) == -1) {
		puts("Error initialiting ABEOZ9S3\n");
		return -1;
	}
	/* Return success */
	return 0;
}

int dtt_get_temp(int sensor)
{
	int aux;

	aux = dtt_read(0, ABEOZ9S3_TEMP);
	if (aux == -1) {
		puts("DTT temperature read failed\n");
		return -1;
	}
	/* Zero value is mean minus ABEOZ9S3_TEMP_MAX_NEG */
	return (aux - ABEOZ9S3_TEMP_MAX_NEG);
}
