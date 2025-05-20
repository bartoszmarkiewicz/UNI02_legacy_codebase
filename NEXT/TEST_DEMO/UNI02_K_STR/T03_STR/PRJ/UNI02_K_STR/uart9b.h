/*uart9b.h*/
//----------------------------------------------------------
#ifndef	_UART9b_H
#define	_UART9b_H

#include "uart9b.tp.h"		//typy danych
/*---------------------------------------------------------------------------------*/
#define _NRS_RBF 80			//rozmiar bufora odebranych danych
#define _NRS_TBF 80			//rozmiar bufora wysylanych danych
/*---------------------------------------------------------------------------------*/
/*-----------------------------PUBLIC DATA-----------------------------------------*/
#define _BRGH 0				//1(w trybie synch. ignorowany) 0 - low speed, 1- high speed 				
#define _SYNC 0				//0 - asynch. 1 - synch.
#define _DSBD 2400			//19200predkosc transmisji

#define _RCHK_ON	0		//aktywna suma kontrolna dla odczytu
#define _RACK_ON	1		//aktywna procedura potwierdzenia odbioru

static inline unsigned char iVerifyNDatCHK(const unsigned char Data);
//----------------------------------------------------------
#endif		/*_UART9b_H*/
