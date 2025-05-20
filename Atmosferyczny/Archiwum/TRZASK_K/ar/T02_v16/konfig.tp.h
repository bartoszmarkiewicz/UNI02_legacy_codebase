/*konfig.tp.h*/
//typy danych dla pliku konfig.c
//----------------------------------------------------------
#ifndef	_KONFIG_TPH
#define	_KONFIG_TPH
//----------------------------------------------------------
typedef struct	{
				unsigned enknf:1;		//aktywacja proc konfiguracji
				unsigned wymCO:1;		//wymuszenie grzania w obiegu CO
				unsigned wymCW:1;		//wymuszenie grzania w obiegu CW

				unsigned char PRM;		//nr. modyfikowanego parametru		
				unsigned char KRK;		//krok konfiguracji	

				unsigned char mstr;		//moc startowa
				unsigned char mxcw;		//moc maksymalna WU		
				unsigned char mxco;		//moc maksymalna WG
				unsigned char czpr;		//czujnik przeplywu WG
				unsigned char tpkt;		//typ kotla
				unsigned char rdgz;		//rodzaj gazu
				//unsigned char rdpm;		//rodzaj pompy
				unsigned char tpwm;		//typ wymiennika
				unsigned char robg;		//typ obiegu 0-otwarty, 1-zamkniety
				unsigned char opdg;		//ogrzewanie podlogowe
				unsigned char tpcs;		//rodzaj czujnika cisnienia

				unsigned char rdpm;		//rodzaj pompy
				unsigned char dtpm;		//wielkosc delty T dla pompy
				unsigned char mdpm;		//dolny prog predkosci dla pompy
				unsigned char mgpm;		//gorny prog predkosci dla pompy
				unsigned char teco;		//tryb pracy ECO

				unsigned char mgpm2;	//gorny prog predkosci dla pompy w obiegu CW
				unsigned char hsco;		//histereza wylaczenia dla CO w trakcie grzania CO
				unsigned char conw;		//temp CO w trakcie grzania CW w kotlach jednofunkcyjnych 
				unsigned char prop;		//przesuniecie rownolegle krzywej grzania dla reg.pogodowego

                unsigned char pgdr;		//tryb pracy regulatora pogodowego
                unsigned char pgmx;     //maksymalna wartosc graniczna CO dla regulatora pogodowego                
                unsigned char cwmg;     //histereza wylaczenia CW(4F)                
                unsigned char cwtm;     //we. sygnalu timera zasobnika (1F)
                
				unsigned char aleg;		//znacznik aktywacji timera ANTY-LEGIONELLA
                
                unsigned char alin;		//adres odbirnika dla magistrali LIN                
				}DtKONF;
typedef struct	{
				unsigned int addr;	//ADRES PARAMETRU W PAMIECI EEPROM
				unsigned int min;	//dozwolone minmum
				unsigned int max;	//dozwolone maksimum

				}DtMINMAX;                
//----------------------------------------------------------
#endif /*_KONFIG_TPH*/
