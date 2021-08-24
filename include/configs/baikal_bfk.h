/*
 * Configuration settings for the Baikal T BFK boards.
 *
 * Copyright (C) 2018 Baikal Electronics JSC
 *
 * Authors:
 *   Alexey Malahov <alexey.malahov@baikalelectronics.ru>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#ifndef __CONFIG_BAIKAL_BFK_H
#define __CONFIG_BAIKAL_BFK_H

//#define DEBUG

/* Use default values from the main file */
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
#if defined(CONFIG_BAIKAL_BFK)		/* BFK board */
# define CONFIG_CUSTOM_SPD
# define CONFIG_CUSTOM_SPD_FILE		"MT41K1G8"
#elif defined(CONFIG_BAIKAL_BFKX)	/* BFKX board (BFK1.6+Baikal-T1) */
# define CONFIG_CUSTOM_SPD
# define CONFIG_CUSTOM_SPD_FILE		"MT41K1G8"
#elif defined(CONFIG_BAIKAL_BFK3)		/* BFK3 board */
# define CONFIG_SYS_SPD_BUS_NUM		1
# define CONFIG_BAIKAL_SPD_ADDRESS	0x50
#endif

/*
 *-----------------------------------------------
 * CPU clock settings
 *-----------------------------------------------
 */
#undef CONFIG_BAIKAL_NAME
#undef CONFIG_SYS_CPU_CLOCK_RATE
#if defined(CONFIG_BAIKAL_QEMU)		/* QEMU simulator */
# define CONFIG_BAIKAL_NAME		"QEMU"
# define CONFIG_SYS_CPU_CLOCK_RATE	100000000	/* 1000 MHz */
#elif defined(CONFIG_BAIKAL_FPGA)	/* FPGA board */
# define CONFIG_BAIKAL_NAME		"HAPS_FPGA"
# define CONFIG_SYS_CPU_CLOCK_RATE	3838383
#elif defined(CONFIG_BAIKAL_BFK)	/* BFK board */
# define CONFIG_BAIKAL_NAME		"BFK"
# define CONFIG_SYS_CPU_CLOCK_MHZ	850
# define CONFIG_SYS_CPU_CLOCK_RATE	(CONFIG_SYS_CPU_CLOCK_MHZ * 1000000)
#elif defined(CONFIG_BAIKAL_BFKX)	/* BFKX board (BFK1.6+Baikal-T1) */
# define CONFIG_BAIKAL_NAME		"BFKX"
# define CONFIG_SYS_CPU_CLOCK_MHZ	1200
# define CONFIG_SYS_CPU_CLOCK_RATE	(CONFIG_SYS_CPU_CLOCK_MHZ * 1000000)
#elif defined(CONFIG_BAIKAL_BFK3)	/* BFK3 board */
# define CONFIG_BAIKAL_NAME		"BFK3"
# define CONFIG_SYS_CPU_CLOCK_MHZ	1200
# define CONFIG_SYS_CPU_CLOCK_RATE	(CONFIG_SYS_CPU_CLOCK_MHZ * 1000000)
#endif

/*
 *-----------------------------------------------
 * OEM, FRU, NVRAM and service options
 *-----------------------------------------------
 */
#define CONFIG_BFK_CMD_SERVICE
#define CONFIG_SYS_NVRAM_BUS_NUM	0x01
#define CONFIG_SYS_I2C_NVRAM_ADDR	0x56
#define CONFIG_SYS_OEM_BUS_NUM		0x01
#define CONFIG_SYS_OEM_I2C_ADDR		0x51
#define CONFIG_SYS_OEM_FRU_OFS		0x00
#define CONFIG_SYS_OEM_VENDOR_OFS	0xC0
#define CONFIG_SYS_OEM_NET_OFS		0xD0
#define CONFIG_SYS_OEM_MISC_OFS		0xE8

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
/* Baikal Electronics' MAC address range: 4C:A5:15:x:x:x */
#define CONFIG_BAIKAL_MAC_BASE		0x4CA515000000ULL
#define CONFIG_PHY_ADDR			0x03
#define CONFIG_PHY_MICREL

/*
 *-----------------------------------------------
 * EEPROM parameters
 *-----------------------------------------------
 */
#define CONFIG_CMD_EEPROM
#define CONFIG_SYS_I2C_MULTI_EEPROMS
#define CONFIG_SYS_EEPROM_WREN
#define CONFIG_ENV_EEPROM_IS_ON_I2C
#define CONFIG_SYS_I2C_EEPROM_ADDR_LEN		1
#define CONFIG_SYS_I2C_EEPROM_ADDR		0x51
#define CONFIG_SYS_EEPROM_PAGE_WRITE_DELAY_MS	10
#define CONFIG_SYS_EEPROM_PAGE_WRITE_BITS	3

/*
 *-----------------------------------------------
 * Board RTC parameters
 *-----------------------------------------------
 */
#if 0
#define CONFIG_CMD_DATE
#define CONFIG_RTC_ABEOZ9
#define CONFIG_SYS_RTC_BUS_NUM  0x01
#define CONFIG_SYS_I2C_RTC_ADDR 0x56
#endif

/*
 *-----------------------------------------------
 * MMC/SD card support over SPI
 *-----------------------------------------------
 */
#if 0
#define CONFIG_GENERIC_MMC
#define CONFIG_MMC_SPI
#define CONFIG_CMD_MMC
#define CONFIG_CMD_MMC_SPI
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
/* Support for Atmel SPI Flash */
#define CONFIG_SPI_FLASH_ATMEL
/* Support for Macronix SPI Flash */
#define CONFIG_SPI_FLASH_MACRONIX
/* Support for Spanison SPI Flash */
#define CONFIG_SPI_FLASH_SPANSION
/* Support for SST SPI Flash */
#define CONFIG_SPI_FLASH_SST
/* Support for STMicro SPI Flash */
#define CONFIG_SPI_FLASH_STMICRO
/* Support for Winbond SPI Flash */
#define CONFIG_SPI_FLASH_WINBOND

/*
 *-----------------------------------------------
 * FLASH and environment organization
 *-----------------------------------------------
 */
/* Use llenv32 implementation of saveenv */
#define CONFIG_LLENV32_SAVEENV
/*#define CONFIG_ENV_OVERWRITE	1*/
/* Enviroment in flash */
#define CONFIG_ENV_IS_IN_FLASH
/* Environment starts here  */
#define CONFIG_ENV_OFFSET		0x00080000
#define CONFIG_ENV_ADDR			(CONFIG_SYS_FLASH_BASE + CONFIG_ENV_OFFSET)
/* Enviroment block size */
#define CONFIG_ENV_SIZE			0x00010000 /* 64 KB */
/* Select SPI Bus controller */
#define CONFIG_ENV_SPI_BUS		CONFIG_BOOT_SPI_BUS
/* Select SPI chip */
#define CONFIG_ENV_SPI_CS		CONFIG_BOOT_SPI_CS
/* Select chip max frequency */
#define CONFIG_ENV_SPI_MAX_HZ		CONFIG_BOOT_SPI_FREQ
/* Enviroment sector size */
#define CONFIG_ENV_SECT_SIZE		(CONFIG_ENV_SIZE)
/* SPI device signal mode */
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
#if defined(CONFIG_BAIKAL_BFK) || defined(CONFIG_BAIKAL_BFK3) || defined(CONFIG_BAIKAL_BFKX)
#define CONFIG_CMD_BOOTMENU
#define CONFIG_MENU
#define CONFIG_MENU_SHOW
#define CONFIG_AUTOBOOT_KEYED
#define CONFIG_BOOTDELAY	3
#define CONFIG_MENU_HEADER	"--== " CONFIG_BAIKAL_NAME " boot menu ==--"
#else
#define CONFIG_BOOTDELAY	3
#endif

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
/*#define CONFIG_CMD_FPGA*/		/* FPGA configuration Support	*/
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
/*#define CONFIG_CMD_CRAMFS*/	/* CramFS Support (obsolete) */
#define CONFIG_CMD_REGINFO	/* Register dump		*/
#define CONFIG_CMD_READ		/* Read data from partition	*/
#define CONFIG_CMD_GPIO		/* GPIO manipulation command	*/
/*#define CONFIG_CMD_TIME*/ 	/* Time suppoer commands */
/*#define CONFIG_CMD_GETTIME*/	/* Time suppoer commands */
#define CONFIG_CMD_MTDPARTS	/* MTD partitions support */
#define CONFIG_MTD_CMDLINE_PARTS
#define CONFIG_MTD_DEVICE
#define CONFIG_BOOTSTAGE_FDT
#define CONFIG_BOOTSTAGE_REPORT
#define CONFIG_MENU		/* User-frendly menu style */

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

#endif /* __CONFIG_BAIKAL_BFK_H */
