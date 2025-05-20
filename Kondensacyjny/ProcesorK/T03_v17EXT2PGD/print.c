/*---------------------------------------------------------------------------------*/
/*print.c*/
/*Funkcje zapisu danych do 2 segmentowego wyswietlacza typu LED*/
/*w oparciu o procedury obslugi wyswietlacza zawarte w pliku wsw.c*/
/*Sposob reprezentacji danych - ITALY*/
/*---------------------------------------------------------------------------------*/
#include <pic18.h>
#include "global.h"				//parametry globalne
#include "print.h"				//parametry lokalne
#include "pomiar.tp.h"
#include "main.tp.h"

extern tpRSDTA1 RSDT1;
//wsw.c
extern WPack PWD[];		//tablica priorytetow wyswietlania PWD[n] (n=0 - najwyzszy priorytet)
extern WPack DWD[];		//tablica danych modyfikowanych dynamicznie wystepujacych wylacznie w kolejkach
extern const unsigned char Znak[];
extern DataCO DtCO;		//pomiar.c 
//---------------------------------------------------------
//---------------------------------------------------------
//Funkcje wyswietlania wg waznosci.
//ClsScr
//-PrintErr
//--PrintPR,PrintWPR
//---PrintNstCW
//----PrintNstCO
//----PrintNstKT
//-----PrintMsg
//------PrintTDZ
//-------PrintOFF
//--------PrintCS
//---------PrintCW
//----------PrintCO

//---------------------------------------------------------
//---------------------------------------------------------
//---------------------------------------------------------
//---------------------------------------------------------
//wyswietla migajacy kod bledu
//kod - parametr okreslajacy migajacy kod bledu do wyswietlenia
//pls - dla 1 dioda error miga, dla 0 dioda error zapalona
void PrintErr(const unsigned char kod, const unsigned char pls)
{
	if(pls) 
	{
		RSDT1.outERR=kod;
		RSDT1.outAWR=0;
	}
	else
	{
		RSDT1.outERR=0;
		RSDT1.outAWR=kod;
	}
	BitSet2(&RSDT1.outSTAT1,_ERR,pls);
	BitSet2(&RSDT1.outSTAT1,_MSG,!pls);
}
void PrintErrCs(const unsigned char kod,const unsigned char cs, const unsigned char pls)
{
	if(pls) 
	{
		RSDT1.outERR=kod;
		RSDT1.outAWR=0;
	}
	else
	{
		RSDT1.outERR=0;
		RSDT1.outAWR=kod;
	}
	BitSet2(&RSDT1.outSTAT1,_ERR,pls);
	BitSet2(&RSDT1.outSTAT1,_MSG,!pls);
}

//usuwa migajacy kod bledu
void EndErr(void)
{
	RSDT1.outERR=0;
	RSDT1.outAWR=0;
	BitSet2(&RSDT1.outSTAT1,_ERR,0);
	BitSet2(&RSDT1.outSTAT1,_MSG,0);
	//ResetKol(_WS_ERR);
}
//---------------------------------------------------------
//---------------------------------------------------------
//wyswietla kod L3 naprzemiennie z temperatura CO
void PrintL3(void)
{
	BitSet2(&RSDT1.outSTAT1,_L3,1);
	/*SetData(&DWD[5],led_L,led_3);
	SetStatus(&DWD[5],tLED|tFOCUS,tALL);				//L
	if(DtCO._err_CO)
	{
		SetData(&DWD[6],led_L,led_3);
		SetStatus(&DWD[6],tLED|tFOCUS,tALL);				//3
	}
	else
	{
		SetData(&DWD[6],0,DtCO.SCO);
		SetStatus(&DWD[6],tDEC|tFOCUS,tALL);				//3
	}
	SetPoz(_WS_MSG,0);
	SetPoz(_WS_MSG,1);*/
}
//----------
//usuwa wskaz komunikatu
void EndL3(void)
{
	BitSet2(&RSDT1.outSTAT1,_L3,0);
	//ResetKol(_WS_MSG);
}
//---------------------------------------------------------
//---------------------------------------------------------
