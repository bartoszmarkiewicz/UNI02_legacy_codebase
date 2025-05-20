/*history.h*/
//----------------------------------------------------------
#ifndef	_HISTORY_H
#define	_HISTORY_H
#include "history.tp.h"

#define _HISTSZ 16
#define _ESTBL 4        //liczba zapamietywanych statusow wylaczenia z blokada awaryjna
#define _ESTPZ 22       //liczba pozycji statusu wylaczenia awaryjnego z blokada

void ReadHBuf(void);
void WriteHTim(void);
void WriteHBuf(void);
extern void if_eeprom_write(const unsigned int badd,const unsigned char bdat);
extern void WriteEESTAT(const unsigned char nr);
extern void CopyEESTAT(const unsigned char nr0,const unsigned char nr1);
//----------------------------------------------------------
#endif		/*_HISTORY_H*/

					
