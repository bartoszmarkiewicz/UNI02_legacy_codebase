/*konfig.h*/
#ifndef	_KNF_H
#define	_KNF_H

#include "konfig.tp.h"		//typy danych
#include "keypad.tp.h"		//parametry lokalne

/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
#define _UNISTR 0 		//aktywacja trybu pracy - uniwersalny
#define _SRVTRB 0		//aktywacja trybu pracy z manualnym sterowaniem modulatorem 
						//wymuszenie wypelnienia pokretlem PCO
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//rodzaje mozliwych konfiguracji (etap kompilacji)
#define _INWEST 1		//miniterm
#define _MMAXP 2		//minimax
#define _ZASOB 3		//minimax zasobnikowy
#define _INWEST2 4		//maxiterm
#define _MMAXPHB 5		//minimax z hydroblokiem
#define _GRZ01 6		//grzejnik WU

/*---------------------------------------------------------------------------------*/
#define _TYPKT _GRZ01	//aktualnie wybrana konfiguracja domyslna dla kompilacji
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
#define _TKNFENT 5			//[s] czas przytrzymania przycisku do realizacji wejscia do procedury
#define _TKNFDIS 20			//[s] maksymalny czas oczekiwania na nacisniecie przycisku 
#define _TKNFEXT 10			//[min] maksymalny czas aktywnosci procedury konfiguracji 
#define _TKNFEXT2 60		//[min] maksymalny czas aktywnosci procedury konfiguracji

#define	_KNPAR1 10			//podstawowa liczba parametrow podlegajacych modyfikacji
/*---------------------------------------------------------------------------------*/
//zakres roboczy parametrow konfiguracyjnych
#define	_KMNST 0			//MocStartowa
#define	_KMXST 99			
#define	_KMNMAXCW 0			//maksimum mocy CW
#define	_KMXMAXCW 8000
#define	_KMNMAXCO 0			//maksimum mocy CO
#define	_KMXMAXCO 8000	
#define	_K2MNMAXCW 0		//MocMaksymalna CW
#define	_K2MXMAXCW 99
#define	_K2MNMAXCO 0		//MocMaksymalna CO
#define	_K2MXMAXCO 99				
#define	_KMINRGZ 0			//rodzaj gazu
#define	_KMAXRGZ 1
#define	_KMINANL 0			//tryb antylegionella
#define	_KMAXANL 1
#define	_KMINWTK 0			//typ kotla
#define	_KMAXWTK 4
#define	_KMINCPG 0			//czujnik przeplywu WG
#define	_KMAXCPG 1	
#define	_KMNMINCW 0			//minimum mocy CW
#define	_KMXMINCW 8000
#define	_KMNMINCO 0			//minimum mocy CO
#define	_KMXMINCO 8000
#define	_KMNPDG 0			//ogrzewanie podlogowe
#define	_KMXPDG 1		
			
/*---------------------------------------------------------------------------------*/
//wartosci jakie moga przyjac poszczegolne parametry
//aktywacja funkcji „Anty-Legionella
#define _NAKTYWNA 0
#define _AKTYWNA 1

//rodzaj gazu
#define _ZIEMNY 0
#define _PLYNNY 1

//obecnosc czujnika przeplywu CO
#define _BRAK 0
#define _OBECNY 1

//typ kotla
#define _UNICO	 0			//_1FN jednofunkcyjny (unico)
#define _BITERM	 1			//bitermiczny (miniterm)
#define _MONOTERM 2			//monotermiczny (minimax)
#define _ZASOBNIK 3			//monotermiczny zasobnikowy
#define _BITERM2 4			//bitermiczny (maxiterm)

//rodzaj pompy
#define _KJEDEN 0
#define _KDWA 1

//rodzaj wymiennika
#define _TERMET 0
#define _INNY 1
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//lokalne
void RdEEParam(void);
void SetTpKtParam(void);
//port.c
extern bit RdPrt(const unsigned char Index);
extern void SetPrt(const unsigned char Index, const unsigned char nw);
//print.c
extern void PrintPR(const unsigned char lkod,unsigned char nkod,unsigned char tryb);
extern void ClsScr(void);
extern void PrintWPR(unsigned char hi_wr,unsigned char lo_wr);
extern void EndPR(void);
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
