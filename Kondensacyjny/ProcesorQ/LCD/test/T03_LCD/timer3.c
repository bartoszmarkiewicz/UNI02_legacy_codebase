/*--------------------------------------------------------------*/
/*timer1.c*/
/*obsluga i konfiguracja modulu Timer1*/
/*--------------------------------------------------------------*/
//#include <pic18.h>
#include <xc.h>
#include "global.h"					//parametry globalne
#include "timer3.h"					//parametry lokalne
/*--------------------------------------------------------------*/
/*-----------------------------PROTECTED DATA-------------------*/

const unsigned int RldT3 = _RLT3; //wartosc poczatkowa timera

/*--------------------------------------------------------------*/
//konfiguracja modulu Timer1

void InitTimer3(void)
{
    //----------------------
    //wartosc poczatkowa dla Timer1
    TMR3H = (unsigned char)(RldT3 >> 8);
    TMR3L = (unsigned char)(RldT3 & 0xFF);
    //----------------------
    //Intrerrupt
    TMR3IF = 0; //PIR tmr1 overflow interrupt flag bit
    TMR3IE = 1; //PIE tmr1 overflow interrupt enable bit
    TMR3IP = 1; //IPR tmr1 overflow interrupt is a high priority
    //----------------------
    //T1CON
    T3CONbits.T3RD16 = 1; //16 -bt Read/Write mode enable bt
    
    //Prescaler
#if _PRE3==1
    T3CONbits.T3CKPS1 = 0; //timer1 input clock prescale select bt
    T3CONbits.T1CKPS0 = 0;
#endif	
#if _PRE3==2
    T3CONbits.T3CKPS1 = 0;
    T3CONbits.T3CKPS0 = 1;
#endif
#if _PRE3==4
    T3CONbits.T3CKPS1 = 1;
    T3CONbits.T3CKPS0 = 0;
#endif	
#if _PRE3==8
    T3CONbits.T3CKPS1 = 1;
    T3CONbits.T3CKPS0 = 1;
#endif
    T3CONbits.TMR3CS = 0; //internal clock
    T3CONbits.TMR3ON = 1; //enables timer1
    //----------------------
}
/*--------------------------------------------------------------*/
//wartosc poczatkowa dla Timer1

void ReloadTimer3(void)
{
    TMR3H = (unsigned char)(RldT3 >> 8);
    TMR3L = (unsigned char)(RldT3 & 0xFF);
}
/*--------------------------------------------------------------*/