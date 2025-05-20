/*kolejka.h*/
#ifndef	_KOLEJKA_H
#define	_KOLEJKA_H
//----------------------------------------------------------
//rtimer.c
extern void StartRTdS(const unsigned char Index);
extern unsigned char RTdS(const unsigned char Index);
//mgrs_rs9b.c
#if (_EXT_BOARD>=2)                     //aktywowany modul LIN1SLAVE?
//amgrs_rs9b.c
extern unsigned char TrToUNI01(unsigned char tDTA);
#else
//mgrs_rs9b.c
extern void TrToUNI01(unsigned char tDTA);
#endif

//----------------------------------------------------------
#endif		/*_KOLEJKA_H*/
