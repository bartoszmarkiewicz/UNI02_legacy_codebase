/*--------------------------------------------------------------*/
/*tacho.c*/
/*pomiar predkosci obrotowej wentylatora*/
/*druga wersja oprogramowania - zwiekszona precyzja pomiaru*/
/*--------------------------------------------------------------*/
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

#include "tacho.h"					//parametry lokalne

extern tpMAIN M;
extern tpRSDTA1 RSDT1;
extern DtKONF DtKNF;
extern DtPWM PWM;							//czeste operacje na danych

extern unsigned char xxy,zzz,vvv;

static unsigned int tnp[_NMAXTAB];
static unsigned int tvv[_NMAXTAB];
unsigned int wynik;
unsigned char nokrobr=2;		//liczba okresów na obrot (cecha charakterystyczna wentylatora)
bit stn,new,zero;
unsigned char tkrk=0;
volatile signed int wvv[_NFLTAB];
volatile signed int wv[_NFLTAB];
volatile unsigned int VDoc;
volatile signed long ddDVNT,v_ddt;
volatile signed int DVNT,dDVNT,tdDVNT,v_t,v_dt,v_tdt;
volatile unsigned int zr0,ni0,ip0,in0,bin0,nw0,tm0;

/*--------------------------------------------------------------*/
/*--------------------------------------------------------------*/
void ClrTachoDta(void)
{
    volatile unsigned char i;
    
    wynik=0;
    stn=0;
    new=0;
    zero=0;
    zr0=0;
    ni0=0;
    ip0=0;
    in0=0;
    bin0=0;
    nw0=0;
    tm0=0;
    tkrk=0;
    for(i=0;i<_NMAXTAB;i++)
    {
        tnp[i]=0;
        tvv[i]=0;
    }
    for(i=0;i<_NFLTAB;i++)
    {
        wvv[i]=0;
        wv[i]=0;
    }
    VDoc=0;
    M.rVVNT=0;
    M.VVNT=0;
    M.I2CVNT=0;
}
/*--------------------------------------------------------------*/
/*--------------------------------------------------------------*/
//Odmierz czas pomiedzy kolejnymi impulsami tachometra
//Umiescic w przerwaniu od Timer3
//0xFF7B
void inline IntTacho(void)
{
#if _DEMO==0    
	if(ip0<_PDZ3*1000) ip0++;
    else
	//if(ip0>=_PDZ3*1000) 
	{
		zr0=1;
		nw0=1;
	}
#endif
}
/*--------------------------------------------------------------*/
/*--------------------------------------------------------------*/
//Reakcja na kolejne impulsy z tachometra
//Umiescic w przerwaniu od INT0
void inline IntINT0(void)
{
    volatile unsigned char dt;
    
    dt=0;
	if(zr0)							//restart w przypadku postoju
	{
		TMR3_StopTimer();
		zr0=0;
		ip0=0;
		ni0=DtKNF.nokr;
		nw0=0;
		dt=1;
	}
	else
	{
		if(ni0) ni0--;				//zmniejsz wartosc licznika impulsow przypadajacych na jeden obrot
		else
		{
            TMR3_StopTimer();
			if(!nw0) 	
			{
				in0=((ip0+bin0)>>1);	//zapamietaj wynik pomiaru
                //wn=(ip0+bin0);
                //in0=(unsigned int)(wn>>1);	//zapamietaj wynik pomiaru
				bin0=in0;			//zapamietaj wynik na potrzeby filtracji
			}
			nw0=1;					//znacznik nowego wyniku
			ip0=0;
			ni0=DtKNF.nokr;			//odswierz wartosc licznika impulsow przypadajacych na jeden obrot
			dt=1;
		}
	}
    if(dt)
    {
        TMR3_Reload();
        TMR3_StartTimer();        
    }
}
/*--------------------------------------------------------------*/
/*--------------------------------------------------------------*/
//Zegar na potrzeby filtracji szybkich zmian wyniku pomiaru
//Umiescic w przerwaniu od Timer1 (100 odpowiada 1 sek.)
void inline IncTimTacho(void)
{
	if(tm0<0xFFFF) tm0++;
}
void StartTimTacho(void)
{
	tm0=0;
}
unsigned char TimTacho(void)
{
	return tm0;
}
/*--------------------------------------------------------------*/
/*--------------------------------------------------------------*/
//Zapis kolejne wartosci predkosci do tablicy
void AddTab(unsigned int *tbb,unsigned char ndat,unsigned int wnk)
{
	unsigned char i;

	if(!ndat||ndat>_NMAXTAB) ndat=_NMAXTAB;
	for(i=1;i<ndat;i++) tbb[i-1]=tbb[i];
	tbb[ndat-1]=wnk;	//nadpisz ostatnia pozycje aktualna dana
}
/*--------------------------------------------------------------*/
//znajdz wypadkowa wartosc pomiarow (dane int)
unsigned int FindPropVal(unsigned int *tbb,unsigned char ndat)
{
	unsigned int wrt;
	unsigned int tmx[_NMAXTAB];
	unsigned char wb[_NMAXTAB];
	unsigned char i,j,im,ix;
	unsigned int min,max;

	if(!ndat||ndat>_NMAXTAB) ndat=_NMAXTAB;

	//kopiuj dane do tablicy roboczej
	for(i=0;i<ndat;i++) tmx[i]=tbb[i];

	//znajdz wartosc minimalna i maksymalna w tablicy i wyzeruj
	for(i=0;i<ndat;i++) wb[i]=0;
	for(j=0;j<(ndat/2);j++)
	{
		min=0xFFFF;
		max=0;
		for(i=0;i<ndat;i++)
		{
			if(wb[i]) continue;
			wrt=tmx[i];
			if(max<wrt)
			{
				max=wrt;
				ix=i;
			}
			if(min>wrt)
			{
				min=wrt;
				im=i;
			}
		}
		if(ix==im)
		{
			im=j;
			ix=(ndat-1-j);
		}
		wb[ix]=1;
		wb[im]=1;
		tmx[ix]=0;
		tmx[im]=0;
	}
	//znajdz wartosc niezerow¹
	for(i=0;i<ndat;i++)
	{
		wrt=tmx[i];
		if(wrt) break;
	}
	return wrt;
}
/*--------------------------------------------------------------*/
//Okresl histogram, wybierz wartosc min z posrod najczesciej wyst. (dane char)
unsigned int HistVal(unsigned int *tbb,unsigned char ndat)
{
	unsigned int wrt;
	unsigned char wb[_NMAXTAB];
	unsigned char i,j,im;
	unsigned int max,min;

	if(!ndat||ndat>_NMAXTAB) ndat=_NMAXTAB;

	//zlicz czestosc wystepowania wartosci
	for(i=0;i<ndat;i++) wb[i]=0;
	for(j=0;j<ndat;j++)
	{
		wrt=tbb[j];
		for(i=0;i<ndat;i++)
		{
			if(wrt==tbb[i]) wb[j]++;
		}
	}
	//znajdz najczesciej wystepujaca wartosc
	max=0;
	im=ndat-1;
	for(i=0;i<ndat;i++)
	{
		wrt=wb[i];
		if(max<=wrt)
		{
			max=wrt;
			im=i;
		}
	}
	//znajdz wartosc ninimalna z najczesciej wystepujacych wartosci
	min=0xff;
	for(i=0;i<ndat;i++)
	{
		if(wb[i]==max)
		{
			wrt=tbb[i];
			if(min>=wrt)
			{
				min=wrt;
				im=i;
			}
		}
	}
	return tbb[im];
}
/*--------------------------------------------------------------*/
/*--------------------------------------------------------------*/
//Filtracja szybkich zmian co ustalony krok czasowy (dla zmian mniejszych niz +/-200obr/min)
unsigned int TFlt(unsigned int wnk)
{
	static unsigned int bwnk=0;
	static unsigned char krok=0;

	if(wnk>bwnk)
	{
		if((wnk-bwnk)>=200)
		{
			krok=0;
			bwnk=wnk;
 			return wnk;
		}
	}
	else
	{
		if((bwnk-wnk)>=200)
		{
			krok=0;
			bwnk=wnk;
 			return wnk;
		}
	}

	switch(krok)
	{
		case 0:
		{
			bwnk=wnk;
			StartTimTacho();
			krok=1;
			break;
		}
		case 1:
		{
			if(TimTacho()<33) //co 1/3 sekundy (*10msek)
			{
				break;
			}
			krok=0;
		}
	}
	return bwnk;
}
/*--------------------------------------------------------------*/
/*--------------------------------------------------------------*/
//zwraca srednia predkosc obrotowa wentylatora (z 10 pomiarow wykonanych co 10msek)
void PomiarVNT(void)
{
	//static unsigned int tvv[_NMAXTAB];
	static unsigned int bwnk2;
    static float avnt;
    
    signed long wn;
	unsigned int wnk,wnk2,krk;
	unsigned char i,wf;

#if _DEMO==0
	if(nw0)										//nowy wynik pomiaru?
	{
		nw0=0;
		if(zr0) wynik=0;
		else
		{
			wynik=in0;
			zero=0;
		}		
		wnk=wynik;
//--------------
		if(zr0)//!wynik&&!zero) 								//wentylator stoi?
		{
			zero=1;
			for(i=0;i<_NMAXTAB;i++)
			{			
 				tnp[i]=0xffff;					//maksymalna liczba impulsow = zerowa predkosc wentylatora
				tvv[i]=0;
			}
			bwnk2=0;
			wnk=0xffff;							//maksymalna liczba impulsow = zerowa predkosc wentylatora
		}
//--------------
		AddTab(tnp,_NDTPOMT,wnk);				//pamiec zliczonych impulsow
		wnk=FindPropVal(tnp,_NDTPOMT);			//filtracja 1 stopnia 
		wnk2=(unsigned int)(round(((1000.0/((float)wnk/_PDZ3))*60.0)));
//--------------
		if(wnk2&&bwnk2!=wnk2)					//filtracja 2 stopnia
		{
			if(bwnk2>wnk2)						//nagly nierealny przyrost predkosci>=5000 obr/sek?
			{
				if(bwnk2-wnk2>5000)
				{
                    krk=(bwnk2-wnk2)/10;
                    wn=bwnk2-krk;
					if(wn>=0) bwnk2-=krk;
                    else bwnk2=0;       
					tkrk=0;						//ignoruj wynik, rozpocznij nowy pomiar
					return;
				}
			}
			else
			{
				if(wnk2-bwnk2>5000)
				{
                    krk=(wnk2-bwnk2)/10;
                    wn=bwnk2+krk;
					if(wn<0xffff) bwnk2+=krk;
                    else bwnk2=0xffff;
					tkrk=0;						//ignoruj wynik, rozpocznij nowy pomiar
					return;
				}
			}
		}
		bwnk2=wnk2;								//zapamietaj wynik
//--------------
		AddTab(tvv,_NDTPOMT2,wnk2);			//pamiec wartosci pomiarow
		wnk2=HistVal(tvv,_NDTPOMT2);			//filtracja 3 stopnia
		M.rVVNT=TFlt(wnk2);						//filtracja 4 stopnia, wynik pomiaru typu int
        //M.rVVNT=wnk2;
        //filtracja wartosci wyswietlanej
        avnt=avnt+((M.rVVNT-avnt)/25.0);
        if(avnt<0)
        {
            avnt=0;
        }
        wf=(unsigned char)(floor(avnt/100.0));
        M.VVNT=(unsigned char)(round(avnt/100.0));    
        if((wf==DtKNF.mmin)||(wf==DtKNF.mmax))
        {
            M.VVNT=wf;
        }
        if(M.VVNT>99) M.VVNT=99;
		//M.VVNT=(unsigned char)((M.rVVNT/100.0));//wynik pomiaru typu char
		//if((M.rVVNT%100)>=70) M.VVNT+=1; 
        if(M.VVNT) M.I2CVNT=(unsigned char)((float)M.VVNT*0.7+5.0/7);
        else M.I2CVNT=0;        
		tkrk=0;									//rozpocznij nowy pomiar
	}
	//Pomiar parametrow dynamicznych VNT
	FPmVNT();
#else
    if(!M.POB_VNT)
    {
        M.rVVNT=0;              //predkosc zerowa
        M.VVNT=M.rVVNT;        
    }
    else
    if(M.POB_VNT&&!M.POB_GRZ)        
    {
        M.rVVNT=ProcToVNT(0);   //predkosc minimalna
        M.VVNT=M.rVVNT;        
    }
    else
    {
        M.rVVNT=ProcToVNT(PWM.APROC);   //predkosc zalezna od wydatku modulatora
        M.VVNT=M.rVVNT;  
    }
    if(M.VVNT) M.I2CVNT=(unsigned char)((float)M.VVNT*0.7+5.0/7);
    else M.I2CVNT=0;    
#endif
	RSDT1.outVNT=M.VVNT;
}
/*--------------------------------------------------------------*/
/*--------------------------------------------------------------*/
//Zerowanie danych dynamicznych
void ClrDtV(void)
{
	DVNT=0;
	dDVNT=0;
	tdDVNT=0;
	ddDVNT=0;
	v_t=0;
	v_dt=0;
	v_tdt=0;
	v_ddt=0;
}
//Szacowanie predkosci zmian dla VNT
void PredkoscZmianVNT(void)
{
	static unsigned int ffv;
	static unsigned char indff;
	unsigned char i;
	signed long w;
#if _DEMO==0
	if(!_VNT_GetValue())
	{
 		ffv=0;
		indff=0;
	}
	else
	{	
		if(!indff) 
		{
			for(i=0;i<10;i++) wvv[i]=0;
		}
		indff++;
		if(indff>=9)
		{
 			indff=9;
			for(i=0;i<9;i++) wvv[i]=wvv[i+1]; //FIFO
		}
		wvv[indff]=M.rVVNT-ffv;			  //przyrost/spadek
	}
#else
	if(!M.POB_VNT)
	{
 		ffv=0;
		indff=0;
	}
	else
	{	
		if(!indff) 
		{
			for(i=0;i<10;i++) wvv[i]=0;
		}
		indff++;
		if(indff>=9)
		{
 			indff=9;
			for(i=0;i<9;i++) wvv[i]=wvv[i+1]; //FIFO
		}
		wvv[indff]=M.rVVNT-ffv;			  //przyrost/spadek
	}    
#endif
    
	ffv=M.rVVNT;
	w=0;
	for(i=0;i<10;i++) {if(w+wvv[i]<=0xff) w+=wvv[i];}
	ddDVNT=w;
}
/*---------------------------------------------------------------------------------*/
//Filtr dla predkosci zmian
void FltParVNT(void)
{
	static unsigned char indv,indtm;
	static unsigned int fv;
	unsigned int wr,i;
	unsigned long www;
//----------
	//wykonywane co 0.1 sek.
	//predkosc zmian
	if(M.rVVNT>fv) wr=M.rVVNT-fv;
	else wr=fv-M.rVVNT;
	if(wr) indv++;
	if(wr>=2) indv=10;
	PredkoscZmianVNT();

//----------
	//wykonywane co 0.1 sek.
	//szybki znacznik trendu
	for(i=0;i<9;i++) wv[i]=wv[i+1]; //FIFO
	wv[9]=M.rVVNT;
	www=0;
	for(i=0;i<9;i++) www+=wv[i];
	tdDVNT=wv[9]-www/9;
	if(tdDVNT>0&&tdDVNT<=3) tdDVNT=0;
	if(tdDVNT<0&&tdDVNT>=-3) tdDVNT=0;
//----------
	//wykonywane co 1 sek.
	//kierunek i wielkosc zmian
	indtm++;
	if(indtm>9)
	{
		indtm=0;
		//szybkosc i kierunek zmian VNT
		if(indv<=7) 
		{
			dDVNT=0;	//temp = const
		}
		else
		{ 
			dDVNT=M.rVVNT-fv;
			fv=M.rVVNT;
		}
		indv=0;
	}
}
/*---------------------------------------------------------------------------------*/
//Pomiar parametrow dynamicznych VNT
void FPmVNT(void)
{
//----------
	//Filtr dla danych wejsciowych modulatora.
	DVNT=VDoc-M.rVVNT;
 	v_t=DVNT;
	if(RTdS(_RTFVNT)>1)
	{
		StartRTdS(_RTFVNT);						//wielkosc bledu VNT
		FltParVNT();
		v_dt=dDVNT;			//wartosc (narostu/spadku)/(1 sek)
		v_tdt=tdDVNT;		//znacznik trendu (0.1sek)
		v_ddt=ddDVNT;		//szybkosc zmian  (0.1sek)
	}
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//------------------------------------------------------------
//Wartosc oczekiwana predkosci obrotowej
void SetNstVNT(unsigned char doc)
{
 	VDoc=doc*100;
}
//------------------------------------------------------------
//zwraca wartosc 1 przy stwierdzonym wzroscie temperatury
//(w obiegu CW)
//dla t=0 zwraca wartosc jeden przy dowolnym wzroscie temp.
//dla t>0 zwraca wartosc jeden przy wroscie > od t [C/sek].
bit Vrosnie(const unsigned char t)
{
	//unsigned char w;
	//w=(dDVNT>t)?1:0;
	//return w;//(v_dt>t)?1:0;
	return (v_dt>t)?1:0;
}
//------------------------------------------------------------
//zwraca wartosc 1 przy stwierdzonym spadku temperatury
//(w obiegu CW)
//dla t=0 zwraca wartosc jeden przy dowolnym spadku temp.
//dla t>0 zwraca wartosc jeden przy spadku > od t [C/sek].
bit Vmaleje(const unsigned int t)
{
	signed char w;
	w=-t;
	return (v_dt<w)?1:0;
	//return (v_dt<t)?1:0;
}
//------------------------------------------------------------
//zwraca wartosc 1 jezeli temperatura sie nie zmienia
//(w obiegu CW)
bit Vstoi(void)
{
	return (!v_dt)?1:0;
}
//------------------------------------------------------------
//zwraca wartosc 1 temperatura jes nizsza od nastawy o wartosc parametru t
//(w obiegu CW)
//dla t=0 zwraca wartosc jeden jezeli temp. jest nizsza od nastawy.
//dla t>0 zwraca wartosc jeden jezeli temp. jest nizsza od nastawy o >= t.
bit Vnizsza(const unsigned int t)
{
	return (v_t>t)?1:0;
}
//------------------------------------------------------------
//zwraca wartosc 1 temperatura jes nizsza od nastawy o wartosc parametru t
//(w obiegu CW)
//dla t=0 zwraca wartosc jeden jezeli temp. jest nizsza od nastawy.
//dla t>0 zwraca wartosc jeden jezeli temp. jest nizsza od nastawy o >= t.
bit Vwyzsza(const unsigned int t)
{
	signed char w;
	w=-t;
	return (v_t<w)?1:0;
}
//------------------------------------------------------------
//zwraca wartosc 1 jezeli temperatura = nastawa.
//(w obiegu CW)
bit Vrowna(void)
{
	return (!v_t)?1:0;
}
/*---------------------------------------------------------------------------------*/
//zwraca wartosc 1 przy stwierdzonym wzroscie temperatury
//(w obiegu CW)
//dla t=0 zwraca wartosc jeden przy dowolnym wzroscie temp.
//dla t>0 zwraca wartosc jeden przy wroscie > od t [C/sek].
bit VTrosnacy(const unsigned char t)
{
	return (v_tdt>t)?1:0;
}
//------------------------------------------------------------
//zwraca wartosc 1 przy stwierdzonym spadku temperatury
//(w obiegu CW)
//dla t=0 zwraca wartosc jeden przy dowolnym spadku temp.
//dla t>0 zwraca wartosc jeden przy spadku > od t [C/sek].
bit VTmalejacy(const unsigned char t)
{
	signed char w;
	w=-t;
	return (v_tdt<w)?1:0;
}
//------------------------------------------------------------
//zwraca wartosc 1 jezeli temperatura sie nie zmienia
//(w obiegu CW)
bit VTzerowy(void)
{
	return (!v_tdt)?1:0;
}
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
#endif