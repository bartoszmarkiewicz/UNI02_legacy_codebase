/*LIN1Slave.h*/
//----------------------------------------------------------
#ifndef	_LIN1SLAV_H
#define	_LIN1SLAV_H
#include "global.h"			//parametry globalne
#include "LIN1Slave.tp.h"		//typy danych
/*---------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------*/
#define _SYG_STN1       0                       //wlacz/wylacz sygnalizacje diodami stanu odbior/zapis
#define _S_READ1        LATGbits.LG1//LATG1     //wyjscie diody zielonej
#define _S_WRITE1       LATGbits.LG2//LATG2     //wyjscie diody czerwonej
/*---------------------------------------------------------------------------------*/
#define _NRS_RBF1       15			//rozmiar bufora odebranych danych
#define _NRS_TBF1       15			//rozmiar bufora wysylanych danych
/*---------------------------------------------------------------------------------*/
#define _NRS_TBF2       80			//rozmiar bufora multi odpowiedzi
/*-----------------------------PUBLIC DATA-----------------------------------------*/
#define _BRGH           1			//(w trybie synch. ignorowany) 0 - low speed, 1- high speed
#define _BRG16          1                       //rozdzielczosc SBRGH
#define _SYNC           0			//0 - asynch. 1 - synch.
#define _DSBD           9600			//wstepna predkosc transmisji RS (dla SLAVE autonegocjacja)
/*---------------------------------------------------------------------------------*/
#define _RBREAK_SYNCH1  2                       //2-break+synch, 1-break, 0-czyste dane
/*---------------------------------------------------------------------------------*/
#define _LN1_ADTA       100                     //100 x10mS - maksymalny czas na pzretworzenie danych do wysylki
#define _LN1_TZW        3                       //3 x10mS - minimalna zwloka przed wysylka odpowiedzi
#define _LN1_TRMF       25                      //25 x10mS - maksymalny czas na wysylke lub odbior ramki
#define _LN1_RMB        3                       //3 x10mS - ogolny maksymalny czas na rejestracje kolejnych bajtow odbioru
/*---------------------------------------------------------------------------------*/
//stan ukladu MCP2003
#define _CS1_DELAY      100                     //[n osc] minimalny czas impulsu CS
#define _TOFF_MODE1     1                       //TOFF mode
#define _OPER_MODE1     2                       //POWER DOWN mode
#define _PWRD_MODE1     3                       //POWER DOWN mode
/*---------------------------------------------------------------------------------*/
#define _ORYGINAL1_CHK   0                      //oryginalny CHK wg specyfikacji LIN
#define _CLASR1_CHK     0                       //rodzaj CHK dla odbioru SLAVE (0-enchanced 1-classic)
/*---------------------------------------------------------------------------------*/
#define _TESTCHK1       0                       //generuje blad chk przez _TESTCHK1 kolejnych ramek odpowiedzi
#define _TESTCHK1S      1                       //generuje blad chk przez dla ramek TFS i TEFS
#define _TESTCHK1F      1                       //generuje blad chk przez dla ramek TFF i TEFF
#define _TESTCHK1C      1                       //generuje blad chk przez dla ramek TFC i TEFC
/*---------------------------------------------------------------------------------*/
#define _PIF_WRITE      0x01                    //MASTER zada zapisu nadeslanych danych
#define _PIF_READ       0x02                    //MASTER zada odeslania danych
/*---------------------------------------------------------------------------------*/
#define _PCI_TSF1       0x00                    //PCItype=SF (single frame) - maksimum 5 bytes data + 1(SID/RSID)
#define _PCI_TFF1       0x10                    //PCItype=FF (first frame for CF) - liczba danych PCI_LENGTH(4 najstarsze bity)|LEN(8 mlodszych bitow)
#define _PCI_TCF1       0x20                    //PCItype=CF (Consecutive Frames) - liczba danych rotacyjnie inkrementowana od 0 do 15 az do wyslania calej paczki.

#define _PCI_TESF1       _PCI_TSF1+0x30
#define _PCI_TEFF1       _PCI_TFF1+0x30
#define _PCI_TECF1       _PCI_TCF1+0x30

/*---------------------------------------------------------------------------------*/
#define _LIN1_ALLEODP     0                     //odpowiedz zwrotna przy ka¿dym bledzie komunikacji
//zwracane w odpowiedzi kody bledow
#define _LIN1_UNCCOM      1                     //nieznana komenda
#define _LIN1_INVPAR      2                     //nieprawidlowy parametr
#define _LIN1_INVDTA      3                     //nieprawidlowe dane
#define _LIN1_INVCHK      4                     //nieprawidlowe CHK dla danych tablicowych
#define _LIN1_ERRODP      5                     //brak lub bledna odpowiedz odbiornika innej sieci (np UNI-02)
#define _LIN1_ERRADR      6                     //nieprawidlowy adres
#define _LIN1_ERRFRM      7                     //blad naglowka ramki
#define _LIN1_ERRCHK      8                     //blad chk
#define _LIN1_ERREXT      9                     //inny blad
//stan procedury Interpreter
#define _LIN1_IFINAL     0x00                     //zakonczono proces przetwarzania/przygotowania danych
#define _LIN1_NORESP     0xF0                     //decyzja o braku odpowiedzi
#define _LIN1_IBUSY      0xFF                     //procedura zajeta

/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
void ResetRC1TX1(void);
void ReaderRX1ON(void);
void SetLIN1_PWRD_Mode(void);
unsigned char ChkTX1Busy(void);
bit StartLIN1TR(void);
bit StartLIN1RC(void);
void StopLIN1TR(void);
void StopLIN1RC(void);
bit ChkR1CHK(void);
unsigned char CalcT1CHK(void);
bit ChkParity1(unsigned char bt);
unsigned char CalcTabCHK(unsigned char *tab, unsigned int start, unsigned int stop);
void LIN1NegativeResponse(unsigned char kod, unsigned char npar, unsigned char par1, unsigned char par2);
//LIN1Stest.c
extern void LIN1StestRT(void);
extern void LIN1StestR(void);
//LIN1SInterpreter
extern unsigned char LIN1SingleResponse_RD(void);
extern unsigned char LIN1SingleResponse_WR(void);
extern unsigned char LIN1StartMultiResponse_RD(void);
extern unsigned char LIN1FinishMultiResponse_WR(void);
extern unsigned char LIN1StartMultiResponse_WR(void);

extern void LIN2SClrMasterIDO(void);
/*---------------------------------------------------------------------------------*/
/*-----------------------------PROTECTED DATA--------------------------------------*/
#if _RBREAK_SYNCH1>2
#error Nieprawidlowa wartosc parametru _BREAK_SYNCH
#endif
//Obliczenie wartosci poczatkowej Timera nr 1
#ifndef _EXT_OSC				//czestotliwosc oscylatora [MHz]
#error Nieokreslony parametr _EXT_OSC
#endif
#ifndef _PLL					//modul PLL (0-nieaktywny,1-aktywny)
#error Nieokreslony parametr _PLL
#endif

#if _PLL!=0
    #if (_BRGH==0)&&(_BRG16==0)&&(_SYNC==0)					//low speed?
    #define _SPBRG ((_EXT_OSC*4*1000000/_DSBD)/64)-1
    #define _CBR (_EXT_OSC*4*1000000)/(64*(_SPBRG+1))
    #endif
    #if (_BRGH!=0)&&(_BRG16==0)&&(_SYNC==0)					//high speed?
    #define _SPBRG ((_EXT_OSC*4*1000000/_DSBD)/16)-1
    #define _CBR (_EXT_OSC*4*1000000)/(16*(_SPBRG+1))
    #endif
    #if (_BRGH==0)&&(_BRG16!=0)&&(_SYNC==0)					//high speed?
    #define _SPBRG ((_EXT_OSC*4*1000000/_DSBD)/16)-1
    #define _CBR (_EXT_OSC*4*1000000)/(16*(_SPBRG+1))
    #endif
    #if (_BRGH!=0)&&(_BRG16!=0)&&(_SYNC==0)					//high speed?
    #define _SPBRG ((_EXT_OSC*4*1000000/_DSBD)/4)-1
    #define _CBR (_EXT_OSC*4*1000000)/(4*(_SPBRG+1))
    #endif
    #if _SYNC!=0					//praca synchroniczna?
    #define _SPBRG ((_EXT_OSC*4*1000000/_DSBD)/4)-1
    #define _CBR (_EXT_OSC*4*1000000)/(4*(_SPBRG+1))
    #endif
#endif

#if _PLL==0
    #if (_BRGH==0)&&(_BRG16==0)&&(_SYNC==0)					//low speed?
    #define _SPBRG ((_EXT_OSC*1000000/_DSBD)/64)-1
    #define _CBR (_EXT_OSC*1000000)/(64*(_SPBRG+1))
    #endif
    #if (_BRGH!=0)&&(_BRG16==0)&&(_SYNC==0)					//high speed?
    #define _SPBRG ((_EXT_OSC*1000000/_DSBD)/16)-1
    #define _CBR (_EXT_OSC*1000000)/(16*(_SPBRG+1))
    #endif
    #if (_BRGH==0)&&(_BRG16!=0)&&(_SYNC==0)					//high speed?
    #define _SPBRG ((_EXT_OSC*1000000/_DSBD)/16)-1
    #define _CBR (_EXT_OSC*1000000)/(16*(_SPBRG+1))
    #endif
    #if (_BRGH!=0)&&(_BRG16!=0)&&(_SYNC==0)					//high speed?
    #define _SPBRG ((_EXT_OSC*1000000/_DSBD)/4)-1
    #define _CBR (_EXT_OSC*1000000)/(4*(_SPBRG+1))
    #endif

    #if _SYNC!=0					//praca synchroniczna?
    #define _SPBRG ((_EXT_OSC*1000000/_DSBD)/4)-1
    #define _CBR (_EXT_OSC*1000000)/(4*(_SPBRG+1))
    #endif
#endif

#if (_BRG16==0) && (_SPBRG>0xFF)
#error _SPBRG - nieprawidlowe parametry obliczen
#endif
#define _EBR=_CBR-_DSBD			//wartosc bledu obliczen


//----------------------------------------------------------
#endif		/*_LIN1SLAV_H*/
