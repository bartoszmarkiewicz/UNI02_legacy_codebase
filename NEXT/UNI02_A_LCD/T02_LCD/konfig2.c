/*konfig.c*/
//Procedury zwiazane z wyborem typu gazu i poziomow
//mocy startowej oraz majksymalnej dla CO i CW
/*---------------------------------------------------------------------------------*/
#include <pic18.h>
#include <htc.h>
#include "global.h"					//parametry globalne
#include "konfig2.h"					//parametry lokalne
#include "eeprom.h"
#include "pomiar.h"
#include "wsw.h"
#include "main.tp.h"
#include "keypad.tp.h"
#include "print.h"
#include "mgs_rs9b.tp.h"			

DtKONF DtKNF;

extern unsigned char x,y;

extern MGSRSData MGSD;
extern tpMAIN M;
extern DataPFN PFN;
extern tpRSDTA1 RSDT1;
extern StPin ky[];
extern StPin xxky[];

DtPZK PZK[_KNPAR1]=
{
	0,0,0,0,0,0,0,1,				//moc startowa
	0,0,0,0,0,0,0,1,				//moc maksymalna CW
	0,0,0,0,0,0,0,1,				//moc maksymalna CO
	0,0,0,0,0,0,1,1,				//wybor rodzaju gazu
	0,0,0,0,0,0,1,1,				//aktywacja zegara antylegionella
	0,0,0,0,0,0,1,0,				//wybor typu kotla
	0,0,0,0,0,0,1,1					//aktywacja czujnika przeplywu
};
/*---------------------------------------------------------------------------------*/
//Odblokowanie procedury wejsciowej konfiguracji
//po uruchomieniu sterownika na pozycji OFF
//UEAGA: umiescic przed petla glowna
void InitKonf(void)
{
		DtKNF.KRK=0;
		DtKNF.PRM=0;
}
//wyjscie z procedury konfiguracji po modyfikacji parametrow
void ExitKonf(void)
{
	PFN._aktywne_KNF=0;
	EndPR();								//wygas wskaz parametru
	ClrAllMarks();							//wygas znaki konfiguracji
	DtKNF.PRM=0;
	DtKNF.KRK=0;
	WaitToLowEx(&ky[S_OFF]);
	WaitToLowEx(&ky[S_SET]);
	PFN._KnfDisErr=0;
}
unsigned int nrp=0;
unsigned char zm,zp;
void UpdateKnfPar(unsigned char par)
{
	switch(par)
	{
		case 0:
			RSDT1.inMSTR=PZK[0].dta;
		break;
		case 1:
			RSDT1.inMMAXU=PZK[1].dta;
		break;
		case 2:
			RSDT1.inMMAXG=PZK[2].dta;
		break;
		case 3:
			RSDT1.inRGZ=PZK[3].dta;
		break;
		case 4:
			RSDT1.inWANL=PZK[4].dta;
		break;
		case 5:
			RSDT1.inTPKT=PZK[5].dta;
		break;
		case 6:
			RSDT1.inWOBG=PZK[6].dta;
		break;
	}
}
//Glowna procedura konfiguracji (wykonywana w petli glownej)
void MKonfiguracja(void)
{
	unsigned char key;
	if(PFN._aktywne_KNF&&(DtKNF.KRK>=2)&&!PFN._opKNF) 					//procedura z jakiegos powodu nieaktywna w uk³adzie UNI-01?
	{
		ExitKonf();
		return;
	}
	if(PFN._aktywne_KNF&&((RTM(_RTKONF3)>=_TKNFEXT)||PFN._opgres))		//uplynol maksymalny dozwolony czas aktywnosci procedury lub wylaczenie awaryjne z blokada w ukladzie UNI-01?
	{
		ExitKonf();
		return;
	}
	//x=DtKNF.KRK;
	switch(DtKNF.KRK)
	{
		//warunek wejscia do procedury modyfikacji parametrow
		case 0:
		{
			RSDT1.inPRM=0;
			RSDT1.inKRK=0;
			if(!PFN._off) return;					//aktywny tryb OFF?
			if(MGSD._NO_ODB||PFN._toInit) return;	//problem z komunikacja?

			//przytrzymano przycisk wejscia do procedury konfiguracji
			key=PresKeyEX(RdPrt(S_OFF)&&RdPrt(S_SET),&ky[S_OFF_SET]);
			
			if(PFN._enKNF&&key!=100&&key>=50)
			{
				WaitToLowEx(&ky[S_OFF_SET]);			
				WaitToLowEx(&ky[S_OFF]);
				WaitToLowEx(&ky[S_SET]);
				PFN._aktywne_KNF=1;
				StartRTM(_RTKONF3);					//start timera aktywnosci proc. konfig
				DtKNF.PRM=0; 						//aktywacja pierwszego parametru
				DtKNF.KRK=1; 						//aktywacja proc. konfiguracji
				ClrAllMarks();						//wygas znaki nie zwiazane z konfiguracja
				StartRTS(_RTKONF1);					//start timera oczekiwania na wejsciowe dane konfiguracyjne
			}
			else 
			{
				DtKNF.PRM=0;
				DtKNF.KRK=0;
				return;
			}
			break;
		}
		//oczekiwanie na na wejsciowe dane konfiguracyjne
		case 1:
		{
			RSDT1.inPRM=0;
			RSDT1.inKRK=1;
			if((RTS(_RTKONF1)>=_TKNFINI)) 
			{
				ExitKonf();
				return;
			}
			if(RSDT1.RD_KNF&&PFN._opKNF)				//odczytaj dane wejsciowe parametrów ktore maj¹ byc modyfikowane
			{
				RSDT1.RRD_KNF=1;
				RSDT1.inMSTR=PZK[0].dta=PZK[0].bfdta=RSDT1.outMSTR;
				PZK[0].min=RSDT1.outMSTRmn;
				PZK[0].max=RSDT1.outMSTRmx;
				RSDT1.inMMAXU=PZK[1].dta=PZK[1].bfdta=RSDT1.outMMAXU;
				PZK[1].min=RSDT1.outMMAXUmn;
				PZK[1].max=RSDT1.outMMAXUmx;
				RSDT1.inMMAXG=PZK[2].dta=PZK[2].bfdta=RSDT1.outMMAXG;
				PZK[2].min=RSDT1.outMMAXGmn;
				PZK[2].max=RSDT1.outMMAXGmx;
				RSDT1.inRGZ=PZK[3].dta=PZK[3].bfdta=RSDT1.outRGZ;
				PZK[3].min=RSDT1.outRGZmn;
				PZK[3].max=RSDT1.outRGZmx;
				RSDT1.inWANL=PZK[4].dta=PZK[4].bfdta=RSDT1.outWANL;
				PZK[4].min=0;
				PZK[4].max=1;
				RSDT1.inTPKT=PZK[5].dta=PZK[5].bfdta=RSDT1.outTPKT;
				PZK[5].min=RSDT1.outTPKTmn;
				PZK[5].max=RSDT1.outTPKTmx;
				PZK[5].shw=GetBit2(RSDT1.outKSTAT0,_ZCFM);
				if(PZK[5].shw) DtKNF.PRM=nrp=5;				//aktywny parametr - wybor typu kotla
				else DtKNF.PRM=nrp=0;						//aktywny pierwszy parametr
				RSDT1.inWOBG=PZK[6].dta=PZK[6].bfdta=RSDT1.outWOBG;
				PZK[6].min=0;
				PZK[6].max=1;
				RSDT1.RRD_KNF=0;
 				DtKNF.KRK=2;
			}
			break;
		}
		//wybor parametru
		case 2:
		{
			RSDT1.inPRM=0;
			RSDT1.inKRK=2;
			PFN._off=1;
			PFN._lato=0;
			PFN._zima=0;
			PFN._KnfDisErr=1;
			if(PZK[nrp].shw)						//parametr mo¿e byc wyswietlony?
			{
				ReadParam(&nrp,&zm,&zp,0,_KNPAR1-1,1,0);
				DtKNF.PRM=nrp;
			}
			else
			{
				if(zm==3)						//zmiana w gore?
				{
					if(nrp<_KNPAR1-1) nrp++;
					else nrp=0;
				}
				else
				if(zm==2)						//zmiana w dol?
				{
					if(nrp>0) nrp--;
					else nrp=_KNPAR1-1;
					}
				else							//gdy pierwsze parametr ukryty						
				{
					if(nrp<_KNPAR1-1) nrp++;
					else nrp=0;
				}
				break;	
			}
			PrintPR(led_P,DtKNF.PRM+1,0);								//kod rx
			Mark(_GZ_MXCW,0);											
			Mark(_GZ_MNCO,0);
			key=PresKeyEX(RdPrt(S_SET),&ky[S_SET]);
			if(key>0)						//wybrano parametr
			{
				WaitToLowEx(&ky[S_SET]);		
				WaitToLowEx(&ky[S_OFF]);		
				EndPR();
				RSDT1.inDDTA=PZK[DtKNF.PRM].bfdta=PZK[DtKNF.PRM].dta;
				DtKNF.KRK=3;											//tryb modyfikacji parametru
			}
			key=PresKeyEX(RdPrt(S_OFF),&ky[S_OFF]);
			if(key!=100&&key>=20)
			{
				WaitToLowEx(&ky[S_OFF]);		
				StartRTS(_RTKONF1);										//start timera zapisu
				DtKNF.KRK=4;
			}
			break;
		}
		//modyfikacji wartosci parametru
		case 3:
		{
			RSDT1.inPRM=DtKNF.PRM;
			RSDT1.inKRK=3;
			PFN._off=0;
			PFN._lato=1;
			PFN._zima=0;
			PFN._KnfDisErr=PZK[DtKNF.PRM].no_msg;
			ReadParam(&(PZK[DtKNF.PRM].bfdta),&zm,&zp,PZK[DtKNF.PRM].min,PZK[DtKNF.PRM].max,0,PZK[DtKNF.PRM].rld);
			RSDT1.inDDTA=(char)PZK[DtKNF.PRM].bfdta;
			PrintWPR(PZK[DtKNF.PRM].bfdta/100,PZK[DtKNF.PRM].bfdta%100);
			Mark(_GZ_MXCW,PZK[DtKNF.PRM].bfdta>=PZK[DtKNF.PRM].max);
			Mark(_GZ_MNCO,PZK[DtKNF.PRM].bfdta<=PZK[DtKNF.PRM].min);
			key=PresKeyEX(RdPrt(S_SET),&ky[S_SET]);
			if(key>0)						//wybrano parametr
			{
				WaitToLowEx(&ky[S_SET]);
				PZK[DtKNF.PRM].zps=(PZK[DtKNF.PRM].dta!=PZK[DtKNF.PRM].bfdta);
				PZK[DtKNF.PRM].dta=PZK[DtKNF.PRM].bfdta;
				RSDT1.inDDTA=PZK[DtKNF.PRM].dta;
				UpdateKnfPar(DtKNF.PRM);
				DtKNF.KRK=2;
			}
			key=PresKeyEX(RdPrt(S_OFF),&ky[S_OFF]);
			if(key>0)
			{
				WaitToLowEx(&ky[S_OFF]);
				PZK[DtKNF.PRM].bfdta=PZK[DtKNF.PRM].dta;
				RSDT1.inDDTA=PZK[DtKNF.PRM].dta;
				UpdateKnfPar(DtKNF.PRM);								
				DtKNF.KRK=2;		
			}
			break;
		}
		//daj czas na zapis parametrow i wyjdz z proc. konfiguracji
		case 4:
		{
			RSDT1.inPRM=0;
			RSDT1.inKRK=4;
			if((RTS(_RTKONF1)>=_TKNFSAV)) 
			{
				ExitKonf();
				return;
			}
		}
	}
}
/*---------------------------------------------------------------------------------*/
