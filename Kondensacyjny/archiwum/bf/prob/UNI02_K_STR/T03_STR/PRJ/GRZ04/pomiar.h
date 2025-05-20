/*pomiar.h*/
#ifndef	_POMIAR_H
#define	_POMIAR_H

#include "main.h"
#include "pomiar.tp.h"		//typy danych
//----------------------------------------------------------
#define _NTC_MIN 0x28	//0x14 wartosc AC przy ktorej stwierdza sie uszkodzenie czujnika NTC
						//da AC <=_NTC_MIN stan error=1. 
#define _NTC_MAX 0xF5	//0xe1 wartosc AC przy ktorej stwierdza sie uszkodzenie czujnika NTC
						//da AC >=_NTC_MAX stan error=1.
//----------------------------------------------------------
#define _ZNTC_MIN 0x19	//0x14 wartosc AC przy ktorej stwierdza sie uszkodzenie czujnika NTC
						//da AC <=_NTC_MIN stan error=1. 
#define _ZNTC_MAX 0xF0	//0xe1 wartosc AC przy ktorej stwierdza sie uszkodzenie czujnika NTC
						//da AC >=_NTC_MAX stan error=1.
//----------------------------------------------------------
//---------------------------------------------------------- 
#define _ITFTM 10		//interwal czasowy filtru parametrow modulatora
#define _ITFGE 20		//interwal czasowy filtru GoldenEye
#define _ITFOP 400		//interwal czasowy filtru OPENTERM
//----------------------------------------------------------
//----------------------------------------------------------
//Pomiar CW
#define _CHN_CW	CWTemp			//kanal poniarowy dla czujnika CW
#define _ON_DZ_CW 5			//temp. dyzurna zalaczenia grzania
#define _OFF_DZ_CW 10		//temp. dyzurna wylaczenia grzania
#define _CLD_CW _ON_DZ_CW	//dolna granica temp. ciaglej pracy pompy
//biterm 
#define F1_OFF_CW	65	//bezwzgledna granica wylaczenia CW>_OFF_CW
#define F1_OFF2_CW	70	//bezwzgledna granica wylaczenia CW>_OFF_CW
#define F1_MR_OFF_CW 2	//marg. dla bezwzglednej granicy zalaczenia CW<=_OFF_CW-MR_OFF_CW
#define F1_SU_CW	1	//granica zalaczenia PCW-CW>=_SU_CW
#define F1_AD_CW	3	//granica wylaczenia CW-PCW>=_AD_CW
#define F1_MRG_CW 20	//dolny margines dla celow modulacji
//monoterm 
#define F2_OFF_CW	65	//bezwzgledna granica wylaczenia CW>_OFF_CW
#define F2_MR_OFF_CW 2	//marg. dla bezwzglednej granicy zalaczenia CW<=_OFF_CW-MR_OFF_CW
#define F2_SU_CW	1	//granica zalaczenia PCW-CW>=_SU_CW
#define F2_AD_CW	3	//granica wylaczenia CW-PCW>=_AD_CW
#define F2_MRG_CW 20	//dolny margines dla celow modulacji
//zasobnik/unico 
#define F3_SU_CW	5	//granica zalaczenia PCW-CW>=_SU_CW
#define F3_AD_CW	1	//granica wylaczenia CW-PCW>=_AD_CW
#define F3_MRG_CW 20	//dolny margines dla celow modulacji

#define _DM_GECW 11		//8dolny margines dla celow GoldenEye
#define _BNCW 3			//2wielkosc porawki wskazu CW dla GoldenEye
#if _BNCW>_DM_GECW/2
#error Bledna wartosc parametru _BNCW
#endif
//----------------------------------------------------------
//----------------------------------------------------------
//pomiar PCW
//biterm
#define F1_MIN_PCW 35		//dolna granica nastawy
#define F1_MAX_PCW 60		//gorna granica nastawy
//monoterm
#define F2_MIN_PCW 30		//dolna granica nastawy
#define F2_MAX_PCW 60		//gorna granica nastawy
//zasobnik
#define F3_MIN_PCW 35		//dolna granica nastawy
#define F3_MAX_PCW 65		//gorna granica nastawy

#define _AL_PCW	70			//domyslna wartosc nastawy CW dla funkcji Anty-Legionella
#define _KNF_PCW 60			//domyslna wartosc nastawy CW dla konfiguracji

#define _MR_MADCW 9			//dolny margines dla celow modulacji (PCW-_MR_MADCW)	

#define _TWRTPCW 20			//[ds] czas po ktorym nastawa (po zmianie wart.) zostanie zapisana w pamieci EEPROM
#define _TFLPCW 5			//[ds] czas po ktorym nastawa po modyfikacji zacznie migaæ
//----------------------------------------------------------	
//----------------------------------------------------------
//pomiar CS
#if _VER5V==0 || defined(_18F46K20)|| defined(_18F47Q10)
	//pomiar CS (3V)
	//parametry AD
	//czujnik zasilany 18V
	#define _CHN_CS CisnCO			//kanal pomiarowy dla czujnika cisnienia
	#define _CS_MIN 0x0F			//1Adolny margines bledu
	#define _CS_MAX 0xF0			//gorny margines bledu
	#define _CS_0 0x1F			//30wskaz minimalny
	#define _CS_4 0xE4//210			//216wskaz maksymalny
	#define _NWCS 40			//liczba ziaren na zakres pomiarowy
	//czujnik zasilany 5V
	#define _CHN_CS5 CisnCO			//kanal pomiarowy dla czujnika cisnienia
	#define _CS5_MIN 16			//dolny margines bledu
	#define _CS5_MAX 0xD0			//gorny margines bledu
	#define _CS5_0 0x29			//22wskaz minimalny
	#define _CS5_4 0xC0			//216wskaz maksymalny
	#define _NWCS5 40			//liczba ziaren na zakres pomiarowy
#else 
	//pomiar CS (5V)
	//parametry AD
	//czujnik zasilany 18V
	#define _CHN_CS CisnCO			//kanal pomiarowy dla czujnika cisnienia
	#define _CS_MIN 16			//dolny margines bledu
	#define _CS_MAX 220			//gorny margines bledu
	#define _CS_0 22			//wskaz minimalny
	#define _CS_4 175			//wskaz maksymalny
	#define _NWCS 40			//liczba ziaren na zakres pomiarowy
	//czujnik zasilany 5V
	#define _CHN_CS5 CisnCO			//kanal pomiarowy dla czujnika cisnienia
	#define _CS5_MIN 16			//dolny margines bledu
	#define _CS5_MAX 220			//gorny margines bledu
	#define _CS5_0 22			//wskaz minimalny
	#define _CS5_4 175			//wskaz maksymalny
	#define _NWCS5 40			//liczba ziaren na zakres pomiarowy
#endif
//parametry po konwersji
#define _NO_CSN 3			//cisnienie ponizej 0.3 bara
#define _LO_CSN 5			//cisnienie ponizej 0.5 bara
#define _ON_HI_CSN 29		//2.9 bara - prog ustawienia wskazu zbyt wysokiego cisnienia
#define _ON1_WRK_CSN 6		//0.5 bara - dolny prog cisnienia roboczego
#define _ON2_WRK_CSN 27		//2.7 bara - gorny prog cisnienia roboczego
#define _OFF1_WRK_CSN 4		//0.3 bara - dolny prog cisnienia awaryjnego
#define _OFF2_WRK_CSN 29	//2.9 bara - gorny prog cisnienia awaryjnego
//----------------------------------------------------------
//----------------------------------------------------------

//----------------------------------------------------------
#endif		/*_POMIAR_H*/
