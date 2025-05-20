/*konfig.h*/
#ifndef	_KNF_H
#define	_KNF_H

#include "konfig3.tp.h"		//typy danych
#include "keypad.tp.h"		//parametry lokalne

#define	_KODKT_1F 0
#define _KODKT_4F 3

//#define _KODKT_2F 2
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
#define	_KNPAR  21			//domyslna ogolna liczba parametrow podlegajacych modyfikacji dla ver 5
#define	_KNPAR1 9			//wsteczna ogolna liczba parametrow podlegajacych modyfikacji dla ver 1
#define	_KNPAR2 13          //wsteczna ogolna liczba parametrow podlegajacych modyfikacji dla ver 2
#define	_KNPAR3 16          //wsteczna ogolna liczba parametrow podlegajacych modyfikacji dla ver 3
#define	_KNPAR4 18          //wsteczna ogolna liczba parametrow podlegajacych modyfikacji dla ver 3
#define	_KNPAR5 21          //wsteczna ogolna liczba parametrow podlegajacych modyfikacji dla ver 3
/*---------------------------------------------------------------------------------*/
#define _TKNFINI 5			//[s] maksymalny czas na inicjalizacje danych
#define _TKNFEXT 10			//[min] maksymalny czas aktywnosci procedury konfiguracji 
#define _TKNFSAV 1			//[s] maksymalny czas na zapis danych
/*---------------------------------------------------------------------------------*/
#define _MIN_PDCO 35		//dolna granica nastawy CO dla ogrzewania podlogowego
#define _MAX_PDCO 55		//dolna granica nastawy CO dla ogrzewania podlogowego
#define _MIN_PCO 40			//gorna granica nastawy	CO
#define _MAX_PCO 85			//gorna granica nastawy	CO
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
extern void SetBufKey(volatile StPin *ky);
extern void ResBufKey(volatile StPin *ky);
extern unsigned char PresKey(unsigned char k,volatile StPin *ky,unsigned char ng);//unsigned char PresKey(unsigned char k,volatile StPin *ky);

//extern void SetBufKey1(void);
//extern void SetBufKey2(void);
//extern unsigned char PresKey1(unsigned char k);
//extern unsigned char PresKey2(unsigned char k);
//extern unsigned char ExPresKey(unsigned char k,volatile StPin *ky,unsigned char dTim,unsigned char pTim);
//extern void ExResKey(volatile StPin *ky);
extern unsigned char ExxPresKey(unsigned char k,volatile StPin *ky,unsigned char dTim,unsigned char pTim);
extern void ExxResKey(volatile StPin *ky);
extern unsigned char PresKeyEX(unsigned char k,volatile StPin *ky);
extern void SetBufKeyEx(volatile StPin *ky);
extern void ResBufKeyEx(volatile StPin *ky);
extern void WaitToLowEx(volatile StPin *ky);
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
extern void ReadParamCW(unsigned int *param,unsigned char *zmiana,unsigned char *zapis,unsigned int min, unsigned int max, unsigned char rckl, unsigned char fst);
extern void ReadParamCO(unsigned int *param,unsigned char *zmiana,unsigned char *zapis,unsigned int min, unsigned int max, unsigned char rckl, unsigned char fst);
/*---------------------------------------------------------------------------------*/
#endif /*_KNF_H*/
