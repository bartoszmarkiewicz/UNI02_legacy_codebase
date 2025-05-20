/*port.h*/
#ifndef	_PORT_H
#define	_PORT_H
#include "port.tp.h"		//typy danych
/*---------------------------------------------------------------------------------*/
/*-----------------------------PUBLIC DATA-----------------------------------------*/
#define _TKRPR	5					//[m sek] krok czasowy odswierzania stanu portow
/*konfiguracja portow fizycznych procesora*/
#if _EXT_BOARD==0
#define	_DIRECTA	0b00001111		//orientacja wyprowadzen portu
#define	_DIRECTB	0b00000001
#define	_DIRECTC	0b10011000
#define	_DIRECTD	0b11111100
#define	_DIRECTE	0b01111111
#define	_DIRECTF	0b11111101
#define	_DIRECTG	0b11100101

#define	_VALUEA		0b00000000		//stan poczatkowy wyprowadzen portu
#define	_VALUEB		0b00000000
#define	_VALUEC		0b00000000
#define	_VALUED		0b00000000
#define	_VALUEE		0b00000000
#define	_VALUEF		0b00000000
#define	_VALUEG		0b00000000
#else

#if _KEY_BBOARD==0
    #if _TEST_OT==1
    #define	_DIRECTA	0b00101011 //przycisk SET jako wyjscie sygnalu synchronizacji pomiaru
    #else
    #define	_DIRECTA	0b00101111 //orientacja wyprowadzen portu
    #endif
    #define	_DIRECTB	0b00000011
    #define	_DIRECTC	0b10011000      
    #define	_DIRECTD	0b11111100
    #define	_DIRECTE	0b01111111
    #define	_DIRECTF	0b11111101
    #define	_DIRECTG	0b11100101

    #define	_VALUEA		0b00000000		//stan poczatkowy wyprowadzen portu
    #define	_VALUEB		0b00000000
    #define	_VALUEC		0b00000000
    #define	_VALUED		0b00000000
    #define	_VALUEE		0b00000000
    #define	_VALUEF		0b00000000
    #define	_VALUEG		0b00000000
#endif
#if _KEY_BBOARD==1
    #if _TEST_OT==1
    #define	_DIRECTA	0b00111011 //przycisk SET jako wyjscie sygnalu synchronizacji pomiaru
    #else
    #define	_DIRECTA	0b00111111 //orientacja wyprowadzen portu
    #endif
    #define	_DIRECTB	0b00100011
    #define	_DIRECTC	0b10011000      
    #define	_DIRECTD	0b11111100
    #define	_DIRECTE	0b01111111
    #define	_DIRECTF	0b11111101
    #define	_DIRECTG	0b11101101

    #define	_VALUEA		0b00000000		//stan poczatkowy wyprowadzen portu
    #define	_VALUEB		0b00000000
    #define	_VALUEC		0b00000000
    #define	_VALUED		0b00000000
    #define	_VALUEE		0b00000000
    #define	_VALUEF		0b00000000
    #define	_VALUEG		0b00000000
#endif
#endif
/* Dalsza konfiguracje modulu nalezy przeprowadzic w pliku port.c*/

/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
#endif	/*_PORT_H*/
