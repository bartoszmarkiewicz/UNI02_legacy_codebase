/*uart9b.c*/
//Funkcje obslugi RS232 w trybie 9 bit z adresowaniem.

/*---------------------------------------------------------------------------------*/
#include <pic18.h>
#include "global.h"			//parametry globalne
#include "uart9b.h"			//parametry lokalne


UARTData UADt;
unsigned char bufTUART[_NRS_TBF];	//bufor roboczy modulu UART
unsigned char bufRUART[_NRS_RBF];	//bufor roboczy modulu UART
volatile near unsigned char RSerial;
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
/*-----------------------------PUBLIC DATA-----------------------------------------*/
void InitUart(void)
{
	//6 wyjscie portu C ustawic jako wyjscie (0)
	//7 wyjscie portu C ustawic jako wejscie (1)

	CSRC=0;			//asynch mode
	RX9=1;			//odbior 9 bitowy
	SPBRG=_SPBRG;	//baud rate
	SYNC=_SYNC;		//tryb asynchroniczny/synchroniczny
	BRGH=_BRGH;		//predkosc generatora Baud Rate
	ADDEN=1;		//z adresowaniem
	TX9=1;			//nadawanie 9 bitowe
	RCIP=1;			//wysoki priorytet przerwania
	RCIE=1;			//aktywne przerwanie dla odbioru
	TXIP=1;			//wysoki priorytet przerwania
	TXEN=1;			//aktywacja modulu transmisji
	SPEN=1;			//aktywacja portu szeregowego
	//CREN=1;			//aktywacja odbioru

	//UWAGA: GIEL=1 oraz GIEH=1
	UADt.RData=0;
	UADt.RErrREC=0;
	UADt.RErrBUF=0;
	
	UADt.TData=0;
	UADt.RNData=0;
	UADt.TNData=0;
}
//------------------------------------------------------------------------------------	
//Procedura odczytu odebranego bajtu danych /wykonaywana w przerwaniu generowanym przy odbiorze danych/
//W buforze zapisywane sa wszystkie odebrane bajty poczawszy od bajtu adresu.	
void IntUSARTRC(void)
{
	if(!FERR&&!OERR) 
	{
		if(ADDEN&&RX9D)							//RX9D - ustawiony 9 ty bit? (znacznik adresu)
		{
			RSerial=RCREG;
			if(RSerial==_MY_ADDR)				//parawidlowy adres?
			{
				ADDEN=0;						//wylacz detekcje adresu
				UADt.indeks=0;					//wyzeruj indeks
				bufRUART[UADt.indeks++]=RSerial;//zapisz adres w buforze pod 0 indeksem
				UADt.RData=1;					//sygnalizuj aktywny odbior danych
			}
			return;								//wyjdz z przerwania
		}
		RSerial=RCREG;
		if (UADt.indeks<_NRS_RBF)				//mozliwe przepelnienienie bufora odbioru?
		{
			bufRUART[UADt.indeks++]=RSerial;
			if (UADt.indeks>=UADt.RNData)		//ostatni bajt?
			{	
				CREN=0;							//wylacz odbior (aktywacja w petli glownej po analizie danych)
				ADDEN=1;						//zalacz detekcje adresu						
		 		UADt.RData=0;					//sygnalizuj deaktywacje odbioru danych
				UADt.REnd=1;					//ustaw znacznik odbioru kompletnej paczki danych
			}
		}
		else									//w przypadku bledu przeplnienia przerwij odbior
		{
			CREN=0;								//przerwij odbior								
			ADDEN=1;							//uaktywnij detekcje adresu
		 	UADt.RData=0;						//sygnalizuj deaktywacje odbioru danych
			UADt.REnd=0;						//usun znacznik odbioru kompletnej paczki danych
			UADt.RErrBUF=1;						//sygnalizuj blad zbyt duzej ilosci danych
		}
	}
	else										//w przypadku bledu przerwij odbior
	{
		RSerial=RCREG;							//wyzeruj bity OERR i FERR
	 	CREN=0;									//przerwij odbior
		ADDEN=1;								//uaktywnij detekcje adresu
	 	UADt.RData=0;							//sygnalizuj deaktywacje odbioru danych
		UADt.REnd=0;							//usun znacznik odbioru kompletnej paczki danych
		UADt.RErrREC=1;							//sygnalizuj blad transmisji danych
	}
}
//------------------------------------------------------------------------------------
//Aktywacja modulu odbioru danych
//Gdzie: nData - ogolna liczba odbieranych bajtow (wlaczywszy adres)
void StartUSARTRC(const unsigned char nData)
{
	if(CREN) return;			//trwa odbior/transmisja danych?
	UADt.RNData=nData;	
	UADt.REnd=0;				//wyzeruj znacznik konca odbioru
	ADDEN=1;
	CREN=1;
}
//Deaktywacja modulu odbioru danych
void StopUSARTRC(void)
{
	CREN=0;
	UADt.REnd=0;							//usun znacznik odbioru kompletnej paczki danych
	UADt.RData=0;							//sygnalizuj deaktywacje odbioru danych
}
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//Transmisja danych /procedura wykonywana w przerwaniu/
//Wysylane sa wszystkie bajty poczawszy od adresu odbiornika
void IntUSARTTR(void)
{
	if(!UADt.indeks) TX9D=1;		//dla zerowego indeksu ustaw znacznik adresu
	else TX9D=0;					//dla pozostalych danych usun znacznik adresu
	TXREG=bufTUART[UADt.indeks++];	//rozpoczyna transmisje			
	if(UADt.indeks<UADt.TNData)	UADt.TData=1; //ustaw znacznik transmisji danych
	else
	{
		UADt.TData=0;				//usun znacznik transmisji danych
		UADt.TEnd=1;				//ustaw znacznik zakonczenia transmisji danych
		TXIE=0;						//deaktywacja transmisji
	}
}	
//------------------------------------------------------------------------------------
//Aktywacja modulu odbioru danych
//Gdzie: nData - ogolna liczba wysylanych bajtow (wlaczywszy adres)
void StartUSARTTR(const unsigned char nData)
{
	if(TXIE) return;				//trwa transmisja danych?
	UADt.TNData=nData;				//liczba wysylanych bajtow
	UADt.indeks=0;					//indeks = 0 - pierwszy bajt oznaczony jako adres
	UADt.TEnd=0;					//wyzeruj znacznik konca odbioru
	TXIE=1;							//aktywacja transmisji
}
//Deaktywacja modulu odbioru danych
void StopUSARTTR(void)
{
	TXIE=0;							//deaktywacja transmisji
	UADt.TData=0;					//usun znacznik transmisji danych
	UADt.TEnd=0;					//wyzeruj znacznik konca odbioru
}
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//Wyzerowanie bufora odbiornika
void ClrRSRBuf(void)
{
	unsigned char i;
	for(i=0;i<_NRS_RBF;i++) bufRUART[i]=0;
}
//------------------------------------------------------------------------------------
//Wyzerowanie bufora nadajnika
void ClrRSTBuf(void)
{
	unsigned char i;
	for(i=0;i<_NRS_TBF;i++) bufTUART[i]=0;
}
//------------------------------------------------------------------------------------
//Zapis danych do bufora nadajnika
void SetRSTBuf(const unsigned char indeks,const unsigned char data)
{
	bufTUART[indeks]=data;	
}
//------------------------------------------------------------------------------------
//Odczyt danych z bufora odbiornika
unsigned char GetRSRBuf(const unsigned char indeks)
{
	return bufRUART[indeks];	
}
//------------------------------------------------------------------------------------
//Oblicza CHK z nData bajtow bufora bf (bufor danych musi zawierac przynajmniej 2 elementy)
//Gdzie bf - adres pierwszego bajtu, nData - liczba bajtow z ktorych liczone bedzie CHK 
unsigned char LiczRSCHK(const unsigned char *bf,const unsigned char nData)
{
	unsigned char i,chk;
	chk=0;
	for(i=0;i<nData;i++) chk+=(*(bf+i)+i)^0xAA;
	return chk;
}
//------------------------------------------------------------------------------------
//Oblicza CHK i zapisuje na ostatniej pozycji bufora danych do wysylki
//nData - ogolna liczba wysylanych bajtow (lacznie z adresem i bajtem CHK)
void SetRSCHK(const unsigned char nData)
{
	UADt.CHK=LiczRSCHK(bufTUART,nData-1); 	//liczy CHK z pominieciem ostatniego bajtu (bajt przeznaczony na zapis obliczonego CHK.)
	bufTUART[nData-1]=UADt.CHK;				//zapisz w buforze danych do wysylki (na ostatniej pozycji) obliczona wartosc CHK
}
//------------------------------------------------------------------------------------
//Funkcja przeznaczona dla sytuacji gdy po wyslaniu paczki danych spodziewamu sie otrzymac odpowiedz w postaci dwoch bajtow:
//adresu i bajtu potwierdzenia. 
//Funkcja zwraca wartosc 1 gdy bajt potwierdzenia = 0xff wynik pozytywny.
unsigned char VerifyRSACK(void)
{
	if(bufRUART[1]==0xff) return 1; 	//porownuje drugi odebrany bajt z obliczonym wczesniej CHK dla paczki wysylanych danych
	return 0;									
}
//------------------------------------------------------------------------------------
//Oblicza CHK z odebranych danych (pominawszy ostani bajt) i porownuje otrzymany wynik z ostatnim odebranym bajtem
//nData - ogolna liczba odebranych bajtow (lacznie z adresem i bajtem CHK)
//Funkcja zwraca wartosc 1 gdy porownanie ma wynik pozytywny.
unsigned char VerifyRSCHK(const unsigned char nData)
{
	if(bufRUART[nData-1]==LiczRSCHK(bufRUART,nData-1)) return 1; 	//liczy CHK z pominieciem ostatniego bajtu (bajt przeznaczony na zapis obliczonego CHK.)
	return 0;														//i porownuje z ostatnim odebranym bajtem.
}
//------------------------------------------------------------------------------------
//Odczyt obliczonej wartosci CHK (rejestracja CHK odbywa sie za pomoca procedury SetRSCHK(nData)
unsigned char GetRSCHK(void)
{
	return UADt.CHK;	
}
//------------------------------------------------------------------------------------
