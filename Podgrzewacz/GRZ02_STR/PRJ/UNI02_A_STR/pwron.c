/*pwron.c*/
#include <xc.h>
#include "../../proj.tp.h"

#ifndef _TYP_PRJ
#error NIEOKRESLONY IDENTYFIKATOR PROJEKTU
#endif

#if _TYP_PRJ==2
#advisory _TYP_PRJ==2 
#include "global.h"
#include "main.h"
#include "global.ex.h"

#include "pwron.h"
//-------------------------------------------------------------------------------
//Procedura oparta o zegar RTdS(_RTON) ktory musi byc zadeklarowany w pliku rtimer.c/h
//Czas na zanik stanow nieustalonych, diagnostyka wyjscia PWM pompy
void PwrOnDelay(void)
{
	StartRTdS(_RTON);
															//dioda w pelni rozswietlona
	while(RTdS(_RTON)<_TPWRON)
	{
		CLRWDT();
		if(RTdS(_RTON)<=7) WritePWM2(ProcToPmpPwm(80));		//dioda srednio rozswietlona	
		if(RTdS(_RTON)<15) WritePWM2(ProcToPmpPwm(100)); 	//dioda wygaszona
	};				
}
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
#endif
