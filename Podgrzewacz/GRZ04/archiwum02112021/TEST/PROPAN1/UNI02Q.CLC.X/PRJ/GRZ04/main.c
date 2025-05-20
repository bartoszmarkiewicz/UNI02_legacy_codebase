/*main.c*/
/*Oprogramowanie sterownikow do podgrzewaczy gazowych typu turbo*/
/*Sterownik na bazie p³yty UNI-02 wersja 3.3V...*/
//Konwersja oprogramowania z p³ytki UNI-01 na UNI-02 (08.05.2014)
//(Wyswietlacz szeregowy, zoptymalizowana procedura modulacji, skrocony wybieg wentylatora gdy nie nastapil zaplon gazu)
#include <xc.h>
#include <stdbool.h>
#include <stdint.h>

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

#include "pwm.h"
#include "pomiar.h"

extern tpMAIN M;
extern DtPWM PWM;			//pwm.c
//extern DataPrCW PrCW;		//czcw.c
extern DataCO DtCO;			//pomiar.c
extern DataCW DtCW;
extern DataZW DtZW;
extern DataPFN PFN;
extern DataCSN DtCS;
extern DtKONF DtKNF;
extern DtRgl rgl;
extern StPin sp;
extern unsigned char pw1,pw2;

volatile unsigned char x,y,t,ty,prz;
unsigned int z;
#if _SRVTRB==1						//aktywny tryb serwisowy kompilacji?
extern unsigned char mypwm;
#endif

void main_prc(void)
{
    
    volatile unsigned char w;
//------------------
//------------------
    InitPrcISR();
    InitDtaI2C();
    InitDtaUART1();
    InitDtaPort();
	InitDtaWsw();    
    InitDtaPWM();
    InitDataPmr();
	ClearData();  
    
//------------------
//------------------
	if (!PCON0bits.POR||!PCON0bits.BOR)							//zalaczono zasilanie?
	{
		M._PW_START=1;						//znacznik rozruchu sterownika po wlaczeniu zasilania
		TestScr();							//wysterowanie wszystkich segmentow wyswietlacza
		PwrOnDelay();						//zwloka na wypadek stanow nieustalonych	
		EndTestScr();						//wyjdz z trybu wysterowania wszystkich segmentow wyswietlacza
	}
	else
	{
		M._RS_START=1;						//znacznik rozruchu sterownika po resecie
		ClsScr();							//wygas wyswietlacz
		PwrOnDelay();						//zwloka na wypadek stanow nieustalonych	
		EndClsScr();						//zapal wyswietlacz
	}
	//Stan po zalaczeniu zasilania - warunek wejscia do trybu konfiguracji
	if (!PCON0bits.POR||!PCON0bits.BOR)		//zalaczono zasilanie? 
	{	
		PCON0bits.POR=1;
		PCON0bits.BOR=1;
		EnKonf();		//aktywacja oczekiwania na wejscie do konfiguracji
	}
	//Odczytaj parametry zapisane w pamieci EEPROM
	RdEEParam();

	//Sprawd¿ czy nastapilo wylaczenie awaryjne z blokada
	if(M.ERR_BTY=ReadEEPROM(_ADRERR)) 		//uruchomienie po wylaczeniu awaryjnym?
	{
		if((M.ERR_BTY>=1)&&(M.ERR_BTY<=3))	//dozwolony kod bledu?
		{
			switch(M.ERR_BTY)
			{
				case 1:
					PrintErr(0xE1,1);		//wyswietl kod bledu
				break;
				case 2:
					PrintErr(0xE2,1);		//wyswietl kod bledu
				break;
				case 3:
					PrintErr(0xE3,1);		//wyswietl kod bledu
				break;
			}
			ErrPTG();						//przejdz do petli wylaczenia awaryjnego
		}
		else M.ERR_BTY=0;
	}
	
	//Pomiar wartosci nastaw dla unikniecia migajacej nastawy po zal. zasilania
	PomiarPCW();			//wartosc nastawy CW
	DtCW._newPCW=0;			//deaktywacja wyswietlania nastawy CW
	DtCW._wrtPCW=0;			//deaktywacja zapisu nowej nastawy

	//deaktywacja pobudzen
	_VNT_SetLow();									//wentylator na stale wylaczony
	_GRZEJ_SetLow();


	//decyzja czy kociol jest typu TURBO
	M._TURBO=1;
	RestartFltPrt();
	StartRTS(_RTSTR);						//start timera rozruchu sterownika						
//------------------
//------------------
//------------------
    y=10;
    t=0;
    
	while(1)					//petla glowna programu				
	{
		CLRWDT();
        //w=VNTtoPPWM(DtKNF.strv);
        //ProcToVNT(w);
        //_VNT_SetHigh();
        //GrzejON();
    
        /*
        DtKNF.lkm=0;
        //x=VNTtoPPWM(y);
        z=MProcToVNT();
        //x=MPPWMtoVNT(y);
        */
        /*PomiarCW();				//wartosc temperatury CW
        FiltrPomiarowyCW();     //filtr danych opisujacych stan temperatur
        PrintCW(DtCW.SCW,0);	//wymuszenie CW		
        w=RdPrt(S_ERR_CW);
		//awaria czujnika CW
		if(w)
		{
			M.ERR_BTY=0x04;
            PrintErr(0xE4,0);
			M._RUSZ_NTC_CW=1;
		}
		else 
		{
            M._RUSZ_NTC_CW=0;
            M.ERR_BTY=0x00;
            EndErr();		//nie wyswietlaj kodu bledu przy konfiguracji par. r4 i r6
		}        
        */
        
   
		M._PRACA=(M._ROZRUCH||_GRZEJ_GetValue());
	//----------------
		MKonfiguracja();		//decyzja o wejsciu do proc. konfiguracji
	//----------------
        PomiarVNT();			//predkosc obrotowa wentylatora
		PomiarPCW();			//wartosc nastawy CW
		PomiarCW();				//wartosc temperatury CW
		WritePCW();
		FiltrPomiarowyCW();	//filtr danych opisujacych stan temperatur
	//----------------
		DecWSW();				//wyswietlane parametry pracy
		DecERR();				//stany awaryjne, wyswietlane kody awaryjne
		
		DecSTR();				//dodatkowe decyzje sterowania
		DecRozruch();			//decyzja o zapaleniu palnika
		DecWylacz();			//decyzja o wygaszeniu palnika
#if _SRVTRB!=1
		FunkcjaSerwisowa();		//decyzja o realizacji funkcji serwisowej
#endif
        //prz=M._pob_CW;//(_PRZ_CW_GetValue())?0:1;
		KolejkaPobudzen();		//aktywacja pobudzen wg priorytetu
        //KontrolaWentylatora();	//kontrola poprawnosci pracy wentylatora
	//----------------

		if(M._ROZRUCH) Rozruch();			//rozruch kotla
		if(M._VWYBIEG) VWybieg();			//wybieg wentylatora
		ReloadPlomien();					//decyzja o wyzerowaniu licznika prob przy braku plomienia
		if(M._RUSZ_PLOMIEN) RuszPlomien();	//awaryjna procedura przy braku plomienia
#if _NO_PRS==0        
		ReloadCiag();						//zerowanie licznika ponowien prob ciagu
		if(M._RUSZ_CIAG) RuszCiag();		//awaryjna procedura przy braku ciagu
#endif
        if(M._SVWB) VSWybieg();             //wybieg wentylatora na predkosci startowej
		if(M._MODULUJ) Moduluj();			//algorytm modulacji
        SterownikVNT();                     //steruj predkoscia obrotow wentylatora
	}
}
//------------------
//------------------
//------------------
//Wyzerowanie podstawowych danych sterujacych
void ClearData(void)
{
	DtKNF.enknf=0;
	M._pob_KCW=0;
	M._pob_SRW=0;
	M._pob_CW=0;

	M._WYLACZ=0;
	M._ROZRUCH=0;
	M._VWYBIEG=0;
	M._MODULUJ=0;
	M._RUSZ_CIAG=0;
	M._RUSZ_PLOMIEN=0;
	M._RUSZ_RPLM=0;
	M._MODULUJ=0;

	M._AKTYWNE_KNF=0;
	M._AKTYWNE_SRW=0;
	M._RUSZ_NTC_CW=0;
	M._RUSZ_STB=0;
	M._RUSZ_MOD=0;

	M._PW_START=0;
	M._RS_START=0;
	M._TURBO=0;				
	M._PRACA=0;
	M._OST_PL=0;
	M._RUN_MNMOD=0;
	M._DE_VNT=0;
    
    M._SVWB=0;
    M._initVNT=0;

	M.ERR_BTY=0;
	M.ROZ=0;
	M.WYL=0;
	M.RPL=0;
	M.RCI=0;
	M.MNM=0;
	M.VWB=0;
	M.KRS=0;
	M.LICZNIK_PL=0;
	M.LICZNIK_CIG=0;
	M.LICZNIK_CIG2=0;
	M.LICZNIK_CIG3=0;

	_VNT_SetLow();
	_GRZEJ_SetLow();
}
void RestartFltPrt(void)
{
	SetPrt(S_WCW,0);
}
//------------------
//------------------
//Wybor aktywnego pobudzenia ze wzgledu na miejsce w kolejce priorytetow i pozycje przelacznika funkcyjnego
void KolejkaPobudzen(void)
{
	unsigned char wr=0;

	//grzanie w proc. konfiguracji
	if(M._AKTYWNE_KNF)
	{
		M._pob_CW=0;
		if(DtKNF.wymCW) M._pob_KCW=1;				//aktywacja pobudzenia
		else M._pob_KCW=0;
	}
	else
	{
		//grzanie od pobudzenia CW           
		M._pob_KCW=0;
		if(RdPrt(S_WCW))
		{
			M._pob_CW=1;	//aktywacja pobudzenia		
		}
		else M._pob_CW=0;	//deaktywacja pobudzenia
	}
}
//------------------
//------------------
//Regulacja stanu wyswietlacza 
void DecWSW(void)
{
	//PrintHex(PFN.AC);
#if _SRVTRB==1						//aktywny tryb serwisowy kompilacji?
	if(M._PRACA)
	{
		EndOFF();
		PrintHex(PWM.BufPWM);
		return;
	}
#endif
	if(!_GRZEJ_GetValue()) PrintOFF();  
	else 
	{
		PrintCW(DtCW.SCW,(M._AKTYWNE_SRW)?1:0);	//wymuszenie CW		
		EndOFF();
	}
	//nastawa PCW
	if(DtCW._newPCW) PrintNstCW(DtCW.PCW,(M._AKTYWNE_SRW)?1:0);
}
//------------------
//------------------
//Reakcja na wiekszosc stanow awaryjnych 
void DecERR(void)
{
	unsigned char KnfDisErr=0;
	unsigned char w;
//Znacznik wylaczajacy wizualizacje kodu bledu podczas pracy w proc. konfiguracji

	KnfDisErr=(M._AKTYWNE_KNF&&(DtKNF.KRK==1||(DtKNF.KRK==2&&DtKNF.PRM==3)))?1:0;
//--------BEZWZGLEDNE WYLACZENIE Z BLOKADA
//UWAGA: natychmiastowe wyswietlenie kodu awaryjnego
//zadzialalo STB
	if(!RdPrt(S_STB))
	{
		M.ERR_BTY=0x02;
		PrintErr(0xE2,1);
		ErrPTG();
	}
//--------BEZWZGLEDNE WYLACZENIE BEZ BLOKADY
	if(M._RUSZ_PLOMIEN) 
	{
		M.ERR_BTY=0;				//wyzeruj wszystkie wczesniejsze kody
		if(KnfDisErr) EndErr();		//nie wyswietlaj kodu bledu przy konfiguracji par. r4 i r6
		else PrintErr(0xE1,0);
		M._RUSZ_NTC_CW=0;
	}
	else 
	if(M._RUSZ_CIAG)
	{
		M.ERR_BTY=0;				//wyzeruj wszystkie wczesniejsze kody
		if(KnfDisErr) EndErr();		//nie wyswietlaj kodu bledu przy konfiguracji par. r4 i r6
		else
		{
			if(M._prb_CIG)
			{
 				PrintErr(0xE3,0);
			}
		}
		M._RUSZ_NTC_CW=0;
		M._RUSZ_MOD=0;
	}
	else
	{
		//w=DtCW._err_CW;
        w=RdPrt(S_ERR_CW);
		//awaria czujnika CW
		if(w)
		{
			M.ERR_BTY=0x04;
			if(KnfDisErr) EndErr();		//nie wyswietlaj kodu bledu przy konfiguracji par. r4 i r6
			else PrintErr(0xE4,0);
			M._RUSZ_NTC_CW=1;
			M._RUSZ_MOD=0;
		}
		else 
		{
			M._RUSZ_NTC_CW=0;
			//awaria obwodu modulatora
			if(!RdPrt(S_MOD_ON))
			{
				M.ERR_BTY=0x07;
				if(KnfDisErr) EndErr();		//nie wyswietlaj kodu bledu przy konfiguracji par. r4 i r6, r7
				else PrintErr(0xE7,0);
				M._RUSZ_MOD=1;
			}
			else
			{
				M._RUSZ_MOD=0;
			}
		}
	}
//--------START PROCEDUR NAPRAWCZYCH - WYLACZENIE Z BLOKADA
//UWAGA: bez wizualizacji kodu awaryjnego (wyswietlany dopiero po wykonaniu procedury naprawczej)
	//brak plomienia (praca kotla)
	if(!M._RUSZ_PLOMIEN&&!RdPrt(S_PLM)&&!M._ROZRUCH&&_GRZEJ_GetValue()) 
	{
		if(M._TURBO)	//sterownik TURBO?
		{
			if(_VNT_GetValue())	//wentylator pracuje?
			{
#if _NO_PRS==0                
				if(RdPrt(S_CIG_NO)&&RdPrt(S_STB))
#else
				if(RdPrt(S_STB))                    
#endif
				{
					M.RPL=0;
					M._RUSZ_PLOMIEN=1;
				}
			}
			else		//wentylator nie pracuje
			{
				if(RdPrt(S_STB))
				{
					M.RPL=0;
					M._RUSZ_PLOMIEN=1;
				}	
			}
		}
		else	//sterownik ATMOSFERYCZNY
		{
			if(RdPrt(S_CIG_NC)&&RdPrt(S_STB))
			{
				M.RPL=0;
				M._RUSZ_PLOMIEN=1;
			}			
		}
	}
#if _NO_PRS==0  
	//brak ciagu (praca kotla)
	if(!M._TURBO)		//kociol atmosferyczny
	{
		if(!M._RUSZ_CIAG&&!RdPrt(S_CIG_NC)&&RdPrt(S_STB)&&M._PRACA)
		{
			M.RCI=0;
			M._RUSZ_CIAG=1;
		}
	}
	else				//kociol turbo
	{
		//if(!M._RUSZ_CIAG&&!M._DE_VNT&&((!RdPrt(S_CIG_NO)&&_VNT_GetValue())||(RdPrt(S_CIG_NO)&&!_VNT_GetValue())))
		if(!M._RUSZ_CIAG&&!M._ROZRUCH&&!M._DE_VNT&&((!RdPrt(S_CIG_NO)&&_VNT_GetValue())))//||(RdPrt(S_CIG_NO)&&!_VNT_GetValue()))) 
		{
			M.RCI=0;
			M._RUSZ_CIAG=1;						//aktywuj procedure awaryjna
			if(!M.LICZNIK_CIG) StartRTS(_RTRSCI); //licznik liczby zanikow ciagu na minute
			M.LICZNIK_CIG++;
			if((M.LICZNIK_CIG>=_NCYKLI_CIG)||((M.LICZNIK_CIG3+M.LICZNIK_CIG)>=_NCYKLI_CIG3))		//przekroczono dozwolona liczbe zanikow ciagu na minute?
			{
				M.LICZNIK_CIG=0;
				M.ERR_BTY=0x03;
				PrintErr(0xE3,1);
				ErrPTG();
			}
		}
	}
#else
    M.RCI=0;
    M._RUSZ_CIAG=0;
    M.LICZNIK_CIG=0;    
#endif
//--------REAKCJA NA BRAK SYTUACJI AWARYJNEJ (DOTYCZY WIZUALIZACJI KODU AWARYJNEGO)

	//usun kod awaryjny z wyswietlacza
	if(!M._RUSZ_PLOMIEN&&!M._RUSZ_CIAG&&!M._RUSZ_NTC_CW&&!M._RUSZ_MOD)
	{
		M.ERR_BTY=0x00;
		EndErr();
	}
}
//------------------
//------------------
//Decyzje sterujace
void DecSTR(void)
{
	//wyzerowanie znacznikow rozruchu sterownika po wlaczeniu zasilania lub resecie
	if(RTS(_RTSTR)>=_TSTR)
	{
 		M._PW_START=0;
		M._RS_START=0;
	}
	//reakcja na zbyt szybki narost temperatury
	//if(!M._ROZRUCH&&_GRZEJ_GetValue()&&DtCO._ldDCO) M._RUN_MNMOD=1;
	//else M._RUN_MNMOD=0;
	//--------------
	//aktywacja/deaktywacja bloku modulacji plomieniem dla pobudzenia CW
#if _SRVTRB==1
	M._MODULUJ=0;
#else
	if(M._pob_CW)
	{
		if(M._AKTYWNE_SRW) M._MODULUJ=0;
		else
		if(!M._ROZRUCH&&_GRZEJ_GetValue())
		{
			if(!M._MODULUJ)
			{
				M._MODULUJ=1;
				RestartMod();
	
			}	
		}
		else M._MODULUJ=0;
	}
	else M._MODULUJ=0;
	//deaktywacja bloku modulacji plomieniem dla awarii modulatora lub dla szybkiego narostu temp.
#if _RNMOD==1
	if(!M._PRACA||!RdPrt(S_MOD_ON)||M._RUN_MNMOD)
	{
		M._MODULUJ=0;					//wylacz blok modulacji
		if(!M._PRACA||!RdPrt(S_MOD_ON))
        {
            WritePWM(0);
        }
		else
        {
            WritePWM(PWM.MDMOD);		//zbyt szybki narost temp.
        }
	}
#else
    if(!M._PRACA||!RdPrt(S_MOD_ON))
	{
		M._MODULUJ=0;					//wylacz blok modulacji
		if(!M._PRACA||!RdPrt(S_MOD_ON))
        {
            WritePWM(0);
        }
		else
        {
            WritePWM(PWM.MDMOD);		//zbyt szybki narost temp.
        }
	}
#endif
	//wymuszenie konkretnej wartosci wypelnienia dla danego wymuszenia
#endif
	if(M._pob_KCW&&!M._ROZRUCH&&_GRZEJ_GetValue()) WritePWM(PWM.MMAXCW);
#if _SRVTRB==1														//aktywny tryb serwisowy kompilacji?
	if(!M._ROZRUCH&&_GRZEJ_GetValue()&&RdPrt(S_MOD_ON)) WriteOPWM(mypwm);
	if(!M._PRACA||!RdPrt(S_MOD_ON)) WritePWM(0);
#else
	if(M._AKTYWNE_SRW&&!M._ROZRUCH&&_GRZEJ_GetValue()) WritePWM(_MG2_MOD);
#endif
}
//Ustaw minimalny wydatek na modulatorze
/*
void SetLoMod(void)
{
	M._MODULUJ=0;	//wylacz blok modulacji
	WritePWM(0);
}*/
//------------------
//------------------
//Decyzja o zapaleniu palnika
void DecRozruch(void)
{
	if(!M._ROZRUCH&&!_GRZEJ_GetValue())
	if(!M._RUSZ_PLOMIEN&&!M._RUSZ_CIAG&&!M._RUSZ_STB&&!M._RUSZ_NTC_CW)
	{
		if(M._pob_KCW)										//konfiguracja obieg CW
		{
			if(DtCW._su_CW)
			{
 				SetRozruch();
			}						
		}
		else
		if(M._pob_CW)										//grzanie obiegu CW
		{
			if(DtCW._su_CW)
			{
 				SetRozruch();
			}						
		}						
	}
}
//Uruchomienie procedury rozruchu
void SetRozruch(void)
{
	M._VWYBIEG=0;
	M._DE_VNT=0;
	M.VWB=0;
	//-----
	M._ROZRUCH=1;
	M.ROZ=0;
}
//------------------
//------------------
//Decyzja o wygaszeniu palnika
void DecWylacz(void)
{
	if(!M._WYLACZ)
	if(M._ROZRUCH||_GRZEJ_GetValue())
	{
		if(!M._pob_KCW&&!M._pob_CW) SetWylacz();
		else
		if(M._RUSZ_PLOMIEN||M._RUSZ_CIAG||M._RUSZ_STB||M._RUSZ_NTC_CW) SetWylacz();
		else
		if(M._pob_KCW)														//konfiguracja obieg CW
		{
			if(DtCW._ad_CW)	SetWylacz();						
		}
		else
		if(M._pob_CW)											//grzanie obiegu CW
		{
 			if(DtCW._ad_CW) SetWylacz();						
		}						
	}
}		
//Uruchomienie procedury wygaszenia palnika
void SetWylacz(void)
{
	GrzejOFF();//_GRZEJ=0;
	if(M._TURBO&&_VNT_GetValue()) M._VWYBIEG=1;		//dla kotla turbo uaktywnij wybieg wentylatora
	M.ROZ=0;
	M.MNM=0;
	M._ROZRUCH=0;
	M._RUN_MNMOD=0;
	M._AKTYWNE_SRW=0;
    M._initVNT=0;
}
//------------------
//------------------
void FunkcjaSerwisowa(void)
{

	if(M._AKTYWNE_KNF||!M._PRACA)
	{
		M.SRV=0;
		M._AKTYWNE_SRW=0;		//deaktywacja funkcji serwisowej		
		return;
	}
	if(M._AKTYWNE_SRW&&RdPrt(S_KDW)) M.SRV=3;
	switch(M.SRV)
	{
		case 0:
		{	
			if(RdPrt(S_KUP)&&(DtCW.PCW==F1_MAX_PCW))
			{
				M.SRV=1;
				StartRTS(_RTSRV0);
			}
			break;
		}
		case 1:
		{
			if(RdPrt(S_KUP)&&(DtCW.PCW==F1_MAX_PCW))
			{
				if(RTS(_RTSRV0)<_TSRV0) return;
				M.SRV=2;
				M._AKTYWNE_SRW=1;		//aktywacja funkcji serwisowej					
				StartRTM(_RTSRV1);
				StartRTS(_RTSRV0);
				PrintNstCW(DtCW.PCW,1);
			}
			else
			{
				M.SRV=0;
			}
			break;
		}
		case 2:
		{
			if(RTM(_RTSRV1)<_TSRV1) 
			{
				if((RTS(_RTSRV0)>=_TSRV0)&&RdPrt(S_KUP)) PrintNstCW(DtCW.PCW,1);
				return;
			}
			else
			{
				M.SRV=0;
				M._AKTYWNE_SRW=0;		//deaktywacja funkcji serwisowej
			}
			break;
		}
		case 3:
		{
			if(!RdPrt(S_KDW))
			{
				M.SRV=0;
				M._AKTYWNE_SRW=0;		//deaktywacja funkcji serwisowej
			}
			break;
		}
	}
}
//------------------
//------------------
//Zalaczenie plytki zapalacza
void GrzejON(void)
{
	_GRZEJ_SetHigh();
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	_VNT_SetHigh();
}
//------------------
//------------------
//Wylaczenie plytki zapalacza
void GrzejOFF(void)
{
	if(!M._TURBO)
	{
		_VNT_SetLow();
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		_GRZEJ_SetLow();
	}
	else
	{
		_GRZEJ_SetLow();
	}
}
//------------------
//------------------
//Procedura rozruchu kotla
void Rozruch(void)
{
    volatile unsigned char mc;
	switch(M.ROZ)
	{
		case 0:
		{
			M._ZAPLON=0;
            M._initVNT=1;
			WritePWM(PWM.MDMOD);
			M.ROZ=1;
			StartRTS(_RTROZ);
		}
		//oczekiwanie az sygnal obecnosci ciagu zniknie przy wylaczonym wentylatorze
		case 1:
		{
#if _NO_PRS==0
			if(M._TURBO)
			{
#if _EX_PRS==0
				if(RdPrt(S_CIG_NO)&&!_VNT_GetValue())		//obecny sygnal ciagu?
#else
				if((RdPrt(S_CIG_NO)||RdPrt(S_CIG2))&&!_VNT_GetValue())		//obecny sygnal ciagu?                    
#endif
				{
					if(RTS(_RTROZ)<_TCIRO) return;
					M.RCI=0;
					M._RUSZ_CIAG=1;
                    M._initVNT=0;
					return;
				}
				_VNT_SetHigh();							//uruchom wentylator	
			}
#else
            _VNT_SetHigh();
#endif
			M.ROZ=2;
			StartRTS(_RTROZ);
		}
		//oczekiwanie na sygnal obecnosci ciagu przy zalaczonym wentylatorze
		case 2:
		{
#if _NO_PRS==0            
			if(M._TURBO)
			{
#if _EX_PRS==0
				if(!RdPrt(S_CIG_NO_1S)&&_VNT_GetValue())		//nieobecny sygnal ciagu?
#else  
                    
				if((!RdPrt(S_CIG_NO_1S)||!RdPrt(S_CIG2))&&_VNT_GetValue())		//nieobecny sygnal ciagu?                        
#endif
				{
					if(RTS(_RTROZ)<_TCIRO) return;
					M.RCI=0;
					M._RUSZ_CIAG=1;
                    M._initVNT=0;
					return;
				}
			}
#endif
			M.ROZ=3;
			StartRTS(_RTROZ);
		}
		//kontrola poprawnosci dzialania ukladu detekcji plomienia
		case 3:
		{
#if _NO_PRS==0              
			if(M._TURBO&&!RdPrt(S_CIG_NO_1S))		//nieobecny sygnal ciagu?
			{
				M.RCI=0;
				M._RUSZ_CIAG=1;
                M._initVNT=0;
				return;
			}  
#endif
			if(RdPrt(S_PLM)&&!_GRZEJ_GetValue())			//obecny plomien mimo ze _GRZEJ=0?
			{
				if(RTS(_RTROZ)<_TBPL0) return;
				M.ERR_BTY=0x06;
				PrintErr(0xE6,1);				//wyswietl kod bledu
				ErrPTG();
			}
            
            M._initVNT=0;
/*
#if _VPID==1
            WritePWM3H(SetVNT(DtKNF.strv));
#else
            mc=VNTtoPPWM(DtKNF.strv);
            ProcToVNT(mc);            
#endif
*/
            if((M.VVNT>=DtKNF.strv-3)&&(M.VVNT<=DtKNF.strv+3))      //osiagnieto predkosc oczekiwana?
            {            
                PrintNstCW(DtCW.PCW,0);				//realizowana bedzie funkcja grzania                
                GrzejON();//_GRZEJ_SetHigh();							//zapal plomien na palniku
                M.ROZ=4;
                StartRTS(_RTROZ);
            }
            else
            {
                if(RTS(_RTROZ)>_TVPOM)              //minal czas oczekiwania na prawidlowa wartosc predkosci startowej?
                {
                    M.RCI=0;
                    M._RUSZ_CIAG=1;
                    M._initVNT=0;
                }
                return;
            }
		}
		//oczekiwanie na sygnal obecnosci plomienia
		case 4:
		{
#if _NO_PRS==0            
			if(M._TURBO&&!RdPrt(S_CIG_NO_1S))		//nieobecny sygnal ciagu?
			{
				M.RCI=0;
				M._RUSZ_CIAG=1;
				return;
			}
#endif
			if(RdPrt(S_MOD_ON)) WritePWM(PWM.MSTART);
			else WritePWM(PWM.MDMOD);

			if(RTS(_RTROZ)<_TITPL) return;			//okres czasu gdy ignorowany jest syg. plomien 

			if(RTS(_RTROZ)<_TDTPL)
			{
				if(RdPrt(S_PLM)) 					//stwierdzono obecnosc plomienia?
				{
					M.ROZ=5;
					StartRTS(_RTROZ);
				}
				return;
			}
			else								//nie stwierdzono plomienia w czasie _TDTPL? 
			{
				DecyzjaPLM();
				return;
			}
		}
		//wybieg na mocy startowej
		case 5:								
		{
			if(RdPrt(S_MOD_ON)) WritePWM(PWM.MSTART);
			else WritePWM(PWM.MDMOD);         
#if _NO_PRS==0            
			if(M._TURBO&&!RdPrt(S_CIG_NO_1S))		//nieobecny sygnal ciagu?
			{
				M.RCI=0;
				M._RUSZ_CIAG=1;
				return;
			}
#endif
			if(!RdPrt(S_PLM)) 					//stwierdzono brak plomienia?		
			{
				DecyzjaPLM2();
				//return;
			}
			/*if(!M._AKTYWNE_KNF)
			{
 				WritePWM(FirstPCW4());
			}*/
			if(RTS(_RTROZ)<_TDWPL) return;	
			M._ZAPLON=1;
			M._ROZRUCH=0;
            StartRTS(_RTVWB2);       //aktywacja wybiegu na mocy startowej dla wentylatora
            M._SVWB=1;
			M.ROZ=0;
		}
	}
}

void DecyzjaPLM(void)
{
	if(M._OST_PL)		//ostatnia proba?
	{
		M.ERR_BTY=0x01;
		PrintErr(0xE1,1);
		ErrPTG();
	}
	else
	{
		M._RUSZ_PLOMIEN=1;
		if(!M.LICZNIK_PL) M._RUSZ_RPLM=1;		//znacznik pierwszej aktywacji proc. w trakcie rozruchu
		M._ROZRUCH=0;
		M.ROZ=0;
	}
}
void DecyzjaPLM2(void)
{
	if(M._TURBO)	//sterownik TURBO?
	{
		if(_VNT_GetValue())	//wentylator pracuje?
		{
			//if(RdPrt(S_CIG_NO)&&RdPrt(S_STB))
#if _NO_PRS==0            
			if(_CIG_NO_GetValue()&&_STB_GetValue())
#else
            if(_STB_GetValue())    
#endif
			{
				if(M._OST_PL)		//ostatnia proba?
				{
					M.ERR_BTY=0x01;
					PrintErr(0x01,1);
					ErrPTG();
				}
				else
				{
					M.RPL=0;
					M._RUSZ_PLOMIEN=1;
					if(!M.LICZNIK_PL) M._RUSZ_RPLM=1;		//znacznik pierwszej aktywacji proc. w trakcie rozruchu
					M._ROZRUCH=0;
					M.ROZ=0;
				}
			}
		}
		else		//wentylator nie pracuje
		{
			if(RdPrt(S_STB))
			{
				if(M._OST_PL)		//ostatnia proba?
				{
					M.ERR_BTY=0x01;
					PrintErr(0x01,1);
					ErrPTG();
				}
				else
				{
					M.RPL=0;
					M._RUSZ_PLOMIEN=1;
					if(!M.LICZNIK_PL) M._RUSZ_RPLM=1;		//znacznik pierwszej aktywacji proc. w trakcie rozruchu
					M._ROZRUCH=0;
					M.ROZ=0;
				}
			}	
		}
	}
	else	//sterownik ATMOSFERYCZNY
	{
		//if(RdPrt(S_CIG_NC)&&RdPrt(S_STB))
		if(_CIG_NC_GetValue()&&_STB_GetValue())
		{
			if(M._OST_PL)		//ostatnia proba?
			{
				M.ERR_BTY=0x01;
				PrintErr(0x01,1);
				ErrPTG();
			}
			else
			{
				M.RPL=0;
				M._RUSZ_PLOMIEN=1;
				if(!M.LICZNIK_PL) M._RUSZ_RPLM=1;		//znacznik pierwszej aktywacji proc. w trakcie rozruchu
				M._ROZRUCH=0;
				M.ROZ=0;
			}
		}			
	}
}
//------------------
//------------------
//Wybieg wentylatora
void VWybieg(void)
{
	switch(M.VWB)
	{
		case 0:
		{
			M.VWB=1;
			M._DE_VNT=1;
			StartRTS(_RTVWB);
		}
		case 1:
		{	
			if(RTS(_RTVWB)>=_TVWB0) M._DE_VNT=0;	//wyzeruj znacznik probnego wybiegu wentylatora
            if(RTS(_RTVWB)<_TVWB1) return;
			if(M._ZAPLON||M._RUSZ_CIAG||M._RUSZ_PLOMIEN)
			{
				if(RTS(_RTVWB)<_TVWB1) return;
			}
			else
			{
				if(RTS(_RTVWB)<_TVWB2) return;
			}
			_VNT_SetLow();									//wylacz wentylator
			M._DE_VNT=1;
			M.VWB=2;
		}
		case 2:
		{
			if(M._ZAPLON||M._RUSZ_CIAG||M._RUSZ_PLOMIEN)
			{
				if(RTS(_RTVWB)<_TVWB1+2) return;		//beznapieciowy wybieg wentylatora
			}
			else
			{
				if(RTS(_RTVWB)<_TVWB2+2) return;		//beznapieciowy wybieg wentylatora
			}
			M._DE_VNT=0;
			M._VWYBIEG=0;
			M.VWB=0;
		}
	}
}
void VSWybieg(void)
{
    if(!M._PRACA||(RTS(_RTVWB2)>=_TVSTRT))
    {
        M._SVWB=0;	//wyzeruj znacznik wybiegu wentylatora na mocy startowej
    }
}
//------------------
//------------------
//Awaryjny rozruch plomienia
void RuszPlomien(void)
{
	switch(M.RPL)
	{
		case 0:
		{
			M.RPL=1;
			StartRTS(_RTRPL);
		}
		case 1:
		{	
			if(RTS(_RTRPL)<_TPLPR) return;
			M.LICZNIK_PL++;
			if(M._RUSZ_RPLM)	//procedura aktywowana w trakcie rozruchu?
			{
				if(!PWM.tpGZ) 				//gaz ziemny
				{
					if(M.LICZNIK_PL>=_NCYKLI_PL-1)
					{
					 	M._OST_PL=1;
						M._RUSZ_RPLM=0;
					}
				}
				else
				{
					if(M.LICZNIK_PL>=_NCYKLI_GPL-1)
					{
					 	M._OST_PL=1;
						M._RUSZ_RPLM=0;
					}
				}				
			}
			else
			{
				if(!PWM.tpGZ) 				//gaz ziemny
				{
					if(M.LICZNIK_PL>=_NCYKLI_PL)
					{
					 	M._OST_PL=1;
						M._RUSZ_RPLM=0;
					}
				}
				else
				{
					if(M.LICZNIK_PL>=_NCYKLI_GPL)
					{
					 	M._OST_PL=1;
						M._RUSZ_RPLM=0;
					}
				}
			}
			StartRTdS(_RTRSPL);
			M._RUSZ_PLOMIEN=0;
			M.RPL=0;
		}
	}
}
//Zerowanie procedury awaryjnego rozruchu plomienia
void ReloadPlomien(void)
{
	if(!M._RUSZ_PLOMIEN&&!M._ZMKCOCW&&!M._ROZRUCH&&(RTdS(_RTRSPL)>9))
	{
		M._RUSZ_RPLM=0;
		M._OST_PL=0;
		M.LICZNIK_PL=0;
	}
}
//------------------
//------------------
//Awaryjny rozruch ciagu
/*
void RuszCiag(void)
{
	if(!M._TURBO) //sterownik atmosferyczny
	{
		switch(M.RCI)
		{
			case 0:
			{
				M.RCI=1;
				M._prb_CIG=0;
				StartRTS(_RTRCI);
				StartRTM(_RTRCI2);
			}
			case 1:
			{	
				if(RTS(_RTRCI)>1) M._prb_CIG=1; 
				//dla kotla atmosferycznego
				if(RdPrt(S_CIG_NC))
				{
					M.RCI=0;
					M._RUSZ_CIAG=0;
					M._prb_CIG=0;
					return;
				}
				if(RTM(_RTRCI2)<_TCIPR2) return;
				M.ERR_BTY=0x03;
				PrintErr(0xE3,1);
				ErrPTG();
			}
		}
	}
	else		//sterownik turbo
	{
		switch(M.RCI)
		{
			case 0:
			{
				M._prb_CIG=0;
				StartRTS(_RTRCI);
				if(_VNT_GetValue()) 				//wentylator pracuje?
				{
					M._VWYBIEG=1;		//zastartuj wybieg wentylatora 
					M.VWB=0;			
					M.RCI=1;
				}
				else M.RCI=2;
				break;
			}
			case 1:						//wentylator pracuje - proba styku
			{	
				if(RTS(_RTRCI)>1) M._prb_CIG=1;
				//dla kotla turbo
				if(RdPrt(S_CIG_NO))		//pojawil sie sygnal ciagu?
				{
					M.RCI=3;
					StartRTS(_RTRCI);
					break;
				}	
				if(RTS(_RTRCI)<_TCIPR0-1) return;
				M.ERR_BTY=0x03;
				PrintErr(0xE3,1);
				ErrPTG();
			}
			case 2:		//wentylator nie pracuje - proba styku
			{	
				if(RTS(_RTRCI)>1) M._prb_CIG=1;
				//dla kotla turbo
				if(!RdPrt(S_CIG_NO))	//zanikl sygnal ciagu
				{
					M.RCI=3;
					StartRTS(_RTRCI);
					break;
				}				
				if(RTS(_RTRCI)<_TCIPR0-1) return;
				M.ERR_BTY=0x03;
				PrintErr(0xE3,1);
				ErrPTG();
			}
			case 3:		//podtrzymanie stanu (zapobieganie impulsowemu zalaczeniu)
			{
				if(RTS(_RTRCI)<_TCIPR1) return;
				M._prb_CIG=0;
				M._RUSZ_CIAG=0;
				M.RCI=0;
			}
		}
	}
}
//------------------
//------------------
//Zerowanie licznuka ponowien awarii ciagu
void ReloadCiag(void)
{
	if(RTS(_RTRSCI)>_TCIRLP)
	{
		M.LICZNIK_CIG=0;
	}
}*/
#if _NO_PRS==0
void RuszCiag(void)
{
	if(!M._TURBO) //sterownik atmosferyczny
	{
		switch(M.RCI)
		{
			case 0:
			{
				M.RCI=1;
				M._prb_CIG=0;
				StartRTS(_RTRCI);
				StartRTM(_RTRCI2);
			}
			case 1:
			{	
				if(RTS(_RTRCI)>1) M._prb_CIG=1; 
 				if(RTM(_RTRCI2)<_TCIPR2) return;
				M.LICZNIK_CIG2++;
				if(M.LICZNIK_CIG2>=_NCYKLI_CIG2)
				{
					M.LICZNIK_CIG2=0;
					M.ERR_BTY=0x03;
					PrintErr(0xE3,1);
					ErrPTG();
				}
				M.RCI=0;
				M._RUSZ_CIAG=0;
				M._prb_CIG=0;
				StartRTM(_RTRCI3);
				return;
			}
		}
	}
	else		//sterownik turbo
	{
		switch(M.RCI)
		{
			case 0:
			{
				M._prb_CIG=0;
				StartRTS(_RTRCI);
				if(_VNT_GetValue()) 				//wentylator pracuje?
				{
					M._VWYBIEG=1;		//startuj procedure wybiegu wentylatora 
					M.VWB=0;			
					M.RCI=1;
				}
				else M.RCI=2;
				break;
			}
			case 1:						//wentylator pracuje - proba styku
			{	
				if(RTS(_RTRCI)>1) M._prb_CIG=1;
				//dla kotla turbo
#if _EX_PRS==0
				if(RdPrt(S_CIG_NO))		//pojawil sie sygnal ciagu?
#else
				if(RdPrt(S_CIG_NO)&&RdPrt(S_CIG2))		//pojawil sie sygnal ciagu?
#endif                    
				{
					M.RCI=4;
					//M.LICZNIK_CIG3=0;	//wyzeruj licznik prob
					StartRTS(_RTRCI);
					break;
				}	
				if(RTS(_RTRCI)<_TCIPR0-1) return;
				M.RCI=3;
				StartRTS(_RTRCI);
				break;
			}
	//---------------------------------
			case 2:						//wentylator nie pracuje - proba styku
			{	
				if(RTS(_RTRCI)>1) M._prb_CIG=1;
				//dla kotla turbo
#if _EX_PRS==0                
				if(!RdPrt(S_CIG_NO))	//zanikl sygnal ciagu
#else
				if(!RdPrt(S_CIG_NO)&&!RdPrt(S_CIG2))	//zanikl sygnal ciagu                    
#endif
				{
					M.RCI=4;
					StartRTS(_RTRCI);
					break;
				}				
				if(RTS(_RTRCI)<_TCIPR0-1) return;
				M.ERR_BTY=0x03;
				PrintErr(0xE3,1);
				ErrPTG();
			}
	//-----------------------------------
			case 3:		//blokada 5 sek.
			{	
				_VNT_SetLow();								//stop wentylatora
 				if(RTS(_RTRCI)<_TCIBLK-1) return;
				M.LICZNIK_CIG3++;
				if(M.LICZNIK_CIG3>=_NCYKLI_CIG3)	//ostatnia proba?
				{
					M.LICZNIK_CIG3=0;
					M.ERR_BTY=0x03;
					PrintErr(0xE3,1);
					ErrPTG();
				}
				M.RCI=4;
				StartRTS(_RTRCI);
				break;
			}
			case 4:		//podtrzymanie stanu (zapobieganie impulsowemu zalaczeniu)
			{
				if(RTS(_RTRCI)<_TCIPR1) return;
				StartRTS(_RTRSCI2);
				M._prb_CIG=0;
				M._RUSZ_CIAG=0;
				M.RCI=0;
				return;
			}
		}
	}
}
//------------------
//------------------
//Zerowanie licznuka ponowien awarii ciagu
void ReloadCiag(void)
{
	if(!M._TURBO) //sterownik atmosferyczny
	{
		if(!M._RUSZ_CIAG&&RTM(_RTRCI3)>_TCIPR3) M.LICZNIK_CIG2=0;	//wyzeruj licznik ponowien procedury po 60min.
	}
	else
	{
		if(RTS(_RTRSCI)>_TCIRLP)					//(turbo) po minucie wyzeruj licznik_zaników_ci¹gu/min //zliczane kolejne zaniki
		{
			M.LICZNIK_CIG=0;
		}
		if(!M._RUSZ_CIAG&&(RTS(_RTRSCI2)>_TCITRS))	//(turbo) po minucie wyzeruj licznik_prob_ci¹gu/min
		{
			M.LICZNIK_CIG3=0;
		}		
	}
}
#endif
//------------------
//------------------
//------------------
//------------------
//------------------
//------------------
//------------------
//------------------
//------------------
//------------------
//------------------
//------------------
//------------------
//Zdarzenie resetujace
/*bit KeyRes(void)
{
	switch(M.KRS)
	{
		case 0:
		{
			if(RdPrt(S_WCW))	//pobudzenie aktywne
			{
				M.KRS=1;			
			}
			else				//pobudzenie nieaktywne
			{
				M.KRS=2;			
			}
			break;
		}
		case 1:
		{	
			if(!RdPrt(S_WCW))	//pobudzenie nieaktywne
			{
				M.KRS=3;			
			}
			break;
		}
		case 2:
		{	
			if(RdPrt(S_WCW))	//pobudzenie aktywne
			{
				M.KRS=3;			
			}
			break;
		}
		case 3:
		{	
			if(RdPrt(S_WCW))	//pobudzenie aktywne
			{
				M.KRS=4;
				StartRTS(_RTSRV0);
			}
			break;
		}
		case 4:
		{
			if(RdPrt(S_WCW))	//pobudzenie aktywne przez min 1 sek.
			{
				if(RTS(_RTSRV0)<_TKRES) return 0;
				return 1;
			}
			else
			{
				M.KRS=0;
			}
		}
	}
	return 0;
}*/
bit KeyRes(void)
{
	switch(M.KRS)
	{
		case 0:
		{
			if(RdPrt(S_WCW))	//pobudzenie aktywne
			{
				M.KRS=1;			
			}
			else				//pobudzenie nieaktywne
			{
				M.KRS=4;			
			}
			break;
		}
		//---------------------------awaria przy zalaczonym CW
		case 1:
		{	
			if(!RdPrt(S_WCW))	//pobudzenie nieaktywne
			{
				M.KRS=2;			
			}
			break;
		}

		case 2:
		{	
			if(RdPrt(S_WCW))	//pobudzenie aktywne
			{
				M.KRS=3;
				StartRTS(_RTSRV0);
			}
			break;
		}
		case 3:
		{
			if(RdPrt(S_WCW))	//pobudzenie aktywne przez min 1 sek.
			{
				if(RTS(_RTSRV0)<_TKRES) return 0;
				return 1;
			}
			else
			{
				M.KRS=1;
			}
			break;
		}
		//---------------------------awaria przy wyl¹czonym CW
		case 4:
		{	
			if(RdPrt(S_WCW))	//pobudzenie aktywne
			{
				M.KRS=5;			
			}
			break;
		}
		case 5:
		{	
			if(!RdPrt(S_WCW))	//pobudzenie nieaktywne
			{
				M.KRS=6;
				StartRTS(_RTSRV0);
			}
			break;
		}
		case 6:
		{
			if(!RdPrt(S_WCW))	//pobudzenie nieaktywne przez min 1 sek.
			{
				if(RTS(_RTSRV0)<_TKRES) return 0;
				return 1;
			}
			else
			{
				M.KRS=4;
			}
			break;
		}
	}
	return 0;
}
//------------------
//------------------
//Detekcja poprawnosci pracy wentylatora
void KontrolaWentylatora(void)
{
	if(!_VNT_GetValue()||!PWM.BufPWM3) 									//wentylator stoi
	{
		StartRTS(_RTRCI);
	}
	else
	{
		if(M.VVNT) 											//predkosc niezerowa?
		{
			StartRTS(_RTRCI);	
		}
		else
		{	
			if(RTS(_RTRCI)>_TCIBLK)	
			{
				_VNT_SetLow();
#if _HIRESVNT==0 
                WritePWM3(0);
#else
                WritePWM3H(0);
#endif
				M.ERR_BTY=0x03;
				PrintErr(0x03,1);
				ErrPTG();
			}
		}	
	}
}
//------------------
//------------------
//Przerwa czasowa na wypadek stanow nieustalonych przy zaniku napiecia
void VZPauza(void)
{
	StartRTS(_RTROZ);
	while(1)
	{
		CLRWDT();
		if(RTS(_RTROZ)>=1) return;
	}
}
//------------------
//------------------
//Petla wylaczenia awaryjnego
void ErrPTG(void)
{
	unsigned char k=0;

	GrzejOFF();//_GRZEJ=0;
	VZPauza();															//przerwa czasowa na wypadek stanow nieustalonych
	if((M.ERR_BTY>=1)&&(M.ERR_BTY<=3)) if_WriteEEPROM(_ADRERR,M.ERR_BTY);//WriteEEPROM(_ADRERR,M.ERR_BTY); 	//zapis kodu bledu do pamieci
	M._ROZRUCH=0;
	M._MODULUJ=0;
	WritePWM(0);
	if(M._TURBO&&_VNT_GetValue()) M._VWYBIEG=1;		//aktywacja wybiegu wentylatora
	M.KRS=0;
//------

	while(1)
	{
		CLRWDT();
//blok pomiarowy
		PomiarPCW();			//wartosc nastawy CW
		PomiarCW();				//wartosc temperatury CW
//blok wykonawczy
		if(M._VWYBIEG) VWybieg();			//wybieg wentylatora
//obsluga zdarzenia resetujacego
		if(KeyRes()) 
		{
			_VNT_SetLow();
			PrintPR(BLACK,BLACK,1);
			EndErr();
			ToReset();
		}
	}
}
//Wymuszenie resetu sterownika.
void ToReset(void)
{
	_VNT_SetLow();
	if_WriteEEPROM(_ADRERR,0x00);
	while(1);
}
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------
#endif
