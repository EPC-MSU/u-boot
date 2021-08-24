/*
 * Copyright (C) 2015
 * Dmitry Dunaev <dmitry.dunaev@baikalelectronics.ru>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#include <common.h>
#include <asm/gpio.h>
#include <asm/io.h>

#define GPIO_INPUT  			0x00
#define GPIO_OUTPUT 			0x01

#define GPIO_PORT_DR  			0x00
#define GPIO_PORT_DDR 			0x04
#define GPIO_PORT_CTL 			0x0C
#define GPIO_PORT_INTEN			0x30
#define GPIO_PORT_INTMASK		0x34
#define GPIO_PORT_INTTYPE_LEVEL	0x38
#define GPIO_PORT_INT_POLARITY	0x3c
#define GPIO_PORT_INTSTATUS		0x40
#define GPIO_PORT_DEBOUNCE		0x48
#define GPIO_PORT_EOI			0x4c
#define GPIO_PORT_EXT			0x50

#define GPIO_PORT(x)		(((x) >> 5) & 0x3)
/* Assume 32 line for each port */
#define GPIO_BIT(x)		((x) & 0x1f)

#define GPIO_PORT_BASE(x) (CONFIG_DW_GPIO_BASE | (GPIO_PORT(gpio) * 0x0c))
#define GPIO_PORT_REG(x, ofs) (GPIO_PORT_BASE(x) + ofs)

static inline unsigned int
dw_readl(unsigned int gpio, unsigned int ofs)
{
	return readl(GPIO_PORT_REG(gpio, ofs));
}

static inline void
dw_writel(unsigned int val, unsigned int gpio, unsigned int ofs)
{
	writel(val, GPIO_PORT_REG(gpio, ofs));
}

int gpio_set_value(unsigned gpio, int value)
{
	unsigned int reg = dw_readl(gpio, GPIO_PORT_DR);

	if (value)
		reg |= (1 << GPIO_BIT(gpio));
	else
		reg &= ~(1 << GPIO_BIT(gpio));

	dw_writel(reg, gpio, GPIO_PORT_DR);

	debug("%s: gpio=%i  value=%i\n", __func__, gpio, value);

	return 0;
}

int gpio_get_value(unsigned gpio)
{
    return !!(dw_readl(gpio, GPIO_PORT_EXT) & (1 << GPIO_BIT(gpio)));
}

int gpio_request(unsigned gpio, const char *label)
{
	return 0;
}

int gpio_free(unsigned gpio)
{
	return 0;
}

int gpio_direction_input(unsigned gpio)
{
	unsigned int reg = dw_readl(gpio, GPIO_PORT_DDR);

	reg &= ~(1 << GPIO_BIT(gpio));
	dw_writel(reg, gpio, GPIO_PORT_DDR);
	debug("%s: gpio=%i\n", __func__, gpio);

	return 0;
}

int gpio_direction_output(unsigned gpio, int value)
{
	unsigned int reg = dw_readl(gpio, GPIO_PORT_DDR);

	reg |= (1 << GPIO_BIT(gpio));
	dw_writel(reg, gpio, GPIO_PORT_DDR);
	debug("%s: gpio=%i  value=%i\n", __func__, gpio, value);

	return gpio_set_value(gpio, value);
}
