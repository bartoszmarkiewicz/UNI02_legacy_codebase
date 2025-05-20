/*keypad.h*/

#ifndef	_KEYPAD_H
#define	_KEYPAD_H

#include "keypad.tp.h"		//typy danych

//----------------------------------------------------------
#define _EN_PK1  1			//1 - aktywna procedura PresKey
#define _EN_PK2  1			//1 - aktywna procedura PresKey
#define _EN_DPK 0			//1 - aktywna procedura DoublePresKey
#define _EN_EXPK 1			//1 - aktywna procedura ExPresKey
#define _EN_EXPK2 1			//1 - aktywna procedura ExPresKey2
#define _EN_EXPK2B 1		//1 - aktywna procedura ExPresKey2
//----------------------------------------------------------
//rtimer.c
extern void StartRTdS(const unsigned char Index);
extern unsigned char RTdS(const unsigned char Index);
//----------------------------------------------------------
#endif		/*_KEYPAD_H*/
