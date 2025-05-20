/*rgl.tp.h*/
#ifndef _RGL_TP_H_
#define _RGL_TP_H_
    volatile bit rglf_btco;
    volatile bit rglf_btcw;
    volatile bit rglf_fz;		
    volatile bit rglf_med_up;	
    volatile bit rglf_fast_up;	
    volatile bit rglf_nmod;
    volatile bit rglf_del1s;
    volatile bit rglf_deln1;
    volatile bit rglf_inv_mod;

typedef struct	sDtRgl{
    /*struct{
        unsigned btco:1;
        unsigned btcw:1;
        unsigned fz:1;		
        unsigned med_up:1;	
        unsigned fast_up:1;	
        unsigned nmod:1;
        unsigned del1s:1;
        unsigned deln1:1;
        unsigned inv_mod:1;

        unsigned int flags0; 
    };*/
volatile signed char x_t;
volatile signed char x_f;	
volatile signed char x_dt;	
volatile signed char x_df;

volatile unsigned char ytmp;
volatile unsigned char srp;
volatile unsigned char xmod;	
volatile unsigned char tmp_pwm;

}DtRgl;
#endif //_RGL_TP_H_
