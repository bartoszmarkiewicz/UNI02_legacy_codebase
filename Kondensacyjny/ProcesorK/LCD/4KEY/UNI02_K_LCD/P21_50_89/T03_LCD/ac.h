/*ac.h*/
#ifndef	_AC_H
#define	_AC_H
#include "global.h"			//parametry globalne
//----------------------------------------------------------
//wartosc czasu opoznienia dla pomiaru AC
#ifndef _PLL						//modul PLL
#error Nieokreslony parametr _PLL
#endif 

#define _ENACQ	1		//1 - aktywna zwloka ACQT, 0 - nieaktywna zwloka ACQT
//----------------------------------------------------------
#if _PLL==0
#define _DLAC 14
#else 
#define _DLAC 56		//56
#endif

//----------------------------------------------------------
#endif		/*_AC_H*/
