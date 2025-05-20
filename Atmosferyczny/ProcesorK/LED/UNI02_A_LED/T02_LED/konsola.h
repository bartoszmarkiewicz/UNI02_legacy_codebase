/*keypad.h*/

#ifndef	_KONSOLA_H
#define	_KONSOLA_H

#include "keypad.tp.h"		//parametry lokalne

#define _KDFST	100			//kod szybkiego nacisniecia przycisku

#define _TKOFF	16			//[ds] czas podtrzymania przycisku RESET do wylaczenia urzadzenia
#define _TKSET	8			//[ds] czas podtrzymania przycisku SET do zmiany trybu pracy (LATO/ZIMA)
#define _TKSET2	2			//[ds] czas podtrzymania przycisku +/- do zmiany nastawy CW w trybie podtrzymania
#define _TKSRV	10			//[ds] czas podtrzymania przycisku + do aktywacji funkcji serwisowej
#define _TKALG	10			//[ds] czas podtrzymania przycisku + do aktywacji funkcji Antylegionella

#define _TSETNST 30			//[ds] czas aktywnosci modyfikacji nastawy i parametrow SET
#define _TPCS 50			//[ds]czas podtrzymania wskazu cisnienia
#define _TFSR 10			//[min] czas aktywnosci funkcji serwisowej
#define _TALG 10			//[min] czas aktywnosci funkcji antylegionella

//#define _TWRTPCW 20			//[ds] czas po ktorym nastawa (po zmianie wart.) zostanie zapisana w pamieci EEPROM
#define _TFLPCW 5			//[ds] zegar podtrzymania znacznika modyfikacji nastawy

#define SS_OFF		0		//SW1
#define SS_SET		1		//SW2	
#define SS_KUP		2		//SW4
#define SS_KDW		3		//SW3
#define SS_SET_UP	4		//SW4
#define SS_SET_DW	5		//SW3
#define SS_OFF_SET	6		//SW3
//----------------------------------------------------------
//port.c
extern bit RdPrt(const unsigned char Index);
extern void SetPrt(const unsigned char Index, const unsigned char nw);
//readdtal.c
//extern unsigned char ReadDataXONLI(unsigned char k,unsigned int *dt,unsigned int max,const unsigned char step,const unsigned char step2);
//extern unsigned char ReadDataXONLD(unsigned char k,unsigned int *dt,unsigned int min,const unsigned char step,const unsigned char step2);
extern unsigned char ReadDataXONLD(unsigned char k,volatile StPin *ky,volatile unsigned int *dt,volatile unsigned char *ovf,unsigned int min,const unsigned char step,const unsigned char step2,const unsigned char fst);
//unsigned char ReadDataXONLI(unsigned char k,unsigned int *dt,unsigned char *ovf,unsigned int max,const unsigned char step,const unsigned char step2);
extern unsigned char ReadDataXONLI(unsigned char k,volatile StPin *ky,volatile unsigned int *dt,volatile unsigned char *ovf,unsigned int max,const unsigned char step,const unsigned char step2,const unsigned char fst);
extern unsigned char ReadDataXONLID(unsigned char k,volatile StPin *ky,volatile unsigned int *dt,volatile unsigned char *ovf, unsigned char tid, unsigned int min, unsigned int max, const unsigned char step,const unsigned char step2,const unsigned char fst);
//print.c
extern void RNst(const unsigned char nst);
//konfig.c
extern void WriteEEPCW(void);
//rtimer.c
extern void StartRTM(const unsigned char Index);
extern unsigned char RTM(const unsigned char Index);
extern void StartRTdS(const unsigned char Index);
extern unsigned char RTdS(const unsigned char Index);
//keypad.c
extern unsigned char ExxPresKey(unsigned char k,volatile StPin *ky,unsigned char dTim,unsigned char pTim);
extern void ExxResKey(volatile StPin *ky);
extern void SetBufKey(volatile StPin *ky);
extern void ResBufKey(volatile StPin *ky);
extern unsigned char PresKey(unsigned char k,volatile StPin *ky,unsigned char ng);//unsigned char PresKey(unsigned char k,StPin *ky);
extern unsigned char PresKeyEX(unsigned char k,volatile StPin *ky);
extern void ResBufKeyEx(volatile StPin *ky);
extern void WaitToLowEx(volatile StPin *ky);

extern void RestartKol2(const unsigned char nrkol);
extern void RestartKol1(const unsigned char nrkol);
extern void RestartKol(const unsigned char nrkol);

//bit.c
extern bit GetBit(const unsigned char data,const unsigned char nbit);
extern void BitSet(unsigned char *data, const unsigned char nbit,const unsigned char bt);
extern void BitSet2(unsigned char *data, const unsigned char nbit,const unsigned char bt);
//----------------------------------------------------------
#endif		/*_KONSOLA_H*/
