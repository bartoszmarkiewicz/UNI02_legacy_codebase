/*pomiar.c*/
//Procedury pomiarowe (oparte na przetworniku AC) UNISTER zmodyfikowany GRZ01
// (!) Bazuje na funkcjach z pliku: ac.c, rtimer.c /RTdS(2)/
/*---------------------------------------------------------------------------------*/
#include <pic18.h>
#include "global.h"				//parametry globalne
#include "pomiar.h"				//parametry lokalne
#include "konfig2.h"
#include "main.tp.h"
#include "pwm.h"
#include "smgs_rs9b.tp.h"			

volatile DataCO DtCO;
volatile DataIN DtIN;
volatile DataCW DtCW;
volatile DataZW DtZW;
volatile DataCSN DtCS;

extern tpMAIN M;
extern DataPFN PFN;
extern DtKONF DtKNF;
extern DtPWM PWM;

volatile static unsigned char PmrtimFlt1, PmrtimFlt2;
volatile static unsigned int PmrtimFlt3;
volatile unsigned char wff[12];

#if _SRVTRB==1						//aktywny tryb serwisowy kompilacji?
unsigned char mypwm = _MG2_MOD;
#endif
/*---------------------------------------------------------------------------------*/
//Inicjalizacja DtCO, DtCW, DtCS, PFN

void InitDataPmr(void)
{
    DtCOf_err_CO = 0;
    DtCOf_su_CO = 0;
    DtCOf_ad_CO = 0;
    DtCOf_su_SRW_CO = 0;
    DtCOf_ad_SRW_CO = 0;
    DtCOf_cold_CO = 0;
    DtCOf_hot_CO = 0;
    DtCOf_on_DZ_CO = 0;
    DtCOf_su_DZ_CO = 0;
    DtCOf_ad_DZ_CO = 0;
    DtCOf_ptwp_CO = 0;
    DtCOf_pz_CO = 0;
    DtCO.indCO = 0;

    DtCWf_err_CW = 0;
    DtCWf_su_CW = 0;
    DtCWf_ad_CW = 0;
    DtCWf_on_DZ_CW = 0;
    DtCWf_su_DZ_CW = 0;
    DtCWf_ad_DZ_CW = 0;
    DtCW.indCW = 0;

    DtCSf_hi = 0;
    DtCSf_lo = 0;
    DtCSf_no = 0;
    DtCSf_error = 0;
    DtCSf_work = 1; //przed pomiarem domyslne cisnienie robocze
}
//-------------------------------------------------------------------------------
//Timer na potrzeby filtracji (umiescic w przerwaniu od zegara TMR1)

void IncPmrTimer(void)
{
    if(PmrtimFlt1 < 0xff) PmrtimFlt1++;
    if(PmrtimFlt2 < 0xff) PmrtimFlt2++;
    if(PmrtimFlt3 < 0xffff) PmrtimFlt3++;
}
//-------------------------------------------------------------------------------
//Zegar 1 odcinkow czasowych

void StartTim1(void)
{
    PmrtimFlt1 = 0;
}

bit PmrTim1(unsigned char tm)
{
    if(PmrtimFlt1 < tm) return 0;
    return 1;
}
//-------------------------------------------------------------------------------
//Zegar 2 odcinkow czasowych

void StartTim2(void)
{
    PmrtimFlt2 = 0;
}

bit PmrTim2(unsigned char tm)
{
    if(PmrtimFlt2 < tm) return 0;
    return 1;
}
//-------------------------------------------------------------------------------
//Zegar 3 odcinkow czasowych

void StartTim3(void)
{
    PmrtimFlt3 = 0;
}

bit PmrTim3(unsigned int tm)
{
    if(PmrtimFlt3 < tm) return 0;
    return 1;
}
/*---------------------------------------------------------------------------------*/
//Filtracja wartosci temperatur na cele wizualizacji.

void GoldenEye(void)
{
    signed char dt;
    signed char pop;
    //----------
    //wskaz CO
    dt = DtCO.PPCO - DtCO.CO;
    if((DtCO.CO >= (DtCO.PPCO + _DM_GECO)) || (DtCO.CO <= (DtCO.PPCO - _DM_GECO))) DtCO.SCO = DtCO.CO;
    else
    {
        if(dt >= 0)
        {
            if(dt < _BNCO) pop = dt;
            else
                if((dt >= _BNCO) && (dt <= (_DM_GECO - _BNCO))) pop = _BNCO;
            else
                if(dt > (_DM_GECO - _BNCO)) pop = _DM_GECO - dt;
            DtCO.SCO = DtCO.CO + pop;
        }
        else    //********* proc poprawiona dla gornego marginesu wylaczenia=10 (a nie 5)
        {
            if(-1 * dt < _BNCO) pop = _BNCO + dt;
            else
                if((-1 * dt >= _BNCO) && (-1 * dt <= (_DM_GECO - _BNCO))) pop = _BNCO;
            else
                //if(-1 * dt > (_DM_GECO - _BNCO)) pop = _DM_GECO + dt;
                pop=0;
            DtCO.SCO = DtCO.CO - pop;
        }
    }
    PFN.CO = DtCO.SCO;
    //----------
    //wskaz CW
    if(DtKNF.tpkt != _UNICO) //kociol inny niz UniCO?
    {
        if(DtKNF.tpkt == _BITERM)
        {
            dt = DtCW.PPCW - DtCW.CW;
            if((DtCW.CW >= (DtCW.PPCW + _DM_GECW2)) || (DtCW.CW <= (DtCW.PPCW - _DM_GECW2))) DtCW.SCW = DtCW.CW;
            else
            {
                if(dt >= 0)
                {
                    if(dt < _BNCW2) pop = dt;
                    else
                        if((dt >= _BNCW2) && (dt <= (_DM_GECW2 - _BNCW2))) pop = _BNCW2;
                    else
                        if(dt > (_DM_GECW2 - _BNCW2)) pop = _DM_GECW2 - dt;
                    DtCW.SCW = DtCW.CW + pop;
                }
                else
                {
                    if((-1 * dt) < _BNCW2) pop = (-1 * dt);
                    else
                        if(((-1 * dt) >= _BNCW2) && ((-1 * dt) <= (_DM_GECW2 - _BNCW2))) pop = _BNCW2;
                    else
                        if((-1 * dt)>(_DM_GECW2 - _BNCW2)) pop = _DM_GECW2 + dt;
                    DtCW.SCW = DtCW.CW - pop;
                }
            }
        }
        else
        {
            dt = DtCW.PPCW - DtCW.CW;
            if((DtCW.CW >= (DtCW.PPCW + _DM_GECW)) || (DtCW.CW <= (DtCW.PPCW - _DM_GECW))) DtCW.SCW = DtCW.CW;
            else
            {
                if(dt >= 0)
                {
                    if(dt < _BNCW) pop = dt;
                    else
                        if((dt >= _BNCW) && (dt <= (_DM_GECW - _BNCW))) pop = _BNCW;
                    else
                        if(dt > (_DM_GECW - _BNCW)) pop = _DM_GECW - dt;
                    DtCW.SCW = DtCW.CW + pop;
                }
                else
                {
                    if(-1 * dt < _BNCW) pop = _BNCW + dt;
                    else
                        if((-1 * dt >= _BNCW) && (-1 * dt <= (_DM_GECW - _BNCW))) pop = _BNCW;
                    else
                        if(-1 * dt > (_DM_GECW - _BNCW)) pop = _DM_GECW + dt;
                    DtCW.SCW = DtCW.CW - pop;
                }
            }
        }
    }
    else DtCW.SCW = DtCW.CW; //kociol zasobnikowy - brak filtracji wskazu temp. CW
    PFN.CW = DtCW.SCW;
}
/*---------------------------------------------------------------------------------*/
//Filtr dla danych wejsciowych modulatora.

void PredkoscZmianCW(void)
{
    static unsigned char ffcw, indff;
    unsigned char i, w;

    if(!Mf_PRACA)
    {
        ffcw = 0;
        indff = 0;
    }
    else
    {
        if(!indff)
        {
            for(i = 0; i < 10; i++) wff[i] = 0;
        }
        indff++;
        if(indff >= 9)
        {
            indff = 9;
            for(i = 0; i < 9; i++) wff[i] = wff[i + 1]; //FIFO
        }
        wff[indff] = DtCW.CW - ffcw; //przyrost/spadek
    }
    ffcw = DtCW.CW;
    w = 0;
    for(i = 0; i < 10; i++) w += wff[i];
    DtCW.ddDCW = w;
}
/*---------------------------------------------------------------------------------*/
//Filtr dla danych wejsciowych modulatora.

void FltParMod(void)
{
    static unsigned char indtm, fco, fcw;
    unsigned char wr;

    //----------
    //wykonywane co 0.1 sek.
    //CO
    if(DtCO.CO > fco) wr = DtCO.CO - fco;
    else wr = fco - DtCO.CO;
    if(wr) DtCO.indCO++;
    if(wr >= 2) DtCO.indCO = 10;
    //CW
    if(DtCW.CW > fcw) wr = DtCW.CW - fcw;
    else wr = fcw - DtCW.CW;
    if(wr) DtCW.indCW++;
    if(wr >= 2) DtCW.indCW = 10;
    PredkoscZmianCW();
    //----------
    //wykonywane co 1 sek.
    indtm++;
    if(indtm > 9)
    {
        indtm = 0;

        DtZW.SZW = DtZW.ZW; //wartosc temp zewnwtrznej (aktualizacja co 1 sek.)

        //szybkosc i kierunek zmian temp CO
        if(DtCO.indCO <= 7) DtCO.dDCO = 0; //temp = const
        else
        {
            DtCO.dDCO = DtCO.CO - fco;
            fco = DtCO.CO;
        }
        DtCO.indCO = 0;
        //szybkosc i kierunek zmian temp CW
        if(DtCW.indCW <= 7)
        {
            DtCW.dDCW = 0; //temp = const
            DtCWf_fpCW = 1;
        }
        else
        {
            DtCW.dDCW = DtCW.CW - fcw;
            DtCWf_fpCW = 1;
            fcw = DtCW.CW;
        }
        DtCW.indCW = 0;
    }
}
/*---------------------------------------------------------------------------------*/
//Filtracja wartosci (nowa wartosc odswiezana jest co 0.5 sek)
//(umiescic w petli glownej programu po procedurach pomiaru CO,CW,PCO,PCW)

void FiltrPomiarowyCOCW(void)
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
    DtCO.DCO = (DtCO.PPCO - _BNCO) - DtCO.CO; //wielkosc bledu CO

    DtCW.DCW = (DtCW.PPCW - (_BNCW - 1)) - DtCW.CW; //wielkosc bledu CW

    if(!DtINf_err_IN)
    {
        //delta wzgledem temp. wyj. CO
        if(DtCO.CO > DtIN.IN) DtIN.DIN = DtCO.CO - DtIN.IN;
        else DtIN.DIN = 0;
        //delta wzgledem nastawy CO
        if((DtCO.PPCO - (_BNCO - 1)) > DtIN.IN) DtIN.DIN2 = (DtCO.PPCO - (_BNCO - 1)) - DtIN.IN;
        else DtIN.DIN2 = 0;
    }

    if(PmrTim2(_ITFTM))
    {
        StartTim2();
        FltParMod();
    }
    if(DtCO.dDCO >= _MAX_DXCO) DtCOf_ldDCO = 1; //b.szybki narost temperatury?
    else DtCOf_ldDCO = 0;
}
/*---------------------------------------------------------------------------------*/
//Pomiar wartosci CO i ustawienie odpowiednich flag stanu

void PomiarCO(void)
{
    DtCO.AC_CO = ReadAC(_CHN_CO);
    DtCO.CO = ReadTabNTC(DtCO.AC_CO);

    if(DtCO.AC_CO <= _NTC_MIN || DtCO.AC_CO >= _NTC_MAX) //stwierdzenie awarii czujnika temp. CO
    {
        DtCOf_err_CO = 1;
        DtCOf_su_CO = 0;
        DtCOf_ad_CO = 0;
        DtCOf_cold_CO = 0;
        DtCOf_hot_CO = 0;
        DtCOf_on_DZ_CO = 0;
        DtCOf_su_DZ_CO = 0;
        DtCOf_ad_DZ_CO = 0;
        DtCOf_ptwp_CO = 0;
        DtCOf_pz_CO = 0;
        DtCOf_su_SRW_CO = 0;
        DtCOf_ad_SRW_CO = 0;
        return;
    }
    else DtCOf_err_CO = 0;
    switch(DtKNF.tpkt) //dla danego rodzaju kotla
    {
        case _BITERM: //bitermiczny (miniterm)
        case _BITERM2: //bitermiczny (maxiterm)
        {
            //if((DtCO.CO - DtCO.PPCO) >= F1_AD_CO) DtCOf_ad_CO = 1;
            //else DtCOf_ad_CO = 0;
            if(DtCO.PPCO>(_MAX_PCO-5))	//nastawa > 80?
            {
                    if(DtCO.PPCO<=_MAX_PCO)	//nastawa <= _MAX_PCO?
                    {
                            if((DtCO.CO-_MAX_PCO)>=F1_AD_CO) DtCOf_ad_CO=1;
                            else DtCOf_ad_CO=0;
                    }
                    else
                    {
                            if((DtCO.CO-DtCO.PPCO)>=F1_AD_CO) DtCOf_ad_CO=1;
                            else DtCOf_ad_CO=0;
                    }
            }
            else	//nastawa <= 80?
            {
                    if((DtCO.CO-DtCO.PPCO)>=F1_AD_CO2) DtCOf_ad_CO=1;
                    else DtCOf_ad_CO=0;
            }
            if((DtCO.PPCO - DtCO.CO) >= F1_SU_CO) DtCOf_su_CO = 1;
            else DtCOf_su_CO = 0;
            if(DtCO.CO <= F1_PTWP_CO) DtCOf_ptwp_CO = 1;
            if(DtCO.CO > F1_PTWP_CO + 1) DtCOf_ptwp_CO = 0;
            if(DtCO.CO <= F1_PZ_CO) DtCOf_pz_CO = 1;
            if(DtCO.CO > F1_PW_CO) DtCOf_pz_CO = 0;
            if(DtCO.CO <= F1_CLD_CO) DtCOf_cold_CO = 1;
            if(DtCO.CO > F1_CLD_CO + 1) DtCOf_cold_CO = 0;
            if(DtCO.CO >= F1_HOT_CO) DtCOf_hot_CO = 1;
            if(DtCO.CO < F1_HOT_CO - 1) DtCOf_hot_CO = 0;
            break;
        }
        case _MONOHB:
        {
            //if((DtCO.CO - DtCO.PPCO) >= F2_AD_CO) DtCOf_ad_CO = 1;
            //else DtCOf_ad_CO = 0;
            if(DtCO.PPCO>(_MAX_PCO-5))	//nastawa > 80?
            {
                    if(DtCO.PPCO<=_MAX_PCO)	//nastawa <= _MAX_PCO?
                    {
                            if((DtCO.CO-_MAX_PCO)>=F2_AD_CO) DtCOf_ad_CO=1;
                            else DtCOf_ad_CO=0;
                    }
                    else
                    {
                            if((DtCO.CO-DtCO.PPCO)>=F2_AD_CO) DtCOf_ad_CO=1;
                            else DtCOf_ad_CO=0;
                    }
            }
            else	//nastawa <= 80?
            {
                    if((DtCO.CO-DtCO.PPCO)>=F2_AD_CO2) DtCOf_ad_CO=1;
                    else DtCOf_ad_CO=0;
            }
            if((DtCO.PPCO - DtCO.CO) >= F2_SU_CO) DtCOf_su_CO = 1;
            else DtCOf_su_CO = 0;
            if(DtCO.CO <= F2_PTWP_CO) DtCOf_ptwp_CO = 1;
            if(DtCO.CO > F2_PTWP_CO + 1) DtCOf_ptwp_CO = 0;
            if(DtCO.CO <= F2_PZ_CO) DtCOf_pz_CO = 1;
            if(DtCO.CO > F2_PW_CO) DtCOf_pz_CO = 0;
            if(DtCO.CO <= F2_CLD_CO) DtCOf_cold_CO = 1;
            if(DtCO.CO > F2_CLD_CO + 1) DtCOf_cold_CO = 0;
            if(DtCO.CO >= F2_HOT_CO) DtCOf_hot_CO = 1;
            if(DtCO.CO < F2_HOT_CO - 1) DtCOf_hot_CO = 0;
            break;
        }
        case _UNICO: //kociol unico
        {
            //if((DtCO.CO - DtCO.PPCO) >= F3_AD_CO) DtCOf_ad_CO = 1;
            //else DtCOf_ad_CO = 0;
            if(DtCO.PPCO>(_MAX_PCO-5))	//nastawa > 80?
            {
                    if(DtCO.PPCO<=_MAX_PCO)	//nastawa <= _MAX_PCO?
                    {
                            if((DtCO.CO-_MAX_PCO)>=F3_AD_CO) DtCOf_ad_CO=1;
                            else DtCOf_ad_CO=0;
                    }
                    else
                    {
                            if((DtCO.CO-DtCO.PPCO)>=F3_AD_CO) DtCOf_ad_CO=1;
                            else DtCOf_ad_CO=0;
                    }
            }
            else	//nastawa <= 80?
            {
                    if((DtCO.CO-DtCO.PPCO)>=F3_AD_CO2) DtCOf_ad_CO=1;
                    else DtCOf_ad_CO=0;
            }  
            if((DtCO.PPCO - DtCO.CO) >= F3_SU_CO) DtCOf_su_CO = 1;
            else DtCOf_su_CO = 0;
            if(DtCO.CO <= F3_PTWP_CO) DtCOf_ptwp_CO = 1;
            if(DtCO.CO > F3_PTWP_CO + 1) DtCOf_ptwp_CO = 0;
            if(DtCO.CO <= F3_PZ_CO) DtCOf_pz_CO = 1;
            if(DtCO.CO > F3_PW_CO) DtCOf_pz_CO = 0;
            if(DtCO.CO <= F3_CLD_CO) DtCOf_cold_CO = 1;
            if(DtCO.CO > F3_CLD_CO + 1) DtCOf_cold_CO = 0;
            if(DtCO.CO >= F3_HOT_CO) DtCOf_hot_CO = 1;
            if(DtCO.CO < F3_HOT_CO - 1) DtCOf_hot_CO = 0;
            break;
        }
        default: //domyslny monotermiczny
        {
            //if((DtCO.CO - DtCO.PPCO) >= F2_AD_CO) DtCOf_ad_CO = 1;
            //else DtCOf_ad_CO = 0;
            if(DtCO.PPCO>(_MAX_PCO-5))	//nastawa > 80?
            {
                    if(DtCO.PPCO<=_MAX_PCO)	//nastawa <= _MAX_PCO?
                    {
                            if((DtCO.CO-_MAX_PCO)>=F2_AD_CO) DtCOf_ad_CO=1;
                            else DtCOf_ad_CO=0;
                    }
                    else
                    {
                            if((DtCO.CO-DtCO.PPCO)>=F2_AD_CO) DtCOf_ad_CO=1;
                            else DtCOf_ad_CO=0;
                    }
            }
            else	//nastawa <= 80?
            {
                    if((DtCO.CO-DtCO.PPCO)>=F2_AD_CO2) DtCOf_ad_CO=1;
                    else DtCOf_ad_CO=0;
            } 
            if((DtCO.PPCO - DtCO.CO) >= F2_SU_CO) DtCOf_su_CO = 1;
            else DtCOf_su_CO = 0;
            if(DtCO.CO <= F2_PTWP_CO) DtCOf_ptwp_CO = 1;
            if(DtCO.CO > F2_PTWP_CO + 1) DtCOf_ptwp_CO = 0;
            if(DtCO.CO <= F2_PZ_CO) DtCOf_pz_CO = 1;
            if(DtCO.CO > F2_PW_CO) DtCOf_pz_CO = 0;
            if(DtCO.CO <= F2_CLD_CO) DtCOf_cold_CO = 1;
            if(DtCO.CO > F2_CLD_CO + 1) DtCOf_cold_CO = 0;
            if(DtCO.CO >= F2_HOT_CO) DtCOf_hot_CO = 1;
            if(DtCO.CO < F2_HOT_CO - 1) DtCOf_hot_CO = 0;
            break;
        }
    }

    if(DtCO.CO <= _ON_DZ_CO)
    {
        DtCOf_su_DZ_CO = 1;
        DtCOf_on_DZ_CO = 1;
    }
    else DtCOf_su_DZ_CO = 0;
    if(DtCO.CO >= _OFF_DZ_CO)
    {
        DtCOf_ad_DZ_CO = 1;
        DtCOf_on_DZ_CO = 0;
    }
    else DtCOf_ad_DZ_CO = 0;

    if(!DtKNF.opdg)
    {
        if(DtCO.CO <= _ON_SRW_CO) DtCOf_su_SRW_CO = 1;
        else DtCOf_su_SRW_CO = 0;
        if(DtCO.CO > _OFF_SRW_CO) DtCOf_ad_SRW_CO = 1;
        else DtCOf_ad_SRW_CO = 0;
    }
    else //ogrzewanie podlogowe
    {
        if(DtCO.CO <= _ONP_SRW_CO) DtCOf_su_SRW_CO = 1;
        else DtCOf_su_SRW_CO = 0;
        if(DtCO.CO > _OFFP_SRW_CO) DtCOf_ad_SRW_CO = 1;
        else DtCOf_ad_SRW_CO = 0;
    }
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Pomiar wartosci temperatury na wlocie i ustawienie odpowiednich flag stanu

void PomiarIN(void)
{
    //	signed char delta;

    DtIN.AC_IN = ReadAC(_CHN_IN);
    DtIN.IN = ReadTabNTC(DtIN.AC_IN);
    PFN.IN = DtIN.IN;
    if(DtIN.AC_IN <= _NTC_MIN || DtIN.AC_IN >= _NTC_MAX) //stwierdzenie awarii czujnika temp. IN
    {
        DtINf_err_IN = 1;
        return;
    }
    else DtINf_err_IN = 0;
}
/*---------------------------------------------------------------------------------*/
//Pomiar wartosci CW i ustawienie odpowiednich flag stanu

void PomiarCW(void)
{
    DtCW.AC_CW = ReadAC(_CHN_CW);
    DtCW.CW = ReadTabNTC(DtCW.AC_CW);

    if(DtCW.AC_CW <= _NTC_MIN || DtCW.AC_CW >= _NTC_MAX) //stwierdzenie awarii czujnika temp. CO
    {
        DtCWf_err_CW = 1;
        DtCWf_su_CW = 0;
        DtCWf_ad_CW = 0;
        DtCWf_cold_CW = 0;
        DtCWf_on_DZ_CW = 0;
        DtCWf_su_DZ_CW = 0;
        DtCWf_ad_DZ_CW = 0;
        DtCWf_wpz_CW = 0;
        return;
    }
    else DtCWf_err_CW = 0;
    
    switch(DtKNF.tpkt) //dla danego rodzaju kotla
    {
        case _BITERM: //bitermiczny (miniterm)
        case _BITERM2: //bitermiczny (maxiterm)
        {
            /*if(DtCW.CW>F1_OFF_CW) DtCWf_ad_CW=1;
            else DtCWf_ad_CW=0;
            if(DtCW.CW<=F1_OFF_CW-F1_MR_OFF_CW) DtCWf_su_CW=1;
            else DtCWf_su_CW=0;*/
            if(DtCW.PCW < 55) //dwa bezwzgledne progi wylaczenia
            {
                if(DtCW.CW > F1_OFF_CW) DtCWf_ad_CW = 1;
                else DtCWf_ad_CW = 0;
                if(DtCW.CW <= F1_OFF_CW - F1_MR_OFF_CW) DtCWf_su_CW = 1;
                else DtCWf_su_CW = 0;
            }
            else
            {
                if(DtCW.CW > F1_OFF2_CW) DtCWf_ad_CW = 1;
                else DtCWf_ad_CW = 0;
                if(DtCW.CW <= F1_OFF2_CW - F1_MR_OFF_CW) DtCWf_su_CW = 1;
                else DtCWf_su_CW = 0;
            }

            break;
        }
        case _MONOHB:
        {
            if(DtCW.CW > F2_OFF_CW) DtCWf_ad_CW = 1;
            else DtCWf_ad_CW = 0;
            if(DtCW.CW <= F2_OFF_CW - F2_MR_OFF_CW) DtCWf_su_CW = 1;
            else DtCWf_su_CW = 0;
            break;
        }
        case _UNICO: //kociol unico
        {
            if((DtCW.CW - DtCW.PPCW) >= F3_AD_CW) DtCWf_ad_CW = 1;
            else DtCWf_ad_CW = 0;
            if((DtCW.PPCW - DtCW.CW) >= F3_SU_CW) DtCWf_su_CW = 1;
            else DtCWf_su_CW = 0;
            if(RdPrt(S_WCW)) //aktywne pobudzenie z timera zasobnika?
            {
                if(DtCWf_su_CW) DtCWf_wpz_CW = 1;
                else if(!Mf_PRACA) DtCWf_wpz_CW = 0;
                if(DtCWf_ad_CW) DtCWf_wpz_CW = 0;
            }
            else DtCWf_wpz_CW = 0;
            break;
        }
        default: //domyslny monotermiczny
        {
            if(DtCW.CW > F2_OFF_CW) DtCWf_ad_CW = 1;
            else DtCWf_ad_CW = 0;
            if(DtCW.CW <= F2_OFF_CW - F2_MR_OFF_CW) DtCWf_su_CW = 1;
            else DtCWf_su_CW = 0;
            break;
        }
    }
    if(DtCW.CW <= _ON_DZ_CW)
    {
        DtCWf_su_DZ_CW = 1;
        DtCWf_on_DZ_CW = 1;
    }
    else DtCWf_su_DZ_CW = 0;
    if(DtCW.CW >= _OFF_DZ_CW)
    {
        DtCWf_ad_DZ_CW = 1;
        DtCWf_on_DZ_CW = 0;
    }
    else DtCWf_ad_DZ_CW = 0;
    if(DtCW.CW <= _CLD_CW) DtCWf_cold_CW = 1;
    if(DtCW.CW > _CLD_CW + 1) DtCWf_cold_CW = 0;
}
/*---------------------------------------------------------------------------------*/
//Pomiar wartosci temperatury zewnetrznej i ustawienie odpowiednich flag stanu

void PomiarZW(void)
{
    DtZW.AC_ZW = ReadAC(_CHN_ZW);
    DtZW.ZW = ReadTabZNTC(DtZW.AC_ZW);
    if(DtZW.AC_ZW <= _ZNTC_MIN || DtZW.AC_ZW >= _ZNTC_MAX) DtZWf_on_ZW = 0; //stwierdzenie awarii czujnika temp. CO
    else DtZWf_on_ZW = 1;
}
/*---------------------------------------------------------------------------------*/
//Formatowanie nastawy CO

void SetModPCO(void)
{
    if(Mf_AKTYWNE_KNF)
    {
        if(!DtKNF.opdg) DtCO.PPCO = _KNF_PCO; //aktywna konfiguracja
        else DtCO.PPCO = _MAX_PDCO; //dla ogrzewania podlogowego
    }
    else
    if(Mf_pob_CW||Mf_pob_AL||Mf_pob_DzCW) //pobudzenie CW?
    {
        switch(DtKNF.tpkt) //dla danego rodzaju kotla
        {
            case _BITERM: //bitermiczny (miniterm)
            case _BITERM2: //bitermiczny (maxiterm)
            {
                DtCO.PPCO = F1_PPCO;
                break;
            }
            case _MONOHB:
            {
                DtCO.PPCO = F2_PPCO;
                break;
            }
            case _UNICO: //kociol unico
            {
				if(Mf_pob_CW||Mf_pob_AL)
				{
					DtCO.PPCO=F3_PPCO;
				}
				else
				{
					DtCO.PPCO=F3_PW_CO;
				}                
                break;
            }
            default: //domyslny monotermiczny
            {
                DtCO.PPCO = F2_PPCO;
                break;
            }
        }
    }
    else
    if(Mf_AKTYWNE_PGD) DtCO.PPCO = DtCO.PGDPCO; //aktywna funkcja pogodowa
    else
    {
        DtCO.PPCO = DtCO.PCO;
    }
    
    if(!DtKNF.opdg)
    {
        if(!Mf_pob_SRW) PFN.PPCO = DtCO.PPCO;
        else 
        {
            DtCO.PPCO = _OFF_SRW_CO;
            PFN.PPCO = _OFF_SRW_CO;
        }
    }
    else //dla aktywnego ogrzewania podlogowego
    {
        if(!Mf_pob_SRW) PFN.PPCO = DtCO.PPCO;
        else 
        {
            DtCO.PPCO = _OFFP_SRW_CO;
            PFN.PPCO = _OFFP_SRW_CO;
        }
    }
    if(Mf_pob_DzCO)
    {
        DtCO.PPCO = _OFF_DZ_CO;
        PFN.PPCO = _OFF_DZ_CO;
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
        if(Mf_AKTYWNE_PGD)		//aktywny regulator pogodowy?
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

    if(!DtKNF.opdg) //ogrzewanie tradycyjne
    {
        if(DtCO.PCO) //kt>0?
        {
            if(DtZW.SZW < 25) //Tzw<25?
            {
                m = (25 - DtZW.SZW) * DtCO.PCO;
                m *= (_MAX_PCO - _MIN_PCO);
                m *= 100;
                m /= 1485; //(25-10)*99
                if((_MIN_PCO + m / 100) > _MAX_PCO) return _MAX_PCO;
                else p = _MIN_PCO + m / 100;
                if(p < _MIN_PCO) return _MIN_PCO;
                return p;
            }
            else return _MIN_PCO;
        }
        else return _MIN_PCO;
    }
    else //ogrzewanie podlogowe
    {
        if(DtCO.PCO) //kt>0?
        {
            if(DtZW.SZW < 25) //Tzw<25?
            {
                m = (25 - DtZW.SZW) * DtCO.PCO;
                m *= (_MAX_PDCO - _MIN_PDCO);
                m *= 100;
                m /= 1485; //(25-10)*99
                if((_MIN_PDCO + m / 100) > _MAX_PDCO) return _MAX_PDCO;
                else p = _MIN_PDCO + m / 100;
                if(p < _MIN_PDCO) return _MIN_PDCO;
                return p;
            }
            else return _MIN_PDCO;
        }
        else return _MIN_PDCO;
    }
}
/*---------------------------------------------------------------------------------*/
//Zapis nastawy PCW po ustalonym czasie do EEPROM

void WritePCO(void)
{
    if(DtCOf_newPCO && RTdS(_RTPCO) >= _TWRTPCO)
    {
        DtCOf_newPCO = 0;
        StartRTdS(_RTPCO);
        WriteEEPCO();
    }
}
/*---------------------------------------------------------------------------------*/
//Nastawa PCO

void PomiarPCO(void)
{
    static unsigned char pnst;
    //unsigned char dac,dz,nst;

    if(!Mf_AKTYWNE_PGD)
    {
        if(!DtKNF.opdg)
        {
            DtCO.minPCO = _MIN_PCO;
            DtCO.maxPCO = _MAX_PCO;
        }
        else
        {
            DtCO.minPCO = _MIN_PDCO;
            DtCO.maxPCO = _MAX_PDCO;
        }
    }
    else
    {
        DtCO.minPCO = _MIN_PGD;
        DtCO.maxPCO = _MAX_PGD;
    }
    pnst = PFN.PCO;
    if(pnst < DtCO.minPCO) pnst = DtCO.minPCO; //nastawa w dozwolonym zakresie?
    if(pnst > DtCO.maxPCO) pnst = DtCO.maxPCO; //nastawa w dozwolonym zakresie?
    if(DtCO.PCO != pnst) //nowa wartosc nastawy?
    {
        DtCO.PCO = pnst;
        DtCOf_newPCO = 1;
        StartRTdS(_RTPCO);
    }
    WritePCO(); //zapisz nastawe do pamieci
    if(Mf_AKTYWNE_PGD) DtCO.PGDPCO = GetPGD_PCO();
    SetModPCO();
    DtCO.DAD_CO = ReturnACfromNTC(DtCO.PPCO - _BNCO);
    DtCO.MAD_CO = ReturnACfromNTC(DtCO.PPCO - _BNCO - _MR_MADCO);
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Zapis nastawy PCW po ustalonym czasie do EEPROM

void WritePCW(void)
{
    if(DtCWf_newPCW && RTdS(_RTPCW) >= _TWRTPCW)
    {
        DtCWf_newPCW = 0;
        StartRTdS(_RTPCW);
        WriteEEPCW();
    }
}
/*---------------------------------------------------------------------------------*/
//Formatowanie nastawy CW

void SetModPCW(void)
{
    if(Mf_AKTYWNE_KNF) DtCW.PPCW = _KNF_PCW; //aktywna konfiguracja?
    else
    if((DtKNF.tpkt == _UNICO) && Mf_pob_AL) DtCW.PPCW = _AL_PCW; //aktywna f. AntyLegionella
    else DtCW.PPCW = DtCW.PCW;

    if(Mf_pob_DzCW) 
    {
        DtCW.PPCW = _OFF_DZ_CW;
    }
    PFN.PPCW = DtCW.PPCW;
}
/*---------------------------------------------------------------------------------*/
//Nastawa PCW

void PomiarPCW(void)
{
    static unsigned char pnst;
    //	unsigned char dac,dz,nst;

    //----------
    switch(DtKNF.tpkt) //dla danego rodzaju kotla
    {
        case _BITERM: //bitermiczny (miniterm)
        case _BITERM2: //bitermiczny (maxiterm)
        {
            DtCW.minPCW = F1_MIN_PCW;
            DtCW.maxPCW = F1_MAX_PCW;
            break;
        }
        case _MONOHB:
        {
            DtCW.minPCW = F2_MIN_PCW;
            DtCW.maxPCW = F2_MAX_PCW;
            break;
        }
        case _UNICO: //kociol unico
        {
            DtCW.minPCW = F3_MIN_PCW;
            DtCW.maxPCW = F3_MAX_PCW;
            break;
        }
        default: //domyslny monotermiczny
        {
            DtCW.minPCW = F2_MIN_PCW;
            DtCW.maxPCW = F2_MAX_PCW;
            break;
        }
    }
    pnst = PFN.PCW;
    if(pnst < DtCW.minPCW) pnst = DtCW.minPCW; //nastawa w dozwolonym zakresie?
    if(pnst > DtCW.maxPCW) pnst = DtCW.maxPCW; //nastawa w dozwolonym zakresie?
    if(DtCW.PCW != pnst) //nowa wartosc nastawy?
    {
        DtCW.PCW = pnst;
        DtCWf_newPCW = 1;
        StartRTdS(_RTPCW);
    }
    WritePCW(); //zapisz nastawe do pamieci
    SetModPCW();
    DtCW.DAD_CW = ReturnACfromNTC(DtCW.PPCW - (_BNCW - 1));
    DtCW.MAD_CW = ReturnACfromNTC(DtCW.PPCW - (_BNCW - 1) - _MR_MADCW);
}



/*---------------------------------------------------------------------------------*/
//Zapis nastawy ECO po ustalonym czasie do EEPROM
/*
void WriteECO(void)
{
        if(DtINf_newECO&&RTdS(_RTECO)>=_TWRTECO)
        {
                DtINf_newECO=0;
                StartRTdS(_RTECO);
                WriteEEECO();
        }
}*/
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Nastawa ECO
/*void PomiarECO(void)
{
        static unsigned char peco;

        if(!GetBit2(RSDT1.inSTAT0,_INITDTA)&&MGSD._CON)
        {
                peco=10-RSDT1.inECO;
                if(peco<N_MIN_ECO) peco=N_MIN_ECO;	//nastawa w dozwolonym zakresie?
                if(peco>N_MAX_ECO) peco=N_MAX_ECO;	//nastawa w dozwolonym zakresie?	//nastawa w dozwolonym zakresie?
                if(DtIN.Eco!=peco)	//nowa wartosc nastawy?
                {
                        DtIN.Eco=peco;
                        DtINf_newECO=1;
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
        if(Mf_AKTYWNE_PGD) 	DtIN.PPEco=_PGDECO;
        else DtIN.PPEco=DtIN.Eco;
        RSDT1.outECO=10-DtIN.PPEco;
}*/
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Zapis pozycji PFN po ustalonym czasie do EEPROM

void WritePFN(void)
{
    if(PFNf_newPFN && RTdS(_RTPFN) >= _TWRTPFN)
    {
        PFNf_newPFN = 0;
        StartRTdS(_RTPFN);
        WriteEEPFN();
    }
}
/*---------------------------------------------------------------------------------*/
//Nastawa przelacznika funkcyjnego

void PomiarPFN(void)
{
    static unsigned char pnst;

    if(PFNf_off) //pozycja off
    {
        PFN.poz = _PFOFF;
    }
    else
        if(PFNf_zima && !PFNf_off) //pozycja zima
    {
        PFN.poz = _PFZMA;
    }
    else
        if(PFNf_lato && !PFNf_off) //pozycja lato
    {
        PFN.poz = _PFLTO;
    }
    else
    {
        PFN.poz = _PFZMA;
    }
    pnst = ReadEEPFN();
    if(PFN.poz != pnst) //nowa wartosc pozycji PFN?
    {
        if(!PFNf_newPFN) StartRTdS(_RTPFN);
        PFNf_newPFN = 1;
    }
    WritePFN(); //zapisz nastawe do pamieci
}
/*---------------------------------------------------------------------------------*/
//Pomiar wartosci cisnienia

void PomiarCS(void)
{
    static unsigned char pcs;
    unsigned char dac, cs;
    dac = ReadAC(_CHN_CS);

    if((!DtKNF.tpcs && (dac < _CS_MIN || dac > _CS_MAX)) || (DtKNF.tpcs && (dac < _CS5_MIN || dac > _CS5_MAX))) //awaria czujnika?
    {
        if(DtKNF.robg == _OTWARTY) //UniCo z ukladem otwartym?
        {
            DtCSf_error = 0;
            DtCSf_no = 0;
            DtCSf_lo = 0;
            DtCSf_hi = 0;
            DtCSf_work = 1;
            DtCSf_no_mont = 1; //czujnik cisnienia nie zam. w ukladzie
            DtCS.CS = 0;
        }
        else
        {
            DtCSf_error = 1;
            DtCSf_no = 0;
            DtCSf_lo = 0;
            DtCSf_hi = 0;
            DtCSf_work = 0;
            DtCSf_no_mont = 0; //czujnik cisnienia zam. w ukladzie
        }
    }
    else
    {
        DtCSf_error = 0;
        DtCSf_no_mont = 0; //czujnik cisnienia zam. w ukladzie
        if(!DtKNF.tpcs) //czujnki 18V?
        {
            if(dac < _CS_0) dac = _CS_0;
            if(dac > _CS_4) dac = _CS_4;
            cs = ((dac - _CS_0)*10) / (((_CS_4 - _CS_0)*10) / _NWCS);
            if(cs < _NWCS)
                if((cs - pcs > 0) && (cs - pcs) < 2) cs = pcs; //filtr wskazu cisnienia
            if(cs > _NWCS) cs = _NWCS;
        }
        else //czujnik 5V
        {
            if(dac < _CS5_0) dac = _CS5_0;
            if(dac > _CS5_4) dac = _CS5_4;
            cs = ((dac - _CS5_0)*10) / (((_CS5_4 - _CS5_0)*10) / _NWCS5);
            if(cs < _NWCS5)
                if((cs - pcs > 0) && (cs - pcs) < 2) cs = pcs; //filtr wskazu cisnienia
            if(cs > _NWCS5) cs = _NWCS5;
        }
        pcs = cs;
        DtCS.CS = cs;
        PFN.CS = DtCS.CS;
        if(DtKNF.robg == _OTWARTY) //UniCo z ukladem otwartym?
        {
            //wylaczona detekcja niskiego cisnienia
            DtCSf_no = 0;
            DtCSf_lo = 0;
            if(DtCS.CS <= _ON2_WRK_CSN) DtCSf_work = 1;
            else
                if(DtCS.CS >= _OFF2_WRK_CSN) DtCSf_work = 0;
        }
        else
        {
            //aktywna detekcja niskiego cisnienia
            if(DtCS.CS < _NO_CSN) DtCSf_no = 1; //brak wody w ukladzie?
            else DtCSf_no = 0;
            if(!DtCSf_no && DtCS.CS < _LO_CSN) DtCSf_lo = 1; //niskie cisnienie w ukladzie?
            else DtCSf_lo = 0;
            if((DtCS.CS >= _ON1_WRK_CSN) && (DtCS.CS <= _ON2_WRK_CSN)) DtCSf_work = 1;
            else
                if((DtCS.CS <= _OFF1_WRK_CSN) || (DtCS.CS >= _OFF2_WRK_CSN)) DtCSf_work = 0;
        }
        if(DtCS.CS >= _ON_HI_CSN) DtCSf_hi = 1; //zbyt wysokie cisnienie?
        else DtCSf_hi = 0;
    }
}
/*---------------------------------------------------------------------------------*/
