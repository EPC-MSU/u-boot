/*
 * File:        mem.c
 * Description: Baikal-T SoC various memory initializations
 * Authors:     Dmitry Dunaev <dmitry.dunaev@baikalelectronics.ru>
 *              Ramil Zaripov <Ramil.Zaripov@baikalelectronics.com>
 *              Alexander Sazonov <Alexander.Sazonov@baikalelectronics.ru>
 * Copyright 2014-2018 Baikal Electronics JSC
 */

#include <common.h>
#include <environment.h>
#include <asm/mipsregs.h>
#include <asm/addrspace.h>
#include <asm/io.h>
#include <i2c.h>
#include <malloc.h>
#include <board.h>
#include <config.h>
#include "llenv_spd.h"
#include "spi.h"

DECLARE_GLOBAL_DATA_PTR;

extern uint8_t  *const ddr_buffer0;
extern uint32_t *const ddr_buffer1;

#ifdef CONFIG_BAIKAL_SPD_ADDRESS
void i2c_hw_init(int speed, int slaveaddr);

int baikal_read_spd(uint32_t addr, int alen, uint8_t *buffer, int len)
{
	int ret;

	/* Init I2C */
	i2c_hw_init(CONFIG_SYS_I2C_SPEED, CONFIG_SYS_I2C_SLAVE);

	/* Init timer */
	(*(volatile unsigned long *)(CONFIG_SRAM_SPD_TIMESTAMP)) = 0;
	write_c0_cause(0);

	ret = i2c_read(CONFIG_BAIKAL_SPD_ADDRESS, addr, alen, buffer, len);
	*((int *)(CONFIG_DDR_INIT_RESULT_i2c)) = ret;

	return ret;
}
#endif

#define DDR3_SEG_SIZE		256

uint64_t get_ddr_highmem_size(void)
{
	uint64_t size;
	size = ddr_buffer1 [DDR3_SPD_SARSIZE1] + 1;
	size *= DDR3_SEG_SIZE * 1024 * 1024;

	return size;
}

uint32_t get_ddr_rank(void)
{
	if (ddr_buffer1 [DDR3_SPD_ADDRMAP0] == 0x1f)
		return 1;
	else
		return 2;
}

void print_ddr_regs (void)
{
	int i;
	uint32_t *p = ddr_buffer1;
	printf("DDRC/PUB reg base = 0x%x\n", (uint32_t)p);
	for (i = 0; i < DDR_SPD_LAST; i++) {
		printf("reg(%02d) = 0x%08x\n", i, p[i]);
	}
}

void print_ddr_spd (void)
{
	int i;
	uint8_t *p = (uint8_t *)ddr_buffer0;
	printf("DDR SPD base = 0x%x\n", (uint32_t)p);
	for (i = 0; i < 256; i+= 2) {
		if ((i % 16) == 0)
			printf("\nspd(%02d):\t", i);
		printf("%02x%02x ", p[i], p[i+1]);
	}
	printf("\n");
}

phys_size_t initdram(int board_type)
{
	uint32_t rank = get_ddr_rank();
	uint32_t mem = (ddr_buffer1[DDR3_SPD_SARSIZE1] + 1)*DDR3_SEG_SIZE;
	uint32_t ecc = ddr_buffer1[DDR3_SPD_ECCCFG0];

#ifdef CONFIG_BAIKAL_PRINT_DDR
	print_ddr_regs();
#endif /* CONFIG_BAIKAL_PRINT_DDR*/
#ifdef CONFIG_BAIKAL_PRINT_SPD
	print_ddr_spd();
#endif /* CONFIG_BAIKAL_PRINT_SPD */

	printf("%s SPD, Ranks = %u, %shighmem = %u MiB, lowmem = ",
		ddr_buffer0 + 128, rank, ecc ? "ECC, " : "", mem);
	if (*((int *)(CONFIG_DDR_INIT_RESULT_v0)))
	{
		printf( /*lowmem*/ "...\n");
		printf("Baikal FW: DDR init failed v0=0x%x, v1=0x%x, step=0x%x, i2c=0x%x\n",
			*((int *)(CONFIG_DDR_INIT_RESULT_v0)),
			*((int *)(CONFIG_DDR_INIT_RESULT_v1)),
			*((int *)(CONFIG_DDR_INIT_RESULT_t7)),
			*((int *)(CONFIG_DDR_INIT_RESULT_i2c))
		);

#ifndef CONFIG_BAIKAL_FW_CONTINUE_ON_DDR_FAIL
		return 0;
#endif /* CONFIG_BAIKAL_FW_CONTINUE_ON_DDR_FAIL */
	}
	return CONFIG_SYS_SDRAM_SIZE;   /* Only one bank on lower 512 MB can be reached without MMU */
}

void board_add_ram_info(int use_default)
{
	bd_t *bd = gd->bd;

	/* Lowlevel DDR init should be here */
	bd->bi_boot_params = CONFIG_SYS_SDRAM_BASE + 100;
	bd->bi_memstart = CONFIG_SYS_SDRAM_BASE;
	bd->bi_memsize = get_ram_size((void *)CONFIG_SYS_SDRAM_BASE,
					CONFIG_SYS_SDRAM_SIZE);

#ifdef CONFIG_SYS_SRAM_BASE
	bd->bi_sramstart = CONFIG_SYS_SRAM_BASE;
	bd->bi_sramsize = get_ram_size((void *)CONFIG_SYS_SRAM_BASE,
					CONFIG_SYS_SRAM_SIZE);
#endif


	bd->bi_flashstart = CONFIG_SYS_TEXT_BASE;
	bd->bi_flashsize = KSEG1 - KSEG0 - CPHYSADDR(CONFIG_SYS_TEXT_BASE);
	bd->bi_flashoffset = (CONFIG_SYS_MONITOR_BASE - CONFIG_SYS_TEXT_BASE);
}

/*
 * Show board name at startup
 */
int checkboard(void)
{
#if defined(CONFIG_BAIKAL_T1)
	puts("Board: Baikal-T1 " CONFIG_BAIKAL_NAME "\n");
#else
	puts("Board: Baikal-T " CONFIG_BAIKAL_NAME "\n");
#endif
	return 0;
}

#if defined(CONFIG_LLENV32_SAVEENV)
int saveenv(void)
{
	llenv32_boot_ctrl(1);
	mdelay(100);

	int port = 0, line = 0;
	int ret = 0;

	void *cfg;
	env_t *env;

	/* spi config */
	cfg = llenv32_spi_store_cfg(port);
	if (!cfg) {
		ret = -1;
		return ret;
	}
	llenv32_spi_init(port);

	/* prepare */
	env = malloc(SPI_SECTOR_SIZE);
	if (!env) {
		ret = -2;
		goto _exit;
	}

	/* erase */
	if (llenv32_spi_erase(port, line, CONFIG_ENV_OFFSET)) {
		ret = -3;
		goto _exit;
	}

	/* export */
	if (env_export(env)) {
		ret = -4;
		goto _exit;
	}

	/* write */
	if (llenv32_spi_write_sector(port, line, CONFIG_ENV_OFFSET, env)) {
		ret = -5;
		goto _exit;
	}

_exit:
	if (llenv32_spi_restore_cfg (port, cfg)) {
		ret = -8;
	}

	if (env) {
		free(env);
	}

	llenv32_boot_ctrl(0);

	return ret;
}

#elif defined(CONFIG_BOOT_BASE)

#define BE_BC_CSR		0x00
#define BE_BC_MAR		0x04
#define BE_BC_DRID		0x08
#define BE_BC_VID		0x0C
#define BC_CSR_SPI_RDA	(1 << 8)
#define BC_CSR_BMODE	((1 << 0) | (1 <<1))

static struct spi_flash *env_flash;
int saveenv(void)
{
	int ret = 1;
	unsigned int reg;
	env_t env_new;

	/* Switch to SPI mode */
	reg = readl(CONFIG_BOOT_BASE + BE_BC_CSR);
	writel(reg | BC_CSR_SPI_RDA, CONFIG_BOOT_BASE + BE_BC_CSR);
	wmb();

	/* Probe flash if needed */
	if (!env_flash) {
		env_flash = spi_flash_probe(CONFIG_ENV_SPI_BUS,
			CONFIG_ENV_SPI_CS,
			CONFIG_ENV_SPI_MAX_HZ, CONFIG_ENV_SPI_MODE);
		if (!env_flash) {
			puts("Boot flash probe failed!\n");
			return 1;
		}
	}

	/* Get enviroment */
	ret = env_export(&env_new);
	if (ret)
		goto __done;

	/* Erase enviroment region */
	printf("spi_flash_erase: ofs=%x  size=%x\n", CONFIG_ENV_OFFSET, CONFIG_ENV_SIZE);
	puts("Erasing SPI flash...");
	ret = spi_flash_erase(env_flash, CONFIG_ENV_OFFSET, CONFIG_ENV_SIZE);
	if (ret)
		goto __done;
	puts("done\n");

	/* Write enviroment region */
	printf("spi_flash_write: ofs=%x  size=%x\n", CONFIG_ENV_OFFSET, CONFIG_ENV_SIZE);
	puts("Writing to SPI flash...");
		ret = spi_flash_write(env_flash, CONFIG_ENV_OFFSET, CONFIG_ENV_SIZE, &env_new);
	if (ret)
		goto __done;
	puts("done\n");

__done:
	/* Switch to transparent mode */
	writel(reg, CONFIG_BOOT_BASE + BE_BC_CSR);
	wmb();
	sync();
	/* Return result */
	return ret;
}

#else
int saveenv(void)
{
	puts("Flash is in read-only mode. Enviroment save is disabled.\n");
	return 1;
}
#endif

#ifdef CONFIG_CMD_EEPROM
int eeprom_write_enable (unsigned dev_addr, int state)
{
	return 0;
}
#endif
