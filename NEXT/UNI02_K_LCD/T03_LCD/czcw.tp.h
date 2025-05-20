/*czcw.tp.h*/
//typy danych dla plinu czcw.c
//----------------------------------------------------------
#ifndef	_CZCW_TPH
#define	_CZCW_TPH
//----------------------------------------------------------
typedef struct	{
				unsigned _ltrUP:1;			//wzrost przeplywu
				unsigned _ltrDW:1;			//spadek przeplywu
				unsigned S_CW:1;			//pobudzenie CW
				unsigned char przCW;		//liczba impulsow/sek
				}DataPrCW;
					
//----------------------------------------------------------
#endif	/*_CZCW_TPH*/
