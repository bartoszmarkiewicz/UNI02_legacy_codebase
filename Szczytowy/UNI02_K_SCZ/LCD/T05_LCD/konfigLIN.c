/*konfigLIN.c*/
//Procedury zwiazane z wyborem typu gazu i poziomow
//mocy startowej oraz majksymalnej dla CO i CW
/*---------------------------------------------------------------------------------*/
//#include <pic18.h>
//#include <htc.h>
#include <xc.h>
#include "global.h"					//parametry globalne
#include "konfigLIN.h"					//parametry lokalne
#include "eeprom.h"
#include "pomiar.h"
#include "wsw.h"
#include "main.tp.h"
#include "keypad.tp.h"
#include "print.h"

#if (_EXT_BOARD>=2)
    #include "LIN1SInterpreter.h"
#else
    #include "mgs_rs9b.tp.h"
#endif

volatile DtKONF2 DtLKNF;
volatile unsigned int nrp=0;
unsigned char zm,zp;


//extern volatile unsigned char x,y;

//extern volatile MGSRSData MGSD;
extern tpMAIN M;
extern volatile DataPFN PFN;
//extern volatile tpRSDTA1 RSDT1;
extern volatile StPin ky[];
extern volatile StPin xxky[];
/*---------------------------------------------------------------------------------*/
//Podstawowe cechy kolejnych parametrow
//MIN,MAX,(RLD,NO_MSG,HIGH,T,B,SHW,x,x)
//volatile CDtPZK2 LCPZK={0 ,99,0b00001100};
/*---------------------------------------------------------------------------------*/
//Podstawowe cechy kolejnych parametrow
//MIN,MAX,DTA,DYN_DTA,(RLD,NO_MSG,HIGH,T,B,SHW)
volatile static DtPZK2 LPZK;

volatile tpMOD2 L2MOD;
/*---------------------------------------------------------------------------------*/
//Inicjalizacja danych zwiazanych z kolejnymi parametrami konfiguracyjnymi
void InitLPZK(void)
{
    LPZK.dta=0;
    LPZK.bfdta=0;        
    LPZK.min=0;
    LPZK.max=0;    
    LPZK.fl=0;        
}
/*---------------------------------------------------------------------------------*/
//Odblokowanie procedury wejsciowej konfiguracji
//po uruchomieniu sterownika na pozycji OFF
//UEAGA: umiescic przed petla glowna
void InitKonfLIN(void)
{
		DtLKNF.KRK=0;
		DtLKNF.PRM=0;
        InitLPZK();                                                          //Inicjalizacja danych charakterystycznych kolejnych parametrow
}
/*---------------------------------------------------------------------------------*/
//wyjscie z procedury konfiguracji po modyfikacji parametrow
void ExitLKonf(void)
{
	PFNf_aktywne_LKNF=0;
	EndPR();								//wygas wskaz parametru
	ClrAllMarks();							//wygas znaki konfiguracji
	DtLKNF.PRM=0;
	DtLKNF.KRK=0;
    ClrRefKnfDta();
    ClrInOutKnfDTA();  
    LIN2SetModKnfDtaFlg(0);                                         //zeruj flage oczekiwania na dane konfiguracji 
    //SetStatusKnfDta(0,0,0);   
#if _KEY_BBOARD==0                
				WaitToLowEx(&ky[SS_OFF_SET_KUP]);			
				WaitToLowEx(&ky[SS_OFF]);
				WaitToLowEx(&ky[SS_SET]);
				WaitToLowEx(&ky[SS_KUP]);                
#endif  
#if _KEY_BBOARD==1                
				WaitToLowEx(&ky[SS_OFF_SET_KUP]);			
				WaitToLowEx(&ky[SS_OFF]);
				WaitToLowEx(&ky[SS_KOM]);
				WaitToLowEx(&ky[SS_KUP_CO]);                
#endif   
    EndKrs();
	PFNf_LKnfDisErr=0;
}
/*---------------------------------------------------------------------------------*/
//Aktualizacja liczby modulow oraz liczby parametrow najblizszego modulu (_CLIN_RRD_KNF)
void LIN2GetKnfPar(unsigned char nmods, unsigned char npar)
{
    if(LIN2SGetMasterIDO())
    {
        L2MOD.KNMODS=nmods;
        L2MOD.FKNPAR=npar;
    }
    else
    {
        L2MOD.KNMODS=0;
        L2MOD.FKNPAR=0;        
    }
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Ustaw flage oczekiwania na dane KPAR z modulu
void LIN2SetModKnfDtaFlg(unsigned char dt)
{
    L2MOD._getDta=(!dt)?0:1;
}
//Odczyt flagi oczekiwania na dane KPAR
unsigned char LIN2GetModKnfDtaFlg(void)
{
    return (L2MOD._getDta)?1:0;
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Zerowanie wdanych KPAR
void ClrInOutKnfDTA(void)
{
    volatile unsigned char i;
    for(i=0;i<16;i++)
    {
        L2MOD.inDTA[i]=0;
        L2MOD.bfIN[i]=0;
        L2MOD.outDTA[i]=0;
        L2MOD.bfOUT[i]=0;
    }    
}
//Zerowanie wskaznika odswiezenia danych
void ClrRefKnfDta(void)
{
//    L2MOD.outREF0=0;
//    L2MOD.outREF1=0;
//    L2MOD.outREF2=0;
//    L2MOD.outREF3=0;    
    L2MOD.inREF=0;
    L2MOD.inREF0=0;
    L2MOD.inREF1=0;
    L2MOD.inREF2=0;
    L2MOD.inREF3=0;    
}
//Sprawdzenie wskaznika odswiezenia danych
unsigned ChkRefKnfDta(void)
{
    if(L2MOD.inREF) return 1;
    return 0;
}
//Ustawienie statusu danych
unsigned char SetStatusKnfDta(unsigned char stat, unsigned char nr_mod, unsigned char nr_par)
{
    volatile static unsigned char zn=0;
    volatile unsigned char xmod;
    
    if((zn+1)<=3) zn=zn+1;                                  //znacznik ramki
    else zn=0;
    
    if(nr_mod>0b00111111) nr_mod=0b00111111;
    xmod=((nr_mod&0b00111111)|((zn&0b00000011)<<6));        //znacznik ramki + nr modulu
    
    L2MOD.outDTA[_MOD_OUT0_STATUS]=stat;  
    L2MOD.outDTA[_MOD_OUT1_STATUS]=stat; 
    L2MOD.outDTA[_MOD_OUT2_STATUS]=stat; 
    L2MOD.outDTA[_MOD_OUT3_STATUS]=stat;    

    L2MOD.outDTA[_MOD_OUT0_NRMOD]=xmod;  
    L2MOD.outDTA[_MOD_OUT0_NRPAR]=nr_par; 
    L2MOD.outDTA[_MOD_OUT0_COUNT]=1;

    L2MOD.outDTA[_MOD_OUT1_NRMOD]=xmod;  
    L2MOD.outDTA[_MOD_OUT1_NRPAR]=nr_par; 
    L2MOD.outDTA[_MOD_OUT2_NRMOD]=xmod;  
    L2MOD.outDTA[_MOD_OUT2_NRPAR]=nr_par;  
    L2MOD.outDTA[_MOD_OUT3_NRMOD]=xmod;  
    L2MOD.outDTA[_MOD_OUT3_NRPAR]=nr_par;  
    
    return xmod;
}
//Kontrola zgodnosci statusu danych
unsigned ChkStatusKnfDta(unsigned char dt, unsigned char nr_mod, unsigned char nr_par)
{
    volatile signed int min,max,dta;
    if((L2MOD.outDTA[_MOD_OUT0_STATUS]!=dt)&&(L2MOD.outDTA[_MOD_OUT1_STATUS]!=dt)&&(L2MOD.outDTA[_MOD_OUT2_STATUS]!=dt)&&(L2MOD.outDTA[_MOD_OUT3_STATUS]!=dt)) return 0;
    if((L2MOD.inDTA[_MOD_IN0_NRMOD]!=nr_mod)&&(L2MOD.inDTA[_MOD_IN1_NRMOD]!=nr_mod)&&(L2MOD.inDTA[_MOD_IN2_NRMOD]!=nr_mod)&&(L2MOD.inDTA[_MOD_IN3_NRMOD]!=nr_mod)) return 0;
    if((L2MOD.inDTA[_MOD_IN0_NRPAR]!=nr_par)&&(L2MOD.inDTA[_MOD_IN1_NRPAR]!=nr_par)&&(L2MOD.inDTA[_MOD_IN2_NRPAR]!=nr_par)&&(L2MOD.inDTA[_MOD_IN3_NRPAR]!=nr_par)) return 0;
       
    if(L2MOD.inDTA[_MOD_IN0_NPAR])      //modul ma parametry?
    {
        min=L2MOD.inDTA[_MOD_IN2_DTA1];
        min=min<<8;
        min|=L2MOD.inDTA[_MOD_IN2_DTA0];

        max=L2MOD.inDTA[_MOD_IN3_DTA1];
        max=max<<8;
        max|=L2MOD.inDTA[_MOD_IN3_DTA0];

        if(min==max) return 0;              //bledne granice

        dta=L2MOD.inDTA[_MOD_IN1_DTA1];
        dta=dta<<8;
        dta|=L2MOD.inDTA[_MOD_IN1_DTA0];

        if(dta<min||dta>max) return 0;      //bledna dana
    }
    return 1;
}
//Kontrola zgodnosci danej do zapisu i danej zwroconej z modulu
unsigned ChkKnfDta(signed int dt)
{
    volatile unsigned char lo_dt, hi_dt;
    lo_dt=(unsigned char)(dt&0x00ff);
    hi_dt=(unsigned char)((dt&0xff00)>>8);
   
    if((L2MOD.outDTA[_MOD_OUT1_DTA0]!=lo_dt)&&(L2MOD.inDTA[_MOD_IN1_DTA0]!=lo_dt)) return 0;
    if((L2MOD.outDTA[_MOD_OUT1_DTA1]!=hi_dt)&&(L2MOD.inDTA[_MOD_IN1_DTA1]!=hi_dt)) return 0;    
    return 1;
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Kontrola czy nr modulu jest w dozwolonym zakresie, a w przypadku jednego modulu czy ma parametry
unsigned char ChkMOD(void)
{
    DtLKNF.KNMODS=L2MOD.KNMODS;     //ogolna liczba modulow
    if((DtLKNF.MOD>DtLKNF.KNMODS)||((DtLKNF.KNMODS==1)&&!L2MOD.FKNPAR))
    {
        EndKrs();
        PrintNoPP();
        StartRTS(_RTLKONFI);
        return 1;
    }
    return 0;
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Czas oczekiwania uzalezniony od liczby modulow
unsigned char SetKnfTim(unsigned char nmod)
{
    if(nmod*_TLKNFINI<=255)
    {
        return (nmod*_TLKNFINI);
    }
    return 255;
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Glowna procedura konfiguracji (wykonywana w petli glownej)
void LKonfiguracja(void)
{
	volatile unsigned char key,fst,min_par,tm,dt;//,kk;
    static signed int bnrp,bfdta;
    
	if(PFNf_aktywne_LKNF&&(DtLKNF.KRK>1)&&!PFNf_FLIN) 					//procedura z jakiegos powodu nieaktywna w uk³adzie UNI-02?
	{
		ExitLKonf();
		return;
	}

	if(PFNf_aktywne_LKNF&&(RTM(_RTLKONF)>=_TLKNFEXT))                   //uplynol maksymalny dozwolony czas aktywnosci procedury lub wylaczenie awaryjne z blokada w ukladzie UNI-02?
	{
		ExitLKonf();
		return;
	}

	switch(DtLKNF.KRK)
	{
		//warunek wejscia do procedury modyfikacji parametrow
		case 0:
		{
			
			if(!PFNf_off) return;					//aktywny tryb OFF?
			
			//przytrzymano przycisk wejscia do procedury konfiguracji 
#if _KEY_BBOARD==0             
			key=PresKeyEX(RdPrt(S_OFF)&&RdPrt(S_SET)&&RdPrt(S_KUP),&ky[SS_OFF_SET_KUP]);
            //kk=RdPrt(S_KUP);
#endif
#if _KEY_BBOARD==1             
			key=PresKeyEX(RdPrt(S_OFF)&&RdPrt(S_KOM)&&RdPrt(S_KUP_CO),&ky[SS_OFF_SET_KUP]);
            //kk=RdPrt(S_KUP_CW);
#endif			
			if(!PFNf_aktywne_KNF&&key!=100&&key>=50) //przytrzymano przycisk wejscia do procedury konfiguracji?
			{
#if _KEY_BBOARD==0                
				WaitToLowEx(&ky[SS_OFF_SET_KUP]);			
				WaitToLowEx(&ky[SS_OFF]);
				WaitToLowEx(&ky[SS_SET]);
				WaitToLowEx(&ky[SS_KUP]);                
#endif  
#if _KEY_BBOARD==1                
				WaitToLowEx(&ky[SS_OFF_SET_KUP]);			
				WaitToLowEx(&ky[SS_OFF]);
				WaitToLowEx(&ky[SS_KOM]);
				WaitToLowEx(&ky[SS_KUP_CO]);                
#endif                
                PFNf_aktywne_LKNF=1;
				StartRTM(_RTLKONF);					//start timera aktywnosci proc. konfig
                if(!PFNf_FLIN||!L2MOD.KNMODS)
                {
                    PrintKrs();
                }
				DtLKNF.KNPAR=1;                     //domyslna maksymalna liczba parametrow (obs.kompatybilnosci w dol)

                ClrRefKnfDta();
                ClrInOutKnfDTA();               
//                SetStatusKnfDta(1,1,1);
                LIN2SetModKnfDtaFlg(0);                 //zeruj flage oczekiwania na dane konfiguracji 
				ClrAllMarks();						//wygas znaki nie zwiazane z konfiguracja
				StartRTS(_RTLKONFI);                //start timera oczekiwania na wejsciowe dane konfiguracyjne
                DtLKNF.KRK=1; 						//aktywacja proc. konfiguracji
                            
			}
			else 
			{
                DtLKNF.MOD=0;
				DtLKNF.PRM=0;
				DtLKNF.KRK=0;
                ClrRefKnfDta();
                ClrInOutKnfDTA();               
//                SetStatusKnfDta(1,1,1);
                LIN2SetModKnfDtaFlg(0);                 //zeruj flage oczekiwania na dane konfiguracji 
				return;
			}
			break;
		}
        case 1:
		{       
            InitLPZK();                                                          //Inicjalizacja danych charakterystycznych kolejnych parametrow
            //if(0)
			if((RTS(_RTLKONFI)>=_TLKNFINI))                                       //przekroczony limit czasu?
			{
                EndKrs();
                PrintNoPP();
                StartRTS(_RTLKONFI);
                DtLKNF.KRK=8;
				return;
			}
            //if(0)
            if(PFNf_FLIN&&L2MOD.KNMODS)                                     //mamy komplet danych koniecznych do porawnej pracy procedury?
            {
                MOD1f_RRD_KNF=1; 
                DtLKNF.MOD=1;
                DtLKNF.KNMODS=L2MOD.KNMODS;                                  //ogolna liczba modulow
                DtLKNF.KNPAR=L2MOD.FKNPAR;                                   //liczba parametrow modulu bezposrednio podlaczonego
                if(ChkMOD())                                                 //tylko jeden modul ale bez parametrow?
                {
                    DtLKNF.KRK=8;
                    return;                    
                }
                if(DtLKNF.KNMODS>1)                                         //podlaczono wieksza liczbe modulow?
                {
                    DtLKNF.PRM=nrp=0;                                       //wskaznik na PP0
                }
                else
                {
                    DtLKNF.PRM=nrp=1;                                        //wskaznik na PP1
                }
                ClrRefKnfDta();
                ClrInOutKnfDTA();  
                DtLKNF.XMOD=SetStatusKnfDta(2,1,1);//111
                LIN2SetModKnfDtaFlg(1);                                         //ustaw flage oczekiwania na dane konfiguracji                 
                MOD1f_RRD_KNF=0;   
#if _KEY_BBOARD==0                
				WaitToLowEx(&ky[SS_OFF_SET_KUP]);			
				WaitToLowEx(&ky[SS_OFF]);
				WaitToLowEx(&ky[SS_SET]);
				WaitToLowEx(&ky[SS_KUP]);
                WaitToLowEx(&xxky[SS_OFF]);
                WaitToLowEx(&xxky[SS_SET]);  
                WaitToLowEx(&xxky[SS_KUP]);
#endif  
#if _KEY_BBOARD==1                
				WaitToLowEx(&ky[SS_OFF_SET_KUP]);			
				WaitToLowEx(&ky[SS_OFF]);
				WaitToLowEx(&ky[SS_KOM]);
				WaitToLowEx(&ky[SS_KUP_CO]);   
                WaitToLowEx(&xxky[SS_OFF]);
                WaitToLowEx(&xxky[SS_KOM]);  
                WaitToLowEx(&xxky[SS_KUP_CO]);                
#endif  
                EndKrs();
                DtLKNF._keyUP=1;
                DtLKNF._keySET=1;
                DtLKNF._keyOFF=1;
                DtLKNF.KRK=2;
            }
			break;
		}
		//wybor parametru
		case 2:
		{
			PFNf_off=1;
			PFNf_lato=0;
			PFNf_zima=0;
			PFNf_KnfDisErr=1;
            Mark(_GZ_OFF,0);
			Mark(_GZ_BAR,0);											
			Mark(_GZ_TCO,0);
			Mark(_GZ_MXCW,0);	
            Mark(_GZ_MXCO,0);	
			Mark(_GZ_MNCO,0);
            DtLKNF.KNMODS=L2MOD.KNMODS;     //ogolna liczba modulow
            //----------------------------------------------------------------
            if(ChkMOD())                                                 //niewlasciwy nr modulu?
            {
                DtLKNF.KRK=8;
                return;                    
            }
            //----------------------------------------------------------------
            if(DtLKNF.KNMODS>1)             //wiecej niz jeden modul?
            {
                min_par=0;
            }
            else  
            {
                min_par=1;
                if(!DtLKNF.PRM)      //zmiana liczby modulow?                        
                {
                    DtLKNF.PRM=nrp=1; 						//aktywacja pierwszego parametru
                }
            } 
            //----------------------------------------------------------------
#if _KEY_BBOARD==0                
				if(!RdPrt(S_KUP)) DtLKNF._keyUP=0;
                if(!RdPrt(S_SET)) DtLKNF._keySET=0;
                if(!RdPrt(S_OFF)) DtLKNF._keyOFF=0;
#endif  
#if _KEY_BBOARD==1                
				if(!RdPrt(S_KUP_CO)) DtLKNF._keyUP=0; 
                if(!RdPrt(S_KOM)) DtLKNF._keySET=0;
                if(!RdPrt(S_OFF)) DtLKNF._keyOFF=0;            
#endif            
            if(!DtLKNF._keyUP&&!DtLKNF._keySET&&!DtLKNF._keyOFF)
            {
    #if _KEY_BBOARD==0  
                bnrp=nrp;
                ReadParam(&bnrp,&zm,&zp,min_par,DtLKNF.KNPAR,1,0);                
                nrp=bnrp;
    #endif
    #if _KEY_BBOARD==1  
                bnrp=nrp;
                ReadParamCO(&bnrp,&zm,&zp,min_par,DtLKNF.KNPAR,1,0);                
                nrp=bnrp;
    #endif                
                DtLKNF.PRM=nrp;
            }
			PrintPR2(led_P,DtLKNF.PRM,1);								//kod rx
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
                if(DtLKNF.PRM<=DtLKNF.KNPAR)
                {
                    if(DtLKNF.PRM)      //zmiana wartosci parametru
                    {
                        DtLKNF.XMOD=SetStatusKnfDta(1,DtLKNF.MOD,(DtLKNF.PRM&0b01111111));                                         //PRM=0,STAT=1 -wejsciowe dane konfiguracyjne
                        StartRTS(_RTLKONFI);                //start timera oczekiwania na wejsciowe dane konfiguracyjne 
                        PrintKrs();
                        DtLKNF.KRK=3;											//wejscie w edycje parametru
                    }
                    else  //wybor modulu
                    { 
                        LPZK.fl=0;
                        LPZK.rld=1;
                        //--------------- wartosc parametru
                        LPZK.dta=(signed int)DtLKNF.MOD;
                        LPZK.bfdta=LPZK.dta;
                        //--------------- granica minimalna
                        LPZK.min=1;
                        //--------------- granica maksymalna
                        LPZK.max=L2MOD.KNMODS;                       
                        DtLKNF.KRK=5;                                             
                    }
                }
                break;
			}
			key=PresKeyEX(RdPrt(S_OFF),&ky[SS_OFF]);
			if(key!=100&&key>=20)
			{
                EndPR();								//wygas wskaz parametru
                ClrAllMarks();
                ClrDig(1,1,0);
                Mark(_GZ_OFF,2);
                
				WaitToLowEx(&ky[SS_OFF]);
                ExitLKonf();
			}
			break;
		}
		//oczekiwanie na wartosc parametru - do edycji
		case 3:
		{
            if(ChkMOD())                                                 //niewlasciwy nr modulu?
            {
                DtLKNF.KRK=8;
                return;                    
            }   
             //----------------------------------------------------------------
            tm=SetKnfTim(DtLKNF.MOD);
			if((RTS(_RTLKONFI)>=tm))                                       //przekroczony limit czasu?
			{
				ExitLKonf();
				return;
			}
            if(PFNf_FLIN&&ChkRefKnfDta()&&ChkStatusKnfDta(1,DtLKNF.XMOD,(DtLKNF.PRM&0b01111111)))                              //mamy komplet danych koniecznych do porawnej pracy procedury?
            {
                MOD1f_RRD_KNF=1; 
                //---------------
                L2MOD.outDTA[_MOD_OUT1_DTA0]=L2MOD.inDTA[_MOD_IN1_DTA0];
                L2MOD.outDTA[_MOD_OUT1_DTA1]=L2MOD.inDTA[_MOD_IN1_DTA1];
                //--------------- format parametru
                LPZK.fl=L2MOD.inDTA[_MOD_IN0_FORMAT];
                //--------------- wartosc parametru
                LPZK.dta=L2MOD.inDTA[_MOD_IN1_DTA1];
                LPZK.dta=LPZK.dta<<8;
                LPZK.dta|=L2MOD.inDTA[_MOD_IN1_DTA0];
                LPZK.bfdta=LPZK.dta;
                //--------------- granica minimalna
                LPZK.min=L2MOD.inDTA[_MOD_IN2_DTA1];
                LPZK.min=LPZK.min<<8;
                LPZK.min|=L2MOD.inDTA[_MOD_IN2_DTA0];
                //--------------- granica maksymalna
                LPZK.max=L2MOD.inDTA[_MOD_IN3_DTA1];
                LPZK.max=LPZK.max<<8;
                LPZK.max|=L2MOD.inDTA[_MOD_IN3_DTA0];
                //--------------- 
                EndKrs();
                MOD1f_RRD_KNF=0;                
                if(!LPZK.exe)   //modyfikacja wartosci parametru? 
                {                       
                    DtLKNF.KRK=5;
                }
                else            //aktywacja multikomendy nadawania adresu?
                {
                    if(DtLKNF.MOD==1) PFNf_fOne=1;        //akceptuj tylko adres 0x01     //nnn
                    else PFNf_fOne=0;
                    ExitLKonf();                    
                }
            }
			break;
		}   
      
		//modyfikacji wartosci parametru
		case 5:
		{           
			PFNf_off=1;
			PFNf_lato=0;
			PFNf_zima=0;
			PFNf_LKnfDisErr=LPZK.no_msg;
            //----------------------------------------------------------------
            if(ChkMOD())                                                 //niewlasciwy nr modulu?
            {
                DtLKNF.KRK=8;
                return;                    
            }
            //----------------------------------------------------------------            
			if((LPZK.max-LPZK.min)>10) fst=1;			//decyzja o zmiennej szybkosci inkrementacji/dekrementacji
			else fst=0;
#if _KEY_BBOARD==0 
            bfdta=LPZK.bfdta;
			ReadParam(&bfdta,&zm,&zp,LPZK.min,LPZK.max,LPZK.rld,fst);            
            LPZK.bfdta=bfdta;
#endif            
#if _KEY_BBOARD==1         
            bfdta=LPZK.bfdta;
            ReadParamCO(&bfdta,&zm,&zp,LPZK.min,LPZK.max,LPZK.rld,fst);
            LPZK.bfdta=bfdta;
#endif			
			//Formatuj wyswietlanie
			if(!(!PFNf_LKnfDisErr&&(PFNf_ferr||PFNf_fmsg)))					//nie jest wyswietlany kod awaryjny?
			{
                PrintWPR2(LPZK.bfdta,LPZK.fl);

                if(LPZK.high)
                {
                    Mark(_GZ_MXCW,LPZK.bfdta>=LPZK.max);
                }
                else
                {
                    if(LPZK.max>=0)
                    {
                        if(LPZK.max>99)
                        {
                            Mark(_GZ_MXCW,LPZK.bfdta>=LPZK.max);
                        }
                        else
                        {
                            Mark(_GZ_MXCO,LPZK.bfdta>=LPZK.max);
                        }
                    }
                    else
                    {
                        if(LPZK.max<-9)
                        {
                            Mark(_GZ_MXCW,LPZK.bfdta>=LPZK.max);
                        }
                        else
                        {
                            Mark(_GZ_MXCO,LPZK.bfdta>=LPZK.max);
                        }                        
                    }
                }

                Mark(_GZ_MNCO,LPZK.bfdta<=LPZK.min);
                Mark(_GZ_TCO,LPZK.T);
                Mark(_GZ_OFF,0);
			}
			else
			{
 				EndPR();
				Mark(_GZ_MXCW,0);	
                Mark(_GZ_MXCO,0);	
				Mark(_GZ_MNCO,0);
				Mark(_GZ_TCO,0);
				Mark(_GZ_BAR,0);
                Mark(_GZ_OFF,0);
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
                if(DtLKNF.PRM)      //parametr
                {
                    LPZK.dta=LPZK.bfdta;
                    L2MOD.outDTA[_MOD_OUT1_DTA0]=(unsigned char)(LPZK.dta&0x00ff);
                    L2MOD.outDTA[_MOD_OUT1_DTA1]=(unsigned char)((LPZK.dta&0xFF00)>>8);
                    DtLKNF.XMOD=SetStatusKnfDta(1,DtLKNF.MOD,(DtLKNF.PRM|0b10000000));                                         //PRM=wr|nr,STAT=1 -wejsciowe dane konfiguracyjne
                    ClrRefKnfDta();
                    StartRTS(_RTLKONFI);										//start timera zapisu
                    Mark(_GZ_MXCW,0);	
                    Mark(_GZ_MXCO,0);
                    Mark(_GZ_MNCO,0);
                    PrintKrs();                   
                    DtLKNF.KRK=6;
                }
                else                //nr modulu
                {
                    LPZK.dta=LPZK.bfdta;
                    DtLKNF.MOD=(unsigned char)LPZK.dta;
                    DtLKNF.XMOD=SetStatusKnfDta(2,DtLKNF.MOD,1);                                         //PRM=0,STAT=1 -wejsciowe dane konfiguracyjne
                    ClrRefKnfDta();
                    StartRTS(_RTLKONFI);										//start timera zapisu
                    Mark(_GZ_MXCW,0);	
                    Mark(_GZ_MXCO,0);   
                    Mark(_GZ_MNCO,0);
                    PrintKrs();
                    DtLKNF.KRK=7;                    
                }
                break;
			}
			key=PresKeyEX(RdPrt(S_OFF),&ky[SS_OFF]);
			if(key>0)						//rezygnacja
			{
				WaitToLowEx(&ky[SS_OFF]);
				LPZK.bfdta=LPZK.dta;
                Mark(_GZ_MXCW,0);	
                Mark(_GZ_MXCO,0);
                Mark(_GZ_MNCO,0);
				DtLKNF.KRK=2;
			}
			break;
		}
		//zmiana wartosci parametru - oczekiwanie na odczyt polecenia i powrot do wyboru parametru
		case 6:
		{
            if(ChkMOD())                                                 //niewlasciwy nr modulu?
            {
                DtLKNF.KRK=8;
                return;                    
            }     
            //----------------------------------------------------------------
            tm=SetKnfTim(DtLKNF.MOD);
			if((RTS(_RTLKONFI)>=tm))                                       //przekroczony limit czasu?
			{
				ExitLKonf();
				return;
			}
            if(PFNf_FLIN&&ChkRefKnfDta()&&ChkStatusKnfDta(1,DtLKNF.XMOD,(DtLKNF.PRM|0b10000000))&&ChkKnfDta(LPZK.dta))                              //mamy komplet danych koniecznych do porawnej pracy procedury?
            {               
                EndKrs();
                DtLKNF.KRK=2;
            }
			break;
		}  
		//zmiana modulu, aktualizacja liczby parametrow - oczekiwanie na odczyt polecenia 
		case 7:
		{
            if(ChkMOD())                                                 //niewlasciwy nr modulu?
            {
                DtLKNF.KRK=8;
                return;                    
            }    
            //----------------------------------------------------------------
            tm=SetKnfTim(DtLKNF.MOD);
			if((RTS(_RTLKONFI)>=tm))                                       //przekroczony limit czasu?
			{
				ExitLKonf();
				return;
			}
            if(PFNf_FLIN&&ChkRefKnfDta()&&ChkStatusKnfDta(2,DtLKNF.XMOD,1))                              //mamy komplet danych koniecznych do porawnej pracy procedury?
            {           
                EndKrs();
                DtLKNF.KNPAR=L2MOD.inDTA[_MOD_IN0_NPAR];                    //liczba parametrow modulu nr DtLKNF.MOD
                if(DtLKNF.KNPAR)                                            //niezerowa liczba parametrow?
                {
                    DtLKNF.PRM=nrp=1;                                           //wskaznik na PP1
                }
                else
                {
                    DtLKNF.PRM=nrp=0;                                           //wskaznik na PP0
                    PrintNoPP();                                            //informacja noPP
                    StartRTS(_RTLKONFI);
                    DtLKNF.KRK=9;
                    return;
                }   
                DtLKNF.KRK=2;
            }
			break;
		}        
        case 8:     //odczekaj pokazujac noPP i wyjdz z konfiguracji
        {
			if((RTS(_RTLKONFI)>=2))                                       //przekroczony limit czasu?
			{             
				ExitLKonf();        
			}
            break;
        }    
        case 9:     //odczekaj pokazujac noPP i wroc do wyboru parametrow
        {
			if((RTS(_RTLKONFI)<2))                                       //przekroczony limit czasu?
			{             
				return;        
			}
            DtLKNF.KRK=2;
            break;
        }
	}
}
/*---------------------------------------------------------------------------------*/
