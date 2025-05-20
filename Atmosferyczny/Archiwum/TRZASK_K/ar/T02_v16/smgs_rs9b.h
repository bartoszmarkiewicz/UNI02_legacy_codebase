/*smgs_rs.h*/
//----------------------------------------------------------
#ifndef	_SMGS_RS_H
#define	_SMGS_RS_H
#include "global.h"			//parametry globalne
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

#define _LIN_GKNF 1             //zadanie wysylki paczki konfig z UNI-02 do LCD
#define _LIN_RPKNF 2            //odczytaj wartosc parametru konfiguracyjnego
#define _LIN_GSTAT 3            //satystyka zdarzen awaryjnych
#define _LIN_GEEST 4            //odczytaj status wylaczenia awaryjnego z blokada
#define _LIN_GHIST 5            //odczytaj bufor historii zdarzen

#define _LIN_SADR 127           //zmodyfikuj adres LIN plutki UNI-02
#define _LIN_WPKNF  128       //zmien wartosc parametru konfiguracyjnego
#define _LIN_WHFLG  129         //wyzeruj licznik flagi serwisowej
//----------------------------------------------------------
void TrToAMMY1(const unsigned char npow);

//rtimer.c
extern void StartRTdS(const unsigned char Index);
extern unsigned char RTdS(const unsigned char Index);
//uart.c
extern void StartUSARTTR(void);
extern void StartUSARTRC(void);
extern void StopUSARTTR(void);
extern void StopUSARTRC(void);
extern void ClrRSRBuf(void);
extern void ClrRSTBuf(void);
extern void SetRSCHK(unsigned char nData);
extern unsigned char VerifyRSCHK(unsigned char nData);
extern void SetExNDatCHK(const unsigned char ndt);
extern unsigned char LiczRSCHK(const unsigned char *bf,const unsigned char nData);
extern void SetRSTBuf(const unsigned char indeks,const unsigned char data);
extern unsigned char GetRSRBuf(const unsigned char indeks);
extern unsigned char GetRSTBuf(const unsigned char indeks);
extern unsigned char SetNDatCHK(const unsigned char Data);
//extern unsigned char SetNDat2(const unsigned char Data);
extern unsigned char VerifyNDatCHK(const unsigned char Data);
extern unsigned char GetNDat(void);
extern unsigned char GetExNDat(void);

extern void WriteEEALIN(unsigned char d);
extern unsigned char ReadEEALIN(void);
//extern void ModParam();
extern void SetTpKtParam(void);
void ClearEETA(void);
extern void RdEEParam(void);
extern unsigned char ReadBtyNZSTAT(const unsigned char ind);
extern unsigned char ReadBtySTAT(const unsigned char nr, const unsigned char ind);

extern void ClrHFLG(void);
extern unsigned char ReadBtyHNPZ(void);
extern unsigned char ReadBtyHFLG(void);
extern unsigned char ReadBtyHREF(const unsigned char ind);
extern unsigned char ReadBtyEEHIST(const unsigned char ind);
//----------------------------------------------------------
#endif		/*_SMGS_RS_H*/
