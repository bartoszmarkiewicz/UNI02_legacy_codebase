/*tacho.h*/
#ifndef	_TACHO_H
#define	_TACHO_H
//#include "timer3.h"

/*
 FOSC/4; PRES 4; PERIOD:129,5uS; _PDZ3 10
 FOSC/4; PRES 1; PERIOD:17uS; _PDZ3 50
 */

#define _PDZ3 20				 //10 dzielnik interwalu czasu _TIM3
#define _POPR3 -90				//-90wartosc poprawki

#define _NMAXTAB 20
#define _NFLTAB 12
/*---------------------------------------------------------------------------------*/
#define _NDTPOMT 11	//5(nieparzysta!) liczba danych w bezposredniej tablicy pomiarow
#define _NDTPOMT2 11	//3(nieparzysta!) liczba danych w wyjsciowej tablicy pomiarow
#define _NOKROBR 2		//liczba okres�w na obrot
#define _NCKL1MS _PDZ3	//liczba cykli przypadajaca na 1msek (= 1msek/(odcinek czasu co ktory nast. przerwanie zegara timer3)
//----------------------------------------------------------
//----------------------------------------------------------
void FPmVNT(void);
#endif		/*_TACHO_H*/
