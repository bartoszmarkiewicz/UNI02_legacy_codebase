/*history.tp.h*/
//typy danych dla plinu history.c
//----------------------------------------------------------
#ifndef	_HISTORY_TPH
#define	_HISTORY_TPH
//----------------------------------------------------------
typedef union   {
                                unsigned char war;
                                /*struct //__PACKED
                                {
                                    unsigned int b0:1;
                                    unsigned int b1:1;
                                    unsigned int b2:1;
                                    unsigned int b3:1;
                                    unsigned int b4:1;
                                    unsigned int b5:1;
                                    unsigned int b6:1;
                                    unsigned int b7:1;
                                 };*/
                                struct //__PACKED
                                {
                                    unsigned wST:6;
                                    unsigned rST:2;
                                }EST;                                 
}HIST_POZ;

/*typedef struct	{
                    BTY_VAL1 HPoz;
				}HIST_POZ;*/
//----------------------------------------------------------
#endif	/*_HISTORY_TPH*/
