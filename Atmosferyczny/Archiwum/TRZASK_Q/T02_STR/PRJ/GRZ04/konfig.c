/*konfig.c*/
//Procedury zwiazane z wyborem typu gazu i poziomow
//mocy startowej oraz majksymalnej dla CO i CW
/*---------------------------------------------------------------------------------*/
#include <xc.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include "../../proj.tp.h"

#ifndef _TYP_PRJ
#error NIEOKRESLONY IDENTYFIKATOR PROJEKTU
#endif

#if _TYP_PRJ==4
#advisory _TYP_PRJ==4 
#include "../../mcc_generated_files\mcc.h"

#include "global.h"
#include "main.h"
#include "global.ex.h"

#include "konfig.h"		//parametry lokalne		

extern tpMAIN M;
extern DataPFN PFN;
extern DataCW DtCW;

DtKONF DtKNF;
unsigned char bfrs;		//znacznik pierwszego przycisniecia reset

unsigned char wf=0;
/*---------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------*/
//Odblokowanie procedury wejsciowej konfiguracji
//po uruchomieniu sterownika na pozycji OFF
//UEAGA: umiescic przed petla glowna
void EnKonf(void)
{
	bfrs=0;
	DtKNF.KRK=0;
	StartRTS(_RTKONF1);
	DtKNF.enknf=1;		//odblokuj mozliwosc wejscia do konf.
}
//wyjscie z procedury konfiguracji po modyfikacji parametrow
void ExitKonf(void)
{
	M._AKTYWNE_KNF=0;
	DtKNF.wymCW=0;
	EndPR();								//wygas wskaz parametru
	DtKNF.enknf=0;							//zablokuj mozliwosc wejscia do konf.
	DtKNF.PRM=1;
	DtKNF.KRK=0;
	bfrs=0;
	RdEEParam();							//odczytaj parametry z pamieci EEPROM
	PomiarPCW();
	DtCW._newPCW=0;
	DtCW._wrtPCW=0;
}
//Glowna procedura konfiguracji (wykonywana w petli glownej)
void MKonfiguracja(void)
{
	unsigned char key=0,prm=0,rgz=0,mstr=1,mxco=1,mxcw=1;
	static unsigned char zustr; 

	if(!DtKNF.enknf) return;						//procedura konf. zablokowana?
	if(DtKNF.KRK&&(RTM(_RTKONF3)>=_TKNFEXT))			//uplynol maksymalny dozwolony czas aktywnosci procedury?
	{
		ExitKonf();
		return;
	}
	switch(DtKNF.KRK)
	{
		//warunek wejscia do procedury modyfikacji parametrow
		case 0:
		{
			//deaktywacja proc. przy braku operacji lub po zmianie polozenia przelacznika
			if((RTS(_RTKONF1)>=_TKNFDIS))				//uplynal czas mozliwosci wejscia do konf.?
			{
				bfrs=0;
				DtKNF.enknf=0;							//zablokuj mozliwosc wejscia do konf.
				M._AKTYWNE_KNF=0;
				return;
			}
			//nacisnieto przycisk RESET
			if(!bfrs&&RdPrt(S_KUP)&&RdPrt(S_KDW))		//nacisnieto oba przyciski MIN i MAX?
			{
				bfrs=1;
				StartRTS(_RTKONF2);						//start timera przytrzymania obu przyciskow MIN i MAX
			}
			//przytrzymano przycisk RESET
			if(bfrs&&RdPrt(S_KUP)&&RdPrt(S_KDW))					//przytrzymany przycisk RESET?
			{
				if(RTS(_RTKONF2)>=_TKNFENT)
				{
					
					M._AKTYWNE_KNF=1;
					DtKNF.PRM=1; 						//aktywacja pierwszego parametru
					DtKNF.KRK=1; 						//aktywacja proc. konfiguracji
					StartRTM(_RTKONF3);					//start timera aktywnosci proc. konfig
					SetBufKey1();
					SetBufKey2();
				}
			}
			else 
			{
				DtKNF.PRM=1;
				DtKNF.KRK=0;
				bfrs=0; 
				return;
			}
			break;
		}
		//wybor parametru
		case 1:
		{
			if(PresKey2(RdPrt(S_KUP))) 
			{
				DtKNF.PRM++;
				if(DtKNF.PRM>_KNPAR1) DtKNF.PRM=1;
			}
			if(DtKNF.PRM<_KNPAR1) PrintPR(led_r,DtKNF.PRM,0);			//kod rx
			else PrintPR(led_E,led_n,1);								//kod En
			if(DtKNF.PRM==_KNPAR1)										//ostatni parametr (En)?
			{
				if(PresKey1(RdPrt(S_KDW)))								//wyjscie z procedury konfiguracji
				{
					DtKNF.KRK=2;
					DtKNF.PRM=0;
				}
			}
			else
			if(DtKNF.PRM!=_KNPAR1)										//parametr rx?
			{
				if(PresKey1(RdPrt(S_KDW)))
				{
					SetBufKey1();
 					DtKNF.KRK=2;										//tryb modyfikacji parametru
				}
			}
			break;
		}
		//modyfikacji wartosci parametru
		case 2:
		{
			switch(DtKNF.PRM)
			{
				//wyjscie z procedury
				case 0:
				{
					if(!RdPrt(S_KDW)) 
					{
						ExitKonf();
						return;
					}
					break;
				}	
				//moc startowa (0...99)
				case 1:
				{
					if(!ReadDataXONL(RdPrt(S_KUP),&DtKNF.mstr,_KMINMST,_KMAXMST,1,1,PresKey1(RdPrt(S_KDW))))
					{
						PrintWPR(DtKNF.mstr);
						MocStart(DtKNF.mstr);
						if(RdPrt(S_WCW)) DtKNF.wymCW=1;			//zalacz wymuszenie CW dla aktywnego przeplywu
						else DtKNF.wymCW=0;						//zalacz wymuszenie CW
					}
					else 
					{
						DtKNF.wymCW=0;							//wylacz wymuszenie CW								//wylacz wymuszenie CO
						if_WriteEEPROM(_ADRMST,DtKNF.mstr);
						DtKNF.KRK=1;		
					}
					break;
				}	
				//moc maksymalna WU (0...99)
				case 2:
				{
					if(!ReadDataXONL(RdPrt(S_KUP),&DtKNF.mxcw,_KMINMCW,_KMAXMCW,1,1,PresKey1(RdPrt(S_KDW))))
					{
						PrintWPR(DtKNF.mxcw);
						MocMaksCW(DtKNF.mxcw);
						if(RdPrt(S_WCW)) DtKNF.wymCW=1;		//zalacz wymuszenie CW dla aktywnego przeplywu
						else DtKNF.wymCW=0;
					}
					else 
					{
						DtKNF.wymCW=0;							//wylacz wymuszenie CW
						if_WriteEEPROM(_ADRMCW,DtKNF.mxcw);
						DtKNF.KRK=1;		
					}
					break;
				}
				//rodzaj gazu (0/1)
				case 3:
				{
					if(!ReadDataONL(RdPrt(S_KUP),&DtKNF.rdgz,_KMINRGZ,_KMAXRGZ,1,PresKey1(RdPrt(S_KDW))))
						PrintWPR(DtKNF.rdgz);
					else 
					{
						if_WriteEEPROM(_ADRRGZ,DtKNF.rdgz);
						ModParam(DtKNF.tpkt,DtKNF.rdgz);		//nowe parametry modulatora dla wybranego typu kotla i rodzaju gazu
						DtKNF.KRK=1;		
					}
					break;
				}
				//minimum predkosci dla wentylatora (5/99)
				case 4:
				{
					if(!ReadDataONL(RdPrt(S_KUP),&DtKNF.minv,_KMINMNV,DtKNF.maxv,1,PresKey1(RdPrt(S_KDW))))
						PrintWPR(DtKNF.minv);
					else 
					{
                        if(DtKNF.minv>DtKNF.maxv) DtKNF.minv=DtKNF.maxv;
						if_WriteEEPROM(_ADRMNV,DtKNF.minv);
						DtKNF.KRK=1;		
					}
					break;
				}  
				//maksimum predkosci dla wentylatora (5/99)
				case 5:
				{
					if(!ReadDataONL(RdPrt(S_KUP),&DtKNF.maxv,DtKNF.minv,_KMAXMXV,1,PresKey1(RdPrt(S_KDW))))
						PrintWPR(DtKNF.maxv);
					else 
					{
                        if(DtKNF.maxv<DtKNF.minv) DtKNF.maxv=DtKNF.minv;
						if_WriteEEPROM(_ADRMXV,DtKNF.maxv);
						DtKNF.KRK=1;		
					}
					break;
				} 
				//startowa predkosc dla wentylatora (5/99)
				case 6:
				{
#if _TEST_PWM==0
					if(!ReadDataONL(RdPrt(S_KUP),&DtKNF.strv,DtKNF.minv,DtKNF.maxv,1,PresKey1(RdPrt(S_KDW))))
						PrintWPR(DtKNF.strv);
					else 
					{
                        if(DtKNF.strv<DtKNF.minv) DtKNF.strv=DtKNF.minv;
                        if(DtKNF.strv>DtKNF.maxv) DtKNF.strv=DtKNF.maxv;
						if_WriteEEPROM(_ADRSTV,DtKNF.strv);
						DtKNF.KRK=1;		
					}
					break;
#else
 					if(!ReadDataONL(RdPrt(S_KUP),&DtKNF.strv,DtKNF.minv,_KMAXMXV,1,PresKey1(RdPrt(S_KDW))))
						PrintWPR(DtKNF.strv);
					else 
					{
                        if(DtKNF.strv<DtKNF.minv) DtKNF.strv=DtKNF.minv;
						if_WriteEEPROM(_ADRSTV,DtKNF.strv);
						DtKNF.KRK=1;		
					}
					break;                   
#endif
				} 
				//dlugosc komina
				case 7:
				{
					if(!ReadDataONL(RdPrt(S_KUP),&DtKNF.lkm,_KMINLKM,_KMAXLKM,1,PresKey1(RdPrt(S_KDW))))
						PrintWPR(DtKNF.lkm);
					else 
					{
						if_WriteEEPROM(_ADRLKM,DtKNF.lkm);
						DtKNF.KRK=1;		
					}
					break;
				}     
				//moc podgrzewacza
				case 8:
				{
					if(!ReadDataONL(RdPrt(S_KUP),&DtKNF.pwr,_KMINPWR,_KMAXPWR,1,PresKey1(RdPrt(S_KDW))))
						PrintWPR(DtKNF.pwr);
					else 
					{
						if_WriteEEPROM(_ADRPWR,DtKNF.pwr);
						DtKNF.KRK=1;		
					}
					break;
				}                 
			}
			break;
		}
	}
}
/*---------------------------------------------------------------------------------*/
float F88ToFData(const signed int dt)
{
    volatile float f;
    f=(float)(dt)/256.0;
    if(!f||(f==round(f))) return f;
    if(f>0) return (f+0.005); //zaokraglenie w gore
    else return (f-0.005);    //zaokraglenie w dol
}
/*---------------------------------------------------------------------------------*/
//Konwersja dwoch bajtow danej 16bit typu F88, na wartosc float
float BtyF88ToFData(const unsigned char lo, const unsigned char hi)
{
    volatile signed int dt;
    volatile float fd;
    dt=(signed int)hi;
    dt=dt<<8;
    dt=dt|lo;
    fd=F88ToFData(dt);
    return fd;
}
/*---------------------------------------------------------------------------------*/
//Odczytuje i interpretuje parametry zapisane w pamieci EEPROM
void RdEEParam(void)
{
	DtKNF.tpkt=ReadEEPROM(_ADRWTK);                         //typ kotla
	SetTpKtParam();											//ustaw param. ukryte dla wybr. typu kotla
	ModParam(DtKNF.tpkt,DtKNF.rdgz=ReadEEPROM(_ADRRGZ));	//parametry modulatora dla wybranego typu kotla i rodzaju gazu
	MocStart(DtKNF.mstr=ReadEEPROM(_ADRMST));				//moc startowa
	MocMaksCW(DtKNF.mxcw=ReadEEPROM(_ADRMCW));				//moc maksymalna CW
    DtKNF.minv=ReadEEPROM(_ADRMNV);                         //minimum PWM na wentylatorze (%)    
    DtKNF.maxv=ReadEEPROM(_ADRMXV);                         //maksimum PWM na wentylatorze (%)
    if(DtKNF.maxv<DtKNF.minv) DtKNF.maxv=DtKNF.minv;
    DtKNF.strv=ReadEEPROM(_ADRSTV);                         //startowe PWM na wentylatorze (%)   
    if(DtKNF.strv<DtKNF.minv) DtKNF.strv=DtKNF.minv;
    if(DtKNF.strv>DtKNF.maxv) DtKNF.strv=DtKNF.maxv;
	DtCW.PCW=ReadEEPROM(_ADRPCW);							//nastawa PCW
	DtKNF.wymCW=0;											//wylacz wymuszenie CW
    DtKNF.nokr=2;                                           //liczba impulsow na obrot
    
    DtKNF.lkm=ReadEEPROM(_ADRLKM);                          //dlugosc komina
    DtKNF.pwr=ReadEEPROM(_ADRPWR);                          //moc podgrzewacza    
    DtKNF.P=(127.0+BtyF88ToFData(ReadEEPROM(_ADRPLO),ReadEEPROM(_ADRPHI)))/100000.0;
    DtKNF.I=(127.0+BtyF88ToFData(ReadEEPROM(_ADRILO),ReadEEPROM(_ADRIHI)))/100000.0;
    DtKNF.D=(127.0+BtyF88ToFData(ReadEEPROM(_ADRDLO),ReadEEPROM(_ADRDHI)))/100000.0;    
}
/*---------------------------------------------------------------------------------*/
//Ustaw parametry ukryte dla wybranego typu kotla
void SetTpKtParam(void)
{
	DtKNF.czpr=_BRAK;
	DtKNF.rdpm=_KJEDEN;
	DtKNF.tpwm=_TERMET;
}
/*---------------------------------------------------------------------------------*/
void WriteEEPCW(void)
{
	if_WriteEEPROM(_ADRPCW,DtCW.PCW);
}
/*---------------------------------------------------------------------------------*/
#endif