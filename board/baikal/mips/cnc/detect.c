/*
 * File:        detect.c
 * Description: Detect T-platforms CNC board type
 * Authors:     Sergey Semin <sergey.semin@t-platforms.ru>
 *
 * Copyright 2018 T-platforms JSC
 */

#include <common.h>
#include <asm/io.h>
#include <miiphy.h>
#include <netdev.h>
#include <pcf857x.h>

#include "devid.h"
#include "detect.h"

struct cnc_dev_dtb {
	u16 did;
	u8 rev;
	const char *name;
};

static struct cnc_dev_dtb cnc_dtbs[] = {
	{CNC_DID_CUDEV, CNC_REV_ANY, "tplatforms_cnccu-%s.dtb"},
	{CNC_DID_CU, 2, "tplatforms_cnccu-%s.dtb"},
	{CNC_DID_CU, CNC_REV_ANY, "tplatforms_cnccu3-%s.dtb"},
	{CNC_DID_OUT, 1, "tplatforms_cncout-%s.dtb"},
	{CNC_DID_OUT, CNC_REV_ANY, "tplatforms_cncout3-%s.dtb"},
	{CNC_DID_IN, 1, "tplatforms_cncin-%s.dtb"},
	{CNC_DID_IN, CNC_REV_ANY, "tplatforms_cncin3-%s.dtb"},
	{CNC_DID_PROBE, CNC_REV_ANY, "tplatforms_cncin3-%s.dtb"},
	{CNC_DID_DAC, 1, "tplatforms_cncdac-%s.dtb"},
	{CNC_DID_DAC, CNC_REV_ANY, "tplatforms_cncdac3-%s.dtb"},
	{CNC_DID_TTL, 1, "tplatforms_cncttl-%s.dtb"},
	{CNC_DID_TTL, CNC_REV_ANY, "tplatforms_cncttl3-%s.dtb"},
	{CNC_DID_ENDAT22, CNC_REV_ANY, "tplatforms_cncendat22-%s.dtb"},
	{CNC_DID_E1VPP, CNC_REV_ANY, "tplatforms_cnce1vpp-%s.dtb"}
};

static const char *cnc_get_fit_config(u16 did, u8 rev)
{
	struct cnc_dev_dtb *dtb;
	int idx;

	for (idx = 0; idx < ARRAY_SIZE(cnc_dtbs); idx++) {
		dtb = &cnc_dtbs[idx];

		if (dtb->did == did && (dtb->rev == rev || dtb->rev == CNC_REV_ANY)) {
			return dtb->name;
		}
	}

	return NULL;
}

int cnc_detect_board(void)
{
	char cfg[64], fdt[64];
	const char *dtb;
	int data;
	u16 did;
	u8 rev;

	data = pcf857x_get_val(CNC_SHRED_INFO);
	if (data < 0) {
		printf("CNC:   Can't read shred %d-%x\n",
			CNC_SHRED_I2C_BUS, CNC_SHRED_INFO);
		return -1;
	}
	did = CNC_SHRED_GET_DID(data);
	rev = CNC_SHRED_GET_REV(data);

	dtb = cnc_get_fit_config(did, rev);
	if (!dtb) {
		printf("CNC:   Can't find config for CNC %s rev.%hu\n",
			cnc_dev_name(did), (u16)rev);
		return -1;
	}

#if defined(CONFIG_TPLATFORMS_CNC_SFBT1)
	snprintf(fdt, sizeof(fdt), dtb, "sfbt1");
#else /* defined(CONFIG_TPLATFORMS_CNC_MSBT2) */
	snprintf(fdt, sizeof(fdt), dtb, "msbt2");
#endif

	snprintf(cfg, 64, "#conf@baikal_%s", fdt);
	setenv("multi_conf", cfg);
	setenv("fdt_file_name", fdt);
	printf("CNC:   %s rev.%hu detected\n", cnc_dev_name(did), (u16)rev);

	return 0;
}
