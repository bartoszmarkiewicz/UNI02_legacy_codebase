#ifndef	_MAIN_H
#define	_MAIN_H

#define _VCC 0			//wersja napiêciowa uk³adu 18F4525 (0 = 3,3V, 1 = 5V)
//--------------------------------------------------------------
//--------------------------------------------------------------
#define _VER5V _VCC
//----------------------------------------------------------
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
#define _NGODZIN_AL 168		//liczba godzin po ktï¿½rej nastapi aktywacja grzania AL
#define _NGODZIN_SRVAL 10	//liczba minut po ktï¿½rej nastapi aktywacja grzania AL (w trybie kontrolnym)
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
 
//czujnik przeplywu WG
#define _TDPR1 6			//3[s] czas zwloki na zalaczenie czujnika przeplywu WG (po czasie staly kod 05)
#define _TRPR1 240			//[s] czas aktywnosci procedury awaryjnej do wylaczenia awaryjnego od czujnika przeplywu (staly kod 05, po czasie migajacy kod 05)
#define _TCPRO 6			//5[s] czas do awaryjnego wylaczenia od uszkodzonego czujnika przeplywu WG (zalaczony mimo wylaczonej pompy (po czasie migajacy kod 05)

#define _TLWPMP 50         //[ds] cas obnizenia predkosci pompy na czas przelaczenia zaworu 3DR     
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
void SetLowPMP(void);
void LicznikGodzin(void);
void SymulacjaPlomienia(void);

//----------------------------------------------------------
#endif		/*_MAIN_H*/