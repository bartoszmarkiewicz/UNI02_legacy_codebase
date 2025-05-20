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
	0 ,99,0b00001100,			//0 moc startowa
	0 ,99,0b00001100,           //1 moc maksymalna CW
	0 ,99,0b00001100,           //2 moc maksymalna CO
	0 , 1,0b01000100,			//3 rodzaj gazu
	0 , 1,0b01000100,			//4 aktywacja zegara antylegionella    
	0 , 3,0b01000000,			//5 kod kotla
	0 , 1,0b01000100,			//6 rodzaj obiegu
	0 , 1,0b01000100,			//7 ogrzewanie podlogowe
	0 , 1,0b01000100,			//8 wybor rodzaju czujnika cisnienia
	0 , 1,0b01000100,			//9 rodzaj pompy
	5 ,25,0b01010100,			//10 delta T dla pompy
	15,99,0b01001100,           //11 minimalny wydatek pompy
	25,99,0b01001100,			//12 maksymalny wydatek pompy    
	25,99,0b01001100,			//13 maksymalny wydatek pompy dla obiegu CW 
	0 ,15,0b01010100,			//14 histereza wylaczenia grzania CO (ponad nastawe)
	70,89,0b01010100,			//15 temp CO przy grzaniu CW zasobnika
	0 ,20,0b01010100,			//16 poprawka dla regulagtora pogodowego
    _MIN_PDCO ,_MAX_PCO,0b01010100,	//17 maksymalna wartosc graniczna CO
    0 , 2,0b01000100,			//18 tryb pracy regulatora pogodowego
    0 , 1,0b01000100,			//19 histereza wylaczenia CW(4F)
    0 , 1,0b01000100			//20 we. sygnalu timera zasobnika (1F)    
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
//Podstawowe cechy kolejnych parametrow
//MIN,MAX,DTA,DYN_DTA,RLD,NO_MSG,HIGH,T,B,SHW
/*DtPZK PZK[_KNPAR]=
{
	0 ,99,0,0,0,0,0,0,1,1,			//0 moc startowa
	0 ,99,0,0,0,0,0,0,1,1,			//1 moc maksymalna CW
	0 ,99,0,0,0,0,0,0,1,1,			//2 moc maksymalna CO
	0 , 1,0,0,0,1,0,0,0,1,			//3 rodzaj gazu    
	0 , 1,0,0,0,1,0,0,0,1,			//4 aktywacja zegara antylegionella    
	0 , 3,0,0,0,1,0,0,0,0,			//5 kod kotla
	0 , 1,0,0,0,1,0,0,0,1,			//6 rodzaj obiegu
	0 , 1,1,0,0,1,0,0,0,1,			//7 ogrzewanie podlogowe
	0 , 1,0,0,0,1,0,0,0,1,			//8 wybor rodzaju czujnika cisnienia
	0 , 1,0,0,0,1,0,0,0,1,			//9 rodzaj pompy
	5 ,25,0,0,0,1,0,1,0,1,			//10 delta T dla pompy
	15,99,0,0,0,1,0,0,1,1,			//11 minimalny wydatek pompy
	25,99,0,0,0,1,0,0,1,1,			//12 maksymalny wydatek pompy 
	25,99,0,0,0,1,0,0,1,1,			//13 maksymalny wydatek pompy dla obiegu CW
	0 ,10,0,0,0,1,0,1,0,1,			//14 histereza wylaczenia grzania CO (ponad nastawe)
	70,89,0,0,0,1,0,1,0,1,			//15 temp CO przy grzaniu CW zasobnika 
	0 ,20,0,0,0,1,0,1,0,1,			//16 poprawka dla regulagtora pogodowego    
    _MIN_PDCO ,_MAX_PCO,0,0,0,1,0,1,0,1,	//17 maksymalna wartosc graniczna CO dla regulatora pogodowego    
};*/
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
	WaitToLowEx(&ky[SS_OFF]);
	WaitToLowEx(&ky[SS_SET]);
#endif
#if _KEY_BBOARD==1    
	WaitToLowEx(&ky[SS_OFF]);
	WaitToLowEx(&ky[SS_KOM]);
#endif    
	PFNf_KnfDisErr=0;
}
/*---------------------------------------------------------------------------------*/
//Aktualizacja wartosci parametru (po akceptacji zmiany)
void UpdateKnfPar(unsigned char par)
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
                RSDT1.inRGZ=PZK[3].dta;         //3 rodzaj gazu		
            break;        
            case 4:
                RSDT1.inWANL=PZK[4].dta;		//4 aktywacja zegara antylegionella		
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
                RSDT1.inRDPM=PZK[9].dta;		//9 rodzaj pompy
            break;
            case 10:
                RSDT1.inDTPM=PZK[10].dta;		//10 delta T dla pompy
            break;
            case 11:
                            RSDT1.inMDPM=PZK[11].dta;                   //11 minimalny wydatek pompy
                            if(RSDT1.inMDPM>RSDT1.inMGPM)
                            {
                                PZK[12].dta=RSDT1.inMDPM;
                                RSDT1.inMGPM=PZK[12].dta;
                            }
            break;
            case 12:
                            RSDT1.inMGPM=PZK[12].dta;                   //12 maksymalny wydatek pompy
                            if(RSDT1.inMGPM<RSDT1.inMDPM)
                            {
                                PZK[11].dta=RSDT1.inMGPM;
                                RSDT1.inMDPM=PZK[11].dta;
                            }
            break;
            case 13:
                RSDT1.inMGPM2=PZK[13].dta;		//13 maksymalny wydatek pompy dla obiegu CW
            break;
            case 14:
                RSDT1.inHSCO=PZK[14].dta;		//14 histereza wylaczenia grzania CO (ponad nastawe)  
            break;
            case 15:
                RSDT1.inCONW=PZK[15].dta;		//15 temp CO przy grzaniu CW zasobnika
            break; 
            case 16:
                RSDT1.inPROP=PZK[16].dta;		//16 poprawka dla regulagtora pogodowego
            break; 
            case 17:
                RSDT1.inPGMX=PZK[17].dta;		//17 wartosc graniczna dla obiegu CO
            break; 
            case 18:
                RSDT1.inPGDR=PZK[18].dta;		//18 tryb pracy regulatora pogodowego   
            break;        
            case 19:
                RSDT1.inCWMG=PZK[19].dta;		//29 histereza wylaczenia CW(4F)   
            break;            
            case 20:
                RSDT1.inCWTM=PZK[20].dta;		//30 we. sygnalu timera zasobnika (1F)   
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
            case 17:        //granica (min,max) dla maksymalnej wartosci granicznej CO
            {
                if(RSDT1.inOPDG)    //ogrzewanie podlogowe?
                {
                    PZK[17].min=_MIN_PDCO;
                    PZK[17].max=_MAX_PDCO;
                }
                else                //ogrzewanie tradycyjne
                {
                    PZK[17].min=_MIN_PCO;
                    PZK[17].max=_MAX_PCO;
                }            
                if(PZK[17].bfdta<PZK[17].min) PZK[17].bfdta=PZK[17].min;
                if(PZK[17].bfdta>PZK[17].max) PZK[17].bfdta=PZK[17].max; 
                if(PZK[17].dta<PZK[17].min) PZK[17].dta=PZK[17].min;
                if(PZK[17].dta>PZK[17].max) PZK[17].dta=PZK[17].max;          
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
/*---------------------------------------------------------------------------------*/
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
	//4 aktywacja zegara antylegionella
    if(RSDT1.inKODKT==_KODKT_1F)    //jednofunkcyjny?
    {    
        PZK[4].fl.shw=1;//GetBit2(RSDT1.outKSTAT0,_KC1F);
    }
    else
    {
        PZK[4].fl.shw=0;
    }
    
	//5 kod kotla
	PZK[5].fl.shw=GetBit2(RSDT1.outKSTAT0,_ZCFM0);//pokaz/ukryj parametry powiazane

	//10 delta T dla pompy
	//11 minimalny wydatek pompy
    //12 maksymalny wydatek pompy dla CO
    //13 maksymalny wydatek pompy dla CW
	if(RSDT1.inRDPM==0)				//pokaz/ukryj parametry powiazane
	{
		PZK[10].fl.shw=0;				//RSDT1.inDTPM
		PZK[11].fl.shw=0;				//RSDT1.inMDPM
		PZK[12].fl.shw=0;				//RSDT1.inMGPM
		PZK[13].fl.shw=0;				//RSDT1.inMGPM2        
	}
	else
	{
        if(RSDT1.inWOBG)            //obieg zamkniety?
        {
            PZK[10].fl.shw=1;                  //RSDT1.inDTPM        
            PZK[11].fl.shw=1;                  //RSDT1.inMDPM
            PZK[12].fl.shw=1;                  //RSDT1.inMGPM
            if(RSDT1.inKODKT==_KODKT_1F)    //jednofunkcyjny?
            {
                PZK[13].fl.shw=1;				//RSDT1.inMGPM2
            }
            else
            {
                PZK[13].fl.shw=0;				//RSDT1.inMGPM2            
            }
        }
        else
        {
            PZK[10].fl.shw=0;				//RSDT1.inDTPM
            PZK[11].fl.shw=0;				//RSDT1.inMDPM
            PZK[12].fl.shw=1;				//RSDT1.inMGPM
            if(RSDT1.inKODKT==_KODKT_1F)    //jednofunkcyjny?
            {
                PZK[13].fl.shw=1;				//RSDT1.inMGPM2
            }
            else
            {
                PZK[13].fl.shw=0;				//RSDT1.inMGPM2            
            }            
        }
	}

    //15 temp CO przy grzaniu CW zasobnika
    if(RSDT1.inKODKT==_KODKT_1F)    //jednofunkcyjny?
    {
        PZK[15].fl.shw=1;				//RSDT1.inCONW
    }
    else
    {
        PZK[15].fl.shw=0;				//RSDT1.inCONW            
    } 
    
    //16 przesuniecie krzywej grzania
    if(RSDT1.vKNF>=5)
    {
        if(RSDT1.inPGDR)                       //rodzaj regulatora pogodowego
        {
            PZK[16].fl.shw=1;                  //RSDT1.inPROP                
        }
        else
        {
            PZK[16].fl.shw=0;       
        }
    }

    //19 histereza wylaczenia CW(4F)
    if(RSDT1.inKODKT==_KODKT_4F)    //dwufunkcyjny,monotermiczny ?
    {
        PZK[19].fl.shw=1;                   //RSDT1.inCWMG
    }
    else
    {
        PZK[19].fl.shw=0;                   //RSDT1.inCWMG        
    }
    //20 we. sygnalu timera zasobnika (1F)
    if(RSDT1.inKODKT==_KODKT_1F)    //jednofunkcyjny?
    {
        PZK[20].fl.shw=1;                   //RSDT1.inCWTM
    }
    else
    {
        PZK[20].fl.shw=0;                   //RSDT1.inCWTM        
    }
}
/*---------------------------------------------------------------------------------*/
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
        default:
            return _KNPAR;
    }  
}
unsigned char SetKNPAR(void)
{
    DtKNF.KNPAR=GetKNPAR();
    return DtKNF.KNPAR;
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Glowna procedura konfiguracji (wykonywana w petli glownej)
void MKonfiguracja(void)
{
	volatile unsigned char key,fst;
    static unsigned int bnrp,bfdta;
    
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
			key=PresKeyEX(RdPrt(S_OFF)&&RdPrt(S_SET),&ky[SS_OFF_SET]);
#endif
#if _KEY_BBOARD==1             
			key=PresKeyEX(RdPrt(S_OFF)&&RdPrt(S_KOM),&ky[SS_OFF_SET]);
#endif			
			if(PFNf_enKNF&&key!=100&&key>=50) //przytrzymano przycisk wejscia do procedury konfiguracji?
			{
#if _KEY_BBOARD==0                
				WaitToLowEx(&ky[SS_OFF_SET]);			
				WaitToLowEx(&ky[SS_OFF]);
				WaitToLowEx(&ky[SS_SET]);
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
            if(RSDT1.vSTD&&RSDT1.vKNF)                                          //mamy komplet danych koniecznych do porawnej pracy procedury?
            {            
                if(RSDT1f_RD_KNF&&PFNf_opKNF)                                   //odczytaj dane wejsciowe parametrów ktore maja byc modyfikowane
                {
                    RSDT1f_RRD_KNF=1;
                    RSDT1.inMSTR=PZK[0].dta=PZK[0].bfdta=RSDT1.outMSTR;			//0 moc startowa
                    RSDT1.inMMAXU=PZK[1].dta=PZK[1].bfdta=RSDT1.outMMAXU;		//1 moc maksymalna CW
                    RSDT1.inMMAXG=PZK[2].dta=PZK[2].bfdta=RSDT1.outMMAXG;		//2 moc maksymalna CO
                    RSDT1.inRGZ=PZK[3].dta=PZK[3].bfdta=RSDT1.outRGZ;			//3 rodzaj gazu                
                    RSDT1.inWANL=PZK[4].dta=PZK[4].bfdta=RSDT1.outWANL;			//4 aktywacja zegara antylegionella
                    RSDT1.inKODKT=PZK[5].dta=PZK[5].bfdta=RSDT1.outKODKT;		//5 kod kotla
                    RSDT1.inWOBG=PZK[6].dta=PZK[6].bfdta=RSDT1.outWOBG;			//6 rodzaj obiegu
                    RSDT1.inOPDG=PZK[7].dta=PZK[7].bfdta=RSDT1.outOPDG;			//7 ogrzewanie podlogowe
                    RSDT1.inTPCS=PZK[8].dta=PZK[8].bfdta=RSDT1.outTPCS;			//8 wybor rodzaju czujnika cisnienia

                    RSDT1.inRDPM=PZK[9].dta=PZK[9].bfdta=RSDT1.outRDPM;         //9 rodzaj pompy
                    RSDT1.inDTPM=PZK[10].dta=PZK[10].bfdta=RSDT1.outDTPM;		//10 delta T dla pompy
                    RSDT1.inMDPM=PZK[11].dta=PZK[11].bfdta=RSDT1.outMDPM;		//11 minimalny wydatek pompy
                    RSDT1.inMGPM=PZK[12].dta=PZK[12].bfdta=RSDT1.outMGPM;		//12 maksymalny wydatek pompy    

                    RSDT1.inMGPM2=PZK[13].dta=PZK[13].bfdta=RSDT1.outMGPM2;		//13 maksymalny wydatek pompy dla obiegu CW                 
                    RSDT1.inHSCO=PZK[14].dta=PZK[14].bfdta=RSDT1.outHSCO;		//14 histereza wylaczenia grzania CO (ponad nastawe) 
                    RSDT1.inCONW=PZK[15].dta=PZK[15].bfdta=RSDT1.outCONW;		//15 temp CO przy grzaniu CW zasobnika

                    RSDT1.inPROP=PZK[16].dta=PZK[16].bfdta=RSDT1.outPROP;		//16 poprawka dla regulagtora pogodowego
                    RSDT1.inPGMX=PZK[17].dta=PZK[17].bfdta=RSDT1.outPGMX;       //17 maksymalna wartosc graniczna CO dla regulatora pogodowego
                    
                    RSDT1.inPGDR=PZK[18].dta=PZK[18].bfdta=RSDT1.outPGDR;       //18 rodzaj regulatora pogodowego
                    RSDT1.inCWMG=PZK[19].dta=PZK[19].bfdta=RSDT1.outCWMG;       //19 histereza wylaczenia CW(4F)                    
                    RSDT1.inCWTM=PZK[20].dta=PZK[20].bfdta=RSDT1.outCWTM;       //20 we. sygnalu timera zasobnika (1F)
                    
                    DecShowHide();												//pokaz/ukryj parametry
                    if(PZK[5].fl.shw) DtKNF.PRM=nrp=5;								//domyslnie zaznaczony parametr nr 5 - wybor typu kotla
                    else DtKNF.PRM=nrp=0;										//domyslnie zaznaczony parametr nr 0 - moc startowa
                    //kompatybilnosc wsteczna 
                    SetKNPAR();                                                 //okresl liczbe parametrow w zaleznosci od wersji oprogramowania
                    RSDT1f_RRD_KNF=0;                
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
			Mark(_GZ_BAR,0);											
			Mark(_GZ_TCO,0);
			Mark(_GZ_MXCW,0);
            Mark(_GZ_MNCW,0);            
			Mark(_GZ_MNCO,0);
			Mark(_GZ_MXCO,0);            
			DecShowHide();
			if((nrp<_KNPAR)&&(PZK[nrp].fl.shw))							//parametr mo¿e byc wyswietlony?
			{
#if _KEY_BBOARD==0  
                bnrp=nrp;
				ReadParam(&bnrp,&zm,&zp,0,DtKNF.KNPAR-1,1,0);                
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
			PrintPR(led_P,DtKNF.PRM+1,0);								//kod rx
#if _KEY_BBOARD==0             
			key=PresKeyEX(RdPrt(S_SET),&ky[SS_SET]);
#endif
#if _KEY_BBOARD==1             
			key=PresKeyEX(RdPrt(S_KOM),&ky[SS_KOM]);
#endif            
			if(key>0)													//wybrano parametr
			{
#if _KEY_BBOARD==0                
				WaitToLowEx(&ky[SS_SET]);		
				WaitToLowEx(&ky[SS_OFF]);
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
			key=PresKeyEX(RdPrt(S_OFF),&ky[SS_OFF]);
			if(key!=100&&key>=20)
			{
				WaitToLowEx(&ky[SS_OFF]);		
				StartRTS(_RTKONF1);										//start timera zapisu
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
			if((PZK[DtKNF.PRM].max-PZK[DtKNF.PRM].min)>10) fst=1;			//decyzja o zmiennej szybkosci inkrementacji/dekrementacji
			else fst=0;
#if _KEY_BBOARD==0 
            bfdta=PZK[DtKNF.PRM].bfdta;
			ReadParam(&bfdta,&zm,&zp,PZK[DtKNF.PRM].min,PZK[DtKNF.PRM].max,PZK[DtKNF.PRM].fl.rld,fst);            
            PZK[DtKNF.PRM].bfdta=bfdta;
#endif            
#if _KEY_BBOARD==1         
            bfdta=PZK[DtKNF.PRM].bfdta;
            ReadParamCO(&bfdta,&zm,&zp,PZK[DtKNF.PRM].min,PZK[DtKNF.PRM].max,PZK[DtKNF.PRM].fl.rld,fst);
            PZK[DtKNF.PRM].bfdta=bfdta;
#endif			
			RSDT1.inDDTA=(char)PZK[DtKNF.PRM].bfdta;
			//Formatuj wyswietlanie
			if(!(!PFNf_KnfDisErr&&(PFNf_ferr||PFNf_fmsg)))					//nie jest wyswietlany kod awaryjny?
			{
				if(!PZK[DtKNF.PRM].fl.high) PrintWPR(0,PZK[DtKNF.PRM].bfdta);
				else PrintWPR(PZK[DtKNF.PRM].bfdta,0);
				Mark(_GZ_MXCW,PZK[DtKNF.PRM].bfdta>=PZK[DtKNF.PRM].max);
				Mark(_GZ_MNCO,PZK[DtKNF.PRM].bfdta<=PZK[DtKNF.PRM].min);
				Mark(_GZ_TCO,PZK[DtKNF.PRM].fl.T);
				//Mark(_GZ_BAR,PZK[DtKNF.PRM].B);
			}
			else
			{
 				EndPR();
				Mark(_GZ_MXCW,0);
				Mark(_GZ_MNCW,0);                
				Mark(_GZ_MNCO,0);
                Mark(_GZ_MXCO,0);
				Mark(_GZ_TCO,0);
				Mark(_GZ_BAR,0);
			}
#if _KEY_BBOARD==0             
			key=PresKeyEX(RdPrt(S_SET),&ky[SS_SET]);
#endif
#if _KEY_BBOARD==1             
			key=PresKeyEX(RdPrt(S_KOM),&ky[SS_KOM]);
#endif            
			if(key>0)						//akceptacja zmian
			{
#if _KEY_BBOARD==0                
				WaitToLowEx(&ky[SS_SET]);
#endif
#if _KEY_BBOARD==1                
				WaitToLowEx(&ky[SS_KOM]);
#endif                
				PZK[DtKNF.PRM].dta=PZK[DtKNF.PRM].bfdta;
				RSDT1.inDDTA=PZK[DtKNF.PRM].dta;
				UpdateKnfPar(DtKNF.PRM);
				DtKNF.KRK=2;
			}
			key=PresKeyEX(RdPrt(S_OFF),&ky[SS_OFF]);
			if(key>0)						//rezygnacja
			{
				WaitToLowEx(&ky[SS_OFF]);
				PZK[DtKNF.PRM].bfdta=PZK[DtKNF.PRM].dta;
				RSDT1.inDDTA=PZK[DtKNF.PRM].dta;
				UpdateKnfPar(DtKNF.PRM);
				DtKNF.KRK=2;
			}
			break;
		}
		//daj czas na zapis parametrow i wyjdz z proc. konfiguracji
		case 4:
		{
			RSDT1.inPRM=0;
			RSDT1.inKRK=DtKNF.KRK;            
			if((RTS(_RTKONF1)>=_TKNFSAV)) 
			{
				ExitKonf();
				return;
			}
		}
	}
}
/*---------------------------------------------------------------------------------*/
