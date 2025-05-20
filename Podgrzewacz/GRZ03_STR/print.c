/*---------------------------------------------------------------------------------*/
/*print.c*/
/*Funkcje zapisu danych do 2 segmentowego wyswietlacza typu LED*/
/*w oparciu o procedury obslugi wyswietlacza zawarte w pliku wsw.c*/
/*Sposob reprezentacji danych - Podgrzewacz wody*/
/*---------------------------------------------------------------------------------*/
#include <pic18.h>
#include "global.h"				//parametry globalne
#include "print.h"				//parametry lokalne
#include "pomiar.tp.h"
//wsw.c
extern WPack PWD[];		//tablica priorytetow wyswietlania PWD[n] (n=0 - najwyzszy priorytet)
extern WPack DWD[];		//tablica danych modyfikowanych dynamicznie wystepujacych wylacznie w kolejkach
extern const unsigned char Znak[];
extern DataCO DtCO;		//pomiar.c 
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
void PrintErr(const unsigned char kod, const unsigned char pls)
{
	if(pls)
	{
		SetData(&DWD[0],0,kod);
		SetData(&DWD[1],BLACK,BLACK);
		SetStatus(&DWD[0],tHEX,tALL);
		SetStatus(&DWD[1],tLED,tALL);
	}
	else
	{
		SetData(&DWD[0],0,kod);
		SetData(&DWD[1],0,kod);
		SetStatus(&DWD[0],tHEX,tALL);
		SetStatus(&DWD[1],tHEX,tALL);
	}
	SetPoz(_WS_ERR,0);
	SetPoz(_WS_ERR,1);
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
	if((nkod>0xf)&&!tryb) nkod=0xf;
	SetData(&DWD[2],lkod,(!tryb)?Znak[nkod]:nkod);
	SetStatus(&DWD[2],tLED,tALL);					
	SetPoz(_WS_PRM,0);
	SetPoz(_WS_PRM,1);
}
//----------
//wyswietla wartosc parametru
void PrintWPR(const unsigned char wr)
{
	SetData(&DWD[2],0,wr);							
	SetStatus(&DWD[2],tDEC,tALL);				
	SetPoz(_WS_PRM,0);
	SetPoz(_WS_PRM,1);
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
	SetData(&DWD[3],0,nst);							//nastawa CO/CW/Kt
	SetData(&DWD[4],BLACK,BLACK);					//nastawa CO/CW/Kt
}
//wyswietla nastawe temp. CW
void PrintNstCW(const unsigned char nst,const unsigned char krp)
{
	SetData(&DWD[3],0,nst);							//nastawa CW
	SetData(&DWD[4],BLACK,BLACK);					//nastawa CW
	if(!krp) SetStatus(&DWD[3],tDEC,tALL);			//wskaz z kropka
	else SetStatus(&DWD[3],tDEC|tKROPKA0,tALL);		//wskaz bez kropki
	SetStatus(&DWD[4],tLED,tALL);
	RestartKol(_WS_NCW);
}
//usuwa wskaz nastawy CW
void EndNst(void)
{
	ResetKol(_WS_NCW);
}
//---------------------------------------------------------
//---------------------------------------------------------
//wygasza wyswietlacz w czsie bezczynnosci
void PrintOFF(void)
{
	SetStatus(&PWD[_WS_OFF],tLED1|tLED0|tFOCUS,tALL);			
	SetData(&PWD[_WS_OFF],BLACK,BLACK);
    //_STR=0;
    //_DTA=0;
    //_CLK=0;
    //_K3=0;
    if(_GRZEJ) _K4=1;
    else _K4=0;
}
// wyswietlacza
void EndOFF(void)
{
    _K4=0;    
	SetStatus(&PWD[_WS_OFF],tLED1|tLED0,tALL);
}
//---------------------------------------------------------
//---------------------------------------------------------
//Wskazy o najnizszym priorytecie wyswietlania
//wyswietla temperature CW gdy aktywne grznie w obiegu CW
void PrintCW(const unsigned char tm,const unsigned char krp)
{
	if(!krp) SetStatus(&PWD[_WS_TCW],tDEC|tFOCUS,tALL);			//wskaz z kropka
	else SetStatus(&PWD[_WS_TCW],tDEC|tKROPKA0|tFOCUS,tALL);		//wskaz bez kropki
	SetData(&PWD[_WS_TCW],0,tm);						//tempetratura CW
}
//usuwa wskaz CW
void EndCW(void)
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
