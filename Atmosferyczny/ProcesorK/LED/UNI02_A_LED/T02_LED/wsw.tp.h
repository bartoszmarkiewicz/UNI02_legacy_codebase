/*wsw.tp.h*/
//typy danych dla pliku wsw.c
//----------------------------------------------------------
#ifndef	_WSW_TPH
#define	_WSW_TPH
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//PODSTAWOWE TYPY DANYCH 
//--------------------------------------------------------------------
//Informacja o sposobie przetworzenia danej do wyswietlenia przed wyslaniem na wyswietlacz
typedef struct sWStatus{
				unsigned focus:1;		//aktywacja/deaktywacja paczki danych (wysw.priorytetowe)
				unsigned dec:1;			//wybor typu na kod dec
				unsigned hex:1;			//wybor typu na kod hex
				unsigned autoOFF:1;		//automatyczne wygaszenie starszej cyfry przy wart. 0		
				unsigned kropka0:1;		//kropka dla cyfry 0		
				unsigned kropka1:1;		//kropka dla cyfry 1		
				unsigned wygas0:1;		//zapal/wygas cyfre 0
				unsigned wygas1:1;		//zapal/wygas cyfre 1
				unsigned led0:1;		//mlodsza cyfra w kodzie led niezaleznie od wybr. typu (dec/hex)					
				unsigned led1:1;		//starsza cyfra w kodzie led niezaleznie od wybr. typu (dec/hex)
				unsigned sdiode:1;		//zapal wybrane diody
				unsigned rdiode:1;		//wygas wybrane diody
				unsigned wbdid:4;		//bity odnoszace sie do kolenych diod ns ktorych ma byc wyk oper. sdiode/rdiode
}WStatus;				
				
//Wartosc danej do wyswietlenia				
typedef struct	sWData{
				unsigned char Data1;	//wartosc starszej cyfry przy formacie led (hex=0,dec=0)
				unsigned char Data0;	//wartosc przy formatach hex,dec lub mlodsza cyfra dla formatu led
				}WData;
//Razem wartosc danej do wyswietlenia i format jej wyswietlenia				
typedef struct	sWPack{
				WStatus St;				//format danej (sposob przetwarzania)
				WData	Dt;				//dana do wyswietlenia
				}WPack;
//--------------------------------------------------------------------
//Dana po przetworzeniu podzielona na czesc starsza i mlodsza do wyslania na wyswietlacz 2 cyfrowy				
typedef struct 	sLedPack{
    //struct{
        unsigned setOff:1;
                
      //  unsigned int flags0;
    //};
    unsigned char dLed1;	//wartosc starszej cyfry wysylana na wyswietlacz
    unsigned char dLed0;	//wartosc mlodszej cyfry wysylana na wyswietlacz
}LedPack;
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//TYPY DANYCH DOTYCZACE POZYCJI W KOLEJCE
//Adres danej dynamicznej umieszczonej na okreslonej pozycji w kolejce
//Typ elementu tablicy o rozmiarze rownym liczbie pozycji w kolejce.				
typedef struct	sKAdrPoz{
				volatile WPack *PAdr;				//adres do danej dynamicznej umieszczonej na okreslonej pozycji w kolejce
				}KAdrPoz;
//Wartosc danej statycznej na pozycji w kolejce
//Typ elementu tablicy o rozmiarze rownym liczbie pozycji w kolejce.				
typedef struct	sKWrtPoz{
				WData PWrt;					//wartosc danej statycznej umieszczonej na okreslonej pozycji w kolejce
				unsigned char delay;
				}KWrtPoz;
//Format danej statycznej na pozycji w kolejce
//Typ elementu tablicy o rozmiarze rownym liczbie pozycji w kolejce.				
typedef struct	sKFrmPoz{
				WStatus PFrm;				//format danej statycznej umieszczonej na okreslonej pozycji w kolejce
				}KFrmPoz;
//Okresla czy dana pozycja kolejki ma byc poddana ekspozycji
//Typ elementu tablicy o rozmiarze rownym liczbie pozycji w kolejce.
typedef struct	sKFocus{
				unsigned focus:1;			//aktywacja ekspozycji pojedynczej pozycji w kolejce
				}KFocus;
//Adres pod ktory ma trafiac wynik dzialania kolejki, czas ekspozycji kolejki.
//Typ elementu tablicy o rozmiarze NKOL.				
typedef struct	sKOutput{				 	
				volatile WPack *CAdr;				//adres pod ktory ma trafic wynik dzialania kolejki
				unsigned char nCykli;		//czas ekspozycji kolejki - liczba pelnych obiegow (0->nieskonczony)
				unsigned char DMask;		//maska rezerwacji diod sygnalizacyjnych dla potrzeb kolejki
				}KOutput;				
//Typ danej zawierajacej wszystkie informacje o kolejce 				
typedef struct	sKolejka{
				const KAdrPoz	*KAP;		//tablica adresow do danych dynamicznych na kolejnych pozycjach kolejki
				const KWrtPoz	*KWP;		//tablica wartosci stalych wyswietlanych na kolejnych pozycjach kolejki
				const KFrmPoz	*KFP;		//tablica wartosci formatu wyswietlenia kolejnych pozycji kolejki					
				KFocus	*KFc;				//tablica flag aktywacji kolejnych pozycji kolejki
				const KOutput	*KOut;		//adres docelowy dla kolejki w tablicy priorytetow wyswietlania PWD oraz czas ekspozycji kol.
				}Kolejka;
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//TYPY DANYCH DOTYCZACE CALYCH KOLEJEK					
//Podstawowe dane kolejki: adres kolejki, liczba pozycji kolejki,
//Typ elementu tablicy o rozmiarze NKOL.
typedef struct	sKGlobal{
				const Kolejka *KL;			//adres kolejki
				unsigned char nPzc;			//liczba pozycji kolejki
				}KGlobal;
//Zawiera informacje o aktualnie wyswietlanej pozycji kolejki
//Bit restart po ustawieniu powoduje restart kolejki (indeks<-0)
//Typ elementu tablicy o rozmiarze NKOL. 
typedef struct	sKIndeks{
				unsigned restart:1;			//restart kolejki (przy okreslonym czasie ekspozycji uaktywnia wszystkie poz. kolejki)
				unsigned reset:1;			//reset kolejki (przy okreslonym czasie ekspozycji deaktywuje wszystkie poz. kolejki)
				unsigned indeks:6;			//nr aktualnie wyswietlanej pozycji
				}KIndeks;
//Dane potrzebne do odliczania czasu ekspozycji dla kazdej pozycji kolejki
//oraz dla calej kolejki (liczba pelnych cykli)
//Typ elementu tablicy o rozmiarze NKOL.
typedef struct	sKTime{
				unsigned char ncS;			//czas trwania ekspozycji
				unsigned char ndS;
				unsigned char cykl;			//czas trwania kolejki (liczba ekspozycji wszystkich aktywnych elementow kolejki)
				}KTime;												
//--------------------------------------------------------------------
//----------------------------------------------------------
#endif	/*_WSW_TPH*/
