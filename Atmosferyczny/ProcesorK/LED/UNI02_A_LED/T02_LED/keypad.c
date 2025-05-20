/*keypad.c*/
//Funkcje obslugi przycisku RESET dla celow konfiguracji
// (!) Bazuje na funkcjach z plikow: rtimer.c
/*---------------------------------------------------------------------------------*/
#include <pic18.h>
#include "global.h"			//parametry globalne
#include "keypad.h"			//parametry lokalne

/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
#if _EN_PK!=0
volatile StPin ky[_EN_PK];
#endif
#if _EN_DPK!=0
volatile StPin dky[_EN_DPK];
#endif
#if _EN_EXPK!=0
volatile StPin xky[_EN_EXPK];
#endif
#if _EN_EXXPK!=0
volatile StPin xxky[_EN_EXXPK];
#endif

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//Timer na potrzeby obslugi przyciskow (umiescic w przerwaniu od zegara TMR1)

void IncKeyTimer(void)
{
    unsigned char i;
#if _EN_PK!=0
    for(i = 0; i < _EN_PK; i++)
    {
        ky[i].cS++;
        if(ky[i].cS > 9)
        {
            ky[i].cS = 0;
            if(ky[i].dS < 255) ky[i].dS++;
        }
    }
#endif
#if _EN_DPK!=0
    for(i = 0; i < _EN_DPK; i++)
    {
        dky[i].cS++;
        if(dky[i].cS > 9)
        {
            dky[i].cS = 0;
            if(dky[i].dS < 255) dky[i].dS++;
        }
    }
#endif
#if _EN_EXPK!=0
    for(i = 0; i < _EN_EXPK; i++)
    {
        xky[i].cS++;
        if(xky[i].cS > 9)
        {
            xky[i].cS = 0;
            if(xky[i].dS < 255) xky[i].dS++;
        }
    }
#endif
#if _EN_EXXPK!=0
    for(i = 0; i < _EN_EXXPK; i++)
    {
        xxky[i].cS++;
        if(xxky[i].cS > 9)
        {
            xxky[i].cS = 0;
            if(xxky[i].dS < 255) xxky[i].dS++;
        }
    }
#endif
}
//-------------------------------------------------------------------------------
//Restart zegara przycisku

void StartKeyTim(volatile StPin *ky)
{
    (*ky).cS = 0;
    (*ky).dS = 0;
}
//-------------------------------------------------------------------------------
//Odczyt stanu zegara przycisku

unsigned char KeyTim(volatile StPin *ky)
{
    return(*ky).dS;
}
/*---------------------------------------------------------------------------------*/
//-----------------------------------------------------------
//Funkcja detekcji nacisniecia przycisku (wersja podstawowa)
//k-fizyczne wejscie procesora dla przycisku
//ky - pamiec stanu przycisku
//ng - typ reakcji: dla 0 zwraca 1 przy nacisnieciu przycisku, dla 1 przy puszczeniu przycisku 
//np. PresKey(RdPrt(S_SET),&ky[S_SET])
/*
#if _EN_PK!=0
unsigned char PresKey(unsigned char k,StPin *ky,unsigned char ng)
{
        (*ky).bk=(k)?1:0;
        if ((!ng&&(!(*ky).sbk&&(*ky).bk))||(ng&&((*ky).sbk&&!(*ky).bk)))	//nacisnieto przycisk?
        {
                (*ky).sbk=((*ky).bk)?1:0;
                return 1;
        }
        (*ky).sbk=((*ky).bk)?1:0;
        return 0;
}
void SetBufKey(StPin *ky)
{
        (*ky).sbk=1;
}
void ResBufKey(StPin *ky)
{
        (*ky).sbk=0;
}

#endif
 */
//-----------------------------------------------------------
//Funkcja detekcji nacisniecia przycisku (I wersja rozszerzona)
//k-fizyczne wejscie procesora dla przycisku
//ky - pamiec stanu przycisku
//ng - typ reakcji: dla 0 zwraca 1 przy nacisnieciu przycisku, dla 1 przy puszczeniu przycisku 
//np. PresKey(RdPrt(S_SET),&ky[S_SET])
//funkcja zwraca:
//0 - przycisk w stanie spoczynku
//1...99 - przycisk wcisniety lub podtrzymanie stanu przycisku, wartosc rowna czasowi przytrzymania przycisku
//100 - dla szybkiego nacisniecia przycisku
#if _EN_PK!=0

unsigned char PresKeyEX(unsigned char k, volatile StPin *ky)
{

    (*ky).bk = (k) ? 1 : 0;
    switch((*ky).krk)
    {
            //oczekiwanie na nacisniecie przycisku
        case 0:
        {
            if(!(*ky).sbk && (*ky).bk) //nacisnieto przycisk?
            {
                (*ky).sbk = 1; //((*ky).bk)?1:0;
                StartKeyTim(ky);
                (*ky).krk = 1;
            }
            else (*ky).sbk = ((*ky).bk) ? 1 : 0;
            return 0;
        }
            //oczekiwanie na puszczenie przycisku
        case 1:
        {
            (*ky).val = KeyTim(ky) + 1; //kod przytrzymania przycisku
            if((*ky).val > 99) (*ky).val = 99;
            if((*ky).sbk && !(*ky).bk) //puszczono przycisk?
            {
                (*ky).sbk = 0; //((*ky).bk)?1:0;
                if(((*ky).val - 1) < _TFASTK)
                {
                    (*ky).val = 100; //kod szybkiego klikniecia
                }
                StartKeyTim(ky);
                (*ky).krk = 2;
                return(*ky).val;
            }
            return(*ky).val;
        }
            //podtrzymanie stanu przycisku
        case 2:
        {
            if((KeyTim(ky) > 1 && !(*ky).sbk && (*ky).bk) || KeyTim(ky) >= _TMEMK) //ponownie nacisnieto przycisk lub minal czas podtrzymania?
            {
                (*ky).krk = 0;
                return 0;
            }
            return(*ky).val; //przez 1 sek podtrzymaj kod stanu przycisku
        }
        case 3: //oczekiwanie na puszczenie przycisku
        {
            if(!(*ky).bk) //puszczono przycisk?
            {
                (*ky).sbk = 0;
                (*ky).krk = 0;
            }
            return 0;
        }
    }
    return 0;
}
//Resetuje czas podtrzymania

void ResBufKeyEx(volatile StPin *ky) //resetuj podtrzymanie kodu stanu przycisku
{
    (*ky).sbk = 0;
    (*ky).krk = 0;
    (*ky).val = 0;
}
//Wymusza oczekiwanie na zwolnienie przyciskow (na czas oczekiwania funkcja zwraca 0)

void WaitToLowEx(volatile StPin *ky) //wymus oczekiwanie na puszczenie przycisku
{
    (*ky).krk = 3;
    (*ky).val = 0;
}
#endif
/*---------------------------------------------------------------------------------*/
#if _EN_DPK!=0
//Funkcja detekcji podwojnego nacisniecia przycisku
//k-fizyczne wejscie procesora dla przycisku
//np. DoublePresKey(RdPrt(S_SET),&ky[S_SET])

unsigned char DoublePresKey(unsigned char k, StPin *ky)
{
    (*ky).c = PresKey(k, ky);
    if(!(*ky).dk && (*ky).c) //pierwsze nacisniecie przycisku?
    {
        (*ky).dk = 1;
        StartKeyTim(ky);
        return 0;
    }
    (*ky).t = (KeyTim(ky) <= 5);
    if((*ky).dk && (*ky).t)
    {
        if((*ky).c) //drugie nacisniecie przycisku?
        {
            (*ky).dk = 0;
            return 1;
        }
    }
    if(!(*ky).t) (*ky).dk = 0; //czas uplynol?
    return 0;
}
#endif
/*---------------------------------------------------------------------------------*/
#if _EN_EXPK!=0
//Funkcja detekcji nacisniecia przycisku (wersja zaawansowana)
//dla danych liczbowych (<2.5 cyfry)
//k-fizyczne wejscie procesora dla przycisku,
//dTim-czas po jakim przy nacisnietym przycisku nastapi wygenerowanie ciagu impulsów [*0.1sek],
//pTim-czas miedzy kolejnymi impulsami przy spelnieniu warunku dTim [*0.1sek]
//zwraca 0 dla przycisku w stanie spoczynku
//zwraca 1 dla pojedynczego nacisniecia przycisku
//zwraca 2 dla impulsu w trakcie podtrzymania pzrycisku
//np. ExPresKey(RdPrt(S_OFF),&xxky[S_OFF],50,1)

unsigned char ExPresKey(unsigned char k, StPin *ky, unsigned char dTim, unsigned char pTim)
{
    (*ky).bk = (k) ? 1 : 0;

    if(!(*ky).sbk && (*ky).bk)
    {
        (*ky).sbk = ((*ky).bk) ? 1 : 0;
        StartKeyTim(ky);
        return 1; //nacisnieto przycisk
    }
    (*ky).sbk = ((*ky).bk) ? 1 : 0;
    if((*ky).sbk && (*ky).bk)
    {
        if(!(*ky).ap && KeyTim(ky) >= dTim) //przytrzymano przycisk
        {
            (*ky).ap = 1;
            StartKeyTim(ky);
            return 2; //pierwszy impuls
        }
        if((*ky).ap && KeyTim(ky) >= pTim)
        {
            StartKeyTim(ky); //kolejne impulsy co pTim
            return 2;
        }
    }
    else (*ky).ap = 0;
    return 0;
}

void ExResKey(StPin *ky)
{
    (*ky).bk = 0;
    (*ky).sbk = 0;
    (*ky).ap = 0;
    StartKeyTim(ky);
}
#endif
/*---------------------------------------------------------------------------------*/
#if _EN_EXXPK!=0
//Funkcja detekcji nacisniecia przycisku (wersja b.zaawansowana)
//dla duzych danych liczbowych (>2.5 cyfry)
//k-fizyczne wejscie procesora dla przycisku,
//dTim-czas po jakim przy nacisnietym przycisku nastapi wygenerowanie ciagu impulsów [*0.1sek],
//pTim-czas miedzy kolejnymi impulsami przy spelnieniu warunku dTim [*0.1sek]
//zwraca 0 dla przycisku w stanie spoczynku
//zwraca 1 dla pojedynczego nacisniecia przycisku
//zwraca 2 dla impulsu w trakcie podtrzymania pzrycisku
//zwraca 3 miedzy impulsami w trakcie podtrzymania pzrycisku 
//np. ExxPresKey(RdPrt(S_OFF),&xxky[S_OFF],50,1)

unsigned char ExxPresKey(unsigned char k, volatile StPin *ky, unsigned char dTim, unsigned char pTim)
{
    (*ky).bk = (k) ? 1 : 0;

    if((*ky).fl && (*ky).sbk && !(*ky).bk)
    {
        (*ky).fl = 0;
        (*ky).sbk = ((*ky).bk) ? 1 : 0;
        StartKeyTim(ky);
    }
    if(!(*ky).sbk && (*ky).bk)
    {
        (*ky).sbk = ((*ky).bk) ? 1 : 0;
        StartKeyTim(ky);
        return 1;
    }
    if(!(*ky).fl && (*ky).sbk && !(*ky).bk)
    {
        (*ky).sbk = ((*ky).bk) ? 1 : 0;
        StartKeyTim(ky);
    }
    if((*ky).sbk && (*ky).bk)
    {

        if(!(*ky).ap && KeyTim(ky) >= dTim)
        {
            (*ky).fl = 1;
            (*ky).ap = 1;
        }
        if((*ky).ap && KeyTim(ky) >= pTim)
        {
            StartKeyTim(ky);
            return 2;
        }
    }
    else (*ky).ap = 0;
    (*ky).sbk = ((*ky).bk) ? 1 : 0;
    if((*ky).ap) return 3;
    return 0;
}

void ExxResKey(volatile StPin *ky)
{
    (*ky).bk = 0;
    (*ky).sbk = 0;
    (*ky).ap = 0;
    StartKeyTim(ky);
}
#endif
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
