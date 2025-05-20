/*konfig.tp.h*/
//typy danych dla pliku konfig.c
//----------------------------------------------------------
#ifndef	_KONFIG_TPH
#define	_KONFIG_TPH
//----------------------------------------------------------
typedef struct	{
        union   {
            unsigned char flag0;
            struct
            {    
				unsigned enknf:1;		//aktywacja proc konfiguracji
				unsigned wymCO:1;		//wymuszenie grzania w obiegu CO
				unsigned wymCW:1;		//wymuszenie grzania w obiegu CW
                unsigned _fl0bt3:1;			//                
                unsigned _fl0bt4:1;			//
                unsigned _fl0bt5:1;			//                
                unsigned _fl0bt6:1;			//
                unsigned _fl0bt7:1;			//                
            };
        };
				unsigned char PRM;		//nr. modyfikowanego parametru		
				unsigned char KRK;		//krok konfiguracji	

				unsigned char dmstr;		//moc startowa (zmiana dynamiczna w konfiguracji)
				unsigned char dmxcw;		//moc maksymalna WU		
				unsigned char dmxco;		//moc maksymalna WG

				unsigned char mstr;		//moc startowa
				unsigned char mxcw;		//moc maksymalna WU		
				unsigned char mxco;		//moc maksymalna WG
				unsigned char def_mxco;		//moc maksymalna WG
				unsigned char mxco2;	//moc maksymalna WG aktywna przez nastawiony czas
				unsigned char tmxco2;	//czas aktywno?ci mocy mxco2
				
				unsigned char czpr;		//czujnik przeplywu WG
				unsigned char kodkt;	//kod kotla
				unsigned char tpkt;		//typ kotla (przeplywowy, jednofunkcyjny)
				unsigned char rdgz;		//rodzaj gazu

				unsigned char rdpm;		//rodzaj pompy
				unsigned char dtpm;		//wielkosc delty T dla pompy
				unsigned char mdpm;		//dolny prog predkosci dla pompy
				unsigned char teco;		//tryb pracy ECO

				unsigned char tpwm;		//typ wymiennika
				unsigned char robg;		//typ obiegu 0-otwarty, 1-zamkniety
				unsigned char nokr;		//liczba okresow na sek
				unsigned char mmin;		//moc minimalna
				unsigned char mmax;		//moc maksymalna
				unsigned char aleg;		//znacznik aktywacji timera ANTY-LEGIONELLA
				unsigned char opdg;		//ogrzewanie podlogowe
				unsigned char tpcs;		//rodzaj czujnika cisnienia

                unsigned char mgpm;		//gorny prog predkosci dla pompy
                unsigned char mgpm2;	//gorny prog predkosci dla pompy w obiegu CW
                unsigned char alin;		//adres odbirnika dla magistrali LIN
				
				unsigned char hsco;		//histereza wylaczenia dla CO w trakcie grzania CO
				unsigned char conw;		//temp CO w trakcie grzania CW w kotlach jednofunkcyjnych 
				unsigned char prop;		//przesuniecie rownolegle krzywej grzania dla reg.pogodowego
                unsigned char rfan;     //rodzaj wentylatora
                unsigned char tstr;     //czas wybiegu na mocy startowej przy grzaniu CO
                
				unsigned char pgdr;		//rodzaj regulatora pogodowego
                unsigned char pgdg;     //wartosc graniczna pobudzenia od regulatora pogodowego
                unsigned char pgdo;     //obnizenie nocne dla regulatora pogodowego
                
                unsigned char pgmx;     //maksymalna wartosc graniczna CO
                unsigned char cwmg;     //histereza wylaczenia CW(2F)                
                unsigned char cwtm;     //we. sygnalu timera zasobnika (1F)
                
                unsigned char tL3;      //czas blokady L3 obiegu CO   
                
                float P;
                float I;
                float D;

                float RCO1;             //korekta punktu rownowagi w fazie dochodzenia (FAZA1) 
                float RCO2;             //regulacja w punkcie rownowagi w fazie stabilizacji (FAZA2)                
                float RCO3;             //korekta punktu rownowagi w fazie stabilizacji (FAZA2)
                float RCO4;             //reakcja superwizyjna
                
                float RCW1;             //korekta punktu rownowagi w fazie dochodzenia (FAZA1)
                float RCW2;             //regulacja w punkcie rownowagi w fazie stabilizacji (FAZA2)
                float RCW3;             //korekta punktu rownowagi w fazie stabilizacji (FAZA2)
                float RCW4;             //reakcja superwizyjna
                            
				}DtKONF;
typedef struct	{
				unsigned int addr;	//ADRES PARAMETRU W PAMIECI EEPROM
				unsigned int min;	//dozwolone minmum
				unsigned int max;	//dozwolone maksimum

				}DtMINMAX;
//----------------------------------------------------------
#endif /*_KONFIG_TPH*/
