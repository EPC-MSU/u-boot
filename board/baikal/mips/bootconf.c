#include <common.h>
#include <asm/gpio.h>
#include <i2c.h>
#include "bootconf.h"
#include "fru.h"

//#define DEBUG

#define BMC_ID1_VAL  0x49
#define BMC_ID2_VAL  0x54
#define BMC_ID3_VAL  0x58
#define BMC_ID4_VAL0 0x32
#define BMC_ID4_VAL1 0x02

#define PCA9535A_CR1 0x06
#define PCA9535A_CR2 0x07
#define PCA9535A_OR1 0x02
#define PCA9535A_OR2 0x03

enum BMC_I2C_REGS {
  R_ID1 = 0,
  R_ID2,
  R_ID3,
  R_ID4,
  R_SOFTOFF_RQ,
  R_PWROFF_RQ,
  R_PWRBTN_STATE,
  R_VERSION1,
  R_VERSION2,
  R_BOOTREASON,
  R_BOOTREASON_ARG,
  R_SCRATCH1,
  R_SCRATCH2,
  R_SCRATCH3,
  R_SCRATCH4,
  R_COUNT
};

#define TST_ARR_SZ 6

uint8_t bmc_version[3];

void *
memmem(const void *haystack, size_t haystack_len, const void *needle, size_t needle_len) {
  /* not really Rabin-Karp, just using additive hashing */
  char* haystack_ = (char*)haystack;
  char* needle_ = (char*)needle;
  int hash = 0;		/* this is the static hash value of the needle */
  int hay_hash = 0;	/* rolling hash over the haystack */
  char* last;
  size_t i;

  if (haystack_len < needle_len)
    return NULL;

  if (!needle_len)
    return haystack_;

  /* initialize hashes */
  for (i = needle_len; i; --i)
    {
      hash += *needle_++;
      hay_hash += *haystack_++;
    }

  /* iterate over the haystack */
  haystack_ = (char*)haystack;
  needle_ = (char*)needle;
  last = haystack_+(haystack_len - needle_len + 1);
  for (; haystack_ < last; ++haystack_) {
    if (__builtin_expect(hash == hay_hash, 0) &&
        *haystack_ == *needle_ &&	/* prevent calling memcmp, was a optimization from existing glibc */
        !memcmp (haystack_, needle_, needle_len))
      return haystack_;
    
    /* roll the hash */
    hay_hash -= *haystack_;
    hay_hash += *(haystack_+needle_len);
  }
  
  return NULL;
}

static char *
_skip_spaces(char *ptr, int *len) {
  int l = *len;
  for (; l>0; l--) {
    char b = *ptr;
    if (b != ' ') {
      *len = l;
      return ptr;
    }
    ptr ++;
  }
  *len = l;
  return NULL;
}

static char *
_skip(char *ptr, int *len) {
  int l = *len;
  for (; l>0; l--) {
    char b = *ptr;
    if (b == ' ') {
      *len = l;
      return ptr;
    }
    ptr ++;
  }
  *len = l;
  return NULL;
}

static char *
_store_string(char *src, int *src_len, char *buf, int *buf_len) {
  int l = *src_len;
  int bl = *buf_len;
  for (; l>0; l--) {
    char b = *src;
    if (b == ' ') {
      break;
    }
    if ((b == '\n') || (b == '\r')) {
      *src_len = l;
      *buf_len = bl;
      return NULL;      
    }

    *buf = b;
    bl--;
    if (bl==0) {
      *src_len = l;
      *buf_len = bl;
      return NULL;
    }
    buf++;
    src ++;
  }
  *src_len = l;
  *buf_len = bl;
  return src;
}
/*
Bootconf format:

default bootconf<x>
bootconf<x> <kernel_name> <load addr> <entry addr> <dtb_name> <initrd_name> <args>
bootconf<y> ...

Example:
default bootconf0
bootconf0 vmlinux.bin 0x80004000 0x80004000 baikal.dtb initrd.img root=/dev/sda6 rw rootwait earlyprintk=uart8520,mmio32,0x1f04a000,115200 maxcpus=2 consoleblank=0 console=tty1 console=ttyS0,115200n8 video=sm750fb:1280x1024-32@60 nohtw nocoherentio ieee754=relaxed
*/
int
tp_bootconf(char *bootconf_data, int bcd_len, char *kernel_buf, int kb_len, uint32_t *loadaddr, uint32_t *entryaddr, char *dtb_buf, int dtb_len, char *initrd_buf, int ird_len, char *args_buf, int args_len) {
  static const char default_str[] = "default";
  static const char bootconf_str[] = "bootconf";
  int d = 0;
  void *ptr = NULL;
  char *bptr = NULL;
  void *bootconf = NULL;
  int conf = 0;
  char buf[16] = {0};
  void *def = NULL;
  char *end;

  printf("Parsing bootconf\n");
#ifdef DEBUG
  {
    int i = 0;
    for (;i<bcd_len;i++) {
      if (i%8==0) {
        printf("\n");
      }
      printf("%02x[%c]", bootconf_data[i], (bootconf_data[i]>=' '?bootconf_data[i]:' '));
    }
    printf("\n");
  }
#endif

  def = memmem(bootconf_data, bcd_len, default_str, strlen(default_str));
  if (def == NULL) {
    printf("Bootconf: default entry not found\n");
    return -1;
  }

  d = (int)((char *)def-bootconf_data);
  //find eol
  ptr = memmem(def, bcd_len-d, "\n", 1);
  if (ptr == NULL) {
    printf("Bootconf: bad default entry line\n");
    return -2;
  }
  d = (int)(ptr - def);
  bootconf = memmem(def, d, bootconf_str, strlen(bootconf_str));
  conf = simple_strtoul(bootconf+strlen(bootconf_str), &end, 10);
  snprintf(buf, 15, "bootconf%i", conf);
  printf("Looking for bootconf%i\n", conf);
  d = (int)((char *)ptr-bootconf_data);
  bootconf = memmem(ptr, bcd_len-d, buf, strlen(buf));
  if (bootconf == NULL) {
    printf("Bootconf: conf line not found\n");
    return -3;
  }
  d = (int)((char *)bootconf-bootconf_data);
  d = bcd_len-d;
  bptr = (char *)bootconf;
  //skip bootconf line
  for (; d>0; d--) {
    char b = *bptr;
    if (b == ' ') {
      break;
    }
    bptr ++;
  }
  bptr = _skip_spaces(bptr, &d);
  if (bptr == NULL) {
    printf("Bootconf: failed to find kernel string\n");
    return -4;
  }
  bptr = _store_string(bptr, &d, kernel_buf, &kb_len);
  if (bptr == NULL) {
    if (kb_len == 0) {
      printf("Bootconf: kernel name buffer is too short\n");
    } else {
      printf("Bootconf: early end of line while parsing kernel name\n");
    }
    return -5;
  }

  //load address
  bptr = _skip_spaces(bptr, &d);
  if (bptr == NULL) {
    printf("Bootconf: failed to find load address\n");
    return -6;
  }
  *loadaddr = simple_strtoul(bptr, &end, 16);
  bptr = _skip(bptr, &d);
  if (bptr == NULL) {
    printf("Bootconf: failed to parse load address\n");
    return -7;
  }

  //entry address
  bptr = _skip_spaces(bptr, &d);
  if (bptr == NULL) {
    printf("Bootconf: failed to find entry address\n");
    return -8;
  }
  *entryaddr = simple_strtoul(bptr, &end, 16);
  bptr = _skip(bptr, &d);
  if (bptr == NULL) {
    printf("Bootconf: failed to parse entry address\n");
    return -9;
  }

  //DTB
  bptr = _skip_spaces(bptr, &d);
  if (bptr == NULL) {
    printf("Bootconf: failed to find DTB string\n");
    return -10;
  }
  bptr = _store_string(bptr, &d, dtb_buf, &dtb_len);
  if (bptr == NULL) {
    if (dtb_len == 0) {
      printf("Bootconf: DTB name buffer is too short\n");
    } else {
      printf("Bootconf: early end of line while parsing DTB name\n");
    }
    return -11;
  }

  //initrd
  bptr = _skip_spaces(bptr, &d);
  if (bptr == NULL) {
    printf("Bootconf: failed to find initrd string\n");
    return -12;
  }
  bptr = _store_string(bptr, &d, initrd_buf, &ird_len);
  if (bptr == NULL) {
    if (ird_len == 0) {
      printf("Bootconf: initrd name buffer is too short\n");
    } else {
      printf("Bootconf: early end of line while parsing initrd name\n");
    }
    return -13;
  }

  //args
  bptr = _skip_spaces(bptr, &d);
  if (bptr == NULL) {
    printf("Bootconf: failed to find args string\n");
    return -14;
  }
  for (;d>0;d--) {
    char b = *bptr;
    if ((b=='\r') || (b=='\n')) {
      break;
    }
    *args_buf = b;
    args_buf++;
    bptr++;
    args_len--;
    if (args_len==0) {
      printf("Bootconf: args buffer is too short\n");
    return -15;
    }
  }
  return 0;
}

void
tp_check_boot(void) {
  char *bootnormal __maybe_unused;
  char *bootrecovery __maybe_unused;
  printf("PWRBTN: ");

  if (getenv_yesno("bootnormal") == -1) {
    printf("bootnormal is missing in ENV, fall back to defaults\n");
    return;
  }
  if (getenv_yesno("bootrecovery") == -1) {
    printf("bootrecovery is missing in ENV, fall back to defaults\n");
    return;
  }
  bootnormal = getenv("bootnormal");
  bootrecovery = getenv("bootrecovery");
#if defined(CONFIG_LONGPRESS)
  int err;
  if (fru.test_ok == 2) {
    tp_bmc_set_bootreason(TEST_FAIL, 0);
    printf("boot recovery\n");
    setenv("bootcmd", bootrecovery);
  } else if (fru.test_ok == 0) {
    tp_bmc_set_bootreason(NOT_TESTED, 0);
    printf("boot recovery\n");
    setenv("bootcmd", bootrecovery);
  } else {
    printf("R_PWRBTN_STATE - ");
    err = i2c_reg_read(CONFIG_SYS_BMC_I2C_ADDR, R_PWRBTN_STATE);
    if (err < 0) {
      printf("Failed to read register 0x%02x from 0x%02x: %i\n", R_PWRBTN_STATE, CONFIG_SYS_BMC_I2C_ADDR, err);
      return;
    }
    if (err == 0) {
      printf("boot normal\n");
      setenv("bootcmd", bootnormal);
    } else {
      printf("boot recovery\n");
      setenv("bootcmd", bootrecovery);
    }
    tp_bmc_set_bootreason(NORMAL, 0);
  }
#elif defined(CONFIG_JUMPER)
  char tst_arr[TST_ARR_SZ] = "glhvwo";
  int i = 0;
  bool normal = true;
  char c;
  printf("Read whatever is in UART\n");
  mdelay(100);
  while (1) {
    if(tstc() != 0) {
      c = getc();
    } else {
      break;
    }
  }
  for (;i<TST_ARR_SZ;i++) {
    putc(tst_arr[i]);
    mdelay(1);
    if (tstc() != 0) {
      c = getc();
      if (c==tst_arr[i]) {
        continue;
      } else {
        printf("[%i] character \"%02x\" not equal \"%02x\"\n", i, c, tst_arr[i]);
        if (tstc() != 0) {
          c = getc();
          printf("Next: %02x\n", c);
        }
      }
    } else {
      printf("[%i]t returned 0\n", i);
    }
    printf("MODE:  boot recovery\n");
    setenv("bootcmd", bootrecovery);
    normal = false;
    break;
  }
  if (normal) {
    printf("MODE:  boot normal\n");
    setenv("bootcmd", bootnormal);
    extern bool disable_console_getc;
    disable_console_getc = true;
    mdelay(100);
    while (1) {
      if(tstc() != 0) {
        c = getc();
      } else {
        break;
      }
    }
  }
  tp_bmc_set_bootreason(NORMAL, 0);
#endif
}

void
tp_bmc_set_bootreason(uint8_t reason, uint8_t arg) {
  if (bmc_version[0]>=2) {
    printf("BMC:   Setting bootreason: %i\n", reason);
    i2c_reg_write(CONFIG_SYS_BMC_I2C_ADDR, R_BOOTREASON, reason);
    udelay(100000);
    i2c_reg_write(CONFIG_SYS_BMC_I2C_ADDR, R_BOOTREASON_ARG, arg);
    /* Read the value back so to workaround a bug in BMC i2c-slave interface,
     * which causes the overwritten value returned on the next read operation.
     */
    (void)i2c_reg_read(CONFIG_SYS_BMC_I2C_ADDR, R_BOOTREASON_ARG);
  } else {
    printf("BMC:   Bootreason setting is not supported\n");
  }
}

void
tp_bmc_get_version(void) {
  int i = 0;
  int err;
  uint8_t vals[] = {BMC_ID1_VAL, BMC_ID2_VAL, BMC_ID3_VAL};
  bmc_version[0] = 0;
  bmc_version[1] = 0;
  bmc_version[2] = 0;
  printf("BMC:   ");
  for (i=R_ID1;i<=R_ID3;i++) {
    debug("Reading %i\n", i);
    err = i2c_reg_read(CONFIG_SYS_BMC_I2C_ADDR, i);
    debug("ret: %i\n", err);
    if (err < 0) {
      printf("Failed to read register 0x%02x from 0x%02x: %i\n", i, CONFIG_SYS_BMC_I2C_ADDR, err);
      return;
    }
    if (err!=vals[i]) {
      printf("Unknown ID[%i] value: 0x%02x\n", i, err);
      return;
    }
  }
  debug("Reading %i\n", R_ID4);
  err = i2c_reg_read(CONFIG_SYS_BMC_I2C_ADDR, R_ID4);
  debug("ret: %i\n", err);
  if (err < 0) {
    printf("Failed to read register 0x%02x from 0x%02x: %i\n", R_ID4, CONFIG_SYS_BMC_I2C_ADDR, err);
    return;
  }
  if (err == BMC_ID4_VAL0) {
    bmc_version[0] = 0;
    printf("v0.0.0\n");
  } else if (err == BMC_ID4_VAL1) {
    bmc_version[0] = 2;
    debug("Reading %i\n", R_VERSION1);
    err = i2c_reg_read(CONFIG_SYS_BMC_I2C_ADDR, R_VERSION1);
    debug("ret: %i\n", err);
    if (err < 0) {
      printf("Failed to read register 0x%02x from 0x%02x: %i\n", R_VERSION1, CONFIG_SYS_BMC_I2C_ADDR, err);
      return;
    }
    bmc_version[1] = err;
    debug("Reading %i\n", R_VERSION2);
    err = i2c_reg_read(CONFIG_SYS_BMC_I2C_ADDR, R_VERSION2);
    debug("ret: %i\n", err);
    if (err < 0) {
      printf("Failed to read register 0x%02x from 0x%02x: %i\n", R_VERSION2, CONFIG_SYS_BMC_I2C_ADDR, err);
      return;
    }
    bmc_version[2] = err;
    printf("v%i.%i.%i\n", bmc_version[0], bmc_version[1], bmc_version[2]);
  } else {
    printf("Unknown version [0x%02x] in register 0x%02x\n", err, R_ID4);
  }
}
