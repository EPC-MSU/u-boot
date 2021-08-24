/*
 * File:        cmd_phy.с
 * Description: SATA Phy tuning commands
 * Created:     06 Jul 2015
 * Author:      Dmitry Dunaev <dmitry.dunaev@baikalelectronics.ru>
 * Copyright 2015 Baikal Electronics
 *
 */

#include <common.h>
#include <command.h>
#include <asm/io.h>
#include <asm/arch/sata.h>

#ifdef CONFIG_CMD_SATA_PHY

static inline unsigned int sata_phy_read(unsigned int ofs)
{
	return readl(CONFIG_DWC_AHSATA_BASE_ADDR + ofs);
}

static inline void sata_phy_write(unsigned int ofs, unsigned int val)
{
	writel(val, CONFIG_DWC_AHSATA_BASE_ADDR + ofs);
}

void sata_phy_init_val(int phyid, unsigned int val)
{
    unsigned int ofs = phyid ? DW_SATA_P1PHYCR_OFS : DW_SATA_P0PHYCR_OFS;

    printf("SATA phy init: phy id %d, value %08x\n", phyid, val);

    sata_phy_write(ofs, val | (DW_SATA_PHYCR_PRST_MASK));
    sync();
    udelay(1000);
    sata_phy_write(ofs, val & (~DW_SATA_PHYCR_PRST_MASK));
    sync();
}

void sata_phy_init()
{
    sata_phy_init_val(0, DW_SATA_PHYCR_INIT);
    sata_phy_init_val(1, DW_SATA_PHYCR_INIT);
}

static int usage(void)
{
	puts("sata phy <channel> <command>\n");
	puts("  - sata phy tuning subsystem\n\n");
	puts("  channel:\n");
	puts("\taffected SATA channel (0 or 1)\n\n");
	puts("  commands:\n");
	puts("\tstatus  - return current phy status for specified channel\n");
	puts("\tdefault - set default phy settings for specified channel\n");
	puts("\tset     - set phy parameter for specified channel\n\n");
	puts("  parameters:\n");
	puts("\tprst   - Independed channel reset\n");
	puts("\t         Values: 0 or 1    (Default: 0)\n");
	puts("\ttxinv  - Invert TX signal polarity\n");
	puts("\t         Values: 0 to 1    (Default: 0)\n");
	puts("\trxinv  - Invert RX signal polarity\n");
	puts("\t         Values: 0 to 1    (Default: 0)\n");
	puts("\trxeq   - RX equalaizer settings\n");
	puts("\t         Values: 0 to 7    (Default: 0)\n");
	puts("\ttxpree - TX driver de-emphasis value\n");
	puts("\t         Values: 0 to 127  (Default: 35)\n");
	puts("\ttxampl - TX driver launch amplitude\n");
	puts("\t         Values: 0 to 127   (Default: 127)\n");
	puts("\n");

    return CMD_RET_SUCCESS;
}

#define ONOFF(NM, R) (DW_SATA_PHYCR_GET(NM, R)) ? "ON" : "OFF"
static int sata_phy_status(int ch)
{
	unsigned int val, reg = sata_phy_read(ch ? DW_SATA_P0PHYCR_OFS :
								DW_SATA_P1PHYCR_OFS);

	printf("\nSATA PHY channel %u status:\n\n", ch);

	printf("  Independed channel reset:    %s\n", ONOFF(PRST, reg));
	printf("  Invert TX signal polarity:   %s\n", ONOFF(TXINV, reg));
	printf("  Invert RX signal polarity:   %s\n", ONOFF(RXINV, reg));

	val = DW_SATA_PHYCR_GET(RXEQ, reg);
	printf("  RX equalaizer settings:      %3u (0x%02x)\n", val, val);

	val = DW_SATA_PHYCR_GET(TXPREE, reg);
	printf("  TX driver de-emphasis value: %3u (0x%02x)\n", val, val);

	val = DW_SATA_PHYCR_GET(TXAMPL, reg);
	printf("  TX driver launch amplitude:  %3u (0x%02x)\n\n", val, val);

	printf("  Raw register value: 0x%08x\n\n", reg);

	return CMD_RET_SUCCESS;
}

static int sata_phy_default(int ch)
{
	sata_phy_write(ch ? DW_SATA_P0PHYCR_OFS :
				DW_SATA_P1PHYCR_OFS, DW_SATA_PHYCR_DEFAULT);

	return CMD_RET_SUCCESS;
}

static int sata_phy_set(int ch, char *param, char *value)
{
	unsigned int val = simple_strtoul(value, NULL, 10);
	unsigned int reg = sata_phy_read(ch ? DW_SATA_P0PHYCR_OFS :
								DW_SATA_P1PHYCR_OFS);

	if ((strncmp(param, "prst",   4) == 0) && (val < 2))
		reg = DW_SATA_PHYCR_SET(PRST, reg, val);

	else if ((strncmp(param, "txinv",  5) == 0) && (val < 2))
		reg = DW_SATA_PHYCR_SET(TXINV, reg, val);

	else if ((strncmp(param, "rxinv",  5) == 0) && (val < 2))
		reg = DW_SATA_PHYCR_SET(RXINV, reg, val);

	else if ((strncmp(param, "rxeq",   4) == 0) && (val < 8))
		reg = DW_SATA_PHYCR_SET(RXEQ, reg, val);

	else if ((strncmp(param, "txpree", 6) == 0) && (val < 128))
		reg = DW_SATA_PHYCR_SET(TXPREE, reg, val);

	else if ((strncmp(param, "txampl", 6) == 0) && (val < 128))
		reg = DW_SATA_PHYCR_SET(TXAMPL, reg, val);

	else
		return usage();

	sata_phy_write(ch ? DW_SATA_P0PHYCR_OFS : DW_SATA_P1PHYCR_OFS, reg);

	printf("New SATA channel %u PHY register value: 0x%08x\n", ch, reg);

	return CMD_RET_SUCCESS;
}

int do_sata_phy(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	unsigned int ch;

	if (argc < 3)
		return usage();

	ch = simple_strtoul(argv[1], NULL, 10);

	if (ch > 2)
		return usage();

	if (strncmp(argv[2], "status", 6) == 0)
		return sata_phy_status(ch);

	if (strncmp(argv[2], "default", 7) == 0)
		return sata_phy_default(ch);

	if ((argc == 5) && (strncmp(argv[2], "set", 3) == 0))
		return sata_phy_set(ch, argv[3], argv[4]);

	return usage();
}

#endif /* CONFIG_CMD_SATA_PHY */
/* End of file cmd_phy.c */
