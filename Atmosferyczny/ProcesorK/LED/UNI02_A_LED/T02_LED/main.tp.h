/*main.tp.h*/
//typy danych dla pliku main.c
//----------------------------------------------------------
#ifndef	_MAIN_TPH
#define	_MAIN_TPH
//----------------------------------------------------------
    volatile bit Mf_pob_KCO;		//znacznik pobudzenia CO z procedury konfiguracji
    volatile bit Mf_pob_KCW;		//znacznik pobudzenia CW z procedury konfiguracji
    volatile bit Mf_pob_SRW;		//znacznik pobudzenia z procedury serwisowej
    volatile bit Mf_pob_AL;			//znacznik pobudzenia CW od funkcji AL z procedury konfiguracji
    volatile bit Mf_pob_CW;			//znacznik pobudzenia CW
    volatile bit Mf_pob_CO;			//znacznik pobudzenia CO 
    volatile bit Mf_pob_DzCO;		//znacznik pobudzenia od temperatury dyzurnej CO
    volatile bit Mf_pob_DzCW;		//znacznik pobudzenia od temperatury dyzurnej CW
    volatile bit Mf_WYLACZ;			//aktywacja procedury wybiegu pompy
    volatile bit Mf_ZMKCOCW;		//aktywacja proc. odp. za zmiane kierunku obrotow pompy
    volatile bit Mf_ROZRUCH;		//aktywacja procedury Rozruch
    volatile bit Mf_RUSZ_48;		//aktywacja procedury Rusz48
    volatile bit Mf_RUSZ_48Z3;		//aktywacja procedury Rusz48 dla WU
    volatile bit Mf_RUSZ_PWR;		//aktywacja procedury RuszPWR
    volatile bit Mf_VWYBIEG;		//aktywacja wybiegu wentylatora
    volatile bit Mf_RUSZ_CIAG;		//aktywacja procedury oczekiwania na powrot ciagu  

    volatile bit Mf_RUSZ_PLOMIEN;	//aktywacja procedury awaryjnej przy braku plomienia               
    volatile bit Mf_RUSZ_RPLM;		//aktywacja procedury awaryjnej przy braku plomienia
    volatile bit Mf_RUSZ_POMPE;		//aktywacja awaryjnego rozruchu pompy
    volatile bit Mf_BLOKUJ_3M;		//aktywacja procedury blokada 3min dla CO
    volatile bit Mf_BLOKUJ_15S;		//aktywacja procedury blokada 15sek dla CO
    volatile bit Mf_MODULUJ;		//aktywacja modulu modulacji
    volatile bit Mf_AKTYWNE_SRW;	//znacznik aktywnosci funkcji serwisowej
    volatile bit Mf_AKTYWNE_AL;		//znacznik aktywnosci funkcji antylegionella
    volatile bit Mf_AKTYWNE_PGD;	//znacznik aktywnosci funkcji pogodowej
    volatile bit Mf_AKTYWNE_KNF;	//znacznik aktywnosci procedury konfiguracji
    volatile bit Mf_RUSZ_NTC_CO;	//znacznik awarii czujnika temp CO
    volatile bit Mf_RUSZ_NTC_CW;	//znacznik awarii czujnika temp CW
    volatile bit Mf_RUSZ_NTC_IN;	//znacznik awarii czujnika temp CO (powrot)
    volatile bit Mf_RUSZ_STB;		//znacznik awarii czujnika STB
    volatile bit Mf_RUSZ_MOD;		//znacznik awarii w obwodzie modulatora 
    volatile bit Mf_RUSZ_CISN;		//znacznik ze cisnienie CO jest poza dozwolonym zakresem  

    volatile bit Mf_RUSZ_NCISN;		//znacznik awarii czujnika cisnienia
    volatile bit Mf_RS_START;		//znacznik rozruchu strtownika po resecie
    volatile bit Mf_PW_START;		//znacznik rozruchu strtownika po zalaczeniu zasilania
    volatile bit Mf_TURBO;			//znacznik rodzaju kotla (0-atmosferyczny, 1- turbo)				
    volatile bit Mf_PRACA;			//znacznik aktywnej proc. ROZRUCH lub aktywnego stanu GRZEJ
    volatile bit Mf_OST_PL;			//znacznik ostatniej proby przy braku plomienia
    volatile bit Mf_RUN_MNMOD;		//praca na minimum modulacji (szybki narost temp. CO)
    volatile bit Mf_KCW_PMP;		//znacznik obiegu CW dla kotla bitermicznego
    volatile bit Mf_DE_VNT;			//znacznik probnego wybiegu wentylatora
    volatile bit Mf_RPM_CW;			//znacznik pracy w obiegu CW dla kotla monotermicznego
    volatile bit Mf_prb_CIG;		//znacznik pewnego braku ciagu
    volatile bit Mf_prb_PRZ;		//znacznik pewnego braku przeplywu WG
    volatile bit Mf_DE_PMP;			//znacznik probnego wybiegu pompy

typedef struct	stpMAIN{
    /*struct{
        unsigned _pob_KCO:1;		//znacznik pobudzenia CO z procedury konfiguracji
        unsigned _pob_KCW:1;		//znacznik pobudzenia CW z procedury konfiguracji
        unsigned _pob_SRW:1;		//znacznik pobudzenia z procedury serwisowej
        unsigned _pob_AL:1;			//znacznik pobudzenia CW od funkcji AL z procedury konfiguracji
        unsigned _pob_CW:1;			//znacznik pobudzenia CW
        unsigned _pob_CO:1;			//znacznik pobudzenia CO 
        unsigned _pob_DzCO:1;		//znacznik pobudzenia od temperatury dyzurnej CO
        unsigned _pob_DzCW:1;		//znacznik pobudzenia od temperatury dyzurnej CW
        unsigned _WYLACZ:1;			//aktywacja procedury wybiegu pompy
        unsigned _ZMKCOCW:1;		//aktywacja proc. odp. za zmiane kierunku obrotow pompy
        unsigned _ROZRUCH:1;		//aktywacja procedury Rozruch
        unsigned _RUSZ_48:1;		//aktywacja procedury Rusz48
        //unsigned _RUSZ_48Z3:1;		//aktywacja procedury Rusz48 dla WU
        unsigned _RUSZ_PWR:1;		//aktywacja procedury RuszPWR
        unsigned _VWYBIEG:1;		//aktywacja wybiegu wentylatora
        unsigned _RUSZ_CIAG:1;		//aktywacja procedury oczekiwania na powrot ciagu  

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
        unsigned _RUSZ_NTC_CO:1;	//znacznik awarii czujnika temp CO
        unsigned _RUSZ_NTC_CW:1;	//znacznik awarii czujnika temp CW
        unsigned _RUSZ_NTC_IN:1;	//znacznik awarii czujnika temp CO (powrot)
        unsigned _RUSZ_STB:1;		//znacznik awarii czujnika STB
        unsigned _RUSZ_MOD:1;		//znacznik awarii w obwodzie modulatora 
        unsigned _RUSZ_CISN:1;		//znacznik ze cisnienie CO jest poza dozwolonym zakresem  

        unsigned _RUSZ_NCISN:1;		//znacznik awarii czujnika cisnienia
        unsigned _RS_START:1;		//znacznik rozruchu strtownika po resecie
        unsigned _PW_START:1;		//znacznik rozruchu strtownika po zalaczeniu zasilania
        unsigned _TURBO:1;			//znacznik rodzaju kotla (0-atmosferyczny, 1- turbo)				
        unsigned _PRACA:1;			//znacznik aktywnej proc. ROZRUCH lub aktywnego stanu GRZEJ
        unsigned _OST_PL:1;			//znacznik ostatniej proby przy braku plomienia
        unsigned _RUN_MNMOD:1;		//praca na minimum modulacji (szybki narost temp. CO)
        unsigned _KCW_PMP:1;		//znacznik obiegu CW dla kotla bitermicznego
        unsigned _DE_VNT:1;			//znacznik probnego wybiegu wentylatora
        unsigned _RPM_CW:1;			//znacznik pracy w obiegu CW dla kotla monotermicznego
        unsigned _prb_CIG:1;		//znacznik pewnego braku ciagu
        unsigned _prb_PRZ:1;		//znacznik pewnego braku przeplywu WG
        unsigned _DE_PMP:1;			//znacznik probnego wybiegu pompy

        unsigned int flags0;
        unsigned int flags1;
        unsigned int flags2;
    };
     */
    volatile unsigned char LICZNIK_PL;	//licznik prob procedury awaryjnej braku plomienia
    volatile unsigned char LICZNIK_CIG;	//licznik ponowien prob braku ciagu (turbo)
    volatile unsigned char LICZNIK_CIG2;	//licznik ponowien prob braku ciagu	(atmosferyczny)
    volatile unsigned char LICZNIK_CIG3;	//licznik ponowien prob braku ciagu
    volatile unsigned char LICZNIK_ANL;	

    volatile unsigned char ERR_BTY;		//aktualny kod awaryjny
    volatile unsigned char ROZ;
    volatile unsigned char R48;
    volatile unsigned char R48Z3;
    volatile unsigned char BL3;
    volatile unsigned char BL15;
    volatile unsigned char WYL;
    volatile unsigned char RPR;
    volatile unsigned char RPL;
    volatile unsigned char RCI;
    volatile unsigned char MNM;
    volatile unsigned char ZKP;
    volatile unsigned char VWB;
    volatile unsigned char SRV;
    volatile unsigned char KRS;
    volatile unsigned char HVZ;
    volatile unsigned char DPR;
}tpMAIN;
/*        
typedef struct	{
//dane otrzymywane z UNI-01
				unsigned char outID;
				unsigned RD_STD:1;				//znacznik odebranej paczki STANDARD
				unsigned RD_KNF:1;				//znacznik odebranej paczki KONFIG
				unsigned RRD_KNF:1;				//blokada na czas odczytu odebranej paczki KONFIG
//tryb normalny
				unsigned char outRDZ0;
				unsigned char outRDZ1;
				unsigned char outCO;
				unsigned char outCW;
				unsigned char outCS;
				unsigned char outIN;
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
//tryb konfiguracji
				unsigned char outKSTAT0;

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
				//unsigned char outTECO;
//dane wysylane do UNI-01
//tryb normalny
				unsigned char inPCO;
				unsigned char inPCW;
				unsigned char inSTAT0;
				unsigned char inSTAT1;
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
				//unsigned char inTECO;
}tpRSDTA1;	
*/
    volatile bit PFNf_off;		//pozycja OFF
    volatile bit PFNf_lato;		//pozycja LATO
    volatile bit PFNf_zima;		//pozycja ZIMA
    volatile bit PFNf_str_nco;	//wskaz nastawy CO po rozruchu
    volatile bit PFNf_str_ncw;	//wskaz nastawy CW po rozruchu
    volatile bit PFNf_b_str_nco;	//wskaz nastawy CO po rozruchu
    volatile bit PFNf_b_str_ncw;	//wskaz nastawy CW po rozruchu
    volatile bit PFNf_set_nco;	//aktywna funkcja zmaiany nastawy CO
    volatile bit PFNf_set_ncw;	//aktywna funkcja zmiany nastawy CW
    volatile bit PFNf_b_set_nco;	
    volatile bit PFNf_b_set_ncw;
    volatile bit PFNf_mod_ncw;
    volatile bit PFNf_set_fnserw;	//aktywna funkcja zmaiany nastawy CO
    volatile bit PFNf_set_aleg;	//aktywna funkcja zmiany nastawy CW	
    volatile bit PFNf_fnserw;		//aktywna funkcja serwisowa
    volatile bit PFNf_fns_max;	//znacznik mocy dla funkcji serwisowej

    volatile bit PFNf_aleg;		//aktywna funkcja antylegionella
    volatile bit PFNf_newPCO;		//znacznik zmiany nastawy PCO
    volatile bit PFNf_newPCW;		//znacznik zmiany nastawy PCW
    volatile bit PFNf_res;		//znacznik nacisniecia off(reset)
    volatile bit PFNf_toInit;
    volatile bit PFNf_KnfDisErr;	//deaktywacja wyswietlania kodu bledu dla wybr. poz. konfiguracji
    volatile bit PFNf_aktywne_KNF;//znacznik wejscia do trybu konfiguracji
    volatile bit PFNf_EnAleg;		//flaga zezwalajaca na aktywacje funkcji antylegionella
    volatile bit PFNf_prn_res;		//wymuszony wskaz cisnienia
    volatile bit PFNf_prn_cs;		//wymuszony wskaz cisnienia
    volatile bit PFNf_prn_in;		//wymuszony wskaz temp. powrotnej CO
    volatile bit PFNf_newPFN;		//nastapila zmana pozycji
    volatile bit PFNf_csn;		//pozycja CISNIENIE
    volatile bit PFNf_prnserw;
    volatile bit PFNf_prnaleg;
    volatile bit PFNf_blkcs;


    volatile bit PFNf_plmCO;  //RSDT1.outSTAT0
    volatile bit PFNf_plmCW;
    volatile bit PFNf_ofnserw;
    volatile bit PFNf_oaleg;	
    volatile bit PFNf_obgCO;
    volatile bit PFNf_obgCW;
    volatile bit PFNf_opKNF;
    volatile bit PFNf_enKNF;
    volatile bit PFNf_tdzCO;  //RSDT1.outSTAT1
    volatile bit PFNf_tdzCW;
    volatile bit PFNf_fmsg;
    volatile bit PFNf_ferr;
    volatile bit PFNf_fL3;
    volatile bit PFNf_pgd;
    volatile bit PFNf_opgres;
    volatile bit PFNf_opgprg;


    volatile bit PFNf_errCS;  //RSDT1.outSTAT2
    volatile bit PFNf_errCO;
    volatile bit PFNf_errCW;
    volatile bit PFNf_fnoCS;
    volatile bit PFNf_ofzima;
    volatile bit PFNf_oflato;
    volatile bit PFNf_ofoff;
    volatile bit PFNf_errIN;  //RSDT1.outSTAT3
    volatile bit PFNf_pmpPWM;
    volatile bit PFNf_trbECO;
//--------
    volatile bit PFNf_b_ofnserw;
    volatile bit PFNf_b_oaleg;

typedef struct	sDataPFN{
    /*struct{
        //flagi ustawiane z konsoli
        unsigned _off:1;		//pozycja OFF
        unsigned _lato:1;		//pozycja LATO
        unsigned _zima:1;		//pozycja ZIMA
        unsigned _str_nco:1;	//wskaz nastawy CO po rozruchu
        unsigned _str_ncw:1;	//wskaz nastawy CW po rozruchu
        unsigned _b_str_nco:1;	//wskaz nastawy CO po rozruchu
        unsigned _b_str_ncw:1;	//wskaz nastawy CW po rozruchu
        unsigned _set_nco:1;	//aktywna funkcja zmaiany nastawy CO
        unsigned _set_ncw:1;	//aktywna funkcja zmiany nastawy CW
        unsigned _b_set_nco:1;	
        unsigned _b_set_ncw:1;
        unsigned _mod_ncw:1;
        unsigned _set_fnserw:1;	//aktywna funkcja zmaiany nastawy CO
        unsigned _set_aleg:1;	//aktywna funkcja zmiany nastawy CW	
        unsigned _fnserw:1;		//aktywna funkcja serwisowa
        unsigned _fns_max:1;	//znacznik mocy dla funkcji serwisowej

        unsigned _aleg:1;		//aktywna funkcja antylegionella
        unsigned _newPCO:1;		//znacznik zmiany nastawy PCO
        unsigned _newPCW:1;		//znacznik zmiany nastawy PCW
        unsigned _res:1;		//znacznik nacisniecia off(reset)
        unsigned _toInit:1;
        unsigned _KnfDisErr:1;	//deaktywacja wyswietlania kodu bledu dla wybr. poz. konfiguracji
        unsigned _aktywne_KNF:1;//znacznik wejscia do trybu konfiguracji
        unsigned _EnAleg:1;		//flaga zezwalajaca na aktywacje funkcji antylegionella
        unsigned _prn_res:1;		//wymuszony wskaz cisnienia
        unsigned _prn_cs:1;		//wymuszony wskaz cisnienia
        unsigned _prn_in:1;		//wymuszony wskaz temp. powrotnej CO
        unsigned _newPFN:1;		//nastapila zmana pozycji
        unsigned _csn:1;		//pozycja CISNIENIE
        unsigned _prnserw:1;
        unsigned _prnaleg:1;
        unsigned _blkcs:1;


        unsigned _plmCO:1;  //RSDT1.outSTAT0
        unsigned _plmCW:1;
        unsigned _ofnserw:1;
        unsigned _oaleg:1;	
        unsigned _obgCO:1;
        unsigned _obgCW:1;
        unsigned _opKNF:1;
        unsigned _enKNF:1;
        unsigned _tdzCO:1;  //RSDT1.outSTAT1
        unsigned _tdzCW:1;
        unsigned _fmsg:1;
        unsigned _ferr:1;
        unsigned _fL3:1;
        unsigned _pgd:1;
        unsigned _opgres:1;
        unsigned _opgprg:1;


        unsigned _errCS:1;  //RSDT1.outSTAT2
        unsigned _errCO:1;
        unsigned _errCW:1;
        unsigned _fnoCS:1;
        unsigned _ofzima:1;
        unsigned _oflato:1;
        unsigned _ofoff:1;
        unsigned _errIN:1;  //RSDT1.outSTAT3
        unsigned _pmpPWM:1;
        unsigned _trbECO:1;
//--------
        unsigned _b_ofnserw:1;
        unsigned _b_oaleg:1;

        unsigned int flags0;
        unsigned int flags1;
        unsigned int flags2;
        unsigned int flags3;                
    };*/

    volatile unsigned char CO;
    volatile unsigned char CW;
    volatile unsigned char CS;
    volatile unsigned char IN;
    volatile unsigned char PPCO;
    volatile unsigned char PPCW;
    volatile unsigned char KAWR;
    volatile unsigned char KERR;
    volatile unsigned int maxPCO;
    volatile unsigned int minPCO;
    volatile unsigned int maxPCW;
    volatile unsigned int minPCW;

    volatile unsigned int PCO;
    volatile unsigned int PCW;
    volatile unsigned char poz;              
}DataPFN;							
//----------------------------------------------------------
#endif /*_MAIN_TPH*/
