/*i2c_s.c*/
/*I2C Slave Mode*/
//-------------------------------------------------------------------------------
//#include <pic18.h>
#include <xc.h>
#include "global.h"			//parametry globalne
#include "i2c_s.h"			//parametry lokalne
#include "main.tp.h"
//#include "uart9b.h"			//parametry lokalne

volatile static DataI2C	DtI2C;
volatile static bfI2C	BufI2C;

//extern volatile UARTData UADt;
extern volatile unsigned char x,y;
extern volatile unsigned char xxy,zzz,vvv;
extern DataPFN PFN;
extern tpRSDTA1 RSDT1;

extern unsigned char ccc;
//-------------------------------------------------------------------------------
//Inicjalizacja modulu I2C
void InitI2C(void)
{
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

	CKP=1;					//Release clock	
    SEN = 1;                //Stretch enable bit

	//SSPSTAT
	SMP=1;					//standart speed mode
	CKE=0;					//data transmited on rising edge

	//PIE
	SSPIE=1;				//serial interrupt enable bit
	//BCLIE=1;				//bus collision interrupt enable bit
	
	//IPR
	//low prioriy interrupt
	SSPIP=0;
	
	BufI2C.indeks=0;
	DtI2C.error=0;
	DtI2Cf_krok=0;
    SSPEN=1;				//enables the serial port
}
//-------------------------------------------------------------------------------
//Procedura transferu/odbioru danych
//UWAGA: umiescic w przerwaniu od SSPIF.
//UWAGA: dla SEN=1 nastepuje autozatrzymanie zegara.Przy wyjsciu z procedury nalezy uwolnic zegar poleceniem CKP=1
volatile static unsigned char dt;
void I2CSlaveTR(void)
{
	if(WCOL)							//nadpisanie danej przy zapisie?
	{ 
		WCOL=0;
		DtI2C.error=_ErWCOL;
	}
	else
	if(SSPOV) 							//nadpisanie danej przy odbiorze?
	{
		SSPOV=0;
		DtI2C.error=_ErSSPOV;
        dt = SSPBUF;
	}
	else
	{ 
		if(!DA)							//adres czy dane?
		{	
			//ADRES
			if(RW)  //transmisja danych
			{
				//Przygotowanie do transmisji danych
				DtI2Cf_krok=1;
                dt = SSPBUF;
				SetTData();
				BufI2C.Bf[_NBDATA-1]=LiczCHK(_NBDATA);
				BufI2C.indeks=0;
			}
			else 
			{
				//Przygotowanie do odbioru danych
				DtI2Cf_krok=0;
				SetRData();
				dt=SSPBUF;
                CKP = 1;    //uwolnij zegar
				return;
			}
		}
		//DATA
		//Odbior danych
		if(DtI2Cf_krok==0) 
		{
			ReceptData();
            CKP = 1;    //uwolnij zegar
			return;
		}
		//Transmisja danych
		if(DtI2Cf_krok==1) 
		{
			TransmitData();
            CKP = 1;    //uwolnij zegar
			return;
		}
	}
	//Koniec transmisji/odbioru
    dt = SSPBUF;
	DtI2Cf_krok=0;
	BufI2C.indeks=0;
    CKP = 1;    //uwolnij zegar
}
//-------------------------------------------------------------------------------
//Oblicza CHK i umieszcza go w ostatnim bajcie bufora danych
//NBTI2C - liczba danych bez adresu z CHK
unsigned char LiczCHK(const unsigned char NBTI2C)
{
	unsigned char i;
	unsigned char chk=0;
	
	for(i=0;i<NBTI2C-1;i++) chk+=(BufI2C.Bf[i]+i)^0xAA;
	return chk;		
}
//-------------------------------------------------------------------------------
//Wysyla dane do odbiornika
void TransmitData(void)
{
	if(RW)				//ACK lub bit STOP?
	{
		if(BufI2C.indeks < _NBSIZE) SSPBUF=BufI2C.Bf[BufI2C.indeks];
                else SSPBUF = BufI2C.Bf[_NBSIZE - 1];
		//CKP=1;
		if(BufI2C.indeks<_NBDATA-1) BufI2C.indeks++;	
	}
	else
	{
		//CKP=1;
		BufI2C.indeks=0;		
	}
}
//-------------------------------------------------------------------------------
//Odbiera nadeslane dane
void ReceptData(void)
{
    dt = SSPBUF;
	if(BufI2C.indeks<_NRCDT)
	{
		BufI2C.Bf[BufI2C.indeks]=dt;
		BufI2C.indeks++;
		if(BufI2C.indeks==_NRCDT-1)
        {
            if(LiczCHK(_NRCDT)==BufI2C.Bf[_NRCDT-1]) ToWork();
        }
	}
}
//-------------------------------------------------------------------------------
//Operacje wykonywane po odebraniu danych
void ToWork(void)
{

}

//-------------------------------------------------------------------------------
//Zapis danych przeznaczonych do wysylki do bufora
void SetTData(void)
{
	//static unsigned char d;

	//d=(d>=99)?0:d+1;
	BufI2C.Bf[0]=0xD1;
	BufI2C.Bf[1]=xxy;
	BufI2C.Bf[2]=zzz;//0;//y;
	BufI2C.Bf[3]=vvv;//zzz;
	BufI2C.Bf[4]=0;
	BufI2C.Bf[5]=0;//x;
	BufI2C.Bf[6]=0;
	BufI2C.Bf[7]=0;
	BufI2C.Bf[8]=0;
	BufI2C.Bf[9]=0;
	BufI2C.Bf[10]=0;
	BufI2C.Bf[11]=0;
	BufI2C.Bf[12]=0;
	BufI2C.Bf[13]=0;
	BufI2C.Bf[14]=0;
	BufI2C.Bf[15]=0;
	BufI2C.Bf[16]=0;
	BufI2C.Bf[17]=0;
	BufI2C.Bf[18]=0;
	BufI2C.Bf[19]=0;
	BufI2C.Bf[20]=0;
}
//-------------------------------------------------------------------------------
//Przygotowanie na odbior danych
void SetRData(void)
{
	BufI2C.indeks=0;
}
//-------------------------------------------------------------------------------
//Formatowanie danych na potrzeby modulu IMY-02
void FormatDataIMY(void)
{

}
//-------------------------------------------------------------------------------
