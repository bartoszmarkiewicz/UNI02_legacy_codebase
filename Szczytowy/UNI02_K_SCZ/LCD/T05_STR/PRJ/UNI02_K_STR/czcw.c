/*czcw.c*/
//Funkcje detekcji przeplywu z turbinki
/*---------------------------------------------------------------------------------*/
#include <xc.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

#include "../../proj.tp.h"

#ifndef _TYP_PRJ
#error NIEOKRESLONY IDENTYFIKATOR PROJEKTU
#endif

#if _TYP_PRJ==5
#advisory _TYP_PRJ==5 
#include "../../mcc_generated_files\mcc.h"

#include "global.h"
#include "global.ex.h"

#include "czcw.h"				//parametry lokalne

//extern DtKONF DtKNF;
DataPrCO PrCO;
	
/*---------------------------------------------------------------------------------*/ 
void InitCzCo(void)
{
//	TMR0IE=1;		//enable interrupt timer0
//	T08BIT=1;		//tryb 8 bitowy
//	T0CS=1;			//taktowany z RA4
//	T0SE=1;			//zbocze HI\LO
//	PSA=1;			//preskaler zostaje pominiety
//	TMR0=0;
    
	PrCO.CzCotim=0;
	PrCO._ltrDW=0;
	PrCO._ltrUP=0;
	PrCO.S_CO=0;
	PrCO.impCO=0;
}

void SetParPrzCo(void)
{
    //ltrONE - liczba impulsow przypadajaca na przeplyw o natezeniu 1 litra
    //ltrON - liczba impulsow przypadajaca na przeplyw o natezeniu 3 litrow
    //ltrOFF - liczba impulsow przypadajaca na przeplyw o natezeniu 2.5 litra  
    //switch(DtKNF.tpcw)      //rodzaj czujnika przeplywu
    switch(1)      //rodzaj czujnika przeplywu            
    {
        case 1:         //przetwornik przeplywu wody uzytkowej w hydrobloku Pakkens
        {
            PrCO.impONE=_ONE1;
            PrCO.impON=_ON_CO1;
            PrCO.impOFF=_OFF_CO1;
            break;
        }
        default:
        {
            PrCO.impONE=_ONE1;
            PrCO.impON=_ON_CO1;
            PrCO.impOFF=_OFF_CO1;           
        }
    }    
}

//-------------------------------------------------------------------------------
float ImpToPrz(const unsigned int imp)
{
    volatile float prz=0;
    //switch(DtKNF.tpcw)      //rodzaj czujnika przeplywu
    switch(1)      //rodzaj czujnika przeplywu
    {
        case 1:             //przetwornik przeplywu wody uzytkowej w hydrobloku Pakkens
        {
            if(imp)
            {
                //prz=(imp+1.4553)/9.3808;
                //prz=imp/4;
                prz=0.346*imp+0.161;
            }
            else
            {
                prz=0;
            }
            break;
        }
        default:
            prz=0;
    }
    return prz;
}
//-------------------------------------------------------------------------------
//Detekcja przeplywu w obiegu CO
void PomiarPrzCO(void)
{
    switch(1)      //rodzaj czujnika przeplywu
    {
        case 0:             //czujnik stykowy        
        {
            PrCO.przCO=0;
            PrCO.przCCO=0;
            PrCO.CzCotim=0;
            PrCO._ltrDW=0;
            PrCO._ltrUP=0;
            PrCO.impCO=0;
            if(_PRZ_CW_GetValue())           //sygnal pobudzenia na wejsciu mikrokontrolera?
            {
                PrCO.S_CO=0;                 //ustaw flage pobudzenia od CW
            }
            else
            {
                PrCO.S_CO=1;                 //zeruj flage pobudzenia od CW
            }
            break;
        }
        default:            //przetwornik impulsowy
        {
            //PrCO.S_CO = flaga pobudzenia ustawiana w przerwaniu obslugi czujnika impulsowego
            PrCO.przCO=ImpToPrz(PrCO.impCO);
            if(PrCO.przCO>99.99)
            {
                PrCO.przCO=99.99;
            }
            PrCO.przCCO = (unsigned int) (PrCO.przCO*100);
        }
    }
}
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-----------------------WYKONYWANE W PRZERWANIU-------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//Ustawia podstawowe flagi i zapamietuje aktualny przeplyw
//proc. wykonywana co 1 sek.
void ProcCO1(void)
{
	static unsigned int tprz_co;
	unsigned int btprz;

	btprz=tprz_co;
	tprz_co=TMR0_ReadTimer();
    TMR0_WriteTimer(0);

	if((btprz-tprz_co)>0)							//spadek przeplywu?
	{
		if((btprz-tprz_co)>PrCO.impONE/2) PrCO._ltrDW=1;	//zasygnalizuj spadek przeplywu o 0.5 litra
		else PrCO._ltrDW=0;
	}
	else
	{
		if((tprz_co-btprz)>PrCO.impONE) PrCO._ltrUP=1;	//zasygnalizuj wzrost przeplywu o 1 litr
		else PrCO._ltrUP=0;
	}
	if(tprz_co>=PrCO.impON)
    {
        PrCO.S_CO=1;                 //ustaw flage pobudzenia od CW
    }
	if(tprz_co<=PrCO.impOFF) 
    {
        PrCO.S_CO=0;				//wyzeruj flage pobudzenia od CW
    }
	PrCO.impCO=tprz_co;								//zapamietaj wielkosc przeplywu
	PrCO.CzCotim=0;
}
//-------------------------------------------------------------------------------
//Szybka reakcja na zanik przeplywu
//proc. wykonywana co 0.1 sek.
void ProcCO2(void)
{
	static unsigned int btpco;
	volatile unsigned int tpco;

	tpco=TMR0_ReadTimer();
	if(((tpco-btpco)==0)&&PrCO.S_CO) 				//zanik przeplywu?
	{
		PrCO.S_CO=0;								//wyzeruj flage pobudzenia od CW
		ProcCO1();				
	}
	else btpco=tpco;
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Przeplyw W.G. - procedura wykonywana w przerwaniu
/*
	if((TMR1IF)&&(TMR1IE))
	{
		TMR1IF=0;
		ReloadTimer1();		//obsluga zegara Timer1
		IncRTimer();		//obsluga zegarow RT
		IntCzCo();			//pomiar przeplywu W.U.
		StabPort();			//stabilizacja stanu okreslonych sygnalow
	}
	if((TMR0IF)&&(TMR0IE))
	{
		TMR0IF=0;
	}
*/
void IntCzCo(void)
{
    //if(DtKNF.kodkt==2)  //kociol 2F (przeplywowy)?
    {
        //if(DtKNF.tpcw)  //czujnik impulsowy?
        {
            if(PrCO.CzCotim<0xFFFF) PrCO.CzCotim++;							//timer czujnika przeplywu

            //Podstawowe flagi stanu i aktualny przeplyw	
            if(PrCO.CzCotim>=_OKCO) 									//minol czas 1 sek.?
            {
                ProcCO1();
                return;
            }
            //Szybka reakcja na zanik przeplywu 
            switch(PrCO.CzCotim)
            {
                case 80:
                case 70:
                case 60:
                case 50:
                case 40:
                case 30:
                case 20:
                case 10:
                {
                    ProcCO2();
                }
            }
        }
//        else
//        {
//            PrCO.CzCwtim=0;
//            PrCO.impCW=0;
//            PrCO._ltrDW=0;
//            PrCO._ltrUP=0;
//        }
    }
//    else
//    {
//        PrCO.CzCwtim=0;
//        PrCO.impCW=0;
//        PrCO._ltrDW=0;
//        PrCO._ltrUP=0;        
//    }
}
//-------------------------------------------------------------------------------
/*---------------------------------------------------------------------------------*/
#endif