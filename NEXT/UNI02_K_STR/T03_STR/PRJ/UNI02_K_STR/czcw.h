/*czcw.h*/
//----------------------------------------------------------
#ifndef	_CZCW_H
#define	_CZCW_H

#include "czcw.tp.h"		//typy danych	
//Imp = 9,3808*Ltr - 1,4553
#define _OKCW 99			//1 sekunda

//#define _NIMP (int)(9,3808*1 - 1,4553)				//liczba imp./sek najeden litr
//#define _LOPRZ ((int)(9,3808*3 - 1,4553)+_NIMP/2) 
//#define _HIPRZ (int)(9,3808*4 - 1,4553)

//progi zalaczenia/wylaczenia pobudzenia CW
#define _LTR 1
#define _ON_LTR 3
#define _OFF_LTR 2.5

//parametry charakterystyczne dla przetwornika przeplywu wody uzytkowej w hydrobloku Pakkens
#define _ONE1 (unsigned int)(9.3808*_LTR - 1.4553)			//liczba imp/sek ustawiajaca CW
#define _ON_CW1 (unsigned int)(9.3808*_ON_LTR - 1.4553)			//liczba imp/sek ustawiajaca CW
#define _OFF_CW1 (unsigned int)(9.3808*_OFF_LTR - 1.4553)			//liczba imp/sek wylaczajaca CW

//#define _NIMP 8				//liczba imp./sek najeden litr
//#define _SETCW 17			//liczba imp/sek ustawiajaca CW
//#define _CLRCW 14			//liczba imp/sek wylaczajaca CW
//#define _LOPRZ _NIMP*3+_NIMP/2
//#define _HIPRZ _NIMP*4
//----------------------------------------------------------
//----------------------------------------------------------
#endif		/*_CZCW_H*/

					
