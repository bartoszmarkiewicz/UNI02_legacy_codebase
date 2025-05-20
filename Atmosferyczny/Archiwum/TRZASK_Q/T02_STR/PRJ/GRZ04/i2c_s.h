/*i2c_s.h*/
/*I2C Slave Mode*/
#ifndef	_I2CS_H
#define	_I2CS_H
#include "i2c_s.tp.h"		//typy danych

/*---------------------------------------------------------------------------------*/
/*-----------------------------PUBLIC DATA-----------------------------------------*/
//UWAGA: rozmir bufora danych okreslony zostal w pliku i2c_s.tp.h
#define _NBDATA		22				//rzeczywista liczba danych w buforze
#define _NRCDT		3				//liczba danych do ew. odbioru
/*---------------------------------------------------------------------------------*/
/*-----------------------------PROTECTED DATA--------------------------------------*/
/*---------------------------------------------------------------------------------*/

#define _I2CSPADD	0b11001100		//adres wlasny (odbiornika)

//----------------------------------------------------------
//Wartosci jakie mo¿e przyjmowaæ DtI2C.error:
//odbior danych
#define _ErWCOL		7				//nadpisanie danej przy zapisie
#define _ErSSPOV	8				//nadpisanie danej przy odbiorze

//----------------------------------------------------------
//Prototypy funkcji
void inline ToWork(void);
void inline SetRData(void);
extern void inline SetTData(void);
//----------------------------------------------------------
#endif /*_I2CS_H*/
