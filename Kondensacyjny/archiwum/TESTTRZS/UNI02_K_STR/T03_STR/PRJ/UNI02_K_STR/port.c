/*port.c*/
//Konfiguracja portow fizycznych mikrokontrolera
//Stabilizacja stanow wejsciowych
/*---------------------------------------------------------------------------------*/
#include <xc.h>
#include <stdbool.h>
#include <stdint.h>

#include "../../proj.tp.h"

#ifndef _TYP_PRJ
#error NIEOKRESLONY IDENTYFIKATOR PROJEKTU
#endif

#if _TYP_PRJ==1
#advisory _TYP_PRJ==1 
#include "../../mcc_generated_files\mcc.h"

#include "global.h"
#include "main.h"
#include "global.ex.h"

#include "port.h"			//parametry lokalne

extern tpMAIN M;
extern DataCO DtCO;
extern DataCW DtCW;
extern DataCSN DtCS;
extern DataZW DtZW;
extern DataIN DtIN;
/*---------------------------------------------------------------------------------*/
/*-----------------------------PUBLIC DATA-----------------------------------------*/
// Podaj liczbe portow fizycznych oraz wirtualnych ktore maja byc poddane procesowi 
// filtracji czasowej.
 
#define	_NVPORT	24		/*liczba obslugiwanych portow/sygnalow*/

/*---------------------------------------------------------------------------------*/
// Podaj przypadajace na kazdy port dwie WARTOSCI OPOZNIENIA dla zmian stanu  
// lo/hi oraz hi/lo (wartosci z zakresu [0..0x3F] co daje maks. czas =[3F*0.05 sek.]/[10/_TKRPR])

#if (_NVPORT!=0)
static const Opoznienie Nastawa[_NVPORT]={	
									5,5,		//port nr 0 czas lo/hi oraz hi\lo
									20,20,		//port nr 1 czas lo/hi oraz hi\lo
									2,4,		//port nr 2 czas lo/hi oraz hi\lo
									2,2,		//port nr 3 czas lo/hi oraz hi\lo
									2,2,		//port nr 4 czas lo/hi oraz hi\lo
									4,8,		//port nr 5 czas lo/hi oraz hi\lo
									10,5,		//port nr 6 czas lo/hi oraz hi\lo
									5,5,		//port nr 7 czas lo/hi oraz hi\lo
									2,2,		//port nr 8 czas lo/hi oraz hi\lo
									20,1,		//port nr 9 czas lo/hi oraz hi\lo
									20,1,		//port nr 10 czas lo/hi oraz hi\lo
									10,5,		//port nr 11 czas lo/hi oraz hi\lo
									20,20,		//port nr 12 czas lo/hi oraz hi\lo
									20,20,		//port nr 13 czas lo/hi oraz hi\lo
									5,5,		//port nr 14 czas lo/hi oraz hi\lo
									5,20,		//port nr 15 czas lo/hi oraz hi\lo
									10,10,		//port nr 16 czas lo/hi oraz hi\lo
									20,20,		//port nr 17 czas lo/hi oraz hi\lo
									5,5,		//port nr 18 czas lo/hi oraz hi\lo
									8,8,		//port nr 19 czas lo/hi oraz hi\lo
									20,20,		//port nr 20 czas lo/hi oraz hi\lo
									20,20,		//port nr 21 czas lo/hi oraz hi\lo
									20,20,		//port nr 22 czas lo/hi oraz hi\lo   
									5,5 		//port nr 23 czas lo/hi oraz hi\lo                                    
								 };
/*---------------------------------------------------------------------------------*/
static volatile IOPort SPort[_NVPORT];			//protected volatile

/*---------------------------------------------------------------------------------*/
//Port fizyczny nalezy przypisac polu SPort[n].Input gdzie 0<=n<_NVPORT.
void inline RefStan(void)
{
	//port 0
	SPort[0].Input=(DtIN._err_IN)? 1 : 0;					//port/sygnal poddany filtracji
	//port 1
	SPort[1].Input=(_PRZ_CO_GetValue())?1:0;						//port/sygnal poddany filtracji
	//port 2
	SPort[2].Input=(_CIG_NC_GetValue()&&_STB_GetValue())?1:0;					//port/sygnal poddany filtracji
	//port 3
	SPort[3].Input=(_CIG_NO_GetValue()&&_STB_GetValue())?1:0;					//port/sygnal poddany filtracji
	//port 4
	SPort[4].Input=(_STB_GetValue())?1:0;							//port/sygnal poddany filtracji
	//port 5
	SPort[5].Input=(_PLOMIEN_GetValue())?0:1;						//port/sygnal poddany filtracji
	//port 6
	SPort[6].Input=(_PRZ_CW_GetValue())?0:1;						//port/sygnal poddany filtracji
	//port 7
	SPort[7].Input=(DtCW._err_CW)? 1 : 0;					//port/sygnal poddany filtracji
	//port 8
	SPort[8].Input=(_MOD_ON_GetValue())?0:1;						//port/sygnal poddany filtracji
	//port 9
	SPort[9].Input=(DtCO._on_DZ_CO)?1:0;				//port/sygnal poddany filtracji
	//port 10
	SPort[10].Input=(DtCW._on_DZ_CW)?1:0;				//port/sygnal poddany filtracji
	//port 11
	SPort[11].Input=((_PRZ_CW_GetValue())?0:1)||((M._LIN_CW)?1:0)||((M._IOT_CW)?1:0);			//port/sygnal poddany filtracji
	//port 12
	SPort[12].Input=((!((M._AKTYWNE_PGD2)?1:0))&&(((_RT_GetValue())?1:0)||((M._LIN_RT)?1:0)||((M._I10V_RT)?1:0)||((M._IOT_RT)?1:0)))||(((M._AKTYWNE_PGD2)?1:0)&&((M._pob_PGDRT)?1:0));                     //port/sygnal poddany filtracji
	//port 13
	SPort[13].Input=(DtCO._cold_CO||DtCO._hot_CO)?1:0;	//port/sygnal poddany filtracji
	//port 14
	SPort[14].Input=(DtCS._error)?1:0;					//port/sygnal poddany filtracji
	//port 15
	SPort[15].Input=(DtCS._work)?1:0;					//port/sygnal poddany filtracji
	//port 16
	SPort[16].Input=(DtCS._lo)?1:0;						//port/sygnal poddany filtracji	
	//port 17
	SPort[17].Input=(DtCW._cold_CW)?1:0;				//port/sygnal poddany filtracji
	//port 18
	SPort[18].Input=(DtCO._err_CO)?1:0;					//port/sygnal poddany filtracji
	//port 19
	SPort[19].Input=(DtCO._ad_CO)?1:0;					//port/sygnal poddany filtracji	
	//port 20
	SPort[20].Input=(DtZW._on_ZW)?1:0;					//port/sygnal poddany filtracji	
	//port 21
	SPort[21].Input=(_CIG_NO_GetValue())?1:0;			//port/sygnal poddany filtracji	
	//port 22
	SPort[22].Input=(_RT_GetValue())?1:0;				//port/sygnal poddany filtracji
	//port 23
	SPort[23].Input=(DtZW._en_ZW)?1:0;					//port/sygnal poddany filtracji    
	//...tak do Prt[NVPORT-1]
}
/*---------------------------------------------------------------------------------*/
// Okreslenie stanu poczatkowego portow (procedura wykonywana jednokrotnie przy starcie
// stabilizacji
void inline FrsStan(void)
{
	//port 0
	SPort[0].Output=SPort[0].Input;	//stan zgodny ze stanem fizycznym	
	SPort[0].Timer=0;
	
	//port 1
	SPort[1].Output=SPort[1].Input;	//stan zgodny ze stanem fizycznym	
	SPort[1].Timer=0;
	
	//port 2
	SPort[2].Output=SPort[2].Input;	//stan zgodny ze stanem fizycznym	
	SPort[2].Timer=0;

	//port 3
	SPort[3].Output=SPort[3].Input;	//stan zgodny ze stanem fizycznym	
	SPort[3].Timer=0;

	//port 4
	SPort[4].Output=SPort[4].Input;	//stan zgodny ze stanem fizycznym	
	SPort[4].Timer=0;

	//port 5
	SPort[5].Output=SPort[5].Input;	//stan zgodny ze stanem fizycznym	
	SPort[5].Timer=0;

	//port 6
	SPort[6].Output=SPort[6].Input;	//stan zgodny ze stanem fizycznym	
	SPort[6].Timer=0;

	//port 7
	SPort[7].Output=SPort[7].Input;	//stan zgodny ze stanem fizycznym	
	SPort[7].Timer=0;

	//port 8
	SPort[8].Output=SPort[8].Input;	//stan zgodny ze stanem fizycznym	
	SPort[8].Timer=0;

	//port 9
	SPort[9].Output=SPort[9].Input;	//stan zgodny ze stanem fizycznym	
	SPort[9].Timer=0;

	//port 10
	SPort[10].Output=SPort[10].Input;	//stan zgodny ze stanem fizycznym	
	SPort[10].Timer=0;

	//port 11
	SPort[11].Output=SPort[11].Input;	//stan zgodny ze stanem fizycznym	
	SPort[11].Timer=0;

	//port 12
	SPort[12].Output=SPort[12].Input;	//stan zgodny ze stanem fizycznym	
	SPort[12].Timer=0;

	//port 13
	SPort[13].Output=SPort[13].Input;	//stan zgodny ze stanem fizycznym	
	SPort[13].Timer=0;

	//port 14
	SPort[14].Output=SPort[14].Input;	//stan zgodny ze stanem fizycznym	
	SPort[14].Timer=0;

	//port 15
	SPort[15].Output=SPort[15].Input;	//stan zgodny ze stanem fizycznym	
	SPort[15].Timer=0;

	//port 16
	SPort[16].Output=SPort[16].Input;	//stan zgodny ze stanem fizycznym	
	SPort[16].Timer=0;

	//port 17
	SPort[17].Output=SPort[17].Input;	//stan zgodny ze stanem fizycznym	
	SPort[17].Timer=0;

	//port 18
	SPort[18].Output=SPort[18].Input;	//stan zgodny ze stanem fizycznym	
	SPort[18].Timer=0;

	//port 19
	SPort[19].Output=SPort[19].Input;	//stan zgodny ze stanem fizycznym	
	SPort[19].Timer=0;

	//port 20
	SPort[20].Output=SPort[20].Input;	//stan zgodny ze stanem fizycznym	
	SPort[20].Timer=0;

	//port 21
	SPort[21].Output=SPort[21].Input;	//stan zgodny ze stanem fizycznym	
	SPort[21].Timer=0;
    
	//port 22
	SPort[22].Output=SPort[22].Input;	//stan zgodny ze stanem fizycznym	
	SPort[22].Timer=0;   
    
	//port 23
	SPort[23].Output=SPort[23].Input;	//stan zgodny ze stanem fizycznym	
	SPort[23].Timer=0;    
	//...tak do Prt[_NVPORT-1]
}
#endif
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
/*-----------------------------PROTECTED DATA--------------------------------------*/
void InitDtaPort(void)
{
#if _NVPORT!=0 
	//RefStan();
	FrsStan();
#endif
}
//----------------------------------------------------------------------------------
#if _NVPORT!=0
void inline StabPort(void)
{
	static unsigned char tim=0;
	unsigned char i,delay;

	//odliczanie czasu 100m sek (funkcja wywolywana w przerwaniu co 10m sek)
	if(tim++<_TKRPR) return;
	tim=0;					
	
	//odczyt stanu portow
	RefStan();
	
	//aby nowy stan zostal zaakceptowany musi trwac min. przez okreslony w polu Delay czas.
	for(i=0;i<_NVPORT;i++)
	{
		if(!SPort[i].Timer)								//trwa odliczanie czasu?
		{
			if(SPort[i].Output==SPort[i].Input)			//stan portu bez zmian?
				 continue;
			else SPort[i].Timer++;						//start odliczania
		}
		else
		{
			if(!SPort[i].Output) 	delay=Nastawa[i].Delay1;
			else 					delay=Nastawa[i].Delay2;
			
			if(SPort[i].Timer>=delay)					//minal nastawiony czas?
			{
				SPort[i].Timer=0;
				SPort[i].Output=SPort[i].Input;			//nowa wartosc stanu wyjsciowego
			}
			else 
			{
				if (SPort[i].Output==SPort[i].Input)	//powrocil do pierwotnego stanu?
					 SPort[i].Timer=0;					//restart odliczania
				else SPort[i].Timer++;					//kontynuacja odliczania
			}
		}	
	}
}
//----------------------------------------------------------------------------------
//Odczyt stanu portu nr Index
inline bool InRdPrt(const unsigned char Index)
{
	return (SPort[Index].Output);
}
//----------------------------------------------------------------------------------
//Odczyt stanu portu nr Index
bit RdPrt(const unsigned char Index)
{
	if (Index>_NVPORT-1) asm("RESET");	//obsluga sytuacji wyjatkowej
	return SPort[Index].Output;
}
//----------------------------------------------------------------------------------
//Ustaw stan portu nr Index
void SetPrt(const unsigned char Index, const unsigned char nw)
{
	if (Index>_NVPORT-1) asm("RESET");	//obsluga sytuacji wyjatkowej
	SPort[Index].Output=((!nw)?0:1);
	SPort[Index].Timer=0;
}
//----------------------------------------------------------------------------------
//Natychmiastowa aktualizacja stanu portu nr Index
void RefPrt(const unsigned char Index)
{
	if (Index>_NVPORT-1) asm("RESET");	//obsluga sytuacji wyjatkowej
	SPort[Index].Output=SPort[Index].Input;
	SPort[Index].Timer=0;
}
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
#endif
