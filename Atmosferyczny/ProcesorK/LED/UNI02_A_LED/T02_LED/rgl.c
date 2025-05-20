/*rgl.c*/
/*---------------------------------------------------------------------------------*/
#include <pic18.h>
#include "global.h"				//parametry globalne
#include "rgl.h"				//parametry lokalne
#include "main.tp.h"
#include "pomiar.h"
#include "pwm.h"
#include "konfig2.h"
#include "pompa.tp.h"


volatile DtRgl rgl;

extern DtPMP PMP;
extern DataCO DtCO;
extern DataCW DtCW;
extern DtPWM PWM;
extern DtKONF DtKNF;
extern tpMAIN M;
//extern unsigned char x;

unsigned char tmr, tmr2;
unsigned char pwm_center, pwm_kwr;

//************************************************************
//Procedura modulacji

void Moduluj(void)
{
    //	unsigned int wpw;
    //	unsigned char wpp;

    switch(DtKNF.tpkt) //rodzaj kotla
    {
        case _BITERM: //kociol bitermiczny (miniterm)
        {
            if(Mf_pob_CO) //obieg CO
            {
                PWM.MAXPWM = PWM.MMAXCO; //maksymalna nastawa modulatora
                PWM.MINPWM = PWM.MDMOD;
                StartCO(); //parametry startowe dla CO
                ReadDataCO(); //wprowadzenie danych wejsciowych dla CO
                ModToCO(); //zmien obieg na CO
                ModZgCO(); //pierwsze przyblizenie
                ModDkCO(); //drugie przyblizenie
                if(RTdS(_RTMOD) > _TOKM)
                {
                    StartRTdS(_RTMOD);
                    IncConstCOCW(); //oblicza czas przez ktory temp CO = const
                    IncMaxCO(); //inkrementacja maksymalnej wartosci mocy na palniku
                    ModPruCO(); //trzecie przyblizenie
                }
                ModPrzCO(); //reakcja superwizyjna
                PropOPT();
            }
            else
                if(Mf_pob_CW) //obieg CW
            {
                StartMCW(); //parametry startowe dla CW
                ReadDataCW(); //wprowadzenie danych wejsciowych dla CW
                ModToCW(); //zmien obieg na CW
                ModZgCW(); //pierwsze przyblizenie
                ModDkCW(); //drugie przyblizenie
                if(RTdS(_RTMOD) > _TOKM)
                {
                    StartRTdS(_RTMOD);
                    if(tmr < 0xff) tmr++;
                    if(tmr2 < 0xff) tmr2++;
                    IncConstCOCW(); //oblicza czas przez ktory temp CW = const
                    IncMaxCW(); //inkrementacja maksymalnej wartosci mocy na palniku
                    ModPruCW(); //trzecie przyblizenie
                }
                ModPrzCW(); //reakcja superwizyjna
                PropCO(); //zabezpieczenie od przegrzania w obwodzie CO				StartMCW2();		//parametry startowe dla CW
            }
            break;
        }
        case _MONOHB: //kociol z hydroblokiem
        {
            if(Mf_pob_CO) //obieg CO
            {
                PWM.MAXPWM = PWM.MMAXCO; //maksymalna nastawa modulatora
                PWM.MINPWM = PWM.MDMOD;
                StartCO2(); //parametry startowe dla CO
                ReadDataCO2(); //wprowadzenie danych wejsciowych dla CO
                ModToCO2(); //zmien obieg na CO
                ModZgCO(); //pierwsze przyblizenie
                ModDkCO(); //drugie przyblizenie
                if(RTdS(_RTMOD) > _TOKM)
                {
                    StartRTdS(_RTMOD);
                    IncConstCO2(); //oblicza czas przez ktory temp CO = const
                    IncMaxCO(); //inkrementacja maksymalnej wartosci mocy na palniku
                    ModPruCO(); //trzecie przyblizenie
                }

                ModPrzCO(); //reakcja superwizyjna
                PropOPT();
            }
            else
                if(Mf_pob_CW) //obieg CW
            {
                StartMCW2(); //parametry startowe dla CW
                ReadDataCW2(); //wprowadzenie danych wejsciowych dla CW
                ModToCW2(); //zmien obieg na CW
                ModZgCW2(); //pierwsze przyblizenie
                ModDkCW2(); //drugie przyblizenie
                if(RTdS(_RTMOD) > _TOKM)
                {
                    StartRTdS(_RTMOD);
                    IncConstCW2(); //oblicza czas przez ktory temp CW = const
                    IncMaxCW2(); //inkrementacja maksymalnej wartosci mocy na palniku
                    ModPruCW2(); //trzecie przyblizenie
                }
                ModPrzCW2(); //reakcja superwizyjna
                PropCO2(); //zabezpieczenie od przegrzania w obwodzie CO
            }
            break;
        }
        case _UNICO:
        {
            if(Mf_pob_CO) //obieg CO
            {
                PWM.MAXPWM = PWM.MMAXCO; //maksymalna nastawa modulatora
                PWM.MINPWM = PWM.MDMOD;
                StartMCO3(); //parametry startowe dla CO
                ReadDataCO2(); //wprowadzenie danych wejsciowych dla CO
                ModToCO3(); //zmien obieg na CO
                ModZgCO(); //pierwsze przyblizenie
                ModDkCO(); //drugie przyblizenie
                if(RTdS(_RTMOD) > _TOKM)
                {
                    StartRTdS(_RTMOD);
                    IncConstCO2(); //oblicza czas przez ktory temp CO = const
                    IncMaxCO(); //inkrementacja maksymalnej wartosci mocy na palniku
                    ModPruCO(); //trzecie przyblizenie
                }

                ModPrzCO(); //reakcja superwizyjna
                PropOPT();
            }
            else
                if(Mf_pob_CW || Mf_pob_AL) //obieg CW
            {
                PWM.MAXPWM = PWM.MMAXCW; //maksymalna nastawa modulatora
                PWM.MINPWM = PWM.MDMOD;
                StartMCO3(); //parametry startowe dla CO
                ReadDataCO2(); //wprowadzenie danych wejsciowych dla CO
                ModToCW3(); //zmien obieg na CO
                ModZgCO(); //pierwsze przyblizenie
                ModDkCO(); //drugie przyblizenie
                if(RTdS(_RTMOD) > _TOKM)
                {
                    StartRTdS(_RTMOD);
                    IncConstCO2(); //oblicza czas przez ktory temp CO = const
                    IncMaxCO(); //inkrementacja maksymalnej wartosci mocy na palniku
                    ModPruCO(); //trzecie przyblizenie
                }

                ModPrzCO(); //reakcja superwizyjna
                PropOPT();
            }
            break;
        }
        case _BITERM2: //kociol bitermiczny (maxiterm)
        {
            if(Mf_pob_CO) //obieg CO
            {
                PWM.MAXPWM = PWM.MMAXCO; //maksymalna nastawa modulatora
                PWM.MINPWM = PWM.MDMOD;
                StartCO4(); //parametry startowe dla CO
                ReadDataCO4(); //wprowadzenie danych wejsciowych dla CO
                ModToCO4(); //zmien obieg na CO
                ModZgCO(); //pierwsze przyblizenie
                ModDkCO(); //drugie przyblizenie
                if(RTdS(_RTMOD) > _TOKM)
                {
                    StartRTdS(_RTMOD);
                    IncConstCOCW4(); //oblicza czas przez ktory temp CO = const
                    IncMaxCO(); //inkrementacja maksymalnej wartosci mocy na palniku
                    ModPruCO(); //trzecie przyblizenie
                }
                ModPrzCO(); //reakcja superwizyjna
                PropOPT();
            }
            else
                if(Mf_pob_CW) //obieg CW
            {
                StartMCW4(); //parametry startowe dla CW
                ReadDataCW4(); //wprowadzenie danych wejsciowych dla CW
                ModToCW4(); //zmien obieg na CW
                ModZgCW4(); //pierwsze przyblizenie
                ModDkCW4(); //drugie przyblizenie
                if(RTdS(_RTMOD) > _TOKM)
                {
                    StartRTdS(_RTMOD);
                    IncConstCOCW4(); //oblicza czas przez ktory temp CW = const
                    IncMaxCW4(); //inkrementacja maksymalnej wartosci mocy na palniku
                    ModPruCW4(); //trzecie przyblizenie
                }
                ModPrzCW4(); //reakcja superwizyjna
                PropCO4(); //zabezpieczenie od przegrzania w obwodzie CO				StartMCW2();		//parametry startowe dla CW
            }
            break;
        }
        default: //domyslny monotermiczny
        {
            if(Mf_pob_CO) //obieg CO
            {
                PWM.MAXPWM = PWM.MMAXCO; //maksymalna nastawa modulatora
                PWM.MINPWM = PWM.MDMOD;
                StartCO2(); //parametry startowe dla CO
                ReadDataCO2(); //wprowadzenie danych wejsciowych dla CO
                ModToCO2(); //zmien obieg na CO
                ModZgCO(); //pierwsze przyblizenie
                ModDkCO(); //drugie przyblizenie
                if(RTdS(_RTMOD) > _TOKM)
                {
                    StartRTdS(_RTMOD);
                    IncConstCO2(); //oblicza czas przez ktory temp CO = const
                    IncMaxCO(); //inkrementacja maksymalnej wartosci mocy na palniku
                    ModPruCO(); //trzecie przyblizenie
                }

                ModPrzCO(); //reakcja superwizyjna
                PropOPT();
            }
            else
                if(Mf_pob_CW) //obieg CW
            {
                StartMCW2(); //parametry startowe dla CW
                ReadDataCW2(); //wprowadzenie danych wejsciowych dla CW
                ModToCW2(); //zmien obieg na CW
                ModZgCW2(); //pierwsze przyblizenie
                ModDkCW2(); //drugie przyblizenie
                if(RTdS(_RTMOD) > _TOKM)
                {
                    StartRTdS(_RTMOD);
                    IncConstCW2(); //oblicza czas przez ktory temp CW = const
                    IncMaxCW2(); //inkrementacja maksymalnej wartosci mocy na palniku
                    ModPruCW2(); //trzecie przyblizenie
                }
                ModPrzCW2(); //reakcja superwizyjna
                PropCO2(); //zabezpieczenie od przegrzania w obwodzie CO
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

    if(rglf_btco && !rglf_btcw) return; //praca w obiegu CO?
    if(rglf_btco && rglf_btcw) //zmiana obiegu CO na CW?
    {
        rglf_btcw = 0;
        if(!Tnizsza(0))
        {
            k = 10;
            if(rgl.srp - k < PWM.MDMOD) rgl.srp = PWM.MDMOD;
            else rgl.srp -= k;
        }
    }
    else //rozpoczecie pracy w obiegu CO lub CW
    {
        StartRTdS(_RTMOD);
        DtCO.dDCO = 0;
        DtCO.indCO = 0;
        rglf_fz = 0;
        rglf_med_up = 0;
        rglf_fast_up = 0;
        //CO - dobor wartosci pierwszego pobudzenia (moment startu modulacji)
        if(PWM.MMAXCO > PWM.FIRSTPB)
        {
            rgl.xmod = PWM.FIRSTPB;
        }
        else
        {
            rgl.xmod = PWM.MMAXCO;
        }
        //		if(!PWMf_tpGZ) rgl.xmod=I80_MOD;
        //		else rgl.xmod=I80_PMOD;
        rgl.srp = 0;
        rgl.ytmp = 0;
        rglf_nmod = 0; //wyjdz z trybu nmod
        rglf_btcw = 0;
        rglf_btco = 1;
        rglf_del1s = 0;
        rglf_deln1 = 0;
        rglf_inv_mod = 0;
    }
}
//-------------------------------------------------------------
//pierwsze przyblizenie

void ModZgCO(void)
{
    signed long m;
    unsigned char p;

    //faza2
    if(rglf_fz) //aktualnie faza 2?
    {
        if(Tnizsza(10)) //przygotowanie do przejscia do fazy 1
        {
            rgl.xmod = PWM.BufPWM;
            rgl.srp = 0;
            rgl.ytmp = 0;
        }
        else return;
    }
    //faza1
    if((!Tnizsza(5) && Tmaleje(0)) || (!Tnizsza(2) && Tstoi())) //warunek przejscia do fazy 2
    {
        rglf_fz = 1;
        rgl.srp = PWM.BufPWM;
        rgl.xmod = PWM.BufPWM;
        if(rgl.xmod > PWM.MGMOD)
        {
            rgl.srp = PWM.MGMOD;
            rgl.xmod = PWM.MGMOD;
        }
        return;
    }
    if(rgl.xmod >= PWM.MDMOD) m = (rgl.xmod - PWM.MDMOD)*10;
    else m = 0;
    m = m / (DtCO.MAD_CO - DtCO.DAD_CO); //DAD_CO-MAD_CO
    m = m * (DtCO.AC_CO - DtCO.DAD_CO); //DAD_CO-AC_CO
    if(m < 0) m = 0;
    m = m / 10 + PWM.MDMOD;
    if(m > 0xff) //przepelnienie
    {
        rglf_fz = 0; //przejscie do fazy1 przy aktywnej fazie2
        rgl.tmp_pwm = rgl.xmod;
        return;
    }
    if(rglf_fz && (rgl.xmod > m))
        rgl.srp = (rgl.xmod - m); //przygotowanie lagodnego przejscia do fazy 1
    m += rgl.srp;
    if(m > rgl.xmod) p = rgl.xmod;
    else p = m;
    rglf_fz = 0; //przejscie do fazy1 przy aktywnej fazie2
    rgl.tmp_pwm = p;
}
//-------------------------------------------------------------
//drugie przyblizenie (tylko dla fazy 1)

void ModDkCO(void)
{
    signed long m;
    signed char ada;
    unsigned char k, p;

    if(!rglf_fz) return; //wyjscie jesli aktualnie faza 1
    ada = DtCO.AC_CO - DtCO.DAD_CO;
    k = 5;
    if(Twyzsza(0)) k = 6; //6		//(Tco>Tnst)?
    m = rgl.srp + k*ada;
    if(m > rgl.xmod) p = rgl.xmod;
    else
        if(m < PWM.MDMOD) p = PWM.MDMOD;
    else p = m;
    rgl.tmp_pwm = p;
}
//-------------------------------------------------------------						
//trzecie przyblizenie
//nowa wartosc punktu rownowagi srp.
//wykonywane co krok czasowy

void ModPruCO(void)
{
    static unsigned char l;
    unsigned char tmp, k;
    //	signed char ada;
    if(!rglf_fz) //aktualnie faza 1?
    {
        if(Tmaleje(0) || rgl.ytmp > 2) //(Tco maleje)lub(Tco=CONST(>=3sek))?
        {
            rgl.ytmp = 0;
            if(PWM.BufPWM > rgl.xmod) return;
            tmp = rgl.xmod - PWM.BufPWM;
            k = 6;
            if(k > tmp) k = tmp;
            if(rgl.srp + k > PWM.MGMOD) rgl.srp = PWM.MGMOD;
            else rgl.srp += k;
        }
        return;
    }
    else //aktualnie faza 2
    {
        if(!Trowna()) //(Tco<>Tnst) ?
        {
            if(DtCO.DAD_CO > DtCO.AC_CO) //Tco>Tnst ?
            {
                if(Tmaleje(0)) return; //(Tco maleje)?
                if(l >= 2)
                {
                    k = 1;
                    l = 0;
                }
                else
                {
                    k = 0;
                    l++;
                }
                if(rgl.srp - k < PWM.MDMOD) rgl.srp = PWM.MDMOD;
                else rgl.srp -= k;
                if(rgl.srp > PWM.MAXPWM) rgl.srp = PWM.MAXPWM;
                if(rgl.xmod < rgl.srp) rgl.xmod = rgl.srp;
                return;
            }
            else //Tco<Tnst
            {
                if(Trosnie(0)) return; //(Tco rosnie)?
                if(l >= 1)
                {
                    k = 1;
                    l = 0;
                }
                else
                {
                    k = 0;
                    l++;
                }
                if(PWM.MAXPWM > PWM.MGMOD)
                {
                    if(rgl.srp + k > PWM.MGMOD) rgl.srp = PWM.MGMOD;
                    else rgl.srp += k;
                }
                else
                {
                    if(rgl.srp + k > PWM.MAXPWM) rgl.srp = PWM.MAXPWM;
                    else rgl.srp += k;
                }
                if(rgl.xmod < rgl.srp) rgl.xmod = rgl.srp;
            }
        }
        else l = 0;
    }
}
//-------------------------------------------------------------								
//inkrementacja maksymalnej wartosci mocy na palniku
//wykonywane co krok czasowy

void IncMaxCO(void)
{
    unsigned char k;

    if(Trosnie(1)) return; //(Tco rosnie szybciej niz 1K/sek)?
    //kontrolowany narost mocy
    k = 6;
    if(!rglf_fz) //faza 1 ?
    {
        /*if(rgl.xmod + k > _MG2_MOD) rgl.xmod = _MG2_MOD;
        else rgl.xmod += k;
        if(rgl.xmod > PWM.MAXPWM) rgl.xmod = PWM.MAXPWM;*/
        if(rgl.xmod+k>PWM.MAXPWM)
		{
	 		rgl.xmod=PWM.MAXPWM;
			PmpToUP();			//reg. obrotami pompy
		}
		else
		{
	 		rgl.xmod+=k;
			PmpToDW();			//reg. obrotami pompy
		}
        if(rgl.xmod>_MG2_MOD) rgl.xmod=_MG2_MOD;
    }
    else //faza 2
    {
        if(PWM.MAXPWM > PWM.MGMOD)
        {
            if(rgl.xmod + k > PWM.MGMOD) rgl.xmod = PWM.MGMOD;
            else rgl.xmod += k;
        }
        else
        {
            if(rgl.xmod + k > PWM.MAXPWM) rgl.xmod = PWM.MAXPWM;
            else rgl.xmod += k;
        }
    }
}
//-------------------------------------------------------------								
//reakcja superwizyjna

void ModPrzCO(void)
{
    unsigned char k;

    if(!rglf_nmod) //nie jest w trybie nmod?
    {
        if(Twyzsza(3)) //Tcw-Tnst>3 ?
        {
            rglf_nmod = 1; //wejdz w tryb nmod
            k = 20;
            if(rgl.srp - k < PWM.MDMOD) rgl.srp = PWM.MDMOD;
            else rgl.srp -= k;
        }
    }
    else //jest w trybie nmod?
    {
        if(!Twyzsza(2))
            rglf_nmod = 0; //wyjdz z trybu nmod
    }
}
//-------------------------------------------------------------							
//Proporcjonalne OPEN-T

void PropOPT(void)
{
    if(rglf_nmod) //aktywny tryb nmod?
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
    //	unsigned char k;

    if(rglf_btco || rglf_btcw) return; //praca w dowolnym obiegu CO?
    StartRTdS(_RTMOD);
    DtCO.dDCO = 0;
    DtCO.indCO = 0;
    DtCW.dDCW = 0;
    DtCW.indCW = 0;
    rglf_fz = 0;
    rglf_med_up = 0;
    rglf_fast_up = 0;
    //CO - dobor wartosci pierwszego pobudzenia (moment startu modulacji)
    if(PWM.MMAXCO > PWM.FIRSTPB)
    {
        rgl.xmod = PWM.FIRSTPB;
    }
    else
    {
        rgl.xmod = PWM.MMAXCO;
    }
    rgl.srp = 0;
    rgl.ytmp = 0;
    rglf_nmod = 0; //wyjdz z trybu nmod
    rglf_del1s = 0;
    rglf_inv_mod = 0;
}
//-------------------------------------------------------------
//parametry startowe dla CW

void StartMCW(void)
{
    if(rglf_btco || rglf_btcw) return; //rozpoczecie pracy modulatora?
    StartRTdS(_RTMOD);
    DtCO.dDCO = 0;
    DtCW.dDCW = 0;
    DtCO.indCO = 0;
    DtCW.indCW = 0;
    rglf_fz = 0;
    rglf_med_up = 0;
    rglf_fast_up = 0;
    //FirstPCW();
    rgl.srp = 0;
    rgl.ytmp = 0;
    rglf_nmod = 0;
    rglf_del1s = 0;
    rglf_inv_mod = 0;
    rgl.xmod = FirstPCW();
    tmr = 1;
    tmr2 = 1;
    pwm_center = PWM.MDMOD + (PWM.MGMOD - PWM.MDMOD) / 2;
    pwm_kwr = PWM.MDMOD + (PWM.MGMOD - PWM.MDMOD) / 4;
}
//-------------------------------------------------------------
//pierwsze pobudzenie dla CW
//-------------------------------------------------------------

/*void FirstPCW(void)
{
        unsigned char tmp;
        if(DtCW.PCW>=45)
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
        if(DtCW.PCW>=40)
        {
                tmp=(PWM.MGMOD-PWM.FIRSTPB)/2;
                if(PWM.MMAXCW>(PWM.FIRSTPB+tmp))
                {
                        rgl.xmod=PWM.FIRSTPB+tmp;
                }
                else
                {
                        rgl.xmod=PWM.MMAXCW;
                }
        }
        else
        if(DtCW.PCW>=35)
        {
                if(PWM.MMAXCW>PWM.FIRSTPB)
                {
                        rgl.xmod=PWM.FIRSTPB;
                }
                else
                {
                        rgl.xmod=PWM.MMAXCW;
                }
        }
        else
        {
                tmp=(PWM.FIRSTPB-PWM.MDMOD)/2;
                if(PWM.MMAXCW>(PWM.FIRSTPB-tmp))
                {
                        rgl.xmod=PWM.FIRSTPB-tmp;
                }
                else
                {
                        rgl.xmod=PWM.MMAXCW;
                }
        }
}*/
unsigned char FirstPCW(void)
{
    signed long m;
    unsigned char maxmod, p, p2, tmp;

    //-------
    //maksymalna dozwolona wartosc pierwszego pobudzenia
    //if(PWM.MMAXCW<PWM.FIRSTPB) maxmod=PWM.MMAXCW;					//wprowadzono ograniczenie mocy?
    //else maxmod=PWM.FIRSTPB;
    tmp = (PWM.FIRSTPB - PWM.MDMOD) / 4;
    if(PWM.MMAXCW > (PWM.FIRSTPB + tmp))
    {
        maxmod = PWM.FIRSTPB + tmp;
    }
    else
    {
        maxmod = PWM.MMAXCW;
    }


    //-------
    //ze wzgledu na temperature
    if(DtCW.CW >= TMIN_MOD && DtCW.CW <= TMAX_MOD) //temperatura w przedziale roboczym?
    {
        m = (TMAX_MOD - DtCW.CW)*10;
        m = m / (TMAX_MOD - TMIN_MOD);
        m = m * (maxmod - PWM.MDMOD);
        if(m < 0) m = 0;
        m = m / 10 + PWM.MDMOD;
        if(m > 0xff) p = maxmod; //przepelnienie
        else p = m;
    }
    else //temperatura poza przedzialem?
    {
        if(DtCW.CW < TMIN_MOD) p = maxmod;
        if(DtCW.CW > TMAX_MOD) p = PWM.MDMOD;
    }
    //-------
    //ze wzgledu na nastawe
    m = (DtCW.PCW - F1_MIN_PCW)*10;
    m = m / (F1_MAX_PCW - F1_MIN_PCW);
    m = m * (maxmod - PWM.MDMOD);
    if(m < 0) m = 0;
    m = m / 10 + PWM.MDMOD;
    if(m > 0xff) p2 = maxmod; //przepelnienie
    else p2 = m;
    //-------
    //wybrana zostaje nizsza wartosc z p i p2
    if(p > p2) p = p2;
    //-------
    //ze wzgledu na relacje miêdzy temperatura i nastawa
    if(DtCW.DCW >= DMIN_MOD && DtCW.DCW <= DMAX_MOD) //roznica w przedziale roboczym?
    {
        m = DtCW.DCW * 10;
        m = m / (DMAX_MOD - DMIN_MOD);
        m = m * (maxmod - PWM.MDMOD);
        if(m < 0) m = 0;
        m = m / 10 + PWM.MDMOD;
        if(m > 0xff) p2 = maxmod; //przepelnienie
        else p2 = m;
    }
    else //roznica poza przedzialem?
    {
        if(DtCW.DCW > DMAX_MOD) p2 = maxmod;
        if(DtCW.DCW < DMIN_MOD) p2 = PWM.MDMOD;
    }
    //-------
    //wybrana zostaje nizsza wartosc z p i p2
    if(p > p2) p = p2;
    //-------
    return p; //wartosc pierwszego pobudzenia
}
//-------------------------------------------------------------
//zeruj parametry modulatora po zmianie obiegu

void ClrMod(void)
{
    rglf_fz = 0;
    rglf_med_up = 0;
    rglf_fast_up = 0;
    rgl.xmod = PWM.BufPWM;
    rgl.srp = 0;
    rgl.ytmp = 0;
    rglf_nmod = 0;
    rglf_del1s = 0;
    rglf_inv_mod = 0;
}
//-------------------------------------------------------------
//zmien obieg na CW

void ModToCW(void)
{
    rglf_btcw = 1;
    if(rglf_btco)
    {
        rglf_btco = 0;
        ClrMod();
    }
}
//-------------------------------------------------------------
//zmien obieg na CO

void ModToCO(void)
{
    rglf_btco = 1;
    if(rglf_btcw)
    {
        rglf_btcw = 0;
        ClrMod();
    }
}
//-------------------------------------------------------------
//oblicza czas przez ktory temp=const

void IncConstCOCW(void)
{
    if(Tstoi()) rgl.ytmp++;
    else rgl.ytmp = 0;
}
//-------------------------------------------------------------
//wprowadzenie danych wejsciowych-CO

void ReadDataCO(void)
{
    rgl.x_t = DtCO.DCO;
    rgl.x_dt = DtCO.dDCO;
}
//-------------------------------------------------------------
//wprowadzenie danych wejsciowych-CW

void ReadDataCW(void)
{
    rgl.x_t = DtCW.DCW;
    rgl.x_f = DtCO.DCO;
    rgl.x_dt = DtCW.dDCW;
    rgl.x_df = DtCO.dDCO;
}
//-------------------------------------------------------------
//pierwsze przyblizenie

void ModZgCW(void)
{
    signed long m;
    unsigned char p;
    if(rglf_fz) //aktualnie faza 2?
    {
        if(Tnizsza(3)) //2 przygotowanie do przejscia do fazy 1
        {
            rgl.xmod = PWM.BufPWM;
            rgl.srp = 0;
            rgl.ytmp = 0;
            rglf_nmod = 0;
            rglf_del1s = 0;
        }
        else return;
    }
    if(Twyzsza(0) && Tstoi()) //warunek przejscia do fazy 2
    {
        rglf_fz = 1;
        rglf_fast_up = 0;
        rglf_med_up = 0;
        rgl.srp = PWM.BufPWM;
        rgl.xmod = PWM.BufPWM;
        if(rgl.xmod > PWM.MGMOD)
        {
            rgl.srp = PWM.MGMOD;
            rgl.xmod = PWM.MGMOD;
        }
        return;
    }
    if(rgl.xmod >= PWM.MDMOD) m = (rgl.xmod - PWM.MDMOD)*10;
    else m = 0;
    m = m / (DtCW.MAD_CW - DtCW.DAD_CW); //DAD_CW-MAD_CW
    m = m * (DtCW.AC_CW - DtCW.DAD_CW); //DAD_CW-AC_CW
    if(m < 0) m = 0;
    m = m / 10 + PWM.MDMOD;
    if(m > 0xff) //przepelnienie
    {
        rglf_fz = 0; //przejscie do fazy1 przy aktywnej fazie2
        rgl.tmp_pwm = rgl.xmod;
        return;
    }
    if(rglf_fz && (rgl.xmod > m))
        rgl.srp = (rgl.xmod - m); //przygotowanie lagodnego przejscia do fazy 1
    m += rgl.srp;
    if(m > rgl.xmod) p = rgl.xmod;
    else p = m;
    rglf_fz = 0; //przejscie do fazy1 przy aktywnej fazie2
    rgl.tmp_pwm = p;
}
//-------------------------------------------------------------
//drugie przyblizenie (tylko dla fazy 1)

void ModDkCW(void)
{
    signed long m;
    signed char ada;
    unsigned char k, p;

    if(!rglf_fz) return; //wyjscie jesli aktualnie faza 1
    ada = DtCW.AC_CW - DtCW.DAD_CW;
    k = 1;
    if(Trowna()) return; //Tcw!=Tnst?
    if(Tnizsza(0)) //Tcw<Tnst?
    {
        k = 2;
        /*if(!Trosnie(0))					//(Tcw=CONST) lub (Tcw maleje) ?
        {
                k=1;
                if(Tnizsza(1)) k=3;			//3Tnst-Tcw>=2?
        }*/
    }
    /*else								//Tcw>Tnst?
    {
            if(!Tmaleje(0)) 				//(Tcw=CONST) lub (Tcw rosnie) ?
            {
                    k=1;
                    if(Twyzsza(1)) k=2;			//2Tcw-Tnst>=2?
            }
    }*/
    m = rgl.srp + k*ada;
    if(m > rgl.xmod) p = rgl.xmod;
    else
        if(m < PWM.MDMOD) p = PWM.MDMOD;
    else p = m;
    rgl.tmp_pwm = p;
}
//-------------------------------------------------------------
//trzecie przyblizenie
//nowa wartosc punktu rownowagi srp.
//wykonywane co krok czasowy

void ModPruCW(void)
{
    static unsigned char l, m, o, p, q, r, s;
    unsigned char tmp;
    signed char k;

    if(!rglf_fz) //aktualnie faza 1?
    {
        l = 0;
        m = 0;
        o = 0;
        p = 0;
        if(q < 255) q++; //licznik sekund
        if(s < 255) s++; //licznik sekund
        if(DtCWf_newPCW) s = 0; //zmieniono nastawe?
        if(Tnizsza(0)) //Tcw<Tnst?
        {
            if(!Tnizsza(10)) //Tnst-Tcw<=10 ?
            {
                k = 0;
                if(rgl.tmp_pwm >= pwm_center)
                {
                    if(Tnizsza(5) && Tstoi() && s >= 1)
                    {
                        s = 0;
                        k = 20; //k=2
                    }
                    else
                        if(Tnizsza(4) && Tstoi() && s >= 1)
                    {
                        s = 0;
                        k = 15; //k=2
                    }
                    else
                        if(Tnizsza(2) && Tstoi() && s >= 1)
                    {
                        s = 0;
                        k = 10; //k=2
                    }
                    else
                        if(Tstoi() && s >= 1)
                    {
                        s = 0;
                        k = 5; //k=2
                    }
                }
                else
                {
                    if(rgl.tmp_pwm >= pwm_kwr)
                    {
                        if(Tnizsza(5) && Tstoi() && s >= 1)
                        {
                            s = 0;
                            k = 10; //k=2
                        }
                        else
                            if(Tnizsza(4) && Tstoi() && s >= 1)
                        {
                            s = 0;
                            k = 8; //k=2
                        }
                        else
                            if(Tnizsza(2) && Tstoi() && s >= 1)
                        {
                            s = 0;
                            k = 5; //k=2
                        }
                        else
                            if(Tstoi() && s >= 1)
                        {
                            s = 0;
                            k = 2; //k=2
                        }
                    }
                    else
                    {
                        if(Tnizsza(2) && Tstoi() && s >= 1)
                        {
                            s = 0;
                            k = 5; //k=2
                        }
                        else
                            if(Tstoi() && s >= 1)
                        {
                            s = 0;
                            k = 2; //k=2
                        }
                    }
                }
                if(rgl.tmp_pwm >= pwm_kwr)
                {
                    if(Trosnie(1) && s >= 1) //szybki narost temperatury
                    {
                        s = 0;
                        k = 10; //10
                        if(Trosnie(2)) k = 8; //8b.szybki narost temperatury?
                    }
                    else
                        rgl.ytmp = 0;
                }
                if(PWM.BufPWM > rgl.xmod) return;
                tmp = rgl.xmod - PWM.BufPWM;
                if(k > tmp) k = tmp;
                if(PWM.MMAXCW > PWM.MGMOD)
                {
                    if(rgl.srp + k > PWM.MGMOD) rgl.srp = PWM.MGMOD;
                    else rgl.srp += k;
                }
                else
                {
                    if(rgl.srp + k > PWM.MMAXCW) rgl.srp = PWM.MMAXCW;
                    else rgl.srp += k;
                }
                return;
            }
            else return;
        }
        else return;
    }
    else //aktualnie faza 2
    {
        q = 0;
        r = 0;
        s = 0;
        if(m < 255) m++; //licznik sekund dla spadku temp CO
        if(p < 255) p++; //licznik sekund dla spadku temp CO
        if(o < 255) o++; //licznik sekund dla narostu temp CO
        if(DtCW.DAD_CW > DtCW.AC_CW) //Tcw>Tnst ?
        {
            k = 0;
            if(Trosnie(0) && m >= 2)
            {
                p = 0;
                o = 0;
                l = 0;
                m = 0;
                k = 2; //3
            }
            else
                if(Twyzsza(1)) //gdy jest powyzej zakresu nastawa+1
            {
                if(Tstoi())
                {
                    if(l >= 2)
                    {
                        k = 1;
                        if(Twyzsza(3)) k = 2;
                        l = 0;
                    }
                    else
                    {
                        k = 0;
                        l++;
                    }
                }
            }
            else l = 0;
            if(PWM.MMAXCW > PWM.MGMOD)
            {
                if(rgl.srp > PWM.MGMOD) rgl.srp = PWM.MGMOD;
            }
            else
            {
                if(rgl.srp > PWM.MMAXCW) rgl.srp = PWM.MMAXCW;
            }
            if(rgl.srp - k < PWM.MDMOD) rgl.srp = PWM.MDMOD;
            else rgl.srp -= k;

            //if(rgl.srp>PWM.MMAXCW) rgl.srp=PWM.MMAXCW;
            if(rgl.xmod < rgl.srp) rgl.xmod = rgl.srp;
            return;
        }
        else //Tcw<Tnst
        {
            k = 0;
            if(Tmaleje(0) && m >= 2)
            {
                p = 0;
                o = 0;
                l = 0;
                m = 0;
                k = 5; //1
                //if(Tnizsza(0)) k=3;				//3
            }
            else
                if(Tnizsza(1))
            {
                if(Tstoi())
                {
                    if(l >= 3)
                    {
                        k = 2;
                        l = 0;
                    }
                    else
                    {
                        k = 0;
                        l++;
                    }
                }
            }
            else l = 0;
            if(PWM.MMAXCW > PWM.MGMOD)
            {
                if(rgl.srp + k > PWM.MGMOD) rgl.srp = PWM.MGMOD;
                else rgl.srp += k;
            }
            else
            {
                if(rgl.srp + k > PWM.MMAXCW) rgl.srp = PWM.MMAXCW;
                else rgl.srp += k;
            }
            if(rgl.xmod < rgl.srp) rgl.xmod = rgl.srp;
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
    if(rglf_fz) //faza 2 ?
    {
        if(PWM.MMAXCW > PWM.MGMOD)
        {
            if(rgl.xmod > PWM.MGMOD) rgl.xmod = PWM.MGMOD;
        }
        else
        {
            if(rgl.xmod > PWM.MMAXCW) rgl.xmod = PWM.MMAXCW;
        }
        return;
    }
    //jednorazowa modyfikacja wartosci srp ze wzgledu na szybkosc
    //narostu temperatury
    if(Trosnie(2) && !rglf_med_up) //(Tcw rosnie szybciej niz 3K/sek)?
    {
        rglf_med_up = 1;
        k = 5; //10
        if(PWM.MMAXCW > PWM.MGMOD)
        {
            if(rgl.srp + k > PWM.MGMOD) rgl.srp = PWM.MGMOD;
            else rgl.srp += k;
        }
        else
        {
            if(rgl.srp + k > PWM.MMAXCW) rgl.srp = PWM.MMAXCW;
            else rgl.srp += k;
        }
    }
    if(Trosnie(3) && !rglf_fast_up) //(Tcw rosnie szybciej niz 4K/sek)?
    {
        rglf_fast_up = 1;
        k = 8; //12
        if(PWM.MMAXCW > PWM.MGMOD)
        {
            if(rgl.srp + k > PWM.MGMOD) rgl.srp = PWM.MGMOD;
            else rgl.srp += k;
        }
        else
        {
            if(rgl.srp + k > PWM.MMAXCW) rgl.srp = PWM.MMAXCW;
            else rgl.srp += k;
        }
    }

    //kontrolowany narost mocy
    if(rgl.tmp_pwm >= pwm_kwr)
    {
        if(Trosnie(2)) return;
    }
    else
    {
        if(Trosnie(1)) return;
    }
    k = 5;
    if(rgl.xmod + k > _MG2_MOD) rgl.xmod = _MG2_MOD;
    else rgl.xmod += k;
    if(rgl.xmod > PWM.MMAXCW) rgl.xmod = PWM.MMAXCW;
}
//------------------------------------------------------------
//reakcja superwizyjna

/*void ModPrzCW(void)
{
        if(!rglf_nmod)					//nie jest w trybie nmod?
        {
                if(rglf_fz)					//faza 2 ?
                {
                        if(!Tmaleje(0))			//(Tcw nie maleje ?)
                        {
                                if(Twyzsza(4))		//Tcw-Tnst>4 ?
                                {
                                        if(rgl.srp-15<PWM.MDMOD) rgl.srp=PWM.MDMOD;
                                        else rgl.srp-=15;
                                        rglf_nmod=1;		//wejdz w tryb nmod
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
                                        rglf_nmod=0;		//wyjdz z trybu nmod
        }
}*/
void ModPrzCW(void)
{
    unsigned char p, maxmod;
    signed int r;
    static unsigned char timer1;

    //stabilizacja predkosci dochodzenia do temp. zadanej.
    if(Tnizsza(0))//(!Tnizsza(20)&&Tnizsza(0)) //!Tnizsza(15)&&Tnizsza(0)
    {
        if(PWM.MMAXCW < PWM.MGMOD) maxmod = PWM.MMAXCW; //wprowadzono ograniczenie mocy?
        else maxmod = PWM.MGMOD;


        if(rgl.tmp_pwm >= pwm_center)
        {
            if(rgl.xmod <= maxmod) p = rgl.xmod;
            else
            {
                p = maxmod;
                rgl.xmod = maxmod;
            }
            if((DtCW.ddDCW > 6) && (tmr > 1)) //temperatura rosnie szybciej ni¿ 3 stop. na sek.?
            {
                tmr = 0;
                r = p - PWM.MDMOD;
                if((p - (r / 3)) > PWM.MDMOD) p -= r / 3; //4
                else p = PWM.MDMOD;
                rgl.xmod = p;
            }
            else
                if((DtCW.ddDCW > 4) && (tmr > 1)) //temperatura rosnie szybciej ni¿ 3 stop. na sek.?
            {
                tmr = 0;
                r = p - PWM.MDMOD;
                if((p - (r / 4)) > PWM.MDMOD) p -= r / 4; //4
                else p = PWM.MDMOD;
                rgl.xmod = p;
            }
            else
                if((DtCW.ddDCW > 3) && (tmr > 0)) //tmr2									//temperatura rosnie szybciej ni¿ 3 stop. na sek.?
            {
                tmr = 0;
                r = p - PWM.MDMOD;
                if((p - (r / 14)) > PWM.MDMOD) p -= r / 14; //16
                else p = PWM.MDMOD;
                rgl.xmod = p;
            }
        }
        else
        {
            if(rgl.xmod <= maxmod) p = rgl.xmod;
            else
            {
                p = maxmod;
                rgl.xmod = maxmod;
            }
            if((DtCW.ddDCW > 6) && (tmr > 1)) //temperatura rosnie szybciej ni¿ 3 stop. na sek.?
            {
                tmr = 0;
                r = p - PWM.MDMOD;
                if((p - (r / 2)) > PWM.MDMOD) p -= r / 2; //2
                else p = PWM.MDMOD;
                rgl.xmod = p;
            }
            else
                if((DtCW.ddDCW > 4) && (tmr > 1)) //temperatura rosnie szybciej ni¿ 3 stop. na sek.?
            {
                tmr = 0;
                r = p - PWM.MDMOD;
                if((p - (r / 3)) > PWM.MDMOD) p -= r / 3; //3
                else p = PWM.MDMOD;
                rgl.xmod = p;
            }
            else
                if((DtCW.ddDCW > 3) && (tmr > 0)) //tmr2									//temperatura rosnie szybciej ni¿ 3 stop. na sek.?
            {
                tmr = 0;
                r = p - PWM.MDMOD;
                if((p - (r / 4)) > PWM.MDMOD) p -= r / 4; //8
                else p = PWM.MDMOD;
                rgl.xmod = p;
            }
        }
        /*else
        if((DtCW.ddDCW<=0)&&(tmr2>1))
        {
                tmr2=0;
                r=p-PWM.MDMOD;
                if((p+(r/16))<maxmod) p+=r/16;
                else p=maxmod;
                rgl.xmod=p;
        }*/
    }
    if(!rglf_nmod) //nie jest w trybie nmod?
    {
        if(rglf_fz) //faza 2 ?
        {
            if(!Tmaleje(0)) //(Tcw nie maleje ?)
            {
                if(Twyzsza(4)) //Tcw-Tnst>4 ?
                {
                    if(rgl.tmp_pwm >= pwm_kwr)
                    {
                        if(rgl.srp - 5 < PWM.MDMOD) rgl.srp = PWM.MDMOD; //10
                        else rgl.srp -= 5;
                    }
                    rglf_nmod = 1; //wejdz w tryb nmod
                    return;
                }
                else return;
            }
            else return;
        }
        else return;
    }
    else //jest w trybie nmod?
    {
        if(Tmaleje(0)) //(Tcw maleje ?)
            rglf_nmod = 0; //wyjdz z trybu nmod
    }
    timer1++;
}



//------------------------------------------------------------
//Zabezpieczenie od przegrzania w obwodzie CO

void PropCO(void)
{
    if(!rglf_nmod) //nie jest w trybie nmod?
    {
        if(!DtCOf_err_CO) //awaria czujnika temperatury CO?
        {
            if(rgl.x_f == 3) //=86C ?
            {
                if(rgl.tmp_pwm > PWM.MGMOD)
                    rgl.tmp_pwm = PWM.MGMOD; //=MGMOD
            }
            else
                if(rgl.x_f == 2) //=87C ?
            {
                if(rgl.tmp_pwm > PWM.PROP1)
                    rgl.tmp_pwm = PWM.PROP1; //-1/7
            }
            else
                if(rgl.x_f == 1) //=88C ?
            {
                if(rgl.tmp_pwm > PWM.PROP2)
                    rgl.tmp_pwm = PWM.PROP2; //-2/7
            }
            else
                if(rgl.x_f <= 0) //>=89C ?
            {
                if(rgl.tmp_pwm > PWM.PROP3)
                    rgl.tmp_pwm = PWM.PROP3; //-3/7
            }
        }
        WriteOPWM(rgl.tmp_pwm);
    }
    else WriteOPWM(PWM.MDMOD); //PWM.BufPWM=PWM.MDMOD;			//wymus minimum plomienia
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
    //unsigned char k;

    if(rglf_btco || rglf_btcw) return; //praca w dowolnym obiegu CO?
    StartRTdS(_RTMOD);
    DtCO.dDCO = 0;
    DtCO.indCO = 0;
    DtCW.dDCW = 0;
    DtCW.indCW = 0;
    rglf_fz = 0;
    rglf_med_up = 0;
    rglf_fast_up = 0;
    //CO - dobor wartosci pierwszego pobudzenia (moment startu modulacji)
    if(PWM.MMAXCO > PWM.FIRSTPB)
    {
        rgl.xmod = PWM.FIRSTPB;
    }
    else
    {
        rgl.xmod = PWM.MMAXCO;
    }
    rgl.srp = 0;
    rgl.ytmp = 0;
    rglf_nmod = 0; //wyjdz z trybu nmod
    rglf_del1s = 0;
    rglf_inv_mod = 0;
}
//-------------------------------------------------------------
//parametry startowe dla CW

void StartMCW4(void)
{
    if(rglf_btco || rglf_btcw) return; //rozpoczecie pracy modulatora?
    StartRTdS(_RTMOD);
    DtCO.dDCO = 0;
    DtCW.dDCW = 0;
    DtCO.indCO = 0;
    DtCW.indCW = 0;
    rglf_fz = 0;
    rglf_med_up = 0;
    rglf_fast_up = 0;
    //FirstPCW4();
    rgl.srp = 0;
    rgl.ytmp = 0;
    rglf_nmod = 0;
    rglf_del1s = 0;
    rglf_inv_mod = 0;
    rgl.xmod = PWM.MDMOD;
}
//-------------------------------------------------------------
//pierwsze pobudzenie dla CW

void FirstPCW4(void)
{
    if(PWM.MMAXCW > PWM.MGMOD)
    {
        if(DtCW.DCW > 0) rgl.xmod = PWM.MGMOD;
        else rgl.xmod = PWM.FIRSTPB;
    }
    else
    {
        if(DtCW.DCW > 0) rgl.xmod = PWM.MMAXCW;
        else
        {
            if(PWM.MMAXCW > PWM.FIRSTPB) rgl.xmod = PWM.FIRSTPB;
            else rgl.xmod = PWM.MMAXCW;
        }
    }
}
//-------------------------------------------------------------
//zeruj parametry modulatora po zmianie obiegu

void ClrMod4(void)
{
    rglf_fz = 0;
    rglf_med_up = 0;
    rglf_fast_up = 0;
    rgl.xmod = PWM.BufPWM;
    rgl.srp = 0;
    rgl.ytmp = 0;
    rglf_nmod = 0;
    rglf_del1s = 0;
    rglf_inv_mod = 0;
}
//-------------------------------------------------------------
//zmien obieg na CW

void ModToCW4(void)
{
    rglf_btcw = 1;
    if(rglf_btco)
    {
        rglf_btco = 0;
        ClrMod4();
    }
}
//-------------------------------------------------------------
//zmien obieg na CO

void ModToCO4(void)
{
    rglf_btco = 1;
    if(rglf_btcw)
    {
        rglf_btcw = 0;
        ClrMod4();
    }
}
//-------------------------------------------------------------
//oblicza czas przez ktory temp=const

void IncConstCOCW4(void)
{
    if(Tstoi()) rgl.ytmp++;
    else rgl.ytmp = 0;
}
//-------------------------------------------------------------
//wprowadzenie danych wejsciowych-CO

void ReadDataCO4(void)
{
    rgl.x_t = DtCO.DCO;
    rgl.x_dt = DtCO.dDCO;
}
//-------------------------------------------------------------
//wprowadzenie danych wejsciowych-CW

void ReadDataCW4(void)
{
    rgl.x_t = DtCW.DCW;
    rgl.x_f = DtCO.DCO;
    rgl.x_dt = DtCW.dDCW;
    rgl.x_df = DtCO.dDCO;
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
        pop = 0; //20wartosc poprawki na przesterowanie
        //FirstPCW4();
    }
    if(rglf_fz) //aktualnie faza 2?
    {
        if(Tnizsza(2)) //Tnizsza(2)przygotowanie do przejscia do fazy 1
        {
            rgl.xmod = PWM.BufPWM;
            rgl.srp = 0;
            rgl.ytmp = 0;
            rglf_del1s = 0;
            pop = 0;
        }
        else return;
    }
    if(!Tnizsza(1)) //warunek przejscia do fazy 2
    {
        rglf_fz = 1;
        rglf_fast_up = 0;
        rglf_med_up = 0;
        rgl.srp = PWM.BufPWM + pop;
        rgl.xmod = PWM.BufPWM + pop;
        pop = 0;
        if(PWM.MMAXCW > PWM.MGMOD)
        {
            if(rgl.xmod > PWM.MGMOD)
            {
                rgl.srp = PWM.MGMOD;
                rgl.xmod = PWM.MGMOD;
            }
        }
        else
        {
            if(rgl.xmod > PWM.MMAXCW)
            {
                rgl.srp = PWM.MMAXCW;
                rgl.xmod = PWM.MMAXCW;
            }
        }
        return;
    }
    if(rgl.xmod >= PWM.MDMOD) m = (rgl.xmod - PWM.MDMOD)*10;
    else m = 0;
    m = m / (DtCW.MAD_CW - DtCW.DAD_CW); //DAD_CW-MAD_CW
    m = m * (DtCW.AC_CW - DtCW.DAD_CW); //DAD_CW-AC_CW
    if(m < 0) m = 0;
    m = m / 10 + PWM.MDMOD;
    if(m > 0xff) //przepelnienie
    {
        rglf_fz = 0; //przejscie do fazy1 przy aktywnej fazie2
        rgl.tmp_pwm = rgl.xmod;
        return;
    }
    if(rglf_fz && (rgl.xmod > m))
        rgl.srp = (rgl.xmod - m); //przygotowanie lagodnego przejscia do fazy 1
    m += rgl.srp;
    if(m > rgl.xmod) p = rgl.xmod;
    else p = m;
    rglf_fz = 0; //przejscie do fazy1 przy aktywnej fazie2
    rgl.tmp_pwm = p;
}
//-------------------------------------------------------------
//drugie przyblizenie (tylko dla fazy 2)

void ModDkCW4(void)
{
    signed long m;
    signed char ada;
    unsigned char k, p;

    if(!rglf_fz) return; //wyjscie jesli aktualnie faza 1
    ada = DtCW.AC_CW - DtCW.DAD_CW;
    k = 2; //9
    if(ada < 0) k = 1; //8
    m = rgl.srp + k*ada;
    if(m > rgl.xmod) p = rgl.xmod;
    else
        if(m < PWM.MDMOD) p = PWM.MDMOD;
    else p = m;
    rgl.tmp_pwm = p;
}
//-------------------------------------------------------------
//trzecie przyblizenie
//nowa wartosc punktu rownowagi srp.
//wykonywane co krok czasowy

void ModPruCW4(void)
{
    static unsigned char l, m, o, p, q, r, s;
    unsigned char tmp;
    signed char k;
    if(!rglf_fz) //aktualnie faza 1?
    {
        l = 0;
        m = 0;
        o = 0;
        p = 0;
        if(q < 255) q++; //licznik sekund
        if(s < 255) s++; //licznik sekund
        if(DtCWf_newPCW) s = 0; //zmieniono nastawe?
        if(Tnizsza(0)) //Tcw<Tnst?
        {
            if(!Tnizsza(10)) //Tnst-Tcw<=10 ?
            {
                k = 0;
                if(Tstoi() && s >= 1)
                {
                    s = 0;
                    k = 2;
                }
                if(Trosnie(1) && s >= 1) //szybki narost temperatury
                {
                    s = 0;
                    k = 10;
                    if(Trosnie(2)) k = 8; //b.szybki narost temperatury?
                }
                else
                    rgl.ytmp = 0;
                if(PWM.BufPWM > rgl.xmod) return;
                tmp = rgl.xmod - PWM.BufPWM;
                if(k > tmp) k = tmp;
                if(PWM.MMAXCW > PWM.MGMOD)
                {
                    if(rgl.srp + k > PWM.MGMOD) rgl.srp = PWM.MGMOD;
                    else rgl.srp += k;
                }
                else
                {
                    if(rgl.srp + k > PWM.MMAXCW) rgl.srp = PWM.MMAXCW;
                    else rgl.srp += k;
                }
                return;
            }
            else return;
        }
        else return;
    }
    else //aktualnie faza 2
    {
        q = 0;
        r = 0;
        s = 0;
        if(m < 255) m++; //licznik sekund dla spadku temp CO
        if(p < 255) p++; //licznik sekund dla spadku temp CO
        if(o < 255) o++; //licznik sekund dla narostu temp CO
        if(DtCW.DAD_CW > DtCW.AC_CW) //Tcw>Tnst ?
        {
            k = 0;
            if(Trosnie(0) && m >= 2)
            {
                p = 0;
                o = 0;
                l = 0;
                m = 0;
                k = 2; //3
            }
            else
                if(Twyzsza(1)) //gdy jest powyzej zakresu nastawa+1
            {
                if(Tstoi())
                {
                    if(l >= 2)
                    {
                        k = 1;
                        if(Twyzsza(3)) k = 2;
                        l = 0;
                    }
                    else
                    {
                        k = 0;
                        l++;
                    }
                }
            }
            else l = 0;
            if(rgl.srp - k < PWM.MDMOD) rgl.srp = PWM.MDMOD;
            else rgl.srp -= k;
            if(rgl.srp > PWM.MMAXCW) rgl.srp = PWM.MMAXCW;
            if(rgl.xmod < rgl.srp) rgl.xmod = rgl.srp;
            return;
        }
        else //Tcw<Tnst
        {
            k = 0;
            if(Tmaleje(0) && m >= 2)
            {
                p = 0;
                o = 0;
                l = 0;
                m = 0;
                k = 1; //2
                //if(Tnizsza(0)) k=3;				//3
            }
            else
                if(Tnizsza(1))
            {
                if(Tstoi())
                {
                    if(l >= 3)
                    {
                        k = 2;
                        l = 0;
                    }
                    else
                    {
                        k = 0;
                        l++;
                    }
                }
            }
            else l = 0;
            if(PWM.MMAXCW > PWM.MGMOD)
            {
                if(rgl.srp + k > PWM.MGMOD) rgl.srp = PWM.MGMOD;
                else rgl.srp += k;
            }
            else
            {
                if(rgl.srp + k > PWM.MMAXCW) rgl.srp = PWM.MMAXCW;
                else rgl.srp += k;
            }
            if(rgl.xmod < rgl.srp) rgl.xmod = rgl.srp;
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
    if(rglf_fz) //faza 2 ?
    {
        if(PWM.MMAXCW > PWM.MGMOD)
        {
            if(rgl.xmod > PWM.MGMOD) rgl.xmod = PWM.MGMOD;
        }
        else
        {
            if(rgl.xmod > PWM.MMAXCW) rgl.xmod = PWM.MMAXCW;
        }
        return;
    }

    //kontrolowany narost mocy dla fazy 1
    if(Trosnie(1)) return; //(Tcw lub Tco rosnie szybciej niz 1K/sek)?
    k = 5;
    if(rgl.xmod + k > _MG2_MOD) rgl.xmod = _MG2_MOD;
    else rgl.xmod += k;
    if(rgl.xmod > PWM.MMAXCW) rgl.xmod = PWM.MMAXCW;
}
//------------------------------------------------------------
//reakcja superwizyjna

void ModPrzCW4(void)
{
    static unsigned char trb = 0;
    unsigned int p;

    p = (rgl.tmp_pwm - PWM.MDMOD)*100;
    p = p / (PWM.MGMOD - PWM.MDMOD);

    if(!rglf_nmod) //nie jest w trybie nmod?
    {
        trb = 0;
        if(p <= 35)
        {
            if(!Tmaleje(0) && Twyzsza(3)) //(Tcw nie maleje ?)
            {
                trb = 1;
                rglf_nmod = 1; //wejdz w tryb nmod
            }
        }
        else
        {
            if(!Tmaleje(0)) //(Tcw nie maleje ?)
            {
                if(Twyzsza(5)) //Tcw-Tnst>4 ?
                {
                    if(rgl.srp - 3 < PWM.MDMOD) rgl.srp = PWM.MDMOD;
                    else rgl.srp -= 3;
                    trb = 2;
                    rglf_nmod = 1; //wejdz w tryb nmod
                    return;
                }
            }
            else return;
        }
    }
    else //jest w trybie nmod?
    {
        switch(trb)
        {
            case 1:
            {
                if(!Twyzsza(3))
                {
                    if(Tnizsza(3) || Tstoi() || Tmaleje(0))
                    {
                        trb = 0;
                        rglf_nmod = 0; //jezeli Tcw maleje wyjdz z trybu nmod
                    }
                }
                break;
            }
            case 2:
            {
                if(Tmaleje(0) || !Twyzsza(4)) //(Tcw maleje ?)
                {
                    trb = 0;
                    rglf_nmod = 0; //wyjdz z trybu nmod
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
    if(!rglf_nmod) //nie jest w trybie nmod?
    {
        if(!DtCOf_err_CO) //awaria czujnika temperatury CO?
        {
            if(rgl.x_f == 3) //=86C ?
            {
                if(rgl.tmp_pwm > PWM.MGMOD)
                    rgl.tmp_pwm = PWM.MGMOD; //=MGMOD
            }
            else
                if(rgl.x_f == 2) //=87C ?
            {
                if(rgl.tmp_pwm > PWM.PROP1)
                    rgl.tmp_pwm = PWM.PROP1; //-1/7
            }
            else
                if(rgl.x_f == 1) //=88C ?
            {
                if(rgl.tmp_pwm > PWM.PROP2)
                    rgl.tmp_pwm = PWM.PROP2; //-2/7
            }
            else
                if(rgl.x_f <= 0) //>=89C ?
            {
                if(rgl.tmp_pwm > PWM.PROP3)
                    rgl.tmp_pwm = PWM.PROP3; //-3/7
            }
        }
        WriteOPWM(rgl.tmp_pwm);
    }
    else WriteOPWM(PWM.MDMOD); //wymus minimum plomienia
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
    //	unsigned char k;

    if(rglf_btco || rglf_btcw) return; //praca w dowolnym obiegu CO?
    StartRTdS(_RTMOD);
    DtCO.dDCO = 0;
    DtCO.indCO = 0;
    DtCW.dDCW = 0;
    DtCW.indCW = 0;
    rglf_fz = 0;
    rglf_med_up = 0;
    rglf_fast_up = 0;
    //CO - dobor wartosci pierwszego pobudzenia (moment startu modulacji)
    //if(!PWMf_tpGZ) rgl.xmod=I80_MOD;
    //else rgl.xmod=I80_PMOD;
    if(PWM.MMAXCO > PWM.FIRSTPB)
    {
        rgl.xmod = PWM.FIRSTPB;
    }
    else
    {
        rgl.xmod = PWM.MMAXCO;
    }
    /*
            if(!PWMf_tpGZ)
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
    rgl.srp = 0;
    rgl.ytmp = 0;
    rglf_nmod = 0; //wyjdz z trybu nmod
    rglf_del1s = 0;
    rglf_deln1 = 0;
    rglf_inv_mod = 0;
}
//-------------------------------------------------------------
//parametry startowe dla CW

void StartMCW2(void)
{
    if(rglf_btco || rglf_btcw) return; //rozpoczecie pracy modulatora?
    StartRTdS(_RTMOD);
    DtCO.dDCO = 0;
    DtCW.dDCW = 0;
    DtCO.indCO = 0;
    DtCW.indCW = 0;
    rglf_fz = 0;
    FirstPCW2();
    rgl.srp = 0;
    rgl.ytmp = 0;
    rglf_del1s = 0;
}
//-------------------------------------------------------------	
//pierwsze pobudzenie dla CW

void FirstPCW2(void)
{
    if(PWM.MMAXCW > PWM.MGMOD)
    {
        if(DtCW.DCW > 0) rgl.xmod = PWM.MGMOD;
        else rgl.xmod = PWM.FIRSTPB;
    }
    else
    {
        if(DtCW.DCW > 0) rgl.xmod = PWM.MMAXCW;
        else
        {
            if(PWM.MMAXCW > PWM.FIRSTPB) rgl.xmod = PWM.FIRSTPB;
            else rgl.xmod = PWM.MMAXCW;
        }
    }
    /*	if(!PWMf_tpGZ)
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
    rglf_fz = 0;
    rgl.xmod = PWM.BufPWM;
    rgl.srp = 0;
    rgl.ytmp = 0;
    rglf_del1s = 0;
}
//-------------------------------------------------------------
//zmien obieg na CW

void ModToCW2(void)
{
    rglf_btcw = 1;
    if(rglf_btco)
    {
        rglf_btco = 0;
        ClrMod2();
    }
}
//-------------------------------------------------------------
//zmien obieg na CO

void ModToCO2(void)
{
    rglf_btco = 1;
    if(rglf_btcw)
    {
        rglf_btcw = 0;
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
        else rgl.ytmp = 0;
    }
    else rgl.ytmp = 0;
}
//-------------------------------------------------------------

void IncConstCO2(void)
{
    if(Tstoi()) rgl.ytmp++;
    else rgl.ytmp = 0;
}
//-------------------------------------------------------------	
//wprowadzenie danych wejsciowych-CO

void ReadDataCO2(void)
{
    rgl.x_t = DtCO.DCO;
    rgl.x_dt = DtCO.dDCO;
}
//-------------------------------------------------------------
//wprowadzenie danych wejsciowych-CW

void ReadDataCW2(void)
{
    rgl.x_t = DtCW.DCW;
    rgl.x_f = DtCO.DCO;
    rgl.x_dt = DtCW.dDCW;
    rgl.x_df = DtCO.dDCO;
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
        pop = 20; //wartosc poprawki na przesterowanie
        FirstPCW2();
    }
    if(rglf_fz) //aktualnie faza 2?
    {
        if(Tnizsza(2)) //Tnizsza(2)przygotowanie do przejscia do fazy 1
        {
            rgl.xmod = PWM.BufPWM;
            rgl.srp = 0;
            rgl.ytmp = 0;
            rglf_del1s = 0;
            pop = 0;
        }
        else return;
    }
    if(!Tnizsza(1)) //warunek przejscia do fazy 2
    {
        rglf_fz = 1;
        rglf_fast_up = 0;
        rglf_med_up = 0;
        rgl.srp = PWM.BufPWM + pop;
        rgl.xmod = PWM.BufPWM + pop;
        pop = 0;
        if(PWM.MMAXCW > PWM.MGMOD)
        {
            if(rgl.xmod > PWM.MGMOD)
            {
                rgl.srp = PWM.MGMOD;
                rgl.xmod = PWM.MGMOD;
            }
        }
        else
        {
            if(rgl.xmod > PWM.MMAXCW)
            {
                rgl.srp = PWM.MMAXCW;
                rgl.xmod = PWM.MMAXCW;
            }
        }
        return;
    }
    if(rgl.xmod >= PWM.MDMOD) m = (rgl.xmod - PWM.MDMOD)*10;
    else m = 0;
    m = m / (DtCW.MAD_CW - DtCW.DAD_CW); //DAD_CW-MAD_CW
    m = m * (DtCW.AC_CW - DtCW.DAD_CW); //DAD_CW-AC_CW
    if(m < 0) m = 0;
    m = m / 10 + PWM.MDMOD;
    if(m > 0xff) //przepelnienie
    {
        rglf_fz = 0; //przejscie do fazy1 przy aktywnej fazie2
        rgl.tmp_pwm = rgl.xmod;
        return;
    }
    if(rglf_fz && (rgl.xmod > m))
        rgl.srp = (rgl.xmod - m); //przygotowanie lagodnego przejscia do fazy 1
    m += rgl.srp;
    if(m > rgl.xmod) p = rgl.xmod;
    else p = m;
    rglf_fz = 0; //przejscie do fazy1 przy aktywnej fazie2
    rgl.tmp_pwm = p;
}
//-------------------------------------------------------------
//drugie przyblizenie (tylko dla fazy 2)

void ModDkCW2(void)
{
    signed long m;
    signed char ada;
    unsigned char k, p;

    if(!rglf_fz) return; //wyjscie jesli aktualnie faza 1
    ada = DtCW.AC_CW - DtCW.DAD_CW;
    k = 9; //9
    if(ada < 0) k = 8; //8
    m = rgl.srp + k*ada;
    if(m > rgl.xmod) p = rgl.xmod;
    else
        if(m < PWM.MDMOD) p = PWM.MDMOD;
    else p = m;
    rgl.tmp_pwm = p;
}
//-------------------------------------------------------------
//trzecie przyblizenie
//nowa wartosc punktu rownowagi srp.
//wykonywane co krok czasowy

void ModPruCW2(void)
{
    static unsigned char l, m, o, p, q, r, s;
    unsigned char tmp;
    signed char k;
    if(!rglf_fz) //aktualnie faza 1?
    {
        l = 0;
        m = 0;
        o = 0;
        p = 0;
        if(q < 255) q++; //licznik sekund
        if(s < 255) s++; //licznik sekund
        if(DtCWf_newPCW) s = 0; //zmieniono nastawe?
        if(Tnizsza(0)) //Tcw<Tnst?
        {
            if(!Tnizsza(10)) //Tnst-Tcw<=10 ?
            {
                k = 0;
                if((Trosnie(1) || TFrosnie(1)) && s >= 1) //szybki narost temperatury
                {
                    s = 0;
                    k = 10;
                    if(Trosnie(2) || TFrosnie(2)) k = 8; //b.szybki narost temperatury?
                }
                else
                    if(TFmaleje(0) && q >= 1)
                {
                    q = 0;
                    k = 6;
                }
                else
                    if(TFstoi())
                {
                    if(r >= 1)
                    {
                        k = 6;
                        r = 0;
                    }
                    else
                    {
                        k = 0;
                        r++;
                    }
                }
                rgl.ytmp = 0;
                if(PWM.BufPWM > rgl.xmod) return;
                tmp = rgl.xmod - PWM.BufPWM;
                if(k > tmp) k = tmp;
                //if(rgl.srp+k>PWM.MGMOD) rgl.srp=PWM.MGMOD;
                //else rgl.srp+=k;
                if(PWM.MMAXCW > PWM.MGMOD)
                {
                    if(rgl.srp + k > PWM.MGMOD) rgl.srp = PWM.MGMOD;
                    else rgl.srp += k;
                }
                else
                {
                    if(rgl.srp + k > PWM.MMAXCW) rgl.srp = PWM.MMAXCW;
                    else rgl.srp += k;
                }
                return;
            }
            else return;
        }
        else return;
    }
    else //aktualnie faza 2
    {
        q = 0;
        r = 0;
        s = 0;
        if(m < 255) m++; //licznik sekund dla spadku temp CO
        if(p < 255) p++; //licznik sekund dla spadku temp CO
        if(o < 255) o++; //licznik sekund dla narostu temp CO
        if(DtCW.DAD_CW > DtCW.AC_CW) //Tcw>Tnst ?
        {
            k = 0;
            if(TFmaleje(0) && !Twyzsza(0) && p >= 2) //(Tmaleje(0)||TFmaleje(0)
            {
                p = 0;
                o = 0;
                l = 0;
                m = 0;
                k = -1; //1
            }
            else
                if(TFrosnie(0) && o >= 2)
            {
                o = 0;
                p = 0;
                l = 0;
                m = 0;
                k = 2; //3
                if(Trowna()) k = 1;
            }
            else
                if(Trosnie(0) && m >= 2)
            {
                p = 0;
                o = 0;
                l = 0;
                m = 0;
                k = 3; //2
            }
            else
                if(Twyzsza(1)) //gdy jest powyzej zakresu nastawa+1
            {
                if(Tstoi() && TFstoi())
                {
                    if(l >= 2)
                    {
                        k = 1;
                        if(Twyzsza(3)) k = 2;
                        l = 0;
                    }
                    else
                    {
                        k = 0;
                        l++;
                    }
                }
            }
            else l = 0;
            if(rgl.srp - k < PWM.MDMOD) rgl.srp = PWM.MDMOD;
            else rgl.srp -= k;
            if(rgl.srp > PWM.MMAXCW) rgl.srp = PWM.MMAXCW;
            if(rgl.xmod < rgl.srp) rgl.xmod = rgl.srp;
            return;
        }
        else //Tcw<Tnst
        {
            k = 0;
            if(TFrosnie(0) && o >= 2) //(Trosnie(0)||TFrosnie(0))
            {
                o = 0;
                p = 0;
                l = 0;
                m = 0;
                k = -3;
                if(Trowna()) k = -2;
            }
            else
                if(TFmaleje(0) && p >= 2)
            {
                p = 0;
                o = 0;
                l = 0;
                m = 0;
                k = 2; //3
                if(Trowna()) k = 1;
            }
            else
                if(Tmaleje(0) && m >= 2)
            {
                p = 0;
                o = 0;
                l = 0;
                m = 0;
                k = 2;
                if(Tnizsza(0)) k = 3; //3
            }
            else
                if(Tnizsza(1))
            {
                if(Tstoi() && TFstoi())
                {
                    if(l >= 3)
                    {
                        k = 2;
                        l = 0;
                    }
                    else
                    {
                        k = 0;
                        l++;
                    }
                }
            }
            else l = 0;
            if(PWM.MMAXCW > PWM.MGMOD)
            {
                if(rgl.srp + k > PWM.MGMOD) rgl.srp = PWM.MGMOD;
                else rgl.srp += k;
            }
            else
            {
                if(rgl.srp + k > PWM.MMAXCW) rgl.srp = PWM.MMAXCW;
                else rgl.srp += k;
            }
            if(rgl.xmod < rgl.srp) rgl.xmod = rgl.srp;
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
    if(rglf_fz) //faza 2 ?
    {
        if(PWM.MMAXCW > PWM.MGMOD)
        {
            if(rgl.xmod > PWM.MGMOD) rgl.xmod = PWM.MGMOD;
        }
        else
        {
            if(rgl.xmod > PWM.MMAXCW) rgl.xmod = PWM.MMAXCW;
        }
        return;
    }
    //kontrolowany narost mocy dla fazy 1
    if(Trosnie(1) || TFrosnie(1)) return; //(Tcw lub Tco rosnie szybciej niz 1K/sek)?
    k = 5;
    if(rgl.xmod + k > _MG2_MOD) rgl.xmod = _MG2_MOD;
    else rgl.xmod += k;
    if(rgl.xmod > PWM.MMAXCW) rgl.xmod = PWM.MMAXCW;
}
//-------------------------------------------------------------
//reakcja superwizyjna

void ModPrzCW2(void)
{
    if(!rglf_nmod) //nie jest w trybie nmod?
    {
        if(!Tmaleje(0) && Twyzsza(3)) //(Tcw nie maleje ?)
        {
            rglf_nmod = 1; //wejdz w tryb nmod
        }
    }
    else //jest w trybie nmod?
    {
        if(!Twyzsza(3))
        {
            if(Tmaleje(0)) rglf_nmod = 0; //jezeli Tcw maleje wyjdz z trybu nmod
            if(Tstoi() && TFmaleje(0)) rglf_nmod = 0; //(Tcw stoi i Tco maleje) wyjdz z trybu nmod
            if(Tnizsza(3)) rglf_nmod = 0;
        }
    }
}
//------------------------------------------------------------
//Zabezpieczenie od przegrzania w obwodzie CO

void PropCO2(void)
{
    if(!rglf_nmod) //nie jest w trybie nmod?
    {
        if(rgl.x_f == 3) //=86C ?
        {
            if(rgl.tmp_pwm > PWM.MGMOD)
                rgl.tmp_pwm = PWM.MGMOD; //=MGMOD
        }
        else
            if(rgl.x_f == 2) //=87C ?
        {
            if(rgl.tmp_pwm > PWM.PROP1)
                rgl.tmp_pwm = PWM.PROP1; //-1/7
        }
        else
            if(rgl.x_f == 1) //=88C ?
        {
            if(rgl.tmp_pwm > PWM.PROP2)
                rgl.tmp_pwm = PWM.PROP2; //-2/7
        }
        else
            if(rgl.x_f <= 0) //>=89C ?
        {
            if(rgl.tmp_pwm > PWM.PROP3)
                rgl.tmp_pwm = PWM.PROP3; //-3/7
        }
        WriteOPWM(rgl.tmp_pwm);
        //PWM.BufPWM=rgl.tmp_pwm;
    }
    else WriteOPWM(PWM.MDMOD); //PWM.BufPWM=PWM.MDMOD;			//wymus minimum plomienia
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
    rglf_btcw = 1;
    if(rglf_btco) //aktywny jest obieg CO?
    {
        rglf_btco = 0;
        //wstepne obnizenie mocy palnika
        if(!Tnizsza(0))
        {
            if(rgl.srp - 10 < PWM.MDMOD) rgl.srp = PWM.MDMOD;
            else rgl.srp -= 10;
        }
    }
}
//-------------------------------------------------------------
//zmien obieg na CO

void ModToCO3(void)
{
    rglf_btco = 1;
    rglf_btcw = 0;
}
//-------------------------------------------------------------
//parametry startowe - kociol z zasobnikiem.

void StartMCO3(void)
{
    if(rglf_btco || rglf_btcw) return; //praca w dowolnym obiegu CO?
    StartRTdS(_RTMOD);
    DtCO.dDCO = 0;
    DtCO.indCO = 0;
    rglf_fz = 0;
    rglf_med_up = 0;
    rglf_fast_up = 0;
    //CO - dobor wartosci pierwszego pobudzenia (moment startu modulacji)
    if(PWM.MAXPWM > PWM.FIRSTPB)
    {
        rgl.xmod = PWM.FIRSTPB;
    }
    else
    {
        rgl.xmod = PWM.MAXPWM;
    }
    rgl.srp = 0;
    rgl.ytmp = 0;
    rglf_nmod = 0; //wyjdz z trybu nmod
    rglf_btcw = 0;
    rglf_btco = 1;
    rglf_del1s = 0;
    rglf_deln1 = 0;
    rglf_inv_mod = 0;
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
    return(rgl.x_dt > t);
}
//------------------------------------------------------------
//zwraca wartosc 1 przy stwierdzonym spadku temperatury
//(w obiegu CW)
//dla t=0 zwraca wartosc jeden przy dowolnym spadku temp.
//dla t>0 zwraca wartosc jeden przy spadku > od t [C/sek].

bit Tmaleje(const unsigned char t)
{
    return(rgl.x_dt < (-1 * t));
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
    return(rgl.x_t > t);
}
//------------------------------------------------------------
//zwraca wartosc 1 temperatura jes nizsza od nastawy o wartosc parametru t
//(w obiegu CW)
//dla t=0 zwraca wartosc jeden jezeli temp. jest nizsza od nastawy.
//dla t>0 zwraca wartosc jeden jezeli temp. jest nizsza od nastawy o >= t.

bit Twyzsza(const unsigned char t)
{
    return(rgl.x_t < (-1 * t));
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
    return(rgl.x_df > t);
}
//------------------------------------------------------------
//zwraca wartosc 1 przy stwierdzonym spadku temperatury
//(w obiegu CO)
//dla t=0 zwraca wartosc jeden przy dowolnym spadku temp.
//dla t>0 zwraca wartosc jeden przy spadku > od t [C/sek].

bit TFmaleje(const unsigned char t)
{
    return(rgl.x_df < (-1 * t));
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
    rglf_btco = 0;
    rglf_btcw = 0;
    rglf_fz = 0;
    rglf_med_up = 0;
    rglf_fast_up = 0;
    rglf_nmod = 0;
    rglf_del1s = 0;
    rglf_deln1 = 0;
    rglf_inv_mod = 0;
}
