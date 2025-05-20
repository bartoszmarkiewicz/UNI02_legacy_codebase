/*rgl.tp.h*/
#ifndef _RGL_TP_H_
#define _RGL_TP_H_
	typedef struct	{
    union   {
            unsigned char flag0;
            struct
            {        
				unsigned btco:1;
				unsigned btcw:1;
				unsigned fz:1;		
				unsigned med_up:1;	
				unsigned fast_up:1;	
				unsigned nmod:1;
				unsigned del1s:1;
				unsigned deln1:1;
            };
        };    
        union   {
            unsigned char flag1;
            struct
            {                
				unsigned inv_mod:1;
				unsigned lo_prz:1;
                unsigned _fl1bt2:1;			//
                unsigned _fl1bt3:1;			//
                unsigned _fl1bt4:1;			//
                unsigned _fl1bt5:1;			//
                unsigned _fl1bt6:1;			//
                unsigned _fl1bt7:1;			//                
            };
        };	
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
