#include "fru.h"
#include <stdbool.h>

#define FRU_VERSION 1
#define BOARD_AREA_VERSION 1
#define PRODUCT_AREA_VERSION 1
#define FRU_SIZE 4096
#define TAG "FRU"

#include <common.h>
#include <net.h>
#include <i2c.h>
#define fmsg(...) {printf (__VA_ARGS__); }
#define flog(...) {printf ("L["TAG"]: "__VA_ARGS__); }
#define fwarn(...) {printf ("W["TAG"]: "__VA_ARGS__); }
#define ferr(...) {printf ("E["TAG"]: "__VA_ARGS__); }

#ifdef FRU_DEBUG
#define fru_dbg(...) {printf (__VA_ARGS__); }
#else
#define fru_dbg(...)
#endif

static uint8_t fru_buf[FRU_SIZE];
static uint8_t fru_buf2[FRU_SIZE];
struct fru fru;

uint8_t
calc_cs(uint8_t *buf, uint8_t size) {
  uint8_t cs = 0;
  int i = 0;
  fru_dbg("CS:\n");
  for (;i<size; i++) {
    cs += buf[i];
#ifdef FRU_DEBUG
    if (i%8==0) {
      fru_dbg("\n");
    }
    fru_dbg("0x%02x ", cs);
#endif
  }
  fru_dbg("\n");
  return cs;
}

int
fru_mk_multirecord(uint8_t *buf, unsigned int buf_size, uint8_t record_type, bool end, uint8_t *record, uint8_t record_size) {
  int i = 0;
  int size = 5+record_size;
  int remainder = buf_size-size;

  if (remainder<=0) {
    return -1;
  }
  buf[0] = record_type;
  buf[1] = (end ? (1<<7) : 0) | 0x02;
  buf[2] = record_size;
  memcpy((buf+5), record, record_size);
  buf[3] = 256-calc_cs(buf+5, record_size);
  buf[4] = 256-calc_cs(buf, 4);
  for (;i<size;i++) {
    if (i%8==0) {
      fru_dbg("\r\n");
    }
    fru_dbg("%02x[%c] ", buf[i], (buf[i]>=0x20 ? buf[i] : ' '));
  }
  fru_dbg("\r\n");
  return size;
}

int
read_fru_str(uint8_t *buf, uint8_t *str, unsigned int *len, unsigned int offt) {
  *len = ((buf[offt]>(FRU_STR_MAX-1))?(FRU_STR_MAX-1):buf[offt]);
  memset(str, 0, FRU_STR_MAX);
  memcpy(str, &buf[offt+1], *len);
  offt += buf[offt]+1;
  return offt;
}

int
parse_board_area(struct fru *f, uint8_t *buf, unsigned int buf_len) {
  uint8_t cs;
  int offt = 0;
  
  if (buf[0] != BOARD_AREA_VERSION) {
    fwarn("FRU: Board area version is not valid\n");
    return -1;
  }
  if ((buf[1]*8)>buf_len) {
    fwarn("FRU: Board area size mismatch\n");
    return -2;
  }
#ifdef FRU_DEBUG
  {
    int i = 0;
    flog("FRU Board area bin:\n");
    for (;i<buf[1]*8;i++) {
      if ((i%8)==0) {
        fru_dbg("\n");
      }
      fru_dbg("0x%02x[%c] ", buf[i], (buf[i]>' '?buf[i]:' '));
    }
    fru_dbg("\n");
  }
#endif

  cs = calc_cs(buf, buf[1]*8);
  if (cs != 0) {
    fwarn("FRU: Bad board area checksum [0-%i]: %i\n", buf[1]*8, cs);
    return -3;
  }
  f->mfg_date[0] = buf[3];
  f->mfg_date[1] = buf[4];
  f->mfg_date[2] = buf[5];

  offt = 5;
  offt = read_fru_str(buf, f->val_mfg_name, &f->len_mfg_name, offt);
  offt = read_fru_str(buf, f->val_product_name, &f->len_product_name, offt);
  offt = read_fru_str(buf, f->val_serial_number, &f->len_serial_number, offt);
  offt = read_fru_str(buf, f->val_part_number, &f->len_part_number, offt);
  offt = read_fru_str(buf, f->val_fru_id, &f->len_fru_id, offt);
#ifdef FRU_DEBUG
  print_board_area(&fru);
#endif
  return 0;
}

void
print_board_area(struct fru *f) {
  flog("FRU Board area:\n");
  flog("Board mfg:          \t%s\n", f->val_mfg_name);
  flog("Board name:         \t%s\n", f->val_product_name);
  flog("Board serial number:\t%s\n", f->val_serial_number);
  flog("Board part number:  \t%s\n", f->val_part_number);
  flog("Board fru id:       \t%s\n", f->val_fru_id);
}

int
parse_product_area(struct fru *f, uint8_t *buf, unsigned int buf_len) {
  int offt = 0;
  if (buf[0] != PRODUCT_AREA_VERSION) {
    fwarn("FRU: Product area version is not valid\n");
    return -1;
  }
  if ((buf[1]*8)>buf_len) {
    fwarn("FRU: Product area size mismatch\n");
    return -2;
  }
#ifdef FRU_DEBUG
  {
    int i = 0;
    flog("FRU Product area bin:\n");
    for (i=0; i<buf[1]*8; i++) {
      if ((i%8)==0) {
        fru_dbg("\n");
      }
      fru_dbg("0x%02x[%c] ", buf[i], (buf[i]>' '?buf[i]:' '));
    }
    fru_dbg("\n");
  }
#endif

  if (calc_cs(buf, buf[1]*8) != 0) {
    fwarn("FRU: Bad product area checksum\n");
    return -3;
  }
  offt = 3;
  offt = read_fru_str(buf, f->val_p_product_mfg, &f->len_p_product_mfg, offt);
  offt = read_fru_str(buf, f->val_p_product_name, &f->len_p_product_name, offt);
  offt = read_fru_str(buf, f->val_p_part_model_number, &f->len_p_part_model_number, offt);
  offt = read_fru_str(buf, f->val_p_product_version, &f->len_p_product_version, offt);
  offt = read_fru_str(buf, f->val_p_serial_number, &f->len_p_serial_number, offt);
  offt = read_fru_str(buf, f->val_p_fru_id, &f->len_p_fru_id, offt);
#ifdef FRU_DEBUG
  print_product_area(&fru);
#endif
  return 0;
}

void
print_product_area(struct fru *f) {
  flog("FRU Product area:\n");
  flog("Product mfg:          \t%s\n", f->val_p_product_mfg);
  flog("Product name:         \t%s\n", f->val_p_product_name);
  flog("Product model number: \t%s\n", f->val_p_part_model_number);
  flog("Product version:      \t%s\n", f->val_p_product_version);
  flog("Product serial number:\t%s\n", f->val_p_serial_number);
  flog("Product fru id:       \t%s\n", f->val_p_fru_id);
}

int
fru_parse_multirecord(struct multirec *m, uint8_t *buf, unsigned int buf_len) {
  uint8_t data_cs;
  if (buf_len<5) {
    fwarn("FRU: no space in multirecord buffer, failed to parse header\n");
    return -4;
  }
#ifdef FRU_DEBUG
  {
    int i = 0;
    flog("FRU mrec header bin:\n");
    for (;i<5;i++) {
      fru_dbg("0x%02x[%c] ", buf[i], (buf[i]>' '?buf[i]:' '));
    }
    fru_dbg("\n");
  }
#endif

  if (calc_cs(buf, 5) != 0) {
    fwarn("FRU: multirecord header checksum is invalid\n");
    m->header_cs_ok = false;
    return -1;
  } else {
    m->header_cs_ok = true;
  }
  m->type = buf[0];
  m->format = buf[1]&0x7;
  if (m->format != 0x2) {
    fwarn("FRU: multirecord format is unknown [%i]\n", m->format);
    return -2;
  }
  m->end = (buf[1]&0x80?true:false);
  if (m->end) {
    fru_dbg("FRU: last multirecord\n");
  }
  m->length = buf[2];
  if ((buf_len-5)<(m->length)) {
    fwarn("FRU: no space in multirecord buffer, failed check data\n");
    return -5;
  }
#ifdef FRU_DEBUG
  {
    int i = 0;
    flog("FRU mrec bin:\n");
    for (;i<(m->length+5);i++) {
      if ((i%8)==0) {
        fru_dbg("\n");
      }
      fru_dbg("0x%02x[%c] ", buf[i], (buf[i]>' '?buf[i]:' '));
    }
    fru_dbg("\n");
  }
#endif
  
  data_cs = calc_cs(&buf[5], m->length)+buf[3];
  if (data_cs != 0) {
    fwarn("FRU: multirecord data checksum is invalid [0x%02x]\n", data_cs);
    return -3;
  }
  m->data = &buf[5];
  return m->length+5;
}

int
parse_fru(struct fru *f, uint8_t *buf, unsigned int buf_len) {
  int ret = 0;
  int mrec_n = 0;
  int offt = 0;
  if (buf_len<8) {
    fwarn("FRU buffer is too short\n");
    return -1;
  }
  fru_dbg("FRU: Checking header [0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x]\n", buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7]);
  if (buf[0] == 0xff) {
    fwarn("FRU: Empty EEPROM detected\n");
    return -2;
  } else if (buf[0] != FRU_VERSION) {
    fwarn("FRU: Header version is not valid\n");
    return -3;
  } else if (calc_cs(buf, 8) != 0) {
    fwarn("FRU: Bad header checksum: %i\n", calc_cs(buf, 8));
    return -4;
  }
  f->board_area_offset = buf[3]*8;
  f->product_area_offset = buf[4]*8;
  f->mrec_area_offset = buf[5]*8;
  if (parse_board_area(f, &buf[f->board_area_offset], buf_len-f->board_area_offset)) {
    return -5;
  }
  if (parse_product_area(f, &buf[f->product_area_offset], buf_len-f->product_area_offset)) {
    return -6;
  }
  f->mrec_count = 0;
  offt = f->mrec_area_offset;
  while (ret >= 0 && (mrec_n < N_MULTIREC)) {
    fru_dbg("FRU: parsing multirecord %i\n", f->mrec_count);
    ret = fru_parse_multirecord(&f->mrec[mrec_n], &buf[offt], buf_len-offt);
    if (ret > 0) {
      f->mrec_count ++;
    } else {
      fwarn("FRU: Failed to parse multirecord\n");
      return -7;
    }
    if (f->mrec[mrec_n].end) {
      break;
    } else {
      offt += ret;
    }
    mrec_n ++;
  }
  return 0;
}

int
fru_mk_multirecords_area(struct fru *f, uint8_t *buf, unsigned int buf_len) {
  int i = 0;
  int ret = 0;
  unsigned int offt = 0;
  flog("Packing multirecord area\n");
  for (;i<f->mrec_count; i++) {
    flog("Packing [%02x]\n", f->mrec[i].type);
    ret = fru_mk_multirecord(buf+offt, buf_len-offt, f->mrec[i].type, (f->mrec_count-i>1?false:true), f->mrec[i].data, f->mrec[i].length);
    if (ret < 0) {
      fwarn("FRU: Failed to pack multirecord\n");
      return -1;
    }
    offt+=ret;
  }
  return 0;
}

int
fru_mrec_update_mac(struct fru *f, uint8_t *mac, int iface) {
  int i = 0;
  static const int mac_mrec_id[N_MAC] = {MR_MAC_REC, MR_MAC2_REC, MR_MAC3_REC};
  if (iface>=N_MAC) {
    return -1;
  }
  memcpy(f->mac_data+iface*6, mac, 6);
  for (; i<f->mrec_count; i++) {
    if (f->mrec[i].type == mac_mrec_id[iface]) {
      f->mrec[i].data = f->mac_data+iface*6;
      return 0;
    }
  }
  return -1;
}

static void fru_setall_macaddr(void)
{
  uchar enetaddr[6];
  char name[32];
  int i;

  for (i = 0; i < CONFIG_FRU_MAX_IFACES; ++i) {
    switch (i) {
    case 0:
      memcpy(enetaddr, fru.mac0, 6);
      break;
    case 1:
      memcpy(enetaddr, fru.mac1, 6);
      break;
    case 2:
      memcpy(enetaddr, fru.mac2, 6);
      break;
    default:
      break;
    }

    sprintf(name, i ? "eth%daddr" : "ethaddr", i);
    eth_setenv_enetaddr(name, enetaddr);
  }
}

int
fru_mrec_update_bootdevice(struct fru *f, uint8_t *bootdevice) {
  int i = 0;
  int len = strlen((char *)bootdevice);
  len = (len>FRU_STR_MAX?FRU_STR_MAX:len);
  memset(f->bootdevice, 0, FRU_STR_MAX);
  memcpy(f->bootdevice, bootdevice, (len>FRU_STR_MAX?FRU_STR_MAX:len));
  flog("Checking if bootdev mrec already exists\n");
  for (; i<f->mrec_count; i++) {
    if (f->mrec[i].type == MR_SATADEV_REC) {
      flog("Found bootdev mrec, updating\n");
      f->mrec[i].data = f->bootdevice;
      f->mrec[i].length = len;
      return 0;
    }
  }
  flog("Bootdev mrec not found, creating mrec %i\n", f->mrec_count);
  //no mrec for sata found, creating one
  struct multirec *m = &(f->mrec[f->mrec_count]);
  m->type = MR_SATADEV_REC;
  m->format = 2;
  m->end = true;
  m->length = len;
  m->data = f->bootdevice;
  f->mrec_count ++;
  return -1;
}

int
fru_mrec_update_passwd_line(struct fru *f, uint8_t *passwd_line) {
  int i = 0;
  int len = strlen((char *)passwd_line);
  len = (len>FRU_PWD_MAX?FRU_PWD_MAX:len);
  memset(f->passwd_line, 0, FRU_PWD_MAX);
  memcpy(f->passwd_line, passwd_line, (len>FRU_PWD_MAX?FRU_PWD_MAX:len));
  flog("Checking if passwd line mrec already exists\n");
  for (; i<f->mrec_count; i++) {
    if (f->mrec[i].type == MR_PASSWD_REC) {
      flog("Found passwd line mrec, updating\n");
      f->mrec[i].data = f->passwd_line;
      f->mrec[i].length = len;
      return 0;
    }
  }
  flog("Passwd line mrec not found, creating mrec %i\n", f->mrec_count);
  struct multirec *m = &(f->mrec[f->mrec_count]);
  m->type = MR_PASSWD_REC;
  m->format = 2;
  m->end = true;
  m->length = len;
  m->data = f->passwd_line;
  f->mrec_count ++;
  return -1;
}

int
fru_mrec_update_test_ok(struct fru *f, uint8_t test_ok) {
  int i = 0;
  f->test_ok = test_ok;
  flog("Checking if test ok mrec already exists\n");
  for (; i<f->mrec_count; i++) {
    if (f->mrec[i].type == MR_TESTOK_REC) {
      flog("Found test ok mrec, updating\n");
      f->mrec[i].data = &f->test_ok;
      f->mrec[i].length = 1;
      return 0;
    }
  }
  flog("Test ok mrec not found, creating mrec %i\n", f->mrec_count);
  struct multirec *m = &(f->mrec[f->mrec_count]);
  m->type = MR_TESTOK_REC;
  m->format = 2;
  m->end = true;
  m->length = 1;
  m->data = &f->test_ok;
  f->mrec_count ++;
  return -1;
}

int
fru_mrec_update_power_policy(struct fru *f, enum POWER_POLICY pp) {
  int i = 0;
  f->power_policy = pp;
  flog("Checking if power policy mrec already exists\n");
  flog("Power policy value: %i\n", f->power_policy);
  for (; i<f->mrec_count; i++) {
    if (f->mrec[i].type == MR_POWER_POLICY_REC) {
      flog("Found power policy mrec, updating\n");
      f->mrec[i].data = &f->power_policy;
      flog("Updating data: %02x\n", f->mrec[i].data[0]);
      f->mrec[i].length = 1;
      return 0;
    }
  }
  flog("Power policy mrec not found, creating mrec %i\n", f->mrec_count);
  struct multirec *m = &(f->mrec[f->mrec_count]);
  m->type = MR_POWER_POLICY_REC;
  m->format = 2;
  m->end = true;
  m->length = 1;
  m->data = &f->power_policy;
  flog("Setting data: %02x\n", m->data[0]);
  f->mrec_count ++;
  return -1;
}

int
fru_mrec_update_power_state(struct fru *f) {
  int i = 0;
  f->power_state = 1;
  flog("Checking if power state mrec already exists\n");
  for (; i<f->mrec_count; i++) {
    if (f->mrec[i].type == MR_POWER_STATE_REC) {
      flog("Found power policy mrec, updating\n");
      f->mrec[i].data = &f->power_state;
      f->mrec[i].length = 1;
      return 0;
    }
  }
  flog("Power policy mrec not found, creating mrec %i\n", f->mrec_count);
  struct multirec *m = &(f->mrec[f->mrec_count]);
  m->type = MR_POWER_STATE_REC;
  m->format = 2;
  m->end = true;
  m->length = 1;
  m->data = &f->power_state;
  f->mrec_count ++;
  return -1;
}

int
read_fru(uint8_t *fru_buf) {
  int ret = 0;
  int i;
  fru_dbg("Reading eeprom\n");
  if (i2c_set_bus_num(CONFIG_SYS_FRU_BUS_NUM)) {
		return -1;
  }

  for (i=0;i<FRU_SIZE;i+=32) {
    ret = i2c_read(CONFIG_SYS_FRU_I2C_ADDR | 1, i, 2, fru_buf+i, 32);
    if (ret != 0) {
      ferr("FRU: failed to read eeprom [%i]\n", ret);
      return -1;
    }

#ifdef FRU_DEBUG
    int j;
    for (j=i;j<32+i;j++) {
      if ((j%8)==0) {
        fru_dbg("\n");
      }
      fru_dbg("%02x[%c] ", fru_buf[j], (fru_buf[j]>' '?fru_buf[j]:' '));
    }
    fru_dbg("\n");
#endif
  }
  return 0;
}

int
fru_update_mrec_eeprom(void) {
  int i = 0;
  int ret = 0;
  memcpy(fru_buf2, fru_buf, fru.mrec_area_offset);
  printf("Put multirecord area at %i\n", fru.mrec_area_offset);
  ret = fru_mk_multirecords_area(&fru, fru_buf2+fru.mrec_area_offset, FRU_SIZE-fru.mrec_area_offset);
  if (ret < 0) {
    return -1;
  }
  flog("Writing eeprom ");
  if (i2c_set_bus_num(CONFIG_SYS_FRU_BUS_NUM)) {
		return -2;
  }

  for (i=0;i<FRU_SIZE;i+=32) {
    ret = i2c_write(CONFIG_SYS_FRU_I2C_ADDR, i, 2, fru_buf2+i, 32);
    if (ret != 0) {
      ferr("FRU: failed to write eeprom [%i]\n", ret);
      return -3;
    }

#ifdef FRU_DEBUG
    int j;
    for (j=i;j<32+i;j++) {
      if ((j%8)==0) {
        fru_dbg("\n");
      }
      fru_dbg("0x%02x[%c] ", fru_buf2[j], (fru_buf2[j]>' '?fru_buf2[j]:' '));
    }
    fru_dbg("\n");
#else
    fmsg(".");
#endif
    udelay(5000);

  }
  fmsg("\n");
  return 0;
}

int
fru_open_parse(void) {
  int i = 0;
  memset(fru.bootdevice, 0, FRU_STR_MAX);
  fru.mac0 = fru.mac_data;
  fru.mac1 = fru.mac_data+6;
  fru.mac2 = fru.mac_data+12;
  read_fru(fru_buf);
  if (parse_fru(&fru, fru_buf, FRU_SIZE) != 0) {
    return -2;
  }
  for (i=0; i<fru.mrec_count; i++) {
    if (fru.mrec[i].type == MR_MAC_REC) {
      memcpy(fru.mac_data, fru.mrec[i].data, 6);
      fru_dbg("FRU: found MAC mrec [%02x %02x %02x %02x %02x %02x]\n", fru.mac_data[0], fru.mac_data[1], fru.mac_data[2], fru.mac_data[3], fru.mac_data[4], fru.mac_data[5]);
    } else if (fru.mrec[i].type == MR_MAC2_REC) {
      memcpy(fru.mac_data+6, fru.mrec[i].data, 6);
      fru_dbg("FRU: found MAC2 mrec [%02x %02x %02x %02x %02x %02x]\n", fru.mac_data[6], fru.mac_data[7], fru.mac_data[8], fru.mac_data[9], fru.mac_data[10], fru.mac_data[11]);
    } else if (fru.mrec[i].type == MR_MAC3_REC) {
      memcpy(fru.mac_data+12, fru.mrec[i].data, 6);
      fru_dbg("FRU: found MAC3 mrec [%02x %02x %02x %02x %02x %02x]\n", fru.mac_data[12], fru.mac_data[13], fru.mac_data[14], fru.mac_data[15], fru.mac_data[16], fru.mac_data[17]);
    } else if (fru.mrec[i].type == MR_SATADEV_REC) {
      memset(fru.bootdevice, 0, FRU_STR_MAX);
      memcpy(fru.bootdevice, fru.mrec[i].data, (fru.mrec[i].length>FRU_STR_MAX?FRU_STR_MAX:fru.mrec[i].length));
      fru_dbg("FRU: found SATA boot device [%s]\n", fru.bootdevice);
    } else if (fru.mrec[i].type == MR_PASSWD_REC) {
      memset(fru.passwd_line, 0, FRU_PWD_MAX);
      memcpy(fru.passwd_line, fru.mrec[i].data, (fru.mrec[i].length>FRU_PWD_MAX?FRU_PWD_MAX:fru.mrec[i].length));
      fru_dbg("FRU: found passwd line [%s]\n", fru.passwd_line);
    } else if (fru.mrec[i].type == MR_TESTOK_REC) {
      fru.test_ok = 0;
      memcpy(&fru.test_ok, fru.mrec[i].data, 1);
      fru_dbg("FRU: found test ok record [0x%02x]\n", fru.test_ok);
    } else if (fru.mrec[i].type == MR_POWER_POLICY_REC) {
      fru.power_policy = 0;
      memcpy(&fru.power_policy, fru.mrec[i].data, 1);
      fru_dbg("FRU: found power policy record [0x%02x]\n", fru.power_policy);
    }
  }

  printf("P/N:   %s\n", fru.val_part_number);
  printf("S/N:   %s\n", fru.val_serial_number);

  fru_setall_macaddr();

  setenv("board_serial", (char *)fru.val_serial_number);
  setenv("board_rev", (char *)fru.val_part_number);
  setenv("board_name", (char *)fru.val_product_name);

  return 0;
}
