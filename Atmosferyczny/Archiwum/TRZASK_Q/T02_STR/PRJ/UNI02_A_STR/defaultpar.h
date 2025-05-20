/*defaultpar.h*/
#ifndef	_DEFPAR_H
#define	_DEFPAR_H
#include "../../proj.tp.h"

#ifndef _TYP_PRJ
#error NIEOKRESLONY IDENTYFIKATOR PROJEKTU
#endif

#if _TYP_PRJ==2
#advisory _TYP_PRJ==2

#include "global.h"
#include "main.h"
#include "konfig.h"
#include "pomiar.h"

//adresy danych przechowywanych w pamieci nieulotnej eeprom
#define _ADRERR 0x00C0		//kod awaryjny blokady trwalej
#define _ADRMST	0x00C1		//moc startowa
#define _ADRMCW	0x00C2		//moc maksymalna dla WU
#define _ADRMCO	0x00C3		//moc maksymalna dla WG
#define _ADRRGZ 0x00C4		//wybor rodzaju gazu
#define _ADRWTK	0x00C5		//wybor typu kotla
#define _ADRTAL	0x00C6		//timer antyleg. aktywacja
#define _ADRRTA	0x00C7		//timer antyleg. wartosc
#define _ADROBG	0x00C8		//rodzaj obiegu
#define _ADRPCO	0x00C9		//nastawa PCO
#define _ADRPCW	0x00CA		//nastawa PCW
#define _ADRPFN	0x00CB		//pozycja PFN
#define _ADRPDG	0x00CC		//ogrzewanie podlogowe
#define _ADRCS5	0x00CD		//czujnik cisnienia 5V

#define _ADRRDPM  0x00D0		//rodzaj pompy
#define _ADRDLPM  0x00D1		//delta temperaturowa CO dla pompy
#define _ADRMNPM  0x00D2		//granica minimalna predkosci pompy
#define _ADRMXPM  0x00D3     //granica maksymalna predkosci pompy
#define _ADRMXPM2 0x00D4     //granica maksymalna predkosci pompy CW
#define _ADRHSCO  0x00D5		//histereza wylaczenia grzania CO (ponad nastawe)
#define _ADRCONW  0x00D6		//temp CO przy grzaniu CW zasobnika
#define _ADRPROP  0x00D7		//poprawka dla regulagtora pogodowego
#define _ADRALIN  0x00D8     	//adres LIN
#define _ADRPGMX  0x00D9     	//maksymalna wartosc graniczna CO
#define _ADRPGDR  0x00DA		//tryb pracy regulatora pogodowego
#define _ADRCWTM  0x00DB		//we. sygnalu timera zasobnika (1F)
#define _ADRCWMG  0x00DC		//histereza wylaczenia CW(4F)
#define _ADRPPGD  0x00DD        //nastawa regulatora pogodowego

#define _ADRRLO   0x00EE        //godziny pracy LO
#define _ADRRHI   0x00EF        //godziny pracy HI

//Historia zdarzen awaryjnych
#define _ADR_HIND  0x01C8		//indeks wskazujacy na miejsce zapisu kolejnego zdarzenia
#define _ADR_SHBF  0x01C9       //pamiec na potrzeby eliminacji powtorzen zapisu dla tego samego bledu
#define _ADR_HNPZ  0x01CA       //liczba zapisanych zdarzen
#define _ADR_HRFL  0x01CB		//licznik zapisow LB
#define _ADR_HRFH  0x01CC		//licznik zapisow HB
#define _ADR_HFLG  0x01CD       //flaga serwisowa (licznik)
#define _ADR_HBUF  0x01CE       //adres bufora historii zdarzen (rozmiar _HISTSZ)

//Status wylaczenia awaryjnego
#define _ADRNZEE   0x01E0		//liczba wylaczen awaryjnych z blokada (2 bajty)
//                 0x01E1
#define _ADRSTPZ   0x01E2		//liczba zapisanych statusow wylaczenia awaryjnego z blokada (1 bajt)

#define _ADRSTEE0  0x01E8		//zrzut statusu kotla po wylaczeniu awaryjnym z blokada

#define _ADRSTEE1  0x0208		//zrzut statusu kotla po wylaczeniu awaryjnym z blokada

#define _ADRSTEE2  0x0228		//zrzut statusu kotla po wylaczeniu awaryjnym z blokada

#define _ADRSTEE3  0x0248		//zrzut statusu kotla po wylaczeniu awaryjnym z blokada


//wartosci domyslne danych w pamieci (pierwsze programowanie)
#define _DTAERR	0			//kod awaryjny
//---------------------
#define _DPWMPMP 1			// ! domyslny typ pompy (1-PWM)
//---------------------
#define _DTALIN	_MY_LADDR   //domyslny adres LIN

#define _DEFRLO   0        //godziny pracy LO
#define _DEFRHI   0        //godziny pracy HI
//---------------------
//Unico
#if _TYPKT==_UNCO
#define _DTAMST 40			//moc startowa
#define _DTAMCW	99			//moc maksymalna dla WU
#define _DTAMCO	99			//moc maksymalna dla WG
#define _DTARGZ _ZIEMNY		//wybor rodzaju gazu
#define _DTAWTK	_UNICO		//wybor typu kotla
#define _DTAPCO	_MAX_PCO	//wybor typu kotla
#define _DTAPCW	F3_MAX_PCW	//wybor typu kotla
#define _DTAPFN	_PFZMA		//wybor typu kotla
#define _DTAPDG	0			//ogrzewanie podlogowe
#define _DTACS5	0			//czujnik cisnienia 5V

#define _DTRDPM	_DPWMPMP			//rodzaj pompy
#define _DTDLPM	6			//delta temperaturowa CO dla pompy
#define _DTMNPM	50			//granica minimalna predkosci pompy
#define _DTMXPM	80			//granica maksylna predkosci pompy
#define _DTMXPM2	99		//granica maksylna predkosci pompy dla WU
#define _DTHSCO  5			//histereza wylaczenia grzania CO (ponad nastawe)
#define _DTCONW  75			//temp CO przy grzaniu CW zasobnika
#define _DTPROP  0			//poprawka dla regulagtora pogodowego
#define _DTPGMX _MAX_PCO    //maksymalna wartosc graniczna CO dla regulatora pogodowego
#define _DTPGDR 1               //tryb pracy regulatora pogodowego
#define _DTCWTM  0              //we. sygnalu timera zasobnika (1F)
#define _DTCWMG  0              //histereza wylaczenia CW(4F)
#define _DTPPGD  99             //domyslna wartosc nastawy KT dla aktywnego regulatora pogodowego
#endif
//Miniterm
#if _TYPKT==_INWEST
#define _DTAMST 40			//moc startowa
#define _DTAMCW	99			//moc maksymalna dla WU
#define _DTAMCO	99			//moc maksymalna dla WG
#define _DTARGZ _ZIEMNY		//wybor rodzaju gazu
#define _DTAWTK	_BITERM		//wybor typu kotla
#define _DTAPCO	_MAX_PCO	//wybor typu kotla
#define _DTAPCW	F1_MAX_PCW	//wybor typu kotla
#define _DTAPFN	_PFZMA		//wybor typu kotla
#define _DTAPDG	0			//ogrzewanie podlogowe
#define _DTACS5	0			//czujnik cisnienia 5V

#define _DTRDPM	_DPWMPMP			//rodzaj pompy
#define _DTDLPM	6			//delta temperaturowa CO dla pompy
#define _DTMNPM	50			//granica minimalna predkosci pompy
#define _DTMXPM	80			//granica maksylna predkosci pompy
#define _DTMXPM2	99		//granica maksylna predkosci pompy dla WU
#define _DTHSCO  5			//histereza wylaczenia grzania CO (ponad nastawe)
#define _DTCONW  75			//temp CO przy grzaniu CW zasobnika
#define _DTPROP  0			//poprawka dla regulagtora pogodowego
#define _DTPGMX _MAX_PCO    //maksymalna wartosc graniczna CO dla regulatora pogodowego
#define _DTPGDR 1               //tryb pracy regulatora pogodowego
#define _DTCWTM  0              //we. sygnalu timera zasobnika (1F)
#define _DTCWMG  0              //histereza wylaczenia CW(4F)
#define _DTPPGD  99             //domyslna wartosc nastawy KT dla aktywnego regulatora pogodowego
#endif
//Maxiterm
#if _TYPKT==_INWEST2
#define _DTAMST 40			//moc startowa
#define _DTAMCW	99			//moc maksymalna dla WU
#define _DTAMCO	99			//moc maksymalna dla WG
#define _DTARGZ _ZIEMNY		//wybor rodzaju gazu
#define _DTAWTK	_BITERM2	//wybor typu kotla
#define _DTAPCO	_MAX_PCO	//wybor typu kotla
#define _DTAPCW	F1_MAX_PCW	//wybor typu kotla
#define _DTAPFN	_PFZMA		//wybor typu kotla
#define _DTAPDG	0			//ogrzewanie podlogowe
#define _DTACS5	0			//czujnik cisnienia 5V

#define _DTRDPM	_DPWMPMP			//rodzaj pompy
#define _DTDLPM	6			//delta temperaturowa CO dla pompy
#define _DTMNPM	50			//granica minimalna predkosci pompy
#define _DTMXPM	80			//granica maksylna predkosci pompy
#define _DTMXPM2	99		//granica maksylna predkosci pompy dla WU
#define _DTHSCO  5			//histereza wylaczenia grzania CO (ponad nastawe)
#define _DTCONW  75			//temp CO przy grzaniu CW zasobnika
#define _DTPROP  0			//poprawka dla regulagtora pogodowego
#define _DTPGMX _MAX_PCO    //maksymalna wartosc graniczna CO dla regulatora pogodowego
#define _DTPGDR 1               //tryb pracy regulatora pogodowego
#define _DTCWTM  0              //we. sygnalu timera zasobnika (1F)
#define _DTCWMG  0              //histereza wylaczenia CW(4F)
#define _DTPPGD  99             //domyslna wartosc nastawy KT dla aktywnego regulatora pogodowego
#endif
//MiniMax z hydroblokiem
#if _TYPKT==_MMAXPHB
#define _DTAMST 40			//moc startowa
#define _DTAMCW	99			//moc maksymalna dla WU
#define _DTAMCO	99			//moc maksymalna dla WG
#define _DTARGZ _ZIEMNY		//wybor rodzaju gazu
#define _DTAWTK	_MONOHB		//wybor typu kotla
#define _DTAPCO	_MAX_PCO	//wybor typu kotla
#define _DTAPCW	F1_MAX_PCW	//wybor typu kotla
#define _DTAPFN	_PFZMA		//wybor typu kotla
#define _DTAPDG	0			//ogrzewanie podlogowe
#define _DTACS5	0			//czujnik cisnienia 5V

#define _DTRDPM	_DPWMPMP			//rodzaj pompy
#define _DTDLPM	6			//delta temperaturowa CO dla pompy
#define _DTMNPM	50			//granica minimalna predkosci pompy
#define _DTMXPM	80			//granica maksylna predkosci pompy
#define _DTMXPM2	99		//granica maksylna predkosci pompy dla WU
#define _DTHSCO  5			//histereza wylaczenia grzania CO (ponad nastawe)
#define _DTCONW  75			//temp CO przy grzaniu CW zasobnika
#define _DTPROP  0			//poprawka dla regulagtora pogodowego
#define _DTPGMX _MAX_PCO    //maksymalna wartosc graniczna CO dla regulatora pogodowego
#define _DTPGDR 1               //tryb pracy regulatora pogodowego
#define _DTCWTM  0              //we. sygnalu timera zasobnika (1F)
#define _DTCWMG  0              //histereza wylaczenia CW(4F)
#define _DTPPGD  99             //domyslna wartosc nastawy KT dla aktywnego regulatora pogodowego
#endif
//-------------------------------------------------------------------------------------
#define _DTATAL	0			//aktywacja timera AntyLegionella
#define _DTARTA	0			//wartosc poczatkowa timera AntyLegionella

#define _DTAOBG	1			//domyslny obieg zamkniety (dot. UniCo

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//----------------------------------------------------------
#endif
#endif		/*_DEFPAR_H*/
