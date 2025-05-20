/*ot.h*/
#ifndef	_OT_H
#define	_OT_H
#include "ot.tp.h"

//Regulator Honwywell
//Odleglosc czasowa miedzy pierwszym zboczem kolejnych ramek zapytania: 996ms
//Rozmiar czasowy ramki zapytania:33.2ms
//Rozmiar czasowy bitu: 500us

//Regulator oczekuje na odpowiedz min 20sek. Potem prawdopodobnie przechodzi w tryb OT-
//Gdy nastapi nawiazanie polaczenia to przy braku odpowiedzi proby dalej kontynuowane sa jeszcze przez minute

//Manchester kode: zbocze narastajace = 0, zbocze opadajace = 1

//Ramka sklada sie z 4 bajtow, brak chk, jest bit parity

//Slave odpowiada po czasie od 20ms do 400ms (normalnie przyjety <100ms)

//Master(regulator RT) nawiazuje kolejne polaczenie po czasie nie mniejszym niz 100ms

//Master musi wysylac kolejne ramki nie zadziej niz co 1.15s

//Jezeli kociol nie odpowie w przeciagu 20 sek. to regulator przestraja sie w tryb OT- (wypelnienie sygnalu)

//Po przejsciu w OT+ jezeli kociol nie odpowie w przeciagu 1min to sygnalizowany jest blad komunikacji jednak regulator pozostaje w trybie OT+


#define _NEG_IN 1   //wejscie OT odwracajace

#if _NEG_IN==0
#define _HIGH_OT 1  //poziom logiczny na wejsciu procesora odpowiadajacy stanowi wysokiemu wejscia OT
#define _LOW_OT 0   //poziom logiczny na wejsciu procesora odpowiadajacy stanowi niskiemu wejscia OT
#else
#define _HIGH_OT 0  //poziom logiczny na wejsciu procesora odpowiadajacy stanowi wysokiemu wejscia OT
#define _LOW_OT 1   //poziom logiczny na wejsciu procesora odpowiadajacy stanowi niskiemu wejscia OT
#endif
#define _SAMPLE_SIZE 10              //Manchester period sample size [number of samples]
#define _TICK_TO_USEC 4              //liczba mikrosekund przypadajaca na zmiane stany timera0

#define _TR_PRELOAD  0xFF84           //0xFC13////Wartosc przeladowania dla 0.5msek dla timer0
//----------------------------------------------------------
//Protocol specific def
#define _FRAME_LENGTH 34             //OT protocol frame length [bits]
#define _MIN_WAIT_FOR_SLAVE 25       //(20ms + 20%)minimum time between master and slave frame [ms]
#define _MIN_WAIT_FOR_MASTER 85      //85(100ms - 15%) minimum time between slave and (next) master frame [ms]
//----------------------------------------------------------
//**************************
#define _MEASURE_TIMO   25000          //25000[msek] maksymalny czas do restartu procedury MeasureOTPeriod
#define _WAITOT_TIMO    10000          //10000[msek] maksymalny czas do ponownego uaktywnienia procedury MeasureOTPeriod po oczekiwaniu w procedurze WaitOTFrame
#define _FEDG_TIMO      20000           //5000[msek] maksymalny czas do ponownego uaktywnienia procedury WaitOTFrame po oczekiwaniu na zbocze inicjujace odczyt ramki
//*************************
//#define _READOT_TIMO    5000           //[msek] maksymalny czas do ponownego uaktywnienia procedury WaitOTFrame po oczekiwaniu na zbocze inicjujace odczyt ramki
#define _WAITTT_TIMO    400            //[msek] maksymalny czas do ponownego uaktywnienia procedury MeasureOTPeriod po oczekiwaniu w procedurze WaitOTFrame
//----------------------------------------------------------
#define _OT_WAIT 0
#define _OT_TO_RESP 1
#define _OT_NO_RESP 2

void StopOTTimeout(void);
void DetachINT(void);
void StartMeasureOTPeriod(void);
void IntMeasureOTPeriod(void);
void SetOTDeamon(const unsigned char tk);
unsigned char ReadOTFrame(void);
void IntOTDeamon(void);
void StartReadOTFrame(void);
void SetTFrameParityOdd(void);

extern void InitTimer0(void);
//extern void WrtTMR0(volatile unsigned int tm);
extern void ReloadTMR0(volatile unsigned int tm);
extern unsigned int ReadTMR0(void);
extern void SetIntTimer0Type(const unsigned char tp);

//----------------------------------------------------------
#endif		/*_OT_H*/
