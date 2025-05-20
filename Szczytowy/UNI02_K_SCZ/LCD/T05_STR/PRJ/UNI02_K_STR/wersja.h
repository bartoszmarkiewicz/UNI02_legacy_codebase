//----------------------------------------------------------
#define _WERSJA 1		//szczytowe zrodlo ciepla do systemow grzewczch z pompami ciepla

/*
Wersja 1 - opracowana 15.06.2023
- wprowadzona obsluga wyswietlacza LCD (modyfikacja wersji LED)
- wprowadzona obsluga impulsowego czujnika przeplywu CO	
(17.08.2023)
- wprowadzone zdalne wywolanie grzania na wysokich parametrach
(21.08.2023)
- wprowadzona zwrotna informacja o wywolaniu grzania na wysokich parametrach 
(26.08.2023)
- adres wlasny LIN ograniczony do zakresu 1..5
(29.09.2023)
- domyslny adres wlasny LIN zmieniony z 2 na 1 (z punktu widzenia automatyki kociol w kaskadzie hybrydy jest elementem wiodacym) 
(30.09.2023)
- poprawa wartosci przeplywu CO, ktora przesylana jest do plytki wyswietlacza
- poprawki w trybie DEMO
(24.10.2023)
- wprowadzony unikalny identyfikator I2C sterownika
(30.10.2023)
- poprawka dla pobudzenia RT z interfejsow LIN i OT
(09.11.2023)
- tabelaryzacja czujnika przeplywu CO
(11.11.2023)
- poprawiony unikalny identyfikator I2C sterownika (0x80)
(13.11.2023)
- poprawki w kierunku umozliwienia odbioru automatycznego na stanowisku produkcyjnym
(14.11.2023)
- poprawki w kierunku umozliwienia odbioru automatycznego na stanowisku produkcyjnym 
*/
