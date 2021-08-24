#ifndef __BOOTCONF_H__
#define __BOOTCONF_H__

enum BMC_BOOTREASON {
  UNKNOWN=0,
  NORMAL,
  BOOTCONF_FAIL,
  NOT_TESTED,
  TEST_FAIL
};

extern uint8_t bmc_version[3];

int tp_bootconf(char *bootconf_data, int bcd_len, char *kernel_buf, int kb_len, uint32_t *loadaddr, uint32_t *entryaddr, char *dtb_buf, int dtb_len, char *initrd_buf, int ird_len, char *args_buf, int args_len);
void tp_check_boot(void);
void tp_bmc_set_bootreason(uint8_t reason, uint8_t arg);
void tp_bmc_get_version(void);

#endif/*__BOOTCONF_H__*/
