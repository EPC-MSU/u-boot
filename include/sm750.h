/*
 * File:        sm750.h
 * Description: SM750 support
 * Created:     27 Feb 2017
 * Author:      Alex Matveev
 * Copyright 2017 T-Platforms
 */

#ifndef __SM750_H__
#define __SM750_H__

int sm750_init(void);
int sm750_enable_output(void);

void sm750_hw_rectfill(unsigned int dst_x, unsigned int dst_y,
                       unsigned int dim_x, unsigned int dim_y, unsigned int
                       color);
void sm750_clear(void);
void sm750_put_pixel(uint32_t color, unsigned int x, unsigned int y);
uint32_t sm750_get_xres(void);
uint32_t sm750_get_yres(void);
void sm750_put_line(void* data, unsigned int x, unsigned int y, unsigned int len);
void sm750_put_gray_bitmap(void *data, unsigned int x, unsigned int y, unsigned int w, unsigned int h);

#endif /* __SM750_H__ */
