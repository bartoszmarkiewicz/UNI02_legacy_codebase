/*mgs_rs.h*/
//----------------------------------------------------------
#ifndef	_MGS_RS_H
#define	_MGS_RS_H

#include "mgs_rs9b.tp.h"		//typy danych
//----------------------------------------------------------
#define _RS_TOUT  20		//timeout dla odbioru [*1/100 sek]
#define _TS_TOUT  20		//timeout dla transmisji [*1/100 sek]
#define _NTXNOODB 100		//graniczna maksymalna liczba prob komunikacji
#define _NTONOODB 20		//liczba prob do sygnalizacji braku odbiornika
//----------------------------------------------------------
//----------------------------------------------------------
//Identyfikatory ramki danych dla komunikacji LCD <-> UNI-02
#define _RD_ERR	0   //odczyt paczki bledu
#define _RD_STD	1   //odczyt paczki standard
#define _RD_KNF	2   //odczyt paczki konfig
#define _RD_STD2	 4
#define _RD_KNF2	 5
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
#endif		/*_MGS_RS_H*/
