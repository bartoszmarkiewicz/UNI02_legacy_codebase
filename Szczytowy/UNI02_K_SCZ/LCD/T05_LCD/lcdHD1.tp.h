/*lcdHD1.tp.h*/

#ifndef	_LCDHD1_TP_H
#define	_LCDHD1_TP_H

typedef struct 	{
				unsigned UpdateLCD:1;
				unsigned ErrLCD:1;

				unsigned char dDAT0;		//mlodsze dwie cyfry
				unsigned char dDAT1;		//starsze dwie cyfry
				
				unsigned char dLED1;		//mlodsza cyfra 
				unsigned char dLED2;		//starsza cyfra
				unsigned char dLED3;		//mlodsza cyfra
				unsigned char dLED4;		//starsza cyfra
					//com1
				unsigned char	bfLCDDATA0;	
				unsigned char	bfLCDDATA1;
				unsigned char	bfLCDDATA2;
				unsigned char	bfLCDDATA3;
					//com2
				unsigned char	bfLCDDATA6;	
				unsigned char	bfLCDDATA7;
				unsigned char	bfLCDDATA8;
				unsigned char	bfLCDDATA9;
					//com3
				unsigned char	bfLCDDATA12;	
				unsigned char	bfLCDDATA13;
				unsigned char	bfLCDDATA14;
				unsigned char	bfLCDDATA15;
					//com4
				unsigned char	bfLCDDATA18;	
				unsigned char	bfLCDDATA19;
				unsigned char	bfLCDDATA20;
				unsigned char	bfLCDDATA21;	
				}tpWSWLCD;
//----------------------------------------------------------
#endif /*_LCDHD1_TP_H*/