/*pwm.h*/
//modulator SITA
#ifndef	_PWM_H
#define	_PWM_H
#include "pwm.tp.h"
#include "konfig.h"
/*---------------------------------------------------------------------------------*/
/*-----------------------------PUBLIC DATA-----------------------------------------*/
//okreslenie typu odbiornika 
#define _FPWM			4			//4 czestotliwosc przebiegu [kHz]
#define _NEG_PWM		0			//PWM1 odwrotma logika wyjscia? (max dla pwm=0, min dla pwm=255)
#define _NEG_PWM2		0			//PWM2 odwrotma logika wyjscia? (max dla pwm=0, min dla pwm=255)
//#define _VLO_PWM		31		    //18,31wypelnienie ponizej ktorego wentylator nie pracuje29

#define _VLO_PWM0		18		    //18 wypelnienie ponizej ktorego wentylator nie pracuje FIME
#define _VLO_PWM1		31		    //31 wypelnienie ponizej ktorego wentylator nie pracuje NG

//#define _MG_MOD			0xFA		//maksymalna dozwolona wartosc PWM1 przy ktorej osiagniety zostaje pelny wydatek pradu ukladu wyjsciowego
//#define _MG2_MOD2		0xFA		//wartosc PWM2 przy ktorej osiagniety zostaje pelny wydatek pradu ukladu wyjsciowego
//-----------------------------
//***********************************************************************************
//***********************************************************************************
/*
Pierwsze pobudzenie (_I80_MOD lub _I80_PMOD) na poziomie 60% zakresu modulacji
gaz ziemny
(X75-X30)=X45
/			=69/111*100%=60%
(X9f-X30)=X6F
gaz p³ynny
(xEB-x48)*60%+x48=x62+x48 =xAA
spr:
(xAA-x48)=x62
/			=98/163*100%=60%
(xEB-x48)=xA3
*/
//---------
//***********************************************************************************
//***********************************************************************************
//***********************************************************************************
//***********************************************************************************
//UNICO
//---------
//---------
/*
//parametry opisujace wlasciwosci modulatora
#define _MD_MOD0	0//0x30			//martwy zakres modulacji - dolna granica 
#define _MD_PMOD0	0//0x48			//martwy zakres modulacji - dolna granica (dla gazu plynnego)
#define _MG_MOD0	0XF0//0x9F			//martwy zakres modulacji - gorna granica
#define _MG_PMOD0	0XF0//0xEB			//martwy zakres modulacji - gorna granica (dla gazu plynnego)
#define _I80_MOD0	0x75			//60% domyslna wartosc pierwszego pobudzenia
#define _I80_PMOD0 	0xAA			//60% domyslna wartosc pierwszego pobudzenia - gaz plynny

#if _MG2_MOD<_MG_MOD0||_MG2_MOD<_MG_PMOD0
#error Nieprawidlowe wartosci parametrow _MG2_MOD,_MG_MOD,_MG_PMOD
#endif
#if _MD_MOD0>_MG_MOD0||_MD_PMOD0>_MG_PMOD0
#error Nieprawidlowe wartosci parametrow _MD_MOD,_MD_PMOD,_MG_MOD,_MG_PMOD
#endif
*/
//-----------------------------
//-----------------------------
/*
//parametry zwiazane z moca startowa
#define _GWMSTART0	99				//graniczna wartosc nastawy mocy startowej
//gaz ziemny
#define _MDSTART0 _MD_MOD0			//dolna granica wartosci PWM dla mocy startowej
#define _MGSTART0 _MG_MOD0+10			//gorna granica wartosci PWM dla mocy startowej
//gaz plynny
#define _MDSTARTP0 _MD_PMOD0			//dolna granica wartosci PWM dla mocy startowej
#define _MGSTARTP0 _MG_PMOD0+10			//gorna granica wartosci PWM dla mocy startowej
//---------
//parametry zwiazane z moca maksymalna CO
#define _GWMMAXCO0	99				//graniczna wartosc nastawy mocy maksymalnej CO
//gaz ziemny
#define _MDMMAXCO0 _MD_MOD0			//dolna granica wartosci PWM dla mocy maksymalnej CO
#define _MGMMAXCO0 _MG_MOD0+10			//gorna granica wartosci PWM dla mocy maksymalnej CO
//gaz plynny
#define _MDMMAXCOP0 _MD_PMOD0			//dolna granica wartosci PWM dla mocy maksymalnej CO
#define _MGMMAXCOP0 _MG_PMOD0+10			//gorna granica wartosci PWM dla mocy maksymalnej CO
//---------
//parametry zwiazane z moca maksymalna CW
#define _GWMMAXCW0	99				//graniczna wartosc nastawy mocy maksymalnej CW
//gaz ziemny
#define _MDMMAXCW0 _MD_MOD0			//dolna granica wartosci PWM dla mocy maksymalnej CW
#define _MGMMAXCW0 _MG_MOD0+10			//gorna granica wartosci PWM dla mocy maksymalnej CW
//gaz plynny
#define _MDMMAXCWP0 _MD_PMOD0			//dolna granica wartosci PWM dla mocy maksymalnej CW
#define _MGMMAXCWP0 _MG_PMOD0+10			//gorna granica wartosci PWM dla mocy maksymalnej CW
*/
//---------
//***********************************************************************************
//***********************************************************************************
//***********************************************************************************
//***********************************************************************************
//MINIMAX HB minimax z hydroblokiem
//---------
//---------
/*
//parametry opisujace wlasciwosci modulatora
#define _MD_MOD4	0//0x2D			//martwy zakres modulacji - dolna granica 
#define _MD_PMOD4	0//0x35			//martwy zakres modulacji - dolna granica (dla gazu plynnego)
#define _MG_MOD4	0xf0//0x9F			//martwy zakres modulacji - gorna granica
#define _MG_PMOD4	0xf0//0xF0			//martwy zakres modulacji - gorna granica (dla gazu plynnego)
#define _I80_MOD4	0x6E			//60% domyslna wartosc pierwszego pobudzenia
#define _I80_PMOD4 	0xA5			//60% domyslna wartosc pierwszego pobudzenia - gaz plynny

#if _MG2_MOD<_MG_MOD4||_MG2_MOD<_MG_PMOD4
#error Nieprawidlowe wartosci parametrow _MG2_MOD,_MG_MOD,_MG_PMOD
#endif
#if _MD_MOD4>_MG_MOD4||_MD_PMOD4>_MG_PMOD4
#error Nieprawidlowe wartosci parametrow _MD_MOD,_MD_PMOD,_MG_MOD,_MG_PMOD
#endif

//-----------------------------
//-----------------------------
//parametry zwiazane z moca startowa
#define _GWMSTART4	99					//graniczna wartosc nastawy mocy startowej
//gaz ziemny
#define _MDSTART4 _MD_MOD4				//dolna granica wartosci PWM dla mocy startowej
#define _MGSTART4 _MG_MOD4+10			//gorna granica wartosci PWM dla mocy startowej
//gaz plynny
#define _MDSTARTP4 _MD_PMOD4			//dolna granica wartosci PWM dla mocy startowej
#define _MGSTARTP4 _MG_PMOD4+10			//gorna granica wartosci PWM dla mocy startowej
//---------
//parametry zwiazane z moca maksymalna CO
#define _GWMMAXCO4	99					//graniczna wartosc nastawy mocy maksymalnej CO
//gaz ziemny
#define _MDMMAXCO4 _MD_MOD4				//dolna granica wartosci PWM dla mocy maksymalnej CO
#define _MGMMAXCO4 _MG_MOD4+10			//gorna granica wartosci PWM dla mocy maksymalnej CO
//gaz plynny
#define _MDMMAXCOP4 _MD_PMOD4			//dolna granica wartosci PWM dla mocy maksymalnej CO
#define _MGMMAXCOP4 _MG_PMOD4+10			//gorna granica wartosci PWM dla mocy maksymalnej CO
//---------
//parametry zwiazane z moca maksymalna CW
#define _GWMMAXCW4	99					//graniczna wartosc nastawy mocy maksymalnej CW
//gaz ziemny
#define _MDMMAXCW4 _MD_MOD4				//dolna granica wartosci PWM dla mocy maksymalnej CW
#define _MGMMAXCW4 _MG_MOD4+10			//gorna granica wartosci PWM dla mocy maksymalnej CW
//gaz plynny
#define _MDMMAXCWP4 _MD_PMOD4			//dolna granica wartosci PWM dla mocy maksymalnej CW
#define _MGMMAXCWP4 _MG_PMOD4+10		//gorna granica wartosci PWM dla mocy maksymalnej CW
//---------
*/
//***********************************************************************************
//***********************************************************************************
//***********************************************************************************
//***********************************************************************************
//***********************************************************************************
//***********************************************************************************
//***********************************************************************************
//***********************************************************************************
//Obliczenie wartosci PR2 dla wybranej czestotliwosci przebiegu PWM
/*
#ifndef _PLL					//modul PLL (0-nieaktywny,1-aktywny)
#error Nieokreslony parametr _PLL
#endif

//Dla _PLL=0 PR2 obliczamy ze wzoru [1/FPWM/(4*PRE2*1/FOSC)]-1
//Dla _PLL=1 PR2 obliczamy ze wzoru [1/FPWM/(PRE2*1/FOSC)]-1
//Gdzie:
//FOSC - czestotliwosc oscylatora
//FPWM - oczekiwana czestotliwosc przebiegu pwm
//PRE2 - wartosc preskalera dla timer2

#if (_PLL==0)&&((_FPWM==16)||(_FPWM==4)||(_FPWM==1))
	#if _FPWM==16
	#define _WT2CON	0b00000000		//timer2 preskaler = 1
	#define _WPR2	0x7C			//wartosc PR2 dla FPWM=16kHz,FOSC=8MHz,PRE2=1;
	#endif
	#if _FPWM==4
	#define _WT2CON	0b00000001		//timer2 preskaler = 4
	#define _WPR2	0x7C			//wartosc PR2 dla FPWM=4kHz,FOSC=8MHz,PRE2=4;
	#endif
	#if _FPWM==1
	#define _WT2CON	0b00000010		//timer2 preskaler = 16
	#define _WPR2	0x7C			//wartosc PR2 dla FPWM=1kHz,FOSC=8MHz,PRE2=1;
	#endif
	#else
	#if (_PLL==0)
	#error Nieprawidlowy parametr _FPWM 
	#endif 
#endif
//----------
#if (_PLL!=0)&&((_FPWM==16)||(_FPWM==4))
	#if _FPWM==16
	#define _WT2CON	0b00000001		//timer2 preskaler = 4
	#define _WPR2	0x7C			//wartosc PR2 dla FPWM=16kHz,FOSC=8MHz,PRE2=4;
	#endif
	#if _FPWM==4
	#define _WT2CON	0b00000010		//timer2 preskaler = 16
	#define _WPR2	0x7C			//wartosc PR2 dla FPWM=4kHz,FOSC=8MHz,PRE2=16;
	#endif
#else
	#if (_PLL!=0)
	#error Nieprawidlowy parametr _FPWM
	#endif
#endif
//----------------------------------------------------------
//Maksymalna rozdzielczosc PWM obliczamy ze wzoru
//Max PWM resolution=log(FOSC/FPWM)/log(2) [bits]
//Dla FOSC=8MHz i FPWM=16kHz wynosi ona 9 bitow.

*/
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
/*-----------------------------PROTECTED DATA--------------------------------------*/

//Maksymalna dozwolona wartosc wypelnienia zalezy od _WPR2
//Nalezy ja obliczyc biorac pod uwage ze wypelnienie jest wartoscia 10 bitowa
#define _WPR2 0x7C //patrz T2PR

#if(_WPR2<0x40)
#define _PSHIFT	2
#define _HI_PWM			(_WPR2+1)<<2	//gorna bezwgledna granica wypelnienia
#endif
#if(_WPR2>0x40 && _WPR2<0x80)
#define _PSHIFT	1
#define _HI_PWM			(_WPR2+1)<<1	//gorna bezwgledna granica wypelnienia
#endif
#if(_WPR2>0x80)
#define _PSHIFT	0
#define _HI_PWM			_WPR2+1			//gorna bezwgledna granica wypelnienia
#endif

//kontrola i formatowanie podanych parametrow
/*
#if (_MAG_MOD1>_HI_PWM)
#define _MAG_MOD1		_HI_PWM
#endif
#if (_MAG_MOD2>_HI_PWM)
#define _MAG_MOD2		_HI_PWM
#endif 
 */
//----------------------------------------------------------
//----------------------------------------------------------
#endif		/*_PWM_H*/
