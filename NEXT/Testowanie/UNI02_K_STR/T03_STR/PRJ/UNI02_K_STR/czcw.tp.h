/*czcw.tp.h*/
//typy danych dla plinu czcw.c
//----------------------------------------------------------
#ifndef	_CZCW_TPH
#define	_CZCW_TPH
//----------------------------------------------------------
typedef struct	{
        union   {
            unsigned char flag0;
            struct
            {   
                unsigned _ltrUP:1;          //wzrost przeplywu                 
                unsigned _ltrDW:1;          //spadek przeplywu
                unsigned S_CW:1;            //pobudzenie CW z turbinki
				unsigned fl0bt3:1;          //            
                unsigned fl0bt4:1;          //
                unsigned fl0bt5:1;          //               
				unsigned fl0bt6:1;          //
				unsigned fl0bt7:1;          //           
            };
        };     
        
        unsigned int impONE;
        unsigned int impON;
        unsigned int impOFF;
        
        unsigned int CzCwtim;
        unsigned int impCW;		//liczba impulsow/sek
        float przCW;            //wartosc przeplywu (float)
        unsigned int przCCW;    //wartosc przeplywu*100 (int)
        }DataPrCW;
					
//----------------------------------------------------------
#endif	/*_CZCW_TPH*/
