/*ac.c*/
//Funkcje obslugi przetwornika AC
// (!) Bazuje na funkcjach z pliku: eeprom.c
/*---------------------------------------------------------------------------------*/
//#include <pic18.h>
//#include <htc.h>
#include <xc.h>
#include "global.h"			//parametry globalne
#include "ac.h"				//parametry lokalne

#if _EXT_BOARD!=0

/*---------------------------------------------------------------------------------*/
void InitAC(void)
{
	ADIE=0;		//disable AC interrupt
//-----------
#if _ENACQ!=0	
	/*ACQT0=1;	//Acquisition time
	ACQT1=1;	//20TAD
	ACQT2=1;
    */   
#else
	ACQT0=0;	//Manual acquisition time
	ACQT1=0;	
	ACQT2=0;
#endif
    /*ACQT0 = 1;  //1 TAD
    ACQT1 = 0; 
    ACQT2 = 0;*/    
//-----------
	/*ADCS0=0;	//fAD = FOSC/32
	ADCS1=1;
	ADCS2=0;*/
    ADCS0 = 1; //fAD = FOSC/16
    ADCS1 = 0;
    ADCS2 = 1;    
//-----------
	VCFG0=0;	//Vref+=VDD
	VCFG1=0;	//Vref-=VSS	
//-----------
	PCFG0=0;	//AN0 jako wejscie analogowe
	PCFG1=1;
	PCFG2=1;
	PCFG3=1;
//-----------
	ADFM=1;		//(16bit) - wyrownanie do prawej!
	ADON=1;		//przylacz modul AC
	CHS0=0;		//CHANEL=0
	CHS1=0;
	CHS2=0;
	CHS3=0;
}
/*---------------------------------------------------------------------------------*/
#if _ENACQ==0
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
/*---------------------------------------------------------------------------------*/
//Pomiar na wybranym wyjsciu
unsigned int ReadAC(const unsigned char chanel)
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
	return ADRES;                   //zwrot wartosci AC (16bit) - wyrownanie do prawej!
}

#endif //_EXT_BOARD!=0
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
