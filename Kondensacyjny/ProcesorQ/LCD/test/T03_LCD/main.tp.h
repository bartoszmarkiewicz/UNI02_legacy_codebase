/*main.tp.h*/
//typy danych dla pliku main.c
//----------------------------------------------------------
#ifndef	_MAIN_TPH
#define	_MAIN_TPH
//----------------------------------------------------------
/*				volatile bit Mf_pob_KCO;			//znacznik pobudzenia CO z procedury konfiguracji
				volatile bit Mf_pob_KCW;			//znacznik pobudzenia CW z procedury konfiguracji
				volatile bit Mf_pob_SRW;			//znacznik pobudzenia z procedury serwisowej
				volatile bit Mf_pob_AL;			//znacznik pobudzenia CW od funkcji AL z procedury konfiguracji
				volatile bit Mf_pob_CW;			//znacznik pobudzenia CW
				volatile bit Mf_pob_CO;			//znacznik pobudzenia CO 
				volatile bit Mf_pob_DzCO;			//znacznik pobudzenia od temperatury dyzurnej CO
				volatile bit Mf_pob_DzCW;			//znacznik pobudzenia od temperatury dyzurnej CW

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

				volatile bit Mf_AKTYWNE_SRW;		//znacznik aktywnosci funkcji serwisowej
				volatile bit Mf_AKTYWNE_AL;		//znacznik aktywnosci funkcji antylegionella
				volatile bit Mf_AKTYWNE_PGD;		//znacznik aktywnosci funkcji pogodowej

				volatile bit Mf_RUSZ_NTC_CO;	//znacznik awarii czujnika temp CO
				volatile bit Mf_RUSZ_NTC_CW;	//znacznik awarii czujnika temp CW
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
*/
typedef struct	{
				/*unsigned _pob_KCO:1;			//znacznik pobudzenia CO z procedury konfiguracji
				unsigned _pob_KCW:1;			//znacznik pobudzenia CW z procedury konfiguracji
				unsigned _pob_SRW:1;			//znacznik pobudzenia z procedury serwisowej
				unsigned _pob_AL:1;			//znacznik pobudzenia CW od funkcji AL z procedury konfiguracji
				unsigned _pob_CW:1;			//znacznik pobudzenia CW
				unsigned _pob_CO:1;			//znacznik pobudzenia CO 
				unsigned _pob_DzCO:1;			//znacznik pobudzenia od temperatury dyzurnej CO
				unsigned _pob_DzCW:1;			//znacznik pobudzenia od temperatury dyzurnej CW

				unsigned _WYLACZ:1;			//aktywacja procedury wybiegu pompy
				unsigned _ZMKCOCW:1;		//aktywacja proc. odp. za zmiane kierunku obrotow pompy
				unsigned _ROZRUCH:1;		//aktywacja procedury Rozruch
				unsigned _RUSZ_48:1;		//aktywacja procedury Rusz48
				unsigned _RUSZ_48Z3:1;		//aktywacja procedury Rusz48 dla WU
				unsigned _RUSZ_PWR:1;		//aktywacja procedury RuszPWR
				unsigned _VWYBIEG:1;		//aktywacja wybiegu wentylatora
				unsigned _RUSZ_CIAG:1;		//aktywacja procedury oczekiwania na powrot ciagu
				unsigned _RUSZ_PLOMIEN:1;	//aktywacja procedury awaryjnej przy braku plomienia
				unsigned _RUSZ_RPLM:1;		//aktywacja procedury awaryjnej przy braku plomienia
				unsigned _RUSZ_POMPE:1;		//aktywacja awaryjnego rozruchu pompy
				unsigned _BLOKUJ_3M:1;		//aktywacja procedury blokada 3min dla CO
				unsigned _BLOKUJ_15S:1;		//aktywacja procedury blokada 15sek dla CO
				unsigned _MODULUJ:1;		//aktywacja modulu modulacji

				unsigned _AKTYWNE_SRW:1;		//znacznik aktywnosci funkcji serwisowej
				unsigned _AKTYWNE_AL:1;		//znacznik aktywnosci funkcji antylegionella
				unsigned _AKTYWNE_PGD:1;		//znacznik aktywnosci funkcji pogodowej

				unsigned _RUSZ_NTC_CO:1;	//znacznik awarii czujnika temp CO
				unsigned _RUSZ_NTC_CW:1;	//znacznik awarii czujnika temp CW
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
*/
                
/*				unsigned char LICZNIK_PL;	//licznik prob procedury awaryjnej braku plomienia
				unsigned char LICZNIK_CIG;	//licznik ponowien prob braku ciagu
				unsigned char LICZNIK_ANL;	//licznik ponowien prob braku ciagu

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
 */
				unsigned char PVB;
		}tpMAIN;
				//flagi ustawiane z konsoli
				volatile bit PFNf_off;		//pozycja OFF
				volatile bit PFNf_lato;		//pozycja LATO
				volatile bit PFNf_zima;		//pozycja ZIMA
				volatile bit PFNf_popFN;	//zapamietana pozycja przelacznika funkcji przed przejsciem w stan off     
                
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

				volatile bit PFNf_prn_res;		//wymuszony wskaz cisnienia
				volatile bit PFNf_prn_cs;		//wymuszony wskaz cisnienia
				volatile bit PFNf_prn_in;		//wymuszony wskaz temp. powrotnej CO
				volatile bit PFNf_prn_pp;		//wymuszony wskaz wydatku proc pompy
				volatile bit PFNf_prn_pg;		//wymuszony wskaz wydatky proc modulatora
				volatile bit PFNf_prn_fl;		//wymuszony wskaz przeplywu CW
                
				volatile bit PFNf_set_eco;	//aktywna funkcja zmaiany nastawy ECO
				volatile bit PFNf_b_set_eco;
				volatile bit PFNf_mod_eco;
                
				volatile bit PFNf_b_ofnserw;
				volatile bit PFNf_b_oaleg;

				volatile bit PFNf_fnserw;		//aktywna funkcja serwisowa
				volatile bit PFNf_fns_max;	//znacznik mocy dla funkcji serwisowej
				volatile bit PFNf_aleg;		//aktywna funkcja antylegionella
				volatile bit PFNf_newPCO;		//znacznik zmiany nastawy PCO
				volatile bit PFNf_newPCW;		//znacznik zmiany nastawy PCW
				volatile bit PFNf_newECO;		//znacznik zmiany nastawy ECO
				volatile bit PFNf_res;		//znacznik nacisniecia off(reset)
				volatile bit PFNf_toInit;
				volatile bit PFNf_KnfDisErr;	//deaktywacja wyswietlania kodu bledu dla wybr. poz. konfiguracji
				volatile bit PFNf_aktywne_KNF;//znacznik wejscia do trybu konfiguracji
				volatile bit PFNf_EnAleg;		//flaga zezwalajaca na aktywacje funkcji antylegionella
				volatile bit PFNf_KnfReloadPar;	//flaga aktualizacji wartosci parametru                
                
                volatile bit PFNf_EnLIN;              //znacznik zezwolenia na odpowiedzi na zapytanie ze strony MASTER                
                volatile bit PFNf_fsetLaddr;          //flaga aktywnej funkcji przypisania adresu LIN
                volatile bit PFNf_fshowLaddr;         //flaga aktywnej funkcji wyswietlania adresu LIN (autowygaszenie)
                volatile bit PFNf_fshowLpulse;        //flaga aktywnej funkcji pulsacji podswietlenia wyswietlacza
                volatile bit PFNf_foneLpulse;         //flaga aktywnej funkcji autowygaszenia pulsacji podswietlenia wyswietlacza gdy _fshowLpulse=1
                volatile bit PFNf_setKey;             //znacznik podtrzymania przycisku dla punkcji warunkowego przypisania adresu LIN
                
                volatile bit PFNf_fsetLTrb;           //tryb nadawania adresu (0-potwierdzany przyciskiem, 1-automatyczny)  //nnn
                volatile bit PFNf_fIgnore;            //ignoruj wszystkie komendy nadawania adresu  //nnn
                volatile bit PFNf_fProcess;           //trwa proces przypisywania adresu    //nnn
                volatile bit PFNf_fOne;               //ignoruj wszystkie adresy inne niz 1 //nnn
                
				volatile bit PFNf_LINres;             //znacznik zdalnego nacisniecia off(reset)
                volatile bit PFNf_LINresSR;           //zdalne wylaczenie funkcji serwisowej
                volatile bit PFNf_LINresAL;           //zdalne wylaczenie funkcji antylegionella  
                volatile bit PFNf_FLIN;               //aktywnosc interfejsu LIN                
                volatile bit PFNf_LIN_RT;             //zdalne wylaczenie pobudzenia RT
                volatile bit PFNf_LIN_CW;             //zdalne wylaczenie pobudzenia timera zasobnika                

                volatile bit PFNf_LIN_CK;              //znacznik nadejscia komendy cyklicznej 
                volatile bit PFNf_LIN_EPBCO;           //znacznik waznosci stanu PBCO                
                volatile bit PFNf_LIN_PBCO;            //pobudzenie dla obiegu CO
                volatile bit PFNf_LIN_EPBCW;           //znacznik waznosci stanu PBCW               
                volatile bit PFNf_LIN_PBCW;            //pobudzenie dla obiegu CW  
                volatile bit PFNf_LIN_KSK;             //znacznik pracy w trybie kaskady
                volatile bit PFNf_LIN_STR;             //znacznik pracy w trybie sterowania z managera strefy lub regulatora RT
                volatile bit PFNf_LIN_DPGD;            //znacznik dezaktywacji wewnetrznego regulatora pogodowego
                volatile bit PFNf_LIN_ENDO;            //znacznik dezaktywacji procedury odpowietrzajacej
                volatile bit PFNf_LIN_PWD;             //znacznik pracy z przewyzszeniem
                volatile bit PFNf_LIN_BUF;             //znacznik pracy z buforem ciepla
                volatile bit PFNf_LIN_UP;              //pobudzenie grzania na wysokich parametrach                 
                volatile bit PFNf_LIN_BPSS;            //bypass dla pobudzenia RT //+++
                
                volatile bit PFNf_bEnF10V;             //znacznik do aktywacji wejscia 10V              
                volatile bit PFNf_EnF10V;              //znacznik aktywnosci wejscia 10V  
                volatile bit PFNf_b10V_RT;             //znacznik do aktywacji pobudzenia z wejscia 10V                
                volatile bit PFNf_10V_RT;              //znacznik pobudzenia z wejscia 10V   
                volatile bit PFNf_10V_nPCO;            //znacznik nowej wartosci PCO z wejscia 10V                
                volatile bit PFNf_10VRT;                //pobudzenie RT interfejsu 10V
                
                volatile bit PFNf_FOT;                  //aktywnosc interfejsu OT                
                volatile bit PFNf_OT_DPGD;              //dezaktywacja wewnetrznego regulatora pogodowego
                volatile bit PFNf_OTRT;                 //pobudzenie RT interfejsu OT
                
                volatile bit PFNf_ex_pgd;               //znacznik aktywnosci zewnetrznego regulatora pogodowego
                
                volatile bit PFNf_bfRT;                 //pamiec pobudzenia CO z interfejsow 10V,OT,LIN
                
                volatile bit PFNf_new_IPCW;             //flaga nowej nastawy IPCW z interfejsu OT  //jjj
                volatile bit PFNf_new_IPCO;             //flaga nowej nastawy IPCO z interfejsu OT
                
                volatile bit PFNf_LKnfDisErr;	//deaktywacja wyswietlania kodu bledu dla wybr. poz. konfiguracji  //kkk
				volatile bit PFNf_aktywne_LKNF;//znacznik wejscia do trybu konfiguracji  //kkk
				volatile bit PFNf_LKnfReloadPar;	//flaga aktualizacji wartosci parametru   //kkk
                
				//flagi ustawiane zdalnie przez uk³ad UNI-01 (po RSie)

//RSDT1.outSTAT0
				volatile bit PFNf_plmCO;
				volatile bit PFNf_plmCW;
				volatile bit PFNf_ofnserw;
				volatile bit PFNf_oaleg;	
				volatile bit PFNf_obgCO;
				volatile bit PFNf_obgCW;
				volatile bit PFNf_opKNF;
				volatile bit PFNf_enKNF;

//RSDT1.outSTAT1
				volatile bit PFNf_tdzCO;
				volatile bit PFNf_tdzCW;
				volatile bit PFNf_fmsg;
				volatile bit PFNf_ferr;
				volatile bit PFNf_fL3;
				volatile bit PFNf_pgd;
				volatile bit PFNf_opgres;
				volatile bit PFNf_opgprg;

//RSDT1.outSTAT2
				volatile bit PFNf_errCS;
				volatile bit PFNf_errCO;
				volatile bit PFNf_errCW;
				volatile bit PFNf_fnoCS;
				volatile bit PFNf_ofzima;
				volatile bit PFNf_oflato;
				volatile bit PFNf_ofoff;
				volatile bit PFNf_kalVNT;

//RSDT1.outSTAT3
				volatile bit PFNf_zcfm;
				volatile bit PFNf_vnt;
				volatile bit PFNf_rozruch;
				volatile bit PFNf_errIN;
				volatile bit PFNf_pmpPWM;
				volatile bit PFNf_trbECO;
				volatile bit PFNf_timZAS;
				volatile bit PFNf_pobRT;   
                
//RSDT1.outSTAT4                
                volatile bit PFNf_prODP;
				volatile bit PFNf_tmpZW;  
				volatile bit PFNf_pgdTP; 
                
				volatile bit PFNf_pmpON;                

//RSDT1.outSTAT5  
                volatile bit PFNf_stkRT;                
                volatile bit PFNf_pobPGRT;                
                volatile bit PFNf_fzaREG;                
                volatile bit PFNf_fenPWD;              

//RSDT1.outKSTAT0                
                volatile bit PFNf_cwIMP;
                volatile bit PFNf_PWD;    
                volatile bit PFNf_BUF;   
                volatile bit PFNf_cwUP;                
                volatile bit PFNf_BPSS;  //+++
                volatile bit PFNf_outRT;    //+++                  
                
typedef struct	{
				//flagi ustawiane z konsoli
/*				unsigned _off:1;		//pozycja OFF
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

				unsigned _prn_res:1;		//wymuszony wskaz cisnienia
				unsigned _prn_cs:1;		//wymuszony wskaz cisnienia
				unsigned _prn_in:1;		//wymuszony wskaz temp. powrotnej CO
				unsigned _prn_pp:1;		//wymuszony wskaz wydatku proc pompy
				unsigned _prn_pg:1;		//wymuszony wskaz wydatky proc modulatora

				unsigned _set_eco:1;	//aktywna funkcja zmaiany nastawy ECO
				unsigned _b_set_eco:1;
				unsigned _mod_eco:1;
                
				unsigned _b_ofnserw:1;
				unsigned _b_oaleg:1;

				unsigned _fnserw:1;		//aktywna funkcja serwisowa
				unsigned _fns_max:1;	//znacznik mocy dla funkcji serwisowej
				unsigned _aleg:1;		//aktywna funkcja antylegionella
				unsigned _newPCO:1;		//znacznik zmiany nastawy PCO
				unsigned _newPCW:1;		//znacznik zmiany nastawy PCW
				unsigned _newECO:1;		//znacznik zmiany nastawy ECO
				unsigned _res:1;		//znacznik nacisniecia off(reset)
				unsigned _toInit:1;
				unsigned _KnfDisErr:1;	//deaktywacja wyswietlania kodu bledu dla wybr. poz. konfiguracji
				unsigned _aktywne_KNF:1;//znacznik wejscia do trybu konfiguracji
				unsigned _EnAleg:1;		//flaga zezwalajaca na aktywacje funkcji antylegionella
                
                unsigned _EnLIN:1;              //znacznik zezwolenia na odpowiedzi na zapytanie ze strony MASTER                
                unsigned _fsetLaddr:1;          //flaga aktywnej funkcji przypisania adresu LIN
                unsigned _fshowLaddr:1;         //flaga aktywnej funkcji wyswietlania adresu LIN (autowygaszenie)
                unsigned _fshowLpulse:1;        //flaga aktywnej funkcji pulsacji podswietlenia wyswietlacza
                unsigned _foneLpulse:1;         //flaga aktywnej funkcji autowygaszenia pulsacji podswietlenia wyswietlacza gdy _fshowLpulse=1
                unsigned _setKey:1;             //znacznik podtrzymania przycisku dla punkcji warunkowego przypisania adresu LIN
				//flagi ustawiane zdalnie przez uk³ad UNI-01 (po RSie)

//RSDT1.outSTAT0
				unsigned _plmCO:1;
				unsigned _plmCW:1;
				unsigned _ofnserw:1;
				unsigned _oaleg:1;	
				unsigned _obgCO:1;
				unsigned _obgCW:1;
				unsigned _opKNF:1;
				unsigned _enKNF:1;

//RSDT1.outSTAT1
				unsigned _tdzCO:1;
				unsigned _tdzCW:1;
				unsigned _fmsg:1;
				unsigned _ferr:1;
				unsigned _fL3:1;
				unsigned _pgd:1;
				unsigned _opgres:1;
				unsigned _opgprg:1;

//RSDT1.outSTAT2
				unsigned _errCS:1;
				unsigned _errCO:1;
				unsigned _errCW:1;
				unsigned _fnoCS:1;
				unsigned _ofzima:1;
				unsigned _oflato:1;
				unsigned _ofoff:1;
				unsigned _kalVNT:1;

//RSDT1.outSTAT3
				unsigned _zcfm:1;
				unsigned _vnt:1;
				unsigned _rozruch:1;
				unsigned _errIN:1;
				unsigned _pmpPWM:1;
				unsigned _trbECO:1;
				unsigned _timZAS:1;
*/
    union   {
            unsigned char EXFLG;        //uwaga flagi zerowane po zaniku komunikacji LIN2
            struct
            {  
                unsigned _FPWD:1;      //znacznik trybu pracy ze stabilizacja temperatury (sygnal zwrotny do sterownika zewnetrznego)                
                unsigned _FBUF:1;      //znacznik trybu pracy na bufor ciepla (sygnal zwrotny do sterownika zewnetrznego)
                unsigned _FZAS:1;      //znacznik trybu pracy na wysokich parametrach (ustawiany przez sterownik zewnetrzny)
                unsigned _FUP:1;       //znacznik aktywnej pracy na wysokich parametrach (sygnal zwrotny do sterownika zewnetrznego)    
                unsigned _FBPSS:1;     //znacznik bypass dla pobudzenia RT  //+++                
                unsigned _FOUTRT:1;    //pobudzenie RT generowane przez plytke sterujaca na potzreby bypass   //+++          
                unsigned _exbt6:1;     
                unsigned _exbt7:1;                
            }; 
    }; 
    union   {
            unsigned char STFLG;        //uwaga flagi zerowane po zaniku komunikacji LIN2
            struct
            {  
                unsigned _FFPWD:1;      //znacznik trybu pracy ze stabilizacja temperatury (sygnal zwrotny do sterownika zewnetrznego)                
                unsigned _FFBUF:1;      //znacznik trybu pracy na bufor ciepla (sygnal zwrotny do sterownika zewnetrznego)
                unsigned _FFZAS:1;      //znacznik trybu pracy na wysokich parametrach (ustawiany przez sterownik zewnetrzny)
                unsigned _FFUP:1;       //znacznik aktywnej pracy na wysokich parametrach (sygnal zwrotny do sterownika zewnetrznego)  
                unsigned _FTRB:2;                               
                unsigned _FRDZ:2;                               
            }; 
    };    
				//wartosci ustawiane zdalnie przez uk³ad UNI-02 (po RSie)
				unsigned char RDZ0;		//kod sterownika
				unsigned char RDZ1;		//starsze 4bity = czesc A wersji oprogramowania (wersja=A+B), mlodsze 4bity = typ kot³a (_UNICO=0,_MONOHB=3)
				unsigned char RDZ2;		//mlodsze 5bitow = czesc B wersji oprogramowania (wersja=A+B)               
				unsigned char CO;
				unsigned char CW;
				unsigned char CS;
				unsigned char IN;
				unsigned char PP;
				unsigned char PG;
				unsigned char ZW;
				unsigned char PRZ;
                
				unsigned char VNT;
				unsigned char PPCO;
				unsigned char PPCW;
				unsigned char KAWR;
				unsigned char KERR;
				unsigned int maxPCO;
				unsigned int minPCO;
				unsigned int maxPCW;
				unsigned int minPCW;

                //unsigned char EXFLG;
                
				unsigned char vUNI02;	//wersja oprogramowania UNI-02
				unsigned char ALIN;	//adres LIN plyty sterujacej

				unsigned int PCO;
				unsigned int PCW;
				unsigned int ECO;
                
				unsigned char rCO;
				unsigned char rCW;                

				unsigned char KRS;
                
                unsigned char PCO10V;
                unsigned char bPCO10V;   
                unsigned char CONTSETP;  
                unsigned char INSETP;
                unsigned char INSETPP;                
                unsigned char SETPmin;
                unsigned char SETPmax;
                unsigned char SETPPmax;
                
                unsigned char IINSETP; //jjj
                unsigned int IPCO;
                unsigned int IPCW;
                
                unsigned char STRB;
				}DataPFN;
/*
typedef struct	{
//parametry napiecia 5V MMY
			unsigned LoVMMY5:1;				//zbyt niskie napiecie
			unsigned HiVMMY5:1;				//za wysokie napiecie
			unsigned StanVMMY5:1;
//parametry napiecia 18V MMY
			unsigned LoVMMY18:1;			//zbyt niskie napiecie
			unsigned HiVMMY18:1;			//za wysokie napiecie
			unsigned StanVMMY18:1;
//parametry napiecia VRT MMY
			unsigned LoVRT:1;				//zbyt niskie napiecie
			unsigned HiVRT:1;				//za wysokie napiecie
			unsigned StanVRT:1;
//stan przyciskow konsolety
			unsigned kStart:1;				//stan przycisku START (przed rozpoczeciem testu)
			unsigned kNext:1;				//stan przycisku START (po rozpoczeciu testu)
			unsigned kReset:1;				//stan przycisku RESET
//stany sygnalow wejsciowych
			unsigned wPmp:1;				//aktywny sygnal pompy
			unsigned wPmpCO:1;				//aktywny sygnal pompy kier. CO
			unsigned wPmpCW:1;				//aktywny sygnal pompy kier. CW
			unsigned wVntPmp:1;				//aktywny sygnal wentylatora
			unsigned wZGaz:1;				//aktywny sygnal zaworu gazowego

			unsigned char AC_VMMY5;
			unsigned char AC_VMMY18;
			unsigned char VMMY5;			//wartosc napiecia 5V MMY			
			unsigned char VMMY18;			//wartosc napiecia 18V MMY
			unsigned char VRT;				//wartosc HEX napiecia VRT MMY	
*/	
/*			unsigned char MS1[28];
			unsigned char MS2[28];
			unsigned char TPK;
			unsigned char Proc;
			unsigned char Op;

			unsigned char Rodzina;
			unsigned char TrybPracy;		//tryb pracy automatu
			
			unsigned char minPCO;
			unsigned char maxPCO;
			unsigned char minPCW;
			unsigned char maxPCW;
			unsigned char bf;
			unsigned _fl:1;	

			unsigned _PWR_ON:1;				//wlaczono zasilanie
			unsigned _NO_AMMY1:1;			//brak komunikacji z modulem AMMY1
			unsigned _NO_AMMY2:1;			//brak komunikacji z modulem AMMY2
			unsigned _NO_AMMY3:1;			//brak komunikacji z modulem AMMY3
			unsigned _NO_STR:1;				//brak komunikacji z modulem MMY
			unsigned _FL_NO_STR:1;			//brak komunikacji z modulem MMY stan filtrowany

			unsigned _STAB:1;				//stan po odczekaniu stanow nieustalonych			
			unsigned _AUTO:1;				//znacznik pracy auto
			unsigned _POWER:1;				//znacznik zalaczonego zasilania w sterowniku
			unsigned _FAZA:1;				//znacznik fazy na zasilania w sterowniku

			unsigned _Turbo:1;				//znacznik sterownika typu turbo
			unsigned _Biterm:1;				//znacznik ukladu bitermicznego
			unsigned _cz_PrzCO:1;			//znacznik kotlow z czujnikiem przeplywu CO
			unsigned _TEST:1;

			unsigned char OSTR;
			unsigned char NRTSTU;
			unsigned char TST;
			unsigned char WSW;

		}tpAMMY;*/
                
				volatile bit RSDT1f_RD_STD;				//znacznik odebranej paczki STANDARD
				volatile bit RSDT1f_RRD_STD;				//blokada na czas odczytu odebranej paczki STANDARD
				volatile bit RSDT1f_RD_KNF;				//znacznik odebranej paczki KONFIG
				volatile bit RSDT1f_RRD_KNF;				//blokada na czas odczytu odebranej paczki KONFIG
                volatile bit RSDT1f_RD_LIN;                              //znacznik odebranej paczki na zapytanie LIN
                volatile bit RSDT1f_NoRespToLIN;                            //znacznik nowej komendy LIN dla UNI-02
                volatile bit RSDT1f_NewLIDN;                            //znacznik nowej komendy LIN dla UNI-02
                volatile bit RSDT1f_OdpLIDN;                            //znacznik nowej komendy LIN dla UNI-02
                volatile bit RSDT1f_RespLIDN;           

                volatile bit MOD1f_RD_KNF;     //kkk                
                volatile bit MOD1f_RRD_KNF;     //kkk
typedef struct	{
//dane otrzymywane z UNI-02

/*				unsigned _RD_STD:1;				//znacznik odebranej paczki STANDARD
				unsigned _RRD_STD:1;				//blokada na czas odczytu odebranej paczki STANDARD
				unsigned _RD_KNF:1;				//znacznik odebranej paczki KONFIG
				unsigned _RRD_KNF:1;				//blokada na czas odczytu odebranej paczki KONFIG
                unsigned _RD_LIN:1;                              //znacznik odebranej paczki na zapytanie LIN
                unsigned _NoRespToLIN:1;                            //znacznik nowej komendy LIN dla UNI-02
                unsigned _NewLIDN:1;                            //znacznik nowej komendy LIN dla UNI-02
                unsigned _OdpLIDN:1;                            //znacznik nowej komendy LIN dla UNI-02
                unsigned _RespLIDN:1;                            //znacznik nowej komendy LIN dla UNI-02
*/                
				unsigned char outID;                
//tryb normalny
				unsigned char vSTD;				//wersja paczki danych standardowych               
				unsigned char outRDZ0;
				unsigned char outRDZ1;
				unsigned char outRDZ2;                
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
				unsigned char outSETPPmax;   
				unsigned char outPRZ;      
                
//tryb konfiguracji
				unsigned char vKNF;				//wersja paczki danych konfiguracji
				unsigned char outKSTAT0;
				unsigned char outKRK;
				unsigned char outPRM;
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
                
                unsigned char outPWTM;                

                unsigned char outTPCW;
                unsigned char outTMPM;
                
                unsigned char outLIDN; //komenda zwrotna LIN odpowiedzi UNI-02
                unsigned char outLDTA1;//parametr1 odpowiedzi UNI-02 na komende LIN
                unsigned char outLDTA2;//parametr2 odpowiedzi UNI-02 na komende LIN
                unsigned char outLDTA3;//parametr3 odpowiedzi UNI-02 na komende LIN
                unsigned char outLDTA4;//parametr4 odpowiedzi UNI-02 na komende LIN
                unsigned char outLDTA5;//parametr5 odpowiedzi UNI-02 na komende LIN                

//dane wysylane do UNI-02
//tryb normalny
				unsigned char inPCO;
				unsigned char inPCW;
				unsigned char inECO;
				unsigned char inSTAT0;
				unsigned char inSTAT1;
				unsigned char inINSTAT;                
				unsigned char inLNSTAT;  
				unsigned char inCONTSETP;   
				unsigned char inSTAT2; 
				unsigned char inLNSTAT2;      //+++           
                
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
                unsigned char inPWTM; 
                
                unsigned char inTPCW;
                unsigned char inTMPM;
                
//komunikaty z LIN
                unsigned char inLIDN; //komenda LIN dla  UNI-02
                unsigned char inLDTA1;//parametr1 komendy LIN dla  UNI-02
                unsigned char inLDTA2;//parametr2 komendy LIN dla  UNI-02
                unsigned char inLDTA3;//parametr3 komendy LIN dla  UNI-02
                unsigned char inLDTA4;//parametr4 komendy LIN dla  UNI-02
                unsigned char ACKUNI;
}tpRSDTA1;	

typedef struct	{ 
   
//    union   {
//            unsigned char flag0;
//            struct
//            {                  
//                unsigned _Init:1;       //flaga oczekiwania na inicjacje polaczenia (wyzerwana wskazuje ze logika komfort moze podjac prace na zdalny styk)
//                unsigned _Init2:1;      //znacznik wstepnej aktualizacji danych
//				unsigned _extPGD:1;     //flaga aktywnosci regulatora pogodowego systemu cieplnego                    
//                unsigned _enPWD:1;      //znacznik gotowosci pracy z przewyzszeniem  
//                unsigned _regCO:1;      //znacznik pracy z regulacja z przewyzszeniem   
//                unsigned _autoCO:1;     //znacznik pracy z regulacja z przewyzszeniem w trybie samodzielnym
//                unsigned _fzaREG:1;     //regulacja w fazie stabilizacji
//                unsigned _NoLIN:1;      //znacznik pracy z systemem cieplnym bez po???rednictwa interfejsu LIN               
//            };
//    };   
//    union   {
//            unsigned char flag1;
//            struct
//            {   
//                unsigned _fl1bt0:1;     //     
//                unsigned _tp1F:1;       //tryb pracy na zasobnik WU
//                unsigned _enCO_OUT:1;   //aktywny czujnik CO_OUT
//                unsigned _enCW_BOIL:1;  //aktywny czujnik zasobnika
//                unsigned _enCO_BOIL:1;  //aktywny czujnik bufora ciepla
//                unsigned _fl1bt5:1;    //
//                unsigned _newDTA1:1;    //
//                unsigned _newDTA2:1;    //znacznik aktualizacji danych podstawowych                                               
//            }; 
//    };  
//    union   {
//            unsigned char flag2;
//            struct
//            {   
//                unsigned _enKCOCW:1;    //flaga aktywnosci funkcji powielenia sygnalu zaworu 3Dr kotla  
//                unsigned _enPMP_CO:1;   //flaga aktywnosci funkcji powielenia sygnalu pompy CO kotla
//                unsigned _enPMP_PNZ:1;  //flaga aktywnosci funkcji obslugi pionu wysokotemperaturowego
//                unsigned _enPMP_RC:1;   //flaga aktywnosci funkcji obslugi  obiegu cyrkulacji
//                unsigned _enTMP_RC:1;   //flaga aktywnosci temperaturowego sterowania obiegiem cyrkulacji
//                unsigned _enREG_PWZ:1;  //flaga aktywnosci regulacji temperatury za sprzeglem       
//                unsigned _newDTA11:1;    //
//                unsigned _newDTA22:1;    //znacznik aktualizacji danych podstawowych                              
//            }; 
//    };   
    union   {
            unsigned char flag3;        //uwaga flagi zerowane po zaniku komunikacji LIN2
            struct
            {  
                unsigned _TRB_WZ:2;      //                 
                unsigned _enWZ:1;        //praca na wezel zasilania
                unsigned _blRC:1;        //stan blokady pompy cyrkulacyjnej
                unsigned _enCMF:1;       //znacznik pracy modulu w trybie komfortu    
                unsigned _ExBLK:1;       //znacznik aktywnosci blokady grzania dla gazowego zrodla ciepla              
                unsigned _pobExCO:1;     //pobudzenie dla zrodla ciepla niezalezne od blokady              
                unsigned _pobUP:1;       //pobudzenie grzania na wysokich parametrach                               
            }; 
    }; 
    union   {
            unsigned char flag4;        //uwaga flagi zerowane po zaniku komunikacji LIN2
            struct
            {   
                unsigned _enPZ1:1;      //flaga aktywnosci pionu zasilania 1
                unsigned _enPZ2:1;      //flaga aktywnosci pionu zasilania 2                         
                unsigned _enPZ3:1;      //flaga aktywnosci pionu zasilania 3
                unsigned _enPZ4:1;      //flaga aktywnosci pionu zasilania 4                  
                unsigned _enPZ5:1;      //flaga aktywnosci pionu zasilania 5
                unsigned _enPZ6:1;      //flaga aktywnosci pionu zasilania 6                         
                unsigned _pobWZ:1;      //pobudzenie z wezla zasilania
                unsigned _enWZS:1;       //aktywnosc wezla zasilania                                
            }; 
    };
    union   {
            unsigned char flag5;        //uwaga flagi zerowane po zaniku komunikacji LIN2
            struct
            {   
                unsigned _gpob1:1;    //flaga pobudzenia posesji1  
                unsigned _gpob2:1;    //flaga pobudzenia posesji2
                unsigned _gpob3:1;    //flaga pobudzenia posesji3
                unsigned _gpob4:1;    //flaga pobudzenia posesji4
                unsigned _gpob5:1;    //flaga pobudzenia posesji5
                unsigned _gpob6:1;    //flaga pobudzenia posesji6       
                unsigned _fl5bt6:1;   //      
                unsigned _setBPSS:1;     //bypass dla pobudzenia RT //+++
            }; 
    };    
    union   {
            unsigned char flag6;        //uwaga flagi zerowane po zaniku komunikacji LIN2
            struct
            {   
                unsigned _spob1:1;    //flaga pobudzenia STR1  
                unsigned _spob2:1;    //flaga pobudzenia STR2
                unsigned _spob3:1;    //flaga pobudzenia STR3
                unsigned _spob4:1;    //flaga pobudzenia STR4
                unsigned _spob5:1;    //flaga pobudzenia STR5
                unsigned _spob6:1;    //flaga pobudzenia STR6       
                unsigned _ndt1:2;          //zarezerwowane                                
            }; 
    };     
    union   {
            unsigned char flag7;        //uwaga flagi zerowane po zaniku komunikacji LIN2
            struct
            {   
                unsigned _ppob1:1;    //flaga pobudzenia PZ1  
                unsigned _ppob2:1;    //flaga pobudzenia PZ2
                unsigned _ppob3:1;    //flaga pobudzenia PZ3
                unsigned _ppob4:1;    //flaga pobudzenia PZ4
                unsigned _ppob5:1;    //flaga pobudzenia PZ5
                unsigned _ppob6:1;    //flaga pobudzenia PZ6       
                unsigned _ndt2:2;          //zarezerwowane                                
            }; 
    };   
    union   {
            unsigned char flag8;
            struct
            {   
                unsigned _newSPOB:1;     //nowy stan pobudzen ze stref                
                unsigned _newPPOB:1;     //nowy stan pobudzen z pionow zasilania
                unsigned _newGPOB:1;     //nowy stan pobudzen z posesji   
                unsigned _newSTAT:1;     //nowy stan statusu modulu
                unsigned _newENPNZ:1;    //nowy stan aktywnosci pionow zasilania 
                unsigned _ID_Pulse:1;    //flaga dla wizualnej identyfikacji modulu (diody)               
                unsigned _NO_MOD:1;      //stwierdzony stan braku modulu nr 1 przy zakluczowaniu pozycji modulu na nr 2
                unsigned _pwrON:1;      //                              
            }; 
    };       
//    union   {
//            unsigned char flag9;
//            struct
//            {   
//                unsigned _refTIM:1;     //flaga aktualizacji sygnalu czasu
//                unsigned _refSTR1:1;    //flaga aktyalizacji danych pionu zasilania                         
//                unsigned _refSTR2:1;    //flaga aktyalizacji danych pionu zasilania
//                unsigned _refSTR3:1;    //flaga aktyalizacji danych pionu zasilania                 
//                unsigned _refSTR4:1;    //flaga aktyalizacji danych pionu zasilania
//                unsigned _refSTR5:1;    //flaga aktyalizacji danych pionu zasilania                         
//                unsigned _refSTR6:1;    //flaga aktyalizacji danych pionu zasilania
//                unsigned _fl9bt7:1;     //                              
//            };             
//    };      
    union   {
            unsigned char flag10;
            struct
            {   
                unsigned _enPNZ1:1;    //aktywnosc pionu zasilania 1
                unsigned _enPNZ2:1;    //aktywnosc pionu zasilania 2
                unsigned _enPNZ3:1;    //aktywnosc pionu zasilania 3
                unsigned _enPNZ4:1;    //aktywnosc pionu zasilania 4
                unsigned _enPNZ5:1;    //aktywnosc pionu zasilania 5
                unsigned _enPNZ6:1;    //aktywnosc pionu zasilania 6               
                unsigned _fl3bt6:1;    //flaga aktywnosci regulacji temperatury za sprzeglem       
                unsigned _fl3bt7:1;    //flaga wspolpracy z urzadzeniem grzewczym z interfejsem LIN                                
            }; 
    };  
//    union   {
//            unsigned char flag11;
//            struct
//            {    
//                unsigned _errZS_CO:1;      //awaria/brak czujnika CO
//                unsigned _errZS_IN:1;      //awaria/brak czujnika IN                
//                unsigned _errZS_CW:1;      //awaria/brak czujnika CW      
//                unsigned _errCS:1;      //awaria/brak czujnika CS 
//                unsigned _plmCO:1;      //stan plomien CO      	                
//                unsigned _plmCW:1;      //stan plomien CW						
//                unsigned _pgd:1;        //aktywny regulator pogodowy systemu cieplnego	
//                unsigned _tzw:1;        //otrzymano z systemu cieplnego wartosc temperatury zewnetrznej
//            };
//    };    
//    union   {
//            unsigned char flag12;
//            struct
//            {    
//                unsigned _BLK_CO:1;      //stan blokady grzania (L3)
//                unsigned _errTIN:1;       //zbiorcza wartosc _errIN 
//                unsigned _refTIN:1;       //flaga aktualizacji zbiorczej wartosci IN               
//                unsigned _refTCO:1;       //flaga aktualizacji zbiorczej wartosci CO					
//                unsigned _refTCW:1;       //flaga aktualizacji zbiorczej wartosci CW                
//                unsigned _refCS:1;       //flaga aktualizacji zbiorczej wartosci CS					
//                unsigned _refSETP:1;     //flaga aktualizacji zbiorczej wartosci SETP	
//                unsigned _refPG:1;       //flaga aktualizacji zbiorczej wartosci PG
//            };
//    }; 
    union   {
            unsigned char flag13;        //uwaga flagi zerowane po zaniku komunikacji LIN2
            struct
            {    
                unsigned _TPROD:1;       //flaga testu produkcyjnego 
                unsigned _fl13bt1:1;       //
                unsigned _fl13bt2:1;       //               
                unsigned _fl13bt3:1;       //					
                unsigned _fl13bt4:1;       //                
                unsigned _fl13bt5:1;       //				
                unsigned _fl13bt6:1;     //
                unsigned _fl13bt7:1;       //
            };
    };    
}DataKFN;   //parametry sterujace calego urzadzenia -kaskady, managera strefy itp...
//----------------------------------------------------------
#endif /*_MAIN_TPH*/
