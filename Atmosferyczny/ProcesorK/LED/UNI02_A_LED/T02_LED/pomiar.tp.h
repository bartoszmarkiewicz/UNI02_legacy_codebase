/*pomiar.tp.h*/
//typy danych dla plinu pomiar.c
//----------------------------------------------------------
#ifndef	_POMIAR_TPH
#define	_POMIAR_TPH
    volatile bit DtCOf_err_CO;		//awaria czujnika
    volatile bit DtCOf_su_CO;		//przekroczenie dolnej granicy zalaczenia
    volatile bit DtCOf_ad_CO;		//przekroczenie gornej granicy wylaczenia		
    volatile bit DtCOf_su_SRW_CO;	//przekroczenie dolnej granicy zalaczenia
    volatile bit DtCOf_ad_SRW_CO;	//przekroczenie gornej granicy wylaczenia		
    volatile bit DtCOf_on_DZ_CO;	//osiagnieto temp dyzurna CO
    volatile bit DtCOf_su_DZ_CO;	//przekroczenie dolnej granicy zalaczenia dyzurnej CO
    volatile bit DtCOf_ad_DZ_CO;	//przekroczenie gornej granicy wylaczenia dyzurnej CO		
    volatile bit DtCOf_ptwp_CO;	//temperaturowy punkt wylaczenia pompy
    volatile bit DtCOf_pz_CO;		//punkt zalaczenia od CO przy grzaniu CW
    volatile bit DtCOf_cold_CO;	//zalacz pompe ponizej okreslonego progu temp.
    volatile bit DtCOf_hot_CO;		//stan przegrzania
    volatile bit DtCOf_ldDCO;		//znacznik zbyt szybkiego narostu temperatury
    volatile bit DtCOf_zroPCO;		//nastawa potencjometru PCO w zakresie zero
    volatile bit DtCOf_resPCO;		//nastawa potencjometru PCO na pozycji reset.	
    volatile bit DtCOf_newPCO;		//nastapila zmana pozycji potencjometru PCO w zakresie nastawy.
//----------------------------------------------------------
typedef struct sDataCO 	{
    /*struct{                    
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

            unsigned int flags0;                                 
    };*/
    //unsigned int flags0;                

    volatile unsigned char CO;		//temp. CO
    volatile unsigned char PCO;		//nastawa CO
    volatile unsigned char PPCO;		//docelowa wartosc temp CO (dla celow modulacji)
    volatile unsigned char minPCO;	//minimalna wartosc nastawy CO
    volatile unsigned char maxPCO;	//maksymalna wartosc nastawy CO
    volatile unsigned char PGDPCO;	//obliczona wartosc nastawy PCO przy ogrzewaniu podlogowym
    volatile signed char DCO;		//wielkosc roznicy PCO-CO
    volatile signed char dDCO;		//szybkosc i kierunek zmian DCO
    volatile unsigned char indCO;
    volatile unsigned char SCO;		//przefiltrowana temp. CO
    volatile unsigned char AC_CO;	//wartosc AC dla temp CO
    volatile unsigned char MAD_CO;	//dolny prog dla procedury modulacji
    volatile unsigned char DAD_CO;	//docalowa wartosc dla procedury modulacji
}DataCO;
//---------------------------------------------------------- 
    volatile bit DtCWf_err_CW;		//awaria czujnika
    volatile bit DtCWf_su_CW;		//przekroczenie dolnej granicy zalaczenia
    volatile bit DtCWf_ad_CW;		//przekroczenie gornej granicy wylaczenia
    volatile bit DtCWf_wpz_CW;		//znacznik warunku pobudzenia dla zasobnika
    volatile bit DtCWf_on_DZ_CW;	//osiagnieto temp dyzurna CW
    volatile bit DtCWf_su_DZ_CW;	//przekroczenie dolnej granicy zalaczenia dyzurnej CW
    volatile bit DtCWf_ad_DZ_CW;	//przekroczenie gornej granicy wylaczenia dyzurnej CW		
    volatile bit DtCWf_cold_CW;	//zalacz pompe ponizej okreslonego progu temp.
    volatile bit DtCWf_newPCW;		//nastapila zmana pozycji potencjometru PCW w zakresie nastawy.				
    volatile bit DtCWf_wrtPCW;	
    volatile bit DtCWf_fpCW;
            
typedef struct sDataCW {
    /*struct{     
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

            unsigned int flags0;                                     
    };*/

    //unsigned int flags01;

    volatile unsigned char CW;		//temp. CW
    volatile unsigned char PCW;		//nastawa CW
    volatile unsigned char PPCW;		//docelowa wartosc temp CW (dla celow modulacji)
    volatile unsigned char minPCW;	//minimalna wartosc nastawy CW
    volatile unsigned char maxPCW;	//maksymalna wartosc nastawy CW
    volatile signed char DCW;		//wielkosc roznicy PCW-CW
    volatile signed char dDCW;		//szybkosc i kierunek zmian DCW
    volatile signed char ddDCW;		//szybkosc i kierunek zmian DCW
    volatile unsigned char indCW;
    volatile unsigned char SCW;		//przefiltrowana temp. CW
    volatile unsigned char AC_CW;	//wartosc AC dla temp CW
    volatile unsigned char MAD_CW;	//dolny prog dla procedury modulacji
    volatile unsigned char DAD_CW;	//docalowa wartosc dla procedury modulacji
}DataCW;              
//----------------------------------------------------------
    volatile bit DtZWf_on_ZW;		//jest czujnik temperatury zewnetrznej
            
typedef struct sDataZW	{
    /*struct{     
            unsigned _on_ZW:1;		//jest czujnik temperatury zewnetrznej

            unsigned int flags0;                                     
    };*/

    //unsigned int flags0;

    volatile unsigned char AC_ZW;	//wartosc AC dla temp. zewnetrznej
    volatile signed char ZW;			//tempratura zewnwtrzna
    volatile signed char SZW;		//przefiltrowana temp. ZW		
}DataZW;
//----------------------------------------------------------
    volatile bit DtINf_err_IN;		//awaria czujnika
    volatile bit DtINf_newECO;		//nastapila nastawy ECO
            
typedef struct sDataIN	{
    /*struct{     
            unsigned _err_IN:1;		//awaria czujnika
            unsigned _newECO:1;		//nastapila nastawy ECO	

            unsigned int flags0;                                     
    };*/

    //unsigned int flags0;

    volatile unsigned char AC_IN;	//wartosc AC dla temp. wlotowej
    volatile unsigned char IN;		//tempratura wlotowa
    volatile unsigned char DIN;		//rzeczywista wartosc delty OUT-IN
    volatile unsigned char DIN2;		//rzeczywista wartosc delty NST-IN
    volatile unsigned char DDLT;		//docelowa wartosc delty
    volatile unsigned char cDLT;		//obliczona optymalna wartosc delty
    volatile unsigned char cold;
    volatile unsigned char kond;

    volatile unsigned char Eco;		//nastawa wspolczynnika Eco
    volatile unsigned char PPEco;	//wspolczynnik Eco
    volatile unsigned char Eco5HI;	//gorna granica temp IN dla Eco=5
    volatile unsigned char Eco5LO;	//dlona granica temp IN dla Eco=5
    volatile unsigned char HI;		//obliczona gorna granica temp IN dla podanego Eco	
    volatile unsigned char LO;		//obliczona gorna granica temp IN dla podanego Eco
    volatile signed char delta1;		//delta wzgledem HI
    volatile signed char delta2;		//delta wzgledem Eco
    volatile signed char delta3;		//delta wzgledem LO
    volatile unsigned char delta;	//wypadkowa wartosc delty

    volatile unsigned char pop;		//poprawka
}DataIN;		
//----------------------------------------------------------
    volatile bit DtCSf_hi;			//zbyt wysokie cisnienie
    volatile bit DtCSf_lo;			//za niskie cisnienie
    volatile bit DtCSf_no;
    volatile bit DtCSf_error;		//brak czujnika - stan awaryjny
    volatile bit DtCSf_work;		//cisnienie w dozwolonym zakresie							
    volatile bit DtCSf_no_mont;		//brak czujnika - nie montowany
            
typedef struct sDataCSN {
    /*struct{     
            unsigned _hi:1;			//zbyt wysokie cisnienie
            unsigned _lo:1;			//za niskie cisnienie
            unsigned _no:1;
            unsigned _error:1;		//brak czujnika - stan awaryjny
            unsigned _work:1;		//cisnienie w dozwolonym zakresie							
            unsigned _no_mont:1;		//brak czujnika - nie montowany

            unsigned int flags0;                                     
    };*/

    //unsigned int flags0;  

    volatile unsigned char CS;		//wartosc cisnienia
}DataCSN;
//----------------------------------------------------------
#endif	/*_POMIAR_TPH*/
