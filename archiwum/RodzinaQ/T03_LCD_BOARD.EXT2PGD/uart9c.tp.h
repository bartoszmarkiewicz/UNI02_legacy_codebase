/*uart9c.tp.h*/
//typy danych dla pliku uart9c.c
//----------------------------------------------------------
#ifndef	_UART9b_TPH
#define	_UART9b_TPH
//----------------------------------------------------------
typedef struct	{
				unsigned RData:1;				//znacznik odczytu danych
				unsigned REnd:1;				//znacznik konca odbioru
				unsigned RErrREC:1;				//blad odbioru	(frame, ovr ...
				unsigned RErrBUF:1;				//blad przepelnienia bufora
				unsigned RErrTim:1;				//blad przekroczenie limitu czasu
				unsigned TErrTim:1;				//blad przekroczenie limitu czasu
				unsigned TErrTRN:1;				//niemozliwosc podjecia nadawania
				unsigned TData:1;				//znacznik transmisji danych
				unsigned TEnd:1;				//znacznik konca transmisji
				unsigned char indeks;			//indeks roboczy midulu UART		
				unsigned char RNData;			//liczba danych do odebrania
				unsigned char TNData;			//liczba danych do transmisji	
				unsigned char CHK;				//suma kontrolna bufora transmisji	
				}UARTData;
//----------------------------------------------------------
#endif /*_UART9c_TPH*/
