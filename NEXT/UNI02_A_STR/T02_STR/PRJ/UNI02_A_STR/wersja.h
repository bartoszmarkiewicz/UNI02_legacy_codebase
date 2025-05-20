//----------------------------------------------------------
#define _WERSJA 19			//aktualna wersja produkcyjna oprogramowania (1=1.1, 2=1.2 ... 10=2.0)
/*Oprogramowanie rodziny sterownikow T02 (na bazie pï¿½ytki UNI-02)*/
/*Wersja z dodana obsluga obiegu otwartego (r7) dla wszystkich typow kotla (28.01.2011)*/
/*oprogramowanie dla platformy 3V i 5V*/
/*optymalizacja procedury zanikow ciagu dla kotlow atm. 20.08.2012*/
/*usuniety blad zapisu nastawy pogodowej 12.02.2013*/
/*	09.04.2015
	WERSJA 7:
	temp CO dla kotlow monoterm. zmienione na 89C
	wprowadzone podbicie mocy minimalnej po procedurze startowej
	czas trwania proc serwisowej zmieniony na 60 min
    i2c pokazuje rzeczywista docelowa temp PPCO i PPCW
    powiekszona histereza przy grzaniu CO (+10)
    poprawiona proc GoldenEye dla wiekszej histerezy wylaczenia przy grzaniu CO
*/

/* 01.06.2015
	WERSJA 8:
	eeprom.h - wprowadzone nowe 4 parametry pracy pompy
	ac.h - nowy adres tabeli ntc czujnika pogodowego
	konfig.tp.h - nowe 4 parametry pracy pompy
	konfig.h - granice dla nowych 4 parametrow pracy pompy, liczba parametrow zwiekszona o 4
	main.tp.h - 4 parametry do wysylki po RS
	konfig.c - dodanie 4 parametrow do procedury konfiguracji
	pwm.h - dodanie parametrow konfiguracyjnych pwm2
	pwm.tp.h - dodanie zmiennych dla pwm2
cd...
	17.06.2015 (8)
	-obsluga pompy PWM definiowana 4 parametrami, 
	-osluga czujnika temp na powrocie, kod bledu dla czujnika na powrocie E14
	-mechanizmy kompatybilnosci wstecznej
	-diagnostyka wyjscia PWM podczas rozrychu sterownika
cd...
	20.07.2015
	-optymalizacja algorytmu zaworu 3dr celem minimalizacji upluwu cieplej wody do instalacji CO w trybie LATO
	-optymalizacja algorytmu procedury serwisowej - zapobieganie grzania w obiegu WU
	-optymalizacja algorytmu funkcji antylegionella - stala domyslna nastawa PPCO
*/
/* 23.10.2015
	WERSJA 9:
	- usunieto blad utraty informacji o rodzaju pompy przy zmianie konfiguracji
*/
/* 27.10.2015
	WERSJA 10: (nie wprowadzona!)
	- powrot do gornego marginesu wylaczenia +5C (wprowadzona +10C w wersji 7)
	- domyslna wartosc delta T dla pomy 10C
*/
/* 08.02.2016...01.09.2016
	WERSJA 10:
	-domyslna minimalna predkosc pompy pwm 50%
	-dla obiegu otwartego popmpa pwm pracuje ze stala predkoscia okreslona prametrami P13, P14
	-dla pompy tradycyjnej wyjscie pwm ustawione na 100%
	-wprowadzona maksymalna granica predkosci pompy dla CW
	-wprowadzona regulowana histereza wylaczenia przy grzaniu CO
	-wprowadzona regulowana temp CO przy grzaniu CW zasobnika
	-wprowadzona poprawka dla regulagtora pogodowego (przesuniecie rownolegle krzywej grzania)
	-zwiekszono wartosc minimum dla maksymalnych granic predkosci pompy z 15 na 25
    -modyfikacja filtra wskazan dla obiegu CO (brak filtra dla T>NST lub hist=0 - dostosowanie do zminnej histerezy) 
    -wsparcie dla klapy spalinowej (zwiekszony o 6sek czas do blokady E1 w czasie rozruchu)
    - edycja projektu w srodowisku xide 
*/
/* 26.01.2017
	WERSJA 11:
    - nowa wersja protokolu komunikacyjnego (zwiekszony zakres mozliwych wersji programu, mo?liwo?? transmisji ramki o rozmiarze do 62 bajtow)
    - przygotowanie dla nowej wersji plytki wyswietlacza
    - obsluga pobudzen od interfejsow komunikacyjnych
    - wprowadzony SETP dla cedlow sterowania z interfejsow zewnetrznych
    - dla kotlow unico wprowadzona blokada zaworu w obiegu CO dla temp na zasobniku >85C
    - dla kotlow unico wprowadzony przedbieg pompy i oczekiwanie na przelaczenie zaworu 3DR przed zalaczeniem plytki zapalacza
28.08.2017
	WERSJA 11.:
    - P16 wartosc domyslna zmieniona z 89 na 75
 */
/*
 21.05.2018
 WERSJA 12
 - skrocony czas E1 z 30 na 16 sek.
 - wprowadzony parametr przesuniecia dla regulatora pogodowego (P17) 
 - wprowadzony parametr ogranicznika CO dla regulatora pogodowego (P18)
 - wprowadzono zapis 16 ostatnich zdarzen awaryjnych do pamieci EEPROM
 - wprowadzony zapis do EEPROM statusu kotla przy wylaczeniu awaryjnym z blokada trwala (4 ostatnie zdarzenia) 
 */
/*
 18.10.2018
 WERSJA 14
 - wprowadzono obsluge flagi serwisowej dla historii zdarzen awaryjnych
 - wprowadzone niezalezne nastawy dla PCO i PGD
 - wprowadzony parametr "ogranicznika CO dla regulatora pogodowego" (P18), zmieniony na "parametr ogranicznika CO"
 - wprowadzony parametr tryb pracy regulatora pogodowego (P19)
 - wprowadzony parametr histereza wylaczenia CW(4F) (P20) 
 - wprowadzony we. sygnalu timera zasobnika (1F) (P21) 
 - (!) poprawiony zapis zdalnych parametrow (wprowadzone wlasciwe nr parametrow 4,11,12) 
 */
/*
Wersja 15 (20.03.2019)
- usuniete wsparcie dla pomp dwukierunkowych (pompa si? nie wylacza na czas zmiany obiegu - pompy ze sterowaniem elektronicznym maja dluzszy czs wzbudzenia do pracy po wylaczeniu)
- wprowadzone sterowanie z priorytetem SETPOINT (warto?? SETPOINT otrzymywana z zewnetrznego regulatora temperatury jest punktem odniesienia dla wylaczenia) 
 */
/*
Wersja 16 (05.11.2019)
- wejscie do procedury konfiguracji bez koniecznosci wylaczania sterownika
- maksymalna wartosc histerezy dla wylaczenia CO zmieniona z 10 na 15
*/
/*
Wersja 17 (16.03.2020)
-przejscie na mikrokontroler Q,
-zmiana kompilatora na xc8
 
(06.07.2020)
-optymalizacja algorytmów PWM
-optymalizacja algorytmów zwiazanych z przetwornikiem AC

(10.07.2020)
- fitracja flag stanu czujnikow NTC
- optymalizacja algorytmu komunikacji z plytka wyswietlacza (UART)
 
(14.08.2020)
- uzaleznienie kontynuacji wybiegu pompy od trwania pobudzenia,
- w kotlach z zasobnikiem dla aktywnego pobudzenia od zasobnika ale przy za wysokiej temp CO wprowadzony wstepny wybieg pompy
- dodanie opcji kompilacji "SOLAR"

(15.09.2020)
- usuniety blad filtracji stanu flagi informujacej o podlaczeniu czujnika temp. zew.

(16.09.2020)
ROZPOCZECIE PRODUKCJI
 * 
Wersja 18 (26.05.2021)
- wprowadzona wartosc domyslna parametru P18 wybiegu pompy 80%
- wprowadzony obnizony do 30% wybieg pompy oraz obnizony wydatek gazu do 0% na czas przelaczania zaworu 3Dr (5 sek.)
(09.08.2021)
- wprowadzone maksimum mocy serwisowej uzaleznione od wartosci parametru P3
 * UWAGA: wycofano sie z zuzaleznienia mocy serwisowej od P3 !!!

Wersja 19 (02.02.2022)
- adres wlasny LIN poszerznony do zakresu 1..4, neutralny 5, 9..12
(11.05.2022)
- zwiekszony czas oczekiwania na zanik przeplywu w ukladach z obiegiem otwartym
(23.06.2022)
- ustawiono domyslna wartosc parametru pracy automatycznej antylegionella P05 na 1
- wylaczenie pracy na zasobnik (nastawa obnizona az na wyswietlaczu pokaza sie dwie kreski) nie pozwala na prace w trybie antylegionella
*/
//----------------------------------------------------------
