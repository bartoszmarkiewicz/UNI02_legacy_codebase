/*pwm.tp.h*/
//typy danych dla pliku pwm.c
//----------------------------------------------------------
#ifndef	_PWM_TPH
#define	_PWM_TPH
//----------------------------------------------------------
        volatile bit PWMf_ENPWM;						//znacznik gotowosci danych do zapisu w PWM
        volatile bit PWMf_ENOSC;
        volatile bit PWMf_tpGZ;						//0-gaz ziemny, 1-gaz plynny
        volatile bit PWMf_ENPWM2;						//znacznik gotowosci danych do zapisu w PWM2

typedef struct sDtPWM{
				//PWM1
    /*struct{
        unsigned _ENPWM:1;						//znacznik gotowosci danych do zapisu w PWM
        unsigned _ENOSC:1;
        unsigned _tpGZ:1;						//0-gaz ziemny, 1-gaz plynny
        unsigned _ENPWM2:1;						//znacznik gotowosci danych do zapisu w PWM2
        
        unsigned int flags0;   
    };*/
                
    volatile near unsigned char BufPWM;		//bufor stanu wypelnienia
    volatile near unsigned char BufOSC;		//bufor stanu oscylacji	
    volatile near unsigned char NewOSC;				//nowa wartosc oscylacji	
    volatile near unsigned char NewPWM;				//wartosc wypelnienia po obrobce

    //PWM2

    volatile near unsigned char BufPWM2;	//bufor stanu wypelnienia
    volatile near unsigned char NewPWM2;				//wartosc wypelnienia po obrobce

    volatile unsigned char HIPWM;					//bezwzgledna gorna granica wypelnienia PWM					
//--------
    volatile unsigned char MDMOD;					//dolny prog modulacji
    volatile char DKNF_MDMOD;				//domyslny dolny prog modulacji zdefiniowany w konfiguracji
    volatile char DV16_MDMOD;				//czasowy, wstepny dolny prog modulacji dla MDMOD
    volatile char MGMOD;					//gorny prog modulacji
    volatile char MAXOSC;					//maksymalna amplituda oscylacji								
    volatile char MINOSC;					//minimalna amplituda oscylacji
    volatile char MAD_MOD;					//dolny margines dla oscylacji
    volatile char MAG_MOD;					//gorny margines dla oscylacji
    volatile char GWMSTART;					//graniczna wartosc nastawy mocy startowej
    volatile char MDSTART;					//dolna granica wartosci PWM dla mocy startowej
    volatile char MGSTART;					//gorna granica wartosci PWM dla mocy startowej
    volatile char GWMMAXCO;					//graniczna wartosc nastawy mocy maksymalnej CO
    volatile char MDMAXCO;					//dolna granica wartosci PWM dla mocy maksymalnej CO
    volatile char MGMAXCO;					//gorna granica wartosci PWM dla mocy maksymalnej CO				
    volatile char GWMMAXCW;					//graniczna wartosc nastawy mocy maksymalnej CW
    volatile char MDMAXCW;					//dolna granica wartosci PWM dla mocy maksymalnej CW
    volatile char MGMAXCW;					//gorna granica wartosci PWM dla mocy maksymalnej CW
    volatile char FIRSTPB;					//domyslna wartosc pierwszego pobudzenia
    volatile char PROP1;					//prog wypelnienia dla czlonu proporcjonalnego CO przy grzaniu CW
    volatile char PROP2;					//prog wypelnienia dla czlonu proporcjonalnego CO przy grzaniu CW
    volatile char PROP3;					//prog wypelnienia dla czlonu proporcjonalnego CO przy grzaniu CW
    //--------
    volatile char MSTART;					//moc startowa
    volatile char MMAXCO;					//moc maksymalna CO
    volatile char MMAXCW;					//moc maksymalna CW
    volatile char PGD;						//[0...100%] modyfikator mocy MAXPWM z regulatora pogodowego
    volatile char MINPWM;					//moc minimalna
    volatile char MAXPWM;					//moc maksymalna dla kotlow jednofunkcyjnych
                                              //lub pozosta³ych przy grzaniu CO
}DtPWM;
//----------------------------------------------------------
#endif	/*_PWM_TPH*/

