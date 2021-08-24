/*
 * File:        sata.h
 * Description: SATA block management functions for Baikal-T
 * Created:     30 Jul 2015
 * Author:      Dmitry Dunaev
 * Copyright 2015 Baikal Electronics
 *
 */

#ifndef _ASM_BAIKAL_SATA_H
#define _ASM_BAIKAL_SATA_H

/* Common macros */

#define DW_SATA_P0PHYCR_OFS			0x178
#define DW_SATA_P1PHYCR_OFS			0x1F8

#define DW_SATA_PHYCR_PRST_MASK		0x00040000
#define DW_SATA_PHYCR_PRST_SHFT		18
#define DW_SATA_PHYCR_RXEQ_MASK		0x00038000
#define DW_SATA_PHYCR_RXEQ_SHFT		15
#define DW_SATA_PHYCR_TXINV_MASK	0x00004000
#define DW_SATA_PHYCR_TXINV_SHFT	14
#define DW_SATA_PHYCR_RXINV_MASK	0x00002000
#define DW_SATA_PHYCR_RXINV_SHFT	13
#define DW_SATA_PHYCR_TXPREE_MASK	0x00001F80
#define DW_SATA_PHYCR_TXPREE_SHFT	7
#define DW_SATA_PHYCR_TXAMPL_MASK	0x0000007F
#define DW_SATA_PHYCR_TXAMPL_SHFT	0

#define DW_SATA_PHYCR_DEFAULT		0x000011FF

#define DW_SATA_PHYCR_GET(NM, R) ((R & DW_SATA_PHYCR_ ## NM ## _MASK)  >> \
 								DW_SATA_PHYCR_ ## NM ## _SHFT)

#define DW_SATA_PHYCR_SET(NM, R, V) ((R & ~DW_SATA_PHYCR_ ## NM ## _MASK) | \
 								((V << DW_SATA_PHYCR_ ## NM ## _SHFT) & \
								 DW_SATA_PHYCR_ ## NM ## _MASK))

#define DW_SATA_PHYCR_INIT		0x00000064

#ifndef __ASSEMBLER__

void sata_phy_init_val(int phyid, unsigned int val);
void sata_phy_init(void);

#endif /* __ASSEMBLER__ */

#endif /* _ASM_BAIKAL_SATA_H */
