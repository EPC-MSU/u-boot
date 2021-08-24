/*
 * File:        oem.h
 * Description: OEM commands for FRU and variables
 * Created:     26 Feb 2015
 * Author:      Dmitry Dunaev <dmitry.dunaev@baikalelectronics.ru>
 * Copyright 2016 Baikal Electronics
 *
 */

#ifndef _ASM_BAIKAL_OEM_H
#define _ASM_BAIKAL_OEM_H

/* Vendor data interface */
struct vendor_data {
	u32 crc; /* Reserved for CRC */
#define OEM_VENDOR_BT_BFK 		0x00
#define OEM_VENDOR_BT_UNKNOWN 	0xff
	u8 type;		/* Board ID */
	u8 revision;	/* Board revision in BCD format */
	u64 serial;		/* Board Serial number */
	u16 cpu;		/* CPU serial number */
};

extern struct vendor_data vdata;

/* Max ethernet interfaces */
#define OEM_MAX_IFACES 4

/* Read mac address of selected interface from EEPROM. */
int oem_get_macaddr(unsigned int iface, uchar *enetaddr);

/* Write mac address of selected interface to EEPROM. */
int oem_set_macaddr(unsigned int iface, uchar *enetaddr);

/* Print mac address of selected interface */
void oem_print_macaddr(unsigned int iface, uchar *enetaddr);

/* Check mac address is valid. */
int oem_check_macaddr(uchar *enetaddr);

/* Set enviroment variable of mac address from EEPROM. */
int oem_setenv_macaddr(unsigned int iface);

/* Set enviroment variables of all mac address from EEPROM. */
void oem_setall_macaddr(void);

/* Read vendor data from EEPROM. */
int oem_get_vendor_data(struct vendor_data *data);

/* Write vendor data from EEPROM. */
int oem_set_vendor_data(struct vendor_data *data);

/* Get board ID by name. */
int oem_get_vendor_board(char *name);

#endif /* _ASM_BAIKAL_OEM_H */
