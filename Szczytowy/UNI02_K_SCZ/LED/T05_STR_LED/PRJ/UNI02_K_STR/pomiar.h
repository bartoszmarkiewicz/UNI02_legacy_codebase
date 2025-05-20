/*pomiar.h*/
#ifndef	_POMIAR_H
#define	_POMIAR_H

#include "main.h"
#include "pomiar.tp.h"		//typy danych
//----------------------------------------------------------
#define _NTC_MIN 0X28	//0x14 wartosc AC przy ktorej stwierdza sie uszkodzenie czujnika NTC
						//da AC <=_NTC_MIN stan error=1. 
#define _NTC_MAX 0XF5	//0xe1 wartosc AC przy ktorej stwierdza sie uszkodzenie czujnika NTC
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
#define _CONST_TRB 1    //staly tryb pracy (zima)
#define _NO_ZW 1        //brak czujnika ZW
#define _NO_CW 1        //brak czujnika CW
#define _NO_CS 1        //brak czujnika cisnienia
#define _NO_IN 1        //brak czujnika powrotu
#define _NO_ECO 1       //brak funkcji ECO
#define _KEY_PCO 1      //zmiana nastawy PCO przyciskami
//----------------------------------------------------------
//----------------------------------------------------------				
//Pomiar CO
#define _CHN_CO	COTemp			//kanal poniarowy dla czujnika CO
#define _ON_DZ_CO 8			//temp. dyzurna zalaczenia grzania
#define _OFF_DZ_CO 20		//temp. dyzurna wylaczenia grzania
#define _ON_SRW_CO 73		//78 zalaczenie dla CO przy pracy serwisowej
#define _OFF_SRW_CO 83		//88 wylaczenie dla CO przy pracy serwisowej
#define _ONP_SRW_CO 48		//zalaczenie dla CO przy pracy serwisowej(ogrzewanie podlogowe)
#define _OFFP_SRW_CO 58		//wylaczenie dla CO przy pracy serwisowej(ogrzewanie podlogowe)
#define _PRIOR_RT 0         //0-sterowanie z priorytetem SETP, 1-sterowanie z priorytetem RT
//monoterm
#define F2_CLD_CO 7			//dolna granica temp. ciaglej pracy pompy
#define F2_HOT_CO 88		//83 gorna granica temp. ciaglej pracy pompy		
#define F2_SU_CO 5			//granica zalaczenia PCO-CO>=_SU_CO
#define F2_AD_CO 5			//granica wylaczenia CO-PCO>=_AD_CO dla (nastawa >(_MAX_PCO+F2_AD_CO)-DtCO.HstCo)
//#define F2_AD_CO2 5			//10 granica wylaczenia CO-PCO>=_AD_CO dla PCO<=75
#define F2_PZ_CO 87			//77 punkt zalaczenia od CO przy grzaniu CW (hot *****)
#define F2_PW_CO 91			//83 punkt wylaczenia od CO przy grzaniu CW (hot *****)
#define F2_PTWP_CO F2_PZ_CO	//temperaturowy CO punkt wylaczenia pompy dla grzania CW

//zasobnik/unico
#define F3_CLD_CO 7			//dolna granica temp. ciaglej pracy pompy
#define F3_HOT_CO 88		//83 gorna granica temp. ciaglej pracy pompy		
#define F3_SU_CO 5			//granica zalaczenia PCO-CO>=_SU_CO
#define F3_AD_CO 5			//granica wylaczenia CO-PCO>=_AD_CO	dla (nastawa >(_MAX_PCO+F3_AD_CO)-DtCO.HstCo)

#define _MAX_DXCO 6			//maksymalny dozwolony narost temperatury
#define _DM_GECO 11			//CO<=PCO dolny margines dla celow GoldenEye
#define _DM_GECO2 11		//5CO>PCO gorny margines dla celow GoldenEye
#define _BNCO 3				//CO<=PCOwielkosc porawki wskazu CO dla GoldenEye
#define _BNCO2 1			//CO>PCO wielkosc porawki wskazu CO dla GoldenEye

#if _BNCO>_DM_GECO/2
#error Bledna wartosc parametru _BNCO
#endif
#if _BNCO2>_DM_GECO2/2
#error Bledna wartosc parametru _BNCO2
#endif
//----------------------------------------------------------
//----------------------------------------------------------
//Pomiar CW
#define _CHN_CW	CWTemp			//kanal poniarowy dla czujnika CW
#define _ON_DZ_CW 5			//temp. dyzurna zalaczenia grzania
#define _OFF_DZ_CW 10		//temp. dyzurna wylaczenia grzania
#define _CLD_CW _ON_DZ_CW	//dolna granica temp. ciaglej pracy pompy
#define _OFF_KCW 85         //granica powyzej ktorej zawor 3DR wysterowany jest tylko na obieg CO, powrót do normalnej pracy po spadku temp. poni¿ej tego progu

//monoterm
#define F2_OFF_CW	65	//bezwzgledna granica wylaczenia CW>_OFF_CW
#define F2_MR_OFF_CW 2	//marg. dla bezwzglednej granicy zalaczenia CW<=_OFF_CW-MR_OFF_CW
#define F2_SU_CW	1	//granica zalaczenia PCW-CW>=_SU_CW
#define F2_AD_CW	5	//granica wylaczenia CW-PCW>=_AD_CW

//zasobnik/unico 
#define F3_SU_CW	5	//granica zalaczenia PCW-CW>=_SU_CW
#define F3_AD_CW	1	//granica wylaczenia CW-PCW>=_AD_CW


#define _DM_GECW 11			//CW<=PCW dolny margines dla celow GoldenEye
#define _DM_GECW2 5			//CW>PCW dolny margines dla celow GoldenEye
#define _BNCW 3				//CW<=PCWwielkosc porawki wskazu CW dla GoldenEye
#define _BNCW2 2			//1 CW>PCW wielkosc porawki wskazu CW dla GoldenEye

#if _BNCW>_DM_GECW/2
#error Bledna wartosc parametru _BNCW
#endif
#if _BNCW2>_DM_GECW2/2
#error Bledna wartosc parametru _BNCW2
#endif

//-------------
//Pomiar temperatury zewnetrznej
#define _CHN_ZW	ZWTemp		//kanal poniarowy dla czujnika temperatury zewnetrznej
//regulator pogodowy 
#define _TESTPGD 0          //tryb testowy pracy regulatora - bez timera
#define _TPGDOFF 3			//3 [h] czas do deaktywacji pobudzenia
//----------------------------------------------------------
//----------------------------------------------------------
//pomiar PCO
#define _MIN_PCO 40			//dolna granica nastawy CO
#define _MAX_PCO 80			//80gorna granica nastawy	CO ************
#define _MIN_PDCO 25		//dolna granica nastawy CO dla ogrzewania podlogowego
#define _MAX_PDCO 55		//gorna granica nastawy	CO dla ogrzewania podlogowego
#define _MIN_PGD 0			//dolna granica nastawy Kt dla ogrzewania pogodowego
#define _MAX_PGD 99			//gorna granica nastawy Kt dla ogrzewania pogodowego
#define _PGD_PCO _MAX_PCO	//domyslna wartosc nastawy CO dla ogrzewania pogodowego
#define _KNF_PCO _MAX_PCO	//domyslna wartosc nastawy CO dla konfiguracji

//monoterm
#define F2_PPCO 89			//_MAX_PCO	//domyslna wartosc nastawy PCO przy grzaniu w obiegu CW (hot *****)
//zasobnik
#define _MR_MADCO 9			//6dolny margines dla celow modulacji (PCO-_MR_MADCO)
#define _TWRTPCO 20			//[ds] czas po ktorym nastawa (po zmianie wart.) zostanie zapisana w pamieci EEPROM
#define _TFLPCO 5			//[ds] czas po ktorym nastawa po modyfikacji zacznie migaæ
//----------------------------------------------------------
//Przewyzszenie temperaturowe
#define _MXPW_CO  (_MAX_PCO+5)              //granica dla nastawy z przewyzszeniem (ogrzewanie tradycyjne)
#define _MXPPW_CO (_MAX_PDCO+5)             //granica dla nastawy z przewyzszeniem (ogrzewanie podlogowe)
//monoterm
#define F2_PWT_CO  (_MXPW_CO+3)              //gorna granica wylaczenia grzania (ogrzewanie tradycyjne)
#define F2_PPWT_CO (_MXPPW_CO+F2_AD_CO)	    //gorna granica wylaczenia grzania (ogrzewanie podlogowe)
//zasobnik
#define F3_PWT_CO  (_MXPW_CO+3)              //gorna granica wylaczenia grzania (ogrzewanie tradycyjne)
#define F3_PPWT_CO (_MXPPW_CO+F3_AD_CO)	    //gorna granica wylaczenia grzania (ogrzewanie podlogowe)
//----------------------------------------------------------
//pomiar PCW
//monoterm
#define F2_MIN_PCW 30		//dolna granica nastawy
#define F2_MAX_PCW 60		//gorna granica nastawy
//zasobnik
#define F3_MIN_PCW 35		//dolna granica nastawy
#define F3_MAX_PCW 65		//gorna granica nastawy

#define _AL_PCW	70			//domyslna wartosc nastawy CW dla funkcji Anty-Legionella
#define _KNF_PCW 60			//domyslna wartosc nastawy CW dla konfiguracji

#define _MR_MADCW 6			// dolny margines dla celow modulacji (PCW-_MR_MADCW)	
#define _TWRTPCW 10			//[ds] czas po ktorym nastawa (po zmianie wart.) zostanie zapisana w pamieci EEPROM

//----------------------------------------------------------
//----------------------------------------------------------				
//Pomiar IN
#define _CHN_IN	INTemp			//kanal poniarowy dla czujnika IN
#define _HOT_IN_CO  95          //wysoka temperatura na powrocie CO

#define N_MIN_ECO 1			//dolna granica nastawy ECO
#define N_MAX_ECO 9			//gorna granica nastawy ECO
#define _PGDECO 5			//wartosc domyslna ECO dla regulacji pogodowej
#define _TWRTECO 10			//[ds] czas po ktorym nastawa (po zmianie wart.) zostanie zapisana w pamieci EEPROM
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
	#define _CS5_0 0x23			//29wskaz minimalny
	#define _CS5_4 0xBE			//c0wskaz maksymalny
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
//pomiar PFN
#define _ON_ZIMA	0xF1	//UWAGA wartosc ON ma byc zawsze wieksza od OFF	
#define _OFF_ZIMA	0xE8
#define _ON_LATO	0xA9	
#define _OFF_LATO	0x9F
#define _ON_OFF		0x6B
#define _OFF_OFF	0x62
#define _TWRTPFN 10			//[ds] czas po ktorym zmiana pozycji PFN (po zmianie wart.) zostanie zapisana w pamieci EEPROM
//----------------------------------------------------------	
//----------------------------------------------------------
void SetModPCO(void);
unsigned char GetPGD_PCO(void);
//----------------------------------------------------------
#endif		/*_POMIAR_H*/
