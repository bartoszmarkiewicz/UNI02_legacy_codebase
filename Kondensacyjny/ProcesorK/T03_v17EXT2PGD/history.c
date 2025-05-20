/*history.c*/
//Procedury zwiazane z impulsowym czujnikiem przeplywu
/*---------------------------------------------------------------------------------*/
#include <pic18.h>
#include "global.h"					//parametry globalne
#include "history.h"
#include "main.tp.h"
#include "defaultpar.h"
#include "main.tp.h"			//parametry lokalne
#include "smgs_rs9b.tp.h"

extern tpMAIN M;
extern tpRSDTA1 RSDT1;
extern unsigned char xxy,zzz,vvv,mmm;
/*---------------------------------------------------------------------------------*/
volatile unsigned char Hind,HNPz,STPz,WrST,Hflg;
volatile unsigned int HRef;

void AddHPoz(HIST_POZ HP)
{
    volatile unsigned char i,d;
    
    Hind=eeprom_read(_ADR_HIND);
    Hflg=eeprom_read(_ADR_HFLG);   
    
    if(Hind>(_HISTSZ-1))  //FIFO
    {
        for(i=0;i<(_HISTSZ-1);i++)
        {
            d=eeprom_read(_ADR_HBUF+(i+1));
            if_eeprom_write(_ADR_HBUF+i,d);
        }
        if_eeprom_write(_ADR_HBUF+i,HP.war);
    }
    else
    {
        if_eeprom_write(_ADR_HBUF+Hind,HP.war); 
    }
    
    HNPz=Hind+1;
    Hflg=Hflg+1;
    if(HNPz>_HISTSZ) HNPz=_HISTSZ;      //liczba zapisanych pozycji historii zdarzen
    if(Hflg>_HISTSZ) Hflg=_HISTSZ;      //licznik flagi serwisowej   
    if(Hind<_HISTSZ) Hind++;            //wskazuje na pozycje do zapisania kolejnej danej
    if_eeprom_write(_ADR_HIND,Hind);    //zapisz wskaznik pozycji do zapisania kolejnej danej
    if_eeprom_write(_ADR_HNPZ,HNPz);    //zapisz liczbe pozycji historii zdarzen
    if_eeprom_write(_ADR_HFLG,Hflg);    //zapisz licznik flagi serwisowej
}
//-------------------------------------------------------------------------------
//Aktualizacja bufora danych historycznych
unsigned char RefBHist(const unsigned char res)
{
    volatile HIST_POZ s_hp;
    volatile HIST_POZ hp;
    volatile unsigned char e;
    
    s_hp.war=eeprom_read(_ADR_SHBF);
    if(res)
    {
        hp.war=0;    
        if(!eeprom_read(_ADR_HNPZ)) //bufor zdarzen pusty?
        {
            s_hp.war=0;
            if_eeprom_write(_ADR_SHBF,0);        
            
            HRef=0;
            //zapisz licznik zapisow
            e=(unsigned char)(HRef&0x00ff);
            if_eeprom_write(_ADR_HRFL,e); 
            e=(unsigned char)((HRef&0xff00)>>8);    
            if_eeprom_write(_ADR_HRFH,e);
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
    
    if(GetBit2(RSDT1.outSTAT1,_ERR))
    {
        if(GetBit2(RSDT1.outSTAT0,_PGRES))
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
    if(GetBit2(RSDT1.outSTAT1,_MSG))
    {
        hp.EST.rST=1;   //procedura awaryjna
        hp.EST.wST=RSDT1.outAWR; 
    }
    else
    {
        s_hp.war=0;
        if_eeprom_write(_ADR_SHBF,0);        //eliminacja powtorzen zapisu dla tego samego bledu
    }

    if(hp.war&&(hp.war!=s_hp.war))
    {
        s_hp.war=hp.war; 
        if_eeprom_write(_ADR_SHBF,s_hp.war);    //zapamietaj wartosc dla eliminacji powtorzen zapisu dla tego samego bledu
        AddHPoz(hp); 
        HRef=eeprom_read(_ADR_HRFL)+(eeprom_read(_ADR_HRFH)*256);
        if(HRef<0xFFFF) HRef++;                 //aktualizuj licznik zapisow
        else HRef=eeprom_read(_ADR_HNPZ); 
        //zapisz licznik zapisow
        e=(unsigned char)(HRef&0x00ff);
        if_eeprom_write(_ADR_HRFL,e); 
        e=(unsigned char)((HRef&0xff00)>>8);    
        if_eeprom_write(_ADR_HRFH,e);               
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
    
    STPz=eeprom_read(_ADRSTPZ);    //liczba zapisanych statusow wylaczenia awaryjnego
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
    if_eeprom_write(_ADRSTPZ,STPz);//zapisz liczbe pozycji historii zdarzen

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
    if_eeprom_write(_ADR_HFLG,Hflg);
}
//-----------------------------------------------------
//-----------------------------------------------------
/*
void RefBHist(const unsigned char res)
{
    static HIST_POZ s_hp;
    volatile HIST_POZ hp;
    volatile unsigned char e;
    
    if(res)
    {
        s_hp.war=0;
        hp.war=0;    
        if(!eeprom_read(_ADR_HNPZ))
        {
            HRef=0;
            //zapisz licznik zapisow
            e=(unsigned char)(HRef&0x00ff);
            if_eeprom_write(_ADR_HRFL,e); 
            e=(unsigned char)((HRef&0xff00)>>8);    
            if_eeprom_write(_ADR_HRFH,e);
        }
        return;
    }
    
    //status urzadzenia    
    hp.EST.rST=0;                   //staus (0-brak zdarzen,1-wylaczenie z proc awaryjna,2-wylaczenie z blokada awaryjna,3-wylaczenie bez plokady(powrot automatyczny))
    hp.EST.wST=0;
    if(M._HPW_START||M._HRS_START) //restart mikrokontrolera
    {
        M._HPW_START=0;
        M._HRS_START=0;
        if(eeprom_read(_ADR_HNPZ))      //nie jest to pierwszy wpis w historii?
        {
            s_hp.war=eeprom_read(_ADR_SHBF);    //eliminacja powtorzen zapisu dla tego samego bledu
        }
    }    
    if(GetBit2(RSDT1.outSTAT1,_ERR))
    {
        if(GetBit2(RSDT1.outSTAT0,_PGRES))
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
    if(GetBit2(RSDT1.outSTAT1,_MSG))
    {
        hp.EST.rST=1;   //procedura awaryjna
        hp.EST.wST=RSDT1.outAWR; 
    }
    else
    {
        s_hp.war=0;
        if_eeprom_write(_ADR_SHBF,0);        //eliminacja powtorzen zapisu dla tego samego bledu
    }

    if(hp.war&&(hp.war!=s_hp.war))
    {
        s_hp.war=hp.war; 
        if_eeprom_write(_ADR_SHBF,s_hp.war);    //zapamietaj wartosc dla eliminacji powtorzen zapisu dla tego samego bledu
        AddHPoz(hp); 
        HRef=eeprom_read(_ADR_HRFL)+(eeprom_read(_ADR_HRFH)*256);
        if(HRef<0xFFFF) HRef++;                 //aktualizuj licznik zapisow
        else HRef=eeprom_read(_ADR_HNPZ); 
        //zapisz licznik zapisow
        e=(unsigned char)(HRef&0x00ff);
        if_eeprom_write(_ADR_HRFL,e); 
        e=(unsigned char)((HRef&0xff00)>>8);    
        if_eeprom_write(_ADR_HRFH,e);               
    }  
}*/
//-------------------------------------------------------------------------------
/*void WriteHind(void)
{
    if_eeprom_write(_ADR_HIND,Hind);        
}*/
//-------------------------------------------------------------------------------
/*void ClearHBuf(void)
{
    volatile unsigned char i;

    Hind=0;
    HNPz=0;
    HRef=0;  
    
    if_eeprom_write(_ADR_HIND,0);
    if_eeprom_write(_ADR_HNPZ,0);
    if_eeprom_write(_ADR_HRFL,0);        //zeruj licznik zapisow
    if_eeprom_write(_ADR_HRFH,0);        //zeruj licznik zapisow    
    for(i=0;i<_HISTSZ;i++)
    {
        if_eeprom_write(_ADR_HBUF+i,0);       
    }
    RefBHist(1);    //ustaw bufor w stan poczatkowy
}*/
//-------------------------------------------------------------------------------
//Zwraca liczbe zapisanych pozycji bufora danych
/*unsigned char GetHBufSize(void)
{
    unsigned char d;
    d=eeprom_read(_ADR_HNPZ);
    return d;
}*/
//-------------------------------------------------------------------------------
//Zwraca pozycje flagi serwisowej
/*unsigned char GetHBufFlag(void)
{
    unsigned char d;
    d=eeprom_read(_ADR_HFLG);
    return d;
}*/
//-------------------------------------------------------------------------------
//Zwraca pozycje flagi serwisowej
/*unsigned char ClrHBufFlag(void)
{
    unsigned char d;
    if_eeprom_write(_ADR_HFLG,0x00);
    return d;
}*/
//-------------------------------------------------------------------------------
//Zwraca licznik aktualizacji
/*unsigned char GetHBuRef(void)
{
    return HRef;
}*/
//-------------------------------------------------------------------------------