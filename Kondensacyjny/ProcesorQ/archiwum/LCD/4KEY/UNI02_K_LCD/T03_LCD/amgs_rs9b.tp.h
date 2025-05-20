/*amgs_rs.h*/
//----------------------------------------------------------
#ifndef	_AMGS_TRS_H
#define	_AMGS_TRS_H
					volatile bit MGSDf_TransData;			//transmisja w toku
					volatile bit MGSDf_TransDataTR;			//transmisja w toku		
					volatile bit MGSDf_TransDataRC;			//transmisja w toku				
					volatile bit MGSDf_TransOK;			//ogolny bit poprawnosci transmisji
					volatile bit MGSDf_TransACK;			//bit potwierdzenia odbioru danych od odbiornika
					volatile bit MGSDf_TERR_TIM;			//timeout dla nadawania
					volatile bit MGSDf_RERR_TIM;			//timeout dla odbioru
					volatile bit MGSDf_RERR_REC;			//blad odbioru
					volatile bit MGSDf_ERRt;				//bit sygnalizujacy ogolny blad transmisji
					volatile bit MGSDf_NO_ODB;				//znacznik braku odbiornika

typedef struct	{
					/*unsigned _TransData:1;			//transmisja w toku
					unsigned _TransDataTR:1;			//transmisja w toku		
					unsigned _TransDataRC:1;			//transmisja w toku				
					unsigned _TransOK:1;			//ogolny bit poprawnosci transmisji
					unsigned _TransACK:1;			//bit potwierdzenia odbioru danych od odbiornika
					unsigned _TERR_TIM:1;			//timeout dla nadawania
					unsigned _RERR_TIM:1;			//timeout dla odbioru
					unsigned _RERR_REC:1;			//blad odbioru
					unsigned _ERRt:1;				//bit sygnalizujacy ogolny blad transmisji
					unsigned _NO_ODB:1;				//znacznik braku odbiornika
                     */
					unsigned char timRS;			//timer o kroku 10 ms					
					unsigned char MTRS;				//krok realizacji transmisji	
					unsigned char MTRS2;				//krok realizacji transmisji	
					unsigned char KTR;				//krok realizacji przesylu danych
					unsigned char LCN;				//licznik powtorzen transmisji w razie niepowodzenia					
				}MGSRSData;

//----------------------------------------------------------
#endif		/*_AMGS_TRS_H*/
