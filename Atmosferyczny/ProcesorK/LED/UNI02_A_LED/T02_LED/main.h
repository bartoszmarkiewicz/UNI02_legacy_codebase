/*main.h*/
#ifndef	_MAIN_H
#define	_MAIN_H
#include "main.tp.h"		//typy danych
//wersja takze dla procesorów serii K
//--------------------------------------------------------------
//--------------------------------------------------------------
//UWAGA:
//Aby przekompilowaæ program na procesor 18F4525 nale¿y w MPLAB
//Configure->Select device ustawiæ 18F4525 (dla wersji LF dodatkowo
//ustawiamy _VCC 0) i dokonac kompilacji.
//Aby przekompilowaæ program na procesor 18F46K20 nale¿y w MPLAB
//Configure->Select device ustawiæ 18F46K20 i dokonac kompilacji.
//
//--------------------------------------------------------------
//--------------------------------------------------------------
//dotyczy 18F4525 lub 18LF4525
#define _VCC 0			//wersja napiêciowa uk³adu 18F4525 (0 = 3,3V, 1 = 5V)
//--------------------------------------------------------------
//--------------------------------------------------------------
#if defined(_18F46K20)	//poprawka - procesory serii K tyko w wersji 3V
#define _VER5V	0	
#else
#define _VER5V	_VCC
#endif
//--------------------------------------------------------------
//--------------------------------------------------------------
//CONFIG
#ifndef _PLL
#error Nieokreslony parametr _PLL
#endif

#if defined(_18F46K20)
//__CONFIG(1,IESOEN&FCMEN&RCIO);
//__CONFIG(2,PWRTDIS&BOREN&BORV27&WDTEN&WDTPS128);
//__CONFIG(3,LPT1DIS&MCLRDIS&PBDIGITAL);
//__CONFIG(4,XINSTDIS&DEBUGDIS&LVPDIS&STVREN);//XINSTDIS
//__CONFIG(5,CPB&CP3&CP2&CP1&CP0);
//__CONFIG(6,WPA&WPC);
// CONFIG1H
#pragma config FOSC = INTIO67   // Oscillator Selection bits (Internal oscillator block, port function on RA6 and RA7)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor enabled)
#pragma config IESO = ON        // Internal/External Oscillator Switchover bit (Oscillator Switchover mode enabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 27        // Brown Out Reset Voltage bits (VBOR set to 2.7 V nominal)

// CONFIG2H
#pragma config WDTEN = ON       // Watchdog Timer Enable bit (WDT is always enabled. SWDTEN bit has no effect)
#pragma config WDTPS = 128      // Watchdog Timer Postscale Select bits (1:128)

// CONFIG3H
#pragma config CCP2MX = PORTC   // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config HFOFST = ON      // HFINTOSC Fast Start-up (HFINTOSC starts clocking the CPU without waiting for the oscillator to stablize.)
#pragma config MCLRE = OFF      // MCLR Pin Enable bit (RE3 input pin enabled; MCLR disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = ON         // Code Protection Block 0 (Block 0 (000800-003FFFh) code-protected)
#pragma config CP1 = ON         // Code Protection Block 1 (Block 1 (004000-007FFFh) code-protected)
#pragma config CP2 = ON         // Code Protection Block 2 (Block 2 (008000-00BFFFh) code-protected)
#pragma config CP3 = ON         // Code Protection Block 3 (Block 3 (00C000-00FFFFh) code-protected)

// CONFIG5H
#pragma config CPB = ON         // Boot Block Code Protection bit (Boot block (000000-0007FFh) code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = ON        // Write Protection Block 0 (Block 0 (000800-003FFFh) write-protected)
#pragma config WRT1 = ON        // Write Protection Block 1 (Block 1 (004000-007FFFh) write-protected)
#pragma config WRT2 = ON        // Write Protection Block 2 (Block 2 (008000-00BFFFh) write-protected)
#pragma config WRT3 = ON        // Write Protection Block 3 (Block 3 (00C000h-00FFFFh) write-protected)

// CONFIG6H
#pragma config WRTC = ON        // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) write-protected)
#pragma config WRTB = ON        // Boot Block Write Protection bit (Boot Block (000000-0007FFh) write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection Block 0 (Block 0 (000800-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection Block 1 (Block 1 (004000-007FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection Block 2 (Block 2 (008000-00BFFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection Block 3 (Block 3 (00C000-00FFFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot Block (000000-0007FFh) not protected from table reads executed in other blocks)
#else
__CONFIG(1, IESOEN&FCMEN&RCIO);
#if _VER5V==0
__CONFIG(2, BOREN&BORV28&PWRTEN&WDTEN&WDTPS128);
#else 
__CONFIG(2, BOREN&BORV43&PWRTEN&WDTEN&WDTPS128);
#endif
__CONFIG(3, LPT1DIS&MCLRDIS&PBANDIS);
__CONFIG(4, XINSTDIS&DEBUGDIS&LVPDIS&STVREN);
//__CONFIG(5,UNPROTECT);
//__CONFIG(6,WRTEN);
__CONFIG(5, CPA);
__CONFIG(6, WPA&WPC);
#endif  


//----------------------------------------------------------
#define _TSTR 3				//[s] czas do wyzerowania znacznika rozruchu sterownika
#define _TBPL0 5			//[s] czas do stwierdzenia uszkodzenia plytki zapalacza
#define _TCIRO 5			//[s] czas do uruchomienia awaryjnej procedury ciagu w trakcie rozruchu
#define _TITPL 2			//[s] okres czasu gdy ignorowany jest syg. plomien
#define _TDTPL 7			//[s] czas na stwierdzenie obecnosci plonienia
#define _TDWPL 2			//[s] wybieg na mocy startowej w proc. rozruchu
#define _TPLPR	30			//[s] czas pojedynczej proby przy braku plomienia
#define _NCYKLI_PL 3		//liczba prob przy braku plomienia dla gazu ziemnego
#define _NCYKLI_GPL 2		//liczba prob przy braku plomienia dla gazu plynnego
#define _NCYKLI_CIG 5		//(turbo) maksymalna liczba zanikow ciagu w zadanym okresie (1min)
#define _NCYKLI_CIG2 4		//(atmosferyczny)maksymalna liczba ponowien proc. braku ciagu w zadanym okresie (x15 min)
#define _NCYKLI_CIG3 10		//(turbo)maksymalna liczba ponowien proc. braku ciagu w zadanym okresie (x15 sek)

#define _NGODZIN_AL 168		//liczba godzin po której nastapi aktywacja grzania AL
#define _NGODZIN_SRVAL 10	//liczba minut po której nastapi aktywacja grzania AL (w trybie kontrolnym)
#define _TCIPR0 15			//[s] czas proby przy braku ciagu (sterowniki turbo)
#define _TCIPR1 1			//[s] podtrzymanie stanu awaryjnego przy braku ciagu
#define _TCIPR2 15			//[m] czas proby przy braku ciagu (sterowniki atmosferyczne)
#define _TCIRLP 60			//[s] okres w ktorym zliczane sa kolejne stany braku ciagu
#define _TCIPR3 60			//*[m] 60czas po ktorym zerowany jest licznik zanikow ciagu
#define _TCIBLK 5			//[s] blokada 5 sek. po probie ciagu 15 sek.
#define _TCITRS 60			//[s] (turbo) czas do wyzerowania licznika prob ciagu
#define _TPR48 24			//[h] czas do wybiegu pompy 24h
#define _TPR48Z3 47			//[h] czas do wybiegu pompy 48h dla zaw 3dr


#define _TPNC 24			//[h] maksymalny czas pracy ciaglej
#define _TWB48 180			//[s] czas wybiegu pompy po 48h
#define _TWB48Z3 15			//[s] czas wybiegu pompy po 48h w obiegu WU
#define _TVWB0 2			//[s] okrs czasu (podczas wybiegu wentylatora) z blokada reakcji na brak ciagu
#define _TVWB1 _TCIPR0		//[s] czas wybiegu wentylatora
#define _TZKP 10			//[ds] zmiana kierunku obrotow pompy
#define _TWYL0 15			//[s] minimalny wybieg pompy
#define _TWYL1 180			//[s] maksymalny wybieg pompy
#define _TSRV0 5			//[s] czas wejscia do funkcji serwisowej
#define _TSRV1 60			//15[min] czas aktywnosci funkcji serwisowej
#define _TBL3 3				//[min] czas blokady L3
#define _TBL15S 150			//[ds] czas blokady 15 sek.
#define _TKRES 1			//[s] minimalny czas przytrzymania przycisku RESET
#define _TVHZ 5				//[s] czas proby przy prawidlowych parametrach sieci
#define _TEXTAL 60			//60 [min] maksymalny czas aktywnosci funkcji ANTYLEGIONELLA
#define _SRVTEXTAL 5        //[min] maksymalny czas aktywnosci funkcji ANTYLEGIONELLA (w trybie kontrolnym)
#define _NPKAL	3			//liczba nacisniec przycisku RESET konieczna do wywolania funkcji ANTYLEGIONELLA
#define _TOKAL	3			//[s] maksymalny czas dla _NPKAL-krotnego nacisniecia przycisku RESET dla funkcji ANTYLEGIONELLA

#define _TWSPWM 5			//[s] czas aktywnosci wstepnego progu dolnej granicznej wartosci PWM dla grzania CO

//czujnik przeplywu WG
#define _TDPR1 3			//[s] czas zwloki na za³aczenie czujnika przeplywu WG (po czasie staly kod 05)
#define _TRPR1 240			//[s] czas aktywnosci procedury awaryjnej do wylaczenia awaryjnego od czujnika przeplywu (staly kod 05, po czasie migajacy kod 05)
#define _TCPRO 5			//[s] czas do awaryjnego wylaczenia od uszkodzonego czujnika przeplywu WG (zalaczony mimo wylaczonej pompy (po czasie migajacy kod 05)

//WYBIEG POMPY
//wybieg CO
#define _TWBCO 180			//[s] czas wybiegu pompy po grzaniu w obiegu CO
//bitermiczny - wybieg CW
#define _TbtWBW0 20			//[s] minimalny czas wybiegu pompy po grzaniu w obiegu CW
#define _TbtWBW1 180		//[s] maksymalny czas wybiegu pompy po grzaniu w obiegu CW
//monotermiczny- wybieg CW
#define _TmnWBW0 20			//[s] minimalny czas wybiegu pompy po grzaniu w obiegu CW
#define _TmnWBW1 180		//[s] maksymalny czas wybiegu pompy po grzaniu w obiegu CW
//zasobnikowy- wybieg CW
#define _TzsWBW 60			//[s] czas wybiegu pompy po grzaniu w obiegu CW

#define _TSTRNST 30			//58[ds] 
//----------------------------------------------------------
//----------------------------------------------------------
//----------------------------------------------------------
extern void GoldenEye(void);

//funkcje lokalne
void InitGlobal(void);
void ErrDTF(void);
void ErrPTG(void);
void ToReset(void);
void DecWSW(void);
void DecERR(void);
void DecSTR(void);
void DecRozruch(void);
void SetRozruch(void);
void DecWylacz(void);
void SetWylacz(void);
void RuszPlomien(void);
void ReloadPlomien(void);
void RuszCiag(void);
void ReloadCiag(void);
void Proba48(void);
void Rusz48(void);
void Proba48Z3(void);
void Rusz48Z3(void);
void Rozruch(void);
void DecyzjaPLM(void);
void DecyzjaPLM2(void);
void VWybieg(void);
void ZmienKCOKCW(void);
void FunkcjaSerwisowa(void);
void KolejkaPobudzen(void);
void BlokadaL3(void);
void Blokada15S(void);
void ToObiegCW(void);
void ToObiegCO(void);
void Wylacz(void);
void RuszPWR(void);
void ClearData(void);
void SetLoMod(void);
void RestartFltPrt(void);
void ClearDiode(void);
void DecyzjaAntyLeg(void);
void DetekcjaPrzeplywuWG(void);
void OczekujNaPrzeplywWG(void);
void RefInterfaceData(void);
void GrzejON(void);
void GrzejOFF(void);

//osc.c
extern void InitOsc(void);
//ac.c
extern void InitAC(void);
//wsw.c
extern void InitWsw(void);
extern bit GetPoz(const unsigned char nrkol, const unsigned char nrpoz);
//pwm.c
extern void InitPWM(void);
extern void WritePWM(const unsigned char tpwm);
extern void WritePWM2(const unsigned char tpwm);
extern void WriteOPWM(unsigned char tpwm);
//print.c
extern void ClsScr(void);
extern void EndClsScr(void);
extern void TestScr(void);
extern void EndTestScr(void);
extern void ClsScr(void);
extern void EndClsScr(void);
extern void SetErr(const unsigned char kod);
extern void SetMsg(const unsigned char kod);
extern void PrintErr(const unsigned char kod, const unsigned char pls);
extern void PrintErrCs(const unsigned char kod, const unsigned char cs, const unsigned char pls);
extern void PrintErrTm(const unsigned char kod, const unsigned char tm, const unsigned char pls, const unsigned char cw);
extern void EndErr(void);
extern void PrintPR(const unsigned char lkod, unsigned char nkod, unsigned char tryb);
extern void EndPR(void);
extern void PrintNstCO(const unsigned char nst);
extern void PrintNstCW(const unsigned char nst);
extern void PrintNstKt(const unsigned char nst);
extern void EndNst(void);
extern void SetL3(void);
extern void PrintL3(void);
extern void EndL3(void);
//extern void PrintTdzCO(const unsigned char tm);
//extern void EndTdzCO(void);
//extern void PrintTdzCW(const unsigned char tm);
//extern void EndTdzCW(void);
extern void PrintOFF(void);
extern void EndOFF(void);
extern void PrintCS(const unsigned char cs);
extern void EndCS(void);
extern void PrintCW(const unsigned char tm);
extern void EndCW(void);
extern void PrintCO(const unsigned char tm);
extern void PrintHex(const unsigned char tm);
extern void SetDiode(unsigned char nrd, unsigned char stn);
extern void RefreshNst(const unsigned char nst);
extern void PrintAL(void);
extern void EndAL(void);
extern void PrintALTm(const unsigned char tm, const unsigned char pls);
extern void EndALTm(void);
extern void PrintSR(void);
extern void EndSR(void);
extern void PrintSRTm(const unsigned char tm, const unsigned char pls);
extern void EndSRTm(void);
//bit.c
extern bit GetBit(const unsigned char data, const unsigned char nbit);
extern void BitSet(unsigned char *data, const unsigned char nbit, const unsigned char bt);
extern void BitSet2(unsigned char *data, const unsigned char nbit, const unsigned char bt);
//rtimer.c
extern void StartRTG(const unsigned char Index);
extern unsigned char RTG(const unsigned char Index);
extern void StartRTS(const unsigned char Index);
extern unsigned char RTS(const unsigned char Index);
extern void StartRTdS(const unsigned char Index);
extern unsigned char RTdS(const unsigned char Index);
//timer1.c
extern void InitTimer1(void);
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
//lcd8x4.c
extern void InitLcd(void);
extern void ClearScr(void);
extern void Print1(const char * s);
extern void Print2(const char * s);
extern void Print3(const char * s);
extern void Print4(const char * s);
extern void AddText(const char * s, const unsigned char ind);
//czcw.c
extern void InitCzCw(void);
//pomiar.c
void InitDataPmr(void);
extern void FiltrPomiarowyCOCW(void);
extern void PomiarCO(void);
extern void PomiarCW(void);
extern void PomiarZW(void);
extern void PomiarPCW(void);
extern void PomiarPCO(void);
extern void PomiarPFN(void);
extern void PomiarCS(void);
unsigned char GetPGD_PCO(void);
extern void PomiarIN(void);
//buzzer.c
extern void Buzzer(void);
extern void SetBuzz(const unsigned char sB, const unsigned char rB, const unsigned char nB);
extern void ResBuzz(void);
//uart.c
extern void InitUart(void);
extern void StartUSARTTR(const unsigned char nData);
extern void SetRSTBuf(const unsigned char indeks, const unsigned char data);
//mgs_rs.h
extern void RingUSART(void);
extern void TrToAMMY1(const unsigned char npow);
extern void TransData(const unsigned char tnDat, const unsigned char rnDat);
//modulator.c
extern unsigned char PierwszePobudzenieCW(const unsigned char nst, const unsigned char prz, const unsigned char tpgz);
extern unsigned char PierwszePobudzenieCO(const unsigned char nst, const unsigned char tpgz);
//konfig.c
extern void EnKonf(void);
extern void MKonfiguracja(void);
extern void RdEEParam(void);
//keypad.c
extern unsigned char PresKey(unsigned char k);
extern unsigned char ExPresKey(unsigned char k, unsigned char dTim, unsigned char pTim);
extern unsigned char ExPresKey2(unsigned char k, unsigned char dTim, unsigned char pTim);
extern unsigned char ReadDataXONL(unsigned char k, unsigned char *dt, unsigned char min, unsigned char max, const unsigned char step, const unsigned char step2, unsigned char wrn);
//rgl.c
extern void RestartMod(void);
extern void Moduluj(void);
//smgs_rs9b.c
extern void ReceptDataLCD(void);
extern void InitReceptProc(void);
//pompa.c
extern void RglPmp(void);
//konsola.c
extern void ReadParam(volatile unsigned int *param, unsigned char *zmiana, unsigned char *zapis, unsigned int min, unsigned int max, unsigned char rckl, unsigned char fst);
extern void TrybPracy(void);
//----------------------------------------------------------
#endif		/*_MAIN_H*/
