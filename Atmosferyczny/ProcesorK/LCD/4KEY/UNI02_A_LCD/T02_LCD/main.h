/*main.h*/
#ifndef	_MAIN_H
#define	_MAIN_H
#include "main.tp.h"		//typy danych
#include "wersja.h"
#include "keypad.tp.h"			//parametry lokalne
//--------------------------------------------------------------
//CONFIG
#ifndef _PLL
#error Nieokreslony parametr _PLL
#endif


//__CONFIG(1,XINSTDIS&DEBUGDIS&WDTDIS&STVREN&WDTEN&CPA);
//__CONFIG(2,IESODIS&FCMEN&INTOSC&HS&WDTPS128);//&HSPLL
//__CONFIG(3,CCP2RC1);

#ifdef __DEBUG                      //TRYB DEBAGOWANIA
#define _DEBUGT 1
#else
#define _DEBUGT 0
#endif

#if defined(_18F63J90) || defined(_18F65J90)
#if _DEBUGT==1                      //TRYB DEBAGOWANIA
    // CONFIG1L
    #pragma config WDTEN = OFF      // Watchdog Timer Enable bit (WDT disabled (control is placed on SWDTEN bit))
    #pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable bit (Reset on stack overflow/underflow enabled)
    #pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

    // CONFIG1H
    #pragma config CP0 = OFF        // Code Protection bit (Program memory is not code-protected)

    // CONFIG2L
    #pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
    #pragma config FOSC2 = OFF      // Default/Reset System Clock Select bit (INTRC enabled as system clock when OSCCON<1:0> = 00)
    #pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor enabled)
    #pragma config IESO = OFF       // Two-Speed Start-up (Internal/External Oscillator Switchover) Control bit (Two-Speed Start-up disabled)

    // CONFIG2H
    #pragma config WDTPS = 128      // Watchdog Timer Postscaler Select bits (1:128)

    // CONFIG3L

    // CONFIG3H
    #pragma config CCP2MX = DEFAULT // CCP2 MUX bit (CCP2 is multiplexed with RC1)

#else                               //TRYB PRODUKCYJNY
    // CONFIG1L
    #pragma config WDTEN = ON       // Watchdog Timer Enable bit (WDT enabled)
    #pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable bit (Reset on stack overflow/underflow enabled)
    #pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

    // CONFIG1H
    #pragma config CP0 = ON         // Code Protection bit (Program memory is code-protected)

    // CONFIG2L
    #pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
    #pragma config FOSC2 = OFF      // Default/Reset System Clock Select bit (INTRC enabled as system clock when OSCCON<1:0> = 00)
    #pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor enabled)
    #pragma config IESO = OFF       // Two-Speed Start-up (Internal/External Oscillator Switchover) Control bit (Two-Speed Start-up disabled)

    // CONFIG2H
    #pragma config WDTPS = 128      // Watchdog Timer Postscaler Select bits (1:128)

    // CONFIG3L

    // CONFIG3H
    #pragma config CCP2MX = DEFAULT // CCP2 MUX bit (CCP2 is multiplexed with RC1)
#endif
#else
#if defined(_18F66J90) || defined(_18F67J90)
#if _DEBUGT==1                      //TRYB DEBAGOWANIA
    // CONFIG1L
    #pragma config WDTEN = OFF      // Watchdog Timer Enable bit (WDT disabled (control is placed on SWDTEN bit))
    #pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable bit (Reset on stack overflow/underflow enabled)
    #pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

    // CONFIG1H
    #pragma config CP0 = OFF        // Code Protection bit (Program memory is not code-protected)

    // CONFIG2L
    #pragma config OSC = INTOSCPLL  // Oscillator Selection bits (Internal oscillator with PLL enabled, port function on RA6 and RA7 )
    #pragma config T1DIG = ON       // Secondary Clock Source T1OSCEN Enforcement (Secondary Oscillator clock source may be selected)
    #pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator Enable bit (Timer1 oscillator configured for higher power operation)
    #pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor enabled)
    #pragma config IESO = OFF       // Two-Speed Start-up (Internal/External Oscillator Switchover) Control bit (Two-Speed Start-up disabled)

    // CONFIG2H
    #pragma config WDTPS = 128      // Watchdog Timer Postscaler Select bits (1:128)

    // CONFIG3L
    #pragma config RTCSOSC = INTOSCREF// RTCC Reference Clock Select bit (RTCC uses INTOSC/INTRC as reference clock)

    // CONFIG3H
    #pragma config CCP2MX = DEFAULT // CCP2 MUX bit (CCP2 is multiplexed with RC1)

#else                               //TRYB PRODUKCYJNY
    // CONFIG1L
    #pragma config WDTEN = ON       // Watchdog Timer Enable bit (WDT enabled)
    #pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable bit (Reset on stack overflow/underflow enabled)
    #pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

    // CONFIG1H
    #pragma config CP0 = ON         // Code Protection bit (Program memory is code-protected)

    // CONFIG2L
    #pragma config OSC = INTOSCPLL  // Oscillator Selection bits (Internal oscillator with PLL enabled, port function on RA6 and RA7 )
    #pragma config T1DIG = ON       // Secondary Clock Source T1OSCEN Enforcement (Secondary Oscillator clock source may be selected)
    #pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator Enable bit (Timer1 oscillator configured for higher power operation)
    #pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor enabled)
    #pragma config IESO = OFF       // Two-Speed Start-up (Internal/External Oscillator Switchover) Control bit (Two-Speed Start-up disabled)

    // CONFIG2H
    #pragma config WDTPS = 128      // Watchdog Timer Postscaler Select bits (1:128)

    // CONFIG3L
    #pragma config RTCSOSC = INTOSCREF// RTCC Reference Clock Select bit (RTCC uses INTOSC/INTRC as reference clock)
    // CONFIG3H
    #pragma config CCP2MX = DEFAULT // CCP2 MUX bit (CCP2 is multiplexed with RC1)

#endif
#endif
#endif
//----------------------------------------------------------
#define _TFOT 100           //[dSek] czas co ktory odswierzany jest znacznik komunikacji OT dla UNI-02
#define _TSTRNST 30			//[ds] zmiana kierunku obrotow pompy
//----------------------------------------------------------
//----------------------------------------------------------
//----------------------------------------------------------

//funkcje lokalne
	void InitGlobal(void);
	void InitUNIData(void);
	void RefData(void);
        void DecRes(void);
	unsigned char PrintVerBoards(unsigned char ref);
    void InitVerExposition(void);
//osc.c
	extern void InitOsc(void);
//ac.c
	extern void InitAC(void);
//wsw.c
	extern void InitWsw(void);
	extern bit GetPoz(const unsigned char nrkol,const unsigned char nrpoz);
//pwm2.c
/*	extern void InitPWM(void);
	extern void WritePWM(const unsigned char tpwm);
	extern void WriteOPWM(unsigned char tpwm);
 */
	extern void InitPWM2(void);
 
//print.c
	extern void ClsScr(void);
	extern void EndClsScr(void);
	extern void TestScr(void);
	extern void EndTestScr(void);
	extern void ClsScr(void);
	extern void EndClsScr(void);
	extern void PrintErr(const unsigned char lkod,unsigned char nkod, const unsigned char pls, const unsigned char res);//void PrintErr(const unsigned char lkod,const unsigned char nkod, const unsigned char pls);
	extern void PrintErrCs(const unsigned char lkod,const unsigned char nkod,const unsigned char cs);
	extern void PrintErrTm(const unsigned char lkod,const unsigned char nkod,const unsigned char tm, const unsigned char pls);
	extern void EndErr(void);
	extern void PrintErrMsg(const unsigned char id, const unsigned char lkod,unsigned char nkod,unsigned char tm,unsigned char cs, unsigned char res);//void PrintErrMsg(const unsigned char id, const unsigned char lkod,unsigned char nkod,unsigned char tm,unsigned char cs);//void PrintErrMsg(const unsigned char lkod,unsigned char nkod,unsigned char tm,unsigned char cs);
	extern void PrintPR(const unsigned char lkod,unsigned char nkod,unsigned char tryb);
	extern void EndPR(void);
	extern void PrintWPR(const unsigned int wr);
	extern void PrintNstCO(const unsigned char nst,const unsigned char kt,const unsigned char only);//void PrintNstCO(const unsigned char nst,const unsigned char only);
	extern void PrintNstCW(const unsigned char nst,const unsigned char only);
	extern void PrintNstKt(const unsigned char nst,const unsigned char only);
	extern void EndNst(void);
	extern void PrintL3(void);
	extern void EndL3(void);
	extern void PrintCS(const unsigned char cs,const unsigned char only);
	extern void PrintCW(const unsigned char tm,const unsigned char only);
	extern void PrintCO(const unsigned char tm,const unsigned char only, const unsigned char l3);//void PrintCO(const unsigned char tm,const unsigned char only);
	extern void PrintHex(const unsigned char tm);
	extern void SetDiode(unsigned char nrd, unsigned char stn);
	extern void RefreshNst(const unsigned char nst);
	extern void MarkSVAL(unsigned char s,unsigned char mx,unsigned al);//void MarkFSRV(unsigned char s,unsigned char mx);
	extern void ClrMarkMinMax(void);
	extern void Mark(unsigned char z,unsigned char s);
	extern void ClrDig(const unsigned char dhigh, const unsigned char dlow, const unsigned char krs);
        extern void RestartKol2(const unsigned char nrkol);
        extern void RestartKol1(const unsigned char nrkol);
        extern void RestartKol(const unsigned char nrkol);
        extern void PrintGrafH(const unsigned char lkod2,const unsigned char lkod1);
        extern void PrintVNT(const unsigned char vn,const unsigned char only);
        extern void PrintIN(const unsigned char tm);
        extern void PrintPP(unsigned char pp);
        extern void PrintPG(unsigned char pg);
        extern void PrintCo(const unsigned char tm);        
        extern void PrintNstECO(const unsigned char nst);
        extern void PrintVER(const unsigned char board, const signed char ver,const unsigned char only);
        extern void PrintNstCW2(const unsigned char nst,const unsigned char min, const unsigned char only);
        extern void PrintLN(const unsigned char laddr);
        extern void PrintPo(const unsigned char cs);
//bit.c
	extern bit GetBit(const unsigned char data,const unsigned char nbit);
	extern bit GetBit2(const unsigned char data,const unsigned char nbit);
	extern void BitSet(unsigned char *data, const unsigned char nbit,const unsigned char bt);
	extern void BitSet2(unsigned char *data, const unsigned char nbit,const unsigned char bt);
	extern void BitSet3(volatile unsigned char *data, const unsigned char nbit,const unsigned char bt);
//rtimer.c
	extern void StartRTG(const unsigned char Index);
	extern unsigned char RTG(const unsigned char Index);
	extern void StartRTS(const unsigned char Index);
	extern unsigned char RTS(const unsigned char Index);
	extern void StartRTdS(const unsigned char Index);
	extern unsigned char RTdS(const unsigned char Index);
//timer1.c
	extern void InitTimer1(void);
//timer3.c
	extern void InitTimer3(void);
//port.c
	extern void InitPort(void);
	extern bit RdPrt(const unsigned char Index);
	extern void SetPrt(const unsigned char Index, const unsigned char nw);
	extern void RefPrt(const unsigned char Index);
//pwron.c
	extern void PwrOnDelay(void);
//i2c_m.c
	extern void InitI2C(void);
	extern void StartI2CTim(void);
	extern bit I2CTim(void);
	extern bit ReceptI2C(unsigned char NBTI2C);
	extern bit TransmitI2C(unsigned char NBTI2C);
//imy.c
	extern void InitIMY(void);
	extern void KomunikacjaIMY(void);
//imz.c
	extern void InitIMZ(void);
	extern void KomunikacjaIMZ(void);
//auart9b.c
	extern void InitAUart2(void); 
//uart.c
	extern void InitUart(void);
	extern void StartUSARTTR(const unsigned char nData);
	extern void SetRSTBuf(const unsigned char indeks,const unsigned char data);
 //mgs_rs9b.c
	extern void RingUSART(void);
	extern void TrToAMMY1(const unsigned char npow);
	extern void TransData(const unsigned char tnDat,const unsigned char rnDat);
	extern void InitTransProc(void);
//konfig.c
	extern void InitKonf(void);
	extern void MKonfiguracja(void);
	extern void RdEEParam(void);
//keypad.c
	extern void SetBufKey(volatile StPin *ky);
	extern void ResBufKey(volatile StPin *ky);
	extern unsigned char PresKey(unsigned char k,volatile StPin *ky);
	extern unsigned char ExPresKey(unsigned char k,unsigned char dTim,unsigned char pTim);
	extern unsigned char ExPresKey2(unsigned char k,unsigned char dTim,unsigned char pTim);
	extern unsigned char ReadDataXONL(unsigned char k,unsigned char *dt,unsigned char min,unsigned char max,const unsigned char step,const unsigned char step2,unsigned char wrn);
        extern unsigned char PresKeyEX(unsigned char k,volatile StPin *ky);
        extern void ResBufKeyEx(volatile StPin *ky);
        extern void WaitToLowEx(volatile StPin *ky);
//konsola.c
	extern void ReadParam(unsigned int *param,unsigned char *zmiana,unsigned char *zapis,unsigned int min, unsigned int max, unsigned char rckl, unsigned char fst);
    extern void ReadParamCW(unsigned int *param,unsigned char *zmiana,unsigned char *zapis,unsigned int min, unsigned int max, unsigned char rckl, unsigned char fst);
    extern void ReadParamCO(unsigned int *param,unsigned char *zmiana,unsigned char *zapis,unsigned int min, unsigned int max, unsigned char rckl, unsigned char fst);    
        //extern void ReadParam(volatile unsigned int *param,unsigned char *zmiana,unsigned char *zapis,unsigned int min, unsigned int max, unsigned char rckl, unsigned char fst);
	extern void TrybPracy(void);
//kolejka.c
	extern void RingProc(void);
//lled.c
	extern void InitLLED(void);
	extern void SetLightLLed(unsigned char nowhigh);
        extern void SetPulseLLed(void);
//LIN1Slave.c
        extern void InitUart1(void);

        extern unsigned char SendLINMessageToUNI02(unsigned char msg);
//pomiar.c        
        extern void InitDataPmr(void);
        extern void Pomiar10V(void);
//ot.c
        extern void InitOT(void);
        extern void OTInterpreter(void);
//----------------------------------------------------------
#endif		/*_MAIN_H*/
