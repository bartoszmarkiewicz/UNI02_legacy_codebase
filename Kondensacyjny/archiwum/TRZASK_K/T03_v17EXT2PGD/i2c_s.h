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


//Prototypy funkcji
unsigned char LiczCHK(const unsigned char NBTI2C);
void TransmitDataI2C(void);
void ReceptDataI2C(void);
void ToWork(void);
void SetTData(void);
void SetRData(void);
void FormatDataIMY(void);
//bit.c
extern void BitSet(unsigned char *data, const unsigned char nbit,const unsigned char bt);
extern bit GetBit(const unsigned char data,const unsigned char nbit);
//port.c
extern bit RdPrt(const unsigned char Index);

extern bit Trosnie(const unsigned char t);
extern bit Tnizsza(const unsigned char t);

extern unsigned char ReadEEPCO(void);
extern unsigned char ReadEEPCW(void);
extern unsigned char ReadEEPFN(void);

unsigned char ProcToVNT(unsigned char wrt);
unsigned char DVNTtoPWM(unsigned char dvnt);

extern unsigned char GetHBufSize(void);
//----------------------------------------------------------
#endif /*_I2CS_H*/
