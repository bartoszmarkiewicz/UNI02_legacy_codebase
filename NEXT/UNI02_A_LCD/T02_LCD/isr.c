/*--------------------------------------------------------------*/
/*isr.c*/
/*obsluga przerwan*/
/*--------------------------------------------------------------*/
//#include <pic18.h>
#include <xc.h>
#include "global.h"				//parametry globalne
#include "isr.h"				//parametry lokalne
#include "ot.tp.h"				//parametry lokalne
volatile unsigned char x,y;
extern volatile unsigned char xxy,zzz,vvv;
extern volatile tpOTDta OT;
//-------------------------------------------------------------------------
//HIHG priority interrupt
void interrupt H_ISR(void)
{
    //komunikacja OT - timer0
    if((TMR0IF)&&(TMR0IE))
    {
#if (_EXT_BOARD>=3)        
        ReloadTMR0(OT.prelTMR0+OT.pop);             //gdzie OT.pop dla odbioru danych stanowi poprawke dla pkt pomiaru
#endif        
        OT.pop=0;
        TMR0IF=0;
#if (_EXT_BOARD>=3)    
        IntOTDeamon();                              //synchronizacja, odbior i transmisja ramki OT            
#endif
        return;
    }    
    //komunikacja OT - INT0
    if((INT0IF) && (INT0IE))          //transmisja OT - zbocze opadajace
    {
        INT0IF=0;
        //if(_OUTX) _OUTX=0;
        //else _OUTX=1;            
#if (_EXT_BOARD>=3)            
        IntINT0();
#endif            
        return;
    }
    //komunikacja OT - INT1
    if((INT1IF) && (INT1IE))          //transmisja OT - zbocze narastajace
    {
        INT1IF=0; 
#if (_EXT_BOARD>=3)            
        IntINT1();
#endif
        return;
    }
         
        
    //Timer zwiazany z protokolami transmisji danych
    if((TMR3IF)&&(TMR3IE))
    {
            TMR3IF=0;
            ReloadTimer3();         //obsluga zegara Timer3
            IncRSTimer();           //obsluga timerow modulu MGS_RS
            #if (_EXT_BOARD>=2)
                IntLIN1Tim();           //obsluga zegarow transmitera LIN1
            #endif
            return;
    }
    
#if (_EXT_BOARD!=0)
        //Komunikacja z plyta UNI-02
        //UART2
        if((RC2IF)&&(RC2IE))		//odbior RS232 (RCIF jest zerowane sprzetowo)
        {
                IntUSARTRC();
                return;
        }
        //UART2
        if((TX2IF)&&(TX2IE))		//transmisja RS232 (TXIF jest zerowane sprzetowo)
        {
                IntUSARTTR();
                return;
        }
#if (_EXT_BOARD>=2)
        //Komunikacja poprzez magistrale LIN z managerem kaskady lub regulatorem RT
        //UART1 (LIN SLAVE)
        if((RC1IF) && (RC1IE))          //odbior RS232 (RCIF jest zerowane sprzetowo)
        {
                IntUSART1SRC();             //slave
                return;
        }
       //UART1 (LIN SLAVE)
        if((TX1IF) && (TX1IE))          //transmisja RS232 (TXIF jest zerowane sprzetowo)
        {
                IntUSART1STR();             //slave
                return;
        }
#endif
  
#else
    //Komunikacja z plyta UNI-02
    //UART1
    if((RCIF)&&(RCIE))		//odbior RS232 (RCIF jest zerowane sprzetowo)
    {
            IntUSARTRC();
            return;
    }
    //UART1
    if((TXIF)&&(TXIE))		//transmisja RS232 (TXIF jest zerowane sprzetowo)
    {
            IntUSARTTR();
            return;
    }
#endif      
}
//-------------------------------------------------------------------------
//LOW priority interrupt
void interrupt low_priority L_ISR(void)
{
    if((LCDIF)&&(LCDIE))	//obsluga przerwania od LCD
    {
            LCDIF=0;
            return;
    }
    if((TMR1IF)&&(TMR1IE))
    {
            TMR1IF=0;
            ReloadTimer1();		//obsluga zegara Timer1
            IncRTimer();		//obsluga zegarow RT
            IncKeyTimer();		//obsluga zegarow klawiszy
            //IncRSTimer();		//obsluga timerow modulu MGS_RS
            IncRingTimer();		//obsluga timerow kolejkowania procedur
            RefreshWSW();		//obsluga wyswietlacza
            StabPort();			//stabilizacja stanu okreslonych sygnalow
            return;
    }
    if((TMR2IF)&&(TMR2IE))      //przerwanie co 1/4 msek
    {
            TMR2IF=0;
            IntPWM2();                  //pwm2 - podswietlenie
#if (_EXT_BOARD>=3)            
            IntMillis();        //zliczanie milisekund poczawszy od zalaczenia zasilania (przeladowanie po 50 dniach)  
            IntOTTimeout();     //restart procedury OT do stanu oczekiwania na ramke po zadanym czasie (1=1ms, 0xFFFF=65,5 sek))
#endif            
            return;
    }
    if((CCP2IF)&&(CCP2IE))
    {
            CCP2IF=0;
            return;
    }
    if((SSPIF)&&(SSPIE))	//obsluga przerwania od I2C
    {
            SSPIF=0;
            I2CSlaveTR();
            return;
    }
}
//-------------------------------------------------------------------------
