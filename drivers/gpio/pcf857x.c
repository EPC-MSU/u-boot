/*
 * Copyright 2018 T-platforms JSC
 *
 * SPDX-License-Identifier:       GPL-2.0
 */

/*
 * Driver for NXP's 8 and 16 bit I2C gpio expanders pcf8574 and pcf8575
 */
#include <common.h>
#include <i2c.h>
#include <pcf857x.h>

#ifndef CONFIG_SYS_I2C_PCF857X_ADDR
#define CONFIG_SYS_I2C_PCF857X_ADDR	(~0)
#endif

#ifndef CONFIG_SYS_I2C_PCF857X_BUS_NUM
#define CONFIG_SYS_I2C_PCF857X_BUS_NUM	0
#endif

enum {
	PCF857X_CMD_INFO,
	PCF857X_CMD_DEVICE,
	PCF857X_CMD_SETPIN,
};

struct pcf857x_chip_ngpio {
	uint8_t chip;
	uint8_t ngpio;
	uint8_t bus;
};

#ifdef CONFIG_SYS_I2C_PCF857X_WIDTH
static struct pcf857x_chip_ngpio pcf857x_chip_ngpios[] =
    CONFIG_SYS_I2C_PCF857X_WIDTH;

static int pcf857x_ngpio(uint8_t chip)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(pcf857x_chip_ngpios); i++)
		if (pcf857x_chip_ngpios[i].chip == chip)
			return pcf857x_chip_ngpios[i].ngpio;

	return 8;
}

static int pcf857x_bus(uint8_t chip)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(pcf857x_chip_ngpios); i++)
		if (pcf857x_chip_ngpios[i].chip == chip)
			return pcf857x_chip_ngpios[i].bus;

	return 0;
}
#else
static inline int pcf857x_ngpio(uint8_t chip)
{
	return 8;
}

static inline int pcf857x_bus(uint8_t chip)
{
	return CONFIG_SYS_I2C_PCF857X_BUS_NUM;
}
#endif

int pcf857x_set_val(uint8_t chip, uint mask, uint data)
{
	int ngpio, old_bus, rc = -1;
	uint16_t valw;
	uint8_t valb;

	old_bus = i2c_get_bus_num();
	i2c_set_bus_num(pcf857x_bus(chip));

	ngpio = pcf857x_ngpio(chip);
	if (ngpio == 8) {
		rc = i2c_read(chip, 0, 0, &valb, 1);
		if (rc) {
			rc = -1;
			goto err_ret;
		}

		valb &= ~mask;
		valb |= data;

		rc = i2c_write(chip, 0, 0, &valb, 1);
	} else if (ngpio == 16) {
		rc = i2c_read(chip, 0, 0, (u8*)&valw, 2);
		if (rc) {
			rc = -1;
			goto err_ret;
		}

		valw &= ~mask;
		valw |= data;

		rc = i2c_write(chip, 0, 0, (u8*)&valw, 2);
	}

err_ret:
	i2c_set_bus_num(old_bus);

	return rc;
}

int pcf857x_get_val(uint8_t chip)
{
	int ngpio, old_bus, rc = -1;
	uint8_t valb;
	uint16_t valw;
	uint data;

	old_bus = i2c_get_bus_num();
	i2c_set_bus_num(pcf857x_bus(chip));

	ngpio = pcf857x_ngpio(chip);
	if (ngpio == 8) {
		rc = i2c_read(chip, 0, 0, &valb, 1);
		if (!rc)
			data = (int)valb;
		else
			rc = -1;
	} else if (ngpio == 16) {
		rc = i2c_read(chip, 0, 0, (u8*)&valw, 2);
		if (!rc)
			data = (int)valw;
		else
			rc = -1;
	}

	i2c_set_bus_num(old_bus);

	return rc ?: data;
}

#ifdef CONFIG_CMD_PCF857X
#ifdef CONFIG_CMD_PCF857X_INFO
/*
 * Display pcf857x information
 */
static int pcf857x_info(uint8_t chip)
{
	int nr_gpio = pcf857x_ngpio(chip);
	int bus_num = pcf857x_bus(chip);
	int msb = nr_gpio - 1;
	int i, data;

	printf("pcf857x@ %d-0x%x (%d pins):\n\n", bus_num, chip, nr_gpio);

	data = pcf857x_get_val(chip);
	if (data < 0)
		return -1;

	printf("gpio pins: ");
	for (i = msb; i >= 0; i--)
		printf("%X", i);
	printf("\n");
	for (i = 11 + nr_gpio; i > 0; i--)
		printf("-");
	printf("\n");

	printf("i/o %04x:  ", data);
	for (i = msb; i >= 0; i--)
		printf("%c", data & (1 << i) ? '1' : '0');
	printf("\n(1 - weakly pulled up input, 0 - pulled down input or output)\n");

	return 0;
}
#endif /* CONFIG_CMD_PCF857X_INFO */

cmd_tbl_t cmd_pcf857x[] = {
	U_BOOT_CMD_MKENT(device, 3, 0, (void *)PCF857X_CMD_DEVICE, "", ""),
	U_BOOT_CMD_MKENT(output, 4, 0, (void *)PCF857X_CMD_SETPIN, "", ""),
#ifdef CONFIG_CMD_PCF857X_INFO
	U_BOOT_CMD_MKENT(info, 2, 0, (void *)PCF857X_CMD_INFO, "", ""),
#endif
};

int do_pcf857x(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	static uint8_t chip = CONFIG_SYS_I2C_PCF857X_ADDR;
	int ret = CMD_RET_USAGE;
	ulong ul_arg2 = 0;
	ulong ul_arg3 = 0;
	cmd_tbl_t *c;

	c = find_cmd_tbl(argv[1], cmd_pcf857x, ARRAY_SIZE(cmd_pcf857x));

	/* All commands but "device" require 'maxargs' arguments */
	if (!c || !((argc == c->maxargs) ||
		(((int)c->cmd == PCF857X_CMD_DEVICE) &&
		 (argc == (c->maxargs - 1))))) {
		return CMD_RET_USAGE;
	}

	/* arg2 used as chip number or pin number */
	if (argc > 2)
		ul_arg2 = simple_strtoul(argv[2], NULL, 16);

	/* arg3 used as pin or invert value */
	if (argc > 3)
		ul_arg3 = simple_strtoul(argv[3], NULL, 16) & 0x1;

	switch ((int)c->cmd) {
#ifdef CONFIG_CMD_PCF857X_INFO
	case PCF857X_CMD_INFO:
		ret = pcf857x_info(chip);
		if (ret)
			ret = CMD_RET_FAILURE;
		break;
#endif
	case PCF857X_CMD_DEVICE:
		if (argc == 3)
			chip = (uint8_t)ul_arg2;
		printf("Current device bus-address: %d-0x%x\n", pcf857x_bus(chip), chip);
		ret = CMD_RET_SUCCESS;
		break;
	case PCF857X_CMD_SETPIN:
		ret = pcf857x_set_val(chip, (1 << ul_arg2), (ul_arg3 << ul_arg2));
		if (ret)
			ret = CMD_RET_FAILURE;
		break;
	}

	if (ret == CMD_RET_FAILURE)
		eprintf("Error talking to chip at 0x%x\n", chip);

	return ret;
}

U_BOOT_CMD(
	pcf857x,	5,	1,	do_pcf857x,
	"pcf857x gpio access method",
	"device [dev]\n"
	"	- show or set current device address\n"
#ifdef CONFIG_CMD_PCF857X_INFO
	"pcf857x info\n"
	"	- display info for current chip\n"
#endif
	"pcf857x set pin 0|1\n"
	"	- set pin with value (1 - weak pull up, 0 - strong pull down)\n"
);

#endif /* CONFIG_CMD_PCF857X */
