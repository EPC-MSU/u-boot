/*
 * File:        board.c
 * Description: T-platforms MRBT1 Baikal-T1 based board initialization
 * Authors:     Sergey Semin <sergey.semin@t-platforms.ru>
 *
 * Copyright 2018 T-platforms JSC
 */

#include <common.h>
#include <asm/io.h>
#include <miiphy.h>
#include <netdev.h>
#include <asm/gpio.h>
#include <i2c.h>

#include <fru.h>
#include <bootconf.h>

DECLARE_GLOBAL_DATA_PTR;

#ifdef CONFIG_BOARD_EARLY_INIT_R
int board_early_init_r(void)
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
        if (memcmp(fru.bootdevice, disk2, strlen(disk2)) == 0) {
            setenv("sata_port", "1");
            printf("FRU: boot from %s\n", disk2);
        } else if (memcmp(fru.bootdevice, disk1, strlen(disk1)) == 0) {
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
    } else {
        sata_phy_init();
    }
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
    return (!err);
}
