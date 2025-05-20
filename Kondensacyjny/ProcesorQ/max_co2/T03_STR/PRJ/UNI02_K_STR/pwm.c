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

#if _TYP_PRJ==1
#advisory _TYP_PRJ==1 
#include "../../mcc_generated_files\mcc.h"

#include "global.h"
#include "main.h"
#include "global.ex.h"

#include "pwm.h"					//parametry lokalne
#include "konfig.h"

extern DtKONF DtKNF;
extern tpMAIN M;
extern tpRSDTA1 RSDT1;
extern unsigned char x,kll,hll;

DtPWM PWM;							//czeste operacje na danych
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
static inline void InPWM3_LoadDutyValue(uint16_t dutyValue)
{
    if(PWM._SPREC3)     //10bit
    {
        // Writing to 8 MSBs of PWM duty cycle in PWMDCH register
        PWM3DCH = (dutyValue & 0x03FC)>>2;

        // Writing to 2 LSBs of PWM duty cycle in PWMDCL register
        PWM3DCL = (dutyValue & 0x0003)<<6;
    }
    else                //8bit
    {
        // Writing to 8 MSBs of PWM duty cycle in PWMDCH register
        PWM3DCH = (dutyValue & 0x03FE)>>1;

        // Writing to 2 LSBs of PWM duty cycle in PWMDCL register
        PWM3DCL = (dutyValue & 0x0001)<<8;
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
*/
/*
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
*/
    
/*
 Dla _HIRESVNT==0 
 W TIMER4: PR4=0x7C, prescaler=16
 Dla _HIRESVNT==1 
 W TIMER4: PR4=0xFF, prescaler=8 
 */    
    
    
    
    
    
    PWM.HIPWM=0xff;
#if _HIRESVNT==0
    PWM.HIPWM3=PWM.HIPWM;
#else
    PWM.HIPWM3=0x3ff; 
#endif    
	PWM.BufPWM3=0;
	PWM.NewPWM3=0;
	PWM.BufPWM2=0;
	PWM.NewPWM2=0;
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Podstawowe parametry modulatora (uwaga: wywolywoac gdy wybrano typ kotla)
//Liczy kluczowe parametry modulatora
#if _HIRESVNT==0
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
        case 2:
        {
            PWM.VLO_PWM=ReadEEPROM(_ADRVVLO);                                  
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
#else
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
        case 2:
        {
            PWM.VLO_PWM=ReadEEPROM(_ADRVVLO);                                
            break;            
        }
        default:
        {
            PWM.VLO_PWM=_VLO_PWM0;  //domyslny FIME           
        }
    }
	PWM.MGMOD=CalkVNTtoPWM(DtKNF.mmax);
    PWM.HMGMOD=CalkVNTtoPWMH(DtKNF.mmax*100);
	if(PWM.MGMOD>PWM.HIPWM) PWM.MGMOD=PWM.HIPWM;
    if(PWM.HMGMOD>PWM.HIPWM3) PWM.HMGMOD=PWM.HIPWM3;
	PWM.MDMOD=CalkVNTtoPWM(DtKNF.mmin);
    PWM.HMDMOD=CalkVNTtoPWMH(DtKNF.mmin*100);
	if(PWM.MDMOD<PWM.VLO_PWM) PWM.MDMOD=PWM.VLO_PWM;
    if(PWM.HMDMOD<(PWM.VLO_PWM<<2)) PWM.HMDMOD=(PWM.VLO_PWM<<2);
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
    PWM.MMAXUP=PWM.MMAXCW;

	PWM.MMAXCO=CalkVNTtoPWM(ProcToVNT(DtKNF.mxco));
	if(PWM.MMAXCO>PWM.MGMOD) PWM.MMAXCO=PWM.MGMOD;
	if(PWM.MMAXCO<PWM.MDMOD) PWM.MMAXCO=PWM.MDMOD;

	PWM.FIRSTPB=PWM.MGMOD-4*((PWM.MGMOD-PWM.MDMOD)/10);
	PWM.PROP1=PWM.MGMOD-1*((PWM.MGMOD-PWM.MDMOD)/7);
	PWM.PROP2=PWM.MGMOD-2*((PWM.MGMOD-PWM.MDMOD)/7);
	PWM.PROP3=PWM.MGMOD-3*((PWM.MGMOD-PWM.MDMOD)/7);
}
#endif
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
unsigned char VPWMtoPROC(unsigned char pwm)
{
	unsigned int p;

	if(pwm>PWM.MGMOD) pwm=PWM.MGMOD;
	if(pwm<PWM.MDMOD) pwm=PWM.MDMOD;
    if(PWM.MDMOD==PWM.MGMOD) return 99;
	p=(pwm-PWM.MDMOD)*100;
	p/=(PWM.MGMOD-PWM.MDMOD);
	if (p>100) p=100;

	return (unsigned char)p;
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Zapisz wartosc do PWM1
#if _HIRESVNT==0
void WritePWM3(const unsigned char tpwm)
{
	PWM._ENPWM3=0;
    PWM._SPREC3=0;
    if(tpwm<=0xFF) 
    {
        PWM.BufPWM3=tpwm;
    }
    else
    {
        PWM.BufPWM3=0xFF;        
    }
	PWM._ENPWM3=1;
#if _DEMO==0    
	PWM.APROC=VPWMtoPROC(PWM.BufPWM3);
#endif
}
#else
static bool inline InGetBit(volatile unsigned char data, const unsigned char nbit)
{
	return ((data&(1<<nbit))?1:0);    
}

void WritePWM3H(const unsigned int tpwm)
{
    volatile static unsigned char b_proc=0;   
    volatile unsigned char n_proc=0;    
    volatile unsigned char fsrv,step;
    
	PWM._ENPWM3=0;
    PWM._SPREC3=1;
    if(tpwm<=PWM.HIPWM3) 
    {
        PWM.BufPWM3=tpwm;
    }
    else
    {
        PWM.BufPWM3=PWM.HIPWM3;        
    }
	PWM._ENPWM3=1;
#if _DEMO==0
	PWM.APROC=VPWMtoPROC((PWM.BufPWM3>>2));
#else
    fsrv=0;
    if(!M._MODULUJ)
    {
        if(M._pob_SRW&&!M._ROZRUCH&&M.POB_GRZ)      //aktywna funkcja serwisowa?
        {
            fsrv=1;
            if(InGetBit(RSDT1.inSTAT1,_SRVMAX)) n_proc=99;
            else n_proc=0;
        }    
        else
        if(M._ROZRUCH)                              //wybieg na mocy startowej?
        {
            n_proc=DtKNF.mstr;      //predkosc startowa
            b_proc=n_proc;
        }
        else
        {
            n_proc=0;
        }
    }
    else    //aktywna modulacja
    {
           if(M._RPM_CW)                   //dla obiegu CW 
           {
               n_proc=DtKNF.dmxcw;   //predkosc zalezna od wydatku modulatora
           } 
           else
           {
               n_proc=DtKNF.dmxco;   //predkosc zalezna od wydatku modulatora                
           }        
    } 
	if(RTdS(_RTMOD2)>1)
	{
		StartRTdS(_RTMOD2);
        if(fsrv) step=5;
        else step=1;
        if(b_proc!=n_proc)
        {
            if(b_proc>n_proc)
            {
                if((b_proc-step)>=0)
                {
                    b_proc-=step;
                }
                else
                {
                    b_proc=0;
                }                
            }
            else 
            {
                if((b_proc+step)<=n_proc)
                {
                    b_proc+=step;
                }
                else
                {
                    b_proc=n_proc;
                }
            }
        }
        PWM.APROC=b_proc;
    }
#endif
}
#endif
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
void inline IntPWM3(void)
{
	if(PWM._ENPWM3)					//nowa wartosc PWM gotowa do pobrania?
	{
		PWM.NewPWM3=PWM.BufPWM3;
	}
#if _HIRESVNT==0    
	if(PWM.NewPWM3>0xff) PWM.NewPWM3=0xff;
#else
	if(PWM.NewPWM3>0x03FF) PWM.NewPWM3=0x03FF;    
#endif
    InPWM3_LoadDutyValue((uint16_t)PWM.NewPWM3);
/*    
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