/*wersja.h*/
//----------------------------------------------------------
#ifndef	_WERSJA_H
#define	_WERSJA_H
//----------------------------------------------------------
#define _WERSJA 16			//aktualna wersja produkcyjna oprogramowania (1=1.1, 2=1.2 ... 10=2.0) UWAGA max 16

/*
Wersja 1 - opracowana 30.05.2014 
-Wprowadzona demonstracja wersji oprogramowania,
-Wprowadzona manualna blokada grzania zasobnika

Wersja 2 opracowana 29.08.2014 
-Zmieniona domyslna wartosc minimalnych obrotow na 1200 obr/min
-Wprowadzona stala ekspozycja temperatury CW w kotlach jednofunkcyjnych
-Wprowadzona w procedurze konfiguracji obsluga dwoch dodatkowych parametrow odpowiedzialnych za czasowe ograniczenie
 mocy maksymalnej po rozpoczeciu grzania obiegu CO (parametry dostepne przy 2 wersji oprogramowania p³yty sterujacej UNI-02)
-(fix) deaktyuwacja trybu serwisowego/antylegionella w konsoli jesli UNI02 (po ustalonym czasie) nie aktywowala wybranego trybu.
-Wprowadzono w trybie jednofunkcyjnym dwie kreski na wskazie temp.CW przy otwartym styku timera zasobnika.
-Przy podpietym czujniku temp powrotnej CO rozszerzono mozliwosc podgladu temperatury powrotnej na wszystkie przypadki w trybie ZIMA oprocz grzania obiegu CW,
 oraz w trakcie grzania CO w innych trybach.
-Wprowadzone rozwiazania kompatybilnosci wstecznej

Wersja 3 20.03.2015
- wprowadzona obsluga interfejsu LIN (w wersji 3 deaktywowana)
- wprowadzony parametr konfiguracyjny P18 maksymalnej granicy wydajnosci pomy PWM
- wprowadzona mozliwosc przerwania procedury kalibracji przyciskiem minus w pozycji OFF (na potrzeby odbioru produkcyjnego)

Wersja 4 23.04.2015
- optymalizacja algorytmu I2C
 
Wersja 5 14.07.2015 
- wprowadzone globalne zmienne typu bit
- wprowadzony parametr konfiguracyjny P19 maksymalnej granicy wydajnosci pomy PWM dla obiegu WU w kotlach jednofunkcyjnych

Wersja 6 29.09.2015 
- poszerzona lista komend dla magistrali LIN

Wersja 7 02.02.2016, 19.05.2016
- wprowadzone natychmiastowe pobudzenia i flagi stanu komend cyklicznych magistrali LIN
- poszerzona ekspozycja parametrow po nacisnieciu przycisku RESET dla obiegu CO i CW tak¿e przy postoju kot³a
- wprowadzona mozliwosc zmiany parametru Eco przy aktywnym regulatorze pogodowym
- wprowadzona sygnalizacja aktywnosci procedury odpowietrzajacej - kod Po
  (deaktywacja procedury odpowietrzajacej przyciskiem RESET)

Wersja 8 19.08.2016
- wprowadzony parametr konfiguracyjny P23 wyboru rodzaju wentylatora
- poprawiona wizualizacja parametrow w kompatybilnosci wstecznej
 
Wersja 9 18.10.2016, 17.05.2017
- wprowadzony parametr konfiguracyjny P24 (czas wybiegu na mocy startowej dla obiegu CO)
- wprowadzony parametr konfiguracyjny P25 (czas blokady L3)
- wprowadzony parametr konfiguracyjny P26 (rodzaj regulatora pogodowego) 
- wprowadzony parametr konfiguracyjny P27 (wartosc graniczna pobudzenia od regulatora pogodowego) 
- wprowadzony parametr konfiguracyjny P28 (obnizenie nocne dla regulatora pogodowego) 
- wprowadzona nowa, rozszerzona ramka komunikacji przy zachowaniu kompatybilnosci wstecznej
- zniesione ograniczenie dla nr wersji oprogramowania plytki sterujacej (stare maksimum=15, nowe maksimum=31)
- usuniety problem z utrata polozenia pzrelacznika funkcji przed wejsciem na pozycje off
  (Po zaniku zasilania, gdy sterowanie jest na pozycji off to -w poziniejszym czasie- po przytrzymaniu przycisku off 
  (sterownik przechodzil w domyslna pozycje zima. Teraz plytka sterujaca zapamietuje poprzednia pozycje - problem rozwiazany)
- wprowadzona obsluga interfejsow komunikacyjnych 10V,OT,LIN
- implementacja dwoch rozwiazan interfejsu urzytkownika (4 przyciski (tradycyjny), 7 przyciskow) 
- przejscie na nowy procesor PIC18F66j90 z mozliwoscia kompilacji na stary procesor (interfejs 4 przyciskowy bez 10V,LIN,OT)

wersja 10 09.08.2017
- renumeracja

wersja 11 21.05.2018
- (OT) parametry zdalne dla OT (dezaktywowano)
- (LIN) rozszerzono informacja dla specjalnego statusu sterownika
- (LIN) wprowadzono mozliwosc odczytu 16 ostatnich zdarzen awaryjnych do pamieci EEPROM p³yty sterujacej
- (LIN) wprowadzono mozliwosc odczytu statusu kot³a przy wy³±czeniu awaryjnym z blokada trwala (4 ostatnie zdarzenia)
- wprowadzony parametr konfiguracyjny P29 (ogranicznik nastawy CO dla regulatora pogodowego)
- optymalizacja procedury dla interfejsu LIN

wersja 11.1 18.07.2018 
- optymalizacja procedury inicjalizacji polaczenia miedzy plytkami
- dodatkowe warunki dla aktualizacji wartosci parametrow w procedurze konfiguracji 
- reorganizacja zmiennych steruj±cych prac± procedury konfiguracji (celem minimalizacji niepozadanego wplywu kompliatora)

wersja 12 22.10.2018 
- nowa wartosc maksymalna parametru P25 = 3 (praca regulatora pogodowego w trybie jak dla P25 = 1, ale nie jest aktywna autodeaktywacja regulatora pogodowego po podlaczeniu regulatora OT)
- wprowadzony parametr histereza wylaczenia CW(2F) (P30) 
- wprowadzony we. sygnalu timera zasobnika (1F) (P31)
- usprawniono wy¶wietlanie w przypadku powtórnej procedury kalibracji wentylatora
- dezaktywowano wplyw interfejsu OT przy pracy w trybie produkcyjnym=2

/*wersja 14 20.03.2019
- dla OT komendy zmian nastawy PCO i PCW pracuja na wartosciach poza dozwolonym przedzialem (dobierane sa wartosci graniczne)
- dla LIN poprawiona odpowiedz na zapytanie specjalne
- dla LIN wprowadzona funkcja negocjacji rozmiaru multiramki
- dla LIN rozszerzona odpowiedz na zapytanie PCO/PCW 
- modulowany SETP
 */
/*wersja 15 05.11.2019
- optymalizacja interfejsu OT
- zwiekszenie gornej granicy histerezy CO z 10 na 15 (parametr konfiguracyjny)
- kod L3 zastapiony migajacym symbolem KALORYFER
 */
/*wersja 16 10.07.2020
- wsparcie dla plyt UNI02v02 z mikrokontrolerem Q
- wprowadzona mo¿liwosc zapisu parametrow stabilizacji predkosci obrotowej wentylatora PID (interfejsem LIN)
- wprowadzona mo¿liwosc zapisu dolnej granicy wentylatora (interfejsem LIN, aktywne przy wyborze typu wentylatora 2) 
- wprowadzona mo¿liwosc zapisu parametrow modulacji (interfejsem LIN, aktywne przy wyborze typu wentylatora 2)
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
