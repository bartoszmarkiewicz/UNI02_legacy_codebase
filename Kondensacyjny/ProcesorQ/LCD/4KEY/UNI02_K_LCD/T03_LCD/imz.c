/*imz.c*/
/*Procedury zwiazane z transmisja danych do ukladu IMZ-xx*/

#include <pic18.h>
#include "global.h"			//parametry globalne
#include "imz.h"			//parametry lokalne
#include "ammy3.h"		
#include "saa.h"
 
tpIMZ DtIMZ;
extern tpAMMY3 DtAMMY3;

bit mk1;
bit lk1;
bit mk0;
bit lk0;
const unsigned char KodLed[17]={
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
void InitIMZ(void)
{
	DtIMZ.bFls=0;
	DtIMZ.timIMZ=0;	
	#if (SYGTRN!=0)							
		LGRN=!SYGGRN;
		LRED=!SYGRED; 						
	#endif
}
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------	
//Timer na potrzeby pulsacji wskazu wyswietlacza (umiescic w przerwaniu od zegara TMR1)
void IncIMZTimer(void)
{
	if(DtIMZ.timIMZ<255) DtIMZ.timIMZ++;
}
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//Wyznaczanie odcinkow czasowych pulsacji wskazu wyswietlacza
void IMZTimer(void)
{
	if(DtIMZ.timIMZ>=_IMZPULSE)
	{
		DtIMZ.timIMZ=0;	
		DtIMZ.bFls=~DtIMZ.bFls;
	}
}
//Start zegara regulujacego odcinki czasowe miedzy transmisjami
void StartIMZTim(void)
{
	DtIMZ.timIMZ=0;
	DtIMZ.bFls=0;	
}
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//konwersja danej typu dec na kod wyswietlacza
void DecTo7Led(signed char data,unsigned char *C1, unsigned char *C0, unsigned char blank)
{
	if(data>99) data=99;
	if(data<(-9)) data=-9;
	if(data>=0)
	{
		if(blank)
		{
			if(data<10) *C1=_BLANK;
			else *C1=~KodLed[data/10];
	 		*C0=~KodLed[data%10];
		}
		else
		{
			*C1=~KodLed[data/10];
	 		*C0=~KodLed[data%10];
		}
	}
	else
	{
		data=-1*data;
		*C1=_KRESKA;
		*C0=~KodLed[data];
	}
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
	//HexTo7Led(DtIMZ.CO,&hb1,&lb1);
	DecTo7Led(DtIMZ.CO,&hb1,&lb1,1);						//konwersja danej na format wyswietlacza
	DecTo7Led(DtIMZ.CW,&hb0,&lb0,1);						//konwersja danej na format wyswietlacza
	//----------------------------------
	//Priorytetowe parametry wyswietlania
	//starsze dwie cyfry
	if(DtIMZ.Status0&_KRS1) 							//wyswietlic dwie kreski?
	{	
		lb1=_KRESKA;
		hb1=_KRESKA;
	}
	if(DtIMZ.Status0&_FLS1&&DtIMZ.bFls)
	{
		lb1=_BLANK;
		hb1=_BLANK;
	}
	if((DtIMZ.Status0&_CLS1)||(DtIMZ.Status0&_CLS)) 	//wygasiæ wuswietlacz?
	{	
		lb1=_BLANK;
		hb1=_BLANK;
	}
	//mlodsze dwie cyfry
	if(DtIMZ.Status0&_KRS0) 							//wyswietlic dwie kreski?
	{	
		lb0=_KRESKA;
		hb0=_KRESKA;
	}
	if(DtIMZ.Status0&_FLS0&&DtIMZ.bFls)
	{
		lb0=_BLANK;
		hb0=_BLANK;
	}
	if((DtIMZ.Status0&_CLS0)||(DtIMZ.Status0&_CLS))	//wygasiæ wuswietlacz? 
	{	
		lb0=_BLANK;
		hb0=_BLANK;
	}
	//wszystkie cyfry
	if(DtIMZ.Status0&_ADDI) DtIMZ.Config0|=_ADD_6mA;	//zwiêkszyc prad wyswietlacza?
	//----------------------------------
	//Zapis danych w buforze wyswietlacza 
	SetDataBufI2C(_ADR_SAA,_SAA_ADR0);					//slave adres
	SetDataBufI2C(_SADR_SAA, 0x00);						//poczatkowa wartosc subadresu (autoinkrementacja)
	SetDataBufI2C(_CONTROL_SAA,DtIMZ.Config0);
	SetDataBufI2C(_DIGIT1_SAA,lb0);
	SetDataBufI2C(_DIGIT2_SAA,hb0);
	SetDataBufI2C(_DIGIT3_SAA,lb1);
	SetDataBufI2C(_DIGIT4_SAA,hb1);
	if(!(DtIMZ.Status0&_CLS)&&!(DtIMZ.Status0&_CLS0)) BitSetBufI2C(_DIGIT1_SAA,_KROPKA);
}
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//Konfiguracja i zapis danych do ukladu SAA1 
void NewDataToSAA1(void)
{
	unsigned char hb1,lb1,hb0,lb0;
	//----------------------------------
	//Obróbka danych do wyswietlenia
	DecTo7Led(DtIMZ.CS,&hb1,&lb1,0);						//konwersja danej na format wyswietlacza
	DecTo7Led(DtIMZ.ZW,&hb0,&lb0,1);						//konwersja danej na format wyswietlacza
	//----------------------------------
	//Priorytetowe parametry wyswietlania
	//starsze dwie cyfry
	if(DtIMZ.Status1&_KRS1) 							//wyswietlic dwie kreski?
	{	
		lb1=_KRESKA;
		hb1=_KRESKA;
	}
	if(DtIMZ.Status1&_FLS1&&DtIMZ.bFls)
	{
		lb1=_BLANK;
		hb1=_BLANK;
	}
	if((DtIMZ.Status1&_CLS1)||(DtIMZ.Status1&_CLS)) 	//wygasiæ wuswietlacz?
	{	
		lb1=_BLANK;
		hb1=_BLANK;
	}
	//mlodsze dwie cyfry
	if(DtIMZ.Status1&_KRS0) 							//wyswietlic dwie kreski?
	{	
		lb0=_KRESKA;
		hb0=_KRESKA;
	}
	if(DtIMZ.Status1&_FLS0&&DtIMZ.bFls)
	{
		lb0=_BLANK;
		hb0=_BLANK;
	}
	if((DtIMZ.Status1&_CLS0)||(DtIMZ.Status1&_CLS))	//wygasiæ wuswietlacz? 
	{	
		lb0=_BLANK;
		hb0=_BLANK;
	}
	//wszystkie cyfry
	if(DtIMZ.Status1&_ADDI) DtIMZ.Config1|=_ADD_6mA;	//zwiêkszyc prad wyswietlacza?
	//----------------------------------
	//Zapis danych w buforze wyswietlacza 
	SetDataBufI2C(_ADR_SAA,_SAA_ADR1);					//slave adres
	SetDataBufI2C(_SADR_SAA, 0x00);						//poczatkowa wartosc subadresu (autoinkrementacja)
	SetDataBufI2C(_CONTROL_SAA,DtIMZ.Config1);
	SetDataBufI2C(_DIGIT1_SAA,lb0);
	SetDataBufI2C(_DIGIT2_SAA,hb0);
	SetDataBufI2C(_DIGIT3_SAA,lb1);
	SetDataBufI2C(_DIGIT4_SAA,hb1);
	if(!(DtIMZ.Status1&_CLS)&&!(DtIMZ.Status1&_CLS1)) BitSetBufI2C(_DIGIT4_SAA,_KROPKA);
}
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//Konfiguracja i zapis danych do ukladu SAA1 
void NewDataToSAA2(void)
{
	unsigned char hb1,lb1,hb0,lb0;
	//----------------------------------
	//Obróbka danych do wyswietlenia
	DecTo7Led(DtIMZ.PRZ,&hb0,&lb0,0);						//konwersja danej na format wyswietlacza
	//DecTo7Led(xxy,&hb1,&lb1,0);						//konwersja danej na format wyswietlacza
	
	//----------------------------------
	//Priorytetowe parametry wyswietlania
	//starsze dwie cyfry
	if(DtIMZ.Status2&_KRS1) 							//wyswietlic dwie kreski?
	{	
		lb1=_KRESKA;
		hb1=_KRESKA;
	}
	if(DtIMZ.Status2&_FLS1&&DtIMZ.bFls)
	{
		lb1=_BLANK;
		hb1=_BLANK;
	}
	if((DtIMZ.Status2&_CLS1)||(DtIMZ.Status2&_CLS)) 	//wygasiæ wuswietlacz?
	{	
		lb1=_BLANK;
		hb1=_BLANK;
	}
	//mlodsze dwie cyfry
	if(DtIMZ.Status2&_KRS0) 							//wyswietlic dwie kreski?
	{	
		lb0=_KRESKA;
		hb0=_KRESKA;
	}
	if(DtIMZ.Status2&_FLS0&&DtIMZ.bFls)
	{
		lb0=_BLANK;
		hb0=_BLANK;
	}

	if((DtIMZ.Status2&_CLS0)||(DtIMZ.Status2&_CLS))	//wygasiæ wuswietlacz? 
	{	
		lb0=_BLANK;
		hb0=_BLANK;
	}

	//wszystkie cyfry
	if(DtIMZ.Status2&_ADDI) DtIMZ.Config2|=_ADD_6mA;	//zwiêkszyc prad wyswietlacza?
	//----------------------------------
	//Zapis danych w buforze wyswietlacza 
	SetDataBufI2C(_ADR_SAA,_SAA_ADR2);					//slave adres
	SetDataBufI2C(_SADR_SAA, 0x00);						//poczatkowa wartosc subadresu (autoinkrementacja)
	SetDataBufI2C(_CONTROL_SAA,DtIMZ.Config2);
	SetDataBufI2C(_DIGIT1_SAA,lb0);
	SetDataBufI2C(_DIGIT2_SAA,hb0);
	if(!(DtIMZ.Status2&_CLS)&&!(DtIMZ.Status2&_CLS0)) BitSetBufI2C(_DIGIT2_SAA,_KROPKA);
	//lb1=_BLANK;
	//hb1=_BLANK;
	SetDataBufI2C(_DIGIT3_SAA,lb1);
	SetDataBufI2C(_DIGIT4_SAA,hb1);
}
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//Konfiguracja i zapis danych do ukladu SAA3 (struktura DtIMZ)
void NewDataToSAA3(void)
{
	unsigned char f0,f1,f2,f3;

	if(DtIMZ.bFls)								//obsluga pulsacji dla wybranych diod
	{
		f0=DtIMZ.WeWy0^(DtIMZ.FStat0&DtIMZ.WeWy0);
		f1=DtIMZ.WeWy1^(DtIMZ.FStat1&DtIMZ.WeWy1);
		f2=DtIMZ.WeWy2^(DtIMZ.FStat2&DtIMZ.WeWy2);
		f3=DtIMZ.WeWy3^(DtIMZ.FStat3&DtIMZ.WeWy3);
	}
	else
	{
		f0=DtIMZ.WeWy0;
		f1=DtIMZ.WeWy1;
		f2=DtIMZ.WeWy2;
		f3=DtIMZ.WeWy3;
	}
	if(DtIMZ.Status3&_CLS) 						//wygasiæ diody?
	{	
		f0=_BLANK;
		f1=_BLANK;
		f2=_BLANK;
		f3=_BLANK;
	}
	
	SetDataBufI2C(_ADR_SAA,_SAA_ADR3);			//slave adres
	SetDataBufI2C(_SADR_SAA, 0x00);				//poczatkowa wartosc subadresu (autoinkrementacja)
	SetDataBufI2C(_CONTROL_SAA,DtIMZ.Config3);
	SetDataBufI2C(_DIGIT1_SAA,f0);
	SetDataBufI2C(_DIGIT2_SAA,f1);
	SetDataBufI2C(_DIGIT3_SAA,f2);
	SetDataBufI2C(_DIGIT4_SAA,f3);
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//Przyk³ad konfiguracji wyswietlania w module IMZ
//
//	DtIMZ.Config0=_DYNAMIC_MODE|_D1_3NBLANK|_D2_4NBLANK|_ADD_3mA;
//	DtIMZ.Status0=_FLS0|_FLS1;
//	DtIMZ.CO=GetI2CError();
//	DtIMZ.CW=78;
//	DtIMZ.Config2=_DYNAMIC_MODE|_D1_3NBLANK|_D2_4NBLANK|_ADD_3mA;
//	DtIMZ.CS=6;
//	DtIMZ.PrzCW=6;
//	DtIMZ.Config3=_DYNAMIC_MODE|_D1_3NBLANK|_D2_4NBLANK|_ADD_3mA;
//	DtIMZ.Status3=_CLS;
//	BitSet(&(DtIMZ.WeWy0),_RMD);
//	BitSet(&(DtIMZ.FStat0),_RMD);
//	DtIMZ.WeWy1=0xff;
//	DtIMZ.WeWy2=0xff;
//	DtIMZ.WeWy3=0xff;
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//Zapis danych struktury DtIMZ
void WriteDataIMZ(void)
{
	unsigned char x,y;
	static unsigned char z;
//---------------------------------------
//Wygaszenie wskazu przy braku MMY
	if(DtIMZ.ClrIMZ)
	{
		DtIMZ.Status0=_CLS;
		DtIMZ.Status1=_CLS;
		DtIMZ.Status2=_CLS;
		DtIMZ.Status3=_CLS;
	}
	else
	{
		DtIMZ.Status0&=~_CLS;
		DtIMZ.Status1&=~_CLS;
		DtIMZ.Status2&=~_CLS;
		DtIMZ.Status3&=~_CLS;
	}
//---------------------------------------
//Wskaz CO i CW
	//x=GetBit(DtAMMY3.MPOT,_bpPCO);
	//y=GetBit(DtAMMY3.MPOT,_bpPCW);
	DtIMZ.Config0=_DYNAMIC_MODE|_D1_3NBLANK|_D2_4NBLANK|_ADD_3mA;
	DtIMZ.CO=DtAMMY3.CO;
	DtIMZ.CW=DtAMMY3.CW;
//---------------------------------------
//Wskaz CS i ZW
	//x=GetBit(DtAMMY3.MPOT,_bpPCS);
//	y=GetBit(DtAMMY3.MPOT,_bpPZW);
	DtIMZ.Config1=_DYNAMIC_MODE|_D1_3NBLANK|_D2_4NBLANK|_ADD_3mA;
	DtIMZ.CS=DtAMMY3.CS;
	DtIMZ.ZW=DtAMMY3.ZW;
//---------------------------------------
//Wskaz PRZ
	//x=GetBit(DtAMMY3.MPOT,_bpPPR);
	DtIMZ.Config2=_DYNAMIC_MODE|_D1_3NBLANK|_D2_4NBLANK|_ADD_3mA;
	DtIMZ.PRZ=DtAMMY3.PRZ;
//---------------------------------------
//Stan kontrolek WE/WY
	DtIMZ.Config3=_DYNAMIC_MODE|_D1_3NBLANK|_D2_4NBLANK|_ADD_3mA;
	
//---------------------------------------
//Sygnalizacja stanu przyciskow

	DtIMZ.WeWy2=0;
	DtIMZ.WeWy3=0;
	DtIMZ.FStat0=0;
	DtIMZ.FStat1=0;
	DtIMZ.FStat2=0;
	DtIMZ.FStat3=0;

	if(!GetBit(DtAMMY3.MPOT,_bpPCO)) DtIMZ.Status0|=_CLS1;
	else DtIMZ.Status0&=~_CLS1;
	if(!GetBit(DtAMMY3.MPOT,_bpPCW)) DtIMZ.Status0|=_CLS0;
	else DtIMZ.Status0&=~_CLS0;
	if(!GetBit(DtAMMY3.MPOT,_bpPCS)) DtIMZ.Status1|=_CLS1;
	else DtIMZ.Status1&=~_CLS1;
	if(!GetBit(DtAMMY3.MPOT,_bpPZW)) DtIMZ.Status1|=_CLS0;
	else DtIMZ.Status1&=~_CLS0;
	if(!GetBit(DtAMMY3.MPOT,_bpPPR)) DtIMZ.Status2|=_CLS0;
	else DtIMZ.Status2&=~_CLS0;
	//BitSet(&(DtIMZ.Status2),_CLS1,1);
	if(GetBit(DtAMMY3.MPOB0,_bpRT)) BitSet(&(DtIMZ.WeWy1),_SW9,GetBit(DtAMMY3.TPOB0,_bpRT));
	if(GetBit(DtAMMY3.MPOB0,_bpCW)) BitSet(&(DtIMZ.WeWy1),_SW10,GetBit(DtAMMY3.TPOB0,_bpCW));
	if(GetBit(DtAMMY3.MPOB0,_bpZW)) BitSet(&(DtIMZ.WeWy1),_SW11,GetBit(DtAMMY3.TPOB0,_bpZW));
	if(GetBit(DtAMMY3.MPOB0,_bpE0)) BitSet(&(DtIMZ.WeWy1),_SW12,GetBit(DtAMMY3.TPOB0,_bpE0));
	if(GetBit(DtAMMY3.MPOB0,_bpE1)) BitSet(&(DtIMZ.WeWy1),_SW13,GetBit(DtAMMY3.TPOB0,_bpE1));
	if(GetBit(DtAMMY3.MPOB0,_bpE2)) BitSet(&(DtIMZ.WeWy1),_SW14,GetBit(DtAMMY3.TPOB0,_bpE2));
	if(GetBit(DtAMMY3.MPOB0,_bpE3)) BitSet(&(DtIMZ.WeWy1),_SW15,GetBit(DtAMMY3.TPOB0,_bpE3));
	if(GetBit(DtAMMY3.MPOB0,_bpE4)) BitSet(&(DtIMZ.WeWy1),_SW16,GetBit(DtAMMY3.TPOB0,_bpE4));
	if(GetBit(DtAMMY3.MPOB1,_bpE5)) BitSet(&(DtIMZ.WeWy0),_SW1,GetBit(DtAMMY3.TPOB1,_bpE5));
	if(GetBit(DtAMMY3.MPOB1,_bpE6)) BitSet(&(DtIMZ.WeWy0),_SW2,GetBit(DtAMMY3.TPOB1,_bpE6));
	if(GetBit(DtAMMY3.MPOB1,_bpE7)) BitSet(&(DtIMZ.WeWy0),_SW3,GetBit(DtAMMY3.TPOB1,_bpE7));
	if(GetBit(DtAMMY3.MPOB1,_bpE8)) BitSet(&(DtIMZ.WeWy0),_SW4,GetBit(DtAMMY3.TPOB1,_bpE8));
	if(GetBit(DtAMMY3.MPOB1,_bpE9)) BitSet(&(DtIMZ.WeWy0),_SW5,GetBit(DtAMMY3.TPOB1,_bpE9));
	if(GetBit(DtAMMY3.MPOB1,_bpE10)) BitSet(&(DtIMZ.WeWy0),_SW6,GetBit(DtAMMY3.TPOB1,_bpE10));
	if(GetBit(DtAMMY3.MPOB1,_bpE11)) BitSet(&(DtIMZ.WeWy0),_SW7,GetBit(DtAMMY3.TPOB1,_bpE11));
	if(GetBit(DtAMMY3.MPOB1,_bpE12)) BitSet(&(DtIMZ.WeWy0),_SW8,GetBit(DtAMMY3.TPOB1,_bpE12));
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
//Konfiguracja i zapis danych do bufora transmisji I2C
/*void TransmitDataToIMZ(void)
{
	IMZTimer();
	WriteDataIMZ();
	ClosePortI2C();
	NewDataToSAA0();
	OpenPortI2C();
	#if (SYGTRN!=0)						//aktywna sygnalizacja transferu danych?
		if(DtIMZ.ClrIMZ) LGRN=SYGGRN;
	#endif
	TransmitI2C(7);
	#if (SYGTRN!=0)						//aktywna sygnalizacja transferu danych?
		LGRN=!SYGGRN;
		LRED=(GetI2CError())?SYGRED:!SYGRED;
	#endif
	ClosePortI2C();
	NewDataToSAA1();
	OpenPortI2C();
	TransmitI2C(7);
	#if (SYGTRN!=0)
		LRED=(GetI2CError())?SYGRED:!SYGRED;
	#endif
	ClosePortI2C();
	NewDataToSAA2();
	OpenPortI2C();
	TransmitI2C(7);
	#if (SYGTRN!=0)
		LRED=(GetI2CError())?SYGRED:!SYGRED;
	#endif
	ClosePortI2C();
	NewDataToSAA3();
	OpenPortI2C();
	TransmitI2C(7);
	#if (SYGTRN!=0)
		LRED=(GetI2CError())?SYGRED:!SYGRED;
	#endif
}*/
unsigned char TransmitDataToIMZ(void)
{
	IMZTimer();									//timer pulsacji wskazu wyswietlacza
	switch (DtIMZ.krok)
	{
		case 0:
			ClosePortI2C();
			WriteDataIMZ();
			NewDataToSAA0();
			OpenPortI2C();
			#if (SYGTRN!=0)						//aktywna sygnalizacja transferu danych?
				if(DtIMZ.ClrIMZ) LGRN=SYGGRN;
			#endif
			DtIMZ.krok=1;
		case 1:
			if(!TransmitI2C(7)) return 0;
			#if (SYGTRN!=0)						//aktywna sygnalizacja transferu danych?
				LGRN=!SYGGRN;
				LRED=(GetI2CError())?SYGRED:!SYGRED;
			#endif
		case 2:
			ClosePortI2C();
			WriteDataIMZ();
			NewDataToSAA1();
			OpenPortI2C();
			DtIMZ.krok=3;
		case 3:
			if(!TransmitI2C(7)) return 0;
			#if (SYGTRN!=0)						//aktywna sygnalizacja transferu danych?
				LGRN=!SYGGRN;
				LRED=(GetI2CError())?SYGRED:!SYGRED;
			#endif
		case 4:
			ClosePortI2C();
			WriteDataIMZ();
			NewDataToSAA2();
			OpenPortI2C();
			DtIMZ.krok=5;
		case 5:
			if(!TransmitI2C(7)) return 0;
			#if (SYGTRN!=0)						//aktywna sygnalizacja transferu danych?
				LGRN=!SYGGRN;
				LRED=(GetI2CError())?SYGRED:!SYGRED;
			#endif
		case 6:
			ClosePortI2C();
			WriteDataIMZ();
			NewDataToSAA3();
			OpenPortI2C();
			DtIMZ.krok=7;
		case 7:
			if(!TransmitI2C(7)) return 0;
			#if (SYGTRN!=0)						//aktywna sygnalizacja transferu danych?
				LGRN=!SYGGRN;
				LRED=(GetI2CError())?SYGRED:!SYGRED;
			#endif
			DtIMZ.krok=0;
			return 1;
	}
	return 0;
}
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//Obsluga wyswietlania danych na module typu IMZ
/*void KomunikacjaIMZ(void)
{
		if(I2CTim())				//odbior i transmisja realizowane co 0.1 sek.
		{
			StartI2CTim();
			TransmitDataToIMZ();	//przeslanie danych do modulu IMZ
		}
}*/
void KomunikacjaIMZ(void)
{
		TransmitDataToIMZ();
		/*if(I2CTim())								//odbior i transmisja realizowane co 0.1 sek.
		{
			//StartI2CTim();
			if(TransmitDataToIMZ()) StartI2CTim();	//przeslanie danych do modulu IMZ
		}*/
}
////-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
