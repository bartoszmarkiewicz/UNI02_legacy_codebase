/*rgl.c*/
/*---------------------------------------------------------------------------------*/
#include <xc.h>
#include <stdbool.h>
#include <stdint.h>

#include "../../proj.tp.h"

#ifndef _TYP_PRJ
#error NIEOKRESLONY IDENTYFIKATOR PROJEKTU
#endif

#if _TYP_PRJ==4
#advisory _TYP_PRJ==4
#include "global.h"
#include "main.h"
#include "global.ex.h"

#include "rgl.h"				//parametry lokalne
#include "konfig.h"
#include "pwm.h"
#include "pomiar.h"

extern DataCO DtCO;
extern DataCW DtCW;
extern DtPWM PWM;
extern DtKONF DtKNF;
extern tpMAIN M;
extern unsigned char x,y,t,ty;
extern volatile unsigned char prz;
DtRgl rgl;
unsigned char tmr,tmr2,tmr3,tmr4,rosnie,stoi;
unsigned char pwm_center,pwm_kwr;
static unsigned char tmee;
//------------------------------------------------------------------
    
//------------------------------------------------------------------    
//Procedura modulacji
void Moduluj(void)
{
	unsigned int wpw;
	unsigned char wpp;
	
	StartMCW();			//parametry startowe dla CW
	ReadDataCW();		//wprowadzenie danych wejsciowych dla CW
	ModToCW();			//zmien obieg na CW
	ModZgCW();			//pierwsze przyblizenie
	ModDkCW();			//drugie przyblizenie
	if(RTdS(_RTMOD)>_TOKM)
	{
		StartRTdS(_RTMOD);
		if(tmr<0xff) tmr++;
		if(tmr2<0xff) tmr2++;
		if(tmr3<0xff) tmr3++;
		if(tmr4<0xff) tmr4++;
		if(tmee<0xff) tmee++;
		IncConstCW();	//oblicza czas przez ktory temp CW = const	
		IncMaxCW();		//inkrementacja maksymalnej wartosci mocy na palniku
		ModPruCW();		//trzecie przyblizenie
	}
	ModPrzCW();			//reakcja superwizyjna
	SetPWM_CW();			//zabezpieczenie od przegrzania w obwodzie CO				StartMCW2();		//parametry startowe dla CW
}
//************************************************************
//------------------------------------------------------------
//------------------------------------------------------------
//------wymiennik bitermiczny (miniterm) - obieg CW
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//parametry startowe dla CW
//org
void StartMCW(void)
{
	if(rgl.btcw) return; //rozpoczecie pracy modulatora?
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
	rgl.xmod=FirstPCW();
	tmr=1;
	tmr2=1;
	tmr3=1;
	tmr4=1;
	tmee=0;
	pwm_center=PWM.MDMOD+(PWM.MGMOD-PWM.MDMOD)/2;
	pwm_kwr=PWM.MDMOD+(PWM.MGMOD-PWM.MDMOD)/4;
}
//-------------------------------------------------------------
//pierwsze pobudzenie dla CW
//org
unsigned char FirstPCW(void)
{
	signed long m;
	unsigned char maxmod,p,p2,tmp;

	//-------
	//maksymalna dozwolona wartosc pierwszego pobudzenia
	tmp=(PWM.FIRSTPB-PWM.MDMOD)/4;
	if(PWM.MMAXCW>(PWM.FIRSTPB+tmp))
	{
		maxmod=PWM.FIRSTPB+tmp;
	}
	else
	{
		maxmod=PWM.MMAXCW;
	}
	//-------
	//ze wzgledu na temperature
	if(DtCW.CW>=TMIN_MOD&&DtCW.CW<=TMAX_MOD)						//temperatura w przedziale roboczym?
	{
		m=(TMAX_MOD-DtCW.CW)*10;
		m=m/(TMAX_MOD-TMIN_MOD);
		m=m*(maxmod-PWM.MDMOD);
		if(m<0) m=0;
		m=m/10+PWM.MDMOD;
		if(m>0xff)	p=maxmod;										//przepelnienie 
		else p=m;
	}
	else															//temperatura poza przedzialem? 
	{
		if(DtCW.CW<TMIN_MOD) p=maxmod;
		if(DtCW.CW>TMAX_MOD) p=PWM.MDMOD;
	}
	//-------
	//ze wzgledu na nastawe
	m=(DtCW.PCW-F1_MIN_PCW)*10;
	m=m/(F1_MAX_PCW-F1_MIN_PCW);
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
	if(DtCW.DCW>=DMIN_MOD&&DtCW.DCW<=DMAX_MOD)						//roznica w przedziale roboczym?
	{
		m=DtCW.DCW*10;
		m=m/(DMAX_MOD-DMIN_MOD);
		m=m*(maxmod-PWM.MDMOD);
		if(m<0) m=0;
		m=m/10+PWM.MDMOD;
		if(m>0xff)	p2=maxmod;										//przepelnienie 
		else p2=m;		
	}
	else															//roznica poza przedzialem?
	{
		if(DtCW.DCW>DMAX_MOD) p2=maxmod;
		if(DtCW.DCW<DMIN_MOD) p2=PWM.MDMOD;
	}
	//-------
	//wybrana zostaje nizsza wartosc z p i p2
	if(p>p2) p=p2;
	//-------
	return p;														//wartosc pierwszego pobudzenia
}
//-------------------------------------------------------------
//zeruj parametry modulatora po zmianie obiegu
/*
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
 */
//-------------------------------------------------------------
//zmien obieg na CW
void ModToCW(void)
{
	rgl.btcw=1;
}
//-------------------------------------------------------------
//oblicza czas przez ktory temp=const
void IncConstCW(void)
{
	if(Tstoi()) rgl.ytmp++;
	else rgl.ytmp=0;
}
//-------------------------------------------------------------
//wprowadzenie danych wejsciowych-CW
void ReadDataCW(void)
{
	rgl.x_t=DtCW.DCW;
	//rgl.x_f=DtCO.DCO;
	rgl.x_dt=DtCW.dDCW;
	//rgl.x_df=DtCO.dDCO;
}
//-------------------------------------------------------------
//pierwsze przyblizenie
void ModZgCW(void)
{
	signed long m;
	unsigned char p;
	if(rgl.fz)						//aktualnie faza 2?
	{
		if(Tnizsza(5))				//3przygotowanie do przejscia do fazy 1
		{
			rgl.xmod=PWM.BufPWM;
			rgl.srp=0;
			rgl.ytmp=0;	
			rgl.nmod=0;
			rgl.del1s=0;
		}
		else return;
	}
	//---------------------------------------------------------------
	//zmiana fazy
	//if(Twyzsza(0)&&Tstoi()) 			//warunek przejscia do fazy 2
	if(!rgl.nmod&&(Trowna()||Twyzsza(0)))//&&Tstoi()) 			//warunek przejscia do fazy 2
	{
		rgl.fz=1;
		rgl.fast_up=0;
		rgl.med_up=0;
		rgl.srp=PWM.BufPWM;//+10;
		rgl.xmod=PWM.BufPWM;//+10;
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
void ModDkCW(void)
{
	signed long m;	
	signed char ada;
	unsigned char p,i;
	float k;
	if(!rgl.fz)	return;					//wyjscie jesli aktualnie faza 1
	ada=DtCW.AC_CW-DtCW.DAD_CW;
	if(Trowna()) return;				//Tcw!=Tnst?
	k=1.0;
	if(Twyzsza(0)||Tnizsza(0))
	{
		for(i=1;i<6;i++)
		{
			if(Twyzsza(i)||Tnizsza(i)) k=k+0.25;
			else break;
		}
	}
	m=(signed long)(rgl.srp+k*ada);
    /*if(rgl.srp==PWM.MDMOD)
    {
                        if(prz<99) prz++;
                    else prz=0;
    }*/
	if(m>rgl.xmod) p=rgl.xmod;
	else
	if(m<PWM.MDMOD) p=PWM.MDMOD;
	else p=m;
    /*if(p==PWM.MDMOD)
    {
                        if(prz<99) prz++;
                    else prz=0;
    }*/
	rgl.tmp_pwm=p;
}
//-------------------------------------------------------------
//trzecie przyblizenie
//nowa wartosc punktu rownowagi srp.
//wykonywane co krok czasowy
void ModPruCW(void)
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
				{
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
				}
				if(Trosnie(1)&&s>=1) 	//szybki narost temperatury
				{
					s=0;
					{
						if(rgl.tmp_pwm>=pwm_kwr) k=35;				//35
						else k=25;
					}
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
			l=0;
			if(Tmaleje(0)&&Twyzsza(1)&&m>=3)
			{	
				p=0;
				o=0;
				l=1;
				m=0;
				k=1;
			}
			else
			/*if(!Tmaleje(0)&&Twyzsza(5)&&m>=0)				//gdy jest powyzej zakresu nastawa+1
			{
				p=0;
				o=0;
				l=0;
				m=0;
				k=2;
			}
            else*/
			if(!Tmaleje(0)&&Twyzsza(4)&&m>=2)				//gdy jest powyzej zakresu nastawa+1
			{
				p=0;
				o=0;
				l=0;
				m=0;
				k=5;
			}
			else
			if(!Tmaleje(0)&&Twyzsza(2)&&m>=3)				//gdy jest powyzej zakresu nastawa+1
			{
				p=0;
				o=0;
				l=0;
				m=0;
				k=1;
			}
			else
			if(!Tmaleje(0)&&Twyzsza(1)&&m>=4)				//gdy jest powyzej zakresu nastawa+1
			{
				p=0;
				o=0;
				l=0;
				m=0;
				k=1;
			}
			if(!l&&k)	//poprawka ujemna?
			{
				if(PWM.MMAXCW>PWM.MGMOD)
				{
					if(rgl.srp>PWM.MGMOD) rgl.srp=PWM.MGMOD;
				}
				else
				{
					if(rgl.srp>PWM.MMAXCW) rgl.srp=PWM.MMAXCW;
				}
				if(rgl.srp-k<PWM.MDMOD) 
                {
                    //prz=99;
                    rgl.srp=PWM.MDMOD;
                }
				else
                {
                    //if(prz<99) prz++;
                    //else prz=0;
                    rgl.srp=rgl.srp-k;
                }
			}
			else
			{
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
			}
			if(rgl.xmod<rgl.srp) rgl.xmod=rgl.srp;
			return;
		}
		else										//Tcw<Tnst
		{
			k=0;
			l=0;
			if(Trosnie(0)&&Tnizsza(1)&&m>=3)
			{	
				p=0;
				o=0;
				l=1;
				m=0;
				k=1;		//-1		
			}
			else
			if(!Trosnie(0)&&Tnizsza(1)&&m>=3)				//1
			{	
				p=0;
				o=0;
				l=0;
				m=0;
				k=2;		//2	
				if(Tnizsza(3)) k=5;				//3	
			}
			else
			if(!Trosnie(0)&&Tnizsza(0)&&m>=4)				//1
			{	
				p=0;
				o=0;
				l=0;
				m=0;
				k=2;		//2	
			}
			if(!l&&k)	//poprawka dodatnia?
			{
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
			}
			else
			{
				if(rgl.srp-k<PWM.MDMOD) rgl.srp=PWM.MDMOD;
				else rgl.srp=rgl.srp-k;
			}
			if(rgl.xmod<rgl.srp) rgl.xmod=rgl.srp;
			return;
		}
	}
}
//------------------------------------------------------------
//inkrementacja maksymalnej wartosci mocy na palniku
//wykonywane co krok czasowy
void IncMaxCW(void)
{
	unsigned char k;
	
	//decyzja o wejsciu w tryb INVMOD
	//kontrola maksimum mocy dla fazy 2
	if(rgl.fz)											//faza 2 ?
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

	//jednorazowa modyfikacja wartosci srp ze wzgledu na szybkosc 
	//narostu temperatury
	if(Trosnie(2)&&!rgl.med_up)		//(Tcw rosnie szybciej niz 3K/sek)?	
	{
		rgl.med_up=1;
		k=5;						//10
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
	}
	if(Trosnie(3)&&!rgl.fast_up)	//(Tcw rosnie szybciej niz 4K/sek)?	
	{
		rgl.fast_up=1;
		k=15;						//8
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
	}

	//kontrolowany narost mocy
	if(Trosnie(1)) return;
	k=5;
	if(rgl.xmod+k>_MG2_MOD) rgl.xmod=_MG2_MOD;
	else rgl.xmod+=k;
	if(rgl.xmod>PWM.MMAXCW) rgl.xmod=PWM.MMAXCW;
}
//------------------------------------------------------------
//reakcja superwizyjna
void ModPrzCW(void)
{
	unsigned char p,m,v,maxmod,kn,i;
	signed int r,q;
	static unsigned char t3,timer1;
 
	//stabilizacja predkosci dochodzenia do temp. zadanej.
	if(Tnizsza(0))
	{
		if(PWM.MMAXCW<PWM.MGMOD) maxmod=PWM.MMAXCW;						//wprowadzono ograniczenie mocy?
		else maxmod=PWM.MGMOD;
		if(rgl.tmp_pwm>=pwm_center)	
		{	
			if(rgl.xmod<=maxmod) p=rgl.xmod;
			else
			{
	 			p=maxmod;	
				rgl.xmod=maxmod;
			}
			if((DtCW.ddDCW>6)&&(tmr>1))											//temperatura rosnie szybciej ni¿ 3 stop. na sek.?							
			{
				tmr=0;
				r=p-PWM.MDMOD;
				if((p-(r/3))>PWM.MDMOD) p-=r/3;//4
				else p=PWM.MDMOD;
				rgl.xmod=p;
			}
			else								
			if((DtCW.ddDCW>4)&&(tmr>1))											//temperatura rosnie szybciej ni¿ 3 stop. na sek.?							
			{
				tmr=0;
				r=p-PWM.MDMOD;
				if((p-(r/4))>PWM.MDMOD) p-=r/4;//4
				else p=PWM.MDMOD;
				rgl.xmod=p;
			}
			else
			if((DtCW.ddDCW>3)&&(tmr>0)) //tmr2									//temperatura rosnie szybciej ni¿ 3 stop. na sek.?							
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
			if((DtCW.ddDCW>6)&&(tmr>1))											//temperatura rosnie szybciej ni¿ 3 stop. na sek.?							
			{
				tmr=0;
				r=p-PWM.MDMOD;
				if((p-(r/2))>PWM.MDMOD) p-=r/2;//2
				else p=PWM.MDMOD;
				rgl.xmod=p;
			}
			else								
			if((DtCW.ddDCW>4)&&(tmr>1))											//temperatura rosnie szybciej ni¿ 3 stop. na sek.?							
			{
				tmr=0;
				r=p-PWM.MDMOD;
				if((p-(r/3))>PWM.MDMOD) p-=r/3;//3
				else p=PWM.MDMOD;
				rgl.xmod=p;
			}
			else
			if((DtCW.ddDCW>3)&&(tmr>0)) //tmr2									//temperatura rosnie szybciej ni¿ 3 stop. na sek.?							
			{
				tmr=0;
				r=p-PWM.MDMOD;
				if((p-(r/4))>PWM.MDMOD) p-=r/4;//8
				else p=PWM.MDMOD;
				rgl.xmod=p;
			}
		}
	}

	//UWAGA: histereza!!!
	if(!rgl.nmod)					//nie jest w trybie nmod?
	{
		//if(rgl.fz)					//faza 2 ?
		{
			if(!Tmaleje(0))			//(Tcw nie maleje ?)
			{
				if(Twyzsza(8))		//Tcw-Tnst>4 ?
				{
					//if(rgl.tmp_pwm>=pwm_kwr)
					{
						{
                            kn=30;  //
							if(rgl.srp-kn<PWM.MDMOD) rgl.srp=PWM.MDMOD;//15
							else rgl.srp=rgl.srp-kn;
						}
					}
//#if _RNMOD==1
                    /*if(!rgl.nmod)
                    {
                        StartDMod();
                    }*/
					rgl.nmod=1;		//wejdz w tryb nmod
//#endif
					return;
				}
				else return;
			}
			else return;
		}
		//else return;
	}
	else							//jest w trybie nmod?
	{
		if((Tmaleje(0)||Tstoi())&&!Twyzsza(7))				//(Tcw maleje ?)
		 			rgl.nmod=0;		//wyjdz z trybu nmod
	}
	timer1++;
}

//------------------------------------------------------------
//Wartosc procentowa wypelnienia dla stanu NMOD
volatile unsigned char nprc;
#define _STR_NPRC 25
void ResetNPrc(void)
{
    StartRTdS(_RTDM0);
    StartRTdS(_RTDM1);
}
unsigned char GetNPrc(void)
{  
    if(!DtKNF.rdgz)
    {
        if(RTdS(_RTDM0)<30)         //czasowa praca na mocy NMOD
        {
            nprc=_STR_NPRC;
            return nprc;
        }
        if(RTdS(_RTDM1)>2)          //powlny spadek do wartosci MDMOD
        {
            StartRTdS(_RTDM1);
            if(nprc>0) nprc--;      //stopniowe obnizenie wartosci procentowej
        }
    }
    else
    {
        if(RTdS(_RTDM0)<30)         //czasowa praca na mocy NMOD
        {
            nprc=_STR_NPRC;
            return nprc;
        }
        if(RTdS(_RTDM1)>4)          //powlny spadek do wartosci MDMOD
        {
            StartRTdS(_RTDM1);
            if(nprc>0) nprc--;      //stopniowe obnizenie wartosci procentowej
        }        
    }
    return nprc;
}
//Zabezpieczenie od przegrzania w obwodzie CO			
void SetPWM_CW(void)
{
    volatile unsigned char mc,prc;
    volatile unsigned char dt;
	if(!rgl.nmod)					//nie jest w trybie nmod?
	{
		//WriteOPWM(rgl.tmp_pwm);
        SetMod(rgl.tmp_pwm);
        ResetNPrc();
	}
#if _RNMOD==1
	else WriteOPWM(PWM.MDMOD);		//wymus minimum plomienia
#else
    else
    {
        //WriteOPWM(PWM.MSTART);		//wymus minimum plomienia

        //SetMod(PWM.MDMOD);      //1m
        prc=GetNPrc();
        SetMod(ProcToMOD2(prc));  
        dt=15;
        if(dt<DtKNF.minv) dt=DtKNF.minv;        
        mc=VNTtoPPWM(dt);
        ProcToVNT(mc);        
    }
#endif
}

void SetMod(unsigned char npwm)
{
    volatile unsigned char v,nv,m;
    volatile static unsigned char dpwm;

    m=MocMod();            //procentowy stan modulatora
    v=M.VVNT;              //wartosc aktualna predkosci wentylatora
    nv=MPPWMtoVNT(m);      //wartosc oczekiwana predkosci wentylatora
    if(!M._SVWB)
    {
        if(v>nv)
        {
            if((v-nv)>10)
            {
                rgl._mdm=1;      //uruchom spowalniacz
            }
            if((v-nv)<5)
            {
                rgl._mdm=0;      //uruchom spowalniacz 
            }            
        }
        if(v<nv)
        {
            if((nv-v)>10)
            {
                rgl._mdm=1;      //uruchom spowalniacz
            }
            if((nv-v)<5)
            {
                rgl._mdm=0;      //uruchom spowalniacz 
            }
        }        
    }
    else
    {
        rgl._mdm=0;        
    }
    //WriteOPWM(npwm);
    if(!rgl._mdm) 
    {
        dpwm=npwm;
        WriteOPWM(npwm);        
    }
    else
    {
        if(RTdS(_RTDM2)>0)
        {
            StartRTdS(_RTDM2);
            if(dpwm>npwm) dpwm--;
            if(dpwm<npwm) dpwm++;  
        }
        WriteOPWM(dpwm);
    }
}
//************************************************************
//------------------------------------------------------------
//------------------------------------------------------------
//------wymiennik bitermiczny (maksiterm) - obieg CW
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//-------------------------------------------------------------
//parametry startowe dla CW
//-------------------------------------------------------------
//pierwsze pobudzenie dla CW
/*
unsigned char FirstPCW4(void)
{
	signed long m;
	unsigned char maxmod,p,p2;
	//-------
	//maksymalna dozwolona wartosc pierwszego pobudzenia
	if(PWM.MMAXCW<PWM.FIRSTPB) maxmod=PWM.MMAXCW;					//wprowadzono ograniczenie mocy?
	else maxmod=PWM.FIRSTPB;
	//-------
	//ze wzgledu na temperature
	if(DtCW.CW>=TMIN_MOD&&DtCW.CW<=TMAX_MOD)						//temperatura w przedziale roboczym?
	{
		m=(TMAX_MOD-DtCW.CW)*10;
		m=m/(TMAX_MOD-TMIN_MOD);
		m=m*(maxmod-PWM.MDMOD);
		if(m<0) m=0;
		m=m/10+PWM.MDMOD;
		if(m>0xff)	p=maxmod;										//przepelnienie 
		else p=m;
	}
	else															//temperatura poza przedzialem? 
	{
		if(DtCW.CW<TMIN_MOD) p=maxmod;
		if(DtCW.CW>TMAX_MOD) p=PWM.MDMOD;
	}
	//-------
	//ze wzgledu na nastawe
	m=(DtCW.PCW-F1_MIN_PCW)*10;
	m=m/(F1_MAX_PCW-F1_MIN_PCW);
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
	if(DtCW.DCW>=DMIN_MOD&&DtCW.DCW<=DMAX_MOD)						//roznica w przedziale roboczym?
	{
		m=DtCW.DCW*10;
		m=m/(DMAX_MOD-DMIN_MOD);
		m=m*(maxmod-PWM.MDMOD);
		if(m<0) m=0;
		m=m/10+PWM.MDMOD;
		if(m>0xff)	p2=maxmod;										//przepelnienie 
		else p2=m;		
	}
	else															//roznica poza przedzialem?
	{
		if(DtCW.DCW>DMAX_MOD) p2=maxmod;
		if(DtCW.DCW<DMIN_MOD) p2=PWM.MDMOD;
	}
	//-------
	//wybrana zostaje nizsza wartosc z p i p2
	if(p>p2) p=p2;
	//-------
	return p;														//wartosc pierwszego pobudzenia
}
 */
/*
void StartMCW4(void)
{
	if(rgl.btcw) return; //rozpoczecie pracy modulatora?
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
	rgl.xmod=FirstPCW4();
	tmr=1;
	tmr2=1;
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
}
//-------------------------------------------------------------
//oblicza czas przez ktory temp=const
void IncConstCW4(void)
{
	if(Tstoi()) rgl.ytmp++;
	else rgl.ytmp=0;
}
//-------------------------------------------------------------
//wprowadzenie danych wejsciowych-CW
void ReadDataCW4(void)
{
	rgl.x_t=DtCW.DCW;
	rgl.x_f=DtCO.DCO;
	rgl.x_dt=DtCW.ddDCW;
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
		rosnie=0;
		stoi=0;
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
	k=3;									//2
	if(ada<0) k=2;							//1
	m=rgl.srp+k*ada;
	if(m>rgl.xmod) p=rgl.xmod;
	else
	if(m<PWM.MDMOD) p=PWM.MDMOD;
	else p=m;
	//x=k;
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
				if(Tstoi()&&s>2)
				{
					s=0;
					if(Tnizsza(3)) k=15;
					else k=10;
				}
				if(Trosnie(1)&&s>=1) 	//szybki narost temperatury
				{
					s=0;
					k=15;//10
					if(Trosnie(2)) k=8;	//b.szybki narost temperatury?
				}
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
				//x=k;
				return;
			}
			else return;
		}
		else return;
	}
	else //aktualnie faza 2
	{
		x=0;
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
				k=3;	//2		
			}
			else
			if(Twyzsza(1))				//gdy jest powyzej zakresu nastawa+1
			{
				if(Tstoi())
				{
					if(l>=2)
					{
						k=2;			//1
						if(Twyzsza(3)) k=3;//2
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
			else {x=k+10;rgl.srp-=k;}
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
				x=k;
			}
			else
			{
				if(rgl.srp+k>PWM.MMAXCW) rgl.srp=PWM.MMAXCW;
				else rgl.srp+=k;
				x=k;
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
	unsigned char k,w,d;
		
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
	if(!Trosnie(1))
	{
		k=5;
		if(rgl.xmod+k>_MG2_MOD) rgl.xmod=_MG2_MOD;
		else rgl.xmod+=k;
		if(rgl.xmod>PWM.MMAXCW) rgl.xmod=PWM.MMAXCW;
		x=k;
	}
}
//------------------------------------------------------------
//Reakcja superwizyjna
void ModPrzCW4(void)
{
	unsigned char p,m,maxmod,mdmod;
	signed int r,q;
	static unsigned char timer1;
 	
	//stabilizacja predkosci dochodzenia do temp. zadanej.
	if(Tnizsza(10))
	{
		if(PWM.MMAXCW<PWM.MGMOD) maxmod=PWM.MMAXCW;						//wprowadzono ograniczenie mocy?
		else maxmod=PWM.MGMOD;		
		if(rgl.xmod<=maxmod) p=rgl.xmod;
		else
		{
 			p=maxmod;	
			rgl.xmod=maxmod;
		}
		if((DtCW.ddDCW>6)&&(tmr>1))											//temperatura rosnie szybciej ni¿ 3 stop. na sek.?							
		{
			tmr=0;
			r=p-PWM.MDMOD;
			if((p-(r/4))>PWM.MDMOD) p-=r/4;
			else p=PWM.MDMOD;
			rgl.xmod=p;
		}
		else								
		if((DtCW.ddDCW>4)&&(tmr>1))											//temperatura rosnie szybciej ni¿ 3 stop. na sek.?							
		{
			tmr=0;
			r=p-PWM.MDMOD;
			if((p-(r/8))>PWM.MDMOD) p-=r/8;
			else p=PWM.MDMOD;
			rgl.xmod=p;
		}
		else
		if((DtCW.ddDCW>3)&&(tmr>0)) //tmr2											//temperatura rosnie szybciej ni¿ 3 stop. na sek.?							
		{
			tmr=0;
			r=p-PWM.MDMOD;
			if((p-(r/16))>PWM.MDMOD) p-=r/16;
			else p=PWM.MDMOD;
			rgl.xmod=p;
		}
		else
		if((DtCW.ddDCW<=0)&&(tmr2>1))
		{
			tmr2=0;
			r=p-PWM.MDMOD;
			if((p+(r/16))<maxmod) p+=r/16;
			else p=maxmod;
			rgl.xmod=p;
		}	
	}
	//t=tmr2;
	//szybka reakcja na przegrzanie
	if(!rgl.nmod)					//nie jest w trybie nmod?
	{
		if(rgl.fz)					//faza 2 ?
		{
			if(!Tmaleje(0))			//(Tcw nie maleje ?)
			{
				if(Twyzsza(9))		//Tcw-Tnst>9 ?
				{
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
		if(Tmaleje(0)&&!Twyzsza(9))	//(Tcw maleje ?)
		 			rgl.nmod=0;		//wyjdz z trybu nmod
	}
	timer1++;
}
//------------------------------------------------------------
//Zabezpieczenie od przegrzania w obwodzie CO			
//Zabezpieczenie od przegrzania w obwodzie CO			
void SetPWM_CW4(void)
{
	if(!rgl.nmod)					//nie jest w trybie nmod?
	{
		WriteOPWM(rgl.tmp_pwm);
	}
	else WriteOPWM(PWM.MDMOD);		//wymus minimum plomienia
}
//-------------------------------------------------------------
//pierwsze przyblizenie
void ModZgCW5(void)
{
	signed long m;
	unsigned char p;
	if(rgl.fz)						//aktualnie faza 2?
	{
		if(Tnizsza(2))				//przygotowanie do przejscia do fazy 1
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
//drugie przyblizenie (tylko dla fazy 1)
void ModDkCW5(void)
{
	signed long m;	
	signed char ada;
	unsigned char k,p;
	
	if(!rgl.fz)	return;					//wyjscie jesli aktualnie faza 1
	ada=DtCW.AC_CW-DtCW.DAD_CW;
	if(Trowna()) return;				//Tcw!=Tnst?
	if(Tnizsza(0))						//Tcw<Tnst?
	{
		if(!Trosnie(0))					//(Tcw=CONST) lub (Tcw maleje) ?
		{
			k=1;
			if(Tnizsza(1)) k=3;			//Tnst-Tcw>=2?
		}
	}
	else								//Tcw>Tnst?
	{
		if(!Tmaleje(0)) 				//(Tcw=CONST) lub (Tcw rosnie) ?
		{
			k=1;
			if(Twyzsza(1)) k=2;			//Tcw-Tnst>=2?
		}
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
void ModPruCW5(void)
{
	unsigned char tmp,k;
	signed char ada;
	if(!rgl.fz)								//aktualnie faza 1?
	{
		if(Tnizsza(0))						//Tcw<Tnst?
		{
			if(Tnizsza(0)&&!Tnizsza(10))	//Tnst-Tcw>=1 i Tnst-Tcw<=10 ?
			{
				if(rgl.del1s)
				{
					rgl.del1s=0;
					return;
				}
				if(Tstoi())						//Tcw=CONST?
				{
					rgl.del1s=1;
					if(PWM.BufPWM>rgl.xmod) return;
					tmp=rgl.xmod-PWM.BufPWM;
					//uwaga w tym miejscu wielkosc k uzalezniona byla od przeplywu
					
					if(Tnizsza(3))				//Tnst-Tcw>=4?
					{
						k=12;	
						if(rgl.fast_up) k=10;	//szybki narost temp.
					}
					else
					{
						k=3;
						if(Tnizsza(1)) k=6;		//Tnst-Tcw>=2
					}
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
		else return;
	}
	else //aktualnie faza 2
	{
		if(Tstoi()&&!Trowna())	 			//(Tcw=CONST) i (Tcw<>Tnst) ?
		{		
			if(DtCW.DAD_CW>DtCW.AC_CW)		//Tcw>Tnst ?
			{
				if(Twyzsza(1)) 				//(Tcw-Tnst>1)?
				{
					if(Tstoi()) 			//(Tcw=CONST)?
					{
						if(!Twyzsza(2))		//(Tcw-Tnst<=2)?
						{
							if(rgl.deln1)	//wykonaj nie czesciej niz...
							{
								rgl.deln1=0;
								return;
							}
							else rgl.deln1=1;
						}
					}
					if(Tmaleje(0))	return;	//(Tcw maleje)?
					//wykonywane dla (Tcw=CONST)lub (Tcw rosnie)
					k=1;
					if(!Twyzsza(1)) k=1;
					else 
					if(!Twyzsza(3)) 
					{
						k=2;
						if(!Tstoi())	k=5;			//Tcw!=CONST?
					}
					else
					{
						k=5;
						if(!Tstoi())	k=10;			//Tcw!=CONST?
					}
					if(rgl.srp-k<PWM.MDMOD) rgl.srp=PWM.MDMOD;
					else rgl.srp-=k;
					if(rgl.srp>PWM.MMAXCW) rgl.srp=PWM.MMAXCW;
					if(rgl.xmod<rgl.srp) rgl.xmod=rgl.srp;
					return;					
				}
				else return;
			}
			else										//Tcw<Tnst
			{
				if(Tnizsza(1)) 							//(Tnst-Tcw>1)?
				{
					if(!Trosnie(0))		//(Tcw=CONST) lub (Tcw maleje) ?
					{
						k=1;
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
					else return;
				}else return;
			}
		}
		else
		{
			if(Tmaleje(0))	//(Tcw maleje)?
			{
				k=1;
				if(Tnizsza(1)) k=3;						//Tnst-Tcw>=1 ?
				
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
			else
			if(Trosnie(0))	//(Tcw rosnie)?
			{
				k=1;
				if(Twyzsza(1)) k=3;						//Tcw-Tnst>=1 ?			
				
				if(rgl.srp-k<PWM.MDMOD) rgl.srp=PWM.MDMOD;
				else rgl.srp-=k;
				if(rgl.srp>PWM.MMAXCW) rgl.srp=PWM.MMAXCW;
				if(rgl.xmod<rgl.srp) rgl.xmod=rgl.srp;
				return;
			}			
		}
	}
}
//------------------------------------------------------------
//inkrementacja maksymalnej wartosci mocy na palniku
//wykonywane co krok czasowy
void IncMaxCW5(void)
{
	unsigned char k;
	
	//decyzja o wejsciu w tryb INVMOD
	if(Twyzsza(4)) rgl.inv_mod=1;						//Tcw-Tnst>=5 ?
	if(!Twyzsza(0)) rgl.inv_mod=0;						//Tnst-Tcw>=0 ?	
	//tryb zerowego wypelnienia
	if(rgl.inv_mod)										//jest w trybie nmod?
	{
		StartRTdS(_RTMOD);
		rgl.fz=0;
		rgl.srp=0;
		rgl.ytmp=0;
		rgl.del1s=0;
		DtCO.dDCO=0;
		DtCW.dDCW=0;
		DtCO.indCO=0;
		DtCW.indCW=0;	
		rgl.tmp_pwm=PWM.MDMOD;
		rgl.xmod=PWM.MDMOD;
		return;
	}
	//kontrola maksimum mocy dla fazy 2
	if(rgl.fz)											//faza 2 ?
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
	//jednorazowa modyfikacja wartosci srp ze wzgledu na szybkosc 
	//narostu temperatury
	if(Trosnie(2)&&!rgl.med_up)		//(Tcw rosnie szybciej niz 3K/sek)?	
	{
		rgl.med_up=1;
		k=5;						//10
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
	}
	if(Trosnie(3)&&!rgl.fast_up)	//(Tcw rosnie szybciej niz 4K/sek)?	
	{
		rgl.fast_up=1;
		k=8;						//12
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
	}
	//kontrolowany narost mocy
	if(Trosnie(1)) return;			//(Tcw rosnie szybciej niz 1K/sek)?
	k=5;
	if(rgl.xmod+k>_MG2_MOD) rgl.xmod=_MG2_MOD;
	else rgl.xmod+=k;
	if(rgl.xmod>PWM.MMAXCW) rgl.xmod=PWM.MMAXCW;
}
//------------------------------------------------------------
//reakcja superwizyjna
void ModPrzCW5(void)
{
	if(!rgl.nmod)					//nie jest w trybie nmod?
	{
		if(rgl.fz)					//faza 2 ?
		{
			if(!Tmaleje(0))			//(Tcw nie maleje ?)
			{
				if(Twyzsza(4))		//Tcw-Tnst>4 ?
				{
					rgl.nmod=1;		//wejdz w tryb nmod
					PWM.BufPWM=PWM.MDMOD;
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
}
//------------------------------------------------------------
//------------------------------------------------------------
//------wymiennik plytowy - obieg CW
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//parametry startowe dla CO
void StartCO2(void)
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
	//if(!PWM.tpGZ) rgl.xmod=I80_MOD;
	//else rgl.xmod=I80_PMOD;
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
	rgl.deln1=0;
	rgl.inv_mod=0;
}
//-------------------------------------------------------------
//parametry startowe dla CW
void StartMCW2(void)
{
	if(rgl.btco||rgl.btcw) return; //rozpoczecie pracy modulatora?
	StartRTdS(_RTMOD);
	DtCO.dDCO=0;
	DtCW.dDCW=0;
	DtCO.indCO=0;
	DtCW.indCW=0;
	rgl.fz=0;
	FirstPCW2();
	rgl.srp=0;
	rgl.ytmp=0;
	rgl.del1s=0;
}
//-------------------------------------------------------------	
//pierwsze pobudzenie dla CW
void FirstPCW2(void)
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
void ClrMod2(void)
{
	rgl.fz=0;
	rgl.xmod=PWM.BufPWM;
	rgl.srp=0;
	rgl.ytmp=0;
	rgl.del1s=0;
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
//pierwsze przyblizenie
void ModZgCW2(void)
{
	signed long m;
	unsigned char p;
	static unsigned char pop;

	if(Tnizsza(10))	
	{
		pop=20;						//wartosc poprawki na przesterowanie
		FirstPCW2();
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
void ModDkCW2(void)
{
	signed long m;	
	signed char ada;
	unsigned char k,p;
	
	if(!rgl.fz)	return;					//wyjscie jesli aktualnie faza 1
	ada=DtCW.AC_CW-DtCW.DAD_CW;
	k=9;									//9
	if(ada<0) k=8;							//8
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
				if((Trosnie(1)||TFrosnie(1))&&s>=1) 	//szybki narost temperatury
				{
					s=0;
					k=10;
					if(Trosnie(2)||TFrosnie(2)) k=8;	//b.szybki narost temperatury?
				}
				else
				if(TFmaleje(0)&&q>=1)
				{
					q=0;
					k=6;
				}else
				if(TFstoi())
				{
					if(r>=1)
					{
						k=6;
						r=0;
					}
					else
					{
						k=0;
 						r++;
					}
				}
				rgl.ytmp=0;
				if(PWM.BufPWM>rgl.xmod) return;
				tmp=rgl.xmod-PWM.BufPWM;
				if(k>tmp) k=tmp;
				//if(rgl.srp+k>PWM.MGMOD) rgl.srp=PWM.MGMOD;
				//else rgl.srp+=k;
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
		if(DtCW.DAD_CW>DtCW.AC_CW)					//Tcw>Tnst ?
		{
			k=0;
			if(TFmaleje(0)&&!Twyzsza(0)&&p>=2)		//(Tmaleje(0)||TFmaleje(0)
			{
				p=0;
				o=0;
				l=0;
				m=0;
				k=-1;								//1
			}
			else
			if(TFrosnie(0)&&o>=2)
			{	
				o=0;
				p=0;
				l=0;
				m=0;			
				k=2;					//3
				if(Trowna()) k=1;
			}
			else
			if(Trosnie(0)&&m>=2)
			{	
				p=0;
				o=0;
				l=0;
				m=0;
				k=3;	//2		
			}
			else
			if(Twyzsza(1))				//gdy jest powyzej zakresu nastawa+1
			{
				if(Tstoi()&&TFstoi())
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
			if(TFrosnie(0)&&o>=2) 						//(Trosnie(0)||TFrosnie(0))
			{
				o=0;
				p=0;
				l=0;
				m=0;
				k=-3;
				if(Trowna()) k=-2;
			}
			else
			if(TFmaleje(0)&&p>=2)
			{	
				p=0;
				o=0;
				l=0;
				m=0;			
				k=2;				//3
				if(Trowna()) k=1;
			}
			else
			if(Tmaleje(0)&&m>=2)
			{	
				p=0;
				o=0;
				l=0;
				m=0;
				k=2;			
				if(Tnizsza(0)) k=3;				//3
			}
			else
			if(Tnizsza(1))
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
//-------------------------------------------------------------
//inkrementacja maksymalnej wartosci mocy na palniku
//wykonywane co krok czasowy
void IncMaxCW2(void)
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
	if(Trosnie(1)||TFrosnie(1)) return;				//(Tcw lub Tco rosnie szybciej niz 1K/sek)?
	k=5;
	if(rgl.xmod+k>_MG2_MOD) rgl.xmod=_MG2_MOD;
	else rgl.xmod+=k;
	if(rgl.xmod>PWM.MMAXCW) rgl.xmod=PWM.MMAXCW;
}
//-------------------------------------------------------------
//reakcja superwizyjna
void ModPrzCW2(void)
{
	if(!rgl.nmod)									//nie jest w trybie nmod?
	{
		if(!Tmaleje(0)&&Twyzsza(3))								//(Tcw nie maleje ?)
		{
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
		WriteOPWM(rgl.tmp_pwm);
		//PWM.BufPWM=rgl.tmp_pwm;
	}
	else WriteOPWM(PWM.MDMOD);//PWM.BufPWM=PWM.MDMOD;			//wymus minimum plomienia
}
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
/*
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
 */
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
	rosnie=0;
	stoi=0;
}
//------------------------------------------------------------
//------------------------------------------------------------
#endif
