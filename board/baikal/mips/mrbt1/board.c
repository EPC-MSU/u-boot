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

#include <fru.h>

DECLARE_GLOBAL_DATA_PTR;

#ifdef CONFIG_BOARD_EARLY_INIT_R
int board_early_init_r(void)
{
	return 0;
}
#endif /* CONFIG_BOARD_EARLY_INIT_R */

static int board_usb_reset(void)
{
    int err;
    int gpio_usb_reset = 13;

    /* reset USB hub and configure it */
    debug("Reset USB: ");
    err = gpio_request(gpio_usb_reset, "usb_reset");
    if (err) {
        printf("Failed to request GPIO %d (ret %d)\n", gpio_usb_reset, err);
        return err;
    }
    gpio_direction_output(gpio_usb_reset, 1);
    gpio_free(gpio_usb_reset);
    mdelay(3);
    debug("Done\n");

    return 0;
}

#ifdef CONFIG_BOARD_LATE_INIT
int board_late_init(void)
{
	fru_open_parse();
	board_usb_reset();

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
