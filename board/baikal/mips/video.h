#ifndef __VIDEO_H__
#define __VIDEO_H__

#define TRUE_WHITE 0xffffffff
#define WHITE 0xfff9fcff
#define RED   0x00f94545
#define GREEN 0x00cbff8c//0x00a4f442
#define BLACK 0x00000000

#if !defined(CONFIG_VIDEO_SM750)
#define vput_string_color(s, c)
#else
int vput_string_color(const char *str, uint32_t color);
#endif

#endif/*__VIDEO_H__*/
