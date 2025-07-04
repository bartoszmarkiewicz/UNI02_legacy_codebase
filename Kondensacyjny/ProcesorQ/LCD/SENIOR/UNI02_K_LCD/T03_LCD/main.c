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
#include "pomiar.tp.h"

extern volatile StPin ky[];
tpMAIN M;
volatile tpRSDTA1 RSDT1;
volatile DataPFN PFN; //****
volatile DataKFN KFN;
extern volatile CMSG_PAR2 CMSGDta;
extern volatile MGSRSData MGSD;
extern volatile tpOTMaster OTMaster;
extern volatile DataCO DtCO;
extern volatile DataCW DtCW;

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
		TestScr();
		PwrOnDelay();						//zwloka na wypadek stanow nieustalonych	
	}
	else
	{
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
    InitKonfLIN();      //kkk
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
            PFNf_fsetLTrb=0;           //tryb nadawania adresu (0-potwierdzany przyciskiem, 1-automatyczny) //nnn
            PFNf_fIgnore=0;            //ignoruj wszystkie komendy nadawania adresu //nnn
            PFNf_fProcess=0;        //nnn
            PFNf_fOne=0;               //ignoruj wszystkie adresy inne niz 1 //nnn             
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
			PFNf_aktywne_LKNF=0;	//****************************            //kkk            
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
    PFNf_toInitNST=1;                       //zdecyduj o pierwszym odczycie nastawy po resecie/zasileniu sterownika
    StartRTS(_RFNST);

	while(1)								//petla glowna programu				
	{
		CLRWDT();   
        TimeoutProc();        
        
        //TRANSMISJA LIN
#if (_EXT_BOARD>=2)
        LIN1SDeamon();                                      //LIN1 SLAVE
        if(RTdS(_RTFLIN)>_TFLIN) 
        {
            BitSet3(&RSDT1.inLNSTAT,_FLIN,0);        //odswiez znacznik komunikacji LIN dla ukladu UNI-02
            PFNf_FLIN=0;
            PFNf_LIN_RT=0;
            PFNf_LIN_CW=0; 
            PFNf_LIN_UP=0;  
            PFNf_LIN_BPSS=0;      //+++
            LIN2SClrMasterIDO();    //zeruj identyfikator nadajnika
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
            PFNf_OTRT=0;            
        }        
#else        
        BitSet3(&RSDT1.inINSTAT,_FOT,0);
        PFNf_FOT=0;
        PFNf_OT_DPGD=0;  
        PFNf_OTRT=0;         
#endif    
        //-----------------------------------------        
        PFNf_FEXT=(PFNf_FLIN||PFNf_FOT)?1:0;
		if(MGSDf_NO_ODB||PFNf_toInit) InitUNIData();		//odczekaj na polaczenie z UNI02
        else
        {          
            if(PFNf_toInitNST) InitNST();       //oczekiwanie na aktywnosc interfejsow LIN/OT
            else
            {
                PomiarPCO();
                PomiarPCW();
                ChkZKR();                       //kontrola zakresow nastaw        
                if(DtCW._newPOZ||DtCO._newPOZ||DtCW._newPCW||DtCO._newPCO)  //zmiana polozenia potencjometrow?
                {
                    if(DtCW._newPOZ||DtCO._newPOZ) PFNf_newPOZ=1;
                    DtCW._newPOZ=0;
                    DtCO._newPOZ=0;
                    SetTrbPRC();                //aktualizuj tryb pracy   
                }
                if(!PFNf_FEXT&&(PFNf_bFEXT!=PFNf_FEXT)) //zanik komunikacji LIN lub OT?
                {
                    SetNST();                   //aktualizuj nastawy
                    SetTrbPRC();                //aktualizuj tryb pracy                       
                } 
            }
        }
        PFNf_bFEXT=(PFNf_FEXT)?1:0;
        //-----------------------------------------
#if _EXT_BOARD==1
        Pomiar10V();        //interfejs 10V
#endif
#if _EXT_BOARD==2
        //if(!PFNf_LIN_KSK&&!PFNf_LIN_STR)
        if((PFNf_LIN_BPSS||!PFNf_LIN_KSK)&&(PFNf_LIN_BPSS||!PFNf_LIN_STR))       //+++
        {    
            Pomiar10V();        //interfejs 10V
        }
        else
        {
            PFNf_bEnF10V=0;             //znacznik do aktywacji wejscia 10V              
            PFNf_EnF10V=0;              //znacznik aktywnosci wejscia 10V  
            PFNf_b10V_RT=0;             //znacznik do aktywacji pobudzenia z wejscia 10V                
            PFNf_10V_RT=0;              //znacznik pobudzenia z wejscia 10V  
            PFNf_10VRT=0;               //znacznik pobudzenia z wejscia 10V            
            PFNf_10V_nPCO=0;            //znacznik nowej wartosci PCO z wejscia 10V            
        }
#endif        
#if _EXT_BOARD>=3
        //if(!PFNf_LIN_KSK&&!PFNf_LIN_STR&&!PFNf_FOT)
        if((PFNf_LIN_BPSS||!PFNf_LIN_KSK)&&(PFNf_LIN_BPSS||!PFNf_LIN_STR)&&!PFNf_FOT)       //+++
        {    
            Pomiar10V();        //interfejs 10V
        }
        else
        {
            PFNf_bEnF10V=0;             //znacznik do aktywacji wejscia 10V              
            PFNf_EnF10V=0;              //znacznik aktywnosci wejscia 10V  
            PFNf_b10V_RT=0;             //znacznik do aktywacji pobudzenia z wejscia 10V                
            PFNf_10V_RT=0;              //znacznik pobudzenia z wejscia 10V 
            PFNf_10VRT=0;               //znacznik pobudzenia z wejscia 10V            
            PFNf_10V_nPCO=0;            //znacznik nowej wartosci PCO z wejscia 10V            
        }
#endif         
        //-----------------------------------------
        LKonfiguracja();					//decyzja o wejsciu do proc. konfiguracji modulow automatyki //kkk        
		MKonfiguracja();					//decyzja o wejsciu do proc. konfiguracji
		TrybPracy();
		RingProc();							//transmisja do/z plytki UNI-02
		RefData();							//aktualizacja danych WE/WY
		DecWsw();							//decyzja o wyswietlaniu parametrow
		DecRes();							//decyzja o wejsciu oczekiwania na transmisje po resecie plytki sterujacej
	}
}
#endif

//--------------------------------------------------------------
#define _TORES1 15
#define _TORES2 120
void InitUNIData(void)
{
	volatile static unsigned char n=0,k=3,j=3;

//-----------OCZEKIWANIE
    StartRTS(_RTNODB1);
    StartRTS(_RTNODB2);
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
    PFNf_fsetLTrb=0;           //tryb nadawania adresu (0-potwierdzany przyciskiem, 1-automatyczny) //nnn
    PFNf_fIgnore=0;            //ignoruj wszystkie komendy nadawania adresu //nnn
    PFNf_fProcess=0;        //nnn
    PFNf_fOne=0;               //ignoruj wszystkie adresy inne niz 1 //nnn      
    RSDT1f_RD_STD=0;    
    RSDT1f_RRD_STD=0;    
    RSDT1f_RD_KNF=0;    
    RSDT1f_RRD_KNF=0;    
    RSDT1.vSTD=0;
    RSDT1.vKNF=0;   
    PFN.vUNI02=0;
    PFN.ALIN=0; //ustaw wstepnie adres niedozwolony LIN - blokada odbioru do czasu nawiazania komunikacji z UNI-02
    PFN.STRB=0; //ustaw wstepnie specjalny tryb pracy na 0
    PFN.EXFLG=0;
    PFN.STFLG=0;
    
    PFN.INSETP=0;
    PFN.INSETPP=0;
    
    PFN.IINSETP=0;  //jjj
    PFN.IPCO=0;
    PFN.IPCW=0;
    PFN.MXCO=0;
    
    PFNf_new_IPCO=0;
    PFNf_new_IPCW=0;    
    
    RSDT1.inSTAT2=0;
       
    KFN.flag3=0;  
    KFN.flag4=0;
    KFN.flag5=0;  
    KFN.flag6=0;
    KFN.flag7=0; 
    KFN.flag8=0;
    KFN.flag10=0;   
    KFN.flag13=0;    
    
    KFN.OT_MXCO=0;
    KFN.IOT_MXCO=0;     //fff
    KFN.LN_MXCO=0;
    
	n=0;
#if (_EXT_BOARD>=2)    
    j=3;        //aktywny, cykliczny pobor paczki konfiguracyjnej
#else
    j=0;        //nieaktywny, pobor paczki konfiguracyjnej
#endif
    k=3;
	PrintVerBoards(1);	//reset procedury wizualizacji wersji
    //Oczekiwanie na ustalony stan plyty sterujacej, okreslona wersje oprogramowania plyty sterujacej i zaktualizowane dane STD
    //gdzie n - wymusza okeslona liczbe wykonan i pozwala na podtrzymanie petli na czas aktywnosci procedury kalibracji
    //gdzie k,j - wymusza odbior ustalonej, minimalnej liczby ramek STD i KNF
	while(!PFN.vUNI02||(!PFNf_opgres&&!PFNf_opgprg)||(PFNf_opgres&&!PFNf_ferr)||(n++<10)||k||j)  
	{
        /*
        //TEST
        if(RdPrt(S_KUP))
        {
            while(1);           //wymuszenie resetu plytki wyswietlacza
        }    
        */      
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
                PFNf_LIN_UP=0; 
                PFNf_LIN_BPSS=0;      //+++
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
            if(RTS(_RTNODB1)>_TORES1)                     //procedura restartu plytki po oczekiwaniu na komunikacje $$$
            {
                asm("RESET");
            }
		}
		MGSDf_TransACK=0;
        StartRTS(_RTNODB1);
        if(RTS(_RTNODB2)>_TORES2)                         //procedura restartu plytki po oczekiwaniu na komunikacje $$$
        {
            asm("RESET");
        }        
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
            PFNf_kalVNT=GetBit2(RSDT1.outSTAT2,_KALIB);

            PFN.RDZ0=RSDT1.outRDZ0;							//nr wesji oprogramowania plytki UNI-02
            PFN.RDZ1=RSDT1.outRDZ1;
            PFN.RDZ2=RSDT1.outRDZ2;

            PFNf_EnAleg=((PFN.RDZ1&0x0F)==_UNICO);					//znacznik kotla zasobnikowego (1F)
            PFN.vUNI02=((PFN.RDZ1&0xF0)>>4)+(PFN.RDZ2&0b00011111);    //wersja opr.UNI-02
            if(PFN.vUNI02)
            {
                if(PFN.vUNI02>2&&PFN.vUNI02<=17) 
                {
                    PFN.ALIN=(PFN.RDZ0&0b11100000)>>5;
                }
                else
                {
                    if(PFN.vUNI02>=18) 
                    {
                        PFN.ALIN=(PFN.RDZ0&0b11100000)>>5;
                        PFN.ALIN=PFN.ALIN|((PFN.RDZ2&0b00100000)>>2);
                    }
                    else PFN.ALIN=5;  
                }    
            }
            else
            {
                PFN.ALIN=5;
            }
            RSDT1f_RRD_STD=0;		//aktywna blokada na czas interpretacji danych
        }
        //informacja o stanie przyciskow dla plytki sterujacej
        if(!k&&!j&&!PFNf_opgres)
        {
#if _KEY_BBOARD==0           
            if(!k&&!j)
            {
//                BitSet3(&RSDT1.inSTAT1,_KSET,RdPrt(S_SET));				//detekcja przyciskow wejscia do konfiguracji
//                BitSet3(&RSDT1.inSTAT1,_KOFF,RdPrt(S_OFF));
//                BitSet3(&RSDT1.inSTAT1,_KMINUS,RdPrt(S_KDW));           //detekcja przycisku na potrzeby wyjscia z proc.kalibracji
//                BitSet3(&RSDT1.inSTAT1,_KPLUS,RdPrt(S_KUP));  
                BitSet3(&RSDT1.inSTAT1,_KSET,RdPrt(S_BAR));				//detekcja przyciskow wejscia do konfiguracji
                BitSet3(&RSDT1.inSTAT1,_KOFF,RdPrt(S_RES));
                BitSet3(&RSDT1.inSTAT1,_KMINUS,0);           //detekcja przycisku na potrzeby wyjscia z proc.kalibracji
                BitSet3(&RSDT1.inSTAT1,_KPLUS,0);                 
            }
            else
            {
                BitSet3(&RSDT1.inSTAT1,_KSET,0);				
                BitSet3(&RSDT1.inSTAT1,_KOFF,0);
                BitSet3(&RSDT1.inSTAT1,_KMINUS,0);           
                BitSet3(&RSDT1.inSTAT1,_KPLUS,0);                
            }
            BitSet3(&RSDT1.inSTAT1,_7KEY,0);                        //znacznik interfejsu 7Key 
            BitSet3(&RSDT1.inSTAT1,_POT,1);                        //znacznik interfejsu z potencjometrami            
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
            BitSet3(&RSDT1.inSTAT1,_POT,1);                        //znacznik interfejsu z potencjometrami            
#endif            
#if _EXT_BOARD!=0
            if(!k&&!j&&PFNf_FLIN)
            {
                RSDT1.inSTAT2=PFN.STRB;                             //specjalny tryb produkcyjny
            }
            else
            {
                RSDT1.inSTAT2=0;                
            }
#else
            RSDT1.inSTAT2=0;
#endif            
            if(PFN.vUNI02&&PFNf_kalVNT)                             //podtrzymanie petli na czas aktywnosci procedury kalibracji
            {
                n=8;												//(!)odczekaj az zakonczy sie tryb kalibracji
                PrintVerBoards(0);									//procedra wizualizacji wersji oprogramowania plytek
            }										
            else 
            {
                Mark(_GZ_MXCW,0);                                   //znacznik oczekiwania na polaczenie
            }
        }
        else
        {
            Mark(_GZ_MXCW,0);                                       //znacznik oczekiwania na polaczenie
        }
	}
	MGSDf_TransACK=0;
    
    PFN.minPCO=RSDT1.outPCOmn;
    PFN.maxPCO=RSDT1.outPCOmx;
    PFN.minPCW=RSDT1.outPCWmn;
    PFN.maxPCW=RSDT1.outPCWmx;    
    n=0;
    while(n++<250)
    {
        CLRWDT();
        PomiarPCO();
        PomiarPCW();        
    }
//    PFN.PCO=DtCO.PCO;
//    PFN.PCW=DtCW.PCW;
    DtCO._newPCO=0;
    DtCO._newPOZ=0;
    DtCO._newZKR=0; 
    DtCW._newPCW=0;
    DtCW._newPOZ=0;
    DtCW._newZKR=0;   
    PFNf_no_wrtPCO=0;
    PFNf_no_wrtPCW=0;
    PFNf_no_wrtPCO2=1;          //zzz
    PFNf_no_wrtPCW2=1;          //zzz
//--------------------------------------------------------------    
//-----------INICJALIZACJA NASTAW WYSWIETLACZA DANYMI Z PLYTKI STERUJACEJ    
    PFNf_fnserw=GetBit2(RSDT1.outSTAT0,_FKSRV);
    PFNf_aleg=GetBit2(RSDT1.outSTAT0,_ANTYL);
    PFNf_ofzima=GetBit2(RSDT1.outSTAT2,_F_ZIMA);
    PFNf_oflato=GetBit2(RSDT1.outSTAT2,_F_LATO);
    PFNf_ofoff=GetBit2(RSDT1.outSTAT2,_F_OFF);      
    PFNf_off=PFNf_ofoff;
    PFNf_lato=PFNf_oflato;
    PFNf_zima=PFNf_ofzima;
    PFNf_kalVNT=GetBit2(RSDT1.outSTAT2,_KALIB);
    PFNf_pgd=GetBit2(RSDT1.outSTAT1,_PGD);
    
    PFN.RDZ0=RSDT1.outRDZ0;
    PFN.RDZ1=RSDT1.outRDZ1;
    PFN.RDZ2=RSDT1.outRDZ2; 
    PFN.PCW=(unsigned int)RSDT1.outPCW;
    PFN.PCO=(unsigned int)RSDT1.outPCO;
    PFN.ECO=(unsigned int)RSDT1.outECO;

    PFN.ALIN=(PFN.RDZ0&0b11100000)>>5;
    PFNf_EnAleg=((PFN.RDZ1&0x0F)==_UNICO);              //znacznik kotla zasobnikowego (1F)
    PFN.vUNI02=((PFN.RDZ1&0xF0)>>4)+(PFN.RDZ2&0b00011111);                      //wersja opr.UNI-02
//    if(PFN.vUNI02>2) PFN.ALIN=(PFN.RDZ0&0b11100000)>>5; //adres LIN
//    else PFN.ALIN=5;
    if(PFN.vUNI02)
    {
        if(PFN.vUNI02>2&&PFN.vUNI02<=17) 
        {
            PFN.ALIN=(PFN.RDZ0&0b11100000)>>5;
        }
        else
        {
            if(PFN.vUNI02>=18) 
            {
                PFN.ALIN=(PFN.RDZ0&0b11100000)>>5;
                PFN.ALIN=PFN.ALIN|((PFN.RDZ2&0b00100000)>>2);
            }
            else PFN.ALIN=5;  
        }    
    }
    else
    {
        PFN.ALIN=5;
    }  
    PFNf_newPCO=0;
    PFNf_newPCW=0;
    PFNf_newECO=0;
    PFNf_hi_nco=0;
    PFNf_hi_ncw=0;    
    PFNf_EnLIN=1;
    PFNf_prn_res=0;
    PFNf_prn_bar=0;    
    PFNf_res=0;	
    PFNf_FEXT=(PFNf_FLIN||PFNf_FOT)?1:0;
    PFNf_bFEXT=PFNf_FEXT;
//--------------------------------------------------------------    
//-----------PIERWSZA AKTUALIZACJA DANYCH DLA PLYTKI STERUJACEJ
    RefData();                  //aktualizacja danych
//--------------------------------------------------------------    
//-----------ZAKONCZENIE PROCEDURY INICJALIZACJI 
    BitSet3(&RSDT1.inSTAT0,_INITDTA,0);	//deaktywacja trybu INICJALIZACJI    
    Mark(_GZ_OFF,0);
    PFNf_toInit=0;
    PFNf_LINres=0;
//--------------------------------------------------------------    
//-----------PRZYGOROWANIE KLAWIATURY DO PRACY    
#if _KEY_BBOARD==0    
//    ResBufKeyEx(&ky[SS_SET]);
//    ResBufKeyEx(&ky[SS_OFF]);
//    ResBufKeyEx(&ky[SS_KUP]);
//    ResBufKeyEx(&ky[SS_KDW]);
    
    ResBufKeyEx(&ky[SS_RES]);
    ResBufKeyEx(&ky[SS_BAR]);
    ResBufKeyEx(&ky[SS_BAR_RES]);    
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
void RefNstTrb(unsigned char nst, unsigned char trb)
{
    if(nst)
    {
        //nastawy
        if(DtCO.PCO!=PFN.PCO)
        {
            if(!DtCO._newZKR)
            {
                DtCO._newPCO=1;    
            }
        }
        if(DtCW.PCW!=PFN.PCW)
        {
            if(!DtCW._newZKR)
            {
                DtCW._newPCW=1; 
            }
        }           
    }
    else
    {
        DtCO._newPCO=0;
        DtCW._newPCW=0;
    }
    if(trb)
    {
        //tryb pracy
        if(DtCW._pozZRO&&DtCO._pozZRO)
        {
            if(!PFNf_ofoff)
            {
                DtCO._newPOZ=1;
                DtCW._newPOZ=1;
            }
        }
        else
        {
            if(!DtCO._pozZRO)
            {
                if(!PFNf_ofzima) 
                {
                    DtCO._newPOZ=1;
                    DtCW._newPOZ=1;                    
                }
            }
            else
            {
                if(!PFNf_oflato) 
                {
                    DtCO._newPOZ=1;
                    DtCW._newPOZ=1;                    
                }                    
            }
        }        
    }
    else
    {
        DtCO._newPOZ=0;
        DtCW._newPOZ=0;        
    }
}
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//Obsluga nastaw i trybu pracy po zaniku zasilania
void InitNST(void)
{
    if(!PFNf_toInit)
    {
        ClrDig(1,1,0);				//wyczysc ekran
        Mark(_GZ_OFF,2);			//zapal migajacy symbol OFF
    }

    PFN.minPCO=RSDT1.outPCOmn;
	PFN.maxPCO=RSDT1.outPCOmx;    
    PomiarPCO();
    PFN.minPCW=RSDT1.outPCWmn;
    PFN.maxPCW=RSDT1.outPCWmx;    
    PomiarPCW();    
    
    DtCO._newPCO=0;
    DtCW._newPCW=0;
    DtCO._newPOZ=0;
    DtCW._newPOZ=0;
    
    if((!DtCO._stabPCO||!DtCW._stabPCW)) return;
    if(PFN.STRB!=2)     //aktywny tryb produkcyjny?
    {
        //if(RTS(_RFNST)<8) return;
        if(!PFNf_FLIN&&!PFNf_FOT)
        {
            if(RTS(_RFNST)<15) return;
        }
    }
    PFNf_toInitNST=0;
    
    PFN.PCW=(unsigned int)RSDT1.outPCW;   
    PFN.PCO=(unsigned int)RSDT1.outPCO;
    PFNf_ofzima=GetBit2(RSDT1.outSTAT2,_F_ZIMA);
    PFNf_oflato=GetBit2(RSDT1.outSTAT2,_F_LATO);
    PFNf_ofoff=GetBit2(RSDT1.outSTAT2,_F_OFF);    
    if((!PFNf_FLIN&&!PFNf_FOT)||(PFN.STRB==2))   //oba interfejsy nie sa aktywne lub aktywny tryb produkcyjny?
    {
        RefNstTrb(1,1);         //aktualizuj nastawy i tryb pracy
    }
    else
    {
        if(PFNf_FOT)            //aktywny interfejs OT?
        {
            RefNstTrb(0,1);     //aktualizuj tryb pracy
        }
    }
}

//void InitNST(void)
//{
//    volatile unsigned char war=0;
//
//    PomiarPCO();
//    PomiarPCW();    
// 
//    war=(!PFNf_FLIN&&!PFNf_FOT&&!DtCO._newPCO&&!DtCO._newPOZ&&!DtCW._newPCW&&!DtCW._newPOZ)?1:0; //nieaktywne interfejsy?
//    //if(RTS(_RFNST)<_TFNST&&war) return;
//    //if(RTS(_RFNST)<_TFNST+10) return;
//    if(RTS(_RFNST)<_TFNST&&war) return;
//    if(RTS(_RFNST)<(_TFNST*3)&&(!DtCO._stabPCO||!DtCW._stabPCW)) return;
//    PFNf_toInitNST=0;
//    if(PFNf_FLIN||PFNf_FOT)
//    {
//        PFNf_hi_nco=1;
//        PFNf_hi_ncw=1;
//    }
//    if((PFN.STRB==2)||war) //odbior produkcyjny lub nieaktywne interfejsy?
//    {
//        SetNST();    
//        SetTrbPRC();       
//    }
//    else
//    {
//        if(PFNf_FOT)        //aktywny interfejs OT?
//        {
//            SetTrbPRC();                
//        }
//    }
//}
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
void ClrNewSTAT(void)
{
    KFN._newSTAT=0;
}
void ClrNewENPNZ(void)
{
    KFN._newENPNZ=0;
}
void ClrNewGPOB(void)
{
    KFN._newGPOB=0;
}
void ClrNewSPOB(void)
{
    KFN._newSPOB=0;
}
void ClrNewPPOB(void)
{
    KFN._newPPOB=0;
}
void TimeoutProc(void)
{         
    if(RTS(_RTPLIN)>_TPLIN)    //magistrala LIN2 nie jest aktywna?
    {
        KFN.flag3=0;             //zeruj zdalny status
        KFN.flag4=0;             //zeruj dane o aktywnosci pionow
        KFN.flag5=0;             //zeruj zdalne pobudzenia posesji  
        KFN.flag13=0;             //zeruj zdalne pobudzenia posesji 
        PFNf_LIN_UP=0; 
        PFNf_LIN_BPSS=0;      //+++        
        ClrNewSTAT();
        ClrNewENPNZ();        
        ClrNewGPOB();
        CMSGDta.REF1=0;
    }

    if(RTS(_RTPLIN2)>_TPLIN)    //magistrala LIN2 nie jest aktywna?
    {
        KFN.flag6=0;            //zeruj zdalne pobudzenia stref  
        KFN.flag7=0;            //zeruj zdalne pobudzenia pionow
        KFN.LN_MXCO=0;
        ClrNewSPOB();
        ClrNewPPOB();
        CMSGDta.REF2=0;
    } 
}
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
unsigned char FormatSETP(volatile unsigned char dt)
{
    if(dt<PFN.SETPmin) dt=PFN.SETPmin;
    if(dt>PFN.SETPmax) dt=PFN.SETPmax;        
    return dt;    
}
unsigned char FormatSETPP(volatile unsigned char dt)
{
    if(dt<PFN.SETPmin) dt=PFN.SETPmin;
    if(dt>PFN.SETPPmax) dt=PFN.SETPPmax;        
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
#define _MRG_SETP 10     //margines w obrebie ktorego zmiana SETP jest powolna +++
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
    //----------------------
    //margines w obrebie ktorego zmiana SETP jest powolna
    if((newSETP-SETP)>_MRG_SETP)    //+++
    {
        SETP=newSETP-_MRG_SETP;
    }
    if((SETP-newSETP)>_MRG_SETP)    //+++
    {
        SETP=newSETP+_MRG_SETP;
    }    
    //----------------------
    //powolna zmiana SETP   
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
        PFN.SETPPmax=RSDT1.outSETPPmax;
        PFN.PRZ=RSDT1.outPRZ;
        
		PFNf_EnAleg=((PFN.RDZ1&0x0F)==_UNICO); 
	
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
        //------        
        nst=GetBit2(RSDT1.outSTAT1,_PGD);
        if(PFN.vUNI02>=12)      //od wersji 12 wprowadzono niezalezna nastawe dla regulatora pogodowego 
        {
            if(PFNf_pgd!=nst)   //zalaczono/wylaczono regulator pogodowy?
            {
                PFN.PCO=(unsigned int)RSDT1.outPCO;     //aktualizuj nastawe dana z plytki sterujacej
            }
        }
		PFNf_pgd=(nst)?1:0;
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
		PFNf_kalVNT=GetBit2(RSDT1.outSTAT2,_KALIB);
 
		PFNf_zcfm=GetBit2(RSDT1.outSTAT3,_ZCFM);
		PFNf_vnt=GetBit2(RSDT1.outSTAT3,_VNT_ON);
		PFNf_rozruch=GetBit2(RSDT1.outSTAT3,_ROZ);
		PFNf_errIN=GetBit2(RSDT1.outSTAT3,_ErrIN);
		PFNf_pmpPWM=GetBit2(RSDT1.outSTAT3,_PMP_PWM);
		PFNf_trbECO=GetBit2(RSDT1.outSTAT3,_TRB_ECO);
		PFNf_timZAS=GetBit2(RSDT1.outSTAT3,_TIM_ZAS);   //znacznik pobudzenia CW/stan na wejsciu timera zasobnika
        PFNf_pobRT=GetBit2(RSDT1.outSTAT3,_POB_RT);     //znacznik pobudzenia RT
        
        PFNf_prODP=GetBit2(RSDT1.outSTAT4,_PR_ODP);     //znacznik aktywnosci procedury odpowietrzajacej
        PFNf_popFN=GetBit2(RSDT1.outSTAT4,_POP_FN);     //poprzedni stan przelacznika funkcji (1-lato, 0-pozostale)
        PFNf_tmpZW=GetBit2(RSDT1.outSTAT4,_TMP_ZW);     //aktywnosc czujnika temperatury zewnetrznej
        PFNf_pgdTP=GetBit2(RSDT1.outSTAT4,_PGD_TP);     //rodzaj wewnetrznego regulatora pogodowego        
        PFNf_pmpON=GetBit2(RSDT1.outSTAT4,_PMP_ON);     //rodzaj wewnetrznego regulatora pogodowego

        PFNf_stkRT=GetBit2(RSDT1.outSTAT5,_STK_RT2);    //stan styku RT
        PFNf_pobPGRT=GetBit2(RSDT1.outSTAT5,_POB_PGRT); //pobudzenie RT od regulatora pogodowego        
        PFNf_fzaREG=GetBit2(RSDT1.outSTAT5,_FZA_REG);   //faza stabilizacji regulatora        
        PFNf_fenPWD=GetBit2(RSDT1.outSTAT5,_FEN_PWD);   //gotowosc do przewyzszenia temperaturowego        
		//poprawka na wersje oprogramowania UNI-02 (kompatybilnosc wsteczna)
		if(PFN.vUNI02<2) PFNf_timZAS=1;
//        if(PFN.vUNI02>2) PFN.ALIN=(PFN.RDZ0&0b11100000)>>5; //adres LIN
//        else PFN.ALIN=5;
        if(PFN.vUNI02)
        {
            if(PFN.vUNI02>2&&PFN.vUNI02<=17) 
            {
                PFN.ALIN=(PFN.RDZ0&0b11100000)>>5;
            }
            else
            {
                if(PFN.vUNI02>=18) 
                {
                    PFN.ALIN=(PFN.RDZ0&0b11100000)>>5;
                    PFN.ALIN=PFN.ALIN|((PFN.RDZ2&0b00100000)>>2);
                }
                else PFN.ALIN=5;  
            }    
        }
        else
        {
            PFN.ALIN=5;
        }
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
    //---------------------------
    PFN.EXFLG=0;
    if(RSDT1.vKNF>=11)
    {
        PFNf_cwIMP=GetBit2(RSDT1.outKSTAT0,_CWIMP);     //flaga obecnosci impulsowego czujnika przeplywu CW
        PFNf_PWD=GetBit2(RSDT1.outKSTAT0,_PWD);         //znacznik trybu pracy ze stabilizacja temperatury
        PFNf_BUF=GetBit2(RSDT1.outKSTAT0,_BUF);         //znacznik trybu pracy na bufor ciepla
        PFNf_cwUP=GetBit2(RSDT1.outKSTAT0,_UPCW);       //znacznik aktywnej pracy na wysokich parametrach
        PFNf_BPSS=GetBit2(RSDT1.outKSTAT0,_BPSS);       //znacznik bypass dla pobudzenia RT //+++
        PFNf_outRT=GetBit2(RSDT1.outKSTAT0,_OUT_RT);    //znacznik pobudzenia RT generowanego z plytki sterujacej //+++

        PFN._enEXFL=1;      //ustaw znacznik aktywnosci EXFLG        
        if(PFNf_PWD)
        {
            PFN._FPWD=1;    //znacznik trybu pracy ze stabilizacja temperatury (sygnal zwrotny do sterownika zewnetrznego) 
        }
        else
        {
            PFN._FPWD=0;
        }
        if(PFNf_BUF)
        {
            PFN._FBUF=1;    //znacznik trybu pracy na bufor ciepla (sygnal zwrotny do sterownika zewnetrznego)
        }
        else
        {
            PFN._FBUF=0;
        }     
        if(PFNf_cwUP)
        {
            PFN._FUP=1;    //znacznik aktywnej pracy na wysokich parametrach (sygnal zwrotny do sterownika zewnetrznego)
        }
        else
        {
            PFN._FUP=0;
        } 
        if(PFNf_BPSS)   //+++
        {
            PFN._FBPSS=1;    //znacznik bypass dla pobudzenia RT //+++
        }
        else
        {
            PFN._FBPSS=0;   //+++
        }   
        if(PFNf_outRT)  //+++
        {
            PFN._FOUTRT=1;    //znacznik pobudzenia RT generowanego przez plytke sterujaca //+++
        }
        else
        {
            PFN._FOUTRT=0;  //+++
        }         
    }
    else
    {
        PFNf_cwIMP=0;
        PFNf_PWD=0;
        PFNf_BUF=0;
        PFNf_cwUP=0;
        PFNf_BPSS=0;    //+++
        PFNf_outRT=0;   //+++        
    }
   //---------------------------    
    //status specjalny
    PFN.STFLG=0;
    PFN._enSTFL=1;      //ustaw znacznik aktywnosci STFLG    
    if(PFNf_PWD)
    {
        PFN._FFPWD=1;    //znacznik trybu pracy ze stabilizacja temperatury (sygnal zwrotny do sterownika zewnetrznego) 
    }
    else
    {
        PFN._FFPWD=0;
    }
    if(PFNf_BUF)
    {
        PFN._FFBUF=1;    //znacznik trybu pracy na bufor ciepla (sygnal zwrotny do sterownika zewnetrznego)
    }
    else
    {
        PFN._FFBUF=0;
    }     
    if(PFNf_cwUP)
    {
        PFN._FFUP=1;    //znacznik aktywnej pracy na wysokich parametrach (sygnal zwrotny do sterownika zewnetrznego)
    }
    else
    {
        PFN._FFUP=0;
    }  
    if(PFNf_ofzima)
    {
        PFN._FTRB=2;    //tryb pracy ZIMA
    }
    else
    if(PFNf_oflato)
    {
        PFN._FTRB=1;    //tryb pracy LATO
    } 
    else
    {
        PFN._FTRB=0;    //tryb pracy OFF    
    }
    if(PFNf_EnAleg)
    {
        PFN._FRDZ=0;    //kociol 1F
    }
    else
    {
        PFN._FRDZ=1;    //kociol 2F       
    }    
    //---------------------------    
//write
//STANDARD
    if(!PFNf_no_wrtPCO) //utrzymuje sie stala wartosc PCO?
    {
        RSDT1.inPCO=(unsigned char)PFN.PCO;
    }
    if(!PFNf_no_wrtPCW) //utrzymuje sie stala wartosc PCW?
    {
        RSDT1.inPCW=(unsigned char)PFN.PCW;
    }
    RSDT1.inECO=(unsigned char)PFN.ECO;

	BitSet3(&RSDT1.inSTAT0,_INITKNF,PFNf_aktywne_KNF);

	BitSet3(&RSDT1.inSTAT0,_INITSRV,PFNf_fnserw&&!PFNf_ofnserw);
	BitSet3(&RSDT1.inSTAT0,_INITANL,PFNf_aleg&&!PFNf_oaleg);
    if(!PFNf_toInitNST)
    {
        BitSet3(&RSDT1.inSTAT0,_PFN_ZIMA,PFNf_zima&&!PFNf_off);
        BitSet3(&RSDT1.inSTAT0,_PFN_LATO,PFNf_lato&&!PFNf_off);
        BitSet3(&RSDT1.inSTAT0,_PFN_OFF,PFNf_off);
    }
    else
    {
        BitSet3(&RSDT1.inSTAT0,_PFN_ZIMA,PFNf_ofzima&&!PFNf_ofoff);
        BitSet3(&RSDT1.inSTAT0,_PFN_LATO,PFNf_oflato&&!PFNf_ofoff);
        BitSet3(&RSDT1.inSTAT0,_PFN_OFF,PFNf_ofoff);        
    }
	BitSet3(&RSDT1.inSTAT0,_PFN_RES,(PFNf_res||PFNf_LINresSR||PFNf_LINresAL));

	BitSet3(&RSDT1.inSTAT1,_SRVMAX,PFNf_fns_max);
    
#if _KEY_BBOARD==0    
//	BitSet3(&RSDT1.inSTAT1,_KPLUS,RdPrt(S_KUP));
//	BitSet3(&RSDT1.inSTAT1,_KMINUS,RdPrt(S_KDW));
//	BitSet3(&RSDT1.inSTAT1,_KSET,RdPrt(S_SET));
//	BitSet3(&RSDT1.inSTAT1,_KOFF,RdPrt(S_OFF));
    if(!PFNf_prODP)     //aktywna procedura odpowietrzajaca?
    {
        BitSet3(&RSDT1.inSTAT1,_KPLUS,0);
        BitSet3(&RSDT1.inSTAT1,_KMINUS,0);
    }
    else
    {
        if(RdPrt(S_BAR)&&RdPrt(S_RES))      //nacisnieto jednoczesnie BAR i RESET?
        {
            BitSet3(&RSDT1.inSTAT1,_KPLUS,1);       //symulacja nacisniecia przycisku PLUS
            BitSet3(&RSDT1.inSTAT1,_KMINUS,1);      //symulacja nacisniecia przycisku MINUS            
        }
        else
        {
            BitSet3(&RSDT1.inSTAT1,_KPLUS,0);
            BitSet3(&RSDT1.inSTAT1,_KMINUS,0);               
        }
    }
	BitSet3(&RSDT1.inSTAT1,_KSET,RdPrt(S_BAR));
	BitSet3(&RSDT1.inSTAT1,_KOFF,RdPrt(S_RES));    
    BitSet3(&RSDT1.inSTAT1,_7KEY,0);                        //znacznik interfejsu 7Key  
    BitSet3(&RSDT1.inSTAT1,_POT,1);                        //znacznik interfejsu z potencjometrami    
#endif
#if _KEY_BBOARD==1    
	BitSet3(&RSDT1.inSTAT1,_KPLUS,RdPrt(S_KUP_CO));
	BitSet3(&RSDT1.inSTAT1,_KMINUS,RdPrt(S_KDW_CO));
	BitSet3(&RSDT1.inSTAT1,_KSET,RdPrt(S_KOM));
	BitSet3(&RSDT1.inSTAT1,_KOFF,RdPrt(S_OFF));
    BitSet3(&RSDT1.inSTAT1,_7KEY,1);                        //znacznik interfejsu 7Key 
    BitSet3(&RSDT1.inSTAT1,_POT,1);                        //znacznik interfejsu z potencjometrami    
#endif  
    
#if _EXT_BOARD!=0
    BitSet3(&RSDT1.inINSTAT,_F10V,PFNf_EnF10V);      //bit aktywnosci interfejsu 10V
    if(PFNf_10V_RT&&PFNf_EnF10V)
    {
        PFNf_10VRT=1;
    }
    else
    {
        PFNf_10VRT=0;        
    }    
    if(!PFNf_LIN_BUF)   
    {
        BitSet3(&RSDT1.inINSTAT,_10V_RT,PFNf_10VRT);    //zdalne pobudzenie RT z interfejsu 10V
    }
    else    //aktywna praca na bufor ciepla
    {
        BitSet3(&RSDT1.inINSTAT,_10V_RT,0);    //zdalne pobudzenie RT z interfejsu 10V        
    }
    if(!PFNf_EnF10V&&!PFNf_LIN_KSK&&!PFNf_LIN_STR&&!PFNf_LIN_PWD&&!PFNf_LIN_BUF&&!(OTMaster.Ref._Control_Setpoint&&PFNf_FOT))   //jjj    
    {
        PFN.CONTSETP=PFN.SETPmax;
        RSDT1.inCONTSETP=PFN.CONTSETP;
        PFN.INSETP=PFN.CONTSETP;
        PFN.IINSETP=PFN.CONTSETP;   //jjj        
    }
    else //dowolny z interfejsow aktywny
    {
        if(!(OTMaster.Ref._Control_Setpoint&&PFNf_FOT))         //interfejs OT nie jest aktywny?
        {
            PFN.IINSETP=PFN.INSETP;
        }        
        if((PFNf_LIN_KSK||PFNf_LIN_STR||PFNf_LIN_PWD||PFNf_LIN_BUF||PFNf_LIN_BPSS)&&PFNf_FLIN)             //interfejs LIN w trybie STR lub KSK?    //jjj
        {
            if(!PFNf_LIN_PWD)       //praca bez sprzegla?
            {
                PFN.CONTSETP=FormatSETP(PFN.INSETP);                //natychmiastowa aktualizacja SETP wartoscia oczekiwana z interfejsu
                //(!) UWAGA: Przy aktywnym BPSS i nieaktywnym PWD modul automatyki pobiera wartosc oczekiwana z zapytania _CLIN_RRD_SETPP
            }
            else                    //praca ze sprzeglem
            {
                PFN.CONTSETP=FormatSETPP(PFN.INSETPP);               //natychmiastowa aktualizacja SETP wartoscia oczekiwana za sprzeglem pozyskana z interfejsu LIN (CMSGPar.PAR1)
                if(!PFNf_EnF10V&&!(OTMaster.Ref._Control_Setpoint&&PFNf_FOT)) //interfejsy 10V i OT nie sa aktywne  //jjj
                {
                    PFN.INSETP=PFN.SETPmax;                             //wartosc nastawy dla przewyzszenia rowna sie nastawie manualnej przy braku aktywnych interfejsow
                }
                else
                {
                    if(PFNf_LIN_BPSS)                                   //aktywny bypass?
                    {
                        PFN.INSETP=PFN.IINSETP;                         //wartosc nastawy dla przewyzszenia rowna sie wartosci oczekiwanej z aktywnego interfejsu
                    }
                    else
                    {
                        PFN.INSETP=PFN.SETPmax;
                    }
                }
                //(!) UWAGA: Przy aktywnym PWD modul automatyki pobiera wartosc oczekiwana z zapytania _CLIN_RRD_SETP
            }
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
        PFNf_LIN_PWD=0;
        PFNf_LIN_BUF=0;        
        PFNf_LIN_DPGD=0;
    }
    BitSet3(&RSDT1.inSTAT1,_END_ODP,(PFNf_prODP&&PFNf_LIN_ENDO&&PFNf_FLIN));     //deaktywacja procedury odpowietrzajacej
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
    BitSet3(&RSDT1.inLNSTAT,_LN_PWD,(PFNf_LIN_PWD&&PFNf_FLIN));                  //znacznik gotowosci dla przewyzszenia 
    BitSet3(&RSDT1.inLNSTAT,_LN_BUF,(PFNf_LIN_BUF&&PFNf_FLIN));                  //znacznik gotowosci dla bufora ciepla 
    
#if _TEST_UP==0 && _TEST_BPSS==0
    BitSet3(&RSDT1.inLNSTAT2,_LN_UP,(PFNf_LIN_UP&&PFNf_FLIN));                  //pobudzenie grzania na wysokich parametrach //+++
    BitSet3(&RSDT1.inLNSTAT2,_LN_BPSS,(PFNf_LIN_BPSS&&PFNf_FLIN));              //bypass dla pobudzenia RT    //+++
#else
    BitSet3(&RSDT1.inLNSTAT,_FLIN,1);        //odswiez znacznik komunikacji LIN dla ukladu UNI-02
    StartRTdS(_RTFLIN);
    PFNf_FLIN=1;    
    BitSet3(&RSDT1.inLNSTAT2,_LN_UP,1);                  //pobudzenie grzania na wysokich parametrach //+++
    BitSet3(&RSDT1.inLNSTAT2,_LN_BPSS,1);                //bypass dla pobudzenia RT   //+++
#endif 
    if(!PFNf_FLIN) PFN.STRB=0;
#else
    BitSet3(&RSDT1.inLNSTAT,_LN_RT,0);                //zdalne pobudzenie RT  
    BitSet3(&RSDT1.inLNSTAT,_LN_CW,0);                //zdalne pobudzenie timera zasobnika
    BitSet3(&RSDT1.inLNSTAT,_LN_KSK,0);               //znacznik pracy w trybie kaskady  
    BitSet3(&RSDT1.inLNSTAT,_LN_STR,0);               //znacznik pracy w trybie sterowania z managera strefy zlub regulatora RT
    BitSet3(&RSDT1.inLNSTAT,_LN_DPGD,0);              //znacznik dezaktywacji wewnetrznego regulatora pogodowego
    BitSet3(&RSDT1.inLNSTAT,_LN_PWD,0);               //dezaktywacja gotowosci dla przewyzszenia  
    BitSet3(&RSDT1.inLNSTAT,_LN_BUF,0);               //dezaktywacja gotowosci dla bufora ciepla    
    BitSet3(&RSDT1.inSTAT1,_END_ODP,0);               //dezaktywacja procedury odpowietrzajacej   
    BitSet3(&RSDT1.inLNSTAT2,_LN_UP,0);               //pobudzenie grzania na wysokich parametrach //+++
    BitSet3(&RSDT1.inLNSTAT2,_LN_BPSS,0);             //bypass dla pobudzenia RT  //+++    
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
            if(OTMaster.Status._CH_enable&&PFNf_FOT)
            {
                PFNf_OTRT=1;
            }
            else
            {
                PFNf_OTRT=0;        
            }             
            if(!PFNf_LIN_BUF)
            {
                BitSet3(&RSDT1.inINSTAT,_OT_RT,PFNf_OTRT);                //zdalne pobudzenie RT
            }
            else
            {
                BitSet3(&RSDT1.inINSTAT,_OT_RT,0);
            }
        }
        else
        {
            PFNf_OTRT=0;
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
    
    PFNf_ex_pgd=(PFNf_tmpZW&&!PFNf_pgd&&((PFNf_OT_DPGD&&PFNf_FOT)||(PFNf_LIN_DPGD&&PFNf_FLIN)))?1:0; //znacznik aktywnosci zewnetrznego regulatora pogodowego 
    
    PFN.MXCO=0;     //ooo
    if(!PFNf_FOT&&!PFNf_FLIN)       //ooo
    {
        KFN.LN_MXCO=0;
        KFN.OT_MXCO=0;
        KFN.IOT_MXCO=0;       //fff 
    }
    else
    {
        if(PFNf_FLIN)        //ooo
        {    
            if(KFN.LN_MXCO&0b10000000)
            {
                PFN.MXCO=KFN.LN_MXCO;
            }
            else
            {
                KFN.LN_MXCO=0;
                if(PFNf_FOT)        //ooo
                {
                    if(KFN.OT_MXCO&0b10000000)
                    {
                        PFN.MXCO=KFN.OT_MXCO;
                    }
                } 
                else
                {
                    KFN.OT_MXCO=0;
                    KFN.IOT_MXCO=0;     //fff
                }
            }
        }
        else
        {
            KFN.LN_MXCO=0;
            if(PFNf_FOT)        //ooo
            {
                if(KFN.OT_MXCO&0b10000000)
                {
                    PFN.MXCO=KFN.OT_MXCO;
                }
            }
            else
            {
                KFN.OT_MXCO=0;
                KFN.IOT_MXCO=0;        //fff        
            }
        }
    }
    RSDT1.inMXCO=PFN.MXCO;          //ooo
}
//--------------------------------------------------------------
void MarkMinMax(unsigned char shCW,unsigned char shCO)
{
    if(!PFNf_EnAleg)    //mmm
    {
        Mark(_GZ_MNCW,shCW&&(PFN.PCW<=PFN.minPCW));
    }
    else
    {
        Mark(_GZ_MNCW,shCW&&(PFN.PCW==PFN.minPCW));
    }
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
unsigned char ALINtoNR(const unsigned char alin)
{
    switch(alin)
    {
        case 0x01:
            return 1;
        case 0x02:
            return 2;
        case 0x03:
            return 3;
        case 0x04:
            return 4;
        case 0x09:
            return 5;
        case 0x0A:
            return 6;
        case 0x0B:
            return 7;
        case 0x0C:
            return 8;
        default:
            return 0;
    }
}
//--------------------------------------------------------------
unsigned char NRtoALIN(const unsigned char nr)
{
    switch(nr)
    {
        case 1:
            return 0x01;
        case 2:
            return 0x02;
        case 3:
            return 0x03;
        case 4:
            return 0x04;
        case 5:
            return 0x09;
        case 6:
            return 0x0A;
        case 7:
            return 0x0B;
        case 8:
            return 0x0C;
        default:
            return 0x05;
    }
}
//--------------------------------------------------------------
#if _TDEMO==0

void PrintVerBoards(unsigned char ref)
{
    volatile unsigned char nr;
    
	if(ref)
	{
 		M.PVB=0;
		StartRTdS(_RTVER);
		return;
	}
	switch(M.PVB)
	{
		case 0:							
			ClrDig(1,1,0);				//wyczysc ekran
			Mark(_GZ_MXCW,0);			//wygas migajacy symbol max
			Mark(_GZ_OFF,0);			//wygas migajacy symbol OFF
			StartRTdS(_RTVER);
			M.PVB=1;
			break;
		case 1:	//b1						
			if(RTdS(_RTVER)<5) return;
			StartRTdS(_RTVER);
			PrintVER(0xb1,PFN.vUNI02,1);			//wyswietl parametr dla b1 
			M.PVB=2;
		break;
		case 2:							
			if(RTdS(_RTVER)<=15) return;
			StartRTdS(_RTVER);
			ClrDig(1,1,0);				//wyczysc ekran
			M.PVB=3;
		break;
		case 3://b2
			if(RTdS(_RTVER)<5) return;
			StartRTdS(_RTVER);
			PrintVER(0xb2,_WERSJA,1);	//wyswietl paranetr dla b2
			M.PVB=4;
		break;
		case 4:
			if(RTdS(_RTVER)<=15) return;
			StartRTdS(_RTVER);
			ClrDig(1,1,0);				//wyczysc ekran
			M.PVB=5;
		break;
		case 5://1F lub 2F
			if(RTdS(_RTVER)<5) return;
			StartRTdS(_RTVER);
            nr=ALINtoNR(PFN.ALIN);
#if (_EXT_BOARD>=2)            
            if(PFN.vUNI02<=2)
            {
                if(!PFNf_EnAleg) PrintVER(0x2F,0,1);	//wyswietl 2F dla kotla dwufunkcyjnego
                else PrintVER(0x1F,0,1);				//wyswietl 1F dla kotla jednofunkcyjnego
            }
            else
            {
//                if(!PFNf_EnAleg) PrintVER(0x2F,(PFN.ALIN<5)?PFN.ALIN:(-1),1);	//wyswietl 2F dla kotla dwufunkcyjnego
//                else PrintVER(0x1F,(PFN.ALIN<5)?PFN.ALIN:(-1),1);				//wyswietl 1F dla kotla jednofunkcyjnego
                if(!PFNf_EnAleg) PrintVER(0x2F,(nr)? nr:(-1),1);	//wyswietl 2F dla kotla dwufunkcyjnego
                else PrintVER(0x1F,(nr)? nr:(-1),1);				//wyswietl 1F dla kotla jednofunkcyjnego                
            }
#else
            if(PFN.vUNI02<=2)
            {
                if(!PFNf_EnAleg) PrintVER(0x2F,0,1);	//wyswietl 2F dla kotla dwufunkcyjnego
                else PrintVER(0x1F,0,1);				//wyswietl 1F dla kotla jednofunkcyjnego
            }
            else
            {
                if(!PFNf_EnAleg) PrintVER(0x2F,-1,1);	//wyswietl 2F dla kotla dwufunkcyjnego
                else PrintVER(0x1F,-1,1);				//wyswietl 1F dla kotla jednofunkcyjnego
            }            
#endif
			M.PVB=6;
		break;
		case 6:
			if(RTdS(_RTVER)<=15) return;
			StartRTdS(_RTVER);
			ClrDig(1,1,0);				//wyczysc ekran
			M.PVB=7;
		break;
		case 7://zakonczenie procedury
			if(RTdS(_RTVER)<5) return;
			M.PVB=8;
		break;
		case 8:
			Mark(_GZ_MXCW,2);			//zpal migajacy symbol max	
			Mark(_GZ_OFF,2);			//zapal migajacy symbol OFF
			M.PVB=9;
		break;
		case 9:
		break;
	}
}
#if _KEY_BBOARD==0
void NastawaCOCW(void)//unsigned char one)
{
	volatile static unsigned char obg_cw=0,obg_co=0,zmn2=0,eco,inst=0;      //mmm
    signed int bfdta;   //kkk    
    
	if(PFNf_ofnserw||PFNf_set_fnserw||PFNf_oaleg||PFNf_set_aleg) //aktywna funkcja serwisowa lub antylegionella?
	{
		PFNf_set_ncw=0;
		PFNf_set_nco=0;
		PFNf_set_eco=0;
		PFNf_str_nco=0;
        PFNf_newPCW=0;
        PFNf_bnewPCW=0;      
        PFNf_newPCO=0;
        PFNf_bnewPCO=0;        
	}
//-----------------
	if(!PFNf_set_ncw&&!obg_cw&&PFNf_obgCW)	//uruchomienie w obiegu CW?
	{
		PFNf_str_ncw=1;
		PFNf_b_str_ncw=1;
		StartRTdS(_RTSTR);
        
        PFNf_newPCW=0;
        PFNf_bnewPCW=0;      
        PFNf_newPCO=0;
        PFNf_bnewPCO=0;        
        
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
        
        PFNf_newPCW=0;
        PFNf_bnewPCW=0;      
        PFNf_newPCO=0;
        PFNf_bnewPCO=0;
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
//                PrintNstCO(PFN.CONTSETP,0,0);
			}
			else
			{
				if(PFN.PCW<PFN.minPCW) PrintNstCO(PFN.PPCO,0,1);
				else PrintNstCO(PFN.PPCO,0,0);	
                
//                if(PFN.PCW<PFN.minPCW) PrintNstCO(PFN.CONTSETP,0,1);
//				else PrintNstCO(PFN.CONTSETP,0,0);
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
    if(PFNf_off)//||(DtCW._pozZRO&&DtCO._pozZRO))
    {
        PFNf_set_ncw=0;
        PFNf_set_nco=0;
        PFNf_newPCW=0;
        PFNf_bnewPCW=0;      
        PFNf_newPCO=0;
        PFNf_bnewPCO=0;        
        EndNst();
    }  
//-----------------    
    if(PFNf_newPOZ)
    {
        PFNf_newPOZ=0;
        if(!PFNf_fshowLpulse) SetLightLLed(1);	//rozswietlenie maksymalne
    }
//-----------------    
	if(PFNf_set_ncw)	//wybrano nastawe CW?
	{
        PFNf_newPCW=0;
        PFNf_bnewPCW=0;
        if(PFN.PCW!=DtCW.PCW)
        {
            inst=1;            //mmm
            PFN.PCW=DtCW.PCW;
            StartRTdS(_RTSET);
            if(!PFNf_fshowLpulse) SetLightLLed(1);	//rozswietlenie maksymalne
            if(!PFNf_EnAleg)
            {
                PrintNstCW(PFN.PCW,0);	//PrintNstCW(PFN.PCW,1);
            }
            else
            {
                PrintNstCW2(PFN.PCW,PFN.minPCW-1,0);	//PrintNstCW(PFN.PCW,1);
            }            
        }
	}   
    else
    {
        if(!PFNf_off&&!PFNf_fnserw&&!PFNf_aleg&&!PFNf_set_fnserw&&!PFNf_set_aleg&&!PFNf_set_nco&&!PFNf_set_eco&&!PFNf_str_ncw&&!PFNf_str_nco)
        {
            if(PFNf_newPCW)     //aktualizacja wartosci nastawy PCW z interfejsu?
            {
                if(!PFNf_bnewPCW)
                {
                    PFNf_bnewPCW=1;    
                    RestartKol2(_KL_NCWCO);
                    RestartKol(_KL_NCWCO);
                    PFNf_prn_res=0;

                    if(!PFNf_fshowLpulse) SetLightLLed(1);	//rozswietlenie maksymalne
                    if(!PFNf_EnAleg)
                    {
                        PrintNstCW(PFN.PCW,0);	//PrintNstCW(PFN.PCW,1);
                    }
                    else
                    {
                        PrintNstCW2(PFN.PCW,PFN.minPCW-1,0);	//PrintNstCW(PFN.PCW,1);
                    }   
                }
            }  
        }
        else
        {
            PFNf_newPCW=0;
            PFNf_bnewPCW=0;
        }
    }
//-----------------    
	if(PFNf_set_nco)	//wybrano nastawe CW?
	{
        PFNf_newPCO=0;
        PFNf_bnewPCO=0; 
        if(PFN.PCO!=DtCO.PCO)
        {
            inst=1;            //mmm
            PFN.PCO=DtCO.PCO;
            StartRTdS(_RTSET);
            if(!PFNf_fshowLpulse) SetLightLLed(1);	//rozswietlenie maksymalne
            PrintNstCO(PFN.PCO,PFNf_pgd,0);  
        }
	}
    else
    {
        if(!PFNf_off&&!PFNf_fnserw&&!PFNf_aleg&&!PFNf_set_fnserw&&!PFNf_set_aleg&&!PFNf_set_ncw&&!PFNf_set_eco&&!PFNf_str_ncw&&!PFNf_str_nco)
        {        
            if(PFNf_newPCO)     //aktualizacja wartosci nastawy PCO z interfejsu?
            {
                if(!PFNf_bnewPCO)
                {
                    PFNf_bnewPCO=1;   
                    RestartKol2(_KL_NCWCO);
                    RestartKol(_KL_NCWCO);
                    PFNf_prn_res=0;

                    if(!PFNf_fshowLpulse) SetLightLLed(1);	//rozswietlenie maksymalne
                    PrintNstCO(PFN.PCO,PFNf_pgd,0);    
                }
            }
        }
        else
        {
            PFNf_newPCO=0;
            PFNf_bnewPCO=0;            
        }
    }
//-----------------    //mmm
    if(PFNf_set_ncw)    
    {
        if(GetPoz(_KL_NCWCO,0))
        {
            inst=0;
        }
        if(!inst)
        {
            if(GetPoz(_KL_NCWCO,0)) 
            {
                MarkMinMax(1,0);
            }
            else       
            {
                Mark(_GZ_MNCW,0);
                Mark(_GZ_MXCW,0);
            }
        }
    }
    if(PFNf_set_nco)    
    {
        if(GetPoz(_KL_NCWCO,0))
        {
            inst=0;
        }
        if(!inst)
        {        
            if(GetPoz(_KL_NCWCO,0)) 
            {
                MarkMinMax(0,1);
            }
            else       
            {
                Mark(_GZ_MNCO,0);
                Mark(_GZ_MXCO,0);
            }
        }
    }       
//-----------------    
//-----------------
//    if(PFNf_off)
//    {
//        PFNf_set_nco=0;
//        EndNst();
//    }  
//    else
//	if(PFNf_set_nco)	//wybrano nastawe CW?
//	{
//        PFNf_set_nco=0;
//        PFNf_set_ncw=0;
//        PFN.PCO=DtCO.PCO;
//
//        StartRTdS(_RTSET);
//        PrintNstCO(PFN.PCO,PFNf_pgd,0);
//	}     

    
    
//	if(PFNf_set_ncw)	//wybrano nastawe CW?
//	{
//		pcw=PFN.PCW;
//		//na potrzeby parametru ECO przytrzymanie przycisku +/- wstrzymuje nastawe
//		if(!PFNf_mod_ncw&&!RdPrt(S_KUP)&&!RdPrt(S_KDW))
//		{
//			PFNf_mod_ncw=1;
//		}
//		if(!PFNf_EnAleg)
//		{
//			if(PFNf_mod_ncw) ReadParam((unsigned int *)(&PFN.PCW),&zmn,&zps,PFN.minPCW,PFN.maxPCW,0,1);
//		}
//		else
//		{
//			if(PFNf_mod_ncw) ReadParam((unsigned int *)(&PFN.PCW),&zmn,&zps,PFN.minPCW-1,PFN.maxPCW,0,1);
//		}
//		if(!PFNf_b_set_ncw)	//nacisnieto przycisk set?
//		{
//			PFNf_b_set_ncw=1;
//			EndNst();
//			zmn2=1;
//			pcw=PFN.PCW;				
//		}
//		if(zmn2||zmn)				//nastawa ulegla zmianie?
//		{
//			PFNf_newPCW=(pcw!=PFN.PCW);
//			zmn2=0;
//			StartRTdS(_RTSET);
//			if(!PFNf_EnAleg)
//			{
//				PrintNstCW(PFN.PCW,0);	//PrintNstCW(PFN.PCW,1);
//			}
//			else
//			{
//				PrintNstCW2(PFN.PCW,PFN.minPCW-1,0);	//PrintNstCW(PFN.PCW,1);
//			}
//		}
//	}
//-----------------
//	if(PFNf_set_nco)	//wybrano nastawe CO?
//	{
//		pcw=PFN.PCO;
//#if (_EXT_BOARD==0)     //stara wersja plytki wyswietlacza   
//		ReadParam((unsigned int *)(&PFN.PCO),&zmn,&zps,PFN.minPCO,PFN.maxPCO,0,1);
//#else
//#if (_EXT_BOARD==1)     //nowa wersja plytki wyswietlacza z interfejsem 10V  
//        ReadParam((unsigned int *)(&PFN.PCO),&zmn,&zps,PFN.minPCO,PFN.maxPCO,0,1);
//#endif
//#if (_EXT_BOARD==2)     //nowa wersja plytki wyswietlacza z interfejsem 10V i LIN   
//        if(!PFNf_LIN_KSK)    //nieaktywny interfejs 10V, interfejs LIN w trybie kaskady lub zdalnego sterowania ze strefy czy RT?
//        {
//            ReadParam((unsigned int *)(&PFN.PCO),&zmn,&zps,PFN.minPCO,PFN.maxPCO,0,1);
//        }
//#endif 
//#if (_EXT_BOARD==3)     //nowa wersja plytki wyswietlacza z interfejsem 10V, LIN i OT        
//        if(!PFNf_LIN_KSK)    //nieaktywny interfejs 10V, interfejs LIN w trybie kaskady lub zdalnego sterowania ze strefy czy RT?
//        {
//            ReadParam((unsigned int *)(&PFN.PCO),&zmn,&zps,PFN.minPCO,PFN.maxPCO,0,1);
//        }
//#endif        
//#endif
//		if(!PFNf_b_set_nco)	//stan po nacisnieciu przycisku set?
//		{
//			PFNf_b_set_nco=1;
//			EndNst();
//			zmn2=1;
//			pcw=PFN.PCO;
//			PFNf_newPCO=0;				
//		}
//		if(zmn2||zmn)				//nastawa ulegla zmianie?
//		{
//			if(!PFNf_newPCO) PFNf_newPCO=(pcw!=PFN.PCO);
//			zmn2=0;
//			StartRTdS(_RTSET);
//			PrintNstCO(PFN.PCO,PFNf_pgd,0);
//		}
//	}
//-----------------
	if(PFNf_set_eco)	//wybrano nastawe parametru ECO?
	{
		eco=PFN.ECO;

		//if(!PFNf_mod_eco&&!RdPrt(S_KUP)&&!RdPrt(S_KDW))
        if(!PFNf_mod_eco&&!RdPrt(S_BAR)&&!RdPrt(S_RES))    
		{
			PFNf_mod_eco=1;
		}
		//if(PFNf_mod_eco) ReadParam((unsigned int *)(&PFN.ECO),&zmn,&zps,1,9,0,1);
		if(PFNf_mod_eco)
        {
            bfdta=(signed int)PFN.ECO;//kkk
            ReadParamONE(&bfdta,&zmn,&zps,1,9,1,1);  
            PFN.ECO=(unsigned int)bfdta;//kkk
        }
		if(!PFNf_b_set_eco)	//nacisnieto przycisk set?
		{
			PFNf_b_set_eco=1;
			EndNst();
			zmn2=1;
            inst=1;     //mmm            
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
    //-----------------     //mmm
    if(PFNf_set_eco)    
    {
        if(GetPoz(_KL_NCWCO,0))
        {
            inst=0;
        }
        if(!inst)
        {        
            if(GetPoz(_KL_NCWCO,0)) 
            {
                MarkMinMaxECO();
            }
            else       
            {
                Mark(_GZ_MNCO,0);
                Mark(_GZ_MXCO,0);            
                Mark(_GZ_MNCO,0);
                Mark(_GZ_MXCO,0);
            }
        }
    } 
    if(!PFNf_set_eco)
    {
        inst=0;
    }    
//-----------------
	if(!PFNf_set_ncw&&!PFNf_set_nco&&!PFNf_set_eco&&!PFNf_newPCO&&!PFNf_newPCW)	//tryb modyfikacji nastawy jest nieaktywny?
	{
		if(!PFNf_str_ncw&&!PFNf_str_nco) EndNst();
		PFNf_b_set_ncw=0;
		PFNf_b_set_nco=0;
		PFNf_b_set_eco=0;
        PFNf_bnewPCO=0;
        PFNf_bnewPCW=0;
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

		if(!PFNf_EnAleg)
		{
			ReadParamCW((unsigned int *)(&PFN.PCW),&zmn,&zps,PFN.minPCW,PFN.maxPCW,0,1);
		}
		else
		{
			ReadParamCW((unsigned int *)(&PFN.PCW),&zmn,&zps,PFN.minPCW-1,PFN.maxPCW,0,1);
		}
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
			if(!PFNf_EnAleg)
			{
				PrintNstCW(PFN.PCW,0);	//PrintNstCW(PFN.PCW,1);
			}
			else
			{
				PrintNstCW2(PFN.PCW,PFN.minPCW-1,0);	//PrintNstCW(PFN.PCW,1);
			}
		}
	}
//-----------------
	if(PFNf_set_nco)	//wybrano nastawe CO?
	{
		pco=PFN.PCO;
		//na potrzeby parametru ECO przytrzymanie przycisku +/- wstrzymuje nastawe
		if(!PFNf_mod_ncw&&!RdPrt(S_KUP_CO)&&!RdPrt(S_KDW_CO))
		{
			PFNf_mod_ncw=1;
		}        
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
	if(PFNf_set_eco)	//wybrano nastawe parametru ECO?
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
//--------------------------------------------------------------
void DecWsw(void)
{    
    volatile unsigned char fl,nr;
    
    if(PFNf_toInitNST) return;
    
    if(PFNf_fsetLaddr||PFNf_fshowLaddr)              //aktywna funkcja przypisania/wizualizacji adresu LIN?
    {
#if _KEY_BBOARD==0             
        //if(!RdPrt(S_SET)) PFNf_setKey=0;
        if(!RdPrt(S_BAR)) PFNf_setKey=0;
#endif
#if _KEY_BBOARD==1             
        if(!RdPrt(S_KOM)) PFNf_setKey=0;
#endif            
        //--------------
        EndErr();
//        if(PFN.ALIN>=0x05) PrintLN(0);
//        else PrintLN(PFN.ALIN);
        nr=ALINtoNR(PFN.ALIN);
        PrintLN(nr);
        SetPulseLLed();
        if(PFNf_fsetLaddr) PFNf_fshowLaddr=0;
        if(RTdS(_RTALIN)>30)                        //czas przez jaki wyswietlany jest adres LIN
        {
            PFNf_fshowLaddr=0;
            if(PFNf_fsetLaddr)
            {
#if _KEY_BBOARD==0                    
                //if(!RdPrt(S_SET))           //poszczony przycisk SET?
                if(!RdPrt(S_BAR))           //poszczony przycisk SET?                    
                {
                    PFNf_fsetLaddr=0;
                    //SetPrt(S_SET,0);
                    SetPrt(S_BAR,0);
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
        if(!PFNf_aktywne_LKNF) //kkk
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
                        if(PFN.KERR!=0x0E&&PFN.KERR!=0x09) PrintErrMsg(_PRN_MSG,led_E,PFN.KERR,0,0,PFNf_opgres);		//migajacy kod awaryjny bez blokady
                        else PrintErrMsg(_PRN_MSG,led_E,PFN.KERR,PFN.KERR==0x0E,PFN.KERR==0x09,PFNf_opgres);			//naprzemienny kod awaryjny
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
    }
	if(!PFNf_fsetLaddr&&!PFNf_fshowLaddr&&!PFNf_opKNF&&!PFNf_aktywne_LKNF&&!PFNf_ferr&&!PFNf_fmsg&&!PFNf_prODP)     //kkk
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
            NastawaCOCW();
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
						if(PFNf_prn_res||PFNf_prn_bar)
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
							else
							if(PFNf_prn_fl) PrintFL(PFN.PRZ);                       //wylaczny wskaz FL
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
						if(PFNf_prn_res||PFNf_prn_bar)
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
							else
							if(PFNf_prn_fl) PrintFL(PFN.PRZ);                       //wylaczny wskaz FL                            
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
						if(PFNf_prn_res||PFNf_prn_bar)
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
							else
							if(PFNf_prn_fl) PrintFL(PFN.PRZ);                       //wylaczny wskaz FL                           
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
							if(PFNf_prn_res||PFNf_prn_bar)
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
                                else
                                if(PFNf_prn_fl) PrintFL(PFN.PRZ);                       //wylaczny wskaz FL                                
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
							if(PFNf_prn_res||PFNf_prn_bar)
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
                                else
                                if(PFNf_prn_fl) PrintFL(PFN.PRZ);                       //wylaczny wskaz FL                                
							}
							else														//wskaz CW
							{
								if(!PFNf_errCW) PrintCW(PFN.CW,0);						//wylaczny wskaz temp CW
							}
						}                        
					}
				}
				else																	//aktywna funkcja serwisowa lub antylegionella
				{
					if(PFNf_ofnserw||PFNf_set_fnserw) 									//aktywna funkcja serwisowa?								
					{
						if(!PFNf_fshowLpulse) SetLightLLed(1); 												//rozswietlenie maksymalne
						if(!PFNf_errCO) PrintCO(PFN.CO,1,PFNf_fL3);						//dzielony wskaz temp CO z predkoscia obrotowa
						PrintVNT(PFN.VNT,1);
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
	//znaki wysterowywane niezale�nie (sygna�y z UNI-02)
	Mark(_GZ_PLW,PFNf_plmCW);												//znacznik plomien CW
	Mark(_GZ_PLC,PFNf_plmCO);												//znacznik plomien CO    
	//Mark(_GZ_PGD,PFNf_pgd&&!PFNf_oflato&&!PFNf_ofoff&&!PFNf_opKNF);			//znacznik funkcja pogodowa     
    if((PFNf_pgd||PFNf_ex_pgd)&&!PFNf_oflato&&!PFNf_ofoff&&!PFNf_opKNF&&!PFNf_aktywne_LKNF)      //znacznik funkcja pogodowa  //kkk
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
        Mark(_GZ_PGD,0);     //wyma� znacznik funkcja pogodowa        
    }
#if _PRNL3==0   
    if(PFNf_ofzima&&!PFNf_ofoff&&!PFNf_opKNF&&!PFNf_aktywne_LKNF)       //kkk
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
	//Mark(_GZ_DCW,(PFNf_ofzima||PFNf_oflato)&&!PFNf_ofoff&&!PFNf_opKNF);		//znacznik KRAN
    if((PFNf_ofzima||PFNf_oflato)&&!PFNf_ofoff&&!PFNf_opKNF&&!PFNf_aktywne_LKNF)        //kkk
    {
        if(PFNf_cwUP)
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
    Mark(_GZ_DCW,fl);		//znacznik KRAN
    
    if(!PFNf_opKNF&&!PFNf_aktywne_LKNF)    //kkk     //nieaktywna konfiguracja?
    {
        //if(PFNf_ofoff||(DtCW._pozZRO&&DtCO._pozZRO))  //pozycja OFF? PFNf_off
        if(PFNf_ofoff||PFNf_off)  //pozycja OFF? PFNf_off    
        {
            Mark(_GZ_OFF,!PFNf_fsetLaddr);									//zapal znacznik OFF
            Mark(_GZ_MNCW,0);									//wygas min CW
            Mark(_GZ_MNCO,0);									//wygas min CO
        }
        //if(!PFNf_ofoff||!(DtCW._pozZRO&&DtCO._pozZRO))  //pozycja OFF?
        if(!PFNf_ofoff||!PFNf_off)  //pozycja OFF?    
        {
            Mark(_GZ_OFF,0);									//wygas znacznik OFF
        }
    }
    
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

	if(!PFNf_opKNF&&!PFNf_aktywne_LKNF&&!PFNf_ferr&&!PFNf_fmsg&&!PFNf_set_ncw&&!PFNf_set_nco&&!PFNf_set_eco)  //kkk
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
#else //demo
//****************************************************************************************************88
void DecWsw(void)
{
    signed int bfdta; //kkk
    
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
	if(!PFNf_aktywne_KNF&&!PFNf_aktywne_LKNF&&!PFNf_ferr) //kkk
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
	//znaki wysterowywane niezale�nie (sygna�y z UNI-01)
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
			if((PFNf_opgres&&PFNf_ferr)||(PFNf_kalVNT&&!PFNf_toInit))
			{
				PFN.KRS=1;
			}
			break;
		}
		case 1:
		{
			if((!PFNf_opgres||!PFNf_ferr))//||(PFNf_kalVNT&&!PFNf_toInit))
			{
				PFNf_toInit=1;
				PFN.KRS=0;
			}
		}
	}   
    /*switch(PFN.KRS)
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
	}*/
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
