/*konfig.h*/
#ifndef	_KNF_H
#define	_KNF_H

#include "konfig.tp.h"		//typy danych
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

#define	_KNPAR1 7			//podstawowa liczba parametrow podlegajacych modyfikacji (razem z En)
/*---------------------------------------------------------------------------------*/
//zakres roboczy parametrow konfiguracyjnych
#define	_KMINMST 0			//MocStartowa
#define	_KMAXMST 99			
#define	_KMINMCW 0			//MocMaksymalna CW
#define	_KMAXMCW 99			
#define	_KMINVMD 10			//dolna granica predkosci obrotowej
#define	_KMAXVMD 20
#define	_KMINVMG 25			//gorna granica predkosci obrotowej
#define	_KMAXVMG 95		
#define	_KMINNOK 1			//liczba okresow na sek.
#define	_KMAXNOK 4
#define _KMINRFAN 0			//rodzaj wentylatora
#define _KMAXRFAN 2
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

#define _VVNT16			16			//wstepna dolna predkosci obr. dla czasowego dolnego progu modulacji przy grzaniu CO
#define _VVNT35			35			//wstepna dolna predkosci obr. dla czasowego dolnego progu modulacji przy grzaniu CO
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
extern void PrintWPR(const unsigned char wr);
extern void EndPR(void);
extern void PrintOFF(void);
extern void EndOFF(void);
//wsw.c
extern void RestartKol(const unsigned char nrkol);
//rtimer.c
extern void StartRTS(const unsigned char Index);
extern unsigned char RTS(const unsigned char Index);
extern void StartRTM(const unsigned char Index);
extern unsigned char RTM(const unsigned char Index);
//keypad.c
extern void SetBufKey(void);
extern void SetBufKey1(void);
extern void SetBufKey2(void);
extern unsigned char PresKey1(unsigned char k);
extern unsigned char PresKey2(unsigned char k);
extern unsigned char ReadDataONL(unsigned char k,unsigned char *dt,unsigned char min,unsigned char max,const unsigned char step,unsigned char wrn);
extern unsigned char ReadDataXONL(unsigned char k,unsigned char *dt,unsigned char min,unsigned char max,const unsigned char step,const unsigned char step2,unsigned char wrn);
//pwm.c
extern void ModParam(void);
extern void MocStart(unsigned char nst);
extern void MocMaksCO(unsigned char nst);
extern void MocMaksCW(unsigned char nst);
//pomiar.c
extern void SetPNST(void);
extern void PomiarPCW(void);
/*---------------------------------------------------------------------------------*/
#endif /*_KNF_H*/
