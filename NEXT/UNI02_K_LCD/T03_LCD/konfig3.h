/*konfig.h*/
#ifndef	_KNF_H
#define	_KNF_H

#include "konfig3.tp.h"		//typy danych
#include "keypad.tp.h"		//parametry lokalne

#define	_KODKT_1F 1
#define _KODKT_2F 2
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
#define	_KNPAR  34			//domyslna ogolna liczba parametrow podlegajacych modyfikacji dla ver 5
#define	_KNPAR1 15			//wsteczna ogolna liczba parametrow podlegajacych modyfikacji dla ver 1
#define	_KNPAR2 17          //wsteczna ogolna liczba parametrow podlegajacych modyfikacji dla ver 2
#define	_KNPAR3 18          //wsteczna ogolna liczba parametrow podlegajacych modyfikacji dla ver 3
#define	_KNPAR4 19          //wsteczna ogolna liczba parametrow podlegajacych modyfikacji dla ver 4
#define	_KNPAR5 22          //wsteczna ogolna liczba parametrow podlegajacych modyfikacji dla ver 5
#define	_KNPAR6 23          //wsteczna ogolna liczba parametrow podlegajacych modyfikacji dla ver 6
#define	_KNPAR7 28          //wsteczna ogolna liczba parametrow podlegajacych modyfikacji dla ver 7
#define	_KNPAR8 29          //wsteczna ogolna liczba parametrow podlegajacych modyfikacji dla ver 8
#define	_KNPAR9 31          //wsteczna ogolna liczba parametrow podlegajacych modyfikacji dla ver 9
#define	_KNPAR10 32         //wsteczna ogolna liczba parametrow podlegajacych modyfikacji dla ver 10
#define	_KNPAR11 34         //wsteczna ogolna liczba parametrow podlegajacych modyfikacji dla ver 10
/*---------------------------------------------------------------------------------*/
#define _TKNFINI 5			//[s] maksymalny czas na inicjalizacje danych
#define _TKNFEXT 60			//10[min] maksymalny czas aktywnosci procedury konfiguracji 
#define _TKNFSAV 1			//[s] maksymalny czas na zapis danych
/*---------------------------------------------------------------------------------*/
#define _MIN_PDCO 25		//dolna granica nastawy CO dla ogrzewania podlogowego
#define _MAX_PDCO 55		//dolna granica nastawy CO dla ogrzewania podlogowego
#define _MIN_PCO 40			//gorna granica nastawy	CO
#define _MAX_PCO 80			//gorna granica nastawy	CO
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
extern void ClrDig(const unsigned char dhigh, const unsigned char dlow, const unsigned char krs);
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

//amgs_rs9b.c
extern void ClrNKFN(void);
extern unsigned char GetNKFN(void);
/*---------------------------------------------------------------------------------*/
#endif /*_KNF_H*/
