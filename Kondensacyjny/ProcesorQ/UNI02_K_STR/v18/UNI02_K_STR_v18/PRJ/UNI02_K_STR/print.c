/*---------------------------------------------------------------------------------*/
/*print.c*/
/*Funkcje zapisu danych do 2 segmentowego wyswietlacza typu LED*/
/*w oparciu o procedury obslugi wyswietlacza zawarte w pliku wsw.c*/
/*Sposob reprezentacji danych - ITALY*/
/*---------------------------------------------------------------------------------*/
#include <xc.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>

#include "../../proj.tp.h"

#ifndef _TYP_PRJ
#error NIEOKRESLONY IDENTYFIKATOR PROJEKTU
#endif

#if _TYP_PRJ==1
#advisory _TYP_PRJ==1 

#include "global.h"
#include "main.h"
#include "global.ex.h"	

#include "print.h"				//parametry lokalne


extern tpRSDTA1 RSDT1;
 
//-------------------------------------------------------------------------------
static void inline InBitSet(volatile unsigned char *data, const unsigned char nbit,const unsigned char bt)
{
	*data=(bt)?(*data|(1<<nbit)):(*data&(~(1<<nbit)));
}
//-------------------------------------------------------------------------------
//---------------------------------------------------------
//---------------------------------------------------------
//Funkcje wyswietlania wg waznosci.
//ClsScr
//-PrintErr, PrintErrCs
//-PrintL3
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
	InBitSet(&RSDT1.outSTAT1,_ERR,pls);
	InBitSet(&RSDT1.outSTAT1,_MSG,!pls);
}
//---------------------------------------------------------
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
	InBitSet(&RSDT1.outSTAT1,_ERR,pls);
	InBitSet(&RSDT1.outSTAT1,_MSG,!pls);
}
//---------------------------------------------------------
//usuwa migajacy kod bledu
void EndErr(void)
{
	RSDT1.outERR=0;
	RSDT1.outAWR=0;
	InBitSet(&RSDT1.outSTAT1,_ERR,0);
	InBitSet(&RSDT1.outSTAT1,_MSG,0);
}
//---------------------------------------------------------
//---------------------------------------------------------
//wyswietla kod L3 naprzemiennie z temperatura CO
void PrintL3(void)
{
	InBitSet(&RSDT1.outSTAT1,_L3,1);
}
//---------------------------------------------------------
//usuwa wskaz komunikatu
void EndL3(void)
{
	InBitSet(&RSDT1.outSTAT1,_L3,0);
}
//---------------------------------------------------------
//---------------------------------------------------------
#endif
