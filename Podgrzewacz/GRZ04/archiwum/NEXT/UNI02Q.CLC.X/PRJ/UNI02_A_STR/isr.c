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

#if _TYP_PRJ==2
#advisory _TYP_PRJ==2 
#include "../../mcc_generated_files\mcc.h"

#include "global.h"
#include "main.h"
#include "global.ex.h"

#include "isr.h"				//parametry lokalne

void inline ISRTimer1(void)
{
    IncRTimer();		//obsluga zegarow RT
    IncPmrTimer();		//obsluga zegarow proc. filtracji wart temp.
    IncRSTimer();		//obsluga timerow modulu MGS_RS
    IncRSTimer2();		//obsluga timerow modulu SMGS_RS  
}
void inline ISRTimer2(void)
{
    IntPWM();			//pwm1 - modulator
    IntPWM2();			//pwm2 - pompa    
}
void inline ISRTimer5(void)
{
    StabPort();			//stabilizacja stanu okreslonych sygnalow
    FormatDataIMY();     
}
//--------------------------------------------
//Przekierowanie procedur przerwan
void InitPrcISR()
{
    TMR1_SetInterruptHandler(ISRTimer1);
    TMR2_SetInterruptHandler(ISRTimer2);
    TMR5_SetInterruptHandler(ISRTimer5);
    I2C1_SlaveSetIsrHandler(I2CSlaveTR);        
    EUSART1_SetTxInterruptHandler(IntUSARTTR);
    EUSART1_SetRxInterruptHandler(IntUSARTRC);    
}
//--------------------------------------------
//oryginal
/*void interrupt ISR(void)
{
	if((RCIF)&&(RCIE))		//odbior RS232 (RCIF jest zerowane sprzetowo)
	{
		IntUSARTRC();
	}
	if((TXIF)&&(TXIE))		//transmisja RS232 (TXIF jest zerowane sprzetowo)
	{
		IntUSARTTR();		
	}
	if((TMR1IF)&&(TMR1IE))
	{
		TMR1IF=0;
		ReloadTimer1();		//obsluga zegara Timer1
		IncRTimer();		//obsluga zegarow RT
		IncPmrTimer();		//obsluga zegarow proc. filtracji wart temp.
		//IntCzCw();			//obsluga czujnika przeplywu CW
		//RefreshWSW();
		IncRSTimer();		//obsluga timerow modulu MGS_RS
		IncRSTimer2();		//obsluga timerow modulu SMGS_RS
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
		IntPWM2();
	}
	if((SSPIF)&&(SSPIE))	//obsluga przerwania od I2C
	{
		SSPIF=0;
		I2CSlaveTR();
	}
}*/
#endif
