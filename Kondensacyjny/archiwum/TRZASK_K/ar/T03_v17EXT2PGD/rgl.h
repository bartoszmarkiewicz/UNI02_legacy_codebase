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

	void StartCO(void);
	void StartMCW(void);
	//void FirstPCW(void);
	unsigned char FirstPCW(void);
	void ClrMod(void);
	void ModToCW(void);
	void ModToCO(void);
	void IncConstCOCW(void);
	void ReadDataCO(void);
	void ReadDataCW(void);
	void ModZgCW(void);
	void ModDkCW(void);
	void ModPruCW(void);
	void IncMaxCW(void);
	void ModPrzCW(void);
	void PropCO(void);

	void StartCO2(void);
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

	void StartMCO3(void);
	void ModToCO3(void);
	void ModToCW3(void);

	void StartCO4(void);
	void StartMCW4(void);
	void FirstPCW4(void);
	void ClrMod4(void);
	void ModToCW4(void);
	void ModToCO4(void);
	void IncConstCOCW4(void);
	void ReadDataCO4(void);
	void ReadDataCW4(void);
	void ModZgCW4(void);
	void ModDkCW4(void);
	void ModPruCW4(void);
	void IncMaxCW4(void);
	void ModPrzCW4(void);
	void PropCO4(void);
	void NewMGMOD(void);
	void New_MIN(void);
	void New_MAX_CW(void);
	void New_MAX_CO(void);
	void NewPop_MIN(void);
	void NewPop_MAX_CW(void);
	void NewPop_MAX_CO(void);

	bit Trosnie(const unsigned char t);
	bit Tmaleje(const unsigned char t);
	bit Tstoi(void);
	bit Tnizsza(const unsigned char t);
	bit Twyzsza(const unsigned char t);
	bit Trowna(void);
	bit TFrosnie(const unsigned char t);
	bit TFmaleje(const unsigned char t);
	bit TFstoi(void);
//pwm.c
	extern void WritePWM(const unsigned char tpwm);
	extern void WriteOPWM(unsigned char tpwm);
//rtimer.c
	extern void StartRTdS(const unsigned char Index);
	extern unsigned char RTdS(const unsigned char Index);



extern unsigned char ProcToVNT(unsigned char wrt);
extern void SetNstVNT(unsigned int doc);
extern bit Vrosnie(const unsigned char t);
extern bit Vmaleje(const unsigned char t);
extern bit Vstoi(void);
extern bit VTrosnacy(const unsigned char t);
extern bit VTmalejacy(const unsigned char t);
extern bit VTzerowy(void);

//pompa.c
	extern void PmpToUP(void);
	extern void PmpToDW(void);
#endif //_RGL_H_
