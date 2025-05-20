/*main.c*/  
/*Oprogramowanie rodziny sterownikow UNI-02 dla kondensatow*/
/*(wprowadzona prezentacja numeru oprogramowania i rodzaju konfiguracji, wprowadzona mozliwosc blokady grzania WU dla kotlow jednofunkcyjnych)*/
/*(wprowadzony staly wskaz temperatury zasobnika*/
/*Wprowadzony parametr P18*/
/*Wprowadzony parametr P19*/
//#include <pic18.h>
//#include <htc.h>
#include <xc.h>
#include <stdio.h>
#include <string.h>
#include "global.h"			//parametry globalne
#include "main.h"			//parametry lokalne
#if (_EXT_BOARD>=2)
    #include "amgs_rs9b.tp.h"
    #include "LIN1SInterpreter.h"
#else
    #include "mgs_rs9b.tp.h"
#endif
#include "port.h"
#include "print.h"			//parametry globalne
#include "keypad.tp.h"
#include "ot.tp.h"			//parametry lokalne

extern volatile StPin ky[];
tpMAIN M;
volatile tpRSDTA1 RSDT1;
volatile DataPFN PFN; //****

extern volatile MGSRSData MGSD;
extern volatile tpOTMaster OTMaster;
extern volatile unsigned char x;
unsigned char zps,zmn;
extern volatile unsigned char xxy,zzz,vvv;
volatile unsigned char aa=0,bb=0,cc=0,dd=0,ee=0,ff=0;
extern unsigned char dCVBC[];

unsigned int LiczCRC(unsigned char *tb, unsigned char n);
unsigned char TWO_ASCIItoHEX(unsigned char a1, unsigned char a0);
unsigned char CheckCHK(unsigned char a3, unsigned char a2, unsigned char a1, unsigned char a0, unsigned int h);
unsigned int FOUR_ASCIItoHEX(unsigned char b3, unsigned char b2, unsigned char b1, unsigned char b0);
void HEXtoTWO_ASCII(unsigned char h,unsigned char *a1, unsigned char *a0);
void HEXtoFOUR_ASCII(unsigned int h,unsigned char *a3, unsigned char *a2,unsigned char *a1, unsigned char *a0);
void StartRWtoCVBS(unsigned char MSS,unsigned int DID,unsigned int DTA);


void DecWsw(void);
void main(void)
{

//------------------
//------------------
	InitOsc();
	InitGlobal();
#if _EXT_BOARD!=0    
    InitAC();
    InitDataPmr();              //inicjalizacja danych dla pomiaru interfejsu 10V
#else
	ADCON1=0x0f;				//(!) wszystkie wejscia AC jako cyfrowe    
#endif
	InitTimer1();
	InitPWM2();
	InitLLED();
	InitPort();
	InitTransProc();
    InitTimer3();
#if _EXT_BOARD!=0
	InitAUart2();
#if (_EXT_BOARD>=2)                    //aktywowany modul LIN1SLAVE?    
    InitUart1();
#endif
#else
    InitUart();
#endif
#if (_EXT_BOARD>=3)    
    InitOT();
#endif    
	InitI2C();
	InitWsw();
	LED=1;
//------------------
//------------------
	if (!POR||!BOR)							//zalaczono zasilanie?
	{
        PFNf_pwrON=1;
		TestScr();
		PwrOnDelay();						//zwloka na wypadek stanow nieustalonych	
	}
	else
	{
        PFNf_pwrON=0;
		PwrOnDelay();						//zwloka na wypadek stanow nieustalonych	
	}
	EndTestScr();
	
	//stan po zalaczeniu zasilania
	if (!POR||!BOR)		//zalaczono zasilanie? 
	{	
		POR=1;
		BOR=1;
	}
	
//------------------
//------------------
//------------------
	InitKonf();
/*#if _EXT_BOARD==0    
	ADCON1=0x0f;				//wszystkie wejscia jako cyfrowe
	TRISA=0x0f;
#endif
 */
#if _TDEMO==1
//Tryb DEMO ***************************************
	#if _TDEMOFL==1
			TestScr();
			while(1)			//****************************			
			{					//****************************
				CLRWDT();		//****************************
			}
	}	
	#else
			PFNf_fmsg=0;		//****************************
			PFN.KAWR=8;			//****************************
			PFNf_ferr=0;		//****************************
			PFN.KERR=2;			//****************************
			PFNf_off=0;			//****************************
			PFNf_lato=0;		//****************************
			PFNf_zima=1;		//****************************
			PFNf_errCS=0;		//****************************
			PFNf_errCO=0;		//****************************
			PFNf_errCW=0;		//****************************
			PFNf_tdzCO=0;		//****************************
			PFNf_tdzCW=0;		//****************************
			PFNf_prn_res=0;		//****************************
			PFN.CO=25;			//****************************
			PFN.CW=35;			//****************************
			PFN.CS=15;			//****************************
			PFN.minPCW=35;		//****************************
			PFN.minPCO=45;		//****************************
			PFN.maxPCW=65;		//****************************
			PFN.maxPCO=85;		//****************************
			PFN.PCW=35;			//****************************
			PFN.PCO=45;			//****************************
			PFN.ECO=5;			//****************************
			PFNf_plmCW=0;
			PFNf_plmCO=0;
			PFNf_pgd=0;
			PFNf_aktywne_KNF=0;	//****************************
			PFNf_KnfDisErr=0;
			PFNf_aleg=0;
			PFNf_fnserw=0;
			PFNf_opgres=0;
			PFNf_opgprg=1;
			while(1)			//****************************			
			{					//****************************
				CLRWDT();		//****************************
				PFNf_ofzima=PFNf_zima;	//****************************
				PFNf_oflato=PFNf_lato;	//****************************
				PFNf_ofoff=PFNf_off;	//****************************
				PFNf_ofnserw=PFNf_fnserw;
				PFNf_oaleg=PFNf_aleg;
				MKonfiguracja();//****************************
				TrybPracy();	//****************************
				DecWsw();		//****************************
			}					//****************************
	}							//****************************
	#endif
//**************************************************
#else
	PFN.KRS=0;
	PFNf_toInit=1;							//odczekaj na polaczenie z UNI02
    StartRTS(_RTEST);//******************

	while(1)								//petla glowna programu				
	{
        //aa=PORTAbits.RA0;
		CLRWDT();     
        //if(PFNf_initVEx) InitVerExposition();
#if _EXT_BOARD==1
        Pomiar10V();        //interfejs 10V
#endif
#if _EXT_BOARD==2
        if(!PFNf_LIN_KSK&&!PFNf_LIN_STR)
        {    
            Pomiar10V();        //interfejs 10V
        }
        else
        {
            PFNf_bEnF10V=0;             //znacznik do aktywacji wejscia 10V              
            PFNf_EnF10V=0;              //znacznik aktywnosci wejscia 10V  
            PFNf_b10V_RT=0;             //znacznik do aktywacji pobudzenia z wejscia 10V                
            PFNf_10V_RT=0;              //znacznik pobudzenia z wejscia 10V   
            PFNf_10V_nPCO=0;            //znacznik nowej wartosci PCO z wejscia 10V            
        }
#endif        
#if _EXT_BOARD>=3
        if(!PFNf_LIN_KSK&&!PFNf_LIN_STR&&!PFNf_FOT)
        {    
            Pomiar10V();        //interfejs 10V
        }
        else
        {
            PFNf_bEnF10V=0;             //znacznik do aktywacji wejscia 10V              
            PFNf_EnF10V=0;              //znacznik aktywnosci wejscia 10V  
            PFNf_b10V_RT=0;             //znacznik do aktywacji pobudzenia z wejscia 10V                
            PFNf_10V_RT=0;              //znacznik pobudzenia z wejscia 10V   
            PFNf_10V_nPCO=0;            //znacznik nowej wartosci PCO z wejscia 10V            
        }
#endif         
        //TRANSMISJA LIN
#if (_EXT_BOARD>=2)
        LIN1SDeamon();                                      //LIN1 SLAVE
        if(RTdS(_RTFLIN)>_TFLIN) 
        {
            BitSet3(&RSDT1.inLNSTAT,_FLIN,0);        //odswiez znacznik komunikacji LIN dla ukladu UNI-02
            PFNf_FLIN=0;
            PFNf_LIN_RT=0;
            PFNf_LIN_CW=0;            
        }
#else
        BitSet3(&RSDT1.inLNSTAT,_FLIN,0);
        PFNf_FLIN=0;
#endif
        //TRANSMISJA OT
#if (_EXT_BOARD>=3)        
        OTInterpreter();
        if(RTdS(_RTFOT)>_TFOT) 
        {
            BitSet3(&RSDT1.inINSTAT,_FOT,0);        //odswiez znacznik komunikacji OT dla ukladu UNI-02
            OTMaster.Ref.war=0;                     //brak aktywnosci - wyzeruj flagi aktualizacji danych            
            PFNf_FOT=0;
            PFNf_OT_DPGD=0;
        }        
#else        
        BitSet3(&RSDT1.inINSTAT,_FOT,0);
        PFNf_FOT=0;
        PFNf_OT_DPGD=0;        
#endif        
		if(MGSDf_NO_ODB||PFNf_toInit) InitUNIData();		//odczekaj na polaczenie z UNI02
		MKonfiguracja();					//decyzja o wejsciu do proc. konfiguracji
		TrybPracy();
		RingProc();							//transmisja do/z plytki UNI-02
		RefData();							//aktualizacja danych WE/WY
		DecWsw();							//decyzja o wyswietlaniu parametrow
		DecRes();							//decyzja o wejsciu oczekiwania na transmisje po resecie
	}
}
#endif

//--------------------------------------------------------------
void InitUNIData(void)
{
	static unsigned char n=0,k=3,j=3;

//-----------OCZEKIWANIE
	EndErr();
	EndPR();
	EndNst();
	ClrAllMarks();
	ClrDig(1,1,0);
	Mark(_GZ_OFF,2);
	BitSet3(&RSDT1.inSTAT0,_INITDTA,1);	//aktywacja trybu INICJALIZACJI
	PFNf_kalVNT=0;
	PFNf_opgres=0;
	PFNf_opgprg=0;
	PFNf_ferr=0;
    RSDT1f_RD_STD=0;    
    RSDT1f_RRD_STD=0;    
    RSDT1f_RD_KNF=0;    
    RSDT1f_RRD_KNF=0;    
    RSDT1.vSTD=0;
    RSDT1.vKNF=0;    
    PFN.ALIN=0; //ustaw wstepnie adres niedozwolony LIN - blokada odbioru do czasu nawiazania komunikacji z UNI-02
    PFN.STRB=0; //ustaw wstepnie specjalny tryb pracu na 0
    RSDT1.inSTAT2=0;    
	n=0;
#if (_EXT_BOARD>=2)    
    j=3;        //aktywny, cykliczny pobor paczki konfiguracyjnej
#else
    j=0;        //nieaktywny, pobor paczki konfiguracyjnej
#endif    
	PrintVerBoards(1);	//reset procedury wizualizacji wersji
    //Oczekiwanie na ustalony stan plyty sterujacej, okreslona wersje oprogramowania plyty sterujacej i zaktualizowane dane STD
    //gdzie n - wymusza okeslona liczbe wykonan i pozwala na podtrzymanie petli na czas aktywnosci procedury kalibracji
    //gdzie k,j - wymusza odbior ustalonej, minimalnej liczby ramek STD i KNF    
	//while((n++<10||(!PFNf_opgres&&!PFNf_opgprg)||(PFNf_opgres&&!PFNf_ferr)))  
	while((!PFNf_opgres&&!PFNf_opgprg)||(PFNf_opgres&&!PFNf_ferr)||(n++<10)||k||j)        
	{
		SetLightLLed(1);
		while(!MGSDf_TransACK)
		{
			CLRWDT();
            //TRANSMISJA UNI-02
			RingProc();                                               //transmisja do/z plytki UNI-02
            //TRANSMISJA LIN
#if (_EXT_BOARD>=2)
            LIN1SDeamon();                                                   //LIN1 SLAVE
            if(RTdS(_RTFLIN)>_TFLIN)
            {
                BitSet3(&RSDT1.inLNSTAT,_FLIN,0);        //odswiez znacznik komunikacji LIN dla ukladu UNI-02
                PFNf_FLIN=0;
                PFNf_LIN_RT=0;
                PFNf_LIN_CW=0;                            
            }
#else
            BitSet3(&RSDT1.inLNSTAT,_FLIN,0);
            PFNf_FLIN=0;
#endif
            //TRANSMISJA OT
#if (_EXT_BOARD>=3)        
            OTInterpreter();
            if(RTdS(_RTFOT)>_TFOT) 
            {
                BitSet3(&RSDT1.inINSTAT,_FOT,0);        //odswiez znacznik komunikacji OT dla ukladu UNI-02
                OTMaster.Ref.war=0;                     //brak aktywnosci - wyzeruj flagi aktualizacji danych
                PFNf_FOT=0;   
                PFNf_OT_DPGD=0;
            }        
#else        
            BitSet3(&RSDT1.inINSTAT,_FOT,0);
            PFNf_FOT=0;   
            PFNf_OT_DPGD=0;
#endif
		}
		MGSDf_TransACK=0;
        //----------------------------------------------------------
        //WYMUSZENIE ODBIORU MINIMUM TRZECH RAMEK STD, KNF
        //odrzuc pierwsze trzy ramki STD
        if(k&&RSDT1f_RD_STD)   
        {
            k--;    
            if(k)
            {
                RSDT1f_RD_STD=0;    //uniewaznij ramke
                RSDT1f_RRD_STD=0;
            }
        }
        //odrzuc pierwsze trzy ramki KNF
        if(j&&RSDT1f_RD_KNF)   
        {
            j--;
            if(j)  
            {
                RSDT1f_RD_KNF=0;    //uniewaznij ramke
                RSDT1f_RRD_KNF=0;
            }
        }         
        //----------------------------------------------------------
        //aktualizacja danych 
        if(!k&&RSDT1f_RD_STD)
        {
            RSDT1f_RRD_STD=1;		//aktywna blokada na czas interpretacji danych        
            PFNf_opgres=GetBit2(RSDT1.outSTAT0,_PGRES);
            PFNf_opgprg=GetBit2(RSDT1.outSTAT0,_PGPRG);
            PFNf_ferr=GetBit2(RSDT1.outSTAT1,_ERR);
            PFNf_kalVNT=0;

            PFN.RDZ0=RSDT1.outRDZ0;							//nr wesji oprogramowania plytki UNI-02
            PFN.RDZ1=RSDT1.outRDZ1;
            PFN.RDZ2=RSDT1.outRDZ2;

            if(RSDT1.vSTD>=4)
            {         
                PFNf_EnAleg=((PFN.RDZ1&0x0F)==_UNICO);					//znacznik kotla zasobnikowego (1F)
                PFN.vUNI02=((PFN.RDZ1&0xF0)>>4)+(PFN.RDZ2&0b00011111);  //wersja opr.UNI-02
    #if (_EXT_BOARD>=2)                                                 //aktywowany modul LIN1SLAVE?            
                PFN.ALIN=(PFN.RDZ0&0b11100000)>>5;
    #else
                PFN.ALIN=5;
    #endif
            }
            else
            {
                PFN.vUNI02=0;                                           //nieznany nr wersji oprogramowania plytki sterujacej
                PFNf_EnAleg=((PFN.RDZ1&0x0F)==_UNICO);					//znacznik kotla zasobnikowego (1F)
                PFN.ALIN=5;
            }
            RSDT1f_RRD_STD=0;		//aktywna blokada na czas interpretacji danych
        }        
        /*
		PFNf_opgres=GetBit2(RSDT1.outSTAT0,_PGRES);
		PFNf_opgprg=GetBit2(RSDT1.outSTAT0,_PGPRG);
		PFNf_ferr=GetBit2(RSDT1.outSTAT1,_ERR);
		PFNf_kalVNT=0;//GetBit2(RSDT1.outSTAT2,_KALIB);
        
        PFN.RDZ0=RSDT1.outRDZ0;                                 //nr wesji oprogramowania plytki UNI-02
        PFN.RDZ1=RSDT1.outRDZ1;
        PFN.RDZ2=RSDT1.outRDZ2;        
        if(RSDT1.vSTD>=4)
        {         
            PFNf_EnAleg=((PFN.RDZ1&0x0F)==_UNICO);					//znacznik kotla zasobnikowego (1F)
            PFN.vUNI02=((PFN.RDZ1&0xF0)>>4)+(PFN.RDZ2&0b00011111);  //wersja opr.UNI-02
#if (_EXT_BOARD>=2)                                                 //aktywowany modul LIN1SLAVE?            
            PFN.ALIN=(PFN.RDZ0&0b11100000)>>5;
#else
            PFN.ALIN=5;
#endif
        }
        else
        {
            PFN.vUNI02=0;
            PFNf_EnAleg=((PFN.RDZ1&0x0F)==_UNICO);					//znacznik kotla zasobnikowego (1F)
            PFN.ALIN=5;
        }
         */
		if(!k&&!j&&!PFNf_opgres)
		{
#if _KEY_BBOARD==0  
            if(!k&&!j)
            {            
                BitSet3(&RSDT1.inSTAT1,_KSET,RdPrt(S_SET));				//detekcja przyciskow wejscia do konfiguracji
                BitSet3(&RSDT1.inSTAT1,_KOFF,RdPrt(S_OFF));
                BitSet3(&RSDT1.inSTAT1,_KMINUS,RdPrt(S_KDW));           //detekcja przycisku na potrzeby wyjscia z proc.kalibracji
                BitSet3(&RSDT1.inSTAT1,_KPLUS,RdPrt(S_KUP));     
            }
            else
            {
                BitSet3(&RSDT1.inSTAT1,_KSET,0);				
                BitSet3(&RSDT1.inSTAT1,_KOFF,0);
                BitSet3(&RSDT1.inSTAT1,_KMINUS,0);           
                BitSet3(&RSDT1.inSTAT1,_KPLUS,0);                
            }
            BitSet3(&RSDT1.inSTAT1,_7KEY,0);                        //znacznik interfejsu 7Key 
#endif
#if _KEY_BBOARD==1  
            if(!k&&!j)
            {            
                BitSet3(&RSDT1.inSTAT1,_KSET,RdPrt(S_KOM));				//detekcja przyciskow wejscia do konfiguracji
                BitSet3(&RSDT1.inSTAT1,_KOFF,RdPrt(S_OFF));            
                BitSet3(&RSDT1.inSTAT1,_KMINUS,RdPrt(S_KDW_CO));        //detekcja przycisku na potrzeby wyjscia z proc.kalibracji
                BitSet3(&RSDT1.inSTAT1,_KPLUS,RdPrt(S_KUP_CO));  
            }
            else
            {
                BitSet3(&RSDT1.inSTAT1,_KSET,0);				
                BitSet3(&RSDT1.inSTAT1,_KOFF,0);            
                BitSet3(&RSDT1.inSTAT1,_KMINUS,0);        
                BitSet3(&RSDT1.inSTAT1,_KPLUS,0);                
            }         
            BitSet3(&RSDT1.inSTAT1,_7KEY,1);                        //znacznik interfejsu 7Key             
#endif            
		}
		else
		{
			Mark(_GZ_MXCW,0);
		}
	}
	MGSDf_TransACK=0; 
    
//--------------------------------------------------------------    
//-----------INICJALIZACJA NASTAW WYSWIETLACZA DANYMI Z PLYTKI STERUJACEJ
	PFNf_fnserw=GetBit2(RSDT1.outSTAT0,_FKSRV);
	PFNf_aleg=GetBit2(RSDT1.outSTAT0,_ANTYL);
	PFNf_off=GetBit2(RSDT1.outSTAT2,_F_OFF);
	PFNf_lato=GetBit2(RSDT1.outSTAT2,_F_LATO);
	PFNf_zima=GetBit2(RSDT1.outSTAT2,_F_ZIMA);
	PFNf_kalVNT=0;//GetBit2(RSDT1.outSTAT2,_KALIB);
    PFNf_pgd=GetBit2(RSDT1.outSTAT1,_PGD);
    
	PFN.RDZ0=RSDT1.outRDZ0;
	PFN.RDZ1=RSDT1.outRDZ1;
	PFN.RDZ2=RSDT1.outRDZ2;    
	PFN.PCW=(unsigned int)RSDT1.outPCW;
	PFN.PCO=(unsigned int)RSDT1.outPCO;
	PFN.ECO=(unsigned int)RSDT1.outECO;

    if(RSDT1.vSTD>=4)
    {         
        PFNf_EnAleg=((PFN.RDZ1&0x0F)==_UNICO);					//znacznik kotla zasobnikowego (1F)
        PFN.vUNI02=((PFN.RDZ1&0xF0)>>4)+(PFN.RDZ2&0b00011111);  //wersja opr.UNI-02
#if (_EXT_BOARD>=2)                                                 //aktywowany modul LIN1SLAVE?            
        PFN.ALIN=(PFN.RDZ0&0b11100000)>>5;
#else
        PFN.ALIN=5;
#endif
    }
    else
    {
        PFN.vUNI02=0;
        PFNf_EnAleg=((PFN.RDZ1&0x0F)==_UNICO);					//znacznik kotla zasobnikowego (1F)
        PFN.ALIN=5;
    }    
	PFNf_newPCO=0;
	PFNf_newPCW=0;
	PFNf_newECO=0;    
  
    //Ekspozycja wersji i konfiguracji p³ytek
    while(PFNf_pwrON&&!PFNf_opgres)    
    {
        CLRWDT();
        //TRANSMISJA UNI-02
        RingProc();                                               //transmisja do/z plytki UNI-02
        //TRANSMISJA LIN
        #if (_EXT_BOARD>=2)
                LIN1SDeamon();                                                   //LIN1 SLAVE
                if(RTdS(_RTFLIN)>_TFLIN)
                {
                    BitSet3(&RSDT1.inLNSTAT,_FLIN,0);        //odswiez znacznik komunikacji LIN dla ukladu UNI-02
                    PFNf_FLIN=0;
                    PFNf_LIN_RT=0;
                    PFNf_LIN_CW=0;                            
                }
        #else
                BitSet3(&RSDT1.inLNSTAT,_FLIN,0);
                PFNf_FLIN=0;
        #endif
        //TRANSMISJA OT
        #if (_EXT_BOARD>=3)        
                OTInterpreter();
                if(RTdS(_RTFOT)>_TFOT) 
                {
                    BitSet3(&RSDT1.inINSTAT,_FOT,0);        //odswiez znacznik komunikacji OT dla ukladu UNI-02
                    OTMaster.Ref.war=0;                     //brak aktywnosci - wyzeruj flagi aktualizacji danych
                    PFNf_FOT=0;   
                    PFNf_OT_DPGD=0;
                }        
        #else        
                BitSet3(&RSDT1.inINSTAT,_FOT,0);
                PFNf_FOT=0;   
                PFNf_OT_DPGD=0;
        #endif        
        RefData();
#if _KEY_BBOARD==0          
        if(PrintVerBoards(0)||(RdPrt(S_OFF)&&RdPrt(S_SET))) break;         
#endif
#if _KEY_BBOARD==1 
        if(PrintVerBoards(0)||(RdPrt(S_OFF)&&RdPrt(S_KOM))) break;        
#endif
    }    
    PFNf_pwrON=0;
	PFNf_prn_res=0;
	PFNf_res=0;	
	RefData();	
	BitSet3(&RSDT1.inSTAT0,_INITDTA,0);	//deaktywacja trybu INICJALIZACJI
	Mark(_GZ_OFF,0);
	PFNf_toInit=0;
    PFNf_LINres=0;
#if _KEY_BBOARD==0    
	ResBufKeyEx(&ky[SS_SET]);
	ResBufKeyEx(&ky[SS_OFF]);
	ResBufKeyEx(&ky[SS_KUP]);
	ResBufKeyEx(&ky[SS_KDW]);
#endif
#if _KEY_BBOARD==1    
	ResBufKeyEx(&ky[SS_SET]);
	ResBufKeyEx(&ky[SS_KOM]);    
	ResBufKeyEx(&ky[SS_OFF]);
	ResBufKeyEx(&ky[SS_KUP_CW]);
	ResBufKeyEx(&ky[SS_KDW_CW]);
	ResBufKeyEx(&ky[SS_KUP_CO]);
	ResBufKeyEx(&ky[SS_KDW_CO]);    
#endif    
}
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
unsigned char FormatSETP(volatile unsigned char dt)
{
    if(dt<PFN.SETPmin) dt=PFN.SETPmin;
    if(dt>PFN.SETPmax) dt=PFN.SETPmax;        
    return dt;    
}
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//Zwiekszenie wartosci SETP o 1 z krokiem czasowym _KRKZSETP [sek]
unsigned char SETPToHigh(volatile unsigned char SETP,volatile unsigned char newSETP)
{
	if(RTS(_RTZSETP)>_KRKZSETP)
	{
		StartRTS(_RTZSETP);
		if((SETP+1)<newSETP)
		{
	 		SETP+=1;
		}
		else
		{
			SETP=newSETP;
		}
	}
    return SETP;
}
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//Zmniejszenie wartosci SETP o 1 z krokiem czasowym _KRKZSETP [sek]
unsigned char SETPToLow(volatile unsigned char SETP,volatile unsigned char newSETP)
{
	if(RTS(_RTZSETP)>_KRKZSETP)
	{
		StartRTS(_RTZSETP);
		if((SETP-1)>newSETP)
		{
	 		SETP-=1;
		}
		else
		{
			SETP=newSETP;
		}
	}
    return SETP;
}
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//Plynna zmiana wartosci SETP o +/- 1 z krokiem czasowym _KRKZSETP [sek]
unsigned char SetNewSETP(volatile unsigned char SETP,volatile unsigned char newSETP)
{
    volatile unsigned char dt;
    
    SETP=FormatSETP(SETP);
    newSETP=FormatSETP(newSETP);
    if(SETP==newSETP)
    {
        return SETP;
    }
	if(SETP<newSETP)
	{
		dt=SETPToHigh(SETP,newSETP);
	}
	else
	{
		dt=SETPToLow(SETP,newSETP);
	}
    return FormatSETP(dt);
}
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
void RefData(void)
{
    volatile unsigned char nst,pob;
//read
//STANDARD
	if(RSDT1f_RD_STD)
	{
		RSDT1f_RRD_STD=1;		//aktywna blokada na czas interpretacji danych
		RSDT1f_RD_STD=0;
        
		PFN.RDZ0=RSDT1.outRDZ0;
		PFN.RDZ1=RSDT1.outRDZ1;
		PFN.RDZ2=RSDT1.outRDZ2;    
        //vvv=PFN.RDZ2;
		PFN.CO=RSDT1.outCO;
		PFN.CW=RSDT1.outCW;
		PFN.CS=RSDT1.outCS;	
		PFN.IN=RSDT1.outIN;
		PFN.VNT=RSDT1.outVNT;
		PFN.minPCO=RSDT1.outPCOmn;
		PFN.maxPCO=RSDT1.outPCOmx;
		PFN.minPCW=RSDT1.outPCWmn;
		PFN.maxPCW=RSDT1.outPCWmx;
		PFN.PPCO=RSDT1.outPPCO;
		PFN.PPCW=RSDT1.outPPCW;
		PFN.KAWR=RSDT1.outAWR;
		PFN.KERR=RSDT1.outERR;
		PFN.PP=RSDT1.outPP;
		PFN.PG=RSDT1.outPG;
		PFN.rCO=RSDT1.outrCO;
		PFN.rCW=RSDT1.outrCW;
        PFN.ZW=RSDT1.outZW;
        PFN.SETPmin=RSDT1.outSETPmin;
        PFN.SETPmax=RSDT1.outSETPmax;       
        
		PFNf_EnAleg=((PFN.RDZ1&0x0F)==_UNICO); 
		//PFNf_EnAleg=1;	
		PFNf_opgres=GetBit2(RSDT1.outSTAT0,_PGRES);
		PFNf_opgprg=GetBit2(RSDT1.outSTAT0,_PGPRG);
        if(!PFNf_opgres&&!PFNf_opgprg) PFNf_toInit=1;
		PFNf_plmCO=GetBit2(RSDT1.outSTAT0,_PLMCO);
		PFNf_plmCW=GetBit2(RSDT1.outSTAT0,_PLMCW);
		PFNf_ofnserw=GetBit2(RSDT1.outSTAT0,_FKSRV);
		PFNf_oaleg=GetBit2(RSDT1.outSTAT0,_ANTYL);
		PFNf_opKNF=GetBit2(RSDT1.outSTAT0,_KNFPRC);
		PFNf_enKNF=GetBit2(RSDT1.outSTAT0,_ENKNF);

		PFNf_tdzCO=PFNf_plmCO&&GetBit2(RSDT1.outSTAT1,_TdzCO);
		PFNf_tdzCW=PFNf_plmCW&&GetBit2(RSDT1.outSTAT1,_TdzCW);
		PFNf_fmsg=GetBit2(RSDT1.outSTAT1,_MSG);
		PFNf_ferr=GetBit2(RSDT1.outSTAT1,_ERR);
		PFNf_fL3=GetBit2(RSDT1.outSTAT1,_L3);
		//PFNf_pgd=GetBit2(RSDT1.outSTAT1,_PGD);
        //------        
        nst=GetBit2(RSDT1.outSTAT1,_PGD);
        if(PFN.vUNI02>=14)      //od wersji 14 wprowadzono niezalezna nastawe dla regulatora pogodowego 
        {
            if(PFNf_pgd!=nst)   //zalaczono/wylaczono regulator pogodowy?
            {
                PFN.PCO=(unsigned int)RSDT1.outPCO;     //aktualizuj nastawe dana z plytki sterujacej
            }
        }
		PFNf_pgd=nst;
        //------        
		PFNf_obgCO=GetBit2(RSDT1.outSTAT1,_OBGCO);
		PFNf_obgCW=GetBit2(RSDT1.outSTAT1,_OBGCW);

		PFNf_errCS=GetBit2(RSDT1.outSTAT2,_ErrCS);
		PFNf_errCO=GetBit2(RSDT1.outSTAT2,_ErrCO);
		PFNf_errCW=GetBit2(RSDT1.outSTAT2,_ErrCW);
		PFNf_fnoCS=GetBit2(RSDT1.outSTAT2,_CSnmt);
		PFNf_ofzima=GetBit2(RSDT1.outSTAT2,_F_ZIMA);
		PFNf_oflato=GetBit2(RSDT1.outSTAT2,_F_LATO);
		PFNf_ofoff=GetBit2(RSDT1.outSTAT2,_F_OFF);   
		PFNf_kalVNT=0;//GetBit2(RSDT1.outSTAT2,_KALIB);
		PFNf_zcfm=GetBit2(RSDT1.outSTAT3,_ZCFM);
		PFNf_vnt=GetBit2(RSDT1.outSTAT3,_VNT_ON);
		PFNf_rozruch=GetBit2(RSDT1.outSTAT3,_ROZ);
		PFNf_errIN=GetBit2(RSDT1.outSTAT3,_ErrorIN);
		PFNf_pmpPWM=GetBit2(RSDT1.outSTAT3,_PMP_PWM);
		PFNf_trbECO=0;//GetBit2(RSDT1.outSTAT3,_TRB_ECO);
		PFNf_timZAS=GetBit2(RSDT1.outSTAT3,_TIM_ZAS);   //znacznik pobudzenia CW/stan na wejsciu timera zasobnika
        PFNf_pobRT=GetBit2(RSDT1.outSTAT3,_POB_RT);     //znacznik pobudzenia RT
        
        PFNf_prODP=0;//GetBit2(RSDT1.outSTAT4,_PR_ODP);     //znacznik aktywnosci procedury odpowietrzajacej
        PFNf_popFN=GetBit2(RSDT1.outSTAT4,_POP_FN);     //poprzedni stan przelacznika funkcji (1-lato, 0-pozostale)
        PFNf_tmpZW=GetBit2(RSDT1.outSTAT4,_TMP_ZW);     //aktywnosc czujnika temperatury zewnetrznej
        PFNf_pgdTP=0;//GetBit2(RSDT1.outSTAT4,_PGD_TP);     //rodzaj wewnetrznego regulatora pogodowego        
        
		//if(PFN.vUNI02<2) PFNf_timZAS=1;
        if(PFN.vUNI02) PFN.ALIN=(PFN.RDZ0&0b11100000)>>5; //adres LIN
        else PFN.ALIN=5;
        if(!PFNf_ofnserw) PFNf_LINresSR=0;
		if(PFNf_ofnserw) PFNf_b_ofnserw=1;
		if(PFNf_ofnserw&&!PFNf_fnserw&&!PFNf_LINresSR) PFNf_fnserw=1;
		if(PFNf_b_ofnserw&&!PFNf_ofnserw) PFNf_fnserw=0;
        if(!PFNf_oaleg) PFNf_LINresAL=0;
		if(PFNf_oaleg) PFNf_b_oaleg=1;
		if(PFNf_oaleg&&!PFNf_aleg&&!PFNf_LINresAL) PFNf_aleg=1;
		if(PFNf_b_oaleg&&!PFNf_oaleg) PFNf_aleg=0;
		RSDT1f_RRD_STD=0;
	}
//write
//STANDARD
	RSDT1.inPCO=(unsigned char)PFN.PCO;
	RSDT1.inPCW=(unsigned char)PFN.PCW;
	RSDT1.inECO=(unsigned char)PFN.ECO;

	BitSet3(&RSDT1.inSTAT0,_INITKNF,PFNf_aktywne_KNF);

	BitSet3(&RSDT1.inSTAT0,_INITSRV,PFNf_fnserw&&!PFNf_ofnserw);
	BitSet3(&RSDT1.inSTAT0,_INITANL,PFNf_aleg&&!PFNf_oaleg);
	BitSet3(&RSDT1.inSTAT0,_PFN_ZIMA,PFNf_zima&&!PFNf_off);
	BitSet3(&RSDT1.inSTAT0,_PFN_LATO,PFNf_lato&&!PFNf_off);
	BitSet3(&RSDT1.inSTAT0,_PFN_OFF,PFNf_off);
	BitSet3(&RSDT1.inSTAT0,_PFN_RES,(PFNf_res||PFNf_LINresSR||PFNf_LINresAL));

	BitSet3(&RSDT1.inSTAT1,_SRVMAX,PFNf_fns_max);
    
#if _KEY_BBOARD==0    
	BitSet3(&RSDT1.inSTAT1,_KPLUS,RdPrt(S_KUP));
	BitSet3(&RSDT1.inSTAT1,_KMINUS,RdPrt(S_KDW));
	BitSet3(&RSDT1.inSTAT1,_KSET,RdPrt(S_SET));
	BitSet3(&RSDT1.inSTAT1,_KOFF,RdPrt(S_OFF));
#endif
#if _KEY_BBOARD==1    
	BitSet3(&RSDT1.inSTAT1,_KPLUS,RdPrt(S_KUP_CO));
	BitSet3(&RSDT1.inSTAT1,_KMINUS,RdPrt(S_KDW_CO));
	BitSet3(&RSDT1.inSTAT1,_KSET,RdPrt(S_KOM));
	BitSet3(&RSDT1.inSTAT1,_KOFF,RdPrt(S_OFF));
#endif  
    
#if _EXT_BOARD!=0
    BitSet3(&RSDT1.inINSTAT,_F10V,PFNf_EnF10V);      //bit aktywnosci interfejsu 10V  
    BitSet3(&RSDT1.inINSTAT,_10V_RT,(PFNf_10V_RT&&PFNf_EnF10V));    //zdalne pobudzenie RT z interfejsu 10V
    if(!PFNf_EnF10V&&!PFNf_LIN_KSK&&!PFNf_LIN_STR&&!(OTMaster.Ref._Control_Setpoint&&PFNf_FOT))
    {
        PFN.CONTSETP=PFN.SETPmax;
        RSDT1.inCONTSETP=PFN.CONTSETP;
        PFN.INSETP=PFN.CONTSETP;
    }
    else //dowolny z interfejsow aktywny
    {
        if((PFNf_LIN_KSK||PFNf_LIN_STR)&&PFNf_FLIN)             //interfejs LIN w trybie STR lub KSK?
        {
            PFN.CONTSETP=FormatSETP(PFN.INSETP);                //natychmiastowa aktualizacja SETP 
        }
        else                                                    //pozostale przypadki
        {
            PFN.CONTSETP=SetNewSETP(PFN.CONTSETP,PFN.INSETP);   //stopniowa aktualizacja SETP z krokiem czasowym
        }
        RSDT1.inCONTSETP=PFN.CONTSETP;  
    }    
    RSDT1.inSTAT2=PFN.STRB;    
#else
    BitSet3(&RSDT1.inINSTAT,_F10V,0);                //bit aktywnosci interfejsu 10V  
    BitSet3(&RSDT1.inINSTAT,_10V_RT,0);              //zdalne pobudzenie RT
    RSDT1.inCONTSETP=(unsigned char)PFN.PCO;    
    RSDT1.inSTAT2=0;    
#endif
        
#if (_EXT_BOARD>=2)                     //aktywowany modul LIN1SLAVE?    
    if(PFNf_LIN_CK)         //nadszedl sygnal cykliczny?
    {
        PFNf_LIN_CK=0;
        StartRTS(_RTLCK);
    }
    if(RTS(_RTLCK)>=15)     //brak sygnalu cyklicznego? 
    {
        PFNf_LIN_EPBCO=0;
        PFNf_LIN_PBCO=0;   
        PFNf_LIN_EPBCW=0;
        PFNf_LIN_PBCW=0;
        PFNf_LIN_KSK=0;  
        PFNf_LIN_STR=0;
        PFNf_LIN_DPGD=0; 
    }
    //BitSet3(&RSDT1.inSTAT1,_END_ODP,(PFNf_prODP&&PFNf_LIN_ENDO&&PFNf_FLIN));     //deaktywacja procedury odpowietrzajacej
    if(!PFNf_prODP) PFNf_LIN_ENDO=0;
    if(!PFNf_LIN_EPBCO)     //nieaktywne pobudzenie cykliczne?
    {
        BitSet3(&RSDT1.inLNSTAT,_LN_RT,(PFNf_LIN_RT&&PFNf_FLIN));                //zdalne pobudzenie RT
    }
    else
    {
        PFNf_LIN_RT=0;
        BitSet3(&RSDT1.inLNSTAT,_LN_RT,(PFNf_LIN_PBCO&&PFNf_FLIN));                //zdalne pobudzenie RT        
    }
    if(!PFNf_LIN_EPBCW)     //nieaktywne pobudzenie cykliczne?
    {   
        BitSet3(&RSDT1.inLNSTAT,_LN_CW,(PFNf_EnAleg&&PFNf_LIN_CW&&PFNf_FLIN));   //zdalne pobudzenie timera zasobnika  (dla kotlow jednofunkcyjnych))
    }
    else
    {
        PFNf_LIN_CW=0;
        BitSet3(&RSDT1.inLNSTAT,_LN_CW,(PFNf_EnAleg&&PFNf_LIN_PBCW&&PFNf_FLIN)); //zdalne pobudzenie timera zasobnika  (dla kotlow jednofunkcyjnych))        
    }
    BitSet3(&RSDT1.inLNSTAT,_LN_KSK,(PFNf_LIN_KSK&&PFNf_FLIN));                  //znacznik pracy w trybie kaskady
    BitSet3(&RSDT1.inLNSTAT,_LN_STR,(PFNf_LIN_STR&&PFNf_FLIN));                  //znacznik pracy w trybie sterowania z managera strefy lub z regulatora RT 
    BitSet3(&RSDT1.inLNSTAT,_LN_DPGD,(PFNf_LIN_DPGD&&PFNf_FLIN));                //znacznik dezaktywacji wewnetrznego regulatora pogodowego
    if(!PFNf_FLIN) PFN.STRB=0;
#else
    BitSet3(&RSDT1.inLNSTAT,_LN_RT,0);                //zdalne pobudzenie RT  
    BitSet3(&RSDT1.inLNSTAT,_LN_CW,0);                //zdalne pobudzenie timera zasobnika
    BitSet3(&RSDT1.inLNSTAT,_LN_KSK,0);               //znacznik pracy w trybie kaskady  
    BitSet3(&RSDT1.inLNSTAT,_LN_STR,0);               //znacznik pracy w trybie sterowania z managera strefy zlub regulatora RT
    BitSet3(&RSDT1.inLNSTAT,_LN_DPGD,0);              //znacznik dezaktywacji wewnetrznego regulatora pogodowego  
    PFN.STRB=0;
#endif    

#if (_EXT_BOARD>=3)                     //aktywowany modul OT?   
    if(PFN.STRB==2)                     //odbior produkcyjny?
    {
        BitSet3(&RSDT1.inINSTAT,_OT_RT,0);                //zdalne pobudzenie RT  
        BitSet3(&RSDT1.inINSTAT,_OT_CW,0);                //zdalne pobudzenie timera zasobnika  
        BitSet3(&RSDT1.inINSTAT,_OT_STR,0);               //znacznik pracy w trybie sterowania z managera strefy lub z regulatora RT 
        BitSet3(&RSDT1.inINSTAT,_OT_DPGD,0);              //dezaktywacja wewnetrznego regulatora pogodowego        
    }
    else
    {    
        if(OTMaster.Ref._Status)
        {
            BitSet3(&RSDT1.inINSTAT,_OT_RT,(OTMaster.Status._CH_enable&&PFNf_FOT));                //zdalne pobudzenie RT   
        }
        else
        {
            BitSet3(&RSDT1.inINSTAT,_OT_RT,0);                //zdalne pobudzenie RT          
        }
        if(OTMaster.Ref._Status)
        {
            BitSet3(&RSDT1.inINSTAT,_OT_CW,(PFNf_EnAleg&&OTMaster.Status._DHW_enable&&PFNf_FOT));   //zdalne pobudzenie timera zasobnika  (dla kotlow jednofunkcyjnych))
        }
        else
        {
            BitSet3(&RSDT1.inINSTAT,_OT_CW,0);                  //zdalne pobudzenie timera zasobnika  (dla kotlow jednofunkcyjnych))        
        }
        BitSet3(&RSDT1.inINSTAT,_OT_STR,(OTMaster.Ref._Control_Setpoint&&PFNf_FOT));                  //znacznik pracy z zewnetrnie regulowanym SETP 
        BitSet3(&RSDT1.inINSTAT,_OT_DPGD,(PFNf_OT_DPGD&&PFNf_FOT));                                   //dezaktywacja wewnetrznego regulatora pogodowego  
    }
#else
    BitSet3(&RSDT1.inINSTAT,_OT_RT,0);                //zdalne pobudzenie RT  
    BitSet3(&RSDT1.inINSTAT,_OT_CW,0);                //zdalne pobudzenie timera zasobnika  
    BitSet3(&RSDT1.inINSTAT,_OT_STR,0);               //znacznik pracy w trybie sterowania z managera strefy lub z regulatora RT 
    BitSet3(&RSDT1.inINSTAT,_OT_DPGD,0);              //dezaktywacja wewnetrznego regulatora pogodowego    
#endif  

    PFNf_ex_pgd=(PFNf_tmpZW&&!PFNf_pgd&&((PFNf_OT_DPGD&&PFNf_FOT)||(PFNf_LIN_DPGD&&PFNf_FLIN)))?1:0; ////znacznik aktywnosci zewnetrznego regulatora pogodowego    
}
//--------------------------------------------------------------
void MarkMinMax(unsigned char shCW,unsigned char shCO)
{
	Mark(_GZ_MNCW,shCW&&(PFN.PCW<=PFN.minPCW));
	Mark(_GZ_MXCW,shCW&&(PFN.PCW>=PFN.maxPCW));
	Mark(_GZ_MNCO,shCO&&(PFN.PCO<=PFN.minPCO));
	Mark(_GZ_MXCO,shCO&&(PFN.PCO>=PFN.maxPCO));
}
void MarkMinMaxECO(void)
{
	Mark(_GZ_MNCW,0);
	Mark(_GZ_MXCW,0);
	Mark(_GZ_MNCO,PFN.ECO<=1);
	Mark(_GZ_MXCO,PFN.ECO>=9);
}
//--------------------------------------------------------------
#if _TDEMO==0
//Ekspozycja wersji i konfiguracji p³ytek
unsigned char PrintVerBoards(unsigned char ref)
{
	if(ref)
	{
 		M.PVB=0;
		StartRTdS(_RTVER);
		return 0;
	}
	switch(M.PVB)
	{
		case 0:	
        {
			ClrDig(1,1,0);				//wyczysc ekran
			Mark(_GZ_MXCW,0);			//wygas migajacy symbol max
			Mark(_GZ_OFF,0);			//wygas migajacy symbol OFF
			StartRTdS(_RTVER);
			M.PVB=1;
			return 0;
        }
		case 1:	//b1
        {
			if(RTdS(_RTVER)<5) return 0;
			StartRTdS(_RTVER);
            if(PFN.vUNI02)
            {
                PrintVER(0xb1,PFN.vUNI02,1);			//wyswietl parametr dla b1 
            }
            else
            {
                PrintVER(0xb1,-1,1);			//wyswietl parametr dla b1                
            }
			M.PVB=2;
            return 0;
        }
		case 2:		
        {
			if(RTdS(_RTVER)<=15) return 0;
			StartRTdS(_RTVER);
			ClrDig(1,1,0);				//wyczysc ekran
			M.PVB=3;
            return 0;
        }
		case 3://b2
        {
			if(RTdS(_RTVER)<5) return 0;
			StartRTdS(_RTVER);
			PrintVER(0xb2,_WERSJA,1);	//wyswietl paranetr dla b2
			M.PVB=4;
            return 0;
        }
		case 4:
        {
			if(RTdS(_RTVER)<=15) return 0;
			StartRTdS(_RTVER);
			ClrDig(1,1,0);				//wyczysc ekran
			M.PVB=5;
            return 0;
        }
		case 5: //aktualna konfiguracja sterownika (0-Unico,1-miniterm,2-maxiterm,3-minimax plus) 
        {
			if(RTdS(_RTVER)<5) return 0;
			StartRTdS(_RTVER);
#if (_EXT_BOARD>=2)            
            if(!PFN.vUNI02)
            {
                switch(PFN.RDZ0&0b00011111)
                {
                    case 0x10:                   //UNICO
                    case 0x11:                   //+turbo     
                    {
                        PrintVER(0x1F,-1,1);	//wyswietl 2F dla kotla dwufunkcyjnego
                        break;
                    }
                    case 0x12:                  //MINITERM
                    case 0x13:                  //+turbo                      
                    {
                        PrintVER(0x2F,-1,1);	//wyswietl 2F dla kotla dwufunkcyjnego
                        break;                         
                    }
                    case 0x14:                  //MAKSITERM
                    case 0x15:                  //+turbo
                    {
                        PrintVER(0x3F,-1,1);	//wyswietl 2F dla kotla dwufunkcyjnego
                        break;                                                 
                    }
                    case 0x16:                  //MONOTERM
                    case 0x17:                  //+turbo
                    {
                        PrintVER(0x4F,-1,1);	//wyswietl 2F dla kotla dwufunkcyjnego
                        break;                                                 
                    }
                }                
            }
            else
            {
                switch(PFN.RDZ0&0b00011111)
                {
                    case 0x10:                   //UNICO
                    case 0x11:                   //+turbo
                    {
                        PrintVER(0x1F,(PFN.ALIN<5)?PFN.ALIN:(-1),1);	//wyswietl 2F dla kotla dwufunkcyjnego
                        break;
                    }
                    case 0x12:                  //MINITERM
                    case 0x13:                  //+turbo
                    {
                        PrintVER(0x2F,(PFN.ALIN<5)?PFN.ALIN:(-1),1);	//wyswietl 2F dla kotla dwufunkcyjnego
                        break;                         
                    }
                    case 0x14:                  //MAKSITERM
                    case 0x15:                  //+turbo
                    {
                        PrintVER(0x3F,(PFN.ALIN<5)?PFN.ALIN:(-1),1);	//wyswietl 2F dla kotla dwufunkcyjnego
                        break;                                                 
                    }
                    case 0x16:                  //MONOTERM
                    case 0x17:                  //+turbo
                    {
                        PrintVER(0x4F,(PFN.ALIN<5)?PFN.ALIN:(-1),1);	//wyswietl 2F dla kotla dwufunkcyjnego
                        break;                                                 
                    }
                }
            }
#else
            switch(PFN.RDZ0&0b00011111)
            {
                case 0x10:                   //UNICO
                case 0x11:                   //+turbo
                {
                    PrintVER(0x1F,-1,1);	//wyswietl 2F dla kotla dwufunkcyjnego
                    break;
                }
                case 0x12:                  //MINITERM
                case 0x13:                  //+turbo
                {
                    PrintVER(0x2F,-1,1);	//wyswietl 2F dla kotla dwufunkcyjnego
                    break;                         
                }
                case 0x14:                  //MAKSITERM
                case 0x15:                  //+turbo
                {
                    PrintVER(0x3F,-1,1);	//wyswietl 2F dla kotla dwufunkcyjnego
                    break;                                                 
                }
                case 0x16:                  //MONOTERM
                case 0x17:                  //+turbo
                {
                    PrintVER(0x4F,-1,1);	//wyswietl 2F dla kotla dwufunkcyjnego
                    break;                                                 
                }
            }                        
#endif
			M.PVB=6;
            return 0;
        }
		case 6:
        {
			if(RTdS(_RTVER)<=15) return 0;
			StartRTdS(_RTVER);
			ClrDig(1,1,0);				//wyczysc ekran
			M.PVB=7;
            return 0;
        }
		case 7://zakonczenie procedury
        {
			if(RTdS(_RTVER)<5) return 0;
			M.PVB=8;
            return 0;
        }
		case 8:
        {
			//Mark(_GZ_MXCW,2);			//zpal migajacy symbol max	
			//Mark(_GZ_OFF,2);			//zapal migajacy symbol OFF
			M.PVB=9;
            return 1;
        }
		case 9:
		return 1;
	}
    return 1;
}
#if _KEY_BBOARD==0
void NastawaCOCW(void)//unsigned char one)
{
	volatile static unsigned char obg_cw=0,obg_co=0,zmn2=0,pcw,eco;


	if(PFNf_ofnserw||PFNf_set_fnserw||PFNf_oaleg||PFNf_set_aleg) //aktywna funkcja serwisowa lub antylegionella?
	{
		PFNf_set_ncw=0;
		PFNf_set_nco=0;
		PFNf_set_eco=0;
		PFNf_str_nco=0;
	}
//-----------------
	if(!PFNf_set_ncw&&!obg_cw&&PFNf_obgCW)	//uruchomienie w obiegu CW?
	{
		PFNf_str_ncw=1;
		PFNf_b_str_ncw=1;
		StartRTdS(_RTSTR);
		if(!PFNf_fshowLpulse) SetLightLLed(1);	//rozswietlenie maksymalne
	}
	obg_cw=PFNf_obgCW;
	if(PFNf_b_str_ncw)
	{
		PFNf_b_str_ncw=0;
		EndNst();
		PrintNstCW(PFN.PPCW,0);//(!PFNf_ferr&&!PFNf_fmsg));
	}
//-----------------
        
	if(!PFNf_set_nco&&!obg_co&&PFNf_obgCO)	//uruchomienie w obiegu CO?
	{
		PFNf_str_nco=1;
		PFNf_b_str_nco=1;
		StartRTdS(_RTSTR);
	}
	obg_co=PFNf_obgCO;
	if(PFNf_b_str_nco)
	{
		PFNf_b_str_nco=0;
		EndNst();
		if(PFNf_ofnserw||PFNf_set_fnserw)	//aktywna funkcja serwisowa?
		{
			PFNf_set_nco=0;
			PFNf_str_nco=0;
			EndNst();
		}
		else 
		{
			if(!PFNf_EnAleg)
			{
				PrintNstCO(PFN.PPCO,0,0);
			}
			else
			{
				if(PFN.PCW<PFN.minPCW) PrintNstCO(PFN.PPCO,0,1);
				else PrintNstCO(PFN.PPCO,0,0);	
			}
		}
	}
        
//-----------------
	if(PFNf_set_nco||PFNf_set_ncw||RTdS(_RTSTR)>_TSTRNST)				//uplynal czas podtrzymania nastawy po rozruchu?
	{
		PFNf_str_nco=0;
		PFNf_str_ncw=0;
	} 
//-----------------
	if(PFNf_set_ncw)	//wybrano nastawe CW?
	{
		pcw=PFN.PCW;
		//na potrzeby parametru ECO przytrzymanie przycisku +/- wstrzymuje nastawe
		if(!PFNf_mod_ncw&&!RdPrt(S_KUP)&&!RdPrt(S_KDW))
		{
			PFNf_mod_ncw=1;
		}
		/*if(!PFNf_EnAleg)
		{
			if(PFNf_mod_ncw) ReadParam((unsigned int *)(&PFN.PCW),&zmn,&zps,PFN.minPCW,PFN.maxPCW,0,1);
		}
		else
		{
			if(PFNf_mod_ncw) ReadParam((unsigned int *)(&PFN.PCW),&zmn,&zps,PFN.minPCW-1,PFN.maxPCW,0,1);
		}*/
        if(PFNf_mod_ncw) ReadParam((unsigned int *)(&PFN.PCW),&zmn,&zps,PFN.minPCW,PFN.maxPCW,0,1);
		if(!PFNf_b_set_ncw)	//nacisnieto przycisk set?
		{
			PFNf_b_set_ncw=1;
			EndNst();
			zmn2=1;
			pcw=PFN.PCW;				
		}
		if(zmn2||zmn)				//nastawa ulegla zmianie?
		{
			PFNf_newPCW=(pcw!=PFN.PCW);
			zmn2=0;
			StartRTdS(_RTSET);
			/*if(!PFNf_EnAleg)
			{
				PrintNstCW(PFN.PCW,0);	//PrintNstCW(PFN.PCW,1);
			}
			else
			{
				PrintNstCW2(PFN.PCW,PFN.minPCW-1,0);	//PrintNstCW(PFN.PCW,1);
			}*/
            PrintNstCW(PFN.PCW,0);
		}
	}
//-----------------
	if(PFNf_set_nco)	//wybrano nastawe CO?
	{
		pcw=PFN.PCO;
#if (_EXT_BOARD==0)     //stara wersja plytki wyswietlacza   
		ReadParam((unsigned int *)(&PFN.PCO),&zmn,&zps,PFN.minPCO,PFN.maxPCO,0,1);
#else
#if (_EXT_BOARD==1)     //nowa wersja plytki wyswietlacza z interfejsem 10V  
        ReadParam((unsigned int *)(&PFN.PCO),&zmn,&zps,PFN.minPCO,PFN.maxPCO,0,1);
#endif
#if (_EXT_BOARD==2)     //nowa wersja plytki wyswietlacza z interfejsem 10V i LIN   
        if(!PFNf_LIN_KSK)    //nieaktywny interfejs 10V, interfejs LIN w trybie kaskady lub zdalnego sterowania ze strefy czy RT?
        {
            ReadParam((unsigned int *)(&PFN.PCO),&zmn,&zps,PFN.minPCO,PFN.maxPCO,0,1);
        }
#endif 
#if (_EXT_BOARD==3)     //nowa wersja plytki wyswietlacza z interfejsem 10V, LIN i OT        
        if(!PFNf_LIN_KSK)    //nieaktywny interfejs 10V, interfejs LIN w trybie kaskady lub zdalnego sterowania ze strefy czy RT?
        {
            ReadParam((unsigned int *)(&PFN.PCO),&zmn,&zps,PFN.minPCO,PFN.maxPCO,0,1);
        }
#endif        
#endif
		if(!PFNf_b_set_nco)	//stan po nacisnieciu przycisku set?
		{
			PFNf_b_set_nco=1;
			EndNst();
			zmn2=1;
			pcw=PFN.PCO;
			PFNf_newPCO=0;				
		}
		if(zmn2||zmn)				//nastawa ulegla zmianie?
		{
			if(!PFNf_newPCO) PFNf_newPCO=(pcw!=PFN.PCO);
			zmn2=0;
			StartRTdS(_RTSET);
			PrintNstCO(PFN.PCO,PFNf_pgd,0);
		}
	}
//-----------------
	if(PFNf_set_eco)	//wybrano nastawe parametru ECO?
	{
		eco=PFN.ECO;
		if(!PFNf_mod_eco&&!RdPrt(S_KUP)&&!RdPrt(S_KDW))
		{
			PFNf_mod_eco=1;
		}
		if(PFNf_mod_eco) ReadParam((unsigned int *)(&PFN.ECO),&zmn,&zps,1,9,0,1);
		if(!PFNf_b_set_eco)	//nacisnieto przycisk set?
		{
			PFNf_b_set_eco=1;
			EndNst();
			zmn2=1;
			eco=PFN.ECO;				
		}
		if(zmn2||zmn)				//nastawa ulegla zmianie?
		{
			PFNf_newECO=(eco!=PFN.ECO);
			zmn2=0;
			StartRTdS(_RTSET);
			PrintNstECO(PFN.ECO);
		}
	}
//-----------------
	if(!PFNf_set_ncw&&!PFNf_set_nco&&!PFNf_set_eco)	//tryb modyfikacji nastawy jest nieaktywny?
	{
		if(!PFNf_str_ncw&&!PFNf_str_nco) EndNst();
		PFNf_b_set_ncw=0;
		PFNf_b_set_nco=0;
		PFNf_b_set_eco=0;
	}

}
#endif
#if _KEY_BBOARD==1
void NastawaCOCW(void)//unsigned char one)
{
	volatile static unsigned char obg_cw=0,obg_co=0,zmn2=0,pcw,pco,eco;


	if(PFNf_ofnserw||PFNf_set_fnserw||PFNf_oaleg||PFNf_set_aleg) //aktywna funkcja serwisowa lub antylegionella?
	{
		PFNf_set_ncw=0;
		PFNf_set_nco=0;
		PFNf_set_eco=0;
		PFNf_str_nco=0;
	}
//-----------------
	if(!PFNf_set_ncw&&!obg_cw&&PFNf_obgCW)	//uruchomienie w obiegu CW?
	{
		PFNf_str_ncw=1;
		PFNf_b_str_ncw=1;
		StartRTdS(_RTSTR);
		if(!PFNf_fshowLpulse) SetLightLLed(1);	//rozswietlenie maksymalne
	}
	obg_cw=PFNf_obgCW;
	if(PFNf_b_str_ncw)
	{
		PFNf_b_str_ncw=0;
		EndNst();
		PrintNstCW(PFN.PPCW,0);//(!PFNf_ferr&&!PFNf_fmsg));
	}
//-----------------
	if(!PFNf_set_nco&&!obg_co&&PFNf_obgCO)	//uruchomienie w obiegu CO?
	{
		PFNf_str_nco=1;
		PFNf_b_str_nco=1;
		StartRTdS(_RTSTR);
	}
	obg_co=PFNf_obgCO;
	if(PFNf_b_str_nco)
	{
		PFNf_b_str_nco=0;
		EndNst();
		if(PFNf_ofnserw||PFNf_set_fnserw)	//aktywna funkcja serwisowa?
		{
			PFNf_set_nco=0;
			PFNf_str_nco=0;
			EndNst();
		}
		else 
		{
			if(!PFNf_EnAleg)
			{
				PrintNstCO(PFN.PPCO,0,0);
			}
			else
			{
				if(PFN.PCW<PFN.minPCW) PrintNstCO(PFN.PPCO,0,1);
				else PrintNstCO(PFN.PPCO,0,0);	
			}
		}
	}
        
//-----------------
	if(PFNf_set_nco||PFNf_set_ncw||RTdS(_RTSTR)>_TSTRNST)				//uplynal czas podtrzymania nastawy po rozruchu?
	{
		PFNf_str_nco=0;
		PFNf_str_ncw=0;
	} 
//-----------------
	if(PFNf_set_ncw)	//wybrano nastawe CW?
	{
		pcw=PFN.PCW;
        
		/*if(!PFNf_EnAleg)
		{
			ReadParamCW((unsigned int *)(&PFN.PCW),&zmn,&zps,PFN.minPCW,PFN.maxPCW,0,1);
		}
		else
		{
			ReadParamCW((unsigned int *)(&PFN.PCW),&zmn,&zps,PFN.minPCW-1,PFN.maxPCW,0,1);
		}*/
        ReadParamCW((unsigned int *)(&PFN.PCW),&zmn,&zps,PFN.minPCW,PFN.maxPCW,0,1);
		if(!PFNf_b_set_ncw)	//nacisnieto przycisk set?
		{
			PFNf_b_set_ncw=1;
			EndNst();
			zmn2=1;
			pcw=PFN.PCW;		
            
            PFNf_set_nco=0;
            PFNf_set_eco=0;        
            PFNf_b_set_nco=0;
            PFNf_b_set_eco=0;            
		}
		if(zmn2||zmn)				//nastawa ulegla zmianie?
		{
			PFNf_newPCW=(pcw!=PFN.PCW);
			zmn2=0;
			StartRTdS(_RTSET);
            /*
			if(!PFNf_EnAleg)
			{
				PrintNstCW(PFN.PCW,0);	//PrintNstCW(PFN.PCW,1);
			}
			else
			{
				PrintNstCW2(PFN.PCW,PFN.minPCW-1,0);	//PrintNstCW(PFN.PCW,1);
			}*/
            PrintNstCW(PFN.PCW,0);
		}
	}
//-----------------
	if(PFNf_set_nco)	//wybrano nastawe CO?
	{
		pco=PFN.PCO;
		//na potrzeby parametru ECO przytrzymanie przycisku +/- wstrzymuje nastawe
		/*if(!PFNf_mod_ncw&&!RdPrt(S_KUP_CO)&&!RdPrt(S_KDW_CO))
		{
			PFNf_mod_ncw=1;
		}*/
        PFNf_mod_ncw=1;        //w kotlach atmosferycznych brak ECO
#if (_EXT_BOARD==0)     //stara wersja plytki wyswietlacza   
		if(PFNf_mod_ncw) ReadParamCO((unsigned int *)(&PFN.PCO),&zmn,&zps,PFN.minPCO,PFN.maxPCO,0,1);
#else
#if (_EXT_BOARD==1)     //nowa wersja plytki wyswietlacza z interfejsem 10V  
        if(PFNf_mod_ncw) ReadParamCO((unsigned int *)(&PFN.PCO),&zmn,&zps,PFN.minPCO,PFN.maxPCO,0,1);
#endif
#if (_EXT_BOARD==2)     //nowa wersja plytki wyswietlacza z interfejsem 10V i LIN   
        if(!PFNf_LIN_KSK)    //nieaktywny interfejs 10V, interfejs LIN w trybie kaskady lub zdalnego sterowania ze strefy czy RT?
        {
            if(PFNf_mod_ncw) ReadParamCO((unsigned int *)(&PFN.PCO),&zmn,&zps,PFN.minPCO,PFN.maxPCO,0,1);
        }
#endif 
#if (_EXT_BOARD==3)     //nowa wersja plytki wyswietlacza z interfejsem 10V, LIN i OT        
        if(!PFNf_LIN_KSK)    //nieaktywny interfejs 10V, interfejs LIN w trybie kaskady lub zdalnego sterowania ze strefy czy RT?
        {
            if(PFNf_mod_ncw) ReadParamCO((unsigned int *)(&PFN.PCO),&zmn,&zps,PFN.minPCO,PFN.maxPCO,0,1);
        }
#endif        
#endif
		if(!PFNf_b_set_nco)	//stan po nacisnieciu przycisku set?
		{
			PFNf_b_set_nco=1;
			EndNst();
			zmn2=1;
			pco=PFN.PCO;
			PFNf_newPCO=0;				
            
            PFNf_set_ncw=0;        
            PFNf_b_set_ncw=0;            
		}
		if(zmn2||zmn)				//nastawa ulegla zmianie?
		{
			if(!PFNf_newPCO) PFNf_newPCO=(pco!=PFN.PCO);
			zmn2=0;
			StartRTdS(_RTSET);
			PrintNstCO(PFN.PCO,PFNf_pgd,0);
		}
	}
//-----------------
	/*if(PFNf_set_eco)	//wybrano nastawe parametru ECO?
	{
		eco=PFN.ECO;
		if(!PFNf_mod_eco&&!RdPrt(S_KUP_CO)&&!RdPrt(S_KDW_CO))
		{
			PFNf_mod_eco=1;
		}
		if(PFNf_mod_eco) ReadParamCO((unsigned int *)(&PFN.ECO),&zmn,&zps,1,9,0,1);
		if(!PFNf_b_set_eco)	//nacisnieto przycisk set?
		{
			PFNf_b_set_eco=1;
			EndNst();
			zmn2=1;
			eco=PFN.ECO;		
            
            PFNf_set_ncw=0;        
            PFNf_b_set_ncw=0;             
		}
		if(zmn2||zmn)				//nastawa ulegla zmianie?
		{
			PFNf_newECO=(eco!=PFN.ECO);
			zmn2=0;
			StartRTdS(_RTSET);
			PrintNstECO(PFN.ECO);
		}
	}*/
//-----------------
	if(!PFNf_set_ncw&&!PFNf_set_nco&&!PFNf_set_eco)	//tryb modyfikacji nastawy jest nieaktywny?
	{
		if(!PFNf_str_ncw&&!PFNf_str_nco) EndNst();
		PFNf_b_set_ncw=0;
		PFNf_b_set_nco=0;
		PFNf_b_set_eco=0;
	}
}
#endif
//--------------------------------------------------------------
void DecWsw(void)
{    
    volatile unsigned char fl;
    
    if(PFNf_fsetLaddr||PFNf_fshowLaddr)              //aktywna funkcja przypisania/wizualizacji adresu LIN?
    {
#if _KEY_BBOARD==0             
        if(!RdPrt(S_SET)) PFNf_setKey=0;
#endif
#if _KEY_BBOARD==1             
        if(!RdPrt(S_KOM)) PFNf_setKey=0;
#endif            
        //--------------
        EndErr();
        if(PFN.ALIN>=0x05) PrintLN(0);
        else PrintLN(PFN.ALIN);
        SetPulseLLed();
        if(PFNf_fsetLaddr) PFNf_fshowLaddr=0;
        if(RTdS(_RTALIN)>30)                        //czas przez jaki wyswietlany jest adres LIN
        {
            PFNf_fshowLaddr=0;
            if(PFNf_fsetLaddr)
            {
#if _KEY_BBOARD==0                    
                if(!RdPrt(S_SET))           //poszczony przycisk SET?
                {
                    PFNf_fsetLaddr=0;
                    SetPrt(S_SET,0);
                }
#endif
#if _KEY_BBOARD==1                    
                if(!RdPrt(S_KOM))           //poszczony przycisk KOM?
                {
                    PFNf_fsetLaddr=0;
                    SetPrt(S_KOM,0);
                }
#endif                    
            }
        }
    }
    else
    {
        if(PFNf_ferr||PFNf_fmsg||PFNf_prODP)        //blad lub komunikat?
        {
            if(!PFNf_fshowLpulse)       //nieaktywna pulsacja rozswietlenia?
            {
                SetLightLLed(1); 	//rozswietlenie maksymalne
            }
            else
            {
                SetPulseLLed();                         //pulsacja rozswietlenia
                if(PFNf_foneLpulse&&(RTdS(_RTALIN)>30)) //czas po jakim nastapi wylaczenie pulsacji
                {
                    PFNf_fshowLpulse=0;
                    PFNf_foneLpulse=0;
                }
            }
            NastawaCOCW();

            if(PFNf_ferr)
            {
                if(!PFNf_KnfDisErr||PFNf_opgres)
                {
                    if(!PFNf_set_nco&&!PFNf_set_ncw&&!PFNf_set_eco) ClrMarkMinMax();
                    if(PFNf_opgres) PrintErrMsg(_PRN_ERR,led_E,PFN.KERR,0,0,PFNf_opgres);							//migajacy kod awaryjny z blokada
                    else
                    if((PFN.KERR!=0x07)&&(PFN.KERR!=0x0E)&&(PFN.KERR!=0x09)) PrintErrMsg(_PRN_MSG,led_E,PFN.KERR,0,0,PFNf_opgres);		//migajacy kod awaryjny bez blokady
                    else PrintErrMsg(_PRN_MSG,led_E,PFN.KERR,(PFN.KERR==0x07)||(PFN.KERR==0x0E),PFN.KERR==0x09,PFNf_opgres);			//naprzemienny kod awaryjny
                }
                else
                {
                    EndErr();
                }
            }
            else
            if(PFNf_fmsg)
            {
                if(!PFNf_KnfDisErr)
                {
                    if(!PFNf_set_nco&&!PFNf_set_ncw&&!PFNf_set_eco) ClrMarkMinMax();
                    PrintErrMsg(_PRN_MSG,led_E,PFN.KAWR,1,1,PFNf_opgres);			//staly kod awaryjny
                }else EndErr();
            }
            else
            if(PFNf_prODP)
            {    
                PrintPo(PFN.CS);
                EndErr();
            }
        }
        else
        {
            if(!PFNf_fshowLpulse)                       //nieaktywna pulsacja rozswietlenia?
            {
                if(PFNf_rozruch)
                {
                    SetLightLLed(1);	//rozswietlenie maksymalne
                }
                else
                {
                    SetLightLLed(0);                   //sterowane rozswietlenie
                }
            }
            else
            {
                SetPulseLLed();                         //pulsacja rozswietlenia
                if(PFNf_foneLpulse&&(RTdS(_RTALIN)>30)) //czas po jakim nastapi wylaczenie pulsacji
                {
                    PFNf_fshowLpulse=0;
                    PFNf_foneLpulse=0;
                }
            }
            EndErr();
        }
    }
	if(!PFNf_fsetLaddr&&!PFNf_fshowLaddr&&!PFNf_opKNF&&!PFNf_ferr&&!PFNf_fmsg&&!PFNf_prODP)
	{
        if(PFNf_fshowLpulse)       //aktywna pulsacja rozswietlenia?
        {
            SetPulseLLed();                         //pulsacja rozswietlenia
            if(PFNf_foneLpulse&&(RTdS(_RTALIN)>30)) //czas po jakim nastapi wylaczenie pulsacji
            {
                PFNf_fshowLpulse=0;
                PFNf_foneLpulse=0;
            }
        }          
		if(PFNf_ofoff)
		{
			//decyzje o wygaszeniu
			if(PFNf_errCS&&!PFNf_tdzCO) ClrDig(0,1,1);
			if(!PFNf_tdzCW) ClrDig(1,0,0);
			//wskazy
			if(!PFNf_errCS&&!PFNf_tdzCO) {Mark(_GZ_DCO,0);PrintCS(PFN.CS,1);}
			if(PFNf_tdzCW&&!PFNf_errCW) PrintCW(PFN.CW,1);
			if(PFNf_tdzCO&&!PFNf_errCO) PrintCO(PFN.CO,1,PFNf_fL3);
		}
		else
		{
			//-----------------------------------
			//tryb LATO
			if(PFNf_oflato)
			{
				if(!PFNf_oaleg||PFNf_set_aleg)
				{
					if(PFNf_obgCO||PFNf_tdzCO)												//temp. dyzurna CO?
					{
						if(PFNf_prn_res)
						{
							if(PFNf_prn_cs)													//czasowy wskaz cisnienia
							{
								if(!PFNf_EnAleg)
								{
									PrintCS(PFN.CS,0);											//wylaczny wskaz CS
								}
								else
								{
									if(PFN.PCW<PFN.minPCW||!PFNf_timZAS) PrintCS(PFN.CS,1);					//dzielony wskaz CS
									else 
									{
											if(!PFNf_errCW) 
											{
												PrintCS(PFN.CS,1);								//dzielony wskaz CS
												PrintCW(PFN.CW,1);								//dzielony wskaz temp CW
											}
											else PrintCS(PFN.CS,0);								//wylaczny wskaz CS
									}
								}											
							}
							else
							if(PFNf_prn_in) PrintIN(PFN.IN);						//wylaczny wskaz IN
                            else
                            if(PFNf_prn_pp) PrintPP(PFN.PP);						//wylaczny wskaz PP
                            else
                            if(PFNf_prn_pg) PrintPG(PFN.PG);						//wylaczny wskaz PG                            
                            
							/*else
							{
								if(PFNf_prn_in) PrintIN(PFN.IN);							//wylaczny wskaz IN
							}*/
						}
						else
						{	
							if(!PFNf_EnAleg)
							{
								if(!PFNf_errCO) PrintCO(PFN.CO,0,PFNf_fL3); 						//wylaczny wskaz temperatury CO
								else ClrDig(0,1,0);
								ClrDig(1,0,0);
							}
							else
							{
								if(PFN.PCW<PFN.minPCW||!PFNf_timZAS)
								{
 									PrintCO(PFN.CO,1,PFNf_fL3); 						//dzielony wskaz temperatury CO
									ClrDig(1,0,1);
								}
								else 
								{
									if(!PFNf_errCW) 
									{
										PrintCO(PFN.CO,1,PFNf_fL3); 						//dzielony wskaz temperatury CO
										PrintCW(PFN.CW,1);									//dzielony wskaz temp CW
									}
									else PrintCO(PFN.CO,0,PFNf_fL3); 						//wylaczny wskaz temperatury CO
								}
							}
						}
					}
					else																	//wskaz/praca CW
					{
						if(PFNf_prn_res)
						{
							if(PFNf_prn_cs)													//czasowy wskaz cisnienia
							{
								if(!PFNf_errCW&&PFNf_obgCW)
								{
									PrintCS(PFN.CS,1);										//dzielony wskaz temp CS
									PrintCW(PFN.CW,1);										//dzielony wskaz temp CW
								}
								else
								{
									if(!PFNf_EnAleg)
									{
										PrintCS(PFN.CS,0);											//wylaczny wskaz CS
									}
									else
									{
										if(PFN.PCW<PFN.minPCW||!PFNf_timZAS) PrintCS(PFN.CS,1);					//dzielony wskaz CS
										else 
										{
												if(!PFNf_errCW) 
												{
													PrintCS(PFN.CS,1);								//dzielony wskaz CS
													PrintCW(PFN.CW,1);								//dzielony wskaz temp CW
												}
												else PrintCS(PFN.CS,0);								//wylaczny wskaz CS
										}
									}
								}
							}
							else
							if(PFNf_prn_in) 
                            {
                                PrintCo(PFN.rCO);                                   //wylaczny wskaz CO (wartosc rzeczywista lub przefiltrowana)
                                /*if(RSDT1.vSTD>=2)
                                {
                                    PrintCo(PFN.rCO);                               //wylaczny wskaz CO (wartosc rzeczywista)
                                }
                                else
                                {
                                    PrintCo(PFN.CO);                                //wylaczny wskaz CO  (filtr)                                      
                                }*/
                            }
                            else
                            if(PFNf_prn_pp) PrintPP(PFN.PP);						//wylaczny wskaz PP
                            else
                            if(PFNf_prn_pg) PrintPG(PFN.PG);						//wylaczny wskaz PG                            
						}		
						else																//tylko wskaz temp. CW przy pobudzeniu
						{
 							if(!PFNf_EnAleg)
							{
								if(!PFNf_errCW&&PFNf_obgCW) PrintCW(PFN.CW,0);
								else
								if(!PFNf_errCW&&PFNf_set_aleg) PrintCW(PFN.CW,0);
								else ClrDig(1,0,0);
								ClrDig(0,1,0);															//wygaszenie najmlodszych cyfr
							}
							else
							{
								if(!PFNf_errCW&&PFNf_obgCW) PrintCW(PFN.CW,0);
								else
								if(!PFNf_errCW&&PFNf_set_aleg) PrintCW(PFN.CW,0);
								else
								if(PFN.PCW<PFN.minPCW||!PFNf_timZAS) ClrDig(1,0,1);
								else
								{
 									ClrDig(0,0,0);
									if(!PFNf_errCW) PrintCW(PFN.CW,0);
								}
								ClrDig(0,1,0);
							}
						}
					}
				}
				else
				{
					if(!PFNf_EnAleg)
					{
						if(!PFNf_errCW&&PFNf_obgCW) PrintCW(PFN.CW,0);
						else ClrDig(1,0,0);
						ClrDig(0,1,0);															//wygaszenie najmlodszych cyfr
					}
					else
					{
						
						if(!PFNf_errCW&&PFNf_obgCW) PrintCW(PFN.CW,0);
						else
						if(PFN.PCW<PFN.minPCW||!PFNf_timZAS) ClrDig(1,0,1);
						else ClrDig(1,0,0);
						ClrDig(0,1,0);
					}
				}				
			}
			else
			//-----------------------------------
			//tryb ZIMA
			if(PFNf_ofzima)
			{
				if(!PFNf_ofnserw&&!PFNf_oaleg&&!PFNf_set_fnserw&&!PFNf_set_aleg)
				{
					if(!PFNf_obgCO&&!PFNf_obgCW)										//oba obiegi nieaktywne?
					{
						if(PFNf_prn_res)
						{
							if(PFNf_prn_cs)
							{
								if(!PFNf_EnAleg)
								{
									PrintCS(PFN.CS,0);							//wylaczny wskaz CS
								}
								else
								{
									if(PFN.PCW<PFN.minPCW||!PFNf_timZAS) PrintCS(PFN.CS,1);					//dzielony wskaz CS
									else 
									{
											if(!PFNf_errCW) 
											{
												PrintCS(PFN.CS,1);								//dzielony wskaz CS
												PrintCW(PFN.CW,1);								//dzielony wskaz temp CW
											}
											else PrintCS(PFN.CS,0);								//wylaczny wskaz CS
									}
								}								
							}
							else
							if(PFNf_prn_in) PrintIN(PFN.IN);						//wylaczny wskaz IN
                            else
                            if(PFNf_prn_pp) PrintPP(PFN.PP);						//wylaczny wskaz PP
                            else
                            if(PFNf_prn_pg) PrintPG(PFN.PG);						//wylaczny wskaz PG
                            /*
							{
								if(PFNf_prn_in) PrintIN(PFN.IN);							//wylaczny wskaz IN
							} 
                             */                           
						}
						else 
						{
							if(!PFNf_EnAleg)
							{
								if(PFNf_fL3) PrintCO(PFN.CO,1,PFNf_fL3);
								else ClrDig(0,1,0);
								ClrDig(1,0,0);
							}
							else
							{
								if(PFNf_fL3)
								{
 									PrintCO(PFN.CO,1,PFNf_fL3);
									if(PFN.PCW<PFN.minPCW||!PFNf_timZAS) ClrDig(1,0,1);
									else
									{
										if(!PFNf_errCW) PrintCW(PFN.CW,1);
									}
								}
								else
								{
 									ClrDig(0,1,0);
									if(PFN.PCW<PFN.minPCW||!PFNf_timZAS) ClrDig(1,0,1);
									else
									{
										if(!PFNf_errCW) PrintCW(PFN.CW,0);
									}
								}
							}
						}
					}
					else																//dowolny obieg aktywny
					{
						if(PFNf_obgCO)													//aktywny obieg CO?
						{
							if(PFNf_prn_res)
							{
								if(PFNf_prn_cs)
								{
									if(!PFNf_EnAleg)
									{
										PrintCS(PFN.CS,0);							//wylaczny wskaz CS
									}
									else
									{
										if(PFN.PCW<PFN.minPCW||!PFNf_timZAS) PrintCS(PFN.CS,1);					//dzielony wskaz CS
										else 
										{
												if(!PFNf_errCW) 
												{
													PrintCS(PFN.CS,1);								//dzielony wskaz CS
													PrintCW(PFN.CW,1);								//dzielony wskaz temp CW
												}
												else PrintCS(PFN.CS,0);								//wylaczny wskaz CS
										}
									}
								}
								else
								if(PFNf_prn_in) PrintIN(PFN.IN);						//wylaczny wskaz IN
								else
								if(PFNf_prn_pp) PrintPP(PFN.PP);						//wylaczny wskaz PP
								else
								if(PFNf_prn_pg) PrintPG(PFN.PG);						//wylaczny wskaz PG
							}
							else														//wskaz CO
							{
								//wskaz temp CO
								if(!PFNf_errCO) 
								{
									if(!PFNf_EnAleg)
									{
										PrintCO(PFN.CO,0,PFNf_fL3);							//wylaczny wskaz temp CO
									}
									else
									{
										if(PFN.PCW<PFN.minPCW||!PFNf_timZAS) PrintCO(PFN.CO,1,PFNf_fL3);	//dzielony wskaz temp CO
										else 
										{
											if(!PFNf_errCW) 
											{
												PrintCO(PFN.CO,1,PFNf_fL3);						//dzielony wskaz temp CO
											}
											else PrintCO(PFN.CO,0,PFNf_fL3);					//wylaczny wskaz temp CO
										}
									}
								}
								else ClrDig(0,1,0);
								//wskaz temp CW
								if(!PFNf_EnAleg)
								{
									ClrDig(1,0,0);											//wygaszony wskaz WU
								}
								else
								{
									if(PFN.PCW<PFN.minPCW||!PFNf_timZAS) ClrDig(1,0,1);					//dwie kreski na WU
									else 
									{
										if(!PFNf_errCW) PrintCW(PFN.CW,1);					//dzielony wskaz temp CW
										else ClrDig(1,0,0);									//wygaszony wskaz WU
									}
								}
							}
						}
						else															//aktywny obieg CW
						{
							if(PFNf_prn_res)
							{
								if(PFNf_prn_cs)											//aktywny wskaz cisnienia?
								{
									PrintCS(PFN.CS,1);									//dzielony wskaz CS	
									if(!PFNf_errCW) 
									{
										PrintCW(PFN.CW,1);								//dzielony wskaz temp CW
									}
									else ClrDig(1,0,0);									//dwie kreski
								}
								else
								if(PFNf_prn_in) 
                                {
                                    PrintCo(PFN.rCO);                                   //wylaczny wskaz CO (wartosc rzeczywista lub przefiltrowana)
                                    /*if(RSDT1.vSTD>=2)
                                    {
                                        PrintCo(PFN.rCO);                               //wylaczny wskaz CO (wartosc rzeczywista)
                                    }
                                    else
                                    {
                                        PrintCo(PFN.CO);                               //wylaczny wskaz CO  (filtr)                                      
                                    }*/
                                }
								else
								if(PFNf_prn_pp) PrintPP(PFN.PP);						//wylaczny wskaz PP
								else
								if(PFNf_prn_pg) PrintPG(PFN.PG);						//wylaczny wskaz PG                                
							}
							else														//wskaz CW
							{
								if(!PFNf_errCW) PrintCW(PFN.CW,0);						//wylaczny wskaz temp CW
							}
						}                        
						/*else															//aktywny obieg CW
						{
							if(PFNf_prn_res)
							{
								if(PFNf_prn_cs)											//aktywny wskaz cisnienia?
								{
									PrintCS(PFN.CS,1);									//dzielony wskaz CS	
									if(!PFNf_errCW) 
									{
										PrintCW(PFN.CW,1);								//dzielony wskaz temp CW
									}
									else ClrDig(1,0,0);									//dwie kreski
								}
							}
							else														//wskaz CW
							{
								if(!PFNf_errCW) PrintCW(PFN.CW,0);						//wylaczny wskaz temp CW
							}
						}*/
					}
				}
				else																	//aktywna funkcja serwisowa lub antylegionella
				{
					if(PFNf_ofnserw||PFNf_set_fnserw) 									//aktywna funkcja serwisowa?								
					{
						if(!PFNf_fshowLpulse) SetLightLLed(1); 							//rozswietlenie maksymalne
						if(!PFNf_errCO) PrintCO(PFN.CO,1,PFNf_fL3);						//dzielony wskaz temp CO z wydatkiem procentowym modulatora
						//PrintVNT(PFN.VNT,1);
						PrintVNT(PFN.PG,1);                                             //wydatek modulatora                     
					}
					else																//aktywna funkcja antylegionella
					{
						if(!PFNf_EnAleg)
						{
							if(!PFNf_errCW&&PFNf_obgCW) PrintCW(PFN.CW,0);
							else
							if(!PFNf_errCW&&PFNf_set_aleg) PrintCW(PFN.CW,0);
							else ClrDig(1,0,0);
							ClrDig(0,1,0);															//wygaszenie najmlodszych cyfr
						}
						else
						{
							if(!PFNf_errCW&&PFNf_obgCW) PrintCW(PFN.CW,0);
							else
							if(!PFNf_errCW&&PFNf_set_aleg) PrintCW(PFN.CW,0);
							else
							if(PFN.PCW<PFN.minPCW||!PFNf_timZAS) ClrDig(1,0,1);
							else
							{
								if(!PFNf_errCW) PrintCW(PFN.CW,0);
								else ClrDig(1,0,0);
							}
							ClrDig(0,1,0);
						}
					}
				}			
			}
			//-----------------------------------
			//nastawa CO/CW
			NastawaCOCW();
			//-----------------------------------
		}
	}
	//znaki wysterowywane niezale¿nie (sygna³y z UNI-02)
	Mark(_GZ_PLW,PFNf_plmCW);												//znacznik plomien CW
	Mark(_GZ_PLC,PFNf_plmCO);												//znacznik plomien CO
	//Mark(_GZ_PGD,PFNf_pgd&&!PFNf_oflato&&!PFNf_ofoff&&!PFNf_opKNF);			//znacznik funkcja pogodowa
    if((PFNf_pgd||PFNf_ex_pgd)&&!PFNf_oflato&&!PFNf_ofoff&&!PFNf_opKNF)      //znacznik funkcja pogodowa
    {
        if(PFNf_pgd)        //wbudowany regulator pogodowy aktywny?
        {
            Mark(_GZ_PGD,1);     //staly znacznik funkcja pogodowa  
        }
        else                //zewnetrzny regulator pogodowy aktywny
        {
            Mark(_GZ_PGD,2);     //pulsujacy znacznik funkcja pogodowa 
        }
    }
    else
    {
        Mark(_GZ_PGD,0);     //wyma¿ znacznik funkcja pogodowa        
    }
#if _PRNL3==0   
    if(PFNf_ofzima&&!PFNf_ofoff&&!PFNf_opKNF)
    {
        if(PFNf_fL3)
        {
            fl=2;
        }
        else
        {
            fl=1;
        }
    }
    else 
    {
        fl=0;
    }
    Mark(_GZ_DCO,fl);					//znacznik KALORYFER 
#else
	Mark(_GZ_DCO,PFNf_ofzima&&!PFNf_ofoff&&!PFNf_opKNF);					//znacznik KALORYFER
#endif
	Mark(_GZ_DCW,(PFNf_ofzima||PFNf_oflato)&&!PFNf_ofoff&&!PFNf_opKNF);		//znacznik KRAN
	Mark(_GZ_OFF,PFNf_ofoff&&!PFNf_opKNF);									//znacznik OFF
        if(PFNf_fsetLaddr)
		Mark(_GZ_SRV,1);
        else
        if(PFNf_set_fnserw||PFNf_set_aleg)
		Mark(_GZ_SRV,2);								//znacznik KLUCZ migajacy
	else
	if((PFNf_oaleg||PFNf_ofnserw))
		Mark(_GZ_SRV,1);								//znacznik KLUCZ staly
	else
		Mark(_GZ_SRV,0);								//znacznik KLUCZ wygaszony

	if(!PFNf_opKNF&&!PFNf_ferr&&!PFNf_fmsg&&!PFNf_set_ncw&&!PFNf_set_nco&&!PFNf_set_eco)
	{
		if(PFNf_ofnserw||PFNf_set_fnserw||PFNf_oaleg||PFNf_set_aleg)
		{
			if(PFNf_oaleg||PFNf_set_aleg)		//antylegionella
			{
	 			Mark(_GZ_MNCW,0);													//wygas min CW
				Mark(_GZ_MXCW,PFNf_oaleg||PFNf_set_aleg);							//zapal max CW dla antylegionella
				Mark(_GZ_MXCO,0);
				Mark(_GZ_MNCO,0);
			}
			else								//funkca serwisowa
			{
				Mark(_GZ_MNCW,(PFNf_ofnserw||PFNf_set_fnserw)&&!PFNf_fns_max);		//zapal min CW dla serwisowej przy fmax=0
				Mark(_GZ_MXCW,(PFNf_ofnserw||PFNf_set_fnserw)&&PFNf_fns_max);		//zapal min CW dla serwisowej przy fmax=1	
				Mark(_GZ_MXCO,0);
				Mark(_GZ_MNCO,0);
			}
		}
		else
		{
			ClrMarkMinMax();
		}
	}
}
#else
//****************************************************************************************************88
void DecWsw(void)
{
	if(PFNf_ferr||PFNf_fmsg) 
	{
		ClrMarkMinMax();
		if(PFNf_ferr) PrintErr(led_E,PFN.KERR,1);
		else 
		{
			if(!PFNf_KnfDisErr) PrintErr(led_E,PFN.KAWR,0);
			else EndErr();
		}
	}
	else EndErr();
	if(!PFNf_aktywne_KNF&&!PFNf_ferr)
	{
		if(PFNf_off)
		{
			//decyzje o wygaszeniu
			if(PFNf_errCS&&!PFNf_tdzCO) ClrDig(0,1,1);
			if(!PFNf_tdzCW) ClrDig(1,0,0);
			//wskazy
			if(!PFNf_errCS&&!PFNf_tdzCO) {Mark(_GZ_DCO,0);PrintCS(PFN.CS,1);}
			if(PFNf_tdzCW&&!PFNf_errCW) PrintCW(PFN.CW,1);
			if(PFNf_tdzCO&&!PFNf_errCO) PrintCO(PFN.CO,1,PFNf_fL3);
		}
		else
		{
			//-----------------------------------
			//tryb LATO
			if(PFNf_lato)
			{
				if(!PFNf_aleg)
				{
					if(PFNf_errCW) ClrDig(1,0,1);
					if(!PFNf_tdzCO&&!PFNf_prn_cs) ClrDig(0,1,0);
					if(PFNf_prn_cs&&!PFNf_errCS) {Mark(_GZ_DCO,0);PrintCS(PFN.CS,1);}
					if(PFNf_tdzCO&&!PFNf_errCO&&!(PFNf_prn_cs&&!PFNf_errCS)) PrintCO(PFN.CO,1,PFNf_fL3);
					if(!PFNf_errCW) PrintCW(PFN.CW,1);
				}
				else
				{
					ClrDig(0,1,0);
					if(!PFNf_errCO) PrintCW(PFN.CW,1);
				}				
			}
			else
			//-----------------------------------
			//tryb ZIMA
			if(PFNf_zima)
			{
				if(!PFNf_fnserw&&!PFNf_aleg)
				{
					if(PFNf_errCW) ClrDig(1,0,1);
					if(PFNf_errCO&&!(PFNf_prn_cs&&!PFNf_errCS)) ClrDig(0,1,1);
					if(PFNf_prn_cs&&!PFNf_errCS) {Mark(_GZ_DCO,1);PrintCS(PFN.CS,1);}
					if(!PFNf_errCO&&!(PFNf_prn_cs&&!PFNf_errCS)) PrintCO(PFN.CO,1,PFNf_fL3);
					if(!PFNf_errCW) PrintCW(PFN.CW,1);
				}
				else
				{
					if(PFNf_fnserw)
					{
						ClrDig(1,0,0);
						if(!PFNf_errCO) PrintCO(PFN.CO,1,PFNf_fL3);
					}
					else
					{
						ClrDig(0,1,0);
						if(!PFNf_errCO) PrintCW(PFN.CW,1);
					}
				}			
			}
			//-----------------------------------
			//nastawa CO/CW
			if(PFNf_set_ncw)	//wybrano nastawe CW?
			{
				ReadParam(&PFN.PCW,&zmn,&zps,PFN.minPCW,PFN.maxPCW,0,1);
				//if(zmn) PFNf_newPCW=1;
				if(!PFNf_b_set_ncw)	//nacisnieto przycisk set?
				{
					PFNf_b_set_ncw=1;
					EndNst();
					zmn=1;				
				}
				if(zmn)				//nastawa ulegla zmianie?
				{
					PFNf_newPCW=1;
	 				StartRTdS(_RTSET);
					MarkMinMax(1,0);
					PrintNstCW(PFN.PCW,0);
				}
			}
			if(PFNf_set_nco)	//wybrano nastawe CO?
			{
				ReadParam(&PFN.PCO,&zmn,&zps,PFN.minPCO,PFN.maxPCO,0,1);
				//if(zmn) PFNf_newPCO=1;
				if(!PFNf_b_set_nco)	//nacisnieto przycisk set?
				{
					PFNf_b_set_nco=1;
					EndNst();
					zmn=1;				
				}
				if(zmn)				//nastawa ulegla zmianie?
				{
					PFNf_newPCO=1;
					StartRTdS(_RTSET);
					MarkMinMax(0,1);
					PrintNstCO(PFN.PCO,PFNf_pgd,0);
				}
			}
			if(!PFNf_set_ncw&&!PFNf_set_nco)	//tryb modyfikacji nastawy jest nieaktywny?
			{
				if(PFNf_b_set_ncw||PFNf_b_set_nco)	//deaktywacja trybu nastapila chwile wczesniej?
				{
					MarkMinMax(0,0);
	 			}
				EndNst();
				PFNf_b_set_ncw=0;
				PFNf_b_set_nco=0;
			}
			//-----------------------------------
		}
		//Mark(_GZ_OFF,PFNf_off);
		if(!PFNf_aleg) MarkSVAL((PFNf_fnserw)?2:0,PFNf_fns_max,0);	//sterowanie znacznikiem min/max
		if(!PFNf_fnserw) MarkSVAL((PFNf_aleg)?2:0,1,1);				//sterowanie znacznikiem min/max
	}
	//znaki wysterowywane niezale¿nie (sygna³y z UNI-01)
	Mark(_GZ_PLW,PFNf_plmCW);												//znacznik plomien CW
	Mark(_GZ_PLC,PFNf_plmCO);												//znacznik plomien CO
	Mark(_GZ_PGD,PFNf_pgd&&!PFNf_aktywne_KNF);								//znacznik funkcja pogodowa
	Mark(_GZ_DCO,(PFNf_zima||(PFNf_lato&&PFNf_tdzCO)||(PFNf_off&&PFNf_tdzCO))&&!PFNf_aktywne_KNF);					//znacznik KALORYFER
	Mark(_GZ_DCW,(PFNf_zima||PFNf_lato||(PFNf_off&&PFNf_tdzCW))&&!PFNf_aktywne_KNF);								//znacznik KRAN
	//Mark(_GZ_OFF,PFNf_off&&!PFNf_ferr&&!PFNf_fmsg&&!PFNf_aktywne_KNF);		//znacznik OFF
	Mark(_GZ_OFF,PFNf_off&&!PFNf_aktywne_KNF);		//znacznik OFF
}
//********************************************************************************************************
#endif
//---------------------------------------------------------------
//Decyzja o uruchomieniu procedury oczekiwania na polaczenie
void DecRes(void)
{
	switch(PFN.KRS)
	{
		case 0:
		{	
			if(PFNf_opgres&&PFNf_ferr)
			{
				PFN.KRS=1;
			}
			break;
		}
		case 1:
		{
			if(!PFNf_opgres||!PFNf_ferr)
			{
				PFNf_toInit=1;
				PFN.KRS=0;
			}
		}
	}
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
//------------------		
//------------------
//------------------		
//------------------
