/*LIN1SInterpreter.h*/
//----------------------------------------------------------
#ifndef	_LIN1SINT_H
#define	_LIN1SINT_H
#include "LIN1Slave.h"


#define _TFLIN 100               //[dSek] czas co ktory odswierzany jest znacznik komunikacji LIN dla UNI-02
#define _TPLIN 30               //[Sek] czas po ktorym uniewaznione zostana zdalne parametry pozyskane interfejsem LIN2 (PAR2)

#define _LIN_READ 0             //znacznik odczytu z odbiornika
#define _LIN_WRITE 1            //znacznik zapisu do odbiornika

#define _OLD_RS_KOM 0           //rodzaj komend RS (0-stara notacja, 1-nowa notacja)
//----------------------------------------------------------
//Identyfikatory (IDN) ramki danych dla komunikacji LCD <-> KSK

//----------------ODCZYT DANYCH PRZEZ KSK
#define _CLIN_RRD_ACK       0x01  //IDN - wysylka potwierdzenia i identyfikatora urzadzenia w odpowiedzi na wywolanie adresem (np. do kontroli obecnosci odbiornika)

#define _CLIN_RRD_STD       0x02  //IDN - zadanie wysylki paczki standard do KSK
#define _NDT_RRD_STD 24     //16Dane tablicowe

#define _CLIN_RRD_STD2      0x03 //IDN - zadanie wysylki paczki standard do KSK
#define _NDT_RRD_STD2 24     //22Dane tablicowe

#define _CLIN_RRD_KNF       0x04  //IDN - zadanie wysylki paczki konfig do KSK
#define _NDT_RRD_KNF 31     //Dane tablicowe

#define _CLIN_RRD_PCW       0x05    //IDN - odczyt nastawy PCW, dozwolonego minimum i maksimum
#define _CLIN_RRD_PCO       0x06    //IDN - odczyt nastawy PCO, dozwolonego minimum i maksimum
#define _CLIN_RRD_ECO       0x07    //IDN - odczyt nastawy ECO, dozwolonego minimum i maksimum
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
#define _NDT_RRD_EEST       23       //Dane tablicowe
#define _CLIN_RRD_HIST      0x18    //IDN - odczyt bufora historii zdarzen awaryjnych
                                    //Dane tablicowe (ze zmiennym rozmiarem)
#define _CLIN_RRD_STH       0x19    //IDN - statystyki zdarzen awaryjnych
#define _CLIN_RRD_P         0x1A    //IDN - odczyt parametru P
#define _CLIN_RRD_I         0x1B    //IDN - odczyt parametru I
#define _CLIN_RRD_D         0x1C    //IDN - odczyt parametru D
#define _CLIN_RRD_RPV       0x1D    //IDN - odczyt wartosc dolnej granicy wentylatora
#define _CLIN_RRD_RCO       0x1E    //IDN - odczyt wspolczynniki korekty modulacji CO przy wyborze typu 2 wentylatora (domyslnie wartosc 1)
#define _CLIN_RRD_RCW       0x1F    //IDN - odczyt wspolczynniki korekty modulacji CW przy wyborze typu 2 wentylatora (domyslnie wartosc 1)
#define _CLIN_RRD_RGZ       0x20    //IDN - odczyt godzin pracy
#define _CLIN_RRD_SETPP     0x40    //IDN - informacja zwrotna pobudzen i wartosci oczekiwanych (na potrzeby BYPASS)  //jjj

#define _CLIN_RRD_KPAR0     0x42    //IDN - parametr konfiguracyjny modulu //kkk
#define _CLIN_RRD_KPAR1     0x43    //IDN - parametr konfiguracyjny modulu //kkk
#define _CLIN_RRD_KPAR2     0x44    //IDN - parametr konfiguracyjny modulu //kkk
#define _CLIN_RRD_KPAR3     0x45    //IDN - parametr konfiguracyjny modulu //kkk

#define _CLIN_RRD_LIFADDR   0x46    //IDN - parametr konfiguracyjny modulu //nnn

//----------------ZAPIS DANYCH PRZEZ KSK
#define _CLIN_WRD_LADDR     0x01  //IDN - zadanie modyfikacji adresu LIN pytki UNI-02 (parametr1=nowy adres urzadzenia)
#define _CLIN_WRD_LIFADDR   0x02  //IDN - zadanie modyfikacji adresu LIN pytki UNI-02 po spelnieniu warunku (parametr1=nowy adres urzadzenia)
#define _CLIN_WRD_SHOWADDR  0x03  //IDN - zadanie pokazania adresu wlasnego LIN, autowygaszenie po 3 sek (wywolanie bez parametru)
#define _CLIN_WRD_PULSE     0x04  //IDN - wlacz/wylacz pulsacje podswietlenia (parametr1: 0-wylacza pulsacje podswietlenia, 1-pulsacja z autodeaktywacja, 2-pulsacja ciagla)
#define _CLIN_WRD_PCW       0x05    //IDN - modyfikacja nastawy PCW, nowa wartosc w parametr1
#define _CLIN_WRD_PCO       0x06    //IDN - modyfikacja nastawy PCO, nowa wartosc w parametr1
#define _CLIN_WRD_ECO       0x07    //IDN - modyfikacja nastawy ECO, nowa wartosc w parametr1
#define _CLIN_WRD_TRB       0x08    //IDN - modyfikacja nastawy ECO, nowa wartosc w parametr1
#define _CLIN_WRD_RES       0x09    //IDN - chwilowa aktywacja przycisku reset
#define _CLIN_WRD_BRES      0x0A    //IDN - zniesienie blokady awaryjnej
#define _CLIN_WRD_FSR       0x0B    //IDN - aktywacja/deaktywacja funkcji serwisowej (par1=0 -deaktywacja,par1=1 - aktywacja na mocy maksymalnej, par1=2 - aktywacja na mocy minimalnej)
#define _CLIN_WRD_ANL       0x0C    //IDN - aktywacja/deaktywacja funkcji antylegionella
#define _CLIN_WRD_NKNF      0x0D    //IDN - nieaktywny
#define _CLIN_WRD_PKNF      0x0E    //IDN - zapis parametru konfiguracji
#define _CLIN_WRD_RT        0x0F    //IDN - zapis pobudzenie RT
#define _CLIN_WRD_CW        0x10    //IDN - zapis pobudzenie CW
#define _CLIN_WRD_ENDO      0x11    //IDN - deaktywacja procedury odpowietrzajacej
#define _CLIN_WRD_SETP      0x12    //IDN - docelowa wartosc modulacji przy grzaniu obiegu WG
#define _CLIN_WRD_PRD       0x13    //IDN - specjalny tryb pracy
#define _CLIN_WRD_HFLG      0x14    //IDN - zerowanie licznika flagi serwisowej historii zdarzen awaryjnych
#define _CLIN_WRD_P         0x15    //IDN - zapis parametru P
#define _CLIN_WRD_I         0x16    //IDN - zapis parametru I
#define _CLIN_WRD_D         0x17    //IDN - zapis parametru D
#define _CLIN_WRD_RPV       0x18    //IDN - zapis wartosc dolnej granicy wentylatora
#define _CLIN_WRD_RCO1      0x19    //IDN - zapis korekta punktu rownowagi w fazie dochodzenia (FAZA1)
#define _CLIN_WRD_RCO2      0x1A    //IDN - zapis regulacja w punkcie rownowagi w fazie stabilizacji (FAZA2)
#define _CLIN_WRD_RCO3      0x1B    //IDN - zapis korekta punktu rownowagi w fazie stabilizacji (FAZA2)
#define _CLIN_WRD_RCO4      0x1C    //IDN - zapis reakcja superwizyjna
#define _CLIN_WRD_RCW1      0x1D    //IDN - zapis korekta punktu rownowagi w fazie dochodzenia (FAZA1)
#define _CLIN_WRD_RCW2      0x1E    //IDN - zapis regulacja w punkcie rownowagi w fazie stabilizacji (FAZA2)
#define _CLIN_WRD_RCW3      0x1F    //IDN - zapis korekta punktu rownowagi w fazie stabilizacji (FAZA2)
#define _CLIN_WRD_RCW4      0x20    //IDN - zapis reakcja superwizyjna
#define _CLIN_WRD_RGZ       0x21    //IDN - zapis godzin pracy (zerowanie)

//Dane tablicowe

//----------------------------------------------------------    //kkk
//------------------------------
//OUT0
#define _MOD_OUT0_STATUS    0           //status konfiguracji
#define _MOD_OUT0_NRMOD     1           //nr modulu
#define _MOD_OUT0_NRPAR     2           //nr parametru
#define _MOD_OUT0_COUNT     3           //licznik modulow
#define _MOD_OUT0_DTA0      4           //dta 0
//IN0
#define _MOD_IN0_NRMOD      0           //nr modulu
#define _MOD_IN0_NRPAR      1           //nr parametru
#define _MOD_IN0_NPAR       2           //liczba parametrow
#define _MOD_IN0_FORMAT     3           //format danej
//------------------------------
//OUT1
#define _MOD_OUT1_STATUS    5           //status konfiguracji
#define _MOD_OUT1_NRMOD     6           //nr modulu
#define _MOD_OUT1_NRPAR     7           //nr parametru
#define _MOD_OUT1_DTA0      8            //wartosc parametru (LO)
#define _MOD_OUT1_DTA1      9            //wartosc parametru (HI)
//IN1
#define _MOD_IN1_NRMOD      4           //nr modulu
#define _MOD_IN1_NRPAR      5           //nr parametru
#define _MOD_IN1_DTA0       6           //wartosc parametru (LO)
#define _MOD_IN1_DTA1       7           //wartosc parametru (HI)
//------------------------------
//OUT2
#define _MOD_OUT2_STATUS    10           //status konfiguracji
#define _MOD_OUT2_NRMOD     11           //nr modulu
#define _MOD_OUT2_NRPAR     12           //nr parametru

//IN2
#define _MOD_IN2_NRMOD      8            //nr modulu
#define _MOD_IN2_NRPAR      9            //nr parametru
#define _MOD_IN2_DTA0       10           //granica minimalna parametru (LO)
#define _MOD_IN2_DTA1       11           //granica minimalna parametru (HI)
//------------------------------
//OUT3
#define _MOD_OUT3_STATUS    13           //status konfiguracji
#define _MOD_OUT3_NRMOD     14           //nr modulu
#define _MOD_OUT3_NRPAR     15           //nr parametru

//IN3
#define _MOD_IN3_NRMOD      12           //nr modulu
#define _MOD_IN3_NRPAR      13           //nr parametru
#define _MOD_IN3_DTA0       14           //granica maksymalna parametru (LO)
#define _MOD_IN3_DTA1       15           //granica maksymalna parametru (HI)
//------------------------------
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

extern void LIN2GetKnfPar(unsigned char nmods, unsigned char npar); //kkk
extern unsigned char LIN2GetModKnfDtaFlg(void);    //kkk
//----------------------------------------------------------
#endif		/*_LIN1SINT_H*/
