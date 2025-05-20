/*imy.c*/
/*Procedury zwiazane z transmisja danych do ukladu IMY-xx*/

#include <pic18.h>
#include "global.h"			//parametry globalne
#include "imy.h"			//parametry lokalne
#include "i2c_m.h"
#include "mmy.h"
#include "saa.h"			
 

tpMMY DtMMY;
static tpIMY DtIMY;
static bit mk1;
static bit lk1;
static bit mk0;
static bit lk0;
static const unsigned char KodLed[17]={
							0b11000000,		//0
							0b11111001,		//1
							0b10100100,		//2
							0b10110000,		//3
							0b10011001,		//4
							0b10010010,		//5
							0b10000010,		//6
							0b11111000,		//7
							0b10000000,		//8
							0b10010000,		//9
							0b10001000,		//A
							0b10000011,		//B
							0b11000110,		//C
							0b10100001,		//D
							0b10000110,		//E
							0b10001110,		//F
							0b11111111		//BLANC
						};
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//Inicjalizacja podstawowych danych
void InitIMY(void)
{
	DtIMY.bFls=0;
	DtIMY.timIMY=0;	
	#if (SYGTRN!=0)							
		LGRN=!SYGGRN;
		LRED=!SYGRED; 						
	#endif
}
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------	
//Timer na potrzeby pulsacji wskazu wyswietlacza (umiescic w przerwaniu od zegara TMR1)
void IncIMYTimer(void)
{
	if(DtIMY.timIMY<255) DtIMY.timIMY++;
}
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//Wyznaczanie odcinkow czasowych pulsacji wskazu wyswietlacza
void IMYTimer(void)
{
	if(DtIMY.timIMY>=_IMYPULSE)
	{
		DtIMY.timIMY=0;	
		DtIMY.bFls=~DtIMY.bFls;
	}
}
//Start zegara regulujacego odcinki czasowe miedzy transmisjami
void StartIMYTim(void)
{
	DtIMY.timIMY=0;
	DtIMY.bFls=0;	
}
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//konwersja danej typu dec na kod wyswietlacza
void DecTo7Led(unsigned char data,unsigned char *C1, unsigned char *C0)
{
	if(data>99) data=99;
	*C1=~KodLed[data/10];
	*C0=~KodLed[data%10];
}
//konwersja danej typu hex na kod wyswietlacza
void HexTo7Led(unsigned char data,unsigned char *C1, unsigned char *C0)
{
	*C1=~KodLed[data/16];
	*C0=~KodLed[data%16];
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//Konfiguracja i zapis danych do ukladu SAA0 
void NewDataToSAA0(void)
{
	unsigned char hb1,lb1,hb0,lb0;
	//----------------------------------
	//Obróbka danych do wyswietlenia
	DecTo7Led(DtIMY.CO,&hb1,&lb1);						//konwersja danej na format wyswietlacza
	DecTo7Led(DtIMY.PCO,&hb0,&lb0);						//konwersja danej na format wyswietlacza
	//----------------------------------
	//Priorytetowe parametry wyswietlania
	//starsze dwie cyfry
	if(DtIMY.Status0&_KRS1) 							//wyswietlic dwie kreski?
	{	
		lb1=_KRESKA;
		hb1=_KRESKA;
	}
	if(DtIMY.Status0&_FLS1&&DtIMY.bFls)
	{
		lb1=_BLANK;
		hb1=_BLANK;
	}
	if((DtIMY.Status0&_CLS1)||(DtIMY.Status0&_CLS)) 	//wygasiæ wuswietlacz?
	{	
		lb1=_BLANK;
		hb1=_BLANK;
	}
	//mlodsze dwie cyfry
	if(DtIMY.Status0&_KRS0) 							//wyswietlic dwie kreski?
	{	
		lb0=_KRESKA;
		hb0=_KRESKA;
	}
	if(DtIMY.Status0&_FLS0&&DtIMY.bFls)
	{
		lb0=_BLANK;
		hb0=_BLANK;
	}
	if((DtIMY.Status0&_CLS0)||(DtIMY.Status0&_CLS))	//wygasiæ wuswietlacz? 
	{	
		lb0=_BLANK;
		hb0=_BLANK;
	}
	//wszystkie cyfry
	if(DtIMY.Status0&_ADDI) DtIMY.Config0|=_ADD_6mA;	//zwiêkszyc prad wyswietlacza?
	//----------------------------------
	//Zapis danych w buforze wyswietlacza 
	SetDataBufI2C(_ADR_SAA,_SAA_ADR0);					//slave adres
	SetDataBufI2C(_SADR_SAA, 0x00);						//poczatkowa wartosc subadresu (autoinkrementacja)
	SetDataBufI2C(_CONTROL_SAA,DtIMY.Config0);
	SetDataBufI2C(_DIGIT1_SAA,lb0);
	SetDataBufI2C(_DIGIT2_SAA,hb0);
	SetDataBufI2C(_DIGIT3_SAA,lb1);
	SetDataBufI2C(_DIGIT4_SAA,hb1);
}
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//Konfiguracja i zapis danych do ukladu SAA1
void NewDataToSAA1(void)
{
	unsigned char hb1,lb1,hb0,lb0;
	//----------------------------------
	//Obróbka danych do wyswietlenia
	DecTo7Led(DtIMY.CW,&hb1,&lb1);						//konwersja danej na format wyswietlacza
	DecTo7Led(DtIMY.PCW,&hb0,&lb0);						//konwersja danej na format wyswietlacza
	//----------------------------------
	//Priorytetowe parametry wyswietlania
	//starsze dwie cyfry
	if(DtIMY.Status1&_KRS1) 							//wyswietlic dwie kreski?
	{	
		lb1=_KRESKA;
		hb1=_KRESKA;
	}
	if(DtIMY.Status1&_FLS1&&DtIMY.bFls)
	{
		lb1=_BLANK;
		hb1=_BLANK;
	}
	if((DtIMY.Status1&_CLS1)||(DtIMY.Status1&_CLS)) 	//wygasiæ wuswietlacz?
	{	
		lb1=_BLANK;
		hb1=_BLANK;
	}
	//mlodsze dwie cyfry
	if(DtIMY.Status1&_KRS0) 							//wyswietlic dwie kreski?
	{	
		lb0=_KRESKA;
		hb0=_KRESKA;
	}
	if(DtIMY.Status1&_FLS0&&DtIMY.bFls)
	{
		lb0=_BLANK;
		hb0=_BLANK;
	}
	if((DtIMY.Status1&_CLS0)||(DtIMY.Status1&_CLS))	//wygasiæ wuswietlacz? 
	{	
		lb0=_BLANK;
		hb0=_BLANK;
	}
	//wszystkie cyfry
	if(DtIMY.Status1&_ADDI) DtIMY.Config1|=_ADD_6mA;	//zwiêkszyc prad wyswietlacza?
	//----------------------------------
	//Zapis danych w buforze wyswietlacza 
	SetDataBufI2C(_ADR_SAA,_SAA_ADR1);					//slave adres
	SetDataBufI2C(_SADR_SAA, 0x00);						//poczatkowa wartosc subadresu (autoinkrementacja)
	SetDataBufI2C(_CONTROL_SAA,DtIMY.Config1);
	SetDataBufI2C(_DIGIT1_SAA,lb0);
	SetDataBufI2C(_DIGIT2_SAA,hb0);
	if(!(DtIMY.Status1&_CLS)) BitSetBufI2C(_DIGIT1_SAA,_KROPKA);					//zapal kropke dla wody WU
	SetDataBufI2C(_DIGIT3_SAA,lb1);
	SetDataBufI2C(_DIGIT4_SAA,hb1);
	if(!(DtIMY.Status1&_CLS)) BitSetBufI2C(_DIGIT3_SAA,_KROPKA);					//zapal kropke dla wody WU
}
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//Konfiguracja i zapis danych do ukladu SAA2
void NewDataToSAA2(void)
{
	unsigned char hb1,lb1,hb0,lb0;
	unsigned char p,c;
	//----------------------------------
	//Obróbka danych do wyswietlenia
	//przeplyw WU
	p=DtIMY.TPRZ_CW/8;
	c=DtIMY.TPRZ_CW%8;
	
	if (p>=10)
	{
		mk1=0;
	 	DecTo7Led(p,&hb1,&lb1);
		if(c>=5) lk1=1;
		else lk1=0;
	}
	else
	{
		lk1=0;
		mk1=1;
		DecTo7Led((DtIMY.TPRZ_CW*10)/8,&hb1,&lb1);
	}
	//cisnienie WG	
	p=DtIMY.CS/10;
	c=DtIMY.CS%10;
	
	if (p>=10)
	{
		mk0=0;
	 	DecTo7Led(p,&hb0,&lb0);
		if(c>=5) lk0=1;
		else lk0=0;
	}
	else
	{
		lk0=0;
		mk0=1;
		DecTo7Led((DtIMY.CS*10)/10,&hb0,&lb0);
	}	
	//----------------------------------
	//Priorytetowe parametry wyswietlania
	//starsze dwie cyfry
	if(DtIMY.Status2&_KRS1) 							//wyswietlic dwie kreski?
	{	
		lb1=_KRESKA;
		hb1=_KRESKA;
	}
	if(DtIMY.Status2&_FLS1&&DtIMY.bFls)
	{
		lb1=_BLANK;
		hb1=_BLANK;
	}
	if((DtIMY.Status2&_CLS1)||(DtIMY.Status2&_CLS)) 	//wygasiæ wuswietlacz?
	{	
		lb1=_BLANK;
		hb1=_BLANK;
	}
	//mlodsze dwie cyfry
	if(DtIMY.Status2&_KRS0) 							//wyswietlic dwie kreski?
	{	
		lb0=_KRESKA;
		hb0=_KRESKA;
	}
	if(DtIMY.Status2&_FLS0&&DtIMY.bFls)
	{
		lb0=_BLANK;
		hb0=_BLANK;
	}
	if((DtIMY.Status2&_CLS0)||(DtIMY.Status2&_CLS))		//wygasiæ wuswietlacz? 
	{	
		lb0=_BLANK;
		hb0=_BLANK;
	}
	//wszystkie cyfry
	if(DtIMY.Status2&_ADDI) DtIMY.Config2|=_ADD_6mA;	//zwiêkszyc prad wyswietlacza?
	//----------------------------------
	//Zapis danych w buforze wyswietlacza 

	SetDataBufI2C(_ADR_SAA,_SAA_ADR2);					//slave adres
	SetDataBufI2C(_SADR_SAA, 0x00);						//poczatkowa wartosc subadresu (autoinkrementacja)
	SetDataBufI2C(_CONTROL_SAA,DtIMY.Config2);
	SetDataBufI2C(_DIGIT1_SAA,lb0);
	SetDataBufI2C(_DIGIT2_SAA,hb0);
	if(lk0&&!(DtIMY.Status2&_CLS)) BitSetBufI2C(_DIGIT1_SAA,_KROPKA);				//zapal kropke dla wody WU
	if(mk0&&!(DtIMY.Status2&_CLS)) BitSetBufI2C(_DIGIT2_SAA,_KROPKA);
	SetDataBufI2C(_DIGIT3_SAA,lb1);
	SetDataBufI2C(_DIGIT4_SAA,hb1);
	if(lk1&&!(DtIMY.Status2&_CLS)) BitSetBufI2C(_DIGIT3_SAA,_KROPKA);				//zapal kropke dla wody WU
	if(mk1&&!(DtIMY.Status2&_CLS)) BitSetBufI2C(_DIGIT4_SAA,_KROPKA);
}
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//Konfiguracja i zapis danych do ukladu SAA3 (struktura DtIMY)
void NewDataToSAA3(void)
{
	unsigned char f0,f1,f2,f3;

	if(DtIMY.bFls)								//obsluga pulsacji dla wybranych diod
	{
		f0=DtIMY.WeWy0^(DtIMY.FStat0&DtIMY.WeWy0);
		f1=DtIMY.WeWy1^(DtIMY.FStat1&DtIMY.WeWy1);
		f2=DtIMY.WeWy2^(DtIMY.FStat2&DtIMY.WeWy2);
		f3=DtIMY.WeWy3^(DtIMY.FStat3&DtIMY.WeWy3);
	}
	else
	{
		f0=DtIMY.WeWy0;
		f1=DtIMY.WeWy1;
		f2=DtIMY.WeWy2;
		f3=DtIMY.WeWy3;
	}
	if(DtIMY.Status3&_CLS) 						//wygasiæ diody?
	{	
		f0=_BLANK;
		f1=_BLANK;
		f2=_BLANK;
		f3=_BLANK;
	}
	
	SetDataBufI2C(_ADR_SAA,_SAA_ADR3);			//slave adres
	SetDataBufI2C(_SADR_SAA, 0x00);				//poczatkowa wartosc subadresu (autoinkrementacja)
	SetDataBufI2C(_CONTROL_SAA,DtIMY.Config3);
	SetDataBufI2C(_DIGIT1_SAA,f0);
	SetDataBufI2C(_DIGIT2_SAA,f1);
	SetDataBufI2C(_DIGIT3_SAA,f2);
	SetDataBufI2C(_DIGIT4_SAA,f3);
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//Przyk³ad konfiguracji wyswietlania w module IMY
//
//	DtIMY.Config0=_DYNAMIC_MODE|_D1_3NBLANK|_D2_4NBLANK|_ADD_3mA;
//	DtIMY.Status0=_FLS0|_FLS1;
//	DtIMY.CO=GetI2CError();
//	DtIMY.PCO=78;
//	DtIMY.Config1=_DYNAMIC_MODE|_D1_3NBLANK|_D2_4NBLANK|_ADD_3mA;
//	DtIMY.CW=35;
//	DtIMY.PCW=60;
//	DtIMY.Config2=_DYNAMIC_MODE|_D1_3NBLANK|_D2_4NBLANK|_ADD_3mA;
//	DtIMY.CS=6;
//	DtIMY.PrzCW=6;
//	DtIMY.Config3=_DYNAMIC_MODE|_D1_3NBLANK|_D2_4NBLANK|_ADD_3mA;
//	DtIMY.Status3=_CLS;
//	BitSet(&(DtIMY.WeWy0),_RMD);
//	BitSet(&(DtIMY.FStat0),_RMD);
//	DtIMY.WeWy1=0xff;
//	DtIMY.WeWy2=0xff;
//	DtIMY.WeWy3=0xff;
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//Zapis danych odczytanych z modulu MMY do struktury DtIMY
void WriteDataIMY(void)
{
//---------------------------------------
//Wygaszenie wskazu przy braku MMY lub bledach transmisji do/z MMY
	if(DtIMY.NoMMY||DtIMY.ErMMY)			//brak modulu MMY?
	{
		DtIMY.Status0=_CLS;
		DtIMY.Status1=_CLS;
		DtIMY.Status2=_CLS;
		DtIMY.Status3=_CLS;
	}
	else
	{
		DtIMY.Status0&=~_CLS;
		DtIMY.Status1&=~_CLS;
		DtIMY.Status2&=~_CLS;
		DtIMY.Status3&=~_CLS;
	}
//---------------------------------------
//Wskaz CO i PCO
	DtIMY.Config0=_DYNAMIC_MODE|_D1_3NBLANK|_D2_4NBLANK|_ADD_3mA;
	if(GetBit(DtMMY.ST_AD1,_ERR_CO)) DtIMY.Status0|=_KRS1;	//awaria czujnika CO?
	else DtIMY.Status0&=~_KRS1;
	DtIMY.CO=DtMMY.CO;
	DtIMY.PCO=DtMMY.PCO;
//---------------------------------------
//Wskaz CW i PCW
	DtIMY.Config1=_DYNAMIC_MODE|_D1_3NBLANK|_D2_4NBLANK|_ADD_3mA;
	if(DtMMY.ST_AD1==UNICO2) DtIMY.Status1|=(_CLS1|_CLS0);
	else DtIMY.Status1&=~(_CLS1|_CLS0);
	if(GetBit(DtMMY.ST_AD1,_ERR_CW)) DtIMY.Status1|=_KRS1;	//awaria czujnika CW?
	else DtIMY.Status1&=~_KRS1;
	DtIMY.CW=DtMMY.CW;
	DtIMY.PCW=DtMMY.PCW;
//---------------------------------------
//Wskaz CS i TPRZ_CW
	DtIMY.Config2=_DYNAMIC_MODE|_D1_3NBLANK|_D2_4NBLANK|_ADD_3mA;
	if(GetBit(DtMMY.ST_AD1,TERR_CSN)) DtIMY.Status2|=_KRS0;	//awaria czujnika cisnienia?
	else DtIMY.Status2&=~_KRS0;
	DtIMY.CS=DtMMY.CS;
	DtIMY.TPRZ_CW=DtMMY.TPRZ_CW;
//---------------------------------------
//Stan kontrolek WE/WY
	DtIMY.Config3=_DYNAMIC_MODE|_D1_3NBLANK|_D2_4NBLANK|_ADD_3mA;
	BitSet(&(DtIMY.WeWy0),_RT,GetBit(DtMMY.ST_GLOBAL0,S_RT));	
	BitSet(&(DtIMY.WeWy0),_WU,GetBit(DtMMY.ST_GLOBAL0,S_CW));	
	BitSet(&(DtIMY.WeWy0),_BTB,!GetBit(DtMMY.ST_GLOBAL0,S_ALARM));
	BitSet(&(DtIMY.WeWy0),_RMD,!GetBit(DtMMY.ST_GLOBAL0,S_MOD_ON));
	BitSet(&(DtIMY.WeWy0),_GPL,GetBit(DtMMY.ST_GLOBAL2,GP));
	BitSet(&(DtIMY.WeWy0),_OPD,GetBit(DtMMY.ST_GLOBAL2,OPDG));
	if(DtMMY.KodSter==UNICO2)		//sterownik Uniko(2)? 
	{
		BitSet(&(DtIMY.WeWy0),_PMP,GetBit(DtMMY.PORTC,VNT));
		BitSet(&(DtIMY.WeWy0),_VNT,0);
	}
	else
	{
 		BitSet(&(DtIMY.WeWy0),_PMP,GetBit(DtMMY.PORTD,POMPA));
		BitSet(&(DtIMY.WeWy0),_VNT,GetBit(DtMMY.PORTC,VNT));
	}
	BitSet(&(DtIMY.WeWy1),_PLM,!GetBit(DtMMY.ST_GLOBAL0,S_PLOMIEN));	
	BitSet(&(DtIMY.WeWy1),_CIG,!GetBit(DtMMY.ST_GLOBAL0,S_BRAK_CI));
	BitSet(&(DtIMY.WeWy1),_PRZ,GetBit(DtMMY.ST_GLOBAL0,S_PRZ_CO));
	BitSet(&(DtIMY.WeWy1),_Z95,!GetBit(DtMMY.ST_GLOBAL2,S_STB));
	BitSet(&(DtIMY.WeWy1),_ZRO,!GetBit(DtMMY.ST_GLOBAL2,S_SWC));
	BitSet(&(DtIMY.WeWy1),_ZMA,GetBit(DtMMY.PORTD,ZIMA));
	BitSet(&(DtIMY.WeWy1),_LTO,GetBit(DtMMY.ST_GLOBAL2,S_SWC)&&!GetBit(DtMMY.PORTD,ZIMA));
	BitSet(&(DtIMY.WeWy1),_RST,GetBit(DtMMY.ST_GLOBAL0,S_RESET));
//---------------------------------------
//Stan kontrolek awaryjnych
	BitSet(&(DtIMY.WeWy2),_ER1,GetBit(DtMMY.ST0_WSW,_MSG_A1));	
	BitSet(&(DtIMY.WeWy2),_ER2,GetBit(DtMMY.ST0_WSW,_MSG_A2));
	BitSet(&(DtIMY.WeWy2),_ER3,GetBit(DtMMY.ST0_WSW,_MSG_A3));
	BitSet(&(DtIMY.WeWy2),_ER4,GetBit(DtMMY.ST0_WSW,_MSG_A4));
	BitSet(&(DtIMY.WeWy2),_ER5,GetBit(DtMMY.ST0_WSW,_MSG_A5));
	BitSet(&(DtIMY.WeWy2),_ER7,GetBit(DtMMY.ST0_WSW,_MSG_A7));
	BitSet(&(DtIMY.WeWy2),_ER8,GetBit(DtMMY.ST0_WSW,_MSG_A8));
	BitSet(&(DtIMY.WeWy3),_ER9,GetBit(DtMMY.ST0_WSW,_MSG_A9));
//Obsluga pulsacji kontrolek stanow awaryjnych
	switch(DtMMY.ERR_BTY)							//kod awaryjny
	{
		case 1:
			DtIMY.WeWy2=0;
			DtIMY.WeWy3=0;
			BitSet(&(DtIMY.WeWy2),_ER1,1);
			BitSet(&(DtIMY.FStat2),_ER1,1);
		break;
		case 2:
			DtIMY.WeWy2=0;
			DtIMY.WeWy3=0;
			BitSet(&(DtIMY.WeWy2),_ER2,1);
			BitSet(&(DtIMY.FStat2),_ER2,1);
		break;			
		case 3:
			DtIMY.WeWy2=0;
			DtIMY.WeWy3=0;
			BitSet(&(DtIMY.WeWy2),_ER3,1);
			BitSet(&(DtIMY.FStat2),_ER3,1);
		break;			
		case 4:
			DtIMY.WeWy2=0;
			DtIMY.WeWy3=0;
			BitSet(&(DtIMY.WeWy2),_ER4,1);
			BitSet(&(DtIMY.FStat2),_ER4,1);
		break;
		case 5:
			DtIMY.WeWy2=0;
			DtIMY.WeWy3=0;
			BitSet(&(DtIMY.WeWy2),_ER5,1);
			BitSet(&(DtIMY.FStat2),_ER5,1);
		break;	
		case 7:
			DtIMY.WeWy2=0;
			DtIMY.WeWy3=0;
			BitSet(&(DtIMY.WeWy2),_ER7,1);
			BitSet(&(DtIMY.FStat2),_ER7,1);
		break;	
		case 8:
			DtIMY.WeWy2=0;
			DtIMY.WeWy3=0;
			BitSet(&(DtIMY.WeWy2),_ER8,1);
			BitSet(&(DtIMY.FStat2),_ER8,1);
		break;
		case 9:
			DtIMY.WeWy2=0;
			DtIMY.WeWy3=0;
			BitSet(&(DtIMY.WeWy3),_ER9,1);	
			BitSet(&(DtIMY.FStat3),_ER9,1);
		break;	
		default:
			DtIMY.FStat2=0;	
			DtIMY.FStat3=0;
	}
}
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//Oblicza CHK i umieszcza go w ostatnim bajcie bufora danych
//NBTI2C - liczba danych bez adresu z CHK
unsigned char LiczCHK(const unsigned char NBTI2C)
{
	unsigned char i;
	unsigned char chk=0;
	
	for(i=0;i<NBTI2C-1;i++) chk+=(GetDataBufI2C(i+1)+i)^0xAA;
	return chk;		
}
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//Zapisz dane z modulu MMY do DtMMY 
bit WriteDataI2C(void)
{
	if(LiczCHK(_NBMMY-1)==GetDataBufI2C(N_CHK))			//poprawna suma kontrolna?
	{
		DtMMY.KodSter=GetDataBufI2C(N_KOD_STER);
		DtMMY.PCO=GetDataBufI2C(N_PCO);
		DtMMY.CO=GetDataBufI2C(N_CO);
		DtMMY.PCW=GetDataBufI2C(N_PCW);
		DtMMY.CW=GetDataBufI2C(N_CW);	
		DtMMY.CS=GetDataBufI2C(N_CS);
		DtMMY.STR_PWM=GetDataBufI2C(N_STR_PWM);
		DtMMY.MAX_PWM=GetDataBufI2C(N_MAX_PWM);
		DtMMY.BUF_PWM=GetDataBufI2C(N_BUF_PWM);
		DtMMY.ST_AD1=GetDataBufI2C(N_ST_AD1);
		DtMMY.ST_AD2=GetDataBufI2C(N_ST_AD2);
		DtMMY.ST_AD3=GetDataBufI2C(N_ST_AD3);
		DtMMY.PORTC=GetDataBufI2C(N_PORTC);
		DtMMY.PORTD=GetDataBufI2C(N_PORTD);
		DtMMY.ST_GLOBAL0=GetDataBufI2C(N_ST_GLOBAL0);
		DtMMY.ST_GLOBAL2=GetDataBufI2C(N_ST_GLOBAL2);
		DtMMY.ST_GLOBAL3=GetDataBufI2C(N_ST_GLOBAL3);
		DtMMY.ST_GLOBAL4=GetDataBufI2C(N_ST_GLOBAL4);
		DtMMY.TPRZ_CW=GetDataBufI2C(N_TPRZ_CW);
		DtMMY.ST0_WSW=GetDataBufI2C(N_ST0_WSW);
		DtMMY.ERR_BTY=GetDataBufI2C(N_ERR_BTY);
		return 1;
	}
	return 0;
}
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//Inicjalizacja modulu I2C
void OpenPortI2C(void)
{
	TRISC3=1;				//SDA i SCA konfiguracja jako wejscia
	TRISC4=1;
	InitI2C();
}
//Wylaczenie modulu I2C
void ClosePortI2C(void)
{
	SSPEN=0;				//disables the serial port
	TRISC3=0;				//SDA i SCA konfiguracja jako wyjscia
	TRISC4=0;
	RC3=0;
	RC4=0;
}
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//Konfiguracja i zapis danych do ukladow SAA modulu IMY
void TransmitDataToIMY(void)
{
	IMYTimer();
	WriteDataIMY();
	ClosePortI2C();
	NewDataToSAA0();
	OpenPortI2C();
	#if (SYGTRN!=0)						//aktywna sygnalizacja transferu danych?
		if(DtIMY.NoMMY) LGRN=SYGGRN;
	#endif
	TransmitI2C(_NBSAA);
	#if (SYGTRN!=0)						//aktywna sygnalizacja transferu danych?
		LGRN=!SYGGRN;
		LRED=(GetI2CError())?SYGRED:!SYGRED;
	#endif
	ClosePortI2C();
	NewDataToSAA1();
	OpenPortI2C();
	TransmitI2C(_NBSAA);
	#if (SYGTRN!=0)
		LRED=(GetI2CError())?SYGRED:!SYGRED;
	#endif
	ClosePortI2C();
	NewDataToSAA2();
	OpenPortI2C();
	TransmitI2C(_NBSAA);
	#if (SYGTRN!=0)
		LRED=(GetI2CError())?SYGRED:!SYGRED;
	#endif
	ClosePortI2C();
	NewDataToSAA3();
	OpenPortI2C();
	TransmitI2C(_NBSAA);
	#if (SYGTRN!=0)
		LRED=(GetI2CError())?SYGRED:!SYGRED;
	#endif
}
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//Odbior danych z modulu MMY
void ReceptDataFromMMY(void)
{
	ClosePortI2C();
	SetDataBufI2C(N_ADR_STER,_AdrMMY);							//aders modulu MMY
	OpenPortI2C();
	#if (SYGTRN!=0)												//aktywna sygnalizacja transferu danych?
		if(!DtIMY.NoMMY) LGRN=SYGGRN;							//otwarcie portu I2C
	#endif
	if(ReceptI2C(_NBMMY)) DtIMY.ErCHK=!WriteDataI2C();			//odbior danej z modulu MMY pomyslny?
	DtIMY.NoMMY=(GetI2CError()==_ErTIMO3);
	if(!DtIMY.NoMMY) DtIMY.ErMMY=(GetI2CError()!=0);
	else DtIMY.ErMMY=0;
	#if (SYGTRN!=0)												//aktywna sygnalizacja transferu danych
		LGRN=!SYGGRN;
		LRED=(DtIMY.ErMMY||DtIMY.ErCHK)?SYGRED:!SYGRED; 		//blad odbioru (pominawszy brak odbiornika)?
	#endif
}
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//Obsluga komunikacji z modulem MMY i wyswietlania danych na module typu IMY
void KomunikacjaIMY(void)
{
		if(I2CTim())				//odbior i transmisja realizowane co 0.1 sek.
		{
			StartI2CTim();
			ReceptDataFromMMY();	//odbior danych z modulu MMY
			TransmitDataToIMY();	//przeslanie danych do modulu IMY
		}
}
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
