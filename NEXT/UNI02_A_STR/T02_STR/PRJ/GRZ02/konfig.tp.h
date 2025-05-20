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
				unsigned wymAL:1;		//wymuszenie grzania dla funkcji AL
                unsigned _fl0bt4:1;			//
                unsigned _fl0bt5:1;			//                
                unsigned _fl0bt6:1;			//
                unsigned _fl0bt7:1;			//                
            };
        };
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

				unsigned char aleg;	//znacznik 1 aktywacji ANTY-LEGIONELLA
				}DtKONF;
//----------------------------------------------------------
#endif /*_KONFIG_TPH*/
