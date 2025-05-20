/*print.h*/
/*Sposob reprezentacji danych analogiczny jak w rodzinie ster. TSTR xxxx*/
#ifndef	_PRINT_H
#define	_PRINT_H
#include "wsw.h"		//typy danych
//--------------------------------------------------------------
//Kolejne poziomy w tablicy priorytetow wyswietlania (0 - najwyzszy priorytet)
#define _WS_ERR		0		//[kolejka] migajacy kod bledu
#define _WS_PRM		1		//wartosc parametru
#define _WS_NCWCO	2		//[kolejka] nastawa CO/CW
#define _WS_MSG		3		//[kolejka] procedura awaryjna
#define _WS_CS		4		//cisnienie
#define _WS_TCW		4		//temperatura CW
#define _WS_TCO		4		//temperatura CO
//Kolejne kolejki wyswietlania
#define _KL_ERR		0		//[kolejka] migajacy kod bledu
#define _KL_NCWCO	1		//[kolejka] nastawa CO/CW
#define _KL_MSG		2		//[kolejka] procedura awaryjna
//--------------------------------------------------------------
//Parametr startowy dla procedury PrintErrMsg
#define _PRN_ERR	0		//wyswietl kod error dla kolejki _WS_ERR
#define _PRN_MSG	4		//wyswietl kod message dla kolejki _WS_MSG
//--------------------------------------------------------------
//Kolejne argumenty proc PrintErr 
#define _EE1		0x01
#define _EE2		0x02
#define _EE3		0x03
#define _EE4		0x04
#define _EE5		0x05
#define _EE6		0x06
#define _EE7		0x07
#define _EE8		0x08
#define _EE9		0x09
#define _EE10		0x10
#define _EE11		0x11
#define _EE12		0x12

//Znaki sterowane z poziomu kolejki (priorytet wyswietlania)
#define _KZ_DCO		_KZ0	//kaloryfer	
#define _KZ_DCW		_KZ1	//kran
#define _KZ_SRV		_KZ2	//serwis
#define _KZ_RES		_KZ3	//reset
#define _KZ_TCO		_KZ4	//temp co
#define _KZ_TCW		_KZ5	//temp cw
#define _KZ_BAR		_KZ6	//bar
#define _KZ_PGD		_KZ7	//tryb pogodowy

//Znaki sterowane globalnie
#define _GZ_DCO		_GZ0		
#define _GZ_DCW		_GZ1
#define _GZ_SRV		_GZ2	
#define _GZ_RES		_GZ3	
#define _GZ_TCO		_GZ4
#define _GZ_TCW		_GZ5
#define _GZ_BAR		_GZ6
#define _GZ_PGD		_GZ7	
#define _GZ_PLC		_GZ8
#define _GZ_PLW		_GZ9
#define _GZ_OFF		_GZ10
#define _GZ_MXCW	_GZ11	
#define _GZ_MNCW	_GZ12
#define _GZ_MXCO	_GZ13
#define _GZ_MNCO	_GZ14
//--------------------------------------------------------------
extern void UpdateWSW(void);
extern void WswOn(void);
extern void WswOff(void);
extern void Swsdid(unsigned char dtd);
extern void Rwsdid(unsigned char dtd);
extern void SetWbtyBit(unsigned char nrbty,  unsigned char nrbit, const unsigned char bt);

extern void SetData(volatile WData *Dt,const unsigned char wdt1,const unsigned char wdt0);

//extern void SetData(WPack *wData,const unsigned char wdt1,const unsigned char wdt0);
//extern void SetData2(WPack *wData,const unsigned char wdt1,const unsigned char wdt0);
extern void SetPoz(const unsigned char nrkol,const unsigned char nrpoz);
extern void ResetPoz(const unsigned char nrkol,const unsigned char nrpoz);
extern void ResetKol(const unsigned char nrkol);
extern void RestartKol(const unsigned char nrkol);
extern void RestartKol1(const unsigned char nrkol);
extern void AllKolToFirstPoz(void);

extern void SetStatus(volatile WStatus *St,const unsigned int wst,const unsigned int mask,const unsigned char sbdid, const unsigned char rbdid);
//extern void SetStatus(WPack *wData,const unsigned int wst,const unsigned int mask,const unsigned char sbdid, const unsigned char rbdid);
//extern void SetStatus2(WPack *wData,const unsigned int wst,const unsigned int mask,const unsigned char sbdid, const unsigned char rbdid);
extern void Swbdid(WPack *wData,unsigned char dtd);
extern void Rwbdid(WPack *wData,unsigned char dtd);
extern void SetMark(volatile WMark *wData,const unsigned char focus,const unsigned char pulse, const unsigned char wrt);
extern void SetWSW(unsigned char sall);

extern void MarkMinMax(unsigned char shCW,unsigned char shCO);
extern void MarkMinMaxECO(void);
//--------------------------------------------------------------
#endif		/*_PRINT_H*/ 
