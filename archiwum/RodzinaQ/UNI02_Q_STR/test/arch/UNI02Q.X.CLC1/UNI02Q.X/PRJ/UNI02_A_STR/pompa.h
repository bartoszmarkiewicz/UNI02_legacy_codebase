/*pompa.h*/
#ifndef	_POMPA_H
#define	_POMPA_H
//----------------------------------------------------------
#include "pompa.tp.h"		//typy danych
//----------------------------------------------------------
#define _TPMPBLK 10			//[s] czas pblokady dla modyfikacji wyp. po rozruchu
#define _TPMPMOD 50			//[*0.1 s] krok czasowy modyfikacji wypelnienia
#define _PMPKDW 2			//krok ujemnej modyfikacji wypelnienia 
#define _PMPKUP 1			//krok ujemnej modyfikacji wypelnienia 

//Parametry trybu ECO
#define _MINT	10			//maksymalna graniczna wartosc temperatury
#define _MAXT	80

#define _DFEKO	5			//wartosc srodkowa wspolczynnika EKO
#define _ECO5LO 25			//dlona granica temp IN dla Eco=_DFEKO-1
#define _ECO5HI 55			//gorna granica temp IN dla Eco=_DFEKO-1
#define _ECODDL 20			//wartosc delty dla Eco=_DFEKO
#define _PDEKO	4			//dzielnik _ECODDL

#define _MKOND	57				//[C] granica kondensacji
#define _MCOLD	30				//[C] granica zimnego powrotu
#define _PCOLD	(_MKOND-_MCOLD)	

//----------------------------------------------------------
//----------------------------------------------------------
//----------------------------------------------------------
#endif		/*_POMPA_H*/