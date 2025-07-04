/*pomiar.c*/
//Procedury pomiarowe (oparte na przetworniku AC) UNISTER zmodyfikowany GRZ01
// (!) Bazuje na funkcjach z pliku: ac.c, rtimer.c /RTdS(2)/
/*---------------------------------------------------------------------------------*/
#include <pic18.h>
#include "global.h"				//parametry globalne
#include "pomiar.h"				//parametry lokalne
#include "konfig.h"
#include "main.tp.h"
#include "pwm.h"
#include "smgs_rs9b.tp.h"			//parametry lokalne

DataCO DtCO;
DataIN DtIN;
DataCW DtCW;
DataZW DtZW;
DataPFN PFN;
DataCSN DtCS;

extern MGSRSData MGSD;
extern tpRSDTA1 RSDT1;
extern tpMAIN M;
extern DtKONF DtKNF;
extern DtPWM PWM;
extern unsigned char x,mmm;

unsigned int pbf;
static unsigned char PmrtimFlt1,PmrtimFlt2;
static unsigned int PmrtimFlt3;
unsigned char wff[12];

#if _SRVTRB==1						//aktywny tryb serwisowy kompilacji?
unsigned char mypwm=_MG2_MOD;
#endif
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
	unsigned char hist,ph;
//----------	
//wskaz CO
	/*dt=DtCO.PPCO-DtCO.CO;
	if((DtCO.CO>=(DtCO.PPCO+_DM_GECO))||(DtCO.CO<=(DtCO.PPCO-_DM_GECO))) DtCO.SCO=DtCO.CO;
     */
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
			//wartosc histerezy
			hist=DtCO.HstCo+ph;
			//kontrola progu histerezy
			switch(DtKNF.tpkt)		//dla danego rodzaju kotla
			{
				case _BITERM:		//bitermiczny (miniterm)
				case _BITERM2:		//bitermiczny (maxiterm)
				{
					if(!DtKNF.opdg)	//ogrzewanie tradycyjne
					{
						if((M.CONTSETP+hist)>(_MAX_PCO+F1_AD_CO))	//nastawa+hist > _MAX_PCO+F2_AD_CO?
						{
							hist=F1_AD_CO;
						}
					}
					else			//ogrzewanie podlogowe
					{
						if((M.CONTSETP+hist)>(_MAX_PDCO+F1_AD_CO))	//nastawa+hist > _MAX_PDCO+F2_AD_CO?
						{
							hist=F1_AD_CO;
						}
					}
					break;
				}
				case _MONOHB:
				{
					if(!DtKNF.opdg)	//ogrzewanie tradycyjne
					{
						if((M.CONTSETP+hist)>(_MAX_PCO+F2_AD_CO))	//nastawa+hist > _MAX_PCO+F2_AD_CO?
						{
							hist=F2_AD_CO;
						}
					}
					else			//ogrzewanie podlogowe
					{
						if((M.CONTSETP+hist)>(_MAX_PDCO+F2_AD_CO))	//nastawa+hist > _MAX_PDCO+F2_AD_CO?
						{
							hist=F2_AD_CO;
						}
					}
					break;
				}
				case _UNICO:				//kociol unico
				{
					if(!DtKNF.opdg)	//ogrzewanie tradycyjne
					{
						if((M.CONTSETP+hist)>(_MAX_PCO+F3_AD_CO))	//nastawa+hist > _MAX_PCO+F2_AD_CO?
						{
							hist=F3_AD_CO;
						}
					}
					else			//ogrzewanie podlogowe
					{
						if((M.CONTSETP+hist)>(_MAX_PDCO+F3_AD_CO))	//nastawa+hist > _MAX_PDCO+F2_AD_CO?
						{
							hist=F3_AD_CO;
						}
					}
					break;
				}
				default:
				{
					if(!DtKNF.opdg)	//ogrzewanie tradycyjne
					{
						if((M.CONTSETP+hist)>(_MAX_PCO+F2_AD_CO))	//nastawa+hist > _MAX_PCO+F2_AD_CO?
						{
							hist=F2_AD_CO;
						}
					}
					else			//ogrzewanie podlogowe
					{
						if((M.CONTSETP+hist)>(_MAX_PDCO+F2_AD_CO))	//nastawa+hist > _MAX_PDCO+F2_AD_CO?
						{
							hist=F2_AD_CO;
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
		if(DtKNF.tpkt==_BITERM)
		{
			dt=DtCW.PPCW-DtCW.CW;
			if((DtCW.CW>=(DtCW.PPCW+_DM_GECW2))||(DtCW.CW<=(DtCW.PPCW-_DM_GECW2))) DtCW.SCW=DtCW.CW;
			else
			{
				if(dt>=0)
				{
					if(dt<_BNCW2) pop=dt;
					else
					if((dt>=_BNCW2)&&(dt<=(_DM_GECW2-_BNCW2))) pop=_BNCW2;
					else
					if(dt>(_DM_GECW2-_BNCW2)) pop=_DM_GECW2-dt;
					DtCW.SCW=DtCW.CW+pop;
				}
				else 
				{
					if((-1*dt)<_BNCW2) pop=(-1*dt);
					else
					if(((-1*dt)>=_BNCW2)&&((-1*dt)<=(_DM_GECW2-_BNCW2))) pop=_BNCW2;
					else
					if((-1*dt)>(_DM_GECW2-_BNCW2)) pop=_DM_GECW2+dt;
					DtCW.SCW=DtCW.CW-pop;
				}
			}
		}
		else
		{
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
					if(-1*dt<_BNCW) pop=_BNCW+dt;
					else
					if((-1*dt>=_BNCW)&&(-1*dt<=(_DM_GECW-_BNCW))) pop=_BNCW;
					else
					if(-1*dt>(_DM_GECW-_BNCW)) pop=_DM_GECW+dt;
					DtCW.SCW=DtCW.CW-pop;
				}
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
		if(DtCO.indCO<=7) DtCO.dDCO=0;	//temp = const
		else
		{ 
			DtCO.dDCO=DtCO.CO-fco;
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

	if(!DtIN._err_IN)							
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
	DtCO.AC_CO=ReadAC(_CHN_CO);
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
	else DtCO._err_CO=0;
	RSDT1.outrCO=DtCO.CO;
	switch(DtKNF.tpkt)		//dla danego rodzaju kotla
	{
		case _BITERM:		//bitermiczny (miniterm)
		case _BITERM2:		//bitermiczny (maxiterm)
		{	
			if(!DtKNF.opdg)
			{
				if((M.CONTSETP+DtCO.HstCo)>(_MAX_PCO+F1_AD_CO))	//nastawa+hist > _MAX_PCO+F2_AD_CO?
				{
					if((DtCO.CO-_MAX_PCO)>=F1_AD_CO) DtCO._ad_CO=1;
					else DtCO._ad_CO=0;
				}
				else	
				{
					if((DtCO.CO-M.CONTSETP)>=DtCO.HstCo) DtCO._ad_CO=1;
					else DtCO._ad_CO=0;
				}
			}
			else	//ogrzewanie podlogowe
			{
				if((M.CONTSETP+DtCO.HstCo)>(_MAX_PDCO+F1_AD_CO))	//nastawa+hist > _MAX_PDCO+F2_AD_CO?
				{
					if((DtCO.CO-_MAX_PDCO)>=F1_AD_CO) DtCO._ad_CO=1;
					else DtCO._ad_CO=0;
				}
				else	
				{
					if((DtCO.CO-M.CONTSETP)>=DtCO.HstCo) DtCO._ad_CO=1;
					else DtCO._ad_CO=0;
				}
			}
            
			if((M.CONTSETP-DtCO.CO)>=F1_SU_CO) DtCO._su_CO=1; 
			else DtCO._su_CO=0;
            
			if(DtCO.CO<=F1_PTWP_CO) DtCO._ptwp_CO=1;
			if(DtCO.CO>F1_PTWP_CO+1) DtCO._ptwp_CO=0;
			if(DtCO.CO<=F1_PZ_CO) DtCO._pz_CO=1;
			if(DtCO.CO>F1_PW_CO) DtCO._pz_CO=0;
			if(DtCO.CO<=F1_CLD_CO) DtCO._cold_CO=1;				
			if(DtCO.CO>F1_CLD_CO+1) DtCO._cold_CO=0;
			if(DtCO.CO>=F1_HOT_CO) DtCO._hot_CO=1;						
			if(DtCO.CO<F1_HOT_CO-1) DtCO._hot_CO=0;
			break;
		}
		case _MONOHB:
		{
			if(!DtKNF.opdg)
			{
				if((M.CONTSETP+DtCO.HstCo)>(_MAX_PCO+F2_AD_CO))	//nastawa+hist > _MAX_PCO+F2_AD_CO?
				{
					if((DtCO.CO-_MAX_PCO)>=F2_AD_CO) DtCO._ad_CO=1;
					else DtCO._ad_CO=0;
				}
				else	
				{
					if((DtCO.CO-M.CONTSETP)>=DtCO.HstCo) DtCO._ad_CO=1;
					else DtCO._ad_CO=0;
				}
			}
			else	//ogrzewanie podlogowe
			{
				if((M.CONTSETP+DtCO.HstCo)>(_MAX_PDCO+F2_AD_CO))	//nastawa+hist > _MAX_PDCO+F2_AD_CO?
				{
					if((DtCO.CO-_MAX_PDCO)>=F2_AD_CO) DtCO._ad_CO=1;
					else DtCO._ad_CO=0;
				}
				else	
				{
					if((DtCO.CO-M.CONTSETP)>=DtCO.HstCo) DtCO._ad_CO=1;
					else DtCO._ad_CO=0;
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
			if(!DtKNF.opdg)
			{
				if((M.CONTSETP+DtCO.HstCo)>(_MAX_PCO+F3_AD_CO))	//nastawa+hist > _MAX_PCO+F2_AD_CO?
				{
					if((DtCO.CO-_MAX_PCO)>=F3_AD_CO) DtCO._ad_CO=1;
					else DtCO._ad_CO=0;
				}
				else	
				{
					if((DtCO.CO-M.CONTSETP)>=DtCO.HstCo) DtCO._ad_CO=1;
					else DtCO._ad_CO=0;
				}
			}
			else	//ogrzewanie podlogowe
			{
				if((M.CONTSETP+DtCO.HstCo)>(_MAX_PDCO+F3_AD_CO))	//nastawa+hist > _MAX_PDCO+F2_AD_CO?
				{
					if((DtCO.CO-_MAX_PDCO)>=F3_AD_CO) DtCO._ad_CO=1;
					else DtCO._ad_CO=0;
				}
				else	
				{
					if((DtCO.CO-M.CONTSETP)>=DtCO.HstCo) DtCO._ad_CO=1;
					else DtCO._ad_CO=0;
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
			if(!DtKNF.opdg)
			{
				if((M.CONTSETP+DtCO.HstCo)>(_MAX_PCO+F2_AD_CO))	//nastawa+hist > _MAX_PCO+F2_AD_CO?
				{
					if((DtCO.CO-_MAX_PCO)>=F2_AD_CO) DtCO._ad_CO=1;
					else DtCO._ad_CO=0;
				}
				else	
				{
					if((DtCO.CO-M.CONTSETP)>=DtCO.HstCo) DtCO._ad_CO=1;
					else DtCO._ad_CO=0;
				}
			}
			else	//ogrzewanie podlogowe
			{
				if((M.CONTSETP+DtCO.HstCo)>(_MAX_PDCO+F2_AD_CO))	//nastawa+hist > _MAX_PDCO+F2_AD_CO?
				{
					if((DtCO.CO-_MAX_PDCO)>=F2_AD_CO) DtCO._ad_CO=1;
					else DtCO._ad_CO=0;
				}
				else	
				{
					if((DtCO.CO-M.CONTSETP)>=DtCO.HstCo) DtCO._ad_CO=1;
					else DtCO._ad_CO=0;
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

	if(!DtKNF.opdg)
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
}
#else
void PomiarCO(void)
{
	DtCO.AC_CO=ReadAC(_CHN_CO);
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
	else DtCO._err_CO=0;
	RSDT1.outrCO=DtCO.CO;
	switch(DtKNF.tpkt)		//dla danego rodzaju kotla
	{
		case _BITERM:		//bitermiczny (miniterm)
		case _BITERM2:		//bitermiczny (maxiterm)
		{	
			if(!DtKNF.opdg)
			{
				if((DtCO.PPCO+DtCO.HstCo)>(_MAX_PCO+F1_AD_CO))	//nastawa+hist > _MAX_PCO+F2_AD_CO?
				{
					if((DtCO.CO-_MAX_PCO)>=F1_AD_CO) DtCO._ad_CO=1;
					else DtCO._ad_CO=0;
				}
				else	
				{
					if((DtCO.CO-DtCO.PPCO)>=DtCO.HstCo) DtCO._ad_CO=1;
					else DtCO._ad_CO=0;
				}
			}
			else	//ogrzewanie podlogowe
			{
				if((DtCO.PPCO+DtCO.HstCo)>(_MAX_PDCO+F1_AD_CO))	//nastawa+hist > _MAX_PDCO+F2_AD_CO?
				{
					if((DtCO.CO-_MAX_PDCO)>=F1_AD_CO) DtCO._ad_CO=1;
					else DtCO._ad_CO=0;
				}
				else	
				{
					if((DtCO.CO-DtCO.PPCO)>=DtCO.HstCo) DtCO._ad_CO=1;
					else DtCO._ad_CO=0;
				}
			}
            
			if((DtCO.PPCO-DtCO.CO)>=F1_SU_CO) DtCO._su_CO=1; 
			else DtCO._su_CO=0;
            
			if(DtCO.CO<=F1_PTWP_CO) DtCO._ptwp_CO=1;
			if(DtCO.CO>F1_PTWP_CO+1) DtCO._ptwp_CO=0;
			if(DtCO.CO<=F1_PZ_CO) DtCO._pz_CO=1;
			if(DtCO.CO>F1_PW_CO) DtCO._pz_CO=0;
			if(DtCO.CO<=F1_CLD_CO) DtCO._cold_CO=1;				
			if(DtCO.CO>F1_CLD_CO+1) DtCO._cold_CO=0;
			if(DtCO.CO>=F1_HOT_CO) DtCO._hot_CO=1;						
			if(DtCO.CO<F1_HOT_CO-1) DtCO._hot_CO=0;
			break;
		}
		case _MONOHB:
		{
			if(!DtKNF.opdg)
			{
				if((DtCO.PPCO+DtCO.HstCo)>(_MAX_PCO+F2_AD_CO))	//nastawa+hist > _MAX_PCO+F2_AD_CO?
				{
					if((DtCO.CO-_MAX_PCO)>=F2_AD_CO) DtCO._ad_CO=1;
					else DtCO._ad_CO=0;
				}
				else	
				{
					if((DtCO.CO-DtCO.PPCO)>=DtCO.HstCo) DtCO._ad_CO=1;
					else DtCO._ad_CO=0;
				}
			}
			else	//ogrzewanie podlogowe
			{
				if((DtCO.PPCO+DtCO.HstCo)>(_MAX_PDCO+F2_AD_CO))	//nastawa+hist > _MAX_PDCO+F2_AD_CO?
				{
					if((DtCO.CO-_MAX_PDCO)>=F2_AD_CO) DtCO._ad_CO=1;
					else DtCO._ad_CO=0;
				}
				else	
				{
					if((DtCO.CO-DtCO.PPCO)>=DtCO.HstCo) DtCO._ad_CO=1;
					else DtCO._ad_CO=0;
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
			if(!DtKNF.opdg)
			{
				if((DtCO.PPCO+DtCO.HstCo)>(_MAX_PCO+F3_AD_CO))	//nastawa+hist > _MAX_PCO+F2_AD_CO?
				{
					if((DtCO.CO-_MAX_PCO)>=F3_AD_CO) DtCO._ad_CO=1;
					else DtCO._ad_CO=0;
				}
				else	
				{
					if((DtCO.CO-DtCO.PPCO)>=DtCO.HstCo) DtCO._ad_CO=1;
					else DtCO._ad_CO=0;
				}
			}
			else	//ogrzewanie podlogowe
			{
				if((DtCO.PPCO+DtCO.HstCo)>(_MAX_PDCO+F3_AD_CO))	//nastawa+hist > _MAX_PDCO+F2_AD_CO?
				{
					if((DtCO.CO-_MAX_PDCO)>=F3_AD_CO) DtCO._ad_CO=1;
					else DtCO._ad_CO=0;
				}
				else	
				{
					if((DtCO.CO-DtCO.PPCO)>=DtCO.HstCo) DtCO._ad_CO=1;
					else DtCO._ad_CO=0;
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
			if(!DtKNF.opdg)
			{
				if((DtCO.PPCO+DtCO.HstCo)>(_MAX_PCO+F2_AD_CO))	//nastawa+hist > _MAX_PCO+F2_AD_CO?
				{
					if((DtCO.CO-_MAX_PCO)>=F2_AD_CO) DtCO._ad_CO=1;
					else DtCO._ad_CO=0;
				}
				else	
				{
					if((DtCO.CO-DtCO.PPCO)>=DtCO.HstCo) DtCO._ad_CO=1;
					else DtCO._ad_CO=0;
				}
			}
			else	//ogrzewanie podlogowe
			{
				if((DtCO.PPCO+DtCO.HstCo)>(_MAX_PDCO+F2_AD_CO))	//nastawa+hist > _MAX_PDCO+F2_AD_CO?
				{
					if((DtCO.CO-_MAX_PDCO)>=F2_AD_CO) DtCO._ad_CO=1;
					else DtCO._ad_CO=0;
				}
				else	
				{
					if((DtCO.CO-DtCO.PPCO)>=DtCO.HstCo) DtCO._ad_CO=1;
					else DtCO._ad_CO=0;
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

	if(!DtKNF.opdg)
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
}
#endif
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Pomiar wartosci temperatury na wlocie i ustawienie odpowiednich flag stanu
void PomiarIN(void)
{
	signed char delta;

	DtIN.AC_IN=ReadAC(_CHN_IN);
	DtIN.IN=ReadTabNTC(DtIN.AC_IN);
	
	if(DtIN.AC_IN<=_NTC_MIN||DtIN.AC_IN>=_NTC_MAX) 		//stwierdzenie awarii czujnika temp. IN
	{
		DtIN._err_IN=1;
		return;
	}	
	else DtIN._err_IN=0;
	RSDT1.outIN=DtIN.IN;
}
/*---------------------------------------------------------------------------------*/
//Pomiar wartosci CW i ustawienie odpowiednich flag stanu
void PomiarCW(void)
{
    volatile unsigned char tmz;
    
	DtCW.AC_CW=ReadAC(_CHN_CW);
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
	else DtCW._err_CW=0;
	RSDT1.outrCW=DtCW.CW;
	switch(DtKNF.tpkt)			//dla danego rodzaju kotla
	{
		case _BITERM:		//bitermiczny (miniterm)
		case _BITERM2:		//bitermiczny (maxiterm)
		{	
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
			break;
		}
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
            else                //granica zalaczenia/wylaczenia zalezna od nastawy
            {
                if((DtCW.CW-DtCW.PPCW)>=F2_AD_CW) DtCW._ad_CW=1;
                else DtCW._ad_CW=0;
                if((DtCW.PPCW-DtCW.CW)>=F2_SU_CW) DtCW._su_CW=1;
                else DtCW._su_CW=0;                
            }                       
			break;			
		}
	}
    //granica powyzej ktorej zawor 3DR wysterowany jest tylko na obieg CO, powr�t do normalnej pracy po spadku temp. ponizej tego progu
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
	DtZW.AC_ZW=ReadAC(_CHN_ZW);
	DtZW.ZW=ReadTabZNTC(DtZW.AC_ZW);
	if(DtZW.AC_ZW<=_ZNTC_MIN||DtZW.AC_ZW>=_ZNTC_MAX)
    {
        DtZW._on_ZW=0;		//stwierdzenie awarii czujnika temp. CO
        DtZW._en_ZW=0;      //brak czujnika temp. zew.   
        RSDT1.outZW=0; 
    }
	else
    {
        RSDT1.outZW=DtZW.ZW;        
        DtZW._en_ZW=1;      //brak czujnika temp. zew.
        if(DtKNF.pgdr)         //konfiguracja zezwala na aktywacje regulatora pogodowego?
        {        
#if (_EXT_BOARD<=1)        
            DtZW._on_ZW=1;
#endif     
#if (_EXT_BOARD==2)        
            if(M._LIN_DPGD) DtZW._on_ZW=0;                                      //ignoruj obecnosc czujnika temp. zew.
            else DtZW._on_ZW=1;                                                 //stwierdzenie obecnosci czujnika temp. zew.- aktywacja regulatora pogodowego   
#endif
#if (_EXT_BOARD>=3)  
            if(M._LIN_DPGD||M._IOT_DPGD) DtZW._on_ZW=0;                         //ignoruj obecnosc czujnika temp. zew.  
            else DtZW._on_ZW=1;                                                 //stwierdzenie obecnosci czujnika temp. zew.- aktywacja regulatora pogodowego  
            if(DtKNF.pgdr==2)                                                   //parametr P19 == 2?
            {
                DtZW._on_ZW=1;                                                  //regulator tradycyjny (jak dla DtKNF.pgdr==1) realizowany niezaleznie od aktywnosci OT czy LIN
            }            
#endif             
        }
        else
        {
            DtZW._on_ZW=0;
        }            
    }
    M._AKTYWNE_PGD2=0;
    M._pob_PGDRT=0;    
}
/*---------------------------------------------------------------------------------*/
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
                if(DtZW._en_ZW)
                {
                    M._AKTYWNE_PGD=1;
                    M._AKTYWNE_PGD2=0;
                    DtCO.PCO=ReadEEPPGD();   //odczyt nastawy z pamieci EEPROM
                    RSDT1.outPCO=DtCO.PCO;
                    SetMinMaxPGD();    
                    ChkPGD();
                    DtCO.PGDPCO=GetPGD_PCO();
                    //dobierz wartosci M.CONTSETP, M.SETPmax, DtCO.PPCO
                    SetModPCO();
                    RSDT1.outSETPmax=M.SETPmax; 
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
                StartRTS(_RTCPGD);
                M._SYNCH_PGD=1;             //ustaw flage aktualizacji nastawy w plytce wyswietlacza
                return;                    
            }
        }
    }   
    M._AKTYWNE_PGD2=0;    
    if(RTS(_RTCPGD)>2)              //uplynal czas na synchronizacje nastaw?
    {
        M._SYNCH_PGD=0;             //zeruj flage aktualizacji nastawy w plytce wyswietlacza
    }    
}
/*---------------------------------------------------------------------------------*/
//Formatowanie nastawy CO
void SetModPCO(void)
{
	if(M._AKTYWNE_KNF)
	{
 		if(!DtKNF.opdg) DtCO.PPCO=_KNF_PCO;		//aktywna konfiguracja
		else DtCO.PPCO=_MAX_PDCO;				//dla ogrzewania podlogowego
        M.CONTSETP=DtCO.PPCO;
        M.SETPmax=DtCO.PPCO;        
	}
	else
	if(M._pob_CW||M._pob_AL||M._pob_DzCW)					//pobudzenie CW?
	{
		switch(DtKNF.tpkt)		//dla danego rodzaju kotla
		{
			case _BITERM:				//bitermiczny (miniterm)
			case _BITERM2:				//bitermiczny (maxiterm)
			{
				DtCO.PPCO=F1_PPCO;
				break;
			}
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
	}
	else
	if(M._AKTYWNE_PGD)
    {
        DtCO.PPCO=DtCO.PGDPCO;		//aktywna funkcja pogodowa
        M.SETPmax=DtCO.PPCO;
        if(M.SETPmin>M.SETPmax) M.SETPmin=M.SETPmax;
#if (_EXT_BOARD==0) 
        M.CONTSETP=DtCO.PGDPCO;
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
                M.CONTSETP=DtCO.PGDPCO;   //nastawa manualna             
            }
        }
        else
        {
            M.CONTSETP=DtCO.PGDPCO;
        }
#endif 
        
#if (_EXT_BOARD==2)   
        if(!M._AKTYWNE_PGD2)                                     //niezalezna praca regulatora pogodowego? 
        {        
            if(M._I10V_RT||M._LIN_STR||M._LIN_KSK)                   //nastawa zdalna?
            {   
                M.CONTSETP=RSDT1.inCONTSETP;
                if(M.CONTSETP>M.SETPmax) M.CONTSETP=M.SETPmax;
                else
                if(M.CONTSETP<M.SETPmin) M.CONTSETP=M.SETPmin;            
            }
            else
            {
                M.CONTSETP=DtCO.PGDPCO;   //nastawa manualna             
            }
        }
        else
        {
            M.CONTSETP=DtCO.PGDPCO;
        }            
#endif  
        
#if (_EXT_BOARD>=3)      
        if(!M._AKTYWNE_PGD2)                                     //niezalezna praca regulatora pogodowego? 
        {        
            if(M._I10V_RT||M._LIN_STR||M._LIN_KSK||M._IOT_STR)      //nastawa zdalna?
            {          
                M.CONTSETP=RSDT1.inCONTSETP;
                if(M.CONTSETP>M.SETPmax) M.CONTSETP=M.SETPmax;
                else
                if(M.CONTSETP<M.SETPmin) M.CONTSETP=M.SETPmin;             
            }
            else
            {
                M.CONTSETP=DtCO.PGDPCO;   //nastawa manualna            
            }
        }
        else
        {
            M.CONTSETP=DtCO.PGDPCO;
        }            
#endif          
    }
	else                    //nastawa zdalna lub wartosc PCO
	{
		DtCO.PPCO=DtCO.PCO;
        M.SETPmax=DtCO.PPCO;
        if(M.SETPmin>M.SETPmax) M.SETPmin=M.SETPmax;
        
#if (_EXT_BOARD==0)  
		M.CONTSETP=DtCO.PCO;
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
            M.CONTSETP=DtCO.PCO;   //nastawa manualna             
        }
#endif 
        
#if (_EXT_BOARD==2)        
        if(M._I10V_RT||M._LIN_STR||M._LIN_KSK)                   //nastawa zdalna?
        {   
            M.CONTSETP=RSDT1.inCONTSETP;
            if(M.CONTSETP>M.SETPmax) M.CONTSETP=M.SETPmax;
            else
            if(M.CONTSETP<M.SETPmin) M.CONTSETP=M.SETPmin;            
        }
        else
        {
            M.CONTSETP=DtCO.PCO;   //nastawa manualna             
        }
#endif    
        
#if (_EXT_BOARD>=3)        
        if(M._I10V_RT||M._LIN_STR||M._LIN_KSK||M._IOT_STR)      //nastawa zdalna?
        {          
            M.CONTSETP=RSDT1.inCONTSETP;
            if(M.CONTSETP>M.SETPmax) M.CONTSETP=M.SETPmax;
            else
            if(M.CONTSETP<M.SETPmin) M.CONTSETP=M.SETPmin;             
        }
        else
        {
            M.CONTSETP=DtCO.PCO;   //nastawa manualna            
        }
#endif        
	}

	if(!DtKNF.opdg)
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
	}
	if(M._pob_DzCO)
	{
		DtCO.PPCO=_OFF_DZ_CO;
		RSDT1.outPPCO=_OFF_DZ_CO;
        M.CONTSETP=DtCO.PPCO;
        M.SETPmax=DtCO.PPCO;        
	}
}
/*---------------------------------------------------------------------------------*/
//Oblicza modyfikator mocy maksymalnej dla grzania obiegu CO
//przy aktywnym regulatorze pogodowym
//gdzie:
//kt - nastawa PCO w zakresie 0...100%
//wspolczynnik krzywej grzania:
//WSP=0,0662*kt-0,0369
//a wartosc modyfikatora mocy maksymalnej [0...100%]:
//dla Tzw<25C PGD=WSP*(Tzw-25)
//dla Tzw>=25 PGD=0
/*void SetWspPGD(void)
{
	signed long m;
	if(M._AKTYWNE_PGD)		//aktywny regulator pogodowy?
	{
		if(DtCO.PCO)		//kt>0?
		{
			if(DtZW.SZW<25)	//Tzw<25?
			{
				m=-66*DtCO.PCO-37;
				m*=(DtZW.SZW-25);
				m/=1000;
				if(m>100) PWM.PGD=100;
				else
				if(m<0) PWM.PGD=0;
				else PWM.PGD=m;
			}
			else PWM.PGD=0;
		}
		else PWM.PGD=0;
	}else PWM.PGD=100;
}*/
/*---------------------------------------------------------------------------------*/
//Dla ogrzewania pogodowego:
//PPCO=_MIN_PCO+(_MAX_PCO-_MIN_PCO)*((25-Tzew)/(25-10))*kt/99
unsigned char GetPGD_PCO(void)
{
	unsigned char p;
	signed long m;

	if(!DtKNF.opdg)	//ogrzewanie tradycyjne
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
                return p;
			}
			else
			{ 
				p=_MIN_PCO;
				if((p+DtKNF.prop)<_MAX_PCO) p=p+DtKNF.prop;             //przesuniecie rownolegle
				else p=_MAX_PCO;
                if(DtKNF.pgmx>=_MIN_PCO)                                    //wartosc graniczna dla regulatora pogodowego
                {
                    if(p>DtKNF.pgmx) p=DtKNF.pgmx;
                }
                else p=_MIN_PCO;
                return p;
			}
		}
		else 
		{
			p=_MIN_PCO;
			if((p+DtKNF.prop)<_MAX_PCO) p=p+DtKNF.prop;             //przesuniecie rownolegle
			else p=_MAX_PCO;
            if(DtKNF.pgmx>=_MIN_PCO)                                    //wartosc graniczna dla regulatora pogodowego
            {
                if(p>DtKNF.pgmx) p=DtKNF.pgmx;
            }
            else p=_MIN_PCO;
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
                return p;
			}
			else// return _MIN_PDCO;
			{ 
				p=_MIN_PDCO;
				if((p+DtKNF.prop)<_MAX_PDCO) p=p+DtKNF.prop;             //przesuniecie rownolegle
				else p=_MAX_PDCO;
                if(DtKNF.pgmx>=_MIN_PDCO)                                    //wartosc graniczna dla regulatora pogodowego
                {
                    if(p>DtKNF.pgmx) p=DtKNF.pgmx;
                }
                else p=_MIN_PDCO;
                return p;
			}
		}
		else// return _MIN_PDCO;
		{ 
			p=_MIN_PDCO;
			if((p+DtKNF.prop)<_MAX_PDCO) p=p+DtKNF.prop;             //przesuniecie rownolegle
			else p=_MAX_PDCO;
            if(DtKNF.pgmx>=_MIN_PDCO)                                    //wartosc graniczna dla regulatora pogodowego
            {
                if(p>DtKNF.pgmx) p=DtKNF.pgmx;
            }
            else p=_MIN_PDCO;
            return p;
		}
	}
}
/*---------------------------------------------------------------------------------*/
//Odczyt nastawy PCO z pamieci EEPROM
unsigned char ReadPCO(void)
{
    if(!M._AKTYWNE_PGD)
    {
        return ReadEEPCO();    //odczyt nastawy z pamieci EEPROM
    }
    else
    {
        return ReadEEPPGD();   //odczyt nastawy z pamieci EEPROM
    }
}
/*---------------------------------------------------------------------------------*/
//Zapis nastawy PCW po ustalonym czasie do EEPROM
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
	unsigned char bnc;

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
    
	if(!GetBit2(RSDT1.inSTAT0,_INITDTA)&&MGSD._CON&&!M._PW_START&&!M._RS_START)
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
	}

    if(M._AKTYWNE_PGD)
    {
        if(DtZW._en_ZW) DtCO.PGDPCO=GetPGD_PCO(); 
    } 
	SetModPCO();
    RSDT1.outSETPmax=M.SETPmax;
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
		case _BITERM:				//bitermiczny (miniterm)
		case _BITERM2:				//bitermiczny (maxiterm)
		{
			DtCW.minPCW=F1_MIN_PCW;
			DtCW.maxPCW=F1_MAX_PCW;
			break;
		}
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
	if(!GetBit2(RSDT1.inSTAT0,_INITDTA)&&MGSD._CON&&!M._PW_START&&!M._RS_START)
	{

		pnst=RSDT1.inPCW;
		if(pnst<DtCW.minPCW) pnst=DtCW.minPCW;	//nastawa w dozwolonym zakresie?
		if(pnst>DtCW.maxPCW) pnst=DtCW.maxPCW;	//nastawa w dozwolonym zakresie?
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
		if(pnst<DtCW.minPCW) pnst=DtCW.minPCW;	//nastawa w dozwolonym zakresie?
		if(pnst>DtCW.maxPCW) pnst=DtCW.maxPCW;	//nastawa w dozwolonym zakresie?
		RSDT1.outPCW=DtCW.PCW;
	}

	SetModPCW();
	DtCW.DAD_CW=ReturnACfromNTC(DtCW.PPCW-(_BNCW-1));
	DtCW.MAD_CW=ReturnACfromNTC(DtCW.PPCW-(_BNCW-1)-_MR_MADCW);
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
		WriteEEPFN();
	}
}
/*---------------------------------------------------------------------------------*/
//Nastawa przelacznika funkcyjnego
void PomiarPFN(void)
{
	static unsigned char pnst;

	if(!GetBit2(RSDT1.inSTAT0,_INITDTA)&&MGSD._CON&&!M._PW_START&&!M._RS_START)
	{
		if(GetBit2(RSDT1.inSTAT0,_PFN_OFF))	
		{
			PFN._zima=0;
			PFN._lato=0;
			PFN._off=1;
			PFN._csn=0;
			PFN.poz=_PFOFF;
			BitSet2(&RSDT1.outSTAT2,_F_ZIMA,0);
			BitSet2(&RSDT1.outSTAT2,_F_LATO,0);
			BitSet2(&RSDT1.outSTAT2,_F_OFF,1);
		}
		else
		if(GetBit2(RSDT1.inSTAT0,_PFN_ZIMA))
		{
			PFN._zima=1;
			PFN._lato=0;
			PFN._off=0;
			PFN._csn=0;
			PFN.poz=_PFZMA;
			BitSet2(&RSDT1.outSTAT2,_F_ZIMA,1);
			BitSet2(&RSDT1.outSTAT2,_F_LATO,0);
			BitSet2(&RSDT1.outSTAT2,_F_OFF,0);
		}
		else
		if(GetBit2(RSDT1.inSTAT0,_PFN_LATO))	
		{
			PFN._zima=0;
			PFN._lato=1;
			PFN._off=0;
			PFN._csn=0;
			PFN.poz=_PFLTO;
			BitSet2(&RSDT1.outSTAT2,_F_ZIMA,0);
			BitSet2(&RSDT1.outSTAT2,_F_LATO,1);
			BitSet2(&RSDT1.outSTAT2,_F_OFF,0);
		}
		else
		{
			PFN._zima=1;
			PFN._lato=0;
			PFN._off=0;
			PFN._csn=0;
			PFN.poz=_PFZMA;
			BitSet2(&RSDT1.outSTAT2,_F_ZIMA,1);
			BitSet2(&RSDT1.outSTAT2,_F_LATO,0);
			BitSet2(&RSDT1.outSTAT2,_F_OFF,0);
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
		/*pnst=ReadEEPFN();
		if(PFN.poz!=pnst)	//nowa wartosc pozycji PFN?
		{
			if(!PFN._newPFN) StartRTdS(_RTPFN);
			PFN._newPFN=1;
		}
		WritePFN();			//zapisz nastawe do pamieci
        */
		if(GetBit2(RSDT1.inSTAT0,_PFN_RES))	PFN._res=1;
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
	static unsigned char pcs;
	unsigned char dac,cs;
	dac=ReadAC(_CHN_CS);
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
			cs=((dac-_CS_0)*10)/(((_CS_4-_CS_0)*10)/_NWCS);
			if(cs<_NWCS)
				if((cs-pcs>0)&&(cs-pcs)<2) cs=pcs;				//filtr wskazu cisnienia
			if(cs>_NWCS) cs=_NWCS;
		}
		else												//czujnik 5V
		{
			if(dac<_CS5_0) dac=_CS5_0;
			if(dac>_CS5_4) dac=_CS5_4;
			cs=((dac-_CS5_0)*10)/(((_CS5_4-_CS5_0)*10)/_NWCS5);
			if(cs<_NWCS5)
				if((cs-pcs>0)&&(cs-pcs)<2) cs=pcs;				//filtr wskazu cisnienia
			if(cs>_NWCS5) cs=_NWCS5;
		}
		pcs=cs;
		DtCS.CS=cs;
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
}
/*---------------------------------------------------------------------------------*/
