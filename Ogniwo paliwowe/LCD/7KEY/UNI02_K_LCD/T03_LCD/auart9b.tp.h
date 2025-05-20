/*auart9b.tp.h*/
//typy danych dla pliku auart9b.c
//----------------------------------------------------------
#ifndef	_AUART9b_TPH
#define	_AUART9b_TPH
//----------------------------------------------------------
				volatile bit UADtf_RData;				//znacznik odczytu danych
				volatile bit UADtf_REnd;				//znacznik konca odbioru
				volatile bit UADtf_RErrREC;				//blad odbioru	
				volatile bit UADtf_RErrBUF;				//blad przepelnienia bufora
				volatile bit UADtf_TData;				//znacznik transmisji danych
				volatile bit UADtf_TEnd;				//znacznik konca transmisji
                volatile bit UADtf_REXT;                //znacznik rozszerzonej liczby danych
                volatile bit UADtf_TEXT;                //znacznik rozszerzonej liczby danych                
typedef struct	{
				/*unsigned _RData:1;				//znacznik odczytu danych
				unsigned _REnd:1;				//znacznik konca odbioru
				unsigned _RErrREC:1;				//blad odbioru	
				unsigned _RErrBUF:1;				//blad przepelnienia bufora
				unsigned _TData:1;				//znacznik transmisji danych
				unsigned _TEnd:1;				//znacznik konca transmisji
                 */
				unsigned char indeks;			//indeks roboczy midulu UART		
				unsigned char RNData;			//liczba danych do odebrania
				unsigned char TNData;			//liczba danych do transmisji	
				unsigned char CHK;				//suma kontrolna bufora transmisji	
				}UARTData;
//----------------------------------------------------------
#endif /*_AUART9b_TPH*/
