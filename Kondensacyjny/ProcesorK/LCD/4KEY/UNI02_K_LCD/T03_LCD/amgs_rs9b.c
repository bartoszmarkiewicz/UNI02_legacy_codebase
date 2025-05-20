/*amgs_rs9b.c*/
//Funkcje obslugi magistrali RS232 dla AUART2

/*---------------------------------------------------------------------------------*/
//#include <pic18.h>
#include <xc.h>
#include "global.h"			//parametry globalne
#if (_EXT_BOARD!=0)

#include "main.tp.h"
#include "ammy1.h"
#include "auart9b.tp.h"
#include "amgs_rs9b.h"			//parametry lokalne
#include "LIN1SInterpreter.h"
#include "wersja.h"
#include "ot.tp.h"

volatile MGSRSData MGSD;
volatile unsigned char stbf[32]={0};
extern volatile UARTData UADt;
extern tpRSDTA1 RSDT1;
extern FRAME_LIN1 FRL1,FTL1;
extern volatile tpOTMaster OTMaster;
extern volatile DataPFN PFN;
volatile unsigned char xxy,zzz,vvv;
//-------------------------------------------------------------------------------
//Timer na potrzeby transmisji (umiescic w przerwaniu od zegara TMR1)
void IncRSTimer(void)
{
	if(MGSD.timRS<255) MGSD.timRS++;
}
//-------------------------------------------------------------------------------
//Restart zegara 
void StartRSTim(void)
{
	MGSD.timRS=0;
}
//-------------------------------------------------------------------------------
//Odczyt stanu zegara 
unsigned char RSTim(void)
{
	return MGSD.timRS;
}
//-------------------------------------------------------------------------------
//Inicjalizacja danych zwiazanych z transmisj¹ UART
void InitTransProc(void)
{
	MGSDf_TransData=0;
	MGSDf_TransDataTR=0;
	MGSDf_TransDataRC=0;
	MGSDf_TransOK=0;
	MGSDf_TransACK=0;
	MGSDf_TERR_TIM=0;
	MGSDf_RERR_TIM=0;
	MGSDf_RERR_REC=0;
	MGSDf_ERRt=0;
	MGSD.timRS=0;
	MGSD.MTRS=0;
	MGSD.MTRS2=0;
	MGSD.KTR=0;
	MGSD.LCN=0;
}
//-------------------------------------------------------------------------------
//Uruchomienie transmisji danych
void StartTransData(void)
{
	MGSDf_TransData=1;		//ustaw bit aktywacji proc. TransData
	MGSD.MTRS=0;
}
/*---------------------------------------------------------------------------------*/
//Realizacja transmisji danych do/z odbiornika
//gdzie:
// tnDat - liczba bajtow wysylanych do odbiornika (razem z CHK jesli wystepuje),
// rnDat - liczba bajtow odpowiedzi odbiornika (razem z CHK jesli wystepuje).
void TransData(const unsigned char tnDat)//,const unsigned char rnDat)
{
	switch(MGSD.MTRS)
	{
		case 0:		
			if(tnDat) StartUSARTTR();//tnDat);			//uaktywnij nadawanie
			StartRSTim();
			MGSD.MTRS=1;
		case 1:
			if(tnDat)
			{
				if(TX2IE)							//trwa nadawanie?
				{
					if(((RSTim()>1)&&!UADtf_TData)||(RSTim()>_TS_TOUT))			//uplynol maksymalny czas oczekiwania na zak. nadawania?	
					{
						MGSD.MTRS=4;
						break;
					}
					break;
				}
			}
			MGSDf_TERR_TIM=0;
			if(!TXSTA2bits.TRMT) break;						//odczekaj na fizyczne zakonczenie transmisji
			StopUSARTTR();
 			StartRSTim();
			MGSD.MTRS=2;
		case 2:
			if(!UADtf_REnd&&(RSTim()<_RS_TOUT)) break;
			if(!UADtf_REnd)
			{
				MGSD.MTRS=5;
				break;
			}
			MGSDf_RERR_TIM=0;
			if(UADtf_REnd)							//odbior bez problemu?
			{
				
				UADtf_REnd=0;
				MGSDf_TransOK=1;						//znacznik potwierdzenia odebrania paczki danych/zakonczenie trans.
				MGSDf_RERR_REC=0;			
			}
			else
			{ 
				MGSDf_TransOK=0;
				MGSDf_RERR_REC=1;
			}
			MGSDf_TERR_TIM=0;						//przekroczony limit czasu dla transmisji
			MGSDf_RERR_TIM=0;						//przekroczony limit czasu dla odbioru
			MGSD.MTRS=6;
			break;
		case 4:
			MGSDf_TERR_TIM=1;						//przekroczony limit czasu dla transmisji
			MGSDf_TransOK=0;
			StopUSARTTR();
			MGSD.MTRS=6;
			break;
		case 5:
			MGSDf_RERR_TIM=1;						//przekroczony limit czasu dla odbioru
			MGSDf_TransOK=0;
		case 6:
			MGSDf_TransData=0;
			MGSD.MTRS=0;
			break;
	}
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//------------------Procedury zwiazane z obsluga transmisji z ukladem UNI-01
/*---------------------------------------------------------------------------------*/
//Zapisuje dane z modulu UNI-02
void SaveRSToData(void)
{
    volatile unsigned char i;
	//unsigned char idn;
	//0 - adres master
	//1 - liczba przesylanych bajtow (ndat)
	//2 - identyfikator paczki danych
	RSDT1.outID=GetRSRBuf(2);

	switch(RSDT1.outID)
	{
		case _RD_STD:				//identyfikator paczki danych
		{
            //(RAMKA DLA WERSJI OPROGRAMOWANIA<=8)
			if(!RSDT1f_RRD_STD)		//aktywna blokada na czas interpretacji danych?
			{
				RSDT1f_RD_STD=0;
				switch(GetNDat())	//liczba przeslanych bajtow
				{
					case 28:
						RSDT1.vSTD=1;
						break;
					case 30:
						RSDT1.vSTD=2;
						break; 
					case 31:
						RSDT1.vSTD=3;
						break;                        
                    default:
						if(GetNDat()>31) RSDT1.vSTD=3;
                        else RSDT1.vSTD=3;
						break;                        
                }                
				RSDT1.outRDZ0=GetRSRBuf(3);
				RSDT1.outRDZ1=GetRSRBuf(4);
                RSDT1.outRDZ2=0;
				RSDT1.outSTAT0=GetRSRBuf(5);
				RSDT1.outSTAT1=GetRSRBuf(6);
				RSDT1.outSTAT2=GetRSRBuf(7);
				RSDT1.outSTAT3=GetRSRBuf(8);
				RSDT1.outCO=GetRSRBuf(9);
				RSDT1.outCW=GetRSRBuf(10);
				RSDT1.outCS=GetRSRBuf(11);
				RSDT1.outIN=GetRSRBuf(12);
				RSDT1.outVNT=GetRSRBuf(13);
				RSDT1.outPCO=GetRSRBuf(14);
				RSDT1.outPCOmn=GetRSRBuf(15);
				RSDT1.outPCOmx=GetRSRBuf(16);
				RSDT1.outPCW=GetRSRBuf(17);
				RSDT1.outPCWmn=GetRSRBuf(18);
				RSDT1.outPCWmx=GetRSRBuf(19);
				RSDT1.outPPCO=GetRSRBuf(20);
				RSDT1.outPPCW=GetRSRBuf(21);
				RSDT1.outERR=GetRSRBuf(22);
				RSDT1.outAWR=GetRSRBuf(23);
				RSDT1.outECO=GetRSRBuf(24);
				RSDT1.outPP=GetRSRBuf(25);
				RSDT1.outPG=GetRSRBuf(26);    
                if(RSDT1.vSTD>1)
                {                
                    RSDT1.outrCO=GetRSRBuf(27);
                    RSDT1.outrCW=GetRSRBuf(28);
                }
                else
                {
                    RSDT1.outrCO=RSDT1.outCO;
                    RSDT1.outrCW=RSDT1.outCW;                    
                }
                if(RSDT1.vSTD>2)
                {
                    RSDT1.outSTAT4=GetRSRBuf(29);  
                    RSDT1.outSTAT5=0;                    
                }
                else
                {
                    RSDT1.outSTAT4=0;
                    RSDT1.outSTAT5=0;                    
                }                               
				RSDT1f_RD_STD=1;
			}
			break;
		}
		case _RD_STD2:				//identyfikator paczki danych
		{
            //(RAMKA DLA WERSJI OPROGRAMOWANIA>=9)
			if(!RSDT1f_RRD_STD)		//aktywna blokada na czas interpretacji danych?
			{
				RSDT1f_RD_STD=0;
				switch(GetExNDat())	//liczba przeslanych bajtow
				{
					case 37:
						RSDT1.vSTD=4;
						break;                        
                    default:
						if(GetExNDat()>37) RSDT1.vSTD=4;
                        else RSDT1.vSTD=4;
						break;                        
                }                
				RSDT1.outRDZ0=GetRSRBuf(4);
				RSDT1.outRDZ1=GetRSRBuf(5);
				RSDT1.outRDZ2=GetRSRBuf(6);                
				RSDT1.outSTAT0=GetRSRBuf(7);
				RSDT1.outSTAT1=GetRSRBuf(8);
				RSDT1.outSTAT2=GetRSRBuf(9);
				RSDT1.outSTAT3=GetRSRBuf(10);
				RSDT1.outSTAT4=GetRSRBuf(11);      
				RSDT1.outSTAT5=GetRSRBuf(12);                
				RSDT1.outCO=GetRSRBuf(13);
				RSDT1.outCW=GetRSRBuf(14);
				RSDT1.outCS=GetRSRBuf(15);
				RSDT1.outIN=GetRSRBuf(16);
				RSDT1.outVNT=GetRSRBuf(17);
				RSDT1.outPCO=GetRSRBuf(18);
				RSDT1.outPCOmn=GetRSRBuf(19);
				RSDT1.outPCOmx=GetRSRBuf(20);
				RSDT1.outPCW=GetRSRBuf(21);
				RSDT1.outPCWmn=GetRSRBuf(22);
				RSDT1.outPCWmx=GetRSRBuf(23);
				RSDT1.outPPCO=GetRSRBuf(24);
				RSDT1.outPPCW=GetRSRBuf(25);
				RSDT1.outERR=GetRSRBuf(26);
				RSDT1.outAWR=GetRSRBuf(27);
				RSDT1.outECO=GetRSRBuf(28);
				RSDT1.outPP=GetRSRBuf(29);
				RSDT1.outPG=GetRSRBuf(30);                
                RSDT1.outrCO=GetRSRBuf(31);
                RSDT1.outrCW=GetRSRBuf(32);
                RSDT1.outZW=GetRSRBuf(33);    
                RSDT1.outSETPmin=GetRSRBuf(34);
                RSDT1.outSETPmax=GetRSRBuf(35);                
				RSDT1f_RD_STD=1;
			}
			break;
		}                 
		case _RD_KNF:
		{
            //(RAMKA DLA WERSJI OPROGRAMOWANIA<=8)
			if(!RSDT1f_RRD_KNF)		//aktywna blokada na czas interpretacji danych?
			{
				RSDT1f_RD_KNF=0;
				//okreslenie wersji paczki danych (na potrzeby proc.konf.- kompatybilnosc wsteczna)
				switch(GetNDat())	//liczba przeslanych bajtow
				{
					case 22:
						RSDT1.vKNF=1;
						break;
					case 24:
						RSDT1.vKNF=2;
						break;
					case 25:
						RSDT1.vKNF=3;
						break;
					case 26:
						RSDT1.vKNF=4;
						break;    
					case 29:
						RSDT1.vKNF=5;
						break;      
                    case 30:
                        RSDT1.vKNF=6;
                        break;                        
					default:
                        if(GetNDat()>30) RSDT1.vKNF=6;
                        else RSDT1.vKNF=6;
						break;												
				}
				RSDT1.outKSTAT0=GetRSRBuf(3);
				RSDT1.outKRK=GetRSRBuf(4);
				RSDT1.outPRM=GetRSRBuf(5);
				RSDT1.outMSTR=GetRSRBuf(6);
				RSDT1.outMMAXU=GetRSRBuf(7);
				RSDT1.outMMAXG=GetRSRBuf(8);
				RSDT1.outOPDG=GetRSRBuf(9);
				RSDT1.outWANL=GetRSRBuf(10);
				RSDT1.outKODKT=GetRSRBuf(11);
				RSDT1.outWOBG=GetRSRBuf(12);
				RSDT1.outNOKR=GetRSRBuf(13);
				RSDT1.outTPCS=GetRSRBuf(14);
				RSDT1.outMMIN=GetRSRBuf(15);
				RSDT1.outMMAX=GetRSRBuf(16);
				RSDT1.outRDPM=GetRSRBuf(17);
				RSDT1.outDTPM=GetRSRBuf(18);
				RSDT1.outMDPM=GetRSRBuf(19);
				RSDT1.outTECO=GetRSRBuf(20);
				RSDT1.outTMMAXG2=GetRSRBuf(21);
				RSDT1.outMMAXG2=GetRSRBuf(22);
				RSDT1.outMGPM=GetRSRBuf(23);
				RSDT1.outMGPM2=GetRSRBuf(24);
                RSDT1.outHSCO=GetRSRBuf(25);
                RSDT1.outCONW=GetRSRBuf(26);
                RSDT1.outPROP=GetRSRBuf(27);
                RSDT1.outRFAN=GetRSRBuf(28);                
				RSDT1f_RD_KNF=1;
			}
			break;
		}
		case _RD_KNF2:
		{
            //(RAMKA DLA WERSJI OPROGRAMOWANIA>=9)
			if(!RSDT1f_RRD_KNF)		//aktywna blokada na czas interpretacji danych?
			{
				RSDT1f_RD_KNF=0;
				//okreslenie wersji paczki danych (na potrzeby proc.konf.- kompatybilnosc wsteczna)
				switch(GetExNDat())	//liczba przeslanych bajtow
				{ 
                    case 39:
                        RSDT1.vKNF=9;
                        break;                    
                    case 37:
                        RSDT1.vKNF=8;
                        break;                    
                    case 36:
                        RSDT1.vKNF=7;
                        break;                        
					default:
						if(GetExNDat()>39) RSDT1.vKNF=9;
                        else RSDT1.vKNF=9;
						break;
				}
				RSDT1.outKSTAT0=GetRSRBuf(4);
				RSDT1.outKRK=GetRSRBuf(5);
				RSDT1.outPRM=GetRSRBuf(6);
				RSDT1.outMSTR=GetRSRBuf(7);
				RSDT1.outMMAXU=GetRSRBuf(8);
				RSDT1.outMMAXG=GetRSRBuf(9);
				RSDT1.outOPDG=GetRSRBuf(10);
				RSDT1.outWANL=GetRSRBuf(11);
				RSDT1.outKODKT=GetRSRBuf(12);
				RSDT1.outWOBG=GetRSRBuf(13);
				RSDT1.outNOKR=GetRSRBuf(14);
				RSDT1.outTPCS=GetRSRBuf(15);
				RSDT1.outMMIN=GetRSRBuf(16);
				RSDT1.outMMAX=GetRSRBuf(17);
				RSDT1.outRDPM=GetRSRBuf(18);
				RSDT1.outDTPM=GetRSRBuf(19);
				RSDT1.outMDPM=GetRSRBuf(20);
				RSDT1.outTECO=GetRSRBuf(21);
				RSDT1.outTMMAXG2=GetRSRBuf(22);
				RSDT1.outMMAXG2=GetRSRBuf(23);
				RSDT1.outMGPM=GetRSRBuf(24);
				RSDT1.outMGPM2=GetRSRBuf(25);     
                RSDT1.outHSCO=GetRSRBuf(26);
                RSDT1.outCONW=GetRSRBuf(27);
                RSDT1.outPROP=GetRSRBuf(28); 
                RSDT1.outRFAN=GetRSRBuf(29); 
                RSDT1.outTSTR=GetRSRBuf(30);     
                RSDT1.outTL3=GetRSRBuf(31);                 
                RSDT1.outPGDR=GetRSRBuf(32); 
                RSDT1.outPGDG=GetRSRBuf(33);     
                RSDT1.outPGDO=GetRSRBuf(34);     
                RSDT1.outPGMX=GetRSRBuf(35);
                RSDT1.outCWMG=GetRSRBuf(36);                
                RSDT1.outCWTM=GetRSRBuf(37);                
				RSDT1f_RD_KNF=1;
			}
			break;
		}         
#if (_EXT_BOARD>=2)                     //aktywowany modul LIN1SLAVE?
		case _RD_LIN:
		{
            RSDT1f_RD_LIN=0;
            RSDT1.outLIDN=GetRSRBuf(3);
            switch(RSDT1.outLIDN)
            {
                /*case _LIN_GKNF:
                {
                    if(!RSDT1f_RRD_KNF)		//aktywna blokada na czas interpretacji danych?
                    {
                        //okreslenie wersji paczki danych (na potrzeby proc.konf.- kompatybilnosc wsteczna)
                        switch(GetRSRBuf(1)&0b00011111)
                        {
                                case 22:
                                        RSDT1.vKNF=1;
                                        break;
                                case 24:
                                        RSDT1.vKNF=2;
                                        break;
                                case 25:
                                        RSDT1.vKNF=3;
                                        break;
                                case 26:
                                        RSDT1.vKNF=4;
                                        break;  
                                case 29:
                                        RSDT1.vKNF=5;
                                        break; 
                                case 30:
                                        RSDT1.vKNF=6;
                                        break;                                        
                                default:
                                        RSDT1.vKNF=2;
                                        break;
                        }
                        RSDT1.outKSTAT0=GetRSRBuf(5);
                        RSDT1.outMSTR=GetRSRBuf(6);
                        RSDT1.outMMAXU=GetRSRBuf(7);
                        RSDT1.outMMAXG=GetRSRBuf(8);
                        RSDT1.outOPDG=GetRSRBuf(9);
                        RSDT1.outWANL=GetRSRBuf(10);
                        RSDT1.outKODKT=GetRSRBuf(11);
                        RSDT1.outWOBG=GetRSRBuf(12);
                        RSDT1.outNOKR=GetRSRBuf(13);
                        RSDT1.outTPCS=GetRSRBuf(14);
                        RSDT1.outMMIN=GetRSRBuf(15);
                        RSDT1.outMMAX=GetRSRBuf(16);
                        RSDT1.outRDPM=GetRSRBuf(17);
                        RSDT1.outDTPM=GetRSRBuf(18);
                        RSDT1.outMDPM=GetRSRBuf(19);
                        RSDT1.outTECO=GetRSRBuf(20);
                        RSDT1.outTMMAXG2=GetRSRBuf(21);
                        RSDT1.outMMAXG2=GetRSRBuf(22);
                        RSDT1.outMGPM=GetRSRBuf(23);
                        RSDT1.outMGPM2=GetRSRBuf(24);   
                        RSDT1.outHSCO=GetRSRBuf(25);
                        RSDT1.outCONW=GetRSRBuf(26);
                        RSDT1.outPROP=GetRSRBuf(27); 
                        RSDT1.outRFAN=GetRSRBuf(28);                         
                        RSDT1f_RD_LIN=1;                 //znacznik odpowiedzi
                    }
                    break;
                }*/
                case _LIN_SADR:                         //aktualny adres lin = parametr1 odpowiedz
                case _LIN_WHFLG:
                case _LIN_RPV:                    
                {
                    RSDT1.outLDTA1=GetRSRBuf(4);        
                    RSDT1f_RD_LIN=1;                    //znacznik odpowiedzi
                    break;
                }                 
                case _LIN_RPKNF:                
                case _LIN_WPKNF: 
                case _LIN_RPP:    
                case _LIN_RPI:   
                case _LIN_RPD:
                case _LIN_WRCO1:
                case _LIN_WRCO2:
                case _LIN_WRCO3:
                case _LIN_WRCO4:     
                case _LIN_WRCW1:
                case _LIN_WRCW2:
                case _LIN_WRCW3:
                case _LIN_WRCW4:                    
                case _LIN_WPV:                     
                {
                    RSDT1.outLDTA1=GetRSRBuf(4);        
                    RSDT1.outLDTA2=GetRSRBuf(5);                             
                    RSDT1f_RD_LIN=1;                    //znacznik odpowiedzi
                    break;
                } 
                case _LIN_WPP:     
                case _LIN_WPI:   
                case _LIN_WPD:                    
                {
                    RSDT1.outLDTA1=GetRSRBuf(4);        //F88
                    RSDT1.outLDTA2=GetRSRBuf(5);           
                    RSDT1.outLDTA3=GetRSRBuf(6);        //status operacji                     
                    RSDT1f_RD_LIN=1;                    //znacznik odpowiedzi
                    break;
                }     
                case _LIN_RRCO:      
                case _LIN_RRCW:                    
                {
                    RSDT1.outLDTA1=GetRSRBuf(4);        
                    RSDT1.outLDTA2=GetRSRBuf(5);           
                    RSDT1.outLDTA3=GetRSRBuf(6);          
                    RSDT1.outLDTA4=GetRSRBuf(7);                            
                    RSDT1f_RD_LIN=1;                    //znacznik odpowiedzi
                    break;
                }                
                case _LIN_GSTAT:                           
                {                
                    RSDT1.outLDTA1=GetRSRBuf(4);        //LO ogolna liczba zdarzen awaryjnych
                    RSDT1.outLDTA2=GetRSRBuf(5);        //HI ogolna liczba zdarzen awaryjnych   
                    RSDT1.outLDTA3=GetRSRBuf(6);        //LO w tym liczba zdarzen awaryjnych z blokada
                    RSDT1.outLDTA4=GetRSRBuf(7);        //LO w tym liczba zdarzen awaryjnych z blokada  
                    RSDT1.outLDTA5=GetRSRBuf(8);        //licznik flagi serwisowej historii zdarzen awaryjnych                    
                    RSDT1f_RD_LIN=1;                    //znacznik odpowiedzi
                    break;
                }                
                case _LIN_GEEST:                           
                {                
                    for(i=0;i<32;i++) stbf[i]=0; 
                    for(i=0;i<_NDT_RRD_EEST;i++)
                    {
                        if(i>=32) break;
                        stbf[i]=GetRSRBuf(4+i);        //wartosc parametru po operacji zapisu
                    }                    
                    RSDT1f_RD_LIN=1;                    //znacznik odpowiedzi
                    break;
                }  
                case _LIN_GHIST:                           
                {   
                    for(i=0;i<32;i++) stbf[i]=0;                    
                    stbf[0]=GetRSRBuf(4);        //REFL
                    stbf[1]=GetRSRBuf(5);        //REFH
                    stbf[2]=GetRSRBuf(6);        //liczba danych                   
                    if((!stbf[0]&&!stbf[1])||!stbf[2])
                    {
                        stbf[2]=0;
                    }
                    else
                    {
                        for(i=0;i<stbf[2];i++)
                        {
                            if((3+i)>=32) break;
                            stbf[3+i]=GetRSRBuf(7+((stbf[2]-1)-i));   //dane bufora zdarzen
                        }
                    }                    
                    RSDT1f_RD_LIN=1;                    //znacznik odpowiedzi
                    break;  
                }          
            }
            break;
        }
#endif
    }
    //ndat - CHK
}
//Przygotowanie danych do wysylki do modulu UNI-02 (razem z obliczonym CHK)
void SaveDataToRS(unsigned char tDTA)
{
	unsigned char ndat;		//liczba danych wysy³anych + CHK
	unsigned char chkndat;

	switch(tDTA)
	{
		case _RD_STD:				//identyfikator paczki danych
		{
			ndat=11;                 //9
			chkndat=SetNDatCHK(ndat);		//oblicz CHK dla liczby danych
		
			SetRSTBuf(0,_UNI02_ADDR);		//0 - adres slave
			SetRSTBuf(1,chkndat);			//1 - liczba przesylanych bajtow (ndat)
			SetRSTBuf(2,_RD_STD);			//2 - identyfikator paczki danych
			SetRSTBuf(3,RSDT1.inSTAT0);
			SetRSTBuf(4,RSDT1.inSTAT1);
			SetRSTBuf(5,RSDT1.inPCO);
			SetRSTBuf(6,RSDT1.inPCW);
			SetRSTBuf(7,RSDT1.inECO);
			SetRSTBuf(8,RSDT1.inLNSTAT);    //8 - (wystepowanie bez wzgledu na zawartosc wskazuje na wersje oprogramowania >=7 i zezwala na funkcje odpowietrzenia)
			SetRSTBuf(9,0);                 //9 - (wystepowanie bez wzgledu na zawartosc wskazuje na wersje oprogramowania >=9)           
			SetRSCHK(ndat);					//ndat - CHK z bajtow [0..ndat] i zapisuje na poz. ndat
			//ndat - CHK
			break;
		}
		case _RD_STD2:				//identyfikator paczki danych
		{
			ndat=15;                        //13
            SetExNDatCHK(ndat);            
			SetRSTBuf(0,_UNI02_ADDR);		//0 - adres slave
			//SetRSTBuf(1,chkndat);			//1 - liczba przesylanych bajtow (ndat)
			SetRSTBuf(2,_RD_STD2);			//2 - identyfikator paczki danych
			//SetRSTBuf(3,chkndat);			//3 - liczba przesylanych bajtow (ndat) 
            SetRSTBuf(4,_WERSJA);
			SetRSTBuf(5,RSDT1.inSTAT0);
			SetRSTBuf(6,RSDT1.inSTAT1);
			SetRSTBuf(7,RSDT1.inPCO);
			SetRSTBuf(8,RSDT1.inPCW);
			SetRSTBuf(9,RSDT1.inECO);
            
            //*********************************************************
            //TEST OT
            //BitSet3(&RSDT1.inINSTAT,_FOT,1);      //****
            //BitSet3(&RSDT1.inINSTAT,_OT_DPGD,1);  //****
            //BitSet3(&RSDT1.inINSTAT,_OT_STR,1);   //****
            //BitSet3(&RSDT1.inINSTAT,_OT_CW,1);    //****
            //********************************************************* 
            
			SetRSTBuf(10,RSDT1.inINSTAT);                 
			SetRSTBuf(11,RSDT1.inLNSTAT);  
            SetRSTBuf(12,RSDT1.inCONTSETP); //obliczona docelowa wartosc punktu modulacji dla grzaniaobiegu WG  
			SetRSTBuf(13,RSDT1.inSTAT2);              
			SetRSCHK(ndat);					//ndat - CHK z bajtow [0..ndat] i zapisuje na poz. ndat
			//ndat - CHK
			break;
		}        
		case _RD_KNF:				//identyfikator paczki danych
		{
			ndat=31;
			chkndat=SetNDatCHK(ndat);		//oblicz CHK dla liczby danych
		
			SetRSTBuf(0,_UNI02_ADDR);		//0 - adres slave
			SetRSTBuf(1,chkndat);			//1 - liczba przesylanych bajtow (ndat)
			SetRSTBuf(2,_RD_KNF);			//2 - identyfikator paczki danych
			SetRSTBuf(3,RSDT1.inKSTAT0);
			SetRSTBuf(4,RSDT1.inKRK);
			SetRSTBuf(5,RSDT1.inPRM);
			SetRSTBuf(6,RSDT1.inDDTA);
			SetRSTBuf(7,RSDT1.inMSTR);
			SetRSTBuf(8,RSDT1.inMMAXU);
			SetRSTBuf(9,RSDT1.inMMAXG);
			SetRSTBuf(10,RSDT1.inOPDG);
			SetRSTBuf(11,RSDT1.inWANL);
			SetRSTBuf(12,RSDT1.inKODKT);
			SetRSTBuf(13,RSDT1.inWOBG);
			SetRSTBuf(14,RSDT1.inNOKR);
			SetRSTBuf(15,RSDT1.inTPCS);
			SetRSTBuf(16,RSDT1.inMMIN);
			SetRSTBuf(17,RSDT1.inMMAX);
			SetRSTBuf(18,RSDT1.inRDPM);
			SetRSTBuf(19,RSDT1.inDTPM);
			SetRSTBuf(20,RSDT1.inMDPM);
			SetRSTBuf(21,RSDT1.inTECO);
			SetRSTBuf(22,RSDT1.inTMMAXG2);
			SetRSTBuf(23,RSDT1.inMMAXG2);
            SetRSTBuf(24,RSDT1.inMGPM);
            SetRSTBuf(25,RSDT1.inMGPM2);     
			SetRSTBuf(26,RSDT1.inHSCO);
            SetRSTBuf(27,RSDT1.inCONW);
            SetRSTBuf(28,RSDT1.inPROP);    
            SetRSTBuf(29,RSDT1.inRFAN);              
			SetRSCHK(ndat);					//ndat - CHK z bajtow [0..ndat] i zapisuje na poz. ndat
			//ndat - CHK
			break;
		}
		case _RD_KNF2:				//identyfikator paczki danych
		{
			ndat=40;
            SetExNDatCHK(ndat);            
			SetRSTBuf(0,_UNI02_ADDR);		//0 - adres slave
			//SetRSTBuf(1,chkndat);			//1 - liczba przesylanych bajtow (ndat)
			SetRSTBuf(2,_RD_KNF2);			//2 - identyfikator paczki danych
			//SetRSTBuf(3,chkndat);			//3 - liczba przesylanych bajtow (ndat)            
			SetRSTBuf(4,RSDT1.inKSTAT0);
			SetRSTBuf(5,RSDT1.inKRK);
			SetRSTBuf(6,RSDT1.inPRM);
			SetRSTBuf(7,RSDT1.inDDTA);
			SetRSTBuf(8,RSDT1.inMSTR);
			SetRSTBuf(9,RSDT1.inMMAXU);
			SetRSTBuf(10,RSDT1.inMMAXG);
			SetRSTBuf(11,RSDT1.inOPDG);
			SetRSTBuf(12,RSDT1.inWANL);
			SetRSTBuf(13,RSDT1.inKODKT);
			SetRSTBuf(14,RSDT1.inWOBG);
			SetRSTBuf(15,RSDT1.inNOKR);
			SetRSTBuf(16,RSDT1.inTPCS);
			SetRSTBuf(17,RSDT1.inMMIN);
			SetRSTBuf(18,RSDT1.inMMAX);
			SetRSTBuf(19,RSDT1.inRDPM);
			SetRSTBuf(20,RSDT1.inDTPM);
			SetRSTBuf(21,RSDT1.inMDPM);
			SetRSTBuf(22,RSDT1.inTECO);
			SetRSTBuf(23,RSDT1.inTMMAXG2);
			SetRSTBuf(24,RSDT1.inMMAXG2);
            SetRSTBuf(25,RSDT1.inMGPM);
            SetRSTBuf(26,RSDT1.inMGPM2);   
			SetRSTBuf(27,RSDT1.inHSCO);
            SetRSTBuf(28,RSDT1.inCONW);
            SetRSTBuf(29,RSDT1.inPROP); 
            SetRSTBuf(30,RSDT1.inRFAN); 
            SetRSTBuf(31,RSDT1.inTSTR); 
            SetRSTBuf(32,RSDT1.inTL3);
            SetRSTBuf(33,RSDT1.inPGDR); 
            SetRSTBuf(34,RSDT1.inPGDG); 
            SetRSTBuf(35,RSDT1.inPGDO);    
            SetRSTBuf(36,RSDT1.inPGMX); 
            SetRSTBuf(37,RSDT1.inCWMG);             
            SetRSTBuf(38,RSDT1.inCWTM);             
			SetRSCHK(ndat);					//ndat - CHK z bajtow [0..ndat] i zapisuje na poz. ndat
			//ndat - CHK
			break;
		}                
#if (_EXT_BOARD>=2)                     //aktywowany modul LIN1SLAVE?
		case _RD_LIN:				//identyfikator paczki danych
		{
            switch(RSDT1.inLIDN)
            {                
                case _LIN_RPKNF:                        
                case _LIN_WPKNF:
                {
                    ndat=7;
                    chkndat=SetNDatCHK(ndat);		//oblicz CHK dla liczby danych

                    SetRSTBuf(0,_UNI02_ADDR);		//0 - adres slave
                    SetRSTBuf(1,chkndat);			//1 - liczba przesylanych bajtow (ndat)
                    SetRSTBuf(2,_RD_LIN);			//2 - identyfikator paczki danych
                    SetRSTBuf(3,RSDT1.inLIDN);      //3 - komenda LIN
                    SetRSTBuf(4,RSDT1.inLDTA1);     //4 - parametr1 LIN (indeks)
                    SetRSTBuf(5,RSDT1.inLDTA2);     //5 - parametr1 LIN (wartosc dla _LIN_WPKNF)                            
                    SetRSCHK(ndat);					//ndat - CHK z bajtow [0..ndat] i zapisuje na poz. ndat
                    //ndat - CHK
                    break;
                } 
                case _LIN_WHFLG:                
                case _LIN_GSTAT:                
                case _LIN_GHIST:                
                case _LIN_RPV:                 
                case _LIN_RRCO:                                    
                case _LIN_RRCW:                
                case _LIN_RPP:                                    
                case _LIN_RPI:                        
                case _LIN_RPD: 
                {
                    ndat=5;
                    chkndat=SetNDatCHK(ndat);		//oblicz CHK dla liczby danych

                    SetRSTBuf(0,_UNI02_ADDR);		//0 - adres slave
                    SetRSTBuf(1,chkndat);			//1 - liczba przesylanych bajtow (ndat)
                    SetRSTBuf(2,_RD_LIN);			//2 - identyfikator paczki danych
                    SetRSTBuf(3,RSDT1.inLIDN);      //3 - komenda LIN                            
                    SetRSCHK(ndat);					//ndat - CHK z bajtow [0..ndat] i zapisuje na poz. ndat
                    //ndat - CHK
                    break;
                } 
                case _LIN_SADR:                
                case _LIN_GEEST:                    //parametr=indeks                
                case _LIN_WPV:                
                case _LIN_WRCO1:                                      
                case _LIN_WRCO2:                      
                case _LIN_WRCO3:  
                case _LIN_WRCO4:    
                case _LIN_WRCW1:                                      
                case _LIN_WRCW2:                      
                case _LIN_WRCW3:  
                case _LIN_WRCW4:                    
                {
                    ndat=6;
                    chkndat=SetNDatCHK(ndat);		//oblicz CHK dla liczby danych

                    SetRSTBuf(0,_UNI02_ADDR);		//0 - adres slave
                    SetRSTBuf(1,chkndat);			//1 - liczba przesylanych bajtow (ndat)
                    SetRSTBuf(2,_RD_LIN);			//2 - identyfikator paczki danych
                    SetRSTBuf(3,RSDT1.inLIDN);      //3 - komenda LIN
                    SetRSTBuf(4,RSDT1.inLDTA1);     //4 - parametr1 LIN                        
                    SetRSCHK(ndat);					//ndat - CHK z bajtow [0..ndat] i zapisuje na poz. ndat
                    //ndat - CHK
                    break;
                }                        
                case _LIN_WPP:                                      
                case _LIN_WPI:                      
                case _LIN_WPD:                
                {
                    ndat=7;
                    chkndat=SetNDatCHK(ndat);		//oblicz CHK dla liczby danych

                    SetRSTBuf(0,_UNI02_ADDR);		//0 - adres slave
                    SetRSTBuf(1,chkndat);			//1 - liczba przesylanych bajtow (ndat)
                    SetRSTBuf(2,_RD_LIN);			//2 - identyfikator paczki danych
                    SetRSTBuf(3,RSDT1.inLIDN);      //3 - komenda LIN
                    SetRSTBuf(4,RSDT1.inLDTA1);     //4 - parametr1 LIN (F88)
                    SetRSTBuf(5,RSDT1.inLDTA2);     //5 - parametr1 LIN (F88)                            
                    SetRSCHK(ndat);					//ndat - CHK z bajtow [0..ndat] i zapisuje na poz. ndat
                    //ndat - CHK
                    break;
                }                                     
                default:
                    Nop();
                    break;
            }
            break;
		}
#endif
	}
}
/*---------------------------------------------------------------------------------*/
//Transmisja danych do/z modulu pobudzen UNI01
//gdzie npow - liczba powtorzen prob transmisji w przypadku niepowodzenia
unsigned char TrToUNI01(unsigned char tDTA)
{
	//static unsigned char err;
	//vvv=MGSD.LCN;
	switch(MGSD.KTR)
	{
		case 0:
			MGSD.KTR=1;
		case 1:
			SaveDataToRS(tDTA);
			StartTransData();										//rozpocznij transmisje danych
			MGSD.KTR=2;
		case 2:
			if(MGSDf_TransData) 									//transmisja w toku?
			{
				TransData(1);										//realizuje proces transmisji
				return 0;
			}
			if(MGSDf_TransOK)										//transmisja przebiegla bez problemu?
			{
				RCSTA2bits.CREN=0;												//blokada odbioru na czas analizy danych
				if(VerifyRSACK()&&VerifyRSCHK(UADt.RNData))			//odpowidz odbiornika pozytywna?
				{
					if(xxy++>99) xxy=0;
					MGSD.LCN=0;										//zeruj licznik niepowodzeñ typu timeout
					MGSDf_NO_ODB=0;									//zeruj znacznik braku odbiornika
					MGSDf_TransACK=1;								//ustaw bit potwierdzenia odbioru danych
					SaveRSToData();									//zapisz dane do pamieci
					MGSD.KTR=0;
					RCSTA2bits.CREN=1;
					return 1;
				}
				else
				{
					MGSDf_TransACK=0;								//wyzeruj bit potwierdzenia odbioru danych
				}
				RCSTA2bits.CREN=1;
			}else MGSDf_TransACK=0;									//wyzeruj bit potwierdzenia odbioru danych
			if(zzz++>99) zzz=0;
			if(MGSDf_RERR_TIM||MGSDf_TERR_TIM||MGSDf_RERR_REC) if(MGSD.LCN<_NTXNOODB) MGSD.LCN++;		//zwieksz o 1 licznik niepowodzeñ typu timeout
			if(!MGSDf_RERR_TIM&&!MGSDf_TERR_TIM&&!MGSDf_RERR_REC) MGSD.LCN=0;
			if(MGSD.LCN>_NTONOODB) MGSDf_NO_ODB=1;								//zasygnalizuj brak odbiornika
			else MGSDf_NO_ODB=0;
			if(MGSDf_TERR_TIM||MGSDf_RERR_TIM||MGSDf_RERR_REC)	MGSDf_ERRt=1;	//blad transmisji?
			else MGSDf_ERRt=0;
			MGSD.KTR=0;
			return 2;
	}
    return 2;
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
#endif //(_EXT_BOARD!=0)
