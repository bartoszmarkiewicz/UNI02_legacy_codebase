/*---------------------------------------------------------------------------------*/
/*wsw.c*/
/*obsluga 2 cyfrowego wyswietlacza typu LCD ze znakami graficznymi*/
/*format wyswietlania typu ITALY*/
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
//#include <pic18.h>
#include <xc.h>
#include "global.h"				//parametry globalne
#include "wsw.h"				//parametry lokalne
#include "print.h"
#include "lcdHD2c.h"			//parametry globalne
/*---------------------------------------------------------------------------------*/
//extern unsigned char x;

#if NPWD!=0
volatile WPack PWD[NPWD];				//tablica priorytetow wyswietlania PWD[n] (n=0 - najwyzszy priorytet)
#endif
#if NdPWD!=0
volatile WPack DWD[NdPWD];				//tablica danych modyfikowanych dynamicznie wystepujacych wylacznie w kolejkach
#endif
#if NMZNK!=0
volatile WMark KMark[NMZNK];
#endif
volatile WDyn DynData;					//dynamiczny bufor wyswietlania (aktywny gdy dyn=1)

volatile static WPack bfWswData;			//pamiec danej najnizszego poziomu priorytetow wyswietlania (aktywna gdy only=1)
volatile static WPack WswData;			//dana z informacja o sposobie formatowania
volatile static LedPack LedData;			//dana po formatowaniu bezposrednio wysylana na wyswietlacz

/*---------------------------------------------------------------------------------*/
/*-----------------------------PUBLIC DATA-----------------------------------------*/
void InitLCD(void)
{
	//LCDCON
	
	LP3=0;				//1:1 presaler dla 1kHz zegara LCD
	LP2=0;				//frame clock prescaler
	LP1=0;
	LP0=0;

	//Segment enable registers (rejestr TRIS musi byæ na tych wejsciach jako input).
	SE00=0;
	SE01=0;	
	SE02=1;
	SE03=1;
	SE04=1;
	SE05=1;
	SE06=1;	
	SE07=1;
	SE08=0;
	SE09=0;
	SE10=0;
	SE11=0;
	SE12=0;
	SE13=0;
	SE14=0;
	SE15=0;
	SE16=0;
	SE17=0;
	SE18=0;
	SE19=0;
	SE20=1;
	SE21=1;
	SE22=1;
	SE23=1;
	SE24=1;
	SE25=1;
	SE26=0;
	SE27=0;
	SE28=0;
	SE29=0;
	SE30=0;
	SE31=0;
	SE32=0;

	//LCDCON
	LMUX1=1;			//COM3:COM0 (nie ma potrzeby zmiany wartosci rejestru TRIS)
	LMUX0=1;
	CS1=0;				//system clock source 8MHz
	CS0=0;
	SLPEN=0;			//LCD aktywny takze w stanie sleep

	//LCDDATA			
	//SetWSW(0);		//wyzerowanie stanu wyswietlacza

	//LCDREG
	CKSEL1=0;			//LCD regulator disabled (konfiguracja M2 lub M3)
	CKSEL0=0;
	CPEN=1;				//1charge pump enable bit

	//LCDPS
	WFT=0;				//waweform TYPEA (nieaktywne przerwanie)
	BIASMD=0;			//bias mode select bit

	//PIR3
	LCDIF=0;			//clear interrupt flag

	//IPR3
	LCDIP=0;			//low priority interrupt

	//PIE3
	LCDIE=0;			//enable interrupt

	//LCDCON
	LCDEN=1;			//aktywacja modulu LCD

	//LCDREG
	BIAS2=1;		//100 voltage control output (3,6Vpeak) - regulacja kontrastu
	BIAS1=1;		//0
	BIAS0=1;		//0
	//konfiguracja sprzetowa ukladu kontrastu M2
}
/*---------------------------------------------------------------------------------*/
//Ustawienie wyswietlacza w stan zapalony (wszystkie znaki wlaczone) lub w stan wygaszony (wszystkie znaki wygaszone)
//gdzie dla sall=0 wygaszenie a dla sall=1 zapalenie
void SetWSW(unsigned char sall)
{
	//com1
	LCDDATA0=(sall)?0xff:0;	
	LCDDATA1=(sall)?0xff:0;
	LCDDATA2=(sall)?0xff:0;
	LCDDATA3=(sall)?0xff:0;
	//com2
	LCDDATA6=(sall)?0xff:0;	
	LCDDATA7=(sall)?0xff:0;
	LCDDATA8=(sall)?0xff:0;
	LCDDATA9=(sall)?0xff:0;
	//com3
	LCDDATA12=(sall)?0xff:0;	
	LCDDATA13=(sall)?0xff:0;
	LCDDATA14=(sall)?0xff:0;
	LCDDATA15=(sall)?0xff:0;
	//com4
	LCDDATA18=(sall)?0xff:0;	
	LCDDATA19=(sall)?0xff:0;
	LCDDATA20=(sall)?0xff:0;
	LCDDATA21=(sall)?0xff:0;
}
/*---------------------------------------------------------------------------------*/
void InitWsw(void)
{
	unsigned char i;

	InitLCD();
	for(i=0;i<NPWD-1;i++) PWD[i].St.focus=0;
#if LCD4D!=0
	for(i=0;i<NPWD-1;i++) PWD[i].St2.focus=0;
#endif
#if NMZNK!=0
	for(i=0;i<NMZNK;i++) KMark[i].focus=0;
#endif
	//**********************
	//W tym miejscu nalezy wstepnie skonfigurowac wyswietlacz:
	//- ustawic format danych dla pozycji tablicy PWD nie zajmowanych przez kolejki
	//- ustawic format danych dla poszczegolnych pozycji tablicy DWD
//--------	
	//format zmiennych podstawowych umieszczonych w tablicy priorytetow wyswietlania	
	/*SetStatus(&PWD[3],tDEC,tALL,0,0);					//temp WG (dyzurna)
	SetStatus(&PWD[4],tDEC,tALL,0,0);					//temp WU (dyzurna)	
	SetStatus(&PWD[5],tHEX,tALL,0,0);					//stan OFF
	SetStatus(&PWD[6],tDEC|tKROPKA1,tALL,0,0);			//cisnienie WG
	SetStatus(&PWD[7],tDEC,tALL,0,0);					//temp. WU
	SetStatus(&PWD[8],tDEC,tALL,0,0);					//temp. WG (najnizszy priorytet)*/
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
//nastawa WU/WG
const KAdrPoz KL1_AP[NKL1]=	{
								DWD+2,
								DWD+3
							};
//kolejka procedur awaryjnych
const KAdrPoz KL2_AP[NKL2]=	{
								DWD+4,
								DWD+5
							};
//**********************
//Dane wyswietlane na kolejnych pozycjach kolejek. Dla kazdej pozycji skladaja sie na nie koleno:
//[wartosc starszej cyfry(ignorowane gdy dec=1 lub hex=1),wartosc wyswietlana (dla dec=1 lub hex=1)
//lub wartosc mlodszej cyfry (dla dec=0 oraz hex=0),czas jaki ma byc wyswietlana pozycja]
//UWAGA: Jezeli ta sama pozycja w tablicy KAdrPoz ma wartosc rozna od 0 brany jest pod uwage tylko ostatni
//parametr czasu przez jaki ma byc wyswietlana pozycja (reszta odczytywana jest pod adresem podanym w tablicy KAdrPoz).
#if RSPADR!=0
//-------------------------------
#if LCD4D==0
//*kolejka kodow awaryjnych						
const KWrtPoz KL0_WP[NKL0]={
							0,0,8,	//6			
							0,0,8
							};
//nastawa WU/WG							
const KWrtPoz KL1_WP[NKL1]={
							0,0,6,
							0,0,6
							};
//kolejka procedur awaryjnych							
const KWrtPoz KL2_WP[NKL2]={
							0,0,20,
							0,0,30
							};
#else
//*kolejka kodow awaryjnych						
const KWrtPoz KL0_WP[NKL0]={
							0,0, 0,0, 8,	//6			
							0,0, 0,0, 8
							};
//nastawa WU/WG							
const KWrtPoz KL1_WP[NKL1]={
							0,0, 0,0, 6,
							0,0, 0,0, 6
							};
//kolejka procedur awaryjnych							
const KWrtPoz KL2_WP[NKL2]={
							0,0, 0,0, 20,
							0,0, 0,0, 30
							};
#endif
//----------------------------------
#else
//*kolejka kodow awaryjnych						
const KWrtPoz KL0_WP[NKL0]={
							8,		
							8
							};
//nastawa WU/WG							
const KWrtPoz KL1_WP[NKL1]={
							6,
							6
							};
//kolejka procedur awaryjnych							
const KWrtPoz KL2_WP[NKL2]={
							15,
							15
							};
//----------------------------------
#endif
//**********************
//Format danych na kolejnych pozycjach kolejki. Na format pozycji skladaja sie kolejno:
//[*,dec,hex,autoOFF,kropka0,kropka1,wygas0,wygas1]
//UWAGA: Jezeli ta sama pozycja w tablicy KAdrPoz ma wartosc rozna od 0 powyzsze parametry 
//nie sa brane pod uwage (odczytywane sa pod adresem podanym w tablicy KAdrPoz)
#if RSPADR!=0
#if LCD4D==0
//*kod bledu
const KFrmPoz KL0_FP[NKL0]={
							0,0,0,0,0,0,0,0,0,0,0,0,0,		//nie brany pod uwage - dana umieszczona w DWD
							0,0,0,0,0,0,0,0,0,0,0,0,0
							};
//*nastawa WU/WG
const KFrmPoz KL1_FP[NKL1]={
							0,0,0,0,0,0,0,0,0,0,0,0,0,		//nie brany pod uwage - dana umieszczona w DWD+1
							0,0,0,0,0,0,0,0,0,0,0,0,0
							};
//kolejka procedur awaryjnych
const KFrmPoz KL2_FP[NKL2]={
							0,0,0,0,0,0,0,0,0,0,0,0,0,		//nie brany pod uwage - dana umieszczona w DWD+1
							0,0,0,0,0,0,0,0,0,0,0,0,0
							};
#else
//*kod bledu
const KFrmPoz KL0_FP[NKL0]={
							0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,		//nie brany pod uwage - dana umieszczona w DWD
							0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0
							};
//*nastawa WU/WG
const KFrmPoz KL1_FP[NKL1]={
							0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,		//nie brany pod uwage - dana umieszczona w DWD
							0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0
							};
//kolejka procedur awaryjnych
const KFrmPoz KL2_FP[NKL2]={
							0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,		//nie brany pod uwage - dana umieszczona w DWD
							0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0
							};
#endif
#endif
//**********************
//Dane okreslajace dla kazdej pozycji kolejki czy ma byc wyswietlana

//*kod bledu
//Jezeli zadna pozycja nie jest ustawiona na 1 to kolejka nie jest aktywna
static KFocus KL0_FC[NKL0]=	{
						0,
						0
						};
//nastawa WU/WG
static KFocus KL1_FC[NKL1]=	{
						0,
						0
						};
//kolejka procedur awaryjnych
static KFocus KL2_FC[NKL2]=	{
						0,
						0
						};
//**********************
//Adres pod ktory maja trafiac dane kolejki (najczesciej pozycja tablicy priorytetow wyswietlania PWD),
//,czas ekspozycji kolejki (0 - ekspozycja ciagla),maska rezerwacji diod sygnalizacyjnych dla potrzeb kolejki

const KOutput KL0_OUT=	{PWD,0};	//kolejka kodow bledu trafia pod adres najwyzszego priorytetu, czas ekspozycji - ciagly,
const KOutput KL1_OUT=	{PWD+2,3};	//nastawa wyswietlana przez okreslony czas - priorytet 3, czas ekspozycji 4 sek.,
const KOutput KL2_OUT=	{PWD+3,0};	//kolejka procedur awaryjnych
//**********************														
//Adresy danych skladajacych sie na kolejke 
#if RSPADR!=0
const Kolejka KL0={KL0_AP,KL0_WP,KL0_FP,KL0_FC,&KL0_OUT};
const Kolejka KL1={KL1_AP,KL1_WP,KL1_FP,KL1_FC,&KL1_OUT};
const Kolejka KL2={KL2_AP,KL2_WP,KL2_FP,KL2_FC,&KL2_OUT};
#else
const Kolejka KL0={KL0_AP,KL0_WP,KL0_FC,&KL0_OUT};
const Kolejka KL1={KL1_AP,KL1_WP,KL1_FC,&KL1_OUT};
const Kolejka KL2={KL2_AP,KL2_WP,KL2_FC,&KL2_OUT};
#endif
//***************************************************************************************
//TABLICA OBSLUGI WSZYSTKICH KOLEJEK
//NKOL - liczba kolejek
//**********************
//Podstawowe dane kolejki: adres kolejki, liczba pozycji kolejki
volatile const KGlobal Klj[NKOL]=	{
							&KL0,NKL0,
							&KL1,NKL1,
							&KL2,NKL2
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
volatile static KIndeks	KInd[NKOL];
volatile static KTime	KTim[NKOL];
#endif

//-------------------------------------------------------------------
//Tablica zamiany danej hex/dec na odpowiadajacy jej kod 7LED
#if LTABLE==0
const unsigned char Znak[16]={						//format danych dla wsp. anody
								0b00111111,			//0
								0b00000110,			//1
								0b01011011,			//2
								0b01001111,			//3
								0b01100110,			//4
								0b01101101,			//5
								0b01111101,			//6
								0b00000111,			//7
								0b01111111,			//8
								0b01101111,			//9
								0b01110111,			//A
								0b01111100,			//b
								0b00111001,			//C
								0b01011110,			//d
								0b01111001,			//E
								0b01110001,			//F
								};
#endif
//-------------------------------------------------------------------
//Tablica zamiany danej hex/dec na odpowiadajacy jej kod 7LED
#if LTABLE==1
const unsigned char Znak[16]={						//format danych dla wsp. anody
								0b00111111,			//0
								0b00000110,			//1
								0b01011011,			//2
								0b01001111,			//3
								0b01100110,			//4
								0b01101101,			//5
								0b01111101,			//6
								0b00000111,			//7
								0b01111111,			//8
								0b01101111,			//9
								0b01110111,			//A
								0b01111100,			//b
								0b00111001,			//C
								0b01011110,			//d
								0b01111001,			//E
								0b01110001,			//F
								};
#endif
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//Przygotowanie i wyslanie danej na wyswietlacz
//Funkcja wywolywana w przerwaniu co 10ms.
void RefreshWSW(void)
{
#if NKOL!=0	
	KolProc();					//obsluga kolejki wyswietlania
#endif
#if NMZNK!=0					//osluga znakow graficznych
	KolMark();
#endif
	SetPrWswData();				//pobranie danej PWD[n]i zapis do WswData
	FormatWswData();			//konwersja wartosci WswData na LedData
	PrintWswData();				//wyslanie danej LedData na wyswietlacz
}
/*--------------------------------------------------------------*/
//Proc. obslugi kolejek wyswietlania. Focus w tablicy priorytetow PWD jest przyznawany/odbierany
//automatycznie jezeli przynajmniej jedna pozycja kolejki jest aktywna.
#if NKOL!=0
void KolProc(void)
{
	unsigned char i,j,k,l,nC,kk;
	volatile WPack *cadr,*padr,*padr2;


	for(j=0;j<NKOL;j++)									
	{
		cadr=(*(*Klj[j].KL).KOut).CAdr;

		//reset pozycji kolejki (liczba cykli i pozycje zostaj¹ deaktywowane)
		nC=(*(*Klj[j].KL).KOut).nCykli;
		if((KInd[j].reset)||(nC&&KTim[j].cykl>=nC))						//wylaczenie/minal czas ekspozycji kolejki?
		{																//dla kolejek z ogr. czasem eksp. po uplynieciu czasu deaktywowane sa wszystkie pozycje kol.
			KTim[j].cykl=0;
			KInd[j].reset=0;
			for(i=0;i<Klj[j].nPzc;i++) (*Klj[j].KL).KFc[i].focus=0;		
		}
		//restart kolejki z ograniczonym czasem ekspozycji (liczba cykli zerowana i wszystkie pozycje zostaj¹ aktywowane)
		if((KInd[j].restart||KInd[j].restart1)&&nC)											//start kolejki z okreslonym czasem eksp.?
		{
			KTim[j].cykl=0;
			for(i=0;i<Klj[j].nPzc;i++) (*Klj[j].KL).KFc[i].focus=1;		//dla kolejek z ogr. czasem eksp. przy starcie aktywowane sa wszystkie pozycje kol.
		}
#if LCD4D==0
		//jezeli pozycja jest nieaktywna to jest zerowana 
		if(KInd[j].restart||!((*cadr).St.focus))	//kolejka nieaktywna lub restart kolejki?
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
			if(KInd[j].restart2) KInd[j].indeks=1;
		}
		(*cadr).St.focus=0;						//wstepna deaktywacja calej kolejki w tablicy priorytetow wyswietlania PWD
#else
		if(KInd[j].restart||((!(*cadr).St.focus)&&(!(*cadr).St2.focus)))	//kolejka nieaktywna lub restart kolejki?
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
			if(KInd[j].restart1&&KInd[j].restart2) KInd[j].indeks=1;
		}
		(*cadr).St.focus=0;						//wstepna deaktywacja calej kolejki w tablicy priorytetow wyswietlania PWD
		(*cadr).St2.focus=0;						//wstepna deaktywacja calej kolejki w tablicy priorytetow wyswietlania PWD
#endif
		for(i=KInd[j].indeks;i<Klj[j].nPzc;i++)							//autoaktywacja kolejki nastapi przy aktywnej conajmniej jednej pozycji w kol.
		{
			padr=((*Klj[j].KL).KAP[i]).PAdr;
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
#if LCD4D==0
			//realizacja czasu ekspozycji pojedynczej pozycji kolejki 
			(*cadr).St.focus=1;											//autoaktywacja kolejki - aktywna conajmniej jedna pozycja w kol.
			if (KTim[j].ndS<(*Klj[j].KL).KWP[i].delay)					//jeszcze nie uplynal czas ekspozycji pozycji?
			{
				KTim[j].ncS++;
				if (KTim[j].ncS>=10) 
				{
					KTim[j].ncS=0;
					KTim[j].ndS++;										//odliczaj czas ekspozycji
				}
#if RSPADR!=0
				if(padr==0)													//nie podano adresu zmiennej dynamicznej?
				{
					if(!KInd[j].restart1)								//kolejka z podtrzymaniem pierwszej pozycji na ekranie przy obsludze znakow dla kolejnych pozycji?
					{
						(*cadr).St=(*Klj[j].KL).KFP[i].PFrm;	//zapisz w tablicy PWD pod pozycja odpowiadajaca kolejce dana do wyswietlenia
						(*cadr).Dt=(*Klj[j].KL).KWP[i].PWrt;
					}
					else
					{
						if(!i) KInd[j].restart1=0;	//podtrzymanie znacznika az do aktywacji pierwszej pozycji kolejki
						(*cadr).St=(*Klj[j].KL).KFP[0].PFrm;
						(*cadr).St.sdiode=(*Klj[j].KL).KFP[i].PFrm.sdiode;
						(*cadr).St.rdiode=(*Klj[j].KL).KFP[i].PFrm.rdiode;
						(*cadr).St.sdiode=(*Klj[j].KL).KFP[i].PFrm.sbdid;
						(*cadr).St.rdiode=(*Klj[j].KL).KFP[i].PFrm.rbdid;
						(*cadr).Dt=(*Klj[j].KL).KWP[0].PWrt;
					}
				}
				else 
#endif
				{
					if(!KInd[j].restart1)	//aktywny tryb kolejki wylacznie dla znakow graficznych?
					{
						*cadr=*padr;													//w przeciwnym przypadku pobierz dana z adresu zmiennej dynamicznej
					}
					else
					{
						if(!i) KInd[j].restart1=0;	//podtrzymanie znacznika az do aktywacji pierwszej pozycji kolejki
						*cadr=*((*Klj[j].KL).KAP[0].PAdr);				//dana pobierana zawsze z pierwszej pozycji kolejki
						(*cadr).St.sdiode=(*padr).St.sdiode; //aktyalizacja stanu znaków dla kolejnych pozycji kolejki
						(*cadr).St.rdiode=(*padr).St.rdiode;
						(*cadr).St.sbdid=(*padr).St.sbdid;
						(*cadr).St.rbdid=(*padr).St.rbdid;
					}
				}
				(*cadr).St.focus=1;										//odtworzenie nadpisanego bitu autoaktywacji
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
#else
			//realizacja czasu ekspozycji pojedynczej pozycji kolejki 
			(*cadr).St.focus=1;										//autoaktywacja kolejki - aktywna conajmniej jedna pozycja w kol.
			(*cadr).St2.focus=1;									//autoaktywacja kolejki - aktywna conajmniej jedna pozycja w kol.
			if (KTim[j].ndS<(*Klj[j].KL).KWP[i].delay)					//jeszcze nie uplynal czas ekspozycji pozycji?
			{
				KTim[j].ncS++;
				if (KTim[j].ncS>=10) 
				{
					KTim[j].ncS=0;
					KTim[j].ndS++;										//odliczaj czas ekspozycji
				}
#if RSPADR!=0
				if(padr==0)							//nie podano adresu zmiennej dynamicznej?
				{
					if(!KInd[j].restart1)								//kolejka z podtrzymaniem pierwszej pozycji na ekranie przy obsludze znakow dla kolejnych pozycji?
					{
						(*cadr).St=(*Klj[j].KL).KFP[i].PFrm;	//zapisz w tablicy PWD pod pozycja odpowiadajaca kolejce dana do wyswietlenia
						(*cadr).Dt=(*Klj[j].KL).KWP[i].PWrt;
						(*cadr).St2=(*Klj[j].KL).KFP[i].PFrm2;	//zapisz w tablicy PWD pod pozycja odpowiadajaca kolejce dana do wyswietlenia
						(*cadr).Dt2=(*Klj[j].KL).KWP[i].PWrt2;
					}
					else
					{
						if(!i) KInd[j].restart1=0;	//podtrzymanie znacznika az do aktywacji pierwszej pozycji kolejki
						(*cadr).Dt=(*Klj[j].KL).KWP[0].PWrt;
						(*cadr).St=(*Klj[j].KL).KFP[0].PFrm;
						(*cadr).St.sdiode=(*Klj[j].KL).KFP[i].PFrm.sdiode;
						(*cadr).St.rdiode=(*Klj[j].KL).KFP[i].PFrm.rdiode;
						(*cadr).St.sdiode=(*Klj[j].KL).KFP[i].PFrm.sbdid;
						(*cadr).St.rdiode=(*Klj[j].KL).KFP[i].PFrm.rbdid;
						(*cadr).Dt2=(*Klj[j].KL).KWP[0].PWrt2;
						(*cadr).St2=(*Klj[j].KL).KFP[0].PFrm2;
						(*cadr).St2.sdiode=(*Klj[j].KL).KFP[i].PFrm2.sdiode;
						(*cadr).St2.rdiode=(*Klj[j].KL).KFP[i].PFrm2.rdiode;
						(*cadr).St2.sdiode=(*Klj[j].KL).KFP[i].PFrm2.sbdid;
						(*cadr).St2.rdiode=(*Klj[j].KL).KFP[i].PFrm2.rbdid;
					}
				}
				else
#endif
				{
					if(!KInd[j].restart1)		//aktywny tryb kolejki wylacznie dla znakow graficznych?
					{
						*cadr=*padr;
					}
					else
					{
						if(!i) {KInd[j].restart1=0;KInd[j].restart2=0;}	//podtrzymanie znacznika az do aktywacji pierwszej pozycji kolejki
						*cadr=*((*Klj[j].KL).KAP[0].PAdr);				//dana pobierana zawsze z pierwszej pozycji kolejki
						if(!KInd[j].restart2)							//zacznij od drugiej pozycji?
						{	
							kk=i;
						}
						else
						{
							kk=1;
						}
						padr2=((*Klj[j].KL).KAP[kk]).PAdr;
						(*cadr).St.sdiode=(*padr2).St.sdiode; //aktyalizacja stanu znaków dla kolejnych pozycji kolejki
						(*cadr).St.rdiode=(*padr2).St.rdiode;
						(*cadr).St.sbdid=(*padr2).St.sbdid;
						(*cadr).St.rbdid=(*padr2).St.rbdid;
						(*cadr).St2.sdiode=(*padr2).St2.sdiode; //aktyalizacja stanu znaków dla kolejnych pozycji kolejki
						(*cadr).St2.rdiode=(*padr2).St2.rdiode;
						(*cadr).St2.sbdid=(*padr2).St2.sbdid;
						(*cadr).St2.rbdid=(*padr2).St2.rbdid;	
					}
				}
				(*cadr).St.focus=1;										//odtworzenie nadpisanego bitu autoaktywacji
				(*cadr).St2.focus=1;									//odtworzenie nadpisanego bitu autoaktywacji
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
#endif
		}
	}	
}
#endif
/*--------------------------------------------------------------*/
//Aktywacja/deaktywacja obslugi kolejnych znakow graficznych (KMark[i].focus) ktorym nadajemy okreslony stan
// wartoscia KMark[i].m lub wymuszamy migotanie wartoscia KMark[i].p=1;
#if NMZNK!=0
void KolMark(void)
{
	static unsigned char c,d,fl;
	unsigned char i;

	//odlicz interwal czasu
	c++;
	if (c>=10) 			//odliczaj czas
	{
		c=0;
		d++;			 
	}
	if(d>=6)			//odlicz interwal pulsacji	
	{
		d=0;
 		if(fl) fl=0;	//ustaw znacznik wygaszenia
		else fl=1;		//ustaw znacznik zapalenia
	}
	//dla aktywnej pulsacji wysteruj/wygas znak
	for(i=0;i<NMZNK;i++)
	{
		if(KMark[i].focus)	//aktywna pozycja kolejki?
		{
			if(KMark[i].p)	//aktywny znacznik pulsacji?
			{
				//przejmij sterowanie znakiem graficznym
				if(fl) KMark[i].m=1;	//wysteruj znak	
				else KMark[i].m=0;		//wygas znak
			}
		}
	}
}
#endif
/*--------------------------------------------------------------*/
//Pobranie danej do wyswietlenia (poczynajac od najwyzszego priorytetu pierwsza
//z ustawiona flaga focus).
//Kolejno 0 - najwyzszy priorytet,...,(NPWD-1) - najnizszy priorytet
void SetPrWswData(void)
{
	unsigned char i;

#if LCD4D!=0
	for(i=0;i<NPWD-1;i++)
	{
		if(PWD[i].St.focus||PWD[i].St2.focus)				//ustawiona flaga focus?
		{
			WswData=PWD[i];
			return;
		};
	}
#else
	for(i=0;i<NPWD-1;i++)
	{
		if(PWD[i].St.focus)				//ustawiona flaga focus?
		{
			WswData=PWD[i];
			return;
		};
	}
#endif
	WswData=PWD[NPWD-1];									//wyswietl dana o najnizszym priorytecie
	bfWswData=PWD[NPWD-1];
}
/*--------------------------------------------------------------*/
void SetLedData(WData Dt,WStatus St,volatile LedPack *LedData,const unsigned char gr)
{
	volatile unsigned char *dLed1,*dLed0;
#if LCD4D!=0
	dLed1=(!gr)?&(*LedData).dLed1:&(*LedData).dLed3;
	dLed0=(!gr)?&(*LedData).dLed0:&(*LedData).dLed2;
#else
	dLed1=&(*LedData).dLed1;
	dLed0=&(*LedData).dLed0;
#endif
	if(St.hex)										//konwersja hex na kod led
	{
		*dLed1=Znak[(Dt.Data0&0xF0)>>4];
		*dLed0=Znak[Dt.Data0&0x0F];
	}else
	if(St.dec)										//konwersja dec na kod led
	{
		if(Dt.Data0>99) Dt.Data0=99;
		*dLed1=Znak[Dt.Data0/10];		
		*dLed0=Znak[Dt.Data0%10];
	}else
	{
		*dLed1=Dt.Data1;						//bezposrednio dane w kodzie LED	
		*dLed0=Dt.Data0;
	}
	if (St.autoOFF&&(*dLed1==Znak[0]))	//auto wygaszenie starszej cyfry gdy == 0 
								*dLed1=BLACK; 	
	if (St.kropka0) 								//zapalenie mlodszej kropki
								*dLed0=*dLed0|KROPKA;
	if (St.kropka1) 								//zapalenie starszej kropki
								*dLed1=*dLed1|KROPKA;
	if (St.wygas0||(*LedData).setOff)					//wygaszenie mlodszej cyfry	
								*dLed0=BLACK;
	if (St.wygas1||(*LedData).setOff)					//wygaszenie starszej cyfry 	
								*dLed1=BLACK;

	//Sterowanie z poziomu kolejki znakami graficznymi (do 8 znakow z priorytetem wyswietlania)
	//gdzie: 
	//St.sdiode - znacznik informujacy ze beda przez pozycje kolejki zapalane znaki graficzne okreslone w St.sbdid (do 8 znakow)
	//St.rdiode - znacznik informujacy ze beda przez pozycje kolejki wygaszane znaki graficzne okreslone w St.rbdid (do 8 znakow)
	if (St.sdiode&&St.sbdid)
	{
			(*LedData).mask|=(0x00ff&St.sbdid);		//aktywuj obsluge znaku			
			(*LedData).mark|=(0x00ff&St.sbdid);		//zapal wybrane znaki
	}
	if (St.rdiode&&St.rbdid)
	{
			(*LedData).mask|=(0x00ff&St.rbdid);		//aktywuj obsluge znaku
			(*LedData).mark&=~(0x00ff&St.rbdid);	//wygas wybrane znaki
	}
}
/*--------------------------------------------------------------*/
//Przygotowanie danej do wyswietlenia
void FormatWswData(void)
{
	unsigned char i;
	unsigned int msk;
//-----------------------------------------------------
	//Sterowanie globalne znakami graficznymi (do 16 znakow - do pierwszych 8 znakow moze pokryc kolejka ktora ma priorytet)
	//gdzie: LedData.mask okresla ktore znaki brane sa pod uwage a LedData.mark zawiera informacje o oczekiwanym stanie znaku
	#if NMZNK!=0
		msk=0b0000000000000001;
		for(i=0;i<NMZNK;i++)
		{
			if(KMark[i].focus) {LedData.mask|=msk;}			//ustaw maske dla znakow graficznych
			else LedData.mask&=~msk;
			if(KMark[i].m) LedData.mark|=msk;				//zapal wygas znaki graficzne
			else LedData.mark&=~msk;
			msk=msk<<1;				
		}
	#endif
#if LCD4D!=0
//-----------------------------------------------------
//dwie mlodsze cyfry
	if(WswData.St2.dyn) SetLedData(DynData.Dt,DynData.St,&LedData,0);		//najmlodsze pobierz z bufora dynamicznego
	else 
	if(WswData.St2.only) SetLedData(bfWswData.Dt,bfWswData.St,&LedData,0);	//najmlodsze pobierz z najnizszego poziomu wyswietlania
	else SetLedData(WswData.Dt,WswData.St,&LedData,0);
//-----------------------------------------------------
//dwie starsze cyfry
	if(WswData.St.dyn) SetLedData(DynData.Dt,DynData.St,&LedData,1);		//najstarsze pobierz z bufora dynamicznego
	else 
	if(WswData.St.only) SetLedData(bfWswData.Dt2,bfWswData.St2,&LedData,1);	//najstarsze pobierz z najnizszego poziomu wyswietlania
	else SetLedData(WswData.Dt2,WswData.St2,&LedData,1);

	//if(!WswData.St.dyn) SetLedData(WswData.Dt2,WswData.St2,&LedData,1);
	//else SetLedData(DynData.Dt,DynData.St,&LedData,1);
#else
//-----------------------------------------------------
//dwie mlodsze cyfry
	SetLedData(WswData.Dt,WswData.St,&LedData,0); 
#endif
}
/*--------------------------------------------------------------*/
//Wyslanie danej na wyswietlacz
void PrintWswData(void)
{
	//----------------------------------------------------------------
	//CYFRY

	//pierwsza cyfra
	_LCD_1A=(LedData.dLed0&0b00000001)?1:0;
	_LCD_1B=(LedData.dLed0&0b00000010)?1:0;
	_LCD_1C=(LedData.dLed0&0b00000100)?1:0;
	_LCD_1D=(LedData.dLed0&0b00001000)?1:0;
	_LCD_1E=(LedData.dLed0&0b00010000)?1:0;
	_LCD_1F=(LedData.dLed0&0b00100000)?1:0;
	_LCD_1G=(LedData.dLed0&0b01000000)?1:0;

	//druga cyfra
	_LCD_2A=(LedData.dLed1&0b00000001)?1:0;
	_LCD_2B=(LedData.dLed1&0b00000010)?1:0;
	_LCD_2C=(LedData.dLed1&0b00000100)?1:0;
	_LCD_2D=(LedData.dLed1&0b00001000)?1:0;
	_LCD_2E=(LedData.dLed1&0b00010000)?1:0;
	_LCD_2F=(LedData.dLed1&0b00100000)?1:0;
	_LCD_2G=(LedData.dLed1&0b01000000)?1:0;
#if LCD4D!=0
	//trzecia cyfra
	_LCD_3A=(LedData.dLed2&0b00000001)?1:0;
	_LCD_3B=(LedData.dLed2&0b00000010)?1:0;
	_LCD_3C=(LedData.dLed2&0b00000100)?1:0;
	_LCD_3D=(LedData.dLed2&0b00001000)?1:0;
	_LCD_3E=(LedData.dLed2&0b00010000)?1:0;
	_LCD_3F=(LedData.dLed2&0b00100000)?1:0;
	_LCD_3G=(LedData.dLed2&0b01000000)?1:0;

	//czwarta cyfra
	_LCD_4A=(LedData.dLed3&0b00000001)?1:0;
	_LCD_4B=(LedData.dLed3&0b00000010)?1:0;
	_LCD_4C=(LedData.dLed3&0b00000100)?1:0;
	_LCD_4D=(LedData.dLed3&0b00001000)?1:0;
	_LCD_4E=(LedData.dLed3&0b00010000)?1:0;
	_LCD_4F=(LedData.dLed3&0b00100000)?1:0;
	_LCD_4G=(LedData.dLed3&0b01000000)?1:0;
#endif
	//kropka
	_LCD_P1=(LedData.dLed1&0b10000000)?1:0;

	//----------------------------------------------------------------
	//ZNAKI GRAFICZNE

	//znaki graficzne wyzwalane globalnie lub z poziomu kolejki
	//wyzwolenie z poziomu kolejki ma priorytet (przeslania wyzwalanie globalne)
	//pierwsze 8 znakow moze byc wyzwolonych z priorytetem kolejki (0b0000000011111111)
	//wszystkie znaki mozna wyzwolic globalnie (ostatnie osiem 8 wylacznie ta metoda)

	//Pierwsze 8 znakow (od 0b0000000000000001 do 0b0000000010000000)
	if(LedData.mask&0b0000000000000001) _LCD_KAL=(LedData.mark&0b0000000000000001)?1:0;		//0- znak kaloryfer
	if(LedData.mask&0b0000000000000010) _LCD_KRN=(LedData.mark&0b0000000000000010)?1:0;		//1- znak kran
	if(LedData.mask&0b0000000000000100) _LCD_SRV=(LedData.mark&0b0000000000000100)?1:0;		//2- znak serwis
	if(LedData.mask&0b0000000000001000) _LCD_RES=(LedData.mark&0b0000000000001000)?1:0;		//3- znak reset
	if(LedData.mask&0b0000000000010000) _LCD_TCO=(LedData.mark&0b0000000000010000)?1:0;		//4- znak C dla co
	if(LedData.mask&0b0000000000100000) _LCD_TCW=(LedData.mark&0b0000000000100000)?1:0;		//5- znak C dla cw
	if(LedData.mask&0b0000000001000000) _LCD_BAR=(LedData.mark&0b0000000001000000)?1:0;		//6- znak bar
	if(LedData.mask&0b0000000010000000) _LCD_PGD=(LedData.mark&0b0000000010000000)?1:0;		//7- znak pogoda
	//Drugie 8 znakow (od 0b0000000100000000 do 0b1000000000000000)
	if(LedData.mask&0b0000000100000000) _LCD_PLC=(LedData.mark&0b0000000100000000)?1:0;		//8- znak plomien co_LCD_PLC
	if(LedData.mask&0b0000001000000000) _LCD_PLW=(LedData.mark&0b0000001000000000)?1:0;		//9- znak plomien cw
	if(LedData.mask&0b0000010000000000) _LCD_OFF=(LedData.mark&0b0000010000000000)?1:0;		//10- znak off
	if(LedData.mask&0b0000100000000000) _LCD_MXW=(LedData.mark&0b0000100000000000)?1:0;		//11- znak maxcw
	if(LedData.mask&0b0001000000000000) _LCD_MNW=(LedData.mark&0b0001000000000000)?1:0;		//12- znak mincw
	if(LedData.mask&0b0010000000000000) _LCD_MXC=(LedData.mark&0b0010000000000000)?1:0;		//13- znak maxco
	if(LedData.mask&0b0100000000000000) _LCD_MNC=(LedData.mark&0b0100000000000000)?1:0;		//14- znak minco
	//-------------------------
	//Stale elementy graficzne zawsze wysterowane/wygaszone
	_LCD_KRS=1;		//znak kreski stale zapalony
}
/*--------------------------------------------------------------*/
/*--------------------------------------------------------------*/
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
void SetData(volatile WData *Dt,const unsigned char wdt1,const unsigned char wdt0)
{
	(*Dt).Data1=wdt1;
	(*Dt).Data0=wdt0;
}
/*--------------------------------------------------------------*/
//Okreslenie formatu danej pod wskazanym adresem (typ WPack)
//Dla danej:
//Pod uwage brane sa tylko pozycje okreslone w danej mask,z tych
//ustawiane sa pozycje okreslone w danej wst pozostale sa zerowane.
//Dla znaku graficznego:
//zapalane sa pozycje okreslone w danej sbdid (zapala dla sdiode=1)
//wygaszane sa pozycje okreslone w danej rbdid (wygasza dla rdiode=1)
//np. SetStatus(&PWD[_WS_TCW],tDEC|tSDID|tFOCUS,tALL,_KZ_TMP_C,0); //wskaz temp. ze znakim graf C
void SetStatus(volatile WStatus *St,const unsigned int wst,const unsigned int mask,const unsigned char sbdid, const unsigned char rbdid)
{
	if (mask&0b1000000000000000)(*St).focus=(wst&0b1000000000000000)?1:0;
	if (mask&0b0100000000000000)(*St).dec=(wst&0b0100000000000000)?1:0;
	if (mask&0b0010000000000000)(*St).hex=(wst&0b0010000000000000)?1:0;
	if (mask&0b0001000000000000)(*St).autoOFF=(wst&0b0001000000000000)?1:0;
	if (mask&0b0000100000000000)(*St).kropka0=(wst&0b0000100000000000)?1:0;
	if (mask&0b0000010000000000)(*St).kropka1=(wst&0b0000010000000000)?1:0;
	if (mask&0b0000001000000000)(*St).wygas0=(wst&0b0000001000000000)?1:0;
	if (mask&0b0000000100000000)(*St).wygas1=(wst&0b0000000100000000)?1:0;
	if (mask&0b0000000010000000)(*St).led0=(wst&0b0000000010000000)?1:0;
	if (mask&0b0000000001000000)(*St).led1=(wst&0b0000000001000000)?1:0;
	if (mask&0b0000000000100000)(*St).sdiode=(wst&0b0000000000100000)?1:0;	//czy razem z dana ma byc wyswietlany wybrany znak graficzny?
	if (mask&0b0000000000010000)(*St).rdiode=(wst&0b0000000000010000)?1:0;	//czy razem z dana ma byc wygaszany wybrany znak graficzny?
	if (mask&0b0000000000001000)(*St).dyn=(wst&0b0000000000001000)?1:0;		//dana ma byc zaladowana na pozostale 2 cyfry z bufora wspolnego?
	if (mask&0b0000000000000100)(*St).only=(wst&0b0000000000000100)?1:0;		//dana ma byc zaladowana na pozostale 2 cyfry z bufora wspolnego?
	//jezeli aktywne jest polecenie zapalenia/wygaszenia znaku graficznego i slowo wskazujace znaku jest niezerowe
	if(((*St).sdiode)&&sbdid)
	{
		(*St).sbdid=sbdid;				//bierze pod uwage tylko wybrane pozycje (zapala dla sdiode=1 gasi dla rdiode=1)
	}
	if(((*St).rdiode)&&rbdid)
	{
		(*St).rbdid=rbdid;				//bierze pod uwage tylko wybrane pozycje (zapala dla sdiode=1 gasi dla rdiode=1)
	}
}
/*--------------------------------------------------------------*/
//Zapalenie wybranego symbolu graficznego (uwaga - aktywny wskaz kolejki ma priorytet wyswietlania)
//gdzie focus oresla aktywnosc znaku, pulse wlacza jego pulsacje a gdy pulse=0 stan znaku okresla wrt.
//np. SetMark(&KMark[_GZ_DCO],1,1,1);
#if NMZNK!=0
void SetMark(volatile WMark *wData,const unsigned char focus,const unsigned char pulse, const unsigned char wrt)
{
	(*wData).focus=(focus)?1:0;
	(*wData).p=(pulse)?1:0;
	(*wData).m=(wrt)?1:0;
}
//-----------------------------------------------------
//Wygaszenie wszystkich symboli na wyswietlaczu (uwaga - aktywny wskaz kolejki ma (jak zwykle) priorytet wyswietlania)
void ClrAllMarks(void)
{
	unsigned char i;
	for(i=0;i<NMZNK;i++)
	{
		KMark[i].focus=1;
		KMark[i].p=0;
		KMark[i].m=0;			
	}
}
#endif
//-----------------------------------------------------
//-----------------------------------------------------
#if NKOL!=0
//Zwraca stan pozycji nrpoz w kolejce nrkol
//bit GetPoz(const unsigned char nrkol,const unsigned char nrpoz)
//{
//	if ((nrkol>=NKOL)||(nrpoz>=Klj[nrkol].nPzc)) asm("RESET");			//obsluga sytuacji wyjatkowej
//	return (*Klj[nrkol].KL).KFc[nrpoz].focus; 
//}
unsigned char GetPoz(const unsigned char nrkol,const unsigned char nrpoz)       //mmm
{
	if ((nrkol>=NKOL)||(nrpoz>=Klj[nrkol].nPzc)) asm("RESET");			//obsluga sytuacji wyjatkowej
	return (((*Klj[nrkol].KL).KFc[nrpoz].focus)&&(KInd[nrkol].indeks==nrpoz)?1:0); 
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
//Start/Restart kolejki nr nrkol (znaki graficzne pracuj¹ niezaleznie)
//(dla kolejek z ogr. czasem eksp. przy starcie aktywowane sa wszystkie pozycje kol.)
void RestartKol1(const unsigned char nrkol)
{
	if (nrkol>=NKOL) asm("RESET");	//obsluga sytuacji wyjatkowej
	KInd[nrkol].restart1=1;
}
//Start/Restart kolejki nr nrkol (znaki graficzne pracuja niezaleznie, rozpoczyna od drugiej pozycji kolejki)
//(dla kolejek z ogr. czasem eksp. przy starcie aktywowane sa wszystkie pozycje kol.)
void RestartKol2(const unsigned char nrkol)
{
	if (nrkol>=NKOL) asm("RESET");	//obsluga sytuacji wyjatkowej
	KInd[nrkol].restart1=1;
	KInd[nrkol].restart2=1;
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
