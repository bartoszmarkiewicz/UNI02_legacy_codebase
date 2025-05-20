/*konfig.tp.h*/
//typy danych dla pliku konfig.c
//----------------------------------------------------------
#ifndef	_KONFIG_TPH
#define	_KONFIG_TPH
//----------------------------------------------------------
typedef struct	{
                unsigned char KNPAR;	//ogolna liczba parametrow
				unsigned char PRM;		//nr. modyfikowanego parametru		
				unsigned char KRK;		//krok konfiguracji	
				}DtKONF;

/*typedef struct	{
				unsigned int min;	//dozwolone minmum
				unsigned int max;	//dozwolone maksimum
				unsigned int dta;	//wartosc zapamietana
				unsigned int bfdta;	//wartosc zmodyfikowana (zmienna dynamicznie)
				unsigned rld:1;		//flaga zezwolenia na przescie z maksimum na minimum i odwrotnie
				unsigned no_msg:1;	//znacznik ze komunikaty o proc. awarynej nie ma byc wyswietlany w trakcie modyfikacji parametru;
				unsigned high:1;	//znacznik wyswietlania parametru na wyzszej pozycji (0 - mlodsze dwie cyfry)
				unsigned T:1;		//znacznik wyswietlania jednostki C
				unsigned P:1;		//znacznik wartosci procentowej /wyswietlania jednostki BAR
				unsigned shw:1;		//blokada wyswietlania parametru(0 - zablokowany)
				}DtPZK;*/
typedef struct	{ 
				unsigned int min;	//dozwolone minmum
				unsigned int max;	//dozwolone maksimum
				unsigned int dta;	//wartosc zapamietana
				unsigned int bfdta;	//wartosc zmodyfikowana (zmienna dynamicznie)
                union   {
                    unsigned char war;
                    struct //__PACKED
                    {
                        unsigned _flb7:1;   
                        unsigned _flb6:1;                        
                        unsigned shw:1;		//blokada wyswietlania parametru(0 - zablokowany)                        
                        unsigned P:1;		//znacznik wartosci procentowej /wyswietlania jednostki BAR                        
                        unsigned T:1;		//znacznik wyswietlania jednostki C                        
                        unsigned high:1;	//znacznik wyswietlania parametru na wyzszej pozycji (0 - mlodsze dwie cyfry)                        
                        unsigned no_msg:1;	//znacznik ze komunikaty o proc. awarynej nie ma byc wyswietlany w trakcie modyfikacji parametru;                        
                        unsigned rld:1;		//flaga zezwolenia na przescie z maksimum na minimum i odwrotnie                        
                    };
                }fl;                
				}DtPZK;
typedef struct	{ 
				const unsigned int min;	//dozwolone minmum
				const unsigned int max;	//dozwolone maksimum
                const unsigned char flwar;
				}CDtPZK;                
//----------------------------------------------------------
#endif /*_KONFIG_TPH*/
