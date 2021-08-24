/*
 * File:        cpu.c
 * Description: CPU information routines
 * Copyright 2014-2018, Baikal Electronics JSC
 *
 */

#include <common.h>
#include <asm/mipsregs.h>
#include <asm/mips-cm.h>
#include <asm/mips-cpc.h>
#include <asm/maar.h>
#include <asm/r4kcache.h>
#include <asm/arch/clock.h>
#include <asm/arch/clock_manager.h>
#include "pvt.h"

DECLARE_GLOBAL_DATA_PTR;

#ifdef CONFIG_ARCH_CPU_INIT_R

#define ERRCTL_PE			    (1 << 31)
#define ERRCTL_L2			    (1 << 23)
#define MIPS_CONFIG3_MSAP		(1 << 29)
#define MIPS_CONFIG5_MSAEN		(1 << 27)
#define MIPS_STATUS_FR			(1 << 26)

#ifndef CONFIG_MIPS_GCR_BASE
#error "Base address of Coherency Manager undefined!"
#endif

#ifndef CONFIG_MIPS_GCR_BASE
#error "Base address of Cluster Power Controller undefined!"
#endif

void *mips_cpc_base = (void *)CONFIG_MIPS_CPC_BASE;
void *mips_cm_base = (void *)CONFIG_MIPS_GCR_BASE;
void *mips_cm_l2sync_base = NULL;

static int arch_cpu_init_simd(void)
{
    /* Enable SIMD */
    if (read_c0_config3() | MIPS_CONFIG3_MSAP) {
        write_c0_status(read_c0_status() | MIPS_STATUS_FR);
        write_c0_config5(read_c0_config5() | MIPS_CONFIG5_MSAEN);
        asm("ehb");
        puts("MIPS:  SIMD ready\n");
    }
    /* Return success */
    return 0;
}

static int arch_cpu_init_wrmerge(void)
{
    /* Setup Write Merge */
    write_c0_config(read_c0_config() | MIPS32R5_CONF_MM);    
    asm("ehb");
    puts("MIPS:  Write Merge enable\n");
    /* Return success */
    return 0;
}

static int arch_cpu_init_maar(void)
{
    unsigned long lower, upper, skip;
    /* Setup MAAR region */

    lower = CPHYSADDR(CONFIG_SYS_SDRAM_BASE);
    skip = 0x10000 - (CONFIG_SYS_SDRAM_BASE & 0xffff);
    lower += skip;

    upper = CPHYSADDR(CONFIG_SYS_SDRAM_BASE);
    upper += CONFIG_SYS_SDRAM_SIZE - 1;

    write_maar_pair(0, lower, upper, MIPS_MAAR_S);
    printf("MIPS:  MAAR[0]: 0x%08lx-0x%08lx speculate\n", lower, upper);

    /* Return success */
    return 0;
}

int arch_cpufreq_setup(void)
{
    unsigned int freq = getenv_ulong("cpufreq", 10, 0);

    /* Set freq */
    if ((freq > 99) && (freq <= 1500)) {
        baikal_clock_core_mhz(freq);
        printf("ARCH:  CPU core freq is set to %uMHz\n", freq);
        gd->arch.cpu_clk  = MHZ_TO_HZ(freq);
    }
    /* Return success */
    return 0;
}

int cpu_init_r(void)
{
    /* Enable SIMD */
    arch_cpu_init_simd();
    /* Enable Write Merge */
    arch_cpu_init_wrmerge();
    /* Setup MAAR */
    arch_cpu_init_maar();
#ifdef CONFIG_SYS_CPU_SCACHE
    /* L2$ init */
    mips_sc_init();
#endif
    /* CPU frequency */
    arch_cpufreq_setup();
    /* Return success */
    return 0;
}
#endif /* CONFIG_ARCH_CPU_INIT_R */

int arch_cpu_init(void)
{
    /* Enable AXI clocks */
    baikal_clock_init();
#ifdef CONFIG_BAIKAL_PLL
    /* Set clocks */
    gd->arch.cpu_clk  = baikal_pll_get(BK_PLL_CORE);
    gd->arch.sata_clk = baikal_pll_get(BK_PLL_SATA);
    gd->arch.eth_clk  = baikal_pll_get(BK_PLL_ETH);
    gd->arch.pci_clk  = baikal_pll_get(BK_PLL_PCI);
    gd->arch.ddr_clk  = baikal_pll_get(BK_PLL_DDR);
#else
    gd->arch.cpu_clk  = CONFIG_SYS_CPU_CLOCK_RATE;
#endif /* CONFIG_BAIKAL_PLL */
    /* Return success */
    return 0;
}

int print_cpuinfo(void)
{
    u32 config = read_c0_config1();
    u32 cpunum = read_c0_ebase() & 0x03ff;
    u32 ecc    = read_c0_ecc();

    gd->arch.cpu = read_c0_prid();
    gd->arch.num_cores = 1;

    printf("CPU:   MIPS32 ");
    switch (gd->arch.cpu & 0x0ffff00) {
        case 0x00018000:
            printf("4Kc");
            break;
        case 0x00018400:
            printf("4KEcR1");
            break;
        case 0x00019000:
            printf("4KEc");
            break;
        case 0x00019300:
            if (config & 1)
                printf("24Kf");
            else
                printf("24Kc");
            break;
        case 0x00019500:
            printf("34Kf");
            break;
        case 0x00000400:
            printf("R4000");
            break;
        case 0x00018100:
            if (config & 1)
                printf("5Kf");
            else
                printf("5Kc");
            break;
        case 0x00018200:
            printf("20Kc");
            break;
        case 0x0001a800:
            printf("P5600");
            gd->arch.num_cores = mips_cm_numcores();
            break;
        default:
            printf("unknown");
    }
    printf(" @ %lu MHz", HZ_TO_MHZ(gd->arch.cpu_clk));
    printf(" (Rev %u.%u)\n", (gd->arch.cpu >> 5) & 0x07, gd->arch.cpu & 0x07);
    printf("FPU:   %sresent\n", (config & 1) ? "P":"Not p");
    printf("Cores: %u (running on CPU%u)\n", gd->arch.num_cores, cpunum);
    printf("Timer: %u MHz\n", CONFIG_SYS_MIPS_TIMER_FREQ / 1000000);
    printf("ECC:   %s %s (%08x)\n", (ecc & ERRCTL_PE) ? "L1" : "NONE",
        (ecc & (ERRCTL_PE | ERRCTL_L2)) ? "L2" : "", ecc);
    if (gd->arch.ddr_clk)
        printf("PLLs:  CPU: %4luMHz  SATA:%4luMHz  ETH:%4luMHz\n"
               "       PCIE:%4luMHz  DDR3:%4luMHz  AXI:%4luMHz\n",
          HZ_TO_MHZ(gd->arch.cpu_clk), HZ_TO_MHZ(gd->arch.sata_clk),
          HZ_TO_MHZ(gd->arch.eth_clk), HZ_TO_MHZ(gd->arch.pci_clk),
          HZ_TO_MHZ(gd->arch.ddr_clk), HZ_TO_MHZ(gd->arch.cpu_clk) / 2);
    else
        printf("PLLs:  CPU:%luMHz (fixed)\n", HZ_TO_MHZ(gd->arch.cpu_clk));

    /* Return success */
    return 0;
}

static inline void wdt_write(unsigned int val, unsigned int ofs)
{
    __raw_writel(val, (CONFIG_DW_WDT_BASE + ofs));
}

static inline unsigned int wdt_read(unsigned int ofs)
{
    return __raw_readl(CONFIG_DW_WDT_BASE + ofs);
}

extern int designware_wdt_settimeout(unsigned int timeout);
void _machine_restart(void)
{
#if defined(CONFIG_MIPS_CPC_BASE)
    fprintf(stderr, "Reset CPU by CPC\n");
    /* Flush cache */
    flush_cache(CONFIG_SYS_SDRAM_BASE, CONFIG_SYS_SDRAM_SIZE);
    /* Coherence only with core 0 */
    write_gcr_cl_coherence(1 << CM_GCR_Cx_COHERENCE_COHDOMAINEN_SHF);
    /* */
    udelay(1000);
    sync();
    write_gcr_cl_coherence(0);
    /* */
    udelay(1000);
    sync();
    /* 1024-cycle delay */
    write_cpc_resetlen(0x3ff);
    /* Write reset cmd */
    write_cpc_cl_cmd(CPC_Cx_CMD_RESET << CPC_Cx_CMD_SHF);
#elif defined(CONFIG_DESIGNWARE_WATCHDOG)
    fprintf(stderr, "Reset CPU by WDT\n");
    /* Setup WDT and start */
    designware_wdt_settimeout(40000);
#else
    fprintf(stderr, "Dummy reset. Branch to reset vector\n");
    /* Go to reset vector */
    void (* __dummy_reset) (void) = (void *)CONFIG_SYS_TEXT_BASE;
    __dummy_reset();
#endif
    /* Suspend */
    asm(".set\tmips3\n\t" "wait\n\t" ".set\tmips0");
}

/* End of file cpu.c */
