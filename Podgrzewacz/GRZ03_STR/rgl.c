/*rgl.c*/
/*---------------------------------------------------------------------------------*/
#include <pic18.h>
#include "global.h"				//parametry globalne
#include "rgl.h"				//parametry lokalne
#include "main.tp.h"
#include "pomiar.h"
#include "pwm.h"
#include "konfig.h"


//extern DataCO DtCO;
extern DataCW DtCW;
extern DtPWM PWM;
extern DtKONF DtKNF;
extern tpMAIN M;
volatile DtRgl rgl; 
extern unsigned char vvvv, vvv, vup, vdw,mmm;
extern unsigned char x,kll,gll,hll,bll;
unsigned char tmr,tmr2,subtim;
unsigned char pwm_center,pwm_kwr;
signed char popMAXCW, popMAXCO, popMIN;
signed char popSMAX,popSMIN,popSMOD;
//************************************************************
//Procedura modulacji
void Moduluj(void)
{
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
            if(subtim<0xff) subtim++;
            IncConstCW2();	//oblicza czas przez ktory temp CW = const	
            IncMaxCW2();	//inkrementacja maksymalnej wartosci mocy na palniku
            ModPruCW2();	//trzecie przyblizenie
        }
        ModPrzCW2();		//reakcja superwizyjna
        SetPWM_CW();
    }
}
//************************************************************
//------------------------------------------------------------
//------------------------------------------------------------

//************************************************************
//------------------------------------------------------------
//------------------------------------------------------------
//------wymiennik bitermiczny - obieg CW
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//DOCHODZENIE
/*
//GRZEJNIK1
//Modu³ pierwszego pobudzenia (dobieramy dla: minimalny przeplyw, minimalna temp., maksymalna nastawa)
float wspPp0=0.5;  //0.5modyfikator dla maksymalnej dozwolonej wartosci pierwszego pobudzenia
float wspPp1=0.5;  //0.5modyfikator dla wartosci pierwszego pobudzenia okreslonej ze wzgledu na temperature
float wspPp2=0.5;  //0.5modyfikator dla wartosci pierwszego pobudzenia okreslonej ze wzgledu na nastawe
float wspPp3=0.5;  //0.5modyfikator dla wartosci pierwszego pobudzenia okreslonej ze wzgledu na relacje nastawa-temperatura

//Modu³ narostu mocy
float wspInc0=0.8;      //sila korekty punktu rownowagi na szybkosc narostu temperatury
float wspInc1=0.8;      //sila przyrostu mocy

//Modul pierwszego przyblizenia (proporcionalny)
float wspZG0=0.8;       //sila regulacji w fazie dochodzenia

//Modu³ trzeciego przyblizenia (zgrubna regulacja punktu rownowagi w fazie dochodzenia)
float wspPR0=0.5;       //sila korekty punktu rownowagi w fazie dochodzenia
//------------------------------------------------------------
//------------------------------------------------------------
//STABILIZACJA
//Modu³ drugiego przyblizenia (dokladna regulacja punktu rownowagi fazie stabilizacji)
float wspDK=0.5;        //sila regulacji w fazie stabilizacji 

//Modu³ trzeciego przyblizenia (zgrubna regulacja punktu rownowagi w fazie stabilizacji)
unsigned char ktimPR=2; //krok czasowy [sek] dla korekty punktu rownowagi 
float wspPR=0.5;        //sila korekty punktu rownowagi w fazie stabilizacji
//------------------------------------------------------------
//MODU£ SUPERWIZYJNY (bardzo szybkie narosty/spadki temperatury, przekroczenia)
float wspSUP0=1.0;      //sila korekty punktu rownowagi po du¿ym przekroczeniu temperatury (stan nmod)
*/
//GRZEJNIK2
//Modu³ pierwszego pobudzenia (dobieramy dla: minimalny przeplyw, minimalna temp., maksymalna nastawa)
float wspPp0=0.5;  //0.5modyfikator dla maksymalnej dozwolonej wartosci pierwszego pobudzenia
float wspPp1=0.5;  //0.5modyfikator dla wartosci pierwszego pobudzenia okreslonej ze wzgledu na temperature
float wspPp2=0.5;  //0.5modyfikator dla wartosci pierwszego pobudzenia okreslonej ze wzgledu na nastawe
float wspPp3=0.5;  //0.5modyfikator dla wartosci pierwszego pobudzenia okreslonej ze wzgledu na relacje nastawa-temperatura

//Modu³ narostu mocy
float wspInc0=0.8;      //sila korekty punktu rownowagi na szybkosc narostu temperatury
float wspInc1=0.8;      //sila przyrostu mocy

//Modul pierwszego przyblizenia (proporcionalny)
float wspZG0=0.8;       //sila regulacji w fazie dochodzenia

//Modu³ trzeciego przyblizenia (zgrubna regulacja punktu rownowagi w fazie dochodzenia)
float wspPR0=0.5;       //sila korekty punktu rownowagi w fazie dochodzenia
//------------------------------------------------------------
//------------------------------------------------------------
//STABILIZACJA
//Modu³ drugiego przyblizenia (dokladna regulacja punktu rownowagi fazie stabilizacji)
float wspDK=0.7;        //0.8sila regulacji w fazie stabilizacji 

//Modu³ trzeciego przyblizenia (zgrubna regulacja punktu rownowagi w fazie stabilizacji)
unsigned char ktimPR=2; //krok czasowy [sek] dla korekty punktu rownowagi 
float wspPR=0.5;        //0.5sila korekty punktu rownowagi w fazie stabilizacji
//------------------------------------------------------------
//MODU£ SUPERWIZYJNY (bardzo szybkie narosty/spadki temperatury, przekroczenia)
float wspSUP0=1.0;      //sila korekty punktu rownowagi po du¿ym przekroczeniu temperatury (stan nmod)
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//parametry startowe dla CO

/*unsigned char SetStab(unsigned char ada2,unsigned char k,unsigned char res)
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
		if(!Trosnie(0))
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
		if(!Tmaleje(0))
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
}*/
//-------------------------------------------------------------
//parametry startowe dla CW
unsigned char FirstXMOD2(void);
void StartMCW2(void)
{
	if(rgl.btcw) return; //rozpoczecie pracy modulatora?
	StartRTdS(_RTMOD);
	popMIN=0;
	popMAXCW=0;
	//DtCO.dDCO=0;
	DtCW.dDCW=0;
	//DtCO.indCO=0;
	DtCW.indCW=0;
	rgl.fz=0;
	rgl.xmod=FirstXMOD2();
    PWM.BufPWM=rgl.xmod;
    //kll=rgl.xmod;
	rgl.srp=0;
	rgl.ytmp=0;
	rgl.del1s=0;
}
//-------------------------------------------------------------	
//pierwsze pobudzenie dla CW
unsigned char FirstXMOD2(void)
{
	/*{
		PWM.FIRSTPB=PWM.MAXPWM-4*((PWM.MAXPWM-PWM.MINPWM)/10);
		if(DtCW.DCW>0) return PWM.MAXPWM;
		else 
		{
			if(PWM.MAXPWM>PWM.FIRSTPB) return PWM.FIRSTPB;
			else return PWM.MAXPWM;
		}
	}*/

 	signed long m;
	unsigned char maxmod,p,p2,tmp;

	//-------
	//maksymalna dozwolona wartosc pierwszego pobudzenia
	tmp=(unsigned char)(((PWM.FIRSTPB-PWM.MDMOD)/4)*wspPp0);
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
	if(DtCW.CW>=TCWMIN_MD1&&DtCW.CW<=TCWMAX_MD1)						//temperatura w przedziale roboczym?
	{
		m=(TCWMAX_MD1-DtCW.CW)*10;
		m=m/(TCWMAX_MD1-TCWMIN_MD1);
		m=m*(maxmod-PWM.MDMOD);
		if(m<0) m=0;
        m=(signed long)(m*wspPp1);        
		m=m/10+PWM.MDMOD;
		if(m>0xff)	p=maxmod;										//przepelnienie 
		else p=m;
	}
	else															//temperatura poza przedzialem? 
	{
		if(DtCW.CW<TCWMIN_MD1) p=maxmod;
		if(DtCW.CW>TCWMAX_MD1) p=PWM.MDMOD;
	}
	//-------
	//ze wzgledu na nastawe
	m=(DtCW.PCW-F2_MIN_PCW)*10;
	m=m/(F2_MAX_PCW-F2_MIN_PCW);
	m=m*(maxmod-PWM.MDMOD);
	if(m<0) m=0;
    m=(signed long)(m*wspPp2);    
	m=m/10+PWM.MDMOD;
	if(m>0xff)	p2=maxmod;											//przepelnienie 
	else p2=m;
	//-------
	//wybrana zostaje nizsza wartosc z p i p2
	if(p>p2) p=p2;
	//-------
	//ze wzgledu na relacje miêdzy temperatura i nastawa
	if(DtCW.DCW>=DCWMIN_MD1&&DtCW.DCW<=DCWMAX_MD1)						//roznica w przedziale roboczym?
	{
		m=DtCW.DCW*10;
		m=m/(DCWMAX_MD1-DCWMIN_MD1);
		m=m*(maxmod-PWM.MDMOD);
		if(m<0) m=0;
        m=(signed long)(m*wspPp3);        
		m=m/10+PWM.MDMOD;       
		if(m>0xff)	p2=maxmod;										//przepelnienie 
		else p2=m;		
	}
	else															//roznica poza przedzialem?
	{
		if(DtCW.DCW>DCWMAX_MD1) p2=maxmod;
		if(DtCW.DCW<DCWMIN_MD1) p2=PWM.MDMOD;
	}
	//-------
	//wybrana zostaje nizsza wartosc z p i p2
	if(p>p2) p=p2;
	//-------
	return p;														//wartosc pierwszego pobudzenia   
}
//-------------------------------------------------------------
//zeruj parametry modulatora po zmianie obiegu
void ClrMod2(void)
{
	rgl.fz=0;
	rgl.med_up=0;
	rgl.fast_up=0;
	//if(rgl.btco) rgl.xmod=PWM.BufPWM;
	//else rgl.xmod=FirstXMOD2();
    rgl.xmod=PWM.BufPWM;
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
}
//-------------------------------------------------------------
//oblicza czas przez ktory temp=const
void IncConstCW2(void)
{
    if(Tstoi()) rgl.ytmp++;
    else rgl.ytmp=0;
}
//-------------------------------------------------------------
//wprowadzenie danych wejsciowych-CW
void ReadDataCW2(void)
{
	rgl.x_t=DtCW.DCW;
	//rgl.x_f=DtCO.DCO;
	rgl.x_dt=DtCW.dDCW;
	//rgl.x_df=DtCO.dDCO;
}
void ModZgCW2(void) //GRZ
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
		if(PWM.MMAXCW>PWM.MAXPWM)
		{
			if(rgl.xmod>PWM.MAXPWM)
			{
				rgl.srp=PWM.MAXPWM;
				rgl.xmod=PWM.MAXPWM;
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
	if(rgl.xmod>=PWM.MINPWM)	m=(rgl.xmod-PWM.MINPWM)*10;
	else m=0;
	m=m/(DtCW.MAD_CW-DtCW.DAD_CW);	//DAD_CW-MAD_CW
	m=m*(DtCW.AC_CW-DtCW.DAD_CW);	//DAD_CW-AC_CW	
	if(m<0) m=0;
    m=(signed long)(m*wspZG0);    
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
	else p=m;
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
void ModDkCW2(void)     //GRZ
{
	signed long m;	
	signed char ada;
	unsigned char p;
	float k;
	if(!rgl.fz)	return;					//wyjscie jesli aktualnie faza 1
	ada=DtCW.AC_CW-DtCW.DAD_CW;
	if(Trowna()) return;				//Tcw!=Tnst?
	k=1.0;
    k=k*wspDK;
    /*if(Twyzsza(0))
    {
        k=k*wspDK;
    } 
    else
    {
        k=k*wspDK;
    }*/
	m=(signed long)(rgl.srp+k*ada);
	if(m>rgl.xmod) p=rgl.xmod;
	else
	if(m<PWM.MINPWM) p=PWM.MINPWM;
	else p=m;
	rgl.tmp_pwm=p;
}
//-------------------------------------------------------------
//FAZA1+FAZA2
//trzecie przyblizenie
//nowa wartosc punktu rownowagi srp.
//wykonywane co krok czasowy
void ModPruCW2(void)    //GRZ
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
		if(DtCW._newPCW) s=0;							//zmieniono nastawe?
		if(Tnizsza(0))									//Tcw<Tnst?
		{
			if(!Tnizsza(10))							//Tnst-Tcw<=10 ?
			{
				k=0;
				/*{
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
                */
                
                if(Tnizsza(5)&&Tstoi()&&s>=1)
                {
                    s=0;
                    k=20;				//k=10
                }
                else
                if(Tnizsza(4)&&Tstoi()&&s>=1)
                {
                    s=0;
                    k=16;				//k=8
                }
                else
                if(Tnizsza(2)&&Tstoi()&&s>=1)
                {
                    s=0;
                    k=12;				//k=6
                }
                else
                if(Tstoi()&&s>=1)
                {
                    s=0;
                    k=8;				//k=4
                }                
				if(Trosnie(1)&&s>=1) 	//szybki narost temperatury
				{
					s=0;
                    k=20;               //k=10
				}
				else
				rgl.ytmp=0;
				if(PWM.BufPWM>rgl.xmod) return;
				tmp=rgl.xmod-PWM.BufPWM;
                if(k>1||wspPR0>1)
                {
                    k=(unsigned char)(k*wspPR0);
                }
				if(k>tmp) k=tmp;
				if(PWM.MMAXCW>PWM.MAXPWM)
				{
					if(rgl.srp+k>PWM.MAXPWM) rgl.srp=PWM.MAXPWM;
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
            if(Tmaleje(0)&&Twyzsza(1)&&m>=ktimPR+1)//3
			{	
				p=0;
				o=0;
				l=1;    //znacznik poprawki dodatniej
				m=0;
				k=2;
			}
			else            
            if(!Tmaleje(0)&&Twyzsza(1)&&m>=ktimPR)//3
			{	
				p=0;
				o=0;
				l=0;    //znacznik poprawki ujemnej
				m=0;
				k=2;
			}            
            if(k>1||wspPR>1)
            {
                k=(unsigned char)(k*wspPR);
            }
			if(!l&&k)	
			{
                //poprawka ujemna
				if(PWM.MMAXCW>PWM.MAXPWM)
				{
					if(rgl.srp>PWM.MAXPWM) rgl.srp=PWM.MAXPWM;
				}
				else
				{
					if(rgl.srp>PWM.MMAXCW) rgl.srp=PWM.MMAXCW;
				}
				if(rgl.srp-k<PWM.MINPWM) rgl.srp=PWM.MINPWM;
				else rgl.srp=rgl.srp-k;
			}
			else    
			{
                //poprawka dodatnia
				if(PWM.MMAXCW>PWM.MAXPWM)
				{
					if(rgl.srp+k>PWM.MAXPWM) rgl.srp=PWM.MAXPWM;
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
            if(Trosnie(0)&&Tnizsza(1)&&m>=(ktimPR+1))    
			{	
				p=0;
				o=0;
				l=1;    //znacznik poprawki ujemnej
				m=0;
				k=1;			
			}
			else            
            if(!Trosnie(0)&&Tnizsza(1)&&m>=ktimPR)				//1
			{	
				p=0;
				o=0;
				l=0;    //znacznik poprawki dodatniej
				m=0;
				k=1;			
			}
            if(k>1||wspPR>1)
            {
                k=(unsigned char)(k*wspPR);
            }
			if(!l&&k)	
			{
                //poprawka dodatnia
				if(PWM.MMAXCW>PWM.MAXPWM)
				{
					if(rgl.srp+k>PWM.MAXPWM) rgl.srp=PWM.MAXPWM;
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
                //poprawka ujemna
				if(PWM.MMAXCW>PWM.MAXPWM)
				{
					if(rgl.srp>PWM.MAXPWM) rgl.srp=PWM.MAXPWM;
				}
				else
				{
					if(rgl.srp>PWM.MMAXCW) rgl.srp=PWM.MMAXCW;
				}                
				if(rgl.srp-k<PWM.MINPWM) rgl.srp=PWM.MINPWM;
				else rgl.srp=rgl.srp-k;
			}
			if(rgl.xmod<rgl.srp) rgl.xmod=rgl.srp;
			return;
		}
	}
}
/*void ModPruCW2(void)    //GRZ
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
                if(k>1||wspPR0>1)
                {
                    k=(unsigned char)(k*wspPR0);
                }
				if(k>tmp) k=tmp;
				if(PWM.MMAXCW>PWM.MAXPWM)
				{
					if(rgl.srp+k>PWM.MAXPWM) rgl.srp=PWM.MAXPWM;
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
			if(Tmaleje(0)&&Twyzsza(1)&&m>=3)//3
			{	
				p=0;
				o=0;
				l=1;
				m=0;
				k=1;
			}
			else
			if(!Tmaleje(0)&&Twyzsza(4)&&m>=2)				//gdy jest powyzej zakresu nastawa+1
			{
				p=0;
				o=0;
				l=0;
				m=0;
				k=3;//5
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
            if(k>1||wspPR1>1)
            {
                k=(unsigned char)(k*wspPR1);
            }
			if(!l&&k)	//poprawka dodatnia?
			{
				if(PWM.MMAXCW>PWM.MAXPWM)
				{
					if(rgl.srp>PWM.MAXPWM) rgl.srp=PWM.MAXPWM;
				}
				else
				{
					if(rgl.srp>PWM.MMAXCW) rgl.srp=PWM.MMAXCW;
				}
				if(rgl.srp-k<PWM.MINPWM) rgl.srp=PWM.MINPWM;
				else rgl.srp=rgl.srp-k;
			}
			else
			{
				if(PWM.MMAXCW>PWM.MAXPWM)
				{
					if(rgl.srp+k>PWM.MAXPWM) rgl.srp=PWM.MAXPWM;
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
            if(k>1||wspPR2>1)
            {
                k=(unsigned char)(k*wspPR2);
            }
			if(!l&&k)	//poprawka dodatnia?
			{
				if(PWM.MMAXCW>PWM.MAXPWM)
				{
					if(rgl.srp+k>PWM.MAXPWM) rgl.srp=PWM.MAXPWM;
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
				if(rgl.srp-k<PWM.MINPWM) rgl.srp=PWM.MINPWM;
				else rgl.srp=rgl.srp-k;
			}
			if(rgl.xmod<rgl.srp) rgl.xmod=rgl.srp;
			return;
		}
	}
}*/
//-------------------------------------------------------------
//inkrementacja maksymalnej wartosci mocy na palniku
//wykonywane co krok czasowy
void IncMaxCW2(void) //GRZ
{
    static unsigned char m,n;
	signed char k,sm;
	float z,wsp;
    
	//decyzja o wejsciu w tryb INVMOD
	//kontrola maksimum mocy dla fazy 2
	if(rgl.fz)											//faza 2 ?
	{
		if(PWM.MMAXCW>PWM.MAXPWM)
		{
			if(rgl.xmod>PWM.MAXPWM) rgl.xmod=PWM.MAXPWM;
		}
		else
		{
			if(rgl.xmod>PWM.MMAXCW) rgl.xmod=PWM.MMAXCW;
		}
		return;
	}

	//jednorazowa modyfikacja wartosci srp ze wzgledu na szybkosc 
	//narostu temperatury
    if(Tnizsza(0))
    {
        if(Trosnie(2)&&!rgl.med_up)		//(Tcw rosnie szybciej niz 3K/sek)?	
        {
            rgl.med_up=1;
            k=5;						//10
            if(k>1||wspInc0>=1)
            {
                k=(unsigned char)(k*wspInc0);
            }
            if(PWM.MMAXCW>PWM.MAXPWM)
            {
                if(rgl.srp+k>PWM.MAXPWM) rgl.srp=PWM.MAXPWM;
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
            if(k>1||wspInc0>=1)
            {
                k=(unsigned char)(k*wspInc0);
            }
            if(PWM.MMAXCW>PWM.MAXPWM)
            {
                if(rgl.srp+k>PWM.MAXPWM) rgl.srp=PWM.MAXPWM;
                else rgl.srp+=k;
            }
            else
            {
                if(rgl.srp+k>PWM.MMAXCW) rgl.srp=PWM.MMAXCW;
                else rgl.srp+=k;
            }
        }
    }
	if(Trosnie(0)) return;
    //zmienny krok narostu mocy
    z=(wspInc1/2.0)/(DtCW.maxPCW-DtCW.minPCW);
    wsp=(wspInc1/2.0)+((DtCW.PCW-DtCW.minPCW)*z);
    k=5;
    if(k>1||wsp>=1)
    {
        k=(unsigned char)(k*wsp);
    }
    if(rgl.xmod+k<PWM.MINPWM) rgl.xmod=PWM.MINPWM;
    
	if(rgl.xmod+k>PWM.MAXPWM) rgl.xmod=PWM.MAXPWM;
	else rgl.xmod+=k;
	if(rgl.xmod>PWM.MMAXCW) rgl.xmod=PWM.MMAXCW;    
}
//-------------------------------------------------------------
//reakcja superwizyjna
void ModPrzCW2(void)    //GRZ
{
	volatile unsigned char k;//p,maxmod;
	signed int r;
	static unsigned char timer1;
 
	//stabilizacja predkosci dochodzenia do temp. zadanej.
	/*if(Tnizsza(0))
	{
        //if(timer1>1)
        
		if(PWM.MMAXCW<PWM.MAXPWM) maxmod=PWM.MMAXCW;						//wprowadzono ograniczenie mocy?
		else maxmod=PWM.MAXPWM;
		if(rgl.tmp_pwm>=pwm_center)	
		{	
			if(rgl.xmod<=maxmod) p=rgl.xmod;
			else
			{
	 			p=maxmod;	
				rgl.xmod=maxmod;
			}
			if((DtCW.ddDCW>(6+popSup0))&&(tmr>1))											//temperatura rosnie szybciej ni¿ 6 stop. na sek.?							
			{
				tmr=0;
                rgl.xmod=rgl.xmod-((rgl.xmod-PWM.MINPWM)/5)*3;
				//r=p-PWM.MINPWM;
				//if((p-(r/3))>PWM.MINPWM) p-=(unsigned char)((r/3)*wspSup0);//4
				///else p=PWM.MINPWM;
				//rgl.xmod=p;
			}
			else								
			if((DtCW.ddDCW>(4+popSup0))&&(tmr>1))											//temperatura rosnie szybciej ni¿ 4 stop. na sek.?							
			{
				tmr=0;
				//r=p-PWM.MINPWM;
				//if((p-(r/4))>PWM.MINPWM) p-=(unsigned char)((r/4)*wspSup0);//4
				//else p=PWM.MINPWM;
				//rgl.xmod=p;
                rgl.xmod=rgl.xmod-((rgl.xmod-PWM.MINPWM)/5)*2;
			}
			else
			if((DtCW.ddDCW>(3+popSup0))&&(tmr>0)) //tmr2									//temperatura rosnie szybciej ni¿ 3 stop. na sek.?							
			{
				tmr=0;
				//r=p-PWM.MINPWM;
				//if((p-(r/14))>PWM.MINPWM) p-=(unsigned char)((r/14)*wspSup0);//16
				//else p=PWM.MINPWM;
				//rgl.xmod=p;
                rgl.xmod=rgl.xmod-((rgl.xmod-PWM.MINPWM)/5)*1;
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
			if((DtCW.ddDCW>(6+popSup1))&&(tmr>1))										//temperatura rosnie szybciej ni¿ 6 stop. na sek.?							
			{
				tmr=0;
                
				//r=p-PWM.MINPWM;
				//if((p-(r/2))>PWM.MINPWM) p-=(unsigned char)((r/2)*wspSup1);//2
				//else p=PWM.MINPWM;
				//rgl.xmod=p;
                rgl.xmod=rgl.xmod-((rgl.xmod-PWM.MINPWM)/5)*3;
			}
			else								
			if((DtCW.ddDCW>(4+popSup1))&&(tmr>1))											//temperatura rosnie szybciej ni¿ 4 stop. na sek.?							
			{
				tmr=0;
				//r=p-PWM.MINPWM;
				//if((p-(r/3))>PWM.MINPWM) p-=(unsigned char)((r/3)*wspSup1);//3
				//else p=PWM.MINPWM;
				//rgl.xmod=p;
                
                rgl.xmod=rgl.xmod-((rgl.xmod-PWM.MINPWM)/5)*2;
			}
			else
			if((DtCW.ddDCW>(3+popSup1))&&(tmr>0)) //tmr2									//temperatura rosnie szybciej ni¿ 3 stop. na sek.?							
			{
				tmr=0;
				//r=p-PWM.MINPWM;
				//if((p-(r/4))>PWM.MINPWM) p-=(unsigned char)((r/4)*wspSup1);//8
				//else p=PWM.MINPWM;
				//rgl.xmod=p;
                
                rgl.xmod=rgl.xmod-((rgl.xmod-PWM.MINPWM)/5)*1;
			}
		}
        
	}*/
    if(subtim>1)
    {
        k=8;        
        subtim=0;
        //if(rgl.nmod&&!Tmaleje(0))
        if(rgl.nmod)    
        {

            if(k>1||wspSUP0>1)
            {
                k=(unsigned char)(k*wspSUP0);
            }            
            if(PWM.MMAXCW>PWM.MAXPWM)
            {
                if(rgl.srp>PWM.MAXPWM) rgl.srp=PWM.MAXPWM;
            }
            else
            {
                if(rgl.srp>PWM.MMAXCW) rgl.srp=PWM.MMAXCW;
            }
            if(rgl.srp-k<PWM.MINPWM) rgl.srp=PWM.MINPWM;
            else rgl.srp=rgl.srp-k;            
        }
    }
    //k=25;
	//UWAGA: histereza!!!
	if(!rgl.nmod)					//nie jest w trybie nmod?
	{
        if(!Tmaleje(0))			//(Tcw nie maleje ?)
        {
            if(Twyzsza(10))		//8Tcw-Tnst>4 ?
            {
                rgl.nmod=1;		//wejdz w tryb nmod
                subtim=0;
                return;
            }
            else return;
        }
        else return;
	}
	else							//jest w trybie nmod?
	{
		if((Tmaleje(0)||Tstoi())&&!Twyzsza(9))				//7(Tcw maleje ?)
        {
		 			rgl.nmod=0;		//wyjdz z trybu nmod
        }
	}
	timer1++;
}
/*
void ModPrzCW2(void)    //GRZ
{
	unsigned char p,maxmod;
	signed int r;
	static unsigned char timer1;
 
	//stabilizacja predkosci dochodzenia do temp. zadanej.
	if(Tnizsza(0))
	{
        //if(timer1>1)
        
		if(PWM.MMAXCW<PWM.MAXPWM) maxmod=PWM.MMAXCW;						//wprowadzono ograniczenie mocy?
		else maxmod=PWM.MAXPWM;
		if(rgl.tmp_pwm>=pwm_center)	
		{	
			if(rgl.xmod<=maxmod) p=rgl.xmod;
			else
			{
	 			p=maxmod;	
				rgl.xmod=maxmod;
			}
			if((DtCW.ddDCW>(6+popSup0))&&(tmr>1))											//temperatura rosnie szybciej ni¿ 6 stop. na sek.?							
			{
				tmr=0;
                rgl.xmod=rgl.xmod-((rgl.xmod-PWM.MINPWM)/5)*3;
				//r=p-PWM.MINPWM;
				//if((p-(r/3))>PWM.MINPWM) p-=(unsigned char)((r/3)*wspSup0);//4
				///else p=PWM.MINPWM;
				//rgl.xmod=p;
			}
			else								
			if((DtCW.ddDCW>(4+popSup0))&&(tmr>1))											//temperatura rosnie szybciej ni¿ 4 stop. na sek.?							
			{
				tmr=0;
				//r=p-PWM.MINPWM;
				//if((p-(r/4))>PWM.MINPWM) p-=(unsigned char)((r/4)*wspSup0);//4
				//else p=PWM.MINPWM;
				//rgl.xmod=p;
                rgl.xmod=rgl.xmod-((rgl.xmod-PWM.MINPWM)/5)*2;
			}
			else
			if((DtCW.ddDCW>(3+popSup0))&&(tmr>0)) //tmr2									//temperatura rosnie szybciej ni¿ 3 stop. na sek.?							
			{
				tmr=0;
				//r=p-PWM.MINPWM;
				//if((p-(r/14))>PWM.MINPWM) p-=(unsigned char)((r/14)*wspSup0);//16
				//else p=PWM.MINPWM;
				//rgl.xmod=p;
                rgl.xmod=rgl.xmod-((rgl.xmod-PWM.MINPWM)/5)*1;
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
			if((DtCW.ddDCW>(6+popSup1))&&(tmr>1))										//temperatura rosnie szybciej ni¿ 6 stop. na sek.?							
			{
				tmr=0;
                
				//r=p-PWM.MINPWM;
				//if((p-(r/2))>PWM.MINPWM) p-=(unsigned char)((r/2)*wspSup1);//2
				//else p=PWM.MINPWM;
				//rgl.xmod=p;
                rgl.xmod=rgl.xmod-((rgl.xmod-PWM.MINPWM)/5)*3;
			}
			else								
			if((DtCW.ddDCW>(4+popSup1))&&(tmr>1))											//temperatura rosnie szybciej ni¿ 4 stop. na sek.?							
			{
				tmr=0;
				//r=p-PWM.MINPWM;
				//if((p-(r/3))>PWM.MINPWM) p-=(unsigned char)((r/3)*wspSup1);//3
				//else p=PWM.MINPWM;
				//rgl.xmod=p;
                
                rgl.xmod=rgl.xmod-((rgl.xmod-PWM.MINPWM)/5)*2;
			}
			else
			if((DtCW.ddDCW>(3+popSup1))&&(tmr>0)) //tmr2									//temperatura rosnie szybciej ni¿ 3 stop. na sek.?							
			{
				tmr=0;
				//r=p-PWM.MINPWM;
				//if((p-(r/4))>PWM.MINPWM) p-=(unsigned char)((r/4)*wspSup1);//8
				//else p=PWM.MINPWM;
				//rgl.xmod=p;
                
                rgl.xmod=rgl.xmod-((rgl.xmod-PWM.MINPWM)/5)*1;
			}
		}
        
	}

	//UWAGA: histereza!!!
	if(!rgl.nmod)					//nie jest w trybie nmod?
	{
        if(!Tmaleje(0))			//(Tcw nie maleje ?)
        {
            if(Twyzsza(10))		//8Tcw-Tnst>4 ?
            {
                rgl.nmod=1;		//wejdz w tryb nmod
                return;
            }
            else return;
        }
        else return;
	}
	else							//jest w trybie nmod?
	{
		if((Tmaleje(0)||Tstoi())&&!Twyzsza(9))				//7(Tcw maleje ?)
        {
		 			rgl.nmod=0;		//wyjdz z trybu nmod
        }
	}
	timer1++;
}*/
//------------------------------------------------------------
void SetPWM_CW(void)
{
	if(!rgl.nmod)					//nie jest w trybie nmod?
	{
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
//Zabezpieczenie od przegrzania w obwodzie CO			
void PropCO2(void)
{
	if(!rgl.nmod)					//nie jest w trybie nmod?
	{
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
        case 2: //NG31
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
        case 2: //NG31
        {
            pw=CalkVNTtoPWM(ppv);	//teoretyczna oczekiwana wartosc PWM dla pv
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
	rgl.btcw=0;
	rgl.fz=0;
	rgl.med_up=0;
	rgl.fast_up=0;
	rgl.nmod=0;
	rgl.del1s=0;
	rgl.deln1=0;
	rgl.inv_mod=0;
}
