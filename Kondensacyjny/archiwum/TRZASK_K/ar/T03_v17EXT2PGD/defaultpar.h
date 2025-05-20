/*defaultpar.h*/
#ifndef	_DEFPAR_H
#define	_DEFPAR_H
#include "main.h"
#include "konfig.h"
#include "pomiar.h"
#include "global.h"
//adresy danych przechowywanych w pamieci nieulotnej eeprom
#define _ADRERR 0x00C0		//kod awaryjny blokady trwalej
#define _ADRMST	0x00C1		//moc startowa
#define _ADRMCW	0x00C2		//moc maksymalna dla WU
#define _ADRMCO	0x00C3		//moc maksymalna dla WG
#define _ADRPDG	0x00C4		//ogrzewanie podlogowe
#define _ADRWKK	0x00C5		//kod kotla
#define _ADRTAL	0x00C6		//timer antyleg. aktywacja
#define _ADRRTA	0x00C7		//timer antyleg. wartosc
#define _ADROBG	0x00C8		//rodzaj obiegu
#define _ADRPCO	0x00C9		//nastawa PCO
#define _ADRPCW	0x00CA		//nastawa PCW
#define _ADRPFN	0x00CB		//pozycja PFN
#define _ADRNOK	0x00CC		//liczba okresow na sek. (tachometer)
#define _ADRCS5	0x00CD		//czujnik cisnienia 5V

#define _ADRMDG	  0x00CE		//dolna granica predkosci wentylatora
#define _ADRMGG	  0x00CF		//gorna granica predkosci wentylatora
#define _ADRRDPM  0x00D0		//rodzaj pompy
#define _ADRDLPM  0x00D1		//delta temperaturowa CO dla pompy
#define _ADRMNPM  0x00D2		//granica minimalna predkosci pompy
#define _ADRTECO  0x00D3		//tryb pracy ECO
#define _ADRECO   0x00D4		//wspolczynnik ECO
#define _ADRTMCO2 0x00D5		//czas pracy przy obnizonej mocy w trybie CO
#define _ADRMCO2  0x00D6		//wartosc obnizonej mocy w trybie CO
#define _ADRMXPM  0x00D7     	//granica maksymalna predkosci pompy CO
#define _ADRALIN  0x00D8     	//adres LIN
#define _ADRMXPM2 0x00D9     	//granica maksymalna predkosci pompy CW
#define _ADRHSCO  0x00DA		//histereza wylaczenia grzania CO (ponad nastawe)
#define _ADRCONW  0x00DB		//temp CO przy grzaniu CW zasobnika
#define _ADRPROP  0x00DC		//poprawka dla regulagtora pogodowego
#define _ADRRFAN  0x00DD		//rodzaj wentylatora
#define _ADRTSTR  0x00DE		//czas wybiegu na mocy startowej
#define _ADRTL3   0x00DF		//czas blokady L3

//adresy pozycji tablicy kalibracji wentylatora
#define _ADRVN0 0x00E0		//pozycja 0
#define _ADRPW0	0x00E1		//pozycja 0
#define _ADRVN1	0x00E2		//pozycja 1
#define _ADRPW1	0x00E3		//pozycja 1
#define _ADRVN2 0x00E4		//pozycja 2
#define _ADRPW2	0x00E5		//pozycja 2
#define _ADRVN3	0x00E6		//pozycja 3
#define _ADRPW3	0x00E7		//pozycja 3
#define _ADRVN4	0x00E8		//pozycja 4
#define _ADRPW4	0x00E9		//pozycja 4
#define _ADRVN5	0x00EA		//pozycja 5
#define _ADRPW5	0x00EB		//pozycja 5
#define _ADRVN6	0x00EC		//pozycja 6
#define _ADRPW6	0x00ED		//pozycja 6
#define _ADRVN7	0x00EE		//pozycja 7
#define _ADRPW7	0x00EF		//pozycja 7


#define _ADRPGDR  0x01C0		//rodzaj regulatora pogodowego
#define _ADRPGDG  0x01C1		//granica pobudzenia dla regulatora pogodowego
#define _ADRPGDO  0x01C2		//obnizenie nocne dla regulatora pogodowego
#define _ADRPGMX  0x01C3		//maksymalna wartosc graniczna CO dla regulatora pogodowego
#define _ADRCWTM  0x01C4		//we. sygnalu timera zasobnika (1F) [P30]
#define _ADRCWMG  0x01C5		//histereza wylaczenia CW(2F) [P30]
#define _ADRPPGD  0x01C6        //nastawa regulatora pogodowego

#define _ADRKRES  0x01D7        //flaga wymuszonej kalibracji wentylatora po programowym restarcie oprogramowania

//Historia zdarzen awaryjnych
#define _ADR_HIND  0x01E8		//indeks wskazujacy na miejsce zapisu kolejnego zdarzenia
#define _ADR_SHBF  0x01E9       //pamiec na potrzeby eliminacji powtorzen zapisu dla tego samego bledu
#define _ADR_HNPZ  0x01EA       //liczba zapisanych zdarzen
#define _ADR_HRFL  0x01EB		//licznik zapisow LB
#define _ADR_HRFH  0x01EC		//licznik zapisow HB
#define _ADR_HFLG  0x01ED       //flaga serwisowa (licznik)
#define _ADR_HBUF  0x01EE       //adres bufora historii zdarzen (rozmiar _HISTSZ)

//Status wylaczenia awaryjnego
#define _ADRNZEE   0x0200		//liczba wylaczen awaryjnych z blokada (2 bajty)
//                 0x0201
#define _ADRSTPZ   0x0202		//liczba zapisanych statusow wylaczenia awaryjnego z blokada (1 bajt)

#define _ADRSTEE0  0x0208		//zrzut statusu kotla po wylaczeniu awaryjnym z blokada

#define _ADRSTEE1  0x0228		//zrzut statusu kotla po wylaczeniu awaryjnym z blokada

#define _ADRSTEE2  0x0248		//zrzut statusu kotla po wylaczeniu awaryjnym z blokada

#define _ADRSTEE3  0x0268		//zrzut statusu kotla po wylaczeniu awaryjnym z blokada



//----------------------------------------------------------------
//----------------------------------------------------------------
//wartosci domyslne danych w pamieci (pierwsze programowanie)
#define _DTAERR	0			//kod awaryjny
//----------
#define _DPWMPMP 1			// ! domyslny typ pompy (1-PWM)
#define _DPRFAN 0			// ! domyslny typ wentylatora (0-FIME,1-NG40)

#define _DTPGDR 1               //rodzaj regulatora pogodowego
#define _DTPGDG 18              //wartosc graniczna pobudzenia od regulatora pogodowego
#define _DTPGDO 5               //obnizenie nocne dla regulatora pogodowego
#define _DTPGMX _MAX_PCO        //maksymalna wartosc graniczna CO dla regulatora pogodowego
#define _DTCWTM  0              //we. sygnalu timera zasobnika (1F)
#define _DTCWMG  0              //histereza wylaczenia CW(2F)
#define _DTPPGD  99             //domyslna wartosc nastawy KT dla aktywnego regulatora pogodowego
//---------
#define _DEFKRES  0             //flaga wymuszonej kalibracji wentylatora po programowym restarcie oprogramowania
//----------------------------------------------------------------
//----------------------------------------------------------------
//Domyslne parametry wentylatora dla wybranego typu kotla
//(wartosci graniczne w pliku konfig.h)
	//r6=0	
	//r6=1	(1f)	LABUSE
	#define _DTMMIN1 15			//12granica minimalna obrotow
	#define _DTMMAX1 56			//56granica maksymalna obrotow

	//r6=2	(2f)	LABUSE
	#define _DTMMIN2 15			//12granica minimalna obrotow
	#define _DTMMAX2 56			//56granica maksymalna obrotow

	//r6=3	(1f)	1800.90.00.00
	#define _DTMMIN3 16			//granica minimalna obrotow
	#define _DTMMAX3 48			//granica maksymalna obrotow

	//r6=4	(2f)	1780.90.00.00
	#define _DTMMIN4 16			//granica minimalna obrotow
	#define _DTMMAX4 48			//granica maksymalna obrotow

	//r6=5	(1f)	1800.89.00.00
	#define _DTMMIN5 16			//granica minimalna obrotow
	#define _DTMMAX5 46			//granica maksymalna obrotow

	//r6=6	(2f)	1780.89.00.00
	#define _DTMMIN6 16			//granica minimalna obrotow
	#define _DTMMAX6 55			//granica maksymalna obrotow

	//r6=7	(1f)	1800.88.00.00
	#define _DTMMIN7 16			//granica minimalna obrotow
	#define _DTMMAX7 58			//granica maksymalna obrotow

	//r6=8	(2f)	1780.88.00.00
	#define _DTMMIN8 16			//granica minimalna obrotow
	#define _DTMMAX8 58			//granica maksymalna obrotow

	//r6=9	(1f)	1800.87.00.00
	#define _DTMMIN9 16			//granica minimalna obrotow
	#define _DTMMAX9 34			//granica maksymalna obrotow

	//r6=10	(2f)	1780.87.00.00
	#define _DTMMIN10 16			//granica minimalna obrotow
	#define _DTMMAX10 34			//granica maksymalna obrotow

	//r6=11	(1f)	1800.86.00.00
	#define _DTMMIN11 16			//granica minimalna obrotow
	#define _DTMMAX11 44			//granica maksymalna obrotow

	//r6=12	(2f)	1780.86.00.00
	#define _DTMMIN12 16			//granica minimalna obrotow
	#define _DTMMAX12 44			//granica maksymalna obrotow

	//r6=13	(1f)	1800.85.00.00
	#define _DTMMIN13 16			//granica minimalna obrotow
	#define _DTMMAX13 48			//granica maksymalna obrotow
			
	//r6=14	(2f)	1780.85.00.00
	#define _DTMMIN14 16			//granica minimalna obrotow
	#define _DTMMAX14 58			//granica maksymalna obrotow

	//r6=15	(1f)	1800.84.00.00
	#define _DTMMIN15 16			//granica minimalna obrotow
	#define _DTMMAX15 32			//granica maksymalna obrotow

	//r6=16	(2f)	1780.84.00.00
	#define _DTMMIN16 16			//granica minimalna obrotow
	#define _DTMMAX16 39			//granica maksymalna obrotow

	//r6=17	(1f)	1800.83.00.00
	#define _DTMMIN17 16			//granica minimalna obrotow
	#define _DTMMAX17 44			//granica maksymalna obrotow

	//r6=18	(2f)	1780.83.00.00
	#define _DTMMIN18 16			//granica minimalna obrotow
	#define _DTMMAX18 52			//granica maksymalna obrotow

	//r6=19	(1f)	1800.82.00.00
	#define _DTMMIN19 16			//granica minimalna obrotow
	#define _DTMMAX19 46			//granica maksymalna obrotow

	//r6=20	(2f)	1780.82.00.00
	#define _DTMMIN20 16			//granica minimalna obrotow
	#define _DTMMAX20 46			//granica maksymalna obrotow

	//r6=21	(1f)	1800.81.00.00
	#define _DTMMIN21 16			//granica minimalna obrotow
	#define _DTMMAX21 54			//granica maksymalna obrotow

	//r6=22	(2f)	1780.81.00.00
	#define _DTMMIN22 16			//granica minimalna obrotow
	#define _DTMMAX22 54			//granica maksymalna obrotow


//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//Parametry domyslne dla wybranego typu kotla (obowiazujace po zaprogramowaniu sterownika)
//Kotly jednofunkcyjne
#if _KODKT==1					//LABUSE
	//jednofunkcyjny
	#define _DTAMST 45			//moc startowa
	#define _DTAMCW	99			//moc maksymalna dla WU
	#define _DTAMCO	99			//moc maksymalna dla WG
	#define _DTAPDG	0			//ogrzewanie podlogowe
	#define _DTAWKK	_KODKT		//wybor typu kotla (jednofunkcyjny)
	#if _DTAPDG==0					//ogrzewanie podlogowe?
		#define _DTAPCO	_MAX_PCO	//nastawa pco
	#else
		#define _DTAPCO	_MAX_PDCO	//nastawa pco
	#endif
	#define _DTAPCW	F3_MAX_PCW	//nastawa pcw
	#define _DTAPFN	(_PFZMA<<4)|_PFZMA		//pozycja ZIMA
	#define _DTANOK 2			//liczba okresow na sek. (tachometer)
	#define _DTACS5	1			//czujnik cisnienia 5V
	#define _DTATAL	0			//aktywacja timera AntyLegionella
	#define _DTARTA	0			//wartosc poczatkowa timera AntyLegionella
	#define _DTAOBG	1			//domyslny obieg zamkniety (dot. UniCo
	#define _DTVMD _DTMMIN1		//granica minimalna obrotow
	#define _DTVMG _DTMMAX1		//granica maksymalna obrotow
	#define _DTRDPM	_DPWMPMP			//rodzaj pompy
	#define _DTDLPM	6			//delta temperaturowa CO dla pompy
	#define _DTMNPM	50			//granica minimalna predkosci pompy
	#define _DTTECO	0			//tryb pracy ECO
	#define _DTAECO	5			//domyslna nastawa ECO
	#define _DTTMCO2 0			//czas pracy przy obnizonej mocy w trybie CO
	#define _DTMCO2	10			//wartosc obnizonej mocy w trybie CO
	#define _DTMXPM	80			//granica maksylna predkosci pompy
	#define _DTALIN	_MY_LADDR                       //domyslny adres LIN
	#define _DTMXPM2	99		//granica maksylna predkosci pompy dla WU
	#define _DTHSCO  5			//histereza wylaczenia grzania CO (ponad nastawe)
	#define _DTCONW  75			//temp CO przy grzaniu CW zasobnika
	#define _DTPROP  0			//poprawka dla regulagtora pogodowego
	#define _DTRFAN	_DPRFAN     //rodzaj wentylatora
	#define _DTTSTR	20          //czas wybiegu na mocy startowej (tryb pracy CO)
	#define _DTTL3	3           //blokady L3 [min] (tryb pracy CO)
	#define _DTPGDR 1           //rodzaj regulatora pogodowego
	#define _DTPGDG 18          //wartosc graniczna pobudzenia od regulatora pogodowego
	#define _DTPGDO 5           //obnizenie nocne dla regulatora pogodowego
#endif
#if _KODKT==3
	//jednofunkcyjny
	#define _DTAMST 45			//moc startowa
	#define _DTAMCW	99			//moc maksymalna dla WU
	#define _DTAMCO	99			//moc maksymalna dla WG
	#define _DTAPDG	0			//ogrzewanie podlogowe
	#define _DTAWKK	_KODKT		//wybor typu kotla (jednofunkcyjny)
	#if _DTAPDG==0					//ogrzewanie podlogowe?
		#define _DTAPCO	_MAX_PCO	//nastawa pco
	#else
		#define _DTAPCO	_MAX_PDCO	//nastawa pco
	#endif
	#define _DTAPCW	F3_MAX_PCW	//nastawa pcw
	#define _DTAPFN	(_PFZMA<<4)|_PFZMA		//pozycja ZIMA
	#define _DTANOK 2			//liczba okresow na sek. (tachometer)
	#define _DTACS5	1			//czujnik cisnienia 5V
	#define _DTATAL	0			//aktywacja timera AntyLegionella
	#define _DTARTA	0			//wartosc poczatkowa timera AntyLegionella
	#define _DTAOBG	1			//domyslny obieg zamkniety (dot. UniCo
	#define _DTVMD _DTMMIN3		//granica minimalna obrotow
	#define _DTVMG _DTMMAX3		//granica maksymalna obrotow
	#define _DTRDPM	_DPWMPMP			//rodzaj pompy
	#define _DTDLPM	6			//delta temperaturowa CO dla pompy
	#define _DTMNPM	50			//granica minimalna predkosci pompy
	#define _DTTECO	0			//tryb pracy ECO
	#define _DTAECO	5			//domyslna nastawa ECO
	#define _DTTMCO2 0			//czas pracy przy obnizonej mocy w trybie CO
	#define _DTMCO2	10			//wartosc obnizonej mocy w trybie CO
    #define _DTMXPM	80			//granica maksylna predkosci pompy
    #define _DTALIN	_MY_LADDR
	#define _DTMXPM2	99		//granica maksylna predkosci pompy dla WU
	#define _DTHSCO  5			//histereza wylaczenia grzania CO (ponad nastawe)
	#define _DTCONW  75			//temp CO przy grzaniu CW zasobnika
	#define _DTPROP  0			//poprawka dla regulagtora pogodowego
	#define _DTRFAN	_DPRFAN     //rodzaj wentylatora
	#define _DTTSTR	20           //czas wybiegu na mocy startowej (tryb pracy CO)
	#define _DTTL3	3           //blokady L3 [min] (tryb pracy CO)
	#define _DTPGDR 1           //rodzaj regulatora pogodowego
	#define _DTPGDG 18          //wartosc graniczna pobudzenia od regulatora pogodowego
	#define _DTPGDO 5           //obnizenie nocne dla regulatora pogodowego
#endif
#if _KODKT==5
	//jednofunkcyjny
	#define _DTAMST 45			//moc startowa
	#define _DTAMCW	99			//moc maksymalna dla WU
	#define _DTAMCO	99			//moc maksymalna dla WG
	#define _DTAPDG	0			//ogrzewanie podlogowe
	#define _DTAWKK	_KODKT		//wybor typu kotla (jednofunkcyjny)
	#if _DTAPDG==0					//ogrzewanie podlogowe?
		#define _DTAPCO	_MAX_PCO	//nastawa pco
	#else
		#define _DTAPCO	_MAX_PDCO	//nastawa pco
	#endif
	#define _DTAPCW	F3_MAX_PCW	//nastawa pcw
	#define _DTAPFN	(_PFZMA<<4)|_PFZMA		//pozycja ZIMA
	#define _DTANOK 2			//liczba okresow na sek. (tachometer)
	#define _DTACS5	1			//czujnik cisnienia 5V
	#define _DTATAL	0			//aktywacja timera AntyLegionella
	#define _DTARTA	0			//wartosc poczatkowa timera AntyLegionella
	#define _DTAOBG	1			//domyslny obieg zamkniety (dot. UniCo
	#define _DTVMD _DTMMIN5		//granica minimalna obrotow
	#define _DTVMG _DTMMAX5		//granica maksymalna obrotow
	#define _DTRDPM	_DPWMPMP			//rodzaj pompy
	#define _DTDLPM	6			//delta temperaturowa CO dla pompy
	#define _DTMNPM	50			//granica minimalna predkosci pompy
	#define _DTTECO	0			//tryb pracy ECO
	#define _DTAECO	5			//domyslna nastawa ECO
	#define _DTTMCO2 0			//czas pracy przy obnizonej mocy w trybie CO
	#define _DTMCO2	10			//wartosc obnizonej mocy w trybie CO
    #define _DTMXPM	80			//granica maksylna predkosci pompy
    #define _DTALIN	_MY_LADDR
	#define _DTMXPM2	99		//granica maksylna predkosci pompy dla WU
	#define _DTHSCO  5			//histereza wylaczenia grzania CO (ponad nastawe)
	#define _DTCONW  75			//temp CO przy grzaniu CW zasobnika
	#define _DTPROP  0			//poprawka dla regulagtora pogodowego
	#define _DTRFAN	_DPRFAN     //rodzaj wentylatora
	#define _DTTSTR	20           //czas wybiegu na mocy startowej (tryb pracy CO)
	#define _DTTL3	3           //blokady L3 [min] (tryb pracy CO)
	#define _DTPGDR 1           //rodzaj regulatora pogodowego
	#define _DTPGDG 18          //wartosc graniczna pobudzenia od regulatora pogodowego
	#define _DTPGDO 5           //obnizenie nocne dla regulatora pogodowego
#endif
#if _KODKT==7
	//jednofunkcyjny
	#define _DTAMST 45			//moc startowa
	#define _DTAMCW	99			//moc maksymalna dla WU
	#define _DTAMCO	99			//moc maksymalna dla WG
	#define _DTAPDG	0			//ogrzewanie podlogowe
	#define _DTAWKK	_KODKT		//wybor typu kotla (jednofunkcyjny)
	#if _DTAPDG==0					//ogrzewanie podlogowe?
		#define _DTAPCO	_MAX_PCO	//nastawa pco
	#else
		#define _DTAPCO	_MAX_PDCO	//nastawa pco
	#endif
	#define _DTAPCW	F3_MAX_PCW	//nastawa pcw
	#define _DTAPFN	(_PFZMA<<4)|_PFZMA		//pozycja ZIMA
	#define _DTANOK 2			//liczba okresow na sek. (tachometer)
	#define _DTACS5	1			//czujnik cisnienia 5V
	#define _DTATAL	0			//aktywacja timera AntyLegionella
	#define _DTARTA	0			//wartosc poczatkowa timera AntyLegionella
	#define _DTAOBG	1			//domyslny obieg zamkniety (dot. UniCo
	#define _DTVMD _DTMMIN7		//granica minimalna obrotow
	#define _DTVMG _DTMMAX7		//granica maksymalna obrotow
	#define _DTRDPM	_DPWMPMP			//rodzaj pompy
	#define _DTDLPM	6			//delta temperaturowa CO dla pompy
	#define _DTMNPM	50			//granica minimalna predkosci pompy
	#define _DTTECO	0			//tryb pracy ECO
	#define _DTAECO	5			//domyslna nastawa ECO
	#define _DTTMCO2 0			//czas pracy przy obnizonej mocy w trybie CO
	#define _DTMCO2	10			//wartosc obnizonej mocy w trybie CO
    #define _DTMXPM	99			//granica maksylna predkosci pompy
    #define _DTALIN	_MY_LADDR
	#define _DTMXPM2	99		//granica maksylna predkosci pompy dla WU
	#define _DTHSCO  5			//histereza wylaczenia grzania CO (ponad nastawe)
	#define _DTCONW  75			//temp CO przy grzaniu CW zasobnika
	#define _DTPROP  0			//poprawka dla regulagtora pogodowego
	#define _DTRFAN	_DPRFAN     //rodzaj wentylatora
	#define _DTTSTR	20           //czas wybiegu na mocy startowej (tryb pracy CO)
	#define _DTTL3	3           //blokady L3 [min] (tryb pracy CO)
	#define _DTPGDR 1           //rodzaj regulatora pogodowego
	#define _DTPGDG 18          //wartosc graniczna pobudzenia od regulatora pogodowego
	#define _DTPGDO 5           //obnizenie nocne dla regulatora pogodowego
#endif
#if _KODKT==9
	//jednofunkcyjny
	#define _DTAMST 45			//moc startowa
	#define _DTAMCW	99			//moc maksymalna dla WU
	#define _DTAMCO	99			//moc maksymalna dla WG
	#define _DTAPDG	0			//ogrzewanie podlogowe
	#define _DTAWKK	_KODKT		//wybor typu kotla (jednofunkcyjny)
	#if _DTAPDG==0					//ogrzewanie podlogowe?
		#define _DTAPCO	_MAX_PCO	//nastawa pco
	#else
		#define _DTAPCO	_MAX_PDCO	//nastawa pco
	#endif
	#define _DTAPCW	F3_MAX_PCW	//nastawa pcw
	#define _DTAPFN	(_PFZMA<<4)|_PFZMA		//pozycja ZIMA
	#define _DTANOK 2			//liczba okresow na sek. (tachometer)
	#define _DTACS5	1			//czujnik cisnienia 5V
	#define _DTATAL	0			//aktywacja timera AntyLegionella
	#define _DTARTA	0			//wartosc poczatkowa timera AntyLegionella
	#define _DTAOBG	1			//domyslny obieg zamkniety (dot. UniCo
	#define _DTVMD _DTMMIN9		//granica minimalna obrotow
	#define _DTVMG _DTMMAX9		//granica maksymalna obrotow
	#define _DTRDPM	_DPWMPMP			//rodzaj pompy
	#define _DTDLPM	6			//delta temperaturowa CO dla pompy
	#define _DTMNPM	50			//granica minimalna predkosci pompy
	#define _DTTECO	0			//tryb pracy ECO
	#define _DTAECO	5			//domyslna nastawa ECO
	#define _DTTMCO2 0			//czas pracy przy obnizonej mocy w trybie CO
	#define _DTMCO2	10			//wartosc obnizonej mocy w trybie CO
    #define _DTMXPM	99			//granica maksylna predkosci pompy
    #define _DTALIN	_MY_LADDR
	#define _DTMXPM2	99		//granica maksylna predkosci pompy dla WU
	#define _DTHSCO  5			//histereza wylaczenia grzania CO (ponad nastawe)
	#define _DTCONW  75			//temp CO przy grzaniu CW zasobnika
	#define _DTPROP  0			//poprawka dla regulagtora pogodowego
	#define _DTRFAN	_DPRFAN     //rodzaj wentylatora
	#define _DTTSTR	20           //czas wybiegu na mocy startowej (tryb pracy CO)
	#define _DTTL3	3           //blokady L3 [min] (tryb pracy CO)
	#define _DTPGDR 1           //rodzaj regulatora pogodowego
	#define _DTPGDG 18          //wartosc graniczna pobudzenia od regulatora pogodowego
	#define _DTPGDO 5           //obnizenie nocne dla regulatora pogodowego
#endif
#if _KODKT==11
	//jednofunkcyjny
	#define _DTAMST 45			//moc startowa
	#define _DTAMCW	99			//moc maksymalna dla WU
	#define _DTAMCO	99			//moc maksymalna dla WG
	#define _DTAPDG	0			//ogrzewanie podlogowe
	#define _DTAWKK	_KODKT		//wybor typu kotla (jednofunkcyjny)
	#if _DTAPDG==0					//ogrzewanie podlogowe?
		#define _DTAPCO	_MAX_PCO	//nastawa pco
	#else
		#define _DTAPCO	_MAX_PDCO	//nastawa pco
	#endif
	#define _DTAPCW	F3_MAX_PCW	//nastawa pcw
	#define _DTAPFN	(_PFZMA<<4)|_PFZMA		//pozycja ZIMA
	#define _DTANOK 2			//liczba okresow na sek. (tachometer)
	#define _DTACS5	1			//czujnik cisnienia 5V
	#define _DTATAL	0			//aktywacja timera AntyLegionella
	#define _DTARTA	0			//wartosc poczatkowa timera AntyLegionella
	#define _DTAOBG	1			//domyslny obieg zamkniety (dot. UniCo
	#define _DTVMD _DTMMIN11		//granica minimalna obrotow
	#define _DTVMG _DTMMAX11		//granica maksymalna obrotow
	#define _DTRDPM	_DPWMPMP			//rodzaj pompy
	#define _DTDLPM	6			//delta temperaturowa CO dla pompy
	#define _DTMNPM	50			//granica minimalna predkosci pompy
	#define _DTTECO	0			//tryb pracy ECO
	#define _DTAECO	5			//domyslna nastawa ECO
	#define _DTTMCO2 0			//czas pracy przy obnizonej mocy w trybie CO
	#define _DTMCO2	10			//wartosc obnizonej mocy w trybie CO
    #define _DTMXPM	99			//granica maksylna predkosci pompy
    #define _DTALIN	_MY_LADDR
	#define _DTMXPM2	99		//granica maksylna predkosci pompy dla WU
	#define _DTHSCO  5			//histereza wylaczenia grzania CO (ponad nastawe)
	#define _DTCONW  75			//temp CO przy grzaniu CW zasobnika
	#define _DTPROP  0			//poprawka dla regulagtora pogodowego
	#define _DTRFAN	_DPRFAN     //rodzaj wentylatora
	#define _DTTSTR	20          //czas wybiegu na mocy startowej (tryb pracy CO)
	#define _DTTL3	3           //blokady L3 [min] (tryb pracy CO)
	#define _DTPGDR 1           //rodzaj regulatora pogodowego
	#define _DTPGDG 18          //wartosc graniczna pobudzenia od regulatora pogodowego
	#define _DTPGDO 5           //obnizenie nocne dla regulatora pogodowego
#endif
#if _KODKT==13
	//jednofunkcyjny
	#define _DTAMST 45			//moc startowa
	#define _DTAMCW	99			//moc maksymalna dla WU
	#define _DTAMCO	99			//moc maksymalna dla WG
	#define _DTAPDG	0			//ogrzewanie podlogowe
	#define _DTAWKK	_KODKT		//wybor typu kotla (jednofunkcyjny)
	#if _DTAPDG==0					//ogrzewanie podlogowe?
		#define _DTAPCO	_MAX_PCO	//nastawa pco
	#else
		#define _DTAPCO	_MAX_PDCO	//nastawa pco
	#endif
	#define _DTAPCW	F3_MAX_PCW	//nastawa pcw
	#define _DTAPFN	(_PFZMA<<4)|_PFZMA		//pozycja ZIMA
	#define _DTANOK 2			//liczba okresow na sek. (tachometer)
	#define _DTACS5	1			//czujnik cisnienia 5V
	#define _DTATAL	0			//aktywacja timera AntyLegionella
	#define _DTARTA	0			//wartosc poczatkowa timera AntyLegionella
	#define _DTAOBG	1			//domyslny obieg zamkniety (dot. UniCo
	#define _DTVMD _DTMMIN13		//granica minimalna obrotow
	#define _DTVMG _DTMMAX13		//granica maksymalna obrotow
	#define _DTRDPM	_DPWMPMP			//rodzaj pompy
	#define _DTDLPM	6			//delta temperaturowa CO dla pompy
	#define _DTMNPM	50			//granica minimalna predkosci pompy
	#define _DTTECO	0			//tryb pracy ECO
	#define _DTAECO	5			//domyslna nastawa ECO
	#define _DTTMCO2 0			//czas pracy przy obnizonej mocy w trybie CO
	#define _DTMCO2	10			//wartosc obnizonej mocy w trybie CO
    #define _DTMXPM	99			//granica maksylna predkosci pompy
    #define _DTALIN	_MY_LADDR
	#define _DTMXPM2	99		//granica maksylna predkosci pompy dla WU
	#define _DTHSCO  5			//histereza wylaczenia grzania CO (ponad nastawe)
	#define _DTCONW  75			//temp CO przy grzaniu CW zasobnika
	#define _DTPROP  0			//poprawka dla regulagtora pogodowego
	#define _DTRFAN	_DPRFAN     //rodzaj wentylatora
	#define _DTTSTR	20           //czas wybiegu na mocy startowej (tryb pracy CO)
	#define _DTTL3	3           //blokady L3 [min] (tryb pracy CO)
	#define _DTPGDR 1           //rodzaj regulatora pogodowego
	#define _DTPGDG 18          //wartosc graniczna pobudzenia od regulatora pogodowego
	#define _DTPGDO 5           //obnizenie nocne dla regulatora pogodowego
#endif
#if _KODKT==15
	//jednofunkcyjny
	#define _DTAMST 45			//moc startowa
	#define _DTAMCW	99			//moc maksymalna dla WU
	#define _DTAMCO	99			//moc maksymalna dla WG
	#define _DTAPDG	0			//ogrzewanie podlogowe
	#define _DTAWKK	_KODKT		//wybor typu kotla (jednofunkcyjny)
	#if _DTAPDG==0					//ogrzewanie podlogowe?
		#define _DTAPCO	_MAX_PCO	//nastawa pco
	#else
		#define _DTAPCO	_MAX_PDCO	//nastawa pco
	#endif
	#define _DTAPCW	F3_MAX_PCW	//nastawa pcw
	#define _DTAPFN	(_PFZMA<<4)|_PFZMA		//pozycja ZIMA
	#define _DTANOK 2			//liczba okresow na sek. (tachometer)
	#define _DTACS5	1			//czujnik cisnienia 5V
	#define _DTATAL	0			//aktywacja timera AntyLegionella
	#define _DTARTA	0			//wartosc poczatkowa timera AntyLegionella
	#define _DTAOBG	1			//domyslny obieg zamkniety (dot. UniCo
	#define _DTVMD _DTMMIN15		//granica minimalna obrotow
	#define _DTVMG _DTMMAX15		//granica maksymalna obrotow
	#define _DTRDPM	_DPWMPMP			//rodzaj pompy
	#define _DTDLPM	6			//delta temperaturowa CO dla pompy
	#define _DTMNPM	50			//granica minimalna predkosci pompy
	#define _DTTECO	0			//tryb pracy ECO
	#define _DTAECO	5			//domyslna nastawa ECO
	#define _DTTMCO2 0			//czas pracy przy obnizonej mocy w trybie CO
	#define _DTMCO2	10			//wartosc obnizonej mocy w trybie CO
    #define _DTMXPM	99			//granica maksylna predkosci pompy
    #define _DTALIN	_MY_LADDR
	#define _DTMXPM2	99		//granica maksylna predkosci pompy dla WU
	#define _DTHSCO  5			//histereza wylaczenia grzania CO (ponad nastawe)
	#define _DTCONW  75			//temp CO przy grzaniu CW zasobnika
	#define _DTPROP  0			//poprawka dla regulagtora pogodowego
	#define _DTRFAN	_DPRFAN     //rodzaj wentylatora
	#define _DTTSTR	20           //czas wybiegu na mocy startowej (tryb pracy CO)
	#define _DTTL3	3           //blokady L3 [min] (tryb pracy CO)
	#define _DTPGDR 1           //rodzaj regulatora pogodowego
	#define _DTPGDG 18          //wartosc graniczna pobudzenia od regulatora pogodowego
	#define _DTPGDO 5           //obnizenie nocne dla regulatora pogodowego
#endif
#if _KODKT==17
	//jednofunkcyjny
	#define _DTAMST 45			//moc startowa
	#define _DTAMCW	99			//moc maksymalna dla WU
	#define _DTAMCO	99			//moc maksymalna dla WG
	#define _DTAPDG	0			//ogrzewanie podlogowe
	#define _DTAWKK	_KODKT		//wybor typu kotla (jednofunkcyjny)
	#if _DTAPDG==0					//ogrzewanie podlogowe?
		#define _DTAPCO	_MAX_PCO	//nastawa pco
	#else
		#define _DTAPCO	_MAX_PDCO	//nastawa pco
	#endif
	#define _DTAPCW	F3_MAX_PCW	//nastawa pcw
	#define _DTAPFN	(_PFZMA<<4)|_PFZMA		//pozycja ZIMA
	#define _DTANOK 2			//liczba okresow na sek. (tachometer)
	#define _DTACS5	1			//czujnik cisnienia 5V
	#define _DTATAL	0			//aktywacja timera AntyLegionella
	#define _DTARTA	0			//wartosc poczatkowa timera AntyLegionella
	#define _DTAOBG	1			//domyslny obieg zamkniety (dot. UniCo
	#define _DTVMD _DTMMIN17		//granica minimalna obrotow
	#define _DTVMG _DTMMAX17		//granica maksymalna obrotow
	#define _DTRDPM	_DPWMPMP			//rodzaj pompy
	#define _DTDLPM	6			//delta temperaturowa CO dla pompy
	#define _DTMNPM	50			//granica minimalna predkosci pompy
	#define _DTTECO	0			//tryb pracy ECO
	#define _DTAECO	5			//domyslna nastawa ECO
	#define _DTTMCO2 0			//czas pracy przy obnizonej mocy w trybie CO
	#define _DTMCO2	10			//wartosc obnizonej mocy w trybie CO
    #define _DTMXPM	99			//granica maksylna predkosci pompy
    #define _DTALIN	_MY_LADDR
	#define _DTMXPM2	99		//granica maksylna predkosci pompy dla WU
	#define _DTHSCO  5			//histereza wylaczenia grzania CO (ponad nastawe)
	#define _DTCONW  75			//temp CO przy grzaniu CW zasobnika
	#define _DTPROP  0			//poprawka dla regulagtora pogodowego
	#define _DTRFAN	_DPRFAN     //rodzaj wentylatora
	#define _DTTSTR	20           //czas wybiegu na mocy startowej (tryb pracy CO)
	#define _DTTL3	3           //blokady L3 [min] (tryb pracy CO)
	#define _DTPGDR 1           //rodzaj regulatora pogodowego
	#define _DTPGDG 18          //wartosc graniczna pobudzenia od regulatora pogodowego
	#define _DTPGDO 5           //obnizenie nocne dla regulatora pogodowego
#endif
#if _KODKT==19
	//jednofunkcyjny
	#define _DTAMST 45			//moc startowa
	#define _DTAMCW	99			//moc maksymalna dla WU
	#define _DTAMCO	99			//moc maksymalna dla WG
	#define _DTAPDG	0			//ogrzewanie podlogowe
	#define _DTAWKK	_KODKT		//wybor typu kotla (jednofunkcyjny)
	#if _DTAPDG==0					//ogrzewanie podlogowe?
		#define _DTAPCO	_MAX_PCO	//nastawa pco
	#else
		#define _DTAPCO	_MAX_PDCO	//nastawa pco
	#endif
	#define _DTAPCW	F3_MAX_PCW	//nastawa pcw
	#define _DTAPFN	(_PFZMA<<4)|_PFZMA		//pozycja ZIMA
	#define _DTANOK 2			//liczba okresow na sek. (tachometer)
	#define _DTACS5	1			//czujnik cisnienia 5V
	#define _DTATAL	0			//aktywacja timera AntyLegionella
	#define _DTARTA	0			//wartosc poczatkowa timera AntyLegionella
	#define _DTAOBG	1			//domyslny obieg zamkniety (dot. UniCo
	#define _DTVMD _DTMMIN19		//granica minimalna obrotow
	#define _DTVMG _DTMMAX19		//granica maksymalna obrotow
	#define _DTRDPM	_DPWMPMP			//rodzaj pompy
	#define _DTDLPM	6			//delta temperaturowa CO dla pompy
	#define _DTMNPM	50			//granica minimalna predkosci pompy
	#define _DTTECO	0			//tryb pracy ECO
	#define _DTAECO	5			//domyslna nastawa ECO
	#define _DTTMCO2 0			//czas pracy przy obnizonej mocy w trybie CO
	#define _DTMCO2	10			//wartosc obnizonej mocy w trybie CO
    #define _DTMXPM	99			//granica maksylna predkosci pompy
    #define _DTALIN	_MY_LADDR
	#define _DTMXPM2	99		//granica maksylna predkosci pompy dla WU
	#define _DTHSCO  5			//histereza wylaczenia grzania CO (ponad nastawe)
	#define _DTCONW  75			//temp CO przy grzaniu CW zasobnika
	#define _DTPROP  0			//poprawka dla regulagtora pogodowego
	#define _DTRFAN	_DPRFAN     //rodzaj wentylatora
	#define _DTTSTR	20           //czas wybiegu na mocy startowej (tryb pracy CO)
	#define _DTTL3	3           //blokady L3 [min] (tryb pracy CO)
	#define _DTPGDR 1           //rodzaj regulatora pogodowego
	#define _DTPGDG 18          //wartosc graniczna pobudzenia od regulatora pogodowego
	#define _DTPGDO 5           //obnizenie nocne dla regulatora pogodowego
#endif
#if _KODKT==21
	//jednofunkcyjny
	#define _DTAMST 45			//moc startowa
	#define _DTAMCW	99			//moc maksymalna dla WU
	#define _DTAMCO	99			//moc maksymalna dla WG
	#define _DTAPDG	0			//ogrzewanie podlogowe
	#define _DTAWKK	_KODKT		//wybor typu kotla (jednofunkcyjny)
	#if _DTAPDG==0					//ogrzewanie podlogowe?
		#define _DTAPCO	_MAX_PCO	//nastawa pco
	#else
		#define _DTAPCO	_MAX_PDCO	//nastawa pco
	#endif
	#define _DTAPCW	F3_MAX_PCW	//nastawa pcw
	#define _DTAPFN	(_PFZMA<<4)|_PFZMA		//pozycja ZIMA
	#define _DTANOK 2			//liczba okresow na sek. (tachometer)
	#define _DTACS5	1			//czujnik cisnienia 5V
	#define _DTATAL	0			//aktywacja timera AntyLegionella
	#define _DTARTA	0			//wartosc poczatkowa timera AntyLegionella
	#define _DTAOBG	1			//domyslny obieg zamkniety (dot. UniCo
	#define _DTVMD _DTMMIN21		//granica minimalna obrotow
	#define _DTVMG _DTMMAX21		//granica maksymalna obrotow
	#define _DTRDPM	_DPWMPMP			//rodzaj pompy
	#define _DTDLPM	6			//delta temperaturowa CO dla pompy
	#define _DTMNPM	50			//granica minimalna predkosci pompy
	#define _DTTECO	0			//tryb pracy ECO
	#define _DTAECO	5			//domyslna nastawa ECO
	#define _DTTMCO2 0			//czas pracy przy obnizonej mocy w trybie CO
	#define _DTMCO2	10			//wartosc obnizonej mocy w trybie CO
    #define _DTMXPM	99			//granica maksylna predkosci pompy
    #define _DTALIN	_MY_LADDR
	#define _DTMXPM2	99		//granica maksylna predkosci pompy dla WU
	#define _DTHSCO  5			//histereza wylaczenia grzania CO (ponad nastawe)
	#define _DTCONW  75			//temp CO przy grzaniu CW zasobnika
	#define _DTPROP  0			//poprawka dla regulagtora pogodowego
	#define _DTRFAN	_DPRFAN     //rodzaj wentylatora
	#define _DTTSTR	20           //czas wybiegu na mocy startowej (tryb pracy CO)
	#define _DTTL3	3           //blokady L3 [min] (tryb pracy CO)
	#define _DTPGDR 1           //rodzaj regulatora pogodowego
	#define _DTPGDG 18          //wartosc graniczna pobudzenia od regulatora pogodowego
	#define _DTPGDO 5           //obnizenie nocne dla regulatora pogodowego
#endif
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//Kotly dwufunkcyjne
#if _KODKT==2
	//dwufunkcyjny
	#define _DTAMST 45			//moc startowa
	#define _DTAMCW	99			//moc maksymalna dla WU
	#define _DTAMCO	99			//moc maksymalna dla WG
	#define _DTAPDG	0			//ogrzewanie podlogowe
	#define _DTAWKK	_KODKT		//wybor typu kotla (dwufunkcyjny)
	#if _DTAPDG==0					//ogrzewanie podlogowe?
		#define _DTAPCO	_MAX_PCO	//nastawa pco
	#else
		#define _DTAPCO	_MAX_PDCO	//nastawa pco
	#endif
	#define _DTAPCW	F2_MAX_PCW	//nastawa pcw
	#define _DTAPFN	(_PFZMA<<4)|_PFZMA		//pozycja ZIMA
	#define _DTANOK 2			//liczba okresow na sek. (tachometer)
	#define _DTACS5	1			//czujnik cisnienia 5V
	#define _DTATAL	0			//aktywacja timera AntyLegionella
	#define _DTARTA	0			//wartosc poczatkowa timera AntyLegionella
	#define _DTAOBG	1			//domyslny obieg zamkniety (dot. UniCo
	#define _DTVMD _DTMMIN2		//granica minimalna obrotow
	#define _DTVMG _DTMMAX2		//granica maksymalna obrotow
	#define _DTRDPM	_DPWMPMP			//rodzaj pompy
	#define _DTDLPM	6			//delta temperaturowa CO dla pompy
	#define _DTMNPM	50			//granica minimalna predkosci pompy
	#define _DTTECO	0			//tryb pracy ECO
	#define _DTAECO	5			//domyslna nastawa ECO
	#define _DTTMCO2 0			//czas pracy przy obnizonej mocy w trybie CO
	#define _DTMCO2	10			//wartosc obnizonej mocy w trybie CO
    #define _DTMXPM	80			//granica maksylna predkosci pompy
    #define _DTALIN	_MY_LADDR
	#define _DTMXPM2	99		//granica maksylna predkosci pompy dla WU
	#define _DTHSCO  5			//histereza wylaczenia grzania CO (ponad nastawe)
	#define _DTCONW  75			//temp CO przy grzaniu CW zasobnika
	#define _DTPROP  0			//poprawka dla regulagtora pogodowego
	#define _DTRFAN	_DPRFAN     //rodzaj wentylatora
	#define _DTTSTR	20           //czas wybiegu na mocy startowej (tryb pracy CO)
	#define _DTTL3	3           //blokady L3 [min] (tryb pracy CO)
	#define _DTPGDR 1           //rodzaj regulatora pogodowego
	#define _DTPGDG 18          //wartosc graniczna pobudzenia od regulatora pogodowego
	#define _DTPGDO 5           //obnizenie nocne dla regulatora pogodowego
#endif
#if _KODKT==4
	//dwufunkcyjny
	#define _DTAMST 45			//moc startowa
	#define _DTAMCW	99			//moc maksymalna dla WU
	#define _DTAMCO	99			//moc maksymalna dla WG
	#define _DTAPDG	0			//ogrzewanie podlogowe
	#define _DTAWKK	_KODKT		//wybor typu kotla (dwufunkcyjny)
	#if _DTAPDG==0					//ogrzewanie podlogowe?
		#define _DTAPCO	_MAX_PCO	//nastawa pco
	#else
		#define _DTAPCO	_MAX_PDCO	//nastawa pco
	#endif
	#define _DTAPCW	F2_MAX_PCW	//nastawa pcw
	#define _DTAPFN	(_PFZMA<<4)|_PFZMA		//pozycja ZIMA
	#define _DTANOK 2			//liczba okresow na sek. (tachometer)
	#define _DTACS5	1			//czujnik cisnienia 5V
	#define _DTATAL	0			//aktywacja timera AntyLegionella
	#define _DTARTA	0			//wartosc poczatkowa timera AntyLegionella
	#define _DTAOBG	1			//domyslny obieg zamkniety (dot. UniCo
	#define _DTVMD _DTMMIN4		//granica minimalna obrotow
	#define _DTVMG _DTMMAX4		//granica maksymalna obrotow
	#define _DTRDPM	_DPWMPMP			//rodzaj pompy
	#define _DTDLPM	6			//delta temperaturowa CO dla pompy
	#define _DTMNPM	50			//granica minimalna predkosci pompy
	#define _DTTECO	0			//tryb pracy ECO
	#define _DTAECO	5			//domyslna nastawa ECO
	#define _DTTMCO2 0			//czas pracy przy obnizonej mocy w trybie CO
	#define _DTMCO2	10			//wartosc obnizonej mocy w trybie CO
    #define _DTMXPM	99			//granica maksylna predkosci pompy
    #define _DTALIN	_MY_LADDR
	#define _DTMXPM2	99		//granica maksylna predkosci pompy dla WU
	#define _DTHSCO  5			//histereza wylaczenia grzania CO (ponad nastawe)
	#define _DTCONW  75			//temp CO przy grzaniu CW zasobnika
	#define _DTPROP  0			//poprawka dla regulagtora pogodowego
	#define _DTRFAN	_DPRFAN     //rodzaj wentylatora
	#define _DTTSTR	20           //czas wybiegu na mocy startowej (tryb pracy CO)
	#define _DTTL3	3           //blokady L3 [min] (tryb pracy CO)
	#define _DTPGDR 1           //rodzaj regulatora pogodowego
	#define _DTPGDG 18          //wartosc graniczna pobudzenia od regulatora pogodowego
	#define _DTPGDO 5           //obnizenie nocne dla regulatora pogodowego
#endif
#if _KODKT==6
	//dwufunkcyjny
	#define _DTAMST 45			//moc startowa
	#define _DTAMCW	99			//moc maksymalna dla WU
	#define _DTAMCO	99			//moc maksymalna dla WG
	#define _DTAPDG	0			//ogrzewanie podlogowe
	#define _DTAWKK	_KODKT		//wybor typu kotla (dwufunkcyjny)
	#if _DTAPDG==0					//ogrzewanie podlogowe?
		#define _DTAPCO	_MAX_PCO	//nastawa pco
	#else
		#define _DTAPCO	_MAX_PDCO	//nastawa pco
	#endif
	#define _DTAPCW	F2_MAX_PCW	//nastawa pcw
	#define _DTAPFN	(_PFZMA<<4)|_PFZMA		//pozycja ZIMA
	#define _DTANOK 2			//liczba okresow na sek. (tachometer)
	#define _DTACS5	1			//czujnik cisnienia 5V
	#define _DTATAL	0			//aktywacja timera AntyLegionella
	#define _DTARTA	0			//wartosc poczatkowa timera AntyLegionella
	#define _DTAOBG	1			//domyslny obieg zamkniety (dot. UniCo
	#define _DTVMD _DTMMIN6		//granica minimalna obrotow
	#define _DTVMG _DTMMAX6		//granica maksymalna obrotow
	#define _DTRDPM	_DPWMPMP			//rodzaj pompy
	#define _DTDLPM	6			//delta temperaturowa CO dla pompy
	#define _DTMNPM	50			//granica minimalna predkosci pompy
	#define _DTTECO	0			//tryb pracy ECO
	#define _DTAECO	5			//domyslna nastawa ECO
	#define _DTTMCO2 0			//czas pracy przy obnizonej mocy w trybie CO
	#define _DTMCO2	10			//wartosc obnizonej mocy w trybie CO
    #define _DTMXPM	99			//granica maksylna predkosci pompy
    #define _DTALIN	_MY_LADDR
	#define _DTMXPM2	99		//granica maksylna predkosci pompy dla WU
	#define _DTHSCO  5			//histereza wylaczenia grzania CO (ponad nastawe)
	#define _DTCONW  75			//temp CO przy grzaniu CW zasobnika
	#define _DTPROP  0			//poprawka dla regulagtora pogodowego
	#define _DTRFAN	_DPRFAN     //rodzaj wentylatora
	#define _DTTSTR	20           //czas wybiegu na mocy startowej (tryb pracy CO)
	#define _DTTL3	3           //blokady L3 [min] (tryb pracy CO)
	#define _DTPGDR 1           //rodzaj regulatora pogodowego
	#define _DTPGDG 18          //wartosc graniczna pobudzenia od regulatora pogodowego
	#define _DTPGDO 5           //obnizenie nocne dla regulatora pogodowego
#endif
#if _KODKT==8
	//dwufunkcyjny
	#define _DTAMST 45			//moc startowa
	#define _DTAMCW	99			//moc maksymalna dla WU
	#define _DTAMCO	99			//moc maksymalna dla WG
	#define _DTAPDG	0			//ogrzewanie podlogowe
	#define _DTAWKK	_KODKT		//wybor typu kotla (dwufunkcyjny)
	#if _DTAPDG==0					//ogrzewanie podlogowe?
		#define _DTAPCO	_MAX_PCO	//nastawa pco
	#else
		#define _DTAPCO	_MAX_PDCO	//nastawa pco
	#endif
	#define _DTAPCW	F2_MAX_PCW	//nastawa pcw
	#define _DTAPFN	(_PFZMA<<4)|_PFZMA		//pozycja ZIMA
	#define _DTANOK 2			//liczba okresow na sek. (tachometer)
	#define _DTACS5	1			//czujnik cisnienia 5V
	#define _DTATAL	0			//aktywacja timera AntyLegionella
	#define _DTARTA	0			//wartosc poczatkowa timera AntyLegionella
	#define _DTAOBG	1			//domyslny obieg zamkniety (dot. UniCo
	#define _DTVMD _DTMMIN8		//granica minimalna obrotow
	#define _DTVMG _DTMMAX8		//granica maksymalna obrotow
	#define _DTRDPM	_DPWMPMP			//rodzaj pompy
	#define _DTDLPM	6			//delta temperaturowa CO dla pompy
	#define _DTMNPM	50			//granica minimalna predkosci pompy
	#define _DTTECO	0			//tryb pracy ECO
	#define _DTAECO	5			//domyslna nastawa ECO
	#define _DTTMCO2 0			//czas pracy przy obnizonej mocy w trybie CO
	#define _DTMCO2	10			//wartosc obnizonej mocy w trybie CO
    #define _DTMXPM	99			//granica maksylna predkosci pompy
    #define _DTALIN	_MY_LADDR
	#define _DTMXPM2	99		//granica maksylna predkosci pompy dla WU
	#define _DTHSCO  5			//histereza wylaczenia grzania CO (ponad nastawe)
	#define _DTCONW  75			//temp CO przy grzaniu CW zasobnika
	#define _DTPROP  0			//poprawka dla regulagtora pogodowego
	#define _DTRFAN	_DPRFAN     //rodzaj wentylatora
	#define _DTTSTR	20           //czas wybiegu na mocy startowej (tryb pracy CO)
	#define _DTTL3	3           //blokady L3 [min] (tryb pracy CO)
	#define _DTPGDR 1           //rodzaj regulatora pogodowego
	#define _DTPGDG 18          //wartosc graniczna pobudzenia od regulatora pogodowego
	#define _DTPGDO 5           //obnizenie nocne dla regulatora pogodowego
#endif
#if _KODKT==10
	//dwufunkcyjny
	#define _DTAMST 45			//moc startowa
	#define _DTAMCW	99			//moc maksymalna dla WU
	#define _DTAMCO	99			//moc maksymalna dla WG
	#define _DTAPDG	0			//ogrzewanie podlogowe
	#define _DTAWKK	_KODKT		//wybor typu kotla (dwufunkcyjny)
	#if _DTAPDG==0					//ogrzewanie podlogowe?
		#define _DTAPCO	_MAX_PCO	//nastawa pco
	#else
		#define _DTAPCO	_MAX_PDCO	//nastawa pco
	#endif
	#define _DTAPCW	F2_MAX_PCW	//nastawa pcw
	#define _DTAPFN	(_PFZMA<<4)|_PFZMA		//pozycja ZIMA
	#define _DTANOK 2			//liczba okresow na sek. (tachometer)
	#define _DTACS5	1			//czujnik cisnienia 5V
	#define _DTATAL	0			//aktywacja timera AntyLegionella
	#define _DTARTA	0			//wartosc poczatkowa timera AntyLegionella
	#define _DTAOBG	1			//domyslny obieg zamkniety (dot. UniCo
	#define _DTVMD _DTMMIN10		//granica minimalna obrotow
	#define _DTVMG _DTMMAX10		//granica maksymalna obrotow
	#define _DTRDPM	_DPWMPMP			//rodzaj pompy
	#define _DTDLPM	6			//delta temperaturowa CO dla pompy
	#define _DTMNPM	50			//granica minimalna predkosci pompy
	#define _DTTECO	0			//tryb pracy ECO
	#define _DTAECO	5			//domyslna nastawa ECO
	#define _DTTMCO2 0			//czas pracy przy obnizonej mocy w trybie CO
	#define _DTMCO2	10			//wartosc obnizonej mocy w trybie CO
    #define _DTMXPM	99			//granica maksylna predkosci pompy
    #define _DTALIN	_MY_LADDR
	#define _DTMXPM2	99		//granica maksylna predkosci pompy dla WU
	#define _DTHSCO  5			//histereza wylaczenia grzania CO (ponad nastawe)
	#define _DTCONW  75			//temp CO przy grzaniu CW zasobnika
	#define _DTPROP  0			//poprawka dla regulagtora pogodowego
	#define _DTRFAN	_DPRFAN     //rodzaj wentylatora
	#define _DTTSTR	20           //czas wybiegu na mocy startowej (tryb pracy CO)
	#define _DTTL3	3           //blokady L3 [min] (tryb pracy CO)
	#define _DTPGDR 1           //rodzaj regulatora pogodowego
	#define _DTPGDG 18          //wartosc graniczna pobudzenia od regulatora pogodowego
	#define _DTPGDO 5           //obnizenie nocne dla regulatora pogodowego
#endif
#if _KODKT==12
	//dwufunkcyjny
	#define _DTAMST 45			//moc startowa
	#define _DTAMCW	99			//moc maksymalna dla WU
	#define _DTAMCO	99			//moc maksymalna dla WG
	#define _DTAPDG	0			//ogrzewanie podlogowe
	#define _DTAWKK	_KODKT		//wybor typu kotla (dwufunkcyjny)
	#if _DTAPDG==0					//ogrzewanie podlogowe?
		#define _DTAPCO	_MAX_PCO	//nastawa pco
	#else
		#define _DTAPCO	_MAX_PDCO	//nastawa pco
	#endif
	#define _DTAPCW	F2_MAX_PCW	//nastawa pcw
	#define _DTAPFN	(_PFZMA<<4)|_PFZMA		//pozycja ZIMA
	#define _DTANOK 2			//liczba okresow na sek. (tachometer)
	#define _DTACS5	1			//czujnik cisnienia 5V
	#define _DTATAL	0			//aktywacja timera AntyLegionella
	#define _DTARTA	0			//wartosc poczatkowa timera AntyLegionella
	#define _DTAOBG	1			//domyslny obieg zamkniety (dot. UniCo
	#define _DTVMD _DTMMIN12		//granica minimalna obrotow
	#define _DTVMG _DTMMAX12		//granica maksymalna obrotow
	#define _DTRDPM	_DPWMPMP			//rodzaj pompy
	#define _DTDLPM	6			//delta temperaturowa CO dla pompy
	#define _DTMNPM	50			//granica minimalna predkosci pompy
	#define _DTTECO	0			//tryb pracy ECO
	#define _DTAECO	5			//domyslna nastawa ECO
	#define _DTTMCO2 0			//czas pracy przy obnizonej mocy w trybie CO
	#define _DTMCO2	10			//wartosc obnizonej mocy w trybie CO
    #define _DTMXPM	99			//granica maksylna predkosci pompy
    #define _DTALIN	_MY_LADDR
	#define _DTMXPM2	99		//granica maksylna predkosci pompy dla WU
	#define _DTHSCO  5			//histereza wylaczenia grzania CO (ponad nastawe)
	#define _DTCONW  75			//temp CO przy grzaniu CW zasobnika
	#define _DTPROP  0			//poprawka dla regulagtora pogodowego
	#define _DTRFAN	_DPRFAN     //rodzaj wentylatora
	#define _DTTSTR	20           //czas wybiegu na mocy startowej (tryb pracy CO)
	#define _DTTL3	3           //blokady L3 [min] (tryb pracy CO)
	#define _DTPGDR 1           //rodzaj regulatora pogodowego
	#define _DTPGDG 18          //wartosc graniczna pobudzenia od regulatora pogodowego
	#define _DTPGDO 5           //obnizenie nocne dla regulatora pogodowego
#endif
#if _KODKT==14
	//dwufunkcyjny
	#define _DTAMST 45			//moc startowa
	#define _DTAMCW	99			//moc maksymalna dla WU
	#define _DTAMCO	99			//moc maksymalna dla WG
	#define _DTAPDG	0			//ogrzewanie podlogowe
	#define _DTAWKK	_KODKT		//wybor typu kotla (dwufunkcyjny)
	#if _DTAPDG==0					//ogrzewanie podlogowe?
		#define _DTAPCO	_MAX_PCO	//nastawa pco
	#else
		#define _DTAPCO	_MAX_PDCO	//nastawa pco
	#endif
	#define _DTAPCW	F2_MAX_PCW	//nastawa pcw
	#define _DTAPFN	(_PFZMA<<4)|_PFZMA		//pozycja ZIMA
	#define _DTANOK 2			//liczba okresow na sek. (tachometer)
	#define _DTACS5	1			//czujnik cisnienia 5V
	#define _DTATAL	0			//aktywacja timera AntyLegionella
	#define _DTARTA	0			//wartosc poczatkowa timera AntyLegionella
	#define _DTAOBG	1			//domyslny obieg zamkniety (dot. UniCo
	#define _DTVMD _DTMMIN14		//granica minimalna obrotow
	#define _DTVMG _DTMMAX14		//granica maksymalna obrotow
	#define _DTRDPM	_DPWMPMP			//rodzaj pompy
	#define _DTDLPM	6			//delta temperaturowa CO dla pompy
	#define _DTMNPM	50			//granica minimalna predkosci pompy
	#define _DTTECO	0			//tryb pracy ECO
	#define _DTAECO	5			//domyslna nastawa ECO
	#define _DTTMCO2 0			//czas pracy przy obnizonej mocy w trybie CO
	#define _DTMCO2	10			//wartosc obnizonej mocy w trybie CO
    #define _DTMXPM	99			//granica maksylna predkosci pompy
    #define _DTALIN	_MY_LADDR
	#define _DTMXPM2	99		//granica maksylna predkosci pompy dla WU
	#define _DTHSCO  5			//histereza wylaczenia grzania CO (ponad nastawe)
	#define _DTCONW  75			//temp CO przy grzaniu CW zasobnika
	#define _DTPROP  0			//poprawka dla regulagtora pogodowego
	#define _DTRFAN	_DPRFAN     //rodzaj wentylatora
	#define _DTTSTR	20           //czas wybiegu na mocy startowej (tryb pracy CO)
	#define _DTTL3	3           //blokady L3 [min] (tryb pracy CO)
	#define _DTPGDR 1           //rodzaj regulatora pogodowego
	#define _DTPGDG 18          //wartosc graniczna pobudzenia od regulatora pogodowego
	#define _DTPGDO 5           //obnizenie nocne dla regulatora pogodowego
#endif
#if _KODKT==16
	//dwufunkcyjny
	#define _DTAMST 45			//moc startowa
	#define _DTAMCW	99			//moc maksymalna dla WU
	#define _DTAMCO	99			//moc maksymalna dla WG
	#define _DTAPDG	0			//ogrzewanie podlogowe
	#define _DTAWKK	_KODKT		//wybor typu kotla (dwufunkcyjny)
	#if _DTAPDG==0					//ogrzewanie podlogowe?
		#define _DTAPCO	_MAX_PCO	//nastawa pco
	#else
		#define _DTAPCO	_MAX_PDCO	//nastawa pco
	#endif
	#define _DTAPCW	F2_MAX_PCW	//nastawa pcw
	#define _DTAPFN	(_PFZMA<<4)|_PFZMA		//pozycja ZIMA
	#define _DTANOK 2			//liczba okresow na sek. (tachometer)
	#define _DTACS5	1			//czujnik cisnienia 5V
	#define _DTATAL	0			//aktywacja timera AntyLegionella
	#define _DTARTA	0			//wartosc poczatkowa timera AntyLegionella
	#define _DTAOBG	1			//domyslny obieg zamkniety (dot. UniCo
	#define _DTVMD _DTMMIN16		//granica minimalna obrotow
	#define _DTVMG _DTMMAX16		//granica maksymalna obrotow
	#define _DTRDPM	_DPWMPMP			//rodzaj pompy
	#define _DTDLPM	6			//delta temperaturowa CO dla pompy
	#define _DTMNPM	50			//granica minimalna predkosci pompy
	#define _DTTECO	0			//tryb pracy ECO
	#define _DTAECO	5			//domyslna nastawa ECO
	#define _DTTMCO2 0			//czas pracy przy obnizonej mocy w trybie CO
	#define _DTMCO2	10			//wartosc obnizonej mocy w trybie CO
    #define _DTMXPM	99			//granica maksylna predkosci pompy
    #define _DTALIN	_MY_LADDR
	#define _DTMXPM2	99		//granica maksylna predkosci pompy dla WU
	#define _DTHSCO  5			//histereza wylaczenia grzania CO (ponad nastawe)
	#define _DTCONW  75			//temp CO przy grzaniu CW zasobnika
	#define _DTPROP  0			//poprawka dla regulagtora pogodowego
	#define _DTRFAN	_DPRFAN     //rodzaj wentylatora
	#define _DTTSTR	20           //czas wybiegu na mocy startowej (tryb pracy CO)
	#define _DTTL3	3           //blokady L3 [min] (tryb pracy CO)
	#define _DTPGDR 1           //rodzaj regulatora pogodowego
	#define _DTPGDG 18          //wartosc graniczna pobudzenia od regulatora pogodowego
	#define _DTPGDO 5           //obnizenie nocne dla regulatora pogodowego
#endif
#if _KODKT==18
	//dwufunkcyjny
	#define _DTAMST 45			//moc startowa
	#define _DTAMCW	99			//moc maksymalna dla WU
	#define _DTAMCO	99			//moc maksymalna dla WG
	#define _DTAPDG	0			//ogrzewanie podlogowe
	#define _DTAWKK	_KODKT		//wybor typu kotla (dwufunkcyjny)
	#if _DTAPDG==0					//ogrzewanie podlogowe?
		#define _DTAPCO	_MAX_PCO	//nastawa pco
	#else
		#define _DTAPCO	_MAX_PDCO	//nastawa pco
	#endif
	#define _DTAPCW	F2_MAX_PCW	//nastawa pcw
	#define _DTAPFN	(_PFZMA<<4)|_PFZMA		//pozycja ZIMA
	#define _DTANOK 2			//liczba okresow na sek. (tachometer)
	#define _DTACS5	1			//czujnik cisnienia 5V
	#define _DTATAL	0			//aktywacja timera AntyLegionella
	#define _DTARTA	0			//wartosc poczatkowa timera AntyLegionella
	#define _DTAOBG	1			//domyslny obieg zamkniety (dot. UniCo
	#define _DTVMD _DTMMIN18		//granica minimalna obrotow
	#define _DTVMG _DTMMAX18		//granica maksymalna obrotow
	#define _DTRDPM	_DPWMPMP			//rodzaj pompy
	#define _DTDLPM	6			//delta temperaturowa CO dla pompy
	#define _DTMNPM	50			//granica minimalna predkosci pompy
	#define _DTTECO	0			//tryb pracy ECO
	#define _DTAECO	5			//domyslna nastawa ECO
	#define _DTTMCO2 0			//czas pracy przy obnizonej mocy w trybie CO
	#define _DTMCO2	10			//wartosc obnizonej mocy w trybie CO
    #define _DTMXPM	99			//granica maksylna predkosci pompy
    #define _DTALIN	_MY_LADDR
	#define _DTMXPM2	99		//granica maksylna predkosci pompy dla WU
	#define _DTHSCO  5			//histereza wylaczenia grzania CO (ponad nastawe)
	#define _DTCONW  75			//temp CO przy grzaniu CW zasobnika
	#define _DTPROP  0			//poprawka dla regulagtora pogodowego
	#define _DTRFAN	_DPRFAN     //rodzaj wentylatora
	#define _DTTSTR	20           //czas wybiegu na mocy startowej (tryb pracy CO)
	#define _DTTL3	3           //blokady L3 [min] (tryb pracy CO)
	#define _DTPGDR 1           //rodzaj regulatora pogodowego
	#define _DTPGDG 18          //wartosc graniczna pobudzenia od regulatora pogodowego
	#define _DTPGDO 5           //obnizenie nocne dla regulatora pogodowego
#endif
#if _KODKT==20
	//dwufunkcyjny
	#define _DTAMST 45			//moc startowa
	#define _DTAMCW	99			//moc maksymalna dla WU
	#define _DTAMCO	99			//moc maksymalna dla WG
	#define _DTAPDG	0			//ogrzewanie podlogowe
	#define _DTAWKK	_KODKT		//wybor typu kotla (dwufunkcyjny)
	#if _DTAPDG==0					//ogrzewanie podlogowe?
		#define _DTAPCO	_MAX_PCO	//nastawa pco
	#else
		#define _DTAPCO	_MAX_PDCO	//nastawa pco
	#endif
	#define _DTAPCW	F2_MAX_PCW	//nastawa pcw
	#define _DTAPFN	(_PFZMA<<4)|_PFZMA		//pozycja ZIMA
	#define _DTANOK 2			//liczba okresow na sek. (tachometer)
	#define _DTACS5	1			//czujnik cisnienia 5V
	#define _DTATAL	0			//aktywacja timera AntyLegionella
	#define _DTARTA	0			//wartosc poczatkowa timera AntyLegionella
	#define _DTAOBG	1			//domyslny obieg zamkniety (dot. UniCo
	#define _DTVMD _DTMMIN20		//granica minimalna obrotow
	#define _DTVMG _DTMMAX20		//granica maksymalna obrotow
	#define _DTRDPM	_DPWMPMP			//rodzaj pompy
	#define _DTDLPM	6			//delta temperaturowa CO dla pompy
	#define _DTMNPM	50			//granica minimalna predkosci pompy
	#define _DTTECO	0			//tryb pracy ECO
	#define _DTAECO	5			//domyslna nastawa ECO
	#define _DTTMCO2 0			//czas pracy przy obnizonej mocy w trybie CO
	#define _DTMCO2	10			//wartosc obnizonej mocy w trybie CO
    #define _DTMXPM	99			//granica maksylna predkosci pompy
    #define _DTALIN	_MY_LADDR
	#define _DTMXPM2	99		//granica maksylna predkosci pompy dla WU
	#define _DTHSCO  5			//histereza wylaczenia grzania CO (ponad nastawe)
	#define _DTCONW  75			//temp CO przy grzaniu CW zasobnika
	#define _DTPROP  0			//poprawka dla regulagtora pogodowego
	#define _DTRFAN	_DPRFAN     //rodzaj wentylatora
	#define _DTTSTR	20           //czas wybiegu na mocy startowej (tryb pracy CO)
	#define _DTTL3	3           //blokady L3 [min] (tryb pracy CO)
	#define _DTPGDR 1           //rodzaj regulatora pogodowego
	#define _DTPGDG 18          //wartosc graniczna pobudzenia od regulatora pogodowego
	#define _DTPGDO 5           //obnizenie nocne dla regulatora pogodowego
#endif
#if _KODKT==22					
	//dwufunkcyjny
	#define _DTAMST 45			//moc startowa
	#define _DTAMCW	99			//moc maksymalna dla WU
	#define _DTAMCO	99			//moc maksymalna dla WG
	#define _DTAPDG	0			//ogrzewanie podlogowe
	#define _DTAWKK	_KODKT		//wybor typu kotla (dwufunkcyjny)
	#if _DTAPDG==0					//ogrzewanie podlogowe?
		#define _DTAPCO	_MAX_PCO	//nastawa pco
	#else
		#define _DTAPCO	_MAX_PDCO	//nastawa pco
	#endif
	#define _DTAPCW	F2_MAX_PCW	//nastawa pcw
	#define _DTAPFN	(_PFZMA<<4)|_PFZMA		//pozycja ZIMA
	#define _DTANOK 2			//liczba okresow na sek. (tachometer)
	#define _DTACS5	1			//czujnik cisnienia 5V
	#define _DTATAL	0			//aktywacja timera AntyLegionella
	#define _DTARTA	0			//wartosc poczatkowa timera AntyLegionella
	#define _DTAOBG	1			//domyslny obieg zamkniety (dot. UniCo
	#define _DTVMD _DTMMIN22	//granica minimalna obrotow
	#define _DTVMG _DTMMAX22	//granica maksymalna obrotow
	#define _DTRDPM	_DPWMPMP			//rodzaj pompy
	#define _DTDLPM	6			//delta temperaturowa CO dla pompy
	#define _DTMNPM	50			//granica minimalna predkosci pompy
	#define _DTTECO	0			//tryb pracy ECO
	#define _DTAECO	5			//domyslna nastawa ECO
	#define _DTTMCO2 0			//czas pracy przy obnizonej mocy w trybie CO
	#define _DTMCO2	10			//wartosc obnizonej mocy w trybie CO
    #define _DTMXPM	99			//granica maksylna predkosci pompy
    #define _DTALIN	_MY_LADDR
	#define _DTMXPM2	99		//granica maksylna predkosci pompy dla WU
	#define _DTHSCO  5			//histereza wylaczenia grzania CO (ponad nastawe)
	#define _DTCONW  75			//temp CO przy grzaniu CW zasobnika
	#define _DTPROP  0			//poprawka dla regulagtora pogodowego
	#define _DTRFAN	_DPRFAN     //rodzaj wentylatora
	#define _DTTSTR	20           //czas wybiegu na mocy startowej (tryb pracy CO)
	#define _DTTL3	3           //blokady L3 [min] (tryb pracy CO)
	#define _DTPGDR 1           //rodzaj regulatora pogodowego
	#define _DTPGDG 18          //wartosc graniczna pobudzenia od regulatora pogodowego
	#define _DTPGDO 5           //obnizenie nocne dla regulatora pogodowego
#endif

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//----------------------------------------------------------
#endif		/*_DEFPAR_H*/
