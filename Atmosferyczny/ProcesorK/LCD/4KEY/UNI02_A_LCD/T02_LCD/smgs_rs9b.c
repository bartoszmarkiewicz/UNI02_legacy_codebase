/*smgs_rs.c*/
//Funkcje obslugi magistrali RS232

/*---------------------------------------------------------------------------------*/
#include <pic18.h>
#include "global.h"			//parametry globalne
#include "main.tp.h"
#include "ammy3.h"			
#include "uart9b.tp.h"
#include "smgs_rs9b.h"			//parametry lokalne

MGSRSData MGSD;
extern tpAMMY3 DtAMMY3;
extern UARTData UADt;
unsigned char xxy,zzz;
/*---------------------------------------------------------------------------------*/
//Rejestracja odebranych danych
void SaveRSToData(void)
{
	//0 - adres slave
	DtAMMY3.KOD=GetRSRBuf(1);
	DtAMMY3.STAT=GetRSRBuf(2);
	DtAMMY3.COmin=GetRSRBuf(3);
	DtAMMY3.COmax=GetRSRBuf(4);
	DtAMMY3.CWmin=GetRSRBuf(5);
	DtAMMY3.CWmax=GetRSRBuf(6);
	DtAMMY3.CSmin=GetRSRBuf(7);
	DtAMMY3.CSmax=GetRSRBuf(8);
	DtAMMY3.ZWmin=GetRSRBuf(9);
	DtAMMY3.ZWmax=GetRSRBuf(10);
	DtAMMY3.PRZmin=GetRSRBuf(11);
	DtAMMY3.PRZmax=GetRSRBuf(12);
	DtAMMY3.MPOB0=GetRSRBuf(13);
	DtAMMY3.MPOB1=GetRSRBuf(14);
	DtAMMY3.MPOT=GetRSRBuf(15);
	//16 - CHK
}
//Przygotowanie danych do wysylki
void SaveDataToRS(void)
{
	SetRSTBuf(0,_AMMY0_ADDR);	//0 - adres master
	//1	- potwierdzenie poprawnosci odebranej wczesniej paczki
	SetRSTBuf(2,_AMMY3_ADDR);	//2 - adres slave
	SetRSTBuf(3,DtAMMY3.KOD);
	SetRSTBuf(4,DtAMMY3.STAT);
	SetRSTBuf(5,DtAMMY3.CO);
	SetRSTBuf(6,DtAMMY3.CW);
	SetRSTBuf(7,DtAMMY3.CS);
	SetRSTBuf(8,DtAMMY3.ZW);
	SetRSTBuf(9,DtAMMY3.PRZ);
	SetRSTBuf(10,DtAMMY3.TPOB0);
	SetRSTBuf(11,DtAMMY3.TPOB1);
	SetRSTBuf(12,DtAMMY3.TPOB2);
	//13 - CHK (liczone po ustawieniu bajtu potwierdzenia)
}
/*void SaveDataToRS(void)
{
	SetRSTBuf(0,_AMMY0_ADDR);	//0 - adres master
	//1	- potwierdzenie poprawnosci odebranej wczesniej paczki
	SetRSTBuf(2,_AMMY3_ADDR);	//2 - adres slave
	//3 - CHK
}*/
//Przygotowanie odpowiedzi pozytywnej ukladu
void SetRSACK(void)
{
	SetRSTBuf(1,0xff);
}
//Przygotowanie odpowiedzi negatywnej ukladu
void ResetRSACK(void)
{
	SetRSTBuf(1,0x00);
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
//-------------------------------------------------------------------------------
//Timer na potrzeby transmisji (umiescic w przerwaniu od zegara TMR1)
void IncRSTimer2(void)
{
	if(MGSD.timRS2<255) MGSD.timRS2++;
}
//-------------------------------------------------------------------------------
//Restart zegara 
void StartRSTim2(void)
{
	MGSD.timRS2=0;
}
//-------------------------------------------------------------------------------
//Odczyt stanu zegara 
unsigned char RSTim2(void)
{
	return MGSD.timRS2;
}
/*---------------------------------------------------------------------------------*/
//Uruchomienie odbioru danych
void StartReceptData(void)
{
	MGSD._ReceptData=1;		//ustaw bit aktywacji proc. ReceptData
	MGSD.MTRS=0;
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//ODBIOR:
//0-adres,
//dane,
//chk,
//NADAWANIE
//0-adres master,
//1-potwierdzenie (0xff (pozytywne) lub 0x00 (negatywne koniec transmisji))
//2-adres slave
//dane
//chk
/*---------------------------------------------------------------------------------*/
//Realizacja transmisji danych do/z odbiornika
// tnDat - liczba bajtow wysylanych do odbiornika (razem z CHK jesli wystepuje),
// rnDat - liczba bajtow odpowiedzi odbiornika (razem z CHK jesli wystepuje).
/*---------------------------------------------------------------------------------*/
//Algorytm odbioru i nadawania z bajtem potwierdzenia
void ReceptData(const unsigned char rnDat, const unsigned char tnDat)
{
	switch(MGSD.MTRS)
	{
		case 0:
			MGSD.MTRS=1;
			if(!RCIDL) return;							//odczekaj na fizyczne zakonczenie odbioru
			CREN=0;										//wylacz modul odbioru
		case 1:
			MGSD._ReceptOK=1;							//znacznik potwierdzenia odebrania paczki danych/zakonczenie trans.
			if(VerifyRSCHK(_NRDAMMY0))					//poprawna paczka danych?
			{
				SaveRSToData();
				MGSD._ReceptACK=1;
				if(xxy++>99) xxy=0;
		 		if(tnDat) SetRSACK();					//pozytywna odpowiedz ukladu
			}	
			else 
			{
				MGSD._ReceptACK=0;
				//if(zzz++>99) zzz=0;
				if(tnDat) ResetRSACK();					//negatywna odpowiedz ukladu
			}
			if(tnDat)
			{
				SaveDataToRS();
				SetRSCHK(tnDat);							//oblicz i zapisz w ostatnim bajcie sume kontrolna
				StartUSARTTR(tnDat);						//wyslij pelna paczke danych
			}
			StartRSTim();
			MGSD.MTRS=2;
		case 2:
			if(tnDat)
			{
				if(TXIE)									//trwa nadawanie?
				{
					if(((RSTim()>1)&&!UADt.TData)||(RSTim()>_TS_TOUT))					//uplynol maksymalny czas oczekiwania na zak. nadawania?	
					{
						MGSD.MTRS=3;
						break;
					}
					break;
				}
			}
			MGSD._TERR_TIM=0;								//nie przekroczono limitu czas
			if(!TRMT)  return;								//odczekaj na fizyczne zakonczenie transmisji
			_TR_ODB=0;
			MGSD.MTRS=5;
			break;
		case 3:
			MGSD._ReceptOK=0;
			MGSD._TERR_TIM=1;								//przekroczony limit czasu dla transmisji
			StopUSARTTR();
			MGSD.MTRS=5;
			break;
		case 4:
			MGSD._ReceptOK=0;
			MGSD._ReceptACK=0;
			MGSD._RERR_TIM=1;								//przekroczony limit czasu dla odbioru
			MGSD.MTRS=5;
		case 5:
			MGSD._ReceptData=0;
			MGSD.MTRS=0;
			UADt.REnd=0;
			StartUSARTRC(rnDat);
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
			StartReceptData();					//wyzeruj procedure odbioru danych
			MGSD.KTR=1;
		case 1:
			if(UADt.REnd)
			{
				
				MGSD._CON=1;
				StartRSTim2();
				if(MGSD._ReceptData) 				//odbior w toku?
				{
					ReceptData(_NRDAMMY0,_NTDAMMY0);	//realizuj proces odbioru danych
					return;
				}
			}
			if(RSTim2()>100) MGSD._CON=0;
			//_SYG=0;
			//_TD=0;
			//if(MGSD._TERR_TIM||MGSD._RERR_TIM||MGSD._RERR_REC)	_ED=1;//blad transmisji?
			//else _ED=0;							//sygnalizacja bledu transmisji
			MGSD.KTR=0;
		break;
	}
}
/*---------------------------------------------------------------------------------*/
