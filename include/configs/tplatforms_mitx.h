/*
 * Configuration settings for the Baikal-T1 based MITX boards.
 *
 * Copyright (C) 2018 T-platforms JSC
 *
 * Authors:
 *   Segey Semin <sergey.semin@t-platforms.ru>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#ifndef __CONFIG_TPLATFORMS_MITX_H
#define __CONFIG_TPLATFORMS_MITX_H

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
#define CONFIG_BAIKAL_SPD_ADDRESS	0x50
#define CONFIG_SYS_SPD_BUS_NUM 		1

/*
 *-----------------------------------------------
 * CPU clock settings
 *-----------------------------------------------
 */
#undef CONFIG_BAIKAL_NAME
#undef CONFIG_SYS_CPU_CLOCK_RATE
#define CONFIG_BAIKAL_NAME		"mITX"
#define CONFIG_SYS_CPU_CLOCK_MHZ	1500
#define CONFIG_SYS_CPU_CLOCK_RATE	(CONFIG_SYS_CPU_CLOCK_MHZ * 1000000)

/*
 *-----------------------------------------------
 * FRU, BMC, SHRED and service options
 *-----------------------------------------------
 */
#define CONFIG_CMD_SERVICE
#define CONFIG_PCF857X
#define CONFIG_CMD_PCF857X
#define CONFIG_CMD_PCA857X_INFO
#define CONFIG_SYS_I2C_PCF857X_BUS_NUM	CONFIG_SYS_SHRED_BUS_NUM
#define CONFIG_SYS_I2C_PCF857X_ADDR	CONFIG_SYS_SHRED_I2C_ADDR
#define CONFIG_SYS_I2C_PCF857X_WIDTH	{{CONFIG_SYS_I2C_PCF857X_ADDR, 8, CONFIG_SYS_I2C_PCF857X_BUS_NUM}}
#define CONFIG_SYS_SHRED_BUS_NUM	1
#define CONFIG_SYS_SHRED_I2C_ADDR	0x21
#define CONFIG_SYS_BMC_BUS_NUM		1
#define CONFIG_SYS_BMC_I2C_ADDR		0x08
#define CONFIG_SYS_FRU_BUS_NUM		1
#define CONFIG_SYS_FRU_I2C_ADDR		0x53
#define CONFIG_FRU_MAX_IFACES		1
#define CONFIG_USB_RST_PIN		0x0D
#define CONFIG_PCIE_RST_PIN		0x0C

/*
 *-----------------------------------------------
 * Ethernet controller parameters
 *-----------------------------------------------
 */
#undef CONFIG_DESIGNWARE_ETH1_BASE
#define CONFIG_PHY_REALTEK
#define CONFIG_PHY_ADDR			0x01
#define CONFIG_PHY_RESET_DELAY		15000

/*
 *-----------------------------------------------
 * EEPROM parameters
 *-----------------------------------------------
 */
#define CONFIG_CMD_EEPROM
#define CONFIG_SYS_I2C_MULTI_EEPROMS
#define CONFIG_SYS_EEPROM_WREN
#define CONFIG_SYS_I2C_EEPROM_ADDR_LEN		2
#define CONFIG_SYS_I2C_EEPROM_ADDR		0x53
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
#define CONFIG_CMD_DATE
#define CONFIG_RTC_PCF2127
#define CONFIG_SYS_RTC_BUS_NUM  0x01
#define CONFIG_SYS_I2C_RTC_ADDR 0x51

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
 * Filesystem support
 *-----------------------------------------------
 */
//#define CONFIG_FS_EXT2
//#define CONFIG_FS_EXT3
#define CONFIG_FS_EXT4
//#define CONFIG_EXT4_WRITE
//#define CONFIG_FS_FAT
//#define CONFIG_FAT_WRITE
#define CONFIG_DOS_PARTITION
#define CONFIG_PARTITION_UUIDS
//#define CONFIG_CMD_EXT4_WRITE
#define CONFIG_CMD_FS_GENERIC

/*
 *-----------------------------------------------
 * U-Boot startup menu
 *-----------------------------------------------
 */
#define CONFIG_BOOTDELAY		3
#define CONFIG_CMD_BOOTMENU
#define CONFIG_MENU
#define CONFIG_MENUKEY			's'
#define CONFIG_AUTOBOOT_KEYED
#define CONFIG_AUTOBOOT_DELAY_STR	"s"
#define CONFIG_AUTOBOOT_PROMPT 		"autoboot in %d seconds\n",bootdelay
#define CONFIG_MENU_HEADER "--== " CONFIG_BAIKAL_NAME " boot menu ==--"

/*
 *-----------------------------------------------
 * Default commands
 *-----------------------------------------------
 */
//#define CONFIG_CMD_BDI		/* bdinfo			*/
#define CONFIG_CMD_BOOTD	/* bootd			*/
#define CONFIG_CMD_BOOTNR	/* bootnr 			*/
#define CONFIG_CMD_CONSOLE	/* coninfo			*/
#define CONFIG_CMD_ECHO		/* echo arguments		*/
#define CONFIG_CMD_EDITENV	/* editenv			*/
//#define CONFIG_CMD_FPGA		/* FPGA configuration Support	*/
//#define CONFIG_CMD_IMI		/* iminfo			*/
//#define CONFIG_CMD_ITEST	/* Integer (and string) test	*/
//#define CONFIG_CMD_LICENSE	/* console license display	*/
//#define CONFIG_CMD_LOADB	/* loadb			*/
//#define CONFIG_CMD_LOADS	/* loads			*/
#define CONFIG_CMD_PING		/* ping cmd			*/
#define CONFIG_CMD_TFTP		/* tftp cmd			*/
#define CONFIG_CMD_RARP		/* rarp cmd			*/
//#define CONFIG_CMD_DNS		/* dns support			*/
//#define CONFIG_CMD_MEMORY	/* md mm nm mw cp cmp crc base loop */
#define CONFIG_CMD_MISC		/* Misc functions like sleep etc*/
#define CONFIG_CMD_RUN		/* run command in env variable	*/
//#define CONFIG_CMD_NFS		/* NFS support			*/
#define CONFIG_CMD_SAVEENV	/* saveenv			*/
//#define CONFIG_CMD_SETGETDCR	/* DCR support on 4xx		*/
//#define CONFIG_CMD_SOURCE	/* "source" command support	*/
//#define CONFIG_CMD_XIMG		/* Load part of Multi Image	*/
#define CONFIG_CMD_DHCP		/* dhcp client */
//#define CONFIG_CMD_EXT2		/* EXT2 Support			*/
//#define CONFIG_CMD_EXT3		/* EXT3 Support			*/
#define CONFIG_CMD_EXT4		/* EXT4 Support			*/
//#define CONFIG_CMD_FAT 		/* FAT support			*/
//#define CONFIG_CMD_CRAMFS	/* CramFS Support (obsolete) */
//#define CONFIG_CMD_REGINFO	/* Register dump		*/
#define CONFIG_CMD_READ		/* Read data from partition	*/
#define CONFIG_CMD_GPIO		/* GPIO manipulation command	*/

//#define CONFIG_BOOTSTAGE_FDT
//#define CONFIG_BOOTSTAGE_REPORT

//#define CONFIG_CMD_TIME 	/* Time suppoer commands */
//#define CONFIG_CMD_GETTIME	/* Time suppoer commands */
//#define CONFIG_CMD_MTDPARTS /* MTD partitions support */
//#define CONFIG_MTD_CMDLINE_PARTS
//#define CONFIG_MTD_DEVICE
//#define CONFIG_MENU			/* User-frendly menu style */

/*
 *-----------------------------------------------
 * Default boot parameters
 *-----------------------------------------------
 */
/* U-Boot config inforation */
#define CONFIG_ENV_VARS_UBOOT_CONFIG
/* Default variables */
#define CONFIG_CLOCKS_IN_MHZ	(CONFIG_SYS_CLK_FREQ / 1000000)
#define CONFIG_PCI_BOOTDELAY	3
#define CONFIG_BOOTARGS			"mem=128M console=ttyS0,mmio32,115200"
#define CONFIG_BOOTCOMMAND 		"sf probe; sf read 0x81000000 0x00400000 0x00400000; bootm 0x81000000"
#define CONFIG_RAMBOOTCOMMAND 	""
#define CONFIG_NFSBOOTCOMMAND	""
#define CONFIG_ETHPRIME			"eth0"
#define CONFIG_ETHADDR 			de:ad:be:ef:01:01
#define CONFIG_ETH0ADDR			de:ad:be:ef:01:02
#define CONFIG_ETH1ADDR			de:ad:be:ef:01:03
#define CONFIG_ETH2ADDR			de:ad:be:ef:01:04
#define CONFIG_ETH3ADDR			de:ad:be:ef:01:05
#define CONFIG_ETH4ADDR			de:ad:be:ef:01:06
#define CONFIG_ETH5ADDR			de:ad:be:ef:01:07
#define CONFIG_IPADDR 			192.168.66.226
#define CONFIG_SERVERIP			192.168.66.151
#define CONFIG_GATEWAYIP		192.168.66.1
#define CONFIG_NETMASK			255.255.255.0
#define CONFIG_HOSTNAME			"baikal"
#define CONFIG_ROOTPATH			"/tftpboot/"
#define CONFIG_BOOTFILE			"uImage"
#define CONFIG_LOADADDR 		CONFIG_SYS_LOAD_ADDR

/* MMC-SPI support */
#undef CONFIG_DW_SPI
#define CONFIG_DESIGNWARE_SPI
#define CONFIG_DW_GPIO_CS	14
#define CONFIG_DW_SPI_BASE	CONFIG_SPI1_BASE
#define CONFIG_MMC
#define CONFIG_GENERIC_MMC
#define CONFIG_MMC_SPI
#define CONFIG_CMD_MMC
#define CONFIG_CMD_MMC_SPI
#define CONFIG_SPI_TX_WORD	0xff	/* mmc-spi wants all-ones rather than zeros */

/* #define CONFIG_EXTRA_ENV_SETTINGS                                       \ */
/*   "build_image=Baikal boot image\0"                                     \ */
/*   "build_version=1.0.0\0"                                               \ */
/*   "build_date=Thu Jan  1 00:00:00 MSK 1970\0"                           \ */
/*   "build_target=baikal-mips\0"                                          \ */
/*   "baudrate=115200\0"                                                   \ */
/*   "stderr=serial\0"                                                     \ */
/*   "stdin=serial\0"                                                      \ */
/*   "stdout=serial\0"                                                     \ */
/*   "num_cores=2\0"                                                       \ */
/*   "cpufreq=900\0"                                                       \ */
/*   "dnsip=192.168.65.10\0"                                               \ */
/*   "serverip=192.168.68.10\0"                                            \ */
/*   "gatewayip=192.168.68.1\0"                                            \ */
/*   "netmask=255.255.255.0\0"                                             \ */
/*   "ipaddr=192.168.68.230\0"                                             \ */
/*   "dnsip=192.168.65.10\0"                                               \ */
/*   "hostname=baikal\0"                                                   \ */
/*   "enet=8\0"                                                            \ */
/*   "enet_mode=x:n\0"                                                     \ */
/*   "ethact=eth0\0"                                                       \ */
/*   "mgmt_enet=e\0"                                                       \ */
/*   "netdev=eth1\0"                                                       \ */
/*   "rgmii_phyid=3\0"                                                     \ */
/*   "xgmii_phyid=:\0"                                                     \ */
/*   "ethaddr=7a:72:6c:4a:7a:ea\0"                                         \ */
/*   "eth1addr=7a:72:6c:4a:7b:eb\0"                                        \ */
/*   "eth2addr=7a:72:6c:4a:7c:ec\0"                                        \ */
/*   "sata=0x3c\0"                                                         \ */
/*   "sataphy=100\0"                                                       \ */
/*   "sata_enet=e:e:n:n\0"                                                 \ */
/*   "sata_dev=/dev/sda1\0"                                                \ */
/*   "nfsroot=/nfsroot/debian\0"                                           \ */
/*   "uboot_addr_f=0x00000000\0"                                           \ */
/*   "uboot_size=0x00000000\0"                                             \ */
/*   "multi_addr_f=0x00000000\0"                                           \ */
/*   "kernel_addr_f=0x00000000\0"                                          \ */
/*   "kernel_addr_r=0x00000000\0"                                          \ */
/*   "kernel_addr_n=0x80100000\0"                                          \ */
/*   "kernel_size=0x00000000\0"                                            \ */
/*   "kernel_file_name=vmlinux.bin\0"                                      \ */
/*   "fdt_addr_n=0x85D00000\0"                                             \ */
/*   "fdt_addr_f=0x00000000\0"                                             \ */
/*   "fdt_len=0x00040000\0"                                                \ */
/*   "fdt_file_name=baikal.dtb\0"                                          \ */
/*   "fdtcontroladdr=0x9C0C0000\0"                                         \ */
/*   "initrd_high=no\0"                                                    \ */
/*   "loadaddr=0x80000000\0"                                               \ */
/*   "bootcmd=bootmenu\0"                                                  \ */
/*   "bootdelay=5\0"                                                       \ */
/*   "bootfile=bfk-mips.rom\0"                                             \ */
/*   "image_name=bfk-mips.rom\0"                                           \ */
/*   "kdb=setenv bootargs ${bootargs} kgdboc=ttyS0\0"                      \ */
/*   "addmisc=setenv bootargs ${bootargs} maxcpus=${num_cores}\0" \ */
/*   "addhw=setenv bootargs ${bootargs} nohtw nocoherentio ieee754=relaxed\0" \ */
/*   "addtty=setenv bootargs ${bootargs} console=ttyS0,${baudrate}\0"    \ */
/*   "addfb=setenv bootargs ${bootargs} video=sm750fb:1440x900-16@60\0"   \ */
/*   "addip=setenv bootargs ${bootargs} ip=:::::${netdev}:dhcp\0"          \ */
/*   "ramargs=setenv bootargs ${bootargs} root=/dev/ram rw rootwait\0"     \ */
/*   "diskargs=setenv bootargs ${bootargs} root=${sata_dev} rw rootwait\0" \ */
/*   "nfsargs=setenv bootargs ${bootargs} root=/dev/nfs rw rootwait nfsroot=${serverip}:${nfsroot} rw rootwait\0" \ */
/*   "uboot_initrd=setenv kernel_file_name uImage; setenv initrd_file_name uInitrd; setenv initrd_high\0" \ */
/*   "uboot_initramfs=setenv kernel_file_name uImage; setenv initrd_file_name uInitramfs; setenv initrd_high\0" \ */
/*   "all_bootnr=gpio clear 12; gpio clear 13; sleep 0.5; gpio set 12; gpio set 13; sleep 0.5; bootnr ${kernel_addr_n} - ${fdt_addr_n}\0" \ */
/*   "sata_init=sata init; sleep 1\0"                                      \ */
/*   "sata_common_disk=ext4load sata 0 ${kernel_addr_n} ${kernel_file_name}; ext4load sata 0:1 ${fdt_addr_n} ${fdt_file_name}; fdt addr ${fdt_addr_n}; run diskargs addfb addtty addmisc addhw kdb;\0" \ */
/*   "sata_setup_ram=run sata_init; run sata_common_ram;\0"                \ */
/*   "sata_setup_disk=run sata_init; run sata_common_disk\0" \ */
/*   "bootmenu_0=Boot 1.  Boot from SATA disk to Debian (/dev/sda1)=run sata_setup_disk; run all_bootnr\0" \ */
/*   "bootmenu_1=Boot 2.  Boot from Flash to minimal FS (rom)=run flash_boot_ram\0" \ */
/*   "bootmenu_2=Boot 3.  Boot from Flash to Debian (rom+/dev/sda1)=run flash_boot_disk\0" \ */
/*   "bootmenu_3=Boot 4.  Boot from Network to Debian (tftp,/dev/sda1)=run net_setup_disk; run net_load_all_tftp; run all_bootnr\0" */

#endif /* __CONFIG_TPLATFORMS_MITX_H */
