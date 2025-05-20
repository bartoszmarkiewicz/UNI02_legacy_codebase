/*--------------------------------------------------------------*/
/*isr.c*/
/*obsluga przerwan*/
/*--------------------------------------------------------------*/
#include <xc.h>
#include <stdbool.h>
#include <stdint.h>

#include "../../proj.tp.h"

#ifndef _TYP_PRJ
#error NIEOKRESLONY IDENTYFIKATOR PROJEKTU
#endif

#if _TYP_PRJ==4
#advisory _TYP_PRJ==4 
#include "../../mcc_generated_files\mcc.h"

#include "global.h"
#include "main.h"
#include "global.ex.h"

#include "isr.h"				//parametry lokalne
extern volatile unsigned char prz;

void inline ISRTimer1(void)
{
    IncTimTacho();      //obsluga zegara filtracji tacho
    IncRTimer();		//obsluga zegarow RT
    IncPmrTimer();		//obsluga zegarow proc. filtracji wart temp.   
}
/*void inline ISRTimer2(void)
{
    IntPWM();			//pwm1 - modulator
    IntPWM3();          //pwm3 - wentylator    
}*/
void inline ISRTimer5(void)
{
    //if(prz<99) prz++;
    //else prz=0;  
    RefreshWSW();		//obsluga wyswietlacza
    StabPort();			//stabilizacja stanu okreslonych sygnalow
    FormatDataIMY();     
}
//--------------------------------------------
//Przekierowanie procedur przerwan
void InitPrcISR()
{
    TMR1_SetInterruptHandler(ISRTimer1);
    //TMR2_SetInterruptHandler(ISRTimer2);
    TMR2_SetInterruptHandler(IntPWM);
    TMR3_SetInterruptHandler(IntTacho);
    TMR4_SetInterruptHandler(IntPWM3);
    TMR5_SetInterruptHandler(ISRTimer5);
    I2C1_SlaveSetIsrHandler(I2CSlaveTR);        
    EUSART1_SetTxInterruptHandler(IntUSARTTR);
    EUSART1_SetRxInterruptHandler(IntUSARTRC);  
    INT0_SetInterruptHandler(IntINT0);
}
//--------------------------------------------
//oryginal
/*void interrupt ISR(void)
{
	if((INT0IF)&&(INT0IE))
	{
		INT0IF=0;
		if(IntINT0())
		{
			ReloadTimer3();
			TMR3ON=1;
		}
	}
	if((TMR3IF)&&(TMR3IE))
	{
		TMR3IF=0;
		ReloadTimer3();			//obsluga zegara Timer3
		IntTacho();			//pomiar predkosci obrotowej
	}    
	if((TMR1IF)&&(TMR1IE))
	{
		TMR1IF=0;
		ReloadTimer1();		//obsluga zegara Timer1
		IncTimTacho();			//obsluga zegara filtracji tacho        
		IncRTimer();		//obsluga zegarow RT
		IncPmrTimer();		//obsluga zegarow proc. filtracji wart temp.
		RefreshWSW();		//obsluga wyswietlacza
		StabPort();		//stabilizacja stanu okreslonych sygnalow
		FormatDataIMY();
	}
	if((TMR0IF)&&(TMR0IE))
	{
		TMR0IF=0;
	}
	if((TMR2IF)&&(TMR2IE))
	{
		TMR2IF=0;
		IntPWM();
        IntPWM3();
	}
	if((SSPIF)&&(SSPIE))	//obsluga przerwania od I2C
	{
		SSPIF=0;
		I2CSlaveTR();
	}
}*/
#endif
