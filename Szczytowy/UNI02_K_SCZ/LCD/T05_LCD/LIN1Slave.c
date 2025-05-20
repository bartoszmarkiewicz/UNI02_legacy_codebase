/*LIN1Slave.c*/
/*---------------------------------------------------------------------------------*/
//MODUL LIN SLAVE DLA UART1
//Funkcje obslugi RS232 w trybie LIN.
//Interfejs oparty na ukladzie MCP2003A
//W czesci naglowkowej nalezy umiescic
//      #include "LIN1SInterpreter.h"
//W czesci inicializacyjnej nalezy umiescic
//      InitTimer3();
//      InitUart1();
//W petli glownej nalezy umiescic funkcje:
//      LIN1SDeamon();
//W obsludze przerwania:
//      if((RC1IF) && (RC1IE)) //odbior RS232 (RCIF jest zerowane sprzetowo)
//      {
//          IntUSART1SRC();     //slave
//      }
//      if((TX1IF) && (TX1IE)) //transmisja RS232 (TXIF jest zerowane sprzetowo)
//      {
//          IntUSART1STR();     //slave
//      }
//      if((TMR3IF) && (TMR3IE))
//      {
//          TMR3IF = 0;
//          ReloadTimer3(); //obsluga zegara Timer3
//          IntLIN2Tim();   //obsluga zegarow transmitera LIN1
//      }
//-------------------------------------------------
//UWAGA:
//RAMKA WYJSCIOWA DLA SINGLE RESPONSE:
    //PIF,NAD,PCI,SID,IDN,DTA1..DTA4,CHK

//RAMKA ZWROTNA DLA SINGLE RESPONSE:
    //ZAPIS DO ODBIORNIKA:
        //NAD,PCI,RSID,CHK                          //odpowiedz pozytywna (potwierdzenie)
        //NAD,PCI,0x7F,SID,ERR,EPar1,Epar2,CHK      //odpowiedz negatywna (sygnalizacja bledu)
    //ODCZYT Z ODBIORNIKA:
        //NAD,PCI,RSID,DTA1...DTA5,CHK              //odpowiedz pozytywna (dane zwrotne)
//-------------------------------------------------
//RAMKA WYJSCIOWA DLA MULTI RESPONSE: (IDN - identyfikator paczki danych)
    //TFF:
        //PIF,NAD,PCI,SID,IDN,STARTL,STARTH,STOPL,STOPH,CHK
    //TCF:
        //ZAPIS DO ODBIORNIKA:
            //Gdy IND>=START i IND<=STOP
                //PIF,NAD,PCI,SID,INDL,INDH,DTA1..DTA4,CHK
            //Gdy IND>STOP
                //PIF,NAD,PCI,SID,INDL,INDH,TCHK,CHK    //suma kontrolna danych z zakresu <START,STOP>
        //ODCZYT Z ODBIORNIKA:
        //PIF,NAD,PCI,SID,INDL,INDH,CHK

//RAMKA ZWROTNA DLA MULTI RESPONSE:
    //ZAPIS DO ODBIORNIKA:
        //TFF,TCF:
            //NAD,PCI,RSID,CHK                          //odpowiedz pozytywna (potwierdzenie kolejnej ramki)
            //NAD,PCI,0x7F,SID,ERR,EPar1,Epar2,CHK      //odpowiedz negatywna (sygnalizacja bledu)
    //ODCZYT Z ODBIORNIKA:
        //TFF:
            //NAD,PCI,RSID,CHK                          //odpowiedz pozytywna (potwierdzenie kolejnej ramki)
        //TCF:
            //Gdy IND>=START i IND<=STOP
                //NAD,PCI,RSID,DTA1...DTA5,CHK          //odpowiedz pozytywna (dane zwrotne)
            //Gdy IND>STOP
                //NAD,PCI,RSID,IDN,TCHK,CHK             //odpowiedz pozytywna (suma kontrolna danych z zakresu <START,STOP>)
/*---------------------------------------------------------------------------------*/
//#include <pic18.h>
#include <xc.h>
#include "global.h"			//parametry globalne
#include "main.tp.h"

#if (_EXT_BOARD>=2)                    //aktywowany modul LIN1SLAVE?
#include "LIN1Slave.h"
#include "amgs_rs9b.h"			//parametry lokalne
#include "LIN1SInterpreter.h"   //nnn

extern volatile MGSRSData MGSD;
extern volatile bit MGSDf_NO_ODB;
extern volatile DataKFN KFN;

volatile static unsigned char dly;
volatile FRAME_LIN1 FTL1,FRL1;


//--------------------------------------------------------------------
//zmienne modyfikowane w przerwaniu i programie glownym
volatile UARTData1 UAD1t;
volatile unsigned char bufTUART1[_NRS_TBF1]; //bufor roboczy modulu UART
volatile unsigned char bufRUART1[_NRS_RBF1]; //bufor roboczy modulu UART
volatile unsigned char tbuf[_NRS_TBF2]={0};
//volatile unsigned char pbufFRUART1[8];
volatile near unsigned char RSerial1;
volatile LIN_STAT1 LIN1;
volatile LIN_ERROR_FLAGS1 LIN1ERR;
volatile CMSG_PAR CMSGPar;
volatile CMSG_PAR2 CMSGDta;
extern volatile unsigned char aa,bb,cc,dd,ee,ff;
//extern unsigned char tbuf[];
extern volatile DataPFN PFN;

/*---------------------------------------------------------------------------------*/
#define _MY_NAD1        PFN.ALIN                    //adres wlasny odbiornika
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
/*-----------------------------PUBLIC DATA-----------------------------------------*/
//Obsluga kolejnych komend dla SINGLE FRAME
//Zwrocone 1 wskazuje na zakonczenie procesu, 0 wskazuje na trwanie procesu
//UWAGA maksymalny czas zajetosci zdefiniowany parametrem _LN1_ADTA
//-------------------------------
//MASTER zada zapisu danych lub wykonania komendy
bit LIN1SInterpreterSF_WR(void)
{
    switch(FRL1.SID)                //SID
    {
        case 0xB2:
        {
            FTL1f_NoResp=0;
            FTL1.NAD=_MY_NAD1;                   //Adres odbiornika
            FTL1.PCI_TYPE=_PCI_TSF1;             //Rodzaj ramki
            if(!FRL1.NAD) FTL1f_NoResp=1;         //dla adresu zerowego nie generuj odpowiedzi
            //zapis danych do pamieci, wykonanie komendy - odebrane z MASTER (LIN1SInterpreter.c)
            switch(LIN1SingleResponse_WR())
            {
                case _LIN1_IFINAL:   //wszystko ok
                {
                    //potwierdzenie odbioru danych, wykonania komendy
                    FTL1.PCI_LENGTH=0x01;                //Rozmiar ramki(RSID+liczba bajtow danych bez CHK)
                    FTL1.WRD0.byte0=0xB2+0x40;           //RSID=SID+0x40
                    break;
                }
                case _LIN1_NORESP:   //decyzja o braku odpowiedzi
                {
                    FTL1f_NoResp=1;
                    return 1;
                }
                case _LIN1_UNCCOM:  //nieznana komenda
                {
                    //negatywna odpowiedz ukladu
                    if(!FRL1f_RSFrame)
                    {
                        LIN1NegativeResponse(_LIN1_UNCCOM,1,FRL1.IDN,0);
                    }
                    else
                    {
                        LIN1NegativeResponse(_LIN1_UNCCOM,1,FRL1.RS_IDN,0);
                    }
                    return 1;
                }
                case _LIN1_INVDTA:  //nieprawidlowe dane
                {
                    //negatywna odpowiedz ukladu
                    if(!FRL1f_RSFrame)
                    {
                        LIN1NegativeResponse(_LIN1_INVDTA,1,FRL1.IDN,0);
                    }
                    else
                    {
                        LIN1NegativeResponse(_LIN1_INVDTA,1,FRL1.RS_IDN,0);
                    }
                    return 1;
                }
                case _LIN1_ERRODP:
                {
                    //negatywna odpowiedz ukladu
                    LIN1NegativeResponse(_LIN1_ERRODP,0,0,0); //brak lub bledna odpowiedz odbiornika innej sieci (np UNI-02)
                    return 1;
                }
                case _LIN1_IBUSY:    //trwa proces przetwarzania danych - poczekaj
                {
                    return 0;        //czekaj na zakonczenie przetwarzania danych
                }
            }
            break;
        }
        default:        //nie odpowiadaj gdy niewlasciwe SID
        {
            LIN1ERR.USID=1;
            FTL1f_NoResp=1;
            return 1;
        }
    }
    return 1;
}
//-------------------------------
//MASTER zada odeslania danych
bit LIN1SInterpreterSF_RD(void)
{
    switch(FRL1.SID)                //SID
    {
        case 0xB2:
        {
            //if(!FRL1.NAD&&FRL1.IDN!=0x01)        //adres zerowy akceptowany wylacznie dla zapytania ACK
            if(!FRL1.NAD&&!(FRL1.IDN==_CLIN_RRD_ACK||FRL1.IDN==_CLIN_RRD_LIFADDR))        //adres zerowy akceptowany wylacznie dla zapytania ACK    //nnn
            {
                FTL1f_NoResp=1;         //dla adresu zerowego i idn!=ACK nie generuj odpowiedzi
                return 1;
            }
            FTL1f_NoResp=0;
            FTL1.NAD=_MY_NAD1;                   //Adres odbiornika
            FTL1.PCI_TYPE=_PCI_TSF1;             //Rodzaj ramki
            //przygotowanie danych do wysylki do MASTER (LIN1SInterpreter.c)
            switch(LIN1SingleResponse_RD())
            {
                case _LIN1_IFINAL:   //wszystko ok, gotow do wysylki danych
                {
                    break;
                }
                case _LIN1_NORESP:   //decyzja o braku odpowiedzi
                {
                    FTL1f_NoResp=1;
                    return 1;
                }
                case _LIN1_UNCCOM:  //nieznana komenda
                {
                    //negatywna odpowiedz ukladu
                    if(!FRL1f_RSFrame)
                    {
                        LIN1NegativeResponse(_LIN1_UNCCOM,1,FRL1.IDN,0);
                    }
                    else
                    {
                        LIN1NegativeResponse(_LIN1_UNCCOM,1,FRL1.RS_IDN,0);
                    }                        
                    return 1;
                }
                case _LIN1_INVDTA:  //nieprawidlowe dane
                {
                    //negatywna odpowiedz ukladu
                    if(!FRL1f_RSFrame)
                    {                    
                        LIN1NegativeResponse(_LIN1_INVDTA,1,FRL1.IDN,0);
                    }
                    else
                    {
                        LIN1NegativeResponse(_LIN1_INVDTA,1,FRL1.RS_IDN,0);                        
                    }
                    return 1;
                }
                case _LIN1_ERRODP:
                {
                    //negatywna odpowiedz ukladu
                    LIN1NegativeResponse(_LIN1_ERRODP,0,0,0); //brak lub bledna odpowiedz odbiornika innej sieci (np UNI-02)
                    return 1;
                }
                case _LIN1_IBUSY:    //trwa proces przetwarzania danych - poczekaj
                {
                    return 0;        //czekaj na zakonczenie przetwarzania danych
                }
            }
            break;
        }
        default:        //nie odpowiadaj gdy niewlasciwe SID
        {
            LIN1ERR.USID=1;
            FTL1f_NoResp=1;
            return 1;
        }
    }
    return 1;
}
//---------------------------------------------------------------------------
//Obsluga kolejnych komend dla CONTINIOUS FRAME
//Zwrocone 1 wskazuje na zakonczenie procesu, 0 wskazuje na trwanie procesu
//UWAGA maksymalny czas zajetosci zdefiniowany parametrem _LN1_ADTA
//-------------------------------
//FIRST FRAME Procedura zwiazana z inicjacja danymch tablicowymi do zapisu/odczytu
bit LIN1SInterpreterFF(void)
{
    volatile static unsigned char krok=0;
    volatile unsigned char stat;
    switch(FRL1.SID)                //SID
    {
        case 0xB2:
        {
            if(!FRL1.NAD)                           //dla adresu zerowego transfer tablicowy nie obslugiwany
            {
                krok=0;
                FTL1f_NoResp=1;                      //dla adresu zerowego nie generuj odpowiedzi
                return 1;
            }
            switch(krok)
            {
                case 0:
                {
                    //odpowiedz ukladu
                    FTL1f_NoResp=0;
                    FTL1.NAD=_MY_NAD1;                      //Adres odbiornika
                    FTL1.PCI_TYPE=_PCI_TFF1;                //Rodzaj ramki
                    //FTL1.PCI_LENGTH=0x01;                   //Rozmiar ramki(RSID+liczba bajtow danych bez CHK)  
                    FTL1.PCI_LENGTH=0x02;                   //Rozmiar ramki(RSID+liczba bajtow danych bez CHK)            
                    FTL1.WRD0.byte0=0xB2+0x40;              //RSID=SID+0x40

                    //parametry startowe dla kolejnych ramek CF
                    FRL1.FFPAR.war=FRL1.WRD1.war;           //dwubajtowy parametr wywolania przy multikomendzie (zapamietanie po weryfikacji)
                    FRL1.START.war=0;                       //pczatkowy element tabeli (zawsze 0)
                    FRL1.STOP.war =FRL1.WRD2.war;           //koncowy element tabeli
                    FRL1.PCI_FCOUNT.war=FRL1.START.war;     //poczatkowy indeks tabeli
                    FTL1.WRD0.byte1=(FRL1.STOP.war)?((unsigned char)FRL1.STOP.war+1):0;       //zwroc odebrany rozmiar tabeli danych     
                    krok=1;
                }
                case 1:
                {
                    //przygotowanie bufora do zapisu/odczytu danych odebranych z MASTER (LIN1SInterpreter.c)
                    if((FRL1.PIF.war&0b00111111)==_PIF_WRITE) stat=LIN1StartMultiResponse_WR(); //MASTER zada zapisu nadeslanych danych
                    else
                    if((FRL1.PIF.war&0b00111111)==_PIF_READ)    //MASTER zada odeslania danych
                    {
                        stat=LIN1StartMultiResponse_RD();  
                    }
                    else                                                    //nieznane PIF
                    {
                        krok=0;
                        LIN1ERR.UPIF=1;
                        FTL1f_NoResp=1;
                        return 1;
                    }
                    //czekaj na przygotowanie danych do wysylki
                    switch(stat)
                    {
                        case _LIN1_IFINAL:
                        {
                            if(!FTL1.WRD0.byte1)    //nie nadeslano wymaganej liczby bajtow
                            {
                                FTL1.WRD0.byte1=(FRL1.STOP.war)?((unsigned char)FRL1.STOP.war+1):0;   //zwroc informacje o odsylanej liczbie bajtow
                            }
                            
                            if((FRL1.IDN==0x18)&&(FTL1.WRD0.byte1>4))
                            {
                                Nop();
                            }
                            krok=0;
                            return 1;
                        }
                        case _LIN1_UNCCOM:  //nieznana komenda
                        {
                            krok=0;
                            //negatywna odpowiedz ukladu
                            if(!FRL1f_RSFrame)
                            {                    
                                LIN1NegativeResponse(_LIN1_UNCCOM,1,FRL1.IDN,0);
                            }
                            else
                            {
                                LIN1NegativeResponse(_LIN1_UNCCOM,1,FRL1.RS_IDN,0);                        
                            }
                            return 1;
                        }
                        case _LIN1_INVPAR:  //nieprawidlowe dane
                        {
                            krok=0;
                            //negatywna odpowiedz ukladu
                            if(!FRL1f_RSFrame)
                            {                    
                                LIN1NegativeResponse(_LIN1_INVPAR,1,FRL1.IDN,0); //niewlasciwa wartosc parametru
                            }
                            else
                            {
                                LIN1NegativeResponse(_LIN1_INVPAR,1,FRL1.RS_IDN,0); //niewlasciwa wartosc parametru                        
                            }
                            return 1;
                        }
                        case _LIN1_ERRODP:
                        {
                            krok=0;
                            //negatywna odpowiedz ukladu
                            LIN1NegativeResponse(_LIN1_ERRODP,0,0,0); //brak lub bledna odpowiedz odbiornika innej sieci (np UNI-02)
                            return 1;
                        }
                        case _LIN1_NORESP:   //decyzja o braku odpowiedzi
                        {
                            krok=0;
                            FTL1f_NoResp=1;
                            return 1;
                        }                        
                        case _LIN1_IBUSY:    //trwa proces przetwarzania danych - poczekaj
                        {
                            return 0;        //czekaj na zakonczenie przetwarzania danych
                        }
                        default:             //niesprecyzowany stan przetwarzania danych
                        {
                            krok=0;
                            //negatywna odpowiedz ukladu
                            LIN1NegativeResponse(_LIN1_ERRODP,0,0,0); //brak lub bledna odpowiedz odbiornika innej sieci (np UNI-02)
                            return 1;
                        }
                    }                    
                }
            }
            break;
        }
        default:        //nie odpowiadaj gdy niewlasciwe SID
        {
            krok=0;
            LIN1ERR.USID=1;
            FTL1f_NoResp=1;
            return 1;
        }
    }
    krok=0;
    return 1;
}
//-------------------------------
//NEXT FRAME Rejestracja danych tablicowych (MASTER zada zapisu nadeslanych danych)
bit LIN1SInterpreterCF_WR(void)
{
    unsigned int i;
    switch(FRL1.CSID)                //SID
    {
        case 0xB2:
        {
            if(!FRL1.NAD)                           //dla adresu zerowego transfer tablicowy nie obslugiwany
            {
                FTL1f_NoResp=1;                      //dla adresu zerowego nie generuj odpowiedzi
                return 1;
            }
            //przygotowanie ew. odpowiedzi
            FTL1f_NoResp=0;
            FTL1.NAD=_MY_NAD1;                      //Adres odbiornika
            FTL1.PCI_TYPE=_PCI_TCF1;                //Rodzaj ramki
            FTL1.PCI_LENGTH=0x01;                   //Rozmiar ramki(RSID+liczba bajtow danych bez CHK)
            FTL1.WRD0.byte0=0xB2+0x40;              //RSID=SID+0x40

            FRL1.PCI_FCOUNT.war=FRL1.WRD0.war;      //odczytaj poczatkowy indeks zapisu danych
            if(FRL1.PCI_FCOUNT.war>FRL1.STOP.war)   //MASTER przysyla sume kontrolna dla nadeslanych wczesniej danych
            {
                if((FRL1.WRD1.byte0==FRL1.IDN)&&(FRL1.WRD1.byte1==CalcTabCHK(FRL1.tab,FRL1.START.war,FRL1.STOP.war)))
                {
                    switch(LIN1FinishMultiResponse_WR())            //aktualizuj dane (LIN1SInterpreter.c)
                    {
                        case _LIN1_NORESP:   //decyzja o braku odpowiedzi
                        {
                            FTL1f_NoResp=1;
                            return 1;
                        }
                        case _LIN1_UNCCOM:  //nieznana komenda
                        {
                            //negatywna odpowiedz ukladu
                            if(!FRL1f_RSFrame)
                            {                            
                                LIN1NegativeResponse(_LIN1_UNCCOM,1,FRL1.IDN,0);
                            }
                            else
                            {                            
                                LIN1NegativeResponse(_LIN1_UNCCOM,1,FRL1.RS_IDN,0);
                            }
                            return 1;
                        }
                        case _LIN1_INVDTA:  //nieprawidlowe dane
                        {
                            //negatywna odpowiedz ukladu
                            if(!FRL1f_RSFrame)
                            {                            
                                LIN1NegativeResponse(_LIN1_INVDTA,1,FRL1.IDN,0);
                            }
                            else
                            {
                                LIN1NegativeResponse(_LIN1_INVDTA,1,FRL1.RS_IDN,0);                                
                            }
                            return 1;
                        }
                        case _LIN1_IBUSY:    //trwa proces przetwarzania danych - poczekaj
                        {
                            return 0;        //czekaj na zakonczenie przetwarzania danych
                        }
                    }
                }
                else        
                {
                    //negatywna odpowiedz ukladu
                    LIN1NegativeResponse(_LIN1_INVCHK,0,0,0); //niewlasciwa wartosc IDN lub CHK
                }
                return 1;
            }
            for(i=0;i<4;i++)
            {
                if((FRL1.PCI_FCOUNT.war+i>FRL1.STOP.war)||(FRL1.PCI_FCOUNT.war+i>=FRL1.imax))
                {
                    break;
                }
                switch(i)
                {
                    case 0:
                        FRL1.tab[FRL1.PCI_FCOUNT.war]=FRL1.WRD1.byte0;
                    break;
                    case 1:
                        FRL1.tab[FRL1.PCI_FCOUNT.war+1]=FRL1.WRD1.byte1;
                    break;
                    case 2:
                        FRL1.tab[FRL1.PCI_FCOUNT.war+2]=FRL1.WRD2.byte0;
                    break;
                    case 3:
                        FRL1.tab[FRL1.PCI_FCOUNT.war+3]=FRL1.WRD2.byte1;
                    break;
                }
            }
            break;
        }
        default:    //nie odpowiadaj gdy niewlasciwe SID
        {
            LIN1ERR.USID=1;
            FTL1f_NoResp=1;
            return 1;
        }
    }
    return 1;
}
//-------------------------------
//NEXT FRAME Wysylka danych tablicowych (MASTER zada odeslania danych)
bit LIN1SInterpreterCF_RD(void)
{
    switch(FRL1.CSID)                //SID
    {
        case 0xB2:
        {
            if(!FRL1.NAD)                           //dla adresu zerowego transfer tablicowy nie obslugiwany
            {
                FTL1f_NoResp=1;                      //dla adresu zerowego nie generuj odpowiedzi
                return 1;
            }
            //przygotowanie ew. odpowiedzi
            FTL1f_NoResp=0;
            FTL1.NAD=_MY_NAD1;                      //Adres odbiornika
            FTL1.PCI_TYPE=_PCI_TCF1;                //Rodzaj ramki
            FRL1.PCI_FCOUNT.war=FRL1.WRD0.war;      //odczytaj indeks danych do wysylki
            if(FRL1.PCI_FCOUNT.war<=FRL1.STOP.war)
            {
                FTL1.PCI_LENGTH=0x06;                   //Rozmiar ramki(liczba bajtow danych bez CHK)
                FRL1.PCI_FCOUNT.war=FRL1.WRD0.war;      //odczytaj poczatkowy indeks zapisu danych
                if((FRL1.STOP.war-FRL1.PCI_FCOUNT.war)<6)
                {
                    FTL1.PCI_LENGTH=(FRL1.STOP.war-FRL1.PCI_FCOUNT.war)+1;                  //Rozmiar ramki(liczba bajtow danych bez CHK)
                }
                if((FRL1.PCI_FCOUNT.war<=FRL1.STOP.war)&&(FRL1.PCI_FCOUNT.war<FRL1.imax))
                FTL1.WRD0.byte0=FRL1.tab[FRL1.PCI_FCOUNT.war];
                else FTL1.WRD0.byte0=0xFF;
                if((FRL1.PCI_FCOUNT.war+1<=FRL1.STOP.war)&&(FRL1.PCI_FCOUNT.war+1<FRL1.imax))
                FTL1.WRD0.byte1=FRL1.tab[FRL1.PCI_FCOUNT.war+1];
                else FTL1.WRD0.byte1=0xFF;
                if((FRL1.PCI_FCOUNT.war+2<=FRL1.STOP.war)&&(FRL1.PCI_FCOUNT.war+2<FRL1.imax))
                FTL1.WRD1.byte0=FRL1.tab[FRL1.PCI_FCOUNT.war+2];
                else FTL1.WRD1.byte0=0xFF;
                if((FRL1.PCI_FCOUNT.war+3<=FRL1.STOP.war)&&(FRL1.PCI_FCOUNT.war+3<FRL1.imax))
                FTL1.WRD1.byte1=FRL1.tab[FRL1.PCI_FCOUNT.war+3];
                else FTL1.WRD1.byte1=0xFF;
                if((FRL1.PCI_FCOUNT.war+4<=FRL1.STOP.war)&&(FRL1.PCI_FCOUNT.war+4<FRL1.imax))
                FTL1.WRD2.byte0=FRL1.tab[FRL1.PCI_FCOUNT.war+4];
                else FTL1.WRD2.byte0=0xFF;
                if((FRL1.PCI_FCOUNT.war+5<=FRL1.STOP.war)&&(FRL1.PCI_FCOUNT.war+5<FRL1.imax))
                FTL1.WRD2.byte1=FRL1.tab[FRL1.PCI_FCOUNT.war+5];
                else FTL1.WRD2.byte1=0xFF;
            }
            else        //Wyslij do MASTER sume kontrolna dla przeslanych wczesniej danych
            {
                FTL1.PCI_LENGTH=0x03;                                                               //Rozmiar ramki(RSID+liczba bajtow danych bez CHK)
                FTL1.WRD0.byte0=0xB2+0x40;                                                          //RSID=SID+0x40
                if(!FRL1f_RSFrame)
                {                
                    FTL1.WRD0.byte1=FRL1.IDN;                                                       //Identyfikator tablicy danych
                }
                else
                {
                    FTL1.WRD0.byte1=FRL1.RS_IDN;                                                    //Identyfikator tablicy danych
                }
                FTL1.WRD1.byte0=CalcTabCHK(FRL1.tab,FRL1.START.war,FRL1.STOP.war);                  //CHK dla tablicy danych w zakresie start-stop
            }
            break;
        }
        default:    //nie odpowiadaj gdy niewlasciwe SID
        {
            LIN1ERR.USID=1;
            FTL1f_NoResp=1;
            return 1;
        }
    }
    return 1;
}
//---------------------------------------------------------------------------------
//Przygotowanie odpowiedzi negatywnej (RSID=0x7F)
void LIN1NegativeResponse(unsigned char kod, unsigned char npar, unsigned char par1, unsigned char par2)
{
    switch(npar)
    {
        case 0:
        {
            FTL1.PCI_LENGTH=0x03;                   //Rozmiar ramki(RSID+liczba bajtow danych bez CHK)
            FTL1.WRD0.byte0=0x7F;                   //NEG RESP
            FTL1.WRD0.byte1=0xB2;                   //SID
            FTL1.WRD1.byte0=kod;                    //kod bledu
            break;
        }
        case 1:
        {
            FTL1.PCI_LENGTH=0x04;                   //Rozmiar ramki(RSID+liczba bajtow danych bez CHK)
            FTL1.WRD0.byte0=0x7F;                   //NEG RESP
            FTL1.WRD0.byte1=0xB2;                   //SID
            FTL1.WRD1.byte0=kod;                    //kod bledu
            FTL1.WRD1.byte1=par1;                   //parametr pierwszy
            break;
        }
        case 2:
        {
            FTL1.PCI_LENGTH=0x05;                   //Rozmiar ramki(RSID+liczba bajtow danych bez CHK)
            FTL1.WRD0.byte0=0x7F;                   //NEG RESP
            FTL1.WRD0.byte1=0xB2;                   //SID
            FTL1.WRD1.byte0=kod;                    //kod bledu
            FTL1.WRD1.byte1=par1;                   //parametr pierwszy
            FTL1.WRD2.byte0=par2;                   //parametr drugi
            break;
        }
        default:
        {
            FTL1.PCI_LENGTH=0x03;                   //Rozmiar ramki(RSID+liczba bajtow danych bez CHK)
            FTL1.WRD0.byte0=0x7F;                   //NEG RESP
            FTL1.WRD0.byte1=0xB2;                   //SID
            FTL1.WRD1.byte0=kod;                    //kod bledu
        }
    }
}
//---------------------------------------------------------------------------------
//Reakcja na otrzymana paczke danych, decyzja odpowiedzi
//Gdzie zwrocone: 0-oczekiwanie na zakonczenie procesu, 1-gotowa odpowiedz, 2-brak odpowiedzi
unsigned char SetLIN1SData(void)
{
//----------------------------------------------------------------------------
//Analiza odebranej i wstepnie zweryfikowanej paczki, reakcja.
//GDZIE:
//      FRL1.PIF.war                                            //protected idetifier
//      FRL1.PCI_TYPE                                           //Rodzaj ramki
//      FRL1.PCI_LENGTH                                         //Rozmiar ramki(SID+liczba bajtow danych+CHK)
//      FRL1.WRD0.byte0                                         //SID
//      FRL1.WRD0.byte1                                         //DTA1
//      FRL1.WRD1.byte0                                         //DTA2
//      FRL1.WRD1.byte1                                         //DTA3
//      FRL1.WRD2.byte0                                         //DTA4
//      FRL1.WRD2.byte1                                         //DTA5
//      FRL1.SID
//      FRL1.IDN
        switch(FRL1.PIF.war&0b00111111)
        {
            case _PIF_WRITE:                      //PIF (write param, execute)
            {
                switch(FRL1.PCI_TYPE)       //PCI
                {
                    case _PCI_TESF1:
                    case _PCI_TSF1:
                    {
                        //Przetwarzanie danych SF
                        if(!LIN1SInterpreterSF_WR()) return 0;                              //czekaj az dane zostana przetworzone
                        break;
                    }
                    case _PCI_TEFF1:
                    case _PCI_TFF1:
                    {
                        //Przetwarzanie danych FF
                        if(!LIN1SInterpreterFF()) return 0;                              //czekaj az dane zostana przetworzone
                        break;
                    }
                    case _PCI_TECF1:
                    case _PCI_TCF1:
                    {
                        //Przetwarzanie danych CF
                        if(!LIN1SInterpreterCF_WR()) return 0;                              //czekaj az dane zostana przetworzone
                        break;
                    }
                    default:
                    {
                        LIN1ERR.PCI_TYPE=1;
                        FTL1f_NoResp=1;
                        break;
                    }
                }
                break;
            }
            case _PIF_READ:                      //PIF (read param)
            {
                switch(FRL1.PCI_TYPE)       //PCI
                {
                    case _PCI_TESF1:
                    case _PCI_TSF1:
                    {
                        //Przetwarzanie danych SF
                        if(!LIN1SInterpreterSF_RD()) return 0;                              //czekaj az dane zostana przetworzone
                        break;
                    }
                    case _PCI_TEFF1:
                    case _PCI_TFF1:
                    {
                        //Przetwarzanie danych FF
                        if(!LIN1SInterpreterFF()) return 0;                              //czekaj az dane zostana przetworzone
                        break;
                    }
                    case _PCI_TECF1:
                    case _PCI_TCF1:
                    {
                        //Przetwarzanie danych CF
                        if(!LIN1SInterpreterCF_RD()) return 0;                              //czekaj az dane zostana przetworzone
                        break;
                    }
                    default:
                    {
                        LIN1ERR.PCI_TYPE=1;
                        FTL1f_NoResp=1;
                        break;
                    }
                }
                break;
            }
            default:
            {
                LIN1ERR.UPIF=1;
                FTL1f_NoResp=1;
                break;
            }
        }
//----------------------------------------------------------------------------
//Odpowiedz (rezygnacja z odpowiedzi za pomoca FTL1f_NoResp=1)
        if(FTL1f_NoResp) return 2;                               //brak odpowiedzi
        else return 1;                                          //paczka odpowiedzi gotowa
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
/*-----------------------------ROTECTED DATA-----------------------------------------*/
//Inicjalizacja UART
void InitUart1(void)
{
    //unsigned char rc;
    //6 wyjscie portu C ustawic jako wyjscie (0)
    //7 wyjscie portu C ustawic jako wejscie (1)
    U1OD=0;                  //wyjscie z podparciem
    TXSTA1bits.CSRC=0;			//asynch mode
    RCSTA1bits.RX9=0;			//odbior 8 bitowy
    BAUDCON1bits.BRG16=_BRG16;          //rozdzielczosc SPBRG1
#if _BRG16==0
    SPBRG1=0x00FF&_SPBRG;                      //baud rate
    SPBRGH1=0;
#else
    SPBRG1=0x00FF&_SPBRG;
    SPBRGH1=(0xFF00&_SPBRG)>>8;
#endif
    TXSTA1bits.SYNC=_SYNC;		//tryb asynchroniczny/synchroniczny
    TXSTA1bits.BRGH=_BRGH;		//predkosc generatora Baud Rate
    RCSTA1bits.ADDEN=0;                 //bez adresowania
    TXSTA1bits.TX9=0;			//nadawanie 8 bitowe
    IPR1bits.RC1IP=1;			//wysoki priorytet przerwania
    IPR1bits.TX1IP=1;			//wysoki priorytet przerwania
    PIE1bits.RC1IE=0;			//nieaktywne przerwanie dla odbioru
    PIE1bits.TX1IE=0;			//nieaktywne przerwanie dla odbioru

    BAUDCON1bits.TXCKP=0;
    TXSTA1bits.TXEN=0;			//deaktywacja modulu transmisji
    RCSTA1bits.SPEN=0;			//deaktywacja portu szeregowego
    RCSTA1bits.CREN=0;			//deaktywacja odbioru

    UAD1tf_RFData=0;
    UAD1tf_RFBrSn=0;
    UAD1tf_RFEnd=0;
    UAD1tf_RFExt=0;

    UAD1tf_TFData=0;
    UAD1tf_TFEnd=0;

    UAD1t.Rindeks=0;
    UAD1t.Tindeks=0;
    UAD1t.RNData=0;
    UAD1t.TNData=0;
    UAD1t.tmFR=0;
    UAD1t.tmB=0;   

    LIN1f_Busy=0;
    LIN1f_FAdta=0;
    LIN1f_FResp=0;
    LIN1f_FTrans=0;
    LIN1f_NDTA=0;
    LIN1.State=0;
    LIN1.tmAd=0;
    LIN1.tmZw=0;    
    LIN1f_FIN=0;
    LIN1f_ERRR=0;
    LIN1f_ACK=0;
    LIN1f_SCS=0;
    LIN1f_CHK=0;
    LIN1ERR.wrd=0;

    KFN.flag3=0;
    KFN.flag4=0;    
    KFN.flag5=0;  
    KFN.flag6=0;  
    KFN.flag7=0;   
    KFN.flag8=0;  
    KFN.flag10=0;  
    KFN.flag13=0;    
    
    RSerial1=RCREG1;
    ResetRC1TX1();                      //wylacz nadajnik/odbiornik
    LIN1f_NDTA=1;                        //aktywacja oczekiwania na paczke danych
    LIN2SClrMasterIDO();                 //zeruj identyfikator nadajnika    
}

//---------------------------------------------------------------------------------
//Wlacz caly 1 port szeregowy
void Serial1ON(void)
{
    RCSTA1bits.SPEN=1;			//aktywacja portu szeregowego
}
//Wylacz caly 1 port szeregowy
void Serial1OFF(void)
{
    RCSTA1bits.SPEN=0;			//deaktywacja portu szeregowego
}
//Wylacz modul 1  odbioru
void ReaderRX1ON(void)
{
    UAD1tf_RFBrSn = 0;                    //restartuj znacznik oczekiwania na BREAk SYNCH
#if _RBREAK_SYNCH1==2
    BAUDCON1bits.WUE=1;                 //oczekiwanie na BREAK
    BAUDCON1bits.ABDEN=1;               //AUTOBAUD
    //BAUDCON1bits.BRG16=_BRG16;
#else
    #if _RBREAK_SYNCH1==1
        BAUDCON1bits.WUE=1;             //oczekiwanie na BREAK
    #endif
#endif
    RCSTA1bits.CREN = 1;		//aktywacja portu szeregowego
    PIE1bits.RC1IE=1;                   //aktywacja przerwania odbioru
}
//Wlacz modul 1  odbioru
void ReaderRX1OFF(void)
{
    RCSTA1bits.CREN = 0;		//deaktywacja portu szeregowego
    PIE1bits.RC1IE=0;                   //deaktywacja przerwania odbioru
}
//---------------------------------------------------------------------------------
//Wlacz modul 1 transmisji
void TransmiterTX1ON(void)
{
    TXSTA1bits.TXEN=1;			//aktywacja modulu transmisji
    PIE1bits.TX1IE=1;
}
//Wylacz modul 1 transmisji
void TransmiterTX1OFF(void)
{
    TXSTA1bits.TXEN=0;			//deaktywacja modulu transmisji
    PIE1bits.TX1IE=0;                             //deaktywacja przerwania transmisji
}
//---------------------------------------------------------------------------------
//Aktywuj tryb transmisji BREAK z  synchronizacja
void SetTX1BreakSynch(void)
{
    TXSTA1bits.SENDB=1;                 //pierwszy bajt BREAK 0x000
    TXREG1=0x00;                         //BREAK - wartosc ignorowana
    TXREG1=0x55;                         //bajt synchronizacji 0x55 (na optrzeby autobaudrate)
}
//Aktywuj tryb transmisji BREAK bez bajtu synchronizacji
void SetTX1Break(void)
{
    TXSTA1bits.SENDB=1;                 //pierwszy bajt BREAK 0x000
    TXREG1=0x00;                         //BREAK - wartosc ignorowana
}
//---------------------------------------------------------------------------------
//Przelacz uklad LIN w tryb Odbioru
void SetLIN1_TOFF_Mode(void)
{
    //RC2=0;                              //CS=0
    //LATC2=0;
    _CS_LIN1=0;
    dly=_CS1_DELAY;
    while(--dly);
    //ustaw TXD=0
    //RC6=0;
    //LATC6=0;                            //TXD=0
    _TXD1=0;
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    //Przejscie do TOFF
    //RC2=1;                              //CS=1
    _CS_LIN1=1;
    dly=_CS1_DELAY;
    while(--dly);
    LIN1.State=_TOFF_MODE1;
}
//Przelacz uklad LIN w tryb wylaczenia
void SetLIN1_PWRD_Mode(void)
{
    //RC6=0;
    //LATC6=0;                            //TXD=0
    _TXD1=0;
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    //Przejscie do TOFF
    //RC2=1;                              //CS=1
    _CS_LIN1=1;
    dly=_CS1_DELAY;
    while(--dly);
    //przejscie do POWER DOWN
    //RC2=0;                              //CS=0
    //LATC2=0;
    _CS_LIN1=0;
    dly=_CS1_DELAY;
    while(--dly);
    LIN1.State=_PWRD_MODE1;
}
//Przelacz uklad LIN w tryb Nadawania
void SetLIN1_OPER_Mode(void)
{
    //RC6=1;                               //TXD=1
    _TXD1=1;
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();

    //Przejscie do Operation mode
    //RC2=1;                              //CS=1
    _CS_LIN1=1;
    dly=_CS1_DELAY;
    while(--dly);
    LIN1.State=_OPER_MODE1;
}
//------------------------------------------------------------------------------------
//---------------------------------------------------------------------
//Wyzwolenie transmisji na linii LIN1
bit StartLIN1TR(void)
{
    if(!TXSTA1bits.TRMT||!BAUDCON1bits.RCIDL) return 0;       //PIE1bits.TX1IE||trwa transmisja lub odbior danych?
    if(UAD1t.TNData>_NRS_TBF1) UAD1t.TNData=_NRS_TBF1;          //kontrola rozmiaru tablicy

    UAD1t.Tindeks=0;                                         //indeks = 0 - pierwszy bajt oznaczony jako adres
    UAD1tf_TFEnd=0;                                           //wyzeruj znacznik konca nadawania ramki
    //--------------------
    //konfiguracja LIN(MCP2003) do transmisji
    ReaderRX1OFF();                                         //wylacz odbiornik
    Serial1OFF();                                           //wylacz modul serial1
    SetLIN1_OPER_Mode();                                    //konfiguruj uklad LIN w tryb nadawania
    Serial1ON();                                            //wlacz modul serial1
    //--------------------
    TransmiterTX1ON();                                      //uruchom transmisje
    UAD1t.tmFR=0;                                            //restart timera ramki
    //--------------------
    return 1;
}
//Deaktywacja modulu odbioru danych
//---------------------------------------------------------------------
void StopLIN1TR(void)
{
    TransmiterTX1OFF();                                     //zatrzymaj transmisje
   //--------------------
    //konfiguracja LIN(MCP2003) do transmisji

    Serial1OFF();                                           //wylacz modul serial1**********

    //dly=_CS1_DELAY;
    //while(--dly);
    //SetLIN1_TOFF_Mode();                                    //konfiguruj uklad LIN1 w tryb odbioru
    Serial1ON();                                            //wlacz modul serial1*********
    UAD1tf_TFData = 0;                                        //usun znacznik transmisji danych
    UAD1tf_TFEnd = 0;                                         //wyzeruj znacznik konca odbioru
    UAD1t.tmFR=0;
}
//------------------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//Zakonczenie odbioru linii LIN1
void StopLIN1RC(void)
{
    ReaderRX1OFF();                                          //zatrzymaj odbior
    UAD1tf_RFData = 0;                                         //usun znacznik odbioru danych
    UAD1tf_RFEnd = 0;                                          //wyzeruj znacznik konca odbioru
    UAD1tf_RFExt = 0;
    UAD1t.tmFR=0;
    UAD1t.tmB=0;
}
//-------------------------------------------------------------------
//Zakonczenie transmisji/odbioru
void ResetRC1TX1(void)
{
   LIN1f_Busy=0;
   LIN1f_FResp=0;
   LIN1f_FTrans=0;
   LIN1f_FAdta=0;
   LIN1.tmAd=0;
   LIN1.tmZw=0;   
   StopLIN1RC();
   StopLIN1TR();
}
//-------------------------------------------------------------------
//Ustaw podstawowe znaczniki danych 
void LIN1RefStat(void)
{
    LIN1f_Busy=0;
    LIN1f_NDTA=0;         
    LIN1f_FIN=1;          //zakonczono odbior/transmisje
    if(LIN1ERR.wrd)
    {
        LIN1f_ERRR=1;
        LIN1f_ACK=0;
        LIN1f_SCS=0;
    }
    else
    {
        LIN1f_ERRR=0;
        if(FRL1f_NoResp)
        {
            LIN1f_ACK=0;
            LIN1f_SCS=1;     //sukces bez potwierdzenia
        }
        else
        {
            LIN1f_ACK=1;
            LIN1f_SCS=1;     //sukces z potwierdzeniem
        }
    }
}
//-----------------------------------------------------------
//Oczekiwanie na zwolnienie linii TX1
unsigned char ChkTX1Busy(void)
{
    if((!TXSTA1bits.TRMT)&&(UAD1t.tmFR>_LN1_TRMF)) //przekroczony czas aktywnosci linii TX?
    {
        return 2;
    }
    if(!TXSTA1bits.TRMT) return 1;             //transmisja trwa?
    UAD1tf_TFEnd=0;                    //zeruj znacznik zakonczenia wysylki ramki
    return 0;

}
//-----------------------------------------------------------------
//Zeruj bufor nadawania
void ClrLIN2TrnBuf(void)
{
    volatile unsigned char i;
    
    FTL1.WRD0.byte0=0;                   
    FTL1.WRD0.byte1=0;                   
    FTL1.WRD1.byte0=0;                   
    FTL1.WRD1.byte1=0;                   
    FTL1.WRD2.byte0=0;    
    FTL1.WRD2.byte1=0; 
    
    for(i=0;i<_NRS_TBF1;i++) bufTUART1[i]=0;
}
//-----------------------------------------------------------------
unsigned char ChkReceptKnf(void)        //bbb
{   
    if(!UAD1tf_RFEnd)   //odbior nie zostal zakonczony?
    {
        if(!UAD1tf_RFData)  //odbior nie zostal rozpoczety?
        {
            if(BAUDCON1bits.WUE&&BAUDCON1bits.ABDEN)
            {
                LIN1.tmZw=0;
            }            
            if(!BAUDCON1bits.WUE||!BAUDCON1bits.ABDEN)
            {
                if(LIN1.tmZw>10)
                {
                    LIN1.tmZw=0;
                    return 1;
                }
            }              
            if(!RCSTA1bits.CREN||!PIE1bits.RC1IE) return 1;
        }
        else    //odbior trwa
        {
            if(!RCSTA1bits.CREN||!PIE1bits.RC1IE) return 1;               //deaktywowany odbior RS w trakcie oczekiwania lub podczas procesu odbioru?                   
        }
    } 
    return 0;
}
//-----------------------------------------------------------------
//Odbior danych w trybie SLAVE
unsigned char LIN1SRecept(unsigned char ref)
{
    volatile static unsigned char krok;
#if _TESTCHK1!=0
    static unsigned char a;
#endif
    if(ref)
    {
        ClrLIN2TrnBuf();
        //***********************
        FRL1f_Auto=0;
        FTL1f_Auto=0;        
        //***********************        
        krok=0;
        return 0;
    }
    switch(krok)
    {
        case 0:
        {
            UAD1t.RNData = 9;                                            //startowy, maksymalny rozmiar ramki
            UAD1t.Rindeks = 0;                                           //wyzeruj indeks
            UAD1tf_RFData = 0;                                            //usun znacznik odbioru danych
            UAD1tf_RFEnd = 0;                                             //wyzeruj znacznik konca odbioru
            UAD1tf_RFExt = 0;
            UAD1tf_RFBrSn = 0;                                            //przygotuj znacznik oczekiwania na BREAk SYNCH
            //--------------------
            ReaderRX1ON();                                               //uruchom odbiornik
            krok=1;
        }
        case 1:
        {
            if(UAD1tf_RFExt)
            {
               UAD1tf_RFExt=0;
               LIN1f_ACK=0;                                             //blad naglowka - odpowiedz negatywna ignoruj paczke
               krok=3;
               return 4;    //blad naglowka
            }           

            if(ChkReceptKnf())//||UAD2tf_RFExt)      //bbb
            {
                UAD1tf_RFExt=0;
                LIN1f_Busy=0;
                LIN1f_NDTA=1;
                LIN1f_ACK=0; 
                krok=3;
                return 0;               //restart procedury
            }            
            if(UAD1tf_RFData&&!LIN1f_FResp)                                //odebrano pierwszy bajt?
            {
                LIN1f_FResp=1;                                            //ustaw znacznik odbioru
                UAD1t.tmFR=0;                                            //zeruj zegar ramki
            }
            if(!UAD1tf_RFEnd) return 0;                                   //czekaj do konca odbioru lub do stanu timeout
            UAD1tf_RFEnd=0;                                               //zkonczono odbior (odbiornik zatrzymany w przerwaniu)
            if(ChkR1CHK())                                               //sprawdz CHK
            {
                LIN1f_CHK=1;
                //if(MGSDf_NO_ODB||PFN.vUNI02<0x09||!PFN.ALIN)             //brak komunikacji z plytka UNI-02 lub wersja oprogramowania UNI-02<09 lub nieprawidlowy adres wlasny LIN?
                if(MGSDf_NO_ODB||!PFN.ALIN)             //brak komunikacji z plytka UNI-02 lub wersja oprogramowania UNI-02<09 lub nieprawidlowy adres wlasny LIN?                    
                {
                    LIN1f_ACK=0;                      
                    krok=3;
                    return 6;   //blad odbiornika                 
                }
                else
                if(!ChkParity1(bufRUART1[0]))        //sprawdz parzystosc
                {
                    LIN1ERR.ID_PR=1;
                    LIN1f_ACK=0;
                    krok=3;
                    return 4;   //blad parzystosci
                }
                else
                //if((!bufRUART1[1]&&(bufRUART1[0]&0b00111111)==_PIF_READ)&&(bufRUART1[4]==0x01)&&(PFN.ALIN==0x05)) //adres zerowy, zadanie wysylki danych ACK do MASTER, ale adres wlasny neutralny?    
                if((!bufRUART1[1]&&(bufRUART1[0]&0b00111111)==_PIF_READ)&&(bufRUART1[4]==_CLIN_RRD_ACK||bufRUART1[4]==_CLIN_RRD_LIFADDR)&&(PFN.ALIN==0x05)) //adres zerowy, zadanie wysylki danych ACK do MASTER, ale adres wlasny neutralny?   //nnn 
                {
                    LIN1f_ACK=0;           //zapobiegaj odpowiedzi przy adresie wlasnym neutralnym           
                    krok=3;
                    return 2;   //ignoruj adres
                }
                else                       
                //if((!bufRUART1[1]&&(bufRUART1[0]&0b00111111)!=_PIF_WRITE)&&(bufRUART1[4]!=0x01)) //adres zerowy i zadanie wysylki danych do MASTER inne niz ACK?
                if((!bufRUART1[1]&&(bufRUART1[0]&0b00111111)!=_PIF_WRITE)&&!(bufRUART1[4]==_CLIN_RRD_ACK||bufRUART1[4]==_CLIN_RRD_LIFADDR)) //adres zerowy i zadanie wysylki danych do MASTER inne niz ACK?    //nnn
                {
                    LIN1f_ACK=0;            //adres zerowy akceptuj tylko przy poleceniach zapisu z wyjatkiem polecenia ACK ktore moze byc do odczytu                   
                    krok=3;
                    return 2;   //ignoruj adres
                }
                else
                if(bufRUART1[1]&&(bufRUART1[1]!=_MY_NAD1))                 //adres niezerowy i niezgodny?
                {
                    LIN1f_ACK=0;                      
                    krok=3;
                    return 3;   //blad adresu
                }
                else                                //zapamietaj ramke
                {
                    FRL1f_NextFr=1;
                    if(FRL1.PFR<0xFF) FRL1.PFR++;                           //licznik powtorzen polecenia
                    if(FRL1.PIF.war!=bufRUART1[0]) FRL1.PFR=0;
                    FRL1.PIF.war=bufRUART1[0];                              //protected idetifier
                    if(FRL1.NAD!=bufRUART1[1]) FRL1.PFR=0;
                    FRL1.NAD=bufRUART1[1];                                  //NAD Adres odbiornika
                    if(FRL1.PCI!=bufRUART1[2]) FRL1.PFR=0;
                    FRL1.PCI=bufRUART1[2];                                  //PCI
                    FRL1.PCI_TYPE=(FRL1.PCI&0xF0);                          //Rodzaj ramki
                    //************************************
                    //FRL1.PCI_LENGTH=(FRL1.PCI&0x0F);                        //Rozmiar ramki(SID+liczba bajtow danych+CHK)                    
                    FRL1.PCI_LENGTH=(FRL1.PCI&0x07);                        //Rozmiar ramki(SID+liczba bajtow danych+CHK)
                    //******************
                    if(FRL1.WRD0.byte0!=bufRUART1[3]) FRL1.PFR=0;
                    FRL1.WRD0.byte0=bufRUART1[3];                           //SID/INDL (licznik)
                    if(FRL1.WRD0.byte1!=bufRUART1[4]) FRL1.PFR=0;
                    FRL1.WRD0.byte1=bufRUART1[4];                           //IDN/INDH
                    FRL1.WRD1.byte0=bufRUART1[5];                           //DTA1/STARTL       - RS [kom,adr]
                    FRL1.WRD1.byte1=bufRUART1[6];                           //DTA2/STARTH       - RS [dta1,par1]
                    FRL1.WRD2.byte0=bufRUART1[7];                           //DTA3/STOPL        - RS [dta2]
                    FRL1.WRD2.byte1=bufRUART1[8];                           //DTA4/STOPH        - RS [dta3]

                    if((FRL1.PCI_TYPE!=_PCI_TCF1)&&(FRL1.PCI_TYPE!=_PCI_TECF1)) //TSF, TFF
                    {
                        FRL1.SID=FRL1.WRD0.byte0;                               //SID   
                        //******************************
                        if(FRL1.PCI_TYPE==_PCI_TSF1)
                        {
                            FRL1f_Auto=0;
                        }
                        else
                        if((FRL1.PCI_TYPE==_PCI_TFF1)||(FRL1.PCI_TYPE==_PCI_TEFF1))
                        {
                            FRL1f_Auto=(FRL1.PCI&0b00001000)?1:0;                //znacznik autoramki
                        }
                        FRL1.PCI=FRL1.PCI&0b11110111;
                        FRL1.RND=bufRUART1[2]&0x07;                  //liczba bajtow danych                        
                        //******************************                        
                        if(!(bufRUART1[4]&0b10000000))      //tradycyjna ramka LIN
                        {
                            FRL1f_RSFrame=0;
                            FRL1f_RSMultiFrame=0;
                            FRL1.IDN=FRL1.WRD0.byte1;                               //IDN                            
                        }
                        else    //transportowa ramka LIN (dane z modulu dostepowego WiFi)
                        {
                            FRL1f_RSFrame=1;
                            FRL1.IDN=bufRUART1[4];                          //wartosc wstepna, dalej (po analizie danych) podmieniony wartoscia LIN IDN
                            FRL1.RS_KOM=(bufRUART1[5]&0b11100000)>>5;       //RS(komenda)
                            if((FRL1.PIF.war&0b00111111)==_PIF_WRITE) FRL1.RS_KOM|=0b10000000;    //znacznik WR
                            else FRL1.RS_KOM&=0b01111111;
                            FRL1.RS_ADR=(bufRUART1[5]&0b00011111);          //RS(adres)
                            FRL1.RS_IDN=bufRUART1[4];                             
                            if(FRL1.PCI_TYPE==_PCI_TSF1)    //pojedyncza ramka?
                            {
                                FRL1f_RSMultiFrame=0;     
                                FRL1.WRD1.byte0=FRL1.WRD1.byte1;            //dta1,param1
                                FRL1.WRD1.byte1=FRL1.WRD2.byte0;            //dta2,param2
                                FRL1.WRD2.byte0=FRL1.WRD2.byte1;            //dta3,param3
                                FRL1.WRD2.byte1=0;
                            }
                            else                            //ramka wielokrotna
                            {
                                FRL1f_RSMultiFrame=1;
                                FRL1.WRD1.byte0=FRL1.WRD1.byte1;            //param1
                                FRL1.WRD1.byte1=0;       
                            }
                        } 
                        if((FRL1.PCI_TYPE==_PCI_TFF1)||(FRL1.PCI_TYPE==_PCI_TEFF1))
                        {
                            FRL1.CSID=FRL1.SID;
                        }
                    }
                }
            }
            else
            {
                LIN1ERR.CRC=1;               //blad CRC
                LIN1f_ACK=0;
                LIN1f_CHK=0;
                krok=3;
                return 5;   //blad CHK
            }
            LIN1f_FResp=0;
            LIN1f_FAdta=1;                   //ustaw znacznik analizy danych
            LIN1.tmAd=0;
            LIN1.tmZw=0;            
            krok=2;
        }
        case 2:
        {
            switch(SetLIN1SData())      //analiza ramki i przygotowanie odpowiedzi
            {
                case 0:                 //trwa przetwarzanie danych
                {
                    return 0;
                }
                case 1:                 //gotowa odpowiedz
                {
#if ((_TESTCHK1!=0)&&((_TESTCHK1S!=0)||(_TESTCHK1F!=0)||(_TESTCHK1C!=0)))
#if (_TESTCHK1S!=0)
                    if(FRL1.PCI_TYPE==_PCI_TSF1) a=_TESTCHK1;
                    else
                    if(FRL1.PCI_TYPE==_PCI_TESF1) if(a) a--;
#else
                    if((FRL1.PCI_TYPE==_PCI_TSF1)||(FRL1.PCI_TYPE==_PCI_TESF1)) a=0;
#endif                    
#if (_TESTCHK1F!=0)
                    if(FRL1.PCI_TYPE==_PCI_TFF1) a=_TESTCHK1;
                    else
                    if(FRL1.PCI_TYPE==_PCI_TEFF1) if(a) a--;
#else
                    if((FRL1.PCI_TYPE==_PCI_TFF1)||(FRL1.PCI_TYPE==_PCI_TEFF1)) a=0;
#endif
#if (_TESTCHK1C!=0)
                    if(FRL1.PCI_TYPE==_PCI_TCF1) a=_TESTCHK1;
                    else
                    if(FRL1.PCI_TYPE==_PCI_TECF1) if(a) a--;
#else
                    if((FRL1.PCI_TYPE==_PCI_TCF1)||(FRL1.PCI_TYPE==_PCI_TECF1)) a=0;
#endif
#endif
                    LIN1f_ACK=1;
                    bufTUART1[0]=_MY_NAD1;                                       //NAD
                    //bufTUART1[1]=(FTL1.PCI_TYPE&0xF0)|(FTL1.PCI_LENGTH&0x0F);    //PCI                    
                    //****************************
                    bufTUART1[1]=(FTL1.PCI_TYPE&0xF0)|(FTL1.PCI_LENGTH&0x07);    //PCI
                    if((FTL1.PCI_TYPE==_PCI_TFF1)||(FTL1.PCI_TYPE==_PCI_TEFF1))
                    {
                        if(FTL1f_Auto) bufTUART1[1]|=0b00001000;
                        else bufTUART1[1]&=0b11110111;   
                    }
                    else
                    {
                        bufTUART1[1]&=0b11110111;    
                    }                                
                    //****************************
                    bufTUART1[2]=FTL1.WRD0.byte0;                                //DTA0/RSID
                    bufTUART1[3]=FTL1.WRD0.byte1;                                //DTA1/TCHK                  
                    bufTUART1[4]=FTL1.WRD1.byte0;                                //DTA2
                    bufTUART1[5]=FTL1.WRD1.byte1;                                //DTA3
                    bufTUART1[6]=FTL1.WRD2.byte0;                                //DTA4
                    bufTUART1[7]=FTL1.WRD2.byte1;                                //DTA5
                    //UAD1t.TNData=2+(FTL1.PCI_LENGTH&0x0F)+1;                     //NAD,PCI,liczba danych do wysylki,CHK                    
                    //***************************
                    UAD1t.TNData=2+(FTL1.PCI_LENGTH&0x07)+1;                     //NAD,PCI,liczba danych do wysylki,CHK                    
                    //***************************
#if _TESTCHK1!=0
                    bufTUART1[UAD1t.TNData-1]=0xFF-CalcT1CHK()+a;                  //neg CHK
#else
                    bufTUART1[UAD1t.TNData-1]=0xFF-CalcT1CHK();                  //neg CHK
#endif
                    break;
                }
                case 2:               //decyzja zeby nie generowac odpowiedzi
                {
                    LIN1f_FAdta=0;     //zeruj znacznik analizy danych
                    LIN1f_ACK=0;
                    krok=3;
                    return 2;         //decyzja o braku odpowiedzi
                }
            }
            LIN1f_FAdta=0;             //zeruj znacznik analizy danych
            krok=3;
        }
        case 3:
        {
            return 1;
        }
    }
    return 1;
}
//-----------------------------------------------------------------
//Transmisja danych w trybie SLAVE
unsigned char LIN1STrans(unsigned char ref)
{
    volatile static unsigned char krok;

    //-----------------------------------------------------
    if(ref)
    {
        krok=0;
        return 0;
    }
    //Algorytm MASTER
    switch(krok)
    {
        case 0: //rozpoczecie procesu
        {
            LIN1f_FTrans=1;
            UAD1t.tmFR=0;
            krok=1;
        }
        case 1: //oczekiwanie na zwolnienie linii
        {
            switch(ChkTX1Busy())                 //oczekuj na zwolnienie linii
            {
                case 1: //busy
                {
                    return 0;
                }
                case 2: //timeout
                {
                    LIN1ERR.TFTOUT=1;
                    krok=5;
                    return 2;
                }
            }
#if _SYG_STN1==1
            _S_WRITE1=1;
#endif
            krok=2;
        }
        case 2: //start transmisji
        {
            if(!StartLIN1TR()) return 0;                //rozpocznij transmisje
            krok=3;
        }
        case 3: //oczekiwanie na koniec transmisji
        {
            switch(ChkTX1Busy())                 //oczekuj na zwolnienie linii
            {
                case 1: //busy
                {
                    return 0;
                }
                case 2: //timeout
                {
                    LIN1ERR.TFTOUT=1;
                    krok=5;
                    return 2;
                }
            }
            LIN1f_FTrans=0;

            StopLIN1TR();                               //wylacz modul transmisji
#if _SYG_STN1==1
            _S_WRITE1=0;
#endif
            krok=5;
        }
        case 5:
        {
            return 1;
        }
    }
    return 1;
}
//------------------------------------------------------------------------------------
//Funkcja transmisji/odbioru w trybie SLAVE (umiescic w petli glownej)
bit LIN1SDeamon(void)
{
    volatile static unsigned char krok=0;

    //-----------------------------------------------
    if(LIN1f_FAdta&&LIN1.tmAd>_LN1_ADTA)                                 //timeout dla przetwarzania danych?
    {
        LIN1ERR.TADTA=1;
        krok=9;
    }
    else
    if(LIN1f_FResp&&UAD1tf_RFData&&(UAD1t.tmB>_LN1_RMB))                  //timeout dla bajtu odpowiedzi?
    {
        LIN1ERR.RBTOUT=1;
        krok=9;
    }
    else
    if(LIN1f_FResp&&(UAD1t.tmFR>_LN1_TRMF))                              //timeout dla ramki odbioru?
    {
        LIN1ERR.RFTOUT=1;
        krok=9;
    }
    else
    if(LIN1f_FTrans&&(UAD1t.tmFR>_LN1_TRMF))                             //timeout dla ramki odpowiedzi?
    {
        LIN1ERR.TFTOUT=1;
        krok=9;
    }
    else
    {
        if(!LIN1f_NDTA) krok=10;                       //blokada odbioru danych?
        //----------------------------------------------------
        //Przejdz w tryb gotowosci do odbioru nowej paczki danych
        if(!LIN1f_Busy&&LIN1f_NDTA)                     //odblokowany odbior danych?
        {
            LIN1ERR.wrd=0;
            LIN1f_Busy=1;
            LIN1f_FIN=0;
            LIN1f_ERRR=0;
            LIN1f_ACK=0;
            LIN1f_SCS=0;
            LIN1SRecept(1);                          //restart procedury
            LIN1STrans(1);                           //restart procedury
            krok=0;
        }
    }
    switch(krok)
    {
        case 0:     //gotowosc odbioru
        {
            switch(LIN1SRecept(0))
            {
                case 0:             //czekaj na koniec odbioru
                    return 0;
                case 1:             //koniec odbioru(nadaj odpowiedz)
                    krok=1;
                    return 0;
                case 2:             //koniec odbioru(decyzja o braku odpowiedzi)
#if _LIN1_ALLEODP==1                    
                    if(FRL1f_RSFrame)
                    {                    
                        if(!FRL1.NAD) //adres 0?
                        {
                            krok=9;//brak odpowiedzi
                        }
                        else
                        {
                            LIN1NegativeResponse(_LIN1_ERREXT,0,0,0);   //inna przyczyna bledu
                            krok=1;
                        }
                    }
                    else
                    {
                        krok=9;
                    }
                    return 0;
#else
                    krok=9;     //brak odpowiedzi
                    return 0;
#endif
                case 3:             //niewlasciwy adres
#if _LIN1_ALLEODP==1                    
                    if(FRL1f_RSFrame)
                    {
                        LIN1NegativeResponse(_LIN1_ERRADR,0,0,0);
                        krok=1;
                    }
                    else
                    {
                        krok=9;     //brak odpowiedzi                       
                    }
                    return 0;                    
                case 4:             //blad ramki
                    if(FRL1f_RSFrame)
                    {                    
                        LIN1NegativeResponse(_LIN1_ERRFRM,0,0,0);
                        krok=1;
                    }
                    else
                    {
                        krok=9;     //brak odpowiedzi                    
                    }                    
                    return 0;
#else
                    krok=9;     //brak odpowiedzi
                    return 0;
#endif                    
                case 5:             //blad CHK
#if _LIN1_ALLEODP==1                     
                    if(FRL1f_RSFrame)
                    {                    
                        LIN1NegativeResponse(_LIN1_ERRCHK,0,0,0);
                        krok=1;
                    }
                    else
                    {
                        krok=9;     //brak odpowiedzi
                    }
                    return 0;   
#else
                    krok=9;     //brak odpowiedzi
                    return 0;
#endif                    
                case 6:             //brak odpowiedzi odbiornika    
#if _LIN1_ALLEODP==1                     
                    if(FRL1f_RSFrame)
                    {                    
                        LIN1NegativeResponse(_LIN1_ERRODP,0,0,0);
                        krok=1;
                    }
                    else
                    {
                        krok=9;     //brak odpowiedzi
                    }
                    return 0;   
#else                         
                    krok=9; //brak odpowiedzi                 
                    return 0;      
#endif                    
                default:
#if _LIN1_ALLEODP==1                    
                    if(FRL1f_RSFrame)
                    {                    
                        LIN1NegativeResponse(_LIN1_ERREXT,0,0,0);   //inna przyczyna bledu
                        krok=1;
                    }
                    else
                    {
                        krok=9;     //brak odpowiedzi                     
                    }                    
                    return 0;       
#else
                    krok=9;     //brak odpowiedzi
                    return 0;
#endif                                    
            }
        }
        case 1:     //zanim odpowiesz odczekaj na zakonczenie transmisji do UNI-02
        {
            if(RSDT1f_NewLIDN)      //transmisja zapytania/polecenia do UNI-02?
            {
                if(MGSDf_TransData) return 0;   //czekaj na zakonczenie transmisji
            }
            //if(LIN1f_FAdta&&(LIN1.tmZw<_LN1_ADTA)) return 0;      //uplynela minimalna zwloka przed wysylka odpowiedzi?
            if(LIN1.tmZw<_LN1_TZW) return 0;      //uplynela minimalna zwloka przed wysylka odpowiedzi?
            krok=2;
        }
        case 2:     //nadawanie odpowiedzi
        {
            switch(LIN1STrans(0))           //czekaj na zakonczenie nadawania
            {
                case 0:
                    return 0;       //czekaj na zakonczenie nadawania
                case 1:             //koniec transmisji
                case 2:             //error
                    krok=9;
                    return 0;
            }
        }
        case 9:     //zakoncz procedure/ustaw flagi stanu
        {
            ResetRC1TX1();            //wylacz nadajnik/odbiornik
            LIN1RefStat();          //ustaw flagi stanu
#if _SYG_STN1==1
            _S_READ1=0;
            _S_WRITE1=0;
#endif
            krok=10;
        }
        case 10:    //oczekiwanie na restart procedury
        {
            LIN1f_NDTA=1;            //przejdz do gotowosci odbioru
            break;
        }
    }
    return 1;
}
//-------------------------------------------------------------
//Oblicz CHK (bez negacji) dla odebranej paczki z nadajnika MASTER i sprawdz zgodnosc
bit ChkR1CHK(void)
{
    unsigned char i,j,c,d,s;

    if(UAD1t.RNData >= _NRS_RBF1) return 0;
    c=0x00;
 #if _ORYGINAL1_CHK==1
         #if _CLASR1_CHK==1
            for(i=1;i<UAD1t.RNData-1;i++)          //classic CHK
            {
                if((c+bufRUART1[i])>=256)
                {
                  c=c+bufRUART1[i]+1;
                }
                else c=c+bufRUART1[i];
            }
        #else
            if(((bufRUART1[0]&0b00111111)==0x3C)||((bufRUART1[0]&0b00111111)==0x3D))  //classic CHK
            {
                for(i=1;i<UAD1t.RNData-1;i++)          //classic CHK
                {
                    if((c+bufRUART1[i])>=256)
                    {
                      c=c+bufRUART1[i]+1;
                    }
                    else c=c+bufRUART1[i];
                }
            }
            else    //enhanced CHK
            {
                for(i=0;i<UAD1t.RNData-1;i++)          //enhanced CHK
                {
                    if((c+bufRUART1[i])>=256)
                    {
                      c=c+bufRUART1[i]+1;
                    }
                    else c=c+bufRUART1[i];
                }
            }
        #endif
#else
    //CRC-8 Dallas/Maxim
    for(i=0;i<UAD1t.RNData-1;i++)
    {
        d=bufRUART1[i];
        for(j=8;j>0;j--)
        {
            s=(c^d)&0x01;
            c>>=1;
            if(s) c^=0x8c;
            d>>=1;
        }
    }
#endif
    if((bufRUART1[UAD1t.RNData-1]+c)==0xFF) return 1;
    return 0;
}
//-------------------------------------------------------------
//Oblicz CHK (bez negacji) dla wysylanej paczki potwierdzenia SLAVE
unsigned char CalcT1CHK(void)
{
    unsigned char i,j,c,d,s;
    c=0x00;
#if _ORYGINAL1_CHK==1
    for(i=0;i<UAD1t.TNData-1;i++)          //classic CHK
    {
        if((c+bufTUART1[i])>=256)
        {
          c=c+bufTUART1[i]+1;
        }
        else c=c+bufTUART1[i];
    }
#else
    //CRC-8 Dallas/Maxim
    for(i=0;i<UAD1t.TNData-1;i++)
    {
        d=bufTUART1[i];
        for(j=8;j>0;j--)
        {
            s=(c^d)&0x01;
            c>>=1;
            if(s) c^=0x8c;
            d>>=1;
        }
    }
#endif
    return c;
}
//-------------------------------------------------------------------------
//sprawdz bity pzrzystosci
bit ChkParity1(unsigned char bt)
{
    BTY_VAL1 d;
    d.war=bt;
    if(d.b6==((d.b0^d.b1^d.b2^d.b4)?1:0))
        if(d.b7==((d.b1^d.b3^d.b4^d.b5)?0:1))
            return 1;
    return 0;
}
unsigned char iChkParity1(unsigned char bt)
{
    BTY_VAL1 d;
    d.war=bt;
    if(d.b6==((d.b0^d.b1^d.b2^d.b4)?1:0))
        if(d.b7==((d.b1^d.b3^d.b4^d.b5)?0:1))
            return 1;
    return 0;
}
//-------------------------------------------------------------------------
//Sprawdz naglowek ramki
bit iAnalizeHeader(volatile unsigned char indeks)
{
    switch(indeks)
    {
        case 0:
        {
            if(!iChkParity1(bufRUART1[0]))
            {
                //indeks=13;
                return 0;
            }
            if((((bufRUART1[0]&0b00111111)!=_PIF_WRITE)&&(bufRUART1[0]&0b00111111)!=_PIF_READ))
            {
                //indeks=14;
                return 0;
            }
            break;
        }
        case 1:
        {
            if(bufRUART1[1]&&(bufRUART1[1]!=_MY_NAD1))            //adres niezerowy i niezgodny?
            {
                //indeks=15;
                return 0;
            }
            break;
        }
        case 2:
        {
            //UAD1t.RNData=3+(bufRUART1[2]&0x0F)+1;                 //PFI,NAD,PCI+liczba danych+CHK            
            //****************************
            UAD1t.RNData=3+(bufRUART1[2]&0x07)+1;                 //PFI,NAD,PCI+liczba danych+CHK
            //****************************
            if(UAD1t.RNData>10)                                   //przekroczony maksymalny rozmiar ramki?
            {
                //indeks=16;
                return 0;
            }
            break;
        }
    }
    return 1;
}
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------	
//Zegary LIN1 (umiescic w przerwaniu od timer3)
void IntLIN1Tim(void)
{
    if(UAD1t.tmB<0xFFFF) UAD1t.tmB++;       //timer dla  bajtu (odbior)
    if(UAD1t.tmFR<0xFFFF) UAD1t.tmFR++;     //timer dla ramki (odbior/nadawanie)
    if(LIN1.tmAd<0xFFFF) LIN1.tmAd++;       //timer dla przetwarzania odebranej paczki
    if(LIN1.tmZw<0xFFFF) LIN1.tmZw++;       //timer dla minimalna zwloka przed wysylka odpowiedzi
}
//------------------------------------------------------------------------------------	
//---------------------------------------------------------------------
//Odbior danych nadsylanych przez SLAVE (umiescic w przerwaniu od RX1)
/*
void IntUSART1SRC(void)
{
    //if(UAD1tf_RFEnd)     //po odbiorze i
    //{
    //    RSerial1 = RCREG1;
    //    return;
    //}
    if(RCSTA1bits.FERR||RCSTA1bits.OERR)
    {
        #if _SYG_STN1==1
        _S_READ1=0;
        #endif
        RSerial1 = RCREG1;      //Wyzeruj RCSTA1bits.FERR
        RCSTA1bits.CREN = 0;    //Wyzeruj RCSTA1bits.OERR
        iRestartRX();
        return;
    }
#if _SYG_STN1==1
    _S_READ1=1;
#endif
    RSerial1 = RCREG1;
    UAD1t.tmB=0;                                             //zeruj timer odbioru bajtu
    UAD1tf_RFData=1;
    //--------------------
    if(!UAD1tf_RFBrSn)                                        //oczekiwanie na BREAK i SYNCH
    {
        UAD1t.Rindeks++;
        if(UAD1t.Rindeks>_RBREAK_SYNCH1)                       //pomin 2 pierwsze bajty
        {
            UAD1t.Rindeks=0;
            UAD1tf_RFBrSn=1;
        }else return;
    }
    //-------------------
    if(UAD1t.Rindeks < _NRS_RBF1)                             //mozliwe przepelnienienie bufora odbioru?
    {
        bufRUART1[UAD1t.Rindeks] = RSerial1;                  //zapamietaj dana w buforze od pozycji 0
        if(!iAnalizeHeader(UAD1t.Rindeks))                    //kontroluj parametry ramki
        {
            #if _SYG_STN1==1
                _S_READ1=0;
            #endif
            iRestartRX();
            return;
        }
        UAD1t.Rindeks++;
        if(UAD1t.Rindeks >= UAD1t.RNData)                     //ostatni bajt ramki?
        {
            RCSTA1bits.CREN = 0;
            PIE1bits.RC1IE=0;                                 //deaktywacja przerwania dla odbioru
            UAD1tf_RFData = 0;                                 //sygnalizuj deaktywacje odbioru danych
            UAD1tf_RFEnd = 1;                                  //ustaw znacznik ze odebrano kompletna paczke danych
            #if _SYG_STN1==1
                _S_READ1=0;
            #endif
        }
    }
    else
    {
        #if _SYG_STN1==1
            _S_READ1=0;
        #endif
        iRestartRX();
        return;
    }
}
*/
void IntUSART1SRC(void)
{
    volatile unsigned char i;
    
    if(RCSTA1bits.FERR||RCSTA1bits.OERR)
    {
        RSerial1 = RCREG1;       //Wyzeruj RCSTA1bits.FERR
        PIE1bits.RC1IE=0;       //deaktywacja przerwania odbioru
        RCSTA1bits.CREN = 0;    //Wyzeruj RCSTA1bits.OERR
        LIN1ERR.BT=1;
#if _SYG_STN1==1
        _S_READ1=0;
#endif
        return;
    }
#if _SYG_STN1==1
    _S_READ1=1;
#endif
    RSerial1 = RCREG1;
    UAD1t.tmB=0;                                             //zeruj timer odbioru bajtu
    UAD1tf_RFData=1;
    //--------------------
    if(!UAD1tf_RFBrSn)                                        //oczekiwanie na BREAK i SYNCH
    {
        UAD1t.Rindeks++;
        if(UAD1t.Rindeks>_RBREAK_SYNCH1)                       //pomin 2 pierwsze bajty
        {
            UAD1t.Rindeks=0;
            //++++++++++++++++++++++++++++++++++++++++++++++++++++
            for(i=0;i<_NRS_RBF1;i++)         //wyzeruj bufor
            {
                bufRUART1[i]=0;
            }
            //++++++++++++++++++++++++++++++++++++++++++++++++++++            
            UAD1tf_RFBrSn=1;
        }else return;
    }
    //-------------------
    if(UAD1t.Rindeks < _NRS_RBF1)                             //mozliwe przepelnienienie bufora odbioru?
    {
        bufRUART1[UAD1t.Rindeks] = RSerial1;                   //zapamietaj dana w buforze od pozycji 0

        if(!iAnalizeHeader(UAD1t.Rindeks))
        {
            RCSTA1bits.CREN = 0;
            PIE1bits.RC1IE=0;                                   //deaktywacja przerwania odbioru
            UAD1tf_RFData = 0;                                  //sygnalizuj deaktywacje odbioru danych
            UAD1tf_RFExt = 1;                                   //ustaw znacznik anulowania odbioru
            #if _SYG_STN1==1
                _S_READ1=0;
            #endif
            return;
        }
        UAD1t.Rindeks++;
        if(UAD1t.Rindeks >= UAD1t.RNData)                     //ostatni bajt ramki?
        {
            RCSTA1bits.CREN = 0;
            PIE1bits.RC1IE=0;                   //deaktywacja przerwania odbioru
            UAD1tf_RFData = 0;                                 //sygnalizuj deaktywacje odbioru danych
            UAD1tf_RFEnd = 1;                                  //ustaw znacznik ze odebrano kompletna paczke danych
            #if _SYG_STN1==1
                _S_READ1=0;
            #endif
        }
    }
    else
    {
        LIN1ERR.BUF=1;                    //blad przepelnienia bufora
        RCSTA1bits.CREN = 0;
        PIE1bits.RC1IE=0;                   //deaktywacja przerwania odbioru
        #if _SYG_STN1==1
            _S_READ1=0;
        #endif
    }
}
 
//------------------------------------------------------------------------------------

//---------------------------------------------------------------------
//Transmisja danych do SLAVE (umiescic w przerwaniu od TX1)
void IntUSART1STR(void)
{
    if(UAD1t.Tindeks>=_NRS_TBF1) return;
    TXREG1=bufTUART1[UAD1t.Tindeks++];	//rozpoczyna transmisje
    if(UAD1t.Tindeks>=UAD1t.TNData)
    {
        UAD1tf_TFEnd=1;                            //ustaw znacznik zakonczenia wysylki ramki
                                                  //UWAGA: transmisja trwa!
        PIE1bits.TX1IE=0;                         //deaktywacja przerwania od transmisji
    }
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//Liczy CHK elementow tablicy tab poczynajac od indeksu start a konczac na stop
unsigned char CalcTabCHK(unsigned char *tab, unsigned int start, unsigned int stop)
{
    unsigned int i;
    unsigned char j,c,d,s;
    c=0x00;

    //CRC-8 Dallas/Maxim
    for(i=start;i<=stop;i++)
    {
        d=tab[i];
        for(j=8;j>0;j--)
        {
            s=(c^d)&0x01;
            c>>=1;
            if(s) c^=0x8c;
            d>>=1;
        }
    }
    return c;
}
#endif      //(_EXT_BOARD>=2)
