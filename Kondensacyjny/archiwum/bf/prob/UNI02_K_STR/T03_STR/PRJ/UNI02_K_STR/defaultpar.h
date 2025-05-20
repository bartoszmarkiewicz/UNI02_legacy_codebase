/*defaultpar.h*/
#ifndef	_DEFPAR_H
#define	_DEFPAR_H
#include "../../proj.tp.h"

#ifndef _TYP_PRJ
#error NIEOKRESLONY IDENTYFIKATOR PROJEKTU
#endif

#if _TYP_PRJ==1
#advisory _TYP_PRJ==1

#include "global.h"
#include "main.h"
#include "konfig.h"
#include "pomiar.h"
#include "pwm.h"

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

//adresy pozycji tablicy kalibracji wentylatora (8bit))
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


//adresy pozycji tablicy kalibracji wentylatora (10bit))
#define _ADRVN0L    0x0290		//pozycja 0
#define _ADRVN0H    0x0291		//pozycja 0
#define _ADRPW0L	0x0292		//pozycja 0
#define _ADRPW0H	0x0293		//pozycja 0
#define _ADRVN1L    0x0294		//pozycja 1
#define _ADRVN1H    0x0295		//pozycja 1
#define _ADRPW1L	0x0296		//pozycja 1
#define _ADRPW1H	0x0297		//pozycja 1
#define _ADRVN2L    0x0298		//pozycja 2
#define _ADRVN2H    0x0299		//pozycja 2
#define _ADRPW2L	0x029A		//pozycja 2
#define _ADRPW2H	0x029B		//pozycja 2
#define _ADRVN3L    0x029C		//pozycja 3
#define _ADRVN3H    0x029D		//pozycja 3
#define _ADRPW3L	0x029E		//pozycja 3
#define _ADRPW3H	0x029F		//pozycja 3
#define _ADRVN4L    0x02A0		//pozycja 4
#define _ADRVN4H    0x02A1		//pozycja 4
#define _ADRPW4L	0x02A2		//pozycja 4
#define _ADRPW4H	0x02A3		//pozycja 4
#define _ADRVN5L    0x02A4		//pozycja 5
#define _ADRVN5H    0x02A5		//pozycja 5
#define _ADRPW5L	0x02A6		//pozycja 5
#define _ADRPW5H	0x02A7		//pozycja 5
#define _ADRVN6L    0x02A8		//pozycja 6
#define _ADRVN6H    0x02A9		//pozycja 6
#define _ADRPW6L	0x02AA		//pozycja 6
#define _ADRPW6H	0x02AB		//pozycja 6
#define _ADRVN7L    0x02AC		//pozycja 7
#define _ADRVN7H    0x02AD		//pozycja 7
#define _ADRPW7L	0x02AE		//pozycja 7
#define _ADRPW7H	0x02AF		//pozycja 7
#define _ADRVN8L    0x02B0		//pozycja 8
#define _ADRVN8H    0x02B1		//pozycja 8
#define _ADRPW8L	0x02B2		//pozycja 8
#define _ADRPW8H	0x02B3		//pozycja 8
#define _ADRVN9L    0x02B4		//pozycja 9
#define _ADRVN9H    0x02B5		//pozycja 9
#define _ADRPW9L	0x02B6		//pozycja 9
#define _ADRPW9H	0x02B7		//pozycja 9

#define _ADRPLO     0x02C0		//P (lo) F88
#define _ADRPHI     0x02C1		//P (hi)
#define _ADRILO     0x02C2		//I (lo) F88
#define _ADRIHI     0x02C3		//I (hi)
#define _ADRDLO     0x02C4		//D (lo) F88
#define _ADRDHI     0x02C5		//D (hi)

#define _ADRPGDR  0x01C0		//rodzaj regulatora pogodowego
#define _ADRPGDG  0x01C1		//granica pobudzenia dla regulatora pogodowego
#define _ADRPGDO  0x01C2		//obnizenie nocne dla regulatora pogodowego
#define _ADRPGMX  0x01C3		//maksymalna wartosc graniczna CO dla regulatora pogodowego
#define _ADRCWTM  0x01C4		//we. sygnalu timera zasobnika (1F) [P30]
#define _ADRCWMG  0x01C5		//histereza wylaczenia CW(2F) [P30]
#define _ADRPPGD  0x01C6        //nastawa regulatora pogodowego
#define _ADRVVLO  0x01C7        //prog minimalny wentylatora przy wyborze typu 2

#define _ADRRCO1  0x01C8        //korekta punktu rownowagi w fazie dochodzenia (FAZA1) 
#define _ADRRCO2  0x01C9        //regulacja w punkcie rownowagi w fazie stabilizacji (FAZA2)
#define _ADRRCO3  0x01CA        //korekta punktu rownowagi w fazie stabilizacji (FAZA2)
#define _ADRRCO4  0x01CB        //reakcja superwizyjna

#define _ADRRCW1  0x01CC        //korekta punktu rownowagi w fazie dochodzenia (FAZA1)
#define _ADRRCW2  0x01CD        //regulacja w punkcie rownowagi w fazie stabilizacji (FAZA2)
#define _ADRRCW3  0x01CE        //korekta punktu rownowagi w fazie stabilizacji (FAZA2)
#define _ADRRCW4  0x01CF        //reakcja superwizyjna

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
#define _DTVVLO     _VLO_PWM0   //prog minimalny wentylatora przy wyborze typu 2 (domyslnie FIME)
//----------------------------------------------------------------
//FIME
#define _DTVV0      812    //predkosc wentylatora
#define _DTVP0      72     //wypelnienie PWM
#define _DTVV1      1527    //predkosc wentylatora
#define _DTVP1      117     //wypelnienie PWM
#define _DTVV2      2317    //predkosc wentylatora
#define _DTVP2      162     //wypelnienie PWM
#define _DTVV3      3077    //predkosc wentylatora
#define _DTVP3      207     //wypelnienie PWM
#define _DTVV4      4781    //predkosc wentylatora
#define _DTVP4      343     //wypelnienie PWM
#define _DTVV5      5941    //predkosc wentylatora
#define _DTVP5      479     //wypelnienie PWM
#define _DTVV6      6780    //predkosc wentylatora
#define _DTVP6      615     //wypelnienie PWM
#define _DTVV7      7407    //predkosc wentylatora
#define _DTVP7      751     //wypelnienie PWM
#define _DTVV8      8000    //predkosc wentylatora
#define _DTVP8      887     //wypelnienie PWM
#define _DTVV9      8571    //predkosc wentylatora
#define _DTVP9      1023     //wypelnienie PWM
//----------------------------------------------------------------
//----------------------------------------------------------------
//wartosci domyslne danych w pamieci (pierwsze programowanie)
#define _DTAERR	0			//kod awaryjny
//----------
#define _DPWMPMP 1			// ! domyslny typ pompy (1-PWM) **************
#define _DPRFAN 0			// ! domyslny typ wentylatora (0-FIME,1-NG40,2-fabryczny)

#define _DTPGDR 1               //rodzaj regulatora pogodowego
#define _DTPGDG 18              //wartosc graniczna pobudzenia od regulatora pogodowego
#define _DTPGDO 5               //obnizenie nocne dla regulatora pogodowego
#define _DTPGMX _MAX_PCO        //maksymalna wartosc graniczna CO dla regulatora pogodowego
#define _DTCWTM  0              //we. sygnalu timera zasobnika (1F)
#define _DTCWMG  0              //histereza wylaczenia CW(2F)
#define _DTPPGD  99             //domyslna wartosc nastawy KT dla aktywnego regulatora pogodowego
//---------
#define _DEFKRES  0             //flaga wymuszonej kalibracji wentylatora po programowym restarcie oprogramowania

//UWAGA: F88
//1. liczby dodatnie:
//int=float*256
//--------------------!!!!!!
//2. liczby ujemne (z automatyczna konwersja na signed int 16bit):
//int=float(bez znaku)*256
//int=neg(int)
//------------------------
//3. liczby ujemne (bez automatycznej konwersji na signed int):
//int=float(bez znaku)*256
//int=neg(int)
//int=int+1

//PID (UWAGA wartosci z zakresu: )
//P=127.0+x;  x=P-127.0; dla P=60 x=-67.0 czyli F88(x)=0xBD00
//I=127.0+x;  x=I-127.0; dla I=0 x=-127.0 czyli F88(x)=0xFF81
//D=127.0+x;  x=I-127.0; dla D=0 x=-127.0 czyli F88(x)=0xFF81
#define _DEFPLO     0x00		//P (lo) F88 
#define _DEFPHI     0xBD		//P (hi)
#define _DEFILO     0x00		//I (lo) F88
#define _DEFIHI     0x81		//I (hi)
#define _DEFDLO     0x00		//D (lo) F88
#define _DEFDHI     0x81		//D (hi)

//Parametry regulacji w obiegu CO (dla DtKNF.rfan==2)
#define _DEFRCO1    10        //[/10]korekta punktu rownowagi w fazie dochodzenia (FAZA1)
#define _DEFRCO2    10        //[/10]regulacja w punkcie rownowagi w fazie stabilizacji (FAZA2)
#define _DEFRCO3    10        //[/10]korekta punktu rownowagi w fazie stabilizacji (FAZA2)
#define _DEFRCO4    10        //[/10]reakcja superwizyjna

//Parametry regulacji w obiegu CW (dla DtKNF.rfan==2)
#define _DEFRCW1    10        //[/10]korekta punktu rownowagi w fazie dochodzenia (FAZA1)
#define _DEFRCW2    10        //[/10]regulacja w punkcie rownowagi w fazie stabilizacji (FAZA2)
#define _DEFRCW3    10        //[/10]korekta punktu rownowagi w fazie stabilizacji (FAZA2)
#define _DEFRCW4    10        //[/10]reakcja superwizyjna
//----------------------------------------------------------------
//----------------------------------------------------------------
//Domyslne parametry wentylatora dla wybranego typu kotla
//(wartosci graniczne w pliku konfig.h)
	//r6=0	
	//r6=1	(1f)	LABUSE
	#define _DTMMIN1 15			//15granica minimalna obrotow
	#define _DTMMAX1 56			//56granica maksymalna obrotow

	//r6=2	(2f)	LABUSE
	#define _DTMMIN2 15			//12granica minimalna obrotow
	#define _DTMMAX2 56			//56granica maksymalna obrotow

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
	#define _DTMXPM	99			//granica maksylna predkosci pompy
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
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//Kotly dwufunkcyjne
#elif _KODKT==2
	//dwufunkcyjny
	#define _DTAMST 45			//moc startowa
	#define _DTAMCW	99			//moc maksymalna dla WU
	#define _DTAMCO	70			//99moc maksymalna dla WG
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
    #define _DTMXPM	99			//granica maksylna predkosci pompy
    #define _DTALIN	_MY_LADDR
	#define _DTMXPM2	99		//granica maksylna predkosci pompy dla WU
	#define _DTHSCO  5			//histereza wylaczenia grzania CO (ponad nastawe)
	#define _DTCONW  75			//temp CO przy grzaniu CW zasobnika
	#define _DTPROP  0			//poprawka dla regulagtora pogodowego
	#define _DTRFAN	_DPRFAN     //rodzaj wentylatora
	#define _DTTSTR	20          //20 czas wybiegu na mocy startowej (tryb pracy CO) ************************************************
	#define _DTTL3	3           //blokady L3 [min] (tryb pracy CO)
	#define _DTPGDR 1           //rodzaj regulatora pogodowego
	#define _DTPGDG 18          //wartosc graniczna pobudzenia od regulatora pogodowego
	#define _DTPGDO 5           //obnizenie nocne dla regulatora pogodowego
#else
#error NIEPRAWIDLOWY _KODKT
#endif
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//----------------------------------------------------------
#endif
#endif		/*_DEFPAR_H*/
