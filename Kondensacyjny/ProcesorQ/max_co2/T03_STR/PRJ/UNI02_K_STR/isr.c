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

#if _TYP_PRJ==1
#advisory _TYP_PRJ==1 
#include "../../mcc_generated_files\mcc.h"

#include "global.h"
#include "main.h"
#include "global.ex.h"

#include "isr.h"				//parametry lokalne

void inline ISRTimer1(void)
{
#if _DEMO==0    
    IncTimTacho();      //obsluga zegara filtracji tacho
#endif
    IncRTimer();		//obsluga zegarow RT
    IncPmrTimer();		//obsluga zegarow proc. filtracji wart temp.
    IncRSTimer();		//obsluga timerow modulu MGS_RS
    IncRSTimer2();		//obsluga timerow modulu SMGS_RS
    IntCzCw();			//pomiar przeplywu W.U.
}
/*void inline ISRTimer2(void)
{
    //IntPWM3();			//pwm3 - vnt
    IntPWM2();			//pwm2 - pompa    
}
void inline ISRTimer4(void)
{
    IntPWM3();			//pwm3 - vnt
    //IntPWM2();			//pwm2 - pompa    
}*/
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
    //TMR2_SetInterruptHandler(ISRTimer2);
    TMR2_SetInterruptHandler(IntPWM2);
    TMR3_SetInterruptHandler(IntTacho);  
    //TMR4_SetInterruptHandler(ISRTimer4);
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
	static unsigned int n,m,j;

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
		IncTimTacho();			//obsluga zegara filtracji tacho
		IncRTimer();		//obsluga zegarow RT
		IncPmrTimer();		//obsluga zegarow proc. filtracji wart temp.
		//IntCzCw();			//obsluga czujnika przeplywu CW
		IncRSTimer();		//obsluga timerow modulu MGS_RS
		IncRSTimer2();		//obsluga timerow modulu SMGS_RS
		StabPort();			//stabilizacja stanu okreslonych sygnalow
		FormatDataIMY();

	}
	if((TMR0IF)&&(TMR0IE))
	{
		TMR0IF=0;
	}
	if((TMR2IF)&&(TMR2IE))
	{
		TMR2IF=0;
		IntPWM();			//pwm1 - modulator, vnt
		IntPWM2();			//pwm2 - pompa
	}

	if((SSPIF)&&(SSPIE))	//obsluga przerwania od I2C
	{
		SSPIF=0;
		I2CSlaveTR();
	}
}*/
#endif
