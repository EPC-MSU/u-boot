/* 
 * File:        cmd_service.c
 * Description: System management command
 * Created:     06 Aug 2015
 * Author:      Dmitry Dunaev <dmitry.dunaev@baikalelectronics.ru>
 * Copyright 2015,2016 Baikal Electronics JSC
 */

#include <common.h>
#include <command.h>
#include <bcd.h>
#include <net.h>
#include <asm/io.h>
#include "oem.h"
#include "nvram.h"

/* Board type get / set routine */
static int do_service_board(int argc, char * const argv[])
{
	if (!vdata.crc)
		 oem_get_vendor_data(&vdata);

	if (argc == 1) {
		vdata.type = simple_strtoul(argv[0], NULL, 10) & 0xff;
		oem_set_vendor_data(&vdata);
	}
	
	puts("Board: ");
	switch (vdata.type) {
		case OEM_VENDOR_BT_BFK:
			puts("BFK");
			break;
		default:
			puts("UNKNOWN");
			break;
	}
	putc('\n');

	return CMD_RET_SUCCESS;
}

/* Board revision get / set routine */
static int do_service_revision(int argc, char * const argv[])
{
	char *tail;

	if (!vdata.crc)
		 oem_get_vendor_data(&vdata);

	if (argc == 1) {

		vdata.revision = (simple_strtoul(argv[0], &tail, 10) & 0x0f) << 4;
		vdata.revision |= simple_strtoul(&tail[1], NULL, 10) & 0x0f;
		oem_set_vendor_data(&vdata);
	}
	
    printf("Board rev: %u.%u\n", (vdata.revision >> 4) & 0x0f,
            vdata.revision & 0x0f);

	return CMD_RET_SUCCESS;
}

/* Board serial number get / set routine */
static int do_service_serial(int argc, char * const argv[])
{
	if (argc == 1) {
		vdata.serial = simple_strtoull(argv[0], NULL, 10);
		oem_set_vendor_data(&vdata);
	}

	printf("Board S/N: %llu\n", vdata.serial);

	return CMD_RET_SUCCESS;
}

/* SoC serial number get / set routine */
static int do_service_cpu(int argc, char * const argv[])
{
	if (argc == 1) {
		vdata.cpu = simple_strtoull(argv[0], NULL, 10);
		oem_set_vendor_data(&vdata);
	}

	printf("SoC S/N: %u\n", vdata.cpu);

	return CMD_RET_SUCCESS;
}

/* MAC addresess manipulation */
static int do_service_mac(int argc, char * const argv[])
{
	u32 iface;
	uchar enetaddr[6];

	if ((argc != 1) && (argc != 2))
		return CMD_RET_USAGE;

	iface = simple_strtoul(argv[0], NULL, 10);

	if (iface >= OEM_MAX_IFACES) {
		printf("Error: Invalid interface\n\n");
		return CMD_RET_USAGE;
	}

	if (argc == 2) {
		eth_parse_enetaddr(argv[1], enetaddr);
		if (oem_set_macaddr(iface, enetaddr))
			return CMD_RET_FAILURE;
	}

	if (oem_get_macaddr(iface, enetaddr))
		return CMD_RET_FAILURE;
	
	oem_print_macaddr(iface, enetaddr);

	return CMD_RET_SUCCESS;
}

static int do_service(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	if (argc < 2)
		return CMD_RET_USAGE;

	if (strcmp(argv[1], "mac") == 0)
		return do_service_mac(argc-2, &argv[2]);

	if (strcmp(argv[1], "board") == 0)
		return do_service_board(argc-2, &argv[2]);

	if (strcmp(argv[1], "rev") == 0)
		return do_service_revision(argc-2, &argv[2]);

	if (strcmp(argv[1], "serial") == 0)
		return do_service_serial(argc-2, &argv[2]);

	if (strcmp(argv[1], "cpu") == 0)
		return do_service_cpu(argc-2, &argv[2]);

	return CMD_RET_USAGE;
}

U_BOOT_CMD(
    service , CONFIG_SYS_MAXARGS,    1,     do_service,
    "Board service maintaince command\n          (authorized personal only)",
    "serial         "
    "- get / set board serial number\n"
    "service board <type>   "
    "- get / set board type (0=BFK)\n"
    "service rev <revision> "
    "- get / set board revision (decimal, 18 digits max)\n"
    "service cpu <SoC serial number> "
    "- get / set SoC serial number (0-65535)\n"
    "service mac <iface_no> "
    "- manage mac address for <iface_no> (0-3)"
);
/* End of file cmd_service.c */
