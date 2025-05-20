/*---------------------------------------------------------------------------------*/
/*wsw.c*/
/*obsluga 2 cyfrowego wyswietlacza typu LED z fizyczn¹ lini¹ MUX*/
/*(format TSTR)*/
/*
UWAGA:
1.Do modyfikacji danych wyswietlanych na kolejnych pozycjach tablicy priorytetow
wyswietlania, nalezy stosowac funkcje:
-SetData(...
-SetStatus(...
Funkcja SetStatus() daje ponadto mozliwosc modyfikacji stanu diod sygnalizacyjnych
automatycznie przez aktywn¹ pozycje tablicy priorytetow wyswietlania.
2.Aby zmienic stan diod sygnalizacyjnych niezaleznie od tablicy priorytetow wyswietlania
nalezy bezwzglednie stosowac funkcje:
-Swsdid(...
-Rwsdid(...
3.Z dowolna pozycja tablicy priorytetow wyswietlania zwiazana moze byc kolejka wyswietlania.
Kolejka wyswietlania steruje sie za pomoc¹ funkcji:
-GetPos(...
-SetPoz(...
-ResetPoz(...
-ResetKol(...
-RestartKol(...
-AllKolToFirstPoz(...
*/
/*---------------------------------------------------------------------------------*/
#include <xc.h>
#include "../../proj.tp.h"

#ifndef _TYP_PRJ
#error NIEOKRESLONY IDENTYFIKATOR PROJEKTU
#endif

#if _TYP_PRJ==3
#advisory _TYP_PRJ==3
#include "../../mcc_generated_files\mcc.h"

#include "wsw2.h"				//parametry lokalne
#include "global.ex.h"
/*---------------------------------------------------------------------------------*/

extern unsigned char x;

#if NPWD!=0
WPack PWD[NPWD];				//tablica priorytetow wyswietlania PWD[n] (n=0 - najwyzszy priorytet)
#endif
#if NdPWD!=0
WPack DWD[NdPWD];				//tablica danych modyfikowanych dynamicznie wystepujacych wylacznie w kolejkach
#endif

static WPack WswData;			//dana z informacja o sposobie formatowania
static LedPack LedData;			//dana po formatowaniu bezposrednio wysylana na wyswietlacz

#if SER_WSW!=0 					//aktywny interfejs szeregowy?
static bit mux;					//znacznik aktywnej cyfry
static unsigned char wbty[2];	//bajty rejestrow szeregowych wyswietlacza
static unsigned char wsdid;		//slowo wysterowania kolenych diod sygnalizacyjnych
static unsigned char Dmask;		//maska rezerwacji diod sygnalizacyjnych dla potrzeb kolejki
#endif
/*---------------------------------------------------------------------------------*/
/*-----------------------------PUBLIC DATA-----------------------------------------*/
void InitDtaWsw(void)
{
	unsigned char i;

	#if SER_WSW!=0		//aktywny interfejs szeregowy?
		_DT_SetLow();
		_CL_SetLow();
		_ST_SetLow();
		#if WKATODA==0
		wbty[0]=0xff;
		wbty[1]=0xff;
		wsdid=0x00;
		#else
		wsdid=0x00;
		wbty[0]=0x00;
		wbty[1]=0x00;
		wsdid=0x0f;
		#endif
	#endif

	for(i=0;i<NPWD-1;i++) PWD[i].St.focus=0;
	//**********************
	//W tym miejscu nalezy wstepnie skonfigurowac wyswietlacz:
	//- ustawic format danych dla pozycji tablicy PWD nie zajmowanych przez kolejki
	//- ustawic format danych dla poszczegolnych pozycji tablicy DWD
//--------	
	//format zmiennych podstawowych umieszczonych w tablicy priorytetow wyswietlania	
//********
	//SetStatus(&PWD[3],tDEC,tALL);					//temp WG (dyzurna)
	//SetStatus(&PWD[4],tDEC,tALL);					//temp WU (dyzurna)	
	//SetStatus(&PWD[5],tHEX,tALL);					//stan OFF
	//SetStatus(&PWD[6],tDEC|tKROPKA1,tALL);			//cisnienie WG
	//SetStatus(&PWD[7],tDEC,tALL);					//temp. WU
	//SetStatus(&PWD[8],tDEC,tALL);					//temp. WG (najnizszy priorytet)
//********
	SetStatus(&PWD[4],tHEX,tALL);					//stan OFF
	SetStatus(&PWD[5],tDEC,tALL);					//temp. WU
	//w pozostalych umieszczone sa kolejki wyswietlania.... 
//--------
	//zmienne dynamiczne wyst. wylacznie w kolejkach
	//format zmiennych

}
//-------------------------------------------------------------------
//UWAGA: DALSZA CZESC DOTYCZY WYLACZNIE KOLEJEK WYSWIETLANIA						
//***************************************************************************************
#if NKOL!=0
//PODSTAWOWE DANE KOLEJEK
//***************************************************************************************
//Kolejka KL0:
//Tablica wskaznikow do danych na kolejnych pozycjach kolejek ktore maja zostac wyswietlone
//Jezeli wskaznik = 0 dana pozycji pobierana jest z tablicy /KWrtPoz/ a jej format okreslony
//jest w tablicy /KFrmPoz/.

//kod awaryjny
const KAdrPoz KL0_AP[NKL0]=	{
								DWD,			
								DWD+1
							};
//wartosci parametrow
const KAdrPoz KL1_AP[NKL1]=	{
								DWD+2,			
								DWD+2
							};
//nastawa WU/WG
const KAdrPoz KL2_AP[NKL2]=	{
								DWD+3,
								DWD+4
							};
//kolejka procedur awaryjnych
const KAdrPoz KL3_AP[NKL3]=	{
								DWD+5,
								DWD+6
							};
//**********************
//Dane wyswietlane na kolejnych pozycjach kolejek. Dla kazdej pozycji skladaja sie na nie koleno:
//[wartosc starszej cyfry(ignorowane gdy dec=1 lub hex=1),wartosc wyswietlana (dla dec=1 lub hex=1)
//lub wartosc mlodszej cyfry (dla dec=0 oraz hex=0),czas jaki ma byc wyswietlana pozycja]
//UWAGA: Jezeli ta sama pozycja w tablicy KAdrPoz ma wartosc rozna od 0 brany jest pod uwage tylko ostatni
//parametr czasu przez jaki ma byc wyswietlana pozycja (reszta odczytywana jest pod adresem podanym w tablicy KAdrPoz).
//****************************
/*
//kolejka kodow awaryjnych						
const KWrtPoz KL0_WP[NKL0]={
							0,0,8,	//6			
							0,0,8
							};
//wartosci parametrow
const KWrtPoz KL1_WP[NKL1]={
							0,0,6,				
							0,0,6
							};
//nastawa WU/WG							
const KWrtPoz KL2_WP[NKL2]={
							0,0,6,
							0,0,6
							};
//kolejka procedur awaryjnych							
const KWrtPoz KL3_WP[NKL3]={
							0,0,30,
							0,0,20
							};
*/
//***************************
//*kolejka kodow awaryjnych						
const KWrtPoz KL0_WP[NKL0]={
							0,0,6,				
							0,0,6
							};
//wartosci parametrow
const KWrtPoz KL1_WP[NKL1]={
							0,0,6,				
							0,0,6
							};
//nastawa WU/WG							
const KWrtPoz KL2_WP[NKL2]={
							0,0,6,
							0,0,6
							};
//kolejka procedur awaryjnych							
const KWrtPoz KL3_WP[NKL3]={
							0,0,6,
							0,0,6
							};

//**********************
//Format danych na kolejnych pozycjach kolejki. Na format pozycji skladaja sie kolejno:
//[*,dec,hex,autoOFF,kropka0,kropka1,wygas0,wygas1]
//UWAGA: Jezeli ta sama pozycja w tablicy KAdrPoz ma wartosc rozna od 0 powyzsze parametry 
//nie sa brane pod uwage (odczytywane sa pod adresem podanym w tablicy KAdrPoz)

//*kod bledu
/*
const KFrmPoz KL0_FP[NKL0]={
							0,0,0,0,0,0,0,0,0,0,0,0,0,		//nie brany pod uwage - dana umieszczona w DWD
							0,0,0,0,0,0,0,0,0,0,0,0,0
							};
//wartosci parametrow
const KFrmPoz KL1_FP[NKL1]={
							0,0,0,0,0,0,0,0,0,0,0,0,0,		//nie brany pod uwage - dana umieszczona w DWD
							0,0,0,0,0,0,0,0,0,0,0,0,0
							};
//nastawa WU/WG
const KFrmPoz KL2_FP[NKL2]={
							0,0,0,0,0,0,0,0,0,0,0,0,0,		//nie brany pod uwage - dana umieszczona w DWD+1
							0,0,0,0,0,0,0,0,0,0,0,0,0
							};
//kolejka procedur awaryjnych
const KFrmPoz KL3_FP[NKL3]={
							0,0,0,0,0,0,0,0,0,0,0,0,0,		//nie brany pod uwage - dana umieszczona w DWD+1
							0,0,0,0,0,0,0,0,0,0,0,0,0
							};
*/
const KFrmPoz KL0_FP[NKL0];
//wartosci parametrow
const KFrmPoz KL1_FP[NKL1];
//*nastawa WU/WG
const KFrmPoz KL2_FP[NKL2];
//kolejka procedur awaryjnych
const KFrmPoz KL3_FP[NKL3];
//**********************
//Dane okreslajace dla kazdej pozycji kolejki czy ma byc wyswietlana

//*kod bledu
//Jezeli zadna pozycja nie jest ustawiona na 1 to kolejka nie jest aktywna
static KFocus KL0_FC[NKL0]=	{
						0,
						0
						};
//wartosci parametrow
static KFocus KL1_FC[NKL1]=	{
						0,
						0
						};
//nastawa WU/WG
static KFocus KL2_FC[NKL2]=	{
						0,
						0
						};
//kolejka procedur awaryjnych
static KFocus KL3_FC[NKL3]=	{
						0,
						0
						};
//**********************
//Adres pod ktory maja trafiac dane kolejki (najczesciej pozycja tablicy priorytetow wyswietlania PWD),
//,czas ekspozycji kolejki (0 - ekspozycja ciagla),maska rezerwacji diod sygnalizacyjnych dla potrzeb kolejki

const KOutput KL0_OUT=	{PWD,0,0};		//kolejka kodow bledu trafia pod adres najwyzszego priorytetu, czas ekspozycji - ciagly,
const KOutput KL1_OUT=	{PWD+1,0,0};	//kolejka wartosci parametru trafia pod adres priorytetu 2, czas ekspozycji - ciagly,
const KOutput KL2_OUT=	{PWD+2,4,0};	//nastawa wyswietlana przez okreslony czas - priorytet 3, czas ekspozycji 4 sek.,
const KOutput KL3_OUT=	{PWD+3,0,0};	//kolejka procedur awaryjnych
//**********************														
//Adresy danych skladajacych sie na kolejke 

const Kolejka KL0={KL0_AP,KL0_WP,KL0_FP,KL0_FC,&KL0_OUT};
const Kolejka KL1={KL1_AP,KL1_WP,KL1_FP,KL1_FC,&KL1_OUT};
const Kolejka KL2={KL2_AP,KL2_WP,KL2_FP,KL2_FC,&KL2_OUT};
const Kolejka KL3={KL3_AP,KL3_WP,KL3_FP,KL3_FC,&KL3_OUT};
//***************************************************************************************
//TABLICA OBSLUGI WSZYSTKICH KOLEJEK
//NKOL - liczba kolejek
//**********************
//Podstawowe dane kolejki: adres kolejki, liczba pozycji kolejki
const KGlobal Klj[NKOL]=	{
							&KL0,NKL0,
							&KL1,NKL1,
							&KL2,NKL2,
							&KL3,NKL3
							};
#endif
//-------------------------------------------------------------------
//-------------------------------------------------------------------							
//PROCEDURY PUBLICZNE
//-------------------------------------------------------------------
/*Bezwarunkowe wygaszenie wyswietlacza*/
//void WswOff(void)

/*Wyjscie z bezwarunkowego wygaszenia wyswietlacza*/
//void WswOn(void)

/*Wyprowadzenie danej na wyswietlacz*/
//void SetData(WPack *wData,unsigned char wdt1, unsigned char wdt0);

/*Okreslenie formatu danej wysylanej na wyswietlacz*/ 							
//void SetStatus(WPack *wData,unsigned char wst,unsigned char mask);

/*Aktywacja pozycji kolejki*/							
//void SetPoz(unsigned char nrkol,unsigned char nrpoz);

/*Deaktywacja pozycji kolejki*/
//void ResetPoz(unsigned char nrkol,unsigned char nrpoz);

/*Deaktuwacja wszystkich pozycji kolejki*/
//void ResetKol(unsigned char nrkol);

/*Restart kolejki*/
//void RestartKol(unsigned char nrkol);

							
/*---------------------------------------------------------------------------------*/
/*-----------------------------PROTECTED DATA--------------------------------------*/
#if NKOL!=0
static KIndeks	KInd[NKOL];
static KTime	KTim[NKOL];
#endif
//-------------------------------------------------------------------
//Tablica zamiany danej hex/dec na odpowiadajacy jej kod 7LED
#if LTABLE==0
const unsigned char Znak[16]={						//format danych dla wsp. anody
								0b11000000,			//0
								0b11111001,			//1
								0b10100100,			//2
								0b10110000,			//3
								0b10011001,			//4
								0b10010010,			//5
								0b10000010,			//6
								0b11111000,			//7
								0b10000000,			//8
								0b10010000,			//9
								0b10001000,			//A
								0b10000011,			//b
								0b11000110,			//C
								0b10100001,			//d
								0b10000110,			//E
								0b10001110,			//F
								};
#endif
//-------------------------------------------------------------------
//Tablica zamiany danej hex/dec na odpowiadajacy jej kod 7LED
#if LTABLE==1
const unsigned char Znak[16]={						//format danych dla wsp. anody
								0b01010000,			//0
								0b11111001,			//1
								0b00110010,			//2
								0b00110001,			//3
								0b10011001,			//4
								0b00010101,			//5
								0b00010100,			//6
								0b01111001,			//7
								0b00010000,			//8
								0b00010001,			//9
								0b00011000,			//A
								0b10010100,			//b
								0b01010110,			//C
								0b10110000,			//d
								0b00010110,			//E
								0b00011110,			//F
								};
#endif
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//Przygotowanie i wyslanie danej na wyswietlacz
//Funkcja wywolywana w przerwaniu co 10ms.
void inline RefreshWSW(void)
{
#if NKOL!=0	
	KolProc();					//obsluga kolejki wyswietlania
#endif
	SetPrWswData();				//pobranie danej PWD[n]i zapis do WswData
	FormatWswData();			//konwersja wartosci WswData na LedData
	PrintWswData();				//wyslanie danej LedData na wyswietlacz
}
/*--------------------------------------------------------------*/
//Proc. obslugi kolejek wyswietlania. Focus w tablicy priorytetow PWD jest przyznawany/odbierany
//automatycznie jezeli przynajmniej jedna pozycja kolejki jest aktywna.
#if NKOL!=0
void inline KolProc(void)
{
	unsigned char i,j,k,l,nC;
	
	for(j=0;j<NKOL;j++)									
	{
		nC=(*(*Klj[j].KL).KOut).nCykli;
		if((KInd[j].reset)||(nC&&KTim[j].cykl>=nC))						//wylaczenie/minal czas ekspozycji kolejki?
		{																//dla kolejek z ogr. czasem eksp. po uplynieciu czasu deaktywowane sa wszystkie pozycje kol.
			KTim[j].cykl=0;
			KInd[j].reset=0;
			for(i=0;i<Klj[j].nPzc;i++) (*Klj[j].KL).KFc[i].focus=0;		
		}
		if(KInd[j].restart&&nC)											//start kolejki z okreslonym czasem eksp.?
		{
			KTim[j].cykl=0;
			for(i=0;i<Klj[j].nPzc;i++) (*Klj[j].KL).KFc[i].focus=1;		//dla kolejek z ogr. czasem eksp. przy starcie aktywowane sa wszystkie pozycje kol.
		}
		if(KInd[j].restart||!((*(*(*Klj[j].KL).KOut).CAdr).St.focus))	//kolejka nieaktywna lub restart kolejki?
		{
			KInd[j].restart=0;
			#if RSNFRS!=0
			if(!nC)
			{
				KInd[j].indeks=0;											
				KTim[j].ncS=0;
				KTim[j].ndS=0;
			}
			#else
				KInd[j].indeks=0;											
				KTim[j].ncS=0;
				KTim[j].ndS=0;
			#endif
		}
		(*(*(*Klj[j].KL).KOut).CAdr).St.focus=0;						//wstepna deaktywacja calej kolejki w tablicy priorytetow wyswietlania PWD
		for(i=KInd[j].indeks;i<Klj[j].nPzc;i++)							//autoaktywacja kolejki nastapi przy aktywnej conajmniej jednej pozycji w kol.
		{
			//poszukuj kolejke o aktywnej przynajmniej jednej pozycji
			if (!(*Klj[j].KL).KFc[i].focus)								//pozycja nieaktywna?
			{
				if(i>=Klj[j].nPzc-1)
				{
					KInd[j].indeks=0;
					if(nC) KTim[j].cykl++;
				}
				continue;					
			}
			//znaleziono kolejke o aktywnej przynajmniej jednej pozycji
			KInd[j].indeks=i;											//zapamietaj nr aktualnie wyswietlanej pozycji
			#if DKLPNC!=0
			//deaktywuj wszystkie pozycje kolejek o nizszym priorytecie i ograniczonym czasie ekspozycji kolejki
			for(k=j+1;k<NKOL;k++)										
				if((*(*Klj[k].KL).KOut).nCykli)							//okreslony czas ekspozycji?
				{
					for(l=0;l<Klj[k].nPzc;l++) (*Klj[k].KL).KFc[l].focus=0;		//wylacz wszystkie pozycje kolejki
					KTim[k].cykl=0;
				}
			#endif
			#if RSAKLP!=0
			//zatrzymanie wszystkich kolejek (o nizszym priorytecie od obecnie wyswietlanej) na pierwszej pozycji (autorestart)
			for(k=j+1;k<NKOL;k++)
			{
				KInd[k].indeks=0;											
				KTim[k].ncS=0;
				KTim[k].ndS=0;
			}
			#endif
			//realizacja czasu ekspozycji pojedynczej pozycji kolejki 
			(*(*(*Klj[j].KL).KOut).CAdr).St.focus=1;					//autoaktywacja kolejki - aktywna conajmniej jedna pozycja w kol.
			#if SER_WSW!=0
			Dmask=(*(*Klj[j].KL).KOut).DMask;
			#endif
			if (KTim[j].ndS<(*Klj[j].KL).KWP[i].delay)					//jeszcze nie uplynal czas ekspozycji pozycji?
			{
				KTim[j].ncS++;
				if (KTim[j].ncS>=10) 
				{
					KTim[j].ncS=0;
					KTim[j].ndS++;										//odliczaj czas ekspozycji
				}
				if((*Klj[j].KL).KAP[i].PAdr==0)							//nie podano adresu zmiennej dynamicznej?
				{
					(*(*(*Klj[j].KL).KOut).CAdr).St=(*Klj[j].KL).KFP[i].PFrm;	//zapisz w tablicy PWD pod pozycja odpowiadajaca kolejce dana do wyswietlenia
					(*(*(*Klj[j].KL).KOut).CAdr).Dt=(*Klj[j].KL).KWP[i].PWrt;
				}else (*(*(*Klj[j].KL).KOut).CAdr)=*((*Klj[j].KL).KAP[i].PAdr);	//w przeciwnym przypadku pobierz dana z adresu zmiennej dynamicznej
				(*(*(*Klj[j].KL).KOut).CAdr).St.focus=1;				//odtworzenie nadpisanego bitu autoaktywacji
				break;
			}else
			{
				KTim[j].ncS=0;
				KTim[j].ndS=0;											//wyzeruj czas ekspozycji
				if (KInd[j].indeks>=Klj[j].nPzc-1) 
				{
					KInd[j].indeks=0;
					if(nC) KTim[j].cykl++;
				}
			}
		}
	}	
}
#endif
/*--------------------------------------------------------------*/
//Pobranie danej do wyswietlenia (poczynajac od najwyzszego priorytetu pierwsza
//z ustawiona flaga focus).
//Kolejno 0 - najwyzszy priorytet,...,(NPWD-1) - najnizszy priorytet
void inline SetPrWswData(void)
{
	unsigned char i;
	for(i=0;i<NPWD-1;i++)
	{
		if(PWD[i].St.focus)									//ustawiona flaga focus?
		{
			WswData=PWD[i];
			return;
		};
	}
	WswData=PWD[NPWD-1];									//wyswietl dana o najnizszym priorytecie
}
/*--------------------------------------------------------------*/
//zapala wybrana diode
void inline InSwbty(unsigned char dtd)
{
	unsigned char mask;
    
	dtd=dtd&0x0f;
	mask=~(wsdid&dtd);

	#if WKATODA==0
		//wspolna anoda
		wbty[_NRDIODE]|=0x0f;
		wbty[_NRDIODE]&=~(dtd|(wsdid&mask));
	#else
		//wspolna katoda
		wbty[_NRDIODE]&=0xf0;
		wbty[_NRDIODE]&=~((~dtd)&(wsdid&mask));
	#endif
}
/*--------------------------------------------------------------*/
//wygasza wybrana diode
void inline InRwbty(unsigned char dtd)
{
	unsigned char mask;

	dtd=dtd&0x0f;
	mask=~(wsdid&dtd);
	#if WKATODA==0
		//wspolna anoda
		wbty[_NRDIODE]|=0x0f;
		wbty[_NRDIODE]&=~((~dtd)&(wsdid&mask));
	#else
		//wspolna katoda
		wbty[_NRDIODE]&=0xf0;
		wbty[_NRDIODE]&=~(dtd|(wsdid&mask));
	#endif
}
/*--------------------------------------------------------------*/
//Przygotowanie danej do wyswietlenia
void inline FormatWswData(void)
{
	unsigned char nrbty;

	if(WswData.St.hex)										//konwersja hex na kod led
	{
		LedData.dLed1=Znak[(WswData.Dt.Data0&0xF0)>>4];
		LedData.dLed0=Znak[WswData.Dt.Data0&0x0F];
	}else
	if(WswData.St.dec)										//konwersja dec na kod led
	{
		if(WswData.Dt.Data0>99) WswData.Dt.Data0=99;
		LedData.dLed1=Znak[WswData.Dt.Data0/10];		
		LedData.dLed0=Znak[WswData.Dt.Data0%10];
	}else
	{
		LedData.dLed1=WswData.Dt.Data1;						//bezposrednio dane w kodzie LED	
		LedData.dLed0=WswData.Dt.Data0;
	}
	if (WswData.St.autoOFF&&(LedData.dLed1==Znak[0]))	//auto wygaszenie starszej cyfry gdy == 0 
								LedData.dLed1=BLACK; 	
	if (WswData.St.kropka0) 								//zapalenie mlodszej kropki
								LedData.dLed0=LedData.dLed0&KROPKA;
	if (WswData.St.kropka1) 								//zapalenie starszej kropki
								LedData.dLed1=LedData.dLed1&KROPKA;
	if (WswData.St.wygas0||LedData.setOff)					//wygaszenie mlodszej cyfry	
								LedData.dLed0=BLACK;
	if (WswData.St.wygas1||LedData.setOff)					//wygaszenie starszej cyfry 	
								LedData.dLed1=BLACK;
#if SER_WSW!=0
	if (LedData.setOff)		//wygasic wyswietlacz?
	{
		#if WKATODA==0
		//wspolna anoda
		wbty[_NRDIODE]|=0x0f;
		#else
		//wspolna katoda
		wbty[_NRDIODE]&=0xf0;
		#endif	
	}
	else
	if (WswData.St.wbdid&&(WswData.St.sdiode||WswData.St.rdiode))
	{
		//zastosuj ogolne slowo wysterowania diod
		//zmodyfikowane informacja zapisana w wybranym poziomie priorytetu wyswietlania
		if (WswData.St.sdiode)
        {
            InSwbty(WswData.St.wbdid);	//zapal wybrane diody
        }
		else
		if (WswData.St.rdiode)
        {
            InRwbty(WswData.St.wbdid);	//wygas wybrane diody
        }
	}
	else	
	{
		//zastosuj ogolne slowo wysterowania diod
		#if WKATODA==0
			//wspolna anoda
			wbty[_NRDIODE]|=0x0f;
			wbty[_NRDIODE]&=~(wsdid);
		#else
			//wspolna katoda
			wbty[_NRDIODE]&=0xf0;
			wbty[_NRDIODE]|=wsdid;
		#endif
	}
#endif
}
/*--------------------------------------------------------------*/
//Wyslanie danej na wyswietlacz
void inline PrintWswData(void)
{
    volatile unsigned char dta;
    
#if SER_WSW!=0								//aktywny interfejs szeregowy wyswietlacza?
	unsigned char i,j,k;
#endif
#if WKATODA!=0								//wspolna katoda?
	LedData.dLed0=~LedData.dLed0;
	LedData.dLed1=~LedData.dLed1;
#endif
#if SER_WSW==0								//aktywny interfejs szeregowy wyswietlacza?
//Interfejs rownolegly
	#if WSWAP==0							//zamienic cyfry miejscami?
		if (MUX)
			PORT_WSW=LedData.dLed0;
		else
			PORT_WSW=LedData.dLed1;
	#else
		if (MUX) 
			PORT_WSW=LedData.dLed1;
		else
			PORT_WSW=LedData.dLed0;
	#endif
#else
//Interfejs szeregowy (wysylanie bajtow do rejestrow bit po bicie.)(mux z wyjscia procesora)
	mux=!mux;

	#if WSWAP==0							//zamienic cyfry miejscami?
		if (mux)
			wbty[0]=LedData.dLed0;
		else
			wbty[0]=LedData.dLed1;
	#else
		if (mux) 
			wbty[0]=LedData.dLed1;
		else
			wbty[0]=LedData.dLed0;
	#endif
	for(k=0;k<NWBTY;k++)
	{
		for(j=0;j<NRBIT;j++)
		{
			dta=(wbty[NWBTY-1-k]&(1<<(NRBIT-1-j)))?1:0;
            if(dta) _DT_SetHigh();
            else _DT_SetLow();
			//bit wyslany - ustaw sygnal zegara
			for(i=0;i<RTIMW;i++);		//zwloka
			_CL_SetHigh();
			for(i=0;i<RTIMW;i++);		//zwloka
			_CL_SetLow();
		}
	}
	//wszystkie bajty wyslane - ustaw sygnal strobe
	for(i=0;i<RTIMW;i++);		//zwloka
	_ST_SetHigh();
	//_MUX=(mux)?1:0;				//zmiana stanu wyjscia MUX
    if(mux) 
    {
        _KY3_SetHigh();    
    }
    else
    {
        _KY3_SetLow();        
    }
	for(i=0;i<RTIMW;i++);		//zwloka
	_ST_SetLow();
#endif		 						
}
/*--------------------------------------------------------------*/
/*--------------------------------------------------------------*/
#if SER_WSW!=0
//Ustawia/zeruje dowolny bit rejestru wyswietlacza
//nrbty - numer bajtu rejastru wyswietlacza
//nrbit - numer bitu w bajcie (nrbty) rejestru wyswietlacza
//bt - stan jaki ma przyjac bit
void SetWbtyBit(unsigned char nrbty,  unsigned char nrbit, const unsigned char bt)
{
	if(nrbty>=NWBTY) nrbty=NWBTY-1;
	if(bt) wbty[nrbty]|=1<<nrbit;
	else wbty[nrbty]&=~(1<<nrbit);	
}
/*--------------------------------------------------------------*/
//zapala wybrana diode
void Swbty(unsigned char dtd)
{
	unsigned char mask;
	dtd=dtd&0x0f;
	mask=~(wsdid&dtd);

	#if WKATODA==0
		//wspolna anoda
		wbty[_NRDIODE]|=0x0f;
		wbty[_NRDIODE]&=~(dtd|(wsdid&mask));
	#else
		//wspolna katoda
		wbty[_NRDIODE]&=0xf0;
		wbty[_NRDIODE]&=~((~dtd)&(wsdid&mask));
	#endif
}
/*--------------------------------------------------------------*/
//wygasza wybrana diode
void Rwbty(unsigned char dtd)
{
	unsigned char mask;

	dtd=dtd&0x0f;
	mask=~(wsdid&dtd);
	#if WKATODA==0
		//wspolna anoda
		wbty[_NRDIODE]|=0x0f;
		wbty[_NRDIODE]&=~((~dtd)&(wsdid&mask));
	#else
		//wspolna katoda
		wbty[_NRDIODE]&=0xf0;
		wbty[_NRDIODE]&=~(dtd|(wsdid&mask));
	#endif
}
/*--------------------------------------------------------------*/
//ustawia bit globalnego slowa wysterowania diod
void Swsdid(unsigned char dtd)
{
	dtd=dtd&0x0f;
	wsdid|=dtd;
}
/*--------------------------------------------------------------*/
//zeruje bit globalnego slowa wysterowania diod
void Rwsdid(unsigned char dtd)
{
	dtd=dtd&0x0f;
	wsdid&=~(dtd);
}
#endif
/*--------------------------------------------------------------*/
/*--------------------------------------------------------------*/
//Bezwarunkowe wygaszenie wyswietlacza
void WswOff(void)
{
	LedData.setOff=1;
}
//Wyjscie z bezwarunkowego wygaszenia wyswietlacza
void WswOn(void)
{
	LedData.setOff=0;
}
/*--------------------------------------------------------------*/
/*--------------------------------------------------------------*/
//Wprowadzenie danej pod wskazany adres (typ WPack)
//np. SetData(&PWD[2],0,35);
void SetData(WPack *wData,const unsigned char wdt1,const unsigned char wdt0)
{
	(*wData).Dt.Data1=wdt1;
	(*wData).Dt.Data0=wdt0;
}
//Okreslenie formatu danej pod wskazanym adresem (typ WPack)
//Pod uwage brane sa tylko pozycje okreslone w danej mask,z tych
//ustawiane sa pozycje okreslone w danej wst pozostale sa zerowane.
//np. SetStatus(&PWD[2],DEC,ALL);
void SetStatus(WPack *wData,const unsigned int wst,const unsigned int mask)
{
	if (mask&0b1000000000000000)(*wData).St.focus=(wst&0b1000000000000000)?1:0;
	if (mask&0b0100000000000000)(*wData).St.dec=(wst&0b0100000000000000)?1:0;
	if (mask&0b0010000000000000)(*wData).St.hex=(wst&0b0010000000000000)?1:0;
	if (mask&0b0001000000000000)(*wData).St.autoOFF=(wst&0b0001000000000000)?1:0;
	if (mask&0b0000100000000000)(*wData).St.kropka0=(wst&0b0000100000000000)?1:0;
	if (mask&0b0000010000000000)(*wData).St.kropka1=(wst&0b0000010000000000)?1:0;
	if (mask&0b0000001000000000)(*wData).St.wygas0=(wst&0b0000001000000000)?1:0;
	if (mask&0b0000000100000000)(*wData).St.wygas1=(wst&0b0000000100000000)?1:0;
	if (mask&0b0000000010000000)(*wData).St.led0=(wst&0b0000000010000000)?1:0;
	if (mask&0b0000000001000000)(*wData).St.led1=(wst&0b0000000001000000)?1:0;
	if (mask&0b0000000000100000)(*wData).St.sdiode=(wst&0b0000000000100000)?1:0;
	if (mask&0b0000000000010000)(*wData).St.rdiode=(wst&0b0000000000010000)?1:0;
	//jezeli aktywne jest polecenie zapalenia/wygaszenia diody i slowo wskazujace diody jest niezerowe
	if (mask&0b0000000000001111)
	if(((*wData).St.sdiode||(*wData).St.rdiode)&&(wst&0b0000000000001111))
	{
		(*wData).St.wbdid=(wst&0b0000000000001111);
	}
}
//-----------------------------------------------------
//-----------------------------------------------------
#if NKOL!=0
//Zwraca stan pozycji nrpoz w kolejce nrkol
bit GetPoz(const unsigned char nrkol,const unsigned char nrpoz)
{
	if ((nrkol>=NKOL)||(nrpoz>=Klj[nrkol].nPzc)) asm("RESET");			//obsluga sytuacji wyjatkowej
	return (*Klj[nrkol].KL).KFc[nrpoz].focus; 
}
//Aktywacja pozycji nrpoz w kolejce nrkol
void SetPoz(const unsigned char nrkol,const unsigned char nrpoz)
{
	if ((nrkol>=NKOL)||(nrpoz>=Klj[nrkol].nPzc)) asm("RESET");			//obsluga sytuacji wyjatkowej
	(*Klj[nrkol].KL).KFc[nrpoz].focus=1; 
	KInd[nrkol].reset=0;			//jezeli wczesniej ustawiono flage resetu kolejki - nalezy ja wyzerowac
}
//Deaktywacja pozycji nrpoz w kolejce nrkol
void ResetPoz(const unsigned char nrkol,const unsigned char nrpoz)
{
	if ((nrkol>=NKOL)||(nrpoz>=Klj[nrkol].nPzc)) asm("RESET");			//obsluga sytuacji wyjatkowej
	(*Klj[nrkol].KL).KFc[nrpoz].focus=0; 
}
//Deaktywacja wszystkich pozycji kolejki nr nrkol
void ResetKol(const unsigned char nrkol)
{
	if (nrkol>=NKOL) asm("RESET");	//obsluga sytuacji wyjatkowej
	KInd[nrkol].reset=1;
}
//Start/Restart kolejki nr nrkol
//(dla kolejek z ogr. czasem eksp. przy starcie aktywowane sa wszystkie pozycje kol.)
void RestartKol(const unsigned char nrkol)
{
	if (nrkol>=NKOL) asm("RESET");	//obsluga sytuacji wyjatkowej
	KInd[nrkol].restart=1;
}
//Ustawia wszystkie kolejki na pierwsze pozycje
void AllKolToFirstPoz(void)
{
	unsigned char j;
	for(j=0;j<NKOL;j++)
	{
		KInd[j].indeks=0;											
		KTim[j].ncS=0;
		KTim[j].ndS=0;
	}	
}
#endif
//-----------------------------------------------------
//-----------------------------------------------------
#endif