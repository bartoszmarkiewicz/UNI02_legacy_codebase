/*global.h*/
//Parametry globalne
//wersja takze dla procesorow serii K
#ifndef	_GLOBAL_H
#define	_GLOBAL_H 1
//--------------------------------------------------------------
//--------------------------------------------------------------
#if	defined(_18C242) || defined(_18C252) || defined(_18F248) || defined(_18F258)
#define _PIC_PORT 3			//liczba portow procesora
#endif
#if defined(_18F442) || defined(_18F452) || defined(_18F448) || defined(_18F458)||defined(_18F4525)||defined(_18F46K20)
#define _PIC_PORT 5			//liczba portow procesora
#endif
#ifndef _PIC_PORT
#define _PIC_PORT 0			//wywolanie komunikatu bledu dla niezdefiniowanej rodziny PIC
#endif

#define _EXT_OSC 8 			//czestotliwosc oscylatora [MHz]
#define _PLL 1				//modul PLL (0-nieaktywny,1-aktywny)
//--------------------------------------------------------------
//--------------------------------------------------------------
//UART
#define _MY_ADDR			0x02  //adres wlasny 
//Podstawowe parametry 
//timery
//*************************************************************
//RTdS
#define _RTON	0			//pwron.c - timer zwloki po zalaczeniu zasilania
#define _RTZKP	1			//main.c - timer zmiany kierunku obrotow
#define _RTMOD	2			//rgl.c - timer procedury modulacji
#define _RTBL15 3			//main.c - timer blokady 15 sek.
#define _RTRSPL 4			//main.c - timer blokady zerowania licznika prob E1
#define _RTPCO  5			//pomiar.c - timer zapisu nastawy PCO
#define _RTPCW  6			//pomiar.c - timer zapisu nastawy PCW
#define _RTPFN  7			//pomiar.c - timer zapisu pozycji PFN
#define _RTPMOD	8			//pompa.c - timer procedury modulacji dla pompy
#define _RTSET	9			//konsola.c
#define _RTPCS	10			//konsola.c
#define _RTSTR2	11			//konsola.c
//*************************************************************
//RTS
#define _RTKONF1 0			//konfig.c - timer konfiguracji
#define _RTROZ	1			//main.c - timer procedury rozruchu
#define _RTRPL	2			//main.c - timer procedury rusz plomien
#define _RTRCI	3			//main.c - timer procedury rusz ciag
#define _RTWB48	4			//main.c - timer wybiegu pompy dla proc 48h
#define _RTVWB	5			//main.c - timer wybiegu wentylatora
#define _RTWYL	6			//main.c - timer wybiegu pompy
#define _RTSRV0 7			//main.c - timer wejscia do funkcji serwisowej
#define _RTSTR 8			//main.c - timer proby parametrow sieci
#define _RTRSCI 9			//main.c - timer procedury zerowania licznika ponowien ciagu
#define _RTKAL 10			//main.c - timer procedury zerowania licznika ponowien ciagu
#define _RTDPR 11			//main.c - timer procedury zwloki na reakcje czujnika przep WG
#define _RTRPR 12			//main.c - timer procedury rusz przep³yw WG
#define _RTRSCI2 13			//main.c - timer procedury zerowania licznika ponowien ciagu
#define _RTMPMP 14			//main.c - timer procedury modulacji wybiegiem pompy
#define _RTWPWM 15			//main.c - timer wstepnego progu dolnej granicznej wartosci PWM dla grzania CO
//*************************************************************
//RTM
#define _RTKONF3 0			//konfig.c - timer aktywnosci konfiguracji
#define _RTSRV1 1			//main.c - timer aktywnosci funkcji serwisowej
#define _RTBL3	2			//main.c - timer blokady L3
#define _RTRCI2 3			//main.c - timer procedury rusz ciag
#define _RTMAL 4			//main.c - timer trwania aktywnosci funkcji AL
#define _SRVRTANL 5			//main.c - timer trwania aktywnosci funkcji AL w trybie kontrolnym
#define _RTRCI3 6			//main.c - timer licznika wystapien zaniku ciagu
//*************************************************************
//RTG
#define _RTR48 0			//main.c - timer aktywacji wybiegu pompy co 48h
#define _RTANL 1			//main.c - timer aktywacji grzania AL
#define _RTR48Z3 2			//main.c - timer aktywacji wybiegu pompy co 48h w obiegu CW
//*************************************************************
//--------------------------------------------------------------

//--------------------------------------------------------------
//Wyjscia 
//#define CLK	RB6			
//#define STR	RB5			
//#define DTA	RB4			
//Wejscia
//#define WCS	RB7 
//----------------------------------------------
//Oznaczenia WE/WY procesora
//------
//PORTA
							//RA0 COTemp (AN0)
							//RA1 CWTemp (AN1)
							//RA2 ZWTemp (AN2)
							//RA3 CisnCO (AN3)
#define _PRZ_CW		PORTAbits.RA4		//WE
							//RA5 coTemp2(AN4)
#define _CIG_NO		PORTAbits.RA6		//WE
#define _STB		PORTAbits.RA7		//WE
//------
//PORTB
#define _TAC_INT	PORTBbits.RB0		//WE	
#define _KY1		PORTBbits.RB1		//WE
#define _KY2		PORTBbits.RB2		//WE
#define _KY3		PORTBbits.RB3		//WE
#define _KY4		PORTBbits.RB4		//WE
#define _PRZ_CO		PORTBbits.RB5		//WE
#define _KNF		PORTBbits.RB6		//WE	wejscie sygnalu na cele konfiguracji
							//RB7 ISP (WE)


#define _OFF		_KY1		//SW1
#define _SET		_KY2		//SW2	
#define _KUP		_KY3		//SW4
#define _KDW		_KY4		//SW3
//------
//PORTC
#define _CIG_NC 	PORTCbits.RC0		//WE
							//RC1 PWM2 (WY)
							//RC2 PWM (WY)
							//RC3 SCL (WE)
							//RC4 SDA (WE)
#define _MOD_ON		PORTCbits.RC5		//WE
							//RC6 TX (WE)
							//RC7 RX (WE)
//------
//PORTD
#define _GRZEJ		PORTDbits.RD0		//WY	
#define _KCO_CW		PORTDbits.RD1		//WY
#define _POMPA		PORTDbits.RD2		//WY	
#define _VNT		PORTDbits.RD3		//WY	
//#define _PLOMIEN	RD4		//WY
#define _PLOMIEN	PORTDbits.RD4 //WY
#define _RT			PORTDbits.RD5		//WY	
							//RD6 NC	
#define _DCFAN		PORTDbits.RD7		//WY	
//------
//PORTE			
#define _STR		PORTEbits.RE0		//WE/WY
#define _DTA		PORTEbits.RE1		//WE/WY
#define _CLK		PORTEbits.RE2		//WE/WY
//----------------------------------------------
//port.c
//Numery filtrowanych sygnalow cyfrowych
#define S_RESET		0		//1/1	
#define S_PRZ_CO	1		//20/20
#define S_CIG_NC	2		//2/2
#define S_CIG_NO	3		//2/2
#define S_STB		4		//2/2
#define S_PLM		5		//4/4
#define S_DTF		6		//2/2		
#define S_HVZ		7		//2/2
#define S_MOD_ON	8		//2/2	
#define S_DZ_CO		9		//20/1	
#define S_DZ_CW		10		//20/1
#define S_WCW		11		//2/2
#define S_RT		12		//20/20
#define S_ON_PMP	13		//20/20		
#define S_ERR_CSN	14		//16/16
#define S_WRK_CSN	15		//18/18
#define S_LO_CSN	16		//20/20
#define S_ON_PMP_CW 17		//20/20
#define S_ERR_CO	18		//10/10
#define S_ADD_CO	19		//10/10
#define S_PGD		20		//20/20
#define S_CIG_NO_1S	21		//2/10
#define S_OFF		22		//SW1
#define S_SET		23		//SW2	
#define S_KUP		24		//SW4
#define S_KDW		25		//SW3

//pozycje przelacznika funkcyjnego
#define _PFCSN 0		//pozycja CISNIENIE
#define _PFOFF 1		//pozycja OFF
#define _PFLTO 2		//pozycja LATO
#define _PFZMA 3		//pozycja ZIMA
//--------------------------------------------------------------
//---------------------------------------------------------------
//Dane wyjœciowe do modulu LCD
//RSDT1.outSTAT0
#define _PGRES 0		//znacznik petli RESET
#define _PGPRG 1		//znacznik petli PROGRAM
#define _PLMCO 2		//plomien CO
#define _PLMCW 3		//plomien CW
#define _FKSRV 4		//funkcja serwisowa
#define _ANTYL 5		//funkcja antylegionella
#define _KNFPRC 6		//flaga aktywnosci procedury konfiguracji
#define _ENKNF 7		//zezwolenie na wejscie do konfiguracji

//RSDT1.outSTAT1
#define _TdzCW 0		//znacznik realizacji temp dyz. CW
#define _TdzCO 1		//znacznik realizacji temp dyz. CO
#define _MSG   2		//znacznik realizacji proc awaryjnej
#define _ERR   3		//znacznik wylaczenia awaryjnego
#define _L3	   4		//znacznik blokady L3
#define _PGD   5		//znacznik ogrzewania pod³ogowego
#define _OBGCO 6		//obieg CO
#define _OBGCW 7		//obieg CW

//RSDT1.outSTAT2
#define _ErrCW 0		//error CW
#define _ErrCO 1		//error CO
#define _ErrCS 2		//error CS
#define _CSnmt 3		//znacznik zamontowania czujnika cisnienia
#define _F_ZIMA	 4		//znacznik zima
#define _F_LATO	 5		//znacznik lato
#define _F_OFF	 6		//znacznik OFF

//RSDT1.outSTAT3
#define _ZCFM 		0	//stan zworki konfiguracyjnej
#define _VNT_ON 	1	//stan pracy wentylatora
#define _ROZ		2	//znacznik akywnej procedury rozruchu
#define _ErrIN		3	//error IN
#define _PMP_PWM	4	//znacznik trybu pracy z pompa PWM
#define _TRB_ECO	5	//tryb ECO

//KONFIGURACJA
//RSDT1.outKSTAT0
#define _ZCFM	 		0	//stan zworki konfiguracyjnej

//--------------------------------------------------------------
//---------------------------------------------------------------
//Dane wejsciowe z modulu LCD
//RSDT1.inSTAT0
#define _INITKNF 0		//wywolanie konfiguracji
#define _INITSRV 1		//wywolanie funkcji serwisowej
#define _INITANL 2		//wywolanie funkcji antylegionella
#define _PFN_ZIMA	 3	//znacznik zima
#define _PFN_LATO	 4	//znacznik lato
#define _PFN_OFF	 5	//znacznik OFF
#define _PFN_RES	 6	//znacznik RESET
#define _INITDTA     7	//znacznik inicjalizacji danych

//RSDT1.inSTAT1
#define _SRVMAX 0		//znacznik mocy dla funkcji serwisowej
//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
#endif	/*_GLOBAL_H*/
