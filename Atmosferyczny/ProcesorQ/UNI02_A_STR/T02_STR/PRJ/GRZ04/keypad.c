/*keypad.c*/
//Funkcje obslugi przycisku RESET dla celow konfiguracji
// (!) Bazuje na funkcjach z plikow: rtimer.c
/*---------------------------------------------------------------------------------*/
#include <xc.h>
#include "../../proj.tp.h"

#ifndef _TYP_PRJ
#error NIEOKRESLONY IDENTYFIKATOR PROJEKTU
#endif

#if _TYP_PRJ==4
#advisory _TYP_PRJ==4

#include "global.h"
#include "global.ex.h"

#include "keypad.h"			//parametry lokalne

/*---------------------------------------------------------------------------------*/
StPin sp;
#if _EN_EXPK2B==1
StPin sp2;
#endif
#if _EN_PK1==1
StPin ky1;
#endif
#if _EN_PK2==1
StPin ky2;
#endif
/*---------------------------------------------------------------------------------*/
void SetBufKey(void)
{
	sp.sbk=1;
}
void ResBufKey(void)
{
	sp.sbk=0;
}
#if _EN_PK1==1
//Funkcja detekcji nacisniecia przycisku (wersja podstawowa)
//k-fizyczne wejscie procesora dla przycisku
void SetBufKey1(void)
{
	ky1.sbk=1;
}
void ResBufKey1(void)
{
	ky1.sbk=0;
}
unsigned char PresKey1(unsigned char k)
{
	ky1.bk=(k)?1:0;
	if (!ky1.sbk&&ky1.bk)
	{
		ky1.sbk=(ky1.bk)?1:0;
		return 1;
	}
	ky1.sbk=(ky1.bk)?1:0;
	return 0;	
}
#endif
#if _EN_PK2==1
//Funkcja detekcji nacisniecia przycisku (wersja podstawowa)
//k-fizyczne wejscie procesora dla przycisku
void SetBufKey2(void)
{
	ky2.sbk=1;
}
void ResBufKey2(void)
{
	ky2.sbk=0;
}
unsigned char PresKey2(unsigned char k)
{
	ky2.bk=(k)?1:0;
	if (!ky2.sbk&&ky2.bk)
	{
		ky2.sbk=(ky2.bk)?1:0;
		return 1;
	}
	ky2.sbk=(ky2.bk)?1:0;
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
#if _EN_EXPK2B==1
//Funkcja detekcji nacisniecia przycisku (wersja b.zaawansowana)
//dla duzych danych liczbowych (>2.5 cyfry)
//k-fizyczne wejscie procesora dla przycisku,
//dTim-czas po jakim przy nacisnietym przycisku nastapi wygenerowanie ciagu impulsów [*0.1sek],
//pTim-czas miedzy kolejnymi impulsami przy spelnieniu warunku dTim [*0.1sek]
//zwraca 0 dla przycisku w stanie spoczynku
//zwraca 1 dla pojedynczego nacisniecia przycisku
//zwraca 2 dla impulsu w trakcie podtrzymania pzrycisku
//zwraca 3 miedzy impulsami w trakcie podtrzymania pzrycisku 
unsigned char ExPresKey2B(unsigned char k,unsigned char dTim,unsigned char pTim)
{
	sp2.bk=(k)?1:0;
	
	if(sp2.fl&&sp2.sbk&&!sp2.bk)
	{
		sp2.fl=0;
		sp2.sbk=(sp2.bk)?1:0;
	}
	if (!sp2.sbk&&sp2.bk) StartRTdS(_RTKEY2);	
		
	if (!sp2.fl&&sp2.sbk&&!sp2.bk)		
	{
		sp2.sbk=(sp2.bk)?1:0;	
		StartRTdS(_RTKEY2);			
		return 1;
	}
	if(sp2.sbk&&sp2.bk)				
	{
		
		if(!sp2.ap&&RTdS(_RTKEY2)>=dTim) 
		{
			sp2.fl=1;
			sp2.ap=1;
		}
		if(sp2.ap&&RTdS(_RTKEY2)>=pTim)
		{
			StartRTdS(_RTKEY2);
			return 2;
		}
	}else sp2.ap=0;	
	sp2.sbk=(sp2.bk)?1:0;	
	if (sp2.ap) return 3;
	return 0;	
}
#endif
/*---------------------------------------------------------------------------------*/
#endif
