/*pwron.c*/
#include <pic18.h>
#include "global.h"			//parametry globalne
#include "pwron.h"			//parametry lokalne

//Procedura oparta o zegar RTdS(_RTON) ktory musi byc zadeklarowany w pliku rtimer.c/h

void PwrOnDelay(void)
{
    StartRTdS(_RTON);
    while(RTdS(_RTON) < _TPWRON)
    {
        CLRWDT();
		if(RTdS(_RTON)<=7) WritePWM2(ProcToPmpPwm(80));		//dioda srednio rozswietlona	
		if(RTdS(_RTON)<15) WritePWM2(ProcToPmpPwm(100)); 	//dioda wygaszona        
    };

}
