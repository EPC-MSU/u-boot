/*
 * File:        board.c
 * Description: T-platforms Axitech v1 SFBT1/MSBT2-based board initialization
 * Authors:     Sergey Semin <sergey.semin@t-platforms.ru>
 *
 * Copyright 2018 T-platforms JSC
 */

#include <common.h>
#include <asm/io.h>
#include <miiphy.h>
#include <netdev.h>
#include <pca953x.h>

#include <fru.h>

DECLARE_GLOBAL_DATA_PTR;

#define BIT(x)			(1<<(x))

#ifdef CONFIG_BOARD_EARLY_INIT_R
int board_early_init_r(void)
{
	return 0;
}
#endif /* CONFIG_BOARD_EARLY_INIT_R */

int board_pci_reset(void)
{
#if defined(CONFIG_PCIE_RST_PIN) && defined(CONFIG_TPLATFORMS_AXT1_MSBT2)
	int ret, pcie_mask = BIT(CONFIG_PCIE_RST_PIN);

	ret = pca953x_set_dir(CONFIG_SYS_SHRED_I2C_ADDR, pcie_mask, 0);
	if (ret) {
		printf("PCIe:  Can't set reset-GPIO dir\n");
		return ret;
	}

	ret = pca953x_set_val(CONFIG_SYS_SHRED_I2C_ADDR, pcie_mask, pcie_mask);
	if (!ret) {
		/* Need to create a pulse since the pca953x GPIO-expander doesn't have
		 * reset pin, so it isn't reset on restart. */
		udelay(500);
		ret = pca953x_set_val(CONFIG_SYS_SHRED_I2C_ADDR, pcie_mask, 0);
	}
	if (ret) {
		printf("PCIe:  Can't set reset-GPIO val\n");
		return ret;
	}
#endif

	return 0;
}

#ifdef CONFIG_BOARD_LATE_INIT
int board_late_init(void)
{
#ifdef CONFIG_TP_FRU
	fru_open_parse();
#endif

	/*
	 * If CONFIG_PCI is enabled board_pci_reset() will be called
	 * from pci_init_board()
	 */
#ifndef CONFIG_PCIE_DW
	board_pci_reset();
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
