/*
 * File:        board.c
 * Description: BFK Baikal-T* based boards initialization
 * Authors:     Dmitry Dunaev <dmitry.dunaev@baikalelectronics.ru>
 *              Ramil Zaripov <Ramil.Zaripov@baikalelectronics.com>
 *              Alexander Sazonov <Alexander.Sazonov@baikalelectronics.ru>
 * Copyright 2014-2018 Baikal Electronics JSC
 */


#include <common.h>
#include <asm/io.h>
#include <miiphy.h>
#include <i2c.h>
#include <netdev.h>
#include <malloc.h>
#include <asm/arch/sata.h>
#include "oem.h"
#include "nvram.h"
#include "../spi.h"

DECLARE_GLOBAL_DATA_PTR;

#define BRD15_BUS_NUM		0x01
#define BRD15_ADDR		0x51
#define BRD16_BUS_NUM		0x01
#define BRD16_ADDR		0x20

struct vendor_data vdata;

/* prototypes */
void designware_clear_phy_reset(ulong base);

/*
 * Get board revision
 */
int get_board_revision(void)
{
	int board_rev;

#ifndef CONFIG_BAIKAL_BFK3
	/* switch to correct I2C bus */
	int old_bus = I2C_GET_BUS();
	I2C_SET_BUS(BRD15_BUS_NUM);

	/* Read control value */
	if (i2c_probe(BRD15_ADDR) == 0)
		board_rev = 0x15;
	else
		board_rev = 0x10;

	I2C_SET_BUS(BRD16_BUS_NUM);
	if (i2c_probe(BRD16_ADDR) == 0)
		board_rev = 0x16;

	/* switch back to original I2C bus */
	I2C_SET_BUS(old_bus);
#else
	board_rev = 0x31;
#endif /* CONFIG_BAIKAL_BFK3 */

	return board_rev;
}

/* Init board nvram */
void board_nvram_init(void)
{
	/* Check NVRAM */
	if (nvram_init())
		return;
	/* ARM generic boot options address is used as ptr to NVRAM data */
	gd->bd->bi_nvram = malloc(nvram_get_size() * sizeof(uchar));
	if (gd->bd->bi_nvram)
		nvram_read_block(gd->bd->bi_nvram, nvram_get_size());
}

#ifdef CONFIG_BOARD_EARLY_INIT_R
int board_early_init_r(void)
{
	/* init NVRAM */
	board_nvram_init();

#ifndef CONFIG_BAIKAL_BFK3
	int updated = 0;

	/* Get vendor data from EEPROM */
	if (oem_get_vendor_data(&vdata) < 0) {
		vdata.crc = 0;
		vdata.type = 0xff;
		vdata.revision = 0xff;
		vdata.serial = 0x0;
	}
#else
	vdata.crc = 0;
	vdata.type = 0xff;
	vdata.revision = 0x31;
	vdata.serial = 0x0;
#endif /* CONFIG_BAIKAL_BFK3 */

	if (vdata.type == 0xff) {
		vdata.type = oem_get_vendor_board(CONFIG_BAIKAL_NAME);
#ifndef CONFIG_BAIKAL_BFK3
		updated = 1;
#endif /* CONFIG_BAIKAL_BFK3 */
	}

	if (vdata.revision == 0xff) {
		vdata.revision = get_board_revision() & 0xff;
#ifndef CONFIG_BAIKAL_BFK3
		updated = 1;
#endif /* CONFIG_BAIKAL_BFK3 */
	}

#ifndef CONFIG_BAIKAL_BFK3
	if (updated)
		oem_set_vendor_data(&vdata);
#endif /* CONFIG_BAIKAL_BFK3 */

	printf("Rev:   %u.%u\n", (vdata.revision >> 4) & 0x0f, vdata.revision & 0x0f);

	if (vdata.serial != 0) {
		printf("S/N:   %llu\n", vdata.serial);
	}

	/* Return success */
	return 0;
}
#endif /* CONFIG_BOARD_EARLY_INIT_R */

#ifdef CONFIG_BOARD_LATE_INIT

#ifdef CONFIG_BAIKAL_BFK3
#define ETH_ID_1G_1		1
#define ETH_ID_1G_2		2
#define ETH_ID_10G		0
void calc_and_set_macaddr(int serial, int rev)
{
	u64 macaddr;
	char buffer[11];
	u8* b=(u8*)&macaddr;

	/* Ethernet 10G (eth0) */
	macaddr = CONFIG_BAIKAL_MAC_BASE | ((serial << 2) & 0xFFFFFC) | ETH_ID_10G;
	sprintf(buffer,"%02x:%02x:%02x:%02x:%02x:%02x",b[5],b[4],b[3],b[2],b[1],b[0]);
	setenv("ethaddr", buffer);

	/* Ethernet 1G (eth1) */
	macaddr = CONFIG_BAIKAL_MAC_BASE | ((serial << 2) & 0xFFFFFC) | ETH_ID_1G_1;
	sprintf(buffer,"%02x:%02x:%02x:%02x:%02x:%02x",b[5],b[4],b[3],b[2],b[1],b[0]);
	setenv("eth1addr", buffer);

	/* Ethernet 1G (eth2) */
	macaddr = CONFIG_BAIKAL_MAC_BASE | ((serial << 2) & 0xFFFFFC) | ETH_ID_1G_2;
	sprintf(buffer,"%02x:%02x:%02x:%02x:%02x:%02x",b[5],b[4],b[3],b[2],b[1],b[0]);
	setenv("eth2addr", buffer);
}
#endif /* CONFIG_BAIKAL_BFK3 */

#define BOOT_FLASH_ID_LEN 20

int board_late_init(void)
{
	char buf[32];

	/* Set mac addresses */
#ifdef CONFIG_BAIKAL_BFK3
	/* Set mac addresses based on CPU serial and rev */
	char flash_chip_id[BOOT_FLASH_ID_LEN * 2 + 1];
	flash_chip_id[BOOT_FLASH_ID_LEN * 2] = '\0';
	unsigned long brd_serial_id = 0;
	uint8_t data[BOOT_FLASH_ID_LEN];
	int err = llenv32_spi_bootid(data, sizeof(data));
	if (err) {
		printf("Error: Failed to read boot flash ID (%d)\n", err);
		brd_serial_id = getenv_ulong("cpu_serial", 10, 0);
	} else {
		for (int i = 0; i < BOOT_FLASH_ID_LEN; i++)
			sprintf(flash_chip_id + i * 2, "%02x", data[i]);

		printf("RomID: %s\n", flash_chip_id);
		/* Last 6 bytes of SPI boot flash chip ID */
		brd_serial_id = simple_strtol(flash_chip_id + (BOOT_FLASH_ID_LEN - 6), NULL, 16);
	}
	calc_and_set_macaddr(brd_serial_id, getenv_ulong("cpu_rev", 10, 0));
#else
	oem_setall_macaddr();
#endif /* CONFIG_BAIKAL_BFK3 */

	/* Set board serial number */
	snprintf(buf, 20, "%llu", vdata.serial);
	setenv("board_serial", buf);

	/* Set board revision */
	snprintf(buf, 10, "%u.%u", (vdata.revision >> 4) & 0x0f,
			vdata.revision & 0x0f);
	setenv("board_rev", buf);

	/* Set board name */
	setenv("board_name", CONFIG_BAIKAL_NAME);

	/* Set default size of high memory size, value has been taken from SPD */
	sprintf(buf,"%x", get_ddr_highmem_size());
	setenv("memory_high_size", buf);

#ifdef CONFIG_SATA_PHY_INIT
	unsigned int phy = getenv_ulong("sataphy", 10, 0);

	/* Init SATA PHY */
	if (phy) {
	sata_phy_init_val(0, phy);
	sata_phy_init_val(1, phy);
	}
	else
	sata_phy_init();
	printf("SATA:  PHY init complete\n");
#endif

	return 0;
}
#endif /* CONFIG_BOARD_LATE_INIT */

/* Init SPI interface */
#if 0
void spi_init(void)
{
	/* Nothing todo here */
	return;
}
#endif

/* Initialization of network */
int board_eth_init(bd_t *bis)
{
	int err = 0;

#if defined(CONFIG_DESIGNWARE_ETH0_BASE)
	if (designware_initialize(CONFIG_DESIGNWARE_ETH0_BASE,
			  PHY_INTERFACE_MODE_GMII) < 0)
		err |= (1 << 0);
#endif /* CONFIG_DESIGNWARE_ETH0_BASE */
#if defined(CONFIG_DESIGNWARE_ETH1_BASE)
	if (designware_initialize(CONFIG_DESIGNWARE_ETH1_BASE,
			  PHY_INTERFACE_MODE_GMII) < 0)
		err |= (1 << 1);
#endif /* CONFIG_DESIGNWARE_ETH1_BASE */
#if defined(CONFIG_DESIGNWARE_ETH2_BASE)
	if (designware_initialize(CONFIG_DESIGNWARE_ETH2_BASE,
			  PHY_INTERFACE_MODE_GMII) < 0)
		err |= (1 << 1);
#endif /* CONFIG_DESIGNWARE_ETH2_BASE */
	return (! err);
}
