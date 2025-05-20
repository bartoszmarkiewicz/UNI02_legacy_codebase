/*pomiar.tp.h*/
//typy danych dla plinu pomiar.c
//----------------------------------------------------------
#ifndef	_POMIAR_TPH
#define	_POMIAR_TPH
//----------------------------------------------------------
volatile bit DtCOf_err_CO;      //awaria czujnika CO
typedef struct	{
                unsigned int AC_NTC;	//wartosc AC dla temp CO
				unsigned char CO;		//temp. CO
                float NTC;                      //temp. CO
                float popNTC;                   //poprawka dla NTC
                unsigned char tNTC;              //temp. CO
                unsigned char dtNTC;            //dziesiate czesci temp. CO
                
                //float NST1;
                //float NST2;
                //float NST3;
                
				}DataCO;
//---------------------------------------------------------- 
volatile bit DtCWf_err_CW;      //awaria czujnika CW                
typedef struct	{
				//unsigned _err_CW:1;		//awaria czujnika
				unsigned char CW;		//temp. CW
				unsigned char AC_CW;	//wartosc AC dla temp CW
				}DataCW;
//----------------------------------------------------------
volatile bit DtZWf_on_ZW;      //awaria czujnika ZW                
typedef struct	{
				//unsigned _on_ZW:1;		//jest czujnik temperatury zewnetrznej
				unsigned char AC_ZW;	//wartosc AC dla temp. zewnetrznej
				signed char ZW;			//tempratura zewnwtrzna
				signed char SZW;		//przefiltrowana temp. ZW		
				}DataZW;
//----------------------------------------------------------
volatile bit DtZSf_err_ZS;      //awaria czujnika ZS                
typedef struct	{
				//unsigned _err_ZS:1;		//awaria czujnika
				unsigned char AC_ZS;	//wartosc AC dla temp. wlotowej
				unsigned char ZS;		//tempratura wlotowa
				}DataZS;
/*
//----------------------------------------------------------
typedef struct	{
				unsigned _newPFN:1;		//nastapila zmana pozycji
				unsigned _csn:1;		//pozycja CISNIENIE
				unsigned _off:1;		//pozycja OFF
				unsigned _lato:1;		//pozycja LATO
				unsigned _zima:1;		//pozycja ZIMA
				unsigned _res:1;
				unsigned poz;
				unsigned AC;
				}DataPFN;
//----------------------------------------------------------
typedef struct	{
				unsigned _hi:1;			//zbyt wysokie cisnienie
				unsigned _lo:1;			//za niskie cisnienie
				unsigned _no:1;
				unsigned _error:1;		//brak czujnika - stan awaryjny
				unsigned _work:1;		//cisnienie w dozwolonym zakresie							
				unsigned _no_mont;		//brak czujnika - nie montowany
				unsigned char CS;		//wartosc cisnienia
				}DataCSN;
//----------------------------------------------------------
typedef struct	{
				unsigned int Prog;			//wynik pomiaru
                                unsigned char Hist;
				}DataParCapKey;
typedef struct	{
				unsigned Stan:1;			//stan przycisku
				unsigned int Wynik;			//wynik pomiaru
				}DataCapKey;
 */
#endif	/*_POMIAR_TPH*/
