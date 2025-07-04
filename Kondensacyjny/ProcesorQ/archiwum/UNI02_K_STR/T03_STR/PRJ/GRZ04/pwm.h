/*pwm.h*/
//modulator SITA
#ifndef	_PWM_H
#define	_PWM_H
#include "pwm.tp.h"
#include "konfig.h"

#define _NVNTPWM 9
#define _NPWMVNT 9

#define _CHR_PWM 0
#define _EN_PID 1
/*
#if _CHR_PWM==1
//Gaz ziemny
const unsigned char pvnt0[21]={
    7,      //0%
    8,      //5%
    10,     //10%
    11,     //15%
    12,     //20%
    13,     //25%
    14,     //30%
    14,     //35%
    14,     //40%
    16,     //45%
    17,     //50%
    17,     //55%
    19,     //60%
    19,     //65%
    20,     //70%
    20,     //75%
    21,     //80%
    21,     //85%
    22,     //90%
    22,     //95%
    22      //100%
};
//Gaz plynny
const unsigned char pvnt1[21]={
    8,      //0%
    9,      //5%
    9,      //10%
    11,     //15%
    11,     //20%
    12,     //25%
    13,     //30%
    14,     //35%
    14,     //40%
    15,     //45%
    16,     //50%
    16,     //55%
    17,     //60%
    18,     //65%
    19,     //70%
    20,     //75%
    21,     //80%
    22,     //85%
    22,     //90%
    22,     //95%
    22      //100%
};
#else
//Gaz ziemny
const unsigned char pvnt0[21]={
    25,      //0%
    25,      //5%
    25,     //10%
    25,     //15%
    28,     //20%
    29,     //25%
    29,     //30%
    31,     //35%
    36,     //40%
    39,     //45%
    43,     //50%
    48,     //55%
    50,     //60%
    61,     //65%
    69,     //70%
    78,     //75%
    80,     //80%
    80,     //85%
    80,     //90%
    80,     //95%
    80      //100%
};
const unsigned char vnt0[21]={
    15,      //0%
    15,      //5%
    15,     //10%
    15,     //15%
    17,     //20%
    18,     //25%
    18,     //30%
    20,     //35%
    22,     //40%
    23,     //45%
    25,     //50%
    28,     //55%
    28,     //60%
    32,     //65%
    35,     //70%
    37,     //75%
    37,     //80%
    37,     //85%
    37,     //90%
    37,     //95%
    37      //100%
};
//Gaz plynny
const unsigned char pvnt1[21]={
    25,      //0%
    25,      //5%
    25,     //10%
    25,     //15%
    28,     //20%
    29,     //25%
    29,     //30%
    31,     //35%
    36,     //40%
    39,     //45%
    43,     //50%
    48,     //55%
    50,     //60%
    61,     //65%
    69,     //70%
    78,     //75%
    80,     //80%
    80,     //85%
    80,     //90%
    80,     //95%
    80      //100%
};
const unsigned char vnt1[21]={
    15,      //0%
    15,      //5%
    15,     //10%
    15,     //15%
    17,     //20%
    18,     //25%
    18,     //30%
    20,     //35%
    22,     //40%
    23,     //45%
    25,     //50%
    28,     //55%
    28,     //60%
    32,     //65%
    35,     //70%
    37,     //75%
    37,     //80%
    37,     //85%
    37,     //90%
    37,     //95%
    37      //100%
};
#endif
*/
//-------------------------------------------------------
//GRZEJNIK NISKOMOCOWY
//Zaleznosc predkosci wentylatora [obr/min*100] od wydatku na modulatorze [%] dla roznych dlugosci komina [MPPWM,VNT]
//Gaz ziemny
const DtPWMVNT pwm_vnt1[_NPWMVNT]={
    10,	10,
    20,	12,
    35,	15,
    50,	20,
    68,	25,
    99,	30,
    99,	30,
    99,	30,
    99,	30  
};
const DtPWMVNT pwm_vnt2[_NPWMVNT]={
    10,	10,
    19,	12,
    32,	15,
    46,	20,
    62,	25,
    99,	30,
    99,	30,
    99,	30,
    99,	30   
};
const DtPWMVNT pwm_vnt3[_NPWMVNT]={
    10,	10,
    16,	12,
    30,	15,
    45,	20,
    60,	25,
    80,	30,
    99,	32,
    99,	32,
    99,	32
};
const DtPWMVNT pwm_vnt4[_NPWMVNT]={
    10,	10,
    27,	15,
    40,	20,
    58,	25,
    70,	30,
    99,	34,
    99,	34,
    99,	34,
    99,	34
};
const DtPWMVNT pwm_vnt5[_NPWMVNT]={
    10, 10,
    27, 15,
    34,	20,
    50,	25,
    60,	30,
    99,	35,
    99,	35,
    99,	35,
    99,	35
};
const DtPWMVNT pwm_vnt6[_NPWMVNT]={
    10,	10,
    20,	15,
    34,	20,
    50,	25,
    60,	30,
    99,	35,
    99,	35,
    99,	35,
    99,	35        
};
const DtPWMVNT pwm_vnt7[_NPWMVNT]={
    10,	10,
    20,	15,
    34,	20,
    46,	25,
    60,	30,
    99,	35,
    99,	35,
    99,	35,
    99,	35
};
const DtPWMVNT pwm_vnt8[_NPWMVNT]={
    10,	10,
    15, 12,
    20,	15,
    30,	20,
    46,	25,
    57,	30,
    70,	35,
    99,	40,
    99,	40
};
//-------------------------------------------------------
//Gaz plynny
const DtPWMVNT pwm_vnt1p[_NPWMVNT]={
    10,	10,
    20,	12,
    35,	15,
    50,	20,
    68,	25,
    99,	30,
    99,	30,
    99,	30,
    99,	30  
};
const DtPWMVNT pwm_vnt2p[_NPWMVNT]={
    10,	10,
    19,	12,
    32,	15,
    46,	20,
    62,	25,
    99,	30,
    99,	30,
    99,	30,
    99,	30   
};
const DtPWMVNT pwm_vnt3p[_NPWMVNT]={
    10,	10,
    16,	12,
    30,	15,
    45,	20,
    60,	25,
    80,	30,
    99,	32,
    99,	32,
    99,	32
};
const DtPWMVNT pwm_vnt4p[_NPWMVNT]={
    10,	10,
    27,	15,
    40,	20,
    58,	25,
    70,	30,
    99,	34,
    99,	34,
    99,	34,
    99,	34
};
const DtPWMVNT pwm_vnt5p[_NPWMVNT]={
    10, 10,
    27, 15,
    34,	20,
    50,	25,
    60,	30,
    99,	35,
    99,	35,
    99,	35,
    99,	35
};
const DtPWMVNT pwm_vnt6p[_NPWMVNT]={
    10,	10,
    20,	15,
    34,	20,
    50,	25,
    60,	30,
    99,	35,
    99,	35,
    99,	35,
    99,	35        
};
const DtPWMVNT pwm_vnt7p[_NPWMVNT]={
    10,	10,
    20,	15,
    34,	20,
    46,	25,
    60,	30,
    99,	35,
    99,	35,
    99,	35,
    99,	35
};
const DtPWMVNT pwm_vnt8p[_NPWMVNT]={
    10,	10,
    15, 12,
    20,	15,
    30,	20,
    46,	25,
    57,	30,
    70,	35,
    99,	40,
    99,	40
};
//-------------------------------------------------------
//GRZEJNIK WYSOKOMOCOWY
//Zaleznosc predkosci wentylatora [obr/min*100] od wydatku na modulatorze [%] dla roznych dlugosci komina [MPPWM,VNT]
//Gaz ziemny
const DtPWMVNT Hpwm_vnt1[_NPWMVNT]={
    0,	10,
    6,	12,
    18,	15,
    36,	21,
    40,	25,
    52,	28,
    60,	32,
    76,	40,
    99,	40  
};
const DtPWMVNT Hpwm_vnt2[_NPWMVNT]={
    0,	12,
    10,	15,
    31,	22,
    40,	25,
    48,	28,
    58,	34,
    70,	41,
    99,	47, 
    99,	47             
};
const DtPWMVNT Hpwm_vnt3[_NPWMVNT]={
    0,	12,
    10,	15,
    31,	22,
    40,	28,
    55,	34,
    70,	41,
    99,	47,
    99,	47,
    99,	47
};
const DtPWMVNT Hpwm_vnt4[_NPWMVNT]={
    0,	12,
    8,	15,
    25,	22,
    39,	25,
    40,	28,
    55,	34,
    70,	41,
    88,	47,
    99,	47
};
const DtPWMVNT Hpwm_vnt5[_NPWMVNT]={
    0,  12,
    7,  15,
    27,	22,
    37,	25,
    40,	28,
    55,	34,
    80,	47,
    99,	47,
    99,	47
};
const DtPWMVNT Hpwm_vnt6[_NPWMVNT]={
    0,	12,
    7,	15,
    26,	21,
    34,	25,
    42,	28,
    52,	32,
    67,	41,
    76,	47,
    99,	47        
};
const DtPWMVNT Hpwm_vnt7[_NPWMVNT]={
    0,	12,
    6,	15,
    22,	21,
    31,	25,
    40,	28,
    50,	32,
    64,	41,
    75,	47,
    99,	50
};
const DtPWMVNT Hpwm_vnt8[_NPWMVNT]={
    0,	12,
    5,  15,
    22,	21,
    26,	25,
    37,	28,
    48,	32,
    56,	40,
    78,	45,
    99,	50
};
//Gaz plynny
const DtPWMVNT Hpwm_vnt1p[_NPWMVNT]={
    0,	10,
    6,	12,
    18,	15,
    36,	21,
    40,	25,
    52,	28,
    60,	32,
    76,	40,
    99,	40  
};
const DtPWMVNT Hpwm_vnt2p[_NPWMVNT]={
    0,	12,
    10,	15,
    31,	22,
    40,	25,
    48,	28,
    58,	34,
    70,	41,
    99,	47, 
    99,	47             
};
const DtPWMVNT Hpwm_vnt3p[_NPWMVNT]={
    0,	12,
    10,	15,
    31,	22,
    40,	28,
    55,	34,
    70,	41,
    99,	47,
    99,	47,
    99,	47
};
const DtPWMVNT Hpwm_vnt4p[_NPWMVNT]={
    0,	12,
    8,	15,
    25,	22,
    39,	25,
    40,	28,
    55,	34,
    70,	41,
    88,	47,
    99,	47
};
const DtPWMVNT Hpwm_vnt5p[_NPWMVNT]={
    0, 12,
    7, 15,
    27,	22,
    37,	25,
    40,	28,
    55,	34,
    80,	47,
    99,	47,
    99,	47
};
const DtPWMVNT Hpwm_vnt6p[_NPWMVNT]={
    0,	12,
    7,	15,
    26,	21,
    34,	25,
    42,	28,
    52,	32,
    67,	41,
    76,	47,
    99,	47        
};
const DtPWMVNT Hpwm_vnt7p[_NPWMVNT]={
    0,	12,
    6,	15,
    22,	21,
    31,	25,
    40,	28,
    50,	32,
    64,	41,
    75,	47,
    99,	50
};
const DtPWMVNT Hpwm_vnt8p[_NPWMVNT]={
    0,	12,
    5,  15,
    22,	21,
    26,	25,
    37,	28,
    48,	32,
    56,	40,
    78,	45,
    99,	50
};
//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
//Wypelnienie PWM wentylatora [%] dla oczekiwanej predkosci wentylatora [obr/min*100] dla roznych dlugosci komina [VNT,PPWM]
//Gaz ziemny
const DtVNTPWM vnt_pwm0[_NVNTPWM]={
    10,	20,
    17,	30,
    22,	40,
    25,	50,
    28,	60,
    31,	70,
    33,	80,
    35,	90,
    37,	99    
};
const DtVNTPWM vnt_pwm1[_NVNTPWM]={
    10,	17,
    12,	20,
    15,	25,
    20,	32,
    25,	45,
    30,	62,
    40,	90,
    40,	90,
    40,	90    
};
const DtVNTPWM vnt_pwm2[_NVNTPWM]={
    10,	17,
    12,	20,
    15,	25,
    20,	32,
    25,	42,
    30,	55,
    40,	90,
    40,	90,
    40,	90    
};
const DtVNTPWM vnt_pwm3[_NVNTPWM]={
    10, 17,
    12, 20,
    15, 25,
    20, 32,
    25, 42,
    30, 52,
    32, 60,
    35, 75,
    40, 90
};
const DtVNTPWM vnt_pwm4[_NVNTPWM]={
    10, 17,
    12, 20,
    15, 25,
    20, 32,
    25, 42,
    30, 52,
    33, 65,
    35, 75,
    40, 90
};
const DtVNTPWM vnt_pwm5[_NVNTPWM]={
    10, 17,
    12, 20,
    15, 25,
    20, 30,
    25, 40,
    30, 50,
    35, 70,
    40, 90,
    40, 90
};
//-------------------------------------------------------
//Gaz plynny
const DtVNTPWM vnt_pwm0p[_NVNTPWM]={
    10,	20,
    17,	30,
    22,	40,
    25,	50,
    28,	60,
    31,	70,
    33,	80,
    35,	90,
    37,	99    
};
const DtVNTPWM vnt_pwm1p[_NVNTPWM]={
    10,	17,
    12,	20,
    15,	25,
    20,	32,
    25,	45,
    30,	62,
    40,	90,
    40,	90,
    40,	90    
};
const DtVNTPWM vnt_pwm2p[_NVNTPWM]={
    10,	17,
    12,	20,
    15,	25,
    20,	32,
    25,	42,
    30,	55,
    40,	90,
    40,	90,
    40,	90    
};
const DtVNTPWM vnt_pwm3p[_NVNTPWM]={
    10, 17,
    12, 20,
    15, 25,
    20, 32,
    25, 42,
    30, 52,
    32, 60,
    35, 75,
    40, 90
};
const DtVNTPWM vnt_pwm4p[_NVNTPWM]={
    10, 17,
    12, 20,
    15, 25,
    20, 32,
    25, 42,
    30, 52,
    33, 65,
    35, 75,
    40, 90
};
const DtVNTPWM vnt_pwm5p[_NVNTPWM]={
    10, 17,
    12, 20,
    15, 25,
    20, 30,
    25, 40,
    30, 50,
    35, 70,
    40, 90,
    40, 90
};
//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
void StartPID(void);
void StartMProc(unsigned char pv);
/*---------------------------------------------------------------------------------*/
/*-----------------------------PUBLIC DATA-----------------------------------------*/
//okreslenie typu odbiornika 
#define _FPWM			4			//czestotliwosc przebiegu [kHz]
#define _NEG_PWM		0			//odwrotma logika wyjscia? (max dla pwm=0, min dla pwm=255)
#define _NEG_PWM3		0			//PWM3 odwrotma logika wyjscia? (max dla pwm=0, min dla pwm=255)
#define _MG2_MOD		0xF0		//wartosc PWM przy ktorej osiagniety zostaje pelny wydatek pradu ukladu wyjsciowego
//-----------------------------

//***********************************************************************************
//MINITERM
//---------
//parametry zwiazane z modulem oscylacji
//!UWAGA: zalaczyc modul oscylacji mozna w pliku pwm.tp.h
#define _EN_OSC1 		1			//aktywacja(1)/deaktywacja(0) oscylacji na sygnale PWM
#define _MAX_OSC_PWM1	30			//maksymalna amplituda oscylacji
#define _MIN_OSC_PWM1	5			//minimalna amplituda oscylacji
//---------
//parametry opisujace wlasciwosci modulatora 
#define _MAD_MOD1	0x14			//dolna dozwolona granica mechaniczna warunku ciszy akustycznej przy oscylacjach
#define _MAG_MOD1	0xF7			//gorna dozwolona granica mechaniczna warunku ciszy akustycznej przy oscylacjach
#define _MD_MOD1	0x35			//20 martwy zakres modulacji - dolna granica 
#define _MD_PMOD1	0x40			//martwy zakres modulacji - dolna granica (dla gazu plynnego)
#define _MG_MOD1	0x9F			//C0 martwy zakres modulacji - gorna granica
#define _MG_PMOD1	0xE0			//martwy zakres modulacji - gorna granica (dla gazu plynnego)
#define _I80_MOD1	0x75			//0xA5			//domyslna wartosc pierwszego pobudzenia
#define _I80_PMOD1 	0xA3			//domyslna wartosc pierwszego pobudzenia - gaz plynny

/*
Pierwsze pobudzenie (_I80_MOD lub _I80_PMOD) na poziomie 60% zakresu modulacji
gaz ziemny
(X75-X35)=X40
/			=64/106*100%=60%
(X9f-X35)=X6A
gaz p�ynny
(xE0-x48)*60%+x48=x5B+x48 =xA3

(xA3-x48)=x5B
/			=91/152*100%=60%
(xE0-x48)=x98
*/

#if _MAX_OSC_PWM1<=_MIN_OSC_PWM1
#error Nieprawidlowe wartosci parametrow _MAX_OSC_PWM,_MIN_OSC_PWM
#endif
#if _MG2_MOD<_MG_MOD1||_MG2_MOD<_MG_PMOD1
#error Nieprawidlowe wartosci parametrow _MG2_MOD,_MG_MOD,_MG_PMOD
#endif
#if _MD_MOD1>_MG_MOD1||_MD_PMOD1>_MG_PMOD1
#error Nieprawidlowe wartosci parametrow _MD_MOD,_MD_PMOD,_MG_MOD,_MG_PMOD
#endif
#if _MAD_MOD1>_MAG_MOD1
#error Nieprawidlowe wartosci parametrow _MAD_MOD,_MAG_MOD
#endif
#if _MAD_MOD1>_MD_MOD1||_MAD_MOD1>_MD_PMOD1
#error Nieprawidlowe wartosci parametrow _MAD_MOD,_MD_MOD,_MD_PMOD
#endif
#if _MAG_MOD1<_MG_MOD1||_MAG_MOD1<_MG_PMOD1
#error Nieprawidlowe wartosci parametrow _MAG_MOD,_MG_MOD,_MG_PMOD
#endif
//-----------------------------
//parametry zwiazane z moca startowa
#define _GWMSTART1	99				//graniczna wartosc nastawy mocy startowej
//gaz ziemny
#define _MDSTART1 _MD_MOD1			//dolna granica wartosci PWM dla mocy startowej
#define _MGSTART1 _MG_MOD1+10			//gorna granica wartosci PWM dla mocy startowej
//gaz plynny
#define _MDSTARTP1 _MD_PMOD1			//dolna granica wartosci PWM dla mocy startowej
#define _MGSTARTP1 _MG_PMOD1+10			//gorna granica wartosci PWM dla mocy startowej
//---------
//parametry zwiazane z moca maksymalna CO
#define _GWMMAXCO1	99				//graniczna wartosc nastawy mocy maksymalnej CO
//gaz ziemny
#define _MDMMAXCO1 _MD_MOD1			//dolna granica wartosci PWM dla mocy maksymalnej CO
#define _MGMMAXCO1 _MG_MOD1+10			//gorna granica wartosci PWM dla mocy maksymalnej CO
//gaz plynny
#define _MDMMAXCOP1 _MD_PMOD1			//dolna granica wartosci PWM dla mocy maksymalnej CO
#define _MGMMAXCOP1 _MG_PMOD1+10			//gorna granica wartosci PWM dla mocy maksymalnej CO
//---------
//parametry zwiazane z moca maksymalna CW
#define _GWMMAXCW1	99				//graniczna wartosc nastawy mocy maksymalnej CW
//gaz ziemny
#define _MDMMAXCW1 _MD_MOD1			//dolna granica wartosci PWM dla mocy maksymalnej CW
#define _MGMMAXCW1 _MG_MOD1+10			//gorna granica wartosci PWM dla mocy maksymalnej CW
//gaz plynny
#define _MDMMAXCWP1 _MD_PMOD1			//dolna granica wartosci PWM dla mocy maksymalnej CW
#define _MGMMAXCWP1 _MG_PMOD1+10			//gorna granica wartosci PWM dla mocy maksymalnej CW
//---------

//***********************************************************************************
//MINIMAX, MINIMAX ZASOBNIKOWY
//---------
//parametry zwiazane z modulem oscylacji
//!UWAGA: zalaczyc modul oscylacji mozna w pliku pwm.tp.h
#define _EN_OSC2 		1			//aktywacja(1)/deaktywacja(0) oscylacji na sygnale PWM
#define _MAX_OSC_PWM2	15			//maksymalna amplituda oscylacji
#define _MIN_OSC_PWM2	5			//minimalna amplituda oscylacji
//---------
//parametry opisujace wlasciwosci modulatora
#define _MAD_MOD2	0x14			//dolna dozwolona granica mechaniczna warunku ciszy akustycznej przy oscylacjach
#define _MAG_MOD2	0xF7			//gorna dozwolona granica mechaniczna warunku ciszy akustycznej przy oscylacjach
#define _MD_MOD2	0x2D			//martwy zakres modulacji - dolna granica 
#define _MD_PMOD2	0x4E			//martwy zakres modulacji - dolna granica (dla gazu plynnego)
#define _MG_MOD2	0x9F			//martwy zakres modulacji - gorna granica
#define _MG_PMOD2	0xE0			//martwy zakres modulacji - gorna granica (dla gazu plynnego)
#define _I80_MOD2	0x6E			//60% domyslna wartosc pierwszego pobudzenia
#define _I80_PMOD2 	0xA5			//60% domyslna wartosc pierwszego pobudzenia - gaz plynny

/*
Pierwsze pobudzenie (_I80_MOD lub _I80_PMOD) na poziomie 60% zakresu modulacji
gaz ziemny
(X6E-X2D)=X41
/			=65/114*100%=57%
(X9f-X2D)=X72
gaz p�ynny
(xE0-x4E)*60%+x4E=x57+x4E =xA5

(xA5-x4E)=x57
/			=87/146*100%=60%
(xE0-x4E)=x92
*/

#if _MAX_OSC_PWM2<=_MIN_OSC_PWM2
#error Nieprawidlowe wartosci parametrow _MAX_OSC_PWM,_MIN_OSC_PWM
#endif
#if _MG2_MOD<_MG_MOD2||_MG2_MOD<_MG_PMOD2
#error Nieprawidlowe wartosci parametrow _MG2_MOD,_MG_MOD,_MG_PMOD
#endif
#if _MD_MOD2>_MG_MOD2||_MD_PMOD2>_MG_PMOD2
#error Nieprawidlowe wartosci parametrow _MD_MOD,_MD_PMOD,_MG_MOD,_MG_PMOD
#endif
#if _MAD_MOD2>_MAG_MOD2
#error Nieprawidlowe wartosci parametrow _MAD_MOD,_MAG_MOD
#endif
#if _MAD_MOD2>_MD_MOD2||_MAD_MOD2>_MD_PMOD2
#error Nieprawidlowe wartosci parametrow _MAD_MOD,_MD_MOD,_MD_PMOD
#endif
#if _MAG_MOD2<_MG_MOD2||_MAG_MOD2<_MG_PMOD2
#error Nieprawidlowe wartosci parametrow _MAG_MOD,_MG_MOD,_MG_PMOD
#endif
//-----------------------------
//-----------------------------
//parametry zwiazane z moca startowa
#define _GWMSTART2	99				//graniczna wartosc nastawy mocy startowej
//gaz ziemny
#define _MDSTART2 _MD_MOD2			//dolna granica wartosci PWM dla mocy startowej
#define _MGSTART2 _MG_MOD2+10			//gorna granica wartosci PWM dla mocy startowej
//gaz plynny
#define _MDSTARTP2 _MD_PMOD2			//dolna granica wartosci PWM dla mocy startowej
#define _MGSTARTP2 _MG_PMOD2+10			//gorna granica wartosci PWM dla mocy startowej
//---------
//parametry zwiazane z moca maksymalna CO
#define _GWMMAXCO2	99				//graniczna wartosc nastawy mocy maksymalnej CO
//gaz ziemny
#define _MDMMAXCO2 _MD_MOD2			//dolna granica wartosci PWM dla mocy maksymalnej CO
#define _MGMMAXCO2 _MG_MOD2+10			//gorna granica wartosci PWM dla mocy maksymalnej CO
//gaz plynny
#define _MDMMAXCOP2 _MD_PMOD2			//dolna granica wartosci PWM dla mocy maksymalnej CO
#define _MGMMAXCOP2 _MG_PMOD2+10			//gorna granica wartosci PWM dla mocy maksymalnej CO
//---------
//parametry zwiazane z moca maksymalna CW
#define _GWMMAXCW2	99				//graniczna wartosc nastawy mocy maksymalnej CW
//gaz ziemny
#define _MDMMAXCW2 _MD_MOD2			//dolna granica wartosci PWM dla mocy maksymalnej CW
#define _MGMMAXCW2 _MG_MOD2+10			//gorna granica wartosci PWM dla mocy maksymalnej CW
//gaz plynny
#define _MDMMAXCWP2 _MD_PMOD2			//dolna granica wartosci PWM dla mocy maksymalnej CW
#define _MGMMAXCWP2 _MG_PMOD2+10			//gorna granica wartosci PWM dla mocy maksymalnej CW
//---------
//***********************************************************************************
//UNICO
//---------
//parametry zwiazane z modulem oscylacji
//!UWAGA: zalaczyc modul oscylacji mozna w pliku pwm.tp.h
#define _EN_OSC0 		1			//aktywacja(1)/deaktywacja(0) oscylacji na sygnale PWM
#define _MAX_OSC_PWM0	20			//maksymalna amplituda oscylacji
#define _MIN_OSC_PWM0	5			//minimalna amplituda oscylacji
//---------
//parametry opisujace wlasciwosci modulatora
#define _MAD_MOD0	0x14			//dolna dozwolona granica mechaniczna warunku ciszy akustycznej przy oscylacjach
#define _MAG_MOD0	0xF7			//gorna dozwolona granica mechaniczna warunku ciszy akustycznej przy oscylacjach
#define _MD_MOD0	0x30			//martwy zakres modulacji - dolna granica 
#define _MD_PMOD0	0x48			//martwy zakres modulacji - dolna granica (dla gazu plynnego)
#define _MG_MOD0	0x9F			//martwy zakres modulacji - gorna granica
#define _MG_PMOD0	0xEB			//martwy zakres modulacji - gorna granica (dla gazu plynnego)
#define _I80_MOD0	0x75			//60% domyslna wartosc pierwszego pobudzenia
#define _I80_PMOD0 	0xAA			//60% domyslna wartosc pierwszego pobudzenia - gaz plynny

/*
Pierwsze pobudzenie (_I80_MOD lub _I80_PMOD) na poziomie 60% zakresu modulacji
gaz ziemny
(X75-X30)=X45
/			=69/111*100%=60%
(X9f-X30)=X6F
gaz p�ynny
(xEB-x48)*60%+x48=x62+x48 =xAA
spr:
(xAA-x48)=x62
/			=98/163*100%=60%
(xEB-x48)=xA3
*/

#if _MAX_OSC_PWM0<=_MIN_OSC_PWM0
#error Nieprawidlowe wartosci parametrow _MAX_OSC_PWM,_MIN_OSC_PWM
#endif
#if _MG2_MOD<_MG_MOD0||_MG2_MOD<_MG_PMOD0
#error Nieprawidlowe wartosci parametrow _MG2_MOD,_MG_MOD,_MG_PMOD
#endif
#if _MD_MOD0>_MG_MOD0||_MD_PMOD0>_MG_PMOD0
#error Nieprawidlowe wartosci parametrow _MD_MOD,_MD_PMOD,_MG_MOD,_MG_PMOD
#endif
#if _MAD_MOD0>_MAG_MOD0
#error Nieprawidlowe wartosci parametrow _MAD_MOD,_MAG_MOD
#endif
#if _MAD_MOD0>_MD_MOD0||_MAD_MOD0>_MD_PMOD0
#error Nieprawidlowe wartosci parametrow _MAD_MOD,_MD_MOD,_MD_PMOD
#endif
#if _MAG_MOD0<_MG_MOD0||_MAG_MOD0<_MG_PMOD0
#error Nieprawidlowe wartosci parametrow _MAG_MOD,_MG_MOD,_MG_PMOD
#endif
//-----------------------------
//-----------------------------
//parametry zwiazane z moca startowa
#define _GWMSTART0	99				//graniczna wartosc nastawy mocy startowej
//gaz ziemny
#define _MDSTART0 _MD_MOD0			//dolna granica wartosci PWM dla mocy startowej
#define _MGSTART0 _MG_MOD0+10			//gorna granica wartosci PWM dla mocy startowej
//gaz plynny
#define _MDSTARTP0 _MD_PMOD0			//dolna granica wartosci PWM dla mocy startowej
#define _MGSTARTP0 _MG_PMOD0+10			//gorna granica wartosci PWM dla mocy startowej
//---------
//parametry zwiazane z moca maksymalna CO
#define _GWMMAXCO0	99				//graniczna wartosc nastawy mocy maksymalnej CO
//gaz ziemny
#define _MDMMAXCO0 _MD_MOD0			//dolna granica wartosci PWM dla mocy maksymalnej CO
#define _MGMMAXCO0 _MG_MOD0+10			//gorna granica wartosci PWM dla mocy maksymalnej CO
//gaz plynny
#define _MDMMAXCOP0 _MD_PMOD0			//dolna granica wartosci PWM dla mocy maksymalnej CO
#define _MGMMAXCOP0 _MG_PMOD0+10			//gorna granica wartosci PWM dla mocy maksymalnej CO
//---------
//parametry zwiazane z moca maksymalna CW
#define _GWMMAXCW0	99				//graniczna wartosc nastawy mocy maksymalnej CW
//gaz ziemny
#define _MDMMAXCW0 _MD_MOD0			//dolna granica wartosci PWM dla mocy maksymalnej CW
#define _MGMMAXCW0 _MG_MOD0+10			//gorna granica wartosci PWM dla mocy maksymalnej CW
//gaz plynny
#define _MDMMAXCWP0 _MD_PMOD0			//dolna granica wartosci PWM dla mocy maksymalnej CW
#define _MGMMAXCWP0 _MG_PMOD0+10			//gorna granica wartosci PWM dla mocy maksymalnej CW
//---------
//***********************************************************************************
//MAXITERM
//---------
//parametry zwiazane z modulem oscylacji
#define _EN_OSC3 		1			//aktywacja(1)/deaktywacja(0) oscylacji na sygnale PWM
#define _MAX_OSC_PWM3	25			//maksymalna amplituda oscylacji
#define _MIN_OSC_PWM3	5			//minimalna amplituda oscylacji
//---------
//parametry opisujace wlasciwosci modulatora 
#define _MAD_MOD3	0x14			//dolna dozwolona granica mechaniczna warunku ciszy akustycznej przy oscylacjach
#define _MAG_MOD3	0xF7			//gorna dozwolona granica mechaniczna warunku ciszy akustycznej przy oscylacjach
#define _MD_MOD3	0x30			//martwy zakres modulacji - dolna granica 
#define _MD_PMOD3	0x5A			//5Amartwy zakres modulacji - dolna granica (dla gazu plynnego)
#define _MG_MOD3	0x86			//martwy zakres modulacji - gorna granica
#define _MG_PMOD3	0xCD			//martwy zakres modulacji - gorna granica (dla gazu plynnego)
#define _I80_MOD3	0x6E			//60% domyslna wartosc pierwszego pobudzenia
#define _I80_PMOD3 	0x9B			//domyslna wartosc pierwszego pobudzenia - gaz plynny

/*
Pierwsze pobudzenie (_I80_MOD lub _I80_PMOD) na poziomie 60% zakresu modulacji
gaz ziemny
(x6E-x30)=x36
/			=54/86*100%=60%
(x86-x30)=x56
gaz plynny
(xC7-x5A)*60%+x5A=x41+x5A=x9B
spr:
(x9B-x5A)=x41
/			=65/109*100%=60%
(xC7-x5A)=x6D
*/

#if _MAX_OSC_PWM3<=_MIN_OSC_PWM3
#error Nieprawidlowe wartosci parametrow _MAX_OSC_PWM,_MIN_OSC_PWM
#endif
#if _MG2_MOD<_MG_MOD3||_MG2_MOD<_MG_PMOD3
#error Nieprawidlowe wartosci parametrow _MG2_MOD,_MG_MOD,_MG_PMOD
#endif
#if _MD_MOD3>_MG_MOD3||_MD_PMOD3>_MG_PMOD3
#error Nieprawidlowe wartosci parametrow _MD_MOD,_MD_PMOD,_MG_MOD,_MG_PMOD
#endif
#if _MAD_MOD3>_MAG_MOD3
#error Nieprawidlowe wartosci parametrow _MAD_MOD,_MAG_MOD
#endif
#if _MAD_MOD3>_MD_MOD3||_MAD_MOD3>_MD_PMOD3
#error Nieprawidlowe wartosci parametrow _MAD_MOD,_MD_MOD,_MD_PMOD
#endif
#if _MAG_MOD3<_MG_MOD3||_MAG_MOD3<_MG_PMOD3
#error Nieprawidlowe wartosci parametrow _MAG_MOD,_MG_MOD,_MG_PMOD
#endif
//-----------------------------
//parametry zwiazane z moca startowa
#define _GWMSTART3	99				//graniczna wartosc nastawy mocy startowej
//gaz ziemny
#define _MDSTART3 _MD_MOD3			//dolna granica wartosci PWM dla mocy startowej
#define _MGSTART3 _MG_MOD3+10			//gorna granica wartosci PWM dla mocy startowej
//gaz plynny
#define _MDSTARTP3 _MD_PMOD3			//dolna granica wartosci PWM dla mocy startowej
#define _MGSTARTP3 _MG_PMOD3+10			//gorna granica wartosci PWM dla mocy startowej
//---------
//parametry zwiazane z moca maksymalna CO
#define _GWMMAXCO3	99				//graniczna wartosc nastawy mocy maksymalnej CO
//gaz ziemny
#define _MDMMAXCO3 _MD_MOD3			//dolna granica wartosci PWM dla mocy maksymalnej CO
#define _MGMMAXCO3 _MG_MOD3+10			//gorna granica wartosci PWM dla mocy maksymalnej CO
//gaz plynny
#define _MDMMAXCOP3 _MD_PMOD3			//dolna granica wartosci PWM dla mocy maksymalnej CO
#define _MGMMAXCOP3 _MG_PMOD3+10			//gorna granica wartosci PWM dla mocy maksymalnej CO
//---------
//parametry zwiazane z moca maksymalna CW
#define _GWMMAXCW3	99				//graniczna wartosc nastawy mocy maksymalnej CW
//gaz ziemny
#define _MDMMAXCW3 _MD_MOD3			//dolna granica wartosci PWM dla mocy maksymalnej CW
#define _MGMMAXCW3 _MG_MOD3+10			//gorna granica wartosci PWM dla mocy maksymalnej CW
//gaz plynny
#define _MDMMAXCWP3 _MD_PMOD3			//dolna granica wartosci PWM dla mocy maksymalnej CW
#define _MGMMAXCWP3 _MG_PMOD3+10			//gorna granica wartosci PWM dla mocy maksymalnej CW
//---------
//----------------------------------------------------------
//Maksymalna rozdzielczosc PWM obliczamy ze wzoru
//Max PWM resolution=log(FOSC/FPWM)/log(2) [bits]
//Dla FOSC=8MHz i FPWM=16kHz wynosi ona 9 bitow.


/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
/*-----------------------------PROTECTED DATA--------------------------------------*/

//Maksymalna dozwolona wartosc wypelnienia zalezy od _WPR2
//Nalezy ja obliczyc biorac pod uwage ze wypelnienie jest wartoscia 10 bitowa
#define _WPR2 0x7C //patrz T2PR
#if(_WPR2<0x40)
#define _PSHIFT	2
#define _HI_PWM			(_WPR2+1)<<2	//gorna bezwgledna granica wypelnienia
#endif
#if(_WPR2>0x40 && _WPR2<0x80)
#define _PSHIFT	1
#define _HI_PWM			(_WPR2+1)<<1	//gorna bezwgledna granica wypelnienia
#endif
#if(_WPR2>0x80)
#define _PSHIFT	0
#define _HI_PWM			_WPR2+1			//gorna bezwgledna granica wypelnienia
#endif

//kontrola i formatowanie podanych parametrow
#if (_MAG_MOD1>_HI_PWM)
#define _MAG_MOD1		_HI_PWM
#endif
#if (_MAG_MOD2>_HI_PWM)
#define _MAG_MOD2		_HI_PWM
#endif 
//----------------------------------------------------------
//----------------------------------------------------------
#endif		/*_PWM_H*/
