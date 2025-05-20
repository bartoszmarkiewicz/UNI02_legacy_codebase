/*pomiar.h*/
#ifndef	_POMIAR_H
#define	_POMIAR_H
//wersja takze dla procesorow serii K
#include "pomiar.tp.h"		//typy danych
//----------------------------------------------------------
//Czujnik NTC (rozmiar bufora na potrzeby filtracji)
#define _NMAXTAB 11 //nieparzysta!!!
#define _NMAXTAB2 51 //nieparzysta!!!
//----------------------------------------------------------
//----------------------------------------------------------
//#define COUNT 500 //500 @ 8MHz = 125uS.//250
//#define DELAY for(j=0;j<COUNT;j++)
#define _F10V_OFF       90  //0.9V - deaktywacja interfejsu
#define _F10V_ON        220 //2.2V - aktywacja interfejsu
#define _F10V_MAXPCO    950 //>=9.5V - zakres dla nastawy maksymalnej
//----------------------------------------------------------
#define _NEG_AC 1       //negacja wyniku pomiaru
#define _MAX_AC 0x0300  //wartosc maksymalna pomiaru
//----------------------------------------------------------
#define _MRG_ZRO  _MAX_AC/20    
#define _MRG_ON_ZRO (_MRG_ZRO - _MRG_ZRO/2)
#define _MRG_OFF_ZRO (_MRG_ZRO + _MRG_ZRO/2)

#define _TSTABP 5 //[sek] czas utrzymania niezmiennej wartosci do stwierdzenia stabilnosci pomiaru
//----------------------------------------------------------
#define _CHN_10V    0       
#define _CHN_FPCO   1
#define _CHN_FPCW   2
//----------------------------------------------------------
//----------------------------------------------------------

void InitTabNTC(void);
extern unsigned char ReadAC(const unsigned char chanel);
extern bit RdPrt(const unsigned char Index);
extern void SetPrt(const unsigned char Index, const unsigned char nw);
//rtimer.c
extern void StartRTS(const unsigned char Index);
extern unsigned char RTS(const unsigned char Index);
extern void StartRTdS(const unsigned char Index);
extern unsigned char RTdS(const unsigned char Index);
//----------------------------------------------------------
#endif		/*_POMIAR_H*/
