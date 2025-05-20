/*pwm.c*/
//Procedury zwiazane z modulem CCP (tryb pracy = PWM)
/*---------------------------------------------------------------------------------*/
#include <xc.h>
#include "../../proj.tp.h"

#ifndef _TYP_PRJ
#error NIEOKRESLONY IDENTYFIKATOR PROJEKTU
#endif

#if _TYP_PRJ==3
#advisory _TYP_PRJ==3
#include "../../mcc_generated_files\mcc.h"

#include "global.h"					//parametry globalne
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
/*-----------------------------PUBLIC DATA-----------------------------------------*/
void InitDtaPWM(void)
{
	/*CCP1CON=0;						//reset CCP1 module
	CCPR1L=0;
	CCPR1H=0;
	TMR2=0;
	T2CON=_WT2CON;					//new timer2 prescale value
	PR2=_WPR2;						//PWM period
	TMR2IE=1;						//enable interrupt timer2
	CCP1CON=0b00001100;				//set PWM mode
	TMR2ON=1;						//start timer2
									//nalezy jeszcze ustawic pin CCP1 jako wyjscie
     */
	PWM.BufPWM=0;
	PWM.NewPWM=0;
	PWM.BufOSC=0;
	PWM.NewOSC=0;
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
		PWM._ENOSC=(_EN_OSC1!=0);
		PWM.MDMOD=_MD_MOD1;
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
	}
	else						//gaz plynny
	{
		PWM._ENOSC=(_EN_OSC1!=0);
		PWM.MDMOD=_MD_PMOD1;
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
//Moc startowa (uwaga: wywolywac po ModParam)
//nst - parametr z zakresu od 0 do _GWMSTART
void MocStart(unsigned char nst)
{
	unsigned int tmp;

	if(nst>PWM.GWMSTART) nst=PWM.GWMSTART;
	tmp=((PWM.MGSTART-PWM.MDSTART)*10)/PWM.GWMSTART;
	PWM.MSTART=PWM.MDSTART+(unsigned char)((nst*tmp)/10);	
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
/*---------------------------------------------------------------------------------*/
#endif
