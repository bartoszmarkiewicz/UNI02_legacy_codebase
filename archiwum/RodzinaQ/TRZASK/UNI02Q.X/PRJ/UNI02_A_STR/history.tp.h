/*history.tp.h*/
//typy danych dla plinu history.c
//----------------------------------------------------------
#ifndef	_HISTORY_TPH
#define	_HISTORY_TPH
//----------------------------------------------------------
typedef union   {
                        unsigned char war;
                        struct
                        {
                            unsigned wST:6;
                            unsigned rST:2;
                        }EST;                                 
}HIST_POZ;
//----------------------------------------------------------
#endif	/*_HISTORY_TPH*/
