/*pomiar.c*/
//Procedury pomiarowe (oparte na przetworniku AC) UNISTER zmodyfikowany GRZ01
// (!) Bazuje na funkcjach z pliku: ac.c, rtimer.c /RTdS(2)/
/*---------------------------------------------------------------------------------*/
//#include <pic18.h>
#include <xc.h>
#include <math.h>
#include "global.h"				//parametry globalne
#include "pomiar.h"				//parametry lokalne
#include "main.tp.h"

extern volatile DataPFN PFN;
extern volatile unsigned char xxy,zzz,vvv;

#if _EXT_BOARD!=0
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Inicjalizacja DtCO, DtCW, DtCS, PFN
void InitDataPmr(void)
{
    InitTabNTC();
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
static unsigned int tbb[_NMAXTAB]={0};
static unsigned int tbb2[_NMAXTAB2]={0};

void InitTabNTC(void)
{
    volatile unsigned char i;

    for(i=0;i<_NMAXTAB;i++) tbb[i]=0;
    for(i=0;i<_NMAXTAB2;i++) tbb2[i]=0;
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
unsigned int FiltrNTC(const unsigned int pom)
{
#if _NMAXKTAB<_NMAXTAB
    volatile unsigned int tmx[_NMAXTAB];
#else
    volatile unsigned int tmx[_NMAXKTAB];
#endif    
#if _NMAXTAB<_NMAXTAB2
    #if _NMAXKTAB2<_NMAXTAB2
        volatile unsigned char wb[_NMAXTAB2];
    #else
        volatile unsigned char wb[_NMAXKTAB2];
    #endif
#else
    #if _NMAXKTAB<_NMAXTAB
        volatile unsigned char wb[_NMAXTAB];
    #else
        volatile unsigned char wb[_NMAXKTAB];
    #endif    
#endif    
    volatile unsigned char i,j,im,ix;
    volatile unsigned int ndat,min,max,wrt;
    //------------------------------------------------------------------------
    //Poszukiwanie warto?ci "srodkowej" po elininacji wszystkich wartosci granicznych min/max z niepazystej liczby probek
    //------------------------------------------------------------------------
    ndat=_NMAXTAB;
    //zapamietaj wynik pomiaru w tablicy pomiarow
    for(i=1;i<ndat;i++) tbb[i-1]=tbb[i];
    tbb[ndat-1]=pom;	//nadpisz ostatnia pozycje aktualna dana
    //------------------------------------------------------------
    //kopiuj dane do tablicy roboczej
    for(i=0;i<ndat;i++) tmx[i]=tbb[i];

    //znajdz wartosc minimalna i maksymalna w tablicy i wyzeruj
    for(i=0;i<ndat;i++) wb[i]=0;
    for(j=0;j<(ndat/2);j++)
    {
            min=0xFFFF;
            max=0;
            for(i=0;i<ndat;i++)
            {
                    if(wb[i]) continue;
                    wrt=tmx[i];
                    if(max<wrt)
                    {
                            max=wrt;
                            ix=i;
                    }
                    if(min>wrt)
                    {
                            min=wrt;
                            im=i;
                    }
            }
            if(ix==im)
            {
                    im=j;
                    ix=(ndat-1-j);
            }
            wb[ix]=1;
            wb[im]=1;
            tmx[ix]=0;
            tmx[im]=0;
    }
    //znajdz wartosc niezerow?
    for(i=0;i<ndat;i++)
    {
            wrt=tmx[i];
            if(wrt) break;
    }
    //------------------------------------------------------------------------
    //Histogram
    //------------------------------------------------------------------------
    ndat=_NMAXTAB2;
    //zapisz dana do tablicy po pierwszym etapie filtracji
    for(i=1;i<ndat;i++) tbb2[i-1]=tbb2[i];
    tbb2[ndat-1]=wrt;	//nadpisz ostatnia pozycje aktualna dana

    //zlicz czestosc wystepowania wartosci
    for(i=0;i<ndat;i++) wb[i]=0;
    for(j=0;j<ndat;j++)
    {
            wrt=tbb2[j];
            for(i=0;i<ndat;i++)
            {
                    if(wrt==tbb2[i]) wb[j]++;
            }
    }
    //znajdz najczesciej wystepujaca wartosc
    max=0;
    im=ndat-1;
    for(i=0;i<ndat;i++)
    {
            wrt=wb[i];
            if(max<=wrt)
            {
                    max=wrt;
                    im=i;
            }
    }
    //znajdz wartosc ninimalna z najczesciej wystepujacych wartosci
    min=0xffff;
    for(i=0;i<ndat;i++)
    {
            if(wb[i]==max)
            {
                    wrt=tbb2[i];
                    if(min>=wrt)
                    {
                            min=wrt;
                            im=i;
                    }
            }
    }
    return tbb2[im];
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Nowa wartosc nastawy CO
unsigned char CalkNewPCO10V(float v)
{
    float k,p;
    if(PFN.SETPmax==PFN.SETPmin) return PFN.SETPmin;
    
    if (v<_F10V_ON) v=_F10V_ON;//return PFN.minPCO;
    if (v>_F10V_MAXPCO) v=_F10V_MAXPCO;//return PFN.maxPCO;
    k=(_F10V_MAXPCO-_F10V_ON)/(PFN.SETPmax-PFN.SETPmin);
    
    p=PFN.SETPmin+((v-_F10V_ON)/k);
    if(p<PFN.PCO10V)                //histereza programowa
    {
        p=round(p);
    }
    return (unsigned char)p;
}

/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Pomiar nastawy interfejsu 10V
void Pomiar10V(void)
{
    volatile static unsigned int avr=0;
    volatile unsigned int pom=0;
    volatile unsigned int AC_10V;
    float volt; 
    
    pom = ReadAC(0);
    //--------------
    pom = FiltrNTC(pom);
    //--------------
    avr= avr + ((long)pom - (long)avr) / 5;             //5 filtracja 3 stopnia, wartosc srednia pomiaru
    //--------------
    AC_10V = avr;       //wynik pomiaru

    volt=1.02429023794232*AC_10V+14.8517473414396;//temp. z regresji wielomianowej dla NTC:2322 640 5103
    //volt=1.02429023794232*AC_NTC+14.3049822501955;//temp. z regresji wielomianowej dla NTC:2322 640 5103
    if(volt>1000.0) volt=1000.0;
    if(volt<0.0) volt=0.0;

    //vvv=(unsigned char)(volt/10);
    //-----------------------------------------------------
#if _TEST_10V==0
    if(volt>=_F10V_OFF)  
    {
        PFNf_bEnF10V=1;         //znacznik do aktywacji interfejsu=1 
    }
    else
    {
        PFNf_bEnF10V=0;         //znacznik do aktywacji interfejsu=0
    }
    
    if(RdPrt(S_bEN_10V))        //aktywacja interfejsu V>=0.9V
    {
        PFNf_EnF10V=1;          //znacznik aktywnosci interfejsu
        if(volt>=_F10V_ON)
        {
            PFNf_b10V_RT=1;     //znacznik do aktywacja pobudzenia RT=1
        }
        else
        {
            PFNf_b10V_RT=0;     //znacznik do aktywacja pobudzenia RT=0
        }
        if(!PFNf_10V_RT&&RdPrt(S_b10V_RT)) //wlasnie przekroczono prog pobudzenia RT?
        {
            PFNf_10V_RT=1;          //ustaw znacznik pobudzenia RT
            SetPrt(S_10V_nPCO,1);   //aktywuj stan filtra (natychmiastowa aktualizacja nastawy)
        }
        
        if(PFNf_10V_RT)         //pobudzenie aktywne (przekroczono 2.2V)
        {
            PFN.PCO10V=CalkNewPCO10V(volt); //nowa wartosc docelowa temperatury
            if(PFN.PCO10V!=PFN.INSETP)         //zmiana wartosci nastawy?
            {
                if(PFNf_10V_nPCO&&(PFN.bPCO10V!=PFN.PCO10V))    //kolejna zmiana nastawy?
                {
                    SetPrt(S_10V_nPCO,0);   //restart filtra (odczekaj az wartosc sie ustabilizuje)
                }
                PFNf_10V_nPCO=1;            //znacznik do zmiany nastawy PCO=1   
                PFN.bPCO10V=PFN.PCO10V;                    
            }
         
            if(RdPrt(S_10V_nPCO))           //uzyskano stabilna wartosc nastawy?
            {
                PFNf_10V_nPCO=0;
                SetPrt(S_10V_nPCO,0); //restart filtra
                if((PFN.PCO10V>=PFN.SETPmin)&&(PFN.PCO10V<=PFN.SETPmax)) 
                {                
                    PFN.INSETP=PFN.PCO10V;     //nowa wartosc docelowa dla WG
                    PFN.bPCO10V=PFN.PCO10V;
                }                
            }
        }
        else
        {
            PFNf_10V_nPCO=0;    //znacznik do zmiany nastawy PCO=0
            PFN.bPCO10V=PFN.SETPmin;
        }
    }
    else    //deaktywacja interfejsu i pobudzenia
    {
        PFNf_EnF10V=0;
        PFNf_10V_RT=0;
        
        PFNf_b10V_RT=0;     //znacznik do aktywacja pobudzenia RT=0        
        PFNf_10V_nPCO=0;    //znacznik do zmiany nastawy PCO=0
        
        PFN.bPCO10V=PFN.SETPmin;
    }
#else
    static volatile unsigned char krk=0;
    
    PFNf_EnF10V=1;
    PFNf_10V_RT=1;    
    if(RdPrt(S_SET))
    {
        SetPrt(S_SET,0);
        krk=1;
    }
    else
    if(RdPrt(S_OFF))
    {
        SetPrt(S_OFF,0);
        krk=2;
    }
     
    switch(krk)
    {
        case 0:
        case 1:
            PFN.PCO10V=PFN.SETPmin;
            break;
        case 2:
            PFN.PCO10V=PFN.SETPmax;//PFN.SETPmin+13;
            break; 
    }         
    PFN.INSETP=PFN.PCO10V;     //nowa wartosc docelowa dla WG
#endif  
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
#endif //_EXT_BOARD!=0

