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
//----------------------------------------------------------

void InitTabNTC(void);
extern unsigned char ReadAC(const unsigned char chanel);
extern bit RdPrt(const unsigned char Index);
extern void SetPrt(const unsigned char Index, const unsigned char nw);
//----------------------------------------------------------
#endif		/*_POMIAR_H*/
