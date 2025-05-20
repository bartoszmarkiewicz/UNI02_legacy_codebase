/*imz.h*/
#ifndef	_IMZ_H
#define	_IMZ_H
#include "imz.tp.h"					//typy danych
/*---------------------------------------------------------------------------------*/
/*---------------------------NADAJNIK DANYCH DO UKLADU SAA-------------------------*/
#define _NBBUF	23					//rozmiar bufora I2C
#define _NCHDAT	22					//liczba bajtow weryfikacji CHK
/*---------------------------------------------------------------------------------*/
#define SYGTRN  0					//zalacz/wylacz sygnalizacje transferu danych
#define SYGRED	0					//poziom sygnalu zapalenia diody
#define SYGGRN	0					//poziom sygnalu zapalenia diody
#define LRED	RD2					//dioda czerwona
#define LGRN	RD1					//dioda zielona
/*---------------------------------------------------------------------------------*/
#define _IMZPULSE		90			//okres pulsacji kontrolki/wyswietlacza
/*---------------------------------------------------------------------------------*/
//segmenty wysterowywane bez zmiany stanu wyswietlacza
#define _KROPKA 7					//nr bitu kropki
/*---------------------------------------------------------------------------------*/
//predefiniowane kody wyswietlane
#define _KRESKA 0b01000000			//kod kreski	
#define _BLANK  0b00000000			//wygaszenie wyswietlacza
/*---------------------------------------------------------------------------------*/
//kolejne bity slowa STATUS dla wyswietlaczy
#define _ADDI	 	0b00000001		//zwiekszenie jasnosci wyswietlania
#define _KRS0	 	0b00000010		//kreski na mlodszych 2 cyfrach
#define _KRS1	 	0b00000100		//kreski na starszych 2 cyfrach
#define _FLS0	 	0b00001000		//wyswietl pulsujaco pierwsze 2 cyfry
#define _FLS1	 	0b00010000		//wyswietl pulsujaco drugie 2 cyfry
#define _CLS0	 	0b00100000		//wygaœ mlodsze 2 cyfry	
#define _CLS1	 	0b01000000		//wygaœ starsze 2 cyfry
#define _CLS	 	0b10000000		//wygaœ wszystkie cyfry
#define _CLR_STATUS	0b00000000		//wyzerowanie slowa status
//kolejne bity slowa STATUS dla diod
#define _ADDI	 	0b00000001		//zwiekszenie jasnosci wyswietlania
#define _CLS	 	0b10000000		//wygaœ wszystkie cyfry
#define _CLR_STATUS	0b00000000		//wyzerowanie slowa status


//Data0
#define _SW16		7
#define _SW15		6
#define _SW14		5
#define _SW13		4
#define _SW12		3
#define _SW11		2
#define _SW10		1
#define _SW9		0
//Data1
#define _SW8		7
#define _SW7		6
#define _SW6		5
#define _SW5		4
#define _SW4		3
#define _SW3		2
#define _SW2		1
#define _SW1		0
//Data2
//Data3


/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//i2c_m.c
extern void InitI2C(void);
extern void StartI2CTim(void);
extern bit I2CTim(void);
extern bit TransmitI2C(unsigned char NBTI2C);
extern bit ReceptI2C(const unsigned char NBTI2C);
extern unsigned char GetI2CError(void);
//rtimer.c
extern void StartRTdS(const unsigned char Index);
extern unsigned char RTdS(const unsigned char Index);
//bit.c
extern bit GetBit(const unsigned char data,const unsigned char nbit);
extern void BitSet(unsigned char *data, const unsigned char nbit,const unsigned char bt);

extern unsigned char GetRSRBuf(unsigned char indeks);
/*---------------------------------------------------------------------------------*/
#endif /_IMZ_H
