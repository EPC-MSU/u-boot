/*
 * Low-level and POST tests.
 *
 * Copyright (C) 2014 Baikal Electronics.
 *
 * Author: Alexey Malahov <Alexey.Malahov@baikalelectronics.com>
 *
 * Description: Main header file.
 */

#ifndef __LLENV_H__
#define __LLENV_H__


typedef struct
{
	int (*ts_init) (int tc_num);
	int  (*ts_run) (char *tc_name);
	void (*ts_info) (void);
	int (*ts_teardown) (int tc_num);
	char *testlink_ts_name;
} llenv_tsuite_t; /* type of test suite. */

typedef struct
{
	int (*tc_run) (void);
	char *testlink_tc_name;
} llenv_tcase_t;


int llenv_default_run_testsuite(llenv_tsuite_t *ts, llenv_tcase_t **tc, char *tc_name);
int llenv_run_all(void);
int llenv_run_ts(char *ts_name, char *tc_name);
int llenv_run_ts_index(int index);


#endif /* __LLENV_H__ */

