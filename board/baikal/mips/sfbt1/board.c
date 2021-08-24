/*
 * File:        board.c
 * Description: T-platforms SFBT1 Baikal-T based board initialization
 * Authors:     Sergey Semin <sergey.semin@t-platforms.ru>
 *
 * Copyright 2018 T-platforms JSC
 */

#include <common.h>
#include <asm/io.h>
#include <miiphy.h>
#include <netdev.h>

DECLARE_GLOBAL_DATA_PTR;

#ifdef CONFIG_BOARD_EARLY_INIT_R
int board_early_init_r(void)
{
	return 0;
}
#endif /* CONFIG_BOARD_EARLY_INIT_R */

int board_pci_reset(void)
{
	return 0;
}

#ifdef CONFIG_BOARD_LATE_INIT
int board_late_init(void)
{
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
