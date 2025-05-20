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
	PIE1bits.ADIE=0;		//disable AC interrupt
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
    ADCON2bits.ACQT0 = 0; 
    ADCON2bits.ACQT1 = 0;
    ADCON2bits.ACQT2 = 0;
//    ADCON2bits.ACQT0 = 1; 
//    ADCON2bits.ACQT1 = 1;
//    ADCON2bits.ACQT2 = 1;    
    
    ADCON2bits.ADCS0 = 1; //fAD = FOSC/16
    ADCON2bits.ADCS1 = 0;
    ADCON2bits.ADCS2 = 1;  
    
//    ADCON2bits.ADCS0 = 1; //fAD = FOSC/64
//    ADCON2bits.ADCS1 = 1;
//    ADCON2bits.ADCS2 = 0;    
//-----------
	ADCON1bits.VCFG0=0;	//Vref+=VDD
	ADCON1bits.VCFG1=0;	//Vref-=VSS	
//-----------
//   	PCFG0=0;	//AN0 jako wejscie analogowe
//	PCFG1=1;
//	PCFG2=1;
//	PCFG3=1; 
    
    
    //RA0, RA2, RA3
	ADCON1bits.PCFG0=0;	//AN0, AN1, AN2 jako wejscie analogowe
	ADCON1bits.PCFG1=0;
	ADCON1bits.PCFG2=1;
	ADCON1bits.PCFG3=1;
//-----------
	ADCON2bits.ADFM=1;		//(16bit) - wyrownanie do prawej!
	ADCON0bits.ADON=1;		//przylacz modul AC
	ADCON0bits.CHS0=0;		//CHANEL=0
	ADCON0bits.CHS1=0;
	ADCON0bits.CHS2=0;
	ADCON0bits.CHS3=0;
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
    volatile unsigned char ch;
    ch=(unsigned char) chanel;
#if _ENACQ==0
	DelayAD();						//opoznienie 50 u sek.
#endif
    ADCON0bits.CHS0=(ch&0b00000001);		//wybor kanalu pomiarowego
    ADCON0bits.CHS1=((ch&0b00000010)>>1);
	ADCON0bits.CHS2=((ch&0b00000100)>>2);
	ADCON0bits.CHS3=((ch&0b00001000)>>3);
	GODONE=1;						//start dla pomiaru
	while(GODONE)continue;
	return ADRES;                   //zwrot wartosci AC (16bit) - wyrownanie do prawej!
}

#endif //_EXT_BOARD!=0
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
