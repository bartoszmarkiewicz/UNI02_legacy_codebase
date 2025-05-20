/*pwm.tp.h*/
//typy danych dla pliku pwm.c
//----------------------------------------------------------
#ifndef	_PWM_TPH
#define	_PWM_TPH
//----------------------------------------------------------
				volatile bit PWMf_ENPWM2;						//znacznik gotowosci danych do zapisu w PWM
typedef struct	{
				//PWM2
				//unsigned _ENPWM2:1;						//znacznik gotowosci danych do zapisu w PWM
				volatile near unsigned char BufPWM2;	//bufor stanu wypelnienia
				near unsigned char NewPWM2;				//wartosc wypelnienia po obrobce
				unsigned char HIPWM;					//bezwzgledna gorna granica wypelnienia PWM								
				unsigned char LLed;						//wartosc obliczonego % wypelnienia dla diody
				}DtPWM;
//----------------------------------------------------------
#endif	/*_PWM_TPH*/

