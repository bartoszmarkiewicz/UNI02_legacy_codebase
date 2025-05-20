/*wersja.h*/
//----------------------------------------------------------
#ifndef	_WERSJA_H
#define	_WERSJA_H
//----------------------------------------------------------
#define _WERSJA 1			//18aktualna wersja produkcyjna oprogramowania (1=1.1, 2=1.2 ... 10=2.0) UWAGA max 16
                            //UWAGA: dla serwisu 17

/*
Wersja 1 - opracowana 15.06.2023 
-dostosowanie oprogramowania do wspolpracy z plytka sterujaca UNI02v02 dla zrodla szczytowego
(22.07.2023)
- zwiekszony czas oczekiwania na rozpoczecie transmisji (_TS_TSTR)
- zwiekszony czas timeout dla transmisji (_TS_TOUT)
(15.08.2023)
- wprowadzony odczyt danych sterujacych z modulow automatyki 
(16.08.2023)
- wprowadzona wysylka do plyty sterujacej informacji o pobudzeniu na wysokich parametrach i rodzaju plytki wyswietlacza (czy jest z potencjometrami)
(20.08.2023)
- poszerzona wymiana danych miedzy plytka sterujaca a plytka wyswietlacza
- poszerzona wymiana danych na interfejsie LIN
(21.08.2023) 
- uzupelnienie informacji przesylanej w uniwersalnych danych statusowych 
- poprawiona procedura wyswietlania przeplywu CO
(22.08.2023)
- wprowadzony migajacy symbol "kran" przy grzaniu na parametrach wysokich
(29.09.2023)
- rozbudowa procedur wprowadzania danych o wartosci ze znakiem, usprawnienia funkcji wizualizacji
- wprowadzona procedura konfiguracji KPAR
(30.09.2023)
- wprowadzona mozliwosc nadawania adresu w trybie automatycznym
(07.10.2023)
- poprawa wyswietlania min/max przy modyfikacji nastaw mocy grzewczej      //mmm
- optymalizacja procedury nadawania adresu ALIN      //nnn
(29.10.2023)
- optymalizacja procedury komunikacji na interfejsie LIN
(30.10.2023)
- poprawka wyswietlania min/max przy modyfikacji nastaw mocy grzewczej 
(08.12.2023)
- na czas grzania na parametrach wysokich blokowana jest obsluga przyciskow +/-
- wprowadzone testowe pobudzenie na parametrach niskich
(17.03.2024)
- wprowadzenie pakietu dalnych udostepnianych na zapytanie _CLIN_RRD_SETPP (LIN)
- poprawka aktualizacji flagi PFNf_pgd
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
