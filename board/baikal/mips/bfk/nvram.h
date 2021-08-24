/*
 * File:        nvram.h
 * Description: NVRAM functions for Baikal-T
 * Created:     06 Aug 2015
 * Author:      Dmitry Dunaev
 * Copyright 2015 Baikal Electronics
 *
 */

#ifndef _ASM_BAIKAL_NVRAM_H
#define _ASM_BAIKAL_NVRAM_H

/* Hardware NVRAM init */
int nvram_init(void);

/* Generic board NVRAM init */
void board_nvram_init(void);

/* Simple NVRAM functions */
uchar nvram_read(u8 ofs);
void nvram_write(u8 ofs, uchar val);

/* Block NVRAM functions */
void nvram_read_block(u8 *buf, u8 size);
void nvram_write_block(u8 *buf, u8 size);

/* EEPROM / Serial Number functions */
#define nvram_read_serial nvram_read_eeprom
u16 nvram_read_eeprom(void);
#define nvram_write_serial nvram_write_eeprom
void nvram_write_eeprom(u16 val);

/* Misc functions */
int nvram_get_size(void);

#endif /* _ASM_BAIKAL_NVRAM_H */
