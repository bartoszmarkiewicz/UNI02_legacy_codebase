/*main.tp.h*/
//typy danych dla pliku main.c
//----------------------------------------------------------
typedef struct	{
        union   {
            unsigned char flag0;
            struct
            {    
				unsigned _pob_KCO:1;			//znacznik pobudzenia CO z procedury konfiguracji
				unsigned _pob_KCW:1;			//znacznik pobudzenia CW z procedury konfiguracji
				unsigned _pob_KAL:1;			//znacznik pobudzenia CW od funkcji AL z procedury konfiguracji
				unsigned _pob_SRW:1;			//znacznik pobudzenia z procedury serwisowej
				unsigned _pob_CW:1;			//znacznik pobudzenia CW
				unsigned _pob_CO:1;			//znacznik pobudzenia CO 
				unsigned _pob_DzCO:1;			//znacznik pobudzenia od temperatury dyzurnej CO
				unsigned _pob_DzCW:1;			//znacznik pobudzenia od temperatury dyzurnej CW
            };
        };    
        union   {
            unsigned char flag1;
            struct
            {
				unsigned _WYLACZ:1;			//aktywacja procedury wybiegu pompy
				unsigned _ZMKCOCW:1;		//aktywacja proc. odp. za zmiane kierunku obrotow pompy
				unsigned _ROZRUCH:1;		//aktywacja procedury Rozruch
				unsigned _RUSZ_48:1;		//aktywacja procedury Rusz48
				unsigned _RUSZ_PWR:1;		//aktywacja procedury RuszPWR
				unsigned _VWYBIEG:1;		//aktywacja wybiegu wentylatora
				unsigned _RUSZ_CIAG:1;		//aktywacja procedury oczekiwania na powrot ciagu
				unsigned _RUSZ_PLOMIEN:1;	//aktywacja procedury awaryjnej przy braku plomienia
            };
        };
        union   {
            unsigned char flag2;
            struct
            {                
				unsigned _RUSZ_RPLM:1;		//aktywacja procedury awaryjnej przy braku plomienia
				unsigned _RUSZ_POMPE:1;		//aktywacja awaryjnego rozruchu pompy
				unsigned _BLOKUJ_3M:1;		//aktywacja procedury blokada 3min dla CO
				unsigned _BLOKUJ_15S:1;		//aktywacja procedury blokada 15sek dla CO
				unsigned _MODULUJ:1;		//aktywacja modulu modulacji
				unsigned _ZAPLON:1;			//znacznik ze nastapil zaplon gazu
				unsigned _AKTYWNE_SRW:1;		//znacznik aktywnosci funkcji serwisowej
				unsigned _AKTYWNE_PGD:1;		//znacznik aktywnosci funkcji pogodowej
            };
        };  
        union   {
            unsigned char flag3;
            struct
            {                 
				unsigned _AKTYWNE_KNF:1;		//znacznik aktywnosci procedury konfiguracji
				unsigned _RUSZ_NTC_CO:1;	//znacznik awarii czujnika temp CO
				unsigned _RUSZ_NTC_CW:1;	//znacznik awarii czujnika temp CW
				unsigned _RUSZ_STB:1;		//znacznik awarii czujnika STB
				unsigned _RUSZ_MOD:1;		//znacznik awarii w obwodzie modulatora 
				unsigned _RUSZ_CISN:1;		//znacznik ze cisnienie CO jest poza dozwolonym zakresem 
				unsigned _RUSZ_NCISN:1;		//znacznik awarii czujnika cisnienia
				unsigned _RS_START:1;		//znacznik rozruchu strtownika po resecie
            };
        };   
        union   {
            unsigned char flag4;
            struct
            {                 
				unsigned _PW_START:1;		//znacznik rozruchu strtownika po zalaczeniu zasilania
				unsigned _TURBO:1;			//znacznik rodzaju kotla (0-atmosferyczny, 1- turbo)				
				unsigned _PRACA:1;			//znacznik aktywnej proc. ROZRUCH lub aktywnego stanu GRZEJ
				unsigned _OST_PL:1;			//znacznik ostatniej proby przy braku plomienia
				unsigned _RUN_MNMOD:1;		//praca na minimum modulacji (szybki narost temp. CO)
				unsigned _KCW_PMP:1;		//znacznik obiegu CW dla kotla bitermicznego
				unsigned _DE_VNT:1;			//znacznik probnego wybiegu wentylatora
				unsigned _prb_CIG:1;		//znacznik pewnego braku ciagu
            };
        };

				unsigned char LICZNIK_PL;	//licznik prob procedury awaryjnej braku plomienia
				unsigned char LICZNIK_CIG;	//licznik ponowien prob braku ciagu
				unsigned char LICZNIK_CIG2;	//licznik ponowien prob braku ciagu	(atmosferyczny)
				unsigned char LICZNIK_CIG3;	//licznik ponowien prob braku ciagu

				unsigned char ERR_BTY;		//aktualny kod awaryjny
				unsigned char ROZ;
				unsigned char R48;
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
				unsigned char RST;
		}tpMAIN;								
//----------------------------------------------------------
