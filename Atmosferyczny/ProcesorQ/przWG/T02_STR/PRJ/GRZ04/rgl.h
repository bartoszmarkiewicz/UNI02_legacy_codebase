/*rgl.h*/
#ifndef _RGL_H_
#define _RGL_H_

#include "rgl.tp.h"

#define _TOKM 10			//[ds] krok czasowy modulacji

//parametry konieczne do bliczenia pierwszego pobudzenia dla obiegu CW
#define TMAX_MOD	 60			//gorna granica zakresu roboczego temperatur
#define TMIN_MOD	 20			//dolna granica zakresu roboczego temperatur
#define DMAX_MOD	 40			//maksymalna wartosc roznicy miedzy nastawa a temperatura
#define DMIN_MOD	 0			//minimalna wartosc roznicy miedzy nastawa a temperatura

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

//lokalne
void StartMCW(void);
unsigned char  FirstPCW(void);
void ClrMod(void);
void ModToCW(void);
void ModToCO(void);
void IncConstCW(void);
void ReadDataCO(void);
void ReadDataCW(void);
void ModZgCW(void);
void ModDkCW(void);
void ModPruCW(void);
void IncMaxCW(void);
void ModPrzCW(void);
void SetPWM_CW(void);

bit Trosnie(const unsigned char t);
bit Tmaleje(const unsigned char t);
bit Tstoi(void);
bit Tnizsza(const unsigned char t);
bit Twyzsza(const unsigned char t);
bit Trowna(void);
#endif //_RGL_H_
