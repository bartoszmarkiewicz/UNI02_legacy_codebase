/*saa.h*/
/*SAA1064*/
#ifndef	_SAA_H
#define	_SAA_H
/*---------------------------------------------------------------------------------*/

//NUMERY KOLEJNYCH BAJTOW UKLADU SAA
#define _ADR_SAA 		0
#define _SADR_SAA 		1
#define _CONTROL_SAA 	2
#define _DIGIT1_SAA		3
#define _DIGIT2_SAA		4
#define _DIGIT3_SAA		5
#define _DIGIT4_SAA		6

//ADRESY 4 UKLADOW SAA
#define _SAA_ADR0 0b01110000
#define _SAA_ADR1 0b01110010
#define _SAA_ADR2 0b01110100
#define _SAA_ADR3 0b01110110

//BITY CONTROL BYTE UKLADU SAA 
#define _STATIC_MODE	0b00000000
#define _DYNAMIC_MODE	0b00000001
#define _D1_3NBLANK		0b00000010
#define _D2_4NBLANK		0b00000100
#define _SEGMENT_TEST	0b00001000
#define _ADD_3mA		0b00010000
#define _ADD_6mA		0b00100000
#define _ADD_12mA		0b01000000
/*---------------------------------------------------------------------------------*/
//i2c_m.c
extern void SetDataBufI2C(const unsigned char indeks,const unsigned char data);
extern unsigned char GetDataBufI2C(const unsigned char indeks);
extern bit GetBitBufI2C(const unsigned char indeks,const unsigned char nbit);
extern void BitSetBufI2C(const unsigned char indeks,const unsigned char nbit);
extern void BitClrBufI2C(const unsigned char indeks,const unsigned char nbit);
/*---------------------------------------------------------------------------------*/
#endif /*_SAA_H*/
