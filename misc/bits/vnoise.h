/*
 * $Id: vnoise.h 741 2012-06-29 15:22:56Z gbc $
 *
 * A deviant form of bnoise which makes vdif formed data simulating
 * N (2Gbps) channels of the the RDBE output in Astro 8Gbps mode.
 */

#ifndef vnoise_h
#define vnoise_h

#include <stdint.h>

extern int  verb;
extern void set_dura(double);
extern void set_corr(double);
extern void set_reps(double);
extern void add_tone(char *);
extern void set_gamp(double);
extern void set_bias(char *);
extern void set_lazy(long);
extern int  set_quant(char *);

extern int new_station(char *name);
extern int fake_the_data(void);
extern double (*adcq)(double);

/* similar as done in BMR/RDBE */
#define VDIF_CHAN_A 0xabaddeea
#define VDIF_CHAN_B 0xabaddeeb
#define VDIF_CHAN_C 0xabaddeec
#define VDIF_CHAN_D 0xabaddeed

typedef struct vdif_ext {
    /* word1 */
    uint32_t secs_inre:30;
    uint32_t legacy:1;
    uint32_t invalid:1;
    /* word2 */
    uint32_t df_num_insec:24;
    uint32_t ref_epoch:6;
    uint32_t UA:2;
    /* word3 */
    uint32_t df_len_octets:24;
    uint32_t num_chan_log2:5 ;
    uint32_t ver:3;
    /* word4 */
    uint32_t stationID:16 ;
    uint32_t threadID:10 ;
    uint32_t bpsm1:5 ;
    uint32_t dt:1 ;
    /* word5 */
    uint32_t seql:24;
    uint32_t edv5:8;
    /* word6 */
    uint32_t seqh;
    uint32_t psn;
    uint32_t eud8;
} VdifExt;

#endif /* vnoise_h */

/*
 * eof
 */
