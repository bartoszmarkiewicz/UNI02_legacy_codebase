/*pomiar.tp.h*/
//typy danych dla plinu pomiar.c
//----------------------------------------------------------
#ifndef	_POMIAR_TPH
#define	_POMIAR_TPH
//----------------------------------------------------------
volatile bit DtCOf_err_CO;      //awaria czujnika CO
typedef struct	{
       union   {
            unsigned char flag0;
            struct
            {                
				unsigned _newPCO:1;     //flaga nowej wartosci nastawy
				unsigned _pozZRO:1;		//flaga stanu "0" nastawy
				unsigned _newPOZ:1;     //flafga zmiany pozycji z "0" na inna i odwrotnie
				unsigned _toUP:1;       //kierunek zminy wartosci na cele filtracji
				unsigned _toDW:1;       //kierunek zminy wartosci na cele filtracji
				unsigned _newZKR:1;     //flaga zminy zakresu nastawy
				unsigned _stabPCO:1;    //flaga stabilnosci nastawy (utrzymanie niezmiennej wartosci przez ustalony czas)	
				unsigned _initFLT:1;    //wymuszona aktualizacja wartosci filtrowanych
            };
        };        
                unsigned int AC_PCO;	//wartosc AC dla nastawy PCO
                unsigned char PCO;		//nastawa PCO 
                unsigned char minPCO;   //dolny margines zakresu nastawy            
                unsigned char maxPCO;   //gorny margines zakresu nastawy                 
				}DataCO;
//---------------------------------------------------------- 
volatile bit DtCWf_err_CW;      //awaria czujnika CW                
typedef struct	{
       union   {
            unsigned char flag0;
            struct
            {                
				unsigned _newPCW:1;     //flaga nowej wartosci nastawy
				unsigned _pozZRO:1;		//flaga stanu "0" nastawy
				unsigned _newPOZ:1;     //flafga zmiany pozycji z "0" na inna i odwrotnie	
				unsigned _toUP:1;       //kierunek zminy wartosci na cele filtracji
				unsigned _toDW:1;       //kierunek zminy wartosci na cele filtracji
				unsigned _newZKR:1;     //flaga zminy zakresu nastawy
				unsigned _stabPCW:1;    //flaga stabilnosci nastawy (utrzymanie niezmiennej wartosci przez ustalony czas)	
				unsigned _initFLT:1;    //wymuszona aktualizacja wartosci filtrowanych
            };
        };    
				unsigned int AC_PCW;	//wartosc AC dnastawy PCW
                unsigned char PCW;		//nastawa PCW          
                unsigned char minPCW;   //dolny margines zakresu nastawy             
                unsigned char maxPCW;   //gorny margines zakresu nastawy                 
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
