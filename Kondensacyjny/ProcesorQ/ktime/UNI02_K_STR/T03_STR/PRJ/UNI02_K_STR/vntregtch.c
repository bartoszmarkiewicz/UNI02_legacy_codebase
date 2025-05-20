/*vntregtch.c*/
//Druga poprawiona wersja oprogramowania
//Zwiekszona precyzja pomiaru - wynik typu int
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

#include "vntregtch.h"			//parametry lokalne

extern unsigned char xxy,zzz,vvv,mmm;
extern unsigned char x,kll,hll;
extern tpRSDTA1 RSDT1;
extern DtPWM PWM;			//pwm.c
extern tpMAIN M;
extern DtKONF DtKNF;
extern DataPFN PFN;
extern DtPMP PMP;
extern volatile signed long ddDVNT,v_ddt;
extern volatile signed int DVNT,dDVNT,tdDVNT,v_t,v_dt,v_tdt;

KALVnt Kvnt[8];
KALVnt2 Kvnt2[10];

const unsigned char nkpkt=10;

//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
static bool inline InGetBit(unsigned char data, const unsigned char nbit)
{
	return ((data&(1<<nbit))?1:0);    
}
//-------------------------------------------------------------------------------
static void inline InBitSet(unsigned char *data, const unsigned char nbit,const unsigned char bt)
{
	*data=(bt)?(*data|(1<<nbit)):(*data&(~(1<<nbit)));
}
//--------------------------------------------------------------------------------------
//Krok kalibracji ukladu
//gdzie nkpkt-liczba krokow kalibracji
//krk-aktualny krok kalibracji
//pwm-zadane wypelnienie pwm
#define _PPPWM 2
#if _HIRESVNT==0
unsigned char SetKalPWM(unsigned char krk,unsigned char pwm)
{
	unsigned char tim,ppp;

    if(!krk)
    {
        tim=60+krk*(40/nkpkt);			//35kalibracja kroku czasowego w zaleznosci od liczby punktów kalibracyjnych
    }
    else
    {
        tim=35+krk*(40/nkpkt);			//35kalibracja kroku czasowego w zaleznosci od liczby punktów kalibracyjnych    
    }

	if(RTdS(_RTFKVN)<tim)			//wybieg na zadanej mocy
	{
		WritePWM3(pwm);
		return 0;
	}
	else							//zapamietanie wartosci kluczowych
	{
		StartRTdS(_RTFKVN);
		Kvnt[krk].dvnt=(unsigned char) (round(M.rVVNT/100.0));		//odczytana predkosc obrotowa
        switch(DtKNF.rfan)  //rodzaj wentylatora
        {
            case 0: //FIME
            case 2:                
            {
                Kvnt[krk].dpwm=pwm;     //aktualna wartosc PWM
                break;
            }
            case 1: //NG40
            {
                ppp=_PPPWM;		//poprawka kalibracyjna
                if((krk>2)||(Kvnt[krk].dvnt>12)) ppp=0;
                if((pwm+ppp)<=0xff)
                {
                    Kvnt[krk].dpwm=pwm+ppp; //aktualna wartosc PWM
                }
                else
                {
                    Kvnt[krk].dpwm=0xff;    //aktualna wartosc PWM
                }                
                break;
            }
            default:
            {
                Kvnt[krk].dpwm=pwm;     //aktualna wartosc PWM
                break;                
            }
		}
	}
	return 1;
}
#else
unsigned char SetKalPWM(unsigned char krk,unsigned int pwm)
{
	unsigned char tim,ppp;
    unsigned int pw;

    if(!krk)
    {
        tim=60+krk*(40/nkpkt);			//60kalibracja kroku czasowego w zaleznosci od liczby punktów kalibracyjnych
    }
    else
    {
        tim=30+krk*(40/nkpkt);			//35kalibracja kroku czasowego w zaleznosci od liczby punktów kalibracyjnych    
    }

	if(RTdS(_RTFKVN)<tim)			//wybieg na zadanej mocy
	{
		WritePWM3H(pwm);
		return 0;
	}
	else							//zapamietanie wartosci kluczowych
	{
		StartRTdS(_RTFKVN);
		Kvnt2[krk].dvnt=M.rVVNT;		//odczytana predkosc obrotowa
        Kvnt2[krk].dpwm=pwm;            //aktualna wartosc PWM
	}
	return 1;
}

unsigned char SetKalVNT(unsigned char ref,unsigned char krk,unsigned char vnt)
{
	unsigned char tim,ppp,stp;
    static unsigned int pwm,vt;

    if(ref)
    {
        StartRTdS(_RTFKVN);
        M.rVVNT=0;
        pwm=0;
        return 0;
    }
    vt=vnt*100;
    if(RTdS(_RTFKVN)>2)			//wybieg na zadanej mocy
    {
        StartRTdS(_RTFKVN);
        stp=1;
        //if((M.rVVNT-vt)>100) stp=2;
        if((M.rVVNT-vt)>500) stp=2;
        if(M.rVVNT<vt)
        {
            pwm+=stp;
            WritePWM3H(pwm);
            return 0;
        }
        else
        {
            Kvnt2[krk].dvnt=M.rVVNT;		//odczytana predkosc obrotowa
            Kvnt2[krk].dpwm=pwm;            //aktualna wartosc PWM
            return 1;  
        }
    }
    WritePWM3H(pwm);
	return 0;
}
#endif
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//ustaw domyslne dane kalibracji na potrzeby odbioru produkcyjnego
#if _HIRESVNT==0
void SetDefFan(const unsigned char tp)
{
    switch(tp)
    {
        case 0:
        {
            Kvnt[0].dvnt=9;
            Kvnt[0].dpwm=18;

            Kvnt[1].dvnt=32;
            Kvnt[1].dpwm=51;

            Kvnt[2].dvnt=46;
            Kvnt[2].dpwm=84;

            Kvnt[3].dvnt=56;
            Kvnt[3].dpwm=117;

            Kvnt[4].dvnt=63;
            Kvnt[4].dpwm=150;

            Kvnt[5].dvnt=69;
            Kvnt[5].dpwm=183;

            Kvnt[6].dvnt=75;
            Kvnt[6].dpwm=216;

            Kvnt[7].dvnt=80;
            Kvnt[7].dpwm=249;
            break;
        }
        case 1:
        {
            Kvnt[0].dvnt=8;
            Kvnt[0].dpwm=33;

            Kvnt[1].dvnt=32;
            Kvnt[1].dpwm=62;

            Kvnt[2].dvnt=44;
            Kvnt[2].dpwm=84;

            Kvnt[3].dvnt=63;
            Kvnt[3].dpwm=124;

            Kvnt[4].dvnt=75;
            Kvnt[4].dpwm=155;

            Kvnt[5].dvnt=86;
            Kvnt[5].dpwm=186;

            Kvnt[6].dvnt=96;
            Kvnt[6].dpwm=217;

            Kvnt[7].dvnt=104;
            Kvnt[7].dpwm=248;  
            break;
        }
        default:
        {
            Kvnt[0].dvnt=9;
            Kvnt[0].dpwm=18;

            Kvnt[1].dvnt=32;
            Kvnt[1].dpwm=51;

            Kvnt[2].dvnt=46;
            Kvnt[2].dpwm=84;

            Kvnt[3].dvnt=56;
            Kvnt[3].dpwm=117;

            Kvnt[4].dvnt=63;
            Kvnt[4].dpwm=150;

            Kvnt[5].dvnt=69;
            Kvnt[5].dpwm=183;

            Kvnt[6].dvnt=75;
            Kvnt[6].dpwm=216;

            Kvnt[7].dvnt=80;
            Kvnt[7].dpwm=249;
            break;
        }
    }
    WriteEEVNT0(Kvnt[0].dvnt,Kvnt[0].dpwm);
    WriteEEVNT1(Kvnt[1].dvnt,Kvnt[1].dpwm);
    WriteEEVNT2(Kvnt[2].dvnt,Kvnt[2].dpwm);
    WriteEEVNT3(Kvnt[3].dvnt,Kvnt[3].dpwm);
    WriteEEVNT4(Kvnt[4].dvnt,Kvnt[4].dpwm);
    WriteEEVNT5(Kvnt[5].dvnt,Kvnt[5].dpwm);
    WriteEEVNT6(Kvnt[6].dvnt,Kvnt[6].dpwm);
    WriteEEVNT7(Kvnt[7].dvnt,Kvnt[7].dpwm);    
    
    //80/127=63
    //
}
#else
void SetDefFan(const unsigned char tp)
{
    switch(tp)
    {
        case 0: //FIME
        {
            Kvnt2[0].dvnt=_DTVV0;
            Kvnt2[0].dpwm=_DTVP0;

            Kvnt2[1].dvnt=_DTVV1;
            Kvnt2[1].dpwm=_DTVP1;

            Kvnt2[2].dvnt=_DTVV2;
            Kvnt2[2].dpwm=_DTVP2;

            Kvnt2[3].dvnt=_DTVV3;
            Kvnt2[3].dpwm=_DTVP3;

            Kvnt2[4].dvnt=_DTVV4;
            Kvnt2[4].dpwm=_DTVP4;

            Kvnt2[5].dvnt=_DTVV5;
            Kvnt2[5].dpwm=_DTVP5;

            Kvnt2[6].dvnt=_DTVV6;
            Kvnt2[6].dpwm=_DTVP6;

            Kvnt2[7].dvnt=_DTVV7;
            Kvnt2[7].dpwm=_DTVP7;
            
            Kvnt2[8].dvnt=_DTVV8;
            Kvnt2[8].dpwm=_DTVP8;

            Kvnt2[9].dvnt=_DTVV9;
            Kvnt2[9].dpwm=_DTVP9;            
            
            /*Kvnt2[0].dvnt=977;
            Kvnt2[0].dpwm=139;

            Kvnt2[1].dvnt=1942;
            Kvnt2[1].dpwm=207;

            Kvnt2[2].dvnt=3681;
            Kvnt2[2].dpwm=342;

            Kvnt2[3].dvnt=4800;
            Kvnt2[3].dpwm=477;

            Kvnt2[4].dvnt=5556;
            Kvnt2[4].dpwm=612;

            Kvnt2[5].dvnt=6123;
            Kvnt2[5].dpwm=747;

            Kvnt2[6].dvnt=6593;
            Kvnt2[6].dpwm=882;

            Kvnt2[7].dvnt=7142;
            Kvnt2[7].dpwm=1017;*/            
            break;
        }
        case 1:
        {
            Kvnt2[0].dvnt=_DTVV0;
            Kvnt2[0].dpwm=_DTVP0;

            Kvnt2[1].dvnt=_DTVV1;
            Kvnt2[1].dpwm=_DTVP1;

            Kvnt2[2].dvnt=_DTVV2;
            Kvnt2[2].dpwm=_DTVP2;

            Kvnt2[3].dvnt=_DTVV3;
            Kvnt2[3].dpwm=_DTVP3;

            Kvnt2[4].dvnt=_DTVV4;
            Kvnt2[4].dpwm=_DTVP4;

            Kvnt2[5].dvnt=_DTVV5;
            Kvnt2[5].dpwm=_DTVP5;

            Kvnt2[6].dvnt=_DTVV6;
            Kvnt2[6].dpwm=_DTVP6;

            Kvnt2[7].dvnt=_DTVV7;
            Kvnt2[7].dpwm=_DTVP7;
            
            Kvnt2[8].dvnt=_DTVV8;
            Kvnt2[8].dpwm=_DTVP8;

            Kvnt2[9].dvnt=_DTVV9;
            Kvnt2[9].dpwm=_DTVP9;            
            
            /*Kvnt2[0].dvnt=800;
            Kvnt2[0].dpwm=132;

            Kvnt2[1].dvnt=3200;
            Kvnt2[1].dpwm=248;

            Kvnt2[2].dvnt=4400;
            Kvnt2[2].dpwm=336;

            Kvnt2[3].dvnt=6300;
            Kvnt2[3].dpwm=496;

            Kvnt2[4].dvnt=7500;
            Kvnt2[4].dpwm=620;

            Kvnt2[5].dvnt=8600;
            Kvnt2[5].dpwm=744;

            Kvnt2[6].dvnt=9600;
            Kvnt2[6].dpwm=868;

            Kvnt2[7].dvnt=10400;
            Kvnt2[7].dpwm=992;*/  
            break;
        }
        default:
        {
            Kvnt2[0].dvnt=_DTVV0;
            Kvnt2[0].dpwm=_DTVP0;

            Kvnt2[1].dvnt=_DTVV1;
            Kvnt2[1].dpwm=_DTVP1;

            Kvnt2[2].dvnt=_DTVV2;
            Kvnt2[2].dpwm=_DTVP2;

            Kvnt2[3].dvnt=_DTVV3;
            Kvnt2[3].dpwm=_DTVP3;

            Kvnt2[4].dvnt=_DTVV4;
            Kvnt2[4].dpwm=_DTVP4;

            Kvnt2[5].dvnt=_DTVV5;
            Kvnt2[5].dpwm=_DTVP5;

            Kvnt2[6].dvnt=_DTVV6;
            Kvnt2[6].dpwm=_DTVP6;

            Kvnt2[7].dvnt=_DTVV7;
            Kvnt2[7].dpwm=_DTVP7;
            
            Kvnt2[8].dvnt=_DTVV8;
            Kvnt2[8].dpwm=_DTVP8;

            Kvnt2[9].dvnt=_DTVV9;
            Kvnt2[9].dpwm=_DTVP9;            
            break;
        }
    }
    Write2EEVNT0(Kvnt2[0].dvnt,Kvnt2[0].dpwm);
    Write2EEVNT1(Kvnt2[1].dvnt,Kvnt2[1].dpwm);
    Write2EEVNT2(Kvnt2[2].dvnt,Kvnt2[2].dpwm);
    Write2EEVNT3(Kvnt2[3].dvnt,Kvnt2[3].dpwm);
    Write2EEVNT4(Kvnt2[4].dvnt,Kvnt2[4].dpwm);
    Write2EEVNT5(Kvnt2[5].dvnt,Kvnt2[5].dpwm);
    Write2EEVNT6(Kvnt2[6].dvnt,Kvnt2[6].dpwm);
    Write2EEVNT7(Kvnt2[7].dvnt,Kvnt2[7].dpwm);     
    Write2EEVNT8(Kvnt2[8].dvnt,Kvnt2[8].dpwm);
    Write2EEVNT9(Kvnt2[9].dvnt,Kvnt2[9].dpwm);
}
#endif
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//Kalibracja ukladu
//gdzie n-liczba krokow kalibracji
#if _HIRESVNT==0
unsigned char KalibracjaVNT(void)
{
	static unsigned char k=0,krok=0;
	//static float kpwm;
	volatile unsigned char i,d;
	unsigned int p=0;

//	nkpkt=8;
//	if(nkpkt>8) nkpkt=8;	//maksymalna liczba punktow kalibracji
	
    d=(InGetBit(RSDT1.inSTAT1,_KMINUS)&&InGetBit(RSDT1.inSTAT1,_KSET))?1:0;
	//if(!M._LCD||!InGetBit(RSDT1.inSTAT1,_KMINUS))	//brak plytki wyswietlacza lub przyciski MINUS i OFF w stanie spoczynku?
	if(!M._LCD||!d)	//brak plytki wyswietlacza lub przyciski MINUS i OFF w stanie spoczynku?        
	{
	 	StartRTS(_RTWPWM);
	}
	//decyzja o przerwaniu procedury kalibracji (tryb pracy ustawiony zostaje na bar)
	//decyzja o wyjsciu z procedury konfigracji wentylatora, odczyt domyslnych danych kalibracji z pamieci.
	//if(M._PW_START&&M._LCD&&RTS(_RTWPWM)>2) 	//aktywna plytka wyswietlacza i przytrzymano przycisk MINUS?
	if((M._PW_START&&M._LCD&&RTS(_RTWPWM)>2)||(M._PW_START&&M._LCD&&(M.STRB==2))) 	//aktywna plytka wyswietlacza i przytrzymano przycisk MINUS lub TRYB SPECJALNY?        
	{
		InBitSet(&RSDT1.outSTAT2,_KALIB,0);          //wyzeruj znacznik aktywnosci procedury kalibracji
		//---------------------------------
        SetDefFan(DtKNF.rfan);                      //zaladuj domyslne wartosci dla danego typu wentylatora
		ModParam();									//oblicz kluczowe parametry modulatora
		//---------------------------------
		//ustaw tryb pracy na OFF (pozycja neutralna dla przytrzymanego przycisku MINUS)
        //if(M.STRB!=2)
        {
            PFN._zima=0;
            PFN._lato=0;
            PFN._off=1;
            PFN._csn=0;
            PFN.poz=_PFOFF;
            InBitSet(&RSDT1.outSTAT2,_F_ZIMA,0);
            InBitSet(&RSDT1.outSTAT2,_F_LATO,0);
            InBitSet(&RSDT1.outSTAT2,_F_OFF,1);
            WriteEEPFN();
        }
		//---------------------------------
		k=0;
#if _DEMO==0
		_VNT_SetLow();										//wylacz wentylator
#else
        M.POB_VNT=0
#endif
		StartRTS(_RTRCI);
		return 2;									//wyjdz z procedury kalibracji								
	}

	switch(k)
	{
		case 0: //start procedury
		{
			WritePWM3(0);								
#if _DEMO==0
            _VNT_SetLow();										//wylacz wentylator
#else
            M.POB_VNT=0
#endif
			InBitSet(&RSDT1.outSTAT2,_KALIB,1);			//ustaw znacznik aktywnosci procedury kalibracji
			if(M._PW_START&&!_KNF_GetValue())						//zalaczono zasilanie i jest wyjeta zworka CF?
			{
				StartRTdS(_RTFKVN);
				k=1;
			}
			else
			{
				for(i=0;i<nkpkt;i++) {Kvnt[i].dvnt=0;Kvnt[i].dpwm=0;}
				StartRTdS(_RTFKVN);
				k=2;
			}
            ClrTachoDta();
			return 0;
		}
		case 1:	//oczekiwanie na ew. przerwanie procedury kalibracji celem wejscia do konfiguracji (dziala bez zworki KNF)
		{
			WritePWM3(0);								
#if _DEMO==0
            _VNT_SetLow();										//wylacz wentylator
#else
             M.POB_VNT=0
#endif
			if(!_KNF_GetValue()&&M._PW_START&&InGetBit(RSDT1.inSTAT1,_KSET)&&InGetBit(RSDT1.inSTAT1,_KOFF))				//ustawiona flaga aktywnosci procedury konfiguracji
			{
 				InBitSet(&RSDT1.outSTAT2,_KALIB,0);															//wyzeruj znacznik aktywnosci procedury kalibracji
				//---------------------------------
				//odczytaj zapamietane dane kalibracji
				OdczytDanychKalibracji();
				//---------------------------------
				//ustaw tryb pracy na OFF
				PFN._zima=0;
				PFN._lato=0;
				PFN._off=1;
				PFN._csn=0;
				PFN.poz=_PFOFF;
				InBitSet(&RSDT1.outSTAT2,_F_ZIMA,0);
				InBitSet(&RSDT1.outSTAT2,_F_LATO,0);
				InBitSet(&RSDT1.outSTAT2,_F_OFF,1);
				WriteEEPFN();
				k=0;
				return 1;									//wyjdz z procedury kalibracji								
			}
			if(!_KNF_GetValue()&&RTdS(_RTFKVN)<=30) return 0;			//oczekiwanie na ew. przerwanie procedury kalibracji
			for(i=0;i<nkpkt;i++) {Kvnt[i].dvnt=0;Kvnt[i].dpwm=0;}
			StartRTdS(_RTFKVN);
			k=2;
			return 0;
		}
		case 2:	//odczekaj az spadna obroty
		{
			WritePWM3(0);								
#if _DEMO==0
            _VNT_SetLow();										//wylacz wentylator
#else
            M.POB_VNT=0
#endif
			if(RTdS(_RTFKVN)>10&&M.rVVNT<500)					//obroty spadly ponizej 500/min
			{
				StartRTdS(_RTFKVN);

				krok=0;
				k=3;
				return 0;
			}
			if(RTdS(_RTFKVN)<=90) return 0;					//odczekaj az spadna obroty		
			StartRTdS(_RTFKVN);

			krok=0;
			k=3;
			return 0;
		}
        case 3:     //wstepne pobudzenie wentylatora do obrotu
        {
			WritePWM3(PWM.VLO_PWM+((PWM.HIPWM3-PWM.VLO_PWM)/8));	
#if _DEMO==0
			_VNT_SetHigh(); 
#else
            M.POB_VNT=1;
#endif
			if(RTdS(_RTFKVN)<=10) return 0;					//odczekaj az spadna obroty  
			StartRTdS(_RTFKVN);

			krok=0;
			k=4;
			return 0;            
        } 
        case 4:     //zejdz do minimum
        {
			WritePWM3(PWM.VLO_PWM);								
			if(RTdS(_RTFKVN)<=10) return 0;					//odczekaj az spadna obroty  
			StartRTdS(_RTFKVN);

			krok=0;
			k=5;
			return 0;            
        }         
		case 5: //uruchomienie wentylatora, zapamietanie wartosci kluczowych
		{
#if _DEMO==0
			_VNT_SetHigh(); 
#else
            M.POB_VNT=1;
#endif
			PMP.APROC=krok*99/(nkpkt-1);				//test pompy
			WritePWM2(ProcToPmpPwm(99-PMP.APROC));		//test pompy
			if(SetKalPWM(krok,PWM.VLO_PWM+krok*((PWM.HIPWM3-PWM.VLO_PWM)/(nkpkt-1))))
			{
				krok++;
				if (krok>=nkpkt) k=6;
			}
			return 0;
		}
		case 6:
		{
			//Kontrola poprawnosci pracy wentylatora
			for(i=0;i<nkpkt;i++) 
			{
				p+=Kvnt[i].dvnt;
				if(i>1&&!Kvnt[i].dvnt)
				{
					p=0;
					break;
				}
			}
			if(p)	//Wentylator pracuje poprawnie
			{
				InBitSet(&RSDT1.outSTAT2,_KALIB,0);			//wyzeruj znacznik aktywnosci procedury kalibracji

				WriteEEVNT0(Kvnt[0].dvnt,Kvnt[0].dpwm);
				WriteEEVNT1(Kvnt[1].dvnt,Kvnt[1].dpwm);
				WriteEEVNT2(Kvnt[2].dvnt,Kvnt[2].dpwm);
				WriteEEVNT3(Kvnt[3].dvnt,Kvnt[3].dpwm);
				WriteEEVNT4(Kvnt[4].dvnt,Kvnt[4].dpwm);
				WriteEEVNT5(Kvnt[5].dvnt,Kvnt[5].dpwm);
				WriteEEVNT6(Kvnt[6].dvnt,Kvnt[6].dpwm);
				WriteEEVNT7(Kvnt[7].dvnt,Kvnt[7].dpwm);
				ModParam();									//oblicz kluczowe parametry modulatora
			}
			else	//Stwierdzono awarie wentylatora
			{
				InBitSet(&RSDT1.outSTAT2,_KALIB,0);			//wyzeruj znacznik aktywnosci procedury kalibracji
				M.ERR_BTY=0x07;
				PrintErr(0x07,1);
                PomALL();                                   //wykonaj zalegle pomiary na potrzeby zapisu statusu wylaczenia awaryjnego z blokada                
                ToWriteESTAT();
				k=0;
				ErrPTG();
			}
			k=7;
			return 0;
		}
		case 7:
		{
			M._VWYBIEG=1;					//uaktywnij wybieg wentylatora (zapobieganie szybkiemu wylaczeniu i zalaczeniu wentylatora przy aktywnych pobudzeniach)
			k=0;
			return 1;
		}        
	}
	return 1;
}
#else
unsigned char KalibracjaVNT(void)
{
	static unsigned char k=0,krok=0;
	//static float kpwm;
	volatile unsigned char i,d;
	unsigned int p=0,wp,mn;
    float zr;

	
    d=(InGetBit(RSDT1.inSTAT1,_KMINUS)&&InGetBit(RSDT1.inSTAT1,_KSET))?1:0;
	//if(!M._LCD||!InGetBit(RSDT1.inSTAT1,_KMINUS))	//brak plytki wyswietlacza lub przyciski MINUS i OFF w stanie spoczynku?
	if(!M._LCD||!d)	//brak plytki wyswietlacza lub przyciski MINUS i OFF w stanie spoczynku?        
	{
	 	StartRTS(_RTWPWM);
	}
	//decyzja o przerwaniu procedury kalibracji (tryb pracy ustawiony zostaje na bar)
	//decyzja o wyjsciu z procedury konfigracji wentylatora, odczyt domyslnych danych kalibracji z pamieci.
	//if(M._PW_START&&M._LCD&&RTS(_RTWPWM)>2) 	//aktywna plytka wyswietlacza i przytrzymano przycisk MINUS?
#if _DEMO==0    
	if((M._PW_START&&M._LCD&&RTS(_RTWPWM)>2)||(M._PW_START&&M._LCD&&(M.STRB==2))) 	//aktywna plytka wyswietlacza i przytrzymano przycisk MINUS lub TRYB SPECJALNY?        
	{
		InBitSet(&RSDT1.outSTAT2,_KALIB,0);          //wyzeruj znacznik aktywnosci procedury kalibracji
		//---------------------------------
        SetDefFan(DtKNF.rfan);                      //zaladuj domyslne wartosci dla danego typu wentylatora
		ModParam();									//oblicz kluczowe parametry modulatora
		//---------------------------------
		//ustaw tryb pracy na OFF (pozycja neutralna dla przytrzymanego przycisku MINUS)
        //if(M.STRB!=2)
        {
            PFN._zima=0;
            PFN._lato=0;
            PFN._off=1;
            PFN._csn=0;
            PFN.poz=_PFOFF;
            InBitSet(&RSDT1.outSTAT2,_F_ZIMA,0);
            InBitSet(&RSDT1.outSTAT2,_F_LATO,0);
            InBitSet(&RSDT1.outSTAT2,_F_OFF,1);
            WriteEEPFN();
        }
		//---------------------------------
		k=0;
		_VNT_SetLow();										//wylacz wentylator
		StartRTS(_RTRCI);
		return 2;									//wyjdz z procedury kalibracji								
	}
#else
    {
		InBitSet(&RSDT1.outSTAT2,_KALIB,0);          //wyzeruj znacznik aktywnosci procedury kalibracji
		//---------------------------------
		OdczytDanychKalibracji();
		//---------------------------------
		//---------------------------------
		k=0;
		M.POB_VNT=0;										//wylacz wentylator
		StartRTS(_RTRCI);
		return 1;									//wyjdz z procedury kalibracji        
    }
#endif
    mn=(PWM.VLO_PWM<<2);
	switch(k)
	{
		case 0: //start procedury
		{
			WritePWM3H(0);								
#if _DEMO==0
            _VNT_SetLow();										//wylacz wentylator
#else
            M.POB_VNT=0;
#endif
			InBitSet(&RSDT1.outSTAT2,_KALIB,1);			//ustaw znacznik aktywnosci procedury kalibracji
			if(M._PW_START&&!_KNF_GetValue())						//zalaczono zasilanie i jest wyjeta zworka CF?
			{
				StartRTdS(_RTFKVN);
				k=1;
			}
			else
			{
				for(i=0;i<nkpkt;i++) {Kvnt2[i].dvnt=0;Kvnt2[i].dpwm=0;}
				StartRTdS(_RTFKVN);
				k=2;
			}
            ClrTachoDta();
			return 0;
		}
		case 1:	//oczekiwanie na ew. przerwanie procedury kalibracji celem wejscia do konfiguracji (dziala bez zworki KNF)
		{
			WritePWM3H(0);								
#if _DEMO==0
            _VNT_SetLow();										//wylacz wentylator
#else
            M.POB_VNT=0;
#endif
			if(!_KNF_GetValue()&&M._PW_START&&InGetBit(RSDT1.inSTAT1,_KSET)&&InGetBit(RSDT1.inSTAT1,_KOFF))				//ustawiona flaga aktywnosci procedury konfiguracji
			{
 				InBitSet(&RSDT1.outSTAT2,_KALIB,0);															//wyzeruj znacznik aktywnosci procedury kalibracji
				//---------------------------------
				//odczytaj zapamietane dane kalibracji
				OdczytDanychKalibracji();
				//---------------------------------
				//ustaw tryb pracy na OFF
				PFN._zima=0;
				PFN._lato=0;
				PFN._off=1;
				PFN._csn=0;
				PFN.poz=_PFOFF;
				InBitSet(&RSDT1.outSTAT2,_F_ZIMA,0);
				InBitSet(&RSDT1.outSTAT2,_F_LATO,0);
				InBitSet(&RSDT1.outSTAT2,_F_OFF,1);
				WriteEEPFN();
				k=0;
				return 1;									//wyjdz z procedury kalibracji								
			}
			if(!_KNF_GetValue()&&RTdS(_RTFKVN)<=30) return 0;			//oczekiwanie na ew. przerwanie procedury kalibracji
			for(i=0;i<nkpkt;i++) {Kvnt2[i].dvnt=0;Kvnt2[i].dpwm=0;}
			StartRTdS(_RTFKVN);
			k=2;
			return 0;
		}
		case 2:	//odczekaj az spadna obroty
		{
			WritePWM3H(0);								
#if _DEMO==0
            _VNT_SetLow();										//wylacz wentylator
#else
            M.POB_VNT=0;
#endif
			if(RTdS(_RTFKVN)>10&&M.rVVNT<500)					//obroty spadly ponizej 500/min
			{
				StartRTdS(_RTFKVN);

				krok=0;
				k=3;
				return 0;
			}
			if(RTdS(_RTFKVN)<=90) return 0;					//odczekaj az spadna obroty		
			StartRTdS(_RTFKVN);

			krok=0;
			k=3;
			return 0;
		}
        case 3:     //wstepne pobudzenie wentylatora do obrotu
        {
			WritePWM3H(mn+((PWM.HIPWM3-mn)/8));								
#if _DEMO==0
			_VNT_SetHigh(); 
#else
            M.POB_VNT=1;
#endif 
			if(RTdS(_RTFKVN)<=10) return 0;					//odczekaj az spadna obroty  
			StartRTdS(_RTFKVN);

			krok=0;
			k=4;
			return 0;            
        } 
        case 4:     //zejdz do minimum
        {
			WritePWM3H(mn);								
			if(RTdS(_RTFKVN)<=10) return 0;					//odczekaj az spadna obroty  
			StartRTdS(_RTFKVN);

			krok=0;
			k=5;
            //***************************88
            /*WritePWM3H(0);								
			if(RTdS(_RTFKVN)<=30) return 0;					//odczekaj az spadna obroty  
			StartRTdS(_RTFKVN);

			krok=0;
			k=5;
            SetKalVNT(1,0,0);*/
            //******************************
			return 0;            
        }         
		case 5: //uruchomienie wentylatora, zapamietanie wartosci kluczowych
		{
#if _DEMO==0
			_VNT_SetHigh(); 
#else
            M.POB_VNT=1;
#endif
			PMP.APROC=krok*99/(nkpkt-1);				//test pompy
			WritePWM2(ProcToPmpPwm(99-PMP.APROC));		//test pompy
            //------------------
            zr=((float)(PWM.HIPWM3-mn)/(nkpkt-3));      //podzial calego przedzialu regulacji na rowne zakresy
            if(krok>=3)                                 //kolejne kroki poczynajac od trzeciego rowne sa zakresom
            {
                wp=(unsigned int)(mn+zr+(krok-3)*zr);
            }
            else                                        //pierwsze trzy kroki sa w obrebie pierwszego zakresu (wieksza dokladnosc)
            {
                switch(krok)
                {
                    case 0:
                    {
                        wp=(unsigned int)mn;                //dolna wartosc graniczna rozruchu
                        //wp=(unsigned int)(mn+zr/10.0);     
                        break;
                    }
                    case 1:
                    {
                        wp=(unsigned int)(mn+zr/3.0);       //1/3 zakresu pierwszego
                        break;
                    }   
                    case 2:
                    {
                        wp=(unsigned int)(mn+(2*zr/3.0));   //2/3 zakresu pierwszego
                        break;
                    }                    
                }
            }
            
            /*if(krok>=2)
            {
                wp=(unsigned int)(mn+zr+(krok-2)*zr);
            }
            else
            {
                if(!krok)
                {
                    wp=(unsigned int)(mn+zr/3.0);       //1/3
                }
                else
                {
                    wp=(unsigned int)(mn+(2*zr/3.0));   //2/3
                }
            }*/
            
            /*if(krok)
            {
                wp=mn+krok*((PWM.HIPWM3-mn)/(nkpkt-1));
            }
            else
            {
                wp=mn+(((PWM.HIPWM3-mn)/(nkpkt-1)))/3;
            }*/
			if(SetKalPWM(krok,wp))
			{
				krok++;
				if (krok>=nkpkt) k=6;
			}
            //********************************88
            /*wp=15+((56-15)/8)*krok;
            if(!wp) 
            {
                wp=(56-15)/16;
            }
            if(SetKalVNT(0,krok,wp))
            {
                krok++;
                if (krok>=nkpkt) k=6;
            }*/
            //**********************************
			return 0;
		}
		case 6:
		{
			//Kontrola poprawnosci pracy wentylatora
			for(i=0;i<nkpkt;i++) 
			{
				p+=Kvnt2[i].dvnt;
				if(i>1&&!Kvnt2[i].dvnt)
				{
					p=0;
					break;
				}
			}
			if(p)	//Wentylator pracuje poprawnie
			{
				InBitSet(&RSDT1.outSTAT2,_KALIB,0);			//wyzeruj znacznik aktywnosci procedury kalibracji

				Write2EEVNT0(Kvnt2[0].dvnt,Kvnt2[0].dpwm);
				Write2EEVNT1(Kvnt2[1].dvnt,Kvnt2[1].dpwm);
				Write2EEVNT2(Kvnt2[2].dvnt,Kvnt2[2].dpwm);
				Write2EEVNT3(Kvnt2[3].dvnt,Kvnt2[3].dpwm);
				Write2EEVNT4(Kvnt2[4].dvnt,Kvnt2[4].dpwm);
				Write2EEVNT5(Kvnt2[5].dvnt,Kvnt2[5].dpwm);
				Write2EEVNT6(Kvnt2[6].dvnt,Kvnt2[6].dpwm);
				Write2EEVNT7(Kvnt2[7].dvnt,Kvnt2[7].dpwm);
                Write2EEVNT8(Kvnt2[8].dvnt,Kvnt2[8].dpwm);
                Write2EEVNT9(Kvnt2[9].dvnt,Kvnt2[9].dpwm);                
				ModParam();									//oblicz kluczowe parametry modulatora
			}
			else	//Stwierdzono awarie wentylatora
			{
				InBitSet(&RSDT1.outSTAT2,_KALIB,0);			//wyzeruj znacznik aktywnosci procedury kalibracji
				M.ERR_BTY=0x07;
				PrintErr(0x07,1);
                PomALL();                                   //wykonaj zalegle pomiary na potrzeby zapisu statusu wylaczenia awaryjnego z blokada                
                ToWriteESTAT();
				k=0;
				ErrPTG();
			}
			k=7;
			return 0;
		}
		case 7:
		{
			M._VWYBIEG=1;					//uaktywnij wybieg wentylatora (zapobieganie szybkiemu wylaczeniu i zalaczeniu wentylatora przy aktywnych pobudzeniach)
			k=0;
			return 1;
		}        
	}
	return 1;
}
#endif
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//Odczytuje zapamietane w EEPROM dane kalibracji
#if _HIRESVNT==0
void OdczytDanychKalibracji()
{
//	nkpkt=8;
	Kvnt[0].dvnt=ReadEEVNT0_V();
	Kvnt[0].dpwm=ReadEEVNT0_P();
	Kvnt[1].dvnt=ReadEEVNT1_V();
	Kvnt[1].dpwm=ReadEEVNT1_P();
	Kvnt[2].dvnt=ReadEEVNT2_V();
	Kvnt[2].dpwm=ReadEEVNT2_P();
	Kvnt[3].dvnt=ReadEEVNT3_V();
	Kvnt[3].dpwm=ReadEEVNT3_P();
	Kvnt[4].dvnt=ReadEEVNT4_V();
	Kvnt[4].dpwm=ReadEEVNT4_P();
	Kvnt[5].dvnt=ReadEEVNT5_V();
	Kvnt[5].dpwm=ReadEEVNT5_P();
	Kvnt[6].dvnt=ReadEEVNT6_V();
	Kvnt[6].dpwm=ReadEEVNT6_P();
	Kvnt[7].dvnt=ReadEEVNT7_V();
	Kvnt[7].dpwm=ReadEEVNT7_P();
	ModParam();								//odczytaj parametry poczatkowe modulatora
}
#else
void OdczytDanychKalibracji()
{
//	nkpkt=8;
	Kvnt2[0].dvnt=Read2EEVNT0_V();
	Kvnt2[0].dpwm=Read2EEVNT0_P();
	Kvnt2[1].dvnt=Read2EEVNT1_V();
	Kvnt2[1].dpwm=Read2EEVNT1_P();
	Kvnt2[2].dvnt=Read2EEVNT2_V();
	Kvnt2[2].dpwm=Read2EEVNT2_P();
	Kvnt2[3].dvnt=Read2EEVNT3_V();
	Kvnt2[3].dpwm=Read2EEVNT3_P();
	Kvnt2[4].dvnt=Read2EEVNT4_V();
	Kvnt2[4].dpwm=Read2EEVNT4_P();
	Kvnt2[5].dvnt=Read2EEVNT5_V();
	Kvnt2[5].dpwm=Read2EEVNT5_P();
	Kvnt2[6].dvnt=Read2EEVNT6_V();
	Kvnt2[6].dpwm=Read2EEVNT6_P();
	Kvnt2[7].dvnt=Read2EEVNT7_V();
	Kvnt2[7].dpwm=Read2EEVNT7_P();    
    Kvnt2[8].dvnt=Read2EEVNT8_V();
	Kvnt2[8].dpwm=Read2EEVNT8_P();
	Kvnt2[9].dvnt=Read2EEVNT9_V();
	Kvnt2[9].dpwm=Read2EEVNT9_P();

	ModParam();								//odczytaj parametry poczatkowe modulatora
}
#endif
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//Zwraca aproksymowana wartosc PWM dla zadanej wartosci predkosci obrotowej DVNT
//gdzie:
//dvnt - oczekiwana wartosc obrotow
//nkpkt - liczna punktow tablicy kalibracji
#if _HIRESVNT==0
unsigned char DVNTtoPWM(unsigned char dvnt)
{
	unsigned char i,v1,v2,p1,p2,dv;
	signed long int pw;
	signed long int wsp=0;
	
	dv=dvnt;
	for(i=0;i<nkpkt;i++)
	{
		v2=Kvnt[i].dvnt;
		p2=Kvnt[i].dpwm;
		if(i!=0)
		{
 			v1=Kvnt[i-1].dvnt;
			p1=Kvnt[i-1].dpwm;
		}
		else
		{		
 			v1=0;
			p1=0;
		}
		if((v2>dv&&v1<dv)||v2==dv)
		{
            if(v2==v1) break;
			wsp=(signed long int)(p2-p1)*1000;
			wsp/=(v2-v1); //wspolczynnik kierunkowy prostej
			pw=(signed long int)(dv-v1)*wsp;
			pw/=1000;
			
			pw+=p1; 				//y=[(y1-y0)/(x1-x0)]*(x-x1)+y0
			if(pw<0) pw=0;
			if(pw>PWM.HIPWM3) pw=PWM.HIPWM3;	//graniczna gorna wartosc wypelnienia
			if(pw<PWM.VLO_PWM) pw=PWM.VLO_PWM;	//graniczna dolna wartosc wypelnienia (wentylator jeszcze pracuje)
			return (unsigned char) pw;
		}
	}
	return p2;	//maksymalne wypelnienie dla predkosci poza zakresem
}
#else
unsigned int DVNTtoPWM(unsigned int dvnt)
{
    unsigned char i;
    unsigned int mn;
	float wsp,pw,v1,v2,p1,p2,dv;
	
	dv=(float)dvnt;
    mn=(PWM.VLO_PWM<<2);
	for(i=0;i<nkpkt;i++)
	{
		v2=(float)Kvnt2[i].dvnt;
		p2=(float)Kvnt2[i].dpwm;
		if(i!=0)
		{
 			v1=(float)Kvnt2[i-1].dvnt;
			p1=(float)Kvnt2[i-1].dpwm;
		}
		else
		{		
 			v1=0.0;
			p1=0.0;
		}
		if((v2>dv&&v1<dv)||v2==dv)
		{
            if(v2==v1) break;
			wsp=(p2-p1)/(v2-v1);    //wspolczynnik kierunkowy prostej
			pw=(dv-v1)*wsp;
			pw+=p1; 				//y=[(y1-y0)/(x1-x0)]*(x-x1)+y0
			if(pw<0.0) pw=0.0;
			if(pw>PWM.HIPWM3) pw=PWM.HIPWM3;	//graniczna gorna wartosc wypelnienia
			if(pw<mn) pw=mn;	//graniczna dolna wartosc wypelnienia (wentylator jeszcze pracuje)
			return (unsigned int) pw;
		}
	}
	return (unsigned int)p2;	//maksymalne wypelnienie dla predkosci poza zakresem
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//Zwraca aproksymowana wartosc VNT dla zadanej wartosci PWM
unsigned char DPWMtoVNT(unsigned char dpwm)
{
	unsigned char i,v1,v2,p1,p2,dv;
	signed long int pw;
	signed long int wsp=0;
	
	dv=dpwm;
	for(i=0;i<nkpkt;i++)
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
			if(pw<10) pw=10;	//graniczna dolna wartosc wypelnienia (wentylator jeszcze pracuje)
			return (unsigned char) pw;
		}
	}
	return p2;	//maksymalne wypelnienie dla predkosci poza zakresem
}
#endif
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//Zwraca aproksymowana wartosc PWM dla zadanej wartosci predkosci obrotowej DVNT
//z poprawka pop dla wyniku
//gdzie:
//dvnt - oczekiwana wartosc obrotow
//pop - poprawka pop dla wyniku przy niskich predkosciach dvnt
#if _HIRESVNT==0
unsigned char DVNTtoPopPWM(unsigned char dvnt,const unsigned char pop)
{
	unsigned char i,v1,v2,p1,p2,dv;
	signed long int pw;
	signed long int wsp=0;
	
	dv=dvnt;
	for(i=0;i<nkpkt;i++)
	{
		v2=Kvnt[i].dvnt;
		p2=Kvnt[i].dpwm;
		if(i!=0)
		{
 			v1=Kvnt[i-1].dvnt;
			p1=Kvnt[i-1].dpwm;
		}
		else
		{		
 			v1=0;
			p1=0;
		}
		if((v2>dv&&v1<dv)||v2==dv)
		{
            if(v2==v1) break;
			wsp=(signed long int)(p2-p1)*1000;
			wsp/=(v2-v1); //wspolczynnik kierunkowy prostej
			pw=(signed long int)(dv-v1)*wsp;
			pw/=1000;
			
			pw+=p1; 				//y=[(y1-y0)/(x1-x0)]*(x-x1)+y0
			if(pw<0) pw=0;
			if(pw>PWM.HIPWM3) pw=PWM.HIPWM3;	//graniczna gorna wartosc wypelnienia
			if(pw<PWM.VLO_PWM) pw=PWM.VLO_PWM;	//graniczna dolna wartosc wypelnienia (wentylator jeszcze pracuje)
			if(i>2||!pop)
			{
				return (unsigned char) pw;
			}
			else
			{
				if((pw+pop)<=PWM.HIPWM3)
				{
					return ((unsigned char)pw+pop);
				}
				else
				{
					return PWM.HIPWM3;
				}
			}
		}
	}
	return p2;	//maksymalne wypelnienie dla predkosci poza zakresem
}
#else
unsigned int DVNTtoPopPWM(unsigned int dvnt,const unsigned char pop)
{
	unsigned char i;
    unsigned int mn;
    float wsp,pw,v1,v2,p1,p2,dv;
    
#if _DEMO==0	
	dv=(float)dvnt;
    mn=(PWM.VLO_PWM<<2);
	for(i=0;i<nkpkt;i++)
	{
		v2=(float)Kvnt2[i].dvnt;
		p2=(float)Kvnt2[i].dpwm;
		if(i!=0)
		{
 			v1=(float)Kvnt2[i-1].dvnt;
			p1=(float)Kvnt2[i-1].dpwm;
		}
		else
		{		
 			v1=0.0;
			p1=0.0;
		}
		if((v2>dv&&v1<dv)||v2==dv)
		{
            if(v2==v1) break;
			wsp=(p2-p1)/(v2-v1);    //wspolczynnik kierunkowy prostej
			pw=(dv-v1)*wsp;
			pw+=p1; 				//y=[(y1-y0)/(x1-x0)]*(x-x1)+y0
			if(pw<0.0) pw=0.0;
			if(pw>PWM.HIPWM3) pw=PWM.HIPWM3;	//graniczna gorna wartosc wypelnienia
			if(pw<mn) pw=mn;	//graniczna dolna wartosc wypelnienia (wentylator jeszcze pracuje)
			if(i>2||!pop)
			{
				return (unsigned int) pw;
			}
			else
			{
				if((pw+pop)<=PWM.HIPWM3)
				{
					return (unsigned int)(pw+pop);
				}
				else
				{
					return PWM.HIPWM3;
				}
			}
		}
	}
	return (unsigned int)p2;	//maksymalne wypelnienie dla predkosci poza zakresem
#else
    return DVNTtoPWM(dvnt);
#endif
}
#endif
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//Przelicza oczekiwana wartosc predkosci obrotowej na wypelnienie pwm
//gdzie nst-oczekiwana wartosc
#if _HIRESVNT==0
unsigned char CalkVNTtoPWM(unsigned char nst)
{
	unsigned char p;
	p=DVNTtoPWM(nst);
	return p;
}
#else
unsigned char CalkVNTtoPWM(unsigned char nst)
{
	unsigned int p;
	p=DVNTtoPWM(nst*100);
    if(p>PWM.HIPWM3) p=PWM.HIPWM3;
	return (p>>2);
}
unsigned int CalkVNTtoPWMH(unsigned int nst)
{
	unsigned int p;
	p=DVNTtoPWM(nst);
	return p;
}
#endif
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//Przelicza oczekiwana wartosc predkosci obrotowej na wypelnienie pwm z poprawka dla niskich predkosci
//gdzie nst-oczekiwana wartosc
//pop-wielkosc poprawki dla pwm
#if (_HIRESVNT==0)||(_PID_VNT==0)
unsigned char CalkVNTtoPopPWM(unsigned char nst,unsigned char pop)
{
	unsigned char p;
	p=DVNTtoPopPWM(nst,pop);
	return p;
}
#else
unsigned int CalkVNTtoPopPWMH(unsigned char nst,unsigned char pop)
{
	unsigned int p;
	p=DVNTtoPopPWM(nst*100,pop);
	return p;
}
#endif
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Przelicza wartosc procentowa na predkosc obrotowa
unsigned char ProcToVNT(unsigned char wrt)
{
	unsigned long tmp;
	if(wrt>99) wrt=99;
	wrt=wrt+1;
	tmp=((DtKNF.mmax-DtKNF.mmin)*100)/100;
	return (DtKNF.mmin+(unsigned char)((wrt*tmp)/100));	
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Przelicza predkosc obrotowa na wartosc procentowa 
unsigned char VNTToProc(unsigned char wrt)
{
	unsigned long tmp;
	if(wrt<DtKNF.mmin) return 0;
	if(wrt>DtKNF.mmax) return 99;
    if(DtKNF.mmin==DtKNF.mmax) return 99;
	tmp=(wrt-DtKNF.mmin)*100;
	tmp/=(DtKNF.mmax-DtKNF.mmin);
	if(tmp>=99) return 99;
	return (unsigned char)tmp;	
}
/*----------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Zwraca wartosc zwloki czasowe dla wybiegu maksymalnego w proc. rozruchu
//gdzie vstr = predkosc went. dla mocy startowej
unsigned char CalkTimStr(unsigned char vstr)
{
	unsigned char mint,maxt,minv,maxv;
	unsigned long int w;
	mint=6;				//minimum dla zwloki czasowej
	maxt=10;			//15maksimum dla zwloki czasowej

	minv=28;			//wartosc pred. went. dla ktorej zwloka czasowa = min
	maxv=50;			//wartosc pred. went. dla ktorej zwloka czasowa = max

    if(maxv==minv) return maxt;
	if(vstr<minv) vstr=minv;
	if(vstr>maxv) vstr=maxv;

	w=(unsigned long int)((maxt-mint)*(vstr-minv)*100); //(10-6)*(50-28)*100=4*22*100=8800
	w/=(maxv-minv);//8800/(50-28)=8800/22=400
	w/=100;//400/100=4
	w+=mint;//4+6=10 [dS] - maksymalny czas wybiegu do osiagniecia mocy startowej
	return (unsigned char) w;	//proponowana wartosc zwloki czasewj dla wyp.maks.
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
#endif


