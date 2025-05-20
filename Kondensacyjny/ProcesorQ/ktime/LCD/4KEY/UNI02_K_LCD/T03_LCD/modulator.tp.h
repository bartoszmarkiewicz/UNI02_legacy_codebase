/*modulator.tp.h*/
//typy danych dla pliku modulator.c
//----------------------------------------------------------
#ifndef	_MODULAT_TPH
#define	_MODULAT_TPH
//--------------------------------------------------------------
//--------------------------------------------------------------
#define _ONEWAR 4	//maksymalna liczba warunkow
#define _ONEkWAR 2	//maksymalna liczba warunkow i parametrow k
typedef struct	{
				unsigned Faza:1;
				unsigned char xt;	//opoznienie dla zmiany lo/hi		
				unsigned char xdt;	//opoznienie dla zmiany hi\lo
				unsigned char tmpPWM;
				}TModul;
typedef struct	{
				unsigned char wr[_ONEWAR]; //warunek1
				//unsigned char wr1; //warunek1
				//unsigned char wr2; //warunek2
				//unsigned char wr3; //warunek3
				//unsigned char wr4; //warunek4
				unsigned char rwr; //relacja miedzy warunkami
				}TWarunek;
typedef struct	{
				unsigned char wr[_ONEkWAR]; //warunek1,warunek2
				signed char kw[_ONEkWAR]; //parametr1,parametr2
				}TkWarunek;
//--------------------------------------------------------------
//--------------------------------------------------------------
#endif	/*_MODULAT_TPH*/
