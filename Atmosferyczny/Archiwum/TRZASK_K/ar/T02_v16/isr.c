/*--------------------------------------------------------------*/
/*isr.c*/
/*obsluga przerwan*/
/*--------------------------------------------------------------*/
#include <pic18.h>
#include "global.h"				//parametry globalne
#include "isr.h"				//parametry lokalne

void interrupt ISR(void)
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
}
