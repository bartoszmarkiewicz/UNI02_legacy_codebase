/*pomiar.c*/
//Procedury pomiarowe (oparte na przetworniku AC) UNISTER zmodyfikowany GRZ01
// (!) Bazuje na funkcjach z pliku: ac.c, rtimer.c /RTdS(2)/
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
#include "../../mcc_generated_files\mcc.h"

#include "global.h"
#include "main.h"
#include "global.ex.h"

#include "pomiar.h"				//parametry lokalne

extern MGSRSData MGSD;
extern tpRSDTA1 RSDT1;
extern tpMAIN M;
extern DtKONF DtKNF;
extern DtPWM PWM;
extern unsigned char x;
extern unsigned char xxy,zzz,mmm,vvv;

DataCO DtCO;
DataIN DtIN;
DataCW DtCW;
DataZW DtZW;
DataPFN PFN;
DataCSN DtCS;
static unsigned char PmrtimFlt1,PmrtimFlt2;
static unsigned int PmrtimFlt3;
unsigned char wff[12],wff2[12];

#if _SRVTRB==1						//aktywny tryb serwisowy kompilacji?
unsigned char mypwm=_MG2_MOD;
#endif
//-------------------------------------------------------------------------------
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
/*---------------------------------------------------------------------------------*/
//Inicjalizacja DtCO, DtCW, DtCS, PFN
void InitDataPmr(void)
{
	DtCO._err_CO=0;
	DtCO._su_CO=0;
	DtCO._ad_CO=0;
	DtCO._su_SRW_CO=0;
	DtCO._ad_SRW_CO=0;
	DtCO._cold_CO=0;
	DtCO._hot_CO=0;
	DtCO._on_DZ_CO=0;
	DtCO._su_DZ_CO=0;
	DtCO._ad_DZ_CO=0;
	DtCO._ptwp_CO=0;
	DtCO._pz_CO=0;
	DtCO.indCO=0;

	DtCW._err_CW=0;
	DtCW._su_CW=0;
	DtCW._ad_CW=0;
	DtCW._on_DZ_CW=0;
	DtCW._su_DZ_CW=0;
	DtCW._ad_DZ_CW=0;
	DtCW.indCW=0;

	DtCS._hi=0;
	DtCS._lo=0;
	DtCS._no=0;
	DtCS._error=0;
	DtCS._work=1;			//przed pomiarem domyslne cisnienie robocze
}
//-------------------------------------------------------------------------------
//Timer na potrzeby filtracji (umiescic w przerwaniu od zegara TMR1)
void inline IncPmrTimer(void)
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
/*
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
 */
/*---------------------------------------------------------------------------------*/
//Filtracja wartosci temperatur na cele wizualizacji.
void GoldenEye(void)
{
	signed char dt;
	signed char pop;
	unsigned char hist,ph;
//----------	
//wskaz CO
	dt=M.CONTSETP-DtCO.CO;
    if((DtCO.PPCO-M.CONTSETP)>0) ph=DtCO.PPCO-M.CONTSETP;   //poprawka na zmienna wartosc punktu modulacji
    else ph=0;    
	if((DtCO.CO>=(M.CONTSETP+_DM_GECO))||(DtCO.CO<=(M.CONTSETP-_DM_GECO))) DtCO.SCO=DtCO.CO;
	else
	{
		if(dt>=0)	//co<=pco?
		{
			//wartosc histerezy
			hist=DtCO.HstCo+ph;
			if(dt<_BNCO) pop=dt;
			else
			if((dt>=_BNCO)&&(dt<=(_DM_GECO-_BNCO))) pop=_BNCO;
			else
			if(dt>(_DM_GECO-_BNCO)) pop=_DM_GECO-dt;

			if(pop&&!hist) pop=pop-1;	//zerowa wartosc histerezy?
			DtCO.SCO=DtCO.CO+pop;
		}
		else //co>pco
		{
			//********* proc poprawiona dla gornego marginesu wylaczenia = hist
			//wartosc histerezy
			hist=DtCO.HstCo+ph;
			//kontrola progu histerezy
			switch(DtKNF.tpkt)		//dla danego rodzaju kotla
			{
				case _MONOHB:
				{
                    switch(DtKNF.opdg)
                    {
                        case 0:
                        case 2:                            
                        {
                            if((M.CONTSETP+hist)>(_MAX_PCO+F2_AD_CO))	//nastawa+hist > _MAX_PCO+F2_AD_CO?
                            {
                                hist=F2_AD_CO;
                            }                            
                            break;
                        }
                        case 1:
                        {
                            if((M.CONTSETP+hist)>(_MAX_PDCO+F2_AD_CO))	//nastawa+hist > _MAX_PDCO+F2_AD_CO?
                            {
                                hist=F2_AD_CO;
                            }                            
                            break;
                        } 
                        default:
                        {
                            if((M.CONTSETP+hist)>(_MAX_PCO+F2_AD_CO))	//nastawa+hist > _MAX_PCO+F2_AD_CO?
                            {
                                hist=F2_AD_CO;
                            }                             
                        }
                    }
					break;                     
                }
				case _UNICO:				//kociol unico
				{
                    switch(DtKNF.opdg)
                    {
                        case 0:
                        case 2:                            
                        {
                            if((M.CONTSETP+hist)>(_MAX_PCO+F3_AD_CO))	//nastawa+hist > _MAX_PCO+F2_AD_CO?
                            {
                                hist=F3_AD_CO;
                            }                            
                            break;
                        }
                        case 1:
                        {
                            if((M.CONTSETP+hist)>(_MAX_PDCO+F3_AD_CO))	//nastawa+hist > _MAX_PDCO+F2_AD_CO?
                            {
                                hist=F3_AD_CO;
                            }                            
                            break;
                        } 
                        default:
                        {
                            if((M.CONTSETP+hist)>(_MAX_PCO+F3_AD_CO))	//nastawa+hist > _MAX_PCO+F2_AD_CO?
                            {
                                hist=F3_AD_CO;
                            }                              
                        }
                    }                    
					break;
				}
				default:
				{
                   switch(DtKNF.opdg)
                    {
                        case 0:
                        case 2:                            
                        {
                            if((M.CONTSETP+hist)>(_MAX_PCO+F2_AD_CO))	//nastawa+hist > _MAX_PCO+F2_AD_CO?
                            {
                                hist=F2_AD_CO;
                            }                            
                            break;
                        }
                        case 1:
                        {
                            if((M.CONTSETP+hist)>(_MAX_PDCO+F2_AD_CO))	//nastawa+hist > _MAX_PDCO+F2_AD_CO?
                            {
                                hist=F2_AD_CO;
                            }                            
                            break;
                        } 
                        default:
                        {
                            if((M.CONTSETP+hist)>(_MAX_PCO+F2_AD_CO))	//nastawa+hist > _MAX_PCO+F2_AD_CO?
                            {
                                hist=F2_AD_CO;
                            }                            
                        }
                    }
					break;
				}
			}
			//nowa wartosc poprawki wyswietlania
			if(hist>_BNCO2)
			{
				if(hist)	//niezerowa wartosc histerezy?
				{
					if(-1*dt<(_BNCO2)) pop=(_BNCO2)+dt;
					else pop=0;							//dla temperatur >nastawa poprawka brak filtra
				}
				else pop=0;	//brak filtra dla hist=0

				//if((-1*dt>=(_BNCO2))&&(-1*dt<=(hist-_BNCO2)+1)) pop=(_BNCO2);	
				//else pop=0;			
			}
			else pop=0;
			DtCO.SCO=DtCO.CO-pop;			
		}
	}
	RSDT1.outCO=DtCO.SCO;
//----------
//wskaz CW
	if(DtKNF.tpkt!=_UNICO)	//kociol inny niz UniCO? 
	{
		dt=DtCW.PPCW-DtCW.CW;
		if((DtCW.CW>=(DtCW.PPCW+_DM_GECW))||(DtCW.CW<=(DtCW.PPCW-_DM_GECW))) DtCW.SCW=DtCW.CW;
		else
		{
			if(dt>=0)	//cw<pcw?
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
				if(-1*dt<(_BNCW2)) pop=(_BNCW2)+dt;
				//else pop=0; //*****
				else
				if((-1*dt>=(_BNCW2))&&(-1*dt<=((_DM_GECW2)-(_BNCW2)))) pop=(_BNCW2);
				else
				if(-1*dt>((_DM_GECW2)-(_BNCW2))) pop=(_DM_GECW2)+dt;
				DtCW.SCW=DtCW.CW-pop;
			}
		}
	}
	else DtCW.SCW=DtCW.CW;	//kociol zasobnikowy - brak filtracji wskazu temp. CW		
	RSDT1.outCW=DtCW.SCW;
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
void PredkoscZmianCO(void)
{
	static unsigned char ffco,indff;
	unsigned char i,w;

	if(!M._PRACA)
	{
 		ffco=0;
		indff=0;
	}
	else
	{	
		if(!indff) 
		{
			for(i=0;i<10;i++) wff2[i]=0;
		}
		indff++;
		if(indff>=9)
		{
 			indff=9;
			for(i=0;i<9;i++) wff2[i]=wff2[i+1]; //FIFO
		}
		wff2[indff]=DtCO.CO-ffco;			  //przyrost/spadek
	}
	ffco=DtCO.CO;
	w=0;
	for(i=0;i<10;i++) w+=wff2[i];
	DtCO.ddDCO=w;
}
/*---------------------------------------------------------------------------------*/
//Filtr dla danych wejsciowych modulatora.
void FltParMod(void)
{
	static unsigned char indtm,fco,fcw;
	unsigned char wr;

//----------
	//wykonywane co 0.1 sek.
	//CO
	if(DtCO.CO>fco) wr=DtCO.CO-fco;
	else wr=fco-DtCO.CO;
	if(wr) DtCO.indCO++;
	if(wr>=2) DtCO.indCO=10;
	PredkoscZmianCO();
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

		DtZW.SZW=DtZW.ZW;				//wartosc temp zewnwtrznej (aktualizacja co 1 sek.)

		//szybkosc i kierunek zmian temp CO
		if(DtCO.indCO<=7)
		{
 			DtCO.dDCO=0;	//temp = const
			DtCO._fpCO=1;
		}
		else
		{ 
			DtCO.dDCO=DtCO.CO-fco;
			DtCO._fpCO=1;
			fco=DtCO.CO;
		}
		DtCO.indCO=0;
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
void FiltrPomiarowyCOCW(void)
{
    unsigned char bnc;
//----------
	//Filtr GoldenEye
	if(PmrTim1(_ITFGE))
	{
		StartTim1();
		GoldenEye();
	}
//----------
    //Dostosuj margines do wymogow zmiennej nastawy modulatora
    if(M.CONTSETP<=DtCO.PPCO)
    {
        if((DtCO.PPCO-M.CONTSETP)>(_BNCO-1))
        {
            bnc=0;
        }
        else
        {
            bnc=(_BNCO-1)-(DtCO.PPCO-M.CONTSETP);
        }
    }
    else
    {
        bnc=(_BNCO-1);
    }
    //Filtr dla danych wejsciowych modulatora.
    DtCO.DCO=(M.CONTSETP-bnc)-DtCO.CO;		//wielkosc bledu CO

	DtCW.DCW=(DtCW.PPCW-(_BNCW-1))-DtCW.CW;		//wielkosc bledu CW

	if(!RdPrt(S_ERR_IN))							
	{
		//delta wzgledem temp. wyj. CO
		if(DtCO.CO>DtIN.IN) DtIN.DIN=DtCO.CO-DtIN.IN;	
		else DtIN.DIN=0;
		//delta wzgledem nastawy CO
		if((M.CONTSETP-bnc)>DtIN.IN) DtIN.DIN2=(M.CONTSETP-bnc)-DtIN.IN;	
		else DtIN.DIN2=0;
	}
	if(PmrTim2(_ITFTM))
	{
		StartTim2();
		FltParMod();
	}
	if(DtCO.dDCO>=_MAX_DXCO) DtCO._ldDCO=1;	//b.szybki narost temperatury?
	else DtCO._ldDCO=0;
}
/*---------------------------------------------------------------------------------*/
//Pomiar wartosci CO i ustawienie odpowiednich flag stanu
#if _PRIOR_RT==0
void PomiarCO(void)
{
    volatile static float avr=0;
    volatile unsigned int AC_DTA;
    volatile unsigned char AC_AVR;
    
    AC_DTA=ReadAC16(_CHN_CO);                       //pomiar 16bit
    DtCO.HstCo=DtKNF.hsco;
	DtCO.F3PPCO=DtKNF.conw;
    //-------------------------------------
    //wartosc srednia
    if(!avr)
    {
        avr = (float)AC_DTA;
    }
    else
    {
        avr = avr + ((float)AC_DTA - avr) / 50;             //wartosc srednia pomiaru
    }
    //-------------------------------------
    //reakcja na zwarcie/rozwarcie
	AC_AVR=(unsigned char)(((unsigned int)avr)>>8);
    if(AC_AVR<=_NTC_MIN||AC_AVR>=_NTC_MAX) 		//stwierdzenie awarii czujnika temp. CO
	{
		DtCO._err_CO=1;
		DtCO._su_CO=0;
		DtCO._ad_CO=0;
		DtCO._cold_CO=0;
		DtCO._hot_CO=0;
		DtCO._on_DZ_CO=0;
		DtCO._su_DZ_CO=0;
		DtCO._ad_DZ_CO=0;
		DtCO._ptwp_CO=0;
		DtCO._pz_CO=0;
		DtCO._su_SRW_CO=0;
		DtCO._ad_SRW_CO=0;
		RSDT1.outrCO=0;
		return;
	}	
	else
    {
        if(DtCO._err_CO)
        {
            avr=0;
        }
        DtCO._err_CO=0;
    }
       //-------------------------------------
    //wynik pomiaru
    DtCO.AC_CO=AC_AVR;
    DtCO.CO=ReadTabNTC(DtCO.AC_CO);
    
	RSDT1.outrCO=DtCO.CO;
	switch(DtKNF.tpkt)		//dla danego rodzaju kotla
	{
		case _MONOHB:
		{
			if(!M._LIN_PWD) //brak przewyzszenia temperaturowego   
            {
                switch(DtKNF.opdg)
                {
                    case 0:
                    case 2:                            
                    {
                        if((M.CONTSETP+DtCO.HstCo)>(_MAX_PCO+F2_AD_CO))	//nastawa+hist > _MAX_PCO+F2_AD_CO?
                        {
                            if((DtCO.CO-_MAX_PCO)>=F2_AD_CO) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }
                        else	//nastawa <= 75?
                        {
                            if((DtCO.CO-M.CONTSETP)>=DtCO.HstCo) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }                         
                        break;
                    }
                    case 1:
                    {
                        if((M.CONTSETP+DtCO.HstCo)>(_MAX_PDCO+F2_AD_CO))	//nastawa+hist > _MAX_PDCO+F2_AD_CO?
                        {
                            if((DtCO.CO-_MAX_PDCO)>=F2_AD_CO) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }
                        else	//nastawa <= 75?
                        {
                            if((DtCO.CO-M.CONTSETP)>=DtCO.HstCo) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }                           
                         break;
                    } 
                    default:
                    {
                        if((M.CONTSETP+DtCO.HstCo)>(_MAX_PCO+F2_AD_CO))	//nastawa+hist > _MAX_PCO+F2_AD_CO?
                        {
                            if((DtCO.CO-_MAX_PCO)>=F2_AD_CO) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }
                        else	//nastawa <= 75?
                        {
                            if((DtCO.CO-M.CONTSETP)>=DtCO.HstCo) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }                            
                    } 
                }
            }
            else    //aktywne przewyzszenie temperaturowe
            {
                switch(DtKNF.opdg)
                {
                    case 0:
                    case 2:                            
                    {
                        if((M.CONTSETP+DtCO.HstCo)>F2_PWT_CO)	//nastawa+hist > F2_PW_CO?
                        {
                            if(DtCO.CO>=F2_PWT_CO) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }
                        else	
                        {
                            if((DtCO.CO-M.CONTSETP)>=DtCO.HstCo) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }                         
                        break;
                    }
                    case 1:
                    {
                        if((M.CONTSETP+DtCO.HstCo)>F2_PPWT_CO)	//nastawa+hist > F2_PPW_CO?
                        {
                            if(DtCO.CO>=F2_PPWT_CO) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }
                        else	
                        {
                            if((DtCO.CO-M.CONTSETP)>=DtCO.HstCo) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }                          
                        break;
                    } 
                    default:
                    {
                        if((M.CONTSETP+DtCO.HstCo)>F2_PWT_CO)	//nastawa+hist > F2_PW_CO?
                        {
                            if(DtCO.CO>=F2_PWT_CO) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }
                        else	
                        {
                            if((DtCO.CO-M.CONTSETP)>=DtCO.HstCo) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }                            
                    } 
                }                              
            }
            
			if((M.CONTSETP-DtCO.CO)>=F2_SU_CO) DtCO._su_CO=1;
			else DtCO._su_CO=0;
			if(DtCO.CO<=F2_PTWP_CO) DtCO._ptwp_CO=1;
			if(DtCO.CO>F2_PTWP_CO+1) DtCO._ptwp_CO=0;
			if(DtCO.CO<=F2_PZ_CO) DtCO._pz_CO=1;
			if(DtCO.CO>F2_PW_CO) DtCO._pz_CO=0;
			if(DtCO.CO<=F2_CLD_CO) DtCO._cold_CO=1;				
			if(DtCO.CO>F2_CLD_CO+1) DtCO._cold_CO=0;
			if(DtCO.CO>=F2_HOT_CO) DtCO._hot_CO=1;						
			if(DtCO.CO<F2_HOT_CO-1) DtCO._hot_CO=0;
			break;
		}
		case _UNICO:				//kociol unico
		{
			if(!M._LIN_PWD) //brak przewyzszenia temperaturowego   
            {
                switch(DtKNF.opdg)
                {
                    case 0:
                    case 2:                            
                    {
                        if((M.CONTSETP+DtCO.HstCo)>(_MAX_PCO+F3_AD_CO))	//nastawa+hist > _MAX_PCO+F2_AD_CO?
                        {
                            if((DtCO.CO-_MAX_PCO)>=F3_AD_CO) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }
                        else	//nastawa <= 75?
                        {
                            if((DtCO.CO-M.CONTSETP)>=DtCO.HstCo) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }                         
                        break;
                    }
                    case 1:
                    {
                        if((M.CONTSETP+DtCO.HstCo)>(_MAX_PDCO+F3_AD_CO))	//nastawa+hist > _MAX_PDCO+F2_AD_CO?
                        {
                            if((DtCO.CO-_MAX_PDCO)>=F3_AD_CO) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }
                        else	//nastawa <= 75?
                        {
                            if((DtCO.CO-M.CONTSETP)>=DtCO.HstCo) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }                          
                        break;
                    } 
                    default:
                    {
                        if((M.CONTSETP+DtCO.HstCo)>(_MAX_PCO+F3_AD_CO))	//nastawa+hist > _MAX_PCO+F2_AD_CO?
                        {
                            if((DtCO.CO-_MAX_PCO)>=F3_AD_CO) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }
                        else	//nastawa <= 75?
                        {
                            if((DtCO.CO-M.CONTSETP)>=DtCO.HstCo) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }                            
                    } 
                }                
            }
            else    //aktywne przewyzszenie temperaturowe
            {
                switch(DtKNF.opdg)
                {
                    case 0:
                    case 2:                            
                    {
                        if((M.CONTSETP+DtCO.HstCo)>F3_PWT_CO)	//nastawa+hist > F3_PW_CO?
                        {
                            if(DtCO.CO>=F3_PWT_CO) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }
                        else	
                        {
                            if((DtCO.CO-M.CONTSETP)>=DtCO.HstCo) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }                         
                        break;
                    }
                    case 1:
                    {
                        if((M.CONTSETP+DtCO.HstCo)>F3_PPWT_CO)	//nastawa+hist > F3_PPW_CO?
                        {
                            if(DtCO.CO>=F3_PPWT_CO) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }
                        else	
                        {
                            if((DtCO.CO-M.CONTSETP)>=DtCO.HstCo) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }                          
                        break;
                    } 
                    default:
                    {
                        if((M.CONTSETP+DtCO.HstCo)>F3_PWT_CO)	//nastawa+hist > F3_PW_CO?
                        {
                            if(DtCO.CO>=F3_PWT_CO) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }
                        else	
                        {
                            if((DtCO.CO-M.CONTSETP)>=DtCO.HstCo) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }                            
                    } 
                }                                
            }
            
			if((M.CONTSETP-DtCO.CO)>=F3_SU_CO) DtCO._su_CO=1;
			else DtCO._su_CO=0;

			if(DtCO.CO<=(DtCO.F3PPCO-2)) DtCO._ptwp_CO=1;
			if(DtCO.CO>((DtCO.F3PPCO-2)+1)) DtCO._ptwp_CO=0;
			if(DtCO.CO<=(DtCO.F3PPCO-2)) DtCO._pz_CO=1;
			if(DtCO.CO>(DtCO.F3PPCO+2)) DtCO._pz_CO=0;

			if(DtCO.CO<=F3_CLD_CO) DtCO._cold_CO=1;				
			if(DtCO.CO>F3_CLD_CO+1) DtCO._cold_CO=0;
			if(DtCO.CO>=F3_HOT_CO) DtCO._hot_CO=1;						
			if(DtCO.CO<F3_HOT_CO-1) DtCO._hot_CO=0;
			break;
		}
		default:		//domyslny monotermiczny
		{
			if(!M._LIN_PWD) //brak przewyzszenia temperaturowego   
            {
                switch(DtKNF.opdg)
                {
                    case 0:
                    case 2:                            
                    {
                        if((M.CONTSETP+DtCO.HstCo)>(_MAX_PCO+F2_AD_CO))	//nastawa+hist > _MAX_PCO+F2_AD_CO?
                        {
                            if((DtCO.CO-_MAX_PCO)>=F2_AD_CO) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }
                        else	//nastawa <= 75?
                        {
                            if((DtCO.CO-M.CONTSETP)>=DtCO.HstCo) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }                         
                        break;
                    }
                    case 1:
                    {
                        if((M.CONTSETP+DtCO.HstCo)>(_MAX_PDCO+F2_AD_CO))	//nastawa+hist > _MAX_PDCO+F2_AD_CO?
                        {
                            if((DtCO.CO-_MAX_PDCO)>=F2_AD_CO) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }
                        else	//nastawa <= 75?
                        {
                            if((DtCO.CO-M.CONTSETP)>=DtCO.HstCo) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }                           
                         break;
                    } 
                    default:
                    {
                        if((M.CONTSETP+DtCO.HstCo)>(_MAX_PCO+F2_AD_CO))	//nastawa+hist > _MAX_PCO+F2_AD_CO?
                        {
                            if((DtCO.CO-_MAX_PCO)>=F2_AD_CO) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }
                        else	//nastawa <= 75?
                        {
                            if((DtCO.CO-M.CONTSETP)>=DtCO.HstCo) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }                            
                    } 
                }
            }
            else    //aktywne przewyzszenie temperaturowe
            {
                switch(DtKNF.opdg)
                {
                    case 0:
                    case 2:                            
                    {
                        if((M.CONTSETP+DtCO.HstCo)>F2_PWT_CO)	//nastawa+hist > F2_PW_CO?
                        {
                            if(DtCO.CO>=F2_PWT_CO) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }
                        else	
                        {
                            if((DtCO.CO-M.CONTSETP)>=DtCO.HstCo) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }                         
                        break;
                    }
                    case 1:
                    {
                        if((M.CONTSETP+DtCO.HstCo)>F2_PPWT_CO)	//nastawa+hist > F2_PPW_CO?
                        {
                            if(DtCO.CO>=F2_PPWT_CO) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }
                        else	
                        {
                            if((DtCO.CO-M.CONTSETP)>=DtCO.HstCo) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }                          
                        break;
                    } 
                    default:
                    {
                        if((M.CONTSETP+DtCO.HstCo)>F2_PWT_CO)	//nastawa+hist > F2_PW_CO?
                        {
                            if(DtCO.CO>=F2_PWT_CO) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }
                        else	
                        {
                            if((DtCO.CO-M.CONTSETP)>=DtCO.HstCo) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }                            
                    } 
                }                              
            }
            
			if((M.CONTSETP-DtCO.CO)>=F2_SU_CO) DtCO._su_CO=1;
			else DtCO._su_CO=0;
			if(DtCO.CO<=F2_PTWP_CO) DtCO._ptwp_CO=1;
			if(DtCO.CO>F2_PTWP_CO+1) DtCO._ptwp_CO=0;
			if(DtCO.CO<=F2_PZ_CO) DtCO._pz_CO=1;
			if(DtCO.CO>F2_PW_CO) DtCO._pz_CO=0;
			if(DtCO.CO<=F2_CLD_CO) DtCO._cold_CO=1;				
			if(DtCO.CO>F2_CLD_CO+1) DtCO._cold_CO=0;
			if(DtCO.CO>=F2_HOT_CO) DtCO._hot_CO=1;						
			if(DtCO.CO<F2_HOT_CO-1) DtCO._hot_CO=0;
			break;
		}						
	}

	if(DtCO.CO<=_ON_DZ_CO) 
	{
		DtCO._su_DZ_CO=1;
		DtCO._on_DZ_CO=1;
	}
	else DtCO._su_DZ_CO=0;			
	if(DtCO.CO>=_OFF_DZ_CO)
	{
		DtCO._ad_DZ_CO=1;
		DtCO._on_DZ_CO=0;
	}
	else DtCO._ad_DZ_CO=0;

    switch(DtKNF.opdg)
    {
        case 0:
        case 2:
        {                
            if(DtCO.CO<=_ON_SRW_CO) DtCO._su_SRW_CO=1;
            else DtCO._su_SRW_CO=0;			
            if(DtCO.CO>_OFF_SRW_CO) DtCO._ad_SRW_CO=1;
            else DtCO._ad_SRW_CO=0;
            break;
        }
        case 1:
        {
            if(DtCO.CO<=_ONP_SRW_CO) DtCO._su_SRW_CO=1;
            else DtCO._su_SRW_CO=0;			
            if(DtCO.CO>_OFFP_SRW_CO) DtCO._ad_SRW_CO=1;
            else DtCO._ad_SRW_CO=0;
            break;
        }
        default:
        {
            if(DtCO.CO<=_ON_SRW_CO) DtCO._su_SRW_CO=1;
            else DtCO._su_SRW_CO=0;			
            if(DtCO.CO>_OFF_SRW_CO) DtCO._ad_SRW_CO=1;
            else DtCO._ad_SRW_CO=0;
            break;                
        }
    }    
	/*if(!DtKNF.opdg)
	{
		if(DtCO.CO<=_ON_SRW_CO) DtCO._su_SRW_CO=1;
		else DtCO._su_SRW_CO=0;			
		if(DtCO.CO>_OFF_SRW_CO) DtCO._ad_SRW_CO=1;
		else DtCO._ad_SRW_CO=0;	
	}
	else		//ogrzewanie podlogowe
	{
		if(DtCO.CO<=_ONP_SRW_CO) DtCO._su_SRW_CO=1;
		else DtCO._su_SRW_CO=0;			
		if(DtCO.CO>_OFFP_SRW_CO) DtCO._ad_SRW_CO=1;
		else DtCO._ad_SRW_CO=0;	
	}*/
}
#else
void PomiarCO(void)
{
    volatile static float avr=0;
    volatile unsigned int AC_DTA;
    volatile unsigned char AC_AVR;
    
    AC_DTA=ReadAC16(_CHN_CO);                       //pomiar 16bit
    DtCO.HstCo=DtKNF.hsco;
	DtCO.F3PPCO=DtKNF.conw;
    //-------------------------------------
    //wartosc srednia
    if(!avr)
    {
        avr = (float)AC_DTA;
    }
    else
    {
        avr = avr + ((float)AC_DTA - avr) / 50;             //wartosc srednia pomiaru
    }
    //-------------------------------------
    //reakcja na zwarcie/rozwarcie
	AC_AVR=(unsigned char)(((unsigned int)avr)>>8);
    if(AC_AVR<=_NTC_MIN||AC_AVR>=_NTC_MAX) 		//stwierdzenie awarii czujnika temp. CO
	{
		DtCO._err_CO=1;
		DtCO._su_CO=0;
		DtCO._ad_CO=0;
		DtCO._cold_CO=0;
		DtCO._hot_CO=0;
		DtCO._on_DZ_CO=0;
		DtCO._su_DZ_CO=0;
		DtCO._ad_DZ_CO=0;
		DtCO._ptwp_CO=0;
		DtCO._pz_CO=0;
		DtCO._su_SRW_CO=0;
		DtCO._ad_SRW_CO=0;
		RSDT1.outrCO=0;
		return;
	}	
	else
    {
        if(DtCO._err_CO)
        {
            avr=0;
        }
        DtCO._err_CO=0;
    }
       //-------------------------------------
    //wynik pomiaru
    DtCO.AC_CO=AC_AVR;
    DtCO.CO=ReadTabNTC(DtCO.AC_CO);
    
	/*DtCO.AC_CO=ReadAC(_CHN_CO);
	DtCO.CO=ReadTabNTC(DtCO.AC_CO);
	DtCO.HstCo=DtKNF.hsco;
	DtCO.F3PPCO=DtKNF.conw;
	if(DtCO.AC_CO<=_NTC_MIN||DtCO.AC_CO>=_NTC_MAX) 		//stwierdzenie awarii czujnika temp. CO
	{
		DtCO._err_CO=1;
		DtCO._su_CO=0;
		DtCO._ad_CO=0;
		DtCO._cold_CO=0;
		DtCO._hot_CO=0;
		DtCO._on_DZ_CO=0;
		DtCO._su_DZ_CO=0;
		DtCO._ad_DZ_CO=0;
		DtCO._ptwp_CO=0;
		DtCO._pz_CO=0;
		DtCO._su_SRW_CO=0;
		DtCO._ad_SRW_CO=0;
		RSDT1.outrCO=0;
		return;
	}	
	else DtCO._err_CO=0;*/
	RSDT1.outrCO=DtCO.CO;
	switch(DtKNF.tpkt)		//dla danego rodzaju kotla
	{
		case _MONOHB:
		{           
			if(!M._LIN_PWD) //brak przewyzszenia temperaturowego   
            {
                switch(DtKNF.opdg)
                {
                    case 0:
                    case 2:                            
                    {
                        if((DtCO.PPCO+DtCO.HstCo)>(_MAX_PCO+F2_AD_CO))	//nastawa+hist > _MAX_PCO+F2_AD_CO?
                        {
                            if((DtCO.CO-_MAX_PCO)>=F2_AD_CO) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }
                        else	//nastawa <= 75?
                        {
                            if((DtCO.CO-DtCO.PPCO)>=DtCO.HstCo) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }                         
                        break;
                    }
                    case 1:
                    {
                        if((DtCO.PPCO+DtCO.HstCo)>(_MAX_PDCO+F2_AD_CO))	//nastawa+hist > _MAX_PDCO+F2_AD_CO?
                        {
                            if((DtCO.CO-_MAX_PDCO)>=F2_AD_CO) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }
                        else	//nastawa <= 75?
                        {
                            if((DtCO.CO-DtCO.PPCO)>=DtCO.HstCo) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }                           
                         break;
                    } 
                    default:
                    {
                        if((DtCO.PPCO+DtCO.HstCo)>(_MAX_PCO+F2_AD_CO))	//nastawa+hist > _MAX_PCO+F2_AD_CO?
                        {
                            if((DtCO.CO-_MAX_PCO)>=F2_AD_CO) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }
                        else	//nastawa <= 75?
                        {
                            if((DtCO.CO-DtCO.PPCO)>=DtCO.HstCo) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }                            
                    } 
                }                
            }
            else    //aktywne przewyzszenie temperaturowe
            {
                switch(DtKNF.opdg)
                {
                    case 0:
                    case 2:                            
                    {
                        if((M.CONTSETP+DtCO.HstCo)>F2_PWT_CO)	//nastawa+hist > F2_PW_CO?
                        {
                            if(DtCO.CO>=F2_PWT_CO) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }
                        else	
                        {
                            if((DtCO.CO-M.CONTSETP)>=DtCO.HstCo) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }                          
                        break;
                    }
                    case 1:
                    {
                        if((M.CONTSETP+DtCO.HstCo)>F2_PPWT_CO)	//nastawa+hist > F2_PPW_CO?
                        {
                            if(DtCO.CO>=F2_PPWT_CO) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }
                        else	
                        {
                            if((DtCO.CO-M.CONTSETP)>=DtCO.HstCo) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }                           
                        break;
                    } 
                    default:
                    {
                        if((M.CONTSETP+DtCO.HstCo)>F2_PWT_CO)	//nastawa+hist > F2_PW_CO?
                        {
                            if(DtCO.CO>=F2_PWT_CO) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }
                        else	
                        {
                            if((DtCO.CO-M.CONTSETP)>=DtCO.HstCo) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }                            
                    } 
                }                                    
            }
            
			if((DtCO.PPCO-DtCO.CO)>=F2_SU_CO) DtCO._su_CO=1;
			else DtCO._su_CO=0;
			if(DtCO.CO<=F2_PTWP_CO) DtCO._ptwp_CO=1;
			if(DtCO.CO>F2_PTWP_CO+1) DtCO._ptwp_CO=0;
			if(DtCO.CO<=F2_PZ_CO) DtCO._pz_CO=1;
			if(DtCO.CO>F2_PW_CO) DtCO._pz_CO=0;
			if(DtCO.CO<=F2_CLD_CO) DtCO._cold_CO=1;				
			if(DtCO.CO>F2_CLD_CO+1) DtCO._cold_CO=0;
			if(DtCO.CO>=F2_HOT_CO) DtCO._hot_CO=1;						
			if(DtCO.CO<F2_HOT_CO-1) DtCO._hot_CO=0;
			break;
		}
		case _UNICO:				//kociol unico
		{
			if(!M._LIN_PWD) //brak przewyzszenia temperaturowego   
            {
                switch(DtKNF.opdg)
                {
                    case 0:
                    case 2:                            
                    {
                        if((DtCO.PPCO+DtCO.HstCo)>(_MAX_PCO+F3_AD_CO))	//nastawa+hist > _MAX_PCO+F2_AD_CO?
                        {
                            if((DtCO.CO-_MAX_PCO)>=F3_AD_CO) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }
                        else	//nastawa <= 75?
                        {
                            if((DtCO.CO-DtCO.PPCO)>=DtCO.HstCo) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }                         
                        break;
                    }
                    case 1:
                    {
                        if((DtCO.PPCO+DtCO.HstCo)>(_MAX_PDCO+F3_AD_CO))	//nastawa+hist > _MAX_PDCO+F2_AD_CO?
                        {
                            if((DtCO.CO-_MAX_PDCO)>=F3_AD_CO) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }
                        else	//nastawa <= 75?
                        {
                            if((DtCO.CO-DtCO.PPCO)>=DtCO.HstCo) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }                           
                         break;
                    } 
                    default:
                    {
                        if((DtCO.PPCO+DtCO.HstCo)>(_MAX_PCO+F3_AD_CO))	//nastawa+hist > _MAX_PCO+F2_AD_CO?
                        {
                            if((DtCO.CO-_MAX_PCO)>=F3_AD_CO) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }
                        else	//nastawa <= 75?
                        {
                            if((DtCO.CO-DtCO.PPCO)>=DtCO.HstCo) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }                            
                    } 
                }                 
            }
            else    //aktywne przewyzszenie temperaturowe
            {
                switch(DtKNF.opdg)
                {
                    case 0:
                    case 2:                            
                    {
                        if((M.CONTSETP+DtCO.HstCo)>F3_PWT_CO)	//nastawa+hist > F2_PW_CO?
                        {
                            if(DtCO.CO>=F3_PWT_CO) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }
                        else	
                        {
                            if((DtCO.CO-M.CONTSETP)>=DtCO.HstCo) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }                          
                        break;
                    }
                    case 1:
                    {
                        if((M.CONTSETP+DtCO.HstCo)>F3_PPWT_CO)	//nastawa+hist > F2_PPW_CO?
                        {
                            if(DtCO.CO>=F3_PPWT_CO) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }
                        else	
                        {
                            if((DtCO.CO-M.CONTSETP)>=DtCO.HstCo) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }                           
                        break;
                    } 
                    default:
                    {
                        if((M.CONTSETP+DtCO.HstCo)>F3_PWT_CO)	//nastawa+hist > F2_PW_CO?
                        {
                            if(DtCO.CO>=F3_PWT_CO) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }
                        else	
                        {
                            if((DtCO.CO-M.CONTSETP)>=DtCO.HstCo) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }                            
                    } 
                }                
            }            
            
			if((DtCO.PPCO-DtCO.CO)>=F3_SU_CO) DtCO._su_CO=1;
			else DtCO._su_CO=0;

			if(DtCO.CO<=(DtCO.F3PPCO-2)) DtCO._ptwp_CO=1;
			if(DtCO.CO>((DtCO.F3PPCO-2)+1)) DtCO._ptwp_CO=0;
			if(DtCO.CO<=(DtCO.F3PPCO-2)) DtCO._pz_CO=1;
			if(DtCO.CO>(DtCO.F3PPCO+2)) DtCO._pz_CO=0;

			if(DtCO.CO<=F3_CLD_CO) DtCO._cold_CO=1;				
			if(DtCO.CO>F3_CLD_CO+1) DtCO._cold_CO=0;
			if(DtCO.CO>=F3_HOT_CO) DtCO._hot_CO=1;						
			if(DtCO.CO<F3_HOT_CO-1) DtCO._hot_CO=0;
			break;
		}
		default:		//domyslny monotermiczny
		{
			if(!M._LIN_PWD) //brak przewyzszenia temperaturowego   
            {
                switch(DtKNF.opdg)
                {
                    case 0:
                    case 2:                            
                    {
                        if((DtCO.PPCO+DtCO.HstCo)>(_MAX_PCO+F2_AD_CO))	//nastawa+hist > _MAX_PCO+F2_AD_CO?
                        {
                            if((DtCO.CO-_MAX_PCO)>=F2_AD_CO) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }
                        else	//nastawa <= 75?
                        {
                            if((DtCO.CO-DtCO.PPCO)>=DtCO.HstCo) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }                         
                        break;
                    }
                    case 1:
                    {
                        if((DtCO.PPCO+DtCO.HstCo)>(_MAX_PDCO+F2_AD_CO))	//nastawa+hist > _MAX_PDCO+F2_AD_CO?
                        {
                            if((DtCO.CO-_MAX_PDCO)>=F2_AD_CO) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }
                        else	//nastawa <= 75?
                        {
                            if((DtCO.CO-DtCO.PPCO)>=DtCO.HstCo) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }                           
                         break;
                    } 
                    default:
                    {
                        if((DtCO.PPCO+DtCO.HstCo)>(_MAX_PCO+F2_AD_CO))	//nastawa+hist > _MAX_PCO+F2_AD_CO?
                        {
                            if((DtCO.CO-_MAX_PCO)>=F2_AD_CO) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }
                        else	//nastawa <= 75?
                        {
                            if((DtCO.CO-DtCO.PPCO)>=DtCO.HstCo) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }                            
                    } 
                }                
            }
            else    //aktywne przewyzszenie temperaturowe
            {
                switch(DtKNF.opdg)
                {
                    case 0:
                    case 2:                            
                    {
                        if((M.CONTSETP+DtCO.HstCo)>F2_PWT_CO)	//nastawa+hist > F2_PW_CO?
                        {
                            if(DtCO.CO>=F2_PWT_CO) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }
                        else	
                        {
                            if((DtCO.CO-M.CONTSETP)>=DtCO.HstCo) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }                          
                        break;
                    }
                    case 1:
                    {
                        if((M.CONTSETP+DtCO.HstCo)>F2_PPWT_CO)	//nastawa+hist > F2_PPW_CO?
                        {
                            if(DtCO.CO>=F2_PPWT_CO) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }
                        else	
                        {
                            if((DtCO.CO-M.CONTSETP)>=DtCO.HstCo) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }                           
                        break;
                    } 
                    default:
                    {
                        if((M.CONTSETP+DtCO.HstCo)>F2_PWT_CO)	//nastawa+hist > F2_PW_CO?
                        {
                            if(DtCO.CO>=F2_PWT_CO) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }
                        else	
                        {
                            if((DtCO.CO-M.CONTSETP)>=DtCO.HstCo) DtCO._ad_CO=1;
                            else DtCO._ad_CO=0;
                        }                            
                    } 
                }                                    
            }
            
			if((DtCO.PPCO-DtCO.CO)>=F2_SU_CO) DtCO._su_CO=1;
			else DtCO._su_CO=0;
			if(DtCO.CO<=F2_PTWP_CO) DtCO._ptwp_CO=1;
			if(DtCO.CO>F2_PTWP_CO+1) DtCO._ptwp_CO=0;
			if(DtCO.CO<=F2_PZ_CO) DtCO._pz_CO=1;
			if(DtCO.CO>F2_PW_CO) DtCO._pz_CO=0;
			if(DtCO.CO<=F2_CLD_CO) DtCO._cold_CO=1;				
			if(DtCO.CO>F2_CLD_CO+1) DtCO._cold_CO=0;
			if(DtCO.CO>=F2_HOT_CO) DtCO._hot_CO=1;						
			if(DtCO.CO<F2_HOT_CO-1) DtCO._hot_CO=0;
			break;
		}						
	}

	if(DtCO.CO<=_ON_DZ_CO) 
	{
		DtCO._su_DZ_CO=1;
		DtCO._on_DZ_CO=1;
	}
	else DtCO._su_DZ_CO=0;			
	if(DtCO.CO>=_OFF_DZ_CO)
	{
		DtCO._ad_DZ_CO=1;
		DtCO._on_DZ_CO=0;
	}
	else DtCO._ad_DZ_CO=0;

    switch(DtKNF.opdg)
    {
        case 0:
        case 2:
        {                
            if(DtCO.CO<=_ON_SRW_CO) DtCO._su_SRW_CO=1;
            else DtCO._su_SRW_CO=0;			
            if(DtCO.CO>_OFF_SRW_CO) DtCO._ad_SRW_CO=1;
            else DtCO._ad_SRW_CO=0;
            break;
        }
        case 1:
        {
            if(DtCO.CO<=_ONP_SRW_CO) DtCO._su_SRW_CO=1;
            else DtCO._su_SRW_CO=0;			
            if(DtCO.CO>_OFFP_SRW_CO) DtCO._ad_SRW_CO=1;
            else DtCO._ad_SRW_CO=0;
            break;
        }
        default:
        {
            if(DtCO.CO<=_ON_SRW_CO) DtCO._su_SRW_CO=1;
            else DtCO._su_SRW_CO=0;			
            if(DtCO.CO>_OFF_SRW_CO) DtCO._ad_SRW_CO=1;
            else DtCO._ad_SRW_CO=0;
            break;                
        }
    } 
    
/*	if(!DtKNF.opdg)
	{
		if(DtCO.CO<=_ON_SRW_CO) DtCO._su_SRW_CO=1;
		else DtCO._su_SRW_CO=0;			
		if(DtCO.CO>_OFF_SRW_CO) DtCO._ad_SRW_CO=1;
		else DtCO._ad_SRW_CO=0;	
	}
	else		//ogrzewanie podlogowe
	{
		if(DtCO.CO<=_ONP_SRW_CO) DtCO._su_SRW_CO=1;
		else DtCO._su_SRW_CO=0;			
		if(DtCO.CO>_OFFP_SRW_CO) DtCO._ad_SRW_CO=1;
		else DtCO._ad_SRW_CO=0;	
	}
 */
}
#endif
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Pomiar wartosci temperatury na wlocie i ustawienie odpowiednich flag stanu
void PomiarIN(void)
{
    volatile static float avr=0;
    volatile unsigned int AC_DTA;
    volatile unsigned char AC_AVR;
    
    AC_DTA=ReadAC16(_CHN_IN);                       //pomiar 16bit
    //-------------------------------------
    //wartosc srednia
    if(!avr)
    {
        avr = (float)AC_DTA;
    }
    else
    {
        avr = avr + ((float)AC_DTA - avr) / 50;             //wartosc srednia pomiaru
    }
    //-------------------------------------
    //reakcja na zwarcie/rozwarcie
	AC_AVR=(unsigned char)(((unsigned int)avr)>>8);
    if(AC_AVR<=_NTC_MIN||AC_AVR>=_NTC_MAX) 		//stwierdzenie awarii czujnika temp. CO
	{
        DtIN._err_IN=1;
		RSDT1.outIN=0;
		return;
	}	
	else
    {
        if(DtIN._err_IN)
        {
            avr=0;
        }
        DtIN._err_IN=0;
    }
       //-------------------------------------
    //wynik pomiaru
    DtIN.AC_IN=AC_AVR;
    DtIN.IN=ReadTabNTC(DtIN.AC_IN);
	/*DtIN.AC_IN=ReadAC(_CHN_IN);
	DtIN.IN=ReadTabNTC(DtIN.AC_IN);
	
	if(DtIN.AC_IN<=_NTC_MIN||DtIN.AC_IN>=_NTC_MAX) 		//stwierdzenie awarii czujnika temp. IN
	{
		DtIN._err_IN=1;
		return;
	}	
	else DtIN._err_IN=0;*/
	RSDT1.outIN=DtIN.IN;
}
/*---------------------------------------------------------------------------------*/
//Pomiar wartosci CW i ustawienie odpowiednich flag stanu
void PomiarCW(void)
{
    volatile static float avr=0;
    volatile unsigned int AC_DTA;
    volatile unsigned char AC_AVR;
    
    volatile unsigned char tmz;    

    AC_DTA=ReadAC16(_CHN_CW);                       //pomiar 16bit
    //-------------------------------------
    //wartosc srednia
    if(!avr)
    {
        avr = (float)AC_DTA;
    }
    else
    {
        avr = avr + ((float)AC_DTA - avr) / 50;             //wartosc srednia pomiaru
    }
    //-------------------------------------
    //reakcja na zwarcie/rozwarcie
	AC_AVR=(unsigned char)(((unsigned int)avr)>>8);
    if(AC_AVR<=_NTC_MIN||AC_AVR>=_NTC_MAX) 		//stwierdzenie awarii czujnika temp. CO
	{
		DtCW._err_CW=1;
		DtCW._su_CW=0;
		DtCW._ad_CW=0;
		DtCW._cold_CW=0;
		DtCW._on_DZ_CW=0;
		DtCW._su_DZ_CW=0;
		DtCW._ad_DZ_CW=0;
		DtCW._wpz_CW=0;
		RSDT1.outrCW=0;
		return;
	}	
	else
    {
        if(DtCW._err_CW)
        {
            avr=0;
        }
        DtCW._err_CW=0;
    }
       //-------------------------------------
    //wynik pomiaru
    DtCW.AC_CW=AC_AVR;
    DtCW.CW=ReadTabNTC(DtCW.AC_CW);
	/*DtCW.AC_CW=ReadAC(_CHN_CW);
	DtCW.CW=ReadTabNTC(DtCW.AC_CW);
	
	if(DtCW.AC_CW<=_NTC_MIN||DtCW.AC_CW>=_NTC_MAX) 		//stwierdzenie awarii czujnika temp. CO
	{
		DtCW._err_CW=1;
		DtCW._su_CW=0;
		DtCW._ad_CW=0;
		DtCW._cold_CW=0;
		DtCW._on_DZ_CW=0;
		DtCW._su_DZ_CW=0;
		DtCW._ad_DZ_CW=0;
		DtCW._wpz_CW=0;
		RSDT1.outrCW=DtCW.CW;
		return;
	}	
	else DtCW._err_CW=0;*/
	RSDT1.outrCW=DtCW.CW;
	switch(DtKNF.tpkt)			//dla danego rodzaju kotla
	{
		case _MONOHB:
		{
            if(!DtKNF.cwmg)     //stala granica zalaczenia/wylaczenia
            {
                if(DtCW.CW>F2_OFF_CW) DtCW._ad_CW=1;
                else DtCW._ad_CW=0;
                if(DtCW.CW<=F2_OFF_CW-F2_MR_OFF_CW) DtCW._su_CW=1;
                else DtCW._su_CW=0;
            }
            else                //granica zalaczenia/wylaczenia zalezna od nastawy
            {
                if((DtCW.CW-DtCW.PPCW)>=F2_AD_CW) DtCW._ad_CW=1;
                else DtCW._ad_CW=0;
                if((DtCW.PPCW-DtCW.CW)>=F2_SU_CW) DtCW._su_CW=1;
                else DtCW._su_CW=0;                
            }
			break;			
		}
		case _UNICO:				//kociol unico
		{
			if((DtCW.CW-DtCW.PPCW)>=F3_AD_CW) DtCW._ad_CW=1;
			else DtCW._ad_CW=0;
			if((DtCW.PPCW-DtCW.CW)>=F3_SU_CW) DtCW._su_CW=1;
			else DtCW._su_CW=0;
            if(!DtKNF.cwtm)     //(timera zasobnika||LIN_CW||OT_CW)
            {
                tmz=RdPrt(S_WCW);   //(styk timera zasobnika||LIN_CW||OT_CW)
            }   
            else                //timera zasobnika
            {
                tmz=RdPrt(S_TIM);   //styk timera zasobnika
            }  
            if(tmz&&(DtCW.PCW>=DtCW.minPCW))	//aktywne pobudzenie z (timera zasobnika||LIN_CW||OT_CW) i nieaktywna blokada grzania?
            {
                if (DtCW._su_CW) DtCW._wpz_CW=1;
                else if(!M._PRACA) DtCW._wpz_CW=0;
                if (DtCW._ad_CW) DtCW._wpz_CW=0;
            }
            else DtCW._wpz_CW=0;  
			break;
		}
		default:		//domyslny monotermiczny
		{
            if(!DtKNF.cwmg)     //stala granica zalaczenia/wylaczenia
            {
                if(DtCW.CW>F2_OFF_CW) DtCW._ad_CW=1;
                else DtCW._ad_CW=0;
                if(DtCW.CW<=F2_OFF_CW-F2_MR_OFF_CW) DtCW._su_CW=1;
                else DtCW._su_CW=0;
            }
            else                //stala granica zalaczenia/wylaczenia zalezna od nastawy
            {
                if((DtCW.CW-DtCW.PPCW)>=F2_AD_CW) DtCW._ad_CW=1;
                else DtCW._ad_CW=0;
                if((DtCW.PPCW-DtCW.CW)>=F2_SU_CW) DtCW._su_CW=1;
                else DtCW._su_CW=0;                
            }
			break;	
		}
	}
    //granica powyzej ktorej zawor 3DR wysterowany jest tylko na obieg CO, powrót do normalnej pracy po spadku temp. poni¿ej tego progu
    if(DtKNF.tpkt==_UNICO)
    {
        if(DtCW.CW>_OFF_KCW) DtCW._offKCW=1;
        if(DtCW.CW<_OFF_KCW) DtCW._offKCW=0;        
    }
    else 
    {
        DtCW._offKCW=0;    
    }
	if(DtCW.CW<=_ON_DZ_CW) 
	{
		DtCW._su_DZ_CW=1;
		DtCW._on_DZ_CW=1;
	}
	else DtCW._su_DZ_CW=0;			
	if(DtCW.CW>=_OFF_DZ_CW)
	{
		DtCW._ad_DZ_CW=1;
		DtCW._on_DZ_CW=0;
	}
	else DtCW._ad_DZ_CW=0;
	if(DtCW.CW<=_CLD_CW) DtCW._cold_CW=1;				
	if(DtCW.CW>_CLD_CW+1) DtCW._cold_CW=0;
}
/*---------------------------------------------------------------------------------*/
//Pomiar wartosci temperatury zewnetrznej i ustawienie odpowiednich flag stanu
void PomiarZW(void)
{
    volatile static float avr=0;
    volatile unsigned int AC_DTA;
    volatile unsigned char AC_AVR;

    AC_DTA=ReadAC16(_CHN_ZW);                       //pomiar 16bit
    //-------------------------------------
    //wartosc srednia
    if(!avr)
    {
        avr = (float)AC_DTA;
    }
    else
    {
        avr = avr + ((float)AC_DTA - avr) / 50;             //wartosc srednia pomiaru
    }
    //-------------------------------------
    //reakcja na zwarcie/rozwarcie
	AC_AVR=(unsigned char)(((unsigned int)avr)>>8);
    if(AC_AVR<=_ZNTC_MIN||AC_AVR>=_ZNTC_MAX) 		//stwierdzenie awarii czujnika temp. CO
	{
        DtZW._en_ZW=0;         //brak czujnika temp. zew.
        DtZW._on_ZW=0;         //brak czujnika temp. zew. 
        M._pob_PGDRT=0;
        RSDT1.outZW=0;        
		return;
	}	
	else
    {
        if(!DtZW._en_ZW)
        {
            avr=0;
        }
        DtZW._en_ZW=1;
        if(RdPrt(S_EN_ZW))
        {
            DtZW.AC_ZW=AC_AVR;
            DtZW.ZW=ReadTabZNTC(DtZW.AC_ZW); 
            RSDT1.outZW=DtZW.ZW;
            if(DtKNF.pgdr)         //konfiguracja zezwala na aktywacje regulatora pogodowego?
            {
    #if (_EXT_BOARD<=1)        
                DtZW._on_ZW=1;                                                      //stwierdzenie obecnosci czujnika temp. zew.- aktywacja regulatora pogodowego
    #endif
    #if (_EXT_BOARD==2) 
                if(!M._AKTYWNE_PGD2)   
                {        
                    if(M._LIN_DPGD) DtZW._on_ZW=0;                                      //ignoruj obecnosc czujnika temp. zew.
                    else DtZW._on_ZW=1;                                                 //stwierdzenie obecnosci czujnika temp. zew.- aktywacja regulatora pogodowego \
                }
                else DtZW._on_ZW=1;       
    #endif
    #if (_EXT_BOARD>=3)  
                if(!M._AKTYWNE_PGD2)   
                {
                    if(M._LIN_DPGD||M._IOT_DPGD) DtZW._on_ZW=0;                         //ignoruj obecnosc czujnika temp. zew.  
                    else DtZW._on_ZW=1;                                                 //stwierdzenie obecnosci czujnika temp. zew.- aktywacja regulatora pogodowego     
                }
                else DtZW._on_ZW=1;

                if(DtKNF.pgdr==3)                                                   //parametr P25 == 3?
                {
                    DtZW._on_ZW=1;                                                  //regulator tradycyjny (jak dla DtKNF.pgdr==1) realizowany niezaleznie od aktywnosci OT czy LIN
                }
    #endif                
                if(M._AKTYWNE_PGD2)                                                 //niezalezna praca regulatora pogodowego?
                {
                    if(RdPrt(S_PGD))                                                 //czujnik zew. pracuje dla wew. regulatora pogodowego?
                    {
                        if(DtZW.ZW<DtKNF.pgdg) M._pob_PGDRT=1;                      //temp. zew < prog zalaczenia
    #if _TESTPGD==1
                        if(DtZW.ZW>DtKNF.pgdg) M._pob_PGDRT=0;                              //temp. zew > prog wylaczenia
    #else                
                        if(DtZW.ZW>DtKNF.pgdg)
                        {
                            if(RTG(_RTPGD2)>=_TPGDOFF) M._pob_PGDRT=0;  //temp. zew > prog wylaczenia
                        }
                        else
                        {
                            StartRTG(_RTPGD2);
                        }
    #endif
                    }
                    else
                    {
                        M._pob_PGDRT=0;
                    }
                }
                else
                {
                    M._pob_PGDRT=0;
                }
            }
            else
            {
                DtZW._on_ZW=0;
                M._pob_PGDRT=0;
            }
        }
        else
        {
            SetPrt(S_PGD,0);    
        }
    }
}
/*---------------------------------------------------------------------------------*/
unsigned char GetPGD_PCO(void);    
//Decyzja o aktywacji/dezaktywacji regulatora pogodowego    
void SetTrbPGD(void)
{
    volatile unsigned char dt;
    
    //regulacja pogodowa aktywna?
    dt=(RdPrt(S_PGD))?1:0;  //DtZW._on_ZW
    if(!M._SYNCH_PGD)
    {
        if(M._AKTYWNE_PGD!=dt)
        {
            if(!M._AKTYWNE_PGD) //aktywacja PGD?
            {        
                if(RdPrt(S_EN_ZW))
                {
                    M._AKTYWNE_PGD=1;
                    M._AKTYWNE_PGD2=(DtKNF.pgdr==2)?1:0;
                    DtCO.PCO=ReadEEPPGD();   //odczyt nastawy z pamieci EEPROM
                    RSDT1.outPCO=DtCO.PCO;
                    SetMinMaxPGD();    
                    ChkPGD();
                    DtCO.PGDPCO=GetPGD_PCO();
                    //dobierz wartosci M.CONTSETP, M.SETPmax, DtCO.PPCO
                    SetModPCO();
                    RSDT1.outSETPmax=M.SETPmax; 
                    RSDT1.outSETPPmax=M.SETPPmax;                    
                    StartRTS(_RTCPGD);
                    M._SYNCH_PGD=1;             //ustaw flage aktualizacji nastawy w plytce wyswietlacza
                    return;                    
                }
            }
            else                //dezaktywacja PGD
            {
                M._AKTYWNE_PGD=0;
                M._AKTYWNE_PGD2=0;
                DtCO.PCO=ReadEEPCO();    //odczyt nastawy z pamieci EEPROM
                RSDT1.outPCO=DtCO.PCO;                
                SetMinMaxPCO();
                ChkPCO();
                //dobierz wartosci M.CONTSETP, M.SETPmax, DtCO.PPCO
                SetModPCO();
                RSDT1.outSETPmax=M.SETPmax;
                RSDT1.outSETPPmax=M.SETPPmax;                
                StartRTS(_RTCPGD);
                M._SYNCH_PGD=1;             //ustaw flage aktualizacji nastawy w plytce wyswietlacza
                return;                    
            }
        }
    }
    
    M._AKTYWNE_PGD2=(M._AKTYWNE_PGD&&(DtKNF.pgdr==2))?1:0;    
    if(RTS(_RTCPGD)>2)              //uplynal czas na synchronizacje nastaw?
    {
        M._SYNCH_PGD=0;             //zeruj flage aktualizacji nastawy w plytce wyswietlacza
    }
}    
/*---------------------------------------------------------------------------------*/
//Formatowanie nastawy CO
void SetModPCO(void)
{
    volatile unsigned char ppc;

	if(M._AKTYWNE_KNF)
	{
        switch(DtKNF.opdg)
        {
            case 0:
            case 2:
            {                
                DtCO.PPCO=_KNF_PCO;		//aktywna konfiguracja
                break;
            }
            case 1:
            {
                DtCO.PPCO=_MAX_PDCO;				//dla ogrzewania podlogowego
                break;
            }
            default:
            {
                DtCO.PPCO=_KNF_PCO;		//aktywna konfiguracja
                break;                
            }
        }
    
 		/*if(!DtKNF.opdg) DtCO.PPCO=_KNF_PCO;		//aktywna konfiguracja
		else DtCO.PPCO=_MAX_PDCO;				//dla ogrzewania podlogowego
         */
        M.CONTSETP=DtCO.PPCO;
        M.SETPmax=DtCO.PPCO;
        M.SETPPmax=M.SETPmax;        
	}
	else
	if(M._pob_CW||M._pob_AL||M._pob_DzCW)								//pobudzenie CW?
	{
		switch(DtKNF.tpkt)		//dla danego rodzaju kotla
		{
			case _MONOHB:
			{
				DtCO.PPCO=F2_PPCO;
				break;
			}
			case _UNICO:				//kociol unico
			{
				if(M._pob_CW||M._pob_AL)
				{
					//DtCO.PPCO=F3_PPCO;
					DtCO.PPCO=DtCO.F3PPCO;
				}
				else
				{
					//DtCO.PPCO=F3_PW_CO;
					DtCO.PPCO=DtCO.F3PPCO+2;
				}
				break;
			}
			default:					//domyslny monotermiczny			
			{
				DtCO.PPCO=F2_PPCO;
				break;
			}
		}
        M.CONTSETP=DtCO.PPCO;
        M.SETPmax=DtCO.PPCO;
        M.SETPPmax=M.SETPmax;        
	}
	else
	if(M._AKTYWNE_PGD)
    {       
        if(M._LIN_PWD)
        {
            ppc=DtCO.PGDPCO+DtKNF.pwtm; 		//aktywna funkcja pogodowa
        }
        else
        {
            ppc=DtCO.PGDPCO;
        }
        switch(DtKNF.opdg)
        {
            case 0:
            case 2:
            {                
                if(ppc>_MXPW_CO) ppc=_MXPW_CO;          //wartosci >85 sa sprowadzane do wartosci 85
                break;
            }
            case 1:
            {
                if(ppc>_MXPPW_CO) ppc=_MXPPW_CO;          //wartosci >85 sa sprowadzane do wartosci 85
                break;
            }
            default:
            {
                if(ppc>_MXPW_CO) ppc=_MXPW_CO;          //wartosci >85 sa sprowadzane do wartosci 85
                break;                
            }
        }         
        /*if(!DtKNF.opdg)
        {
            if(ppc>_MXPW_CO) ppc=_MXPW_CO;          //wartosci >85 sa sprowadzane do wartosci 85    
        }
        else
        {
            if(ppc>_MXPPW_CO) ppc=_MXPPW_CO;          //wartosci >85 sa sprowadzane do wartosci 85                    
        }*/
        DtCO.PPCO=DtCO.PGDPCO;		//aktywna funkcja pogodowa
        M.SETPmax=DtCO.PPCO;      
        M.SETPPmax=ppc;
        
        if(M.SETPmin>M.SETPmax) M.SETPmin=M.SETPmax;        
#if (_EXT_BOARD==0) 
        //M.CONTSETP=DtCO.PGDPCO;
        M.CONTSETP=DtCO.PPCO;
#endif 
#if (_EXT_BOARD==1) 
        if(!M._AKTYWNE_PGD2)                                     //niezalezna praca regulatora pogodowego? 
        {
            if(M._I10V_RT)                                           //nastawa zdalna?
            {          
                M.CONTSETP=RSDT1.inCONTSETP;
                if(M.CONTSETP>M.SETPmax) M.CONTSETP=M.SETPmax;
                else
                if(M.CONTSETP<M.SETPmin) M.CONTSETP=M.SETPmin;         
            }
            else
            {
                //M.CONTSETP=DtCO.PGDPCO;   //nastawa manualna 
                M.CONTSETP=DtCO.PPCO;   //nastawa manualna
            }
        }
        else
        {
            //M.CONTSETP=DtCO.PGDPCO;
            M.CONTSETP=DtCO.PPCO;
        }
#endif 
#if (_EXT_BOARD==2)   
        if(!M._AKTYWNE_PGD2)                                     //niezalezna praca regulatora pogodowego? 
        {        
            if(M._I10V_RT||M._LIN_STR||M._LIN_KSK||M._LIN_PWD||M._LIN_BUF)                   //nastawa zdalna?
            {   
                M.CONTSETP=RSDT1.inCONTSETP;
                if(!M._LIN_PWD)  
                {
                    if(M.CONTSETP>M.SETPmax) M.CONTSETP=M.SETPmax;
                    else
                    if(M.CONTSETP<M.SETPmin) M.CONTSETP=M.SETPmin; 
                }
                else
                {
                    if(M.CONTSETP>M.SETPPmax) M.CONTSETP=M.SETPPmax;
                    else
                    if(M.CONTSETP<M.SETPmin) M.CONTSETP=M.SETPmin;                    
                }            
            }
            else
            {
                //M.CONTSETP=DtCO.PGDPCO;   //nastawa manualna    
                M.CONTSETP=DtCO.PPCO;   //nastawa manualna
            }
        }
        else
        {
            //M.CONTSETP=DtCO.PGDPCO;
            M.CONTSETP=DtCO.PPCO; 
        }            
#endif    
#if (_EXT_BOARD>=3)     
        if(!M._AKTYWNE_PGD2)                                     //niezalezna praca regulatora pogodowego? 
        {        
            if(M._I10V_RT||M._LIN_STR||M._LIN_KSK||M._IOT_STR||M._LIN_PWD||M._LIN_BUF)      //nastawa zdalna?
            {          
                M.CONTSETP=RSDT1.inCONTSETP;
                if(!M._LIN_PWD)  
                {
                    if(M.CONTSETP>M.SETPmax) M.CONTSETP=M.SETPmax;
                    else
                    if(M.CONTSETP<M.SETPmin) M.CONTSETP=M.SETPmin; 
                }
                else
                {
                    if(M.CONTSETP>M.SETPPmax) M.CONTSETP=M.SETPPmax;
                    else
                    if(M.CONTSETP<M.SETPmin) M.CONTSETP=M.SETPmin;                    
                }             
            }
            else
            {
                //M.CONTSETP=DtCO.PGDPCO;   //nastawa manualna  
                M.CONTSETP=DtCO.PPCO;   //nastawa manualna
            }
        }
        else
        {
            //M.CONTSETP=DtCO.PGDPCO;
            M.CONTSETP=DtCO.PPCO;
        }            
#endif  
    }
	else                                            //nastawa zdalna lub wartosc PCO
	{
        if(M._LIN_PWD)
        {        
            ppc=DtCO.PCO+DtKNF.pwtm; 
        }
        else
        {
            ppc=DtCO.PCO;
        }

        switch(DtKNF.opdg)
        {
            case 0:
            case 2:
            {                
                if(ppc>_MXPW_CO) ppc=_MXPW_CO;          //wartosci >85 sa sprowadzane do wartosci 85
                break;
            }
            case 1:
            {
                if(ppc>_MXPPW_CO) ppc=_MXPPW_CO;          //wartosci >85 sa sprowadzane do wartosci 85
                break;
            }
            default:
            {
                if(ppc>_MXPW_CO) ppc=_MXPW_CO;          //wartosci >85 sa sprowadzane do wartosci 85
                break;                
            }
        }		
        /*if(!DtKNF.opdg)
        {
            if(ppc>_MXPW_CO) ppc=_MXPW_CO;          //wartosci >85 sa sprowadzane do wartosci 85    
        }
        else
        {
            if(ppc>_MXPPW_CO) ppc=_MXPPW_CO;          //wartosci >85 sa sprowadzane do wartosci 85                    
        }*/
        DtCO.PPCO=DtCO.PCO;
        M.SETPmax=DtCO.PPCO;      
        M.SETPPmax=ppc; 
        
        if(M.SETPmin>M.SETPmax) M.SETPmin=M.SETPmax;
#if (_EXT_BOARD==0)  
		//M.CONTSETP=DtCO.PCO;
        M.CONTSETP=DtCO.PPCO;
#endif 
#if (_EXT_BOARD==1)        
        if(M._I10V_RT)                                           //nastawa zdalna?
        {          
            M.CONTSETP=RSDT1.inCONTSETP;
            if(M.CONTSETP>M.SETPmax) M.CONTSETP=M.SETPmax;
            else
            if(M.CONTSETP<M.SETPmin) M.CONTSETP=M.SETPmin;         
        }
        else
        {
            //M.CONTSETP=DtCO.PCO;   //nastawa manualna    
            M.CONTSETP=DtCO.PPCO;   //nastawa manualna
        }
#endif 
#if (_EXT_BOARD==2)        
        if(M._I10V_RT||M._LIN_STR||M._LIN_KSK||M._LIN_PWD||M._LIN_BUF)                   //nastawa zdalna?
        {   
            M.CONTSETP=RSDT1.inCONTSETP;
            if(!M._LIN_PWD)  
            {
                if(M.CONTSETP>M.SETPmax) M.CONTSETP=M.SETPmax;
                else
                if(M.CONTSETP<M.SETPmin) M.CONTSETP=M.SETPmin; 
            }
            else
            {
                if(M.CONTSETP>M.SETPPmax) M.CONTSETP=M.SETPPmax;
                else
                if(M.CONTSETP<M.SETPmin) M.CONTSETP=M.SETPmin;                    
            }            
        }
        else
        {
            //M.CONTSETP=DtCO.PCO;   //nastawa manualna 
            M.CONTSETP=DtCO.PPCO;   //nastawa manualna            
        }
#endif    
#if (_EXT_BOARD>=3)        
        if(M._I10V_RT||M._LIN_STR||M._LIN_KSK||M._IOT_STR||M._LIN_PWD||M._LIN_BUF)      //nastawa zdalna?
        {          
            M.CONTSETP=RSDT1.inCONTSETP;
            if(!M._LIN_PWD)  
            {
                if(M.CONTSETP>M.SETPmax) M.CONTSETP=M.SETPmax;
                else
                if(M.CONTSETP<M.SETPmin) M.CONTSETP=M.SETPmin; 
            }
            else
            {
                if(M.CONTSETP>M.SETPPmax) M.CONTSETP=M.SETPPmax;
                else
                if(M.CONTSETP<M.SETPmin) M.CONTSETP=M.SETPmin;                    
            }             
        }
        else
        {
            //M.CONTSETP=DtCO.PCO;   //nastawa manualna  
            M.CONTSETP=DtCO.PPCO;   //nastawa manualna            
        }
#endif           
	}
    switch(DtKNF.opdg)
    {
        case 0:
        case 2:
        {                
            if(!M._pob_SRW)	RSDT1.outPPCO=DtCO.PPCO;
            else 
            {
                DtCO.PPCO=_OFF_SRW_CO;
                RSDT1.outPPCO=_OFF_SRW_CO;
                M.CONTSETP=DtCO.PPCO;
                M.SETPmax=DtCO.PPCO;
                M.SETPPmax=M.SETPmax;
            }
            break;
        }
        case 1:
        {
            if(!M._pob_SRW)	RSDT1.outPPCO=DtCO.PPCO;
            else 
            {
                DtCO.PPCO=_OFFP_SRW_CO;
                RSDT1.outPPCO=_OFFP_SRW_CO;
                M.CONTSETP=DtCO.PPCO;
                M.SETPmax=DtCO.PPCO;
                M.SETPPmax=M.SETPmax;                
            }
            break;
        }
        default:
        {
            if(!M._pob_SRW)	RSDT1.outPPCO=DtCO.PPCO;
            else 
            {
                DtCO.PPCO=_OFF_SRW_CO;
                RSDT1.outPPCO=_OFF_SRW_CO;
                M.CONTSETP=DtCO.PPCO;
                M.SETPmax=DtCO.PPCO;
                M.SETPPmax=M.SETPmax;                
            }
            break;                
        }
    }
	/*if(!DtKNF.opdg)
	{
		if(!M._pob_SRW)	RSDT1.outPPCO=DtCO.PPCO;
		else 
		{
			DtCO.PPCO=_OFF_SRW_CO;
			RSDT1.outPPCO=_OFF_SRW_CO;
            M.CONTSETP=DtCO.PPCO;
            M.SETPmax=DtCO.PPCO;
		}
	}
	else											//dla aktywnego ogrzewania podlogowego
	{
		if(!M._pob_SRW)	RSDT1.outPPCO=DtCO.PPCO;
		else 
		{
			DtCO.PPCO=_OFFP_SRW_CO;
			RSDT1.outPPCO=_OFFP_SRW_CO;
            M.CONTSETP=DtCO.PPCO;
            M.SETPmax=DtCO.PPCO;
		}
	}*/
	if(M._pob_DzCO) 
	{
		DtCO.PPCO=_OFF_DZ_CO;
		RSDT1.outPPCO=_OFF_DZ_CO;
        M.CONTSETP=DtCO.PPCO;
        M.SETPmax=DtCO.PPCO;
        M.SETPPmax=M.SETPmax;        
	}
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Dla ogrzewania pogodowego:
//PPCO=_MIN_PCO+(_MAX_PCO-_MIN_PCO)*((25-Tzew)/(25-10))*kt/99
unsigned char GetPGD_PCO(void)
{
	unsigned char p;
	signed long m;

    switch(DtKNF.opdg)
    {
        case 0:           
        {
            if(DtCO.PCO)		//kt>0?
            {
                if(DtZW.SZW<25)	//Tzw<25?
                {
                    m=(25-DtZW.SZW)*DtCO.PCO;
                    m*=(_MAX_PCO-_MIN_PCO);
                    m*=100;
                    m/=1485;	//(25-10)*99
                    if((_MIN_PCO+m/100)>_MAX_PCO) p=_MAX_PCO;
                    else
                    {    
                        p=_MIN_PCO+m/100;
                        if(p<_MIN_PCO) p=_MIN_PCO;
                        if((p+DtKNF.prop)<_MAX_PCO) p=p+DtKNF.prop;             //przesuniecie rownolegle
                        else p=_MAX_PCO;
                    }
                    if(DtKNF.pgmx>=_MIN_PCO)                                    //wartosc graniczna dla regulatora pogodowego
                    {
                        if(p>DtKNF.pgmx) p=DtKNF.pgmx;
                    }
                    else p=_MIN_PCO;
                    if(M._AKTYWNE_PGD2&&RdPrt(S_TIM_PGD))                       //obnizenie nocne
                    {
                        if((p-DtKNF.pgdo)>_MIN_PCO) p=p-DtKNF.pgdo;
                        else p=_MIN_PCO;
                    }
                    return p;
                }
                else //return _MIN_PCO;
                { 
                    p=_MIN_PCO;
                    if((p+DtKNF.prop)<_MAX_PCO) p=p+DtKNF.prop;                 //przesuniecie rownolegle
                    else p=_MAX_PCO;           
                    if(DtKNF.pgmx>=_MIN_PCO)                                    //wartosc graniczna dla regulatora pogodowego
                    {
                        if(p>DtKNF.pgmx) p=DtKNF.pgmx;
                    }
                    else p=_MIN_PCO;                
                    if(M._AKTYWNE_PGD2&&RdPrt(S_TIM_PGD))                        //obnizenie nocne
                    {
                        if((p-DtKNF.pgdo)>_MIN_PCO) p=p-DtKNF.pgdo;
                        else p=_MIN_PCO;
                    } 
                    return p;
                }
            }
            else //return _MIN_PCO;
            {
                p=_MIN_PCO;
                if((p+DtKNF.prop)<_MAX_PCO) p=p+DtKNF.prop;             //przesuniecie rownolegle
                else p=_MAX_PCO;        
                if(DtKNF.pgmx>=_MIN_PCO)                                    //wartosc graniczna dla regulatora pogodowego
                {
                    if(p>DtKNF.pgmx) p=DtKNF.pgmx;
                }
                else p=_MIN_PCO;            
                if(M._AKTYWNE_PGD2&&RdPrt(S_TIM_PGD))                        //obnizenie nocne
                {
                    if((p-DtKNF.pgdo)>_MIN_PCO) p=p-DtKNF.pgdo;
                    else p=_MIN_PCO;
                } 
                return p;
            }
            break;
        }
        case 1:
        {
            if(DtCO.PCO)		//kt>0?
            {
                if(DtZW.SZW<25)	//Tzw<25?
                {
                    m=(25-DtZW.SZW)*DtCO.PCO;
                    m*=(_MAX_PDCO-_MIN_PDCO);
                    m*=100;
                    m/=1485;	//(25-10)*99
                    if((_MIN_PDCO+m/100)>_MAX_PDCO) p=_MAX_PDCO;
                    else
                    {    
                        p=_MIN_PDCO+m/100;
                        if(p<_MIN_PDCO) p=_MIN_PDCO;
                        if((p+DtKNF.prop)<_MAX_PDCO) p=p+DtKNF.prop;             //przesuniecie rownolegle
                        else p=_MAX_PDCO;
                    }                     
                    if(DtKNF.pgmx>=_MIN_PDCO)                                    //wartosc graniczna dla regulatora pogodowego
                    {
                        if(p>DtKNF.pgmx) p=DtKNF.pgmx;
                    }
                    else p=_MIN_PDCO;                
                    if(M._AKTYWNE_PGD2&&RdPrt(S_TIM_PGD))                        //obnizenie nocne
                    {
                        if((p-DtKNF.pgdo)>_MIN_PDCO) p=p-DtKNF.pgdo;
                        else p=_MIN_PDCO;
                    }                
                    return p;
                }
                else //return _MIN_PDCO;
                { 
                    p=_MIN_PDCO;
                    if((p+DtKNF.prop)<_MAX_PDCO) p=p+DtKNF.prop;        //przesuniecie rownolegle
                    else p=_MAX_PDCO;
                    if(DtKNF.pgmx>=_MIN_PDCO)                                    //wartosc graniczna dla regulatora pogodowego
                    {
                        if(p>DtKNF.pgmx) p=DtKNF.pgmx;
                    }
                    else p=_MIN_PDCO;                 
                    if(M._AKTYWNE_PGD2&&RdPrt(S_TIM_PGD))                        //obnizenie nocne
                    {
                        if((p-DtKNF.pgdo)>_MIN_PDCO) p=p-DtKNF.pgdo;
                        else p=_MIN_PDCO;
                    }  
                    return p;
                }
            }
            else //return _MIN_PDCO;
            { 
                p=_MIN_PDCO;
                if((p+DtKNF.prop)<_MAX_PDCO) p=p+DtKNF.prop;            //przesuniecie rownolegle
                else p=_MAX_PDCO;        
                if(DtKNF.pgmx>=_MIN_PDCO)                                    //wartosc graniczna dla regulatora pogodowego
                {
                    if(p>DtKNF.pgmx) p=DtKNF.pgmx;
                }
                else p=_MIN_PDCO;            
                if(M._AKTYWNE_PGD2&&RdPrt(S_TIM_PGD))                        //obnizenie nocne
                {
                    if((p-DtKNF.pgdo)>_MIN_PDCO) p=p-DtKNF.pgdo;
                    else p=_MIN_PDCO;
                }            
                return p;
            }
            break;
        } 
        case 2:            
        {
            if(DtCO.PCO)		//kt>0?
            {
                if(DtZW.SZW<25)	//Tzw<25?
                {
                    m=(25-DtZW.SZW)*DtCO.PCO;
                    m*=(_MAX_PCO-_MIN_PDCO);
                    m*=100;
                    m/=1485;	//(25-10)*99
                    if((_MIN_PDCO+m/100)>_MAX_PCO) p=_MAX_PCO;
                    else
                    {    
                        p=_MIN_PDCO+m/100;
                        if(p<_MIN_PDCO) p=_MIN_PDCO;
                        if((p+DtKNF.prop)<_MAX_PCO) p=p+DtKNF.prop;             //przesuniecie rownolegle
                        else p=_MAX_PCO;
                    }
                    if(DtKNF.pgmx>=_MIN_PDCO)                                    //wartosc graniczna dla regulatora pogodowego
                    {
                        if(p>DtKNF.pgmx) p=DtKNF.pgmx;
                    }
                    else p=_MIN_PDCO;
                    if(M._AKTYWNE_PGD2&&RdPrt(S_TIM_PGD))                       //obnizenie nocne
                    {
                        if((p-DtKNF.pgdo)>_MIN_PDCO) p=p-DtKNF.pgdo;
                        else p=_MIN_PDCO;
                    }
                    return p;
                }
                else //return _MIN_PDCO;
                { 
                    p=_MIN_PDCO;
                    if((p+DtKNF.prop)<_MAX_PCO) p=p+DtKNF.prop;                 //przesuniecie rownolegle
                    else p=_MAX_PCO;           
                    if(DtKNF.pgmx>=_MIN_PDCO)                                    //wartosc graniczna dla regulatora pogodowego
                    {
                        if(p>DtKNF.pgmx) p=DtKNF.pgmx;
                    }
                    else p=_MIN_PDCO;                
                    if(M._AKTYWNE_PGD2&&RdPrt(S_TIM_PGD))                        //obnizenie nocne
                    {
                        if((p-DtKNF.pgdo)>_MIN_PDCO) p=p-DtKNF.pgdo;
                        else p=_MIN_PDCO;
                    } 
                    return p;
                }
            }
            else //return _MIN_PDCO;
            {
                p=_MIN_PDCO;
                if((p+DtKNF.prop)<_MAX_PCO) p=p+DtKNF.prop;             //przesuniecie rownolegle
                else p=_MAX_PCO;        
                if(DtKNF.pgmx>=_MIN_PDCO)                                    //wartosc graniczna dla regulatora pogodowego
                {
                    if(p>DtKNF.pgmx) p=DtKNF.pgmx;
                }
                else p=_MIN_PDCO;            
                if(M._AKTYWNE_PGD2&&RdPrt(S_TIM_PGD))                        //obnizenie nocne
                {
                    if((p-DtKNF.pgdo)>_MIN_PDCO) p=p-DtKNF.pgdo;
                    else p=_MIN_PDCO;
                } 
                return p;
            }
            break;
        }        
    }    
	/*if(!DtKNF.opdg)	//ogrzewanie tradycyjne
	{
		if(DtCO.PCO)		//kt>0?
		{
			if(DtZW.SZW<25)	//Tzw<25?
			{
                m=(25-DtZW.SZW)*DtCO.PCO;
				m*=(_MAX_PCO-_MIN_PCO);
				m*=100;
				m/=1485;	//(25-10)*99
				if((_MIN_PCO+m/100)>_MAX_PCO) p=_MAX_PCO;
				else
                {    
                    p=_MIN_PCO+m/100;
                    if(p<_MIN_PCO) p=_MIN_PCO;
                    if((p+DtKNF.prop)<_MAX_PCO) p=p+DtKNF.prop;             //przesuniecie rownolegle
                    else p=_MAX_PCO;
                }
                if(DtKNF.pgmx>=_MIN_PCO)                                    //wartosc graniczna dla regulatora pogodowego
                {
                    if(p>DtKNF.pgmx) p=DtKNF.pgmx;
                }
                else p=_MIN_PCO;
                if(M._AKTYWNE_PGD2&&RdPrt(S_TIM_PGD))                       //obnizenie nocne
                {
                    if((p-DtKNF.pgdo)>_MIN_PCO) p=p-DtKNF.pgdo;
                    else p=_MIN_PCO;
                }
                return p;
			}
			else //return _MIN_PCO;
			{ 
				p=_MIN_PCO;
				if((p+DtKNF.prop)<_MAX_PCO) p=p+DtKNF.prop;                 //przesuniecie rownolegle
				else p=_MAX_PCO;           
                if(DtKNF.pgmx>=_MIN_PCO)                                    //wartosc graniczna dla regulatora pogodowego
                {
                    if(p>DtKNF.pgmx) p=DtKNF.pgmx;
                }
                else p=_MIN_PCO;                
                if(M._AKTYWNE_PGD2&&RdPrt(S_TIM_PGD))                        //obnizenie nocne
                {
                    if((p-DtKNF.pgdo)>_MIN_PCO) p=p-DtKNF.pgdo;
                    else p=_MIN_PCO;
                } 
                return p;
			}
		}
		else //return _MIN_PCO;
		{
			p=_MIN_PCO;
			if((p+DtKNF.prop)<_MAX_PCO) p=p+DtKNF.prop;             //przesuniecie rownolegle
			else p=_MAX_PCO;        
            if(DtKNF.pgmx>=_MIN_PCO)                                    //wartosc graniczna dla regulatora pogodowego
            {
                if(p>DtKNF.pgmx) p=DtKNF.pgmx;
            }
            else p=_MIN_PCO;            
            if(M._AKTYWNE_PGD2&&RdPrt(S_TIM_PGD))                        //obnizenie nocne
            {
                if((p-DtKNF.pgdo)>_MIN_PCO) p=p-DtKNF.pgdo;
                else p=_MIN_PCO;
            } 
            return p;
		}
	}
	else	//ogrzewanie podlogowe
	{
		if(DtCO.PCO)		//kt>0?
		{
			if(DtZW.SZW<25)	//Tzw<25?
			{
				m=(25-DtZW.SZW)*DtCO.PCO;
				m*=(_MAX_PDCO-_MIN_PDCO);
				m*=100;
				m/=1485;	//(25-10)*99
				if((_MIN_PDCO+m/100)>_MAX_PDCO) p=_MAX_PDCO;
				else
                {    
                    p=_MIN_PDCO+m/100;
                    if(p<_MIN_PDCO) p=_MIN_PDCO;
                    if((p+DtKNF.prop)<_MAX_PDCO) p=p+DtKNF.prop;             //przesuniecie rownolegle
                    else p=_MAX_PDCO;
                }                     
                if(DtKNF.pgmx>=_MIN_PDCO)                                    //wartosc graniczna dla regulatora pogodowego
                {
                    if(p>DtKNF.pgmx) p=DtKNF.pgmx;
                }
                else p=_MIN_PDCO;                
                if(M._AKTYWNE_PGD2&&RdPrt(S_TIM_PGD))                        //obnizenie nocne
                {
                    if((p-DtKNF.pgdo)>_MIN_PDCO) p=p-DtKNF.pgdo;
                    else p=_MIN_PDCO;
                }                
                return p;
			}
			else //return _MIN_PDCO;
			{ 
				p=_MIN_PDCO;
				if((p+DtKNF.prop)<_MAX_PDCO) p=p+DtKNF.prop;        //przesuniecie rownolegle
				else p=_MAX_PDCO;
                if(DtKNF.pgmx>=_MIN_PDCO)                                    //wartosc graniczna dla regulatora pogodowego
                {
                    if(p>DtKNF.pgmx) p=DtKNF.pgmx;
                }
                else p=_MIN_PDCO;                 
                if(M._AKTYWNE_PGD2&&RdPrt(S_TIM_PGD))                        //obnizenie nocne
                {
                    if((p-DtKNF.pgdo)>_MIN_PDCO) p=p-DtKNF.pgdo;
                    else p=_MIN_PDCO;
                }  
                return p;
			}
		}
		else //return _MIN_PDCO;
		{ 
			p=_MIN_PDCO;
			if((p+DtKNF.prop)<_MAX_PDCO) p=p+DtKNF.prop;            //przesuniecie rownolegle
			else p=_MAX_PDCO;        
            if(DtKNF.pgmx>=_MIN_PDCO)                                    //wartosc graniczna dla regulatora pogodowego
            {
                if(p>DtKNF.pgmx) p=DtKNF.pgmx;
            }
            else p=_MIN_PDCO;            
            if(M._AKTYWNE_PGD2&&RdPrt(S_TIM_PGD))                        //obnizenie nocne
            {
                if((p-DtKNF.pgdo)>_MIN_PDCO) p=p-DtKNF.pgdo;
                else p=_MIN_PDCO;
            }            
            return p;
		}
	}*/
}
/*---------------------------------------------------------------------------------*/
//Odczyt nastawy PCO z pamieci EEPROM
unsigned char ReadPCO(void)
{
    if(!M._AKTYWNE_PGD)
    {
        return ReadEEPCO();   //odczyt nastawy z pamieci EEPROM
    }
    else
    {
        return ReadEEPPGD();   //odczyt nastawy z pamieci EEPROM
    }
}
/*---------------------------------------------------------------------------------*/
//Zapis nastawy PCO po ustalonym czasie do EEPROM
void WritePCO(void)
{
	if(DtCO._newPCO&&RTdS(_RTPCO)>=_TWRTPCO)
	{
		DtCO._newPCO=0;
		StartRTdS(_RTPCO);
        if(!M._AKTYWNE_PGD)
        {
            WriteEEPCO();   //zapis nastawy w pamieci EEPROM
        }
        else
        {
            WriteEEPPGD();   //zapis nastawy w pamieci EEPROM
        }
	}
}
/*---------------------------------------------------------------------------------*/
//Nastawa PCO
void PomiarPCO(void)
{
	static unsigned char pnst;
    volatile unsigned char bnc;
    
    if(M._SYNCH_PGD)
    {
        return;    //trwa synchronizacja nastaw?
    }

	//ustaw wartosci graniczne nastawy
    if(!M._AKTYWNE_PGD)
	{
        SetMinMaxPCO();       
	}
	else
	{
        SetMinMaxPGD();
	}
    
	if(!InGetBit(RSDT1.inSTAT0,_INITDTA)&&MGSD._CON&&!M._PW_START&&!M._RS_START)
	{
        pnst=RSDT1.inPCO;
        if(pnst<DtCO.minPCO) pnst=DtCO.minPCO;	//nastawa w dozwolonym zakresie?
        if(pnst>DtCO.maxPCO) pnst=DtCO.maxPCO;	//nastawa w dozwolonym zakresie?
        if(DtCO.PCO!=pnst)	//nowa wartosc nastawy?
        {
            DtCO.PCO=pnst;
            DtCO._newPCO=1;
            StartRTdS(_RTPCO);
        }
        WritePCO();		//zapisz nastawe do pamieci
        RSDT1.outPCO=DtCO.PCO;  
	}
	else
	{
		DtCO.PCO=ReadPCO();
		if(pnst<DtCO.minPCO) pnst=DtCO.minPCO;	//nastawa w dozwolonym zakresie?
		if(pnst>DtCO.maxPCO) pnst=DtCO.maxPCO;	//nastawa w dozwolonym zakresie?
		RSDT1.outPCO=DtCO.PCO;
        //if(mmm<99) mmm++;
        //else mmm=0;        
	}

	if(M._AKTYWNE_PGD)
    {
        if(RdPrt(S_EN_ZW)) DtCO.PGDPCO=GetPGD_PCO(); 
    }
	SetModPCO();		
	RSDT1.outSETPmax=M.SETPmax;
	RSDT1.outSETPPmax=M.SETPPmax;    
    //Dostosuj margines do wymogow zmiennej nastawy modulatora
    if(M.CONTSETP<=DtCO.PPCO)
    {
        if((DtCO.PPCO-M.CONTSETP)>(_BNCO))
        {
            bnc=0;
        }
        else
        {
            bnc=(_BNCO)-(DtCO.PPCO-M.CONTSETP);
        }
    }
    else
    {
        bnc=(_BNCO);
    }
	DtCO.DAD_CO=ReturnACfromNTC(M.CONTSETP-bnc);
	DtCO.MAD_CO=ReturnACfromNTC(M.CONTSETP-bnc-_MR_MADCO);
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Zapis nastawy PCW po ustalonym czasie do EEPROM
void WritePCW(void)
{
	if(DtCW._newPCW&&RTdS(_RTPCW)>=_TWRTPCW)
	{
		DtCW._newPCW=0;
		StartRTdS(_RTPCW);
		WriteEEPCW();
	}
}
/*---------------------------------------------------------------------------------*/
//Formatowanie nastawy CW
void SetModPCW(void)
{
	if(M._AKTYWNE_KNF) DtCW.PPCW=_KNF_PCW;		//aktywna konfiguracja?
	else
	if((DtKNF.tpkt==_UNICO)&&M._pob_AL) DtCW.PPCW=_AL_PCW;		//aktywna f. AntyLegionella
	else DtCW.PPCW=DtCW.PCW;

	if(M._pob_DzCW)
	{
		DtCW.PPCW=_OFF_DZ_CW;
	}
	RSDT1.outPPCW=DtCW.PPCW;
}
/*---------------------------------------------------------------------------------*/
//Nastawa PCW
void PomiarPCW(void)
{
	static unsigned char pnst;
	unsigned char dac,dz,nst;

	//----------
	switch(DtKNF.tpkt)		//dla danego rodzaju kotla
	{
		case _MONOHB:
		{
			DtCW.minPCW=F2_MIN_PCW;
			DtCW.maxPCW=F2_MAX_PCW;
			break;
		}
		case _UNICO:				//kociol unico
		{
			DtCW.minPCW=F3_MIN_PCW;
			DtCW.maxPCW=F3_MAX_PCW;
			break;
		}
		default:					//domyslny monotermiczny
		{
			DtCW.minPCW=F2_MIN_PCW;
			DtCW.maxPCW=F2_MAX_PCW;
			break;
		}
	}
	RSDT1.outPCWmn=DtCW.minPCW;
	RSDT1.outPCWmx=DtCW.maxPCW;
	if(!InGetBit(RSDT1.inSTAT0,_INITDTA)&&MGSD._CON&&!M._PW_START&&!M._RS_START)
	{

		pnst=RSDT1.inPCW;
		if(DtKNF.tpkt!=_UNICO)
		{
			if(pnst<DtCW.minPCW) pnst=DtCW.minPCW;		//nastawa w dozwolonym zakresie?
		}
		else
		{
			if(pnst<DtCW.minPCW) pnst=DtCW.minPCW-1;	//nastawa poni¿ej minimum zakresu?
		}
		if(pnst>DtCW.maxPCW) pnst=DtCW.maxPCW;	//nastawa w dozwolonym zakresie?	//nastawa w dozwolonym zakresie?
		if(DtCW.PCW!=pnst)	//nowa wartosc nastawy?
		{
			DtCW.PCW=pnst;
			DtCW._newPCW=1;
			StartRTdS(_RTPCW);
		}
		WritePCW();		//zapisz nastawe do pamieci
		RSDT1.outPCW=DtCW.PCW;
	}
	else
	{
		DtCW.PCW=ReadEEPCW();
		if(DtKNF.tpkt!=_UNICO)
		{
			if(pnst<DtCW.minPCW) pnst=DtCW.minPCW;		//nastawa w dozwolonym zakresie?
		}
		else
		{
			if(pnst<DtCW.minPCW) pnst=DtCW.minPCW-1;	//nastawa poni¿ej minimum zakresu?
		}
		if(pnst>DtCW.maxPCW) pnst=DtCW.maxPCW;	//nastawa w dozwolonym zakresie?
		RSDT1.outPCW=DtCW.PCW;
	}
	SetModPCW();
	DtCW.DAD_CW=ReturnACfromNTC(DtCW.PPCW-(_BNCW-1));
	DtCW.MAD_CW=ReturnACfromNTC(DtCW.PPCW-(_BNCW-1)-_MR_MADCW);
}
/*---------------------------------------------------------------------------------*/
//Zapis nastawy ECO po ustalonym czasie do EEPROM
void WriteECO(void)
{
	if(DtIN._newECO&&RTdS(_RTECO)>=_TWRTECO)
	{
		DtIN._newECO=0;
		StartRTdS(_RTECO);
		WriteEEECO();
	}
}
/*---------------------------------------------------------------------------------*/
//Nastawa PCO
void PomiarECO(void)
{
	static unsigned char peco;

	if(!InGetBit(RSDT1.inSTAT0,_INITDTA)&&MGSD._CON&&!M._PW_START&&!M._RS_START)
	{
		peco=10-RSDT1.inECO;
		if(peco<N_MIN_ECO) peco=N_MIN_ECO;	//nastawa w dozwolonym zakresie?
		if(peco>N_MAX_ECO) peco=N_MAX_ECO;	//nastawa w dozwolonym zakresie?	//nastawa w dozwolonym zakresie?
		if(DtIN.Eco!=peco)	//nowa wartosc nastawy?
		{
			DtIN.Eco=peco;
			DtIN._newECO=1;
			StartRTdS(_RTECO);
		}
		WriteECO();		//zapisz nastawe do pamieci
	}
	else
	{
		DtIN.Eco=ReadEEECO();
		if(peco<N_MIN_ECO) peco=N_MIN_ECO;	//nastawa w dozwolonym zakresie?
		if(peco>N_MAX_ECO) peco=N_MAX_ECO;	//nastawa w dozwolonym zakresie?
	}
	//if(M._AKTYWNE_PGD) 	DtIN.PPEco=_PGDECO;
	//else DtIN.PPEco=DtIN.Eco;
	DtIN.PPEco=DtIN.Eco;
	RSDT1.outECO=10-DtIN.PPEco;
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Zapis pozycji PFN po ustalonym czasie do EEPROM
void WritePFN(void)
{
	if(PFN._newPFN&&RTdS(_RTPFN)>=_TWRTPFN)
	{
		PFN._newPFN=0;
		StartRTdS(_RTPFN);
        //xxy=1;
		WriteEEPFN();           //zapamietaj aktualna i porzednia pozycje przelacznika funkcji
	}
}
/*---------------------------------------------------------------------------------*/
//Nastawa przelacznika funkcyjnego
void PomiarPFN(void)
{
	static unsigned char pnst;

	if(!InGetBit(RSDT1.inSTAT0,_INITDTA)&&MGSD._CON&&!M._PW_START&&!M._RS_START)
	{
        if(InGetBit(RSDT1.inSTAT0,_PFN_OFF))	
		{          
			PFN._zima=0;
			PFN._lato=0;
			PFN._off=1;
			PFN._csn=0;
			PFN.poz=_PFOFF;
			InBitSet(&RSDT1.outSTAT2,_F_ZIMA,0);
			InBitSet(&RSDT1.outSTAT2,_F_LATO,0);
			InBitSet(&RSDT1.outSTAT2,_F_OFF,1);
		}
		else
		if(InGetBit(RSDT1.inSTAT0,_PFN_ZIMA))
		{
			PFN._zima=1;
			PFN._lato=0;
			PFN._off=0;
			PFN._csn=0;
			PFN.poz=_PFZMA;
			InBitSet(&RSDT1.outSTAT2,_F_ZIMA,1);
			InBitSet(&RSDT1.outSTAT2,_F_LATO,0);
			InBitSet(&RSDT1.outSTAT2,_F_OFF,0);
		}
		else
		if(InGetBit(RSDT1.inSTAT0,_PFN_LATO))	
		{
            PFN._zima=0;
			PFN._lato=1;
			PFN._off=0;
			PFN._csn=0;
			PFN.poz=_PFLTO;
			InBitSet(&RSDT1.outSTAT2,_F_ZIMA,0);
			InBitSet(&RSDT1.outSTAT2,_F_LATO,1);
			InBitSet(&RSDT1.outSTAT2,_F_OFF,0);
		}
		else
		{
			PFN._zima=1;
			PFN._lato=0;
			PFN._off=0;
			PFN._csn=0;
			PFN.poz=_PFZMA;
			InBitSet(&RSDT1.outSTAT2,_F_ZIMA,1);
			InBitSet(&RSDT1.outSTAT2,_F_LATO,0);
			InBitSet(&RSDT1.outSTAT2,_F_OFF,0);
		}
        if(!M._AKTYWNE_KNF)     //zapobiegaj zapisowi przy aktywnej procedurze konfiguracji
        {
            pnst=ReadEEPFN();
            if(PFN.poz!=pnst)	//nowa wartosc pozycji PFN?
            {
                if(!PFN._newPFN) StartRTdS(_RTPFN);
                PFN._newPFN=1;    
            }
            WritePFN();			//zapisz nastawe do pamieci
        }
	
		if(InGetBit(RSDT1.inSTAT0,_PFN_RES))	PFN._res=1;
		else PFN._res=0;
	}
	else
	{
        PFN.poz=ReadEEPFN();
		SetPFN();
	}
    PFN.pop_poz=ReadEEPopPFN();
}
/*---------------------------------------------------------------------------------*/
//Pomiar wartosci cisnienia
void PomiarCS(void)
{
    volatile static float avr=0;
    volatile unsigned int AC_DTA;
    volatile unsigned char AC_AVR;
    
	//unsigned char dac;
    static float acs;
    float cs;
    
    AC_DTA=ReadAC16(_CHN_CS);                       //pomiar 16bit
    //-------------------------------------
    //wartosc srednia
    if(!avr)
    {
        avr = (float)AC_DTA;
    }
    else
    {
        avr = avr + ((float)AC_DTA - avr) / 50;             //wartosc srednia pomiaru
    }
    //-------------------------------------
    //reakcja na zwarcie/rozwarcie
	AC_AVR=(unsigned char)(((unsigned int)avr)>>8);
    if((!DtKNF.tpcs&&(AC_AVR<_CS_MIN||AC_AVR>_CS_MAX))||(DtKNF.tpcs&&(AC_AVR<_CS5_MIN||AC_AVR>_CS5_MAX)))						//awaria czujnika?
	{       
        DtCS._errorZ=1;                 //pomiar nie miesci sie w zakresie
		if(DtKNF.robg==_OTWARTY)		//UniCo z ukladem otwartym?
		{
			DtCS._error=0;
			DtCS._no=0;
			DtCS._lo=0;
			DtCS._hi=0;
			DtCS._work=1;
			DtCS._no_mont=1;								//czujnik cisnienia nie zam. w ukladzie								
			DtCS.CS=0;
		}
		else
		{
			DtCS._error=1;
			DtCS._no=0;
			DtCS._lo=0;
			DtCS._hi=0;
			DtCS._work=0;
			DtCS._no_mont=0;								//czujnik cisnienia zam. w ukladzie
		}
	}	
	else
    {
        if(DtCS._errorZ)
        {
            avr=0;
        }
        DtCS._errorZ=0;
        
 		DtCS._error=0;
		DtCS._no_mont=0;									//czujnik cisnienia zam. w ukladzie
		if(!DtKNF.tpcs)										//czujnki 18V?
		{
			if(AC_AVR<_CS_0) AC_AVR=_CS_0;
			if(AC_AVR>_CS_4) AC_AVR=_CS_4;
			cs=((AC_AVR-_CS_0)*10.0)/(((_CS_4-_CS_0)*10.0)/_NWCS);
			if(cs>_NWCS) cs=_NWCS;
		}
		else												//czujnik 5V
		{
			if(AC_AVR<_CS5_0) AC_AVR=_CS5_0;
			if(AC_AVR>_CS5_4) AC_AVR=_CS5_4;
			cs=((AC_AVR-_CS5_0)*10.0)/(((_CS5_4-_CS5_0)*10.0)/_NWCS5);
			if(cs>_NWCS5) cs=_NWCS5;
		}
        acs=acs+((cs-acs)/10.0);    //50
        if(acs<0) acs=0;
		DtCS.CS=(unsigned char)round(acs);
		RSDT1.outCS=DtCS.CS;
		if(DtKNF.robg==_OTWARTY)		//UniCo z ukladem otwartym?
		{
			//wylaczona detekcja niskiego cisnienia
			DtCS._no=0;
			DtCS._lo=0;
			if(DtCS.CS<=_ON2_WRK_CSN) DtCS._work=1;
			else
			if(DtCS.CS>=_OFF2_WRK_CSN) DtCS._work=0;
		}
		else
		{
			//aktywna detekcja niskiego cisnienia
			if(DtCS.CS<_NO_CSN) DtCS._no=1;				//brak wody w ukladzie?
			else DtCS._no=0;
			if(!DtCS._no&&DtCS.CS<_LO_CSN) DtCS._lo=1;	//niskie cisnienie w ukladzie?
			else DtCS._lo=0;
			if((DtCS.CS>=_ON1_WRK_CSN)&&(DtCS.CS<=_ON2_WRK_CSN)) DtCS._work=1;
			else
			if((DtCS.CS<=_OFF1_WRK_CSN)||(DtCS.CS>=_OFF2_WRK_CSN)) DtCS._work=0;
		}	
		if(DtCS.CS>=_ON_HI_CSN)	DtCS._hi=1;			//zbyt wysokie cisnienie? 
		else DtCS._hi=0;        
    }
  
	/*dac=ReadAC(_CHN_CS);
	if((!DtKNF.tpcs&&(dac<_CS_MIN||dac>_CS_MAX))||(DtKNF.tpcs&&(dac<_CS5_MIN||dac>_CS5_MAX)))						//awaria czujnika?
	{
		if(DtKNF.robg==_OTWARTY)		//UniCo z ukladem otwartym?
		{
			DtCS._error=0;
			DtCS._no=0;
			DtCS._lo=0;
			DtCS._hi=0;
			DtCS._work=1;
			DtCS._no_mont=1;								//czujnik cisnienia nie zam. w ukladzie								
			DtCS.CS=0;
		}
		else
		{
			DtCS._error=1;
			DtCS._no=0;
			DtCS._lo=0;
			DtCS._hi=0;
			DtCS._work=0;
			DtCS._no_mont=0;								//czujnik cisnienia zam. w ukladzie
		}
	}	
	else
	{
 		DtCS._error=0;
		DtCS._no_mont=0;									//czujnik cisnienia zam. w ukladzie
		if(!DtKNF.tpcs)										//czujnki 18V?
		{
			if(dac<_CS_0) dac=_CS_0;
			if(dac>_CS_4) dac=_CS_4;
			cs=((dac-_CS_0)*10.0)/(((_CS_4-_CS_0)*10.0)/_NWCS);
			if(cs>_NWCS) cs=_NWCS;
		}
		else												//czujnik 5V
		{
			if(dac<_CS5_0) dac=_CS5_0;
			if(dac>_CS5_4) dac=_CS5_4;
			cs=((dac-_CS5_0)*10.0)/(((_CS5_4-_CS5_0)*10.0)/_NWCS5);
			if(cs>_NWCS5) cs=_NWCS5;
		}
        acs=acs+((cs-acs)/50.0);
        if(acs<0) acs=0;
		DtCS.CS=(unsigned char)round(acs);
		RSDT1.outCS=DtCS.CS;
		if(DtKNF.robg==_OTWARTY)		//UniCo z ukladem otwartym?
		{
			//wylaczona detekcja niskiego cisnienia
			DtCS._no=0;
			DtCS._lo=0;
			if(DtCS.CS<=_ON2_WRK_CSN) DtCS._work=1;
			else
			if(DtCS.CS>=_OFF2_WRK_CSN) DtCS._work=0;
		}
		else
		{
			//aktywna detekcja niskiego cisnienia
			if(DtCS.CS<_NO_CSN) DtCS._no=1;				//brak wody w ukladzie?
			else DtCS._no=0;
			if(!DtCS._no&&DtCS.CS<_LO_CSN) DtCS._lo=1;	//niskie cisnienie w ukladzie?
			else DtCS._lo=0;
			if((DtCS.CS>=_ON1_WRK_CSN)&&(DtCS.CS<=_ON2_WRK_CSN)) DtCS._work=1;
			else
			if((DtCS.CS<=_OFF1_WRK_CSN)||(DtCS.CS>=_OFF2_WRK_CSN)) DtCS._work=0;
		}	
		if(DtCS.CS>=_ON_HI_CSN)	DtCS._hi=1;			//zbyt wysokie cisnienie? 
		else DtCS._hi=0;
	}*/
}
/*---------------------------------------------------------------------------------*/
#endif