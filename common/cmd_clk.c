/*
 * Copyright (C) 2013 Xilinx, Inc.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#include <common.h>
#include <command.h>
#include <clk.h>

int __weak soc_clk_dump(void)
{
	puts("Not implemented\n");
	return 1;
}

int __weak soc_clk_show(int id)
{
	puts("Not implemented\n");
	return 1;
}

int __weak soc_clk_enable(int id)
{
	puts("Not implemented\n");
	return 1;
}

int __weak soc_clk_disable(int id)
{
	puts("Not implemented\n");
	return 1;
}

int __weak soc_clock_set(int id,
	unsigned int od, unsigned int nf, unsigned int nr)
{
	puts("Not implemented\n");
	return 1;
}

static int do_clk_dump(cmd_tbl_t *cmdtp, int flag, int argc,
		       char *const argv[])
{
	return soc_clk_dump();
}

static int do_clk_show(cmd_tbl_t *cmdtp, int flag, int argc,
		       char *const argv[])
{
	int id;

	if (argc != 2)
		return CMD_RET_USAGE;

	id = simple_strtoul(argv[1], NULL, 10);

	return soc_clk_show(id);
}

static int do_clk_enable(cmd_tbl_t *cmdtp, int flag, int argc,
		       char *const argv[])
{
	int id;

	if (argc != 2)
		return CMD_RET_USAGE;

	id = simple_strtoul(argv[1], NULL, 10);

	return soc_clk_enable(id);
}

static int do_clk_disable(cmd_tbl_t *cmdtp, int flag, int argc,
		       char *const argv[])
{
	int id;

	if (argc != 2)
		return CMD_RET_USAGE;

	id = simple_strtoul(argv[1], NULL, 10);

	return soc_clk_disable(id);
}

#if 0
static int do_clk_set(cmd_tbl_t *cmdtp, int flag, int argc,
		       char *const argv[])
{
	int id = simple_strtoul(argv[1], NULL, 10);
	unsigned int od = simple_strtoul(argv[2], NULL, 10);;
	unsigned int nf = simple_strtoul(argv[3], NULL, 10);;
	unsigned int nr = simple_strtoul(argv[4], NULL, 10);;

	soc_clock_set(id, od, nf, nr);

	return 0;
}
#endif

static cmd_tbl_t cmd_clk_sub[] = {
	U_BOOT_CMD_MKENT(dump,    1, 1, do_clk_dump,    "", ""),
	U_BOOT_CMD_MKENT(get,     2, 0, do_clk_show,    "", ""),
	U_BOOT_CMD_MKENT(enable,  2, 0, do_clk_enable,  "", ""),
	U_BOOT_CMD_MKENT(disable, 2, 0, do_clk_disable, "", ""),
};

static int do_clk(cmd_tbl_t *cmdtp, int flag, int argc,
		  char *const argv[])
{
	cmd_tbl_t *c;

	if (argc < 2)
		return CMD_RET_USAGE;

	/* Strip off leading 'clk' command argument */
	argc--;
	argv++;

	c = find_cmd_tbl(argv[0], &cmd_clk_sub[0], ARRAY_SIZE(cmd_clk_sub));

	if (c)
		return c->cmd(cmdtp, flag, argc, argv);
	else
		return CMD_RET_USAGE;
}

#ifdef CONFIG_SYS_LONGHELP
static char clk_help_text[] =
	"dump             - Print clock frequencies\n"
	"clk get     <clk_id> - Print clock <clk_id> frequence\n"
	"clk enable  <clk_id> - Enable clock <clk_id>\n"
	"clk disable <clk_id> - Disable clock <clk_id>\n"
	"\nClock ID <clk_id> is decimal value.\n";
#endif

U_BOOT_CMD(clk, 5, 1, do_clk, "CLK sub-system", clk_help_text);
