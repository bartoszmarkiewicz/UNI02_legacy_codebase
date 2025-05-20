/*smgs_rs.h*/
//----------------------------------------------------------
#ifndef	_SMGS_RS_H
#define	_SMGS_RS_H

#include "smgs_rs9b.tp.h"		//typy danych

//----------------------------------------------------------
#define _RS_TOUT  20		//timeout dla odbioru [*1/100 sek]
#define _TS_TOUT  20		//timeout dla transmisji [*1/100 sek]
//----------------------------------------------------------
//Dane zwiazane z komunikacja RS232 z modulem LCD
#define _LCD_ADDR	0x01	//adres modulu LCD
//#define _NRDLCD 16		//liczba bajtow odebranych z mod. LCD
//#define _NTDLCD 16		//liczba bajtow odpowiedzi do mod. LCD
//----------------------------------------------------------
//Identyfikatory ramki danych
#define _RD_ERR	0
#define _RD_STD	1
#define _RD_KNF	2
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
extern unsigned char VerifyRSCHK(const unsigned char nData);
extern unsigned char LiczRSCHK(unsigned char nData);
extern void SetRSTBuf(unsigned char indeks,unsigned char data);
extern unsigned char GetRSRBuf(unsigned char indeks);
extern unsigned char SetNDatCHK(const unsigned char Data);
extern unsigned char VerifyNDatCHK(const unsigned char Data);
//----------------------------------------------------------
#endif		/*_SMGS_RS_H*/
