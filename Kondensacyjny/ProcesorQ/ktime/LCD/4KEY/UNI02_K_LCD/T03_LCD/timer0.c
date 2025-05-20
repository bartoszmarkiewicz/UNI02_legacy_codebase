/*--------------------------------------------------------------*/
/*timer0.c*/
/*obsluga i konfiguracja modulu Timer0*/
/*--------------------------------------------------------------*/
#include <pic18.h>
#include "global.h"					//parametry globalne
#include "timer0.h"					//parametry lokalne
#if (_EXT_BOARD>=3)
/*--------------------------------------------------------------*/
/*-----------------------------PROTECTED DATA-------------------*/
const unsigned int RldT0=0;		//wartosc poczatkowa timera
volatile unsigned char Tim0Type=0;  //tryb przerwania od timer0

extern volatile bit PFNf_firstEdge;		//pierwsze zbocze
//extern volatile bit PFNf_dataReady;		//pierwsze zbocze
//extern volatile bit PFNf_TimPeriod;     //znacznik przerwania po czasie TimPeriod
//extern volatile unsigned long data1;
extern volatile unsigned char db;

//konfiguracja modulu Timer1
//pojedynczy impuls co 4u
void InitTimer0(void)
{
//----------------------
//wartosc poczatkowa dla Timer1
	TMR0H=(unsigned char)(RldT0>>8);
	TMR0L=(unsigned char)(RldT0&0xFF);

//----------------------
//T1CON
	T08BIT=0;           //16 -bit
    T0CS=0;             //taktowany z internal clock    
#if _PRE0==0
    PSA=1               //prescaler wylaczony
#else        
    PSA=0;              //prescaler aktywny
    //Prescaler
    #if _PRE0==2
        T0PS2=0;			//timer1 input clock prescale select bt
        T0PS1=0;			//timer1 input clock prescale select bt
        T0PS0=0;			//timer1 input clock prescale select bt        
    #endif	
    #if _PRE0==4
        T0PS2=0;			//timer1 input clock prescale select bt
        T0PS1=0;			//timer1 input clock prescale select bt
        T0PS0=1;			//timer1 input clock prescale select bt
    #endif
    #if _PRE0==8
        T0PS2=0;			//timer1 input clock prescale select bt
        T0PS1=1;			//timer1 input clock prescale select bt
        T0PS0=0;			//timer1 input clock prescale select bt
    #endif	
    #if _PRE0==16
        T0PS2=0;			//timer1 input clock prescale select bt
        T0PS1=1;			//timer1 input clock prescale select bt
        T0PS0=1;			//timer1 input clock prescale select bt
    #endif
    #if _PRE0==32
        T0PS2=1;			//timer1 input clock prescale select bt
        T0PS1=0;			//timer1 input clock prescale select bt
        T0PS0=0;			//timer1 input clock prescale select bt
    #endif
    #if _PRE0==64
        T0PS2=1;			//timer1 input clock prescale select bt
        T0PS1=0;			//timer1 input clock prescale select bt
        T0PS0=1;			//timer1 input clock prescale select bt
    #endif
    #if _PRE0==128
        T0PS2=1;			//timer1 input clock prescale select bt
        T0PS1=1;			//timer1 input clock prescale select bt
        T0PS0=0;			//timer1 input clock prescale select bt
    #endif
    #if _PRE0==256
        T0PS2=1;			//timer1 input clock prescale select bt
        T0PS1=1;			//timer1 input clock prescale select bt
        T0PS0=1;			//timer1 input clock prescale select bt
    #endif
#endif
//----------------------	
	TMR0=0;             //internal clock        
        
//Intrerrupt
	TMR0IF=0;			//PIR tmr1 overflow interrupt flag bit
	TMR0IP=1;			//IPR tmr1 overflow interrupt is a high priority
	TMR0IE=0;			//PIE tmr1 overflow interrupt enable bit    
    
//----------------------    
	TMR0ON=1;			//enables timer0
}
unsigned int ReadTMR0(void)
{
    unsigned int tm;
    
    if(TMR0ON==1)
    {
        TMR0ON=0;			//disable timer0
        tm=TMR0;
        TMR0ON=1;			//enable/disable timer0
    }
    else
    {
        tm=TMR0;
    }
    return tm;
}
volatile unsigned char h,l;
/*
void WrtTMR0(volatile unsigned int tm)
{  
    if(TMR0ON==1)
    {
        while(TMR0!=tm)
        {
            TMR0ON=0;			//disable timer0 
            TMR0H=(unsigned char)((tm&0xFF00)>>8);
            TMR0L=(unsigned char)(tm&0x00FF);       
            TMR0=tm;
        }
        TMR0ON=1;			//enable/disable timer0
    }
    else
    {
        while(TMR0!=tm)
        {        
            TMR0H=(unsigned char)((tm&0xFF00)>>8);
            TMR0L=(unsigned char)(tm&0x00FF);    
            TMR0=tm;
        }
    }
}*/
void ReloadTMR0(volatile unsigned int tm)
{  
    if(TMR0ON==1)
    {
        while(TMR0!=tm)
        {
            TMR0ON=0;			//disable timer0 
            TMR0H=(unsigned char)((tm&0xFF00)>>8);
            TMR0L=(unsigned char)(tm&0x00FF);       
            TMR0=tm;
        }
        TMR0ON=1;			//enable/disable timer0
    }
    else
    {
        while(TMR0!=tm)
        {        
            TMR0H=(unsigned char)((tm&0xFF00)>>8);
            TMR0L=(unsigned char)(tm&0x00FF);    
            TMR0=tm;
        }
    }
}
/*---------------------------------------------------------------------------------*/
#endif //(_EXT_BOARD>=3)