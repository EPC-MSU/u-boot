/*
 * File:        devid.h
 * Description: CNC devices identifiers
 * Authors:     Sergey Semin <sergey.semin@t-platforms.ru>
 *
 * Copyright 2018 T-platforms JSC
 */
#ifndef __CNC_DEVID_H__
#define __CNC_DEVID_H__

#include <linux/types.h>

/*
 * CNC devices shred-related information
 * @CNC_SHRED_I2C_BUS:  Shred i2c-bus
 * @CNC_SHRED_INFO:     CNC device information GPIO-exapnder address
 * @CNC_SHRED_GET_DID:  Get device ID from GPIO-expander data
 * @CNC_SHRED_GET_REV:  Get device revision from GPIO-expander data
 */
#define CNC_SHRED_I2C_BUS	CONFIG_SYS_SHRED1_BUS_NUM
#define CNC_SHRED_INFO		CONFIG_SYS_SHRED1_I2C_ADDR
#define CNC_SHRED_GET_DID(data)	(((data) >> 4) & 0xF)
#define CNC_SHRED_GET_REV(data)	((data) & 0xF)

/*
 * Common CNC device IDs
 */
#define CNC_DID_CUDEV		((u16)0x0000)
#define CNC_DID_CU		((u16)0x0001)
#define CNC_DID_OUT		((u16)0x0002)
#define CNC_DID_IN		((u16)0x0003)
#define CNC_DID_PROBE		((u16)0x0004)
#define CNC_DID_DAC		((u16)0x0005)
#define CNC_DID_TTL		((u16)0x0006)
#define CNC_DID_ENDAT22		((u16)0x0007)
#define CNC_DID_E1VPP		((u16)0x0008)
#define CNC_REV_ANY		((u8)0xFF)

/*
 * CNC Device stringified name
 */
static inline const char *cnc_dev_name(u16 DID)
{
	switch (DID) {
	case CNC_DID_CUDEV:
		return "cudev";
	case CNC_DID_CU:
		return "cu";
	case CNC_DID_OUT:
		return "out";
	case CNC_DID_IN:
		return "in";
	case CNC_DID_PROBE:
		return "probe";
	case CNC_DID_DAC:
		return "dac";
	case CNC_DID_TTL:
		return "ttl";
	case CNC_DID_ENDAT22:
		return "endat22";
	case CNC_DID_E1VPP:
		return "e1vpp";
	}
	return "unknown";
}

#endif /* __CNC_DEVID_H__ */
