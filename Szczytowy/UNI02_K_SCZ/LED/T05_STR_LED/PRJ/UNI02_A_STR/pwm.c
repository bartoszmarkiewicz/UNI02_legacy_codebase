/*pwm.c*/
//Procedury zwiazane z modulem CCP (tryb pracy = PWM)
/*---------------------------------------------------------------------------------*/
#include <xc.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>

#include "../../proj.tp.h"

#ifndef _TYP_PRJ
#error NIEOKRESLONY IDENTYFIKATOR PROJEKTU
#endif

#if _TYP_PRJ==2
#advisory _TYP_PRJ==2 
#include "../../mcc_generated_files\mcc.h"

#include "global.h"
#include "main.h"
#include "global.ex.h"

#include "pwm.h"					//parametry lokalne

DtPWM PWM;							//czeste operacje na danych
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
static inline void InPWM1_LoadDutyValue(uint16_t dutyValue)
{
    dutyValue &= 0x03FF;
    
    // Load duty cycle value
    if(CCP1CONbits.FMT)
    {
        dutyValue <<= 6;
        CCPR1H = dutyValue >> 8;
        CCPR1L = dutyValue;
    }
    else
    {
        CCPR1H = dutyValue >> 8;
        CCPR1L = dutyValue;
    }
}
/*---------------------------------------------------------------------------------*/
static inline void InPWM2_LoadDutyValue(uint16_t dutyValue)
{
    dutyValue &= 0x03FF;
    
    // Load duty cycle value
    if(CCP2CONbits.FMT)
    {
        dutyValue <<= 6;
        CCPR2H = dutyValue >> 8;
        CCPR2L = dutyValue;
    }
    else
    {
        CCPR2H = dutyValue >> 8;
        CCPR2L = dutyValue;
    }
}
/*---------------------------------------------------------------------------------*/
/*-----------------------------PUBLIC DATA-----------------------------------------*/
void InitDtaPWM(void)
{
/*
	//pwm1 (modulator)
	CCP1CON=0;						//reset CCP1 module
	CCPR1L=0;
	CCPR1H=0;
	CCP1CON=0b00001100;				//set PWM mode

	//pwm2 (pompa)
	CCP2CON=0;						//reset CCP1 module
	CCPR2L=0;
	CCPR2H=0;
	CCP2CON=0b00001100;				//set PWM mode, single output

	//timer2
	TMR2=0;
	T2CON=_WT2CON;					//new timer2 prescale value
	PR2=_WPR2;						//PWM period
	TMR2IE=1;						//enable interrupt timer2
	TMR2ON=1;						//start timer2
									//nalezy jeszcze ustawic pin CCP1 jako wyjscie
*/
#if(_WPR2<0x40)
#if(((_WPR2+1)<<2)<0xFF)
    PWM.HIPWM = (_WPR2 + 1) << 2; //gorna bezwgledna granica wypelnienia
#else
    PWM.HIPWM = 0xFF; //gorna bezwgledna granica wypelnienia
#endif
#endif
#if(_WPR2>0x40 && _WPR2<0x80)
#if(((_WPR2+1)<<1)<0xFF)
    PWM.HIPWM = (_WPR2 + 1) << 1; //gorna bezwgledna granica wypelnienia
#else
    PWM.HIPWM = 0xFF; //gorna bezwgledna granica wypelnienia
#endif
#endif
#if(_WPR2>0x80)
#if((_WPR2+1)<0xFF)
    PWM.HIPWM = (_WPR2 + 1); //gorna bezwgledna granica wypelnienia
#else
    PWM.HIPWM = 0xFF; //gorna bezwgledna granica wypelnienia
#endif
#endif

	PWM.BufPWM=0;
	PWM.NewPWM=0;
	PWM.BufOSC=0;
	PWM.NewOSC=0;

	PWM.BufPWM2=0;
	PWM.NewPWM2=0;
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Podstawowe parametry modulatora (uwaga: wywolywoac gdy wybrano typ kotla)
//tpk - typ kotla
//tpg - rodzaj gazu
void ModParam(const unsigned char tpk,const unsigned char tpg)
{
	PWM.tpGZ=(tpg!=0);

	//parametry zalezne od typu kotla i wybranego rodzaju gazu
	if(!PWM.tpGZ) 				//gaz ziemny
	{
		switch(tpk)		//typ kotla
		{
			case _UNICO:		//jednofunkcyjny
			{
				PWM._ENOSC=(_EN_OSC0!=0);
				PWM.MDMOD=_MD_MOD0;
				PWM.DKNF_MDMOD=PWM.MDMOD; //domyslna wartosc minimum
				PWM.MGMOD=_MG_MOD0;
				PWM.MAXOSC=_MAX_OSC_PWM0;
				PWM.MINOSC=_MIN_OSC_PWM0;
				PWM.MAD_MOD=_MAD_MOD0;
				PWM.MAG_MOD=_MAG_MOD0;
				PWM.GWMSTART=_GWMSTART0;
				PWM.MDSTART=_MDSTART0;
				PWM.MGSTART=_MGSTART0;
				PWM.GWMMAXCO=_GWMMAXCO0;
				PWM.MDMAXCO=_MDMMAXCO0;
				PWM.MGMAXCO=_MGMMAXCO0;
				PWM.GWMMAXCW=_GWMMAXCW0;
				PWM.MDMAXCW=_MDMMAXCW0;
				PWM.MGMAXCW=_MGMMAXCW0;
				PWM.FIRSTPB=_I80_MOD0;
				PWM.PROP1=_MG_MOD0-1*((_MG_MOD0-_MD_MOD0)/7);
				PWM.PROP2=_MG_MOD0-2*((_MG_MOD0-_MD_MOD0)/7);
				PWM.PROP3=_MG_MOD0-3*((_MG_MOD0-_MD_MOD0)/7);

				break;
			}
			case _BITERM:		//bitermiczny (miniterm)
			{
				PWM._ENOSC=(_EN_OSC1!=0);
				PWM.MDMOD=_MD_MOD1;
				PWM.DKNF_MDMOD=PWM.MDMOD; //domyslna wartosc minimum
				PWM.MGMOD=_MG_MOD1;
				PWM.MAXOSC=_MAX_OSC_PWM1;
				PWM.MINOSC=_MIN_OSC_PWM1;
				PWM.MAD_MOD=_MAD_MOD1;
				PWM.MAG_MOD=_MAG_MOD1;
				PWM.GWMSTART=_GWMSTART1;
				PWM.MDSTART=_MDSTART1;
				PWM.MGSTART=_MGSTART1;
				PWM.GWMMAXCO=_GWMMAXCO1;
				PWM.MDMAXCO=_MDMMAXCO1;
				PWM.MGMAXCO=_MGMMAXCO1;
				PWM.GWMMAXCW=_GWMMAXCW1;
				PWM.MDMAXCW=_MDMMAXCW1;
				PWM.MGMAXCW=_MGMMAXCW1;
				PWM.FIRSTPB=_I80_MOD1;
				PWM.PROP1=_MG_MOD1-1*((_MG_MOD1-_MD_MOD1)/7);
				PWM.PROP2=_MG_MOD1-2*((_MG_MOD1-_MD_MOD1)/7);
				PWM.PROP3=_MG_MOD1-3*((_MG_MOD1-_MD_MOD1)/7);
				break;
			}
/*
			case _MONOTERM:		//monotermiczny
			case _ZASOBNIK:		//zasobnikowy
			{
				PWM._ENOSC=(_EN_OSC2!=0);
				PWM.MDMOD=_MD_MOD2;
				PWM.DKNF_MDMOD=PWM.MDMOD; //domyslna wartosc minimum
				PWM.MGMOD=_MG_MOD2;
				PWM.MAXOSC=_MAX_OSC_PWM2;
				PWM.MINOSC=_MIN_OSC_PWM2;
				PWM.MAD_MOD=_MAD_MOD2;
				PWM.MAG_MOD=_MAG_MOD2;
				PWM.GWMSTART=_GWMSTART2;
				PWM.MDSTART=_MDSTART2;
				PWM.MGSTART=_MGSTART2;
				PWM.GWMMAXCO=_GWMMAXCO2;
				PWM.MDMAXCO=_MDMMAXCO2;
				PWM.MGMAXCO=_MGMMAXCO2;
				PWM.GWMMAXCW=_GWMMAXCW2;
				PWM.MDMAXCW=_MDMMAXCW2;
				PWM.MGMAXCW=_MGMMAXCW2;
				PWM.FIRSTPB=_I80_MOD2;
				PWM.PROP1=_MG_MOD2-1*((_MG_MOD2-_MD_MOD2)/7);
				PWM.PROP2=_MG_MOD2-2*((_MG_MOD2-_MD_MOD2)/7);
				PWM.PROP3=_MG_MOD2-3*((_MG_MOD2-_MD_MOD2)/7);
				break;
			}
*/
			case _BITERM2:		//bitermiczny (maxiterm)
			{
				PWM._ENOSC=(_EN_OSC3!=0);
				PWM.MDMOD=_MD_MOD3;
				PWM.MGMOD=_MG_MOD3;
				PWM.DKNF_MDMOD=PWM.MDMOD; //domyslna wartosc minimum
				PWM.MAXOSC=_MAX_OSC_PWM3;
				PWM.MINOSC=_MIN_OSC_PWM3;
				PWM.MAD_MOD=_MAD_MOD3;
				PWM.MAG_MOD=_MAG_MOD3;
				PWM.GWMSTART=_GWMSTART3;
				PWM.MDSTART=_MDSTART3;
				PWM.MGSTART=_MGSTART3;
				PWM.GWMMAXCO=_GWMMAXCO3;
				PWM.MDMAXCO=_MDMMAXCO3;
				PWM.MGMAXCO=_MGMMAXCO3;
				PWM.GWMMAXCW=_GWMMAXCW3;
				PWM.MDMAXCW=_MDMMAXCW3;
				PWM.MGMAXCW=_MGMMAXCW3;
				PWM.FIRSTPB=_I80_MOD3;
				PWM.PROP1=_MG_MOD3-1*((_MG_MOD3-_MD_MOD3)/7);
				PWM.PROP2=_MG_MOD3-2*((_MG_MOD3-_MD_MOD3)/7);
				PWM.PROP3=_MG_MOD3-3*((_MG_MOD3-_MD_MOD3)/7);
				break;
			}
			case _MONOHB:		//monotermiczny z hydroblokiem (minimax hb)
			{
				PWM._ENOSC=(_EN_OSC4!=0);
				PWM.MDMOD=_MD_MOD4;
				PWM.DKNF_MDMOD=PWM.MDMOD; //domyslna wartosc minimum
				PWM.MGMOD=_MG_MOD4;
				PWM.MAXOSC=_MAX_OSC_PWM4;
				PWM.MINOSC=_MIN_OSC_PWM4;
				PWM.MAD_MOD=_MAD_MOD4;
				PWM.MAG_MOD=_MAG_MOD4;
				PWM.GWMSTART=_GWMSTART4;
				PWM.MDSTART=_MDSTART4;
				PWM.MGSTART=_MGSTART4;
				PWM.GWMMAXCO=_GWMMAXCO4;
				PWM.MDMAXCO=_MDMMAXCO4;
				PWM.MGMAXCO=_MGMMAXCO4;
				PWM.GWMMAXCW=_GWMMAXCW4;
				PWM.MDMAXCW=_MDMMAXCW4;
				PWM.MGMAXCW=_MGMMAXCW4;
				PWM.FIRSTPB=_I80_MOD4;
				PWM.PROP1=_MG_MOD4-1*((_MG_MOD4-_MD_MOD4)/7);
				PWM.PROP2=_MG_MOD4-2*((_MG_MOD4-_MD_MOD4)/7);
				PWM.PROP3=_MG_MOD4-3*((_MG_MOD4-_MD_MOD4)/7);
				break;
			}
			default:			//domyslny monotermiczny
			{
				PWM._ENOSC=(_EN_OSC4!=0);
				PWM.MDMOD=_MD_MOD4;
				PWM.DKNF_MDMOD=PWM.MDMOD; //domyslna wartosc minimum
				PWM.MGMOD=_MG_MOD4;
				PWM.MAXOSC=_MAX_OSC_PWM4;
				PWM.MINOSC=_MIN_OSC_PWM4;
				PWM.MAD_MOD=_MAD_MOD4;
				PWM.MAG_MOD=_MAG_MOD4;
				PWM.GWMSTART=_GWMSTART4;
				PWM.MDSTART=_MDSTART4;
				PWM.MGSTART=_MGSTART4;
				PWM.GWMMAXCO=_GWMMAXCO4;
				PWM.MDMAXCO=_MDMMAXCO4;
				PWM.MGMAXCO=_MGMMAXCO4;
				PWM.GWMMAXCW=_GWMMAXCW4;
				PWM.MDMAXCW=_MDMMAXCW4;
				PWM.MGMAXCW=_MGMMAXCW4;
				PWM.FIRSTPB=_I80_MOD4;
				PWM.PROP1=_MG_MOD4-1*((_MG_MOD4-_MD_MOD4)/7);
				PWM.PROP2=_MG_MOD4-2*((_MG_MOD4-_MD_MOD4)/7);
				PWM.PROP3=_MG_MOD4-3*((_MG_MOD4-_MD_MOD4)/7);
				break;
			}
		}
	}
	else						//gaz plynny
	{
		switch(tpk)		//typ kotla
		{
			case _UNICO:		//jednofunkcyjny
			{
				PWM._ENOSC=(_EN_OSC0!=0);
				PWM.MDMOD=_MD_PMOD0;
				PWM.DKNF_MDMOD=PWM.MDMOD; //domyslna wartosc minimum
				PWM.MGMOD=_MG_PMOD0;
				PWM.MAXOSC=_MAX_OSC_PWM0;
				PWM.MINOSC=_MIN_OSC_PWM0;
				PWM.MAD_MOD=_MAD_MOD0;
				PWM.MAG_MOD=_MAG_MOD0;
				PWM.GWMSTART=_GWMSTART0;
				PWM.MDSTART=_MDSTARTP0;
				PWM.MGSTART=_MGSTARTP0;
				PWM.GWMMAXCO=_GWMMAXCO0;
				PWM.MDMAXCO=_MDMMAXCOP0;
				PWM.MGMAXCO=_MGMMAXCOP0;
				PWM.GWMMAXCW=_GWMMAXCW0;
				PWM.MDMAXCW=_MDMMAXCWP0;
				PWM.MGMAXCW=_MGMMAXCWP0;
				PWM.FIRSTPB=_I80_PMOD0;
				PWM.PROP1=_MG_PMOD0-1*((_MG_PMOD0-_MD_PMOD0)/7);
				PWM.PROP2=_MG_PMOD0-2*((_MG_PMOD0-_MD_PMOD0)/7);
				PWM.PROP3=_MG_PMOD0-3*((_MG_PMOD0-_MD_PMOD0)/7);
				break;
			}
			case _BITERM:		//bitermiczny (miniterm)
			{
				PWM._ENOSC=(_EN_OSC1!=0);
				PWM.MDMOD=_MD_PMOD1;
				PWM.DKNF_MDMOD=PWM.MDMOD; //domyslna wartosc minimum
				PWM.MGMOD=_MG_PMOD1;
				PWM.MAXOSC=_MAX_OSC_PWM1;
				PWM.MINOSC=_MIN_OSC_PWM1;
				PWM.MAD_MOD=_MAD_MOD1;
				PWM.MAG_MOD=_MAG_MOD1;
				PWM.GWMSTART=_GWMSTART1;
				PWM.MDSTART=_MDSTARTP1;
				PWM.MGSTART=_MGSTARTP1;
				PWM.GWMMAXCO=_GWMMAXCO1;
				PWM.MDMAXCO=_MDMMAXCOP1;
				PWM.MGMAXCO=_MGMMAXCOP1;
				PWM.GWMMAXCW=_GWMMAXCW1;
				PWM.MDMAXCW=_MDMMAXCWP1;
				PWM.MGMAXCW=_MGMMAXCWP1;
				PWM.FIRSTPB=_I80_PMOD1;
				PWM.PROP1=_MG_PMOD1-1*((_MG_PMOD1-_MD_PMOD1)/7);
				PWM.PROP2=_MG_PMOD1-2*((_MG_PMOD1-_MD_PMOD1)/7);
				PWM.PROP3=_MG_PMOD1-3*((_MG_PMOD1-_MD_PMOD1)/7);
				break;
			}
/*
			case _MONOTERM:		//monotermiczny
			case _ZASOBNIK:		//zasobnikowy
			{
				PWM._ENOSC=(_EN_OSC2!=0);
				PWM.DKNF_MDMOD=PWM.MDMOD; //domyslna wartosc minimum
				PWM.MDMOD=_MD_PMOD2;
				PWM.MGMOD=_MG_PMOD2;
				PWM.MAXOSC=_MAX_OSC_PWM2;
				PWM.MINOSC=_MIN_OSC_PWM2;
				PWM.MAD_MOD=_MAD_MOD2;
				PWM.MAG_MOD=_MAG_MOD2;
				PWM.GWMSTART=_GWMSTART2;
				PWM.MDSTART=_MDSTARTP2;
				PWM.MGSTART=_MGSTARTP2;
				PWM.GWMMAXCO=_GWMMAXCO2;
				PWM.MDMAXCO=_MDMMAXCOP2;
				PWM.MGMAXCO=_MGMMAXCOP2;
				PWM.GWMMAXCW=_GWMMAXCW2;
				PWM.MDMAXCW=_MDMMAXCWP2;
				PWM.MGMAXCW=_MGMMAXCWP2;
				PWM.FIRSTPB=_I80_PMOD2;
				PWM.PROP1=_MG_PMOD2-1*((_MG_PMOD2-_MD_PMOD2)/7);
				PWM.PROP2=_MG_PMOD2-2*((_MG_PMOD2-_MD_PMOD2)/7);
				PWM.PROP3=_MG_PMOD2-3*((_MG_PMOD2-_MD_PMOD2)/7);
				break;
			}
*/
			case _BITERM2:		//bitermiczny (maxiterm)
			{
				PWM._ENOSC=(_EN_OSC3!=0);
				PWM.MDMOD=_MD_PMOD3;
				PWM.DKNF_MDMOD=PWM.MDMOD; //domyslna wartosc minimum
				PWM.MGMOD=_MG_PMOD3;
				PWM.MAXOSC=_MAX_OSC_PWM3;
				PWM.MINOSC=_MIN_OSC_PWM3;
				PWM.MAD_MOD=_MAD_MOD3;
				PWM.MAG_MOD=_MAG_MOD3;
				PWM.GWMSTART=_GWMSTART3;
				PWM.MDSTART=_MDSTARTP3;
				PWM.MGSTART=_MGSTARTP3;
				PWM.GWMMAXCO=_GWMMAXCO3;
				PWM.MDMAXCO=_MDMMAXCOP3;
				PWM.MGMAXCO=_MGMMAXCOP3;
				PWM.GWMMAXCW=_GWMMAXCW3;
				PWM.MDMAXCW=_MDMMAXCWP3;
				PWM.MGMAXCW=_MGMMAXCWP3;
				PWM.FIRSTPB=_I80_PMOD3;
				PWM.PROP1=_MG_PMOD3-1*((_MG_PMOD3-_MD_PMOD3)/7);
				PWM.PROP2=_MG_PMOD3-2*((_MG_PMOD3-_MD_PMOD3)/7);
				PWM.PROP3=_MG_PMOD3-3*((_MG_PMOD3-_MD_PMOD3)/7);
				break;
			}
			case _MONOHB:	//monotermiczny z hydroblokiem (minimax hb)
			{
				PWM._ENOSC=(_EN_OSC4!=0);
				PWM.MDMOD=_MD_PMOD4;
				PWM.DKNF_MDMOD=PWM.MDMOD; //domyslna wartosc minimum
				PWM.MGMOD=_MG_PMOD4;
				PWM.MAXOSC=_MAX_OSC_PWM4;
				PWM.MINOSC=_MIN_OSC_PWM4;
				PWM.MAD_MOD=_MAD_MOD4;
				PWM.MAG_MOD=_MAG_MOD4;
				PWM.GWMSTART=_GWMSTART4;
				PWM.MDSTART=_MDSTARTP4;
				PWM.MGSTART=_MGSTARTP4;
				PWM.GWMMAXCO=_GWMMAXCO4;
				PWM.MDMAXCO=_MDMMAXCOP4;
				PWM.MGMAXCO=_MGMMAXCOP4;
				PWM.GWMMAXCW=_GWMMAXCW4;
				PWM.MDMAXCW=_MDMMAXCWP4;
				PWM.MGMAXCW=_MGMMAXCWP4;
				PWM.FIRSTPB=_I80_PMOD4;
				PWM.PROP1=_MG_PMOD4-1*((_MG_PMOD4-_MD_PMOD4)/7);
				PWM.PROP2=_MG_PMOD4-2*((_MG_PMOD4-_MD_PMOD4)/7);
				PWM.PROP3=_MG_PMOD4-3*((_MG_PMOD4-_MD_PMOD4)/7);
				break;
			}
			default:			//domyslny monotermiczny
			{
				PWM._ENOSC=(_EN_OSC4!=0);
				PWM.MDMOD=_MD_PMOD4;
				PWM.DKNF_MDMOD=PWM.MDMOD; //domyslna wartosc minimum
				PWM.MGMOD=_MG_PMOD4;
				PWM.MAXOSC=_MAX_OSC_PWM4;
				PWM.MINOSC=_MIN_OSC_PWM4;
				PWM.MAD_MOD=_MAD_MOD4;
				PWM.MAG_MOD=_MAG_MOD4;
				PWM.GWMSTART=_GWMSTART4;
				PWM.MDSTART=_MDSTARTP4;
				PWM.MGSTART=_MGSTARTP4;
				PWM.GWMMAXCO=_GWMMAXCO4;
				PWM.MDMAXCO=_MDMMAXCOP4;
				PWM.MGMAXCO=_MGMMAXCOP4;
				PWM.GWMMAXCW=_GWMMAXCW4;
				PWM.MDMAXCW=_MDMMAXCWP4;
				PWM.MGMAXCW=_MGMMAXCWP4;
				PWM.FIRSTPB=_I80_PMOD4;
				PWM.PROP1=_MG_PMOD4-1*((_MG_PMOD4-_MD_PMOD4)/7);
				PWM.PROP2=_MG_PMOD4-2*((_MG_PMOD4-_MD_PMOD4)/7);
				PWM.PROP3=_MG_PMOD4-3*((_MG_PMOD4-_MD_PMOD4)/7);
				break;
			}
		}
	}	
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Zapisz wartosc do PWM bez formatowania (amplituda oscylacji=0).
void WritePWM(const unsigned char tpwm)
{
	PWM._ENPWM=0;
	PWM.BufPWM=tpwm;
	PWM.BufOSC=0;
	PWM._ENPWM=1;
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Zapisz wartosc do PWM2
void WritePWM2(const unsigned char tpwm)
{
	PWM._ENPWM2=0;
	PWM.BufPWM2=tpwm;
	PWM._ENPWM2=1;
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Zapisz wartosc do PWM z formatowaniem (oblicza wartosc amplitudy oscylacji).
void WriteOPWM(unsigned char tpwm)
{
	unsigned char aosc;
	unsigned int tmp;

	//kontrola czy wartosc PWM miesci sie w oczekiwanym zakresie
#if _SRVTRB==1	
	if(PWM._ENOSC)
	{												//aktywny tryb serwisowy kompilacji?
		if(tpwm<PWM.MDMOD) tpwm=PWM.MDMOD;
	}
#else
	if(tpwm<PWM.MDMOD) tpwm=PWM.MDMOD;
#endif
	if(tpwm>_MG2_MOD) tpwm=_MG2_MOD;
	if(PWM._ENOSC)
	{
		//obliczenie amplitudy osculacji
		if(tpwm>=PWM.MGMOD) aosc=PWM.MINOSC;
		else
		{
			tmp=(tpwm-PWM.MDMOD)*100;
			tmp/=(PWM.MGMOD-PWM.MDMOD);
			tmp*=(PWM.MAXOSC-PWM.MINOSC);
			tmp/=100;
			if(tmp>PWM.MAXOSC) aosc=PWM.MINOSC;
			else aosc=PWM.MAXOSC-(unsigned char)tmp;	//amplituda oscylacji maleje ze wzrostem wartosci PWM
		}
	//---------
		if(aosc>(PWM.MAG_MOD-tpwm)) aosc=PWM.MAG_MOD-tpwm;
		else
		if(aosc>(tpwm-PWM.MAD_MOD)) aosc=tpwm-PWM.MAD_MOD;
	}
//---------
	//zapamietanie obliczonych wartosci
	PWM._ENPWM=0;
	PWM.BufPWM=tpwm;
	if(PWM._ENOSC) PWM.BufOSC=aosc;
	else PWM.BufOSC=0;
	PWM._ENPWM=1;
}

/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Przelicza wydatek gazu na wartosc procentowa 
unsigned char ModToProc(unsigned char wrt)
{
	unsigned long tmp;
	if(wrt<PWM.MDMOD) return 0;
	if(wrt>PWM.MGMOD) return 99;
	tmp=(wrt-PWM.MDMOD)*100;
	tmp/=(PWM.MGMOD-PWM.MDMOD);
	if(tmp>=99) return 99;
	return (unsigned char)tmp;	
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Moc minimalna (uwaga: wywolywac po ModParam)
//nst - parametr z zakresu od 0 do 99
unsigned char MocProc(unsigned char nst)
{
	unsigned int tmp;

	if(nst>99) nst=99;
	tmp=((PWM.MGMOD-PWM.MDMOD)*10)/99;
	return (PWM.MDMOD+(unsigned char)((nst*tmp)/10));	
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Moc startowa (uwaga: wywolywac po ModParam)
//nst - parametr z zakresu od 0 do _GWMSTART
void MocStart(unsigned char nst)
{
	unsigned int tmp;

	if(nst>PWM.GWMSTART) nst=PWM.GWMSTART;
	tmp=((PWM.MGSTART-PWM.MDSTART)*10)/PWM.GWMSTART;
	PWM.MSTART=PWM.MDSTART+(unsigned char)((nst*tmp)/10);

	//casowa moc minimalna nie mniejsza od 25% i nie wieksza od 50%
	PWM.DV16_MDMOD=MocProc(25);								//czasowa wartosc minimum
	if(PWM.MSTART>PWM.DV16_MDMOD)
	{	
		PWM.DV16_MDMOD=MocProc(50);						//czasowa wartosc minimum
		if(PWM.MSTART<PWM.DV16_MDMOD) PWM.DV16_MDMOD=PWM.MSTART;
	}
	if(PWM.DV16_MDMOD<PWM.MDMOD) PWM.DV16_MDMOD=PWM.MDMOD;	
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Moc maksymalna wpdy grzewczej (uwaga: wywolywac po ModParam)
//nst - parametr z zakresu od 0 do _GWMMAXCO
void MocMaksCO(unsigned char nst)
{
	unsigned int tmp;

	if(nst>PWM.GWMMAXCO) nst=PWM.GWMMAXCO;
	tmp=((PWM.MGMAXCO-PWM.MDMAXCO)*10)/PWM.GWMMAXCO;
	PWM.MMAXCO=PWM.MDMAXCO+(unsigned char)((nst*tmp)/10);	
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Moc maksymalna wpdy uzytkowej (uwaga: wywolywac po ModParam) 
//nst - parametr z zakresu od 0 do _GWMMAXCW
void MocMaksCW(unsigned char nst)
{
	unsigned int tmp;

	if(nst>PWM.GWMMAXCW) nst=PWM.GWMMAXCW;
	tmp=((PWM.MGMAXCW-PWM.MDMAXCW)*10)/PWM.GWMMAXCW;
	PWM.MMAXCW=PWM.MDMAXCW+(unsigned char)((nst*tmp)/10);
}
/*---------------------------------------------------------------------------------*/
/*-----------------------------PROTECTED DATA--------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Modyfikacja stanu wyjscia PWM
//UWAGA - procedura wykonywana w przerwaniu
/*if((TMR2IF)&&(TMR2IE))
	{
		TMR2IF=0;
		IntPwm();			//nowe wypelnienie PWM
	}*/
void inline IntPWM(void)
{
	static bit btOsc=0;

	if(PWM._ENPWM)					//nowa wartosc PWM gotowa do pobrania?
	{
		PWM.NewPWM=PWM.BufPWM;
		PWM.NewOSC=PWM.BufOSC;
	}
	if(PWM._ENOSC)					//aktywny modul oscylacji?
	{

		if(!btOsc)
		{
			btOsc=1;
			PWM.NewPWM-=PWM.NewOSC;
		}
		else
		{
			btOsc=0;
			PWM.NewPWM+=PWM.NewOSC;
		}
	}

	//if(PWM.NewPWM>_HI_PWM) PWM.NewPWM=_HI_PWM;
    if(PWM.NewPWM>0xFF) PWM.NewPWM=0xFF;
    InPWM1_LoadDutyValue((uint16_t)(PWM.NewPWM<<1));
/*	
#if _PSHIFT>=2
	DC1B1=((PWM.NewPWM&0b00000010)>0)?1:0;
	DC1B0=((PWM.NewPWM&0b00000001)>0)?1:0;
	CCPR1L=PWM.NewPWM>>2;
#endif
#if _PSHIFT==1
	DC1B1=((PWM.NewPWM&0b00000001)>0)?1:0;
	DC1B0=0;
	CCPR1L=PWM.NewPWM>>1;
#endif
#if _PSHIFT==0
	DC1B1=0;
	DC1B0=0;
	CCPR1L=PWM.NewPWM;
#endif
*/
}

//---------------------------------------
//PWM2 - pompa
void inline IntPWM2(void)
{
	if(PWM._ENPWM2)					//nowa wartosc PWM gotowa do pobrania?
	{
		PWM.NewPWM2=PWM.BufPWM2;
	}
	//if(PWM.NewPWM2>PWM.HIPWM) PWM.NewPWM2=PWM.HIPWM;
    if(PWM.NewPWM2>0xFF) PWM.NewPWM2=0xFF;
#if _NEG_PWM2==1
	PWM.NewPWM2=~PWM.NewPWM2;
#endif	
    InPWM2_LoadDutyValue((uint16_t)(PWM.NewPWM2<<1));
/*
#if _PSHIFT>=2
	DC2B1=((PWM.NewPWM2&0b00000010)>0)?1:0;
	DC2B0=((PWM.NewPWM2&0b00000001)>0)?1:0;
	CCPR2L=PWM.NewPWM2>>2;
#endif
#if _PSHIFT==1
	DC2B1=((PWM.NewPWM2&0b00000001)>0)?1:0;
	DC2B0=0;
	CCPR2L=PWM.NewPWM2>>1;
#endif
#if _PSHIFT==0
	DC2B1=0;
	DC2B0=0;
	CCPR2L=PWM.NewPWM2;
#endif
 */
}
/*---------------------------------------------------------------------------------*/
#endif
