/*--------------------------------------------------------------*/
/*rtime.c*/
/*zestaw zegarow czasu rzeczywistego*/
/*typy danych i parametry konfiguracyjne zdefiniowano w pliku rtimer.h*/
/*--------------------------------------------------------------*/
#include <xc.h>
#include <stdbool.h>
#include <stdint.h>

#include "../../proj.tp.h"

#ifndef _TYP_PRJ
#error NIEOKRESLONY IDENTYFIKATOR PROJEKTU
#endif

#if _TYP_PRJ==3
#advisory _TYP_PRJ==3

#include "global.h"
#include "main.h"

#include "rtimer.h"			//parametry lokalne
/*---------------------------------------------------------------------------------*/
/*-----------------------------PROTECTED DATA--------------------------------------*/

#if (_NRT_M)||(_NRT_G)||(_NRT_S)
volatile static near RTcomm		CommRT;
#endif
#if _NRT_cS
volatile near RTcSekundy	cSk[_NRT_cS];
#endif
#if _NRT_dS
volatile near RTdSekundy	dSk[_NRT_dS];
#endif
#if _NRT_S
volatile near RTsekundy	Sk[_NRT_S];
#endif
#if _NRT_M
volatile RTminuty	Mn[_NRT_M];			//near RTminuty	Mn[_NRT_M];
#endif
#if _NRT_G
volatile RTgodziny	Gd[_NRT_G];		//near RTgodziny	Gd[_NRT_G];
#endif

/*--------------------------------------------------------------*/
/*Funkcje obslugi timerow centy sekund*/
#if _NRT_cS
void StartRTcS(const unsigned char Index)
{
	if (Index>_NRT_cS-1) asm("RESET");	//obsluga sytuacji wyjatkowej
	cSk[Index]=0;
}
unsigned char RTcS(const unsigned char Index)
{
	if (Index>_NRT_cS-1) asm("RESET");	//obsluga sytuacji wyjatkowej
	return cSk[Index];
}
#endif
/*--------------------------------------------------------------*/
/*Funkcje obslugi timerow decy sekund*/
#if _NRT_dS
void StartRTdS(const unsigned char Index)
{
	if (Index>_NRT_dS-1) asm("RESET");	//obsluga sytuacji wyjatkowej
	dSk[Index].cS=0;
	dSk[Index].dS=0;
}
unsigned char RTdS(const unsigned char Index)
{
	if (Index>_NRT_dS-1) asm("RESET");	//obsluga sytuacji wyjatkowej
	return dSk[Index].dS;
}
#endif
/*--------------------------------------------------------------*/
/*Funkcje obslugi timerow sekund*/
#if _NRT_S
void StartRTS(const unsigned char Index)
{
	if (Index>_NRT_S-1) asm("RESET");		//obsluga sytuacji wyjatkowej
	Sk[Index].dS=0;
	Sk[Index].S=0;
}
unsigned char RTS(const unsigned char Index)
{
	if (Index>_NRT_S-1) asm("RESET");		//obsluga sytuacji wyjatkowej
	return Sk[Index].S;
}
#endif
/*--------------------------------------------------------------*/
/*Funkcje obslugi timerow minut*/
#if _NRT_M
void StartRTM(const unsigned char Index)
{
	if (Index>_NRT_M-1) asm("RESET");		//obsluga sytuacji wyjatkowej
	Mn[Index].S=0;
	Mn[Index].M=0;
}
unsigned char RTM(const unsigned char Index)
{
	if (Index>_NRT_M-1) asm("RESET");		//obsluga sytuacji wyjatkowej
	return Mn[Index].M;
}
unsigned char RTMs(const unsigned char Index)
{
	if (Index>_NRT_M-1) asm("RESET");		//obsluga sytuacji wyjatkowej
	return Mn[Index].S;
}
#endif
/*--------------------------------------------------------------*/
/*Funkcje obslugi timerow godzin*/
#if _NRT_G
void StartRTG(const unsigned char Index)
{
	if (Index>_NRT_G-1) asm("RESET");		//obsluga sytuacji wyjatkowej
	Gd[Index].M=0;
	Gd[Index].G=0;
}
unsigned char RTG(const unsigned char Index)
{
	if (Index>_NRT_G-1) asm("RESET");		//obsluga sytuacji wyjatkowej
	return Gd[Index].G;
}
#endif

/*---------------------------------------------------------------*/
/*Glowna funkcja obslugi timerow*/
void inline IncRTimer(void)
{
		#if (_NRT_M)||(_NRT_G)||(_NRT_S)||(_NRT_dS)||(_NRT_cS)
		unsigned char i;
		#endif
		
/*zakres czasu od 0,01 do 2,55 sek. z krokiem 0,01 sek. blad=+/-0,01 sek*/

		#if _NRT_cS
		for(i=0;i<_NRT_cS;i++)
			if(cSk[i]<255) cSk[i]++;
		#endif

/*zakres czasu od 0,1 do 25,5 sek. z krokiem 0,1 sek. blad=+/-0,01 sek*/

		#if _NRT_dS
		for(i=0;i<_NRT_dS;i++)
		{
			dSk[i].cS++;
			if(dSk[i].cS>9)
			{
				dSk[i].cS=0;
				if(dSk[i].dS<255) dSk[i].dS++;
			}
		}
		#endif

		#if (_NRT_S)||(_NRT_M)||(_NRT_G)
		
		CommRT.cS++;
		if(CommRT.cS>9)
		{
			CommRT.cS=0;
			CommRT.dS++;
			
/*zakres czasu od 1 do 255 sek. z krokiem 1 sek. blad=+/-0,1 sek*/

			#if _NRT_S
			for(i=0;i<_NRT_S;i++)
			{
				Sk[i].dS++;
				if(Sk[i].dS>9)
				{
					Sk[i].dS=0;
					if(Sk[i].S<255) Sk[i].S++;
				}
			}
			#endif
			
			if(CommRT.dS>9)
			{
				CommRT.dS=0;
				CommRT.S++;
				
/*zakres czasu od 1 do 255 min. z krokiem 1 min. blad=+/- 1 sek*/

				#if _NRT_M
				for(i=0;i<_NRT_M;i++)
				{
					Mn[i].S++;
					if(Mn[i].S>59)
					{
						Mn[i].S=0;
						if(Mn[i].M<255) Mn[i].M++;
					}
				}
				#endif

				if (CommRT.S>59)
				{
					CommRT.S=0;
					
/*zakres czasu od 1 do 255 godzin z krokiem 1 godz. blad=+/- 1 min*/

					#if _NRT_G
					for(i=0;i<_NRT_G;i++)
					{
						Gd[i].M++;
						if(Gd[i].M>59)
						{
							Gd[i].M=0;
							if(Gd[i].G<255) Gd[i].G++;
						}
					}
					#endif
				}
			}

		}
		#endif
	
}
/*---------------------------------------------------------------*/
#endif
