/*uart9c.c*/
//Funkcje obslugi RS232 w trybie 8 bit bez adresowania.
//Interfejs dla celow wspolpracy z modulem Honeywell
/*---------------------------------------------------------------------------------*/
#include <pic18.h>
#include "global.h"			//parametry globalne
#include "uart9c.h"			//parametry lokalne

extern unsigned char xxy,zzz;
UARTData UADt;
unsigned char bufTUART[_NRS_TBF];	//bufor roboczy modulu UART
unsigned char bufRUART[_NRS_RBF];	//bufor roboczy modulu UART
volatile near unsigned char RSerial;
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
/*-----------------------------PUBLIC DATA-----------------------------------------*/
void StartUSARTRC(const unsigned char nData);
void InitUart(void)
{
	//6 wyjscie portu C ustawic jako wyjscie (0)
	//7 wyjscie portu C ustawic jako wejscie (1)

	CSRC=0;			//asynch mode
	RX9=0;			//odbior 8 bitowy
	SPBRG=_SPBRG;	//baud rate
	SYNC=_SYNC;		//tryb asynchroniczny/synchroniczny
	BRGH=_BRGH;		//predkosc generatora Baud Rate
	ADDEN=0;		//bez adresowania
	TX9=0;			//nadawanie 8 bitowe
	RCIP=1;			//wysoki priorytet przerwania
	RCIE=1;			//aktywne przerwanie dla odbioru
	TXIP=1;			//wysoki priorytet przerwania
	TXEN=1;			//aktywacja modulu transmisji
	SPEN=1;			//aktywacja portu szeregowego
	CREN=0;			//deaktywacja odbioru

	//UWAGA: GIEL=1 oraz GIEH=1
	UADt.RData=0;
	UADt.RErrREC=0;
	UADt.RErrBUF=0;
	UADt.RErrTim=0;
	
	UADt.TData=0;
	UADt.TErrTim=0;
	UADt.TErrTRN=0;

	UADt.RNData=0;
	UADt.TNData=0;
}
//------------------------------------------------------------------------------------	
//------------------------------------------------------------------------------------
//Procedura odczytu odebranego bajtu danych i zapisu do tablicy
//realizowana w przerwaniu generowanym przy odbiorze danych
void IntUSARTRC(void)
{
	if(!FERR&&!OERR) 
	{
		RSerial=RCREG;
		if (UADt.indeks<_NRS_BTY)				//mozliwe przepelnienienie bufora odbioru?
		{
			bufRUART[UADt.indeks++]=RSerial;
			if (UADt.indeks>=UADt.RNData)		//ostatni bajt?
			{	
				CREN=0;							//wylacz odbior (aktywacja w petli glownej po analizie danych)
		 		UADt.RData=0;					//sygnalizuj deaktywacje odbioru danych
				UADt.REnd=1;					//ustaw znacznik odbioru kompletnej paczki danych
			}
		}
		else									//w przypadku bledu przeplnienia przerwij odbior
		{
			CREN=0;							//przerwij odbior								
		 	UADt.RData=0;						//sygnalizuj deaktywacje odbioru danych
			UADt.REnd=0;						//usun znacznik odbioru kompletnej paczki danych
			UADt.RErrBUF=1;						//sygnalizuj blad zbyt duzej ilosci danych
		}
	}
	else										//w przypadku bledu przerwij odbior
	{
		RSerial=RCREG;							//wyzeruj bity OERR i FERR
	 	CREN=0;									//przerwij odbior
	 	UADt.RData=0;							//sygnalizuj deaktywacje odbioru danych
		UADt.REnd=0;							//usun znacznik odbioru kompletnej paczki danych
		UADt.RErrREC=1;							//sygnalizuj blad transmisji danych
	}
}

//------------------------------------------------------------------------------------
//Aktywacja modulu odbioru danych
//Gdzie: nData - ogolna liczba odbieranych bajtow
void StartUSARTRC(const unsigned char nData)
{
	if(CREN) return;					//trwa odbior/transmisja danych?
	if(UADt.RData||UADt.TData) return;	
	UADt.RNData=nData;	
	UADt.indeks=0;
	UADt.REnd=0;				//wyzeruj znacznik konca odbioru

	UADt.RErrREC=0;				//wyzeruj flagu bledow
	UADt.RErrBUF=0;
	UADt.RErrTim=0;
	UADt.TErrTim=0;
	UADt.TErrTRN=0;

	UADt.RData=1;				//ustaw znacznik aktywacji odbioru
	CREN=1;
}
//------------------------------------------------------------------------------------
//Deaktywacja modulu odbioru danych
void StopUSARTRC(void)
{
	UADt.RData=0;							//sygnalizuj deaktywacje odbioru danych
	CREN=0;
}
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//Transmisja danych /procedura wykonywana w przerwaniu/
//Wysylane sa wszystkie bajty poczawszy od adresu odbiornika
void IntUSARTTR(void)
{

	TXREG=bufTUART[UADt.indeks++];				//rozpoczyna transmisje	
	if(UADt.indeks<UADt.TNData)	UADt.TData=1; 	//ustaw znacznik transmisji danych
	else
	{
		UADt.TData=0;							//usun znacznik transmisji danych
		UADt.TEnd=1;							//ustaw znacznik zakonczenia transmisji danych
		TXIE=0;									//deaktywacja transmisji
	}
}	
//------------------------------------------------------------------------------------
//Aktywacja modulu transmisji danych
//Gdzie: nData - ogolna liczba wysylanych bajtow (wlaczywszy adres)
void StartUSARTTR(const unsigned char nData)
{
	if(TXIE) return;					//trwa transmisja danych?
	if(UADt.RData||UADt.TData) return;
	UADt.TNData=nData;					//liczba wysylanych bajtow
	UADt.indeks=0;						//indeks = 0 - pierwszy bajt oznaczony jako adres
	UADt.TEnd=0;						//wyzeruj znacznik konca odbioru

	UADt.RErrREC=0;				//wyzeruj flagu bledow
	UADt.RErrBUF=0;
	UADt.RErrTim=0;
	UADt.TErrTim=0;
	UADt.TErrTRN=0;

	UADt.TData=1;
	TXIE=1;								//aktywacja transmisji
}
//------------------------------------------------------------------------------------
//Deaktywacja modulu odbioru danych
void StopUSARTTR(void)
{
	UADt.TData=0;					//usun znacznik transmisji danych
	TXIE=0;							//deaktywacja transmisji
}
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//Wyzerowanie bufora odbiornika
void ClrBufRUART(void)
{
	unsigned char i;
	for(i=0;i<_NRS_RBF;i++) bufRUART[i]=0;
}
//------------------------------------------------------------------------------------
//Wyzerowanie bufora nadajnika
void ClrBufTUART(void)
{
	unsigned char i;
	for(i=0;i<_NRS_TBF;i++) bufTUART[i]=0;
}
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//Zapis danych do bufora nadajnika
void SetBufTUART(const unsigned char indeks,const unsigned char data)
{
	bufTUART[indeks]=data;	
}
//------------------------------------------------------------------------------------
//Odczyt danych z bufora odbiornika
unsigned char GetBufRUART(const unsigned char indeks)
{
	return bufRUART[indeks];	
}
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
