/*LIN1SInterpreter.c*/
/*Reakcja na odpowiedz SLAVE*/
//#include <pic18.h>
#include <xc.h>
#include <stdio.h>
#include "global.h"			//parametry globalne
#include "wersja.h"			//parametry globalne
#include "main.tp.h"			
#if (_EXT_BOARD>=2)                     //aktywowany modul LIN1SLAVE?

#include "LIN1SInterpreter.h"
#include "amgs_rs9b.h"
#include "konfig3.h"
#include "ot.tp.h"

extern volatile unsigned char stbf[32];
extern unsigned char tbuf[];
extern FRAME_LIN1 FRL1,FTL1;
extern volatile tpRSDTA1 RSDT1;
extern CDtPZK CPZK[];
extern volatile CMSG_PAR CMSGPar;
extern volatile CMSG_PAR2 CMSGDta;
extern volatile tpOTMaster OTMaster;
extern volatile DataPFN PFN;
extern volatile DataKFN KFN;
//unsigned char ccc;
//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
void LIN2SClrMasterIDO(void)
{
    FRL1.MSTIDO=0x00;   //zeruj identyfikator nadajnika
    PFNf_LIN_PWD=0;    
    PFNf_LIN_BUF=0;    
}
void LIN2SetMasterIDO(const unsigned char dt)
{
    if(FRL1.RND<4)          //dane nie zawieraja informacji o identyfikatorze?
    {
        FRL1.MSTIDO=0;      //wyzeruj identyfikator
        PFNf_LIN_PWD=0;
        PFNf_LIN_BUF=0;        
    }
    else    
    {
        if(PFNf_fenPWD)                                     //plyta sterujaca wspiera przewyzszenie i prace z buforem?
        {
            if(dt&0b10000000)                                       //wybrany bit identyfikatora
            {
                PFNf_LIN_PWD=1;                                 //ustaw flage sterowania z przewyzszeniem
            }
            else
            {
                PFNf_LIN_PWD=0;            
            }  
            if(dt&0b01000000)                                       //wybrany bit identyfikatora
            {
                PFNf_LIN_BUF=1;                                 //ustaw flage sterowania z buforem
            }
            else
            {
                PFNf_LIN_BUF=0;            
            }        
        }
        else
        {
            PFNf_LIN_PWD=0;
            PFNf_LIN_BUF=0;            
        }
        FRL1.MSTIDO=dt&0b00111111;     //przypisz identyfikator nadajnika
    }
}
unsigned char LIN2SGetMasterIDO(void)
{
    return FRL1.MSTIDO;   //zeruj identyfikator nadajnika    
}
//------------------------------------------------------------
//------------------------------------------------------------
//******************************** POLECENIE WYKONANIA POLECENIA/ZAPISU DANYCH OTRZYMANYCH Z MASTER **************************************
//------------------------------------------------------------
//------------------------------------------------------------
//SINGLE RESPONSE
//Interpretacja komendy i odebranych danych
//UWAGA: zwrot wartosci _LIN1_IBUSY sygnalizuje trwanie procesu (maksymalny czas zajetosci zdefiniowany parametrem _LN1_ADTA)
unsigned char LIN1SingleResponse_WR(void)
{
    static unsigned char krok,bidn;

    BitSet3(&RSDT1.inLNSTAT,_FLIN,1);        //odswiez znacznik komunikacji LIN dla ukladu UNI-02
    PFNf_FLIN=1;
    StartRTdS(_RTFLIN);    
    if(FRL1f_RSFrame)                 //ramka nadeslana z modulu Wifi?
    {
        if(!RSFInterpreter_SingleWR()) return _LIN1_UNCCOM;       
    }
    //----------------
    if(bidn!=FRL1.IDN)                      //przy zmianie komendy zeruj stan procedury bezposredniej komunikacji z UNI-02
    {
        bidn=FRL1.IDN;
        krok=0;
    }
    //komenda na ktora nastapila reakcja ukladu SLAVE
    switch(FRL1.IDN)                        //identyfikator komendy
    {
        case _CLIN_WRD_LADDR:                //zmien adres LIN plytki sterujacej UNI-02 (nowy adres w parametr1)
        case _CLIN_WRD_LIFADDR:              //zmien adres LIN plytki sterujacej UNI-02 (nowy adres w parametr1) po spelnieniu warunku
        {
            if((FRL1.IDN!=_CLIN_WRD_LADDR)) PFNf_fshowLaddr=0;
            //konieczna odpowiedz zwrotna UNI-02 do KSK stanowiaca potwierdzenie wykonania komendy
            switch(krok)
            {
                case 0:         //0.Odczekaj na zakonczenie poprzedniej transmisji do UNI-02
                    if(!LINWaitForAnswerUNI02(_LIN_SADR)) return _LIN1_IBUSY;
                    krok++;
                case 1:         //1.Wysylka komendy do UNI-02
                {
                    //wstepny warunek
                    if((FRL1.IDN==_CLIN_WRD_LIFADDR))  //warunkowe przypisanie adresu odbiornikowi z adresem neutralnym?
                    {
                        PFNf_fsetLaddr=1;               //ustaw tryb wyswietlania adresu lin
                        StartRTdS(_RTALIN);             //timer do autowyjscia z trybu
#if _KEY_BBOARD==0                         
                        if(PFNf_setKey&&RdPrt(S_SET))   //przytrzymany przycisk SET?
                        {
                            return _LIN1_NORESP;        //zignoruj polecenie
                        }
#endif
#if _KEY_BBOARD==1                         
                        if(PFNf_setKey&&RdPrt(S_KOM))   //przytrzymany przycisk KOM?
                        {
                            return _LIN1_NORESP;        //zignoruj polecenie
                        }
#endif                        
                        //warunek dla _CLIN_WRD_LIFADR
#if _KEY_BBOARD==0                         
                        if(!RdPrt(S_SET))               //puszczony przycisk SET?
                        {
                            PFNf_setKey=0;
                            return _LIN1_NORESP;        //zignoruj polecenie
                        }
#endif
#if _KEY_BBOARD==1                         
                        if(!RdPrt(S_KOM))               //puszczony przycisk KOM?
                        {
                            PFNf_setKey=0;
                            return _LIN1_NORESP;        //zignoruj polecenie
                        }
#endif                        
                        PFNf_setKey=1;                  //nacisniety przycisk SET - wykonaj polecenie
                        if(PFN.ALIN==FRL1.WRD1.byte0)   //adres zgodny z parametrem1?
                        {
                            if(FRL1.NAD) return _LIN1_IFINAL;
                            else return _LIN1_NORESP;
                        }
                    }
                    //komendy i parametry wywolania dla UNI-02
                    RSDT1.inLIDN=_LIN_SADR;             //komenda
                    RSDT1.inLDTA1=FRL1.WRD1.byte0;      //parametr1
                    
                    //start wywolania
                    if(!FRL1.NAD) FRL1f_ZeroAdr=1;
                    else FRL1f_ZeroAdr=0;
                    RSDT1f_NewLIDN=1;
                    RSDT1.ACKUNI=0;
                    FRL1f_NextFr=0;
                    krok++;
                }
                case 2:         //2.Oczekiwanie na odpowiedz
                {
                    switch(LINWaitForAnswerUNI02(_LIN_SADR))
                    {
                        case 0:
                            return _LIN1_IBUSY;
                        case 1:
                            krok++;
                            break;
                        default:
                            krok=0;
                            return _LIN1_ERRODP;
                    }
                }
                case 3:         //3.Przygotowanie danych dla LIN
                {
                    krok=0;
                    if(FRL1f_ZeroAdr) return _LIN1_NORESP;                               //odpowiedz do LIN nie ma byc generowana?
                    if(!FRL1f_NextFr)                                                    //w trakcie wysylki do UNI-02 nie bylo aktualizacji inna ramka LIN?
                    {
                        if(RSDT1.outLDTA1!=RSDT1.inLDTA1)                               //kontrlola wykonania polecenia przez UNI-02
                        {
                            return _LIN1_ERRODP;
                        }
                    }
                    else                                                                //aktualizowano ramke - odpowiedz negatywna
                    {
                        return _LIN1_ERRODP;
                    }
                }
            }
            break;
        }
        case _CLIN_WRD_SHOWADDR:    //wyswietl adres LIN
        {
            if(!PFNf_fsetLaddr)
            {
                PFNf_fshowLaddr=1;                  //ustaw tryb wyswietlania adresu lin
                StartRTdS(_RTALIN);                 //timer do autowyjscia z trybu
            }
            else PFNf_fshowLaddr=0;
            if(!FRL1.NAD) return _LIN1_NORESP;
            break;
        }
        case _CLIN_WRD_PULSE:   //pulsacja podswietlenia
        {
            switch(FRL1.WRD1.byte0)
            {
                case 0: //wylacz pulsacje
                {
                    PFNf_fshowLpulse=0;            //ustaw tryb wyswietlania adresu lin
                    PFNf_foneLpulse=0;             //ustaw tryb wyswietlania adresu lin
                    break;
                }
                case 1: //wlacz pulsacje z autowygaszeniem
                {
                    if((!PFNf_fshowLpulse)||(!PFNf_foneLpulse));            //ustaw tryb wyswietlania adresu lin
                    {
                        PFNf_fshowLpulse=1;                 //ustaw tryb pulsaacji podswietlenia
                        PFNf_foneLpulse=1;
                        StartRTdS(_RTALIN);                 //timer do autowyjscia z trybu pulsacji podswietlenia
                    }
                    break;
                }
                default: //wlacz ciagla pulsacje
                {
                    PFNf_fshowLpulse=1;            //ustaw tryb wyswietlania adresu lin
                    PFNf_foneLpulse=0;
                    break;
                }
            }
            if(!FRL1.NAD) return _LIN1_NORESP;
            break;
        }
        case _CLIN_WRD_PCW:     //zmiana nastawy CW
        {
            if(!PFNf_EnAleg)
            {
                if((FRL1.WRD1.byte0>=PFN.minPCW)&&(FRL1.WRD1.byte0<=PFN.maxPCW))
                {
                    PFN.PCW=(unsigned int)FRL1.WRD1.byte0;
                }
                else return _LIN1_ERRODP;
            }
            else
            {
                if((FRL1.WRD1.byte0>=PFN.minPCW-1)&&(FRL1.WRD1.byte0<=PFN.maxPCW))
                {
                    PFN.PCW=(unsigned int)FRL1.WRD1.byte0;
                }
                else return _LIN1_ERRODP;
            }                
            break;
        }  
        case _CLIN_WRD_PCO:     //zmiana nastawy CO
        {
            if((FRL1.WRD1.byte0>=PFN.minPCO)&&(FRL1.WRD1.byte0<=PFN.maxPCO))
            {
                PFN.PCO=(unsigned int)FRL1.WRD1.byte0;
            }
            else return _LIN1_ERRODP;
            break;
        }
        case _CLIN_WRD_ECO:     //zmiana nastawy ECO
        {
            if((FRL1.WRD1.byte0>=1)&&(FRL1.WRD1.byte0<=9))
            {
                PFN.ECO=(unsigned int)FRL1.WRD1.byte0;
            }
            else return _LIN1_ERRODP;
            break;
        }  
        case _CLIN_WRD_TRB:     //zmiana trybu pracy
        {
            //ccc=FRL1.WRD1.byte0;
            if(FRL1.WRD1.byte0<=2)
            {
                if(FRL1.WRD1.byte0==2)
                {
                    PFNf_off=1;                                     //tryb OFF
                }
                else
                if(FRL1.WRD1.byte0==1)
                {
                    PFNf_off=0;
                    PFNf_lato=1;                                    //tryb LATO
                    PFNf_zima=0;
                }
                else
                {
                    PFNf_off=0;
                    PFNf_lato=0;                                    
                    PFNf_zima=1;                                    //tryb ZIMA
                }
            }
            else return _LIN1_ERRODP;
            break;
        }
        case _CLIN_WRD_RES:      //aktywowac przycisk RESET?
        {
            PFNf_res=1;
            SetPrt(S_OFF,1);
            PFNf_set_nco=0;
            PFNf_set_ncw=0;
            PFNf_str_nco=0;
            PFNf_str_ncw=0;
            PFNf_mod_ncw=0;
            PFNf_set_eco=0;
            PFNf_mod_eco=0;
            PFNf_prn_res=0;
            PFNf_set_fnserw=0;
            PFNf_fnserw=0;
            PFNf_set_aleg=0;
            PFNf_aleg=0;            
            break;
        }          
        case _CLIN_WRD_BRES:      //usunac blokade awaryjna?
        {
            if(PFNf_opgres)       //wylaczenie awaryjne?
            {
                if(FRL1.WRD1.byte0==0xAA)
                {
                    PFNf_LINres=1;
                }
                else 
                {
                    PFNf_LINres=0;                
                }
            }
            else
            {
                PFNf_LINres=0;                
            }
            break;
        }
        case _CLIN_WRD_FSR:      //aktywuj/deaktywuj funkcje serwisowa
        {
            if(!PFNf_fsetLaddr&&!PFNf_aktywne_KNF&&!PFNf_opgres)     //blokada nieaktywna?
            {
                if(FRL1.WRD1.byte0)
                {
                    if(PFNf_zima&&!PFNf_obgCW&&!PFNf_set_fnserw)
                    {
                        if(!PFNf_ferr&&!PFNf_fmsg&&!PFNf_off&&!PFNf_fnserw&&!PFNf_aleg)
                        {
                            PFNf_set_fnserw=1;
                            PFNf_fnserw=1;
                            PFNf_prn_res=0;
                            PFNf_set_nco=0;
                            PFNf_set_ncw=0;
                            PFNf_set_eco=0;
                            PFNf_b_ofnserw=0;
                            StartRTdS(_RTFSP);
                        }
                    }
                    if(!PFNf_off&&(PFNf_fnserw||PFNf_ofnserw))					//aktywna funkcja serwisowa?
                    {
                        if(FRL1.WRD1.byte0==2)PFNf_fns_max=0;
                        else PFNf_fns_max=1;                    
                    }
                }
                else
                {
                    PFNf_LINresSR=1;
                    PFNf_set_fnserw=0;
                    PFNf_fnserw=0;                
                }
            }
            break;
        }
        case _CLIN_WRD_ANL:      //aktywuj/deaktywuj funkcje antylegionella
        {
            if(!PFNf_fsetLaddr&&!PFNf_aktywne_KNF&&!PFNf_opgres)     //blokada nieaktywna?
            {            
                if(FRL1.WRD1.byte0)
                {
                    if(PFNf_EnAleg&&!PFNf_set_aleg)
                    {
                        if(!PFNf_ferr&&!PFNf_fmsg&&!PFNf_off&&!PFNf_fnserw&&!PFNf_aleg)
                        {
                            PFNf_set_aleg=1;
                            PFNf_aleg=1;
                            PFNf_set_nco=0;
                            PFNf_set_ncw=0;
                            PFNf_set_eco=0;
                            PFNf_b_oaleg=0;
                            StartRTdS(_RTFSP);
                        }
                    }
                }
                else
                {
                    PFNf_LINresAL=1;
                    PFNf_set_aleg=0;
                    PFNf_aleg=0;                
                }
            }
            break;
        }    
        case _CLIN_WRD_PKNF:              //zmien parametr konfiguracyjny plytki sterujacej UNI-02 
        {
            //konieczna odpowiedz zwrotna UNI-02 do KSK stanowiaca potwierdzenie wykonania komendy
            switch(krok)
            {
                case 0:         //0.Odczekaj na zakonczenie poprzedniej transmisji do UNI-02
                    if(!LINWaitForAnswerUNI02(_LIN_WPKNF)) return _LIN1_IBUSY;
                    krok++;
                case 1:         //1.Wysylka komendy do UNI-02
                {
                    //komendy i parametry wywolania dla UNI-02
                    RSDT1.inLIDN=_LIN_WPKNF;             //komenda
                    if(FRL1.WRD1.byte0<GetKNPAR())
                    {
                        RSDT1.inLDTA1=FRL1.WRD1.byte0;      //parametr1 (indeks parametru)
                    }
                    else
                    {
                        krok=0;
                        return _LIN1_ERRODP; 
                    }
                    RSDT1.inLDTA2=FRL1.WRD1.byte1;      //parametr1 (wartosc parametru)                     
                    //start wywolania
                    if(!FRL1.NAD) FRL1f_ZeroAdr=1;
                    else FRL1f_ZeroAdr=0;
                    RSDT1f_NewLIDN=1;
                    RSDT1.ACKUNI=0;
                    FRL1f_NextFr=0;
                    krok++;
                }
                case 2:         //2.Oczekiwanie na odpowiedz
                {
                    switch(LINWaitForAnswerUNI02(_LIN_WPKNF))
                    {
                        case 0:
                            return _LIN1_IBUSY;
                        case 1:
                            krok++;
                            break;
                        default:
                            krok=0;
                            return _LIN1_ERRODP;
                    }
                }
                case 3:         //3.Przygotowanie danych dla LIN
                {
                    krok=0;
                    if(FRL1f_ZeroAdr) return _LIN1_NORESP;                              //odpowiedz do LIN nie ma byc generowana?
                    if(!FRL1f_NextFr)                                                   //w trakcie wysylki do UNI-02 nie bylo aktualizacji inna ramka LIN?
                    {
                        if(RSDT1.outLDTA2) return _LIN1_ERRODP;                         //status operacji na UNI-02 wskazuje na blad?
                    }
                    else                                                                //aktualizowano ramke - odpowiedz negatywna
                    {
                        return _LIN1_ERRODP;
                    }
                }
            }
            break;
        }
        case _CLIN_WRD_P:              //zmien parametr konfiguracyjny plytki sterujacej UNI-02 
        case _CLIN_WRD_I:                
        case _CLIN_WRD_D:   
        case _CLIN_WRD_RPV:
        case _CLIN_WRD_RCO1:
        case _CLIN_WRD_RCO2:
        case _CLIN_WRD_RCO3:
        case _CLIN_WRD_RCO4:  
        case _CLIN_WRD_RCW1:
        case _CLIN_WRD_RCW2:
        case _CLIN_WRD_RCW3:
        case _CLIN_WRD_RCW4:    
        case _CLIN_WRD_RGZ:            
        {
            //konieczna odpowiedz zwrotna UNI-02 do KSK stanowiaca potwierdzenie wykonania komendy
            switch(krok)
            {
                case 0:         //0.Odczekaj na zakonczenie poprzedniej transmisji do UNI-02
                {
                    switch(FRL1.IDN)
                    {
                        case _CLIN_WRD_P:
                        {                         
                            RSDT1.inLIDN=_LIN_WPP;             //komenda
                            break;
                        }
                        case _CLIN_WRD_I:
                        {
                            RSDT1.inLIDN=_LIN_WPI;             //komenda                            
                            break;
                        }  
                        case _CLIN_WRD_D:
                        {
                            RSDT1.inLIDN=_LIN_WPD;             //komenda                            
                            break;
                        }   
                        case _CLIN_WRD_RPV:
                        {
                            RSDT1.inLIDN=_LIN_WPV;             //komenda                            
                            break;
                        }                              
                        case _CLIN_WRD_RCO1:
                        {                         
                            RSDT1.inLIDN=_LIN_WRCO1;             //komenda
                            break;
                        }
                        case _CLIN_WRD_RCO2:
                        {
                            RSDT1.inLIDN=_LIN_WRCO2;             //komenda                            
                            break;
                        }  
                        case _CLIN_WRD_RCO3:
                        {
                            RSDT1.inLIDN=_LIN_WRCO3;             //komenda                            
                            break;
                        }   
                        case _CLIN_WRD_RCO4:
                        {
                            RSDT1.inLIDN=_LIN_WRCO4;             //komenda                            
                            break;
                        } 
                        case _CLIN_WRD_RCW1:
                        {                         
                            RSDT1.inLIDN=_LIN_WRCW1;             //komenda
                            break;
                        }
                        case _CLIN_WRD_RCW2:
                        {
                            RSDT1.inLIDN=_LIN_WRCW2;             //komenda                            
                            break;
                        }  
                        case _CLIN_WRD_RCW3:
                        {
                            RSDT1.inLIDN=_LIN_WRCW3;             //komenda                            
                            break;
                        }   
                        case _CLIN_WRD_RCW4:
                        {
                            RSDT1.inLIDN=_LIN_WRCW4;             //komenda                            
                            break;
                        }    
                        case _CLIN_WRD_RGZ:
                        {
                            RSDT1.inLIDN=_LIN_WGDZ;             //komenda                            
                            break;
                        }
                        default:
                        {
                            return _LIN1_ERRODP;    
                        }                        
                    }   
                    if(!LINWaitForAnswerUNI02(RSDT1.inLIDN)) return _LIN1_IBUSY;
                    krok++;
                }
                case 1:         //1.Wysylka komendy do UNI-02
                {
                    //komendy i parametry wywolania dla UNI-02                    
                    if(PFN.vUNI02<16)
                    {
                        krok=0;
                        return _LIN1_ERRODP; 
                    }
                    RSDT1.inLDTA1=FRL1.WRD1.byte0;      //parametr1 (wartosc parametru 1)
                    RSDT1.inLDTA2=FRL1.WRD1.byte1;      //parametr2 (wartosc parametru 2)                     
                    //start wywolania
                    if(!FRL1.NAD) FRL1f_ZeroAdr=1;
                    else FRL1f_ZeroAdr=0;
                    RSDT1f_NewLIDN=1;
                    RSDT1.ACKUNI=0;
                    FRL1f_NextFr=0;
                    krok++;
                }
                case 2:         //2.Oczekiwanie na odpowiedz
                {                                         
                    switch(LINWaitForAnswerUNI02(RSDT1.inLIDN))
                    {
                        case 0:
                            return _LIN1_IBUSY;
                        case 1:
                            krok++;
                            break;
                        default:
                            krok=0;
                            return _LIN1_ERRODP;
                    }
                }
                case 3:         //3.Przygotowanie danych dla LIN
                {
                    krok=0;
                    if(FRL1f_ZeroAdr) return _LIN1_NORESP;                              //odpowiedz do LIN nie ma byc generowana?
                    if(!FRL1f_NextFr)                                                   //w trakcie wysylki do UNI-02 nie bylo aktualizacji inna ramka LIN?
                    {
                        switch(FRL1.IDN)
                        {
                            case _CLIN_WRD_P:
                            case _CLIN_WRD_I:
                            case _CLIN_WRD_D:
                            {    
                                if(RSDT1.outLDTA3) return _LIN1_ERRODP;                         //status operacji na UNI-02 wskazuje na blad?
                                break;
                            }
                            case _CLIN_WRD_RPV:
                            case _CLIN_WRD_RCO1:
                            case _CLIN_WRD_RCO2:
                            case _CLIN_WRD_RCO3:
                            case _CLIN_WRD_RCO4:  
                            case _CLIN_WRD_RCW1:
                            case _CLIN_WRD_RCW2:
                            case _CLIN_WRD_RCW3:
                            case _CLIN_WRD_RCW4:                                
                            {    
                                if(RSDT1.outLDTA2) return _LIN1_ERRODP;                         //status operacji na UNI-02 wskazuje na blad?
                                break;
                            }  
                            case _CLIN_WRD_RGZ:                                
                            {    
                                if(RSDT1.outLDTA1) return _LIN1_ERRODP;                         //zwrocona wartosc licznika jest rozna od 0?
                                break;
                            }  
                            default:
                            {
                                return _LIN1_ERRODP;    
                            }                            
                        }
                    }
                    else                                                                //aktualizowano ramke - odpowiedz negatywna
                    {
                        return _LIN1_ERRODP;
                    }
                }
            }
            break;
        }        
        case _CLIN_WRD_RT:     //ustaw pobudzenie RT
        {
            if(FRL1.WRD1.byte0>0)
            {
                PFNf_LIN_RT=1;
            }
            else
            {
                PFNf_LIN_RT=0;                
            }
            break;
        }
        case _CLIN_WRD_CW:     //ustaw pobudzenie z timera zasobnika
        {
            if((FRL1.WRD1.byte0>0)&&PFNf_EnAleg)
            {
                PFNf_LIN_CW=1;
            }
            else
            {
                PFNf_LIN_CW=0;                
            }
            break;
        } 
        case _CLIN_WRD_ENDO:     //ustaw nowy punkt regulacji dla grzania WG
        {
            if(PFNf_prODP)      //aktywna procedura odpowietrzajaca?
            {
                PFNf_LIN_ENDO=1;  
            }
            else
            {
                PFNf_LIN_ENDO=0;
            }
            break;
        }         
        case _CLIN_WRD_SETP:     //ustaw nowy punkt regulacji dla grzania WG
        {
            //if((PFNf_LIN_STR&&PFNf_FLIN)||(PFNf_LIN_KSK&&PFNf_FLIN)||(PFNf_LIN_PWD&&PFNf_FLIN)||(PFNf_LIN_BUF&&PFNf_FLIN))    //LIN w trybie pracy sterowanie?
            if((!PFNf_LIN_BPSS&&PFNf_LIN_STR&&PFNf_FLIN)||(!PFNf_LIN_BPSS&&PFNf_LIN_KSK&&PFNf_FLIN)||(PFNf_LIN_PWD&&PFNf_FLIN)||(PFNf_LIN_BUF&&PFNf_FLIN))    //LIN w trybie pracy sterowanie? //+++
            {
                if(FRL1.WRD1.byte0<PFN.SETPmin)
                {
                    PFN.INSETP=PFN.SETPmin;
                }    
                else
                {
                    if(!PFNf_LIN_PWD)
                    {
                        if(FRL1.WRD1.byte0>PFN.SETPmax)
                        {
                            PFN.INSETP=PFN.SETPmax;
                        }
                        else PFN.INSETP=FRL1.WRD1.byte0;
                    }
                    else   //aktywne przewyzszenie?
                    {
                        if(FRL1.WRD1.byte0>PFN.SETPPmax)
                        {
                            PFN.INSETPP=PFN.SETPPmax;
                        } 
                        else PFN.INSETPP=FRL1.WRD1.byte0;
                    }
                } 
            }
            else return _LIN1_ERRODP;              
            break;
        }   
        case _CLIN_WRD_PRD:      //ustawic specjalny tryb pracy sterownika?
        {
            if(FRL1.WRD1.byte0==0xAA)
            {
                PFN.STRB=FRL1.WRD1.byte1;    
            }
            else return _LIN1_ERRODP;
            break;
        }  
        case _CLIN_WRD_HFLG:   //wyzeruj licznik flagi serwisowej historii zdarzen awaryjnych w plytce sterujacej UNI-02 
        {
            //konieczna odpowiedz zwrotna UNI-02 do KSK stanowiaca potwierdzenie wykonania komendy
            switch(krok)
            {
                case 0:         //0.Odczekaj na zakonczenie poprzedniej transmisji do UNI-02
                    if(!LINWaitForAnswerUNI02(_LIN_WHFLG)) return _LIN1_IBUSY;
                    krok++;
                case 1:         //1.Wysylka komendy do UNI-02
                {
                    //komendy i parametry wywolania dla UNI-02
                    RSDT1.inLIDN=_LIN_WHFLG;             //komenda
                    //wywolanie bez parametrow
                    //start wywolania
                    if(!FRL1.NAD) FRL1f_ZeroAdr=1;
                    else FRL1f_ZeroAdr=0;
                    RSDT1f_NewLIDN=1;
                    RSDT1.ACKUNI=0;
                    FRL1f_NextFr=0;
                    krok++;
                }
                case 2:         //2.Oczekiwanie na odpowiedz
                {
                    switch(LINWaitForAnswerUNI02(_LIN_WHFLG))
                    {
                        case 0:
                            return _LIN1_IBUSY;
                        case 1:
                            krok++;
                            break;
                        default:
                            krok=0;
                            return _LIN1_ERRODP;
                    }
                }
                case 3:         //3.Przygotowanie danych dla LIN
                {
                    krok=0;
                    if(FRL1f_ZeroAdr) return _LIN1_NORESP;                              //odpowiedz do LIN nie ma byc generowana?
                    if(!FRL1f_NextFr)                                                   //w trakcie wysylki do UNI-02 nie bylo aktualizacji inna ramka LIN?
                    {
                        if(RSDT1.outLDTA1) return _LIN1_ERRODP;                         //zwrocona wartosc licznika jest rozna od 0?
                    }
                    else                                                                //aktualizowano ramke - odpowiedz negatywna
                    {
                        return _LIN1_ERRODP;
                    }
                }
            }
            break;
        }        
        default:                   //nieznana komenda
        {
            return _LIN1_UNCCOM;
        }
    }
    return _LIN1_IFINAL;            //koniec przetwarzania danych, automatyczne potwierdzenie
}
//------------------------------------------------------------
//------------------------------------------------------------
//MULTIRESPONSE
//PRZYGOTOWANIE bufora do wypelnienia przez dane nadeslane z MASTER (wskazanie adresu i rozmiaru)
//UWAGA: zwrot wartosci _LIN1_IBUSY sygnalizuje trwanie procesu (maksymalny czas zajetosci zdefiniowany parametrem _LN1_ADTA)
//Start odbioru - wskazanie bufora na dane
unsigned char LIN1StartMultiResponse_WR(void)
{
    BitSet3(&RSDT1.inLNSTAT,_FLIN,1);        //odswiez znacznik komunikacji LIN dla ukladu UNI-02
    PFNf_FLIN=1;    
    StartRTdS(_RTFLIN);
    /*
    switch(FRL1.IDN)               //komenda na ktora nastapila reakcja ukladu SLAVE
    {
        case 0x01:        //identyfikator komendy
        {
            FRL1.imax=32;          //rozmiar tabeli
            FRL1.tab=tbuf;         //adres tabeli

            break;
        }
        default:                   //nieznana komenda
        {
            return _LIN1_UNCCOM;
        }
    }
    if((FRL1.STOP.war<FRL1.START.war)||(FRL1.STOP.war>=FRL1.imax))
    {
        return _LIN1_INVPAR;       //nieprawidlowy parametr
    }
    return _LIN1_IFINAL;            //koniec przetwarzania danych, automatyczne potwierdzenie
     */
    return _LIN1_UNCCOM;            //brak wsparcia dla komendy
}
//Koniec odbioru - aktualizacja danych danymi z bufora w zakresie start-stop
unsigned char LIN1FinishMultiResponse_WR(void)
{
    BitSet3(&RSDT1.inLNSTAT,_FLIN,1);        //odswiez znacznik komunikacji LIN dla ukladu UNI-02
    PFNf_FLIN=1;    
    StartRTdS(_RTFLIN);
    /*
    switch(FRL1.IDN)               //komenda na ktora nastapila reakcja ukladu SLAVE
    {
        case 0x01:                 //identyfikator komendy
        {           
            break;
        }
        default:                   //nieznana komenda
        {
            return _LIN1_UNCCOM;
        }
    }
    return _LIN1_IFINAL;            //koniec przetwarzania danych, automatyczne potwierdzenie     
    */
    return _LIN1_UNCCOM;
}
//------------------------------------------------------------
//------------------------------------------------------------
//******************************** POLECENIE WYSYLKI WSKAZANYCH DANYCH DO MASTER**************************************
//------------------------------------------------------------
//------------------------------------------------------------
//SINGLE RESPONSE
//interpretacja komendy, ew. parametrow i zwrotna wysylka danych
//UWAGA: zwrot wartosci _LIN1_IBUSY sygnalizuje trwanie procesu (maksymalny czas zajetosci zdefiniowany parametrem _LN1_ADTA)
unsigned char LIN1SingleResponse_RD(void)
{
    volatile unsigned char dt;
    static unsigned char krok;
    BitSet3(&RSDT1.inLNSTAT,_FLIN,1);        //odswiez znacznik komunikacji LIN dla ukladu UNI-02
    PFNf_FLIN=1;    
    StartRTdS(_RTFLIN);   
    //----------------    
    if(FRL1f_RSFrame)                 //ramka nadeslana z modulu Wifi?
    {
        if(!RSFInterpreter_SingleRD()) return _LIN1_UNCCOM;       
    }    
    //----------------   
    //komenda na ktora nastapila reakcja ukladu SLAVE
    switch(FRL1.IDN)                        //identyfikator komendy
    {      
        case _CLIN_RRD_ACK:                                     //wygeneruj potwierdzenie w odpowiedzi na wywolanie adresem
        {
            if(!FRL1.NAD&&FRL1.WRD1.byte0&&(FRL1.WRD1.byte0!=PFN.ALIN)) //adres zerowy i pierwszy parametr wywolania (0...4) ró¿ny od zera i rozny od adresu wlasnego?
            {
                return _LIN1_NORESP;        //ignoruj wywolanie
            }         
            
            LIN2SetMasterIDO(FRL1.WRD1.byte1);                    //zapamietaj identyfikator nadajnika (MASTER LIN2)
            
            FTL1.PCI_LENGTH=0x06;                               //Rozmiar ramki(RSID+liczba bajtow danych bez CHK)
            FTL1.WRD0.byte0=0xB2+0x40;                          //RSID=SID+0x40
            FTL1.WRD0.byte1=_LIN_OID;                           //identyfikator odbiornika
            //*******************************
            FTL1.WRD1.byte0=(_WERSJA&0b00011111);               //data0: wersja oprogramowania plytki wyswietlacza
            FTL1.WRD1.byte0|=0b10000000;                        //flaga wsparcia dla skroconej multiramki
            FTL1.WRD1.byte1=(PFN.vUNI02&0b00011111);            //data1: wersja oprogramowania plytki UNI-02 (0 - brak wsparcia dla identyfikacji wersji)
            //*******************************            
            FTL1.WRD2.byte0=(PFNf_EnAleg)?0x01:0x02;            //data2: aktualna konfiguracja sterownika (1F/2F)   
            //*******************************
            FTL1.WRD2.byte0=(FTL1.WRD2.byte0&0b00011111);
            //*******************************            
            dt=0x01;                                            //(1-p³ytka UNI-02)                                          
            dt=dt|(_KEY_BBOARD<<4);                             //(0-4 key, 1-7key)
            FTL1.WRD2.byte1=dt;                                 //cztery m³odsze bity - identyfikator warstwy sprzetowej plyty sterujacej (1-p³ytka UNI-02)      
                                                                //cztery starsze bity - identyfikator warstwy sprzetowej p³ytki LCD (0-4 key, 1-7key)
            break;
        }
        case _CLIN_RRD_PCW:                 //wygeneruj wartosc PCW
        {
            if(!((FRL1.WRD1.byte0==0xAA)&&(FRL1.WRD1.byte1==0x55)))
            {            
                FTL1.PCI_LENGTH=0x04;                               //Rozmiar ramki(RSID+liczba bajtow danych bez CHK)
                FTL1.WRD0.byte0=0xB2+0x40;                          //RSID=SID+0x40
                FTL1.WRD0.byte1=(unsigned char)PFN.PCW;//RSDT1.outPCW;                       //parametr  
                FTL1.WRD0.byte1=FTL1.WRD0.byte1&0b01111111;                                     //usun znacznik odpowiedzi rozszerzonej                
                if(!PFNf_EnAleg)
                {
                    FTL1.WRD1.byte0=(unsigned char)PFN.minPCW;//RSDT1.outPCWmn;                     //dolna granica parametru
                }
                else
                {
                    FTL1.WRD1.byte0=(unsigned char)PFN.minPCW-1;//RSDT1.outPCWmn-1;                   //dolna granica parametru  
                }
                FTL1.WRD1.byte1=(unsigned char)PFN.maxPCW;//RSDT1.outPCWmx;                         //gorna granica parametru
            }
            else
            {
                FTL1.PCI_LENGTH=0x05;                               //Rozmiar ramki(RSID+liczba bajtow danych bez CHK)
                FTL1.WRD0.byte0=0xB2+0x40;                          //RSID=SID+0x40
                //-----                
                FTL1.WRD0.byte1=(unsigned char)PFN.PCW;//RSDT1.outPCW;                       //parametr 
                FTL1.WRD0.byte1=FTL1.WRD0.byte1|0b10000000;                                     //ustaw znacznik odpowiedzi rozszerzonej
                //-----              
                if(!PFNf_EnAleg)
                {
                    FTL1.WRD1.byte0=(unsigned char)PFN.minPCW;//RSDT1.outPCWmn;                     //dolna granica parametru
                }
                else
                {
                    if(PFN.minPCW)
                    {
                        FTL1.WRD1.byte0=(unsigned char)PFN.minPCW-1;//RSDT1.outPCWmn-1;                   //dolna granica parametru
                    }
                    else
                    {
                        FTL1.WRD1.byte0=0;
                    }
                }
                if(PFNf_EnAleg)
                {
                    FTL1.WRD1.byte0=FTL1.WRD1.byte0|0b10000000;                     //rodzaj danej (0-nastawa PCO, 1-wsp. KT)
                }
                else
                {
                    FTL1.WRD1.byte0=FTL1.WRD1.byte0&0b01111111;
                } 
                //-----                
                FTL1.WRD1.byte1=(unsigned char)PFN.maxPCW;//RSDT1.outPCWmx;                         //gorna granica parametru          
                if(PFNf_plmCW)
                {
                    FTL1.WRD1.byte1=FTL1.WRD1.byte1|0b10000000;                     //plomien CW
                }
                else
                {
                    FTL1.WRD1.byte1=FTL1.WRD1.byte1&0b01111111;
                }                
                //-----
                FTL1.WRD2.byte0=(unsigned char)PFN.CW;                                          //temperatura w obiegu CW
                if(PFNf_errCW) 
                {
                    FTL1.WRD2.byte0=0b10000000;                                     //awaria/brak czujnika CW
                }
                else
                {
                    FTL1.WRD2.byte0=FTL1.WRD2.byte0&0b01111111;    
                }
                //-----            
            }
            break;
        } 
        case _CLIN_RRD_PCO:                 //wygeneruj wartosc PCO
        {
            if(!((FRL1.WRD1.byte0==0xAA)&&(FRL1.WRD1.byte1==0x55)))
            {
                FTL1.PCI_LENGTH=0x05;                               //Rozmiar ramki(RSID+liczba bajtow danych bez CHK)
                FTL1.WRD0.byte0=0xB2+0x40;                          //RSID=SID+0x40
                FTL1.WRD0.byte1=(unsigned char)PFN.PCO;//RSDT1.outPCO;                          //parametr 
                FTL1.WRD0.byte1=FTL1.WRD0.byte1&0b01111111;                                     //usun znacznik odpowiedzi rozszerzonej
                FTL1.WRD1.byte0=(unsigned char)PFN.minPCO;//RSDT1.outPCOmn;                     //dolna granica parametru
                FTL1.WRD1.byte1=(unsigned char)PFN.maxPCO;//RSDT1.outPCOmx;                     //gorna granica parametru
                FTL1.WRD2.byte0=(PFNf_pgd)?1:0;                     //rodzaj danej (0-nastawa PCO, 1-wsp. KT)  
            }
            else
            {
                FTL1.PCI_LENGTH=0x06;                               //Rozmiar ramki(RSID+liczba bajtow danych bez CHK)
                FTL1.WRD0.byte0=0xB2+0x40;                          //RSID=SID+0x40
                //-----
                FTL1.WRD0.byte1=(unsigned char)PFN.PCO;//RSDT1.outPCO;                          //parametr  
                FTL1.WRD0.byte1=FTL1.WRD0.byte1|0b10000000;                                     //ustaw znacznik odpowiedzi rozszerzonej                
                //-----
                FTL1.WRD1.byte0=(unsigned char)PFN.minPCO;//RSDT1.outPCOmn;                     //dolna granica parametru
                if(PFNf_pgd)
                {
                    FTL1.WRD1.byte0=FTL1.WRD1.byte0|0b10000000;                     //rodzaj danej (0-nastawa PCO, 1-wsp. KT)
                }
                else
                {
                    FTL1.WRD1.byte0=FTL1.WRD1.byte0&0b01111111;
                } 
                if(FTL1.WRD1.byte0&0b10000000)
                {
                    Nop();
                    Nop();
                }
                //-----
                FTL1.WRD1.byte1=(unsigned char)PFN.maxPCO;//RSDT1.outPCOmx;                     //gorna granica parametru
                if(PFNf_plmCO)
                {
                    FTL1.WRD1.byte1=FTL1.WRD1.byte1|0b10000000;                     //plomien CO
                }
                else
                {
                    FTL1.WRD1.byte1=FTL1.WRD1.byte1&0b01111111;
                }                
                //-----
                FTL1.WRD2.byte0=(unsigned char)PFN.CO;                                          //temperatura w obiegu CO
                if(PFNf_errCO) 
                {
                    FTL1.WRD2.byte0=0b10000000;                                     //awaria/brak czujnika CO
                }
                else
                {
                    FTL1.WRD2.byte0=FTL1.WRD2.byte0&0b01111111;    
                }
                //-----
                FTL1.WRD2.byte1=(unsigned char)PFN.CS;                                          //cisnienie w obiegu CO
                if(PFNf_errCS) 
                {
                    FTL1.WRD2.byte1=0b10000000;                                     //awaria/brak czujnika CS
                }
                else
                {
                    FTL1.WRD2.byte1=FTL1.WRD2.byte1&0b01111111;    
                }             
            }
            //---------------------------------
            //zapamietaj przeslane pobudzenia stref i pionow
            if(!FRL1f_RSFrame)                 //ramka niskiego poziomu?
            {
                StartRTS(_RTPLIN2); 
                if(CMSGDta.REF2<255) CMSGDta.REF2++;
                else CMSGDta.REF2=1;  
                if(CMSGDta.DTA4!=FRL1.WRD2.byte0)
                {
                    KFN._newSPOB=1;                 //ustaw flage nowej wartosci
                }       
                if(CMSGDta.DTA5!=FRL1.WRD2.byte1)
                {
                    KFN._newPPOB=1;                 //ustaw flage nowej wartosci
                }            
                CMSGDta.DTA4=FRL1.WRD2.byte0;       //pobudzenia stref
                CMSGDta.DTA5=FRL1.WRD2.byte1;       //pobudzenia pionow zasilania            
                KFN.flag6=CMSGDta.DTA4;  
                KFN.flag7=CMSGDta.DTA5;                
            }             
            break;
        } 
        case _CLIN_RRD_ECO:                 //wygeneruj wartosc ECO
        {
            FTL1.PCI_LENGTH=0x04;                               //Rozmiar ramki(RSID+liczba bajtow danych bez CHK)
            FTL1.WRD0.byte0=0xB2+0x40;                          //RSID=SID+0x40
            FTL1.WRD0.byte1=(unsigned char)PFN.ECO;//RSDT1.outECO;                            //parametr  //********************
            FTL1.WRD1.byte0=1;                                  //dolna granica parametru            
            FTL1.WRD1.byte1=9;                                  //gorna granica parametru                       
            break;
        }    
        case _CLIN_RRD_TRB:                 
        {
            FTL1.PCI_LENGTH=0x06;                               //Rozmiar ramki(RSID+liczba bajtow danych bez CHK)
            FTL1.WRD0.byte0=0xB2+0x40;                          //RSID=SID+0x40
            if(PFNf_off)
            {
                FTL1.WRD0.byte1=2;                              //tryb OFF
            }
            else
            if(PFNf_lato)
            {
                FTL1.WRD0.byte1=1;                              //tryb LATO
            }
            else
            {
                FTL1.WRD0.byte1=0;                              //tryb ZIMA
            }            
            FTL1.WRD1.byte0=0;                                  //dolna granica parametru
            FTL1.WRD1.byte1=2;                                  //gorna granica parametru
            FTL1.WRD2.byte0=0;  
            FTL1.WRD2.byte1=PFN.EXFLG;
            //---------
             if(!FRL1f_RSFrame)                 //ramka niskiego poziomu?
            {            
                StartRTS(_RTPLIN); 
                if(CMSGDta.REF1<255) CMSGDta.REF1++;
                else CMSGDta.REF1=1;     
                
                if(CMSGDta.DTA1!=FRL1.WRD1.byte1)
                {
                    KFN._newSTAT=1;                 //ustaw flage nowej wartosci
                }
                if(CMSGDta.DTA2!=FRL1.WRD2.byte0)
                {
                    KFN._newENPNZ=1;                 //ustaw flage nowej wartosci
                }                
                if(CMSGDta.DTA3!=FRL1.WRD2.byte1)
                {
                    KFN._newGPOB=1;                 //ustaw flage nowej wartosci
                }       
                
                CMSGDta.DTA1=FRL1.WRD1.byte1;               //status modulu
                if(CMSGDta._TRB_CMF) KFN._enCMF=1;          //praca modulu w trybie komfortu
                else KFN._enCMF=0; 
                KFN._TRB_WZ=CMSGDta._TRB_WZ;
                if(CMSGDta._TRB_WZ!=0) KFN._enWZ=1;         //praca modulu z aktywnym wezlem zasilania (dla CMSGDta._TRB_WZ=0 pion wylaczony, dla CMSGDta._TRB_WZ=1 pion w trybie pracy 1,dla CMSGDta._TRB_WZ=2 pion w trybie pracy 2)
                else KFN._enWZ=0;            
                if(CMSGDta._ExBLK) KFN._ExBLK=1;            //aktywnosc zewnetrznej blokady grzania dla kotla gazowego
                else KFN._ExBLK=0; 
                if(CMSGDta._pobExCO) KFN._pobExCO=1;        //niezalezne od blokady pobudzenie grzania
                else KFN._pobExCO=0; 
                if(CMSGDta._KSK_BLK_RC) KFN._blRC=1;        //aktywnosc blokady pompy RC
                else KFN._blRC=0;
                
                if(CMSGDta._pobUP) KFN._pobUP=1;            //pobudzenie na wysokich parametrach
                else KFN._pobUP=0;
                if(KFN._pobUP) PFNf_LIN_UP=1;
                else PFNf_LIN_UP=0;
                
                CMSGDta.DTA2=FRL1.WRD2.byte0;               //aktywnosci pionow zasilania
                if(KFN._enWZ)
                {
                    KFN.flag10=CMSGDta.DTA2&0b00111111;
                }
                else
                {
                    KFN.flag10=0;
                }
                CMSGDta.DTA3=FRL1.WRD2.byte1;               //pobudzenia posesji/grup 
                if(CMSGDta._setBPSS) KFN._setBPSS=1;            //bypass dla pobudzenia RT  //+++
                else KFN._setBPSS=0;
                if(KFN._setBPSS) PFNf_LIN_BPSS=1;  //+++ 
                else PFNf_LIN_BPSS=0;
                
                KFN.flag4=CMSGDta.DTA2;  
                KFN.flag5=CMSGDta.DTA3;                
            }          
            break;
        } 
        case _CLIN_RRD_RES:                 //wygeneruj stan przycisku res
        {
            FTL1.PCI_LENGTH=0x02;                               //Rozmiar ramki(RSID+liczba bajtow danych bez CHK)
            FTL1.WRD0.byte0=0xB2+0x40;                          //RSID=SID+0x40
            if(PFNf_res)                                        //nacisniety przycisk RESET?
            {
                FTL1.WRD0.byte1=1;                              //parametr
            }
            else
            {
                FTL1.WRD0.byte1=0;                              //parametr                
            }
            break;     
        }        
        case _CLIN_RRD_BRES:                                    //wygeneruj informacje o blokadzie awaryjnej
        {
            FTL1.PCI_LENGTH=0x05;                               //Rozmiar ramki(RSID+liczba bajtow danych bez CHK)
            FTL1.WRD0.byte0=0xB2+0x40;                          //RSID=SID+0x40
            if((!PFNf_opgres)&&PFNf_fmsg)
            {
                FTL1.WRD0.byte1=1;                              //wylaczenie bez blokady awaryjnej (sygnalizacja, realizacja procedury awaryjnej)
                FTL1.WRD1.byte0=PFN.KAWR;                
            }
            else
            if((PFNf_opgres)&&PFNf_ferr) 
            {
                FTL1.WRD0.byte1=2;                              //wylaczenie z blokada awaryjna
                FTL1.WRD1.byte0=PFN.KERR;                                            
            }
            //*****************
            else
            //*****************            
            if((!PFNf_opgres)&&PFNf_ferr)
            {
                FTL1.WRD0.byte1=3;                              //wylaczenie bez blokady awaryjnej (sygnalizacja, autowznowienie pracy po usunieciu przyczyny awarii)
                FTL1.WRD1.byte0=PFN.KERR;                
            }
            else
            {
                FTL1.WRD0.byte1=0;                              //brak stanu awaryjnego
                FTL1.WRD1.byte0=0;                                                                           
            }
            FTL1.WRD1.byte1=PFN.STFLG;
            FTL1.WRD2.byte0=0;    
            FTL1.WRD2.byte1=0;            
            break;     
        }
        case _CLIN_RRD_FSR:                //wygeneruj informacje o funkcji serwisowej 
        {
            FTL1.PCI_LENGTH=0x02;                               //Rozmiar ramki(RSID+liczba bajtow danych bez CHK)
            FTL1.WRD0.byte0=0xB2+0x40;                          //RSID=SID+0x40
            if(PFNf_ofnserw)                                    //aktywna funkcja serwisowa?
            {
                if(PFNf_fns_max) FTL1.WRD0.byte1=1;             //praca na mocy maksymalnej
                else FTL1.WRD0.byte1=2;                         //praca na mocy minimalnej
            }
            else
            {
                if(!PFNf_ferr&&!PFNf_fmsg&&!PFNf_fsetLaddr&&!PFNf_aktywne_KNF&&!PFNf_opgres&&PFNf_zima&&!PFNf_obgCW&&!PFNf_off&&!PFNf_aleg)
                {               
                    FTL1.WRD0.byte1=0;                              //funkcja nieaktywna   
                }
                else 
                {
                    FTL1.WRD0.byte1=3;                              //nie mozna aktywowac funkcji                    
                }
            }
            break;     
        }  
        case _CLIN_RRD_ANL:                 //wygeneruj informacje o stanie funkcji antylegionella
        {
            FTL1.PCI_LENGTH=0x02;                               //Rozmiar ramki(RSID+liczba bajtow danych bez CHK)
            FTL1.WRD0.byte0=0xB2+0x40;                          //RSID=SID+0x40
            if(PFNf_oaleg)                                      //aktywna funkcja antylegionella?
            {
                FTL1.WRD0.byte1=1;                              //funkcja aktywna
            }
            else
            {
                if(!PFNf_ferr&&!PFNf_fmsg&&!PFNf_fsetLaddr&&!PFNf_aktywne_KNF&&!PFNf_opgres&&PFNf_EnAleg&&!PFNf_off&&!PFNf_fnserw)
                {               
                    FTL1.WRD0.byte1=0;                          //funkcja nieaktywna   
                }
                else 
                {
                    FTL1.WRD0.byte1=3;                          //nie mozna aktywowac funkcji                    
                }               
            }
            break;     
        }   
        case _CLIN_RRD_NKNF:                //wygeneruj pinformacje o liczbie parametrow konfiguracyjnych
        {
            FTL1.PCI_LENGTH=0x02;                               //Rozmiar ramki(RSID+liczba bajtow danych bez CHK)
            FTL1.WRD0.byte0=0xB2+0x40;                          //RSID=SID+0x40
            //++++++++++++++++++
            FTL1.WRD0.byte1=GetKNPAR();                         //liczba parametrow konfiguracyjnych         
            //++++++++++++++++++
            break;     
        } 
        case _CLIN_RRD_PKNF:              //zmien parametr konfiguracyjny plytki sterujacej UNI-02 
        {
            //konieczna odpowiedz zwrotna UNI-02 do KSK stanowiaca potwierdzenie wykonania komendy
            switch(krok)
            {
                case 0:         //0.Odczekaj na zakonczenie poprzedniej transmisji do UNI-02
                    if(!LINWaitForAnswerUNI02(_LIN_RPKNF)) return _LIN1_IBUSY;
                    krok++;
                case 1:         //1.Wysylka komendy do UNI-02
                {
                    //komendy i parametry wywolania dla UNI-02
                    RSDT1.inLIDN=_LIN_RPKNF;             //komenda
                    //RSDT1.inLDTA1=FRL1.WRD1.byte0;      //parametr1 (indeks parametru)    
                    if(FRL1.WRD1.byte0<GetKNPAR())
                    {
                        RSDT1.inLDTA1=FRL1.WRD1.byte0;      //parametr1 (indeks parametru)
                    }
                    else
                    {
                        krok=0;
                        return _LIN1_ERRODP; 
                    }                    
                    //start wywolania
                    if(!FRL1.NAD) FRL1f_ZeroAdr=1;
                    else FRL1f_ZeroAdr=0;
                    RSDT1f_NewLIDN=1;
                    RSDT1.ACKUNI=0;
                    FRL1f_NextFr=0;
                    krok++;
                }
                case 2:         //2.Oczekiwanie na odpowiedz
                {
                    switch(LINWaitForAnswerUNI02(_LIN_RPKNF))
                    {
                        case 0:
                            return _LIN1_IBUSY;
                        case 1:
                            krok++;
                            break;
                        default:
                            krok=0;
                            return _LIN1_ERRODP;
                    }
                }
                case 3:         //3.Przygotowanie danych dla LIN
                {
                    krok=0;
                    if(FRL1f_ZeroAdr) return _LIN1_NORESP;                               //odpowiedz do LIN nie ma byc generowana?
                    if(!FRL1f_NextFr)                                                    //w trakcie wysylki do UNI-02 nie bylo aktualizacji inna ramka LIN?
                    {
                        if(RSDT1.outLDTA2) return _LIN1_ERRODP;                         //status operacji na UNI-02 wskazuje na blad?
                        FTL1.PCI_LENGTH=0x05;                                           //Rozmiar ramki(RSID+liczba bajtow danych bez CHK)
                        FTL1.WRD0.byte0=0xB2+0x40;                                      //RSID=SID+0x40
                        if(RSDT1.inLDTA1<GetKNPAR())
                        {
                            FTL1.WRD0.byte1=RSDT1.outLDTA1;  
                            FTL1.WRD1.byte0=(unsigned char) CPZK[RSDT1.inLDTA1].min;
                            FTL1.WRD1.byte1=(unsigned char) CPZK[RSDT1.inLDTA1].max; 
                            if(PFN.vUNI02>=16)
                            {
                                if(RSDT1.inLDTA1==22)               //rodzaj wentylatora
                                {
                                    FTL1.WRD1.byte1=2;
                                }
                            }                            
                            FTL1.WRD2.byte0=0;
                            if(CPZK[RSDT1.inLDTA1].flwar&0b00001000) FTL1.WRD2.byte0=1;
                            if(CPZK[RSDT1.inLDTA1].flwar&0b00010000) FTL1.WRD2.byte0=2;                            
                        }
                        else return _LIN1_ERRODP;                             
                    }
                    else                                                                //aktualizowano ramke - odpowiedz negatywna
                    {
                        Nop();
                        return _LIN1_ERRODP;
                    }
                }
            }
            break;
        }
        case _CLIN_RRD_RT:                                      //wygeneruj pinformacje o pobudzeniu RT
        {
            FTL1.PCI_LENGTH=0x02;                               //Rozmiar ramki(RSID+liczba bajtow danych bez CHK)
            FTL1.WRD0.byte0=0xB2+0x40;                          //RSID=SID+0x40
            FTL1.WRD0.byte1=(PFNf_pobRT)?1:0;                   //liczba parametrow konfiguracyjnych           
            break;     
        }         
        case _CLIN_RRD_CW:                                      //wygeneruj pinformacje o pobudzeniu CW/stanie timera zasobnika
        {
            FTL1.PCI_LENGTH=0x02;                               //Rozmiar ramki(RSID+liczba bajtow danych bez CHK)
            FTL1.WRD0.byte0=0xB2+0x40;                          //RSID=SID+0x40
            FTL1.WRD0.byte1=(PFNf_timZAS)?1:0;                  //pobudzenie CW           
            break;     
        }  
        case _CLIN_RRD_10V:                                     //wygeneruj informacje o interfejsie 10V
        {
            FTL1.PCI_LENGTH=0x04;                               //Rozmiar ramki(RSID+liczba bajtow danych bez CHK)
            FTL1.WRD0.byte0=0xB2+0x40;                          //RSID=SID+0x40
            FTL1.WRD0.byte1=(PFNf_EnF10V)?1:0;                  //aktywnosc interfejsu 10V
            FTL1.WRD1.byte0=(PFNf_10V_RT)?1:0;                  //Pobudzenie RT od interfejsu 10V
            FTL1.WRD1.byte1=PFN.PCO10V;                         //nowa wartosc SETPOINT z interfejsu 10V
            break;
        } 
        case _CLIN_RRD_SETP:                                    //wygeneruj pinformacje o wartosci SETP
        {
            if(!((FRL1.WRD1.byte0==0xAA)&&(FRL1.WRD1.byte1==0x55)))
            {            
                FTL1.PCI_LENGTH=0x04;                               //Rozmiar ramki(RSID+liczba bajtow danych bez CHK)
                FTL1.WRD0.byte0=0xB2+0x40;                          //RSID=SID+0x40
                FTL1.WRD0.byte1=PFN.CONTSETP;                       //docelowa wartosc modulacji dla grzania obiegu WG 
                FTL1.WRD0.byte1=FTL1.WRD0.byte1&0b01111111;         //usun znacznik odpowiedzi rozszerzonej                
                FTL1.WRD1.byte0=PFN.SETPmin;                        //dolna granica dla SETP
                FTL1.WRD1.byte0&=0b01111111;                
                FTL1.WRD1.byte1=PFN.SETPmax;                        //gorna granica dla SETP (wartosc graniczna grzania WG)
                FTL1.WRD1.byte1&=0b01111111;
            }
            else
            {
                FTL1.PCI_LENGTH=0x06;                               //Rozmiar ramki(RSID+liczba bajtow danych bez CHK)
                FTL1.WRD0.byte0=0xB2+0x40;                          //RSID=SID+0x40
                //-----                 
                FTL1.WRD0.byte1=PFN.CONTSETP&0b01111111;            //docelowa wartosc modulacji dla grzania obiegu WG  
                FTL1.WRD0.byte1=FTL1.WRD0.byte1|0b10000000;         //ustaw znacznik odpowiedzi rozszerzonej                
                //-----                
                FTL1.WRD1.byte0=PFN.SETPmin;                        //dolna granica dla SETP
                if(PFNf_fzaREG)                                     //faza stabilizacji regulatora
                {
                    FTL1.WRD1.byte0|=0b10000000;
                }   
                else
                {
                    FTL1.WRD1.byte0&=0b01111111;
                }       
                //-----                
                FTL1.WRD1.byte1=PFN.SETPmax;                        //gorna granica dla SETP (wartosc graniczna grzania WG) 
                if((PFNf_stkRT&&!PFNf_pgdTP)||(PFNf_pobPGRT&&PFNf_pgdTP)||PFNf_OTRT||PFNf_10VRT)             //dowolne pobudzenie zewnetrzne
                {
                    FTL1.WRD1.byte1|=0b10000000;
                }   
                else
                {
                    FTL1.WRD1.byte1&=0b01111111;
                }                
                //----               
                if(PFNf_fenPWD)                                     //plyta sterujaca wspiera przewyzszenie temperaturowe?
                {
                    FTL1.WRD2.byte0=0b10000000;                                   //flaga wsparcia dla przewyzszenia
                    FTL1.WRD2.byte0|=(RSDT1.outSETPPmax&0b01111111);              //dozwolona warto¶æ maksymalna SETP z uwzglednieniem przewyzszenia
                    if(PFNf_pmpPWM)
                    {
                        FTL1.WRD2.byte1|=0b10000000;                //flaga trybu pracy pompy (0-tradycyjna, 1-pompa PWM)
                    }
                    else
                    {
                        FTL1.WRD2.byte1&=0b01111111;                //flaga trybu pracy pompy (0-tradycyjna, 1-pompa PWM)                    
                    }
                    if(PFN.INSETP<=RSDT1.outSETPmin)    
                    {
                        FTL1.WRD2.byte1|=(RSDT1.outSETPmin&0b01111111);
                    }
                    else
                    {
                        if(PFN.INSETP<=RSDT1.outSETPPmax)
                        {
                            FTL1.WRD2.byte1|=(PFN.INSETP&0b01111111);              //nastawa  SETP (istotna dla samodzielnej pracy modulu rozszerzen)
                        }
                        else
                        {
                            FTL1.WRD2.byte1|=(RSDT1.outSETPPmax&0b01111111);
                        }
                    }
                }
                else
                {
                    FTL1.WRD2.byte0=0; 
                    FTL1.WRD2.byte1=0;
                    if(PFNf_pmpPWM)
                    {
                        FTL1.WRD2.byte1|=0b10000000;                //flaga trybu pracy pompy (0-tradycyjna, 1-pompa PWM)
                    }
                    else
                    {
                        FTL1.WRD2.byte1&=0b01111111;                //flaga trybu pracy pompy (0-tradycyjna, 1-pompa PWM)                    
                    }                    
                }                
            }
            break;     
        }
        case _CLIN_RRD_ZW:                                      //temp. z czujnika zewnetrznego, aktywnosc regulatora pogodowego, wsp. KT
        {
            FTL1.PCI_LENGTH=0x05;                               //Rozmiar ramki(RSID+liczba bajtow danych bez CHK)
            FTL1.WRD0.byte0=0xB2+0x40;                          //RSID=SID+0x40
            FTL1.WRD0.byte1=(PFNf_tmpZW)?1:0;                   //aktywnosc czujnika (0-brak)
            FTL1.WRD1.byte0=(PFNf_tmpZW)?PFN.ZW:0;              //temp zewnetrzna
            FTL1.WRD1.byte1=(PFNf_pgd)?1:0;                     //aktywnosc regulatora pogodowego (0-nieaktywny)   
            FTL1.WRD2.byte0=(PFNf_pgd)?(unsigned char)PFN.PCO:0;               //wartosc KT
            break;
        }        
        case _CLIN_RRD_INT:                                     //wygeneruj informacje zbiorcza o aktywnosci interfejsach 10V,OT,LIN
        {
            FTL1.PCI_LENGTH=0x04;                               //Rozmiar ramki(RSID+liczba bajtow danych bez CHK)
            FTL1.WRD0.byte0=0xB2+0x40;                          //RSID=SID+0x40 
            //------------------------------            
            //interfejs komunikacyjny - status i docelowa wartosc modulacji
            dt=0;
            //LIN
            if(PFNf_FLIN)   dt=dt|0b10000000;                                                       //aktywnosc LIN
            if((PFNf_LIN_RT&&PFNf_FLIN)||(PFNf_LIN_PBCO&&PFNf_FLIN))                                //RT LIN
            {
                dt=dt|0b01000000;
            }
            if((PFNf_EnAleg&&PFNf_LIN_CW&&PFNf_FLIN)||(PFNf_EnAleg&&PFNf_LIN_PBCW&&PFNf_FLIN))      //CW LIN
            {
                dt=dt|0b00100000;
            }            
            //OT
            if(PFNf_FOT) dt=dt|0b00010000;                                                          //aktywnosc OT
            if(OTMaster.Status._CH_enable&&PFNf_FOT) dt=dt|0b00001000;                              //RT OT   
            if(PFNf_EnAleg&&(OTMaster.Status._DHW_enable&&PFNf_FOT)) dt=dt|0b00000100;              //CW OT            
            //10V
            if(PFNf_EnF10V) dt=dt|0b00000010;                                                       //aktywnosc 10V
            if(PFNf_10V_RT&&PFNf_EnF10V) dt=dt|0b00000001;                                          //RT 10V     
           
            FTL1.WRD0.byte1=dt;                                                                     //status interfejsow 
            //------------------------------
            dt=0;
            if(PFNf_LIN_KSK&&PFNf_FLIN) dt=dt|0b00000100;                                           //tryb kaskada interfejsu LIN
            if(PFNf_LIN_STR&&PFNf_FLIN) dt=dt|0b00000010;                                           //tryb sterowania z zadanym SETP z interfejsu LIN
            if(PFNf_LIN_DPGD&&PFNf_FLIN) dt=dt|0b00000001;                                          //dezaktywacja wbudowanego regulatora pogodowego za po¶rednictwem interfejsu LIN
            //+++
            if(PFNf_LIN_PWD&&PFNf_FLIN) dt=dt|0b00100000;                                           //praca z przewyzszeniem z interfejsu LIN
            if(PFNf_LIN_BUF&&PFNf_FLIN) dt=dt|0b01000000;                                           //praca na bufor ciepla z interfejsu LIN
            if(PFNf_LIN_UP&&PFNf_FLIN) dt=dt|0b10000000;                                            //praca na parametrach podwyzszonych interfejsu LIN
                      
            if(OTMaster.Ref._Control_Setpoint&&PFNf_FOT) dt=dt|0b00010000;                          //tryb sterowania z zadanym SETP z interfejsu OT
            if(PFNf_OT_DPGD&&PFNf_FOT) dt=dt|0b00001000;                                            //dezaktywacja wbudowanego regulatora pogodowego za po¶rednictwem interfejsu OT
            
            FTL1.WRD1.byte0=dt;
            //------------------------------
            //docelowa wartosc modulacji
            //if((PFNf_LIN_KSK&&PFNf_FLIN)||(PFNf_LIN_STR&&PFNf_FLIN)||(PFNf_LIN_PWD&&PFNf_FLIN)||(PFNf_LIN_BUF&&PFNf_FLIN))
            if((!PFNf_LIN_BPSS&&PFNf_LIN_STR&&PFNf_FLIN)||(!PFNf_LIN_BPSS&&PFNf_LIN_KSK&&PFNf_FLIN)||(PFNf_LIN_PWD&&PFNf_FLIN)||(PFNf_LIN_BUF&&PFNf_FLIN))    //+++        
            {
                if(!PFNf_LIN_PWD)
                {
                    FTL1.WRD1.byte1=PFN.INSETP; 
                }
                else
                {
                    FTL1.WRD1.byte1=PFN.INSETPP;                    
                }
            }
            else
            if(OTMaster.Ref._Control_Setpoint&&PFNf_FOT)
            {
                FTL1.WRD1.byte1=(unsigned char)OTMaster.Control_Setpoint;
            }
            else
            if(PFNf_EnF10V)
            {
                FTL1.WRD1.byte1=PFN.PCO10V;
            }
            else
            {
                FTL1.WRD1.byte1=0;
            }
            break;
        }
        case _CLIN_RRD_PRD:                                     //specjalny tryb pracy sterownika
        {
            FTL1.PCI_LENGTH=0x02;                               //Rozmiar ramki(RSID+liczba bajtow danych bez CHK)
            FTL1.WRD0.byte0=0xB2+0x40;                          //RSID=SID+0x40
            FTL1.WRD0.byte1=PFN.STRB;                           //specjalny tryb pracy sterownika           
            break;     
        }   
        case _CLIN_RRD_STH:              //odczytaj statystyki zdarzen awaryjnych 
        {
            //konieczna odpowiedz zwrotna UNI-02 do KSK stanowiaca potwierdzenie wykonania komendy
            switch(krok)
            {
                case 0:         //0.Odczekaj na zakonczenie poprzedniej transmisji do UNI-02
                    if(!LINWaitForAnswerUNI02(_LIN_GSTAT)) return _LIN1_IBUSY;
                    krok++;
                case 1:         //1.Wysylka komendy do UNI-02
                {
                    //komendy i parametry wywolania dla UNI-02
                    RSDT1.inLIDN=_LIN_GSTAT;             //komenda    
                    
                    //start wywolania
                    if(!FRL1.NAD) FRL1f_ZeroAdr=1;
                    else FRL1f_ZeroAdr=0;
                    RSDT1f_NewLIDN=1;
                    RSDT1.ACKUNI=0;
                    FRL1f_NextFr=0;
                    krok++;
                }
                case 2:         //2.Oczekiwanie na odpowiedz
                {
                    switch(LINWaitForAnswerUNI02(_LIN_GSTAT))
                    {
                        case 0:
                            return _LIN1_IBUSY;
                        case 1:
                            krok++;
                            break;
                        default:
                            krok=0;
                            return _LIN1_ERRODP;
                    }
                }
                case 3:         //3.Przygotowanie danych dla KSK
                {
                    krok=0;
                    if(FRL1f_ZeroAdr) return _LIN1_NORESP;                               //odpowiedz do LIN nie ma byc generowana?
                    if(!FRL1f_NextFr)                                                    //w trakcie wysylki do UNI-02 nie bylo aktualizacji ramki LIN?
                    {
                        if(PFN.vUNI02>11)
                        {
                            FTL1.PCI_LENGTH=0x06;                                       //Rozmiar ramki(RSID+liczba bajtow danych bez CHK)
                            FTL1.WRD2.byte1=RSDT1.outLDTA5;            
                            FTL1.WRD2.byte1|=0b10000000;                                //ustaw znacznik obslugi flagi serwisowej (najstarszy bit)
                        }
                        else
                        {
                            FTL1.PCI_LENGTH=0x05;                                       //Rozmiar ramki(RSID+liczba bajtow danych bez CHK)                            
                        }
                        FTL1.WRD0.byte0=0xB2+0x40;                                      //RSID=SID+0x40
                        FTL1.WRD0.byte1=RSDT1.outLDTA1;  
                        FTL1.WRD1.byte0=RSDT1.outLDTA2;
                        FTL1.WRD1.byte1=RSDT1.outLDTA3; 
                        FTL1.WRD2.byte0=RSDT1.outLDTA4;                        
                    }
                    else                                                                //aktualizowano ramke - odpowiedz negatywna
                    {
                        return _LIN1_ERRODP;
                    }
                }
            }
            break;
        }   
        case _CLIN_RRD_P:              //odczytaj statystyki zdarzen awaryjnych 
        case _CLIN_RRD_I:              //odczytaj statystyki zdarzen awaryjnych
        case _CLIN_RRD_D:              //odczytaj statystyki zdarzen awaryjnych  
        case _CLIN_RRD_RPV:
        case _CLIN_RRD_RCO:
        case _CLIN_RRD_RCW:    
        case _CLIN_RRD_RGZ:            
        {
            //konieczna odpowiedz zwrotna UNI-02 do KSK stanowiaca potwierdzenie wykonania komendy
            switch(krok)
            {
                case 0:         //0.Odczekaj na zakonczenie poprzedniej transmisji do UNI-02
                {
                    switch(FRL1.IDN)
                    {
                        case _CLIN_RRD_P:
                        {
                            RSDT1.inLIDN=_LIN_RPP;             //komenda
                            break;
                        }
                        case _CLIN_RRD_I:
                        {
                            RSDT1.inLIDN=_LIN_RPI;             //komenda                            
                            break;
                        }  
                        case _CLIN_RRD_D:
                        {
                            RSDT1.inLIDN=_LIN_RPD;             //komenda                            
                            break;
                        }  
                        case _CLIN_RRD_RPV:
                        {
                            RSDT1.inLIDN=_LIN_RPV;             //komenda                            
                            break;
                        }  
                        case _CLIN_RRD_RCO:
                        {
                            RSDT1.inLIDN=_LIN_RRCO;             //komenda                            
                            break;
                        }
                        case _CLIN_RRD_RCW:
                        {
                            RSDT1.inLIDN=_LIN_RRCW;             //komenda                            
                            break;
                        }   
                        case _CLIN_RRD_RGZ:
                        {
                            RSDT1.inLIDN=_LIN_RGDZ;             //komenda                            
                            break;
                        } 
                        default:
                        {
                            return _LIN1_ERRODP;    
                        }                        
                    } 
                    if(!LINWaitForAnswerUNI02(RSDT1.inLIDN)) return _LIN1_IBUSY;                    
                    krok++;
                }
                case 1:         //1.Wysylka komendy do UNI-02
                {
                    //komendy i parametry wywolania dla UNI-02
                    if(PFN.vUNI02<16)
                    {
                        krok=0;
                        return _LIN1_ERRODP; 
                    }                    
                    //start wywolania
                    if(!FRL1.NAD) FRL1f_ZeroAdr=1;
                    else FRL1f_ZeroAdr=0;
                    RSDT1f_NewLIDN=1;
                    RSDT1.ACKUNI=0;
                    FRL1f_NextFr=0;
                    krok++;
                }
                case 2:         //2.Oczekiwanie na odpowiedz
                {                    
                    switch(LINWaitForAnswerUNI02(RSDT1.inLIDN))
                    {
                        case 0:
                            return _LIN1_IBUSY;
                        case 1:
                            krok++;
                            break;
                        default:
                            krok=0;
                            return _LIN1_ERRODP;
                    }
                }
                case 3:         //3.Przygotowanie danych dla KSK
                {
                    krok=0;
                    if(FRL1f_ZeroAdr) return _LIN1_NORESP;                               //odpowiedz do LIN nie ma byc generowana?
                    if(!FRL1f_NextFr)                                                    //w trakcie wysylki do UNI-02 nie bylo aktualizacji ramki LIN?
                    {
                        switch(FRL1.IDN)
                        {
                            case _CLIN_RRD_P:
                            case _CLIN_RRD_I:
                            case _CLIN_RRD_D: 
                            case _CLIN_RRD_RGZ:    
                            {
                                FTL1.PCI_LENGTH=0x03;                                       //Rozmiar ramki(RSID+liczba bajtow danych bez CHK)
                                FTL1.WRD0.byte0=0xB2+0x40;                                      //RSID=SID+0x40
                                FTL1.WRD0.byte1=RSDT1.outLDTA1;  
                                FTL1.WRD1.byte0=RSDT1.outLDTA2;  
                                break;
                            }
                            case _CLIN_RRD_RPV:
                            {
                                FTL1.PCI_LENGTH=0x02;                                       //Rozmiar ramki(RSID+liczba bajtow danych bez CHK)
                                FTL1.WRD0.byte0=0xB2+0x40;                                      //RSID=SID+0x40
                                FTL1.WRD0.byte1=RSDT1.outLDTA1; 
                                break;
                            }
                            case _CLIN_RRD_RCO:
                            case _CLIN_RRD_RCW:       
                            {
                                FTL1.PCI_LENGTH=0x05;                                       //Rozmiar ramki(RSID+liczba bajtow danych bez CHK)
                                FTL1.WRD0.byte0=0xB2+0x40;                                      //RSID=SID+0x40
                                FTL1.WRD0.byte1=RSDT1.outLDTA1;  
                                FTL1.WRD1.byte0=RSDT1.outLDTA2;  
                                FTL1.WRD1.byte1=RSDT1.outLDTA3;
                                FTL1.WRD2.byte0=RSDT1.outLDTA4;                                
                                break;
                            } 
                            default:
                            {
                                return _LIN1_ERRODP;    
                            }                            
                        }
                    }
                    else                                                                //aktualizowano ramke - odpowiedz negatywna
                    {
                        return _LIN1_ERRODP;
                    }
                }
            }
            break;
        }        
        default:                   //nieznana komenda
        {
            return _LIN1_UNCCOM;
        }
    }
    return _LIN1_IFINAL;            //koniec przetwarzania danych
}
//------------------------------------------------------------
//------------------------------------------------------------
//******************************
void ifInitAutoMFrame(volatile unsigned char nr)
{
    static unsigned char pchk[10];//,a,b;
    volatile unsigned char chk;
    
    FTL1f_Auto=0;
    if(nr>=10) return;
    
    chk=CalcTabCHK(tbuf, 0, (FRL1.imax-1));            
    if(FRL1f_Auto)                      //master oczekuje na autoramke?
    {
        if(chk==pchk[nr])               //zawartosc bufora sie nie zmienila?
        {
            FTL1f_Auto=1;               //ustaw znacznik autoramki
            tbuf[0]=chk;                //wyslij tylko CHK bufora
            tbuf[1]=~chk;
            FRL1.imax=2;
            FRL1.STOP.war=FRL1.imax-1;
            Nop();
        }
        /*
        else
        {
            Nop();
            if(nr==3)
            {            
            if(b<99)b++;
            else b=0;
            }
        }*/
    }
    /*if(!FRL1f_Auto)
    {
        Nop();
        if(nr==3)
        {
            if(a<99)a++;
            else a=0;
        }
    }*/
    pchk[nr]=chk;    
}
//*****************************
//MULTIRESPONSE
//PRZYGOTOWANIE bufora do wysylki do MASTER (wskazanie adresu i rozmiaru oraz aktualizacja danych bufora)
//UWAGA: zwrot wartosci _LIN1_IBUSY sygnalizuje trwanie procesu (maksymalny czas zajetosci zdefiniowany parametrem _LN1_ADTA)
//Start nadawania - aktualizacja danych w buforze, wskazanie bufora z danymi
unsigned char LIN1StartMultiResponse_RD(void)
{
    volatile unsigned char dt,i;
    static unsigned char krok,bidn;
    BitSet3(&RSDT1.inLNSTAT,_FLIN,1);        //odswiez znacznik komunikacji LIN dla ukladu UNI-02
    PFNf_FLIN=1;    
    StartRTdS(_RTFLIN);
    
    //----------------    
    if(FRL1f_RSFrame)                 //ramka nadeslana z modulu Wifi?
    {
        if(!RSFInterpreter_MultiRD()) return _LIN1_UNCCOM;       
    }    
    //----------------
    if(bidn!=FRL1.IDN)                      //przy zmianie komendy zeruj stan procedury bezposredniej komunikacji z UNI-02
    {
        bidn=FRL1.IDN;
        krok=0;
    }
    //komenda na ktora nastapila reakcja ukladu SLAVE
    switch(FRL1.IDN)                        //identyfikator komendy
    {
        case _CLIN_RRD_STD:                 //odeslij do KSK standardowa paczke danych opisujaca stan sterownika
        {          
            //Odpowiedz zwrotna LCD do KSK
            //Przygotowanie danych dla KSK
            FRL1.imax=_NDT_RRD_STD;          //rozmiar tabeli
            FRL1.tab=tbuf;         //adres tabeli
            if(!FRL1.STOP.war&&FRL1.imax) FRL1.STOP.war=FRL1.imax-1;
            //!!!! aktualizacja zawartosci tabeli w zakresie od FRL1.START.war do FRL1.STOP.war
            tbuf[0]=RSDT1.outRDZ0;
            tbuf[1]=RSDT1.outRDZ1;
            tbuf[2]=RSDT1.outSTAT0;
            if(PFNf_EnAleg)
            {
                tbuf[2]|=0b00000010;
            }
            else
            {
                tbuf[2]&=0b11111101;
            }            
            tbuf[3]=RSDT1.outSTAT1;
            tbuf[4]=RSDT1.outSTAT2;
            if(PFNf_off)
            {
                tbuf[4]|=0b01000000;                             //ustaw tryb OFF
            }
            else
            {
                tbuf[4]&=0b10111111;                             //zeruj tryb OFF
            }
            if(PFNf_lato)
            {
                tbuf[4]|=0b00100000;                              //ustaw tryb LATO
                tbuf[4]&=0b11101111;                              //zeruj tryb ZIMA
            }
            else
            {
                tbuf[4]|=0b00010000;                              //ustaw tryb ZIMA
                tbuf[4]&=0b11011111;                              //zeruj tryb LATO
            }            
            tbuf[5]=RSDT1.outSTAT3;
            tbuf[6]=RSDT1.outCO;
            tbuf[7]=RSDT1.outCW;
            tbuf[8]=RSDT1.outCS;
            tbuf[9]=RSDT1.outIN;
            tbuf[10]=RSDT1.outVNT;
            tbuf[11]=(unsigned char)PFN.PCO;//RSDT1.outPCO;//**********************
            tbuf[12]=(unsigned char)PFN.PCW;//RSDT1.outPCW;//**********************
            tbuf[13]=RSDT1.outPPCO;
            tbuf[14]=RSDT1.outPPCW;
            tbuf[15]=RSDT1.outERR;
            tbuf[16]=RSDT1.outAWR;
            tbuf[17]=RSDT1.outECO;
            tbuf[18]=RSDT1.outPP;
            tbuf[19]=RSDT1.outPG;//23
            if(RSDT1.vSTD==1)
            {
                tbuf[20]=RSDT1.outCO;
                tbuf[21]=RSDT1.outCW;                
            }
            else
            {
                tbuf[20]=RSDT1.outrCO;
                tbuf[21]=RSDT1.outrCW; 
            }
            if(RSDT1.vSTD>=4)
            {
                tbuf[22]=RSDT1.outZW; 
            }
            else
            {
                tbuf[22]=0;
            }     
            if(RSDT1.vSTD>=6)
            {
                tbuf[23]=RSDT1.outPRZ; 
            }
            else
            {
                tbuf[23]=0;
            }             
            //******************************
            ifInitAutoMFrame(0);      
            //******************************
            break;
        }
        case _CLIN_RRD_STD2:                 //odeslij do KSK rozszerzona paczke danych opisujaca stan sterownika
        {
            //-----------------------------------
            //Parametry transmisji cyklicznej
            //++++++++++++++++++++++++++++++++++++++++++++
            //TEST SETP
            /*PFNf_LIN_STR=1;
            PFNf_LIN_CK=1;
            if(RdPrt(SS_KUP))
            {
                CMSGPar.PAR1=80;
                PFNf_LIN_PBCO=1;
            }
            if(RdPrt(SS_KDW))
            {
                CMSGPar.PAR1=40;
                PFNf_LIN_PBCO=0;
            }
            if((CMSGPar.PAR1>=PFN.SETPmin)&&(CMSGPar.PAR1<=PFN.SETPmax))
            {
                PFN.INSETP=CMSGPar.PAR1;
            }  
            else
            if(CMSGPar.PAR1<PFN.SETPmin)    
            {
                PFN.INSETP=PFN.SETPmin;
            }
            else
            if(CMSGPar.PAR1>PFN.SETPmax)
            {
                PFN.INSETP=PFN.SETPmax;      
            }*/
            //++++++++++++++++++++++++++++++++++++++++++++
            if(FRL1f_RSFrame)                 //ramka nadeslana z modulu Wifi?
            {
                CMSGPar.PAR0=0;
                CMSGPar.PAR1=0;                
            }
            else
            {
                CMSGPar.PAR0=FRL1.WRD1.byte0;
                CMSGPar.PAR1=FRL1.WRD1.byte1;
            }
            PFNf_LIN_CK=1;
            if(CMSGPar._EN_POB_CO) PFNf_LIN_EPBCO=1;    //znacznik waznosci stanu PBCO
            else PFNf_LIN_EPBCO=0;           
            if(CMSGPar._KSK_POB_CO&&CMSGPar._EN_POB_CO) PFNf_LIN_PBCO=1;    //cyklicznie odswierzane pobudzenie dla obiegu CO
            else PFNf_LIN_PBCO=0;
            if(CMSGPar._EN_POB_CW) PFNf_LIN_EPBCW=1;    //znacznik waznosci stanu PBCW
            else PFNf_LIN_EPBCW=0;
            if(CMSGPar._KSK_POB_CW&&CMSGPar._EN_POB_CW) PFNf_LIN_PBCW=1;    //cyklicznie odswierzane pobudzenie dla obiegu CW
            else PFNf_LIN_PBCW=0; 
            if(CMSGPar._TRB_KSK) PFNf_LIN_KSK=1;
            else PFNf_LIN_KSK=0;
            if(CMSGPar._TRB_STR) PFNf_LIN_STR=1;
            else PFNf_LIN_STR=0;

            //if(PFNf_LIN_KSK||PFNf_LIN_STR||PFNf_LIN_PWD||PFNf_LIN_BUF)
            if((!PFNf_LIN_BPSS&&PFNf_LIN_KSK)||(!PFNf_LIN_BPSS&&PFNf_LIN_STR)||PFNf_LIN_PWD||PFNf_LIN_BUF)  //+++                
            {
                //if(PFNf_LIN_KSK||PFNf_LIN_STR)
                if((!PFNf_LIN_BPSS&&PFNf_LIN_KSK)||(!PFNf_LIN_BPSS&&PFNf_LIN_STR))   //+++ 
                {
                    if(CMSGPar._DPGD) PFNf_LIN_DPGD=1;
                    else PFNf_LIN_DPGD=0;
                }
                else    //autonomiczna praca modulu rozszerzen
                {
                    PFNf_LIN_DPGD=0;
                }
                if(CMSGPar._NST_PCO)                        //nowa wartosc docelowej temperatury WG?
                {
                    CMSGPar._NST_PCO=0;
                    if(!PFNf_LIN_PWD)
                    {
                        if((CMSGPar.PAR1>=PFN.SETPmin)&&(CMSGPar.PAR1<=PFN.SETPmax))
                        {
                            PFN.INSETP=CMSGPar.PAR1;
                        }  
                        else
                        if(CMSGPar.PAR1<PFN.SETPmin)    
                        {
                            PFN.INSETP=PFN.SETPmin;
                        }
                        else
                        if(CMSGPar.PAR1>PFN.SETPmax)
                        {
                            PFN.INSETP=PFN.SETPmax;      
                        }    
                    }
                    else
                    {
                        if((CMSGPar.PAR1>=PFN.SETPmin)&&(CMSGPar.PAR1<=PFN.SETPPmax))
                        {
                            PFN.INSETPP=CMSGPar.PAR1;
                        }  
                        else
                        if(CMSGPar.PAR1<PFN.SETPmin)    
                        {
                            PFN.INSETPP=PFN.SETPmin;
                        }
                        else
                        if(CMSGPar.PAR1>PFN.SETPPmax)
                        {
                            PFN.INSETPP=PFN.SETPPmax;      
                        }                        
                    }
                }
            }
            else
            {
                PFNf_LIN_DPGD=0;
            }
            //-----------------------------------
            //Odpowiedz zwrotna LCD do KSK
            //Przygotowanie danych dla KSK
            FRL1.imax=_NDT_RRD_STD2;          //rozmiar tabeli
            FRL1.tab=tbuf;         //adres tabeli
            if(!FRL1.STOP.war&&FRL1.imax) FRL1.STOP.war=FRL1.imax-1;            
            //!!!! aktualizacja zawartosci tabeli w zakresie od FRL1.START.war do FRL1.STOP.war
            tbuf[0]=RSDT1.outSTAT0;
            if(PFNf_EnAleg)
            {
                tbuf[0]|=0b00000010;
            }
            else
            {
                tbuf[0]&=0b11111101;
            }         
            tbuf[1]=RSDT1.outSTAT1;
            tbuf[2]=RSDT1.outSTAT2;
            if(PFNf_off)
            {
                tbuf[2]|=0b01000000;                             //ustaw tryb OFF
            }
            else
            {
                tbuf[2]&=0b10111111;                             //zeruj tryb OFF
            }
            if(PFNf_lato)
            {
                tbuf[2]|=0b00100000;                              //ustaw tryb LATO
                tbuf[2]&=0b11101111;                              //zeruj tryb ZIMA
            }
            else
            {
                tbuf[2]|=0b00010000;                              //ustaw tryb ZIMA
                tbuf[2]&=0b11011111;                              //zeruj tryb LATO
            }            
            tbuf[3]=RSDT1.outSTAT3;
            tbuf[4]=RSDT1.outSTAT4;
            tbuf[5]=RSDT1.outSTAT5;   
            if(PFNf_ex_pgd)
            {
                tbuf[5]|=0b00100000;                             //ustaw znacznik aktywnosci zewnetrznego regulatora pogodowego
            }
            else
            {
                tbuf[5]&=0b11011111;                             //zeruj znacznik aktywnosci zewnetrznego regulatora pogodowego
            }
            tbuf[6]=RSDT1.outCO;
            tbuf[7]=RSDT1.outCW;
            tbuf[8]=RSDT1.outCS;
            tbuf[9]=RSDT1.outIN;
            tbuf[10]=RSDT1.outVNT;
            tbuf[11]=(unsigned char)PFN.PCO;//RSDT1.outPCO;//**********************
            tbuf[12]=(unsigned char)PFN.PCW;//RSDT1.outPCW;//**********************
            tbuf[13]=RSDT1.outPPCO;
            tbuf[14]=RSDT1.outPPCW;
            tbuf[15]=RSDT1.outERR;
            tbuf[16]=RSDT1.outAWR;
            tbuf[17]=RSDT1.outPP;
            tbuf[18]=RSDT1.outPG;
            if(RSDT1.vSTD==1)
            {
                tbuf[19]=RSDT1.outCO;
                tbuf[20]=RSDT1.outCW;                
            }
            else
            {
                tbuf[19]=RSDT1.outrCO;
                tbuf[20]=RSDT1.outrCW; 
            }

            tbuf[21]=PFN.CONTSETP;                                                                  //docelowa wartosc modulacji dla temp zasilania
            //tbuf[22]=RSDT1.outZW;      
            if(RSDT1.vSTD>=4)
            {
                tbuf[22]=RSDT1.outZW; 
            }
            else
            {
                tbuf[22]=0;
            }     
            if(RSDT1.vSTD>=6)
            {
                tbuf[23]=RSDT1.outPRZ; 
            }
            else
            {
                tbuf[23]=0;
            }             
            //******************************
            ifInitAutoMFrame(1);      
            //******************************            
            break;
        }
        case _CLIN_RRD_OT:
        {
            //-----------------------------------
            //Odpowiedz zwrotna LCD
            FRL1.imax=_NDT_RRD_OT;          //rozmiar tabeli
            FRL1.tab=tbuf;         //adres tabeli
            if(!FRL1.STOP.war&&FRL1.imax) FRL1.STOP.war=FRL1.imax-1;            

            if(PFNf_FOT)
            {
                tbuf[0]=1;                                      //znacznik aktywnosci interfejsu
                //Class 2, ID=2
                if(OTMaster.Ref._MemberID)                      //znacznik ID regulatora
                {
                    tbuf[1]=OTMaster.MemberID;
                }
                else
                {
                    tbuf[1]=0;
                }
                //Class 2, ID=126
                if(OTMaster.Ref._ProductVer)                    //wersja regulatora                   
                {
                    tbuf[2]=OTMaster.ProductVer;
                }
                else
                {
                    tbuf[2]=0;
                }
                //Class 2, ID=126
                if(OTMaster.Ref._ProductType)                   //typ regulatora
                {
                    tbuf[3]=OTMaster.ProductType;
                }
                else
                {
                    tbuf[3]=0;
                }     
                //Class 2, ID=124
                if(OTMaster.Ref._OTVer)                         //wersja protokolu OT
                {
                    tbuf[4]=OTMaster.OTVer;
                }
                else
                {
                    tbuf[4]=0;
                }    
                //Class 1, ID=0
                if(OTMaster.Ref._Status)                        //sta
                {
                    //byte1 - Master status
                    /*bit: description [ clear/0, set/1]
                    0: CH enable [ CH is disabled, CH is enabled]           //pobudzenie RT
                    1: DHW enable [ DHW is disabled, DHW is enabled]        //timer zasobnika
                    2: Cooling enable [ Cooling is disabled, Cooling is enabled]
                    3: OTC active [OTC not active, OTC is active]
                    4: CH2 enable [CH2 is disabled, CH2 is enabled]
                    5: Summer/winter mode [winter mode active, summer mode active]
                    6: DHW blocking [DHW unblocked, DHW blocked]
                    7: reserved
                     */                
                    tbuf[5]=OTMaster.Status.war;
                }
                else
                {
                    tbuf[5]=0;
                }  
                //Class 2, ID=2
                if(OTMaster.Ref._Config)
                {
                    /*bit: description [ clear/0, set/1]
                    0: Smart Power [not implemented, implemented]
                    1-7: reserved*/                
                    tbuf[6]=OTMaster.Config.war;
                }
                else
                {
                    tbuf[6]=0;
                }    
                //Status interfejsu OT
                dt=0;
                if(PFNf_FOT) dt=dt|0b00000001;                                                          //aktywnosc OT
                if(OTMaster.Status._CH_enable&&PFNf_FOT) dt=dt|0b00000010;                              //RT OT   
                if(PFNf_EnAleg&&(OTMaster.Status._DHW_enable&&PFNf_FOT)) dt=dt|0b00000100;              //CW OT            
                if(OTMaster.Ref._Control_Setpoint&&PFNf_FOT) dt=dt|0b00001000;                          //tryb sterowania z zadanym SETP z interfejsu OT
                if(PFNf_OT_DPGD&&PFNf_FOT) dt=dt|0b00010000;                                            //deaktywacja wbudowanego regulatora pogodowego za po¶rednictwem interfejsu OT

                tbuf[7]=dt;                
                
                //Class 1, ID=1
                if(OTMaster.Ref._Control_Setpoint&&PFNf_FOT)
                {
                    tbuf[8]=(unsigned char)OTMaster.Control_Setpoint;                     
                }
                else
                {
                    tbuf[8]=0;            
                }                
            }
            else
            {
                tbuf[0]=0;
                tbuf[1]=0;
                tbuf[2]=0;   
                tbuf[3]=0;
                tbuf[4]=0;
                tbuf[5]=0;     
                tbuf[6]=0; 
                tbuf[7]=0;    
                tbuf[8]=0;                
            } 
            //******************************
            ifInitAutoMFrame(2);      
            //******************************
            break;
        }
        case _CLIN_RRD_KNF:
        {
            //-----------------------------------
            //Odpowiedz zwrotna LCD
            //++++++++++++++++++++++++++++
            //FRL1.imax=_NDT_RRD_KNF;             //rozmiar tabeli
            FRL1.imax=GetKNPAR();               //rozmiar tabeli
            //++++++++++++++++++++++++++++
            FRL1.tab=tbuf;                      //adres tabeli
            if(!FRL1.STOP.war&&FRL1.imax) FRL1.STOP.war=FRL1.imax-1;
            //!!!! aktualizacja zawartosci tabeli w zakresie od FRL1.START.war do FRL1.STOP.war
            tbuf[0]=RSDT1.outMSTR;
            tbuf[1]=RSDT1.outMMAXU;
            tbuf[2]=RSDT1.outMMAXG;
            tbuf[3]=RSDT1.outMMIN;
            tbuf[4]=RSDT1.outMMAX; 
            tbuf[5]=RSDT1.outKODKT;   
            tbuf[6]=RSDT1.outWOBG;                       
            tbuf[7]=RSDT1.outOPDG;
            tbuf[8]=RSDT1.outTPCS;            
            tbuf[9]=RSDT1.outWANL;
            tbuf[10]=RSDT1.outNOKR;
            tbuf[11]=RSDT1.outRDPM;
            tbuf[12]=RSDT1.outDTPM;
            tbuf[13]=RSDT1.outMDPM;
            tbuf[14]=RSDT1.outTECO;
            tbuf[15]=RSDT1.outTMMAXG2;
            tbuf[16]=RSDT1.outMMAXG2;
            tbuf[17]=RSDT1.outMGPM;
            tbuf[18]=RSDT1.outMGPM2;  
            tbuf[19]=RSDT1.outHSCO;
            tbuf[20]=RSDT1.outCONW;  
            tbuf[21]=RSDT1.outPROP;
            tbuf[22]=RSDT1.outRFAN; 
            tbuf[23]=RSDT1.outTSTR;        
            tbuf[24]=RSDT1.outTL3;  
            tbuf[25]=RSDT1.outPGDR;
            tbuf[26]=RSDT1.outPGDG; 
            tbuf[27]=RSDT1.outPGDO;
            tbuf[28]=RSDT1.outPGMX;              
            tbuf[29]=RSDT1.outCWMG;   
            tbuf[30]=RSDT1.outCWTM; 
            tbuf[31]=RSDT1.outPWTM;   
            
            tbuf[32]=RSDT1.outTPCW; 
            tbuf[33]=RSDT1.outTMPM;            
            //******************************
            ifInitAutoMFrame(3);      
            //******************************             
            break;
        }   
        case _CLIN_RRD_EEST:
        {
            //konieczna odpowiedz zwrotna UNI-02 do KSK stanowiaca potwierdzenie wykonania komendy
            switch(krok)
            {
                case 0:         //0.Odczekaj na zakonczenie poprzedniej transmisji do UNI-02
                    if(!LINWaitForAnswerUNI02(_LIN_GEEST)) return _LIN1_IBUSY;
                    krok++;
                case 1:         //1.Wysylka komendy do UNI-02
                {
                    //komendy i parametry wywolania dla UNI-02
                    RSDT1.inLIDN=_LIN_GEEST;             //komenda  
                    RSDT1.inLDTA1=FRL1.WRD1.byte0;       //parametr = nr statusu
                    for(i=0;i<32;i++) stbf[i]=0;
                    
                    //start wywolania
                    if(!FRL1.NAD) FRL1f_ZeroAdr=1;
                    else FRL1f_ZeroAdr=0;
                    RSDT1f_NewLIDN=1;
                    RSDT1.ACKUNI=0;
                    FRL1f_NextFr=0;
                    krok++;
                }
                case 2:         //2.Oczekiwanie na odpowiedz
                {
                    switch(LINWaitForAnswerUNI02(_LIN_GEEST))
                    {
                        case 0:
                            return _LIN1_IBUSY;
                        case 1:
                            krok++;
                            break;
                        default:
                            krok=0;
                            return _LIN1_ERRODP;
                    }
                }
                case 3:         //3.Przygotowanie danych dla LIN
                {
                    krok=0;
                    if(FRL1f_ZeroAdr) return _LIN1_NORESP;                               //odpowiedz do LIN nie ma byc generowana?
                    if(!FRL1f_NextFr)                                                    //w trakcie wysylki do UNI-02 nie bylo aktualizacji inna ramka LIN?
                    {
                        if(!stbf[0]&&!stbf[1]) 
                        {
                            FRL1.imax=2;             //rozmiar tabeli
                        }
                        else
                        {           
                            FRL1.imax=_NDT_RRD_EEST;             //rozmiar tabeli
                        }
                        FRL1.tab=tbuf;                      //adres tabeli
                        if(!FRL1.STOP.war&&FRL1.imax) FRL1.STOP.war=FRL1.imax-1;
                        //!!!! aktualizacja zawartosci tabeli w zakresie od FRL1.START.war do FRL1.STOP.war
                        for(i=0;i<_NRS_TBF2;i++)
                        {
                            tbuf[i]=0;    
                        }                        
                        for(i=0;i<FRL1.imax;i++)
                        {
                            if(i>=32) break;
                            tbuf[i]=stbf[i];    
                        }                          
                        //******************************
                        ifInitAutoMFrame(4);      
                        //******************************
                    }
                    else                                                                //aktualizowano ramke - odpowiedz negatywna
                    {
                        return _LIN1_ERRODP;
                    }
                }
            }
            break;
        }
        case _CLIN_RRD_HIST:
        {
            //konieczna odpowiedz zwrotna UNI-02 do KSK stanowiaca potwierdzenie wykonania komendy
            switch(krok)
            {
                case 0:         //0.Odczekaj na zakonczenie poprzedniej transmisji do UNI-02
                    if(!LINWaitForAnswerUNI02(_LIN_GHIST)) return _LIN1_IBUSY;
                    krok++;
                case 1:         //1.Wysylka komendy do UNI-02
                {
                    //komendy i parametry wywolania dla UNI-02
                    RSDT1.inLIDN=_LIN_GHIST;             //komenda
                    for(i=0;i<32;i++) stbf[i]=0;
                    
                    //start wywolania
                    if(!FRL1.NAD) FRL1f_ZeroAdr=1;
                    else FRL1f_ZeroAdr=0;
                    RSDT1f_NewLIDN=1;
                    RSDT1.ACKUNI=0;
                    FRL1f_NextFr=0;
                    krok++;
                }
                case 2:         //2.Oczekiwanie na odpowiedz
                {
                    switch(LINWaitForAnswerUNI02(_LIN_GHIST))
                    {
                        case 0:
                            return _LIN1_IBUSY;
                        case 1:
                            krok++;
                            break;
                        default:
                            krok=0;
                            return _LIN1_ERRODP;
                    }
                }
                case 3:         //3.Przygotowanie danych dla LIN
                {
                    krok=0;
                    if(FRL1f_ZeroAdr) return _LIN1_NORESP;                               //odpowiedz do LIN nie ma byc generowana?
                    if(!FRL1f_NextFr)                                                    //w trakcie wysylki do UNI-02 nie bylo aktualizacji inna ramka LIN?
                    {
                        for(i=0;i<_NRS_TBF2;i++)
                        {
                            tbuf[i]=0;    
                        }                        
                        if((!stbf[0]&&!stbf[1])||!stbf[2]) 
                        {
                            tbuf[0]=stbf[0];
                            tbuf[1]=stbf[1];                            
                            FRL1.imax=2;             //rozmiar tabeli
                        }
                        else
                        {           
                            tbuf[0]=stbf[0];
                            tbuf[1]=stbf[1];                            
                            FRL1.imax=2+stbf[2];             //rozmiar tabeli
                            for(i=0;i<stbf[2];i++)
                            {
                                if((3+i)>=32) break;                    
                                tbuf[2+i]=stbf[3+i];    
                            }                
                        }
                        FRL1.tab=tbuf;                      //adres tabeli
                        if(!FRL1.STOP.war&&FRL1.imax) FRL1.STOP.war=FRL1.imax-1;
                        //!!!! aktualizacja zawartosci tabeli w zakresie od FRL1.START.war do FRL1.STOP.war                           
                        //******************************
                        ifInitAutoMFrame(5);      
                        //******************************
                    }
                    else                                                                //aktualizowano ramke - odpowiedz negatywna
                    {
                        return _LIN1_ERRODP;
                    }
                }
            }
            break;            
        }        
        /*case _CLIN_RRD_KNF:                         //odeslij do KSK paczke danych opisujaca konfiguracje sterownika
        {
            //konieczna odpowiedz zwrotna UNI-02 do KSK
            switch(krok)
            {
                case 0:         //0.Odczekaj na zakonczenie poprzedniej transmisji do UNI-02
                    if(!LINWaitForAnswerUNI02(_LIN_GKNF)) return _LIN1_IBUSY;
                    krok++;
                case 1:         //1.Wysylka komendy do UNI-02
                {
                    //start wywolania
                    if(!FRL1.NAD) FRL1f_ZeroAdr=1;
                    else FRL1f_ZeroAdr=0;
                    RSDT1.inLIDN=_LIN_GKNF;
                    RSDT1f_NewLIDN=1;
                    RSDT1.ACKUNI=0;
                    FRL1f_NextFr=0;
                    krok++;
                }
                case 2:         //2.Oczekiwanie na odpowiedz z UNI-02
                {
                    switch(LINWaitForAnswerUNI02(_LIN_GKNF))
                    {
                        case 0:
                            return _LIN1_IBUSY;
                        case 1:
                            krok++;
                            break;
                        default:
                            krok=0;
                            return _LIN1_ERRODP;
                    }
                }
                case 3:         //3.Przygotowanie danych dla LIN
                {
                    krok=0;
                    if(FRL1f_ZeroAdr) return _LIN1_NORESP;                               //odpowiedz do LIN nie ma byc generowana?
                    if(!FRL1f_NextFr)                                                    //w trakcie wysylki do UNI-02 nie bylo aktualizacji inna ramka LIN?
                    {
                        FRL1.imax=_NDT_RRD_KNF;             //rozmiar tabeli
                        FRL1.tab=tbuf;                      //adres tabeli
                        if(!FRL1.STOP.war&&FRL1.imax) FRL1.STOP.war=FRL1.imax-1;
                        //!!!! aktualizacja zawartosci tabeli w zakresie od FRL1.START.war do FRL1.STOP.war
                        tbuf[0]=RSDT1.outKSTAT0;
                        if(PFNf_aktywne_KNF)                //aktywny tryb konfiguracji?
                        {
                            tbuf[1]=RSDT1.outKRK;
                            tbuf[2]=RSDT1.outPRM;
                        }
                        else
                        {
                            tbuf[1]=0;
                            tbuf[2]=0;
                        }
                        tbuf[3]=RSDT1.outMSTR;
                        tbuf[4]=RSDT1.outMMAXU;
                        tbuf[5]=RSDT1.outMMAXG;
                        tbuf[6]=RSDT1.outOPDG;
                        tbuf[7]=RSDT1.outWANL;
                        tbuf[8]=RSDT1.outKODKT;
                        tbuf[9]=RSDT1.outWOBG;
                        tbuf[10]=RSDT1.outNOKR;
                        tbuf[11]=RSDT1.outTPCS;
                        tbuf[12]=RSDT1.outMMIN;
                        tbuf[13]=RSDT1.outMMAX;
                        tbuf[14]=RSDT1.outRDPM;
                        tbuf[15]=RSDT1.outDTPM;
                        tbuf[16]=RSDT1.outMDPM;
                        tbuf[17]=RSDT1.outTECO;
                        tbuf[18]=RSDT1.outTMMAXG2;
                        tbuf[19]=RSDT1.outMMAXG2;
                        tbuf[20]=RSDT1.outMGPM;
                        tbuf[21]=RSDT1.outMGPM2;  
                        tbuf[22]=RSDT1.outHSCO;
                        tbuf[23]=RSDT1.outCONW;  
                        tbuf[24]=RSDT1.outPROP;
                        tbuf[25]=RSDT1.outRFAN; 
                        tbuf[26]=RSDT1.outTSTR;        
                        tbuf[27]=RSDT1.outTL3;  
                        tbuf[28]=RSDT1.outPGDR;
                        tbuf[29]=RSDT1.outPGDG; 
                        tbuf[30]=RSDT1.outPGDO;                        
                    }
                    else                                                                //aktualizowano ramke - odpowiedz negatywna
                    {
                        Nop();
                        return _LIN1_ERRODP;
                    }
                    break;
                }
            }
            break;
        }*/
        default:                   //nieznana komenda
        {
            return _LIN1_UNCCOM;
        }
    }
    if((FRL1.STOP.war<FRL1.START.war)||(FRL1.STOP.war>=FRL1.imax))
    {
        return _LIN1_INVPAR;       //nieprawidlowy parametr
    }
    return _LIN1_IFINAL;            //koniec przetwarzania danych
}
//------------------------------------------------------------
//------------------------------------------------------------
//Oczekiwanie na odpowiedz UNI-02
unsigned char LINWaitForAnswerUNI02(const unsigned char kmn)
{
    if(RSDT1f_NewLIDN) return 0;  //oczekiwanie na zakonczenie transmisji
    if(RSDT1.ACKUNI!=1) 
    {
        RSDT1f_RD_LIN=0;
        return 2;
    }
    if(!RSDT1f_RD_LIN) return 2;   //nie zaktualizowano danych z UNI-02?
    RSDT1f_RD_LIN=0;
    if((RSDT1.outLIDN!=kmn)||(RSDT1.outLIDN!=RSDT1.inLIDN)) return 2;  //nie prawidlowa komenda?
    return 1;   //odpowiedz gotowa do interpretacji
}
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//Decyzja po otrzymaniu ramki z modulu Wifi
//------------------------------------------------------------
//------------------------------------------------------------
#if _OLD_RS_KOM!=0
unsigned char RSFInterpreter_SingleRD(void)
{
    if(FRL1.RS_ADR>1) return 0;   //ADR paczki RS >1 ?
    if(FRL1.RS_KOM!=0x07) return 0;     //nieprawidlowa komenda?
    
    switch(FRL1.RS_IDN&0b01111111) //IDN paczki RS
    {
        case 0x01:
        {
            FRL1.IDN=_CLIN_RRD_ACK;
            return 1;
        }
        case 0x05:
        {
            FRL1.IDN=_CLIN_RRD_PCW;
            return 1;                    
        }    
        case 0x06:
        {
            FRL1.IDN=_CLIN_RRD_PCO;
            return 1;                    
        }
        case 0x07:
        {
            FRL1.IDN=_CLIN_RRD_ECO;
            return 1;                    
        }
        case 0x08:
        {
            FRL1.IDN=_CLIN_RRD_TRB;
            return 1;                    
        }     
        case 0x09:
        {
            FRL1.IDN=_CLIN_RRD_RES;
            return 1;                    
        }   
        case 0x0A:
        {
            FRL1.IDN=_CLIN_RRD_BRES;
            return 1;                    
        }     
        case 0x0B:
        {
            FRL1.IDN=_CLIN_RRD_FSR;
            return 1;                    
        }  
        case 0x0C:
        {
            FRL1.IDN=_CLIN_RRD_ANL;
            return 1;                    
        } 
        case 0x0D:
        {
            FRL1.IDN=_CLIN_RRD_NKNF;
            return 1;                    
        } 
        case 0x0E:
        {
            FRL1.IDN=_CLIN_RRD_PKNF;
            return 1;                    
        }  
        case 0x0F:
        {
            FRL1.IDN=_CLIN_RRD_RT;
            return 1;                    
        }
        case 0x10:
        {
            FRL1.IDN=_CLIN_RRD_CW;
            return 1;                    
        }   
        case 0x11:
        {
            FRL1.IDN=_CLIN_RRD_10V;
            return 1;                    
        }  
        case 0x12:
        {
            FRL1.IDN=_CLIN_RRD_SETP;
            return 1;                    
        } 
        case 0x13:
        {
            FRL1.IDN=_CLIN_RRD_ZW;
            return 1;                    
        }  
        case 0x14:
        {
            FRL1.IDN=_CLIN_RRD_INT;
            return 1;                    
        } 
        case 0x15:
        {
            FRL1.IDN=_CLIN_RRD_PRD;
            return 1;                    
        }        
        default:
        {
            return 0;
        }
    }          
}
//------------------------------------------------------------
//------------------------------------------------------------
unsigned char RSFInterpreter_MultiRD(void)
{
    if(FRL1.RS_ADR>1) return 0;   //ADR paczki RS >1 ?

    switch(FRL1.RS_KOM)
    {
        case 0x01:
        {    
            switch(FRL1.RS_IDN&0b01111111) //IDN paczki RS 
            {
                case 0x01:
                {
                    FRL1.IDN=_CLIN_RRD_STD2;
                    //if(!FRL1.STOP.war) FRL1.STOP.war=_NDT_RRD_STD2;
                    return 1;
                }        
                default:
                {
                    return 0;
                }
            }
        }
        case 0x07:
        {    
            switch(FRL1.RS_IDN&0b01111111) //IDN paczki RS 
            {
                case 0x02:
                {
                    FRL1.IDN=_CLIN_RRD_OT;
                    //if(!FRL1.STOP.war) FRL1.STOP.war=_NDT_RRD_OT;
                    return 1;
                } 
                case 0x03:
                {
                    FRL1.IDN=_CLIN_RRD_KNF;
                    //if(!FRL1.STOP.war) FRL1.STOP.war=_NDT_RRD_KNF;                    
                    return 1;
                }                
                default:
                {
                    return 0;
                }
            }
        }        
        default:
        {
            return 0;
        }
    }
}
//------------------------------------------------------------
//------------------------------------------------------------
unsigned char RSFInterpreter_SingleWR(void)
{
    if(FRL1.RS_ADR>1) return 0;   //ADR paczki RS >1 ?
    
    switch(FRL1.RS_KOM)
    {
        case 0x82:
        {
            switch(FRL1.RS_IDN&0b01111111) //IDN paczki RS
            {
                case 0x01:
                {
                    FRL1.IDN=_CLIN_WRD_LADDR;                //zmien adres LIN plytki sterujacej UNI-02 (nowy adres w parametr1)
                    return 1;                    
                }    
                case 0x02:
                {
                    FRL1.IDN=_CLIN_WRD_LIFADDR;              //zmien adres LIN plytki sterujacej UNI-02 (nowy adres w parametr1) po spelnieniu warunku
                    return 1;                    
                }
                case 0x03:
                {
                    FRL1.IDN=_CLIN_WRD_SHOWADDR;            //wyswietl adres LIN;
                    return 1;                    
                }
                case 0x04:
                {
                    FRL1.IDN=_CLIN_WRD_PULSE;              //pulsacja podswietlenia
                    return 1;                    
                }                              
                default:
                {
                    return 0;
                }
            }
        }
        case 0x87:
        {
            switch(FRL1.RS_IDN&0b01111111) //IDN paczki RS
            {
                case 0x05:
                {
                    FRL1.IDN=_CLIN_WRD_PCW;
                    return 1;                    
                }    
                case 0x06:
                {
                    FRL1.IDN=_CLIN_WRD_PCO;
                    return 1;                    
                }
                case 0x07:
                {
                    FRL1.IDN=_CLIN_WRD_ECO;
                    return 1;                    
                }
                case 0x08:
                {
                    FRL1.IDN=_CLIN_WRD_TRB;
                    return 1;                    
                }     
                case 0x09:
                {
                    FRL1.IDN=_CLIN_WRD_RES;
                    return 1;                    
                }   
                case 0x0A:
                {
                    FRL1.IDN=_CLIN_WRD_BRES;
                    return 0;                       //brak mozliwosci zdalnego resetu 
                }     
                case 0x0B:
                {
                    FRL1.IDN=_CLIN_WRD_FSR;
                    return 1;                    
                }  
                case 0x0C:
                {
                    FRL1.IDN=_CLIN_WRD_ANL;
                    return 1;                    
                } 
                case 0x0E:
                {
                    FRL1.IDN=_CLIN_WRD_PKNF;
                    return 1;                    
                }  
                case 0x0F:
                {
                    FRL1.IDN=_CLIN_WRD_RT;
                    return 1;                    
                }
                case 0x10:
                {
                    FRL1.IDN=_CLIN_WRD_CW;
                    return 1;                    
                }
                case 0x11:
                {
                    FRL1.IDN=_CLIN_WRD_ENDO;
                    return 1;                    
                }
                case 0x12:
                {
                    FRL1.IDN=_CLIN_WRD_PRD;
                    return 1;                    
                }        
                default:
                {
                    return 0;
                }
            }             
        }
        default:
        {
            return 0;
        }
    }              
}
#else
//----------------------------------------------------------------
//Pojedyncza ramka odczytu RD
unsigned char RSFInterpreter_SingleRD(void)
{
    if(FRL1.RS_ADR>1) return 0;   //ADR paczki RS >1 ?

    switch(FRL1.RS_KOM)
    {
        case 0x03:  //Parametry konfiguracji
        {
            switch(FRL1.RS_IDN&0b01111111) //IDN paczki RS
            {
                case 0x01://liczba parametrow konfiguracyjnych
                {
                    FRL1.IDN=_CLIN_RRD_NKNF;
                    return 1;
                }
                case 0x02://wartosc wbranego parametru konfiguracji, warto¶ci graniczne
                {
                    FRL1.IDN=_CLIN_RRD_PKNF;
                    return 1;                    
                }
                default:
                {
                    return 0;
                }
            }
        }       
        case 0x07:  //Pozostale parametry
        {        
            switch(FRL1.RS_IDN&0b01111111) //IDN paczki RS
            {
                case 0x01:  //potwierdzenie obecnosci wraz z kompletem danych o sterowniku
                {
                    FRL1.IDN=_CLIN_RRD_ACK;
                    return 1;
                }
                case 0x02:  //Nastawa PCW, wartosci graniczne
                {
                    FRL1.IDN=_CLIN_RRD_PCW;
                    return 1;                    
                }    
                case 0x03://Nastawa PCO, wartosci graniczne
                {
                    FRL1.IDN=_CLIN_RRD_PCO;
                    return 1;                    
                }
                case 0x04://Nastawa ECO, wartosci graniczne
                {
                    FRL1.IDN=_CLIN_RRD_ECO;
                    return 1;                    
                }
                case 0x05://Tryb pracy , wartosci graniczne
                {
                    FRL1.IDN=_CLIN_RRD_TRB;
                    return 1;                    
                }     
                case 0x06://Stan przycisku reset
                {
                    FRL1.IDN=_CLIN_RRD_RES;
                    return 1;                    
                }   
                case 0x07://Blokada awaryjna
                {
                    FRL1.IDN=_CLIN_RRD_BRES;
                    return 1;                    
                }     
                case 0x08://Funkcja serwisowa
                {
                    FRL1.IDN=_CLIN_RRD_FSR;
                    return 1;                    
                }  
                case 0x09://Funkcja antylegionella
                {
                    FRL1.IDN=_CLIN_RRD_ANL;
                    return 1;                    
                }  
                case 0x0A://Pobudzenie RT
                {
                    FRL1.IDN=_CLIN_RRD_RT;
                    return 1;                    
                }
                case 0x0B://Pobudzenie CW / timera zasobnika
                {
                    FRL1.IDN=_CLIN_RRD_CW;
                    return 1;                    
                }   
                case 0x0C://Status interfejsu 10V
                {
                    FRL1.IDN=_CLIN_RRD_10V;
                    return 1;                    
                }  
                case 0x0D://Docelowa wartosc modulacji przy grzaniu obiegu WG (SETP)
                {
                    FRL1.IDN=_CLIN_RRD_SETP;
                    return 1;                    
                } 
                case 0x0E://Temp zewnetrzna, reg pogodowy
                {
                    FRL1.IDN=_CLIN_RRD_ZW;
                    return 1;                    
                }  
                case 0x0F://Status interfejsów
                {
                    FRL1.IDN=_CLIN_RRD_INT;
                    return 1;                    
                } 
                case 0x10://Specjalny tryb pracy sterownika
                {
                    FRL1.IDN=_CLIN_RRD_PRD;
                    return 1;                    
                }    
                case 0x11://Statystyki zdarzen awaryjnych
                {
                    if(PFN.vUNI02<11) return 0;
                    FRL1.IDN=_CLIN_RRD_STH;
                    return 1;
                }  
                case 0x12://wartosc dolnej granicy wentylatora
                {
                    if(PFN.vUNI02<16) return 0;
                    FRL1.IDN=_CLIN_RRD_RPV;
                    return 1;
                }  
                case 0x13://odczyt parametru P
                {
                    if(PFN.vUNI02<16) return 0;
                    FRL1.IDN=_CLIN_RRD_P;
                    return 1;
                }     
                case 0x14://odczyt parametru I
                {
                    if(PFN.vUNI02<16) return 0;
                    FRL1.IDN=_CLIN_RRD_I;
                    return 1;
                }    
                case 0x15://odczyt parametru D
                {
                    if(PFN.vUNI02<16) return 0;
                    FRL1.IDN=_CLIN_RRD_D;
                    return 1;
                }   
                case 0x16://odczyt wspolczynniki korekty modulacji CO przy wyborze typu 2 wentylatora (domyslnie wartosc 1)
                {
                    if(PFN.vUNI02<16) return 0;
                    FRL1.IDN=_CLIN_RRD_RCO;
                    return 1;
                }        
                case 0x17://odczyt wspolczynniki korekty modulacji CW przy wyborze typu 2 wentylatora (domyslnie wartosc 1)
                {
                    if(PFN.vUNI02<16) return 0;
                    FRL1.IDN=_CLIN_RRD_RCW;
                    return 1;
                }    
                case 0x18://odczyt liczba godzin pracy
                {
                    if(PFN.vUNI02<17) return 0;
                    FRL1.IDN=_CLIN_RRD_RGZ;
                    return 1;
                }                
                default:
                {
                    return 0;
                }
            }
        }
        default:
        {
            return 0;
        }            
    }      
}
//----------------------------------------------------------------
//Multi ramka odczytu RD
unsigned char RSFInterpreter_MultiRD(void)
{
    if(FRL1.RS_ADR>1) return 0;   //ADR paczki RS >1 ?

    switch(FRL1.RS_KOM)
    {
        case 0x01:  //Status w multiramce
        {    
            switch(FRL1.RS_IDN&0b01111111) //IDN paczki RS 
            {
                case 0x01://STATUS - Pobierz komplet danych okreslajacych stan urz±dzenia
                {
                    FRL1.IDN=_CLIN_RRD_STD2;
                    return 1;
                }        
                default:
                {
                    return 0;
                }
            }
        }
        case 0x03:  //Parametry konfiguracyjne w multiramce
        {    
            switch(FRL1.RS_IDN&0b01111111) //IDN paczki RS 
            {
                case 0x01://KONFIG - Pobierz komplet parametrow konfiguracyjnych (wartosci bez min/max)
                {
                    FRL1.IDN=_CLIN_RRD_KNF;                 
                    return 1;
                }                
                default:
                {
                    return 0;
                }
            }
        }        
        case 0x07:  //Pozostale parametry w multiramce
        {    
            switch(FRL1.RS_IDN&0b01111111) //IDN paczki RS 
            {
                case 0x01://Status interfejsu OT
                {
                    FRL1.IDN=_CLIN_RRD_OT;
                    return 1;
                }   
                case 0x02://Status sterownika z momentu wylaczenia awaryjnego z blokada
                {
                    if(PFN.vUNI02<11) return 0;
                    FRL1.IDN=_CLIN_RRD_EEST;
                    return 1;
                }
                case 0x03://Odczyt zapisu historii zdarzen awaryjnych (do 16 ostatnich zdarzen)
                {
                    if(PFN.vUNI02<11) return 0;
                    FRL1.IDN=_CLIN_RRD_HIST;
                    return 1;
                }                
                default:
                {
                    return 0;
                }
            }
        }        
        default:
        {
            return 0;
        }
    }
}
//----------------------------------------------------------------
//Pojedyncza ramka zapisu WD
unsigned char RSFInterpreter_SingleWR(void)
{
    if(FRL1.RS_ADR>1) return 0;   //ADR paczki RS >1 ?
    
    switch(FRL1.RS_KOM)
    {
        case 0x82:  //Konfiguracja magistrali LIN, wizualizacja i sterowanie pulsacja podswietlenia
        {
            switch(FRL1.RS_IDN&0b01111111) //IDN paczki RS
            {
                case 0x01:
                {
                    FRL1.IDN=_CLIN_WRD_LADDR;                //zmien adres LIN plytki sterujacej UNI-02 (nowy adres w parametr1)
                    return 1;                    
                }    
                case 0x02:
                {
                    FRL1.IDN=_CLIN_WRD_LIFADDR;              //zmien adres LIN plytki sterujacej UNI-02 (nowy adres w parametr1) po spelnieniu warunku
                    return 1;                    
                }
                case 0x03:
                {
                    FRL1.IDN=_CLIN_WRD_SHOWADDR;            //wyswietl adres LIN;
                    return 1;                    
                }
                case 0x04:
                {
                    FRL1.IDN=_CLIN_WRD_PULSE;              //pulsacja podswietlenia
                    return 1;                    
                }                              
                default:
                {
                    return 0;
                }
            }
        }
        case 0x83:  //Parametry konfiguracyjne
        {
            switch(FRL1.RS_IDN&0b01111111) //IDN paczki RS
            {
                case 0x01://Zapis parametru konfiguracji
                {
                    FRL1.IDN=_CLIN_WRD_PKNF;                //zmien adres LIN plytki sterujacej UNI-02 (nowy adres w parametr1)
                    return 1;                    
                }                                 
                default:
                {
                    return 0;
                }
            }
        }        
        case 0x87:  //Pozostale parametry
        {
            switch(FRL1.RS_IDN&0b01111111) //IDN paczki RS
            {
                case 0x01://Zapisz nastwê PCW
                {
                    FRL1.IDN=_CLIN_WRD_PCW;
                    return 1;                    
                }    
                case 0x02://Zapisz nastwê PCO
                {
                    FRL1.IDN=_CLIN_WRD_PCO;
                    return 1;                    
                }
                case 0x03://Zapisz nastwê ECO
                {
                    FRL1.IDN=_CLIN_WRD_ECO;
                    return 1;                    
                }
                case 0x04://Zapisz tryb pracy
                {
                    FRL1.IDN=_CLIN_WRD_TRB;
                    return 1;                    
                }     
                case 0x05://Chwilowa aktywacja przycisku reset
                {
                    FRL1.IDN=_CLIN_WRD_RES;
                    return 1;                    
                }   
                case 0x06://Zniesienie blokady awaryjnej
                {
                    FRL1.IDN=_CLIN_WRD_BRES;
                    return 0;                       //brak mozliwosci zdalnego resetu 
                }     
                case 0x07://Sterowanie funkcj± serwisow±
                {
                    FRL1.IDN=_CLIN_WRD_FSR;
                    return 1;                    
                }  
                case 0x08://Sterowanie funkcj± antylegionella
                {
                    FRL1.IDN=_CLIN_WRD_ANL;
                    return 1;                    
                }   
                case 0x09://Zdalne pobudzenie RT
                {
                    FRL1.IDN=_CLIN_WRD_RT;
                    return 1;                    
                }
                case 0x0A://Zdalne pobudzenie od timera zasobnika
                {
                    FRL1.IDN=_CLIN_WRD_CW;
                    return 1;                    
                }
                case 0x0B://Deaktywacja proc. odpowietrzenia 
                {
                    FRL1.IDN=_CLIN_WRD_ENDO;
                    return 1;                    
                }
                case 0x0C://Specjalny tryb pracy sterownika
                {
                    FRL1.IDN=_CLIN_WRD_PRD;
                    return 1;                    
                } 
                case 0x0D://zerowanie licznika flagi serwisowej historii zdarzen awaryjnych
                {
                    FRL1.IDN=_CLIN_WRD_HFLG;
                    return 1;                    
                }          
                case 0x0E://wartosc dolnej granicy wentylatora
                {
                    if(PFN.vUNI02<16) return 0;
                    FRL1.IDN=_CLIN_WRD_RPV;
                    return 1;
                }  
                case 0x0F://parametr P
                {
                    if(PFN.vUNI02<16) return 0;
                    FRL1.IDN=_CLIN_WRD_P;
                    return 1;
                }     
                case 0x10://parametr I
                {
                    if(PFN.vUNI02<16) return 0;
                    FRL1.IDN=_CLIN_WRD_I;
                    return 1;
                }    
                case 0x11://parametr D
                {
                    if(PFN.vUNI02<16) return 0;
                    FRL1.IDN=_CLIN_WRD_D;
                    return 1;
                }   
                case 0x12://korekta punktu rownowagi w fazie dochodzenia (FAZA1)
                {
                    if(PFN.vUNI02<16) return 0;
                    FRL1.IDN=_CLIN_WRD_RCO1;
                    return 1;
                }  
                case 0x13://regulacja w punkcie rownowagi w fazie stabilizacji (FAZA2)
                {
                    if(PFN.vUNI02<16) return 0;
                    FRL1.IDN=_CLIN_WRD_RCO2;
                    return 1;
                }   
                case 0x14://korekta punktu rownowagi w fazie stabilizacji (FAZA2)
                {
                    if(PFN.vUNI02<16) return 0;
                    FRL1.IDN=_CLIN_WRD_RCO3;
                    return 1;
                }   
                case 0x15://reakcja superwizyjna
                {
                    if(PFN.vUNI02<16) return 0;
                    FRL1.IDN=_CLIN_WRD_RCO4;
                    return 1;
                }     
                case 0x16://korekta punktu rownowagi w fazie dochodzenia (FAZA1)
                {
                    if(PFN.vUNI02<16) return 0;
                    FRL1.IDN=_CLIN_WRD_RCW1;
                    return 1;
                }  
                case 0x17://regulacja w punkcie rownowagi w fazie stabilizacji (FAZA2)
                {
                    if(PFN.vUNI02<16) return 0;
                    FRL1.IDN=_CLIN_WRD_RCW2;
                    return 1;
                }   
                case 0x18://korekta punktu rownowagi w fazie stabilizacji (FAZA2)
                {
                    if(PFN.vUNI02<16) return 0;
                    FRL1.IDN=_CLIN_WRD_RCW3;
                    return 1;
                }   
                case 0x19://reakcja superwizyjna
                {
                    if(PFN.vUNI02<16) return 0;
                    FRL1.IDN=_CLIN_WRD_RCW4;
                    return 1;
                }  
                case 0x1A://zerowanie liczby godzin pracy
                {
                    if(PFN.vUNI02<17) return 0;
                    FRL1.IDN=_CLIN_WRD_RGZ;
                    return 1;
                }                 
                default:
                {
                    return 0;
                }
            }             
        }
        default:
        {
            return 0;
        }
    }              
}
//----------------------------------------------------------------
#endif
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
#endif      //(_EXT_BOARD>=2)


