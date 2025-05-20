/*--------------------------------------------------------------*/
/*timer3.c*/
/*obsluga i konfiguracja modulu Timer3 oraz przerwania INT0*/

/*--------------------------------------------------------------*/
#include <pic18.h>
#include "global.h"					//parametry globalne
#include "timer3.h"					//parametry lokalne

/*--------------------------------------------------------------*/
/*-----------------------------PROTECTED DATA-------------------*/

const unsigned int RldT3=_RLT3;		//wartosc poczatkowa timera

/*--------------------------------------------------------------*/
//konfiguracja modulu Timer1
void InitTimer3(void)
{
//----------------------
//wartosc poczatkowa dla Timer1
	TMR3H=(unsigned char)(RldT3>>8);
	TMR3L=(unsigned char)(RldT3&0xFF);
//----------------------	
//Intrerrupt
	TMR3IF=0;			//PIR tmr3 overflow interrupt flag bit
	TMR3IE=1;			//PIE tmr3 overflow interrupt enable bit
	TMR3IP=1;			//IPR tmr3 overflow interrupt is a high priority
//----------------------
//T1CON
	T3RD16=1;				//16 -bt Read/Write mode enable bt
	
//Prescaler
#if _PRE3==1
	T3CKPS1=0;			//timer1 input clock prescale select bt
	T3CKPS0=0;
#endif	
#if _PRE3==2
	T3CKPS1=0;
	T3CKPS0=1;
#endif
#if _PRE3==4
	T3CKPS1=1;
	T3CKPS0=0;
#endif	
#if _PRE3==8
	T3CKPS1=1;
	T3CKPS0=1;
#endif
	TMR3CS=0;			//internal clock
	TMR3ON=1;			//enables timer1
//----------------------
//INT0
	INT0IE=1;			//enable INT0
	INTEDG0=1;			//narastajace zbocze
}
/*--------------------------------------------------------------*/
//wartosc poczatkowa dla Timer1
void ReloadTimer3(void)
{
	//TMR3H=(unsigned char)(RldT3>>8);
	//TMR3L=(unsigned char)(RldT3&0xFF);
	TMR3H=_RLT3H;
	TMR3L=_RLT3L;

}
