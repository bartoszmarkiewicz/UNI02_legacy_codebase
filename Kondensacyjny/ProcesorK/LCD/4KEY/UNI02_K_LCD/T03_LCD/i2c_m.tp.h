/*i2c_m.tp.h*/
/*I2C Master Mode*/
//----------------------------------------------------------
#ifndef	_I2C_M_TPH
#define	_I2C_M_TPH
//----------------------------------------------------------
typedef struct 	{
				unsigned SSPIF:1;
				unsigned BCLIF:1;
				volatile unsigned char timI2C;
				volatile unsigned char timOutI2C;
				unsigned char krok;
				unsigned char error;
				}stI2C;
typedef struct	{
				unsigned char indeks;
				unsigned char Bf[_NBDATA];
				}bfI2C;
//----------------------------------------------------------
#endif /*_I2C_M_TPH*/
