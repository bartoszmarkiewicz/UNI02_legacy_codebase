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

#if _TYP_PRJ==4
#advisory _TYP_PRJ==4
#include "../../mcc_generated_files\mcc.h"

#include "global.h"					//parametry globalne
#include "global.ex.h"				

#include "pwm.h"					//parametry lokalne

extern DtKONF DtKNF;
extern tpMAIN M;

DtPWM PWM;							//czeste operacje na danych
KALVnt Kvnt[21];
unsigned char pw1,pw2;
float fpopSMOD,fSMOD;
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
    //pwm2 (wentylator)
	CCP2CON=0;						//reset CCP1 module
	CCPR2L=0;
	CCPR2H=0;
	CCP2CON=0b00001100;				//set PWM mode, single output
    
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
*/
     
/*
 Dla _HIRESVNT==0 
 W TIMER4: PR4=0x7C, prescaler=16
 Dla _HIRESVNT==1 
 W TIMER4: PR4=0xFF, prescaler=8 
     */

	PWM.BufPWM=0;
	PWM.NewPWM=0;
	PWM.BufOSC=0;
	PWM.NewOSC=0;   
    
    PWM.HIPWM=0xff;
#if _HIRESVNT==0
    PWM.HIPWM3=PWM.HIPWM;
#else
    PWM.HIPWM3=0x3ff; 
#endif    
    PWM.BufPWM3=0;
    PWM.NewPWM3=0;
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
//Zapisz wartosc do PWM3
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
}
#else
void WritePWM3H(const unsigned int tpwm)
{
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
}
#endif
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
/*---------------------------------------------------------------------------------*/
//moc na modulatorze (%)
unsigned char MocMod(void)
{
    float dt;

    dt=(float)PWM.BufPWM-PWM.MDMOD;
    dt=dt/(float)(PWM.MGMOD-PWM.MDMOD);
    dt=dt*100.0;
    if(dt<0.0) dt=0.0;
    if(dt>99.0) dt=99.0;
    pw1=(unsigned char)dt;
    
    return (unsigned char)dt;
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Zwraca predkosc wentylatora  z uwzglednieniem oporow kominowych
//gdzie tp-rodzaj gazu, nr-dlugosc przewodu kominowego,ind-kolejny punkt charakterystyki
unsigned char GetVNT(const unsigned char tp, const unsigned char nr,const unsigned char ind)
{
    if(ind>=_NVNTPWM) return 0;
    switch(nr)
    {
        case 0:            
        {
            if(!tp) return vnt_pwm0[ind].VNT;
            return vnt_pwm0p[ind].VNT;
        }
        case 1:            
        {
            if(!tp) return vnt_pwm1[ind].VNT;
            return vnt_pwm1p[ind].VNT;
        }
        case 2:
        {
            if(!tp) return vnt_pwm2[ind].VNT;
            return vnt_pwm2p[ind].VNT;
        }
        case 3:
        {
            if(!tp) return vnt_pwm3[ind].VNT;
            return vnt_pwm3p[ind].VNT;            
        }
        case 4:
        {
            if(!tp) return vnt_pwm4[ind].VNT;
            return vnt_pwm4p[ind].VNT;            
        }
        case 5:
        {
            if(!tp) return vnt_pwm5[ind].VNT;
            return vnt_pwm5p[ind].VNT;            
        }    
        case 6:
        {
            if(!tp) return vnt_pwm6[ind].VNT;
            return vnt_pwm6p[ind].VNT;            
        }     
        case 7:
        {
            if(!tp) return vnt_pwm7[ind].VNT;
            return vnt_pwm7p[ind].VNT;            
        }        
        case 8:
        {
            if(!tp) return vnt_pwm8[ind].VNT;
            return vnt_pwm8p[ind].VNT;            
        }        
        default:
        {
            if(!tp) return vnt_pwm8[ind].VNT;
            return vnt_pwm8p[ind].VNT;            
        }
    }
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Zwraca procent PWM wentylatora z uwzglednieniem oporow kominowych
//gdzie tp-rodzaj gazu, nr-dlugosc przewodu kominowego,ind-kolejny punkt charakterystyki
unsigned char GetPPWM(const unsigned char tp, const unsigned char nr,const unsigned char ind)
{
    if(ind>=_NVNTPWM) return 0;
    switch(nr)
    {
        case 0:
        {
            if(!tp) return vnt_pwm0[ind].PPWM;
            return vnt_pwm0p[ind].PPWM;
        }            
        case 1:            
        {
            if(!tp) return vnt_pwm1[ind].PPWM;
            return vnt_pwm1p[ind].PPWM;            
        }
        case 2:
        {
            if(!tp) return vnt_pwm2[ind].PPWM;
            return vnt_pwm2p[ind].PPWM;            
        }
        case 3:
        {
            if(!tp) return vnt_pwm3[ind].PPWM;
            return vnt_pwm3p[ind].PPWM;            
        }
        case 4:
        {
            if(!tp) return vnt_pwm4[ind].PPWM;
            return vnt_pwm4p[ind].PPWM;            
        }
        case 5:
        {
            if(!tp) return vnt_pwm5[ind].PPWM;
            return vnt_pwm5p[ind].PPWM;            
        }    
        case 6:
        {
            if(!tp) return vnt_pwm6[ind].PPWM;
            return vnt_pwm6p[ind].PPWM;            
        }     
        case 7:
        {
            if(!tp) return vnt_pwm7[ind].PPWM;
            return vnt_pwm7p[ind].PPWM;            
        }        
        case 8:
        {
            if(!tp) return vnt_pwm8[ind].PPWM;
            return vnt_pwm8p[ind].PPWM;            
        }         
        default:
        {
            if(!tp) return vnt_pwm8[ind].PPWM;  
            return vnt_pwm8p[ind].PPWM;            
        }
    }
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Oczekiwana predkosc VNT przeliczana na procent wypelnienia na wentylator
//gdzie dvnt-oczekiwana predkosc
unsigned char VNTtoPPWM(unsigned char dvnt)
{
	unsigned char i;
    float v1,v2,p1,p2,dv;
	float pw,wsp;
    
	dv=dvnt;
	for(i=0;i<_NVNTPWM;i++)
	{
        Kvnt[i].dpwm=GetPPWM(PWM.tpGZ,DtKNF.lkm,i);
        Kvnt[i].dvnt=GetVNT(PWM.tpGZ,DtKNF.lkm,i);        
    } 
	for(i=0;i<_NVNTPWM;i++)
	{     
		p2=(float)Kvnt[i].dpwm;
		v2=(float)Kvnt[i].dvnt;
		if(i!=0)
		{
 			p1=(float)Kvnt[i-1].dpwm;
			v1=(float)Kvnt[i-1].dvnt;
		}
		else
		{		
 			p1=0;
			v1=0;
		}
		if((v2>dv&&v1<=dv)||v2==dv)
		{
            if(v2==v1) break;
            wsp=(p2-p1)/(v2-v1);
            pw=(dv-v1)*wsp+p1;              //y=[(y1-y0)/(x1-x0)]*(x-x1)+y0
			if(pw<0) pw=0;
			if(pw>99) pw=99;	//graniczna gorna wartosc wypelnienia
			return (unsigned char)round(pw);
		}
	}
	return (unsigned char)p2;	//maksymalne wypelnienie dla predkosci poza zakresem
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Zwraca predkosc wentylatora  z uwzglednieniem oporow kominowych
//gdzie tp-rodzaj gazu, nr-dlugosc przewodu kominowego,ind-kolejny punkt charakterystyki
unsigned char MGetVNT(const unsigned char tp, const unsigned char nr,const unsigned char ind)
{
    if(ind>=_NPWMVNT) return 0; 
    if(DtKNF.pwr==0)										//grzejnik niskiej mocy?
    {
        switch(nr)
        {
            case 0:
            case 1:            
            {
                if(!tp) return pwm_vnt1[ind].VNT;
                return pwm_vnt1p[ind].VNT;
            }
            case 2:
            {
                if(!tp) return pwm_vnt2[ind].VNT;
                return pwm_vnt2p[ind].VNT;
            }
            case 3:
            {
                if(!tp) return pwm_vnt3[ind].VNT;
                return pwm_vnt3p[ind].VNT;            
            }
            case 4:
            {
                if(!tp) return pwm_vnt4[ind].VNT;
                return pwm_vnt4p[ind].VNT;            
            }
            case 5:
            {
                if(!tp) return pwm_vnt5[ind].VNT;
                return pwm_vnt5p[ind].VNT;
            }
            case 6:
            {
                if(!tp) return pwm_vnt6[ind].VNT;
                return pwm_vnt6p[ind].VNT;
            }
            case 7:
            {
                if(!tp) return pwm_vnt7[ind].VNT;
                return pwm_vnt7p[ind].VNT;            
            }
            case 8:
            {
                if(!tp) return pwm_vnt8[ind].VNT;
                return pwm_vnt8p[ind].VNT;            
            }        
            default:
            {
                if(!tp) return pwm_vnt8[ind].VNT; 
                return pwm_vnt8p[ind].VNT;            
            }
        }
    }
    else    //grzejnik wysokiej mocy
    {
       switch(nr)
        {
            case 0:
            case 1:            
            {
                if(!tp) return Hpwm_vnt1[ind].VNT;
                return Hpwm_vnt1p[ind].VNT;
            }
            case 2:
            {
                if(!tp) return Hpwm_vnt2[ind].VNT;
                return Hpwm_vnt2p[ind].VNT;
            }
            case 3:
            {
                if(!tp) return Hpwm_vnt3[ind].VNT;
                return Hpwm_vnt3p[ind].VNT;            
            }
            case 4:
            {
                if(!tp) return Hpwm_vnt4[ind].VNT;
                return Hpwm_vnt4p[ind].VNT;            
            }
            case 5:
            {
                if(!tp) return Hpwm_vnt5[ind].VNT;
                return Hpwm_vnt5p[ind].VNT;
            }
            case 6:
            {
                if(!tp) return Hpwm_vnt6[ind].VNT;
                return Hpwm_vnt6p[ind].VNT;
            }
            case 7:
            {
                if(!tp) return pwm_vnt7[ind].VNT;
                return pwm_vnt7p[ind].VNT;            
            }
            case 8:
            {
                if(!tp) return Hpwm_vnt8[ind].VNT;
                return Hpwm_vnt8p[ind].VNT;            
            }        
            default:
            {
                if(!tp) return Hpwm_vnt8[ind].VNT; 
                return Hpwm_vnt8p[ind].VNT;            
            }
        }        
    }
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Zwraca procent PWM wentylatora z uwzglednieniem oporow kominowych
//gdzie tp-rodzaj gazu, nr-dlugosc przewodu kominowego,ind-kolejny punkt charakterystyki
unsigned char MGetPPWM(const unsigned char tp, const unsigned char nr,const unsigned char ind)
{
    if(ind>=_NPWMVNT) return 0;
    if(DtKNF.pwr==0)										//grzejnik niskiej mocy?
    {    
        switch(nr)
        {
            case 0:
            case 1:            
            {
                if(!tp) return pwm_vnt1[ind].PPWM;
                return pwm_vnt1p[ind].PPWM;
            }
            case 2:
            {
                if(!tp) return pwm_vnt2[ind].PPWM;
                return pwm_vnt2p[ind].PPWM;
            }
            case 3:
            {
                if(!tp) return pwm_vnt3[ind].PPWM;
                return pwm_vnt3p[ind].PPWM;            
            }
            case 4:
            {
                if(!tp) return pwm_vnt4[ind].PPWM;
                return pwm_vnt4p[ind].PPWM;            
            }
            case 5:
            {
                if(!tp) return pwm_vnt5[ind].PPWM;
                return pwm_vnt5p[ind].PPWM;
            }
            case 6:
            {
                if(!tp) return pwm_vnt6[ind].PPWM;
                return pwm_vnt6p[ind].PPWM;
            }
            case 7:
            {
                if(!tp) return pwm_vnt7[ind].PPWM;
                return pwm_vnt7p[ind].PPWM;            
            }
            case 8:
            {
                if(!tp) return pwm_vnt8[ind].PPWM;
                return pwm_vnt8p[ind].PPWM;            
            }        
            default:
            {
                if(!tp) return pwm_vnt8[ind].PPWM; 
                return pwm_vnt8p[ind].PPWM;            
            }
        }
    }
    else
    {
        switch(nr)
        {
            case 0:
            case 1:            
            {
                if(!tp) return Hpwm_vnt1[ind].PPWM;
                return Hpwm_vnt1p[ind].PPWM;
            }
            case 2:
            {
                if(!tp) return Hpwm_vnt2[ind].PPWM;
                return Hpwm_vnt2p[ind].PPWM;
            }
            case 3:
            {
                if(!tp) return Hpwm_vnt3[ind].PPWM;
                return Hpwm_vnt3p[ind].PPWM;            
            }
            case 4:
            {
                if(!tp) return Hpwm_vnt4[ind].PPWM;
                return Hpwm_vnt4p[ind].PPWM;            
            }
            case 5:
            {
                if(!tp) return Hpwm_vnt5[ind].PPWM;
                return Hpwm_vnt5p[ind].PPWM;
            }
            case 6:
            {
                if(!tp) return Hpwm_vnt6[ind].PPWM;
                return Hpwm_vnt6p[ind].PPWM;
            }
            case 7:
            {
                if(!tp) return Hpwm_vnt7[ind].PPWM;
                return Hpwm_vnt7p[ind].PPWM;            
            }
            case 8:
            {
                if(!tp) return Hpwm_vnt8[ind].PPWM;
                return Hpwm_vnt8p[ind].PPWM;            
            }        
            default:
            {
                if(!tp) return Hpwm_vnt8[ind].PPWM; 
                return Hpwm_vnt8p[ind].PPWM;            
            }
        }        
    }
}

/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
/*
//Procent wypelnienia PWM na modulatorze na procent wypelnienie PWM na wentylatorze
//(z uwzglednieniem rodzju gazu i oporow kominowych)
unsigned char MPWMtoPPWM(unsigned char dpwm)
{
	unsigned char i,v1,v2,p1,p2,dv;
	signed long int pw;
	signed long int wsp=0;
	
	dv=dpwm;
	for(i=0;i<8;i++)
	{
        Kvnt[i].dpwm=MGetPPWM(PWM.tpGZ, DtKNF.lkm,i);
        Kvnt[i].dvnt=MGetVNT(PWM.tpGZ, DtKNF.lkm,i);
    } 
	for(i=0;i<21;i++)
	{     
		v2=Kvnt[i].dpwm;
		p2=Kvnt[i].dvnt;
		if(i!=0)
		{
 			v1=Kvnt[i-1].dpwm;
			p1=Kvnt[i-1].dvnt;
		}
		else
		{		
 			v1=0;
			p1=0;
		}
		if((v2>dv&&v1<dv)||v2==dv)
		{
			wsp=(signed long int)(p2-p1)*1000;
			wsp/=(v2-v1); //wspolczynnik kierunkowy prostej
			pw=(signed long int)(dv-v1)*wsp;
			pw/=1000;
			
			pw+=p1; 				//y=[(y1-y0)/(x1-x0)]*(x-x1)+y0
			if(pw<0) pw=0;
			if(pw>99) pw=99;	//graniczna gorna wartosc wypelnienia
			return (unsigned char) pw;
		}
	}
	return p2;	//maksymalne wypelnienie dla predkosci poza zakresem
}
 */
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Procent wypelnienia PWM na modulatorze na predkosc na wentylatorze
//gdzie dpwm-wydatek na modulatorze [%]
//VNT=f(MPWM)
unsigned char MPPWMtoVNT(unsigned char dpwm)
{
	unsigned char i;
	float vw,wsp,v1,v2,p1,p2,dp;
    
	dp=(float)dpwm;
	for(i=0;i<_NPWMVNT;i++)
	{
        Kvnt[i].dpwm=MGetPPWM(PWM.tpGZ, DtKNF.lkm,i);
        Kvnt[i].dvnt=MGetVNT(PWM.tpGZ, DtKNF.lkm,i);
    } 
	for(i=0;i<_NPWMVNT;i++)
	{     
		p2=(float)Kvnt[i].dpwm;
		v2=(float)Kvnt[i].dvnt;
		if(i!=0)
		{
 			p1=(float)Kvnt[i-1].dpwm;
			v1=(float)Kvnt[i-1].dvnt;
		}
		else
		{		
 			p1=0;
			v1=0;
		}
		if((p2>dp&&p1<=dp)||p2==dp)
		{
            if(p2==p1) break;
            wsp=(v2-v1)/(p2-p1);
            vw=(dp-p1)*wsp+v1;          //y=[(y1-y0)/(x1-x0)]*(x-x1)+y0
			if(vw<0) vw=0;
			if(vw>99) vw=99;	//graniczna gorna wartosc wypelnienia
			return (unsigned char)round(vw);
		}
	}
	return (unsigned char)v2;	//maksymalna predkosc
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/





//moc na wentylatorze (%)
/*unsigned char MocModToVNT()
{
    unsigned char mm,pvt;
    mm=MocMod();
    pvt=PPWMtoPVNT(mm);
    if(pvt<DtKNF.minv) pvt=DtKNF.minv;
    if(pvt>DtKNF.maxv) pvt=DtKNF.maxv;
    pw2=pvt;
    return pvt; 
}*/
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Procent na wypelnienie PWM
#if _HIRESVNT==0
void ProcToVNT(unsigned char pv)
{
    float dt;
    if(pv>99) pv=99;    
    dt=(PWM.HIPWM*pv)/99.0;
    WritePWM3((unsigned char)dt);            
}
#else
void ProcToVNT(const unsigned char pv)
{
    volatile unsigned char prc;
    volatile float dt;
    
    prc=pv;
    if(prc>99) prc=99;
    dt=(float)PWM.HIPWM3;
    dt=(dt*prc)/99.0;
    WritePWM3H((unsigned int)dt);            
}


/*void ProcToVNT(const unsigned char pv)
{
    static unsigned char bpv;
    volatile unsigned char prc;
    volatile float dt;
    
    if(RTdS(_RTMVV)>2)
    {
        StartRTdS(_RTMVV);        
        if(bpv!=pv)
        {
            if(bpv<pv)
            {
                bpv++;
            }
            if(bpv>pv)
            {
                bpv--;
            }        
            bpv=pv;
        }
    }
    prc=bpv;
    if(prc>99) prc=99;
    dt=(float)PWM.HIPWM3;
    dt=(dt*prc)/99.0;
    
    WritePWM3H((unsigned int)dt);            
}
*/
#endif
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Ustaw predkosc wentylatora
void SterownikVNT(void)
{          
    float prc,vpwm;
    unsigned char mc;
    return;
    if(_VNT_GetValue()&&!_GRZEJ_GetValue())         //zalaczony wentylator, wylaczona czesc zapalacza
    {
        /*ProcToVNT(99); 
        pw1=0;
        pw2=99;
        StartPID(1);
        */
        prc=(float)VNTtoPPWM(DtKNF.maxv)/99.0;         //oczekiwana predkosc wentylatora na procent wypelnienia na wentylatorze
        vpwm=((float)PWM.HIPWM3)*prc;
        WritePWM3H((unsigned int)vpwm);
        pw2=VNTtoPPWM(DtKNF.maxv);
        pw1=0;
        StartPID(1);        
    }
    else
    if(_VNT_GetValue()&&_GRZEJ_GetValue())          //zalaczony wentylator, zalaczona czesc zapalacza
    {
        if(M._ROZRUCH)                //trwa rozruch?
        {
            /*if(!RdPrt(S_MOD_ON))        //przerwa w obwodzie modulatora?   
            {
                ProcToVNT(0);                       //ustaw minimalna predkosc obrotowa
                pw1=0;
                pw2=0;  
                StartPID(0);
                StartMProc(DtKNF.minv);                              
            }
            else*/
            {
                //ProcToVNT(DtKNF.strv);                  //predkosc startowa wentylatora
                StartPID(1);
                                  /* StartMProc(DtKNF.strv);                
                    WritePWM3H(SetVNT(DtKNF.strv)); 
                    pw2=VNTtoPPWM(DtKNF.strv);*/
                if(RdPrt(S_PLM)) 					//stwierdzono plomien?
                {
                    StartPID(1);
                    StartMProc(DtKNF.strv+10);                       
                    mc=VNTtoPPWM(DtKNF.strv+10);
                    ProcToVNT(mc);
                    StartMProc(DtKNF.strv+10);                    
                }
                else
                {
                    StartPID(1);
                    StartMProc(DtKNF.strv);                       
                    mc=VNTtoPPWM(DtKNF.strv);
                    ProcToVNT(mc);
                    StartMProc(DtKNF.strv); 
                }
                pw1=MocMod();
            }
            StartRTdS(_RTMIP);
        }
        else                                        //grzanie wody uzytkowej?
        {
#if _TEST_PWM==0
            /*if(!RdPrt(S_MOD_ON)||M._RUN_MNMOD)      //przerwa w obwodzie modulatora lub tryb modulacji NMOD?
            {
                ProcToVNT(0);                       //ustaw minimalna predkosc obrotowa
                pw1=0;
                pw2=0;   
                if(!RdPrt(S_MOD_ON))
                {
                    StartPID(0);
                    StartMProc(DtKNF.minv);                    
                }
            }    
            else*/
            {
                if(RTdS(_RTMIP)<50)
                {
                    if(!PWM.tpGZ) 				//gaz ziemny
                    {
                        PWM.MDMOD=_MD_MOD1+50;
                    }
                    else
                    {
                        PWM.MDMOD=_MD_PMOD1+50;
                    }
                }
                else
                {
                    if(!PWM.tpGZ) 				//gaz ziemny
                    {
                        PWM.MDMOD=_MD_MOD1;
                    }
                    else
                    {
                        PWM.MDMOD=_MD_PMOD1;
                    }                
                }
                if(M._SVWB) //wybieg na mocy startowej?
                {
                    StartPID(1);
                    StartMProc(DtKNF.strv+10);                       
                    /*WritePWM3H(SetVNT(DtKNF.strv));
                    pw2=VNTtoPPWM(DtKNF.strv);
                    pw1=MocMod();*/
                    
                    //mc=pw1+15;
                    //if(mc>99) mc=99;
                    mc=VNTtoPPWM(DtKNF.strv+10);
                    ProcToVNT(mc);
                    StartPID(1);
                    StartMProc(DtKNF.strv+10);
                    
                }
                else
                {
                    //WritePWM3H(MProcToVNT());                //predkosc wentylatora zalezna od wydatku na modulatorze zaworu gazowego
                    pw1=MocMod();
                    pw2=VNTtoPPWM(MPPWMtoVNT(pw1));
                    
                    mc=MPPWMtoVNT(pw1);                    //wydatek na modulatorze(z pomiarow) na oczekiwan? predko?? wentylatora
                    
                    ProcToVNT(VNTtoPPWM(mc));
                }
            }
#else
            ProcToVNT(DtKNF.mxcw);                  //predkosc zadana wentylatora
            /*WritePWM3H(SetVNT(DtKNF.maxv));
            pw2=VNTtoPPWM(DtKNF.maxv);
            pw1=MocMod();*/          
#endif
        }
    }
    else                                            //wylaczony wentylator, wylaczona czesc zapalacza
    {
        ProcToVNT(0);                 
        pw1=0;
        pw2=0;
        /*if(!RdPrt(S_MOD_ON))        //przerwa w obwodzie modulatora?
        {
            StartPID(0);
        }
        else*/
        {
            StartPID(1);            
        }
    }     
}



//------------------------------------------------------------
//Poprawka dla sygnalu sterujacego praca wentylatora
//ppv - docelowa wartosc predkosci
float StrPID(int ppv)
{
    volatile float p,i,d,u,r,pv,v;
 
    pv=(float)ppv;
    v=(float)M.rVVNT;
    u=pv-v;                              //roznica miedzy nastawa a aktualna predkosca obrotowa
    p=DtKNF.P*u;                                 //skladnik proporcionalny P
    
    M.VVSU=M.VVSU+u;                            //suma wszystkich uchybow regulacji
    i=DtKNF.I*M.VVSU;                            //skaladnik ca?kowy I
    
    d=DtKNF.D*(u-M.VVUP);                         //skladnik D
    M.VVUP=u;                                    //uchyb regulacji w poprzednim wykonaniu
    
    r=p+i+d;                                    //modyfikator sygnalu sterujacego
    return r;
}

void StartPID(const unsigned char tp)
{
    float prc;
    
    M.VVSU=0;  
    M.VVUP=0;
    fpopSMOD=0;   
    StartRTdS(_RTMOD2);
    if(tp)
    {
        prc=(float)VNTtoPPWM(DtKNF.strv)/99.0;         //oczekiwana predkosc wentylatora na procent wypelnienia na wentylatorze
    }
    else
    {
        prc=(float)VNTtoPPWM(DtKNF.minv)/99.0;         //oczekiwana predkosc wentylatora na procent wypelnienia na wentylatorze        
    }
    fSMOD=((float)PWM.HIPWM3)*prc;
}
//------------------------------------------------------------
static unsigned char bpvv, bpvv2;
unsigned int SetVNT(unsigned char pv)
{
    static signed char pop=0;
    unsigned int rpv;
    float vpwm,prc;
    
    if(pv>DtKNF.maxv) pv=DtKNF.maxv;
    if(pv<DtKNF.minv) pv=DtKNF.minv;
    rpv=pv*100;                           //oczekiwana predkosc wentylatora
     
	if(RTdS(_RTMOD2)>1)//1
	{
		StartRTdS(_RTMOD2);
        /*if(bpvv2<pv)
        {
            pop=-50;
        }
        else
        if(bpvv2>pv)            
        {
            pop=50;
        }        
        */
        pop=0;
        //---------------------------------------
        prc=(float)VNTtoPPWM(pv)/99.0;         //oczekiwana predkosc wentylatora na procent wypelnienia na wentylatorze
        vpwm=((float)PWM.HIPWM3)*prc;           //???
        //---------------------------------------
#if _EN_PID==1
        fpopSMOD=fpopSMOD+StrPID(rpv);  //poprawka do wartosci granicznej minimalnej
        fSMOD=vpwm+fpopSMOD;            //teoretyczna oczekiwana wartosc PWM dla pv 
        if(fSMOD>PWM.HIPWM3) 
        {
            fpopSMOD=PWM.HIPWM3-vpwm;
        }
#else
        fSMOD=vpwm;
#endif
        //kontrolowana zmiana obrotow z maksimum do minimum
        if(bpvv2!=pv)
        {
            fpopSMOD=0;
            if(!bpvv2) bpvv2=M.VVNT;
            else
            {
                if(bpvv2>pv)
                {
                    if(bpvv2) bpvv2--;
                }
                else 
                {
                    if(bpvv2<0xff) bpvv2++;
                }
            }
        }
	}
    if(fSMOD>PWM.HIPWM3) 
    {
        fSMOD=PWM.HIPWM3;
    }
    return (unsigned int)fSMOD;    
}
//------------------------------------------------------------
//Procedury modulacji dla potrzeb konfiguracji i funkcji serwisowej
/*unsigned int MProcToVNT(void)
{
	unsigned char pv,mm;    
    
    mm=MocMod();                          //procentowa wartosc mocy ma modulatorze
    pv=MPPWMtoVNT(mm);                    //wydatek na modulatorze(z pomiarow) na oczekiwana predkosc wentylatora     
    return SetVNT(pv);                    //oczekiwana predkosc na wentylatorze na wypelnienie PWM wentylatora
}*/

void StartMProc(unsigned char pv)
{
    volatile unsigned char mm; 
    
    //bpvv=(unsigned char)(((pv-DtKNF.minv)/(DtKNF.maxv-DtKNF.minv))*100.0);  //???
    
    mm=MocMod();                          //procentowa wartosc mocy ma modulatorze
    mm=mm+15;
    if(mm>99) mm=99;
    bpvv=MPPWMtoVNT(mm);                    //wydatek na modulatorze(z pomiarow) na oczekiwan? predko?? wentylatora
    
    bpvv2=bpvv;
}
unsigned int MProcToVNT(void)
{
	volatile unsigned char pv,mm; 
    
    if(bpvv>100) bpvv=100;
    mm=MocMod();                          //procentowa wartosc mocy ma modulatorze
    pv=MPPWMtoVNT(mm);                    //wydatek na modulatorze(z pomiarow) na oczekiwan? predko?? wentylatora   
    if(RTdS(_RTMVV)>0)                    //1
    {
        StartRTdS(_RTMVV);        
        if(bpvv!=pv)
        {
            if(bpvv<pv)
            {
                bpvv+=2;
                if(bpvv>pv) bpvv=pv;
            }
            if(bpvv>pv)
            {
                bpvv-=1;
                if(bpvv<pv) bpvv=pv;
            }        
        }
    }
    
    return SetVNT(bpvv);                    //oczekiwana predkosc na wentylatorze na wypelnienie PWM wentylatora
}
/*unsigned char MocModToVNT()
{
    unsigned char mm,pvt;
    mm=MocMod();
    pvt=MPWMtoVNT(mm);           //oczekiwana predkosc wentylatora
    if(pvt<DtKNF.minv) pvt=DtKNF.minv;
    if(pvt>DtKNF.maxv) pvt=DtKNF.maxv;
    pw2=pvt;
    return pvt; 
}*/
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
//PWM2 - wentylator
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
 */
}
/*---------------------------------------------------------------------------------*/
#endif
