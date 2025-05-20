
/*pwm.h*/
//modulator SITA
//Weryfikacja zapisów 04.04.2022
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
//UWAGA
//Przy zmianach nastaw nalezy uwzglednic podniesienie mocy minimalnej i wybieg na mocy ustalonej po wyjsciu z procedury rozruvhowej
//Patrz procedura SterownikVNT w pliku pwm.c
//-------------------------------------------------------
//GRZEJNIK NISKOMOCOWY
//Domyslna zaleznosc startowej predkosci wentylatora [obr/min*100] od startowego wydatku na modulatorze [%] dla roznych dlugosci komina [PPWM,VNT]
//UWAGA: wartosci domyslne przywracane sa automatycznie po zmianie parametrow: P7 lub P8
//const DtPWMVNT r1_r6[_NPWMVNT]={
//    40,	10, //0m
//    40,	10, //1m
//    40,	10, //2m
//    40,	10, //3m
//    40,	10, //4m
//    40,	10, //5m
//    40,	10, //6m
//    40,	10, //7m
//    40,	10  //8m  
//};
//(01.04.2022 zmiana nastaw)
const DtPWMVNT r1_r6[_NPWMVNT]={
    30,	11, //0m
    30,	11, //1m
    30,	11, //2m
    30,	11, //3m
    30,	11, //4m
    30,	11, //5m
    30,	11, //6m
    30,	11, //7m
    30,	11  //8m  
};
const DtPWMVNT r1_r6p[_NPWMVNT]={
    40,	10, //0m
    40,	10, //1m
    40,	10, //2m
    40,	10, //3m
    40,	10, //4m
    40,	10, //5m
    40,	10, //6m
    40,	10, //7m
    40,	10  //8m
};
//(29.03.2022 zmiana nastaw)
const DtPWMVNT r1_r6p2[_NPWMVNT]={
    20,	10, //0m
    20,	10, //1m
    20,	10, //2m
    20,	10, //3m
    20,	10, //4m
    20,	10, //5m
    20,	10, //6m
    20,	10, //7m
    20,	10  //8m 
};
//GRZEJNIK WYSOKOMOCOWY
//Domyslna zaleznosc startowej predkosci wentylatora [obr/min*100] od startowego wydatku na modulatorze [%] dla roznych dlugosci komina [PPWM,VNT]
//UWAGA: wartosci domyslne przywracane sa automatycznie po zmianie parametrow: P7 lub P8
//const DtPWMVNT Hr1_r6[_NPWMVNT]={
//    30,	10, //0m
//    30,	10, //1m
//    30,	10, //2m
//    30,	10, //3m
//    30,	10, //4m
//    30,	10, //5m
//    30,	10, //6m
//    30,	10, //7m
//    30,	10  //8m   
//};
//(01.04.2022 zmiana nastaw)
const DtPWMVNT Hr1_r6[_NPWMVNT]={
    30,	11, //0m
    30,	11, //1m
    30,	11, //2m
    30,	11, //3m
    30,	11, //4m
    30,	11, //5m
    30,	11, //6m
    30,	11, //7m
    30,	11  //8m   
};
const DtPWMVNT Hr1_r6p[_NPWMVNT]={
    30,	10, //0m
    30,	10, //1m
    30,	10, //2m
    30,	10, //3m
    30,	10, //4m
    30,	10, //5m
    30,	10, //6m
    30,	10, //7m
    30,	10  //8m  
};

const DtPWMVNT Hr1_r6p2[_NPWMVNT]={
    30,	10, //0m
    30,	10, //1m
    30,	10, //2m
    30,	10, //3m
    30,	10, //4m
    30,	10, //5m
    30,	10, //6m
    30,	10, //7m
    30,	10  //8m 
};
//----------------------------------------------------------------------------------------------------------------------------------------------------------
//GRZEJNIK NISKOMOCOWY
//Zaleznosc predkosci wentylatora [obr/min*100] od wydatku na modulatorze [%] dla roznych dlugosci komina [MPPWM,VNT]
//Gaz ziemny (GT1903eco 2e g20 prot. 80/20)
//Protokó? nr 80-20 (ostatni zapis zmian 16.03.2022)
const DtPWMVNT pwm_vnt1[_NPWMVNT]={
    00,	10,
    24, 12,
    40, 14,
    48, 17,
    55,	20,
    65,	21,
    70,	22,
    99,	24,
    99,	24  
};
const DtPWMVNT pwm_vnt2[_NPWMVNT]={
    00,	10,
    24, 12,
    40, 14,
    48, 17,
    55,	20,
    65,	21,
    70,	22,
    99,	24,
    99,	24  
};
const DtPWMVNT pwm_vnt3[_NPWMVNT]={
    00,	10,
    24, 12,
    37, 14,
    48, 17,
    55,	20,
    65,	21,
    73,	24,
    99,	26,
    99,	26
};
const DtPWMVNT pwm_vnt4[_NPWMVNT]={
    00,	10,
    24, 12,
    37, 14,
    48, 17,
    55,	20,
    65,	21,
    73,	24,
    99,	26,
    99,	26
};
const DtPWMVNT pwm_vnt5[_NPWMVNT]={
    00,	11,
    24, 14,
    30, 17,
    50, 19,
    55,	21,
    65,	23,
    70,	26,
    99,	28,
    99,	28
};
const DtPWMVNT pwm_vnt6[_NPWMVNT]={
    00,	11,
    24, 14,
    30, 17,
    50, 19,
    55,	21,
    65,	23,
    70,	26,
    99,	28,
    99,	28        
};
const DtPWMVNT pwm_vnt7[_NPWMVNT]={
    00,	11,
    19, 14,
    30, 17,
    45, 20,
    55,	22,
    66,	25,
    75,	28,
    99,	30,
    99,	30
};
const DtPWMVNT pwm_vnt8[_NPWMVNT]={
    00,	11,
    19, 14,
    30, 17,
    45, 20,
    55,	22,
    66,	25,
    75,	28,
    99,	30,
    99,	30
};
//-------------------------------------------------------
//Gaz plynny (tabela 1)
//Gaz propan (GT1903eco 3p g31 prot. 82/20)
//Protokol nr 82-20 (ostatni zapis zmian 16.03.2022)
const DtPWMVNT pwm_vnt1p[_NPWMVNT]={
    00,	10,
    28, 11,
    35, 13,
    45, 16,
    51,	18,
    57,	22,
    66,	23,
    99,	24,
    99,	24  
};
const DtPWMVNT pwm_vnt2p[_NPWMVNT]={
    00,	10,
    28, 11,
    35, 13,
    45, 16,
    51,	18,
    57,	22,
    66,	23,
    99,	24,
    99,	24     
};
const DtPWMVNT pwm_vnt3p[_NPWMVNT]={
    00,	11,
    29, 13,
    35, 15,
    45, 17,
    51,	20,
    57,	22,
    66,	23,
    99,	24,
    99,	24    
};
const DtPWMVNT pwm_vnt4p[_NPWMVNT]={
    00,	11,
    29, 13,
    40, 16,
    45, 18,
    50,	20,
    59,	22,
    65,	23,
    99,	26,
    99,	26    
};
const DtPWMVNT pwm_vnt5p[_NPWMVNT]={
    00,	12,
    23, 14,
    39, 16,
    43, 19,
    50,	23,
    57,	26,
    61,	27,
    99,	28,
    99,	28 
};
const DtPWMVNT pwm_vnt6p[_NPWMVNT]={
    00,	12,
    23, 14,
    39, 16,
    43, 19,
    50,	23,
    57,	26,
    61,	27,
    99,	28,
    99,	28        
};
const DtPWMVNT pwm_vnt7p[_NPWMVNT]={
    00,	12,
    23, 14,
    39, 16,
    43, 19,
    50,	23,
    57,	26,
    61,	27,
    99,	28,
    99,	28 
};
const DtPWMVNT pwm_vnt8p[_NPWMVNT]={
    00,	12,
    23, 14,
    39, 16,
    43, 19,
    50,	23,
    57,	26,
    61,	27,
    99,	28,
    99,	28 
};

//-------------------------------------------------------
//Gaz plynny (tabela 2)
//Gaz propan-butan (GT1903eco 3bp g30 prot. 48/21)
//Protokó? nr 48-21 (ostatni zapis zmian 16.03.2022)
const DtPWMVNT pwm_vnt1p2[_NPWMVNT]={
    00,	10,
    23,	11,
    30,	12,
    39,	15,
    46,	16,
    99,	17,
    99,	17,
    99,	17,
    99,	17  
};
const DtPWMVNT pwm_vnt2p2[_NPWMVNT]={
    00,	10,
    23,	11,
    30,	12,
    39,	15,
    46,	16,
    99,	17,
    99,	17,
    99,	17,
    99,	17   
};
const DtPWMVNT pwm_vnt3p2[_NPWMVNT]={
    00,	10,
    23,	11,
    30,	12,
    39,	15,
    46,	16,
    99,	17,
    99,	17,
    99,	17,
    99,	17
};
const DtPWMVNT pwm_vnt4p2[_NPWMVNT]={
    00,	10,
    23,	11,
    30,	12,
    39,	15,
    46,	16,
    99,	17,
    99,	17,
    99,	17,
    99,	17
};
const DtPWMVNT pwm_vnt5p2[_NPWMVNT]={
    00,	10,
    15,	11,
    29,	12,
    30,	15,
    36,	16,
    42,	17,
    99,	18,
    99,	18,
    99,	18
};
const DtPWMVNT pwm_vnt6p2[_NPWMVNT]={
    00,	10,
    15,	12,
    23,	14,
    27,	16,
    37,	18,
    42,	19,
    99,	20,
    99,	20,
    99,	20        
};
const DtPWMVNT pwm_vnt7p2[_NPWMVNT]={
    00,	10,
    15,	12,
    23,	14,
    27,	16,
    37,	18,
    42,	19,
    99,	20,
    99,	20,
    99,	20 
};
const DtPWMVNT pwm_vnt8p2[_NPWMVNT]={
    00,	10,
    15,	12,
    23,	14,
    27,	16,
    37,	18,
    42,	19,
    99,	20,
    99,	20,
    99,	20
};
//-------------------------------------------------------
//GRZEJNIK WYSOKOMOCOWY
//Zaleznosc predkosci wentylatora [obr/min*100] od wydatku na modulatorze [%] dla roznych dlugosci komina [MPPWM,VNT]

//Gaz ziemny (GT2403eco 2e g20 prot. 02/21)
//Protokol nr 02-21 (ostatni zapis zmian 21.03.2022)
const DtPWMVNT Hpwm_vnt1[_NPWMVNT]={
    00,	10,
    14,	11,
    20,	12,
    33,	14,
    50,	18,
    60,	22,
    75,	24,
    99,	26,
    99,	26  
};
const DtPWMVNT Hpwm_vnt2[_NPWMVNT]={
    00,	10,
    14,	11,
    20,	12,
    33,	14,
    50,	18,
    60,	22,
    75,	24,
    99,	26,
    99,	26    
};
const DtPWMVNT Hpwm_vnt3[_NPWMVNT]={
    00,	10,
    14,	11,
    20,	12,
    33,	14,
    50,	18,
    60,	22,
    75,	24,
    99,	26,
    99,	26    
};
const DtPWMVNT Hpwm_vnt4[_NPWMVNT]={
    00,	11,
    10,	13,
    22,	16,
    30,	18,
    46,	22,
    60,	26,
    68,	28,
    99,	30,
    99,	30   
};
const DtPWMVNT Hpwm_vnt5[_NPWMVNT]={
    00,	11,
    10,	13,
    22,	16,
    30,	18,
    46,	22,
    60,	26,
    68,	28,
    99,	30,
    99,	30 
};
const DtPWMVNT Hpwm_vnt6[_NPWMVNT]={
    00,	11,
    10,	13,
    22,	16,
    30,	18,
    46,	22,
    60,	26,
    68,	28,
    99,	30,
    99,	30      
};
const DtPWMVNT Hpwm_vnt7[_NPWMVNT]={
    00,	11,
    10,	13,
    22,	16,
    30,	18,
    46,	22,
    60,	26,
    68,	28,
    99,	30,
    99,	30 
};
const DtPWMVNT Hpwm_vnt8[_NPWMVNT]={
    00,	11,
    10,	13,
    22,	16,
    30,	18,
    46,	22,
    60,	26,
    68,	28,
    99,	30,
    99,	30 
};

//const DtPWMVNT Hpwm_vnt8[_NPWMVNT]={
//    00,	10,
//    8,	11,
//    10,	13,
//    25,	16,
//    30,	18,
//    35,	20,
//    44,	23,
//    53,	26,
//    99,	35 
//};
//-------------------------------------------------------
//Gaz plynny (tabela 1)
//Gaz propan (GT2403eco 3p g31 prot. 81/20)
//Protokol nr 81-20 (ostatni zapis zmian 21.03.2022)
const DtPWMVNT Hpwm_vnt1p[_NPWMVNT]={
    00,	10,
    19, 11,
    26, 13,
    38, 16,
    43,	18,
    48,	20,
    55,	23,
    99,	26,
    99,	26  
};
const DtPWMVNT Hpwm_vnt2p[_NPWMVNT]={
    00,	10,
    19, 11,
    26, 13,
    38, 16,
    43,	18,
    48,	20,
    55,	23,
    99,	26,
    99,	26 
};
const DtPWMVNT Hpwm_vnt3p[_NPWMVNT]={
    00,	10,
    19, 11,
    26, 13,
    38, 16,
    43,	18,
    48,	20,
    55,	23,
    99,	26,
    99,	26 
};
const DtPWMVNT Hpwm_vnt4p[_NPWMVNT]={
    00,	10,
    15, 11,
    22, 13,
    32, 16,
    38,	18,
    40,	20,
    43,	23,
    99,	26,
    99,	26 
};
const DtPWMVNT Hpwm_vnt5p[_NPWMVNT]={
    00,	10,
    15, 11,
    22, 13,
    32, 16,
    38,	18,
    40,	22,
    43,	26,
    99,	28,
    99,	28  
};
const DtPWMVNT Hpwm_vnt6p[_NPWMVNT]={
    00,	10,
    15, 11,
    22, 13,
    32, 16,
    38,	18,
    40,	22,
    43,	26,
    99,	28,
    99,	28       
};
const DtPWMVNT Hpwm_vnt7p[_NPWMVNT]={
    00,	11,
    10, 17,
    20, 20,
    33, 23,
    36,	26,
    43,	28,
    99,	30,
    99,	30,
    99,	30 
};
const DtPWMVNT Hpwm_vnt8p[_NPWMVNT]={
    00,	11,
    10, 17,
    20, 20,
    33, 23,
    36,	26,
    43,	28,
    99,	30,
    99,	30,
    99,	30
};

//Gaz plynny (tabela 2)
//Gaz propan-butan (GT2403eco 3bp g30 prot. 79/20)
//Protokol nr 79-20 (ostatni zapis zmian 21.03.2022)
const DtPWMVNT Hpwm_vnt1p2[_NPWMVNT]={
    00,	10,
    15,	11,
    25,	12,
    36,	15,
    41,	17,
    46,	18,
    52,	19,
    99,	20,
    99,	20  
};
const DtPWMVNT Hpwm_vnt2p2[_NPWMVNT]={
    00,	10,
    15,	11,
    25,	12,
    36,	14,
    41,	16,
    46,	18,
    52,	19,
    99,	20,
    99,	20    
};
const DtPWMVNT Hpwm_vnt3p2[_NPWMVNT]={
    00,	10,
    15,	11,
    25,	12,
    36,	14,
    41,	16,
    46,	18,
    52,	19,
    99,	20,
    99,	20  
};
const DtPWMVNT Hpwm_vnt4p2[_NPWMVNT]={
    00,	10,
    16,	11,
    22,	13,
    32,	15,
    38,	17,
    42,	19,
    48,	21,
    99,	22,
    99,	22 
};
const DtPWMVNT Hpwm_vnt5p2[_NPWMVNT]={
    00,	10,
    16,	11,
    22,	13,
    32,	15,
    38,	17,
    42,	19,
    48,	21,
    99,	22,
    99,	22 
};
const DtPWMVNT Hpwm_vnt6p2[_NPWMVNT]={
    00,	10,
    16,	11,
    22,	13,
    32,	16,
    38,	18,
    42,	20,
    48,	22,
    99,	24,
    99,	24     
};
const DtPWMVNT Hpwm_vnt7p2[_NPWMVNT]={
    00,	10,
    16,	11,
    22,	13,
    32,	16,
    38,	18,
    42,	20,
    48,	22,
    99,	24,
    99,	24
};
const DtPWMVNT Hpwm_vnt8p2[_NPWMVNT]={
    00,	10,
    11,	11,
    15,	13,
    25,	16,
    31,	18,
    35,	20,
    46,	22,
    99,	24,
    99,	24
};
//-------------------------------------------------------
//-------------------------------------------------------
//OPORY KOMINOWE
//-------------------------------------------------------
//GRZEJNIK NISKOMOCOWY
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
//GRZEJNIK WYSOKOMOCOWY
const DtVNTPWM Hvnt_pwm0[_NVNTPWM]={
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
    34,	66,//72
    35,	67//76                   
};
const DtVNTPWM Hvnt_pwm1[_NVNTPWM]={
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
    34,	66,//72
    35,	67 //76  
};
const DtVNTPWM Hvnt_pwm2[_NVNTPWM]={
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
    34,	65, //71
    35,	66  //75  
};
const DtVNTPWM Hvnt_pwm3[_NVNTPWM]={
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
    34,	64,//70
    35,	65//74
};
const DtVNTPWM Hvnt_pwm4[_NVNTPWM]={
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
    34,	64,//69
    35,	65//73
};
const DtVNTPWM Hvnt_pwm5[_NVNTPWM]={
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
    34,	63,//68
    35,	65//71
};
const DtVNTPWM Hvnt_pwm6[_NVNTPWM]={
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
    34,	63,//67
    35,	65//70
};
const DtVNTPWM Hvnt_pwm7[_NVNTPWM]={
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
    34,	62,//66
    35,	65//70
};
const DtVNTPWM Hvnt_pwm8[_NVNTPWM]={
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
    34,	62,//65
    35,	65//70
};

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
