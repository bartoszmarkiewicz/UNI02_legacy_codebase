/*smgs_rs.h*/
//----------------------------------------------------------
#ifndef	_SMGS_RS_H
#define	_SMGS_RS_H

#include "smgs_rs9b.tp.h"		//typy danych
#define _AMMY3_ADDR			0x04
#define _TR_PAUSE 2			//czas na przygotowanie sie ukladu master na odbior [*1/100 sek]
#define _RS_TOUT  40		//timeout dla odbioru [*1/100 sek]
#define _TS_TOUT  40		//timeout dla transmisji [*1/100 sek]

//----------------------------------------------------------
//Dane zwiazane z komunikacja RS232 z modulem AMMY0
#define _AMMY0_ADDR	0x01	//adres modulu AMMY0
#define _NRDAMMY0 17		//liczba bajtow odebranych z mod. AMMY0
#define _NTDAMMY0 14		//liczba bajtow odpowiedzi do mod. AMMY0

//Dane zwiazane z komunikacja RS232 z modulem AMMY0
/*#define _AMMY0_ADDR	0x01	//adres modulu AMMY0
#define _NRDAMMY0 4		//liczba bajtow odebranych z mod. AMMY0
#define _NTDAMMY0 4		//liczba bajtow odpowiedzi do mod. AMMY0
*/
//----------------------------------------------------------
//----------------------------------------------------------
void TrToAMMY1(const unsigned char npow);

//rtimer.c
extern void StartRTdS(const unsigned char Index);
extern unsigned char RTdS(const unsigned char Index);
//uart.c
extern void StartUSARTTR(unsigned char nData);
extern void StartUSARTRC(unsigned char nData);
extern void StopUSARTTR(void);
extern void StopUSARTRC(void);
extern void ClrRSRBuf(void);
extern void ClrRSTBuf(void);
extern void SetRSCHK(unsigned char nData);
extern unsigned char VerifyRSCHK(const unsigned char nData);
extern unsigned char LiczRSCHK(unsigned char nData);
extern void SetRSTBuf(unsigned char indeks,unsigned char data);
extern unsigned char GetRSRBuf(unsigned char indeks);
//----------------------------------------------------------
#endif		/*_SMGS_RS_H*/
