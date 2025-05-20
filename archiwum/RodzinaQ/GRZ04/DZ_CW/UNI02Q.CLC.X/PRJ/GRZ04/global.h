/*global.h*/
//Parametry globalne
#ifndef	_GLOBAL_H
#define	_GLOBAL_H 1

#define _PIC_PORT 5			//liczba portow procesora

#define _EXT_OSC 16 			//czestotliwosc oscylatora [MHz]
#define _PLL 1				// 1modul PLL (0-nieaktywny,1-aktywny)
//--------------------------------------------------------------
//--------------------------------------------------------------
#define _EXT_BOARD  3       //LCD: 0-plytka starego typu, 1-plytka nowego typu z interfejsem 10V, 2-plytka nowego typu z interfejsem 10V i LIN, 3 -plytka nowego typu z interfejsem 10V, LIN i OT

#if(_EXT_BOARD<2)
#define _MY_LADDR       0x05    //domyslny adres wlasny LIN
#else
#define _MY_LADDR       0x01    //domyslny adres wlasny LIN
#endif
//--------------------------------------------------------------
//--------------------------------------------------------------
//Podstawowe parametry 
#define _NO_PRS 0           //wykluczenie obecnosci presostatu
#define _EX_PRS 1           //praca z dodatkowym czujnikiem ciagu (sygnal zwrotny na wejsciu PRZ_CO)
#define _TEST_PWM 0         //tryb testowy pwm (celem ustalenia vnt=f(Imod))
#define _HIRESVNT 1         //pwm3 10bit

#define _DZ_CW 1            //procedura temperatury dyzurnej
//timery
//*************************************************************
//RTdS
#define _RTON	0			//pwron.c - timer zwloki po zalaczeniu zasilania
#define _RTKEY	1			//keypad.c - timer konsoli
#define _RTKEY2	2			//main.c - timer zmiany kierunku obrotow
#define _RTMOD	3			//rgl.c - timer procedury modulacji
#define _RTBL15 4			//main.c - timer blokady 15 sek.
#define _RTRSPL 5			//main.c - timer blokady zerowania licznika prob E1
#define _RTPCW 6			//main.c - timer procedury zapisu PCW
#define _RTFVNT 7			//tacho.c - timer procedury zapisu PCW
#define _RTMOD2	8			//rgl.c - timer procedury modulacji
#define _RTMIP	9
#define _RTMVV	10

#define _RTDM0	11
#define _RTDM1	12
#define _RTDM2	13
//*************************************************************
//RTS
#define _RTKONF1 0			//konfig.c - timer konfiguracji
#define _RTKONF2 1			//konfig.c - timer konfiguracji
#define _RTROZ	2			//main.c - timer procedury rozruchu
#define _RTRPL	3			//main.c - timer procedury rusz plomien
#define _RTRCI	4			//main.c - timer procedury rusz ciag 				
#define _RTWB48	5			//main.c - timer wybiegu pompy dla proc 48h
#define _RTVWB	6			//main.c - timer wybiegu wentylatora
#define _RTWYL	7			//main.c - timer wybiegu pompy
#define _RTSRV0 8			//main.c - timer wejscia do funkcji serwisowej
#define _RTHVZ 	9			//main.c - timer proby parametrow sieci
#define _RTSTR 10			//main.c - timer proby parametrow sieci
#define _RTRSCI 11			//main.c - timer procedury zerowania licznika ponowien ciagu
#define _RTRSCI2 12			//main.c - timer procedury zerowania licznika ponowien ciagu
#define _RTVWB2 13          //main.c - wybieg na mocy startowej wentylatora
//*************************************************************
//RTM
#define _RTKONF3 0			//konfig.c - timer aktywnosci konfiguracji
#define _RTSRV1 1			//main.c - timer aktywnosci funkcji serwisowej
#define _RTBL3	2			//main.c - timer blokady L3
#define _RTRCI2 3			//main.c - timer procedury rusz ciag
#define _RTRCI3 4			//main.c - timer licznika wystapien zaniku ciagu
#define _RTBLCW 5			//main.c - timer licznika blokady dla pobudzenia od temp. dyz. CW
//*************************************************************
//RTG
#define _RTR48 0			//main.c - timer aktywacji wybiegu pompy co 48h

//*************************************************************
//--------------------------------------------------------------
//UART
#define _MY_ADDR			0x00 //adres wlasny modulu AMMY0
//--------------------------------------------------------------
//----------------------------------------------
/*
//Oznaczenia WE/WY procesora
//------
//PORTA
							//RA0 COTemp (AN0)
							//RA1 CWTemp (AN1)
							//RA2 ZWTemp (AN2)
							//RA3 CisnCO (AN3)
#define _PRZ_CW		RA4		//WE
							//RA5 coTemp2(AN4)

#define _CIG_NO		RA6		//WE
#define _STB 		RA7		//WE

//------
//PORTB

#define _TAC_INT	RB0		//WE	
#define _K2			RB1		//WE
#define _K1			RB2		//WE
#define _K3			RB3		//WE
#define _K4			RB4		//WE
#define _PRZ_CO		RB5		//WE
#define _KNF		RB6		//WE	wejscie sygnalu na cele konfiguracji

#define _MUX		_K3		//WY
//------
//PORTC

#define _CIG_NC 	RC0		//WE
							//RC1 PWM2 (WY)
							//RC2 PWM (WY)
							//RC3 SCL (WE)
							//RC4 SDA (WE)
#define _MOD_ON		RC5		//WE
							//RC6 TX (WE)
							//RC7 RX (WE)

//------
//PORTD
							//WY - WYSWIETLACZ
#define _GRZEJ		RD0		//WY	
#define _KCO_CW		RD1		//WY
#define _POMPA		RD2		//WY	
#define _VNT		RD3		//WY	
#define _PLOMIEN	RD4 	//WY
#define _RT			RD5		//WY	
							//RD6 NC	
#define _DCFAN		RD7		//WY

//------
//PORTE	

#define _STR		RE0		//WE/WY
#define _DTA		RE1		//WE/WY
#define _CLK		RE2		//WE/WY
							//RE3 ISP (WE)
*/

//----------------------------------------------
//port.c
//Numery filtrowanych sygnalow cyfrowych
#define S_KDW		0		//1/1	
#define S_KUP		1		//1/1
#define S_CIG_NC	2		//2/1
#define S_CIG_NO	3		//2/1
#define S_STB		4		//2/2
#define S_PLM		5		//4/4
#define S_WCW		6		//2/2
#define S_ERR_CW	7		//5/5		
#define S_MOD_ON	8		//2/2
#define S_CIG_NO_1S	9		//2/10
#define S_CIG2		10		//2/2
//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
#endif	/*_GLOBAL_H*/