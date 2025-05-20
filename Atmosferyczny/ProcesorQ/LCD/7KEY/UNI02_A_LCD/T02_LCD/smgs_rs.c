/*smgs_rs.c*/
//Funkcje obslugi magistrali RS232

/*---------------------------------------------------------------------------------*/
#include <pic18.h>
#include "global.h"			//parametry globalne
#include "main.tp.h"
#include "ammy1.h"			
#include "uart.tp.h"
#include "smgs_rs.h"			//parametry lokalne

MGSRSData MGSD;
extern tpAMMY DtAMMY;
extern tpAMMY1 DtAMMY1;
extern UARTData UADt;

/*---------------------------------------------------------------------------------*/
//Rejestracja odebranych danych
void SaveRSToData(void)
{
	DtAMMY1.KOD=GetRSBuf(1);
	DtAMMY1.STAT=GetRSBuf(2);
	DtAMMY1.CO=GetRSBuf(3);
	DtAMMY1.CW=GetRSBuf(4);
	DtAMMY1.CS=GetRSBuf(5);
	DtAMMY1.MOD=GetRSBuf(6);
	DtAMMY1.TPRZ_CW=GetRSBuf(7);
	DtAMMY1.IMPS_CW=GetRSBuf(8);
	DtAMMY1.TPOB0=GetRSBuf(9);
	DtAMMY1.TPOB1=GetRSBuf(10);
	DtAMMY1.TPOB2=GetRSBuf(11);
	DtAMMY1.limit=GetRSBuf(12);
	DtAMMY1.etap=GetRSBuf(13);
}
//Przygotowanie odpowiedzi ukladu do wysylki
void SaveDataToRS(void)
{
	SetRSBuf(0,MGSD.CHK);		//1 bajt odpowiedzi (suma kontrolna)
}
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//Timer na potrzeby transmisji (umiescic w przerwaniu od zegara TMR1)
void IncRSTimer(void)
{
	if(MGSD.timRS<255) MGSD.timRS++;
}
//-------------------------------------------------------------------------------
//Restart zegara 
void StartRSTim(void)
{
	MGSD.timRS=0;
}
//-------------------------------------------------------------------------------
//Odczyt stanu zegara 
unsigned char RSTim(void)
{
	return MGSD.timRS;
}
/*---------------------------------------------------------------------------------*/
//Uruchomienie odbioru danych
void StartReceptData(void)
{
	MGSD._ReceptData=1;		//ustaw bit aktywacji proc. ReceptData
	MGSD.MTRS=0;
}
/*---------------------------------------------------------------------------------*/
//Realizacja transmisji danych do/z odbiornika
//gdzie:
// tnDat - liczba bajtow wysylanych do odbiornika (razem z CHK jesli wystepuje),
// rnDat - liczba bajtow odpowiedzi odbiornika (razem z CHK jesli wystepuje).
void ReceptData(const unsigned char rnDat, const unsigned char tnDat)
{
	switch(MGSD.MTRS)
	{
		case 0:
			ClrRSBuf();					//wyzeruj bufor danych
			StartUSARTRC(rnDat);		//start odbioru
			if(!UADt.RData)				//udalo sie rozpoczac odbior? 		
			{
				MGSD.MTRS=8;
				break;
			}else MGSD._RERR_REC=0;
			StartRSTim();
			MGSD.MTRS=1;
		case 1:
			if(UADt.RData)				//trwa odbior?
			{
				if(RSTim()>_RS_TOUT)	
				{
					MGSD.MTRS=6;
					break;
				}
				break;
			}
			MGSD._RERR_TIM=0;
			if(GetRSBuf(0)==_ADR_AMMY1)				//poprawny adres odbiornika?
			{
				MGSD._RIgnore=0;
				MGSD.CHK=LiczRSCHK(rnDat-1);
				if(MGSD.CHK==GetRSBuf(rnDat-1))
				{
					MGSD._RERR_CHK=0;
					MGSD._ReceptOK=1;				//znacznik potwierdzenia odebrania paczki danych/zakonczenie trans.
					SaveRSToData();					//tutaj zapis danych do odp. zmiennych
					ClrRSBuf();
					if(tnDat) SaveDataToRS();		//przygotuj odpowiedz
				}
				else
				{
					MGSD._ReceptOK=0;
					MGSD.MTRS=4;					//zasygnalizuj blad chk
					break;
				}
			}
			else
			{
				MGSD._ReceptOK=0;
				MGSD._RIgnore=1;					//zignoruj dane adresowane do innego odbiornika
				MGSD.MTRS=9;
				break;
			}
			StartRSTim();
			MGSD.MTRS=2;
		case 2:
			if(tnDat)
			{
				if(RSTim()<_TR_PAUSE) break;				//czas na przygotowanie sie ukladu master na odbior
				StartUSARTTR(tnDat);						//start transmisji bajtu potwierdzenia
				if((tnDat-1)&&!UADt.TData)					//udalo sie rozpoczac transmisje? 		
				{
					MGSD.MTRS=7;
					break;
				}else MGSD._TERR_TRN=0;
				StartRSTim();
			}
			MGSD.MTRS=3;
		case 3:
			if(tnDat)
			{
				if(UADt.TData)					//trwa nadawanie?
				{
					if(RSTim()>_TS_TOUT)	
					{
						MGSD.MTRS=5;
						break;
					}
					break;
				}
			}
			MGSD._TERR_TIM=0;
			MGSD.MTRS=9;
			break;
		case 4:
			MGSD._RERR_CHK=1;				//blad CHK
			MGSD.MTRS=9;
			break;
		case 5:
			MGSD._TERR_TIM=1;				//przekroczony limit czasu dla transmisji
			StopUSARTTR();
			MGSD.MTRS=9;
			break;
		case 6:
			MGSD._RERR_TIM=1;				//przekroczony limit czasu dla odbioru
			StopUSARTRC();
			MGSD.MTRS=9;
			break;
		case 7:
			MGSD._TERR_TRN=1;				//niemozliwosc podjecia transmisji
			StopUSARTTR();
			MGSD.MTRS=9;
			break;
		case 8:
			MGSD._RERR_REC=1;				//niemozliwosc podjecia odbioru
			StopUSARTRC();
			MGSD.MTRS=9;
		case 9:	
			MGSD._ReceptData=0;
			MGSD.MTRS=0;
			break;			
	}
}

/*---------------------------------------------------------------------------------*/
//Odbior danych z modulu AMMY0 (procedure umiescic w petli glownej programu)
void ReceptDataAMMY0(void)
{
	switch(MGSD.KTR)
	{
		case 0:
			if(!_SBT) return;					//czekaj na pojawienie sie sygnalu synchronizacji
			StartReceptData();					//wyzeruj procedure odbioru danych
			MGSD.KTR=1;
		case 1:
			if(_SBT) return;					//czekaj na zanik sygnalu synchronizacji
			MGSD.KTR=2;
			_TD=1;
		case 2:
			if(MGSD._ReceptData) 				//odbior w toku?
			{
				ReceptData(_NRDAMMY1,_NTDAMMY1);	//realizuj proces odbioru danych
				return;
			}
			_TD=0;
			if(MGSD._TERR_TRN||MGSD._TERR_TIM||MGSD._RERR_TIM||MGSD._RERR_REC||MGSD._RERR_CHK)	_ED=1;//blad transmisji?
			else _ED=0;							//sygnalizacja bledu transmisji
			MGSD.KTR=0;
		break;
	}
}
/*---------------------------------------------------------------------------------*/
