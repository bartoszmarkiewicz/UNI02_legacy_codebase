/*ac.h*/
//dla procesorów serii K
#ifndef	_AC_H
#define	_AC_H
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
#define _DLAC 56
#endif
//----------------------------------------------------------
//Tablica czujnika NTC
//Pierwsze 48 bajtow pamieci EEPROM jest zarezerwowane
//dla potrzeb programu.
//----------
#define _NTC_0	0x34			//wartosc AC dla 99 C - dolny margines tabeli
							//wartosc AC <= _NTC_0 zwraca 0 C
#define _NTC_99	0xED			//wartosc AC dla 0 C - gorny margines tabeli
							//wartosc AC >= _NTC_99 zwraca 99 C
//----------						
#define _AEETCO	0x0000		//adres poczatku tablicy czujnika NTC
#define _NDTTCO	192			//liczba danych tablicy czujnika NTC
//----------
#define _ZNTC_0	0x29		//wartosc AC dla 99 C - dolny margines tabeli
							//wartosc AC <= _NTC_0 zwraca 0 C
#define _ZNTC_99 0xEC		//wartosc AC dla 0 C - gorny margines tabeli
							//wartosc AC >= _NTC_99 zwraca 99 C
//----------						
#define _AEETZW	0x00E0		//adres poczatku tablicy czujnika NTC
#define _NDTTZW	200			//liczba danych tablicy czujnika NTC

//----------------------------------------------------------
#endif		/*_AC_H*/
