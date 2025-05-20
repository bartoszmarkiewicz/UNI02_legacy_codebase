#ifndef	_MAIN_H
#define	_MAIN_H

#define _VCC 0			//wersja napiêciowa uk³adu 18F4525 (0 = 3,3V, 1 = 5V)
//--------------------------------------------------------------
//--------------------------------------------------------------
#define _VER5V _VCC
//----------------------------------------------------------
#define _RNMOD 0
//----------------------------------------------------------
#define _TSTR 3				//[s] czas do wyzerowania znacznika rozruchu sterownika
#define _TBPL0 5			//[s] czas do stwierdzenia uszkodzenia plytki zapalacza
#define _TCIRO 5			//[s] czas do uruchomienia awaryjnej procedury ciagu w trakcie rozruchu
#define _TITPL 2			//[s] okres czasu gdy ignorowany jest syg. plomien
#define _TDTPL 7			//[s] czas na stwierdzenie obecnosci plonienia

#define _TDWPL 3			//5 [s] wybieg na mocy startowej (modulator/wentylator) w proc. rozruchu
#define _TVSTRT 3           //3 [s] przedluzony wybieg na mocy startowej (wentylator)

#define _TPLPR	30			//[s] czas pojedynczej proby przy braku plomienia
#define _NCYKLI_PL 3		//liczba prob przy braku plomienia dla gazu ziemnego
#define _NCYKLI_GPL 2		//liczba prob przy braku plomienia dla gazu plynnego
#define _NCYKLI_CIG 5		//maksymalna liczba ponowien braku ciagu w zadanym okresie 
#define _NCYKLI_CIG2 4		//(atmosferyczny)maksymalna liczba ponowien proc. braku ciagu w zadanym okresie (x15 min)
#define _NCYKLI_CIG3 3		//10(turbo)maksymalna liczba ponowien proc. braku ciagu w zadanym okresie (x15 sek)

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
#define _TVWB1 _TCIPR0-2	//[s] czas wybiegu wentylatora po zaplonie lub podczas awarii
#define _TVWB2 5			//[s] czas wybiegu wentylatora po przerwanym zaplonie

#define _TZKP 10			//[ds] zmiana kierunku obrotow pompy
#define _TWYL0 15			//[s] minimalny wybieg pompy
#define _TWYL1 180			//[s] maksymalny wybieg pompy
#define _TSRV0 5			//[s] czas wejscia do funkcji serwisowej
#define _TSRV1 15			//[min] czas aktywnosci funkcji serwisowej 
#define _TBL3 3				//[min] czas blokady L3
#define _TBL15S 150			//[ds] czas blokady 15 sek.
#define _TKRES 1			//[s] minimalny czas przytrzymania przycisku RESET
#define _TVHZ 5				//[s] czas proby przy prawidlowych parametrach sieciq                                                    

#define _TVPOM 15			//[s] maksymalny czas oczekiwania na uzyskanie predkosci startowej

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
void VSWybieg(void);
//----------------------------------------------------------
#endif		/*_MAIN_H*/