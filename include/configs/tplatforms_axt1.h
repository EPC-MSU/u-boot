/*
 * Configuration settings for Axitech boards with SFBT1/MSBT2 boards installed.
 *
 * Copyright (C) 2018 T-platforms JSC
 *
 * Authors:
 *   Segey Semin <sergey.semin@t-platforms.ru>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#ifndef __CONFIG_TPLATFORMS_AXT1_H
#define __CONFIG_TPLATFORMS_AXT1_H

#include <generated/autoconf.h>

#if defined(CONFIG_TPLATFORMS_AXT1_SFBT1)
# include <configs/tplatforms_sfbt1.h>
#elif defined(CONFIG_TPLATFORMS_AXT1_MSBT2)
# include <configs/tplatforms_msbt2.h>
#endif

/*
 *-----------------------------------------------
 * Board common settings
 *-----------------------------------------------
 */
#if defined(CONFIG_TPLATFORMS_AXT1_SFBT1)
# undef CONFIG_BAIKAL_NAME
# define CONFIG_BAIKAL_NAME		"AXT1-SFBT1"
#elif defined(CONFIG_TPLATFORMS_AXT1_MSBT2)
# undef CONFIG_BAIKAL_NAME
# define CONFIG_BAIKAL_NAME		"AXT1-MSBT2"
#endif

/*
 *-----------------------------------------------
 * Axitech board specific hardware
 *-----------------------------------------------
 */
#define CONFIG_SYS_CTRL_BUS_NUM		0
#define CONFIG_SYS_CTRL_I2C_ADDR	0x22
#define CONFIG_SYS_LEDS_BUS_NUM		0
#define CONFIG_SYS_LEDS_I2C_ADDR	0x23
#undef CONFIG_SYS_I2C_PCA953X_WIDTH
#define CONFIG_SYS_I2C_PCA953X_WIDTH \
	{{CONFIG_SYS_I2C_PCA953X_ADDR, 16, CONFIG_SYS_I2C_PCA953X_BUS_NUM}, \
	 {CONFIG_SYS_CTRL_I2C_ADDR, 16, CONFIG_SYS_CTRL_BUS_NUM}, \
	 {CONFIG_SYS_LEDS_I2C_ADDR, 16, CONFIG_SYS_LEDS_BUS_NUM}}
#define CONFIG_SYS_CTRL_SD_RST_PIN	5
#define CONFIG_SYS_CTRL_SSD_RST_PIN	6
#define CONFIG_SYS_CTRL_GSM_RST_PIN	7
#define CONFIG_SYS_CTRL_RSISO_RST_PIN	12
#define CONFIG_SYS_CTRL_USBHUB_RST_PIN	15 /* DON'T TOUCH IT!!! */

#endif /* __CONFIG_TPLATFORMS_AXT1_H */
