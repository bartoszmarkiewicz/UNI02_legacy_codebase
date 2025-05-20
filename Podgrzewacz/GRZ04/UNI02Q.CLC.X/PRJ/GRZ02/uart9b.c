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

#if _TYP_PRJ==3
#advisory _TYP_PRJ==3 
#include "../../mcc_generated_files\mcc.h"

#include "global.h"
#include "main.h"
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//-------------------------------------------------------------------------------
//---------------------------UWAGA-----------------------------------------------
//PONIZSZE NALEZY ZAKTUALIZOWAC PO ZMIANIE RODZAJU MIKROKONTROLERA
inline void UART1_SetRCIE_Ex(bool dt)
{
    PIE3bits.RC1IE=dt;
}
inline void UART1_SetTXIE_Ex(bool dt)
{
    PIE3bits.TX1IE=dt;
}
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
static inline void UART1_SetRCIE(bool dt)
{
    PIE3bits.RC1IE=dt;
}
static inline void UART1_SetTXIE(bool dt)
{
    PIE3bits.TX1IE=dt;
}
static inline unsigned char UART1_GetRxData(void)
{
    return RC1REG;
}
//------------------------------------------------------------------------------------	
//Procedura odczytu odebranego bajtu danych /wykonaywana w przerwaniu generowanym przy odbiorze danych/
//W buforze zapisywane sa wszystkie odebrane bajty poczawszy od bajtu adresu.	
void inline IntUSARTRC(void)
{
    volatile unsigned char dt;
    UART1_ClearIrqRx();
    dt=UART1_GetRxData();
}
//------------------------------------------------------------------------------------
//Transmisja danych /procedura wykonywana w przerwaniu/
//Wysylane sa wszystkie bajty poczawszy od adresu odbiornika
void inline IntUSARTTR(void)
{
    UART1_ClearIrqTx();
}	
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//Dezaktywacja interfejsu RS232
void InitDtaUART1(void)
{
    UART1_SetRCIE_Ex(0);
    UART1_SetTXIE_Ex(0);
}
//------------------------------------------------------------------------------------
#endif