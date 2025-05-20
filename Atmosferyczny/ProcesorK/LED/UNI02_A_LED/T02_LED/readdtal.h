/*readdta.h*/
#include "keypad.tp.h"			//parametry lokalne
#ifndef	_READDTA_H
#define	_READDTA_H

/*---------------------------------------------------------------------------------*/
#define _ENRDT	0			//zal/wyl proc ReadData(...)
#define _RDTOFS 10			//czas do przejscia w tryb szybkiej inkrementacji/dekrementacji *[0.1sek]
#define _RDKRFS 1			//krok czasowy szybkiej inkrementacji/dekrementacji *[0.1sek]
/*---------------------------------------------------------------------------------*/
#define _ENRDTX	0			//zal/wyl proc ReadDataX(...)
#define _RDTOFSX 5			//czas do przejscia w tryb szybkiej inkrementacji/dekrementacji *[0.1sek]
#define _RDKRFSX 5			//maksymalny krok czasowy szybkiej inkrementacji/dekrementacji *[0.1sek]
#define _VKRFSX 3			//krok zmiany predkosci inkrementacji/dekrementacji
/*---------------------------------------------------------------------------------*/
#define _ENRDTONL 0			//zal/wyl proc ReadDataONL(...)
#define _RDTOFSNL 10		//czas do przejscia w tryb szybkiej inkrementacji/dekrementacji *[0.1sek]
#define _RDKRFSNL 6			//krok czasowy szybkiej inkrementacji/dekrementacji *[0.1sek]
/*---------------------------------------------------------------------------------*/
#define _ENRDTOXL 0			//zal/wyl proc ReadDataXONL(...)
#define _RDTOFSXL 5			//czas do przejscia w tryb szybkiej inkrementacji/dekrementacji *[0.1sek]
#define _RDKRFSXL 5			//maksymalny krok czasowy szybkiej inkrementacji/dekrementacji *[0.1sek]
#define _VKRFSXL 3			//krok zmiany predkosci inkrementacji/dekrementacji
/*---------------------------------------------------------------------------------*/
#define _ENRDTOXLI 0		//zal/wyl proc ReadDataXONLI(...),ReadDataXONLD(...)
#define _RDTOFSXLI 5		//czas do przejscia w tryb szybkiej inkrementacji/dekrementacji *[0.1sek]
#define _RDKRFSXLI 5		//maksymalny krok czasowy szybkiej inkrementacji/dekrementacji *[0.1sek]
#define _VKRFSXLI 3			//krok zmiany predkosci inkrementacji/dekrementacji
/*---------------------------------------------------------------------------------*/
#define _ENRDTOXLD 0		//zal/wyl proc ReadDataXONLI(...),ReadDataXONLD(...)
#define _RDTOFSXLD 5		//czas do przejscia w tryb szybkiej inkrementacji/dekrementacji *[0.1sek]
#define _RDKRFSXLD 5		//maksymalny krok czasowy szybkiej inkrementacji/dekrementacji *[0.1sek]
#define _VKRFSXLD 3			//krok zmiany predkosci inkrementacji/dekrementacji
/*---------------------------------------------------------------------------------*/
#define _ENRDTOXLID 1		//zal/wyl proc ReadDataXONLI(...),ReadDataXONLD(...)
#define _RDTOFSXLID 5		//czas do przejscia w tryb szybkiej inkrementacji/dekrementacji *[0.1sek]
#define _RDKRFSXLID 5		//maksymalny krok czasowy szybkiej inkrementacji/dekrementacji *[0.1sek]
#define _VKRFSXLID 3			//krok zmiany predkosci inkrementacji/dekrementacji

//----------------------------------------------------------
//print.c
	extern void PrintCO(const unsigned char tm);
//keypad.c
	//extern unsigned char PresKey(unsigned char k);
	extern void SetBufKey(volatile StPin *ky);
	extern void ResBufKey(volatile StPin *ky);
	//extern unsigned char PresKey(unsigned char k,StPin *ky);
	extern unsigned char ExPresKey1(unsigned char k,unsigned char dTim,unsigned char pTim);
	extern unsigned char ExxPresKey1(unsigned char k,unsigned char dTim,unsigned char pTim);
	extern unsigned char ExxPresKey2(unsigned char k,unsigned char dTim,unsigned char pTim);
	extern unsigned char ExxPresKey(unsigned char k,volatile StPin *ky,unsigned char dTim,unsigned char pTim);
//rtimer.c	
	extern void StartRTS(const unsigned char Index);
	extern unsigned char RTS(const unsigned char Index);
//----------------------------------------------------------
#endif		/*_READDTA_H*/
