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

#if _TYP_PRJ==1
#advisory _TYP_PRJ==1 
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
volatile unsigned char xxy,zzz,vvv,mmm;

/*---------------------------------------------------------------------------------*/
//Rejestracja odebranych danych z modu³u LCD
void SaveRSToData(void)
{
	unsigned char nbty,bf;
    float fd;
    
	//0 - adres slave
	//1 - liczba przesylanych bajtow (ndat)
	//2 - identyfikator paczki danych
	RSDT1.outID=GetRSRBuf(2);
	switch(RSDT1.outID)
	{
		case _RD_STD:				//identyfikator paczki danych            
		{
            //(RAMKA DLA WERSJI OPROGRAMOWANIA<=8)
			RSDT1.RD_STD=0;
            nbty=GetNDat();

			if(nbty>=10) RSDT1.vSTD=1;	//wersja wyswitlacza >=7 (procedura odpowietrzenia)
			else RSDT1.vSTD=0;									//wersja wyswietlacza <7

			RSDT1.inSTAT0=GetRSRBuf(3);
			RSDT1.inSTAT1=GetRSRBuf(4);
			RSDT1.inPCO=GetRSRBuf(5);
			RSDT1.inPCW=GetRSRBuf(6);
			RSDT1.inECO=GetRSRBuf(7);
            RSDT1.inINSTAT=0;
            RSDT1.inLNSTAT=0;  
            RSDT1.inSTAT2=0;            
            RSDT1.inRDZ0=0;            
			RSDT1.RD_STD=1;
			break;
		}
		case _RD_STD2:				//identyfikator paczki danych            
		{
            //(RAMKA DLA WERSJI OPROGRAMOWANIA>=9)
			RSDT1.RD_STD=0;
            nbty=GetExNDat();

            switch(nbty)
            {
                case 15:
                    RSDT1.vSTD=2;
                    break;
                default:
                    if(nbty>=15) RSDT1.vSTD=2;
                    else RSDT1.vSTD=2;              //wersja ramki danych
                    break;
            }  
			RSDT1.inRDZ0=GetRSRBuf(4);   
			RSDT1.inSTAT0=GetRSRBuf(5);
			RSDT1.inSTAT1=GetRSRBuf(6);
			RSDT1.inPCO=GetRSRBuf(7);
			RSDT1.inPCW=GetRSRBuf(8);
			RSDT1.inECO=GetRSRBuf(9);
			RSDT1.inINSTAT=GetRSRBuf(10);  
            RSDT1.inLNSTAT=GetRSRBuf(11);
            RSDT1.inCONTSETP=GetRSRBuf(12); 
            RSDT1.inSTAT2=GetRSRBuf(13);
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
            //(RAMKA DLA WERSJI OPROGRAMOWANIA<=8)
			if(!RSDT1.RRD_KNF)		//aktywna blokada na czas interpretacji danych?
			{
				RSDT1.RD_KNF=0;

				//okreslenie wersji paczki danych (na potrzeby proc.konf.- kompatybilnosc wsteczna)
                nbty=GetNDat();

				switch(nbty)	//liczba przeslanych bajtow
				{
					case 23:
						RSDT1.vKNF=1;
						break;
					case 25:
						RSDT1.vKNF=2;
						break;
					case 26:
						RSDT1.vKNF=3;
						break;
					case 27:
						RSDT1.vKNF=4;
						break;
					case 30:
						RSDT1.vKNF=5;
						break;
					case 31:
						RSDT1.vKNF=6;
						break;                          
					default:
                        if(nbty>31) RSDT1.vKNF=6;
						else RSDT1.vKNF=2;
						break;												
				}
                RSDT1.inKSTAT0=GetRSRBuf(3);
				RSDT1.inKRK=GetRSRBuf(4);
				RSDT1.inPRM=GetRSRBuf(5);
				RSDT1.inDDTA=GetRSRBuf(6);
				RSDT1.inMSTR=GetRSRBuf(7);
				RSDT1.inMMAXU=GetRSRBuf(8);
				RSDT1.inMMAXG=GetRSRBuf(9);
				RSDT1.inOPDG=GetRSRBuf(10);
				RSDT1.inWANL=GetRSRBuf(11);
				RSDT1.inKODKT=GetRSRBuf(12);
				RSDT1.inWOBG=GetRSRBuf(13);
				RSDT1.inNOKR=GetRSRBuf(14);
				RSDT1.inTPCS=GetRSRBuf(15);
				RSDT1.inMMIN=GetRSRBuf(16);
				RSDT1.inMMAX=GetRSRBuf(17);
				RSDT1.inRDPM=GetRSRBuf(18);
				RSDT1.inDTPM=GetRSRBuf(19);
				RSDT1.inMDPM=GetRSRBuf(20);
				RSDT1.inTECO=GetRSRBuf(21);
				RSDT1.inTMMAXG2=GetRSRBuf(22);
				RSDT1.inMMAXG2=GetRSRBuf(23);
				RSDT1.inMGPM=GetRSRBuf(24); 
				RSDT1.inMGPM2=GetRSRBuf(25); 
				RSDT1.inHSCO=GetRSRBuf(26);
				RSDT1.inCONW=GetRSRBuf(27); 
				RSDT1.inPROP=GetRSRBuf(28);
				RSDT1.inRFAN=GetRSRBuf(29);               
				RSDT1.RD_KNF=1;
			}
			break;
		}
		case _RD_KNF2:				//identyfikator paczki danych
		{
            //(RAMKA DLA WERSJI OPROGRAMOWANIA>=9)
			if(!RSDT1.RRD_KNF)		//aktywna blokada na czas interpretacji danych?
			{
				RSDT1.RD_KNF=0;
                nbty=GetExNDat();

				switch(nbty)	//liczba przeslanych bajtow
				{
					case 40:
						RSDT1.vKNF=9;
						break;                    
					case 38:
						RSDT1.vKNF=8;
						break;                    
					case 37:
						RSDT1.vKNF=7;
						break;                        
					default:
                        if(nbty>40) RSDT1.vKNF=9;
						else RSDT1.vKNF=9;
						break;												
				}
                RSDT1.inKSTAT0=GetRSRBuf(4);
				RSDT1.inKRK=GetRSRBuf(5);
				RSDT1.inPRM=GetRSRBuf(6);
				RSDT1.inDDTA=GetRSRBuf(7);
				RSDT1.inMSTR=GetRSRBuf(8);
				RSDT1.inMMAXU=GetRSRBuf(9);
				RSDT1.inMMAXG=GetRSRBuf(10);
				RSDT1.inOPDG=GetRSRBuf(11);
				RSDT1.inWANL=GetRSRBuf(12);
				RSDT1.inKODKT=GetRSRBuf(13);
				RSDT1.inWOBG=GetRSRBuf(14);
				RSDT1.inNOKR=GetRSRBuf(15);
				RSDT1.inTPCS=GetRSRBuf(16);
				RSDT1.inMMIN=GetRSRBuf(17);
				RSDT1.inMMAX=GetRSRBuf(18);
				RSDT1.inRDPM=GetRSRBuf(19);
				RSDT1.inDTPM=GetRSRBuf(20);
				RSDT1.inMDPM=GetRSRBuf(21);
				RSDT1.inTECO=GetRSRBuf(22);
				RSDT1.inTMMAXG2=GetRSRBuf(23);
				RSDT1.inMMAXG2=GetRSRBuf(24);
				RSDT1.inMGPM=GetRSRBuf(25); 
				RSDT1.inMGPM2=GetRSRBuf(26); 
				RSDT1.inHSCO=GetRSRBuf(27);
				RSDT1.inCONW=GetRSRBuf(28); 
				RSDT1.inPROP=GetRSRBuf(29);
				RSDT1.inRFAN=GetRSRBuf(30);
				RSDT1.inTSTR=GetRSRBuf(31);       
				RSDT1.inTL3=GetRSRBuf(32);  
                
				RSDT1.inPGDR=GetRSRBuf(33);
				RSDT1.inPGDG=GetRSRBuf(34);
				RSDT1.inPGDO=GetRSRBuf(35);  
				RSDT1.inPGMX=GetRSRBuf(36);

				RSDT1.inCWMG=GetRSRBuf(37);
				RSDT1.inCWTM=GetRSRBuf(38);                
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
								case 9:     //9 aktywacja zegara antylegionella
								{
									if(RSDT1.inLDTA2!=ReadEEPROM(MinMax[RSDT1.inLDTA1].addr)) 
									{
										StartRTG(_RTANL);
										ClearEETA();	//zeruj pamiec timera zasobnika					 											
										if_WriteEEPROM(MinMax[RSDT1.inLDTA1].addr,RSDT1.inLDTA2);
									}
									break;
								}
								case 13:    //13 minimalny wydatek pompy
								{
									if(RSDT1.inLDTA2!=ReadEEPROM(MinMax[RSDT1.inLDTA1].addr)) 
									{
										bf=ReadEEPROM(MinMax[17].addr);
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
								case 17:    //17 maksymalny wydatek pompy
								{
									if(RSDT1.inLDTA2!=ReadEEPROM(MinMax[RSDT1.inLDTA1].addr)) 
									{
										bf=ReadEEPROM(MinMax[13].addr);
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
                                case 22:    //22 rodzaj wentylatora
                                {
									if(RSDT1.inLDTA2!=ReadEEPROM(MinMax[RSDT1.inLDTA1].addr))      
									{					 											
										if_WriteEEPROM(MinMax[RSDT1.inLDTA1].addr,RSDT1.inLDTA2);
                                        SetDefFan(RSDT1.inLDTA2);                   //ustaw domyslne parametry nowego wentylatora
									}                                            
                                    break;
                                }  
                                case 25:    //25 rodzaj regulatora pogodowego
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
		        case _LIN_RPP:     //pobierz wartosc parametru P
		        {
		            break;          //(wywolanie bez parametrow)
		        } 
		        case _LIN_RPI:     //pobierz wartosc parametru I
		        {
		            break;          //(wywolanie bez parametrow)
		        } 
		        case _LIN_RPD:     //pobierz wartosc parametru D
		        {
		            break;          //(wywolanie bez parametrow)
		        } 
		        case _LIN_RPV:     //pobierz wartosc parametru D
		        {
		            break;          //(wywolanie bez parametrow)
		        }      
		        case _LIN_RRCO:     //pobierz wartosc wspolczynnika korekty modulacji CO przy wyborze typu 2 wentylatora
		        {
		            break;          //(wywolanie bez parametrow)
		        }
		        case _LIN_RRCW:     //pobierz wartosc wspolczynnika korekty modulacji CW przy wyborze typu 2 wentylatora
		        {
		            break;          //(wywolanie bez parametrow)
		        }                
		        case _LIN_WPP:     //zapisz wartosc parametru P
		        {
                    RSDT1.inLDTA1=GetRSRBuf(4);                //(wywolanie z parametrem)
                    RSDT1.inLDTA2=GetRSRBuf(5);                //(wywolanie z parametrem)
                    fd=BtyF88ToFData(RSDT1.inLDTA1,RSDT1.inLDTA2);
                    RSDT1.outPKERR=0;
                    if((fd>=-127.0)&&(fd<=127.0)) 
                    {
                        if_WriteEEPROM(_ADRPLO,RSDT1.inLDTA1);
                        if_WriteEEPROM(_ADRPHI,RSDT1.inLDTA2);
                        DtKNF.P=(127.0+BtyF88ToFData(ReadEEPROM(_ADRPLO),ReadEEPROM(_ADRPHI)))/100000.0;
                    }
                    else
                    {
                        RSDT1.outPKERR=1;
                    }
		            break;          //(wywolanie bez parametrow)
		        } 
		        case _LIN_WPI:     //zapisz wartosc parametru I
		        {
                    RSDT1.inLDTA1=GetRSRBuf(4);                //(wywolanie z parametrem)
                    RSDT1.inLDTA2=GetRSRBuf(5);                //(wywolanie z parametrem)   
                    fd=BtyF88ToFData(RSDT1.inLDTA1,RSDT1.inLDTA2);
                    RSDT1.outPKERR=0;
                    if((fd>=-127.0)&&(fd<=127.0)) 
                    {
                        if_WriteEEPROM(_ADRILO,RSDT1.inLDTA1);
                        if_WriteEEPROM(_ADRIHI,RSDT1.inLDTA2);
                        DtKNF.I=(127.0+BtyF88ToFData(ReadEEPROM(_ADRILO),ReadEEPROM(_ADRIHI)))/100000.0;
                    } 
                    else
                    {
                        RSDT1.outPKERR=1;
                    }                    
		            break;          //(wywolanie bez parametrow)
		        } 
		        case _LIN_WPD:     //zapisz wartosc parametru D
		        {
                    RSDT1.inLDTA1=GetRSRBuf(4);                //(wywolanie z parametrem)
                    RSDT1.inLDTA2=GetRSRBuf(5);                //(wywolanie z parametrem)
                    fd=BtyF88ToFData(RSDT1.inLDTA1,RSDT1.inLDTA2);
                    RSDT1.outPKERR=0;                    
                    if((fd>=-127.0)&&(fd<=127.0)) 
                    {
                        if_WriteEEPROM(_ADRDLO,RSDT1.inLDTA1);
                        if_WriteEEPROM(_ADRDHI,RSDT1.inLDTA2);
                        DtKNF.D=(127.0+BtyF88ToFData(ReadEEPROM(_ADRDLO),ReadEEPROM(_ADRDHI)))/100000.0;
                    } 
                    else
                    {
                        RSDT1.outPKERR=1;
                    }                    
		            break;          //(wywolanie bez parametrow)
		        }   
		        case _LIN_WPV:     //zapisz dolna wartosc graniczna wentylatora (obowiazuje dla DtKNF.rfan==2)
		        {
                    RSDT1.outPKERR=0; 
                    RSDT1.inLDTA1=GetRSRBuf(4);                //(wywolanie z parametrem)
                    if_WriteEEPROM(_ADRVVLO,RSDT1.inLDTA1);
                    if(DtKNF.rfan==2) 
                    {
                        ModParam();									//oblicz kluczowe parametry modulatora  
                    }
		            break;          //(wywolanie bez parametrow)
		        }  
		        case _LIN_WRCO1:     //zapisz wspolczynnik korekty punktu rownowagi w fazie dochodzenia (FAZA1)
		        {
                    RSDT1.inLDTA1=GetRSRBuf(4);                //(wywolanie z parametrem)
                    RSDT1.outPKERR=0;                    
                    if((RSDT1.inLDTA1>=0)&&(RSDT1.inLDTA1<=100)) 
                    {
                        if_WriteEEPROM(_ADRRCO1,RSDT1.inLDTA1);
                        DtKNF.RCO1=(float)ReadEEPROM(_ADRRCO1)/10.0;
                    } 
                    else
                    {
                        RSDT1.outPKERR=1;
                    }                    
		            break;          //(wywolanie bez parametrow)
		        }    
		        case _LIN_WRCO2:     //zapisz wspolczynnik regulacji w punkcie rownowagi w fazie stabilizacji (FAZA2)
		        {
                    RSDT1.inLDTA1=GetRSRBuf(4);                //(wywolanie z parametrem)
                    RSDT1.outPKERR=0;                    
                    if((RSDT1.inLDTA1>=0)&&(RSDT1.inLDTA1<=100)) 
                    {
                        if_WriteEEPROM(_ADRRCO2,RSDT1.inLDTA1);
                        DtKNF.RCO2=(float)ReadEEPROM(_ADRRCO2)/10.0;
                    } 
                    else
                    {
                        RSDT1.outPKERR=1;
                    }                    
		            break;          //(wywolanie bez parametrow)
		        }    
		        case _LIN_WRCO3:     //zapisz wspolczynnik korekty punktu rownowagi w fazie stabilizacji (FAZA2)
		        {
                    RSDT1.inLDTA1=GetRSRBuf(4);                //(wywolanie z parametrem)
                    RSDT1.outPKERR=0;                    
                    if((RSDT1.inLDTA1>=0)&&(RSDT1.inLDTA1<=100)) 
                    {
                        if_WriteEEPROM(_ADRRCO3,RSDT1.inLDTA1);
                        DtKNF.RCO3=(float)ReadEEPROM(_ADRRCO3)/10.0;
                    } 
                    else
                    {
                        RSDT1.outPKERR=1;
                    }                    
		            break;          //(wywolanie bez parametrow)
		        }   
		        case _LIN_WRCO4:     //zapisz wspolczynnik reakcji superwizyjnej
		        {
                    RSDT1.inLDTA1=GetRSRBuf(4);                //(wywolanie z parametrem)
                    RSDT1.outPKERR=0;                    
                    if((RSDT1.inLDTA1>=0)&&(RSDT1.inLDTA1<=100)) 
                    {
                        if_WriteEEPROM(_ADRRCO4,RSDT1.inLDTA1);
                        DtKNF.RCO4=(float)ReadEEPROM(_ADRRCO4)/10.0;
                    } 
                    else
                    {
                        RSDT1.outPKERR=1;
                    }                    
		            break;          //(wywolanie bez parametrow)
		        }   
		        case _LIN_WRCW1:     //zapisz wspolczynnik korekty punktu rownowagi w fazie dochodzenia (FAZA1)
		        {
                    RSDT1.inLDTA1=GetRSRBuf(4);                //(wywolanie z parametrem)
                    RSDT1.outPKERR=0;                    
                    if((RSDT1.inLDTA1>=0)&&(RSDT1.inLDTA1<=100)) 
                    {
                        if_WriteEEPROM(_ADRRCW1,RSDT1.inLDTA1);
                        DtKNF.RCW1=(float)ReadEEPROM(_ADRRCW1)/10.0;
                    } 
                    else
                    {
                        RSDT1.outPKERR=1;
                    }                    
		            break;          //(wywolanie bez parametrow)
		        }    
		        case _LIN_WRCW2:     //zapisz wspolczynnik regulacji w punkcie rownowagi w fazie stabilizacji (FAZA2)
		        {
                    RSDT1.inLDTA1=GetRSRBuf(4);                //(wywolanie z parametrem)
                    RSDT1.outPKERR=0;                    
                    if((RSDT1.inLDTA1>=0)&&(RSDT1.inLDTA1<=100)) 
                    {
                        if_WriteEEPROM(_ADRRCW2,RSDT1.inLDTA1);
                        DtKNF.RCW2=(float)ReadEEPROM(_ADRRCW2)/10.0;
                    } 
                    else
                    {
                        RSDT1.outPKERR=1;
                    }                    
		            break;          //(wywolanie bez parametrow)
		        }    
		        case _LIN_WRCW3:     //zapisz wspolczynnik korekty punktu rownowagi w fazie stabilizacji (FAZA2)
		        {
                    RSDT1.inLDTA1=GetRSRBuf(4);                //(wywolanie z parametrem)
                    RSDT1.outPKERR=0;                    
                    if((RSDT1.inLDTA1>=0)&&(RSDT1.inLDTA1<=100)) 
                    {
                        if_WriteEEPROM(_ADRRCW3,RSDT1.inLDTA1);
                        DtKNF.RCW3=(float)ReadEEPROM(_ADRRCW3)/10.0;
                    } 
                    else
                    {
                        RSDT1.outPKERR=1;
                    }                    
		            break;          //(wywolanie bez parametrow)
		        }   
		        case _LIN_WRCW4:     //zapisz wspolczynnik reakcji superwizyjnej
		        {
                    RSDT1.inLDTA1=GetRSRBuf(4);                //(wywolanie z parametrem)
                    RSDT1.outPKERR=0;                    
                    if((RSDT1.inLDTA1>=0)&&(RSDT1.inLDTA1<=100)) 
                    {
                        if_WriteEEPROM(_ADRRCW4,RSDT1.inLDTA1);
                        DtKNF.RCW4=(float)ReadEEPROM(_ADRRCW4)/10.0;
                    } 
                    else
                    {
                        RSDT1.outPKERR=1;
                    }                    
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
	volatile unsigned char ndat,i,nr;		//liczba danych wysy³anych + CHK
	unsigned char chkndat;

	//const unsigned char ndat=19;		//liczba danych wysy³anych (+ CHK)
	//unsigned char chkndat;
	switch(tDTA)
	{
		case _RD_ERR:						//identyfikator paczki danych
		{
			ndat=4;								//liczba danych wysy³anych (+ CHK)
			chkndat=SetNDatCHK(ndat);			//oblicz CHK dla liczby danych
		
			SetRSTBuf(0,_LCD_ADDR);				//0 - adres master
			SetRSTBuf(1,chkndat);				//1 - liczba przesy³anych danych (ndat)
			SetRSTBuf(2,_RD_ERR);				//2 - identyfikator paczki danych
			//ndat - CHK
			break;
		}
		case _RD_STD:                           //identyfikator paczki danych           
		{
			ndat=31;                            //30liczba danych wysy³anych (+ CHK)
            chkndat=SetNDatCHK(ndat);			//oblicz CHK dla liczby danych
			SetRSTBuf(0,_LCD_ADDR);				//0 - adres master
			SetRSTBuf(1,chkndat);				//1 - liczba przesy³anych danych (ndat)
			SetRSTBuf(2,_RD_STD);				//3 - identyfikator paczki danych
			SetRSTBuf(3,RSDT1.outRDZ0);
			SetRSTBuf(4,RSDT1.outRDZ1);
			SetRSTBuf(5,RSDT1.outSTAT0);
			SetRSTBuf(6,RSDT1.outSTAT1);
			SetRSTBuf(7,RSDT1.outSTAT2);
			SetRSTBuf(8,RSDT1.outSTAT3);
			SetRSTBuf(9,RSDT1.outCO);			//filtrowana wartosc temp
			SetRSTBuf(10,RSDT1.outCW);			//filtrowana wartosc temp
			SetRSTBuf(11,RSDT1.outCS);
			SetRSTBuf(12,RSDT1.outIN);
			SetRSTBuf(13,RSDT1.outVNT);
			SetRSTBuf(14,RSDT1.outPCO);
			SetRSTBuf(15,RSDT1.outPCOmn);
			SetRSTBuf(16,RSDT1.outPCOmx);
			SetRSTBuf(17,RSDT1.outPCW);
			SetRSTBuf(18,RSDT1.outPCWmn);
			SetRSTBuf(19,RSDT1.outPCWmx);
			SetRSTBuf(20,RSDT1.outPPCO);
			SetRSTBuf(21,RSDT1.outPPCW);
			SetRSTBuf(22,RSDT1.outERR);
			SetRSTBuf(23,RSDT1.outAWR);
			SetRSTBuf(24,RSDT1.outECO);
			SetRSTBuf(25,RSDT1.outPP);
			SetRSTBuf(26,RSDT1.outPG);
			SetRSTBuf(27,RSDT1.outrCO);			//rzeczywista wartosc temp
			SetRSTBuf(28,RSDT1.outrCW);			//rzeczywista wartosc temp
			SetRSTBuf(29,RSDT1.outSTAT4);
			SetRSCHK(ndat);			//ndat - CHK z bajtow [0..ndat] i zapisuje na poz. ndat
			//ndat - CHK
			break;
		}
		case _RD_STD2:                          //identyfikator paczki danych            
		{
			ndat=37;                            //liczba danych wysy³anych (+ CHK)
            SetExNDatCHK(ndat); 
			SetRSTBuf(0,_LCD_ADDR);				//0 - adres master           
			//SetRSTBuf(1,chkndat);				//1 - liczba przesy³anych danych (ndat)
			SetRSTBuf(2,_RD_STD2);				//2 - identyfikator paczki danych
            //SetRSTBuf(3,chkndat);             //3 - liczba przesy³anych danych (ndat)
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
			SetRSTBuf(17,RSDT1.outVNT);
			SetRSTBuf(18,RSDT1.outPCO);
			SetRSTBuf(19,RSDT1.outPCOmn);
			SetRSTBuf(20,RSDT1.outPCOmx);
			SetRSTBuf(21,RSDT1.outPCW);
			SetRSTBuf(22,RSDT1.outPCWmn);
			SetRSTBuf(23,RSDT1.outPCWmx);
			SetRSTBuf(24,RSDT1.outPPCO);
			SetRSTBuf(25,RSDT1.outPPCW);
			SetRSTBuf(26,RSDT1.outERR);
			SetRSTBuf(27,RSDT1.outAWR);
			SetRSTBuf(28,RSDT1.outECO);
			SetRSTBuf(29,RSDT1.outPP);
			SetRSTBuf(30,RSDT1.outPG);
			SetRSTBuf(31,RSDT1.outrCO);			//rzeczywista wartosc temp
			SetRSTBuf(32,RSDT1.outrCW);			//rzeczywista wartosc temp
			SetRSTBuf(33,RSDT1.outZW);		
			SetRSTBuf(34,RSDT1.outSETPmin);		
			SetRSTBuf(35,RSDT1.outSETPmax);            
			SetRSCHK(ndat);			//ndat - CHK z bajtow [0..ndat] i zapisuje na poz. ndat
			//ndat - CHK
			break;
		}        
		case _RD_KNF:                                           //identyfikator paczki danych           
		{
			ndat=30;                                        //liczba danych wysy³anych (+ CHK)
            chkndat=SetNDatCHK(ndat);			//oblicz CHK dla liczby danych
			SetRSTBuf(0,_LCD_ADDR);				//0 - adres master
			SetRSTBuf(1,chkndat);				//1 - liczba przesy³anych danych (ndat)
			SetRSTBuf(2,_RD_KNF);				//2 - identyfikator paczki danych
			SetRSTBuf(3,RSDT1.outKSTAT0); //(5 najstarszych bitow to wartosc dodatkowa liczby danych)
			SetRSTBuf(4,RSDT1.outKRK);
			SetRSTBuf(5,RSDT1.outPRM);
			SetRSTBuf(6,RSDT1.outMSTR);
			SetRSTBuf(7,RSDT1.outMMAXU);
			SetRSTBuf(8,RSDT1.outMMAXG);
			SetRSTBuf(9,RSDT1.outOPDG);
			SetRSTBuf(10,RSDT1.outWANL);
			SetRSTBuf(11,RSDT1.outKODKT);
			SetRSTBuf(12,RSDT1.outWOBG);
			SetRSTBuf(13,RSDT1.outNOKR);
			SetRSTBuf(14,RSDT1.outTPCS);
			SetRSTBuf(15,RSDT1.outMMIN);
			SetRSTBuf(16,RSDT1.outMMAX);
			SetRSTBuf(17,RSDT1.outRDPM);
			SetRSTBuf(18,RSDT1.outDTPM);
			SetRSTBuf(19,RSDT1.outMDPM);
			SetRSTBuf(20,RSDT1.outTECO);
			SetRSTBuf(21,RSDT1.outTMMAXG2);
			SetRSTBuf(22,RSDT1.outMMAXG2);
			SetRSTBuf(23,RSDT1.outMGPM);
			SetRSTBuf(24,RSDT1.outMGPM2); 

			SetRSTBuf(25,RSDT1.outHSCO);
			SetRSTBuf(26,RSDT1.outCONW);
			SetRSTBuf(27,RSDT1.outPROP);
            SetRSTBuf(28,RSDT1.outRFAN);        
			SetRSCHK(ndat);			//ndat - CHK z bajtow [0..ndat] i zapisuje na poz. ndat
			//ndat - CHK
			break;
		}
		case _RD_KNF2:                                          //identyfikator paczki danych            
		{
			ndat=39;                                        //liczba danych wysy³anych (+ CHK)
            SetExNDatCHK(ndat);            
			SetRSTBuf(0,_LCD_ADDR);				//0 - adres master            
			//SetRSTBuf(1,chkndat);				//1 - liczba przesy³anych danych (ndat)
			SetRSTBuf(2,_RD_KNF2);				//2 - identyfikator paczki danych
            //SetRSTBuf(3,chkndat);             //3 - liczba przesy³anych danych (ndat)          
			SetRSTBuf(4,RSDT1.outKSTAT0);       
			SetRSTBuf(5,RSDT1.outKRK);
			SetRSTBuf(6,RSDT1.outPRM);
			SetRSTBuf(7,RSDT1.outMSTR);
			SetRSTBuf(8,RSDT1.outMMAXU);
			SetRSTBuf(9,RSDT1.outMMAXG);
			SetRSTBuf(10,RSDT1.outOPDG);
			SetRSTBuf(11,RSDT1.outWANL);
			SetRSTBuf(12,RSDT1.outKODKT);
			SetRSTBuf(13,RSDT1.outWOBG);
			SetRSTBuf(14,RSDT1.outNOKR);
			SetRSTBuf(15,RSDT1.outTPCS);
			SetRSTBuf(16,RSDT1.outMMIN);
			SetRSTBuf(17,RSDT1.outMMAX);
			SetRSTBuf(18,RSDT1.outRDPM);
			SetRSTBuf(19,RSDT1.outDTPM);
			SetRSTBuf(20,RSDT1.outMDPM);
			SetRSTBuf(21,RSDT1.outTECO);
			SetRSTBuf(22,RSDT1.outTMMAXG2);
			SetRSTBuf(23,RSDT1.outMMAXG2);
			SetRSTBuf(24,RSDT1.outMGPM);
			SetRSTBuf(25,RSDT1.outMGPM2); 

			SetRSTBuf(26,RSDT1.outHSCO);
			SetRSTBuf(27,RSDT1.outCONW);
			SetRSTBuf(28,RSDT1.outPROP);
            SetRSTBuf(29,RSDT1.outRFAN);
            SetRSTBuf(30,RSDT1.outTSTR); 
            SetRSTBuf(31,RSDT1.outTL3);  
            SetRSTBuf(32,RSDT1.outPGDR);
            SetRSTBuf(33,RSDT1.outPGDG); 
            SetRSTBuf(34,RSDT1.outPGDO); 
            SetRSTBuf(35,RSDT1.outPGMX);   
            SetRSTBuf(36,RSDT1.outCWMG);              
            SetRSTBuf(37,RSDT1.outCWTM);            
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
		            ndat=30;                                            //liczba danych wysy³anych (+ CHK)
		            chkndat=SetNDatCHK(ndat);                           //oblicz CHK dla liczby danych
		            SetRSTBuf(0,_LCD_ADDR);				//0 - adres master
		            SetRSTBuf(1,chkndat);				//1 - liczba przesy³anych danych (ndat)
		            SetRSTBuf(2,_RD_LIN);				//2 - identyfikator paczki danych
		            SetRSTBuf(3,_LIN_GKNF);             //3 - identyfikator komendy LIN
                    if(ndat>31)
                    {
                        chkndat=SetNDatCHK(ndat-31);			//oblicz CHK dla liczby danych    
                    }
                    else chkndat=0;                     
                    SetRSTBuf(4,chkndat);               //4 - liczba przesylanych danych	
		            SetRSTBuf(5,RSDT1.outKSTAT0);       
		            SetRSTBuf(6,RSDT1.outMSTR);
		            SetRSTBuf(7,RSDT1.outMMAXU);
		            SetRSTBuf(8,RSDT1.outMMAXG);
		            SetRSTBuf(9,RSDT1.outOPDG);
		            SetRSTBuf(10,RSDT1.outWANL);
		            SetRSTBuf(11,RSDT1.outKODKT);
		            SetRSTBuf(12,RSDT1.outWOBG);
		            SetRSTBuf(13,RSDT1.outNOKR);
		            SetRSTBuf(14,RSDT1.outTPCS);
		            SetRSTBuf(15,RSDT1.outMMIN);
		            SetRSTBuf(16,RSDT1.outMMAX);
		            SetRSTBuf(17,RSDT1.outRDPM);
		            SetRSTBuf(18,RSDT1.outDTPM);
		            SetRSTBuf(19,RSDT1.outMDPM);
		            SetRSTBuf(20,RSDT1.outTECO);
		            SetRSTBuf(21,RSDT1.outTMMAXG2);
		            SetRSTBuf(22,RSDT1.outMMAXG2);
		            SetRSTBuf(23,RSDT1.outMGPM);
		            SetRSTBuf(24,RSDT1.outMGPM2);

					SetRSTBuf(25,RSDT1.outHSCO);
					SetRSTBuf(26,RSDT1.outCONW);
					SetRSTBuf(27,RSDT1.outPROP);
                    SetRSTBuf(28,RSDT1.outRFAN);                    
		            SetRSCHK(ndat);			//ndat - CHK z bajtow [0..ndat] i zapisuje na poz. ndat
		            //ndat - CHK
		            break;
		        }*/
				case _LIN_RPKNF:          //odczytaj wartosc parametru konfiguracyjnego
				{
		            ndat=7;                             //liczba danych wysy³anych (+ CHK)
		            chkndat=SetNDatCHK(ndat);           //oblicz CHK dla liczby danych
		            SetRSTBuf(0,_LCD_ADDR);				//0 - adres master
		            SetRSTBuf(1,chkndat);				//1 - liczba przesy³anych danych (ndat)
		            SetRSTBuf(2,_RD_LIN);				//2 - identyfikator paczki danych
		            SetRSTBuf(3,_LIN_RPKNF);          	//3 - identyfikator komendy LIN
					SetRSTBuf(4,RSDT1.outRPKNF);		//4 - wartosc parametru konfiguracji
					SetRSTBuf(5,RSDT1.outPKERR);		//5 - status operacji
					SetRSCHK(ndat);						//ndat - CHK z bajtow [0..ndat] i zapisuje na poz. ndat
	            	//ndat - CHK	
					RSDT1.outPKERR=0;			
					break;
				}
				case _LIN_WPKNF:          //zapisz wartosc parametru konfiguracyjnego
				{
		            ndat=7;                             //liczba danych wysy³anych (+ CHK)
		            chkndat=SetNDatCHK(ndat);           //oblicz CHK dla liczby danych
		            SetRSTBuf(0,_LCD_ADDR);				//0 - adres master
		            SetRSTBuf(1,chkndat);				//1 - liczba przesy³anych danych (ndat)
		            SetRSTBuf(2,_RD_LIN);				//2 - identyfikator paczki danych
		            SetRSTBuf(3,_LIN_WPKNF);          	//3 - identyfikator komendy LIN
					SetRSTBuf(4,RSDT1.outWPKNF);		//4 - wartosc parametru konfiguracji (wartosc parametru po zapisie)
					SetRSTBuf(5,RSDT1.outPKERR);		//5 - status operacji
					SetRSCHK(ndat);						//ndat - CHK z bajtow [0..ndat] i zapisuje na poz. ndat
	            	//ndat - CHK	
					RSDT1.outPKERR=0;			
					break;
				}
                case _LIN_GSTAT:         //statystyki zdarzen awaryjnych
		        {
                    ndat=10;                                            //liczba danych wysy³anych (+ CHK)
                    chkndat=SetNDatCHK(ndat);                           //oblicz CHK dla liczby danych
                    SetRSTBuf(0,_LCD_ADDR);				//0 - adres master
                    SetRSTBuf(1,chkndat);				//1 - liczba przesy³anych danych (ndat)
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
                        ndat=7;                                            //liczba danych wysy³anych (+ CHK)
                        chkndat=SetNDatCHK(ndat);                           //oblicz CHK dla liczby danych
                        SetRSTBuf(0,_LCD_ADDR);				//0 - adres master
                        SetRSTBuf(1,chkndat);				//1 - liczba przesy³anych danych (ndat)
                        SetRSTBuf(2,_RD_LIN);				//2 - identyfikator paczki danych
                        SetRSTBuf(3,_LIN_GEEST);             //3 - identyfikator komendy LIN  
                        SetRSTBuf(4,0);
                        SetRSTBuf(5,0);  
                    }
                    else
                    {
                        ndat=28;                                            //liczba danych wysy³anych (+ CHK)
                        chkndat=SetNDatCHK(ndat);                           //oblicz CHK dla liczby danych
                        SetRSTBuf(0,_LCD_ADDR);				//0 - adres master
                        SetRSTBuf(1,chkndat);				//1 - liczba przesy³anych danych (ndat)
                        SetRSTBuf(2,_RD_LIN);				//2 - identyfikator paczki danych
                        SetRSTBuf(3,_LIN_GEEST);             //3 - identyfikator komendy LIN
                        for(i=0;i<23;i++)
                        {
                            SetRSTBuf(4+i,ReadBtySTAT(nr,i));
                        }
                        
                        /*SetRSTBuf(4,ReadBtySTAT(nr,0));
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
                        SetRSTBuf(26,ReadBtySTAT(nr,22));*/
                    }
		            SetRSCHK(ndat);			//ndat - CHK z bajtow [0..ndat] i zapisuje na poz. ndat
		            //ndat - CHK
		            break; 
		        }
		        case _LIN_GHIST:         //bufor historii zdarzen
		        {
                    if(!ReadBtyHNPZ())      //historia pusta?
                    {
                        ndat=6+2;                                            //liczba danych wysy³anych (+ CHK)
                        chkndat=SetNDatCHK(ndat);                           //oblicz CHK dla liczby danych
                        SetRSTBuf(0,_LCD_ADDR);				//0 - adres master
                        SetRSTBuf(1,chkndat);				//1 - liczba przesy³anych danych (ndat)
                        SetRSTBuf(2,_RD_LIN);				//2 - identyfikator paczki danych
                        SetRSTBuf(3,_LIN_GHIST);            //3 - identyfikator komendy LIN  
                        SetRSTBuf(4,0);                     //4 - REFL
                        SetRSTBuf(5,0);                     //5 - REFH           
                        SetRSTBuf(6,0);                     //6 - rozmiar bufora                        
                    }
                    else
                    {
                        ndat=6+ReadBtyHNPZ()+2;                                            //liczba danych wysy³anych (+ CHK)
                        chkndat=SetNDatCHK(ndat);                           //oblicz CHK dla liczby danych
                        SetRSTBuf(0,_LCD_ADDR);				//0 - adres master
                        SetRSTBuf(1,chkndat);				//1 - liczba przesy³anych danych (ndat)
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
                case _LIN_RPP:         //pobierz wartosc parametru P
		        {
                    ndat=7;                                            //liczba danych wysy³anych (+ CHK)
                    chkndat=SetNDatCHK(ndat);                           //oblicz CHK dla liczby danych
                    SetRSTBuf(0,_LCD_ADDR);				//0 - adres master
                    SetRSTBuf(1,chkndat);				//1 - liczba przesy³anych danych (ndat)
                    SetRSTBuf(2,_RD_LIN);				//2 - identyfikator paczki danych
                    SetRSTBuf(3,_LIN_RPP);              //3 - identyfikator komendy LIN  
                    SetRSTBuf(4,ReadEEPROM(_ADRPLO));   //4 - L wartosc parametru P F88
                    SetRSTBuf(5,ReadEEPROM(_ADRPHI));   //5 - H wartosc parametru P F88                   
		            SetRSCHK(ndat);			//ndat - CHK z bajtow [0..ndat] i zapisuje na poz. ndat
		            //ndat - CHK
		            break; 
		        }   
                case _LIN_RPI:         //pobierz wartosc parametru I
		        {
                    ndat=7;                                            //liczba danych wysy³anych (+ CHK)
                    chkndat=SetNDatCHK(ndat);                           //oblicz CHK dla liczby danych
                    SetRSTBuf(0,_LCD_ADDR);				//0 - adres master
                    SetRSTBuf(1,chkndat);				//1 - liczba przesy³anych danych (ndat)
                    SetRSTBuf(2,_RD_LIN);				//2 - identyfikator paczki danych
                    SetRSTBuf(3,_LIN_RPI);              //3 - identyfikator komendy LIN  
                    SetRSTBuf(4,ReadEEPROM(_ADRILO));   //4 - L wartosc parametru I F88
                    SetRSTBuf(5,ReadEEPROM(_ADRIHI));   //5 - H wartosc parametru I F88                     
		            SetRSCHK(ndat);			//ndat - CHK z bajtow [0..ndat] i zapisuje na poz. ndat
		            //ndat - CHK
		            break; 
		        }  
                case _LIN_RPD:         //pobierz wartosc parametru D
		        {
                    ndat=7;                                            //liczba danych wysy³anych (+ CHK)
                    chkndat=SetNDatCHK(ndat);                           //oblicz CHK dla liczby danych
                    SetRSTBuf(0,_LCD_ADDR);				//0 - adres master
                    SetRSTBuf(1,chkndat);				//1 - liczba przesy³anych danych (ndat)
                    SetRSTBuf(2,_RD_LIN);				//2 - identyfikator paczki danych
                    SetRSTBuf(3,_LIN_RPD);               //3 - identyfikator komendy LIN  
                    SetRSTBuf(4,ReadEEPROM(_ADRDLO));   //4 - L wartosc parametru D F88
                    SetRSTBuf(5,ReadEEPROM(_ADRDHI));   //5 - H wartosc parametru D F88    
		            SetRSCHK(ndat);			//ndat - CHK z bajtow [0..ndat] i zapisuje na poz. ndat
		            //ndat - CHK
		            break; 
		        } 
                case _LIN_RPV:         //pobierz dolna wartosc graniczna wentylatora (obowiazuje dla DtKNF.rfan==2)
		        {
                    ndat=6;                                            //liczba danych wysy³anych (+ CHK)
                    chkndat=SetNDatCHK(ndat);                           //oblicz CHK dla liczby danych
                    SetRSTBuf(0,_LCD_ADDR);				//0 - adres master
                    SetRSTBuf(1,chkndat);				//1 - liczba przesy³anych danych (ndat)
                    SetRSTBuf(2,_RD_LIN);				//2 - identyfikator paczki danych
                    SetRSTBuf(3,_LIN_RPV);              //3 - identyfikator komendy LIN  
                    SetRSTBuf(4,ReadEEPROM(_ADRVVLO));  //4 - dolna wartosc wartosc graniczna wentylatora   
		            SetRSCHK(ndat);			//ndat - CHK z bajtow [0..ndat] i zapisuje na poz. ndat
		            //ndat - CHK
		            break; 
		        } 
                case _LIN_RRCO:         //pobierz wspolczynnik korekty modulacji CO przy wyborze typu 2 wentylatora
		        {
                    ndat=9;                                            //liczba danych wysy³anych (+ CHK)
                    chkndat=SetNDatCHK(ndat);                           //oblicz CHK dla liczby danych
                    SetRSTBuf(0,_LCD_ADDR);				//0 - adres master
                    SetRSTBuf(1,chkndat);				//1 - liczba przesy³anych danych (ndat)
                    SetRSTBuf(2,_RD_LIN);				//2 - identyfikator paczki danych
                    SetRSTBuf(3,_LIN_RRCO);             //3 - identyfikator komendy LIN  
                    SetRSTBuf(4,ReadEEPROM(_ADRRCO1));  //4 - [/10]regulacja w punkcie rownowagi w fazie stabilizacji (FAZA2)
                    SetRSTBuf(5,ReadEEPROM(_ADRRCO2));  //5 - [/10]korekta punktu rownowagi w fazie dochodzenia (FAZA1) 
                    SetRSTBuf(6,ReadEEPROM(_ADRRCO3));  //4 - [/10]korekta punktu rownowagi w fazie stabilizacji (FAZA2)
                    SetRSTBuf(7,ReadEEPROM(_ADRRCO4));  //5 - [/10]reakcja superwizyjna                    
		            SetRSCHK(ndat);			//ndat - CHK z bajtow [0..ndat] i zapisuje na poz. ndat
		            //ndat - CHK
		            break; 
		        }           
                case _LIN_RRCW:         //pobierz wspolczynnik korekty modulacji CW przy wyborze typu 2 wentylatora
		        {
                    ndat=9;                                            //liczba danych wysy³anych (+ CHK)
                    chkndat=SetNDatCHK(ndat);                           //oblicz CHK dla liczby danych
                    SetRSTBuf(0,_LCD_ADDR);				//0 - adres master
                    SetRSTBuf(1,chkndat);				//1 - liczba przesy³anych danych (ndat)
                    SetRSTBuf(2,_RD_LIN);				//2 - identyfikator paczki danych
                    SetRSTBuf(3,_LIN_RRCW);             //3 - identyfikator komendy LIN  
                    SetRSTBuf(4,ReadEEPROM(_ADRRCW1));  //4 - [/10]regulacja w punkcie rownowagi w fazie stabilizacji (FAZA2)
                    SetRSTBuf(5,ReadEEPROM(_ADRRCW2));  //5 - [/10]korekta punktu rownowagi w fazie dochodzenia (FAZA1) 
                    SetRSTBuf(6,ReadEEPROM(_ADRRCW3));  //4 - [/10]korekta punktu rownowagi w fazie stabilizacji (FAZA2)
                    SetRSTBuf(7,ReadEEPROM(_ADRRCW4));  //5 - [/10]reakcja superwizyjna                    
		            SetRSCHK(ndat);			//ndat - CHK z bajtow [0..ndat] i zapisuje na poz. ndat    
		            //ndat - CHK
		            break; 
		        }                  
		        //---------------GRUPA NROZKAZOW TYPU MODYFIKACJA
                case _LIN_WPP:         //zapisz wartosc parametru P
		        {
                    ndat=8;                                            //liczba danych wysy³anych (+ CHK)
                    chkndat=SetNDatCHK(ndat);                           //oblicz CHK dla liczby danych
                    SetRSTBuf(0,_LCD_ADDR);				//0 - adres master
                    SetRSTBuf(1,chkndat);				//1 - liczba przesy³anych danych (ndat)
                    SetRSTBuf(2,_RD_LIN);				//2 - identyfikator paczki danych
                    SetRSTBuf(3,_LIN_WPP);              //3 - identyfikator komendy LIN  
                    SetRSTBuf(4,ReadEEPROM(_ADRPLO));   //4 - L wartosc parametru P F88
                    SetRSTBuf(5,ReadEEPROM(_ADRPHI));   //5 - H wartosc parametru P F88 
					SetRSTBuf(6,RSDT1.outPKERR);		//6 - status operacji                    
		            SetRSCHK(ndat);			//ndat - CHK z bajtow [0..ndat] i zapisuje na poz. ndat
		            //ndat - CHK
		            break; 
		        }   
                case _LIN_WPI:         //zapisz wartosc parametru I
		        {
                    ndat=8;                                            //liczba danych wysy³anych (+ CHK)
                    chkndat=SetNDatCHK(ndat);                           //oblicz CHK dla liczby danych
                    SetRSTBuf(0,_LCD_ADDR);				//0 - adres master
                    SetRSTBuf(1,chkndat);				//1 - liczba przesy³anych danych (ndat)
                    SetRSTBuf(2,_RD_LIN);				//2 - identyfikator paczki danych
                    SetRSTBuf(3,_LIN_WPI);              //3 - identyfikator komendy LIN  
                    SetRSTBuf(4,ReadEEPROM(_ADRILO));   //4 - L wartosc parametru I F88
                    SetRSTBuf(5,ReadEEPROM(_ADRIHI));   //5 - H wartosc parametru I F88   
					SetRSTBuf(6,RSDT1.outPKERR);		//6 - status operacji                    
		            SetRSCHK(ndat);			//ndat - CHK z bajtow [0..ndat] i zapisuje na poz. ndat
		            //ndat - CHK
		            break; 
		        }  
                case _LIN_WPD:         //zapisz wartosc parametru D
		        {
                    ndat=8;                                            //liczba danych wysy³anych (+ CHK)
                    chkndat=SetNDatCHK(ndat);                           //oblicz CHK dla liczby danych
                    SetRSTBuf(0,_LCD_ADDR);				//0 - adres master
                    SetRSTBuf(1,chkndat);				//1 - liczba przesy³anych danych (ndat)
                    SetRSTBuf(2,_RD_LIN);				//2 - identyfikator paczki danych
                    SetRSTBuf(3,_LIN_WPD);              //3 - identyfikator komendy LIN  
                    SetRSTBuf(4,ReadEEPROM(_ADRDLO));   //4 - L wartosc parametru D F88
                    SetRSTBuf(5,ReadEEPROM(_ADRDHI));   //5 - H wartosc parametru D F88   
					SetRSTBuf(6,RSDT1.outPKERR);		//6 - status operacji                    
		            SetRSCHK(ndat);			//ndat - CHK z bajtow [0..ndat] i zapisuje na poz. ndat
		            //ndat - CHK
		            break; 
		        }          
                case _LIN_WPV:         //zapisz dolna wartosc graniczna wentylatora (obowiazuje dla DtKNF.rfan==2)
		        {
                    ndat=7;                                            //liczba danych wysy³anych (+ CHK)
                    chkndat=SetNDatCHK(ndat);                           //oblicz CHK dla liczby danych
                    SetRSTBuf(0,_LCD_ADDR);				//0 - adres master
                    SetRSTBuf(1,chkndat);				//1 - liczba przesy³anych danych (ndat)
                    SetRSTBuf(2,_RD_LIN);				//2 - identyfikator paczki danych
                    SetRSTBuf(3,_LIN_WPV);              //3 - identyfikator komendy LIN  
                    SetRSTBuf(4,ReadEEPROM(_ADRVVLO));  //4 - dolna wartosc wartosc graniczna wentylatora   
					SetRSTBuf(5,RSDT1.outPKERR);		//5 - status operacji                    
		            SetRSCHK(ndat);			//ndat - CHK z bajtow [0..ndat] i zapisuje na poz. ndat
		            //ndat - CHK
		            break; 
		        } 
                case _LIN_WRCO1:         //zapisz wspolczynnik korekty punktu rownowagi w fazie dochodzenia (FAZA1)
		        {
                    ndat=7;                                            //liczba danych wysy³anych (+ CHK)
                    chkndat=SetNDatCHK(ndat);                           //oblicz CHK dla liczby danych
                    SetRSTBuf(0,_LCD_ADDR);				//0 - adres master
                    SetRSTBuf(1,chkndat);				//1 - liczba przesy³anych danych (ndat)
                    SetRSTBuf(2,_RD_LIN);				//2 - identyfikator paczki danych
                    SetRSTBuf(3,_LIN_WRCO1);            //3 - identyfikator komendy LIN  
                    SetRSTBuf(4,ReadEEPROM(_ADRRCO1));  //4 - wspolczynnik korekty punktu rownowagi w fazie dochodzenia (FAZA1)  
					SetRSTBuf(5,RSDT1.outPKERR);		//5 - status operacji                    
		            SetRSCHK(ndat);			//ndat - CHK z bajtow [0..ndat] i zapisuje na poz. ndat
		            //ndat - CHK
		            break; 
		        } 
                case _LIN_WRCO2:         //zapisz wspolczynnik regulacji w punkcie rownowagi w fazie stabilizacji (FAZA2)
		        {
                    ndat=7;                                            //liczba danych wysy³anych (+ CHK)
                    chkndat=SetNDatCHK(ndat);                           //oblicz CHK dla liczby danych
                    SetRSTBuf(0,_LCD_ADDR);				//0 - adres master
                    SetRSTBuf(1,chkndat);				//1 - liczba przesy³anych danych (ndat)
                    SetRSTBuf(2,_RD_LIN);				//2 - identyfikator paczki danych
                    SetRSTBuf(3,_LIN_WRCO2);            //3 - identyfikator komendy LIN  
                    SetRSTBuf(4,ReadEEPROM(_ADRRCO2));  //4 - wspolczynnik regulacja w punkcie rownowagi w fazie stabilizacji (FAZA2)  
					SetRSTBuf(5,RSDT1.outPKERR);		//5 - status operacji                    
		            SetRSCHK(ndat);			//ndat - CHK z bajtow [0..ndat] i zapisuje na poz. ndat
		            //ndat - CHK
		            break; 
		        }     
                case _LIN_WRCO3:         //zapisz wspolczynnik korekty punktu rownowagi w fazie stabilizacji (FAZA2)
		        {
                    ndat=7;                                            //liczba danych wysy³anych (+ CHK)
                    chkndat=SetNDatCHK(ndat);                           //oblicz CHK dla liczby danych
                    SetRSTBuf(0,_LCD_ADDR);				//0 - adres master
                    SetRSTBuf(1,chkndat);				//1 - liczba przesy³anych danych (ndat)
                    SetRSTBuf(2,_RD_LIN);				//2 - identyfikator paczki danych
                    SetRSTBuf(3,_LIN_WRCO3);            //3 - identyfikator komendy LIN  
                    SetRSTBuf(4,ReadEEPROM(_ADRRCO3));  //4 - wspolczynnik korekty punktu rownowagi w fazie stabilizacji (FAZA2) 
					SetRSTBuf(5,RSDT1.outPKERR);		//5 - status operacji                    
		            SetRSCHK(ndat);			//ndat - CHK z bajtow [0..ndat] i zapisuje na poz. ndat
		            //ndat - CHK
		            break; 
		        }     
                case _LIN_WRCO4:         //zapisz wspolczynnik reakcji superwizyjnej
		        {
                    ndat=7;                                            //liczba danych wysy³anych (+ CHK)
                    chkndat=SetNDatCHK(ndat);                           //oblicz CHK dla liczby danych
                    SetRSTBuf(0,_LCD_ADDR);				//0 - adres master
                    SetRSTBuf(1,chkndat);				//1 - liczba przesy³anych danych (ndat)
                    SetRSTBuf(2,_RD_LIN);				//2 - identyfikator paczki danych
                    SetRSTBuf(3,_LIN_WRCO4);            //3 - identyfikator komendy LIN  
                    SetRSTBuf(4,ReadEEPROM(_ADRRCO4));  //4 - wspolczynnik reakcji superwizyjnej 
					SetRSTBuf(5,RSDT1.outPKERR);		//5 - status operacji                    
		            SetRSCHK(ndat);			//ndat - CHK z bajtow [0..ndat] i zapisuje na poz. ndat
		            //ndat - CHK
		            break; 
		        }                
                case _LIN_WRCW1:         //zapisz wspolczynnik korekty punktu rownowagi w fazie dochodzenia (FAZA1)
		        {
                    ndat=7;                                            //liczba danych wysy³anych (+ CHK)
                    chkndat=SetNDatCHK(ndat);                           //oblicz CHK dla liczby danych
                    SetRSTBuf(0,_LCD_ADDR);				//0 - adres master
                    SetRSTBuf(1,chkndat);				//1 - liczba przesy³anych danych (ndat)
                    SetRSTBuf(2,_RD_LIN);				//2 - identyfikator paczki danych
                    SetRSTBuf(3,_LIN_WRCW1);            //3 - identyfikator komendy LIN  
                    SetRSTBuf(4,ReadEEPROM(_ADRRCW1));  //4 - wspolczynnik korekty punktu rownowagi w fazie dochodzenia (FAZA1)  
					SetRSTBuf(5,RSDT1.outPKERR);		//5 - status operacji                    
		            SetRSCHK(ndat);			//ndat - CHK z bajtow [0..ndat] i zapisuje na poz. ndat
		            //ndat - CHK
		            break; 
		        } 
                case _LIN_WRCW2:         //zapisz wspolczynnik regulacji w punkcie rownowagi w fazie stabilizacji (FAZA2)
		        {
                    ndat=7;                                            //liczba danych wysy³anych (+ CHK)
                    chkndat=SetNDatCHK(ndat);                           //oblicz CHK dla liczby danych
                    SetRSTBuf(0,_LCD_ADDR);				//0 - adres master
                    SetRSTBuf(1,chkndat);				//1 - liczba przesy³anych danych (ndat)
                    SetRSTBuf(2,_RD_LIN);				//2 - identyfikator paczki danych
                    SetRSTBuf(3,_LIN_WRCW2);            //3 - identyfikator komendy LIN  
                    SetRSTBuf(4,ReadEEPROM(_ADRRCW2));  //4 - wspolczynnik regulacja w punkcie rownowagi w fazie stabilizacji (FAZA2)  
					SetRSTBuf(5,RSDT1.outPKERR);		//5 - status operacji                    
		            SetRSCHK(ndat);			//ndat - CHK z bajtow [0..ndat] i zapisuje na poz. ndat
		            //ndat - CHK
		            break; 
		        }     
                case _LIN_WRCW3:         //zapisz wspolczynnik korekty punktu rownowagi w fazie stabilizacji (FAZA2)
		        {
                    ndat=7;                                            //liczba danych wysy³anych (+ CHK)
                    chkndat=SetNDatCHK(ndat);                           //oblicz CHK dla liczby danych
                    SetRSTBuf(0,_LCD_ADDR);				//0 - adres master
                    SetRSTBuf(1,chkndat);				//1 - liczba przesy³anych danych (ndat)
                    SetRSTBuf(2,_RD_LIN);				//2 - identyfikator paczki danych
                    SetRSTBuf(3,_LIN_WRCW3);            //3 - identyfikator komendy LIN  
                    SetRSTBuf(4,ReadEEPROM(_ADRRCW3));  //4 - wspolczynnik korekty punktu rownowagi w fazie stabilizacji (FAZA2) 
					SetRSTBuf(5,RSDT1.outPKERR);		//5 - status operacji                    
		            SetRSCHK(ndat);			//ndat - CHK z bajtow [0..ndat] i zapisuje na poz. ndat
		            //ndat - CHK
		            break; 
		        }     
                case _LIN_WRCW4:         //zapisz wspolczynnik reakcji superwizyjnej
		        {
                    ndat=7;                                            //liczba danych wysy³anych (+ CHK)
                    chkndat=SetNDatCHK(ndat);                           //oblicz CHK dla liczby danych
                    SetRSTBuf(0,_LCD_ADDR);				//0 - adres master
                    SetRSTBuf(1,chkndat);				//1 - liczba przesy³anych danych (ndat)
                    SetRSTBuf(2,_RD_LIN);				//2 - identyfikator paczki danych
                    SetRSTBuf(3,_LIN_WRCW4);            //3 - identyfikator komendy LIN  
                    SetRSTBuf(4,ReadEEPROM(_ADRRCW4));  //4 - wspolczynnik reakcji superwizyjnej 
					SetRSTBuf(5,RSDT1.outPKERR);		//5 - status operacji                    
		            SetRSCHK(ndat);			//ndat - CHK z bajtow [0..ndat] i zapisuje na poz. ndat
		            //ndat - CHK
		            break; 
		        }                
		        case _LIN_WHFLG:     //wyzeruj licznik flagi serwisowej
		        {
		            ndat=6;
		            chkndat=SetNDatCHK(ndat);                           //oblicz CHK dla liczby danych
		            SetRSTBuf(0,_LCD_ADDR);				//0 - adres master
		            SetRSTBuf(1,chkndat);				//1 - liczba przesy³anych danych (ndat)
		            SetRSTBuf(2,_RD_LIN);				//2 - identyfikator paczki danych
		            SetRSTBuf(3,_LIN_WHFLG);            //3 - identyfikator komendy LIN
		            SetRSTBuf(4,ReadBtyHFLG());         //4 - aktualny wartosc licznika flagi serwisowej
		            SetRSCHK(ndat);                     //ndat - CHK z bajtow [0..ndat] i zapisuje na poz. ndat
		            //ndat - CHK
		            break;
		        }                                
		        case _LIN_SADR:          //zapisz adres LIN
		        {
		            ndat=6;
		            chkndat=SetNDatCHK(ndat);                           //oblicz CHK dla liczby danych
		            SetRSTBuf(0,_LCD_ADDR);				//0 - adres master
		            SetRSTBuf(1,chkndat);				//1 - liczba przesy³anych danych (ndat)
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
//Inicjalizacja danych zwiazanych z transmisj¹ UART
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
            if(GetRSRBuf(2)==_RD_STD2)
            {
                Nop();
                Nop();
            }
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