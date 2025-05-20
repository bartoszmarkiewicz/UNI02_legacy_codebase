/*rgl.c*/
/*---------------------------------------------------------------------------------*/
#include <pic18.h>
#include "global.h"				//parametry globalne
#include "rgl.h"				//parametry lokalne
#include "main.tp.h"
#include "pomiar.h"
#include "pwm.h"
#include "konfig.h"


extern DataCO DtCO;
extern DataCW DtCW;
extern DtPWM PWM;
extern DtKONF DtKNF;
extern tpMAIN M;
DtRgl rgl; 

extern unsigned char x;

//************************************************************
//Procedura modulacji
void Moduluj(void)
{
	unsigned int wpw;
	unsigned char wpp;

	switch(DtKNF.tpkt)			//rodzaj kotla
	{
		case _BITERM:			//kociol bitermiczny (miniterm)
		{
			if(M._pob_CO)		//obieg CO
			{
				PWM.MAXPWM=PWM.MMAXCO; //maksymalna nastawa modulatora
				StartCO();			//parametry startowe dla CO
				ReadDataCO();		//wprowadzenie danych wejsciowych dla CO
				ModToCO();			//zmien obieg na CO
				ModZgCO();			//pierwsze przyblizenie
				ModDkCO();			//drugie przyblizenie
				if(RTdS(_RTMOD)>_TOKM)
				{
					StartRTdS(_RTMOD);
					IncConstCOCW();	//oblicza czas przez ktory temp CO = const
					IncMaxCO();		//inkrementacja maksymalnej wartosci mocy na palniku
					ModPruCO();		//trzecie przyblizenie
				}
				ModPrzCO();			//reakcja superwizyjna
				PropOPT();
			}
			else
			if(M._pob_CW)			//obieg CW
			{
				StartMCW();			//parametry startowe dla CW
				ReadDataCW();		//wprowadzenie danych wejsciowych dla CW
				ModToCW();			//zmien obieg na CW
				ModZgCW();			//pierwsze przyblizenie
				ModDkCW();			//drugie przyblizenie
				if(RTdS(_RTMOD)>_TOKM)
				{
					StartRTdS(_RTMOD);
					IncConstCOCW();	//oblicza czas przez ktory temp CW = const	
					IncMaxCW();		//inkrementacja maksymalnej wartosci mocy na palniku
					ModPruCW();		//trzecie przyblizenie
				}
				ModPrzCW();			//reakcja superwizyjna
				PropCO();			//zabezpieczenie od przegrzania w obwodzie CO				StartMCW2();		//parametry startowe dla CW
			}
			break;
		}
		case _MONOTERM:				//kociol z wymiennikiem plytowym
		case _MONOHB:				//kociol z hydroblokiem
		{
			if(M._pob_CO)			//obieg CO
			{
				PWM.MAXPWM=PWM.MMAXCO;//maksymalna nastawa modulatora
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
		case _ZASOBNIK:				//kociol zasobnikowy
		{
			if(M._pob_CO)			//obieg CO
			{
				PWM.MAXPWM=PWM.MMAXCO;//maksymalna nastawa modulatora
				StartMCO3();		//parametry startowe dla CO
				ReadDataCO2();		//wprowadzenie danych wejsciowych dla CO
				ModToCO3();			//zmien obieg na CO
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
			if(M._pob_CW||M._pob_AL)	//obieg CW
			{
				PWM.MAXPWM=PWM.MMAXCW;//maksymalna nastawa modulatora
				StartMCO3();		//parametry startowe dla CO
				ReadDataCO2();		//wprowadzenie danych wejsciowych dla CO
				ModToCW3();			//zmien obieg na CO
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
			break;
		}
		case _BITERM2:			//kociol bitermiczny (maxiterm)
		{
			if(M._pob_CO)		//obieg CO
			{
				PWM.MAXPWM=PWM.MMAXCO; //maksymalna nastawa modulatora
				StartCO4();			//parametry startowe dla CO
				ReadDataCO4();		//wprowadzenie danych wejsciowych dla CO
				ModToCO4();			//zmien obieg na CO
				ModZgCO();			//pierwsze przyblizenie
				ModDkCO();			//drugie przyblizenie
				if(RTdS(_RTMOD)>_TOKM)
				{
					StartRTdS(_RTMOD);
					IncConstCOCW4();//oblicza czas przez ktory temp CO = const
					IncMaxCO();		//inkrementacja maksymalnej wartosci mocy na palniku
					ModPruCO();		//trzecie przyblizenie
				}
				ModPrzCO();			//reakcja superwizyjna
				PropOPT();
			}
			else
			if(M._pob_CW)			//obieg CW
			{
				StartMCW4();			//parametry startowe dla CW
				ReadDataCW4();		//wprowadzenie danych wejsciowych dla CW
				ModToCW4();			//zmien obieg na CW
				ModZgCW4();			//pierwsze przyblizenie
				ModDkCW4();			//drugie przyblizenie
				if(RTdS(_RTMOD)>_TOKM)
				{
					StartRTdS(_RTMOD);
					IncConstCOCW4();	//oblicza czas przez ktory temp CW = const	
					IncMaxCW4();		//inkrementacja maksymalnej wartosci mocy na palniku
					ModPruCW4();		//trzecie przyblizenie
				}
				ModPrzCW4();			//reakcja superwizyjna
				PropCO4();			//zabezpieczenie od przegrzania w obwodzie CO				StartMCW2();		//parametry startowe dla CW
			}
			break;
		}
		default:		//domyslny monotermiczny
		{
			if(M._pob_CO)			//obieg CO
			{
				PWM.MAXPWM=PWM.MMAXCO;//maksymalna nastawa modulatora
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
//------dowolny wymiennik - obieg CO
//------------------------------------------------------------
//------------------------------------------------------------								
//-------------------------------------------------------------
//parametry startowe dla CO - kociol 1 funkcyjny.
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
			if(rgl.srp-k<PWM.MDMOD) rgl.srp=PWM.MDMOD;
			else rgl.srp-=k;
		}
	}
	else //rozpoczecie pracy w obiegu CO lub CW
	{
		StartRTdS(_RTMOD);
		DtCO.dDCO=0;
		DtCO.indCO=0;
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
//		if(!PWM.tpGZ) rgl.xmod=I80_MOD;
//		else rgl.xmod=I80_PMOD;
		rgl.srp=0;
		rgl.ytmp=0;
		rgl.nmod=0;		//wyjdz z trybu nmod
		rgl.btcw=0;
		rgl.btco=1;
		rgl.del1s=0;
		rgl.deln1=0;
		rgl.inv_mod=0;
	}
}
//-------------------------------------------------------------
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
	k=5;
	if(Twyzsza(0)) k=6;			//6		//(Tco>Tnst)?
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
			k=6;
			if(k>tmp) k=tmp;
			if(rgl.srp+k>PWM.MGMOD) rgl.srp=PWM.MGMOD;
			else rgl.srp+=k;
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
				if(rgl.srp-k<PWM.MDMOD) rgl.srp=PWM.MDMOD;
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
				if(PWM.MAXPWM>PWM.MGMOD)
				{
					if(rgl.srp+k>PWM.MGMOD) rgl.srp=PWM.MGMOD;
					else rgl.srp+=k;
				}
				else
				{
					if(rgl.srp+k>PWM.MAXPWM) rgl.srp=PWM.MAXPWM;
					else rgl.srp+=k;
				}
				if(rgl.xmod<rgl.srp) rgl.xmod=rgl.srp;
			}
		}else l=0;
	}
}
//-------------------------------------------------------------								
//inkrementacja maksymalnej wartosci mocy na palniku
//wykonywane co krok czasowy
void IncMaxCO(void)
{
	unsigned char k;
	
	if(Trosnie(1)) return;								//(Tco rosnie szybciej niz 1K/sek)?	
	//kontrolowany narost mocy
	k=6;
	if(!rgl.fz)											//faza 1 ?
	{
		if(rgl.xmod+k>_MG2_MOD) rgl.xmod=_MG2_MOD;
		else rgl.xmod+=k;
		if(rgl.xmod>PWM.MAXPWM) rgl.xmod=PWM.MAXPWM;		
	}
	else												//faza 2
	{
		if(PWM.MAXPWM>PWM.MGMOD)
		{
			if(rgl.xmod+k>PWM.MGMOD) rgl.xmod=PWM.MGMOD;
			else rgl.xmod+=k;
		}
		else
		{
			if(rgl.xmod+k>PWM.MAXPWM) rgl.xmod=PWM.MAXPWM;
			else rgl.xmod+=k;
		}
	}
}
//-------------------------------------------------------------								
//reakcja superwizyjna
void ModPrzCO(void)
{
	unsigned char k;
	
	if(!rgl.nmod)					//nie jest w trybie nmod?
	{
		if(Twyzsza(3))				//Tcw-Tnst>3 ?
		{
			rgl.nmod=1;				//wejdz w tryb nmod
			k=20;
			if(rgl.srp-k<PWM.MDMOD) rgl.srp=PWM.MDMOD;
			else rgl.srp-=k;
		}
	}
	else							//jest w trybie nmod?
	{
		if(!Twyzsza(2))				
		 			rgl.nmod=0;		//wyjdz z trybu nmod
	}
}
//-------------------------------------------------------------							
//Proporcjonalne OPEN-T	
void PropOPT(void)
{
	if(rgl.nmod)				//aktywny tryb nmod?
	{
		WriteOPWM(PWM.MDMOD);
		//PWM.BufPWM=PWM.MDMOD;	//wymus minimum plomienia
	}
	else
	{
		WriteOPWM(rgl.tmp_pwm);
		//PWM.BufPWM=rgl.tmp_pwm;
	}
}
//************************************************************
//------------------------------------------------------------
//------------------------------------------------------------
//------wymiennik bitermiczny (miniterm) - obieg CW
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//parametry startowe dla CO
void StartCO(void)
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
void StartMCW(void)
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
	FirstPCW();
	rgl.srp=0;
	rgl.ytmp=0;
	rgl.nmod=0;
	rgl.del1s=0;
	rgl.inv_mod=0;
}
//-------------------------------------------------------------
//pierwsze pobudzenie dla CW
void FirstPCW(void)
{
	if(PWM.MMAXCW>PWM.MDMOD)
	{
		rgl.xmod=PWM.MDMOD;
	}
	else
	{
		rgl.xmod=PWM.MMAXCW;
	}
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
//-------------------------------------------------------------
//pierwsze przyblizenie
void ModZgCW(void)
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
void ModDkCW(void)
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
void ModPrzCW(void)
{
	if(!rgl.nmod)					//nie jest w trybie nmod?
	{
		if(rgl.fz)					//faza 2 ?
		{
			if(!Tmaleje(0))			//(Tcw nie maleje ?)
			{
				if(Twyzsza(4))		//Tcw-Tnst>4 ?
				{
					if(rgl.srp-15<PWM.MDMOD) rgl.srp=PWM.MDMOD;
					else rgl.srp-=15;
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
		WriteOPWM(rgl.tmp_pwm);
	}
	else WriteOPWM(PWM.MDMOD);//PWM.BufPWM=PWM.MDMOD;			//wymus minimum plomienia
}
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
		WriteOPWM(rgl.tmp_pwm);
	}
	else WriteOPWM(PWM.MDMOD);				//wymus minimum plomienia
}
//************************************************************
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
/*
	if(!PWM.tpGZ)
	{
		if(PWM.MMAXCO>I80_MOD)
		{
			rgl.xmod=I80_MOD;
		}
		else
		{
			rgl.xmod=PWM.MMAXCO;
		}
	}
	else
	{
		if(PWM.MMAXCO>I80_PMOD)
		{
			rgl.xmod=I80_PMOD;
		}
		else
		{
			rgl.xmod=PWM.MMAXCO;
		}
	}
*/
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
/*	if(!PWM.tpGZ)
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
	else
	{
		if(PWM.MMAXCW>PWM.MGMOD)
		{
			if(DtCW.DCW>0) rgl.xmod=PWM.MGMOD;
			else rgl.xmod=I80_PMOD;
		}
		else
		{
			if(DtCW.DCW>0) rgl.xmod=PWM.MMAXCW;
			else 
			{
				if(PWM.MMAXCW>I80_PMOD) rgl.xmod=I80_PMOD;
				else rgl.xmod=PWM.MMAXCW;
			}
		}
	}
*/
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
