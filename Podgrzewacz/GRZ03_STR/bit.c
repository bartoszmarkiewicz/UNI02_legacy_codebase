/*bit.c*/
/*Procedury zwiazane operacjami na bitach*/

#include <pic18.h>
#include "global.h"			//parametry globalne
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//odczyt stanu wybranego bitu dowolnej danej typu char
bit GetBit(const unsigned char data,const unsigned char nbit)
{
	if(data&(1<<nbit)) return 1;
	return 0;
}
//ustawia/kasuje wybrany bit dowolnej danej typu char (zapalenie/wygaszenie diody)
void BitSet(unsigned char *data, const unsigned char nbit,const unsigned char bt)
{
	if(bt) *data|=1<<nbit;
	else *data&=~(1<<nbit);
}
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
