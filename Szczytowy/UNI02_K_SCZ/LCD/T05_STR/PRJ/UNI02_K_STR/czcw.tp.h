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
                unsigned S_CO:1;            //pobudzenie CW z turbinki
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
        
        unsigned int CzCotim;
        unsigned int impCO;		//liczba impulsow/sek
        float przCO;            //wartosc przeplywu (float)
        unsigned int przCCO;    //wartosc przeplywu*100 (int)
        }DataPrCO;
					
//----------------------------------------------------------
#endif	/*_CZCW_TPH*/
