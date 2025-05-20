/*main.c*/
/*Oprogramowanie rodziny sterownikow T02 (na bazie p?ytki UNI-02) z interfejsem LED*/
/*dodana funkcja modulacja pwm obrotow pompy, pomiar temp wlotowej 08.03.2013*/
/*	14.04.2015
 poprawki:
        brak autowylaczenia funkcji serwisowej
        brak autowylaczenia f.antylegionella w trybie manual (kotly UNICO)
        poprawiona wizualizacja wybranego trybu pracy na I2C
 nowe:
	temp CO dla kotlow monoterm. zmienione na 89C
	wprowadzone podbicie mocy minimalnej po procedurze startowej
	maksymalny czas trwania funkcji serwisowej zmieniony na 60 min
        i2c pokazuje rzeczywista docelowa temp PPCO i PPCW
        powiekszona histereza przy grzaniu CO (+10)
        poprawiona proc GoldenEye dla wiekszej histerezy wylaczenia przy grzaniu CO
*/
#include <pic18.h>
#include <htc.h>
#include <stdio.h>
#include <string.h>
#include "global.h"			//parametry globalne
#include "main.h"			//parametry lokalne
#include "wsw.h"
#include "print.h"
#include "eeprom.h"
#include "pwm.h"
#include "czcw.tp.h"
#include "pomiar.h"

#include "keypad.h"
#include "konfig2.h"
#include "pompa.tp.h"
#include "rgl.h"

//tpRSDTA1 RSDT1;
volatile tpMAIN M;
volatile DataPFN PFN;
unsigned char zps, zmn;  

extern DtPMP PMP; //pompa.c
extern DtPWM PWM; //pwm.c
extern DataPrCW PrCW; //czcw.c
extern DataCO DtCO; //pomiar.c
extern DataCW DtCW;
extern DataZW DtZW;
//extern DataPFN PFN;
extern DataCSN DtCS;
extern DataIN DtIN;
extern DtKONF DtKNF;


extern DtRgl rgl;
volatile unsigned char x,xj;
extern StPin sp;
//extern unsigned char xxy, zzz, vvv;
#if _SRVTRB==1						//aktywny tryb serwisowy kompilacji?
extern unsigned char mypwm;
#endif

void main(void)
{
    //------------------
    //------------------
    InitOsc();
    InitGlobal();
    InitTimer1();
    InitPort();
    InitAC();
    InitWsw();
    InitPWM();
    InitI2C();
    InitUart();
    InitDataPmr();
    ClearData();

    //------------------
    //------------------
    if(!POR || !BOR) //zalaczono zasilanie?
    {
        Mf_PW_START = 1; //znacznik rozruchu sterownika po wlaczeniu zasilania
        TestScr(); //wysterowanie wszystkich segmentow wyswietlacza
        PwrOnDelay(); //zwloka na wypadek stanow nieustalonych
        EndTestScr(); //wyjdz z trybu wysterowania wszystkich segmentow wyswietlacza
    }
    else
    {
        Mf_RS_START = 1; //znacznik rozruchu sterownika po resecie
        ClsScr(); //wygas wyswietlacz
        PwrOnDelay(); //zwloka na wypadek stanow nieustalonych
        EndClsScr(); //zapal wyswietlacz
    }
    //Odczytaj parametry zapisane w pamieci EEPROM
    RdEEParam();

    //Stan po zalaczeniu zasilania - warunek wejscia do trybu konfiguracji
    if(!POR || !BOR) //zalaczono zasilanie?
    {
        POR = 1;
        BOR = 1;
        EnKonf(); //aktywacja oczekiwania na wejscie do konfiguracji
    }
    //ustaw znacznik aktualnego obiegu
    _POMPA = 0;
    switch(DtKNF.tpkt) //dla danego rodzaju kotla
    {
        case _BITERM: //bitermiczny (miniterm)
        case _BITERM2: //bitermiczny (maxiterm)
        {
            Mf_RPM_CW = !_POMPA;
            break;
        }
        case _UNICO:
        case _MONOHB:
        {
            Mf_RPM_CW = !_KCO_CW;
            break;
        }
        default: //domyslny monotermiczny
        {
            Mf_RPM_CW = !_KCO_CW;
            break;
        }
    }

    //Sprawd? czy nastapilo wylaczenie awaryjne z blokada
    if(M.ERR_BTY = eeprom_read(_ADRERR)) //uruchomienie po wylaczeniu awaryjnym?
    {
        if(DtKNF.robg == _OTWARTY) //obieg otwarty?
        {
            if(M.ERR_BTY == 5) //dozwolony kod bledu?
            {
                SetErr(M.ERR_BTY);//PrintErr(M.ERR_BTY, 1); //wyswietl kod bledu
                ErrPTG(); //przejdz do petli wylaczenia awaryjnego
            }
        }
        if((M.ERR_BTY >= 1) && (M.ERR_BTY <= 3)) //dozwolony kod bledu?
        {
            SetErr(M.ERR_BTY);//PrintErr(M.ERR_BTY, 1); //wyswietl kod bledu
            ErrPTG(); //przejdz do petli wylaczenia awaryjnego
        }
        else M.ERR_BTY = 0;
    }

    //Pomiar wartosci nastaw dla unikniecia migajacej nastawy po zal. zasilania
    PomiarCO(); //wartosc temperatury CO
    PomiarCW(); //wartosc temperatury CW
    PomiarZW(); //temperatura z czujnika zewnetrznego
    PomiarCS(); //wartosc cisnienia w instalacji CO
    PomiarPCO(); //wartosc nastawy CO
    PomiarPCW(); //wartosc nastawy CW
    PomiarPFN(); //stan przelacznika funkcji
    PomiarIN(); //wartosc temperatury na powrocie
    DtCOf_newPCO = 0; //deaktywacja wyswietlania nastawy CO
    DtCWf_newPCW = 0; //deaktywacja wyswietlania nastawy CW
    RefInterfaceData(); //aktualizacja danych na potrzeby interfejsu uzytkownika
    //deaktywacja pobudzen
    _VNT = 0; //wentylator na stale wylaczony
    _GRZEJ = 0;

    //decyzja czy kociol jest typu TURBO
    Mf_TURBO = !RdPrt(S_CIG_NC) || RdPrt(S_CIG_NO);
    RestartFltPrt();
    StartRTS(_RTSTR); //start timera rozruchu sterownika
    //------------------
    //------------------
    //------------------
    DtCW.PCW = 65;
    DtCO.PCO = 85;
    //------------------
    //------------------
    WritePWM2(0);
    while(1) //petla glowna programu
    {
        CLRWDT();
        PFNf_opgprg = 1;
        PFNf_opgres = 0;

        //BitSet2(&RSDT1.outSTAT0,_PGPRG,1);		//znacznik petli programu
        //BitSet2(&RSDT1.outSTAT0,_PGRES,0);
        Mf_PRACA = Mf_ROZRUCH || _GRZEJ;

        //----------------
        RefInterfaceData(); //aktualizacja danych na potrzeby interfejsu uzytkownika
        TrybPracy();
        RefInterfaceData(); //aktualizacja danych na potrzeby interfejsu uzytkownika
        MKonfiguracja();		//sterowanie znacznikiem mozliwosci wejscia do proc. konfiguracji
        //----------------
        PomiarPCO(); //wartosc nastawy CO
        PomiarPCW(); //wartosc nastawy CW
        PomiarPFN(); //stan przelacznika funkcji
        PomiarCO(); //wartosc temperatury CO
        PomiarCW(); //wartosc temperatury CW
        PomiarZW(); //temperatura z czujnika zewnetrznego
        FiltrPomiarowyCOCW(); //filtr danych opisujacych stan temperatur
        PomiarCS(); //wartosc cisnienia w instalacji CO
        PomiarIN(); //wartosc temperatury na powrocie
        //----------------
        DecWSW(); //wyswietlane parametry pracy
        DecERR(); //stany awaryjne, wyswietlane kody awaryjne
        DecSTR(); //dodatkowe decyzje sterowania
        DecRozruch(); //decyzja o zapaleniu palnika
        DecWylacz(); //decyzja o wygaszeniu palnika
        FunkcjaSerwisowa(); //decyzja o realizacji funkcji serwisowej
        DecyzjaAntyLeg(); //decyzja o realizacji funkcji ANTYLEGIONELLA
        KolejkaPobudzen(); //aktywacja pobudzen wg priorytetu

        //----------------
        RglPmp(); //dla pompy PWM reguluj predkosc obrotowa
        if(Mf_ZMKCOCW) ZmienKCOKCW(); //zmiana kierunku obrotu pompy
        if(Mf_WYLACZ) Wylacz(); //kontrola wybiegu pompy
        if(Mf_ROZRUCH) Rozruch(); //rozruch kotla
        if(Mf_VWYBIEG) VWybieg(); //wybieg wentylatora
        ReloadPlomien(); //decyzja o wyzerowaniu licznika prob przy braku plomienia
        if(Mf_RUSZ_PLOMIEN) RuszPlomien(); //awaryjna procedura przy braku plomienia
        ReloadCiag(); //zerowanie licznika ponowien prob ciagu
        if(Mf_RUSZ_CIAG) RuszCiag(); //awaryjna procedura przy braku ciagu
        Proba48(); //zegar 48h
        if(Mf_RUSZ_48) Rusz48(); //wybieg pompy co 48h
        Proba48Z3(); //zegar 48h dla Z3Dr
        if(Mf_RUSZ_48Z3) Rusz48Z3(); //wybieg pompy co 48h w obiegu WU
        if(Mf_BLOKUJ_3M) BlokadaL3(); //blokada 3 minuty
        if(Mf_BLOKUJ_15S)
        {
            if(!Mf_PRACA) Blokada15S(); //blokada 15 sekund
        }
        if(Mf_MODULUJ) Moduluj(); //algorytm modulacji
        if(DtKNF.robg == _OTWARTY) //uklad z obiegiem otwartym?
        {
            DetekcjaPrzeplywuWG(); //kontrola nieprawidlowosci przeplywu wody grzewczej
            if(Mf_RUSZ_POMPE) OczekujNaPrzeplywWG(); //oczekiwanie na powrot czujnika do prawidlowej pozycji
        }
    }
}
//------------------
//------------------
//------------------
//Wyzerowanie podstawowych danych sterujacych

void ClearData(void)
{
    DtKNFf_enknf = 0;
    Mf_pob_KCO = 0;
    Mf_pob_KCW = 0;
    Mf_pob_AL = 0;
    Mf_pob_SRW = 0;
    Mf_pob_CO = 0;
    Mf_pob_CW = 0;
    Mf_pob_DzCO = 0;
    Mf_pob_DzCW = 0;

    Mf_WYLACZ = 0;
    Mf_ZMKCOCW = 0;
    Mf_ROZRUCH = 0;
    Mf_RUSZ_48 = 0;
    Mf_RUSZ_48Z3 = 0;
    Mf_RUSZ_PWR = 0;
    Mf_VWYBIEG = 0;
    Mf_MODULUJ = 0;
    Mf_RUSZ_CIAG = 0;
    Mf_RUSZ_PLOMIEN = 0;
    Mf_RUSZ_RPLM = 0;
    Mf_RUSZ_POMPE = 0;
    Mf_BLOKUJ_3M = 0;
    Mf_BLOKUJ_15S = 0;
    Mf_MODULUJ = 0;

    Mf_AKTYWNE_AL = 0;
    Mf_AKTYWNE_SRW = 0;
    Mf_AKTYWNE_PGD = 0;
    Mf_AKTYWNE_KNF = 0;
    Mf_RUSZ_NTC_CO = 0;
    Mf_RUSZ_NTC_CW = 0;
    Mf_RUSZ_STB = 0;
    Mf_RUSZ_MOD = 0;
    Mf_RUSZ_CISN = 0;
    Mf_RUSZ_NCISN = 0;

    Mf_PW_START = 0;
    Mf_RS_START = 0;
    Mf_TURBO = 0;
    Mf_PRACA = 0;
    Mf_OST_PL = 0;
    Mf_RUN_MNMOD = 0;
    Mf_KCW_PMP = 0;
    Mf_DE_VNT = 0;
    Mf_DE_PMP = 0;
    Mf_RPM_CW = 0;
    Mf_RUSZ_NTC_IN = 0;

    M.ERR_BTY = 0;
    M.ROZ = 0;
    M.R48 = 0;
    M.R48Z3 = 0;
    M.BL3 = 0;
    M.BL15 = 0;
    M.WYL = 0;
    M.RPR = 0;
    M.RPL = 0;
    M.RCI = 0;
    M.MNM = 0;
    M.ZKP = 0;
    M.VWB = 0;
    M.SRV = 0;
    M.KRS = 0;
    M.HVZ = 0;
    M.LICZNIK_PL = 0;
    M.LICZNIK_CIG = 0;
    M.LICZNIK_CIG2 = 0;
    M.LICZNIK_CIG3 = 0;
    M.DPR = 0;

    _VNT = 0;
    _GRZEJ = 0;
    _POMPA = 0;
}
//------------------

void RestartFltPrt(void)
{
    SetPrt(S_ON_PMP, 0);
    SetPrt(S_ON_PMP_CW, 0);
    SetPrt(S_DZ_CO, 0);
    SetPrt(S_DZ_CW, 0);
    SetPrt(S_RT, 0);
    SetPrt(S_WCW, 0);
    SetPrt(S_HVZ, 0);
}
//------------------
//------------------

unsigned char KodSterownika(void)
{
    //Kod sterownika
    switch(DtKNF.tpkt) //dla danego rodzaju kotla
    {
        case _UNICO: //unico - monotermiczny
            if(!Mf_TURBO) return 0x10;
            else return 0x11;
        case _BITERM: //bitermiczny (miniterm)
            if(!Mf_TURBO) return 0x12;
            else return 0x13;
        case _BITERM2: //bitermiczny (maksiterm)
            if(!Mf_TURBO) return 0x14;
            else return 0x15;
        case _MONOHB: //monotermiczny z hydroblokiem
            if(!Mf_TURBO) return 0x16;
            else return 0x17;
        default: //domyslny monotermiczny z hydroblokiem
            if(!Mf_TURBO) return 0x18;
            else return 0x19;
    }
}


//------------------
//------------------
//Aktualizacja danych do wysylki

void RefInterfaceData(void)
{
    PFNf_EnAleg = (DtKNF.tpkt == _UNICO);
    PFNf_plmCO = Mf_PRACA && !Mf_RPM_CW && RdPrt(S_PLM);
    PFNf_plmCW = Mf_PRACA && Mf_RPM_CW && RdPrt(S_PLM);
    //PFNf_ofnserw = Mf_AKTYWNE_SRW;
    //PFNf_oaleg = Mf_AKTYWNE_AL;
    PFNf_opKNF = Mf_AKTYWNE_KNF;

    PFNf_tdzCO = PFNf_plmCO && Mf_pob_DzCO;
    PFNf_tdzCW = PFNf_plmCW && Mf_pob_DzCW;


    PFNf_pgd = Mf_AKTYWNE_PGD;
    PFNf_obgCO = Mf_PRACA && !Mf_RPM_CW;
    PFNf_obgCW = Mf_PRACA && Mf_RPM_CW;

    PFNf_errCS = DtCSf_error;
    PFNf_errCO = DtCOf_err_CO;
    PFNf_errCW = DtCWf_err_CW;
    PFNf_fnoCS = DtCSf_no_mont;

    PFNf_ofzima = PFNf_zima;
    PFNf_oflato = PFNf_lato;
    PFNf_ofoff = PFNf_off;

    PFNf_errIN = DtINf_err_IN;
    PFNf_pmpPWM = (DtKNF.rdpm != 0) ? 1 : 0;
    PFNf_trbECO = (DtKNF.teco != 0) ? 1 : 0;

    PFNf_oaleg=PFNf_aleg&&Mf_AKTYWNE_AL;
    PFNf_ofnserw=PFNf_fnserw&&Mf_AKTYWNE_SRW;
     
    if(!PFNf_set_fnserw && !PFNf_ofnserw) PFNf_prnserw=0;
    else if(PFNf_set_fnserw || PFNf_ofnserw) PFNf_prnserw=1;

    if(!PFNf_set_aleg && !PFNf_oaleg) PFNf_prnaleg=0;
    else if(PFNf_set_aleg || PFNf_oaleg) PFNf_prnaleg=1;
}
//------------------
//------------------
//Wybor aktywnego pobudzenia ze wzgledu na miejsce w kolejce priorytetow i pozycje przelacznika funkcyjnego

void KolejkaPobudzen(void)
{
    unsigned char wr = 0;

    //grzanie w proc. konfiguracji
    if(Mf_AKTYWNE_KNF)
    {
        Mf_pob_SRW = 0; //zerowanie pobudzen o nizszym priorytecie
        Mf_pob_AL = 0;
        Mf_pob_CO = 0;
        Mf_pob_CW = 0;
        Mf_pob_DzCO = 0;
        Mf_pob_DzCW = 0;
        if((PFN.poz == _PFLTO) || (PFN.poz == _PFZMA))
        {
            if(DtKNFf_wymCW) Mf_pob_KCW = 1; //aktywacja pobudzenia
            else Mf_pob_KCW = 0;
            if(DtKNFf_wymCO)
            {
                Mf_pob_KCO = 1; //aktywacja pobudzenia
                Mf_BLOKUJ_15S = 0;
            }
            else Mf_pob_KCO = 0;
        }
        else
        {
            Mf_pob_KCW = 0;
            Mf_pob_KCO = 0;
        }
    }
    else
    {
        //grzanie w proc. serwisowej
        Mf_pob_KCW = 0;
        Mf_pob_KCO = 0;
        if(Mf_AKTYWNE_SRW && (PFN.poz == _PFZMA))
        {
            Mf_pob_SRW = 1; //aktywacja pobudzenia
            Mf_pob_AL = 0;
            Mf_pob_CW = 0; //zerowanie pobudzen o nizszym priorytecie
            Mf_pob_CO = 0;
            Mf_pob_DzCO = 0;
            Mf_pob_DzCW = 0;
            Mf_BLOKUJ_15S = 0;
        }
        else
        {
            //grzanie w funkcji ANTYLEGIONELLA
            Mf_pob_SRW = 0;
            if(Mf_AKTYWNE_AL)
            {
                if(_AL_PCW > DtCW.CW) Mf_pob_AL = 1; //zalacz wymuszenie grzania
                else
                {
                    PFNf_aleg=0;        //usun znacznik konsoli
                    Mf_pob_AL = 0;      //wylacz wymuszenie grzania
                    Mf_AKTYWNE_AL = 0;  //deaktywuj funkcje ANTYLEGIONELLA
                }
                Mf_pob_CW = 0; //zerowanie pobudzen o nizszym priorytecie
                Mf_pob_CO = 0;
                Mf_pob_DzCO = 0;
                Mf_pob_DzCW = 0;
            }
            else
            {
                //grzanie od pobudzenia CW
                Mf_pob_AL = 0;
                if(DtKNF.tpkt != _UNICO) wr = RdPrt(S_WCW) && ((PFN.poz == _PFLTO) || (PFN.poz == _PFZMA));
                else wr = DtCWf_wpz_CW && DtCOf_pz_CO && ((PFN.poz == _PFLTO) || (PFN.poz == _PFZMA)); //DtCWf_wpz_CW uzaleznione jest od stanu RdPrt(S_WCW)
                if(wr != 0)
                {
                    Mf_pob_CW = 1; //aktywacja pobudzenia
                    Mf_pob_CO = 0; //zerowanie pobudzen o nizszym priorytecie
                    Mf_pob_DzCO = 0;
                    Mf_pob_DzCW = 0;
                }
                else
                {
                    //grzanie od pobudzenia RT
                    Mf_pob_CW = 0;
                    if(RdPrt(S_RT) && (PFN.poz == _PFZMA))
                    {
                        Mf_pob_CO = 1; //aktywacja pobudzenia
                        Mf_pob_DzCO = 0; //zerowanie pobudzen o nizszym priorytecie
                        Mf_pob_DzCW = 0;
                    }
                    else
                    {
                        //grzanie od pobudzenia temp. dyz. CO
                        Mf_pob_CO = 0;
                        if(RdPrt(S_DZ_CO))
                        {
                            Mf_pob_DzCO = 1; //aktywacja pobudzenia
                            Mf_pob_DzCW = 0; //zerowanie pobudzen o nizszym priorytecie
                        }
                        else
                        {
                            Mf_pob_DzCO = 0;
                            if(DtKNF.tpkt == _UNICO) //opcja tylko dla kotla z zasobnikiem
                            {
                                //grzanie od pobudzenia temp. dyz. CW
                                if(RdPrt(S_DZ_CW)) Mf_pob_DzCW = 1; //aktywacja pobudzenia
                                else Mf_pob_DzCW = 0;
                            }
                            else Mf_pob_DzCW = 0;
                        }
                    }
                }
            }
        }
    }
}
//------------------
//------------------
//Decyzja o realizacji funkcji ANTYLEGIONELLA

void DecyzjaAntyLeg(void)
{
    //static unsigned char kanl;
#if _SRVANL==1										//aktywny tryb kontrolny antylegionella?
    if(DtKNF.tpkt == _UNICO) //obowiazuje tylko dla kotlow z obsluga zasobnika
    {
        //--------
        //Warunki przerwania realizacji funkcji ANTYLEGIONELLA
        if(Mf_AKTYWNE_AL)
        {
            if(DtKNF.aleg)
            {
                //StartRTG(_RTANL);						//zatrzymanie zegara ponawiania funkcji ANTYLEGIONELLA
                StartRTM(_SRVRTANL);
                M.LICZNIK_ANL = 0;
            }
            if(Mf_AKTYWNE_KNF || Mf_AKTYWNE_SRW)
            {
                PFNf_aleg=0;
                Mf_AKTYWNE_AL = 0;
                return;
            }
            //niedozwolona pozycja przelacznika funkcyjnego
            if(!((PFN.poz == _PFLTO) || (PFN.poz == _PFZMA)))
            {
                PFNf_aleg=0;
                Mf_AKTYWNE_AL = 0;
                return;
            }
            //maksymalny dozwolony czas aktywnosci funkcji ANTYLEGIONELLA
            if(RTM(_RTMAL) >= _SRVTEXTAL)
            {
                PFNf_aleg=0;                
                Mf_AKTYWNE_AL = 0;
                return;
            }
            //deaktywowano procedure (np nacisnieto przycisk RESET)
            if(!PFNf_aleg)//PFNf_res)
            {
                Mf_AKTYWNE_AL = 0;
                return;
            }
        }
        //--------
        //Warunki aktywacji funkcji ANTYLEGIONELLA
        if(!Mf_AKTYWNE_AL) //mozliwa realizacja funkcji ANTYLEGIONELLA?
        {
            if(Mf_AKTYWNE_KNF || Mf_AKTYWNE_SRW) //realizowana funkcja serwisowa lub konfiguracyjna?
            {
                return;
            }
            //-------
            //zegar ponawiania funkcji ANTYLEGIONELLA
            if(DtKNF.aleg)
            {
                //if(RTG(_RTANL)>=1)							//timer godzinowy wyzwalania funkcji ANTYLEGIONELLA
                if(RTM(_SRVRTANL) >= 1) //timer godzinowy wyzwalania funkcji ANTYLEGIONELLA
                {
                    //StartRTG(_RTANL);
                    StartRTM(_SRVRTANL);
                    M.LICZNIK_ANL++;
                    if(M.LICZNIK_ANL >= _NGODZIN_SRVAL) //licznik godzin do wyzwolenia funkcji ANTYLEGIONELLA
                    {
                        M.LICZNIK_ANL = 0;
                        if((PFN.poz == _PFLTO) || (PFN.poz == _PFZMA)) {Mf_AKTYWNE_AL = 1;PFNf_aleg=1;} //aktywacja funkcji ANTYLEGIONELLA po czasie odliczonym przez timer.
                        StartRTM(_RTMAL); //start timera maksymalnego dozwolonego czasu aktywnosci funkcji ANTYLEGIONELLA
                        eeprom_write(_ADRRTA, 0);
                        return;
                    }
                    else
                    {
                        eeprom_write(_ADRRTA, M.LICZNIK_ANL);
                    }
                }
            }
            //-------
            if(!((PFN.poz == _PFLTO) || (PFN.poz == _PFZMA))) //niedozwolona pozycja przelacznika funkcyjnego?
            {
                return;
            }
            //--------
            //wymuszenie grzania AL z konsoli
            if(PFNf_aleg)// && !PFNf_oaleg) //aktywowano recznie funkcje ANTYLEGIONELLA?
            {
                M.LICZNIK_ANL = 0;
                Mf_AKTYWNE_AL = 1; //aktywacja funkcji ANTYLEGIONELLA
                StartRTM(_RTMAL); //start timera maksymalnego dozwolonego czasu aktywnosci funkcji ANTYLEGIONELLA
                eeprom_write(_ADRRTA, 0);
            }           
        }
        else
        {
            //StartRTG(_RTANL);						//zatrzymanie zegara (funkcja ANTYLEGIONELLA nieaktywna)
            StartRTM(_SRVRTANL);
            M.LICZNIK_ANL = 0;
        }
    }
    else {Mf_AKTYWNE_AL = 0;PFNf_aleg=0;}
#else
    if(DtKNF.tpkt == _UNICO) //obowiazuje tylko dla kotlow z obsluga zasobnika
    {
        //--------
        //Warunki przerwania realizacji funkcji ANTYLEGIONELLA
        if(Mf_AKTYWNE_AL)
        {
            if(DtKNF.aleg)
            {
                StartRTG(_RTANL); //zatrzymanie zegara ponawiania funkcji ANTYLEGIONELLA
                M.LICZNIK_ANL = 0;
            }
            if(Mf_AKTYWNE_KNF || Mf_AKTYWNE_SRW)
            {
                PFNf_aleg=0;
                Mf_AKTYWNE_AL = 0;
                return;
            }
            //niedozwolona pozycja przelacznika funkcyjnego
            if(!((PFN.poz == _PFLTO) || (PFN.poz == _PFZMA)))
            {
                PFNf_aleg=0;
                Mf_AKTYWNE_AL = 0;
                return;
            }
            //maksymalny dozwolony czas aktywnosci funkcji ANTYLEGIONELLA
            if(RTM(_RTMAL) >= _TEXTAL)
            {
                PFNf_aleg=0;
                Mf_AKTYWNE_AL = 0;
                return;
            }
            //deaktywowano procedure (np nacisnieto przycisk RESET)
            if(!PFNf_aleg)
            {
                Mf_AKTYWNE_AL = 0;
                return;
            }
        }
        //--------
        //Warunki aktywacji funkcji ANTYLEGIONELLA
        if(!Mf_AKTYWNE_AL) //mozliwa realizacja funkcji ANTYLEGIONELLA?
        {
            if(Mf_AKTYWNE_KNF || Mf_AKTYWNE_SRW) //realizowana funkcja serwisowa lub konfiguracyjna?
            {
                return;
            }
            //-------
            //zegar ponawiania funkcji ANTYLEGIONELLA
            if(DtKNF.aleg)
            {
                if(RTG(_RTANL) >= 1) //timer godzinowy wyzwalania funkcji ANTYLEGIONELLA
                {
                    StartRTG(_RTANL);
                    M.LICZNIK_ANL++;
                    if(M.LICZNIK_ANL >= _NGODZIN_AL) //licznik godzin do wyzwolenia funkcji ANTYLEGIONELLA
                    {
                        M.LICZNIK_ANL = 0;
                        if((PFN.poz == _PFLTO) || (PFN.poz == _PFZMA)) {Mf_AKTYWNE_AL = 1;PFNf_aleg=1;} //aktywacja funkcji ANTYLEGIONELLA po czasie odliczonym przez timer.
                        StartRTM(_RTMAL); //start timera maksymalnego dozwolonego czasu aktywnosci funkcji ANTYLEGIONELLA
                        eeprom_write(_ADRRTA, 0);
                        return;
                    }
                    else
                    {
                        eeprom_write(_ADRRTA, M.LICZNIK_ANL);
                    }
                }
            }
            //-------
            if(!((PFN.poz == _PFLTO) || (PFN.poz == _PFZMA))) //niedozwolona pozycja przelacznika funkcyjnego?
            {
                return;
            }
            //--------
            //wymuszenie grzania AL z konsoli
            if(PFNf_aleg) //aktywowano recznie funkcje ANTYLEGIONELLA?
            {
                M.LICZNIK_ANL = 0;
                Mf_AKTYWNE_AL = 1; //aktywacja funkcji ANTYLEGIONELLA
                StartRTM(_RTMAL); //start timera maksymalnego dozwolonego czasu aktywnosci funkcji ANTYLEGIONELLA
                eeprom_write(_ADRRTA, 0);
            }
        }
        else
        {
            StartRTG(_RTANL); //zatrzymanie zegara (funkcja ANTYLEGIONELLA nieaktywna)
            M.LICZNIK_ANL = 0;
        }
    }
    else {Mf_AKTYWNE_AL = 0;PFNf_aleg=0;}
#endif
}
//------------------
//------------------
//Regulacja stanu wyswietlacza 
//void NastawaCOCW(unsigned char one);
/*
void DecWSW(void)
{
        //NastawaCOCW(1);
        //PrintHex(PFN.AC);

        //stan plomienia
        SetDiode(_DPL,RdPrt(S_PLM)&&Mf_PRACA);
        //funkcja serwisowa
        SetDiode(_DKM,Mf_AKTYWNE_SRW);
        //sygnalizacja obiegu
        SetDiode(_DCO,Mf_PRACA&&!Mf_RPM_CW);
        SetDiode(_DCW,Mf_PRACA&&Mf_RPM_CW);

#if _SRVTRB==1						//aktywny tryb serwisowy kompilacji?
        if(Mf_pob_SRW&&!Mf_ROZRUCH&&_GRZEJ)
        {
                PrintHex(PWM.BufPWM);
                return;
        }
#endif

        if(!DtCSf_no_mont)		//czujnik cisnienia zamontowany
        {
                //przelacznik funkcji na pozycji CISNIENIE
                if((PFN.poz==_PFCSN)&&!RdPrt(S_ERR_CSN)) PrintCS(DtCS.CS);
                else EndCS();

                //przelacznik funkcji na dowolnej pozycji oprocz pozycji CISNIENIE gdy nie jest realizowana funkcja grzania
                if((PFN.poz!=_PFCSN)&&!Mf_PRACA) PrintOFF();
                else
                //przelacznik funkcji na pozycji CISNIENIE gdy jest awaria czujnika cisnienia
                if((PFN.poz==_PFCSN)&&RdPrt(S_ERR_CSN)) PrintOFF();
                else EndOFF();
        }
        else					//czujnik cisnienia nie zamontowany
        {
                //przelacznik funkcji na dowolnej pozycji oprocz pozycji CISNIENIE gdy nie jest realizowana funkcja grzania
                if(!Mf_PRACA) PrintOFF();
                else EndOFF();
        }

        //nastawa PCO
        if(DtCOf_newPCO)
        {
                if(Mf_AKTYWNE_PGD) PrintNstKt(DtCO.PCO);
                else PrintNstCO(DtCO.PCO);
        }
        else
        //nastawa PCW
        if(DtCWf_newPCW) PrintNstCW(DtCW.PCW);

        //przelacznik funkcji na pozycji LATO lub ZIMA
        if((PFN.poz==_PFLTO)&&!DtCWf_err_CW) PrintCW(DtCW.SCW);							//pozycja LATO
        else
        if((PFN.poz==_PFZMA)&&!DtCWf_err_CW&&Mf_pob_CW&&Mf_PRACA) PrintCW(DtCW.SCW);	//pozycja ZIMA i wymuszenie CW
        else
        {
                EndCW();
                PrintCO(DtCO.SCO);
        }

        //temperatura dyzurna
        if(!DtCSf_no_mont)		//czujnik cisnienia zamontowany
        {
                if(PFN.poz!=_PFCSN)			//przelacznik funkcji na pozycji innej niz CISNIENIE
                {
                        if(Mf_pob_DzCO&&Mf_PRACA) PrintTdzCO(DtCO.SCO);
                        else
                        {
                                EndTdzCO();
                                if(Mf_pob_DzCW&&Mf_PRACA) PrintTdzCW(DtCW.SCW);
                                else EndTdzCW();
                        }
                }
                else
                {
                        EndTdzCO();
                        EndTdzCW();
                }
        }
        else					//czujnik cisnienia nie zamontowany
        {
                if(Mf_pob_DzCO&&Mf_PRACA) PrintTdzCO(DtCO.SCO);
                else
                {
                        EndTdzCO();
                        if(Mf_pob_DzCW&&Mf_PRACA) PrintTdzCW(DtCW.SCW);
                        else EndTdzCW();
                }
        }
}
 */
//------------------
//------------------

void NastawaCOCW(void)
{
    volatile static unsigned char obg_cw = 0, obg_co = 0, zmn2 = 0, zmn3 = 0, pcw;

    //-----------------
    //if(PFNf_fnserw || PFNf_ofnserw || PFNf_set_fnserw || PFNf_aleg || PFNf_oaleg || PFNf_set_aleg) //aktywna funkcja serwisowa lub antylegionella?
    if(PFNf_prnserw || PFNf_prnaleg)
    {
        PFNf_set_ncw = 0;
        PFNf_set_nco = 0;
        PFNf_str_nco = 0;
    }
    //-----------------
    if(!PFNf_prnserw)
    {
        if(!PFNf_set_ncw && !obg_cw && PFNf_obgCW) //uruchomienie w obiegu CW?
        {
            PFNf_str_ncw = 1;
            PFNf_b_str_ncw = 1;
            StartRTdS(_RTSTR2);
        }
        obg_cw = PFNf_obgCW;
        if(PFNf_b_str_ncw)
        {
            PFNf_b_str_ncw = 0;
            EndNst();
            PrintNstCW(PFN.PPCW);
        }
    }
    //-----------------
    if(!PFNf_set_nco && !obg_co && PFNf_obgCO) //uruchomienie w obiegu CO?
    {
        PFNf_str_nco = 1;
        PFNf_b_str_nco = 1;
        StartRTdS(_RTSTR2);
    }
    obg_co = PFNf_obgCO;
    if(PFNf_b_str_nco)
    {
        PFNf_b_str_nco = 0;
        EndNst();
        if(PFNf_prnserw)
        {
            PFNf_set_nco = 0;
            PFNf_str_nco = 0;
            EndNst();
        }
        else PrintNstCO(PFN.PPCO);
    }
    //-----------------
    if(PFNf_set_nco || PFNf_set_ncw || RTdS(_RTSTR2) > _TSTRNST) //uplynal czas podtrzymania nastawy po rozruchu?
    {
        PFNf_str_nco = 0;
        PFNf_str_ncw = 0;
    }
    //-----------------
    if(PFNf_set_ncw) //wybrano nastawe CW?
    {
        pcw = PFN.PCW;
        if(!PFNf_mod_ncw)
        {
            if(!RdPrt(S_KUP)&&!RdPrt(S_KDW)) PFNf_mod_ncw=1;
            if(RTdS(_RTSET)<5) PrintNstCW(PFN.PCW);
            else PFNf_mod_ncw=1;
        }
        if(PFNf_mod_ncw) ReadParam(&PFN.PCW, &zmn, &zps, DtCW.minPCW, DtCW.maxPCW, 0, 1);
        if(!PFNf_b_set_ncw) //nacisnieto przycisk set?
        {
            PFNf_b_set_ncw = 1;
            EndNst();
            zmn2 = 1;
            pcw = PFN.PCW;
        }
        if(zmn2 || zmn) //nastawa ulegla zmianie?
        {
            PFNf_newPCW = (pcw != PFN.PCW);
            zmn2 = 0;
            StartRTdS(_RTSET);
            PrintNstCW(PFN.PCW);
        }
    }
    //-----------------
    if(PFNf_set_nco) //wybrano nastawe CO?
    {
        pcw = PFN.PCO;
        ReadParam(&PFN.PCO, &zmn, &zps, DtCO.minPCO, DtCO.maxPCO, 0, 1);
        if(!PFNf_b_set_nco) //stan po nacisnieciu przycisku set?
        {
            PFNf_b_set_nco = 1;
            EndNst();
            zmn2 = 1;
            pcw = PFN.PCO;
            PFNf_newPCO = 0;
        }
        if(zmn2 || zmn) //nastawa ulegla zmianie?
        {
            if(!PFNf_newPCO) PFNf_newPCO = (pcw != PFN.PCO);
            zmn2 = 0;
            StartRTdS(_RTSET);
            if(Mf_AKTYWNE_PGD) PrintNstKt(PFN.PCO);
            else PrintNstCO(PFN.PCO);
        }
    }
    //-----------------
    if(!PFNf_set_ncw && !PFNf_set_nco) //tryb modyfikacji nastawy jest nieaktywny?
    {
        if(!PFNf_str_ncw && !PFNf_str_nco) EndNst();
        PFNf_b_set_ncw = 0;
        PFNf_b_set_nco = 0;
    }
}

//--------------------------------------------------------------
#define _KRSK 0

void DecWSW(void)
{
    //Komunikat bledu
    if(PFNf_ferr || PFNf_fmsg)
    {
        NastawaCOCW();
        if(PFNf_ferr)
        {
            if(!PFNf_KnfDisErr || PFNf_opgres)
            {
                if(PFNf_opgres) PrintErr(PFN.KERR, 1); //migajacy kod awaryjny w blokadzie trwalej
                else
                {
                    if(PFN.KERR != 7 && PFN.KERR != 9 && PFN.KERR != 0x14) PrintErr(PFN.KERR, 1); //migajacy kod awaryjny w petli glownej
                    else
                    {
                        if(PFN.KERR == 7 || PFN.KERR == 0x14)
                        {
                            if(PFNf_obgCW)
                            {
                                if(!PFNf_errCW) PrintErrTm(PFN.KERR, PFN.CW, 1, 1);
                                else PrintErr(PFN.KERR, 1);
                            }
                            else
                            {
                                if(!PFNf_errCO) PrintErrTm(PFN.KERR, PFN.CO, 1, 0);
                                else PrintErr(PFN.KERR, 1);   
                            }
                        }
                        else
                            if(PFN.KERR == 9)
                        {
                            PrintErrCs(PFN.KERR, PFN.CS, 1);
                        }
                    }
                }
            }
            else
            {
                EndErr();
            }
        }
        else
        {
            if(!PFNf_KnfDisErr)
            {
                PrintErr(PFN.KAWR, 0); //staly kod awaryjny
            }
            else EndErr();
        }
    }
    else
    {
        EndErr();
    }
    //Normalna praca urzadzenia
    if(!PFNf_opKNF && !PFNf_ferr && !PFNf_fmsg)
    {
        if(PFNf_ofoff) //pozycja wylacz
        {
            //decyzje o wygaszeniu
            if((PFNf_errCS || PFNf_fnoCS) && !PFNf_tdzCO && !PFNf_tdzCW)
            {
                PrintOFF(); //dwie kreski
            }
            else
            {
                if(!PFNf_errCS && !PFNf_fnoCS && !PFNf_tdzCO && !PFNf_tdzCW)
                {
                    EndOFF();
                    PrintCS(PFN.CS); //domyslny wskaz cisnienia
                }
                else
                    if(PFNf_tdzCW && !PFNf_errCW)
                {
                    EndOFF();
                    EndCS();
                    PrintCW(PFN.CW); //temp. dyzurna CW
                }
                else
                {

                    if(PFNf_tdzCO && !PFNf_errCO)
                    {
                        EndOFF();
                        EndCS();
                        EndCW();
                        PrintCO(PFN.CO); //temp. dyzurna CO
                    }
                    else
                    {
                        PrintOFF();
                    }
                }
            }
        }
        else
        {
            //-----------------------------------
            //tryb LATO
            if(PFNf_oflato)
            {
                if(!PFNf_prnaleg)
                //if(!PFNf_aleg && !PFNf_oaleg && !PFNf_set_aleg)
                {
                    if(PFNf_obgCO || PFNf_tdzCO) //temp. dyzurna CO?
                    {
                        if(PFNf_prn_res)
                        {
                            if(PFNf_prn_cs) //czasowy wskaz cisnienia
                            {
                                EndOFF();
                                PrintCS(PFN.CS); //wylaczny wskaz CS
                            }
							else
							{
								if(PFNf_prn_in) 
								{
                                    EndOFF();
                                    EndCS();
                                    EndCW();                                    
									PrintCO(PFN.IN);							//czasowy wskaz IN
								}
                                else EndCS();
							}                            
                        }
                        else
                        {
                            if(!PFNf_errCO)
                            {
                                EndOFF();
                                EndCS();
                                EndCW();
                                PrintCO(PFN.CO); //wylaczny wskaz temperatury CO
                            }
                            else
                            {
                                PrintOFF();
                            }
                        }
                    }
                    else //wskaz/praca CW
                    {
                        if(PFNf_prn_res)
                        {
                            if(PFNf_prn_cs) //czasowy wskaz cisnienia
                            {
                                EndOFF();
                                PrintCS(PFN.CS); //wylaczny wskaz temp CW
                            }
							else
							{
								if(PFNf_prn_in) 
								{
                                    EndOFF();
                                    EndCS();
                                    EndCW(); 
									PrintCO(PFN.IN);							//czasowy wskaz IN
								}
                                else EndCS();
							}                            
                        }
                        else //tylko wskaz temp. CW przy pobudzeniu
                        {
                            if(!PFNf_errCW && PFNf_obgCW)
                            {
                                EndOFF();
                                EndCS();
                                PrintCW(PFN.CW); //temp. dyzurna CW
                            }
                            else
                            {
                                PrintOFF(); //dwie kreski
                            }
                        }
                    }
                }
                else
                {
                        EndOFF();
                        EndCS();
                        PrintAL();
                }
            }
            else
            //-----------------------------------
            //tryb ZIMA
            if(PFNf_ofzima)
            {
                if(!PFNf_prnserw && !PFNf_prnaleg)
                //if(!PFNf_ofnserw && !PFNf_oaleg && !PFNf_set_fnserw && !PFNf_set_aleg)
                {
                    if(!PFNf_obgCO && !PFNf_obgCW) //oba obiegi nieaktywne?
                    {
                        if(PFNf_prn_res)
                        {
                            if(PFNf_prn_cs)
                            {
                                EndOFF();
                                PrintCS(PFN.CS); //wylaczny wskaz CS
                            }
							else
							{
								if(PFNf_prn_in) 
								{
                                    EndOFF();
                                    EndCS();
                                    EndCW(); 
									PrintCO(PFN.IN);							//czasowy wskaz IN
								}
                                else EndCS();
                            }
                            //else EndCS();
                            //else
                            //if(PFNf_prn_in) PrintIN(PFN.IN);							//wylaczny wskaz IN
                        }
                        else
                        {
                            if(PFNf_fL3) PrintL3();
                            else
                            {
                                PrintOFF();
                            }
                        }
                    }
                    else //dowolny obieg aktywny
                    {
                        if(PFNf_obgCO) //aktywny obieg CO?
                        {
                            if(PFNf_prn_res)
                            {
                                if(PFNf_prn_cs)
                                {
                                    EndOFF();
                                    PrintCS(PFN.CS); //wylaczny wskaz CS
                                }
                                else
                                {
                                    if(PFNf_prn_in) 
                                    {
                                        EndOFF();
                                        EndCS();
                                        EndCW(); 
                                        PrintCO(PFN.IN);							//czasowy wskaz IN
                                    }
                                    else EndCS();
                                }                                
                                //else EndCS();
                                //else
                                //if(PFNf_prn_in) PrintIN(PFN.IN);						//wylaczny wskaz IN
                            }
                            else //wskaz CO
                            {
                                if(!PFNf_errCO)
                                {
                                    EndOFF();
                                    EndCS();
                                    EndCW();
                                    PrintCO(PFN.CO); //wylaczny wskaz temp CO
                                }
                                else PrintOFF();
                            }
                        }
                        else //aktywny obieg CW
                        {
                            if(PFNf_prn_res)
                            {
                                if(PFNf_prn_cs) //aktywny wskaz cisnienia?
                                {
                                    EndOFF();
                                    PrintCS(PFN.CS); //dzielony wskaz CS
                                }
                                else
                                {
                                    if(PFNf_prn_in) 
                                    {
                                        EndOFF();
                                        EndCS();
                                        EndCW(); 
                                        PrintCO(PFN.IN);							//czasowy wskaz IN
                                    }
                                    else EndCS();
                                }
                            }
                            else //wskaz CW
                            {

                                if(!PFNf_errCW)
                                {
                                    EndOFF();
                                    EndCS();
                                    PrintCW(PFN.CW); //wylaczny wskaz temp CW
                                }
                                else PrintOFF();
                            }
                        }
                    }
                }
                else //aktywna funkcja serwisowa lub antylegionella
                {
                    if(PFNf_prnserw) //aktywna funkcja serwisowa?
                    {
                        EndOFF();
                        EndCS();
                        EndCW();
                        PrintSR();
                    }
                    else //aktywna funkcja antylegionella
                    {
                        EndOFF();
                        EndCS();
                        PrintAL();
                    }
                }
            }
            //-----------------------------------
            //nastawa CO/CW
            NastawaCOCW();
            //-----------------------------------
        }
    }
    if(!PFNf_fL3)
    {
        EndL3();
    }
    if(!PFNf_prnserw) EndSR();
    if(!PFNf_prnaleg) EndAL();
    //stan plomienia
    SetDiode(_DPL, PFNf_plmCW || PFNf_plmCO);
    //funkcja serwisowa
    SetDiode(_DKM, PFNf_ofnserw || PFNf_oaleg);
    //sygnalizacja obiegu
    SetDiode(_DCO, PFNf_ofzima && !PFNf_ofoff && !PFNf_opKNF);
    SetDiode(_DCW, (PFNf_ofzima || PFNf_oflato) && !PFNf_ofoff && !PFNf_opKNF);
}

//------------------
//------------------
//Reakcja na wiekszosc stanow awaryjnych

void DecERR(void)
{
    //unsigned char KnfDisErr = 0;
    unsigned char w;

    //Znacznik wylaczajacy wizualizacje kodu bledu podczas pracy w proc. konfiguracji

    //KnfDisErr=(Mf_AKTYWNE_KNF&&(DtKNF.KRK==1||(DtKNF.KRK==2&&(DtKNF.PRM==4||DtKNF.PRM==6||DtKNF.PRM==7))))?1:0;

    //--------BEZWZGLEDNE WYLACZENIE Z BLOKADA
    //UWAGA: natychmiastowe wyswietlenie kodu awaryjnego
    //zadzialalo STB
    if(!RdPrt(S_STB))
    {
        M.ERR_BTY = 0x02;
        //PrintErr(0x02,1);
        SetErr(0x02);
        ErrPTG();
    }
    //--------BEZWZGLEDNE WYLACZENIE BEZ BLOKADY
    if(Mf_RUSZ_PLOMIEN)
    {
        M.ERR_BTY = 0; //wyzeruj wszystkie wczesniejsze kody
        //if(KnfDisErr) EndErr();		//nie wyswietlaj kodu bledu przy konfiguracji par. r4 i r6, r7
        //else PrintErr(0x01,0);
        if(PFNf_KnfDisErr) EndErr(); //nie wyswietlaj kodu bledu przy konfiguracji par. r4 i r6, r7
        else SetMsg(0x01);
        Mf_RUSZ_NTC_CO = 0;
        Mf_RUSZ_CISN = 0;
        Mf_RUSZ_NCISN = 0;
        Mf_RUSZ_NTC_CW = 0;
        Mf_RUSZ_MOD = 0;
    }
    else
        if(Mf_RUSZ_CIAG)
    {
        M.ERR_BTY = 0; //wyzeruj wszystkie wczesniejsze kody
        /*if(KnfDisErr) EndErr();		//nie wyswietlaj kodu bledu przy konfiguracji par. r4 i r6, r7
        else
        {
                if(Mf_prb_CIG)
                {
                        PrintErr(0x03,0);
                }
        }*/
        if(PFNf_KnfDisErr) EndErr(); //nie wyswietlaj kodu bledu przy konfiguracji par. r4 i r6, r7
        else
        {
            if(Mf_prb_CIG) SetMsg(0x03);
        }
        Mf_RUSZ_NTC_CO = 0;
        Mf_RUSZ_CISN = 0;
        Mf_RUSZ_NCISN = 0;
        Mf_RUSZ_NTC_CW = 0;
        Mf_RUSZ_MOD = 0;
    }
    else
        //awaria czujnika CO (wersja dla kotla mono i bitermicznego)
        if((RdPrt(S_ERR_CO) && (DtKNF.tpkt != _BITERM) && (DtKNF.tpkt != _BITERM2)) || (RdPrt(S_ERR_CO) && !Mf_pob_KCW && !Mf_pob_CW && (DtKNF.tpkt == _BITERM || DtKNF.tpkt == _BITERM2)))
    {
        M.ERR_BTY = 0x04;
        //if(KnfDisErr) EndErr();		//nie wyswietlaj kodu bledu przy konfiguracji par. r4 i r6, r7
        //else PrintErr(0x04,1);
        if(PFNf_KnfDisErr) EndErr(); //nie wyswietlaj kodu bledu przy konfiguracji par. r4 i r6, r7
        else SetErr(0x04);
        Mf_RUSZ_NTC_CO = 1;
        Mf_RUSZ_CISN = 0;
        Mf_RUSZ_NCISN = 0;
        Mf_RUSZ_NTC_CW = 0;
        Mf_RUSZ_MOD = 0;
    }
    else
    {
        Mf_RUSZ_NTC_CO = 0;
        //brak przeplywu WG
        if(Mf_RUSZ_POMPE)
        {
            M.ERR_BTY = 0;
            /*if(KnfDisErr) EndErr();		//nie wyswietlaj kodu bledu przy konfiguracji par. r4 i r6, r7
            else
            {
                    if(Mf_prb_PRZ) PrintErr(0x05,0);
            }*/
            if(PFNf_KnfDisErr) EndErr(); //nie wyswietlaj kodu bledu przy konfiguracji par. r4 i r6, r7
            else
            {
                if(Mf_prb_PRZ) SetMsg(0x05);
            }
            Mf_RUSZ_CISN = 0;
            Mf_RUSZ_NCISN = 0;
            Mf_RUSZ_NTC_CW = 0;
            Mf_RUSZ_MOD = 0;
        }
        else
        {
            //awaria obwodu czujnika cisnienia
            if(RdPrt(S_ERR_CSN))
            {
                M.ERR_BTY = 0x08;
                //if(KnfDisErr) EndErr();		//nie wyswietlaj kodu bledu przy konfiguracji par. r4 i r6, r7
                //else PrintErr(0x08,1);
                if(PFNf_KnfDisErr) EndErr(); //nie wyswietlaj kodu bledu przy konfiguracji par. r4 i r6, r7
                else SetErr(0x08);
                Mf_RUSZ_NCISN = 1;
                Mf_RUSZ_CISN = 0;
                Mf_RUSZ_NTC_CW = 0;
                Mf_RUSZ_MOD = 0;
            }
            else
            {
                Mf_RUSZ_NCISN = 0;
                //zbyt niskie lub zbyt wysokie cisnienie w obiegu CO
                if(!RdPrt(S_WRK_CSN))
                {

                    M.ERR_BTY = 0x09;
                    //if(KnfDisErr) EndErr();		//nie wyswietlaj kodu bledu przy konfiguracji par. r4 i r6, r7
                    //else PrintErrCs(0x09,DtCS.CS,1);
                    if(PFNf_KnfDisErr) EndErr(); //nie wyswietlaj kodu bledu przy konfiguracji par. r4 i r6, r7
                    else SetErr(0x09);
                    //SetMsg(0x09);
                    Mf_RUSZ_CISN = 1;
                    Mf_RUSZ_NTC_CW = 0;
                    Mf_RUSZ_MOD = 0;
                }
                else
                {
                    Mf_RUSZ_CISN = 0;
                    if(DtKNF.tpkt != _UNICO)
                    {
                        w = (DtCWf_err_CW && (PFN.poz == _PFLTO)) || ((PFN.poz == _PFZMA) && DtCWf_err_CW && (Mf_pob_CW || Mf_pob_KCW || Mf_pob_AL));
                    }
                    else
                    {
                        w = (DtCWf_err_CW && (PFN.poz == _PFLTO)) || ((PFN.poz == _PFZMA) && DtCWf_err_CW && RdPrt(S_WCW));
                    }
                    //awaria czujnika CW
                    if(w)
                    {
                        M.ERR_BTY = 0x10; //0x10;
                        //if(KnfDisErr) EndErr();		//nie wyswietlaj kodu bledu przy konfiguracji par. r4 i r6, r7
                        //else PrintErr(0x0A,1);
                        if(PFNf_KnfDisErr) EndErr(); //nie wyswietlaj kodu bledu przy konfiguracji par. r4 i r6, r7
                        else SetErr(0x10);
                        Mf_RUSZ_NTC_CW = 1;
                        Mf_RUSZ_MOD = 0;
                    }
                    else
                    {
                        Mf_RUSZ_NTC_CW = 0;
                        //awaria obwodu modulatora
                        if(!RdPrt(S_MOD_ON))
                        {
                            M.ERR_BTY = 0x07;
                            //if(KnfDisErr) EndErr();		//nie wyswietlaj kodu bledu przy konfiguracji par. r4 i r6, r7
                            //else PrintErr(0x07,1);
                            if(PFNf_KnfDisErr) EndErr(); //nie wyswietlaj kodu bledu przy konfiguracji par. r4 i r6, r7
                            else SetErr(0x07);
                            Mf_RUSZ_MOD = 1;
                        }
                        else
                        {
                            Mf_RUSZ_MOD = 0;
                            if(DtKNF.rdpm != 0)
                            {
                                if((PFN.poz == _PFZMA) && (Mf_PRACA && !Mf_RPM_CW) && DtINf_err_IN) //pozycja ZIMA, grzanie w obiegu CO
                                {
                                    M.ERR_BTY = 0x14;
                                    //if(KnfDisErr) EndErr();		//nie wyswietlaj kodu bledu przy konfiguracji par. r4 i r6, r7
                                    //else PrintErr(0x0E,1);
                                    if(PFNf_KnfDisErr) EndErr(); //nie wyswietlaj kodu bledu przy konfiguracji par. r4 i r6, r7
                                    else SetErr(0x14);
                                    Mf_RUSZ_NTC_IN = 1;
                                }
                                else
                                {
                                    Mf_RUSZ_NTC_IN = 0;
                                }
                            }
                            else
                            {
                                Mf_RUSZ_NTC_IN = 0;
                            }
                        }
                    }
                }
            }
        }
    }
    //--------START PROCEDUR NAPRAWCZYCH - WYLACZENIE Z BLOKADA
    //UWAGA: bez wizualizacji kodu awaryjnego (wyswietlany dopiero po wykonaniu procedury naprawczej)

    //brak plomienia (praca kotla)
    if(!Mf_RUSZ_PLOMIEN && !RdPrt(S_PLM) && !Mf_ROZRUCH && _GRZEJ)
    {
        if(Mf_TURBO) //sterownik TURBO?
        {
            if(_VNT) //wentylator pracuje?
            {
                if(RdPrt(S_CIG_NO) && RdPrt(S_STB))
                {
                    M.RPL = 0;
                    Mf_RUSZ_PLOMIEN = 1;
                }
            }
            else //wentylator nie pracuje
            {
                if(RdPrt(S_STB))
                {
                    M.RPL = 0;
                    Mf_RUSZ_PLOMIEN = 1;
                }
            }
        }
        else //sterownik ATMOSFERYCZNY
        {
            if(RdPrt(S_CIG_NC) && RdPrt(S_STB))
            {
                M.RPL = 0;
                Mf_RUSZ_PLOMIEN = 1;
            }
        }
    }

    //brak ciagu (praca kotla)
    if(!Mf_TURBO) //kociol atmosferyczny
    {
        if(!Mf_RUSZ_CIAG && !RdPrt(S_CIG_NC) && RdPrt(S_STB) && Mf_PRACA)
        {
            M.RCI = 0;
            Mf_RUSZ_CIAG = 1;
            /*if(!M.LICZNIK_CIG) StartRTS(_RTRSCI);
            M.LICZNIK_CIG++;
            if(M.LICZNIK_CIG>=_NCYKLI_CIG)
            {
                    M.LICZNIK_CIG=0;
                    M.ERR_BTY=0x03;
                    PrintErr(0x03,1);
                    ErrPTG();
            }*/
        }
    }
    else //kociol turbo (praca wentylatora, pomijana procedura rozruchu)
    {
        if(!Mf_RUSZ_CIAG && !Mf_ROZRUCH && !Mf_DE_VNT && ((!RdPrt(S_CIG_NO) && _VNT) || (RdPrt(S_CIG_NO) && !_VNT)))
        {
            M.RCI = 0;
            Mf_RUSZ_CIAG = 1; //aktywuj procedure awaryjna
            if(!M.LICZNIK_CIG) StartRTS(_RTRSCI); //licznik liczby zanikow ciagu na minute
            M.LICZNIK_CIG++;
            if(M.LICZNIK_CIG >= _NCYKLI_CIG) //przekroczono dozwolona liczbe zanikow ciagu na minute?
            {
                M.LICZNIK_CIG = 0;
                M.ERR_BTY = 0x03;
                SetErr(0x03);//PrintErr(0x03, 1);
                ErrPTG();
            }
        }
    }


    //--------REAKCJA NA BRAK SYTUACJI AWARYJNEJ (DOTYCZY WIZUALIZACJI KODU AWARYJNEGO)

    //usun kod awaryjny z wyswietlacza
    if(!Mf_RUSZ_PLOMIEN && !Mf_RUSZ_CIAG)
        if(!Mf_RUSZ_NTC_CO && !Mf_RUSZ_NCISN && !Mf_RUSZ_CISN)
            if(!Mf_RUSZ_NTC_CW && !Mf_RUSZ_MOD && !Mf_RUSZ_POMPE && !Mf_RUSZ_NTC_IN)
            {
                M.ERR_BTY = 0x00;
                EndErr();
            }
}
//------------------
//------------------
//Decyzje sterujace

void DecSTR(void)
{
    //dla wylaczonego modulu zapalacza zweruj zegar pracy nieciaglej
    //if(!_GRZEJ) StartRTG(_RTPNC);
    //jezeli praca trwa >24h wywolaj reset sterownika
    //if(RTG(_RTPNC)>=_TPNC) while(1);
    //wyzerowanie znacznikow rozruchu sterownika po wlaczeniu zasilania lub resecie
    if(RTS(_RTSTR) >= _TSTR)
    {
        Mf_PW_START = 0;
        Mf_RS_START = 0;
    }
    //regulacja pogodowa aktywna?
    Mf_AKTYWNE_PGD = (RdPrt(S_PGD)) ? 1 : 0;
    //reakcja na zbyt szybki narost temperatury
    if(!Mf_ROZRUCH && _GRZEJ && DtCOf_ldDCO) Mf_RUN_MNMOD = 1;
    else Mf_RUN_MNMOD = 0;
    //--------------
    //wybor obiegu dla pompy (w stanie spoczynku pompa pracuje w obiegu CW)
    if(!Mf_ZMKCOCW)
    {
            //wymuszenie dla obiegu CW w trakcie pracy?
            if(Mf_PRACA && (Mf_pob_KCW || Mf_pob_CW || Mf_pob_DzCW || Mf_pob_AL)) ToObiegCW();
        else
            //wymuszenie dla obiegu CO w trakcie pracy?
            if(Mf_PRACA && !Mf_BLOKUJ_15S && (Mf_pob_KCO || Mf_pob_SRW || Mf_pob_CO || Mf_pob_DzCO)) ToObiegCO();
        else
            //pompa pracuje, pobudzenie CW,temperatura CO poza zakresem
            if(!Mf_PRACA && Mf_WYLACZ && (DtKNF.tpkt == _MONOHB) && (!DtCOf_err_CO && !DtCOf_ptwp_CO) && Mf_pob_CW) ToObiegCW();
        else
            if(!Mf_PRACA && Mf_WYLACZ && (DtKNF.tpkt == _UNICO) && (!DtCOf_err_CO && !DtCOf_ptwp_CO) && ((PFN.poz == _PFLTO) || (PFN.poz == _PFZMA)) && DtCWf_wpz_CW) ToObiegCW();
        else
            //temperatura CO poza zakresem lub aktywny wybieg 48h?
            if(!Mf_PRACA && !Mf_BLOKUJ_15S && (RdPrt(S_ON_PMP) || Mf_RUSZ_48))
            {
                if(RdPrt(S_ON_PMP)) //temperatura CO poza zakresem
                {
                    if(PFNf_off) ToObiegCO();                   //pozycja OFF
                    else
                    if(PFNf_oflato)                              //pozycja LATO
                    {
                        if((DtKNF.tpkt!=_INWEST)&&(DtKNF.tpkt!=_INWEST2))
                        {
                            if(DtCOf_cold_CO) ToObiegCO();
                            else ToObiegCW();
                        }
                        else ToObiegCO();                            
                    }
                    else                                        //pozycja ZIMA
                    {
                        if(!Mf_pob_KCW && !Mf_pob_CW && !Mf_pob_DzCW && !Mf_pob_AL) ToObiegCO();                //brak pobudzenia CW
                        else ToObiegCW();                                                                       //aktywne pobudzenie CW   
                    }                                       
                }
                else 
                {
                    if(Mf_RUSZ_48) ToObiegCO();   //aktywny wybieg 48h
                    else ToObiegCW();
                }
            }
        else
            //aktywne RT (pozycja zima)i temp. CO wyzsza od warunku zalaczenia kotla?
            if(!Mf_PRACA && !Mf_BLOKUJ_15S && Mf_pob_CO && !DtCOf_su_CO) ToObiegCO();
        else //kociol w spoczynku
        {
            switch(DtKNF.tpkt) //dla danego rodzaju kotla
            {
                case _BITERM: //bitermiczny (miniterm)
                case _BITERM2: //bitermiczny (maxiterm)
                {
                    //nieaktywne warunki wybiegu pompy?
                    if(!Mf_PRACA && !Mf_WYLACZ && !Mf_RUSZ_48) ToObiegCW();
                    break;
                }
                case _MONOHB:
                {
                    //nieaktywna pompa?
                    if(!Mf_PRACA && !Mf_WYLACZ && !Mf_RUSZ_48 && (!_POMPA || Mf_RUSZ_48Z3)) ToObiegCW();
                    break;
                }
                case _UNICO:
                {
                    //b.niska temperatura CW lub nieaktywna pompa?
                    if(!Mf_PRACA && (RdPrt(S_ON_PMP_CW) || (!Mf_WYLACZ && !Mf_RUSZ_48 && !_POMPA))) ToObiegCW();
                    break;
                }
                default: //domyslny monotermiczny
                {
                    //nieaktywna pompa?
                    if(!Mf_PRACA && !Mf_WYLACZ && !Mf_RUSZ_48 && !_POMPA) ToObiegCW();
                    break;
                }
            }
        }
    }
    //--------------
    //decyzja o aktywacji wybiegu pompy
    if(!Mf_ZMKCOCW)
    {
        switch(DtKNF.tpkt) //dla danego rodzaju kotla
        {
            case _BITERM: //bitermiczny (miniterm)
            case _BITERM2: //bitermiczny (maxiterm)
            {
                if(!Mf_WYLACZ && !Mf_RUSZ_48 && !Mf_PRACA)
                {
                    //temperatura CO/CW poza dozwolonym zakresem
                    if(!Mf_RPM_CW&&RdPrt(S_ON_PMP))
                    {
                        Mf_WYLACZ = 1; //uaktywnij wybieg pompy
                    }
                    else
                    //decyzja o zlaczeniu pompy dla RT i temp. CO wyzszej od warunku zalaczenia kotla
                    if(!Mf_RPM_CW&&!PFNf_off && !PFNf_oflato && Mf_pob_CO && !DtCOf_su_CO)
                    {
                        Mf_WYLACZ = 1; //uaktywnij wybieg pompy
                    }
                }
                break;
            }
            case _MONOHB:
            {
                if(!_POMPA && !Mf_WYLACZ && !Mf_RUSZ_48 && !Mf_RUSZ_48Z3 && !Mf_PRACA)
                {
                    //temperatura CO/CW poza dozwolonym zakresem
                    if(RdPrt(S_ON_PMP))
                    {
                        Mf_WYLACZ = 1; //uaktywnij wybieg pompy
                        _POMPA = 1;
                    }
                    else
                        //decyzja o zlaczeniu pompy dla RT i temp. CO wyzszej od warunku zalaczenia kotla
                        if(!PFNf_off && !PFNf_oflato  && Mf_pob_CO && !DtCOf_su_CO)
                    {
                        Mf_WYLACZ = 1; //uaktywnij wybieg pompy
                        _POMPA = 1;
                    }
                }
                break;
            }
            case _UNICO:
            {
                if(!_POMPA && !Mf_WYLACZ && !Mf_RUSZ_48 && !Mf_PRACA)
                {
                    //temperatura CO/CW poza dozwolonym zakresem
                    if((RdPrt(S_ON_PMP)) || (Mf_RPM_CW && RdPrt(S_ON_PMP_CW)))
                    {
                        Mf_WYLACZ = 1; //uaktywnij wybieg pompy
                        _POMPA = 1;
                    }
                    else
                        //decyzja o zlaczeniu pompy dla RT i temp. CO wyzszej od warunku zalaczenia kotla
                        if(!PFNf_off && !PFNf_oflato && Mf_pob_CO && !DtCOf_su_CO)
                    {
                        Mf_WYLACZ = 1; //uaktywnij wybieg pompy
                        _POMPA = 1;
                    }
                }
                break;
            }
            default: //domyslny monotermiczny
            {
                if(!_POMPA && !Mf_WYLACZ && !Mf_RUSZ_48 && !Mf_PRACA)
                {
                    //temperatura CO/CW poza dozwolonym zakresem
                    if(RdPrt(S_ON_PMP))
                    {
                        Mf_WYLACZ = 1; //uaktywnij wybieg pompy
                        _POMPA = 1;
                    }
                    else
                        //decyzja o zlaczeniu pompy dla RT i temp. CO wyzszej od warunku zalaczenia kotla
                        if(!PFNf_off && !PFNf_oflato && Mf_pob_CO && !DtCOf_su_CO)
                    {
                        Mf_WYLACZ = 1; //uaktywnij wybieg pompy
                        _POMPA = 1;
                    }
                }
                break;
            }
        }
    }
    //--------------
    //Czasowe podbicie mocy minimalnej
    if(Mf_PRACA&&(Mf_pob_CO||Mf_pob_DzCO||Mf_pob_DzCW)&&!Mf_pob_SRW&&!Mf_AKTYWNE_KNF&&!Mf_pob_KCO&&!Mf_pob_KCW&&!Mf_pob_CW&&!Mf_pob_AL)
    {
            if((PWM.DKNF_MDMOD<PWM.DV16_MDMOD)&&(RTS(_RTWPWM)<_TWSPWM))
            {
                    PWM.MDMOD=PWM.DV16_MDMOD;	//czasowa praca z moca minimalna od 25 do 50%
            }
            else
            {
                    PWM.MDMOD=PWM.DKNF_MDMOD;	//praca z domyslna moca minimalna zdefiniowana w konfiguracji
            }
    }
    else
    {
            PWM.MDMOD=PWM.DKNF_MDMOD;	//praca z domyslna moca minimalna zdefiniowana w konfiguracji
    }
    //--------------
    //aktywacja/deaktywacja bloku modulacji plomieniem dla pobudzen RT i CW
    if(Mf_pob_CO || Mf_pob_CW || Mf_pob_AL)
    {
        if(!Mf_ROZRUCH && _GRZEJ)
        {
            if(!Mf_MODULUJ)
            {
                Mf_MODULUJ = 1;
                RestartMod();

            }
        }
        else Mf_MODULUJ = 0;
    }
    else Mf_MODULUJ = 0;
    //deaktywacja bloku modulacji plomieniem dla awarii modulatora lub dla szybkiego narostu temp.
    if(!Mf_PRACA || !RdPrt(S_MOD_ON) || Mf_RUN_MNMOD)
    {
        Mf_MODULUJ = 0; //wylacz blok modulacji
        if(!Mf_PRACA || !RdPrt(S_MOD_ON)) WritePWM(0);
        else WritePWM(PWM.MDMOD); //zbyt szybki narost temp.
    }
    //wymuszenie konkretnej wartosci wypelnienia dla danego wymuszenia
    if(Mf_pob_KCW && !Mf_ROZRUCH && _GRZEJ) WritePWM(PWM.MMAXCW);
    if(Mf_pob_KCO && !Mf_ROZRUCH && _GRZEJ) WritePWM(PWM.MMAXCO);
#if _SRVTRB==1														//aktywny tryb serwisowy kompilacji?
    if(Mf_pob_SRW && !Mf_ROZRUCH && _GRZEJ) WriteOPWM(mypwm);
#else
    if(Mf_pob_SRW && !Mf_ROZRUCH && _GRZEJ)
    {
        if(PFNf_fns_max) WritePWM(_MG2_MOD); //tryb serwisowy z moca maksymalna?
        else WritePWM(PWM.MDMOD);
    }
#endif
    if(Mf_pob_DzCO && !Mf_ROZRUCH && _GRZEJ) WritePWM(PWM.MDMOD);
    if(Mf_pob_DzCW && !Mf_ROZRUCH && _GRZEJ) WritePWM(PWM.MDMOD);
}
//zmiana obiegu na CO

void ToObiegCO(void)
{
    switch(DtKNF.tpkt) //dla danego rodzaju kotla
    {
        case _BITERM: //bitermiczny (miniterm)
        case _BITERM2: //bitermiczny (maxiterm)
        {
            if(!Mf_ZMKCOCW && !_POMPA) //aktywny obieg CW?
            {
                Mf_RPM_CW = 0; //wyzeruj znacznik obiegu CW
                Mf_ZMKCOCW = 1; //uruchom procedure zmiany obiegu CW na CO
                M.ZKP = 0;
            }
            return;
        }
        case _UNICO:
        case _MONOHB:
        {
            if(!Mf_ZMKCOCW && !_KCO_CW) //aktywny obieg CW?
            {
                Mf_RPM_CW = 0; //wyzeruj znacznik obiegu CW
                Mf_ZMKCOCW = 1; //uruchom procedure zmiany obiegu CW na CO
                M.ZKP = 0;
            }
            return;
        }
        default: //domyslny monotermiczny
        {
            if(!Mf_ZMKCOCW && !_KCO_CW) //aktywny obieg CW?
            {
                Mf_RPM_CW = 0; //wyzeruj znacznik obiegu CW
                Mf_ZMKCOCW = 1; //uruchom procedure zmiany obiegu CW na CO
                M.ZKP = 0;
            }
            return;
        }
    }
}
//zmiana obiegu na CW

void ToObiegCW(void)
{
    switch(DtKNF.tpkt) //dla danego rodzaju kotla
    {
        case _BITERM: //bitermiczny (miniterm)
        case _BITERM2: //bitermiczny (maxiterm)
        {
            if(!Mf_ZMKCOCW && _POMPA) //aktywny obieg CO?
            {
                Mf_RPM_CW = 1; //ustaw znacznik obiegu CW
                Mf_ZMKCOCW = 1; //uruchom procedure zmiany obiegu CO na CW
                M.ZKP = 0;
                if(Mf_pob_KCW || Mf_pob_CW || Mf_pob_DzCW)
                {
                    if(Mf_PRACA)
                    {
                        Mf_BLOKUJ_15S = 1; //ustaw flage blokady 15 sek dla grzania CO
                        M.BL15 = 0;
                    }
                }
            }
            return;
        }
        case _UNICO:
        case _MONOHB:
        {
            if(!Mf_ZMKCOCW && _KCO_CW) //aktywny obieg CO?
            {
                Mf_RPM_CW = 1; //ustaw znacznik obiegu CW
                Mf_ZMKCOCW = 1; //uruchom procedure zmiany obiegu CO na CW
                M.ZKP = 0;
                if(Mf_pob_KCW || Mf_pob_CW || Mf_pob_DzCW || Mf_pob_AL)
                {
                    if(Mf_PRACA)
                    {
                        Mf_BLOKUJ_15S = 1; //ustaw flage blokady 15 sek dla grzania CO
                        M.BL15 = 0;
                    }
                }
            }
            return;
        }
        default: //domyslny monotermiczny
        {
            if(!Mf_ZMKCOCW && _KCO_CW) //aktywny obieg CO?
            {
                Mf_RPM_CW = 1; //ustaw znacznik obiegu CW
                Mf_ZMKCOCW = 1; //uruchom procedure zmiany obiegu CO na CW
                M.ZKP = 0;
                if(Mf_pob_KCW || Mf_pob_CW || Mf_pob_DzCW || Mf_pob_AL)
                {
                    if(Mf_PRACA)
                    {
                        Mf_BLOKUJ_15S = 1; //ustaw flage blokady 15 sek dla grzania CO
                        M.BL15 = 0;
                    }
                }
            }
            return;
        }
    }
}
//Ustaw minimalny wydatek na modulatorze

void SetLoMod(void)
{
    Mf_MODULUJ = 0; //wylacz blok modulacji
    WritePWM(0);
}
//------------------
//------------------
//Decyzja o zapaleniu palnika

void DecRozruch(void)
{
    switch(DtKNF.tpkt) //dla danego rodzaju kotla
    {
        case _BITERM: //bitermiczny (miniterm)
        case _BITERM2: //bitermiczny (maxiterm)
        {
            if(!Mf_ROZRUCH && !_GRZEJ)
                if(!Mf_ZMKCOCW && !Mf_RUSZ_PWR && !Mf_RUSZ_PLOMIEN && !Mf_RUSZ_CIAG && !Mf_RUSZ_STB && !Mf_RUSZ_NTC_CO && !Mf_RUSZ_NTC_CW)
                    if(!Mf_RUSZ_POMPE && !Mf_RUSZ_CISN && !Mf_RUSZ_NCISN)
                    {
                        if(Mf_pob_KCW) //konfiguracja obieg CW
                        {
                            if(DtCWf_su_CW)
                            {
                                Mf_BLOKUJ_15S = 1;
                                M.BL15 = 0;
                                SetRozruch();
                            }
                        }
                        else
                            if(Mf_pob_KCO) //konfiguracja obieg CO
                        {
                            if(DtCOf_su_CO) SetRozruch();
                        }
                        else
                            if(Mf_pob_SRW) //funkcja serwisowa
                        {
                            if(DtCOf_su_SRW_CO) SetRozruch();
                        }
                        else
                            if(Mf_pob_CW) //grzanie obiegu CW
                        {
                            if(DtCWf_su_CW)
                            {
                                Mf_BLOKUJ_15S = 1;
                                M.BL15 = 0;
                                SetRozruch();
                            }
                        }
                        else
                            if(Mf_pob_CO && DtCOf_su_CO && !Mf_BLOKUJ_3M && !Mf_BLOKUJ_15S) //grzanie obiegu CO
                        {
                            SetRozruch();
                        }
                        else
                            if(Mf_pob_DzCO && DtCOf_su_DZ_CO && !Mf_BLOKUJ_3M && !Mf_BLOKUJ_15S) //temperatura dyzurna w obiegu CO
                        {
                            SetRozruch();
                        }
                        else
                            if(Mf_pob_DzCW && DtCWf_su_DZ_CW) //temperatura dyzurna w obiegu CW
                        {
                            Mf_BLOKUJ_15S = 1;
                            M.BL15 = 0;
                            SetRozruch();
                        }
                    }
            break;
        }
        case _UNICO:
        case _MONOHB:
        {
            if(!Mf_ROZRUCH && !_GRZEJ)
                if(!Mf_ZMKCOCW && !Mf_RUSZ_PWR && !Mf_RUSZ_PLOMIEN && !Mf_RUSZ_CIAG && !Mf_RUSZ_STB && !Mf_RUSZ_NTC_CO && !Mf_RUSZ_NTC_CW)
                    if(!Mf_RUSZ_POMPE && !Mf_RUSZ_CISN && !Mf_RUSZ_NCISN)
                    {
                        if(Mf_pob_KCW) //konfiguracja obieg CW
                        {
                            if(DtCWf_su_CW && DtCOf_pz_CO)
                            {
                                Mf_BLOKUJ_15S = 1;
                                M.BL15 = 0;
                                SetRozruch();
                            }
                        }
                        else
                            if(Mf_pob_KCO) //konfiguracja obieg CO
                        {
                            if(DtCOf_su_CO) SetRozruch();
                        }
                        else
                            if(Mf_pob_SRW) //funkcja serwisowa
                        {
                            if(DtCOf_su_SRW_CO) SetRozruch();
                        }
                        else
                            if(Mf_pob_AL)
                        {
                            if(DtCOf_pz_CO)
                            {
                                Mf_BLOKUJ_15S = 1;
                                M.BL15 = 0;
                                SetRozruch();
                            }
                        }
                        else
                            if(Mf_pob_CW) //grzanie obiegu CW
                        {
                            if(DtCWf_su_CW && DtCOf_pz_CO)
                            {
                                Mf_BLOKUJ_15S = 1;
                                M.BL15 = 0;
                                SetRozruch();
                            }
                        }
                        else
                            if(Mf_pob_CO && DtCOf_su_CO && !Mf_BLOKUJ_3M && !Mf_BLOKUJ_15S) //grzanie obiegu CO
                        {
                            SetRozruch();
                        }
                        else
                            if(Mf_pob_DzCO && DtCOf_su_DZ_CO && !Mf_BLOKUJ_3M && !Mf_BLOKUJ_15S) //temperatura dyzurna w obiegu CO
                        {
                            SetRozruch();
                        }
                        else
                            if(Mf_pob_DzCW && DtCWf_su_DZ_CW && DtCOf_pz_CO) //temperatura dyzurna w obiegu CW
                        {
                            Mf_BLOKUJ_15S = 1;
                            M.BL15 = 0;
                            SetRozruch();
                        }
                    }
            break;
        }
        default: //domyslny monotermiczny
        {
            if(!Mf_ROZRUCH && !_GRZEJ)
                if(!Mf_ZMKCOCW && !Mf_RUSZ_PWR && !Mf_RUSZ_PLOMIEN && !Mf_RUSZ_CIAG && !Mf_RUSZ_STB && !Mf_RUSZ_NTC_CO && !Mf_RUSZ_NTC_CW)
                    if(!Mf_RUSZ_POMPE && !Mf_RUSZ_CISN && !Mf_RUSZ_NCISN)
                    {
                        if(Mf_pob_KCW) //konfiguracja obieg CW
                        {
                            if(DtCWf_su_CW && DtCOf_pz_CO)
                            {
                                Mf_BLOKUJ_15S = 1;
                                M.BL15 = 0;
                                SetRozruch();
                            }
                        }
                        else
                            if(Mf_pob_KCO) //konfiguracja obieg CO
                        {
                            if(DtCOf_su_CO) SetRozruch();
                        }
                        else
                            if(Mf_pob_SRW) //funkcja serwisowa
                        {
                            if(DtCOf_su_SRW_CO) SetRozruch();
                        }
                        else
                            if(Mf_pob_AL)
                        {
                            if(DtCOf_pz_CO)
                            {
                                Mf_BLOKUJ_15S = 1;
                                M.BL15 = 0;
                                SetRozruch();
                            }
                        }
                        else
                            if(Mf_pob_CW) //grzanie obiegu CW
                        {
                            if(DtCWf_su_CW && DtCOf_pz_CO)
                            {
                                Mf_BLOKUJ_15S = 1;
                                M.BL15 = 0;
                                SetRozruch();
                            }
                        }
                        else
                            if(Mf_pob_CO && DtCOf_su_CO && !Mf_BLOKUJ_3M && !Mf_BLOKUJ_15S) //grzanie obiegu CO
                        {
                            SetRozruch();
                        }
                        else
                            if(Mf_pob_DzCO && DtCOf_su_DZ_CO && !Mf_BLOKUJ_3M && !Mf_BLOKUJ_15S) //temperatura dyzurna w obiegu CO
                        {
                            SetRozruch();
                        }
                        else
                            if(Mf_pob_DzCW && DtCWf_su_DZ_CW && DtCOf_pz_CO) //temperatura dyzurna w obiegu CW
                        {
                            Mf_BLOKUJ_15S = 1;
                            M.BL15 = 0;
                            SetRozruch();
                        }
                    }
            break;
        }
    }
}
//Uruchomienie procedury rozruchu

void SetRozruch(void)
{
    Mf_BLOKUJ_3M = 0; //usun blokade L3
    M.BL3 = 0;
    EndL3();
    Mf_RUSZ_48 = 0;
    M.R48 = 0;
    Mf_RUSZ_48Z3 = 0;
    M.R48Z3 = 0;
    Mf_VWYBIEG = 0;
    Mf_DE_VNT = 0;
    M.VWB = 0;
    //-----
    Mf_KCW_PMP = 0;
    M.WYL = 0;
    M.RPR = 0;
    Mf_WYLACZ = 0;
    Mf_RUSZ_POMPE = 0;
    Mf_ROZRUCH = 1;
    M.ROZ = 0;
    //	x=0;
}
//------------------
//------------------
//Decyzja o wygaszeniu palnika

void DecWylacz(void)
{
    switch(DtKNF.tpkt) //dla danego rodzaju kotla
    {
        case _BITERM: //bitermiczny (miniterm)
        case _BITERM2: //bitermiczny (maxiterm)
        {
            if(!Mf_WYLACZ)
                if(Mf_ROZRUCH || _GRZEJ)
                {
                    if(!Mf_pob_KCW && !Mf_pob_KCO && !Mf_pob_SRW && !Mf_pob_CW && !Mf_pob_CO && !Mf_pob_DzCO && !Mf_pob_DzCW)
                    {
                        SetWylacz();
                        return;
                    }
                    if(Mf_RUSZ_PWR || Mf_RUSZ_PLOMIEN || Mf_RUSZ_CIAG || Mf_RUSZ_STB || Mf_RUSZ_NTC_CO || Mf_RUSZ_NTC_CW || Mf_RUSZ_POMPE || Mf_RUSZ_CISN || Mf_RUSZ_NCISN)
                    {
                        SetWylacz();
                    }
                    else
                        if(Mf_pob_KCW) //konfiguracja obieg CW
                    {
                        if(DtCWf_ad_CW) SetWylacz();
                    }
                    else
                        if(Mf_pob_KCO) //konfiguracja obieg CO
                    {
                        if(RdPrt(S_ADD_CO)) SetWylacz();
                    }
                    else
                        if(Mf_pob_SRW) //funkcja serwisowa
                    {
                        if(DtCOf_ad_SRW_CO) SetWylacz();
                    }
                    else
                        if(Mf_pob_CW) //grzanie obiegu CW
                    {
                        if(DtCWf_ad_CW) SetWylacz();
                    }
                    else
                        if(Mf_pob_CO && (RdPrt(S_ADD_CO) || Mf_BLOKUJ_3M || Mf_BLOKUJ_15S)) //grzanie obiegu CO
                    {
                        if(RdPrt(S_ADD_CO) && !Mf_BLOKUJ_3M && !Mf_BLOKUJ_15S)
                        {
                            Mf_BLOKUJ_3M = 1; //uaktywnij blokade L3
                            M.BL3 = 0;
                            SetL3();
                        }
                        SetWylacz();
                    }
                    else
                        if(Mf_pob_DzCO && (DtCOf_ad_DZ_CO || Mf_BLOKUJ_3M || Mf_BLOKUJ_15S)) //temperatura dyzurna w obiegu CO
                    {
                        SetWylacz();
                    }
                    else
                        if(Mf_pob_DzCW && DtCWf_ad_DZ_CW) //temperatura dyzurna w obiegu CW
                    {
                        SetWylacz();
                    }
                }
            break;
        }
        case _UNICO:
        case _MONOHB:
        {
            if(!Mf_WYLACZ)
                if(Mf_ROZRUCH || _GRZEJ)
                {
                    if(!Mf_pob_KCW && !Mf_pob_AL && !Mf_pob_KCO && !Mf_pob_SRW && !Mf_pob_CW && !Mf_pob_CO && !Mf_pob_DzCO && !Mf_pob_DzCW)
                    {
                        SetWylacz();
                        return;
                    }
                    if(Mf_RUSZ_PWR || Mf_RUSZ_PLOMIEN || Mf_RUSZ_CIAG || Mf_RUSZ_STB || Mf_RUSZ_NTC_CO || Mf_RUSZ_NTC_CW || Mf_RUSZ_POMPE || Mf_RUSZ_CISN || Mf_RUSZ_NCISN)
                    {
                        SetWylacz();
                    }
                    else
                        if(Mf_pob_KCW) //konfiguracja obieg CW
                    {
                        if(DtCWf_ad_CW || !DtCOf_pz_CO) SetWylacz();
                    }
                    else
                        if(Mf_pob_AL) //konfiguracja obieg CW
                    {
                        if(!DtCOf_pz_CO) SetWylacz();
                    }
                    else
                        if(Mf_pob_KCO) //konfiguracja obieg CO
                    {
                        if(RdPrt(S_ADD_CO)) SetWylacz();
                    }
                    else
                        if(Mf_pob_SRW) //funkcja serwisowa
                    {
                        if(DtCOf_ad_SRW_CO) SetWylacz();
                    }
                    else
                        if(Mf_pob_CW) //grzanie obiegu CW
                    {
                        if(DtCWf_ad_CW || !DtCOf_pz_CO) SetWylacz();
                    }
                    else
                        if(Mf_pob_CO && (RdPrt(S_ADD_CO) || Mf_BLOKUJ_3M || Mf_BLOKUJ_15S)) //grzanie obiegu CO
                    {
                        if(RdPrt(S_ADD_CO) && !Mf_BLOKUJ_3M && !Mf_BLOKUJ_15S)
                        {
                            Mf_BLOKUJ_3M = 1; //uaktywnij blokade L3
                            M.BL3 = 0;
                            SetL3();
                        }
                        SetWylacz();
                    }
                    else
                        if(Mf_pob_DzCO && (DtCOf_ad_DZ_CO || Mf_BLOKUJ_3M || Mf_BLOKUJ_15S)) //temperatura dyzurna w obiegu CO
                    {
                        SetWylacz();
                    }
                    else
                        if(Mf_pob_DzCW && (DtCWf_ad_DZ_CW || !DtCOf_pz_CO)) //temperatura dyzurna w obiegu CW
                    {
                        SetWylacz();
                    }
                }
            break;
        }
        default: //domyslny monotermiczny
        {
            if(!Mf_WYLACZ)
                if(Mf_ROZRUCH || _GRZEJ)
                {
                    if(!Mf_pob_KCW && !Mf_pob_AL && !Mf_pob_KCO && !Mf_pob_SRW && !Mf_pob_CW && !Mf_pob_CO && !Mf_pob_DzCO && !Mf_pob_DzCW)
                    {
                        SetWylacz();
                        return;
                    }
                    if(Mf_RUSZ_PWR || Mf_RUSZ_PLOMIEN || Mf_RUSZ_CIAG || Mf_RUSZ_STB || Mf_RUSZ_NTC_CO || Mf_RUSZ_NTC_CW || Mf_RUSZ_POMPE || Mf_RUSZ_CISN || Mf_RUSZ_NCISN)
                    {
                        SetWylacz();
                    }
                    else
                        if(Mf_pob_KCW) //konfiguracja obieg CW
                    {
                        if(DtCWf_ad_CW || !DtCOf_pz_CO) SetWylacz();
                    }
                    else
                        if(Mf_pob_AL) //konfiguracja obieg CW
                    {
                        if(!DtCOf_pz_CO) SetWylacz();
                    }
                    else
                        if(Mf_pob_KCO) //konfiguracja obieg CO
                    {
                        if(RdPrt(S_ADD_CO)) SetWylacz();
                    }
                    else
                        if(Mf_pob_SRW) //funkcja serwisowa
                    {
                        if(DtCOf_ad_SRW_CO) SetWylacz();
                    }
                    else
                        if(Mf_pob_CW) //grzanie obiegu CW
                    {
                        if(DtCWf_ad_CW || !DtCOf_pz_CO) SetWylacz();
                    }
                    else
                        if(Mf_pob_CO && (RdPrt(S_ADD_CO) || Mf_BLOKUJ_3M || Mf_BLOKUJ_15S)) //grzanie obiegu CO
                    {
                        if(RdPrt(S_ADD_CO) && !Mf_BLOKUJ_3M && !Mf_BLOKUJ_15S)
                        {
                            Mf_BLOKUJ_3M = 1; //uaktywnij blokade L3
                            M.BL3 = 0;
                            SetL3();
                        }
                        SetWylacz();
                    }
                    else
                        if(Mf_pob_DzCO && (DtCOf_ad_DZ_CO || Mf_BLOKUJ_3M || Mf_BLOKUJ_15S)) //temperatura dyzurna w obiegu CO
                    {
                        SetWylacz();
                    }
                    else
                        if(Mf_pob_DzCW && (DtCWf_ad_DZ_CW || !DtCOf_pz_CO)) //temperatura dyzurna w obiegu CW
                    {
                        SetWylacz();
                    }
                }
            break;
        }
    }
}
//Uruchomienie procedury wygaszenia palnika

void SetWylacz(void)
{
    GrzejOFF();
    //-----
    //operacje wykonywane po kazdej pracy w obiegu CW
    switch(DtKNF.tpkt) //dla danego rodzaju kotla
    {
        case _BITERM: //bitermiczny (miniterm)
        case _BITERM2: //bitermiczny (maxiterm)
        {
            if(!_POMPA) //obieg CW?
            {
                Mf_KCW_PMP = 1; //ustaw znacznik obiegu CW
                Mf_BLOKUJ_15S = 1; //uaktywnij blokade 15s
                M.BL15 = 0;
            }
            else Mf_KCW_PMP = 0;
            ToObiegCO(); //po grzaniu w CW zmien obieg na CO
            break;
        }
        case _UNICO:
        case _MONOHB:
        {
            if(!_KCO_CW) //obieg CW?
            {
                Mf_BLOKUJ_15S = 1; //uaktywnij blokade 15s
                M.BL15 = 0;
            }
            break;
        }
        default: //domyslny monotermiczny
        {
            if(!_KCO_CW) //obieg CW?
            {
                Mf_BLOKUJ_15S = 1; //uaktywnij blokade 15s
                M.BL15 = 0;
            }
            break;
        }
    }
    //-----
    Mf_WYLACZ = 1; //uaktywnij procedure sterowania wybiegiem pompy
    if(Mf_TURBO && _VNT) Mf_VWYBIEG = 1; //dla kotla turbo uaktywnij wybieg wentylatora
    M.ROZ = 0;
    M.MNM = 0;
    Mf_ROZRUCH = 0;
    Mf_RUN_MNMOD = 0;

    //wyzeruj bity pobudzen od temperatur dyzurnych
    DtCOf_on_DZ_CO = 0;
    SetPrt(S_DZ_CO, 0);
    DtCWf_on_DZ_CW = 0;
    SetPrt(S_DZ_CW, 0);
}
//------------------
//------------------

void FunkcjaSerwisowa(void)
{
    if(Mf_AKTYWNE_KNF)
    {
        M.SRV = 0;
        Mf_AKTYWNE_SRW = 0; //deaktywacja funkcji serwisowej
    }
    else
        switch(M.SRV)
        {
            case 0:
            {
                //if((PFN.poz == _PFZMA) && (PFNf_fnserw && !PFNf_ofnserw))
                if(!Mf_AKTYWNE_SRW && (PFN.poz == _PFZMA) && PFNf_fnserw)
                {
                    M.SRV = 1;
                    Mf_AKTYWNE_SRW = 1; //aktywacja funkcji serwisowej
                    StartRTM(_RTSRV1);
                }
                break;
            }
            case 1:
            {
                //if((PFN.poz==_PFZMA)&&GetBit2(RSDT1.inSTAT0,_INITSRV)&&(RTM(_RTSRV1)<_TSRV1)) return;
                //if((PFN.poz == _PFZMA) && !PFNf_res && (RTM(_RTSRV1) < _TSRV1)) return;
                
                if((PFN.poz == _PFZMA) && PFNf_fnserw && (RTM(_RTSRV1) < _TSRV1)) return;
                else
                {
                    M.SRV = 0;
                    PFNf_fnserw=0;
                    Mf_AKTYWNE_SRW = 0; //deaktywacja funkcji serwisowej
                }
                break;
            }
        }
}
/*
void FunkcjaSerwisowa(void)
{
        if(Mf_AKTYWNE_KNF)
        {
                M.SRV=0;
                Mf_AKTYWNE_SRW=0;		//deaktywacja funkcji serwisowej
        }
        else
        switch(M.SRV)
        {
                case 0:
                {
                        if((PFN.poz==_PFZMA)&&PFNf_res)
                        {
                                M.SRV=1;
                                StartRTS(_RTSRV0);
                        }
                        break;
                }
                case 1:
                {
                        if((PFN.poz==_PFZMA)&&PFNf_res)
                        {
                                if(RTS(_RTSRV0)<_TSRV0) return;
                                M.SRV=2;
                                Mf_AKTYWNE_SRW=1;		//aktywacja funkcji serwisowej
                                StartRTM(_RTSRV1);
                        }
                        else
                        {
                                M.SRV=0;
                        }
                }
                case 2:
                {
                        if((PFN.poz==_PFZMA)&&(RTM(_RTSRV1)<_TSRV1)) return;
                        else
                        {
                                M.SRV=0;
                                Mf_AKTYWNE_SRW=0;		//deaktywacja funkcji serwisowej
                        }
                }
        }
}*/
//------------------
//------------------
//Blokada L3

void BlokadaL3(void)
{
    switch(M.BL3)
    {
        case 0:
        {
            M.BL3 = 1;
            StartRTM(_RTBL3);
        }
        case 1:
        {
            SetL3();
            if((PFN.poz != _PFZMA && !Mf_ZMKCOCW) || Mf_pob_CW || (DtKNF.tpkt == _UNICO && DtCWf_wpz_CW)) //przelacznik funkcyjny na pozycji innej niz ZIMA lub aktywne?
            {
                Mf_BLOKUJ_3M = 0;
                M.BL3 = 0;
                EndL3();
            }
            if(RTM(_RTBL3) < _TBL3) return;
            Mf_BLOKUJ_3M = 0;
            M.BL3 = 0;
            EndL3();
        }
    }
}
//------------------
//------------------
//Blokada 15s

void Blokada15S(void)
{
    switch(M.BL15)
    {
        case 0:
        {
            M.BL15 = 1;
            StartRTdS(_RTBL15);
        }
        case 1:
        {
            if((RTdS(_RTBL15) > 1) && (PFN.poz != _PFZMA)) //przelacznik funkcyjny na pozycji innej niz ZIMA?
            { //1 sek. zwloki na filtracje momentu zalaczenia w pozycji LATO
                Mf_BLOKUJ_15S = 0;
                M.BL15 = 0;
            }
            if(RTdS(_RTBL15) < _TBL15S) return;
            Mf_BLOKUJ_15S = 0;
            M.BL15 = 0;
        }
    }
}
//------------------
//------------------
//Zalaczenie plytki zapalacza

void GrzejON(void)
{
    _GRZEJ = 1;
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    _VNT = 1;
}
//------------------
//------------------
//Wylaczenie plytki zapalacza

void GrzejOFF(void)
{
    if(!Mf_TURBO)
    {
        _VNT = 0;
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        _GRZEJ = 0;
    }
    else
    {
        _GRZEJ = 0;
    }
}
//------------------
//------------------
//Procedura rozruchu kotla

void Rozruch(void)
{
    StartRTS(_RTWPWM);
    switch(M.ROZ)
    {
            //dla kotla z otwartym obiegiem wst?pne, kontrolne wylaczenie pompy
        case 0:
        {
            if(DtKNF.robg == _OTWARTY) //uniko z obiegiem otwartym?
            {
                if(Mf_ZMKCOCW) return; //odczekaj az minie procedura zmiany obiegu
            }
            M.ROZ = 1;
            StartRTS(_RTROZ);
        }
            //dla kotla z otwartym obiegiem oczekiwanie az sygnal przeplywu WG zniknie przy wylaczonej pompie
        case 1:
        {
            if(DtKNF.robg == _OTWARTY) //uklad z obiegiem otwartym?
            {
                if(RTS(_RTROZ) < 1) return; //zwloka na klepanie przekaznikiem
                _POMPA = 0; //wylacz pompe
                if(!RdPrt(S_PRZ_CO)) //czekaj na stan brak przeplywu WG
                {
                    M.ROZ = 2;
                    StartRTS(_RTROZ);
                    return;
                }
                if(RTS(_RTROZ) < _TCPRO) return;
                M.ERR_BTY = 0x05;
                SetErr(0x05);//PrintErr(0x05, 1); //wyswietl kod bledu
                ErrPTG(); //wylaczenie awaryjne z blokad?
                return;
            }
            M.ROZ = 2;
            StartRTS(_RTROZ);
        }
            //procedura rozruchu
        case 2:
        {
            switch(DtKNF.tpkt) //dla danego rodzaju kotla
            {
                case _BITERM: //bitermiczny (miniterm)
                case _BITERM2: //bitermiczny (maxiterm)
                {
                    if(!Mf_RPM_CW && (DtKNF.robg == _OTWARTY)) _POMPA = 1; //zalacz pompe dla obiegu CO
                    break;
                }
                case _UNICO: //jednofunkcyjny
                case _MONOHB:
                {
                    if(!Mf_ZMKCOCW) _POMPA = 1; //zalacz pompe
                    break;
                }
                default: //domyslny monotermiczny
                {
                    if(!Mf_ZMKCOCW) _POMPA = 1; //zalacz pompe
                    break;
                }
            }
            WritePWM(PWM.MDMOD);
            M.ROZ = 3;
            StartRTS(_RTROZ);
        }
            //oczekiwanie az sygnal obecnosci ciagu zniknie przy wylaczonym wentylatorze
        case 3:
        {
            if(Mf_TURBO)
            {
                if(RdPrt(S_CIG_NO) && !_VNT) //obecny sygnal ciagu?
                {
                    if(RTS(_RTROZ) < _TCIRO) return;
                    M.RCI = 0;
                    Mf_RUSZ_CIAG = 1;
                    return;
                }
                _VNT = 1; //uruchom wentylator
            }
            M.ROZ = 4;
            StartRTS(_RTROZ);
        }
            //oczekiwanie na sygnal obecnosci ciagu przy zalaczonym wentylatorze
        case 4:
        {
            if(Mf_TURBO)
            {
                if(!RdPrt(S_CIG_NO_1S) && _VNT) //nieobecny sygnal ciagu?
                {
                    if(RTS(_RTROZ) < _TCIRO) return;
                    M.RCI = 0;
                    Mf_RUSZ_CIAG = 1;
                    return;
                }
            }
            M.ROZ = 5;
            StartRTS(_RTROZ);
        }
            //kontrola poprawnosci dzialania ukladu detekcji plomienia
        case 5:
        {
            if(Mf_TURBO && !RdPrt(S_CIG_NO_1S)) //nieobecny sygnal ciagu?
            {
                M.RCI = 0;
                Mf_RUSZ_CIAG = 1;
                return;
            }
            if(RdPrt(S_PLM) && !_GRZEJ) //obecny plomien mimo ze _GRZEJ=0?
            {
                if(RTS(_RTROZ) < _TBPL0) return;
                M.ERR_BTY = 0x06;
                SetErr(0x06);//PrintErr(0x06, 1); //wyswietl kod bledu
                ErrPTG();
            }
            GrzejON(); //_GRZEJ=1;							//zapal plomien na palniku
            M.ROZ = 6;
            StartRTS(_RTROZ);
        }
            //zalaczenie pompy, oczekiwanie na sygnal obecnosci plomienia
        case 6:
        {
            switch(DtKNF.tpkt) //dla danego rodzaju kotla
            {
                case _BITERM: //bitermiczny (miniterm)
                case _BITERM2: //bitermiczny (maxiterm)
                {
                    break;
                }
                case _UNICO:
                case _MONOHB:
                {
                    if(!Mf_ZMKCOCW) _POMPA = 1; //zalacz pompe (np po zmianie obiegu)
                    break;
                }
                default: //domyslny monotermiczny
                {
                    if(!Mf_ZMKCOCW) _POMPA = 1; //zalacz pompe (np po zmianie obiegu)
                    break;
                }
            }
            if(Mf_TURBO && !RdPrt(S_CIG_NO_1S)) //nieobecny sygnal ciagu?
            {
                M.RCI = 0;
                Mf_RUSZ_CIAG = 1;
                return;
            }
            if(RdPrt(S_MOD_ON)) WritePWM(PWM.MSTART);
            else WritePWM(PWM.MDMOD);

            if(RTS(_RTROZ) < _TITPL) return; //okres czasu gdy ignorowany jest syg. plomien

            if(RTS(_RTROZ) < _TDTPL)
            {
                if(RdPrt(S_PLM)) //stwierdzono obecnosc plomienia?
                {
                    M.ROZ = 7;
                    StartRTS(_RTROZ);
                }
                return;
            }
            else //nie stwierdzono plomienia w czasie _TDTPL?
            {
                DecyzjaPLM();
                return;
            }
        }
            //wybieg na mocy startowej
        case 7:
        {
            if(Mf_RPM_CW)
            {
                Mf_ROZRUCH = 0;
                M.ROZ = 0;
                return;
            }
            if(Mf_TURBO && !RdPrt(S_CIG_NO_1S)) //nieobecny sygnal ciagu?
            {
                M.RCI = 0;
                Mf_RUSZ_CIAG = 1;
                return;
            }
            if(!RdPrt(S_PLM)) //stwierdzono brak plomienia?
            {
                DecyzjaPLM2();
                //return;
            }
            if(RTS(_RTROZ) < _TDWPL) return;
            Mf_ROZRUCH = 0;
            M.ROZ = 0;
        }
    }
}

void DecyzjaPLM(void)
{
    if(Mf_OST_PL) //ostatnia proba?
    {
        M.ERR_BTY = 0x01;
        SetErr(0x01);//PrintErr(0x01, 1);
        ErrPTG();
    }
    else
    {
        Mf_RUSZ_PLOMIEN = 1;
        if(!M.LICZNIK_PL) Mf_RUSZ_RPLM = 1; //znacznik pierwszej aktywacji proc. w trakcie rozruchu
        Mf_ROZRUCH = 0;
        M.ROZ = 0;
    }
}

void DecyzjaPLM2(void)
{
    if(Mf_TURBO) //sterownik TURBO?
    {
        if(_VNT) //wentylator pracuje?
        {
            //if(RdPrt(S_CIG_NO)&&RdPrt(S_STB))
            if(_CIG_NO && _STB)
            {
                if(Mf_OST_PL) //ostatnia proba?
                {
                    M.ERR_BTY = 0x01;
                    SetErr(0x01);//PrintErr(0x01, 1);
                    ErrPTG();
                }
                else
                {
                    M.RPL = 0;
                    Mf_RUSZ_PLOMIEN = 1;
                    if(!M.LICZNIK_PL) Mf_RUSZ_RPLM = 1; //znacznik pierwszej aktywacji proc. w trakcie rozruchu
                    Mf_ROZRUCH = 0;
                    M.ROZ = 0;
                }
            }
        }
        else //wentylator nie pracuje
        {
            if(RdPrt(S_STB))
            {
                if(Mf_OST_PL) //ostatnia proba?
                {
                    M.ERR_BTY = 0x01;
                    SetErr(0x01);//PrintErr(0x01, 1);
                    ErrPTG();
                }
                else
                {
                    M.RPL = 0;
                    Mf_RUSZ_PLOMIEN = 1;
                    if(!M.LICZNIK_PL) Mf_RUSZ_RPLM = 1; //znacznik pierwszej aktywacji proc. w trakcie rozruchu
                    Mf_ROZRUCH = 0;
                    M.ROZ = 0;
                }
            }
        }
    }
    else //sterownik ATMOSFERYCZNY
    {
        //if(RdPrt(S_CIG_NC)&&RdPrt(S_STB))
        if(_CIG_NC && _STB)
        {
            if(Mf_OST_PL) //ostatnia proba?
            {
                M.ERR_BTY = 0x01;
                SetErr(0x01);//PrintErr(0x01, 1);
                ErrPTG();
            }
            else
            {
                M.RPL = 0;
                Mf_RUSZ_PLOMIEN = 1;
                if(!M.LICZNIK_PL) Mf_RUSZ_RPLM = 1; //znacznik pierwszej aktywacji proc. w trakcie rozruchu
                Mf_ROZRUCH = 0;
                M.ROZ = 0;
            }
        }
    }
}
//------------------
//------------------
//Wybieg pompy

void Wylacz(void)
{
    switch(DtKNF.tpkt) //dla danego rodzaju kotla
    {
        case _BITERM: //bitermiczny (miniterm)
        case _BITERM2: //bitermiczny (maxiterm)
        {
            switch(M.WYL)
            {
                case 0:
                {
                    M.WYL = 1;
                    StartRTS(_RTWYL);
                }
                case 1:
                {
                    if(Mf_KCW_PMP) //wybieg po grzaniu w obiegu CW?
                    {
                        if(RTS(_RTWYL) >= _TbtWBW0) //uplynol minimalny wybieg pompy?
                        {
                            if(!DtCOf_err_CO && DtCOf_ptwp_CO) //temperatura CO osiagnela prog wylaczenia?
                            {
                                M.WYL = 2;
                                return;
                            }
                        }
                        if(RTS(_RTWYL) < _TbtWBW1) return; //uplynol maksymalny wybieg pompy?
                        M.WYL = 2;
                        return;
                    }
                    else //wybieg po grzaniu w obiegu CO
                    {
                        if(RTS(_RTWYL) < _TWBCO) return; //uplynol maksymalny wybieg pompy?
                        M.WYL = 2;
                    }
                }
                case 2:
                {
                    if(RdPrt(S_ON_PMP)) return; //przekroczone wartosci graniczne temp w obiegu CO
                    if(!Mf_KCW_PMP) //wybieg po grzaniu w obiegu CO?
                    {
                        if(!DtCOf_err_CO && !DtCOf_su_CO) return; //osiagnieto wartosc wylaczenia po pracy w ukladzie CO?
                    }
                    if(DtKNF.robg == _OTWARTY) //z obiegiem otwartym?
                    {
                        if(Mf_RUSZ_POMPE) return; //oczekiwanie na powrot czujnika przeplywu WG do prawidlowej pozycji
                    }
                    Mf_KCW_PMP = 0;
                    Mf_WYLACZ = 0;
                    M.WYL = 0;
                }
            }
            return;
        }
        case _MONOHB:
        {
            switch(M.WYL)
            {
                case 0:
                {
                    M.WYL = 1;
                    StartRTS(_RTWYL);
                }
                case 1:
                {
                    if(Mf_RPM_CW) //obieg CW
                    {
                        if(RTS(_RTWYL) >= _TmnWBW0) //uplynol minimalny wybieg pompy?
                        {
                            if(!DtCOf_err_CO && DtCOf_ptwp_CO) //temperatura CO osiagnela prog wylaczenia?
                            {
                                M.WYL = 2;
                                return;
                            }
                        }
                        if(RTS(_RTWYL) < _TmnWBW1) return; //uplynol maksymalny wybieg pompy?
                        M.WYL = 2;
                        return;
                    }
                    else //obieg CO
                    {
                        if(RTS(_RTWYL) < _TWBCO) return; //uplynol maksymalny wybieg pompy?
                        M.WYL = 2;
                    }
                }
                case 2:
                {
                    if(!Mf_RPM_CW) //obieg CO
                    {
                        if(RdPrt(S_ON_PMP)) return; //przekroczone wartosci graniczne temp w obiegu CO
                        if(!DtCOf_err_CO && !DtCOf_su_CO) return; //osiagnieto wartosc wylaczenia po pracy w ukladzie CO?
                    }
                    else
                    {
                        if(!DtCOf_err_CO && !DtCOf_ptwp_CO && Mf_pob_CW) return; //obieg CW, trwa wymuszenie CW a temp CO>od progu zalaczenia
                    }
                    if(DtKNF.robg == _OTWARTY) //z obiegiem otwartym?
                    {
                        if(Mf_RUSZ_POMPE) return; //oczekiwanie na powrot czujnika przeplywu WG do prawidlowej pozycji
                    }
                    if(!Mf_ZMKCOCW) _POMPA = 0;
                    Mf_WYLACZ = 0;
                    M.WYL = 0;
                }
            }
            return;
        }
        case _UNICO:
        {
            switch(M.WYL)
            {
                case 0:
                {
                    M.WYL = 1;
                    StartRTS(_RTWYL);
                }
                case 1:
                {
                    if(Mf_RPM_CW) //obieg CW
                    {
                        if(RTS(_RTWYL) < _TzsWBW) return; //uplynol maksymalny wybieg pompy?
                        if(!DtCOf_err_CO)
                        {
                            if(DtCOf_ptwp_CO) //temperatura CO osiagnela prog wylaczenia?
                            {
                                M.WYL = 2;
                            }
                            return;
                        }
                        else
                        {
                            M.WYL = 2;
                        }
                    }
                    else //obieg CO
                    {
                        if(RTS(_RTWYL) < _TWBCO) return; //uplynol maksymalny wybieg pompy?
                        M.WYL = 2;
                    }
                }
                case 2:
                {
                    if(Mf_RPM_CW) //obieg CW
                    {
                        if(RdPrt(S_ON_PMP_CW)) return; //przekroczone wartosci graniczne temp w obiegu CW
                    }
                    else //obieg CO
                    {
                        if(RdPrt(S_ON_PMP)) return; //przekroczone wartosci graniczne temp w obiegu CO
                        if(!DtCOf_err_CO && !DtCOf_su_CO) return; //osiagnieto wartosc wylaczenia po pracy w ukladzie CO?
                    }
                    if(DtKNF.robg == _OTWARTY) //z obiegiem otwartym?
                    {
                        if(Mf_RUSZ_POMPE) return; //oczekiwanie na powrot czujnika przeplywu WG do prawidlowej pozycji
                    }
                    if(!Mf_ZMKCOCW)
                    {
                        _POMPA = 0;
                    }
                    Mf_WYLACZ = 0;
                    M.WYL = 0;
                }
            }
            return;
        }
        default: //domyslny monotermiczny
        {
            switch(M.WYL)
            {
                case 0:
                {
                    M.WYL = 1;
                    StartRTS(_RTWYL);
                }
                case 1:
                {
                    if(Mf_RPM_CW) //obieg CW
                    {
                        if(RTS(_RTWYL) >= _TmnWBW0) //uplynol minimalny wybieg pompy?
                        {
                            if(!DtCOf_err_CO && DtCOf_ptwp_CO) //temperatura CO osiagnela prog wylaczenia?
                            {
                                M.WYL = 2;
                                return;
                            }
                        }
                        if(RTS(_RTWYL) < _TmnWBW1) return; //uplynol maksymalny wybieg pompy?
                        M.WYL = 2;
                        return;
                    }
                    else //obieg CO
                    {
                        if(RTS(_RTWYL) < _TWBCO) return; //uplynol maksymalny wybieg pompy?
                        M.WYL = 2;
                    }
                }
                case 2:
                {
                    if(!Mf_RPM_CW) //obieg CO
                    {
                        if(RdPrt(S_ON_PMP)) return; //przekroczone wartosci graniczne temp w obiegu CO
                        if(!DtCOf_err_CO && !DtCOf_su_CO) return; //osiagnieto wartosc wylaczenia po pracy w ukladzie CO?
                    }
                    else
                    {
                        if(!DtCOf_err_CO && !DtCOf_ptwp_CO && Mf_pob_CW) return; //obieg CW, trwa wymuszenie a temp CO>od progu zalaczenia
                    }
                    if(DtKNF.robg == _OTWARTY) //z obiegiem otwartym?
                    {
                        if(Mf_RUSZ_POMPE) return; //oczekiwanie na powrot czujnika przeplywu WG do prawidlowej pozycji
                    }
                    if(!Mf_ZMKCOCW) _POMPA = 0;
                    Mf_WYLACZ = 0;
                    M.WYL = 0;
                }
            }
            return;
        }
    }
}
//------------------
//------------------
//Wybieg wentylatora

void VWybieg(void)
{
    switch(M.VWB)
    {
        case 0:
        {
            M.VWB = 1;
            Mf_DE_VNT = 1;
            StartRTS(_RTVWB);
        }
        case 1:
        {
            if(RTS(_RTVWB) >= _TVWB0) Mf_DE_VNT = 0; //wyzeruj znacznik probnego wybiegu wentylatora
            if(RTS(_RTVWB) < _TVWB1) return;

            _VNT = 0; //wylacz wentylator
            Mf_DE_VNT = 1;
            M.VWB = 2;
        }
        case 2:
        {
            if(RTS(_RTVWB) < _TVWB1 + 2) return; //beznapieciowy wybieg wentylatora
            Mf_DE_VNT = 0;
            Mf_VWYBIEG = 0;
            M.VWB = 0;
        }
    }
}
//------------------
//------------------
//Detekcja zaniku przeplywu WG (oczekuje przez pewien czas zanim uruchomi procedure rozruchu przeplywu)

void DetekcjaPrzeplywuWG(void)
{
    unsigned char pmp, prz;

    pmp = (_POMPA) ? 1 : 0;
    prz = (RdPrt(S_PRZ_CO)) ? 1 : 0;

    if(pmp) //pompa pracuje?
    {
        if(!prz && !Mf_DE_PMP) //zmiana stanu przep?ywu?
        {
            Mf_DE_PMP = 1;
            M.DPR = 0;
        }
        if(Mf_DE_PMP)
        {
            switch(M.DPR)
            {
                case 0:
                {
                    StartRTS(_RTDPR);
                    M.DPR = 1;
                }
                case 1:
                {
                    if(prz) //dla przeplywu zakoncz procedure
                    {
                        Mf_DE_PMP = 0;
                        M.DPR = 0;
                        return;
                    }
                    if(RTS(_RTDPR) < _TDPR1) return; //oczekuj przez zadany czas na przep?yw WG
                    if(!Mf_RUSZ_POMPE)
                    {
                        M.RPR = 0;
                        Mf_RUSZ_POMPE = 1;
                    }
                    Mf_DE_PMP = 0;
                    M.DPR = 0;
                }
            }
        }
    }
    else
    {
        Mf_DE_PMP = 0;
        M.DPR = 0;
    }
}
//------------------
//------------------
//Oczekiwanie na powrot przeplywu

void OczekujNaPrzeplywWG(void)
{
    unsigned char pmp, prz;

    pmp = (_POMPA) ? 1 : 0;
    prz = (RdPrt(S_PRZ_CO)) ? 1 : 0;

    switch(M.RPR)
    {
        case 0:
        {
            M.RPR = 1;
            Mf_prb_PRZ = 1;
            StartRTS(_RTRPR);
        }
        case 1:
        {
            if(prz && pmp) //powrocil przeplyw?
            {
                M.RPR = 0;
                Mf_RUSZ_POMPE = 0;
                Mf_prb_PRZ = 0;
                return;
            }
            if(RTS(_RTRPR) < _TRPR1) return; //czekaj na powrot przeplywu
            Mf_RUSZ_POMPE = 0;
            M.ERR_BTY = 0x05;
            SetErr(0x05);//PrintErr(0x05, 1);
            ErrPTG(); //wylaczenie z blokada
        }
    }
}
//------------------
//------------------
//Zmiana kierunku obrotow pompy

void ZmienKCOKCW(void)
{
    switch(DtKNF.tpkt) //dla danego rodzaju kotla
    {
        case _BITERM: //bitermiczny (miniterm)
        case _BITERM2: //bitermiczny (maxiterm)
        {
            switch(M.ZKP)
            {
                case 0:
                {
                    _POMPA = !_POMPA; //zalacz/wylacz pompe
                    M.ZKP = 1;
                    StartRTdS(_RTZKP);
                }
                case 1:
                {
                    if(RTdS(_RTZKP) < _TZKP / 2) return;
                    Mf_ZMKCOCW = 0;
                    M.ZKP = 0;
                }
            }
            return;
        }
        case _UNICO:
        case _MONOHB:
        {
            switch(M.ZKP)
            {
                case 0:
                {
                    M.ZKP = 1;
                    _POMPA = 0; //wylacz pompe
                    StartRTdS(_RTZKP);
                }
                case 1:
                {
                    if(RTdS(_RTZKP) < _TZKP / 2) return;
                    _KCO_CW = !_KCO_CW; //zmian kierunku obrotu pompy
                    M.ZKP = 2;
                }
                case 2:
                {
                    if(RTdS(_RTZKP) < _TZKP) return;
                    Mf_ZMKCOCW = 0;
                    M.ZKP = 0;

                    //warunek ponownego zalaczenia pompy
                    if(Mf_ROZRUCH || _GRZEJ || Mf_WYLACZ) _POMPA = 1; //wlacz pompe
                    if(DtKNF.robg == _OTWARTY) //uniko z obiegiem otwartym?
                    {
                        if(Mf_RUSZ_POMPE) _POMPA = 1; //trwa oczekiwanie na powrot czujnika przeplywu WG do prawidlowej pozycji
                    }
                }
            }
            return;
        }
        default: //domyslny monotermiczny
        {
            switch(M.ZKP)
            {
                case 0:
                {
                    M.ZKP = 1;
                    _POMPA = 0; //wylacz pompe
                    StartRTdS(_RTZKP);
                }
                case 1:
                {
                    if(RTdS(_RTZKP) < _TZKP / 2) return;
                    _KCO_CW = !_KCO_CW; //zmian kierunku obrotu pompy
                    M.ZKP = 2;
                }
                case 2:
                {
                    if(RTdS(_RTZKP) < _TZKP) return;
                    Mf_ZMKCOCW = 0;
                    M.ZKP = 0;

                    //warunek ponownego zalaczenia pompy
                    if(Mf_ROZRUCH || _GRZEJ || Mf_WYLACZ) _POMPA = 1; //wlacz pompe
                    if(DtKNF.robg == _OTWARTY) //uniko z obiegiem otwartym?
                    {
                        if(Mf_RUSZ_POMPE) _POMPA = 1; //trwa oczekiwanie na powrot czujnika przeplywu WG do prawidlowej pozycji
                    }
                }
            }
            return;
        }
    }
}
//------------------
//------------------
//Awaryjny rozruch plomienia

void RuszPlomien(void)
{
    switch(M.RPL)
    {
        case 0:
        {
            M.RPL = 1;
            StartRTS(_RTRPL);
        }
        case 1:
        {
            if(RTS(_RTRPL) < _TPLPR) return;
            M.LICZNIK_PL++;
            if(Mf_RUSZ_RPLM) //procedura aktywowana w trakcie rozruchu?
            {
                if(!PWMf_tpGZ) //gaz ziemny
                {
                    //x++;
                    if(M.LICZNIK_PL >= (_NCYKLI_PL - 1))
                    {
                        Mf_OST_PL = 1;
                        Mf_RUSZ_RPLM = 0;
                    }
                }
                else
                {
                    if(M.LICZNIK_PL >= (_NCYKLI_GPL - 1))
                    {
                        Mf_OST_PL = 1;
                        Mf_RUSZ_RPLM = 0;
                    }
                }
            }
            else
            {
                if(!PWMf_tpGZ) //gaz ziemny
                {
                    if(M.LICZNIK_PL >= _NCYKLI_PL)
                    {
                        Mf_OST_PL = 1;
                        Mf_RUSZ_RPLM = 0;
                    }
                }
                else
                {
                    if(M.LICZNIK_PL >= _NCYKLI_GPL)
                    {
                        Mf_OST_PL = 1;
                        Mf_RUSZ_RPLM = 0;
                    }
                }
            }
            StartRTdS(_RTRSPL);
            Mf_RUSZ_PLOMIEN = 0;
            M.RPL = 0;
        }
    }
}
//Zerowanie procedury awaryjnego rozruchu plomienia

void ReloadPlomien(void)
{
    if(!Mf_RUSZ_PLOMIEN && !Mf_ZMKCOCW && !Mf_ROZRUCH && (RTdS(_RTRSPL) > 9))
    {
        Mf_RUSZ_RPLM = 0;
        Mf_OST_PL = 0;
        M.LICZNIK_PL = 0;
    }
}
//------------------
//------------------
//Awaryjny rozruch ciagu

void RuszCiag(void)
{
    if(!Mf_TURBO) //sterownik atmosferyczny
    {
        switch(M.RCI)
        {
            case 0:
            {
                M.RCI = 1;
                Mf_prb_CIG = 0;
                StartRTS(_RTRCI);
                StartRTM(_RTRCI2);
            }
            case 1:
            {
                if(RTS(_RTRCI) > 1) Mf_prb_CIG = 1;
                if(RTM(_RTRCI2) < _TCIPR2) return;
                M.LICZNIK_CIG2++;
                if(M.LICZNIK_CIG2 >= _NCYKLI_CIG2)
                {
                    M.LICZNIK_CIG2 = 0;
                    M.ERR_BTY = 0x03;
                    SetErr(0x03);//PrintErr(0x03, 1);
                    ErrPTG();
                }
                M.RCI = 0;
                Mf_RUSZ_CIAG = 0;
                Mf_prb_CIG = 0;
                StartRTM(_RTRCI3);
                return;
            }
        }
    }
    else //sterownik turbo
    {
        switch(M.RCI)
        {
            case 0:
            {
                Mf_prb_CIG = 0;
                StartRTS(_RTRCI);
                if(_VNT) //wentylator pracuje?
                {
                    Mf_VWYBIEG = 1; //zastartuj wybieg wentylatora
                    M.VWB = 0;
                    M.RCI = 1;
                }
                else M.RCI = 2;
                break;
            }
            case 1: //wentylator pracuje - proba styku
            {
                if(RTS(_RTRCI) > 1) Mf_prb_CIG = 1;
                //dla kotla turbo
                if(RdPrt(S_CIG_NO)) //pojawil sie sygnal ciagu?
                {
                    M.RCI = 4;
                    M.LICZNIK_CIG3 = 0; //wyzeruj licznik prob
                    StartRTS(_RTRCI);
                    break;
                }
                if(RTS(_RTRCI) < _TCIPR0 - 1) return;
                M.RCI = 3;
                StartRTS(_RTRCI);
                break;
            }
                //---------------------------------
            case 2: //wentylator nie pracuje - proba styku
            {
                if(RTS(_RTRCI) > 1) Mf_prb_CIG = 1;
                //dla kotla turbo
                if(!RdPrt(S_CIG_NO)) //zanikl sygnal ciagu
                {
                    M.RCI = 4;
                    StartRTS(_RTRCI);
                    break;
                }
                if(RTS(_RTRCI) < _TCIPR0 - 1) return;
                M.ERR_BTY = 0x03;
                SetErr(0x03);//PrintErr(0x03, 1);
                ErrPTG();
            }
                //-----------------------------------
            case 3: //blokada 5 sek.
            {
                _VNT = 0; //stop wentylatora
                if(RTS(_RTRCI) < _TCIBLK - 1) return;
                M.LICZNIK_CIG3++;
                if(M.LICZNIK_CIG3 >= _NCYKLI_CIG3) //ostatnia proba?
                {
                    M.LICZNIK_CIG3 = 0;
                    M.ERR_BTY = 0x03;
                    SetErr(0x03);//PrintErr(0x03, 1);
                    ErrPTG();
                }
                M.RCI = 4;
                StartRTS(_RTRCI);
                break;
            }
            case 4: //podtrzymanie stanu (zapobieganie impulsowemu zalaczeniu)
            {
                if(RTS(_RTRCI) < _TCIPR1) return;
                StartRTS(_RTRSCI2);
                Mf_prb_CIG = 0;
                Mf_RUSZ_CIAG = 0;
                M.RCI = 0;
                return;
            }
        }
    }
}
//------------------
//------------------
//Zerowanie licznuka ponowien awarii ciagu

void ReloadCiag(void)
{
    if(!Mf_TURBO) //sterownik atmosferyczny
    {
        if(!Mf_RUSZ_CIAG && RTM(_RTRCI3) > _TCIPR3) M.LICZNIK_CIG2 = 0; //wyzeruj licznik ponowien procedury po 60min.
    }
    else
    {
        if(RTS(_RTRSCI) > _TCIRLP) //(turbo) po minucie wyzeruj licznik_zaników_ci?gu/min
        {
            M.LICZNIK_CIG = 0;
        }
        if(!Mf_RUSZ_CIAG && (RTS(_RTRSCI2) > _TCITRS)) //(turbo) po minucie wyzeruj licznik_prob_ci?gu/min
        {
            M.LICZNIK_CIG3 = 0;
        }
    }
}
//------------------
//------------------
//Awaryjny rozruch pompy

void Proba48(void)
{
#if _SRV48H==0
    if(Mf_RUSZ_48 || Mf_RUSZ_48Z3) return;
    if(_POMPA) StartRTG(_RTR48);
    else
    {
        if(RTG(_RTR48) >= _TPR48)
        {
            M.R48 = 0;
            Mf_RUSZ_48 = 1;
            StartRTG(_RTR48);
        }
    }
#else
    if(Mf_RUSZ_48 || Mf_RUSZ_48Z3) return;
    if(_POMPA) StartRTM(_SRVRTANL);
    else
    {
        if(RTM(_SRVRTANL) >= 10)
        {
            M.R48 = 0;
            Mf_RUSZ_48 = 1;
            StartRTM(_SRVRTANL);
        }
    }
#endif
}

//------------------
//------------------
//Wybieg pompy po 48h

void Rusz48(void)
{
    switch(DtKNF.tpkt) //dla danego rodzaju kotla
    {
        case _BITERM: //bitermiczny (miniterm)
        case _BITERM2: //bitermiczny (maxiterm)
        {
            switch(M.R48)
            {
                case 0:
                {
                    if(Mf_ZMKCOCW) return; //odczekaj az zakonczy sie procedura zmiany obiegu
                    StartRTS(_RTWB48);
                    ToObiegCO(); //zalacz pompe
                    M.R48 = 1;
                }
                case 1:
                {
                    if(RTS(_RTWB48) < _TWB48) return;
                    if(Mf_ZMKCOCW) return; //odczekaj az zakonczy sie procedura zmiany obiegu
                    ToObiegCW(); //wylacz pompe
                    Mf_RUSZ_48 = 0;
                    M.R48 = 0;
                }
            }
            break;
        }
        case _UNICO:
        case _MONOHB:
        {
            switch(M.R48)
            {
                case 0:
                {
                    ToObiegCO();
                    if(Mf_ZMKCOCW) return; //odczekaj az zakonczy sie procedura zmiany obiegu
                    StartRTS(_RTWB48);
                    _POMPA = 1; //zalacz pompe
                    M.R48 = 1;
                }
                case 1:
                {
                    if(RTS(_RTWB48) < _TWB48) return;
                    if(Mf_ZMKCOCW) return; //odczekaj az zakonczy sie procedura zmiany obiegu
                    _POMPA = 0; //wylacz pompe
                    Mf_RUSZ_48 = 0;
                    M.R48 = 0;
                }
            }
            break;
        }
        default: //domyslny monotermiczny
        {
            switch(M.R48)
            {
                case 0:
                {
                    ToObiegCO();
                    if(Mf_ZMKCOCW) return; //odczekaj az zakonczy sie procedura zmiany obiegu
                    StartRTS(_RTWB48);
                    _POMPA = 1; //zalacz pompe
                    M.R48 = 1;
                }
                case 1:
                {
                    if(RTS(_RTWB48) < _TWB48) return;
                    if(Mf_ZMKCOCW) return; //odczekaj az zakonczy sie procedura zmiany obiegu
                    _POMPA = 0; //wylacz pompe
                    Mf_RUSZ_48 = 0;
                    M.R48 = 0;
                }
            }
            break;
        }
    }
}
//-----------------------------------
//Awaryjny rozruch pompy

void Proba48Z3(void)
{
#if _SRV48H==0
    switch(DtKNF.tpkt) //dla danego rodzaju kotla
    {
        case _MONOHB: //uklady sterowania z hydroblokiem?
        {
            if(Mf_RUSZ_48Z3 || Mf_RUSZ_48) return;
            if(_POMPA && !_KCO_CW) StartRTG(_RTR48Z3); //pompa pracuje w obiegu WU?
            else
            {
                if(_POMPA || Mf_ZMKCOCW) return;
                if(RTG(_RTR48Z3) >= _TPR48Z3)
                {
                    M.R48Z3 = 0;
                    Mf_RUSZ_48Z3 = 1;
                    StartRTG(_RTR48Z3);
                }
            }
            break;
        }
    }
#else
    switch(DtKNF.tpkt) //dla danego rodzaju kotla
    {
        case _MONOHB: //uklady sterowania z hydroblokiem?
        {
            if(Mf_RUSZ_48Z3 || Mf_RUSZ_48) return;
            if(_POMPA && !_KCO_CW) StartRTM(_RTMAL); //pompa pracuje w obiegu WU?
            else
            {
                if(_POMPA || Mf_ZMKCOCW) return;
                if(RTM(_RTMAL) >= 9)
                {
                    M.R48Z3 = 0;
                    Mf_RUSZ_48Z3 = 1;
                    StartRTM(_RTMAL);
                }
            }
            break;
        }
    }
#endif
}
//------------------
//------------------
//Wybieg pompy po 48h dla WU

void Rusz48Z3(void)
{
    switch(DtKNF.tpkt) //dla danego rodzaju kotla
    {
        case _MONOHB:
        {
            switch(M.R48Z3)
            {
                case 0:
                {
                    ToObiegCW();
                    if(Mf_ZMKCOCW) return; //odczekaj az zakonczy sie procedura zmiany obiegu
                    StartRTS(_RTWB48);
                    _POMPA = 1; //zalacz pompe
                    M.R48Z3 = 1;
                }
                case 1:
                {
                    if(RTS(_RTWB48) < _TWB48Z3) return;
                    if(Mf_ZMKCOCW) return; //odczekaj az zakonczy sie procedura zmiany obiegu
                    _POMPA = 0; //wylacz pompe
                    Mf_RUSZ_48Z3 = 0;
                    M.R48Z3 = 0;
                }
            }
            break;
        }
    }
}
//------------------
//------------------
//------------------
//------------------
//------------------
//------------------
//wygaszenie diod roboczych
/*void ClearDiode(void)
{
        SetDiode(_DPL,0);
        SetDiode(_DCO,0);
        SetDiode(_DCW,0);
}*/
//------------------
//------------------
//Reakcja na bledne podlaczenie do sieci
/*void ErrDTF(void)
{
        PrintErr(0x11,1);
        ClearDiode();
        while(1)
        {
                CLRWDT();
                if(!RdPrt(S_DTF)) 			//prawidlowe podlaczenie fazy?
                {
                        EndErr();				//wygas kod bledu
                        return;					//wyjdz z petli
                }
        }
}*/

//------------------
//------------------

bit KeyRes(void)
{
    if(RdPrt(S_OFF))
    {
        PFNf_res = 1;
    }
    else PFNf_res = 0;
    switch(M.KRS)
    {
        case 0:
        {
            if(PFNf_res)
            {
                M.KRS = 1;
                StartRTS(_RTSRV0);
            }
            break;
        }
        case 1:
        {
            if(PFNf_res)
            {
                if(RTS(_RTSRV0) < _TKRES) return 0;
                return 1;

            }
            else
            {
                M.KRS = 0;
            }
        }
    }
    return 0;
}
//------------------
//------------------
//Przerwa czasowa na wypadek stanow nieustalonych przy zaniku napiecia

void VZPauza(void)
{
    //ClsScr();
    StartRTS(_RTROZ);
    while(1)
    {
        CLRWDT();
        DecWSW();
        if(M.ERR_BTY == 2)
        {
            if(RTS(_RTROZ) >= 3)
            {
                //EndClsScr();
                return;
            }
        }
        else
        {
            if(RTS(_RTROZ) >= 2)
            {
                //EndClsScr();
                return;
            }
        }
    }
}
//------------------
//------------------
//Petla wylaczenia awaryjnego

void ErrPTG(void)
{
    GrzejOFF(); //_GRZEJ=0;
    VZPauza();
    if(DtKNF.robg == _OTWARTY) //uklad z obiegiem otwartym?
    {
        if(M.ERR_BTY == 5) //dozwolony kod bledu?
        {
            eeprom_write(_ADRERR, M.ERR_BTY);
        }
    } //przerwa czasowa na wypadek stanow nieustalonych
    if((M.ERR_BTY >= 1) && (M.ERR_BTY <= 3)) eeprom_write(_ADRERR, M.ERR_BTY); //WriteEEPROM(_ADRERR,M.ERR_BTY); 	//zapis kodu bledu do pamieci
    Mf_RUSZ_POMPE = 0;
    Mf_ROZRUCH = 0;
    Mf_MODULUJ = 0;
    WritePWM(0);
    if(!Mf_WYLACZ) //aktywacja wybiegu pompy
    {
        M.WYL = 0;
        switch(DtKNF.tpkt) //dla danego rodzaju kotla
        {
            case _BITERM: //bitermiczny (miniterm)
            case _BITERM2: //bitermiczny (maxiterm)
            {
                if(_POMPA) Mf_WYLACZ = 1;
                break;
            }
            case _UNICO:
            case _MONOHB:
            {
                if(Mf_ZMKCOCW) Mf_WYLACZ = 1;
                else
                    if(_POMPA) Mf_WYLACZ = 1;
                break;
            }
            default: //domyslny monotermiczny
            {
                if(Mf_ZMKCOCW) Mf_WYLACZ = 1;
                else
                    if(_POMPA) Mf_WYLACZ = 1;
                break;
            }
        }
    }
    if(Mf_TURBO && _VNT) Mf_VWYBIEG = 1; //aktywacja wybiegu wentylatora
    Mf_RUSZ_PWR = 0;
    M.HVZ = 0;
    M.KRS = 0;
    //k=0;
    //------
    while(1)
    {
        CLRWDT();
        PFNf_opgprg = 0;
        PFNf_opgres = 1;
        PFNf_ferr = 1;
        //BitSet2(&RSDT1.outSTAT0,_PGPRG,0);
        //BitSet2(&RSDT1.outSTAT0,_PGRES,1);		//znacznik petli wylaczenia awaryjnego
        //BitSet2(&RSDT1.outSTAT1,_ERR,1);

        //ReceptDataLCD();
        //ClearDiode();			//wygaszenie diod roboczych
        //blok pomiarowy
        PomiarPCO(); //wartosc nastawy CO
        PomiarPCW(); //wartosc nastawy CW
        PomiarPFN(); //stan przelacznika funkcji
        PomiarCO(); //wartosc temperatury CO
        PomiarCW(); //wartosc temperatury CW
        PomiarCS(); //wartosc cisnienia w instalacji CO
        //blok decyzyjny
        Proba48(); //zegar 48h wybiegu pompy
        Proba48Z3(); //zegar 48h wybiegu pompy dla WU
        //sygnalizacja stanu pracy dla modulu LCD
        RefInterfaceData();
        DecWSW();
        //wybor obiegu
        switch(DtKNF.tpkt) //dla danego rodzaju kotla
        {
            case _BITERM: //bitermiczny (miniterm)
            case _BITERM2: //bitermiczny (maxiterm)
            {
                //zalaczenie/wylaczenie pompy
                if(!Mf_RUSZ_PWR && !Mf_ZMKCOCW && RdPrt(S_ON_PMP)) ToObiegCO(); //temperatura CO poza dozwolonym zakresem?
                else
                    if(!Mf_ZMKCOCW && !Mf_WYLACZ && !Mf_RUSZ_48) ToObiegCW(); //nieaktywna pompa?
                //aktywacja wybiegu
                if(!Mf_WYLACZ && _POMPA)
                {
                    Mf_WYLACZ = 1; //uaktywnij wybieg pompy							 									//uaktywnij procedure wybiegu pompy
                }
                break;
            }
            case _MONOHB:
            {
                //wybor obiegu
                if(!Mf_ZMKCOCW && RdPrt(S_ON_PMP)) ToObiegCO(); //temperatura CO poza dozwolonym zakresem?
                else
                    if(!Mf_ZMKCOCW && !Mf_WYLACZ && !Mf_RUSZ_48 && (!_POMPA || Mf_RUSZ_48Z3)) ToObiegCW(); //nieaktywna pompa?
                //aktywacja wybiegu, zalaczenie pompy
                if(!Mf_RUSZ_PWR && !Mf_ZMKCOCW && !_POMPA && !Mf_WYLACZ && !Mf_RUSZ_48 && !Mf_RUSZ_48Z3)
                {
                    if(!Mf_RPM_CW && RdPrt(S_ON_PMP)) //temperatura CO/CW poza dozwolonym zakresem?
                    {
                        Mf_WYLACZ = 1; //uaktywnij wybieg pompy							 									//uaktywnij procedure wybiegu pompy
                        _POMPA = 1; //zalacz pompe
                    }
                }
                break;
            }
            case _UNICO:
            {
                //wybor obiegu
                if(!Mf_ZMKCOCW && RdPrt(S_ON_PMP)) ToObiegCO(); //temperatura CO poza dozwolonym zakresem?
                else
                    if(!Mf_ZMKCOCW && RdPrt(S_DZ_CW)) ToObiegCW(); //temperatura dyzurna CW?
                else
                    if(!Mf_ZMKCOCW && !_POMPA && !Mf_WYLACZ && !Mf_RUSZ_48) ToObiegCW(); //nieaktywna pompa?
                //aktywacja wybiegu, zalaczenie pompy
                if(!Mf_RUSZ_PWR && !Mf_ZMKCOCW && !_POMPA && !Mf_WYLACZ && !Mf_RUSZ_48)
                {
                    if((!Mf_RPM_CW && RdPrt(S_ON_PMP)) || (Mf_RPM_CW && RdPrt(S_DZ_CW))) //temperatura CO/CW poza dozwolonym zakresem?
                    {
                        Mf_WYLACZ = 1; //uaktywnij wybieg pompy							 									//uaktywnij procedure wybiegu pompy
                        _POMPA = 1; //zalacz pompe
                    }
                }
                break;
            }
            default: //domyslny monotermiczny
            {
                //wybor obiegu
                if(!Mf_ZMKCOCW && RdPrt(S_ON_PMP)) ToObiegCO(); //temperatura CO poza dozwolonym zakresem?
                else
                    if(!Mf_ZMKCOCW && !_POMPA && !Mf_WYLACZ && !Mf_RUSZ_48) ToObiegCW(); //nieaktywna pompa?
                //aktywacja wybiegu, zalaczenie pompy
                if(!Mf_RUSZ_PWR && !Mf_ZMKCOCW && !_POMPA && !Mf_WYLACZ && !Mf_RUSZ_48)
                {
                    if(!Mf_RPM_CW && RdPrt(S_ON_PMP)) //temperatura CO/CW poza dozwolonym zakresem?
                    {
                        Mf_WYLACZ = 1; //uaktywnij wybieg pompy							 									//uaktywnij procedure wybiegu pompy
                        _POMPA = 1; //zalacz pompe
                    }
                }
                break;
            }
        }
        //blok wykonawczy
        if(Mf_ZMKCOCW) ZmienKCOKCW(); //zmiana kierunku obrotu pompy
        if(Mf_WYLACZ) Wylacz(); //kontrola wybiegu pompy
        if(Mf_VWYBIEG) VWybieg(); //wybieg wentylatora
        if(Mf_RUSZ_48) Rusz48(); //wybieg pompy co 48h
        if(Mf_RUSZ_48Z3) Rusz48Z3(); //wybieg pompy co 48h w obiegu WU

        //obsluga przycisku RESET
        if(KeyRes())
        {
            _VNT = 0;
            _POMPA = 0;
            ToReset();
        }
    }
}
//Wymuszenie resetu sterownika.

void ToReset(void)
{
    _VNT = 0;
    _POMPA = 0;
    ClsScr();
    EndErr();
    PFNf_opgprg = 0;
    PFNf_opgres = 0;
    eeprom_write(_ADRERR, 0x00);
    while(1);
}
//------------------
//------------------

void InitGlobal(void)
{
    //RCON
    IPEN = 1; //enable priority levels
    //INTCON
    GIEH = 1; //enable oll un-masked interrupts
    GIEL = 1; //enable oll un-masked interrupts
}

