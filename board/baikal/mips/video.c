/*
 * File:        video.c
 * Description: Platforms video support
 * Author:      Alex Matveev
 * Copyright (C) 2017-2018 T-Platforms JSC
 *
 * SM750 video initilization code
 */

#include <common.h>
#include <sm750.h>
#include <version.h>
#include <asm/global_data.h>
#include "video.h"

DECLARE_GLOBAL_DATA_PTR;

unsigned int cursor_x;
unsigned int cursor_y;
bool sm750_inited = false;

#define WIDTH_MULTIPLIER 1
#define HEIGHT_MULTIPLIER 2
#define CHAR_WIDTH (8*WIDTH_MULTIPLIER)
#define CHAR_HEIGHT (8*HEIGHT_MULTIPLIER)

unsigned int img_width(void);
unsigned int img_height(void);

int
vput_char(int ch, unsigned int x, unsigned int y, uint32_t color) {
    if (!sm750_inited) {
        return -2;
    }
    if (((x+CHAR_WIDTH)>=sm750_get_xres()) ||
            ((y+CHAR_HEIGHT)>=sm750_get_yres())) {
        return -1;
    }
    extern char font8x8_basic[128][8];
    uint32_t line[CHAR_WIDTH] = {0};
    int c = 0;
    int r = 0;
    for (r=0; r<CHAR_HEIGHT; r++) {
        for (c=0; c<CHAR_WIDTH; c++) {
            if (font8x8_basic[ch][r/HEIGHT_MULTIPLIER] & 1<<(c/WIDTH_MULTIPLIER)) {
                line[c] = color;
            } else {
                line[c] = BLACK;
            }
        }
        sm750_put_line((void*)line, x, y+r, CHAR_WIDTH*sizeof(uint32_t));
    }
    return 0;
}

int
vput_string_xy(const char *str, uint32_t color) {
    if (!sm750_inited) {
        return -2;
    }
    int xres = sm750_get_xres();
    int yres = sm750_get_yres();

    int ctr = 0;
    char *ptr = (char*)str;
    while (*ptr!='\0') {
        if (*ptr == '\n') {
            cursor_y+=CHAR_HEIGHT;
            cursor_x=0;
        } else if (*ptr == '\r') {
            cursor_x=0;
        } else {
            vput_char(*ptr, cursor_x, cursor_y, color);
            cursor_x+=CHAR_WIDTH;
        }
        if (cursor_x+CHAR_WIDTH>=xres) {
            cursor_y+=CHAR_HEIGHT;
            cursor_x=0;
        }
        if (cursor_y+CHAR_HEIGHT>=yres) {
            sm750_clear();
            cursor_x = 0;
            cursor_y = 0;
        }
        ptr++;
        ctr++;
    }
    return ctr;
}

int
vput_string(const char *str) {
    return vput_string_xy(str, WHITE);
}

int
vputc(char ch) {
    char b[2] = {ch, 0};
    return vput_string_xy(b, WHITE);
}

int
vput_string_color(const char *str, uint32_t color) {
    return vput_string_xy(str, color);    
}

int drv_video_init(void)
{
    cursor_x = 0;
    cursor_y = 0;

    if (sm750_init()) {
        printf("Failed to initialize video.\n");
        return 1;
    }

    extern uint8_t img_data[];

    sm750_clear();
    sm750_inited = true;

    vput_string_color(U_BOOT_VERSION_STRING, GREEN);
    vput_string_color("\n ", GREEN);
    vput_string_color("Built with: ", GREEN);
    vput_string_color(CC_VERSION_STRING, GREEN);
    vput_string_color("\n ", GREEN);
    vput_string_color(LD_VERSION_STRING, GREEN);
    vput_string_color("\n", GREEN);
    
    gd->flags |= GD_FLG_SM750_READY;
    sm750_put_gray_bitmap(img_data, sm750_get_xres()-img_width(), 0, img_width(), img_height());
    printf("Video: %ix%i\n", sm750_get_xres(), sm750_get_yres());

    if (sm750_enable_output()) {
        printf("Failed to enable video output.\n");
        return 1;
    }
    return 0;
}
