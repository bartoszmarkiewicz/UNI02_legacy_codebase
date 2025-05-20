//----------------------------------------------------------
#define _WERSJA 1			//18aktualna wersja produkcyjna oprogramowania (1=1.1, 2=1.2 ... 10=2.0)

/*
Wersja 1 - opracowana 16.12.2022
- pierwsza wersja produkcyjna opracowana na podstawie kodu do wersji 18 sterownika kotlow kondensacyjnych
 */


 /*  
							UNI-02	
				v1 											v2                                      v3									v4
	
	v1	(funkcje wer 1)                 		bez stalej ekspozycji temp CW                       jak v2 bez mozliwosci zmiany P18	jak v3 bez P2 na zasobnik
												dla jednofunkcyjnych, bez mozliwosci zmiany         brak funkcji LIN
												P16,P17
																									wybieg 48h po zasileniu
																									podbicie mocy minimalnej po rozr.
																									w obiegu CO.
LCD

	v2									                                    
		etala ekspozycja temp CW     			(funkcje wer. 2) 									brak funkcji LIN  					jak v3 bez P2 na zasobnik
		dla jednofunkcyjnych,                                                                       brak mozliwosci zmiany P18
		bez mozliwosci zmiany P16,P17
																									wybieg 48h po zasileniu
																									podbicie mocy minimalnej po rozr.
																									w obiegu CO.

	v3
        jak v2                                  brak funkcji LIN                                    (funkcje wer 3)						jak v3 bez P2 na zasobnik
	    bez mozliwosci zmiany P18               brak mozliwosci zmiany P18
        brak funkcji LIN
		nie mozna przerwac proc.kal.			nie mozna przerwac proc.kal.


	v4 
		jak v3 bez P2 na zasobnik				jak v3 bez P2 na zasobnik							jak v3 bez P2 na zasobnik			(funkcje ver 4)
	
*/
//----------------------------------------------------------
