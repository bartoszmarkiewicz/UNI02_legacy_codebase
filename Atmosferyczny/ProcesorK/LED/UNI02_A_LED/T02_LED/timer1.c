/*--------------------------------------------------------------*/
/*timer1.c*/
/*obsluga i konfiguracja modulu Timer1*/
/*--------------------------------------------------------------*/
#include <pic18.h>
#include "global.h"					//parametry globalne
#include "timer1.h"					//parametry lokalne

/*--------------------------------------------------------------*/
/*-----------------------------PROTECTED DATA-------------------*/

const unsigned int RldT1 = _RLT1; //wartosc poczatkowa timera

/*--------------------------------------------------------------*/
//konfiguracja modulu Timer1

void InitTimer1(void)
{
    //----------------------
    //wartosc poczatkowa dla Timer1
    TMR1H = (unsigned char)(RldT1 >> 8);
    TMR1L = (unsigned char)(RldT1 & 0xFF);
    //----------------------
    //Intrerrupt
    TMR1IF = 0; //PIR tmr1 overflow interrupt flag bit
    TMR1IE = 1; //PIE tmr1 overflow interrupt enable bit
    TMR1IP = 1; //IPR tmr1 overflow interrupt is a high priority
    //----------------------
    //T1CON
    T1RD16 = 1; //16 -bt Read/Write mode enable bt

    //Prescaler
#if _PRE==1
    T1CKPS1 = 0; //timer1 input clock prescale select bt
    T1CKPS0 = 0;
#endif	
#if _PRE==2
    T1CKPS1 = 0;
    T1CKPS0 = 1;
#endif
#if _PRE==4
    T1CKPS1 = 1;
    T1CKPS0 = 0;
#endif	
#if _PRE==8
    T1CKPS1 = 1;
    T1CKPS0 = 1;
#endif
    TMR1CS = 0; //internal clock
    TMR1ON = 1; //enables timer1
    //----------------------
}
/*--------------------------------------------------------------*/
//wartosc poczatkowa dla Timer1

void ReloadTimer1(void)
{
    TMR1H = (unsigned char)(RldT1 >> 8);
    TMR1L = (unsigned char)(RldT1 & 0xFF);
}
