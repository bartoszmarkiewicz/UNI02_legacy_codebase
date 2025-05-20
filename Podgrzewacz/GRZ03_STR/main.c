/*main.c*/
/*Oprogramowanie sterownikow do podgrzewaczy gazowych typu turbo*/
/*Sterownik na bazie p?yty UNI-02 wersja 3.3V...*/
//Konwersja oprogramowania z p?ytki UNI-01 na UNI-02 (08.05.2014)
//(Wyswietlacz szeregowy, zoptymalizowana procedura modulacji, skrocony wybieg wentylatora gdy nie nastapil zaplon gazu)
#include <pic18.h>
#include <htc.h>
#include <stdio.h>
#include <string.h>
#include "global.h"			//parametry globalne
#include "main.h"			//parametry lokalne
#include "wsw2.h"
#include "print.h"
#include "eeprom.h"
#include "pwm.h"
#include "czcw.tp.h"
#include "pomiar.h"

#include "keypad.h"
#include "konfig.h"

#include "rgl.h"

tpMAIN M;
extern DtPWM PWM;			//pwm.c
extern DataPrCW PrCW;		//czcw.c
extern DataCO DtCO;			//pomiar.c
extern DataCW DtCW;
extern DataZW DtZW;
extern DataPFN PFN;
extern DataCSN DtCS;
extern DtKONF DtKNF;

extern DtRgl rgl;
unsigned char x,y,t,ty;
extern StPin sp;

#if _SRVTRB==1						//aktywny tryb serwisowy kompilacji?
extern unsigned char mypwm;
#endif

void main(void)
{
//------------------
//------------------
	InitOsc();
	InitGlobal();
	InitTimer1();
	InitTimer3();    
	InitPort();
	InitAC();
	InitWsw();
	InitPWM();
	InitI2C();
	InitDataPmr();
	ClearData();
	
//------------------
//------------------
    EndOFF();
	if (!POR||!BOR)							//zalaczono zasilanie?
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
	if (!POR||!BOR)		//zalaczono zasilanie? 
	{	
		POR=1;
		BOR=1;
		EnKonf();		//aktywacja oczekiwania na wejscie do konfiguracji
	}
	//Odczytaj parametry zapisane w pamieci EEPROM
	RdEEParam();

	//Sprawd? czy nastapilo wylaczenie awaryjne z blokada
	if(M.ERR_BTY=eeprom_read(_ADRERR)) 		//uruchomienie po wylaczeniu awaryjnym?
	{
		if(((M.ERR_BTY>=1)&&(M.ERR_BTY<=3))||(M.ERR_BTY==0x07))	//dozwolony kod bledu?
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
				case 6:
					PrintErr(0xE6,1);		//wyswietl kod bledu
				break;                
				case 7:
					PrintErr(0xE7,1);		//wyswietl kod bledu
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
	_VNT=0;									//wentylator na stale wylaczony
	_GRZEJ=0;

	//decyzja czy kociol jest typu TURBO
	M._TURBO=1;
	RestartFltPrt();
	StartRTS(_RTSTR);						//start timera rozruchu sterownika	
    //-----------
	PomiarVNT();							//predkosc obrotowa wentylatora  
	if(M._PW_START)
	{
 		KalibracjaWentylatora();
	}
	else 
	{
		OdczytDanychKalibracji();
	}
    
//------------------
//------------------
//------------------
	while(1)					//petla glowna programu				
	{
		CLRWDT();
		M._PRACA=M._ROZRUCH||_GRZEJ;
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
		KolejkaPobudzen();		//aktywacja pobudzen wg priorytetu	
		KontrolaWentylatora();	//kontrola poprawnosci pracy wentylatora        
	//----------------

		if(M._ROZRUCH) Rozruch();			//rozruch kotla
		if(M._VWYBIEG) VWybieg();			//wybieg wentylatora
		ReloadPlomien();					//decyzja o wyzerowaniu licznika prob przy braku plomienia
		if(M._RUSZ_PLOMIEN) RuszPlomien();	//awaryjna procedura przy braku plomienia
		if(M._MODULUJ) Moduluj();			//algorytm modulacji
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

	_VNT=0;
	_GRZEJ=0;
}
void RestartFltPrt(void)
{
	SetPrt(S_WCW,0);
}

//Procedura kalibracji wentylatora
void KalibracjaWentylatora(void)
{
	volatile unsigned char x;

	M._PO_KAL=0;
	_VNT=0;						//wentylator na stale wylaczony
	WritePWM(0);				//predkosc obrotowa wentylatora = 0
	PomiarVNT();				//predkosc obrotowa wentylatora
	while(!(x=KalibracjaVNT()))
	{
 		CLRWDT();
		//***************************************************
		PomiarCW();				//wartosc temperatury CW
		PomiarPCW();			//wartosc nastawy CW
		PomiarVNT();			//predkosc obrotowa wentylatora
	};
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
    if(_GRZEJ&&!RdPrt(S_PLM)) 
    {     
        PrintOFF();                                 //zapobieganie migotaqniu wyswietlacza
        EndNst();
    }
    else
    {
        if(!RdPrt(S_PLM))
        {
#if _OFFTMP==1        
            PrintOFF(); 
#else
            EndOFF();
            PrintCW(DtCW.SCW,(M._AKTYWNE_SRW)?1:0);	//wymuszenie CW        
#endif                   
        }
        else 
        {
            EndOFF();
            PrintCW(DtCW.SCW,(M._AKTYWNE_SRW)?1:0);	//wymuszenie CW		    
        }
        //nastawa PCW
        //if(!(_GRZEJ&&!RdPrt(S_PLM))&&DtCW._newPCW)
        if(DtCW._newPCW)
        {
            EndOFF();
            PrintNstCW(DtCW.PCW,(M._AKTYWNE_SRW)?1:0);
        }
    }
	/*if(!_GRZEJ)
    {
        PrintOFF(); 
    }
	else 
	{
        if(RdPrt(S_PLM))    //wygas wskaz temperatury na czas iskrzenia (duze zaklucenia przy zapalaniu elektroda na czujniku plomienia)
        {
            EndOFF();
            PrintCW(DtCW.SCW,(M._AKTYWNE_SRW)?1:0);	//wymuszenie CW		    
        }
	}
	//nastawa PCW
	if(DtCW._newPCW) 
    {
        EndOFF();
        PrintNstCW(DtCW.PCW,(M._AKTYWNE_SRW)?1:0);
    }*/   
}
//------------------
//------------------
//Reakcja na wiekszosc stanow awaryjnych 
/*void DecERR(void)
{
	unsigned char KnfDisErr=0;
	unsigned char w;
//Znacznik wylaczajacy wizualizacje kodu bledu podczas pracy w proc. konfiguracji

	KnfDisErr=(M._AKTYWNE_KNF&&(DtKNF.KRK==1||(DtKNF.KRK==2&&(DtKNF.PRM>=3))))?1:0;
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
		w=DtCW._err_CW;
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
	if(!M._RUSZ_PLOMIEN&&!RdPrt(S_PLM)&&!M._ROZRUCH&&_GRZEJ) 
	{
		if(M._TURBO)	//sterownik TURBO?
		{
			if(_VNT)	//wentylator pracuje?
			{
				if(RdPrt(S_CIG_NO)&&RdPrt(S_STB))
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
		//if(!M._RUSZ_CIAG&&!M._DE_VNT&&((!RdPrt(S_CIG_NO)&&_VNT)||(RdPrt(S_CIG_NO)&&!_VNT)))
		if(!M._RUSZ_CIAG&&!M._ROZRUCH&&!M._DE_VNT&&((!RdPrt(S_CIG_NO)&&_VNT)||(RdPrt(S_CIG_NO)&&!_VNT))) 
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

//--------REAKCJA NA BRAK SYTUACJI AWARYJNEJ (DOTYCZY WIZUALIZACJI KODU AWARYJNEGO)

	//usun kod awaryjny z wyswietlacza
	if(!M._RUSZ_PLOMIEN&&!M._RUSZ_CIAG&&!M._RUSZ_NTC_CW&&!M._RUSZ_MOD)
	{
		M.ERR_BTY=0x00;
		EndErr();
	}
}*/
void DecERR(void)
{
	unsigned char KnfDisErr=0;
	unsigned char w;
//Znacznik wylaczajacy wizualizacje kodu bledu podczas pracy w proc. konfiguracji

	KnfDisErr=(M._AKTYWNE_KNF&&(DtKNF.KRK==1||(DtKNF.KRK==2&&(DtKNF.PRM>=3))))?1:0;
//--------BEZWZGLEDNE WYLACZENIE Z BLOKADA
//UWAGA: natychmiastowe wyswietlenie kodu awaryjnego
//zadzialalo STB
	if(!RdPrt(S_STB))
	{
		M.ERR_BTY=0x02;
		PrintErr(0xE2,1);
		ErrPTG();
	}
//zadzialal czujnik ciagu
	if(!RdPrt(S_CIG_NC))
	{
		M.ERR_BTY=0x03;
		PrintErr(0xE3,1);
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
	{
		w=DtCW._err_CW;
		//awaria czujnika CW
		if(w)
		{
			M.ERR_BTY=0x04;
			if(KnfDisErr) EndErr();		//nie wyswietlaj kodu bledu przy konfiguracji par. r4 i r6
			else PrintErr(0xE4,0);
			M._RUSZ_NTC_CW=1;
		}
		else 
		{
			M._RUSZ_NTC_CW=0;
		}
	}
//--------START PROCEDUR NAPRAWCZYCH - WYLACZENIE Z BLOKADA
//UWAGA: bez wizualizacji kodu awaryjnego (wyswietlany dopiero po wykonaniu procedury naprawczej)
    
	//brak plomienia (praca kotla)
	if(!M._RUSZ_PLOMIEN&&!RdPrt(S_PLM)&&!M._ROZRUCH&&_GRZEJ) 
	{
		if(RdPrt(S_CIG_NC)&&RdPrt(S_STB))
		{
			M.RPL=0;
			M._RUSZ_PLOMIEN=1;
		}
	}  
//--------REAKCJA NA BRAK SYTUACJI AWARYJNEJ (DOTYCZY WIZUALIZACJI KODU AWARYJNEGO)

	//usun kod awaryjny z wyswietlacza
	if(!M._RUSZ_PLOMIEN&&!M._RUSZ_NTC_CW)
	{
		M.ERR_BTY=0x00;
		EndErr();
	}
}
//------------------
//------------------
//Decyzje sterujace
/*
void DecSTR(void)
{
	//wyzerowanie znacznikow rozruchu sterownika po wlaczeniu zasilania lub resecie
	if(RTS(_RTSTR)>=_TSTR)
	{
 		M._PW_START=0;
		M._RS_START=0;
	}
	//reakcja na zbyt szybki narost temperatury
	//if(!M._ROZRUCH&&_GRZEJ&&DtCO._ldDCO) M._RUN_MNMOD=1;
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
		if(!M._ROZRUCH&&_GRZEJ)
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
	if(!M._PRACA||!RdPrt(S_MOD_ON)||M._RUN_MNMOD)
	{
		M._MODULUJ=0;					//wylacz blok modulacji
		if(!M._PRACA||!RdPrt(S_MOD_ON)) WritePWM(0);
		else WritePWM(PWM.MDMOD);		//zbyt szybki narost temp.
	}
	//wymuszenie konkretnej wartosci wypelnienia dla danego wymuszenia
#endif
	if(M._pob_KCW&&!M._ROZRUCH&&_GRZEJ) WritePWM(PWM.MMAXCW);
#if _SRVTRB==1														//aktywny tryb serwisowy kompilacji?
	if(!M._ROZRUCH&&_GRZEJ&&RdPrt(S_MOD_ON)) WriteOPWM(mypwm);
	if(!M._PRACA||!RdPrt(S_MOD_ON)) WritePWM(0);
#else
	if(M._AKTYWNE_SRW&&!M._ROZRUCH&&_GRZEJ) WritePWM(_MG2_MOD);
#endif
}*/
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
	//--------------
	//Czasowe podbicie mocy minimalnej
	PWM.MDMOD=PWM.DKNF_MDMOD;	//praca z domyslna moca minimalna zdefiniowana w konfiguracji
	//--------------
	//aktywacja/deaktywacja bloku modulacji plomieniem dla pobudzenia CW
	if(M._pob_CW&&!M._AKTYWNE_SRW)
	{
		if(!M._ROZRUCH&&_GRZEJ)
		{
			if(!M._MODULUJ)
			{
				M._MODULUJ=1;
				RestartMod();	
			}
            
            //WritePWM(ProcToMod(0));
            //WritePWM(_VLO_PWM2);
		}
		else M._MODULUJ=0;
	}
	else M._MODULUJ=0;

	//deaktywacja bloku modulacji plomieniem dla awarii modulatora lub dla szybkiego narostu temp.
	if(!M._PRACA||M._RUN_MNMOD||(!M._PRACA&&M._VWYBIEG))
	{
		M._MODULUJ=0;	//wylacz blok modulacji
		if(M._RUN_MNMOD)
		{
			WritePWM(ProcToMod(0));//WritePWM(PWM.BufPWM=ModToDVNT(DtKNF.mmin,DtKNF.mmin,DtKNF.mmax,PWM.BufPWM));//WritePWM(PWM.MDMOD);		//zbyt szybki narost temp.
		}
		else
		if(M._VWYBIEG)
		{
			WritePWM(ProcToMod(0));//WritePWM(PWM.BufPWM=ModToDVNT(DtKNF.mmin,DtKNF.mmin,DtKNF.mmax,PWM.BufPWM));//WritePWM(PWM.MDMOD);		//wybieg wentylatora
		}
		else
		if(!M._PRACA)
		{
			WritePWM(0);
		}
		else WritePWM(0);
	}
	//wymuszenie konkretnej wartosci wypelnienia dla danego wymuszenia
	if(M._pob_KCW&&!M._ROZRUCH&&_GRZEJ) WritePWM(ProcToMod(DtKNF.mxcw));//WritePWM(PWM.BufPWM=ModToDVNT(ProcToVNT(DtKNF.dmxcw),DtKNF.mmin,DtKNF.mmax,PWM.BufPWM));//WritePWM(PWM.MMAXCW);
#if _SRVTRB==1														//aktywny tryb serwisowy kompilacji?
	if(M._AKTYWNE_SRW&&!M._ROZRUCH&&_GRZEJ) WritePWM(mypwm);
#else
	if(M._AKTYWNE_SRW&&!M._ROZRUCH&&_GRZEJ)
	{
        if(M._min_SRW) WritePWM(ProcToMod(0));
        else WritePWM(ProcToMod(99));    
	}
#endif
}
//Ustaw minimalny wydatek na modulatorze
void SetLoMod(void)
{
	M._MODULUJ=0;	//wylacz blok modulacji
	WritePWM(0);
}
//------------------
//------------------
//Decyzja o zapaleniu palnika
void DecRozruch(void)
{
	if(!M._ROZRUCH&&!_GRZEJ)
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
	if(M._ROZRUCH||_GRZEJ)
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
/*void SetWylacz(void)
{
	GrzejOFF();//_GRZEJ=0;
	if(M._TURBO&&_VNT) M._VWYBIEG=1;		//dla kotla turbo uaktywnij wybieg wentylatora
	M.ROZ=0;
	M.MNM=0;
	M._ROZRUCH=0;
	M._RUN_MNMOD=0;
	M._AKTYWNE_SRW=0;
}*/
void SetWylacz(void)
{
	GrzejOFF();
	//-----
	//-----
	if(_VNT) M._VWYBIEG=1;					//dla kotla turbo uaktywnij wybieg wentylatora
	M.ROZ=0;
	M.MNM=0;
	M._ROZRUCH=0;
	M._RUN_MNMOD=0;
	M._AKTYWNE_SRW=0;    
}
//------------------
//------------------
void FunkcjaSerwisowa(void)
{

	if(M._AKTYWNE_KNF||!M._PRACA)
	{
		M.SRV=0;
		M._AKTYWNE_SRW=0;		//dezaktywacja funkcji serwisowej		
		return;
	}
	//if(M._AKTYWNE_SRW&&RdPrt(S_KDW)) M.SRV=3; //wyjscie po nacisnieciu minus
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
				if(RTS(_RTSRV0)<_TSRV0) return;     //odczekaj az przytrzymanie przycisku potrwa 5 sek
				M.SRV=2;
				M._AKTYWNE_SRW=1;		//aktywacja funkcji serwisowej	
                M._min_SRW=0;           //praca na maksimum				
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
                if(RdPrt(S_KUP)) M._min_SRW=0;  //praca na maksimum mocy
                if(RdPrt(S_KDW)) M._min_SRW=1;  //praca na minimum mocy
				//if((RTS(_RTSRV0)>=_TSRV0)&&RdPrt(S_KUP)) PrintNstCW(DtCW.PCW,1);    //wskaz temperatury z kropk?
				return;
			}
			else
			{
				M.SRV=0;
				M._AKTYWNE_SRW=0;		//deaktywacja funkcji serwisowej
                M._min_SRW=0;
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
/*
//Zalaczenie plytki zapalacza
void GrzejON(void)
{
	_GRZEJ=1;
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	_VNT=1;
}
//------------------
//------------------
//Wylaczenie plytki zapalacza
void GrzejOFF(void)
{
	if(!M._TURBO)
	{
		_VNT=0;
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		_GRZEJ=0;
	}
	else
	{
		_GRZEJ=0;
	}
}
//------------------
//------------------
//Procedura rozruchu kotla
void Rozruch(void)
{
	switch(M.ROZ)
	{
		case 0:
		{
			M._ZAPLON=0;
			WritePWM(PWM.MDMOD);
			M.ROZ=1;
			StartRTS(_RTROZ);
		}
		//oczekiwanie az sygnal obecnosci ciagu zniknie przy wylaczonym wentylatorze
		case 1:
		{
			if(M._TURBO)
			{
				if(RdPrt(S_CIG_NO)&&!_VNT)		//obecny sygnal ciagu?
				{
					if(RTS(_RTROZ)<_TCIRO) return;
					M.RCI=0;
					M._RUSZ_CIAG=1;
					return;
				}
				_VNT=1;							//uruchom wentylator	
			}
			M.ROZ=2;
			StartRTS(_RTROZ);
		}
		//oczekiwanie na sygnal obecnosci ciagu przy zalaczonym wentylatorze
		case 2:
		{
			if(M._TURBO)
			{
				if(!RdPrt(S_CIG_NO_1S)&&_VNT)		//nieobecny sygnal ciagu?
				{
					if(RTS(_RTROZ)<_TCIRO) return;
					M.RCI=0;
					M._RUSZ_CIAG=1;
					return;
				}
			}
			M.ROZ=3;
			StartRTS(_RTROZ);
		}
		//kontrola poprawnosci dzialania ukladu detekcji plomienia
		case 3:
		{
			if(M._TURBO&&!RdPrt(S_CIG_NO_1S))		//nieobecny sygnal ciagu?
			{
				M.RCI=0;
				M._RUSZ_CIAG=1;
				return;
			}
			if(RdPrt(S_PLM)&&!_GRZEJ)			//obecny plomien mimo ze _GRZEJ=0?
			{
				if(RTS(_RTROZ)<_TBPL0) return;
				M.ERR_BTY=0x06;
				PrintErr(0xE6,1);				//wyswietl kod bledu
				ErrPTG();
			}
			PrintNstCW(DtCW.PCW,0);				//realizowana bedzie funkcja grzania
			GrzejON();//_GRZEJ=1;							//zapal plomien na palniku
			M.ROZ=4;
			StartRTS(_RTROZ);
		}
		//oczekiwanie na sygnal obecnosci plomienia
		case 4:
		{
			if(M._TURBO&&!RdPrt(S_CIG_NO_1S))		//nieobecny sygnal ciagu?
			{
				M.RCI=0;
				M._RUSZ_CIAG=1;
				return;
			}
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
			if(M._TURBO&&!RdPrt(S_CIG_NO_1S))		//nieobecny sygnal ciagu?
			{
				M.RCI=0;
				M._RUSZ_CIAG=1;
				return;
			}
			if(!RdPrt(S_PLM)) 					//stwierdzono brak plomienia?		
			{
				DecyzjaPLM2();
				//return;
			}
			if(RTS(_RTROZ)<_TDWPL) return;	
			M._ZAPLON=1;
			M._ROZRUCH=0;
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
		if(_VNT)	//wentylator pracuje?
		{
			//if(RdPrt(S_CIG_NO)&&RdPrt(S_STB))
			if(_CIG_NO&&_STB)
			{
				if(M._OST_PL)		//ostatnia proba?
				{
					M.ERR_BTY=0x01;
					PrintErr(0xE1,1);
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
					PrintErr(0xE1,1);
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
		if(_CIG_NC&&_STB)
		{
			if(M._OST_PL)		//ostatnia proba?
			{
				M.ERR_BTY=0x01;
				PrintErr(0xE1,1);
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
}*/
void GrzejON(void)
{
	_GRZEJ=1;
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	_VNT=1;
}
//------------------
//------------------
//Wylaczenie plytki zapalacza
void GrzejOFF(void)
{
	_GRZEJ=0;
}
//------------------
//------------------

//Procedura rozruchu kotla
void Rozruch(void)
{
	static unsigned char tim;

	switch(M.ROZ)
	{
        case 0:
        {
            M._ZAPLON=0;                        //zeruj znacznik ze doszlo do zaplonu
			StartRTS(_RTROZ);
			M.ROZ=1;            
        }
		//kontrola poprawnosci dzialania ukladu detekcji plomienia
		case 1:
		{
			if(RdPrt(S_PLM)&&!_GRZEJ)			//obecny plomien mimo ze _GRZEJ=0?
			{
				if(RTS(_RTROZ)<_TBPL0) return;
				M.ERR_BTY=0x06;
				PrintErr(0xE6,1);					//wyswietl kod bledu
				ErrPTG();
			}
			GrzejON();//_GRZEJ=1;_VNT=1;							//zapal plomien na palniku,uruchom wentylator
			M.ROZ=2;
		}
		//oczekiwanie az sygnal obecnosci ciagu zniknie przy wylaczonym wentylatorze
		case 2:
		{
			StartRTdS(_RTFKVN);
			M.ROZ=3;
		}
		//oczekiwanie na sygnal obecnosci ciagu przy zalaczonym wentylatorze
		case 3:
		{
			WritePWM(CalkVNTtoPWM(DtKNF.mmax));
			tim=CalkTimStr(ProcToVNT(DtKNF.mstr));
			if(RTdS(_RTFKVN)<tim) return;		//wstepny wybieg wentylatora na mocy maksymalnej
			WritePWM(ProcToMod(DtKNF.mstr));
			StartRTS(_RTROZ);
			M.ROZ=4;
		}

		//zalaczenie pompy, oczekiwanie na sygnal obecnosci plomienia
		case 4:
		{
			WritePWM(ProcToMod(DtKNF.mstr));//WritePWM(CalkVNTtoPWM(ProcToVNT(DtKNF.mstr)));	//moc startowa

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
			WritePWM(ProcToMod(DtKNF.mstr));//WritePWM(CalkVNTtoPWM(ProcToVNT(DtKNF.mstr)));	//moc startowa
            M._ZAPLON=1;                    //znacznik ze doszlo do zaplonu
            M._ROZRUCH=0;
            M.ROZ=0;
            return;
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
	if(_STB&&_CIG_NC)
	{
		if(M._OST_PL)		//ostatnia proba?
		{
			M.ERR_BTY=0x01;
			PrintErr(0xE1,1);
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
//------------------
//------------------
//Wybieg wentylatora
/*void VWybieg(void)
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
			if(M._ZAPLON||M._RUSZ_CIAG||M._RUSZ_PLOMIEN)
			{
				if(RTS(_RTVWB)<_TVWB1) return;
			}
			else
			{
				if(RTS(_RTVWB)<_TVWB2) return;
			}
			_VNT=0;									//wylacz wentylator
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
}*/
void VWybieg(void)
{
	switch(M.VWB)
	{
		case 0:
		{
			M.VWB=1;
			M._DE_VNT=1;
			StartRTS(_RTVWB);
			WritePWM(ProcToMod(0));
		}
		case 1:
		{	
			if(RTS(_RTVWB)>=_TVWB0) M._DE_VNT=0;	//wyzeruj znacznik probnego wybiegu wentylatora
			//if(RTS(_RTVWB)<_TVWB1) return;
			if(M._ZAPLON||M._RUSZ_CIAG||M._RUSZ_PLOMIEN)
			{
				if(RTS(_RTVWB)<_TVWB1) return;      //pelny wybieg wentylatora
			}
			else
			{
				if(RTS(_RTVWB)<_TVWB2) return;      //skrocony wybieg wentylatora
			}            
			WritePWM(0);
			_VNT=0;									//wylacz wentylator
			M._DE_VNT=1;
			M.VWB=2;
		}
		case 2:
		{
			//if(RTS(_RTVWB)<_TVWB1+2) return;		//beznapieciowy wybieg wentylatora
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
			//x=0;
		}
	}
}
//------------------
//------------------
//Awaryjny rozruch plomienia
/*
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
*/
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
				if(M.LICZNIK_PL>=_NCYKLI_PL-1)
				{
				 	M._OST_PL=1;
					M._RUSZ_RPLM=0;
				}
			}
			else
			{
				if(M.LICZNIK_PL>=_NCYKLI_PL)
				{
				 	M._OST_PL=1;
					M._RUSZ_RPLM=0;
				}
			}
			StartRTdS(_RTRSPL);
			M._RUSZ_PLOMIEN=0;
			M.RPL=0;
		}
	}
}
//------------------
//------------------
//Zerowanie procedury awaryjnego rozruchu plomienia
void ReloadPlomien(void)
{
	if(!M._RUSZ_PLOMIEN&&!M._ZMKCOCW&&!M._ROZRUCH&&(RTdS(_RTRSPL)>9))
	{
		M._RUSZ_RPLM=0;
		M._OST_PL=0;
		M.LICZNIK_PL=0;
	}
	/*if(M._RUSZ_PLOMIEN) StartRTS(_RTRPL2);
	if(RTS(_RTRPL2)>_TPLPR2)					//po zadanym czasie wyzeruj licznik_zaników_plomienia/min
	{
		M.LICZNIK_PL2=0;
	}*/
}


//------------------
//------------------
//Detekcja poprawnosci pracy wentylatora
void KontrolaWentylatora(void)
{
	if(!_VNT||!PWM.BufPWM) 									//wentylator stoi
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
				_VNT=0;
				WritePWM(0);
				M.ERR_BTY=0x07;
				PrintErr(0xE7,1);
				ErrPTG();
			}
		}	
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
				if(_VNT) 				//wentylator pracuje?
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
				if(_VNT) 				//wentylator pracuje?
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
				if(!RdPrt(S_CIG_NO))	//zanikl sygnal ciagu
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
				_VNT=0;								//stop wentylatora
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
}*/
//------------------
//------------------
//Zerowanie licznuka ponowien awarii ciagu
/*void ReloadCiag(void)
{
	if(!M._TURBO) //sterownik atmosferyczny
	{
		if(!M._RUSZ_CIAG&&RTM(_RTRCI3)>_TCIPR3) M.LICZNIK_CIG2=0;	//wyzeruj licznik ponowien procedury po 60min.
	}
	else
	{
		if(RTS(_RTRSCI)>_TCIRLP)					//(turbo) po minucie wyzeruj licznik_zaników_ci?gu/min //zliczane kolejne zaniki
		{
			M.LICZNIK_CIG=0;
		}
		if(!M._RUSZ_CIAG&&(RTS(_RTRSCI2)>_TCITRS))	//(turbo) po minucie wyzeruj licznik_prob_ci?gu/min
		{
			M.LICZNIK_CIG3=0;
		}		
	}
}*/

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
		//---------------------------awaria przy wyl?czonym CW
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
//Przerwa czasowa na wypadek stanow nieustalonych przy zaniku napiecia
void VZPauza(void)
{
	StartRTS(_RTROZ);
	while(1)
	{
		CLRWDT();
		if(M.ERR_BTY==2)
		{
			if(RTS(_RTROZ)>=3) return;
		}
		else
		{
			if(RTS(_RTROZ)>=2) return;
		}
	}
}
//------------------
//------------------
//Petla wylaczenia awaryjnego
void ErrPTG(void)
{
	unsigned char k=0;
    
    EndOFF();
	GrzejOFF();//_GRZEJ=0;
	VZPauza();															//przerwa czasowa na wypadek stanow nieustalonych
	if(((M.ERR_BTY>=1)&&(M.ERR_BTY<=3))||((M.ERR_BTY>=6)&&(M.ERR_BTY<=7))) eeprom_write(_ADRERR,M.ERR_BTY);//WriteEEPROM(_ADRERR,M.ERR_BTY); 	//zapis kodu bledu do pamieci
	M._ROZRUCH=0;
	M._MODULUJ=0;
    WritePWM(0);
	if(_VNT) M._VWYBIEG=1;		//aktywacja wybiegu wentylatora
    else M.VVNT=0;              //zeruj wartosc pomiaru predkosci wentylatora    
	M.KRS=0;
//------

	while(1)
	{
		CLRWDT();
//blok pomiarowy
		PomiarPCW();			//wartosc nastawy CW
		PomiarCW();				//wartosc temperatury CW
		PomiarVNT();			//predkosc obrotowa wentylatora

        if(!_VNT&&!M._VWYBIEG) M.VVNT=0;    //zeruj wartosc pomiaru predkosci wentylatora        
//blok wykonawczy
		if(M._VWYBIEG) VWybieg();			//wybieg wentylatora
//obsluga zdarzenia resetujacego
		if(KeyRes()) 
		{
			_VNT=0;
			PrintPR(BLACK,BLACK,1);
			EndErr();
			ToReset();
		}
	}
}
//Wymuszenie resetu sterownika.
void ToReset(void)
{
	_VNT=0;
	eeprom_write(_ADRERR,0x00);
	StartRTS(_RTROZ);
	while(RTS(_RTROZ)<1)
	{
 		CLRWDT();
	}
    while(1);
}
//------------------
//------------------
void InitGlobal(void)
{
	//RCON
	IPEN=1;			//enable priority levels
	//INTCON
	GIEH=1;			//enable oll un-masked interrupts
	GIEL=1;			//enable oll un-masked interrupts
}

