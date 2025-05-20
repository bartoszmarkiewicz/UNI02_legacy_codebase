/*keypad.h*/

#ifndef	_KEYPAD_H
#define	_KEYPAD_H

#include "keypad.tp.h"		//typy danych
#if _KEY_BBOARD==0
    //----------------------------------------------------------
    #define _EN_PK  7			//0 - aktywna procedura PresKey 4
    #define _EN_DPK 0			//1 - aktywna procedura DoublePresKey
    #define _EN_EXPK 0			//1 - aktywna procedura ExPresKey
    #define _EN_EXXPK 7			//1 - aktywna procedura ExPresKey2
#endif
#if _KEY_BBOARD==1
    //----------------------------------------------------------
    #define _EN_PK  10			//0 - aktywna procedura PresKey 4
    #define _EN_DPK 0			//1 - aktywna procedura DoublePresKey
    #define _EN_EXPK 0			//1 - aktywna procedura ExPresKey
    #define _EN_EXXPK 10			//1 - aktywna procedura ExPresKey2
#endif
    //----------------------------------------------------------
    #define _TFASTK	8			//maksymalny czas kliku dla ktorego uznany zostaje za szybki
    #define _TMEMK	10			//czas podtrzymania stanu przycisku (po jego zwolnieniu)
    //----------------------------------------------------------

//rtimer.c
extern void StartRTdS(const unsigned char Index);
extern unsigned char RTdS(const unsigned char Index);
//----------------------------------------------------------
#endif		/*_KEYPAD_H*/
