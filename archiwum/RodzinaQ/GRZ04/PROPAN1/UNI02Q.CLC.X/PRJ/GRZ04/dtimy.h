/*mmy.h*/
#ifndef	_MMY_H
#define	_MMY_H
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
#define _AdrMMY 0b11001101			//adres modulu MMY
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Identyfikator typu kotla
#define UNICO2		0XC0			//unico (2)
#define MMAXTP_OLD	0XC1			//minimax turbo przeplywowy (old)
#define MMAXTP		0XC2			//minimax turbo przeplywowy
#define MMAXTZ		0XC3			//minimax turbo zasobnikowy
#define MMAXP		0XC4			//minimax plus
#define INWT		0XC5			//inwesterm
#define INWTRB		0XC6			//inwesterm turbo
#define INWTIT		0XC7			//inwesterm (wloski)
#define MMAXT1F		0XC8			//minimax turbo jednofunkcyjny
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Identyfikatory kolejnych odebranych bajtow
#define N_ADR_STER		0
#define N_KOD_STER		1	
#define N_PCO			2
#define N_CO			3		
#define N_PCW			4
#define N_CW			5
#define N_CS			6	
#define N_STR_PWM		7
#define N_MAX_PWM		8
#define N_BUF_PWM		9
#define N_ST_AD1		10
#define N_ST_AD2		11
#define N_ST_AD3		12
#define N_PORTC			13
#define N_PORTD			14
#define N_ST_GLOBAL0	15
#define N_ST_GLOBAL2	16
#define N_ST_GLOBAL3	17
#define N_ST_GLOBAL4	18
#define N_TPRZ_CW		19
#define N_ST0_WSW		20
#define N_ERR_BTY		21
#define N_CHK			22
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Oznaczenia kolejnych bitow danych odebranych z modulu MMY
//PORTC
#define b_PLOMIEN		0	
#define b_ALARM			1
#define b_VNT			3		//KOPIA VNT Z PORTE,1
#define b_PRZEPLYW_CO	5
#define b_BRAK_CIAGU	6
#define b_MOD_ON		7
//PORTD
#define b_DTF			0
#define b_GRZEJ			1
#define b_KCO_KCW		2
#define b_POMPA			3
#define b_RESET_O		4
#define b_SW_CONTROL	5
#define b_ZIMA			6
//ST_GLOBAL0
#define b_S_CW			0		//! wymuszenie CW
#define b_S_RT			1		//wymuszenie RT
#define b_S_ALARM		2		//sygnal ALARM
#define b_S_PRZ_CO		3		//sygnal PrzeplywCO
#define b_S_BRAK_CI		4		//sygnal BrakCiagu
#define b_S_MOD_ON		5		//sygnal BrakModulatora
#define b_S_RESET		6		//sygnal ResetON
#define b_S_PLOMIEN		7		//sygnal PLOMIEN
//ST_GLOBAL1
#define b_F_STB		0		//znacznik zmiany sygnalu STB
#define b_F_ALARM	1		//znacznik zmiany sygnalu ALARM
#define b_F_PRZ_CO1	2		//znacznik1 zmiany sygnalu PrzeplywCO
#define b_F_PRZ_CO2	3		//znacznik2 zmiany sygnalu PrzeplywCO
#define b_F_BRAK_CI	4		//znacznik zmiany sygnalu BrakCiagu
#define b_F_MOD_ON	5		//znacznik zmiany sygnalu BrakCiagu
#define b_F_RESE	6		//znacznik zmiany sygnalu RESET
#define b_F_PLOMIEN	7		//znacznik zmiany sygnalu PLOMIEN
//ST_GLOBAL2
#define b_OPDG		0		//!ogrzewanie podlogowe
#define b_GP		1		//!rodzaj gazu
#define b_OK_WRT	2		//zapis do pamieci gotowy
#define b_S_ON_DZ	3		//sygnal _ON_DZ
#define b_F_ON_DZ	4		//znacznik zmiany sygnalu _ON_DZ
#define b_S_SWC		5		//sygnal SW_CONTROL
#define b_F_SWC		6		//znacznik zmiany sygnalu SW_CONTROL
#define b_S_STB		7		//sygnal STB
//ST_GLOBAL3
#define b_ROZRUCH		0
#define b_WYLACZ		1
#define b_RUSZ_CIAG		2
#define b_RUSZ_POMPE	3
#define b_CZEKAJ_NTC	4
#define b_RUSZ_PLOMIEN	5
#define b_RUSZ_STB		6
#define b_BLOKUJ_3M		7
//ST_GLOBAL4
#define b_RUSZ_48		0	
#define	b_ROZRUCH_AWR	1
#define b_MODULUJ		2
#define	b_OST_STB		3
#define b_OST_PLM		4
#define b_VWYBIEG		5
#define b_PPOMP			6
#define b_ZMKCOCW		7
//ST_GLOBAL5
#define b_DE_VNT		0		//zwloka na ustalenie stanu czujnika ciagu
#define b_F_PLOMIEN2	3		//znacznik zmiany sygnalu PLOMIEN
#define b_RUSZ_PMPDZ	4		//awaryjny wybieg pompy dla temp. dyzurnej
#define b_START			7		//znacznik pierwszego wybiegu pompy
//ST0_WSW
#define b_MSG_A1	0	
#define b_MSG_A2	1	
#define b_MSG_A3	2	
#define b_MSG_A4	3	
#define b_MSG_A5	4	
#define b_MSG_A7	5	
#define b_MSG_A8	6	
#define b_MSG_A9	7	
//ST_AD1
#define b_ERR_CO	0
#define b_ERR_CW	1
#define b_NEW_PCO	2
#define b_NEW_PCW	3
#define b_TERR_CSN	4
#define b_TLO_CSN	5
#define b_THI_CSN	6
#define b_TWRK_CSN	7
//ST_AD2
#define b_SU_CO		0
#define b_AD_CO		1
#define b_ON_DZ		2
#define b_ON_POMP	3
#define b_SU_CW		4
#define b_AD_CW		5
#define b_S_DTF		6
#define b_F_DTF		7
//ST_AD3
#define b_HOT_CO	0
#define b_NO_CSN	1
#define b_F_LO_CSN	2
#define b_S_LO_CSN	3
#define b_BLOK_15S	5

/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
typedef struct 	{
				unsigned char Adres;				//adres odbiornika /nalezy okreslic przed odbiorem danych/
//------
				unsigned char KodSter;				//wuslany przez odbiornik kod identyfikujacy sterownik
				unsigned char PCO;
				unsigned char CO;
				unsigned char PCW;
				unsigned char CW;
				unsigned char CS;
				unsigned char STR_PWM;
				unsigned char MAX_PWM;
				unsigned char BUF_PWM;
				unsigned char ST_AD1;
				unsigned char ST_AD2;
				unsigned char ST_AD3;
				unsigned char PORTC;
				unsigned char PORTD;
				unsigned char ST_GLOBAL0;
				unsigned char ST_GLOBAL2;
				unsigned char ST_GLOBAL3;
				unsigned char ST_GLOBAL4;
				unsigned char TPRZ_CW;
				unsigned char ST0_WSW;
				unsigned char ERR_BTY;
				unsigned char CHK;					//suma kontrolna
				}tpMMY;
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
#endif //_MMY_H
