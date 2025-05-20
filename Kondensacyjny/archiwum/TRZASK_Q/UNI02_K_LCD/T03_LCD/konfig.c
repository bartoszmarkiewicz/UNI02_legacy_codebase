/*konfig.c*/
//Procedury zwiazane z wyborem typu gazu i poziomow
//mocy startowej oraz majksymalnej dla CO i CW
/*---------------------------------------------------------------------------------*/
#include <pic18.h>
#include <htc.h>
#include "global.h"					//parametry globalne
#include "konfig.h"					//parametry lokalne
#include "eeprom.h"
#include "pomiar.h"
#include "wsw.h"
#include "main.tp.h"
#include "keypad.tp.h"
extern unsigned char x;
extern tpMAIN M;
DtKONF DtKNF;
unsigned char bfrs;		//znacznik pierwszego przycisniecia - i +
//extern DataPFN PFN;
//extern DataCW DtCW;
unsigned char wf=0;

extern StPin ky[];
extern StPin xxky[];
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
	ClrAllMarks();							//wygas znaki konfiguracji
	DtKNF.enknf=0;							//zablokuj mozliwosc wejscia do konf.
	DtKNF.PRM=1;
	DtKNF.KRK=0;
//	RdEEParam();							//odczytaj parametry z pamieci EEPROM
//	PomiarPCW();
//	DtCW._newPCW=0;
//	DtCW._wrtPCW=0;
}
unsigned int nrp=1,rdgz=0,mxcw=0,mxco=0,mstr=0,anlg=0,tpkt=0,cpwg=0,mncw=0,mnco=0,opdg=0;
unsigned char zm,zp;

//Glowna procedura konfiguracji (wykonywana w petli glownej)
void MKonfiguracja(void)
{
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
				DtKNF.enknf=0;							//zablokuj mozliwosc wejscia do konf.
				M._AKTYWNE_KNF=0;
				return;
			}
			//przytrzymano przycisk RESET
			if(ExxPresKey(RdPrt(S_OFF),&xxky[S_OFF],50,1)==2)
			{
				M._AKTYWNE_KNF=1;
				StartRTM(_RTKONF3);					//start timera aktywnosci proc. konfig
				DtKNF.PRM=1; 						//aktywacja pierwszego parametru
				DtKNF.KRK=1; 						//aktywacja proc. konfiguracji
				ClrAllMarks();						//wygas znaki nie zwiazane z konfiguracja
				SetBufKey(&ky[S_OFF]);
				SetBufKey(&ky[S_SET]);
				ExxResKey(&xxky[S_OFF]);
			}
			else 
			{
				DtKNF.PRM=1;
				DtKNF.KRK=0;
				return;
			}
			break;
		}
		//wybor parametru
		case 1:
		{
			ReadParam(&nrp,&zm,&zp,1,_KNPAR1,1,0);
			DtKNF.PRM=nrp;
			PrintPR(led_r,DtKNF.PRM,0);								//kod rx
			if(PresKey(RdPrt(S_SET),&ky[S_SET],0))								//wybrano parametr
			{
				SetBufKey(&ky[S_OFF]);
				SetBufKey(&ky[S_SET]);
				EndPR();
				DtKNF.KRK=2;										//tryb modyfikacji parametru
			}
			if(ExxPresKey(RdPrt(S_OFF),&xxky[S_OFF],50,1)==2)					//wyjscie z procedury konfiguracji
			{
				DtKNF.KRK=2;										
				DtKNF.PRM=0;
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
					ExitKonf();
					return;
				}	
				//moc startowa (0...99)
				case 1:
				{
					ReadParam(&mstr,&zm,&zp,_KMNST,_KMXST,0,1);
					PrintWPR(mstr/100,mstr%100);
					//MocStart(mstr);
					//if(RdPrt(S_WCW)) DtKNF.wymCW=1;			//zalacz wymuszenie CW dla aktywnego przeplywu
					//else DtKNF.wymCW=0;						//zalacz wymuszenie CW
					if(PresKey(RdPrt(S_SET),&ky[S_SET],0))
					{
						DtKNF.wymCW=0;							//wylacz wymuszenie CW								//wylacz wymuszenie CO
						DtKNF.mstr=mstr;
						//eeprom_write(_ADRMST,DtKNF.mstr);		//zapisz nowa wartosc mstr
						DtKNF.KRK=1;		
					}
					if(PresKey(RdPrt(S_OFF),&ky[S_OFF],0))
					{
						DtKNF.wymCW=0;							//wylacz wymuszenie CW								//wylacz wymuszenie CO
						DtKNF.KRK=1;		
					}
					break;
				}	
				//maksimum modulacji CW
				case 2:
				{
					ReadParam(&mxcw,&zm,&zp,_KMNMAXCW,_KMXMAXCW,0,1);
					PrintWPR(mxcw/100,mxcw%100);
					//MocMaksCW(mmxcw);
					//if(RdPrt(S_WCW)) DtKNF.wymCW=1;		//zalacz wymuszenie CW dla aktywnego przeplywu
					//else DtKNF.wymCW=0;
					if(PresKey(RdPrt(S_SET),&ky[S_SET],0))
					{
						DtKNF.wymCW=0;							//wylacz wymuszenie CW
						DtKNF.mxcw=mxcw;
						//eeprom_write(_ADRMST,DtKNF.mxcw);		//zapisz nowa wartosc mstr
						DtKNF.KRK=1;		
					}
					if(PresKey(RdPrt(S_OFF),&ky[S_OFF],0))
					{
						DtKNF.wymCW=0;							//wylacz wymuszenie CW								//wylacz wymuszenie CO
						DtKNF.KRK=1;		
					}
					break;
				}
				//maksimum modulacji CO
				case 3:
				{
					ReadParam(&mxco,&zm,&zp,_KMNMAXCO,_KMXMAXCO,0,1);
					PrintWPR(mxco/100,mxco%100);
					//MocMaksCW(mmxcw);
					//if(RdPrt(S_WCW)) DtKNF.wymCW=1;		//zalacz wymuszenie CW dla aktywnego przeplywu
					//else DtKNF.wymCW=0;
					if(PresKey(RdPrt(S_SET),&ky[S_SET],0))
					{
						DtKNF.wymCW=0;							//wylacz wymuszenie CW
						DtKNF.mxcw=mxco;
						//eeprom_write(_ADRMST,DtKNF.mxco);		//zapisz nowa wartosc mstr
						DtKNF.KRK=1;		
					}
					if(PresKey(RdPrt(S_OFF),&ky[S_OFF],0))
					{
						DtKNF.wymCW=0;							//wylacz wymuszenie CW								//wylacz wymuszenie CO
						DtKNF.KRK=1;		
					}
					break;
				}
				//rodzaj gazu (0/1)
				case 4:
				{
					ReadParam(&rdgz,&zm,&zp,_KMINRGZ,_KMAXRGZ,0,0);
					PrintWPR(rdgz/100,rdgz%100);
					if(PresKey(RdPrt(S_SET),&ky[S_SET],0))
					{
						DtKNF.rdgz=rdgz;
						//eeprom_write(_ADRMST,DtKNF.rdgz);		//zapisz nowa wartosc mstr
						DtKNF.KRK=1;		
					}
					if(PresKey(RdPrt(S_OFF),&ky[S_OFF],0))
					{
						DtKNF.KRK=1;		
					}
					break;
				}
				//tryb pracy ANTYLEGINELLA
				case 5:
				{
					ReadParam(&anlg,&zm,&zp,_KMINANL,_KMAXANL,0,0);
					PrintWPR(anlg/100,anlg%100);
					if(PresKey(RdPrt(S_SET),&ky[S_SET],0))
					{
						DtKNF.anlg=anlg;
						//eeprom_write(_ADRMST,DtKNF.rdgz);		//zapisz nowa wartosc mstr
						DtKNF.KRK=1;		
					}
					if(PresKey(RdPrt(S_OFF),&ky[S_OFF],0))
					{
						DtKNF.KRK=1;		
					}
					break;
				}
				//rodzaj kotla
				case 6:
				{
					ReadParam(&tpkt,&zm,&zp,_KMINWTK,_KMAXWTK,0,0);
					PrintWPR(tpkt/100,tpkt%100);
					if(PresKey(RdPrt(S_SET),&ky[S_SET],0))
					{
						DtKNF.tpkt=tpkt;
						//eeprom_write(_ADRMST,DtKNF.rdgz);		//zapisz nowa wartosc mstr
						DtKNF.KRK=1;		
					}
					if(PresKey(RdPrt(S_OFF),&ky[S_OFF],0))
					{
						DtKNF.KRK=1;		
					}
					break;
				}
				//czujnik przeplywu WG
				case 7:
				{
					ReadParam(&cpwg,&zm,&zp,_KMINCPG,_KMAXCPG,0,0);
					PrintWPR(cpwg/100,cpwg%100);
					if(PresKey(RdPrt(S_SET),&ky[S_SET],0))
					{
						DtKNF.cpwg=cpwg;
						//eeprom_write(_ADRMST,DtKNF.rdgz);		//zapisz nowa wartosc mstr
						DtKNF.KRK=1;		
					}
					if(PresKey(RdPrt(S_OFF),&ky[S_OFF],0))
					{
						DtKNF.KRK=1;		
					}
					break;
				}
				//moc minimalna WU
				case 8:
				{
					ReadParam(&mncw,&zm,&zp,_KMNMINCW,_KMXMINCW,0,1);
					PrintWPR(mncw/100,mncw%100);
					//MocMaksCW(mmxcw);
					//if(RdPrt(S_WCW)) DtKNF.wymCW=1;		//zalacz wymuszenie CW dla aktywnego przeplywu
					//else DtKNF.wymCW=0;
					if(PresKey(RdPrt(S_SET),&ky[S_SET],0))
					{
						DtKNF.wymCW=0;							//wylacz wymuszenie CW
						DtKNF.mxcw=mxcw;
						//eeprom_write(_ADRMST,DtKNF.mxcw);		//zapisz nowa wartosc mstr
						DtKNF.KRK=1;		
					}
					if(PresKey(RdPrt(S_OFF),&ky[S_OFF],0))
					{
						DtKNF.wymCW=0;							//wylacz wymuszenie CW								//wylacz wymuszenie CO
						DtKNF.KRK=1;		
					}
					break;
				}
				//moc minimalna WG
				case 9:
				{
					ReadParam(&mnco,&zm,&zp,_KMNMINCO,_KMXMINCO,0,1);
					PrintWPR(mnco/100,mnco%100);
					//MocMaksCW(mmxcw);
					//if(RdPrt(S_WCW)) DtKNF.wymCW=1;		//zalacz wymuszenie CW dla aktywnego przeplywu
					//else DtKNF.wymCW=0;
					if(PresKey(RdPrt(S_SET),&ky[S_SET],0))
					{
						DtKNF.wymCW=0;							//wylacz wymuszenie CW
						DtKNF.mxcw=mxcw;
						//eeprom_write(_ADRMST,DtKNF.mxcw);		//zapisz nowa wartosc mstr
						DtKNF.KRK=1;		
					}
					if(PresKey(RdPrt(S_OFF),&ky[S_OFF],0))
					{
						DtKNF.wymCW=0;							//wylacz wymuszenie CW								//wylacz wymuszenie CO
						DtKNF.KRK=1;		
					}
					break;
				}
				//ogrzewanie podlogowe
				case 10:
				{
					ReadParam(&opdg,&zm,&zp,_KMNPDG,_KMXPDG,0,0);
					PrintWPR(opdg/100,opdg%100);
					//MocMaksCW(mmxcw);
					//if(RdPrt(S_WCW)) DtKNF.wymCW=1;		//zalacz wymuszenie CW dla aktywnego przeplywu
					//else DtKNF.wymCW=0;
					if(PresKey(RdPrt(S_SET),&ky[S_SET],0))
					{
						DtKNF.wymCW=0;							//wylacz wymuszenie CW
						DtKNF.mxcw=mxcw;
						//eeprom_write(_ADRMST,DtKNF.mxcw);		//zapisz nowa wartosc mstr
						DtKNF.KRK=1;		
					}
					if(PresKey(RdPrt(S_OFF),&ky[S_OFF],0))
					{
						DtKNF.wymCW=0;							//wylacz wymuszenie CW								//wylacz wymuszenie CO
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
//Odczytuje i interpretuje parametry zapisane w pamieci EEPROM
void RdEEParam(void)
{
	/*DtKNF.tpkt=eeprom_read(_ADRWTK);						//typ kotla
	SetTpKtParam();											//ustaw param. ukryte dla wybr. typu kotla
	ModParam(DtKNF.tpkt,DtKNF.rdgz=eeprom_read(_ADRRGZ));	//parametry modulatora dla wybranego typu kotla i rodzaju gazu
	MocStart(DtKNF.mstr=eeprom_read(_ADRMST));				//moc startowa
	MocMaksCW(DtKNF.mxcw=eeprom_read(_ADRMCW));				//moc maksymalna CW
	DtCW.PCW=eeprom_read(_ADRPCW);							//nastawa PCW
	DtKNF.wymCW=0;											//wylacz wymuszenie CW
	*/
}
/*---------------------------------------------------------------------------------*/
//Ustaw parametry ukryte dla wybranego typu kotla
void SetTpKtParam(void)
{
	/*
	DtKNF.czpr=_BRAK;
	DtKNF.rdpm=_KJEDEN;
	DtKNF.tpwm=_TERMET;
	*/
}
/*---------------------------------------------------------------------------------*/
void WriteEEPCW(void)
{
//	eeprom_write(_ADRPCW,DtCW.PCW);
}
