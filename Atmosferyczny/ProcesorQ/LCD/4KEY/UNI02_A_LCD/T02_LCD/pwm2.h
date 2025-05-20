/*pwm2.h*/
//modulator SITA
#ifndef	_PWM2_H
#define	_PWM2_H
#include "global.h"			//parametry globalne
#include "pwm2.tp.h"

//okreslenie typu odbiornika 
#define _FPWM			4			//czestotliwosc przebiegu [kHz]
#define _NEG_PWM2		0			//PWM2 odwrotma logika wyjscia? (max dla pwm=0, min dla pwm=255)
//***********************************************************************************
//***********************************************************************************
//***********************************************************************************
//***********************************************************************************
//***********************************************************************************
//***********************************************************************************
//***********************************************************************************
//***********************************************************************************
//Obliczenie wartosci PR2 dla wybranej czestotliwosci przebiegu PWM

#ifndef _PLL					//modul PLL (0-nieaktywny,1-aktywny)
#error Nieokreslony parametr _PLL
#endif

//Dla _PLL=0 PR2 obliczamy ze wzoru [1/FPWM/(4*PRE2*1/FOSC)]-1
//Dla _PLL=1 PR2 obliczamy ze wzoru [1/FPWM/(PRE2*1/FOSC)]-1
//Gdzie:
//FOSC - czestotliwosc oscylatora
//FPWM - oczekiwana czestotliwosc przebiegu pwm
//PRE2 - wartosc preskalera dla timer2

#if (_PLL==0)&&((_FPWM==16)||(_FPWM==4)||(_FPWM==1))
#if _FPWM==16
#define _WT2CON	0b00000000		//timer2 preskaler = 1
#define _WPR2	0x7C			//wartosc PR2 dla FPWM=16kHz,FOSC=8MHz,PRE2=1;
#endif
#if _FPWM==4
#define _WT2CON	0b00000001		//timer2 preskaler = 4
#define _WPR2	0x7C			//wartosc PR2 dla FPWM=4kHz,FOSC=8MHz,PRE2=4;
#endif
#if _FPWM==1
#define _WT2CON	0b00000010		//timer2 preskaler = 16
#define _WPR2	0x7C			//wartosc PR2 dla FPWM=1kHz,FOSC=8MHz,PRE2=1;
#endif
#else
#if (_PLL==0)
#error Nieprawidlowy parametr _FPWM 
#endif 
#endif
//----------
#if (_PLL!=0)&&((_FPWM==16)||(_FPWM==4))
#if _FPWM==16
#define _WT2CON	0b00000001		//timer2 preskaler = 4
#define _WPR2	0x7C			//wartosc PR2 dla FPWM=16kHz,FOSC=8MHz,PRE2=4;
#endif
#if _FPWM==4
#define _WT2CON	0b00000010		//timer2 preskaler = 16
#define _WPR2	0x7C			//wartosc PR2 dla FPWM=4kHz,FOSC=8MHz,PRE2=16;
#endif
#else
#if (_PLL!=0)
#error Nieprawidlowy parametr _FPWM
#endif
#endif
//----------------------------------------------------------
//Maksymalna rozdzielczosc PWM obliczamy ze wzoru
//Max PWM resolution=log(FOSC/FPWM)/log(2) [bits]
//Dla FOSC=8MHz i FPWM=16kHz wynosi ona 9 bitow.


/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
/*-----------------------------PROTECTED DATA--------------------------------------*/

//Maksymalna dozwolona wartosc wypelnienia zalezy od _WPR2
//Nalezy ja obliczyc biorac pod uwage ze wypelnienie jest wartoscia 10 bitowa

#if(_WPR2<0x40)
#define _PSHIFT	2
#define _HI_PWM			(_WPR2+1)<<2	//gorna bezwgledna granica wypelnienia
#endif
#if(_WPR2>0x40 && _WPR2<0x80)
#define _PSHIFT	1
#define _HI_PWM			(_WPR2+1)<<1	//gorna bezwgledna granica wypelnienia
#endif
#if(_WPR2>0x80)
#define _PSHIFT	0
#define _HI_PWM			_WPR2+1			//gorna bezwgledna granica wypelnienia
#endif

//kontrola i formatowanie podanych parametrow
#if (_MAG_MOD1>_HI_PWM)
#define _MAG_MOD1		_HI_PWM
#endif
#if (_MAG_MOD2>_HI_PWM)
#define _MAG_MOD2		_HI_PWM
#endif 
//----------------------------------------------------------
void SetOsc(void);
//----------------------------------------------------------
#endif		/*_PWM_H*/
