/*lled.c*/
//Procedury zwiazane z podswietleniem wyswietlacza
/*---------------------------------------------------------------------------------*/
//#include <pic18.h>
#include <xc.h>
#include "global.h"					//parametry globalne
#include "lled.h"					//parametry lokalne
#include "pwm2.tp.h"				

extern DtPWM PWM;
/*---------------------------------------------------------------------------------*/
//Inicjalizacja z maksymalnym podswietleniem	
void InitLLED(void)
{		
	PWM.LLed=_LLHIGH;
}		
/*---------------------------------------------------------------------------------*/
//Zwraca wartosc wypelnienia PWM dla podanej wartosci %
unsigned char ProcToLLedPwm(unsigned char prc)
{
	unsigned int p;

	if(prc>100) prc=100;
	p=PWM.HIPWM;
	p=(prc*p)/100;	//pmppwm/100 - ziarno przypadajace na 1%
	return (unsigned char)p;
}
/*---------------------------------------------------------------------------------*/
//Ustaw maksymalne rozswietlenie wyswietlacza
/*void LLedToHigh(void)
{
	if((PWM.LLed<_LLHIGH)&&(RTcS(_RTLLED2)>1))
	{
		StartRTcS(_RTLLED2);
		if((PWM.LLed+_LLLUP)<_LLHIGH)
		{	
	 		PWM.LLed+=_LLLUP;
		}	
		else
		{
			PWM.LLed=_LLHIGH;
		}
	}
	WritePWM2(ProcToLLedPwm(PWM.LLed));
}*/
void LLedToHigh(const unsigned char up)
{
	if((PWM.LLed<_LLHIGH)&&(RTcS(_RTLLED2)>1))
	{
		StartRTcS(_RTLLED2);
		if((PWM.LLed+up)<_LLHIGH)
		{
	 		PWM.LLed+=up;
		}
		else
		{
			PWM.LLed=_LLHIGH;
		}
	}
	WritePWM2(ProcToLLedPwm(PWM.LLed));
}
/*---------------------------------------------------------------------------------*/
//Powolne wygaszzenie rozswietlenia wyswietlacza
/*void LLedToLow(void)
{
	if((PWM.LLed>_LLLOW)&&(RTcS(_RTLLED2)>1))
	{
		StartRTcS(_RTLLED2);
		if((PWM.LLed-_LLLDW)>_LLLOW)
		{	
	 		PWM.LLed-=_LLLDW;
		}	
		else
		{
			PWM.LLed=_LLLOW;
		}
	}
	WritePWM2(ProcToLLedPwm(PWM.LLed));
}*/
void LLedToLow(const unsigned char dw)
{
	if((PWM.LLed>_LLLOW)&&(RTcS(_RTLLED2)>1))
	{
		StartRTcS(_RTLLED2);
		if((PWM.LLed-dw)>_LLLOW)
		{
	 		PWM.LLed-=dw;
		}
		else
		{
			PWM.LLed=_LLLOW;
		}
	}
	WritePWM2(ProcToLLedPwm(PWM.LLed));
}
/*---------------------------------------------------------------------------------*/
//Steruj rozswietleniem wyswietlacza
#if _KEY_BBOARD==0
void SetLightLLed(unsigned char nowhigh)
{
	if(nowhigh||RdPrt(S_KUP)||RdPrt(S_KDW)||RdPrt(S_SET)||RdPrt(S_OFF))
	{
		StartRTS(_RTLLED);
	}
	if(RTS(_RTLLED)<_TTLOW)
	{
		LLedToHigh(_LLLUP);
	}
	else
	{
		LLedToLow(_LLLDW);
	}
}
#endif
#if _KEY_BBOARD==1
void SetLightLLed(unsigned char nowhigh)
{
	if(nowhigh||RdPrt(S_KUP_CO)||RdPrt(S_KDW_CO)||RdPrt(S_KUP_CW)||RdPrt(S_KDW_CW)||RdPrt(S_SET)||RdPrt(S_KOM)||RdPrt(S_OFF))
	{
		StartRTS(_RTLLED);
	}
	if(RTS(_RTLLED)<_TTLOW)
	{
		LLedToHigh(_LLLUP);
	}
	else
	{
		LLedToLow(_LLLDW);
	}
}
#endif
/*---------------------------------------------------------------------------------*/
//Pulsacja rozswietleniem wyswietlacza
void SetPulseLLed(void)
{
    static unsigned char c=0;
    if(!c)
    {
        LLedToHigh(_FLLLUP);
        if((RTdS(_RTFLED)>5))
        {
            c=1;
            StartRTdS(_RTFLED);
        }
    }
    else
    {
        LLedToLow(_FLLLDW);
        if((RTdS(_RTFLED)>5))
        {
            c=0;
            StartRTdS(_RTFLED);
        }
    }
}
/*---------------------------------------------------------------------------------*/