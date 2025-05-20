/*port.c*/
//Konfiguracja portow fizycznych mikrokontrolera
//Stabilizacja stanow wejsciowych
/*---------------------------------------------------------------------------------*/
#include <pic18.h>
#include "global.h"			//parametry globalne
#include "port.h"			//parametry lokalne
#include "pomiar.h"		//typy danych pomiar.c
//pomiar.c
extern DataCO DtCO;
extern DataCW DtCW;
extern DataCSN DtCS;
extern DataZW DtZW;
/*---------------------------------------------------------------------------------*/
/*-----------------------------PUBLIC DATA-----------------------------------------*/
// Podaj liczbe portow fizycznych oraz wirtualnych ktore maja byc poddane procesowi 
// filtracji czasowej.
 
#define	_NVPORT	10		/*liczba obslugiwanych portow/sygnalow*/

/*---------------------------------------------------------------------------------*/
// Podaj przypadajace na kazdy port dwie WARTOSCI OPOZNIENIA dla zmian stanu  
// lo/hi oraz hi/lo (wartosci z zakresu [0..0x3F] co daje maks. czas =[3F*0.05 sek.]/[10/_TKRPR])

#if (_NVPORT!=0)
static const Opoznienie Nastawa[_NVPORT]={	
									1,1,		//port nr 0 czas lo/hi oraz hi\lo
									1,1,		//port nr 1 czas lo/hi oraz hi\lo
									2,4,		//2,1port nr 2 czas lo/hi oraz hi\lo
									2,2,		//2,1port nr 3 czas lo/hi oraz hi\lo
									2,2,		//port nr 4 czas lo/hi oraz hi\lo
									4,8,		//port nr 5 czas lo/hi oraz hi\lo
									2,2,		//port nr 6 czas lo/hi oraz hi\lo
									5,5,		//port nr 7 czas lo/hi oraz hi\lo
									2,2,		//port nr 8 czas lo/hi oraz hi\lo
									2,10		//port nr 9 czas lo/hi oraz hi\lo
								 };
/*---------------------------------------------------------------------------------*/
static volatile IOPort SPort[_NVPORT];			//protected volatile

/*---------------------------------------------------------------------------------*/
//Port fizyczny nalezy przypisac polu SPort[n].Input gdzie 0<=n<_NVPORT.

void RefStan(void)
{
	//port 0
	SPort[0].Input=(_K2) ? 0 : 1;							//_KEY port/sygnal poddany filtracji
	//port 1
	SPort[1].Input=(_K1) ? 0 : 1;							//port/sygnal poddany filtracji
	//port 2
	SPort[2].Input=(_CIG_NC&&_STB) ? 1 : 0;						//port/sygnal poddany filtracji
	//port 3
	SPort[3].Input=(_CIG_NO&&_STB) ? 1 : 0;						//port/sygnal poddany filtracji
	//port 4
	SPort[4].Input=(_STB) ? 1 : 0;								//port/sygnal poddany filtracji
	//port 5
	SPort[5].Input=(_PLOMIEN) ? 0 : 1;							//port/sygnal poddany filtracji
	//port 6
	SPort[6].Input=(_PRZ_CW) ? 0 : 1;							//port/sygnal poddany filtracji
	//port 7
	SPort[7].Input=(DtCW._err_CW)? 1 : 0;					//port/sygnal poddany filtracji
	//port 8
	SPort[8].Input=(_MOD_ON) ? 0 : 1;							//port/sygnal poddany filtracji	
	//port 9
	SPort[9].Input=(_CIG_NO) ? 1 : 0;								//port/sygnal poddany filtracji
	//...tak do Prt[NVPORT-1]
}
/*---------------------------------------------------------------------------------*/
// Okreslenie stanu poczatkowego portow (procedura wykonywana jednokrotnie przy starcie
// stabilizacji
void FrsStan(void)
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
	//...tak do Prt[_NVPORT-1]
}
#endif
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
/*-----------------------------PROTECTED DATA--------------------------------------*/
void InitPort(void)
{
#if _PIC_PORT==0
#error _PIC_PORT - niewlasciwa wartosc parametru
#endif
#if _PIC_PORT==3
	PORTA=_VALUEA;
	TRISA=_DIRECTA;
	PORTB=_VALUEB;
	TRISB=_DIRECTB;
	PORTC=_VALUEC;
	TRISC=_DIRECTC;
#endif
#if _PIC_PORT==5
	PORTA=_VALUEA;
	TRISA=_DIRECTA;
	PORTB=_VALUEB;
	TRISB=_DIRECTB;
	PORTC=_VALUEC;
	TRISC=_DIRECTC;	
	PORTD=_VALUED;
	TRISD=_DIRECTD;
	PORTE=_VALUEE;
	TRISE=(TRISE&0b11111000)|(_DIRECTE&0b00000111);
#endif
#if _NVPORT!=0 
	RefStan();
	FrsStan();
#endif
}
//----------------------------------------------------------------------------------
#if _NVPORT!=0
void StabPort(void)
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
