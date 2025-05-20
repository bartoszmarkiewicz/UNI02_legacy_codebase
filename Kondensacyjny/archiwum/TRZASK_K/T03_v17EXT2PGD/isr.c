/*--------------------------------------------------------------*/
/*isr.c*/
/*obsluga przerwan*/
/*--------------------------------------------------------------*/
#include <pic18.h>
#include "global.h"				//parametry globalne
#include "isr.h"				//parametry lokalne

void interrupt ISR(void)
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
}
