/*ac.c*/
//Funkcje obslugi przetwornika AC
// (!) Bazuje na funkcjach z pliku: eeprom.c
//wersja takze dla procesorow serii K
/*---------------------------------------------------------------------------------*/
#include <xc.h>
#include <stdbool.h>
#include <stdint.h>

#include "../../proj.tp.h"

#ifndef _TYP_PRJ
#error NIEOKRESLONY IDENTYFIKATOR PROJEKTU
#endif

#if _TYP_PRJ==2
#advisory _TYP_PRJ==2 
#include "../../mcc_generated_files\mcc.h"

#include "global.h"
#include "global.ex.h"

#include "ac.h"				//parametry lokalne

/*---------------------------------------------------------------------------------*/
/*
#if defined(_18F46K20)||defined(_18F47Q10)
	void InitAC(void)
	{
		ADIE=0;		//disable AC interrupt
	//-----------
	#if _ENACO!=0	
		ACQT0=1;	//Acquisition time
		ACQT1=1;	//20TAD
		ACQT2=1;
	#else
		ACQT0=0;	//Manual acquisition time
		ACQT1=0;	
		ACQT2=0;
	#endif
	//-----------
		ADCS0=0;	//fAD = FOSC/32
		ADCS1=1;
		ADCS2=0;
	//-----------
		VCFG0=0;	//Vref+=VDD
		VCFG1=0;	//Vref-=VSS	
	//-----------
		//PCFG0=1;	//AN0 do AN4 jako wyjscie analogowe
		//PCFG1=1;
		//PCFG2=0;
		//PCFG3=1;
		ANSEL=0b00001111;
		ANSELH=0b00000000;
	//-----------
		ADFM=0;		//bardziej znaczaca czesc wyniku w ADRESH
		ADON=1;		//przylacz modul AC
		CHS0=0;		//CHANEL=0
		CHS1=0;
		CHS2=0;
		CHS3=0;
	}
#else
	void InitAC(void)
	{
		ADIE=0;		//disable AC interrupt
	//-----------
	#if _ENACO!=0	
		ACQT0=1;	//Acquisition time
		ACQT1=1;	//20TAD
		ACQT2=1;
	#else
		ACQT0=0;	//Manual acquisition time
		ACQT1=0;	
		ACQT2=0;
	#endif
	//-----------
		ADCS0=0;	//fAD = FOSC/32
		ADCS1=1;
		ADCS2=0;
	//-----------
		VCFG0=0;	//Vref+=VDD
		VCFG1=0;	//Vref-=VSS	
	//-----------
		PCFG0=1;	//AN0 do AN4 jako wyjscie analogowe
		PCFG1=1;
		PCFG2=0;
		PCFG3=1;
	//-----------
		ADFM=0;		//bardziej znaczaca czesc wyniku w ADRESH
		ADON=1;		//przylacz modul AC
		CHS0=0;		//CHANEL=0
		CHS1=0;
		CHS2=0;
		CHS3=0;
	}
#endif
 */
/*---------------------------------------------------------------------------------*/
/*#if _ENACQ==0
//Opoznienie 50u sek.
void DelayAD(void)	//54u sek.
{
	unsigned char i;
	for(i=0;i<_DLAC;i++)
	{
		asm("nop");
	}
}
#endif
*/
/*---------------------------------------------------------------------------------*/
//Pomiar na wybranym wyjsciu
unsigned char ReadAC(adcc_channel_t chanel)
{
    volatile unsigned int acdt;
    
    acdt=ADCC_GetSingleConversion(chanel);
    ADCC_GetSingleConversion(channel_VSS);
    return (unsigned char)(acdt>>8);
}

unsigned int ReadAC16(adcc_channel_t chanel)
{
    volatile unsigned int acdt;
    
    acdt=ADCC_GetSingleConversion(chanel);
    ADCC_GetSingleConversion(channel_VSS);
    return acdt;
}
/*unsigned char ReadAC(unsigned char chanel)
{
#if _ENACQ==0
	DelayAD();						//opoznienie 50 u sek.
#endif
	CHS0=(chanel&0b00000001);		//wybor kanalu pomiarowego
	CHS1=((chanel&0b00000010)>>1);
	CHS2=((chanel&0b00000100)>>2);
	CHS3=((chanel&0b00001000)>>3);
	GODONE=1;						//start dla pomiaru
	while(GODONE)continue;
	return ADRESH;					//zwrot wartosci AC
}*/
/*---------------------------------------------------------------------------------*/
//Przetworzenie wyniku pomiaru na wartosc z tab. NTC czujnika typu termet.
unsigned char ReadTabNTC(unsigned char zrAC)
{
	unsigned int i;
	unsigned char data;
	i=zrAC;
	//obliczenie indeksu tablicy
	if(i<_NTC_0) i=_NTC_0;
	if(i>_NTC_99) i=_NTC_99;
	i-=_NTC_0;
	if(i>_NDTTCO) i=_NDTTCO;
	i+=_AEETCO;
	//odczyt danej z tablicy
	data=ReadEEPROM(i);
	return data;
}
/*---------------------------------------------------------------------------------*/
//Zwrot wartosci AC adresujacej podana wartosc z tabeli NTC czujnika typu termet.
unsigned char ReturnACfromNTC(unsigned char data)
{
	unsigned int i;
	unsigned char bdta;
	for(i=_AEETCO;i<_AEETCO+_NDTTCO;i++)
	{
		bdta=ReadEEPROM(i);
		if(bdta<data)	return ((i-_AEETCO)+_NTC_0)-1;
	}
	return (i-_AEETCO)+_NTC_0;
}
/*---------------------------------------------------------------------------------*/
//Przetworzenie wyniku pomiaru na wartosc z tab. NTC czujnika zewnetrznego.
signed char ReadTabZNTC(unsigned char zrAC)
{
	unsigned int i;
	signed char data;
	i=zrAC;
	//obliczenie indeksu tablicy
	if(i<_ZNTC_0) i=_ZNTC_0;
	if(i>_ZNTC_99) i=_ZNTC_99;
	i-=_ZNTC_0;
	if(i>_NDTTZW) i=_NDTTZW;
	i+=_AEETZW;
	//odczyt danej z tablicy
	data=ReadEEPROM(i);
	return data;
}
/*---------------------------------------------------------------------------------*/
#endif
