/*
 * Configuration settings for T-client boards with SFBT1/MSBT2 boards installed.
 *
 * Copyright (C) 2018 T-platforms JSC
 *
 * Authors:
 *   Segey Semin <sergey.semin@t-platforms.ru>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#ifndef __CONFIG_TPLATFORMS_TC_H
#define __CONFIG_TPLATFORMS_TC_H

#include <generated/autoconf.h>

#if defined(CONFIG_TPLATFORMS_TC_SFBT1)
# include <configs/tplatforms_sfbt1.h>
#elif defined(CONFIG_TPLATFORMS_TC_MSBT2)
# include <configs/tplatforms_msbt2.h>
#endif

/*
 *-----------------------------------------------
 * Board common settings
 *-----------------------------------------------
 */
#if defined(CONFIG_TPLATFORMS_TC_SFBT1)
# undef CONFIG_BAIKAL_NAME
# define CONFIG_BAIKAL_NAME		"Tclient-SFBT1"
#elif defined(CONFIG_TPLATFORMS_TC_MSBT2)
# undef CONFIG_BAIKAL_NAME
# define CONFIG_BAIKAL_NAME		"Tclient-MSBT2"
#endif

/*
 *-----------------------------------------------
 * T-client board specific hardware
 *-----------------------------------------------
 */
#define CONFIG_SYS_MUX_MAX7358_BUS_NUM	0
#define CONFIG_SYS_MUX_MAX7358_ADDR	0x70
#define CONFIG_SYS_CTRL0_BUS_NUM	0
#define CONFIG_SYS_CTRL0_I2C_ADDR	0x20
#define CONFIG_SYS_CTRL1_BUS_NUM	0
#define CONFIG_SYS_CTRL1_I2C_ADDR	0x22
#define CONFIG_SYS_SHRED1_BUS_NUM	1
#define CONFIG_SYS_SHRED1_I2C_ADDR	0x21
#define CONFIG_PCF857X
#define CONFIG_CMD_PCF857X
#define CONFIG_CMD_PCF857X_INFO
#define CONFIG_SYS_I2C_PCF857X_BUS_NUM	CONFIG_SYS_SHRED1_BUS_NUM
#define CONFIG_SYS_I2C_PCF857X_ADDR	CONFIG_SYS_SHRED1_I2C_ADDR
#define CONFIG_SYS_I2C_PCF857X_WIDTH \
	{{CONFIG_SYS_I2C_PCF857X_ADDR, 8, CONFIG_SYS_I2C_PCF857X_BUS_NUM}, \
	 {CONFIG_SYS_CTRL0_I2C_ADDR, 8, CONFIG_SYS_CTRL0_BUS_NUM}, \
	 {CONFIG_SYS_CTRL1_I2C_ADDR, 8, CONFIG_SYS_CTRL1_BUS_NUM}}
#define CONFIG_SYS_CTRL0_SSD_RST_PIN	0
#define CONFIG_SYS_CTRL0_STM_RST_PIN	1
#define CONFIG_SYS_CTRL0_MC_RST_PIN	2
#define CONFIG_SYS_CTRL0_USB_RST_PIN	3
#define CONFIG_SYS_CTRL0_SSD_WP_PIN	7
#define CONFIG_SYS_CTRL1_SVGA_RST_PIN	0

/*
 *-----------------------------------------------
 * DW PCIe - SM750 Video chip
 *-----------------------------------------------
 */
#define CONFIG_PCI
#define CONFIG_PCIE_DW
#define CONFIG_PCI_PNP
#define CONFIG_PCI_SCAN_SHOW
#define CONFIG_CMD_PCI
#define CONFIG_CMD_PCI_ENUM
#define CONFIG_VIDEO
#define CONFIG_VIDEO_SM750
#define CONFIG_SM750_CONSOLE

/*
 *-----------------------------------------------
 * U-Boot startup menu
 *-----------------------------------------------
 */
#undef CONFIG_MENU_SHOW
#define CONFIG_MENUKEY			's'
#define CONFIG_AUTOBOOT_KEYED
#define CONFIG_AUTOBOOT_DELAY_STR	"s"
#define CONFIG_AUTOBOOT_PROMPT 		"autoboot in %d seconds\n",bootdelay
/*
 *-----------------------------------------------
 * Undefine some least usefull commands for Video
 *-----------------------------------------------
 */
#undef CONFIG_CMD_ITEST		/* Integer (and string) test	*/
#undef CONFIG_CMD_LICENSE	/* console license display	*/
#undef CONFIG_CMD_LOADB		/* loadb			*/
#undef CONFIG_CMD_LOADS		/* loads			*/
#undef CONFIG_CMD_DNS		/* dns support			*/
#undef CONFIG_CMD_MEMORY	/* md mm nm mw cp cmp crc base loop */
#undef CONFIG_CMD_NFS		/* NFS support			*/
#undef CONFIG_CMD_SETGETDCR	/* DCR support on 4xx		*/
#undef CONFIG_CMD_SOURCE	/* "source" command support	*/
#undef CONFIG_CMD_XIMG		/* Load part of Multi Image	*/
#undef CONFIG_CMD_REGINFO	/* Register dump		*/

/* NOTE T-client board also got EEPROM on bus 1 with address 0x53, which
 * is the same as EEPROM placed on MSBT2. Be careful using it!
 */

#endif /* __CONFIG_TPLATFORMS_TC_H */
