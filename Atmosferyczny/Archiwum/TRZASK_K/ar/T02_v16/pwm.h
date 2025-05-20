/*pwm.h*/
//modulator SITA
#ifndef	_PWM_H
#define	_PWM_H
#include "pwm.tp.h"
#include "konfig.h"
/*---------------------------------------------------------------------------------*/
/*-----------------------------PUBLIC DATA-----------------------------------------*/
//okreslenie typu odbiornika 
#define _FPWM			4			//czestotliwosc przebiegu [kHz]
#define _NEG_PWM		0			//odwrotma logika wyjscia? (max dla pwm=0, min dla pwm=255)
#define _NEG_PWM2		0			//PWM2 odwrotma logika wyjscia? (max dla pwm=0, min dla pwm=255)
#define _MG2_MOD		0xF0		//wartosc PWM przy ktorej osiagniety zostaje pelny wydatek pradu ukladu wyjsciowego
//-----------------------------
//***********************************************************************************
//***********************************************************************************
//MINITERM
//---------
//parametry zwiazane z modulem oscylacji
//!UWAGA: zalaczyc modul oscylacji mozna w pliku pwm.tp.h
#define _EN_OSC1 		1			//aktywacja(1)/deaktywacja(0) oscylacji na sygnale PWM
#define _MAX_OSC_PWM1	30			//20maksymalna amplituda oscylacji
#define _MIN_OSC_PWM1	5			//minimalna amplituda oscylacji
//---------
//parametry opisujace wlasciwosci modulatora 
#define _MAD_MOD1	0x14			//dolna dozwolona granica mechaniczna warunku ciszy akustycznej przy oscylacjach
#define _MAG_MOD1	0xF7			//gorna dozwolona granica mechaniczna warunku ciszy akustycznej przy oscylacjach
#define _MD_MOD1	0x20			//35martwy zakres modulacji - dolna granica 
#define _MD_PMOD1	0x40			//martwy zakres modulacji - dolna granica (dla gazu plynnego)
#define _MG_MOD1	0x9F			//martwy zakres modulacji - gorna granica
#define _MG_PMOD1	0xE0			//martwy zakres modulacji - gorna granica (dla gazu plynnego)
#define _I80_MOD1	0x75			//domyslna wartosc pierwszego pobudzenia
#define _I80_PMOD1 	0xA3			//domyslna wartosc pierwszego pobudzenia - gaz plynny

/*
Pierwsze pobudzenie (_I80_MOD lub _I80_PMOD) na poziomie 60% zakresu modulacji
gaz ziemny
(X75-X35)=X40
/			=64/106*100%=60%
(X9f-X35)=X6A
gaz p造nny
(xE0-x48)*60%+x48=x5B+x48 =xA3

(xA3-x48)=x5B
/			=91/152*100%=60%
(xE0-x48)=x98
*/

#if _MAX_OSC_PWM1<=_MIN_OSC_PWM1
#error Nieprawidlowe wartosci parametrow _MAX_OSC_PWM,_MIN_OSC_PWM
#endif
#if _MG2_MOD<_MG_MOD1||_MG2_MOD<_MG_PMOD1
#error Nieprawidlowe wartosci parametrow _MG2_MOD,_MG_MOD,_MG_PMOD
#endif
#if _MD_MOD1>_MG_MOD1||_MD_PMOD1>_MG_PMOD1
#error Nieprawidlowe wartosci parametrow _MD_MOD,_MD_PMOD,_MG_MOD,_MG_PMOD
#endif
#if _MAD_MOD1>_MAG_MOD1
#error Nieprawidlowe wartosci parametrow _MAD_MOD,_MAG_MOD
#endif
#if _MAD_MOD1>_MD_MOD1||_MAD_MOD1>_MD_PMOD1
#error Nieprawidlowe wartosci parametrow _MAD_MOD,_MD_MOD,_MD_PMOD
#endif
#if _MAG_MOD1<_MG_MOD1||_MAG_MOD1<_MG_PMOD1
#error Nieprawidlowe wartosci parametrow _MAG_MOD,_MG_MOD,_MG_PMOD
#endif
//-----------------------------
//parametry zwiazane z moca startowa
#define _GWMSTART1	99				//graniczna wartosc nastawy mocy startowej
//gaz ziemny
#define _MDSTART1 _MD_MOD1			//dolna granica wartosci PWM dla mocy startowej
#define _MGSTART1 _MG_MOD1+10			//gorna granica wartosci PWM dla mocy startowej
//gaz plynny
#define _MDSTARTP1 _MD_PMOD1			//dolna granica wartosci PWM dla mocy startowej
#define _MGSTARTP1 _MG_PMOD1+10			//gorna granica wartosci PWM dla mocy startowej
//---------
//parametry zwiazane z moca maksymalna CO
#define _GWMMAXCO1	99				//graniczna wartosc nastawy mocy maksymalnej CO
//gaz ziemny
#define _MDMMAXCO1 _MD_MOD1			//dolna granica wartosci PWM dla mocy maksymalnej CO
#define _MGMMAXCO1 _MG_MOD1+10			//gorna granica wartosci PWM dla mocy maksymalnej CO
//gaz plynny
#define _MDMMAXCOP1 _MD_PMOD1			//dolna granica wartosci PWM dla mocy maksymalnej CO
#define _MGMMAXCOP1 _MG_PMOD1+10			//gorna granica wartosci PWM dla mocy maksymalnej CO
//---------
//parametry zwiazane z moca maksymalna CW
#define _GWMMAXCW1	99				//graniczna wartosc nastawy mocy maksymalnej CW
//gaz ziemny
#define _MDMMAXCW1 _MD_MOD1			//dolna granica wartosci PWM dla mocy maksymalnej CW
#define _MGMMAXCW1 _MG_MOD1+10			//gorna granica wartosci PWM dla mocy maksymalnej CW
//gaz plynny
#define _MDMMAXCWP1 _MD_PMOD1			//dolna granica wartosci PWM dla mocy maksymalnej CW
#define _MGMMAXCWP1 _MG_PMOD1+10			//gorna granica wartosci PWM dla mocy maksymalnej CW
//---------

//***********************************************************************************
//***********************************************************************************
//MINIMAX, MINIMAX ZASOBNIKOWY
//---------
//parametry zwiazane z modulem oscylacji
//!UWAGA: zalaczyc modul oscylacji mozna w pliku pwm.tp.h
#define _EN_OSC2 		1			//aktywacja(1)/deaktywacja(0) oscylacji na sygnale PWM
#define _MAX_OSC_PWM2	15			//maksymalna amplituda oscylacji
#define _MIN_OSC_PWM2	5			//minimalna amplituda oscylacji
//---------
//parametry opisujace wlasciwosci modulatora
#define _MAD_MOD2	0x14			//dolna dozwolona granica mechaniczna warunku ciszy akustycznej przy oscylacjach
#define _MAG_MOD2	0xF7			//gorna dozwolona granica mechaniczna warunku ciszy akustycznej przy oscylacjach
#define _MD_MOD2	0x2D			//martwy zakres modulacji - dolna granica 
#define _MD_PMOD2	0x4E			//martwy zakres modulacji - dolna granica (dla gazu plynnego)
#define _MG_MOD2	0x9F			//martwy zakres modulacji - gorna granica
#define _MG_PMOD2	0xE0			//martwy zakres modulacji - gorna granica (dla gazu plynnego)
#define _I80_MOD2	0x6E			//60% domyslna wartosc pierwszego pobudzenia
#define _I80_PMOD2 	0xA5			//60% domyslna wartosc pierwszego pobudzenia - gaz plynny

/*
Pierwsze pobudzenie (_I80_MOD lub _I80_PMOD) na poziomie 60% zakresu modulacji
gaz ziemny
(X6E-X2D)=X41
/			=65/114*100%=57%
(X9f-X2D)=X72
gaz p造nny
(xE0-x4E)*60%+x4E=x57+x4E =xA5

(xA5-x4E)=x57
/			=87/146*100%=60%
(xE0-x4E)=x92
*/

#if _MAX_OSC_PWM2<=_MIN_OSC_PWM2
#error Nieprawidlowe wartosci parametrow _MAX_OSC_PWM,_MIN_OSC_PWM
#endif
#if _MG2_MOD<_MG_MOD2||_MG2_MOD<_MG_PMOD2
#error Nieprawidlowe wartosci parametrow _MG2_MOD,_MG_MOD,_MG_PMOD
#endif
#if _MD_MOD2>_MG_MOD2||_MD_PMOD2>_MG_PMOD2
#error Nieprawidlowe wartosci parametrow _MD_MOD,_MD_PMOD,_MG_MOD,_MG_PMOD
#endif
#if _MAD_MOD2>_MAG_MOD2
#error Nieprawidlowe wartosci parametrow _MAD_MOD,_MAG_MOD
#endif
#if _MAD_MOD2>_MD_MOD2||_MAD_MOD2>_MD_PMOD2
#error Nieprawidlowe wartosci parametrow _MAD_MOD,_MD_MOD,_MD_PMOD
#endif
#if _MAG_MOD2<_MG_MOD2||_MAG_MOD2<_MG_PMOD2
#error Nieprawidlowe wartosci parametrow _MAG_MOD,_MG_MOD,_MG_PMOD
#endif
//-----------------------------
//-----------------------------
//parametry zwiazane z moca startowa
#define _GWMSTART2	99				//graniczna wartosc nastawy mocy startowej
//gaz ziemny
#define _MDSTART2 _MD_MOD2			//dolna granica wartosci PWM dla mocy startowej
#define _MGSTART2 _MG_MOD2+10			//gorna granica wartosci PWM dla mocy startowej
//gaz plynny
#define _MDSTARTP2 _MD_PMOD2			//dolna granica wartosci PWM dla mocy startowej
#define _MGSTARTP2 _MG_PMOD2+10			//gorna granica wartosci PWM dla mocy startowej
//---------
//parametry zwiazane z moca maksymalna CO
#define _GWMMAXCO2	99				//graniczna wartosc nastawy mocy maksymalnej CO
//gaz ziemny
#define _MDMMAXCO2 _MD_MOD2			//dolna granica wartosci PWM dla mocy maksymalnej CO
#define _MGMMAXCO2 _MG_MOD2+10			//gorna granica wartosci PWM dla mocy maksymalnej CO
//gaz plynny
#define _MDMMAXCOP2 _MD_PMOD2			//dolna granica wartosci PWM dla mocy maksymalnej CO
#define _MGMMAXCOP2 _MG_PMOD2+10			//gorna granica wartosci PWM dla mocy maksymalnej CO
//---------
//parametry zwiazane z moca maksymalna CW
#define _GWMMAXCW2	99				//graniczna wartosc nastawy mocy maksymalnej CW
//gaz ziemny
#define _MDMMAXCW2 _MD_MOD2			//dolna granica wartosci PWM dla mocy maksymalnej CW
#define _MGMMAXCW2 _MG_MOD2+10			//gorna granica wartosci PWM dla mocy maksymalnej CW
//gaz plynny
#define _MDMMAXCWP2 _MD_PMOD2			//dolna granica wartosci PWM dla mocy maksymalnej CW
#define _MGMMAXCWP2 _MG_PMOD2+10			//gorna granica wartosci PWM dla mocy maksymalnej CW
//---------
//***********************************************************************************
//***********************************************************************************
//UNICO
//---------
//parametry zwiazane z modulem oscylacji
//!UWAGA: zalaczyc modul oscylacji mozna w pliku pwm.tp.h
#define _EN_OSC0 		1			//aktywacja(1)/deaktywacja(0) oscylacji na sygnale PWM
#define _MAX_OSC_PWM0	20			//maksymalna amplituda oscylacji
#define _MIN_OSC_PWM0	5			//minimalna amplituda oscylacji
//---------
//parametry opisujace wlasciwosci modulatora
#define _MAD_MOD0	0x14			//dolna dozwolona granica mechaniczna warunku ciszy akustycznej przy oscylacjach
#define _MAG_MOD0	0xF7			//gorna dozwolona granica mechaniczna warunku ciszy akustycznej przy oscylacjach
#define _MD_MOD0	0x30			//martwy zakres modulacji - dolna granica 
#define _MD_PMOD0	0x48			//martwy zakres modulacji - dolna granica (dla gazu plynnego)
#define _MG_MOD0	0x9F			//martwy zakres modulacji - gorna granica
#define _MG_PMOD0	0xEB			//martwy zakres modulacji - gorna granica (dla gazu plynnego)
#define _I80_MOD0	0x75			//60% domyslna wartosc pierwszego pobudzenia
#define _I80_PMOD0 	0xAA			//60% domyslna wartosc pierwszego pobudzenia - gaz plynny

/*
Pierwsze pobudzenie (_I80_MOD lub _I80_PMOD) na poziomie 60% zakresu modulacji
gaz ziemny
(X75-X30)=X45
/			=69/111*100%=60%
(X9f-X30)=X6F
gaz p造nny
(xEB-x48)*60%+x48=x62+x48 =xAA
spr:
(xAA-x48)=x62
/			=98/163*100%=60%
(xEB-x48)=xA3
*/

#if _MAX_OSC_PWM0<=_MIN_OSC_PWM0
#error Nieprawidlowe wartosci parametrow _MAX_OSC_PWM,_MIN_OSC_PWM
#endif
#if _MG2_MOD<_MG_MOD0||_MG2_MOD<_MG_PMOD0
#error Nieprawidlowe wartosci parametrow _MG2_MOD,_MG_MOD,_MG_PMOD
#endif
#if _MD_MOD0>_MG_MOD0||_MD_PMOD0>_MG_PMOD0
#error Nieprawidlowe wartosci parametrow _MD_MOD,_MD_PMOD,_MG_MOD,_MG_PMOD
#endif
#if _MAD_MOD0>_MAG_MOD0
#error Nieprawidlowe wartosci parametrow _MAD_MOD,_MAG_MOD
#endif
#if _MAD_MOD0>_MD_MOD0||_MAD_MOD0>_MD_PMOD0
#error Nieprawidlowe wartosci parametrow _MAD_MOD,_MD_MOD,_MD_PMOD
#endif
#if _MAG_MOD0<_MG_MOD0||_MAG_MOD0<_MG_PMOD0
#error Nieprawidlowe wartosci parametrow _MAG_MOD,_MG_MOD,_MG_PMOD
#endif
//-----------------------------
//-----------------------------
//parametry zwiazane z moca startowa
#define _GWMSTART0	99				//graniczna wartosc nastawy mocy startowej
//gaz ziemny
#define _MDSTART0 _MD_MOD0			//dolna granica wartosci PWM dla mocy startowej
#define _MGSTART0 _MG_MOD0+10			//gorna granica wartosci PWM dla mocy startowej
//gaz plynny
#define _MDSTARTP0 _MD_PMOD0			//dolna granica wartosci PWM dla mocy startowej
#define _MGSTARTP0 _MG_PMOD0+10			//gorna granica wartosci PWM dla mocy startowej
//---------
//parametry zwiazane z moca maksymalna CO
#define _GWMMAXCO0	99				//graniczna wartosc nastawy mocy maksymalnej CO
//gaz ziemny
#define _MDMMAXCO0 _MD_MOD0			//dolna granica wartosci PWM dla mocy maksymalnej CO
#define _MGMMAXCO0 _MG_MOD0+10			//gorna granica wartosci PWM dla mocy maksymalnej CO
//gaz plynny
#define _MDMMAXCOP0 _MD_PMOD0			//dolna granica wartosci PWM dla mocy maksymalnej CO
#define _MGMMAXCOP0 _MG_PMOD0+10			//gorna granica wartosci PWM dla mocy maksymalnej CO
//---------
//parametry zwiazane z moca maksymalna CW
#define _GWMMAXCW0	99				//graniczna wartosc nastawy mocy maksymalnej CW
//gaz ziemny
#define _MDMMAXCW0 _MD_MOD0			//dolna granica wartosci PWM dla mocy maksymalnej CW
#define _MGMMAXCW0 _MG_MOD0+10			//gorna granica wartosci PWM dla mocy maksymalnej CW
//gaz plynny
#define _MDMMAXCWP0 _MD_PMOD0			//dolna granica wartosci PWM dla mocy maksymalnej CW
#define _MGMMAXCWP0 _MG_PMOD0+10			//gorna granica wartosci PWM dla mocy maksymalnej CW
//---------
//***********************************************************************************
//***********************************************************************************
//MAXITERM
//---------
//parametry zwiazane z modulem oscylacji
#define _EN_OSC3 		1			//aktywacja(1)/deaktywacja(0) oscylacji na sygnale PWM
#define _MAX_OSC_PWM3	25			//maksymalna amplituda oscylacji
#define _MIN_OSC_PWM3	5			//minimalna amplituda oscylacji
//---------
//parametry opisujace wlasciwosci modulatora 
#define _MAD_MOD3	0x14			//dolna dozwolona granica mechaniczna warunku ciszy akustycznej przy oscylacjach
#define _MAG_MOD3	0xF7			//gorna dozwolona granica mechaniczna warunku ciszy akustycznej przy oscylacjach
#define _MD_MOD3	0x30			//martwy zakres modulacji - dolna granica 
#define _MD_PMOD3	0x5A			//5Amartwy zakres modulacji - dolna granica (dla gazu plynnego)
#define _MG_MOD3	0xA0			//86martwy zakres modulacji - gorna granica
#define _MG_PMOD3	0xCD			//martwy zakres modulacji - gorna granica (dla gazu plynnego)
#define _I80_MOD3	0x6E			//60% domyslna wartosc pierwszego pobudzenia
#define _I80_PMOD3 	0x9B			//domyslna wartosc pierwszego pobudzenia - gaz plynny

/*
Pierwsze pobudzenie (_I80_MOD lub _I80_PMOD) na poziomie 60% zakresu modulacji
gaz ziemny
(x6E-x30)=x36
/			=54/86*100%=60%
(x86-x30)=x56
gaz plynny
(xC7-x5A)*60%+x5A=x41+x5A=x9B
spr:
(x9B-x5A)=x41
/			=65/109*100%=60%
(xC7-x5A)=x6D
*/

#if _MAX_OSC_PWM3<=_MIN_OSC_PWM3
#error Nieprawidlowe wartosci parametrow _MAX_OSC_PWM,_MIN_OSC_PWM
#endif
#if _MG2_MOD<_MG_MOD3||_MG2_MOD<_MG_PMOD3
#error Nieprawidlowe wartosci parametrow _MG2_MOD,_MG_MOD,_MG_PMOD
#endif
#if _MD_MOD3>_MG_MOD3||_MD_PMOD3>_MG_PMOD3
#error Nieprawidlowe wartosci parametrow _MD_MOD,_MD_PMOD,_MG_MOD,_MG_PMOD
#endif
#if _MAD_MOD3>_MAG_MOD3
#error Nieprawidlowe wartosci parametrow _MAD_MOD,_MAG_MOD
#endif
#if _MAD_MOD3>_MD_MOD3||_MAD_MOD3>_MD_PMOD3
#error Nieprawidlowe wartosci parametrow _MAD_MOD,_MD_MOD,_MD_PMOD
#endif
#if _MAG_MOD3<_MG_MOD3||_MAG_MOD3<_MG_PMOD3
#error Nieprawidlowe wartosci parametrow _MAG_MOD,_MG_MOD,_MG_PMOD
#endif
//-----------------------------
//parametry zwiazane z moca startowa
#define _GWMSTART3	99				//graniczna wartosc nastawy mocy startowej
//gaz ziemny
#define _MDSTART3 _MD_MOD3			//dolna granica wartosci PWM dla mocy startowej
#define _MGSTART3 _MG_MOD3+10			//gorna granica wartosci PWM dla mocy startowej
//gaz plynny
#define _MDSTARTP3 _MD_PMOD3			//dolna granica wartosci PWM dla mocy startowej
#define _MGSTARTP3 _MG_PMOD3+10			//gorna granica wartosci PWM dla mocy startowej
//---------
//parametry zwiazane z moca maksymalna CO
#define _GWMMAXCO3	99				//graniczna wartosc nastawy mocy maksymalnej CO
//gaz ziemny
#define _MDMMAXCO3 _MD_MOD3			//dolna granica wartosci PWM dla mocy maksymalnej CO
#define _MGMMAXCO3 _MG_MOD3+10			//gorna granica wartosci PWM dla mocy maksymalnej CO
//gaz plynny
#define _MDMMAXCOP3 _MD_PMOD3			//dolna granica wartosci PWM dla mocy maksymalnej CO
#define _MGMMAXCOP3 _MG_PMOD3+10			//gorna granica wartosci PWM dla mocy maksymalnej CO
//---------
//parametry zwiazane z moca maksymalna CW
#define _GWMMAXCW3	99				//graniczna wartosc nastawy mocy maksymalnej CW
//gaz ziemny
#define _MDMMAXCW3 _MD_MOD3			//dolna granica wartosci PWM dla mocy maksymalnej CW
#define _MGMMAXCW3 _MG_MOD3+10			//gorna granica wartosci PWM dla mocy maksymalnej CW
//gaz plynny
#define _MDMMAXCWP3 _MD_PMOD3			//dolna granica wartosci PWM dla mocy maksymalnej CW
#define _MGMMAXCWP3 _MG_PMOD3+10			//gorna granica wartosci PWM dla mocy maksymalnej CW
//---------
//***********************************************************************************
//***********************************************************************************
//MINIMAX HB minimax z hydroblokiem
//---------
//parametry zwiazane z modulem oscylacji
//!UWAGA: zalaczyc modul oscylacji mozna w pliku pwm.tp.h
#define _EN_OSC4 		1			//aktywacja(1)/deaktywacja(0) oscylacji na sygnale PWM
#define _MAX_OSC_PWM4	15			//maksymalna amplituda oscylacji
#define _MIN_OSC_PWM4	5			//minimalna amplituda oscylacji
//---------
//parametry opisujace wlasciwosci modulatora
#define _MAD_MOD4	0x14			//dolna dozwolona granica mechaniczna warunku ciszy akustycznej przy oscylacjach
#define _MAG_MOD4	0xF7			//gorna dozwolona granica mechaniczna warunku ciszy akustycznej przy oscylacjach
#define _MD_MOD4	0x2D			//martwy zakres modulacji - dolna granica 
#define _MD_PMOD4	0x35			//martwy zakres modulacji - dolna granica (dla gazu plynnego)
#define _MG_MOD4	0x9F			//martwy zakres modulacji - gorna granica
#define _MG_PMOD4	0xF0			//martwy zakres modulacji - gorna granica (dla gazu plynnego)
#define _I80_MOD4	0x6E			//60% domyslna wartosc pierwszego pobudzenia
#define _I80_PMOD4 	0xA5			//60% domyslna wartosc pierwszego pobudzenia - gaz plynny

/*
Pierwsze pobudzenie (_I80_MOD lub _I80_PMOD) na poziomie 60% zakresu modulacji
gaz ziemny
(X6E-X2D)=X41
/			=65/114*100%=57%
(X9f-X2D)=X72
gaz p造nny
(xE0-x4E)*60%+x4E=x57+x4E =xA5

(xA5-x4E)=x57
/			=87/146*100%=60%
(xE0-x4E)=x92
*/

#if _MAX_OSC_PWM4<=_MIN_OSC_PWM4
#error Nieprawidlowe wartosci parametrow _MAX_OSC_PWM,_MIN_OSC_PWM
#endif
#if _MG2_MOD<_MG_MOD4||_MG2_MOD<_MG_PMOD4
#error Nieprawidlowe wartosci parametrow _MG2_MOD,_MG_MOD,_MG_PMOD
#endif
#if _MD_MOD4>_MG_MOD4||_MD_PMOD4>_MG_PMOD4
#error Nieprawidlowe wartosci parametrow _MD_MOD,_MD_PMOD,_MG_MOD,_MG_PMOD
#endif
#if _MAD_MOD4>_MAG_MOD4
#error Nieprawidlowe wartosci parametrow _MAD_MOD,_MAG_MOD
#endif
#if _MAD_MOD4>_MD_MOD4||_MAD_MOD4>_MD_PMOD4
#error Nieprawidlowe wartosci parametrow _MAD_MOD,_MD_MOD,_MD_PMOD
#endif
#if _MAG_MOD4<_MG_MOD4||_MAG_MOD4<_MG_PMOD4
#error Nieprawidlowe wartosci parametrow _MAG_MOD,_MG_MOD,_MG_PMOD
#endif
//-----------------------------
//-----------------------------
//parametry zwiazane z moca startowa
#define _GWMSTART4	99					//graniczna wartosc nastawy mocy startowej
//gaz ziemny
#define _MDSTART4 _MD_MOD4				//dolna granica wartosci PWM dla mocy startowej
#define _MGSTART4 _MG_MOD4+10			//gorna granica wartosci PWM dla mocy startowej
//gaz plynny
#define _MDSTARTP4 _MD_PMOD4			//dolna granica wartosci PWM dla mocy startowej
#define _MGSTARTP4 _MG_PMOD4+10			//gorna granica wartosci PWM dla mocy startowej
//---------
//parametry zwiazane z moca maksymalna CO
#define _GWMMAXCO4	99					//graniczna wartosc nastawy mocy maksymalnej CO
//gaz ziemny
#define _MDMMAXCO4 _MD_MOD4				//dolna granica wartosci PWM dla mocy maksymalnej CO
#define _MGMMAXCO4 _MG_MOD4+10			//gorna granica wartosci PWM dla mocy maksymalnej CO
//gaz plynny
#define _MDMMAXCOP4 _MD_PMOD4			//dolna granica wartosci PWM dla mocy maksymalnej CO
#define _MGMMAXCOP4 _MG_PMOD4+10			//gorna granica wartosci PWM dla mocy maksymalnej CO
//---------
//parametry zwiazane z moca maksymalna CW
#define _GWMMAXCW4	99					//graniczna wartosc nastawy mocy maksymalnej CW
//gaz ziemny
#define _MDMMAXCW4 _MD_MOD4				//dolna granica wartosci PWM dla mocy maksymalnej CW
#define _MGMMAXCW4 _MG_MOD4+10			//gorna granica wartosci PWM dla mocy maksymalnej CW
//gaz plynny
#define _MDMMAXCWP4 _MD_PMOD4			//dolna granica wartosci PWM dla mocy maksymalnej CW
#define _MGMMAXCWP4 _MG_PMOD4+10		//gorna granica wartosci PWM dla mocy maksymalnej CW
//---------
//***********************************************************************************
//***********************************************************************************
//***********************************************************************************
//***********************************************************************************
//***********************************************************************************
//***********************************************************************************
//***********************************************************************************
//***********************************************************************************
//Obliczenie wartosci PR2 dla wybranej czestotliwosci przebiegu PWM

#ifndef _PLL					//modul PLL (0-nieaktywny,1-aktywny)
#error Nieokreslony parametr _PLL
#endif

//Dla _PLL=0 PR2 obliczamy ze wzoru [1/FPWM/(4*PRE2*1/FOSC)]-1
//Dla _PLL=1 PR2 obliczamy ze wzoru [1/FPWM/(PRE2*1/FOSC)]-1
//Gdzie:
//FOSC - czestotliwosc oscylatora
//FPWM - oczekiwana czestotliwosc przebiegu pwm
//PRE2 - wartosc preskalera dla timer2

#if (_PLL==0)&&((_FPWM==16)||(_FPWM==4)||(_FPWM==1))
#if _FPWM==16
#define _WT2CON	0b00000000		//timer2 preskaler = 1
#define _WPR2	0x7C			//wartosc PR2 dla FPWM=16kHz,FOSC=8MHz,PRE2=1;
#endif
#if _FPWM==4
#define _WT2CON	0b00000001		//timer2 preskaler = 4
#define _WPR2	0x7C			//wartosc PR2 dla FPWM=4kHz,FOSC=8MHz,PRE2=4;
#endif
#if _FPWM==1
#define _WT2CON	0b00000010		//timer2 preskaler = 16
#define _WPR2	0x7C			//wartosc PR2 dla FPWM=1kHz,FOSC=8MHz,PRE2=1;
#endif
#else
#if (_PLL==0)
#error Nieprawidlowy parametr _FPWM 
#endif 
#endif
//----------
#if (_PLL!=0)&&((_FPWM==16)||(_FPWM==4))
#if _FPWM==16
#define _WT2CON	0b00000001		//timer2 preskaler = 4
#define _WPR2	0x7C			//wartosc PR2 dla FPWM=16kHz,FOSC=8MHz,PRE2=4;
#endif
#if _FPWM==4
#define _WT2CON	0b00000010		//timer2 preskaler = 16
#define _WPR2	0x7C			//wartosc PR2 dla FPWM=4kHz,FOSC=8MHz,PRE2=16;
#endif
#else
#if (_PLL!=0)
#error Nieprawidlowy parametr _FPWM
#endif
#endif
//----------------------------------------------------------
//Maksymalna rozdzielczosc PWM obliczamy ze wzoru
//Max PWM resolution=log(FOSC/FPWM)/log(2) [bits]
//Dla FOSC=8MHz i FPWM=16kHz wynosi ona 9 bitow.


/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
/*-----------------------------PROTECTED DATA--------------------------------------*/

//Maksymalna dozwolona wartosc wypelnienia zalezy od _WPR2
//Nalezy ja obliczyc biorac pod uwage ze wypelnienie jest wartoscia 10 bitowa

#if(_WPR2<0x40)
#define _PSHIFT	2
#define _HI_PWM			(_WPR2+1)<<2	//gorna bezwgledna granica wypelnienia
#endif
#if(_WPR2>0x40 && _WPR2<0x80)
#define _PSHIFT	1
#define _HI_PWM			(_WPR2+1)<<1	//gorna bezwgledna granica wypelnienia
#endif
#if(_WPR2>0x80)
#define _PSHIFT	0
#define _HI_PWM			_WPR2+1			//gorna bezwgledna granica wypelnienia
#endif

//kontrola i formatowanie podanych parametrow
#if (_MAG_MOD1>_HI_PWM)
#define _MAG_MOD1		_HI_PWM
#endif
#if (_MAG_MOD2>_HI_PWM)
#define _MAG_MOD2		_HI_PWM
#endif 
//----------------------------------------------------------
void SetOsc(void);
//----------------------------------------------------------
#endif		/*_PWM_H*/
