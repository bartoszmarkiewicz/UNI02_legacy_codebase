/*port.tp.h*/
//typy danych dla pliku port.c
//----------------------------------------------------------
#ifndef	_PORT_TPH
#define	_PORT_TPH
//----------------------------------------------------------
//Rozny cas opoznienia dla zmian 0/1 i 1/0
typedef struct	sOpoznienie{
				unsigned char Delay1;	//opoznienie dla zmiany lo/hi		
				unsigned char Delay2;	//opoznienie dla zmiany hi\lo
				}Opoznienie;
typedef struct	sIOPort{
    union{
        struct{
            unsigned Input:1;		//wejscie sygnalu
            unsigned Output:1;		//wyjscie sygnalu
            unsigned Timer:6;		//czas 0...0x3F
        };
    };
}IOPort;
//----------------------------------------------------------
#endif	/*_PORT_TPH*/
