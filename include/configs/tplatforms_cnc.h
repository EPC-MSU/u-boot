/*
 * Configuration settings for CNC boards with SFBT1/MSBT2 boards installed.
 *
 * Copyright (C) 2018 T-platforms JSC
 *
 * Authors:
 *   Segey Semin <sergey.semin@t-platforms.ru>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#ifndef __CONFIG_TPLATFORMS_CNC_H
#define __CONFIG_TPLATFORMS_CNC_H

#include <generated/autoconf.h>

#if defined(CONFIG_TPLATFORMS_CNC_SFBT1)
# include <configs/tplatforms_sfbt1.h>
#elif defined(CONFIG_TPLATFORMS_CNC_MSBT2)
# include <configs/tplatforms_msbt2.h>
#endif

/*
 *-----------------------------------------------
 * Board common settings
 *-----------------------------------------------
 */
#if defined(CONFIG_TPLATFORMS_CNC_SFBT1)
# undef CONFIG_BAIKAL_NAME
# define CONFIG_BAIKAL_NAME		"CNC-SFBT1"
#elif defined(CONFIG_TPLATFORMS_CNC_MSBT2)
# undef CONFIG_BAIKAL_NAME
# define CONFIG_BAIKAL_NAME		"CNC-MSBT2"
#endif

/*
 *-----------------------------------------------
 * CNC boards specific hardware
 *-----------------------------------------------
 */
#define CONFIG_SYS_SHRED1_BUS_NUM	1
#define CONFIG_SYS_SHRED1_I2C_ADDR	0x21
#define CONFIG_PCF857X
#define CONFIG_CMD_PCF857X
#define CONFIG_CMD_PCF857X_INFO
#define CONFIG_SYS_I2C_PCF857X_BUS_NUM	CONFIG_SYS_SHRED1_BUS_NUM
#define CONFIG_SYS_I2C_PCF857X_ADDR	CONFIG_SYS_SHRED1_I2C_ADDR
#define CONFIG_SYS_I2C_PCF857X_WIDTH \
	{{CONFIG_SYS_I2C_PCF857X_ADDR, 8, CONFIG_SYS_I2C_PCF857X_BUS_NUM}}

/* NOTE Some CNC boards also got EEPROM on bus 1 with address 0x51/0x57, which
 * we don't really use at the moment, since they are DNP on new boards.
 */

/*
 *-----------------------------------------------
 * U-Boot startup menu
 *-----------------------------------------------
 */
#undef CONFIG_MENU_SHOW
#define CONFIG_MENUKEY			'm'
#define CONFIG_AUTOBOOT_KEYED
#define CONFIG_AUTOBOOT_DELAY_STR	"m"
#define CONFIG_AUTOBOOT_PROMPT 		"autoboot in %d seconds (press %c to show menu)\n",bootdelay,CONFIG_MENUKEY

#endif /* __CONFIG_TPLATFORMS_CNC_H */
