/*--------------------------------------------------------------*/
/*mux.c*/
/*naprzemienna zmiana stanu wybranego wejscia*/
/*wygaszenie portu obslugi wyswietlacza*/
/*--------------------------------------------------------------*/
#include <pic18.h>
#include "global.h"				//parametry globalne
#include "mux.h"				//parametry lokalne

void SetResMux(void)
{
#if WKATODA==0
	PORT_WSW=0xFF;				//wygas port wyswietlacza
#else 
	PORT_WSW=0x00;
#endif
	MUX=!MUX;					//zmien stan wyjscia MUX
}
/*--------------------------------------------------------------*/
