/*lled.h*/
#ifndef	_LLED_H
#define	_LLED_H
//#include "lled.tp.h"		//typy danych
/*---------------------------------------------------------------------------------*/
#define _TTLOW 15			//[s] czas do automatycznego wygaszenia wyswietlacza
#define _LLHIGH 100			//maksymalne rozswietlenie podswietlenia
#define _LLLOW 15			//15 minimalne rozswietlenie podswietlenia
#define _LLLDW 1			//krok redukcji podswietlenia
#define _LLLUP 2			//krok redukcji podswietlenia

#define _FLLLDW 5
#define _FLLLUP 5			//krok redukcji podswietlenia
/*---------------------------------------------------------------------------------*/
	extern void StartRTS(const unsigned char Index);
	extern unsigned char RTS(const unsigned char Index);
	extern void StartRTcS(const unsigned char Index);
	extern unsigned char RTcS(const unsigned char Index);
	extern void WritePWM2(const unsigned char tpwm);
	extern bit RdPrt(const unsigned char Index);
        extern void StartRTdS(const unsigned char Index);
	extern unsigned char RTdS(const unsigned char Index);
/*---------------------------------------------------------------------------------*/
#endif	/*_LLED_H*/