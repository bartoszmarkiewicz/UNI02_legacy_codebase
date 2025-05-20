/*konfig.h*/
#ifndef	_KNF_H
#define	_KNF_H

#include "konfig.tp.h"		//typy danych
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
#define _UNISTR 0 		//aktywacja trybu pracy - uniwersalny
#define _SRVTRB 0		//aktywacja trybu pracy z manualnym sterowaniem modulatorem 
						//wymuszenie wypelnienia pokretlem PCO
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//rodzaje mozliwych konfiguracji (etap kompilacji)
#define _INWEST 1		//miniterm
#define _MMAXP 2		//minimax
#define _ZASOB 3		//minimax zasobnikowy
#define _INWEST2 4		//maxiterm
#define _MMAXPHB 5		//minimax z hydroblokiem
#define _GRZ01 6		//grzejnik WU

/*---------------------------------------------------------------------------------*/
#define _TYPKT _GRZ01	//aktualnie wybrana konfiguracja domyslna dla kompilacji
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
#define _TKNFENT 5			//[s] czas przytrzymania przycisku do realizacji wejscia do procedury
#define _TKNFDIS 20			//[s] maksymalny czas oczekiwania na nacisniecie przycisku 
#define _TKNFEXT 10			//[min] maksymalny czas aktywnosci procedury konfiguracji 
#define _TKNFEXT2 60		//[min] maksymalny czas aktywnosci procedury konfiguracji

#define	_KNPAR1 9			//podstawowa liczba parametrow podlegajacych modyfikacji
/*---------------------------------------------------------------------------------*/
//zakres roboczy parametrow konfiguracyjnych
#define	_KMINMST 0			//MocStartowa
#define	_KMAXMST 99			
#define	_KMINMCW 0			//MocMaksymalna CW
#define	_KMAXMCW 99						
#define	_KMINRGZ 0			//rodzaj gazu
#define	_KMAXRGZ 1
#define	_KMINMNV 10			//5minimum predkosci wentylatora
#define	_KMAXMNV 40         //99
#define	_KMINMXV 10			//5  maksimum predkosci wentylatora
#define	_KMAXMXV 40         //99
#define	_KMINMSV 10			//5  predkosc startowa
#define	_KMAXMSV 40         //99
#define	_KMINLKM 0			//dlugosc komina
#define	_KMAXLKM 8
#define	_KMINPWR 0			//moc podgrzewacza
#define	_KMAXPWR 1
/*---------------------------------------------------------------------------------*/
//wartosci jakie moga przyjac poszczegolne parametry
//aktywacja funkcji „Anty-Legionella
#define _NAKTYWNA 0
#define _AKTYWNA 1

//rodzaj gazu
#define _ZIEMNY 0
#define _PLYNNY 1

//obecnosc czujnika przeplywu CO
#define _BRAK 0
#define _OBECNY 1

//typ kotla
#define _UNICO	 0			//_1FN jednofunkcyjny (unico)
#define _BITERM	 1			//bitermiczny (miniterm)
#define _MONOTERM 2			//monotermiczny (minimax)
#define _ZASOBNIK 3			//monotermiczny zasobnikowy
#define _BITERM2 4			//bitermiczny (maxiterm)

//rodzaj pompy
#define _KJEDEN 0
#define _KDWA 1

//rodzaj wymiennika
#define _TERMET 0
#define _INNY 1
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
void RdEEParam(void);
void SetTpKtParam(void);
/*---------------------------------------------------------------------------------*/
#endif /*_KNF_H*/
