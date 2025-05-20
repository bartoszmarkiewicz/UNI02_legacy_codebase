/*czcw.c*/
//Procedury zwiazane z impulsowym czujnikiem przeplywu
/*---------------------------------------------------------------------------------*/
#include <pic18.h>
#include "global.h"					//parametry globalne
#include "czcw.h"					//parametry lokalne
/*---------------------------------------------------------------------------------*/

DataPrCW PrCW;
static unsigned char CzCwtim;		
/*---------------------------------------------------------------------------------*/ 
void InitCzCw(void)
{
	TMR0IE=1;		//enable interrupt timer0
	T08BIT=1;		//tryb 8 bitowy
	T0CS=1;			//taktowany z RA4
	T0SE=1;			//zbocze HI\LO
	PSA=1;			//preskaler zostaje pominiety
	TMR0=0;
	CzCwtim=0;
	PrCW._ltrDW=0;
	PrCW._ltrUP=0;
	PrCW.S_CW=0;
	PrCW.przCW=0;
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
	if(CzCwtim<255) CzCwtim++;							//timer czujnika przeplywu

	//Podstawowe flagi stanu i aktualny przeplyw	
	if(CzCwtim>=_OKCW) 									//minol czas 1 sek.?
	{
		ProcCW1();
		return;
	}
	//Szybka reakcja na zanik przeplywu 
	switch(CzCwtim)
	{
		case 80:
		case 70:
		case 60:
		case 50:
		case 40:
		case 30:
		case 20:
		case 10:
		ProcCW2();
	}
}
//-------------------------------------------------------------------------------
//Ustawia podstawowe flagi i zapamietuje aktualny przeplyw
//proc. wykonywana co 1 sek.
void ProcCW1(void)
{
	static unsigned char tprz_cw;
	unsigned char btprz;

	btprz=tprz_cw;
	tprz_cw=TMR0L;
	if((btprz-tprz_cw)>0)							//spadek przeplywu?
	{
		if((btprz-tprz_cw)>_NIMP/2) PrCW._ltrDW=1;	//zasygnalizuj spadek przeplywu
		else PrCW._ltrDW=0;
	}
	else
	{
		if((tprz_cw-btprz)>_NIMP) PrCW._ltrUP=1;	//zasygnalizuj wzrost przeplywu
		else PrCW._ltrUP=0;
	}
	if(tprz_cw>=_SETCW) PrCW.S_CW=1;				//ustaw flage pobudzenia od CW
	if(tprz_cw<=_CLRCW) PrCW.S_CW=0;				//wyzeruj flage pobudzenia od CW
	PrCW.przCW=tprz_cw;								//zapamietaj wielkosc przeplywu
	CzCwtim=0;
}
//-------------------------------------------------------------------------------
//Szybka reakcja na zanik przeplywu
//proc. wykonywana co 0.1 sek.
void ProcCW2(void)
{
	static unsigned char btpcw;
	unsigned char tpcw;

	tpcw=TMR0L;
	if(((tpcw-btpcw)==0)&&PrCW.S_CW) 				//zanik przeplywu?
	{
		PrCW.S_CW=0;								//wyzeruj flage pobudzenia od CW
		ProcCW1();				
	}
	else btpcw=tpcw;
}
//-------------------------------------------------------------------------------
