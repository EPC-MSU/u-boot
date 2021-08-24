#
# (C) Copyright 2013-2018
# Baikal Electronics OJSC
# Author: Dmitry Dunaev <dmitry.dunaev@baikalelectronics.ru>
#
# SPDX-License-Identifier:	GPL-2.0+
#

obj-y += cpu.o
obj-y += clock.o
obj-y += lowlevel_init.o
obj-y += spi.o
obj-y += mem.o
obj-y += boot.o boot_misc.o ddr3_init.o pmu_misc.o ddr_spd.o
obj-y += pvt.o
obj-$(CONFIG_BAIKAL_PLL) += clock_manager.o cmd_cpu.o
obj-$(CONFIG_CMD_SATA_PHY) += cmd_phy.o
obj-$(CONFIG_CMD_EXIT) += cmd_exit.o
obj-$(CONFIG_OF_LIBFDT) += fdt.o
obj-$(CONFIG_BAIKAL_LLENV) += llenv32.o
obj-$(CONFIG_BAIKAL_LLENV) += llenv/
obj-$(CONFIG_PCIE_DW) += pcie_dw.o
obj-$(CONFIG_VIDEO_SM750) += video.o font8x8.o logo.o
obj-$(CONFIG_TP_BOOTCONF) += bootconf.o
obj-$(CONFIG_TP_FRU) += fru.o
obj-$(CONFIG_CMD_SERVICE) += cmd_service.o
obj-$(CONFIG_BAIKAL_BFK) += bfk/
obj-$(CONFIG_BAIKAL_BFKX) += bfk/
obj-$(CONFIG_BAIKAL_BFK3) += bfk/
obj-$(CONFIG_BAIKAL_QEMU) += bfk/
obj-$(CONFIG_BAIKAL_FPGA) += bfk/
obj-$(CONFIG_TPLATFORMS_SFBT1) += sfbt1/
obj-$(CONFIG_TPLATFORMS_MITX) += mitx/
obj-$(CONFIG_TPLATFORMS_RT1MB) += rt1mb/
obj-$(CONFIG_TPLATFORMS_MSBT2) += msbt2/
obj-$(CONFIG_TPLATFORMS_MRBT1) += mrbt1/
obj-$(CONFIG_TPLATFORMS_AXT1_SFBT1) += axt1/
obj-$(CONFIG_TPLATFORMS_AXT1_MSBT2) += axt1/
obj-$(CONFIG_TPLATFORMS_BC3BT1_MRBT1) += bc3bt1/
obj-$(CONFIG_TPLATFORMS_TC_SFBT1) += tclient/
obj-$(CONFIG_TPLATFORMS_TC_MSBT2) += tclient/
obj-$(CONFIG_TPLATFORMS_CNC_SFBT1) += cnc/
obj-$(CONFIG_TPLATFORMS_CNC_MSBT2) += cnc/
obj-$(CONFIG_TPLATFORMS_BN1BT1) += bn1bt1/
obj-$(CONFIG_TPLATFORMS_SBC101) += sbc101/
#ifdef CONFIG_CUSTOM_SPD
SPD_FILE := $(patsubst "%", SPD_%.o, $(CONFIG_CUSTOM_SPD_FILE))
obj-y += $(SPD_FILE)
#endif
subdir-ccflags-y += -I$(srctree)/board/baikal/mips
