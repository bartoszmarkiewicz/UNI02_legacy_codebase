/*wsw.h*/
/*Sposob reprezentacji danych - ITALY*/
/*---------------------------------------------------------------------------------*/
#ifndef	_WSW_H
#define	_WSW_H
/*---------------------------------------------------------------------------------*/
#define LCD4D		1					//wyswietlacz 4 cyfrowy?
/*---------------------------------------------------------------------------------*/
#include "wsw.tp.h"		//typy danych
/*---------------------------------------------------------------------------------*/
/*-----------------------------PUBLIC DATA-----------------------------------------*/
//Konfiguracja sprzetu
#define WKATODA		0					//typ wyswietlacza 0-wsp.anoda,1-wsp.katoda
#define WSWAP		0					//zamiana cyfr miejscami 0-nie zamienione, 1- zamienione
//---------
//dla int. rownoleglego
#define PORT_WSW	PORTB				//port wyswietlacza
#define MUX			RD7					//wyjscie multipleksera
//---------			
/*---------------------------------------------------------------------------------*/
//Konfiguracja algorytmu programu zwiazanego z kolejkami.
#define NPWD		5					//liczba poziomow priorytetu wyswietlania (rozmiar tablicy PWD)
#define NdPWD		6					//liczba danych dynamicznych wyst. wylacznie w kolejkach
#define NKOL		3					//liczba kolejek wyswietlania (0-brak kolejek)

#define NKL0		2					//liczba pozycji w kolejce nr 0
#define NKL1		2					//liczba pozycji w kolejce nr 1
#define NKL2		2					//liczba pozycji w kolejce nr 2
#define DKLPNC		1					//1-deaktywuj wszystkie pozycje kolejek o nizszym priorytecie i ograniczonym czasie ekspozycji kolejki
#define RSAKLP		1					//1-zatrzymanie wszystkich kolejek (o nizszym priorytecie od obecnie wyswietlanej) na pierwszej pozycji (autorestart)
#define RSNFRS		0					//1-przy ustawionej fladze "restart" aktywna jest obsluga wyswietlania kolejnych pozycji kolejki 
										//0-w czasie gdy ustawiona jest flaga "restart" wyswietlana jest tylko pierwsza pozycja kolejki 
#define RSPADR		0					//1-kolejka zawiera tak¿e dane statyczne
#define NMZNK		16					//liczba obslugiwanych znakow graficznych (0-brak znakow)
/*---------------------------------------------------------------------------------*/
//Kody led
#define LTABLE		0					//wybor tablicy znakow (zalezna od rozwiazania konstr. wyswietlacza)
//--------- 
#if LTABLE==0
#define KROPKA		0b10000000
#define KRESKA		0b01000000
#define BLACK		0b00000000
#define FULL		0b11111111
#define led_E		0b01111001
#define led_F		0b01110001
#define led_L		0b00111000
#define led_C		0b00111001
#define led_G		0b00111101
#define led_3		0b01001111
#define led_P		0b01110011
#define led_Z		0b01011011
#define led_u		0b00011100
#define led_c		0b01011000
#define led_r		0b01010000
#define led_o		0b01011100
#define led_I		0b00110000
#define led_n		0b01010100
#define led_h		0b01110100
#endif
//---------
#if LTABLE==1
#define KROPKA		0b11101111
#define KRESKA		0b10111111
#define BLACK		0b11111111
#define FULL		0b00000000
#define led_E		0b00010110
#define led_L		0b11010110
#define led_3		0b00110001
#define led_P		0b00011010
#define led_Z		0b00110010
#define led_u		0b11110100
#define led_c		0b10110110
#define led_r		0b10111110
#define led_o		0b10110100
#endif

//znacznik znakow sterowanych poziomu kolejki (priorytet wyswietlania)
#define _KZ0		0b0000000000000001	
#define _KZ1		0b0000000000000010	
#define _KZ2		0b0000000000000100	
#define _KZ3		0b0000000000001000
#define _KZ4		0b0000000000010000
#define _KZ5		0b0000000000100000	
#define _KZ6		0b0000000001000000
#define _KZ7		0b0000000010000000

#define _KZ_ALL		0b0000000011111111

//indeksy znaków sterowanych globalnie
#define _GZ0		0	//odpowiada _KZ0	
#define _GZ1		1	//odpowiada _KZ1		
#define _GZ2		2	//odpowiada _KZ2		
#define _GZ3		3	//odpowiada _KZ3	
#define _GZ4		4	//odpowiada _KZ4	
#define _GZ5		5	//odpowiada _KZ5		
#define _GZ6		6	//odpowiada _KZ6	
#define _GZ7		7	//odpowiada _KZ7
#define _GZ8		8		
#define _GZ9		9			
#define _GZ10		10			
#define _GZ11		11		
#define _GZ12		12		
#define _GZ13		13			
#define _GZ14		14		
#define _GZ15		15	

	
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
#define	tSDID		0b0000000000100000		//zapal wskazany znak graficzny
#define	tRDID		0b0000000000010000		//wygas wskazany znak graficzny
#define	tDYN		0b0000000000001000		//zaladuj dana na pozostale 2 cyfry z bufora wspolnego
#define	tONL		0b0000000000000100		//zaladuj dana na pozostale 2 cyfry z bufora wspolnego
#define	tRES		0b0000000000000000		//wyzeruj wszystkie bity
#define tALL		0b1111111111111111		//maska - ustaw/wyzeruj wszystkie bity

//--------------------------------------------------------------
//Stany definiowane globalnie dla wybranego znaku graficznego 
#define _ACTIVE		1
#define _NO_ACTIVE	0
#define _PULSE_ON	1
#define _PULSE_OFF	0
#define _SET_ON		1
#define _SET_OFF	0
//--------------------------------------------------------------------
void UpdateWSW(void);
void KolProc(void);	
void KolMark(void);
void Pulse(void);			
void SetPrWswData(void);
void FormatWswData(void);
void PrintWswData(void);
void SetWbtyBit(unsigned char nrbty,  unsigned char nrbit, const unsigned char bt);
//void RefreshWSW(void);
void WswOff(void);
void WswOn(void);
void ClrAllMarks(void);
//void SetData(WPack *wData,const unsigned char wdt1,const unsigned char wdt0);
//void SetStatus(WPack *wData,const unsigned int wst,const unsigned int mask,const unsigned char sbdid, const unsigned char rbdid);
void SetPoz(const unsigned char nrkol,const unsigned char nrpoz);
void ResetPoz(const unsigned char nrkol,const unsigned char nrpoz);
void ResetKol(const unsigned char nrkol);
void RestartKol(const unsigned char nrkol);
void BitSet(unsigned char *data, const unsigned char nbit,const unsigned char bt);
void SetMark(volatile WMark *wData,const unsigned char focus,const unsigned char pulse, const unsigned char wrt);
//----------------------------------------------------------
//--------------------------------------------------------------------
#endif		/*_MAIN_H*/
