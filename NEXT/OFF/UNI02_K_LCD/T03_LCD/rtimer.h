/*rtimer.h*/

#ifndef	_RTIMER_H
#define	_RTIMER_H

#include "rtimer.tp.h"		//typy danych
/*---------------------------------------------------------------------------------*/
//(!) Timery zarezerwowane dla nast. plikow projektu

//RTcS(0)		czcw.c
//RTcS(1)		czcw.c

//RTdS(0)		pwron.c
//RTdS(1)		keypad.c
//RTdS(2)		pomiar.c
//RTdS(3)		vpwm.c

//RTS(0)		menu.c
//RTS(1)		readdta.c, modulator.c	
/*---------------------------------------------------------------------------------*/
/*-----------------------------PUBLIC DATA-----------------------------------------*/

/*Jest to miejsce w ktorym nalezy zadeklarowac liczbe i rodzaj stosowanych timerow
  Dla wartosci rownej 0 kod danego typu timera nie bedzie generowany.*/


#define	_NRT_cS	1		/*liczba timerow typu cS*/
#define	_NRT_dS	12		/*liczba timerow typu dS*/
#define _NRT_S	8		/*liczba timerow typu S $$$*/ 
#define _NRT_M	4		/*liczba timerow typu M*/
#define _NRT_G	0		/*liczba timerow typu G*/

/*
* Start timera nalezy przeprowadzic za pomoca funkcji z grupy StartRT np. StartRTcS(n)
* gdzie n jest jedna z wartosci z zakresu 0,...,NRT_cS-1 i odpowiada numerowi wybranego
* timera.

* Kontrole stanu timera przeprowadza siê za pomoca funkcji z grupy RT np. RTcS(n)
* gdzie n jest jedna z wartosci z zakresu 0,...,NRT_cS-1 i odpowiada numerowi wybranego
* timera. Funkcja zwraca wartosc czasu ktory uplynol od momentu staru timera (wartosc
* z zakresu od 0 do 255).
* Tak wiec dla t=10cS RTcS(n)=10.
*/
/*---------------------------------------------------------------------------------*/
#endif		/*_RTIMER_H*/
