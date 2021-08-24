#include <common.h>
#include <asm/io.h>
#include "pvt.h"

#define HWMON_BASE 0xBF200000

/* PVT registers */
#define BK_PVT_CTRL     0x00
#define BK_PVT_DATA     0x04
#define BK_PVT_TTHRES   0x08
#define BK_PVT_VTHRES   0x0C
#define BK_PVT_TTIMEOUT 0x1C
#define BK_PVT_INTR_STAT 0x20
#define BK_PVT_INTR_MASK 0x24
#define BK_PVT_CLR_INTR 0x2C
/* PVT VALID bit reads TIMEOUT */
#define BK_PVT_VALID_TIMEOUT 10000

/* PVT VALUES and MASKS */
#define BK_PVT_CTRL_EN_BIT  0x1
#define BK_PVT_CTRL_TMOD    0x0
#define BK_PVT_CTRL_VMOD    0x2
#define BK_PVT_CTRL_LVTMOD  0b0100
#define BK_PVT_CTRL_HVTMOD  0b1000
#define BK_PVT_CTRL_SVTMOD  0b1100

#define BK_PVT_INTR_MASK_TONLY  0x7F9
#define BK_PVT_INTR_MASK_TVONLY 0x7E1
#define BK_PVT_INTR_MASK_ALL  0x7FF

#define BK_PVT_DATA_MASK    0x3ff
#define BK_PVT_DATA_VALID   (1 << 10)

#define BK_PVT_THRES_HI    0xFFC00
#define BK_PVT_THRES_LO    0x3FF

#define BK_PVT_TTIMEOUT_SET 10000000

#define BK_PVT_INTR_STAT_TTHRES_LO 0x02
#define BK_PVT_INTR_STAT_TTHRES_HI 0x04
#define BK_PVT_INTR_STAT_VTHRES_LO 0x08
#define BK_PVT_INTR_STAT_VTHRES_HI 0x10

/* TEMP limits */
#define TEMP_PVT_MAX 125000
#define TEMP_PVT_MIN -40000
/* Voltage limits */
#define VOLT_PVT_MAX 800
#define VOLT_PVT_MIN 1000

/* coef for transformtion to T,C (10^-3) times 10^6
 * DATA = BK_PVT_DATA [0:9]
 * T =  COEF4 * DATA ^ 4 + COEF3 * DATA ^ 3 + COEF2 * DATA ^ 2 +
 *    + COEF1 * DATA ^ 1 + COEF0 */
#define COEF4   (-16743 )        /* (-1.6743E-11f)  * 10^15 */
#define COEF3   (81542  )        /* (8.1542E-08f)   * 10^12 */
#define COEF2   (-182010)        /* (-1.8201E-04f)  * 10^9  */
#define COEF1   (310200 )        /* (3.1020E-01f)   * 10^6  */
#define COEF0   (-48380 )        /* (-4.8380E+01f)  * 10^3  */

/* coef for transformation T,C (10^-3) to DATA
 * DATA = DCOEF3 * T^3 + DCOEF2 * T ^ 2 + DCOEF1 * T + DCOEF 0 */

#define DCOEF3  (2617)
#define DCOEF2  (8654)
#define DCOEF1  (3923)
#define DCOEF0  (172 )

/*  coef for transformatio to V, mV
 *  DATA = 1865.8 *  VOLTAGE- 1157.2 =>
 *  VOLTAGE = 620 + data * 10000 / 18658;
*/
#define COEF0_V 620
#define COEF1_V 18658

static int hwmon_mon_mod = 0;
static int firstrun = 1;

static void switch_pvt_mod( uint32_t addr, long int mod)
{
  debug("BK PVT now %x, but need %lx \n",readl(addr+BK_PVT_CTRL), (unsigned long)mod);
  writel( 0, addr + BK_PVT_CTRL);
  //Set timeout of PVT measurment
  writel( 0, addr + BK_PVT_TTIMEOUT);
  //Mask all interupts
  writel( BK_PVT_INTR_MASK_ALL, addr + BK_PVT_INTR_MASK);
  writel( mod, addr + BK_PVT_CTRL);
  writel( ((BK_PVT_CTRL_EN_BIT)|mod), addr + BK_PVT_CTRL);
  debug("BK PVT MOD %x\n",readl(addr+BK_PVT_CTRL));
}

static void switch_to_mon_mod(void) {
  //OFF PVT
  writel( 0, HWMON_BASE + BK_PVT_CTRL);
  //Set timeout of inerupts
  writel( BK_PVT_TTIMEOUT_SET, HWMON_BASE + BK_PVT_TTIMEOUT);
  debug("pvt switch_to_mon_mod and set BK_PVT_TTIMEOUT %d\n",readl(HWMON_BASE + BK_PVT_TTIMEOUT));
  //Mask all interupts
  writel( BK_PVT_INTR_MASK_TVONLY, HWMON_BASE + BK_PVT_INTR_MASK);
  //Switch to last VOLT or Temprature mon_mod
  writel( ((hwmon_mon_mod)<<1), HWMON_BASE + BK_PVT_CTRL);
  debug("pvt switch_to_mon_mod and set BK_PVT_CTRL %d\n",readl(HWMON_BASE + BK_PVT_CTRL));
  //ON PVT
  writel( (BK_PVT_CTRL_EN_BIT)| ((hwmon_mon_mod)<<1), HWMON_BASE + BK_PVT_CTRL);
}

static int read_valid_datareg(void)
{
  register int data, i = 0;
  data = readl(HWMON_BASE + BK_PVT_DATA);
  data = 0;
  while ( ! (data & (BK_PVT_DATA_VALID) )  ) {
    data = readl(HWMON_BASE + BK_PVT_DATA);
    if( ++i == BK_PVT_VALID_TIMEOUT ) {
      debug("Timeout reading valid datareg\n");
      return -EINVAL;
    }
  }

  data &= ( BK_PVT_DATA_MASK);
  switch_to_mon_mod();
  return data;
}

static int data2temp(int data)
{
  int temp, temp4, temp3, temp2, temp1, temp0;
  debug("pvt %d and data %d \n",( BK_PVT_DATA_MASK), data);
  /*Dont changer the order of multiplication !!! */
  temp4 = (COEF4) * data / 1000 * data / 1000 * data / 1000 * data / 1000;
  temp3 = (COEF3) * data / 1000 * data / 1000 * data / 1000;
  temp2 = (COEF2) * data / 1000 * data / 1000;
  temp1 = (COEF1) * data / 1000;
  temp0 = (COEF0) ;
  temp = temp0 + temp1 + temp2 + temp3 + temp4;
  debug("BK PVT temp  %d = %d + %d + %d + %d + %d \n",temp, temp4, temp3, temp2 ,temp1, temp0);
  return temp;
}

int pvt_get_temp(void) {
  int data, temp;

  //Set timeout of PVT measurment
  if (firstrun) {
    firstrun = 0;
    //Mask all interupts except TTRES_HILO
    writel( BK_PVT_INTR_MASK_TVONLY, HWMON_BASE + BK_PVT_INTR_MASK);
    debug("pvt_probe BK_PVT_INTR_MASK %x\n",readl(HWMON_BASE + BK_PVT_INTR_MASK));

    writel( BK_PVT_TTIMEOUT_SET, HWMON_BASE + BK_PVT_TTIMEOUT);
    debug("pvt_probe BK_PVT_TTIMEOUT %d\n",readl(HWMON_BASE + BK_PVT_TTIMEOUT));
    
    hwmon_mon_mod = 0;
    switch_to_mon_mod();
  }
  
  switch_pvt_mod(HWMON_BASE,BK_PVT_CTRL_TMOD);
  data = read_valid_datareg();
  temp = data2temp(data);

  //printf("Check IRQ\n");
  uint32_t val = readl( HWMON_BASE + BK_PVT_INTR_STAT);
  if (BK_PVT_INTR_STAT_TTHRES_LO & val)
    printf("PVT WARNING Lo Temperature \n");
  if (BK_PVT_INTR_STAT_TTHRES_HI & val)
    printf("PVT WARNING Hi Temperature \n");
  if (BK_PVT_INTR_STAT_VTHRES_LO & val)
    printf("PVT WARNING Lo Voltage \n");
  if (BK_PVT_INTR_STAT_VTHRES_HI & val)
    printf("PVT WARNING Lo Voltage \n");
  val = readl( HWMON_BASE + BK_PVT_CLR_INTR);
    
  return temp;
}
