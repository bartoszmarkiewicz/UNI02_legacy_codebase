/*rgl.tp.h*/
#ifndef _RGL_TP_H_
#define _RGL_TP_H_
	typedef struct	{
				unsigned btco:1;
				unsigned btcw:1;
				unsigned fz:1;		
				unsigned med_up:1;	
				unsigned fast_up:1;	
				unsigned nmod:1;
				unsigned del1s:1;
				unsigned deln1:1;
				unsigned inv_mod:1;
				unsigned slow_inc:1;
				
				signed char x_t;
				signed char x_f;	
				signed char x_dt;	
				signed char x_df;

				unsigned char ytmp;
				unsigned char srp;
				unsigned char xmod;	
				unsigned char tmp_pwm;

				}DtRgl;
#endif //_RGL_TP_H_
