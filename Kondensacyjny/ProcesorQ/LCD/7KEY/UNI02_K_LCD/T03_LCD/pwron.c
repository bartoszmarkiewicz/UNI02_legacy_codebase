/*pwron.c*/
//#include <pic18.h>
#include <xc.h>
#include "global.h"			//parametry globalne
#include "pwron.h"			//parametry lokalne

//Procedura oparta o zegar RTdS(_RTON) ktory musi byc zadeklarowany w pliku rtimer.c/h
void PwrOnDelay(void)
{
	StartRTdS(_RTON);
	while(RTdS(_RTON)<_TPWRON)
	{
		CLRWDT();
	};
					
}
