/*wersja.h*/
//----------------------------------------------------------
#define _WERSJA 1			//aktualna wersja produkcyjna oprogramowania (1=1.1, 2=1.2 ... 10=2.0)

/*
Wersja 1 - opracowana 02.03.2024	
-przejscie na mikrokontroler Q,
-zmiana kompilatora na xc8

(06.07.2020)
-optymalizacja algorytmów PWM
-optymalizacja algorytmów pomiaru i zadawania predkosci obrotowej wentylatora 
-optymalizacja algorytmów zwiazanych z przetwornikiem AC

(20.08.2020)
- realizacja na matrycy CLC detekcji przep?ywu powietrza

(24.08.2020)
- przypisanie PWM3 timera TMR4
- wprowadzenie nowych tabel dla VNT_PWM
- zmodyfikowane wartosci graniczne dla parametrow w konfiguracji

(03.09.2020)
- filtracja sygna?u presostatu (1sek/0.4sek)
- czas wybiegu wentylatora o 2 sek. krótszy od czasu próby ciagu
- zmiana maksymalnej liczby prob ciagu z 10 na 3

(03.02.2021)
- aktualizacja tabel oporow kominowych (zestawy pod rozne moce urzadzenia r8)
- aktualizacja tabel zaleznosci miedzy wydatkiem na modulatorze a predkoscia wentylatora (dla gazu plynnego, ziemnego i urzadzenia duzej mocy)
- wprowadzenie automatycznej zaleznosci predkosci startowej wentylatora (r6) od nastawionej wartosci startowej modulatora (r1) przy okreslonej dlugosci komina (r7) i mocy urz?dznia (r8)
- wprowadzone oczekiwanie na predkosc startowa wentylatora przed zapaleniem gazu
- optymalizacja procedury modulacji z uwzglednieniem bezwladnosci wentylatora
- usuniety blad wewnetrzny kompilatora powodujacy brak mozliwosci ponownego zalaczenia grzania WU (dezaktywowal timer odpowiedzialny za filtracje sygnalow WE/WY)

(05.02.2021)
- optymalizacja procedury modulacji

(24.02.2021)
- optymalizacja modulacji dla grzejnikow niskiej mocy
- eliminacja migotania wyswietlacza w czasie zapalania plomienia
*/
//----------------------------------------------------------
