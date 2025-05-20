/*ot.c*/
//Funkcje obslugi komunikacji OT
/*---------------------------------------------------------------------------------*/
//#include <pic18.h>
//#include <htc.h>
#include <xc.h>
#include "global.h"			//parametry globalne
#include "ot.h"				//parametry lokalne

volatile tpOTDta OT;
extern volatile tpOTMaster OTMaster;

volatile unsigned int db=0;
//volatile bit PFNf_Rsync;		 
volatile bit PFNf_firstEdge;		 
volatile bit PFNf_OTTimeout;

volatile unsigned char xxy,zzz,vvv;
#if (_EXT_BOARD>=3)
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
void InitOT(void)
{
    OT.RFrame.war=0;
    OT.T2=0;
    OT.otPeriod=0;
    OT.preload=0;
    OT.preload4=0;
    OT.cnt=0;
    OT.cnt1=0;
    OT.cnt2=0;
    OT.data1=0;
    OT.errorMsg=0;
    OT.millis=0;
    OT.parityOdd=0;
    OT.previousTransitionCount=0;
    OT.sum=0;
    OT.tdr=0;
    OT.tempFrame=0;
    OT.tempFrame=0;
    OT.timeLow=0;
    OT.transitionCount=0;
    OTMaster.Ref.war=0;
    
    
    OT.tmo=0xffff;    
    OT.tptm=0;    
    PFNf_OTTimeout=0;
    
    
    
    PFNf_firstEdge=1;
    

    OT.RFrame.war=0;
	INT0IF=0;			
	INT0IE=0;
#if _NEG_IN==0    
	INTEDG0=0;          //zbocze opadajace
#else
	INTEDG0=1;          //zbocze narastajace
#endif    
	INT1IF=0;			
	INT1IE=0;			
	INT1IP=1;  
#if _NEG_IN==0    
	INTEDG1=1;          //zbocze narastajace 
#else
	INTEDG1=0;          //zbocze opadajace
#endif
    //OT.RFrame.war=0b11100000100010001111000010101010;
    //db=OT.RFrame.word0.war;    
    //db=OT.RFrame.byte2.war;    
    //db=OT.RFrame.parity;
    //db=OT.RFrame.msg_type;
    //db=OT.RFrame.data_id;
    //db=OT.RFrame.data1;
    //db=OT.RFrame.data0;    
    DetachINT();
    InitTimer0();    
    SetOTDeamon(0);   
    _OUT_OT=0;
}

void WrtTMR0(volatile unsigned int tm)
{  
    OT.prelTMR0=tm;
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Zlicza milisekundy poczawszy od aktywacji timera2  - przepelnienie po 50 dniach
//umiescic w przerwaniu od timera2
void IntMillis(void)
{
    volatile static unsigned char m4=0;
    m4++;
    if(m4>=4)   //przeladowanie co 250us, 4*250us=1ms
    {
        m4=0;
        OT.millis++;         
    }
}
/*---------------------------------------------------------------------------------*/
//zwraca wartosc milisekund ktore uplynely od aktywacji timera2
unsigned long int Millis(void)
{
    return OT.millis;
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
void StartOTTimeout(const unsigned int tmout, const unsigned char typ)
{
    OT.tdr=0;
    OT.tmo=tmout;    
    OT.tptm=typ;
    PFNf_OTTimeout=1;
}
void StopOTTimeout(void)
{
    OT.tdr=0; 
    OT.tmo=0xffff;    
    OT.tptm=0;    
    PFNf_OTTimeout=0;
}
void IntOTTimeout(void)
{
    volatile static unsigned char m4=0;
    m4++;
    if(m4>=4)   //przeladowanie co 250us, 4*250us=1ms
    {
        m4=0;
        if(OT.tdr<OT.tmo) OT.tdr++;  //przeladowanie po 268ms
        else
        {
            if(PFNf_OTTimeout)
            {
                OT.tdr=0;                
                PFNf_OTTimeout=0;
                switch(OT.tptm)
                {
                    case 0:
                    {
                        return;
                    }
                    case 1:     //timeout dla procedury synchronizacji (MeasureOTPeriod)
                        //*********************
                        db=1;
                        InitOT();
                        return;
                        //********************
                        //db=1;
                    case 2:     //timeout dla procedury oczekiwania (WaitOTFrame)
                    {
                        db=2;
                        InitOT();
                        return;                        
                        //Nop();
                        //db=2;
                        //SetOTDeamon(0);      //start MeasureOTPeriod
                        //return;
                    }
                    case 3:     //timeout dla zbocza inicjujacego odczyt ramki                       
                    {
                        db=3;
                        InitOT();
                        return;
                        //db=3;
                        //SetOTDeamon(2);      //start WaitOTFrame
                        //return;
                    } 
                    case 4:     //timeout dla decyzji na odpowiedz                        
                    {
                        OT.ToACK=0;    
                        OT.ACKType=0;
                        SetOTDeamon(2);      //start WaitOTFrame
                        return;
                    }                    
                    default:
                    {
                        return;
                    }
                }

            }
        }
    }
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Korekta dla zmiennych szerokosci bitów informacji
SetOffset(void)
{
    volatile unsigned int offset,rtim;
    
    rtim=ReadTMR0();     //read timer0    
    offset=0xFFFF-rtim;
    OT.pop=0;
    if(offset<OT.offsetL)                   //za szybki pkt pomiaru
    {
        db++;
        OT.pop=offset-OT.offsetL;
        //ReloadTMR0(rtim+pop);    
    } 
    else
    if(offset<OT.offset2)                   //za wolny pkt pomiaru
    {
        OT.pop=offset-OT.offsetL;    
    }
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Przerwanie INT1
volatile static unsigned char INT0krok=0;
void SetINT0Krk(const unsigned char kr)
{
    INT0krok=kr;
}
void IntINT0(void)
{
    volatile unsigned int offset,rtim;
    //volatile signed int pop;
    
    switch(INT0krok)
    {
        case 0:
        {
            return;
        }
        case 1:     //IntMeasureOTPeriod
        {           
            if(PFNf_firstEdge)  //zbocze inicjujace pomiar?
            {                
                ReloadTMR0(0);      //reset timer0  
                WrtTMR0(0);         //reset timer0                
                PFNf_firstEdge=0;
                OT.data1=0;
            }
            else                //zbocze zamykajace pomiar
            {      
                OT.data1=ReadTMR0();     //read timer0
                IntMeasureOTPeriod();   //Synchronizacja
                PFNf_firstEdge=1;                
            } 
            return;
        }
        case 2:     //WaitOTFrame
        {
            OT.timeLow=Millis();   //nowy punkt startowy pomiaru czasu
            return;
        }         
        case 3:     //ReadOTFrame (pierwsze zbocze)
        {
            DetachINT();    
            StopOTTimeout();
            StartReadOTFrame();
            return;
        }        
        case 4:     //ReadOTFrame (kolejne bity)
        {
            OT.transitionCount++;
            SetOffset();                //poprawka dla pkt. pomiaru
            return;
        }
    }
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Przerwanie INT1
volatile static unsigned char INT1krok=0;
void SetINT1Krk(const unsigned char kr)
{
    INT1krok=kr;
}
void IntINT1(void)
{
    //volatile unsigned int offset,rtim;    
    //volatile signed int pop;
    
    switch(INT1krok)
    {
        case 0:
        {    
            return;
        }
        case 1:     //WaitOTFrame
        {
            OT.timeLow=Millis();       //nowy punkt startowy pomiaru czasu 
            return;
        }        
        case 2:     //ReadOTFrame (kolejne bity)
        {
            OT.transitionCount++;   
            SetOffset();                //poprawka dla pkt. pomiaru
            return;
        }
        
    }
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Deaktywacja przerwania INT0, INT1 na wejsciu OT
void DetachINT(void)
{
	INT0IE=0;			   		
	INT1IE=0;    
    SetINT0Krk(0);    
    SetINT1Krk(0);				  
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Aktywacja przerwania INT0 na wejsciu OT dla funkcji INTRsync (zbocze OT opadajace)
void AtachINTRsync(void)
{					   		
	INT0IE=0;			   		
	INT1IE=0;
    SetINT0Krk(1);    
    SetINT1Krk(0);
#if _NEG_IN==0    
	INTEDG0=0;          //zbocze opadajace
#else
	INTEDG0=1;          //zbocze narastajace
#endif
	INT0IE=1;    
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Aktywacja przerwania INT0,INT1 na wejsciu OT dla funkcji WaitOTFrame (zbocze OT opadajace)
void AtachINTWaitOTFrame(void)
{		
	INT0IE=0;			   		
	INT1IE=0;    
    SetINT0Krk(2);    
    SetINT1Krk(1);
#if _NEG_IN==0    
	INTEDG0=0;          //zbocze opadajace
	INTEDG1=1;          //zbocze narastajace    
#else
	INTEDG0=1;          //zbocze narastajace
	INTEDG1=0;          //zbocze opadajace    
#endif   
	INT0IE=1; 
	INT1IE=1;    
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Aktywacja przerwania INT0 na wejsciu OT dla funkcji WaitOTFrame (zbocze OT opadajace)
void AtachINTStartRec(void)
{					   		
	INT0IE=0;			   		
	INT1IE=0;
    SetINT0Krk(3);    
    SetINT1Krk(0);
#if _NEG_IN==0    
	INTEDG0=1;          //zbocze narastajace
#else
	INTEDG0=0;          //zbocze opadajace
#endif
	INT0IE=1;    
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Aktywacja przerwania INT0,INT1 na wejsciu OT dla funkcji INTTransition (oba zbocza)
void AtachINTTransition(void)
{					   		
    SetINT0Krk(4);    
    SetINT1Krk(2);
#if _NEG_IN==0    
	INTEDG0=0;          //zbocze opadajace
	INTEDG1=1;          //zbocze narastajace    
#else
	INTEDG0=1;          //zbocze narastajace
	INTEDG1=0;          //zbocze opadajace    
#endif   
	INT0IE=1; 
	INT1IE=1;    
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Pomiar wartosci sredniej parametrow czasowych ramki (przerwanie INT0)
volatile static unsigned char MOTkrk=0;
//volatile static unsigned char cnt1=0;
//volatile static unsigned char cnt2=0;
//volatile static int sum=0;
//volatile static unsigned int T2;
#define _RELOAD0    0
void StartMeasureOTPeriod(void)
{
    TMR0IE=0;
    MOTkrk=0;
    OT.cnt1=0;
    OT.cnt2=0;      
    OT.sum=0;
    OT.nRERR=0;
    PFNf_firstEdge=1;
    AtachINTRsync();                    //aktywacja przerwania INT0
    StartOTTimeout(_MEASURE_TIMO,1);    //restart procedury po _MEASURE_TIMO [usek] oczekiwania
    ReloadTMR0(_RELOAD0);               //przerwanie po pelnym przepelnieniu TIMER0    
    WrtTMR0(_RELOAD0);                  //wartosc przeladowania dla kolejnego przerwania TIMER0  
    TMR0IE=1;
}
void IntMeasureOTPeriod(void)
{
    //wartosc srednia odcinkow czasowych T2
    switch(MOTkrk)
    {
        case 0:
        {
            if(OT.cnt1<_SAMPLE_SIZE) 
            {
                OT.cnt2=0;
                OT.T2=0xffff;                
                MOTkrk=1;             
            }
            else    //wynik
            {
                TMR0IE=0;
                StopOTTimeout();                 
                DetachINT();  
                OT.otPeriod=(OT.sum*_TICK_TO_USEC)/_SAMPLE_SIZE;            //wartosc srednia [u sek]
                OT.preload=0xffff-(OT.otPeriod/_TICK_TO_USEC);              //wartosc przeladowania timera0;
                OT.preload2=0xffff-((OT.otPeriod/_TICK_TO_USEC)/2);         //wartosc przeladowania timera0;                
                OT.preload4=0xffff-((OT.otPeriod/_TICK_TO_USEC)/5);         //wartosc przeladowania wstepnego TIMER0 dla odczytu ramki                
                OT.offset2=0xffff-OT.preload2;                
                OT.offsetL=0xffff-OT.preload4;
                //db=1;                                
                SetOTDeamon(2);                              //inicjalizacja timer0 dla WaitOTFrame   
                MOTkrk=2;                 
                return;
            }
        }
        case 1:
        {            
            if(OT.cnt2<_SAMPLE_SIZE)    
            {             
                if(OT.data1!=0)    //nie nastapilo przepelnienie timera0?
                {
                    if(OT.data1<OT.T2)    //znajdz najkrotszy odcinek czasu miedzy kolejnymi narastajacymi zboczami
                    {
                        OT.T2=OT.data1;
                    }
                    OT.cnt2++;                      
                }       
            }
            else
            {
                OT.sum+=OT.T2;
                OT.cnt1++;     
                MOTkrk=0;
            }
            return;            
        }
        case 2:
        {
            return;
        }
    }
    return;
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Oczekiwanie na poczatek kolejnej ramki
volatile static unsigned char MOTFkrk=0;

#define _RELOAD1    0xffff-(2000/_TICK_TO_USEC)  //przerwanie co 2000us TIMER0
void StartWaitOTFrame(void)
{
    MOTFkrk=0; 
    StartOTTimeout(_WAITOT_TIMO,1);                 //restart do MeasureOTPeriod po _WAITOT_TIMO [usek] oczekiwania
    AtachINTWaitOTFrame();    
    ReloadTMR0(_RELOAD1);                           //przerwanie TIMER0 po 2000us    
    WrtTMR0(_RELOAD1);                              //wartosc przeladowania dla kolejnego przerwania TIMER0                       
    TMR0IE=1;                 
}
bit WaitOTFrame(void)
{   
//    volatile static unsigned long int timeLow;
    volatile unsigned long int waitTime;
    
    //WrtTMR0(_RELOAD1);                              //przerwanie co 2000us
    //wartosc srednia odcinkow czasowych T2
    switch(MOTFkrk)
    {
        case 0:
        {
            if(_IN_OT==_HIGH_OT) return 0;          //stan wysoki linii - oczekuj
            else 
            {             
                OT.timeLow=Millis();
                MOTFkrk=1;
            } 
        }
        case 1:
        {
            if(_IN_OT==_LOW_OT)                     //stan niski linii - trwa transmisja ramki lub przerwa miedzy ramkami MASTER
            {
                waitTime=Millis()-OT.timeLow;
                if(waitTime>= _MIN_WAIT_FOR_MASTER) //stan niski utrzymywany bez przerwy przez 100ms MASTER?
                {
                    StopOTTimeout();
                    //deaktywacja timer0
                    TMR0IE=0;                       //zatrzymanie przerwania od TIMER0
                    TMR0ON=0;                       //zatrzymanie TIMER0   
                    //WrtTMR0(OT.preload4);           //wartosc przeladowania wstepnego dla odczytu ramki                    
                    //ReloadTMR0(OT.preload4);        //wartosc przeladowania wstepnego dla odczytu ramki
                    //aktywacja przerwania INT
                    DetachINT();
                    AtachINTStartRec();             //aktywacja przerwania INT0 dla pierwszego zbocza ramki 
                    //aktywacja timeout dla oczekiwania na zbocze inicjujace odczyt 
                    StartOTTimeout(_FEDG_TIMO,3);   //ponow WaitOTFrame po _FEDG_TIMO [usek] oczekiwania na zbocze inicjujace
                    MOTFkrk=2;
                    return 1;                       //KONIEC - przygotuj sie do odczytu kolejnej ramki MASTER
                }                
            }
            else
            {
                MOTFkrk=0;                          //jestes w trakcie nadawania ramki przez MASTER, kontynuuj oczekiwanie
            }
            return 0;
        }
        case 2:
        {
            return 1;
        }
    }
    return 0;
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Oczekiwanie na poczatek kolejnej ramki
volatile static unsigned char ROTFkrk=0;
//volatile unsigned char cnt=0;
//volatile unsigned char parityOdd=0;
//volatile unsigned char previousTransitionCount=0;
//volatile long unsigned int tempFrame=0;
void StartReadOTFrame(void)
{ 
    //_WOUTX=1;       
    OT.cnt=0;
    OT.parityOdd=0; 
    OT.tempFrame=0;             
    OT.transitionCount=0; 
    OT.previousTransitionCount=0;    
    //INT0,INT1
    AtachINTTransition();
    //aktywacja timer0
    ReloadTMR0(OT.preload4);        //wartosc przeladowania wstepnego TIMER0 dla odczytu ramki
    WrtTMR0(OT.preload);            //wartosc przeladowania dla kolejnego przerwania TIMER0
    StopOTTimeout();
    TMR0IE=1;                       //aktywacja przerwania od TIMER0
    TMR0ON=1;                       //uruchomienie TIMER0 
    ROTFkrk=0;     
}
/*unsigned char ReadOTFrame(void)
{   
    switch(ROTFkrk)
    {
        case 0:                                       //przerwanie timer0 po _otPeriod/4 (zapobiega odczytowi stanu podczas zmiany sygnalu)
        {  
            if(OT.cnt>_FRAME_LENGTH)
            {
                _WOUTX=0;
                return 1;
            }            
            _WOUTX=0;
            WrtTMR0(OT.preload);            
            ROTFkrk=1;
            OT.cnt++;            
            return 0;
        }
        case 1:
        {     
            if(OT.cnt>_FRAME_LENGTH)
            {
                _WOUTX=0;
                return 1;
            }            
            _WOUTX=1;            
            WrtTMR0(OT.preload);            
            ROTFkrk=0;
            OT.cnt++;            
            return 0;
        }        
    }
    return 1;
}*/
unsigned char ReadOTFrame(void)
{
    volatile unsigned char sample;
  
    switch(ROTFkrk)
    {   
        case 0:                                       //przerwanie timer0 co _otPeriod
        {
#if _TEST_OT==1            
            if(_ROUTX) _WOUTX=0;
            else _WOUTX=1;          
#endif
            if(OT.cnt<_FRAME_LENGTH)
            {
                if((OT.cnt>0)&&(OT.transitionCount==OT.previousTransitionCount))
                {
                    //db=1;
                    if(OT.nRERR<0xff) OT.nRERR++;   //zwieksz licznik bledow odczytu
                    OT.errorMsg=1;     //bad data
                    OT.RFrame.war=0;
                    return 2;
                }
                else
                {
                    OT.previousTransitionCount=OT.transitionCount;
                }
#if _NEG_IN==0
                sample=_IN_OT;
#else
                if(_IN_OT) sample=0;
                else sample=1;
#endif         
                if(sample==1)
                {
                    OT.parityOdd=!OT.parityOdd; //cala ramka (razem z parity bit) musi miec parzysta liczbe jedynek
                }
                //First sample is start bit, last is stop bit
                if((OT.cnt==0)||(OT.cnt==(_FRAME_LENGTH-1)))
                {
                    if(sample!=1)
                    {
                        //db=2;
                        if(OT.nRERR<0xff) OT.nRERR++;   //zwieksz licznik bledow odczytu                        
                        OT.errorMsg=2;     //bad start/stop
                        OT.RFrame.war=0;
                        return 2;
                    }
                }
                else
                {
                    OT.tempFrame<<=1;
                    OT.tempFrame|=sample;
                }
                OT.cnt++;
                return 0;
            }
            else
            {
                ROTFkrk=1;
            }
        }
        case 1:
        {
            DetachINT();
#if _TEST_OT==1            
            _WOUTX=0;
#endif            
            if(OT.parityOdd==1)     //mimo dopelnienia w parity bit stwierdzono nieparzysta liczbe jedynek?
            {
                //db=3;
                if(OT.nRERR<0xff) OT.nRERR++;   //zwieksz licznik bledow odczytu                
                OT.errorMsg=3;
                OT.RFrame.war=0;
                return 2;                
            }
            else
            {
                //db=4;
                if(OT.nRERR>0) OT.nRERR--;  //zmniejsz licznik bledow odczytu
                OT.RFrame.war=OT.tempFrame;
            }
            return 1;
        }
    }
    return 1;
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
long unsigned int GetOTFrame(void)
{
    return OT.RFrame.war;
}
/*---------------------------------------------------------------------------------*/
unsigned char GetOTErrMsg(void)
{
    return OT.errorMsg;
}
/*---------------------------------------------------------------------------------*/
unsigned char GetOTPeriod(void)
{
    return OT.otPeriod;
}
/*---------------------------------------------------------------------------------*/
unsigned char GetOTMsgType(void)
{
    return OT.RFrame.msg_type;
}
/*unsigned char GetOTMsgType(void)
{
    volatile long unsigned int tempFrame;
    volatile unsigned char msgType;
    tempFrame=OT.RFrame.war;
    tempFrame>>=28;
    msgType=tempFrame&0b00000111;
    return msgType;
}*/
/*---------------------------------------------------------------------------------*/
unsigned char GetOTDataId(void)
{
    return OT.RFrame.data_id;
}
/*unsigned char GetOTDataId(void)
{
    volatile long unsigned int tempFrame;
    tempFrame=OT.RFrame.war;
    tempFrame>>=16;
    return (unsigned char) tempFrame;
}*/
/*---------------------------------------------------------------------------------*/
unsigned int GetOTRDataWord(void)
{
    return OT.RFrame.word0.war;
}
/*---------------------------------------------------------------------------------*/
unsigned int GetOTRDataByte0(void)
{
    return OT.RFrame.byte0.war;
}
/*---------------------------------------------------------------------------------*/
unsigned int GetOTRDataByte1(void)
{
    return OT.RFrame.byte1.war;
}
/*---------------------------------------------------------------------------------*/
unsigned char GetOTRParity(void)
{
    if(OT.RFrame.parity)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
/*unsigned char GetOTParity(void)
{
    volatile long unsigned int tempFrame;
    tempFrame=OT.RFrame.war;
    tempFrame>>=31;
    return (unsigned char)tempFrame;
}*/

/*---------------------------------------------------------------------------------*/
unsigned int GetOTIsMaster(void)
{
    if(GetOTMsgType()<=3) return 1;
    else return 0;
}
volatile unsigned char T0krok=0;
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Oczekiwanie na warunki konieczne przed wysylka ramki odpowiedzi
volatile static unsigned char MOTRkrk=0;

#define _RELOAD1    0xffff-(2000/_TICK_TO_USEC)  //przerwanie co 2000us TIMER0
void StartWaitOTResp(void)
{
    MOTRkrk=0; 
    OT.ToACK=1;                                     //znacznik ze odebrano paczke danych   
    OT.ACKType=0;                                   //wynik analizy paczki danych (decyzja o odpowiedzi lu rezygnacji)
    StartOTTimeout(_WAITTT_TIMO,4);                 //rezygnacja z odpowiedzi po _WAITTT_TIMO [msek] oczekiwania
    DetachINT();        
    ReloadTMR0(_RELOAD1);                           //przerwanie TIMER0 po 2000us    
    WrtTMR0(_RELOAD1);                              //wartosc przeladowania dla kolejnego przerwania TIMER0     
    TMR0IE=1;                 
}
unsigned char WaitOTResp(void)
{   
//    volatile static unsigned long int timeLow;
    volatile unsigned long int waitTime;
     
    //wartosc srednia odcinkow czasowych T2
    switch(MOTRkrk)
    {
        case 0:
        {            
            OT.timeLow=Millis();
            MOTRkrk=1;
        }
        case 1:
        {
            //if(_ROUTX) _WOUTX=0;
            //else _WOUTX=1;            
            waitTime=Millis()-OT.timeLow;
            if(waitTime<_MIN_WAIT_FOR_SLAVE) return 0;  //uplynal minimalny dozwolony czas do transmisji? 
            MOTRkrk=2;
        }
        case 2:
        {
            //_WOUTX=1;            
            return OT.ACKType;                          //wartosc zwracana po przygotowaniu odpowiedzi
        }     
    }
    return 0;
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Ustawienie bitu parzystosci w ramce odpowiedzi
void SetTFrameParityOdd(void)
{
    unsigned char i;
    long unsigned int mask;
    mask=0x40000000;
    
    OT.TFrame.parity=0;
    for(i=0;i<31;i++)
    {
        if(OT.TFrame.war&mask) OT.TFrame.parity=!OT.TFrame.parity;
        mask>>=1;        
    }
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Wysylanie ramki odpowiedzi
volatile static unsigned char TOTFkrk=0;

void StartTransmitOTFrame(void)
{       
    OT.cnt=0;
    OT.cnt2=0;   
    OT.tempFrame=OT.TFrame.war;    
    DetachINT();
    //aktywacja timer0
    ReloadTMR0(_TR_PRELOAD);        //przerwanie TIMER0 co 0.5msek   (0xFF84)  
    WrtTMR0(_TR_PRELOAD);           //wartosc przeladowania dla kolejnego przerwania TIMER0
    StopOTTimeout();
    TMR0IE=1;                       //aktywacja przerwania od TIMER0
    TMR0ON=1;                       //uruchomienie TIMER0 
    TOTFkrk=0;     
}
unsigned char TransmitOTFrame(void)
{
    volatile static unsigned char sample=0;
    //_WOUTX=0;
    switch(TOTFkrk)
    {   
        case 0:                                       //przerwanie timer0 co _otPeriod
        {           
            if(OT.cnt2)
            {
                OT.cnt2=0;
                _OUT_OT=!sample;
                //_WOUTX=!sample;
                return 0;
            }
            else
            if(OT.cnt<_FRAME_LENGTH)
            {
                OT.cnt2=1;       
                //First sample is start bit, last is stop bit
                if((OT.cnt==0)||(OT.cnt==(_FRAME_LENGTH-1)))
                {
                    sample=1;   //bit stop/start
                }
                else
                {
                    if(OT.tempFrame&0x80000000) sample=1;
                    else sample=0;
                    OT.tempFrame<<=1;
                }
                _OUT_OT=sample;                
                //_WOUTX=sample;                
                OT.cnt++;
                return 0;
            }
            else
            {
                _OUT_OT=0;
                //_WOUTX=1;
                OT.cnt2=0;
                TOTFkrk=1;
            }
        }
        case 1:
        {
            return 1;
        }
    }
    return 1;
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Procedura odbioru i wysylki ramki OT
void SetOTDeamon(const unsigned char tk)
{
    T0krok=tk;   
    IntOTDeamon();    //wykonaj wskazany krok;
}
void IntOTDeamon(void)
{
    volatile unsigned char stn=0; 
    
    //if(T0krok!=4) OT.data2=0;
    
    switch(T0krok)
    {
        //Pomiar parametrow przebiegu OT
        case 0:                             //(MeasureOTPeriod)
        {
            StopOTTimeout();            
            StartMeasureOTPeriod();         //aktywne przerwanie TIMER0, uruchomienie przerwania INT0
            T0krok=1;
            return;
        }
        case 1:                             //na potrzeby (MeasureOTPeriod) przerwanie po 268ms
        {
            WrtTMR0(_RELOAD0);              //przerwanie co 268ms TIMER0 
            PFNf_firstEdge=1;               //restart oczekiwania na zbocze inicjujace
            OT.data1=0;            
            return;
        }
        //Oczekiwanie na ramke OT
        case 2:                             //(WaitOTFrame)
        {
            StopOTTimeout();            
            StartWaitOTFrame();             //aktywne przerwanie TIMER0           
            T0krok=3;
            return;
        }
        case 3:                             //(WaitOTFrame)aktywne przerwanie TIMER0 (co 2ms), potem (WaitToFirstEdge) aktywne przerwanie INT0
        {
            if(!WaitOTFrame())return;    //oczekuj na poziom aktywnosci, przy aktywnym odczekaj jeszcze 100ms, a nastepnie przejdz dalej po pierwszym zboczu
            T0krok=4; 
            return;
        } 
        //Odczyt ramki OT
        case 4:                             //(ReadOTFrame)aktywne przerwanie TIMER0,aktywne przerwanie INT0,INT1
        {
            stn=ReadOTFrame();
            switch(stn)
            {
                case 0:     //oczekiwanie na zakonczenie procedury
                {
                    return;
                }
                case 1:     //sukces, przetwarzanie danych
                {
                    //_WOUTX=1; 
                    StopOTTimeout();
                    StartWaitOTResp();                 //aktywne przerwanie TIMER0
                    T0krok=5;
                    //db=1;
                    return;
                }
                default:    //blad odbioru
                {
                    if(OT.nRERR<5)                      //liczba bledow odczytu w normie?
                    {
                        StopOTTimeout();
                        StartWaitOTFrame();             //aktywne przerwanie TIMER0
                        T0krok=3;  
                    }
                    else                                //ponow synchronizacje
                    {
                        StopOTTimeout();                        
                        StartMeasureOTPeriod();         //aktywne przerwanie TIMER0, uruchomienie przerwania INT0
                        T0krok=1;                        
                    }
                    return;
                }
            }
        }
        //Oczekiwanie na analize ramki OT i odpowiedni moment wysylki (przerwanie TIMER0 co 2[ms])
        case 5:                             
        {               
            stn=WaitOTResp();
            switch(stn)
            {
                case 0:                             //czekaj na decyzje lub odpowiedz
                {
                    return;
                }
                case _OT_TO_RESP:                   //odpowiedz gotowa przejdz do transmisji
                {
                    StopOTTimeout();                    
                    SetTFrameParityOdd();           //ustaw bit dopelnienia do parzystosci
                    StartTransmitOTFrame();
                    T0krok=6;
                    return;
                }
                default:                            //rezygnacja z odpowiedzi, czekaj na kolejna ramke
                {
                    StopOTTimeout();
                    StartWaitOTFrame();             
                    T0krok=3;      
                    return;
                }
            }
        }  
        case 6:
        {
            if(!TransmitOTFrame())return;    //oczekuj na poziom aktywnosci, przy aktywnym odczekaj jeszcze 100ms, a nastepnie przejdz dalej po pierwszym zboczu
            StartWaitOTFrame();             
            T0krok=3;            
            return;            
        }
    }
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
#endif //(_EXT_BOARD>=3)