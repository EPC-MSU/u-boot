/*
 * File:        board.c
 * Description: T-platforms MITX Baikal-T1 based board initialization
 * Author:      Konstantin Kirik <dmitry.dunaev@baikalelectronics.ru>
 *
 * Copyright 2018 T-platforms JSC
 */

#include <common.h>
#include <environment.h>
#include <spi_flash.h>
#include <asm/mipsregs.h>
#include <asm/addrspace.h>
#include <asm/io.h>
#include <miiphy.h>
//#include <sata.h>
#include <i2c.h>
#include <netdev.h>
#include <malloc.h>
#include <asm/gpio.h>
#include <asm/arch/sata.h>
#include <asm/arch/clock.h>
#include <asm/arch/clock_manager.h>

#include "fru.h"
#include "bootconf.h"

DECLARE_GLOBAL_DATA_PTR;

static int board_usb_config(void)
{
    uint8_t def_val[256] = {0};
    int err;
    uint8_t tmp[17];
    int start = 0;
    int gpio_usb_reset = 13;

    /* reset USB hub and configure it */
    debug("Reset and configure USB hub: ");
    err = gpio_request(gpio_usb_reset, "usb_reset");
    if (err) {
        printf("Failed to request GPIO %d (ret %d)\n", gpio_usb_reset, err);
        return err;
    }
    gpio_direction_output(gpio_usb_reset, 1);
    gpio_free(gpio_usb_reset);
    udelay(1000);

    def_val[0] = 0x24;
    def_val[1] = 0x04;
    def_val[2] = 0x17;
    def_val[3] = 0x25;
    def_val[4] = 0x00;
    def_val[5] = 0x00;
    def_val[6] = 0x9b;
    def_val[7] = 0x20;
    def_val[8] = 0x00;
    def_val[9] = 0x00;
    def_val[10] = 0x00;
    def_val[11] = 0x00;
    def_val[12] = 0x32;
    def_val[13] = 0x32;
    def_val[14] = 0x32;
    def_val[15] = 0x32;
    def_val[16] = 0x32;
    def_val[255] = 1;

    i2c_set_bus_num(1);
    for (; start<256; start += 16) {
        memcpy(tmp + 1, def_val + start, 16);
        tmp[0] = 16;
        err = i2c_write(0x2c, start, 1, tmp, 17);
        if (err)
            printf("i2c_write[%i] 0x2c returned %i\n", start, err);
    }
    mdelay(3);
    debug("Done\n");

    return 0;
}

int board_pci_reset(void)
{
    int ret, delay;

    delay = getenv_ulong("pci_delay", 10, 0);
    if (delay > 1000)
        delay = 1000;
    ret = gpio_request(CONFIG_PCIE_RST_PIN, "pcie_rst");
    if (ret) {
        printf("PCIe:  failed to request GPIO %d (ret %d)\n", CONFIG_PCIE_RST_PIN, ret);
        return ret;
    }
    gpio_direction_output(CONFIG_PCIE_RST_PIN, 1);
    mdelay(delay);

    return 0;
}

#ifdef CONFIG_BOARD_EARLY_INIT_R
int board_early_init_r(void)
{
    unsigned int gpio_hdd_led = 18;
    int ret;

    board_usb_config();

    ret = gpio_request(gpio_hdd_led, "hdd_led");
    if (ret) {
        printf("Failed to request GPIO %i; ret: %i\n", gpio_hdd_led, ret);
    } else {
        gpio_direction_output(gpio_hdd_led, 1);
        gpio_free(gpio_hdd_led);
    }
    /*
     * If CONFIG_PCI is enabled board_pci_reset() will be called
     * from pci_init_board()
     */
#ifndef CONFIG_PCIE_DW
    board_pci_reset();
#endif
    /* Return success */
    return 0;
}
#endif /* CONFIG_BOARD_EARLY_INIT_R */

#ifdef CONFIG_BOARD_LATE_INIT
int board_late_init(void)
{
    fru_open_parse();
    tp_bmc_get_version();
    tp_check_boot();

    setenv("sata_port", "0");
    if (fru.bootdevice[0] != 0) {
        static const char disk1[] = "sata0:0";
        static const char disk2[] = "sata0:1";
        if (memcmp(fru.bootdevice, disk2, strlen(disk2))==0) {
            setenv("sata_port", "1");
            printf("FRU: boot from %s\n", disk2);
        } else if (memcmp(fru.bootdevice, disk1, strlen(disk1))==0) {
            printf("FRU: boot from %s\n", disk1);
        } else {
            printf("FRU: unknown boot device %s, falling back to %s\n", fru.bootdevice, disk1);
        }
    }

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

/* Initialization of network */
int board_eth_init(bd_t *bis)
{
    int err = 0;

#if defined(CONFIG_DESIGNWARE_ETH0_BASE)
    if (designware_initialize(CONFIG_DESIGNWARE_ETH0_BASE,
                              PHY_INTERFACE_MODE_RGMII) < 0)
        err |= (1 << 0);
#endif /* CONFIG_DESIGNWARE_ETH0_BASE */
#if defined(CONFIG_DESIGNWARE_ETH1_BASE)
    if (designware_initialize(CONFIG_DESIGNWARE_ETH1_BASE,
                              PHY_INTERFACE_MODE_RGMII) < 0)
        err |= (1 << 1);
#endif /* CONFIG_DESIGNWARE_ETH1_BASE */
#if defined(CONFIG_DESIGNWARE_ETH2_BASE)
    if (designware_initialize(CONFIG_DESIGNWARE_ETH2_BASE,
                              PHY_INTERFACE_MODE_RGMII) < 0)
        err |= (1 << 1);
#endif /* CONFIG_DESIGNWARE_ETH2_BASE */
    return (! err);
}
