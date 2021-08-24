/*
 * Non-relocable images boot routines
 *
 * (C) 2016 Baikal Electronics JSC
 *
 * Author:
 *   Dmitry Dunaev <dmitry.dunaev@baikalelectronics.ru>
 *
 * SPDX-License-Identifier:	Proprinetary
 */

#include <common.h>
#include <bootm.h>
#include <command.h>
#include <environment.h>
#include <image.h>
#include <malloc.h>

DECLARE_GLOBAL_DATA_PTR;

/* Pointers to binary os/initrd/fdt images */
bootm_headers_t images;

#ifdef CONFIG_LMB
static void boot_start_lmb(bootm_headers_t *images)
{
	ulong		mem_start;
	phys_size_t	mem_size;

	lmb_init(&images->lmb);

	mem_start = getenv_bootm_low();
	mem_size = getenv_bootm_size();

	lmb_add(&images->lmb, (phys_addr_t)mem_start, mem_size);

	arch_lmb_reserve(&images->lmb);
	board_lmb_reserve(&images->lmb);
}
#else
#define lmb_reserve(lmb, base, size)
static inline void boot_start_lmb(bootm_headers_t *images) { }
#endif

/*
 * ----------------------------
 * bootnr - boot non-relocable application
 * image from image in memory
 * ----------------------------
 */
int do_bootnr(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	/* Remove command from parameters */
	argc--; argv++;
	if (!argc)
		return CMD_RET_USAGE;

	memset((void *)&images, 0, sizeof(images));

	boot_start_lmb(&images);

	images.ep = simple_strtoul(argv[0], NULL, 16);
	images.verify = 0;

	argc--; argv++;
	if (!argc)
		goto __boot;

	images.initrd_start = simple_strtoul(argv[0], NULL, 16);
	images.initrd_end = images.initrd_start +
				getenv_ulong("initrd_len", 16, 0);

	images.rd_start = images.initrd_start;
	images.rd_end = images.initrd_end;

	argc--; argv++;
	if (!argc)
		goto __boot;

	images.ft_addr = (char *)simple_strtoul(argv[0], NULL, 16);
	images.ft_len = getenv_ulong("fdt_len", 16, 0);

	debug("boot parameters:\n"
			"  Entry point:   %lx\n"
			"  Initrd start:  %lx\n"
			"  Initrd end:    %lx\n"
			"  Ramdisk start: %lx\n"
			"  Ramdisk end:   %lx\n"
			"  Fdt start:     %p\n"
			"  Fdt lenght:    %lx\n",
			images.ep, images.initrd_start,
			images.initrd_end, images.rd_start,
			images.rd_end,	images.ft_addr,
			images.ft_len);

__boot:
	do_bootm_linux(0, argc, argv, &images);

	return 0;
}

#ifdef CONFIG_SYS_LONGHELP
static char bootnr_help_text[] =
	"[addr [arg ...]]\n    - boot application image stored in memory\n"
	"\tpassing arguments 'arg ...'; when booting a Linux kernel,\n"
	"\t'arg' can be the address of an initrd image\n"
#if defined(CONFIG_OF_LIBFDT)
	"\tWhen booting a Linux kernel which requires a flat device-tree\n"
	"\ta third argument is required which is the address of the\n"
	"\tdevice-tree blob. To boot that kernel without an initrd image,\n"
	"\tuse a '-' for the second argument. If you do not pass a third\n"
	"\ta bd_info struct will be passed instead\n"
#endif
	;
#endif
U_BOOT_CMD(
	bootnr,	CONFIG_SYS_MAXARGS,	1,	do_bootnr,
	"boot non-relocable application image from memory", bootnr_help_text
);
