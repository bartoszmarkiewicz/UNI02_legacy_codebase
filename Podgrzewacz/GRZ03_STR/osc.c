/*osc.c*/
/*Konfiguracja wewnetrznego oscylatora*/
#include <pic18.h>
#include "global.h"			//parametry globalne
#if defined(_18F46K20) 
	void InitOsc(void)
	{
		IRCF2=1;	//wewnetrzny oscylator ustawiony na 8MHz (K20)
		IRCF1=1;
		IRCF0=0;	
		SCS1=0;		//Primary clock (determined by CONFIG1H[FOSC<3:0>]).
		SCS0=0;
	/*	//USTAWIANY W MAIN.H JAKO RCIO
		FOSC3=1;	//Internal oscillator block, port function on RA6 and RA7	
		FOSC2=0;
		FOSC1=0;
		FOSC0=0;
	*/
	#if _PLL!=0
		PLLEN=1;	//zalacz petle PLLx4
	#else
		PLLEN=0;	//wylacz petle PLLx4
	#endif
	}
#else
	void InitOsc(void)
	{
		IRCF0=1;	//wewnetrzny oscylator ustawiony na 8MHz
		IRCF1=1;
		IRCF2=1;
	#if _PLL!=0
		PLLEN=1;	//zalacz petle x4
	#else
		PLLEN=0;	//wylacz petle x4
	#endif
	}
#endif
