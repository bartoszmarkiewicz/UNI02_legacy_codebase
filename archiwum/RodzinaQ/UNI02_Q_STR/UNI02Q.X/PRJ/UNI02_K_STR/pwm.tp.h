/*pwm.tp.h*/
//typy danych dla pliku pwm.c
//----------------------------------------------------------
#ifndef	_PWM_TPH
#define	_PWM_TPH
//----------------------------------------------------------
typedef struct	{
    union   {
            unsigned char flag0;
            struct
            {    
				unsigned _ENPWM3:1;						//znacznik gotowosci danych do zapisu w PWM
                unsigned _SPREC3:1;                      //znacznik wartosci 10 bit
                unsigned _fl0bt2:1;			//
                unsigned _fl0bt3:1;			//
                unsigned _fl0bt4:1;			//
                unsigned _fl0bt5:1;			//
                unsigned _fl0bt6:1;			//
                unsigned _fl0bt7:1;			//
            };
    };          
    union   {
            unsigned char flag1;
            struct
            {    
				unsigned _ENPWM2:1;						//znacznik gotowosci danych do zapisu w PWM
                unsigned _fl1bt1:1;			//
                unsigned _fl1bt2:1;			//                
                unsigned _fl1bt3:1;			//
                unsigned _fl1bt4:1;			//
                unsigned _fl1bt5:1;			//
                unsigned _fl1bt6:1;			//
                unsigned _fl1bt7:1;			//                
            };
    };                
                
                //PWM3
				//volatile near unsigned char BufPWM3;		//bufor stanu wypelnienia
				//near unsigned char NewPWM3;				//wartosc wypelnienia po obrobce
    
				volatile unsigned int BufPWM3;          //bufor stanu wypelnienia
				unsigned int NewPWM3;                   //wartosc wypelnienia po obrobce
    
				//PWM2
				volatile near unsigned char BufPWM2;	//bufor stanu wypelnienia
				near unsigned char NewPWM2;				//wartosc wypelnienia po obrobce

				unsigned char HIPWM;					//bezwzgledna gorna granica wypelnienia PWM2
                
                unsigned int HIPWM3;					//bezwzgledna gorna granica wypelnienia PWM3 (10bit)	
//--------
				unsigned char VLO_PWM;					//wypelnienie ponizej ktorego wentylator nie pracuje                
				unsigned char MDMOD;					//dolny prog modulacji
				unsigned int HMDMOD;					//dolny prog modulacji (10bit)                
				unsigned char DKNF_MDMOD;				//domyslny dolny prog modulacji zdefiniowany w konfiguracji
				unsigned char DV16_MDMOD;				//czasowy, wstepny dolny prog modulacji dla MDMOD(<1600obr/min)
				unsigned char MGMOD;					//gorny prog modulacji
				unsigned int HMGMOD;					//gorny prog modulacji (10bit)               
				unsigned char MDSTART;					//dolna granica wartosci PWM dla mocy startowej
				unsigned char MGSTART;					//gorna granica wartosci PWM dla mocy startowej
				unsigned char MDMAXCO;					//dolna granica wartosci PWM dla mocy maksymalnej CO
				unsigned char MGMAXCO;					//gorna granica wartosci PWM dla mocy maksymalnej CO
		
				unsigned char MDMAXCW;					//dolna granica wartosci PWM dla mocy maksymalnej CW
				unsigned char MGMAXCW;					//gorna granica wartosci PWM dla mocy maksymalnej CW
				unsigned char FIRSTPB;					//domyslna wartosc pierwszego pobudzenia
				unsigned char PROP1;					//prog wypelnienia dla czlonu proporcjonalnego CO przy grzaniu CW
				unsigned char PROP2;					//prog wypelnienia dla czlonu proporcjonalnego CO przy grzaniu CW
				unsigned char PROP3;					//prog wypelnienia dla czlonu proporcjonalnego CO przy grzaniu CW
				unsigned char APROC;					//procentowy wydatek modulatora
//--------
				unsigned _TMAXCO:1;						//znacznik czasowej pracy na obniżonej mocy maksymalnej
				unsigned char MSTART;					//moc startowa
				unsigned char MMAXCO;					//moc maksymalna CO
				unsigned char MMAXCW;					//moc maksymalna CW
				unsigned char PGD;						//[0...100%] modyfikator mocy MAXPWM z regulatora pogodowego
				unsigned char MINPWM;					//moc minimalna
				unsigned char MAXPWM;					//moc maksymalna dla kotlow jednofunkcyjnych
														//lub pozostałych przy grzaniu CO
				
				}DtPWM;
//----------------------------------------------------------
#endif	/*_PWM_TPH*/

