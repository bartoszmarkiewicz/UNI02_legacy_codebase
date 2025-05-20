/*rgl.c*/
/*---------------------------------------------------------------------------------*/
#include <pic18.h>
#include "global.h"				//parametry globalne
#include "rgl.h"				//parametry lokalne
#include "main.tp.h"
#include "pomiar.h"
#include "pwm.h"
#include "konfig.h"
#include "pompa.tp.h"

extern DtPMP PMP;
extern DataCO DtCO;
extern DataCW DtCW;
extern DtPWM PWM;
extern DtKONF DtKNF;
extern tpMAIN M;
DtRgl rgl; 
extern unsigned char vvvv, vvv, vup, vdw,mmm;
extern unsigned char x,kll,gll,hll,bll;
unsigned char tmr,tmr2;
unsigned char pwm_center,pwm_kwr;
signed char popMAXCW, popMAXCO, popMIN;
signed char popSMAX,popSMIN,popSMOD;
//************************************************************
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
				if(RTdS(_RTMOD2)>1)
				{
					StartRTdS(_RTMOD2);
					NewPop_MIN(); 	//poprawka do wartosci granicznej minimalnej
					NewPop_MAX_CO(); //poprawka do wartosci granicznej maksymalnej
				}
				New_MIN();			//wartosc graniczna modulacji
				New_MAX_CO();		//wartosc graniczna modulacji
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
				if(RTdS(_RTMOD2)>1)
				{
					StartRTdS(_RTMOD2);
					NewPop_MIN(); 	//poprawka do wartosci granicznej minimalnej
					NewPop_MAX_CW(); //poprawka do wartosci granicznej maksymalnej
				}
				New_MIN();			//wartosc graniczna modulacji
				New_MAX_CW();		//wartosc graniczna modulacji
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
			break;
		}
		default:		//domyslny monotermiczny z hydroblokiem
		{
			if(M._pob_CO)			//obieg CO
			{
//-------------
				if(RTdS(_RTMOD2)>1)
				{
					StartRTdS(_RTMOD2);
					NewPop_MIN(); 	//poprawka do wartosci granicznej minimalnej
					NewPop_MAX_CO(); //poprawka do wartosci granicznej maksymalnej
				}
				New_MIN();			//wartosc graniczna modulacji
				New_MAX_CO();		//wartosc graniczna modulacji
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
				if(RTdS(_RTMOD2)>1)
				{
					StartRTdS(_RTMOD2);
					NewPop_MIN(); 	//poprawka do wartosci granicznej minimalnej
					NewPop_MAX_CW(); //poprawka do wartosci granicznej maksymalnej
				}
				New_MIN();			//wartosc graniczna modulacji
				New_MAX_CW();		//wartosc graniczna modulacji
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
//************************************************************
//------------------------------------------------------------
//------------------------------------------------------------

//------------------------------------------------------------
//------------------------------------------------------------
//------dowolny wymiennik - obieg CO
//------------------------------------------------------------
//------------------------------------------------------------								
//-------------------------------------------------------------
//parametry startowe dla CO - kociol 1 funkcyjny.
/*
void StartMCO1F(void)
{
	unsigned char k;
	
	if(rgl.btco&&!rgl.btcw) return; //praca w obiegu CO?	
	if(rgl.btco&&rgl.btcw)			//zmiana obiegu CO na CW?
	{
		rgl.btcw=0;
		if(!Tnizsza(0))
		{
			k=10;
			if(rgl.srp-k<PWM.MINPWM) rgl.srp=PWM.MINPWM;
			else rgl.srp-=k;
		}
	}
	else //rozpoczecie pracy w obiegu CO lub CW
	{
		StartRTdS(_RTMOD);
		popMIN=0;
		popMAXCO=0;
		DtCO.dDCO=0;
		DtCO.indCO=0;
		rgl.fz=0;
		rgl.med_up=0;
		rgl.fast_up=0;
		//CO - dobor wartosci pierwszego pobudzenia (moment startu modulacji)
		if(PWM.MAXPWM>PWM.FIRSTPB)
		{
			rgl.xmod=PWM.FIRSTPB;
		}
		else
		{
			rgl.xmod=PWM.MAXPWM;
		}
		rgl.srp=0;
		rgl.ytmp=0;
		rgl.nmod=0;		//wyjdz z trybu nmod
		rgl.btcw=0;
		rgl.btco=1;
		rgl.del1s=0;
		rgl.deln1=0;
		rgl.inv_mod=0;
	}
	pwm_center=PWM.MINPWM+(PWM.MAXPWM-PWM.MINPWM)/2;
	pwm_kwr=PWM.MINPWM+(PWM.MAXPWM-PWM.MINPWM)/4;
}*/

unsigned char FirstXMOD(void);
void StartCO2(void)//StartMCO1F(void)
{
	if(rgl.btco||rgl.btcw) return; //rozpoczecie pracy modulatora?
	StartRTdS(_RTMOD);
	popMIN=0;
	popMAXCO=0;
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
//Oblicz wartosc startow¹ ogranicznika mocy
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
/*
//pierwsze przyblizenie
void ModZgCO(void)
{
	signed long m;
	unsigned char p;

	//faza2
	if(rgl.fz)						//aktualnie faza 2?
	{
		if(Tnizsza(10))				//przygotowanie do przejscia do fazy 1
		{
			rgl.xmod=PWM.BufPWM;
			rgl.srp=0;
			rgl.ytmp=0;	
		}
		else return;
	}
	//faza1
	if((!Tnizsza(5)&&Tmaleje(0))||(!Tnizsza(2)&&Tstoi())) 		//warunek przejscia do fazy 2
	{
		rgl.fz=1;
		rgl.srp=PWM.BufPWM;
		rgl.xmod=PWM.BufPWM;
		{
			if(rgl.xmod>PWM.MAXPWM)
			{
				rgl.srp=PWM.MAXPWM;
				rgl.xmod=PWM.MAXPWM;
			}
		}
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
*/
//pierwsze przyblizenie
void ModZgCO(void)
{
	signed long m;
	unsigned char p;
	if(rgl.fz)						//aktualnie faza 2?
	{
		if(Tnizsza(3))				//2 przygotowanie do przejscia do fazy 1
		{
			rgl.xmod=PWM.BufPWM;
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
		rgl.srp=PWM.BufPWM;
		rgl.xmod=PWM.BufPWM;
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





							
//-------------------------------------------------------------

/*

//drugie przyblizenie (tylko dla fazy 1)
void ModDkCO(void)
{
	signed long m;	
	signed char ada;
	unsigned char k,p,pop;
	
	if(!rgl.fz)
	{
		SetStab(DtCO.AC_CO,0,1);			//reset stabilizatora
		kll=pop;
		return;								//wyjscie jesli aktualnie faza 1
	}
	ada=DtCO.AC_CO-DtCO.DAD_CO;
	//-------------------------------
	k=5;
	if(Twyzsza(0)) k=6;			//6		//(Tco>Tnst)?
	//-------------------------------
	pop=SetStab(DtCO.AC_CO,k,0);				//poprawka ze stabilizatora
	kll=pop;
	//-------------------------
	m=rgl.srp+(k-pop)*(signed long)ada;
	if(m>rgl.xmod) p=rgl.xmod;
	else
	if(m<PWM.MINPWM) p=PWM.MINPWM;
	else p=m;
	rgl.tmp_pwm=p;
}
*/


//-------------------------------------------------------------
//drugie przyblizenie (tylko dla fazy 1)
void ModDkCO(void)
{
	signed long m;	
	signed char ada;
	unsigned char k,p;
	
	if(!rgl.fz) return;
	ada=DtCO.AC_CO-DtCO.DAD_CO;
	k=2;//5
	if(Trowna()) return;				//Tco!=Tnst?
	if(Tnizsza(0))						//Tco<Tnst?
	{
		k=3;//6
	}
	m=rgl.srp+k*ada;
	if(m>rgl.xmod) p=rgl.xmod;
	else
	if(m<PWM.MINPWM) p=PWM.MINPWM;
	else p=(unsigned char)m;
	rgl.tmp_pwm=p;
}
							
//-------------------------------------------------------------						
//trzecie przyblizenie
//nowa wartosc punktu rownowagi srp.
//wykonywane co krok czasowy
/*
void ModPruCO(void)
{
	static unsigned char l;
	unsigned char tmp,k;
	signed char ada;
	if(!rgl.fz)	//aktualnie faza 1?
	{
		if(Tmaleje(0)||rgl.ytmp>2)						//(Tco maleje)lub(Tco=CONST(>=3sek))?
		{
			rgl.ytmp=0;
			if(PWM.BufPWM>rgl.xmod) return;
			tmp=rgl.xmod-PWM.BufPWM;
			//k=6;
			k=15;
			if(k>tmp) k=tmp;
			{
				if(rgl.srp+k>PWM.MAXPWM) rgl.srp=PWM.MAXPWM;
				else rgl.srp+=k;
			}
			//if(rgl.srp+k>PWM.MAXPWM) rgl.srp=PWM.MAXPWM;
			//else rgl.srp+=k;
		}
		return;
	}
	else //aktualnie faza 2
	{
		if(!Trowna())	 								//(Tco<>Tnst) ?
		{		
			if(DtCO.DAD_CO>DtCO.AC_CO)					//Tco>Tnst ?
			{
				if(Tmaleje(0)) return; 					//(Tco maleje)?
				if(l>=2)
				{
					k=1;
					l=0;
				}
				else
				{
					k=0;
 					l++;
				}
				if(rgl.srp-k<PWM.MINPWM) rgl.srp=PWM.MINPWM;
				else rgl.srp-=k;
				if(rgl.srp>PWM.MAXPWM) rgl.srp=PWM.MAXPWM;
				if(rgl.xmod<rgl.srp) rgl.xmod=rgl.srp;
				return;					
			}
			else										//Tco<Tnst
			{
				if(Trosnie(0)) return; 					//(Tco rosnie)?
				if(l>=1)
				{
					k=1;
					l=0;
				}
				else
				{
					k=0;
 					l++;
				}	
				{
					if(rgl.srp+k>PWM.MAXPWM) rgl.srp=PWM.MAXPWM;
					else rgl.srp+=k;
				}
				if(rgl.xmod<rgl.srp) rgl.xmod=rgl.srp;
			}
		}else l=0;
	}
}
*/
//-------------------------------------------------------------
//trzecie przyblizenie
//nowa wartosc punktu rownowagi srp.
//wykonywane co krok czasowy
void ModPruCO(void)
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
				if(PWM.BufPWM<rgl.xmod) tmp=rgl.xmod-PWM.BufPWM;
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
			if(rgl.srp>PWM.MAXPWM) rgl.srp=PWM.MAXPWM;
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
			if(rgl.srp+k>PWM.MAXPWM) rgl.srp=PWM.MAXPWM;
			else rgl.srp+=k;
			if(rgl.xmod<rgl.srp) rgl.xmod=rgl.srp;
			return;
		}
	}
}
//-------------------------------------------------------------								
//inkrementacja maksymalnej wartosci mocy na palniku
//wykonywane co krok czasowy
/*void IncMaxCO(void)
{
	unsigned char k;
	
	if(Trosnie(1)) return;								//(Tco rosnie szybciej niz 1K/sek)?	
	//kontrolowany narost mocy
	k=6;
	if(!rgl.fz)											//faza 1 ?
	{
		if(rgl.xmod+k>PWM.MAXPWM) rgl.xmod=PWM.MAXPWM;
		else rgl.xmod+=k;
		if(rgl.xmod>PWM.MAXPWM) rgl.xmod=PWM.MAXPWM;		
	}
	else												//faza 2
	{
		{
			if(rgl.xmod+k>PWM.MAXPWM) rgl.xmod=PWM.MAXPWM;
			else rgl.xmod+=k;
		}
	}
}*/
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

//-------------------------------------------------------------								
//reakcja superwizyjna
/*
void ModPrzCO(void)
{
	unsigned char k;
	
	if(!rgl.nmod)					//nie jest w trybie nmod?
	{
		if(Twyzsza(3))				//Tco-Tnst>3 ?
		{
			rgl.nmod=1;				//wejdz w tryb nmod
			k=20;
			if(rgl.srp-k<PWM.MINPWM) rgl.srp=PWM.MINPWM;
			else rgl.srp-=k;
		}
	}
	else							//jest w trybie nmod?
	{
		if(!Twyzsza(2))				
		 			rgl.nmod=0;		//wyjdz z trybu nmod
	}
}*/
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
						if(rgl.srp-15<PWM.MINPWM) rgl.srp=PWM.MINPWM;//10
						else rgl.srp-=15;
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

/*
	if(!rgl.nmod)					//nie jest w trybie nmod?
	{
		if(!Tmaleje(0))			//(Tco nie maleje ?)
		{
			if(Twyzsza(4))				//Tco-Tnst>3 ?
			{
				rgl.nmod=1;				//wejdz w tryb nmod
				k=20;
				if(rgl.srp-k<PWM.MINPWM) rgl.srp=PWM.MINPWM;
				else rgl.srp-=k;
			}
		}
	}
	else							//jest w trybie nmod?
	{
		if(!Twyzsza(2)||Tmaleje(0))	{zn=0;rgl.nmod=0;}		//wyjdz z trybu nmod
	}
*/
	timer1++;
}

//-------------------------------------------------------------							
//Proporcjonalne OPEN-T	
void PropOPT(void)
{
	if(rgl.nmod)				//aktywny tryb nmod?
	{
		WritePWM(PWM.MINPWM);
	}
	else
	{
		if(rgl.tmp_pwm<PWM.MINPWM) rgl.tmp_pwm=PWM.MINPWM;
		WritePWM(rgl.tmp_pwm);
	}
}
/*
//************************************************************
//------------------------------------------------------------
//------------------------------------------------------------
//------wymiennik bitermiczny (miniterm) - obieg CW
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//parametry startowe dla CO
//-------------------------------------------------------------
//parametry startowe dla CW
void StartMCO(void)
{
	if(rgl.btco||rgl.btcw) return; //rozpoczecie pracy modulatora?
	StartRTdS(_RTMOD);
	DtCO.dDCO=0;
	DtCW.dDCW=0;
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
	rgl.xmod=FirstPCO();
	tmr=1;
	tmr2=1;
	pwm_center=PWM.MDMOD+(PWM.MGMOD-PWM.MDMOD)/2;
	pwm_kwr=PWM.MDMOD+(PWM.MGMOD-PWM.MDMOD)/4;
}

unsigned char FirstPCO(void)
{
	signed long m;
	unsigned char maxmod,p,p2,tmp;

	//-------
	//maksymalna dozwolona wartosc pierwszego pobudzenia
	tmp=(PWM.FIRSTPB-PWM.MDMOD)/4;
	if(PWM.MMAXCO>(PWM.FIRSTPB+tmp))
	{
		maxmod=PWM.FIRSTPB+tmp;
	}
	else
	{
		maxmod=PWM.MMAXCO;
	}
	

	//-------
	//ze wzgledu na temperature
	if(DtCW.CO>=TMIN_MOD&&DtCW.CO<=TMAX_MOD)						//temperatura w przedziale roboczym?
	{
		m=(TMAX_MOD-DtCW.CO)*10;
		m=m/(TMAX_MOD-TMIN_MOD);
		m=m*(maxmod-PWM.MDMOD);
		if(m<0) m=0;
		m=m/10+PWM.MDMOD;
		if(m>0xff)	p=maxmod;										//przepelnienie 
		else p=m;
	}
	else															//temperatura poza przedzialem? 
	{
		if(DtCW.CO<TMIN_MOD) p=maxmod;
		if(DtCW.CO>TMAX_MOD) p=PWM.MDMOD;
	}
	//-------
	//ze wzgledu na nastawe
	m=(DtCO.PCO-F1_MIN_PCO)*10;
	m=m/(F1_MAX_PCO-F1_MIN_PCO);
	m=m*(maxmod-PWM.MDMOD);
	if(m<0) m=0;
	m=m/10+PWM.MDMOD;
	if(m>0xff)	p2=maxmod;											//przepelnienie 
	else p2=m;
	//-------
	//wybrana zostaje nizsza wartosc z p i p2
	if(p>p2) p=p2;
	//-------
	//ze wzgledu na relacje miêdzy temperatura i nastawa
	if(DtCO.DCO>=DMIN_MOD&&DtCO.DCO<=DMAX_MOD)						//roznica w przedziale roboczym?
	{
		m=DtCO.DCO*10;
		m=m/(DMAX_MOD-DMIN_MOD);
		m=m*(maxmod-PWM.MDMOD);
		if(m<0) m=0;
		m=m/10+PWM.MDMOD;
		if(m>0xff)	p2=maxmod;										//przepelnienie 
		else p2=m;		
	}
	else															//roznica poza przedzialem?
	{
		if(DtCO.DCO>DMAX_MOD) p2=maxmod;
		if(DtCO.DCO<DMIN_MOD) p2=PWM.MDMOD;
	}
	//-------
	//wybrana zostaje nizsza wartosc z p i p2
	if(p>p2) p=p2;
	//-------
	return p;														//wartosc pierwszego pobudzenia
}
//-------------------------------------------------------------
//zeruj parametry modulatora po zmianie obiegu
void ClrMod(void)
{
	rgl.fz=0;
	rgl.med_up=0;
	rgl.fast_up=0;
	rgl.xmod=PWM.BufPWM;
	rgl.srp=0;
	rgl.ytmp=0;
	rgl.nmod=0;
	rgl.del1s=0;
	rgl.inv_mod=0;
}
//-------------------------------------------------------------
//zmien obieg na CW
void ModToCW(void)
{
	rgl.btcw=1;
	if(rgl.btco)
	{
		rgl.btco=0;
		ClrMod();
	}
}
//-------------------------------------------------------------
//zmien obieg na CO
void ModToCO(void)
{
	rgl.btco=1;
	if(rgl.btcw)
	{
		rgl.btcw=0;
		ClrMod();
	}
}
//-------------------------------------------------------------
//oblicza czas przez ktory temp=const
void IncConstCOCW(void)
{
	if(Tstoi()) rgl.ytmp++;
	else rgl.ytmp=0;
}
//-------------------------------------------------------------
//wprowadzenie danych wejsciowych-CO
void ReadDataCO(void)
{
	rgl.x_t=DtCO.DCO;
	rgl.x_dt=DtCO.dDCO;
}
//-------------------------------------------------------------
//wprowadzenie danych wejsciowych-CW
void ReadDataCW(void)
{
	rgl.x_t=DtCW.DCW;
	rgl.x_f=DtCO.DCO;
	rgl.x_dt=DtCW.dDCW;
	rgl.x_df=DtCO.dDCO;
}
//**************************************************************************8888
//8888888888888888888888888888888888888888888888888888888888
//8888888888888888888888888888888888888
//8888888888888888888888888888
//8888888888888888888888
//888888888888888
//-------------------------------------------------------------
//pierwsze przyblizenie
void ModZgCO(void)
{
	signed long m;
	unsigned char p;
	if(rgl.fz)						//aktualnie faza 2?
	{
		if(Tnizsza(3))				//2 przygotowanie do przejscia do fazy 1
		{
			rgl.xmod=PWM.BufPWM;
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
		rgl.srp=PWM.BufPWM;
		rgl.xmod=PWM.BufPWM;
		if(rgl.xmod>PWM.MGMOD)
		{
			rgl.srp=PWM.MGMOD;
			rgl.xmod=PWM.MGMOD;
		}
		return;
	}
	if(rgl.xmod>=PWM.MDMOD)	m=(rgl.xmod-PWM.MDMOD)*10;
	else m=0;
	m=m/(DtCO.MAD_CO-DtCO.DAD_CO);	//DAD_CO-MAD_CO
	m=m*(DtCO.AC_CO-DtCO.DAD_CO);	//DAD_CO-AC_CO	
	if(m<0) m=0;
	m=m/10+PWM.MDMOD;
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
	else p=m;
	rgl.fz=0;						//przejscie do fazy1 przy aktywnej fazie2
	rgl.tmp_pwm=p;
}
//-------------------------------------------------------------
//drugie przyblizenie (tylko dla fazy 1)
void ModDkCO(void)
{
	signed long m;	
	signed char ada;
	unsigned char k,p;
	
	if(!rgl.fz)	return;					//wyjscie jesli aktualnie faza 1
	ada=DtCO.AC_CO-DtCO.DAD_CO;
	k=1;
	if(Trowna()) return;				//Tcw!=Tnst?
	if(Tnizsza(0))						//Tcw<Tnst?
	{
		k=2;
	}
	m=rgl.srp+k*ada;
	if(m>rgl.xmod) p=rgl.xmod;
	else
	if(m<PWM.MDMOD) p=PWM.MDMOD;
	else p=m;
	rgl.tmp_pwm=p;
}
//-------------------------------------------------------------
//trzecie przyblizenie
//nowa wartosc punktu rownowagi srp.
//wykonywane co krok czasowy
void ModPruCO(void)
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
		if(DtCO._newPCO) s=0;							//zmieniono nastawe?
		if(Tnizsza(0))									//Tcw<Tnst?
		{
			if(!Tnizsza(10))							//Tnst-Tcw<=10 ?
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
				if(PWM.BufPWM>rgl.xmod) return;
				tmp=rgl.xmod-PWM.BufPWM;
				if(k>tmp) k=tmp;
				if(PWM.MMAXCO>PWM.MGMOD)
				{
					if(rgl.srp+k>PWM.MGMOD) rgl.srp=PWM.MGMOD;
					else rgl.srp+=k;
				}
				else
				{
					if(rgl.srp+k>PWM.MMAXCO) rgl.srp=PWM.MMAXCO;
					else rgl.srp+=k;
				}
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
		if(DtCO.DAD_CO>DtCO.AC_CO)	//Tcw>Tnst ?
		{
			k=0;
			if(Trosnie(0)&&m>=2)
			{	
				p=0;
				o=0;
				l=0;
				m=0;
				k=2;	//3		
			}
			else
			if(Twyzsza(1))				//gdy jest powyzej zakresu nastawa+1
			{
				if(Tstoi())
				{
					if(l>=2)
					{
						k=1;
						if(Twyzsza(3)) k=2;
						l=0;
					}
					else
					{
						k=0;
 						l++;
					}
				}
			} else l=0;
			if(PWM.MMAXCO>PWM.MGMOD)
			{
				if(rgl.srp>PWM.MGMOD) rgl.srp=PWM.MGMOD;
			}
			else
			{
				if(rgl.srp>PWM.MMAXCO) rgl.srp=PWM.MMAXCO;
			}
			if(rgl.srp-k<PWM.MDMOD) rgl.srp=PWM.MDMOD;
			else rgl.srp-=k;

			//if(rgl.srp>PWM.MMAXCO) rgl.srp=PWM.MMAXCO;
			if(rgl.xmod<rgl.srp) rgl.xmod=rgl.srp;
			return;
		}
		else										//Tcw<Tnst
		{
			k=0;
			if(Tmaleje(0)&&m>=2)
			{	
				p=0;
				o=0;
				l=0;
				m=0;
				k=5;		//1			
				//if(Tnizsza(0)) k=3;				//3
			}
			else
			if(Tnizsza(1))
			{		
				if(Tstoi())
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
			}else l=0;
			if(PWM.MMAXCO>PWM.MGMOD)
			{
				if(rgl.srp+k>PWM.MGMOD) rgl.srp=PWM.MGMOD;
				else rgl.srp+=k;
			}
			else
			{
				if(rgl.srp+k>PWM.MMAXCO) rgl.srp=PWM.MMAXCO;
				else rgl.srp+=k;
			}
			if(rgl.xmod<rgl.srp) rgl.xmod=rgl.srp;
			return;
		}
	}
}

//------------------------------------------------------------
//inkrementacja maksymalnej wartosci mocy na palniku
//wykonywane co krok czasowy
void IncMaxCO(void)
{
	unsigned char k;
	
	//decyzja o wejsciu w tryb INVMOD
	//kontrola maksimum mocy dla fazy 2
	if(rgl.fz)											//faza 2 ?
	{
		if(PWM.MMAXCO>PWM.MGMOD)
		{
			if(rgl.xmod>PWM.MGMOD) rgl.xmod=PWM.MGMOD;
		}
		else
		{
			if(rgl.xmod>PWM.MMAXCO) rgl.xmod=PWM.MMAXCO;
		}
		return;
	}
	//jednorazowa modyfikacja wartosci srp ze wzgledu na szybkosc 
	//narostu temperatury
	if(Trosnie(2)&&!rgl.med_up)		//(Tcw rosnie szybciej niz 3K/sek)?	
	{
		rgl.med_up=1;
		k=5;						//10
		if(PWM.MMAXCO>PWM.MGMOD)
		{
			if(rgl.srp+k>PWM.MGMOD) rgl.srp=PWM.MGMOD;
			else rgl.srp+=k;
		}
		else
		{
			if(rgl.srp+k>PWM.MMAXCO) rgl.srp=PWM.MMAXCO;
			else rgl.srp+=k;
		}
	}
	if(Trosnie(3)&&!rgl.fast_up)	//(Tcw rosnie szybciej niz 4K/sek)?	
	{
		rgl.fast_up=1;
		k=8;						//12
		if(PWM.MMAXCO>PWM.MGMOD)
		{
			if(rgl.srp+k>PWM.MGMOD) rgl.srp=PWM.MGMOD;
			else rgl.srp+=k;
		}
		else
		{
			if(rgl.srp+k>PWM.MMAXCO) rgl.srp=PWM.MMAXCO;
			else rgl.srp+=k;
		}
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
	if(rgl.xmod+k>_MG2_MOD) rgl.xmod=_MG2_MOD;
	else rgl.xmod+=k;
	if(rgl.xmod>PWM.MMAXCO) rgl.xmod=PWM.MMAXCO;
}
//------------------------------------------------------------
//reakcja superwizyjna
void ModPrzCO(void)
{
	unsigned char p,m,maxmod,mdmod;
	signed int r,q;
	static unsigned char timer1;
 
	//stabilizacja predkosci dochodzenia do temp. zadanej.
	if(Tnizsza(0))//(!Tnizsza(20)&&Tnizsza(0)) //!Tnizsza(15)&&Tnizsza(0)
	{
		if(PWM.MMAXCO<PWM.MGMOD) maxmod=PWM.MMAXCO;						//wprowadzono ograniczenie mocy?
		else maxmod=PWM.MGMOD;


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
				r=p-PWM.MDMOD;
				if((p-(r/3))>PWM.MDMOD) p-=r/3;//4
				else p=PWM.MDMOD;
				rgl.xmod=p;
			}
			else								
			if((DtCO.ddDCO>4)&&(tmr>1))											//temperatura rosnie szybciej ni¿ 3 stop. na sek.?							
			{
				tmr=0;
				r=p-PWM.MDMOD;
				if((p-(r/4))>PWM.MDMOD) p-=r/4;//4
				else p=PWM.MDMOD;
				rgl.xmod=p;
			}
			else
			if((DtCO.ddDCO>3)&&(tmr>0)) //tmr2									//temperatura rosnie szybciej ni¿ 3 stop. na sek.?							
			{
				tmr=0;
				r=p-PWM.MDMOD;
				if((p-(r/14))>PWM.MDMOD) p-=r/14;//16
				else p=PWM.MDMOD;
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
				r=p-PWM.MDMOD;
				if((p-(r/2))>PWM.MDMOD) p-=r/2;//2
				else p=PWM.MDMOD;
				rgl.xmod=p;
			}
			else								
			if((DtCO.ddDCO>4)&&(tmr>1))											//temperatura rosnie szybciej ni¿ 3 stop. na sek.?							
			{
				tmr=0;
				r=p-PWM.MDMOD;
				if((p-(r/3))>PWM.MDMOD) p-=r/3;//3
				else p=PWM.MDMOD;
				rgl.xmod=p;
			}
			else
			if((DtCO.ddDCO>3)&&(tmr>0)) //tmr2									//temperatura rosnie szybciej ni¿ 3 stop. na sek.?							
			{
				tmr=0;
				r=p-PWM.MDMOD;
				if((p-(r/4))>PWM.MDMOD) p-=r/4;//8
				else p=PWM.MDMOD;
				rgl.xmod=p;
			}
		}
	}
	if(!rgl.nmod)					//nie jest w trybie nmod?
	{
		if(rgl.fz)					//faza 2 ?
		{
			if(!Tmaleje(0))			//(Tcw nie maleje ?)
			{
				if(Twyzsza(4))		//Tcw-Tnst>4 ?
				{
					if(rgl.tmp_pwm>=pwm_kwr)
					{
						if(rgl.srp-5<PWM.MDMOD) rgl.srp=PWM.MDMOD;//10
						else rgl.srp-=5;
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
		if(Tmaleje(0))				//(Tcw maleje ?)
		 			rgl.nmod=0;		//wyjdz z trybu nmod
	}
	timer1++;
}



//------------------------------------------------------------
//Zabezpieczenie od przegrzania w obwodzie CO			
void PropCO(void)
{
	if(!rgl.nmod)					//nie jest w trybie nmod?
	{
		if(!DtCO._err_CO)			//awaria czujnika temperatury CO?
		{
			if(rgl.x_f==3)						//=86C ? 
			{
				if(rgl.tmp_pwm>PWM.MGMOD)		
				rgl.tmp_pwm=PWM.MGMOD;			//=MGMOD
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
		}
		WritePWM(rgl.tmp_pwm);
	}
	else WritePWM(PWM.MDMOD);//PWM.BufPWM=PWM.MDMOD;			//wymus minimum plomienia
}
*/
/*
//************************************************************
//------------------------------------------------------------
//------------------------------------------------------------
//------wymiennik bitermiczny (maksiterm) - obieg CW
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//parametry startowe dla CO
void StartCO4(void)
{
	unsigned char k;
	
	if(rgl.btco||rgl.btcw) return; //praca w dowolnym obiegu CO?	
	StartRTdS(_RTMOD);
	DtCO.dDCO=0;
	DtCO.indCO=0;
	DtCW.dDCW=0;
	DtCW.indCW=0;
	rgl.fz=0;
	rgl.med_up=0;
	rgl.fast_up=0;
	//CO - dobor wartosci pierwszego pobudzenia (moment startu modulacji)
	if(PWM.MMAXCO>PWM.FIRSTPB)
	{
		rgl.xmod=PWM.FIRSTPB;
	}
	else
	{
		rgl.xmod=PWM.MMAXCO;
	}
	rgl.srp=0;
	rgl.ytmp=0;
	rgl.nmod=0;		//wyjdz z trybu nmod
	rgl.del1s=0;
	rgl.inv_mod=0;
}
//-------------------------------------------------------------
//parametry startowe dla CW
void StartMCW4(void)
{
	if(rgl.btco||rgl.btcw) return; //rozpoczecie pracy modulatora?
	StartRTdS(_RTMOD);
	DtCO.dDCO=0;
	DtCW.dDCW=0;
	DtCO.indCO=0;
	DtCW.indCW=0;
	rgl.fz=0;
	rgl.med_up=0;
	rgl.fast_up=0;
	//FirstPCW4();
	rgl.srp=0;
	rgl.ytmp=0;
	rgl.nmod=0;
	rgl.del1s=0;
	rgl.inv_mod=0;
	rgl.xmod=PWM.MDMOD;
}
//-------------------------------------------------------------
//pierwsze pobudzenie dla CW
void FirstPCW4(void)
{
	if(PWM.MMAXCW>PWM.MGMOD)
	{
		if(DtCW.DCW>0) rgl.xmod=PWM.MGMOD;
		else rgl.xmod=PWM.FIRSTPB;
	}
	else
	{
		if(DtCW.DCW>0) rgl.xmod=PWM.MMAXCW;
		else 
		{
			if(PWM.MMAXCW>PWM.FIRSTPB) rgl.xmod=PWM.FIRSTPB;
			else rgl.xmod=PWM.MMAXCW;
		}
	}
}
//-------------------------------------------------------------
//zeruj parametry modulatora po zmianie obiegu
void ClrMod4(void)
{
	rgl.fz=0;
	rgl.med_up=0;
	rgl.fast_up=0;
	rgl.xmod=PWM.BufPWM;
	rgl.srp=0;
	rgl.ytmp=0;
	rgl.nmod=0;
	rgl.del1s=0;
	rgl.inv_mod=0;
}
//-------------------------------------------------------------
//zmien obieg na CW
void ModToCW4(void)
{
	rgl.btcw=1;
	if(rgl.btco)
	{
		rgl.btco=0;
		ClrMod4();
	}
}
//-------------------------------------------------------------
//zmien obieg na CO
void ModToCO4(void)
{
	rgl.btco=1;
	if(rgl.btcw)
	{
		rgl.btcw=0;
		ClrMod4();
	}
}
//-------------------------------------------------------------
//oblicza czas przez ktory temp=const
void IncConstCOCW4(void)
{
	if(Tstoi()) rgl.ytmp++;
	else rgl.ytmp=0;
}
//-------------------------------------------------------------
//wprowadzenie danych wejsciowych-CO
void ReadDataCO4(void)
{
	rgl.x_t=DtCO.DCO;
	rgl.x_dt=DtCO.dDCO;
}
//-------------------------------------------------------------
//wprowadzenie danych wejsciowych-CW
void ReadDataCW4(void)
{
	rgl.x_t=DtCW.DCW;
	rgl.x_f=DtCO.DCO;
	rgl.x_dt=DtCW.dDCW;
	rgl.x_df=DtCO.dDCO;
}
//-------------------------------------------------------------
//pierwsze przyblizenie
void ModZgCW4(void)
{
	signed long m;
	unsigned char p;
	static unsigned char pop;

	if(Tnizsza(10))	
	{
		pop=0;						//20wartosc poprawki na przesterowanie
		//FirstPCW4();
	}
	if(rgl.fz)						//aktualnie faza 2?
	{
		if(Tnizsza(2))				//Tnizsza(2)przygotowanie do przejscia do fazy 1
		{
			rgl.xmod=PWM.BufPWM;
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
		rgl.srp=PWM.BufPWM+pop;
		rgl.xmod=PWM.BufPWM+pop;
		pop=0;
		if(PWM.MMAXCW>PWM.MGMOD)
		{
			if(rgl.xmod>PWM.MGMOD)
			{
				rgl.srp=PWM.MGMOD;
				rgl.xmod=PWM.MGMOD;
			}
		}
		else
		{
			if(rgl.xmod>PWM.MMAXCW)
			{
				rgl.srp=PWM.MMAXCW;
				rgl.xmod=PWM.MMAXCW;
			}
		}
		return;
	}
	if(rgl.xmod>=PWM.MDMOD)	m=(rgl.xmod-PWM.MDMOD)*10;
	else m=0;
	m=m/(DtCW.MAD_CW-DtCW.DAD_CW);	//DAD_CW-MAD_CW
	m=m*(DtCW.AC_CW-DtCW.DAD_CW);	//DAD_CW-AC_CW	
	if(m<0) m=0;
	m=m/10+PWM.MDMOD;
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
	else p=m;
	rgl.fz=0;						//przejscie do fazy1 przy aktywnej fazie2
	rgl.tmp_pwm=p;
}
//-------------------------------------------------------------
//drugie przyblizenie (tylko dla fazy 2)
void ModDkCW4(void)
{
	signed long m;	
	signed char ada;
	unsigned char k,p;
	
	if(!rgl.fz)	return;					//wyjscie jesli aktualnie faza 1
	ada=DtCW.AC_CW-DtCW.DAD_CW;
	k=2;									//9
	if(ada<0) k=1;							//8
	m=rgl.srp+k*ada;
	if(m>rgl.xmod) p=rgl.xmod;
	else
	if(m<PWM.MDMOD) p=PWM.MDMOD;
	else p=m;
	rgl.tmp_pwm=p;
}
//-------------------------------------------------------------
//trzecie przyblizenie
//nowa wartosc punktu rownowagi srp.
//wykonywane co krok czasowy
void ModPruCW4(void)
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
				if(Tstoi()&&s>=1)
				{
					s=0;
					k=2;
				}
				if(Trosnie(1)&&s>=1) 	//szybki narost temperatury
				{
					s=0;
					k=10;
					if(Trosnie(2)) k=8;	//b.szybki narost temperatury?
				}
				else
				rgl.ytmp=0;
				if(PWM.BufPWM>rgl.xmod) return;
				tmp=rgl.xmod-PWM.BufPWM;
				if(k>tmp) k=tmp;
				if(PWM.MMAXCW>PWM.MGMOD)
				{
					if(rgl.srp+k>PWM.MGMOD) rgl.srp=PWM.MGMOD;
					else rgl.srp+=k;
				}
				else
				{
					if(rgl.srp+k>PWM.MMAXCW) rgl.srp=PWM.MMAXCW;
					else rgl.srp+=k;
				}
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
		if(DtCW.DAD_CW>DtCW.AC_CW)	//Tcw>Tnst ?
		{
			k=0;
			if(Trosnie(0)&&m>=2)
			{	
				p=0;
				o=0;
				l=0;
				m=0;
				k=2;	//3		
			}
			else
			if(Twyzsza(1))				//gdy jest powyzej zakresu nastawa+1
			{
				if(Tstoi())
				{
					if(l>=2)
					{
						k=1;
						if(Twyzsza(3)) k=2;
						l=0;
					}
					else
					{
						k=0;
 						l++;
					}
				}
			} else l=0;
			if(rgl.srp-k<PWM.MDMOD) rgl.srp=PWM.MDMOD;
			else rgl.srp-=k;
			if(rgl.srp>PWM.MMAXCW) rgl.srp=PWM.MMAXCW;
			if(rgl.xmod<rgl.srp) rgl.xmod=rgl.srp;
			return;
		}
		else										//Tcw<Tnst
		{
			k=0;
			if(Tmaleje(0)&&m>=2)
			{	
				p=0;
				o=0;
				l=0;
				m=0;
				k=1;		//2			
				//if(Tnizsza(0)) k=3;				//3
			}
			else
			if(Tnizsza(1))
			{		
				if(Tstoi())
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
			}else l=0;
			if(PWM.MMAXCW>PWM.MGMOD)
			{
				if(rgl.srp+k>PWM.MGMOD) rgl.srp=PWM.MGMOD;
				else rgl.srp+=k;
			}
			else
			{
				if(rgl.srp+k>PWM.MMAXCW) rgl.srp=PWM.MMAXCW;
				else rgl.srp+=k;
			}
			if(rgl.xmod<rgl.srp) rgl.xmod=rgl.srp;
			return;
		}
	}
}
//------------------------------------------------------------
//inkrementacja maksymalnej wartosci mocy na palniku
//wykonywane co krok czasowy
void IncMaxCW4(void)
{
	unsigned char k;
	
	//kontrola maksimum mocy dla fazy 2
	if(rgl.fz)										//faza 2 ?
	{
		if(PWM.MMAXCW>PWM.MGMOD)
		{
			if(rgl.xmod>PWM.MGMOD) rgl.xmod=PWM.MGMOD;
		}
		else
		{
			if(rgl.xmod>PWM.MMAXCW) rgl.xmod=PWM.MMAXCW;
		}
		return;
	}

	//kontrolowany narost mocy dla fazy 1
	if(Trosnie(1)) return;				//(Tcw lub Tco rosnie szybciej niz 1K/sek)?
	k=5;
	if(rgl.xmod+k>_MG2_MOD) rgl.xmod=_MG2_MOD;
	else rgl.xmod+=k;
	if(rgl.xmod>PWM.MMAXCW) rgl.xmod=PWM.MMAXCW;
}
//------------------------------------------------------------
//reakcja superwizyjna
void ModPrzCW4(void)
{
	static unsigned char trb=0;
	unsigned int p;
	
	p=(rgl.tmp_pwm-PWM.MDMOD)*100;
	p=p/(PWM.MGMOD-PWM.MDMOD);

	if(!rgl.nmod)									//nie jest w trybie nmod?
	{
		trb=0;
		if(p<=35)
		{
			if(!Tmaleje(0)&&Twyzsza(3))					//(Tcw nie maleje ?)
			{
					trb=1;
					rgl.nmod=1;							//wejdz w tryb nmod
			}
		}
		else
		{
			if(!Tmaleje(0))			//(Tcw nie maleje ?)
			{
				if(Twyzsza(5))		//Tcw-Tnst>4 ?
				{
					if(rgl.srp-3<PWM.MDMOD) rgl.srp=PWM.MDMOD;
					else rgl.srp-=3;
					trb=2;
					rgl.nmod=1;		//wejdz w tryb nmod
					return;
				}
			}
			else return;
		}
	}
	else											//jest w trybie nmod?
	{
		switch(trb)
		{
			case 1:
			{
				if(!Twyzsza(3))
				{
					if(Tnizsza(3)||Tstoi()||Tmaleje(0))
					{
						trb=0;
 						rgl.nmod=0;					//jezeli Tcw maleje wyjdz z trybu nmod
					}
				}
				break;
			}
			case 2:
			{
				if(Tmaleje(0)||!Twyzsza(4))				//(Tcw maleje ?)
				{
					trb=0;
					rgl.nmod=0;		//wyjdz z trybu nmod
					return;
				}
				break;
			}
		}
	}
}
//------------------------------------------------------------
//Zabezpieczenie od przegrzania w obwodzie CO			
void PropCO4(void)
{
	if(!rgl.nmod)					//nie jest w trybie nmod?
	{
		if(!DtCO._err_CO)			//awaria czujnika temperatury CO?
		{
			if(rgl.x_f==3)						//=86C ? 
			{
				if(rgl.tmp_pwm>PWM.MGMOD)		
				rgl.tmp_pwm=PWM.MGMOD;			//=MGMOD
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
		}
		WritePWM(rgl.tmp_pwm);
	}
	else WritePWM(PWM.MDMOD);				//wymus minimum plomienia
}
*/
//************************************************************
//------------------------------------------------------------
//------------------------------------------------------------
//------wymiennik plytowy - obieg CW
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//parametry startowe dla CO
/*void StartCO2(void)
{
	unsigned char k;
	
	if(rgl.btco||rgl.btcw) return; //praca w dowolnym obiegu CO?	
	StartRTdS(_RTMOD);
	popMIN=0;
	popMAXCO=0;
	DtCO.dDCO=0;
	DtCO.indCO=0;
	DtCW.dDCW=0;
	DtCW.indCW=0;
	rgl.fz=0;
	rgl.med_up=0;
	rgl.fast_up=0;
	//CO - dobor wartosci pierwszego pobudzenia (moment startu modulacji)
	if(PWM.MAXPWM>PWM.FIRSTPB)
	{
		rgl.xmod=PWM.FIRSTPB;
	}
	else
	{
		rgl.xmod=PWM.MAXPWM;
	}
	rgl.srp=0;
	rgl.ytmp=0;
	rgl.nmod=0;		//wyjdz z trybu nmod
	rgl.del1s=0;
	rgl.deln1=0;
	rgl.inv_mod=0;
}*/

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
//-------------------------------------------------------------
//parametry startowe dla CW
unsigned char FirstXMOD2(void);
void StartMCW2(void)
{
	if(rgl.btco||rgl.btcw) return; //rozpoczecie pracy modulatora?
	StartRTdS(_RTMOD);
	popMIN=0;
	popMAXCW=0;
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
    unsigned char fp,proc;
    PWM.FIRSTPB=PWM.MAXPWM-4*((PWM.MAXPWM-PWM.MINPWM)/10);
    if(DtCW.DCW>0) return PWM.MAXPWM;
    else 
    {
        if(PWM.MAXPWM>PWM.FIRSTPB) return PWM.FIRSTPB;
        else return PWM.MAXPWM;
    }
    /*PWM.FIRSTPB=PWM.MAXPWM-4*((PWM.MAXPWM-PWM.MINPWM)/10);
    if(DtCW.DCW>0) fp=PWM.MAXPWM;
    else 
    {
        if(PWM.MAXPWM>PWM.FIRSTPB) fp=PWM.FIRSTPB;
        else fp=PWM.MAXPWM;
    }   
    proc=1;
    return (unsigned char)(PWM.MINPWM+((fp-PWM.MINPWM)*proc)/100);*/
}

/*unsigned char FirstXMOD2(unsigned char str)
{
	signed long m;
	unsigned char maxmod,p,p2,tmp;

	//-------
	//maksymalna dozwolona wartosc pierwszego pobudzenia
	PWM.FIRSTPB=PWM.MAXPWM-4*((PWM.MAXPWM-PWM.MINPWM)/10);
	if(DtCW.DCW>0) maxmod=PWM.MAXPWM;
	else 
	{
		if(PWM.MAXPWM>PWM.FIRSTPB) maxmod=PWM.FIRSTPB;
		else maxmod=PWM.MAXPWM;
	}
	p=maxmod;
	//-------
	//ze wzgledu na temperature
	if(DtCW.CW>=TCWMIN_MD1&&DtCW.CW<=TCWMAX_MD1)						//temperatura w przedziale roboczym?
	{
		m=(signed long)(TCWMAX_MD1-DtCW.CW)*10;
		m=m/(signed long)(TCWMAX_MD1-TCWMIN_MD1);
		m=m*(signed long)(maxmod-PWM.MINPWM);
		if(m<0) m=0;
		m=m/10+PWM.MINPWM;
		if(m>0xff)	p=maxmod;										//przepelnienie 
		else p2=(unsigned char)m;
	}
	else															//temperatura poza przedzialem? 
	{
		if(DtCW.CW<TCWMIN_MD1) p2=maxmod;
		if(DtCW.CW>TCWMAX_MD1) p2=PWM.MINPWM;
	}
	if(p>p2) p=p2;
	//-------
	//-------
	//ze wzgledu na relacje miêdzy temperatura i nastawa
	if(DtCW.DCW>=DCWMIN_MD1&&DtCW.DCW<=DCWMAX_MD1)						//roznica w przedziale roboczym?
	{
		m=(signed long)DtCW.DCW*10;
		m=m/(signed long)(DCWMAX_MD1-DCWMIN_MD1);
		m=m*(signed long)(maxmod-PWM.MINPWM);
		if(m<0) m=0;
		m=m/10+PWM.MINPWM;
		if(m>0xff)	p2=maxmod;										//przepelnienie 
		else p2=(unsigned char)m;		
	}
	else															//roznica poza przedzialem?
	{
		if(DtCW.DCW>DCWMAX_MD1) p2=maxmod;
		if(DtCW.DCW<DCWMIN_MD1) p2=PWM.MINPWM;
	}
	//-------
	//wybrana zostaje nizsza wartosc z p i p2
	if(p>p2) p=p2;
	//-------
	return p;
														//wartosc pierwszego pobudzenia
}

*/
//-------------------------------------------------------------
//zeruj parametry modulatora po zmianie obiegu
void ClrMod2(void)
{
	rgl.fz=0;
	rgl.med_up=0;
	rgl.fast_up=0;
	if(rgl.btco) rgl.xmod=PWM.BufPWM;
	else rgl.xmod=FirstXMOD2();
	rgl.srp=0;
	rgl.ytmp=0;
	rgl.nmod=0;
	rgl.del1s=0;
	rgl.inv_mod=0;
}
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
			rgl.xmod=PWM.BufPWM;
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
		rgl.srp=PWM.BufPWM+pop;
		rgl.xmod=PWM.BufPWM+pop;
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
//-------------------------------------------------------------
//-------------------------------------------------------------

//-------------------------------------------------------------
//-------------------------------------------------------------

//-------------------------------------------------------------
//FAZA2
//drugie przyblizenie (tylko dla fazy 2)
void ModDkCW2(void)
{
	signed long m;	
	signed char ada;
	unsigned char k,p,pop;
	
	if(!rgl.fz)
	{
		//SetStab(DtCW.AC_CW,0,1);			//reset stabilizatora
		return;								//wyjscie jesli aktualnie faza 1
	}
	ada=DtCW.AC_CW-DtCW.DAD_CW;
	//-------------------------------
    if(!DtKNF.rfan)                         //FIME?
    {
        k=3;									//6*********************************
        if(ada<0) k=2;							//5*********************************
    }
    else                                    //NG40
    {
        k=6;									//6*********************************
        if(ada<0) k=5;							//5*********************************        
    }
	//-------------------------------
	//pop=SetStab(DtCW.AC_CW,k,0);				//poprawka ze stabilizatora
	//kll=pop;
	pop=0;
	//-------------------------
	m=rgl.srp+(k-pop)*(signed long)ada;
	if(m>rgl.xmod) p=rgl.xmod;
	else
	if(m<PWM.MINPWM) p=PWM.MINPWM;
	else p=(unsigned char)m;
	rgl.tmp_pwm=p;
}
//-------------------------------------------------------------
//FAZA1+FAZA2
//trzecie przyblizenie
//nowa wartosc punktu rownowagi srp.
//wykonywane co krok czasowy
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
                if(!DtKNF.rfan)                         //FIME?
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

				if(PWM.BufPWM<rgl.xmod) tmp=rgl.xmod-PWM.BufPWM;
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
			if(rgl.srp-k<PWM.MINPWM) rgl.srp=PWM.MINPWM;
			else rgl.srp-=k;
			if(rgl.srp>PWM.MAXPWM) rgl.srp=PWM.MAXPWM;
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
			if(rgl.srp+k>PWM.MAXPWM) rgl.srp=PWM.MAXPWM;
			else rgl.srp+=k;
			if(rgl.xmod<rgl.srp) rgl.xmod=rgl.srp;
			return;
		}
	}
}
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
	if(rgl.xmod+k>PWM.MAXPWM) rgl.xmod=PWM.MAXPWM;
	else rgl.xmod+=k;
	if(rgl.xmod>PWM.MAXPWM) rgl.xmod=PWM.MAXPWM;
}
//-------------------------------------------------------------
//reakcja superwizyjna
void ModPrzCW2(void)
{
	unsigned char k;
	if(!rgl.nmod)									//nie jest w trybie nmod?
	{
		if(!Tmaleje(0)&&Twyzsza(3))					//(Tcw nie maleje ?)
		{
                if(!DtKNF.rfan)                         //FIME?
                {
                    k=15;
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
//------------------------------------------------------------
//Zabezpieczenie od przegrzania w obwodzie CO			
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
		WritePWM(rgl.tmp_pwm);
	}
	else 
    {
        if(rgl.tmp_pwm<PWM.MINPWM+5)
        {
            WritePWM(PWM.MINPWM);//PWM.BufPWM=PWM.MINPWM;			//wymus minimum plomienia
        }
        else
        {
            if(rgl.tmp_pwm>PWM.MINPWM+5)
            {
                WritePWM(PWM.MINPWM+5);//PWM.BufPWM=PWM.MINPWM;			//wymus minimum plomienia
            }
        }
    }
}
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//Procedury modulacji dla potrzeb regulacji - wartosci graniczne
//------------------------------------------------------------
const unsigned char kdw=_VNT_KDW;
const unsigned char kup=_VNT_KUP;

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
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//Procedury modulacji dla potrzeb konfiguracji i funkcji serwisowej
//------------------------------------------------------------
//Nowa minimalna wartosc graniczna
unsigned char CalkVNTtoPopPWM(unsigned char nst,unsigned char pop);
//Procedury modulacji dla potrzeb konfiguracji i funkcji serwisowej
//------------------------------------------------------------
//Nowa minimalna wartosc graniczna
unsigned char New_SMOD(unsigned char pv)
{
	unsigned char pw;

	//pw=CalkVNTtoPWM(pv);
    switch(DtKNF.rfan)  //rodzaj wentylatora
    {
        case 0: //FIME
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


/*unsigned char New_SMOD(unsigned char pv)
{
	unsigned char pw;

	pw=CalkVNTtoPWM(pv);
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
	pw=CalkVNTtoPWM(ppv);		//teoretyczna oczekiwana wartosc PWM dla pv
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
}*/
//-------------------------------------------------------------
//-------------------------------------------------------------
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
	//return mmm=New_SMOD(pv);	
	//return CalkVNTtoPWM(pv);
}
/*
//************************************************************
//------------------------------------------------------------
//------------------------------------------------------------
//------Minimax z zasobnikiem, Unico
//------------------------------------------------------------
//------------------------------------------------------------								
//-------------------------------------------------------------
//-------------------------------------------------------------
//zmien obieg na CW
void ModToCW3(void)
{
	rgl.btcw=1;
	if(rgl.btco)	//aktywny jest obieg CO?
	{
		rgl.btco=0;
		//wstepne obnizenie mocy palnika
		if(!Tnizsza(0))
		{
			if(rgl.srp-10<PWM.MDMOD) rgl.srp=PWM.MDMOD;
			else rgl.srp-=10;
		}
	}
}
//-------------------------------------------------------------
//zmien obieg na CO
void ModToCO3(void)
{
	rgl.btco=1;
	rgl.btcw=0;
}
//-------------------------------------------------------------
//parametry startowe - kociol z zasobnikiem.
void StartMCO3(void)
{
	if(rgl.btco||rgl.btcw) return; //praca w dowolnym obiegu CO?	
	StartRTdS(_RTMOD);
	DtCO.dDCO=0;
	DtCO.indCO=0;
	rgl.fz=0;
	rgl.med_up=0;
	rgl.fast_up=0;
	//CO - dobor wartosci pierwszego pobudzenia (moment startu modulacji)
	if(PWM.MAXPWM>PWM.FIRSTPB)
	{
		rgl.xmod=PWM.FIRSTPB;
	}
	else
	{
		rgl.xmod=PWM.MAXPWM;
	}
	rgl.srp=0;
	rgl.ytmp=0;
	rgl.nmod=0;						//wyjdz z trybu nmod
	rgl.btcw=0;
	rgl.btco=1;
	rgl.del1s=0;
	rgl.deln1=0;
	rgl.inv_mod=0;
}
*/
//************************************************************
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
