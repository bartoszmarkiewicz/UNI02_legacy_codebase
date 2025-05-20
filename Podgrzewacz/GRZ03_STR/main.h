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
	__CONFIG(1,IESOEN&FCMEN&RCIO);
	//__CONFIG(2,BOREN&BORV27&PWRTEN&WDTEN&WDTPS128);
	__CONFIG(2,PWRTDIS&BOREN&BORV27&WDTEN&WDTPS128);
	//__CONFIG(2,BORDIS&PWRTEN&WDTEN&WDTPS128);
	__CONFIG(3,LPT1DIS&MCLRDIS&PBDIGITAL);
	__CONFIG(4,XINSTDIS&DEBUGDIS&LVPDIS&STVREN);
	//__CONFIG(5,UNPROTECT);
	//__CONFIG(6,WRTEN);
	__CONFIG(5,CPA);
	__CONFIG(6,WPA&WPC);
#else
	__CONFIG(1,IESOEN&FCMEN&RCIO);
	#if _VER5V==0
	__CONFIG(2,BOREN&BORV28&PWRTEN&WDTEN&WDTPS128);
	#else 
	__CONFIG(2,BOREN&BORV43&PWRTEN&WDTEN&WDTPS128);
	#endif
	__CONFIG(3,LPT1DIS&MCLRDIS&PBANDIS);
	__CONFIG(4,XINSTDIS&DEBUGDIS&LVPDIS&STVREN);
	//__CONFIG(5,UNPROTECT);
	//__CONFIG(6,WRTEN);
	__CONFIG(5,CPA);
	__CONFIG(6,WPA&WPC); 
#endif 
#define _OFFTMP 1           //znacznik czy ma byc stale wygaszony wskaz temperatury (0-temp wyswietlana, 1 - temp wygaszona)
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
#define _NCYKLI_CIG 5		//maksymalna liczba ponowien braku ciagu w zadanym okresie 
#define _NCYKLI_CIG2 4		//(atmosferyczny)maksymalna liczba ponowien proc. braku ciagu w zadanym okresie (x15 min)
#define _NCYKLI_CIG3 10		//(turbo)maksymalna liczba ponowien proc. braku ciagu w zadanym okresie (x15 sek)

#define _TCIPR0 15			//[s] czas proby przy braku ciagu (sterowniki turbo)
#define _TCIPR1 1			//[s] podtrzymanie stanu awaryjnego przy braku ciagu
#define _TCIPR2 15			//[m] czas proby przy braku ciagu (sterowniki atmosferyczne)
#define _TCIRLP 240			//60[s] okres w ktorym zliczane sa kolejne stany braku ciagu
#define _TCIPR3 60			//*[m] 60czas po ktorym zerowany jest licznik zanikow ciagu
#define _TCIBLK 5			//[s] blokada 5 sek. po probie ciagu 15 sek.
#define _TCITRS 60			//[s] (turbo) czas do wyzerowania licznika prob ciagu



#define _TPR48 24			//[h] czas do wybiegu pompy 24h
#define _TPNC 24			//[h] maksymalny czas pracy ciaglej
#define _TWB48 180			//[s] czas wybiegu pompy po 48h
#define _TVWB0 2			//[s] okrs czasu (podczas wybiegu wentylatora) z blokada reakcji na brak ciagu
#define _TVWB1 _TCIPR0		//[s] czas wybiegu wentylatora po zaplonie lub podczas awarii
#define _TVWB2 5			//[s] czas wybiegu wentylatora po przerwanym zaplonie

#define _TZKP 10			//[ds] zmiana kierunku obrotow pompy
#define _TWYL0 15			//[s] minimalny wybieg pompy
#define _TWYL1 180			//[s] maksymalny wybieg pompy
#define _TSRV0 5			//[s] czas wejscia do funkcji serwisowej
#define _TSRV1 15			//[min] czas aktywnosci funkcji serwisowej 
#define _TBL3 3				//[min] czas blokady L3
#define _TBL15S 150			//[ds] czas blokady 15 sek.
#define _TKRES 1			//[s] minimalny czas przytrzymania przycisku RESET
#define _TVHZ 5				//[s] czas proby przy prawidlowych parametrach sieci


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
//----------------------------------------------------------
//----------------------------------------------------------
//----------------------------------------------------------
extern void GoldenEye(void);

//funkcje lokalne
	void InitGlobal(void);
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
	void Rozruch(void);
	void DecyzjaPLM(void);
	void DecyzjaPLM2(void);
	void VWybieg(void);
	void KolejkaPobudzen(void);
	void FunkcjaSerwisowa(void);
	void ClearData(void);
	void SetLoMod(void);
	void RestartFltPrt(void);
	void ErrDTF(void);
	void GrzejON(void);
	void GrzejOFF(void);
    void KontrolaWentylatora(void);
    void KalibracjaWentylatora(void);
//timer3.c
	extern void InitTimer3(void);    
//osc.c
	extern void InitOsc(void);
//ac.c
	extern void InitAC(void);
//wsw.c
	extern void InitWsw(void);
	extern bit GetPoz(const unsigned char nrkol,const unsigned char nrpoz);
//pwm.c
	extern void InitPWM(void);
	extern void WritePWM(const unsigned char tpwm);
	extern void WriteOPWM(unsigned char tpwm);
//print.c
	extern void ClsScr(void);
	extern void EndClsScr(void);
	extern void TestScr(void);
	extern void EndTestScr(void);
	extern void ClsScr(void);
	extern void EndClsScr(void);
	extern void PrintErr(const unsigned char kod, const unsigned char pls);
	extern void PrintErrCs(const unsigned char kod,const unsigned char cs, const unsigned char pls);
	extern void PrintErrTm(const unsigned char kod,const unsigned char tm, const unsigned char pls);
	extern void EndErr(void);
	extern void PrintPR(const unsigned char lkod,unsigned char nkod,unsigned char tryb);
	extern void EndPR(void);
	extern void PrintNstCO(const unsigned char nst);
	extern void PrintNstCW(const unsigned char nst,const unsigned char krp);
	extern void PrintNstKt(const unsigned char nst);
	extern void EndNst(void);
	extern void PrintL3(void);
	extern void EndL3(void);
	extern void PrintTdzCO(const unsigned char tm);
	extern void EndTdzCO(void);
	extern void PrintTdzCW(const unsigned char tm);
	extern void EndTdzCW(void);
	extern void PrintOFF(void);
	extern void EndOFF(void);
	extern void PrintCS(const unsigned char cs);
	extern void EndCS(void);
	extern void PrintCW(const unsigned char tm,const unsigned char krp);
	extern void EndCW(void);
	extern void PrintCO(const unsigned char tm);
	extern void PrintHex(const unsigned char tm);
	extern void SetDiode(unsigned char nrd, unsigned char stn);
	extern void RefreshNst(const unsigned char nst);
//bit.c
	extern bit GetBit(const unsigned char data,const unsigned char nbit);
	extern void BitSet(unsigned char *data, const unsigned char nbit,const unsigned char bt);
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
	extern void AddText(const char * s,const unsigned char ind);
//czcw.c
	extern void InitCzCw(void);
//pomiar.c
	void InitDataPmr(void);
	extern void FiltrPomiarowyCW(void);
	extern void PomiarCW(void);
	extern void PomiarPCW(void);
	extern void WritePCW(void);
//buzzer.c
	extern void Buzzer(void);
	extern void SetBuzz(const unsigned char sB,const unsigned char rB,const unsigned char nB);
	extern void ResBuzz(void);
//uart.c
	extern void InitUart(void);
	extern void StartUSARTTR(const unsigned char nData);
	extern void SetRSTBuf(const unsigned char indeks,const unsigned char data);
//mgs_rs.h
	extern void RingUSART(void);
	extern void TrToAMMY1(const unsigned char npow);
	extern void TransData(const unsigned char tnDat,const unsigned char rnDat);
//modulator.c
	extern unsigned char PierwszePobudzenieCW(const unsigned char nst,const unsigned char prz,const unsigned char tpgz);
	extern unsigned char PierwszePobudzenieCO(const unsigned char nst,const unsigned char tpgz);
//konfig.c
	extern void EnKonf(void);
	extern void MKonfiguracja(void);
	extern void RdEEParam(void);
//keypad.c
	extern unsigned char PresKey(unsigned char k);
	extern unsigned char ExPresKey(unsigned char k,unsigned char dTim,unsigned char pTim);
	extern unsigned char ExPresKey2(unsigned char k,unsigned char dTim,unsigned char pTim);
	extern unsigned char ReadDataXONL(unsigned char k,unsigned char *dt,unsigned char min,unsigned char max,const unsigned char step,const unsigned char step2,unsigned char wrn);
//rgl.c
	extern void RestartMod(void);
	extern void Moduluj(void);
	//extern unsigned char FirstPCW4(void);
    extern unsigned char ProcToMod(unsigned char prvnt);
//vntregtch.c
    extern unsigned char CalkTimStr(unsigned char vstr);
    extern unsigned char KalibracjaVNT(void);
    extern void OdczytDanychKalibracji();
    extern void PomiarVNT(void);
    extern unsigned char ProcToVNT(unsigned char wrt);
//----------------------------------------------------------
#endif		/*_MAIN_H*/
