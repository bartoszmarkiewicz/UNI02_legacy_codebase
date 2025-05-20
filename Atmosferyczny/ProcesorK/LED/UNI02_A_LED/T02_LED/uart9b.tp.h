/*uart9b.tp.h*/
//typy danych dla pliku uart9b.c
//----------------------------------------------------------
#ifndef	_UART9b_TPH
#define	_UART9b_TPH
//----------------------------------------------------------
    volatile bit UADtf_RData;				//znacznik odczytu danych
    volatile bit UADtf_REnd;				//znacznik konca odbioru
    volatile bit UADtf_RErrREC;				//blad odbioru	
    volatile bit UADtf_RErrBUF;				//blad przepelnienia bufora
    volatile bit UADtf_TData;				//znacznik transmisji danych
    volatile bit UADtf_TEnd;				//znacznik konca transmisji

typedef struct	sUARTData{
    /*struct{
        unsigned RData:1;				//znacznik odczytu danych
        unsigned REnd:1;				//znacznik konca odbioru
        unsigned RErrREC:1;				//blad odbioru	
        unsigned RErrBUF:1;				//blad przepelnienia bufora
        unsigned TData:1;				//znacznik transmisji danych
        unsigned TEnd:1;				//znacznik konca transmisji
        
        unsigned int flags0;
    };*/
                
    volatile unsigned char indeks;			//indeks roboczy midulu UART		
    volatile unsigned char RNData;			//liczba danych do odebrania
    volatile unsigned char TNData;			//liczba danych do transmisji	
    volatile unsigned char CHK;				//suma kontrolna bufora transmisji	
}UARTData;
//----------------------------------------------------------
#endif /*_UART9b_TPH*/
