/*pwm.tp.h*/
//typy danych dla pliku pwm.c
//----------------------------------------------------------
#ifndef	_PWM_TPH
#define	_PWM_TPH
//----------------------------------------------------------
typedef struct	{
				//PWM1
				unsigned _ENPWM:1;						//znacznik gotowosci danych do zapisu w PWM
				volatile near unsigned char BufPWM;		//bufor stanu wypelnienia
				near unsigned char NewPWM;				//wartosc wypelnienia po obrobce

				//PWM2
				unsigned _ENPWM2:1;						//znacznik gotowosci danych do zapisu w PWM
				volatile near unsigned char BufPWM2;	//bufor stanu wypelnienia
				near unsigned char NewPWM2;				//wartosc wypelnienia po obrobce

				unsigned char HIPWM;					//bezwzgledna gorna granica wypelnienia PWM				
//--------
				unsigned char VLO_PWM;					//wypelnienie ponizej ktorego wentylator nie pracuje                
				unsigned char MDMOD;					//dolny prog modulacji
				unsigned char DKNF_MDMOD;				//domyslny dolny prog modulacji zdefiniowany w konfiguracji
				unsigned char DV16_MDMOD;				//czasowy, wstepny dolny prog modulacji dla MDMOD(<1600obr/min)
				unsigned char MGMOD;					//gorny prog modulacji
				//unsigned char GWMSTART;				//graniczna wartosc nastawy mocy startowej
				unsigned char MDSTART;					//dolna granica wartosci PWM dla mocy startowej
				unsigned char MGSTART;					//gorna granica wartosci PWM dla mocy startowej
				//unsigned char GWMMAXCO;				//graniczna wartosc nastawy mocy maksymalnej CO
				unsigned char MDMAXCO;					//dolna granica wartosci PWM dla mocy maksymalnej CO
				unsigned char MGMAXCO;					//gorna granica wartosci PWM dla mocy maksymalnej CO
		
				//unsigned char GWMMAXCW;				//graniczna wartosc nastawy mocy maksymalnej CW
				unsigned char MDMAXCW;					//dolna granica wartosci PWM dla mocy maksymalnej CW
				unsigned char MGMAXCW;					//gorna granica wartosci PWM dla mocy maksymalnej CW
				unsigned char FIRSTPB;					//domyslna wartosc pierwszego pobudzenia
				unsigned char PROP1;					//prog wypelnienia dla czlonu proporcjonalnego CO przy grzaniu CW
				unsigned char PROP2;					//prog wypelnienia dla czlonu proporcjonalnego CO przy grzaniu CW
				unsigned char PROP3;					//prog wypelnienia dla czlonu proporcjonalnego CO przy grzaniu CW
				unsigned char APROC;					//procentowy wydatek modulatora
//--------
				unsigned _TMAXCO:1;						//znacznik czasowej pracy na obni¿onej mocy maksymalnej
				unsigned char MSTART;					//moc startowa
				unsigned char MMAXCO;					//moc maksymalna CO
				unsigned char MMAXCW;					//moc maksymalna CW
				unsigned char PGD;						//[0...100%] modyfikator mocy MAXPWM z regulatora pogodowego
				unsigned char MINPWM;					//moc minimalna
				unsigned char MAXPWM;					//moc maksymalna dla kotlow jednofunkcyjnych
														//lub pozosta³ych przy grzaniu CO
				
				}DtPWM;
//----------------------------------------------------------
#endif	/*_PWM_TPH*/

