/*
 * File:        cmd_cpu.с
 * Description: Implementation of 'cpu' command
 * Copyright 2015-2018, Baikal Electronics JSC
 *
 */

#include <common.h>
#include <command.h>
#include <asm/io.h>
#include <asm/mipsregs.h>
#include <asm/arch/clock_manager.h>

#define CMD_CPU_HIGH_LIMIT_MHZ 1500
#define CMD_CPU_LOW_LIMIT_MHZ 100

DECLARE_GLOBAL_DATA_PTR;

#ifdef CONFIG_BAIKAL_PLL
static int do_cpu_freq(int argc, char * const argv[])
{
	unsigned int freq;
	char fstr[5] = "\0";

	/* Check arguments */
	if (argc != 1)
		return CMD_RET_USAGE;
	/* Get value in MHz */
	freq = simple_strtoul(argv[0], NULL, 10);
	/* Check high freq limit */
	if (freq > CMD_CPU_HIGH_LIMIT_MHZ) {
		sprintf(fstr, "%u", CMD_CPU_HIGH_LIMIT_MHZ);
		puts("The CPU frequency higher than ");
		puts(fstr);
		puts("MHz is not supported\n");
		return CMD_RET_FAILURE;
	}
	/* Check low freq limit */
	if (freq < CMD_CPU_LOW_LIMIT_MHZ) {
		sprintf(fstr, "%u", CMD_CPU_LOW_LIMIT_MHZ);
		puts("The CPU frequency less than ");
		puts(fstr);
		puts("MHz is not supported\n");
		return CMD_RET_FAILURE;
	}
	/* Set clock and check errors*/
	if (baikal_clock_core_mhz(freq)) {
		puts("Failed to set core clock\n");
		return CMD_RET_FAILURE;
	}
	sync();
	/* Command exec result */
	puts("Core clock setup is done.\n\n");
	gd->arch.cpu_clk  = MHZ_TO_HZ(freq);

	/* Full cpu info */
	print_cpuinfo();

	puts("\n");

	return CMD_RET_SUCCESS;
}
#endif

static int do_cpu_info(void)
{
	puts("\n");
	/* Full cpu info */
	print_cpuinfo();
	puts("\n");

	return CMD_RET_SUCCESS;
}

#define CPU_CACHE_ECC_PE	(1 << 31)
#define CPU_CACHE_ECC_PO	(1 << 30)
#define CPU_CACHE_ECC_WST	(1 << 29)
#define CPU_CACHE_ECC_PCO	(1 << 27)
#define CPU_CACHE_ECC_LBE	(1 << 25)
#define CPU_CACHE_ECC_WABE	(1 << 24)
#define CPU_CACHE_ECC_L2	(1 << 23)
#define CPU_CACHE_ECC_PCD	(1 << 22)
#define CPU_CACHE_ECC_DYT	(1 << 21)
#define CPU_CACHE_ECC_SE	(1 << 20)
#define CPU_CACHE_ECC_FE	(1 << 19)
#define CPU_CACHE_ECC_PI	0x0ff0
#define CPU_CACHE_ECC_PD	0x000f

#define ON_OFF(V,A) ((V) & (A)) ? "ON" : "OFF"
#define SET(V,A) ((V) & (A)) ? "SET" : "NOT SET"

static int do_cpu_ecc(int argc, char * const argv[])
{
	unsigned int ecc = read_c0_ecc();

	puts("\n\tCPU  L1 and L2 cache ECC state\n");
    puts("\t------------------------------\n");
    printf("\tL1 Parity checking:     %s\n", ON_OFF(ecc, CPU_CACHE_ECC_PE));
    printf("\tL2 Parity checking:     %s\n", ON_OFF(ecc, CPU_CACHE_ECC_PE | CPU_CACHE_ECC_L2));
    printf("\tParity Overwrite:       %s\n", ON_OFF(ecc, CPU_CACHE_ECC_PO));
    printf("\tPrecode override:       %s\n", ON_OFF(ecc, CPU_CACHE_ECC_PCO));
    printf("\tTag Index test mode:    %s\n", ON_OFF(ecc, CPU_CACHE_ECC_WST));
    printf("\tLoad Bus Error (LBE):   %s\n", SET(ecc, CPU_CACHE_ECC_LBE));
    printf("\tWrite Bus Error (WABE): %s\n", SET(ecc, CPU_CACHE_ECC_WABE));
    printf("\tPrecode didabled (PCD): %s\n", SET(ecc, CPU_CACHE_ECC_PCD));
    printf("\tDirty Array flag:       %s\n", SET(ecc, CPU_CACHE_ECC_DYT));
    printf("\tFirst unrecover. error: %s\n", SET(ecc, CPU_CACHE_ECC_SE));
    printf("\tFirst recover. error:   %s\n", SET(ecc, CPU_CACHE_ECC_FE));
    printf("\tParity bits per dword:  0x%02X\n", ecc & CPU_CACHE_ECC_PI);
    printf("\tParity bits being r/w:  0x%02X\n\n", ecc & CPU_CACHE_ECC_PD);

	return CMD_RET_SUCCESS;
}

static int do_cpu_power(int argc, char * const argv[])
{
	int i;
	printf("Arguments:\n");
	for (i = 0; i < argc; i++)
		printf("\t%s", argv[i]);
	printf("\n\nNot implemented yet!\n");
	return CMD_RET_SUCCESS;
}

static int do_cpu(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	if (argc < 2)
		return CMD_RET_USAGE;

	if (strcmp(argv[1], "info") == 0)
		return do_cpu_info();

#ifdef CONFIG_BAIKAL_PLL
	if (strcmp(argv[1], "freq") == 0)
		return do_cpu_freq(argc-2, &argv[2]);
#endif

	if (strcmp(argv[1], "ecc") == 0)
		return do_cpu_ecc(argc-2, &argv[2]);

	if ((strcmp(argv[1], "off") == 0) ||
	    (strcmp(argv[1], "reset") == 0))
		return do_cpu_power(argc-1, &argv[1]);

	return CMD_RET_USAGE;
}

U_BOOT_CMD(
    cpu ,    CONFIG_SYS_MAXARGS,    1,     do_cpu,
    "CPU manipulation commands",
    "info\n"
    "       - show CPU info\n"
#ifdef CONFIG_BAIKAL_PLL
    "freq <freq>\n"
    "       - set core frequency to <freq> MHz"
#endif
    "ecc\n"
    "       - core cache ECC manipulation"
    "off\n"
    "       - power off CPU by CPC"
    "reset\n"
    "       - reset CPU by CPC"
);
/* End of file cmd_cpu.c */

