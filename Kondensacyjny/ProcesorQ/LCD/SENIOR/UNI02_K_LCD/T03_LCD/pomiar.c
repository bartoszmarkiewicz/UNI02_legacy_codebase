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

volatile DataCO DtCO;
volatile DataCW DtCW;

#if _EXT_BOARD!=0
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Inicjalizacja DtCO, DtCW, DtCS, PFN
void InitDataPmr(void)
{
    InitTabNTC();
    
    PFNf_EnF10V=0;
    PFNf_10V_RT=0;
    PFNf_b10V_RT=0;       
    PFNf_10V_nPCO=0;
    PFN.bPCO10V=0;
    
    DtCO.flag0=0;
    DtCO.AC_PCO=0;
    DtCO.PCO=0;
    DtCO.minPCO=0;
    DtCO.maxPCO=0;
    
    DtCW.flag0=0;
    DtCW.AC_PCW=0;
    DtCW.PCW=0;
    DtCW.minPCW=0;
    DtCW.maxPCW=0;
    StartRTdS(_RTPMCO);
    StartRTdS(_RTPMCW);           
    StartRTS(_RTSTBCO);
    StartRTS(_RTSTBCW);
    
    StartRTdS(_RTWPCW);
    StartRTdS(_RTFPCW);   //zzz
    PFNf_no_wrtPCW=0; 
    PFNf_no_wrtPCW2=0;    //zzz
    StartRTdS(_RTWPCO);
    StartRTdS(_RTFPCO);   //zzz
    PFNf_no_wrtPCO=0;  
    PFNf_no_wrtPCO2=0;    //zzz
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
    
    pom = ReadAC(_CHN_10V);
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
            if((!PFNf_LIN_BPSS&&(PFN.PCO10V!=PFN.INSETP))||(PFNf_LIN_BPSS&&(PFN.PCO10V!=PFN.IINSETP)))         //zmiana wartosci nastawy?   //jjj
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
                if((PFN.PCO10V>=PFN.SETPmin)&&(PFN.PCO10V<=PFN.SETPmax)) //jjj
                {                
                    if(!PFNf_LIN_BPSS)  //jjj
                    {
                        PFN.INSETP=PFN.PCO10V;     //nowa wartosc docelowa dla WG
                        PFN.IINSETP=PFN.PCO10V;
                    }
                    else
                    {
                        PFN.IINSETP=PFN.PCO10V;
                    }
                    PFN.bPCO10V=PFN.PCO10V;
                }  
                else
                if(PFN.PCO10V<PFN.SETPmin)
                {
                    PFN.PCO10V=PFN.SETPmin;
                    if(!PFNf_LIN_BPSS)  //jjj
                    {
                        PFN.INSETP=PFN.PCO10V;     //nowa wartosc docelowa dla WG
                        PFN.IINSETP=PFN.PCO10V;
                    }
                    else
                    {
                        PFN.IINSETP=PFN.PCO10V;
                    }
                    PFN.bPCO10V=PFN.PCO10V;                        
                }
                else
                if(PFN.PCO10V>PFN.SETPmax)
                {
                    PFN.PCO10V=PFN.SETPmax;
                    if(!PFNf_LIN_BPSS)  //jjj
                    {
                        PFN.INSETP=PFN.PCO10V;     //nowa wartosc docelowa dla WG
                        PFN.IINSETP=PFN.PCO10V;
                    }
                    else
                    {
                        PFN.IINSETP=PFN.PCO10V;
                    }
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
    //TEST INTERFEJSU 10V
    static volatile unsigned char krk=0;    
    PFNf_EnF10V=1;
    
    PFNf_10V_RT=1;    //pobudzenie RT
    
    //-------------------------------
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
            PFN.PCO10V=PFN.SETPmin;                     //wartosc SETP po nacisnieciu SET
            break;
        case 2:
            PFN.PCO10V=PFN.SETPmax;//PFN.SETPmin+13;    //wartosc SETP po nacisnieciu RES
            break; 
    }         
    if(!PFNf_LIN_BPSS)  //jjj
    {
        PFN.INSETP=PFN.PCO10V;     //nowa wartosc docelowa dla WG
        PFN.IINSETP=PFN.PCO10V;
    }
    else
    {
        PFN.IINSETP=PFN.PCO10V;
    }
#endif
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Nastawa PCO
void PomiarPCO(void)
{
    volatile static unsigned int avr=_MAX_AC/2;
    volatile unsigned int pom=0;
    
	volatile static unsigned char pnst=0,pzro=0,pop=0;
	volatile unsigned char nst,dz,fkr;
    volatile float k;

    pom=ReadAC(_CHN_FPCO);
    avr= avr + ((long)pom - (long)avr) / 5;             //5 filtracja 3 stopnia, wartosc srednia pomiaru
    
    DtCO.AC_PCO=avr;      
    if(DtCO.AC_PCO>_MAX_AC)
    {
        DtCO.AC_PCO=_MAX_AC;
    }   
#if _NEG_AC==1
    DtCO.AC_PCO=_MAX_AC - DtCO.AC_PCO;
#endif
    
    //flaga dla pozycji ZERO
    if(DtCO.AC_PCO<=_MRG_ON_ZRO)
    {
        DtCO._pozZRO=1;
    }
    if(DtCO.AC_PCO>=_MRG_OFF_ZRO)
    {
        DtCO._pozZRO=0;
    }    
    //moc filtracji
    if(PFNf_FLIN||PFNf_FOT)
    {
        PFNf_hi_nco=1;
    }
    else
    {
        PFNf_hi_nco=0;
    }  
    //zakres nastawy
    if((DtCO.minPCO!=PFN.minPCO)||(DtCO.maxPCO!=PFN.maxPCO))
    {
        DtCO._newZKR=1;    //flaga zmiany zakresu
        DtCO._initFLT=1;   //wymuszona aktualizacja wartosci filtrowanych
    }
    DtCO.minPCO=(unsigned char)PFN.minPCO;
    DtCO.maxPCO=(unsigned char)PFN.maxPCO;
    
    //astawa PCO
    if(DtCO.AC_PCO<=_MRG_OFF_ZRO)
    {
        nst=DtCO.minPCO;
        if(DtCO.PCO!=nst)
        {
            StartRTS(_RTSTBCO);
        }
        DtCO.PCO=nst;   
        //---------------------        
        if(DtCO._initFLT)   //wymuszona aktualizacja wartosci filtrowanych
        {
            DtCO._initFLT=0;
        }
        //---------------------
		if(!DtCO._pozZRO&&nst!=pnst)
		{
 			DtCO._newPCO=1;		
		}
        //---------------------
        if(pzro!=DtCO._pozZRO)          //stan pozycji ZERO ulegl zmianie?
        {
            DtCO._newPOZ=1;             //ustaw flage zmiany pozycji            
        }
        pzro=(DtCO._pozZRO)?1:0; 
        //---------------------
        pnst=nst; 
        pop=nst;
    }
    else
    {    
        k=(_MAX_AC-_MRG_OFF_ZRO)/((DtCO.maxPCO+1)-DtCO.minPCO);
        k=((DtCO.AC_PCO-_MRG_OFF_ZRO)/k);
        dz=(unsigned char)k;
		nst=DtCO.minPCO+dz;
        //----------------------------
        if(pzro!=DtCO._pozZRO)          //stan pozycji ZERO ulegl zmianie?
        {
            DtCO._newPOZ=1;             //ustaw flage zmiany pozycji
            if(!DtCO._pozZRO)
            {
                DtCO._initFLT=1;
                PFNf_hi_nco=0;
            }            
        }
        pzro=(DtCO._pozZRO)?1:0;
        //----------------------------
        if(DtCO._initFLT)   //wymuszona aktualizacja wartosci filtrowanych
        {
            DtCO._initFLT=0;
            pop=nst;
            pnst=nst;
        }
        nst=(nst+pop)/2;
        pop=nst;
        //----------------------------
        if(RTdS(_RTPMCO)>30)        //timer modyfikacji nastawy przekrocxzyl ustalony czas?
        {
            DtCO._toUP=0;           //zeruj flagi modyfikacji nastawy                
            DtCO._toDW=0;            
        }
		if(nst<=DtCO.maxPCO)        //nastawa w zakresie dozwolonym?
        {
            //dobor marginesu filtracji w zaleznosci od szerokosci zakresu nastawy
            fkr=2;          //domyslnie +/-2
            if((DtCO.maxPCO-DtCO.minPCO)>50)        //zakres wiekszy od 50 pkt.?
            {
                fkr=3;      //+/-3
            }                 
            if(PFNf_hi_nco) //zwiekszony prog wejscia przy aktywmym interfejsie LIN
            {
                fkr=fkr*2;
            }
            if(!DtCO._toUP&&!DtCO._toDW)    //nieaktywny tryb modyfikacji nastawy?
            {
                if((nst<DtCO.maxPCO-1)&&((nst-pnst>=0)&&(nst-pnst)<fkr))  //zmiana w gore w zakresie marginesu filtracji?
                {
                     nst=pnst;	//filtr nastawy
                     DtCO._newPCO=0;
                }
                else
                if((nst>DtCO.minPCO+1)&&(pnst-nst>=0)&&(pnst-nst)<fkr)  //zmiana w dol w zakresie marginesu filtracji?
                {
                     nst=pnst;	//filtr nastawy 
                     DtCO._newPCO=0;
                }
                else    //zmiana przekroczyla margines filtracji?
                {
                    PFNf_hi_nco=0;      //na czas zmiany zmniejsz prog wejscia
                    DtCO._toUP=0;                
                    DtCO._toDW=0;                                    
                    if(nst-pnst>0)  //zmiana w gore?
                    {
                        DtCO._toUP=1;       //flaga modyfikacji w gore

                    }
                    else            //zmiana w dol
                    {
                        DtCO._toDW=1;       //flaga modyfikacji w dol                  
                    }
                }
            }
            if(DtCO._toUP)      //zmiana nastawy w gore
            {
                if((nst<DtCO.maxPCO-1)&&((nst-pnst>=0)&&(nst-pnst)<2)) //zmiana w gore w zakresie marginesu filtracji?
                {
                     nst=pnst;	//filtr nastawy 
                     DtCO._newPCO=0;
                }
                else    //zmiana przekroczyla margines filtracji?
                {
                    if(nst!=pnst)          //nastapila zmiana stanu nastawy?
                    {
                        DtCO._newPCO=1;         //ustaw flage zmiany		
                        StartRTdS(_RTPMCO);     
                    }                
                    pnst=nst;
                }                            
            }
            else
            if(DtCO._toDW)  //zmiana nastawy w dol    
            {
                if((nst>DtCO.minPCO+1)&&(pnst-nst>=0)&&(pnst-nst)<2) //zmiana w dol w zakresie marginesu filtracji?
                {
                     nst=pnst;	//filtr nastawy 
                     DtCO._newPCO=0;
                }
                else        //zmiana przekroczyla margines filtracji?
                {
                    if(nst!=pnst)          //nastapila zmiana stanu nastawy?
                    {
                        DtCO._newPCO=1;         //ustaw flage zmiany	
                        StartRTdS(_RTPMCO);                        
                    }                
                    pnst=nst;
                }                             
            }           
        }
        else
        {            
            nst=DtCO.maxPCO;        //przyjmij wartosc maksymalna zakresu
            if(nst!=pnst)           //nastapila zmiana stanu nastawy?
            {
                DtCO._newPCO=1;     //ustaw flage zmiany			
            }            
            pnst=nst;
        }
        if(DtCO.PCO!=nst)
        {
            StartRTS(_RTSTBCO);
        }
		DtCO.PCO=nst;
    }
    //----------------------------    
    if(DtCO._newPCO)
    {
        StartRTdS(_RTWPCO);
        PFNf_no_wrtPCO=1; 
        StartRTdS(_RTFPCO);
        PFNf_no_wrtPCO2=1;     //zzz   
        StartRTS(_RTSTBCO);
        DtCO._stabPCO=0;
    }    
    //---------------------------- 
    if(DtCO._newZKR)                //zmiana zakresu nastawy?
    {
        DtCO._newPCO=0;
    }
    //----------------------------
    if(RTS(_RTSTBCO)>_TSTABP)       //nastawa ulega zmianie?
    {
        DtCO._stabPCO=1;
    }  
    //----------------------------
    if(RTdS(_RTWPCO)>10)
    {
        PFNf_no_wrtPCO=0;        
    }    
    if(RTdS(_RTFPCO)>100)           //zzz
    {
        PFNf_no_wrtPCO2=0;        
    }     
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Nastawa PCW
void PomiarPCW(void)
{
    volatile static unsigned int avr=_MAX_AC/2;
    volatile unsigned int pom=0;
    
	volatile static unsigned char pnst=0,pzro=0,pop=0;
	volatile unsigned char nst,dz,fkr,zmin;
    volatile float k;    

    pom=ReadAC(_CHN_FPCW);
    avr= avr + ((long)pom - (long)avr) / 5;             //5 filtracja 3 stopnia, wartosc srednia pomiaru
    DtCW.AC_PCW=avr;
    
    if(DtCW.AC_PCW>_MAX_AC)
    {
        DtCW.AC_PCW=_MAX_AC;
    }
#if _NEG_AC==1
    DtCW.AC_PCW=_MAX_AC - DtCW.AC_PCW;
#endif
    //flaga dla pozycji ZERO
    if(DtCW.AC_PCW<=_MRG_ON_ZRO)
    {
        DtCW._pozZRO=1;
    }
    if(DtCW.AC_PCW>=_MRG_OFF_ZRO)
    {
        DtCW._pozZRO=0;
    }    
    //moc filtracji
    if(PFNf_FLIN||PFNf_FOT)
    {
        PFNf_hi_ncw=1;
    }
    else
    {
        PFNf_hi_ncw=0;
    }
    //zakres nastawy
    if(!PFNf_EnAleg)
    {
        zmin=(unsigned char)PFN.minPCW;
    }
    else
    {
        zmin=(unsigned char)(PFN.minPCW-1);
    }    
    if((DtCW.minPCW!=zmin)||(DtCW.maxPCW!=PFN.maxPCW))
    {
        DtCW._newZKR=1;    //flaga zmiany zakresu
        DtCW._initFLT=1;   //wymuszona aktualizacja wartosci filtrowanych
    }    
    DtCW.minPCW=zmin;
    DtCW.maxPCW=(unsigned char)PFN.maxPCW;
    
   //nastawa PCW    
    if(DtCW.AC_PCW<=_MRG_OFF_ZRO)  
    {
        nst=DtCW.minPCW;
        if(DtCW.PCW!=nst)
        {
            StartRTS(_RTSTBCW);
        }        
        DtCW.PCW=nst;
        //---------------------          
        if(DtCW._initFLT)   //wymuszona aktualizacja wartosci filtrowanych
        {
            DtCW._initFLT=0;
        }        
        //---------------------          
		if(!DtCW._pozZRO&&nst!=pnst)
		{
 			DtCW._newPCW=1;		
		}
        //---------------------
        if(pzro!=DtCW._pozZRO)          //stan pozycji ZERO ulegl zmianie?
        {
            DtCW._newPOZ=1;             //ustaw flage zmiany pozycji            
        }
        pzro=(DtCW._pozZRO)?1:0; 
        //---------------------
		pnst=nst; 
        pop=nst;
    }
    else
    {  
        
        k=(_MAX_AC-_MRG_OFF_ZRO)/((DtCW.maxPCW+1)-DtCW.minPCW);
        k=((DtCW.AC_PCW-_MRG_OFF_ZRO)/k);
        dz=(unsigned char)k;
		nst=DtCW.minPCW+dz;
        //----------------------------
        if(pzro!=DtCW._pozZRO)          //stan pozycji ZERO ulegl zmianie?
        {
            if(!DtCW._pozZRO)
            {
                DtCW._initFLT=1;
                PFNf_hi_ncw=0;
            }
            DtCW._newPOZ=1;             //ustaw flage zmiany pozycji
        }
        pzro=(DtCW._pozZRO)?1:0;  
        //----------------------------      
        if(DtCW._initFLT)   //wymuszona aktualizacja wartosci filtrowanych
        {
            DtCW._initFLT=0;
            pop=nst;
            pnst=nst;
        }        
        nst=(nst+pop)/2;
        pop=nst;        
        //---------------------------- 
        if(RTdS(_RTPMCW)>30)        //timer modyfikacji nastawy przekrocxzyl ustalony czas?
        {
            DtCW._toUP=0;           //zeruj flagi modyfikacji nastawy                
            DtCW._toDW=0;            
        }
		if(nst<=DtCW.maxPCW)        //nastawa w zakresie dozwolonym?
        {
            //dobor marginesu filtracji w zaleznosci od szerokosci zakresu nastawy
            fkr=2;          //domyslnie +/-2
            if((DtCW.maxPCW-DtCW.minPCW)>50)        //zakres wiekszy od 50 pkt.?
            {
                fkr=3;      //+/-3
            }                 
            if(PFNf_hi_ncw) //zwiekszony prog wejscia przy aktywmym interfejsie LIN
            {
                fkr=fkr*2;
            }
            if(!DtCW._toUP&&!DtCW._toDW)    //nieaktywny tryb modyfikacji nastawy?
            {
                if((nst<DtCW.maxPCW-1)&&((nst-pnst>=0)&&(nst-pnst)<fkr))  //zmiana w gore w zakresie marginesu filtracji?
                {
                     nst=pnst;	//filtr nastawy
                     DtCW._newPCW=0;
                }
                else
                if((nst>DtCW.minPCW+1)&&(pnst-nst>=0)&&(pnst-nst)<fkr)  //zmiana w dol w zakresie marginesu filtracji?
                {
                     nst=pnst;	//filtr nastawy 
                     DtCW._newPCW=0;
                }
                else    //zmiana przekroczyla margines filtracji?
                {
                    PFNf_hi_ncw=0;      //na czas zmiany zmniejsz prog wejscia
                    DtCW._toUP=0;                
                    DtCW._toDW=0;                                    
                    if(nst-pnst>0)  //zmiana w gore?
                    {
                        DtCW._toUP=1;       //flaga modyfikacji w gore

                    }
                    else            //zmiana w dol
                    {
                        DtCW._toDW=1;       //flaga modyfikacji w dol                  
                    }
                }
            }
            if(DtCW._toUP)      //zmiana nastawy w gore
            {
                if((nst<DtCW.maxPCW-1)&&((nst-pnst>=0)&&(nst-pnst)<2)) //zmiana w gore w zakresie marginesu filtracji?
                {
                     nst=pnst;	//filtr nastawy 
                     DtCW._newPCW=0;
                }
                else    //zmiana przekroczyla margines filtracji?
                {
                    if(nst!=pnst)          //nastapila zmiana stanu nastawy?
                    {
                        DtCW._newPCW=1;         //ustaw flage zmiany		
                        StartRTdS(_RTPMCW);     
                    }                
                    pnst=nst;
                }                            
            }
            else
            if(DtCW._toDW)  //zmiana nastawy w dol    
            {
                if((nst>DtCW.minPCW+1)&&(pnst-nst>=0)&&(pnst-nst)<2) //zmiana w dol w zakresie marginesu filtracji?
                {
                     nst=pnst;	//filtr nastawy 
                     DtCW._newPCW=0;
                }
                else        //zmiana przekroczyla margines filtracji?
                {
                    if(nst!=pnst)          //nastapila zmiana stanu nastawy?
                    {
                        DtCW._newPCW=1;         //ustaw flage zmiany	
                        StartRTdS(_RTPMCW);                        
                    }                
                    pnst=nst;
                }                             
            }           
        }
        else
        {            
            nst=DtCW.maxPCW;        //przyjmij wartosc maksymalna zakresu
            if(nst!=pnst)           //nastapila zmiana stanu nastawy?
            {
                DtCW._newPCW=1;     //ustaw flage zmiany			
            }            
            pnst=nst;
        }
        if(DtCW.PCW!=nst)
        {
            StartRTS(_RTSTBCW);
        }         
		DtCW.PCW=nst;
    }
    //----------------------------    
    if(DtCW._newPCW)
    {
        StartRTdS(_RTWPCW);
        PFNf_no_wrtPCW=1;   
        StartRTdS(_RTFPCW);
        PFNf_no_wrtPCW2=1;        //zzz
        StartRTS(_RTSTBCW);
        DtCW._stabPCW=0;        
    }    
    //---------------------------- 
    if(DtCW._newZKR)                //zmiana zakresu nastawy?
    {
        DtCW._newPCW=0;
    } 
    //----------------------------
    if(RTS(_RTSTBCW)>_TSTABP)       //nastawa ulega zmianie?
    {
        DtCW._stabPCW=1;
    }  
    //----------------------------    
    if(RTdS(_RTWPCW)>10)
    {
        PFNf_no_wrtPCW=0;        
    }
    if(RTdS(_RTFPCW)>100)       //zzz
    {
        PFNf_no_wrtPCW2=0;        
    }    
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
void ChkZKR(void)
{
    if(DtCW._newZKR)    //zmiana zakresu nastawy?
    {
        DtCW._newZKR=0;
        if(PFN.PCW<DtCW.minPCW)
        {
            PFN.PCW=(unsigned int)DtCW.minPCW;
        }
        if(PFN.PCW>DtCW.maxPCW)
        {
            PFN.PCW=(unsigned int)DtCW.maxPCW;
        }                    
    }
    if(DtCO._newZKR)    //zmiana zakresu nastawy?
    {
        DtCO._newZKR=0;
        if(PFN.PCO<DtCO.minPCO)
        {
            PFN.PCO=(unsigned int)DtCO.minPCO;
        }
        if(PFN.PCO>DtCO.maxPCO)
        {
            PFN.PCO=(unsigned int)DtCO.maxPCO;
        }                     
    }     
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Aktualizacja nastaw
void SetNST(void)
{
    PFN.PCO=(unsigned int)DtCO.PCO;
    PFN.PCW=(unsigned int)DtCW.PCW;     
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Aktualizacja trybu pracy
void SetTrbPRC(void)
{
    if(!PFNf_aktywne_LKNF)
    {
        if(DtCW._pozZRO&&DtCO._pozZRO)
        {
            PFNf_off=1;
        }
        else
        {
            PFNf_off=0;
            if(!DtCO._pozZRO)
            {
                PFNf_lato=0;
                PFNf_zima=1;                
            }
            else
            {
                PFNf_lato=1;
                PFNf_zima=0;                    
            }
        }     
    }
    else
    {
        PFNf_off=1;
    }
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
#endif //_EXT_BOARD!=0

