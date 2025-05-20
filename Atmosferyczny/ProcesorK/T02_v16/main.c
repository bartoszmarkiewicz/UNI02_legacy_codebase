/*main.c*/  
//UWAGA:
    //Odbior produkcyjny (aktywny przy aktywnym interfejsie LIN):
    //M.STRB=1,M.STRB=2
    //Deaktrywacja przedbiegu pompy
    //Deaktywacja blokady dla zaworu 3DR na obieg CO w przypadku kotlow 1F przy TCW>85C



/*Oprogramowanie rodziny sterownikow T02 (na bazie p�ytki UNI-02)*/
/*Wersja z dodana obsluga obiegu otwartego (r7) dla wszystkich typow kotla (28.01.2011)*/
/*oprogramowanie dla platformy 3V i 5V*/
/*optymalizacja procedury zanikow ciagu dla kotlow atm. 20.08.2012*/
/*usuniety blad zapisu nastawy pogodowej 12.02.2013*/
/*	09.04.2015
	temp CO dla kotlow monoterm. zmienione na 89C
	wprowadzone podbicie mocy minimalnej po procedurze startowej
	czas trwania proc serwisowej zmieniony na 60 min
    i2c pokazuje rzeczywista docelowa temp PPCO i PPCW
    powiekszona histereza przy grzaniu CO (+10)
    poprawiona proc GoldenEye dla wiekszej histerezy wylaczenia przy grzaniu CO
*/

/* 01.06.2015
	eeprom.h - wprowadzone nowe 4 parametry pracy pompy
	ac.h - nowy adres tabeli ntc czujnika pogodowego
	konfig.tp.h - nowe 4 parametry pracy pompy
	konfig.h - granice dla nowych 4 parametrow pracy pompy, liczba parametrow zwiekszona o 4
	main.tp.h - 4 parametry do wysylki po RS
	konfig.c - dodanie 4 parametrow do procedury konfiguracji
	pwm.h - dodanie parametrow konfiguracyjnych pwm2
	pwm.tp.h - dodanie zmiennych dla pwm2
cd...
	17.06.2015
	-obsluga pompy PWM definiowana 4 parametrami, 
	-osluga czujnika temp na powrocie, kod bledu dla czujnika na powrocie E14
	-mechanizmy kompatybilnosci wstecznej
	-diagnostyka wyjscia PWM podczas rozrychu sterownika
cd...
	20.07.2015
	-optymalizacja algorytmu zaworu 3dr celem minimalizacji upluwu cieplej wody do instalacji CO w trybie LATO
	-optymalizacja algorytmu procedury serwisowej - zapobieganie grzania w obiegu WU
	-optymalizacja algorytmu funkcji antylegionella - stala domyslna nastawa PPCO
*/

#include <pic18.h>
#include <htc.h>
#include <stdio.h>
#include <string.h>
#include "global.h"			//parametry globalne
#include "main.h"			//parametry lokalne
#include "wersja.h"
#include "wsw.h"
#include "print.h"
#include "eeprom.h"
#include "pwm.h"
#include "czcw.tp.h"
#include "pomiar.h"

#include "keypad.h"
#include "konfig.h"

#include "pompa.tp.h"
#include "rgl.h"
#include "smgs_rs9b.tp.h"

tpRSDTA1 RSDT1;
tpMAIN M;

extern DtPMP PMP; //pompa.c
extern DtPWM PWM;			//pwm.c
extern DataPrCW PrCW;		//czcw.c
extern DataCO DtCO;			//pomiar.c
extern DataCW DtCW;
extern DataZW DtZW;
extern DataPFN PFN;
extern DataCSN DtCS;
extern DataIN DtIN;
extern DtKONF DtKNF;


extern DtRgl rgl;
unsigned char x;
extern StPin sp;
extern unsigned char xxy,zzz,vvv;
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
	InitPort();
	InitAC();
	InitPWM();
	InitI2C();
	InitReceptProc();
	InitUart();
	InitDataPmr();
	ClearData();
    BitSet2(&RSDT1.outSTAT0,_PGPRG,0);                              //ustaw znacznik inicjalizacji programu
    BitSet2(&RSDT1.outSTAT0,_PGRES,0);
    RefBHist(1);
    RefBStat(1);
//------------------
//------------------
	if (!POR||!BOR)							//zalaczono zasilanie?
	{
		M._PW_START=1;						//znacznik rozruchu sterownika po wlaczeniu zasilania
		PwrOnDelay();						//zwloka na wypadek stanow nieustalonych	
	}
	else
	{
		M._RS_START=1;						//znacznik rozruchu sterownika po resecie
		PwrOnDelay();						//zwloka na wypadek stanow nieustalonych	
	}
	//Odczytaj parametry zapisane w pamieci EEPROM
	RdEEParam();

	//Stan po zalaczeniu zasilania - warunek wejscia do trybu konfiguracji
	if (!POR||!BOR)		//zalaczono zasilanie? 
	{	
		POR=1;
		BOR=1;
#if _PWRKNF==1	
		EnKonf();				//aktywacja oczekiwania na wejscie do konfiguracji
#endif
	}
    
	//ustaw znacznik aktualnego obiegu
	_POMPA=0;
	switch(DtKNF.tpkt)		//dla danego rodzaju kotla
	{
		case _BITERM:		//bitermiczny (miniterm)
		case _BITERM2:		//bitermiczny (maxiterm)
		{
			M._RPM_CW=!_POMPA;
			break;
		}
		case _UNICO:
		case _MONOHB:
		{
			M._RPM_CW=!_KCO_CW;
			break;
		}
		default:			//domyslny monotermiczny
		{
			M._RPM_CW=!_KCO_CW;
			break;
		}
	}	

	//Sprawd� czy nastapilo wylaczenie awaryjne z blokada
	if(M.ERR_BTY=eeprom_read(_ADRERR)) 		//uruchomienie po wylaczeniu awaryjnym?
	{
		if(DtKNF.robg==_OTWARTY) 			//obieg otwarty?
		{
			if(M.ERR_BTY==0x05)					//dozwolony kod bledu?
			{
				PrintErr(M.ERR_BTY,1);			//wyswietl kod bledu
				ErrPTG();						//przejdz do petli wylaczenia awaryjnego
			}
		}
		if((M.ERR_BTY>=0x01)&&(M.ERR_BTY<=0x03))	//dozwolony kod bledu?
		{
			PrintErr(M.ERR_BTY,1);			//wyswietl kod bledu
			ErrPTG();						//przejdz do petli wylaczenia awaryjnego
		}
		else M.ERR_BTY=0;
	}
	
	//Pomiar wartosci nastaw dla unikniecia migajacej nastawy po zal. zasilania
	PomiarCO();				//wartosc temperatury CO
	PomiarCW();				//wartosc temperatury CW
	PomiarZW();				//temperatura z czujnika zewnetrznego
	PomiarCS();				//wartosc cisnienia w instalacji CO
	PomiarPCO();			//wartosc nastawy CO
	PomiarPCW();			//wartosc nastawy CW
	PomiarPFN();			//stan przelacznika funkcji
	PomiarIN();				//wartosc temperatury na powrocie
	DtCO._newPCO=0;			//deaktywacja wyswietlania nastawy CO
	DtCW._newPCW=0;			//deaktywacja wyswietlania nastawy CW

	//deaktywacja pobudzen
	_VNT=0;									//wentylator na stale wylaczony
	_GRZEJ=0;
	WritePWM(0);			//predkosc obrotowa wentylatora = 0

	//decyzja czy kociol jest typu TURBO
	M._TURBO=!RdPrt(S_CIG_NC)||RdPrt(S_CIG_NO);
	RestartFltPrt();
	StartRTS(_RTSTR);						//start timera rozruchu sterownika						
//------------------
//------------------
//------------------
	DtCW.PCW=65;
	DtCO.PCO=85;
    WritePWM2(0);
	while(1)					//petla glowna programu				
	{
		CLRWDT();
		BitSet2(&RSDT1.outSTAT0,_PGPRG,1);		//znacznik petli programu
		BitSet2(&RSDT1.outSTAT0,_PGRES,0);
		M._PRACA=M._ROZRUCH||_GRZEJ;
	//----------------
		ReceptDataLCD();
		MKonfiguracja();		//sterowanie znacznikiem mozliwosci wejscia do proc. konfiguracji
	//----------------
		PomiarPCO();			//wartosc nastawy CO
		PomiarPCW();			//wartosc nastawy CW
		PomiarPFN();			//stan przelacznika funkcji
		PomiarCO();				//wartosc temperatury CO
		PomiarCW();				//wartosc temperatury CW
		PomiarZW();				//temperatura z czujnika zewnetrznego
		FiltrPomiarowyCOCW();	//filtr danych opisujacych stan temperatur
		PomiarCS();				//wartosc cisnienia w instalacji CO
        PomiarIN(); 			//wartosc temperatury na powrocie
	//----------------
		DecERR();				//stany awaryjne, wyswietlane kody awaryjne
		DecSTR();				//dodatkowe decyzje sterowania
		DecRozruch();			//decyzja o zapaleniu palnika
		DecWylacz();			//decyzja o wygaszeniu palnika
		FunkcjaSerwisowa();		//decyzja o realizacji funkcji serwisowej
		DecyzjaAntyLeg();		//decyzja o realizacji funkcji ANTYLEGIONELLA
		KolejkaPobudzen();		//aktywacja pobudzen wg priorytetu	
		RefLCDData();			//aktualizacja danych wysylanych do modulu LCD
        RefBHist(0);            //aktualizacja bufora zdarzen
	//----------------
		RglPmp(); 				//dla pompy PWM reguluj predkosc obrotowa
		if(M._ZMKCOCW) ZmienKCOKCW();		//zmiana kierunku obrotu pompy
        if(M._LOW_PMP) SetLowPMP();         //oobnizona predkosc wybiegu pompy na czas przelaczenia zaworu 3DR        
		if(M._WYLACZ) Wylacz();				//kontrola wybiegu pompy
		if(M._ROZRUCH) Rozruch();			//rozruch kotla
		if(M._VWYBIEG) VWybieg();			//wybieg wentylatora
		ReloadPlomien();					//decyzja o wyzerowaniu licznika prob przy braku plomienia
		if(M._RUSZ_PLOMIEN) RuszPlomien();	//awaryjna procedura przy braku plomienia
		ReloadCiag();						//zerowanie licznika ponowien prob ciagu
		if(M._RUSZ_CIAG) RuszCiag();		//awaryjna procedura przy braku ciagu
		Proba48();							//zegar 48h
		if(M._RUSZ_48) Rusz48();			//wybieg pompy co 48h
		Proba48Z3();						//zegar 48h dla Z3Dr
		if(M._RUSZ_48Z3) Rusz48Z3();		//wybieg pompy co 48h w obiegu WU
		if(M._BLOKUJ_3M) BlokadaL3();		//blokada 3 minuty
		if(M._BLOKUJ_15S)
		{
			if(!M._PRACA) Blokada15S();		//blokada 15 sekund
		}
		if(M._MODULUJ) Moduluj();			//algorytm modulacji
		if(DtKNF.robg==_OTWARTY) 			//klad z obiegiem otwartym?
		{
			DetekcjaPrzeplywuWG();				//kontrola nieprawidlowosci przeplywu wody grzewczej
			if(M._RUSZ_POMPE) OczekujNaPrzeplywWG(); //oczekiwanie na powrot czujnika do prawidlowej pozycji
		}
		else
		{
			M._RUSZ_POMPE=0;
			M._prb_PRZ=0;
		}
	}
}
//------------------
//------------------
//------------------
//Wyzerowanie podstawowych danych sterujacych
void ClearData(void)
{
	DtKNF.enknf=0;
	M._pob_KCO=0;
	M._pob_KCW=0;
	M._pob_AL=0;	
	M._pob_SRW=0;
	M._pob_CO=0;
	M._pob_CW=0;
	M._pob_DzCO=0;
	M._pob_DzCW=0;
    M._pob_PGDRT=0;    

	M._WYLACZ=0;
	M._ZMKCOCW=0;
	M._ROZRUCH=0;
	M._RUSZ_48=0;
	M._RUSZ_48Z3=0;
	M._RUSZ_PWR=0;
	M._VWYBIEG=0;
	M._MODULUJ=0;
	M._RUSZ_CIAG=0;
	M._RUSZ_PLOMIEN=0;
	M._RUSZ_RPLM=0;
	M._RUSZ_POMPE=0;
	M._BLOKUJ_3M=0;
	M._BLOKUJ_15S=0;
	M._MODULUJ=0;

	M._AKTYWNE_AL=0;
	M._AKTYWNE_SRW=0;
	M._AKTYWNE_PGD=0;
	M._AKTYWNE_PGD2=0;    
	M._AKTYWNE_KNF=0;
	M._RUSZ_NTC_CO=0;
	M._RUSZ_NTC_CW=0;
	M._RUSZ_STB=0;
	M._RUSZ_MOD=0;
	M._RUSZ_CISN=0;
	M._RUSZ_NCISN=0;

	M._PW_START=0;
	M._RS_START=0;
	M._TURBO=0;				
	M._PRACA=0;
	M._OST_PL=0;
	M._RUN_MNMOD=0;
	M._KCW_PMP=0;
	M._DE_VNT=0;
	M._DE_PMP=0;
	M._RPM_CW=0;
	M._RUSZ_NTC_IN=0;

    M._SYNCH_PGD=0;    
    
	M._LCD=0;
	M._LIN=0;
	M._LIN_RT=0;
	M._LIN_CW=0;    
    M._LOW_PMP=0;
    
	M.ERR_BTY=0;
	M.ROZ=0;
	M.R48=0;
	M.R48Z3=0;
	M.BL3=0;
	M.BL15=0;
	M.WYL=0;
	M.RPR=0;
	M.RPL=0;
	M.RCI=0;
	M.MNM=0;
	M.ZKP=0;
	M.VWB=0;
	M.SRV=0;
	M.KRS=0;
	M.HVZ=0;
	M.LICZNIK_PL=0;
	M.LICZNIK_CIG=0;
	M.LICZNIK_CIG2=0;
	M.LICZNIK_CIG3=0;
	M.DPR=0;
	M.STRB=0;
    
	_VNT=0;
	_GRZEJ=0;
	_POMPA=0;
}
//------------------
void RestartFltPrt(void)
{
	SetPrt(S_ON_PMP,0);
	SetPrt(S_ON_PMP_CW,0);
	SetPrt(S_DZ_CO,0);
	SetPrt(S_DZ_CW,0);
	SetPrt(S_RT,0);
	SetPrt(S_WCW,0);
	SetPrt(S_HVZ,0);
}
//------------------
//------------------
unsigned char KodSterownika(void)
{
	//Kod sterownika
	switch(DtKNF.tpkt)		//dla danego rodzaju kotla
	{
		case _UNICO:		//unico - monotermiczny
			if(!M._TURBO) return 0x10;
			else return 0x11;
		case _BITERM:		//bitermiczny (miniterm)
			if(!M._TURBO) return 0x12;
			else return 0x13;	
		case _BITERM2:		//bitermiczny (maksiterm)
			if(!M._TURBO) return 0x14;
			else return 0x15;	
		case _MONOHB:		//monotermiczny z hydroblokiem
			if(!M._TURBO) return 0x16;
			else return 0x17;
		default:			//domyslny monotermiczny z hydroblokiem
			if(!M._TURBO) return 0x18;
			else return 0x19;
	}
}
//------------------
//------------------
//Aktualizacja danych do wysylki
void RefLCDData(void)
{
    volatile unsigned char wr;    
    if(!RSDT1.vSTD)
    {
        RSDT1.outRDZ0=KodSterownika();
        RSDT1.outRDZ1=DtKNF.tpkt;
        RSDT1.outRDZ2=0;
    }
    else
    {
        RSDT1.outRDZ0=(KodSterownika()&0b00011111)|((DtKNF.alin&0b00000111)<<5);            //kod sterownika i adres LIN plutki UNI-02 RSDT1.outRDZ0=KodSterownika()
        if(_WERSJA<=0x0F)
        {
            RSDT1.outRDZ1=(DtKNF.tpkt&0x0F)|((_WERSJA&0x0F)<<4);                            //typ kotla i wersja oprogramowania plytki UNI-02
            wr=RSDT1.outRDZ2&0b11100000;
            RSDT1.outRDZ2=wr;
        }
        else
        {
            RSDT1.outRDZ1=(DtKNF.tpkt&0x0F)|0xF0;                                           //typ kotla i wersja oprogramowania plytki UNI-02   
            wr=RSDT1.outRDZ2&0b11100000;
            RSDT1.outRDZ2=wr|((_WERSJA-0x0F)&0b00011111);
        }    
    }
	BitSet2(&RSDT1.outSTAT0,_PLMCO,M._PRACA&&!M._RPM_CW&&RdPrt(S_PLM));		//plomien CO
	BitSet2(&RSDT1.outSTAT0,_PLMCW,M._PRACA&&M._RPM_CW&&RdPrt(S_PLM));		//plomien CW
	BitSet2(&RSDT1.outSTAT0,_FKSRV,M._AKTYWNE_SRW);							//sygnalizuj realizacje funkcji serwisowej
	BitSet2(&RSDT1.outSTAT0,_ANTYL,M._AKTYWNE_AL);							//sygnalizuj realizacje funkcji antylegionella
	BitSet2(&RSDT1.outSTAT0,_KNFPRC,M._AKTYWNE_KNF);						//sygnalizuj realizacje funkcji konfiguracji
	BitSet2(&RSDT1.outSTAT0,_ENKNF,DtKNF.enknf);							//zezwolenie na wejscie do konfiguracji

	BitSet2(&RSDT1.outSTAT1,_TdzCW,M._pob_DzCW);							//temp. dyzurna CO
	BitSet2(&RSDT1.outSTAT1,_TdzCO,M._pob_DzCO);							//temp. dyzurna CW					
	BitSet2(&RSDT1.outSTAT1,_PGD,M._AKTYWNE_PGD);							//aktywna funkcja pogodowa
	BitSet2(&RSDT1.outSTAT1,_OBGCO,M._PRACA&&!M._RPM_CW);					//aktywny obieg CO
	BitSet2(&RSDT1.outSTAT1,_OBGCW,M._PRACA&&M._RPM_CW);					//aktywny obieg CW


	BitSet2(&RSDT1.outSTAT2,_ErrCO,DtCO._err_CO);							//awaria czujnika CO
	BitSet2(&RSDT1.outSTAT2,_ErrCW,DtCW._err_CW);							//awaria czujnika CW
	BitSet2(&RSDT1.outSTAT2,_ErrCS,DtCS._error);							//awaria czujnika CS
	BitSet2(&RSDT1.outSTAT2,_CSnmt,DtCS._no_mont);							//brak czujnika CS
    BitSet2(&RSDT1.outSTAT2,_TTRB,M._TURBO);                               //znacznik pracy w trybie TURBO

	BitSet2(&RSDT1.outSTAT3,_ErrorIN,DtIN._err_IN);						//aktywnosc czujnika IN
	BitSet2(&RSDT1.outSTAT3,_PMP_PWM,(DtKNF.rdpm!=0)?1:0);					//rodzaj pompy
	BitSet2(&RSDT1.outSTAT3,_ZCFM,(_KNF)?0:1);								//stan zworki konfiguracyjnej
	BitSet2(&RSDT1.outSTAT3,_VNT_ON,_VNT);									//stan pracy wentylatora
	BitSet2(&RSDT1.outSTAT3,_ROZ,M._ROZRUCH);								//znacznik aktywnej procedury rozruchu
    
    if(DtKNF.tpkt==_UNICO)				//kociol UniCO?
    {
        if(!DtKNF.cwtm)     //(timera zasobnika||LIN_CW||OT_CW)
        {
            BitSet2(&RSDT1.outSTAT3,_TIM_ZAS,(RdPrt(S_WCW)!=0)?1:0);				//(timera zasobnika||LIN_CW||OT_CW)
        }
        else                //timer zasobnika
        {
            BitSet2(&RSDT1.outSTAT3,_TIM_ZAS,(RdPrt(S_TIM)!=0)?1:0);				//timera zasobnika        
        }
    }
    else
    {
        BitSet2(&RSDT1.outSTAT3,_TIM_ZAS,(RdPrt(S_WCW)!=0)?1:0);				//pobudzenie CW   
    }
    
	BitSet2(&RSDT1.outSTAT3,_POB_RT,(RdPrt(S_RT)!=0)?1:0);					//pobudzenie RT

    if(PFN.pop_poz==_PFLTO)
    {
        BitSet2(&RSDT1.outSTAT4,_POP_FN,1);                                 //ostatnia pozycja PFN przed przejsciem na pozycje OFF (0-zima,1-lato)   
    }
    else
    {
        BitSet2(&RSDT1.outSTAT4,_POP_FN,0);                                 //ostatnia pozycja PFN przed przejsciem na pozycje OFF (0-zima,1-lato)          
    }
    BitSet2(&RSDT1.outSTAT4,_TMP_ZW,(DtZW._en_ZW!=0)?1:0);                  //znacznik obecnosci czujnika temp zew.
    BitSet2(&RSDT1.outSTAT4,_PMP_ON,_POMPA);                                //stan pracy pompy (0-postoj, 1-paraca)
    BitSet2(&RSDT1.outSTAT4,_KCOCW,_KCO_CW);                                //zawor 3d (0-obieg CW, 1-obieg CO)   
    BitSet2(&RSDT1.outSTAT4,_PRZ_CO2,(RdPrt(S_PRZ_CO)!=0)?1:0);             //zawor 3d (0-obieg CW, 1-obieg CO)    
    BitSet2(&RSDT1.outSTAT4,_GRZ_ON,_GRZEJ);                                //pobudzenie modu?u kontroli plomienia (0-postoj, 1-paraca)    

    BitSet2(&RSDT1.outSTAT5,_STK_RT2,(_RT)?1:0);                            //styk wejscia pobudzenia RT/obni?enie nocne (0-rozwarty, 1-zwarty)
    BitSet2(&RSDT1.outSTAT5,_STK_CW2,(_PRZ_CW)?0:1);                        //styk wejscia pobudzenia CW/timer zasobnika (0-rozwarty, 1-zwarty)
    BitSet2(&RSDT1.outSTAT5,_CIG_NO2,(RdPrt(S_CIG_NO)!=0)?1:0);             //styk czujnika ciagu NO (0-rozwarty, 1-zwarty)  
    BitSet2(&RSDT1.outSTAT5,_CIG_NC2,(RdPrt(S_CIG_NC)!=0)?1:0);             //styk czujnika ciagu NC (0-rozwarty, 1-zwarty)           
    BitSet2(&RSDT1.outSTAT5,_STK_STB,(RdPrt(S_STB)!=0)?1:0);                //styk czujnika STB NC (0-rozwarty, 1-zwarty)
    BitSet2(&RSDT1.outSTAT5,_MOD_CON,(RdPrt(S_MOD_ON)!=0)?1:0);             //ciaglosc obwodu modulatora (0-rozwarty, 1-zwarty)
    
	BitSet2(&RSDT1.outKSTAT0,_ZCFM0,(_KNF)?0:1);							//stan zworki konfiguracyjnej
	BitSet2(&RSDT1.outKSTAT0,_KC1F,(DtKNF.tpkt==_UNCO)?1:0);				//znacznik rodzaju kotla (1=jednofunkcyjny)
	RSDT1.outPP=PMP.APROC;													//procentowy wydatek pompy
    RSDT1.outPG=ModToProc(PWM.BufPWM);                                  	//procentowy wydatek modulatora
    M.vWSW=RSDT1.inRDZ0;                                                    //wersja plytki wyswietlacza  
    if(GetBit2(RSDT1.inSTAT1,_7KEY))
    {
        M._KEY7=1;                                                          //znacznik konsoli 7key
        if(GetBit2(RSDT1.inSTAT1,_KSET))                                    //nacisnieto przycisk funkcji specjalnych?
        {
            M._KKOM=1;
        }
        else
        {
            M._KKOM=0;
        }
    }
    else 
    {
        M._KEY7=0;
        M._KKOM=0;
    }
//Kolejne interfejsy
//Priorytet:
//LIN(KSK lub STR)---                       
//                   OT---
//                        10V---
#if (_EXT_BOARD==0)     
    M._I10V=0;
    M._I10V_RT=0;
	M._LIN=0;
	M._LIN_RT=0;
	M._LIN_CW=0;
	M._LIN_KSK=0;
    M._LIN_STR=0;
    M._LIN_DPGD=0;   
    M._LIN_ENDO=0;    
    M._IOT=0;
    M._IOT_RT=0;
    M._IOT_CW=0;
    M._IOT_STR=0;    
    M._IOT_DPGD=0;    
    M.STRB=0;    
#endif            
#if (_EXT_BOARD==1)    
    M._I10V=GetBit2(RSDT1.inINSTAT,_F10V)&&M._LCD;   						//ustaw znacznik aktywnego interfejsi 10V
    M._I10V_RT=GetBit2(RSDT1.inINSTAT,_10V_RT)&&M._I10V;
    //brak interfejsu LIN i OT
	M._LIN=0;
	M._LIN_RT=0;
	M._LIN_CW=0;
	M._LIN_KSK=0;
    M._LIN_STR=0;
    M._LIN_DPGD=0;
    M._LIN_ENDO=0;    
    M._IOT=0;
    M._IOT_RT=0;
    M._IOT_CW=0;
    M._IOT_STR=0;    
    M._IOT_DPGD=0;    
    M.STRB=0;    
#endif
#if _EXT_BOARD==2)
    M._LIN=GetBit2(RSDT1.inLNSTAT,_FLIN)&&M._LCD;   						//ustaw znacznik aktywnej komunikacji LIN
    M._LIN_RT=GetBit2(RSDT1.inLNSTAT,_LN_RT)&&M._LIN;   					//ustaw pobudzenie RT z interfejsu LIN
    M._LIN_CW=GetBit2(RSDT1.inLNSTAT,_LN_CW)&&(DtKNF.tpkt==_UNICO)&&M._LIN; //ustaw pobudzenie CW z interfejsu LIN
    M._LIN_KSK=GetBit2(RSDT1.inLNSTAT,_LN_KSK)&&M._LIN;                     //znacznik pracy w trybie kaskady
    M._LIN_STR=GetBit2(RSDT1.inLNSTAT,_LN_STR)&&M._LIN;                     //znacznik pracy w trybie sterowania z managera stref lub regulatora RT
    M._LIN_DPGD=GetBit2(RSDT1.inLNSTAT,_LN_DPGD)&&M._LIN;                   //znacznik dezaktywacji wewnetrznego regulatora pogodowego 
    //M._LIN_ENDO=GetBit2(RSDT1.inSTAT1,_END_ODP)&&M._LIN&&M._BLOKUJ_ODP;     //zdalna deaktywacja procedury odpowietrzajacej
    if(!M._LIN_KSK&&!M._LIN_STR)
    {
        M._I10V=GetBit2(RSDT1.inINSTAT,_F10V)&&M._LCD;   						//ustaw znacznik aktywnego interfejsi 10V
        M._I10V_RT=GetBit2(RSDT1.inINSTAT,_10V_RT)&&M._I10V;     
    }
    else
    {
        M._I10V=0;   						
        M._I10V_RT=0;          
    }
    if(M._LIN)
    {
        M.STRB=RSDT1.inSTAT2;
    }
    else
    {
        M.STRB=0;
    }    
    //brak interfejsu OT
    M._IOT=0;
    M._IOT_RT=0;
    M._IOT_CW=0;    
    M._IOT_STR=0;    
    M._IOT_DPGD=0;    
#endif
#if (_EXT_BOARD>=3) 
    M._LIN=GetBit2(RSDT1.inLNSTAT,_FLIN)&&M._LCD;   						//ustaw znacznik aktywnej komunikacji LIN
    M._LIN_RT=GetBit2(RSDT1.inLNSTAT,_LN_RT)&&M._LIN;   					//ustaw pobudzenie RT z interfejsu LIN
    M._LIN_CW=GetBit2(RSDT1.inLNSTAT,_LN_CW)&&(DtKNF.tpkt==_UNICO)&&M._LIN; //ustaw pobudzenie CW z interfejsu LIN
    M._LIN_KSK=GetBit2(RSDT1.inLNSTAT,_LN_KSK)&&M._LIN;                     //znacznik pracy w trybie kaskady
    M._LIN_STR=GetBit2(RSDT1.inLNSTAT,_LN_STR)&&M._LIN;                     //znacznik pracy w trybie sterowania z managera stref lub regulatora RT 
    M._LIN_DPGD=GetBit2(RSDT1.inLNSTAT,_LN_DPGD)&&M._LIN;                   //znacznik dezaktywacji wewnetrznego regulatora pogodowego 
    //M._LIN_ENDO=GetBit2(RSDT1.inSTAT1,_END_ODP)&&M._LIN&&M._BLOKUJ_ODP;     //zdalna deaktywacja procedury odpowietrzajacej    
    if(!M._LIN_KSK&&!M._LIN_STR)    
    {
        M._IOT=GetBit2(RSDT1.inINSTAT,_FOT)&&M._LCD;                            //ustaw znacznik aktywnego interfejsi OT
        M._IOT_RT=GetBit2(RSDT1.inINSTAT,_OT_RT)&&M._IOT;                       //ustaw pobudzenie RT z interfejsu OT
        M._IOT_CW=GetBit2(RSDT1.inINSTAT,_OT_CW)&&(DtKNF.tpkt==_UNICO)&&M._IOT; //ustaw pobudzenie CW z interfejsu OT  
        M._IOT_STR=GetBit2(RSDT1.inINSTAT,_OT_STR)&&M._IOT;                     //znacznik pracy w trybie sterowania z managera stref lub regulatora RT  
        M._IOT_DPGD=GetBit2(RSDT1.inINSTAT,_OT_DPGD)&&M._IOT;                     //znacznik pracy w trybie sterowania z managera stref lub regulatora RT
        if(!M._IOT)
        {
            M._I10V=GetBit2(RSDT1.inINSTAT,_F10V)&&M._LCD;   						//ustaw znacznik aktywnego interfejsi 10V
            M._I10V_RT=GetBit2(RSDT1.inINSTAT,_10V_RT)&&M._I10V;                 
        }
        else
        {
            M._I10V=0;   						
            M._I10V_RT=0;            
        }
    }
    else
    {
        M._IOT=0;
        M._IOT_RT=0;
        M._IOT_CW=0;
        M._IOT_STR=0; 
        M._IOT_DPGD=0;
        M._I10V=0;   						
        M._I10V_RT=0;                
    }
    if(M._LIN)
    {
        M.STRB=RSDT1.inSTAT2;
    }
    else
    {
        M.STRB=0;
    }    
#endif     
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
		M._pob_SRW=0;									//zerowanie pobudzen o nizszym priorytecie	
		M._pob_AL=0;	
		M._pob_CO=0;		
		M._pob_CW=0;
		M._pob_DzCO=0;
		M._pob_DzCW=0;
		if((PFN.poz==_PFLTO)||(PFN.poz==_PFZMA))
		{
			if(DtKNF.wymCW) M._pob_KCW=1;				//aktywacja pobudzenia
			else M._pob_KCW=0;
			if(DtKNF.wymCO) 
			{
				M._pob_KCO=1;				//aktywacja pobudzenia
				M._BLOKUJ_15S=0;
			}
			else M._pob_KCO=0;
		}
		else
		{
			M._pob_KCW=0;
			M._pob_KCO=0;
		}
	}
	else
	{
		//grzanie w proc. serwisowej
		M._pob_KCW=0;
		M._pob_KCO=0;
		if(M._AKTYWNE_SRW&&(PFN.poz==_PFZMA)) 
		{
			M._pob_SRW=1;		//aktywacja pobudzenia
			M._pob_AL=0;
			M._pob_CW=0;		//zerowanie pobudzen o nizszym priorytecie
			M._pob_CO=0;		
			M._pob_DzCO=0;
			M._pob_DzCW=0;
			M._BLOKUJ_15S=0;
		}
		else 
		{
			//grzanie w funkcji ANTYLEGIONELLA
			M._pob_SRW=0;
			if(M._AKTYWNE_AL) 
			{
				if(DtCW.CW<_AL_PCW-1) M._pob_AL=1;		//******************* zalacz wymuszenie grzania
				else 
				{
					if(DtCW.CW>=_AL_PCW)
					{
						M._pob_AL=0;					//wylacz wymuszenie grzania
						M._AKTYWNE_AL=0;				//deaktywuj funkcje ANTYLEGIONELLA
					}
				}
				M._pob_CW=0;		//zerowanie pobudzen o nizszym priorytecie
				M._pob_CO=0;		
				M._pob_DzCO=0;
				M._pob_DzCW=0;
			}
			else
			{
				//grzanie od pobudzenia CW
				M._pob_AL=0;
				if(DtKNF.tpkt!=_UNICO) wr=RdPrt(S_WCW)&&((PFN.poz==_PFLTO)||(PFN.poz==_PFZMA));
				else wr=DtCW._wpz_CW&&DtCO._pz_CO&&((PFN.poz==_PFLTO)||(PFN.poz==_PFZMA));		//DtCW._wpz_CW uzaleznione jest od stanu RdPrt(S_WCW) albo RdPrt(S_TIM)
				if(wr!=0)
				{
					M._pob_CW=1;	//aktywacja pobudzenia		
					M._pob_CO=0;	//zerowanie pobudzen o nizszym priorytecie		
					M._pob_DzCO=0;
					M._pob_DzCW=0;
				}
				else
				{
					//grzanie od pobudzenia RT
					M._pob_CW=0;
					if(RdPrt(S_RT)&&(PFN.poz==_PFZMA))
					{
						M._pob_CO=1;	//aktywacja pobudzenia	
						M._pob_DzCO=0;	//zerowanie pobudzen o nizszym priorytecie		
						M._pob_DzCW=0;
					}
					else
					{
						//grzanie od pobudzenia temp. dyz. CO
						M._pob_CO=0;	
						if(RdPrt(S_DZ_CO))
						{
							M._pob_DzCO=1;	//aktywacja pobudzenia	
							M._pob_DzCW=0;	//zerowanie pobudzen o nizszym priorytecie	
						}
						else	//grzanie od pobudzenia temp. dyz. CW
						{
							M._pob_DzCO=0;
							if(DtKNF.tpkt==_UNICO) 				//opcja tylko dla kotla z zasobnikiem
							{
								//grzanie od pobudzenia temp. dyz. CW
								if(RdPrt(S_DZ_CW)) M._pob_DzCW=1;	//aktywacja pobudzenia
								else M._pob_DzCW=0;	
							}
							else M._pob_DzCW=0;
						}
					}
				}
			}
		}	
	}
}
//------------------
//------------------
//Decyzja o realizacji funkcji ANTYLEGIONELLA
void DecyzjaAntyLeg(void)
{
	//static unsigned char kanl;
#if _SRVANL==1										//aktywny tryb kontrolny antylegionella?
	if(DtKNF.tpkt==_UNICO)	//obowiazuje tylko dla kotlow z obsluga zasobnika
	{
		//--------
		//Warunki przerwania realizacji funkcji ANTYLEGIONELLA
		if(M._AKTYWNE_AL)
		{
			if(DtCW._err_CW||DtCO._err_CO)				//*************************
			{
				M._AKTYWNE_AL=0;
				return;	
			}
			if(DtKNF.aleg)
			{
				//StartRTG(_RTANL);						//zatrzymanie zegara ponawiania funkcji ANTYLEGIONELLA
				StartRTM(_SRVRTANL);
				M.LICZNIK_ANL=0;
			}
			if(M._AKTYWNE_KNF||M._AKTYWNE_SRW)
			{
				M._AKTYWNE_AL=0;
				return;		
			}
			//niedozwolona pozycja przelacznika funkcyjnego
			if(!((PFN.poz==_PFLTO)||(PFN.poz==_PFZMA))) 
			{
				M._AKTYWNE_AL=0;
				return;
			}
			//maksymalny dozwolony czas aktywnosci funkcji ANTYLEGIONELLA
			if(RTM(_RTMAL)>=_SRVTEXTAL)
			{
				M._AKTYWNE_AL=0;
				return;
			}
			//deaktywowano procedure (np nacisnieto przycisk RESET)
			//if(!GetBit2(RSDT1.inSTAT0,_INITANL))
			if(PFN._res)
			{
				M._AKTYWNE_AL=0;
				return;
			}
		}
		//--------
		//Warunki aktywacji funkcji ANTYLEGIONELLA
		if(!M._AKTYWNE_AL)					//mozliwa realizacja funkcji ANTYLEGIONELLA?
		{
			if(M._AKTYWNE_KNF||M._AKTYWNE_SRW)			//realizowana funkcja serwisowa lub konfiguracyjna?
			{
				return;
			}
			//-------
			//zegar ponawiania funkcji ANTYLEGIONELLA
			if(DtKNF.aleg)
			{
				//if(RTG(_RTANL)>=1)							//timer godzinowy wyzwalania funkcji ANTYLEGIONELLA
				if(RTM(_SRVRTANL)>=1)						//timer godzinowy wyzwalania funkcji ANTYLEGIONELLA
				{
					//StartRTG(_RTANL);
					StartRTM(_SRVRTANL);
					M.LICZNIK_ANL++;
					if(M.LICZNIK_ANL>=_NGODZIN_SRVAL) 		//licznik godzin do wyzwolenia funkcji ANTYLEGIONELLA
					{
						M.LICZNIK_ANL=0;
						if(!(DtCW._err_CW||DtCO._err_CO))	//*************************
						{
							if((PFN.poz==_PFLTO)||(PFN.poz==_PFZMA)) M._AKTYWNE_AL=1;		//aktywacja funkcji ANTYLEGIONELLA po czasie odliczonym przez timer.
						}
						StartRTM(_RTMAL);					//start timera maksymalnego dozwolonego czasu aktywnosci funkcji ANTYLEGIONELLA
						eeprom_write(_ADRRTA,0);
						return;	
					}
					else
					{
						eeprom_write(_ADRRTA,M.LICZNIK_ANL);
					}
				}
			}
			//-------
			if(!((PFN.poz==_PFLTO)||(PFN.poz==_PFZMA))) //niedozwolona pozycja przelacznika funkcyjnego?
			{
				return;
			}
			//--------
			//wymuszenie grzania AL z konsoli
			if(GetBit2(RSDT1.inSTAT0,_INITANL))			//aktywowano recznie funkcje ANTYLEGIONELLA?
			{
				if(!(DtCW._err_CW||DtCO._err_CO))		//***********************
				{
					M.LICZNIK_ANL=0;
					M._AKTYWNE_AL=1;			//aktywacja funkcji ANTYLEGIONELLA
					StartRTM(_RTMAL);			//start timera maksymalnego dozwolonego czasu aktywnosci funkcji ANTYLEGIONELLA
					eeprom_write(_ADRRTA,0);
				}
			}
		}
		else
		{
			//StartRTG(_RTANL);						//zatrzymanie zegara (funkcja ANTYLEGIONELLA nieaktywna)
			StartRTM(_SRVRTANL);
			M.LICZNIK_ANL=0;
		}
	}
	else M._AKTYWNE_AL=0;
#else
	if(DtKNF.tpkt==_UNICO)	//obowiazuje tylko dla kotlow z obsluga zasobnika
	{
		//--------
		//Warunki przerwania realizacji funkcji ANTYLEGIONELLA
		if(M._AKTYWNE_AL)
		{
			if(DtCW._err_CW||DtCO._err_CO)			//***************************
			{
				M._AKTYWNE_AL=0;
				return;	
			}
			if(DtKNF.aleg)
			{
				StartRTG(_RTANL);						//zatrzymanie zegara ponawiania funkcji ANTYLEGIONELLA
				M.LICZNIK_ANL=0;
			}
			if(M._AKTYWNE_KNF||M._AKTYWNE_SRW)
			{
				M._AKTYWNE_AL=0;
				return;		
			}
			//niedozwolona pozycja przelacznika funkcyjnego
			if(!((PFN.poz==_PFLTO)||(PFN.poz==_PFZMA))) 
			{
				M._AKTYWNE_AL=0;
				return;
			}
			//maksymalny dozwolony czas aktywnosci funkcji ANTYLEGIONELLA
			if(RTM(_RTMAL)>=_TEXTAL)
			{
				M._AKTYWNE_AL=0;
				return;
			}
			//deaktywowano procedure (np nacisnieto przycisk RESET)
			//if(!GetBit2(RSDT1.inSTAT0,_INITANL))
			if(PFN._res)
			{
				M._AKTYWNE_AL=0;
				return;
			}
		}
		//--------
		//Warunki aktywacji funkcji ANTYLEGIONELLA
		if(!M._AKTYWNE_AL)					//mozliwa realizacja funkcji ANTYLEGIONELLA?
		{
			if(M._AKTYWNE_KNF||M._AKTYWNE_SRW)			//realizowana funkcja serwisowa lub konfiguracyjna?
			{
				return;
			}
			//-------
			//zegar ponawiania funkcji ANTYLEGIONELLA
			if(DtKNF.aleg)
			{
				if(RTG(_RTANL)>=1)							//timer godzinowy wyzwalania funkcji ANTYLEGIONELLA
				{
					StartRTG(_RTANL);
					M.LICZNIK_ANL++;
					if(M.LICZNIK_ANL>=_NGODZIN_AL) 		//licznik godzin do wyzwolenia funkcji ANTYLEGIONELLA
					{
						M.LICZNIK_ANL=0;
						if(!(DtCW._err_CW||DtCO._err_CO))	//***************************
						{
							if((PFN.poz==_PFLTO)||(PFN.poz==_PFZMA)) M._AKTYWNE_AL=1;		//aktywacja funkcji ANTYLEGIONELLA po czasie odliczonym przez timer.
						}
						StartRTM(_RTMAL);					//start timera maksymalnego dozwolonego czasu aktywnosci funkcji ANTYLEGIONELLA
						eeprom_write(_ADRRTA,0);
						return;	
					}
					else
					{
						eeprom_write(_ADRRTA,M.LICZNIK_ANL);
					}
				}
			}
			//-------
			if(!((PFN.poz==_PFLTO)||(PFN.poz==_PFZMA))) //niedozwolona pozycja przelacznika funkcyjnego?
			{
				return;
			}
			//--------
			//wymuszenie grzania AL z konsoli
			if(GetBit2(RSDT1.inSTAT0,_INITANL))			//aktywowano recznie funkcje ANTYLEGIONELLA?
			{
				if(!(DtCW._err_CW||DtCO._err_CO))		//*****************************
				{
					M.LICZNIK_ANL=0;
					M._AKTYWNE_AL=1;			//aktywacja funkcji ANTYLEGIONELLA
					StartRTM(_RTMAL);			//start timera maksymalnego dozwolonego czasu aktywnosci funkcji ANTYLEGIONELLA
					eeprom_write(_ADRRTA,0);
				}
			}
		}
		else
		{
			StartRTG(_RTANL);						//zatrzymanie zegara (funkcja ANTYLEGIONELLA nieaktywna)
			M.LICZNIK_ANL=0;
		}
	}
	else M._AKTYWNE_AL=0;
#endif
}
//------------------
//------------------
//Regulacja stanu wyswietlacza 
void DecWSW(void)
{
	//PrintHex(PFN.AC);

	//stan plomienia
	//SetDiode(_DPL,RdPrt(S_PLM)&&M._PRACA);
/*
	BitSet2(&RSDT1.outSTAT0,_PLMCO,M._PRACA&&!M._RPM_CW&&RdPrt(S_PLM));
	BitSet2(&RSDT1.outSTAT0,_PLMCW,M._PRACA&&M._RPM_CW&&RdPrt(S_PLM));
	//funkcja serwisowa
	//SetDiode(_DKM,M._AKTYWNE_SRW);
	BitSet2(&RSDT1.outSTAT0,_FKSRV,M._AKTYWNE_SRW);	
	//sygnalizacja obiegu
	//SetDiode(_DCO,M._PRACA&&!M._RPM_CW);
	BitSet2(&RSDT1.outSTAT1,_OBGCO,M._PRACA&&!M._RPM_CW);
	//SetDiode(_DCW,M._PRACA&&M._RPM_CW);
	BitSet2(&RSDT1.outSTAT1,_OBGCW,M._PRACA&&M._RPM_CW);

#if _SRVTRB==1						//aktywny tryb serwisowy kompilacji?
	if(M._pob_SRW&&!M._ROZRUCH&&_GRZEJ)
	{
		//PrintHex(PWM.BufPWM);
		//BitSet2(&RSDT1.outSTAT0,_TRBSR,1);
		RSDT1.outCO=PWM.BufPWM;
		return;
	}
#else
	//BitSet2(&RSDT1.outSTAT0,_TRBSR,0);
#endif
*/
	/*
	if(!DtCS._no_mont)		//czujnik cisnienia zamontowany
	{
		//przelacznik funkcji na pozycji CISNIENIE
		//if((PFN.poz==_PFCSN)&&!RdPrt(S_ERR_CSN)) PrintCS(DtCS.CS);
		//else EndCS();

		//przelacznik funkcji na dowolnej pozycji oprocz pozycji CISNIENIE gdy nie jest realizowana funkcja grzania
		//if((PFN.poz!=_PFCSN)&&!M._PRACA) PrintOFF(); 
		//else
		//przelacznik funkcji na pozycji CISNIENIE gdy jest awaria czujnika cisnienia
		//if((PFN.poz==_PFCSN)&&RdPrt(S_ERR_CSN)) PrintOFF();
		//else EndOFF();
	}
	else					//czujnik cisnienia nie zamontowany
	{
		//przelacznik funkcji na dowolnej pozycji oprocz pozycji CISNIENIE gdy nie jest realizowana funkcja grzania
		if(!M._PRACA) PrintOFF(); 
		else EndOFF();
	}

	//nastawa PCO
	if(DtCO._newPCO) 
	{
		if(M._AKTYWNE_PGD) PrintNstKt(DtCO.PCO);
		else PrintNstCO(DtCO.PCO);
	}
	else
	//nastawa PCW
	if(DtCW._newPCW) PrintNstCW(DtCW.PCW);

	//przelacznik funkcji na pozycji LATO lub ZIMA
	if((PFN.poz==_PFLTO)&&!DtCW._err_CW) PrintCW(DtCW.SCW);							//pozycja LATO
	else
	if((PFN.poz==_PFZMA)&&!DtCW._err_CW&&M._pob_CW&&M._PRACA) PrintCW(DtCW.SCW);	//pozycja ZIMA i wymuszenie CW
	else 
	{
		EndCW();
		PrintCO(DtCO.SCO);
	}
	*/
	//temperatura dyzurna
	/*if(!DtCS._no_mont)		//czujnik cisnienia zamontowany
	{
		if(PFN.poz!=_PFCSN)			//przelacznik funkcji na pozycji innej niz CISNIENIE
		{
			if(M._pob_DzCO&&M._PRACA) PrintTdzCO(DtCO.SCO);
			else
			{
				EndTdzCO();
				if(M._pob_DzCW&&M._PRACA) PrintTdzCW(DtCW.SCW);	
				else EndTdzCW();
			}
		}
		else 
		{
			EndTdzCO();
			EndTdzCW();
		}
	}
	else					//czujnik cisnienia nie zamontowany
	{
		if(M._pob_DzCO&&M._PRACA) PrintTdzCO(DtCO.SCO);
		else
		{
			EndTdzCO();
			if(M._pob_DzCW&&M._PRACA) PrintTdzCW(DtCW.SCW);	
			else EndTdzCW();
		}
	}*/
}
//------------------
//------------------
//Reakcja na wiekszosc stanow awaryjnych 
void DecERR(void)
{
	unsigned char KnfDisErr=0;
	unsigned char w;
	
//Znacznik wylaczajacy wizualizacje kodu bledu podczas pracy w proc. konfiguracji
	//KnfDisErr=(M._AKTYWNE_KNF&&(DtKNF.KRK==1||(DtKNF.KRK==2&&(DtKNF.PRM==4||DtKNF.PRM==6||DtKNF.PRM==7))))?1:0;
	KnfDisErr=(M._AKTYWNE_KNF&&(DtKNF.KRK==1||(DtKNF.KRK==2&&(DtKNF.PRM>2))))?1:0;
//--------BEZWZGLEDNE WYLACZENIE Z BLOKADA
//UWAGA: natychmiastowe wyswietlenie kodu awaryjnego
//zadzialalo STB
	if(!RdPrt(S_STB))
	{
		M.ERR_BTY=0x02;
		PrintErr(0x02,1);
        ToWriteESTAT();
		ErrPTG();
	}
//--------BEZWZGLEDNE WYLACZENIE BEZ BLOKADY
	if(M._RUSZ_PLOMIEN) 
	{
		M.ERR_BTY=0;				//wyzeruj wszystkie wczesniejsze kody
		if(KnfDisErr) EndErr();		//nie wyswietlaj kodu bledu przy konfiguracji par. r4 i r6, r7
		else PrintErr(0x01,0);
		M._RUSZ_NTC_CO=0;
		M._RUSZ_CISN=0;
		M._RUSZ_NCISN=0;
		M._RUSZ_NTC_CW=0;
		M._RUSZ_MOD=0;	
		M._RUSZ_NTC_IN = 0;		
	}
	else 
	if(M._RUSZ_CIAG)
	{
		M.ERR_BTY=0;		//wyzeruj wszystkie wczesniejsze kody
		if(KnfDisErr) EndErr();		//nie wyswietlaj kodu bledu przy konfiguracji par. r4 i r6, r7
		else
		{
			if(M._prb_CIG)
			{
 				PrintErr(0x03,0);
			}
		}
		M._RUSZ_NTC_CO=0;
		M._RUSZ_CISN=0;
		M._RUSZ_NCISN=0;
		M._RUSZ_NTC_CW=0;
		M._RUSZ_MOD=0;
		M._RUSZ_NTC_IN = 0;
	}
	else
	//awaria czujnika CO (wersja dla kotla mono i bitermicznego)
	if((RdPrt(S_ERR_CO)&&(DtKNF.tpkt!=_BITERM)&&(DtKNF.tpkt!=_BITERM2))||(RdPrt(S_ERR_CO)&&!M._pob_KCW&&!M._pob_CW&&(DtKNF.tpkt==_BITERM||DtKNF.tpkt==_BITERM2)))
	{
		M.ERR_BTY=0x04;
		if(KnfDisErr) EndErr();		//nie wyswietlaj kodu bledu przy konfiguracji par. r4 i r6, r7
		else PrintErr(0x04,1);
		M._RUSZ_NTC_CO=1;
		M._RUSZ_CISN=0;
		M._RUSZ_NCISN=0;
		M._RUSZ_NTC_CW=0;
		M._RUSZ_MOD=0;
		M._RUSZ_NTC_IN = 0;
	}
	else
	{
		M._RUSZ_NTC_CO=0;
		//brak przeplywu WG
		if(M._RUSZ_POMPE)
		{
			M.ERR_BTY=0;
			if(KnfDisErr) EndErr();		//nie wyswietlaj kodu bledu przy konfiguracji par. r4 i r6, r7
			else
			{
				if(M._prb_PRZ) PrintErr(0x05,0);
			}
			M._RUSZ_CISN=0;
			M._RUSZ_NCISN=0;
			M._RUSZ_NTC_CW=0;
			M._RUSZ_MOD=0;
			M._RUSZ_NTC_IN = 0;
		}
		else
		{
			//awaria obwodu czujnika cisnienia
			if(RdPrt(S_ERR_CSN)) 
			{
				M.ERR_BTY=0x08;
				if(KnfDisErr) EndErr();		//nie wyswietlaj kodu bledu przy konfiguracji par. r4 i r6, r7
				else PrintErr(0x08,1);
				M._RUSZ_NCISN=1;
				M._RUSZ_CISN=0;
				M._RUSZ_NTC_CW=0;
				M._RUSZ_MOD=0;
				M._RUSZ_NTC_IN = 0;
			}
			else
			{
				M._RUSZ_NCISN=0;
				//zbyt niskie lub zbyt wysokie cisnienie w obiegu CO
				if(!RdPrt(S_WRK_CSN))
				{

					M.ERR_BTY=0x09;
					if(KnfDisErr) EndErr();		//nie wyswietlaj kodu bledu przy konfiguracji par. r4 i r6, r7
					else PrintErrCs(0x09,DtCS.CS,1);
					M._RUSZ_CISN=1;
					M._RUSZ_NTC_CW=0;
					M._RUSZ_MOD=0;
					M._RUSZ_NTC_IN = 0;
				}
				else
				{
					M._RUSZ_CISN=0;
					if(DtKNF.tpkt!=_UNICO)
					{
						w=(DtCW._err_CW&&(PFN.poz==_PFLTO))||((PFN.poz==_PFZMA)&&DtCW._err_CW&&(M._pob_CW||M._pob_KCW||M._pob_AL));
					}
					else
					{
                        if(!DtKNF.cwtm)     //(timera zasobnika||LIN_CW||OT_CW)
                        {
                            w=(DtCW._err_CW&&(PFN.poz==_PFLTO))||((PFN.poz==_PFZMA)&&DtCW._err_CW&&RdPrt(S_WCW));
                        }
                        else                //timera zasobnika
                        {
                            w=(DtCW._err_CW&&(PFN.poz==_PFLTO))||((PFN.poz==_PFZMA)&&DtCW._err_CW&&RdPrt(S_TIM));                            
                        }
					}
					//awaria czujnika CW
					if(w)
					{
						M.ERR_BTY=0x0A;//0x10;
						if(KnfDisErr) EndErr();		//nie wyswietlaj kodu bledu przy konfiguracji par. r4 i r6, r7
						else PrintErr(0x0A,1);
						M._RUSZ_NTC_CW=1;
						M._RUSZ_MOD=0;
						M._RUSZ_NTC_IN = 0;
					}
					else
					{
						M._RUSZ_NTC_CW=0;
						//awaria obwodu modulatora
						if(!RdPrt(S_MOD_ON))
						{
							M.ERR_BTY=0x07;
							if(KnfDisErr) EndErr();		//nie wyswietlaj kodu bledu przy konfiguracji par. r4 i r6, r7
							else PrintErr(0x07,1);
							M._RUSZ_MOD=1;
							M._RUSZ_NTC_IN = 0;
						}
						else
						{
							M._RUSZ_MOD=0;
							if(DtKNF.rdpm != 0)
                            {
                                if((PFN.poz == _PFZMA) && (M._PRACA && !M._RPM_CW) && DtIN._err_IN) //pozycja ZIMA, grzanie w obiegu CO
                                {
                                    M.ERR_BTY = 0x0E;
                                    if(KnfDisErr) EndErr(); //nie wyswietlaj kodu bledu przy konfiguracji par. r4 i r6, r7
                                    else PrintErr(0x0E,1);
                                    M._RUSZ_NTC_IN = 1;
                                }
                                else
                                {
                                    M._RUSZ_NTC_IN = 0;
                                }
                            }
                            else
                            {
                                M._RUSZ_NTC_IN = 0;
                            }
						}
					}

				}
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
			/*if(!M.LICZNIK_CIG) StartRTS(_RTRSCI);
			M.LICZNIK_CIG++;
			if(M.LICZNIK_CIG>=_NCYKLI_CIG)
			{
				M.LICZNIK_CIG=0;
				M.ERR_BTY=0x03;
				PrintErr(0x03,1);
             *  ToWriteESTAT();
				ErrPTG();
			}*/
		}
	}
	else				//kociol turbo (praca wentylatora, pomijana procedura rozruchu)
	{
		if(!M._RUSZ_CIAG&&!M._ROZRUCH&&!M._DE_VNT&&((!RdPrt(S_CIG_NO)&&_VNT)||(RdPrt(S_CIG_NO)&&!_VNT)))
		{
			M.RCI=0;
			M._RUSZ_CIAG=1;						//aktywuj procedure awaryjna
			if(!M.LICZNIK_CIG) StartRTS(_RTRSCI); //licznik liczby zanikow ciagu na minute
			M.LICZNIK_CIG++;
			if(M.LICZNIK_CIG>=_NCYKLI_CIG)		//przekroczono dozwolona liczbe zanikow ciagu na minute?
			{
				M.LICZNIK_CIG=0;
				M.ERR_BTY=0x03;
				PrintErr(0x03,1);
                ToWriteESTAT();
				ErrPTG();
			}
		}
	}
	

//--------REAKCJA NA BRAK SYTUACJI AWARYJNEJ (DOTYCZY WIZUALIZACJI KODU AWARYJNEGO)

	//usun kod awaryjny z wyswietlacza
	if(!M._RUSZ_PLOMIEN&&!M._RUSZ_CIAG)
	if(!M._RUSZ_NTC_CO&&!M._RUSZ_NCISN&&!M._RUSZ_CISN)
	if(!M._RUSZ_NTC_CW&&!M._RUSZ_MOD&&!M._RUSZ_NTC_IN&&!M._RUSZ_POMPE)
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
    
    //Decyzja o aktywacji/dezaktywacji regulatora pogodowego
    SetTrbPGD();
    
	//reakcja na zbyt szybki narost temperatury
	if(!M._ROZRUCH&&_GRZEJ&&DtCO._ldDCO) M._RUN_MNMOD=1;
	else M._RUN_MNMOD=0;
	//--------------
	//wybor obiegu dla pompy 
    if((DtKNF.tpkt==_UNICO)&&(DtCW._offKCW==1))     //spelnione warynki dla blokady zaworu 3DR dla obiegu CW?
    {
        if(!M._ZMKCOCW)
        {
            if(!M._RUSZ_48Z3) ToObiegCO();
        }
    }
    else    //(w stanie spoczynku pompa pracuje w obiegu CW)
    {    
        if(!M._ZMKCOCW)
        {
            //wymuszenie dla obiegu CW w trakcie pracy? 
            if(M._PRACA&&(M._pob_KCW||M._pob_CW||M._pob_DzCW||M._pob_AL)) ToObiegCW();
            else
            //wymuszenie dla obiegu CO w trakcie pracy?
            if(M._PRACA&&!M._BLOKUJ_15S&&(M._pob_KCO||M._pob_SRW||M._pob_CO||M._pob_DzCO)) ToObiegCO();
            else
            //pompa pracuje, pobudzenie CW,temperatura CO poza zakresem
            if(!M._PRACA&&M._WYLACZ&&(DtKNF.tpkt==_MONOHB)&&(!DtCO._err_CO&&!DtCO._ptwp_CO)&&M._pob_CW) ToObiegCW();
            else
            if(!M._PRACA&&M._WYLACZ&&(DtKNF.tpkt==_UNICO)&&(!DtCO._err_CO&&!DtCO._ptwp_CO)&&((PFN.poz==_PFLTO)||(PFN.poz==_PFZMA))&&DtCW._wpz_CW) ToObiegCW();
            else
            //temperatura CO poza zakresem lub aktywny wybieg 48h?
            //if(!M._PRACA&&!M._BLOKUJ_15S&&(RdPrt(S_ON_PMP)||M._RUSZ_48)) ToObiegCO();
            if(!M._PRACA&&!M._BLOKUJ_15S&&(RdPrt(S_ON_PMP)||M._RUSZ_48))
            {
                if(PFN.poz==_PFOFF) ToObiegCO();            //pozycja OFF
                else
                if(PFN.poz==_PFLTO)                         //pozycja LATO
                {
                    if((DtKNF.tpkt!=_INWEST)&&(DtKNF.tpkt!=_INWEST2))
                    {
                         if(DtCO._cold_CO) ToObiegCO();
                         else ToObiegCW();  
                    }
                    else ToObiegCO();
                }
                else                                        //pozycja ZIMA
                {
                    if(!M._pob_KCW && !M._pob_CW && !M._pob_DzCW && !M._pob_AL) ToObiegCO();                //brak pobudzenia CW
                    else ToObiegCW();                                                                       //aktywne pobudzenie CW   
                }
            }
            else
            //aktywne RT i temp. CO wyzsza od warunku zalaczenia kotla?	
            if(!M._PRACA&&!M._BLOKUJ_15S&&M._pob_CO&&!DtCO._su_CO) ToObiegCO();
            else	//kociol w spoczynku
            {
                switch(DtKNF.tpkt)		//dla danego rodzaju kotla
                {
                    case _BITERM:		//bitermiczny (miniterm)
                    case _BITERM2:		//bitermiczny (maxiterm)
                    {
                        //nieaktywne warunki wybiegu pompy?
                        if(!M._PRACA&&!M._WYLACZ&&!M._RUSZ_48) ToObiegCW();
                        break;
                    }
                    case _MONOHB:
                    {
                        //nieaktywna pompa?
                        if(!M._PRACA&&!M._WYLACZ&&!M._RUSZ_48&&(!_POMPA||M._RUSZ_48Z3)) ToObiegCW();
                        break;
                    }
                    case _UNICO:
                    {					
                        //b.niska temperatura CW lub nieaktywna pompa?
                        //if(!M._PRACA&&(RdPrt(S_ON_PMP_CW)||(!M._WYLACZ&&!M._RUSZ_48&&!_POMPA))) ToObiegCW();
                        if(!M._PRACA&&(RdPrt(S_ON_PMP_CW))) //b.niska temperatura CW?
                        {     
                            ToObiegCW();
                        }
                        else                                //nieaktywna pompa?
                        {
                            if(!M._WYLACZ&&!M._RUSZ_48&&!_POMPA) ToObiegCO(); 
                        }
                        break;
                    }
                    default:			//domyslny monotermiczny
                    {
                        //nieaktywna pompa?
                        if(!M._PRACA&&!M._WYLACZ&&!M._RUSZ_48&&!_POMPA) ToObiegCW();
                        break;
                    }
                }
            }
        }
    }
	//--------------
	//decyzja o aktywacji wybiegu pompy
	if(!M._ZMKCOCW)
	{	
		switch(DtKNF.tpkt)								//dla danego rodzaju kotla
		{
			case _BITERM:		//bitermiczny (miniterm)
			case _BITERM2:		//bitermiczny (maxiterm)
			{
				if(!M._WYLACZ&&!M._RUSZ_48&&!M._PRACA)
				{
					//temperatura CO/CW poza dozwolonym zakresem
					if(!M._RPM_CW&&RdPrt(S_ON_PMP))
					{
						M._WYLACZ=1;				 //uaktywnij wybieg pompy
					}
					else
					//decyzja o zlaczeniu pompy dla RT i temp. CO wyzszej od warunku zalaczenia kotla
					if(!M._RPM_CW&&(PFN.poz!=_PFOFF) && (PFN.poz!=_PFLTO) && M._pob_CO&&!DtCO._su_CO)
					{
						M._WYLACZ=1;				//uaktywnij wybieg pompy	
					}
				}
				break;
			}
			case _MONOHB:
			{
				if(!_POMPA&&!M._WYLACZ&&!M._RUSZ_48&&!M._RUSZ_48Z3&&!M._PRACA)
				{
					//temperatura CO/CW poza dozwolonym zakresem
					if(RdPrt(S_ON_PMP))
					{
						M._WYLACZ=1;				 //uaktywnij wybieg pompy
						_POMPA=1;
					}
					else
					//decyzja o zlaczeniu pompy dla RT i temp. CO wyzszej od warunku zalaczenia kotla
					if((PFN.poz!=_PFOFF) && (PFN.poz!=_PFLTO) && M._pob_CO&&!DtCO._su_CO)
					{
						M._WYLACZ=1;				//uaktywnij wybieg pompy	
						_POMPA=1;
					}
				}
				break;
			}
			case _UNICO:
			{
				if(!_POMPA&&!M._WYLACZ&&!M._RUSZ_48&&!M._PRACA)
				{
					//temperatura CO/CW poza dozwolonym zakresem
					if((RdPrt(S_ON_PMP))||(M._RPM_CW&&RdPrt(S_ON_PMP_CW)))
					{
						M._WYLACZ=1;				 //uaktywnij wybieg pompy
						_POMPA=1;
					}
					else
					//decyzja o zlaczeniu pompy dla RT i temp. CO wyzszej od warunku zalaczenia kotla
					if((PFN.poz!=_PFOFF) && (PFN.poz!=_PFLTO) && M._pob_CO && !DtCO._su_CO)
					{
						M._WYLACZ=1;				//uaktywnij wybieg pompy	
						_POMPA=1;
					}
				}
				break;
			}
			default: 		//domyslny monotermiczny
			{
				if(!_POMPA&&!M._WYLACZ&&!M._RUSZ_48&&!M._PRACA)
				{
					//temperatura CO/CW poza dozwolonym zakresem
					if(RdPrt(S_ON_PMP))
					{
						M._WYLACZ=1;				 //uaktywnij wybieg pompy
						_POMPA=1;
					}
					else
					//decyzja o zlaczeniu pompy dla RT i temp. CO wyzszej od warunku zalaczenia kotla
					if((PFN.poz!=_PFOFF) && (PFN.poz!=_PFLTO) && M._pob_CO&&!DtCO._su_CO)
					{
						M._WYLACZ=1;				//uaktywnij wybieg pompy	
						_POMPA=1;
					}
				}
				break;
			}
		}
	}
	//--------------
	//Czasowe podbicie mocy minimalnej
	if(M._PRACA&&(M._pob_CO||M._pob_DzCO||M._pob_DzCW)&&!M._pob_SRW&&!M._AKTYWNE_KNF&&!M._pob_KCO&&!M._pob_KCW&&!M._pob_CW&&!M._pob_AL)
	{
		if((PWM.DKNF_MDMOD<PWM.DV16_MDMOD)&&(RTS(_RTWPWM)<_TWSPWM))
		{
			PWM.MDMOD=PWM.DV16_MDMOD;	//czasowa praca z moca minimalna od 25 do 50%
		}
		else
		{
			PWM.MDMOD=PWM.DKNF_MDMOD;	//praca z domyslna moca minimalna zdefiniowana w konfiguracji
		}
	}
	else
	{
		PWM.MDMOD=PWM.DKNF_MDMOD;	//praca z domyslna moca minimalna zdefiniowana w konfiguracji
	}
	//--------------
	//aktywacja/deaktywacja bloku modulacji plomieniem dla pobudzen RT i CW
	if(M._pob_CO||M._pob_CW||M._pob_AL)
	{
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
	if(!M._PRACA||!RdPrt(S_MOD_ON)||M._RUN_MNMOD||(!M._ROZRUCH&&M._LOW_PMP))
	{
		M._MODULUJ=0;	//wylacz blok modulacji
		if(!M._PRACA||!RdPrt(S_MOD_ON)) WritePWM(0);
		else WritePWM(PWM.MDMOD);		//zbyt szybki narost temp.
	}
	//wymuszenie konkretnej wartosci wypelnienia dla danego wymuszenia
	if(M._pob_KCW&&!M._ROZRUCH&&_GRZEJ) WritePWM(PWM.MMAXCW);
	if(M._pob_KCO&&!M._ROZRUCH&&_GRZEJ) WritePWM(PWM.MMAXCO);
#if _SRVTRB==1														//aktywny tryb serwisowy kompilacji?
	if(M._pob_SRW&&!M._ROZRUCH&&_GRZEJ) WriteOPWM(mypwm);
#else
	if(M._pob_SRW&&!M._ROZRUCH&&_GRZEJ)
	{
		if(GetBit2(RSDT1.inSTAT1,_SRVMAX)) WritePWM(_MG2_MOD);//WritePWM(PWM.MMAXCO);//WritePWM(_MG2_MOD);		//tryb serwisowy z moca maksymalna?
		else WritePWM(PWM.MDMOD);
	}
#endif
	if(M._pob_DzCO&&!M._ROZRUCH&&_GRZEJ) WritePWM(PWM.MDMOD);
	if(M._pob_DzCW&&!M._ROZRUCH&&_GRZEJ) WritePWM(PWM.MDMOD);
}
//zmiana obiegu na CO
void ToObiegCO(void)
{
	switch(DtKNF.tpkt)		//dla danego rodzaju kotla
	{
		case _BITERM:		//bitermiczny (miniterm)
		case _BITERM2:		//bitermiczny (maxiterm)
		{
			if(!M._ZMKCOCW&&!_POMPA) 			//aktywny obieg CW?
			{
				M._RPM_CW=0;					//wyzeruj znacznik obiegu CW
				M._ZMKCOCW=1;					//uruchom procedure zmiany obiegu CW na CO
				M.ZKP=0;
			}
			return;
		}
		case _UNICO:
		case _MONOHB:
		{		
			if(!M._ZMKCOCW&&!_KCO_CW) 			//aktywny obieg CW?
			{
				M._RPM_CW=0;					//wyzeruj znacznik obiegu CW
				M._ZMKCOCW=1;					//uruchom procedure zmiany obiegu CW na CO
				M.ZKP=0;
			}
			return;
		}
		default: 			//domyslny monotermiczny
		{
			if(!M._ZMKCOCW&&!_KCO_CW) 			//aktywny obieg CW?
			{
				M._RPM_CW=0;					//wyzeruj znacznik obiegu CW
				M._ZMKCOCW=1;					//uruchom procedure zmiany obiegu CW na CO
				M.ZKP=0;
			}
			return;
		}
	}
}
//zmiana obiegu na CW
void ToObiegCW(void)
{
	switch(DtKNF.tpkt)								//dla danego rodzaju kotla
	{
		case _BITERM:		//bitermiczny (miniterm)
		case _BITERM2:		//bitermiczny (maxiterm)
		{
			if(!M._ZMKCOCW&&_POMPA) 			//aktywny obieg CO?
			{
				M._RPM_CW=1;					//ustaw znacznik obiegu CW
				M._ZMKCOCW=1;					//uruchom procedure zmiany obiegu CO na CW
				M.ZKP=0;
				if(M._pob_KCW||M._pob_CW||M._pob_DzCW)
				{
					if(M._PRACA)
					{
						M._BLOKUJ_15S=1;			//ustaw flage blokady 15 sek dla grzania CO
						M.BL15=0;
					}
				}
			}
			return;
		}
		case _UNICO:
		case _MONOHB:
		{		
			if(!M._ZMKCOCW&&_KCO_CW) 			//aktywny obieg CO?
			{
				M._RPM_CW=1;					//ustaw znacznik obiegu CW
				M._ZMKCOCW=1;					//uruchom procedure zmiany obiegu CO na CW
				M.ZKP=0;
				if(M._pob_KCW||M._pob_CW||M._pob_DzCW||M._pob_AL)
				{
					if(M._PRACA)
					{
						M._BLOKUJ_15S=1;				//ustaw flage blokady 15 sek dla grzania CO
						M.BL15=0;
					}
				}
			}
			return;
		}
		default: 			//domyslny monotermiczny
		{		
			if(!M._ZMKCOCW&&_KCO_CW) 			//aktywny obieg CO?
			{
				M._RPM_CW=1;					//ustaw znacznik obiegu CW
				M._ZMKCOCW=1;					//uruchom procedure zmiany obiegu CO na CW
				M.ZKP=0;
				if(M._pob_KCW||M._pob_CW||M._pob_DzCW||M._pob_AL)
				{
					if(M._PRACA)
					{
						M._BLOKUJ_15S=1;				//ustaw flage blokady 15 sek dla grzania CO
						M.BL15=0;
					}
				}
			}
			return;
		}
	}
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
	switch(DtKNF.tpkt)								//dla danego rodzaju kotla
	{
		case _BITERM:		//bitermiczny (miniterm)
		case _BITERM2:		//bitermiczny (maxiterm)
		{
			if(!M._ROZRUCH&&!_GRZEJ)
			if(!M._ZMKCOCW&&!M._RUSZ_PWR&&!M._RUSZ_PLOMIEN&&!M._RUSZ_CIAG&&!M._RUSZ_STB&&!M._RUSZ_NTC_CO&&!M._RUSZ_NTC_CW)
			if(!M._RUSZ_POMPE&&!M._RUSZ_CISN&&!M._RUSZ_NCISN)
			{
				if(M._pob_KCW)										//konfiguracja obieg CW
				{
					if(DtCW._su_CW)
					{
						M._BLOKUJ_15S=1;
						M.BL15=0;
		 				SetRozruch();
					}						
				}
				else
				if(M._pob_KCO)										//konfiguracja obieg CO 
				{
					if(DtCO._su_CO) SetRozruch();						
				}												
				else
				if(M._pob_SRW)									//funkcja serwisowa
				{
					if(DtCO._su_SRW_CO) SetRozruch();						
				}								
				else
				if(M._pob_CW)										//grzanie obiegu CW
				{
					if(DtCW._su_CW)
					{
						M._BLOKUJ_15S=1;
						M.BL15=0;
		 				SetRozruch();
					}						
				}						
				else
				if(M._pob_CO&&DtCO._su_CO&&!M._BLOKUJ_3M&&!M._BLOKUJ_15S)		//grzanie obiegu CO 
				{
		 			SetRozruch();						
				}		
				else
				if(M._pob_DzCO&&DtCO._su_DZ_CO&&!M._BLOKUJ_3M&&!M._BLOKUJ_15S)	//temperatura dyzurna w obiegu CO
				{
		 			SetRozruch();						
				}
				else
				if(M._pob_DzCW&&DtCW._su_DZ_CW)									//temperatura dyzurna w obiegu CW
				{
					M._BLOKUJ_15S=1;
					M.BL15=0;
		 			SetRozruch();						
				}				
			}
			break;
		}
		case _UNICO:
		case _MONOHB:
		{
			if(!M._ROZRUCH&&!_GRZEJ)
			if(!M._ZMKCOCW&&!M._RUSZ_PWR&&!M._RUSZ_PLOMIEN&&!M._RUSZ_CIAG&&!M._RUSZ_STB&&!M._RUSZ_NTC_CO&&!M._RUSZ_NTC_CW)
			if(!M._RUSZ_POMPE&&!M._RUSZ_CISN&&!M._RUSZ_NCISN)
			{
				if(M._pob_KCW)						//konfiguracja obieg CW
				{
					if(DtCW._su_CW&&DtCO._pz_CO)
					{
						M._BLOKUJ_15S=1;
						M.BL15=0;
		 				SetRozruch();
					}						
				}
				else
				if(M._pob_KCO)										//konfiguracja obieg CO 
				{
					if(DtCO._su_CO) SetRozruch();						
				}												
				else
				if(M._pob_SRW)									//funkcja serwisowa
				{
					if(DtCO._su_SRW_CO) SetRozruch();						
				}								
				else
				if(M._pob_AL)				
				{
					if(DtCO._pz_CO)
					{
						M._BLOKUJ_15S=1;
						M.BL15=0;
		 				SetRozruch();
					}						
				}
				else
				if(M._pob_CW)							//grzanie obiegu CW
				{
					if(DtCW._su_CW&&DtCO._pz_CO)
					{
						M._BLOKUJ_15S=1;
						M.BL15=0;
		 				SetRozruch();
					}						
				}						
				else
				if(M._pob_CO&&DtCO._su_CO&&!M._BLOKUJ_3M&&!M._BLOKUJ_15S)		//grzanie obiegu CO 
				{
		 			SetRozruch();						
				}		
				else
				if(M._pob_DzCO&&DtCO._su_DZ_CO&&!M._BLOKUJ_3M&&!M._BLOKUJ_15S)	//temperatura dyzurna w obiegu CO
				{
		 			SetRozruch();						
				}
				else
				if(M._pob_DzCW&&DtCW._su_DZ_CW&&DtCO._pz_CO)					//temperatura dyzurna w obiegu CW
				{
					M._BLOKUJ_15S=1;
					M.BL15=0;
		 			SetRozruch();						
				}				
			}
			break;
		}
		default:				//domyslny monotermiczny
		{
			if(!M._ROZRUCH&&!_GRZEJ)
			if(!M._ZMKCOCW&&!M._RUSZ_PWR&&!M._RUSZ_PLOMIEN&&!M._RUSZ_CIAG&&!M._RUSZ_STB&&!M._RUSZ_NTC_CO&&!M._RUSZ_NTC_CW)
			if(!M._RUSZ_POMPE&&!M._RUSZ_CISN&&!M._RUSZ_NCISN)
			{
				if(M._pob_KCW)						//konfiguracja obieg CW
				{
					if(DtCW._su_CW&&DtCO._pz_CO)
					{
						M._BLOKUJ_15S=1;
						M.BL15=0;
		 				SetRozruch();
					}						
				}
				else
				if(M._pob_KCO)										//konfiguracja obieg CO 
				{
					if(DtCO._su_CO) SetRozruch();						
				}												
				else
				if(M._pob_SRW)									//funkcja serwisowa
				{
					if(DtCO._su_SRW_CO) SetRozruch();						
				}								
				else
				if(M._pob_AL)				
				{
					if(DtCO._pz_CO)
					{
						M._BLOKUJ_15S=1;
						M.BL15=0;
		 				SetRozruch();
					}						
				}
				else
				if(M._pob_CW)							//grzanie obiegu CW
				{
					if(DtCW._su_CW&&DtCO._pz_CO)
					{
						M._BLOKUJ_15S=1;
						M.BL15=0;
		 				SetRozruch();
					}						
				}						
				else
				if(M._pob_CO&&DtCO._su_CO&&!M._BLOKUJ_3M&&!M._BLOKUJ_15S)		//grzanie obiegu CO 
				{
		 			SetRozruch();						
				}		
				else
				if(M._pob_DzCO&&DtCO._su_DZ_CO&&!M._BLOKUJ_3M&&!M._BLOKUJ_15S)	//temperatura dyzurna w obiegu CO
				{
		 			SetRozruch();						
				}
				else
				if(M._pob_DzCW&&DtCW._su_DZ_CW&&DtCO._pz_CO)					//temperatura dyzurna w obiegu CW
				{
					M._BLOKUJ_15S=1;
					M.BL15=0;
		 			SetRozruch();						
				}				
			}
			break;
		}
	}
}
//Uruchomienie procedury rozruchu
void SetRozruch(void)
{
	M._BLOKUJ_3M=0;				//usun blokade L3
	M.BL3=0;
	EndL3();
	M._RUSZ_48=0;
	M.R48=0;
	M._RUSZ_48Z3=0;
	M.R48Z3=0;
	M._VWYBIEG=0;
	M._DE_VNT=0;
	M.VWB=0;
	//-----
	M._KCW_PMP=0;
	M.WYL=0;
	M.RPR=0;
	M._WYLACZ=0;
	M._RUSZ_POMPE=0;
	M._ROZRUCH=1;
	M.ROZ=0;

}
//------------------
//------------------
//Decyzja o wygaszeniu palnika
void DecWylacz(void)
{
	switch(DtKNF.tpkt)								//dla danego rodzaju kotla
	{
		case _BITERM:		//bitermiczny (miniterm)
		case _BITERM2:		//bitermiczny (maxiterm)
		{
			if(!M._WYLACZ)
			if(M._ROZRUCH||_GRZEJ)
			{
				if(!M._pob_KCW&&!M._pob_KCO&&!M._pob_SRW&&!M._pob_CW&&!M._pob_CO&&!M._pob_DzCO&&!M._pob_DzCW)
				{
					SetWylacz();
					return;
				}
				if(M._RUSZ_PWR||M._RUSZ_PLOMIEN||M._RUSZ_CIAG||M._RUSZ_STB||M._RUSZ_NTC_CO||M._RUSZ_NTC_CW||M._RUSZ_POMPE||M._RUSZ_CISN||M._RUSZ_NCISN)
				{
					SetWylacz();
				}
				else
				if(M._pob_KCW)														//konfiguracja obieg CW
				{
					if(DtCW._ad_CW)	SetWylacz();						
				}
				else
				if(M._pob_KCO)										//konfiguracja obieg CO 
				{
		 			if(RdPrt(S_ADD_CO)) SetWylacz();						
				}												
				else
				if(M._pob_SRW)										//funkcja serwisowa
				{
		 			if(DtCO._ad_SRW_CO) SetWylacz();						
				}								
				else
				if(M._pob_CW)											//grzanie obiegu CW
				{
		 			if(DtCW._ad_CW) SetWylacz();						
				}						
				else
				if(M._pob_CO&&(RdPrt(S_ADD_CO)||M._BLOKUJ_3M||M._BLOKUJ_15S))		//grzanie obiegu CO 
				{
					if(RdPrt(S_ADD_CO)&&!M._BLOKUJ_3M&&!M._BLOKUJ_15S) 
					{
						M._BLOKUJ_3M=1;												//uaktywnij blokade L3
						M.BL3=0;
						PrintL3();
					}
		 			SetWylacz();						
				}		
				else
				if(M._pob_DzCO&&(DtCO._ad_DZ_CO||M._BLOKUJ_3M||M._BLOKUJ_15S))		//temperatura dyzurna w obiegu CO
				{
		 			SetWylacz();						
				}
				else
				if(M._pob_DzCW&&DtCW._ad_DZ_CW)										//temperatura dyzurna w obiegu CW
				{
		 			SetWylacz();						
				}
			}
			break;
		}
		case _UNICO:
		case _MONOHB:
		{
			if(!M._WYLACZ)
			if(M._ROZRUCH||_GRZEJ)
			{
				if(!M._pob_KCW&&!M._pob_AL&&!M._pob_KCO&&!M._pob_SRW&&!M._pob_CW&&!M._pob_CO&&!M._pob_DzCO&&!M._pob_DzCW)
				{
					SetWylacz();
					return;
				}
				if(M._RUSZ_PWR||M._RUSZ_PLOMIEN||M._RUSZ_CIAG||M._RUSZ_STB||M._RUSZ_NTC_CO||M._RUSZ_NTC_CW||M._RUSZ_POMPE||M._RUSZ_CISN||M._RUSZ_NCISN)
				{
					SetWylacz();
				}
				else
				if(M._pob_KCW)							//konfiguracja obieg CW
				{
		 			if(DtCW._ad_CW||!DtCO._pz_CO) SetWylacz();						
				}
				else
				if(M._pob_AL)										//konfiguracja obieg CW
				{
		 			if(!DtCO._pz_CO) SetWylacz();						
				}
				else
				if(M._pob_KCO)										//konfiguracja obieg CO 
				{
		 			if(RdPrt(S_ADD_CO)) SetWylacz();						
				}												
				else
				if(M._pob_SRW)										//funkcja serwisowa
				{
		 			if(DtCO._ad_SRW_CO) SetWylacz();						
				}								
				else
				if(M._pob_CW)							//grzanie obiegu CW
				{
		 			if(DtCW._ad_CW||!DtCO._pz_CO) SetWylacz();						
				}						
				else
				if(M._pob_CO&&(RdPrt(S_ADD_CO)||M._BLOKUJ_3M||M._BLOKUJ_15S))		//grzanie obiegu CO 
				{
					if(RdPrt(S_ADD_CO)&&!M._BLOKUJ_3M&&!M._BLOKUJ_15S) 
					{
						M._BLOKUJ_3M=1;												//uaktywnij blokade L3
						M.BL3=0;
						PrintL3();
					}
		 			SetWylacz();						
				}		
				else
				if(M._pob_DzCO&&(DtCO._ad_DZ_CO||M._BLOKUJ_3M||M._BLOKUJ_15S))		//temperatura dyzurna w obiegu CO
				{
		 			SetWylacz();						
				}
				else
				if(M._pob_DzCW&&(DtCW._ad_DZ_CW||!DtCO._pz_CO))						//temperatura dyzurna w obiegu CW
				{
		 			SetWylacz();						
				}
			}
			break;
		}
		default:		//domyslny monotermiczny
		{
			if(!M._WYLACZ)
			if(M._ROZRUCH||_GRZEJ)
			{
				if(!M._pob_KCW&&!M._pob_AL&&!M._pob_KCO&&!M._pob_SRW&&!M._pob_CW&&!M._pob_CO&&!M._pob_DzCO&&!M._pob_DzCW)
				{
					SetWylacz();
					return;
				}
				if(M._RUSZ_PWR||M._RUSZ_PLOMIEN||M._RUSZ_CIAG||M._RUSZ_STB||M._RUSZ_NTC_CO||M._RUSZ_NTC_CW||M._RUSZ_POMPE||M._RUSZ_CISN||M._RUSZ_NCISN)
				{
					SetWylacz();
				}
				else
				if(M._pob_KCW)							//konfiguracja obieg CW
				{
		 			if(DtCW._ad_CW||!DtCO._pz_CO) SetWylacz();						
				}
				else
				if(M._pob_AL)										//konfiguracja obieg CW
				{
		 			if(!DtCO._pz_CO) SetWylacz();						
				}
				else
				if(M._pob_KCO)										//konfiguracja obieg CO 
				{
		 			if(RdPrt(S_ADD_CO)) SetWylacz();						
				}												
				else
				if(M._pob_SRW)										//funkcja serwisowa
				{
		 			if(DtCO._ad_SRW_CO) SetWylacz();						
				}								
				else
				if(M._pob_CW)							//grzanie obiegu CW
				{
		 			if(DtCW._ad_CW||!DtCO._pz_CO) SetWylacz();						
				}						
				else
				if(M._pob_CO&&(RdPrt(S_ADD_CO)||M._BLOKUJ_3M||M._BLOKUJ_15S))		//grzanie obiegu CO 
				{
					if(RdPrt(S_ADD_CO)&&!M._BLOKUJ_3M&&!M._BLOKUJ_15S) 
					{
						M._BLOKUJ_3M=1;												//uaktywnij blokade L3
						M.BL3=0;
						PrintL3();
					}
		 			SetWylacz();						
				}		
				else
				if(M._pob_DzCO&&(DtCO._ad_DZ_CO||M._BLOKUJ_3M||M._BLOKUJ_15S))		//temperatura dyzurna w obiegu CO
				{
		 			SetWylacz();						
				}
				else
				if(M._pob_DzCW&&(DtCW._ad_DZ_CW||!DtCO._pz_CO))						//temperatura dyzurna w obiegu CW
				{
		 			SetWylacz();						
				}
			}
			break;
		}
	}
}		
//Uruchomienie procedury wygaszenia palnika
void SetWylacz(void)
{
	GrzejOFF();
	//-----
	//operacje wykonywane po kazdej pracy w obiegu CW 
	switch(DtKNF.tpkt)		//dla danego rodzaju kotla
	{
		case _BITERM:		//bitermiczny (miniterm)
		case _BITERM2:		//bitermiczny (maxiterm)
		{ 
			if(!_POMPA)						//obieg CW?
			{
				M._KCW_PMP=1;				//ustaw znacznik obiegu CW
				M._BLOKUJ_15S=1;			//uaktywnij blokade 15s
				M.BL15=0;
			}
			else M._KCW_PMP=0;
			ToObiegCO();					//po grzaniu w CW zmien obieg na CO
			break;
		}
		case _UNICO:
		case _MONOHB:
		{		
			if(!_KCO_CW)					//obieg CW?
			{
				M._BLOKUJ_15S=1;			//uaktywnij blokade 15s
				M.BL15=0;		
			}
			break;
		}
		default:			//domyslny monotermiczny
		{		
			if(!_KCO_CW)					//obieg CW?
			{
				M._BLOKUJ_15S=1;			//uaktywnij blokade 15s
				M.BL15=0;		
			}
			break;
		}
	}
	//-----
	M._WYLACZ=1;							//uaktywnij procedure sterowania wybiegiem pompy
	if(M._TURBO&&_VNT) M._VWYBIEG=1;		//dla kotla turbo uaktywnij wybieg wentylatora
	M.ROZ=0;
	M.MNM=0;
	M._ROZRUCH=0;
	M._RUN_MNMOD=0;

	//wyzeruj bity pobudzen od temperatur dyzurnych
	DtCO._on_DZ_CO=0;
	SetPrt(S_DZ_CO,0);
	DtCW._on_DZ_CW=0;
	SetPrt(S_DZ_CW,0);
}
//------------------
//------------------
void FunkcjaSerwisowa(void)
{
	if(M._AKTYWNE_KNF)
	{
		M.SRV=0;
		M._AKTYWNE_SRW=0;		//deaktywacja funkcji serwisowej		
	}
	else
	switch(M.SRV)
	{
		case 0:
		{
			if((PFN.poz==_PFZMA)&&GetBit2(RSDT1.inSTAT0,_INITSRV))
			{
				M.SRV=1;
				M._AKTYWNE_SRW=1;		//aktywacja funkcji serwisowej					
				StartRTM(_RTSRV1);
			}
			break;
		}
		case 1:
		{
			//if((PFN.poz==_PFZMA)&&GetBit2(RSDT1.inSTAT0,_INITSRV)&&(RTM(_RTSRV1)<_TSRV1)) return;
			if((PFN.poz==_PFZMA)&&!PFN._res&&(RTM(_RTSRV1)<_TSRV1)) return;
			else
			{
				M.SRV=0;
				M._AKTYWNE_SRW=0;		//deaktywacja funkcji serwisowej
			}
			break;
		}
	}
}
/*
void FunkcjaSerwisowa(void)
{
	if(M._AKTYWNE_KNF)
	{
		M.SRV=0;
		M._AKTYWNE_SRW=0;		//deaktywacja funkcji serwisowej		
	}
	else
	switch(M.SRV)
	{
		case 0:
		{	
			if((PFN.poz==_PFZMA)&&PFN._res)
			{
				M.SRV=1;
				StartRTS(_RTSRV0);
			}
			break;
		}
		case 1:
		{
			if((PFN.poz==_PFZMA)&&PFN._res)
			{
				if(RTS(_RTSRV0)<_TSRV0) return;
				M.SRV=2;
				M._AKTYWNE_SRW=1;		//aktywacja funkcji serwisowej					
				StartRTM(_RTSRV1);
			}
			else
			{
				M.SRV=0;
			}
		}
		case 2:
		{
			if((PFN.poz==_PFZMA)&&(RTM(_RTSRV1)<_TSRV1)) return;
			else
			{
				M.SRV=0;
				M._AKTYWNE_SRW=0;		//deaktywacja funkcji serwisowej
			}
		}
	}
}*/
//------------------
//------------------
//Blokada L3
void BlokadaL3(void)
{
	switch(M.BL3)
	{
		case 0:
		{
			M.BL3=1;
			StartRTM(_RTBL3);
		}
		case 1:
		{
			PrintL3();
			if((PFN.poz!=_PFZMA&&!M._ZMKCOCW)||M._pob_CW||(DtKNF.tpkt==_UNICO&&DtCW._wpz_CW)) //przelacznik funkcyjny na pozycji innej niz ZIMA lub aktywne?
			{
				M._BLOKUJ_3M=0;
				M.BL3=0;
				EndL3();
			}
			if(RTM(_RTBL3)<_TBL3) return;
			M._BLOKUJ_3M=0;
			M.BL3=0;
			EndL3();
		}
	}
}
//------------------
//------------------
//Blokada 15s
void Blokada15S(void)
{
	switch(M.BL15)
	{
		case 0:
		{
			M.BL15=1;
			StartRTdS(_RTBL15);
		}
		case 1:
		{
			if((RTdS(_RTBL15)>1)&&(PFN.poz!=_PFZMA))		//przelacznik funkcyjny na pozycji innej niz ZIMA?
			{												//1 sek. zwloki na filtracje momentu zalaczenia w pozycji LATO
				M._BLOKUJ_15S=0;
				M.BL15=0;
			}
			if(RTdS(_RTBL15)<_TBL15S) return;
			M._BLOKUJ_15S=0;
			M.BL15=0;
		}
	}
}
//------------------
//------------------
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
	StartRTS(_RTWPWM);
	switch(M.ROZ)
	{
		//dla kotla z otwartym obiegiem wstepne, kontrolne wylaczenie pompy 
		case 0:
		{
			if((DtKNF.robg==_OTWARTY)||(DtKNF.tpkt==_UNICO))	//uniko lub obieg otwarty?
			{
				if(M._ZMKCOCW) return;					//odczekaj az minie procedura zmiany obiegu
			}
			M.ROZ=1;
			StartRTS(_RTROZ);
		}
		//dla kotla z otwartym obiegiem oczekiwanie az sygnal przeplywu WG zniknie przy wylaczonej pompie
		case 1:
		{
			if(DtKNF.robg==_OTWARTY) //uklad z obiegiem otwartym?
			{
				if(RTS(_RTROZ)<1) return; 			//zwloka na klepanie przekaznikiem
				_POMPA=0;  							//wylacz pompe
				if(!RdPrt(S_PRZ_CO)) 				//czekaj na stan brak przeplywu WG
				{
					M.ROZ=2;
					StartRTS(_RTROZ); 
					return;
				}
				if(RTS(_RTROZ)<_TCPRO) return;
				M.ERR_BTY=0x05;
				PrintErr(0x05,1);								//wyswietl kod bledu
                ToWriteESTAT();
				ErrPTG();										//wylaczenie awaryjne z blokad�					
				return;
			}
			M.ROZ=2;
			StartRTS(_RTROZ);
		}
		//procedura rozruchu
		case 2:
		{
			switch(DtKNF.tpkt)								//dla danego rodzaju kotla
			{
				case _BITERM:		//bitermiczny (miniterm)
				case _BITERM2:		//bitermiczny (maxiterm)
				{
					if(!M._RPM_CW&&(DtKNF.robg==_OTWARTY)) _POMPA=1;	//zalacz pompe dla obiegu CO
					break;
				}
				case _UNICO:		//jednofunkcyjny
				case _MONOHB:
				{
#if _I0_PMP==1                     
					if(!M._ZMKCOCW) _POMPA=1;					//zalacz pompe
#else
                    _POMPA=1;
#endif
					break;
				}
				default:			//domyslny monotermiczny
				{
#if _I0_PMP==1                     
					if(!M._ZMKCOCW) _POMPA=1;					//zalacz pompe
#else
                    _POMPA=1;
#endif
					break;
				}
			}
			WritePWM(PWM.MDMOD);
			M.ROZ=3;
			StartRTS(_RTROZ);
		}
		//oczekiwanie az sygnal obecnosci ciagu zniknie przy wylaczonym wentylatorze
		case 3:
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
			M.ROZ=4;
			StartRTS(_RTROZ);
		}
		//oczekiwanie na sygnal obecnosci ciagu przy zalaczonym wentylatorze
		case 4:
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
			M.ROZ=5;
			StartRTS(_RTROZ);
		}
		//kontrola poprawnosci dzialania ukladu detekcji plomienia
		case 5:
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
				PrintErr(0x06,1);					//wyswietl kod bledu
                ToWriteESTAT();
				ErrPTG();
			}
            if(DtKNF.tpkt!=_UNICO)
            {
                M.ROZ=7;
                GrzejON();							//zapal plomien na palniku                
                StartRTS(_RTROZ);
                return;
            }
            else
            {
                M.ROZ=6;
                StartRTS(_RTROZ);               
            }
		}
		//kocol jednofunkcyjny - zalaczenie pompy, wstepny wybieg przez 
		case 6:      
        {
            if((M.STRB==1)||(M.STRB==2))        //tryb produkcyjny? 
            {
#if _I0_PMP==1                     
				if(!M._ZMKCOCW) _POMPA=1;					//zalacz pompe
#else
                _POMPA=1;
#endif
            }
            else
            {            
                if(M._TURBO&&!RdPrt(S_CIG_NO_1S))		//nieobecny sygnal ciagu?
                {
                    M.RCI=0;
                    M._RUSZ_CIAG=1;
                    return;
                }            
                if(RTS(_RTROZ)>1)                   //odczekaj na domkniecie zaworu 3DR
                {
#if _I0_PMP==1                     
					if(!M._ZMKCOCW) _POMPA=1;					//zalacz pompe
#else
                    _POMPA=1;
#endif                
                }
                if(RTS(_RTROZ)<_TPRWB) return;      //odczekaj czas wstepnego wybiegu 
            }
            M.ROZ=7;
            GrzejON();                          //zapal plomien na palniku                
            StartRTS(_RTROZ);            
        }
		//zalaczenie pompy, oczekiwanie na sygnal obecnosci plomienia
		case 7:
		{
			switch(DtKNF.tpkt)					//dla danego rodzaju kotla
			{
				case _BITERM:		//bitermiczny (miniterm)
				case _BITERM2:		//bitermiczny (maxiterm)
				{
					break;
				}
				case _UNICO:
				case _MONOHB:
				{
#if _I0_PMP==1                     
					if(!M._ZMKCOCW) _POMPA=1;					//zalacz pompe
#else
                    _POMPA=1;
#endif
					break;
				}
				default:			//domyslny monotermiczny
				{
#if _I0_PMP==1                     
					if(!M._ZMKCOCW) _POMPA=1;					//zalacz pompe
#else
                    _POMPA=1;
#endif
					break;
				}				
			}			
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
					M.ROZ=8;
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
		case 8:								
		{
			if(M._RPM_CW) 
			{
				M._ROZRUCH=0;
				M.ROZ=0;
				return;
			}
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
		PrintErr(0x01,1);
        ToWriteESTAT();
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
					PrintErr(0x01,1);
                    ToWriteESTAT();
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
                    ToWriteESTAT();
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
				PrintErr(0x01,1);
                ToWriteESTAT();
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
//Wybieg pompy
void Wylacz(void)
{
	switch(DtKNF.tpkt)										//dla danego rodzaju kotla
	{
		case _BITERM:		//bitermiczny (miniterm)
		case _BITERM2:		//bitermiczny (maxiterm)
		{
			switch(M.WYL)
			{
				case 0:
				{
					M.WYL=1;
					StartRTS(_RTWYL);
				}
				case 1:
				{	
					if(M._KCW_PMP)								//wybieg po grzaniu w obiegu CW?
					{			
						if(RTS(_RTWYL)>=_TbtWBW0)				//uplynol minimalny wybieg pompy?		
						{
							if(!DtCO._err_CO&&DtCO._ptwp_CO)	//temperatura CO osiagnela prog wylaczenia?
							{
								M.WYL=2;
								return;	
							}
						}
						if(RTS(_RTWYL)<_TbtWBW1) return;  		//uplynol maksymalny wybieg pompy?
						M.WYL=2;
						return;
					}
					else										//wybieg po grzaniu w obiegu CO
					{
						if(RTS(_RTWYL)<_TWBCO) return; 			//uplynol maksymalny wybieg pompy?
						M.WYL=2;
					}
				}
				case 2:
				{
					if(RdPrt(S_ON_PMP)) return;					//przekroczone wartosci graniczne temp w obiegu CO
					if(!M._KCW_PMP)								//wybieg po grzaniu w obiegu CO?
					{
						if(!DtCO._err_CO&&!DtCO._su_CO) return;	//osiagnieto wartosc wylaczenia po pracy w ukladzie CO?
					}
					if(DtKNF.robg==_OTWARTY) 	//z obiegiem otwartym?
					{
						if(M._RUSZ_POMPE) return;						//oczekiwanie na powrot czujnika przeplywu WG do prawidlowej pozycji
					}
					M._KCW_PMP=0;
					M._WYLACZ=0;
					M.WYL=0;
				}
			}
			return;
		}
		case _MONOHB:
		{
			switch(M.WYL)
			{
				case 0:
				{
					M.WYL=1;
					StartRTS(_RTWYL);
				}
				case 1:
				{	
					if(M._RPM_CW)	//obieg CW
					{			
						if(RTS(_RTWYL)>=_TmnWBW0)				//uplynol minimalny wybieg pompy?		
						{
							if(!DtCO._err_CO&&DtCO._ptwp_CO)	//temperatura CO osiagnela prog wylaczenia?
							{
								M.WYL=2;
								return;	
							}
						}
						if(RTS(_RTWYL)<_TmnWBW1) return;  		//uplynol maksymalny wybieg pompy?
						M.WYL=2;
						return;
					}
					else			//obieg CO
					{
						if(RTS(_RTWYL)<_TWBCO) return;  				//uplynol maksymalny wybieg pompy?
						M.WYL=2;
					}
				}
				case 2:
				{
					if(!M._RPM_CW)	//obieg CO
					{
						if(RdPrt(S_ON_PMP)) return; 					//przekroczone wartosci graniczne temp w obiegu CO
						if(!DtCO._err_CO&&!DtCO._su_CO) return; 		//osiagnieto wartosc wylaczenia po pracy w ukladzie CO?
					}
					else
					{
						if(!DtCO._err_CO&&!DtCO._ptwp_CO&&M._pob_CW) return; //obieg CW, trwa wymuszenie CW a temp CO>od progu zalaczenia
					}
					if(DtKNF.robg==_OTWARTY) 	//z obiegiem otwartym?
					{
						if(M._RUSZ_POMPE) return;						//oczekiwanie na powrot czujnika przeplywu WG do prawidlowej pozycji
					}
#if _I0_PMP==1                    
					if(!M._ZMKCOCW) _POMPA=0;
#else
                    _POMPA=0;
#endif
					M._WYLACZ=0;
					M.WYL=0;
				}
			}
			return;			
		}
		case _UNICO:
		{
			switch(M.WYL)
			{
				case 0:
				{
					M.WYL=1;
					StartRTS(_RTWYL);
				}
				case 1:
				{	
					if(M._RPM_CW)	//obieg CW
					{			
						if(RTS(_RTWYL)<_TzsWBW) return;  		//uplynol maksymalny wybieg pompy?
						if(!DtCO._err_CO)
						{
							if(DtCO._ptwp_CO)					//temperatura CO osiagnela prog wylaczenia?
							{
								M.WYL=2;
							}
							return;
						}
						else
						{
							M.WYL=2;
						}
					}
					else			//obieg CO
					{
						if(RTS(_RTWYL)<_TWBCO) return;  				//uplynol maksymalny wybieg pompy?
						M.WYL=2;
					}
				}
				case 2:
				{
					if(M._RPM_CW)	//obieg CW
					{
						if(RdPrt(S_ON_PMP_CW)) return; 					//przekroczone wartosci graniczne temp w obiegu CW
					}
					else			//obieg CO
					{
						if(RdPrt(S_ON_PMP)) return; 					//przekroczone wartosci graniczne temp w obiegu CO
						if(!DtCO._err_CO&&!DtCO._su_CO) return; 		//osiagnieto wartosc wylaczenia po pracy w ukladzie CO?
					}
					if(DtKNF.robg==_OTWARTY) 	//z obiegiem otwartym?
					{
						if(M._RUSZ_POMPE) return;						//oczekiwanie na powrot czujnika przeplywu WG do prawidlowej pozycji
					}
#if _I0_PMP==1                    
					if(!M._ZMKCOCW) _POMPA=0;
#else
                    _POMPA=0;
#endif
					M._WYLACZ=0;
					M.WYL=0;
				}
			}
			return;
		}
		default:		//domyslny monotermiczny
		{
			switch(M.WYL)
			{
				case 0:
				{
					M.WYL=1;
					StartRTS(_RTWYL);
				}
				case 1:
				{	
					if(M._RPM_CW)	//obieg CW
					{			
						if(RTS(_RTWYL)>=_TmnWBW0)				//uplynol minimalny wybieg pompy?		
						{
							if(!DtCO._err_CO&&DtCO._ptwp_CO)	//temperatura CO osiagnela prog wylaczenia?
							{
								M.WYL=2;
								return;	
							}
						}
						if(RTS(_RTWYL)<_TmnWBW1) return;  		//uplynol maksymalny wybieg pompy?
						M.WYL=2;
						return;
					}
					else			//obieg CO
					{
						if(RTS(_RTWYL)<_TWBCO) return;  				//uplynol maksymalny wybieg pompy?
						M.WYL=2;
					}
				}
				case 2:
				{
					if(!M._RPM_CW)	//obieg CO
					{
						if(RdPrt(S_ON_PMP)) return; 					//przekroczone wartosci graniczne temp w obiegu CO
						if(!DtCO._err_CO&&!DtCO._su_CO) return; 		//osiagnieto wartosc wylaczenia po pracy w ukladzie CO?
					}
					else
					{
						if(!DtCO._err_CO&&!DtCO._ptwp_CO&&M._pob_CW) return; //obieg CW, trwa wymuszenie a temp CO>od progu zalaczenia
					}
					if(DtKNF.robg==_OTWARTY) 	//z obiegiem otwartym?
					{
						if(M._RUSZ_POMPE) return;						//oczekiwanie na powrot czujnika przeplywu WG do prawidlowej pozycji
					}
                    
#if _I0_PMP==1                    
					if(!M._ZMKCOCW) _POMPA=0;
#else
                    _POMPA=0;
#endif
					M._WYLACZ=0;
					M.WYL=0;
				}
			}
			return;			
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

			_VNT=0;									//wylacz wentylator
			M._DE_VNT=1;
			M.VWB=2;
		}
		case 2:
		{
			if(RTS(_RTVWB)<_TVWB1+2) return;		//beznapieciowy wybieg wentylatora
			M._DE_VNT=0;
			M._VWYBIEG=0;
			M.VWB=0;
		}
	}
}
//------------------
//------------------
//Detekcja zaniku przeplywu WG (oczekuje przez pewien czas zanim uruchomi procedure rozruchu przeplywu)
void DetekcjaPrzeplywuWG(void) 
{
	unsigned char pmp,prz;

	pmp=(_POMPA)?1:0;
	prz=(RdPrt(S_PRZ_CO))?1:0;

	if(pmp)		//pompa pracuje?
	{
		if(!prz&&!M._DE_PMP)							//zmiana stanu przep�ywu?
		{
	 		M._DE_PMP=1;
			M.DPR=0;
		}
		if(M._DE_PMP)
		{
			switch(M.DPR)
			{
				case 0:
				{
					StartRTS(_RTDPR);
					M.DPR=1;
				}
				case 1:
				{	
					if(prz)								//dla przeplywu zakoncz procedure
					{
						M._DE_PMP=0;
						M.DPR=0;
						return;
					}
					if(RTS(_RTDPR)<_TDPR1) return;		//oczekuj przez zadany czas na przep�yw WG
					if(!M._RUSZ_POMPE)
					{
						M.RPR=0;
						M._RUSZ_POMPE=1;
					}
					M._DE_PMP=0;
					M.DPR=0;
				}
			}
		}
	}
	else
	{
		M._DE_PMP=0;
		M.DPR=0;
	}	
}
//------------------
//------------------
//Oczekiwanie na powrot przeplywu
void OczekujNaPrzeplywWG(void)
{
	unsigned char pmp,prz;

	pmp=(_POMPA)?1:0;
	prz=(RdPrt(S_PRZ_CO))?1:0;

	switch(M.RPR)
	{
		case 0:
		{
			M.RPR=1;
			M._prb_PRZ=1;
			StartRTS(_RTRPR);
		}
		case 1:
		{	
			if(prz&&pmp)					//powrocil przeplyw?
			{
				M.RPR=0;
				M._RUSZ_POMPE=0;
				M._prb_PRZ=0;
				return;
			}
			if(RTS(_RTRPR)<_TRPR1) return;	//czekaj na powrot przeplywu
			M._RUSZ_POMPE=0;
			M.ERR_BTY=0x05;
			PrintErr(0x05,1);
            ToWriteESTAT();
			ErrPTG();					//wylaczenie z blokada	
		}
	}
}
//------------------
//------------------
//Zmiana kierunku obrotow pompy
void ZmienKCOKCW(void)
{
	switch(DtKNF.tpkt)								//dla danego rodzaju kotla
	{
		case _BITERM:		//bitermiczny (miniterm)
		case _BITERM2:		//bitermiczny (maxiterm)
		{
			switch(M.ZKP)
			{
				case 0:
				{
					_POMPA=!_POMPA;					//zalacz/wylacz pompe
					M.ZKP=1;
					StartRTdS(_RTZKP);
                    StartRTdS(_RTHPM);
                    M._LOW_PMP=0;            //aktywacja obnizonej predkosci pompy                     
				}
				case 1:
				{	
					if(RTdS(_RTZKP)<_TZKP/2) return;
					M._ZMKCOCW=0;
					M.ZKP=0;
				}
			}
			return;
		}
		case _UNICO:
		case _MONOHB:
		{
			switch(M.ZKP)
			{
				case 0:
				{
					M.ZKP=1;
#if _I0_PMP==1                    
					_POMPA=0;									//wylacz pompe
#endif
					StartRTdS(_RTZKP);
                    StartRTdS(_RTHPM);
                    M._LOW_PMP=1;            //aktywacja obnizonej predkosci pompy                    
				}
				case 1:
				{	
					if(RTdS(_RTZKP)<_TZKP/2) return;
					_KCO_CW=!_KCO_CW;							//zmian kierunku obrotu pompy
					M.ZKP=2;
				}
				case 2:
				{
					if(RTdS(_RTZKP)<_TZKP) return;
					M._ZMKCOCW=0;
					M.ZKP=0;
#if _I0_PMP==1
					//warunek ponownego zalaczenia pompy
					if(M._ROZRUCH||_GRZEJ||M._WYLACZ) _POMPA=1;			//wlacz pompe
					if(DtKNF.robg==_OTWARTY) 							//uniko z obiegiem otwartym?
					{
						if(M._RUSZ_POMPE) _POMPA=1;						//trwa oczekiwanie na powrot czujnika przeplywu WG do prawidlowej pozycji
					}
#endif
				}
			}
			return;
		}
		default:		//domyslny monotermiczny
		{
			switch(M.ZKP)
			{
				case 0:
				{
					M.ZKP=1;
#if _I0_PMP==1                    
					_POMPA=0;									//wylacz pompe
#endif
					StartRTdS(_RTZKP);
                    StartRTdS(_RTHPM);
                    M._LOW_PMP=1;            //aktywacja obnizonej predkosci pompy                    
				}
				case 1:
				{	
					if(RTdS(_RTZKP)<_TZKP/2) return;
					_KCO_CW=!_KCO_CW;							//zmian kierunku obrotu pompy
					M.ZKP=2;
				}
				case 2:
				{
					if(RTdS(_RTZKP)<_TZKP) return;
					M._ZMKCOCW=0;
					M.ZKP=0;
#if _I0_PMP==1                    
					//warunek ponownego zalaczenia pompy
					if(M._ROZRUCH||_GRZEJ||M._WYLACZ) _POMPA=1;	//wlacz pompe
					if(DtKNF.robg==_OTWARTY) 							//uniko z obiegiem otwartym?
					{
						if(M._RUSZ_POMPE) _POMPA=1;						//trwa oczekiwanie na powrot czujnika przeplywu WG do prawidlowej pozycji
					}
#endif
				}
			}
			return;
		}
	}
}
//Obnizona predkosc wybiegu pompy na czas przelaczenia kierunku na zaworze 3DR
void SetLowPMP(void)
{
    if(!M._ZMKCOCW&&!_POMPA)     //pompa nie pracuje?
    {
        M._LOW_PMP=0;
    }
    if(RTdS(_RTHPM)<_TLWPMP) return;         //minal czas obnizonej predkosci wybiegu pompy?
    M._LOW_PMP=0;
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
	if(!M._RUSZ_PLOMIEN&&!M._ZMKCOCW&&!M._ROZRUCH&&(RTdS(_RTRSPL)>20))
	{
		M._RUSZ_RPLM=0;
		M._OST_PL=0;
		M.LICZNIK_PL=0;
	}
}
//------------------
//------------------
//Awaryjny rozruch ciagu
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
					PrintErr(0x03,1);
                    ToWriteESTAT();
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
					M.LICZNIK_CIG3=0;	//wyzeruj licznik prob
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
				PrintErr(0x03,1);
                ToWriteESTAT();
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
					PrintErr(0x03,1);
                    ToWriteESTAT();
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
		if(RTS(_RTRSCI)>_TCIRLP)					//(turbo) po minucie wyzeruj licznik_zanik�w_ci�gu/min
		{
			M.LICZNIK_CIG=0;
		}
		if(!M._RUSZ_CIAG&&(RTS(_RTRSCI2)>_TCITRS))	//(turbo) po minucie wyzeruj licznik_prob_ci�gu/min
		{
			M.LICZNIK_CIG3=0;
		}		
	}
}
//------------------
//------------------
//Awaryjny rozruch pompy
void Proba48(void)
{
#if _SRV48H==0
	if(M._RUSZ_48||M._RUSZ_48Z3) return;
	if(_POMPA) StartRTG(_RTR48);
	else
	{
		if(RTG(_RTR48)>=_TPR48)
		{
			M.R48=0;
			M._RUSZ_48=1;
			StartRTG(_RTR48);
		}
	}
#else
	if(M._RUSZ_48||M._RUSZ_48Z3) return;
	if(_POMPA) StartRTM(_SRVRTANL);
	else
	{
		if(RTM(_SRVRTANL)>=10)
		{
			M.R48=0;
			M._RUSZ_48=1;
			StartRTM(_SRVRTANL);
		}
	}
#endif
}

//------------------
//------------------
//Wybieg pompy po 48h
void Rusz48(void)
{
	switch(DtKNF.tpkt)								//dla danego rodzaju kotla
	{
		case _BITERM:		//bitermiczny (miniterm)
		case _BITERM2:		//bitermiczny (maxiterm)
		{
			switch(M.R48)
			{
				case 0:
				{
					if(M._ZMKCOCW) return;	//odczekaj az zakonczy sie procedura zmiany obiegu
					StartRTS(_RTWB48);
					ToObiegCO();			//zalacz pompe
					M.R48=1;
				}
				case 1:
				{	
					if(RTS(_RTWB48)<_TWB48) return;
					if(M._ZMKCOCW) return;	//odczekaj az zakonczy sie procedura zmiany obiegu
					ToObiegCW();			//wylacz pompe
					M._RUSZ_48=0;
					M.R48=0;
				}
			}
			break;
		}
		case _UNICO:
		case _MONOHB:
		{
			switch(M.R48)
			{
				case 0:
				{
					ToObiegCO();
					if(M._ZMKCOCW) return;	//odczekaj az zakonczy sie procedura zmiany obiegu
					StartRTS(_RTWB48);
					_POMPA=1;				//zalacz pompe
					M.R48=1;
				}
				case 1:
				{	
					if(RTS(_RTWB48)<_TWB48) return;
					if(M._ZMKCOCW) return;	//odczekaj az zakonczy sie procedura zmiany obiegu
					_POMPA=0;				//wylacz pompe
					M._RUSZ_48=0;
					M.R48=0;
				}
			}
			break;
		}
		default:			//domyslny monotermiczny
		{
			switch(M.R48)
			{
				case 0:
				{
					ToObiegCO();
					if(M._ZMKCOCW) return;	//odczekaj az zakonczy sie procedura zmiany obiegu
					StartRTS(_RTWB48);
					_POMPA=1;				//zalacz pompe
					M.R48=1;
				}
				case 1:
				{	
					if(RTS(_RTWB48)<_TWB48) return;
					if(M._ZMKCOCW) return;	//odczekaj az zakonczy sie procedura zmiany obiegu
					_POMPA=0;				//wylacz pompe
					M._RUSZ_48=0;
					M.R48=0;
				}
			}
			break;
		}
	}
}
//-----------------------------------
//Awaryjny rozruch pompy
void Proba48Z3(void)
{
#if _SRV48H==0
	switch(DtKNF.tpkt)								//dla danego rodzaju kotla
	{
		case _MONOHB:								//uklady sterowania z hydroblokiem?
		{
			if(M._RUSZ_48Z3||M._RUSZ_48) return;
			if(_POMPA&&!_KCO_CW) StartRTG(_RTR48Z3); //pompa pracuje w obiegu WU?
			else
			{
				if(_POMPA||M._ZMKCOCW) return;
				if(RTG(_RTR48Z3)>=_TPR48Z3)
				{
					M.R48Z3=0;
					M._RUSZ_48Z3=1;
					StartRTG(_RTR48Z3);
				}
			}
			break;
		}
	}
#else
	switch(DtKNF.tpkt)								//dla danego rodzaju kotla
	{
		case _MONOHB:								//uklady sterowania z hydroblokiem?
		{
			if(M._RUSZ_48Z3||M._RUSZ_48) return;
			if(_POMPA&&!_KCO_CW) StartRTM(_RTMAL); //pompa pracuje w obiegu WU?
			else
			{
				if(_POMPA||M._ZMKCOCW) return;
				if(RTM(_RTMAL)>=9)
				{
					M.R48Z3=0;
					M._RUSZ_48Z3=1;
					StartRTM(_RTMAL);
				}
			}
			break;
		}
	}
#endif
}
//------------------
//------------------
//Wybieg pompy po 48h dla WU
void Rusz48Z3(void)
{
	switch(DtKNF.tpkt)								//dla danego rodzaju kotla
	{
		case _MONOHB:
		{
			switch(M.R48Z3)
			{
				case 0:
				{
					ToObiegCW();
					if(M._ZMKCOCW) return;	//odczekaj az zakonczy sie procedura zmiany obiegu
					StartRTS(_RTWB48);
					_POMPA=1;				//zalacz pompe
					M.R48Z3=1;
				}
				case 1:
				{	
					if(RTS(_RTWB48)<_TWB48Z3) return;
					if(M._ZMKCOCW) return;	//odczekaj az zakonczy sie procedura zmiany obiegu
					_POMPA=0;				//wylacz pompe
					M._RUSZ_48Z3=0;
					M.R48Z3=0;
				}
			}
			break;
		}
	}
}
//------------------
//------------------
//------------------
//------------------
//------------------
//------------------
//wygaszenie diod roboczych
/*void ClearDiode(void)
{
	SetDiode(_DPL,0);
	SetDiode(_DCO,0);
	SetDiode(_DCW,0);
}*/
//------------------
//------------------
//Reakcja na bledne podlaczenie do sieci
/*void ErrDTF(void)
{
	PrintErr(0x11,1);
	ClearDiode();	
	while(1)
	{
		CLRWDT();
		if(!RdPrt(S_DTF)) 			//prawidlowe podlaczenie fazy?
		{
			EndErr();				//wygas kod bledu
			return;					//wyjdz z petli
		}	
	}
}*/

//------------------
//------------------
bit KeyRes(void)
{
	switch(M.KRS)
	{
		case 0:
		{	
			if(PFN._res)
			{
				M.KRS=1;
				StartRTS(_RTSRV0);
			}
			break;
		}
		case 1:
		{
			if(PFN._res)
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
    BitSet2(&RSDT1.outSTAT0,_PGPRG,0);
    BitSet2(&RSDT1.outSTAT0,_PGRES,1);		//znacznik petli wylaczenia awaryjnego
    if(RefBHist(0))                         //uzupelnij historie stanow awaryjnych, jesli nie nastapila powtorka wystapienia to zapisz stan kotla z blokady awaryjnej w pamieci nieulotnej
    {
        RefBStat(0);                            //zapisz stan kotla w pamieci nieulotnej
    }   
	GrzejOFF();//_GRZEJ=0;
	VZPauza();	
	if(DtKNF.robg==_OTWARTY) 				//uklad z obiegiem otwartym?
	{
		if(M.ERR_BTY==0x05)					//dozwolony kod bledu?
		{
			eeprom_write(_ADRERR,M.ERR_BTY);
		}
	}														//przerwa czasowa na wypadek stanow nieustalonych
	if((M.ERR_BTY>=0x01)&&(M.ERR_BTY<=0x03)) eeprom_write(_ADRERR,M.ERR_BTY);//WriteEEPROM(_ADRERR,M.ERR_BTY); 	//zapis kodu bledu do pamieci
	M._RUSZ_POMPE=0;
	M._ROZRUCH=0;
	M._MODULUJ=0;
	WritePWM(0);
	if(!M._WYLACZ)							//aktywacja wybiegu pompy 
	{
		M.WYL=0;
		switch(DtKNF.tpkt)								//dla danego rodzaju kotla
		{
			case _BITERM:		//bitermiczny (miniterm)
			case _BITERM2:		//bitermiczny (maxiterm)
			{
				if(_POMPA) M._WYLACZ=1;
				break;
			}
			case _UNICO:
			case _MONOHB:
			{
#if _I0_PMP==1                     
                if(M._ZMKCOCW) M._WYLACZ=1;
                else
                if(_POMPA) M._WYLACZ=1;
#else
                if(_POMPA) M._WYLACZ=1;
#endif                
				break;	
			}
			default:			//domyslny monotermiczny
			{
#if _I0_PMP==1                     
                if(M._ZMKCOCW) M._WYLACZ=1;
                else
                if(_POMPA) M._WYLACZ=1;
#else
                if(_POMPA) M._WYLACZ=1;
#endif 	
			}
		}
	}
	if(M._TURBO&&_VNT) M._VWYBIEG=1;		//aktywacja wybiegu wentylatora
	M._RUSZ_PWR=0;
    M._SYNCH_PGD=0;
	M.HVZ=0;
	M.KRS=0;
	//k=0;
//------
	while(1)
	{
		CLRWDT();
		BitSet2(&RSDT1.outSTAT0,_PGPRG,0);
		BitSet2(&RSDT1.outSTAT0,_PGRES,1);		//znacznik petli wylaczenia awaryjnego
		BitSet2(&RSDT1.outSTAT1,_ERR,1);
        if(RTS(_RTSTR)>=_TSTR)
        {
            M._PW_START=0;
            M._RS_START=0;
        }        
		ReceptDataLCD();
		//blok pomiarowy
		PomiarPCO();			//wartosc nastawy CO
		PomiarPCW();			//wartosc nastawy CW
		PomiarPFN();			//stan przelacznika funkcji
		PomiarCO();				//wartosc temperatury CO
		PomiarCW();				//wartosc temperatury CW
		PomiarZW();				//temperatura z czujnika zewnetrznego
		FiltrPomiarowyCOCW();	//filtr danych opisujacych stan temperatur
		PomiarCS();				//wartosc cisnienia w instalacji CO
		PomiarIN();				//wartosc temperatury na powrocie

        SetTrbPGD();//************************
        
		//blok decyzyjny	
		Proba48();				//zegar 48h wybiegu pompy
		Proba48Z3();			//zegar 48h wybiegu pompy dla WU
		//sygnalizacja stanu pracy dla modulu LCD
		RefLCDData();
		//wybor obiegu
		switch(DtKNF.tpkt)								//dla danego rodzaju kotla
		{
			case _BITERM:		//bitermiczny (miniterm)
			case _BITERM2:		//bitermiczny (maxiterm)
			{
				//zalaczenie/wylaczenie pompy
				if(!M._RUSZ_PWR&&!M._ZMKCOCW&&RdPrt(S_ON_PMP)) ToObiegCO();		//temperatura CO poza dozwolonym zakresem?
				else
				if(!M._ZMKCOCW&&!M._WYLACZ&&!M._RUSZ_48) ToObiegCW();			//nieaktywna pompa?
				//aktywacja wybiegu		
				if(!M._WYLACZ&&_POMPA)
				{
					M._WYLACZ=1;	//uaktywnij wybieg pompy							 									//uaktywnij procedure wybiegu pompy
				}
				break;
			}
			case _MONOHB:
			{
				//wybor obiegu
				if(!M._ZMKCOCW&&RdPrt(S_ON_PMP)) ToObiegCO();							//temperatura CO poza dozwolonym zakresem?
				else
				if(!M._ZMKCOCW&&!M._WYLACZ&&!M._RUSZ_48&&(!_POMPA||M._RUSZ_48Z3)) ToObiegCW();			//nieaktywna pompa?
				//aktywacja wybiegu, zalaczenie pompy		
				if(!M._RUSZ_PWR&&!M._ZMKCOCW&&!_POMPA&&!M._WYLACZ&&!M._RUSZ_48&&!M._RUSZ_48Z3)
				{
					if(!M._RPM_CW&&RdPrt(S_ON_PMP))	 									//temperatura CO/CW poza dozwolonym zakresem?
					{
						M._WYLACZ=1;	//uaktywnij wybieg pompy							 									//uaktywnij procedure wybiegu pompy
						_POMPA=1;		//zalacz pompe
					}
				}
				break;
			}
			case _UNICO:
			{
                //wybor obiegu
                if((DtKNF.tpkt==_UNICO)&&(DtCW._offKCW==1))     //spelnione warynki dla blokady zaworu 3DR dla obiegu CW?
                {
                    if(!M._ZMKCOCW)
                    {
                        if(!M._RUSZ_48Z3) ToObiegCO();
                    }
                }
                else
                {
                    if(!M._ZMKCOCW&&RdPrt(S_ON_PMP)) ToObiegCO();							//temperatura CO poza dozwolonym zakresem?
                    else
                    if(!M._ZMKCOCW&&RdPrt(S_DZ_CW)) ToObiegCW();							//temperatura dyzurna CW?
                    else
                    if(!M._ZMKCOCW&&!_POMPA&&!M._WYLACZ&&!M._RUSZ_48) ToObiegCW();			//nieaktywna pompa?
                }
				//aktywacja wybiegu, zalaczenie pompy		
				if(!M._RUSZ_PWR&&!M._ZMKCOCW&&!_POMPA&&!M._WYLACZ&&!M._RUSZ_48)
				{
					if((!M._RPM_CW&&RdPrt(S_ON_PMP))||(M._RPM_CW&&RdPrt(S_DZ_CW)))		//temperatura CO/CW poza dozwolonym zakresem?
					{
						M._WYLACZ=1;	//uaktywnij wybieg pompy							 									//uaktywnij procedure wybiegu pompy
						_POMPA=1;		//zalacz pompe
					}
				}
				break;
			}
			default:			//domyslny monotermiczny
			{
				//wybor obiegu
				if(!M._ZMKCOCW&&RdPrt(S_ON_PMP)) ToObiegCO();							//temperatura CO poza dozwolonym zakresem?
				else
				if(!M._ZMKCOCW&&!_POMPA&&!M._WYLACZ&&!M._RUSZ_48) ToObiegCW();			//nieaktywna pompa?
				//aktywacja wybiegu, zalaczenie pompy		
				if(!M._RUSZ_PWR&&!M._ZMKCOCW&&!_POMPA&&!M._WYLACZ&&!M._RUSZ_48)
				{
					if(!M._RPM_CW&&RdPrt(S_ON_PMP))	 									//temperatura CO/CW poza dozwolonym zakresem?
					{
						M._WYLACZ=1;	//uaktywnij wybieg pompy							 									//uaktywnij procedure wybiegu pompy
						_POMPA=1;		//zalacz pompe
					}
				}
				break;
			}						
		}		
//blok wykonawczy
		if(M._ZMKCOCW) ZmienKCOKCW();		//zmiana kierunku obrotu pompy
		if(M._WYLACZ) Wylacz();				//kontrola wybiegu pompy
		if(M._VWYBIEG) VWybieg();			//wybieg wentylatora
		if(M._RUSZ_48) Rusz48();			//wybieg pompy co 48h
		if(M._RUSZ_48Z3) Rusz48Z3();		//wybieg pompy co 48h w obiegu WU

//wypelnienie dla pompy PWM
		if(_POMPA)
		{
			PMP.APROC=99;
		}
		else
		{
			PMP.APROC=0;
		}
		WritePWM2(ProcToPmpPwm(PMP.APROC));

//obsluga przycisku RESET
		if(KeyRes()) 
		{
			_VNT=0;
			_POMPA=0;
			ToReset();
		}
	}
}
//Wymuszenie resetu sterownika.
void ToReset(void)
{
	_VNT=0;
	_POMPA=0;
	EndErr();
	BitSet2(&RSDT1.outSTAT0,_PGRES,0);		//znacznik petli wylaczenia awaryjnego
	BitSet2(&RSDT1.outSTAT0,_PGPRG,0);	
	eeprom_write(_ADRERR,0x00);
	StartRTS(_RTROZ);
	while(RTS(_RTROZ)<1)
	{
 		CLRWDT();
		ReceptDataLCD();
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

