/*port.h*/
#ifndef	_PORT_H
#define	_PORT_H
#include "port.tp.h"		//typy danych

/*---------------------------------------------------------------------------------*/
/*-----------------------------PUBLIC DATA-----------------------------------------*/
#define _TKRPR	5					//[m sek] krok czasowy odswierzania stanu portow
/*konfiguracja portow fizycznych procesora*/

#define	_DIRECTA	0b11111111		//orientacja wyprowadzen portu
#define	_DIRECTB	0b11100111		//0b11110111
#define	_DIRECTC	0b11111001		
#define	_DIRECTD	0b00110000		
#define	_DIRECTE	0b11111000		

#define	_VALUEA		0b00000000		//stan poczatkowy wyprowadzen portu
#define	_VALUEB		0b00000000
#define	_VALUEC		0b00000000
#define	_VALUED		0b00000000
#define	_VALUEE		0b00000000

/* Dalsza konfiguracje modulu nalezy przeprowadzic w pliku port.c*/

/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
#endif	/*_PORT_H*/
