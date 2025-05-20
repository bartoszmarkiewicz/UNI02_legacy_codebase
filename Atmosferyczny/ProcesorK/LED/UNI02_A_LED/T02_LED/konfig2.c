/*konfig2.c*/
//Procedury zwiazane z wyborem typu gazu i poziomow
//mocy startowej oraz majksymalnej dla CO i CW
/*---------------------------------------------------------------------------------*/
#include <pic18.h>
#include <htc.h>
#include "global.h"					//parametry globalne
#include "konfig2.h"					//parametry lokalne
#include "eeprom.h"
#include "pomiar.h"
#include "wsw.h"
#include "main.tp.h"
#include "keypad.tp.h"
#include "print.h"
#include "konsola.h"

volatile DtKONF DtKNF;

//extern unsigned char x,y;

//extern MGSRSData MGSD;
extern tpMAIN M;
extern DataPFN PFN;
extern DataCO DtCO; //pomiar.c
extern DataZW DtZW;
extern DataCW DtCW;
extern DataIN DtIN;

//extern tpRSDTA1 RSDT1;
extern StPin ky[];
extern StPin xxky[];

DtPZK PZK[_KNPAR1] ={
    0, 0, 0, 0, 0, 0, 0, 1, //moc startowa
    0, 0, 0, 0, 0, 0, 0, 1, //moc maksymalna CW
    0, 0, 0, 0, 0, 0, 0, 1, //moc maksymalna CO
    0, 0, 0, 0, 0, 0, 1, 1, //wybor rodzaju gazu
    0, 0, 0, 0, 0, 0, 1, 1, //aktywacja zegara antylegionella
    0, 0, 0, 0, 0, 0, 1, 0, //wybor typu kotla
    0, 0, 0, 0, 0, 0, 1, 1, //aktywacja czujnika przeplywu
    0, 0, 0, 0, 0, 0, 1, 1, //ogrzewanie podlogowe
    0, 0, 0, 0, 0, 0, 1, 1, //wybor rodzaju czujnika cisnienia
    0, 0, 0, 0, 0, 0, 1, 1, //rodzaj pompy
    0, 0, 0, 0, 0, 0, 1, 1, //delta T dla pompy
    0, 0, 0, 0, 0, 0, 1, 1, //minimalny wydatek pompy
    0, 0, 0, 0, 0, 0, 1, 1  //maksymalny wydatek pompy            
};
/*---------------------------------------------------------------------------------*/
//Odblokowanie procedury wejsciowej konfiguracji
//po uruchomieniu sterownika na pozycji OFF
//UEAGA: umiescic przed petla glowna

void EnKonf(void)
{
    DtKNFf_enknf = 1; //odblokuj mozliwosc wejscia do konf.(znacznik wysylany w RefLCDData())
    DtKNF.KRK = 0;
    DtKNF.PRM = 0;
    PFNf_enKNF = 1;
    PFNf_aktywne_KNF = 0;
    StartRTS(_RTKONF1); //start zegara dla czasu w ktorym mozna aktywowac procedure konfiguracji
}
/*---------------------------------------------------------------------------------*/
//wyjscie z procedury konfiguracji po modyfikacji parametrow

void ExitKonf(void)
{
    PFNf_aktywne_KNF = 0;
    Mf_AKTYWNE_KNF = 0;
    DtKNFf_wymCO = 0; //wylacz wymuszenie CO
    DtKNFf_wymCW = 0;
    EndPR(); //wygas wskaz parametru
    DtKNF.PRM = 0;
    DtKNF.KRK = 0;
    WaitToLowEx(&ky[S_OFF]);
    WaitToLowEx(&ky[S_SET]);
    PFNf_KnfDisErr = 0;
}
unsigned int nrp = 0;
unsigned char zm, zp;

void UpdateKnfPar(unsigned char par)
{
    switch(par)
    {
        case 0:
            DtKNF.mstr = PZK[0].dta;
            break;
        case 1:
            DtKNF.mxcw = PZK[1].dta;
            break;
        case 2:
            DtKNF.mxco = PZK[2].dta;
            break;
        case 3:
            DtKNF.rdgz = PZK[3].dta;
            break;
        case 4:
            DtKNF.aleg = PZK[4].dta;
            break;
        case 5:
            DtKNF.tpkt = PZK[5].dta;
            if(DtKNF.tpkt==_UNICO)
            {
                PZK[4].shw=1;
            }
            else
            {
                PZK[4].shw=0;        
            }            
            break;
        case 6:
            DtKNF.robg = PZK[6].dta;
            break;
        case 7:
            DtKNF.opdg = PZK[7].dta;
            break;
        case 8:
            DtKNF.tpcs = PZK[8].dta;
            break;
        case 9:
        {
            DtKNF.rdpm = PZK[9].dta;
            if(!DtKNF.rdpm)
            {
                PZK[10].shw=0;
                PZK[11].shw=0;
                PZK[12].shw=0;        
            }
            else
            {
                PZK[10].shw=1;
                PZK[11].shw=1;
                PZK[12].shw=1;          
            }            
            break;
        }
        case 10:
            DtKNF.dtpm = PZK[10].dta;
            break;
        case 11:
            DtKNF.mdpm = PZK[11].dta;
            if(DtKNF.mdpm>DtKNF.mgpm)
            {
                if(DtKNF.mgpm>=_KMINMNP) DtKNF.mdpm=DtKNF.mgpm;
            }
            break;
        case 12:
            DtKNF.mgpm = PZK[12].dta;
            if(DtKNF.mgpm<DtKNF.mdpm)
            {
                if(DtKNF.mdpm<=_KMAXMXP) DtKNF.mgpm=DtKNF.mdpm;
            }    
            break;            
    }
}

void ReadAllPar(void)
{
    PZK[0].dta = PZK[0].bfdta = DtKNF.mstr;
    PZK[0].min = _KMINMST;
    PZK[0].max = _KMAXMST;
    PZK[1].dta = PZK[1].bfdta = DtKNF.mxcw;
    PZK[1].min = _KMINMCW;
    PZK[1].max = _KMAXMCW;
    PZK[2].dta = PZK[2].bfdta = DtKNF.mxco;
    PZK[2].min = _KMINMCO;
    PZK[2].max = _KMAXMCO;
    PZK[3].dta = PZK[3].bfdta = DtKNF.rdgz;
    PZK[3].min = _KMINRGZ;
    PZK[3].max = _KMAXRGZ;
    PZK[4].dta = PZK[4].bfdta = DtKNF.aleg;
    PZK[4].min = _KMINEAL;
    PZK[4].max = _KMAXEAL;
    if(DtKNF.tpkt==_UNICO)
    {
        PZK[4].shw=1;
    }
    else
    {
        PZK[4].shw=0;        
    }    
    PZK[5].dta = PZK[5].bfdta = DtKNF.tpkt;
    PZK[5].min = _KMINWTK;
    PZK[5].max = _KMAXWTK;
    PZK[5].shw = (_KNF) ? 0 : 1;
    if(PZK[5].shw) DtKNF.PRM = nrp = 5; //aktywny parametr - wybor typu kotla
    else DtKNF.PRM = nrp = 0; //aktywny pierwszy parametr
   
    PZK[6].dta = PZK[6].bfdta = DtKNF.robg;
    PZK[6].min = _KMINOBG;
    PZK[6].max = _KMAXOBG;
    PZK[7].dta = PZK[7].bfdta = DtKNF.opdg;
    PZK[7].min = _KMINOGP;
    PZK[7].max = _KMAXOGP;
    PZK[8].dta = PZK[8].bfdta = DtKNF.tpcs;
    PZK[8].min = _KMINTCS;
    PZK[8].max = _KMAXTCS;
    PZK[9].dta = PZK[9].bfdta = DtKNF.rdpm;
    PZK[9].min = _KMINRDP;
    PZK[9].max = _KMAXRDP;
    if(!DtKNF.rdpm)
    {
        PZK[10].shw=0;
        PZK[11].shw=0;
        PZK[12].shw=0;        
    }
    else
    {
        PZK[10].shw=1;
        PZK[11].shw=1;
        PZK[12].shw=1;          
    }
    PZK[10].dta = PZK[10].bfdta = DtKNF.dtpm;
    PZK[10].min = _KMINDLP;
    PZK[10].max = _KMAXDLP;
    PZK[11].dta = PZK[11].bfdta = DtKNF.mdpm;
    PZK[11].min = _KMINMNP;
    PZK[11].max = _KMAXMNP;
    PZK[12].dta = PZK[12].bfdta = DtKNF.mgpm;
    PZK[12].min = _KMINMXP;
    PZK[12].max = _KMAXMXP;    
}
//Obsluga parametrow modyfikowanych dynamicznie

void RunDynamicParam(unsigned char prm, unsigned char ddta)
{
    //Parametry modyfikowane dynamicznie
    switch(prm)
    {
            //moc startowa (0...99)
        case 0:
        {
            DtKNF.mstr = ddta; //dana dynamiczna
            MocStart(DtKNF.mstr);
            DtKNFf_wymCO = 1; //zalacz wymuszenie CO
            DtKNFf_wymCW = 0; //wylacz wymuszenie CW
            break;
        }
            //moc maksymalna WU (0...99)
        case 1:
        {
            DtKNF.mxcw = ddta; //dana dynamiczna
            MocMaksCW(DtKNF.mxcw);
            DtKNFf_wymCO = 0; //wylacz wymuszenie CO
            if(DtKNF.tpkt == _UNICO) //kociol UniCO?
            {
                DtKNFf_wymCW = 1; //zalacz wymuszenie CW niezaleznie od stanu czujnika przeplywu
            }
            else
            {
                if(RdPrt(S_WCW)) DtKNFf_wymCW = 1; //zalacz wymuszenie CW dla aktywnego przeplywu
                else DtKNFf_wymCW = 0;
            }
            break;
        }
            //moc maksymalna WG (0...99)
        case 2:
        {
            DtKNF.mxco = ddta; //dana dynamiczna
            MocMaksCO(DtKNF.mxco);
            DtKNFf_wymCO = 1; //zalacz wymuszenie CO
            DtKNFf_wymCW = 0; //wylacz wymuszenie CW
            break;
        }
    }
}
//Glowna procedura konfiguracji (wykonywana w petli glownej)

void MKonfiguracja(void)
{
    unsigned char key, fst;

    Mf_AKTYWNE_KNF = PFNf_aktywne_KNF;

    //wyzerowanie flagi zezwolenia na aktywacje trybu konfiguracji
    if(PFNf_enKNF && (PFNf_aktywne_KNF || (RTS(_RTKONF1) >= _TKNFDIS))) //konfiguracja juz aktywna lub uplynal czas mozliwosci wejscia do konf.?
    {
        DtKNFf_enknf = 0;
        PFNf_enKNF = 0;
    }

    if(PFNf_aktywne_KNF && ((RTM(_RTKONF3) >= _TKNFEXT) || PFNf_opgres)) //uplynol maksymalny dozwolony czas aktywnosci procedury lub wylaczenie awaryjne z blokada w ukladzie UNI-02?
    {
        ExitKonf();
        return;
    }
    switch(DtKNF.KRK)
    {
            //warunek wejscia do procedury modyfikacji parametrow
        case 0:
        {
            if(!PFNf_off || !PFNf_enKNF) return; //aktywny tryb OFF?

            //przytrzymano przycisk wejscia do procedury konfiguracji
            key = PresKeyEX(RdPrt(S_OFF) && RdPrt(S_SET), &ky[SS_OFF_SET]);

            if(key != 100 && key >= 50)
            {
                WaitToLowEx(&ky[SS_OFF_SET]);
                WaitToLowEx(&ky[SS_OFF]);
                WaitToLowEx(&ky[SS_SET]);
                PFNf_aktywne_KNF = 1;
                ReadAllPar(); //wstepny odczyt aktualnych wartosci parametrow
                StartRTM(_RTKONF3); //start timera aktywnosci proc. konfig
                DtKNF.PRM = 0; //aktywacja pierwszego parametru
                DtKNF.KRK = 1; //aktywacja proc. konfiguracji
            }
            else
            {
                DtKNF.PRM = 0;
                DtKNF.KRK = 0;
                return;
            }
            break;
        }
            //wybor parametru
        case 1:
        {
            DtKNFf_wymCO = 0; //wylacz wymuszenie CO
            DtKNFf_wymCW = 0; //wylacz wymuszenie CO
            PFNf_off = 1;
            PFNf_lato = 0;
            PFNf_zima = 0;
            PFNf_KnfDisErr = 1;
            if(PZK[nrp].shw) //parametr moze byc wyswietlony?
            {
                ReadParam(&nrp, &zm, &zp, 0, _KNPAR1 - 1, 1, 0);
                DtKNF.PRM = nrp;
                if(zm==2||zm==3) RestartPR();
            }
            else
            {
                if(zm == 3) //zmiana w gore?
                {
                    if(nrp < _KNPAR1 - 1) nrp++;
                    else nrp = 0;
                }
                else
                    if(zm == 2) //zmiana w dol?
                {
                    if(nrp > 0) nrp--;
                    else nrp = _KNPAR1 - 1;
                }
                else //gdy pierwsze parametr ukryty
                {
                    if(nrp < _KNPAR1 - 1) nrp++;
                    else nrp = 0;
                }
                break;
            }
            PrintPR(led_P, DtKNF.PRM + 1, 0); //kod rx
            key = PresKeyEX(RdPrt(S_SET), &ky[SS_SET]);
            if(key > 0) //wybrano parametr
            {
                WaitToLowEx(&ky[SS_SET]);
                WaitToLowEx(&ky[SS_OFF]);
                EndPR();
                PZK[DtKNF.PRM].bfdta = PZK[DtKNF.PRM].dta;
                DtKNF.KRK = 2; //tryb modyfikacji parametru
            }
            key = PresKeyEX(RdPrt(S_OFF), &ky[SS_OFF]);
            if(key != 100 && key >= 20)
            {
                WaitToLowEx(&ky[SS_OFF]); //start timera zapisu
                DtKNF.KRK = 3;
            }
            break;
        }
            //modyfikacji wartosci parametru
        case 2:
        {
            PFNf_off = 0;
            PFNf_lato = 1;
            PFNf_zima = 0;
            PFNf_KnfDisErr = PZK[DtKNF.PRM].no_msg;
            if((PZK[DtKNF.PRM].max - PZK[DtKNF.PRM].min) > 10) fst = 1; //decyzja o zmiennej szybkosci inkrementacji/dekrementacji
            else fst = 0;
            ReadParam(&(PZK[DtKNF.PRM].bfdta), &zm, &zp, PZK[DtKNF.PRM].min, PZK[DtKNF.PRM].max, PZK[DtKNF.PRM].rld, fst);
            RunDynamicParam(DtKNF.PRM, (char)PZK[DtKNF.PRM].bfdta); //obsluga parametrow dynamicznych
            PrintWPR((char)PZK[DtKNF.PRM].bfdta);
            key = PresKeyEX(RdPrt(S_SET), &ky[SS_SET]);
            if(key > 0) //wybrano parametr
            {
                WaitToLowEx(&ky[SS_SET]);
                PZK[DtKNF.PRM].zps = (PZK[DtKNF.PRM].dta != PZK[DtKNF.PRM].bfdta);
                PZK[DtKNF.PRM].dta = PZK[DtKNF.PRM].bfdta;
                UpdateKnfPar(DtKNF.PRM);
                DtKNF.KRK = 1;
            }
            key = PresKeyEX(RdPrt(S_OFF), &ky[SS_OFF]);
            if(key > 0)
            {
                WaitToLowEx(&ky[SS_OFF]);
                PZK[DtKNF.PRM].bfdta = PZK[DtKNF.PRM].dta;
                UpdateKnfPar(DtKNF.PRM);
                DtKNF.KRK = 1;
            }
            break;
        }
        case 3:
        {
            SaveKnfPar();
            RdEEParam();
            ExitKonf();
            return;
        }
    }
}

void SaveKnfPar(void)
{
    if((DtKNF.mstr > _KMINMST) && (DtKNF.mstr < _KMAXMST) || DtKNF.mstr == _KMINMST || DtKNF.mstr == _KMAXMST)
        if(DtKNF.mstr != eeprom_read(_ADRMST)) eeprom_write(_ADRMST, DtKNF.mstr);
    if((DtKNF.mxcw > _KMINMCW) && (DtKNF.mxcw < _KMAXMCW) || DtKNF.mxcw == _KMINMCW || DtKNF.mxcw == _KMAXMCW)
        if(DtKNF.mxcw != eeprom_read(_ADRMCW)) eeprom_write(_ADRMCW, DtKNF.mxcw);
    if((DtKNF.mxco > _KMINMCO) && (DtKNF.mxco < _KMAXMCO) || DtKNF.mxco == _KMINMCO || DtKNF.mxco == _KMAXMCO)
        if(DtKNF.mxco != eeprom_read(_ADRMCO)) eeprom_write(_ADRMCO, DtKNF.mxco);
    if((DtKNF.rdgz > _KMINRGZ) && (DtKNF.rdgz < _KMAXRGZ) || DtKNF.rdgz == _KMINRGZ || DtKNF.rdgz == _KMAXRGZ)
    {
        if(DtKNF.rdgz != eeprom_read(_ADRRGZ))
        {
            eeprom_write(_ADRRGZ, DtKNF.rdgz);
            ModParam(DtKNF.tpkt, DtKNF.rdgz); //nowe parametry modulatora dla wybranego typu kotla i rodzaju gazu
        }
    }
    if(DtKNF.aleg == _KMINEAL || DtKNF.aleg == _KMAXEAL)
    {
        if(DtKNF.aleg != eeprom_read(_ADRTAL))
        {
            StartRTG(_RTANL);
            eeprom_write(_ADRRTA, 0);
            eeprom_write(_ADRTAL, DtKNF.aleg);
        }
    }
    if((DtKNF.tpkt > _KMINWTK) && (DtKNF.tpkt < _KMAXWTK) || DtKNF.tpkt == _KMINWTK || DtKNF.tpkt == _KMAXWTK)
    {
        if(DtKNF.tpkt != eeprom_read(_ADRWTK))
        {
            SetTpKtParam(); //ustaw param. ukryte dla wybr. kotla
            if((DtKNF.rdgz > _KMINRGZ) && (DtKNF.rdgz < _KMAXRGZ) || DtKNF.rdgz == _KMINRGZ || DtKNF.rdgz == _KMAXRGZ)
                ModParam(DtKNF.tpkt, DtKNF.rdgz); //nowe parametry modulatora dla wybranego typu kotla i rodzaju gazu
            SetDefaultValue(); //parametry domyslne dla danego typu kotla
            eeprom_write(_ADRWTK, DtKNF.tpkt);
        }
    }
    if(DtKNF.robg == _KMINOBG || DtKNF.robg == _KMAXOBG)
        if(DtKNF.robg != eeprom_read(_ADROBG)) eeprom_write(_ADROBG, DtKNF.robg);
    if(DtKNF.opdg == _KMINOGP || DtKNF.opdg == _KMAXOGP)
        if(DtKNF.opdg != eeprom_read(_ADRPDG)) eeprom_write(_ADRPDG, DtKNF.opdg);
    if(DtKNF.tpcs == _KMINTCS || DtKNF.tpcs == _KMAXTCS)
        if(DtKNF.tpcs != eeprom_read(_ADRCS5)) eeprom_write(_ADRCS5, DtKNF.tpcs);

    if((DtKNF.rdpm > _KMINRDP) && (DtKNF.rdpm < _KMAXRDP) || DtKNF.rdpm == _KMINRDP || DtKNF.rdpm == _KMAXRDP)
        if(DtKNF.rdpm != eeprom_read(_ADRRDPM)) eeprom_write(_ADRRDPM, DtKNF.rdpm);

    if((DtKNF.dtpm > _KMINDLP) && (DtKNF.dtpm < _KMAXDLP) || DtKNF.dtpm == _KMINDLP || DtKNF.dtpm == _KMAXDLP)
        if(DtKNF.dtpm != eeprom_read(_ADRDLPM)) eeprom_write(_ADRDLPM, DtKNF.dtpm);

    if((DtKNF.mdpm > _KMINMNP) && (DtKNF.mdpm < _KMAXMNP) || DtKNF.mdpm == _KMINMNP || DtKNF.mdpm == _KMAXMNP)
    {
        if(DtKNF.mdpm>DtKNF.mgpm)
        {
            if(DtKNF.mgpm>=_KMINMNP) DtKNF.mdpm=DtKNF.mgpm;
        }
        if(DtKNF.mdpm != eeprom_read(_ADRMNPM)) eeprom_write(_ADRMNPM, DtKNF.mdpm);
    }
    if((DtKNF.mgpm>_KMINMXP)&&(DtKNF.mgpm<_KMAXMXP)||DtKNF.mgpm==_KMINMXP||DtKNF.mgpm==_KMAXMXP)
    {
        if(DtKNF.mgpm<DtKNF.mdpm)
        {
            if(DtKNF.mdpm<=_KMAXMXP) DtKNF.mgpm=DtKNF.mdpm;
        }
        if(DtKNF.mgpm!=eeprom_read(_ADRMXPM)) eeprom_write(_ADRMXPM,DtKNF.mgpm);
    }    
}

void SetNST(void)
{
    PomiarZW(); //temperatura z czujnika zewnetrznego
    if(!DtZWf_on_ZW)
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
        Mf_AKTYWNE_PGD = 1;
        SetPrt(S_PGD, 1);
        DtCO.minPCO = _MIN_PGD;
        DtCO.maxPCO = _MAX_PGD;
    }
    if(DtCO.PCO < DtCO.minPCO) DtCO.PCO = DtCO.minPCO;
    if(DtCO.PCO > DtCO.maxPCO) DtCO.PCO = DtCO.maxPCO;
    if(DtCO.PCO != eeprom_read(_ADRPCO)) eeprom_write(_ADRPCO, DtCO.PCO);
}
/*---------------------------------------------------------------------------------*/
//Odczytuje i interpretuje parametry zapisane w pamieci EEPROM

void RdEEParam(void)
{
    //-----
    DtKNF.tpkt = eeprom_read(_ADRWTK); //typ kotla
    SetTpKtParam(); //ustaw param. ukryte dla wybr. typu kotla
    //-----
    ModParam(DtKNF.tpkt, DtKNF.rdgz = eeprom_read(_ADRRGZ)); //parametry modulatora dla wybranego typu kotla i rodzaju gazu
    MocStart(DtKNF.mstr = eeprom_read(_ADRMST)); //moc startowa
    MocMaksCW(DtKNF.mxcw = eeprom_read(_ADRMCW)); //moc maksymalna CW
    MocMaksCO(DtKNF.mxco = eeprom_read(_ADRMCO)); //moc maksymalna CO
    DtKNF.aleg = eeprom_read(_ADRTAL); //aktywacja timera AL
    DtKNF.robg = eeprom_read(_ADROBG); //rodzaj obiegu 0-otwarty,1-zamkniety
    PFN.poz = eeprom_read(_ADRPFN); //pozycja PFN
    SetPFN();
    DtKNF.opdg = eeprom_read(_ADRPDG); //aktywacja trybu ogrzewania podlogowego
    DtCO.PCO = eeprom_read(_ADRPCO); //nastawa PCO
    DtCW.PCW = eeprom_read(_ADRPCW); //nastawa PCW
    SetNST();
    PFN.PCO = DtCO.PCO;
    PFN.PCW = DtCW.PCW;
    DtKNF.tpcs = eeprom_read(_ADRCS5); //alternatywny czujnik cisnienia 5V
    DtKNFf_wymCO = 0; //wylacz wymuszenie CO
    DtKNFf_wymCW = 0; //wylacz wymuszenie CW
    M.LICZNIK_ANL = eeprom_read(_ADRRTA); //wartosc timera AL

    if(M.LICZNIK_ANL)
    {
        if(!DtKNF.aleg || (DtKNF.tpkt != _UNICO)) //nieaktywna funkcja AL?
        {
            eeprom_write(_ADRRTA, 0); //wyzeruj licznik timera aktywacji AL
            M.LICZNIK_ANL = 0;
        }
    }
    //-----
    DtKNF.rdpm = eeprom_read(_ADRRDPM); //rodzaj pompy
    DtKNF.dtpm = eeprom_read(_ADRDLPM); //wielkosc delty T dla pompy
    DtKNF.mdpm = eeprom_read(_ADRMNPM); //dolny prog predkosci dla pompy
	DtKNF.mgpm = eeprom_read(_ADRMXPM);	//gorny prog predkosci dla pompy    
    DtKNF.teco = 0;
    DtIN.Eco = 5;
    
}
/*---------------------------------------------------------------------------------*/
//Ustaw parametry ukryte dla wybranego typu kotla

void SetTpKtParam(void)
{
    switch(DtKNF.tpkt)
    {
        case _UNICO: //zasobnikowy
        {
            DtKNF.czpr = _BRAK;
            //DtKNF.rdpm=_KDWA;
            DtKNF.tpwm = _TERMET;
            break;
        }
        case _BITERM: //bitermiczny (miniterm)
        {
            DtKNF.czpr = _BRAK;
            //DtKNF.rdpm=_KJEDEN;
            DtKNF.tpwm = _TERMET;
            break;
        }
        case _MONOHB: //monotermiczny z hydroblokiem
        {
            DtKNF.czpr = _BRAK;
            //DtKNF.rdpm=_KDWA;
            DtKNF.tpwm = _TERMET;
            break;
        }
        case _BITERM2: //bitermiczny (maxiterm)
        {
            DtKNF.czpr = _BRAK;
            //DtKNF.rdpm=_KJEDEN;
            DtKNF.tpwm = _TERMET;
            break;
        }
        default: //domyslny monotermiczny
        {
            DtKNF.czpr = _BRAK;
            //DtKNF.rdpm=_KDWA;
            DtKNF.tpwm = _TERMET;
            break;
        }
    }
}
/*---------------------------------------------------------------------------------*/
//ustaw domyslne wartosci wybranych parametrów po zmianie typu kotla

void SetDefaultValue(void)
{
    switch(DtKNF.tpkt)
    {
        case _MONOHB: //monotermiczny z hydroblokiem
        {
            DtKNF.mstr = 15;
            eeprom_write(_ADRMST, DtKNF.mstr);
            break;
        }
        default:
        {
            DtKNF.mstr = 40;
            eeprom_write(_ADRMST, DtKNF.mstr);
            break;
        }
    }
}
/*---------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------*/
void SetPFN(void)
{
    if(PFN.poz == _PFOFF)
    {
        PFNf_zima = 0;
        PFNf_lato = 0;
        PFNf_off = 1;
        PFNf_csn = 0;
    }
    else
        if(PFN.poz == _PFZMA)
    {
        PFNf_zima = 1;
        PFNf_lato = 0;
        PFNf_off = 0;
        PFNf_csn = 0;
    }
    else
        if(PFN.poz == _PFLTO)
    {
        PFNf_zima = 0;
        PFNf_lato = 1;
        PFNf_off = 0;
        PFNf_csn = 0;
    }
    else
    {
        PFNf_zima = 1;
        PFNf_lato = 0;
        PFNf_off = 0;
        PFNf_csn = 0;
        PFN.poz = _PFZMA;
    }
}
/*---------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------*/
void WriteEEPCW(void)
{
    eeprom_write(_ADRPCW, DtCW.PCW);
}

unsigned char ReadEEPCW(void)
{
    return eeprom_read(_ADRPCW);
}
/*---------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------*/
void WriteEEPCO(void)
{
    eeprom_write(_ADRPCO, DtCO.PCO);
}

unsigned char ReadEEPCO(void)
{
    return eeprom_read(_ADRPCO);
}
/*---------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------*/
void WriteEEPFN(void)
{
    eeprom_write(_ADRPFN, PFN.poz);
}

unsigned char ReadEEPFN(void)
{
    return eeprom_read(_ADRPFN);
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
