/*konfig3.c*/
//Procedury zwiazane z wyborem typu gazu i poziomow
//mocy startowej oraz majksymalnej dla CO i CW
/*---------------------------------------------------------------------------------*/
//#include <pic18.h>
//#include <htc.h>
#include <xc.h>
#include "global.h"					//parametry globalne
#include "konfig3.h"					//parametry lokalne
#include "eeprom.h"
#include "pomiar.h"
#include "wsw.h"
#include "main.tp.h"
#include "keypad.tp.h"
#include "print.h"

#if (_EXT_BOARD>=2)
    #include "amgs_rs9b.tp.h"
#else
    #include "mgs_rs9b.tp.h"
#endif

volatile DtKONF DtKNF;
volatile unsigned int nrp=0;
unsigned char zm,zp;


extern volatile unsigned char x,y;

extern volatile MGSRSData MGSD;
extern tpMAIN M;
extern volatile DataPFN PFN;
extern volatile tpRSDTA1 RSDT1;
extern volatile StPin ky[];
extern volatile StPin xxky[];
/*---------------------------------------------------------------------------------*/
//Podstawowe cechy kolejnych parametrow
//MIN,MAX,(RLD,NO_MSG,HIGH,T,B,SHW,x,x)
const CDtPZK CPZK[_KNPAR]=
{
	0 ,99,0b10001100,			//0 moc startowa
	0 ,99,0b10001100,           //1 moc maksymalna CW
	0 ,99,0b10001100,           //2 moc maksymalna CO
	12,20,0b11100100,			//3 dolna granica predkosci obrotowej wentylatora
	25,95,0b11100100,			//4 gorna granica predkosci obrotowej wentylatora

	1 , 2,0b11000000,			//5 kod kotla

	0 , 1,0b11000100,			//6 rodzaj obiegu
	0 , 2,0b11000100,			//7 rodzaj ogrzewania (0-ogrzewanie tradycyjne, 1-ogrzewanie podlogowe, 2-pe³en zakres)
	0 , 1,0b11000100,			//8 wybor rodzaju czujnika cisnienia
	0 , 1,0b11000100,			//9 aktywacja zegara antylegionella

	1 , 4,0b11000100,			//10 tachometr - liczba okresow na sek.
	0 , 1,0b11000100,			//11 rodzaj pompy
	5 ,25,0b11010100,			//12 delta T dla pompy
	15,99,0b11001100,           //13 minimalny wydatek pompy
	0 , 1,0b11000100,			//14 aktywacja/deaktywacja trybu pracy ECO

	0 , 5,0b11000100,			//15 czas pracy przy obnizonej mocy w trybie CO/deaktywacja dla wartosci 0
	0 ,25,0b11001100,			//16 wartosc obnizonej mocy w trybie CO
	25,99,0b11001100,			//17 maksymalny wydatek pompy
	25,99,0b11001100,			//18 maksymalny wydatek pompy dla obiegu CW     

	0 ,15,0b11010100,			//19 histereza wylaczenia grzania CO (ponad nastawe)
	50,89,0b11010100,			//20 temp CO przy grzaniu CW zasobnika
	0 ,20,0b11010100,			//21 poprawka dla regulagtora pogodowego  
    0 , 2,0b11000100,			//22 wybor rodzaju wentylatora
    2 ,30,0b11000100,			//23 czas wybiegu na mocy startowej dla obiegu CO   
    1 ,60,0b11000100,			//24 czas blokady L3 dla obiegu CO     
    
    0 , 3,0b11000100,			//25 rodzaj regulatora pogodowego
    10,21,0b11010100,			//26 wartosc graniczna pobudzenia od regulatora pogodowego
    0 ,20,0b11010100,			//27 obnizenie nocne dla regulatora pogodowego  
    _MIN_PDCO ,_MAX_PCO,0b01010100,	//28 maksymalna wartosc graniczna CO
    0 , 1,0b11000100,			//29 histereza wylaczenia CW(2F)
    0 , 1,0b11000100,			//30 we. sygnalu timera zasobnika (1F)     
    5 ,15,0b11010100,			//31 przewyzszenie temperaturowe    
    
    0 , 1,0b11000100,			//32 rodzaj czujnika przeplywu CW (0-stykowy, 1-impulsowy) (2F)     
    20 ,240,0b11000100			//33 czas wybiegu pompy CO     
};
/*---------------------------------------------------------------------------------*/
//Podstawowe cechy kolejnych parametrow
//MIN,MAX,DTA,DYN_DTA,(RLD,NO_MSG,HIGH,T,B,SHW)
volatile static DtPZK PZK[_KNPAR];
/*---------------------------------------------------------------------------------*/
//Inicjalizacja danych zwiazanych z kolejnymi parametrami konfiguracyjnymi
void InitPZK(void)
{
    volatile unsigned char i;
    for(i=0;i<_KNPAR;i++)
    {
        PZK[i].dta=0;
        PZK[i].bfdta=0;        
        PZK[i].min=CPZK[i].min;
        PZK[i].max=CPZK[i].max;    
        PZK[i].fl.war=CPZK[i].flwar;        
    }
}
/*---------------------------------------------------------------------------------*/
//Odblokowanie procedury wejsciowej konfiguracji
//po uruchomieniu sterownika na pozycji OFF
//UEAGA: umiescic przed petla glowna
void InitKonf(void)
{
		DtKNF.KRK=0;
		DtKNF.PRM=0;
        InitPZK();                                                          //Inicjalizacja danych charakterystycznych kolejnych parametrow
}
/*---------------------------------------------------------------------------------*/
//wyjscie z procedury konfiguracji po modyfikacji parametrow
void ExitKonf(void)
{
	PFNf_aktywne_KNF=0;
	EndPR();								//wygas wskaz parametru
	ClrAllMarks();							//wygas znaki konfiguracji
	DtKNF.PRM=0;
	DtKNF.KRK=0;
#if _KEY_BBOARD==0    
//	WaitToLowEx(&ky[SS_OFF]);
//	WaitToLowEx(&ky[SS_SET]);
    WaitToLowEx(&ky[SS_BAR_RES]);
	WaitToLowEx(&ky[SS_RES]);
	WaitToLowEx(&ky[SS_BAR]);    
#endif
#if _KEY_BBOARD==1    
	WaitToLowEx(&ky[SS_OFF]);
	WaitToLowEx(&ky[SS_KOM]);
#endif    
	PFNf_KnfDisErr=0;
}
/*---------------------------------------------------------------------------------*/
//Aktualizacja wartosci parametru (po akceptacji zmiany)
void UpdateKnfPar(const unsigned char par)
{
    if(par<_KNPAR)
    {
        switch(par)
        {
            case 0:
                RSDT1.inMSTR=PZK[0].dta;		//0 moc startowa
            break;
            case 1:
                RSDT1.inMMAXU=PZK[1].dta;		//1 moc maksymalna CW
            break;
            case 2:
                RSDT1.inMMAXG=PZK[2].dta;		//2 moc maksymalna CO
            break;
            case 3:
                RSDT1.inMMIN=PZK[3].dta;		//3 dolna granica predkosci obrotowej wentylatora
            break;
            case 4:
                RSDT1.inMMAX=PZK[4].dta;		//4 gorna granica predkosci obrotowej wentylatora
            break;
            case 5:
                RSDT1.inKODKT=PZK[5].dta;		//5 kod kotla
            break;
            case 6:
                RSDT1.inWOBG=PZK[6].dta;		//6 rodzaj obiegu
            break;
            case 7:
                RSDT1.inOPDG=PZK[7].dta;		//7 ogrzewanie podlogowe
            break;
            case 8:
                RSDT1.inTPCS=PZK[8].dta;		//8 wybor rodzaju czujnika cisnienia
            break;
            case 9:
                RSDT1.inWANL=PZK[9].dta;		//9 aktywacja zegara antylegionella		
            break;
            case 10:
                RSDT1.inNOKR=PZK[10].dta;		//10 tachometr - liczba okresow na sek.
            break;
            case 11:
                RSDT1.inRDPM=PZK[11].dta;		//11 rodzaj pompy
            break;
            case 12:
                RSDT1.inDTPM=PZK[12].dta;		//12 delta T dla pompy
            break;
            case 13:
                            RSDT1.inMDPM=PZK[13].dta;                   //13 minimalny wydatek pompy
                            if(RSDT1.inMDPM>RSDT1.inMGPM)
                            {
                                PZK[17].dta=RSDT1.inMDPM;
                                RSDT1.inMGPM=PZK[17].dta;
                            }
            break;
            case 14:
                RSDT1.inTECO=PZK[14].dta;		//14 aktywacja/deaktywacja trybu pracy ECO
            break;
            case 15:
                RSDT1.inTMMAXG2=PZK[15].dta;            //15 czas pracy przy obnizonej mocy w trybie CO/deaktywacja dla wartosci 0
            break;
            case 16:
                RSDT1.inMMAXG2=PZK[16].dta;		//16 wartosc obnizonej mocy w trybie CO
            break;
            case 17:
                            RSDT1.inMGPM=PZK[17].dta;		//17 maksymalny wydatek pompy
                            if(RSDT1.inMGPM<RSDT1.inMDPM)
                            {
                                PZK[13].dta=RSDT1.inMGPM;
                                RSDT1.inMDPM=PZK[13].dta;
                            }
            break;
            case 18:
                RSDT1.inMGPM2=PZK[18].dta;		//18 maksymalny wydatek pompy dla obiegu CW
            break;
            case 19:
                RSDT1.inHSCO=PZK[19].dta;		//19 histereza wylaczenia grzania CO (ponad nastawe)  
            break;
            case 20:
                RSDT1.inCONW=PZK[20].dta;		//20 temp CO przy grzaniu CW zasobnika
            break;
            case 21:
                RSDT1.inPROP=PZK[21].dta;		//21 poprawka dla regulagtora pogodowego   
            break;
            case 22:
                RSDT1.inRFAN=PZK[22].dta;		//22 wybor rodzaju wentylatora   
            break; 
            case 23:
                RSDT1.inTSTR=PZK[23].dta;		//23 czas wybiegu na mocy startowej dla obiegu CO   
            break;
            case 24:
                RSDT1.inTL3=PZK[24].dta;		//24 czas blokady L3 dla obiegu CO   
            break;           
            case 25:
                RSDT1.inPGDR=PZK[25].dta;		//25 rodzaj regulatora pogodowego   
            break; 
            case 26:
                RSDT1.inPGDG=PZK[26].dta;		//26 wartosc graniczna pobudzenia od regulatora pogodowego   
            break;
            case 27:
                RSDT1.inPGDO=PZK[27].dta;		//27 obnizenie nocne dla regulatora pogodowego   
            break; 
            case 28:
                RSDT1.inPGMX=PZK[28].dta;		//28 maksymalna wartosc graniczna CO dla regulatora pogodowego   
            break;  
            case 29:
                RSDT1.inCWMG=PZK[29].dta;		//29 histereza wylaczenia CW(2F)   
            break;            
            case 30:
                RSDT1.inCWTM=PZK[30].dta;		//30 we. sygnalu timera zasobnika (1F)   
            break;      
            case 31:
                RSDT1.inPWTM=PZK[31].dta;		//31 przewyzszenie temp.  
            break;
            case 32:
                RSDT1.inTPCW=PZK[32].dta;		//32 rodzaj czujnika przeplywu CW (0-stykowy, 1-impulsowy)
            break;
            case 33:
                RSDT1.inTMPM=PZK[33].dta;		//33 czas wybiegu pompy CO                
            break;            
        }
    }
}
/*---------------------------------------------------------------------------------*/
//Aktualizuj wartosci graniczne wskazanego parametru
void UpdateMinMax(const unsigned char par)
{
    if(par<_KNPAR)
    {
        //wartosci graniczne
        switch(par)
        {
            case 22:        //granica (min,max) dla wybor rodzaju wentylatora       
            {
                PZK[22].min=CPZK[22].min;
                PZK[22].max=CPZK[22].max;
                if(PFN.vUNI02<16)               //wersje przed wprowadzeniem procesora Q
                {
                    PZK[22].max=1;
                }
                if(PZK[22].bfdta<PZK[22].min) PZK[22].bfdta=PZK[22].min;
                if(PZK[22].dta<PZK[22].min) PZK[22].dta=PZK[22].min;
                if(PZK[22].bfdta>PZK[22].max) PZK[22].bfdta=PZK[22].max;
                if(PZK[22].dta>PZK[22].max) PZK[22].dta=PZK[22].max;     
                break;
            }
            case 28:        //granica (min,max) dla maksymalnej wartosci granicznej CO
            {
                switch(RSDT1.inOPDG)
                {
                    case 0:
                    {
                        PZK[28].min=_MIN_PCO;
                        PZK[28].max=_MAX_PCO;                        
                        break;
                    }
                    case 1:
                    {
                        PZK[28].min=_MIN_PDCO;
                        PZK[28].max=_MAX_PDCO;                        
                        break;
                    }
                    case 2:
                    {
                        PZK[28].min=_MIN_PDCO;
                        PZK[28].max=_MAX_PCO;                        
                        break;
                    }
                    default:
                    {
                        PZK[28].min=_MIN_PCO;
                        PZK[28].max=_MAX_PCO;                        
                    }
                }
                /*if(RSDT1.inOPDG)    //ogrzewanie podlogowe?
                {
                    PZK[28].min=_MIN_PDCO;
                    PZK[28].max=_MAX_PDCO;
                }
                else                //ogrzewanie tradycyjne
                {
                    PZK[28].min=_MIN_PCO;
                    PZK[28].max=_MAX_PCO;
                }*/            
                if(PZK[28].bfdta<PZK[28].min) PZK[28].bfdta=PZK[28].min;
                if(PZK[28].bfdta>PZK[28].max) PZK[28].bfdta=PZK[28].max; 
                if(PZK[28].dta<PZK[28].min) PZK[28].dta=PZK[28].min;
                if(PZK[28].dta>PZK[28].max) PZK[28].dta=PZK[28].max;         
                break;
            }
            default:      //pozostale parametry
            {
                PZK[par].min=CPZK[par].min;
                PZK[par].max=CPZK[par].max;            
            }
        }
    } 
}
//------------------------------------------------------------------------------------
//Decyzja o ukryciu wybranych parametrow
void DecShowHide(void)
{
    volatile unsigned char i;
    //-----------------------------------------------
    //Zaladuj znaczniki domyslne wszystkim parametrom
    for(i=0;i<_KNPAR;i++)
    {   
        PZK[i].fl.war=CPZK[i].flwar;        
    }
    //-----------------------------------------------
    //Zaktualizuj znaczniki zwiazane z ukrywaniem parametrow

	//5 kod kotla
	PZK[5].fl.shw=GetBit2(RSDT1.outKSTAT0,_ZCFM);//pokaz/ukryj parametry powiazane

	//9 aktywacja zegara antylegionella
	PZK[9].fl.shw=GetBit2(RSDT1.outKSTAT0,_KC1F);//pokaz/ukryj parametry powiazane

	//12 delta T dla pompy
	//13 minimalny wydatek pompy
	//14 aktywacja/deaktywacja trybu pracy ECO
    //17 maksymalny wydatek pompy dla CO
    //18 maksymalny wydatek pompy dla CW
	if(RSDT1.inRDPM==0)				//pokaz/ukryj parametry powiazane
	{
		PZK[12].fl.shw=0;				//RSDT1.inDTPM
		PZK[13].fl.shw=0;				//RSDT1.inMDPM
		PZK[14].fl.shw=0;				//RSDT1.inTECO
		PZK[17].fl.shw=0;				//RSDT1.inMGPM
		PZK[18].fl.shw=0;				//RSDT1.inMGPM2        
	}
	else
	{
        if(RSDT1.inWOBG)            //obieg zamkniety?
        {
            if(RSDT1.inTECO)        //tryb ECO?
            {
                PZK[12].fl.shw=0;				//RSDT1.inDTPM    
            }
            else
            {
                PZK[12].fl.shw=1;				//RSDT1.inDTPM        
            }
            PZK[13].fl.shw=1;                  //RSDT1.inMDPM
            PZK[14].fl.shw=1;                  //RSDT1.inTECO
            PZK[17].fl.shw=1;                  //RSDT1.inMGPM
            if(RSDT1.inKODKT==_KODKT_1F)    //jednofunkcyjny?
            {
                PZK[18].fl.shw=1;				//RSDT1.inMGPM2
            }
            else
            {
                PZK[18].fl.shw=0;				//RSDT1.inMGPM2            
            }
        }
        else
        {
            PZK[12].fl.shw=0;				//RSDT1.inDTPM
            PZK[13].fl.shw=0;				//RSDT1.inMDPM
            PZK[14].fl.shw=0;				//RSDT1.inTECO 
            PZK[17].fl.shw=1;				//RSDT1.inMGPM
            if(RSDT1.inKODKT==_KODKT_1F)    //jednofunkcyjny?
            {
                PZK[18].fl.shw=1;				//RSDT1.inMGPM2
            }
            else
            {
                PZK[18].fl.shw=0;				//RSDT1.inMGPM2            
            }            
        }
	}

	//16 wartosc obnizonej mocy w trybie CO
	if(RSDT1.inTMMAXG2==0)				//pokaz/ukryj parametry powiazane
	{
		PZK[16].fl.shw=0;				//RSDT1.inMMAXG2
	}
	else
	{
		PZK[16].fl.shw=1;				//RSDT1.inMMAXG2
	}
    //20 temp CO przy grzaniu CW zasobnika
    if(RSDT1.inKODKT==_KODKT_1F)    //jednofunkcyjny?
    {
        PZK[20].fl.shw=1;				//RSDT1.inCONW
    }
    else
    {
        PZK[20].fl.shw=0;				//RSDT1.inCONW            
    }
    //21 poprawka dla regulagtora pogodowego
    //26 wartosc graniczna pobudzenia od regulatora pogodowego
    //27 obnizenie nocne dla regulatora pogodowego
    if(RSDT1.inPGDR)                        //tryb pracy regulatora pogodowego
    {
        PZK[21].fl.shw=1;                   //RSDT1.inPROP
        if(RSDT1.inPGDR==2)                 //pobudzenie RT generowane przez regulator pogodowy?
        {
            PZK[26].fl.shw=1;				//RSDT1.inPGDG
            PZK[27].fl.shw=1;				//RSDT1.inPGDO          
        }
        else
        {
            PZK[26].fl.shw=0;				//RSDT1.inPGDG 
            PZK[27].fl.shw=0;				//RSDT1.inPGDO        
        } 
    }
    else
    {
        PZK[21].fl.shw=0;                   //RSDT1.inPROP
        PZK[26].fl.shw=0;                   //RSDT1.inPGDG 
        PZK[27].fl.shw=0;                   //RSDT1.inPGDO        
    }
    //29 histereza wylaczenia CW(2F)
    if(RSDT1.inKODKT==_KODKT_2F)    //dwufunkcyjny?
    {
        PZK[29].fl.shw=1;                   //RSDT1.inCWMG
    }
    else
    {
        PZK[29].fl.shw=0;                   //RSDT1.inCWMG        
    }
    //30 we. sygnalu timera zasobnika (1F)
    if(RSDT1.inKODKT==_KODKT_1F)    //jednofunkcyjny?
    {
        PZK[30].fl.shw=1;                   //RSDT1.inCWTM
    }
    else
    {
        PZK[30].fl.shw=0;                   //RSDT1.inCWTM        
    }    
    //32 rodzaj czujnika przeplywu CW (0-stykowy, 1-impulsowy) (2F)
    if(RSDT1.inKODKT==_KODKT_1F)    //jednofunkcyjny?
    {
        PZK[32].fl.shw=0;                   //RSDT1.inTPCW
    }
    else
    {
        PZK[32].fl.shw=1;                   //RSDT1.inTPCW        
    }     
}
/*---------------------------------------------------------------------------------*/
//++++++++++++++++++++
unsigned char GetKNPAR(void)
{
    switch(RSDT1.vKNF)
    {
        case 1:												//pierwsza wersja paczki danych plyty UNI-02
            return _KNPAR1;				
        case 2:												//druga wersja paczki danych plyty UNI-02
            return _KNPAR2;
        case 3:
            return _KNPAR3;                    
        case 4:
            return _KNPAR4;
        case 5:
            return _KNPAR5;
        case 6:
            return _KNPAR6;
        case 7:
            return _KNPAR7;    
        case 8:
            return _KNPAR8;
        case 9:
            return _KNPAR9;       
        case 10:
            return _KNPAR10;   
        case 11:
            return _KNPAR11;             
        default:
            return _KNPAR;
    }  
}
unsigned char SetKNPAR(void)
{
    DtKNF.KNPAR=GetKNPAR();
    return DtKNF.KNPAR;
}
//++++++++++++++++++
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Glowna procedura konfiguracji (wykonywana w petli glownej)
void MKonfiguracja(void)
{
	volatile unsigned char key,fst,kk;      //kkk
    static signed int bnrp,bfdta;           //kkk
    signed int zkr=0;                       //kkk
    
	if(PFNf_aktywne_KNF&&(DtKNF.KRK>=2)&&!PFNf_opKNF) 					//procedura z jakiegos powodu nieaktywna w uk³adzie UNI-02?
	{
		ExitKonf();
		return;
	}

	if(PFNf_aktywne_KNF&&((RTM(_RTKONF3)>=_TKNFEXT)||PFNf_opgres))		//uplynol maksymalny dozwolony czas aktywnosci procedury lub wylaczenie awaryjne z blokada w ukladzie UNI-02?
	{
		ExitKonf();
		return;
	}

	switch(DtKNF.KRK)
	{
		//warunek wejscia do procedury modyfikacji parametrow
		case 0:
		{
			RSDT1.inPRM=0;
			RSDT1.inKRK=DtKNF.KRK;
			if(!PFNf_off) return;					//aktywny tryb OFF?
			if(MGSDf_NO_ODB||PFNf_toInit) return;	//problem z komunikacja?

			//przytrzymano przycisk wejscia do procedury konfiguracji 
#if _KEY_BBOARD==0   
            kk=(PFNf_aktywne_LKNF||!PFNf_off)?1:0;       //kkk
			//key=PresKeyEX(RdPrt(S_OFF)&&RdPrt(S_SET),&ky[SS_OFF_SET]);
            key=PresKeyEX(RdPrt(S_BAR)&&RdPrt(S_RES)&&!kk,&ky[SS_BAR_RES]); //kkk
#endif
#if _KEY_BBOARD==1     
            kk=(PFNf_aktywne_LKNF||!PFNf_off)?1:0;       //kkk
			key=PresKeyEX(RdPrt(S_OFF)&&RdPrt(S_KOM)&&!kk,&ky[SS_OFF_SET]); //kkk
#endif			
			if(!PFNf_aktywne_LKNF&&PFNf_enKNF&&key!=100&&key>=50) //przytrzymano przycisk wejscia do procedury konfiguracji?        //kkk
			{
#if _KEY_BBOARD==0                
				WaitToLowEx(&ky[SS_BAR_RES]);			
				WaitToLowEx(&ky[SS_BAR]);
				WaitToLowEx(&ky[SS_RES]);
#endif  
#if _KEY_BBOARD==1                
				WaitToLowEx(&ky[SS_OFF_SET]);			
				WaitToLowEx(&ky[SS_OFF]);
				WaitToLowEx(&ky[SS_KOM]);
#endif                
				PFNf_aktywne_KNF=1;
				StartRTM(_RTKONF3);					//start timera aktywnosci proc. konfig
				DtKNF.KNPAR=_KNPAR;					//domyslna maksymalna liczba parametrow (obs.kompatybilnosci w dol)
				DtKNF.PRM=0; 						//aktywacja pierwszego parametru
				DtKNF.KRK=1; 						//aktywacja proc. konfiguracji
				RSDT1.inPRM=0;
				ClrAllMarks();						//wygas znaki nie zwiazane z konfiguracja
				StartRTS(_RTKONF1);					//start timera oczekiwania na wejsciowe dane konfiguracyjne
			}
			else 
			{
				DtKNF.PRM=0;
				DtKNF.KRK=0;
				RSDT1.inPRM=0;
				RSDT1.inKRK=DtKNF.KRK;
				return;
			}
			break;
		}
		//oczekiwanie na na wejsciowe dane konfiguracyjne
		case 1:
		{
			RSDT1.inPRM=0;
			RSDT1.inKRK=DtKNF.KRK;
            InitPZK();                                                          //Inicjalizacja danych charakterystycznych kolejnych parametrow
			if((RTS(_RTKONF1)>=_TKNFINI))                                       //przekroczony limit czasu?
			{
				ExitKonf();
				return;
			}
            if(PFN.vUNI02&&RSDT1.vSTD&&RSDT1.vKNF)                              //mamy komplet danych koniecznych do porawnej pracy procedury?
            {
                if(RSDT1f_RD_KNF&&PFNf_opKNF)                                   //odczytaj dane wejsciowe parametrów ktore maja byc modyfikowane
                {
                    RSDT1f_RRD_KNF=1;                   
                    RSDT1.inMSTR=PZK[0].dta=PZK[0].bfdta=RSDT1.outMSTR;			//0 moc startowa
                    RSDT1.inMMAXU=PZK[1].dta=PZK[1].bfdta=RSDT1.outMMAXU;		//1 moc maksymalna CW
                    RSDT1.inMMAXG=PZK[2].dta=PZK[2].bfdta=RSDT1.outMMAXG;		//2 moc maksymalna CO
                    RSDT1.inMMIN=PZK[3].dta=PZK[3].bfdta=RSDT1.outMMIN;			//3 dolna granica predkosci obrotowej wentylatora
                    RSDT1.inMMAX=PZK[4].dta=PZK[4].bfdta=RSDT1.outMMAX;			//4 gorna granica predkosci obrotowej wentylatora
                    RSDT1.inKODKT=PZK[5].dta=PZK[5].bfdta=RSDT1.outKODKT;		//5 kod kotla
                    RSDT1.inWOBG=PZK[6].dta=PZK[6].bfdta=RSDT1.outWOBG;			//6 rodzaj obiegu
                    RSDT1.inOPDG=PZK[7].dta=PZK[7].bfdta=RSDT1.outOPDG;			//7 ogrzewanie podlogowe
                    RSDT1.inTPCS=PZK[8].dta=PZK[8].bfdta=RSDT1.outTPCS;			//8 wybor rodzaju czujnika cisnienia
                    RSDT1.inWANL=PZK[9].dta=PZK[9].bfdta=RSDT1.outWANL;			//9 aktywacja zegara antylegionella
                    RSDT1.inNOKR=PZK[10].dta=PZK[10].bfdta=RSDT1.outNOKR;		//10 tachometr - liczba okresow na sek.
                    RSDT1.inRDPM=PZK[11].dta=PZK[11].bfdta=RSDT1.outRDPM;		//11 rodzaj pompy
                    RSDT1.inDTPM=PZK[12].dta=PZK[12].bfdta=RSDT1.outDTPM;		//12 delta T dla pompy
                    RSDT1.inMDPM=PZK[13].dta=PZK[13].bfdta=RSDT1.outMDPM;		//13 minimalny wydatek pompy
                    RSDT1.inTECO=PZK[14].dta=PZK[14].bfdta=RSDT1.outTECO;		//14 aktywacja/deaktywacja trybu ECO
                    RSDT1.inTMMAXG2=PZK[15].dta=PZK[15].bfdta=RSDT1.outTMMAXG2;	//15 czas pracy przy obnizonej mocy w trybie CO/deaktywacja dla wartosci 0
                    RSDT1.inMMAXG2=PZK[16].dta=PZK[16].bfdta=RSDT1.outMMAXG2;	//16 wartosc obnizonej mocy w trybie CO
                    RSDT1.inMGPM=PZK[17].dta=PZK[17].bfdta=RSDT1.outMGPM;		//17 maksymalny wydatek pompy
                    RSDT1.inMGPM2=PZK[18].dta=PZK[18].bfdta=RSDT1.outMGPM2;		//18 maksymalny wydatek pompy dla obiegu CW                      
                    RSDT1.inHSCO=PZK[19].dta=PZK[19].bfdta=RSDT1.outHSCO;		//19 histereza wylaczenia grzania CO (ponad nastawe) 
                    RSDT1.inCONW=PZK[20].dta=PZK[20].bfdta=RSDT1.outCONW;		//20 temp CO przy grzaniu CW zasobnika                 
                    RSDT1.inPROP=PZK[21].dta=PZK[21].bfdta=RSDT1.outPROP;		//21 poprawka dla regulagtora pogodowego
                    RSDT1.inRFAN=PZK[22].dta=PZK[22].bfdta=RSDT1.outRFAN;		//22 wybor rodzaju wentylatora
                    RSDT1.inTSTR=PZK[23].dta=PZK[23].bfdta=RSDT1.outTSTR;		//23 czas wybiegu na mocy startowej dla obiegu CO 
                    RSDT1.inTL3=PZK[24].dta=PZK[24].bfdta=RSDT1.outTL3;         //24 czas blokady L3 dla obiegu CO  

                    RSDT1.inPGDR=PZK[25].dta=PZK[25].bfdta=RSDT1.outPGDR;       //25 rodzaj regulatora pogodowego
                    RSDT1.inPGDG=PZK[26].dta=PZK[26].bfdta=RSDT1.outPGDG;       //26 wartosc graniczna pobudzenia od regulatora pogodowego
                    RSDT1.inPGDO=PZK[27].dta=PZK[27].bfdta=RSDT1.outPGDO;       //27 obnizenie nocne dla regulatora pogodowego                
                    RSDT1.inPGMX=PZK[28].dta=PZK[28].bfdta=RSDT1.outPGMX;       //28 maksymalna wartosc graniczna CO dla regulatora pogodowego
                    
                    RSDT1.inCWMG=PZK[29].dta=PZK[29].bfdta=RSDT1.outCWMG;       //29 histereza wylaczenia CW(2F)                    
                    RSDT1.inCWTM=PZK[30].dta=PZK[30].bfdta=RSDT1.outCWTM;       //30 we. sygnalu timera zasobnika (1F)
                    RSDT1.inPWTM=PZK[31].dta=PZK[31].bfdta=RSDT1.outPWTM;       //31 przewyzszenie temp.
                    
                    RSDT1.inTPCW=PZK[32].dta=PZK[32].bfdta=RSDT1.outTPCW;       //32 rodzaj czujnika przeplywu CW (0-stykowy, 1-impulsowy)
                    RSDT1.inTMPM=PZK[33].dta=PZK[33].bfdta=RSDT1.outTMPM;       //33 czas wybiegu pompy CO
                    
                    DecShowHide();												//pokaz/ukryj parametry
                    if(PZK[5].fl.shw) DtKNF.PRM=nrp=5;							//domyslnie zaznaczony parametr nr 5 - wybor typu kotla
                    else DtKNF.PRM=nrp=0;										//domyslnie zaznaczony parametr nr 0 - moc startowa
                    //kompatybilnosc wsteczna 
                    SetKNPAR();                                                 //okresl liczbe parametrow w zaleznosci od wersji oprogramowania
                    RSDT1f_RRD_KNF=0;     
                    
                    WaitToLowEx(&xxky[SS_BAR]);
                    WaitToLowEx(&xxky[SS_RES]);
                    WaitToLowEx(&ky[SS_BAR]);		
                    WaitToLowEx(&ky[SS_RES]);
                    DtKNF.KRK=2;
                }
            }
			break;
		}
		//wybor parametru
		case 2:
		{
			RSDT1.inPRM=0;
			RSDT1.inKRK=DtKNF.KRK;
			RSDT1.inDDTA=0;
			PFNf_off=1;
			PFNf_lato=0;
			PFNf_zima=0;
			PFNf_KnfDisErr=1;
            Mark(_GZ_OFF,0);
			Mark(_GZ_BAR,0);											
			Mark(_GZ_TCO,0);
			Mark(_GZ_MXCW,0);
            Mark(_GZ_MXCO,0);   //kkk
			Mark(_GZ_MNCO,0);
			DecShowHide();
			if((nrp<_KNPAR)&&(PZK[nrp].fl.shw))							//parametr mo¿e byc wyswietlony?
			{
#if _KEY_BBOARD==0  
                bnrp=nrp;
				ReadParamONE(&bnrp,&zm,&zp,0,DtKNF.KNPAR-1,1,0);                
                nrp=bnrp;
#endif
#if _KEY_BBOARD==1  
                bnrp=nrp;
				ReadParamCO(&bnrp,&zm,&zp,0,DtKNF.KNPAR-1,1,0);                
                nrp=bnrp;
#endif                
				DtKNF.PRM=nrp;
			}
			else
			{
				if(zm==3)						//zmiana w gore?
				{
					if(nrp<(DtKNF.KNPAR-1)) nrp++;
					else nrp=0;
				}
				else
				if(zm==2)						//zmiana w dol?
				{
					if(nrp>0) nrp--;
					else nrp=DtKNF.KNPAR-1;
					}
				else							//gdy pierwsze parametr ukryty						
				{
					if(nrp<(DtKNF.KNPAR-1)) nrp++;
					else nrp=0;
				}
				break;	
			}
            
//            key=PresKeyEX(RdPrt(S_BAR),&ky[SS_BAR]);
//            if(key>0) 
//            {
//                WaitToLowEx(&ky[SS_BAR]);
//                DtKNF.PRM++;
//                while(DtKNF.PRM<_KNPAR1&&!(PZK[nrp].fl.shw))
//                {
//                    DtKNF.PRM++;
//                }
//                if(DtKNF.PRM>_KNPAR1) DtKNF.PRM=1;
//            }
                 
			//PrintPR(led_P,DtKNF.PRM+1,0);								//kod rx
            PrintPR2(led_P,DtKNF.PRM+1,0);								//kod rx        //kkk
#if _KEY_BBOARD==0             
			key=PresKeyEX(RdPrt(S_RES),&ky[SS_RES]);
#endif
#if _KEY_BBOARD==1             
			key=PresKeyEX(RdPrt(S_KOM),&ky[SS_KOM]);
#endif            
			if(key==100)													//wybrano parametr (nacisnieto RESET)
			{
#if _KEY_BBOARD==0                
				WaitToLowEx(&ky[SS_BAR]);		
				WaitToLowEx(&ky[SS_RES]);
#endif
#if _KEY_BBOARD==1                
				WaitToLowEx(&ky[SS_KOM]);		
				WaitToLowEx(&ky[SS_OFF]);
#endif                
                if(DtKNF.PRM<_KNPAR)
                {
                    RSDT1.inDDTA=PZK[DtKNF.PRM].bfdta=PZK[DtKNF.PRM].dta;
                    RSDT1.inPRM=DtKNF.PRM;
                    UpdateMinMax(DtKNF.PRM);                                //aktualizuj wartosci graniczne parametru                
                    DtKNF.KRK=3;											//wejscie w edycje parametru
                }
			}
			//key=PresKeyEX(RdPrt(S_OFF),&ky[SS_OFF]);
            else
//			if(key!=100&&key>=20)                                       //wyjscie z procedury (przytrzymany RESET)
            if(key>20)                                       //wyjscie z procedury (przytrzymany RESET)    
			{
                EndPR();								//wygas wskaz parametru
                ClrAllMarks();
                ClrDig(1,1,0);
                Mark(_GZ_OFF,2);
                
				WaitToLowEx(&ky[SS_RES]);		
                WaitToLowEx(&ky[SS_BAR]);
				StartRTS(_RTKONF1);										//start timera zapisu
                ClrNKFN();                                              //zerowanie licznika ramek konfiguracyjnych %%%%
				DtKNF.KRK=4;											//wyjscie z procedury
			}
			break;
		}
		//modyfikacji wartosci parametru
		case 3:
		{
			RSDT1.inPRM=DtKNF.PRM;
			RSDT1.inKRK=DtKNF.KRK;
			PFNf_off=0;
			PFNf_lato=1;
			PFNf_zima=0;
			PFNf_KnfDisErr=PZK[DtKNF.PRM].fl.no_msg;
//			if((PZK[DtKNF.PRM].max-PZK[DtKNF.PRM].min)>10) fst=1;			//decyzja o zmiennej szybkosci inkrementacji/dekrementacji
//			else fst=0;
            zkr=(PZK[DtKNF.PRM].max-PZK[DtKNF.PRM].min);    //kkk
			if(zkr>=0)
            {
                if(zkr>10) fst=1;
                else fst=0;
            }
            else
            {
                if((-1*zkr)>10) fst=1;
                else fst=0;        
            }            
#if _KEY_BBOARD==0 
            bfdta=PZK[DtKNF.PRM].bfdta;
			ReadParamONE(&bfdta,&zm,&zp,PZK[DtKNF.PRM].min,PZK[DtKNF.PRM].max,1,fst);            
            PZK[DtKNF.PRM].bfdta=bfdta;
#endif            
#if _KEY_BBOARD==1         
            bfdta=PZK[DtKNF.PRM].bfdta;
            ReadParamCO(&bfdta,&zm,&zp,PZK[DtKNF.PRM].min,PZK[DtKNF.PRM].max,PZK[DtKNF.PRM].fl.rld,fst);
            PZK[DtKNF.PRM].bfdta=bfdta;
#endif			
			RSDT1.inDDTA=(unsigned char)PZK[DtKNF.PRM].bfdta;        //kkk
			//Formatuj wyswietlanie
			if(!(!PFNf_KnfDisErr&&(PFNf_ferr||PFNf_fmsg)))					//nie jest wyswietlany kod awaryjny?
			{
//				if(!PZK[DtKNF.PRM].fl.high)
//                {
//                    if(PZK[DtKNF.PRM].bfdta<=99)
//                    {
//                        PrintWPR(0,PZK[DtKNF.PRM].bfdta);   
//                    }
//                    else
//                    {
//                        PrintWPR(PZK[DtKNF.PRM].bfdta/100,PZK[DtKNF.PRM].bfdta%100);     
//                    }
//                }
//                else
//                {
//                    PrintWPR(PZK[DtKNF.PRM].bfdta,0);
//                }
                PrintWPR2(PZK[DtKNF.PRM].bfdta,PZK[DtKNF.PRM].fl.war);      //kkk
                if(PZK[DtKNF.PRM].fl.high)  //kkk
                {
                    Mark(_GZ_MXCW,PZK[DtKNF.PRM].bfdta>=PZK[DtKNF.PRM].max);
                }
                else
                {
                    if(PZK[DtKNF.PRM].max>=0)
                    {
                        if(PZK[DtKNF.PRM].max>99)
                        {
                            Mark(_GZ_MXCW,PZK[DtKNF.PRM].bfdta>=PZK[DtKNF.PRM].max);
                        }
                        else
                        {
                            Mark(_GZ_MXCO,PZK[DtKNF.PRM].bfdta>=PZK[DtKNF.PRM].max);
                        }
                    }
                    else
                    {
                        if(PZK[DtKNF.PRM].max<-9)
                        {
                            Mark(_GZ_MXCW,PZK[DtKNF.PRM].bfdta>=PZK[DtKNF.PRM].max);
                        }
                        else
                        {
                            Mark(_GZ_MXCO,PZK[DtKNF.PRM].bfdta>=PZK[DtKNF.PRM].max);
                        }                        
                    }
                }                
				Mark(_GZ_MNCO,PZK[DtKNF.PRM].bfdta<=PZK[DtKNF.PRM].min);
				Mark(_GZ_TCO,PZK[DtKNF.PRM].fl.T);
				Mark(_GZ_OFF,0);
			}
			else
			{
 				EndPR();
				Mark(_GZ_MXCW,0);		
                Mark(_GZ_MXCO,0);   //kkk
				Mark(_GZ_MNCO,0);
				Mark(_GZ_TCO,0);
				Mark(_GZ_BAR,0);
                Mark(_GZ_OFF,0);
			}
#if _KEY_BBOARD==0             
			key=PresKeyEX(RdPrt(S_RES),&ky[SS_RES]);
#endif
#if _KEY_BBOARD==1             
			key=PresKeyEX(RdPrt(S_KOM),&ky[SS_KOM]);
#endif            
			if(key==100)						//akceptacja zmian
			{
#if _KEY_BBOARD==0                
				WaitToLowEx(&ky[SS_RES]);
                WaitToLowEx(&ky[SS_BAR]);
#endif
#if _KEY_BBOARD==1                
				WaitToLowEx(&ky[SS_KOM]);
#endif                
				PZK[DtKNF.PRM].dta=PZK[DtKNF.PRM].bfdta;
				RSDT1.inDDTA=(unsigned char)PZK[DtKNF.PRM].dta;  //kkk
				UpdateKnfPar(DtKNF.PRM);
				DtKNF.KRK=2;
			}
//			key=PresKeyEX(RdPrt(S_OFF),&ky[SS_OFF]);
//			if(key>0)						//rezygnacja
//			{
//				WaitToLowEx(&ky[SS_OFF]);
//				PZK[DtKNF.PRM].bfdta=PZK[DtKNF.PRM].dta;
//				RSDT1.inDDTA=PZK[DtKNF.PRM].dta;
//				UpdateKnfPar(DtKNF.PRM);
//				DtKNF.KRK=2;
//			}
			break;
		}
		//daj czas na zapis parametrow i wyjdz z proc. konfiguracji
		case 4:
		{
			RSDT1.inPRM=0;
			RSDT1.inKRK=DtKNF.KRK;  
            if(MGSDf_NO_ODB)
            {
                if((RTS(_RTKONF1)>=_TKNFSAV))          //%%%%
                {
                    ExitKonf();
                    return;
                }                
            }
            else
            {
                if((RTS(_RTKONF1)>=_TKNFSAV)&&GetNKFN()>2)          //%%%%
                {
                    ExitKonf();
                    return;
                }
            }
            break;
		}
	}
}
/*---------------------------------------------------------------------------------*/
