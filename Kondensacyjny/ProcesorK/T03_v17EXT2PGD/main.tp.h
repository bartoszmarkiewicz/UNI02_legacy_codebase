/*main.tp.h*/
//typy danych dla pliku main.c
//----------------------------------------------------------
#ifndef	_MAIN_TPH
#define	_MAIN_TPH
//----------------------------------------------------------
typedef struct	{
				unsigned _pob_KCO:1;		//znacznik pobudzenia CO z procedury konfiguracji
				unsigned _pob_KCW:1;		//znacznik pobudzenia CW z procedury konfiguracji
				unsigned _pob_SRW:1;		//znacznik pobudzenia z procedury serwisowej
				unsigned _pob_AL:1;			//znacznik pobudzenia CW od funkcji AL z procedury konfiguracji
				unsigned _pob_CW:1;			//znacznik pobudzenia CW
				unsigned _pob_CO:1;			//znacznik pobudzenia CO 
				unsigned _pob_DzCO:1;		//znacznik pobudzenia od temperatury dyzurnej CO
				unsigned _pob_DzCW:1;       //znacznik pobudzenia od temperatury dyzurnej CW
				unsigned _pob_PGDRT:1;      //znacznik pobudzenia od regulatora pogodowego
                
				unsigned _WYLACZ:1;			//aktywacja procedury wybiegu pompy
				unsigned _ZMKCOCW:1;		//aktywacja proc. odp. za zmiane kierunku obrotow pompy
				unsigned _ROZRUCH:1;		//aktywacja procedury Rozruch
				unsigned _RUSZ_48:1;		//aktywacja procedury Rusz48
				unsigned _RUSZ_48Z3:1;		//aktywacja procedury Rusz48 dla WU
				unsigned _RUSZ_PWR:1;		//aktywacja procedury RuszPWR
				unsigned _VWYBIEG:1;		//aktywacja wybiegu wentylatora
				unsigned _RUSZ_PLOMIEN:1;	//aktywacja procedury awaryjnej przy braku plomienia
				unsigned _RUSZ_RPLM:1;		//aktywacja procedury awaryjnej przy braku plomienia
				unsigned _RUSZ_POMPE:1;		//aktywacja awaryjnego rozruchu pompy
				unsigned _BLOKUJ_3M:1;		//aktywacja procedury blokada 3min dla CO
				unsigned _BLOKUJ_15S:1;		//aktywacja procedury blokada 15sek dla CO
				unsigned _MODULUJ:1;		//aktywacja modulu modulacji

				unsigned _AKTYWNE_SRW:1;	//znacznik aktywnosci funkcji serwisowej
				unsigned _AKTYWNE_AL:1;		//znacznik aktywnosci funkcji antylegionella
				unsigned _AKTYWNE_PGD:1;	//znacznik aktywnosci funkcji pogodowej
				unsigned _AKTYWNE_KNF:1;	//znacznik aktywnosci procedury konfiguracji
                unsigned _AKTYWNE_PGD2:1;	//znacznik aktywnosci funkcji pogodowej (tryb niezalezny)
                
				unsigned _RUSZ_NTC_CO:1;	//znacznik awarii czujnika temp CO
				unsigned _RUSZ_NTC_CW:1;	//znacznik awarii czujnika temp CW
				unsigned _RUSZ_NTC_IN:1;	//znacznik awarii czujnika temp CO (powrot)
				unsigned _RUSZ_STB:1;		//znacznik awarii czujnika STB
				unsigned _RUSZ_CISN:1;		//znacznik ze cisnienie CO jest poza dozwolonym zakresem 
				unsigned _RUSZ_NCISN:1;		//znacznik awarii czujnika cisnienia

				unsigned _RS_START:1;		//znacznik rozruchu strtownika po resecie
				unsigned _PW_START:1;		//znacznik rozruchu strtownika po zalaczeniu zasilania
				unsigned _PRACA:1;			//znacznik aktywnej proc. ROZRUCH lub aktywnego stanu GRZEJ
				unsigned _OST_PL:1;			//znacznik ostatniej proby przy braku plomienia
				unsigned _RUN_MNMOD:1;		//praca na minimum modulacji (szybki narost temp. CO)
				unsigned _KCW_PMP:1;		//znacznik obiegu CW dla kotla bitermicznego
				unsigned _DE_VNT:1;			//znacznik probnego wybiegu wentylatora
				unsigned _RPM_CW:1;			//znacznik pracy w obiegu CW dla kotla monotermicznego
				unsigned _prb_CIG:1;		//znacznik pewnego braku ciagu
				unsigned _prb_PRZ:1;		//znacznik pewnego braku przeplywu WG
				unsigned _DE_PMP:1;			//znacznik probnego wybiegu pompy
				unsigned _PO_KAL:1;			//znacznik ze uklad przeszedl procedure kalibracji

                unsigned _LIN:1;            //znacznik aktywnosci interfejsu LIN
                unsigned _LCD:1;			//znacznik aktywnosci interfejsu plytki LCD
                unsigned _LIN_RT:1;         //pobudzenie RT z LIN		
				unsigned _LIN_CW:1;         //pobudzenie CW z LIN
				unsigned _LIN_KSK:1;        //znacznik pracy w trybie kaskady
				unsigned _LIN_STR:1;        //znacznik pracy w trybie sterowania z managera stref lub regulatora RT
				unsigned _LIN_DPGD:1;       //znacznik dezaktywacji wewnetrznego regulatora pogodowego
                unsigned _LIN_ENDO:1;       //zdalna deaktywacja procedury odpowietrzajacej
                
                unsigned _I10V:1;            //znacznik aktywnosci interfejsu 10V                
                unsigned _I10V_RT:1;      
                
                unsigned _IOT:1;            //znacznik aktywnosci interfejsu OT                
                unsigned _IOT_RT:1;         //pobudzenie RT z OT                 
                unsigned _IOT_CW:1;         //pobudzenie CW z OT
                unsigned _IOT_STR:1;        //znacznik pracy w trybie sterowania z managera stref lub regulatora RT
				unsigned _IOT_DPGD:1;       //znacznik dezaktywacji wewnetrznego regulatora pogodowego                

                unsigned _KEY7:1;            //znacznik konsoli 7 Key
                unsigned _KKOM:1;            //stan przycisku funkcji specjalnych konsoli 7 Key
                
				unsigned _BLOKUJ_ODP:1;		//aktywacja blokady przy za niskim cisnieniu - patrz odpowietrzanie obiegu CO/CW
				unsigned _PROC_ODP:1;		//znacznik aktywnosci procedury odpowietrzenia
				unsigned _WYM_ODP:1;		//znacznik bezwarunkowej aktywacja blokady na czas odpowietrzenia (po zalaczeniu zasilania) - patrz odpowietrzanie obiegu CO/CW
				unsigned _HWB_CW:1;			//znacznik przegrzania CO w trakcie wybiegu pompy w obiegu CW - dotyczy kotlow 1F

                unsigned _SYNCH_PGD:1;      //czasowa blokada aktywacji nastawy po aktywacji/dezaktywacji regulatora pogodowego                

                unsigned _KNF_KAL:1;        //flaga wymuszonej kalibracji wentylatora po programowym restarcie oprogramowania
                
                unsigned _LOW_PMP:1;		//obnizona predkosc wybiegu pompy na czas przrzelaczenia zaworu 3DR
                
				unsigned char LICZNIK_PL;	//licznik prob procedury awaryjnej braku plomienia
				unsigned char LICZNIK_PL2;	//licznik prob procedury awaryjnej braku plomienia
				unsigned char LICZNIK_ANL;	//licznik ponowien prob braku ciagu
				unsigned char LICZNIK_ODP;	//licznik odpowietrzenia

				unsigned char ERR_BTY;		//aktualny kod awaryjny
				unsigned char ROZ;
				unsigned char R48;
				unsigned char R48Z3;
				unsigned char BL3;
				unsigned char BL15;
				unsigned char WYL;
				unsigned char RPR;
				unsigned char RPL;
				unsigned char RCI;
				unsigned char MNM;
				unsigned char ZKP;
				unsigned char VWB;
				unsigned char SRV;
				unsigned char KRS;
				unsigned char HVZ;
				unsigned char DPR;
				unsigned char VVNT;			//predkosc obrotowa wentylatora (*100)
				unsigned int rVVNT;			//predkosc obrotowa wentylatora	
				unsigned char ODP;
                
                unsigned char vWSW;         //wersja oprogramowania plytki wyswietlacza (dla <9 wartosc przyjmuje 0)
                unsigned char CONTSETP;     //wyliczona wartosc nastawy CO        
				unsigned char SETPmin;   
				unsigned char SETPmax;   
                
                unsigned char STRB;                
	}tpMAIN;
typedef struct	{
//dane wysylane do LCD
				unsigned char outID;
				unsigned RD_STD:1;				//znacznik odebranej paczki STANDARD
				unsigned RD_KNF:1;				//znacznik odebranej paczki KONFIG
				unsigned RD_LIN:1;				//znacznik odebranej paczki LIN
				unsigned RRD_KNF:1;				//blokada na czas odczytu odebranej paczki KONFIG
                
                //unsigned EXT_SFR:1;             //znacznik rozszerzonej ramki danych standardowych
                //unsigned EXT_KFR:1;             //znacznik rozszerzonej ramki danych konfiguracyjnych
//tryb normalny
				unsigned char outRDZ0;			//kod sterownika
				unsigned char outRDZ1;			//starsze 4bity = czesc A wersji oprogramowania (wersja = A+B), mlodsze 4bity = typ kot³a (_UNICO=0,_MMAXPHB=3)
				unsigned char outRDZ2;			//mlodsze 5bitow = czesc B wersji oprogramowania (wersja = A+B), mlodsze 4bity = typ kot³a (_UNICO=0,_MMAXPHB=3)                
				unsigned char outCO;
				unsigned char outCW;
				unsigned char outCS;
				unsigned char outIN;
				unsigned char outVNT;
				unsigned char outPCO;
				unsigned char outPCW;
				unsigned char outPCOmn;
				unsigned char outPCOmx;
				unsigned char outPCWmn;
				unsigned char outPCWmx;
				unsigned char outPPCO;
				unsigned char outPPCW;
				unsigned char outERR;
				unsigned char outAWR;
				unsigned char outECO;
				unsigned char outPP;
				unsigned char outPG;
				unsigned char outSTAT0;
				unsigned char outSTAT1;
				unsigned char outSTAT2;
				unsigned char outSTAT3;
				unsigned char outSTAT4;
				unsigned char outSTAT5;                
				unsigned char outrCO;
				unsigned char outrCW;
				unsigned char outZW;
				unsigned char outSETPmin;   
				unsigned char outSETPmax;                
//tryb konfiguracji
				unsigned char vKNF;				//wersja paczki danych konfiguracji
				unsigned char outKSTAT0;
				unsigned char outKRK;	//dolna granica dla aktualnie modyfikowanego parametru
				unsigned char outPRM;	//gorna granica dla aktualnie modyfikowanego parametru
				unsigned char outMSTR;
				unsigned char outMMAXU;
				unsigned char outMMAXG;
				unsigned char outWANL;
				unsigned char outKODKT;
				unsigned char outWOBG;
				unsigned char outNOKR;
				unsigned char outOPDG;
				unsigned char outTPCS;
				unsigned char outMMIN;
				unsigned char outMMAX;
				unsigned char outRDPM;
				unsigned char outDTPM;
				unsigned char outMDPM;
				unsigned char outTECO;
				unsigned char outTMMAXG2;
				unsigned char outMMAXG2;
                unsigned char outMGPM;
                unsigned char outMGPM2;
                unsigned char outALIN;
				unsigned char outHSCO;
				unsigned char outCONW;
				unsigned char outPROP;
				unsigned char outRFAN;
                unsigned char outTSTR;
                unsigned char outTL3;        
				unsigned char outPGDR;  
                unsigned char outPGDG;
                unsigned char outPGDO;
                unsigned char outPGMX;
                unsigned char outCWMG;                
                unsigned char outCWTM;                
                
				unsigned char outRPKNF;	//wartosc parametru do odczytu
				unsigned char outWPKNF;	//wartosc parametru po zapisie
				unsigned char outPKERR;	//status operacji zapis/odczyt
//dane odczytane z LCD
//tryb normalny
				unsigned char vSTD;				//wersja paczki danych z plytki LCD
				unsigned char inRDZ0;			//wersja plytki wyswietlacza (dla wersji wyswietlacza <9 wartosc 0)                
				unsigned char inPCO;
				unsigned char inPCW;
				unsigned char inECO;
				unsigned char inSTAT0;
				unsigned char inSTAT1;
				unsigned char inINSTAT;                
				unsigned char inLNSTAT;
				unsigned char inCONTSETP;
				unsigned char inSTAT2;
                
//tryb konfiguracji
				unsigned char inKSTAT0;
				unsigned char inKRK;	//krok procedury konfiguracji
				unsigned char inPRM;	//nr parametru
				unsigned char inDDTA;	//odswierzana dynamicznie warosc modyfikowanego parametru (niezalezna od akceptacji zmian)
				unsigned char inMSTR;
				unsigned char inMMAXU;
				unsigned char inMMAXG;
				unsigned char inWANL;
				unsigned char inKODKT;
				unsigned char inWOBG;
				unsigned char inNOKR;
				unsigned char inOPDG;
				unsigned char inTPCS;
				unsigned char inMMIN;
				unsigned char inMMAX;
				unsigned char inRDPM;
				unsigned char inDTPM;
				unsigned char inMDPM;
				unsigned char inTECO;
				unsigned char inTMMAXG2;
				unsigned char inMMAXG2;
                unsigned char inMGPM;
                unsigned char inMGPM2;
				unsigned char inHSCO;
				unsigned char inCONW;
				unsigned char inPROP;
				unsigned char inRFAN;  
                unsigned char inTSTR;
                unsigned char inTL3;    
				unsigned char inPGDR;  
                unsigned char inPGDG;
                unsigned char inPGDO;                 
                unsigned char inPGMX;     
                unsigned char inCWMG;                 
                unsigned char inCWTM;                
//komunikacja LIN
                unsigned char inLIDN;
                unsigned char inLDTA1;
                unsigned char inLDTA2;
                unsigned char inLDTA3;
                unsigned char inLDTA4;
                unsigned char inLDTA5;                
}tpRSDTA1;								
//----------------------------------------------------------
#endif /*_MAIN_TPH*/
