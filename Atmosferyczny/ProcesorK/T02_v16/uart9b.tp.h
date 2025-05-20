/*uart9b.tp.h*/
//typy danych dla pliku uart9b.c
//----------------------------------------------------------
#ifndef	_UART9b_TPH
#define	_UART9b_TPH
//----------------------------------------------------------
typedef struct	{
				unsigned RData:1;				//znacznik odczytu danych
				unsigned REnd:1;				//znacznik konca odbioru
				unsigned RErrREC:1;				//blad odbioru	
				unsigned RErrBUF:1;				//blad przepelnienia bufora
				unsigned TData:1;				//znacznik transmisji danych
				unsigned TEnd:1;				//znacznik konca transmisji
                unsigned REXT:1;                //znacznik rozszerzonej liczby danych
                unsigned TEXT:1;                //znacznik rozszerzonej liczby danych                
				unsigned char indeks;			//indeks roboczy midulu UART		
				unsigned char RNData;			//liczba danych do odebrania
				unsigned char TNData;			//liczba danych do transmisji	
				unsigned char CHK;				//suma kontrolna bufora transmisji	
				}UARTData;
//----------------------------------------------------------
#endif /*_UART9b_TPH*/
