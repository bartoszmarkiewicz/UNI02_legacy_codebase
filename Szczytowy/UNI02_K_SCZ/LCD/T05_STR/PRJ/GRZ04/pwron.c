/*pwron.c*/
#include <xc.h>
#include "../../proj.tp.h"

#ifndef _TYP_PRJ
#error NIEOKRESLONY IDENTYFIKATOR PROJEKTU
#endif

#if _TYP_PRJ==4
#advisory _TYP_PRJ==4 
#include "global.h"
#include "main.h"
#include "global.ex.h"

#include "pwron.h"
//-------------------------------------------------------------------------------
//Procedura oparta o zegar RTdS(_RTON) ktory musi byc zadeklarowany w pliku rtimer.c/h
//Czas na zanik stanow nieustalonych
void PwrOnDelay(void)
{
	StartRTdS(_RTON);
	while(RTdS(_RTON)<_TPWRON)
	{
		CLRWDT();
	};
}
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
#endif
