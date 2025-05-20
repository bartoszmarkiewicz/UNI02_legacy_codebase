/*pompa.c*/
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

#include "pompa.h"			//parametry lokalne
#include "konfig.h"

extern DtRgl rgl;
extern tpMAIN M;
extern DtPWM PWM;
extern DataIN DtIN;
extern DtKONF DtKNF;
extern tpRSDTA1 RSDT1;
extern DataCO DtCO;
extern unsigned char xxy,zzz,vvv,mmm;

DtPMP PMP;
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//Zwraca wartosc wypelnienia PWM dla podanej wartosci %
unsigned char ProcToPmpPwm(unsigned char prc)
{
	unsigned int p;

	if(prc>100) prc=100;
	p=PWM.HIPWM;
	p=(prc*p)/100;	//pmppwm/100 - ziarno przypadajace na 1%
	return (unsigned char)p;
}

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//Modulator ze wzgledu na delta T = DtIN.DDLT
unsigned char ModPmp1(unsigned char p)
{
	unsigned char ppwm, tim, pup, pdw;

	ppwm=p;

	if(rgl.fz||ppwm!=PMP.MGPROC)		//aktywny modulator pompy i faza 2 modulacji?
	{

		if(DtIN.DIN2>DtIN.DDLT)
		{
			tim=50;
			if(DtIN.DIN2-DtIN.DDLT==2) tim=100;
			else
			if(DtIN.DIN2-DtIN.DDLT==1) tim=150;				
		}
		else
		{
			tim=80;
			if(DtIN.DDLT-DtIN.DIN2==2) tim=100;
			else 
			if(DtIN.DDLT-DtIN.DIN2==1) tim=150;	
		}
		pup=0;
		pdw=0;
		if((RTdS(_RTPMOD)>tim)||(ppwm<=PMP.MDPROC)||(ppwm>=PMP.MGPROC))
		{
			StartRTdS(_RTPMOD);
			//-------------------------
			if(!pup)
			if(rgl.tmp_pwm<=PWM.MINPWM)
			{
				if(Twyzsza(2)||Tnizsza(2))
				{
					if(ppwm<PMP.MGPROC)
					{
						if(((ppwm+_PMPKUP)<PMP.MGPROC))
						{
							ppwm+=_PMPKUP;
							pup=1;
						}
						else ppwm=PMP.MGPROC;
					}
				}
			}
			//-------------------------
			if(rgl.tmp_pwm>PWM.MINPWM||((rgl.tmp_pwm<=PWM.MINPWM)&&!Twyzsza(1)))
			{
				if(DtIN.DIN<DtIN.DDLT)
				{
					if(!pdw)
					if(ppwm>PMP.MDPROC)
					{
						if(((ppwm-_PMPKDW)>PMP.MDPROC))
						{
							ppwm-=_PMPKDW;
							pdw=1;
						}
						else ppwm=PMP.MDPROC;
					}
					if(!pdw)
					if(DtIN.DIN<DtIN.DIN2)
					{
						if(ppwm>PMP.MDPROC)
						{
							if(((ppwm-_PMPKDW)>PMP.MDPROC))
							{
								ppwm-=_PMPKDW;
								pdw=1;
							}
							else ppwm=PMP.MDPROC;
						}
					}
				}
			}
			//-------------------------
			if(rgl.tmp_pwm<PWM.MAXPWM||((rgl.tmp_pwm>=PWM.MAXPWM)&&!Tnizsza(1)))
			{
				if(DtIN.DIN>DtIN.DDLT)
				{
					if(!pup)
					if(ppwm<PMP.MGPROC)
					{
						if(((ppwm+_PMPKUP)<PMP.MGPROC))
						{
							ppwm+=_PMPKUP;
							pup=1;
						}
						else ppwm=PMP.MGPROC;
					}
					if(!pup)
					if(DtIN.DIN>DtIN.DIN2)
					{
						if(ppwm<PMP.MGPROC)
						{
							if(((ppwm+_PMPKUP)<PMP.MGPROC))
							{
								ppwm+=_PMPKUP;
								pup=1;
							}
							else ppwm=PMP.MGPROC;
						}
					}
				}
				//-------------------------
			}				
		}
	}
	else
	{
		StartRTdS(_RTPMOD);
		ppwm=PMP.MGPROC;
	}
	return ppwm;
}
//--------------------------------------------------------------------------------------
//-----------------------------------------------------------------------
//Modulacja praca pompy w zaleznosci od wielkosci temp IN (delta zdefiniowana parametrem DtKNF.dtpm)
void RglPmp1(void)
{
	PMP.MGPROC=DtKNF.mgpm;		//100 gorny prog procentowy predkosci

	PMP.MDPROC=DtKNF.mdpm;		//dolny prog procentowy predkosci

    if(PMP.MDPROC>PMP.MGPROC) PMP.MDPROC=PMP.MGPROC;

	DtIN.DDLT=DtKNF.dtpm;		//docelowa granica delty

	if(DtKNF.tpkt==_UNICO)
	{
		PMP.MGPROC2=DtKNF.mgpm2;	//100 gorny prog procentowy predkosci dla obiegu CW
	}
	else
	{
		PMP.MGPROC2=99;
	}

	if(!M._PRACA)
	{
		DtIN.pop=0;
		StartRTS(_RTMPMP);
		if(_POMPA_GetValue())
		{
			if(!M._RPM_CW) PMP.APROC=PMP.MGPROC;	//pelny wybieg dla CO
			else PMP.APROC=PMP.MGPROC2;				//pelny wybieg dla CW
		}
		else
		{
			PMP.APROC=0;			//minimalizuj straty energii
		}
		WritePWM2(ProcToPmpPwm(PMP.APROC));
	}
	else
	{
		if(M._MODULUJ&&!RdPrt(S_ERR_IN)&&!M._RPM_CW&&RTS(_RTMPMP)>_TPMPBLK)			//obieg CO, po rozruchu po ustalonym czasie rozpocznij modulacje pompa
		{
			if(rgl.fz)		//dla fazy 1 narost lub spadek obrotow w rgl.c (IncMaxCO())
			{
				PMP.APROC=ModPmp1(PMP.APROC);
			}
		}
		else
		{
			if(!M._RPM_CW) PMP.APROC=PMP.MGPROC;	//pelny wybieg dla CO
			else PMP.APROC=PMP.MGPROC2;				//pelny wybieg dla CW
		}
		WritePWM2(ProcToPmpPwm(PMP.APROC));
	}
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//----------------------------------------ECO------------------------------------------
//--------------------------------------------------------------------------------------
//-----------------------------------------------------------------
//Zswraca mnizsza wartosc
signed char RetMin(signed char a, signed char b)
{
	if(a<=b) return a;
	return b;
}
//-----------------------------------------------------------------
//Zwraca wyzsza wartosc
signed char RetMax(signed char a, signed char b)
{
	if(a>=b) return a;
	return b;
}
//-----------------------------------------------------------------
//Zwraca wartosc HI zmodyfikowana o wsp ECO
//max - wartosc maksimum ktorej nie chcemy przekroczyc (max PPCO)
unsigned char RetHI(unsigned char pco, unsigned char eco, unsigned char max)
{
	signed char w;
	unsigned char k,emax;

	if(pco>max) emax=pco;
	else emax=max;

	k=(emax-DtIN.Eco5HI)/_PDEKO;
	w=(DtIN.Eco5HI+((_DFEKO-eco)*k));
	if(w<=emax&&w>=DtIN.Eco5LO) return w;
	else 
	if(w>emax) return emax;
	else return DtIN.Eco5LO;
}
//-----------------------------------------------------------------
//Zwraca wartosc LO zmodyfikowana o wsp ECO
//min - wartosc minimum ktorej nie chcemy przekroczyc (10C)
unsigned char RetLO(unsigned char pco, unsigned char eco, unsigned char min)
{
	signed char w;
	unsigned char k,ehi;

	if(pco<DtIN.Eco5HI) ehi=pco;
	else ehi=DtIN.Eco5HI;

	k=(ehi-DtIN.Eco5LO)/_PDEKO;
	w=(DtIN.Eco5LO+((_DFEKO-eco)*k));

	if(w<=ehi&&w>=min) return w;
	else 
	if(w>ehi) return ehi;
	else return min;
}
//-----------------------------------------------------------------
//Zwraca wartosc delty wzgledem gornej granicy HI
signed char RetDelta1(unsigned char pco,unsigned char hi)
{
	return (pco-hi);
}
//-----------------------------------------------------------------
//Zwraca wartosc delty bezwzglednej zmodyfikowanej o wsp ECO
//Dla ECO=5 jest to wartosc P13
signed char RetDelta2(unsigned char eco,unsigned char ddlt)
{
	unsigned char kD;

	kD=ddlt/_PDEKO;
	return (ddlt+(kD*(eco-_DFEKO)));
}
//-----------------------------------------------------------------
//Zwraca wartosc delty wzgledem dolnej granicy LO
signed char RetDelta3(unsigned char pco,unsigned char lo)
{
	return (pco-lo);
}
//-----------------------------------------------------------------
//Zwraca wartosc wypadkowa delty
unsigned char RetDelta4(unsigned char pco,unsigned char eco,unsigned char hi,unsigned char lo)
{
	signed char d;

	DtIN.delta1=RetDelta1(pco,hi);//DtIN.HI);
	DtIN.delta2=RetDelta2(eco,_ECODDL);
	DtIN.delta3=RetDelta3(pco,lo);//DtIN.LO);
	d=RetMin(DtIN.delta3,RetMax(DtIN.delta1,DtIN.delta2));
	if(d>0) return d;
	return 0;
}
//-----------------------------------------------------------------
//Zwraca sredni krok zmiany delty przy zmianie ECO o 1
unsigned char RetKrkDelta(unsigned char pco)
{
	unsigned char i,d,min,max;

	max=0;
	min=100;
	for(i=1;i<10;i++)
	{
		d=RetDelta4(pco,i,RetHI(pco,i,_MAXT),RetLO(pco,i,_MINT));
		if(d>max) max=d;
		if(d<min) min=d;
	}
	if(max<=min) return 0;
	return ((max-min)*10)/9;	
}
//-----------------------------------------------------------------
//Zwraca wartosc delty z uwzglednieniem sredniego kroku zmiany delty
unsigned char RetDelta(unsigned char pco,unsigned char eco)
{
	unsigned char k;

	k=RetKrkDelta(pco);
	if(eco&&k) return (k*(eco))/10;
	else return 0;
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//Modulator - wielkosc delty uzalezniona od parametru ECO i nastawy PPCO
unsigned char ModPmp2(unsigned char p)
{
	unsigned char ppwm, tim, pup, pdw;

	//parametry kluczowe modulacji
	DtIN.Eco5LO=_ECO5LO;						//dlona granica temp IN dla Eco=5
	DtIN.Eco5HI=_ECO5HI;						//gorna granica temp IN dla Eco=5
	if(DtIN.PPEco)
	{
		DtIN.HI=RetHI(M.CONTSETP,DtIN.PPEco-1,_MAXT);	//dlona granica IN dla parametru ECO i nastawy PPCO	
		DtIN.LO=RetLO(M.CONTSETP,DtIN.PPEco-1,_MINT);	//gorna granica IN dla parametru ECO i nastawy PPCO
		DtIN.delta=RetDelta(M.CONTSETP,DtIN.PPEco-1);	//wartosc delta dla parametru ECO i nastawy PPCO
	}
	else
	{
		DtIN.HI=0;
		DtIN.LO==0;
		DtIN.delta=0;
	}
	if(!DtIN.delta)						//dla delta=0 predkosc maksymalna pompy
	{
		StartRTdS(_RTPMOD);
		return PMP.MGPROC;
	}
	
	//modulacja praca pompy
	ppwm=p;

	if(rgl.fz||ppwm!=PMP.MGPROC)		//aktywny modulator pompy i faza 2 modulacji?
	{
		//wielkosc delty uzalezniona od parametru ECO i nastawy PPCO
		DtIN.cDLT=DtIN.delta;		
		
		//dobor kroku czasowego dla modyfikacji
		if(DtIN.DIN2>DtIN.cDLT)
		{
			tim=50;
			if(DtIN.DIN2-DtIN.cDLT==2) tim=100;
			else
			if(DtIN.DIN2-DtIN.cDLT==1) tim=150;				
		}
		else
		{
			tim=80;
			if(DtIN.cDLT-DtIN.DIN2==2) tim=100;
			else 
			if(DtIN.cDLT-DtIN.DIN2==1) tim=150;	
		}

		//dobor wypelnienia PWM w zaleznosci od delta T
		pup=0;
		pdw=0;
		if((RTdS(_RTPMOD)>tim)||(ppwm<=PMP.MDPROC)||(ppwm>=PMP.MGPROC))
		{
			StartRTdS(_RTPMOD);
			//-------------------------
			if(!pup)
			if(rgl.tmp_pwm<=PWM.MINPWM)
			{
				if(Twyzsza(2)||Tnizsza(2))
				{
					if(ppwm<PMP.MGPROC)
					{
						if(((ppwm+_PMPKUP)<PMP.MGPROC))
						{
							ppwm+=_PMPKUP;
							pup=1;
						}
						else ppwm=PMP.MGPROC;
					}
				}
			}
			//-------------------------
			if(rgl.tmp_pwm>PWM.MINPWM||((rgl.tmp_pwm<=PWM.MINPWM)&&!Twyzsza(1)))
			{
				if(DtIN.DIN<DtIN.cDLT)
				{
					if(!pdw)
					if(ppwm>PMP.MDPROC)
					{
						if(((ppwm-_PMPKDW)>PMP.MDPROC))
						{
							ppwm-=_PMPKDW;
							pdw=1;
						}
						else ppwm=PMP.MDPROC;
					}
					if(!pdw)
					if(DtIN.DIN<DtIN.DIN2)
					{
						if(ppwm>PMP.MDPROC)
						{
							if(((ppwm-_PMPKDW)>PMP.MDPROC))
							{
								ppwm-=_PMPKDW;
								pdw=1;
							}
							else ppwm=PMP.MDPROC;
						}
					}
				}
			}
			//-------------------------
			if(rgl.tmp_pwm<PWM.MAXPWM||((rgl.tmp_pwm>=PWM.MAXPWM)&&!Tnizsza(1)))
			{
				if(DtIN.DIN>DtIN.cDLT)
				{
					if(!pup)
					if(ppwm<PMP.MGPROC)
					{
						if(((ppwm+_PMPKUP)<PMP.MGPROC))
						{
							ppwm+=_PMPKUP;
							pup=1;
						}
						else ppwm=PMP.MGPROC;
					}
					if(!pup)
					if(DtIN.DIN>DtIN.DIN2)
					{
						if(ppwm<PMP.MGPROC)
						{
							if(((ppwm+_PMPKUP)<PMP.MGPROC))
							{
								ppwm+=_PMPKUP;
								pup=1;
							}
							else ppwm=PMP.MGPROC;
						}
					}
				}
				//-------------------------
			}				
		}
	}
	else
	{
		StartRTdS(_RTPMOD);
		ppwm=PMP.MGPROC;
	}
	return ppwm;
}
//--------------------------------------------------------------------------------------
//-----------------------------------------------------------------------
//Modulacja praca pompy w zaleznosci od temperatury IN  (delta uzalezniona od parametru ECO i nastawy PPCO)
void RglPmp2(void)
{
	PMP.MGPROC=DtKNF.mgpm;						//gorny prog procentowy predkosci
	PMP.MDPROC=DtKNF.mdpm;						//dolny prog procentowy predkosci

    if(PMP.MDPROC>PMP.MGPROC) PMP.MDPROC=PMP.MGPROC;

	if(DtKNF.tpkt==_UNICO)
	{
		PMP.MGPROC2=DtKNF.mgpm2;	//100 gorny prog procentowy predkosci dla obiegu CW
	}
	else
	{
		PMP.MGPROC2=99;
	}

	if(!M._PRACA)
	{
		DtIN.pop=0;
		StartRTS(_RTMPMP);
		if(_POMPA_GetValue())
		{
			if(!M._RPM_CW) PMP.APROC=PMP.MGPROC;	//pelny wybieg dla CO
			else PMP.APROC=PMP.MGPROC2;				//pelny wybieg dla CW
		}
		else
		{
			PMP.APROC=0;			//minimalizuj straty energii
		}
		WritePWM2(ProcToPmpPwm(PMP.APROC));
	}
	else
	{
		if(M._MODULUJ&&!RdPrt(S_ERR_IN)&&!M._RPM_CW&&RTS(_RTMPMP)>_TPMPBLK)			//obieg CO, po rozruchu po ustalonym czasie rozpocznij modulacje pompa
		{
			if(rgl.fz)	//dla fazy 1 narost lub spadek obrotow w rgl.c (IncMaxCO())
			{
			 	PMP.APROC=ModPmp2(PMP.APROC);
			}
		}
		else
		{
			if(!M._RPM_CW) PMP.APROC=PMP.MGPROC;	//pelny wybieg dla CO
			else PMP.APROC=PMP.MGPROC2;				//pelny wybieg dla CW
		}
		WritePWM2(ProcToPmpPwm(PMP.APROC));
	}
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//Funkcja wywolywana w proc regulacji [rgl.c (IncMaxCO())] przy naroscie mocy (FAZA1)
void PmpToUP(void)
{
	static unsigned char tm;

	if(DtKNF.rdpm&&!M._LIN_KSK&&!M._LIN_PWD&&!M._LIN_BUF)
	{
		tm++;
		if(tm>30)
		{
			tm=0;
			if(!M._RPM_CW)
			{
				if(PMP.APROC+1<PMP.MGPROC) PMP.APROC=PMP.APROC+1;
			}
		}
	}
}
//Funkcja wywolywana w proc regulacji [rgl.c (IncMaxCO())] przy naroscie mocy (FAZA1)
void PmpToDW(void)
{
	static unsigned char tm;

	if(DtKNF.rdpm&&!M._LIN_KSK&&!M._LIN_PWD&&!M._LIN_BUF)
	{
		tm++;
		if(tm>0)
		{
			tm=0;
			if(!M._RPM_CW)
			{
				if(PMP.APROC-1>50) PMP.APROC=PMP.APROC-1;
			}
		}
	}
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//Modulacja praca pompy 
void RglPmp(void)
{
	if(DtKNF.rdpm&&!M._LIN_KSK&&!M._LIN_PWD&&!M._LIN_BUF)					//zdecyduj ze wzgledu na rodzaj pompy
	{
        if(!M._LOW_PMP)
        {        
            if(DtKNF.robg)				//obieg zamkniety?
            {
                if(!DtKNF.teco)
                {
                    RglPmp1();				//moduluj tradycyjnym algorytmem (delta = DtKNF.dtpm)
                }
                else
                {
                    RglPmp2();				//moduluj algorytmem ECO
                }
            }
            else
            {
                PMP.MGPROC=DtKNF.mgpm;						//gorny prog procentowy predkosci
                if(DtKNF.tpkt==_UNICO)
                {
                    PMP.MGPROC2=DtKNF.mgpm2;	//99 gorny prog procentowy predkosci dla obiegu CW
                }
                else
                {
                    PMP.MGPROC2=99;
                }
                if(!M._RPM_CW) PMP.APROC=PMP.MGPROC;	//pelny wybieg dla CO
                else PMP.APROC=PMP.MGPROC2;
                WritePWM2(ProcToPmpPwm(PMP.APROC));
            }
        }
        else
        {
            PMP.APROC=_PLOW_PRC;
            WritePWM2(ProcToPmpPwm(PMP.APROC));            
        }
	}
	else	//minimalizuj straty energii/zapobiegnij postojowi w przypadku pomylki konfiguracyjnej
	{
        if(!M._LOW_PMP)
        {
            if(!M._PRACA)
            {
                StartRTS(_RTMPMP);
                if(_POMPA_GetValue())
                {
                    PMP.APROC=99;			//wybieg z predkoscia bezpieczenstwa
                }
                else
                {
                    PMP.APROC=0;			//minimalizuj straty energii
                }
            }
            else
            {
                PMP.APROC=99;			//wybieg z predkoscia bezpieczenstwa
            }
            WritePWM2(ProcToPmpPwm(PMP.APROC));	
        }
        else
        {
            PMP.APROC=_PLOW_PRC;
            WritePWM2(ProcToPmpPwm(PMP.APROC));
        }
	}
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
#endif