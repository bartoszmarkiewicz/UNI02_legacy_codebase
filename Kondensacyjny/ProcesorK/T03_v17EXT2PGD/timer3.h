/*timer3.h*/
//Konfiguracja na porzeby precyzyjnego pomiaru predkosci obrotowej
#ifndef	_TIMER3_H
#define	_TIMER3_H

/*---------------------------------------------------------------------------------*/
/*-----------------------------PUBLIC DATA-----------------------------------------*/

#define _TIM3 1					//1oczekiwany kwant czasu [mS]
#define _PRE3 8					//2prescaler (1,2,4,8)
#define _PDZ3 4					//5 dzielnik interwalu czasu _TIM3
#define _POPR3 -90				//51 wartosc poprawki
//Uzyskany kwant czasu = _TIM3/_PDZ3
/*---------------------------------------------------------------------------------*/
/*-----------------------------PROTECTED DATA--------------------------------------*/

//Obliczenie wartosci poczatkowej Timera nr 3
#ifndef _EXT_OSC				//czestotliwosc oscylatora [MHz]
#error Nieokreslony parametr _EXT_OSC
#endif
#ifndef _PLL					//modul PLL (0-nieaktywny,1-aktywny)
#error Nieokreslony parametr _PLL
#endif

#if _PLL==0
#if ((((_EXT_OSC*_TIM3)/4)*(1000/_PRE3))/_PDZ3)+_POPR3>=(0XFFFF-0x007F)
#error _RLT3 - nieprawidlowe parametry obliczen
#endif 
#define	_RLT3 0XFFFF-((((_EXT_OSC*_TIM3)/4)*(1000/_PRE3))/_PDZ3)+_POPR3
#endif

#if _PLL!=0
#if (((_EXT_OSC*_TIM3)*(1000/_PRE3))/_PDZ3)+_POPR3>=(0XFFFF-0x007F)
#error _RLT3 - nieprawidlowe parametry obliczen
#endif
#define	_RLT3 0XFFFF-(((_EXT_OSC*_TIM3)*(1000/_PRE3))/_PDZ3)+_POPR3
#endif

#define _RLT3H _RLT3>>8
#define _RLT3L _RLT3&0x00FF
//----------------------------------------------------------
//----------------------------------------------------------
#endif		/*_TIMER_3*/
