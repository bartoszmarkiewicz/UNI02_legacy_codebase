/*konfig.h*/
#ifndef	_KNF_H
#define	_KNF_H

#include "konfig2.tp.h"		//typy danych
#include "keypad.tp.h"		//parametry lokalne

/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
#define	_KNPAR1 7			//ogolna liczba parametrow podlegajacych modyfikacji
/*---------------------------------------------------------------------------------*/
#define _TKNFINI 5			//[s] maksymalny czas na inicjalizacje danych
#define _TKNFEXT 10			//[min] maksymalny czas aktywnosci procedury konfiguracji 
#define _TKNFSAV 1			//[s] maksymalny czas na zapis danych
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//lokalne
void RdEEParam(void);
void SetTpKtParam(void);
//bit.c
extern bit GetBit2(const unsigned char data,const unsigned char nbit);
//port.c
extern bit RdPrt(const unsigned char Index);
extern void SetPrt(const unsigned char Index, const unsigned char nw);
//print.c
extern void PrintPR(const unsigned char lkod,unsigned char nkod,unsigned char tryb);
extern void ClsScr(void);
extern void PrintWPR(unsigned char hi_wr,unsigned char lo_wr);
extern void EndPR(void);
extern void Mark(unsigned char z,unsigned char s);
//wsw.c
extern void ClrAllMarks(void);
extern void RestartKol(const unsigned char nrkol);
//rtimer.c
extern void StartRTS(const unsigned char Index);
extern unsigned char RTS(const unsigned char Index);
extern void StartRTM(const unsigned char Index);
extern unsigned char RTM(const unsigned char Index);
//keypad.c
extern void SetBufKey(StPin *ky);
extern void ResBufKey(StPin *ky);
extern unsigned char PresKey(unsigned char k,StPin *ky,unsigned char ng);//unsigned char PresKey(unsigned char k,StPin *ky);

//extern void SetBufKey1(void);
//extern void SetBufKey2(void);
//extern unsigned char PresKey1(unsigned char k);
//extern unsigned char PresKey2(unsigned char k);
//extern unsigned char ExPresKey(unsigned char k,StPin *ky,unsigned char dTim,unsigned char pTim);
//extern void ExResKey(StPin *ky);
extern unsigned char ExxPresKey(unsigned char k,StPin *ky,unsigned char dTim,unsigned char pTim);
extern void ExxResKey(StPin *ky);
extern unsigned char PresKeyEX(unsigned char k,StPin *ky);
extern void SetBufKeyEx(StPin *ky);
extern void ResBufKeyEx(StPin *ky);
extern void WaitToLowEx(StPin *ky);
//extern unsigned char ExPresKey1(unsigned char k,unsigned char dTim,unsigned char pTim);
//extern void ExResKey1(void);
//pwm.c
extern void ModParam(const unsigned char tpk,const unsigned char tpg);
extern void MocStart(unsigned char nst);
extern void MocMaksCO(unsigned char nst);
extern void MocMaksCW(unsigned char nst);
//pomiar.c
extern void SetPNST(void);
extern void PomiarPCW(void);
//konsola.c
extern void ReadParam(unsigned int *param,unsigned char *zmiana,unsigned char *zapis,unsigned int min, unsigned int max, unsigned char rckl, unsigned char fst);
/*---------------------------------------------------------------------------------*/
#endif /*_KNF_H*/
