/*timer3.h*/
#ifndef	_TIMER3_H
#define	_TIMER3_H
#include "global.h"			//parametry globalne
/*---------------------------------------------------------------------------------*/
/*-----------------------------PUBLIC DATA-----------------------------------------*/

#define _TIM3 10				//oczekiwany kwant czasu [mS]
#define _PRE3 4                                 //2 prescaler (1,2,4,8)
#define _POPR3 0				//wartosc poprawki

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
#if (((_EXT_OSC*_TIM3)/4)*(1000/_PRE3))+_POPR3>=(0XFFFF-0x007F)
#error _RLT3 - nieprawidlowe parametry obliczen
#endif 
#define	_RLT3 0XFFFF-(((_EXT_OSC*_TIM3)/4)*(1000/_PRE3))+_POPR3
#endif

#if _PLL!=0
#if ((_EXT_OSC*_TIM3)*(1000/_PRE3))+_POPR3>=(0XFFFF-0x007F)
#error _RLT3 - nieprawidlowe parametry obliczen
#endif
#define	_RLT3 0XFFFF-((_EXT_OSC*_TIM3)*(1000/_PRE3))+_POPR3
#endif
//----------------------------------------------------------
//----------------------------------------------------------
#endif		/*_TIMER_1*/
