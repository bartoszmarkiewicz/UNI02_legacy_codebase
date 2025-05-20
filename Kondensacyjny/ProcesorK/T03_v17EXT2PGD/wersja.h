/*wersja.h*/
//----------------------------------------------------------
#ifndef	_WERSJA_H
#define	_WERSJA_H
//----------------------------------------------------------
#define _WERSJA 15			//aktualna wersja produkcyjna oprogramowania (1=1.1, 2=1.2 ... 10=2.0)

/*
Wersja 1 - opracowana 30.05.2014	
-Wprowadzona demonstracja wersji oprogramowania,
-Wprowadzona manualna blokada grzania zasobnika

Wersja 2 - opracowana 29.08.2014 	
-Wprowadzona wstepna moc minimalna dla grzania CO 
	(
		Wynosi 1600 obr/min i jest aktywna dla ustawionej w konfiguracji mocy minimalnej ponizej 1600obr./min
		aktywna przez 10 sek od zakonczenia procedury rozruchu.
		Wstepna moc minimalna aktywna jest dla normalnego grzania obiegu CO oraz przy grzaniu od temp dyzurnej dla obiegu CO i CW
	)
-Wprowadzono wstepne ograniczenie mocy maksymalnej dla grzania CO (aktywacja wymaga minimum 2 wersji oprogramowania LCD)
	{
		Aktywowane parametrem P15!=0, wartosc parametru okresla czas trwania ogramiczenia w minutach od momentu zakonczenia procedury rozruchu.
		Parametr P16 okre?la w procentach wielko?? czasowego ograniczenia mocy maksymalnej dla grzania CO.
		Ograniczenie funkcionuje wy?¹cznie w trakcie normalnego grzania obiegu CO.
		Po zakonczeniu ograniczenia narost mocy na palniku jest dwukrotnie spowolniony az do przejscia w faze 2 modulacji.
	}
-Wprowadzone rozwiazania kompatybilnosci wstecznej


 Wersja 3 
 - czas aktywnosci funkcji serwisowej 60 min (_TSRV1).
 - wprowadzono nowy parametr P18 gornej granicy przedkosci obrotowej pompy pwm
 - wprowadzono adres LIN odbiornika do pamieci EEPROM (domyslny 5 - czyli nieaktywny)

 - wprowadzono wybieg 48h w obiegu CO po zasileniu sterownika
 - wprowadzono wstepne podbicie mocy minimalnej po rozruchu i wybiegu na mocy startowej w obiegu CO do wartosci mocy startowej ale nie mniej niz 1600 i nie wiecej niz 3500
   funkcja aktywna przez 3 sek.	
 - mozliwosc przerwania procedury kalibracji wentylatora przyciskiem minus (na potrzeby odbiorow)	
 - punkt wy??czenia wody grzewczej dla grzania wody u?ytkowej >=91°, punkt za??czenia wody grzewczej dla grzania wody u?ytkowej <=87									 - punkt wy??czenia wody grzewczej = [nastawa +10°] ale nie wi?cej ni? [maksymalna warto¶? nastawy +5°] 
 										

Wersja 4 (01.06.2015)
 - poprawa: dla kotlow jednofunkcyjnych dziala ogrzniczenie mocy na zasobnik (P2)
 - usprawnienie I2C

Wersja 5 
 - //wprowadzono obsluge komend interfejsu LIN
 - optymalizacja procedury przerwania kalibracji wentylatora przyciskiem minus (wprowadzono domyslne wartosci kalibracji)
 - poprawiono algorytm wyboru obiegu pompy przy roznych trybach pracy (minimalizacja dogrzewu obiegu CW)
 - wprowadzony parametr P19 dla pompy PWM (ograniczenie predkosci dla obiegu CW)
 - poprawiono algorytm grzania dla ANTYLEGIONELLA (prawidlowa wartosc docelowa PPCO, prawidlowo interpretowany parametr oraniczenia mocy P2)
 - optymalizacja algorytmu modulacji pompy PWM w prostym trybie deltaT

Wersja 6 (28.09.2015)

 - zwiekszona precyzja pomiaru predkosci obrotowej wentylatora
 - zwiekszona precyzja modulacji na granicach zakresu.
 - wprowadzona mozliwosc zdalnego sterowania pobudzeniami
 - dodano komendy zdalnego sterowania z magistrali LIN (mozliwo¶? zmiany parametrow pracy)
 - poprawiona ekspozycja procentowej wartosci predkosci wentylatora (Fr)
 - powrot do temp wylaczenia +5
 - zmieniona domyslna wartosc delta t z 20 na 10 (P13)

Wersja 7 (08.02.2016) 19.05.2016

 -poprawiony blad zapisu kodu ERR do EEPROM (!wystepowal od wersji 3)
 -domyslna minimalna predkosc pompy pwm 50%
 -zmieniona domyslna wartosc delta t z 10 na 6 (P13)
 -mozliwosc zminy wspolczynnika Eco przy regulatorze pogodowym
 -dla obiegu otwartego popmpa pwm pracuje ze stala predkoscia okreslona prametrami P18,P19
 -dla pompy tradycyjnej wyjscie pwm ustawione na 100%
 -wprowadzona regulowana histereza wylaczenia przy grzaniu CO
 -wprowadzona regulowana temp CO przy grzaniu CW zasobnika
 -wprowadzone przesuniecie rownolegle krzywej grzania
 -zwiekszono wartosc minimum dla maksymalnych granic predkosci pompy z 15 na 25
 -wprowadzona procedura odpowietrzjaca z blokada grzania na czas odpowietrzenia (aktywowana dla wer. wsw>=7)
 -zmieniona dolna wartosc graniczna PWM wentylatora z 18 na 31 oraz wprowadzono poprawke kalibracyna dla NG60
 -modyfikacja filtra wskazan dla obiegu CO (brak filtra dla T>NST lub hist=0 - dostosowanie do zminnej histerezy)
 -w kot?ach zasobnikowych dla przekroczonej granicznej temp CO (po grzaniu CW) przy braku pobydzenia RT wybieg pompy pozostaje w obiegu CW

Wersja 7.1 (21.07.2016)
 -usuniety blad braku mozliwosci modyfikacji minimalnego wydatku pompy (P14)

Wersja 8 (17.08.2016)
 - edycja projektu w srodowisku xide
 - wprowadzony parametr konfiguracyjny dla obslugi roznych typow wentylatorow (domyslny typ 1 - NG40)
 - przedluzony czas decyzji kodu E1 (o 6 sek) w proceduzre rozruchu (unifikacja procedury rozruchu)
 
Wersja 9 (10.10.2016...11.01.2017)
- rozszerzenie protokolu komunikacji (zwiekszony zakres mozliwych wersji programu, mo¿liwo?æ transmisji ramki o rozmiarze do 62 bajtow)
- plytka LCD uzyskuje informacje o temp zew. i wartosciach granicznych wartosci PCO przy aktywnym wew. regulatorze pogodowym
- wprowadzenie parametru P24 zmiany czasu wybiegu na mocy startowej przy grzaniu CO (domyslny 2 sek.)
- wprowadzenie parametru P25 zmiany czasu blokady L3 obiegu CO (domyslny 3 min.) 
- zmiana domyslnego typu wentylatora z 1 na 0
- tryb ECO domy?lnie wylaczony
- rozszerzenie wsparcia dla interfejsu LIN
- wsparcie dla interfejsow 10V,OT

Wersja 10 (09.08.2017)
- dolna granice predkosci obrotowej mozliwa do ustawienia w konfiguracji zmieniono z 1000 obr na 1200
- domyslna wartosc granicy minimalnej obrotow zmieniono z 1200 obr na 1500
- poprawa algorytmu regulacji dla wentylatorow FIME
28.08.2017
- P21 zmiana wartosci domyslnej z 89 na 75
 
Wersja 10.1 (10.2017) * 
- usuniety blad nastawy pogodowej - po resecie przyjmuje wartosc domyslna

Wersja 11 (21.05.2018)
- skrocony czas E1 z 30 na 16 sek.
- wprowadzony parametr ogranicznika CO dla regulatora pogodowego (P29)
- wprowadzono zapis 16 ostatnich zdarzen awaryjnych do pamieci EEPROM
- wprowadzony zapis do EEPROM statusu kot³a przy wy³?czeniu awaryjnym z blokada trwala (4 ostatnie zdarzenia)
- zmieniony warunek dla recznego przerwania proc. kalibracji wentylatora (by³ przycisk MINUS, od teraz jest przycisk MINUS razem z przyciskiem SET)
- zaktualizowane tabele wentylatorow FIME i NG40
- zoptymalizowana procedura kalibracji wentylatora (wydluzony wybieg dla kroku 0, zerowanie wszystkich zmiennych po restarcie mikrokontrolera) 
  
Wersja 11.1 (16.07.2018)
- poprawka majaca na celu wyeliminowanie przypadkow zmiany nastawy PCO, PCW do wartosci minimum po resecie z blokady trwalej

Wersja 12 (18.10.2018)
- kolejny tryb racy regulatora pogodowego P25==3 gdzie regulator pracuje jak dla P25==1 ignorujac aktywnosc interfejsow komunikacyjnych
- parametr P29 stanowi od tej pory graniczna wartosc temperatury zasilania CO
- wprowadzono obsluge flagi serwisowej dla historii zdarzen awaryjnych
- wprowadzone niezalezne nastawy dla PCO i PGD
- wprowadzony parametr histereza wylaczenia CW(2F) (P30) 
- wprowadzony we. sygnalu timera zasobnika (1F) (P31)
- rekalibracja wentylatora po zmianie parametru P23(rodzaj wentylatora) i wyj?ciu z procedury konfiguracji

Wersja 14 (20.03.2019)
- usuniete wsparcie dla pomp dwukierunkowych (pompa siê nie wylacza na czas zmiany obiegu - pompy ze sterowaniem elektronicznym maja dluzszy czs wzbudzenia do pracy po wylaczeniu)
- wprowadzone sterowanie z priorytetem SETPOINT (warto?æ SETPOINT otrzymywana z zewnetrznego regulatora temperatury jest punktem odniesienia dla wylaczenia)

Wersja 15 (05.11.2019)
- wejscie do procedury konfiguracji bez koniecznosci wylaczania sterownika
- maksymalna wartosc histerezy dla wylaczenia CO zmieniona z 10 na 15

Wersja 15. (28.05.2021)
- obnizona predkosc pompy na czas przelaczenia Z3Dr
- obnizony wydatek na modulatorze na czas przelaczania Z3Dr
- warto?æ domy?lna gornej granicy predkosci pompy zmieniona ze 100% na 80%
(09.08.2021)
- wprowadzone maksimum mocy serwisowej uzaleznione od wartosci parametru P3
(15.09.2021)
- powrot do tradycyjnej procedury serwisowej
 *  
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
#endif		/*_WERSJA*/
