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
*/
//----------------------------------------------------------
