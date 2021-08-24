#ifndef __FRU_H__
#define __FRU_H__

#include <common.h>
#include <stdbool.h>

#define MR_MAC_REC          0xC0
#define MR_SATADEV_REC      0xC1
#define MR_PASSWD_REC       0xC2
#define MR_TESTOK_REC       0xC3
#define MR_POWER_POLICY_REC 0xC4
#define MR_POWER_STATE_REC  0xC5
#define MR_MAC2_REC         0xC6
#define MR_MAC3_REC         0xC7

#define FRU_ADDR      0xa6
#define FRU_PAGE_SIZE 32
#define FRU_ADDR_SIZE 2

#define FRU_STR_MAX 32
#define FRU_PWD_MAX 128
#define N_MULTIREC  8

#define N_MAC 3

#define FRU_STR(name, len) unsigned int len_##name; uint8_t val_##name[len]

enum POWER_POLICY {
  PP_OFF=0,
  PP_KEEP,
  PP_ON,
  PP_NUM
};

struct multirec {
  uint8_t type;
  uint8_t format;
  bool end;
  unsigned int length;
  bool header_cs_ok;
  bool cs_ok;
  uint8_t *data;
};

struct fru {
  uint8_t mac_data[6*N_MAC];
  uint8_t *mac0;
  uint8_t *mac1;
  uint8_t *mac2;
  uint8_t mfg_date[3];
  uint8_t bootdevice[FRU_STR_MAX];
  uint8_t passwd_line[FRU_PWD_MAX];
  uint8_t power_policy;
  uint8_t power_state;
  uint8_t test_ok;
  unsigned int board_area_offset;
  unsigned int product_area_offset;
  unsigned int mrec_area_offset;
  FRU_STR(mfg_name, FRU_STR_MAX);
  FRU_STR(product_name, FRU_STR_MAX);
  FRU_STR(serial_number, FRU_STR_MAX);
  FRU_STR(part_number, FRU_STR_MAX);
  FRU_STR(fru_id, FRU_STR_MAX);
  
  FRU_STR(p_product_mfg, FRU_STR_MAX);
  FRU_STR(p_product_name, FRU_STR_MAX);
  FRU_STR(p_part_model_number, FRU_STR_MAX);
  FRU_STR(p_product_version, FRU_STR_MAX);
  FRU_STR(p_serial_number, FRU_STR_MAX);
  FRU_STR(p_fru_id, FRU_STR_MAX);
  struct multirec mrec[N_MULTIREC];
  unsigned int mrec_count;
};

extern struct fru fru;
int fru_open_parse(void);
int fru_update_mrec_eeprom(void);
int fru_mrec_update_mac(struct fru *f, uint8_t *mac, int iface);
int fru_mrec_update_bootdevice(struct fru *f, uint8_t *bootdevice);
int fru_mrec_update_passwd_line(struct fru *f, uint8_t *passwd_line);
int fru_mrec_update_test_ok(struct fru *f, uint8_t test_ok);
int fru_mrec_update_power_policy(struct fru *f, enum POWER_POLICY pp);
int fru_mrec_update_power_state(struct fru *f);
void print_board_area(struct fru *f);
void print_product_area(struct fru *f);

#endif/*__FRU_H__*/
