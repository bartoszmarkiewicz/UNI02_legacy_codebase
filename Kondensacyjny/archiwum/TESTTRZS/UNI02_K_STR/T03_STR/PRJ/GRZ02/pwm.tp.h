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
				unsigned _ENPWM:1;						//znacznik gotowosci danych do zapisu w PWM
				unsigned _ENOSC:1;
				unsigned tpGZ:1;						//0-gaz ziemny, 1-gaz plynny
                unsigned _fl0bt3:1;			//
                unsigned _fl0bt4:1;			//
                unsigned _fl0bt5:1;			//
                unsigned _fl0bt6:1;			//
                unsigned _fl0bt7:1;			//
            };
    };                    
				volatile near unsigned char BufPWM;		//bufor stanu wypelnienia
				volatile near unsigned char BufOSC;		//bufor stanu oscylacji	
				near unsigned char NewOSC;				//nowa wartosc oscylacji	
				near unsigned char NewPWM;				//wartosc wypelnienia po obrobce
				
//--------
				unsigned char MDMOD;					//dolny prog modulacji
				unsigned char MGMOD;					//gorny prog modulacji
				unsigned char MAXOSC;					//maksymalna amplituda oscylacji								
				unsigned char MINOSC;					//minimalna amplituda oscylacji
				unsigned char MAD_MOD;					//dolny margines dla oscylacji
				unsigned char MAG_MOD;					//gorny margines dla oscylacji
				unsigned char GWMSTART;					//graniczna wartosc nastawy mocy startowej
				unsigned char MDSTART;					//dolna granica wartosci PWM dla mocy startowej
				unsigned char MGSTART;					//gorna granica wartosci PWM dla mocy startowej
				unsigned char GWMMAXCO;					//graniczna wartosc nastawy mocy maksymalnej CO
				unsigned char MDMAXCO;					//dolna granica wartosci PWM dla mocy maksymalnej CO
				unsigned char MGMAXCO;					//gorna granica wartosci PWM dla mocy maksymalnej CO				
				unsigned char GWMMAXCW;					//graniczna wartosc nastawy mocy maksymalnej CW
				unsigned char MDMAXCW;					//dolna granica wartosci PWM dla mocy maksymalnej CW
				unsigned char MGMAXCW;					//gorna granica wartosci PWM dla mocy maksymalnej CW
				unsigned char FIRSTPB;					//domyslna wartosc pierwszego pobudzenia
				unsigned char PROP1;					//prog wypelnienia dla czlonu proporcjonalnego CO przy grzaniu CW
				unsigned char PROP2;					//prog wypelnienia dla czlonu proporcjonalnego CO przy grzaniu CW
				unsigned char PROP3;					//prog wypelnienia dla czlonu proporcjonalnego CO przy grzaniu CW
//--------
				unsigned char MSTART;					//moc startowa
				unsigned char MMAXCO;					//moc maksymalna CO
				unsigned char MMAXCW;					//moc maksymalna CW
				unsigned char PGD;						//[0...100%] modyfikator mocy MAXPWM z regulatora pogodowego
				unsigned char MAXPWM;					//moc maksymalna dla kotlow jednofunkcyjnych
														//lub pozostałych przy grzaniu CO
				
				}DtPWM;
//----------------------------------------------------------
#endif	/*_PWM_TPH*/

