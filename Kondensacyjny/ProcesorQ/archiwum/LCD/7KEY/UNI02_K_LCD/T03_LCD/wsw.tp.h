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
typedef struct	{
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
				unsigned sdiode:1;		//zapal wybrany(e) znak(i)
				unsigned rdiode:1;		//wygas wybrany(e) znak(i)
				unsigned dyn:1;			//zaladuj dana z bufora wspolnego na pozostale 2 cyfry
				unsigned only;			//odswieza tylko dwie cyfry, pozostale dwie aktualizowane sa z najnizszego poziomu wyswietlania	
				unsigned sbdid:8;		//bity odnoszace sie do kolenych znaków na ktorych ma byc wyk oper. sdiode (zapalenie)
				unsigned rbdid:8;		//bity odnoszace sie do kolenych znaków na ktorych ma byc wyk oper. rdiode (wygaszenie)
				}WStatus;				
				
//Wartosc danej do wyswietlenia				
typedef struct	{
				unsigned char Data1;	//wartosc starszej cyfry przy formacie led (hex=0,dec=0)
				unsigned char Data0;	//wartosc przy formatach hex,dec lub mlodsza cyfra dla formatu led
				}WData;
//Razem wartosc danej do wyswietlenia i format jej wyswietlenia				
typedef struct	{
				//dwie najmlodsze cyfry
					WStatus St;				//format danej (sposob przetwarzania)
					WData	Dt;				//dana do wyswietlenia
#if LCD4D!=0
				//dwie najstarsze cyfry
					WData	Dt2;			//dana do wyswietlenia
					WStatus St2;			//format danej (sposob przetwarzania)
#endif
				}WPack;

typedef struct	{
					WStatus St;				//format danej (sposob przetwarzania)
					WData	Dt;				//dana do wyswietlenia
				}WDyn;

typedef struct	{
				unsigned focus:1;		//aktywacja/deaktywacja pozycji kolejki
				unsigned m:1;			//aktywacja/deaktywacja znaku graficznego
				unsigned p:1;			//znacznik czy znak ma migac
				}WMark;
//--------------------------------------------------------------------
//Dana po przetworzeniu podzielona na czesc starsza i mlodsza do wyslania na wyswietlacz 2 cyfrowy				
typedef struct 	{
				unsigned setOff:1;
#if LCD4D!=0
				unsigned char dLed3;	//wartosc starszej cyfry wysylana na wyswietlacz
				unsigned char dLed2;	//wartosc mlodszej cyfry wysylana na wyswietlacz
#endif
				unsigned char dLed1;	//wartosc starszej cyfry wysylana na wyswietlacz
				unsigned char dLed0;	//wartosc mlodszej cyfry wysylana na wyswietlacz
				unsigned int mark;		//znacznik wysterowania znaku graficznego
				unsigned int mask;		//maska obslugiwanych nakow graficznych
				}LedPack;
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//TYPY DANYCH DOTYCZACE POZYCJI W KOLEJCE
//Adres danej dynamicznej umieszczonej na okreslonej pozycji w kolejce
//Typ elementu tablicy o rozmiarze rownym liczbie pozycji w kolejce.				
typedef struct	{
				volatile WPack *PAdr;				//adres do danej dynamicznej umieszczonej na okreslonej pozycji w kolejce
				}KAdrPoz;
#if LCD4D==0
	//Wartosc danej statycznej na pozycji w kolejce
	//Typ elementu tablicy o rozmiarze rownym liczbie pozycji w kolejce.				
	typedef struct	{
					WData PWrt;					//wartosc danej statycznej umieszczonej na okreslonej pozycji w kolejce
					unsigned char delay;
					}KWrtPoz;
	//Format danej statycznej na pozycji w kolejce
	//Typ elementu tablicy o rozmiarze rownym liczbie pozycji w kolejce.				
	typedef struct	{
					WStatus PFrm;				//format danej statycznej umieszczonej na okreslonej pozycji w kolejce
	
					}KFrmPoz;
#else
	//Wartosc danej statycznej na pozycji w kolejce
	//Typ elementu tablicy o rozmiarze rownym liczbie pozycji w kolejce.				
	typedef struct	{
#if RSPADR!=0
					WData PWrt;					//wartosc danej statycznej umieszczonej na okreslonej pozycji w kolejce
					WData PWrt2;					//wartosc danej statycznej umieszczonej na okreslonej pozycji w kolejce
#endif
					unsigned char delay;
					}KWrtPoz;
	//Format danej statycznej na pozycji w kolejce
	//Typ elementu tablicy o rozmiarze rownym liczbie pozycji w kolejce.				
	typedef struct	{
					WStatus PFrm;				//format danej statycznej umieszczonej na okreslonej pozycji w kolejce
					WStatus PFrm2;				//format danej statycznej umieszczonej na okreslonej pozycji w kolejce
					}KFrmPoz;
#endif

//Okresla czy dana pozycja kolejki ma byc poddana ekspozycji
//Typ elementu tablicy o rozmiarze rownym liczbie pozycji w kolejce.
typedef struct	{
				unsigned focus:1;			//aktywacja ekspozycji pojedynczej pozycji w kolejce
				}KFocus;
//Adres pod ktory ma trafiac wynik dzialania kolejki, czas ekspozycji kolejki.
//Typ elementu tablicy o rozmiarze NKOL.				
typedef struct	{				 	
				volatile WPack *CAdr;				//adres pod ktory ma trafic wynik dzialania kolejki
				unsigned char nCykli;		//czas ekspozycji kolejki - liczba pelnych obiegow (0->nieskonczony)
				}KOutput;				
//Typ danej zawierajacej wszystkie informacje o kolejce 				
typedef struct	{
				const KAdrPoz	*KAP;		//tablica adresow do danych dynamicznych na kolejnych pozycjach kolejki
				const KWrtPoz	*KWP;		//tablica wartosci stalych wyswietlanych na kolejnych pozycjach kolejki
#if RSPADR!=0
				const KFrmPoz	*KFP;		//tablica wartosci formatu wyswietlenia kolejnych pozycji kolejki
#endif					
				KFocus	*KFc;				//tablica flag aktywacji kolejnych pozycji kolejki
				const KOutput	*KOut;		//adres docelowy dla kolejki w tablicy priorytetow wyswietlania PWD oraz czas ekspozycji kol.
				}Kolejka;
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//TYPY DANYCH DOTYCZACE CALYCH KOLEJEK					
//Podstawowe dane kolejki: adres kolejki, liczba pozycji kolejki,
//Typ elementu tablicy o rozmiarze NKOL.
typedef struct	{
				const Kolejka *KL;			//adres kolejki
				unsigned char nPzc;			//liczba pozycji kolejki
				}KGlobal;
//Zawiera informacje o aktualnie wyswietlanej pozycji kolejki
//Bit restart po ustawieniu powoduje restart kolejki (indeks<-0)
//Typ elementu tablicy o rozmiarze NKOL. 
typedef struct	{
				unsigned restart2:1;			//restart kolejki (przy okreslonym czasie ekspozycji uaktywnia wszystkie poz. kolejki)
				unsigned restart1:1;			//restart kolejki (przy okreslonym czasie ekspozycji uaktywnia wszystkie poz. kolejki)
				unsigned restart:1;			//restart kolejki (przy okreslonym czasie ekspozycji uaktywnia wszystkie poz. kolejki)
				unsigned reset:1;			//reset kolejki (przy okreslonym czasie ekspozycji deaktywuje wszystkie poz. kolejki)
				unsigned indeks:6;			//nr aktualnie wyswietlanej pozycji
				}KIndeks;
//Dane potrzebne do odliczania czasu ekspozycji dla kazdej pozycji kolejki
//oraz dla calej kolejki (liczba pelnych cykli)
//Typ elementu tablicy o rozmiarze NKOL.
typedef struct	{
				unsigned char ncS;			//czas trwania ekspozycji
				unsigned char ndS;
				unsigned char cykl;			//czas trwania kolejki (liczba ekspozycji wszystkich aktywnych elementow kolejki)
				}KTime;												
//--------------------------------------------------------------------
//----------------------------------------------------------
#endif	/*_WSW_TPH*/
