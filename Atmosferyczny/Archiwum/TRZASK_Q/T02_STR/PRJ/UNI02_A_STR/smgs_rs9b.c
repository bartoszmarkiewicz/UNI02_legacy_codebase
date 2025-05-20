/*smgs_rs.c*/
//Funkcje obslugi magistrali RS232

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

#include "smgs_rs9b.h"			//parametry lokalne
#include "konfig.h"				

extern UARTData UADt;
extern tpRSDTA1 RSDT1;
extern tpMAIN M;
extern DtKONF DtKNF;
extern DtMINMAX MinMax[];

MGSRSData MGSD;
unsigned char xxy,zzz,vvv,mmm;

/*---------------------------------------------------------------------------------*/
//Rejestracja odebranych danych z modu?u LCD
void SaveRSToData(void)
{
	unsigned char nbty,bf;
    
	//0 - adres slave
	//1 - liczba przesylanych bajtow (ndat)
	//2 - identyfikator paczki danych
	RSDT1.outID=GetRSRBuf(2);
	switch(RSDT1.outID)
	{
		case _RD_STD:				//identyfikator paczki danych
		{
            //(RAMKA DLA WERSJI OPROGRAMOWANIA<=10)
			RSDT1.RD_STD=0;
            nbty=GetNDat();

			if(nbty>=9) RSDT1.vSTD=1;	//nowa wersja wyswitlacza lub wersja oprogramowania >10
			else RSDT1.vSTD=0;	

			RSDT1.inSTAT0=GetRSRBuf(3);
			RSDT1.inSTAT1=GetRSRBuf(4);
			RSDT1.inPCO=GetRSRBuf(5);
			RSDT1.inPCW=GetRSRBuf(6);
            RSDT1.inINSTAT=0;
            RSDT1.inLNSTAT=0;  
            RSDT1.inSTAT2=0;            
            RSDT1.inRDZ0=0;            
			RSDT1.RD_STD=1;
			break;
		}
		case _RD_STD2:				//identyfikator paczki danych
		{
            //(RAMKA DLA WERSJI OPROGRAMOWANIA>=11)
			RSDT1.RD_STD=0;
            nbty=GetExNDat();

            switch(nbty)
            {
                case 14:
                    RSDT1.vSTD=2;
                    break;
                default:
                    if(nbty>=14) RSDT1.vSTD=2;
                    else RSDT1.vSTD=2;              //wersja ramki danych
                    break;
            }            
			RSDT1.inRDZ0=GetRSRBuf(4);              //wersja plytki wyswietlacza   
			RSDT1.inSTAT0=GetRSRBuf(5);
			RSDT1.inSTAT1=GetRSRBuf(6);
			RSDT1.inPCO=GetRSRBuf(7);
			RSDT1.inPCW=GetRSRBuf(8);
			RSDT1.inINSTAT=GetRSRBuf(9);  
            RSDT1.inLNSTAT=GetRSRBuf(10);
            RSDT1.inCONTSETP=GetRSRBuf(11);   
            RSDT1.inSTAT2=GetRSRBuf(12);            
#if (_EXT_BOARD==0)            
            RSDT1.inINSTAT=0; 
            RSDT1.inLNSTAT=0;            
            RSDT1.inSTAT2=0;            
#endif            
#if (_EXT_BOARD<2)            
            RSDT1.inLNSTAT=0;
            RSDT1.inSTAT2=0;
#endif            
			RSDT1.RD_STD=1; 
			break;
		}        
		case _RD_KNF:				//identyfikator paczki danych
		{
            //(RAMKA DLA WERSJI OPROGRAMOWANIA<=10)
			if(!RSDT1.RRD_KNF)		//aktywna blokada na czas interpretacji danych?
			{
				RSDT1.RD_KNF=0;
				switch(GetRSRBuf(1)&0b00011111)	//liczba przeslanych bajtow
				{
					case 17:
						RSDT1.vKNF=1;
						break;
					case 21:
						RSDT1.vKNF=2;
						break;
					case 24:
						RSDT1.vKNF=3;
						break;
					default:
                        if((GetRSRBuf(1)&0b00011111)>24) RSDT1.vKNF=3;
						else RSDT1.vKNF=1;
						break;												
				}
				RSDT1.inKSTAT0=GetRSRBuf(3);
				RSDT1.inKRK=GetRSRBuf(4);
				RSDT1.inPRM=GetRSRBuf(5);
				RSDT1.inDDTA=GetRSRBuf(6);
				RSDT1.inMSTR=GetRSRBuf(7);
				RSDT1.inMMAXU=GetRSRBuf(8);
				RSDT1.inMMAXG=GetRSRBuf(9);
				RSDT1.inRGZ=GetRSRBuf(10);
				RSDT1.inWANL=GetRSRBuf(11);
				RSDT1.inTPKT=GetRSRBuf(12);
				RSDT1.inWOBG=GetRSRBuf(13);
				RSDT1.inOPDG=GetRSRBuf(14);
				RSDT1.inTPCS=GetRSRBuf(15);

				RSDT1.inRDPM=GetRSRBuf(16);
				RSDT1.inDTPM=GetRSRBuf(17);
				RSDT1.inMDPM=GetRSRBuf(18);
				RSDT1.inMGPM=GetRSRBuf(19); 

				RSDT1.inMGPM2=GetRSRBuf(20); 
				RSDT1.inHSCO=GetRSRBuf(21);
				RSDT1.inCONW=GetRSRBuf(22); 
				RSDT1.RD_KNF=1;
			}
			break;
		}
		case _RD_KNF2:				//identyfikator paczki danych
		{
            //(RAMKA DLA WERSJI OPROGRAMOWANIA>=11)
			if(!RSDT1.RRD_KNF)		//aktywna blokada na czas interpretacji danych?
			{
				RSDT1.RD_KNF=0;
                nbty=GetExNDat();
				switch(nbty)	//liczba przeslanych bajtow
				{
					case 30:
						RSDT1.vKNF=5;
						break;                    
					case 27:
						RSDT1.vKNF=4;
						break;                    
					case 25:
						RSDT1.vKNF=3;
						break;
					default:
                        if(nbty>30) RSDT1.vKNF=5;
						else RSDT1.vKNF=5;
						break;												
				}
				RSDT1.inKSTAT0=GetRSRBuf(4);
				RSDT1.inKRK=GetRSRBuf(5);
				RSDT1.inPRM=GetRSRBuf(6);
				RSDT1.inDDTA=GetRSRBuf(7);
				RSDT1.inMSTR=GetRSRBuf(8);
				RSDT1.inMMAXU=GetRSRBuf(9);
				RSDT1.inMMAXG=GetRSRBuf(10);    
				RSDT1.inRGZ=GetRSRBuf(11);
				RSDT1.inWANL=GetRSRBuf(12);
				RSDT1.inTPKT=GetRSRBuf(13);
				RSDT1.inWOBG=GetRSRBuf(14);
				RSDT1.inOPDG=GetRSRBuf(15);
				RSDT1.inTPCS=GetRSRBuf(16);

				RSDT1.inRDPM=GetRSRBuf(17);
				RSDT1.inDTPM=GetRSRBuf(18);
				RSDT1.inMDPM=GetRSRBuf(19);
				RSDT1.inMGPM=GetRSRBuf(20); 

				RSDT1.inMGPM2=GetRSRBuf(21); 
				RSDT1.inHSCO=GetRSRBuf(22);
				RSDT1.inCONW=GetRSRBuf(23);
				RSDT1.inPROP=GetRSRBuf(24);                
				RSDT1.inPGMX=GetRSRBuf(25);
				RSDT1.inPGDR=GetRSRBuf(26); 
				RSDT1.inCWMG=GetRSRBuf(27);                
				RSDT1.inCWTM=GetRSRBuf(28);                
				RSDT1.RD_KNF=1;
			}
			break;
		}
#if (_EXT_BOARD>1)
		//FAZA 1 - odczyt polecenia z plytki LCD
		case _RD_LIN:                                           //identyfikator paczki danych
		{
            //xxy=1;
		    RSDT1.RD_LIN=0;
		    RSDT1.inLIDN=GetRSRBuf(3);
		    switch(RSDT1.inLIDN)                                //komenda LIN
		    {
		        //---------------GRUPA NROZKAZOW TYPU ODCZYT
		        case _LIN_GKNF:     //pobierz paczke konfiuracyjna
		        {
		            break;          //(wywolanie bez parametrow)
		        }
		        case _LIN_RPKNF:     //pobierz parametr konfiuracyjny
		        {
                    //xxy++;
					RSDT1.inLDTA1=GetRSRBuf(4);                //(wywolanie z parametrem)
                    //xxy=RSDT1.inLDTA1;
					RSDT1.outPKERR=0;
					if(RSDT1.inLDTA1<_KNPAR1)
					{
						RSDT1.outRPKNF=ReadEEPROM(MinMax[RSDT1.inLDTA1].addr);
					}
					else RSDT1.outPKERR=1;
		            break;          //(wywolanie bez parametrow)
		        }
		        //---------------GRUPA NROZKAZOW TYPU MODYFIKACJA
		        case _LIN_WPKNF:     //zapisz parametr konfiuracyjny
		        {
					RSDT1.inLDTA1=GetRSRBuf(4);                //(wywolanie z parametrem)
					RSDT1.inLDTA2=GetRSRBuf(5);                //(wywolanie z parametrem)
					RSDT1.outPKERR=0;
					if(RSDT1.inLDTA1<_KNPAR1)
					{
						if((RSDT1.inLDTA2>=MinMax[RSDT1.inLDTA1].min)&&(RSDT1.inLDTA2<=MinMax[RSDT1.inLDTA1].max))
						{
							switch(RSDT1.inLDTA1)
							{
								case 4:     //4 aktywacja zegara antylegionella
								{
									if(RSDT1.inLDTA2!=ReadEEPROM(MinMax[RSDT1.inLDTA1].addr)) 
									{
										StartRTG(_RTANL);
										ClearEETA();	//zeruj pamiec timera zasobnika					 											
										if_WriteEEPROM(MinMax[RSDT1.inLDTA1].addr,RSDT1.inLDTA2);
									}
									break;
								}
								case 11:    //11 minimalny wydatek pompy
								{
									if(RSDT1.inLDTA2!=ReadEEPROM(MinMax[RSDT1.inLDTA1].addr)) 
									{
										bf=ReadEEPROM(MinMax[12].addr);
					                    if(RSDT1.inLDTA2>bf)
					                    {
					                        if(bf>=MinMax[RSDT1.inLDTA1].min)
											{
			
											 	if_WriteEEPROM(MinMax[RSDT1.inLDTA1].addr,bf);
												break;
											}
					                    }
					                    if_WriteEEPROM(MinMax[RSDT1.inLDTA1].addr,RSDT1.inLDTA2);
									}
									break;
								}
								case 12:    //12 maksymalny wydatek pompy
								{
									if(RSDT1.inLDTA2!=ReadEEPROM(MinMax[RSDT1.inLDTA1].addr)) 
									{
										bf=ReadEEPROM(MinMax[11].addr);
					                    if(RSDT1.inLDTA2<bf)
					                    {
					                        if(bf<=MinMax[RSDT1.inLDTA1].max)
											{
			
											 	if_WriteEEPROM(MinMax[RSDT1.inLDTA1].addr,bf);
												break;
											}
					                    }
					                    if_WriteEEPROM(MinMax[RSDT1.inLDTA1].addr,RSDT1.inLDTA2);
									}
									break;
								}
                                case 18:    //18 tryb pracy regulatora pogodowego
                                {
									if(RSDT1.inLDTA2!=ReadEEPROM(MinMax[RSDT1.inLDTA1].addr))      
									{					 											
										if_WriteEEPROM(MinMax[RSDT1.inLDTA1].addr,RSDT1.inLDTA2);
                                        StartRTS(_RTCPGD);
                                        M._SYNCH_PGD=1;             //ustaw flage aktualizacji nastawy w plytce wyswietlacza
									}                                            
                                    break;
                                }                                
								default:
									if(RSDT1.inLDTA2!=ReadEEPROM(MinMax[RSDT1.inLDTA1].addr)) if_WriteEEPROM(MinMax[RSDT1.inLDTA1].addr,RSDT1.inLDTA2);
								break;
							}
							RdEEParam(); //aktualizuj dane konfiguracyjne
							RSDT1.outWPKNF=ReadEEPROM(MinMax[RSDT1.inLDTA1].addr); //odczytaj wartosc modyfikowanego parametru
						}else RSDT1.outPKERR=1;						    
					}else RSDT1.outPKERR=1;
					break;
		        }
		        case _LIN_GSTAT:     //pobierz statystyki zdarzen awaryjnych
		        {
		            break;          //(wywolanie bez parametrow)
		        }                
		        case _LIN_GEEST:     //pobierz status wylaczenia awaryjnego z blokada
		        {
                    RSDT1.inLDTA1=GetRSRBuf(4);                //(wywolanie z parametrem)
		            break;          //(wywolanie bez parametrow)
		        }
		        case _LIN_GHIST:     //pobierz historie zdarzen awaryjnych
		        {
		            break;          //(wywolanie bez parametrow)
		        } 
		        case _LIN_WGDZ:     //wyzeruj licznik godzin pracy
		        {
                    SetRGDZ(0);
                    M.RGDZ=0;
                    M.RMDZ=0;
                    M.RSDZ=0;
		            break;          //(wywolanie bez parametrow)
		        }                
		        case _LIN_WHFLG:     //wyzeruj licznik flagi serwisowej
		        {
                    ClrHFLG();      //zerowanie licznika
		            break;          //(wywolanie bez parametrow)
		        }                
		        case _LIN_SADR:     //zmien adres LIN 
		        {
		            RSDT1.inLDTA1=GetRSRBuf(4);                //(wywolanie z parametrem)
		            if((RSDT1.inLDTA1>=1)&&(RSDT1.inLDTA1<=5))
		            {
		                if(RSDT1.inLDTA1!=ReadEEALIN())
		                {
		                    WriteEEALIN(RSDT1.inLDTA1);
		                }
		            }
		            DtKNF.alin=ReadEEALIN();
		            RSDT1.outALIN=DtKNF.alin;
		            break;
		        }
		    }
		    RSDT1.RD_LIN=1;
		    break;
		}
#endif        
	}
	//ndat - CHK
}
/*---------------------------------------------------------------------------------*/
//Przygotowanie danych do wysylki do modulu LCD
void SaveDataToRS(unsigned char tDTA)
{
	volatile unsigned char ndat,i,nr;		//liczba danych wysy?anych + CHK
	unsigned char chkndat;

	//const unsigned char ndat=19;		//liczba danych wysy?anych (+ CHK)
	//unsigned char chkndat;
	switch(tDTA)
	{
		case _RD_ERR:							//identyfikator paczki danych
		{
			ndat=4;								//liczba danych wysy?anych (+ CHK)
			chkndat=SetNDatCHK(ndat);			//oblicz CHK dla liczby danych
		
			SetRSTBuf(0,_LCD_ADDR);				//0 - adres master
			SetRSTBuf(1,chkndat);				//1 - liczba przesy?anych danych (ndat)
			SetRSTBuf(2,_RD_ERR);				//2 - identyfikator paczki danych
			//ndat - CHK
			break;
		}
		case _RD_STD:				//identyfikator paczki danych
		{
			ndat=25+1;		//liczba danych wysylanych (+ CHK), dodatkowy pusty bajt jako znacznik gotowosci dla ramek tyu 2
			chkndat=SetNDatCHK(ndat);			//oblicz CHK dla liczby danych
		
			SetRSTBuf(0,_LCD_ADDR);				//0 - adres master
			SetRSTBuf(1,chkndat);				//1 - liczba przesy?anych danych (ndat)
			SetRSTBuf(2,_RD_STD);				//3 - identyfikator paczki danych
			SetRSTBuf(3,RSDT1.outRDZ0);
			SetRSTBuf(4,RSDT1.outRDZ1);
			SetRSTBuf(5,RSDT1.outSTAT0);
			SetRSTBuf(6,RSDT1.outSTAT1);
			SetRSTBuf(7,RSDT1.outSTAT2);
			SetRSTBuf(8,RSDT1.outCO);
			SetRSTBuf(9,RSDT1.outCW);
			SetRSTBuf(10,RSDT1.outCS);
			SetRSTBuf(11,RSDT1.outPCO);
			SetRSTBuf(12,RSDT1.outPCOmn);
			SetRSTBuf(13,RSDT1.outPCOmx);
			SetRSTBuf(14,RSDT1.outPCW);
			SetRSTBuf(15,RSDT1.outPCWmn);
			SetRSTBuf(16,RSDT1.outPCWmx);
			SetRSTBuf(17,RSDT1.outPPCO);
			SetRSTBuf(18,RSDT1.outPPCW);
			SetRSTBuf(19,RSDT1.outERR);
			SetRSTBuf(20,RSDT1.outAWR);

			SetRSTBuf(21,RSDT1.outSTAT3);
			SetRSTBuf(22,RSDT1.outIN);
			SetRSTBuf(23,RSDT1.outPP);
			SetRSTBuf(24,0x00);                 //dodatkowy pusty bajt jako znacznik gotowosci dla ramek tyu 2 (!konieczny do konfiguracji kolejki wyswietlacza)        
			SetRSCHK(ndat);			//ndat - CHK z bajtow [0..ndat] i zapisuje na poz. ndat
			//ndat - CHK
			break;
		}
		case _RD_STD2:                          //identyfikator paczki danych            
		{
			ndat=35;                            //liczba danych wysy?anych (+ CHK)
            SetExNDatCHK(ndat); 
			SetRSTBuf(0,_LCD_ADDR);				//0 - adres master           
			//SetRSTBuf(1,chkndat);				//1 - liczba przesy?anych danych (ndat)
			SetRSTBuf(2,_RD_STD2);				//2 - identyfikator paczki danych
            //SetRSTBuf(3,chkndat);             //3 - liczba przesy?anych danych (ndat)
			SetRSTBuf(4,RSDT1.outRDZ0);
			SetRSTBuf(5,RSDT1.outRDZ1);
			SetRSTBuf(6,RSDT1.outRDZ2);            
			SetRSTBuf(7,RSDT1.outSTAT0);
			SetRSTBuf(8,RSDT1.outSTAT1);
			SetRSTBuf(9,RSDT1.outSTAT2);
			SetRSTBuf(10,RSDT1.outSTAT3);   
			SetRSTBuf(11,RSDT1.outSTAT4);
			SetRSTBuf(12,RSDT1.outSTAT5);            
			SetRSTBuf(13,RSDT1.outCO);			//filtrowana wartosc temp
			SetRSTBuf(14,RSDT1.outCW);			//filtrowana wartosc temp
			SetRSTBuf(15,RSDT1.outCS);
			SetRSTBuf(16,RSDT1.outIN);
			SetRSTBuf(17,RSDT1.outPCO);
			SetRSTBuf(18,RSDT1.outPCOmn);
			SetRSTBuf(19,RSDT1.outPCOmx);
			SetRSTBuf(20,RSDT1.outPCW);
			SetRSTBuf(21,RSDT1.outPCWmn);
			SetRSTBuf(22,RSDT1.outPCWmx);
			SetRSTBuf(23,RSDT1.outPPCO);
			SetRSTBuf(24,RSDT1.outPPCW);
			SetRSTBuf(25,RSDT1.outERR);
			SetRSTBuf(26,RSDT1.outAWR);
			SetRSTBuf(27,RSDT1.outPP);
			SetRSTBuf(28,RSDT1.outPG);
			SetRSTBuf(29,RSDT1.outrCO);			//rzeczywista wartosc temp
			SetRSTBuf(30,RSDT1.outrCW);			//rzeczywista wartosc temp
			SetRSTBuf(31,RSDT1.outZW);		
			SetRSTBuf(32,RSDT1.outSETPmin);		
			SetRSTBuf(33,RSDT1.outSETPmax);            
			SetRSCHK(ndat);			//ndat - CHK z bajtow [0..ndat] i zapisuje na poz. ndat
			//ndat - CHK
			break;
		}        
		case _RD_KNF:				//identyfikator paczki danych
		{
			ndat=31;//32;		//liczba danych wysy?anych (+ CHK)
			chkndat=SetNDatCHK(ndat);			//oblicz CHK dla liczby danych
		
			SetRSTBuf(0,_LCD_ADDR);				//0 - adres master
			SetRSTBuf(1,chkndat);				//1 - liczba przesy?anych danych (ndat)
			SetRSTBuf(2,_RD_KNF);				//2 - identyfikator paczki danych
			SetRSTBuf(3,RSDT1.outKSTAT0);
			SetRSTBuf(4,RSDT1.outMSTR);
			SetRSTBuf(5,_KMINMST);
			SetRSTBuf(6,_KMAXMST);
			SetRSTBuf(7,RSDT1.outMMAXU);
			SetRSTBuf(8,_KMINMCW);
			SetRSTBuf(9,_KMAXMCW);
			SetRSTBuf(10,RSDT1.outMMAXG);
			SetRSTBuf(11,_KMINMCO);
			SetRSTBuf(12,_KMAXMCO);
			SetRSTBuf(13,RSDT1.outRGZ);
			SetRSTBuf(14,_KMINRGZ);
			SetRSTBuf(15,_KMAXRGZ);
			SetRSTBuf(16,RSDT1.outWANL);
			SetRSTBuf(17,RSDT1.outTPKT);
			SetRSTBuf(18,_KMINWTK);
			SetRSTBuf(19,_KMAXWTK);
			SetRSTBuf(20,RSDT1.outWOBG);
			SetRSTBuf(21,RSDT1.outOPDG);
			SetRSTBuf(22,RSDT1.outTPCS);

			SetRSTBuf(23,RSDT1.outRDPM);
			SetRSTBuf(24,RSDT1.outDTPM);
			SetRSTBuf(25,RSDT1.outMDPM);
			SetRSTBuf(26,RSDT1.outMGPM);

			SetRSTBuf(27,RSDT1.outMGPM2); 
			SetRSTBuf(28,RSDT1.outHSCO);
			SetRSTBuf(29,RSDT1.outCONW);
			//SetRSTBuf(30,RSDT1.outPROP);

			SetRSCHK(ndat);			//ndat - CHK z bajtow [0..ndat] i zapisuje na poz. ndat
			//ndat - CHK
			break;
		}
		case _RD_KNF2:                                          //identyfikator paczki danych            
		{
			ndat=29;                            //liczba danych wysy?anych (+ CHK)
            SetExNDatCHK(ndat);            
			SetRSTBuf(0,_LCD_ADDR);				//0 - adres master            
			//SetRSTBuf(1,chkndat);				//1 - liczba przesy?anych danych (ndat)
			SetRSTBuf(2,_RD_KNF2);				//2 - identyfikator paczki danych
            //SetRSTBuf(3,chkndat);             //3 - liczba przesy?anych danych (ndat)          
			SetRSTBuf(4,RSDT1.outKSTAT0);       
			SetRSTBuf(5,RSDT1.outKRK);
			SetRSTBuf(6,RSDT1.outPRM);
			SetRSTBuf(7,RSDT1.outMSTR);
			SetRSTBuf(8,RSDT1.outMMAXU);
			SetRSTBuf(9,RSDT1.outMMAXG);
			SetRSTBuf(10,RSDT1.outRGZ);
			SetRSTBuf(11,RSDT1.outWANL);
			SetRSTBuf(12,RSDT1.outTPKT);
			SetRSTBuf(13,RSDT1.outWOBG);
			SetRSTBuf(14,RSDT1.outOPDG);
			SetRSTBuf(15,RSDT1.outTPCS);
			SetRSTBuf(16,RSDT1.outRDPM);
			SetRSTBuf(17,RSDT1.outDTPM);
			SetRSTBuf(18,RSDT1.outMDPM);
			SetRSTBuf(19,RSDT1.outMGPM);
			SetRSTBuf(20,RSDT1.outMGPM2); 
			SetRSTBuf(21,RSDT1.outHSCO);
			SetRSTBuf(22,RSDT1.outCONW);  
			SetRSTBuf(23,RSDT1.outPROP);            
            SetRSTBuf(24,RSDT1.outPGMX); 
            SetRSTBuf(25,RSDT1.outPGDR);  
            SetRSTBuf(26,RSDT1.outCWMG);            
            SetRSTBuf(27,RSDT1.outCWTM);            
			SetRSCHK(ndat);			//ndat - CHK z bajtow [0..ndat] i zapisuje na poz. ndat
			//ndat - CHK
			break;
		}  
#if (_EXT_BOARD>1)
		//FAZA 2 - odpowiedz do plytki LCD
		case _RD_LIN:                                           //identyfikator paczki danych
		{
		    switch(RSDT1.inLIDN)                                //komenda LIN
		    {
		        //---------------GRUPA NROZKAZOW TYPU ODCZYT
		        /*case _LIN_GKNF:         //pobierz paczke konfiuracyjna
		        {
		            ndat=25;                                            //liczba danych wysy?anych (+ CHK)
		            chkndat=SetNDatCHK(ndat);                           //oblicz CHK dla liczby danych
		            SetRSTBuf(0,_LCD_ADDR);				//0 - adres master
		            SetRSTBuf(1,chkndat);				//1 - liczba przesy?anych danych (ndat)
		            SetRSTBuf(2,_RD_LIN);				//2 - identyfikator paczki danych
		            SetRSTBuf(3,_LIN_GKNF);             //3 - identyfikator komendy LIN
                    if(ndat>31)
                    {
                        chkndat=SetNDatCHK(ndat-31);			//oblicz CHK dla liczby danych    
                    }
                    else chkndat=0;                     
                    SetRSTBuf(4,chkndat);               //4 - liczba przesylanych danych	
                    SetRSTBuf(5,RSDT1.outKSTAT0);       
                    SetRSTBuf(6,RSDT1.outKRK);
                    SetRSTBuf(7,RSDT1.outPRM);
                    SetRSTBuf(8,RSDT1.outMSTR);
                    SetRSTBuf(9,RSDT1.outMMAXU);
                    SetRSTBuf(10,RSDT1.outMMAXG);
                    SetRSTBuf(11,RSDT1.outRGZ);
                    SetRSTBuf(12,RSDT1.outWANL);
                    SetRSTBuf(13,RSDT1.outTPKT);
                    SetRSTBuf(14,RSDT1.outWOBG);
                    SetRSTBuf(15,RSDT1.outOPDG);
                    SetRSTBuf(16,RSDT1.outTPCS);
                    SetRSTBuf(17,RSDT1.outRDPM);
                    SetRSTBuf(18,RSDT1.outDTPM);
                    SetRSTBuf(19,RSDT1.outMDPM);
                    SetRSTBuf(20,RSDT1.outMGPM);
                    SetRSTBuf(21,RSDT1.outMGPM2); 
                    SetRSTBuf(22,RSDT1.outHSCO);
                    SetRSTBuf(23,RSDT1.outCONW);                    
		            SetRSCHK(ndat);			//ndat - CHK z bajtow [0..ndat] i zapisuje na poz. ndat
		            //ndat - CHK
		            break;
		        }*/
				case _LIN_RPKNF:          //odczytaj wartosc parametru konfiguracyjnego
				{
		            ndat=7;                             //liczba danych wysy?anych (+ CHK)
		            chkndat=SetNDatCHK(ndat);           //oblicz CHK dla liczby danych
		            SetRSTBuf(0,_LCD_ADDR);				//0 - adres master
		            SetRSTBuf(1,chkndat);				//1 - liczba przesy?anych danych (ndat)
		            SetRSTBuf(2,_RD_LIN);				//2 - identyfikator paczki danych
		            SetRSTBuf(3,_LIN_RPKNF);          	//3 - identyfikator komendy LIN
					SetRSTBuf(4,RSDT1.outRPKNF);		//4 - wartosc parametru konfiguracji
					SetRSTBuf(5,RSDT1.outPKERR);		//5 - status operacji
					SetRSCHK(ndat);						//ndat - CHK z bajtow [0..ndat] i zapisuje na poz. ndat
	            	//ndat - CHK	
					RSDT1.outPKERR=0;			
					break;
				}
				case _LIN_WPKNF:          //odczytaj wartosc parametru konfiguracyjnego
				{
		            ndat=7;                             //liczba danych wysy?anych (+ CHK)
		            chkndat=SetNDatCHK(ndat);           //oblicz CHK dla liczby danych
		            SetRSTBuf(0,_LCD_ADDR);				//0 - adres master
		            SetRSTBuf(1,chkndat);				//1 - liczba przesy?anych danych (ndat)
		            SetRSTBuf(2,_RD_LIN);				//2 - identyfikator paczki danych
		            SetRSTBuf(3,_LIN_WPKNF);          	//3 - identyfikator komendy LIN
					SetRSTBuf(4,RSDT1.outWPKNF);		//4 - wartosc parametru konfiguracji (wartosc parametru po zapisie)
					SetRSTBuf(5,RSDT1.outPKERR);		//5 - status operacji
					SetRSCHK(ndat);						//ndat - CHK z bajtow [0..ndat] i zapisuje na poz. ndat
	            	//ndat - CHK	
					RSDT1.outPKERR=0;			
					break;
				}
                case _LIN_GSTAT:         //status wylaczenia awaryjnego z blokada
		        {
                    ndat=10;                                            //liczba danych wysy?anych (+ CHK)
                    chkndat=SetNDatCHK(ndat);                           //oblicz CHK dla liczby danych
                    SetRSTBuf(0,_LCD_ADDR);				//0 - adres master
                    SetRSTBuf(1,chkndat);				//1 - liczba przesy?anych danych (ndat)
                    SetRSTBuf(2,_RD_LIN);				//2 - identyfikator paczki danych
                    SetRSTBuf(3,_LIN_GSTAT);            //3 - identyfikator komendy LIN  
                    SetRSTBuf(4,ReadBtyHREF(0));        //4 - L ogolna liczba zdzarzen awaryjnych (nr ostatniego zdarzenia awaryjnego)
                    SetRSTBuf(5,ReadBtyHREF(1));        //5 - H ogolna liczba zdzarzen awaryjnych (nr ostatniego zdarzenia awaryjnego)
                    SetRSTBuf(6,ReadBtyNZSTAT(0));      //6 - L liczba zdzarzen awaryjnych z blokada
                    SetRSTBuf(7,ReadBtyNZSTAT(1));      //7 - H liczba zdzarzen awaryjnych z blokada      
                    SetRSTBuf(8,ReadBtyHFLG());         //8 - licznik flagi serwisowej                    
		            SetRSCHK(ndat);			//ndat - CHK z bajtow [0..ndat] i zapisuje na poz. ndat
		            //ndat - CHK
		            break; 
		        }                
		        case _LIN_GEEST:         //status wylaczenia awaryjnego z blokada
		        {
                    nr=RSDT1.inLDTA1;                      
                    if(nr>_NSTBL-1||(!ReadBtySTAT(nr,0)&&!ReadBtySTAT(nr,1)))      //zanotowano wylaczenie awaryjne z blokada?
                    {
                        ndat=7;                                            //liczba danych wysy?anych (+ CHK)
                        chkndat=SetNDatCHK(ndat);                           //oblicz CHK dla liczby danych
                        SetRSTBuf(0,_LCD_ADDR);				//0 - adres master
                        SetRSTBuf(1,chkndat);				//1 - liczba przesy?anych danych (ndat)
                        SetRSTBuf(2,_RD_LIN);				//2 - identyfikator paczki danych
                        SetRSTBuf(3,_LIN_GEEST);             //3 - identyfikator komendy LIN  
                        SetRSTBuf(4,0);
                        SetRSTBuf(5,0);  
                    }
                    else
                    {
                        ndat=27;                                            //liczba danych wysy?anych (+ CHK)
                        chkndat=SetNDatCHK(ndat);                           //oblicz CHK dla liczby danych
                        SetRSTBuf(0,_LCD_ADDR);				//0 - adres master
                        SetRSTBuf(1,chkndat);				//1 - liczba przesy?anych danych (ndat)
                        SetRSTBuf(2,_RD_LIN);				//2 - identyfikator paczki danych
                        SetRSTBuf(3,_LIN_GEEST);             //3 - identyfikator komendy LIN
                        SetRSTBuf(4,ReadBtySTAT(nr,0));
                        SetRSTBuf(5,ReadBtySTAT(nr,1));  
                        SetRSTBuf(6,ReadBtySTAT(nr,2));
                        SetRSTBuf(7,ReadBtySTAT(nr,3));
                        SetRSTBuf(8,ReadBtySTAT(nr,4));
                        SetRSTBuf(9,ReadBtySTAT(nr,5));   
                        SetRSTBuf(10,ReadBtySTAT(nr,6));
                        SetRSTBuf(11,ReadBtySTAT(nr,7));            
                        SetRSTBuf(12,ReadBtySTAT(nr,8));
                        SetRSTBuf(13,ReadBtySTAT(nr,9));
                        SetRSTBuf(14,ReadBtySTAT(nr,10));
                        SetRSTBuf(15,ReadBtySTAT(nr,11));			//rzeczywista wartosc temp
                        SetRSTBuf(16,ReadBtySTAT(nr,12));			//rzeczywista wartosc temp    
                        SetRSTBuf(17,ReadBtySTAT(nr,13));
                        SetRSTBuf(18,ReadBtySTAT(nr,14));
                        SetRSTBuf(19,ReadBtySTAT(nr,15));
                        SetRSTBuf(20,ReadBtySTAT(nr,16));
                        SetRSTBuf(21,ReadBtySTAT(nr,17));
                        SetRSTBuf(22,ReadBtySTAT(nr,18));
                        SetRSTBuf(23,ReadBtySTAT(nr,19));
                        SetRSTBuf(24,ReadBtySTAT(nr,20));
                        SetRSTBuf(25,ReadBtySTAT(nr,21));
                    }
		            SetRSCHK(ndat);			//ndat - CHK z bajtow [0..ndat] i zapisuje na poz. ndat
		            //ndat - CHK
		            break; 
		        }
		        case _LIN_GHIST:         //bufor historii zdarzen
		        {
                    if(!ReadBtyHNPZ())      //historia pusta?
                    {
                        ndat=6+2;                                            //liczba danych wysy?anych (+ CHK)
                        chkndat=SetNDatCHK(ndat);                           //oblicz CHK dla liczby danych
                        SetRSTBuf(0,_LCD_ADDR);				//0 - adres master
                        SetRSTBuf(1,chkndat);				//1 - liczba przesy?anych danych (ndat)
                        SetRSTBuf(2,_RD_LIN);				//2 - identyfikator paczki danych
                        SetRSTBuf(3,_LIN_GHIST);            //3 - identyfikator komendy LIN  
                        SetRSTBuf(4,0);                     //4 - REFL
                        SetRSTBuf(5,0);                     //5 - REFH           
                        SetRSTBuf(6,0);                     //6 - rozmiar bufora                        
                    }
                    else
                    {
                        ndat=6+ReadBtyHNPZ()+2;                                            //liczba danych wysy?anych (+ CHK)
                        chkndat=SetNDatCHK(ndat);                           //oblicz CHK dla liczby danych
                        SetRSTBuf(0,_LCD_ADDR);				//0 - adres master
                        SetRSTBuf(1,chkndat);				//1 - liczba przesy?anych danych (ndat)
                        SetRSTBuf(2,_RD_LIN);				//2 - identyfikator paczki danych
                        SetRSTBuf(3,_LIN_GHIST);            //3 - identyfikator komendy LIN
                        SetRSTBuf(4,ReadBtyHREF(0));        //4 - REFL
                        SetRSTBuf(5,ReadBtyHREF(1));        //5 - REFH
                        SetRSTBuf(6,ReadBtyHNPZ());         //6 - rozmiar bufora
                        for(i=0;i<ReadBtyHNPZ();i++)
                        {
                            SetRSTBuf(7+i,ReadBtyEEHIST(i)); // dane bufora
                        }
                    }
		            SetRSCHK(ndat);			//ndat - CHK z bajtow [0..ndat] i zapisuje na poz. ndat
		            //ndat - CHK
		            break; 
		        }       
                case _LIN_RGDZ:         ////odczytaj liczbe godzin pracy
		        {
                    ndat=7;                                            //liczba danych wysy�anych (+ CHK)
                    chkndat=SetNDatCHK(ndat);                           //oblicz CHK dla liczby danych
                    SetRSTBuf(0,_LCD_ADDR);				//0 - adres master
                    SetRSTBuf(1,chkndat);				//1 - liczba przesy�anych danych (ndat)
                    SetRSTBuf(2,_RD_LIN);				//2 - identyfikator paczki danych
                    SetRSTBuf(3,_LIN_RGDZ);             //3 - identyfikator komendy LIN  
                    SetRSTBuf(4,ReadEEPROM(_ADRRLO));   //4 - L liczba godzin pracy
                    SetRSTBuf(5,ReadEEPROM(_ADRRHI));   //5 - H liczba godzin pracy    
		            SetRSCHK(ndat);			//ndat - CHK z bajtow [0..ndat] i zapisuje na poz. ndat
		            //ndat - CHK
		            break; 
		        }                 
		        //---------------GRUPA NROZKAZOW TYPU MODYFIKACJA
		        case _LIN_WHFLG:     //wyzeruj licznik flagi serwisowej
		        {
		            ndat=6;
		            chkndat=SetNDatCHK(ndat);                           //oblicz CHK dla liczby danych
		            SetRSTBuf(0,_LCD_ADDR);				//0 - adres master
		            SetRSTBuf(1,chkndat);				//1 - liczba przesy?anych danych (ndat)
		            SetRSTBuf(2,_RD_LIN);				//2 - identyfikator paczki danych
		            SetRSTBuf(3,_LIN_WHFLG);            //3 - identyfikator komendy LIN
		            SetRSTBuf(4,ReadBtyHFLG());         //4 - aktualny wartosc licznika flagi serwisowej
		            SetRSCHK(ndat);                     //ndat - CHK z bajtow [0..ndat] i zapisuje na poz. ndat
		            //ndat - CHK
		            break;
		        }   
		        case _LIN_WGDZ:     //wyzeruj licznik godzin pracy
		        {
		            ndat=6;
		            chkndat=SetNDatCHK(ndat);                           //oblicz CHK dla liczby danych
		            SetRSTBuf(0,_LCD_ADDR);				//0 - adres master
		            SetRSTBuf(1,chkndat);				//1 - liczba przesy³anych danych (ndat)
		            SetRSTBuf(2,_RD_LIN);				//2 - identyfikator paczki danych
		            SetRSTBuf(3,_LIN_WGDZ);             //3 - identyfikator komendy LIN
                    if(GetRGDZ())
                    {
                        SetRSTBuf(4,1);                 //4 - licznik nie jest wyzerowany 
                    }
                    else
                    {
                        SetRSTBuf(4,0);                  //4 - licznik jest wyzerowany                        
                    }
		            SetRSCHK(ndat);                     //ndat - CHK z bajtow [0..ndat] i zapisuje na poz. ndat
		            //ndat - CHK
		            break;
		        } 				               
		        case _LIN_SADR:          //zapisz adres LIN
		        {
		            ndat=6;
		            chkndat=SetNDatCHK(ndat);                           //oblicz CHK dla liczby danych
		            SetRSTBuf(0,_LCD_ADDR);				//0 - adres master
		            SetRSTBuf(1,chkndat);				//1 - liczba przesy?anych danych (ndat)
		            SetRSTBuf(2,_RD_LIN);				//2 - identyfikator paczki danych
		            SetRSTBuf(3,_LIN_SADR);                             //3 - identyfikator komendy LIN
		            SetRSTBuf(4,RSDT1.outALIN);                         //4 - aktualny adres LIN odbiornika
		            SetRSCHK(ndat);                                     //ndat - CHK z bajtow [0..ndat] i zapisuje na poz. ndat
		            //ndat - CHK
		            break;
		        }
		    }
		    break;
		}
#endif        
	}
}
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//Timer na potrzeby transmisji (umiescic w przerwaniu od zegara TMR1)
inline void IncRSTimer(void)
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
/*---------------------------------------------------------------------------------*/
//-------------------------------------------------------------------------------
//Timer na potrzeby transmisji (umiescic w przerwaniu od zegara TMR1)
inline void IncRSTimer2(void)
{
	if(MGSD.timRS2<255) MGSD.timRS2++;
}
//-------------------------------------------------------------------------------
//Restart zegara 
void StartRSTim2(void)
{
	MGSD.timRS2=0;
}
//-------------------------------------------------------------------------------
//Odczyt stanu zegara 
unsigned char RSTim2(void)
{
	return MGSD.timRS2;
}
//-------------------------------------------------------------------------------
//Inicjalizacja danych zwiazanych z transmisj? UART
void InitReceptProc(void)
{
	MGSD._ReceptData=0;
	MGSD._ReceptOK=0;
	MGSD._ReceptACK=0;
	MGSD._TERR_TIM=0;
	MGSD._RERR_TIM=0;
	MGSD._RERR_REC=0;
	MGSD._CON=0;
	MGSD.timRS=0;
	MGSD.timRS2=0;
	MGSD.MTRS=0;
	MGSD.KTR=0;
    MGSD.nerr=0;
}
//-------------------------------------------------------------------------------
//Uruchomienie odbioru danych
void StartReceptData(void)
{
	MGSD._ReceptData=1;		//ustaw bit aktywacji proc. ReceptData
	MGSD.MTRS=0;
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//ODBIOR:
//0-adres,
//dane,
//chk,
//NADAWANIE
//0-adres master,
//1-potwierdzenie (0xff (pozytywne) lub 0x00 (negatywne koniec transmisji))
//2-adres slave
//dane
//chk
/*---------------------------------------------------------------------------------*/
//Realizacja transmisji danych do/z odbiornika
// tnDat - liczba bajtow wysylanych do odbiornika (razem z CHK jesli wystepuje),
// rnDat - liczba bajtow odpowiedzi odbiornika (razem z CHK jesli wystepuje).
/*---------------------------------------------------------------------------------*/
//Algorytm odbioru i nadawania z bajtem potwierdzenia

void ReceptData(const unsigned char tnDat)
{
	switch(MGSD.MTRS)
	{
		case 0:
        {
			if(!UART1_ReceiveStat_Ex()) return;							//odczekaj na fizyczne zakonczenie odbioru
			MGSD.MTRS=1;            
        }
		case 1:
        {
			MGSD._ReceptOK=1;							//znacznik potwierdzenia odebrania paczki danych/zakonczenie trans.
			UART1_SetContinuousReceive_Ex2(0);			//blokada odbioru na czas analizy danych
			if(VerifyRSCHK(UADt.RNData))				//poprawna paczka danych?
			{
				SaveRSToData();							//zapisz dane w pamieci
				MGSD._ReceptACK=1;
				MGSD._CON=1;							//ustaw znacznik polaczenia
				//if(xxy<99) xxy++;
                //else xxy=0;
			}	
			else 
			{
				MGSD._ReceptACK=0;
			}
			UART1_SetContinuousReceive_Ex2(1);
            StartRSTim();
			if(tnDat)
			{
				if(MGSD._ReceptACK)                     //odebrano zrozumiala paczke danych?
                {
                    SaveDataToRS(RSDT1.outID);          //odpowiedz
                }
				else
                {
                    SaveDataToRS(_RD_ERR);              //sygnalizacja bledu
                }
			}
            StartRSTim2();
			MGSD.MTRS=2;
        }
        case 2: 
        {
			if(tnDat)
			{
                if(RSTim()<1) return;           //odczekaj minimalny czas do wysylki odpowiedzi                
				StartUSARTTR();                 //wyslij pelna paczke danych
			}            
            StartRSTim();
            StartRSTim2();
			MGSD.MTRS=3;            
        }
		case 3: 
        {
			if(tnDat)
			{
				if(UART1_IsTXIE_Ex())									//trwa nadawanie?
				{
					if(((RSTim()>1)&&!UADt.TData)||(RSTim()>_TS_TOUT))					//uplynol maksymalny czas oczekiwania na zak. nadawania?	
					{
						MGSD.MTRS=4;
						return;
					}
					return;
				}
                if(!UART1_TransmitStat_Ex())  return;				//odczekaj na fizyczne zakonczenie transmisji
			}
            StartRSTim();
            StartRSTim2();
            MGSD._TERR_TIM=0;	
            MGSD._ReceptData=0;
			UADt.REnd=0;
            MGSD.nerr=0; 
			MGSD.MTRS=0;            
            return;
        }            
		case 4:     //przekroczony czas
        {
			MGSD._ReceptOK=0;
			MGSD._TERR_TIM=1;								//przekroczony limit czasu dla transmisji
			StopUSARTTR();
			MGSD._ReceptData=0;
			UADt.REnd=0;
            MGSD.MTRS=0;
			return;	
        }
	}
}
/*---------------------------------------------------------------------------------*/
//Odbior danych z modulu LCD (procedure umiescic w petli glownej programu)
void ReceptDataLCD(void)
{
	switch(MGSD.KTR)
	{
		case 0:
        {
			StartReceptData();					//wyzeruj procedure odbioru danych
			MGSD.KTR=1;
        }
		case 1:
        {
			if(UADt.REnd)
			{
				if(MGSD._ReceptData) 				//odbior w toku?
				{
					ReceptData(1);	//realizuj proces odbioru danych
					return;
				}
			}
			if(RSTim2()>200)
            {
                PMD4bits.UART1MD=1;
                MGSD.KTR=2;
                return;
            }
            M._LCD=(MGSD._CON)?1:0;
			MGSD.KTR=0;
            return;
        }
        case 2:
        {
			if(RSTim2()>220)
            {
                StartRSTim2();
                PMD4bits.UART1MD=0;
                EUSART1_Initialize();
                EUSART1_SetTxInterruptHandler(IntUSARTTR);
                EUSART1_SetRxInterruptHandler(IntUSARTRC);
                InitDtaUART1();                           
                MGSD.KTR=0; 
                //if(zzz++>99) zzz=0;
                if(MGSD.nerr++>4)
                {
                    MGSD.nerr=0;
                    MGSD._CON=0;
                }
                return;
            }
            M._LCD=(MGSD._CON)?1:0;
			return;            
        }
	}
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
#endif
