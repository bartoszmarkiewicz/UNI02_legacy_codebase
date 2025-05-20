/*pwm.c*/
//Procedury zwiazane z modulem CCP (tryb pracy = PWM)
/*---------------------------------------------------------------------------------*/
#include <pic18.h>
#include "global.h"					//parametry globalne
#include "pwm.h"					//parametry lokalne
#include "konfig.h"

DtPWM PWM;							//czeste operacje na danych
extern DtKONF DtKNF;
extern unsigned char x,kll,hll;
/*---------------------------------------------------------------------------------*/
/*-----------------------------PUBLIC DATA-----------------------------------------*/
void InitPWM(void)
{
//pwm1 (modulator,vnt)
	CCP1CON=0;						//reset CCP1 module
	CCPR1L=0;
	CCPR1H=0;
	CCP1CON=0b00001100;				//set PWM mode, single output
	STRA=0;
	STRB=0;
	STRC=0;
	STRD=1;							//aktywne yjscie PWM na RD7 (vnt)
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

#if(_WPR2<0x40)
	#if(((_WPR2+1)<<2)<0xFF)
		PWM.HIPWM=(_WPR2+1)<<2;			//gorna bezwgledna granica wypelnienia
	#else
		PWM.HIPWM=0xFF;					//gorna bezwgledna granica wypelnienia
	#endif
#endif
#if(_WPR2>0x40 && _WPR2<0x80)
	#if(((_WPR2+1)<<1)<0xFF)
		PWM.HIPWM=(_WPR2+1)<<1;			//gorna bezwgledna granica wypelnienia
	#else
		PWM.HIPWM=0xFF;					//gorna bezwgledna granica wypelnienia
	#endif
#endif
#if(_WPR2>0x80)
	#if((_WPR2+1)<0xFF)
		PWM.HIPWM=(_WPR2+1);			//gorna bezwgledna granica wypelnienia		
	#else
		PWM.HIPWM=0xFF;					//gorna bezwgledna granica wypelnienia
	#endif
#endif
	
	PWM.BufPWM=0;
	PWM.NewPWM=0;
	PWM.BufPWM2=0;
	PWM.NewPWM2=0;
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Podstawowe parametry modulatora (uwaga: wywolywoac gdy wybrano typ kotla)
//Liczy kluczowe parametry modulatora
void ModParam(void)
{
    switch(DtKNF.rfan)  //rodzaj wentylatora
    {
        case 0: //wentylator FIME
        {
            PWM.VLO_PWM=_VLO_PWM0;
            break;
        }
        case 1: //wentylator NG
        {
            PWM.VLO_PWM=_VLO_PWM1;            
            break;
        }
        default:
        {
            PWM.VLO_PWM=_VLO_PWM0;  //domyslny FIME           
        }
    }
	PWM.MGMOD=CalkVNTtoPWM(DtKNF.mmax);
	if(PWM.MGMOD>PWM.HIPWM) PWM.MGMOD=PWM.HIPWM;
	PWM.MDMOD=CalkVNTtoPWM(DtKNF.mmin);
	if(PWM.MDMOD<PWM.VLO_PWM) PWM.MDMOD=PWM.VLO_PWM;
	PWM.DKNF_MDMOD=PWM.MDMOD; //domyslna wartosc minimum
							
	//PWM.DV16_MDMOD=CalkVNTtoPWM(_VVNT16);				//czasowa wartosc minimum
	//if(PWM.DV16_MDMOD<PWM.VLO_PWM) PWM.DV16_MDMOD=PWM.VLO_PWM;
	PWM.MSTART=CalkVNTtoPWM(ProcToVNT(DtKNF.mstr));
	if(PWM.MSTART>PWM.MGMOD) PWM.MSTART=PWM.MGMOD;
	if(PWM.MSTART<PWM.MDMOD) PWM.MSTART=PWM.MDMOD;

	//casowa moc minimalna nie mniejsza od 1600 i nie wieksza od 3500
	PWM.DV16_MDMOD=CalkVNTtoPWM(_VVNT16);						//czasowa wartosc minimum
	if(PWM.MSTART>PWM.DV16_MDMOD)
	{	
		PWM.DV16_MDMOD=CalkVNTtoPWM(_VVNT35);						//czasowa wartosc minimum
		if(PWM.MSTART<PWM.DV16_MDMOD) PWM.DV16_MDMOD=PWM.MSTART;
	}
	if(PWM.DV16_MDMOD<PWM.VLO_PWM) PWM.DV16_MDMOD=PWM.VLO_PWM;

	PWM.MMAXCW=CalkVNTtoPWM(ProcToVNT(DtKNF.mxcw));
	if(PWM.MMAXCW>PWM.MGMOD) PWM.MMAXCW=PWM.MGMOD;
	if(PWM.MMAXCW<PWM.MDMOD) PWM.MMAXCW=PWM.MDMOD;
	PWM.MMAXCO=CalkVNTtoPWM(ProcToVNT(DtKNF.mxco));
	if(PWM.MMAXCO>PWM.MGMOD) PWM.MMAXCO=PWM.MGMOD;
	if(PWM.MMAXCO<PWM.MDMOD) PWM.MMAXCO=PWM.MDMOD;

	PWM.FIRSTPB=PWM.MGMOD-4*((PWM.MGMOD-PWM.MDMOD)/10);
	PWM.PROP1=PWM.MGMOD-1*((PWM.MGMOD-PWM.MDMOD)/7);
	PWM.PROP2=PWM.MGMOD-2*((PWM.MGMOD-PWM.MDMOD)/7);
	PWM.PROP3=PWM.MGMOD-3*((PWM.MGMOD-PWM.MDMOD)/7);
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
unsigned char VPWMtoPROC(unsigned char pwm)
{
	unsigned int p;

	if(pwm>PWM.MGMOD) pwm=PWM.MGMOD;
	if(pwm<PWM.MDMOD) pwm=PWM.MDMOD;

	p=(pwm-PWM.MDMOD)*100;
	p/=(PWM.MGMOD-PWM.MDMOD);
	if (p>100) p=100;

	return (unsigned char)p;
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Zapisz wartosc do PWM1
void WritePWM(const unsigned char tpwm)
{

	PWM._ENPWM=0;
	PWM.BufPWM=tpwm;//(tpwm>15)?tpwm:16;
	PWM._ENPWM=1;
	PWM.APROC=VPWMtoPROC(PWM.BufPWM);
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
/*---------------------------------------------------------------------------------*/
/*-----------------------------PROTECTED DATA--------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Modyfikacja stanu wyjscia PWM
//UWAGA - procedura wykonywana w przerwaniu
/*if((TMR2IF)&&(TMR2IE))
	{
		TMR2IF=0;
		IntPwm();			//nowe wypelnienie PWM1
		IntPwm2();			//nowe wypelnienie PWM2
	}*/
//---------------------------------------
//PWM1 - modulator lub wentylator
void IntPWM(void)
{
	static bit btOsc=0;

	if(PWM._ENPWM)					//nowa wartosc PWM gotowa do pobrania?
	{
		PWM.NewPWM=PWM.BufPWM;
	}
	if(PWM.NewPWM>PWM.HIPWM) PWM.NewPWM=PWM.HIPWM;
#if _NEG_PWM==1
	PWM.NewPWM=~PWM.NewPWM;
#endif	
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
}

//---------------------------------------
//PWM2 - pompa
void IntPWM2(void)
{
	static bit btOsc=0;

	if(PWM._ENPWM2)					//nowa wartosc PWM gotowa do pobrania?
	{
		PWM.NewPWM2=PWM.BufPWM2;
	}
	if(PWM.NewPWM2>PWM.HIPWM) PWM.NewPWM2=PWM.HIPWM;
#if _NEG_PWM2==1
	PWM.NewPWM2=~PWM.NewPWM2;
#endif	
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
}
/*---------------------------------------------------------------------------------*/
