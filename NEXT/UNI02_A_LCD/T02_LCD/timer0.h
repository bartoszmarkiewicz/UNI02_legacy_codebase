/*timer0.h*/
#ifndef	_TIMER0_H
#define	_TIMER0_H
#include "global.h"			//parametry globalne
/*---------------------------------------------------------------------------------*/
/*-----------------------------PUBLIC DATA-----------------------------------------*/

/*---------------------------------------------------------------------------------*/
/*-----------------------------PROTECTED DATA--------------------------------------*/

//Obliczenie wartosci poczatkowej Timera nr 1
#ifndef _EXT_OSC				//czestotliwosc oscylatora [MHz]
#error Nieokreslony parametr _EXT_OSC
#endif
#ifndef _PLL					//modul PLL (0-nieaktywny,1-aktywny)
#error Nieokreslony parametr _PLL
#endif

#if (_PLL==0)&&(_EXT_OSC==8)
#define _PRE0 8				//prescaler dla kroku 4u (4u/(1/(_EXT_OSC*1000000)))
#endif
#if (_PLL==1)&&(_EXT_OSC==8)
#define _PRE0 32            //32 prescaler dla kroku 4u (4u/(1/(_EXT_OSC*1000000)))  
//#define _PRE0 8
#endif


/*
 * Timer0 w trybie 16bit - dla kroku co 4u przepelnienie nastepuje co 268ms
 * (65535*4u=268m) 
 * 
 * Chcemy zeby timer zmienial stan co 4us:
 * 
 * Dla PLL=0:
 * 1/8000000=125n
 * Jeden krok timera to 4 kroki zegara:
 * 125n*4=500n  -krok timera
 * Preskaler=8
 * 500n*8=4u
 * 
 * Dla PLL=1:
 * 1/(8000000*4)=31,25n
 * Jeden krok timera to 4 kroki zegara:
 * 31,25n*4=125n  -krok timera
 * Preskaler=32
 * 125n*32=4u 
 * 
 
 */
 
//----------------------------------------------------------
//----------------------------------------------------------
#endif		/*_TIMER_0*/
