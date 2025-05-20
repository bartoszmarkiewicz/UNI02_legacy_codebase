/*osc.c*/
/*Konfiguracja wewnetrznego oscylatora*/
//#include <pic18.h>
#include <xc.h>
#include "global.h"			//parametry globalne

void InitOsc(void)
{
	IRCF2=1;
	IRCF1=1;
	IRCF0=1;	//wewnetrzny oscylator ustawiony na 8MHz

#if _PLL!=0		//dla PIC18F63j90 aktywny tylko przy zew. osc.
	SCS1=0;		//default internal oscylator (zdefiniowany w CONFIG)
	SCS0=0;    
	PLLEN=1;	//zalacz petle x4
#else
	SCS1=1;     //select internal oscylator
	SCS0=1;    
	PLLEN=0;	//wylacz petle x4
#endif
}
