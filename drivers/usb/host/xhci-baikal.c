/*
 * Baikal-T USB HOST xHCI Controller
 *
 * (C) Copyright 2014,2015 Baikal Electronics
 * http://www.baikalelectronics.ru
 *
 * Author: Dmitry Dunaev <dmitry.dunaev@baikalelectronics.ru>
 * Based on drivers/usb/host/xhci-baikal.c
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <usb.h>
#include <asm-generic/errno.h>

#include <linux/compat.h>
#include <linux/usb/dwc3.h>

#include "xhci.h"

/* Declare global data pointer */
DECLARE_GLOBAL_DATA_PTR;

struct baikal_xhci {
	struct xhci_hccr *hcd;
	struct dwc3 *dwc3_reg;
};

static struct baikal_xhci baikal;

int __attribute__((weak)) board_usb_init(int index, enum usb_init_type init);

static void dwc3_set_mode(struct dwc3 *dwc3_reg, u32 mode)
{
	dwc3_reg->g_ctl &= ~DWC3_GCTL_PRTCAPDIR(DWC3_GCTL_PRTCAP_OTG);
	dwc3_reg->g_ctl |= DWC3_GCTL_PRTCAPDIR(mode);
}

static void dwc3_core_soft_reset(struct dwc3 *dwc3_reg)
{
	/* Before Resetting PHY, put Core in Reset */
	dwc3_reg->g_ctl |= DWC3_GCTL_CORESOFTRESET;

	/* Assert USB3 PHY reset */
	dwc3_reg->g_usb3pipectl[0] |= DWC3_GUSB3PIPECTL_PHYSOFTRST;

	/* Assert USB2 PHY reset */
	dwc3_reg->g_usb2phycfg[0] |= DWC3_GUSB2PHYCFG_PHYSOFTRST;

	mdelay(100);

	/* Clear USB3 PHY reset */
	dwc3_reg->g_usb3pipectl[0] &= ~DWC3_GUSB3PIPECTL_PHYSOFTRST;

	/* Clear USB2 PHY reset */
	dwc3_reg->g_usb2phycfg[0] &= ~DWC3_GUSB2PHYCFG_PHYSOFTRST;

	/* After PHYs are stable we can take Core out of reset state */
	dwc3_reg->g_ctl &= ~DWC3_GCTL_CORESOFTRESET;

	mdelay(100);
}

static int dwc3_core_init(struct dwc3 *dwc3_reg)
{
	u32 reg;
	u32 revision;
	unsigned int dwc3_hwparams1;

	revision = readl(&dwc3_reg->g_snpsid);
	/* This should read as U3 followed by revision number */
	if ((revision & DWC3_GSNPSID_MASK) != 0x55330000) {
		puts("this is not a DesignWare USB3 DRD Core\n");
		return -1;
	}

	dwc3_core_soft_reset(dwc3_reg);

	dwc3_hwparams1 = readl(&dwc3_reg->g_hwparams1);

	reg = readl(&dwc3_reg->g_ctl);
	reg &= ~DWC3_GCTL_SCALEDOWN_MASK;
	reg &= ~DWC3_GCTL_DISSCRAMBLE;
	switch (DWC3_GHWPARAMS1_EN_PWROPT(dwc3_hwparams1)) {
	case DWC3_GHWPARAMS1_EN_PWROPT_CLK:
		reg &= ~DWC3_GCTL_DSBLCLKGTNG;
		break;
	default:
		debug("No power optimization available\n");
	}

	/*
	 * WORKAROUND: DWC3 revisions <1.90a have a bug
	 * where the device can fail to connect at SuperSpeed
	 * and falls back to high-speed mode which causes
	 * the device to enter a Connect/Disconnect loop
	 */
	if ((revision & DWC3_REVISION_MASK) < 0x190a)
		reg |= DWC3_GCTL_U2RSTECN;

	writel(reg, &dwc3_reg->g_ctl);

	return 0;
}

static int baikal_xhci_core_init(struct baikal_xhci *baikal)
{
	int ret;

	ret = dwc3_core_init(baikal->dwc3_reg);
	if (ret) {
		debug("%s:failed to initialize core\n", __func__);
		return ret;
	}

	/* We are hard-coding DWC3 core to Host Mode */
	dwc3_set_mode(baikal->dwc3_reg, DWC3_GCTL_PRTCAP_HOST);

	return ret;
}

int xhci_hcd_init(int index, struct xhci_hccr **hccr, struct xhci_hcor **hcor)
{
	struct baikal_xhci *ctx = &baikal;
	int ret;

	ctx->hcd = (struct xhci_hccr *)CONFIG_USB_XHCI_BAIKAL_BASE;
	ctx->dwc3_reg = (struct dwc3 *)((char *)(ctx->hcd) + DWC3_REG_OFFSET);

	ret = board_usb_init(index, USB_INIT_HOST);
	if (ret != 0) {
		puts("Failed to initialize board for USB\n");
		return ret;
	}

	ret = baikal_xhci_core_init(ctx);
	if (ret < 0) {
		puts("Failed to initialize xhci\n");
		return ret;
	}

	*hccr = (struct xhci_hccr *)(CONFIG_USB_XHCI_BAIKAL_BASE);
	*hcor = (struct xhci_hcor *)((uint32_t) *hccr
				+ HC_LENGTH(xhci_readl(&(*hccr)->cr_capbase)));

	debug("baikal-xhci: init hccr %x and hcor %x hc_length %d\n",
	      (uint32_t)*hccr, (uint32_t)*hcor,
	      (uint32_t)HC_LENGTH(xhci_readl(&(*hccr)->cr_capbase)));

	return ret;
}

void xhci_hcd_stop(int index)
{
	/* Nothing todo here */
	return;
}
