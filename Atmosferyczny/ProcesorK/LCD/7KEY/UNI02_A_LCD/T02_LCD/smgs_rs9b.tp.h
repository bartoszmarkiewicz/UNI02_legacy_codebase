/*smgs_rs.h*/
//----------------------------------------------------------
#ifndef	_SMGS_TRS_H
#define	_SMGS_TRS_H

typedef struct	{
					unsigned _ReceptData:1;		//odbior w toku
					unsigned _ReceptDataTR:1;			//transmisja w toku		
					unsigned _ReceptDataRC:1;			//transmisja w toku				
					unsigned _ReceptOK:1;		//ogolny bit poprawnosci odbioru
					unsigned _ReceptACK:1;		//bit potwierdzenia odbioru danych od ukladu MASTER
					unsigned _TERR_TIM:1;		//timeout dla nadawania
					unsigned _RERR_TIM:1;		//timeout dla odbioru
					unsigned _RERR_REC:1;		//blad odbioru
					unsigned _CON:1;			//flaga polaczenia
					unsigned char timRS;		//timer o kroku 10 ms					
					unsigned char timRS2;		//timer o kroku 10 ms

					unsigned char MTRS;			//krok realizacji transmisji	
					unsigned char MTRS2;				//krok realizacji transmisji	
					unsigned char KTR;			//krok realizacji przesylu danych
				}MGSRSData;

//----------------------------------------------------------
#endif		/*_SMGS_TRS_H*/
