/*wersja.h*/
//----------------------------------------------------------
#ifndef	_WERSJA_H
#define	_WERSJA_H
//----------------------------------------------------------
#define _WERSJA 17			//aktualna wersja produkcyjna oprogramowania (1=1.1, 2=1.2 ... 10=2.0) 

/*
Wersja 11 - opracowana 31.01.2017 
-Wprowadzona demonstracja wersji oprogramowania,
-Wprowadzona rozszerzona ramka komunikacji
-Implementacja rozwiazan interfejsu uzytkownika z kotlow kondensacyjnych (4 przyciski, 7 przyciskow)
-Wprowadzone interfejsy 10V,OT,LIN
-przejscie na nowy procesor PIC18F66j90 z mozliwoscia kompilacji na stary procesor (interfejs 4 przyciskowy bez 10V,LIN,OT) 
*/
/*
wersja 12 21.05.2018
- (OT) parametry zdalne dla OT (dezaktywowano)
- (LIN) rozszerzono informacja dla specjalnego statusu sterownika
- (LIN) wprowadzono mozliwosc odczytu 16 ostatnich zdarzen awaryjnych do pamieci EEPROM p³yty sterujacej
- (LIN) wprowadzono mozliwosc odczytu statusu kot³a przy wy³±czeniu awaryjnym z blokada trwala (4 ostatnie zdarzenia)
- wprowadzony parametr kofiguracyjny P17 (przesuniecie dla regulatora pogodowego)
- wprowadzony parametr konfiguracyjny P18 (ogranicznik nastawy CO dla regulatora pogodowego)
 */ 
/*wersja 14 25.10.2018
- optymalizacja procedury inicjujacej polaczenie miedzy plytka sterujaca a plytka wyswietlacza (jak w sterowaniu do kotlow kondensacyjnych)
- optymalizacja procedury konfiguracyjnej(jak w sterowaniu do kotlow kondensacyjnych)
- wprowadzony parametr kofiguracyjny P19 (tryb pracy regulatora pogodowego)
- wprowadzony parametr konfiguracyjny P20 (histereza wylaczenia CW(4F))
- wprowadzony parametr konfiguracyjny P21 (we. sygnalu timera zasobnika (1F)) 
*/
/*wersja 15. 20.03.2019
- dla OT komendy zmian nastawy PCO i PCW pracuja na wartosciach poza dozwolonym przedzialem (dobierane sa wartosci graniczne)
- dla LIN poprawiona odpowiedz na zapytanie specjalne 
- dla LIN wprowadzona funkcja negocjacji rozmiaru multiramki 
- dla LIN rozszerzona odpowiedz na zapytanie PCO/PCW
 */
/*wersja 16 05.11.2019
- optymalizacja interfejsu OT
- zwiekszenie histerezy dla CO z 10 na 15
- kod L3 zastapiony migajacym symbolem KALORYFER
 */
 /*wersja 17 24.07.2020
- renumeracja (wprowadzenie na plycie sterujacej mikrokontrolera Q)
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
