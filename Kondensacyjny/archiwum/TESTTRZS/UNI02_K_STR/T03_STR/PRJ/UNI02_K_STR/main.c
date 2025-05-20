/*main.c*/  
/*Oprogramowanie rodziny sterownikow T03 (na bazie p³ytki UNI-02)*/
/*Wersja z modulacja PWM pompy, automat grzewczy*/
/*wprowadzony parametr P18, wybieg na powyzszonej mocy minimalnej , mozliwosc przerwania procedury kalibracji*/
//UWAGA:
    //Odbior produkcyjny (aktywny przy aktywnym interfejsie LIN):
    //M.STRB=1
    //Deaktywacja procedury odpowietrzj?cej (zachowany wstepny wybieg pompy po zalaczeniu zasilania)
    //Deaktywacja wybiegu na mocy startowej
    //Deaktrywacja przedbiegu pompy
    //Deaktywacja blokady dla zaworu 3DR na obieg CO w przypadku kotlow 1F przy TCW>85C
    //M.STRB=2
    //Jak dla PFN.STRB=1 tylko bez procedury kalibracji wentylatora i bez wstepnego wybiegu pompy po wlaczeniu zasilania. Ignorowane s± polecenia interfejsu OT

#include "../../proj.tp.h"

#ifndef _TYP_PRJ
#error NIEOKRESLONY IDENTYFIKATOR PROJEKTU
#endif

#if _TYP_PRJ==1
#advisory _TYP_PRJ==1 
#include "../../mcc_generated_files\mcc.h"  

#include "global.h"
#include "main.h"
#include "global.ex.h"

#include "wersja.h"

extern DtPMP PMP;			//pompa.c
extern DtPWM PWM;			//pwm.c
extern DataCO DtCO;			//pomiar.c
extern DataCW DtCW;
extern DataZW DtZW;
extern DataPFN PFN;
extern DataCSN DtCS;
extern DataIN DtIN;
extern DtKONF DtKNF;
extern KALVnt Kvnt[];
extern DtRgl rgl;
extern MGSRSData MGSD;
extern tpRSDTA1 RSDT1;
extern tpMAIN M;

unsigned char x,kll,hll,gll,bll;//,HBL,HRF;
extern unsigned char xxy,zzz,mmm,vvv;
unsigned char vvvv, vup, vdw;
unsigned int wn,vv;
unsigned int pmppwm;
#if _SRVTRB==1						//aktywny tryb serwisowy kompilacji?
extern unsigned char mypwm;
#endif

//-------------------------------------------------------------------------------
static bool inline InGetBit(volatile unsigned char data, const unsigned char nbit)
{
	return ((data&(1<<nbit))?1:0);    
}
//-------------------------------------------------------------------------------
static void inline InBitSet(volatile unsigned char *data, const unsigned char nbit,const unsigned char bt)
{
	*data=(bt)?(*data|(1<<nbit)):(*data&(~(1<<nbit)));
}
//-------------------------------------------------------------------------------
void main_prc(void)
{
//------------------
//------------------
    InitPrcISR();
    InitDtaI2C();
    InitDtaUART1();
    InitDtaPort();  
    InitDtaPWM();
    InitReceptProc();
    InitDataPmr();
    
	ClearData();
    InBitSet(&RSDT1.outSTAT0,_PGPRG,0);                              //ustaw znacznik inicjalizacji programu
    InBitSet(&RSDT1.outSTAT0,_PGRES,0);
    RefBHist(1);  
    RefBStat(1);        
//------------------
//------------------
	if (!PCON0bits.POR||!PCON0bits.BOR)							//zalaczono zasilanie?
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
	if (!PCON0bits.POR||!PCON0bits.BOR)							//zalaczono zasilanie? 
	{	
		PCON0bits.POR=1;
		PCON0bits.BOR=1;
		//EnKonf();		//aktywacja oczekiwania na wejscie do konfiguracji
	}
//------------------
//------------------
	//ustaw znacznik aktualnego obiegu
	_POMPA_SetLow();
	switch(DtKNF.tpkt)		//dla danego rodzaju kotla
	{
		case _UNICO:
		case _MONOHB:
		{
			M._RPM_CW=!_KCO_CW_GetValue();
			break;
		}
		default:			//domyslny monotermiczny
		{
			M._RPM_CW=!_KCO_CW_GetValue();
			break;
		}
	}	
//------------------

//------------------
	//Sprawd¿ czy nastapilo wylaczenie awaryjne z blokada
	if(M.ERR_BTY=ReadEEPROM(_ADRERR)) 		//uruchomienie po wylaczeniu awaryjnym?
	{
		if(DtKNF.robg==_OTWARTY) 			//obieg otwarty?
		{
			if(M.ERR_BTY==0x05)					//dozwolony kod bledu?
			{
				PrintErr(M.ERR_BTY,1);			//wyswietl kod bledu
				ErrPTG();						//przejdz do petli wylaczenia awaryjnego
			}
		}
		if(((M.ERR_BTY>=0x01)&&(M.ERR_BTY<=0x03))||(M.ERR_BTY==0x07)||(M.ERR_BTY==0x0D))	//dozwolony kod bledu?
		{
			PrintErr(M.ERR_BTY,1);			//wyswietl kod bledu
			ErrPTG();						//przejdz do petli wylaczenia awaryjnego
		}
		else M.ERR_BTY=0;
	}
//------------------
//------------------	
	//Pomiar wartosci nastaw dla unikniecia migajacej nastawy po zal. zasilania
	PomiarCO();				//wartosc temperatury CO
	PomiarCW();				//wartosc temperatury CW
	PomiarZW();				//temperatura z czujnika zewnetrznego
	PomiarCS();				//wartosc cisnienia w instalacji CO
	PomiarPCO();			//wartosc nastawy CO
	PomiarPCW();			//wartosc nastawy CW
	PomiarPFN();			//stan przelacznika funkcji
	PomiarECO();			//wartosc wspolczynnika ECO
	PomiarIN();				//wartosc temperatury na powrocie
	DtCO._newPCO=0;			//deaktywacja wyswietlania nastawy CO
	DtCW._newPCW=0;			//deaktywacja wyswietlania nastawy CW

	//deaktywacja pobudzen
	_VNT_SetLow();					//wentylator na stale wylaczony
	_GRZEJ_SetLow();
#if _HIRESVNT==0            
	WritePWM3(0);
#else
	WritePWM3H(0);            
#endif
				
	RestartFltPrt();
	StartRTS(_RTSTR);						//start timera rozruchu sterownika						
//------------------
//------------------
    //RefBHist(1);    
//------------------
	DtCW.PCW=65;
	DtCO.PCO=85;
//------------------
    
//------------------
	WritePWM2(0);
	if(M._PW_START||ReadEEPROM(_ADRKRES))
    //if(1)    
	{
        if_WriteEEPROM(_ADRKRES,0);        //zeruj flage wymuszonej kalibracji wentylatora po programowym restarcie oprogramowania
 		KalibracjaWentylatora();
	}
	else 
	{
		OdczytDanychKalibracji();
	}
 

    //if_WriteEEPROM(_ADRKRES,0);        //zeruj flage wymuszonej kalibracji wentylatora po programowym restarcie oprogramowania
 	//KalibracjaWentylatora();  
    //OdczytDanychKalibracji();

//------------------
    //ErrPTG();						//przejdz do petli wylaczenia awaryjnego
/*
    kll=0;
    while(1)					//petla glowna programu				
	{
		CLRWDT();

        _VNT_SetHigh();					//wentylator na stale wlaczony

		InBitSet(&RSDT1.outSTAT0,_PGPRG,1);		//znacznik petli programu
		InBitSet(&RSDT1.outSTAT0,_PGRES,0);
		M._PRACA=(M._ROZRUCH||_GRZEJ_GetValue());
	//----------------
		ReceptDataLCD();
        RefLCDData();			//aktualizacja danych wysylanych do modulu LCD 
       
        //WritePWM3H(0x00af); 
#if _HIRESVNT==0        
        WritePWM3(ProcToMod(0));
#else
        if(InGetBit(RSDT1.inSTAT1,_KPLUS))
        {
            kll=1;   
        }
        if(InGetBit(RSDT1.inSTAT1,_KMINUS))
        {
            kll=0;
        }
        if(kll)
        {
            WritePWM3H(ProcToMod(99));    
        }
        else
        {
            WritePWM3H(ProcToMod(0));
        }        
#endif

        PomiarVNT();			//predkosc obrotowa wentylatora
		PomiarCO();				//wartosc temperatury CO
		PomiarCW();				//wartosc temperatury CW
		PomiarZW();				//temperatura z czujnika zewnetrznego
		FiltrPomiarowyCOCW();	//filtr danych opisujacych stan temperatur
		PomiarCS();				//wartosc cisnienia w instalacji CO
		PomiarIN();				//wartosc temperatury na powrocie
        
    }
*/
//------------------
#if _PWRKNF==1
	if (M._PW_START)		//zalaczono zasilanie? 
	{	
		EnKonf();				//aktywacja oczekiwania na wejscie do konfiguracji
	}
#endif
//----------------------------
//----------------------------
//----------------------------
	while(1)					//petla glowna programu				
	{
		CLRWDT();
		InBitSet(&RSDT1.outSTAT0,_PGPRG,1);		//znacznik petli programu
		InBitSet(&RSDT1.outSTAT0,_PGRES,0);
		M._PRACA=(M._ROZRUCH||_GRZEJ_GetValue());
	//----------------
		ReceptDataLCD();
		MKonfiguracja();		//sterowanie znacznikiem mozliwosci wejscia do proc. konfiguracji   
        /*if(InGetBit(RSDT1.inSTAT1,_KSET))  //wymuszenie resetu plytki sterujacej przyciskiem SET
        {
            while(1);
        }*/
	//----------------
		PomiarVNT();			//predkosc obrotowa wentylatora
		PomiarPCO();			//wartosc nastawy CO
		PomiarPCW();			//wartosc nastawy CW
		PomiarPFN();			//stan przelacznika funkcji
		PomiarECO();			//wartosc wspolczynnika ECO
		PomiarCO();				//wartosc temperatury CO
		PomiarCW();				//wartosc temperatury CW
		PomiarZW();				//temperatura z czujnika zewnetrznego
		FiltrPomiarowyCOCW();	//filtr danych opisujacych stan temperatur
		PomiarCS();				//wartosc cisnienia w instalacji CO
		PomiarIN();				//wartosc temperatury na powrocie
	//----------------
		DecERR();				//stany awaryjne, wyswietlane kody awaryjne
		DecSTR();				//dodatkowe decyzje sterowania
		DecRozruch();			//decyzja o zapaleniu palnika
		DecWylacz();			//decyzja o wygaszeniu palnika
		FunkcjaSerwisowa();		//decyzja o realizacji funkcji serwisowej
		DecyzjaAntyLeg();		//decyzja o realizacji funkcji ANTYLEGIONELLA
		KolejkaPobudzen();		//aktywacja pobudzen wg priorytetu	
		KontrolaWentylatora();	//kontrola poprawnosci pracy wentylatora
		RefLCDData();			//aktualizacja danych wysylanych do modulu LCD
        RefBHist(0);            //aktualizacja bufora zdarzen
	//----------------
		RglPmp();							//dla pompy PWM reguluj predkosc obrotowa
		if(M._ZMKCOCW) ZmienKCOKCW();		//zmiana kierunku obrotu pompy
        if(M._LOW_PMP) SetLowPMP();         //oobnizona predkosc wybiegu pompy na czas przelaczenia zaworu 3DR
		if(M._WYLACZ) Wylacz();				//kontrola wybiegu pompy
		if(M._ROZRUCH) Rozruch();			//rozruch kotla
		if(M._VWYBIEG) VWybieg();			//wybieg wentylatora
		ReloadPlomien();					//decyzja o wyzerowaniu licznika prob przy braku plomienia
		if(M._RUSZ_PLOMIEN) RuszPlomien();	//awaryjna procedura przy braku plomienia
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
		DecyzjaOdp();
		if(M._BLOKUJ_ODP) Odpowietrzenie();			//oczekiwanie na powrot cisnienia (blokada) i cykliczny wybieg pompy celem odpowietrzenia obiegu CO/CW
	}
}
//------------------
//------------------
void PomALL(void)
{
    PomiarVNT();			//predkosc obrotowa wentylatora
    PomiarPCO();			//wartosc nastawy CO
    PomiarPCW();			//wartosc nastawy CW
    PomiarPFN();			//stan przelacznika funkcji
    PomiarECO();			//wartosc wspolczynnika ECO
    PomiarCO();				//wartosc temperatury CO
    PomiarCW();				//wartosc temperatury CW
    PomiarZW();				//temperatura z czujnika zewnetrznego
    FiltrPomiarowyCOCW();	//filtr danych opisujacych stan temperatur
    PomiarCS();				//wartosc cisnienia w instalacji CO
    PomiarIN();				//wartosc temperatury na powrocie
}
//------------------
//------------------
//Aktualizacja nastaw w plytce wyswietlacza
/*
void InitPFN(const unsigned char pos)
{
	PFN.poz=pos;		//przywroc tryb pracy
	SetPFN();
	WriteEEPFN();
	if(MGSD._CON)
	{
		InBitSet(&RSDT1.outSTAT0,_PGPRG,0);		//aktywacja procedury inicjalizacji danych
		InBitSet(&RSDT1.outSTAT0,_PGRES,0);
		while(!InGetBit(RSDT1.inSTAT0,_INITDTA)&&MGSD._CON)	//procedura jeszcze nieaktywna?
		{
			CLRWDT();
			RefLCDData();			//aktualizacja danych wysylanych do modulu LCD
			ReceptDataLCD();		//aktualizacja danych pobieranych z modulu LCD
	
		}
		InBitSet(&RSDT1.outSTAT0,_PGPRG,1);		//deaktywacja procedury inicjalizacji danych (w tym momencie nastepuje aktualizacja danej)
		InBitSet(&RSDT1.outSTAT0,_PGRES,0);
		while(InGetBit(RSDT1.inSTAT0,_INITDTA)&&MGSD._CON)	//procedura jeszcze aktywna?
		{
			CLRWDT();
			RefLCDData();			//aktualizacja danych wysylanych do modulu LCD
			ReceptDataLCD();		//aktualizacja danych pobieranych z modulu LCD
	
		}
	}
}
 */
//Procedura kalibracji wentylatora
void KalibracjaWentylatora(void)
{
	volatile unsigned char st;
	//volatile static unsigned char poz;

	M._PO_KAL=0;
	_VNT_SetLow();						//wentylator na stale wylaczony
    
#if _HIRESVNT==0            
			WritePWM3(0);
#else
			WritePWM3H(0);            
#endif
	//PomiarVNT();				//predkosc obrotowa wentylatora
	M.R48=0;
	M._RUSZ_48=0;
	while(!(st=KalibracjaVNT()))
	{        
 		CLRWDT();
        if(M.STRB!=2)                //tryb produkcyjny?
        {
            if(RTS(_RTSTR)>9&&RTS(_RTSTR)<11)
            {            
                ToObiegCO();		//przejdz na obieg CO
                _POMPA_SetHigh();			//wlacz wybieg pompy
                M._WYLACZ=1;
                PMP.APROC=99;
                WritePWM2(ProcToPmpPwm(PMP.APROC));
            }
            if(M._ZMKCOCW) ZmienKCOKCW();
            if(M._WYLACZ) Wylacz();				//kontrola wybiegu pompy
        }
		//***************************************************
		InBitSet(&RSDT1.outSTAT0,_PGPRG,1);		//znacznik petli programu
		InBitSet(&RSDT1.outSTAT0,_PGRES,0);
		PomiarCO();				//wartosc temperatury CO
		PomiarCW();				//wartosc temperatury CW
		PomiarZW();				//temperatura z czujnika zewnetrznego
		PomiarCS();				//wartosc cisnienia w instalacji CO
		PomiarPCO();			//wartosc nastawy CO
		PomiarPCW();			//wartosc nastawy CW
		PomiarPFN();			//stan przelacznika funkcji
		PomiarECO();			//wartosc wspolczynnika ECO
		PomiarIN();				//wartosc temperatury na powrocie
        
        SetTrbPGD();//************************
        
		if(_VNT_GetValue()) PomiarVNT();	//predkosc obrotowa wentylatora
        else ClrTachoDta();
		ReceptDataLCD();		//aktualizacja danych pobieranych z modulu LCD
		RefLCDData();			//aktualizacja danych wysylanych do modulu LCD        
	};
	
	if((x<2))	//procedura kalibracji nie zostala przerwana przyciskiem minus?
	{
		if((!MGSD._CON)||(MGSD._CON&&(RSDT1.vSTD!=0)))
		{
            if((M.STRB==1)||(M.STRB==2))                //tryb produkcyjny?
            {
                M._BLOKUJ_ODP=0;
                M._WYM_ODP=0;                
            }
            else
            {
                M._BLOKUJ_ODP=1;						//aktywuj blokade odpowietrzenia
                M._WYM_ODP=1;							//procedura bezwarunkowa
            }
		}
		else
		{
			M._BLOKUJ_ODP=0;
			M._WYM_ODP=0;
		}
	}
	StartRTS(_RTMPMP);
	M._PO_KAL=1;
    M._KNF_KAL=0;
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
	M._RUSZ_PLOMIEN=0;
	M._RUSZ_RPLM=0;
	M._RUSZ_POMPE=0;
	M._BLOKUJ_3M=0;
	M._BLOKUJ_15S=0;
	M._BLOKUJ_ODP=0;
	M._PROC_ODP=0;
	M._WYM_ODP=0;
	M._MODULUJ=0;

	M._AKTYWNE_AL=0;
	M._AKTYWNE_SRW=0;
	M._AKTYWNE_PGD=0;
	M._AKTYWNE_PGD2=0;    
	M._AKTYWNE_KNF=0;
	M._RUSZ_NTC_CO=0;
	M._RUSZ_NTC_CW=0;
	M._RUSZ_STB=0;
	M._RUSZ_CISN=0;
	M._RUSZ_NCISN=0;

	M._PW_START=0;
	M._RS_START=0;		
	M._PRACA=0;
	M._OST_PL=0;
	M._RUN_MNMOD=0;
	M._KCW_PMP=0;
	M._DE_VNT=0;
	M._DE_PMP=0;
	M._RPM_CW=0;
	M._RUSZ_NTC_IN=0;
    
    M._SYNCH_PGD=0;    
    M._LOW_PMP=0;

	M._LCD=0;
	M._LIN=0;
	M._LIN_RT=0;
	M._LIN_CW=0;
	M._HWB_CW=0;

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
	M.LICZNIK_PL2=0;
	M.LICZNIK_ODP=0;
	M.DPR=0;
	M.VVNT=0;
	M.rVVNT=0;    
	M.ODP=0;
	M.STRB=0;
    
	_VNT_SetLow();
	_GRZEJ_SetLow();
	_POMPA_SetLow();
	ClrDtV();
    ClrTachoDta();
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
}
//------------------
//------------------
unsigned char KodSterownika(void)
{
	//Kod sterownika
	switch(DtKNF.tpkt)		//dla danego rodzaju kotla
	{
		case _UNICO:		//unico - monotermiczny
			return 0x11;
		case _MONOHB:		//monotermiczny z hydroblokiem
			return 0x17;
		default:			//domyslny monotermiczny z hydroblokiem
			return 0x17;
	}
}
//------------------
//------------------
//Aktualizacja danych do wysylki
void RefLCDData(void)
{
    volatile unsigned char wr;
    
	RSDT1.outRDZ0=(KodSterownika()&0b00011111)|((DtKNF.alin&0b00000111)<<5);         //kod sterownika i adres LIN plutki UNI-02 RSDT1.outRDZ0=KodSterownika()
#if _WERSJA<=0x0F
        RSDT1.outRDZ1=(DtKNF.tpkt&0x0F)|((_WERSJA&0x0F)<<4);                            //typ kotla i wersja oprogramowania plytki UNI-02
        wr=RSDT1.outRDZ2&0b11100000;
        RSDT1.outRDZ2=wr;
#else
        RSDT1.outRDZ1=(DtKNF.tpkt&0x0F)|0xF0;                            //typ kotla i wersja oprogramowania plytki UNI-02   
        wr=RSDT1.outRDZ2&0b11100000;
        RSDT1.outRDZ2=wr|((_WERSJA-0x0F)&0b00011111);
#endif
	InBitSet(&RSDT1.outSTAT0,_PLMCO,M._PRACA&&!M._RPM_CW&&RdPrt(S_PLM));		//plomien CO
	InBitSet(&RSDT1.outSTAT0,_PLMCW,M._PRACA&&M._RPM_CW&&RdPrt(S_PLM));		//plomien CW
	InBitSet(&RSDT1.outSTAT0,_FKSRV,M._AKTYWNE_SRW);							//sygnalizuj realizacje funkcji serwisowej
	InBitSet(&RSDT1.outSTAT0,_ANTYL,M._AKTYWNE_AL);							//sygnalizuj realizacje funkcji antylegionella
	InBitSet(&RSDT1.outSTAT0,_KNFPRC,M._AKTYWNE_KNF);						//sygnalizuj realizacje funkcji konfiguracji
	InBitSet(&RSDT1.outSTAT0,_ENKNF,DtKNF.enknf);							//zezwolenie na wejscie do konfiguracji

	InBitSet(&RSDT1.outSTAT1,_TdzCW,M._pob_DzCW);							//temp. dyzurna CO
	InBitSet(&RSDT1.outSTAT1,_TdzCO,M._pob_DzCO);							//temp. dyzurna CW
	InBitSet(&RSDT1.outSTAT1,_PGD,M._AKTYWNE_PGD);							//aktywna funkcja pogodowa

	InBitSet(&RSDT1.outSTAT1,_OBGCO,M._PRACA&&!M._RPM_CW);					//aktywny obieg CO
	InBitSet(&RSDT1.outSTAT1,_OBGCW,M._PRACA&&M._RPM_CW);					//aktywny obieg CW

    if(RSDT1.outSTAT1)
    {
        Nop();
        Nop();
    }    

	InBitSet(&RSDT1.outSTAT2,_ErrCO,RdPrt(S_ERR_CO));							//awaria czujnika CO
	InBitSet(&RSDT1.outSTAT2,_ErrCW,RdPrt(S_ERR_CW));							//awaria czujnika CW
	InBitSet(&RSDT1.outSTAT2,_ErrCS,DtCS._error);							//awaria czujnika CS
	InBitSet(&RSDT1.outSTAT2,_CSnmt,DtCS._no_mont);							//brak czujnika CS

	InBitSet(&RSDT1.outSTAT3,_ZCFM,(_KNF_GetValue())?0:1);								//stan zworki konfiguracyjnej
	InBitSet(&RSDT1.outSTAT3,_VNT_ON,_VNT_GetValue());									//stan pracy wentylatora (0-postoj, 1-paraca)
	InBitSet(&RSDT1.outSTAT3,_ROZ,M._ROZRUCH);								//znacznik aktywnej procedury rozruchu
	InBitSet(&RSDT1.outSTAT3,_ErrIN,RdPrt(S_ERR_IN));							//awaria czujnika IN
	InBitSet(&RSDT1.outSTAT3,_PMP_PWM,(DtKNF.rdpm!=0)?1:0);					//pompa PWM
	InBitSet(&RSDT1.outSTAT3,_TRB_ECO,((DtKNF.teco!=0)&&(DtKNF.robg!=0))?1:0);	//tryb pracy ECO
    
	if(DtKNF.tpkt==_UNICO)				//kociol UniCO?
    {
        if(!DtKNF.cwtm)     //(timera zasobnika||LIN_CW||OT_CW)
        {
            InBitSet(&RSDT1.outSTAT3,_TIM_ZAS,(RdPrt(S_WCW)!=0)?1:0);				//(timera zasobnika||LIN_CW||OT_CW)
        }
        else                //timer zasobnika
        {
            InBitSet(&RSDT1.outSTAT3,_TIM_ZAS,(RdPrt(S_TIM)!=0)?1:0);				//timera zasobnika  
        }
    }
    else
    {
        InBitSet(&RSDT1.outSTAT3,_TIM_ZAS,(RdPrt(S_WCW)!=0)?1:0);				//pobudzenie CW   
    }
    
	InBitSet(&RSDT1.outSTAT3,_POB_RT,(RdPrt(S_RT)!=0)?1:0);					//pobudzenie RT

	InBitSet(&RSDT1.outSTAT4,_PR_ODP,M._PROC_ODP);							//aktywacja blokady na odpowietrzenie ukladu
    if(PFN.pop_poz==_PFLTO)
    {
        InBitSet(&RSDT1.outSTAT4,_POP_FN,1);                                 //ostatnia pozycja PFN przed przejsciem na pozycje OFF (0-zima,1-lato)   
    }
    else
    {
        InBitSet(&RSDT1.outSTAT4,_POP_FN,0);                                 //ostatnia pozycja PFN przed przejsciem na pozycje OFF (0-zima,1-lato)          
    }
    InBitSet(&RSDT1.outSTAT4,_TMP_ZW,(RdPrt(S_EN_ZW)!=0)?1:0);                  //znacznik obecnosci czujnika temp zew.
    InBitSet(&RSDT1.outSTAT4,_PGD_TP,(DtKNF.pgdr==2)?1:0);                   //rodzaj regulatora pogodowego (0-tradycyjny, 1-rozszerzony)
    InBitSet(&RSDT1.outSTAT4,_PMP_ON,_POMPA_GetValue());                                //stan pracy pompy (0-postoj, 1-paraca)
    InBitSet(&RSDT1.outSTAT4,_KCOCW,_KCO_CW_GetValue());                                //zawor 3d (0-obieg CW, 1-obieg CO)   
    InBitSet(&RSDT1.outSTAT4,_PRZ_CO2,(RdPrt(S_PRZ_CO)!=0)?1:0);             //zawor 3d (0-obieg CW, 1-obieg CO)    
    InBitSet(&RSDT1.outSTAT4,_GRZ_ON,_GRZEJ_GetValue());                                //pobudzenie modu³u kontroli plomienia (0-postoj, 1-paraca)
    
    InBitSet(&RSDT1.outSTAT5,_STK_RT2,(_RT_GetValue())?1:0);                            //styk wejscia pobudzenia RT/obni¿enie nocne (0-rozwarty, 1-zwarty)
    InBitSet(&RSDT1.outSTAT5,_STK_CW2,(_PRZ_CW_GetValue())?0:1);                        //styk wejscia pobudzenia CW/timer zasobnika (0-rozwarty, 1-zwarty)
    InBitSet(&RSDT1.outSTAT5,_CIG_NO2,(RdPrt(S_CIG_NO)!=0)?1:0);             //styk czujnika ciagu NO (0-rozwarty, 1-zwarty)
    InBitSet(&RSDT1.outSTAT5,_CIG_NC2,(RdPrt(S_CIG_NC)!=0)?1:0);             //styk czujnika ciagu NC (0-rozwarty, 1-zwarty)            
    InBitSet(&RSDT1.outSTAT5,_STK_STB,(RdPrt(S_STB)!=0)?1:0);                //styk czujnika STB NC (0-rozwarty, 1-zwarty)    
    
    
	InBitSet(&RSDT1.outKSTAT0,_ZCFM,(_KNF_GetValue())?0:1);								//stan zworki konfiguracyjnej
	InBitSet(&RSDT1.outKSTAT0,_KC1F,(DtKNF.tpkt==_UNICO)?1:0);				//znacznik rodzaju kotla (1=jednofunkcyjny)
	RSDT1.outPP=PMP.APROC;													//procentowy wydatek pompy
	RSDT1.outPG=VNTToProc(M.VVNT);//PWM.APROC;								//procentowy wydatek modulatora
    //------------------------------------------
    M.vWSW=RSDT1.inRDZ0;                                                    //wersja plytki wyswietlacza
    if(InGetBit(RSDT1.inSTAT1,_7KEY))
    {
        M._KEY7=1;                                                          //znacznik konsoli 7key
        if(InGetBit(RSDT1.inSTAT1,_KSET))                                    //nacisnieto przycisk funkcji specjalnych?
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
    //------------------------------------------
    
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
    M._I10V=InGetBit(RSDT1.inINSTAT,_F10V)&&M._LCD;   						//ustaw znacznik aktywnego interfejsi 10V
    M._I10V_RT=InGetBit(RSDT1.inINSTAT,_10V_RT)&&M._I10V;
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
    M._LIN=InGetBit(RSDT1.inLNSTAT,_FLIN)&&M._LCD;   						//ustaw znacznik aktywnej komunikacji LIN
    M._LIN_RT=InGetBit(RSDT1.inLNSTAT,_LN_RT)&&M._LIN;   					//ustaw pobudzenie RT z interfejsu LIN
    M._LIN_CW=InGetBit(RSDT1.inLNSTAT,_LN_CW)&&(DtKNF.tpkt==_UNICO)&&M._LIN; //ustaw pobudzenie CW z interfejsu LIN
    M._LIN_KSK=InGetBit(RSDT1.inLNSTAT,_LN_KSK)&&M._LIN;                     //znacznik pracy w trybie kaskady
    M._LIN_STR=InGetBit(RSDT1.inLNSTAT,_LN_STR)&&M._LIN;                     //znacznik pracy w trybie sterowania z managera stref lub regulatora RT
    M._LIN_DPGD=InGetBit(RSDT1.inLNSTAT,_LN_DPGD)&&M._LIN;                   //znacznik dezaktywacji wewnetrznego regulatora pogodowego 
    M._LIN_ENDO=InGetBit(RSDT1.inSTAT1,_END_ODP)&&M._LIN&&M._BLOKUJ_ODP;     //zdalna deaktywacja procedury odpowietrzajacej
    if(!M._LIN_KSK&&!M._LIN_STR)
    {
        M._I10V=InGetBit(RSDT1.inINSTAT,_F10V)&&M._LCD;   						//ustaw znacznik aktywnego interfejsi 10V
        M._I10V_RT=InGetBit(RSDT1.inINSTAT,_10V_RT)&&M._I10V;     
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
    M._LIN=InGetBit(RSDT1.inLNSTAT,_FLIN)&&M._LCD;   						//ustaw znacznik aktywnej komunikacji LIN
    M._LIN_RT=InGetBit(RSDT1.inLNSTAT,_LN_RT)&&M._LIN;   					//ustaw pobudzenie RT z interfejsu LIN
    M._LIN_CW=InGetBit(RSDT1.inLNSTAT,_LN_CW)&&(DtKNF.tpkt==_UNICO)&&M._LIN; //ustaw pobudzenie CW z interfejsu LIN
    M._LIN_KSK=InGetBit(RSDT1.inLNSTAT,_LN_KSK)&&M._LIN;                     //znacznik pracy w trybie kaskady
    M._LIN_STR=InGetBit(RSDT1.inLNSTAT,_LN_STR)&&M._LIN;                     //znacznik pracy w trybie sterowania z managera stref lub regulatora RT 
    M._LIN_DPGD=InGetBit(RSDT1.inLNSTAT,_LN_DPGD)&&M._LIN;                   //znacznik dezaktywacji wewnetrznego regulatora pogodowego 
    
    if(!M._LIN_DPGD)
    {
        Nop();
        Nop();
    }
    M._LIN_ENDO=InGetBit(RSDT1.inSTAT1,_END_ODP)&&M._LIN&&M._BLOKUJ_ODP;     //zdalna deaktywacja procedury odpowietrzajacej    
    if(!M._LIN_KSK&&!M._LIN_STR)    
    {
        M._IOT=InGetBit(RSDT1.inINSTAT,_FOT)&&M._LCD;                            //ustaw znacznik aktywnego interfejsi OT
        M._IOT_RT=InGetBit(RSDT1.inINSTAT,_OT_RT)&&M._IOT;                       //ustaw pobudzenie RT z interfejsu OT
        M._IOT_CW=InGetBit(RSDT1.inINSTAT,_OT_CW)&&(DtKNF.tpkt==_UNICO)&&M._IOT; //ustaw pobudzenie CW z interfejsu OT  
        M._IOT_STR=InGetBit(RSDT1.inINSTAT,_OT_STR)&&M._IOT;                     //znacznik pracy w trybie sterowania z managera stref lub regulatora RT  
        M._IOT_DPGD=InGetBit(RSDT1.inINSTAT,_OT_DPGD)&&M._IOT;                     //znacznik pracy w trybie sterowania z managera stref lub regulatora RT
        if(!M._IOT)
        {
            M._I10V=InGetBit(RSDT1.inINSTAT,_F10V)&&M._LCD;   						//ustaw znacznik aktywnego interfejsi 10V
            M._I10V_RT=InGetBit(RSDT1.inINSTAT,_10V_RT)&&M._I10V;                 
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
				if(_AL_PCW>DtCW.CW) M._pob_AL=1;	//zalacz wymuszenie grzania
				else 
				{
					M._pob_AL=0;					//wylacz wymuszenie grzania
					M._AKTYWNE_AL=0;				//deaktywuj funkcje ANTYLEGIONELLA
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
			//if(!InGetBit(RSDT1.inSTAT0,_INITANL))
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
						if((PFN.poz==_PFLTO)||(PFN.poz==_PFZMA)) M._AKTYWNE_AL=1;		//aktywacja funkcji ANTYLEGIONELLA po czasie odliczonym przez timer.
						StartRTM(_RTMAL);					//start timera maksymalnego dozwolonego czasu aktywnosci funkcji ANTYLEGIONELLA
						if_WriteEEPROM(_ADRRTA,0);
						return;	
					}
					else
					{
						if_WriteEEPROM(_ADRRTA,M.LICZNIK_ANL);
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
			if(InGetBit(RSDT1.inSTAT0,_INITANL))			//aktywowano recznie funkcje ANTYLEGIONELLA?
			{
				M.LICZNIK_ANL=0;
				M._AKTYWNE_AL=1;			//aktywacja funkcji ANTYLEGIONELLA
				StartRTM(_RTMAL);			//start timera maksymalnego dozwolonego czasu aktywnosci funkcji ANTYLEGIONELLA
				if_WriteEEPROM(_ADRRTA,0);
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
			//if(!InGetBit(RSDT1.inSTAT0,_INITANL))
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
						if((PFN.poz==_PFLTO)||(PFN.poz==_PFZMA)) M._AKTYWNE_AL=1;		//aktywacja funkcji ANTYLEGIONELLA po czasie odliczonym przez timer.
						StartRTM(_RTMAL);					//start timera maksymalnego dozwolonego czasu aktywnosci funkcji ANTYLEGIONELLA
						if_WriteEEPROM(_ADRRTA,0);
						return;	
					}
					else
					{
						if_WriteEEPROM(_ADRRTA,M.LICZNIK_ANL);
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
			if(InGetBit(RSDT1.inSTAT0,_INITANL))			//aktywowano recznie funkcje ANTYLEGIONELLA?
			{
				M.LICZNIK_ANL=0;
				M._AKTYWNE_AL=1;			//aktywacja funkcji ANTYLEGIONELLA
				StartRTM(_RTMAL);			//start timera maksymalnego dozwolonego czasu aktywnosci funkcji ANTYLEGIONELLA
				if_WriteEEPROM(_ADRRTA,0);
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

//------------------
//------------------
//Reakcja na wiekszosc stanow awaryjnych 
void DecERR(void)
{
	unsigned char KnfDisErr=0;
	unsigned char w;
	
//Znacznik wylaczajacy wizualizacje kodu bledu podczas pracy w proc. konfiguracji

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
//zadzialal czujnik ciagu
	if(!RdPrt(S_CIG_NC))
	{
		M.ERR_BTY=0x03;
		PrintErr(0x03,1);
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
	}
	else 
	//awaria czujnika CO (wersja dla kotla mono i bitermicznego)
	if(RdPrt(S_ERR_CO))
	{
		M.ERR_BTY=0x04;
		if(KnfDisErr) EndErr();		//nie wyswietlaj kodu bledu przy konfiguracji par. r4 i r6, r7
		else PrintErr(0x04,1);
		M._RUSZ_NTC_CO=1;
		M._RUSZ_CISN=0;
		M._RUSZ_NCISN=0;
		M._RUSZ_NTC_CW=0;
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
				}
				else
				{
					M._RUSZ_CISN=0;
                    if(DtKNF.tpkt!=_UNICO)
					{
						w=(RdPrt(S_ERR_CW)&&(PFN.poz==_PFLTO))||((PFN.poz==_PFZMA)&&RdPrt(S_ERR_CW)&&(M._pob_CW||M._pob_KCW||M._pob_AL));
					}
					else
					{
                        if(!DtKNF.cwtm)     //(timera zasobnika||LIN_CW||OT_CW)
                        {
                            w=(RdPrt(S_ERR_CW)&&(PFN.poz==_PFLTO))||((PFN.poz==_PFZMA)&&RdPrt(S_ERR_CW)&&RdPrt(S_WCW));
                        }
                        else                //timera zasobnika
                        {
                            w=(RdPrt(S_ERR_CW)&&(PFN.poz==_PFLTO))||((PFN.poz==_PFZMA)&&RdPrt(S_ERR_CW)&&RdPrt(S_TIM));
                        }
					}                    
					/*else
					{
                        if(!DtKNF.cwtm)     //(timera zasobnika||LIN_CW||OT_CW)
                        {
                            w=(RdPrt(S_ERR_CW)&&(PFN.poz==_PFLTO))||((PFN.poz==_PFZMA)&&RdPrt(S_ERR_CW)&&RdPrt(S_WCW)&&!M._pob_CO&&!M._pob_KCO&&!M._pob_SRW);
                        }
                        else                //timera zasobnika
                        {
                            w=(RdPrt(S_ERR_CW)&&(PFN.poz==_PFLTO))||((PFN.poz==_PFZMA)&&RdPrt(S_ERR_CW)&&RdPrt(S_TIM)&&!M._pob_CO&&!M._pob_KCO&&!M._pob_SRW);
                        }
					}*/
                    
                   
					/*if(DtKNF.tpkt!=_UNICO)
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
					}*/
					//awaria czujnika CW
					if(w)
					{
						M.ERR_BTY=0x0A;//0x10;
						if(KnfDisErr) EndErr();		//nie wyswietlaj kodu bledu przy konfiguracji par. r4 i r6, r7
						else PrintErr(0x0A,1);
						M._RUSZ_NTC_CW=1;
					}
					else
					{ 
						M._RUSZ_NTC_CW=0;
						if(DtKNF.rdpm!=0)
						{
							if((PFN.poz==_PFZMA)&&(M._PRACA&&!M._RPM_CW)&&RdPrt(S_ERR_IN))	//pozycja ZIMA, grzanie w obiegu CO
							{
								M.ERR_BTY=0x0E;
								if(KnfDisErr) EndErr();		//nie wyswietlaj kodu bledu przy konfiguracji par. r4 i r6, r7
								else PrintErr(0x0E,1);
								M._RUSZ_NTC_IN=1;
							}
							else
							{
								M._RUSZ_NTC_IN=0;
							}
						}
						else
						{
							M._RUSZ_NTC_IN=0;
						}
					}
				}
			}
		}
	}
//--------START PROCEDUR NAPRAWCZYCH - WYLACZENIE Z BLOKADA
//UWAGA: bez wizualizacji kodu awaryjnego (wyswietlany dopiero po wykonaniu procedury naprawczej)

	//brak plomienia (praca kotla)
	if(!M._RUSZ_PLOMIEN&&!RdPrt(S_PLM)&&!M._ROZRUCH&&_GRZEJ_GetValue()) 
	{
		if(RdPrt(S_CIG_NC)&&RdPrt(S_STB))
		{
			M.RPL=0;
			M._RUSZ_PLOMIEN=1;
			M.LICZNIK_PL2++;					//licznik liczby aktywacji procedury E1 w okreslonym odcinku czasu
			if(M.LICZNIK_PL2>=_NCYKLI_PL2)		//przekroczono dozwolona liczbe aktywacji procedury E1?
			{
				M.LICZNIK_PL2=0;
				M.ERR_BTY=0x0D;
				PrintErr(0x0D,1);
                ToWriteESTAT();
				ErrPTG();
			}
		}
	}

//--------REAKCJA NA BRAK SYTUACJI AWARYJNEJ (DOTYCZY WIZUALIZACJI KODU AWARYJNEGO)

	//usun kod awaryjny z wyswietlacza
	if(!M._RUSZ_PLOMIEN)
	if(!M._RUSZ_NTC_CO&&!M._RUSZ_NCISN&&!M._RUSZ_CISN)
	if(!M._RUSZ_NTC_CW&&!M._RUSZ_NTC_IN&&!M._RUSZ_POMPE)
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
	if(!M._ROZRUCH&&_GRZEJ_GetValue()&&DtCO._ldDCO) M._RUN_MNMOD=1;
	else M._RUN_MNMOD=0;
	//--------------
	//wybor obiegu dla pompy 
#if _SOLAR==0
    if((DtKNF.tpkt==_UNICO)&&(DtCW._offKCW==1))     //spelnione warynki dla blokady zaworu 3DR dla obiegu CW? (zapobieganie skokom cisnienia na zaczopowanym zaworze.)
    {
        if(!M._ZMKCOCW)
        {
            if(!M._RUSZ_48Z3&&!M._PROC_ODP) ToObiegCO();
        }
    }
    else //(w stanie spoczynku pompa pracuje w obiegu CW)
#endif
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
            if(!M._PRACA&&M._WYLACZ&&!M._PROC_ODP&&(DtKNF.tpkt==_MONOHB)&&(!RdPrt(S_ERR_CO)&&!DtCO._ptwp_CO)&&M._pob_CW) ToObiegCW();
            else
            if(!M._PRACA&&M._WYLACZ&&!M._PROC_ODP&&(DtKNF.tpkt==_UNICO)&&(!RdPrt(S_ERR_CO)&&!DtCO._ptwp_CO)&&((PFN.poz==_PFLTO)||(PFN.poz==_PFZMA))&&DtCW._wpz_CW) ToObiegCW();
            else
            //temperatura CO poza zakresem lub aktywny wybieg 48h?
            if(!M._PRACA&&!M._BLOKUJ_15S&&!M._PROC_ODP&&(RdPrt(S_ON_PMP)||M._RUSZ_48))
            {
                if(PFN.poz==_PFOFF) ToObiegCO();            //pozycja OFF
                else
                if(PFN.poz==_PFLTO)                         //pozycja LATO
                {
                     if(DtCO._cold_CO) ToObiegCO();
                     else ToObiegCW();  
                }
                else                                        //pozycja ZIMA
                {
                    if(DtKNF.tpkt==_UNICO)
                    {
                        if(!M._WYLACZ) M._HWB_CW=0;
                        if(M._HWB_CW)
                        {
                            //ToObiegCW();                
                            if((!M._pob_KCW && !M._pob_CW && !M._pob_DzCW && !M._pob_AL)&&M._pob_CO) ToObiegCO();                //brak pobudzenia CW, pobudzenie RT, temp CO poza zakresem
                            else ToObiegCW();                            
                        }
                        else
                        { 
                            if(!M._pob_KCW && !M._pob_CW && !M._pob_DzCW && !M._pob_AL) ToObiegCO();                //brak pobudzenia CW
                            else ToObiegCW();
                        } 
                    }
                    else
                    {
                        if(!M._pob_KCW && !M._pob_CW && !M._pob_DzCW && !M._pob_AL) ToObiegCO();                //brak pobudzenia CW
                        else ToObiegCW();
                    }                                                                       //aktywne pobudzenie CW   
                }
            }
            else
            //aktywne RT i temp. CO wyzsza od warunku zalaczenia kotla?	
            if(!M._PRACA&&!M._BLOKUJ_15S&&!M._PROC_ODP&&M._pob_CO&&!DtCO._su_CO) ToObiegCO();
            else	//kociol w spoczynku
            {
                switch(DtKNF.tpkt)		//dla danego rodzaju kotla
                {
                    case _MONOHB:
                    {
                        //nieaktywna pompa?
                        if(!M._PRACA&&!M._WYLACZ&&!M._PROC_ODP&&!M._RUSZ_48&&(!_POMPA_GetValue()||M._RUSZ_48Z3)) ToObiegCW();
                        break;
                    }
                    case _UNICO:
                    {					
                        //b.niska temperatura CW lub nieaktywna pompa?
                        //if(!M._PRACA&&!M._PROC_ODP&&(RdPrt(S_ON_PMP_CW)||(!M._WYLACZ&&!M._RUSZ_48&&!_POMPA_GetValue()&&!M._PROC_ODP))) ToObiegCW();
                        if(!M._PRACA&&!M._PROC_ODP&&(RdPrt(S_ON_PMP_CW)))   //b.niska temperatura CW?
                        {
                            ToObiegCW();    
                        }
                        else        //nieaktywna pompa?
                        {
                            if(!M._WYLACZ&&!M._RUSZ_48&&!_POMPA_GetValue()&&!M._PROC_ODP) ToObiegCO();
                        }
                        break;
                    }
                    default:			//domyslny monotermiczny
                    {
                        //nieaktywna pompa?
                        if(!M._PRACA&&!M._WYLACZ&&!M._PROC_ODP&&!M._RUSZ_48&&!_POMPA_GetValue()) ToObiegCW();
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
			case _MONOHB:
			{
				if(!_POMPA_GetValue()&&!M._WYLACZ&&!M._PROC_ODP&&!M._RUSZ_48&&!M._RUSZ_48Z3&&!M._PRACA)
				{
					//temperatura CO/CW poza dozwolonym zakresem
					if(RdPrt(S_ON_PMP))
					{
						M._WYLACZ=1;				 //uaktywnij wybieg pompy
						_POMPA_SetHigh();
					}
					else
					//decyzja o zlaczeniu pompy dla RT i temp. CO wyzszej od warunku zalaczenia kotla
					if((PFN.poz!=_PFOFF) && (PFN.poz!=_PFLTO) && M._pob_CO&&!DtCO._su_CO)
					{
						M._WYLACZ=1;				//uaktywnij wybieg pompy	
						_POMPA_SetHigh();
					}
				}
				break;
			}
			case _UNICO:
			{
				if(!_POMPA_GetValue()&&!M._WYLACZ&&!M._PROC_ODP&&!M._RUSZ_48&&!M._PRACA)
				{
                    //jest spelniony war temp. pobudzenie CW ale temp. CO jest za wysoka
                    if((DtCW._wpz_CW||M._pob_AL||M._pob_DzCW||M._pob_KCW)&&(PFN.poz!=_PFOFF)&&(!RdPrt(S_ERR_CO)&&!DtCO._ptwp_CO))
                    {
						M._WYLACZ=1;				 //uaktywnij wybieg pompy
						_POMPA_SetHigh();                        
                    }
                    else
					//temperatura CO/CW poza dozwolonym zakresem
					if((RdPrt(S_ON_PMP))||(M._RPM_CW&&RdPrt(S_ON_PMP_CW)))
					{
						M._WYLACZ=1;				 //uaktywnij wybieg pompy
						_POMPA_SetHigh();
					}
					else
					//decyzja o zlaczeniu pompy dla RT i temp. CO wyzszej od warunku zalaczenia kotla
					if((PFN.poz!=_PFOFF) && (PFN.poz!=_PFLTO) && M._pob_CO && !DtCO._su_CO)
					{
						M._WYLACZ=1;				//uaktywnij wybieg pompy	
						_POMPA_SetHigh();
					}
				}
				break;
			}
			default: 		//domyslny monotermiczny
			{
				if(!_POMPA_GetValue()&&!M._WYLACZ&&!M._PROC_ODP&&!M._RUSZ_48&&!M._RUSZ_48Z3&&!M._PRACA)
				{                   
					//temperatura CO/CW poza dozwolonym zakresem
					if(RdPrt(S_ON_PMP))
					{
						M._WYLACZ=1;				 //uaktywnij wybieg pompy
						_POMPA_SetHigh();
					}
					else
					//decyzja o zlaczeniu pompy dla RT i temp. CO wyzszej od warunku zalaczenia kotla
					if((PFN.poz!=_PFOFF) && (PFN.poz!=_PFLTO) && M._pob_CO&&!DtCO._su_CO)
					{
						M._WYLACZ=1;				//uaktywnij wybieg pompy	
						_POMPA_SetHigh();
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
			PWM.MDMOD=PWM.DV16_MDMOD;	//czasowa praca z moca minimalnea = 1600
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
	//Czasowy ogranicznik mocy maksymalnej dla grzania CO
	if(!M._ROZRUCH&&_GRZEJ_GetValue()&&M._pob_CO&&!M._pob_SRW&&!M._AKTYWNE_KNF&&!M._pob_KCO&&!M._pob_KCW&&!M._pob_CW&&!M._pob_DzCW&&!M._pob_AL)
	{
		if(DtKNF.tmxco2&&(RTM(_RTMMAXCO)<DtKNF.tmxco2))
		{
			if(!PWM._TMAXCO)
			{
				DtKNF.mxco=DtKNF.mxco2;		//czasowa praca z moca maksymalna2 zdefiniowana w konfiguracji
				rgl.slow_inc=1;				//aktywuj powolny narost mocy po uplywie czasu pracy z moca maksymalna2
				ModParam();					//oblicz parametry kluczowe modulatora
			}
			PWM._TMAXCO=1;
			//mmm=1;
		}
		else
		{
			if(PWM._TMAXCO)
			{
				DtKNF.mxco=DtKNF.def_mxco;	//praca z domyslna moca maksymalna zdefiniowana w konfiguracji
				ModParam();					//oblicz parametry kluczowe modulatora
			}
			PWM._TMAXCO=0;
			//mmm=0;
		}
	}
	else
	{
		if(PWM._TMAXCO)
		{
			DtKNF.mxco=DtKNF.def_mxco;		//praca z domyslna moca maksymalna zdefiniowana w konfiguracji
			ModParam();						//oblicz parametry kluczowe modulatora
		}
		rgl.slow_inc=0;
		PWM._TMAXCO=0;
		//mmm=0;
	}

	//--------------
	//aktywacja/deaktywacja bloku modulacji plomieniem dla pobudzen RT i CW
	if(M._pob_CO||M._pob_CW||M._pob_AL)
	{
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
#if _HIRESVNT==0  
	//deaktywacja bloku modulacji plomieniem dla awarii modulatora lub dla szybkiego narostu temp.
	if(!M._PRACA||M._RUN_MNMOD||(!M._PRACA&&M._VWYBIEG))
	{
		M._MODULUJ=0;	//wylacz blok modulacji
		if(M._RUN_MNMOD)
		{
			WritePWM3(ProcToMod(0));//WritePWM(PWM.BufPWM=ModToDVNT(DtKNF.mmin,DtKNF.mmin,DtKNF.mmax,PWM.BufPWM));//WritePWM(PWM.MDMOD);		//zbyt szybki narost temp.
		}
		else
		if(M._VWYBIEG)
		{
			WritePWM3(ProcToMod(0));//WritePWM(PWM.BufPWM=ModToDVNT(DtKNF.mmin,DtKNF.mmin,DtKNF.mmax,PWM.BufPWM));//WritePWM(PWM.MDMOD);		//wybieg wentylatora
		}
		else
		if(!M._PRACA)
		{
			WritePWM3(0);
		}
		else WritePWM3(0);
	}
	//wymuszenie konkretnej wartosci wypelnienia dla danego wymuszenia  
	if(M._pob_KCW&&!M._ROZRUCH&&_GRZEJ_GetValue()) WritePWM3(ProcToMod(DtKNF.dmxcw));//WritePWM(PWM.BufPWM=ModToDVNT(ProcToVNT(DtKNF.dmxcw),DtKNF.mmin,DtKNF.mmax,PWM.BufPWM));//WritePWM(PWM.MMAXCW);
	if(M._pob_KCO&&!M._ROZRUCH&&_GRZEJ_GetValue()) WritePWM3(ProcToMod(DtKNF.dmxco));//WritePWM(PWM.BufPWM=ModToDVNT(ProcToVNT(DtKNF.dmxco),DtKNF.mmin,DtKNF.mmax,PWM.BufPWM));//WritePWM(PWM.MMAXCO);
#if _SRVTRB==1														//aktywny tryb serwisowy kompilacji?
	if(M._pob_SRW&&!M._ROZRUCH&&_GRZEJ_GetValue()) WritePWM3(mypwm);
#else
	if(M._pob_SRW&&!M._ROZRUCH&&_GRZEJ_GetValue())
	{
		if(InGetBit(RSDT1.inSTAT1,_SRVMAX)) WritePWM3(ProcToMod(99));
		else WritePWM3(ProcToMod(0));
	}
#endif
	if(M._pob_DzCO&&!M._ROZRUCH&&_GRZEJ_GetValue()) WritePWM3(PWM.MDMOD);//WritePWM(ProcToMod(0));//WritePWM(ModToMin());//WritePWM(PWM.BufPWM=ModToDVNT(DtKNF.mmin,DtKNF.mmin,ProcToVNT(DtKNF.mxco),PWM.BufPWM));//WritePWM(PWM.MDMOD);
	if(M._pob_DzCW&&!M._ROZRUCH&&_GRZEJ_GetValue()) WritePWM3(PWM.MDMOD);//WritePWM(ProcToMod(0));//WritePWM(ModToMin());//WritePWM(PWM.BufPWM=ModToDVNT(DtKNF.mmin,DtKNF.mmin,ProcToVNT(DtKNF.mxcw),PWM.BufPWM));//WritePWM(PWM.MDMOD);	
#else
	//deaktywacja bloku modulacji plomieniem dla awarii modulatora lub dla szybkiego narostu temp.
	if(!M._PRACA||M._RUN_MNMOD||(!M._PRACA&&M._VWYBIEG))
	{
		M._MODULUJ=0;	//wylacz blok modulacji
		if(M._RUN_MNMOD)
		{
			WritePWM3H(ProcToMod(0));//WritePWM(PWM.BufPWM=ModToDVNT(DtKNF.mmin,DtKNF.mmin,DtKNF.mmax,PWM.BufPWM));//WritePWM(PWM.MDMOD);		//zbyt szybki narost temp.
		}
		else
		if(M._VWYBIEG)
		{
			WritePWM3H(ProcToMod(0));//WritePWM(PWM.BufPWM=ModToDVNT(DtKNF.mmin,DtKNF.mmin,DtKNF.mmax,PWM.BufPWM));//WritePWM(PWM.MDMOD);		//wybieg wentylatora
		}
		else
		if(!M._PRACA)
		{
			WritePWM3H(0);
		}
		else WritePWM3H(0);
	}    
	if(M._pob_KCW&&!M._ROZRUCH&&_GRZEJ_GetValue()) WritePWM3H(ProcToMod(DtKNF.dmxcw));//WritePWM(PWM.BufPWM=ModToDVNT(ProcToVNT(DtKNF.dmxcw),DtKNF.mmin,DtKNF.mmax,PWM.BufPWM));//WritePWM(PWM.MMAXCW);
	if(M._pob_KCO&&!M._ROZRUCH&&_GRZEJ_GetValue()) WritePWM3H(ProcToMod(DtKNF.dmxco));//WritePWM(PWM.BufPWM=ModToDVNT(ProcToVNT(DtKNF.dmxco),DtKNF.mmin,DtKNF.mmax,PWM.BufPWM));//WritePWM(PWM.MMAXCO);
#if _SRVTRB==1														//aktywny tryb serwisowy kompilacji?
	if(M._pob_SRW&&!M._ROZRUCH&&_GRZEJ_GetValue()) WritePWM3H(mypwm);
#else
	if(M._pob_SRW&&!M._ROZRUCH&&_GRZEJ_GetValue())
	{
		if(InGetBit(RSDT1.inSTAT1,_SRVMAX)) WritePWM3H(ProcToMod(99));
		else WritePWM3H(ProcToMod(0));
	}
#endif
	if(M._pob_DzCO&&!M._ROZRUCH&&_GRZEJ_GetValue()) WritePWM3H(ProcToMod(0));//WritePWM(ProcToMod(0));//WritePWM(ModToMin());//WritePWM(PWM.BufPWM=ModToDVNT(DtKNF.mmin,DtKNF.mmin,ProcToVNT(DtKNF.mxco),PWM.BufPWM));//WritePWM(PWM.MDMOD);
	if(M._pob_DzCW&&!M._ROZRUCH&&_GRZEJ_GetValue()) WritePWM3H(ProcToMod(0));//WritePWM(ProcToMod(0));//WritePWM(ModToMin());//WritePWM(PWM.BufPWM=ModToDVNT(DtKNF.mmin,DtKNF.mmin,ProcToVNT(DtKNF.mxcw),PWM.BufPWM));//WritePWM(PWM.MDMOD);	    
#endif
}


//zmiana obiegu na CO
void ToObiegCO(void)
{
	switch(DtKNF.tpkt)		//dla danego rodzaju kotla
	{
		case _UNICO:
		case _MONOHB:
		{		
			if(!M._ZMKCOCW&&!_KCO_CW_GetValue()) 			//aktywny obieg CW?
			{
				M._RPM_CW=0;					//wyzeruj znacznik obiegu CW
				M._ZMKCOCW=1;					//uruchom procedure zmiany obiegu CW na CO
				M.ZKP=0;
			}
			return;
		}
		default: 			//domyslny monotermiczny
		{
			if(!M._ZMKCOCW&&!_KCO_CW_GetValue()) 			//aktywny obieg CW?
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
		case _UNICO:
		case _MONOHB:
		{		
			if(!M._ZMKCOCW&&_KCO_CW_GetValue()) 			//aktywny obieg CO?
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
			if(!M._ZMKCOCW&&_KCO_CW_GetValue()) 			//aktywny obieg CO?
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

//------------------
//------------------
//Decyzja o zapaleniu palnika
void DecRozruch(void)
{
	switch(DtKNF.tpkt)								//dla danego rodzaju kotla
	{
		case _UNICO:
		case _MONOHB:
		{
			if(!M._ROZRUCH&&!_GRZEJ_GetValue()&&!M._BLOKUJ_ODP)
			if(!M._ZMKCOCW&&!M._RUSZ_PWR&&!M._RUSZ_PLOMIEN&&!M._RUSZ_STB&&!M._RUSZ_NTC_CO&&!M._RUSZ_NTC_CW)
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
					if(DtCO._su_CO) {SetRozruch();}						
				}												
				else
				if(M._pob_SRW)									//funkcja serwisowa
				{
					if(DtCO._su_SRW_CO) {SetRozruch();}							
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
			if(!M._ROZRUCH&&!_GRZEJ_GetValue()&&!M._BLOKUJ_ODP)
			if(!M._ZMKCOCW&&!M._RUSZ_PWR&&!M._RUSZ_PLOMIEN&&!M._RUSZ_STB&&!M._RUSZ_NTC_CO&&!M._RUSZ_NTC_CW)
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
	M._HWB_CW=0;
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
		case _UNICO:
		case _MONOHB:
		{
			if(!M._WYLACZ)
			if(M._ROZRUCH||_GRZEJ_GetValue())
			{
				if(!M._pob_KCW&&!M._pob_AL&&!M._pob_KCO&&!M._pob_SRW&&!M._pob_CW&&!M._pob_CO&&!M._pob_DzCO&&!M._pob_DzCW)
				{
					SetWylacz();
					return;
				}
				if(M._RUSZ_PWR||M._RUSZ_PLOMIEN||M._RUSZ_STB||M._RUSZ_NTC_CO||M._RUSZ_NTC_CW||M._RUSZ_POMPE||M._RUSZ_CISN||M._RUSZ_NCISN)
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
			if(M._ROZRUCH||_GRZEJ_GetValue())
			{
				if(!M._pob_KCW&&!M._pob_AL&&!M._pob_KCO&&!M._pob_SRW&&!M._pob_CW&&!M._pob_CO&&!M._pob_DzCO&&!M._pob_DzCW)
				{
					SetWylacz();
					return;
				}
				if(M._RUSZ_PWR||M._RUSZ_PLOMIEN||M._RUSZ_STB||M._RUSZ_NTC_CO||M._RUSZ_NTC_CW||M._RUSZ_POMPE||M._RUSZ_CISN||M._RUSZ_NCISN)
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
		case _UNICO:
		case _MONOHB:
		{		
			if(!_KCO_CW_GetValue())					//obieg CW?
			{
				M._BLOKUJ_15S=1;			//uaktywnij blokade 15s
				M.BL15=0;		
			}
			break;
		}
		default:			//domyslny monotermiczny
		{		
			if(!_KCO_CW_GetValue())					//obieg CW?
			{
				M._BLOKUJ_15S=1;			//uaktywnij blokade 15s
				M.BL15=0;		
			}
			break;
		}
	}
	//-----
	M._WYLACZ=1;							//uaktywnij procedure sterowania wybiegiem pompy
	if(_VNT_GetValue()) M._VWYBIEG=1;					//dla kotla turbo uaktywnij wybieg wentylatora
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
			if((PFN.poz==_PFZMA)&&InGetBit(RSDT1.inSTAT0,_INITSRV))
			{
				M.SRV=1;
				M._AKTYWNE_SRW=1;		//aktywacja funkcji serwisowej					
				StartRTM(_RTSRV1);
			}
			break;
		}
		case 1:
		{
			//if((PFN.poz==_PFZMA)&&InGetBit(RSDT1.inSTAT0,_INITSRV)&&(RTM(_RTSRV1)<_TSRV1)) return;
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
			//if(RTM(_RTBL3)<_TBL3) return;
			if(RTM(_RTBL3)<DtKNF.tL3) return;            
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
			if(RTdS(_RTBL15)<_TBL15S&&(!M._pob_SRW)) return;
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
	_GRZEJ_SetLow();
}
//------------------
//------------------

//Procedura rozruchu kotla
void Rozruch(void)
{
	static unsigned char tim;

	StartRTS(_RTWPWM);
	StartRTM(_RTMMAXCO);
	switch(M.ROZ)
	{
		//dla kotla z otwartym obiegiem wstêpne, kontrolne wylaczenie pompy 
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
				_POMPA_SetLow();  							//wylacz pompe
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
				ErrPTG();										//wylaczenie awaryjne z blokad?					
				return;
			}
			M.ROZ=2;
			StartRTS(_RTROZ);
		}
		//procedura rozruchu
		case 2:
		{
            if(DtKNF.tpkt==_UNICO)	//1f (uniko?)    
            {
                if((M.STRB==1)||(M.STRB==2))        //tryb produkcyjny? 
                {
#if _I0_PMP==1                     
					if(!M._ZMKCOCW) _POMPA_SetHigh();					//zalacz pompe
#else
                    _POMPA_SetHigh();
#endif
                }
                else
                {
                    if(RTS(_RTROZ)>1)                   //odczekaj na domkniecie zaworu 3DR
                    {  
#if _I0_PMP==1                     
                        if(!M._ZMKCOCW) _POMPA_SetHigh();					//zalacz pompe
#else
                        _POMPA_SetHigh();
#endif                
                    }              
                    if(RTS(_RTROZ)<_TPRWB) return;      //odczekaj czas wstepnego wybiegu      
                }
            }
            else                    //2f
            {
#if _I0_PMP==1                     
				if(!M._ZMKCOCW) _POMPA_SetHigh();					//zalacz pompe
#else
                _POMPA_SetHigh();
#endif
            }
			M.ROZ=3;
			StartRTS(_RTROZ);
		}
		//kontrola poprawnosci dzialania ukladu detekcji plomienia
		case 3:
		{
#if _I0_PMP==1                     
			if(!M._ZMKCOCW) _POMPA_SetHigh();					//zalacz pompe
#else
            _POMPA_SetHigh();
#endif
			if(RdPrt(S_PLM)&&!_GRZEJ_GetValue())			//obecny plomien mimo ze _GRZEJ=0?
			{
				if(RTS(_RTROZ)<_TBPL0) return;
				M.ERR_BTY=0x06;
				PrintErr(0x06,1);					//wyswietl kod bledu
                ToWriteESTAT();
				ErrPTG();
			}
			GrzejON();//_GRZEJ_SetHigh();_VNT_SetHigh();							//zapal plomien na palniku,uruchom wentylator
			M.ROZ=4;
		}
		//oczekiwanie az sygnal obecnosci ciagu zniknie przy wylaczonym wentylatorze
		case 4:
		{
			StartRTdS(_RTFKVN);
			M.ROZ=5;
		}
		//oczekiwanie na sygnal obecnosci ciagu przy zalaczonym wentylatorze
		case 5:
		{
#if _I0_PMP==1                     
            if(!M._ZMKCOCW) _POMPA_SetHigh();					//zalacz pompe
#else
            _POMPA_SetHigh();
#endif
#if _HIRESVNT==0            
			WritePWM3(CalkVNTtoPWM(DtKNF.mmax));
			tim=CalkTimStr(ProcToVNT(DtKNF.mstr));
			if(RTdS(_RTFKVN)<tim) return;		//wstepny wybieg wentylatora na mocy maksymalnej
			WritePWM3(ProcToMod(DtKNF.mstr));
#else
			WritePWM3H(CalkVNTtoPWMH(DtKNF.mmax));
			tim=CalkTimStr(ProcToVNT(DtKNF.mstr));
			if(RTdS(_RTFKVN)<tim) return;		//wstepny wybieg wentylatora na mocy maksymalnej
			WritePWM3H(ProcToMod(DtKNF.mstr));            
#endif
			StartRTS(_RTROZ);
			M.ROZ=6;
		}
		//zalaczenie pompy, oczekiwanie na sygnal obecnosci plomienia
		case 6:
		{
			switch(DtKNF.tpkt)					//dla danego rodzaju kotla
			{
				case _UNICO:
				case _MONOHB:
				{
#if _I0_PMP==1                     
                    if(!M._ZMKCOCW) _POMPA_SetHigh();					//zalacz pompe
#else
                    _POMPA_SetHigh();
#endif
					break;
				}
				default:			//domyslny monotermiczny
				{
#if _I0_PMP==1                     
					if(!M._ZMKCOCW) _POMPA_SetHigh();					//zalacz pompe
#else
                    _POMPA_SetHigh();
#endif
					break;
				}				
			}
#if _HIRESVNT==0            
			WritePWM3(ProcToMod(DtKNF.mstr));//WritePWM(CalkVNTtoPWM(ProcToVNT(DtKNF.mstr)));	//moc startowa
#else
			WritePWM3H(ProcToMod(DtKNF.mstr));//WritePWM(CalkVNTtoPWM(ProcToVNT(DtKNF.mstr)));	//moc startowa            
#endif
			if(RTS(_RTROZ)<_TITPL) return;			//okres czasu gdy ignorowany jest syg. plomien 

			if(RTS(_RTROZ)<_TDTPL)
			{
				if(RdPrt(S_PLM)) 					//stwierdzono obecnosc plomienia?
				{
					M.ROZ=7;
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
		case 7:								
		{
#if _HIRESVNT==0            
			WritePWM3(ProcToMod(DtKNF.mstr));//WritePWM(CalkVNTtoPWM(ProcToVNT(DtKNF.mstr)));	//moc startowa
#else
			WritePWM3H(ProcToMod(DtKNF.mstr));//WritePWM(CalkVNTtoPWM(ProcToVNT(DtKNF.mstr)));	//moc startowa            
#endif
			if(M._RPM_CW)                   //dla obiegu CW zakoncz wybieg
			{
				M._ROZRUCH=0;
				M.ROZ=0;
				return;
			}         
			if(!RdPrt(S_PLM)) 								//stwierdzono brak plomienia?		
			{
				DecyzjaPLM2();
			}
            if(M._AKTYWNE_SRW||M._AKTYWNE_KNF||(M.STRB==1)||(M.STRB==2))    //funkcja serwisowa,tryb konfiguracji lub TRYB SPECJALNY
            {
                if(RTS(_RTROZ)<_TDWPL) return;              //krotki wybieg na mocy startowej
            }
            else
            {
                if(RTS(_RTROZ)<DtKNF.tstr) return;          //wybieg na mocy startowej regulowany parametrem P24           
            }
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
	if(_STB_GetValue()&&_CIG_NC_GetValue())
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
//------------------
//------------------
//Wybieg pompy
void Wylacz(void)
{
	switch(DtKNF.tpkt)										//dla danego rodzaju kotla
	{
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
						if(RTS(_RTWYL)>=_TmnWBW0)				//uplynal minimalny wybieg pompy?		
						{
                            if(!RdPrt(S_ERR_CO)&&DtCO._ptwp_CO)	//temperatura CO osiagnela prog wylaczenia?
                            {
                                M.WYL=2;
                                return;	
                            }
						}
						if(RTS(_RTWYL)<_TmnWBW1) return;  		//uplynal maksymalny wybieg pompy?
						M.WYL=2;
						return;
					}
					else			//obieg CO
					{
						if(RTS(_RTWYL)<_TWBCO) return;  				//uplynal maksymalny wybieg pompy?
						M.WYL=2;
					}
				}
				case 2:
				{
					if(!M._RPM_CW)	//obieg CO
					{
						/*if(RdPrt(S_ON_PMP)) return; 					//przekroczone wartosci graniczne temp w obiegu CO
						if(!RdPrt(S_ERR_CO)&&!DtCO._su_CO) return; 		//osiagnieto wartosc wylaczenia po pracy w ukladzie CO?*/
                        //***************************************
						if(RdPrt(S_ON_PMP)) return; 					//przekroczone wartosci graniczne temp w obiegu CO
                        if(M._pob_KCO||M._pob_SRW||M._pob_CO)
                        {
                            if(!RdPrt(S_ERR_CO)&&!DtCO._su_CO) return; 		//osiagnieto wartosc wylaczenia po pracy w ukladzie CO? 
                        }
					}
					else
					{
                        //***************************************
                        if(RdPrt(S_ON_PMP)) return; 					//przekroczone wartosci graniczne temp w obiegu CO
						if(!RdPrt(S_ERR_CO)&&!DtCO._ptwp_CO&&M._pob_CW) return; //obieg CW, trwa wymuszenie CW a temp CO>od progu zalaczenia
					}
					if(DtKNF.robg==_OTWARTY) 	//z obiegiem otwartym?
					{
						if(M._RUSZ_POMPE) return;						//oczekiwanie na powrot czujnika przeplywu WG do prawidlowej pozycji
					}
#if _I0_PMP==1                     
					if(!M._ZMKCOCW) _POMPA_SetLow();					//wylacz pompe
#else
                    _POMPA_SetLow();
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
						M._HWB_CW=1;							//ustaw znacznik wybiegu od wylaczenia w obiegu CW
						if(RTS(_RTWYL)<_TzsWBW) return;  		//uplynal maksymalny wybieg pompy?
						if(!RdPrt(S_ERR_CO))
						{
                            if(DtCW._wpz_CW||M._pob_AL||M._pob_DzCW||M._pob_KCW)
                            {
                                if(DtCO._ptwp_CO)					//temperatura CO osiagnela prog wylaczenia?
                                {
                                    M.WYL=2;
                                    return;
                                }
                            }
                            else
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
						M._HWB_CW=0;
						if(RTS(_RTWYL)<_TWBCO) return;  				//uplynal maksymalny wybieg pompy?
						M.WYL=2;
					}
				}
				case 2:
				{
					if(M._RPM_CW)	//obieg CW
					{
						if(RdPrt(S_ON_PMP)||RdPrt(S_ON_PMP_CW)) return;	//niska graniczne temp w obiegu CW
						//if(RdPrt(S_ON_PMP)&&(PFN.poz==_PFZMA)&&!RdPrt(S_RT)) return;	//brak pobudzen i przekroczone wartosci graniczne temp w obiegu CO
					}
					else			//obieg CO
					{
						//if(RdPrt(S_ON_PMP)) return; 					//przekroczone wartosci graniczne temp w obiegu CO
						//if(!RdPrt(S_ERR_CO)&&!DtCO._su_CO) return; 		//osiagnieto wartosc wylaczenia po pracy w ukladzie CO?
                        //***************************************
						if(RdPrt(S_ON_PMP)) return; 					//przekroczone wartosci graniczne temp w obiegu CO
                        if(M._pob_KCO||M._pob_SRW||M._pob_CO)
                        {
                            if(!RdPrt(S_ERR_CO)&&!DtCO._su_CO) return; 		//osiagnieto wartosc wylaczenia po pracy w ukladzie CO? 
                        }                        
					}
					M._HWB_CW=0;
					if(DtKNF.robg==_OTWARTY) 	//z obiegiem otwartym?
					{
						if(M._RUSZ_POMPE) return;						//oczekiwanie na powrot czujnika przeplywu WG do prawidlowej pozycji
					}
#if _I0_PMP==1                     
					if(!M._ZMKCOCW) _POMPA_SetLow();					//wylacz pompe
#else
                    _POMPA_SetLow();
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
						if(RTS(_RTWYL)>=_TmnWBW0)				//uplynal minimalny wybieg pompy?		
						{
							if(!RdPrt(S_ERR_CO)&&DtCO._ptwp_CO)	//temperatura CO osiagnela prog wylaczenia?
							{
								M.WYL=2;
								return;	
							}
						}
						if(RTS(_RTWYL)<_TmnWBW1) return;  		//uplynal maksymalny wybieg pompy?
						M.WYL=2;
						return;
					}
					else			//obieg CO
					{
						if(RTS(_RTWYL)<_TWBCO) return;  				//uplynal maksymalny wybieg pompy?
						M.WYL=2;
					}
				}
				case 2:
				{
					if(!M._RPM_CW)	//obieg CO
					{
						//if(RdPrt(S_ON_PMP)) return; 					//przekroczone wartosci graniczne temp w obiegu CO
						//if(!RdPrt(S_ERR_CO)&&!DtCO._su_CO) return; 		//osiagnieto wartosc wylaczenia po pracy w ukladzie CO?
                        //***************************************
						if(RdPrt(S_ON_PMP)) return; 					//przekroczone wartosci graniczne temp w obiegu CO
                        if(M._pob_KCO||M._pob_SRW||M._pob_CO)
                        {
                            if(!RdPrt(S_ERR_CO)&&!DtCO._su_CO) return; 		//osiagnieto wartosc wylaczenia po pracy w ukladzie CO? 
                        }                        
					}
					else
					{
                        //***************************************
						if(RdPrt(S_ON_PMP)) return; 					//przekroczone wartosci graniczne temp w obiegu CO                        
						if(!RdPrt(S_ERR_CO)&&!DtCO._ptwp_CO&&M._pob_CW) return; //obieg CW, trwa wymuszenie a temp CO>od progu zalaczenia
					}
					if(DtKNF.robg==_OTWARTY) 	//z obiegiem otwartym?
					{
						if(M._RUSZ_POMPE) return;						//oczekiwanie na powrot czujnika przeplywu WG do prawidlowej pozycji
					}
#if _I0_PMP==1                     
					if(!M._ZMKCOCW) _POMPA_SetLow();					//wylacz pompe
#else
                    _POMPA_SetLow();
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
#if _HIRESVNT==0            
			WritePWM3(ProcToMod(0));
#else
			WritePWM3H(ProcToMod(0));            
#endif
		}
		case 1:
		{	
			if(RTS(_RTVWB)>=_TVWB0) M._DE_VNT=0;	//wyzeruj znacznik probnego wybiegu wentylatora
			if(RTS(_RTVWB)<_TVWB1) return;
#if _HIRESVNT==0            
			WritePWM3(0);
#else
			WritePWM3H(0);            
#endif            
			_VNT_SetLow();									//wylacz wentylator
			M._DE_VNT=1;
			M.VWB=2;
		}
		case 2:
		{
			if(RTS(_RTVWB)<_TVWB1+2) return;		//beznapieciowy wybieg wentylatora
			M._DE_VNT=0;
			M._VWYBIEG=0;
			M.VWB=0;
			//x=0;
		}
	}
}
//------------------
//------------------
//Detekcja zaniku przeplywu WG (oczekuje przez pewien czas zanim uruchomi procedure rozruchu przeplywu)
void DetekcjaPrzeplywuWG(void) 
{
	unsigned char pmp,prz;

	pmp=(_POMPA_GetValue())?1:0;
	prz=(RdPrt(S_PRZ_CO))?1:0;

	if(pmp)		//pompa pracuje?
	{
		if(!prz&&!M._DE_PMP)							//zmiana stanu przep³ywu?
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
					if(RTS(_RTDPR)<_TDPR1) return;		//oczekuj przez zadany czas na przep³yw WG
					if(!M._RUSZ_POMPE&&!M._PROC_ODP)
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

	pmp=(_POMPA_GetValue())?1:0;
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
		case _UNICO:
		case _MONOHB:
		{
			switch(M.ZKP)
			{
				case 0:
				{
					M.ZKP=1;
#if _I0_PMP==1                    
					_POMPA_SetLow();									//wylacz pompe
#endif
					StartRTdS(_RTZKP);
                    StartRTdS(_RTHPM);
                    M._LOW_PMP=1;
				}
				case 1:
				{	
					if(RTdS(_RTZKP)<_TZKP/2) return;
					_KCO_CW_Toggle();							//zmian kierunku obrotu pompy
					M.ZKP=2;
				}
				case 2:
				{
					if(RTdS(_RTZKP)<_TZKP) return;
					M._ZMKCOCW=0;
					M.ZKP=0;
#if _I0_PMP==1
					//warunek ponownego zalaczenia pompy
					if(M._ROZRUCH||_GRZEJ_GetValue()||M._WYLACZ||M._RUSZ_48||M._RUSZ_48Z3) _POMPA_SetHigh();			//wlacz pompe
					if(DtKNF.robg==_OTWARTY) 							//uniko z obiegiem otwartym?
					{
						if(M._RUSZ_POMPE) _POMPA_SetHigh();						//trwa oczekiwanie na powrot czujnika przeplywu WG do prawidlowej pozycji
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
					_POMPA_SetLow();									//wylacz pompe
#endif
					StartRTdS(_RTZKP);
                    StartRTdS(_RTHPM);
                    M._LOW_PMP=1;            //aktywacja obnizonej predkosci pompy         
				}
				case 1:
				{	
					if(RTdS(_RTZKP)<_TZKP/2) return;
					_KCO_CW_Toggle();							//zmian kierunku obrotu pompy
					M.ZKP=2;
				}
				case 2:
				{
					if(RTdS(_RTZKP)<_TZKP) return;
					M._ZMKCOCW=0;
					M.ZKP=0;
#if _I0_PMP==1
					//warunek ponownego zalaczenia pompy
					if(M._ROZRUCH||_GRZEJ_GetValue()||M._WYLACZ||M._RUSZ_48||M._RUSZ_48Z3) _POMPA_SetHigh();	//wlacz pompe
					if(DtKNF.robg==_OTWARTY) 							//uniko z obiegiem otwartym?
					{
						if(M._RUSZ_POMPE) _POMPA_SetHigh();						//trwa oczekiwanie na powrot czujnika przeplywu WG do prawidlowej pozycji
					}
#endif
				}
			}
			return;
		}
	}
}
//------------------
//------------------
//Obnizona predkosc wybiegu pompy na czas przelaczenia kierunku na zaworze 3DR
void SetLowPMP(void)
{
    if(!M._ZMKCOCW&&!_POMPA_GetValue())     //pompa nie pracuje?
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
	if(M._RUSZ_PLOMIEN) StartRTS(_RTRPL2);
	if(RTS(_RTRPL2)>_TPLPR2)					//po zadanym czasie wyzeruj licznik_zaników_plomienia/min
	{
		M.LICZNIK_PL2=0;
	}
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
				InBitSet(&RSDT1.outSTAT2,_KALIB,0);
				_VNT_SetLow();
#if _HIRESVNT==0            
			WritePWM3(0);
#else
			WritePWM3H(0);            
#endif
				M.ERR_BTY=0x07;
				PrintErr(0x07,1);
                ToWriteESTAT();
				ErrPTG();
			}
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
	if(_POMPA_GetValue()) StartRTG(_RTR48);
	else
	{
		if(M._PROC_ODP) return;
		if(RTG(_RTR48)>=_TPR48)
		{
			M.R48=0;
			M._RUSZ_48=1;
			StartRTG(_RTR48);
		}
	}
#else
	if(M._RUSZ_48||M._RUSZ_48Z3) return;
	if(_POMPA_GetValue()) StartRTM(_SRVRTANL);
	else
	{
		if(M._PROC_ODP) return;
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
					_POMPA_SetHigh();				//zalacz pompe
					M.R48=1;
				}
				case 1:
				{	
					if(RTS(_RTWB48)<_TWB48) return;
					if(M._ZMKCOCW) return;	//odczekaj az zakonczy sie procedura zmiany obiegu
					_POMPA_SetLow();				//wylacz pompe
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
					_POMPA_SetHigh();				//zalacz pompe
					M.R48=1;
				}
				case 1:
				{	
					if(RTS(_RTWB48)<_TWB48) return;
					if(M._ZMKCOCW) return;	//odczekaj az zakonczy sie procedura zmiany obiegu
					_POMPA_SetLow();				//wylacz pompe
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
			if(_POMPA_GetValue()&&!_KCO_CW_GetValue()) StartRTG(_RTR48Z3); //pompa pracuje w obiegu WU?
			else
			{
				if(_POMPA_GetValue()||M._ZMKCOCW||M._PROC_ODP) return;
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
			if(_POMPA_GetValue()&&!_KCO_CW_GetValue()) StartRTM(_RTMAL); //pompa pracuje w obiegu WU?
			else
			{
				if(_POMPA_GetValue()||M._ZMKCOCW||M._PROC_ODP) return;
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
					_POMPA_SetHigh();				//zalacz pompe
					M.R48Z3=1;
				}
				case 1:
				{	
					if(RTS(_RTWB48)<_TWB48Z3) return;
					if(M._ZMKCOCW) return;	//odczekaj az zakonczy sie procedura zmiany obiegu
					_POMPA_SetLow();				//wylacz pompe
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
//Odpowietrzenie obiegu CO
void DecyzjaOdp(void)
{
	if(DtKNF.robg!=_OTWARTY)		//uklad zamkniety?
	{
		if((!MGSD._CON)||(MGSD._CON&&(RSDT1.vSTD!=0)))	//wersja wyswietlacza >=7 ?
		{
			if(M._BLOKUJ_ODP||M._RUSZ_POMPE||M._RUSZ_48||M._RUSZ_48Z3||M._AKTYWNE_KNF) return;
			if(M._RUSZ_CISN&&!RdPrt(S_WRK_CSN)&&(DtCS.CS<=_OFF1_WRK_CSN))
			{
			 	M._BLOKUJ_ODP=1;		//uruchom blokade przed proc.odpowietrzenia
			}
		}
	}
}
void Odpowietrzenie(void)
{
	static unsigned char obg=0;
	
	//wyjscie z blokady w wyniku aktywacji procedury konfiguracji
	if(M._BLOKUJ_ODP&&M._AKTYWNE_KNF)
	{
		M._BLOKUJ_ODP=0;			//zakoncz procedure
		M._PROC_ODP=0;
		M.LICZNIK_ODP=0;
		M.ODP=0;
		ToObiegCO();		//przejdz na obieg CO
		_POMPA_SetHigh();			//wlacz wybieg pompy
		M._WYLACZ=1;
		M.WYL=0;
		return;
	}

	//wyjscie manualne z procedury odpowietrzenia
	if(M._BLOKUJ_ODP&&M._PROC_ODP)	//trwa procedura odpowietrzenia?
	{
        //przycisniety przycisk (+ i -) lub proba wejscia do konfiguracji lub zdalna deaktywacja procedury lub tryb produkcyjny
        if((InGetBit(RSDT1.inSTAT1,_KPLUS)&&InGetBit(RSDT1.inSTAT1,_KMINUS))||(PFN._off&&DtKNF.enknf&&InGetBit(RSDT1.inSTAT1,_KSET)&&InGetBit(RSDT1.inSTAT1,_KOFF))||M._LIN_ENDO||(M.STRB==1)||(M.STRB==2))	//manualne wyjscie z procedury    
		{
			M._BLOKUJ_ODP=0;				//zakoncz procedure
			M._PROC_ODP=0;
			M.LICZNIK_ODP=0;
			M.ODP=0;
			ToObiegCO();		//przejdz na obieg CO
			_POMPA_SetHigh();			//wlacz wybieg pompy
			M._WYLACZ=1;
			M.WYL=0;
			return;	
		}
	}

	switch(M.ODP)
	{
		//blokada przed proc.odpowietrzenia
		case 0:		
		{
			obg=0;						//(pierwszy wybieg w obiegu CO)
			StartRTS(_RTWB48);
			if(M._ZMKCOCW) return;		//odczekaj az zakonczy sie procedura zmiany obiegu
			if(!M._WYM_ODP)				//wymuszenie procedury?
			{
				if(!M._RUSZ_CISN&&RdPrt(S_WRK_CSN)) //uruchom procedure po naroscie cisnienia po stanie awaryjnym E9
				{
					M.ODP=1;
				}
			}
			else
			{
				M._WYM_ODP=0;			//bezwarunkowo uruchom procedure
				M.ODP=1;
			}
			if(M.ODP)
			{
				M._PROC_ODP=1;

				M._RUSZ_48=0;			//deaktywuj procedure 48h
				M.R48=0;
				StartRTG(_RTR48);
	
				M._RUSZ_48Z3=0;			//deaktywuj procedure z48h
				M.R48Z3=0;
				StartRTG(_RTR48Z3);

				M._WYLACZ=0;			//deaktywuj procedure wylacz			
				M.WYL=0;
			}
			break;
		}
		//proc. odpowietrzenia
		case 1:	//praca pompy
		{	
            //zmien obieg (dla kotlow 1F obg stale rowna sie 0)
			if(!obg) ToObiegCO();		//przejdz na obieg CO
			else ToObiegCW();			//przejdz na obieg CW
			if(M._ZMKCOCW) return;		//odczekaj az zakonczy sie procedura zmiany obiegu
			PMP.APROC=99;
			WritePWM2(ProcToPmpPwm(PMP.APROC));
			_POMPA_SetHigh();					//wlacz pompe
			if(RTS(_RTWB48)<15) return;	//odczekaj czas pracy
			StartRTS(_RTWB48);
			M.ODP=2;
			break;
		}
		case 2:	//wylacz pompe
		{	
			if(M._ZMKCOCW) return;		//odczekaj az zakonczy sie ew.procedura zmiany obiegu
			PMP.APROC=0;
			WritePWM2(ProcToPmpPwm(PMP.APROC));
			_POMPA_SetLow();					//wylacz pompe
			if(RTS(_RTWB48)<15) return;	//odczekaj czas wybiegu
			StartRTS(_RTWB48);
			M.LICZNIK_ODP++;
			if(M.LICZNIK_ODP<6)
			{
				if(DtKNF.tpkt!=_UNICO)
				{
					obg=0;              //stale obieg CO dla kotlow 2F
				}
				else                    //kotly 1F
				{
					if(obg) obg=0;		//nastempny wybieg w obiegu CO
					else obg=1;			//nastempny wybieg w obiegu CW
				}
				M.ODP=1;
			}
			else
			{
				M._BLOKUJ_ODP=0;	//zakoncz procedure
				M._PROC_ODP=0;
				M._WYM_ODP=0;
				M.LICZNIK_ODP=0;
				M.ODP=0;

				ToObiegCO();		//przejdz na obieg CO
				_POMPA_SetHigh();			//wlacz wybieg pompy
				M._WYLACZ=1;
				M.WYL=0;
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
    InBitSet(&RSDT1.outSTAT0,_PGPRG,0);
    InBitSet(&RSDT1.outSTAT0,_PGRES,1);		//znacznik petli wylaczenia awaryjnego
    if(RefBHist(0))                         //uzupelnij historie stanow awaryjnych, jesli nie nastapila powtorka wystapienia to zapisz stan kotla z blokady awaryjnej w pamieci nieulotnej
    {
        RefBStat(0);                            //zapisz stan kotla w pamieci nieulotnej
    }
    //HBL=ReadBtyNZSTAT(0);
    //HRF=ReadBtyHREF(0);
    
	GrzejOFF();//_GRZEJ=0;
	VZPauza();								//przerwa czasowa na wypadek stanow nieustalonych
	if(DtKNF.robg==_OTWARTY) 				//uklad z obiegiem otwartym?
	{
		if(M.ERR_BTY==0x05)					//dozwolony kod bledu?
		{
			if_WriteEEPROM(_ADRERR,M.ERR_BTY);
		}
	}														
	if(((M.ERR_BTY>=0x01)&&(M.ERR_BTY<=0x03))||(M.ERR_BTY==0x07)||(M.ERR_BTY==0x0D)) if_WriteEEPROM(_ADRERR,M.ERR_BTY);//WriteEEPROM(_ADRERR,M.ERR_BTY); 	//zapis kodu bledu do pamieci
	M._RUSZ_POMPE=0;
	M._ROZRUCH=0;
	M._MODULUJ=0;

	M._BLOKUJ_ODP=0;		
	M._PROC_ODP=0;
	M._WYM_ODP=0;

#if _HIRESVNT==0            
	WritePWM3(0);
#else
	WritePWM3H(0);            
#endif
	if(!M._WYLACZ)							//aktywacja wybiegu pompy 
	{
		M.WYL=0;
		switch(DtKNF.tpkt)								//dla danego rodzaju kotla
		{
			case _UNICO:
			case _MONOHB:
			{
#if _I0_PMP==1                     
                if(M._ZMKCOCW) M._WYLACZ=1;
                else
                if(_POMPA_GetValue()) M._WYLACZ=1;
#else
                if(_POMPA_GetValue()) M._WYLACZ=1;
#endif
				break;	
			}
			default:			//domyslny monotermiczny
			{
#if _I0_PMP==1                     
                if(M._ZMKCOCW) M._WYLACZ=1;
                else
                if(_POMPA_GetValue()) M._WYLACZ=1;
#else
                if(_POMPA_GetValue()) M._WYLACZ=1;
#endif
				break;	
			}
		}
	}
	if(_VNT_GetValue()) M._VWYBIEG=1;		//aktywacja wybiegu wentylatora
    else M.VVNT=0;              //zeruj wartosc pomiaru predkosci wentylatora
	M._RUSZ_PWR=0;
    M._SYNCH_PGD=0;    
	M.HVZ=0;
	M.KRS=0;
	//k=0;
//------
	while(1)
	{
		CLRWDT();
        
		InBitSet(&RSDT1.outSTAT2,_KALIB,0);		//wyzeruj znacznik aktywnosci procedury kalibracji
		InBitSet(&RSDT1.outSTAT0,_PGPRG,0);
		InBitSet(&RSDT1.outSTAT0,_PGRES,1);		//znacznik petli wylaczenia awaryjnego
		InBitSet(&RSDT1.outSTAT1,_ERR,1);
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
		PomiarVNT();			//predkosc obrotowa wentylatora

        SetTrbPGD();//************************
        
		//blok decyzyjny	
		Proba48();				//zegar 48h wybiegu pompy
		Proba48Z3();			//zegar 48h wybiegu pompy dla WU
        
        if(!_VNT_GetValue()&&!M._VWYBIEG) M.VVNT=0;    //zeruj wartosc pomiaru predkosci wentylatora
   
		//sygnalizacja stanu pracy dla modulu LCD
		RefLCDData();

		//wybor obiegu
		switch(DtKNF.tpkt)								//dla danego rodzaju kotla
		{
			case _MONOHB:
			{
				//wybor obiegu
				if(!M._ZMKCOCW&&RdPrt(S_ON_PMP)) ToObiegCO();							//temperatura CO poza dozwolonym zakresem?
				else
				if(!M._ZMKCOCW&&!M._WYLACZ&&!M._RUSZ_48&&(!_POMPA_GetValue()||M._RUSZ_48Z3)) ToObiegCW();			//nieaktywna pompa?
				//aktywacja wybiegu, zalaczenie pompy		
				if(!M._RUSZ_PWR&&!M._ZMKCOCW&&!_POMPA_GetValue()&&!M._WYLACZ&&!M._RUSZ_48&&!M._RUSZ_48Z3)
				{
					if(!M._RPM_CW&&RdPrt(S_ON_PMP))	 									//temperatura CO/CW poza dozwolonym zakresem?
					{
						M._WYLACZ=1;	//uaktywnij wybieg pompy							 									//uaktywnij procedure wybiegu pompy
						_POMPA_SetHigh();		//zalacz pompe
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
                    if(!M._ZMKCOCW&&!_POMPA_GetValue()&&!M._WYLACZ&&!M._RUSZ_48) ToObiegCW();			//nieaktywna pompa?
                }
				//aktywacja wybiegu, zalaczenie pompy		
				if(!M._RUSZ_PWR&&!M._ZMKCOCW&&!_POMPA_GetValue()&&!M._WYLACZ&&!M._RUSZ_48)
				{
					if((!M._RPM_CW&&RdPrt(S_ON_PMP))||(M._RPM_CW&&RdPrt(S_DZ_CW)))		//temperatura CO/CW poza dozwolonym zakresem?
					{
						M._WYLACZ=1;	//uaktywnij wybieg pompy							 									//uaktywnij procedure wybiegu pompy
						_POMPA_SetHigh();		//zalacz pompe
					}
				}
				break;
			}
			default:			//domyslny monotermiczny
			{
				if(!M._ZMKCOCW&&RdPrt(S_ON_PMP)) ToObiegCO();							//temperatura CO poza dozwolonym zakresem?
				else
				if(!M._ZMKCOCW&&!_POMPA_GetValue()&&!M._WYLACZ&&!M._RUSZ_48) ToObiegCW();			//nieaktywna pompa?
				//aktywacja wybiegu, zalaczenie pompy		
				if(!M._RUSZ_PWR&&!M._ZMKCOCW&&!_POMPA_GetValue()&&!M._WYLACZ&&!M._RUSZ_48)
				{
					if(!M._RPM_CW&&RdPrt(S_ON_PMP))	 									//temperatura CO/CW poza dozwolonym zakresem?
					{
						M._WYLACZ=1;	//uaktywnij wybieg pompy							 									//uaktywnij procedure wybiegu pompy
						_POMPA_SetHigh();		//zalacz pompe
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
		if(_POMPA_GetValue())
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
			_VNT_SetLow();
			_POMPA_SetLow();
			ToReset();
		}
	}
}
//Wymuszenie resetu sterownika.
void ToReset(void)
{
	_VNT_SetLow();
	_POMPA_SetLow();
	EndErr();
	InBitSet(&RSDT1.outSTAT0,_PGRES,0);		//znacznik petli wylaczenia awaryjnego
	InBitSet(&RSDT1.outSTAT0,_PGPRG,0);
	if_WriteEEPROM(_ADRERR,0x00);
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
#endif

