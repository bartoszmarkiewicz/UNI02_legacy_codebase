/*modulator.c*/
//Algorytm modulacji
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
#include <pic18.h>
#include "global.h"			//parametry globalne
#include "modulator.h"		//parametry lokalne
TWarunek Warunek[5];		//warunki temperaturowe
TkWarunek kWarunek[5];		//warunki temperaturowe z wynikowymi wartosciami parametru k

//--------------------------------------------------------------
//--------------------------------------------------------------
//Tabela wartosci wypelnien PWM odpowiadajacych wartosciom procentowym (co 5%)
//Wartosci podane dla gazu ziemnego i plynnego
unsigned char PobProcToPWM[2][21]={
								0,0,0,0,0x38,0,0x48,0,0x58,0,0,0,0x6D,0,0x78,0,0,0x84,0,0,0xFF,
								0,0,0,0,0x64,0,0x78,0,0x88,0,0,0,0xA0,0,0xB0,0,0,0xC0,0,0,0xFF
								};
//--------------------------------------------------------------
//--------------------------------------------------------------
//(DANE JAK DLA KOTLA INWESTERM)
//Zakres aktywnych nastaw i przeplywow w tabeli pobudzen
//Pierwsze pozycje wierszy tabeli zawieraja slowa aktywacji pol tabeli
//Zawartosc wierszy powinna byc poukladana rosnaco poczynajac od 3 pozycji wiersza
//Drugie pozycje wierszy tabeli (wart. 0) oznaczaja < pozycja 3 wiersza 
unsigned char CWPobNstPrz[2][NG_PNSPR]={	0b00011111,0,40,45,50,55,0,0,0,
									0b00111111,0,4*7,5*7,6*7,7*7,8*7,0,0};
//Tabela pobudzen z wartosciami procentowymi wypelnienia modulatora
unsigned char CWPobNstPrzToProc[NG_NSPR][NG_NSPR]={	0,0,0,30,30,30,0,0,
										   		0,0,0,20,60,60,0,0,
												0,0,30,60,85,85,0,0,
												0,20,60,80,100,100,0,0,
												20,60,60,80,100,100,0,0,
												0,0,0,0,0,0,0,0,
												0,0,0,0,0,0,0,0,
												0,0,0,0,0,0,0,0
												};
//--------------------------------------------------------------
//--------------------------------------------------------------
//Zakres aktywnych nastaw w tabeli pobudzen
//Pierwsze pozycje wierszy tabeli zawieraja slowa aktywacji pol tabeli
//Zawartosc wierszy powinna byc poukladana rosnaco poczynajac od 3 pozycji wiersza
//Drugie pozycje wierszy tabeli (wart. 0) oznaczaja < pozycja 3 wiersza 
unsigned char COPobNst[NG_PNSPR]={0b00011111,0,40,45,50,55,0,0,0};
//Tabela pobudzen z wartosciami procentowymi wypelnienia modulatora
unsigned char COPobNstToProc[NG_NSPR]={0,20,60,80,100,100,0,0};
//--------------------------------------------------------------
//--------------------------------------------------------------
//Rozpoczecie pracy modulatora w obiegu CO
//Dobor wartosci poczatkowych, okreslenie amplitudy pierwszego pobudzenia
void MCOStart(void)
{

}
//Dobor amplitudy pobudzenia po uruchomieniu modulatora
void MCOPPobudzenie(void)
{

}
//Obsluga przejscia modulatora do trybu grzania w obiegu CO
void MCOPowrotDoMCO(void)
{

}
//Decyzja o zmianie trybu pracy modulatora (przejscie miedzy FAZA1/FAZA2)
void MCOManger(void)
{

}
//Wplyw wartosci OpenTerm na maksimum modulacji
void MCOOpenTerm(void)
{

}
//-------------------------------------------------------------
//Sprawdza realizacje warunku w zespole warunkow
//wr - bajt zawierajacy informacje o warunku
//xxt - wielkosc roznicy temperatur
//xxdt - predkosc i kierunek zmian temperatury
unsigned char nWar(const unsigned char wr,const signed char xxt, const signed char xxdt)
{	
	unsigned char wrt=wr&_PARMASK;

	switch(wr&_CHRMASK)
	{
		case _MNIEJSZE:			//mniejszy
		if(xxt<wrt) return 1;
		else return 0;

		case _ROWNE:			//rowny
		if(xxt==wrt) return 1;
		else return 0;

		case _WIEKSZE:			//wiekszy
		if(xxt>wrt) return 1;
		else return 0;

		case _MALEJE:			//maleje /K/sek./
		if(xxdt<wrt) return 1;
		else return 0;

		case _STOI:				//stoi /ile sekund/
		if(xxdt==0) return 1;
		else return 0;

		case _ROSNIE:			//rosnie /K/sek./
		if(xxdt>wrt) return 1;
		else return 0;		
	}
	return 2;	//pomin warunek
}
//-------------------------------------------------------------
//Sprawdza realizacje zespolu warunkow
//nr - numer zespolu warunkow
//xxxt - wielkosc roznicy temperatur
//xxxdt - predkosc i kierunek zmian temperatury
bit SprawdzWarunek(const unsigned char nr,const signed char xxxt,const signed char xxxdt)
{
	unsigned char i,rtw;
	unsigned char wynik=1;		//domyslnie warunek spelniony
	for(i=0;i<_ONEWAR;i++)
	{
		if((rtw=nWar(Warunek[nr].wr[i],xxxt,xxxdt))>1) break;			 //dla wyniku ujemnego przerwanie procedury
		switch(i)
		{
			case 0:
				if(Warunek[nr].rwr&_NEG0) (rtw)?0:1;						//negacja na aktualnym warunku?
				wynik=wynik&&rtw;
			break;
			case 1:
				if(Warunek[nr].rwr&_NEG1) (rtw)?0:1;						//negacja na aktualnym warunku?
				if(Warunek[nr].rwr&_AND0) wynik=wynik&&rtw;	//operacja AND z poprzednim warunkiem? 
				else wynik=wynik||rtw;									//operacja OR z poprzednim warunkiem 
			break;
			case 2:
				if(Warunek[nr].rwr&_NEG2) (rtw)?0:1;						//negacja na aktualnym warunku?
				if(Warunek[nr].rwr&_AND1) wynik=wynik&&rtw;	//operacja AND z poprzednim warunkiem? 
				else wynik=wynik||rtw;
			break;
			case 3:
				if(Warunek[nr].rwr&_NEG3) (rtw)?0:1;;						//negacja na aktualnym warunku?
				if(Warunek[nr].rwr&_AND2) wynik=wynik&&rtw;	//operacja AND z poprzednim warunkiem? 
				else wynik=wynik||rtw;
			break;
		}
	} 
	return wynik!=0;
}
//-------------------------------------------------------------
//Sprawdza realizacje zespolu warunkow i zwraca w zmiennej kp wartosc parametru.
//jezeli zaden warunek nie zostal spelniony zmienna kp nie jest modyfikowana.
//Funkcja zwaraca wartosc 1 jezeli przynajmniej jeden warunek zostal spelniony (kp ulegl modyfikacji)
//nr - numer zespolu par warunkek + zwracana wartosc
//kp - adres zmiennej do ktorej zapisywana jest wartosc parametru przy spelnieniu warunku.
//xxxt - wielkosc roznicy temperatur
//xxxdt - predkosc i kierunek zmian temperatury
bit WybierzWartosc(const unsigned char nr,signed char *kp,const signed char xxxt,const signed char xxxdt)
{
	unsigned char i;
	unsigned char wynik=0;							//domyslnie warunek niespelniony
	for(i=0;i<_ONEkWAR;i++)
	{
		if (nWar(kWarunek[nr].wr[i],xxxt,xxxdt)<2) 	//warunek spelniony?
		{
			*kp=kWarunek[nr].kw[i]; 				//nowa wartosc parametru kp
			wynik=1;								//znacznik ze parametr kp ulegl modyfikacji
		}
	}
	return wynik!=0;
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Funkcja zwraca wartosc pierwszego pobudzenia dla okreslonej nastawy i przeplywu
//nst - aktualna nastawa
//prz - aktualny przeplyw
//tpgz - rodzaj gazu (0-ziemny, 1-plynny)
unsigned char PierwszePobudzenieCW(const unsigned char nst,const unsigned char prz,const unsigned char tpgz)
{
	unsigned char i,ind,inst,iprz;
	ind=0b10000000;
	inst=0;
	if(CWPobNstPrz[wNASTAWA][pMASKA])					//aktywna minimum jedna pozycja?
		for(i=NG_PNSPR-1;i>0;i--)					//poszukiwanie wiersza tabeli		
		{
			if(CWPobNstPrz[wNASTAWA][pMASKA]&ind)		//pozycja aktywna?
				if(nst>=CWPobNstPrz[wNASTAWA][i])
				{		
 					inst=i;
					break;		
				}
			ind=ind>>1;
		}
	if(inst) inst--;
	ind=0b10000000;
	iprz=0;
	if(CWPobNstPrz[wPRZEPLYW][pMASKA])					//aktywna minimum jedna pozycja?
		for(i=NG_PNSPR-1;i>0;i--)					//poszukiwanie kolumny tabeli
		{
			if(CWPobNstPrz[wPRZEPLYW][pMASKA]&ind)		//pozycja aktywna?
				if(prz>=CWPobNstPrz[wPRZEPLYW][i])
				{		
 					iprz=i;
					break;		
				}
			ind=ind>>1;
		}
	if(iprz) iprz--;
	//zwraca wartosc PWM pierwszego pobudzenia dla danego typu gazu
	return PobProcToPWM[(tpgz<2)?tpgz:wPLYNNY][CWPobNstPrzToProc[inst][iprz]/5]; 
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Funkcja zwraca wartosc pierwszego pobudzenia dla okreslonej nastawy i przeplywu
//nst - aktualna nastawa
//tpgz - rodzaj gazu (0-ziemny, 1-plynny)
unsigned char PierwszePobudzenieCO(const unsigned char nst,const unsigned char tpgz)
{
	unsigned char i,ind,inst;
	ind=0b10000000;
	inst=0;
	if(COPobNst[pMASKA])					//aktywna minimum jedna pozycja?
		for(i=NG_PNSPR-1;i>0;i--)					//poszukiwanie wiersza tabeli		
		{
			if(COPobNst[pMASKA]&ind)		//pozycja aktywna?
				if(nst>=COPobNst[i])
				{		
 					inst=i;
					break;		
				}
			ind=ind>>1;
		}
	if(inst) inst--;
	//zwraca wartosc PWM pierwszego pobudzenia dla danego typu gazu
	return PobProcToPWM[(tpgz<2)?tpgz:wPLYNNY][COPobNstToProc[inst]/5]; 
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
