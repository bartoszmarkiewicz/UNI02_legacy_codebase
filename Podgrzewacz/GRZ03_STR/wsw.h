/*wsw.h*/
/*Sposob reprezentacji danych - ITALY*/
/*---------------------------------------------------------------------------------*/
#ifndef	_WSW_H
#define	_WSW_H
#include "wsw.tp.h"		//typy danych
/*---------------------------------------------------------------------------------*/
/*-----------------------------PUBLIC DATA-----------------------------------------*/
//Konfiguracja sprzetu
#define SER_WSW		0					//typ interfejsu 0- rownolegly, 1- szeregowy
#define WKATODA		0					//typ wyswietlacza 0-wsp.anoda,1-wsp.katoda
#define WSWAP		1					//zamiana cyfr miejscami 0-nie zamienione, 1- zamienione
//---------
//dla int. rownoleglego
#define PORT_WSW	PORTD				//port wyswietlacza
#define MUX			RC5					//wyjscie multipleksera
//---------			
//dla int. szeregowego
#define SMUX		4					//nr bitu multipleksera w rejestrze		
#define NWBTY 		2					//liczba przesylanych bajtow
#define NRBIT 		8					//liczba bitow na rejestr
#define RTIMW 		10					//podstawowy polokres przebiegu szeregowego(krotnosc petli opoznajacej)
/*---------------------------------------------------------------------------------*/
//Konfiguracja algorytmu programu zwiazanego z kolejkami.
#define NPWD		6					//liczba poziomow priorytetu wyswietlania (rozmiar tablicy PWD)
#define NdPWD		7					//liczba danych dynamicznych wyst. wylacznie w kolejkach
#define NKOL		4					//liczba kolejek wyswietlania (0-brak kolejek)

#define NKL0		2					//liczba pozycji w kolejce nr 0
#define NKL1		2					//liczba pozycji w kolejce nr 1
#define NKL2		2					//liczba pozycji w kolejce nr 2
#define NKL3		2					//liczba pozycji w kolejce nr 2
#define DKLPNC		1					//1-deaktywuj wszystkie pozycje kolejek o nizszym priorytecie i ograniczonym czasie ekspozycji kolejki
#define RSAKLP		1					//1-zatrzymanie wszystkich kolejek (o nizszym priorytecie od obecnie wyswietlanej) na pierwszej pozycji (autorestart)
#define RSNFRS		0					//1-przy ustawionej fladze "restart" aktywna jest obsluga wyswietlania kolejnych pozycji kolejki 
										//0-w czasie gdy ustawiona jest flaga "restart" wyswietlana jest tylko pierwsza pozycja kolejki 
/*---------------------------------------------------------------------------------*/
/*
#define KROPKA		0b01111111	
#define KRESKA		0b10111111	
#define BLACK		0b11111111
#define FULL		0b00000000
#define led_E		0b10000110	
#define led_L		0b11000111	
#define led_3		0b10110000	
#define led_P		0b10001100	
#define led_Z		0b10100100	
#define led_u		0b11100011	
#define led_c		0b10100111	
#define led_r		0b10101111	
#define led_o		0b10100011	
#define led_n		0b10101011
*/

#define KROPKA		0b11111110
#define KRESKA		0b11111101
#define BLACK		0b11111111
#define FULL		0b00000000
#define led_E		0b01100001
#define led_L		0b11100011
#define led_3		0b00001101
#define led_P		0b00110001
#define led_Z		0b00100101
#define led_u		0b11000111
#define led_c		0b11100101
#define led_r		0b11110101
#define led_o		0b11000101
#define led_n		0b11010101
//Konfiguracja wskaznika diodowego na module wyswietlacza
#define _NRDIODE	1 			//nr rejestru wyswietlacza ktory zawiera stany diod
#define _NDIODE		4			//liczba diod umieszczonych na module wyswietlacza
#define _DL1		0b00000001	//adres - górna lewa dioda
#define _DL2		0b00000100	//adres - gorna prawa dioda
#define _DL3		0b00001000	//adres - dolna lewa dioda
#define _DL4		0b00000010	//adres - dolna prawa dioda
/*---------------------------------------------------------------------------------*/
/*-----------------------------PROTECTED DATA--------------------------------------*/
#if NPWD==0								
#define NPWD=1					//minimum 1 poziom priorytetu wyswietlania
#endif
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//Pozycje okreslone w parametrze wst i mask procedury SetStatus.
//Pod uwage brane sa tylko pozycje okreslone w parametrze mask,
//z tych ustawiane sa pozycje okreslone w parametrze wst
//pozostale sa zerowane.
#define	tLED		0b0000000000000000		//wyswietl dane typu LED
#define	tFOCUS		0b1000000000000000		//aktywacja wyswietlania
#define	tDEC		0b0100000000000000		//wyswietl dane typu DEC
#define	tHEX		0b0010000000000000		//wyswietl dane typu HEX
#define	tAUTO		0b0001000000000000		//autowygaszenie pierwszej cyfry
#define	tKROPKA0	0b0000100000000000		//zapal kropke mlodszej cyfry
#define	tKROPKA1	0b0000010000000000		//zapal kropke starszej cyfry
#define	tWYGAS0		0b0000001000000000		//wygas mlodsza cyfre
#define	tWYGAS1		0b0000000100000000		//wygas starsza cyfre
#define tWYGAS		0b0000001100000000		//wygas obie cyfry
#define	tLED0		0b0000000010000000		//wyswietl kod led na mlodszej cyfsze
#define	tLED1		0b0000000001000000		//wyswietl kod led na starszej cyfsze
#define	tSDID		0b0000000000100000		//zapal wskazana diode (tND1:tND0)
#define	tRDID		0b0000000000010000		//wygas wskazana diode (tND1:tND0)
#define	tRES		0b0000000000000000		//wyzeruj wszystkie bity
#define tALL		0b1111111111111111		//maska - ustaw/wyzeruj wszystkie bity

//--------------------------------------------------------------------
void KolProc(void);	
void Pulse(void);			
void SetPrWswData(void);
void FormatWswData(void);
void PrintWswData(void);
void SetWbtyBit(unsigned char nrbty,  unsigned char nrbit, const unsigned char bt);
//void RefreshWSW(void);
void WswOff(void);
void WswOn(void);
void SetData(WPack *wData,const unsigned char wdt1,const unsigned char wdt0);
void SetStatus(WPack *wData,const unsigned int wst,const unsigned int mask);
void SetPoz(const unsigned char nrkol,const unsigned char nrpoz);
void ResetPoz(const unsigned char nrkol,const unsigned char nrpoz);
void ResetKol(const unsigned char nrkol);
void RestartKol(const unsigned char nrkol);
void BitSet(unsigned char *data, const unsigned char nbit,const unsigned char bt);
//----------------------------------------------------------
//--------------------------------------------------------------------
#endif		/*_MAIN_H*/
