/*history.c*/
//Procedury zwiazane z impulsowym czujnikiem przeplywu
/*---------------------------------------------------------------------------------*/
#include <xc.h>
#include <stdbool.h>
#include <stdint.h>

#include "../../proj.tp.h"

#ifndef _TYP_PRJ
#error NIEOKRESLONY IDENTYFIKATOR PROJEKTU
#endif

#if _TYP_PRJ==1
#advisory _TYP_PRJ==1 
#include "global.h"
#include "main.h"
#include "global.ex.h"

#include "history.h"

extern tpMAIN M;
extern tpRSDTA1 RSDT1;
extern unsigned char xxy,zzz,vvv,mmm;
/*---------------------------------------------------------------------------------*/
volatile unsigned char Hind,HNPz,STPz,WrST,Hflg;
volatile unsigned int HRef;
//-------------------------------------------------------------------------------
static bool inline InGetBit(volatile unsigned char data, const unsigned char nbit)
{
	return ((data&(1<<nbit))?1:0);    
}
//-------------------------------------------------------------------------------
void AddHPoz(HIST_POZ HP)
{
    volatile unsigned char i,d;
    
    Hind=ReadEEPROM(_ADR_HIND);
    Hflg=ReadEEPROM(_ADR_HFLG);   
    
    if(Hind>(_HISTSZ-1))  //FIFO
    {
        for(i=0;i<(_HISTSZ-1);i++)
        {
            d=ReadEEPROM(_ADR_HBUF+(i+1));
            if_WriteEEPROM(_ADR_HBUF+i,d);
        }
        if_WriteEEPROM(_ADR_HBUF+i,HP.war);
    }
    else
    {
        if_WriteEEPROM(_ADR_HBUF+Hind,HP.war); 
    }
    
    HNPz=Hind+1;
    Hflg=Hflg+1;
    if(HNPz>_HISTSZ) HNPz=_HISTSZ;      //liczba zapisanych pozycji historii zdarzen
    if(Hflg>_HISTSZ) Hflg=_HISTSZ;      //licznik flagi serwisowej   
    if(Hind<_HISTSZ) Hind++;            //wskazuje na pozycje do zapisania kolejnej danej
    if_WriteEEPROM(_ADR_HIND,Hind);    //zapisz wskaznik pozycji do zapisania kolejnej danej
    if_WriteEEPROM(_ADR_HNPZ,HNPz);    //zapisz liczbe pozycji historii zdarzen
    if_WriteEEPROM(_ADR_HFLG,Hflg);    //zapisz licznik flagi serwisowej
}
//-------------------------------------------------------------------------------
//Aktualizacja bufora danych historycznych
unsigned char RefBHist(const unsigned char res)
{
    volatile HIST_POZ s_hp;
    volatile HIST_POZ hp;
    volatile unsigned char e;
    
    s_hp.war=ReadEEPROM(_ADR_SHBF);
    if(res)
    {
        hp.war=0;    
        if(!ReadEEPROM(_ADR_HNPZ)) //bufor zdarzen pusty?
        {
            s_hp.war=0;
            if_WriteEEPROM(_ADR_SHBF,0);        
            
            HRef=0;
            //zapisz licznik zapisow
            e=(unsigned char)(HRef&0x00ff);
            if_WriteEEPROM(_ADR_HRFL,e); 
            e=(unsigned char)((HRef&0xff00)>>8);    
            if_WriteEEPROM(_ADR_HRFH,e);
        }
        else
        {
            
        }
        return 0;
    }
    if(M.STRB) return 0;  //spec. tryb produkcyjny?
    
    //status urzadzenia    
    hp.EST.rST=0;                   //staus (0-brak zdarzen,1-wylaczenie z proc awaryjna,2-wylaczenie z blokada awaryjna,3-wylaczenie bez plokady(powrot automatyczny))
    hp.EST.wST=0;
    
    if(InGetBit(RSDT1.outSTAT1,_ERR))
    {
        if(InGetBit(RSDT1.outSTAT0,_PGRES))
        {
            hp.EST.rST=2;   //wylaczenie awaryjne z blokada 
        }
        else
        {
            hp.EST.rST=3;   //wylaczenie awaryjne (automatyczny powrot do normalnej pracy po usunieciu przyczyny awarii)
        }
        hp.EST.wST=RSDT1.outERR;        
    }
    else
    if(InGetBit(RSDT1.outSTAT1,_MSG))
    {
        hp.EST.rST=1;   //procedura awaryjna
        hp.EST.wST=RSDT1.outAWR; 
    }
    else
    {
        s_hp.war=0;
        if_WriteEEPROM(_ADR_SHBF,0);        //eliminacja powtorzen zapisu dla tego samego bledu
    }

    if(hp.war&&(hp.war!=s_hp.war))
    {
        s_hp.war=hp.war; 
        if_WriteEEPROM(_ADR_SHBF,s_hp.war);    //zapamietaj wartosc dla eliminacji powtorzen zapisu dla tego samego bledu
        AddHPoz(hp); 
        HRef=ReadEEPROM(_ADR_HRFL)+(ReadEEPROM(_ADR_HRFH)*256);
        if(HRef<0xFFFF) HRef++;                 //aktualizuj licznik zapisow
        else HRef=ReadEEPROM(_ADR_HNPZ); 
        //zapisz licznik zapisow
        e=(unsigned char)(HRef&0x00ff);
        if_WriteEEPROM(_ADR_HRFL,e); 
        e=(unsigned char)((HRef&0xff00)>>8);    
        if_WriteEEPROM(_ADR_HRFH,e);               
    }  
    else return 0;
    return 1;
}
//-----------------------------------------------------
//-----------------------------------------------------
//Dodaj pozycje statusu wylaczenia awaryjnego z blokada
void AddSTPoz(void)
{
    volatile unsigned char i,j;
    
    STPz=ReadEEPROM(_ADRSTPZ);    //liczba zapisanych statusow wylaczenia awaryjnego
    if(STPz)
    {
        for(i=0;i<STPz;i++)
        {
            j=STPz-1-i;
            CopyEESTAT(j,j+1);    //i+1=i
        }
    }
    WriteEESTAT(0);
    if(STPz<_ESTBL) STPz++;
    if_WriteEEPROM(_ADRSTPZ,STPz);//zapisz liczbe pozycji historii zdarzen

}
//-----------------------------------------------------
//Aktywacja zapisu statusu wylaczenia awaryjnego z blokada
//(umiescic w miejscu skoku do petli wylaczenia z blokada awaryjna)
void ToWriteESTAT(void)
{
    WrST=1;
}
//-----------------------------------------------------
//Procedura zapisu statusu wylaczenia awaryjnego z blokada
void RefBStat(const unsigned char res)
{
    if(res)
    {
        WrST=0;
        return;
    }   
    if(M.STRB) return;  //spec. tryb produkcyjny?
    if(WrST)
    {
        WrST=0;
        AddSTPoz();
    }
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Wyzerowanie w EEPROM licznika flagi serwisowej
void ClrHFLG(void)
{
    Hflg=0;
    if_WriteEEPROM(_ADR_HFLG,Hflg);
}
//-----------------------------------------------------
//-----------------------------------------------------
//-------------------------------------------------------------------------------
#endif
