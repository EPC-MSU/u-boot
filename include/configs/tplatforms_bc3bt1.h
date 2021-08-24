/*
 * Configuration settings for Axitech BC3BT1 with MRBT1 boards installed.
 *
 * Copyright (C) 2018 T-platforms JSC
 *
 * Authors:
 *   Segey Semin <sergey.semin@t-platforms.ru>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#ifndef __CONFIG_TPLATFORMS_BC3BT1_H
#define __CONFIG_TPLATFORMS_BC3BT1_H

#include <generated/autoconf.h>

#include <configs/tplatforms_mrbt1.h>

/*
 *-----------------------------------------------
 * Board common settings
 *-----------------------------------------------
 */
#undef CONFIG_BAIKAL_NAME
#define CONFIG_BAIKAL_NAME		"BC3BT1-MRBT1"

/*
 *-----------------------------------------------
 * Axitech board specific hardware
 *-----------------------------------------------
 */
#define CONFIG_PCA953X
#define CONFIG_CMD_PCA953X
#define CONFIG_CMD_PCA953X_INFO
#define CONFIG_SYS_CTRL_BUS_NUM		0
#define CONFIG_SYS_CTRL_I2C_ADDR	0x22
#define CONFIG_SYS_LEDS_BUS_NUM		0
#define CONFIG_SYS_LEDS_I2C_ADDR	0x23
#define CONFIG_SYS_I2C_PCA953X_WIDTH \
	{{CONFIG_SYS_CTRL_I2C_ADDR, 16, CONFIG_SYS_CTRL_BUS_NUM}, \
	 {CONFIG_SYS_LEDS_I2C_ADDR, 16, CONFIG_SYS_LEDS_BUS_NUM}}
#define CONFIG_SYS_CTRL_SD_RST_PIN	4
#define CONFIG_SYS_CTRL_SSD_RST_PIN	5
#define CONFIG_SYS_CTRL_GSM_RST_PIN	6
#define CONFIG_SYS_CTRL_FTDI_RST_PIN	9
#define CONFIG_SYS_CTRL_SIM_SEL_PIN	14
#define CONFIG_SYS_USB2513_BUS_NUM	0
#define CONFIG_SYS_USB2513_I2C_ADDR	0x2C
#define CONFIG_SYS_USB2513_RST_PIN	23
#define CONFIG_SYS_GSM_PWR_OFF_PIN	22

/*
 *-----------------------------------------------
 * Ethernet controller parameters
 *-----------------------------------------------
 */
#define CONFIG_MSCC_LEDS_ACTIVE_HIGH
#define CONFIG_MSCC_LEDS_DRIVE_LOW_HIGH

/*
 *-----------------------------------------------
 * Board RTC parameters
 *-----------------------------------------------
 */
#define CONFIG_CMD_DATE
#define CONFIG_RTC_PCF2127
#define CONFIG_SYS_RTC_BUS_NUM  0x01
#define CONFIG_SYS_I2C_RTC_ADDR 0x51

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

#endif /* __CONFIG_TPLATFORMS_BC3BT1_H */
