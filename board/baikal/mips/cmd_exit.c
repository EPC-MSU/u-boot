/* 
 * File:        cmd_exit.c
 * Description: Implementation of proprenitary 'exit' command
 * Created:     15 Nov 2013
 * Author:      Dmitry Dunaev <dmitry.dunaev@baikalelectronics.ru>
 * Copyright 2013 Baikal Electronics
 *
 */

#include <common.h>
#include <command.h>
#include <asm/io.h>

#ifdef CONFIG_CMD_EXIT

#define QEMU_EXIT_BLOCK KSEG1ADDR(0x1F0FF000)

static int do_exit(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	if (argc != 2)
		return CMD_RET_USAGE;

    __raw_writel(simple_strtoul(argv[1], NULL, 10), QEMU_EXIT_BLOCK);
    return 0;
}

U_BOOT_CMD(
    exit ,    2,    1,     do_exit,
    "exit from simulator with specified error code",
    "RC\n"
    "    - exit with exit code = RC (RC is _decimal_ !!!)"
);
#endif
/* End of file cmd_exit.c */

