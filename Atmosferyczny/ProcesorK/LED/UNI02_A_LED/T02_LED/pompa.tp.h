/*pompa.tp.h*/
//typy danych dla pliku pompa.c
//----------------------------------------------------------
#ifndef	_POMPA_TPH
#define	_POMPA_TPH
//----------------------------------------------------------
typedef struct	sDtPMP{
				    volatile unsigned char MGPROC;	//gorna granica predkosci w %
				    volatile unsigned char MDPROC;	//dolna granica predkosci w %
				    volatile unsigned char APROC;	//aktualna predkosc pompy w %
				}DtPMP;		
//----------------------------------------------------------
#endif	/*_POMPA_TPH*/