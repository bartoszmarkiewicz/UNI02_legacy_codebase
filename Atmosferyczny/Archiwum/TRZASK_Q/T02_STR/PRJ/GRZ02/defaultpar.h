/*defaultpar.h*/
#ifndef	_DEFPAR_H
#define	_DEFPAR_H
#include "../../proj.tp.h"

#ifndef _TYP_PRJ
#error NIEOKRESLONY IDENTYFIKATOR PROJEKTU
#endif

#if _TYP_PRJ==3
#advisory _TYP_PRJ==3

#include "konfig.h"

//adresy danych przechowywanych w pamieci nieulotnej eeprom
#define _ADRERR 0x00C0		//kod awaryjny blokady trwalej
#define _ADRMST	0x00C1		//moc startowa
#define _ADRMCW	0x00C2		//moc maksymalna dla WU
#define _ADRPCW	0x00C3		//nastawa WU
#define _ADRRGZ 0x00C4		//wybor rodzaju gazu
#define _ADRWTK	0x00C5		//wybor typu kotla

//wartosci domyslne danych w pamieci (pierwsze programowanie)
#define _DTAERR	0			//kod awaryjny
#define _DTAMST	80			//moc startowa
#define _DTAMCW	99			//moc maksymalna dla WU
#define _DTAPCW	60			//nastawa WU

//Unico
#if _TYPKT==_UNCO
#define _DTARGZ _ZIEMNY		//wybor rodzaju gazu
#define _DTAWTK	_UNICO	//wybor typu kotla
#endif
//MiniMax
#if _TYPKT==_MMAXP
#define _DTARGZ _ZIEMNY		//wybor rodzaju gazu
#define _DTAWTK	_MONOTERM	//wybor typu kotla
#endif
//Miniterm
#if _TYPKT==_INWEST
#define _DTARGZ _ZIEMNY		//wybor rodzaju gazu
#define _DTAWTK	_BITERM		//wybor typu kotla
#endif
//MiniMax zasobnik
#if _TYPKT==_ZASOB
#define _DTARGZ _ZIEMNY		//wybor rodzaju gazu
#define _DTAWTK	_ZASOBNIK	//wybor typu kotla
#endif
//Maxiterm
#if _TYPKT==_INWEST2
#define _DTARGZ _ZIEMNY		//wybor rodzaju gazu
#define _DTAWTK	_BITERM2	//wybor typu kotla
#endif
#if _TYPKT==_GRZ01
#define _DTARGZ _ZIEMNY		//wybor rodzaju gazu
#define _DTAWTK	_BITERM		//wybor typu kotla
#endif
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//----------------------------------------------------------
#endif
#endif		/*_DEFPAR_H*/
