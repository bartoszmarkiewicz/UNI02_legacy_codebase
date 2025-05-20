/*--------------------------------------------------------------*/
/*timer2.c*/
/*obsluga i konfiguracja modulu Timer2*/
/*--------------------------------------------------------------*/
#include <pic18.h>
#include "global.h"					//parametry globalne

/*--------------------------------------------------------------*/
/*-----------------------------PROTECTED DATA-------------------*/
//konfiguracja modulu Timer1
void InitTimer2(void)
{
/*
	TMR0IE=1;		//enable interrupt timer0
	//TMR0IP=1;		//enable interrupt timer0
	T08BIT=1;		//tryb 8 bitowy
	T0CS=0;			//taktowany z internal clock
	T0SE=1;			//zbocze HI\LO
	PSA=1;			//preskaler zostaje przypisany
	T0PS2=0;		//:64
	T0PS1=0;
	T0PS1=0;
	TMR0=0;
	TMR0ON=1;	
*/
	TMR2IE=1;		//enable interrupt timer2
	T2OUTPS3=0;
	T2OUTPS2=0;
	T2OUTPS1=0;
	T2OUTPS0=0;
	T2CKPS1=0;
	T2CKPS0=0;
	TMR2=0;
	TMR2ON=1;
}

