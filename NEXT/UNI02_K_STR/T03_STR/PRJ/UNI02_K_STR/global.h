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
//--------------------------------------------------------------
//--------------------------------------------------------------
#if(_EXT_BOARD<2)
#define _MY_LADDR       0x05    //domyslny adres wlasny LIN
#else
#define _MY_LADDR       0x01    //domyslny adres wlasny LIN
#endif

#define _ZK_LADDR       0       //wybor zakresu adresu (0-adres od 1..5, 1-adres od 1..5 i 9..12)
//--------------------------------------------------------------
//--------------------------------------------------------------
#define _DEMO 1             //tryb demonstracyjny pracy sterownika
#define _DEMO_NZW 1         //wylaczenie symulacji czujnika zewnetrznego w trybie DEMO

#define _TSPRZ_CW 0      //wyswietlanie testowe stanu przetwornika przeplywu CW na I2C
#define _TST_RT 0        //ustaw dla celow testu stan styku RT na 1
//--------------------------------------------------------------
//--------------------------------------------------------------
//UART - Dane zwiazane z komunikacja RS232 z modulem LCD
#define _MY_ADDR	0x10            //adres wlasny plytki sterujacej UNI-02 do kondensatow
#define _LCD_ADDR	0x11            //adres modulu LCD
//-------------------
#define _PWRKNF 0                   //warunek wejscia do konfiguracji (0-bez warunku,1-po zaniku zasilania)
#define _I0_PMP 0                   //tryb z wylaczaniem pompy w trakcie zmiany obiegu (0-nieaktywny, 1-aktywny))
#define _SOLAR  0        //opcja solara (wylaczenie dla kotlow z zasobnikiem funkcji przesterowania zaworu 3dr na obieg CO pry temp CW>85)

#define _HIRESVNT 1     //pwm3 10bit

//-------------------
//Podstawowe parametry 
//timery
//*************************************************************
//RTdS
#define _RTON	0			//pwron.c - timer zwloki po zalaczeniu zasilania
#define _RTKEY	1			//keypad.c - timer konsoli
#define _RTZKP	2			//main.c - timer zmiany kierunku obrotow
#define _RTMOD	3			//rgl.c - timer procedury modulacji
#define _RTBL15 4			//main.c - timer blokady 15 sek.
#define _RTRSPL 5			//main.c - timer blokady zerowania licznika prob E1
#define _RTPCO  6			//pomiar.c - timer zapisu nastawy PCO
#define _RTPCW  7			//pomiar.c - timer zapisu nastawy PCW
#define _RTPFN  8			//pomiar.c - timer zapisu pozycji PFN
#define _RTFVNT  9			//vntregtch.c - timer filtracji wartosci predkosci obrotowej
#define _RTFKVN  10			//vntregtch.c - timer kalibracji
#define _RTFVNT2 11			//vntregtch.c - timer kalibracji
#define _RTMOD2	12			//rgl.c - timer procedury modulacji
#define _RTPMOD	13			//pompa.c - timer procedury modulacji dla pompy
#define _RTECO	14			//pompa.c - timer procedury modulacji dla pompy
#define _RTHPM	15			//pompa.c - oobnizona predkosc wybiegu pompy na czas przelaczenia zaworu 3DR
#define _RTRDZ	16			//main.c - dla licznika godzin pracy

#define _RTDCO 17
#define _RTDCW 18
//**************************************************************
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
#define _RTSTR 9			//main.c - timer rozruchu sterownika
#define _RTDPR 10			//main.c - timer procedury zwloki na reakcje czujnika przep WG
#define _RTRPR 11			//main.c - timer procedury rusz przep³yw WG
#define _RTMPMP 12			//main.c - timer procedury modulacji wybiegiem pompy
#define _RTRPL2	13			//main.c - timer procedury rusz plomien
#define _RTWPWM 14			//main.c - timer wstepnego progu dolnej granicznej wartosci PWM wentylatora dla grzania CO
#define _RTCPGD 15          //pomiar.c - timer aktualizacji nastawy PCO przy aktywacji/dezaktywacji regulatora pogodowego

#define _RTSPLM 16          //nain.c - zegar symulacji plomienia (na potrzeby trybu DEMO)
//*************************************************************
//RTM
#define _RTKONF3 0			//konfig.c - timer aktywnosci konfiguracji
#define _RTSRV1 1			//main.c - timer aktywnosci funkcji serwisowej
#define _RTBL3	2			//main.c - timer blokady L3
#define _RTMAL 3			//main.c - timer trwania aktywnosci funkcji AL
#define _SRVRTANL 4			//main.c - timer trwania aktywnosci funkcji AL w trybie kontrolnym
#define _RTMMAXCO 5			//main.c - timer trwania wstepnej mocy maksymalnej obiegu CO
//*************************************************************
//RTG
#define _RTR48 0			//main.c - timer aktywacji wybiegu pompy co 48h
#define _RTANL 1			//main.c - timer aktywacji grzania AL
#define _RTR48Z3 2			//main.c - timer aktywacji wybiegu pompy co 48h w obiegu CW
#define _RTPGD2 3			//main.c - timer deaktywacji pobudzenia od regulatora pogodowego
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
#define _STB		RA7		//WE
//------
//PORTB
#define _TAC_INT	RB0		//FTACH (WE)	
#define _KY1		RB1		//WE
#define _KY2		RB2		//WE
#define _KY3		RB3		//WE
#define _KY4		RB4		//WE
#define _PRZ_CO		RB5		//WE
#define _KNF		RB6		//WE	wejscie sygnalu na cele konfiguracji
							//RB7 ISP (WE)
//------
//PORTC
#define _CIG_NC 	RC0		//WE
							//RC1 PWM2 pompa (WY)
							//RC2 PWM modulator (WY)
							//RC3 SCL (WE)
							//RC4 SDA (WE)
#define _MOD_ON		RC5		//WE
							//RC6 TX (WE)
							//RC7 RX (WE)
//------
//PORTD
#define _GRZEJ		RD0		//WY	
#define _KCO_CW		RD1		//WY
#define _POMPA		RD2		//WY	
#define _VNT		RD3		//WY	
#define _PLOMIEN	RD4		//WY
#define _RT			RD5		//WY	
							//RD6 NC	
#define _FFPWM		RD7		//PWM wentylator (WY)	
//------
//PORTE			
#define _ST			RE0		//WE/WY
#define _DT			RE1		//WE/WY
#define _CL			RE2		//WE/WY
 */
//----------------------------------------------
//port.c
//Numery filtrowanych sygnalow cyfrowych
#define S_ERR_IN	0		//1/1	
#define S_PRZ_CO	1		//20/20
#define S_CIG_NC	2		//2/2
#define S_CIG_NO	3		//2/2
#define S_STB		4		//2/2
#define S_PLM		5		//4/4
#define S_TIM		6		//2/2		
#define S_ERR_CW	7		//5/5
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
#define S_TIM_PGD	22		//2/10
#define S_EN_ZW     23		//2/10
#define S_HOT_IN    24		//20/20 $$$
#define S_UP        25		//20/20 
#define S_OUT_RT    26		//20/20 

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
#define _PGD   5		//aktywnosci regulatora pogodowego
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
#define _KALIB 7		//znacznik aktywnosci proc. kalibracji

//RSDT1.outSTAT3
#define _ZCFM 		0	//stan zworki konfiguracyjnej
#define _VNT_ON 	1	//stan pracy wentylatora (0-postoj, 1-paraca)
#define _ROZ		2	//znacznik akywnej procedury rozruchu
#define _ErrIN		3	//error IN
#define _PMP_PWM	4	//znacznik trybu pracy z pompa PWM
#define _TRB_ECO	5	//tryb ECO
#define _TIM_ZAS	6	//stan zworki timer zasobnika
#define _POB_RT 	7	//pobudzenie RT/obni¿enie nocne

//RSDT1.outSTAT4
#define _PR_ODP 	0	//aktywnosc procedury odpowietrzajacej
#define _POP_FN 	1	//stan poprzedni przelacznika funkcji (0-zima,1-lato)
#define _TMP_ZW 	2	//aktywnosc czujnika temperatury zewnetrznej
#define _PGD_TP 	3	//rodzaj regulatora pogodowego (0-tradycyjny, 1-rozszerzony)
#define _PMP_ON 	4	//stan pracy pompy (0-postoj, 1-paraca)
#define _KCOCW     	5	//zawor 3d (0-obieg CW, 1-obieg CO)
#define _PRZ_CO2   	6	//styk czujnika przeplywu CO (0-rozwarty, 1-zwarty)
#define _GRZ_ON 	7	//pobudzenie modulu kontroli plomienia (0-postoj, 1-paraca)

//RSDT1.outSTAT5
#define _STK_RT2   	0	//styk wejscia pobudzenia RT/obni¿enie nocne (0-rozwarty, 1-zwarty)
#define _STK_CW2   	1	//styk wejscia pobudzenia CW/timer zasobnika (0-rozwarty, 1-zwarty)
#define _CIG_NO2   	2	//styk czujnika ciagu NO (0-rozwarty, 1-zwarty)
#define _CIG_NC2   	3	//styk czujnika ciagu NC (0-rozwarty, 1-zwarty)
#define _STK_STB   	4	//styk czujnika STB NC (0-rozwarty, 1-zwarty)
#define _POB_PGRT 	5	//pobudzenie RT od regulatora pogodowego
#define _FZA_REG    6   //faza stabilizacji regulatora
#define _FEN_PWD   	7	//flaga gotowosci pracy na sprzeglo lub bufor
//--------------------------------------------------------------
//---------------------------------------------------------------
//KONFIGURACJA
//RSDT1.outKSTAT0
#define _KZCFM	 	0	//stan zworki konfiguracyjnej
#define _KC1F		1	//znacznik kotla jednofunkcyjnego
#define _CWIMP		2	//znacznik impulsowego kczujnika przeplywu CW
#define _PWD		3	//znacznik trybu pracy ze stabilizacja temperatury
#define _BUF		4	//znacznik trybu pracy na bufor ciepla
#define _UPCW		5	//znacznik aktywnej pracy na wysokich parametrach
#define _BPSS		6	//znacznik bypass dla pobudzenia RT
#define _OUT_RT     7   //znacznik pobudzenia na potzreby bypass
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
#define _KPLUS 	1		//znacznik stanu przycisku
#define _KMINUS 2		//znacznik stanu przycisku
#define _KSET 	3		//znacznik stanu przycisku
#define _KOFF 	4		//znacznik stanu przycisku
#define _END_ODP 5		//deaktywacja zdalna procedury odpowietrzjacej
#define _7KEY   6       //interfejs 7 przyciskowy (oddzielnie nastawa CO i CW)
#define _POT   7        //interfejs z potencjometrami (tryb pracy i nastawa CO i CW odczytywane z pozycji 2 potencjometrow)

//RSDT1.inINSTAT
#define _F10V   0       //znacznk aktywnosci wejscia 10V
#define _10V_RT 1       //pobudzenie RT z wejscia 10V
#define _FOT    2       //znacznk aktywnosci wejscia OT pobudzenie CW z LIN
#define _OT_RT  3       //pobudzenie RT z wejscia OT
#define _OT_CW  4       //pobudzenie CW z wejscia OT
#define _OT_STR 5       //znacznik pracy w trybie sterowania z managera stref lub regulatora RT
#define _OT_DPGD 6       //znacznik dezaktywacji dla wewnetrznego regulatora pogodowego

//RSDT1.inLNSTAT
#define _FLIN   0       //znacznk aktywnosci transmisji LIN
#define _LN_RT  1       //pobudzenie RT z LIN
#define _LN_CW  2       //pobudzenie CW z LIN
#define _LN_KSK 3       //znacznik pracy w trybie kaskady
#define _LN_STR 4       //znacznik pracy w trybie sterowania z managera stref lub regulatora RT
#define _LN_DPGD 5      //znacznik dezaktywacji dla wewnetrznego regulatora pogodowego
#define _LN_PWD 6       //znacznik gotowosci dla przewyzszenia temperatury
#define _LN_BUF 7       //znacznik gotowosci dla pracy na bufor ciepla
//RSDT1.inSTAT2
//bit3...bit0           //specjalny tryb pracy urzadzenia

//RSDT1.inLNSTAT2
#define _LN_UP 0        //pobudzenie grzania na wysokich parametrach
#define _LN_BPSS  1     //bypass dla pobudzenia RT

//KONFIGURACJA
//RSDT1.inKSTAT0
#define _IKC1F	1       //znacznik kotla jednofunkcyjnego
#define _EXT_KFR 7      //rozszerzona konfiguracyjna ramka danych
//--------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
#endif	/*_GLOBAL_H*/
