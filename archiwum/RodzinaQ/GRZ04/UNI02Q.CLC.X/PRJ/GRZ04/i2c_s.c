/*i2c_s.c*/
/*I2C Slave Mode*/
//-------------------------------------------------------------------------------
#include <xc.h>
#include <stdbool.h>
#include <stdint.h>

#include "../../proj.tp.h"

#ifndef _TYP_PRJ
#error NIEOKRESLONY IDENTYFIKATOR PROJEKTU
#endif

#if _TYP_PRJ==4
#advisory _TYP_PRJ==4 
#include "../../mcc_generated_files\mcc.h"

#include "global.h"
#include "main.h"
#include "global.ex.h"

#include "i2c_s.h"			//parametry lokalne

extern volatile tpMAIN M;
extern unsigned char x,kll,hll,gll,bll;
extern unsigned char xxy,zzz,vvv,mmm;
//extern unsigned char HBL,HRF;
//extern signed char DVNT,dDVNT,ddDVNT,tdDVNT,v_t,v_dt,v_ddt,v_tdt;

extern volatile unsigned char Hind,HNPz;
extern volatile unsigned char HRef;

volatile DataI2C	DtI2C;
volatile bfI2C	BufI2C;

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
volatile unsigned char i2c1RdData;     
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//---------------------------UWAGA---------------------------------------
//PONIZSZE NALEZY ZAKTUALIZOWAC PO ZMIANIE RODZAJU MIKROKONTROLERA 

static inline void I2C1_SlaveSendTxData(uint8_t data)
{
    SSP1BUF = data;
}
static inline uint8_t I2C1_SlaveGetRxData()
{
    return SSP1BUF;
}	
static inline bool I2C1_SlaveIsAddr()
{
    return !(SSP1STATbits.D_nA);
}

static inline bool I2C1_SlaveIsRead()
{
    return (SSP1STATbits.R_nW);
}

static inline void I2C1_SlaveClearIrq()
{
    PIR3bits.SSP1IF = 0;
}

static inline void I2C1_SlaveReleaseClock()
{
    SSP1CON1bits.CKP = 1;
}

static inline bool I2C1_SlaveIsWriteCollision()
{
    return SSP1CON1bits.WCOL;
}

static inline bool I2C1_SlaveIsReceiveOverflow(void)
{
    return SSP1CON1bits.SSPOV;
}

static inline void I2C1_SlaveResetWriteCollision()
{
    SSP1CON1bits.WCOL=0;
}

static inline void I2C1_SlaveResetReceiveOverflow()
{
    SSP1CON1bits.SSPOV=0;
}
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//funkcja wywolywana w przerwaniu
void inline I2CSlaveTR(void)
{   
    volatile unsigned char i,chk;
    
    I2C1_SlaveClearIrq();    
    // read SSPBUF to clear BF
    i2c1RdData = I2C1_SlaveGetRxData();
    
    if(I2C1_SlaveIsWriteCollision())
    {
        I2C1_SlaveResetWriteCollision();   
    }        
	else    
    if(I2C1_SlaveIsReceiveOverflow())  //Receive Overflow
    {
        I2C1_SlaveResetReceiveOverflow();  
    }        
	else
	{ 
		if(I2C1_SlaveIsAddr())							//adres czy dane?
		{	
			//ADRES
			if(I2C1_SlaveIsRead()) 						//transmisja danych
			{
				//Przygotowanie do transmisji danych
				DtI2C.krok=1;
				SetTData();
                chk=0;
                for(i=0;i<(_NBDATA-1);i++) chk+=(BufI2C.Bf[i]+i)^0xAA;
                BufI2C.Bf[_NBDATA-1]=chk;
				BufI2C.indeks=0;
			}
			else                                        //odbior danych
			{
				//Przygotowanie do odbioru danych
				DtI2C.krok=0;
				SetRData();
                I2C1_SlaveReleaseClock();    //uwolnij zegar
				return;
			}
		}
		//DATA
		//Odbior danych
		if(DtI2C.krok==0) 
		{
            if(BufI2C.indeks<_NRCDT)
            {
                BufI2C.Bf[BufI2C.indeks]=i2c1RdData;
                BufI2C.indeks++;
                chk=0;
                for(i=0;i<(_NBDATA-1);i++) chk+=(BufI2C.Bf[i]+i)^0xAA;                
                if(BufI2C.indeks==_NRCDT-1)
                if(chk==BufI2C.Bf[_NRCDT-1]) ToWork();    
            }            
            I2C1_SlaveReleaseClock();    //uwolnij zegar
			return;
		}
		//Transmisja danych
		if(DtI2C.krok==1) 
		{
            if(I2C1_SlaveIsRead()) 						//transmisja danych
            {
                if(BufI2C.indeks<_NBSIZE) I2C1_SlaveSendTxData(BufI2C.Bf[BufI2C.indeks]);
                else I2C1_SlaveSendTxData(BufI2C.Bf[_NBSIZE-1]);
                if(BufI2C.indeks<_NBDATA-1) BufI2C.indeks++;	
            }
            else
            {
                BufI2C.indeks=0;		
            }            
            I2C1_SlaveReleaseClock();    //uwolnij zegar
			return;
		}
	}
	//Koniec transmisji/odbioru
	DtI2C.krok=0;
	BufI2C.indeks=0;
    I2C1_SlaveReleaseClock();    //uwolnij zegar
}
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//---------------------------UWAGA---------------------------------------
//Inicjalizacja modulu I2C
//UWAGA!: wywolanie umiescic w main.c zaraz po wywolaniu SYSTEM_Initialize();
void InitDtaI2C(void)
{
    /*
	//TRISC
	TRISC3=1;				//SDA i SCA konfiguracja jako wejscia
	TRISC4=1;

	//SSPADD
	SSPADD=_I2CSPADD;		
	
	//SSPCON1
	SSPM0=0;
	SSPM1=1;
	SSPM2=1;
	SSPM3=0;				//I2C Slave Mode, 7 bit address

    SEN=1;                  //Stretch enable bit
	CKP=1;					//Release clock	

	//SSPSTAT
	SMP=1;					//standart speed mode
	CKE=0;					//data transmited on rising edge

	//PIE
	SSPIE=1;				//serial interrupt enable bit
	//BCLIE=1;				//bus collision interrupt enable bit
	
	//IPR
	//high prioriy interrupt				
	SSPIP=1;
    SSPEN=1;				//enables the serial port				
	*/
    //I2C1_SlaveSetIsrHandler(I2CSlaveTR);        //przekieruj przerwanie na funkcje I2CSlaveTR()
	BufI2C.indeks=0;
	DtI2C.error=0;
	DtI2C.krok=0;
}
//-------------------------------------------------------------------------------
#endif