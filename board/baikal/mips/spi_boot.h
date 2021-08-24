/*
 * File:        boot_cntr.h
 * Author:      Ramil Zaripov <Ramil.Zaripov@baikalelectronics.com>
 * Copyright (C) 2014 Baikal Electronics.
 * Description: Boot controller registers.
 */



#ifndef __LLENV_SPI_BOOT_H__
#define __LLENV_SPI_BOOT_H__


#include <configs/baikal_mips.h>
#include <linux/types.h>     /* uint16_t */


#define BC_CSR  (CONFIG_BOOT_BASE + 0x0)
#define BC_MAR  (CONFIG_BOOT_BASE + 0x4)
#define BC_DRID (CONFIG_BOOT_BASE + 0x8)
#define BC_VID  (CONFIG_BOOT_BASE + 0xC)

#define BC_MAR_VAR	1
#define BC_CSR_RDA      (1 << 8)

/* control and status register */
typedef struct {
    uint32_t mode       :1-0  +1;  /* boot method */
    uint32_t __         :7-2  +1;
    uint32_t spi_rda    :8-8  +1;  /* operation mode: 0- transparent, 1- not transparent */
    uint32_t _          :31-9 +1;
} boot_csr_t;

/* memory access control register */
typedef struct {
    uint32_t bsab       :0-0  +1;  /* reset when writing to the register */
    uint32_t _          :31-1 +1;
} boot_mar_t;

void llenv32_boot_ctrl (int mode);


#endif /* __LLENV32_BOOT_CNTR_H__ */

