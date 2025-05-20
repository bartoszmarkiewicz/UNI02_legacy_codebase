/*wersja.h*/
//----------------------------------------------------------
#ifndef	_WERSJA_H
#define	_WERSJA_H
//----------------------------------------------------------
#define _WERSJA 1			//18aktualna wersja produkcyjna oprogramowania (1=1.1, 2=1.2 ... 10=2.0) UWAGA max 16
                            //UWAGA: dla serwisu 17

/*
Wersja 1 - opracowana 16.12.2022
- pierwsza wersja produkcyjna opracowana na podstawie kodu do wersji 18 sterownika kotlow kondensacyjnych 
 */
 /*
							UNI-02	
				v1 									v2                                          v3
	
	v1	(funkcje wer 1)					                bez etalej ekspozycji temp CW                           jak v2, bez P18
										dla jednofunkcyjnych, bez mozliwosci zmiany 
										P16,P17,bez 2 kresek na CW przy otwartym
										styku timera zasobnika
LCD

	v2									(wszystkie funkcje wer. 2)                              bez P18 i LIN
		etala ekspozycja temp CW
		dla jednofunkcyjnych, 
		bez mozliwosci zmiany P16,P17
		bez 2 kresek na CW przy otwartym
		styku timera zasobnika



        v3      jak v2, bez LIN, bez P16,17,18                                  jak v2, bez LIN, bez P18                                (wszystkie funkcje V3)
                nie mozna przerwac proc. kalibracji                             nie mozna przerwac proc. kalibracji
*/
//----------------------------------------------------------
#endif		/*_WERSJA*/
