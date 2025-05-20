/*konfig.h*/
#ifndef	_KNF_H
#define	_KNF_H

#include "pomiar.h"		//typy danych
#include "konfig.tp.h"		//typy danych
#include "history.h"
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
#define	_KNPAR1 21			//ogolna liczba parametrow podlegajacych modyfikacji
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
#define _UNISTR 1 		//aktywacja trybu pracy - uniwersalny
#define _SRVTRB 0		//aktywacja trybu pracy z manualnym sterowaniem modulatorem 
						//wymuszenie wypelnienia pokretlem PCO
#define _SRVANL 0		//aktywacja trybu kontrolnego funkcji ANTYLEGIONELLA (krotszy czas do aktywacji)
#define _SRV48H 0		//aktywacja trybu kontrolnego funkcji 48h (krotszy czas do aktywacji)

#if _SRVANL!=0 
	#define _SRV48H 0
#endif
#define _NHSPZ _HISTSZ       //liczba pozycji historii zdarzen awaryjnych
#define _NSTBL _ESTBL        //liczba zapamietywanych statusow wylaczenia z blokada awaryjna
#define _NSTPZ _ESTPZ        //liczba pozycji statusu wylaczenia awaryjnego z blokada
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//rodzaje mozliwych konfiguracji (etap kompilacji)
#define _UNCO 0			//unico
#define _INWEST 1		//miniterm
#define _INWEST2 2		//maxiterm
#define _MMAXPHB 3		//minimax z hydroblokiem

//#define _UNCO 0			//unico
//#define _INWEST 1		//miniterm
//#define _MMAXP 2		//minimax
//#define _ZASOB 3		//minimax zasobnikowy
//#define _INWEST2 4		//maxiterm
//#define _MMAXPHB 5		//minimax z hydroblokiem

/*---------------------------------------------------------------------------------*/
#define _TYPKT _MMAXPHB	//aktualnie wybrana konfiguracja domyslna dla kompilacji
//#define _TYPKT _INWEST	//aktualnie wybrana konfiguracja domyslna dla kompilacji
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
#define _TKNFENT 5			//[s] czas przytrzymania przycisku do realizacji wejscia do procedury
#define _TKNFDIS 20			//[s] maksymalny czas oczekiwania na nacisniecie przycisku 
#define _TKNFEXT 10			//[min] maksymalny czas aktywnosci procedury konfiguracji 
#define _TKNFEXT2 60		//[min] maksymalny czas aktywnosci procedury konfiguracji
/*---------------------------------------------------------------------------------*/
//zakres roboczy parametrow konfiguracyjnych
#define	_KMINMST 0			//MocStartowa
#define	_KMAXMST 99			
#define	_KMINMCW 0			//MocMaksymalna CW
#define	_KMAXMCW 99			
#define	_KMINMCO 0			//MocMaksymalna CO
#define	_KMAXMCO 99	
#define	_KMINRGZ 0			//rodzaj gazu
#define	_KMAXRGZ 1
#define	_KMINEAL 0			//aktywacja AL
#define	_KMAXEAL 1
#define	_KMINWTK 0			//typ kotla
#define	_KMAXWTK 3
#define	_KMINOBG 0			//rodzaj obiegu
#define	_KMAXOBG 1
#define	_KMINOGP 0			//ogrzewanie podlogowe
#define	_KMAXOGP 1
#define	_KMINTPC 0			//czujnik cisnienia
#define	_KMAXTPC 1

#define	_KMINRDP 0			//rodzaj pompy
#define	_KMAXRDP 1

//#define	_KMINECP 0			//czujnik przeplywu
//#define	_KMAXECP 1						
//#define	_KMINWRP 0			//rodzaj pompy
//#define	_KMAXWRP 1			
//#define	_KMINWTW 0			//rodzaj wymiennika
//#define	_KMAXWTW 1	

#define	_KMINDLP 5			//delta temperaturowa CO dla pompy
#define	_KMAXDLP 25

#define	_KMINMNP 15			//minimalna granica predkosci pompy
#define	_KMAXMNP 99	

#define	_KMINMXP 25			//maksymalna granica predkosci pompy
#define	_KMAXMXP 99

#define	_KMINMXP2 25		//maksymalna granica predkosci pompy dla CW
#define	_KMAXMXP2 99

#define	_KMINHSCO 0			//histereza wylaczenia grzania CO (ponad nastawe)
#define	_KMAXHSCO 15

#define	_KMINCONW 70		//temp CO przy grzaniu CW zasobnika
#define	_KMAXCONW 89

#define _KMINPROP 0			//poprawka dla regulatora pogodowego
#define _KMAXPROP 20

#if _MIN_PCO>_MIN_PDCO
#define _KMINPGMX _MIN_PDCO         //maksymalna wartosc graniczna CO dla regulatora pogodowego
#else
#define _KMINPGMX _MIN_PCO          //maksymalna wartosc graniczna CO dla regulatora pogodowego
#endif
#define _KMAXPGMX _MAX_PCO

#define _KMINPGDR 0			//tryb pracy regulatora pogodowego
#define _KMAXPGDR 2

#define _KMINCWMG 0			//histereza wylaczenia CW(4F)
#define _KMAXCWMG 1

#define _KMINCWTM 0			//timer zasobnika (1F)
#define _KMAXCWTM 1
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

//typ kotla (oznaczenie dla potrzeb algorytmu)
#define _UNICO	 0			//jednofunkcyjny (unico)
#define _BITERM	 1			//bitermiczny (miniterm)
#define _BITERM2 2			//bitermiczny (maxiterm)
#define _MONOHB  3			//monotermiczny (minimax) z hydroblokiem

//#define _UNICO	 0			//jednofunkcyjny (unico)
//#define _BITERM	 1			//bitermiczny (miniterm)
//#define _MONOTERM 2			//monotermiczny (minimax)
//#define _ZASOBNIK 3			//monotermiczny zasobnikowy
//#define _BITERM2 4			//bitermiczny (maxiterm)
//#define _MONOHB 5			//monotermiczny (minimax) z hydroblokiem

//rodzaj pompy
#define _KJEDEN 0
#define _KDWA 1

//rodzaj wymiennika
#define _TERMET 0
#define _INNY 1

//rodzaj obiegu
#define _OTWARTY 0
#define _ZAMKNIETY 1
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//lokalne
unsigned char SaveKnfPar(void);
void RdEEParam(void);
void SetTpKtParam(void);
void SetDefaultValue(void);
void SetPFN(void);
unsigned char RefKnfPar(void);
//port.c
extern bit RdPrt(const unsigned char Index);
extern void SetPrt(const unsigned char Index, const unsigned char nw);
//print.c
extern void PrintPR(const unsigned char lkod,unsigned char nkod,unsigned char tryb);
extern void PrintWPR(const unsigned char wr);
extern void EndPR(void);
//wsw.c
extern void RestartKol(const unsigned char nrkol);
//rtimer.c
extern void StartRTS(const unsigned char Index);
extern unsigned char RTS(const unsigned char Index);
extern void StartRTM(const unsigned char Index);
extern unsigned char RTM(const unsigned char Index);
extern void StartRTG(const unsigned char Index);
//keypad.c
extern void SetBufKey(void);
extern unsigned char PresKey(unsigned char k);
extern unsigned char ReadDataONL(unsigned char k,unsigned char *dt,unsigned char min,unsigned char max,const unsigned char step,unsigned char wrn);
extern unsigned char ReadDataXONL(unsigned char k,unsigned char *dt,unsigned char min,unsigned char max,const unsigned char step,const unsigned char step2,unsigned char wrn);
//pwm.c
extern void ModParam(const unsigned char tpk,const unsigned char tpg);
extern void MocStart(unsigned char nst);
extern void MocMaksCO(unsigned char nst);
extern void MocMaksCW(unsigned char nst);
//pomiar.c
extern void PomiarPFN(void);
extern void PomiarZW(void);
//bit.c
extern void BitSet2(unsigned char *data, const unsigned char nbit,const unsigned char bt);
/*---------------------------------------------------------------------------------*/
#endif /*_KNF_H*/
