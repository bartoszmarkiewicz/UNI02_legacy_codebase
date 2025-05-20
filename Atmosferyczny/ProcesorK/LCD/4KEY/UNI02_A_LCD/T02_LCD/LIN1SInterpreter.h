/*LIN1SInterpreter.h*/
//----------------------------------------------------------
#ifndef	_LIN1SINT_H
#define	_LIN1SINT_H
#include "LIN1Slave.h"


#define _TFLIN 100               //[dSek] czas co ktory odswierzany jest znacznik komunikacji LIN dla UNI-02

#define _LIN_READ 0             //znacznik odczytu z odbiornika
#define _LIN_WRITE 1            //znacznik zapisu do odbiornika

#define _OLD_RS_KOM 0           //rodzaj komend RS (0-stara notacja, 1-nowa notacja)
//----------------------------------------------------------
//Identyfikatory (IDN) ramki danych dla komunikacji LCD <-> KSK

//----------------ODCZYT DANYCH PRZEZ KSK
#define _CLIN_RRD_ACK       0x01  //IDN - wysylka potwierdzenia i identyfikatora urzadzenia w odpowiedzi na wywolanie adresem (np. do kontroli obecnosci odbiornika)

#define _CLIN_RRD_STD       0x02  //IDN - zadanie wysylki paczki standard do KSK
#define _NDT_RRD_STD 21     //16Dane tablicowe

#define _CLIN_RRD_STD2      0x03 //IDN - zadanie wysylki paczki standard do KSK
#define _NDT_RRD_STD2 22     //22Dane tablicowe

#define _CLIN_RRD_KNF       0x04  //IDN - zadanie wysylki paczki konfig do KSK
#define _NDT_RRD_KNF 21     //Dane tablicowe

#define _CLIN_RRD_PCW       0x05    //IDN - odczyt nastawy PCW, dozwolonego minimum i maksimum
#define _CLIN_RRD_PCO       0x06    //IDN - odczyt nastawy PCO, dozwolonego minimum i maksimum
#define _CLIN_RRD_TRB       0x08    //IDN - odczyt aktualnego trybu pracy, dozwolonego minimum i maksimum
#define _CLIN_RRD_RES       0x09    //IDN - odczyt stanu przycisku reset
#define _CLIN_RRD_BRES      0x0A    //IDN - odczyt stanu blokady awaryjnej
#define _CLIN_RRD_FSR       0x0B    //IDN - odczyt aktualnego stanu funkcji serwisowej
#define _CLIN_RRD_ANL       0x0C    //IDN - odczyt aktualnego stanu funkcji antylegionella
#define _CLIN_RRD_NKNF      0x0D    //IDN - odczyt liczby parametrow konfiguracyjnych
#define _CLIN_RRD_PKNF      0x0E    //IDN - odczyt parametru konfiguracji
#define _CLIN_RRD_RT        0x0F    //IDN - odczyt pobudzenie RT
#define _CLIN_RRD_CW        0x10    //IDN - odczyt pobudzenie CW

#define _CLIN_RRD_OT        0x11    //IDN - odczyt danych regulatora OT
#define _NDT_RRD_OT         9       //Dane tablicowe

#define _CLIN_RRD_10V       0x12    //IDN - odczyt danych interfejsu 10V
#define _CLIN_RRD_SETP      0x13    //IDN - docelowa wartosc modulacji przy grzaniu obiegu WG
#define _CLIN_RRD_ZW        0x14    //IDN - temperatura z czujnika pogodowego
#define _CLIN_RRD_INT       0x15    //IDN - stan interfejsow komunikacyjnych
#define _CLIN_RRD_PRD       0x16    //IDN - specjalny tryb pracy
#define _CLIN_RRD_EEST      0x17    //IDN - status kotla przy wylaczeniu awaryjnym z blokada
#define _NDT_RRD_EEST       22       //Dane tablicowe
#define _CLIN_RRD_HIST      0x18    //IDN - odczyt bufora historii zdarzen awaryjnych
                                    //Dane tablicowe (ze zmiennym rozmiarem)
#define _CLIN_RRD_STH       0x19    //IDN - statystyki zdarzen awaryjnych
//----------------ZAPIS DANYCH PRZEZ KSK
#define _CLIN_WRD_LADDR     0x01  //IDN - zadanie modyfikacji adresu LIN pytki UNI-02 (parametr1=nowy adres urzadzenia)
#define _CLIN_WRD_LIFADDR   0x02  //IDN - zadanie modyfikacji adresu LIN pytki UNI-02 po spelnieniu warunku (parametr1=nowy adres urzadzenia)
#define _CLIN_WRD_SHOWADDR  0x03  //IDN - zadanie pokazania adresu wlasnego LIN, autowygaszenie po 3 sek (wywolanie bez parametru)
#define _CLIN_WRD_PULSE     0x04  //IDN - wlacz/wylacz pulsacje podswietlenia (parametr1: 0-wylacza pulsacje podswietlenia, 1-pulsacja z autodeaktywacja, 2-pulsacja ciagla)
#define _CLIN_WRD_PCW       0x05    //IDN - modyfikacja nastawy PCW, nowa wartosc w parametr1
#define _CLIN_WRD_PCO       0x06    //IDN - modyfikacja nastawy PCO, nowa wartosc w parametr1
#define _CLIN_WRD_TRB       0x08    //IDN - modyfikacja nastawy ECO, nowa wartosc w parametr1
#define _CLIN_WRD_RES       0x09    //IDN - chwilowa aktywacja przycisku reset
#define _CLIN_WRD_BRES      0x0A    //IDN - zniesienie blokady awaryjnej
#define _CLIN_WRD_FSR       0x0B    //IDN - aktywacja/deaktywacja funkcji serwisowej (par1=0 -deaktywacja,par1=1 - aktywacja na mocy maksymalnej, par1=2 - aktywacja na mocy minimalnej)
#define _CLIN_WRD_ANL       0x0C    //IDN - aktywacja/deaktywacja funkcji antylegionella
#define _CLIN_WRD_NKNF      0x0D    //IDN - nieaktywny
#define _CLIN_WRD_PKNF      0x0E    //IDN - zapis parametru konfiguracji
#define _CLIN_WRD_RT        0x0F    //IDN - zapis pobudzenie RT
#define _CLIN_WRD_CW        0x10    //IDN - zapis pobudzenie CW
#define _CLIN_WRD_SETP      0x12    //IDN - docelowa wartosc modulacji przy grzaniu obiegu WG
#define _CLIN_WRD_PRD       0x13    //IDN - specjalny tryb pracy
#define _CLIN_WRD_HFLG      0x14    //IDN - zerowanie licznika flagi serwisowej historii zdarzen awaryjnych
//Dane tablicowe

//----------------------------------------------------------

//------------------------------------
unsigned char LINWaitForAnswerUNI02(const unsigned char adr);
unsigned char SelectResponseType(void);
unsigned char RSFInterpreter_SingleRD(void);
unsigned char RSFInterpreter_MultiRD(void);
unsigned char RSFInterpreter_SingleWR(void);
unsigned char RSFInterpreter_MultiWR(void);
extern bit LIN1SDeamon(void);
extern void StartRTdS(const unsigned char Index);
extern void BitSet2(unsigned char *data, const unsigned char nbit,const unsigned char bt);
extern void BitSet3(volatile unsigned char *data, const unsigned char nbit,const unsigned char bt);
extern bit RdPrt(const unsigned char Index);
extern void SetPrt(const unsigned char Index, const unsigned char nw);
extern unsigned char SetKNPAR(void);
extern unsigned char GetKNPAR(void);
//----------------------------------------------------------
#endif		/*_LIN1SINT_H*/
