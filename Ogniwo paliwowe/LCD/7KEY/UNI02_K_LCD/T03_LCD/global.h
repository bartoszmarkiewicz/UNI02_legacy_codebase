/*global.h EXT*/
//Parametry globalne

#ifndef	_GLOBAL_H
#define	_GLOBAL_H 
//--------------------------------------------------------------
//--------------------------------------------------------------
#if	defined(_18C242) || defined(_18C252) || defined(_18F248) || defined(_18F258)
#define _PIC_PORT 3			//liczba portow procesora
#endif
#if defined(_18F442) || defined(_18F452) || defined(_18F448) || defined(_18F458)||defined(_18F4525)
#define _PIC_PORT 5			//liczba portow procesora
#endif
#if defined(_18F63J90) || defined(_18F65J90) || defined(_18F66J90) || defined(_18F67J90)
#define _PIC_PORT 7			//liczba portow procesora
#endif      
#ifndef _PIC_PORT
#define _PIC_PORT 0			//wywolanie komunikatu bledu dla niezdefiniowanej rodziny PIC
#endif
//--------------------------------------------------------------
//--------------------------------------------------------------
#define _EXT_BOARD 3		//3 0-plytka starego typu, 1-plytka nowego typu z interfejsem 10V, 2-plytka nowego typu z interfejsem 10V i LIN, 3 -plytka nowego typu z interfejsem 10V, LIN i OT
#define _KEY_BBOARD 0       //rodzaj klawiatury (0-cztery przyciski 1-siedem przyciskow)
#if (_EXT_BOARD==0)
    #define _KEY_BBOARD 0
#endif
//--------------------------------------------------------------
//--------------------------------------------------------------
#define _EXT_OSC 8 			//czestotliwosc oscylatora [MHz]

#if defined(_18F66J90) || defined(_18F67J90)
#define _PLL 1				//1 modul PLL (0-nieaktywny,1-aktywny)
							//UWAGA: dla PIC18F63j90 aktywny tylko przy zew. osc.
#else
#define _PLL 0				//modul PLL (0-nieaktywny,1-aktywny)
							//UWAGA: dla PIC18F63j90 aktywny tylko przy zew. osc.
#endif

#if _PLL==0
    #define _XTAL_FREQ _EXT_OSC*1000000
#else
    #define _XTAL_FREQ 4*_EXT_OSC*1000000
#endif

#if (_PLL==1)&&(defined(_18F63J90) || defined(_18F65J90))
#error Niewlasciwy mikrokontroler - brak zasobow dla PLL=1
#endif
//--------------------------------------------------------------
//--------------------------------------------------------------
#if (_EXT_BOARD>=2)
#if defined(_18F63J90) || defined(_18F65J90)
#error Niewlasciwy mikrokontroler - brak zasobow
#endif
#endif
#define _LIN_OID        0x01                //identyfikator kotla kondensacyjnego
//--------------------------------------------------------------
//--------------------------------------------------------------
#define _NEWKEY		1		//typ interfejsu (0=old, 1=new)
#define _TDEMO		0		//aktywuj tryb demo
#define _TDEMOFL	0		//w trybie demo zapal na stale wszystkie segmenty
//--------------------------------------------------------------
//typ kotla (oznaczenie dla potrzeb algorytmu)
#define _UNICO	 0			//jednofunkcyjny (unico)
//#define _BITERM	 1			//bitermiczny (miniterm)
//#define _BITERM2 2			//bitermiczny (maxiterm)
//#define _MONOHB  3			//monotermiczny (minimax) z hydroblokiem
//--------------------------------------------------------------
//Dane zwiazane z komunikacja RS232 z modulem UNI02
#define _MY_ADDR	0x11	//adres wlasny plytki wyswietlacza
#define _UNI02_ADDR	0x10	//adres plytki steruj¹cej
//--------------------------------------------------------------------------
//RTcS
#define _RTLLED2 0			//lled.c

//RTdS
#define _RTON	0			//pwron.c - timer zwloki po zalaczeniu zasilania
#define _RTPCW	1			//konsola.c
#define _RTSET	2			//konsola.c
#define _RTPCS	3			//konsola.c
#define _RTPEX	4			//keypad.c
#define _RTSTR	5			//main.c
#define _RTVER	6			//main.c
#define _RTFSP	7			//main.c
#define _RTFLIN 8                       //
#define _RTFLED 9                       //lled.c
#define _RTALIN 10
#define _RTFOT 11
//RTS
#define _RTKONF1 0			//konfig.c - timer konfiguracji
#define _RTLLED  1			//lled.c
#define _RTEST   2
#define _RTLCK   3          //timer dla pobudzenia z komendy cyklicznej
#define _RTZSETP 4          //timer modulacji wartosci SETP
#define _RTNODB1  5          //timer resetu wyswietlacza po zaniku komunikacji z plytka sterujaca $$$
#define _RTNODB2  6          //timer resetu wyswietlacza po zaniku komunikacji z plytka sterujaca $$$
//RTM
#define _RTKONF3 0			//konfig.c - timer aktywnosci konfiguracji
#define _RTFSR	 1			//konsola.c
#define _RTALG	 2			//konsola.c

#define _KRKZSETP 5         //krok czasowy modulacji SETP
//--------------------------------------------------------------------------
//--------------------------------------------------------------
#define _PRNL3 0            //decyzja jak wyswietlac stan L3 (0-migajacy kaloryfer,1-kod L3)
//---------------------------------------------------------------
//---------------------------------------------------------------
#define _TEST_OT 0          //testowanie interfejsu OT
//--------------------------------------------------------------
//---------------------------------------------------------------
#if (_EXT_BOARD==0)
#define LED         LATCbits.LC1                //WY
#define _CS_LIN1    LATCbits.LC2                //WY
#define _TXD1       LATCbits.LC6                //WY
#if _NEWKEY==0		//stary interfejs
	#define _OFF			PORTAbits.RA3		//WE
	#define _SET			PORTAbits.RA2		//WE
	#define _KUP			PORTAbits.RA1		//WE
	#define _KDW			PORTAbits.RA0		//WE
#else				//nowy interfejs
	#define _OFF			PORTAbits.RA0		//WE
	#define _SET			PORTAbits.RA1		//WE
	#define _KUP			PORTAbits.RA2		//WE
	#define _KDW			PORTAbits.RA3		//WE
#endif
#else

#if _KEY_BBOARD==0
    #define LED         LATCbits.LC1                //WY
    #define _CS_LIN1    LATCbits.LC2                //WY
    #define _TXD1       LATCbits.LC6                //WY

    #if _TEST_OT==1
    #define _WOUTX		LATAbits.LA2		//WY
    #define _ROUTX		PORTAbits.RA2		//WY
    #endif

    #define _OFF		PORTAbits.RA1		//WE
    #define _SET		PORTAbits.RA2		//WE
    #define _KUP		PORTAbits.RA3		//WE
    #define _KDW		PORTAbits.RA5		//WE

    #define _IN_OT      PORTBbits.RB0               //WE OT
    #define _IN2_OT     PORTBbits.RB1               //WE OT
    #define _OUT_OT     LATBbits.LB2                //WY OT
#endif
#if _KEY_BBOARD==1
    #define LED         LATCbits.LC1                //WY
    #define _CS_LIN1    LATCbits.LC2                //WY
    #define _TXD1       LATCbits.LC6                //WY

    #if _TEST_OT==1
    #define _WOUTX		LATAbits.LA2		//WY
    #define _ROUTX		PORTAbits.RA2		//WY
    #endif

    #define _OFF		PORTAbits.RA4		//WE P5
    #define _SET		PORTAbits.RA5		//WE P4 (ZIMA/LATO))
    #define _KOM		PORTAbits.RA2		//WE P2 (FUNKCJA SERWISOWA/ANTYLEGIONELLA)

    #define _KUP_CW		PORTBbits.RB5		//WE P6
    #define _KDW_CW		PORTAbits.RA3		//WE P3

    #define _KUP_CO		PORTGbits.RG3		//WE
    #define _KDW_CO		PORTAbits.RA1		//WE P1

    #define _IN_OT      PORTBbits.RB0               //WE OT
    #define _IN2_OT     PORTBbits.RB1               //WE OT
    #define _OUT_OT     LATBbits.LB2                //WY OT
#endif
#endif
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
#if _KEY_BBOARD==0
    #define S_OFF			0		//WE
    #define S_SET			1		//WE
    #define S_KUP			2		//WE
    #define S_KDW			3		//WE
    #define S_bEN_10V		4		//WE
    #define S_b10V_RT		5		//WE
    #define S_10V_nPCO      6       //WE

    #define SS_OFF			0		//WE
    #define SS_SET			1		//WE
    #define SS_KUP			2		//WE
    #define SS_KDW			3		//WE
    //#define SS_SET_UP		4		//WE
    //#define SS_SET_DW		5		//WE
    #define SS_OFF_SET		4		//WE
#endif
#if _KEY_BBOARD==1
    #define S_OFF			0		//WE
    #define S_SET			1		//WE
    #define S_KOM			2		//WE
    #define S_KUP_CW		3		//WE
    #define S_KDW_CW		4		//WE
    #define S_KUP_CO		5		//WE
    #define S_KDW_CO		6		//WE
    #define S_bEN_10V		7		//WE
    #define S_b10V_RT		8		//WE
    #define S_10V_nPCO      9       //WE

    #define SS_OFF			0		//WE
    #define SS_SET			1		//WE
    #define SS_KOM			2		//WE
    #define SS_KUP_CW		3		//WE
    #define SS_KDW_CW		4		//WE
    #define SS_KUP_CO		5		//WE
    #define SS_KDW_CO		6		//WE
    #define SS_OFF_SET		7		//WE
#endif
//--------------------------------------------------------------
//---------------------------------------------------------------
//Dane wyjsciowe z modulu UNI-01
//STANDARD
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
#define _PGD   5		//znacznik aktywnosci funkcji pogodowej
#define _OBGCO 6		//aktywny (PRACA) obieg CO
#define _OBGCW 7		//aktywny (PRACA) obieg CW

//RSDT1.outSTAT2
#define _ErrCW 0		//error CW
#define _ErrCO 1		//error CO
#define _ErrCS 2		//error CS
#define _CSnmt 3		//znacznik zamontowania czujnika cisnienia
#define _F_ZIMA	 4		//znacznik zima
#define _F_LATO	 5		//znacznik lato
#define _F_OFF	 6		//znacznik OFF
#define _KALIB 7		//znacznik aktywnosci proc. kalibracji wentylatora

//RSDT1.outSTAT3
#define _ZCFM 		0	//stan zworki konfiguracyjnej
#define _VNT_ON 	1	//stan pracy wentylatora
#define _ROZ		2	//znacznik akywnej procedury rozruchu
#define _ErrIN		3	//error IN
#define _PMP_PWM	4	//znacznik trybu pracy z pompa PWM
#define _TRB_ECO	5	//tryb ECO
#define _TIM_ZAS	6	//znacznik pobudzenia CW/stan na wejsciu timera zasobnika
#define _POB_RT 	7	//pobydzenie RT

//RSDT1.outSTAT4
#define _PR_ODP 	0	//aktywnosc procedury odpowietrzajacej
#define _POP_FN 	1	//stan poprzedni przelacznika funkcji (0-zima,1-lato)
#define _TMP_ZW 	2	//aktywnosc czujnika temperatury zewnetrznej
#define _PGD_TP 	3	//rodzaj regulatora pogodowego (0-tradycyjny, 1-rozszerzony)
#define _PMP_ON 	4	//stan pracy pompy (0-postoj, 1-paraca)
#define _KCOCW     	5	//zawor 3d (0-obieg CW, 1-obieg CO)
#define _PRZ_CO2   	6	//styk czujnika przeplywu CO (0-rozwarty, 1-zwarty)
#define _GRZ_ON 	7	//pobudzenie modu³u kontroli plomienia (0-postoj, 1-paraca)

//RSDT1.outSTAT5
#define _STK_RT2   	0	//styk wejscia pobudzenia RT/obni¿enie nocne (0-rozwarty, 1-zwarty)
#define _STK_CW2   	1	//styk wejscia pobudzenia CW/timer zasobnika (0-rozwarty, 1-zwarty)
#define _CIG_NO2   	2	//styk czujnika ciagu NO (0-rozwarty, 1-zwarty)
#define _CIG_NC2   	3	//styk czujnika ciagu NC (0-rozwarty, 1-zwarty)
#define _STK_STB   	4	//styk czujnika STB NC (0-rozwarty, 1-zwarty)
#define _POB_PGRT 	5	//pobudzenie RT od regulatora pogodowego
#define _FZA_REG    6   //faza stabilizacji regulatora
#define _FEN_PWD   	7	//flaga gotowosci pracy na sprzeglo
                  //5 znacznik aktywnosci zew. reg. pogodowego ustawiany przez plytke LCD
//KONFIGURACJA
//RSDT1.outKSTAT0
#define _ZCFM		0	//stan zworki konfiguracyjnej
#define _KC1F		1	//znacznik kotla jednofunkcyjnego	
//--------------------------------------------------------------
//---------------------------------------------------------------
//Dane wejsciowe do modulu UNI-02
//STANDARD
//RSDT1.inSTAT0
#define _INITKNF 		0	//wywolanie konfiguracji
#define _INITSRV 		1	//wywolanie funkcji serwisowej
#define _INITANL 		2	//wywolanie funkcji antylegionella
#define _PFN_ZIMA	 	3	//znacznik zima
#define _PFN_LATO	 	4	//znacznik lato
#define _PFN_OFF	 	5	//znacznik OFF
#define _PFN_RES	 	6	//znacznik RESET
#define _INITDTA	 	7	//znacznik inicjalizacji danych

//RSDT1.inSTAT1
#define _SRVMAX 0		//znacznik mocy dla funkcji serwisowej
#define _KPLUS 	1		//znacznik stanu przycisku
#define _KMINUS 2		//znacznik stanu przycisku
#define _KSET 	3		//znacznik stanu przycisku
#define _KOFF 	4		//znacznik stanu przycisku
#define _END_ODP 5		//deaktywacja zdalna procedury odpowietrzjacej
#define _7KEY   6       //interfejs 7 przyciskowy (oddzielnie nastawa CO i CW)       

//RSDT1.inINSTAT
#define _F10V   0       //znacznk aktywnosci wejscia 10V
#define _10V_RT 1       //pobudzenie RT z wejscia 10V
#define _FOT    2       //znacznk aktywnosci wejscia OT pobudzenie CW z LIN
#define _OT_RT  3       //pobudzenie RT z wejscia OT
#define _OT_CW  4       //pobudzenie CW z wejscia OT
#define _OT_STR 5       //znacznik pracy w trybie sterowania z managera stref lub regulatora RT
#define _OT_DPGD 6       //znacznik odczytu przez regulator OT informacji o temp zew.

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


//KONFIGURACJA
//RSDT1.inKSTAT0
#define _KC1F		1	//znacznik kotla jednofunkcyjnego
//--------------------------------------------------------------
//Graniczne wartosci temperatury TCO
#define _MIN_TCO			0
#define _MAX_TCO			99
//--------------------------------------------------------------
//Graniczne wartosci temperatury TCW
#define _MIN_TCW			0
#define _MAX_TCW			99
//--------------------------------------------------------------
//Graniczne wartosci cisienia CS
#define _MIN_CS				0
#define _MAX_CS				40
//--------------------------------------------------------------
//Graniczne wartosci temperatury ZW
#define _MIN_ZW				-5
#define _MAX_ZW				40
//--------------------------------------------------------------
//pozycje przelacznika funkcyjnego
#define _PFCSN 0		//pozycja CISNIENIE
#define _PFOFF 1		//pozycja OFF
#define _PFLTO 2		//pozycja LATO
#define _PFZMA 3		//pozycja ZIMA
//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
#endif	/*_GLOBAL_H*/
