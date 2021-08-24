#include <stdint.h>
#include <stdbool.h>
#include <config.h>
#include "llenv_spd.h"
#include <linux/string.h>
#include <linux/compiler.h>

//#define TEST_MODE to build standalone program to test regs calculation

int baikal_read_spd (uint32_t addr, int alen, uint8_t *buffer, int len);

#ifndef TEST_MODE
uint8_t  *ddr_buffer0 = (uint8_t  *) CONFIG_SRAM_BUF0_BASE;
uint32_t *ddr_buffer1 = (uint32_t *) CONFIG_SRAM_BUF1_BASE;
#else
uint8_t ddr_buffer0[256];
uint32_t ddr_buffer1[64];
#endif

#define LLENV_SET_SPD_REG(r,v)  ddr_buffer1[(r)]=(v)
#define read_spd(x) ddr_buffer0[x]

#define DLL_FAST_EXIT 0
#define MEMC_FREQ_RATIO 2
#define MEMC_MODE_2T 0
#define BL 8

#define tMRD    4       /* Mode Register Set cycle time (nCK) */
#define tREFI   7800000 /* Average Periodic Refresh interval (ps) */
#define tCCD    4       /* CAS to CAS delay (nCK) */
#define tWLMRD  40      /* nCK */

#ifndef CONFIG_DDR_FREQ_TWEAK
#define CONFIG_DDR_FREQ_TWEAK 0
#endif

#if CONFIG_DDR_FREQ_TWEAK == 0
/* nominal frequencies */
#define PLL_400		0x0e07f01	/* 400MHz */
#define PLL_333		0x0a04f01	/* 333MHz */
#define PLL_267		0x1607f01	/* 267MHz */
#elif CONFIG_DDR_FREQ_TWEAK == -1
#define PLL_400		0x0e07b01	/* 387MHz */
#define PLL_333		0x0a04c01	/* 320MHz */
#define PLL_267		0x1607b01	/* 258MHz */
#elif CONFIG_DDR_FREQ_TWEAK == -2
#define PLL_400		0x0e07701	/* 375MHz */
#define PLL_333		0x0a04a01	/* 312MHz */
#define PLL_267		0x1607701	/* 250MHz */
#elif CONFIG_DDR_FREQ_TWEAK == 1
#define PLL_400		0x0e08301	/* 412MHz */
#define PLL_333		0x0a05201	/* 345MHz */
#define PLL_267		0x1608301	/* 275MHz */
#elif CONFIG_DDR_FREQ_TWEAK == 2
#define PLL_400		0x0e08701	/* 425MHz */
#define PLL_333		0x0a05501	/* 358MHz */
#define PLL_267		0x1608701	/* 283MHz */
#else
#error "Specified CONFIG_DDR_FREQ_TWEAK not supported"
#endif


struct ddr3_timings {
	unsigned tCK;
	unsigned tAA;
	unsigned tRCD;
	unsigned tRP;
	unsigned tRC;
	unsigned tRASmin;
	unsigned tFAWmin;
	unsigned tRTP;
	unsigned tWRmin;
	unsigned tRRD;
	unsigned tRFC;
	unsigned tWTR;
};

struct ddr3_geometry {
	unsigned bus_width;
	unsigned ranks;
	unsigned whole_mem;
	unsigned rows;
	unsigned cols;
	unsigned ecc;
};

static unsigned roundup(unsigned x, unsigned y)
{
	return (x - 1) / y + 1;
}

static unsigned max(unsigned a, unsigned b)
{
	return (a > b) ? a : b;
}

static unsigned encode_wr(unsigned wr)
{
    if ((wr >= 5) && (wr <= 7)) {
        return (wr - 4);
    } else if (((wr % 2) == 0) && (wr >= 8) && (wr <= 14)) {
        return wr / 2;
    }
    return 0;
}

static unsigned encode_cl(unsigned cl)
{
	if (cl < 5)
		return 0;
	if (cl < 12)
		return (cl-4) << 1;

	return ((cl - 12) << 1) | 1;
}

static unsigned time2memc(unsigned t, int up)
{
	if (up)
		t = roundup(t, MEMC_FREQ_RATIO);
	else
		t /= MEMC_FREQ_RATIO;
	t += MEMC_MODE_2T;
	return t;
}

#define DDR3_MR0_PD_S	12
#define DDR3_MR0_WR_S	9
#define DDR3_MR0_CL_S0	2
#define DDR3_MR0_CL_S1	4

#define DDR3_MR1_AL_S	3
#define DDR3_MR1_AL_0	0
#define DDR3_MR1_AL_CL1	1
#define DDR3_MR1_AL_CL2	2
#if !defined(CONFIG_BAIKAL_T1)
#define DDR3_MR1_AL	DDR3_MR1_AL_CL1
#else
/*
 * vvv: The Doc recommends to avoid DRAMTMG4.t_rcd equal 1 when
 * MEMC_FREQ_RATIO=2, so disable additive latency.
 */
#define DDR3_MR1_AL	DDR3_MR1_AL_0
/*
 * However, performance degradation was not confirmed for the setting
 * #define DDR3_MR1_AL	DDR3_MR1_AL_CL1
 */
#endif

#define DDR3_MR1_DR_S   1
#define DDR3_MR1_RTT_S  2


#define DDR3_MR2_CWL_S	3
#define DDR3_MR2_RTT_S  9

#if defined(CONFIG_CUSTOM_SPD)
extern const uint8_t ddr_user_spd[];
#endif

#ifdef DDR_USER_REGS
const uint32_t ddr_user_regs [64] = {
    0x1, 0x2, 0x3, 0x4, 0x5
};
#endif

static uint16_t crc16 (int count)
{
    unsigned crc = 0, i, j;
    for (j = 0; j < count; ++j) {
        crc ^= read_spd(j) << 8;
        for (i = 0; i < 8; ++i) {
            unsigned next = crc << 1;
            if (crc & 0x8000)
                next ^= 0x1021;
            crc = next;
        }
    }
    return crc;
}

static const __maybe_unused char static_spd[8] = "Static\0";
static const __maybe_unused char eeprom_spd[12] = "I2C EEPROM\0";

int llenv_prepare_buffer0 (void)
{
    int rc = -1;
    unsigned cov, crc;

    memset(ddr_buffer1, 0, 256);

#if defined(CONFIG_BAIKAL_SPD_ADDRESS)
    memset(ddr_buffer0, 0, 256);
    rc = baikal_read_spd(0, 1, ddr_buffer0, 256);
    if (rc == 0) {
	memcpy(ddr_buffer0 + 128, eeprom_spd, 12);
        cov = (read_spd(0) & (1U << 7)) ? 116 : 125;
        crc = crc16(cov + 1);
        if (crc != ((read_spd(127) << 8) | read_spd(126)))
            rc = -1;
        else
            return 0;
    }
#endif
#if defined(CONFIG_CUSTOM_SPD)
    memcpy(ddr_buffer0, ddr_user_spd, 256);
    memcpy(ddr_buffer0 + 128, static_spd, 8);
    cov = (read_spd(0) & (1U << 7)) ? 116 : 125;
    crc = crc16(cov + 1);
    if (crc != ((read_spd(127) << 8) | read_spd(126)))
        rc = -1;
    else
        rc = 0;
#endif /* CONFIG_CUSTOM_SPD */

    return rc;
}

int llenv_prepare_buffer1 (void)
{
#ifdef DDR_USER_REGS
    memcpy(ddr_buffer1, ddr_user_regs, 256);
    return 0;

#else /* DDR_USER_REGS */

    struct ddr3_timings spd_t;
    struct ddr3_geometry spd_g;
    // all times in ps
    // ddr3 common
    int i;
  {
    /* SPD parsing part. TODO: move to a separate function. */

    spd_g.bus_width = (8 << (read_spd(8) & 7)) +
                      8 * ((read_spd(8) >> 3) & 7); /* this includes ECC bits and extra unused bits */
    unsigned chip_width = 4 << (read_spd(7) & 7);
    unsigned data_width; /* available data bits: 16 or 32 */
    if (spd_g.bus_width >= 32)
        data_width = 32;
    else if (spd_g.bus_width >= 16)
        data_width = 16;
    else
        return -1;

    spd_g.ranks = 1 + ((read_spd(7) >> 3) & 7);
    spd_g.whole_mem = (256 << (read_spd(4) & 0xf)) * data_width/chip_width/8*spd_g.ranks;
    spd_g.cols = 9 + (read_spd(5) & 7);
    spd_g.rows = 12 + (read_spd(5) >> 3);

    unsigned dv = 1000 * read_spd(10) / read_spd(11); // [ps]
#if CONFIG_DDR_CUSTOM_CLK >= 1066 && CONFIG_DDR_CUSTOM_CLK <= 1600
    /* Tck[ps] = 1000 * tck * MTB[ns] = 2 * 1000000 / Freq[MT] */
    spd_t.tCK = 2000000 / CONFIG_DDR_CUSTOM_CLK;
#else
    spd_t.tCK = read_spd(12) * dv;
#endif
    spd_t.tAA = read_spd(16) * dv;
    spd_t.tWRmin = read_spd(17) * dv;
    spd_t.tRCD = read_spd(18) * dv;
    spd_t.tRRD = read_spd(19) * dv;

    spd_t.tRP = read_spd(20) * dv;
    spd_t.tRC = (((read_spd(21) >> 4) << 8) | read_spd(23)) * dv;
    spd_t.tRASmin = (((0xf & read_spd(21)) << 8) | read_spd(22)) * dv;
    spd_t.tRTP = read_spd(27) * dv;
    spd_t.tRFC = ((read_spd(25) << 8) | read_spd(24)) * dv;
    spd_t.tWTR = read_spd(26) * dv;
    spd_t.tFAWmin = ((read_spd(28) << 8) | read_spd(29)) * dv;
  }

    unsigned CL = roundup(spd_t.tAA, spd_t.tCK);
    __maybe_unused unsigned half_width = 0, rank_mask;
    uint32_t t1, reg;
    uint32_t mr0, mr1, mr2;

    /* calculated vals used in more than one register */
    unsigned c_cke; /* tCKE in tCK units */
    unsigned c_xp; /* tXP in tCK units */
    unsigned c_mod; /* tMOD in tCK units */

    if (spd_t.tCK > 1500) {
        spd_t.tCK = 1875;
        LLENV_SET_SPD_REG(DDR_PLL_CTL, PLL_267); /* 267MHz */
    } else if (spd_t.tCK > 1250) {
        spd_t.tCK = 1500;
        LLENV_SET_SPD_REG(DDR_PLL_CTL, PLL_333); /* 333MHz */
    } else {
        spd_t.tCK = 1250;
        LLENV_SET_SPD_REG(DDR_PLL_CTL, PLL_400); /* 400MHz */
    }

    unsigned AL = 0;
    // MR0
    mr0 = (encode_wr(roundup(spd_t.tWRmin, spd_t.tCK)) << DDR3_MR0_WR_S) |
          ((encode_cl(CL) & 0xe) << (DDR3_MR0_CL_S1 - 1)) |
          ((encode_cl(CL) & 1) << DDR3_MR0_CL_S0);
    // MR1
    mr1 = DDR3_MR1_AL << DDR3_MR1_AL_S;
    mr1 |= 1 << DDR3_MR1_DR_S; // 34 Ohm
    mr1 |= 1 << DDR3_MR1_RTT_S; // 60 Ohm

    if (DDR3_MR1_AL == DDR3_MR1_AL_CL1) {
        AL = CL - 1;
    } else if (DDR3_MR1_AL == DDR3_MR1_AL_CL2) {
        AL = CL - 2;
    }

    if (DLL_FAST_EXIT)
	    mr1 |= 1 << DDR3_MR0_PD_S;
    reg = (mr0 << 16) | mr1;
    LLENV_SET_SPD_REG(DDR3_SPD_INIT3, reg);

    // MR2
    unsigned CWL;
    if (spd_t.tCK < 1500)
        CWL = 8;
    else if (spd_t.tCK < 1875)
        CWL = 7;
    else
        CWL = 6;

    mr2 = (CWL - 5) << DDR3_MR2_CWL_S;
#if !defined(CONFIG_BAIKAL_T1)
    // ...
#else
    mr2 |= 1 << DDR3_MR2_RTT_S; // Rtt_WR(RZQ/4) 60 Ohm
#endif
    reg = mr2 << 16;
    LLENV_SET_SPD_REG(DDR3_SPD_INIT4, reg);

#if !defined(CONFIG_BAIKAL_T1)
    reg = 0x02918210;
#else
    reg = 0x02808200 | /* rddata_en */ ((CL + AL - 4) << 16) | /* wrlat */  (CWL + AL - 2);
#endif
    LLENV_SET_SPD_REG(DDR3_SPD_DFITMG0, reg);

    reg = 0x00080404;
    LLENV_SET_SPD_REG(DDR3_SPD_DFITMG1, reg);


    reg = 0;
    //wr2pre = WL + BL/2 + tWRmin
    // WL = AL + CWL
    t1 = time2memc(CWL + AL + BL/2 + spd_t.tWRmin/spd_t.tCK, false);
    reg |= t1 << 24;
    // t_faw
    t1 = time2memc(roundup(spd_t.tFAWmin, spd_t.tCK), true);
    reg |= t1 << 16;
    // t_ras_max = tRASmax/1024 = (tREFI * 9)/1024
    t1 = time2memc(((tREFI * 9) / spd_t.tCK - 1) / 1024, false);
#if !defined(CONFIG_BAIKAL_T1) || 1 // vvv: t1 already divided by MEMC_FREQ_RATIO
    // ...
#else
    if (MEMC_FREQ_RATIO == 2) {
        t1 = (t1 - 1) / 2;
    }
#endif
    reg |= t1 << 8;
    // t_ras_min
    t1 = time2memc(spd_t.tRASmin / spd_t.tCK, false);
#if !defined(CONFIG_BAIKAL_T1) || 1 //vvv: same as above
    // ...
#else
    if (MEMC_FREQ_RATIO == 2) {
        t1 = (t1) / 2;
    }
#endif
    reg |= t1;
    LLENV_SET_SPD_REG(DDR3_SPD_DRAMTMG0, reg);


    reg = 0;
    // t_xp
    c_xp = (DLL_FAST_EXIT)?
            max(3, roundup((spd_t.tCK < 1875) ? 6000 : 7500, spd_t.tCK)):
            max(10, roundup(24000, spd_t.tCK));
    t1 = time2memc(c_xp, true);
    reg |= t1 << 16;
    // rd2pre: AL + max(4CK, tRTP)
    t1 = max(4, roundup(spd_t.tRTP, spd_t.tCK));
    t1 = time2memc(t1 + AL, false);
    reg |= t1 << 8;
    // t_rc
    t1 = time2memc(roundup(spd_t.tRC, spd_t.tCK), true);
    reg |= t1;
    LLENV_SET_SPD_REG(DDR3_SPD_DRAMTMG1, reg);


    reg = 0;
    // rd2wr = RL + BL/2 + 2 - WL
    t1 = time2memc(CL + BL/2 + 2 - CWL, true);
    reg |= t1 << 8;
    // wr2rd = CWL + BL/2 + tWTR
    t1 = time2memc(roundup(spd_t.tWTR, spd_t.tCK) + CWL + BL/2, true);
    reg |= t1;
    LLENV_SET_SPD_REG(DDR3_SPD_DRAMTMG2, reg);


    reg = 0;
    // t_mrd
    t1 = time2memc(tMRD, true);
    reg |= t1 << 12;
    // t_mod
    c_mod = max(12, roundup(15000, spd_t.tCK));
    t1 = time2memc(c_mod, true);
    reg |= t1;
    LLENV_SET_SPD_REG(DDR3_SPD_DRAMTMG3, reg);


    reg = 0;
    // t_rcd
    t1 = time2memc(roundup(spd_t.tRCD, spd_t.tCK) - AL, true);
    reg |= t1 << 24;
    // t_ccd
    t1 = time2memc(tCCD, true);
    reg |= t1 << 16;
    // t_rrd
    t1 = time2memc(roundup(spd_t.tRRD, spd_t.tCK), true);
    reg |= t1 << 8;
    // t_rp
#if !defined(CONFIG_BAIKAL_T1)
    t1 = time2memc(spd_t.tRP/spd_t.tCK, true) + MEMC_FREQ_RATIO - 1;
#else
    t1 = time2memc(roundup(spd_t.tRP, spd_t.tCK), false) +
         (MEMC_FREQ_RATIO == 2);
#endif
    reg |= t1;
    LLENV_SET_SPD_REG(DDR3_SPD_DRAMTMG4, reg);


    reg = 0;
    // t_cksrx
    t1 = max(5, roundup(10000, spd_t.tCK));
    t1 = time2memc(t1, true);
    reg |= t1 << 24;
    // t_cksre
    reg |= t1 << 16;
    // t_ckesr
    c_cke = max(3, roundup((spd_t.tCK < 1500) ? 5000 : 5625, spd_t.tCK));
    t1 = time2memc(c_cke + 1, true);
    reg |= t1 << 8;
    // t_cke
    t1 = time2memc(c_cke, true);
    reg |= t1;
    LLENV_SET_SPD_REG(DDR3_SPD_DRAMTMG5, reg);


    reg = 0;
    // t_xs_dll_x32 = tDDLKmin
    t1 = time2memc(512/32, true);
    reg |= (t1 & 0x7f) << 8;
    // t_xs_x32 = tXSmin
    //t1 = max(5 * spd_t.tCK, xtRFC + 10000);
    // typical tRFC is 90000-350000 ps which is always > 5 tCK
    t1 = roundup(spd_t.tRFC + 10000, 32 * spd_t.tCK);
#if !defined(CONFIG_BAIKAL_T1) || 1 // vvv???
    t1 = time2memc(t1, true);
#else
    t1 = time2memc(t1, true) + 1; //vvv: why "+1"???
#endif
    reg |= t1 & 0x7f;
    LLENV_SET_SPD_REG(DDR3_SPD_DRAMTMG8, reg);


    // t_rfc_nom_x32 = tREFI
#if !defined(CONFIG_BAIKAL_T1)
    t1 = time2memc((tREFI-spd_t.tRFC)/spd_t.tCK/32, false);
#else
    t1 = time2memc((tREFI/*-xtRFC*/)/spd_t.tCK/32, false);
#endif
    reg = t1 << 16;
    t1 = time2memc(roundup(spd_t.tRFC, spd_t.tCK), true);
    reg |= t1;
    LLENV_SET_SPD_REG(DDR3_SPD_RFSHTMG, reg);

    /* Geometry */
    unsigned byte_width;
    unsigned ecc = 0;
    unsigned cols = spd_g.cols;

#ifdef CONFIG_BAIKAL_ECC
    if (spd_g.bus_width >= 40) {
        byte_width = 5;
        ecc = 1;
    } else
#endif
    if (spd_g.bus_width >= 32) {
        byte_width = 4;
    } else
#ifdef CONFIG_BAIKAL_ECC
    if (spd_g.bus_width >= 24) { /* not tested */
        byte_width = 3;
        half_width = 1;
        ecc = 1;
        cols--; /* one column address bit is used for half-word selection */
    } else
#endif
    if (spd_g.bus_width >= 16) {
        byte_width = 2;
        half_width = 1;
        cols--;
    } else {
        return -1;
    }

    LLENV_SET_SPD_REG(DDR3_SPD_ADDRMAP2, 0);


    reg = 0x0f000000U;
    if (cols >= 10)
	    reg = 0;
    LLENV_SET_SPD_REG(DDR3_SPD_ADDRMAP3, reg);


    reg = 0x00000f0fU;
#if !defined(CONFIG_BAIKAL_T1)
    if (cols == 12) {
        reg &= ~0xfU;
    }
#else
    if (cols == 11)
	    reg &= ~0xfU;
    else if (cols == 12)
	    reg = 0;
#endif
    LLENV_SET_SPD_REG(DDR3_SPD_ADDRMAP4, reg);

#if !defined(CONFIG_BAIKAL_T1)
    unsigned row_offset = cols - 7;
#else
#define ROW_INTERNAL_BASE	6
    unsigned row_offset = cols - ROW_INTERNAL_BASE;
#endif
    reg = 0;
    for (i = 0; i < 4; ++i)
        reg |= row_offset << (i * 8);
    LLENV_SET_SPD_REG(DDR3_SPD_ADDRMAP5, reg);
    for (i = spd_g.rows - 12; i < 4; i++)
        reg |= 0x1f << (i * 8);
    LLENV_SET_SPD_REG(DDR3_SPD_ADDRMAP6, reg);

#if !defined(CONFIG_BAIKAL_T1)
    unsigned bank_offset = row_offset + spd_g.rows + 6 - 2;
#else
#define BANK_INTERNAL_BASE	2
    unsigned bank_offset = cols + spd_g.rows - BANK_INTERNAL_BASE;
#endif
    unsigned banks = 3;
    reg = 0;
    for (i = 0; i < banks; ++i)
        reg |= bank_offset << (i * 8);
    LLENV_SET_SPD_REG(DDR3_SPD_ADDRMAP1, reg);

#if !defined(CONFIG_BAIKAL_T1)
    unsigned rank_offset = bank_offset - 3 + 2;
    reg = 0;
    for (i = 0; i < spd_g.ranks; ++i)
        reg |= rank_offset << (i * 8);
#else
    #define RANK_INTERNAL_BASE	6
    unsigned rank_offset = cols + spd_g.rows + banks - RANK_INTERNAL_BASE;
    if (spd_g.ranks == 1) reg = 0x1f;
    else reg = rank_offset;
#endif
    LLENV_SET_SPD_REG(DDR3_SPD_ADDRMAP0, reg);

    LLENV_SET_SPD_REG(DDR3_SPD_SARBASE0, 0);
    LLENV_SET_SPD_REG(DDR3_SPD_SARBASE1, 0x2);

    reg = (spd_g.whole_mem / 256) + 2;
    LLENV_SET_SPD_REG(DDR3_SPD_SARBASE2, reg);
    LLENV_SET_SPD_REG(DDR3_SPD_SARSIZE0, 0);

    reg = (spd_g.whole_mem / 256 - 1 - 1);
    LLENV_SET_SPD_REG(DDR3_SPD_SARSIZE1, reg);
    LLENV_SET_SPD_REG(DDR3_SPD_SARSIZE2, 0);


    // Default:
    reg = (53334 << 12) | 4800;
    LLENV_SET_SPD_REG(DDR_PUB_SPD_PTR1, reg);

    // tDINIT0 = 500uS/tCK;
    t1 = (500 * 1000 * 1000)/spd_t.tCK;
    reg = t1 & 0xfffff;
    // tDINIT1 = max(tRFC + 10ns,  5*tCK);
    // t1 = (max(spd_t.tRFC + 10 * 1000, 5 * spd_t.tCK))/spd_t.tCK;
    t1 = roundup(spd_t.tRFC + 10 * 1000, spd_t.tCK);
    reg |= t1 << 20;
    LLENV_SET_SPD_REG(DDR_PUB_SPD_PTR3, reg);


    // tDINIT2 = 200uS
    t1 = (200*1000*1000)/spd_t.tCK;
    reg = t1 & 0x3ffff;
    // tDINIT3 = 1uS
    t1 = (1*1000*1000)/spd_t.tCK;
    reg |= t1 << 18;
    LLENV_SET_SPD_REG(DDR_PUB_SPD_PTR4, reg);


    LLENV_SET_SPD_REG(DDR_PUB_SPD_MR0, mr0);
    LLENV_SET_SPD_REG(DDR_PUB_SPD_MR1, mr1);
    LLENV_SET_SPD_REG(DDR_PUB_SPD_MR2, mr2);

    /*
    ZQCTL0  = 0x008C0040
    ECCCFG0 = 0x00000004
    INIT0   = 0x40020001
    DFIUPD0 = 0x80400003
    MSTR    = 0x03040001
    PGCR1   = 0x020046a0
    PGCR2   = 0x00f016d0
    */

    // tRTP = max (4CK or 7.5ns)
    // t1 = max(4*spd_t.tCK, 7500) / spd_t.tCK;
    t1 = roundup(spd_t.tRTP, spd_t.tCK);
    reg = 0xf & t1;
    // tRP
    t1 = roundup(spd_t.tRP, spd_t.tCK);
    reg |= (0x7f & t1) << 8;
    // tRASmin
    t1 = roundup(spd_t.tRASmin, spd_t.tCK);
    reg |= (0x7f & t1) << 16;
    // tRRD
    t1 = roundup(spd_t.tRRD, spd_t.tCK);
    reg |= (0xf & t1) << 24;
    LLENV_SET_SPD_REG(DDR_PUB_SPD_DTPR0, reg);

    //tMRD
    //t1 = 4;
    //reg = (t1 - 4) & 3;
    // why "-4"??? leave the default:
    //reg = 2;
    reg = tMRD;
    //tMOD
    reg |= (7 & (c_mod - 12)) << 8;
    //tFAWmin
    t1 = roundup(spd_t.tFAWmin, spd_t.tCK);
    reg |= (0x3f & t1) << 16;
    //tWLMRD
    t1 = tWLMRD;
    reg |= t1 << 24;
    LLENV_SET_SPD_REG(DDR_PUB_SPD_DTPR1, reg);

    //tXS
    t1 = max(512, roundup(spd_t.tRFC + 10000, spd_t.tCK));
    reg = 0x3ff & t1;
    // t_ckesr
    t1 = c_cke + 1;
    reg |= (0xf & t1) << 16;
    // tRODT = tRTW = 0;
    LLENV_SET_SPD_REG(DDR_PUB_SPD_DTPR2, reg);

    //tDQSCK -- LPDDR3 only
    reg = 1 | (1 << 8);
    // tDLLK
    t1 = 512;
    reg |= t1 << 16;
    //tCCD = BL, 0 encoding
    //tOFD = 0
    LLENV_SET_SPD_REG(DDR_PUB_SPD_DTPR3, reg);

    //tXP
    reg = 0x1f & c_xp;
    //tWLO
    t1 = roundup((spd_t.tCK < 1250) ? 9000 : 7500, spd_t.tCK);
    reg |= (0xf & t1) << 8;
    //tRFC
    t1 = roundup(spd_t.tRFC, spd_t.tCK);
    reg |= (0x3ff & t1) << 16;
    LLENV_SET_SPD_REG(DDR_PUB_SPD_DTPR4, reg);

    //tWTR
    t1 = roundup(spd_t.tWTR, spd_t.tCK);
    reg = 0x1f & t1;
    //tRCD
    t1 = roundup(spd_t.tRCD, spd_t.tCK);
    reg |= (0x3f & t1) << 8;
    //tRC
    t1 = roundup(spd_t.tRC, spd_t.tCK);
    reg |= (0xff & t1) << 16;
    LLENV_SET_SPD_REG(DDR_PUB_SPD_DTPR5, reg);

#if !defined(CONFIG_BAIKAL_T1)
    LLENV_SET_SPD_REG(DDR3_SPD_ODTCFG, 0x0600060C);
#else
    reg = (0x06000600 | ((CL - CWL) << 2));
    LLENV_SET_SPD_REG(DDR3_SPD_ODTCFG, reg);
#endif

#if !defined(CONFIG_BAIKAL_T1)
    LLENV_SET_SPD_REG(DDR3_SPD_ODTMAP, 0x00000201);
#else
    if (spd_g.ranks == 1)   reg = 0x00000001;
    else              reg = 0x00000201;
    LLENV_SET_SPD_REG(DDR3_SPD_ODTMAP, reg);
#endif

    LLENV_SET_SPD_REG(DDR3_SPD_ZQCTL0, 0x008C0040);

    if (ecc)
        reg = 0x00000004;
    else
        reg = 0;
    LLENV_SET_SPD_REG(DDR3_SPD_ECCCFG0, reg);

    //LLENV_SET_SPD_REG(DDR3_SPD_INIT0, 0x40020001);
    //pre_cke_x1024 = (500us/tCK/1024)
    reg = time2memc(500000000/spd_t.tCK/1024, true);
    reg |= 0x40020000;
    LLENV_SET_SPD_REG(DDR3_SPD_INIT0, reg);
    LLENV_SET_SPD_REG(DDR3_SPD_DFIUPD0, 0x80400003);
#if !defined(CONFIG_BAIKAL_T1)
    // ...
#else
    LLENV_SET_SPD_REG(DDR3_SPD_INIT1, 0x00010000);
#endif

#if !defined(CONFIG_BAIKAL_T1)
    LLENV_SET_SPD_REG(DDR3_SPD_MSTR, 0x01040001);  //0x03040001 - if not single rank
#else
    if (spd_g.ranks == 1)  rank_mask = 0x1;
    else             rank_mask = 0x3;
    reg = 0x40001 | (rank_mask << 24);
    reg |= half_width << 12;
    LLENV_SET_SPD_REG(DDR3_SPD_MSTR, reg);
    LLENV_SET_SPD_REG(DDR3_SPD_MRCTRL0, (rank_mask << 4));
#endif
#if !defined(CONFIG_BAIKAL_T1)
    LLENV_SET_SPD_REG(DDR_PUB_SPD_DTCR1, 0x00010237);
#else
    LLENV_SET_SPD_REG(DDR_PUB_SPD_DTCR1, (0x237 | (rank_mask << 16)));
#endif
    LLENV_SET_SPD_REG(DDR3_SPD_PCTRL_0, 0x00000001);
    LLENV_SET_SPD_REG(DDR_PUB_SPD_PGCR1, 0x020046a0);
    LLENV_SET_SPD_REG(DDR_PUB_SPD_PTR1, 0xd05612c0);

    reg = tREFI * 9 / spd_t.tCK - 400;
    reg |= (0xf << 20);
    LLENV_SET_SPD_REG(DDR_PUB_SPD_PGCR2, reg);

    LLENV_SET_SPD_REG(DDR_PUB_SPD_DTCR0, 0x800031c7);
    LLENV_SET_SPD_REG(DDR_PUB_SPD_PIR_VAL1, 0x00000073);
    LLENV_SET_SPD_REG(DDR_PUB_SPD_DXCCR, 0x20401004);
    LLENV_SET_SPD_REG(DDR_PUB_SPD_PIR_VAL2, 0x0000ff81);
    LLENV_SET_SPD_REG(DDR_BYTE_WIDTH, byte_width);
    return 0;

#endif  /* DDR_USER_REGS */
}

#ifdef TEST_MODE
#include <stdio.h>
#include <unistd.h>

int baikal_read_spd (uint32_t addr, int alen, uint8_t *buffer, int len)
{
    read(0, buffer, len);
    return 0;
}

int main()
{
    int i;

    llenv_prepare_buffer0();
    if (llenv_prepare_buffer1()) {
        fprintf(stderr, "error\n");
        return 1;
    }
    for (i = 0; i < DDR_SPD_LAST; i++)
        printf("reg(%02d) = 0x%08x\n", i, ddr_buffer1[i]);
    return 0;
}
#endif

