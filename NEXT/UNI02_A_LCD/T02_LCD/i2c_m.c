/*i2c_m.c*/
/*I2C Master Mode*/

#include <pic18.h>
#include "global.h"			//parametry globalne
#include "i2c_m.h"			//parametry lokalne

stI2C	StatI2C;
static bfI2C	BufI2C;
//-------------------------------------------------------------------------------
void InitI2C(void)
{
	//TRISC
	TRISC3=1;				//SDA i SCA konfiguracja jako wejscia
	TRISC4=1;

	//SSPADD
	SSPADD=_I2CSPADD;		//I2C Master Mode clock=Fosc/(4*(SSPADD+1))
	
	//SSPCON1
	SSPM0=0;
	SSPM1=0;
	SSPM2=0;
	SSPM3=1;				//I2C Master Mode, clock=Fosc/(4*(SSPADD+1))
	SSPEN=1;				//enables the serial port
	
	//SSPSTAT
	SMP=1;					//standart speed mode
	CKE=0;					//data transmited on rising edge

	//PIE
	SSPIE=1;				//serial interrupt enable bit
	BCLIE=1;				//bus collision interrupt enable bit
	
	//IPR
	SSPIP=1;				//high prioriy interrupt
	
	BufI2C.indeks=0;
	StatI2C.error=0;
	StatI2C.krok=0;
}
//-------------------------------------------------------------------------------
//Timer na potrzeby transmisji (umiescic w przerwaniu od zegara TMR1)
void IncI2CTimer(void)
{
	if(StatI2C.timI2C<255) StatI2C.timI2C++;
	if(StatI2C.timOutI2C<255) StatI2C.timOutI2C++;
}
//-------------------------------------------------------------------------------
//Skladowe procedury procesu transmisji/odbioru
void CpSSPIF(void)
{
	StatI2C.SSPIF=1;
}
void CpBCLIF(void)
{
	StatI2C.BCLIF=1;
}
bit StanSSPIF(void)
{
	if(StatI2C.SSPIF)
	{
		StatI2C.SSPIF=0;	//pamiec stanu SSPIF
		return 1;
	}
	return 0;
}
bit StanBCLIF(void)
{
	if(StatI2C.BCLIF)
	{
		StatI2C.BCLIF=0;	//pamiec stanu BCLIF
		return 1;
	}
	return 0;
}
bit StanWCOL(void)
{
	if(WCOL)
	{
		WCOL=0;				
		return 1;
	}
	return 0;
}
bit StanSSPOV(void)
{
	if(SSPOV)
	{
		SSPOV=0;				
		return 1;
	}
	return 0;
}
void SetStartBit(void)
{
	SEN=1;					//start zerowany sprzetowo
}
void SetStopBit(void)
{
	PEN=1;					//stop zerowany sprzetowo
}
void SetRecBit(void)
{
	RCEN=1;
}
void SetACK(void)			
{
	ACKDT=0;				//ustaw ACK
	ACKEN=1;				//rozpocznij sekwencje
}
void ClrACK(void)
{
	ACKDT=1;				//wyzeruj ACK
	ACKEN=1;				//rozpocznij sekwencje
}
bit StanACK(void)
{
	return !ACKSTAT;		//potwierdzenie odbioru
}
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//Zegar odcinkow czasowych pomiedzy kolejnymi transmisjami
bit I2CTim(void)
{
	if(StatI2C.timI2C<_I2CPAUSE) return 0;
	return 1;
}
//Start zegara regulujacego odcinki czasowe miedzy transmisjami
void StartI2CTim(void)
{
	StatI2C.timI2C=0;
}
//Restart zegara timout
void StartI2CTimOut(void)
{
	StatI2C.timOutI2C=0;
}
//Odczyt stanu zegara timoutu.
unsigned char I2CTimOut(void)
{
	return StatI2C.timOutI2C;
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//Odczyt wartosci bledu transmisji
unsigned char GetI2CError(void)
{
	return StatI2C.error;
}
//Zapis danej w buforze
void SetDataBufI2C(const unsigned char indeks,const unsigned char data)
{
	BufI2C.Bf[indeks]=data;
}
//Odczyt danej z bufora
unsigned char GetDataBufI2C(const unsigned char indeks)
{
	return BufI2C.Bf[indeks];
}
//Odczyt bitu z bufora
bit GetBitBufI2C(const unsigned char indeks,const unsigned char nbit)
{
	if(BufI2C.Bf[indeks]&(1<<nbit)) return 1;
	return 0;
}
//Ustawienie bitu w buforze
void BitSetBufI2C(const unsigned char indeks,const unsigned char nbit)
{
	BufI2C.Bf[indeks]|=1<<nbit;
}
//Wyzerowanie bitu w buforze
void BitClrBufI2C(const unsigned char indeks,const unsigned char nbit)
{
	BufI2C.Bf[indeks]&=~(1<<nbit);
}
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//****************TRANSMISJA DANYCH***************
//Reakcja na timeout
void TrTimeOut(const unsigned char err)
{
	if(I2CTimOut()>=_I2CTMOUT) 
	{
		StatI2C.error=err;
		StatI2C.krok=5;
	}
}
//Procedura transmisji danych przez I2C
/*bit TransmitI2C(const unsigned char NBTI2C)
{
	StatI2C.krok=0;
	StatI2C.error=0;
	while(StatI2C.krok<7)
	{
		CLRWDT();
		if(StanBCLIF())							  	//bus collision?
		{
			if(StatI2C.krok<5)
			{
				StatI2C.error=_EtBCLIF;
				StatI2C.krok=5;//5
			}
		}
		switch (StatI2C.krok)
		{
			case 0:									//start bit
			{
				SetStartBit();
				StartI2CTimOut();
				BufI2C.indeks=0;
				StatI2C.krok=1;
			};
			case 1:									//oczekiwanie na SSPIF
			{
				if(StanSSPIF()) StatI2C.krok=2;
				else TrTimeOut(_EtTIMO1);
				break;
			};
			case 2:									//transmisja kolejnych bajtow
			{
				if(BufI2C.indeks<NBTI2C)
				{
					SSPBUF=BufI2C.Bf[BufI2C.indeks];
					BufI2C.indeks++;
					StartI2CTimOut();
					StatI2C.krok=3;
				}
				else StatI2C.krok=5;
				break;
			};
			case 3:									//oczekiwanie na SSPIF
			{
				if(StanSSPIF()) 
				{
					StartI2CTimOut();
					StatI2C.krok=4;
				}
				else TrTimeOut(_EtTIMO2);
				break;
			};
			case 4:									//oczekiwanie na ACK
			{
				if(StanACK()) StatI2C.krok=2;
				else TrTimeOut(_EtTIMO3);
				break;	
			}
			case 5:									//stop bit
			{
				SetStopBit();
				StartI2CTimOut();
				StatI2C.krok=6;
			}
			case 6:
			{
				if(StanSSPIF()) StatI2C.krok=7;
				else if(I2CTimOut()>=_I2CTMOUT) 		//oczekiwanie na SSPIF
				{
					StatI2C.error=_EtTIMO4;
					StatI2C.krok=7;
				}
			}
		}
	}
	StanBCLIF();									//wyzeruj znacznik bledu	
	if(StatI2C.error) return 0;
	return 1; 
}*/
bit TransmitI2C(const unsigned char NBTI2C)
{
	switch (StatI2C.krok)
	{
		case 0:									//start bit
		{
			StatI2C.error=0;
			SetStartBit();
			StartI2CTimOut();
			BufI2C.indeks=0;
			StatI2C.krok=1;
		};
		case 1:									//oczekiwanie na SSPIF
		{
			if(StanSSPIF()) StatI2C.krok=2;
			else TrTimeOut(_EtTIMO1);
			break;
		};
		case 2:									//transmisja kolejnych bajtow
		{
			if(BufI2C.indeks<NBTI2C)
			{
				SSPBUF=BufI2C.Bf[BufI2C.indeks];
				BufI2C.indeks++;
				StartI2CTimOut();
				StatI2C.krok=3;
			}
			else StatI2C.krok=5;
			break;
		};
		case 3:									//oczekiwanie na SSPIF
		{
			if(StanSSPIF()) 
			{
				StartI2CTimOut();
				StatI2C.krok=4;
			}
			else TrTimeOut(_EtTIMO2);
			break;
		};
		case 4:									//oczekiwanie na ACK
		{
			if(StanACK()) StatI2C.krok=2;
			else TrTimeOut(_EtTIMO3);
			break;	
		}
		case 5:									//stop bit
		{
			SetStopBit();
			StartI2CTimOut();
			StatI2C.krok=6;
		}
		case 6:
		{
			if(StanSSPIF()) StatI2C.krok=7;
			else if(I2CTimOut()>=_I2CTMOUT) 		//oczekiwanie na SSPIF
			{
				StatI2C.error=_EtTIMO4;
				StatI2C.krok=7;
			}
			break;
		}
		case 7:
		{
			StanSSPIF();
			StanBCLIF();									//wyzeruj znaczniki bledow
			StanWCOL();
			StanSSPOV();
			StatI2C.krok=0;
			return 1;

		}
	}
	return 0;
}
//-------------------------------------------------------------------------------
//****************ODBIOR DANYCH***************
//Reakcja na timeout
void RcTimeOut(const unsigned char err)
{
	if(I2CTimOut()>=_I2CTMOUT) 
	{
		StatI2C.error=err;
		StatI2C.krok=11;
	}
}
//Procedura odboru danych
bit ReceptI2C(const unsigned char NBTI2C)
{
	StatI2C.krok=0;
	StatI2C.error=0;
	while(StatI2C.krok<13)
	{
		CLRWDT();
		if(StanBCLIF()) 							//bus collision?
		{
			if(StatI2C.krok<11)
			{
				StatI2C.error=_ErBCLIF;
				StatI2C.krok=11;//
			}
		}
		if(StanWCOL()) 								//write collision?
		{
			if(StatI2C.krok<11)
			{
				StatI2C.error=_ErWCOL;
				StatI2C.krok=11;
			}
		}
		if(StanSSPOV())								//receive overflow?
		{
			if(StatI2C.krok<11)
			{
				StatI2C.error=_ErSSPOV;
				StatI2C.krok=11;
			}
		}
		switch (StatI2C.krok)
		{
			case 0:									//start bit
			{
				SetStartBit();
				StartI2CTimOut();
				BufI2C.indeks=0;
				StatI2C.krok=1;
			};
			case 1:									//oczekiwanie na SSPIF
			{
				if(StanSSPIF()) StatI2C.krok=2;
				else RcTimeOut(_ErTIMO1);
				break;
			};
			case 2:									//transmisja adresu
			{
				SSPBUF=BufI2C.Bf[0];
				BufI2C.indeks++;
				StartI2CTimOut();
				StatI2C.krok=3;
			};
			case 3:									//oczekiwanie na SSPIF
			{
				if(StanSSPIF()) 
				{
					StartI2CTimOut();
					StatI2C.krok=4;
				}else RcTimeOut(_ErTIMO2);
				break;
			};
			case 4:									//oczekiwanie na ACK
			{
				if(StanACK()) StatI2C.krok=5;
				else RcTimeOut(_ErTIMO3);
				break;	
			};
			case 5:									//aktywacja odbioru danych
			{
				SetRecBit();
				StartI2CTimOut();
				StatI2C.krok=6;
			};
			case 6:									//oczekiwanie na SSPIF
			{
				if(StanSSPIF()) StatI2C.krok=7;
				else RcTimeOut(_ErTIMO4);
				break;
			};
			case 7:									//zapamietanie danej
			{
				BufI2C.Bf[BufI2C.indeks]=SSPBUF;
				BufI2C.indeks++;
				StatI2C.krok=8;
			};
			case 8:									//potwierdzenie/zakonczenie odbioru
			{
				if(BufI2C.indeks<NBTI2C) SetACK();
				else ClrACK();
				StartI2CTimOut();
				StatI2C.krok=9;
			};
			case 9:									//oczekiwanie na SSPIF
			{
				if(StanSSPIF()) StatI2C.krok=10;
				else RcTimeOut(_ErTIMO5);
				break;	
			};
			case 10:								//decyzja o ponowieniu/zakonczeniu odbioru
			{
				if(BufI2C.indeks<NBTI2C) StatI2C.krok=5;
				else StatI2C.krok=11;
				break;
			};
			case 11:								//stop bit
			{	
				SetStopBit();
				StartI2CTimOut();
				StatI2C.krok=12;
			};
			case 12:
			{
				if(StanSSPIF()) StatI2C.krok=13;
				else if(I2CTimOut()>=_I2CTMOUT)		//oczekiwanie na SSPIF
				{
					StatI2C.error=_ErTIMO6;
					StatI2C.krok=13;
				}
			}
		}
	}
	StanBCLIF();									//wyzeruj znaczniki bledow
	StanWCOL();
	StanSSPOV();
	if(StatI2C.error) return 0;
	return 1; 
}
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
