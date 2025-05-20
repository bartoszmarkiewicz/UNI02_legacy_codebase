/*rgl.c*/
/*---------------------------------------------------------------------------------*/
#include <xc.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

#include "../../proj.tp.h"

#ifndef _TYP_PRJ
#error NIEOKRESLONY IDENTYFIKATOR PROJEKTU
#endif

#if _TYP_PRJ==1
#advisory _TYP_PRJ==1 
#include "global.h"
#include "main.h"
#include "global.ex.h"

#include "rgl.h"				//parametry lokalne
#include "konfig.h"

extern DtPMP PMP;
extern DataCO DtCO;
extern DataCW DtCW;
extern DtPWM PWM;
extern DtKONF DtKNF;
extern tpMAIN M;
extern unsigned char vvvv, vvv, vup, vdw,mmm;
extern unsigned char x,kll,gll,hll,bll;

DtRgl rgl; 
unsigned char tmr,tmr2;
unsigned char pwm_center,pwm_kwr;
signed char popMAXCW, popMAXCO, popMIN;
signed char popSMAX,popSMIN,popSMOD;
float fpopSMOD,fSMOD,fpopMIN,fpopMAXCO,fpopMAXCW;
//------------------------------------------------------------
//------------------------------------------------------------
unsigned int ProcToRegMod(unsigned char ref, unsigned char prvnt);
//Procedura modulacji
void Moduluj(void)
{
	unsigned int wpw;
	unsigned char wpp;
	//FPmVNT2();					//parametry dynamiczne wentylatora
	switch(DtKNF.tpkt)			//rodzaj kotla
	{
		case _MONOHB:				//kociol z hydroblokiem
		{
			if(M._pob_CO)			//obieg CO
			{
//-------------
#if _PID_VNT==0                
				if(RTdS(_RTMOD2)>1)//>1
				{
					StartRTdS(_RTMOD2);
					NewPop_MIN(); 	//poprawka do wartosci granicznej minimalnej
					NewPop_MAX_CO(); //poprawka do wartosci granicznej maksymalnej
				}
				New_MIN();			//wartosc graniczna modulacji
				New_MAX_CO();		//wartosc graniczna modulacji
#else
                PWM.MINPWM=PWM.MDMOD;          
                PWM.MAXPWM=PWM.MMAXCO;
#endif
                
                /*
                StartCO2();			
				ReadDataCO2();		               
                ModToCO2();			
                rgl.tmp_pwm=PWM.MAXPWM; 
                if(vvvv<99) vvvv++;
                else vvvv=0;
                if(rgl.tmp_pwm<=PWM.MINPWM)
                {

                    WritePWM3H(VMinToPWM(0));
                }
                else
                if(rgl.tmp_pwm>=PWM.MAXPWM)
                {                        
                    WritePWM3H(VMaxToPWM(0));
                }   
                else
                {
                    ResetVntToPWM();
                    WritePWM3(rgl.tmp_pwm);
                }             
                break;
                 */
//-------------
				StartCO2();			//parametry startowe dla CO
				ReadDataCO2();		//wprowadzenie danych wejsciowych dla CO
				ModToCO2();			//zmien obieg na CO
				ModZgCO();			//pierwsze przyblizenie
				ModDkCO();			//drugie przyblizenie
				if(RTdS(_RTMOD)>_TOKM)
				{
					StartRTdS(_RTMOD);
					IncConstCO2();	//oblicza czas przez ktory temp CO = const
					IncMaxCO();		//inkrementacja maksymalnej wartosci mocy na palniku
					ModPruCO();		//trzecie przyblizenie
				}
			
				ModPrzCO();			//reakcja superwizyjna
				PropOPT();
			}
			else
			if(M._pob_CW)			//obieg CW
			{
//-------------
#if _PID_VNT==0                
                if(RTdS(_RTMOD2)>1)
				{
					StartRTdS(_RTMOD2);
					NewPop_MIN(); 	//poprawka do wartosci granicznej minimalnej
					NewPop_MAX_CW(); //poprawka do wartosci granicznej maksymalnej
				}
				New_MIN();			//wartosc graniczna modulacji
				New_MAX_CW();		//wartosc graniczna modulacji
#else
                PWM.MINPWM=PWM.MDMOD;          
                PWM.MAXPWM=PWM.MMAXCW;                
#endif
//-------------
				StartMCW2();		//parametry startowe dla CW
				ReadDataCW2();		//wprowadzenie danych wejsciowych dla CW
				ModToCW2();			//zmien obieg na CW
				ModZgCW2();			//pierwsze przyblizenie
				ModDkCW2();			//drugie przyblizenie

				if(RTdS(_RTMOD)>_TOKM)
				{				
					StartRTdS(_RTMOD);
					IncConstCW2();	//oblicza czas przez ktory temp CW = const	
					IncMaxCW2();	//inkrementacja maksymalnej wartosci mocy na palniku
					ModPruCW2();	//trzecie przyblizenie
				}
				ModPrzCW2();		//reakcja superwizyjna
				PropCO2();			//zabezpieczenie od przegrzania w obwodzie CO				
			}
			break;
		}
		case _UNICO:
		{
//-------------
#if _PID_VNT==0              
			if(RTdS(_RTMOD2)>1)
			{
				StartRTdS(_RTMOD2);
				NewPop_MIN(); 	//poprawka do wartosci granicznej minimalnej
				if(M._pob_CW||M._pob_AL)			//obieg CW
				{
					NewPop_MAX_CW(); //poprawka do wartosci granicznej maksymalnej
				}
				else
				{
					NewPop_MAX_CO(); //poprawka do wartosci granicznej maksymalnej
				}
			}
			New_MIN();			//wartosc graniczna modulacji
			if(M._pob_CW||M._pob_AL)			//obieg CW
			{
				New_MAX_CW();		//wartosc graniczna modulacji
			}
			else
			{
				New_MAX_CO();		//wartosc graniczna modulacji
			}
#else
                PWM.MINPWM=PWM.MDMOD;
                if(M._pob_CW||M._pob_AL)			//obieg CW
                {
                    PWM.MAXPWM=PWM.MMAXCW; 
                }
                else
                {
                    PWM.MAXPWM=PWM.MMAXCO;
                }          
#endif
//-------------
			StartCO3();			//parametry startowe dla CO
			ReadDataCO2();		//wprowadzenie danych wejsciowych dla CO
			ModToCO2();			//zmien obieg na CO
			ModZgCO();			//pierwsze przyblizenie
			ModDkCO();			//drugie przyblizenie
			if(RTdS(_RTMOD)>_TOKM)
			{
				StartRTdS(_RTMOD);
				IncConstCO2();	//oblicza czas przez ktory temp CO = const
				IncMaxCO();		//inkrementacja maksymalnej wartosci mocy na palniku
				ModPruCO();		//trzecie przyblizenie
			}
		
			ModPrzCO();			//reakcja superwizyjna
			PropOPT();
			break;
		}
		default:		//domyslny monotermiczny z hydroblokiem
		{           
			if(M._pob_CO)			//obieg CO
			{
//-------------
#if _PID_VNT==0                 
				if(RTdS(_RTMOD2)>1)
				{
					StartRTdS(_RTMOD2);
					NewPop_MIN(); 	//poprawka do wartosci granicznej minimalnej
					NewPop_MAX_CO(); //poprawka do wartosci granicznej maksymalnej
				}
				New_MIN();			//wartosc graniczna modulacji
				New_MAX_CO();		//wartosc graniczna modulacji
#else
                PWM.MINPWM=PWM.MDMOD;          
                PWM.MAXPWM=PWM.MMAXCO;                             
#endif                
//-------------
				StartCO2();			//parametry startowe dla CO
				ReadDataCO2();		//wprowadzenie danych wejsciowych dla CO
				ModToCO2();			//zmien obieg na CO
				ModZgCO();			//pierwsze przyblizenie
				ModDkCO();			//drugie przyblizenie
				if(RTdS(_RTMOD)>_TOKM)
				{
					StartRTdS(_RTMOD);
					IncConstCO2();	//oblicza czas przez ktory temp CO = const
					IncMaxCO();		//inkrementacja maksymalnej wartosci mocy na palniku
					ModPruCO();		//trzecie przyblizenie
				}
				ModPrzCO();			//reakcja superwizyjna
				PropOPT();
			}
			else
			if(M._pob_CW)			//obieg CW
			{
//-------------
#if _PID_VNT==0                
				if(RTdS(_RTMOD2)>1)
				{
					StartRTdS(_RTMOD2);
					NewPop_MIN(); 	//poprawka do wartosci granicznej minimalnej
					NewPop_MAX_CW(); //poprawka do wartosci granicznej maksymalnej
				}
				New_MIN();			//wartosc graniczna modulacji
				New_MAX_CW();		//wartosc graniczna modulacji
#else
                PWM.MINPWM=PWM.MDMOD;          
                PWM.MAXPWM=PWM.MMAXCW;                            
#endif
//-------------
				StartMCW2();		//parametry startowe dla CW
				ReadDataCW2();		//wprowadzenie danych wejsciowych dla CW
				ModToCW2();			//zmien obieg na CW
				ModZgCW2();			//pierwsze przyblizenie
				ModDkCW2();			//drugie przyblizenie

				if(RTdS(_RTMOD)>_TOKM)
				{				
					StartRTdS(_RTMOD);
					IncConstCW2();	//oblicza czas przez ktory temp CW = const	
					IncMaxCW2();	//inkrementacja maksymalnej wartosci mocy na palniku
					ModPruCW2();	//trzecie przyblizenie
				}
				ModPrzCW2();		//reakcja superwizyjna
				PropCO2();			//zabezpieczenie od przegrzania w obwodzie CO
				
			}
			break;
		}
	}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//korekta punktu rownowagi w fazie dochodzenia (FAZA1) 
float Format_kCO1(float dt)
{
    float f;
    f=dt*DtKNF.RCO1;
    if(f>100) f=100;
    if(f<-100) f=-100;    
    return f;          
}
//regulacja w punkcie rownowagi w fazie stabilizacji (FAZA2)
float Format_kCO2(float dt)
{
    float f;
    f=dt*DtKNF.RCO2;
    if(f<0) f=0;
    if(f>100.0) f=100.0;
    return f;
}
//korekta punktu rownowagi w fazie stabilizacji (FAZA2)
float Format_kCO3(float dt)
{
    float f;
    f=dt*DtKNF.RCO3;
    if(f>100) f=100;
    if(f<-100) f=-100; 
    return f;
}
//Reakcja superwizyjna
float Format_kCO4(float dt)
{
    float f;
    f=dt*DtKNF.RCO4;
    if(f<0) f=0;    
    if(f>100) f=100;
    return f;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//Korekta (trzecie przyblizenie-faza1)
float Format_kCW1(float dt)
{
    float f;
    f=dt*DtKNF.RCW1;
    if(f>100) f=100;
    if(f<-100) f=-100; 
    return f;
}
//Regulacja (drugie przyblizenie)
float Format_kCW2(float dt)
{
    float f;
    f=dt*DtKNF.RCW2;
    if(f<0) f=0;
    if(f>100.0) f=100.0;
    return f;
}
//Korekta (trzecie przyblizenie-faza2)
float Format_kCW3(float dt)
{
    float f;
    f=dt*DtKNF.RCW3;
    if(f>100) f=100;
    if(f<-100) f=-100; 
    return f;
}
//Reakcja superwizyjna
float Format_kCW4(float dt)
{
    float f;
    f=dt*DtKNF.RCW4;
    if(f<0) f=0;    
    if(f>100) f=100;
    return f;
}
//------------------------------------------------------------
//------------------------------------------------------------
//------dowolny wymiennik - obieg CO
//------------------------------------------------------------
//------------------------------------------------------------								
//-------------------------------------------------------------
//parametry startowe dla CO - kociol 1 funkcyjny.
unsigned char FirstXMOD(void);
void StartCO2(void)//StartMCO1F(void)
{
	if(rgl.btco||rgl.btcw) return; //rozpoczecie pracy modulatora?
	StartRTdS(_RTMOD);
#if _PID_VNT!=0    
    PWM.MINPWM=PWM.MDMOD;          
    PWM.MAXPWM=PWM.MMAXCO;
    ResetVntToPWM();    
#endif
	popMIN=0;
	fpopMIN=0;    
	popMAXCO=0;
	fpopMAXCO=0;    
	DtCO.dDCO=0;
	DtCW.dDCW=0;
	DtCO.ddDCO=0;
	DtCW.ddDCW=0;
	DtCO.indCO=0;
	DtCW.indCW=0;
	rgl.fz=0;
	rgl.med_up=0;
	rgl.fast_up=0;
	rgl.srp=0;
	rgl.ytmp=0;
	rgl.nmod=0;
	rgl.del1s=0;
	rgl.inv_mod=0;
	rgl.xmod=FirstXMOD();				//startowa wartosc ogranicznika mocy
	tmr=1;
	tmr2=1;
	pwm_center=PWM.MINPWM+(PWM.MAXPWM-PWM.MINPWM)/2;
	pwm_kwr=PWM.MINPWM+(PWM.MAXPWM-PWM.MINPWM)/4;
	//rgl.tmp_pwm=rgl.xmod;
}

void StartCO3(void)//StartMCO1F(void)
{
	if(rgl.btco||rgl.btcw) return; //rozpoczecie pracy modulatora?
	StartRTdS(_RTMOD);
#if _PID_VNT!=0    
    PWM.MINPWM=PWM.MDMOD;
    if(M._pob_CW||M._pob_AL)			//obieg CW   
    {
        PWM.MAXPWM=PWM.MMAXCW;    
    }
    else
    {
        PWM.MAXPWM=PWM.MMAXCO;        
    }
    ResetVntToPWM();
#endif
	popMIN=0;
	fpopMIN=0;    
	popMAXCO=0;
	fpopMAXCO=0;    
	DtCO.dDCO=0;
	DtCW.dDCW=0;
	DtCO.ddDCO=0;
	DtCW.ddDCW=0;
	DtCO.indCO=0;
	DtCW.indCW=0;
	rgl.fz=0;
	rgl.med_up=0;
	rgl.fast_up=0;
	rgl.srp=0;
	rgl.ytmp=0;
	rgl.nmod=0;
	rgl.del1s=0;
	rgl.inv_mod=0;
	rgl.xmod=FirstXMOD();				//startowa wartosc ogranicznika mocy
	tmr=1;
	tmr2=1;
	pwm_center=PWM.MINPWM+(PWM.MAXPWM-PWM.MINPWM)/2;
	pwm_kwr=PWM.MINPWM+(PWM.MAXPWM-PWM.MINPWM)/4;
	//rgl.tmp_pwm=rgl.xmod;
}
//-------------------------------------------------------------
//Oblicz wartosc startow ogranicznika mocy
unsigned char FirstXMOD(void)
{
	signed long m;
	unsigned char maxmod,p,p2,tmp;

	//-------
	//maksymalna dozwolona wartosc pierwszego pobudzenia
	PWM.FIRSTPB=PWM.MAXPWM-4*((PWM.MAXPWM-PWM.MINPWM)/10);
	tmp=(PWM.FIRSTPB-PWM.MINPWM)/4;
	if(PWM.MAXPWM>(PWM.FIRSTPB+tmp))
	{
		maxmod=PWM.FIRSTPB+tmp;
	}
	else
	{
		maxmod=PWM.MAXPWM;
	}
	p=maxmod;

	//-------
	//ze wzgledu na temperature
	if(DtCO.CO>=TCOMIN_MD1&&DtCO.CO<=TCOMAX_MD1)						//temperatura w przedziale roboczym?
	{
		m=(signed long)(TCOMAX_MD1-DtCO.CO)*10;
		m=m/(signed long)(TCOMAX_MD1-TCOMIN_MD1);
		m=m*(signed long)(maxmod-PWM.MINPWM);
		if(m<0) m=0;
		m=m/10+PWM.MINPWM;
		if(m>0xff)	p=maxmod;										//przepelnienie 
		else p=(unsigned char)m;
	}
	else															//temperatura poza przedzialem? 
	{
		if(DtCO.CO<TCOMIN_MD1) p=maxmod;
		if(DtCO.CO>TCOMAX_MD1) p=PWM.MINPWM;
	}
	//-------
	//ze wzgledu na wartosc docelow¹ (nastawe)
	if(!DtKNF.opdg)
	{
		if(M.CONTSETP>=_MIN_PCO&&M.CONTSETP<=_MAX_PCO)					//wartosc docelowa w zakresie nastaw?
		{
			m=(signed long)(M.CONTSETP-_MIN_PCO)*10;
			m=m/(signed long)(_MAX_PCO-_MIN_PCO);
			m=m*(signed long)(maxmod-PWM.MINPWM);
			if(m<0) m=0;
			m=m/10+PWM.MINPWM;
			if(m>0xff)	p2=maxmod;										//przepelnienie 
			else p2=(unsigned char)m;
		}else p2=p;
	}
	else
	{
		if(M.CONTSETP>=_MIN_PDCO&&M.CONTSETP<=_MAX_PDCO)					//wartosc docelowa w zakresie nastaw?
		{
			m=(signed long)(M.CONTSETP-_MIN_PDCO)*10;
			m=m/(signed long)(_MAX_PDCO-_MIN_PDCO);
			m=m*(signed long)(maxmod-PWM.MINPWM);
			if(m<0) m=0;
			m=m/10+PWM.MINPWM;
			if(m>0xff)	p2=maxmod;										//przepelnienie 
			else p2=(unsigned char)m;
		}else p2=p;
	}
	//-------
	//wybrana zostaje nizsza wartosc z p i p2
	if(p>p2) p=p2;
	//-------
	//ze wzgledu na relacje miêdzy temperatura i nastawa
	if(DtCO.DCO>=DCOMIN_MD1&&DtCO.DCO<=DCOMAX_MD1)						//roznica w przedziale roboczym?
	{
		m=(signed long)DtCO.DCO*10;
		m=m/(signed long)(DCOMAX_MD1-DCOMIN_MD1);
		m=m*(signed long)(maxmod-PWM.MINPWM);
		if(m<0) m=0;
		m=m/10+PWM.MINPWM;
		if(m>0xff)	p2=maxmod;										//przepelnienie 
		else p2=(unsigned char)m;		
	}
	else															//roznica poza przedzialem?
	{
		if(DtCO.DCO>DCOMAX_MD1) p2=maxmod;
		if(DtCO.DCO<DCOMIN_MD1) p2=PWM.MINPWM;
	}
	//-------
	//wybrana zostaje nizsza wartosc z p i p2
	if(p>p2) p=p2;
	//-------
	return p;
														//wartosc pierwszego pobudzenia
}
//-------------------------------------------------------------
//pierwsze przyblizenie
#if (_HIRESVNT==0)
void ModZgCO(void)
{
	signed long m;
	unsigned char p;
	if(rgl.fz)						//aktualnie faza 2?
	{
		if(Tnizsza(3))				//2 przygotowanie do przejscia do fazy 1
		{
			rgl.xmod=PWM.BufPWM3;
			rgl.srp=0;
			rgl.ytmp=0;	
			rgl.nmod=0;
			rgl.del1s=0;
		}
		else return;
	}
	if(Twyzsza(0)&&Tstoi()) 			//warunek przejscia do fazy 2
	{
		rgl.fz=1;
		rgl.fast_up=0;
		rgl.med_up=0;
		rgl.srp=PWM.BufPWM3;
		rgl.xmod=PWM.BufPWM3;
		if(rgl.xmod>PWM.MAXPWM)
		{
			rgl.srp=PWM.MAXPWM;
			rgl.xmod=PWM.MAXPWM;
		}
		rgl.slow_inc=0;
		return;
	}
	if(rgl.xmod>=PWM.MINPWM)	m=(rgl.xmod-PWM.MINPWM)*10;
	else m=0;
	m=m/(signed long)(DtCO.MAD_CO-DtCO.DAD_CO);	//DAD_CO-MAD_CO
	m=m*(signed long)(DtCO.AC_CO-DtCO.DAD_CO);	//DAD_CO-AC_CO	
	if(m<0) m=0;
	m=m/10+PWM.MINPWM;
	if(m>0xff)						//przepelnienie 
	{
		rgl.fz=0;					//przejscie do fazy1 przy aktywnej fazie2       
		rgl.tmp_pwm=rgl.xmod;        
		return;
	}
	if(rgl.fz&&(rgl.xmod>m))
		rgl.srp=(rgl.xmod-m); 		//przygotowanie lagodnego przejscia do fazy 1
	m+=rgl.srp;
	if(m>rgl.xmod) p=rgl.xmod;
	else p=(unsigned char)m;
	rgl.fz=0;						//przejscie do fazy1 przy aktywnej fazie2
	rgl.tmp_pwm=p;
}
#else

/*void ModZgCO(void)
{
	signed long m;
    unsigned int dt;
	unsigned char p;
	if(rgl.fz)						//aktualnie faza 2?
	{
		if(Tnizsza(3))				//2 przygotowanie do przejscia do fazy 1
		{
			rgl.xmod=(PWM.BufPWM3>>2);
			rgl.srp=0;
			rgl.ytmp=0;	
			rgl.nmod=0;
			rgl.del1s=0;
		}
		else return;
	}
	if(Twyzsza(0)&&Tstoi()) 			//warunek przejscia do fazy 2
	{
		rgl.fz=1;
		rgl.fast_up=0;
		rgl.med_up=0;
		rgl.srp=(float)PWM.BufPWM3;
		rgl.xmod=(PWM.BufPWM3>>2);
		if(rgl.xmod>PWM.MAXPWM)
		{
			rgl.srp=(float)(PWM.MAXPWM<<2);
			rgl.xmod=PWM.MAXPWM;
		}
		rgl.slow_inc=0;
		return;
	}
	if(rgl.xmod>=PWM.MINPWM)	m=(rgl.xmod-PWM.MINPWM)*10;
	else m=0;
	m=m/(signed long)(DtCO.MAD_CO-DtCO.DAD_CO);	//DAD_CO-MAD_CO
	m=m*(signed long)(DtCO.AC_CO-DtCO.DAD_CO);	//DAD_CO-AC_CO	
	if(m<0) m=0;
	m=m/10+PWM.MINPWM;
	if(m>0xff)						//przepelnienie 
	{
		rgl.fz=0;					//przejscie do fazy1 przy aktywnej fazie2
#if (_HIRESVNT==0)        
		rgl.tmp_pwm=rgl.xmod;
#else
		rgl.tmp_pwm=(rgl.xmod<<2);        
#endif        
		return;
	}
	if(rgl.fz&&(rgl.xmod>m))
    {
		rgl.srp=(float)((rgl.xmod-m)<<2); 		//przygotowanie lagodnego przejscia do fazy 1
    }
    dt=((unsigned int)(round(rgl.srp)))>>2;
	m+=dt;
	if(m>rgl.xmod) p=rgl.xmod;
	else p=(unsigned char)m;
	rgl.fz=0;						//przejscie do fazy1 przy aktywnej fazie2
    rgl.tmp_pwm=(p<<2);
}
*/

//pierwsze przyblizenie
void ModZgCO(void)
{
	float m;
    unsigned int dt;
	unsigned char p;
	if(rgl.fz)						//aktualnie faza 2?
	{
		if(Tnizsza(3))				//2 przygotowanie do przejscia do fazy 1
		{
			rgl.xmod=(PWM.BufPWM3>>2);
			rgl.srp=0;
			rgl.ytmp=0;	
			rgl.nmod=0;
			rgl.del1s=0;
		}
		else return;
	}
	if(Twyzsza(0)&&Tstoi()) 			//warunek przejscia do fazy 2
	{
		rgl.fz=1;
		rgl.fast_up=0;
		rgl.med_up=0;
		rgl.srp=(float)PWM.BufPWM3;
		rgl.xmod=(PWM.BufPWM3>>2);
		if(rgl.xmod>PWM.MAXPWM)
		{
			rgl.srp=(float)(PWM.MAXPWM<<2);
			rgl.xmod=PWM.MAXPWM;
		}
		rgl.slow_inc=0;
		return;
	}
    
    if((DtCO.MAD_CO-DtCO.DAD_CO)!=0)
    {
        m=(DtCO.AC_CO-DtCO.DAD_CO)/(DtCO.MAD_CO-DtCO.DAD_CO);		
    }
    else
    {
        m=0;
    }
	
	m=m*(rgl.xmod-PWM.MINPWM)+PWM.MINPWM;
    if(m<0) m=PWM.MINPWM;
	if(m>0xff)						//przepelnienie 
	{
		rgl.fz=0;					//przejscie do fazy1 przy aktywnej fazie2
        rgl.tmp_pwm=(rgl.xmod<<2);
		return;
	}
	if(rgl.fz&&(rgl.xmod>m))
    {
        p=(unsigned char)m;
		rgl.srp=(float)((rgl.xmod-p)<<2); 		//przygotowanie lagodnego przejscia do fazy 1
    }
    dt=(unsigned int)(round(rgl.srp));
	m+=(dt>>2);
	if(m>rgl.xmod) p=rgl.xmod;
	else p=(unsigned char)m;
	rgl.fz=0;						//przejscie do fazy1 przy aktywnej fazie2
    rgl.tmp_pwm=(p<<2);
}
#endif
//-------------------------------------------------------------
//-------------------------------------------------------------
//drugie przyblizenie (tylko dla fazy 2)
#if (_HIRESVNT==0)
void ModDkCO(void)
{
	signed long m;	
	signed char ada;
    float k;
	unsigned char p;
	
	if(!rgl.fz) return;
	ada=DtCO.AC_CO-DtCO.DAD_CO;
	k=2.0;//5
	if(Trowna()) return;				//Tco!=Tnst?
	if(Tnizsza(0))						//Tco<Tnst?
	{
		k=3.0;//6
        if(DtKNF.rfan==2)
        {
            k=Format_kCO2(k);
        }        
	}
	m=(signed long)(round(rgl.srp+k*ada));
	if(m>rgl.xmod) p=rgl.xmod;
	else
	if(m<PWM.MINPWM) p=PWM.MINPWM;
	else p=(unsigned char)m;   
	rgl.tmp_pwm=p;
}	
#else
//drugie przyblizenie (tylko dla fazy 2)
void ModDkCO(void)
{
	signed long m;	
	signed char ada;
	float k;
    unsigned int p;
	
	if(!rgl.fz) return;
	ada=DtCO.AC_CO-DtCO.DAD_CO;
	k=2.0;
	if(Trowna()) return;				//Tco!=Tnst?
	if(Tnizsza(0))						//Tco<Tnst?
	{
		k=3.0;//6
        if(DtKNF.rfan==2)
        {
            k=Format_kCO2(k);
        }        
	}
	m=(signed long)(round(rgl.srp+k*ada));
	if(m>(rgl.xmod<<2)) p=(rgl.xmod<<2);
	else
	if(m<(PWM.MINPWM<<2)) p=(PWM.MINPWM<<2);
	else p=(unsigned int)m;
	rgl.tmp_pwm=p;    
}
#endif
//-------------------------------------------------------------						
//trzecie przyblizenie
//nowa wartosc punktu rownowagi srp.
//wykonywane co krok czasowy
#if (_HIRESVNT==0)
void ModPruCO(void)
{
	static unsigned char l,m,o,p,q,r,s;
	unsigned char tmp;
	signed char k;
	
	if(!rgl.fz)											//aktualnie faza 1?
	{
		l=0;
		m=0;
		o=0;
		p=0;
		if(q<255) q++;									//licznik sekund 
		if(s<255) s++;									//licznik sekund 
		if(DtCO._newPCO) s=0;							//zmieniono nastawe?
		if(Tnizsza(0))									//Tco<Tnst?
		{
			if(!Tnizsza(10))							//Tnst-Tco<=10 ?
			{
				k=0;
				if(rgl.tmp_pwm>=pwm_center)
				{
					if(Tnizsza(5)&&Tstoi()&&s>=1)
					{
						s=0;
						k=20;				//k=2
					}
					else
					if(Tnizsza(4)&&Tstoi()&&s>=1)
					{
						s=0;
						k=15;				//k=2
					}
					else
					if(Tnizsza(2)&&Tstoi()&&s>=1)
					{
						s=0;
						k=10;				//k=2
					}
					else
					if(Tstoi()&&s>=1)
					{
						s=0;
						k=5;				//k=2
					}
				}
				else
				{
					if(rgl.tmp_pwm>=pwm_kwr)
					{
						if(Tnizsza(5)&&Tstoi()&&s>=1)
						{
							s=0;
							k=10;				//k=2
						}
						else
						if(Tnizsza(4)&&Tstoi()&&s>=1)
						{
							s=0;
							k=8;				//k=2
						}
						else
						if(Tnizsza(2)&&Tstoi()&&s>=1)
						{
							s=0;
							k=5;				//k=2
						}
						else
						if(Tstoi()&&s>=1)
						{
							s=0;
							k=2;				//k=2
						}
					}
					else
					{
						if(Tnizsza(2)&&Tstoi()&&s>=1)
						{
							s=0;
							k=5;				//k=2
						}
						else
						if(Tstoi()&&s>=1)
						{
							s=0;
							k=2;				//k=2
						}
					}
				}
				if(rgl.tmp_pwm>=pwm_kwr)
				{
					if(Trosnie(1)&&s>=1) 	//szybki narost temperatury
					{
						s=0;
						k=10;				//10
						if(Trosnie(2)) k=8;	//8b.szybki narost temperatury?
					}
					else
					rgl.ytmp=0;
				}
                if(DtKNF.rfan==2)
                {
                    k=Format_kCO1(k);
                }                
				if(PWM.BufPWM3<rgl.xmod) tmp=rgl.xmod-PWM.BufPWM3;
				else tmp=0;
				if(tmp&&k>tmp) k=tmp;
                if(rgl.srp+k<PWM.MINPWM) rgl.srp=PWM.MINPWM;
                else
				if(rgl.srp+k>PWM.MAXPWM) rgl.srp=PWM.MAXPWM;
				else rgl.srp+=k;
				return;
			}
			else return;
		}
		else return;
	}
	else //aktualnie faza 2
	{
		q=0;
		r=0;
		s=0;
		if(m<255) m++;				//licznik sekund dla spadku temp CO
		if(p<255) p++;				//licznik sekund dla spadku temp CO
		if(o<255) o++;				//licznik sekund dla narostu temp CO
		if(DtCO.DAD_CO>DtCO.AC_CO)	//Tco>Tnst ?
		{
			k=0;
			if(Trosnie(0))//&&m>=2)
			{	
				if(m>=2)
				{
					p=0;
					o=0;
					l=0;
					m=0;
					k=2;	//3	
				}	
			}
			else
			if(Twyzsza(1))				//gdy jest powyzej zakresu nastawa+1
			{
				if(Tstoi())
				{
					if(l>=2)
					{
						k=1;
						if(Twyzsza(3)) k=15;//2
						l=0;
					}
					else
					{
						k=0;
						if(Twyzsza(2)) k=5;//2
 						l++;
					}
				}
			} else l=0;
            if(DtKNF.rfan==2)
            {
                k=Format_kCO3(k);
            }            
			if(rgl.srp-k>PWM.MAXPWM) rgl.srp=PWM.MAXPWM;
            else
			if(rgl.srp-k<PWM.MINPWM) rgl.srp=PWM.MINPWM;
			else rgl.srp-=k;
			if(rgl.xmod<rgl.srp) rgl.xmod=rgl.srp;
			return;
		}
		else										//Tco<Tnst
		{
			k=0;
			if(Tmaleje(0))
			{	
				if(m>=2)
				{
					p=0;
					o=0;
					l=0;
					m=0;
					k=5;		//1			
				}
			}
			else
			if(Tnizsza(0))//1
			{	
				if(Tstoi())
				{
					if(l>=3)
					{
						k=5;//2
						l=0;
					}
					else
					{
						k=0;
 						l++;
					}
				}
			}else l=0;
            if(DtKNF.rfan==2)
            {
                k=Format_kCO3(k);
            }           
            if(rgl.srp+k<PWM.MINPWM) rgl.srp=PWM.MINPWM;
            else
			if(rgl.srp+k>PWM.MAXPWM) rgl.srp=PWM.MAXPWM;
			else rgl.srp+=k;
			if(rgl.xmod<rgl.srp) rgl.xmod=rgl.srp;
			return;
		}
	}
}
#else
//trzecie przyblizenie
//nowa wartosc punktu rownowagi srp.
//wykonywane co krok czasowy
void ModPruCO(void)
{
	static unsigned char l,m,o,p,q,r,s;
	unsigned char tmp;
    unsigned int dt;
	float k;
	
	if(!rgl.fz)											//aktualnie faza 1?
	{
		l=0;
		m=0;
		o=0;
		p=0;
		if(q<255) q++;									//licznik sekund 
		if(s<255) s++;									//licznik sekund 
		if(DtCO._newPCO) s=0;							//zmieniono nastawe?
		if(Tnizsza(0))									//Tco<Tnst?
		{
			if(!Tnizsza(10))							//Tnst-Tco<=10 ?
			{
				k=0;
				if(rgl.tmp_pwm>=(pwm_center<<2))
				{
					if(Tnizsza(5)&&Tstoi()&&s>=1)
					{
						s=0;
						k=20;				//k=2
					}
					else
					if(Tnizsza(4)&&Tstoi()&&s>=1)
					{
						s=0;
						k=15;				//k=2
					}
					else
					if(Tnizsza(2)&&Tstoi()&&s>=1)
					{
						s=0;
						k=10;				//k=2
					}
					else
					if(Tstoi()&&s>=1)
					{
						s=0;
						k=5;				//k=2
					}
				}
				else
				{
					if(rgl.tmp_pwm>=(pwm_kwr<<2))
					{
						if(Tnizsza(5)&&Tstoi()&&s>=1)
						{
							s=0;
							k=10;				//k=2
						}
						else
						if(Tnizsza(4)&&Tstoi()&&s>=1)
						{
							s=0;
							k=8;				//k=2
						}
						else
						if(Tnizsza(2)&&Tstoi()&&s>=1)
						{
							s=0;
							k=5;				//k=2
						}
						else
						if(Tstoi()&&s>=1)
						{
							s=0;
							k=2;				//k=2
						}
					}
					else
					{
						if(Tnizsza(2)&&Tstoi()&&s>=1)
						{
							s=0;
							k=5;				//k=2
						}
						else
						if(Tstoi()&&s>=1)
						{
							s=0;
							k=2;				//k=2
						}
					}
				}
				if(rgl.tmp_pwm>=(pwm_kwr<<2))
				{
					if(Trosnie(1)&&s>=1) 	//szybki narost temperatury
					{
						s=0;
						k=10;				//10
						if(Trosnie(2)) k=8;	//8b.szybki narost temperatury?
					}
					else
					rgl.ytmp=0;
				}
                if(DtKNF.rfan==2)
                {
                    k=Format_kCO1(k);
                }                
				if(PWM.BufPWM3<(rgl.xmod<<2)) tmp=rgl.xmod-(PWM.BufPWM3>>2);
				else tmp=0;
				if(tmp&&(k>tmp)) k=(float)tmp;
                if((rgl.srp+k)<(PWM.MINPWM<<2)) rgl.srp=(float)(PWM.MINPWM<<2);
                else
				if((rgl.srp+k)>(PWM.MAXPWM<<2)) rgl.srp=(float)(PWM.MAXPWM<<2);
				else rgl.srp+=k;
				return;
			}
			else return;
		}
		else return;
	}
	else //aktualnie faza 2
	{
		q=0;
		r=0;
		s=0;
		if(m<255) m++;				//licznik sekund dla spadku temp CO
		if(p<255) p++;				//licznik sekund dla spadku temp CO
		if(o<255) o++;				//licznik sekund dla narostu temp CO
		if(DtCO.DAD_CO>DtCO.AC_CO)	//Tco>Tnst ?
		{
			k=0;
			if(Trosnie(0))//&&m>=2)
			{	
				if(m>=2)
				{
					p=0;
					o=0;
					l=0;
					m=0;
					k=2;	//3	
				}	
			}
			else
			if(Twyzsza(1))				//gdy jest powyzej zakresu nastawa+1
			{
				if(Tstoi())
				{
					if(l>=2)
					{
						k=1;
						if(Twyzsza(3)) k=15;//2
						l=0;
					}
					else
					{
						k=0;
						if(Twyzsza(2)) k=5;//2
 						l++;
					}
				}
			} else l=0;
            if(DtKNF.rfan==2)
            {
                k=Format_kCO3(k);
            }    
            if((rgl.srp-k)>(PWM.MAXPWM<<2)) rgl.srp=(float)(PWM.MAXPWM<<2);
            else
			if((rgl.srp-k)<(PWM.MINPWM<<2)) rgl.srp=(float)(PWM.MINPWM<<2);
			else rgl.srp-=k;
            dt=((unsigned int)(round(rgl.srp)))>>2;
            if(dt>PWM.MAXPWM) dt=PWM.MAXPWM;
			if(rgl.xmod<dt) rgl.xmod=(unsigned char)dt;
			return;
		}
		else										//Tco<Tnst
		{
			k=0;
			if(Tmaleje(0))
			{	
				if(m>=2)
				{
					p=0;
					o=0;
					l=0;
					m=0;
					k=5;		//1			
				}
			}
			else
			if(Tnizsza(0))//1
			{	
				if(Tstoi())
				{
					if(l>=3)
					{
						k=5;//2
						l=0;
					}
					else
					{
						k=0;
 						l++;
					}
				}
			}else l=0;
            if(DtKNF.rfan==2)
            {
                k=Format_kCO3(k);
            }      
            if((rgl.srp+k)<(PWM.MINPWM<<2)) rgl.srp=(PWM.MINPWM<<2);
            else            
			if((rgl.srp+k)>(PWM.MAXPWM<<2)) rgl.srp=(PWM.MAXPWM<<2);
			else rgl.srp+=k;
            dt=((unsigned int)(round(rgl.srp)))>>2;
            if(dt>PWM.MAXPWM) dt=PWM.MAXPWM;            
			if(rgl.xmod<dt) rgl.xmod=(unsigned char)dt;
			return;
		}
	}
}
#endif
//-------------------------------------------------------------								
//inkrementacja maksymalnej wartosci mocy na palniku
//wykonywane co krok czasowy
#if (_HIRESVNT==0)
void IncMaxCO(void)
{
	unsigned char k;
	
	//decyzja o wejsciu w tryb INVMOD
	//kontrola maksimum mocy dla fazy 2
	if(rgl.fz)											//faza 2 ?
	{
		if(rgl.xmod>PWM.MAXPWM) rgl.xmod=PWM.MAXPWM;
		return;
	}
	//jednorazowa modyfikacja wartosci srp ze wzgledu na szybkosc 
	//narostu temperatury
	if(Trosnie(2)&&!rgl.med_up)		//(Tco rosnie szybciej niz 3K/sek)?	
	{
		rgl.med_up=1;
		k=5;						//10
		if(rgl.srp+k>PWM.MAXPWM) rgl.srp=PWM.MAXPWM;
		else rgl.srp+=k;

	}
	if(Trosnie(3)&&!rgl.fast_up)	//(Tco rosnie szybciej niz 4K/sek)?	
	{
		rgl.fast_up=1;
		k=8;						//12        
		if(rgl.srp+k>PWM.MAXPWM) rgl.srp=PWM.MAXPWM;
		else rgl.srp+=k;
	}

	//kontrolowany narost mocy
	if(rgl.tmp_pwm>=pwm_kwr)
	{
		if(Trosnie(2)) return;
	}
	else
	{
		if(Trosnie(1)) return;
	}
	k=5;
	if(rgl.slow_inc) k=2;	//powolny narost mocy po uplywie czasu pracy z moca maksymalna2
	if(rgl.xmod+k>PWM.MAXPWM)
	{
 		rgl.xmod=PWM.MAXPWM;
		PmpToUP();			//reg. obrotami pompy
	}
	else
	{
 		rgl.xmod+=k;
		PmpToDW();			//reg. obrotami pompy
	}
}
#else
void IncMaxCO(void)
{
	unsigned char k;
	
	//decyzja o wejsciu w tryb INVMOD
	//kontrola maksimum mocy dla fazy 2
	if(rgl.fz)											//faza 2 ?
	{
		if(rgl.xmod>PWM.MAXPWM) rgl.xmod=PWM.MAXPWM;
		return;
	}
	//jednorazowa modyfikacja wartosci srp ze wzgledu na szybkosc 
	//narostu temperatury
	if(Trosnie(2)&&!rgl.med_up)		//(Tco rosnie szybciej niz 3K/sek)?	
	{
		rgl.med_up=1;
		k=5;						//10     
        
		if((rgl.srp+k)>(PWM.MAXPWM<<2)) rgl.srp=(float)(PWM.MAXPWM<<2);
		else rgl.srp+=k;

	}
	if(Trosnie(3)&&!rgl.fast_up)	//(Tco rosnie szybciej niz 4K/sek)?	
	{
		rgl.fast_up=1;
		k=8;						//12        
		if((rgl.srp+k)>(PWM.MAXPWM<<2)) rgl.srp=(float)(PWM.MAXPWM<<2);
		else rgl.srp+=k;
	}

	//kontrolowany narost mocy
	if(rgl.tmp_pwm>=(pwm_kwr<<2))
	{
		if(Trosnie(2)) return;
	}
	else
	{
		if(Trosnie(1)) return;
	}
	k=5;
	if(rgl.slow_inc) k=2;	//powolny narost mocy po uplywie czasu pracy z moca maksymalna2
	if(rgl.xmod+k>PWM.MAXPWM)
	{
 		rgl.xmod=PWM.MAXPWM;
		PmpToUP();			//reg. obrotami pompy
	}
	else
	{
 		rgl.xmod+=k;
		PmpToDW();			//reg. obrotami pompy
	}
}
#endif
//-------------------------------------------------------------								
//reakcja superwizyjna
#if (_HIRESVNT==0)
void ModPrzCO(void)
{
	unsigned char k,p,m,maxmod,mdmod;
	signed int r,q;
	static unsigned char timer1,zn;
 
	//stabilizacja predkosci dochodzenia do temp. zadanej.
	if(Tnizsza(0))
	{
		maxmod=PWM.MAXPWM;						//wprowadzono ograniczenie mocy?

		if(rgl.tmp_pwm>=pwm_center)	
		{	
			if(rgl.xmod<=maxmod) p=rgl.xmod;
			else
			{
	 			p=maxmod;	
				rgl.xmod=maxmod;
			}
			if((DtCO.ddDCO>6)&&(tmr>1))											//temperatura rosnie szybciej ni¿ 3 stop. na sek.?							
			{
				tmr=0;
				r=p-PWM.MINPWM;
				if((p-(r/3))>PWM.MINPWM) p-=r/3;//4
				else p=PWM.MINPWM;
				rgl.xmod=p;
			}
			else								
			if((DtCO.ddDCO>4)&&(tmr>1))											//temperatura rosnie szybciej ni¿ 3 stop. na sek.?							
			{
				tmr=0;
				r=p-PWM.MINPWM;
				if((p-(r/4))>PWM.MINPWM) p-=r/4;//4
				else p=PWM.MINPWM;
				rgl.xmod=p;
			}
			else
			if((DtCO.ddDCO>3)&&(tmr>0)) //tmr2									//temperatura rosnie szybciej ni¿ 3 stop. na sek.?							
			{
				tmr=0;
				r=p-PWM.MINPWM;
				if((p-(r/14))>PWM.MINPWM) p-=r/14;//16
				else p=PWM.MINPWM;
				rgl.xmod=p;
			}
		}
		else
		{	
			if(rgl.xmod<=maxmod) p=rgl.xmod;
			else
			{
	 			p=maxmod;	
				rgl.xmod=maxmod;
			}
			if((DtCO.ddDCO>6)&&(tmr>1))											//temperatura rosnie szybciej ni¿ 3 stop. na sek.?							
			{
				tmr=0;
				r=p-PWM.MINPWM;
				if((p-(r/2))>PWM.MINPWM) p-=r/2;//2
				else p=PWM.MINPWM;
				rgl.xmod=p;
			}
			else								
			if((DtCO.ddDCO>4)&&(tmr>1))											//temperatura rosnie szybciej ni¿ 3 stop. na sek.?							
			{
				tmr=0;
				r=p-PWM.MINPWM;
				if((p-(r/3))>PWM.MINPWM) p-=r/3;//3
				else p=PWM.MINPWM;
				rgl.xmod=p;
			}
			else
			if((DtCO.ddDCO>3)&&(tmr>0)) //tmr2									//temperatura rosnie szybciej ni¿ 3 stop. na sek.?							
			{
				tmr=0;
				r=p-PWM.MINPWM;
				if((p-(r/4))>PWM.MINPWM) p-=r/4;//8
				else p=PWM.MINPWM;
				rgl.xmod=p;
			}
		}
	}

	if(!rgl.nmod)					//nie jest w trybie nmod?
	{
		if(rgl.fz)					//faza 2 ?
		{
			if(!Tmaleje(0))			//(Tco nie maleje ?)
			{
				if(Twyzsza(4))		//Tco-Tnst>4 ?
				{
					if(rgl.tmp_pwm>=pwm_kwr)
					{
                        k=15;
                        if(DtKNF.rfan==2)
                        {
                            k=Format_kCO4(k);
                        }
						if((rgl.srp-k)<(PWM.MINPWM<<2))
                        {
                            rgl.srp=(PWM.MINPWM<<2);//10
                        }
						else 
                        {
                            rgl.srp-=k;
                        }
					}
					rgl.nmod=1;		//wejdz w tryb nmod
					return;
				}
				else return;
			}
			else return;
		}
		else return;
	}
	else							//jest w trybie nmod?
	{
		if(Tmaleje(0))				//(Tco maleje ?)
		 			rgl.nmod=0;		//wyjdz z trybu nmod
	}
	timer1++;
}
#else
void ModPrzCO(void)
{
	unsigned char p,maxmod;
	signed int r;
	static unsigned char timer1;
    float k;
    
	//stabilizacja predkosci dochodzenia do temp. zadanej.
	if(Tnizsza(0))
	{
		maxmod=PWM.MAXPWM;						//wprowadzono ograniczenie mocy?

		if(rgl.tmp_pwm>=(pwm_center<<2))	
		{	
			if(rgl.xmod<=maxmod) p=rgl.xmod;
			else
			{
	 			p=maxmod;	
				rgl.xmod=maxmod;
			}
			if((DtCO.ddDCO>6)&&(tmr>1))											//temperatura rosnie szybciej ni¿ 3 stop. na sek.?							
			{
				tmr=0;
				r=p-PWM.MINPWM;
				if((p-(r/3))>PWM.MINPWM) p-=r/3;//4
				else p=PWM.MINPWM;
				rgl.xmod=p;
			}
			else								
			if((DtCO.ddDCO>4)&&(tmr>1))											//temperatura rosnie szybciej ni¿ 3 stop. na sek.?							
			{
				tmr=0;
				r=p-PWM.MINPWM;
				if((p-(r/4))>PWM.MINPWM) p-=r/4;//4
				else p=PWM.MINPWM;
				rgl.xmod=p;
			}
			else
			if((DtCO.ddDCO>3)&&(tmr>0)) //tmr2									//temperatura rosnie szybciej ni¿ 3 stop. na sek.?							
			{
				tmr=0;
				r=p-PWM.MINPWM;
				if((p-(r/14))>PWM.MINPWM) p-=r/14;//16
				else p=PWM.MINPWM;
				rgl.xmod=p;
			}
		}
		else
		{	
			if(rgl.xmod<=maxmod) p=rgl.xmod;
			else
			{
	 			p=maxmod;	
				rgl.xmod=maxmod;
			}
			if((DtCO.ddDCO>6)&&(tmr>1))											//temperatura rosnie szybciej ni¿ 3 stop. na sek.?							
			{
				tmr=0;
				r=p-PWM.MINPWM;
				if((p-(r/2))>PWM.MINPWM) p-=r/2;//2
				else p=PWM.MINPWM;
				rgl.xmod=p;
			}
			else								
			if((DtCO.ddDCO>4)&&(tmr>1))											//temperatura rosnie szybciej ni¿ 3 stop. na sek.?							
			{
				tmr=0;
				r=p-PWM.MINPWM;
				if((p-(r/3))>PWM.MINPWM) p-=r/3;//3
				else p=PWM.MINPWM;
				rgl.xmod=p;
			}
			else
			if((DtCO.ddDCO>3)&&(tmr>0)) //tmr2									//temperatura rosnie szybciej ni¿ 3 stop. na sek.?							
			{
				tmr=0;
				r=p-PWM.MINPWM;
				if((p-(r/4))>PWM.MINPWM) p-=r/4;//8
				else p=PWM.MINPWM;
				rgl.xmod=p;
			}
		}
	}

	if(!rgl.nmod)					//nie jest w trybie nmod?
	{
		if(rgl.fz)					//faza 2 ?
		{
			if(!Tmaleje(0))			//(Tco nie maleje ?)
			{
				if(Twyzsza(4))		//Tco-Tnst>4 ?
				{
					if(rgl.tmp_pwm>=(pwm_kwr<<2))
					{
                        k=15;
                        if(DtKNF.rfan==2)
                        {
                            k=Format_kCO4(k);
                        }
						if((rgl.srp-k)<(PWM.MINPWM<<2))
                        {
                            rgl.srp=(float)(PWM.MINPWM<<2);//10
                        }
						else 
                        {
                            rgl.srp-=k;
                        }
					}
					rgl.nmod=1;		//wejdz w tryb nmod
					return;
				}
				else return;
			}
			else return;
		}
		else return;
	}
	else							//jest w trybie nmod?
	{
		if(Tmaleje(0))				//(Tco maleje ?)
        {
		 			rgl.nmod=0;		//wyjdz z trybu nmod
        }
	}
	timer1++;
}
#endif
//-------------------------------------------------------------							
//Proporcjonalne OPEN-T	
#if (_HIRESVNT==0)
void PropOPT(void)
{  
	if(rgl.nmod)				//aktywny tryb nmod?
	{
		WritePWM3H((PWM.MINPWM<<2));
	}
	else
	{

		if(rgl.tmp_pwm<PWM.MINPWM) rgl.tmp_pwm=PWM.MINPWM;
        WritePWM3H((rgl.tmp_pwm<<2));
/*
#if _PID_VNT==0        
        WritePWM3H((rgl.tmp_pwm<<2));
#else        
        if(rgl.tmp_pwm<=PWM.MINPWM)
        {
            WritePWM3H(VMinToPWM(0));
        }
        else
        if(rgl.tmp_pwm>=PWM.MAXPWM)
        {                        
            WritePWM3H(VMaxToPWM(0));
        }   
        else
        {
            ResetVntToPWM();
            WritePWM3H((rgl.tmp_pwm<<2));
        }
#endif   
*/      
    }
}
#else    
void PropOPT(void)
{
	if(rgl.nmod)				//aktywny tryb nmod?
	{
        ResetVntToPWM();
		WritePWM3H((PWM.MINPWM<<2));
	}
	else
	{

		if(rgl.tmp_pwm<(PWM.MINPWM<<2)) rgl.tmp_pwm=(PWM.MINPWM<<2);

#if _PID_VNT==0        
        WritePWM3H(rgl.tmp_pwm);
#else        
        if(rgl.tmp_pwm<=(PWM.MINPWM<<2))
        {
            WritePWM3H(VMinToPWM(0));
        }
        else
        if(rgl.tmp_pwm>=(PWM.MAXPWM<<2))
        {                        
            if(DtKNF.tpkt!=_UNICO)          //monoterm?
            {
                WritePWM3H(VMaxToPWM(0,0));
            }
            else                            //unico?
            {
                if(M._pob_CW||M._pob_AL)			//obieg CW
                {
                    WritePWM3H(VMaxToPWM(0,1));
                }
                else
                {
                    WritePWM3H(VMaxToPWM(0,0));                    
                }
            }
            //WritePWM3H(PWM.MAXPWM<<2);
        }   
        else
        {
            ResetVntToPWM();
            WritePWM3H(rgl.tmp_pwm);
        }
#endif                
	}
}
#endif    
//************************************************************
//------------------------------------------------------------
//------------------------------------------------------------
//------wymiennik plytowy - obieg CW
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
/*
//parametry startowe dla CO
unsigned char SetStab(unsigned char ada2,unsigned char k,unsigned char res)
{
	static unsigned char pop;
	static unsigned char tpk,pk,t;
	static unsigned char tpk2,pk2,t2,t3;
	static unsigned char bada2,btpk,btpk2;

	if(res)									//reset stanu stabilizatora
	{
		t=0;
		pk=0;
		t2=0;
		pk2=0;
		pop=0;
		bada2=ada2;
		tpk=0;
		btpk=0;
		tpk2=0;
		btpk2=0;
		t3=0;
		return 0;					
	}
	if(!Twyzsza(3)&&!Tnizsza(2))			//2,2wejscie do trybu precyzyjnego
	{
		if(!Trosnie(0)&&!TFrosnie(0))
		{
			tpk=(bada2>ada2)?1:0;
			if(tpk&&tpk!=btpk) pk=1;			//nastapilo kolejne przekroczenie punktu 0?
			if(pk&&++t>5) 						//modyfikacja poprawki
			{
				t=0;
				pk=0;
				t2=0;
				pk2=0;
				pop+=2;
				if(pop>=k) pop=k-1;	
			}
		}
		if(!Tmaleje(0)&&!TFmaleje(0))
		{		
			tpk2=(bada2<ada2)?1:0;
			if(tpk2&&tpk2!=btpk2) pk2=1;			//nastapilo kolejne przekroczenie punktu 0?
			if(pk2&&++t2>5) 					//modyfikacja poprawki
			{
				t2=0;
				pk2=0;
				t=0;
				pk=0;
				if(pop>0) pop-=1;		
			}
		}
	}
	else
	{
		t=0;
		pk=0;
		t2=0;
		pk2=0;
		pop=0;
	}
	btpk=tpk;
	btpk2=tpk2;
	bada2=ada2;
	return pop;
}
 */
//-------------------------------------------------------------
//parametry startowe dla CW
unsigned char FirstXMOD2(void);
void StartMCW2(void)
{
	if(rgl.btco||rgl.btcw) return; //rozpoczecie pracy modulatora?
	StartRTdS(_RTMOD);
#if _PID_VNT!=0      
    PWM.MINPWM=PWM.MDMOD;
    PWM.MAXPWM=PWM.MMAXCW;
    ResetVntToPWM();
#endif    
	popMIN=0;
    fpopMIN=0;
	popMAXCW=0;
    fpopMAXCW=0;
	DtCO.dDCO=0;
	DtCW.dDCW=0;
	DtCO.indCO=0;
	DtCW.indCW=0;
	rgl.fz=0;
	rgl.xmod=FirstXMOD2();
	rgl.srp=0;
	rgl.ytmp=0;
	rgl.del1s=0;
}
//-------------------------------------------------------------	
//pierwsze pobudzenie dla CW
unsigned char FirstXMOD2(void)
{
    PWM.FIRSTPB=PWM.MAXPWM-4*((PWM.MAXPWM-PWM.MINPWM)/10);
    if(DtCW.DCW>0) return PWM.MAXPWM;
    else 
    {
        if(PWM.MAXPWM>PWM.FIRSTPB) return PWM.FIRSTPB;
        else return PWM.MAXPWM;
    }
}
//-------------------------------------------------------------
//zeruj parametry modulatora po zmianie obiegu
#if (_HIRESVNT==0)
void ClrMod2(void)
{
	rgl.fz=0;
	rgl.med_up=0;
	rgl.fast_up=0;
	if(rgl.btco) rgl.xmod=PWM.BufPWM3;
	else rgl.xmod=FirstXMOD2();
	rgl.srp=0;
	rgl.ytmp=0;
	rgl.nmod=0;
	rgl.del1s=0;
	rgl.inv_mod=0;
}
#else
void ClrMod2(void)
{
	rgl.fz=0;
	rgl.med_up=0;
	rgl.fast_up=0;
	if(rgl.btco) rgl.xmod=(PWM.BufPWM3>>2);
	else rgl.xmod=FirstXMOD2();
	rgl.srp=0;
	rgl.ytmp=0;
	rgl.nmod=0;
	rgl.del1s=0;
	rgl.inv_mod=0;
}
#endif
//-------------------------------------------------------------
//zmien obieg na CW
void ModToCW2(void)
{
	rgl.btcw=1;
	if(rgl.btco)
	{
		rgl.btco=0;
		ClrMod2();
	}
}
//-------------------------------------------------------------
//zmien obieg na CO
void ModToCO2(void)
{
	rgl.btco=1;
	if(rgl.btcw)
	{
		rgl.btcw=0;
		ClrMod2();
	}
}
//-------------------------------------------------------------
//oblicza czas przez ktory temp=const
void IncConstCW2(void)
{
	if(!TFrosnie(0))
	{
		if(Tstoi()) rgl.ytmp++;
		else rgl.ytmp=0;
	}
	else rgl.ytmp=0;
}
//-------------------------------------------------------------
void IncConstCO2(void)
{
	if(Tstoi()) rgl.ytmp++;
	else rgl.ytmp=0;
}
//-------------------------------------------------------------	
//wprowadzenie danych wejsciowych-CO
void ReadDataCO2(void)
{
	rgl.x_t=DtCO.DCO;
	rgl.x_dt=DtCO.dDCO;
}
//-------------------------------------------------------------
//wprowadzenie danych wejsciowych-CW
void ReadDataCW2(void)
{
	rgl.x_t=DtCW.DCW;
	rgl.x_f=DtCO.DCO;
	rgl.x_dt=DtCW.dDCW;
	rgl.x_df=DtCO.dDCO;
}
//-------------------------------------------------------------
//FAZA1
//pierwsze przyblizenie
#if (_HIRESVNT==0)
void ModZgCW2(void)
{
	signed long m;
	unsigned char p;
	static unsigned char pop;

	if(Tnizsza(_MR_MADCW))	//10
	{
		pop=0;						//20wartosc poprawki na przesterowanie
		rgl.xmod=FirstXMOD2();
	}
	if(rgl.fz)						//aktualnie faza 2?
	{
		if(Tnizsza(2))				//Tnizsza(2)przygotowanie do przejscia do fazy 1
		{
			rgl.xmod=PWM.BufPWM3;
			rgl.srp=0;
			rgl.ytmp=0;	
			rgl.del1s=0;
			pop=0;
		}
		else return;
	}
	if(!Tnizsza(1)) 				//warunek przejscia do fazy 2
	{
		rgl.fz=1;
		rgl.fast_up=0;
		rgl.med_up=0;
		rgl.srp=PWM.BufPWM3+pop;
		rgl.xmod=PWM.BufPWM3+pop;
		pop=0;
		if(rgl.xmod>PWM.MAXPWM)
		{
			rgl.srp=PWM.MAXPWM;
			rgl.xmod=PWM.MAXPWM;
		}
		return;
	}
	//kll=rgl.srp;
	if(rgl.xmod>=PWM.MINPWM)	m=(rgl.xmod-PWM.MINPWM)*10;
	else m=0;
	m=m/(signed long)(DtCW.MAD_CW-DtCW.DAD_CW);	//DAD_CW-MAD_CW
	m=m*(signed long)(DtCW.AC_CW-DtCW.DAD_CW);	//DAD_CW-AC_CW	
	if(m<0) m=0;
	m=m/10+PWM.MINPWM;
	if(m>0xff)						//przepelnienie 
	{
		rgl.fz=0;					//przejscie do fazy1 przy aktywnej fazie2       
		rgl.tmp_pwm=rgl.xmod;
		return;
	}
	if(rgl.fz&&(rgl.xmod>m))
		rgl.srp=(rgl.xmod-m); 		//przygotowanie lagodnego przejscia do fazy 1

	m+=rgl.srp;
	if(m>rgl.xmod) p=rgl.xmod;
	else p=(unsigned char)m;
	rgl.fz=0;						//przejscie do fazy1 przy aktywnej fazie2   
	rgl.tmp_pwm=p;
}
#else
//pierwsze przyblizenie
/*void ModZgCW2(void)
{
	signed long m;
    unsigned int dt;
	unsigned char p;
	static unsigned char pop;

	if(Tnizsza(_MR_MADCW))	//10
	{
		pop=0;						//20wartosc poprawki na przesterowanie
		rgl.xmod=FirstXMOD2();
	}
	if(rgl.fz)						//aktualnie faza 2?
	{
		if(Tnizsza(2))				//Tnizsza(2)przygotowanie do przejscia do fazy 1
		{
			rgl.xmod=(PWM.BufPWM3>>2);
			rgl.srp=0;
			rgl.ytmp=0;	
			rgl.del1s=0;
			pop=0;
		}
		else return;
	}
	if(!Tnizsza(1)) 				//warunek przejscia do fazy 2
	{
		rgl.fz=1;
		rgl.fast_up=0;
		rgl.med_up=0;
		rgl.srp=(float)(PWM.BufPWM3+pop);
		rgl.xmod=((PWM.BufPWM3+pop)>>2);
		pop=0;
		if(rgl.xmod>PWM.MAXPWM)
		{
			rgl.srp=(float)(PWM.MAXPWM<<2);
			rgl.xmod=PWM.MAXPWM;
		}
		return;
	}
	//kll=rgl.srp;
	if(rgl.xmod>=PWM.MINPWM)	m=(rgl.xmod-PWM.MINPWM)*10;
	else m=0;
	m=m/(signed long)(DtCW.MAD_CW-DtCW.DAD_CW);	//DAD_CW-MAD_CW
	m=m*(signed long)(DtCW.AC_CW-DtCW.DAD_CW);	//DAD_CW-AC_CW	
	if(m<0) m=0;
	m=m/10+PWM.MINPWM;
	if(m>0xff)						//przepelnienie 
	{
		rgl.fz=0;					//przejscie do fazy1 przy aktywnej fazie2
        rgl.tmp_pwm=(rgl.xmod<<2);
		return;
	}
	if(rgl.fz&&(rgl.xmod>m))
    {
		rgl.srp=(float)((rgl.xmod-m)<<2); 		//przygotowanie lagodnego przejscia do fazy 1
    }
    dt=((unsigned int)(round(rgl.srp)))>>2;
	m+=dt;
	if(m>rgl.xmod) p=rgl.xmod;
	else p=(unsigned char)m;
	rgl.fz=0;						//przejscie do fazy1 przy aktywnej fazie2
    rgl.tmp_pwm=(p<<2);
}
*/
void ModZgCW2(void)
{
	float m;
    unsigned int dt;
	unsigned char p;
	static unsigned char pop;

	if(Tnizsza(_MR_MADCW))	//10
	{
		pop=0;						//20wartosc poprawki na przesterowanie
		rgl.xmod=FirstXMOD2();
	}
	if(rgl.fz)						//aktualnie faza 2?
	{
		if(Tnizsza(2))				//Tnizsza(2)przygotowanie do przejscia do fazy 1
		{
			rgl.xmod=(PWM.BufPWM3>>2);
			rgl.srp=0;
			rgl.ytmp=0;	
			rgl.del1s=0;
			pop=0;
		}
		else return;
	}
	if(!Tnizsza(1)) 				//warunek przejscia do fazy 2
	{
		rgl.fz=1;
		rgl.fast_up=0;
		rgl.med_up=0;
		rgl.srp=(float)(PWM.BufPWM3+pop);
		rgl.xmod=((PWM.BufPWM3+pop)>>2);
		pop=0;
		if(rgl.xmod>PWM.MAXPWM)
		{
			rgl.srp=(float)(PWM.MAXPWM<<2);
			rgl.xmod=PWM.MAXPWM;
		}
		return;
	}
	//kll=rgl.srp;
    if((DtCW.MAD_CW-DtCW.DAD_CW)!=0)
    {
        m=(DtCW.AC_CW-DtCW.DAD_CW)/(DtCW.MAD_CW-DtCW.DAD_CW);	//DAD_CW-AC_CW	
    }
    else
    {
        m=0;
    }
	
	m=m*(rgl.xmod-PWM.MINPWM)+PWM.MINPWM;
    if(m<0) m=PWM.MINPWM;
	if(m>0xff)						//przepelnienie 
	{
		rgl.fz=0;					//przejscie do fazy1 przy aktywnej fazie2
        rgl.tmp_pwm=(rgl.xmod<<2);
		return;
	}
	if(rgl.fz&&(rgl.xmod>m))
    {
        p=(unsigned char)m;
		rgl.srp=(float)((rgl.xmod-p)<<2); 		//przygotowanie lagodnego przejscia do fazy 1
    }
    dt=(unsigned int)(round(rgl.srp));
	m+=(dt>>2);
	if(m>rgl.xmod) p=rgl.xmod;
	else p=(unsigned char)m;
	rgl.fz=0;						//przejscie do fazy1 przy aktywnej fazie2
    rgl.tmp_pwm=(p<<2);
}
#endif
//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
//FAZA2
//drugie przyblizenie (tylko dla fazy 2)
#if (_HIRESVNT==0)
void ModDkCW2(void)
{
	signed long m;	
	signed char ada;
	unsigned char p;    
    float k;

	
	if(!rgl.fz)
	{
		return;								//wyjscie jesli aktualnie faza 1
	}
	ada=DtCW.AC_CW-DtCW.DAD_CW;
	//-------------------------------
    if(!DtKNF.rfan||(DtKNF.rfan==2))                         //FIME?
    {
        k=3.0;									//6*********************************
        if(ada<0) k=2.0;							//5*********************************
        if(DtKNF.rfan==2)
        {
            k=Format_kCW2(k);
        }
    }
    else                                    //NG40
    {
        k=6.0;									//6*********************************
        if(ada<0) k=5.0;							//5*********************************        
    }
    
	m=(signed long)(round(rgl.srp+k*ada));
	if(m>rgl.xmod) p=rgl.xmod;
	else
	if(m<PWM.MINPWM) p=PWM.MINPWM;
	else p=(unsigned char)m;  
	rgl.tmp_pwm=p;
}
#else
//drugie przyblizenie (tylko dla fazy 2)
void ModDkCW2(void)
{
	signed long m;	
	signed char ada;
    unsigned int p;    
	float k;
	
	if(!rgl.fz)
	{
		return;								//wyjscie jesli aktualnie faza 1
	}
	ada=DtCW.AC_CW-DtCW.DAD_CW;
	//-------------------------------
    if(!DtKNF.rfan||(DtKNF.rfan==2))                         //FIME?
    {
        k=3.0;									//6*********************************
        if(ada<0) k=2.0;							//5*********************************
        if(DtKNF.rfan==2)
        {
            k=Format_kCW2(k);
        }        
    }
    else                                    //NG40
    {
        k=6.0;									//6*********************************
        if(ada<0) k=5.0;							//5*********************************        
    }
    k=k*_HIWSP;
	m=(signed long)(round(rgl.srp+k*ada));
	if(m>(rgl.xmod<<2)) p=(rgl.xmod<<2);
	else
	if(m<(PWM.MINPWM<<2)) p=(PWM.MINPWM<<2);
	else p=(unsigned int)m;
	rgl.tmp_pwm=p;    
}
#endif
//-------------------------------------------------------------
//FAZA1+FAZA2
//trzecie przyblizenie
//nowa wartosc punktu rownowagi srp.
//wykonywane co krok czasowy
#if (_HIRESVNT==0)
void ModPruCW2(void)
{
	static unsigned char l,m,o,p,q,r,s;
	unsigned char tmp;
	signed char ada,k;

	if(!rgl.fz)											//aktualnie faza 1?
	{
		l=0;
		m=0;
		o=0;
		p=0;
		if(q<255) q++;									//licznik sekund 
		if(s<255) s++;									//licznik sekund 
		if(DtCW._newPCW) s=0;							//zmieniono nastawe?
		if(Tnizsza(0))									//Tcw<Tnst?
		{
			if(!Tnizsza(10))							//Tnst-Tcw<=10 ?
			{
				k=0;
                if(!DtKNF.rfan||(DtKNF.rfan==2))                         //FIME?
                {
                   if((Trosnie(1)||TFrosnie(1))) 	//szybki narost temperatury
                    {
                        if(s>=1)
                        {
                            s=0;
                            q=0;
                            k=5;
                            if(Trosnie(2)||TFrosnie(2)) k=4;	//8b.szybki narost temperatury?
                        }
                    }
                    else
                    if(TFmaleje(0))
                    {
                        if(q>=1)
                        {
                            s=0;
                            q=0;
                            k=3;
                        }
                    }
                    else
                    if(TFstoi())
                    {
                        if(r>=1)
                        {
                            if(!Trosnie(0)) k=15;
                            else k=5;
                            r=0;
                        }
                        else
                        {
                            k=0;
                            r++;
                        }
                    } 
                    if(DtKNF.rfan==2)
                    {
                        k=Format_kCW1(k);
                    }                   
                }
                else
                {                
                    if((Trosnie(1)||TFrosnie(1))) 	//szybki narost temperatury
                    {
                        if(s>=1)
                        {
                            s=0;
                            q=0;
                            k=10;
                            if(Trosnie(2)||TFrosnie(2)) k=8;	//8b.szybki narost temperatury?
                        }
                    }
                    else
                    if(TFmaleje(0))
                    {
                        if(q>=1)
                        {
                            s=0;
                            q=0;
                            k=6;
                        }
                    }
                    else
                    if(TFstoi())
                    {
                        if(r>=1)
                        {
                            if(!Trosnie(0)) k=30;
                            else k=10;
                            r=0;
                        }
                        else
                        {
                            k=0;
                            r++;
                        }
                    }
                }
				rgl.ytmp=0;
         
				if(PWM.BufPWM3<rgl.xmod) tmp=rgl.xmod-PWM.BufPWM3;
				else tmp=0;
				if(tmp&&k>tmp) k=tmp;
				if(rgl.srp+k>PWM.MAXPWM) rgl.srp=PWM.MAXPWM;
				else rgl.srp+=k;
				return;
			}
			else return;
		}
		else return;
	}
	else //aktualnie faza 2
	{
		q=0;
		r=0;
		s=0;
		if(m<255) m++;				//licznik sekund dla spadku temp CO
		if(p<255) p++;				//licznik sekund dla spadku temp CO
		if(o<255) o++;				//licznik sekund dla narostu temp CO
//------------
		if(DtCW.DAD_CW>DtCW.AC_CW)					//Tcw>Tnst ?
		{
			k=0;
			if(TFmaleje(0)&&!Twyzsza(0))		//(Tmaleje(0)||TFmaleje(0)
			{
				if(p>=2)
				{
					p=0;
					o=0;
					m=0;
					k=-1;
				}								
			}
			else
			if(TFrosnie(0))
			{	
				if(o>=2)
				{
					o=0;
					p=0;
					m=0;			
					k=1;
				}
			}
			else
			if(Trosnie(0))
			{	
				if(m>=2)
				{
					p=0;
					o=0;
					m=0;
					k=1;
				}		
			}
			else
			if(Twyzsza(2))				//gdy jest powyzej zakresu nastawa+1
			{
				if(Tstoi()&&TFstoi())
				{
					if(l>=2)
					{
						k=2;
						l=0;
					}
					else
					{
						k=0;
 						l++;
					}
				}
			} else l=0;
            //-------------------------------------------------------------
            if(DtKNF.rfan==2)
            {
                k=Format_kCW3(k);
            }   
			if(rgl.srp-k>PWM.MAXPWM) rgl.srp=PWM.MAXPWM; 
            else
			if(rgl.srp-k<PWM.MINPWM) rgl.srp=PWM.MINPWM;
			else rgl.srp-=k;
			if(rgl.xmod<rgl.srp) rgl.xmod=rgl.srp;
			return;
		}
//------------
		else										//Tcw<Tnst
		{
			k=0;
			if(TFrosnie(0)&&!Tnizsza(0)) 						//(Trosnie(0)||TFrosnie(0))
			{
				if(o>=2)
				{
					o=0;
					p=0;
					l=0;
					m=0;
					k=-1;
				}
			}
			else
			if(TFmaleje(0))
			{	
				if(p>=2)
				{
					p=0;
					o=0;
					l=0;
					m=0;			
					k=1;
				}
			}
			else
			if(Tmaleje(0))
			{	
				if(m>=2)
				{
					p=0;
					o=0;
					l=0;
					m=0;
					k=1;
				}
			}
			else
			if(Tnizsza(0))
			{		
				if(Tstoi()&&TFstoi())
				{
					if(l>=3)
					{
						k=2;
						l=0;
					}
					else
					{
						k=0;
 						l++;
					}
				}
			} else l=0;
            //-------------------------------------------------------------
            if(DtKNF.rfan==2)
            {
                k=Format_kCW3(k);
            }      
            if(rgl.srp+k<PWM.MINPWM) rgl.srp=PWM.MINPWM;
            else
			if(rgl.srp+k>PWM.MAXPWM) rgl.srp=PWM.MAXPWM;
			else rgl.srp+=k;
			if(rgl.xmod<rgl.srp) rgl.xmod=rgl.srp;
			return;
		}
	}
}
#else
//trzecie przyblizenie
//nowa wartosc punktu rownowagi srp.
//wykonywane co krok czasowy
void ModPruCW2(void)
{
	static unsigned char l,m,o,p,q,r,s;
    unsigned int dt;
	unsigned char tmp;
	float k;

	if(!rgl.fz)											//aktualnie faza 1?
	{
		l=0;
		m=0;
		o=0;
		p=0;
		if(q<255) q++;									//licznik sekund 
		if(s<255) s++;									//licznik sekund 
		if(DtCW._newPCW) s=0;							//zmieniono nastawe?
		if(Tnizsza(0))									//Tcw<Tnst?
		{
			if(!Tnizsza(10))							//Tnst-Tcw<=10 ?
			{
				k=0;
                if(!DtKNF.rfan||(DtKNF.rfan==2))                         //FIME?
                {
                    if((Trosnie(1)||TFrosnie(1))) 	//szybki narost temperatury
                    {
                        if(s>=1)
                        {
                            s=0;
                            q=0;
                            k=5;
                            if(Trosnie(2)||TFrosnie(2)) k=4;	//8b.szybki narost temperatury?
                        }
                    }
                    else
                    if(TFmaleje(0))
                    {
                        if(q>=1)
                        {
                            s=0;
                            q=0;
                            k=3;
                        }
                    }
                    else
                    if(TFstoi())
                    {
                        if(r>=1)
                        {
                            if(!Trosnie(0)) k=15;
                            else k=5;
                            r=0;
                        }
                        else
                        {
                            k=0;
                            r++;
                        }
                    } 
                    if(DtKNF.rfan==2)
                    {
                        k=Format_kCW1(k);
                    }                    
                }
                else
                {                
                    if((Trosnie(1)||TFrosnie(1))) 	//szybki narost temperatury
                    {
                        if(s>=1)
                        {
                            s=0;
                            q=0;
                            k=10;
                            if(Trosnie(2)||TFrosnie(2)) k=8;	//8b.szybki narost temperatury?
                        }
                    }
                    else
                    if(TFmaleje(0))
                    {
                        if(q>=1)
                        {
                            s=0;
                            q=0;
                            k=6;
                        }
                    }
                    else
                    if(TFstoi())
                    {
                        if(r>=1)
                        {
                            if(!Trosnie(0)) k=30;
                            else k=10;
                            r=0;
                        }
                        else
                        {
                            k=0;
                            r++;
                        }
                    }
                }
                k=k*_HIWSP;
				rgl.ytmp=0;
				if(PWM.BufPWM3<(rgl.xmod<<2)) tmp=rgl.xmod-(PWM.BufPWM3>>2);
				else tmp=0;
				if(tmp&&k>tmp) k=(float)tmp;
                if((rgl.srp+k)<(PWM.MINPWM<<2)) rgl.srp=(float)(PWM.MINPWM<<2);
                else
				if((rgl.srp+k)>(PWM.MAXPWM<<2)) rgl.srp=(float)(PWM.MAXPWM<<2);
				else rgl.srp+=k;
				return;
			}
			else return;
		}
		else return;
	}
	else //aktualnie faza 2
	{
		q=0;
		r=0;
		s=0;
		if(m<255) m++;				//licznik sekund dla spadku temp CO
		if(p<255) p++;				//licznik sekund dla spadku temp CO
		if(o<255) o++;				//licznik sekund dla narostu temp CO
//------------
		if(DtCW.DAD_CW>DtCW.AC_CW)					//Tcw>Tnst ?
		{
			k=0;
			if(TFmaleje(0)&&!Twyzsza(0))		//(Tmaleje(0)||TFmaleje(0)
			{
				if(p>=2)
				{
					p=0;
					o=0;
					m=0;
					k=-1;
				}								
			}
			else
			if(TFrosnie(0))
			{	
				if(o>=2)
				{
					o=0;
					p=0;
					m=0;			
					k=1;
				}
			}
			else
			if(Trosnie(0))
			{	
				if(m>=2)
				{
					p=0;
					o=0;
					m=0;
					k=1;
				}		
			}
			else
			if(Twyzsza(2))				//gdy jest powyzej zakresu nastawa+1
			{
				if(Tstoi()&&TFstoi())
				{
					if(l>=2)
					{
						k=2;
						l=0;
					}
					else
					{
						k=0;
 						l++;
					}
				}
			} else l=0;
            if(DtKNF.rfan==2)
            {
                k=Format_kCW3(k);
            }     
            k=k*_HIWSP;
            if((rgl.srp-k)>(PWM.MAXPWM<<2)) rgl.srp=(float)(PWM.MAXPWM<<2);
            else
			if((rgl.srp-k)<(PWM.MINPWM<<2)) rgl.srp=(float)(PWM.MINPWM<<2);
			else rgl.srp-=k;
            dt=((unsigned int)(round(rgl.srp)))>>2;
            if(dt>PWM.MAXPWM) dt=PWM.MAXPWM;            
			if(rgl.xmod<dt) rgl.xmod=(unsigned char)dt;
			return;
		}
//------------
		else										//Tcw<Tnst
		{
			k=0;
			if(TFrosnie(0)&&!Tnizsza(0)) 						//(Trosnie(0)||TFrosnie(0))
			{
				if(o>=2)
				{
					o=0;
					p=0;
					l=0;
					m=0;
					k=-1;
				}
			}
			else
			if(TFmaleje(0))
			{	
				if(p>=2)
				{
					p=0;
					o=0;
					l=0;
					m=0;			
					k=1;
				}
			}
			else
			if(Tmaleje(0))
			{	
				if(m>=2)
				{
					p=0;
					o=0;
					l=0;
					m=0;
					k=1;
				}
			}
			else
			if(Tnizsza(0))
			{		
				if(Tstoi()&&TFstoi())
				{
					if(l>=3)
					{
						k=2;
						l=0;
					}
					else
					{
						k=0;
 						l++;
					}
				}
			} else l=0;
            if(DtKNF.rfan==2)
            {
                k=Format_kCW3(k);
            } 
            k=k*_HIWSP;
            if((rgl.srp+k)<(PWM.MINPWM<<2)) rgl.srp=(float)(PWM.MINPWM<<2);
            else            
			if((rgl.srp+k)>(PWM.MAXPWM<<2)) rgl.srp=(float)(PWM.MAXPWM<<2);
			else rgl.srp+=k;
            dt=((unsigned int)(round(rgl.srp)))>>2;
            if(dt>PWM.MAXPWM) dt=PWM.MAXPWM;            
			if(rgl.xmod<dt) rgl.xmod=(unsigned char)dt;
			return;
		}
	}
}
#endif
//-------------------------------------------------------------
//inkrementacja maksymalnej wartosci mocy na palniku
//wykonywane co krok czasowy
void IncMaxCW2(void)
{
	unsigned char k;
	
	//kontrola maksimum mocy dla fazy 2
	if(rgl.fz)										//faza 2 ?
	{
		if(rgl.xmod>PWM.MAXPWM) rgl.xmod=PWM.MAXPWM;
		return;
	}
	//kontrolowany narost mocy dla fazy 1
	if(TFrosnie(0)||Trosnie(0)) return;	
	//if(Trosnie(1)||TFrosnie(1)) return;				//(Tcw lub Tco rosnie szybciej niz 1K/sek)?
	k=5;
    k=k*_HIWSP;
	if(rgl.xmod+k>PWM.MAXPWM) rgl.xmod=PWM.MAXPWM;
	else rgl.xmod+=k;
	if(rgl.xmod>PWM.MAXPWM) rgl.xmod=PWM.MAXPWM;
}
//-------------------------------------------------------------
//reakcja superwizyjna
#if (_HIRESVNT==0)
void ModPrzCW2(void)
{
	unsigned char k;
	if(!rgl.nmod)									//nie jest w trybie nmod?
	{
		if(!Tmaleje(0)&&Twyzsza(3))					//(Tcw nie maleje ?)
		{
                if(!DtKNF.rfan||(DtKNF.rfan==2))                         //FIME?
                {
                    k=15;
                    if(DtKNF.rfan==2)
                    {
                        k=Format_kCW4(k);
                    }                    
                }
                else
                {
                    k=25;//25
                }               
				if(rgl.srp-k<PWM.MINPWM) rgl.srp=PWM.MINPWM;
				else rgl.srp-=k;
				rgl.nmod=1;							//wejdz w tryb nmod
		}
	}
	else											//jest w trybie nmod?
	{
		if(!Twyzsza(3))
		{
			if(Tmaleje(0)) rgl.nmod=0;					//jezeli Tcw maleje wyjdz z trybu nmod
			if(Tstoi()&&TFmaleje(0)) rgl.nmod=0;		//(Tcw stoi i Tco maleje) wyjdz z trybu nmod
			if(Tnizsza(3)) rgl.nmod=0;					
		}
	}
}
#else
//reakcja superwizyjna
void ModPrzCW2(void)
{
	float k;
    
    /*if(!rgl.nmod)					//nie jest w trybie nmod?
	{
		if(rgl.fz)					//faza 2 ?
		{
			if(!Tmaleje(0))			//(Tco nie maleje ?)
			{
				if(Twyzsza(4))		//Tco-Tnst>4 ?
				{
                    k=15;
                    if(DtKNF.rfan==2)
                    {
                        k=Format_kCW4(k);
                    }
                    if((rgl.srp-k)<(PWM.MINPWM<<2))
                    {
                        rgl.srp=(float)(PWM.MINPWM<<2);//10
                    }
                    else 
                    {
                        rgl.srp-=k;
                    }
					rgl.nmod=1;		//wejdz w tryb nmod
					return;
				}
				else return;
			}
			else return;
		}
		else return;
	}
	else							//jest w trybie nmod?
	{
		if(!Twyzsza(4))
		{
			if(Tmaleje(0)) rgl.nmod=0;					//jezeli Tcw maleje wyjdz z trybu nmod
			if(Tstoi()&&TFmaleje(0)) rgl.nmod=0;		//(Tcw stoi i Tco maleje) wyjdz z trybu nmod
			if(Tnizsza(4)) rgl.nmod=0;					
		}        
	}*/
	
    if(!rgl.nmod)									//nie jest w trybie nmod?
	{
		if(!Tmaleje(0)&&Twyzsza(5))					//(Tcw nie maleje ?)
		{
                if(!DtKNF.rfan||(DtKNF.rfan==2))                         //FIME?
                {
                    k=15;
                    if(DtKNF.rfan==2)
                    {
                        k=Format_kCW4(k);
                    }                    
                }
                else
                {
                    k=25;
                }
                k=k*_HIWSP;
				if((rgl.srp-k)<(PWM.MINPWM<<2)) rgl.srp=(float)(PWM.MINPWM<<2);
				else rgl.srp-=k;
				rgl.nmod=1;							//wejdz w tryb nmod
		}
	}
	else											//jest w trybie nmod?
	{
		if(!Twyzsza(5))
		{
			if(Tmaleje(0)) rgl.nmod=0;					//jezeli Tcw maleje wyjdz z trybu nmod
			if(Tstoi()&&TFmaleje(0)) rgl.nmod=0;		//(Tcw stoi i Tco maleje) wyjdz z trybu nmod
			if(Tnizsza(5)) rgl.nmod=0;					
		}
	}
}
#endif
//------------------------------------------------------------
//Zabezpieczenie od przegrzania w obwodzie CO
#if (_HIRESVNT==0) 
void PropCO2(void)
{
	if(!rgl.nmod)					//nie jest w trybie nmod?
	{
		PWM.PROP1=PWM.MAXPWM-1*((PWM.MAXPWM-PWM.MINPWM)/7);
		PWM.PROP2=PWM.MAXPWM-2*((PWM.MAXPWM-PWM.MINPWM)/7);
		PWM.PROP3=PWM.MAXPWM-3*((PWM.MAXPWM-PWM.MINPWM)/7);
		if(rgl.x_f==3)						//=86C ? 
		{
			if(rgl.tmp_pwm>PWM.MAXPWM)	                
			rgl.tmp_pwm=PWM.MAXPWM;			//=MGMOD
		}
		else 
		if(rgl.x_f==2)						//=87C ? 
		{
			if(rgl.tmp_pwm>PWM.PROP1)
			rgl.tmp_pwm=PWM.PROP1;	//-1/7
		}
		else
		if(rgl.x_f==1)						//=88C ? 
		{
			if(rgl.tmp_pwm>PWM.PROP2)
			rgl.tmp_pwm=PWM.PROP2;	//-2/7
		}
		else
		if(rgl.x_f<=0) 						//>=89C ?
		{
			if(rgl.tmp_pwm>PWM.PROP3)
			rgl.tmp_pwm=PWM.PROP3;	//-3/7
		}
		if(rgl.tmp_pwm<PWM.MINPWM) rgl.tmp_pwm=PWM.MINPWM;
		if(rgl.tmp_pwm>PWM.MAXPWM) rgl.tmp_pwm=PWM.MAXPWM;
#if _PID_VNT==0        
        WritePWM3(rgl.tmp_pwm);
#else
        if(rgl.tmp_pwm<=PWM.MINPWM)
        {

            WritePWM3(VMinToPWM(0));
        }
        else
        if(rgl.tmp_pwm>=PWM.MAXPWM)
        {                        
            WritePWM3(VMaxToPWM(0));
        }   
        else
        {
            ResetVntToPWM();
            WritePWM3(rgl.tmp_pwm);
        }    
#endif        
	}
	else 
    {
        if(rgl.tmp_pwm<PWM.MINPWM+5)
        {
            WritePWM3(PWM.MINPWM);//PWM.BufPWM3=PWM.MINPWM;			//wymus minimum plomienia
        }
        else
        {
            if(rgl.tmp_pwm>PWM.MINPWM+5)
            {
                WritePWM3(PWM.MINPWM+5);//PWM.BufPWM3=PWM.MINPWM;			//wymus minimum plomienia
            }
        }
    }
}
#else
void PropCO2(void)
{
	if(!rgl.nmod)					//nie jest w trybie nmod?
	{
		PWM.PROP1=PWM.MAXPWM-1*((PWM.MAXPWM-PWM.MINPWM)/7);
		PWM.PROP2=PWM.MAXPWM-2*((PWM.MAXPWM-PWM.MINPWM)/7);
		PWM.PROP3=PWM.MAXPWM-3*((PWM.MAXPWM-PWM.MINPWM)/7);
		if(rgl.x_f==3)						//=86C ? 
		{
			if(rgl.tmp_pwm>(PWM.MAXPWM<<2))	                
			rgl.tmp_pwm=(PWM.MAXPWM<<2);			//=MGMOD
		}
		else 
		if(rgl.x_f==2)						//=87C ? 
		{
			if(rgl.tmp_pwm>(PWM.PROP1<<2))
			rgl.tmp_pwm=(PWM.PROP1<<2);	//-1/7
		}
		else
		if(rgl.x_f==1)						//=88C ? 
		{
			if(rgl.tmp_pwm>(PWM.PROP2<<2))
			rgl.tmp_pwm=(PWM.PROP2<<2);	//-2/7
		}
		else
		if(rgl.x_f<=0) 						//>=89C ?
		{
			if(rgl.tmp_pwm>(PWM.PROP3<<2))
			rgl.tmp_pwm=(PWM.PROP3<<2);	//-3/7
		}
		if(rgl.tmp_pwm<(PWM.MINPWM<<2)) rgl.tmp_pwm=(PWM.MINPWM<<2);
		if(rgl.tmp_pwm>(PWM.MAXPWM<<2)) rgl.tmp_pwm=(PWM.MAXPWM<<2);
#if _PID_VNT==0        
        WritePWM3H(rgl.tmp_pwm);
#else
        if(rgl.tmp_pwm<=(PWM.MINPWM<<2))
        {

            WritePWM3H(VMinToPWM(0));
        }
        else
        if(rgl.tmp_pwm>=(PWM.MAXPWM<<2))
        {                        
            WritePWM3H(VMaxToPWM(0,1));
            //WritePWM3H(PWM.MAXPWM<<2);
        }   
        else
        {
            ResetVntToPWM();
            WritePWM3H(rgl.tmp_pwm);
        }    
#endif        
	}
	else 
    {
        WritePWM3H(VMinToPWM(0));
        /*if(rgl.tmp_pwm<((PWM.MINPWM+5)<<2))
        {
            WritePWM3H((PWM.MINPWM<<2));//PWM.BufPWM3=PWM.MINPWM;			//wymus minimum plomienia
        }
        else
        {
            if(rgl.tmp_pwm>((PWM.MINPWM+5)<<2))
            {
                WritePWM3H((PWM.MINPWM+5)<<2);//PWM.BufPWM3=PWM.MINPWM;			//wymus minimum plomienia
            }
        }*/
    }    
}
#endif
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//Procedury modulacji dla potrzeb regulacji - wartosci graniczne
//------------------------------------------------------------
const unsigned char kdw=_VNT_KDW;
const unsigned char kup=_VNT_KUP;
#if (_HIRESVNT==0)||(_PID_VNT==0)
void NewPop_MIN(void)
{
	static unsigned char bpv,t1,t2,bw1,bw2;
	unsigned int v,pv;
	unsigned char w;

	v=M.rVVNT;						//zapamietaj predkosc chwilowa
	pv=DtKNF.mmin*100;				//zapamietaj predkosc docelowa
	SetNstVNT(DtKNF.mmin);

	if(rgl.tmp_pwm<=PWM.MINPWM)
	{
		//poprawka w dol
		if((!VTmalejacy(0)&&!Vmaleje(0)))//||Vstoi())
		{
			if(v>pv+60)
			{
				t1=0;
				if((v-pv)>100) w=3;
				else w=6;
				if(bw2) w=8; //wlasnie zmniejszono nastawe? - opoznij reakcje (jednorazowo)

				if(++t2>w)
				{
					t2=0;
					bw1=1;
					bw2=0;
					if(popMIN>-50) popMIN-=kdw;	//1
				}
			}
		}
		//poprawka w gore
		if((!VTrosnacy(0)&&!Vrosnie(0)))//||Vstoi());
		{
			if(v<pv-30)
			{
				t2=0;
				if((pv-v)>100) w=5;
				else w=8;
				if(bw1) w=10;	//wlasnie zwiekszono nastawe? - opoznij reakcje (jednorazowo)
				if(++t1>w)
				{
					t1=0;
					bw1=0;
					bw2=1;
					if(popMIN<50) popMIN+=kup;	//2
				}
			}
		} 
	}
}
//------------------------------------------------------------
//Poprawka dla wartosci maksymalnej granicznej predkosci obrotowej w obiegu CW
void NewPop_MAX_CW(void)
{
	static unsigned char bpv,t1,t2,bw1,bw2;
	unsigned int v,pv;
	unsigned char w,ppv;

	v=M.rVVNT;							//zapamietaj predkosc chwilowa
	ppv=ProcToVNT(DtKNF.mxcw);
	SetNstVNT(ppv);
	pv=ppv*100;						//zapamietaj predkosc docelowa

	if(rgl.tmp_pwm>=PWM.MAXPWM)
	{
		//poprawka w dol
		if((!VTmalejacy(0)&&!Vmaleje(0)))//||Vstoi())
		{
			if(v>pv+60)
			{
				t1=0;
				if((v-pv)>100) w=3;
				else w=6;
				if(bw2) w=8; //wlasnie zmniejszono nastawe? - opoznij reakcje (jednorazowo)
				if(++t2>w)
				{
					t2=0;
					bw1=1;
					bw2=0;
					if(popMAXCW>-50) popMAXCW-=kdw;	//1
				}
			}
		}
		//poprawka w gore
		if((!VTrosnacy(0)&&!Vrosnie(0)))//||Vstoi());
		{
			if(v<pv-30)
			{
				t2=0;
				if((pv-v)>100) w=5;
				else w=8;
				if(bw1) w=10;	//wlasnie zwiekszono nastawe? - opoznij reakcje (jednorazowo)
				if(++t1>w)
				{
					t1=0;
					bw1=0;
					bw2=1;
					if(popMAXCW<50) popMAXCW+=kup;	//2
				}
			}
		} 
	}
}
//------------------------------------------------------------
//Poprawka dla wartosci maksymalnej granicznej predkosci obrotowej w obiegu CW
void NewPop_MAX_CO(void)
{
	static unsigned char bpv,t1,t2,bw1,bw2;
	unsigned int v,pv;
	unsigned char w,ppv;

	v=M.rVVNT;						//zapamietaj predkosc chwilowa
	ppv=ProcToVNT(DtKNF.mxco);
	SetNstVNT(ppv);
	pv=ppv*100;						//DtKNF.mxco zapamietaj predkosc docelowa


	if(rgl.tmp_pwm>=PWM.MAXPWM)
	{
		//poprawka w dol
		if((!VTmalejacy(0)&&!Vmaleje(0)))//||Vstoi())
		{
			if(v>pv+60)
			{
				t1=0;
				if((v-pv)>100) w=3;
				else w=6;
				if(bw2) w=8; //wlasnie zmniejszono nastawe? - opoznij reakcje (jednorazowo)
				if(++t2>w)
				{
					t2=0;
					bw1=1;
					bw2=0;
					if(popMAXCO>-50) popMAXCO-=kdw;	//1
				}
			}
		}
		//poprawka w gore
		if((!VTrosnacy(0)&&!Vrosnie(0)))//||Vstoi());
		{
			if(v<pv-30)
			{
				t2=0;
				if((pv-v)>100) w=5;
				else w=8;
				if(bw1) w=10;	//wlasnie zwiekszono nastawe? - opoznij reakcje (jednorazowo)
				if(++t1>w)
				{
					t1=0;
					bw1=0;
					bw2=1;
					if(popMAXCO<50) popMAXCO+=kup;	//2
				}
			}
		} 
	}
}
//------------------------------------------------------------
//Nowa minimalna wartosc graniczna
void New_MIN(void)
{
	if(PWM.MINPWM+popMIN<PWM.VLO_PWM) PWM.MINPWM=PWM.VLO_PWM;
	else
	if(PWM.MINPWM+popMIN>PWM.MAXPWM) PWM.MINPWM=PWM.MAXPWM;
	else PWM.MINPWM=PWM.MDMOD+popMIN;
    PWM.MINPWM=PWM.MDMOD+popMIN;
}
//------------------------------------------------------------
//Nowe maksymalna wartosc graniczna dla CW
void New_MAX_CW(void)
{
	if(PWM.MAXPWM+popMAXCW<PWM.MDMOD) PWM.MAXPWM=PWM.MDMOD;
	else
	if(PWM.MAXPWM+popMAXCW>0xFF) PWM.MAXPWM=0xFF;
	else PWM.MAXPWM=PWM.MMAXCW+popMAXCW;
}
//------------------------------------------------------------
//Nowe maksymalna wartosc graniczna dla CO
void New_MAX_CO(void)
{
	if(PWM.MAXPWM+popMAXCO<PWM.MDMOD) PWM.MAXPWM=PWM.MDMOD;
	else
	if(PWM.MAXPWM+popMAXCO>0xFF) PWM.MAXPWM=0xFF;
	else PWM.MAXPWM=PWM.MMAXCO+popMAXCO;
}
#else   //_HIRESVNT
//------------------------------------------------------
unsigned int FormatVPWM(float pwm)
{
    if(pwm>(float)PWM.HMGMOD)
    {
        return (unsigned int)PWM.HMGMOD;    
    }
    else
    if(pwm<(float)(PWM.VLO_PWM<<2))
    {
        return (unsigned int)(PWM.VLO_PWM<<2);        
    }
    else
    {
        return (unsigned int)fSMOD;		//wartosc graniczna modulacji
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
//------------------------------------------------------------
unsigned int VMinToPWM(unsigned char ref)
{
    static unsigned char bpv=0;
	unsigned char pv;
    float vpwm;
    
    if(ref)
    {
        bpv=0;
        fpopSMOD=0;
        return 0;
    }
	pv=DtKNF.mmin;                //zapamietaj predkosc docelowa
    
	if(RTdS(_RTMOD2)>1)
	{
		StartRTdS(_RTMOD2);
        fpopSMOD=fpopSMOD+StrPID(pv*100);        //poprawka do wartosci granicznej minimalnej
        vpwm=(float)(PWM.MINPWM<<2);
        fSMOD=vpwm+fpopSMOD;		//teoretyczna oczekiwana wartosc PWM dla pv 
        if(fSMOD>PWM.HIPWM3) 
        {
            fpopSMOD=PWM.HIPWM3-vpwm;
        }        
        //kontrolowana zmiana obrotow z maksimum do minimum
        if(bpv!=pv)
        {
            fpopSMOD=0;
            
            if(!bpv) bpv=M.VVNT;
            else
            {
                if(bpv>pv)
                {
                    if(bpv) bpv--;
                }
                else 
                {
                    if(bpv<0xff) bpv++;
                }
            }
        }
	}
    return FormatVPWM(fSMOD);
}
//------------------------------------------------------------
unsigned int VMaxToPWM(unsigned char ref, unsigned char cw)
{
    volatile static unsigned char bpv=0;
	volatile unsigned char pv;
    volatile float vpwm;
    
    if(ref)
    {
        bpv=0;
        fpopSMOD=0;
        return 0;
    }
	//pv=DtKNF.mmax;                //zapamietaj predkosc docelowa
    if(!cw)
    {
        pv=ProcToVNT(DtKNF.mxco);
    }
    else
    {
        pv=ProcToVNT(DtKNF.mxcw);        
    }
	if(RTdS(_RTMOD2)>1)
	{
		StartRTdS(_RTMOD2);
        fpopSMOD=fpopSMOD+StrPID(pv*100);        //poprawka do wartosci granicznej minimalnej
        vpwm=(float)(PWM.MAXPWM<<2);
        fSMOD=vpwm+fpopSMOD;		//teoretyczna oczekiwana wartosc PWM dla pv
        if(fSMOD>PWM.HIPWM3) 
        {
            fpopSMOD=PWM.HIPWM3-vpwm;
        }         
        //kontrolowana zmiana obrotow z maksimum do minimum
        if(bpv!=pv)
        {
            fpopSMOD=0;
            
            if(!bpv) bpv=M.VVNT;
            else
            {
                if(bpv>pv)
                {
                    if(bpv) bpv--;
                }
                else 
                {
                    if(bpv<0xff) bpv++;
                }
            }
        }
	}
    return FormatVPWM(fSMOD);
}
//------------------------------------------------------------
void ResetVntToPWM(void)
{
    VMinToPWM(1);
    VMaxToPWM(1,0);    
}
#endif
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//Procedury modulacji dla potrzeb konfiguracji i funkcji serwisowej
//------------------------------------------------------------
#if (_HIRESVNT==0)||(_PID_VNT==0)
//Nowa minimalna wartosc graniczna
//Procedury modulacji dla potrzeb konfiguracji i funkcji serwisowej
unsigned char New_SMOD(unsigned char pv)
{
	unsigned char pw;

	//pw=CalkVNTtoPWM(pv);
    switch(DtKNF.rfan)  //rodzaj wentylatora
    {
        case 0: //FIME
        case 2:             
        {
            pw=CalkVNTtoPWM(pv);	//******************************************
            break;
        }
        case 1: //NG40
        {
            pw=CalkVNTtoPopPWM(pv,2);	//******************************************              
            break;
        }
        default:
        {
            pw=CalkVNTtoPWM(pv);	//******************************************
            break;            
        }
    }
	if(pw+popSMOD<PWM.VLO_PWM) pw=PWM.VLO_PWM;
	else
	if(pw+popSMOD>PWM.MGMOD) pw=PWM.MGMOD;
	else pw=pw+popSMOD;
	return pw;
}
//------------------------------------------------------------
//------------------------------------------------------------
//pv-oczekiwana predkosc wentylatora
void NewPop_SMOD(unsigned char ppv)
{
	unsigned int v,pv;
	unsigned char pw,w,mrg,krk;
	static unsigned char bpv,t1,t2,bw1,bw2;

//---------------------
	SetNstVNT(ppv);
	v=M.rVVNT;					//zapamietaj predkosc chwilowa
	//pw=CalkVNTtoPWM(ppv);		//teoretyczna oczekiwana wartosc PWM dla pv
								//******************************************
    switch(DtKNF.rfan)  //rodzaj wentylatora
    {
        case 0: //FIME
        case 2:            
        {
            pw=CalkVNTtoPWM(ppv);	//teoretyczna oczekiwana wartosc PWM dla pv
            break;
        }
        case 1: //NG40
        {
            pw=CalkVNTtoPopPWM(ppv,2);	//teoretyczna oczekiwana wartosc PWM dla pv              
            break;
        }
        default:
        {
            pw=CalkVNTtoPWM(pv);	//******************************************
            break;            
        }        
    }    
    
	pv=ppv*100;
//---------------------
//rodzaj sytuacji
	if(pw<=PWM.MDMOD) mrg=1;	//oczekiwana dolna granica modulacji
	else 
	if(pw>=PWM.MGMOD) mrg=2;	//oczekiwana gorna granica modulacji
	else mrg=0;					//oczekiwana dowolna wartosc miedzy granicami modulacji
//---------------------
	//modyfikacja aktualnej wartosci poprawki po zmianie nastawy
	if(bpv!=ppv)					//nastapila zmiana nastawy?
	{
		t1=0;
		t2=0;
		bw1=0;
		bw2=0;
		//ustawienie znacznika rodzaju zmiany nastawy
		if(bpv>ppv)				//zmniejszono nastawe?
		{
			{
				if(popSMOD>0)		//aktualnie poprawka >0?
				{
					popSMOD=popSMOD-(bpv-ppv);
					if(popSMOD<0) {bw2=1;popSMOD=0;}
				}
				else
				{
					popSMOD=popSMOD+(bpv-ppv); 
					if(popSMOD>0) {bw2=1;popSMOD=0;}
				}
			}
		}
		else					//zwiekszono nastaw?
		{
			if(popSMOD>0)		//aktualnie poprawka >0?
			{
				popSMOD=popSMOD-(ppv-bpv);
				if(popSMOD<0) {bw1=1;popSMOD=0;}
			}
			else
			{
				popSMOD=popSMOD+(ppv-bpv);
				if(popSMOD>0) {bw1=1;popSMOD=0;}
			}
		}
		if(popSMOD>0) popSMOD--;
		if(popSMOD<0) popSMOD++;
		bpv=ppv;
	}
	//-------------------------------------------------------------
	//bw2=1 - zmniejszono nastawe (jednorazowo opoznij reakcje dla ew.poprawki w dol)
	//bw1=1 - zwiekszono nastawe (jednorazowo opoznij reakcje dla ew.poprawki w gore)
	//-------------------------------------------------------------
	//-------------------------------------------------------------
	//poprawka w dol
	if((!VTmalejacy(0)&&!Vmaleje(0)))//&&Vstoi()) //trend nie malejacy i predkosc nie maleje?
	{
		if((!mrg&&(v>pv+60))||((mrg==1)&&(v>=pv+60))||((mrg==2)&&(v>=pv+60)))	//predkosc za wysoka?
		{
			t1=0;	//zeruj timer modyfikacji poprawki w dol
			if((v-pv)>100) w=3;
			else w=6;
			if(bw2) w=8; //5wlasnie zmniejszono nastawe? - opoznij reakcje (jednorazowo)
			if(++t2>w)
			{
				t2=0;
				bw1=1;	//opoznij reakcje dla ew.poprawki w gore (jednorazowo)
				bw2=0;	//zeruj znacznik opoznionej reakcji
				if(popSMOD>-50) popSMOD-=kdw;		//1 zmniejsz wartosc poprawki
			}
		}
	}
	//-------------------------------------------------------------
	//poprawka w gore
	if((!VTrosnacy(0)&&!Vrosnie(0)))//&&Vstoi());	//trend nie rosnacy i predkosc nie rosnie?
	{
		if((!mrg&&(v<pv-30))||((mrg==1)&&(v<=pv-30))||((mrg==2)&&(v<=pv-30))) //predkosc za niska?
		{
			t2=0;	//zeruj timer modyfikacji poprawki w gore
			//---------------------------------------
			//modyfikacja poprawki w gore
			if((pv-v)>100) w=5;
			else w=8;
			if(bw1) w=10;	//5wlasnie zwiekszono nastawe? - opoznij reakcje (jednorazowo)
			if(++t1>w)
			{
				t1=0;
				bw1=0;	//zeruj znacznik opoznionej reakcji
				bw2=1;	//opoznij reakcje dla ew.poprawki w dol (jednorazowo)
				if(popSMOD<50) popSMOD+=kup;	//2 zwieksz wartosc poprawki
			}
		}
	} 
}
//-------------------------------------------------------------
//-------------------------------------------------------------
#if (_HIRESVNT==0)
unsigned char ProcToMod(unsigned char prvnt)
{
	unsigned char pv;

	pv=ProcToVNT(prvnt);		//zapamietaj predkosc docelowa
	if(RTdS(_RTMOD2)>1)
	{
		StartRTdS(_RTMOD2);
		NewPop_SMOD(pv); 		//poprawka do wartosci granicznej minimalnej
	}
	return New_SMOD(pv);		//wartosc graniczna modulacji
}
#else
unsigned int ProcToMod(unsigned char prvnt)
{
	unsigned char pv;

	pv=ProcToVNT(prvnt);		//zapamietaj predkosc docelowa
	if(RTdS(_RTMOD2)>1)
	{
		StartRTdS(_RTMOD2);
		NewPop_SMOD(pv); 		//poprawka do wartosci granicznej minimalnej
	}
	return (New_SMOD(pv)<<2);		//wartosc graniczna modulacji
}
#endif
#else   //_HIRESVNT
//-------------------------------------------------------------
//Nowa minimalna wartosc graniczna
//Procedury modulacji dla potrzeb konfiguracji i funkcji serwisowej
unsigned int ProcToMod(unsigned char prvnt)
{
    static unsigned char bpv=0;
    static signed char pop=0;
	unsigned char pv;
    unsigned int rpv;
    float vpwm;
    
	pv=ProcToVNT(prvnt);                //zapamietaj predkosc docelowa
    //return CalkVNTtoPWMH(pv*100);
    rpv=pv*100;
    
	if(RTdS(_RTMOD2)>1)
	{
		StartRTdS(_RTMOD2);
        if(bpv<pv)
        {
            pop=-50;
        }
        else
        if(bpv>pv)            
        {
            pop=50;
        }        
        fpopSMOD=fpopSMOD+StrPID(rpv);        //poprawka do wartosci granicznej minimalnej
        vpwm=(float)CalkVNTtoPWMH(rpv+pop);
        fSMOD=vpwm+fpopSMOD;		//teoretyczna oczekiwana wartosc PWM dla pv 
        if(fSMOD>PWM.HIPWM3) 
        {
            fpopSMOD=PWM.HIPWM3-vpwm;
        }
        //kontrolowana zmiana obrotow z maksimum do minimum
        if(bpv!=pv)
        {
            fpopSMOD=0;
            if(!bpv) bpv=M.VVNT;
            else
            {
                if(bpv>pv)
                {
                    if(bpv) bpv--;
                }
                else 
                {
                    if(bpv<0xff) bpv++;
                }
            }
        }
	}
    return FormatVPWM(fSMOD);
}
//----------------------------------------
//----------------------------------------
/*
unsigned int ProcToRegMod(unsigned char ref,unsigned char prvnt)
{
    static unsigned char bpv=0;
	unsigned char pv;
    unsigned int rpv;
        
    if(ref)
    {
        bpv=0;
        return;
    }
	pv=ProcToVNT(prvnt);                //zapamietaj predkosc docelowa

    rpv=pv*100;
    
	if(RTdS(_RTMOD2)>1)
	{
		StartRTdS(_RTMOD2);
        fpopSMOD=fpopSMOD+StrPID(rpv);        //poprawka do wartosci granicznej minimalnej
        fSMOD=(float)CalkVNTtoPWMH(rpv)+fpopSMOD;		//teoretyczna oczekiwana wartosc PWM dla pv 
        //kontrolowana zmiana obrotow z maksimum do minimum
        if(bpv!=pv)
        {
            fpopSMOD=0;
            
            if(!bpv) bpv=M.VVNT;
            else
            {
                if(bpv>pv)
                {
                    if(bpv) bpv--;
                }
                else 
                {
                    if(bpv<0xff) bpv++;
                }
            }
        }
	}
    //kontrola wartosci granicznych
    if(fSMOD>(float)PWM.HMGMOD)
    {
        return PWM.HMGMOD;    
    }
    else
    if(fSMOD<(float)(PWM.VLO_PWM<<2))
    {
        return (PWM.VLO_PWM<<2);        
    }
    else
    {
        return (unsigned int)fSMOD;		//wartosc graniczna modulacji
    }
}
 */
#endif //_HIRESVNT
//-------------------------------------------------------------
//-------------------------------------------------------------

//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//zwraca wartosc 1 przy stwierdzonym wzroscie temperatury
//(w obiegu CW)
//dla t=0 zwraca wartosc jeden przy dowolnym wzroscie temp.
//dla t>0 zwraca wartosc jeden przy wroscie > od t [C/sek].
bit Trosnie(const unsigned char t)
{
	return (rgl.x_dt>t);
}
//------------------------------------------------------------
//zwraca wartosc 1 przy stwierdzonym spadku temperatury
//(w obiegu CW)
//dla t=0 zwraca wartosc jeden przy dowolnym spadku temp.
//dla t>0 zwraca wartosc jeden przy spadku > od t [C/sek].
bit Tmaleje(const unsigned char t)
{
	return (rgl.x_dt<(-1*t));
}
//------------------------------------------------------------
//zwraca wartosc 1 jezeli temperatura sie nie zmienia
//(w obiegu CW)
bit Tstoi(void)
{
	return !rgl.x_dt;
}

//------------------------------------------------------------
//zwraca wartosc 1 temperatura jes nizsza od nastawy o wartosc parametru t
//(w obiegu CW)
//dla t=0 zwraca wartosc jeden jezeli temp. jest nizsza od nastawy.
//dla t>0 zwraca wartosc jeden jezeli temp. jest nizsza od nastawy o >= t.
bit Tnizsza(const unsigned char t)
{
	return (rgl.x_t>t);
}
//------------------------------------------------------------
//zwraca wartosc 1 temperatura jes nizsza od nastawy o wartosc parametru t
//(w obiegu CW)
//dla t=0 zwraca wartosc jeden jezeli temp. jest nizsza od nastawy.
//dla t>0 zwraca wartosc jeden jezeli temp. jest nizsza od nastawy o >= t.
bit Twyzsza(const unsigned char t)
{
	return (rgl.x_t<(-1*t));
}
//------------------------------------------------------------
//zwraca wartosc 1 jezeli temperatura = nastawa.
//(w obiegu CW)
bit Trowna(void)
{
	return !rgl.x_t;
}
//------------------------------------------------------------
//zwraca wartosc 1 przy stwierdzonym wzroscie temperatury
//(w obiegu CO)
//dla t=0 zwraca wartosc jeden przy dowolnym wzroscie temp.
//dla t>0 zwraca wartosc jeden przy wroscie > od t [C/sek].
bit TFrosnie(const unsigned char t)
{
	return (rgl.x_df>t);
}
//------------------------------------------------------------
//zwraca wartosc 1 przy stwierdzonym spadku temperatury
//(w obiegu CO)
//dla t=0 zwraca wartosc jeden przy dowolnym spadku temp.
//dla t>0 zwraca wartosc jeden przy spadku > od t [C/sek].
bit TFmaleje(const unsigned char t)
{
	return (rgl.x_df<(-1*t));
}
//------------------------------------------------------------
//zwraca wartosc 1 jezeli temperatura sie nie zmienia
//(w obiegu CO)
bit TFstoi(void)
{
	return !rgl.x_df;
}
//------------------------------------------------------------
//Restart procedury modulatora
void RestartMod(void)
{
	rgl.btco=0;
	rgl.btcw=0;
	rgl.fz=0;
	rgl.med_up=0;
	rgl.fast_up=0;
	rgl.nmod=0;
	rgl.del1s=0;
	rgl.deln1=0;
	rgl.inv_mod=0;
}
//------------------------------------------------------------
//------------------------------------------------------------
#endif
