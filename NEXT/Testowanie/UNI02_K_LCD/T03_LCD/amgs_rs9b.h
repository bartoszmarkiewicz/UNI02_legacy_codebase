/*amgs_rs.h*/
//----------------------------------------------------------
#ifndef	_AMGS_RS_H
#define	_AMGS_RS_H

#include "amgs_rs9b.tp.h"		//typy danych
//----------------------------------------------------------
#define _RS_TOUT  200		//timeout dla odbioru [*1/100 sek] ^^^
#define _TS_TSTR  20		//timeout dla rozpoczecia transmisji [*1/100 sek] ^^^
#define _TS_TOUT  50		//timeout dla transmisji [*1/100 sek] ^^^
#define _NTXNOODB 100		//graniczna maksymalna liczba prob komunikacji
#define _NTONOODB 20		//liczba prob do sygnalizacji braku odbiornika
//----------------------------------------------------------
//----------------------------------------------------------

//Identyfikatory ramki danych dla komunikacji LCD <-> UNI-02
#define _RD_ERR     0   //odczyt paczki bledu
#define _RD_STD     1   //odczyt paczki standard (old)
#define _RD_KNF     2   //odczyt paczki konfig (old)
#define _RD_LIN     3   //odczyt paczki wymuszonej interfejsem LIN
#define _RD_STD2    4   //odczyt paczki standard (new)
#define _RD_KNF2    5   //odczyt paczki konfig (new)

//Dla _RD_LIN identyfikatory (IDN) ramki danych dla komunikacji LIN <->LCD <-> UNI-02
//--------ODCZYT
#define _LIN_GKNF   1       //zadanie wysylki paczki konfig z UNI-02 do LCD
#define _LIN_RPKNF  2       //odczytaj wartosc parametru konfiguracyjnego
#define _LIN_GSTAT  3       //satystyka zdarzen awaryjnych
#define _LIN_GEEST  4       //odczytaj status wylaczenia awaryjnego z blokada
#define _LIN_GHIST  5       //odczytaj bufor historii zdarzen
#define _LIN_RPP    6       //odczyt parametru P
#define _LIN_RPI    7       //odczyt parametru I
#define _LIN_RPD    8       //odczyt parametru D
#define _LIN_RPV    9       //odczytaj wartosc dolnej granicy wentylatora
#define _LIN_RRCO   10      //odczytaj wspolczynniki korekty modulacji CO przy wyborze typu 2 wentylatora (domyslnie wartosc 1)
#define _LIN_RRCW   11      //odczytaj wspolczynniki korekty modulacji CW przy wyborze typu 2 wentylatora (domyslnie wartosc 1)
#define _LIN_RGDZ   12      //odczytaj liczbe godzin pracy

//--------MODYFIKACJA
#define _LIN_WGDZ   114     //wyzeruj liczbe godzin pracy
#define _LIN_WRCO1  115     //korekta punktu rownowagi w fazie dochodzenia (FAZA1)
#define _LIN_WRCO2  116     //regulacja w punkcie rownowagi w fazie stabilizacji (FAZA2)
#define _LIN_WRCO3  117     //korekta punktu rownowagi w fazie stabilizacji (FAZA2)
#define _LIN_WRCO4  118     //reakcja superwizyjna
#define _LIN_WRCW1  119     //korekta punktu rownowagi w fazie dochodzenia (FAZA1)
#define _LIN_WRCW2  120     //regulacja w punkcie rownowagi w fazie stabilizacji (FAZA2)
#define _LIN_WRCW3  121     //korekta punktu rownowagi w fazie stabilizacji (FAZA2)
#define _LIN_WRCW4  122     //reakcja superwizyjna
#define _LIN_WPV    123     //zapisz wartosc dolnej granicy wentylatora
#define _LIN_WPP    124     //zapisz wartosc parametru P
#define _LIN_WPI    125     //zapisz wartosc parametru I
#define _LIN_WPD    126     //zapisz wartosc parametru D
#define _LIN_SADR   127     //zmien aktualny adres LIN plytki UNI-02
#define _LIN_WPKNF  128     //zmien wartosc parametru konfiguracyjnego
#define _LIN_WHFLG  129     //wyzeruj licznik flagi serwisowej
//---------------------------------------------------------


void TrToUNI(void);
//kolejka.c
	extern void ZtnToNext(unsigned char delay);
//pomiar.c
extern unsigned char PomiarTPRC(void);
//rtimer.c
extern void StartRTdS(const unsigned char Index);
extern unsigned char RTdS(const unsigned char Index);
//uart.c
extern void StartUSARTTR(void);//const unsigned char nData);
extern void StartUSARTRC(const unsigned char nData);
extern void StopUSARTTR(void);
extern void StopUSARTRC(void);
extern void ClrRSRBuf(void);
extern void ClrRSTBuf(void);
extern void SetRSCHK(const unsigned char nData);
extern unsigned char VerifyRSACK(void);
extern unsigned char VerifyRSCHK(const unsigned char nData);
extern void SetExNDatCHK(const unsigned char ndt);
extern void SetRSTBuf(const unsigned char indeks,const unsigned char data);
extern unsigned char GetRSRBuf(const unsigned char indeks);
extern unsigned char SetNDatCHK(const unsigned char Data);
extern unsigned char VerifyNDatCHK(const unsigned char Data);
extern unsigned char GetNDat(void);
extern unsigned char GetExNDat(void);
//bit.c
extern bit GetBit(const unsigned char data,const unsigned char nbit);
extern void BitSet(unsigned char *data, const unsigned char nbit,const unsigned char bt);
//----------------------------------------------------------
#endif		/*_AMGS_RS_H*/
