/*pwm2.c*/
//Procedury zwiazane z modulem CCP (tryb pracy = PWM)
/*---------------------------------------------------------------------------------*/
//#include <pic18.h>
#include <xc.h>
#include "global.h"					//parametry globalne
#include "pwm2.h"					//parametry lokalne

volatile DtPWM PWM;							//czeste operacje na danych
/*---------------------------------------------------------------------------------*/
/*-----------------------------PUBLIC DATA-----------------------------------------*/
void WritePWM2(const unsigned char tpwm);
void InitPWM2(void)
{
//pwm2 (LCD)
	CCP2CON=0;						//reset CCP1 module
	CCPR2L=0;
	CCPR2H=0;
	CCP2CON=0b00001100;				//set PWM mode
        CCP2IP=0;
//timer2
	TMR2=0;
	T2CON=_WT2CON;					//new timer2 prescale value
	PR2=_WPR2;					//PWM period
        TMR2IP=0;					//low priority interrupt
	TMR2IE=1;					//enable interrupt timer2
	TMR2ON=1;					//start timer2
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

	PWM.BufPWM2=0;
	PWM.NewPWM2=0;						
	WritePWM2(PWM.HIPWM);
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Zapisz wartosc do PWM2
void WritePWM2(const unsigned char tpwm)
{
	PWMf_ENPWM2=0;
	PWM.BufPWM2=tpwm;
	PWMf_ENPWM2=1;
}
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
//PWM2 - pompa
void IntPWM2(void)
{
	if(PWMf_ENPWM2)					//nowa wartosc PWM gotowa do pobrania?
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
