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

typedef struct	{ 
				signed int min;	//dozwolone minmum  //kkk
				signed int max;	//dozwolone maksimum //kkk
				signed int dta;	//wartosc zapamietana //kkk
				signed int bfdta;	//wartosc zmodyfikowana (zmienna dynamicznie)  //kkk
                union   {
                    unsigned char war;
                    struct //__PACKED
                    {
                        unsigned _flb7:1;   
                        unsigned flt:1;     //liczba z kropka  (int=(float*10)) //kkk                        
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
				const signed int min;	//dozwolone minmum  //kkk
				const signed int max;	//dozwolone maksimum  //kkk
                const unsigned char flwar;
				}CDtPZK;    
typedef struct	{ 
                    union   {
                        unsigned char war;
                        struct //__PACKED
                        {
                            unsigned flbt0:1;     //
                            unsigned flt:1;     //liczba z kropka  (int=(float*10)) //kkk                  
                            unsigned shw:1;		//blokada wyswietlania parametru(0 - zablokowany)                        
                            unsigned P:1;		//znacznik wartosci procentowej /wyswietlania jednostki BAR                        
                            unsigned T:1;		//znacznik wyswietlania jednostki C                        
                            unsigned high:1;	//znacznik wyswietlania parametru na wyzszej pozycji (0 - mlodsze dwie cyfry)                        
                            unsigned no_msg:1;	//znacznik ze komunikaty o proc. awarynej nie ma byc wyswietlany w trakcie modyfikacji parametru;                        
                            unsigned rld:1;		//flaga zezwolenia na przescie z maksimum na minimum i odwrotnie                        
                        };
                    }fl;  
				}FlPZK;           //kkk                
//----------------------------------------------------------
#endif /*_KONFIG_TPH*/
