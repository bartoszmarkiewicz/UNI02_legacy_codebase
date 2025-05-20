/*ac.h*/
#ifndef	_CHKCITT_H
#define	_CHKCITT_H
//----------------------------------------------------------
//Obliczenie CHK-CITT

#define _POLY		0x1021		//slowo polynom
#define _INIT_CHK	0Xffff		//wartosc poczatkowa


unsigned char CheckFourASCII(unsigned char b3, unsigned char b2, unsigned char b1, unsigned char b0);
void TWO_ASCIItoHEX(unsigned char a1, unsigned char a0, unsigned char *h);
unsigned int FOUR_ASCIItoHEX(unsigned char b3, unsigned char b2, unsigned char b1, unsigned char b0);
//----------------------------------------------------------
#endif		/*_CHKCITT_H*/