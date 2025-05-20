/*chkcitt.c*/
//Obliczenie CHK-CITT
#include <pic18.h>
#include "global.h"			//parametry globalne
#include "chkcitt.h"			//parametry lokalne

unsigned int CRC;
//-----------------------------
//Aktualizacja CRC dla pojedynczego bajtu
void UpdateCRC(unsigned char znak)
{
	unsigned char vbit=0x80;
	unsigned char xor_fl;

	while(vbit)
	{
		xor_fl=0;
		if(CRC&0x8000) xor_fl=1;
		CRC=CRC<<1;
		if(znak&vbit) CRC++;
		if(xor_fl) CRC=CRC^_POLY;
		vbit=vbit>>1;	
	}
}
//-----------------------------
//Ostatni krok aktualizacji CRC
void FinalUpdateCRC(void)
{
	unsigned char i,xor_fl;

	for(i=0;i<16;i++)
	{
		xor_fl=0;
		if(CRC&0x8000) xor_fl=1;
		CRC=CRC<<1;
		if(xor_fl) CRC=CRC^_POLY;
	}
}

//Obliczanie sumy kontrolnej z n danych z tablicy tb
unsigned int LiczCRC(unsigned char *tb, unsigned char n)
{
	unsigned char i;

	CRC=_INIT_CHK;
	for(i=0;i<n;i++) UpdateCRC(*(tb+i));
	FinalUpdateCRC();
	return CRC;
}



//-----------------------------
//Sprawdzenie zgodnosci CHK miedzy 4 bajtowa wartoscia ASCII a 2 bajtowa wartoscia HEX
unsigned char CheckCHK(unsigned char aaa3, unsigned char aaa2, unsigned char aaa1, unsigned char aaa0, unsigned int h)
{
	if(!CheckFourASCII(aaa3,aaa2,aaa1,aaa0)) return 0;	//sprawdzenie czy wszystkie bajty sa wartosciami ASCII z zakr:30-39, 40-46
	return (h==FOUR_ASCIItoHEX(aaa3,aaa2,aaa1,aaa0));
}
//-----------------------------
	