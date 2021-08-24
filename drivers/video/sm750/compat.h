/*
 * File:        compat.h
 * Description: Video support
 * Created:     09 Mar 2017
 * Author:      Alex Matveev
 * Copyright 2017 T-Platforms
 */

#ifndef __SM750_COMPAT_H__
#define __SM750_COMPAT_H__

#include <common.h>
#include <linux/compat.h>
#include <asm/types.h>

typedef phys_addr_t resource_size_t;

#define BIT(x)         (1 << (x))

#define pr_err(...)    printf(__VA_ARGS__)
#define pr_warn(...)   printf(__VA_ARGS__)
#define pr_info(...)   do {} while (0)
#define pr_debug(...)  do {} while (0)

/**
 * strlcpy - Copy a %NUL terminated string into a sized buffer
 * @dest: Where to copy the string to
 * @src: Where to copy the string from
 * @size: size of destination buffer
 *
 * Compatible with *BSD: the result is always a valid
 * NUL-terminated string that fits in the buffer (unless,
 * of course, the buffer size is zero). It does not pad
 * out the result like strncpy() does.
 */
static inline size_t strlcpy(char *dest, const char *src, size_t size)
{
  size_t ret = strlen(src);

  if (size) {
    size_t len = (ret >= size) ? size - 1 : ret;
    memcpy(dest, src, len);
    dest[len] = '\0';
  }
  return ret;
}

#endif /* __SM750_COMPAT_H__ */
