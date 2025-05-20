/*lcdHD1.c*/
/*Sterowanie wyswietlaczem LCD*/
//-------------------------------------------------------------------------------
#include <pic18.h>
#include "global.h"			//parametry globalne
#include "lcdHD1.h"			//parametry globalne

tpWSWLCD LCD;
const unsigned char Znak2[16]={						
								0b00111111,			//0
								0b00000110,			//1
								0b01011011,			//2
								0b01001111,			//3
								0b01100110,			//4
								0b01101101,			//5
								0b01111101,			//6
								0b00000111,			//7
								0b01111111,			//8
								0b01101111,			//9
								0b01110111,			//A
								0b01111100,			//b
								0b00111001,			//C
								0b01011110,			//d
								0b01111001,			//E
								0b01110001,			//F
								};
void ClrWSW(void);
//---------------------
void InitLCD(void)
{
	//LCDCON
	
	LP3=0;				//1:1 presaler dla 1kHz zegara LCD
	LP2=0;				//frame clock prescaler
	LP1=0;
	LP0=0;
/*
	LP3=1;				//1:1 presaler dla 1kHz zegara LCD
	LP2=1;				//frame clock prescaler
	LP1=1;
	LP0=1;
*/
	//Segment enable registers (rejestr TRIS musi byæ na tych wejsciach jako input).
	SE00=1;
	SE01=1;	
	SE02=1;
	SE03=1;
	SE04=1;
	SE05=1;
	SE06=1;	
	SE07=1;
	SE08=1;
	SE09=1;
	SE10=1;
	SE11=1;
	SE12=1;
	SE14=1;
	SE15=1;
	SE19=1;
	SE20=1;
	SE21=1;
	SE22=1;
	SE23=1;
	SE24=1;
	SE25=1;
	SE26=1;
	SE29=1;
	SE30=1;


	//LCDCON
	LMUX1=1;			//COM3:COM0 (nie ma potrzeby zmiany wartosci rejestru TRIS)
	LMUX0=1;
	CS1=0;				//system clock source 8MHz
	CS0=0;
	SLPEN=0;			//LCD aktywny takze w stanie sleep

	//LCDDATA			
	ClrWSW();			//wyzerowanie stanu wyswietlacza

	//LCDREG
	CKSEL1=0;			//LCD regulator disabled (konfiguracja M2 lub M3)
	CKSEL0=0;
	CPEN=1;				//charge pump enable bit

	//LCDPS
	WFT=0;				//waweform TYPEA (nieatywne przerwanie)

	//PIR3
	LCDIF=0;			//clear interrupt flag

	//IPR3
	//LCDIP=1;			//high priority

	//PIE3
	//LCDIE=0;			//enable interrupt

	//LCDCON
	LCDEN=1;			//aktywacja modulu LCD

	//Bias mode tylko dla M0 lub M1
	//LCDPS
	//WFT=0;				//waweform TYPEA
	//BIASMD=0;			//bias mode select bit


	//LCDREG
	BIAS2=1;		//100voltage control output (3,6Vpeak) - regulacja kontrastu
	BIAS1=0;
	BIAS0=0;
	//konfiguracja sprzetowa ukladu kontrastu M2
}
//---------------------
//Wyzerowanie stanu wyswietlacza
void ClrWSW(void)
{
	//com1
	LCDDATA0=0;	
	LCDDATA1=0;
	LCDDATA2=0;
	LCDDATA3=0;
	//com2
	LCDDATA6=0;	
	LCDDATA7=0;
	LCDDATA8=0;
	LCDDATA9=0;
	//com3
	LCDDATA12=0;	
	LCDDATA13=0;
	LCDDATA14=0;
	LCDDATA15=0;
	//com4
	LCDDATA18=0;	
	LCDDATA19=0;
	LCDDATA20=0;
	LCDDATA21=0;
}
//---------------------
//Wyzerowanie stanu wyswietlacza
void UpdateWSW(void)
{
	FormatLcdData();
}
//---------------------
//Konwersja danej data na kod led starszej i mlodszej cyfry
//data - dana do konwersji
//&L1 - zwracany kod led starszej cyfry, &L0 - zwracany kod led mlodszej cyfry
//dhex - typ danej data (0 - kod dec,1 - kod hex)
void DataToK7Led(unsigned char data, unsigned char *L1,unsigned char *L0, unsigned char dhex)
{
	if(dhex)												//konwersja hex na kod led
	{
		*L1=Znak2[(data&0xF0)>>4];
		*L0=Znak2[data&0x0F];
	}else													//konwersja dec na kod led
	{
		if(data>99) data=99;
		*L1=Znak2[data/10];		
		*L0=Znak2[data%10];
	}
}
//---------------------
void FormatLcdData(void)
{
	static unsigned char j;
	
	//konwersja danych na kod lcd
	DataToK7Led(LCD.dDAT0,&LCD.dLED2,&LCD.dLED1,0);
	DataToK7Led(LCD.dDAT1,&LCD.dLED4,&LCD.dLED3,0);

	//pierwsza cyfra
	_LCD_1A=(LCD.dLED1&0b00000001)?1:0;
	_LCD_1B=(LCD.dLED1&0b00000010)?1:0;
	_LCD_1C=(LCD.dLED1&0b00000100)?1:0;
	_LCD_1D=(LCD.dLED1&0b00001000)?1:0;
	_LCD_1E=(LCD.dLED1&0b00010000)?1:0;
	_LCD_1F=(LCD.dLED1&0b00100000)?1:0;
	_LCD_1G=(LCD.dLED1&0b01000000)?1:0;

	//druga cyfra
	_LCD_2A=(LCD.dLED2&0b00000001)?1:0;
	_LCD_2B=(LCD.dLED2&0b00000010)?1:0;
	_LCD_2C=(LCD.dLED2&0b00000100)?1:0;
	_LCD_2D=(LCD.dLED2&0b00001000)?1:0;
	_LCD_2E=(LCD.dLED2&0b00010000)?1:0;
	_LCD_2F=(LCD.dLED2&0b00100000)?1:0;
	_LCD_2G=(LCD.dLED2&0b01000000)?1:0;

	//trzecia cyfra
	_LCD_3A=(LCD.dLED3&0b00000001)?1:0;
	_LCD_3B=(LCD.dLED3&0b00000010)?1:0;
	_LCD_3C=(LCD.dLED3&0b00000100)?1:0;
	_LCD_3D=(LCD.dLED3&0b00001000)?1:0;
	_LCD_3E=(LCD.dLED3&0b00010000)?1:0;
	_LCD_3F=(LCD.dLED3&0b00100000)?1:0;
	_LCD_3G=(LCD.dLED3&0b01000000)?1:0;

	//czwarta cyfra
	_LCD_4A=(LCD.dLED4&0b00000001)?1:0;
	_LCD_4B=(LCD.dLED4&0b00000010)?1:0;
	_LCD_4C=(LCD.dLED4&0b00000100)?1:0;
	_LCD_4D=(LCD.dLED4&0b00001000)?1:0;
	_LCD_4E=(LCD.dLED4&0b00010000)?1:0;
	_LCD_4F=(LCD.dLED4&0b00100000)?1:0;
	_LCD_4G=(LCD.dLED4&0b01000000)?1:0;

	//kreski
	_LCD_T1=1;
	_LCD_T2=1;
	_LCD_T3=1;
	_LCD_T4=1;
	_LCD_T5=1;
	_LCD_T6=1;
	_LCD_T7=1;
	_LCD_T8=1;

/*
	switch(j)
	{
		case 0:
	//pierwsza cyfra
	_LCD_1A=(LCD.dLED1&0b00000001)?1:0;
	_LCD_1B=(LCD.dLED1&0b00000010)?1:0;
	_LCD_1C=(LCD.dLED1&0b00000100)?1:0;
	_LCD_1D=(LCD.dLED1&0b00001000)?1:0;
	_LCD_1E=(LCD.dLED1&0b00010000)?1:0;
	_LCD_1F=(LCD.dLED1&0b00100000)?1:0;
	_LCD_1G=(LCD.dLED1&0b01000000)?1:0;
		break;
		case 1:
	//druga cyfra
	_LCD_2A=(LCD.dLED2&0b00000001)?1:0;
	_LCD_2B=(LCD.dLED2&0b00000010)?1:0;
	_LCD_2C=(LCD.dLED2&0b00000100)?1:0;
	_LCD_2D=(LCD.dLED2&0b00001000)?1:0;
	_LCD_2E=(LCD.dLED2&0b00010000)?1:0;
	_LCD_2F=(LCD.dLED2&0b00100000)?1:0;
	_LCD_2G=(LCD.dLED2&0b01000000)?1:0;
		break;
		case 2:
	//trzecia cyfra
	_LCD_3A=(LCD.dLED3&0b00000001)?1:0;
	_LCD_3B=(LCD.dLED3&0b00000010)?1:0;
	_LCD_3C=(LCD.dLED3&0b00000100)?1:0;
	_LCD_3D=(LCD.dLED3&0b00001000)?1:0;
	_LCD_3E=(LCD.dLED3&0b00010000)?1:0;
	_LCD_3F=(LCD.dLED3&0b00100000)?1:0;
	_LCD_3G=(LCD.dLED3&0b01000000)?1:0;
		break;
		case 3:
	//czwarta cyfra
	_LCD_4A=(LCD.dLED4&0b00000001)?1:0;
	_LCD_4B=(LCD.dLED4&0b00000010)?1:0;
	_LCD_4C=(LCD.dLED4&0b00000100)?1:0;
	_LCD_4D=(LCD.dLED4&0b00001000)?1:0;
	_LCD_4E=(LCD.dLED4&0b00010000)?1:0;
	_LCD_4F=(LCD.dLED4&0b00100000)?1:0;
	_LCD_4G=(LCD.dLED4&0b01000000)?1:0;
		break;
	}
	j++;
	if(j>3) j=0;
*/
/*
	switch(j)
	{
		case 0:
	//pierwszy kanal
	_LCD_1A=(LCD.dLED1&0b00000001)?1:0;
	_LCD_2A=(LCD.dLED2&0b00000001)?1:0;
	_LCD_3A=(LCD.dLED3&0b00000001)?1:0;
	_LCD_4A=(LCD.dLED4&0b00000001)?1:0;
	_LCD_1F=(LCD.dLED1&0b00100000)?1:0;
	_LCD_2F=(LCD.dLED2&0b00100000)?1:0;
	_LCD_3F=(LCD.dLED3&0b00100000)?1:0;
	_LCD_4F=(LCD.dLED4&0b00100000)?1:0;
		break;
		case 1:
	//drugi kanal
	_LCD_1B=(LCD.dLED1&0b00000010)?1:0;
	_LCD_2B=(LCD.dLED2&0b00000010)?1:0;
	_LCD_3B=(LCD.dLED3&0b00000010)?1:0;
	_LCD_4B=(LCD.dLED4&0b00000010)?1:0;
	_LCD_1G=(LCD.dLED1&0b01000000)?1:0;
	_LCD_2G=(LCD.dLED2&0b01000000)?1:0;
	_LCD_3G=(LCD.dLED3&0b01000000)?1:0;
	_LCD_4G=(LCD.dLED4&0b01000000)?1:0;
		break;
		case 2:
	//trzeci kanal
	_LCD_1C=(LCD.dLED1&0b00000100)?1:0;
	_LCD_2C=(LCD.dLED2&0b00000100)?1:0;
	_LCD_3C=(LCD.dLED3&0b00000100)?1:0;
	_LCD_4C=(LCD.dLED4&0b00000100)?1:0;
	_LCD_1E=(LCD.dLED1&0b00010000)?1:0;
	_LCD_2E=(LCD.dLED2&0b00010000)?1:0;
	_LCD_3E=(LCD.dLED3&0b00010000)?1:0;
	_LCD_4E=(LCD.dLED4&0b00010000)?1:0;
		break;
		case 3:
	//czwarty kanal
	_LCD_1D=(LCD.dLED1&0b00001000)?1:0;
	_LCD_2D=(LCD.dLED2&0b00001000)?1:0;
	_LCD_3D=(LCD.dLED3&0b00001000)?1:0;
	_LCD_4D=(LCD.dLED4&0b00001000)?1:0;
		break;
	}
	j++;
	if(j>3) j=0;
*/
}
