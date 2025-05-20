/*konsola.c*/
//Funkcje obslugi przycisków p³ytki interfejsu
// (!) Bazuje na funkcjach z plikow: rtimer.c
/*---------------------------------------------------------------------------------*/
#include <pic18.h>
#include "global.h"			//parametry globalne
#include "konsola.h"			//parametry lokalne
#include "main.tp.h"
#include "print.h"

//extern unsigned char x,y;
/*---------------------------------------------------------------------------------*/
extern tpMAIN M;
extern DataPFN PFN;
//extern tpRSDTA1 RSDT1;

#if _SRVTRB==1						//aktywny tryb serwisowy kompilacji?
unsigned char mypwm = _MG2_MOD;
#endif
extern StPin xxky[];
extern StPin ky[];


//Zapis nastawy PCW po ustalonym czasie do EEPROM
/*void WriteParam(unsigned char *zapis)
{
                if(*zapis&&RTdS(_RTPCW)>=_TWRTPCW)
                {
 *zapis=0;
                        StartRTdS(_RTPCW);
                        //WriteEEPCW();
                }
}*/
/*---------------------------------------------------------------------------------*/
//Inkrementacja/Dekrementacja parametru
//gdzie:
//param- adres modyfikowanego parametru (16bit)
//zmiana - znacznik ze wartosc parametru ulegla zmianie (0- brak zmianay, 1 - tylko podglad min/max, 2 - wartosc zmniejszona, 3 - wartosc zwiekszona)
//zapis - znacznik sugestji/aktywacji zapisu parametru do pamieci
//min- minimalna wartosc jaka moze przyjac parametr
//max- maksymalna wartosc jaka moze przyjac parametr
//rckl - po osiagnieciu max+1 nastepuje przeladowanie wartoscia min i odwrotnie po osiagnieciu min-1 nastepuje przeladowanie wartoscia max
//fst - aktywacja funkcji przyspieszajacej inkrementacji/dekrementacji

void ReadParam(volatile unsigned int *param, unsigned char *zmiana, unsigned char *zapis, unsigned int min, unsigned int max, unsigned char rckl, unsigned char fst)
{
    volatile static unsigned char pnst;
    volatile unsigned char stp, ovi, ovd;

    ovi = 0;
    ovd = 0;
    if(*param < min) *param = min;
    if(*param > max) *param = max;
    if((max - min) > 99) stp = 5;
    else stp = 1;
    if(!(RdPrt(S_KDW) && RdPrt(S_KUP))) //nie nacisnieto jednoczesnie obu przyciskow?
    {
        if(!RdPrt(S_KDW))
        {
            ReadDataXONLID(RdPrt(S_KUP), &xxky[SS_KUP], param, &ovi, 1, min, max, 1, stp, fst);
            if(rckl && ovi) *param = min;
            if(!rckl && RdPrt(S_KUP) && (*param == max)) *zmiana = 1; //wymuszony wskaz nastawy dla maksimum
        }
        else
            if(!RdPrt(S_KUP))
        {
            ReadDataXONLID(RdPrt(S_KDW), &xxky[SS_KDW], param, &ovd, 0, min, max, 1, stp, fst);
            if(rckl && ovd) *param = max;
            if(!rckl && RdPrt(S_KDW) && (*param == min)) *zmiana = 1; //wymuszony wskaz nastawy dla minimum
        }
        if(!RdPrt(S_KDW) && !RdPrt(S_KUP)) //oba przyciski wycisniete
        {
            if(RTdS(_RTPCW) > _TFLPCW) *zmiana = 0; //podtrzymanie stanu znacznika modyfikacji przez czas _TWRTPCW-1
        }

        if(*param != pnst) //nastapila zmiana nastawy?
        {
            if(rckl && ovi) *zmiana = 3;
            else
                if(rckl && ovd) *zmiana = 2;
            else *zmiana = (pnst<*param) ? 3 : 2; //znacznik zmiany nastawy
            pnst = *param;
            StartRTdS(_RTPCW); //zegar podtrzymania znacznika modyfikacji nastawy
            *zapis = 1; //sugestia zapisu dla nowej nastawy (uwaga po zapisie nalezy wyzerowac)
        }
    }
    else *zmiana = 0; //nacisnieto dwa przyciski jednoczesnie
}

/*---------------------------------------------------------------------------------*/
void TrybPracy(void)
{
    //static bit bf_oaleg;
    volatile unsigned char key, key2, ExtR, ExtR2, tm;

    if(!PFNf_aktywne_KNF && !PFNf_opgres)
    {
        //--------------------------PRZELACZNIK FUNKCJI------------------------------
        //przytrzymano przycisk RESET (wylaczenie/zalaczenie sterownika)
        if(!RdPrt(S_SET))
        {
            key = PresKeyEX(RdPrt(S_OFF), &ky[SS_OFF]);
            if(key != _KDFST && key >= _TKOFF)
            {
                WaitToLowEx(&ky[SS_OFF]);
                PFNf_off = !PFNf_off;
                PFNf_set_nco = 0;
                PFNf_set_ncw = 0;
                PFNf_str_nco = 0;
                PFNf_str_ncw = 0;
                PFNf_prn_res = 0;
                PFNf_set_fnserw = 0;
                PFNf_fnserw = 0;
                PFNf_set_aleg = 0;
                PFNf_aleg = 0;
                ResBufKeyEx(&ky[SS_KUP]);
                ResBufKeyEx(&ky[SS_KDW]);
                ResBufKeyEx(&ky[SS_SET]);
            }
        }
        //---------------------
        if(!PFNf_off && !PFNf_lato && !PFNf_zima) //stan nieustalony?
        {
            PFNf_lato = 0;
            PFNf_zima = 1;
        }
        //przytrzymano przycisk SET (zmiana trybu pracy)
        if(!RdPrt(S_OFF) && !RdPrt(S_KUP) && !RdPrt(S_KDW) && !PFNf_off && !PFNf_fnserw && !PFNf_aleg)
        {
            key = PresKeyEX(RdPrt(S_SET), &ky[SS_SET]);
            if(key != _KDFST && key >= _TKSET)
            {
                WaitToLowEx(&ky[SS_SET]);
                PFNf_lato = !PFNf_lato;
                PFNf_zima = !PFNf_lato;
                PFNf_set_nco = 0;
                PFNf_set_ncw = 0;
                PFNf_fnserw = 0; //przerwanie funkcji serwisowej
                PFNf_aleg = 0; //przerwanie funkcji antylegionella
                PFNf_prn_res = 0;
            }
        }
        //--------------------------NASTAWY------------------------------
        //nacisnieto przycisk SET (wybor nastawy CO, funkcji serwisowej, antylegionella)
        if(PFNf_oaleg) PFNf_set_aleg = 0;
        if(PFNf_ofnserw) PFNf_set_fnserw = 0;

        if(!RdPrt(S_OFF) && !RdPrt(S_KUP) && !RdPrt(S_KDW) && !PFNf_off && !PFNf_fnserw && !PFNf_aleg)
        {
            key = PresKeyEX(RdPrt(S_SET), &ky[SS_SET]);
            if(key == _KDFST)
            {
                ResBufKeyEx(&ky[SS_SET]); //resetuj podtrzymanie
                PFNf_b_set_nco = 0;
                PFNf_b_set_ncw = 0;
                PFNf_set_ncw = 0;
                PFNf_prn_res = 0;
                if(PFNf_set_nco)
                {
                    if(PFNf_newPCO) //nastawa ulegla zmianie (potraktuj klawisz SET jako potwierdzenie
                    {
                        PFNf_newPCO = 0;
                        PFNf_set_nco = 0;
                        PFNf_b_set_nco = 0;
                        PFNf_set_ncw = 0;
                        PFNf_b_set_ncw = 0;
                        PFNf_set_fnserw = 0;
                        PFNf_set_aleg = 0;
                        SetPrt(S_KDW, 0);
                        SetPrt(S_KUP, 0);
                        SetPrt(S_SET, 0);
                        ResBufKeyEx(&ky[SS_KDW]); //resetuj podtrzymanie
                        ResBufKeyEx(&ky[SS_KUP]); //resetuj podtrzymanie
                    }
                    else
                    if(PFNf_zima && !PFNf_obgCW && !PFNf_set_fnserw)
                    {
                        PFNf_set_nco = 0;
                        PFNf_set_fnserw = 1;
                        PFNf_fns_max = 1;
                    }
                    else
                    if(PFNf_EnAleg && !PFNf_set_aleg)
                    {
                        PFNf_set_nco = 0;
                        PFNf_set_aleg = 1;
                    }
                    else
                    {
                        PFNf_newPCO = 0;
                        PFNf_set_nco = 0;
                        PFNf_b_set_nco = 0;
                        PFNf_set_ncw = 0;
                        PFNf_b_set_ncw = 0;
                        PFNf_set_fnserw = 0;
                        PFNf_set_aleg = 0;
                        SetPrt(S_KDW, 0);
                        SetPrt(S_KUP, 0);
                        SetPrt(S_SET, 0);
                        ResBufKeyEx(&ky[SS_KDW]); //resetuj podtrzymanie
                        ResBufKeyEx(&ky[SS_KUP]); //resetuj podtrzymanie
                    }
                }
                else
                if(PFNf_set_fnserw && !PFNf_ferr && !PFNf_fmsg)
                {
                    if(PFNf_EnAleg && !PFNf_set_aleg)
                    {
                        PFNf_set_fnserw = 0;
                        PFNf_set_aleg = 1;
                    }
                    else
                    {
                        PFNf_newPCO = 0;
                        PFNf_set_nco = 0;
                        PFNf_b_set_nco = 0;
                        PFNf_set_ncw = 0;
                        PFNf_b_set_ncw = 0;
                        PFNf_set_fnserw = 0;
                        PFNf_set_aleg = 0;
                    }
                }
                else
                if(PFNf_set_aleg && !PFNf_ferr && !PFNf_fmsg)
                {
                    PFNf_newPCO = 0;
                    PFNf_set_nco = 0;
                    PFNf_b_set_nco = 0;
                    PFNf_set_ncw = 0;
                    PFNf_b_set_ncw = 0;
                    PFNf_set_fnserw = 0;
                    PFNf_set_aleg = 0;
                }
                else
                {
                    PFNf_set_fnserw = 0;
                    PFNf_set_aleg = 0;
                    PFNf_set_nco = 1;
                }
                StartRTdS(_RTSET);
                if(PFNf_set_nco)
                {
                    PFNf_b_set_nco = 0;
                    RestartKol(_WS_NCWCO);
                }
            }
        }
        if(RTdS(_RTSET) > _TSETNST) //uplynal czas podtrzymania wybranego parametru?
        {
            PFNf_set_nco = 0;
            PFNf_b_set_nco = 0;
            PFNf_set_ncw = 0;
            PFNf_b_set_ncw = 0;
            PFNf_set_fnserw = 0;
            PFNf_set_aleg = 0;
        }

        if(!RdPrt(S_OFF) && !RdPrt(S_KDW) && !PFNf_off && !PFNf_fnserw && !PFNf_aleg)
        {
            if(PFNf_set_fnserw)
            {
                key = PresKeyEX(RdPrt(S_KUP), &ky[SS_KUP]);
                if(key != _KDFST && key >= _TKALG)
                {
                    WaitToLowEx(&ky[SS_KUP]);
                    PFNf_fnserw = 1;
                    PFNf_prn_res = 0;
                    PFNf_set_nco = 0;
                    PFNf_set_ncw = 0;
                    PFNf_b_ofnserw = 0;
                }
            }
            if(PFNf_set_aleg)
            {
                key = PresKeyEX(RdPrt(S_KUP), &ky[SS_KUP]);
                if(key != _KDFST && key >= _TKSRV)
                {
                    WaitToLowEx(&ky[SS_KUP]);
                    PFNf_aleg = 1;
                    PFNf_prn_res = 0;
                    PFNf_set_nco = 0;
                    PFNf_set_ncw = 0;
                    PFNf_b_oaleg = 0;
                }
            }
        }
        //---------------------
        //nacisnieto przycisk UP lub DW (zmiana nastawy CW)
        //if(!RdPrt(S_OFF) && !RdPrt(S_SET) && !PFNf_off && !PFNf_fnserw && !PFNf_aleg && !PFNf_set_fnserw && !PFNf_set_aleg && !PFNf_set_nco && !PFNf_set_ncw)
        if(!RdPrt(S_OFF) && !RdPrt(S_SET) && !PFNf_off && !PFNf_prnserw && !PFNf_prnaleg && !PFNf_set_nco && !PFNf_set_ncw)
        {
            key = PresKeyEX(RdPrt(S_KDW), &ky[SS_KDW]);
            key2 = PresKeyEX(RdPrt(S_KUP), &ky[SS_KUP]);
            if(key == _KDFST || key >= _TKSET2 || key2 == _KDFST || key2 >= _TKSET2) //key>=_TKSET
            {
                ResBufKeyEx(&ky[SS_KDW]); //resetuj podtrzymanie
                ResBufKeyEx(&ky[SS_KUP]); //resetuj podtrzymanie
                RestartKol(_WS_NCWCO);
                //				RestartKol2(_KL_NCWCO);
                //				RestartKol(_KL_NCWCO);
                PFNf_b_set_ncw = 0;
                PFNf_prn_res = 0;
                PFNf_set_ncw = 1;
                PFNf_mod_ncw = 0;
                StartRTdS(_RTSET);
            }
        }
        //--------------------------FUNKCJE SPECJALNE------------------------------
        ExtR = (!PFNf_pmpPWM || PFNf_errIN || !PFNf_ofzima || PFNf_obgCW || !PFNf_obgCO) ? 0 : 1;
        //ExtR2 = (PFNf_errIN || !PFNf_obgCO) ? 0 : 1;
        ExtR2=(PFNf_errIN||(PFNf_oflato&&!PFNf_obgCO)||PFNf_obgCW)?0:1;				//odwrotny warunek wskazu CS,IN
        
        //if(!ExtR) tm=_TPCS;
		//else tm=2*_TPCS;
        if(RTdS(_RTPCS) > _TPCS) //uplynal czas podtrzymania wskazu cisnienia?       
        {
            PFNf_prn_res = 0;
        }
        if(PFNf_prn_res && (PFNf_errCS || PFNf_fnoCS) && (PFNf_obgCW || PFNf_oflato)) //awaria CS lub brak CS i (aktywny obieg CW lub tryb LATO)?
        {
            PFNf_prn_res = 0;
        }
        if(PFNf_prn_res && (PFNf_errCS || PFNf_fnoCS) && PFNf_errIN) //nie ma co pokazywac?
        {
            PFNf_prn_res = 0;
        }

        if(!PFNf_prn_res)
        {
            PFNf_prn_cs = 0;
            PFNf_prn_in = 0;
        }
        else
        {
            if(ExtR) //Dzielony wskaz CS, IN
            {
                if(RTdS(_RTPCS)<(_TPCS / 2))
                {
                    PFNf_prn_cs = 1;
                    PFNf_prn_in = 0;
                }
                else
                {
                    PFNf_prn_cs = 0;
                    PFNf_prn_in = 1;
                }
            }
            else
            {
                if(ExtR2) //Dzielony wskaz CS i IN?
                {
                    if(PFNf_errCS || PFNf_fnoCS)
                    {
                        if(RTdS(_RTPCS) < _TPCS) //nadszedl czas podtrzymania wskazu temp wlotowej?
                        {
                            PFNf_prn_cs = 0;
                            PFNf_prn_in = 1;
                        }
                    }
                    else
                    {
                        if(RTdS(_RTPCS)<(_TPCS / 2)) //nadszedl czas podtrzymania wskazu temp wlotowej?
                        {
                            PFNf_prn_cs = 1;
                            PFNf_prn_in = 0;
                        }
                        else
                        {
                            if(!PFNf_errIN)
                            {
                                if(RTdS(_RTPCS) >= (_TPCS / 2)) //nadszedl czas podtrzymania wskazu temp wlotowej?
                                {
                                    PFNf_prn_cs = 0;
                                    PFNf_prn_in = 1;
                                }
                            }
                            else
                            {
                                PFNf_prn_cs = 1;
                                PFNf_prn_in = 0;
                            }
                        }
                    }
                }
                else //Wylaczny wskaz CS
                {
                    if(!(PFNf_errCS || PFNf_fnoCS))
                    {
                        if(RTdS(_RTPCS) < _TPCS) //nadszedl czas podtrzymania wskazu temp wlotowej?
                        {
                            PFNf_prn_cs = 1;
                            PFNf_prn_in = 0;
                        }
                    }
                    else
                    {
                        PFNf_prn_cs = 0;
                        PFNf_prn_in = 0;
                    }
                }
            }
        }
        //---------------------
        if(!PFNf_off && (PFNf_fnserw || PFNf_ofnserw)) //aktywna funkcja serwisowa?
        {
            if(PresKeyEX(RdPrt(S_KUP), &ky[SS_KUP]) > 0) //nacisnieto przycisk +?
            {
                ResBufKeyEx(&ky[SS_KUP]); //resetuj podtrzymanie
                PFNf_fns_max = 1;
            }
            if(PresKeyEX(RdPrt(S_KDW), &ky[SS_KDW]) > 0) //nacisnieto przycisk -?
            {
                ResBufKeyEx(&ky[SS_KDW]); //resetuj podtrzymanie
                PFNf_fns_max = 0;
            }
        }
        //--------------------------RESET------------------------------
        //nacisnieto przycisk RESET
        key = PresKeyEX(RdPrt(S_OFF), &ky[SS_OFF]);
        if(key == _KDFST)
        {
            //if(PFNf_set_nco || PFNf_set_ncw || PFNf_str_nco || PFNf_str_ncw || PFNf_prn_res || PFNf_set_fnserw || PFNf_fnserw || PFNf_set_aleg || PFNf_aleg)
            if(PFNf_set_nco || PFNf_set_ncw || PFNf_str_nco || PFNf_str_ncw || PFNf_prn_res || PFNf_prnserw || PFNf_prnaleg)
            {
                WaitToLowEx(&ky[SS_OFF]);
                PFNf_set_nco = 0;
                PFNf_set_ncw = 0;
                PFNf_str_nco = 0;
                PFNf_str_ncw = 0;
                PFNf_mod_ncw = 0;
                PFNf_prn_res = 0;
                PFNf_set_fnserw = 0;
                PFNf_fnserw = 0;
                PFNf_set_aleg = 0;
                PFNf_aleg = 0;
                //PFNf_blkcs = 1;
                //StartRTdS(_RTPCS);
            }
            else
            {
                //if(PFNf_blkcs&&RTdS(_RTPCS)>50) {PFNf_blkcs=0;PFNf_prn_res = 0;}
                
                //if(!PFNf_blkcs)
                if(!PFNf_off && !PFNf_ferr && !PFNf_fmsg)
                {
                    WaitToLowEx(&ky[SS_OFF]);
                    if(!PFNf_errCS || (PFNf_fnoCS && !PFNf_errIN))
                    {
                        PFNf_prn_res = 1;
                    }
                    StartRTdS(_RTPCS);
                }
            }
        }

    }
    else
    {
        PFNf_set_nco = 0;
        PFNf_set_ncw = 0;
        PFNf_str_nco = 0;
        PFNf_str_ncw = 0;
        PFNf_prn_res = 0;
        PFNf_set_fnserw = 0;
        PFNf_fnserw = 0;
        PFNf_set_aleg = 0;
        PFNf_aleg = 0;
    }
    //stan przycisku RESET
    if(RdPrt(S_OFF))
    {
        PFNf_res = 1;
    }
    else PFNf_res = 0;
}
