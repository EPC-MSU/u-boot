/*
 * Copyright 2018 T-platforms JSC
 *
 * SPDX-License-Identifier:       GPL-2.0
 */
#ifndef __PCF857X_H_
#define __PCF857X_H_

#include <linux/types.h>

int pcf857x_set_val(u8 chip, uint mask, uint data);
int pcf857x_get_val(u8 chip);

#endif /* __PCF857X_H_ */
