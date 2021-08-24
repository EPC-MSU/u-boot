/*
 * (C) Copyright 2002-2010
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef	__ASM_GBL_DATA_H
#define __ASM_GBL_DATA_H

#include <asm/regdef.h>

/* Architecture-specific global data */
struct arch_global_data {
#ifdef CONFIG_JZSOC
	/* There are other clocks in the jz4740 */
	unsigned long per_clk;	/* Peripheral bus clock */
	unsigned long dev_clk;	/* Device clock */
	unsigned long sys_clk;
	unsigned long tbl;
	unsigned long lastinc;
#endif
#ifdef CONFIG_TARGET_BAIKAL_MIPS
	unsigned int cpu;
	unsigned int num_cores;
	unsigned long cpu_clk;	/* CPU bus clock */
	unsigned long sata_clk;	/* SATA bus clock */
	unsigned long eth_clk;	/* Base ETH bus clock */
	unsigned long ddr_clk;	/* Base ETH bus clock */
	unsigned long pci_clk;	/* Base ETH bus clock */
	unsigned long axi_clk;	/* APB bus clock */
	unsigned long apb_clk;	/* APB bus clock */
	unsigned char *nvram;	/* NVRAM blob data */
#endif
};

#include <asm-generic/global_data.h>

#define DECLARE_GLOBAL_DATA_PTR     register volatile gd_t *gd asm ("k0")

#endif /* __ASM_GBL_DATA_H */
