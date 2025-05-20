
/*pwm.h*/
//modulator SITA
#ifndef	_PWM_H
#define	_PWM_H
#include "pwm.tp.h"
#include "konfig.h"

#define _NVNTPWM 14  //9
#define _NPWMVNT 9

#define _CHR_PWM 0
#define _EN_PID 1
#define _VPID 1//0
//-------------------------------------------------------
const DtPWMVNT r1_r6[_NPWMVNT]={
    20,	16, //0m
    20,	16, //1m
    20,	16, //2m
    20,	16, //3m
    18,	17, //4m
    15,	19, //5m
    13,	19, //6m
    13,	19, //7m
    10,	20  //8m  
};


//GRZEJNIK NISKOMOCOWY
//Zaleznosc predkosci wentylatora [obr/min*100] od wydatku na modulatorze [%] dla roznych dlugosci komina [MPPWM,VNT]
//Gaz ziemny
const DtPWMVNT pwm_vnt1[_NPWMVNT]={
    00,	12,
    39,	20,
    60,	25,
    68,	30,
    99,	35,
    99,	35,
    99,	35,
    99,	35,
    99,	35  
};
const DtPWMVNT pwm_vnt2[_NPWMVNT]={
    00,	12,
    39,	20,
    54,	25,
    68,	30,
    99,	35,
    99,	35,
    99,	35,
    99,	35,
    99,	35   
};
const DtPWMVNT pwm_vnt3[_NPWMVNT]={
    00,	12,
    37,	20,
    54,	25,
    61,	30,
    99,	35,
    99,	35,
    99,	35,
    99,	35,
    99,	35
};
const DtPWMVNT pwm_vnt4[_NPWMVNT]={
    00,	12,
    35,	20,
    49,	25,
    60,	30,
    99,	35,
    99,	35,
    99,	35,
    99,	35,
    99,	35
};
const DtPWMVNT pwm_vnt5[_NPWMVNT]={
    00, 12,
    31, 20,
    49,	25,
    57,	30,
    99,	35,
    99,	35,
    99,	35,
    99,	35,
    99,	35
};
const DtPWMVNT pwm_vnt6[_NPWMVNT]={
    00,	12,
    32,	20,
    44,	25,
    55,	30,
    99,	35,
    99,	35,
    99,	35,
    99,	35,
    99,	35        
};
const DtPWMVNT pwm_vnt7[_NPWMVNT]={
    00,	12,
    31,	20,
    44,	25,
    55,	30,
    99,	35,
    99,	35,
    99,	35,
    99,	35,
    99,	35
};
const DtPWMVNT pwm_vnt8[_NPWMVNT]={
    00,	12,
    25, 20,
    42,	25,
    50,	30,
    99,	35,
    99,	35,
    99,	35,
    99,	35,
    99,	35
};
//-------------------------------------------------------
//Gaz plynny (tabela 1)
const DtPWMVNT pwm_vnt1p[_NPWMVNT]={
    00,	12,
    38,	20,
    49,	25,
    69,	30,
    99,	35,
    99,	35,
    99,	35,
    99,	35,
    99,	35  
};
const DtPWMVNT pwm_vnt2p[_NPWMVNT]={
    00,	12,
    34,	20,
    46,	25,
    63,	30,
    99,	35,
    99,	35,
    99,	35,
    99,	35,
    99,	35   
};
const DtPWMVNT pwm_vnt3p[_NPWMVNT]={
    00,	12,
    34,	20,
    49,	25,
    79,	30,
    99,	35,
    99,	35,
    99,	35,
    99,	35,
    99,	35
};
const DtPWMVNT pwm_vnt4p[_NPWMVNT]={
    00,	12,
    31,	20,
    42,	25,
    56,	30,
    99,	35,
    99,	35,
    99,	35,
    99,	35,
    99,	35
};
const DtPWMVNT pwm_vnt5p[_NPWMVNT]={
    00, 12,
    30, 20,
    40,	25,
    52,	30,
    99,	35,
    99,	35,
    99,	35,
    99,	35,
    99,	35
};
const DtPWMVNT pwm_vnt6p[_NPWMVNT]={
    00,	12,
    32,	20,
    40,	25,
    53,	30,
    99,	35,
    99,	35,
    99,	35,
    99,	35,
    99,	35        
};
const DtPWMVNT pwm_vnt7p[_NPWMVNT]={
    00,	12,
    29,	20,
    40,	25,
    48,	30,
    99,	35,
    99,	35,
    99,	35,
    99,	35,
    99,	35
};
const DtPWMVNT pwm_vnt8p[_NPWMVNT]={
    00,	12,
    29, 20,
    40,	25,
    48,	30,
    99,	35,
    99,	35,
    99,	35,
    99,	35,
    99,	35
};
//-------------------------------------------------------
//GRZEJNIK WYSOKOMOCOWY
//Zaleznosc predkosci wentylatora [obr/min*100] od wydatku na modulatorze [%] dla roznych dlugosci komina [MPPWM,VNT]
//Gaz ziemny
const DtPWMVNT Hpwm_vnt1[_NPWMVNT]={
    00,	12,
    15,	12,
    25,	14,
    34,	17,
    45,	22,
    53,	24,
    60,	27,
    70,	32,
    99,	35  
};
const DtPWMVNT Hpwm_vnt2[_NPWMVNT]={
    00,	12,
    15,	12,
    25,	14,
    33,	17,
    45,	22,
    53,	24,
    60,	27,
    70,	32,
    99,	35   
};
const DtPWMVNT Hpwm_vnt3[_NPWMVNT]={
    00,	12,
    15,	12,
    25,	15,
    32,	17,
    45,	22,
    53,	24,
    60,	27,
    70,	34,
    99,	35
};
const DtPWMVNT Hpwm_vnt4[_NPWMVNT]={
    00,	12,
    15,	12,
    25,	15,
    33,	18,
    45,	22,
    53,	25,
    60,	28,
    70,	32,
    99,	35
};
const DtPWMVNT Hpwm_vnt5[_NPWMVNT]={
    00, 12,
    13, 12,
    16,	14,
    25,	17,
    34,	22,
    45,	24,
    52,	27,
    70,	35,
    99,	35
};
const DtPWMVNT Hpwm_vnt6[_NPWMVNT]={
    00, 12,
    13, 12,
    16,	14,
    25,	17,
    34,	22,
    45,	24,
    52,	27,
    70,	35,
    99,	35        
};
const DtPWMVNT Hpwm_vnt7[_NPWMVNT]={
    00, 12,
    13, 12,
    18,	14,
    26,	17,
    34,	22,
    45,	24,
    52,	27,
    70,	35,
    99,	35 
};
const DtPWMVNT Hpwm_vnt8[_NPWMVNT]={
    00, 12,
    13, 12,
    15,	14,
    25,	17,
    33,	21,
    45,	24,
    53,	28,
    70,	35,
    99,	35 
};
//-------------------------------------------------------
//Gaz plynny
const DtPWMVNT Hpwm_vnt1p[_NPWMVNT]={
    00,	10,
    17,	12,
    23,	15,
    39,	21,
    50,	25,
    54,	29,
    99,	30,
    99,	35,
    99,	35  
};
const DtPWMVNT Hpwm_vnt2p[_NPWMVNT]={
    00,	10,
    16,	13,
    23,	16,
    35,	19,
    50,	25,
    54,	29,
    99,	30,
    99,	35,
    99,	35   
};
const DtPWMVNT Hpwm_vnt3p[_NPWMVNT]={
    00,	10,
    16,	13,
    20,	16,
    33,	20,
    43,	26,
    49,	29,
    54,	30,
    99,	32,
    99,	35
};
const DtPWMVNT Hpwm_vnt4p[_NPWMVNT]={
    00,	10,
    15,	13,
    20,	16,
    33,	20,
    43,	26,
    49,	29,
    54,	33,
    99,	35,
    99,	35
};
const DtPWMVNT Hpwm_vnt5p[_NPWMVNT]={
    00, 10,
    13, 13,
    30,	20,
    45,	26,
    49,	29,
    54,	33,
    99,	35,
    99,	35,
    99,	35
};
const DtPWMVNT Hpwm_vnt6p[_NPWMVNT]={
    00,	10,
    13,	13,
    20,	16,
    30,	20,
    43,	26,
    49,	30,
    55,	33,
    99,	35,
    99,	35        
};
const DtPWMVNT Hpwm_vnt7p[_NPWMVNT]={
    00,	10,
    15,	13,
    20,	16,
    32,	20,
    43,	26,
    48,	30,
    53,	33,
    99,	35,
    99,	35
};
const DtPWMVNT Hpwm_vnt8p[_NPWMVNT]={
    00,	10,
    14, 13,
    20,	16,
    32,	20,
    43,	26,
    48,	30,
    52,	33,
    99,	35,
    99,	35
};
//-------------------------------------------------------
//-------------------------------------------------------
//OPORY KOMINOWE
//-------------------------------------------------------
//Wypelnienie PWM wentylatora [%] dla oczekiwanej predkosci wentylatora [obr/min*100] dla roznych dlugosci komina [VNT,PPWM]
//Gaz ziemny
const DtVNTPWM vnt_pwm0[_NVNTPWM]={
    10,	17,
    12,	22,
    14,	23,
    17,	28,
    18,	29,
    20,	31,
    22,	36,
    24,	41,
    27,	48, 
    28,	51,
    30,	54,
    32,	64,             
    34,	72,
    35,	76                   
};
const DtVNTPWM vnt_pwm1[_NVNTPWM]={
    10,	17,
    12,	22,
    14,	23,
    17,	28,
    18,	29,
    20,	31,
    22,	36,
    24,	41,
    27,	48, 
    28,	51,
    30,	54,
    32,	64,             
    34,	72,
    35,	76   
};
const DtVNTPWM vnt_pwm2[_NVNTPWM]={
    10,	17,
    12,	22,
    14,	23,
    17,	27,
    18,	29,
    20,	31,
    22,	36,
    24,	40,
    27,	47, 
    28,	50,
    30,	53,
    32,	63,             
    34,	71,
    35,	75    
};
const DtVNTPWM vnt_pwm3[_NVNTPWM]={
    10,	17,
    12,	22,
    14,	23,
    17,	27,
    18,	29,
    20,	30,
    22,	36,
    24,	40,
    27,	47, 
    28,	50,
    30,	52,
    32,	62,             
    34,	70,
    35,	74
};
const DtVNTPWM vnt_pwm4[_NVNTPWM]={
    10,	17,
    12,	22,
    14,	23,
    17,	27,
    18,	29,
    20,	30,
    22,	35,
    24,	39,
    27,	46, 
    28,	49,
    30,	52,
    32,	61,             
    34,	69,
    35,	73
};
const DtVNTPWM vnt_pwm5[_NVNTPWM]={
    10,	17,
    12,	22,
    14,	23,
    17,	27,
    18,	29,
    20,	30,
    22,	35,
    24,	39,
    27,	46, 
    28,	48,
    30,	51,
    32,	60,             
    34,	68,
    35,	71
};
const DtVNTPWM vnt_pwm6[_NVNTPWM]={
    10,	17,
    12,	22,
    14,	23,
    17,	27,
    18,	29,
    20,	30,
    22,	36,
    24,	39,
    27,	46, 
    28,	48,
    30,	51,
    32,	60,             
    34,	67,
    35,	70
};
const DtVNTPWM vnt_pwm7[_NVNTPWM]={
    10,	17,
    12,	22,
    14,	23,
    17,	27,
    18,	29,
    20,	30,
    22,	36,
    24,	39,
    27,	45, 
    28,	48,
    30,	51,
    32,	59,             
    34,	66,
    35,	70
};
const DtVNTPWM vnt_pwm8[_NVNTPWM]={
    10,	17,
    12,	22,
    14,	23,
    17,	27,
    18,	29,
    20,	30,
    22,	37,
    24,	39,
    27,	45, 
    28,	48,
    30,	51,
    32,	58,             
    34,	65,
    35,	70
};
//-------------------------------------------------------
//Gaz plynny
const DtVNTPWM vnt_pwm0p[_NVNTPWM]={
    10,	17,
    12,	22,
    14,	23,
    17,	28,
    18,	29,
    20,	31,
    22,	36,
    24,	41,
    27,	48, 
    28,	51,
    30,	54,
    32,	64,             
    34,	72,
    35,	76                   
};
const DtVNTPWM vnt_pwm1p[_NVNTPWM]={
    10,	17,
    12,	22,
    14,	23,
    17,	28,
    18,	29,
    20,	31,
    22,	36,
    24,	41,
    27,	48, 
    28,	51,
    30,	54,
    32,	64,             
    34,	72,
    35,	76   
};
const DtVNTPWM vnt_pwm2p[_NVNTPWM]={
    10,	17,
    12,	22,
    14,	23,
    17,	27,
    18,	29,
    20,	31,
    22,	36,
    24,	40,
    27,	47, 
    28,	50,
    30,	53,
    32,	63,             
    34,	71,
    35,	75    
};
const DtVNTPWM vnt_pwm3p[_NVNTPWM]={
    10,	17,
    12,	22,
    14,	23,
    17,	27,
    18,	29,
    20,	30,
    22,	36,
    24,	40,
    27,	47, 
    28,	50,
    30,	52,
    32,	62,             
    34,	70,
    35,	74
};
const DtVNTPWM vnt_pwm4p[_NVNTPWM]={
    10,	17,
    12,	22,
    14,	23,
    17,	27,
    18,	29,
    20,	30,
    22,	35,
    24,	39,
    27,	46, 
    28,	49,
    30,	52,
    32,	61,             
    34,	69,
    35,	73
};
const DtVNTPWM vnt_pwm5p[_NVNTPWM]={
    10,	17,
    12,	22,
    14,	23,
    17,	27,
    18,	29,
    20,	30,
    22,	35,
    24,	39,
    27,	46, 
    28,	48,
    30,	51,
    32,	60,             
    34,	68,
    35,	71
};
const DtVNTPWM vnt_pwm6p[_NVNTPWM]={
    10,	17,
    12,	22,
    14,	23,
    17,	27,
    18,	29,
    20,	30,
    22,	36,
    24,	39,
    27,	46, 
    28,	48,
    30,	51,
    32,	60,             
    34,	67,
    35,	70
};
const DtVNTPWM vnt_pwm7p[_NVNTPWM]={
    10,	17,
    12,	22,
    14,	23,
    17,	27,
    18,	29,
    20,	30,
    22,	36,
    24,	39,
    27,	45, 
    28,	48,
    30,	51,
    32,	59,             
    34,	66,
    35,	70
};
const DtVNTPWM vnt_pwm8p[_NVNTPWM]={
    10,	17,
    12,	22,
    14,	23,
    17,	27,
    18,	29,
    20,	30,
    22,	37,
    24,	39,
    27,	45, 
    28,	48,
    30,	51,
    32,	58,             
    34,	65,
    35,	70
};
/*
//Gaz ziemny
const DtVNTPWM vnt_pwm0[_NVNTPWM]={
    10,	20,
    14,	30,
    17,	40,
    20,	50,
    21,	60,
    22,	70,
    24,	80,
    25,	90,
    27,	99    
};
const DtVNTPWM vnt_pwm1[_NVNTPWM]={
    14,	20,
    21,	30,
    27,	40,
    32,	50,
    37,	60,
    40,	70,
    42,	80,
    45,	90,
    48,	99    
};
const DtVNTPWM vnt_pwm2[_NVNTPWM]={
    14,	20,
    21,	30,
    28,	40,
    32,	50,
    37,	60,
    40,	70,
    42,	80,
    45,	90,
    50,	99    
};
const DtVNTPWM vnt_pwm3[_NVNTPWM]={
    14, 20,
    22, 30,
    28, 40,
    32, 50,
    37, 60,
    40, 70,
    44, 80,
    47, 90,
    50, 99
};
const DtVNTPWM vnt_pwm4[_NVNTPWM]={
    14, 20,
    21, 30,
    27, 40,
    32, 50,
    37, 60,
    40, 70,
    44, 80,
    47, 90,
    50, 99
};
const DtVNTPWM vnt_pwm5[_NVNTPWM]={
    14, 20,
    21, 30,
    28, 40,
    32, 50,
    37, 60,
    40, 70,
    44, 80,
    47, 90,
    50, 99
};
const DtVNTPWM vnt_pwm6[_NVNTPWM]={
    14, 20,
    21, 30,
    28, 40,
    32, 50,
    37, 60,
    40, 70,
    44, 80,
    47, 90,
    50, 99
};
const DtVNTPWM vnt_pwm7[_NVNTPWM]={
    14, 20,
    21, 30,
    28, 40,
    32, 50,
    37, 60,
    40, 70,
    44, 80,
    47, 90,
    50, 99
};
const DtVNTPWM vnt_pwm8[_NVNTPWM]={
    14, 20,
    21, 30,
    28, 40,
    32, 50,
    37, 60,
    40, 70,
    44, 80,
    47, 90,
    50, 99
};
//-------------------------------------------------------
//Gaz plynny
const DtVNTPWM vnt_pwm0p[_NVNTPWM]={
    10,	20,
    14,	30,
    17,	40,
    20,	50,
    21,	60,
    22,	70,
    24,	80,
    25,	90,
    27,	99    
};
const DtVNTPWM vnt_pwm1p[_NVNTPWM]={
    14,	20,
    21,	30,
    27,	40,
    32,	50,
    37,	60,
    40,	70,
    42,	80,
    45,	90,
    48,	99    
};
const DtVNTPWM vnt_pwm2p[_NVNTPWM]={
    14,	20,
    21,	30,
    28,	40,
    32,	50,
    37,	60,
    40,	70,
    42,	80,
    45,	90,
    50,	99    
};
const DtVNTPWM vnt_pwm3p[_NVNTPWM]={
    14, 20,
    22, 30,
    28, 40,
    32, 50,
    37, 60,
    40, 70,
    44, 80,
    47, 90,
    50, 99
};
const DtVNTPWM vnt_pwm4p[_NVNTPWM]={
    14, 20,
    21, 30,
    27, 40,
    32, 50,
    37, 60,
    40, 70,
    44, 80,
    47, 90,
    50, 99
};
const DtVNTPWM vnt_pwm5p[_NVNTPWM]={
    14, 20,
    21, 30,
    28, 40,
    32, 50,
    37, 60,
    40, 70,
    44, 80,
    47, 90,
    50, 99
};
const DtVNTPWM vnt_pwm6p[_NVNTPWM]={
    14, 20,
    21, 30,
    28, 40,
    32, 50,
    37, 60,
    40, 70,
    44, 80,
    47, 90,
    50, 99
};
const DtVNTPWM vnt_pwm7p[_NVNTPWM]={
    14, 20,
    21, 30,
    28, 40,
    32, 50,
    37, 60,
    40, 70,
    44, 80,
    47, 90,
    50, 99
};
const DtVNTPWM vnt_pwm8p[_NVNTPWM]={
    14, 20,
    21, 30,
    28, 40,
    32, 50,
    37, 60,
    40, 70,
    44, 80,
    47, 90,
    50, 99
};*/
//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
void StartPID(const unsigned char tp);
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
gaz p³ynny
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
gaz p³ynny
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
gaz p³ynny
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
