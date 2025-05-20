/*timer1.h*/
#ifndef	_TIMER1_H
#define	_TIMER1_H
#include "global.h"			//parametry globalne
/*---------------------------------------------------------------------------------*/
/*-----------------------------PUBLIC DATA-----------------------------------------*/

#define _TIM 10				//oczekiwany kwant czasu [mS]
#define _PRE 2				//prescaler (1,2,4,8)
#define _POPR 0				//wartosc poprawki

/*---------------------------------------------------------------------------------*/
/*-----------------------------PROTECTED DATA--------------------------------------*/

//Obliczenie wartosci poczatkowej Timera nr 1
#ifndef _EXT_OSC				//czestotliwosc oscylatora [MHz]
#error Nieokreslony parametr _EXT_OSC
#endif
#ifndef _PLL					//modul PLL (0-nieaktywny,1-aktywny)
#error Nieokreslony parametr _PLL
#endif

#if _PLL==0
#if (((_EXT_OSC*_TIM)/4)*(1000/_PRE))+_POPR>=(0XFFFF-0x007F)
#error _RLT1 - nieprawidlowe parametry obliczen
#endif 
#define	_RLT1 0XFFFF-(((_EXT_OSC*_TIM)/4)*(1000/_PRE))+_POPR
#endif

#if _PLL!=0
#if ((_EXT_OSC*_TIM)*(1000/_PRE))+_POPR>=(0XFFFF-0x007F)
#error _RLT1 - nieprawidlowe parametry obliczen
#endif
#define	_RLT1 0XFFFF-((_EXT_OSC*_TIM)*(1000/_PRE))+_POPR
#endif

//----------------------------------------------------------
//----------------------------------------------------------
#endif		/*_TIMER_1*/
