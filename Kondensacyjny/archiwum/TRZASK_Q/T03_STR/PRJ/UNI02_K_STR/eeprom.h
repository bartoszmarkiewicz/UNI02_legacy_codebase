/*eeprom.h*/
//wersja tak�e dla procesor�w serii K
#ifndef	_EEPROM_H
#define	_EEPROM_H

#include "../../proj.tp.h" 

#ifndef _TYP_PRJ
#error NIEOKRESLONY IDENTYFIKATOR PROJEKTU
#endif

#if _TYP_PRJ==1
#advisory _TYP_PRJ==1  

#include "main.h"
#include "defaultpar.h"

#define _lb(dta) ((dta)&0x00ff)
#define _hb(dta) ((dta)>>8)
//----------
//Adres 0x0000
#if _VER5V==0 || defined(_18F46K20)|| defined(_18F47Q10)
//Tablica NTC dla czujnikow temperatury CO/CW (3V)
__EEPROM_DATA(99,99,99,98,97,96,96,95);
__EEPROM_DATA(94,93,93,92,91,91,90,89);
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
__EEPROM_DATA(28,28,27,27,26,26,25,25);
__EEPROM_DATA(24,24,23,23,22,22,21,21);
__EEPROM_DATA(20,20,19,19,18,17,17,16);
__EEPROM_DATA(15,15,14,13,12,12,11,10);
__EEPROM_DATA(10,09,08,07,06,05,04,03);
__EEPROM_DATA(03,02,01,00,00,00,00,00);
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
__EEPROM_DATA(_DTAERR,_DTAMST,_DTAMCW,_DTAMCO,_DTAPDG,_DTAWKK,_DTATAL,_DTARTA);	//error,r1,...,r6
__EEPROM_DATA(_DTAOBG,_DTAPCO,_DTAPCW,_DTAPFN,_DTANOK,_DTACS5,_DTVMD,_DTVMG);	
//Adres 0x00D0
__EEPROM_DATA(_DTRDPM,_DTDLPM,_DTMNPM,_DTTECO,_DTAECO,_DTTMCO2,_DTMCO2,_DTMXPM);
__EEPROM_DATA(_DTALIN,_DTMXPM2,_DTHSCO,_DTCONW,_DTPROP,_DTRFAN,_DTTSTR,_DTTL3);

//Adres 0x00E0
//Kluczowe domyslne parametry modulatora FIME (8bit))
__EEPROM_DATA(0,18,20,51,38,84,48,117);	//parami VNT,PWM(hex)
__EEPROM_DATA(56,150,62,183,67,216,71,249);	

//Adres 0x00F0
#if _VER5V==0 || defined(_18F46K20)|| defined(_18F47Q10)
//Tablica NTC dla czujnika zewnetrznego (3V)
__EEPROM_DATA(40,40,40,39,38,37,36,36);
__EEPROM_DATA(35,35,34,34,33,33,32,32);
__EEPROM_DATA(31,31,30,30,29,29,28,28);
__EEPROM_DATA(27,27,26,26,25,25,24,24);
__EEPROM_DATA(23,23,22,22,22,21,21,21);
__EEPROM_DATA(20,20,20,19,19,19,18,18);
__EEPROM_DATA(18,17,17,17,16,16,16,15);
__EEPROM_DATA(15,15,14,14,14,13,13,13);
__EEPROM_DATA(12,12,12,11,11,11,10,10);
__EEPROM_DATA(10, 9, 9, 9, 8, 8, 8, 7);
__EEPROM_DATA( 7, 7, 6, 6, 6, 5, 5, 5);
__EEPROM_DATA( 4, 4, 4, 3, 3, 3, 2, 2);
__EEPROM_DATA( 2, 2, 1, 1, 1, 0, 0, 0);
__EEPROM_DATA( 0,-1,-1,-1,-2,-2,-2,-3);
__EEPROM_DATA(-3,-3,-3,-4,-4,-4,-4,-5);
__EEPROM_DATA(-5,-5,-6,-6,-6,-7,-7,-7);
__EEPROM_DATA(-8,-8,-8,-9,-9,-9,-10,-10);
__EEPROM_DATA(-10,-11,-11,-11,-12,-12,-12,-13);
__EEPROM_DATA(-13,-13,-14,-14,-14,-15,-15,-15);
__EEPROM_DATA(-16,-16,-16,-17,-17,-17,-18,-18);
__EEPROM_DATA(-18,-19,-19,-19,-20,-20,-20,-21);
__EEPROM_DATA(-21,-21,-22,-22,-23,-23,-24,-24);
__EEPROM_DATA(-25,-25,-26,-26,-27,-27,-28,-28);
__EEPROM_DATA(-29,-29,-30,-30,-31,-31,-32,-33);
__EEPROM_DATA(-34,-34,-35,-35,-35,-35,-35,-35);
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
//Adres 0x01B8
__EEPROM_DATA(0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF);
//Adres 0x01C0
__EEPROM_DATA(_DTPGDR,_DTPGDG,_DTPGDO,_DTPGMX,_DTCWTM,_DTCWMG,_DTPPGD,_DTVVLO);
__EEPROM_DATA(_DEFRCO1,_DEFRCO2,_DEFRCO3,_DEFRCO4,_DEFRCW1,_DEFRCW2,_DEFRCW3,_DEFRCW4);
//Adres 0x01D0
__EEPROM_DATA(00,00,00,00,00,_DEFRLO,_DEFRHI,_DEFKRES);

//Adres 0x01D8
__EEPROM_DATA(0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF);
__EEPROM_DATA(0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF);
//Adres 0x01E8 - Historia zdarzen awaryjnych
__EEPROM_DATA(00,00,00,00,00,00,00,00);
__EEPROM_DATA(00,00,00,00,00,00,00,00);
__EEPROM_DATA(00,00,00,00,00,00,00,00);
//Adres 0x0200
__EEPROM_DATA(00,00,00,00,00,00,00,00);
//Adres 0x0208 - Status wylaczenia awaryjnego (0)
__EEPROM_DATA(00,00,00,00,00,00,00,00);
__EEPROM_DATA(00,00,00,00,00,00,00,00);
__EEPROM_DATA(00,00,00,00,00,00,00,00);
__EEPROM_DATA(00,00,00,00,00,00,00,00);
//Adres 0x0228 - Status wylaczenia awaryjnego (1)
__EEPROM_DATA(00,00,00,00,00,00,00,00);
__EEPROM_DATA(00,00,00,00,00,00,00,00);
__EEPROM_DATA(00,00,00,00,00,00,00,00);
__EEPROM_DATA(00,00,00,00,00,00,00,00);
//Adres 0x0248 - Status wylaczenia awaryjnego (2)
__EEPROM_DATA(00,00,00,00,00,00,00,00);
__EEPROM_DATA(00,00,00,00,00,00,00,00);
__EEPROM_DATA(00,00,00,00,00,00,00,00);
__EEPROM_DATA(00,00,00,00,00,00,00,00);
//Adres 0x0268 - Status wylaczenia awaryjnego (4)
__EEPROM_DATA(00,00,00,00,00,00,00,00);
__EEPROM_DATA(00,00,00,00,00,00,00,00);
__EEPROM_DATA(00,00,00,00,00,00,00,00);
__EEPROM_DATA(00,00,00,00,00,00,00,00);
//Adres 0x0288
__EEPROM_DATA(00,00,00,00,00,00,00,00);
//Adres 0x0290
//Kluczowe domyslne parametry modulatora FIME (10bit)
__EEPROM_DATA(_lb(_DTVV0) ,_hb(_DTVV0) ,_lb(_DTVP0),_hb(_DTVP0),_lb(_DTVV1),_hb(_DTVV1),_lb(_DTVP1) ,_hb(_DTVP1) );//parami lo(VNT),hi(VNT),lo(PWM),hi(PWM))
__EEPROM_DATA(_lb(_DTVV2),_hb(_DTVV2),_lb(_DTVP2),_hb(_DTVP2),_lb(_DTVV3),_hb(_DTVV3),_lb(_DTVP3) ,_hb(_DTVP3) );
__EEPROM_DATA(_lb(_DTVV4),_hb(_DTVV4),_lb(_DTVP4),_hb(_DTVP4),_lb(_DTVV5),_hb(_DTVV5),_lb(_DTVP5) ,_hb(_DTVP5) );
__EEPROM_DATA(_lb(_DTVV6),_hb(_DTVV6),_lb(_DTVP6),_hb(_DTVP6),_lb(_DTVV7),_hb(_DTVV7),_lb(_DTVP7),_hb(_DTVP7));
__EEPROM_DATA(_lb(_DTVV8),_hb(_DTVV8),_lb(_DTVP8),_hb(_DTVP8),_lb(_DTVV9),_hb(_DTVV9),_lb(_DTVP9),_hb(_DTVP9));
__EEPROM_DATA(00,00,00,00,00,00,00,00);
//Adres 0x02C0
//Parametry PID
__EEPROM_DATA(_DEFPLO,_DEFPHI, _DEFILO,_DEFIHI,_DEFDLO,_DEFDHI,00,00);
//Adres 0x02C8
//----------------------------------------------------------
#endif
#endif		/*_EEPROM_H*/
