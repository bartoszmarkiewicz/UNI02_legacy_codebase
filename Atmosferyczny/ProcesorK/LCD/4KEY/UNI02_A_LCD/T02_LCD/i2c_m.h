/*i2c_m.h*/
/*I2C Master Mode*/
#ifndef	_I2C_H
#define	_I2C_H

/*---------------------------------------------------------------------------------*/
/*-----------------------------PUBLIC DATA-----------------------------------------*/
#define _NBDATA		24				//rozmiar bufora danych
#define _I2COSC		100				//czestotliwosc SCK [kHZ]
#define _I2CPAUSE	10				//przerwa miedzy transmisjami = I2CPAUSE*0.01 sek
#define _I2CTMOUT	2				//timeout = I2CTMOUT*0.01 sek
/*---------------------------------------------------------------------------------*/
/*-----------------------------PROTECTED DATA--------------------------------------*/
#include "i2c_m.tp.h"		//typy danych
/*---------------------------------------------------------------------------------*/
#ifndef _EXT_OSC				//czestotliwosc oscylatora [MHz]
#error Nieokreslony parametr _EXT_OSC
#endif
#ifndef _PLL					//modul _PLL (0-nieaktywny,1-aktywny)
#error Nieokreslony parametr _PLL
#endif
#define _I2CFOSC		_EXT_OSC*1000
#if _PLL==0
#define _I2CSPADD	(_I2CFOSC/(4*_I2COSC))-1
#endif
#if _PLL!=0
#define _I2CSPADD	(_I2CFOSC/_I2COSC)-1
#endif
#if _I2CSPADD<=0			
#error Nieprawidlowa wartosc parametru _I2CSPADD
#endif
//----------------------------------------------------------
//Wartosci jakie mo¿e przyjmowaæ StatI2C.error:
//transmisja danych 
#define _EtBCLIF	1		//kolizja na interfejsie przy transmisji danych
#define _EtTIMO1	2		//b³ad przerwania przy transmisji bitu startu
#define _EtTIMO2	3		//b³ad przerwania przy transmisji danej
#define _EtTIMO3	4		//brak potwierdzenia odbioru danej przez nadajnik
#define _EtTIMO4	5		//b³ad przerwania przy transmisji bitu stopu
//odbior danych
#define _ErBCLIF	6		//kolizja na interfejsie przy odbiorze danych
#define _ErWCOL		7		//nadpisanie danej przy zapisie
#define _ErSSPOV	8		//nadpisanie danej przy odbiorze
#define _ErTIMO1	9		//b³ad przerwania przy transmisji bitu startu
#define _ErTIMO2	10		//b³ad przerwania przy transmisji adresu
#define _ErTIMO3	11		//brak potwierdzenia adresu przez nadajnik 			
#define _ErTIMO4	12		//b³ad przerwania przy odbiorze danej
#define _ErTIMO5	13		//brak potwierdzenia po odbiorze danej
#define _ErTIMO6	14		//b³ad przerwania przy transmisji bitu stopu

//----------------------------------------------------------
#endif /*_I2C_H*/
