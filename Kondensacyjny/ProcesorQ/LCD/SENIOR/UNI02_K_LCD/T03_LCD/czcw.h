/*czcw.h*/
//----------------------------------------------------------
#ifndef	_CZCW_H
#define	_CZCW_H

#include "czcw.tp.h"		//typy danych
#include "rtimer.tp.h"		

#define _OKCW 99			//1 sekunda
#define _KTCW 9
#define _NIMP 8				//liczba imp./sek najeden litr
#define _SETCW 17			//liczba imp/sek ustawiajaca CW
#define _CLRCW 14			//liczba imp/sek wylaczajaca CW
#define _LOPRZ _NIMP*3+_NIMP/2
#define _HIPRZ _NIMP*4
//----------------------------------------------------------
//funkcje wlasne
	void ProcCW1(void);
	void ProcCW2(void);
//rtimer.c
	extern void StartRTcS(const unsigned char Index);
//----------------------------------------------------------
#endif		/*_CZCW_H*/

					
