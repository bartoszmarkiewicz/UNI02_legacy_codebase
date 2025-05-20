/*i2c_s.tp.h*/
/*I2C Slave Mode*/
//----------------------------------------------------------
#ifndef	_I2C_S_TPH
#define	_I2C_S_TPH
//----------------------------------------------------------
#define _NBSIZE		24				//rozmiar bufora danych
//----------------------------------------------------------
typedef struct 	{
				unsigned krok:1;
				unsigned char error;
//-----------------
				unsigned char kod;
				unsigned char PCO;
				unsigned char CO;
				unsigned char PCW;
				unsigned char CW;
				unsigned char CS;
				unsigned char STR_PWM;
				unsigned char MAX_PWM;
				unsigned char BUF_PWM;
				unsigned char ST_AD1;
				unsigned char ST_AD2;							
				unsigned char ST_AD3;
				unsigned char PORTC;
				unsigned char PORTD;
				unsigned char ST_GLOBAL0;
				unsigned char ST_GLOBAL2;
				unsigned char ST_GLOBAL3;
				unsigned char ST_GLOBAL4;
				unsigned char TPRZ_CW;
				unsigned char ST0_WSW;
				unsigned char ERR_BTY;
				}DataI2C;
typedef struct	{
				unsigned char indeks;
				unsigned char Bf[_NBSIZE];
				}bfI2C;
//----------------------------------------------------------
#endif /*_I2C_S_TPH*/
