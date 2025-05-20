/*tacho.h*/
#ifndef	_TACHO_H
#define	_TACHO_H
#include "timer3.h"

#define _NMAXTAB 20
#define _NFLTAB 12
/*---------------------------------------------------------------------------------*/
#define _NDTPOMT 11	//5(nieparzysta!) liczba danych w bezposredniej tablicy pomiarow
#define _NDTPOMT2 11	//3(nieparzysta!) liczba danych w wyjsciowej tablicy pomiarow
#define _NOKROBR 2		//liczba okresów na obrot
#define _NCKL1MS _PDZ3	//liczba cykli przypadajaca na 1msek (= 1msek/(odcinek czasu co ktory nast. przerwanie zegara timer3)
//----------------------------------------------------------
//----------------------------------------------------------
void AddTabNp(unsigned int *tbb,unsigned int d);
unsigned int CalcSrNp(unsigned int *tbb);
unsigned char VVent(unsigned int sso);
void ClrMinMax(unsigned int *tbb);
void FPmVNT(void);
void ClrDtV(void);
	extern void StartRTdS(const unsigned char Index);
	extern unsigned char RTdS(const unsigned char Index);
	extern void StartRTcS(const unsigned char Index);
	extern unsigned char RTcS(const unsigned char Index);
#endif		/*_TACHO_H*/
