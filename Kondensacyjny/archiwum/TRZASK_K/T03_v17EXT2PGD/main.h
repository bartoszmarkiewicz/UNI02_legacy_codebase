/*main.h*/
#ifndef	_MAIN_H
#define	_MAIN_H
#include "main.tp.h"		//typy danych
#include "wersja.h"			//oznaczenie wersji p³yty steruj±cej
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
	__CONFIG(2,PWRTDIS&BOREN&BORV27&WDTEN&WDTPS128);
	__CONFIG(3,LPT1DIS&MCLRDIS&PBDIGITAL&CCP2RC1);
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


//----------------------------------------------------------
#define _TSTR 3				//[s] czas do wyzerowania znacznika rozruchu sterownika
#define _TBPL0 5			//[s] czas do stwierdzenia uszkodzenia plytki zapalacza
#define _TITPL 2			//[s] okres czasu gdy ignorowany jest syg. plomien
#define _TDTPL 7+6			//[s] czas na stwierdzenie obecnosci plonienia (wsparcie dla ukladow z blokada kominowa)
#define _TDWPL 2			//[s] wybieg na mocy startowej w proc. rozruchu
#define _TPLPR	16			//[s] czas pojedynczej proby przy braku plomienia
#define _TPLPR2	60			//[s] czas po ktorym zerowany jest licznik wystapien sytuacji awaryjnej E1
#define _NCYKLI_PL 3		//liczba prob przy braku plomienia dla gazu ziemnego
#define _NCYKLI_PL2 5		//maks.liczba wystapien sytuacji awaryjnych E1 w okr. odcinku czasu do wylaczenia z blokada
#define _NGODZIN_AL 168		//liczba godzin po której nastapi aktywacja grzania AL
#define _NGODZIN_SRVAL 10	//liczba minut po której nastapi aktywacja grzania AL (w trybie kontrolnym)
#define _TCIBLK 5			//[s] blokada 5 sek. po probie ciagu 15 sek.
#define _TPR48 24			//[h] czas do wybiegu pompy 24h
#define _TPR48Z3 47			//[h] czas do wybiegu pompy 48h dla zaw 3dr


#define _TWB48 180			//[s] czas wybiegu pompy po 48h
#define _TWB48Z3 15			//[s] czas wybiegu pompy po 48h w obiegu WU
#define _TVWB0 2			//[s] okrs czasu (podczas wybiegu wentylatora) z blokada reakcji na brak ciagu
#define _TVWB1 15			//[s] czas wybiegu wentylatora
#define _TZKP 10			//[ds] zmiana kierunku obrotow pompy
#define _TWYL0 15			//[s] minimalny wybieg pompy
#define _TWYL1 180			//[s] maksymalny wybieg pompy
#define _TSRV0 5			//[s] czas wejscia do funkcji serwisowej
#define _TSRV1 60			//15[min] czas aktywnosci funkcji serwisowej 
#define _TBL3 3				//[min] czas blokady L3 (!od teraz definiowana w konfiguracji)
#define _TBL15S 150			//[ds] czas blokady 15 sek.
#define _TKRES 1			//[s] minimalny czas przytrzymania przycisku RESET
#define _TEXTAL 60			//[min] maksymalny czas aktywnosci funkcji ANTYLEGIONELLA
#define _SRVTEXTAL 5		//[min] maksymalny czas aktywnosci funkcji ANTYLEGIONELLA (w trybie kontrolnym)
#define _NPKAL	3			//liczba nacisniec przycisku RESET konieczna do wywolania funkcji ANTYLEGIONELLA
#define _TOKAL	3			//[s] maksymalny czas dla _NPKAL-krotnego nacisniecia przycisku RESET dla funkcji ANTYLEGIONELLA

#define _TWSPWM 5			//[s] czas aktywnosci wstepnego progu dolnej granicznej wartosci PWM wentylatora dla grzania CO

#define _TLWPMP 50         //[ds] cas obnizenia predkosci pompy na czas przelaczenia zaworu 3DR         
    
//czujnik przeplywu WG
#define _TDPR1 3			//[s] czas zwloki na za³aczenie czujnika przeplywu WG (po czasie staly kod 05)
#define _TRPR1 240			//[s] czas aktywnosci procedury awaryjnej do wylaczenia awaryjnego od czujnika przeplywu (staly kod 05, po czasie migajacy kod 05)
#define _TCPRO 5			//[s] czas do awaryjnego wylaczenia od uszkodzonego czujnika przeplywu WG (zalaczony mimo wylaczonej pompy (po czasie migajacy kod 05)


    
//WYBIEG POMPY
//wybieg CO
#define _TWBCO 180			//[s] czas wybiegu pompy po grzaniu w obiegu CO
#define _TPRWB 3			//[s] czas wstepnego wybiegu pompy dla kotlow jednofunkcyjnych      
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
	void RefLCDData(void);
	void GrzejON(void);
	void GrzejOFF(void);
	void KalibracjaWentylatora(void);
	void KontrolaWentylatora(void);
	void Odpowietrzenie(void);
	void DecyzjaOdp(void);
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
	extern void WritePWM2(const unsigned char tpwm);
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
	extern void PrintNstCW(const unsigned char nst);
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
	extern void PrintCW(const unsigned char tm);
	extern void EndCW(void);
	extern void PrintCO(const unsigned char tm);
	extern void PrintHex(const unsigned char tm);
	extern void SetDiode(unsigned char nrd, unsigned char stn);
	extern void RefreshNst(const unsigned char nst);
//bit.c
	extern bit GetBit(const unsigned char data,const unsigned char nbit);
	extern void BitSet(unsigned char *data, const unsigned char nbit,const unsigned char bt);
	extern void BitSet2(unsigned char *data, const unsigned char nbit,const unsigned char bt);
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
	extern void InitDataPmr(void);
	extern void FiltrPomiarowyCOCW(void);
	extern void PomiarCO(void);
	extern void PomiarCW(void);
	extern void PomiarZW(void);
	extern void PomiarPCW(void);
	extern void PomiarPCO(void);
	extern void PomiarPFN(void);
	extern void PomiarCS(void);
	extern unsigned char GetPGD_PCO(void);
	extern void PomiarIN(void);
	extern void PomiarECO(void);
    extern void SetTrbPGD(void);    
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
	extern void DsKonf(void);
	extern void MKonfiguracja(void);
	extern void RdEEParam(void);
    extern void WriteEESTAT(const unsigned char nr);
    extern unsigned char ReadBtySTAT(const unsigned char nr, const unsigned char ind);
//keypad.c
	extern unsigned char PresKey(unsigned char k);
	extern unsigned char ExPresKey(unsigned char k,unsigned char dTim,unsigned char pTim);
	extern unsigned char ExPresKey2(unsigned char k,unsigned char dTim,unsigned char pTim);
	extern unsigned char ReadDataXONL(unsigned char k,unsigned char *dt,unsigned char min,unsigned char max,const unsigned char step,const unsigned char step2,unsigned char wrn);
//rgl.c
	extern void RestartMod(void);
	extern void Moduluj(void);
//smgs_rs9b.c
	extern void ReceptDataLCD(void);
	extern void InitReceptProc(void);
//tacho.c
    extern void ClrTachoDta(void);
//history.c
    extern void ToWriteESTAT(void);
    extern unsigned char RefBHist(const unsigned char res);
    extern void RefBStat(const unsigned char res);

void ClrDtV(void);
void AddTabNp(unsigned char d);
unsigned int CalcSrNp(void);
unsigned int VVent(unsigned int np);
void PomiarVNT(void);
//unsigned char ModToDVNT(unsigned char dvnt,unsigned char minvnt,unsigned char maxvnt,unsigned char pw,unsigned char kal);
unsigned char MrgVNT(unsigned char minvnt,unsigned char maxvnt,unsigned char pw);
unsigned char KalibracjaVNT(void);
void OdczytDanychKalibracji();
unsigned char ReadEECF(void);
unsigned char ProcToVNT(unsigned char wrt);
unsigned char ModToDVNT(unsigned char dvnt,unsigned char minvnt,unsigned char maxvnt,unsigned char pw);
unsigned char DVNTtoPWM(unsigned char dvnt);
unsigned char CalkTimStr(unsigned char vstr);
unsigned char VNTToProc(unsigned char wrt);
unsigned char ModToMin(void);
unsigned char ModToMax(void);
unsigned char ProcToMod(unsigned char prvnt);
extern void RglPmp(void);
extern void RglPmp2(void);
extern unsigned char ProcToPmpPwm(unsigned char prc);
extern void WritePWM2(const unsigned char tpwm);

unsigned char ReadBtyNZSTAT(const unsigned char ind);
unsigned char ReadBtyHREF(const unsigned char ind);
void SetLowPMP(void);
//----------------------------------------------------------
#endif		/*_MAIN_H*/
