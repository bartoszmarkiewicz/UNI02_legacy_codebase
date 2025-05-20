/*smgs_rs.c*/
//Funkcje obslugi magistrali RS232

/*---------------------------------------------------------------------------------*/
#include <pic18.h>
#include "global.h"			//parametry globalne
#include "main.tp.h"
#include "ammy1.h"			
#include "uart9b.tp.h"
#include "smgs_rs9b.h"			//parametry lokalne
#include "konfig.h"				

MGSRSData MGSD;
tpAMMY1 DtAMMY1;
extern UARTData UADt;
extern tpRSDTA1 RSDT1;
unsigned char xxy,zzz,vvv,mmm;
/*---------------------------------------------------------------------------------*/
//Rejestracja odebranych danych z modu³u LCD
void SaveRSToData(void)
{
	unsigned char idn;
	//0 - adres slave
	//1 - liczba przesylanych bajtow (ndat)
	//2 - identyfikator paczki danych
	RSDT1.outID=GetRSRBuf(2);
	switch(RSDT1.outID)
	{
		case _RD_STD:				//identyfikator paczki danych
		{
			RSDT1.RD_STD=0;
			RSDT1.inSTAT0=GetRSRBuf(3);
			RSDT1.inSTAT1=GetRSRBuf(4);
			RSDT1.inPCO=GetRSRBuf(5);
			RSDT1.inPCW=GetRSRBuf(6);
			RSDT1.RD_STD=1;
			break;
		}
		case _RD_KNF:				//identyfikator paczki danych
		{
			if(!RSDT1.RRD_KNF)		//aktywna blokada na czas interpretacji danych?
			{
				RSDT1.RD_KNF=0;
				RSDT1.inKSTAT0=GetRSRBuf(3);
				RSDT1.inKRK=GetRSRBuf(4);
				RSDT1.inPRM=GetRSRBuf(5);
				RSDT1.inDDTA=GetRSRBuf(6);
				RSDT1.inMSTR=GetRSRBuf(7);
				RSDT1.inMMAXU=GetRSRBuf(8);
				RSDT1.inMMAXG=GetRSRBuf(9);
				RSDT1.inRGZ=GetRSRBuf(10);
				RSDT1.inWANL=GetRSRBuf(11);
				RSDT1.inTPKT=GetRSRBuf(12);
				RSDT1.inWOBG=GetRSRBuf(13);
				RSDT1.inOPDG=GetRSRBuf(14);
				RSDT1.inTPCS=GetRSRBuf(15);
				RSDT1.inRDPM=GetRSRBuf(16);
				RSDT1.inDTPM=GetRSRBuf(17);
				RSDT1.inMDPM=GetRSRBuf(18);
				RSDT1.RD_KNF=1;
			}
			break;
		}
	}
	//ndat - CHK
}
/*---------------------------------------------------------------------------------*/
//Przygotowanie danych do wysylki do modulu LCD
void SaveDataToRS(unsigned char tDTA)
{
	unsigned char ndat;		//liczba danych wysy³anych + CHK
	unsigned char chkndat;

	//const unsigned char ndat=19;		//liczba danych wysy³anych (+ CHK)
	//unsigned char chkndat;
	switch(tDTA)
	{
		case _RD_ERR:							//identyfikator paczki danych
		{
			ndat=4;								//liczba danych wysy³anych (+ CHK)
			chkndat=SetNDatCHK(ndat);			//oblicz CHK dla liczby danych
		
			SetRSTBuf(0,_LCD_ADDR);				//0 - adres master
			SetRSTBuf(1,chkndat);				//1 - liczba przesy³anych danych (ndat)
			SetRSTBuf(2,_RD_ERR);				//2 - identyfikator paczki danych
			//ndat - CHK
			break;
		}
		case _RD_STD:				//identyfikator paczki danych
		{
			ndat=24;		//liczba danych wysy³anych (+ CHK)
			chkndat=SetNDatCHK(ndat);			//oblicz CHK dla liczby danych
		
			SetRSTBuf(0,_LCD_ADDR);				//0 - adres master
			SetRSTBuf(1,chkndat);				//1 - liczba przesy³anych danych (ndat)
			SetRSTBuf(2,_RD_STD);				//3 - identyfikator paczki danych
			SetRSTBuf(3,RSDT1.outRDZ0);
			SetRSTBuf(4,RSDT1.outRDZ1);
			SetRSTBuf(5,RSDT1.outSTAT0);
			SetRSTBuf(6,RSDT1.outSTAT1);
			SetRSTBuf(7,RSDT1.outSTAT2);
			SetRSTBuf(8,RSDT1.outSTAT3);
			SetRSTBuf(9,RSDT1.outCO);
			SetRSTBuf(10,RSDT1.outCW);
			SetRSTBuf(11,RSDT1.outCS);
			SetRSTBuf(12,RSDT1.outIN);
			SetRSTBuf(13,RSDT1.outPCO);
			SetRSTBuf(14,RSDT1.outPCOmn);
			SetRSTBuf(15,RSDT1.outPCOmx);
			SetRSTBuf(16,RSDT1.outPCW);
			SetRSTBuf(17,RSDT1.outPCWmn);
			SetRSTBuf(18,RSDT1.outPCWmx);
			SetRSTBuf(19,RSDT1.outPPCO);
			SetRSTBuf(20,RSDT1.outPPCW);
			SetRSTBuf(21,RSDT1.outERR);
			SetRSTBuf(22,RSDT1.outAWR);
			SetRSCHK(ndat);			//ndat - CHK z bajtow [0..ndat] i zapisuje na poz. ndat
			//ndat - CHK
			break;
		}
		case _RD_KNF:				//identyfikator paczki danych
		{
			ndat=27;		//liczba danych wysy³anych (+ CHK)
			chkndat=SetNDatCHK(ndat);			//oblicz CHK dla liczby danych
		
			SetRSTBuf(0,_LCD_ADDR);				//0 - adres master
			SetRSTBuf(1,chkndat);				//1 - liczba przesy³anych danych (ndat)
			SetRSTBuf(2,_RD_KNF);				//2 - identyfikator paczki danych
			SetRSTBuf(3,RSDT1.outKSTAT0);
			SetRSTBuf(4,RSDT1.outMSTR);
			SetRSTBuf(5,_KMINMST);
			SetRSTBuf(6,_KMAXMST);
			SetRSTBuf(7,RSDT1.outMMAXU);
			SetRSTBuf(8,_KMINMCW);
			SetRSTBuf(9,_KMAXMCW);
			SetRSTBuf(10,RSDT1.outMMAXG);
			SetRSTBuf(11,_KMINMCO);
			SetRSTBuf(12,_KMAXMCO);
			SetRSTBuf(13,RSDT1.outRGZ);
			SetRSTBuf(14,_KMINRGZ);
			SetRSTBuf(15,_KMAXRGZ);
			SetRSTBuf(16,RSDT1.outWANL);
			SetRSTBuf(17,RSDT1.outTPKT);
			SetRSTBuf(18,_KMINWTK);
			SetRSTBuf(19,_KMAXWTK);
			SetRSTBuf(20,RSDT1.outWOBG);
			SetRSTBuf(21,RSDT1.outOPDG);
			SetRSTBuf(22,RSDT1.outTPCS);
			SetRSTBuf(23,RSDT1.outRDPM);
			SetRSTBuf(24,RSDT1.outDTPM);
			SetRSTBuf(25,RSDT1.outMDPM);
			SetRSCHK(ndat);			//ndat - CHK z bajtow [0..ndat] i zapisuje na poz. ndat
			//ndat - CHK
			break;
		}
	}
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
//-------------------------------------------------------------------------------
//Inicjalizacja danych zwiazanych z transmisj¹ UART
void InitReceptProc(void)
{
	MGSD._ReceptData=0;
	MGSD._ReceptOK=0;
	MGSD._ReceptACK=0;
	MGSD._TERR_TIM=0;
	MGSD._RERR_TIM=0;
	MGSD._RERR_REC=0;
	MGSD._CON=0;
	MGSD.timRS=0;
	MGSD.timRS2=0;
	MGSD.MTRS=0;
	MGSD.KTR=0;
}
//-------------------------------------------------------------------------------
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

void ReceptData(const unsigned char tnDat)
{
	switch(MGSD.MTRS)
	{
		case 0:
			MGSD.MTRS=1;
			if(!RCIDL) return;							//odczekaj na fizyczne zakonczenie odbioru
		case 1:
			MGSD._ReceptOK=1;							//znacznik potwierdzenia odebrania paczki danych/zakonczenie trans.
			CREN=0;										//blokada odbioru na czas analizy danych
			if(VerifyRSCHK(UADt.RNData))				//poprawna paczka danych?
			{
				SaveRSToData();							//zapisz dane w pamieci
				MGSD._ReceptACK=1;
				MGSD._CON=1;							//*************************ustaw znacznik polaczenia
				StartRSTim2();							//**********************zeruj zegar detekcji polaczenia
				//if(xxy++>99) xxy=0;
			}	
			else 
			{
				MGSD._ReceptACK=0;
			}
			CREN=1;
			if(tnDat)
			{
				if(MGSD._ReceptACK) SaveDataToRS(RSDT1.outID);
				else SaveDataToRS(_RD_ERR);
				StartUSARTTR();				//wyslij pelna paczke danych
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
			MGSD.MTRS=5;
			break;
		case 3:
			//if(zzz++>99) zzz=0;
			MGSD._ReceptOK=0;
			MGSD._TERR_TIM=1;								//przekroczony limit czasu dla transmisji
			StopUSARTTR();
			MGSD.MTRS=5;
			break;
		case 4:
			//if(zzz++>99) zzz=0;
			MGSD._ReceptOK=0;
			MGSD._ReceptACK=0;
			MGSD._RERR_TIM=1;								//przekroczony limit czasu dla odbioru
			MGSD.MTRS=5;
		case 5:
			MGSD._ReceptData=0;
			MGSD.MTRS=0;
			UADt.REnd=0;
			//StartUSARTRC();
			break;			
	}
}
/*---------------------------------------------------------------------------------*/
//Odbior danych z modulu LCD (procedure umiescic w petli glownej programu)
void ReceptDataLCD(void)
{
	switch(MGSD.KTR)
	{
		case 0:
			StartReceptData();					//wyzeruj procedure odbioru danych
			MGSD.KTR=1;
		case 1:
			if(UADt.REnd)
			{
				//MGSD._CON=1;//**********************************
				//StartRSTim2();//********************************
				if(MGSD._ReceptData) 				//odbior w toku?
				{
					ReceptData(1);	//realizuj proces odbioru danych
					return;
				}
			}
			if(RSTim2()>200) MGSD._CON=0;
			MGSD.KTR=0;
		break;
	}
}
/*---------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------*/
