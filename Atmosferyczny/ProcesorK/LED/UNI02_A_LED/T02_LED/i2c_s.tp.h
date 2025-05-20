/*i2c_s.tp.h*/
/*I2C Slave Mode*/
//----------------------------------------------------------
#ifndef	_I2C_S_TPH
#define	_I2C_S_TPH
//----------------------------------------------------------
#define _NBSIZE		24				//rozmiar bufora danych
//----------------------------------------------------------
volatile bit DtI2Cf_krok;


typedef struct sDataI2C	{
    /*struct{
            unsigned krok:1;
            unsigned int flags0;
    };*/
                
    volatile unsigned char error;
//-----------------
    volatile unsigned char kod;
    volatile unsigned char PCO;
    volatile unsigned char CO;
    volatile unsigned char PCW;
    volatile unsigned char CW;
    volatile unsigned char CS;
    volatile unsigned char STR_PWM;
    volatile unsigned char MAX_PWM;
    volatile unsigned char BUF_PWM;
    volatile unsigned char ST_AD1;
    volatile unsigned char ST_AD2;							
    volatile unsigned char ST_AD3;
    volatile unsigned char PORTC;
    volatile unsigned char PORTD;
    volatile unsigned char ST_GLOBAL0;
    volatile unsigned char ST_GLOBAL2;
    volatile unsigned char ST_GLOBAL3;
    volatile unsigned char ST_GLOBAL4;
    volatile unsigned char TPRZ_CW;
    volatile unsigned char ST0_WSW;
    volatile unsigned char ERR_BTY;
}DataI2C;

typedef struct sbfI2C {
    volatile unsigned char indeks;
    volatile unsigned char Bf[_NBSIZE];
}bfI2C;
//----------------------------------------------------------
#endif /*_I2C_S_TPH*/
