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
#include <i2c.h>
#include <asm/io.h>

#include "fru.h"
#include "pvt.h"
#include "video.h"
#include "bootconf.h"
#include "board.h"

//#define debug printf

static int
tp_read_pcf8574(int bus, int addr) {
  int oldbus = i2c_get_bus_num();
  char val = 0;
  i2c_set_bus_num(bus);
  int ret = i2c_read(addr, 0, 0, (unsigned char*)&val, 1);
  i2c_set_bus_num(oldbus);
  if (ret!=0) {
    return -1;
  }
  return val;
}

/* Get CPU temperature routine */
static int do_service_temp(int argc, char * const argv[])
{
  printf("CPU temperature: %i\n", pvt_get_temp());
	return CMD_RET_SUCCESS;
}

/* Board type get / set routine */
static int do_service_board(int argc, char * const argv[])
{
  print_board_area(&fru);
  print_product_area(&fru);

	return CMD_RET_SUCCESS;
}

/* Board revision get / set routine */
static int do_service_revision(int argc, char * const argv[])
{
  printf("Part number: %s\n", fru.val_part_number);

	return CMD_RET_SUCCESS;
}

/* Board serial number get / set routine */
static int do_service_serial(int argc, char * const argv[])
{
	printf("Board S/N: %s\n", fru.val_serial_number);

	return CMD_RET_SUCCESS;
}

static int do_service_shred(int argc, char * const argv[])
{
  int ret = tp_read_pcf8574(CONFIG_SYS_SHRED_BUS_NUM, CONFIG_SYS_SHRED_I2C_ADDR);
  printf("SHRED: %i\n", ret);
  return CMD_RET_SUCCESS;
}

static int do_service_memory(int argc, char * const argv[])
{
  uint64_t high_mem = get_ddr_highmem_size();
  char buf[17];

  snprintf(buf, sizeof(buf), "%llx", high_mem);
  setenv("ddr_size", buf);
  setenv("accessible_mem", buf);
  printf("DDR size: %llu\n", high_mem);

  return CMD_RET_SUCCESS;
}

static int do_service_mmc_speed(int argc, char * const argv[])
{
  int ret = tp_read_pcf8574(CONFIG_SYS_SHRED_BUS_NUM, CONFIG_SYS_SHRED_I2C_ADDR);
  int mmc_speed=10000000;
  if (ret<4) {
    mmc_speed=1000000;
  }
  setenv_hex("mmc_speed", mmc_speed);
  printf("Recommended MMC speed: %ikHz\n", mmc_speed/1000);

	return CMD_RET_SUCCESS;
}

/* MAC addresess manipulation */
static int do_service_mac(int argc, char * const argv[])
{
	u32 iface;
	uchar enetaddr[6];
	char name[8];
	int i;

	if ((argc != 1) && (argc != 2))
		return CMD_RET_USAGE;

	iface = simple_strtoul(argv[0], NULL, 10);

	if (iface >= CONFIG_FRU_MAX_IFACES) {
		printf("Error: Invalid interface\n\n");
		return CMD_RET_USAGE;
	}

	if (argc == 2) {
		eth_parse_enetaddr(argv[1], enetaddr);

		fru_mrec_update_mac(&fru, enetaddr, iface);
		fru_update_mrec_eeprom();
	}

	switch (iface) {
	case 0:
		memcpy(enetaddr, fru.mac0, 6);
		break;
	case 1:
		memcpy(enetaddr, fru.mac1, 6);
		break;
	case 2:
		memcpy(enetaddr, fru.mac2, 6);
		break;
	default:
		return CMD_RET_FAILURE;
	}

	sprintf(name, iface ? "eth%daddr" : "ethaddr", iface);
	printf("%s: ", name);
	for (i = 0; i < 6; ++i)
		printf((i == 0) ? "%02x" : ":%02x", enetaddr[i]);
	printf("\n");

	return CMD_RET_SUCCESS;
}

static int do_service_bootconf(int argc, char * const argv[]) {
  char *end;
  char *addr;
  char *filesize;
  uint32_t bootconf_addr;
  uint32_t bootconf_len;
  char kernel_buf[64] = {0};
  char dtb_buf[64] = {0};
  char initrd_buf[64] = {0};
  char args_buf[2048] = {0};
  uint32_t loadaddr;
  uint32_t entryaddr;
#if defined(CONFIG_LONGPRESS) || defined(CONFIG_T_BOARD_MRBT1) || defined(CONFIG_T_BOARD_MSBT2)
  char *bootrecovery;
#endif
  int err;
#if !defined(CONFIG_LONGPRESS)
  if (fru.test_ok == 2) {
    vput_string_color("ERROR: BAD TEST DATA\n", RED);
    while (1) {};
  } else if (fru.test_ok == 0) {
    vput_string_color("ERROR: NO HW TEST DATA\n", RED);
    while (1) {};
  }
#endif

  if (argc == 2) {
    printf("Parse addr: %s, filesize: %s\n", argv[0], argv[1]);
    addr = argv[0];
    filesize = argv[1];
    bootconf_addr = simple_strtoul(addr, &end, 16);
    bootconf_len = simple_strtoul(filesize, &end, 16);
    if (bootconf_len == 0) {
#if defined(CONFIG_LONGPRESS) || defined(CONFIG_T_BOARD_MRBT1) || defined(CONFIG_T_BOARD_MSBT2)
      printf("Seems like bootconf was not found, fall back to recovery\n");
      tp_bmc_set_bootreason(BOOTCONF_FAIL, 0);
      printf("boot recovery\n");
      bootrecovery = getenv("bootrecovery");
      setenv("bootcmd", bootrecovery);
      run_command("boot", 0);
      return CMD_RET_SUCCESS;
#else
      vput_string_color("---------------------------------------------------------------\nsata info:\n", RED);
      run_command("sata info", 0);
      vput_string_color("---------------------------------------------------------------\nsata part:\n", RED);
      run_command("sata part", 0);
      vput_string_color("---------------------------------------------------------------\next4ls sata 0:\n", RED);
      run_command("ext4ls sata 0", 0);
      vput_string_color("ERROR: BOOTCONF not found or size is zero\n", RED);
      while (1) {};
#endif
    }
    err = tp_bootconf((char *)bootconf_addr, bootconf_len, kernel_buf, 64, &loadaddr, &entryaddr, dtb_buf, 64, initrd_buf, 64, args_buf, 2048);
    if (err) {
#if defined(CONFIG_LONGPRESS) || defined(CONFIG_T_BOARD_MRBT1) || defined(CONFIG_T_BOARD_MSBT2)
      printf("tp_bootconf returned abnormal code %i\n", err);
      printf("Fall back to recovery\n");
      tp_bmc_set_bootreason(BOOTCONF_FAIL, 0);
      printf("boot recovery\n");
      bootrecovery = getenv("bootrecovery");
      setenv("bootcmd", bootrecovery);
      run_command("boot", 0);
      return CMD_RET_SUCCESS;
#else
      char buf[64] = {};
      sprintf(buf, "ERROR: BOOTCONF parser returned %i\n", err);
      vput_string_color(buf, RED);
      while (1) {};
#endif
    }
    printf("Bootconf[kernel]:    %s\n", kernel_buf);
    printf("Bootconf[loadaddr]:  0x%08x\n", loadaddr);
    printf("Bootconf[entryaddr]: 0x%08x\n", entryaddr);
    printf("Bootconf[DTB]:       %s\n", dtb_buf);
    printf("Bootconf[initrd]:    %s\n", initrd_buf);
    printf("Bootconf[args]:      %s\n", args_buf);
    if ((strlen(kernel_buf)!=0) &&
        (strlen(dtb_buf)!=0) &&
        (strlen(initrd_buf)!=0) &&
        (strlen(args_buf)!=0)) {
      setenv("kernel_file_name", kernel_buf);
      setenv_hex("kernel_addr_n", entryaddr);
      setenv_hex("kernel_addr_r", entryaddr);
      setenv("initrd_file_name", initrd_buf);
      setenv("fdt_file_name", dtb_buf);
      setenv("bootargs", args_buf);
    }
  } else {
    printf("** service bootconf requires 2 arguments **\n");
  }
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

  if (strcmp(argv[1], "bootconf") == 0)
    return do_service_bootconf(argc-2, &argv[2]);

  if (strcmp(argv[1], "memory") == 0)
    return do_service_memory(argc-2, &argv[2]);

  if (strcmp(argv[1], "temp") == 0)
    return do_service_temp(argc-2, &argv[2]);

  if (strcmp(argv[1], "mmc_speed") == 0)
    return do_service_mmc_speed(argc-2, &argv[2]);

  if (strcmp(argv[1], "shred") == 0)
    return do_service_shred(argc-2, &argv[2]);

	return CMD_RET_USAGE;
}

U_BOOT_CMD(
    service , CONFIG_SYS_MAXARGS,    1,     do_service,
    "Board service maintaince command\n          (authorized personnel only)",
    "service serial         "
    "- get / set board serial number\n"
    "service board <type>   "
    "- get / set board type (0=BFK, 1=MODULE)\n"
    "service rev <revision> "
    "- get / set board revision (decimal, 18 digits max)\n"
    "service mac <iface_no> "
    "- manage mac address for <iface_no> (0-3)\n"
    "service temp           "
    "- get CPU temperature\n"
    "service memory         "
    "- load DDR info\n"
    "service mmc_speed      "
    "- deduce best MMC speed based on board info\n"
    "service shred          "
    "- load and display SHRE\n"
);
/* End of file cmd_service.c */
