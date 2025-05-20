/*uart9b.h*/
//----------------------------------------------------------
#ifndef	_UART9b_H
#define	_UART9b_H

#include "uart9b.tp.h"		//typy danych
/*---------------------------------------------------------------------------------*/
#define _NRS_RBF 35			//rozmiar bufora odebranych danych
#define _NRS_TBF 35			//rozmiar bufora wysylanych danych
/*---------------------------------------------------------------------------------*/
/*-----------------------------PUBLIC DATA-----------------------------------------*/
#define _BRGH 0				//1(w trybie synch. ignorowany) 0 - low speed, 1- high speed 				
#define _SYNC 0				//0 - asynch. 1 - synch.
#define _DSBD 2400			//19200predkosc transmisji

#define _RCHK_ON	0		//aktywna suma kontrolna dla odczytu
#define _RACK_ON	1		//aktywna procedura potwierdzenia odbioru
/*---------------------------------------------------------------------------------*/
/*-----------------------------PROTECTED DATA--------------------------------------*/
//Obliczenie wartosci poczatkowej Timera nr 1
#ifndef _EXT_OSC				//czestotliwosc oscylatora [MHz]
#error Nieokreslony parametr _EXT_OSC
#endif
#ifndef _PLL					//modul PLL (0-nieaktywny,1-aktywny)
#error Nieokreslony parametr _PLL
#endif

#if _PLL!=0
#if (_BRGH==0)&&(_SYNC==0)					//low speed?
#define _SPBRG ((_EXT_OSC*4*1000000/_DSBD)/64)-1
#define _CBR (_EXT_OSC*4*1000000)/(64*(_SPBRG+1))
#endif
#if (_BRGH!=0)&&(_SYNC==0)					//high speed?
#define _SPBRG ((_EXT_OSC*4*1000000/_DSBD)/16)-1
#define _CBR (_EXT_OSC*4*1000000)/(16*(_SPBRG+1))
#endif
#if _SYNC!=0					//praca synchroniczna?
#define _SPBRG ((_EXT_OSC*4*1000000/_DSBD)/4)-1
#define _CBR (_EXT_OSC*4*1000000)/(4*(_SPBRG+1))
#endif
#endif

#if _PLL==0
#if (_BRGH==0)&&(_SYNC==0)					//low speed?
#define _SPBRG ((_EXT_OSC*1000000/_DSBD)/64)-1
#define _CBR (_EXT_OSC*1000000)/(64*(_SPBRG+1))
#endif
#if (_BRGH!=0)&&(_SYNC==0)					//high speed?
#define _SPBRG ((_EXT_OSC*1000000/_DSBD)/16)-1
#define _CBR (_EXT_OSC*1000000)/(16*(_SPBRG+1))
#endif
#if _SYNC!=0					//praca synchroniczna?
#define _SPBRG ((_EXT_OSC*1000000/_DSBD)/4)-1
#define _CBR (_EXT_OSC*1000000)/(4*(_SPBRG+1))
#endif
#endif

#if (_SPBRG>0xFF)
#error _SPBRG - nieprawidlowe parametry obliczen
#endif
#define _EBR=_CBR-_DSBD			//wartosc bledu obliczen


//----------------------------------------------------------
#endif		/*_UART9b_H*/
