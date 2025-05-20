/*smgs_rs.h*/
//----------------------------------------------------------
#ifndef	_SMGS_RS_H
#define	_SMGS_RS_H
#include "smgs_rs9b.tp.h"		//typy danych

//----------------------------------------------------------
#define _RS_TOUT  20		//timeout dla odbioru [*1/100 sek]
#define _TS_TOUT  20		//timeout dla transmisji [*1/100 sek]
//----------------------------------------------------------
//Identyfikatory ramki danych
#define _RD_ERR	 0
#define _RD_STD	 1
#define _RD_KNF	 2
#define _RD_LIN  3
#define _RD_STD2	 4
#define _RD_KNF2	 5

#define _LIN_GKNF   1            //zadanie wysylki paczki konfig z UNI-02 do LCD
#define _LIN_RPKNF  2            //odczytaj wartosc parametru konfiguracyjnego
#define _LIN_GSTAT  3            //satystyka zdarzen awaryjnych
#define _LIN_GEEST  4            //odczytaj status wylaczenia awaryjnego z blokada
#define _LIN_GHIST  5            //odczytaj bufor historii zdarzen
#define _LIN_RGDZ   6            //odczytaj liczbe godzin pracy

#define _LIN_WGDZ   126         //wyzeruj liczbe godzin pracy
#define _LIN_SADR   127         //zmodyfikuj adres LIN plutki UNI-02
#define _LIN_WPKNF  128         //zmien wartosc parametru konfiguracyjnego
#define _LIN_WHFLG  129         //wyzeruj licznik flagi serwisowej

//----------------------------------------------------------
//----------------------------------------------------------
#endif		/*_SMGS_RS_H*/
