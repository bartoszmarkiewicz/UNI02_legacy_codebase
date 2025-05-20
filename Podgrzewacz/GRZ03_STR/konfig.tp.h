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
				unsigned wymAL:1;		//wymuszenie grzania dla funkcji AL

				unsigned char PRM;		//nr. modyfikowanego parametru		
				unsigned char KRK;		//krok konfiguracji	

				unsigned char mstr;		//moc startowa
				unsigned char mxcw;		//moc maksymalna WU		
				unsigned char mxco;		//moc maksymalna WG
				unsigned char czpr;		//czujnik przeplywu WG
				unsigned char tpkt;		//typ kotla
				unsigned char rdgz;		//rodzaj gazu
				unsigned char rdpm;		//rodzaj pompy
				unsigned char tpwm;		//typ wymiennika
				unsigned char nokr;		//liczba okresow na sek
				unsigned char mmin;		//moc minimalna
				unsigned char mmax;		//moc maksymalna                
                unsigned char rfan;     //rodzaj wentylatora
				unsigned char aleg;	//znacznik 1 aktywacji ANTY-LEGIONELLA
				}DtKONF;
//----------------------------------------------------------
#endif /*_KONFIG_TPH*/
