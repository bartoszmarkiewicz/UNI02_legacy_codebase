/*pompa.tp.h*/
//typy danych dla pliku pompa.c
//----------------------------------------------------------
#ifndef	_POMPA_TPH
#define	_POMPA_TPH
//----------------------------------------------------------
typedef struct	{
				unsigned char MGPROC;	//gorna granica predkosci w %
				unsigned char MDPROC;	//dolna granica predkosci w %
				unsigned char APROC;	//aktualna predkosc pompy w %

				unsigned char MGPROC2;
				}DtPMP;		
//----------------------------------------------------------
#endif	/*_POMPA_TPH*/