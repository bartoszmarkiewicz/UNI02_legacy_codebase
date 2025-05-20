/*uart9b.c*/
//Funkcje obslugi RS232 w trybie 9 bit z adresowaniem.
/*---------------------------------------------------------------------------------*/
#include <xc.h>
#include <stdbool.h>
#include <stdint.h>

#include "../../proj.tp.h"
 
#ifndef _TYP_PRJ
#error NIEOKRESLONY IDENTYFIKATOR PROJEKTU
#endif

#if _TYP_PRJ==5
#advisory _TYP_PRJ==5 
#include "../../mcc_generated_files\mcc.h"

#include "global.h"
#include "main.h"

#include "uart9b.h"			//parametry lokalne
#include "smgs_rs9b.h"

extern volatile unsigned char xxy,zzz;

UARTData UADt;
unsigned char bufTUART[_NRS_TBF];	//bufor roboczy modulu UART
unsigned char bufRUART[_NRS_RBF];	//bufor roboczy modulu UART
volatile unsigned char RSerial;

//-------------------------------------------------------------------------------
//---------------------------UWAGA-----------------------------------------------
//PONIZSZE NALEZY ZAKTUALIZOWAC PO ZMIANIE RODZAJU MIKROKONTROLERA
static inline void UART1_ClearIrqRx()
{
    //PIR3bits.C = 0;
    Nop();      //RC1IF zerowane sprzetowo
}
static inline void UART1_ClearIrqTx()
{
    //PIR3bits.TX1IF = 0;
    Nop();      //TCIF zerowane sprzetowo
}
static inline bool UART1_IsRCIE()
{
    return (PIE3bits.RC1IE);
}
static inline void UART1_SetRCIE(bool dt)
{
    PIE3bits.RC1IE=dt;
}
static inline bool UART1_IsTXIE()
{
    return (PIE3bits.TX1IE);
}
static inline void UART1_SetTXIE(bool dt)
{
    PIE3bits.TX1IE=dt;
}
static inline void UART1_SendTxData(unsigned char data)
{
    TX1REG = data;
}
static inline unsigned char UART1_GetRxData(void)
{
    return RC1REG;
}
static inline bool UART1_IsEnAddr()       //Address Detect
{
    return (RC1STAbits.ADDEN);
}
static inline void UART1_SetEnAddr(bool dt)       //Address Detect
{
    RC1STAbits.ADDEN=dt;
}
static inline bool UART1_IsRX9D()
{
    return (RC1STAbits.RX9D);
}
static inline void UART1_SetContinuousReceive(bool dt)
{
    RC1STAbits.CREN=dt;    
}
static inline void UART1_SetTX9D(bool dt)
{
    TX1STAbits.TX9D=dt;
}
//--------------------------------------------------------
//---------------------------------------------------------
inline void UART1_SetRCIE_Ex(bool dt)
{
    PIE3bits.RC1IE=dt;
}
inline void UART1_SetTXIE_Ex(bool dt)
{
    PIE3bits.TX1IE=dt;
}
inline void UART1_SetContinuousReceive_Ex(bool dt)
{
    RC1STAbits.CREN=dt;    
}
inline void UART1_SetContinuousReceive_Ex2(bool dt)
{
    RC1STAbits.CREN=dt;    
}
inline bool UART1_TransmitStat_Ex()
{
    return (TX1STAbits.TRMT);
}
inline bool UART1_IsTXIE_Ex()
{
    return (PIE3bits.TX1IE);
}
inline bool UART1_ReceiveStat_Ex()
{
    return (BAUD1CONbits.RCIDL);
}
//------------------------------------------------------------------------------------	
//Procedura odczytu odebranego bajtu danych /wykonaywana w przerwaniu generowanym przy odbiorze danych/
//W buforze zapisywane sa wszystkie odebrane bajty poczawszy od bajtu adresu.	
void inline IntUSARTRC(void)
{
    UART1_ClearIrqRx();

    if(RCSTA1bits.FERR||RCSTA1bits.OERR)
    {
        RSerial=UART1_GetRxData();          //zeruj FERR
        UART1_SetContinuousReceive_Ex(0);   //zeruj OERR
        Nop();
        Nop();
        UART1_SetContinuousReceive_Ex(1);                  
        UART1_SetEnAddr(1);                 //uaktywnij detekcje adresu
        UADt.REXT=0;
        UADt.RData=0;						//sygnalizuj deaktywacje odbioru danych
        UADt.REnd=0;						//usun znacznik odbioru kompletnej paczki danych
        UADt.RErrBUF=1;						//sygnalizuj blad zbyt duzej ilosci danych  
        return;
    }    
	if(UART1_IsEnAddr()&&UART1_IsRX9D())							//RC1STA.RX9D - ustawiony 9 ty bit? (znacznik adresu)
	{
		RSerial=UART1_GetRxData();
		if(RSerial==_MY_ADDR)				//parawidlowy adres?
		{
			UART1_SetEnAddr(0);				//wylacz detekcje adresu
			UADt.indeks=0;					//wyzeruj indeks
			bufRUART[UADt.indeks++]=RSerial;//zapisz adres w buforze pod 0 indeksem
			UADt.RData=1;					//sygnalizuj aktywny odbior danych            
		}
		return;								//wyjdz z przerwania
	}
	RSerial=UART1_GetRxData();
	if (UADt.indeks<_NRS_RBF)				//mozliwe przepelnienienie bufora odbioru?
	{
 
		bufRUART[UADt.indeks++]=RSerial;
        switch(UADt.indeks)
        {
            case 2: //odebrany drugi bajt (liczba danych)
            {
                UADt.RNData=bufRUART[1];
                
                if(iVerifyNDatCHK(UADt.RNData))		//kontroluj poprawnosc danej
                {
                    UADt.RNData=UADt.RNData&0b00011111;                
                    if(UADt.RNData>_NRS_RBF) UADt.RNData=_NRS_RBF;                         
                }
                else
                {
                    UART1_SetEnAddr(1);							//uaktywnij detekcje adresu
                    UADt.REXT=0;
                    UADt.RData=0;						//sygnalizuj deaktywacje odbioru danych
                    UADt.REnd=0;						//usun znacznik odbioru kompletnej paczki danych
                    UADt.RErrBUF=1;						//sygnalizuj blad zbyt duzej ilosci danych 
                    return;
                }    
                break;
            }
            case 4: //odebrany czwarty bajt (mozliwa korekta liczby danych)
            {
                if((bufRUART[2]==_RD_KNF2)||(bufRUART[2]==_RD_STD2))    //odebrana paczka konfiguracyjna 
                {
                    if(iVerifyNDatCHK(bufRUART[3]))		//kontroluj poprawnosc danej
                    {                     
                        UADt.REXT=1;
                        UADt.RNData=UADt.RNData+(bufRUART[3]&0b00011111);                
                        if(UADt.RNData>_NRS_RBF) UADt.RNData=_NRS_RBF;                         
                    }
                    else
                    {
                        UART1_SetEnAddr(1);							//uaktywnij detekcje adresu
                        UADt.REXT=0;
                        UADt.RData=0;						//sygnalizuj deaktywacje odbioru danych
                        UADt.REnd=0;						//usun znacznik odbioru kompletnej paczki danych
                        UADt.RErrBUF=1;						//sygnalizuj blad zbyt duzej ilosci danych  
                        return;
                    }    
                }
                break;
            }      
        }
        if (UADt.indeks>=UADt.RNData)		//ostatni bajt?
        {	       
            UART1_SetEnAddr(1);						//uaktywnij detekcje adresu
            UADt.REXT=0;
            UADt.RData=0;					//sygnalizuj deaktywacje odbioru danych
            UADt.REnd=1;					//ustaw znacznik odbioru kompletnej paczki danych                
        }
	}
	else									//w przypadku bledu przeplnienia przerwij odbior
	{
		UART1_SetEnAddr(1);							//uaktywnij detekcje adresu
        UADt.REXT=0;
	 	UADt.RData=0;						//sygnalizuj deaktywacje odbioru danych
		UADt.REnd=0;						//usun znacznik odbioru kompletnej paczki danych
		UADt.RErrBUF=1;						//sygnalizuj blad zbyt duzej ilosci danych 
	}
}
//------------------------------------------------------------------------------------
//Aktywacja modulu odbioru danych
//Gdzie: nData - ogolna liczba odbieranych bajtow (wlaczywszy adres)
/*
void StartUSARTRC(void)
{
    UADt.REXT=0;
	UADt.REnd=0;				//wyzeruj znacznik konca odbioru
	UART1_SetEnAddr(1);
	UART1_SetContinuousReceive(1);
}
*/
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//Transmisja danych /procedura wykonywana w przerwaniu/
//Wysylane sa wszystkie bajty poczawszy od adresu odbiornika
void inline IntUSARTTR(void)
{
    UART1_ClearIrqTx();
	if(!UADt.indeks) UART1_SetTX9D(1);		//dla zerowego indeksu ustaw znacznik adresu
	else UART1_SetTX9D(0);					//dla pozostalych danych usun znacznik adresu
	if(UADt.TNData>_NRS_TBF) UADt.TNData=_NRS_TBF;
	if(UADt.indeks>_NRS_TBF-1) UADt.indeks=_NRS_TBF-1;
	UART1_SendTxData(bufTUART[UADt.indeks++]);	//rozpoczyna transmisje	
	if(UADt.indeks<UADt.TNData)	UADt.TData=1; //ustaw znacznik transmisji danych
	else
	{
        UADt.TEXT=0;
		UADt.TData=0;				//usun znacznik transmisji danych
		UADt.TEnd=1;				//ustaw znacznik zakonczenia transmisji danych
		UART1_SetTXIE(0);			//dezaktywacja transmisji
        
	}
}	
//------------------------------------------------------------------------------------
//Aktywacja modulu odbioru danych
//Gdzie: nData - ogolna liczba wysylanych bajtow (wlaczywszy adres)
void StartUSARTTR(void)
{
	if(UART1_IsTXIE()) return;                                      //trwa transmisja danych?
    UADt.TEXT=0;
	UADt.TNData=(bufTUART[1]&0b00011111);                           //liczba wysylanych bajtow
    if((bufTUART[2]==_RD_KNF2)||(bufTUART[2]==_RD_STD2))            //ramka konfiguracyjna
    {
        UADt.TEXT=1;
        UADt.TNData=UADt.TNData+(bufTUART[3]&0b00011111);           //zwieksz liczbe danych o wartosc korekty
    }         
	if(UADt.TNData>_NRS_TBF) UADt.TNData=_NRS_TBF;
	UADt.indeks=0;                                                  //indeks = 0 - pierwszy bajt oznaczony jako adres
	UADt.TEnd=0;                                                    //wyzeruj znacznik konca odbioru
	UART1_SetTXIE_Ex(1);                                               //aktywacja transmisji
}
//Deaktywacja modulu odbioru danych
void StopUSARTTR(void)
{
	UART1_SetTXIE_Ex(0);                                               //dezaktywacja transmisji
	UADt.TData=0;                                                   //usun znacznik transmisji danych
	UADt.TEnd=0;                                                    //wyzeruj znacznik konca odbioru
	UADt.TEXT=0;
}
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
/*
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
 */
//------------------------------------------------------------------------------------
//Zapis danych do bufora nadajnika
void SetRSTBuf(const unsigned char indeks,const unsigned char data)
{
	if(indeks<_NRS_TBF) bufTUART[indeks]=data;	
	else bufTUART[_NRS_TBF-1]=data;
	
}
/*
unsigned char GetRSTBuf(const unsigned char indeks)
{
	if(indeks<_NRS_TBF) return bufTUART[indeks];
	else return bufTUART[_NRS_TBF-1];	
}
*/
//------------------------------------------------------------------------------------
//Odczyt danych z bufora odbiornika
unsigned char GetRSRBuf(const unsigned char indeks)
{
	if(indeks<_NRS_RBF) return bufRUART[indeks];
	else return bufRUART[_NRS_RBF-1];	
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
void SetRSCHK(unsigned char nData)
{
	if(nData>_NRS_TBF) nData=_NRS_TBF;
	if(!nData) nData=_NRS_TBF;
	UADt.CHK=LiczRSCHK(bufTUART,nData-1); 	//liczy CHK z pominieciem ostatniego bajtu (bajt przeznaczony na zapis obliczonego CHK.)
	bufTUART[nData-1]=UADt.CHK;				//zapisz w buforze danych do wysylki (na ostatniej pozycji) obliczona wartosc CHK
}
//------------------------------------------------------------------------------------
//Funkcja przeznaczona dla sytuacji gdy po wyslaniu paczki danych spodziewamu sie otrzymac odpowiedz w postaci dwoch bajtow:
//adresu i bajtu potwierdzenia. 
//Funkcja zwraca wartosc 1 gdy bajt potwierdzenia = 0xff wynik pozytywny.
/*
unsigned char VerifyRSACK(void)
{
	if(bufRUART[2]==0xff) return 1; 	//porownuje drugi odebrany bajt z obliczonym wczesniej CHK dla paczki wysylanych danych
	return 0;									
}
 */
//------------------------------------------------------------------------------------
//Oblicza CHK z odebranych danych (pominawszy ostani bajt) i porownuje otrzymany wynik z ostatnim odebranym bajtem
//nData - ogolna liczba odebranych bajtow (lacznie z adresem i bajtem CHK)
//Funkcja zwraca wartosc 1 gdy porownanie ma wynik pozytywny.
unsigned char VerifyRSCHK(unsigned char nData)
{
	if(nData>_NRS_RBF) nData=_NRS_RBF;
	if(!nData) nData=_NRS_RBF;
	if(bufRUART[nData-1]==LiczRSCHK(bufRUART,nData-1)) return 1; 	//liczy CHK z pominieciem ostatniego bajtu (bajt przeznaczony na zapis obliczonego CHK.)
	return 0;														//i porownuje z ostatnim odebranym bajtem.
}
//------------------------------------------------------------------------------------
//Odczyt obliczonej wartosci CHK (rejestracja CHK odbywa sie za pomoca procedury SetRSCHK(nData)
/*
unsigned char GetRSCHK(void)
{
	return UADt.CHK;	
}
 */
//------------------------------------------------------------------------------------
//Oblicza CHK dla liczby danych i zapisuje na trzech najstarszych bitach
/*static inline unsigned char iSetNDatCHK(const unsigned char Data)
{
	unsigned char a,b,d;
	d=Data&0b00011111;
	a=d&0b00000111;
	b=0b00000100|(d>>3);
	return d|((a^b)<<5);
}*/
//------------------------------------------------------------------------------------
//Weryfikacja wartosci CHK dla liczby danych
static inline unsigned char iVerifyNDatCHK(const unsigned char Data)
{
    volatile unsigned char a,b,d,w;
    
	d=Data&0b00011111;
	a=d&0b00000111;
	b=0b00000100|(d>>3);
    w=d|((a^b)<<5);
	if(w==Data) return 1;                                           //liczy CHK z pominieciem ostatniego bajtu (bajt przeznaczony na zapis obliczonego CHK.)
	return 0;														//i porownuje z ostatnim odebranym bajtem.
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
    SetRSTBuf(1,chkndat);				//1 - liczba przesy�anych danych (ndat)
    if(ndt>31)
    {
        chkndat=SetNDatCHK(ndt-31);    //oblicz CHK dla liczby danych    
    }
    else chkndat=SetNDatCHK(0);			//oblicz CHK dla liczby danych
    SetRSTBuf(3,chkndat);               //1 - liczba przesy�anych danych (ndat) 
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
void InitDtaUART1(void)
{
	unsigned char rc;
    /*
	//6 wyjscie portu C ustawic jako wyjscie (0)
	//7 wyjscie portu C ustawic jako wejscie (1)

	CSRC=0;			//asynch mode
	RX9=1;			//odbior 9 bitowy
	SPBRG=_SPBRG;	//baud rate
	SYNC=_SYNC;		//tryb asynchroniczny/synchroniczny
	BRGH=_BRGH;		//predkosc generatora Baud Rate
	RC1STA.ADDEN=1;		//z adresowaniem
	TX9=1;			//nadawanie 9 bitowe
	RCIP=1;			//wysoki priorytet przerwania
	RCIE=1;			//aktywne przerwanie dla odbioru
	TXIP=1;			//wysoki priorytet przerwania
	TXEN=1;			//aktywacja modulu transmisji
	SPEN=1;			//aktywacja portu szeregowego
	RC1STA.CREN=1;			//aktywacja odbioru

	//UWAGA: GIEL=1 oraz GIEH=1
    */
    
    //UART1_SetTXIE(0);
    //EUSART1_SetTxInterruptHandler(IntUSARTTR);
    //UART1_SetRCIE(0);
    //EUSART1_SetRxInterruptHandler(IntUSARTRC); 
    /*
     SPBRG=64000000/2400/64-1=0x19f
     SPBRG=64000000/2400/16-1=0x681
     
     */
	UADt.RData=0;
	UADt.RErrREC=0;
	UADt.RErrBUF=0;
	
	UADt.TData=0;
	UADt.RNData=0;
	UADt.TNData=0;
    
    UADt.REXT=0;
	UADt.REnd=0;
    UADt.TEXT=0;
	UADt.TEnd=0;
	UADt.indeks=0;
	//rc=UART1_GetRxData();
    UART1_SetRCIE_Ex(1);
    UART1_SetTXIE_Ex(0);
    rc=RCREG;
}
//------------------------------------------------------------------------------------
#endif