/*pomiar.tp.h*/
//typy danych dla plinu pomiar.c
//----------------------------------------------------------
#ifndef	_POMIAR_TPH
#define	_POMIAR_TPH
//----------------------------------------------------------
typedef struct	{
				unsigned _err_CO:1;		//awaria czujnika
				unsigned _su_CO:1;		//przekroczenie dolnej granicy zalaczenia
				unsigned _ad_CO:1;		//przekroczenie gornej granicy wylaczenia		
				unsigned _su_SRW_CO:1;	//przekroczenie dolnej granicy zalaczenia
				unsigned _ad_SRW_CO:1;	//przekroczenie gornej granicy wylaczenia		
				unsigned _on_DZ_CO:1;	//osiagnieto temp dyzurna CO
				unsigned _su_DZ_CO:1;	//przekroczenie dolnej granicy zalaczenia dyzurnej CO
				unsigned _ad_DZ_CO:1;	//przekroczenie gornej granicy wylaczenia dyzurnej CO		
				unsigned _ptwp_CO:1;	//temperaturowy punkt wylaczenia pompy
				unsigned _pz_CO:1;		//punkt zalaczenia od CO przy grzaniu CW
				unsigned _cold_CO:1;	//zalacz pompe ponizej okreslonego progu temp.
				unsigned _hot_CO:1;		//stan przegrzania
				unsigned _ldDCO:1;		//znacznik zbyt szybkiego narostu temperatury
				unsigned _zroPCO:1;		//nastawa potencjometru PCO w zakresie zero
				unsigned _resPCO:1;		//nastawa potencjometru PCO na pozycji reset.	
				unsigned _newPCO:1;		//nastapila zmana pozycji potencjometru PCO w zakresie nastawy.
				unsigned char CO;		//temp. CO
				unsigned char PCO;		//nastawa CO
				unsigned char PPCO;		//docelowa wartosc temp CO (dla celow modulacji)
				unsigned char minPCO;	//minimalna wartosc nastawy CO
				unsigned char maxPCO;	//maksymalna wartosc nastawy CO
				unsigned char PGDPCO;	//obliczona wartosc nastawy PCO przy ogrzewaniu podlogowym
				signed char DCO;		//wielkosc roznicy PCO-CO
				signed char dDCO;		//szybkosc i kierunek zmian DCO
				unsigned char indCO;
				unsigned char SCO;		//przefiltrowana temp. CO
				unsigned char AC_CO;	//wartosc AC dla temp CO
				unsigned char MAD_CO;	//dolny prog dla procedury modulacji
				unsigned char DAD_CO;	//docalowa wartosc dla procedury modulacji

				unsigned char HstCo;	//histereza dla wylaczenia CO
				unsigned char F3PPCO;	//domyslna nastawa CO dla grzania CW w kotlach zasobnikowych
				}DataCO;
//---------------------------------------------------------- 
typedef struct	{
				unsigned _err_CW:1;		//awaria czujnika
				unsigned _su_CW:1;		//przekroczenie dolnej granicy zalaczenia
				unsigned _ad_CW:1;		//przekroczenie gornej granicy wylaczenia
				unsigned _wpz_CW:1;		//znacznik warunku pobudzenia dla zasobnika
				unsigned _on_DZ_CW:1;	//osiagnieto temp dyzurna CW
				unsigned _su_DZ_CW:1;	//przekroczenie dolnej granicy zalaczenia dyzurnej CW
				unsigned _ad_DZ_CW:1;	//przekroczenie gornej granicy wylaczenia dyzurnej CW		
				unsigned _cold_CW:1;	//zalacz pompe ponizej okreslonego progu temp.
				unsigned _newPCW:1;		//nastapila zmana pozycji potencjometru PCW w zakresie nastawy.				
				unsigned _wrtPCW:1;	
				unsigned _fpCW:1;
                unsigned _offKCW:1;     //nastapilo przekroczenie granicy powyzej ktorej zawor 3DR wysterowany jest tylko na obieg CO     
                
				unsigned char CW;		//temp. CW
				unsigned char PCW;		//nastawa CW
				unsigned char PPCW;		//docelowa wartosc temp CW (dla celow modulacji)
				unsigned char minPCW;	//minimalna wartosc nastawy CW
				unsigned char maxPCW;	//maksymalna wartosc nastawy CW
				signed char DCW;		//wielkosc roznicy PCW-CW
				signed char dDCW;		//szybkosc i kierunek zmian DCW
				signed char ddDCW;		//szybkosc i kierunek zmian DCW
				unsigned char indCW;
				unsigned char SCW;		//przefiltrowana temp. CW
				unsigned char AC_CW;	//wartosc AC dla temp CW
				unsigned char MAD_CW;	//dolny prog dla procedury modulacji
				unsigned char DAD_CW;	//docalowa wartosc dla procedury modulacji
				}DataCW;
//----------------------------------------------------------
typedef struct	{
				unsigned _on_ZW:1;		//jest czujnik temperatury zewnetrznej - aktywacja regulatora pogodowego
				unsigned _en_ZW:1;		//znacznik obecnosci czujnika temperatury zewnetrznej na potrzeby zewnetrznych regulatorow                
				unsigned char AC_ZW;	//wartosc AC dla temp. zewnetrznej
				signed char ZW;			//tempratura zewnwtrzna
				signed char SZW;		//przefiltrowana temp. ZW		
				}DataZW;	
//----------------------------------------------------------
typedef struct	{
				unsigned _err_IN:1;		//awaria czujnika
				unsigned _newECO:1;		//nastapila nastawy ECO				

				unsigned char AC_IN;	//wartosc AC dla temp. wlotowej
				unsigned char IN;		//tempratura wlotowa
				unsigned char DIN;		//rzeczywista wartosc delty OUT-IN
				unsigned char DIN2;		//rzeczywista wartosc delty NST-IN
				unsigned char DDLT;		//docelowa wartosc delty
				unsigned char cDLT;		//obliczona optymalna wartosc delty
				unsigned char cold;
				unsigned char kond;

				unsigned char Eco;		//nastawa wspolczynnika Eco
				unsigned char PPEco;	//wspolczynnik Eco
				unsigned char Eco5HI;	//gorna granica temp IN dla Eco=5
				unsigned char Eco5LO;	//dlona granica temp IN dla Eco=5
				unsigned char HI;		//obliczona gorna granica temp IN dla podanego Eco	
				unsigned char LO;		//obliczona gorna granica temp IN dla podanego Eco
				signed char delta1;		//delta wzgledem HI
				signed char delta2;		//delta wzgledem Eco
				signed char delta3;		//delta wzgledem LO
				unsigned char delta;	//wypadkowa wartosc delty

				unsigned char pop;		//poprawka
				}DataIN;		
//----------------------------------------------------------
typedef struct	{
				unsigned _newPFN:1;		//nastapila zmana pozycji
				unsigned _csn:1;		//pozycja CISNIENIE
				unsigned _off:1;		//pozycja OFF
				unsigned _lato:1;		//pozycja LATO
				unsigned _zima:1;		//pozycja ZIMA
				unsigned _res:1;
				unsigned poz;
				unsigned pop_poz;                
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
#endif	/*_POMIAR_TPH*/
