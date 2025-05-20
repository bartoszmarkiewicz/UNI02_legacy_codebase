/*keypad.tp.h*/
//typy danych dla pliku keypad.c
//----------------------------------------------------------
#ifndef	_KEYPAD_TPH
#define	_KEYPAD_TPH
//----------------------------------------------------------
/*typedef struct sStPin {
				unsigned bk:1;			
				unsigned sbk:1;	
				unsigned ap:1;
				unsigned dk:1;
				unsigned fl:1;
				unsigned c:1;
				unsigned t:1;
                //unsigned int flags0;                 
                
				unsigned char cS;
				unsigned char dS;
				unsigned char krk;
				unsigned char val;               
				}StPin;*/

typedef struct sStPin {
    union{
        struct{
            unsigned bk:1;			
            unsigned sbk:1;	
            unsigned ap:1;
            unsigned dk:1;
            unsigned fl:1;
            unsigned c:1;
            unsigned t:1;                           
          };
    };
    unsigned char cS;
    unsigned char dS;
    unsigned char krk;
    unsigned char val;               
}StPin;                
//----------------------------------------------------------
#endif /*_KEYPAD_TPH*/
