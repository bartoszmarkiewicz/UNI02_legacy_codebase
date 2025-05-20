/*vntregtch.c*/
//Druga poprawiona wersja oprogramowania
//Zwiekszona precyzja pomiaru - wynik typu int
/*---------------------------------------------------------------------------------*/
#include <pic18.h>
#include <math.h>
#include "global.h"				//parametry globalne
#include "vntregtch.h"			//parametry lokalne
#include "main.tp.h"
#include "pomiar.tp.h"			
#include "konfig.tp.h"
#include "pompa.tp.h"		//typy danych
#include "pwm.h"
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

KALVnt Kvnt[10];
const unsigned char nkpkt=8;

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
unsigned char DVNTtoPWM(unsigned char dvnt);
unsigned char ModToDVNT(unsigned char dvnt,unsigned char minvnt,unsigned char maxvnt,unsigned char pw);
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------

//Krok kalibracji uk³adu
//gdzie nkpkt-liczba krokow kalibracji
//krk-aktualny krok kalibracji
//pwm-zadane wypelnienie pwm
/*unsigned char SetKalPWM(unsigned char krk,unsigned char pwm)
{
	unsigned char tim;

	tim=35+krk*(40/nkpkt);			//kalibracja kroku czasowego w zaleznosci od liczby punktów kalibracyjnych

	if(RTdS(_RTFKVN)<tim)			//wybieg na zadanej mocy
	{
		WritePWM(pwm);
		return 0;
	}
	else							//zapamietanie wartosci kluczowych
	{
		StartRTdS(_RTFKVN);
		Kvnt[krk].dvnt=(unsigned char) (round(M.rVVNT/100.0));		//odczytana predkosc obrotowa
		Kvnt[krk].dpwm=pwm;//PWM.BufPWM;	//aktualna wartosc PWM
	}
	return 1;
}*/
#define _PPPWM 2
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
		WritePWM(pwm);
		return 0;
	}
	else							//zapamietanie wartosci kluczowych
	{
		StartRTdS(_RTFKVN);
		Kvnt[krk].dvnt=(unsigned char) (round(M.rVVNT/100.0));		//odczytana predkosc obrotowa
        switch(DtKNF.rfan)  //rodzaj wentylatora
        {
            case 0: //FIME
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
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//ustaw domyslne dane kalibracji na potrzeby odbioru produkcyjnego
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
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//Kalibracja uk³adu
//gdzie n-liczba krokow kalibracji
unsigned char KalibracjaVNT(void)
{
	static unsigned char k=0,krok=0;
	//static float kpwm;
	volatile unsigned char i,d;
	unsigned int p=0;

//	nkpkt=8;
//	if(nkpkt>8) nkpkt=8;	//maksymalna liczba punktow kalibracji
	
    d=(GetBit2(RSDT1.inSTAT1,_KMINUS)&&GetBit2(RSDT1.inSTAT1,_KSET))?1:0;
	//if(!M._LCD||!GetBit2(RSDT1.inSTAT1,_KMINUS))	//brak plytki wyswietlacza lub przyciski MINUS i OFF w stanie spoczynku?
	if(!M._LCD||!d)	//brak plytki wyswietlacza lub przyciski MINUS i OFF w stanie spoczynku?        
	{
	 	StartRTS(_RTWPWM);
	}
	//decyzja o przerwaniu procedury kalibracji (tryb pracy ustawiony zostaje na bar)
	//decyzja o wyjsciu z procedury konfigracji wentylatora, odczyt domyslnych danych kalibracji z pamieci.
	//if(M._PW_START&&M._LCD&&RTS(_RTWPWM)>2) 	//aktywna plytka wyswietlacza i przytrzymano przycisk MINUS?
	if((M._PW_START&&M._LCD&&RTS(_RTWPWM)>2)||(M._PW_START&&M._LCD&&(M.STRB==2))) 	//aktywna plytka wyswietlacza i przytrzymano przycisk MINUS lub TRYB SPECJALNY?        
	{
		BitSet2(&RSDT1.outSTAT2,_KALIB,0);          //wyzeruj znacznik aktywnosci procedury kalibracji
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
            BitSet2(&RSDT1.outSTAT2,_F_ZIMA,0);
            BitSet2(&RSDT1.outSTAT2,_F_LATO,0);
            BitSet2(&RSDT1.outSTAT2,_F_OFF,1);
            WriteEEPFN();
        }
		//---------------------------------
		k=0;
		_VNT=0;										//wylacz wentylator
		StartRTS(_RTRCI);
		return 2;									//wyjdz z procedury kalibracji								
	}

	switch(k)
	{
		case 0: //start procedury
		{
			WritePWM(0);								
			_VNT=0;
			BitSet2(&RSDT1.outSTAT2,_KALIB,1);			//ustaw znacznik aktywnosci procedury kalibracji
			if(M._PW_START&&!_KNF)						//zalaczono zasilanie i jest wyjeta zworka CF?
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
			WritePWM(0);								
			_VNT=0;
			if(!_KNF&&M._PW_START&&GetBit2(RSDT1.inSTAT1,_KSET)&&GetBit2(RSDT1.inSTAT1,_KOFF))				//ustawiona flaga aktywnosci procedury konfiguracji
			{
 				BitSet2(&RSDT1.outSTAT2,_KALIB,0);															//wyzeruj znacznik aktywnosci procedury kalibracji
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
				BitSet2(&RSDT1.outSTAT2,_F_ZIMA,0);
				BitSet2(&RSDT1.outSTAT2,_F_LATO,0);
				BitSet2(&RSDT1.outSTAT2,_F_OFF,1);
				WriteEEPFN();
				k=0;
				return 1;									//wyjdz z procedury kalibracji								
			}
			if(!_KNF&&RTdS(_RTFKVN)<=30) return 0;			//oczekiwanie na ew. przerwanie procedury kalibracji
			for(i=0;i<nkpkt;i++) {Kvnt[i].dvnt=0;Kvnt[i].dpwm=0;}
			StartRTdS(_RTFKVN);
			k=2;
			return 0;
		}
		case 2:	//odczekaj az spadna obroty
		{
			WritePWM(0);								
			_VNT=0;
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
			WritePWM(PWM.VLO_PWM+((PWM.HIPWM-PWM.VLO_PWM)/8));								
			_VNT=1; 
			if(RTdS(_RTFKVN)<=10) return 0;					//odczekaj az spadna obroty  
			StartRTdS(_RTFKVN);

			krok=0;
			k=4;
			return 0;            
        } 
        case 4:     //zejdz do minimum
        {
			WritePWM(PWM.VLO_PWM);								
			if(RTdS(_RTFKVN)<=10) return 0;					//odczekaj az spadna obroty  
			StartRTdS(_RTFKVN);

			krok=0;
			k=5;
			return 0;            
        }         
		case 5: //uruchomienie wentylatora, zapamietanie wartosci kluczowych
		{
			_VNT=1;
			PMP.APROC=krok*99/(nkpkt-1);				//test pompy
			WritePWM2(ProcToPmpPwm(99-PMP.APROC));		//test pompy
			if(SetKalPWM(krok,PWM.VLO_PWM+krok*((PWM.HIPWM-PWM.VLO_PWM)/(nkpkt-1))))
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
				BitSet2(&RSDT1.outSTAT2,_KALIB,0);			//wyzeruj znacznik aktywnosci procedury kalibracji

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
				BitSet2(&RSDT1.outSTAT2,_KALIB,0);			//wyzeruj znacznik aktywnosci procedury kalibracji
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
        
        /*case 3: //uruchomienie wentylatora, zapamietanie wartosci kluczowych
		{
			_VNT=1;
			PMP.APROC=krok*99/(nkpkt-1);				//test pompy
			WritePWM2(ProcToPmpPwm(99-PMP.APROC));		//test pompy
			if(SetKalPWM(krok,PWM.VLO_PWM+krok*((PWM.HIPWM-PWM.VLO_PWM)/(nkpkt-1))))
			{
				krok++;
				if (krok>=nkpkt) k=4;
			}
			return 0;
		}
		case 4:
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
				BitSet2(&RSDT1.outSTAT2,_KALIB,0);			//wyzeruj znacznik aktywnosci procedury kalibracji

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
				BitSet2(&RSDT1.outSTAT2,_KALIB,0);			//wyzeruj znacznik aktywnosci procedury kalibracji
				M.ERR_BTY=0x07;
				PrintErr(0x07,1);
                PomALL();                                   //wykonaj zalegle pomiary na potrzeby zapisu statusu wylaczenia awaryjnego z blokada         * 
         *      ToWriteESTAT();
				k=0;
				ErrPTG();
			}
			k=5;
			return 0;
		}
		case 5:
		{
			M._VWYBIEG=1;					//uaktywnij wybieg wentylatora (zapobieganie szybkiemu wylaczeniu i zalaczeniu wentylatora przy aktywnych pobudzeniach)
			k=0;
			return 1;
		}*/        
	}
	return 1;
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//Odczytuje zapamietane w EEPROM dane kalibracji
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
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//Zwraca aproksymowana wartosc PWM dla zadanej wartosci predkosci obrotowej DVNT
//gdzie:
//dvnt - oczekiwana wartosc obrotow
//nkpkt - liczna punktow tablicy kalibracji
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
			wsp=(signed long int)(p2-p1)*1000;
			wsp/=(v2-v1); //wspolczynnik kierunkowy prostej
			pw=(signed long int)(dv-v1)*wsp;
			pw/=1000;
			
			pw+=p1; 				//y=[(y1-y0)/(x1-x0)]*(x-x1)+y0
			if(pw<0) pw=0;
			if(pw>PWM.HIPWM) pw=PWM.HIPWM;	//graniczna gorna wartosc wypelnienia
			if(pw<PWM.VLO_PWM) pw=PWM.VLO_PWM;	//graniczna dolna wartosc wypelnienia (wentylator jeszcze pracuje)
			return (unsigned char) pw;
		}
	}
	return p2;	//maksymalne wypelnienie dla predkosci poza zakresem
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//Zwraca aproksymowana wartosc PWM dla zadanej wartosci predkosci obrotowej DVNT
//z poprawka pop dla wyniku
//gdzie:
//dvnt - oczekiwana wartosc obrotow
//pop - poprawka pop dla wyniku przy niskich predkosciach dvnt
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
			wsp=(signed long int)(p2-p1)*1000;
			wsp/=(v2-v1); //wspolczynnik kierunkowy prostej
			pw=(signed long int)(dv-v1)*wsp;
			pw/=1000;
			
			pw+=p1; 				//y=[(y1-y0)/(x1-x0)]*(x-x1)+y0
			if(pw<0) pw=0;
			if(pw>PWM.HIPWM) pw=PWM.HIPWM;	//graniczna gorna wartosc wypelnienia
			if(pw<PWM.VLO_PWM) pw=PWM.VLO_PWM;	//graniczna dolna wartosc wypelnienia (wentylator jeszcze pracuje)
			if(i>2||!pop)
			{
				return (unsigned char) pw;
			}
			else
			{
				if((pw+pop)<=PWM.HIPWM)
				{
					return ((unsigned char)pw+pop);
				}
				else
				{
					return PWM.HIPWM;
				}
			}
		}
	}
	return p2;	//maksymalne wypelnienie dla predkosci poza zakresem
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//Przelicza oczekiwana wartosc predkosci obrotowej na wypelnienie pwm
//gdzie nst-oczekiwana wartosc
unsigned char CalkVNTtoPWM(unsigned char nst)
{
	unsigned char p;
	p=DVNTtoPWM(nst);
	return p;
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//Przelicza oczekiwana wartosc predkosci obrotowej na wypelnienie pwm z poprawka dla niskich predkosci
//gdzie nst-oczekiwana wartosc
//pop-wielkosc poprawki dla pwm
unsigned char CalkVNTtoPopPWM(unsigned char nst,unsigned char pop)
{
	unsigned char p;
	p=DVNTtoPopPWM(nst,pop);
	return p;
}
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
	tmp=(wrt-DtKNF.mmin)*100;
	tmp/=(DtKNF.mmax-DtKNF.mmin);
	if(tmp>=99) return 99;
	return (unsigned char)tmp;	
}
/*-------------------3--------------------------------------------------------------*/
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

//------------------------------------------------------------
//------------------------------------------------------------

