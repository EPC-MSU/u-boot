/*
 * Date & Time support (no alarms) for Abracon
 * AB-RTCMC-32.768Khz-EOZ9-S3 Real Time Clock (RTC).
 *
 * Detailed datasheet of the chip is available here:
 * http://www.abracon.com/realtimeclock/AB-RTCMC-32.768kHz-EOZ9-S3-Application-Manual.pdf
 *
 * based on ds1307.c
 *
 * Copyright (C) 2015 Baikal Electronics JSC
 * 
 * Aurthor:
 *   Dmitry Dunaev <dmitry.dunaev@baikalelectronics.ru>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <command.h>
#include <rtc.h>
#include <i2c.h>

#if defined(CONFIG_CMD_DATE)

/*---------------------------------------------------------------------*/
#undef DEBUG_RTC

#ifdef DEBUG_RTC
#define DEBUGR(fmt,args...) printf(fmt ,##args)
#else
#define DEBUGR(fmt,args...)
#endif /* DEBUG_RTC */
/*---------------------------------------------------------------------*/

#ifndef CONFIG_SYS_I2C_RTC_ADDR
# define CONFIG_SYS_I2C_RTC_ADDR	0x56
#endif /* CONFIG_SYS_I2C_RTC_ADDR */

/*
 * RTC register addresses
 */
#define RTC_CTL_REG_ADDR	0x03

#define RTC_CTL_REG_V1F		(1 << 2)
#define RTC_CTL_REG_V2F		(1 << 3)
#define RTC_CTL_REG_SR		(1 << 4)
#define RTC_CTL_REG_PON		(1 << 5)
#define RTC_CTL_REG_EEBUSY	(1 << 7)

#define RTC_SEC_REG_ADDR	0x08
#define RTC_MIN_REG_ADDR	0x09
#define RTC_HR_REG_ADDR		0x0A
#define RTC_HR_REG_AMPM		(1 << 6)
#define RTC_HR_REG_PM		(1 << 5)
#define RTC_DAY_REG_ADDR	0x0B
#define RTC_DATE_REG_ADDR	0x0C
#define RTC_MON_REG_ADDR	0x0D
#define RTC_YR_REG_ADDR		0x0E

static uchar rtc_read (uchar reg);
static void rtc_write (uchar reg, uchar val);

/*
 * Get the current time from the RTC
 */
int rtc_get (struct rtc_time *tmp)
{
	int rel = 0;
	uchar sec, min, hour, mday, wday, mon, year, ctrl;

	i2c_set_bus_num(CONFIG_SYS_RTC_BUS_NUM);

	sec =  rtc_read (RTC_SEC_REG_ADDR);
	min =  rtc_read (RTC_MIN_REG_ADDR);
	hour = rtc_read (RTC_HR_REG_ADDR);
	wday = rtc_read (RTC_DATE_REG_ADDR);
	mday = rtc_read (RTC_DAY_REG_ADDR);
	mon =  rtc_read (RTC_MON_REG_ADDR);
	year = rtc_read (RTC_YR_REG_ADDR);
	ctrl = rtc_read (RTC_CTL_REG_ADDR);

	if (hour & RTC_HR_REG_AMPM)
		hour = (hour & 0x1f) + 12 * (hour & RTC_HR_REG_PM);

	DEBUGR ("Get RTC year: %02x mon: %02x mday: %02x wday: %02x "
		"hr: %02x min: %02x sec: %02x\n",
		year, mon, mday, wday, hour, min, sec);

	if (ctrl & RTC_CTL_REG_PON) {
		printf ("### Warning: RTC date/time corrupted\n");
		/* clear the PON flag */
		rtc_write (RTC_CTL_REG_ADDR, ctrl & ~RTC_CTL_REG_PON);
		rel = -1;
	}

	tmp->tm_sec  = bcd2bin (sec & 0x7F);
	tmp->tm_min  = bcd2bin (min & 0x7F);
	tmp->tm_hour = bcd2bin (hour & 0x3F);
	tmp->tm_mday = bcd2bin (mday & 0x3F);
	tmp->tm_mon  = bcd2bin (mon & 0x1F);
	tmp->tm_year = bcd2bin (year) + ( bcd2bin (year) >= 70 ? 1900 : 2000);
	tmp->tm_wday = bcd2bin ((wday - 1) & 0x07);
	tmp->tm_yday = 0;
	tmp->tm_isdst= 0;

	DEBUGR ("Get DATE: %4d-%02d-%02d (wday=%d)  TIME: %2d:%02d:%02d\n",
		tmp->tm_year, tmp->tm_mon, tmp->tm_mday, tmp->tm_wday,
		tmp->tm_hour, tmp->tm_min, tmp->tm_sec);

	return rel;
}


/*
 * Set the RTC
 */
int rtc_set (struct rtc_time *tmp)
{
	DEBUGR ("Set DATE: %4d-%02d-%02d (wday=%d)  TIME: %2d:%02d:%02d\n",
		tmp->tm_year, tmp->tm_mon, tmp->tm_mday, tmp->tm_wday,
		tmp->tm_hour, tmp->tm_min, tmp->tm_sec);

	if (tmp->tm_year < 1970 || tmp->tm_year > 2069)
		printf("WARNING: year should be between 1970 and 2069!\n");

	i2c_set_bus_num(CONFIG_SYS_RTC_BUS_NUM);

	rtc_write (RTC_YR_REG_ADDR, bin2bcd (tmp->tm_year % 100));
	rtc_write (RTC_MON_REG_ADDR, bin2bcd (tmp->tm_mon));
	rtc_write (RTC_DAY_REG_ADDR, bin2bcd (tmp->tm_wday + 1));
	rtc_write (RTC_DATE_REG_ADDR, bin2bcd (tmp->tm_mday));
	rtc_write (RTC_HR_REG_ADDR, bin2bcd (tmp->tm_hour));
	rtc_write (RTC_MIN_REG_ADDR, bin2bcd (tmp->tm_min));
	rtc_write (RTC_SEC_REG_ADDR, bin2bcd (tmp->tm_sec));

	return 0;
}


/*
 * Reset the RTC. We setting the date back to 1970-01-01.
 */
void rtc_reset (void)
{
	struct rtc_time tmp;

	rtc_write (RTC_CTL_REG_ADDR, 0x00);	/* clearing status */

	tmp.tm_year = 1970;
	tmp.tm_mon = 1;
	tmp.tm_mday= 1;
	tmp.tm_hour = 0;
	tmp.tm_min = 0;
	tmp.tm_sec = 0;

	rtc_set(&tmp);

	printf ( "RTC:   %4d-%02d-%02d %2d:%02d:%02d UTC\n",
		tmp.tm_year, tmp.tm_mon, tmp.tm_mday,
		tmp.tm_hour, tmp.tm_min, tmp.tm_sec);

	return;
}


/*
 * Helper functions
 */

static uchar rtc_read (uchar reg)
{
	return (i2c_reg_read (CONFIG_SYS_I2C_RTC_ADDR, reg));
}


static void rtc_write (uchar reg, uchar val)
{
	i2c_reg_write (CONFIG_SYS_I2C_RTC_ADDR, reg, val);
}
#endif /* CONFIG_CMD_DATE */
