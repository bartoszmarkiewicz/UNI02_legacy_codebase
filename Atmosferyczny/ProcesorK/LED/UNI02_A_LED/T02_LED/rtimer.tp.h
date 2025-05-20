/*rtimer.tp.h*/
//typy danych dla pliku rtimer.c
//----------------------------------------------------------
#ifndef	_RTIMER_TPH
#define	_RTIMER_TPH
//----------------------------------------------------------
	typedef struct	sRTgodziny{
				unsigned char M;
				unsigned char G;
			}RTgodziny;
	typedef struct	sRTminuty{
				unsigned char S;
				unsigned char M;
			}RTminuty;
	typedef struct	sRTsekundy{
				unsigned char dS;
				unsigned char S;
			}RTsekundy;
	typedef struct	sRTdSekundy{
				unsigned char cS;
				unsigned char dS;
			}RTdSekundy;

	typedef unsigned char RTcSekundy;

	typedef struct	sRTcomm{
				unsigned char cS;
				unsigned char dS;
				unsigned char S;
			}RTcomm;
//----------------------------------------------------------
#endif /*_RTIMER_TPH*/
