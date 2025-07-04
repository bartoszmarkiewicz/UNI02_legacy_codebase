/*konfig.c*/
//Procedury zwiazane z wyborem typu gazu i poziomow
//mocy startowej oraz majksymalnej dla CO i CW
/*---------------------------------------------------------------------------------*/
#include <xc.h>
#include <stdbool.h>
#include <stdint.h>

#include "../../proj.tp.h"

#ifndef _TYP_PRJ
#error NIEOKRESLONY IDENTYFIKATOR PROJEKTU
#endif

#if _TYP_PRJ==2
#advisory _TYP_PRJ==2 
#include "../../mcc_generated_files\mcc.h"

#include "global.h"
#include "main.h"
#include "global.ex.h"

#include "konfig.h"		//parametry lokalne			
#include "pomiar.h"

extern MGSRSData MGSD;
extern tpMAIN M;
extern tpRSDTA1 RSDT1;
extern DataPFN PFN;
extern DataCO DtCO;
extern DataCW DtCW;
extern DataZW DtZW;

DtKONF DtKNF;

//Dozwolone wartosci graniczne wszystkich parametrow
DtMINMAX MinMax[_KNPAR1]=
{
	_ADRMST, _KMINMST, _KMAXMST,				//0 moc startowa
	_ADRMCW, _KMINMCW, _KMAXMCW,				//1 moc maksymalna CW
	_ADRMCO, _KMINMCO, _KMAXMCO, 				//2 moc maksymalna CO
    _ADRRGZ, _KMINRGZ, _KMAXRGZ,                //3 rodzaj gazu
	_ADRTAL, _KMINEAL, _KMAXEAL,				//4 aktywacja zegara antylegionella
	_ADRWTK, _KMINWTK, _KMAXWTK,                //5 kod kotla
	_ADROBG, _KMINOBG, _KMAXOBG,				//6 rodzaj obiegu
	_ADRPDG, _KMINOGP, _KMAXOGP,				//7 ogrzewanie podlogowe
	_ADRCS5, _KMINTPC, _KMAXTPC,				//8 wybor rodzaju czujnika cisnienia
	_ADRRDPM, _KMINRDP,_KMAXRDP,				//9 rodzaj pompy
	_ADRDLPM, _KMINDLP,_KMAXDLP,				//10 delta T dla pompy
	_ADRMNPM, _KMINMNP,_KMAXMNP,				//11 minimalny wydatek pompy
	_ADRMXPM, _KMINMXP,_KMAXMXP,				//12 maksymalny wydatek pompy
	_ADRMXPM2,_KMINMXP2,_KMAXMXP2,				//13 maksymalny wydatek pompy dla obiegu CW
	_ADRHSCO,_KMINHSCO,_KMAXHSCO,				//14 histereza wylaczenia grzania CO (ponad nastawe)
	_ADRCONW,_KMINCONW,_KMAXCONW,				//15 temp CO przy grzaniu CW zasobnika 
	_ADRPROP,_KMINPROP,_KMAXPROP,				//16 poprawka dla regulatora pogodowego
	_ADRPGMX,_KMINPGMX,_KMAXPGMX,				//17 maksymalna wartosc graniczna CO
	_ADRPGDR, _KMINPGDR, _KMAXPGDR,				//18 tryb pracy regulatora pogodowego
	_ADRCWMG, _KMINCWMG, _KMAXCWMG,				//19 histereza wylaczenia CW(4F) 
	_ADRCWTM, _KMINCWTM, _KMAXCWTM,				//20 we. sygnalu timera zasobnika (1F)     
};
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
static bool inline InGetBit(unsigned char data, const unsigned char nbit)
{
	return ((data&(1<<nbit))?1:0);    
}
//-------------------------------------------------------------------------------
static void inline InBitSet(unsigned char *data, const unsigned char nbit,const unsigned char bt)
{
	*data=(bt)?(*data|(1<<nbit)):(*data&(~(1<<nbit)));
}
//-------------------------------------------------------------------------------
/*---------------------------------------------------------------------------------*/
//Odblokowanie procedury wejsciowej konfiguracji
//po uruchomieniu sterownika na pozycji OFF
//UEAGA: umiescic przed petla glowna
void EnKonf(void)
{
	DtKNF.enknf=1;							//odblokuj mozliwosc wejscia do konf.(znacznik wysylany w RefLCDData())
	StartRTS(_RTKONF1);						//start zegara dla czasu w ktorym mozna aktywowac procedure konfiguracji
}
void DsKonf(void)
{
	DtKNF.enknf=0;							//zablokuj mozliwosc wejscia do konf.(znacznik wysylany w RefLCDData())
	StartRTS(_RTKONF1);						//start zegara dla czasu w ktorym mozna aktywowac procedure konfiguracji
}
/*---------------------------------------------------------------------------------*/
//Procedura konfiguracji sterowana z modulu zewnetrznego (deaktywacja nastepuje z poziomu plytki interfejsu lub po utracie komunikacji)
void MKonfiguracja(void)
{
	static unsigned char bf_AKTYWNE_KNF=0, bf_SAVE_PAR=0;

	M._AKTYWNE_KNF=InGetBit(RSDT1.inSTAT0,_INITKNF);								//flaga aktywnosci procedury konfiguracji

	//wyzerowanie flagi zezwolenia na aktywacje trybu konfiguracji
#if _PWRKNF==1   
	if(DtKNF.enknf&&(M._AKTYWNE_KNF||(RTS(_RTKONF1)>=_TKNFDIS)))				//konfiguracja juz aktywna lub uplynal czas mozliwosci wejscia do konf.?
	{
		DtKNF.enknf=0;															//zablokuj mozliwosc aktywowania procedury konf. (znacznik wysylany w RefLCDData())
	}
#else
    if(PFN._off) EnKonf();                  //aktywacja oczekiwania na wejscie do konfiguracji
    else DsKonf();    
#endif
	//wyjscie z proc. konfiguracji
	if((bf_AKTYWNE_KNF&&!M._AKTYWNE_KNF)||!MGSD._CON) 										//odczytaj parametry po wyjsciu z procedury konfiguracji
	{
		bf_AKTYWNE_KNF=0;
		DtKNF.wymCO=0;															//wylacz wymuszenie CO
		DtKNF.wymCW=0;															//wylacz wymuszenie CO
		bf_SAVE_PAR=0;
		RdEEParam();															//odczytaj parametry z pamieci EEPROM
		return;
	}	
	if(M._AKTYWNE_KNF)
	{
		if(!bf_AKTYWNE_KNF)
		{
			bf_SAVE_PAR=0;
		}
		bf_AKTYWNE_KNF=1;
		DtKNF.KRK=RSDT1.inKRK;								//krok procedury konfiguracji
		RSDT1.outKRK=DtKNF.KRK;								//informacja zwrotna o aktualnym kroku
		switch(DtKNF.KRK)
		{
			case 2:	//tryb wyboru parametru
			{
				DtKNF.wymCO=0;								//wylacz wymuszenie CO
				DtKNF.wymCW=0;								//wylacz wymuszenie CO
				RefKnfPar();								//aktualizuj wartosc parametrów
				break;	
			}	
			case 3: //tryb modyfikacji parametru
			{
				DtKNF.PRM=RSDT1.inPRM;						//nr parametru
				RSDT1.outPRM=DtKNF.PRM;						//informacja zwrotna o aktualnym parametrze

				if(DtKNF.PRM>2)								//sterowanie wymuszeniami dla parametrow statycznych
				{
					DtKNF.wymCO=0;							//wylacz wymuszenie CO
					DtKNF.wymCW=0;							//wylacz wymuszenie CW
				}
				//Parametry modyfikowane dynamicznie
				switch(DtKNF.PRM)
				{
					//moc startowa (0...99)
					case 0:
					{
						DtKNF.mstr=RSDT1.inDDTA;			//dana dynamiczna
						MocStart(DtKNF.mstr);
						DtKNF.wymCO=1;						//zalacz wymuszenie CO
						DtKNF.wymCW=0;						//wylacz wymuszenie CW
						break;
					}
					//moc maksymalna WU (0...99)
					case 1:		
					{
						DtKNF.mxcw=RSDT1.inDDTA;			//dana dynamiczna
						MocMaksCW(DtKNF.mxcw);
						DtKNF.wymCO=0;						//wylacz wymuszenie CO
						if(DtKNF.tpkt==_UNICO)				//kociol UniCO?
						{
								DtKNF.wymCW=1;				//zalacz wymuszenie CW niezaleznie od stanu czujnika przeplywu
						}
						else
						{
							if(RdPrt(S_WCW)) DtKNF.wymCW=1;	//zalacz wymuszenie CW dla aktywnego przeplywu
							else DtKNF.wymCW=0;
						}
						break;
					}
					//moc maksymalna WG (0...99)
					case 2:	
					{
						DtKNF.mxco=RSDT1.inDDTA;			//dana dynamiczna
						MocMaksCO(DtKNF.mxco);
						DtKNF.wymCO=1;						//zalacz wymuszenie CO
						DtKNF.wymCW=0;						//wylacz wymuszenie CW
						break;
					}
				}
				break;	
			}
			case 4:	//zapis/wyjscie z procedury
			{
				if(!bf_SAVE_PAR)
				{
 					bf_SAVE_PAR=SaveKnfPar();															//zapisz zaakceptowane parametry w pamieci EEPROM
				}
				break;	
			}
		}
	}
}
/*---------------------------------------------------------------------------------*/
//Aktualizacjia wartosci parametrow (dane pobierane z mod.LCD)
unsigned char RefKnfPar(void)
{
	if(RSDT1.RD_KNF)
	{
		RSDT1.RRD_KNF=1;			//zablokuj paczke danych
		DtKNF.mstr=RSDT1.inMSTR;
		DtKNF.mxcw=RSDT1.inMMAXU;
		DtKNF.mxco=RSDT1.inMMAXG;
		DtKNF.rdgz=RSDT1.inRGZ;	
		DtKNF.aleg=RSDT1.inWANL;
		DtKNF.tpkt=RSDT1.inTPKT;
		DtKNF.robg=RSDT1.inWOBG;
		DtKNF.opdg=RSDT1.inOPDG;
		DtKNF.tpcs=RSDT1.inTPCS;

		//kompatybilnosc wsteczna

		switch(RSDT1.vKNF)
		{
            case 5:                                             //(30)piata wersja paczki danych plyty LCD
                DtKNF.cwtm=RSDT1.inCWTM;
                DtKNF.cwmg=RSDT1.inCWMG;                
                DtKNF.pgdr=RSDT1.inPGDR;               
			case 4:												//(27)czwarta wersja paczki danych plyty LCD 
                DtKNF.pgmx=RSDT1.inPGMX;                
				DtKNF.prop=RSDT1.inPROP;                
			case 3:												//(25)trzecia wersja paczki danych plyty LCD
				DtKNF.hsco=RSDT1.inHSCO;
				DtKNF.conw=RSDT1.inCONW;
				DtKNF.mgpm2=RSDT1.inMGPM2;
			case 2:												//(21)druga wersja paczki danych plyty LCD
				DtKNF.rdpm=RSDT1.inRDPM;
				DtKNF.dtpm=RSDT1.inDTPM;
				DtKNF.mdpm=RSDT1.inMDPM;
				DtKNF.mgpm=RSDT1.inMGPM;
				if(DtKNF.mdpm>DtKNF.mgpm) DtKNF.mdpm=DtKNF.mgpm;
			case 1:												//(17)pierwsza wersja paczki danych plyty LCD
				break;						
		}

		DtKNF.teco=0;
		RSDT1.RRD_KNF=0;			//odblokuj paczke danych
		RSDT1.RD_KNF=0;
		return 1;
	}
	return 0;
}
/*---------------------------------------------------------------------------------*/
//Zapis parametrow do EEPROM
unsigned char SaveKnfPar(void)
{
	if(RefKnfPar())
	{
		if((DtKNF.mstr>MinMax[0].min)&&(DtKNF.mstr<MinMax[0].max)||DtKNF.mstr==MinMax[0].min||DtKNF.mstr==MinMax[0].max)
        {
            if_WriteEEPROM(_ADRMST,DtKNF.mstr);
        }
		if((DtKNF.mxcw>MinMax[1].min)&&(DtKNF.mxcw<MinMax[1].max)||DtKNF.mxcw==MinMax[1].min||DtKNF.mxcw==MinMax[1].max)
        {
            if_WriteEEPROM(_ADRMCW,DtKNF.mxcw);
        }

		if((DtKNF.mxco>MinMax[2].min)&&(DtKNF.mxco<MinMax[2].max)||DtKNF.mxco==MinMax[2].min||DtKNF.mxco==MinMax[2].max)
        {
            if_WriteEEPROM(_ADRMCO,DtKNF.mxco);
        }
		if((DtKNF.rdgz>MinMax[3].min)&&(DtKNF.rdgz<MinMax[3].max)||DtKNF.rdgz==MinMax[3].min||DtKNF.rdgz==MinMax[3].max)
        {
            if(DtKNF.rdgz!=ReadEEPROM(_ADRRGZ))
            {
                if_WriteEEPROM(_ADRRGZ,DtKNF.rdgz);
                ModParam(DtKNF.tpkt,DtKNF.rdgz);		//nowe parametry modulatora dla wybranego typu kotla i rodzaju gazu
            }
        }
        if((DtKNF.aleg>MinMax[4].min)&&(DtKNF.aleg<MinMax[4].max)||DtKNF.aleg==MinMax[4].min||DtKNF.aleg==MinMax[4].max)  
        {
            if(DtKNF.aleg!=ReadEEPROM(_ADRTAL))
            {
                StartRTG(_RTANL);
                if_WriteEEPROM(_ADRRTA,0);
                if_WriteEEPROM(_ADRTAL,DtKNF.aleg);
            }
        }
        if((DtKNF.tpkt>MinMax[5].min)&&(DtKNF.tpkt<MinMax[5].max)||DtKNF.tpkt==MinMax[5].min||DtKNF.tpkt==MinMax[5].max)
        {
            if(DtKNF.tpkt!=ReadEEPROM(_ADRWTK))
            {
                SetTpKtParam();							//ustaw param. ukryte dla wybr. kotla
                if((DtKNF.rdgz>MinMax[3].min)&&(DtKNF.rdgz<MinMax[3].max)||DtKNF.rdgz==MinMax[3].min||DtKNF.rdgz==MinMax[3].max)
                {
                    ModParam(DtKNF.tpkt,DtKNF.rdgz);		//nowe parametry modulatora dla wybranego typu kotla i rodzaju gazu
                }
                SetDefaultValue();						//parametry domy?lne dla danego typu kotla
                if_WriteEEPROM(_ADRWTK,DtKNF.tpkt);
            }
        }

        if((DtKNF.robg>MinMax[6].min)&&(DtKNF.robg<MinMax[6].max)||DtKNF.robg==MinMax[6].min||DtKNF.robg==MinMax[6].max)  
        {
            if_WriteEEPROM(_ADROBG,DtKNF.robg);
        }
        if((DtKNF.opdg>MinMax[7].min)&&(DtKNF.opdg<MinMax[7].max)||DtKNF.opdg==MinMax[7].min||DtKNF.opdg==MinMax[7].max)
        {            
            if_WriteEEPROM(_ADRPDG,DtKNF.opdg);
        }
        if((DtKNF.tpcs>MinMax[8].min)&&(DtKNF.tpcs<MinMax[8].max)||DtKNF.tpcs==MinMax[8].min||DtKNF.tpcs==MinMax[8].max)            
		{
            if_WriteEEPROM(_ADRCS5,DtKNF.tpcs);
        }
        if((DtKNF.rdpm>MinMax[9].min)&&(DtKNF.rdpm<MinMax[9].max)||DtKNF.rdpm==MinMax[9].min||DtKNF.rdpm==MinMax[9].max)            
		{
            if_WriteEEPROM(_ADRRDPM,DtKNF.rdpm);
        }
        if((DtKNF.dtpm>MinMax[10].min)&&(DtKNF.dtpm<MinMax[10].max)||DtKNF.dtpm==MinMax[10].min||DtKNF.dtpm==MinMax[10].max)            
		{
            if_WriteEEPROM(_ADRDLPM,DtKNF.dtpm);
        }
        if((DtKNF.mdpm>MinMax[11].min)&&(DtKNF.mdpm<MinMax[11].max)||DtKNF.mdpm==MinMax[11].min||DtKNF.mdpm==MinMax[11].max)             
        {
            if(DtKNF.mdpm>DtKNF.mgpm)
            {
                if(DtKNF.mgpm>=MinMax[11].min) DtKNF.mdpm=DtKNF.mgpm;
            }
            if_WriteEEPROM(_ADRMNPM,DtKNF.mdpm);
        }
        
        if((DtKNF.mgpm>MinMax[12].min)&&(DtKNF.mgpm<MinMax[12].max)||DtKNF.mgpm==MinMax[12].min||DtKNF.mgpm==MinMax[12].max)        
        {
            if(DtKNF.mgpm<DtKNF.mdpm)
            {
                if(DtKNF.mdpm<=MinMax[12].max) DtKNF.mgpm=DtKNF.mdpm;
            }
            if_WriteEEPROM(_ADRMXPM,DtKNF.mgpm);
        }

        if((DtKNF.mgpm2>MinMax[13].min)&&(DtKNF.mgpm2<MinMax[13].max)||DtKNF.mgpm2==MinMax[13].min||DtKNF.mgpm2==MinMax[13].max)        
        {
            if_WriteEEPROM(_ADRMXPM2,DtKNF.mgpm2);
        }

        if((DtKNF.hsco>MinMax[14].min)&&(DtKNF.hsco<MinMax[14].max)||DtKNF.hsco==MinMax[14].min||DtKNF.hsco==MinMax[14].max)         
        {
            if_WriteEEPROM(_ADRHSCO,DtKNF.hsco);
        }

        if((DtKNF.conw>MinMax[15].min)&&(DtKNF.conw<MinMax[15].max)||DtKNF.conw==MinMax[15].min||DtKNF.conw==MinMax[15].max)      
        {
            if_WriteEEPROM(_ADRCONW,DtKNF.conw);
        }

        if((DtKNF.prop>MinMax[16].min)&&(DtKNF.prop<MinMax[16].max)||DtKNF.prop==MinMax[16].min||DtKNF.prop==MinMax[16].max)
        {
            if_WriteEEPROM(_ADRPROP,DtKNF.prop);
        }
        
        if((DtKNF.pgmx>MinMax[17].min)&&(DtKNF.pgmx<MinMax[17].max)||DtKNF.pgmx==MinMax[17].min||DtKNF.pgmx==MinMax[17].max)
        {
            if_WriteEEPROM(_ADRPGMX,DtKNF.pgmx);
        }
        if((DtKNF.pgdr>MinMax[18].min)&&(DtKNF.pgdr<MinMax[18].max)||DtKNF.pgdr==MinMax[18].min||DtKNF.pgdr==MinMax[18].max)
        {
            if_WriteEEPROM(_ADRPGDR,DtKNF.pgdr);
        }        
        if((DtKNF.cwmg>MinMax[19].min)&&(DtKNF.cwmg<MinMax[19].max)||DtKNF.cwmg==MinMax[19].min||DtKNF.cwmg==MinMax[19].max)
        {
            if_WriteEEPROM(_ADRCWMG,DtKNF.cwmg);    //histereza wylaczenia CW(4F)              
        }        
        if((DtKNF.cwtm>MinMax[20].min)&&(DtKNF.cwtm<MinMax[20].max)||DtKNF.cwtm==MinMax[20].min||DtKNF.cwtm==MinMax[20].max)
        {
            if_WriteEEPROM(_ADRCWTM,DtKNF.cwtm);    //we. sygnalu timera zasobnika (1F)                  
        }
		DtKNF.teco=0;
		RSDT1.RRD_KNF=0;
		RSDT1.RD_KNF=0;
		return 1;
	}
	return 0;
}
/*---------------------------------------------------------------------------------*/
void SetMinMaxPCO(void)
{
    volatile unsigned char kn;
    
    if(!DtKNF.opdg)
    {
        DtCO.minPCO=_MIN_PCO;
        kn=DtKNF.pgmx;
        if(kn<_MIN_PCO) kn=_MIN_PCO;
        if(kn>_MAX_PCO) kn=_MAX_PCO;
        DtCO.maxPCO=kn;
        if(DtCO.maxPCO<DtCO.minPCO) DtCO.maxPCO=DtCO.minPCO;
        M.SETPmin=_MIN_PCO;
    }
    else
    {
        DtCO.minPCO=_MIN_PDCO;
        kn=DtKNF.pgmx;   
        if(kn<_MIN_PDCO) kn=_MIN_PDCO;
        if(kn>_MAX_PDCO) kn=_MAX_PDCO;            
        DtCO.maxPCO=kn;
        if(DtCO.maxPCO<DtCO.minPCO) DtCO.maxPCO=DtCO.minPCO;            
        M.SETPmin=_MIN_PDCO;            
    } 
	RSDT1.outPCOmn=DtCO.minPCO;
	RSDT1.outPCOmx=DtCO.maxPCO;
    RSDT1.outSETPmin=M.SETPmin;    
}
/*---------------------------------------------------------------------------------*/
void SetMinMaxPGD(void)
{
    DtCO.minPCO=_MIN_PGD;
    DtCO.maxPCO=_MAX_PGD;
    if(!DtKNF.opdg)
    {
        M.SETPmin=_MIN_PCO;
    }
    else
    {
        M.SETPmin=_MIN_PDCO;            
    }  
	RSDT1.outPCOmn=DtCO.minPCO;
	RSDT1.outPCOmx=DtCO.maxPCO;
    RSDT1.outSETPmin=M.SETPmin;    
}
/*---------------------------------------------------------------------------------*/
void ChkPCO(void)
{
	if(DtCO.PCO<DtCO.minPCO) DtCO.PCO=DtCO.minPCO;
	if(DtCO.PCO>DtCO.maxPCO) DtCO.PCO=DtCO.maxPCO;
    if_WriteEEPROM(_ADRPCO,DtCO.PCO);    
}
/*---------------------------------------------------------------------------------*/
void ChkPGD(void)
{
	if(DtCO.PCO<DtCO.minPCO) DtCO.PCO=DtCO.minPCO;
	if(DtCO.PCO>DtCO.maxPCO) DtCO.PCO=DtCO.maxPCO;
    if_WriteEEPROM(_ADRPPGD,DtCO.PCO);        
}
/*---------------------------------------------------------------------------------*/
//Dobor wartosci granicznych nastawy PCO
void SetNSTCO(void)
{
    volatile unsigned char i;

    i=0;
    while(i<10)                 //10 krotny pomiar stanu czujnuka zewnetrznego
    {
        CLRWDT();
        PomiarZW();				//temperatura z czujnika zewnetrznego
        i++;
    }
	if(!DtZW._on_ZW)            //brak czujnika zewnetrznego?
	{
        M._AKTYWNE_PGD=0;
        DtCO.PCO=ReadEEPROM(_ADRPCO);
        
        SetMinMaxPCO(); 
        ChkPCO();
	}
	else    //aktywny czujnik zewnetrzny
	{
		M._AKTYWNE_PGD=1;
        DtCO.PCO=ReadEEPROM(_ADRPPGD);
        
		SetPrt(S_PGD,1);
        SetMinMaxPGD();  
        ChkPGD();
        DtCO.PGDPCO=GetPGD_PCO();        
	}
    //dobierz wartosci M.CONTSETP, M.SETPmax, DtCO.PPCO
    SetModPCO();
}
/*---------------------------------------------------------------------------------*/
//Odczytuje i interpretuje parametry zapisane w pamieci EEPROM
void RdEEParam(void)
{
    volatile unsigned int dt;
	//-----
	DtKNF.tpkt=ReadEEPROM(_ADRWTK);						//typ kotla
	RSDT1.outTPKT=DtKNF.tpkt;
	SetTpKtParam();											//ustaw param. ukryte dla wybr. typu kotla
	//-----
	ModParam(DtKNF.tpkt,DtKNF.rdgz=ReadEEPROM(_ADRRGZ));	//parametry modulatora dla wybranego typu kotla i rodzaju gazu
	RSDT1.outRGZ=DtKNF.rdgz;
	MocStart(DtKNF.mstr=ReadEEPROM(_ADRMST));				//moc startowa, moc minimalna wstepnego wybiegu
	RSDT1.outMSTR=DtKNF.mstr;
	MocMaksCW(DtKNF.mxcw=ReadEEPROM(_ADRMCW));				//moc maksymalna CW
	RSDT1.outMMAXU=DtKNF.mxcw;
	MocMaksCO(DtKNF.mxco=ReadEEPROM(_ADRMCO));				//moc maksymalna CO
	RSDT1.outMMAXG=DtKNF.mxco;
	DtKNF.aleg=ReadEEPROM(_ADRTAL);						//aktywacja timera AL
	RSDT1.outWANL=DtKNF.aleg;
	DtKNF.robg=ReadEEPROM(_ADROBG);						//rodzaj obiegu 0-otwarty,1-zamkniety
	RSDT1.outWOBG=DtKNF.robg;
	PFN.poz=ReadEEPROM(_ADRPFN);							//pozycja PFN
	SetPFN();
	DtKNF.opdg=ReadEEPROM(_ADRPDG);						//aktywacja trybu ogrzewania podlogowego
	RSDT1.outOPDG=DtKNF.opdg;
	//DtCO.PCO=ReadEEPROM(_ADRPCO);							//nastawa PCO
	//DtCW.PCW=ReadEEPROM(_ADRPCW);							//nastawa PCW
  
	DtKNF.tpcs=ReadEEPROM(_ADRCS5);						//alternatywny czujnik cisnienia 5V
	RSDT1.outTPCS=DtKNF.tpcs;
	DtKNF.wymCO=0;											//wylacz wymuszenie CO
	DtKNF.wymCW=0;											//wylacz wymuszenie CW
	M.LICZNIK_ANL=ReadEEPROM(_ADRRTA);						//wartosc timera AL
		
	if(M.LICZNIK_ANL)
	{
		if(!DtKNF.aleg||(DtKNF.tpkt!=_UNICO))	//nieaktywna funkcja AL?
		{
			if_WriteEEPROM(_ADRRTA,0);						//wyzeruj licznik timera aktywacji AL
			M.LICZNIK_ANL=0;			
		}
	}
	//-----
	DtKNF.rdpm=ReadEEPROM(_ADRRDPM);						//rodzaj pompy
	RSDT1.outRDPM=DtKNF.rdpm;
	DtKNF.dtpm=ReadEEPROM(_ADRDLPM);						//wielkosc delty T dla pompy
	RSDT1.outDTPM=DtKNF.dtpm;
	DtKNF.mdpm=ReadEEPROM(_ADRMNPM);						//dolny prog predkosci dla pompy
	RSDT1.outMDPM=DtKNF.mdpm;
	DtKNF.mgpm=ReadEEPROM(_ADRMXPM);						//gorny prog predkosci dla pompy
	RSDT1.outMGPM=DtKNF.mgpm;
    DtKNF.mgpm2=ReadEEPROM(_ADRMXPM2);						//gorny prog predkosci dla pompy w obiegu CW
    RSDT1.outMGPM2=DtKNF.mgpm2;
	DtKNF.hsco=ReadEEPROM(_ADRHSCO);
	RSDT1.outHSCO=DtKNF.hsco;
	DtKNF.conw=ReadEEPROM(_ADRCONW);
	RSDT1.outCONW=DtKNF.conw;
	DtKNF.prop=ReadEEPROM(_ADRPROP);
	RSDT1.outPROP=DtKNF.prop;
    DtKNF.pgmx=ReadEEPROM(_ADRPGMX);
	RSDT1.outPGMX=DtKNF.pgmx;
    
    DtKNF.pgdr=ReadEEPROM(_ADRPGDR);
	RSDT1.outPGDR=DtKNF.pgdr;    
    DtKNF.cwmg=ReadEEPROM(_ADRCWMG);       //histereza wylaczenia CW(4F) 
	RSDT1.outCWMG=DtKNF.cwmg;    
    DtKNF.cwtm=ReadEEPROM(_ADRCWTM);       //we. sygnalu timera zasobnika (1F)    
	RSDT1.outCWTM=DtKNF.cwtm;
    
	DtKNF.teco=0;
#if (_EXT_BOARD>1) 
    DtKNF.alin=ReadEEPROM(_ADRALIN);                                               //adres odbiornika dla magistrali LIN
    //if((DtKNF.alin<1)||(DtKNF.alin>5)) DtKNF.alin=5;
    if((DtKNF.alin<1)||(DtKNF.alin>5&&DtKNF.alin<9)||(DtKNF.alin>12)) DtKNF.alin=5;    
#else
	DtKNF.alin=5;
#endif
    RSDT1.outALIN=DtKNF.alin;   
    //--------------------------------  
    DtCW.PCW=ReadEEPROM(_ADRPCW);							//nastawa PCW
    RSDT1.outPCW=DtCW.PCW;
	SetNSTCO();												//odczyt i formatowanie nastawy PCO, dobierz wartosci M.CONTSETP, M.SETPmax, DtCO.PPCO
    //M.SETPmax=DtCO.PCO;
    //M.CONTSETP=M.SETPmax;    
	RSDT1.outPCO=DtCO.PCO;
	RSDT1.outSETPmax=M.SETPmax;      
    
    M.RGDZ=GetRGDZ();
}

unsigned int GetRGDZ(void)
{
    volatile unsigned int dt,dtlo,dthi;   

    dtlo=(unsigned int)(ReadEEPROM(_ADRRLO));
    dtlo=dtlo&0x00FF;
    dthi=(unsigned int)(ReadEEPROM(_ADRRHI));
    dthi=dthi&0x00FF;    
    dt=(dthi<<8);
    dt=dt|dtlo;
    return dt;
}
void SetRGDZ(const unsigned int dt)
{
    volatile unsigned int dtlo,dthi;   
    dthi=((dt&0xFF00)>>8);
    dtlo=(dt&0x00FF);
    if_WriteEEPROM(_ADRRLO,dtlo);
    if_WriteEEPROM(_ADRRHI,dthi);    
}
/*---------------------------------------------------------------------------------*/
//Ustaw parametry ukryte dla wybranego typu kotla
void SetTpKtParam(void)
{
	switch(DtKNF.tpkt)
	{
		case _UNICO:		//zasobnikowy
		{
			DtKNF.czpr=_BRAK;
			//DtKNF.rdpm=_KDWA;
			DtKNF.tpwm=_TERMET;
			break;
		}
		case _BITERM:		//bitermiczny (miniterm)
		{
			DtKNF.czpr=_BRAK;
			//DtKNF.rdpm=_KJEDEN;
			DtKNF.tpwm=_TERMET;
			break;
		} 
		case _MONOHB:		//monotermiczny z hydroblokiem
		{
			DtKNF.czpr=_BRAK;
			//DtKNF.rdpm=_KDWA;
			DtKNF.tpwm=_TERMET;
			break;
		}
		case _BITERM2:		//bitermiczny (maxiterm)
		{
			DtKNF.czpr=_BRAK;
			//DtKNF.rdpm=_KJEDEN;
			DtKNF.tpwm=_TERMET;
			break;
		}
		default:			//domyslny monotermiczny
		{
			DtKNF.czpr=_BRAK;
			//DtKNF.rdpm=_KDWA;
			DtKNF.tpwm=_TERMET;
			break;
		}
	}
}
/*---------------------------------------------------------------------------------*/
//ustaw domyslne wartosci wybranych parametrów po zmianie typu kotla
void SetDefaultValue(void)					
{
	switch(DtKNF.tpkt)
	{
		case _MONOHB:		//monotermiczny z hydroblokiem
		{
			DtKNF.mstr=15;
			if_WriteEEPROM(_ADRMST,DtKNF.mstr);
			break;
		}
		default:
		{
			DtKNF.mstr=40;
			if_WriteEEPROM(_ADRMST,DtKNF.mstr);
			break;
		}
	}
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Decyzja o trybie pracy na podstawie pozycji przelacznika funkcji
void SetPFN(void)
{
	if(PFN.poz==_PFOFF)	
	{
		PFN._zima=0;
		PFN._lato=0;
		PFN._off=1;
		PFN._csn=0;
		InBitSet(&RSDT1.outSTAT2,_F_ZIMA,0);
		InBitSet(&RSDT1.outSTAT2,_F_LATO,0);
		InBitSet(&RSDT1.outSTAT2,_F_OFF,1);
	}
	else
	if(PFN.poz==_PFZMA)
	{
		PFN._zima=1;
		PFN._lato=0;
		PFN._off=0;
		PFN._csn=0;
		InBitSet(&RSDT1.outSTAT2,_F_ZIMA,1);
		InBitSet(&RSDT1.outSTAT2,_F_LATO,0);
		InBitSet(&RSDT1.outSTAT2,_F_OFF,0);
	}
	else
	if(PFN.poz==_PFLTO)	
	{
		PFN._zima=0;
		PFN._lato=1;
		PFN._off=0;
		PFN._csn=0;
		InBitSet(&RSDT1.outSTAT2,_F_ZIMA,0);
		InBitSet(&RSDT1.outSTAT2,_F_LATO,1);
		InBitSet(&RSDT1.outSTAT2,_F_OFF,0);
	}
	else
	{
		PFN._zima=1;
		PFN._lato=0;
		PFN._off=0;
		PFN._csn=0;
		PFN.poz=_PFZMA;
		InBitSet(&RSDT1.outSTAT2,_F_ZIMA,1);
		InBitSet(&RSDT1.outSTAT2,_F_LATO,0);
		InBitSet(&RSDT1.outSTAT2,_F_OFF,0);
	}
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Zapamietaj/odczytaj nastawe PCW
void WriteEEPCW(void)
{
    if_WriteEEPROM(_ADRPCW,DtCW.PCW);
}
unsigned char ReadEEPCW(void)
{
	return ReadEEPROM(_ADRPCW);
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Zapamietaj/odczytaj nastawe PCO
void WriteEEPCO(void)
{    
    if_WriteEEPROM(_ADRPCO,DtCO.PCO);
}
unsigned char ReadEEPCO(void)
{
	return ReadEEPROM(_ADRPCO);
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Zapamietaj/odczytaj nastawe PPGD
unsigned char ReadEEPPGD(void)
{
	return ReadEEPROM(_ADRPPGD);
}
void WriteEEPPGD(void)
{    
    if_WriteEEPROM(_ADRPPGD,DtCO.PCO);
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Zapamietaj/odczytaj pozycje przelacznika funkcyjnego
void WriteEEPFN(void)
{
	//if_WriteEEPROM(_ADRPFN,PFN.poz);
    unsigned char d1,d2;
    d1=ReadEEPROM(_ADRPFN);
    d1=d1<<4;                   //zapamietaj poprzednia pozycje na 4 najstarszych bitach
    d2=(PFN.poz&0b00001111);
    d1=d1|d2;
    if_WriteEEPROM(_ADRPFN,d1); 
}
//Aktualna pozycja przelacznika
unsigned char ReadEEPFN(void)
{
	//return ReadEEPROM(_ADRPFN);
    unsigned char d;
    d=ReadEEPROM(_ADRPFN);
    d=d&0b00001111;
	return d;
}
//Poprzednia pozycja przelacznika
unsigned char ReadEEPopPFN(void)
{
    unsigned char d;
    d=ReadEEPROM(_ADRPFN);
    d=d&0b11110000;
    d=d>>4;
	return d;
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Zapamietaj/odczytaj adres LIN plytki UNI-02
void WriteEEALIN(unsigned char d)
{
    if_WriteEEPROM(_ADRALIN,d);
}
unsigned char ReadEEALIN(void)
{
	return ReadEEPROM(_ADRALIN);
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Zeruj timer ANTYLEGIONELLA
void ClearEETA(void)
{
	if_WriteEEPROM(_ADRRTA,0);
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Odczyt bajtu zwiazanego z historia zdarzen awaryjnych
unsigned char ReadBtyHNPZ(void)
{
    return ReadEEPROM(_ADR_HNPZ);
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Odczyt stanu licznika flagi serwisowej
unsigned char ReadBtyHFLG(void)
{
    return ReadEEPROM(_ADR_HFLG);
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Odczyt bajtu zwiazanego z historia zdarzen awaryjnych
unsigned char ReadBtyHREF(const unsigned char ind)
{
    if(ind>1) return 0;
    return ReadEEPROM(_ADR_HRFL+ind);
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Odczyt bajtu zwiazanego z historia zdarzen awaryjnych
unsigned char ReadBtyEEHIST(const unsigned char ind)
{
    if(ind>_NHSPZ-1) return 0;
    return ReadEEPROM(_ADR_HBUF+ind);
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Zapis w EEPROM bajtu zwiazanego z historia zdarzen awaryjnych
/*void WriteBtyEEHIST(const unsigned char ind, const unsigned char dt)
{
    if(ind>_NHSPZ-1) return;
    if_if_WriteEEPROM(_ADR_HBUF+ind,dt);
}*/
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Odczyt liczby wylaczen awaryjnych z blokada
unsigned char ReadBtyNZSTAT(const unsigned char ind)
{
    if(ind>1) return 0;
    return ReadEEPROM(_ADRNZEE+ind);
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Zapisz w EEPROM liczbe wylaczen awaryjnych z blokada
void WriteBtyNZSTAT(const unsigned char ind, const unsigned char dt)
{
    if(ind>1) return;
    if_WriteEEPROM(_ADRNZEE+ind,dt);
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Odczyt bajtu zwiazanego ze statusem urzadzenia w momencie wylaczenia awaryjnego z blokada
//gdzie:
//nr-nr statusu
//ind - nr bajtu
unsigned char ReadBtySTAT(const unsigned char nr, const unsigned char ind)
{
    volatile unsigned int adr;
    
    if(nr>_NSTBL-1) return 0;
    if(ind>_NSTPZ-1) return 0;
    switch(nr)
    {
        case 0:
        {
            adr=_ADRSTEE0;
            break;
        }            
        case 1:
        {
            adr=_ADRSTEE1;            
            break;
        }            
        case 2:
        {
            adr=_ADRSTEE2;            
            break;
        }            
        case 3:
        {
            adr=_ADRSTEE3;            
            break;
        }
    }    
    return ReadEEPROM(adr+ind);
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Zapis bajtu zwiazanego ze statusem urzadzenia w momencie wylaczenia awaryjnego z blokada
//gdzie:
//nr-nr statusu
//ind - nr bajtu
void WriteBtySTAT(const unsigned char nr, const unsigned char ind, const unsigned char dt)
{
    volatile unsigned int adr;
    
    if(nr>_NSTBL-1) return;
    if(ind>_NSTPZ-1) return;
    switch(nr)
    {
        case 0:
        {
            adr=_ADRSTEE0;
            break;
        }            
        case 1:
        {
            adr=_ADRSTEE1;            
            break;
        }            
        case 2:
        {
            adr=_ADRSTEE2;            
            break;
        }            
        case 3:
        {
            adr=_ADRSTEE3;            
            break;
        }
    }    
    if_WriteEEPROM(adr+ind,dt);
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Zapisz ststus urzadzenia w momencie wylaczenia awaryjnego z blokada
//gdzie:
//nr-nr statusu
void WriteEESTAT(const unsigned char nr)
{
    volatile unsigned int d;
    volatile unsigned char e;
    
    if(nr>_NSTBL-1) return;

    //--------------------------------------
    //Liczba wylaczen awaryjnych z blokada
    d=ReadBtyNZSTAT(0)+(ReadBtyNZSTAT(1)*256); //liczba wylaczen z blokada  
    if(d<0xffff) d++;
    else d=1;
    
    e=(unsigned char)(d&0x00ff);
    WriteBtyNZSTAT(0,e);
    e=(unsigned char)((d&0xff00)>>8);    
    WriteBtyNZSTAT(1,e);
    //---------------------------------------
    //Nr zdarzenia
    e=ReadBtyHREF(0);
    WriteBtySTAT(nr,0,e);
    e=ReadBtyHREF(1);
    WriteBtySTAT(nr,1,e);
    //Dane    
    e=RSDT1.outSTAT0;
    e|=0b00000001;      //ustaw znacznik blokady trwalej
    if(DtKNF.tpkt==_UNICO)
    {
        e|=0b00000010;      //ustaw znacznik kotla 1F
    }
    else
    {
        e&=0b11111101;      //zeruj znacznik kotla 1F
    }     
    WriteBtySTAT(nr,2,e);
    WriteBtySTAT(nr,3,RSDT1.outSTAT1);
    WriteBtySTAT(nr,4,RSDT1.outSTAT2);
    WriteBtySTAT(nr,5,RSDT1.outSTAT3);   
    WriteBtySTAT(nr,6,RSDT1.outSTAT4);
    WriteBtySTAT(nr,7,RSDT1.outSTAT5);            
    WriteBtySTAT(nr,8,RSDT1.outrCO);			//filtrowana wartosc temp
    WriteBtySTAT(nr,9,RSDT1.outrCW);			//filtrowana wartosc temp
    WriteBtySTAT(nr,10,RSDT1.outCS);
    WriteBtySTAT(nr,11,RSDT1.outIN);
    WriteBtySTAT(nr,12,RSDT1.outPCO);
    WriteBtySTAT(nr,13,RSDT1.outPCW);
    WriteBtySTAT(nr,14,RSDT1.outPPCO);
    WriteBtySTAT(nr,15,RSDT1.outPPCW);
    WriteBtySTAT(nr,16,RSDT1.outERR);
    WriteBtySTAT(nr,17,RSDT1.outAWR);
    WriteBtySTAT(nr,18,RSDT1.outPP);
    WriteBtySTAT(nr,19,RSDT1.outPG);
    WriteBtySTAT(nr,20,M.CONTSETP);    
    WriteBtySTAT(nr,21,RSDT1.outZW);			  
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Kopiuj ststus nr 0 urzadzenia do statusu nr1 (stat(nr1)=stat(nr0))
//gdzie:
//nr0-nr statusu do skopiowania
//nr1-nr ststusu do nadpisania
void CopyEESTAT(const unsigned char nr0,const unsigned char nr1)
{
    volatile unsigned char i;
    if((nr0>_NSTBL-1)||(nr1>_NSTBL-1)) return;
    for(i=0;i<_NSTPZ;i++)
    {
        WriteBtySTAT(nr1,i, ReadBtySTAT(nr0,i));
    }			  
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
#endif