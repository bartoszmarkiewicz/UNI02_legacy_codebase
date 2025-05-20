/*keypad.c*/
//Funkcje obslugi przycisku RESET dla celow konfiguracji
// (!) Bazuje na funkcjach z plikow: rtimer.c
/*---------------------------------------------------------------------------------*/
#include <pic18.h>
#include "global.h"			//parametry globalne
#include "keypad.h"			//parametry lokalne
/*---------------------------------------------------------------------------------*/
StPin sp;
/*---------------------------------------------------------------------------------*/
void SetBufKey(void)
{
	sp.sbk=1;
}
void ResBufKey(void)
{
	sp.sbk=0;
}
#if _EN_PK==1
//Funkcja detekcji nacisniecia przycisku (wersja podstawowa)
//k-fizyczne wejscie procesora dla przycisku
unsigned char PresKey(unsigned char k)
{
	sp.bk=(k)?1:0;
	if (!sp.sbk&&sp.bk)
	{
		sp.sbk=(sp.bk)?1:0;
		return 1;
	}
	sp.sbk=(sp.bk)?1:0;
	return 0;	
}
#endif
/*---------------------------------------------------------------------------------*/
#if _EN_DPK==1
//Funkcja detekcji nacisniecia przycisku (wersja podstawowa)
//k-fizyczne wejscie procesora dla przycisku
unsigned char DoublePresKey(unsigned char k)
{
	static bit c,t;
	
	c=PresKey(k); 
	if(!sp.dk&&c) 			//pierwsze nacisniecie przycisku?
	{
		sp.dk=1;
		StartRTdS(_RTKEY);
		return 0;
	}
	t=(RTdS(_RTKEY)<=5);
	if(sp.dk&&t) 						
	{
		if(c)					//drugie nacisniecie przycisku?
		{	
			sp.dk=0;
			return 1;
		}
	}
	if(!t) sp.dk=0;		//czas uplynol?
	return 0;	
}
#endif
/*---------------------------------------------------------------------------------*/
#if _EN_EXPK==1
//Funkcja detekcji nacisniecia przycisku (wersja zaawansowana)
//dla danych liczbowych (<2.5 cyfry)
//k-fizyczne wejscie procesora dla przycisku,
//dTim-czas po jakim przy nacisnietym przycisku nastapi wygenerowanie ciagu impulsów [*0.1sek],
//pTim-czas miedzy kolejnymi impulsami przy spelnieniu warunku dTim [*0.1sek]
//zwraca 0 dla przycisku w stanie spoczynku
//zwraca 1 dla pojedynczego nacisniecia przycisku
//zwraca 2 dla impulsu w trakcie podtrzymania pzrycisku
unsigned char ExPresKey(unsigned char k,unsigned char dTim,unsigned char pTim)
{
	sp.bk=(k)?1:0;
	
	if (!sp.sbk&&sp.bk)
	{
		sp.sbk=(sp.bk)?1:0;
		StartRTdS(_RTKEY);
		return 1;
	}
	sp.sbk=(sp.bk)?1:0;
	if(sp.sbk&&sp.bk)
	{
		if(!sp.ap&&RTdS(_RTKEY)>=dTim) 
		{
			sp.ap=1;
			StartRTdS(_RTKEY);
		}
		if(sp.ap&&RTdS(_RTKEY)>=pTim)
		{
			StartRTdS(_RTKEY);
			return 2;
		}
	}else sp.ap=0;
	return 0;	
}
#endif
/*---------------------------------------------------------------------------------*/
#if _EN_EXPK2==1
//Funkcja detekcji nacisniecia przycisku (wersja b.zaawansowana)
//dla duzych danych liczbowych (>2.5 cyfry)
//k-fizyczne wejscie procesora dla przycisku,
//dTim-czas po jakim przy nacisnietym przycisku nastapi wygenerowanie ciagu impulsów [*0.1sek],
//pTim-czas miedzy kolejnymi impulsami przy spelnieniu warunku dTim [*0.1sek]
//zwraca 0 dla przycisku w stanie spoczynku
//zwraca 1 dla pojedynczego nacisniecia przycisku
//zwraca 2 dla impulsu w trakcie podtrzymania pzrycisku
//zwraca 3 miedzy impulsami w trakcie podtrzymania pzrycisku 
unsigned char ExPresKey2(unsigned char k,unsigned char dTim,unsigned char pTim)
{
	sp.bk=(k)?1:0;
	
	if(sp.fl&&sp.sbk&&!sp.bk)
	{
		sp.fl=0;
		sp.sbk=(sp.bk)?1:0;
	}
	if (!sp.sbk&&sp.bk) StartRTdS(_RTKEY);	
		
	if (!sp.fl&&sp.sbk&&!sp.bk)		
	{
		sp.sbk=(sp.bk)?1:0;	
		StartRTdS(_RTKEY);			
		return 1;
	}
	if(sp.sbk&&sp.bk)				
	{
		
		if(!sp.ap&&RTdS(_RTKEY)>=dTim) 
		{
			sp.fl=1;
			sp.ap=1;
		}
		if(sp.ap&&RTdS(_RTKEY)>=pTim)
		{
			StartRTdS(_RTKEY);
			return 2;
		}
	}else sp.ap=0;	
	sp.sbk=(sp.bk)?1:0;	
	if (sp.ap) return 3;
	return 0;	
}
#endif
/*---------------------------------------------------------------------------------*/
