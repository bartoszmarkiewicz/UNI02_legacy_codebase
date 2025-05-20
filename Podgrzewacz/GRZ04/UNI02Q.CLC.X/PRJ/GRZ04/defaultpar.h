/*defaultpar.h*/
#ifndef	_DEFPAR_H
#define	_DEFPAR_H
#include "../../proj.tp.h"

#ifndef _TYP_PRJ
#error NIEOKRESLONY IDENTYFIKATOR PROJEKTU
#endif

#if _TYP_PRJ==4
#advisory _TYP_PRJ==4

#include "konfig.h"

//adresy danych przechowywanych w pamieci nieulotnej eeprom
#define _ADRERR 0x00C0		//kod awaryjny blokady trwalej
#define _ADRMST	0x00C1		//moc startowa
#define _ADRMCW	0x00C2		//moc maksymalna dla WU
#define _ADRPCW	0x00C3		//nastawa WU
#define _ADRRGZ 0x00C4		//wybor rodzaju gazu
#define _ADRWTK	0x00C5		//wybor typu kotla
#define _ADRMNV	0x00C6		//minimum PWM wentylatora
#define _ADRMXV	0x00C7		//maksimum PWM wentylatora
#define _ADRSTV	0x00C8		//startowe PWM wentylatora
#define _ADRLKM	0x00C9		//dlugosc komina
#define _ADRPWR	0x00CA		//moc podgrzewacza

#define _ADRPLO     0x0198		//P (lo) F88
#define _ADRPHI     0x0199		//P (hi)
#define _ADRILO     0x019A		//I (lo) F88
#define _ADRIHI     0x019B		//I (hi)
#define _ADRDLO     0x019C		//D (lo) F88
#define _ADRDHI     0x019D		//D (hi)



//wartosci domyslne danych w pamieci (pierwsze programowanie)
#define _DTAERR	0			//kod awaryjny
#define _DTAMCW	99			//moc maksymalna dla WU
#define _DTAPCW	60			//nastawa WU
#define _DTAMNV 10           //12 bezwzgledne minimum PWM na wentylatorze

#define _DTAMXV 35          //99bezwzgledne maksimum PWM na wentylatorze


#define _DTLKM	1           //dlugosc komina
#define _DTPWR	0           //moc podgrzewacza


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
#if _TYPKT==_GRZ04
#define _DTARGZ _ZIEMNY		//wybor rodzaju gazu
#define _DTAWTK	_BITERM		//wybor typu kotla
#endif

#if _DTPWR==0
//GRZEJNIK NISKOMOCOWY
#if _DTARGZ==0
#define _DTAMST	30			//moc startowa
#define _DTASTV 11          //22startowe PWM na wentylatorze
#endif
#if _DTARGZ==1
#define _DTAMST	40			//moc startowa
#define _DTASTV 10          //22startowe PWM na wentylatorze
#endif
#if _DTARGZ==2
#define _DTAMST	20			//moc startowa
#define _DTASTV 10          //22startowe PWM na wentylatorze
#endif

#else
//GRZEJNIK WYSOKOMOCOWY
#if _DTARGZ==0
#define _DTAMST	30			//moc startowa
#define _DTASTV 11          //22startowe PWM na wentylatorze
#endif
#if _DTARGZ==1
#define _DTAMST	30			//moc startowa
#define _DTASTV 10          //22startowe PWM na wentylatorze
#endif
#if _DTARGZ==2
#define _DTAMST	30			//moc startowa
#define _DTASTV 10          //22startowe PWM na wentylatorze
#endif

#endif



//UWAGA: F88
//1. liczby dodatnie:
//int=float*256
//--------------------!!!!!!
//2. liczby ujemne (z automatyczna konwersja na signed int 16bit):
//int=float(bez znaku)*256
//int=neg(int)
//------------------------
//3. liczby ujemne (bez automatycznej konwersji na signed int):
//int=float(bez znaku)*256
//int=neg(int)
//int=int+1

//PID (UWAGA wartosci z zakresu: )
//P=127.0+x;  x=P-127.0; dla P=60 x=-67.0 czyli F88(x)=0xBD00
//P=127.0+x;  x=P-127.0; dla P=80 x=-47.0 czyli F88(x)=0xD100
//P=127.0+x;  x=P-127.0; dla P=120 x=-7.0 czyli F88(x)=0xF900

//I=127.0+x;  x=I-127.0; dla I=0 x=-127.0 czyli F88(x)=0xFF81
//D=127.0+x;  x=I-127.0; dla D=0 x=-127.0 czyli F88(x)=0xFF81
#define _DEFPLO     0x00		//P (lo) F88 
#define _DEFPHI     0xF9		//P (hi)
#define _DEFILO     0x00		//I (lo) F88
#define _DEFIHI     0x81		//I (hi)
#define _DEFDLO     0x00		//D (lo) F88
#define _DEFDHI     0x81		//D (hi)
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//----------------------------------------------------------
#endif
#endif		/*_DEFPAR_H*/
