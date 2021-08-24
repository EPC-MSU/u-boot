/*
 * Copyright (C) 2015 Imagination Technologies
 *
 * SPDX-License-Identifier:	GPL-2.0
 */

#ifndef _ASM_HAZARDS_H
#define _ASM_HAZARDS_H

static inline void back_to_back_c0_hazard(void)
{
	asm volatile("ehb");
}

static inline void instruction_hazard(void)
{
	asm volatile(
		".set	push\n\t"
		".set	noreorder\n\t"
		".set	noat\n\t"
		"bal	1f\n\t"
		" nop\n\t"
	"1:	addiu	$1, $31, 2f - 1b\n\t"
		"jr.hb	$1\n\t"
		" nop\n\t"
	"2:	.set	pop"
		: : : "$1", "$31");
}

#endif /* _ASM_HAZARDS_H */
