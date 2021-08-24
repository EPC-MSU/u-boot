/*
 * Configuration settings for the Baikal-T based SFBT1 boards.
 *
 * Copyright (C) 2018 T-platforms JSC
 *
 * Authors:
 *   Segey Semin <sergey.semin@t-platforms.ru>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#ifndef __CONFIG_TPLATFORMS_SFBT1_H
#define __CONFIG_TPLATFORMS_SFBT1_H

/*
 *-----------------------------------------------
 * Default Baikal-T1 config macro
 *-----------------------------------------------
 */
#include <configs/baikal_mips.h>

/*
 *-----------------------------------------------
 * Generic Configurations
 *-----------------------------------------------
 */
/* Use board_early_init_r and board_late_init for boards initialization */
#define CONFIG_BOARD_EARLY_INIT_R
#define CONFIG_BOARD_LATE_INIT

/*
 *-----------------------------------------------
 * Init caches, tlb, DDR, cp0, PMU
 *-----------------------------------------------
 */
#define CONFIG_BAIKAL_SPD_ADDRESS       0x50
#define CONFIG_SYS_SPD_BUS_NUM          1
#define CONFIG_CUSTOM_SPD
#define CONFIG_CUSTOM_SPD_FILE		"MT41K512M16"

/*
 *-----------------------------------------------
 * CPU clock settings
 *-----------------------------------------------
 */
#undef CONFIG_BAIKAL_NAME
#undef CONFIG_SYS_CPU_CLOCK_RATE
#define CONFIG_BAIKAL_NAME		"SFBT1"
#define CONFIG_SYS_CPU_CLOCK_MHZ	850
#define CONFIG_SYS_CPU_CLOCK_RATE	(CONFIG_SYS_CPU_CLOCK_MHZ * 1000000)

/*
 *-----------------------------------------------
 * FRU, BMC, SHRED and service options
 *-----------------------------------------------
 */
#define CONFIG_PCA953X
#define CONFIG_CMD_PCA953X
#define CONFIG_CMD_PCA953X_INFO
#define CONFIG_SYS_I2C_PCA953X_BUS_NUM	CONFIG_SYS_SHRED_BUS_NUM
#define CONFIG_SYS_I2C_PCA953X_ADDR	CONFIG_SYS_SHRED_I2C_ADDR
#define CONFIG_SYS_I2C_PCA953X_WIDTH	{{CONFIG_SYS_I2C_PCA953X_ADDR, 16, CONFIG_SYS_I2C_PCA953X_BUS_NUM}}
#define CONFIG_SYS_SHRED_BUS_NUM	1
#define CONFIG_SYS_SHRED_I2C_ADDR	0x20
#define CONFIG_PCIE_RST_PIN		15

/*
 *-----------------------------------------------
 * RTC Temperature sensor
 *-----------------------------------------------
 */
#if 0
#define CONFIG_CMD_DTT
#define CONFIG_SYS_DTT_BUS_NUM		0x01
#define CONFIG_SYS_I2C_DTT_ADDR		0x56
#define CONFIG_DTT_SENSORS		{ 0 }
#define CONFIG_DTT_ABEOZ9S3
#endif

/*
 *-----------------------------------------------
 * Ethernet controller parameters
 *-----------------------------------------------
 */
#define CONFIG_PHY_ADDR			0x03
#define CONFIG_PHY_MICREL

/*
 *-----------------------------------------------
 * EEPROM parameters
 *-----------------------------------------------
 */
#define CONFIG_CMD_EEPROM
#define CONFIG_SYS_EEPROM_WREN
#define CONFIG_SYS_I2C_EEPROM_ADDR_LEN		1
#define CONFIG_SYS_I2C_EEPROM_ADDR		0x50
#define CONFIG_SYS_EEPROM_PAGE_WRITE_DELAY_MS	10
#define CONFIG_SYS_EEPROM_PAGE_WRITE_BITS	3

/*
 *-----------------------------------------------
 * I2C controller parameters
 *-----------------------------------------------
 */
#undef CONFIG_SYS_I2C_SPEED
#define CONFIG_SYS_I2C_SPEED	100000
#define IC_SDA_HOLD		0x10050
#define IC_CLK			125

/*
 *-----------------------------------------------
 * Board RTC parameters
 *-----------------------------------------------
 */
#if 0
#define CONFIG_CMD_DATE
#define CONFIG_RTC_ABEOZ9
#define CONFIG_SYS_RTC_BUS_NUM  	0x01
#define CONFIG_SYS_I2C_RTC_ADDR 	0x56
#endif

/*
 *-----------------------------------------------
 * FLASH driver setup
 *-----------------------------------------------
 */
/* No memory-mapped flash at now */
#define CONFIG_SYS_NO_FLASH
/* Use SPI flash */
#define CONFIG_SPI_FLASH
/* SPI flash banked */
#define CONFIG_SPI_FLASH_BAR
/* SPI framework command */
#define CONFIG_CMD_SF
#define CONFIG_CMD_SF_TEST

/*
 *-----------------------------------------------
 * FLASH and environment organization
 *-----------------------------------------------
 */
/* Use llenv32 implementation of saveenv */
#define CONFIG_LLENV32_SAVEENV
#define CONFIG_ENV_IS_IN_FLASH
#define CONFIG_ENV_OFFSET		0x000E0000
#define CONFIG_ENV_SIZE			0x00010000 /* 64 KB */
#define CONFIG_ENV_ADDR			(CONFIG_SYS_FLASH_BASE + CONFIG_ENV_OFFSET)
#define CONFIG_ENV_SPI_BUS		CONFIG_BOOT_SPI_BUS
#define CONFIG_ENV_SPI_CS		CONFIG_BOOT_SPI_CS
#define CONFIG_ENV_SPI_MAX_HZ		CONFIG_BOOT_SPI_FREQ
#define CONFIG_ENV_SECT_SIZE		(CONFIG_ENV_SIZE)
#define CONFIG_ENV_SPI_MODE		SPI_MODE_3

/*
 *-----------------------------------------------
 * Filesystem support
 *-----------------------------------------------
 */
#define CONFIG_FS_EXT2
#define CONFIG_FS_EXT3
#define CONFIG_FS_EXT4
#define CONFIG_EXT4_WRITE
#define CONFIG_FS_FAT
#define CONFIG_FAT_WRITE
#define CONFIG_DOS_PARTITION
#define CONFIG_PARTITION_UUIDS
#define CONFIG_CMD_EXT4_WRITE
#define CONFIG_CMD_FS_GENERIC

/*
 *-----------------------------------------------
 * U-Boot startup menu
 *-----------------------------------------------
 */
#define CONFIG_BOOTDELAY 3
#define CONFIG_CMD_BOOTMENU
#define CONFIG_MENU
#define CONFIG_MENU_SHOW
#define CONFIG_MENU_HEADER "--== " CONFIG_BAIKAL_NAME " boot menu ==--"

/*
 *-----------------------------------------------
 * Default commands
 *-----------------------------------------------
 */
#define CONFIG_CMD_BDI		/* bdinfo			*/
#define CONFIG_CMD_BOOTD	/* bootd			*/
#define CONFIG_CMD_BOOTNR	/* bootnr 			*/
#define CONFIG_CMD_CONSOLE	/* coninfo			*/
#define CONFIG_CMD_ECHO		/* echo arguments		*/
#define CONFIG_CMD_EDITENV	/* editenv			*/
#define CONFIG_CMD_IMI		/* iminfo			*/
#define CONFIG_CMD_ITEST	/* Integer (and string) test	*/
#define CONFIG_CMD_LICENSE	/* console license display	*/
#define CONFIG_CMD_LOADB	/* loadb			*/
#define CONFIG_CMD_LOADS	/* loads			*/
#define CONFIG_CMD_PING		/* ping cmd			*/
#define CONFIG_CMD_RARP		/* rarp cmd			*/
#define CONFIG_CMD_DNS		/* dns support			*/
#define CONFIG_CMD_MEMORY	/* md mm nm mw cp cmp crc base loop */
#define CONFIG_CMD_MISC		/* Misc functions like sleep etc*/
#define CONFIG_CMD_RUN		/* run command in env variable	*/
#define CONFIG_CMD_NFS		/* NFS support			*/
#define CONFIG_CMD_SAVEENV	/* saveenv			*/
#define CONFIG_CMD_SETGETDCR	/* DCR support on 4xx		*/
#define CONFIG_CMD_SOURCE	/* "source" command support	*/
#define CONFIG_CMD_XIMG		/* Load part of Multi Image	*/
#define CONFIG_CMD_DHCP		/* dhcp client */
#define CONFIG_CMD_EXT2		/* EXT2 Support			*/
#define CONFIG_CMD_EXT3		/* EXT3 Support			*/
#define CONFIG_CMD_EXT4		/* EXT4 Support			*/
#define CONFIG_CMD_FAT 		/* FAT support			*/
#define CONFIG_CMD_REGINFO	/* Register dump		*/
#define CONFIG_CMD_READ		/* Read data from partition	*/
#define CONFIG_CMD_GPIO		/* GPIO manipulation command	*/
/*#define CONFIG_CMD_TIME*/ 	/* Time support commands */
/*#define CONFIG_CMD_GETTIME*/	/* Time support commands */
#define CONFIG_CMD_MTDPARTS 	/* MTD partitions support */
#define CONFIG_MTD_CMDLINE_PARTS
#define CONFIG_MTD_DEVICE
#define CONFIG_MTD_PARTITIONS

/*
 *-----------------------------------------------
 * Default boot parameters
 *-----------------------------------------------
 */
/* U-Boot config information */
#define CONFIG_ENV_VARS_UBOOT_CONFIG
/* Default variables */
#define CONFIG_CLOCKS_IN_MHZ		(CONFIG_SYS_CLK_FREQ / 1000000)
#define CONFIG_PCI_BOOTDELAY		3
#define CONFIG_BOOTARGS			"mem=128M console=ttyS0,mmio32,115200"
#define CONFIG_BOOTCOMMAND 		"sf probe; sf read 0x81000000 0x00400000 0x00400000; bootm 0x81000000"
#define CONFIG_RAMBOOTCOMMAND 		""
#define CONFIG_NFSBOOTCOMMAND		""
#define CONFIG_ETHPRIME			"eth0"
#define CONFIG_ETHADDR 			de:ad:be:ef:01:01
#define CONFIG_ETH1ADDR			de:ad:be:ef:01:02
#define CONFIG_ETH2ADDR			de:ad:be:ef:01:03
#define CONFIG_ETH3ADDR			de:ad:be:ef:01:04
#define CONFIG_ETH4ADDR			de:ad:be:ef:01:05
#define CONFIG_ETH5ADDR			de:ad:be:ef:01:06
#define CONFIG_IPADDR 			192.168.66.226
#define CONFIG_SERVERIP			192.168.66.151
#define CONFIG_GATEWAYIP		192.168.66.1
#define CONFIG_NETMASK			255.255.255.0
#define CONFIG_HOSTNAME			"baikal"
#define CONFIG_ROOTPATH			"/tftpboot/"
#define CONFIG_BOOTFILE			"uImage"
#define CONFIG_LOADADDR 		CONFIG_SYS_LOAD_ADDR

#endif /* __CONFIG_TPLATFORMS_SFBT1_H */
