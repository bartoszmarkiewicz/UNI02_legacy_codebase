/*print.h*/
/*Sposob reprezentacji danych analogiczny jak w rodzinie ster. TSTR xxxx*/
#ifndef	_PRINT_H
#define	_PRINT_H
#include "wsw2.h"		//typy danych
//--------------------------------------------------------------
//Kolejne poziomy w tablicy priorytetow wyswietlania (0 - najwyzszy priorytet)
#define _WS_ERR		0		//[kolejka] migajacy kod bledu
#define _WS_PRM		1		//[kolejka] wartosc parametru
#define _WS_NCW		2		//[kolejka] nastawa CW
#define _WS_MSG		3		//[kolejka] procedura awaryjna
#define _WS_OFF		4		//wylaczenie
#define _WS_TCW		5		//temperatura CW

//--------------------------------------------------------------
//Kolejne argumenty proc PrintErr 
#define _E1		0x01
#define _E2		0x02
#define _E3		0x03
#define _E4		0x04
#define _E5		0x05
#define _E6		0x06
#define _E7		0x07
#define _E8		0x08
#define _E9		0x09
#define _E10	0x10
#define _E11	0x11
#define _E12	0x12

//--------------------------------------------------------------
//Konfiguracja wskaznika diodowego na module wyswietlacza
#define _NRDIODE	1 	//nr rejestru wyswietlacza ktory zawiera stany diod
#define _NDIODE		4	//liczba diod umieszczonych na module wyswietlacza

#define _DCO	_DL1		
#define _DCW	_DL3
#define _DPL	_DL4
#define _DKM	_DL2
//--------------------------------------------------------------
extern void WswOn(void);
extern void WswOff(void);
extern void Swsdid(unsigned char dtd);
extern void Rwsdid(unsigned char dtd);
extern void SetWbtyBit(unsigned char nrbty,  unsigned char nrbit, const unsigned char bt);
extern void SetData(WPack *wData,const unsigned char wdt1,const unsigned char wdt0);
extern void SetPoz(const unsigned char nrkol,const unsigned char nrpoz);
extern void ResetPoz(const unsigned char nrkol,const unsigned char nrpoz);
extern void ResetKol(const unsigned char nrkol);
extern void RestartKol(const unsigned char nrkol);
extern void AllKolToFirstPoz(void);
extern void SetStatus(WPack *wData,const unsigned int wst,const unsigned int mask);
extern void Swbdid(WPack *wData,unsigned char dtd);
extern void Rwbdid(WPack *wData,unsigned char dtd);
//--------------------------------------------------------------
#endif		/*_PRINT_H*/ 
