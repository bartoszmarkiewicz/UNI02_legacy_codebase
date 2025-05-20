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
#include "print.h"
#include "wsw2.h"
#include "main.tp.h"

extern tpMAIN M;
DtKONF DtKNF;
unsigned char bfrs;		//znacznik pierwszego przycisniecia reset
extern DataPFN PFN;
extern DataCW DtCW;
unsigned char wf=0;
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
            EndOFF();
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
            if(_GRZEJ&&!RdPrt(S_PLM)) 
            {
                PrintOFF(); 
            }
            else
            {
                EndOFF();
            }           
			switch(DtKNF.PRM)
			{
				//wyjscie z procedury
				case 0:
				{
                    //EndOFF();
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
                    /*if(_GRZEJ&&!RdPrt(S_PLM)) 
                    {
                        PrintOFF(); 
                    }
                    else
                    {
                        EndOFF();
                    }*/
					if(!ReadDataXONL(RdPrt(S_KUP),&DtKNF.mstr,_KMINMST,_KMAXMST,1,1,PresKey1(RdPrt(S_KDW))))
					{
						PrintWPR(DtKNF.mstr);
						//MocStart(DtKNF.mstr);
						if(RdPrt(S_WCW)) DtKNF.wymCW=1;			//zalacz wymuszenie CW dla aktywnego przeplywu
						else DtKNF.wymCW=0;						//zalacz wymuszenie CW
					}
					else 
					{
						DtKNF.wymCW=0;							//wylacz wymuszenie CW								//wylacz wymuszenie CO
                        if(DtKNF.mstr!=eeprom_read(_ADRMST))
                        {
                            eeprom_write(_ADRMST,DtKNF.mstr);
                        }
						DtKNF.KRK=1;		
					}
					break;
				}	
				//moc maksymalna WU (0...99)
				case 2:
				{
                    /*if(_GRZEJ&&!RdPrt(S_PLM)) 
                    {
                        PrintOFF(); 
                    }
                    else
                    {
                        EndOFF();
                    }*/                    
					if(!ReadDataXONL(RdPrt(S_KUP),&DtKNF.mxcw,_KMINMCW,_KMAXMCW,1,1,PresKey1(RdPrt(S_KDW))))
					{
						PrintWPR(DtKNF.mxcw);
						//MocMaksCW(DtKNF.mxcw);
						if(RdPrt(S_WCW)) DtKNF.wymCW=1;		//zalacz wymuszenie CW dla aktywnego przeplywu
						else DtKNF.wymCW=0;
					}
					else 
					{
						DtKNF.wymCW=0;							//wylacz wymuszenie CW
                        if(DtKNF.mxcw!=eeprom_read(_ADRMCW))
                        {
                            eeprom_write(_ADRMCW,DtKNF.mxcw);
                        }
						DtKNF.KRK=1;		
					}
					break;
				}
				//minimum dla wentylatora (0/1)
				case 3:
				{
                    //EndOFF();
					if(!ReadDataONL(RdPrt(S_KUP),&DtKNF.mmin,_KMINVMD,_KMAXVMD,1,PresKey1(RdPrt(S_KDW))))
						PrintWPR(DtKNF.mmin);
					else 
					{
                        if(DtKNF.mmin!=eeprom_read(_ADRMDG))
                        {                        
                            eeprom_write(_ADRMDG,DtKNF.mmin);
                            ModParam();								//oblicz parametry kluczowe modulatora
                        }
						DtKNF.KRK=1;		
					}
					break;
				}
				//maksimum dla wentylatora
				case 4:
				{
                    //EndOFF();
					if(!ReadDataONL(RdPrt(S_KUP),&DtKNF.mmax,_KMINVMG,_KMAXVMG,1,PresKey1(RdPrt(S_KDW))))
						PrintWPR(DtKNF.mmax);
					else 
					{
                        if(DtKNF.mmax!=eeprom_read(_ADRMGG))
                        {
                            eeprom_write(_ADRMGG,DtKNF.mmax);
                            ModParam();								//oblicz parametry kluczowe modulatora
                        }
						DtKNF.KRK=1;		
					}
					break;
				}
				//liczba impulsow na obrot
				case 5:
				{
                    //EndOFF();
					if(!ReadDataONL(RdPrt(S_KUP),&DtKNF.nokr,_KMINNOK,_KMAXNOK,1,PresKey1(RdPrt(S_KDW))))
						PrintWPR(DtKNF.nokr);
					else 
					{
						if(DtKNF.nokr!=eeprom_read(_ADRNOK)) eeprom_write(_ADRNOK,DtKNF.nokr);
						DtKNF.KRK=1;		
					}
					break;
				}  
				//rodzaj wentylatora
				case 6:
				{
                    //EndOFF();
					if(!ReadDataONL(RdPrt(S_KUP),&DtKNF.rfan,_KMINRFAN,_KMAXRFAN,1,PresKey1(RdPrt(S_KDW))))
						PrintWPR(DtKNF.rfan);
					else 
					{
						if(DtKNF.rfan!=eeprom_read(_ADRRFAN)) eeprom_write(_ADRRFAN,DtKNF.rfan);
						ModParam();								//oblicz parametry kluczowe modulatora
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
	DtKNF.tpkt=eeprom_read(_ADRWTK);						//typ kotla
	SetTpKtParam();											//ustaw param. ukryte dla wybr. typu kotla
	//ModParam(DtKNF.tpkt,DtKNF.rdgz=eeprom_read(_ADRRGZ));	//parametry modulatora dla wybranego typu kotla i rodzaju gazu
	//MocStart(DtKNF.mstr=eeprom_read(_ADRMST));				//moc startowa
	//MocMaksCW(DtKNF.mxcw=eeprom_read(_ADRMCW));				//moc maksymalna CW
	DtKNF.mstr=eeprom_read(_ADRMST);						//moc startowa
	DtKNF.mxcw=eeprom_read(_ADRMCW);						//moc maksymalna CW
	DtKNF.rfan=eeprom_read(_ADRRFAN);                       //rodzaj wentylatora
	DtKNF.mmin=eeprom_read(_ADRMDG);					//dolna granica predkosci wentylatora
	DtKNF.mmax=eeprom_read(_ADRMGG);					//gorna granica predkosci wentylatora
	ModParam();												//oblicz parametry kluczowe modulatora
	DtKNF.nokr=eeprom_read(_ADRNOK);						//liczba impulsow na obrot dla tachometru wentylatora   
	DtCW.PCW=eeprom_read(_ADRPCW);							//nastawa PCW
	DtKNF.wymCW=0;											//wylacz wymuszenie CW
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
	eeprom_write(_ADRPCW,DtCW.PCW);
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Zapamietaj/odczytaj kolejne punkty funkcji kalibracji
//Rodzaj wentylatora
unsigned char ReadEERFAN()
{   
	return eeprom_read(_ADRRFAN);                       //rodzaj wentylatora
}
//------
void WriteEEVNT0(unsigned char v,unsigned char p)
{
    volatile unsigned char d;
    
    p=ReadEERFAN()*0x10;  
	eeprom_write(_ADRVN0+d,v);
	eeprom_write(_ADRPW0+d,p);
}
unsigned char ReadEEVNT0_V()
{
    volatile unsigned char d;
    
    d=ReadEERFAN()*0x10;    
	return eeprom_read(_ADRVN0+d);
}
unsigned char  ReadEEVNT0_P()
{
    volatile unsigned char d;
    
    d=ReadEERFAN()*0x10;    
	return eeprom_read(_ADRPW0+d);
}
//------
void WriteEEVNT1(unsigned char v,unsigned char p)
{
    volatile unsigned char d;
    
    d=ReadEERFAN()*0x10;    
	eeprom_write(_ADRVN1+d,v);
	eeprom_write(_ADRPW1+d,p);
}
unsigned char ReadEEVNT1_V()
{
    volatile unsigned char d;
    
    d=ReadEERFAN()*0x10;    
	return eeprom_read(_ADRVN1+d);
}
unsigned char ReadEEVNT1_P()
{
    volatile unsigned char d;
    
    d=ReadEERFAN()*0x10;    
	return eeprom_read(_ADRPW1+d);
}
//------
void WriteEEVNT2(unsigned char v,unsigned char p)
{
    volatile unsigned char d;
    
    d=ReadEERFAN()*0x10;    
	eeprom_write(_ADRVN2+d,v);
	eeprom_write(_ADRPW2+d,p);
}
unsigned char ReadEEVNT2_V()
{
    volatile unsigned char d;
    
    d=ReadEERFAN()*0x10;    
	return eeprom_read(_ADRVN2+d);
}
unsigned char ReadEEVNT2_P()
{
    volatile unsigned char d;
    
    d=ReadEERFAN()*0x10;    
	return eeprom_read(_ADRPW2+d);
}
//------
void WriteEEVNT3(unsigned char v,unsigned char p)
{
    volatile unsigned char d;
    
    d=ReadEERFAN()*0x10;    
	eeprom_write(_ADRVN3+d,v);
	eeprom_write(_ADRPW3+d,p);
}
unsigned char ReadEEVNT3_V()
{
    volatile unsigned char d;
    
    d=ReadEERFAN()*0x10;    
	return eeprom_read(_ADRVN3+d);
}
unsigned char ReadEEVNT3_P()
{
    volatile unsigned char d;
    
    d=ReadEERFAN()*0x10;    
	return eeprom_read(_ADRPW3+d);
}
//------
void WriteEEVNT4(unsigned char v,unsigned char p)
{
    volatile unsigned char d;
    
    d=ReadEERFAN()*0x10;    
	eeprom_write(_ADRVN4+d,v);
	eeprom_write(_ADRPW4+d,p);
}
unsigned char ReadEEVNT4_V()
{
    volatile unsigned char d;
    
    d=ReadEERFAN()*0x10;    
	return eeprom_read(_ADRVN4+d);
}
unsigned char ReadEEVNT4_P()
{
    volatile unsigned char d;
    
    d=ReadEERFAN()*0x10;    
	return eeprom_read(_ADRPW4+d);
}
//------
void WriteEEVNT5(unsigned char v,unsigned char p)
{
    volatile unsigned char d;
    
    d=ReadEERFAN()*0x10;    
	eeprom_write(_ADRVN5+d,v);
	eeprom_write(_ADRPW5+d,p);
}
unsigned char ReadEEVNT5_V()
{
    volatile unsigned char d;
    
    d=ReadEERFAN()*0x10;    
	return eeprom_read(_ADRVN5+d);
}
unsigned char ReadEEVNT5_P()
{
    volatile unsigned char d;
    
    d=ReadEERFAN()*0x10;    
	return eeprom_read(_ADRPW5+d);
}
//------
void WriteEEVNT6(unsigned char v,unsigned char p)
{
    volatile unsigned char d;
    
    d=ReadEERFAN()*0x10;    
	eeprom_write(_ADRVN6+d,v);
	eeprom_write(_ADRPW6+d,p);
}
unsigned char ReadEEVNT6_V()
{
    volatile unsigned char d;
    
    d=ReadEERFAN()*0x10;    
	return eeprom_read(_ADRVN6+d);
}
unsigned char ReadEEVNT6_P()
{
    volatile unsigned char d;
    
    d=ReadEERFAN()*0x10;    
	return eeprom_read(_ADRPW6+d);
}
//------
void WriteEEVNT7(unsigned char v,unsigned char p)
{
    volatile unsigned char d;
    
    d=ReadEERFAN()*0x10;    
	eeprom_write(_ADRVN7+d,v);
	eeprom_write(_ADRPW7+d,p);
}
unsigned char ReadEEVNT7_V()
{
    volatile unsigned char d;
    
    d=ReadEERFAN()*0x10;    
	return eeprom_read(_ADRVN7+d);
}
unsigned char ReadEEVNT7_P()
{
    volatile unsigned char d;
    
    d=ReadEERFAN()*0x10;    
	return eeprom_read(_ADRPW7+d);
}


