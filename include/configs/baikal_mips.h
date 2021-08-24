/*
 * Configuration settings for the Baikal T platform.
 *
 * Copyright (C) 2013-2017 Baikal Electronics JSC
 *
 * Authors:
 *   Dmitry Dunaev <dmitry.dunaev@baikalelectronics.ru>
 *   Alexey.Malahov <alexey.malahov@baikalelectronics.ru>
 *   Ekaterina.Skachko <ekaterina.skachko@baikalelectronics.ru>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#ifndef __CONFIG_BAIKAL_MIPS_H
#define __CONFIG_BAIKAL_MIPS_H

/*
 *-----------------------------------------------
 * Generic Configurations
 *-----------------------------------------------
 */
#define CONFIG_SYS_CPU_CLOCK_RATE	850000000
#define CONFIG_BAIKAL_NAME		"UNDEF"
/* Uboot continues work on DDR fail. */
#define CONFIG_BAIKAL_FW_CONTINUE_ON_DDR_FAIL
/* Use compiler's -lgcc only */
#define CONFIG_USE_PRIVATE_LIBGCC
/* Use comprehensive common/board_f.c and common/board_r.c code to init the
 * system instead of mips-limited arch/mips/lib/board.c code */
#define CONFIG_SYS_GENERIC_BOARD

/*
 *-----------------------------------------------
 * Include low-level tests
 *
 * Locate the libllenv.a and llenv.h files to
 * the <uboot_dir>/lib/baikal/llenv directory
 *-----------------------------------------------
 */
#define CONFIG_BAIKAL_LLENV

/*
 *-----------------------------------------------
 * Common MIPS U-Boot Parameters
 *-----------------------------------------------
 */
/* Base address of reset vector */
#define CONFIG_SYS_TEXT_BASE		0x9FC00000
#define CONFIG_SYS_CLK_FREQ		(CONFIG_SYS_CPU_CLOCK_RATE)
#define CONFIG_SYS_MIPS_TIMER_FREQ	(CONFIG_SYS_CPU_CLOCK_RATE / 2)
/* Startup core and timer freq for lowlevel init */
#define CONFIG_SYS_INIT_CLK_FREQ	600000000
#define CONFIG_SYS_INIT_TIMER_FREQ	(CONFIG_SYS_INIT_CLK_FREQ / 2)
/*
 * Fix for boot controller startup.
 * Bootup delay barrier (in ms).
 * Up to 16s for 300MHz core timer clock.
 */
#if !defined(CONFIG_BAIKAL_T1)
#define CONFIG_SYS_BOOT_DELAY_MS	1000 /* 1s */
#endif /* BAIKAL_T1 */
#define CONFIG_SYS_HZ			1000
#define CONFIG_OF_UPDATE_FREQ
#define CONFIG_ARCH_CPU_INIT_R

/*
 *-----------------------------------------------
 * FIT/FDT image support
 *-----------------------------------------------
 */
#define CONFIG_FIT
#define CONFIG_FIT_SIGNATURE
#define CONFIG_SHA1
#define CONFIG_SHA256
#define CONFIG_MD5
#define CONFIG_LZMA
#define CONFIG_AES
#define CONFIG_RSA
#define CONFIG_IMAGE_FORMAT_LEGACY
#define OF_STDOUT_PATH	"serial0:115200n8"	/* Serial console settings */
#define CONFIG_OF_LIBFDT			/* Open Firmware devicetree support	*/
#define CONFIG_OF_SEPARATE			/* Append dtb to U-boot binary */
#define CONFIG_DEFAULT_DEVICE_TREE 		baikal

/*
 *-----------------------------------------------
 * Cache Configuration
 *-----------------------------------------------
 */
#define CONFIG_SYS_DCACHE_SIZE		(64 * 1024)
#define CONFIG_SYS_ICACHE_SIZE		(64 * 1024)
#define CONFIG_SYS_CACHELINE_SIZE	32
#define CONFIG_MIPS_L2_SIZE		(1024 * 1024)
#define CONFIG_MIPS_L2_LINE_SIZE	32
#define ARCH_DMA_MINALIGN 		CONFIG_SYS_CACHELINE_SIZE

/*
 *-----------------------------------------------
 * U-Boot initial parameters
 *-----------------------------------------------
 */
#define CONFIG_SYS_MONITOR_BASE		CONFIG_SYS_TEXT_BASE
#define CONFIG_SYS_MONITOR_LEN		(1 << 20) /* 1 MB */
#define CONFIG_SYS_MALLOC_LEN		(1 << 20)	/* 1 MB */
#define CONFIG_SYS_BOOTPARAMS_LEN	(128 << 10)	/* 128 KB */
#define CONFIG_SYS_INIT_SP_OFFSET	0xc00000
#define CONFIG_SYS_BOOTM_LEN 		0x01000000

/*
 *-----------------------------------------------
 * MIPS P5600 Warrior Core defaults
 *-----------------------------------------------
 */
#define CONFIG_MIPS_P5600
#define CONFIG_MIPS_GIC_BASE 		0xBBDC0000
#define CONFIG_MIPS_CPC_BASE 		0xBBDE0000
#define CONFIG_MIPS_GCR_BASE 		0xBFBF8000

/*
 *-----------------------------------------------
 * DesignWire DDR controller
 *-----------------------------------------------
 */
#define CONFIG_DDR_CTRL_BASE		0xBF042000
#define CONFIG_DDR_PHY_BASE		0xBF043000
#define CONFIG_DDR_SCHED_OFF		0x0250
#define CONFIG_DDR_SCHED_INIT		0x00001001
#define CONFIG_DDR_SCHED_MSK		0xFFFFFFFB
// #define CONFIG_BAIKAL_PRINT_DDR
/* Offset of the DDR init handler. */
#define CONFIG_DDR_INIT_RESULT_OFF_v0	0x0
#define CONFIG_DDR_INIT_RESULT_OFF_v1	0x4
#define CONFIG_DDR_INIT_RESULT_OFF_t7	0x8
#define CONFIG_DDR_INIT_RESULT_OFF_i2c	0xc
#define CONFIG_DDR_INIT_RESULT_v0	(CONFIG_SRAM_DDR_INIT_RESULT + CONFIG_DDR_INIT_RESULT_OFF_v0)
#define CONFIG_DDR_INIT_RESULT_v1	(CONFIG_SRAM_DDR_INIT_RESULT + CONFIG_DDR_INIT_RESULT_OFF_v1)
#define CONFIG_DDR_INIT_RESULT_t7	(CONFIG_SRAM_DDR_INIT_RESULT + CONFIG_DDR_INIT_RESULT_OFF_t7)
#define CONFIG_DDR_INIT_RESULT_i2c	(CONFIG_SRAM_DDR_INIT_RESULT + CONFIG_DDR_INIT_RESULT_OFF_i2c)
/* Offset of the DDR init handler. */
#define CONFIG_DDR_INIT_RESULT_OFF_v0	0x0
#define CONFIG_DDR_INIT_RESULT_OFF_v1	0x4
#define CONFIG_DDR_INIT_RESULT_OFF_t7	0x8
#define CONFIG_DDR_INIT_RESULT_OFF_i2c	0xc
#define CONFIG_DDR_INIT_RESULT_v0	(CONFIG_SRAM_DDR_INIT_RESULT + CONFIG_DDR_INIT_RESULT_OFF_v0)
#define CONFIG_DDR_INIT_RESULT_v1	(CONFIG_SRAM_DDR_INIT_RESULT + CONFIG_DDR_INIT_RESULT_OFF_v1)
#define CONFIG_DDR_INIT_RESULT_t7	(CONFIG_SRAM_DDR_INIT_RESULT + CONFIG_DDR_INIT_RESULT_OFF_t7)
#define CONFIG_DDR_INIT_RESULT_i2c	(CONFIG_SRAM_DDR_INIT_RESULT + CONFIG_DDR_INIT_RESULT_OFF_i2c)

/*
 *-----------------------------------------------
 * SRAM and it variables
 *-----------------------------------------------
 */
#define CONFIG_SRAM_BASE		0xBBF80000
#define CONFIG_SRAM_SIZE		0x10000
#define CONFIG_SRAM_SPD_TIMESTAMP	(CONFIG_SRAM_BASE + 0x0)
#define CONFIG_SRAM_DDR_INIT_RESULT	(CONFIG_SRAM_BASE + 0x100)
#define CONFIG_SRAM_BUF0_BASE		(CONFIG_SRAM_BASE + 0x200)
#define CONFIG_SRAM_BUF1_BASE		(CONFIG_SRAM_BASE + 0x300)
#define CONFIG_SRAM_STACK_BASE		(CONFIG_SRAM_BASE + CONFIG_SRAM_SIZE - 0x100)

/*
 *-----------------------------------------------
 * Physical Memory Map
 *-----------------------------------------------
 */
#define CONFIG_SYS_SDRAM_BASE		0x80000000
/* Only one bank on lower 512 MB can be reached without MMU */
#define CONFIG_SYS_SDRAM_SIZE		0x08000000
/* Default load address */
#define CONFIG_SYS_LOAD_ADDR		0xA0100000
#define CONFIG_PAGE_SIZE_16KB

/*
 *-----------------------------------------------
 * Define DRAM Banks
 *-----------------------------------------------
 */
/* Memory banks */
#define CONFIG_NR_DRAM_BANKS		1
/* DRAM Bank #1 start address */
#define PHYS_DRAM_BANK1		 	0x00000000
/* DRAM Bank #1 size  */
#define PHYS_DRAM_BANK1_SIZE		0x08000000
/* DRAM Bank #2 start address */
#define PHYS_DRAM_BANK2		 	0x20000000
/* SDRAM Bank #1 size */
#define PHYS_DRAM_BANK2_SIZE		0x04000000

/*
 *-----------------------------------------------
 * PLL setup
 *-----------------------------------------------
 */
#define CONFIG_CMD_CLK
#define CONFIG_BAIKAL_PLL
#define CONFIG_BAIKAL_PLL_BASE	0xBF04D000
#define CONFIG_BAIKAL_OSC_CLOCK 25000000

/*
 *-----------------------------------------------
 * Miscellaneous configurable options
 *-----------------------------------------------
 */
#ifndef DEBUG
/* Should be undefined to save memory */
#define CONFIG_SYS_LONGHELP
#endif
/* Monitor Command Prompt   */
#define CONFIG_SYS_PROMPT			"BAIKAL # "
/* Configuration identification string */
#define CONFIG_INDENT_STRING			"BAIKAL " CONFIG_BAIKAL_NAME " Core"
/* Console I/O Buffer Size  */
#define CONFIG_SYS_CBSIZE			1024
/* Print Buffer Size */
#define CONFIG_SYS_PBSIZE			(CONFIG_SYS_CBSIZE+sizeof(CONFIG_SYS_PROMPT)+16)
/* Мax number of command args   */
#define CONFIG_SYS_MAXARGS			16
/* Boot Argument Buffer Size    */
#define CONFIG_SYS_BARGSIZE			CONFIG_SYS_CBSIZE
/* Use port0 as console */
#define CONFIG_CONS_INDEX			1
#define CONFIG_SYS_HUSH_PARSER

/*
 *-----------------------------------------------
 * UART Configuration
 *-----------------------------------------------
 */
/* Use 16550 compatible device */
#define CONFIG_SYS_NS16550
/* UART pool base address */
#define CONFIG_SYS_NS16550_BASE			0xBF04A000
/* Device vemory load/store as word */
#define CONFIG_SYS_NS16550_MEM32
/* Register width 4 bytes, little endian */
#define CONFIG_SYS_NS16550_REG_SIZE		(-4)
/* Use NS16550 UART as Serial device */
#define CONFIG_SYS_NS16550_SERIAL		1
/* UART CLK divisor */
#if defined(CONFIG_BAIKAL_T1)
#define CONFIG_SYS_NS16550_CLK			(7350000) /* 7.35MHZ / DIV=4 */
#else
#define CONFIG_SYS_NS16550_CLK			(12500000) /* 12.5MHZ / DIV=10 */
#endif /* CONFIG_BAIKAL_T1 */
/* UART1 is enable and COM1 Port base */
#define CONFIG_SYS_NS16550_COM1			(CONFIG_SYS_NS16550_BASE + 0x00000)
/* UART2 is enable and COM2 Port base  */
#define CONFIG_SYS_NS16550_COM2			(CONFIG_SYS_NS16550_BASE + 0x10000)

/*
 *-----------------------------------------------
 * Watchdog Timer parameters
 *-----------------------------------------------
 */
#define CONFIG_DESIGNWARE_WATCHDOG
#define CONFIG_HW_WATCHDOG
#define CONFIG_HW_WATCHDOG_TIMEOUT_MS	2000		/* 2000ms */
#define CONFIG_DW_WDT_CLOCK_KHZ		1000000		/* 1000KHz */
#define CONFIG_DW_WDT_BASE		0xBF04C000

/*
 *-----------------------------------------------
 * GPIO ports parameters
 *-----------------------------------------------
 */
#define CONFIG_DW_GPIO
#define CONFIG_DW_GPIO_BASE		0xBF044000

/*
 *-----------------------------------------------
 * Ethernet controller parameters
 *-----------------------------------------------
 */
#define CONFIG_CMD_NET
#define CONFIG_DESIGNWARE_ETH
#define CONFIG_DESIGNWARE_ETH0_BASE	0xBF05E000
#define CONFIG_DESIGNWARE_ETH1_BASE	0xBF060000
#define CONFIG_DW_ALTDESCRIPTOR
#define CONFIG_NET_MULTI
#define CONFIG_MII
#define CONFIG_CMD_MII
#define CONFIG_PHYLIB
#define CONFIG_PHY_GIGE

/*
 *-----------------------------------------------
 * I2C controller parameters
 *-----------------------------------------------
 */
#define CONFIG_CMD_I2C
#define CONFIG_SYS_I2C_BUS_MAX			2
#define CONFIG_HARD_I2C
#define CONFIG_DW_I2C
#define CONFIG_I2C_MULTI_BUS
#define CONFIG_SYS_I2C_BASE			0xBF046000
#define CONFIG_SYS_I2C_BASE1			0xBF047000
#define CONFIG_SYS_I2C_SPEED			400000		/* 400 kHz */
#define CONFIG_SYS_I2C_SLAVE			0x02

/*
 *-----------------------------------------------
 * DesignWire AHCI SATA controller parameters
 *-----------------------------------------------
 */
#define CONFIG_CMD_SATA
#define CONFIG_CMD_SATA_PHY
#define CONFIG_DWC_AHSATA
#define CONFIG_SYS_SATA_MAX_DEVICE      1
#define CONFIG_DWC_AHSATA_PORT_ID       0
#define CONFIG_DWC_AHSATA_BASE_ADDR	0xBF050000
#define CONFIG_LBA48
#define CONFIG_LIBATA

/*
 *-----------------------------------------------
 * DesignWire xHCD USB controller parameters
 * Due to size constraints (currently, u-boot.bin must fit 0x80000 bytes)
 * we have to choose between SATA and USB.
 *-----------------------------------------------
 */
#define CONFIG_USB
#define CONFIG_USB_STORAGE
#define CONFIG_CMD_USB
#define CONFIG_USB_XHCI
#define CONFIG_USB_XHCI_BAIKAL
#define CONFIG_USB_XHCI_BAIKAL_BASE		0xBF100000
#define CONFIG_SYS_USB_XHCI_MAX_ROOT_PORTS	1
#define CONFIG_USB_MAX_CONTROLLER_COUNT		1

/*
 *-----------------------------------------------
 * DesignWire SPI controller parameters
 *-----------------------------------------------
 */
/* #define CONFIG_SPI */
/* #define CONFIG_HARD_SPI */
#define CONFIG_SPI_FIFO_SIZE		16
#define CONFIG_DW_SPI
#define CONFIG_SPI0_BASE		0xBF040100
#define CONFIG_SPI1_BASE		0xBF04E000
#define CONFIG_SPI2_BASE		0xBF04F000
#define CONFIG_DW_CLOCK_FREQ		50000000
#define CONFIG_DW_SPI_FREQ		11
#ifdef DEBUG
#define CONFIG_DW_SPI_VERBOSE
#endif

/*
 *-----------------------------------------------
 * SPI-flash/Boot controller parameters
 *-----------------------------------------------
 */
#define CONFIG_SYS_FLASH_BASE		0x9C000000
#define CONFIG_BOOT_BASE		0xBF040000
#define CONFIG_BOOT_SPI_FREQ		1000000
#define CONFIG_BOOT_SPI_BUS		0
#define CONFIG_BOOT_SPI_CS		0

#endif /* __CONFIG_BAIKAL_MIPS_H */
