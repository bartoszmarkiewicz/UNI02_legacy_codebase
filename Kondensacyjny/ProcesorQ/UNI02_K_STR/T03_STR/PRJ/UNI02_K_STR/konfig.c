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

#if _TYP_PRJ==1
#advisory _TYP_PRJ==1 
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
extern DataIN DtIN;
extern unsigned char xxy,zzz,vvv,mmm;

DtKONF DtKNF;

//Dozwolone wartosci graniczne wszystkich parametrow
DtMINMAX MinMax[_KNPAR1]=
{
	_ADRMST, _KMINMST, _KMAXMST,				//0 moc startowa
	_ADRMCW, _KMINMCW, _KMAXMCW,				//1 moc maksymalna CW
	_ADRMCO, _KMINMCO, _KMAXMCO, 				//2 moc maksymalna CO
	_ADRMDG, _KMINVMD, _KMAXVMD,				//3 dolna granica predkosci obrotowej wentylatora
	_ADRMGG, _KMINVMG, _KMAXVMG,				//4 gorna granica predkosci obrotowej wentylatora

	_ADRWKK, _KMINKK,  _KMAXKK,					//5 kod kotla

	_ADROBG, _KMINOBG, _KMAXOBG,				//6 rodzaj obiegu
	_ADRPDG, _KMINOGP, _KMAXOGP,				//7 ogrzewanie podlogowe
	_ADRCS5, _KMINTPC, _KMAXTPC,				//8 wybor rodzaju czujnika cisnienia
	_ADRTAL, _KMINEAL, _KMAXEAL,				//9 aktywacja zegara antylegionella

	_ADRNOK, _KMINNOK, _KMAXNOK,				//10 tachometr - liczba okresow na sek.
	_ADRRDPM, _KMINRDP,_KMAXRDP,				//11 rodzaj pompy
	_ADRDLPM, _KMINDLP,_KMAXDLP,				//12 delta T dla pompy
	_ADRMNPM, _KMINMNP,_KMAXMNP,				//13 minimalny wydatek pompy
	_ADRTECO, _KMINTEC,_KMAXTEC,				//14 aktywacja/deaktywacja trybu ECO
	_ADRTMCO2,_KMINTMCO2,_KMAXTMCO2,          	//15 czas pracy przy obnizonej mocy w trybie CO/deaktywacja dla wartosci 0
	_ADRMCO2, _KMINMCO2,_KMAXMCO2,				//16 wartosc obnizonej mocy w trybie CO
	_ADRMXPM, _KMINMXP,_KMAXMXP,				//17 maksymalny wydatek pompy
	_ADRMXPM2,_KMINMXP2,_KMAXMXP2,				//18 maksymalny wydatek pompy dla obiegu CW

	_ADRHSCO,_KMINHSCO,_KMAXHSCO,				//19 histereza wylaczenia grzania CO (ponad nastawe)
	_ADRCONW,_KMINCONW,_KMAXCONW,				//20 temp CO przy grzaniu CW zasobnika
	_ADRPROP,_KMINPROP,_KMAXPROP,				//21 poprawka dla regulatora pogodowego
	_ADRRFAN,_KMINRFAN,_KMAXRFAN,				//22 rodzaj wentylatora    
	_ADRTSTR,_KMINTSTR,_KMAXTSTR,				//23 czas wybiegu na mocy startowej w trakcie grzania CO 
	_ADRTL3, _KMINTL3, _KMAXTL3,				//24 czas blokady L3 dla obiegu CO    
    
	_ADRPGDR, _KMINPGDR, _KMAXPGDR,				//25 rodzaj regulatora pogodowego
	_ADRPGDG, _KMINPGDG, _KMAXPGDG,				//26 wartosc graniczna pobudzenia od regulatora pogodowego
	_ADRPGDO, _KMINPGDO, _KMAXPGDO,				//27 obnizenie nocne dla regulatora pogodowego
	_ADRPGMX, _KMINPGMX, _KMAXPGMX,				//28 maksymalna wartosc graniczna CO dla regulatora pogodowego 
	_ADRCWMG, _KMINCWMG, _KMAXCWMG,				//29 histereza wylaczenia CW(2F) 
	_ADRCWTM, _KMINCWTM, _KMAXCWTM,				//30 we. sygnalu timera zasobnika (1F)  
	_ADRPWTM, _KMINPWTM, _KMAXPWTM,				//31 przewyzszenie temperaturowe    
    
	_ADRRCCW, _KMINCCW, _KMAXCCW,				//32 rodzaj czujnika przeplywu CW (0-stykowy, 1-impulsowy) 
	_ADRTWPM, _KMINTPM, _KMAXTPM,				//33 czas wybiegu pompy po grzaniu obiegu CO     
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
//Konwersja danej typu float na dwa bajty danej 16bit typu F88
signed int FDataToF88(float dt)
{
    if(dt>=128) dt=127.99;
    if(dt<=-128) dt=-127.99;
    return (signed int)(dt*256);
}
void FDataToBtyF88(const float dt,volatile unsigned char *lo,volatile unsigned char *hi)
{
    volatile signed int sd;
    sd=FDataToF88(dt);
    *lo=(unsigned char)(sd&0x00FF);
    *hi=(unsigned char)((sd&0xFF00)>>8);
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
 unsigned char SaveKNFPar(void)        //%%%%
{
    volatile unsigned char svp;
    
    M._KNF_KAL=0;                                   //domyslnie zeruj znacznik wymuszenia restartu sterownika celem przeprowadzenia ponownej kalibracji wentylatora
    if_WriteEEPROM(_ADRKRES,0);                    //domyslnie zeruj flage wymuszonej kalibracji wentylatora po programowym restarcie oprogramowania
    svp=SaveKnfPar();                       //zapisz zaakceptowane parametry w pamieci EEPROM
    if(svp)
    {
        if((M.STRB==2)||(RSDT1.vKNF<9)) M._KNF_KAL=0;   //produkcyjny TRYB SPECJALNY lub wersja programu plytki wyswietlacza jest < 12?
        if(M._KNF_KAL)
        {
            if_WriteEEPROM(_ADRKRES,1);                //ustaw flage wymuszonej kalibracji wentylatora po programowym restarcie oprogramowania
            InBitSet(&RSDT1.outSTAT2,_KALIB,1);			//ustaw znacznik aktywnosci procedury kalibracji
            while(1);                                   //wymuszenie restartu plytki sterownika
        }
    }
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
	if((bf_AKTYWNE_KNF&&!M._AKTYWNE_KNF)||!MGSD._CON) 							//odczytaj parametry po wyjsciu z procedury konfiguracji
	{
		bf_AKTYWNE_KNF=0;
		DtKNF.wymCO=0;															//wylacz wymuszenie CO
		DtKNF.wymCW=0;															//wylacz wymuszenie CO
        //------------------------ %%%%
        if(MGSD._CON&&DtKNF.KRK==5)
        {
            DtKNF.KRK=0;
            bf_SAVE_PAR=SaveKNFPar();
        }
        //------------------------
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
//		DtKNF.KRK=RSDT1.inKRK;								//krok procedury konfiguracji
//		RSDT1.outKRK=DtKNF.KRK;								//informacja zwrotna o aktualnym kroku
        
        if(DtKNF.KRK!=5)        //%%%%
        {
            DtKNF.KRK=RSDT1.inKRK;								//krok procedury konfiguracji
            RSDT1.outKRK=DtKNF.KRK;								//informacja zwrotna o aktualnym kroku    
        }
        else
        {
            RSDT1.outKRK=4;								//informacja zwrotna o aktualnym kroku
        }
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
						DtKNF.dmstr=RSDT1.inDDTA;			//dana dynamiczna
						DtKNF.wymCO=1;						//zalacz wymuszenie CO
						DtKNF.wymCW=0;						//wylacz wymuszenie CW
						break;
					}
					//moc maksymalna WU (0...99)
					case 1:		
					{
						DtKNF.dmxcw=RSDT1.inDDTA;			//dana dynamiczna
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
						DtKNF.dmxco=RSDT1.inDDTA;			//dana dynamiczna
						DtKNF.wymCO=1;						//zalacz wymuszenie CO
						DtKNF.wymCW=0;						//wylacz wymuszenie CW
						break;
					}
				}
				break;	
			}
//			case 4:	//zapis/wyjscie z procedury
//			{
//				if(!bf_SAVE_PAR)
//				{
//                    M._KNF_KAL=0;                                   //domyslnie zeruj znacznik wymuszenia restartu sterownika celem przeprowadzenia ponownej kalibracji wentylatora
//                    if_WriteEEPROM(_ADRKRES,0);                    //domyslnie zeruj flage wymuszonej kalibracji wentylatora po programowym restarcie oprogramowania
// 					bf_SAVE_PAR=SaveKnfPar();                       //zapisz zaakceptowane parametry w pamieci EEPROM
//                    if((M.STRB==2)||(RSDT1.vKNF<9)) M._KNF_KAL=0;   //produkcyjny TRYB SPECJALNY lub wersja programu plytki wyswietlacza jest < 12?
//                    if(M._KNF_KAL)
//                    {
//                        if_WriteEEPROM(_ADRKRES,1);                //ustaw flage wymuszonej kalibracji wentylatora po programowym restarcie oprogramowania
//                        InBitSet(&RSDT1.outSTAT2,_KALIB,1);			//ustaw znacznik aktywnosci procedury kalibracji
//                        while(1);                                   //wymuszenie restartu plytki sterownika
//                    }
//				}
//				break;	
//			}
			case 4:	//zapis/wyjscie z procedury %%%%
			{    
                DtKNF.KRK=5;
                break;
            }  
			case 5:	//zapis/wyjscie z procedury %%%%
			{    
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
		DtKNF.def_mxco=DtKNF.mxco;
		DtKNF.opdg=RSDT1.inOPDG;
		DtKNF.aleg=RSDT1.inWANL;
		if(RSDT1.inKODKT!=DtKNF.kodkt)
		{
			DtKNF.kodkt=RSDT1.inKODKT;
			SetTpKtParam();					//ustaw param. ukryte dla wybr. kotla
			ModParam();						//oblicz parametry kluczowe modulatora
		}	
		DtKNF.robg=RSDT1.inWOBG;
		DtKNF.nokr=RSDT1.inNOKR;
		DtKNF.tpcs=RSDT1.inTPCS;
		if(DtKNF.kodkt<3)
		{
			if(RSDT1.inMMIN!=DtKNF.mmin)
			{
				DtKNF.mmin=RSDT1.inMMIN;		//swobodny dobor wartosci granicznych modulatora
				ModParam();						//oblicz parametry kluczowe modulatora
			}
			if(RSDT1.inMMAX!=DtKNF.mmax)
			{
				DtKNF.mmax=RSDT1.inMMAX;		//swobodny dobor wartosci granicznych modulatora
				ModParam();						//oblicz parametry kluczowe modulatora
			}
		}
		DtKNF.rdpm=RSDT1.inRDPM;
		DtKNF.dtpm=RSDT1.inDTPM;
		DtKNF.mdpm=RSDT1.inMDPM;
		DtKNF.teco=RSDT1.inTECO;

		//kompatybilnosc wsteczna
		switch(RSDT1.vKNF)
		{
            case 11:                                            //(43)jedenasta wersja paczki danych plyty LCD  
                DtKNF.tpcw=RSDT1.inTPCW;         
                DtKNF.tmpm=RSDT1.inTMPM;                
            case 10:                                            //(41)dziesiata wersja paczki danych plyty LCD  
                DtKNF.pwtm=RSDT1.inPWTM;                
            case 9:                                             //(40)dziewiata wersja paczki danych plyty LCD 
                DtKNF.cwtm=RSDT1.inCWTM; 
                DtKNF.cwmg=RSDT1.inCWMG;                
            case 8:                                             //(38)osma wersja paczki danych plyty LCD 
                DtKNF.pgmx=RSDT1.inPGMX;                
            case 7:                                             //(37)siodma wersja paczki danych plyty LCD
                DtKNF.pgdr=RSDT1.inPGDR;
                DtKNF.pgdg=RSDT1.inPGDG;
                DtKNF.pgdo=RSDT1.inPGDO;
                DtKNF.tstr=RSDT1.inTSTR;
                DtKNF.tL3=RSDT1.inTL3;                             
			case 6:												//(31)szusta wersja paczki danych plyty LCD
                if(RSDT1.inRFAN!=DtKNF.rfan)
                {
                    DtKNF.rfan=RSDT1.inRFAN;
                    ModParam();                                 //oblicz parametry kluczowe modulatora
                }                           
			case 5:												//(30)piata wersja paczki danych plyty LCD
				DtKNF.hsco=RSDT1.inHSCO;
				DtKNF.conw=RSDT1.inCONW;
				DtKNF.prop=RSDT1.inPROP;
			case 4:												//(27)czwarta wersja paczki danych plyty LCD
				DtKNF.mgpm2=RSDT1.inMGPM2;
			case 3:												//(26)trzecia wersja paczki danych plyty LCD
				DtKNF.mgpm=RSDT1.inMGPM;
				if(DtKNF.mdpm>DtKNF.mgpm) DtKNF.mdpm=DtKNF.mgpm;
			case 2:												//(25)druga wersja paczki danych plyty LCD
				DtKNF.tmxco2=RSDT1.inTMMAXG2;
				DtKNF.mxco2=RSDT1.inMMAXG2;
			case 1:												//(23)pierwsza wersja paczki danych plyty LCD
				break;						
		}
        
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

		if(DtKNF.kodkt<3)
		{
			if((DtKNF.mmin>MinMax[3].min)&&(DtKNF.mmin<MinMax[3].max)||DtKNF.mmin==MinMax[3].min||DtKNF.mmin==MinMax[3].max)
            {
                if(DtKNF.mmin!=ReadEEPROM(_ADRMDG))
                {
                    if_WriteEEPROM(_ADRMDG,DtKNF.mmin);
                    ModParam();								//oblicz parametry kluczowe modulatora
                }
            }
			if((DtKNF.mmax>MinMax[4].min)&&(DtKNF.mmax<MinMax[4].max)||DtKNF.mmax==MinMax[4].min||DtKNF.mmax==MinMax[4].max)
            {
                if(DtKNF.mmax!=ReadEEPROM(_ADRMGG))
                {
                    if_WriteEEPROM(_ADRMGG,DtKNF.mmax);
                    ModParam();								//oblicz parametry kluczowe modulatora
                }
            }
		}

		if((DtKNF.kodkt>MinMax[5].min)&&(DtKNF.kodkt<MinMax[5].max)||DtKNF.kodkt==MinMax[5].min||DtKNF.kodkt==MinMax[5].max)
        {
            if(DtKNF.kodkt!=ReadEEPROM(_ADRWKK))		//zmieniono typ kotla
            {
                SetTpKtParam();							//ustaw param. ukryte dla wybr. kotla
                ModParam();								//oblicz parametry kluczowe modulatora
                if_WriteEEPROM(_ADRWKK,DtKNF.kodkt);
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

		if((DtKNF.aleg>MinMax[9].min)&&(DtKNF.aleg<MinMax[9].max)||DtKNF.aleg==MinMax[9].min||DtKNF.aleg==MinMax[9].max)
        {
            if(DtKNF.aleg!=ReadEEPROM(_ADRTAL))
            {
                StartRTG(_RTANL);
                if_WriteEEPROM(_ADRRTA,0);
                if_WriteEEPROM(_ADRTAL,DtKNF.aleg);
            }
        }

		if((DtKNF.nokr>MinMax[10].min)&&(DtKNF.nokr<MinMax[10].max)||DtKNF.nokr==MinMax[10].min||DtKNF.nokr==MinMax[10].max)
        {
            if_WriteEEPROM(_ADRNOK,DtKNF.nokr);
        }

		if((DtKNF.rdpm>MinMax[11].min)&&(DtKNF.rdpm<MinMax[11].max)||DtKNF.rdpm==MinMax[11].min||DtKNF.rdpm==MinMax[11].max)
        {
            if_WriteEEPROM(_ADRRDPM,DtKNF.rdpm);
        }

		if((DtKNF.dtpm>MinMax[12].min)&&(DtKNF.dtpm<MinMax[12].max)||DtKNF.dtpm==MinMax[12].min||DtKNF.dtpm==MinMax[12].max)
        {
            if_WriteEEPROM(_ADRDLPM,DtKNF.dtpm);
        }

		if((DtKNF.mdpm>MinMax[13].min)&&(DtKNF.mdpm<MinMax[13].max)||DtKNF.mdpm==MinMax[13].min||DtKNF.mdpm==MinMax[13].max)
        {
            if(DtKNF.mdpm>DtKNF.mgpm)
            {
                if(DtKNF.mgpm>=MinMax[13].min) DtKNF.mdpm=DtKNF.mgpm;
            }
            if_WriteEEPROM(_ADRMNPM,DtKNF.mdpm);
        }

		if((DtKNF.teco>MinMax[14].min)&&(DtKNF.teco<MinMax[14].max)||DtKNF.teco==MinMax[14].min||DtKNF.teco==MinMax[14].max)
        {
            if_WriteEEPROM(_ADRTECO,DtKNF.teco);
        }

		if((DtKNF.tmxco2>MinMax[15].min)&&(DtKNF.tmxco2<MinMax[15].max)||DtKNF.tmxco2==MinMax[15].min||DtKNF.tmxco2==MinMax[15].max)
        {
            if_WriteEEPROM(_ADRTMCO2,DtKNF.tmxco2);
        }

		if((DtKNF.mxco2>MinMax[16].min)&&(DtKNF.mxco2<MinMax[16].max)||DtKNF.mxco2==MinMax[16].min||DtKNF.mxco2==MinMax[16].max)
        {
            if_WriteEEPROM(_ADRMCO2,DtKNF.mxco2);
        }

        if((DtKNF.mgpm>MinMax[17].min)&&(DtKNF.mgpm<MinMax[17].max)||DtKNF.mgpm==MinMax[17].min||DtKNF.mgpm==MinMax[17].max)
        {
            if(DtKNF.mgpm<DtKNF.mdpm)
            {
                if(DtKNF.mdpm<=MinMax[17].max) DtKNF.mgpm=DtKNF.mdpm;
            }
            if_WriteEEPROM(_ADRMXPM,DtKNF.mgpm);
        }

        if((DtKNF.mgpm2>MinMax[18].min)&&(DtKNF.mgpm2<MinMax[18].max)||DtKNF.mgpm2==MinMax[18].min||DtKNF.mgpm2==MinMax[18].max)
        {
            if_WriteEEPROM(_ADRMXPM2,DtKNF.mgpm2);
        }

		if((DtKNF.hsco>MinMax[19].min)&&(DtKNF.hsco<MinMax[19].max)||DtKNF.hsco==MinMax[19].min||DtKNF.hsco==MinMax[19].max)
        {
            if_WriteEEPROM(_ADRHSCO,DtKNF.hsco);
        }

        if((DtKNF.conw>MinMax[20].min)&&(DtKNF.conw<MinMax[20].max)||DtKNF.conw==MinMax[20].min||DtKNF.conw==MinMax[20].max)
        {
            if_WriteEEPROM(_ADRCONW,DtKNF.conw);
        }

        if((DtKNF.prop>MinMax[21].min)&&(DtKNF.prop<MinMax[21].max)||DtKNF.prop==MinMax[21].min||DtKNF.prop==MinMax[21].max)
        {
            if_WriteEEPROM(_ADRPROP,DtKNF.prop);
        }
        
        if((DtKNF.rfan>MinMax[22].min)&&(DtKNF.rfan<MinMax[22].max)||DtKNF.rfan==MinMax[22].min||DtKNF.rfan==MinMax[22].max)
        {
            if(DtKNF.rfan!=ReadEEPROM(_ADRRFAN))
            {
                if_WriteEEPROM(_ADRRFAN,DtKNF.rfan);
                SetDefFan(DtKNF.rfan);
                M._KNF_KAL=1;                           //flaga wymuszonej kalibracji wentylatora po programowym restarcie oprogramowania
            }
            ModParam();                                 //oblicz parametry kluczowe modulatora
        }  
        if((DtKNF.tstr>MinMax[23].min)&&(DtKNF.tstr<MinMax[23].max)||DtKNF.tstr==MinMax[23].min||DtKNF.tstr==MinMax[23].max)
        {
            if_WriteEEPROM(_ADRTSTR,DtKNF.tstr);
        } 
        if((DtKNF.tL3>MinMax[24].min)&&(DtKNF.tL3<MinMax[24].max)||DtKNF.tL3==MinMax[24].min||DtKNF.tL3==MinMax[24].max)
        {
            if_WriteEEPROM(_ADRTL3,DtKNF.tL3);
        }   
        
        if((DtKNF.pgdr>MinMax[25].min)&&(DtKNF.pgdr<MinMax[25].max)||DtKNF.pgdr==MinMax[25].min||DtKNF.pgdr==MinMax[25].max)
        {
            if_WriteEEPROM(_ADRPGDR,DtKNF.pgdr);
        }  
        if((DtKNF.pgdg>MinMax[26].min)&&(DtKNF.pgdg<MinMax[26].max)||DtKNF.pgdg==MinMax[26].min||DtKNF.pgdg==MinMax[26].max)
        {
            if_WriteEEPROM(_ADRPGDG,DtKNF.pgdg);
        }   
        if((DtKNF.pgdo>MinMax[27].min)&&(DtKNF.pgdo<MinMax[27].max)||DtKNF.pgdo==MinMax[27].min||DtKNF.pgdo==MinMax[27].max)
        {
            if_WriteEEPROM(_ADRPGDO,DtKNF.pgdo);
        }
        if((DtKNF.pgmx>MinMax[28].min)&&(DtKNF.pgmx<MinMax[28].max)||DtKNF.pgmx==MinMax[28].min||DtKNF.pgmx==MinMax[28].max)
        {
            if_WriteEEPROM(_ADRPGMX,DtKNF.pgmx);
        } 
        if((DtKNF.cwmg>MinMax[29].min)&&(DtKNF.cwmg<MinMax[29].max)||DtKNF.cwmg==MinMax[29].min||DtKNF.cwmg==MinMax[29].max)
        {
            if_WriteEEPROM(_ADRCWMG,DtKNF.cwmg);    //histereza wylaczenia CW(2F)              
        }        
        if((DtKNF.cwtm>MinMax[30].min)&&(DtKNF.cwtm<MinMax[30].max)||DtKNF.cwtm==MinMax[30].min||DtKNF.cwtm==MinMax[30].max)
        {
            if_WriteEEPROM(_ADRCWTM,DtKNF.cwtm);    //we. sygnalu timera zasobnika (1F)                  
        }    
        if((DtKNF.pwtm>MinMax[31].min)&&(DtKNF.pwtm<MinMax[31].max)||DtKNF.pwtm==MinMax[31].min||DtKNF.pwtm==MinMax[31].max)
        {
            if_WriteEEPROM(_ADRPWTM,DtKNF.pwtm);    //przewyzszenie temperaturowe                  
        }    
        if((DtKNF.tpcw>MinMax[32].min)&&(DtKNF.tpcw<MinMax[32].max)||DtKNF.tpcw==MinMax[32].min||DtKNF.tpcw==MinMax[32].max)
        {
            if_WriteEEPROM(_ADRRCCW,DtKNF.tpcw);    //rodzaj czujnika przeplywu CW     
            SetParPrzCw();
            InBitSet(&RSDT1.outKSTAT0,_CWIMP,((DtKNF.tpkt!=_UNICO)&&(DtKNF.tpcw!=0))?1:0);  //rodzaj czujnika przeplywu CW (0-stykowy, 1-impulsowy)
        }        
        if((DtKNF.tmpm>MinMax[33].min)&&(DtKNF.tmpm<MinMax[33].max)||DtKNF.tmpm==MinMax[33].min||DtKNF.tmpm==MinMax[33].max)
        {
            if_WriteEEPROM(_ADRTWPM,DtKNF.tmpm);    //czas wybiegu pompy CO                  
        }        
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
    
    switch(DtKNF.opdg)
    {
        case 0:
        {
            DtCO.minPCO=_MIN_PCO;
            kn=DtKNF.pgmx;
            if(kn<_MIN_PCO) kn=_MIN_PCO;
            if(kn>_MAX_PCO) kn=_MAX_PCO;
            DtCO.maxPCO=kn;
            if(DtCO.maxPCO<DtCO.minPCO) DtCO.maxPCO=DtCO.minPCO;
            M.SETPmin=_MIN_PCO;            
            break;
        }
        case 1:
        {
            DtCO.minPCO=_MIN_PDCO;
            kn=DtKNF.pgmx;   
            if(kn<_MIN_PDCO) kn=_MIN_PDCO;
            if(kn>_MAX_PDCO) kn=_MAX_PDCO;            
            DtCO.maxPCO=kn;
            if(DtCO.maxPCO<DtCO.minPCO) DtCO.maxPCO=DtCO.minPCO;            
            M.SETPmin=_MIN_PDCO;            
            break;
        } 
        case 2:
        {
            DtCO.minPCO=_MIN_PDCO;
            kn=DtKNF.pgmx;
            if(kn<_MIN_PDCO) kn=_MIN_PDCO;
            if(kn>_MAX_PCO) kn=_MAX_PCO;
            DtCO.maxPCO=kn;
            if(DtCO.maxPCO<DtCO.minPCO) DtCO.maxPCO=DtCO.minPCO;
            M.SETPmin=_MIN_PDCO;            
            break;
        }        
    }    
    /*if(!DtKNF.opdg)
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
    }*/  
	RSDT1.outPCOmn=DtCO.minPCO;
	RSDT1.outPCOmx=DtCO.maxPCO;
    RSDT1.outSETPmin=M.SETPmin;     
}
/*---------------------------------------------------------------------------------*/
void SetMinMaxPGD(void)
{
    DtCO.minPCO=_MIN_PGD;
    DtCO.maxPCO=_MAX_PGD;
    switch(DtKNF.opdg)
    {
        case 0:
        {
            M.SETPmin=_MIN_PCO;            
            break;
        }
        case 1:
        {
            M.SETPmin=_MIN_PDCO;          
            break;
        } 
        case 2:
        {
            M.SETPmin=_MIN_PDCO;            
            break;
        }        
    }    
    /*if(!DtKNF.opdg)
    {
        M.SETPmin=_MIN_PCO;
    }
    else
    {
        M.SETPmin=_MIN_PDCO;            
    }*/   
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
    volatile float dt;
	//-----
	DtKNF.kodkt=ReadEEPROM(_ADRWKK);						//kod kotla
	RSDT1.outKODKT=DtKNF.kodkt;
	SetTpKtParam();											//ustaw param. kluczowe dla wybr. typu kotla
	//-----
	DtKNF.mstr=ReadEEPROM(_ADRMST);						//moc startowa
	DtKNF.dmstr=DtKNF.mstr;
	RSDT1.outMSTR=DtKNF.mstr;
	DtKNF.mxcw=ReadEEPROM(_ADRMCW);						//moc maksymalna CW
	DtKNF.dmxcw=DtKNF.mxcw;
	RSDT1.outMMAXU=DtKNF.mxcw;
	DtKNF.mxco=ReadEEPROM(_ADRMCO);						//moc maksymalna CO	
	DtKNF.dmxco=DtKNF.mxco;	
	DtKNF.def_mxco=DtKNF.mxco;		
	RSDT1.outMMAXG=DtKNF.mxco;
	DtKNF.tmxco2=ReadEEPROM(_ADRTMCO2);					//czas pracy przy obnizonej mocy w trybie CO
	RSDT1.outTMMAXG2=DtKNF.tmxco2;
	DtKNF.mxco2=ReadEEPROM(_ADRMCO2);						//wartosc obnizonej mocy w trybie CO
	RSDT1.outMMAXG2=DtKNF.mxco2;
    
	DtKNF.rfan=ReadEEPROM(_ADRRFAN);                       //rodzaj wentylatora
	RSDT1.outRFAN=DtKNF.rfan;    
    
	if(DtKNF.kodkt<3)
	{
		DtKNF.mmin=ReadEEPROM(_ADRMDG);					//dolna granica predkosci wentylatora
		DtKNF.mmax=ReadEEPROM(_ADRMGG);					//gorna granica predkosci wentylatora
	}
	RSDT1.outMMIN=DtKNF.mmin;
	RSDT1.outMMAX=DtKNF.mmax;
	ModParam();												//oblicz parametry kluczowe modulatora
	//-----
	DtKNF.robg=ReadEEPROM(_ADROBG);						//rodzaj obiegu 0-otwarty,1-zamkniety
	RSDT1.outWOBG=DtKNF.robg;
	DtKNF.opdg=ReadEEPROM(_ADRPDG);						//aktywacja trybu ogrzewania podlogowego
	RSDT1.outOPDG=DtKNF.opdg;
	DtKNF.tpcs=ReadEEPROM(_ADRCS5);						//alternatywny czujnik cisnienia 5V
	RSDT1.outTPCS=DtKNF.tpcs;
	DtKNF.aleg=ReadEEPROM(_ADRTAL);						//aktywacja timera AL
	RSDT1.outWANL=DtKNF.aleg;
	DtKNF.nokr=ReadEEPROM(_ADRNOK);						//liczba impulsow na obrot dla tachometru wentylatora
	RSDT1.outNOKR=DtKNF.nokr;
	//-----
	PFN.poz=ReadEEPROM(_ADRPFN);							//pozycja PFN
	SetPFN();
	//-----
	//DtCO.PCO=ReadEEPROM(_ADRPCO);							//nastawa PCO
	//DtCW.PCW=ReadEEPROM(_ADRPCW);							//nastawa PCW
	//-----
	DtKNF.wymCO=0;											//wylacz wymuszenie CO
	DtKNF.wymCW=0;											//wylacz wymuszenie CW
	//-----
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
	DtKNF.teco=ReadEEPROM(_ADRTECO);						//aktywacja/deaktywacja trybu ECO
	RSDT1.outTECO=DtKNF.teco;
	//-----
	DtIN.Eco=ReadEEPROM(_ADRECO);							//wspolczynnik ECO
	RSDT1.outECO=DtIN.Eco;
        
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
    DtKNF.tstr=ReadEEPROM(_ADRTSTR);
	RSDT1.outTSTR=DtKNF.tstr;
    DtKNF.tL3=ReadEEPROM(_ADRTL3);
	RSDT1.outTL3=DtKNF.tL3; 
    
    DtKNF.pgdr=ReadEEPROM(_ADRPGDR);
	RSDT1.outPGDR=DtKNF.pgdr;    
    DtKNF.pgdg=ReadEEPROM(_ADRPGDG);
	RSDT1.outPGDG=DtKNF.pgdg; 
    DtKNF.pgdo=ReadEEPROM(_ADRPGDO);
	RSDT1.outPGDO=DtKNF.pgdo;  
    DtKNF.pgmx=ReadEEPROM(_ADRPGMX);
	RSDT1.outPGMX=DtKNF.pgmx;  
    
    DtKNF.cwmg=ReadEEPROM(_ADRCWMG);       //histereza wylaczenia CW(2F) 
	RSDT1.outCWMG=DtKNF.cwmg;    
    DtKNF.cwtm=ReadEEPROM(_ADRCWTM);       //we. sygnalu timera zasobnika (1F)    
	RSDT1.outCWTM=DtKNF.cwtm;

    DtKNF.pwtm=ReadEEPROM(_ADRPWTM);       //maksymalna wartosc przewyzszenia temperaturowego    
	RSDT1.outPWTM=DtKNF.pwtm;

    DtKNF.tpcw=ReadEEPROM(_ADRRCCW);       //rodzaj czujnika przeplywu CW (0-stykowy, 1-impulsowy)  
    SetParPrzCw();
	RSDT1.outTPCW=DtKNF.tpcw;
    InBitSet(&RSDT1.outKSTAT0,_CWIMP,((DtKNF.tpkt!=_UNICO)&&(DtKNF.tpcw!=0))?1:0);  //rodzaj czujnika przeplywu CW (0-stykowy, 1-impulsowy)
    
    DtKNF.tmpm=ReadEEPROM(_ADRTWPM);       //czas wybiegu pompy CO   
	RSDT1.outTMPM=DtKNF.tmpm;

    
#if (_EXT_BOARD>1)
    DtKNF.alin=ReadEEPROM(_ADRALIN);                                               //adres odbiornika dla magistrali LIN
#if _ZK_LADDR==0    
    if((DtKNF.alin<1)||(DtKNF.alin>5)) DtKNF.alin=5;
#else
    if((DtKNF.alin<1)||(DtKNF.alin>5&&DtKNF.alin<9)||(DtKNF.alin>12)) DtKNF.alin=5;
#endif
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
	RSDT1.outSETPPmax=M.SETPPmax;    
    
    DtKNF.P=(127.0+BtyF88ToFData(ReadEEPROM(_ADRPLO),ReadEEPROM(_ADRPHI)))/100000.0;
    DtKNF.I=(127.0+BtyF88ToFData(ReadEEPROM(_ADRILO),ReadEEPROM(_ADRIHI)))/100000.0;
    DtKNF.D=(127.0+BtyF88ToFData(ReadEEPROM(_ADRDLO),ReadEEPROM(_ADRDHI)))/100000.0; 
    
    DtKNF.RCO1=(float)ReadEEPROM(_ADRRCO1)/10.0;
    DtKNF.RCO2=(float)ReadEEPROM(_ADRRCO2)/10.0;
    DtKNF.RCO3=(float)ReadEEPROM(_ADRRCO3)/10.0;
    DtKNF.RCO4=(float)ReadEEPROM(_ADRRCO4)/10.0;

    DtKNF.RCW1=(float)ReadEEPROM(_ADRRCW1)/10.0;
    DtKNF.RCW2=(float)ReadEEPROM(_ADRRCW2)/10.0;
    DtKNF.RCW3=(float)ReadEEPROM(_ADRRCW3)/10.0;
    DtKNF.RCW4=(float)ReadEEPROM(_ADRRCW4)/10.0;    
    
    M.RGDZ=GetRGDZ();
}
/*---------------------------------------------------------------------------------*/
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
//Okresl typ kotla i przelicz/zdef. jego parametry
void SetTpKtParam(void)
{
	switch(DtKNF.kodkt)
	{
		//-------
		case 0://1f	(UNI)
		case 1:
		{
			DtKNF.tpkt=_UNICO;
			DtKNF.czpr=_BRAK;
			DtKNF.tpwm=_TERMET;
			//DtKNF.mmin - odczyt parametru P4 z pamieci EEPROM
			//DtKNF.mmax - odczyt parametru P5 z pamieci EEPROM
			break;
		}
		case 2://2f (UNI)
		{
			DtKNF.tpkt=_MONOHB;
			DtKNF.czpr=_BRAK;
			DtKNF.tpwm=_TERMET;
			//DtKNF.mmin - odczyt parametru P4 z pamieci EEPROM
			//DtKNF.mmax - odczyt parametru P5 z pamieci EEPROM
			break;
		}
		default:			//domyslny 1780.90.00.00
		{
			DtKNF.tpkt=_MONOHB;
			DtKNF.czpr=_BRAK;
			DtKNF.tpwm=_TERMET;
			//DtKNF.mmin - odczyt parametru P4 z pamieci EEPROM
			//DtKNF.mmax - odczyt parametru P5 z pamieci EEPROM
			break;
		}
	}
	InBitSet(&RSDT1.outKSTAT0,_KC1F,(DtKNF.tpkt==_UNICO)?1:0);
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
unsigned char ReadEEPCW(void)
{
	return ReadEEPROM(_ADRPCW);
}
void WriteEEPCW(void)
{
    if_WriteEEPROM(_ADRPCW,DtCW.PCW);
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Zapamietaj/odczytaj nastawe PCO
unsigned char ReadEEPCO(void)
{
	return ReadEEPROM(_ADRPCO);
}
void WriteEEPCO(void)
{   
    if_WriteEEPROM(_ADRPCO,DtCO.PCO);
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
//Zapamietaj/odczytaj nastawe ECO
unsigned char ReadEEECO(void)
{
    return ReadEEPROM(_ADRECO);
}
void WriteEEECO(void)
{    
    if_WriteEEPROM(_ADRECO,DtIN.Eco);
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Zapamietaj/odczytaj pozycje przelacznika funkcyjnego
void WriteEEPFN(void)
{
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
//Zapamietaj/odczytaj kolejne punkty funkcji kalibracji
//------
void WriteEEVNT0(unsigned char v,unsigned char p)
{
	if_WriteEEPROM(_ADRVN0,v);
	if_WriteEEPROM(_ADRPW0,p);
}
unsigned char ReadEEVNT0_V()
{
	return ReadEEPROM(_ADRVN0);
}
unsigned char  ReadEEVNT0_P()
{
	return ReadEEPROM(_ADRPW0);
}
//------
void WriteEEVNT1(unsigned char v,unsigned char p)
{
	if_WriteEEPROM(_ADRVN1,v);
	if_WriteEEPROM(_ADRPW1,p);
}
unsigned char ReadEEVNT1_V()
{
	return ReadEEPROM(_ADRVN1);
}
unsigned char ReadEEVNT1_P()
{
	return ReadEEPROM(_ADRPW1);
}
//------
void WriteEEVNT2(unsigned char v,unsigned char p)
{
	if_WriteEEPROM(_ADRVN2,v);
	if_WriteEEPROM(_ADRPW2,p);
}
unsigned char ReadEEVNT2_V()
{
	return ReadEEPROM(_ADRVN2);
}
unsigned char ReadEEVNT2_P()
{
	return ReadEEPROM(_ADRPW2);
}
//------
void WriteEEVNT3(unsigned char v,unsigned char p)
{
	if_WriteEEPROM(_ADRVN3,v);
	if_WriteEEPROM(_ADRPW3,p);
}
unsigned char ReadEEVNT3_V()
{
	return ReadEEPROM(_ADRVN3);
}
unsigned char ReadEEVNT3_P()
{
	return ReadEEPROM(_ADRPW3);
}
//------
void WriteEEVNT4(unsigned char v,unsigned char p)
{
	if_WriteEEPROM(_ADRVN4,v);
	if_WriteEEPROM(_ADRPW4,p);
}
unsigned char ReadEEVNT4_V()
{
	return ReadEEPROM(_ADRVN4);
}
unsigned char ReadEEVNT4_P()
{
	return ReadEEPROM(_ADRPW4);
}
//------
void WriteEEVNT5(unsigned char v,unsigned char p)
{
	if_WriteEEPROM(_ADRVN5,v);
	if_WriteEEPROM(_ADRPW5,p);
}
unsigned char ReadEEVNT5_V()
{
	return ReadEEPROM(_ADRVN5);
}
unsigned char ReadEEVNT5_P()
{
	return ReadEEPROM(_ADRPW5);
}
//------
void WriteEEVNT6(unsigned char v,unsigned char p)
{
	if_WriteEEPROM(_ADRVN6,v);
	if_WriteEEPROM(_ADRPW6,p);
}
unsigned char ReadEEVNT6_V()
{
	return ReadEEPROM(_ADRVN6);
}
unsigned char ReadEEVNT6_P()
{
	return ReadEEPROM(_ADRPW6);
}
//------
void WriteEEVNT7(unsigned char v,unsigned char p)
{
	if_WriteEEPROM(_ADRVN7,v);
	if_WriteEEPROM(_ADRPW7,p);
}
unsigned char ReadEEVNT7_V()
{
	return ReadEEPROM(_ADRVN7);
}
unsigned char ReadEEVNT7_P()
{
	return ReadEEPROM(_ADRPW7);
}

/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Zapamietaj/odczytaj kolejne punkty funkcji kalibracji
//------
void Write2EEVNT0(unsigned int v,unsigned int p)
{
	if_WriteEEPROM(_ADRVN0L,(unsigned char)(v&0x00FF));
	if_WriteEEPROM(_ADRVN0H,(unsigned char)(v>>8));
	if_WriteEEPROM(_ADRPW0L,(unsigned char)(p&0x00FF));
	if_WriteEEPROM(_ADRPW0H,(unsigned char)(p>>8));    
}
unsigned int Read2EEVNT0_V()
{
    volatile unsigned int d;
    
    d=(ReadEEPROM(_ADRVN0H)<<8)+ReadEEPROM(_ADRVN0L);
	return d;
}
unsigned int Read2EEVNT0_P()
{
    volatile unsigned int d;
    
    d=(ReadEEPROM(_ADRPW0H)<<8)+ReadEEPROM(_ADRPW0L);
	return d;    
}
//------
void Write2EEVNT1(unsigned int v,unsigned int p)
{
	if_WriteEEPROM(_ADRVN1L,(unsigned char)(v&0x00FF));
	if_WriteEEPROM(_ADRVN1H,(unsigned char)(v>>8));
	if_WriteEEPROM(_ADRPW1L,(unsigned char)(p&0x00FF));
	if_WriteEEPROM(_ADRPW1H,(unsigned char)(p>>8));    
}
unsigned int Read2EEVNT1_V()
{
    volatile unsigned int d;
    
    d=(ReadEEPROM(_ADRVN1H)<<8)+ReadEEPROM(_ADRVN1L);
	return d;
}
unsigned int Read2EEVNT1_P()
{
    volatile unsigned int d;
    
    d=(ReadEEPROM(_ADRPW1H)<<8)+ReadEEPROM(_ADRPW1L);
	return d;    
}
//------
void Write2EEVNT2(unsigned int v,unsigned int p)
{
	if_WriteEEPROM(_ADRVN2L,(unsigned char)(v&0x00FF));
	if_WriteEEPROM(_ADRVN2H,(unsigned char)(v>>8));
	if_WriteEEPROM(_ADRPW2L,(unsigned char)(p&0x00FF));
	if_WriteEEPROM(_ADRPW2H,(unsigned char)(p>>8));    
}
unsigned int Read2EEVNT2_V()
{
    volatile unsigned int d;
    
    d=(ReadEEPROM(_ADRVN2H)<<8)+ReadEEPROM(_ADRVN2L);
	return d;
}
unsigned int Read2EEVNT2_P()
{
    volatile unsigned int d;
    
    d=(ReadEEPROM(_ADRPW2H)<<8)+ReadEEPROM(_ADRPW2L);
	return d;    
}
//------
void Write2EEVNT3(unsigned int v,unsigned int p)
{
	if_WriteEEPROM(_ADRVN3L,(unsigned char)(v&0x00FF));
	if_WriteEEPROM(_ADRVN3H,(unsigned char)(v>>8));
	if_WriteEEPROM(_ADRPW3L,(unsigned char)(p&0x00FF));
	if_WriteEEPROM(_ADRPW3H,(unsigned char)(p>>8));    
}
unsigned int Read2EEVNT3_V()
{
    volatile unsigned int d;
    
    d=(ReadEEPROM(_ADRVN3H)<<8)+ReadEEPROM(_ADRVN3L);
	return d;
}
unsigned int Read2EEVNT3_P()
{
    volatile unsigned int d;
    
    d=(ReadEEPROM(_ADRPW3H)<<8)+ReadEEPROM(_ADRPW3L);
	return d;    
}
//------
void Write2EEVNT4(unsigned int v,unsigned int p)
{
	if_WriteEEPROM(_ADRVN4L,(unsigned char)(v&0x00FF));
	if_WriteEEPROM(_ADRVN4H,(unsigned char)(v>>8));
	if_WriteEEPROM(_ADRPW4L,(unsigned char)(p&0x00FF));
	if_WriteEEPROM(_ADRPW4H,(unsigned char)(p>>8));    
}
unsigned int Read2EEVNT4_V()
{
    volatile unsigned int d;
    
    d=(ReadEEPROM(_ADRVN4H)<<8)+ReadEEPROM(_ADRVN4L);
	return d;
}
unsigned int Read2EEVNT4_P()
{
    volatile unsigned int d;
    
    d=(ReadEEPROM(_ADRPW4H)<<8)+ReadEEPROM(_ADRPW4L);
	return d;    
}
//------
void Write2EEVNT5(unsigned int v,unsigned int p)
{
	if_WriteEEPROM(_ADRVN5L,(unsigned char)(v&0x00FF));
	if_WriteEEPROM(_ADRVN5H,(unsigned char)(v>>8));
	if_WriteEEPROM(_ADRPW5L,(unsigned char)(p&0x00FF));
	if_WriteEEPROM(_ADRPW5H,(unsigned char)(p>>8));    
}
unsigned int Read2EEVNT5_V()
{
    volatile unsigned int d;
    
    d=(ReadEEPROM(_ADRVN5H)<<8)+ReadEEPROM(_ADRVN5L);
	return d;
}
unsigned int Read2EEVNT5_P()
{
    volatile unsigned int d;
    
    d=(ReadEEPROM(_ADRPW5H)<<8)+ReadEEPROM(_ADRPW5L);
	return d;    
}
//------
void Write2EEVNT6(unsigned int v,unsigned int p)
{
	if_WriteEEPROM(_ADRVN6L,(unsigned char)(v&0x00FF));
	if_WriteEEPROM(_ADRVN6H,(unsigned char)(v>>8));
	if_WriteEEPROM(_ADRPW6L,(unsigned char)(p&0x00FF));
	if_WriteEEPROM(_ADRPW6H,(unsigned char)(p>>8));    
}
unsigned int Read2EEVNT6_V()
{
    volatile unsigned int d;
    
    d=(ReadEEPROM(_ADRVN6H)<<8)+ReadEEPROM(_ADRVN6L);
	return d;
}
unsigned int Read2EEVNT6_P()
{
    volatile unsigned int d;
    
    d=(ReadEEPROM(_ADRPW6H)<<8)+ReadEEPROM(_ADRPW6L);
	return d;    
}
//------
void Write2EEVNT7(unsigned int v,unsigned int p)
{
	if_WriteEEPROM(_ADRVN7L,(unsigned char)(v&0x00FF));
	if_WriteEEPROM(_ADRVN7H,(unsigned char)(v>>8));
	if_WriteEEPROM(_ADRPW7L,(unsigned char)(p&0x00FF));
	if_WriteEEPROM(_ADRPW7H,(unsigned char)(p>>8));    
}
unsigned int Read2EEVNT7_V()
{
    volatile unsigned int d;
    
    d=(ReadEEPROM(_ADRVN7H)<<8)+ReadEEPROM(_ADRVN7L);
	return d;
}
unsigned int Read2EEVNT7_P()
{
    volatile unsigned int d;
    
    d=(ReadEEPROM(_ADRPW7H)<<8)+ReadEEPROM(_ADRPW7L);
	return d;    
}
//------
void Write2EEVNT8(unsigned int v,unsigned int p)
{
	if_WriteEEPROM(_ADRVN8L,(unsigned char)(v&0x00FF));
	if_WriteEEPROM(_ADRVN8H,(unsigned char)(v>>8));
	if_WriteEEPROM(_ADRPW8L,(unsigned char)(p&0x00FF));
	if_WriteEEPROM(_ADRPW8H,(unsigned char)(p>>8));    
}
unsigned int Read2EEVNT8_V()
{
    volatile unsigned int d;
    
    d=(ReadEEPROM(_ADRVN8H)<<8)+ReadEEPROM(_ADRVN8L);
	return d;
}
unsigned int Read2EEVNT8_P()
{
    volatile unsigned int d;
    
    d=(ReadEEPROM(_ADRPW8H)<<8)+ReadEEPROM(_ADRPW8L);
	return d;    
}
//------
void Write2EEVNT9(unsigned int v,unsigned int p)
{
	if_WriteEEPROM(_ADRVN9L,(unsigned char)(v&0x00FF));
	if_WriteEEPROM(_ADRVN9H,(unsigned char)(v>>8));
	if_WriteEEPROM(_ADRPW9L,(unsigned char)(p&0x00FF));
	if_WriteEEPROM(_ADRPW9H,(unsigned char)(p>>8));    
}
unsigned int Read2EEVNT9_V()
{
    volatile unsigned int d;
    
    d=(ReadEEPROM(_ADRVN9H)<<8)+ReadEEPROM(_ADRVN9L);
	return d;
}
unsigned int Read2EEVNT9_P()
{
    volatile unsigned int d;
    
    d=(ReadEEPROM(_ADRPW9H)<<8)+ReadEEPROM(_ADRPW9L);
	return d;    
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Odczyt liczby zapisanych pozycji historii zdarzen awaryjnych
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
    WriteBtySTAT(nr,12,RSDT1.outVNT);    
    WriteBtySTAT(nr,13,RSDT1.outPCO);
    WriteBtySTAT(nr,14,RSDT1.outPCW);
    WriteBtySTAT(nr,15,RSDT1.outPPCO);
    WriteBtySTAT(nr,16,RSDT1.outPPCW);
    WriteBtySTAT(nr,17,RSDT1.outERR);
    WriteBtySTAT(nr,18,RSDT1.outAWR);
    WriteBtySTAT(nr,19,RSDT1.outPP);
    WriteBtySTAT(nr,20,RSDT1.outPG);
    WriteBtySTAT(nr,21,M.CONTSETP);    
    WriteBtySTAT(nr,22,RSDT1.outZW);			  
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Kopiuj ststus nr0 urzadzenia do statusu nr1 (stat(nr1)=stat(nr0))
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
//Zapisz ststus urzadzenia w momencie wylaczenia awaryjnego z blokada
/*void WriteEESTAT(void)
{
    volatile unsigned int d;
    volatile unsigned char e;
    
    //--------------------------------------
    //Liczba wylaczen awaryjnych z blokada
    d=ReadNZSTAT(0)+(ReadNZSTAT(1)*256); //liczba wylaczen z blokada 
    if(d<0xffff) d++;
    else d=1;
    
    e=(unsigned char)(d&0x00ff);
    if_if_WriteEEPROM(_ADRNZEE+0,e); 
    e=(unsigned char)((d&0xff00)>>8);    
    if_if_WriteEEPROM(_ADRNZEE+1,e);
    //---------------------------------------
    //---------------------------------------
    //Nr zdarzenia
    e=ReadEEHIST(1);
    if_if_WriteEEPROM(_ADRSTEE+0,e);
    e=ReadEEHIST(2);
    if_if_WriteEEPROM(_ADRSTEE+1,e);
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
    if_if_WriteEEPROM(_ADRSTEE+2,e);
    if_if_WriteEEPROM(_ADRSTEE+3,RSDT1.outSTAT1);
    if_if_WriteEEPROM(_ADRSTEE+4,RSDT1.outSTAT2);
    if_if_WriteEEPROM(_ADRSTEE+5,RSDT1.outSTAT3);   
    if_if_WriteEEPROM(_ADRSTEE+6,RSDT1.outSTAT4);
    if_if_WriteEEPROM(_ADRSTEE+7,RSDT1.outSTAT5);  
    if_if_WriteEEPROM(_ADRSTEE+8,RSDT1.outrCO);			//rzeczywista wartosc temp
    if_if_WriteEEPROM(_ADRSTEE+9,RSDT1.outrCW);			//rzeczywista wartosc temp    
    if_if_WriteEEPROM(_ADRSTEE+10,RSDT1.outCS);
    if_if_WriteEEPROM(_ADRSTEE+11,RSDT1.outIN);
    if_if_WriteEEPROM(_ADRSTEE+12,RSDT1.outVNT);
    if_if_WriteEEPROM(_ADRSTEE+13,RSDT1.outPCO);
    if_if_WriteEEPROM(_ADRSTEE+14,RSDT1.outPCW);
    if_if_WriteEEPROM(_ADRSTEE+15,RSDT1.outPPCO);
    if_if_WriteEEPROM(_ADRSTEE+16,RSDT1.outPPCW);
    if_if_WriteEEPROM(_ADRSTEE+17,RSDT1.outERR);
    if_if_WriteEEPROM(_ADRSTEE+18,RSDT1.outAWR);
    if_if_WriteEEPROM(_ADRSTEE+19,RSDT1.outPP);
    if_if_WriteEEPROM(_ADRSTEE+20,RSDT1.outPG);
    if_if_WriteEEPROM(_ADRSTEE+21,M.CONTSETP);
    if_if_WriteEEPROM(_ADRSTEE+22,RSDT1.outZW);    
}*/

/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
#endif