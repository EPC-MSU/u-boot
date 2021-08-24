/*
 * File:        board.c
 * Description: T-platforms Axitech BC3BT1 MRBT1-based board initialization
 * Authors:     Sergey Semin <sergey.semin@t-platforms.ru>
 *
 * Copyright 2018 T-platforms JSC
 */

#include <common.h>
#include <asm/io.h>
#include <miiphy.h>
#include <netdev.h>
#include <pca953x.h>
#include <asm/gpio.h>
#include <i2c.h>

#include <fru.h>

DECLARE_GLOBAL_DATA_PTR;

#define BIT(x)			(1<<(x))

#ifdef CONFIG_BOARD_EARLY_INIT_R
int board_early_init_r(void)
{
	return 0;
}
#endif /* CONFIG_BOARD_EARLY_INIT_R */

static int board_usb_reset(void)
{
#if defined(CONFIG_USB_RST_PIN)
	int ret;

	ret = gpio_request(CONFIG_USB_RST_PIN, "usb_reset");
	if (ret) {
		printf("USB:   Failed to request GPIO %d (err %d)\n", CONFIG_USB_RST_PIN, ret);
		return ret;
	}

	gpio_direction_output(CONFIG_USB_RST_PIN, 1);
	gpio_free(CONFIG_USB_RST_PIN);
	mdelay(3);
#endif

	return 0;
}

int board_sd_reset(void)
{
#if defined(CONFIG_SYS_CTRL_SD_RST_PIN)
	int ret, sd_mask = BIT(CONFIG_SYS_CTRL_SD_RST_PIN);

	ret = pca953x_set_val(CONFIG_SYS_CTRL_I2C_ADDR, sd_mask, 0);
	if (ret) {
		printf("SD:    Can't init reset-GPIO val (err %d)\n", ret);
		return ret;
	}

	ret = pca953x_set_dir(CONFIG_SYS_CTRL_I2C_ADDR, sd_mask, 0);
	if (ret) {
		printf("SD:    Can't set reset-GPIO dir (err %d)\n", ret);
		return ret;
	}

	/* Need to create a pulse since the pca953x GPIO-expander doesn't have
	 * reset pin, so it isn't reset on restart. */
	udelay(500);
	ret = pca953x_set_val(CONFIG_SYS_CTRL_I2C_ADDR, sd_mask, sd_mask);
	if (ret) {
		printf("SD:    Can't set reset-GPIO val (err %d)\n", ret);
		return ret;
	}
#endif

	return 0;
}

static uint8_t usb2513_fw[256] = {
	0x24, 0x04, 0x13, 0x25, 0xb3, 0x0b, 0x9b, 0x20, 0x02, 0x06, 0x00, 0x00, 0x32, 0xfa, 0x32, 0xfa,
	0x32, 0x00, 0x00, 0x09, 0x09, 0x00, 0x4d, 0x00, 0x69, 0x00, 0x63, 0x00, 0x72, 0x00, 0x6f, 0x00,
	0x63, 0x00, 0x68, 0x00, 0x69, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x55, 0x00, 0x53, 0x00, 0x42, 0x00, 0x32, 0x00, 0x35, 0x00, 0x31, 0x00,
	0x33, 0x00, 0x42, 0x00, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01
};

int board_hub_reset(void)
{
	int ret = 0;

#if defined(CONFIG_SYS_USB2513_I2C_ADDR)
	unsigned int old;
	uint8_t buf[17];
	int offset;

	ret = gpio_request(CONFIG_SYS_USB2513_RST_PIN, "hub_reset");
	if (ret) {
		printf("HUB:   Failed to request GPIO %d (err %d)\n", CONFIG_SYS_USB2513_RST_PIN, ret);
		return ret;
	}

	gpio_direction_output(CONFIG_SYS_USB2513_RST_PIN, 1);
	gpio_free(CONFIG_SYS_USB2513_RST_PIN);
	udelay(1000);

	old = i2c_get_bus_num();
	ret = i2c_set_bus_num(CONFIG_SYS_USB2513_BUS_NUM);
	if (ret) {
		printf("HUB:   Failed to set i2c bus %d (err %d)\n", CONFIG_SYS_USB2513_BUS_NUM, ret);
		return ret;
	}
	for (offset = 0; offset < 256; offset += 16) {
		buf[0] = 16;
	        memcpy(buf + 1, usb2513_fw + offset, 16);

		ret = i2c_write(CONFIG_SYS_USB2513_I2C_ADDR, offset, 1, buf, 17);
		if (ret) {
			printf("HUB:   Failed to send data at offset %d (err %d)\n", offset, ret);
			goto err_set_buf_num;
		}
	}

err_set_buf_num:
	i2c_set_bus_num(old);

	mdelay(3);
#endif

	return ret;
}

int board_pd_reset(void)
{
	int ret = 0, mask;

#if defined(CONFIG_SYS_CTRL_FTDI_RST_PIN)
	mask = BIT(CONFIG_SYS_CTRL_FTDI_RST_PIN);

	ret = pca953x_set_val(CONFIG_SYS_CTRL_I2C_ADDR, mask, 0);
	if (ret) {
		printf("PD:    Can't init FTDI reset-GPIO val (err %d)\n", ret);
		return ret;
	}

	ret = pca953x_set_dir(CONFIG_SYS_CTRL_I2C_ADDR, mask, 0);
	if (ret) {
		printf("PD:    Can't set FTDI reset-GPIO dir (err %d)\n", ret);
		return ret;
	}

	/* Need to create a pulse since the pca953x GPIO-expander doesn't have
	 * reset pin, so it isn't reset on restart. */
	udelay(500);
	ret = pca953x_set_val(CONFIG_SYS_CTRL_I2C_ADDR, mask, mask);
	if (ret) {
		printf("PD:    Can't set FTDI reset-GPIO val (err %d)\n", ret);
		return ret;
	}
#endif

#if defined(CONFIG_SYS_CTRL_SIM_SEL_PIN)
	mask = BIT(CONFIG_SYS_CTRL_SIM_SEL_PIN);

	ret = pca953x_set_val(CONFIG_SYS_CTRL_I2C_ADDR, mask, 0);
	if (ret) {
		printf("PD:    Can't set SIM-select GPIO val (err %d)\n", ret);
		return ret;
	}

	ret = pca953x_set_dir(CONFIG_SYS_CTRL_I2C_ADDR, mask, 0);
	if (ret) {
		printf("PD:    Can't set SIM-select GPIO dir (err %d)\n", ret);
		return ret;
	}
#endif

#if defined(CONFIG_SYS_GSM_PWR_OFF_PIN)
	ret = gpio_request(CONFIG_SYS_GSM_PWR_OFF_PIN, "gsm_pwr_off");
	if (ret) {
		printf("PD:    Failed to request GSM PWR OFF GPIO %d (err %d)\n", CONFIG_SYS_GSM_PWR_OFF_PIN, ret);
		return ret;
	}

	gpio_direction_output(CONFIG_SYS_GSM_PWR_OFF_PIN, 1);
	gpio_free(CONFIG_SYS_GSM_PWR_OFF_PIN);
#endif

	return ret;
}

#ifdef CONFIG_BOARD_LATE_INIT
int board_late_init(void)
{
	int ret = 0;

	fru_open_parse();

	ret = board_usb_reset();
	if (ret)
		goto err_ret;

	ret = board_sd_reset();
	if (ret)
		goto err_ret;

	ret = board_hub_reset();
	if (ret)
		goto err_ret;

	ret = board_pd_reset();

err_ret:
	return ret;
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
