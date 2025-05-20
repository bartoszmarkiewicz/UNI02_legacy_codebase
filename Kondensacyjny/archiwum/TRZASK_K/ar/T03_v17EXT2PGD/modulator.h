/*modulator.h*/
#ifndef	_MODULAT_H
#define	_MODULAT_H
#include "modulator.tp.h"		//typy danych
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//---------------------------------------------
//Parametry pojedynczego warunku
#define _POMIN		0b00000000
//--------
#define _CHRMASK	0b11100000
#define _WIEKSZE	0b01000000
#define _ROWNE		0b01100000
#define _MNIEJSZE	0b00100000
//--------
#define _ROSNIE		0b11000000
#define _STOI		0b11100000
#define _MALEJE		0b10100000
#define _PARMASK	0b00011111
//---------------------------------------------
//Parametry zespolu warunkow
#define _AND0		0b01000000
#define _NEG0		0b10000000
//--------
#define _AND1		0b00010000
#define _NEG1		0b00100000
//--------
#define _AND2		0b00000100
#define _NEG2		0b00001000
//--------
#define _NEG3		0b00000010
//---------------------------------------------
//Rozmir tablicy pierwszych pobudzen
#define NG_NSPR 8			//rozmiar tabeli pobydzen NG_NSPR x NG_NSPR
#define NG_PNSPR NG_NSPR+1	//rozmiar tabeli wartosci nastawy i przeplywu NG_NSPR x 2
#define pMASKA		0		//pozycja tabeli nastaw i przeplywow gdzie znajdyje sie maska aktywacji
#define wNASTAWA	0		//nr wiersza z wartosciami nastaw
#define wPRZEPLYW	1		//nr wiersza z wartosciami przeplywu
#define wZIEMNY		0		//nr wiersza tabeli ProcToPWM dla gazu ziemnego
#define wPLYNNY		1		//nr wiersza tabeli ProcToPWM dla gazu plynnego

//--------------------------------------------------------------
//--------------------------------------------------------------
#endif	/*_MODULAT_H*/
