/*eeprom.h*/
//wersja tak¿e dla procesorów serii K
#ifndef	_EEPROM_H
#define	_EEPROM_H
#include "konfig2.h"
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

#define _ADRRDPM 0x00D0		//rodzaj pompy
#define _ADRDLPM 0x00D1		//delta temperaturowa CO dla pompy
#define _ADRMNPM 0x00D2		//granica minimalna predkosci pompy
#define _ADRTECO 0x00D3		//tryb pracy ECO
#define _ADRECO  0x00D4		//wspolczynnik ECO
#define _ADRMXPM 0x00D5		//granica maksymalna predkosci pompy

//wartosci domyslne danych w pamieci (pierwsze programowanie)
#define _DTAERR	0			//kod awaryjny
//---------------------
#define _DPWMPMP 1			// ! domyslny typ pompy (1-PWM)
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
#define _DTRDPM	_DPWMPMP	//rodzaj pompy
#define _DTDLPM	20			//delta temperaturowa CO dla pompy
#define _DTMNPM	15			//granica minimalna predkosci pompy
#define _DTTECO	0			//tryb pracy ECO
#define _DTAECO	5			//domyslna nastawa ECO
#define _DTMGPM	99			//granica maksymalna predkosci pompy
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
#define _DTRDPM	_DPWMPMP	//rodzaj pompy
#define _DTDLPM	20			//delta temperaturowa CO dla pompy
#define _DTMNPM	15			//granica minimalna predkosci pompy
#define _DTTECO	0			//tryb pracy ECO
#define _DTAECO	5			//domyslna nastawa ECO
#define _DTMGPM	99			//granica maksymalna predkosci pompy
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
#define _DTRDPM	_DPWMPMP	//rodzaj pompy
#define _DTDLPM	20			//delta temperaturowa CO dla pompy
#define _DTMNPM	15			//granica minimalna predkosci pompy
#define _DTTECO	0			//tryb pracy ECO
#define _DTAECO	5			//domyslna nastawa ECO
#define _DTMGPM	99			//granica maksymalna predkosci pompy
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
#define _DTRDPM	_DPWMPMP	//rodzaj pompy
#define _DTDLPM	20			//delta temperaturowa CO dla pompy
#define _DTMNPM	15			//granica minimalna predkosci pompy
#define _DTTECO	0			//tryb pracy ECO
#define _DTAECO	5			//domyslna nastawa ECO
#define _DTMGPM	99			//granica maksymalna predkosci pompy
#endif

#define _DTATAL	0			//aktywacja timera AntyLegionella
#define _DTARTA	0			//wartosc poczatkowa timera AntyLegionella

#define _DTAOBG	1			//domyslny obieg zamkniety (dot. UniCo

//----------
//Adres 0x0000
#if _VER5V==0 || defined(_18F46K20)
//Tablica NTC dla czujnikow temperatury CO/CW (3V)
__EEPROM_DATA(99,99,98,98,97,96,95,95);
__EEPROM_DATA(94,93,92,92,91,90,90,89);
__EEPROM_DATA(88,88,87,87,86,85,85,84);
__EEPROM_DATA(84,83,82,82,81,81,80,80);
__EEPROM_DATA(79,79,78,77,77,76,76,75);
__EEPROM_DATA(75,74,74,73,73,72,72,71);
__EEPROM_DATA(71,70,70,70,69,69,68,68);
__EEPROM_DATA(67,67,66,66,65,65,64,64);
__EEPROM_DATA(64,63,63,62,62,61,61,61);
__EEPROM_DATA(60,60,59,59,58,58,57,57);
__EEPROM_DATA(57,56,56,55,55,54,54,54);
__EEPROM_DATA(53,53,52,52,52,51,51,50);
__EEPROM_DATA(50,49,49,49,48,48,47,47);
__EEPROM_DATA(46,46,46,45,45,44,44,44);
__EEPROM_DATA(43,43,42,42,41,41,41,40);
__EEPROM_DATA(40,39,39,38,38,37,37,37);
__EEPROM_DATA(36,36,35,35,34,34,33,33);
__EEPROM_DATA(32,32,31,31,30,30,29,29);
__EEPROM_DATA(28,28,27,27,26,26,25,24);
__EEPROM_DATA(24,23,23,22,22,21,21,20);
__EEPROM_DATA(19,19,18,17,17,16,16,15);
__EEPROM_DATA(14,14,13,12,11,11,10,09);
__EEPROM_DATA(08,07,06,06,05,04,03,02);
__EEPROM_DATA(01,00,00,00,00,00,00,00);
#else
//Tablica NTC dla czujnikow temperatury CO/CW (5V)
__EEPROM_DATA(99,99,98,98,97,96,95,95);
__EEPROM_DATA(94,93,93,92,91,91,90,89);
__EEPROM_DATA(89,88,87,87,86,85,85,84);
__EEPROM_DATA(84,83,83,82,81,81,80,80);
__EEPROM_DATA(79,79,78,78,77,76,76,76);
__EEPROM_DATA(75,75,74,74,73,73,72,72);
__EEPROM_DATA(71,71,70,70,69,69,68,68);
__EEPROM_DATA(67,67,66,66,65,65,64,64);
__EEPROM_DATA(64,63,63,62,62,61,61,61);
__EEPROM_DATA(60,60,59,59,58,58,57,57);
__EEPROM_DATA(57,56,56,55,55,54,54,54);
__EEPROM_DATA(53,53,52,52,52,51,51,50);
__EEPROM_DATA(50,49,49,49,48,48,47,47);
__EEPROM_DATA(46,46,46,45,45,44,44,44);
__EEPROM_DATA(43,43,42,42,41,41,41,40);
__EEPROM_DATA(40,39,39,38,38,37,37,37);
__EEPROM_DATA(36,36,35,35,34,34,33,33);
__EEPROM_DATA(32,32,31,31,31,30,30,29);
__EEPROM_DATA(29,28,27,27,26,26,25,24);
__EEPROM_DATA(24,23,23,22,22,21,21,20);
__EEPROM_DATA(19,19,18,17,17,16,16,15);
__EEPROM_DATA(14,14,13,12,11,11,10,09);
__EEPROM_DATA(08,08,07,06,05,04,03,02);
__EEPROM_DATA(01,00,00,00,00,00,00,00); 
#endif
//Adres 0x00C0
//Kod awaryjny, parametry konfiguracyjne kotla
__EEPROM_DATA(_DTAERR,_DTAMST,_DTAMCW,_DTAMCO,_DTARGZ,_DTAWTK,_DTATAL,_DTARTA);	//error,r1,...,r6
__EEPROM_DATA(_DTAOBG,_DTAPCO,_DTAPCW,_DTAPFN,_DTAPDG,_DTACS5,00,00);	
//Adres 0x00D0
__EEPROM_DATA(_DTRDPM,_DTDLPM,_DTMNPM,_DTTECO,_DTAECO,_DTMGPM,0,0);	
__EEPROM_DATA(0,0,0,0,0,0,0,0);

//Adres 0x00E0
#if _VER5V==0 || defined(_18F46K20)
//Tablica NTC dla czujnika zewnetrznego (3V)
__EEPROM_DATA(40,40,39,38,38,37,36,36);
__EEPROM_DATA(35,34,34,33,33,32,32,31);
__EEPROM_DATA(31,30,30,29,29,28,28,27);
__EEPROM_DATA(27,26,26,25,25,24,24,23);
__EEPROM_DATA(23,22,22,22,21,21,20,20);
__EEPROM_DATA(20,19,19,18,18,18,17,17);
__EEPROM_DATA(17,16,16,15,15,15,14,14);
__EEPROM_DATA(14,13,13,13,12,12,12,11);
__EEPROM_DATA(11,11,10,10,10, 9, 9, 9);
__EEPROM_DATA( 8, 8, 8, 7, 7, 7, 6, 6);
__EEPROM_DATA( 6, 5, 5, 5, 4, 4, 4, 3);
__EEPROM_DATA( 3, 3, 2, 2, 2, 1, 1, 1);
__EEPROM_DATA( 1, 0, 0, 0,-1,-1,-1,-2);
__EEPROM_DATA(-2,-2,-2,-3,-3,-3,-4,-4);
__EEPROM_DATA(-4,-5,-5,-5,-6,-6,-6,-7);
__EEPROM_DATA(-7,-7,-8,-8,-8,-9,-9,-9);
__EEPROM_DATA(-9,-10,-10,-10,-11,-11,-11,-12);
__EEPROM_DATA(-12,-13,-13,-13,-13,-14,-14,-14);
__EEPROM_DATA(-15,-15,-16,-16,-16,-17,-17,-17);
__EEPROM_DATA(-18,-18,-18,-19,-19,-20,-20,-20);
__EEPROM_DATA(-21,-21,-22,-22,-22,-23,-23,-24);
__EEPROM_DATA(-24,-25,-25,-25,-26,-26,-27,-27);
__EEPROM_DATA(-28,-28,-29,-29,-30,-30,-31,-31);
__EEPROM_DATA(-32,-33,-33,-34,-35,-35,-36,-37);
__EEPROM_DATA(-37,-38,-39,-40,-40,-40,-40,-40);
#else
//Tablica NTC dla czujnika zewnetrznego (5V)
__EEPROM_DATA(40,39,39,38,37,37,36,36);
__EEPROM_DATA(35,34,34,33,33,32,32,31);
__EEPROM_DATA(31,30,30,29,29,28,28,27);
__EEPROM_DATA(27,26,26,25,25,24,24,23);
__EEPROM_DATA(23,22,22,22,21,21,20,20);
__EEPROM_DATA(19,19,19,18,18,18,17,17);
__EEPROM_DATA(17,16,16,15,15,15,14,14);
__EEPROM_DATA(14,13,13,13,12,12,12,11);
__EEPROM_DATA(11,11,10,10,10, 9, 9, 9);
__EEPROM_DATA( 8, 8, 8, 7, 7, 7, 6, 6);
__EEPROM_DATA( 5, 5, 5, 4, 4, 4, 4, 3);
__EEPROM_DATA( 3, 3, 2, 2, 2, 1, 1, 1);
__EEPROM_DATA( 1, 0, 0, 0,-1,-1,-1,-2);
__EEPROM_DATA(-2,-2,-2,-3,-3,-3,-4,-4);
__EEPROM_DATA(-4,-5,-5,-5,-6,-6,-6,-7);
__EEPROM_DATA(-7,-7,-8,-8,-8,-9,-9,-9);
__EEPROM_DATA(-10,-10,-10,-11,-11,-11,-12,-12);
__EEPROM_DATA(-12,-13,-13,-13,-14,-14,-14,-15);
__EEPROM_DATA(-15,-15,-16,-16,-16,-17,-17,-17);
__EEPROM_DATA(-18,-18,-19,-19,-19,-20,-20,-20);
__EEPROM_DATA(-21,-21,-22,-22,-23,-23,-23,-24);
__EEPROM_DATA(-24,-25,-25,-26,-26,-27,-27,-27);
__EEPROM_DATA(-28,-28,-29,-29,-30,-30,-31,-31);
__EEPROM_DATA(-32,-33,-33,-34,-35,-35,-36,-37);
__EEPROM_DATA(-37,-38,-39,-40,-40,-40,-40,-40);
#endif
//Adres 0x0198
//----------------------------------------------------------
#endif		/*_EEPROM_H*/
