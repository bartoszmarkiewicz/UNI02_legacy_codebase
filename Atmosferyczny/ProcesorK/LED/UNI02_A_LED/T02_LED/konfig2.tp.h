/*konfig.tp.h*/
//typy danych dla pliku konfig.c
//----------------------------------------------------------
#ifndef	_KONFIG_TPH
#define	_KONFIG_TPH
//----------------------------------------------------------

    volatile bit DtKNFf_enknf;		//aktywacja proc konfiguracji
    volatile bit DtKNFf_wymCO;		//wymuszenie grzania w obiegu CO
    volatile bit DtKNFf_wymCW;		//wymuszenie grzania w obiegu CW

typedef struct	sDtKONF{
    /*struct{
        unsigned _enknf:1;		//aktywacja proc konfiguracji
        unsigned _wymCO:1;		//wymuszenie grzania w obiegu CO
        unsigned _wymCW:1;		//wymuszenie grzania w obiegu CW
        
        unsigned int flags0;
    };*/
                
                
    volatile unsigned char PRM;		//nr. modyfikowanego parametru		
    volatile unsigned char KRK;		//krok konfiguracji	

    volatile unsigned char mstr;		//moc startowa
    volatile unsigned char mxcw;		//moc maksymalna WU		
    volatile unsigned char mxco;		//moc maksymalna WG
    volatile unsigned char czpr;		//czujnik przeplywu WG
    volatile unsigned char tpkt;		//typ kotla
    volatile unsigned char rdgz;		//rodzaj gazu
    volatile unsigned char rdpm;		//rodzaj pompy
    volatile unsigned char dtpm;		//wielkosc delty T dla pompy
    volatile unsigned char mdpm;		//dolny prog predkosci dla pompy
    volatile unsigned char mgpm;		//dolny prog predkosci dla pompy                
    volatile unsigned char teco;		//tryb pracy ECO
    volatile unsigned char tpwm;		//typ wymiennika
    volatile unsigned char robg;		//typ obiegu 0-otwarty, 1-zamkniety
    volatile unsigned char opdg;		//ogrzewanie podlogowe
    volatile unsigned char tpcs;		//rodzaj czujnika cisnienia

    unsigned char aleg;		//znacznik aktywacji timera ANTY-LEGIONELLA
}DtKONF;

typedef struct	sDtPZK{
				unsigned int dta;	//wartosc zapamietana
				unsigned int min;	//dozwolone minmum
				unsigned int max;	//dozwolone maksimum
				unsigned int bfdta;	//wartosc zmodyfikowana (zmienna dynamicznie)

				unsigned zps:1;		//znacznik ze parametr zmodyfikowano
				unsigned rld:1;		//flaga zezwolenia na przescie z maksimum na minimum i odwrotnie
				unsigned no_msg:1;	//znacznik ze komunikaty o proc. awarynej nie ma byc wyswietlany w trakcie modyfikacji parametru;
				unsigned shw:1;		//blokada wyswietlania parametru(0 - zablokowany)
				}DtPZK;
//----------------------------------------------------------
#endif /*_KONFIG_TPH*/
