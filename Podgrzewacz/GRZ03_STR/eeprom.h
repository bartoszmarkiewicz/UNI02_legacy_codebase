/*eeprom.h*/
#ifndef	_EEPROM_H
#define	_EEPROM_H
#include "konfig.h"

//adresy danych przechowywanych w pamieci nieulotnej eeprom
#define _ADRERR 0x00C0		//kod awaryjny blokady trwalej
#define _ADRMST	0x00C1		//moc startowa
#define _ADRMCW	0x00C2		//moc maksymalna dla WU
#define _ADRMDG	0x00C3		//dolna granica predkosci wentylatora
#define _ADRMGG	0x00C4		//gorna granica predkosci wentylatora
#define _ADRNOK	0x00C5		//liczba okresow na sek. (tachometer)
#define _ADRRFAN 0x00C6		//rodzaj wentylatora
#define _ADRWTK	0x00C7		//wybor typu kotla
#define _ADRPCW	0x00C8		//nastawa WU

//adresy pozycji tablicy kalibracji wentylatora FIME,(+0x10)NG40,(+0x20)NG31
#define _ADRVN0 0x00D0		//pozycja 0
#define _ADRPW0	0x00D1		//pozycja 0
#define _ADRVN1	0x00D2		//pozycja 1
#define _ADRPW1	0x00D3		//pozycja 1
#define _ADRVN2 0x00D4		//pozycja 2
#define _ADRPW2	0x00D5		//pozycja 2
#define _ADRVN3	0x00D6		//pozycja 3
#define _ADRPW3	0x00D7		//pozycja 3
#define _ADRVN4	0x00D8		//pozycja 4
#define _ADRPW4	0x00D9		//pozycja 4
#define _ADRVN5	0x00DA		//pozycja 5
#define _ADRPW5	0x00DB		//pozycja 5
#define _ADRVN6	0x00DC		//pozycja 6
#define _ADRPW6	0x00DD		//pozycja 6
#define _ADRVN7	0x00DE		//pozycja 7
#define _ADRPW7	0x00DF		//pozycja 7



//wartosci domyslne danych w pamieci (pierwsze programowanie)
#define _DTAERR	0			//kod awaryjny
#define _DTAMST	80			//moc startowa
#define _DTAMCW	99			//moc maksymalna dla WU
#define _DTMMIN 9			//granica minimalna obrotow
#define _DTMMAX 56			//granica maksymalna obrotow
#define _DTANOK 2			//liczba okresow na sek. (tachometer)
#define _DPRFAN 2			// ! domyslny typ wentylatora (0-FIME,1-NG40,2-NG31)

#define _DTAPCW	60			//nastawa WU


//Unico
#if _TYPKT==_UNCO
#define _DTARGZ _ZIEMNY		//wybor rodzaju gazu
#define _DTAWTK	_UNICO	//wybor typu kotla
#endif
//MiniMax
#if _TYPKT==_MMAXP
#define _DTARGZ _ZIEMNY		//wybor rodzaju gazu
#define _DTAWTK	_MONOTERM	//wybor typu kotla
#endif
//Miniterm
#if _TYPKT==_INWEST
#define _DTARGZ _ZIEMNY		//wybor rodzaju gazu
#define _DTAWTK	_BITERM		//wybor typu kotla
#endif
//MiniMax zasobnik
#if _TYPKT==_ZASOB
#define _DTARGZ _ZIEMNY		//wybor rodzaju gazu
#define _DTAWTK	_ZASOBNIK	//wybor typu kotla
#endif
//Maxiterm
#if _TYPKT==_INWEST2
#define _DTARGZ _ZIEMNY		//wybor rodzaju gazu
#define _DTAWTK	_BITERM2	//wybor typu kotla
#endif
#if _TYPKT==_GRZ01
#define _DTARGZ _ZIEMNY		//wybor rodzaju gazu
#define _DTAWTK	_BITERM		//wybor typu kotla
#endif
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
__EEPROM_DATA(_DTAERR,_DTAMST,_DTAMCW,_DTMMIN,_DTMMAX,_DTANOK,_DPRFAN,_DTAWTK);	//error,r1,...,r6
__EEPROM_DATA(_DTAPCW,00,00,00,00,00,00,00);	
//Adres 0x00D0
__EEPROM_DATA(0,18,23,51,45,84,62,117);	//parami VNT,PWM(hex) FIME
__EEPROM_DATA(75,150,89,183,100,216,109,249);
//Adres 0x00E0
__EEPROM_DATA(09,33,34,62,52,93,67,124);	//parami VNT,PWM(hex) NG40
__EEPROM_DATA(80,155,92,186,104,217,114,248);
//Adres 0x00F0
#if _PLL==0
//1kHz
__EEPROM_DATA(10,21,31,53,44,85,58,117);	//parami VNT,PWM(hex) NG31
__EEPROM_DATA(68,149,80,184,89,215,96,246);
#else
//4kHz
__EEPROM_DATA(10,26,30,58,44,90,59,122);	//parami VNT,PWM(hex) NG31
__EEPROM_DATA(71,154,83,186,92,218,100,250);
#endif
//----------------------------------------------------------
#endif		/*_EEPROM_H*/
