/*auart9b.c*/
//Funkcje obslugi RS232 w trybie 9 bit z adresowaniem dla AUART2.
//Interfejs oparty na ukladzie MAX487
/*---------------------------------------------------------------------------------*/
//#include <pic18.h>
#include <xc.h>
#include "global.h"			//parametry globalne
#include "main.tp.h"
#if (_EXT_BOARD!=0)
#include "auart9b.h"			//parametry lokalne
#include "amgs_rs9b.h"
#include "rtimer.tp.h"
extern volatile unsigned char xxy,zzz,vvv;
extern volatile near RTsekundy	Sk[];//**************************

volatile UARTData UADt;
volatile unsigned char bufTUART[_NRS_TBF];	//bufor roboczy modulu UART
volatile unsigned char bufRUART[_NRS_RBF];	//bufor roboczy modulu UART
volatile near unsigned char RSerial;

extern volatile tpRSDTA1 RSDT1;
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
/*-----------------------------PUBLIC DATA-----------------------------------------*/
void StartUSARTRC(const unsigned char nData);
unsigned char VerifyNDatCHK(const unsigned char Data);
void InitAUart2(void)
{
	//1 wyjscie portu G ustawic jako wyjscie (0)
	//2 wyjscie portu C ustawic jako wejscie (1)

	TXSTA2bits.CSRC=0;			//asynch mode
	RCSTA2bits.RX9=1;			//odbior 9 bitowy
	SPBRG2=_SPBRG;	//baud rate
	TXSTA2bits.SYNC=_SYNC;		//tryb asynchroniczny/synchroniczny
	TXSTA2bits.BRGH=_BRGH;		//predkosc generatora Baud Rate
	RCSTA2bits.ADDEN=1;		//z adresowaniem
	TXSTA2bits.TX9=1;			//nadawanie 9 bitowe
	IPR3bits.RC2IP=1;			//wysoki priorytet przerwania
	PIE3bits.RC2IE=1;			//aktywne przerwanie dla odbioru
	IPR3bits.TX2IP=1;			//wysoki priorytet przerwania
	TXSTA2bits.TXEN=1;			//aktywacja modulu transmisji
	RCSTA2bits.SPEN=1;			//aktywacja portu szeregowego
	RCSTA2bits.CREN=1;			//aktywacja odbioru
	U2OD=0;

	//UWAGA: GIEL=1 oraz GIEH=1
	UADtf_RData=0;
	UADtf_RErrREC=0;
	UADtf_RErrBUF=0;
	
	UADtf_TData=0;
	UADt.RNData=0;
	UADt.TNData=0;

	UADtf_REnd=0;
	UADtf_TEnd=0;
    
    UADtf_REXT=0;                //znacznik rozszerzonej liczby danych
    UADtf_TEXT=0;                //znacznik rozszerzonej liczby danych    
	UADt.indeks=0;
        
	RSerial=RCREG2;
	PIE3bits.TX2IE=0;
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Odbior danych /procedura wykonywana w przerwaniu/
/*void IntUSARTRC(void)
{
	if(RCSTA2bits.ADDEN&&RCSTA2bits.RX9D)							//RX9D - ustawiony 9 ty bit? (znacznik adresu)
	{
		RSerial=RCREG2;
		if(RSerial==_MY_ADDR)				//parawidlowy adres?
		{
			//if(zzz++>99) zzz=0;
			RCSTA2bits.ADDEN=0;						//wylacz detekcje adresu
			UADt.indeks=0;					//wyzeruj indeks
			bufRUART[UADt.indeks++]=RSerial;//zapisz adres w buforze pod 0 indeksem
			UADtf_RData=1;					//sygnalizuj aktywny odbior danych
		}
		return;								//wyjdz z przerwania
	}
	RSerial=RCREG2;
	if (UADt.indeks<_NRS_RBF)				//mozliwe przepelnienienie bufora odbioru?
	{
		bufRUART[UADt.indeks++]=RSerial;
		UADt.RNData=bufRUART[1];
		if(VerifyNDatCHK(UADt.RNData))		//kontroluj poprawnosc danej
		{
			UADt.RNData=UADt.RNData&0b00011111;
			if (UADt.indeks>=UADt.RNData)		//ostatni bajt?
			{	
				RCSTA2bits.ADDEN=1;						//zalacz detekcje adresu
		 		UADtf_RData=0;					//sygnalizuj deaktywacje odbioru danych
				UADtf_REnd=1;					//ustaw znacznik odbioru kompletnej paczki danych
				//if(xxy++>99) xxy=0;
			}
		}
		else
		{
			RCSTA2bits.ADDEN=1;							//uaktywnij detekcje adresu
		 	UADtf_RData=0;						//sygnalizuj deaktywacje odbioru danych
			UADtf_REnd=0;						//usun znacznik odbioru kompletnej paczki danych
			UADtf_RErrBUF=1;						//sygnalizuj blad zbyt duzej ilosci danych
		}
	}
	else									//w przypadku bledu przeplnienia przerwij odbior
	{
		//CREN=0;							//przerwij odbior

		RCSTA2bits.ADDEN=1;							//uaktywnij detekcje adresu
	 	UADtf_RData=0;						//sygnalizuj deaktywacje odbioru danych
		UADtf_REnd=0;						//usun znacznik odbioru kompletnej paczki danych
		UADtf_RErrBUF=1;						//sygnalizuj blad zbyt duzej ilosci danych

	}
}*/

void IntUSARTRC(void)
{
    volatile unsigned char i;
    
	if(RCSTA2bits.ADDEN&&RCSTA2bits.RX9D)							//RX9D - ustawiony 9 ty bit? (znacznik adresu)
	{
		RSerial=RCREG2;
		if(RSerial==_MY_ADDR)				//parawidlowy adres?
		{
			//if(zzz++>99) zzz=0;
			RCSTA2bits.ADDEN=0;						//wylacz detekcje adresu
			UADt.indeks=0;					//wyzeruj indeks
            //++++++++++++++++++++++++++++++++++++++++++++++++++++
            for(i=0;i<_NRS_RBF;i++)         //wyzeruj bufor
            {
                bufRUART[i]=0;
            }
            //++++++++++++++++++++++++++++++++++++++++++++++++++++            
			bufRUART[UADt.indeks++]=RSerial;//zapisz adres w buforze pod 0 indeksem
			UADtf_RData=1;					//sygnalizuj aktywny odbior danych
		}
		return;								//wyjdz z przerwania
	}
	RSerial=RCREG2;
	if (UADt.indeks<_NRS_RBF)				//mozliwe przepelnienienie bufora odbioru?
	{
		bufRUART[UADt.indeks++]=RSerial;
        switch(UADt.indeks)
        {
            case 2: //odebrany drugi bajt (liczba danych)
            {
                UADt.RNData=bufRUART[1];
                if(VerifyNDatCHK(UADt.RNData))		//kontroluj poprawnosc danej
                {
                    UADt.RNData=UADt.RNData&0b00011111;                
                    if(UADt.RNData>_NRS_RBF) UADt.RNData=_NRS_RBF;                
                }
                else
                {
                    //if(vvv<99) vvv++;
                    //else vvv=0;
                    RCSTA2bits.ADDEN=1;							//uaktywnij detekcje adresu
                    UADtf_REXT=0;
                    UADtf_RData=0;						//sygnalizuj deaktywacje odbioru danych
                    UADtf_REnd=0;						//usun znacznik odbioru kompletnej paczki danych
                    UADtf_RErrBUF=1;						//sygnalizuj blad zbyt duzej ilosci danych  
                    return;
                }    
                break;
            }
            case 4: //odebrany czwarty bajt (mozliwa korekta liczby danych)
            {
                if((bufRUART[2]==_RD_KNF2)||(bufRUART[2]==_RD_STD2))    //odebrana paczka konfiguracyjna 
                {
                    if(VerifyNDatCHK(bufRUART[3]))		//kontroluj poprawnosc danej
                    {
                        UADtf_REXT=1;
                        UADt.RNData=UADt.RNData+(bufRUART[3]&0b00011111);                
                        if(UADt.RNData>_NRS_RBF) UADt.RNData=_NRS_RBF;                
                    }
                    else
                    {
                        //if(vvv<99) vvv++;
                        //else vvv=0;
                        RCSTA2bits.ADDEN=1;							//uaktywnij detekcje adresu
                        UADtf_REXT=0;
                        UADtf_RData=0;						//sygnalizuj deaktywacje odbioru danych
                        UADtf_REnd=0;						//usun znacznik odbioru kompletnej paczki danych
                        UADtf_RErrBUF=1;						//sygnalizuj blad zbyt duzej ilosci danych  
                        return;
                    }    
                }
                break;
            }      
        }
        if (UADt.indeks>=UADt.RNData)		//ostatni bajt?
        {	       
            RCSTA2bits.ADDEN=1;						//zalacz detekcje adresu
            UADtf_REXT=0;
            UADtf_RData=0;					//sygnalizuj deaktywacje odbioru danych
            UADtf_REnd=1;					//ustaw znacznik odbioru kompletnej paczki danych
        }
	}
	else									//w przypadku bledu przeplnienia przerwij odbior
	{
		RCSTA2bits.ADDEN=1;							//uaktywnij detekcje adresu
        UADtf_REXT=0;
	 	UADtf_RData=0;						//sygnalizuj deaktywacje odbioru danych
		UADtf_REnd=0;						//usun znacznik odbioru kompletnej paczki danych
		UADtf_RErrBUF=1;						//sygnalizuj blad zbyt duzej ilosci danych
	}   
}
//------------------------------------------------------------------------------------
//Aktywacja modulu odbioru danych
//Gdzie: nData - ogolna liczba odbieranych bajtow (wlaczywszy adres)
void StartUSARTRC(const unsigned char nData)
{
	UADtf_REnd=0;				//wyzeruj znacznik konca odbioru
    UADtf_REXT=0;    
	RCSTA2bits.ADDEN=1;
	RCSTA2bits.CREN=1;
}

//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//Transmisja danych /procedura wykonywana w przerwaniu/
//Wysylane sa wszystkie bajty poczawszy od adresu odbiornika
void IntUSARTTR(void)
{
	if(!UADt.indeks) TXSTA2bits.TX9D=1;		//dla zerowego indeksu ustaw znacznik adresu
	else TXSTA2bits.TX9D=0;					//dla pozostalych danych usun znacznik adresu
	if(UADt.TNData>_NRS_TBF) UADt.TNData=_NRS_TBF;
	if(UADt.indeks>_NRS_TBF-1) UADt.indeks=_NRS_TBF-1;    
	TXREG2=bufTUART[UADt.indeks++];	//rozpoczyna transmisje
	if(UADt.indeks<UADt.TNData)	UADtf_TData=1; //ustaw znacznik transmisji danych
	else
	{
        UADtf_TEXT=0;
		UADtf_TData=0;				//usun znacznik transmisji danych
		UADtf_TEnd=1;				//ustaw znacznik zakonczenia transmisji danych
		PIE3bits.TX2IE=0;						//deaktywacja transmisji
	}
}	
//------------------------------------------------------------------------------------
//Aktywacja modulu odbioru danych
//Gdzie: nData - ogolna liczba wysylanych bajtow (wlaczywszy adres)
void StartUSARTTR(void)//const unsigned char nData)
{
	if(PIE3bits.TX2IE) return;				//trwa transmisja danych?
	UADt.TNData=(bufTUART[1]&0b00011111);//liczba wysylanych bajtow
    if((bufTUART[2]==_RD_KNF2)||(bufTUART[2]==_RD_STD2))            //ramka konfiguracyjna
    {
        UADtf_TEXT=1;
        UADt.TNData=UADt.TNData+(bufTUART[3]&0b00011111);  //zwieksz liczbe danych o wartosc korekty
    }  
	if(UADt.TNData>_NRS_TBF) UADt.TNData=_NRS_TBF;    
	UADt.indeks=0;					//indeks = 0 - pierwszy bajt oznaczony jako adres
	UADtf_TEnd=0;					//wyzeruj znacznik konca odbioru
	PIE3bits.TX2IE=1;							//aktywacja transmisji
}
//Deaktywacja modulu odbioru danych
void StopUSARTTR(void)
{
	PIE3bits.TX2IE=0;							//deaktywacja transmisji
	UADtf_TData=0;					//usun znacznik transmisji danych
	UADtf_TEnd=0;					//wyzeruj znacznik konca odbioru
    UADtf_TEXT=0;	
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
unsigned char LiczRSCHK(volatile unsigned char *bf,const unsigned char nData)
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
	if(bufRUART[2]!=_RD_ERR) return 1; 	//porownuje drugi odebrany bajt z obliczonym wczesniej CHK dla paczki wysylanych danych
	return 0;									
}
/*unsigned char VerifyRSACK(void)
{
	if(bufRUART[2]==0xff) return 1; 	//porownuje drugi odebrany bajt z obliczonym wczesniej CHK dla paczki wysylanych danych
	return 0;									
}*/
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
//Oblicza CHK dla liczby danych i zapisuje na trzech najstarszych bitach
unsigned char SetNDatCHK(const unsigned char Data)
{
	unsigned char a,b,d;
	d=Data&0b00011111;
	a=d&0b00000111;
	b=0b00000100|(d>>3);
	return d|((a^b)<<5);
}
//Weryfikacja wartosci CHK dla liczby danych
unsigned char VerifyNDatCHK(const unsigned char Data)
{
	if(SetNDatCHK(Data)==Data) return 1; 	//liczy CHK z pominieciem ostatniego bajtu (bajt przeznaczony na zapis obliczonego CHK.)
	return 0;														//i porownuje z ostatnim odebranym bajtem.
}
//------------------------------------------------------------------------------------
//Oblicza CHK dla liczby bajtow w rozszerzonej ramce danych
void SetExNDatCHK(const unsigned char ndt)
{
    volatile unsigned char chkndat;
    if(ndt<=31)
    {
        chkndat=SetNDatCHK(ndt);       //oblicz CHK dla liczby danych    
    }
    else chkndat=SetNDatCHK(31);        //oblicz CHK dla liczby danych            
    SetRSTBuf(1,chkndat);				//1 - liczba przesy³anych danych (ndat)
    if(ndt>31)
    {
        chkndat=SetNDatCHK(ndt-31);    //oblicz CHK dla liczby danych    
    }
    else chkndat=SetNDatCHK(0);			//oblicz CHK dla liczby danych
    SetRSTBuf(3,chkndat);               //1 - liczba przesy³anych danych (ndat) 
}
//------------------------------------------------------------------------------------
//Zwraca liczbe bajtow dla rozszerzonej ramki danych
unsigned char GetNDat(void)
{
    return (GetRSRBuf(1)&0b00011111);
}
//------------------------------------------------------------------------------------
//Zwraca liczbe bajtow dla rozszerzonej ramki danych
unsigned char GetExNDat(void)
{
    return (GetRSRBuf(1)&0b00011111)+(GetRSRBuf(3)&0b00011111);
}
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
#endif //(_EXT_BOARD!=0)