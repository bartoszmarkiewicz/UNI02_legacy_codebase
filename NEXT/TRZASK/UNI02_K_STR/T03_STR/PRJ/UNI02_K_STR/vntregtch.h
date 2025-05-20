/*vntregtch.h*/
#ifndef	_VNTTCH_H
#define	_VNTTCH_H
//----------------------------------------------------------
#include "vntregtch.tp.h"		//typy danych
#include "global.h"
//----------------------------------------------------------
void OdczytDanychKalibracji();
void OdczytDanychKalibracji2();

#if _HIRESVNT==0
unsigned char DVNTtoPWM(unsigned char dvnt);
#else
unsigned int DVNTtoPWM(unsigned int dvnt);
#endif

unsigned char ModToDVNT(unsigned char dvnt,unsigned char minvnt,unsigned char maxvnt,unsigned char pw);
//----------------------------------------------------------
#endif		/*_VNTTCH_H*/