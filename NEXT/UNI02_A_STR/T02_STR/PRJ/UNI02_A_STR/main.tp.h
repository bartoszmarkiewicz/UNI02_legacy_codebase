/*main.tp.h*/
//typy danych dla pliku main.c
//----------------------------------------------------------
typedef struct	{
        union   {
            unsigned char flag0;
            struct
            {    
				unsigned _pob_KCO:1;		//znacznik pobudzenia CO z procedury konfiguracji
				unsigned _pob_KCW:1;		//znacznik pobudzenia CW z procedury konfiguracji
				unsigned _pob_SRW:1;		//znacznik pobudzenia z procedury serwisowej
				unsigned _pob_AL:1;			//znacznik pobudzenia CW od funkcji AL z procedury konfiguracji
				unsigned _pob_CW:1;			//znacznik pobudzenia CW
				unsigned _pob_CO:1;			//znacznik pobudzenia CO 
				unsigned _pob_DzCO:1;		//znacznik pobudzenia od temperatury dyzurnej CO
				unsigned _pob_DzCW:1;       //znacznik pobudzenia od temperatury dyzurnej CW
            };
        }; 
        union   {
            unsigned char flag1;
            struct
            {        
				unsigned _pob_PGDRT:1;      //znacznik pobudzenia od regulatora pogodowego                
				unsigned _WYLACZ:1;			//aktywacja procedury wybiegu pompy
				unsigned _ZMKCOCW:1;		//aktywacja proc. odp. za zmiane kierunku obrotow pompy
				unsigned _ROZRUCH:1;		//aktywacja procedury Rozruch
				unsigned _RUSZ_48:1;		//aktywacja procedury Rusz48
				unsigned _RUSZ_48Z3:1;		//aktywacja procedury Rusz48 dla WU
				unsigned _RUSZ_PWR:1;		//aktywacja procedury RuszPWR
				unsigned _VWYBIEG:1;		//aktywacja wybiegu wentylatora
            };
        }; 
        union   {
            unsigned char flag2;
            struct
            {        
				unsigned _RUSZ_CIAG:1;		//aktywacja procedury oczekiwania na powrot ciagu
				unsigned _RUSZ_PLOMIEN:1;	//aktywacja procedury awaryjnej przy braku plomienia
				unsigned _RUSZ_RPLM:1;		//aktywacja procedury awaryjnej przy braku plomienia
				unsigned _RUSZ_POMPE:1;		//aktywacja awaryjnego rozruchu pompy
				unsigned _BLOKUJ_3M:1;		//aktywacja procedury blokada 3min dla CO
				unsigned _BLOKUJ_15S:1;		//aktywacja procedury blokada 15sek dla CO
				unsigned _MODULUJ:1;		//aktywacja modulu modulacji
				unsigned _AKTYWNE_SRW:1;	//znacznik aktywnosci funkcji serwisowej
            };
        };
        union   {
            unsigned char flag3;
            struct
            {         
				unsigned _AKTYWNE_AL:1;		//znacznik aktywnosci funkcji antylegionella
				unsigned _AKTYWNE_PGD:1;	//znacznik aktywnosci funkcji pogodowej
				unsigned _AKTYWNE_KNF:1;	//znacznik aktywnosci procedury konfiguracji
                unsigned _AKTYWNE_PGD2:1;	//znacznik aktywnosci funkcji pogodowej (tryb niezalezny)                
				unsigned _RUSZ_NTC_CO:1;	//znacznik awarii czujnika temp CO
				unsigned _RUSZ_NTC_CW:1;	//znacznik awarii czujnika temp CW
				unsigned _RUSZ_NTC_IN:1;	//znacznik awarii czujnika temp CO (powrot)
				unsigned _RUSZ_STB:1;		//znacznik awarii czujnika STB
            };
        };        
        union   {
            unsigned char flag4;
            struct
            {        
				unsigned _RUSZ_MOD:1;		//znacznik awarii w obwodzie modulatora 
				unsigned _RUSZ_CISN:1;		//znacznik ze cisnienie CO jest poza dozwolonym zakresem 
				unsigned _RUSZ_NCISN:1;		//znacznik awarii czujnika cisnienia
				unsigned _RS_START:1;		//znacznik rozruchu strtownika po resecie
				unsigned _PW_START:1;		//znacznik rozruchu strtownika po zalaczeniu zasilania
				unsigned _TURBO:1;			//znacznik rodzaju kotla (0-atmosferyczny, 1- turbo)				
				unsigned _PRACA:1;			//znacznik aktywnej proc. ROZRUCH lub aktywnego stanu GRZEJ
				unsigned _OST_PL:1;			//znacznik ostatniej proby przy braku plomienia
            };
        };
        union   {
            unsigned char flag5;
            struct
            {        
				unsigned _RUN_MNMOD:1;		//praca na minimum modulacji (szybki narost temp. CO)
				unsigned _KCW_PMP:1;		//znacznik obiegu CW dla kotla bitermicznego
				unsigned _DE_VNT:1;			//znacznik probnego wybiegu wentylatora
				unsigned _RPM_CW:1;			//znacznik pracy w obiegu CW dla kotla monotermicznego
				unsigned _prb_CIG:1;		//znacznik pewnego braku ciagu
				unsigned _prb_PRZ:1;		//znacznik pewnego braku przeplywu WG
				unsigned _DE_PMP:1;			//znacznik probnego wybiegu pompy
                unsigned _LIN:1;            //znacznik aktywnosci interfejsu LIN
            };
        };                
        union   {
            unsigned char flag6;
            struct
            {                
                unsigned _LCD:1;			//znacznik aktywnosci interfejsu plytki LCD
                unsigned _LIN_RT:1;         //pobudzenie RT z LIN		
				unsigned _LIN_CW:1;         //pobudzenie CW z LIN
				unsigned _LIN_KSK:1;        //znacznik pracy w trybie kaskady
				unsigned _LIN_STR:1;        //znacznik pracy w trybie sterowania z managera stref lub regulatora RT
				unsigned _LIN_DPGD:1;       //znacznik dezaktywacji wewnetrznego regulatora pogodowego
                unsigned _LIN_ENDO:1;       //zdalna deaktywacja procedury odpowietrzajacej                
                unsigned _I10V:1;            //znacznik aktywnosci interfejsu 10V  
            };
        };            
        union   {
            unsigned char flag7;
            struct
            {        
                unsigned _I10V_RT:1;                     
                unsigned _IOT:1;            //znacznik aktywnosci interfejsu OT                
                unsigned _IOT_RT:1;         //pobudzenie RT z OT                 
                unsigned _IOT_CW:1;         //pobudzenie CW z OT
                unsigned _IOT_STR:1;        //znacznik pracy w trybie sterowania z managera stref lub regulatora RT
				unsigned _IOT_DPGD:1;       //znacznik dezaktywacji wewnetrznego regulatora pogodowego                                
                unsigned _KEY7:1;            //znacznik konsoli 7 Key
                unsigned _KKOM:1;            //stan przycisku funkcji specjalnych konsoli 7 Key  
            };
        };     
        union   {
            unsigned char flag8;
            struct
            {         
                unsigned _SYNCH_PGD:1;      //czasowa blokada aktywacji nastawy po aktywacji/dezaktywacji regulatora pogodowego  
                unsigned _LOW_PMP:1;		//obnizona predkosc wybiegu pompy na czas przrzelaczenia zaworu 3DR 
                unsigned _fl8_bt2:1; 
                unsigned _fl8_bt3:1; 
                unsigned _fl8_bt4:1;
                unsigned _fl8_bt5:1; 
                unsigned _fl8_bt6:1; 
                unsigned _fl8_bt7:1;               
            };
        };
        
				unsigned char LICZNIK_PL;	//licznik prob procedury awaryjnej braku plomienia
				unsigned char LICZNIK_CIG;	//licznik ponowien prob braku ciagu (turbo)
				unsigned char LICZNIK_CIG2;	//licznik ponowien prob braku ciagu	(atmosferyczny)
				unsigned char LICZNIK_CIG3;	//licznik ponowien prob braku ciagu
				unsigned char LICZNIK_ANL;	

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
                
                unsigned char vWSW;         //wersja oprogramowania plytki wyswietlacza (dla <9 wartosc przyjmuje 0)
                unsigned char CONTSETP;     //wyliczona wartosc nastawy CO        
				unsigned char SETPmin;   
				unsigned char SETPmax;        
                
                unsigned char STRB;                

                unsigned int RGDZ;          //godziny pracy urzadzenia
                unsigned int RSDZ;          //godziny pracy urzadzenia (sekundy)                
                unsigned int RMDZ;          //godziny pracy urzadzenia (minuty)
		}tpMAIN;
typedef struct	{
//dane wysylane do LCD
        union   {
            unsigned char flag0;
            struct
            {                
				unsigned RD_STD:1;				//znacznik odebranej paczki STANDARD
				unsigned RD_KNF:1;				//znacznik odebranej paczki KONFIG
				unsigned RD_LIN:1;				//znacznik odebranej paczki LIN
				unsigned RRD_KNF:1;				//blokada na czas odczytu odebranej paczki KONFIG
                unsigned _fl0_bt4:1;
                unsigned _fl0_bt5:1; 
                unsigned _fl0_bt6:1; 
                unsigned _fl0_bt7:1;                
            };
        }; 
				unsigned char outID;        
//tryb normalny

				unsigned char outRDZ0;          //kod sterownika
				unsigned char outRDZ1;          //starsze 4bity = czesc A wersji oprogramowania (wersja = A+B), mlodsze 4bity = typ kot?a (_UNICO=0,_MMAXPHB=3)
				unsigned char outRDZ2;			//mlodsze 5bitow = czesc B wersji oprogramowania (wersja = A+B), mlodsze 4bity = typ kot?a (_UNICO=0,_MMAXPHB=3)                
				unsigned char outCO;
				unsigned char outCW;
				unsigned char outCS;
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
				unsigned char outSTAT0;
				unsigned char outSTAT1;
				unsigned char outSTAT2;
				unsigned char outSTAT3;
				unsigned char outSTAT4; 
				unsigned char outSTAT5;                
				unsigned char outIN;
				unsigned char outPP;
				unsigned char outPG;                
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
				unsigned char outMSTRmn;
				unsigned char outMSTRmx;
				unsigned char outMMAXU;
				unsigned char outMMAXUmn;
				unsigned char outMMAXUmx;
				unsigned char outMMAXG;
				unsigned char outMMAXGmn;
				unsigned char outMMAXGmx;
				unsigned char outRGZ;
				unsigned char outRGZmn;
				unsigned char outRGZmx;
				unsigned char outWANL;
				unsigned char outTPKT;
				unsigned char outTPKTmn;
				unsigned char outTPKTmx;
				unsigned char outWOBG;
				unsigned char outOPDG;
				unsigned char outTPCS;

				unsigned char outRDPM;
				unsigned char outDTPM;
				unsigned char outMDPM;
                unsigned char outMGPM;

				unsigned char outMGPM2;
				unsigned char outHSCO;
				unsigned char outCONW;
				unsigned char outPROP;
                unsigned char outPGMX;
                unsigned char outPGDR;     
                unsigned char outCWMG;                
                unsigned char outCWTM;
                
                unsigned char outALIN;                

				unsigned char outRPKNF;	//wartosc parametru do odczytu
				unsigned char outWPKNF;	//wartosc parametru po zapisie
				unsigned char outPKERR;	//status operacji zapis/odczyt
//dane odczytane z LCD
//tryb normalny
				unsigned char vSTD;				//wersja paczki danych z plytki LCD
				unsigned char inRDZ0;			//wersja plytki wyswietlacza (dla wersji wyswietlacza <9 wartosc 0)                
				unsigned char inPCO;
				unsigned char inPCW;
				unsigned char inSTAT0;
				unsigned char inSTAT1;
				unsigned char inINSTAT;                
				unsigned char inLNSTAT;
				unsigned char inCONTSETP;              
				unsigned char inSTAT2;                
//tryb konfiguracji
				unsigned char inKSTAT0;
				unsigned char inKRK;
				unsigned char inPRM;
				unsigned char inDDTA;
				unsigned char inMSTR;
				unsigned char inMMAXU;
				unsigned char inMMAXG;
				unsigned char inRGZ;
				unsigned char inWANL;
				unsigned char inTPKT;
				unsigned char inWOBG;
				unsigned char inOPDG;
				unsigned char inTPCS;

				unsigned char inRDPM;
				unsigned char inDTPM;
				unsigned char inMDPM;
				unsigned char inMGPM;

                unsigned char inMGPM2;
				unsigned char inHSCO;
				unsigned char inCONW;
				unsigned char inPROP;
                unsigned char inPGMX; 
                unsigned char inPGDR;    
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
