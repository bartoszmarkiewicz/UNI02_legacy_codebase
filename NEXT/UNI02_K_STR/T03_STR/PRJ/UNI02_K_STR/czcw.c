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

#if _TYP_PRJ==1
#advisory _TYP_PRJ==1 
#include "../../mcc_generated_files\mcc.h"

#include "global.h"
#include "global.ex.h"

#include "czcw.h"				//parametry lokalne

extern DtKONF DtKNF;
DataPrCW PrCW;
	
/*---------------------------------------------------------------------------------*/ 
void InitCzCw(void)
{
//	TMR0IE=1;		//enable interrupt timer0
//	T08BIT=1;		//tryb 8 bitowy
//	T0CS=1;			//taktowany z RA4
//	T0SE=1;			//zbocze HI\LO
//	PSA=1;			//preskaler zostaje pominiety
//	TMR0=0;
    
	PrCW.CzCwtim=0;
	PrCW._ltrDW=0;
	PrCW._ltrUP=0;
	PrCW.S_CW=0;
	PrCW.impCW=0;
}

void SetParPrzCw(void)
{
    //ltrONE - liczba impulsow przypadajaca na przeplyw o natezeniu 1 litra
    //ltrON - liczba impulsow przypadajaca na przeplyw o natezeniu 3 litrow
    //ltrOFF - liczba impulsow przypadajaca na przeplyw o natezeniu 2.5 litra  
    switch(DtKNF.tpcw)      //rodzaj czujnika przeplywu
    {
        case 1:         //przetwornik przeplywu wody uzytkowej w hydrobloku Pakkens
        {
            PrCW.impONE=_ONE1;
            PrCW.impON=_ON_CW1;
            PrCW.impOFF=_OFF_CW1;
            break;
        }
        default:
        {
            PrCW.impONE=_ONE1;
            PrCW.impON=_ON_CW1;
            PrCW.impOFF=_OFF_CW1;           
        }
    }    
}

//-------------------------------------------------------------------------------
float ImpToPrz(const unsigned int imp)
{
    volatile float prz=0;
    switch(DtKNF.tpcw)      //rodzaj czujnika przeplywu
    {
        case 1:             //przetwornik przeplywu wody uzytkowej w hydrobloku Pakkens
        {
            if(imp)
            {
                prz=(imp+1.4553)/9.3808;
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
//Detekcja przeplywu w obiegu CW
void PomiarPrzCW(void)
{
    if(DtKNF.kodkt==2)  //kociol 2F (przeplywowy)?
    {
        switch(DtKNF.tpcw)      //rodzaj czujnika przeplywu
        {
            case 0:             //czujnik stykowy        
            {
                PrCW.przCW=0;
                PrCW.przCCW=0;
                PrCW.CzCwtim=0;
                PrCW._ltrDW=0;
                PrCW._ltrUP=0;
                PrCW.impCW=0;
                if(_PRZ_CW_GetValue())           //sygnal pobudzenia na wejsciu mikrokontrolera?
                {
                    PrCW.S_CW=0;                 //ustaw flage pobudzenia od CW
                }
                else
                {
                    PrCW.S_CW=1;                 //zeruj flage pobudzenia od CW
                }
                break;
            }
            default:            //przetwornik impulsowy
            {
                //PrCW.S_CW = flaga pobudzenia ustawiana w przerwaniu obslugi czujnika impulsowego
                PrCW.przCW=ImpToPrz(PrCW.impCW);
                if(PrCW.przCW>99.99)
                {
                    PrCW.przCW=99.99;
                }
                PrCW.przCCW = (unsigned int) (PrCW.przCW*100);
            }
        }
    }
    else
    {
        PrCW.przCW=0;
        PrCW.przCCW=0;
        PrCW.CzCwtim=0;
        PrCW._ltrDW=0;
        PrCW._ltrUP=0;
        PrCW.impCW=0;
        if(_PRZ_CW_GetValue())           //sygnal pobudzenia na wejsciu mikrokontrolera?
        {
            PrCW.S_CW=0;                 //ustaw flage pobudzenia od CW
        }
        else
        {
            PrCW.S_CW=1;                 //zeruj flage pobudzenia od CW
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
void ProcCW1(void)
{
	static unsigned int tprz_cw;
	unsigned int btprz;

	btprz=tprz_cw;
	tprz_cw=TMR0_ReadTimer();
    TMR0_WriteTimer(0);

	if((btprz-tprz_cw)>0)							//spadek przeplywu?
	{
		if((btprz-tprz_cw)>PrCW.impONE/2) PrCW._ltrDW=1;	//zasygnalizuj spadek przeplywu o 0.5 litra
		else PrCW._ltrDW=0;
	}
	else
	{
		if((tprz_cw-btprz)>PrCW.impONE) PrCW._ltrUP=1;	//zasygnalizuj wzrost przeplywu o 1 litr
		else PrCW._ltrUP=0;
	}
	if(tprz_cw>=PrCW.impON)
    {
        PrCW.S_CW=1;                 //ustaw flage pobudzenia od CW
    }
	if(tprz_cw<=PrCW.impOFF) 
    {
        PrCW.S_CW=0;				//wyzeruj flage pobudzenia od CW
    }
	PrCW.impCW=tprz_cw;								//zapamietaj wielkosc przeplywu
	PrCW.CzCwtim=0;
}
//-------------------------------------------------------------------------------
//Szybka reakcja na zanik przeplywu
//proc. wykonywana co 0.1 sek.
void ProcCW2(void)
{
	static unsigned int btpcw;
	volatile unsigned int tpcw;

	tpcw=TMR0_ReadTimer();
	if(((tpcw-btpcw)==0)&&PrCW.S_CW) 				//zanik przeplywu?
	{
		PrCW.S_CW=0;								//wyzeruj flage pobudzenia od CW
		ProcCW1();				
	}
	else btpcw=tpcw;
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Przeplyw W.U. - procedura wykonywana w przerwaniu
/*
	if((TMR1IF)&&(TMR1IE))
	{
		TMR1IF=0;
		ReloadTimer1();		//obsluga zegara Timer1
		IncRTimer();		//obsluga zegarow RT
		IntCzCw();			//pomiar przeplywu W.U.
		StabPort();			//stabilizacja stanu okreslonych sygnalow
	}
	if((TMR0IF)&&(TMR0IE))
	{
		TMR0IF=0;
	}
*/
void IntCzCw(void)
{
    if(DtKNF.kodkt==2)  //kociol 2F (przeplywowy)?
    {
        if(DtKNF.tpcw)  //czujnik impulsowy?
        {
            if(PrCW.CzCwtim<0xFFFF) PrCW.CzCwtim++;							//timer czujnika przeplywu

            //Podstawowe flagi stanu i aktualny przeplyw	
            if(PrCW.CzCwtim>=_OKCW) 									//minol czas 1 sek.?
            {
                ProcCW1();
                return;
            }
            //Szybka reakcja na zanik przeplywu 
            switch(PrCW.CzCwtim)
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
                    ProcCW2();
                }
            }
        }
        else
        {
            PrCW.CzCwtim=0;
            PrCW.impCW=0;
            PrCW._ltrDW=0;
            PrCW._ltrUP=0;
        }
    }
    else
    {
        PrCW.CzCwtim=0;
        PrCW.impCW=0;
        PrCW._ltrDW=0;
        PrCW._ltrUP=0;        
    }
}
//-------------------------------------------------------------------------------
/*---------------------------------------------------------------------------------*/
#endif