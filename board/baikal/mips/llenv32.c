/*
 * Copyright (C) 2013 Baikal Electronics.
 *
 * Author: Alexey Malahov <Alexey.Malahov@baikalelectronics.com>
 *
 * Low-level tests are to verify functionality of the basic hardware/software
 * subcomponents. These tests should be used to detect and indicate hardware
 * errors which does not allow OS to operate properly.
 */

#include <common.h>
#include <command.h>
#include <llenv.h>

#define LLENV32_NUM_MAX		3

int do_llenv32 (cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
	switch (argc) {
		case 2:
			if (strcmp(argv[1], "all") == 0) {
				printf("llenv32: all\n");
				llenv_run_all();
			} else {
				printf("llenv32: ts: %s\n", argv[1]);
				llenv_run_ts(argv[1], NULL);
			}
			break;
		case 3:
			printf("llenv32: ts/tc: %s/%s\n", argv[1], argv[2]);
			llenv_run_ts(argv[1], argv[2]);
	}

	return 0;
}

U_BOOT_CMD(
	llenv32,    LLENV32_NUM_MAX,     0,      do_llenv32,
	"low-level and POST tests.",
	"memtest [movinv0 | movinv1 | movinv_walk]"
);

