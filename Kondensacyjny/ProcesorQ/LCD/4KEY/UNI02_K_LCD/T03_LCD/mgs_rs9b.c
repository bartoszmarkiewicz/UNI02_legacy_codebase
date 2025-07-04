/*mgs_rs.c*/
//Funkcje obslugi magistrali RS232

/*---------------------------------------------------------------------------------*/
//#include <pic18.h>
#include <xc.h>
#include "global.h"			//parametry globalne
#if (_EXT_BOARD==0)

#include "main.tp.h"
#include "ammy1.h"
#include "uart9b.tp.h"
#include "mgs_rs9b.h"			//parametry lokalne
#include "wersja.h"

volatile MGSRSData MGSD;
//extern tpMMY DtMMY;
//tpAMMY DtAMMY;
//tpAMMY1 DtAMMY1;
extern volatile UARTData UADt;
extern tpRSDTA1 RSDT1;
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
//Inicjalizacja danych zwiazanych z transmisją UART
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
				if(TX1IE)							//trwa nadawanie?
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
			if(!TXSTA1bits.TRMT) break;						//odczekaj na fizyczne zakonczenie transmisji
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
                        else RSDT1.vSTD=1;
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
					case 38:
						RSDT1.vSTD=5;
						break;                        
                    default:
						if(GetExNDat()>38) RSDT1.vSTD=5;
                        else RSDT1.vSTD=5;
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
                RSDT1.outSETPPmax=GetRSRBuf(36);                
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
                        else RSDT1.vKNF=2;
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
                    case 42:
                        RSDT1.vKNF=11;
                        break;                    
                    case 40:
                        RSDT1.vKNF=10;
                        break;                    
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
						if(GetExNDat()>42) RSDT1.vKNF=11;
                        else RSDT1.vKNF=11;
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
                RSDT1.outPWTM=GetRSRBuf(38);       
                
                RSDT1.outTPCW=GetRSRBuf(39);
                RSDT1.outTMPM=GetRSRBuf(40);                
				RSDT1f_RD_KNF=1;
			}
			break;
		}
    }
	//ndat - CHK
}
//Liczba przesylanych bajtow z uwzglednieniem kompatybilnosci wstecznej 
unsigned char GetNDAT(unsigned char n)
{
    if(PFN.vUNI02<17)               //kompatybilnosc wsteczna
    {
        if(n>40) n=40;        //dla sterownikow z oprogramowaniem <17 wielkosc bufora odbioru to 40 bajtow
    }   
    return n;
}
//Przygotowanie danych do wysylki do modulu UNI-02 (razem z obliczonym CHK)
void SaveDataToRS(unsigned char tDTA)
{
	unsigned char ndat;		//liczba danych wysyłanych + CHK
	unsigned char chkndat;

	switch(tDTA)
	{
		case _RD_STD:				//identyfikator paczki danych
		{
			ndat=GetNDAT(11);
			chkndat=SetNDatCHK(ndat);		//oblicz CHK dla liczby danych
		
			SetRSTBuf(0,_UNI02_ADDR);		//0 - adres slave
			SetRSTBuf(1,chkndat);			//1 - liczba przesylanych bajtow (ndat)
			SetRSTBuf(2,_RD_STD);			//2 - identyfikator paczki danych
			SetRSTBuf(3,RSDT1.inSTAT0);
			SetRSTBuf(4,RSDT1.inSTAT1);
			SetRSTBuf(5,RSDT1.inPCO);
			SetRSTBuf(6,RSDT1.inPCW);
			SetRSTBuf(7,RSDT1.inECO);
			SetRSTBuf(8,0);                 //8 - /RSDT1.inLNSTAT/ (wystepowanie bez wzgledu na zawartosc wskazuje na wersje oprogramowania >=7 i zezwala na funkcje odpowietrzenia)   
			SetRSTBuf(9,0);                 //9 -   -------------- (wystepowanie bez wzgledu na zawartosc wskazuje na wersje oprogramowania >=9)             
			SetRSCHK(ndat);					//ndat - CHK z bajtow [0..ndat] i zapisuje na poz. ndat
			//ndat - CHK
			break;
		}
		case _RD_STD2:				//identyfikator paczki danych
		{
			ndat=GetNDAT(15);
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
			SetRSTBuf(10,0);                //10- /RSDT1.inINSTAT/                
			SetRSTBuf(11,0);                //11- /RSDT1.inLNSTAT/             
            SetRSTBuf(12,PFN.PCO);          //nastawiona wartosc PCO  
			SetRSTBuf(13,0);                //13- /RSDT1.inSTAT2/      
			SetRSCHK(ndat);					//ndat - CHK z bajtow [0..ndat] i zapisuje na poz. ndat
			//ndat - CHK
			break;
		}        
		case _RD_KNF:				//identyfikator paczki danych
		{
			ndat=GetNDAT(31);
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
			ndat=GetNDAT(43);
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
            SetRSTBuf(39,RSDT1.inPWTM);  
            
            SetRSTBuf(40,RSDT1.inTPCW);      
            SetRSTBuf(41,RSDT1.inTMPM);            
			SetRSCHK(ndat);					//ndat - CHK z bajtow [0..ndat] i zapisuje na poz. ndat
			//ndat - CHK
			break;
		}        
	}
}
/*---------------------------------------------------------------------------------*/
//Transmisja danych do/z modulu pobudzen UNI01
//gdzie npow - liczba powtorzen prob transmisji w przypadku niepowodzenia
void TrToUNI01(unsigned char tDTA)
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
				return;
			}
			if(MGSDf_TransOK)										//transmisja przebiegla bez problemu?
			{
				RCSTA1bits.CREN=0;												//blokada odbioru na czas analizy danych
				if(VerifyRSACK()&&VerifyRSCHK(UADt.RNData))			//odpowidz odbiornika pozytywna?
				{
					if(xxy++>99) xxy=0;
					MGSD.LCN=0;										//zeruj licznik niepowodzeń typu timeout
					MGSDf_NO_ODB=0;									//zeruj znacznik braku odbiornika
					MGSDf_TransACK=1;								//ustaw bit potwierdzenia odbioru danych
					SaveRSToData();									//zapisz dane do pamieci
					MGSD.KTR=0;
					ZtnToNext(0);
					RCSTA1bits.CREN=1;
					return;
				}
				else
				{
					MGSDf_TransACK=0;								//wyzeruj bit potwierdzenia odbioru danych
				}
				RCSTA1bits.CREN=1;
			}else MGSDf_TransACK=0;									//wyzeruj bit potwierdzenia odbioru danych
			if(zzz++>99) zzz=0;
			if(MGSDf_RERR_TIM||MGSDf_TERR_TIM||MGSDf_RERR_REC) if(MGSD.LCN<_NTXNOODB) MGSD.LCN++;		//zwieksz o 1 licznik niepowodzeń typu timeout
			if(!MGSDf_RERR_TIM&&!MGSDf_TERR_TIM&&!MGSDf_RERR_REC) MGSD.LCN=0;
			if(MGSD.LCN>_NTONOODB) MGSDf_NO_ODB=1;								//zasygnalizuj brak odbiornika
			else MGSDf_NO_ODB=0;
			if(MGSDf_TERR_TIM||MGSDf_RERR_TIM||MGSDf_RERR_REC)	MGSDf_ERRt=1;	//blad transmisji?
			else MGSDf_ERRt=0;
			MGSD.KTR=0;
			ZtnToNext(0);				
			break;
	}
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
#endif //(_EXT_BOARD==0)