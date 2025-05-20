/*rgl.h*/
#ifndef _RGL_H_
#define _RGL_H_

#include "rgl.tp.h"
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//regulacja dla parametrow granicznych
#define _VNT_KDW 1				//wielkoœæ modyfikacji obrotow [pwm] co krok czasowy _VNT_TDW
//#define _VNT_TDW 6				//3krok czasowy [*0.1sek] dla modyfikacji obrotow w dol o krok _VNT_KDW
								//UWAGA _VNT_TDW>=1!
#define _VNT_KUP 1				//wielkoœæ modyfikacji obrotow [pwm] co krok czasowy _VNT_TUP
//#define _VNT_TUP 6				//3krok czasowy [*0.1sek] dla modyfikacji obrotow w dol o krok _VNT_KUP
								//UWAGA _VNT_TUP>=1!
//#define _VNT_TZ 10				//regulacja - jednorazowe opoznienie reakcji [*0.1sek] po zmianie wartosci docelowej
//#define _VNT_TZ2 5				//manual - jednorazowe opoznienie reakcji [*0.1sek] po zmianie wartosci docelowej

#define _PID_VNT 1
#define _HIWSP 4
/*
#define _PID_P 60/100000.0      //60
#define _PID_I 0//0.001/100000.0
#define _PID_D 0.0/100000.0
 */
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//regulacja w obiegu CO/CW
#define _TOKM 10			//[ds] krok czasowy modulacji

//parametry konieczne do obliczenia pierwszego pobudzenia dla obiegu CO
#define TCOMAX_MD1	 80			//gorna granica zakresu roboczego temperatur
#define TCOMIN_MD1	 20			//dolna granica zakresu roboczego temperatur
#define DCOMAX_MD1	 40			//40 maksymalna wartosc roznicy miedzy nastawa a temperatura
#define DCOMIN_MD1	 0			//0 minimalna wartosc roznicy miedzy nastawa a temperatura

//parametry konieczne do obliczenia pierwszego pobudzenia dla obiegu CW
#define TCWMAX_MD1	 60			//gorna granica zakresu roboczego temperatur
#define TCWMIN_MD1	 20			//dolna granica zakresu roboczego temperatur
#define DCWMAX_MD1	 5			//40 maksymalna wartosc roznicy miedzy nastawa a temperatura
#define DCWMIN_MD1	 0			//0 minimalna wartosc roznicy miedzy nastawa a temperatura

//pierwsze pobudzenie dla obiegu CO
#define I80_MOD	 0x6E
#define I80_PMOD 0x8C


#define MS_250		0x38		//P=250		20%
#define MS_400		0x49		//P=400		30%
#define MS_600		0x6A		//P=600		40%
#define MS_800		0x80		//P=800		60%
#define MS_1000		0x90		//P=1.000	70%
#define MS_1200		0x9A		//P=1.200	85%

#define MS_P600		0x64		//P=0.600	20%
#define MS_P900		0x82		//P=0.900	30%
#define MS_P1200	0x9A		//P=1.200	40%
#define MS_P1700	0xB4		//P=1.700	60%
#define MS_P2100	0xC8		//P=2.100	70%
#define MS_P2500	0xDC		//P=2.500	85%
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//lokalne
void StartMCO1F(void);
void ModZgCO(void);
void ModDkCO(void);
void ModPruCO(void);
void IncMaxCO(void);
void ModPrzCO(void);
void PropOPT(void);

void StartCO2(void);
void StartCO3(void);
void StartMCW2(void);
void FirstPCW2(void);
void ClrMod2(void);
void ModToCW2(void);
void ModToCO2(void);
void IncConstCW2(void);
void IncConstCO2(void);
void ReadDataCO2(void);
void ReadDataCW2(void);
void ModZgCW2(void);
void ModDkCW2(void);
void ModPruCW2(void);
void IncMaxCW2(void);
void ModPrzCW2(void);
void PropCO2(void);

void New_MIN(void);
void New_MAX_CW(void);
void New_MAX_CO(void);
void NewPop_MIN(void);
void NewPop_MAX_CW(void);
void NewPop_MAX_CO(void);
unsigned int VMinToPWM(unsigned char ref);
unsigned int VMaxToPWM(unsigned char ref, unsigned char cw);
void ResetVntToPWM(void);
bit Trosnie(const unsigned char t);
bit Tmaleje(const unsigned char t);
bit Tstoi(void);
bit Tnizsza(const unsigned char t);
bit Twyzsza(const unsigned char t);
bit Trowna(void);
bit TFrosnie(const unsigned char t);
bit TFmaleje(const unsigned char t);
bit TFstoi(void);

#endif //_RGL_H_
