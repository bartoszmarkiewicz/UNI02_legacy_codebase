/*pomiar.c*/
//Procedury pomiarowe (oparte na przetworniku AC)
// (!) Bazuje na funkcjach z pliku: ac.c, rtimer.c /RTdS(2)/
/*---------------------------------------------------------------------------------*/
#include <pic18.h>
#include "global.h"				//parametry globalne
#include "pomiar.h"				//parametry lokalne
#include "konfig.h"
#include "main.tp.h"
#include "pwm.h"


DataCO DtCO;
DataCW DtCW;
DataZW DtZW;
DataPFN PFN;
DataCSN DtCS;

extern tpMAIN M;
extern DtKONF DtKNF;
extern DtPWM PWM;

unsigned int pbf;
unsigned char pnst;
static unsigned char PmrtimFlt1,PmrtimFlt2;
static unsigned int PmrtimFlt3;

unsigned char wff[12];

#if _SRVTRB==1						//aktywny tryb serwisowy kompilacji?
unsigned char mypwm=_MG2_MOD;
#endif
/*---------------------------------------------------------------------------------*/
//Inicjalizacja DtCW
void InitDataPmr(void)
{
	DtCW._err_CW=0;
	DtCW._su_CW=0;
	DtCW._ad_CW=0;
	DtCW.indCW=0;
}
//-------------------------------------------------------------------------------
//Timer na potrzeby filtracji (umiescic w przerwaniu od zegara TMR1)
void IncPmrTimer(void)
{
	if(PmrtimFlt1<0xff) PmrtimFlt1++;
	if(PmrtimFlt2<0xff) PmrtimFlt2++;
	if(PmrtimFlt3<0xffff) PmrtimFlt3++;
}
//-------------------------------------------------------------------------------
//Zegar 1 odcinkow czasowych
void StartTim1(void)
{
	PmrtimFlt1=0;
}
bit PmrTim1(unsigned char tm)
{
	if(PmrtimFlt1<tm) return 0;
	return 1;
}
//-------------------------------------------------------------------------------
//Zegar 2 odcinkow czasowych
void StartTim2(void)
{
	PmrtimFlt2=0;
}
bit PmrTim2(unsigned char tm)
{
	if(PmrtimFlt2<tm) return 0;
	return 1;
}
//-------------------------------------------------------------------------------
//Zegar 3 odcinkow czasowych
void StartTim3(void)
{
	PmrtimFlt3=0;
}
bit PmrTim3(unsigned int tm)
{
	if(PmrtimFlt3<tm) return 0;
	return 1;
}
/*---------------------------------------------------------------------------------*/
//Filtracja wartosci temperatur na cele wizualizacji.
void GoldenEye(void)
{
	signed char dt;
	signed char pop;
//----------
//wskaz CW
	dt=DtCW.PPCW-DtCW.CW;
	if((DtCW.CW>=(DtCW.PPCW+_DM_GECW))||(DtCW.CW<=(DtCW.PPCW-_DM_GECW))) DtCW.SCW=DtCW.CW;
	else
	{
		if(dt>=0)
		{
			if(dt<_BNCW) pop=dt;
			else
			if((dt>=_BNCW)&&(dt<=(_DM_GECW-_BNCW))) pop=_BNCW;
			else
			if(dt>(_DM_GECW-_BNCW)) pop=_DM_GECW-dt;
			DtCW.SCW=DtCW.CW+pop;
		}
		else 
		{
			if((-1*dt)<_BNCW) pop=(-1*dt);//pop=_BNCW+dt;
			else
			if(((-1*dt)>=_BNCW)&&((-1*dt)<=(_DM_GECW-_BNCW))) pop=_BNCW;
			else
			if((-1*dt)>(_DM_GECW-_BNCW)) pop=_DM_GECW+dt;
			DtCW.SCW=DtCW.CW-pop;
		}
	}
}
/*---------------------------------------------------------------------------------*/
//Filtr dla danych wejsciowych modulatora.
void PredkoscZmianCW(void)
{
	static unsigned char ffcw,indff;
	unsigned char i,w;

	if(!M._PRACA)
	{
 		ffcw=0;
		indff=0;
	}
	else
	{	
		if(!indff) 
		{
			for(i=0;i<10;i++) wff[i]=0;
		}
		indff++;
		if(indff>=9)
		{
 			indff=9;
			for(i=0;i<9;i++) wff[i]=wff[i+1]; //FIFO
		}
		wff[indff]=DtCW.CW-ffcw;			  //przyrost/spadek
	}
	ffcw=DtCW.CW;
	w=0;
	for(i=0;i<10;i++) w+=wff[i];
	DtCW.ddDCW=w;
}
/*---------------------------------------------------------------------------------*/
//Filtr dla danych wejsciowych modulatora.
void FltParMod(void)
{
	static unsigned char indtm,fcw;
	unsigned char wr;

//----------
	//wykonywane co 0.1 sek.
	//CW
	if(DtCW.CW>fcw) wr=DtCW.CW-fcw;
	else wr=fcw-DtCW.CW;
	if(wr) DtCW.indCW++;
	if(wr>=2) DtCW.indCW=10;

	PredkoscZmianCW();
//----------
	//wykonywane co 1 sek.
	indtm++;
	if(indtm>9)
	{
		indtm=0;

		//szybkosc i kierunek zmian temp CW
		if(DtCW.indCW<=7) 
		{
			DtCW.dDCW=0;	//temp = const
			DtCW._fpCW=1;
		}
		else
		{ 
			DtCW.dDCW=DtCW.CW-fcw;
			DtCW._fpCW=1;
			fcw=DtCW.CW;
		}
		DtCW.indCW=0;
	}
}
/*---------------------------------------------------------------------------------*/
//Filtracja wartosci (nowa wartosc odswiezana jest co 0.5 sek)
//(umiescic w petli glownej programu po procedurach pomiaru CO,CW,PCO,PCW)
void FiltrPomiarowyCW(void)
{
//----------
	//Filtr GoldenEye
	if(PmrTim1(_ITFGE))
	{
		StartTim1();
		GoldenEye();
	}
//----------
	//Filtr dla danych wejsciowych modulatora.
	DtCW.DCW=(DtCW.PPCW-(_BNCW-1))-DtCW.CW;		//wielkosc bledu CW
	if(PmrTim2(_ITFTM))
	{
		StartTim2();
		FltParMod();
	}
}
/*---------------------------------------------------------------------------------*/
//Pomiar wartosci CW i ustawienie odpowiednich flag stanu
void PomiarCW(void)
{
	DtCW.AC_CW=ReadAC(_CHN_CW);
	DtCW.CW=ReadTabNTC(DtCW.AC_CW);
	
	if(DtCW.AC_CW<=_NTC_MIN||DtCW.AC_CW>=_NTC_MAX) 		//stwierdzenie awarii czujnika temp. CO
	{
		DtCW._err_CW=1;
		DtCW._su_CW=0;
		DtCW._ad_CW=0;
		return;
	}	
	else DtCW._err_CW=0;
	if(DtCW.PCW<55)										//dwa bezwzgledne progi wylaczenia
	{
		if(DtCW.CW>F1_OFF_CW) DtCW._ad_CW=1;
		else DtCW._ad_CW=0;
		if(DtCW.CW<=F1_OFF_CW-F1_MR_OFF_CW) DtCW._su_CW=1;
			else DtCW._su_CW=0;
	}
	else
	{
		if(DtCW.CW>F1_OFF2_CW) DtCW._ad_CW=1;
		else DtCW._ad_CW=0;
		if(DtCW.CW<=F1_OFF2_CW-F1_MR_OFF_CW) DtCW._su_CW=1;
			else DtCW._su_CW=0;
	}
}
/*---------------------------------------------------------------------------------*/
//Formatowanie nastawy CW
void SetModPCW(void)
{
	if(M._AKTYWNE_KNF)		//aktywna konfiguracja?
	{
		DtCW.PPCW=_KNF_PCW;		
	}
	else DtCW.PPCW=DtCW.PCW;
}
//Zapis nastawy PCW do EEPROM
void WritePCW(void)
{
	if(!M._AKTYWNE_KNF)
	{
		if(DtCW._wrtPCW&&RTdS(_RTPCW)>=_TWRTPCW)
		{
			DtCW._wrtPCW=0;
			StartRTdS(_RTPCW);
			WriteEEPCW();
		}
	}
}
//Nastawa PCW
void PomiarPCW(void)
{
	static unsigned char pnst;

	DtCW.minPCW=F1_MIN_PCW;
	DtCW.maxPCW=F1_MAX_PCW;

#if _SRVTRB==1						//aktywny tryb serwisowy kompilacji?
	DtCW.PCW=F1_MAX_PCW;
	DtCW._newPCW=0;
	DtCW._wrtPCW=0;
	if(!RdPrt(S_KDW))
	{
		ReadDataXONLI(RdPrt(S_KUP),&mypwm,_MG2_MOD,1,1);
	}
	if(!RdPrt(S_KUP))
	{
		if(PWM._ENOSC)
		{
			if(mypwm<PWM.MDMOD) mypwm=PWM.MDMOD;
			ReadDataXONLD(RdPrt(S_KDW),&mypwm,PWM.MDMOD,1,1);
		}
		else
		{
			ReadDataXONLD(RdPrt(S_KDW),&mypwm,0,1,1);
		}
	}
#else
    if(!(_GRZEJ&&!RdPrt(S_PLM)))    //zapobiegaj zmianie nastawy podczas rozruchu
    {
        if(!(RdPrt(S_KDW)&&RdPrt(S_KUP))) 					//nie nacisnieto jednoczesnie obu przyciskow
        {
            if(!M._RUSZ_PLOMIEN&&!M._RUSZ_STB&&!M._RUSZ_CIAG&&!M._RUSZ_NTC_CW)
            {
                if(!M._AKTYWNE_KNF&&!M._AKTYWNE_SRW)
                {
                    if(!RdPrt(S_KDW))
                    {
                        ReadDataXONLI(RdPrt(S_KUP),&DtCW.PCW,DtCW.maxPCW,1,1);
                        if(RdPrt(S_KUP)&&(DtCW.PCW==DtCW.maxPCW)) DtCW._newPCW=1;
                    }
                    if(!RdPrt(S_KUP))
                    {
                        ReadDataXONLD(RdPrt(S_KDW),&DtCW.PCW,DtCW.minPCW,1,1);
                        if(RdPrt(S_KDW)&&(DtCW.PCW==DtCW.minPCW)) DtCW._newPCW=1;
                    }
                    if(!RdPrt(S_KDW)&&!RdPrt(S_KUP)) //oba przyciski wycisniete
                    {
                        if(DtCW._wrtPCW)
                        {
                            if(RTdS(_RTPCW)>_TFLPCW)	DtCW._newPCW=0;	 //podtrzymanie wyswietlania nastawy przez czas _TWRTPCW-1
                        }
                        else DtCW._newPCW=0;
                    }
                    if(DtCW.PCW!=pnst)
                    {
                        pnst=DtCW.PCW;
                        //-------------------							//znacznik nowej nastawy
                        DtCW._newPCW=1;
                        RNst(DtCW.PCW);
                        //-------------------
                        DtCW._wrtPCW=1;									//zapis nowej nastawy w pamieci
                        StartRTdS(_RTPCW);
                    }
                }else DtCW._newPCW=0;
            }else DtCW._newPCW=0;
        }else DtCW._newPCW=0;
    }else DtCW._newPCW=0;
#endif								//nacisnieto dwa przyciski jednoczesnie
	SetModPCW();
	DtCW.DAD_CW=ReturnACfromNTC(DtCW.PPCW-(_BNCW-1));
	DtCW.MAD_CW=ReturnACfromNTC(DtCW.PPCW-(_BNCW-1)-_MR_MADCW);
}
/*---------------------------------------------------------------------------------*/


