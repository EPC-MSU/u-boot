/*
 * (C) Copyright 2018
 * Konstantin Kirik, T-Platforms, snegovick@uprojects.org.
 *
 * SPDX-License-Identifier:	T-Platforms
 */

/*
 * Date & Time support for NXP PCF2127/X RTC
 */

/* #define	DEBUG	*/

#include <common.h>
#include <command.h>
#include <rtc.h>
#include <i2c.h>

#if defined(CONFIG_CMD_DATE)

#define PCF2127_REG_CTRL1       (0x00)  /* Control Register 1 */
#define PCF2127_REG_CTRL2       (0x01)  /* Control Register 2 */

#define PCF2127_REG_CTRL3       (0x02)  /* Control Register 3 */
#define PCF2127_REG_CTRL3_BLF		(1<<2)

#define PCF2127_REG_SC          (0x03)  /* datetime */
#define PCF2127_REG_MN          (0x04)
#define PCF2127_REG_HR          (0x05)
#define PCF2127_REG_DM          (0x06)
#define PCF2127_REG_DW          (0x07)
#define PCF2127_REG_MO          (0x08)
#define PCF2127_REG_YR          (0x09)

#define PCF2127_OSF             (1<<7)  /* Oscillator Fail flag */

//#define debug printf

static uchar rtc_read  (uchar reg);
static void  rtc_write (uchar reg, uchar val);

/* ------------------------------------------------------------------------- */

int rtc_get (struct rtc_time *tmp)
{
	int rel = 0;
	uchar sec, min, hour, mday, wday, mon, year, ctl3;

	ctl3 = rtc_read (PCF2127_REG_CTRL3);

	if (ctl3 & PCF2127_REG_CTRL3_BLF) {
		printf("%s low voltage detected, check/replace RTC battery\n", __func__);
	}

	sec  = rtc_read (PCF2127_REG_SC);
	min  = rtc_read (PCF2127_REG_MN);
	hour = rtc_read (PCF2127_REG_HR);
	mday = rtc_read (PCF2127_REG_DM);
	wday = rtc_read (PCF2127_REG_DW);
	mon  = rtc_read (PCF2127_REG_MO);
	year = rtc_read (PCF2127_REG_YR);

	if (sec & PCF2127_OSF) {
		printf("%s oscillator stop detected, date/time is not reliable\n", __func__);
		//rtc_write (PCF2127_REG_SC, sec & 0x7F); //Attempt to clear OSF
		rel = -1;
	}

	tmp->tm_sec  = bcd2bin(sec & 0x7F);
	tmp->tm_min  = bcd2bin(min & 0x7F);
	tmp->tm_hour = bcd2bin(hour & 0x3F); /* rtc hr 0-23 */
	tmp->tm_mday = bcd2bin(mday & 0x3F);
	tmp->tm_wday = wday & 0x07;
	tmp->tm_mon  = bcd2bin(mon & 0x1F) - 1; /* rtc mn 1-12 */
	tmp->tm_year = bcd2bin(year);
	if (tmp->tm_year < 70) {
		tmp->tm_year += 100;	/* assume we are in 1970...2069 */
	}
	tmp->tm_year += 1900;

	debug ( "Get DATE: %4d-%02d-%02d (wday=%d)	TIME: %2d:%02d:%02d\n",
					tmp->tm_year, tmp->tm_mon, tmp->tm_mday, tmp->tm_wday,
					tmp->tm_hour, tmp->tm_min, tmp->tm_sec);

	return rel;
}

int rtc_set (struct rtc_time *tmp)
{
	debug ( "Set DATE: %4d-%02d-%02d (wday=%d)  TIME: %2d:%02d:%02d\n",
		tmp->tm_year, tmp->tm_mon, tmp->tm_mday, tmp->tm_wday,
		tmp->tm_hour, tmp->tm_min, tmp->tm_sec);

	/* this will also clear OSF flag */
	rtc_write (PCF2127_REG_SC, bin2bcd(tmp->tm_sec));
	rtc_write (PCF2127_REG_MN, bin2bcd(tmp->tm_min));
	rtc_write (PCF2127_REG_HR, bin2bcd(tmp->tm_hour));
	rtc_write (PCF2127_REG_DM, bin2bcd(tmp->tm_mday));
	rtc_write (PCF2127_REG_DW, tmp->tm_wday & 0x07);
	rtc_write (PCF2127_REG_MO, bin2bcd(tmp->tm_mon+1));
	rtc_write (PCF2127_REG_YR, bin2bcd(tmp->tm_year % 100));

	return 0;
}

void rtc_reset (void)
{
	struct rtc_time tmp;

	tmp.tm_year = 1970;
	tmp.tm_mon  = 1;
	tmp.tm_mday = 1;
	tmp.tm_hour = 0;
	tmp.tm_min  = 0;
	tmp.tm_sec  = 0;

	rtc_set(&tmp);

	printf ( "RTC:   %4d-%02d-%02d %2d:%02d:%02d UTC\n",
		tmp.tm_year, tmp.tm_mon, tmp.tm_mday,
		tmp.tm_hour, tmp.tm_min, tmp.tm_sec);
}

/* ------------------------------------------------------------------------- */

static uchar rtc_read (uchar reg)
{
	i2c_set_bus_num(CONFIG_SYS_RTC_BUS_NUM);
	return (i2c_reg_read (CONFIG_SYS_I2C_RTC_ADDR, reg));
}

static void rtc_write (uchar reg, uchar val)
{
	i2c_set_bus_num(CONFIG_SYS_RTC_BUS_NUM);
	i2c_reg_write (CONFIG_SYS_I2C_RTC_ADDR, reg, val);
}

#endif
