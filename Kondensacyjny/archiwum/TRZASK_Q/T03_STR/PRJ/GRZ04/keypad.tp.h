/*keypad.tp.h*/
//typy danych dla pliku keypad.c
//----------------------------------------------------------
#ifndef	_KEYPAD_TPH
#define	_KEYPAD_TPH
//----------------------------------------------------------
typedef struct	{
    union   {
            unsigned char flag0;
            struct
            {    
				unsigned bk:1;			
				unsigned sbk:1;	
				unsigned ap:1;
				unsigned dk:1;
				unsigned fl:1;
                unsigned _fl0bt5:1;			//
                unsigned _fl0bt6:1;			//
                unsigned _fl0bt7:1;			//
            };
    };                    
				}StPin;
//----------------------------------------------------------
#endif /*_KEYPAD_TPH*/
