/*
 * File:        oem.c
 * Description: OEM commands for FRU and variables
 * Created:     26 Feb 2015
 * Author:      Dmitry Dunaev <dmitry.dunaev@baikalelectronics.ru>
 * Copyright 2016 Baikal Electronics
 *
 * On BFK board the 24C02 EEPROM is used as FRU device and as
 * storage for several variables.
 */
#include <common.h>
#include <command.h>
#include <i2c.h>
#include <net.h>
#include "oem.h"

/*
 * EEPROM structure
 * ----------------
 * FRU block (offset 0x00):
 *    0x00..0xBF As IPMI 2.0 specification
 *
 * VENDOR block (offset 0xC0):
 *    0xC0..0xC3 Reserved for CRC
 *    0xC4       Board type
 *    0xC5       Board revision
 *    0xC6..0xCD Board Serial Number
 *    0xCE..0xCF CPU Serial Number
 *
 * NET block (offset 0xD0):
 *    0xD0..0xD5 ETH0 MAC address
 *    0xD6..0xDB ETH1 MAC address
 *    0xDC..0xE1 ETH2 MAC address
 *    0xE2..0xE7 ETH3 MAC address
 *
 * MISC block (offset 0xE8):
 *    0xE8..0xFF Reserved
 */

/* Guards */
#ifndef CONFIG_SYS_OEM_BUS_NUM 
#define CONFIG_SYS_OEM_BUS_NUM		0x00
#endif

#ifndef CONFIG_SYS_OEM_I2C_ADDR
#define CONFIG_SYS_OEM_I2C_ADDR		0x50
#endif

#ifndef CONFIG_SYS_OEM_FRU_OFS
#define CONFIG_SYS_OEM_FRU_OFS		0x00
#endif

#ifndef CONFIG_SYS_OEM_VENDOR_OFS
#define CONFIG_SYS_OEM_VENDOR_OFS	0xC0
#endif

#ifndef CONFIG_SYS_OEM_NET_OFS
#define CONFIG_SYS_OEM_NET_OFS		0xD0
#endif

#ifndef CONFIG_SYS_OEM_NET_OFS
#define CONFIG_SYS_OEM_MISC_OFS		0xE8
#endif

#define OEM_FRU_REG(reg)	(reg + CONFIG_SYS_OEM_FRU_OFS)
#define OEM_VENDOR_REG(reg)	(reg + CONFIG_SYS_OEM_VENDOR_OFS)
#define OEM_NET_REG(reg)	(reg + CONFIG_SYS_OEM_NET_OFS)
#define OEM_MISC_REG(reg)	(reg + CONFIG_SYS_MISC_NET_OFS)

/* Chip helpers */
static inline uchar oem_chip_read(uchar reg)
{
	return (i2c_reg_read (CONFIG_SYS_OEM_I2C_ADDR, reg));
}

static inline void oem_chip_write(uchar reg, uchar val)
{
	i2c_reg_write (CONFIG_SYS_OEM_I2C_ADDR, reg, val);
}

static inline void oem_chip_wait(void)
{
	udelay(1000);
}

/* 
 * Read mac address of selected interface from EEPROM.
 * Return 0 on success.
 */
int oem_get_macaddr(unsigned int iface, uchar *enetaddr)
{
	uchar ofs;

	if (i2c_set_bus_num(CONFIG_SYS_OEM_BUS_NUM))
		return -1;

	/* Register offset */
	ofs = OEM_NET_REG(iface * 6);
	
	/* Max offset is (0xe8 - 0x06) */
	if (ofs > 0xe2)
		return -2;

	if (eeprom_read(CONFIG_SYS_OEM_I2C_ADDR, ofs, enetaddr, 6)) {
		printf("Error: EEPROM read failed\n");
		return -3;
	}

	return  0;
}

/* 
 * Write mac address of selected interface to EEPROM.
 * Return 0 on success.
 */
int oem_set_macaddr(unsigned int iface, uchar *enetaddr)
{
	uchar ofs;
	
	if (i2c_set_bus_num(CONFIG_SYS_OEM_BUS_NUM))
		return -1;

	/* Register offset */
	ofs = OEM_NET_REG(iface * 6);
	
	/* Max offset is (0xe8 - 0x06) */
	if (ofs > 0xe2)
		return -2;

	if (eeprom_write(CONFIG_SYS_OEM_I2C_ADDR, ofs, enetaddr, 6)) {
		printf("Error: EEPROM write failed\n");
		return -3;
	}

	return  0;
}

/* Print mac address of selected interface from EEPROM. */
void oem_print_macaddr(unsigned int iface, uchar *enetaddr)
{
	int i;
	char name[8];

	sprintf(name, iface ? "eth%daddr" : "ethaddr", iface);

	printf("%s: ", name);

	for (i = 0; i < 6; ++i)
		printf((i == 0) ? "%02x" : ":%02x", enetaddr[i]);

	printf("\n");
}

/* 
 * Check mac address is valid.
 * Return 0 on success.
 */
int oem_check_macaddr(uchar *enetaddr)
{
	int i;
	int zf = 0;
	int ff = 0;

	for (i = 0; i < 6; ++i) {
		ff += (enetaddr[i] != 0xff) ? 1 : 0;
		zf += (enetaddr[i] != 0x00) ? 1 : 0;
	}

	return (!ff || !zf);
}

/* 
 * Set enviroment variable of mac address from EEPROM.
 * Return 0 on success.
 */
int oem_setenv_macaddr(unsigned int iface)
{
	char name[32];
	uchar enetaddr[6];

	if (oem_get_macaddr(iface, enetaddr))
		return -1;

	if (oem_check_macaddr(enetaddr))
		return -2;

	sprintf(name, iface ? "eth%daddr" : "ethaddr", iface);

	return eth_setenv_enetaddr(name, enetaddr);
}

/* Set enviroment variables of all mac address from EEPROM. */
void oem_setall_macaddr(void)
{
	int i;
	for (i = 0; i < OEM_MAX_IFACES; ++i) 
		oem_setenv_macaddr(i);
}

/* 
 * Read vendor data from EEPROM.
 * Return 0 on success.
 */
int oem_get_vendor_data(struct vendor_data *data)
{
	if (i2c_set_bus_num(CONFIG_SYS_OEM_BUS_NUM))
		return -1;

	if (eeprom_read(CONFIG_SYS_OEM_I2C_ADDR, CONFIG_SYS_OEM_VENDOR_OFS,
				(uchar *)data, sizeof(struct vendor_data))) {
		printf("Error: EEPROM read failed\n");
		return -3;
	}

	return  0;
}

/* 
 * Write vendor data from EEPROM.
 * Return 0 on success.
 */
int oem_set_vendor_data(struct vendor_data *data)
{
	if (i2c_set_bus_num(CONFIG_SYS_OEM_BUS_NUM))
		return -1;

	if (eeprom_write(CONFIG_SYS_OEM_I2C_ADDR, CONFIG_SYS_OEM_VENDOR_OFS,
			(uchar *)data, sizeof(struct vendor_data))) {
		printf("Error: EEPROM write failed\n");
		return -3;
	}

	return  0;
}

/* 
 * Get board ID by name.
 * Return ID number.
 */
int oem_get_vendor_board(char *name)
{
	if (strcmp(CONFIG_BAIKAL_NAME, "BFK") == 0)
		return OEM_VENDOR_BT_BFK;

	return OEM_VENDOR_BT_UNKNOWN;
}

/* End of file oem.c */
