/*
 * File:        nvram.c
 * Description: System storage for BFK board
 * Created:     06 Aug 2015
 * Author:      Dmitry Dunaev <dmitry.dunaev@baikalelectronics.ru>
 * Copyright 2015 Baikal Electronics
 *
 * On BFK board the Abracon AB-RTCMC-32.768Khz-EOZ9-S3
 * RTC Clock chip is used as system NVRAM (8 byte SRAM)
 * and as EEPROM for serial number (2 byte EEPROM)
 */

#include <common.h>
#include <command.h>
#include <asm/errno.h>
#include <asm/io.h>
#include <i2c.h>
#include "nvram.h"

/* Control 4 register */
#define CHIP_REG_CTRL          0x03    /* Control 4 register */
#define CHIP_REG_CTRL_V1F      (1 << 2)  /*  */
#define CHIP_REG_CTRL_V2F      (1 << 3)  /*  */
#define CHIP_REG_CTRL_SR       (1 << 4)  /*  */
#define CHIP_REG_CTRL_PON      (1 << 5)  /*  */
#define CHIP_REG_CTRL_EEBUSY   (1 << 7)  /*  */

/* EEPROM user section */
#define CHIP_REG_EEPROM_USR1   0x28    /* User EEPROM data byte 1 register */
#define CHIP_REG_EEPROM_USR2   0x29    /* User EEPROM data byte 2 register */
#define CHIP_REG_EEPROM_TIMOUT 1000

/* EEPROM control section */
#define CHIP_REG_EEPROM_CTL    0x30    /* EEPROM control register */
#define CHIP_REG_EEPROM_THP    (1 << 0)  /*  */
#define CHIP_REG_EEPROM_THE    (1 << 1)  /*  */
#define CHIP_REG_EEPROM_FD0    (1 << 2)  /*  */
#define CHIP_REG_EEPROM_FD1    (1 << 3)  /*  */
#define CHIP_REG_EEPROM_R1K    (1 << 4)  /*  */
#define CHIP_REG_EEPROM_R5K    (1 << 5)  /*  */
#define CHIP_REG_EEPROM_R20K   (1 << 6)  /*  */
#define CHIP_REG_EEPROM_R80K   (1 << 7)  /*  */

/* NVRAM section */
#define CHIP_REG_NVRAM_BEGIN   0x38    /* Start RAM register */
#define CHIP_REG_NVRAM_END     0x3F    /* End RAM register */
#define CHIP_REG_NVRAM_LEN     0x08    /* 8 Byte lenght */

#ifndef CONFIG_SYS_I2C_NVRAM_ADDR
# define CONFIG_SYS_I2C_NVRAM_ADDR	0x56
#endif /* CONFIG_SYS_I2C_RTC_ADDR */

DECLARE_GLOBAL_DATA_PTR;

/* Chip helpers */
static inline uchar chip_read(uchar reg)
{
	return (i2c_reg_read (CONFIG_SYS_I2C_NVRAM_ADDR, reg));
}

static inline void chip_write(uchar reg, uchar val)
{
	i2c_reg_write (CONFIG_SYS_I2C_NVRAM_ADDR, reg, val);
}

static inline void chip_wait(void)
{
	int tout = CHIP_REG_EEPROM_TIMOUT;

	while ((chip_read(CHIP_REG_CTRL) & CHIP_REG_CTRL_EEBUSY) && tout) {
		udelay(1000);
		tout--;
	}
}

int nvram_get_size(void)
{
	return CHIP_REG_NVRAM_LEN;
}

int nvram_init(void)
{
	puts("NVRAM: ");

	i2c_set_bus_num(CONFIG_SYS_NVRAM_BUS_NUM);

	if (i2c_probe(CONFIG_SYS_I2C_NVRAM_ADDR)) {
		puts("failed\n");
		return -ENODEV;
	}

	puts("ready\n");

	return 0;
}

uchar nvram_read(u8 ofs)
{
	ofs = ofs % CHIP_REG_NVRAM_LEN;
	i2c_set_bus_num(CONFIG_SYS_NVRAM_BUS_NUM);
	return chip_read(CHIP_REG_NVRAM_BEGIN + ofs);
}

void nvram_write(u8 ofs, uchar val)
{
	ofs = ofs % CHIP_REG_NVRAM_LEN;
	i2c_set_bus_num(CONFIG_SYS_NVRAM_BUS_NUM);
	chip_write(CHIP_REG_NVRAM_BEGIN + ofs, val);
}

void nvram_read_block(u8 *buf, u8 size)
{
	u8 i;
	for (i=0; i < size; i++)
		buf[i] = nvram_read(i);
}

void nvram_write_block(u8 *buf, u8 size)
{
	u8 i;
	for (i=0; i < size; i++)
		nvram_write(i, buf[i]);
}

u16 nvram_read_eeprom(void)
{
	u16 val=0;
	i2c_set_bus_num(CONFIG_SYS_NVRAM_BUS_NUM);

	chip_wait();
	val = chip_read(CHIP_REG_EEPROM_USR1) << 8;

	chip_wait();
	val |= chip_read(CHIP_REG_EEPROM_USR2);

	return val;
}

void nvram_write_eeprom(u16 val)
{
	i2c_set_bus_num(CONFIG_SYS_NVRAM_BUS_NUM);

	chip_wait();
	chip_write(CHIP_REG_EEPROM_USR1, val >> 8);

	chip_wait();
	chip_write(CHIP_REG_EEPROM_USR2, val & 0xff);
}

/* End of file nvram.c */


