/*rtimer.tp.h*/
//typy danych dla pliku rtimer.c
//----------------------------------------------------------
#ifndef	_RTIMER_TPH
#define	_RTIMER_TPH
//----------------------------------------------------------
	typedef struct	{
				unsigned char M;
				unsigned char G;
			}RTgodziny;
	typedef struct	{
				unsigned char S;
				unsigned char M;
			}RTminuty;
	typedef struct	{
				unsigned char dS;
				unsigned char S;
			}RTsekundy;
	typedef struct	{
				unsigned char cS;
				unsigned char dS;
			}RTdSekundy;

	typedef unsigned char RTcSekundy;

	typedef struct	{
				unsigned char cS;
				unsigned char dS;
				unsigned char S;
			}RTcomm;
//----------------------------------------------------------
#endif /*_RTIMER_TPH*/
