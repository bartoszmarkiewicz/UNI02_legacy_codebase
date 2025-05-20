/*komendy.c*/
/*Procedury zwiazane z obsluga automatu*/
//#include <pic18.h>
#include <xc.h>
#include "global.h"			//parametry globalne
#include "main.tp.h"
#include "kolejka.h"
#include "amgs_rs9b.h"		

extern volatile DataPFN PFN;
static volatile unsigned char timRing;
static unsigned char ZTN,KZTN;
extern volatile unsigned char xxy,zzz,vvv;
extern volatile tpRSDTA1 RSDT1;
extern volatile MGSRSData MGSD;
//----------------------------------------------------------
#if (_EXT_BOARD>=2)                     //aktywowany modul LIN1SLAVE?
#define _NPOZ_KOL 10 //9liczba pozycji kolejki liczac od 1
#else
#define _NPOZ_KOL 8  //liczba pozycji kolejki liczac od 1
#endif

//----------------------------------------------------------
//-------------------------------------------------------------------------------
//Timer na potrzeby kroku czasowego kolejkowania (umiescic w przerwaniu od zegara TMR1)
void IncRingTimer(void)
{
	if(timRing<255) timRing++;
}
//-------------------------------------------------------------------------------
//Restart zegara 
void StartRingTim(void)
{
	timRing=0;
}
//-------------------------------------------------------------------------------
//Odczyt stanu zegara 
unsigned char RingTim(void)
{
	return timRing;
}
//-------------------------------------------------------------------------------
//Adresowanie kolejnych pozycji kolejki
void ZtnToNext(unsigned char delay)
{
	if(delay)
	{
		switch(KZTN)
		{
			case 0:	
				StartRingTim();
				KZTN=1;
			case 1:	
				if(RingTim()>5)
				{
					ZTN++;
					if(ZTN>=_NPOZ_KOL) ZTN=0;
					KZTN=0;
					//zzz=ZTN;

				}
		}
	}
	else
	{
		ZTN++;
		if(ZTN>=_NPOZ_KOL) ZTN=0;
		KZTN=0;
	}
}
//----------------------------------------------------------
//Kolejka procedur

#if (_EXT_BOARD>=2)                     //aktywowany modul LIN1SLAVE?
void RingProc(void)
{
    volatile unsigned char rk,rs;

    if(RSDT1.vSTD<3)
    {
        rk=_RD_KNF;     //ramka podstawowa
        rs=_RD_STD;
    }
    else
    {
        rk=_RD_KNF2;    //ramka rozszerzona
        rs=_RD_STD2;        
    }
    
    switch(ZTN)
    {
        case 0:		//pozycja 1
        {
                if(TrToUNI01(rs)) ZtnToNext(0);     //paczka standardowa
                return;
        }
        case 1:
        {
                if(RSDT1f_NewLIDN)            //ndac komunikat LIN do UNI-02?
                {
                    if(RSDT1.ACKUNI=TrToUNI01(_RD_LIN))
                    {
                        RSDT1f_NewLIDN=0;
                        ZtnToNext(0);
                    }
                }else ZtnToNext(1);          //przerwa
                return;
        }
        case 2:		//pozycja 2
        {
                if(TrToUNI01(rk)) ZtnToNext(0);     //paczka konfiguracyjna
                return;
        }
        case 3:
        {
                if(RSDT1f_NewLIDN)            //ndac komunikat LIN do UNI-02?
                {
                    if(RSDT1.ACKUNI=TrToUNI01(_RD_LIN))
                    {
                        RSDT1f_NewLIDN=0;
                        ZtnToNext(0);
                    }
                }else ZtnToNext(1);          //przerwa
                return;
        }
        case 4:		//pozycja 3
        {
                if(TrToUNI01(rs)) ZtnToNext(0);
                return;
        }
        case 5:
        {
                if(RSDT1f_NewLIDN)            //ndac komunikat LIN do UNI-02?
                {
                    if(RSDT1.ACKUNI=TrToUNI01(_RD_LIN))
                    {
                        RSDT1f_NewLIDN=0;
                        ZtnToNext(0);
                    }
                }else ZtnToNext(1);          //przerwa
                return;
        }
        case 6:		//pozycja 4
        {
                if(PFNf_aktywne_KNF)
                {
                    if(TrToUNI01(rk))
                    {
                        ZtnToNext(0);
                        ZtnToNext(0);
                    }
                }
                else ZtnToNext(0);
                return;
        }
        case 7:		//lub pozycja 4
        {
                if(TrToUNI01(rs)) ZtnToNext(0);
                return;
        }
        case 8:
        {
                if(RSDT1f_NewLIDN)            //ndac komunikat LIN do UNI-02?
                {
                    if(RSDT1.ACKUNI=TrToUNI01(_RD_LIN))
                    {
                        RSDT1f_NewLIDN=0;
                        ZtnToNext(0);
                    }
                }else ZtnToNext(0);          
                return;
        }
        case 9:		//przerwa
        {
                ZtnToNext(1);                 //przerwa
                return;
        }        
    }
}
/*void RingProc(void)
{
    volatile unsigned char rk,rs;

    if(RSDT1.vSTD<3)
    {
        rk=_RD_KNF;     //ramka podstawowa
        rs=_RD_STD;
    }
    else
    {
        rk=_RD_KNF2;    //ramka rozszerzona
        rs=_RD_STD2;        
    }    
    switch(ZTN)
    {
        case 0:		//pozycja 1
        {
                if(TrToUNI01(rs)) ZtnToNext(0);
                return;
        }
        case 1:
        {
                if(RSDT1f_NewLIDN)            //ndac komunikat LIN do UNI-02?
                {
                    if(RSDT1.ACKUNI=TrToUNI01(_RD_LIN))
                    {
                        RSDT1f_NewLIDN=0;
                        ZtnToNext(0);
                    }
                }else ZtnToNext(1);          //przerwa
                return;
        }
        case 2:		//pozycja 2
        {
                if(PFNf_aktywne_KNF)
                {
                    if(TrToUNI01(rk))
                    {
                        ZtnToNext(0);
                        ZtnToNext(0);
                    }
                }
                else ZtnToNext(0);
                return;
        }
        case 3:         //lub pozycja 2
        {
                if(TrToUNI01(rs)) ZtnToNext(0);
                return;
        }
        case 4:
        {
                if(RSDT1f_NewLIDN)            //ndac komunikat LIN do UNI-02?
                {
                    if(RSDT1.ACKUNI=TrToUNI01(_RD_LIN))
                    {
                        RSDT1f_NewLIDN=0;
                        ZtnToNext(0);
                    }
                }else ZtnToNext(1);          //przerwa
                return;
        }
        case 5:		//pozycja 3
        {
                if(TrToUNI01(rs)) ZtnToNext(0);
                return;
        }
        case 6:
        {
                if(RSDT1f_NewLIDN)            //ndac komunikat LIN do UNI-02?
                {
                    if(RSDT1.ACKUNI=TrToUNI01(_RD_LIN))
                    {
                        RSDT1f_NewLIDN=0;
                        ZtnToNext(0);
                    }
                }else ZtnToNext(1);          //przerwa
                return;
        }
        case 7:		//pozycja 4
        {
                if(PFNf_aktywne_KNF)
                {
                    if(TrToUNI01(rk))
                    {
                        ZtnToNext(0);
                        ZtnToNext(0);
                    }
                }
                else ZtnToNext(0);
                return;
        }
        case 8:		//lub pozycja 4
        {
                if(TrToUNI01(rs)) ZtnToNext(0);
                return;
        }
        case 9:
        {
                if(RSDT1f_NewLIDN)            //ndac komunikat LIN do UNI-02?
                {
                    if(RSDT1.ACKUNI=TrToUNI01(_RD_LIN))
                    {
                        RSDT1f_NewLIDN=0;
                        ZtnToNext(0);
                    }
                }else ZtnToNext(1);          //przerwa
                return;
        }
    }
}*/
#else
void RingProc(void)
{
    volatile unsigned char rk,rs;
    
    //vvv=PFN.vUNI02;
    if(RSDT1.vSTD<3)
    {      
        rk=_RD_KNF;     //ramka podstawowa
        rs=_RD_STD;
    }
    else
    {
        rk=_RD_KNF2;    //ramka rozszerzona
        rs=_RD_STD2;        
    }
	switch(ZTN)
    {
        case 0:		//pozycja 1
            TrToUNI01(rs);
            return;
        case 1:		//pozycja 2
            if(PFNf_aktywne_KNF) TrToUNI01(rk);
            else ZtnToNext(1);
            return;
        case 2:		//pozycja 3
            if(PFNf_aktywne_KNF) TrToUNI01(rk);
            else TrToUNI01(rs);
            return;
        case 3:		//pozycja 4
            if(PFNf_aktywne_KNF) TrToUNI01(rk);
            else ZtnToNext(1);
            return;
        case 4:		//pozycja 5
            TrToUNI01(rs);
            return;
        case 5:		//pozycja 6
            if(PFNf_aktywne_KNF) TrToUNI01(rk);
            else ZtnToNext(1);
            return;
        case 6:		//pozycja 7
            if(PFNf_aktywne_KNF) TrToUNI01(rk);
            else TrToUNI01(rs);
        case 7:		//pozycja 8
            if(PFNf_aktywne_KNF) TrToUNI01(rs);
            else ZtnToNext(1);
            return;
    }
	/*switch(ZTN)
    {
        case 0:		//pozycja 1
            TrToUNI01(_RD_STD);
            return;
        case 1:		//pozycja 2
            if(PFNf_aktywne_KNF) TrToUNI01(_RD_KNF);
            else ZtnToNext(1);
            return;
        case 2:		//pozycja 3
            if(PFNf_aktywne_KNF) TrToUNI01(_RD_KNF);
            else TrToUNI01(_RD_STD);
            return;
        case 3:		//pozycja 4
                            if(PFNf_aktywne_KNF) TrToUNI01(_RD_KNF);
            else ZtnToNext(1);
            return;
        case 4:		//pozycja 5
            TrToUNI01(_RD_STD);
            return;
        case 5:		//pozycja 6
            if(PFNf_aktywne_KNF) TrToUNI01(_RD_KNF);
            else ZtnToNext(1);
            return;
        case 6:		//pozycja 7
            if(PFNf_aktywne_KNF) TrToUNI01(_RD_KNF);
            else TrToUNI01(_RD_STD);
        case 7:		//pozycja 8
            if(PFNf_aktywne_KNF) TrToUNI01(_RD_STD);
            else ZtnToNext(1);
            return;
    }*/    
}
#endif //(_EXT_BOARD>=2)
//----------------------------------------------------------
