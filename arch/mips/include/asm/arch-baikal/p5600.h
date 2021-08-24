/*
 *  Baikal-T (P5600) core system blocks definitions
 *  (c)2014 Baikal Electronics OJSC
 *
 */

#ifndef _ASM_P5600_H
#define _ASM_P5600_H

#include <asm/mipsregs.h>

//#include <asm/arch/addr.h>

/* Core-local and Core-other blocks */
#define MIPS_GCR_GCB_BASE		0x0000
#define MIPS_GCR_CLCB_BASE		0x2000
#define MIPS_GCR_COCB_BASE		0x4000

/* Offsets from the GCR base address to various control blocks */
#define MIPS_GCR_CONFIG			0x00
#define MIPS_GCR_SELF_BASE		0x08
#define MIPS_GCR_ACCESS			0x20
#define MIPS_GCR_CUSTOM_BASE	0x60
#define MIPS_GCR_GIC_BASE		0x80
#define MIPS_GCR_CPC_BASE		0x88
/* Core-local offsets */
#define MIPS_GCR_COHER_OFF		0x08
#define MIPS_GCR_CFG_OFF		0x10
#define MIPS_GCR_ADDR_OFF		0x18
#define MIPS_GCR_RST_BS_OFF		0x20
#define MIPS_GCR_ID_OFF			0x28
#define MIPS_GCR_RST_EXT_OFF	0x30
#define MIPS_GCR_TCID_OFF		0x40
/* Core-other offsets */
#define MIPS_GCR_CL_COHER_OFF	0x08
#define MIPS_GCR_CL_CFG_OFF		0x10
#define MIPS_GCR_CL_ADDR_OFF	0x18
#define MIPS_GCR_CL_RST_BS_OFF	0x20
#define MIPS_GCR_CL_ID_OFF		0x28
#define MIPS_GCR_CL_RST_EXT_OFF	0x30
#define MIPS_GCR_CL_TCID_OFF	0x40

/* Block enable flags */
#define MIPS_GCR_BLOCK_ENABLE	0x01
/* Offsets from the CPC base address to various control blocks */
#define MIPS_CPC_GCB_BASE		0x0000
#define MIPS_CPC_CLCB_BASE		0x2000
#define MIPS_CPC_COCB_BASE		0x4000
/* CPC Core Local Block offsets */
#define MIPS_CPC_CL_CMD_OFF		0x0000
#define MIPS_CPC_CL_STATUS_OFF	0x0008
#define MIPS_CPC_CL_ADDR_OFF	0x0010
/* CPC Core Other Block offsets */
#define MIPS_CPC_CO_CMD_OFF		0x0000
#define MIPS_CPC_CO_STATUS_OFF	0x0008
#define MIPS_CPC_CO_ADDR_OFF	0x0010
/* CPC Blocks Commands */
#define MIPS_CPC_CMD_CLKOFF		1
#define MIPS_CPC_CMD_PWROFF		2
#define MIPS_CPC_CMD_PWRON		3
#define MIPS_CPC_CMD_RESET		4
/* CPC Core Other address shift */
#define MIPS_CPC_ADDR_SHFT		16


#ifndef __ASSEMBLER__
#include <asm/io.h>

/* Global Configuration Register functions */
#ifdef CONFIG_MIPS_GCR_BASE

#define GCR_BASE_KSEG1 			KSEG1ADDR(CONFIG_MIPS_GCR_BASE)
#define read_gcr_reg(reg) 		__raw_readl((GCR_BASE_KSEG1 + reg))
#define write_gcr_reg(reg, val)	__raw_writel(val, (GCR_BASE_KSEG1 + reg))

static inline u32 read_gcr_cpu_reg(u32 cpuid, u32 reg)
{
	write_gcr_reg(MIPS_GCR_CLCB_BASE + MIPS_GCR_ADDR_OFF, cpuid << 16);
	return read_gcr_reg(MIPS_GCR_COCB_BASE + reg);
}

static inline void write_gcr_cpu_reg(u32 cpuid, u32 reg, u32 val)
{
	write_gcr_reg(MIPS_GCR_CLCB_BASE + MIPS_GCR_ADDR_OFF, cpuid << 16);
	write_gcr_reg(MIPS_GCR_COCB_BASE + reg, val);
}

static inline u32 read_gcr_config(void)
{
	return read_gcr_reg(MIPS_GCR_GCB_BASE + MIPS_GCR_CONFIG);
}

static inline u32 read_gcr_access(void)
{
	return read_gcr_reg(MIPS_GCR_GCB_BASE + MIPS_GCR_ACCESS);
}

static inline void write_gcr_access(u32 mask)
{
	write_gcr_reg(MIPS_GCR_GCB_BASE + MIPS_GCR_ACCESS, mask & 0xff);
}

static inline void write_gcr_co_reset_legacy(u32 cpuid)
{
	/* No EVA, BEV=0xBFCO_0000, BEVExceptionBaseMask = 0x00 */
	write_gcr_cpu_reg(cpuid, MIPS_GCR_RST_EXT_OFF, 0);
	/* Set BEV=0 */
	write_c0_status(read_c0_status() & ~ST0_BEV);
}

static inline void write_gcr_co_reset_base(u32 cpuid, u32 base)
{
	/* No EVA, BEVExceptionBaseMask = 0x00 BEVExceptionPhysMask = 0x00 */
	write_gcr_cpu_reg(cpuid, MIPS_GCR_RST_EXT_OFF, 1 << 30);
	/* Set reset address */
	write_gcr_cpu_reg(cpuid, MIPS_GCR_CL_RST_BS_OFF, KSEG1ADDR(base));
	/* Set BEV=1 */
	write_c0_status(read_c0_status() | ST0_BEV);
}
static inline void write_gcr_co_coherence(u32 cpuid, u8 mask)
{
	write_gcr_cpu_reg(cpuid, MIPS_GCR_COHER_OFF, mask);
}

#else
static inline u32 read_gcr_config(void)
{
	return 0;
}
static inline u32 read_gcr_access(void)
{
	return 0xff;
}
static inline void write_gcr_access(u32 mask) {}
static inline void write_gcr_co_reset_legacy(u32 cpuid) {}
static inline void write_gcr_co_reset_base(u32 cpuid, u32 base) {}
static inline void write_gcr_co_coherence(u32 cpuid, u8 mask) {}

#endif /* CONFIG_MIPS_GCR_BASE */

/* Cluster Power Control functions */
#ifdef CONFIG_MIPS_CPC_BASE
static inline void set_pwr_cmd(u32 cpunum, u32 cmd)
{
	/* Set core other pointer */
	__raw_writel(cpunum << MIPS_CPC_ADDR_SHFT, KSEG1ADDR(CONFIG_MIPS_CPC_BASE +
			MIPS_CPC_CLCB_BASE + MIPS_CPC_CL_ADDR_OFF));
	/* Perform operation */
	__raw_writel(cmd & 0x0f, KSEG1ADDR(CONFIG_MIPS_CPC_BASE +
			MIPS_CPC_COCB_BASE + MIPS_CPC_CO_CMD_OFF));
}
static inline void cpc_rst_cpu(u32 cpunum)
{
	set_pwr_cmd(cpunum, MIPS_CPC_CMD_RESET);
}
static inline void cpc_pwr_on_cpu(u32 cpunum)
{
	set_pwr_cmd(cpunum, MIPS_CPC_CMD_PWRON);
}
static inline void cpc_pwr_off_cpu(u32 cpunum)
{
	set_pwr_cmd(cpunum, MIPS_CPC_CMD_PWROFF);
}
static inline void cpc_clk_off_cpu(u32 cpunum)
{
	set_pwr_cmd(cpunum, MIPS_CPC_CMD_CLKOFF);
}
#else
static inline void cpc_rst_cpu(u32 cpunum) {}
static inline void cpc_pwr_on_cpu(u32 cpunum) {}
static inline void cpc_pwr_off_cpu(u32 cpunum) {}
static inline void cpc_clk_off_cpu(u32 cpunum) {}
#endif /* CONFIG_MIPS_CPC_BASE */

/* Common functions */
static inline u32 mips_numcores(void)
{
	return ((read_gcr_config() & 0xff) + 1);
}
#endif /* __ASSEMBLER__ */

#endif /* _ASM_P5600_H */
