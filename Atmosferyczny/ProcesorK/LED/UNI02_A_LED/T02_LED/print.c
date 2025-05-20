/*---------------------------------------------------------------------------------*/
/*print.c*/
/*Funkcje zapisu danych do 2 segmentowego wyswietlacza typu LED*/
/*w oparciu o procedury obslugi wyswietlacza zawarte w pliku wsw.c*/
/*Sposob reprezentacji danych - ITALY*/
/*---------------------------------------------------------------------------------*/
#include <pic18.h>
#include "global.h"				//parametry globalne
#include "print.h"				//parametry lokalne
#include "pomiar.tp.h"
#include "main.tp.h"
//wsw.c
extern WPack PWD[]; //tablica priorytetow wyswietlania PWD[n] (n=0 - najwyzszy priorytet)
extern WPack DWD[]; //tablica danych modyfikowanych dynamicznie wystepujacych wylacznie w kolejkach
extern const unsigned char Znak[];
extern DataCO DtCO; //pomiar.c
extern DataPFN PFN;
//---------------------------------------------------------
//---------------------------------------------------------
//Funkcje wyswietlania wg waznosci.
//ClsScr
//-PrintErr
//--PrintPR,PrintWPR
//---PrintNstCW
//----PrintNstCO
//----PrintNstKT
//-----PrintMsg
//------PrintTDZ
//-------PrintOFF
//--------PrintCS
//---------PrintCW
//----------PrintCO

//---------------------------------------------------------
//---------------------------------------------------------
//Deaktywacja (wygaszenie) wyswietlacza

void ClsScr(void)
{
    WswOff();
}
//Aktywacja wyswietlacza, ustawienie wszystkich kolejek na pozycji pierwszej

void EndClsScr(void)
{
    AllKolToFirstPoz();
    WswOn();
}
//---------------------------------------------------------
//---------------------------------------------------------
//Ustaw znacznik kodu bledu dla procedury DecWsw()

void SetErr(const unsigned char kod)
{
    PFNf_ferr = 1;
    PFN.KERR = kod;
}
//---------------------------------------------------------
//---------------------------------------------------------
//Ustaw znacznik komunikatu awaryjnego dla procedury DecWsw()

void SetMsg(const unsigned char kod)
{
    PFNf_fmsg = 1;
    PFN.KAWR = kod;
}
//---------------------------------------------------------
//---------------------------------------------------------
//wyswietla migajacy kod bledu
//kod - parametr okreslajacy migajacy kod bledu do wyswietlenia
//pls - dla 1 dioda error miga, dla 0 dioda error zapalona
//cw - kropka przy mlodszej cyfrze
void PrintErr(const unsigned char kod, const unsigned char pls)
{
    SetData(&DWD[0], 0, kod);
    SetData(&DWD[1], 0, kod);
    SetStatus(&DWD[0], tHEX | tSDID | _DKM, tALL);
    if(pls) SetStatus(&DWD[1], tHEX | tRDID | _DKM, tALL); //migajacy kod bledu
    else SetStatus(&DWD[1], tHEX | tSDID | _DKM, tALL); //staly kod bledu
    SetPoz(_WS_ERR, 0);
    SetPoz(_WS_ERR, 1);
}

void PrintErrCs(const unsigned char kod, const unsigned char cs, const unsigned char pls)
{
    SetData(&DWD[0], 0, kod);
    SetData(&DWD[1], 0, cs);
    SetStatus(&DWD[0], tHEX | tSDID | _DKM, tALL); //kod bledu/cisnienie

    if(pls) SetStatus(&DWD[1], tDEC | tKROPKA1 | tRDID | _DKM, tALL); //cisnienie
    else SetStatus(&DWD[1], tDEC | tKROPKA1 | tSDID | _DKM, tALL);
    SetPoz(_WS_ERR, 0);
    SetPoz(_WS_ERR, 1);
}

void PrintErrTm(const unsigned char kod, const unsigned char tm, const unsigned char pls, const unsigned char cw)
{
    unsigned int fl;
    fl=(cw)?tKROPKA0:0;
    SetData(&DWD[0], 0, kod);
    SetData(&DWD[1], 0, tm);
    SetStatus(&DWD[0], tHEX | tSDID | _DKM, tALL); //kod bledu/temperatura
    if(pls) SetStatus(&DWD[1], fl | tDEC | tRDID | _DKM, tALL); //temperatura
    else SetStatus(&DWD[1], fl | tDEC | tSDID | _DKM, tALL);
    SetPoz(_WS_ERR, 0);
    SetPoz(_WS_ERR, 1);
}
//usuwa migajacy kod bledu

void EndErr(void)
{
    PFNf_fmsg = 0;
    PFNf_ferr = 0;
    PFN.KERR = 0;
    PFN.KAWR = 0;
    ResetKol(_WS_ERR);
}
//---------------------------------------------------------
//---------------------------------------------------------
//Wyswietla wskaz skladajacy sie z oznaczenia literowego i cyfry od 0 do 9
//lkod - dowolny znak w kodzie led wyswietlany na starszej cyfrze
//dla tryb=0:
//nkod - liczba zakresu 0-F wyswietlana na mlodszej cyfrze
//dla tryb=1:
//nkod - dowolny znak w kodzie led wyswietlany na mlodszej cyfrze

void PrintPR(const unsigned char lkod, unsigned char nkod, unsigned char tryb)
{
    //if((nkod > 0xf) && !tryb) nkod = 0xf;
    //SetData(&DWD[2], lkod, (!tryb) ? Znak[nkod] : nkod);
    if(nkod < 10)
    {
        SetData(&DWD[2], lkod, (!tryb) ? Znak[nkod] : nkod);
        SetData(&DWD[3], lkod, (!tryb) ? Znak[nkod] : nkod);
        SetStatus(&DWD[2], tLED, tALL);
        SetStatus(&DWD[3], tLED, tALL);
    }
    else
    {
        SetData(&DWD[3], lkod, BLACK);
        SetData(&DWD[2],0 ,nkod);
        SetStatus(&DWD[3], tLED, tALL);
        SetStatus(&DWD[2], tDEC, tALL);
    }
    SetPoz(_WS_PRM, 0);
    SetPoz(_WS_PRM, 1);
}
//----------
//wyswietla wartosc parametru

void PrintWPR(const unsigned char wr)
{
    SetData(&DWD[2], 0, wr);
    SetData(&DWD[3], 0, wr);
    SetStatus(&DWD[2], tDEC, tALL);
    SetStatus(&DWD[3], tDEC, tALL);  
    SetPoz(_WS_PRM, 0);
    SetPoz(_WS_PRM, 1);
}
//----------
//restartuje kolejke wskazu parametru
void RestartPR(void)
{
    RestartKol(_WS_PRM);
}
//----------
//usuwa wskaz parametru
void EndPR(void)
{
    ResetKol(_WS_PRM);
}
//---------------------------------------------------------
//---------------------------------------------------------
//natychmiastowa aktualizacja wskazu nastawy

void RNst(const unsigned char nst)
{
    SetData(&DWD[4], 0, nst); //nastawa CO/CW/Kt
    SetData(&DWD[5], 0, nst); //nastawa CO/CW/Kt
}
//wyswietla nastawe temp. CW

void PrintNstCW(const unsigned char nst)
{
    SetData(&DWD[4], 0, nst); //nastawa CW
    SetData(&DWD[5], 0, nst); //nastawa CW
    SetStatus(&DWD[4], tDEC | tKROPKA0 | tSDID | _DCW, tALL);
    SetStatus(&DWD[5], tDEC | tKROPKA0 | tRDID | _DCW, tALL);
    RestartKol(_WS_NCWCO);
}
//wyswietla nastawe temp. CO

void PrintNstCO(const unsigned char nst)
{
    SetData(&DWD[4], 0, nst); //nastawa CO
    SetData(&DWD[5], 0, nst); //nastawa CO
    SetStatus(&DWD[4], tDEC | tSDID | _DCO, tALL);
    SetStatus(&DWD[5], tDEC | tRDID | _DCO, tALL);
    RestartKol(_WS_NCWCO);
}
//wyswietla nastawe Kt

void PrintNstKt(const unsigned char nst)
{
    SetData(&DWD[4], 0, nst); //nastawa Kt
    SetData(&DWD[5], 0, nst); //nastawa Kt
    SetStatus(&DWD[4], tDEC | tKROPKA1 | tSDID | _DCO, tALL);
    SetStatus(&DWD[5], tDEC | tKROPKA1 | tRDID | _DCO, tALL);
    RestartKol(_WS_NCWCO);
}
//usuwa wskaz nastawy CW/CO

void EndNst(void)
{
    ResetKol(_WS_NCWCO);
}
//---------------------------------------------------------
//---------------------------------------------------------

void SetL3(void)
{
    PFNf_fL3 = 1;
}
//---------------------------------------------------------
//---------------------------------------------------------
//wyswietla kod L3 naprzemiennie z temperatura CO

void PrintL3(void)
{
    SetData(&DWD[6], led_L, led_3);
    SetStatus(&DWD[6], tLED | tFOCUS, tALL); //L
    if(DtCOf_err_CO)
    {
        SetData(&DWD[7], led_L, led_3);
        SetStatus(&DWD[7], tLED | tFOCUS, tALL); //3
    }
    else
    {
        SetData(&DWD[7], 0, DtCO.SCO);
        SetStatus(&DWD[7], tDEC | tFOCUS, tALL); //3
    }
    SetPoz(_WS_MSG, 0);
    SetPoz(_WS_MSG, 1);
}
//----------
//usuwa wskaz komunikatu

void EndL3(void)
{
    PFNf_fL3 = 0;
    ResetKol(_WS_MSG);
}
//---------------------------------------------------------
//---------------------------------------------------------
//wyswietla temperature dyzurna dla CO (do zastosowania przy realizacji funkcji temp. dyzurnej)

/*void PrintTdzCO(const unsigned char tm)
{
        SetStatus(&PWD[_WS_TDCO],tDEC|tFOCUS,tALL);
        SetData(&PWD[_WS_TDCO],0,tm);
}
//usuwa wskaz TdzCO
void EndTdzCO(void)
{
        SetStatus(&PWD[_WS_TDCO],tDEC,tALL);
}*/
void PrintAL(void)
{
    SetStatus(&PWD[_WS_TDCO], tLED | tFOCUS, tALL);
    SetData(&PWD[_WS_TDCO], led_A, led_L);
}
//usuwa wskaz AL
void EndAL(void)
{
    SetStatus(&PWD[_WS_TDCO], tLED, tALL);
}
void PrintALTm(const unsigned char tm, const unsigned char pls)
{
    SetData(&DWD[6], led_A, led_L);
    SetData(&DWD[7], 0, tm);
    SetStatus(&DWD[6], tLED | tSDID | _DKM, tALL); //kod bledu/temperatura
    if(pls) SetStatus(&DWD[7], tKROPKA0 | tDEC | tRDID | _DKM, tALL); //temperatura
    else SetStatus(&DWD[7], tKROPKA0 | tDEC | tSDID | _DKM, tALL);
    SetPoz(_WS_MSG, 0);
    SetPoz(_WS_MSG, 1);
}
void EndALTm(void)
{
    ResetKol(_WS_MSG);
}
//---------------------------------------------------------
//---------------------------------------------------------
//wyswietla temperature dyzurna dla CO (do zastosowania przy realizacji funkcji temp. dyzurnej)

/*void PrintTdzCW(const unsigned char tm)
{
        SetStatus(&PWD[_WS_TDCW],tDEC|tFOCUS,tALL);
        SetData(&PWD[_WS_TDCW],0,tm);
}
//usuwa wskaz TdzCO
void EndTdzCW(void)
{
        SetStatus(&PWD[_WS_TDCW],tDEC,tALL);
}*/
void PrintSR(void)
{
    SetStatus(&PWD[_WS_TDCW], tLED | tFOCUS, tALL);
    SetData(&PWD[_WS_TDCW], Znak[5], led_r);
}
//usuwa wskaz TdzCO

void EndSR(void)
{
    SetStatus(&PWD[_WS_TDCW], tLED, tALL);
}
void PrintSRTm(const unsigned char tm, const unsigned char pls)
{
    SetData(&DWD[6], Znak[5], led_r);
    SetData(&DWD[7], 0, tm);
    SetStatus(&DWD[6], tLED | tSDID | _DKM, tALL); //kod bledu/temperatura
    if(pls) SetStatus(&DWD[7], tDEC | tRDID | _DKM, tALL); //temperatura
    else SetStatus(&DWD[7], tDEC | tSDID | _DKM, tALL);
    SetPoz(_WS_MSG, 0);
    SetPoz(_WS_MSG, 1);
}
void EndSRTm(void)
{
    ResetKol(_WS_MSG);
}
//---------------------------------------------------------
//---------------------------------------------------------
//wyswietla dwie kreski na wyswietlaczu

void PrintOFF(void)
{
    SetStatus(&PWD[_WS_OFF], tLED1 | tLED0 | tFOCUS, tALL);
    SetData(&PWD[_WS_OFF], KRESKA, KRESKA);
}
//usuwa dwie kreski z wyswietlacza

void EndOFF(void)
{
    SetStatus(&PWD[_WS_OFF], tLED1 | tLED0, tALL);
}
//---------------------------------------------------------
//---------------------------------------------------------
//wyswietla cisnienie CS

void PrintCS(const unsigned char cs)
{
    SetStatus(&PWD[_WS_CS], tDEC | tKROPKA1 | tFOCUS, tALL); //cisnienie
    SetData(&PWD[_WS_CS], 0, cs);
}
//usuwa wskaz CS

void EndCS(void)
{
    SetStatus(&PWD[_WS_CS], tDEC | tKROPKA1, tALL);
}
//---------------------------------------------------------
//---------------------------------------------------------
//wyswietla temperature CW gdy aktywne grznie w obiegu CW
//ustawia diode grzania CW w stan aktywny

void PrintCW(const unsigned char tm)
{
    SetStatus(&PWD[_WS_TCW], tDEC | tKROPKA0 | tFOCUS, tALL);
    SetData(&PWD[_WS_TCW], 0, tm); //tempetratura CW
}
//usuwa wskaz CW

void EndCW(void)
{
    SetStatus(&PWD[_WS_TCW], tDEC, tALL);
}
//---------------------------------------------------------
//---------------------------------------------------------
//Wskazy o najnizszym priorytecie wyswietlania
//wyswietla temperature CO

void PrintCO(const unsigned char tm)
{
    SetStatus(&PWD[_WS_TCO], tDEC, tALL);
    SetData(&PWD[_WS_TCO], 0, tm); //tempetratura CO
}
//---------------------------------------------------------
//---------------------------------------------------------
//Funkcja na potrzeby testow oprogramowania

void PrintHex(const unsigned char tm)
{
    SetStatus(&PWD[_WS_TCO], tHEX, tALL);
    SetData(&PWD[_WS_TCO], 0, tm); //tempetratura CO
}
//---------------------------------------------------------
//---------------------------------------------------------
//Wysterowanie wszystkich elementow swietlnych wyswietlacza
void SetDiode(unsigned char nrd, unsigned char stn);

void TestScr(void)
{
    SetStatus(&PWD[_WS_TCO], tLED | tKROPKA1 | tKROPKA0, tALL);
    SetData(&PWD[_WS_TCO], FULL, FULL);
#if SER_WSW!=0
    SetDiode(_DCO | _DCW | _DPL | _DKM, 1);
#endif
}

void EndTestScr(void)
{
    SetStatus(&PWD[_WS_TCO], tDEC, tALL);
    SetData(&PWD[_WS_TCO], 0, 0);
#if SER_WSW!=0
    SetDiode(_DCO | _DCW | _DPL | _DKM, 0);
#endif
}
//---------------------------------------------------------
//---------------------------------------------------------
#if SER_WSW!=0
//Zapala/gasi wybrane diody z 4 umieszczonych na module wyswietlacza
//wd-slowo 4 bitowe gdzie wartosc 1 oznacza wybor konkretnej diody
//stn-stan jaki maja przyjac wybrane diody (1-zapalona, 0-wygaszona)

void SetDiode(unsigned char wd, unsigned char stn)
{
    if(stn) Swsdid(wd);
    else Rwsdid(wd);
}
#endif
