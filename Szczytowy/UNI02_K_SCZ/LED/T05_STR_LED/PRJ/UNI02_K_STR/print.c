/*---------------------------------------------------------------------------------*/
/*print.c*/
/*Funkcje zapisu danych do 2 segmentowego wyswietlacza typu LED*/
/*w oparciu o procedury obslugi wyswietlacza zawarte w pliku wsw.c*/
/*Sposob reprezentacji danych - Podgrzewacz wody*/
/*---------------------------------------------------------------------------------*/
#include <xc.h>
#include "../../proj.tp.h"

#ifndef _TYP_PRJ
#error NIEOKRESLONY IDENTYFIKATOR PROJEKTU
#endif

#if _TYP_PRJ==5
#advisory _TYP_PRJ==5
#include "../../mcc_generated_files\mcc.h" 
#include "global.h"
#include "main.h"
#include "global.ex.h"

#include "print.h"				//parametry lokalne
//#include "wsw2.h"

//wsw.c
extern WPack PWD[];		//tablica priorytetow wyswietlania PWD[n] (n=0 - najwyzszy priorytet)
extern WPack DWD[];		//tablica danych modyfikowanych dynamicznie wystepujacych wylacznie w kolejkach
extern const unsigned char Znak[];
extern tpMAIN M;
//---------------------------------------------------------
//---------------------------------------------------------
//Funkcje wyswietlania wg waznosci.
//ClsScr
//-PrintErr
//--PrintPR,PrintWPR
//---PrintNstCW
//-----PrintMsg
//-------PrintOFF
//---------PrintCW
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
//wyswietla migajacy kod bledu
//kod - parametr okreslajacy migajacy kod bledu do wyswietlenia
//pls - dla 1 kod error miga, dla 0 kod error stale zapalona
void PrintErr(const unsigned char kod, const unsigned char pls, const unsigned char krp)
{
    volatile unsigned char dt;
    
    SetStatus(&DWD[2],tLED,tALL);
    SetData(&DWD[2],BLACK,BLACK);
    
	if(kod<10)
	{
        SetStatus(&DWD[0],tLED,tALL);
        SetData(&DWD[0],BLACK,BLACK);
        dt=0xE0|kod;
        SetData(&DWD[1],0,dt);
        if(!krp)
        {
            SetStatus(&DWD[1],tHEX,tALL);
        }
        else
        {
            SetStatus(&DWD[1],tHEX|tKROPKA0,tALL);    
        }
        SetPoz(_WS_ERR,1);
        ResetPoz(_WS_ERR,0);       
	}
	else
	{
        dt=0xE0;
		SetData(&DWD[0],led_E,BLACK);
		SetData(&DWD[1],0,kod);
		SetStatus(&DWD[0],tLED,tALL);
        if(!krp)
        {
            SetStatus(&DWD[1],tDEC,tALL);
        }
        else
        {
            SetStatus(&DWD[1],tDEC|tKROPKA0,tALL);
        }            
        SetPoz(_WS_ERR,0);
        SetPoz(_WS_ERR,1);
	}
    if(pls)
    {
        SetPoz(_WS_ERR,2);
    }
    else
    {
        ResetPoz(_WS_ERR,2);
    }
}
//usuwa migajacy kod bledu
void EndErr(void)
{
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
void PrintPR(const unsigned char lkod,unsigned char nkod,unsigned char tryb)
{
//	if((nkod>0xf)&&!tryb) nkod=0xf;
//	SetData(&DWD[3],lkod,(!tryb)?Znak[nkod]:nkod);
//	SetStatus(&DWD[3],tLED,tALL);					
//	SetPoz(_WS_PRM,0);
//	SetPoz(_WS_PRM,1);
    
    volatile unsigned char dt;
    
	if(tryb||nkod<10)
	{
        SetData(&DWD[3],lkod,(!tryb)?Znak[nkod]:nkod);
        SetData(&DWD[4],BLACK,BLACK);
        SetStatus(&DWD[3],tLED,tALL);	
        SetStatus(&DWD[4],tLED,tALL);
        SetPoz(_WS_PRM,0);
        ResetPoz(_WS_PRM,1);       
	}
	else
	{
        dt=0xE0;
		SetData(&DWD[3],lkod,BLACK);
		SetData(&DWD[4],0,nkod);
		SetStatus(&DWD[3],tLED,tALL);
		SetStatus(&DWD[4],tDEC,tALL);
        SetPoz(_WS_PRM,0);
        SetPoz(_WS_PRM,1);
	}  
}
//----------
//Dla parametru wiekszego od 9 wymusza wyswietlenie numeru parametru z pominieciem symbolu parametru
//npar - numer parametru
void UpdatePR(const unsigned char npar)
{
    if(npar>9)
    {
        SetActiveKolPoz(_WS_PRM,1);
    }
}
//---------------------------------------------------------
//---------------------------------------------------------
//wyswietla wartosc parametru
void PrintWPR(const unsigned char wr)
{
	SetData(&DWD[3],0,wr);							
	SetStatus(&DWD[3],tDEC,tALL);				
	SetPoz(_WS_PRM,0);
	ResetPoz(_WS_PRM,1);
}
//----------
//usuwa wskaz parametru
void EndPR(void)
{
	ResetKol(_WS_PRM);
}
//---------------------------------------------------------
//---------------------------------------------------------
void PrintSRV(const unsigned char tryb)
{
    switch(tryb)
	{
        case 0:
        {
            SetData(&DWD[3],led_L,led_o);
            SetData(&DWD[4],BLACK,BLACK);
            SetStatus(&DWD[3],tLED,tALL);	
            SetStatus(&DWD[4],tLED,tALL);
            SetPoz(_WS_PRM,0);
            SetPoz(_WS_PRM,1); 
            break;
        }
        case 1:
        {
            SetData(&DWD[3],led_H,led_i);
            SetData(&DWD[4],BLACK,BLACK);
            SetStatus(&DWD[3],tLED,tALL);
            SetStatus(&DWD[4],tLED,tALL);
            SetPoz(_WS_PRM,0);
            SetPoz(_WS_PRM,1);
            break;
        }   
        default:
        {
            SetData(&DWD[3],BLACK,BLACK);
            SetData(&DWD[4],BLACK,BLACK);
            SetStatus(&DWD[3],tLED,tALL);
            SetStatus(&DWD[4],tLED,tALL);
            SetPoz(_WS_PRM,0);
            SetPoz(_WS_PRM,1);            
        }
	}    
}
//----------
//usuwa wskaz parametru
void EndSRV(void)
{
	ResetKol(_WS_PRM);
}
//---------------------------------------------------------
//---------------------------------------------------------
//natychmiastowa aktualizacja wskazu nastawy
void RNst(const unsigned char nst)
{
	SetData(&DWD[5],0,nst);							//nastawa CO/CW/Kt
	SetData(&DWD[6],BLACK,BLACK);					//nastawa CO/CW/Kt
}
//wyswietla nastawe temp. CW
//void PrintNstCO(const unsigned char nst,const unsigned char krp)
//{
//	SetData(&DWD[3],0,nst);							//nastawa CW
//	SetData(&DWD[4],BLACK,BLACK);					//nastawa CW
//	if(!krp) SetStatus(&DWD[3],tDEC,tALL);			//wskaz z kropka
//	else SetStatus(&DWD[3],tDEC|tKROPKA0,tALL);		//wskaz bez kropki
//	SetStatus(&DWD[4],tLED,tALL);
//	RestartKol(_WS_NCW);
//}

void PrintNstCO(const unsigned char nst,const unsigned char pgd)
{
	SetData(&DWD[5],0,nst);							//nastawa CW
	SetData(&DWD[6],BLACK,BLACK);					//nastawa CW
	if(!pgd) SetStatus(&DWD[5],tDEC,tALL);			//wskaz bez kropki
	else SetStatus(&DWD[5],tDEC|tKROPKA1,tALL);		//wskaz z kropka w srodku
	SetStatus(&DWD[6],tLED,tALL);
	RestartKol(_WS_NCW);
}
//usuwa wskaz nastawy CW
void EndNst(void)
{
	ResetKol(_WS_NCW);
}
//---------------------------------------------------------
//---------------------------------------------------------
void PrintKal(void)
{
    SetData(&DWD[7],KRESKA1,KRESKA1);
    SetData(&DWD[8],KRESKA2,KRESKA2);
    SetData(&DWD[9],KRESKA3,KRESKA3);
    SetData(&DWD[10],KRESKA2,KRESKA2);    
    SetStatus(&DWD[7],tLED,tALL);
    SetStatus(&DWD[8],tLED,tALL);
    SetStatus(&DWD[9],tLED,tALL);
    SetStatus(&DWD[10],tLED,tALL);    
	SetPoz(_WS_MSG,0);
	SetPoz(_WS_MSG,1);
	SetPoz(_WS_MSG,2);
	SetPoz(_WS_MSG,3);    
}
void EndKal(void)
{
	ResetKol(_WS_MSG);
}
//---------------------------------------------------------
//---------------------------------------------------------
//wygasza wyswietlacz w czsie bezczynnosci
void PrintOFF(const unsigned char L3)
{
    if(!L3)
    {
        SetStatus(&PWD[_WS_OFF],tLED1|tLED0|tFOCUS,tALL);	
    }
    else
    {
        SetStatus(&PWD[_WS_OFF],tLED1|tLED0|tKROPKA0|tFOCUS,tALL);
    }
	SetData(&PWD[_WS_OFF],BLACK,BLACK);
    
    if(M._ROZRUCH||_GRZEJ_GetValue())       //zapobieganie migotaniu wyswietlacza w czasie iskrzenia
    {
        //_K4=1;
        _KY4_SetHigh();
    }
    else
    {
        //_K4=0;
        _KY4_SetLow();
    }
}
// wyswietlacza
void EndOFF(void)
{
    //_K4=0;
    _KY4_SetLow();    
	SetStatus(&PWD[_WS_OFF],tLED1|tLED0,tALL);
}
//---------------------------------------------------------
//---------------------------------------------------------
//Wskazy o najnizszym priorytecie wyswietlania
//wyswietla temperature CW gdy aktywne grznie w obiegu CW
void PrintCO(const unsigned char tm,const unsigned char krp)
{
	if(!krp) SetStatus(&PWD[_WS_TCW],tDEC|tFOCUS,tALL);			//wskaz z kropka
	else SetStatus(&PWD[_WS_TCW],tDEC|tKROPKA0|tFOCUS,tALL);		//wskaz bez kropki
	SetData(&PWD[_WS_TCW],0,tm);						//tempetratura CW
}
//usuwa wskaz CW
void EndCO(void)
{
	SetStatus(&PWD[_WS_TCW],tDEC,tALL);
}
//---------------------------------------------------------
//---------------------------------------------------------
//Funkcja na potrzeby testow oprogramowania
void PrintHex(const unsigned char tm)
{
	SetStatus(&PWD[_WS_TCW],tHEX,tALL);				
	SetData(&PWD[_WS_TCW],0,tm);						//tempetratura CO
}
//---------------------------------------------------------
//---------------------------------------------------------
//Wysterowanie wszystkich elementow swietlnych wyswietlacza
void SetDiode(unsigned char nrd, unsigned char stn);
void TestScr(void)
{
	SetStatus(&PWD[_WS_TCW],tLED|tKROPKA1|tKROPKA0,tALL);				
	SetData(&PWD[_WS_TCW],FULL,FULL);
#if SER_WSW!=0
	SetDiode(_DCO|_DCW|_DPL|_DKM,1);
#endif
}
void EndTestScr(void)
{
	SetStatus(&PWD[_WS_TCW],tDEC,tALL);				
	SetData(&PWD[_WS_TCW],0,0);
#if SER_WSW!=0
	SetDiode(_DCO|_DCW|_DPL|_DKM,0);
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
//---------------------------------------------------------
//---------------------------------------------------------
void PrintErrCs(unsigned char dt1,unsigned char dt2,unsigned char dt3)
{
    
}
void PrintL3(void)
{
    
}
void EndL3(void)
{
    
}
#endif