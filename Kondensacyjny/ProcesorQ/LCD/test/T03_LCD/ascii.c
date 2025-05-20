/*pwron.c*/
#include <pic18.h>
#include <ctype.h>
#include "global.h"			//parametry globalne
//Sprawdzenie czy kod jest kodem ASCII z zaresu 0x31 do 0x39 oraz 0x40 do 0x46
unsigned char CheckASCII(unsigned char a)
{
	if(!isxdigit(a)||islower(a)) return 0;
	return 1;
}
//Kontrola kodu ASCII dla 4 bajtow
unsigned char CheckFourASCII(unsigned char b3, unsigned char b2, unsigned char b1, unsigned char b0)
{
	return CheckASCII(b3)&&CheckASCII(b2)&&CheckASCII(b1)&&CheckASCII(b0);
}
//Przeliczanie wartosci 2 bajtowej ASCII na HEX(8bit)
unsigned char TWO_ASCIItoHEX(unsigned char a1, unsigned char a0)
{
	unsigned char bc=0;
	if(a0<=0x39) bc=a0-0x30;
	else bc=a0-0x37;
	if(a1<=0x39) return (bc|((a1-0x30)<<4);
	return (bc|((a1-0x37)<<4);
}
//Przeliczanie wartosci 4 bajtowej ASCII na HEX(16bit)
unsigned int FOUR_ASCIItoHEX(unsigned char b3, unsigned char b2, unsigned char b1, unsigned char b0)
{
	unsigned int bcc=0;
	bcc=(TWO_ASCIItoHEX(b3,b2))<<8;
	return bcc|TWO_ASCIItoHEX(b1,b0);
}
//Przeliczanie wartosci HEX(8bit) na 2 bajty ASCII
void HEXtoTWO_ASCII(unsigned char h,unsigned char *a1, unsigned char *a0)
{
	unsigned char bc;

	bc=h&0x0f;
	if(bc<=9) *a0=bc+0x30;
	else *a0=bc+0x37;
	bc=(h&0xf0)>>4;	
	if(bc<=9) *a1=bc+0x30;
	else *a1=bc+0x37;	
}
//Przeliczanie wartosci HEX(16bit) na 4 bajty ASCII
void HEXtoFOUR_ASCII(unsigned int h,unsigned char *a3, unsigned char *a2,unsigned char *a1, unsigned char *a0)
{
	unsigned char bc;
	bc=h&0x00ff;
	HEXtoTWO_ASCII(bc,a1,a0);
	bc=(h&0xff00)>>8;
	HEXtoTWO_ASCII(bc,a3,a2);
}