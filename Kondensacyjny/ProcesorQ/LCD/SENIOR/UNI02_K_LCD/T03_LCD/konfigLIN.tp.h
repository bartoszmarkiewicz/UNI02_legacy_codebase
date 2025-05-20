/*konfig.tp.h*/
//typy danych dla pliku konfig.c
//----------------------------------------------------------
#ifndef	_KONFIG_LIN_TPH
#define	_KONFIG_LIN_TPH
//----------------------------------------------------------
#define _GET_MOD_PARAM 1
#define _EXIT_MOD_PARAM 0xff
//-------------------------------------------------------
typedef struct	{
    union   {
      unsigned char flag0;
      struct //__PACKED
      {      
         unsigned _keyBAR:1;	//ustawienie flagi oczekiwania na puszczenie przycisku SET                      
         unsigned _keyRES:1;    //ustawienie flagi oczekiwania na puszczenie przycisku OFF  
         unsigned fl0bt2:1;		//         
         unsigned fl0bt3:1;		//                       
         unsigned fl0bt4:1;		//                        
         unsigned fl0bt5:1;		//                        
         unsigned fl0bt6:1;		//     
         unsigned fl0bt7:7;		//            
      };
    };    
    unsigned char KNMODS;	//ogolna liczba parametrow    
    unsigned char KNPAR;	//ogolna liczba parametrow
    unsigned char MOD;      //nr modulu
    unsigned char XMOD;     //nr modulu + znacznik
    unsigned char PRM;		//nr. modyfikowanego parametru		
    unsigned char KRK;		//krok konfiguracji	
}DtKONF2;
//-------------------------------------------------------
typedef struct	{ 
				signed int min;	//dozwolone minmum
				signed int max;	//dozwolone maksimum
				signed int dta;	//wartosc zapamietana
				signed int bfdta;	//wartosc zmodyfikowana (zmienna dynamicznie)               
                union   {
                    unsigned char fl;
                    struct //__PACKED
                    {
                        unsigned exe:1;     //wykonywalne przy odzycie (zwracane 0-brak mozliwosci wykonania)
                        unsigned flt:1;     //liczba z kropka  (int=(float*10))             
                        unsigned shw:1;		//blokada wyswietlania parametru(0 - zablokowany)                        
                        unsigned P:1;		//znacznik wartosci procentowej /wyswietlania jednostki BAR                        
                        unsigned T:1;		//znacznik wyswietlania jednostki C                        
                        unsigned high:1;	//znacznik wyswietlania parametru na wyzszej pozycji (0 - mlodsze dwie cyfry)                        
                        unsigned no_msg:1;	//znacznik ze komunikaty o proc. awarynej nie ma byc wyswietlany w trakcie modyfikacji parametru;                        
                        unsigned rld:1;		//flaga zezwolenia na przescie z maksimum na minimum i odwrotnie                        
                    };
                };                
				}DtPZK2;
//-------------------------------------------------------
typedef struct	{
    
        union   {
         unsigned char flag0;
         struct //__PACKED
         {
            unsigned _getDta:1;    //ustawienie flagi oczekiwania na parametry konfiguracyjne z modulu automatyki            
            unsigned _clrREF:1;		//flaga zerowania licznika inREF                      
            unsigned fl0bt2:1;		//                       
            unsigned fl0bt3:1;		//                       
            unsigned fl0bt4:1;		//                        
            unsigned fl0bt5:1;		//                        
            unsigned fl0bt6:1;		//     
            unsigned fl0bt7:7;		//            
         };
     };
     
    //--------------------                
    //READ
    unsigned char inDTA[16];
    unsigned char bfIN[16];
    //--------------------    
    //WRITE    
    unsigned char outDTA[16];
    unsigned char bfOUT[16];
    //--------------------
    
    unsigned char inREF; 
    //--------------------                
    //read/write 1
    unsigned char inREF0;

    //read/write 2
    unsigned char inREF1;

    //read/write 3
    unsigned char inREF2;

    //read/write 4
    unsigned char inREF3;
                
    unsigned char nn;             
    //--------------------
    unsigned char KNMODS;           //liczba modulow automatyki                
    unsigned char FKNPAR;          //liczba parametrow pierwszego modulu

}tpMOD2;
//----------------------------------------------------------
//-------------------------------------------------------
#endif /*_KONFIG_LIN_TPH*/
