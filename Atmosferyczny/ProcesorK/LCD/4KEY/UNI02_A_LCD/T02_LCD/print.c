/*---------------------------------------------------------------------------------*/
/*print.c*/
/*Funkcje zapisu danych do 2 segmentowego wyswietlacza typu LED*/
/*w oparciu o procedury obslugi wyswietlacza zawarte w pliku wsw.c*/
/*Sposob reprezentacji danych - ITALY*/
/*---------------------------------------------------------------------------------*/
//#include <pic18.h>
#include <xc.h>
#include "global.h"				//parametry globalne
#include "print.h"				//parametry lokalne
#include "main.tp.h"

extern volatile DataPFN PFN;
extern volatile unsigned char x;
//wsw.c
extern volatile WPack PWD[];		//tablica priorytetow wyswietlania PWD[n] (n=0 - najwyzszy priorytet)
extern volatile WPack DWD[];		//tablica danych modyfikowanych dynamicznie wystepujacych wylacznie w kolejkach
extern volatile WDyn DynData;	//bufor wspolny wyswietlania

extern const unsigned char Znak[];
extern volatile WMark KMark[];;
//---------------------------------------------------------
//---------------------------------------------------------
//Funkcje wyswietlania wg waznosci.
//ClsScr
//-PrintErr
//--PrintPR,PrintWPR
//---PrintNstCW
//----PrintNstCO
//----PrintNstKT
//-----PrintMsg
//------PrintTDZ
//-------PrintOFF
//--------PrintCS
//---------PrintCW
//----------PrintCO

//---------------------------------------------------------
//---------------------------------------------------------
//Deaktywacja (wygaszenie) wyswietlacza
void ClsScr(void)
{
	WswOff();
	//UpdateWSW();
}
//Aktywacja wyswietlacza, ustawienie wszystkich kolejek na pozycji pierwszej
void EndClsScr(void)
{
	AllKolToFirstPoz();
	WswOn();
	//UpdateWSW();
}
//---------------------------------------------------------
//---------------------------------------------------------
//wyswietla migajacy/staly kod bledu
//lkod - kod led oznaczenia literowego bledu
//nkod - wartosc liczbowa bledu
//pls - dla 1 migajacy kod, dla 0 kod sta³y
/*
void PrintErr(const unsigned char lkod,unsigned char nkod, const unsigned char pls, const unsigned char res)
{
	unsigned int fk,fl,fm;
	fl=(!res)?0:_KZ_RES;
	fk=(res)?0:_KZ_RES;
	fm=(fl)?tSDID:0;
#if LCD4D==0
	if(nkod>0xf) nkod=0xf;
	SetData(&(DWD[0].Dt),lkod,Znak[nkod]);
	if(!pls)
	{
		SetData(&&(DWD[1].Dt),lkod,Znak[nkod]);
		SetStatus(&(DWD[0].St),tLED|fm|tRDID,tALL,fl,fk|_KZ_SRV|_KZ_TCO|_KZ_BAR);//_KZ_SRV|_KZ_TCO|_KZ_DCO|_KZ_PLC|_KZ_BAR);					//kod bledu
		SetStatus(&(DWD[1].St),tLED|fm|tRDID,tALL,fl,fk|_KZ_SRV|_KZ_TCO|_KZ_BAR);//_KZ_SRV|_KZ_TCO|_KZ_DCO|_KZ_PLC|_KZ_BAR);
	}
	else
	{
		SetData(&&(DWD[1].Dt),BLACK,BLACK);
		SetStatus(&(DWD[0].St),tLED|tSDID|tRDID,tALL,fl|_KZ_SRV,fk|_KZ_TCO|_KZ_BAR);//_KZ_TCO|_KZ_DCO|_KZ_BAR);					//kod bledu
		SetStatus(&(DWD[1].St),tLED|tSDID|tRDID,tALL,_KZ_SRV,fk|fl|_KZ_TCO|_KZ_BAR);//_KZ_TCO|_KZ_DCO|_KZ_BAR|_KZ_RES);
	}
#else
	SetData(&(DWD[0].Dt2),BLACK,lkod);
	SetData(&(DWD[0].Dt),0,nkod);

	if(!pls)
	{
		SetStatus(&(DWD[0].St),tDEC|fm|tRDID|tFOCUS,tALL,fl,fk|_KZ_SRV|_KZ_TCO|_KZ_TCW|_KZ_BAR);//_KZ_SRV|_KZ_TCO|_KZ_TCW|_KZ_DCO|_KZ_DCW|_KZ_BAR|_KZ_PGD);	
		SetStatus(&(DWD[0].St2),tLED|fm|tRDID|tFOCUS,tALL,fl,fk|_KZ_SRV|_KZ_TCO|_KZ_TCW|_KZ_BAR);//_KZ_SRV|_KZ_TCO|_KZ_TCW|_KZ_DCO|_KZ_DCW|_KZ_BAR|_KZ_PGD);
		SetData(&(DWD[1].Dt2),BLACK,lkod);
		SetStatus(&(DWD[1].St2),tLED|fm|tRDID|tFOCUS,tALL,fl,fk|_KZ_SRV|_KZ_TCO|_KZ_TCW|_KZ_BAR);//_KZ_SRV|_KZ_TCO|_KZ_TCW|_KZ_DCO|_KZ_DCW|_KZ_BAR|_KZ_PGD);
		SetData(&(DWD[1].Dt),0,nkod);
		SetStatus(&(DWD[1].St),tDEC|fm|tRDID|tFOCUS,tALL,fl,fk|_KZ_SRV|_KZ_TCO|_KZ_TCW|_KZ_BAR);//_KZ_SRV|_KZ_TCO|_KZ_TCW|_KZ_DCO|_KZ_DCW|_KZ_BAR|_KZ_PGD);
	}
	else
	{
		SetStatus(&(DWD[0].St),tDEC|tSDID|tRDID|tFOCUS,tALL,fl|_KZ_SRV,fk|_KZ_TCO|_KZ_TCW|_KZ_BAR);//_KZ_TCO|_KZ_TCW|_KZ_DCO|_KZ_DCW|_KZ_BAR|_KZ_PGD);	
		SetStatus(&(DWD[0].St2),tLED|tSDID|tRDID|tFOCUS,tALL,fl|_KZ_SRV,fk|_KZ_TCO|_KZ_TCW|_KZ_BAR);//_KZ_TCO|_KZ_TCW|_KZ_DCO|_KZ_DCW|_KZ_BAR|_KZ_PGD);
		SetData(&(DWD[1].Dt2),BLACK,BLACK);
		SetStatus(&(DWD[1].St2),tLED|tSDID|tRDID|tFOCUS,tALL,_KZ_SRV,fk|fl|_KZ_TCO|_KZ_TCW|_KZ_BAR);//_KZ_TCO|_KZ_TCW|_KZ_DCO|_KZ_DCW|_KZ_BAR|_KZ_RES|_KZ_PGD);
		SetData(&(DWD[1].Dt),BLACK,BLACK);
		SetStatus(&(DWD[1].St),tLED|tSDID|tRDID|tFOCUS,tALL,_KZ_SRV,fk|fl|_KZ_TCO|_KZ_TCW|_KZ_BAR);//_KZ_TCO|_KZ_TCW|_KZ_DCO|_KZ_DCW|_KZ_BAR|_KZ_RES|_KZ_PGD);
	}
#endif
	SetPoz(_KL_ERR,0);
	SetPoz(_KL_ERR,1);
}*/
//usuwa migajacy kod bledu
void EndErr(void)
{
	ResetKol(_KL_MSG);
	ResetKol(_KL_ERR);
}
//---------------------------------------------------------
//---------------------------------------------------------
//wyswietla migajacy kod procedury awaryjnej naprzemiennie z temp. lub cisnieniem
//id - 0 - kod error, 4 - kod message
//lkod - kod led oznaczenia literowego bledu
//nkod - wartosc liczbowa bledu
//tm - dla 1 wyswietla naprzemiennie z temp.
//cs - dla 1 wyswietla naprzemiennie z cisnieniem.
//dla cs=0,tm=0 wyswietla migajacy kod z migajacym znakiem klucza
//dla cs=1,tm=1 wyswietla staly kod bez znaku klucza
void PrintErrMsg(const unsigned char id, const unsigned char lkod,unsigned char nkod,unsigned char tm,unsigned char cs, unsigned char res)
{
	unsigned int fk,fl,gk,gl,fm;


	fk=(tm&&cs)?_KZ_SRV:0;
	fl=(tm&&cs)?0:_KZ_SRV;

	gl=(!res)?0:_KZ_RES;
	gk=(res)?0:_KZ_RES;

	fm=(fl||gl)?tSDID:0;

#if LCD4D==0
	if(nkod>0xf) nkod=0xf;
	SetData(&(DWD[id].Dt),lkod,Znak[nkod]);
	SetStatus(&(DWD[id].St),tLED|fm|tRDID,tALL,fl|gl,fk|gk|_KZ_TCO|_KZ_BAR);

	if(tm)		//naprzemiennie z temperatura
	{
		if(!PFNf_fL3)		//aktywna blokada L3?
		{
			if(!PFNf_errCO&&!PFNf_oflato)
			{
				SetData(&(DWD[id+1].Dt),0,PFN.CO);
				SetStatus(&(DWD[id+1].St),tDEC|tSDID|tRDID|tFOCUS,tALL,_KZ_TCO,_KZ_RES|_KZ_SRV|_KZ_BAR);
			}
			else
			{
				SetStatus(&(DWD[id+1].St),tLED|tRDID|tFOCUS,tALL,0,_KZ_RES|_KZ_SRV|_KZ_TCO|_KZ_BAR);
				if(!PFNf_oflato) SetData(&(DWD[id+1].Dt),KRESKA,KRESKA);
				else SetData(&(DWD[id+1].Dt),BLACK,BLACK);
			}
		}
		else
		{
			SetStatus(&(DWD[id+1].St),tLED|tRDID|tFOCUS,tALL,0,_KZ_RES|_KZ_SRV|_KZ_BAR|_KZ_TCO);
			SetData(&(DWD[id+1].Dt),led_L,led_3);
		}
	}
	else			
	if(cs)		//naprzemiennie z cisnieniem
	{
		if(!PFNf_errCS&&!PFNf_fnoCS)
		{
			SetData(&(DWD[id+1].Dt),0,PFN.CS);
			SetStatus(&(DWD[id+1].St),tDEC|tKROPKA1|tSDID|tRDID|tFOCUS,tALL,_KZ_BAR,_KZ_RES|_KZ_SRV|_KZ_TCO);
		}
		else
		{
			SetStatus(&(DWD[id+1].St),tLED|tRDID|tFOCUS,tALL,0,_KZ_RES|_KZ_SRV|_KZ_TCO|_KZ_BAR);
			SetData(&(DWD[id+1].Dt),KRESKA,KRESKA);
		}
	}
	else
	{
		SetStatus(&(DWD[id+1].St),tLED|tRDID,tALL,0,_KZ_RES|_KZ_SRV|_KZ_TCO|_KZ_BAR);
		if(tm==1&&cs==1)//staly kod bez znaku klucza
		{
			SetData(&(DWD[id+1].Dt),lkod,Znak[nkod]);
		}
		else
		if(!tm&&!cs)	//migajacy kod ze znakiem klucza
		{
			SetData(&(DWD[id+1].Dt),BLACK,BLACK);
		}
	}	
#else
	SetData(&(DWD[id].Dt2),BLACK,lkod);		//kod E
	SetData(&(DWD[id].Dt),0,nkod);			//nr bledu
	SetStatus(&(DWD[id].St2),tLED|fm|tRDID|tFOCUS,tALL,fl|gl,fk|gk|_KZ_TCO|_KZ_TCW|_KZ_BAR);
	SetStatus(&(DWD[id].St),tDEC|fm|tRDID|tFOCUS,tALL,fl|gl,fk|gk|_KZ_TCO|_KZ_TCW|_KZ_BAR);
	if((tm&&!cs)||(cs&&!tm))
	{	
		if(!PFNf_off)
		{
			if(PFNf_obgCW)
			{
				if(!PFNf_errCW)
				{
					SetStatus(&(DWD[id+1].St2),tDEC|tSDID|tRDID|tFOCUS,tALL,_KZ_TCW,_KZ_RES|_KZ_SRV);
					SetData(&(DWD[id+1].Dt2),0,PFN.CW);
				}
				else
				{
					SetStatus(&(DWD[id+1].St2),tLED|tRDID|tFOCUS,tALL,0,_KZ_RES|_KZ_SRV|_KZ_TCW);
					SetData(&(DWD[id+1].Dt2),KRESKA,KRESKA);
				}
			}
			else
			{
					SetStatus(&(DWD[id+1].St2),tWYGAS|tRDID,tALL,0,_KZ_TCW);			
			}
		}
		else
		{
			SetStatus(&(DWD[id+1].St2),tWYGAS|tRDID,tALL,0,_KZ_TCW);
		}
		if(tm)		//naprzemiennie z temperatura
		{
			if(!PFNf_off)
			{
				if(!PFNf_obgCW)
				{
					if(!PFNf_fL3)		//aktywna blokada L3?
					{
						
						if(!PFNf_errCO&&!PFNf_oflato)
						{
                            if(PFNf_obgCO)
                            {
                                SetStatus(&(DWD[id+1].St),tDEC|tSDID|tRDID|tFOCUS,tALL,_KZ_TCO,_KZ_BAR);//_KZ_SRV|_KZ_TCO|_KZ_TCW|_KZ_DCO|_KZ_DCW|_KZ_BAR|_KZ_PGD);
                                SetData(&(DWD[id+1].Dt),0,PFN.CO);
                            }
                            else
                            {
                                SetStatus(&(DWD[id+1].St),tLED|tRDID|tFOCUS,tALL,0,_KZ_TCO|_KZ_BAR);//_KZ_SRV|_KZ_TCO|_KZ_TCW|_KZ_DCO|_KZ_DCW|_KZ_BAR|_KZ_PGD);
                                SetData(&(DWD[id+1].Dt),BLACK,BLACK);
                            }
						}
						else
						{
							SetStatus(&(DWD[id+1].St),tLED|tRDID|tFOCUS,tALL,0,_KZ_TCO|_KZ_BAR);//_KZ_SRV|_KZ_TCO|_KZ_TCW|_KZ_DCO|_KZ_DCW|_KZ_BAR|_KZ_PGD);
							if(!PFNf_oflato) SetData(&(DWD[id+1].Dt),KRESKA,KRESKA);
							else SetData(&(DWD[id+1].Dt),BLACK,BLACK);
						}
					}
					else
					{
						SetStatus(&(DWD[id+1].St),tLED|tRDID|tFOCUS,tALL,0,_KZ_BAR|_KZ_TCO);
						SetData(&(DWD[id+1].Dt),led_L,led_3);
					}
				}
				else
				{
					SetStatus(&(DWD[id+1].St),tWYGAS|tRDID,tALL,0,_KZ_BAR|_KZ_TCO);
				}
			}
			else
			{
				SetStatus(&(DWD[id+1].St),tWYGAS|tRDID,tALL,0,_KZ_BAR|_KZ_TCO);
			}
		}
		else
		if(cs)		//naprzemiennie z cisnieniem
		{
			if(!PFNf_errCS&&!PFNf_fnoCS)
			{
				SetStatus(&(DWD[id+1].St),tDEC|tKROPKA1|tSDID|tRDID|tFOCUS,tALL,_KZ_BAR,_KZ_TCO);			//cisnienie
				SetData(&(DWD[id+1].Dt),0,PFN.CS);
			}
			else
			{
				SetStatus(&(DWD[id+1].St),tLED|tRDID|tFOCUS,tALL,0,_KZ_TCO|_KZ_BAR);
				SetData(&(DWD[id+1].Dt),KRESKA,KRESKA);
			}
		}
	}
	else
	{
		SetStatus(&(DWD[id+1].St2),tLED|tRDID|tFOCUS,tALL,0,_KZ_RES|_KZ_SRV|_KZ_TCO|_KZ_TCW|_KZ_BAR);
		if(tm==1&&cs==1)//staly kod bez znaku klucza
		{
			SetStatus(&(DWD[id+1].St),tDEC|tRDID|tFOCUS,tALL,0,_KZ_RES|_KZ_SRV|_KZ_TCO|_KZ_TCW|_KZ_BAR);
			SetData(&(DWD[id+1].Dt2),BLACK,lkod);		//kod E
			SetData(&(DWD[id+1].Dt),0,nkod);			//nr bledu
	
		}
		else
		if(!tm&&!cs)	//migajacy kod ze znakiem klucza
		{
			SetStatus(&(DWD[id+1].St),tLED|tRDID|tFOCUS,tALL,0,_KZ_RES|_KZ_SRV|_KZ_TCO|_KZ_TCW|_KZ_BAR);
			SetData(&(DWD[id+1].Dt2),BLACK,BLACK);
			SetData(&(DWD[id+1].Dt),BLACK,BLACK);
		}
	}
#endif
	if(!id)
	{
		SetPoz(_KL_ERR,0);
		SetPoz(_KL_ERR,1);
	}
	else
	{
		SetPoz(_KL_MSG,0);
		SetPoz(_KL_MSG,1);
	}
}
/*
void PrintMsg(const unsigned char lkod,unsigned char nkod)
{
	unsigned int fk,fl,fm;
	fm=(nkod==1||nkod==3||nkod==5)?0:tSDID;
	fl=(!fm)?0:_KZ_SRV;

#if LCD4D==0
	if(nkod>0xf) nkod=0xf;
	SetData(&(DWD[4].Dt),lkod,Znak[nkod]);
	SetStatus(&(DWD[4].St),tLED|fm|tRDID,tALL,fl,_KZ_TCO|_KZ_BAR);
	if(nkod!=7&&nkod!=9)
	{
		SetData(&&(DWD[5].Dt),lkod,Znak[nkod]);
		SetStatus(&(DWD[5].St),tLED|tRDID,tALL,0,fl|_KZ_TCO|_KZ_BAR);
	}
	else
	{
		if(nkod==7&&!PFNf_errCO)		//naprzemiennie z temperatura
		{
			SetData(&&(DWD[5].Dt),0,tm);
			SetStatus(&(DWD[5].St),tDEC|tSDID|tRDID|tFOCUS,tALL,_KZ_TCO,fl|_KZ_BAR);
		}
		else			
		if(nkod==9&&!PFNf_errCS)		//naprzemiennie z cisnieniem
		{
			SetData(&&(DWD[5].Dt),0,cs);
			SetStatus(&(DWD[5].St),tDEC|tKROPKA1|tSDID|tRDID|tFOCUS,tALL,_KZ_BAR,fl|_KZ_TCO);
		}
		else
		{
			SetData(&&(DWD[5].Dt),lkod,Znak[nkod]);
			SetStatus(&(DWD[5].St),tLED|tRDID,tALL,0,fl|_KZ_TCO|_KZ_BAR);
		}	
	}
#else
	SetData(&(DWD[4].Dt2),BLACK,lkod);		//kod E
	SetData(&(DWD[4].Dt),0,nkod);			//nr bledu
	SetStatus(&(DWD[4].St2),tLED|fm|tRDID|tFOCUS,tALL,fl,_KZ_TCO|_KZ_TCW|_KZ_BAR);
	SetStatus(&(DWD[4].St),tDEC|fm|tRDID|tFOCUS,tALL,fl,_KZ_TCO|_KZ_TCW|_KZ_BAR);	
	if(nkod!=7&&nkod!=9)
	{
		SetData(&(DWD[5].Dt2),BLACK,lkod);
		SetData(&(DWD[5].Dt),0,nkod);
		SetStatus(&(DWD[5].St2),tLED|tRDID|tFOCUS,tALL,0,fl|_KZ_TCO|_KZ_TCW|_KZ_BAR);
		SetStatus(&(DWD[5].St),tDEC|tRDID|tFOCUS,tALL,0,fl|_KZ_TCO|_KZ_TCW|_KZ_BAR);
	}
	else
	{
		if(!PFNf_errCW)
		{
			SetStatus(&(DWD[5].St2),tDEC|tSDID|tRDID|tFOCUS,tALL,_KZ_TCW,fl);
			SetData(&(DWD[5].Dt2),0,PFN.CW);
		}
		else
		{
			SetStatus(&(DWD[5].St2),tLED|tRDID|tFOCUS,tALL,0,fl|_KZ_TCW);
			SetData(&(DWD[5].Dt2),KRESKA,KRESKA);
		}
		if(nkod==7)		//naprzemiennie z temperatura
		{
			if(!PFNf_fL3)		//aktywna blokada L3?
			{
				if(!PFNf_errCO&&!PFNf_oflato)
				{
					SetStatus(&(DWD[5].St),tDEC|tSDID|tRDID|tFOCUS,tALL,_KZ_TCO,_KZ_BAR);//_KZ_SRV|_KZ_TCO|_KZ_TCW|_KZ_DCO|_KZ_DCW|_KZ_BAR|_KZ_PGD);
					SetData(&(DWD[5].Dt),0,PFN.CO);
				}
				else
				{
					SetStatus(&(DWD[5].St),tLED|tRDID|tFOCUS,tALL,0,_KZ_TCO|_KZ_BAR);//_KZ_SRV|_KZ_TCO|_KZ_TCW|_KZ_DCO|_KZ_DCW|_KZ_BAR|_KZ_PGD);
					if(!PFNf_oflato) SetData(&(DWD[5].Dt),KRESKA,KRESKA);
					else SetData(&(DWD[5].Dt),BLACK,BLACK);
				}
			}
			else
			{
				SetStatus(&(DWD[5].St),tLED|tRDID|tFOCUS,tALL,0,_KZ_BAR|_KZ_TCO);
				SetData(&(DWD[5].Dt),led_L,led_3);
			}
		}
		else
		if(nkod==9)		//naprzemiennie z cisnieniem
		{
			if(!PFNf_errCS&&!PFNf_fnoCS)
			{
				SetStatus(&(DWD[5].St),tDEC|tKROPKA1|tSDID|tRDID|tFOCUS,tALL,_KZ_BAR,_KZ_TCO);			//cisnienie
				SetData(&(DWD[5].Dt),0,PFN.CS);
			}
			else
			{
				SetStatus(&(DWD[5].St),tLED|tRDID|tFOCUS,tALL,0,_KZ_TCO|_KZ_BAR);
				SetData(&(DWD[5].Dt),KRESKA,KRESKA);
			}
		}
	}
#endif
	SetPoz(_KL_MSG,0);
	SetPoz(_KL_MSG,1);
}*/
//---------------------------------------------------------
//---------------------------------------------------------

/*
//---------------------------------------------------------
//---------------------------------------------------------
//Kod b³edu naprzemiennie z cisnieniem
//lkod - kod led oznaczenia literowego bledu
//nkod - wartosc liczbowa bledu
//tm - temperatura wskacywana naprzemiennie z kodem bledu
void PrintErrCs(const unsigned char lkod,unsigned char nkod,const unsigned char cs)
{
#if LCD4D==0
	if(nkod>0xf) nkod=0xf;
	SetData(&DWD[0],lkod,Znak[nkod]);
	SetData(&DWD[1],0,cs);
	SetStatus(&DWD[0],tLED|tRDID,tALL,0,_KZ_TCO);					//kod bledu/cisnienie
	SetStatus(&DWD[1],tDEC|tKROPKA1|tRDID,tALL,0,_KZ_TCO);					//temperatura
#else
	SetData2(&DWD[0],BLACK,lkod);
	SetStatus2(&DWD[0],tLED|tRDID|tFOCUS,tALL,0,_KZ_TCO);
	SetData(&DWD[0],0,nkod);
	SetStatus(&DWD[0],tDEC|tRDID|tFOCUS,tALL,0,_KZ_TCO);
	SetData(&DWD[1],0,cs);
	SetStatus(&DWD[1],tDEC|tKROPKA1|tRDID,tALL,0,_KZ_TCO);					//temperatura
	SetData2(&DWD[1],BLACK,BLACK);
	SetStatus2(&DWD[1],tLED,tALL,0,0);								//temperatura
	//SetStatus2(&DWD[1],tLED|tRDID,tALL,0,_KZ_TCOW);				//temperatura
#endif
	SetPoz(_WS_ERR,0);
	SetPoz(_WS_ERR,1);
}
//---------------------------------------------------------
//---------------------------------------------------------
//Kod b³edu naprzemiennie z temperatura
//lkod - kod led oznaczenia literowego bledu
//nkod - wartosc liczbowa bledu
//tm - temperatura wskacywana naprzemiennie z kodem bledu
//pls - znacznik czy ma byc wskazana temp CO (0) czy CW (1)
void PrintErrTm(const unsigned char lkod,unsigned char nkod,const unsigned char tm, const unsigned char pls)
{
#if LCD4D==0
	if(nkod>0xf) nkod=0xf;
	SetData(&DWD[0],lkod,Znak[nkod]);
	SetData(&DWD[1],0,tm);
	SetStatus(&DWD[0],tLED|tRDID,tALL,0,_KZ_TCO);					//kod bledu/cisnienie
	SetStatus(&DWD[1],tDEC|tSDID,tALL,_KZ_TCO,0);					//temperatura
#else
	SetData2(&DWD[0],BLACK,lkod);
	SetStatus2(&DWD[0],tLED|tRDID|tFOCUS,tALL,0,_KZ_TCO);
	SetData(&DWD[0],0,nkod);
	SetStatus(&DWD[0],tDEC|tRDID|tFOCUS,tALL,0,_KZ_TCO);
	if(!pls)
	{
		SetData(&DWD[1],0,tm);
		SetStatus(&DWD[1],tDEC|tSDID,tALL,_KZ_TCO,0);					//temperatura
		SetData2(&DWD[1],BLACK,BLACK);
		SetStatus2(&DWD[1],tLED,tALL,0,0);								//temperatura
		//SetStatus2(&DWD[1],tLED|tRDID,tALL,0,_KZ_TCOW);				//temperatura
	}
	else
	{
		SetData(&DWD[1],BLACK,BLACK);
		SetStatus(&DWD[1],tLED,tALL,0,0);				//temperatura
		//SetStatus(&DWD[1],tLED|tRDID,tALL,0,_KZ_TCOO);				//temperatura
		SetData2(&DWD[1],0,tm);
		SetStatus2(&DWD[1],tDEC|tSDID,tALL,_KZ_TCO,0);					//temperatura
	}
#endif
	SetPoz(_WS_ERR,0);
	SetPoz(_WS_ERR,1);
}
*/


//---------------------------------------------------------
//---------------------------------------------------------
//Wyswietla wskaz skladajacy sie z oznaczenia literowego i cyfry od 0 do 9
//WYSWIETLANIE 2 CYFROWE
//lkod - dowolny znak w kodzie led wyswietlany na starszej cyfrze
//dla tryb=0:
//nkod - liczba zakresu 0-F wyswietlana na mlodszej cyfrze
//dla tryb=1:
//nkod - dowolny znak w kodzie led wyswietlany na mlodszej cyfrze
//WYSWIETLANIE 4 CYFROWE
//lkod - dowolny znak w kodzie led wyswietlany na 3 cyfrze
//nkod - liczba zakresu 0-99 wyswietlana na 2 najmlodszych cyfrach
void PrintPR(const unsigned char lkod,unsigned char nkod,unsigned char tryb)
{
#if LCD4D==0
	if((nkod>0xf)&&!tryb) nkod=0xf;
	SetData(&(PWD[_WS_PRM].Dt),lkod,(!tryb)?Znak[nkod]:nkod);
	SetStatus(&(PWD[_WS_PRM].St),tLED|tSDID|tFOCUS,tALL,_KZ_SRV,0);
#else
	if((nkod>99)&&!tryb) nkod=99;
	SetData(&(PWD[_WS_PRM].Dt2),BLACK,lkod);
	SetStatus(&(PWD[_WS_PRM].St2),tLED|tFOCUS,tALL,0,0);
	SetData(&(PWD[_WS_PRM].Dt),0,nkod);
	SetStatus(&(PWD[_WS_PRM].St),tDEC|tSDID|tFOCUS,tALL,_KZ_SRV,0);
#endif
	//UpdateWSW();
}
//----------
//wyswietla wartosc parametru
void PrintWPR(unsigned char hi_wr,unsigned char lo_wr)
{
#if LCD4D==0
	SetData(&(PWD[_WS_PRM].Dt),0,lo_wr);
	SetStatus(&(PWD[_WS_PRM].St),tDEC|tSDID|tFOCUS,tALL,_KZ_SRV,0);
#else
	SetData(&(PWD[_WS_PRM].Dt2),0,hi_wr);//wr/100);
	SetStatus(&(PWD[_WS_PRM].St2),tDEC|tFOCUS,tALL,0,0);
	SetData(&(PWD[_WS_PRM].Dt),0,lo_wr);//%100);
	SetStatus(&(PWD[_WS_PRM].St),tDEC|tSDID|tFOCUS,tALL,_KZ_SRV,0);
#endif
	//UpdateWSW();
}
//----------
//usuwa wskaz parametru
void EndPR(void)
{
#if LCD4D==0
	SetStatus(&(PWD[_WS_PRM].St),tDEC,tALL,0,0);
#else
	SetStatus(&(PWD[_WS_PRM].St),tDEC,tALL,0,0);
	SetStatus(&(PWD[_WS_PRM].St2),tDEC,tALL,0,0);
#endif
	//UpdateWSW();
}
//---------------------------------------------------------
//---------------------------------------------------------
/*
//natychmiastowa aktualizacja wskazu nastawy
void RNst(const unsigned char nst)
{
	SetData(&(DWD[2].Dt),0,nst);							//nastawa CO/CW/Kt
	SetData(&(DWD[3].Dt),0,nst);							//nastawa CO/CW/Kt
	UpdateWSW();
}
//----------
#if LCD4D!=0
void RNst2(const unsigned char nst)
{
	SetData(&(DWD[2].Dt2),0,nst);							//nastawa CO/CW/Kt
	SetData(&(DWD[3].Dt2),0,nst);							//nastawa CO/CW/Kt
	UpdateWSW();
}
#endif
*/
//----------
//wyswietla nastawe temp. CW
void PrintNstCW(const unsigned char nst,const unsigned char only)
{
	unsigned int fl;
	fl=(!only)?0:tONL;
	SetData(&(DWD[2].Dt2),0,nst);							//nastawa CW
	SetData(&(DWD[3].Dt2),BLACK,BLACK);
	SetStatus(&(DWD[2].St2),fl|tDEC|tSDID|tFOCUS,tALL,_KZ_TCW|_KZ_DCW,0);
	SetStatus(&(DWD[3].St2),fl|tLED|tRDID,tALL,0,_KZ_DCW|_KZ_TCW);
	MarkMinMax(1,0);
#if LCD4D!=0
	if(!only)
	{
		SetStatus(&(DWD[2].St),tWYGAS|tRDID,tALL,0,_KZ_PGD|_KZ_TCO|_KZ_BAR);
		SetStatus(&(DWD[3].St),tWYGAS|tRDID,tALL,0,_KZ_PGD|_KZ_TCO|_KZ_BAR);
	}
#endif
	RestartKol1(_KL_NCWCO);
}
void ClrMarkMinMax(void);
void PrintNstCW2(const unsigned char nst,const unsigned char min, const unsigned char only)
{
	unsigned int fl;
	fl=(!only)?0:tONL;
	if(nst>min)
	{
		SetData(&(DWD[2].Dt2),0,nst);							//nastawa CW
		SetData(&(DWD[3].Dt2),BLACK,BLACK);
		SetStatus(&(DWD[2].St2),fl|tDEC|tSDID|tFOCUS,tALL,_KZ_TCW|_KZ_DCW,0);
		SetStatus(&(DWD[3].St2),fl|tLED|tRDID,tALL,0,_KZ_DCW|_KZ_TCW);
		MarkMinMax(1,0);
	}
	else
	{
		SetData(&(DWD[2].Dt2),KRESKA,KRESKA);							//nastawa CW
		SetData(&(DWD[3].Dt2),BLACK,BLACK);
		SetStatus(&(DWD[2].St2),fl|tLED|tSDID|tRDID|tFOCUS,tALL,_KZ_DCW,_KZ_TCW);
		SetStatus(&(DWD[3].St2),fl|tLED|tSDID|tRDID,tALL,_KZ_DCW,_KZ_TCW);
		ClrMarkMinMax();
	}
#if LCD4D!=0
	if(!only)
	{
		SetStatus(&(DWD[2].St),tWYGAS|tRDID,tALL,0,_KZ_PGD|_KZ_TCO|_KZ_BAR);
		SetStatus(&(DWD[3].St),tWYGAS|tRDID,tALL,0,_KZ_PGD|_KZ_TCO|_KZ_BAR);
	}
#endif
	RestartKol1(_KL_NCWCO);
}
//----------
//wyswietla nastawe temp. CO
void PrintNstCO(const unsigned char nst,const unsigned char kt,const unsigned char only)
{
	unsigned int fl,fk,fm,fn;
	fl=(!only)?0:tONL;
	fk=(!kt)?0:tKROPKA1;
	fm=(!kt)?_KZ_TCO:_KZ_PGD;
    fn=(!kt)?_KZ_PGD:_KZ_TCO;

	SetData(&(DWD[2].Dt),0,nst);							//nastawa CO,Kt
	SetData(&(DWD[3].Dt),BLACK,BLACK);						//wygaszenie
	SetStatus(&(DWD[2].St),fl|fk|tDEC|tSDID|tRDID|tFOCUS,tALL,fm|_KZ_DCO,fn|_KZ_BAR);
	SetStatus(&(DWD[3].St),fl|tLED|tRDID,tALL,0,_KZ_PGD|_KZ_TCO|_KZ_DCO|_KZ_BAR);
	MarkMinMax(0,1);
#if LCD4D!=0
	if(!only)
	{	
		SetStatus(&(DWD[2].St2),tWYGAS|tRDID,tALL,0,_KZ_TCW);
		SetStatus(&(DWD[3].St2),tWYGAS|tRDID,tALL,0,_KZ_TCW);
	}
#endif
	RestartKol1(_KL_NCWCO);
}
/*void PrintNstCO(const unsigned char nst,const unsigned char kt,const unsigned char only)
{
	unsigned int fl,fk,fm,fn,fo;
	fl=(!only)?0:tONL;
	fk=(!kt)?0:tKROPKA1;
	fm=(!kt)?_KZ_TCO:_KZ_PGD;
	fn=(!kt)?0:_KZ_TCO;
	fo=(!kt)?0:_KZ_PGD;
	SetData(&(DWD[2].Dt),0,nst);							//nastawa CO
	SetData(&(DWD[3].Dt),BLACK,BLACK);						//nastawa Kt
	SetStatus(&(DWD[2].St),fl|fk|tDEC|tSDID|tRDID|tFOCUS,tALL,fm|_KZ_DCO,fn|_KZ_BAR);
	SetStatus(&(DWD[3].St),fl|tLED|tRDID,tALL,0,fo|_KZ_TCO|_KZ_DCO|_KZ_BAR);
	MarkMinMax(0,1);
#if LCD4D!=0
	if(!only)
	{	
		SetStatus(&(DWD[2].St2),tWYGAS|tRDID,tALL,0,_KZ_TCW);
		SetStatus(&(DWD[3].St2),tWYGAS|tRDID,tALL,0,_KZ_TCW);
	}
#endif
	RestartKol1(_KL_NCWCO);
}*/
//----------
//wyswietla nastawe ECO
void PrintNstECO(const unsigned char nst)
{
	//unsigned int fl,fk,fm,fn,fo;

	SetData(&(DWD[2].Dt),0,nst);							//wsp.ECO
	SetData(&(DWD[3].Dt),BLACK,BLACK);						//Ec
	SetData(&(DWD[2].Dt2),led_E,led_c);							//wsp.ECO
	SetData(&(DWD[3].Dt2),BLACK,BLACK);						//Ec
	//SetData(&(DWD[3].Dt2),led_E,led_c);
	SetStatus(&(DWD[2].St),tDEC|tKROPKA1|tSDID|tRDID|tFOCUS,tALL,_KZ_PGD|_KZ_DCO,_KZ_TCO|_KZ_BAR);
	SetStatus(&(DWD[3].St),tWYGAS|tRDID,tALL,0,_KZ_PGD|_KZ_TCO|_KZ_DCO|_KZ_BAR);
	SetStatus(&(DWD[2].St2),tLED|tRDID,tALL,0,_KZ_TCW);
	SetStatus(&(DWD[3].St2),tWYGAS|tRDID,tALL,0,_KZ_TCW);
	//SetStatus(&(DWD[3].St2),tLED|tRDID,tALL,0,_KZ_TCW);
	MarkMinMaxECO();
	RestartKol1(_KL_NCWCO);
}
//----------
//usuwa wskaz nastawy CW/CO
void EndNst(void)
{
	ResetKol(_KL_NCWCO);
}
//---------------------------------------------------------
//---------------------------------------------------------
//wyswietla kod L3 naprzemiennie z temperatura CO
/*
void PrintL3(void)
{
	SetData(&DWD[5],led_L,led_3);
	SetStatus(&DWD[5],tLED|tFOCUS,tALL);				//L
	if(DtCO._err_CO)
	{
		SetData(&DWD[6],led_L,led_3);
		SetStatus(&DWD[6],tLED|tFOCUS,tALL);				//3
	}
	else
	{
		SetData(&DWD[6],0,DtCO.SCO);
		SetStatus(&DWD[6],tDEC|tFOCUS,tALL);				//3
	}
	SetPoz(_KL_MSG,0);
	SetPoz(_KL_MSG,1);
}
//----------
//usuwa wskaz komunikatu
void EndL3(void)
{
	ResetKol(_KL_MSG);
}
*/
//---------------------------------------------------------
//---------------------------------------------------------
//Wysterowuje znak z 
//dla s=0 wygaszenie
//dla s=1 zapalenie
//dla s>1 pulsacja
void Mark(unsigned char z,unsigned char s)
{
	SetMark(&KMark[z],1,s>1,s);		//zapal znak
	//UpdateWSW();
}
//---------------------------------------------------------
//---------------------------------------------------------
//wysterowanie znakow dla funkcji serwisowej
/*void MarkSVAL(unsigned char s,unsigned char mx,unsigned al)
{
	//static unsigned char bs;

	//Mark(_GZ_SRV,s);
	if(s)
	{
		if(!al)
		{
			Mark(_GZ_MXCO,mx);
			Mark(_GZ_MNCO,!mx);
		}
		else
		{
			Mark(_GZ_MXCW,1);	//zapal max dla antylegionella
		}
	}
	else
	{
		//if(bs!=s)
		{
			Mark(_GZ_MXCO,0);
			Mark(_GZ_MNCO,0);
			Mark(_GZ_MXCW,0);
		}
	}
	//bs=s;
}*/
void ClrMarkMinMax(void)
{
	Mark(_GZ_MXCO,0);
	Mark(_GZ_MNCO,0);
	Mark(_GZ_MXCW,0);
	Mark(_GZ_MNCW,0);
}
//---------------------------------------------------------
//---------------------------------------------------------
//---------------------------------------------------------
//---------------------------------------------------------
//---------------------------------------------------------
//---------------------------------------------------------
//wyswietla cisnienie CS
void PrintCS(const unsigned char cs,const unsigned char only)
{
	unsigned int fl;
	fl=(!only)?0:tONL;
#if LCD4D==0
	SetStatus(&(PWD[_WS_CS].St),tDEC|tKROPKA1|tSDID|tRDID|tFOCUS,tALL,_KZ_BAR,_KZ_TCO);			//| _KZ_DCOcisnienie
	SetData(&(PWD[_WS_CS].Dt),0,cs);
#else
    if(!PFNf_errCS&&!PFNf_fnoCS)
    {
        SetStatus(&(PWD[_WS_CS].St),fl|tDEC|tKROPKA1|tSDID|tRDID|tFOCUS,tALL,_KZ_BAR,_KZ_TCO);			//| _KZ_DCOcisnienie
        SetData(&(PWD[_WS_CS].Dt),0,cs);
    }
    else
    {
        SetStatus(&(PWD[_WS_CS].St),fl|tLED|tSDID|tRDID|tFOCUS,tALL,_KZ_BAR,_KZ_TCO);	//tempetratura IN
        SetData(&(PWD[_WS_CS].Dt),KRESKA,KRESKA);        
    }
	if(!only) SetStatus(&(PWD[_WS_CS].St2),tWYGAS|tFOCUS,tALL,0,0);
#endif
	//UpdateWSW();
}
//---------------------------------------------------------
//---------------------------------------------------------
//wyswietla temperature CW gdy aktywne grznie w obiegu CW
//ustawia diode grzania CW w stan aktywny
void PrintCW(const unsigned char tm,const unsigned char only)
{
	unsigned int fl;
	fl=(!only)?0:tONL;
#if LCD4D==0
	SetStatus(&(PWD[_WS_TCW].St),tDEC|tSDID|tFOCUS,tALL,_KZ_TCW,0);//|_KZ_DCW,0);
	SetData(&(PWD[_WS_TCW].Dt),0,tm);						//tempetratura CW
#else
	SetStatus(&(PWD[_WS_TCW].St2),fl|tDEC|tSDID|tFOCUS,tALL,_KZ_TCW,0);//|_KZ_DCW,0);
	SetData(&(PWD[_WS_TCW].Dt2),0,tm);						//tempetratura CW
	if(!only) SetStatus(&(PWD[_WS_TCW].St),tWYGAS|tFOCUS,tALL,0,0);
#endif
	//UpdateWSW();
}
//---------------------------------------------------------
//---------------------------------------------------------
//wyswietla wersje oprogramowania
//gdzie 
//board- kolejny nr plytki sterujacej (format 0xbn - gdzie n to nr od 1 do 9)
//ver - wersja oprogramowania dla plytki o podanym numerze
void PrintVER(const unsigned char board, const signed char ver,const unsigned char only)
{
	SetStatus(&(PWD[_WS_TCW].St2),tHEX|tFOCUS,tALL,0,0);
	SetData(&(PWD[_WS_TCW].Dt2),0,board);
    if(ver>=0)
    {
        SetStatus(&(PWD[_WS_TCO].St),tDEC|tFOCUS,tALL,0,0);
        SetData(&(PWD[_WS_TCO].Dt),0,ver);	//wskaz bez kropki (dla wersji 1 wskaz 01)
    }
    else
    {
        SetStatus(&(PWD[_WS_TCO].St),tLED|tFOCUS,tALL,0,0);	
        SetData(&(PWD[_WS_TCO].Dt),KRESKA,KRESKA);  //w miejscu wersji dwie kreski      
    }
	if(!only) SetStatus(&(PWD[_WS_TCW].St),tWYGAS|tFOCUS,tALL,0,0);
}
//---------------------------------------------------------
//---------------------------------------------------------
//wyswietla predkosc obrotowa gdy pracuje wentylator
void PrintVNT(const unsigned char vn,const unsigned char only)
{
	unsigned int fl;
	fl=(!only)?0:tONL;
#if LCD4D==0
	SetStatus(&(PWD[_WS_TCW].St),tDEC|tRDID|tFOCUS,tALL,0,_KZ_TCW);//|_KZ_DCW,0);
	SetData(&(PWD[_WS_TCW].Dt),0,vn);						//tempetratura CW
#else
	SetStatus(&(PWD[_WS_TCW].St2),fl|tDEC|tRDID|tFOCUS,tALL,0,_KZ_TCW);//|_KZ_DCW,0);
	SetData(&(PWD[_WS_TCW].Dt2),0,vn);						//tempetratura CW
	if(!only) SetStatus(&(PWD[_WS_TCW].St),tWYGAS|tFOCUS,tALL,0,0);
#endif
}
//---------------------------------------------------------
//---------------------------------------------------------
//Wskazy o najnizszym priorytecie wyswietlania
//wyswietla temperature CO
#if _PRNL3==0
void PrintCO(const unsigned char tm,const unsigned char only, const unsigned char l3)
{
	unsigned int fl;
	fl=(!only)?0:tONL;
	#if LCD4D==0
		SetStatus(&(PWD[_WS_TCO].St),tDEC|tSDID|tRDID|tFOCUS,tALL,_KZ_TCO,_KZ_BAR);
		SetData(&(PWD[_WS_TCO].Dt),0,tm);													//tempetratura CO
	#else
		SetStatus(&(PWD[_WS_TCO].St),fl|tDEC|tSDID|tRDID|tFOCUS,tALL,_KZ_TCO,_KZ_BAR);
		SetData(&(PWD[_WS_TCO].Dt),0,tm);												//tempetratura CO
		if(!only) SetStatus(&(PWD[_WS_TCO].St2),tWYGAS|tFOCUS,tALL,0,0);
	#endif
}
#else
void PrintCO(const unsigned char tm,const unsigned char only, const unsigned char l3)
{
	unsigned int fl;
	fl=(!only)?0:tONL;
	if(!l3)		//aktywna blokada L3?
	{
	#if LCD4D==0
		SetStatus(&(PWD[_WS_TCO].St),tDEC|tSDID|tRDID|tFOCUS,tALL,_KZ_TCO,_KZ_BAR);
		SetData(&(PWD[_WS_TCO].Dt),0,tm);													//tempetratura CO
	#else
		SetStatus(&(PWD[_WS_TCO].St),fl|tDEC|tSDID|tRDID|tFOCUS,tALL,_KZ_TCO,_KZ_BAR);
		SetData(&(PWD[_WS_TCO].Dt),0,tm);												//tempetratura CO
		if(!only) SetStatus(&(PWD[_WS_TCO].St2),tWYGAS|tFOCUS,tALL,0,0);
	#endif
	}
	else
	{
	#if LCD4D==0
		SetStatus(&(PWD[_WS_TCO].St),tLED|tRDID|tFOCUS,tALL,0,_KZ_BAR|_KZ_TCO);
		SetData(&(PWD[_WS_TCO].Dt),led_L,led_3);										//kod L3
	#else
		SetStatus(&(PWD[_WS_TCO].St),fl|tLED|tRDID|tFOCUS,tALL,0,_KZ_BAR|_KZ_TCO);
		SetData(&(PWD[_WS_TCO].Dt),led_L,led_3);										//kod L3
		if(!only) SetStatus(&(PWD[_WS_TCO].St2),tWYGAS|tFOCUS,tALL,0,0);
	#endif
	}
}
#endif
//---------------------------------------------------------
//---------------------------------------------------------
//wyswietla temperature IN 
void PrintIN(const unsigned char tm)
{
#if LCD4D==0
	SetStatus(&(PWD[_WS_TCO].St),tDEC|tSDID|tRDID|tFOCUS,tALL,_KZ_TCO,_KZ_BAR);
	SetData(&(PWD[_WS_TCO].Dt),0,tm);												//tempetratura IN
#else
	SetStatus(&(PWD[_WS_TCW].St2),tLED|tRDID|tFOCUS,tALL,0,_KZ_TCW);
	SetData(&(PWD[_WS_TCW].Dt2),led_I,led_n);
    if(!PFNf_errIN)
    {
        SetStatus(&(PWD[_WS_TCO].St),tDEC|tSDID|tRDID|tFOCUS,tALL,_KZ_TCO,_KZ_BAR);	//tempetratura IN
        SetData(&(PWD[_WS_TCO].Dt),0,tm);
    }
    else
    {
        SetStatus(&(PWD[_WS_TCO].St),tLED|tRDID|tFOCUS,tALL,0,_KZ_BAR|_KZ_TCO);	//tempetratura IN
        SetData(&(PWD[_WS_TCO].Dt),KRESKA,KRESKA);        
    }
#endif
}
//---------------------------------------------------------
//---------------------------------------------------------
//wyswietla wartosc PP 
void PrintPP(unsigned char pp)
{
	if(pp>99) pp=99;
#if LCD4D==0
	SetStatus(&(PWD[_WS_TCO].St),tDEC|tSDID|tRDID|tFOCUS,tALL,_KZ_TCO,_KZ_BAR);
	SetData(&(PWD[_WS_TCO].Dt),0,pp);												
#else
	SetStatus(&(PWD[_WS_TCW].St2),tLED|tRDID|tFOCUS,tALL,0,_KZ_TCW);
	SetData(&(PWD[_WS_TCW].Dt2),led_P,led_r);
    if(PFNf_pmpPWM)
    {
        SetStatus(&(PWD[_WS_TCO].St),tDEC|tRDID|tFOCUS,tALL,0,_KZ_BAR|_KZ_TCO);	
        SetData(&(PWD[_WS_TCO].Dt),0,pp);
    }
    else
    {
        SetStatus(&(PWD[_WS_TCO].St),tLED|tRDID|tFOCUS,tALL,0,_KZ_BAR|_KZ_TCO);	//tempetratura IN
        SetData(&(PWD[_WS_TCO].Dt),KRESKA,KRESKA);        
    }
#endif
}
//---------------------------------------------------------
//---------------------------------------------------------
//wyswietla wartosc PG 
void PrintPG(unsigned char pg)
{
	if(pg>99) pg=99;
#if LCD4D==0
	SetStatus(&(PWD[_WS_TCO].St),tDEC|tSDID|tRDID|tFOCUS,tALL,_KZ_TCO,_KZ_BAR);
	SetData(&(PWD[_WS_TCO].Dt),0,pg);												//tempetratura IN
#else
	SetStatus(&(PWD[_WS_TCW].St2),tLED|tRDID|tFOCUS,tALL,0,_KZ_TCW);
	SetData(&(PWD[_WS_TCW].Dt2),led_F,led_L);						
	SetStatus(&(PWD[_WS_TCO].St),tDEC|tRDID|tFOCUS,tALL,0,_KZ_BAR|_KZ_TCO);	
	SetData(&(PWD[_WS_TCO].Dt),0,pg);
#endif
}
//---------------------------------------------------------
//---------------------------------------------------------
//wyswietla temperature CO w trakcie grzania CW po nacisnieciu przycisku RESET 
void PrintCo(const unsigned char tm)
{
#if LCD4D==0
	SetStatus(&(PWD[_WS_TCO].St),tDEC|tSDID|tRDID|tFOCUS,tALL,_KZ_TCO,_KZ_BAR);
	SetData(&(PWD[_WS_TCO].Dt),0,tm);												//tempetratura IN
#else
	SetStatus(&(PWD[_WS_TCW].St2),tLED|tRDID|tFOCUS,tALL,0,_KZ_TCW);
	SetData(&(PWD[_WS_TCW].Dt2),led_C,led_h);	
    if(!PFNf_errCO)
    {
        SetStatus(&(PWD[_WS_TCO].St),tDEC|tSDID|tRDID|tFOCUS,tALL,_KZ_TCO,_KZ_BAR);	//tempetratura IN
        SetData(&(PWD[_WS_TCO].Dt),0,tm);
    }
    else
    {
        SetStatus(&(PWD[_WS_TCO].St),tLED|tRDID|tFOCUS,tALL,0,_KZ_BAR|_KZ_TCO);	//tempetratura IN
        SetData(&(PWD[_WS_TCO].Dt),KRESKA,KRESKA);        
    }
#endif
}
//---------------------------------------------------------
//---------------------------------------------------------
//wyswietla symbol Po i cisnienie CS
void PrintPo(const unsigned char cs)
{
#if LCD4D==0
	SetStatus(&(PWD[_WS_CS].St),tDEC|tKROPKA1|tSDID|tRDID|tFOCUS,tALL,_KZ_BAR,_KZ_TCO);			//| _KZ_DCOcisnienie
	SetData(&(PWD[_WS_CS].Dt),0,cs);
#else
	SetStatus(&(PWD[_WS_TCW].St2),tLED|tRDID|tFOCUS,tALL,0,_KZ_TCW);
	SetData(&(PWD[_WS_TCW].Dt2),led_P,led_o);    
    if(!PFNf_errCS&&!PFNf_fnoCS)
    {
        SetStatus(&(PWD[_WS_CS].St),tDEC|tKROPKA1|tSDID|tRDID|tFOCUS,tALL,_KZ_SRV|_KZ_BAR,_KZ_TCO);			//| _KZ_DCOcisnienie
        SetData(&(PWD[_WS_CS].Dt),0,cs);
    }
    else
    {
        SetStatus(&(PWD[_WS_CS].St),tLED|tSDID|tRDID|tFOCUS,tALL,_KZ_SRV|_KZ_BAR,_KZ_TCO);	//tempetratura IN
        SetData(&(PWD[_WS_CS].Dt),KRESKA,KRESKA);        
    }
#endif
}
//---------------------------------------------------------
//---------------------------------------------------------
//wyswietla adres LIN
void PrintLN(const unsigned char laddr)
{
#if LCD4D==0
	SetStatus(&(PWD[_WS_TCO].St),tDEC|tSDID|tRDID|tFOCUS,tALL,_KZ_TCO,_KZ_BAR);
	SetData(&(PWD[_WS_TCO].Dt),0,tm);												//tempetratura IN
#else
	SetStatus(&(PWD[_WS_TCW].St2),tLED|tRDID|tFOCUS,tALL,0,_KZ_TCW);
	SetData(&(PWD[_WS_TCW].Dt2),led_L,led_n);
    if(laddr)
    {
        SetStatus(&(PWD[_WS_TCO].St),tDEC|tRDID|tFOCUS,tALL,0,_KZ_BAR|_KZ_TCO);	//tempetratura IN
        SetData(&(PWD[_WS_TCO].Dt),0,laddr);
    }
    else
    {
        SetStatus(&(PWD[_WS_TCO].St),tLED|tRDID|tFOCUS,tALL,0,_KZ_BAR|_KZ_TCO);	//tempetratura IN
        SetData(&(PWD[_WS_TCO].Dt),KRESKA,KRESKA);
    }
#endif
}
//---------------------------------------------------------
//---------------------------------------------------------
//wyswietla temperature IN 
/*
void PrintEC(const unsigned char ec)
{
#if LCD4D==0
	SetStatus(&(PWD[_WS_TCO].St),tDEC|tRDID|tFOCUS,tALL,0,_KZ_TCO|_KZ_BAR);
	SetData(&(PWD[_WS_TCO].Dt),0,ec);												//tempetratura IN
#else
	SetStatus(&(PWD[_WS_TCW].St2),tLED|tRDID|tFOCUS,tALL,0,_KZ_TCW);
	SetData(&(PWD[_WS_TCW].Dt2),led_I,led_n);						
	SetStatus(&(PWD[_WS_TCO].St),tDEC|tSDID|tRDID|tFOCUS,tALL,,_KZ_TCO|_KZ_BAR);	//tempetratura IN
	SetData(&(PWD[_WS_TCO].Dt),0,tm);
#endif
}
*/
//---------------------------------------------------------
//---------------------------------------------------------
//Wygasza wybrany wskaz wraz z powiazanymi z nim znacznikami
//gdzie dhigh - wskazuje na dwie starsze cyfry wraz ze znakami
//dlow - wskazuje na dwie mlodsze cyfry wraz ze znakami
//krs - zamiast cyfr zapalane s¹ dwie kreski
/*
void ClrDig(const unsigned char dhigh, const unsigned char dlow, const unsigned char krs)
{
#if LCD4D==0
	if(dlow)
	{
		if(!krs) SetStatus(&(PWD[_WS_TCO].St),tWYGAS|tRDID|tFOCUS,tALL,0,_KZ_TCO|_KZ_DCO|_KZ_BAR);
		else
		{
			SetData(&(PWD[_WS_TCO].Dt),KRESKA,KRESKA);
			SetStatus(&(PWD[_WS_TCO].St),tLED|tRDID|tFOCUS,tALL,0,_KZ_TCO|_KZ_DCO|_KZ_BAR);
		}		
	}
#else
	if(dlow)
	{
		if(!krs) SetStatus(&(PWD[_WS_TCO].St),tWYGAS|tRDID|tFOCUS,tALL,0,_KZ_TCO|_KZ_DCO|_KZ_BAR);
		else
		{
			SetData(&(PWD[_WS_TCO].Dt),KRESKA,KRESKA);
			SetStatus(&(PWD[_WS_TCO].St),tLED|tRDID|tFOCUS,tALL,0,_KZ_TCO|_KZ_DCO|_KZ_BAR);
		}
	}
	if(dhigh)
	{
		if(!krs) SetStatus(&(PWD[_WS_TCO].St2),tWYGAS|tRDID|tFOCUS,tALL,0,_KZ_TCW|_KZ_DCW);
		else
		{
			SetData(&(PWD[_WS_TCO].Dt2),KRESKA,KRESKA);
			SetStatus(&(PWD[_WS_TCO].St2),tLED|tRDID|tFOCUS,tALL,0,_KZ_TCW|_KZ_DCW);
		}		
	}
#endif
}
*/
void ClrDig(const unsigned char dhigh, const unsigned char dlow, const unsigned char krs)
{
#if LCD4D==0
	if(dlow)
	{
		if(!krs) SetStatus(&(PWD[_WS_TCO].St),tWYGAS|tRDID|tFOCUS,tALL,0,_KZ_TCO|_KZ_BAR);
		else
		{
			SetData(&(PWD[_WS_TCO].Dt),KRESKA,KRESKA);
			SetStatus(&(PWD[_WS_TCO].St),tLED|tRDID|tFOCUS,tALL,0,_KZ_TCO|_KZ_BAR);
		}		
	}
#else
	if(dlow)
	{
		if(!krs) SetStatus(&(PWD[_WS_TCO].St),tWYGAS|tRDID|tFOCUS,tALL,0,_KZ_TCO|_KZ_BAR);
		else
		{
			SetData(&(PWD[_WS_TCO].Dt),KRESKA,KRESKA);
			SetStatus(&(PWD[_WS_TCO].St),tLED|tRDID|tFOCUS,tALL,0,_KZ_TCO|_KZ_BAR);
		}
	}
	if(dhigh)
	{
		if(!krs) SetStatus(&(PWD[_WS_TCW].St2),tWYGAS|tRDID|tFOCUS,tALL,0,_KZ_TCW);
		else
		{
			SetData(&(PWD[_WS_TCW].Dt2),KRESKA,KRESKA);
			SetStatus(&(PWD[_WS_TCW].St2),tLED|tRDID|tFOCUS,tALL,0,_KZ_TCW);
		}		
	}
#endif
	//UpdateWSW();
}
//---------------------------------------------------------
//---------------------------------------------------------
//Funkcja na potrzeby testow oprogramowania
/*
void PrintHex(const unsigned char tm)
{
	SetStatus(&(PWD[_WS_TCO].St),tHEX,tALL,0,0);				
	SetData(&(PWD[_WS_TCO].Dt),0,tm);						//tempetratura CO
}
*/
//---------------------------------------------------------
//---------------------------------------------------------
//Wysterowanie wszystkich elementow swietlnych wyswietlacza
void TestScr(void)
{
	SetStatus(&(PWD[_WS_TCO].St),tLED|tKROPKA1|tKROPKA0,tALL,0,0);				
	SetData(&(PWD[_WS_TCO].Dt),FULL,FULL);	//zapal wszystkie segmenty wyswietlacza
#if LCD4D!=0
	SetStatus(&(PWD[_WS_TCO].St2),tLED|tKROPKA1|tKROPKA0,tALL,0,0);				
	SetData(&(PWD[_WS_TCO].Dt2),FULL,FULL);	//zapal wszystkie segmenty wyswietlacza
#endif
	SetWSW(1); 							//zapal wszystkie znaki wyswietlacza
	//UpdateWSW();
}
void EndTestScr(void)
{
	SetStatus(&(PWD[_WS_TCO].St),tDEC|tWYGAS,tALL,0,0);				
	SetData(&(PWD[_WS_TCO].Dt),0,0);			//wygas wszystkie segmenty wyswietlacza
#if LCD4D!=0
	SetStatus(&(PWD[_WS_TCO].St2),tDEC|tWYGAS,tALL,0,0);				
	SetData(&(PWD[_WS_TCO].Dt2),0,0);			//wygas wszystkie segmenty wyswietlacza
#endif
	SetWSW(0); 							//wygas wszystkie znaki wyswietlacza
	//UpdateWSW();
}
/*
void TestScr(void)
{
	SetStatus(&PWD[_WS_TCO],tLED|tKROPKA1|tKROPKA0,tALL,0,0);				
	SetData(&PWD[_WS_TCO],FULL,FULL);	//zapal wszystkie segmenty wyswietlacza
#if LCD4D!=0
	SetStatus2(&PWD[_WS_TCO],tLED|tKROPKA1|tKROPKA0,tALL,0,0);				
	SetData2(&PWD[_WS_TCO],FULL,FULL);	//zapal wszystkie segmenty wyswietlacza
#endif
	SetWSW(1); 							//zapal wszystkie znaki wyswietlacza
}
void EndTestScr(void)
{
	SetStatus(&PWD[_WS_TCO],tDEC|tWYGAS,tALL,0,0);				
	SetData(&PWD[_WS_TCO],0,0);			//wygas wszystkie segmenty wyswietlacza
#if LCD4D!=0
	SetStatus2(&PWD[_WS_TCO],tDEC|tWYGAS,tALL,0,0);				
	SetData2(&PWD[_WS_TCO],0,0);			//wygas wszystkie segmenty wyswietlacza
#endif
	SetWSW(0); 							//wygas wszystkie znaki wyswietlacza
}*/
//---------------------------------------------------------
//---------------------------------------------------------
//Wyswietla wskaz skladajacy sie z oznaczenia literowego i cyfry od 0 do 9
//WYSWIETLANIE 2 CYFROWE
//lkod - dowolny znak w kodzie led wyswietlany na starszej cyfrze
//dla tryb=0:
//nkod - liczba zakresu 0-F wyswietlana na mlodszej cyfrze
//dla tryb=1:
//nkod - dowolny znak w kodzie led wyswietlany na mlodszej cyfrze
//WYSWIETLANIE 4 CYFROWE
//lkod - dowolny znak w kodzie led wyswietlany na 3 cyfrze
//nkod - liczba zakresu 0-99 wyswietlana na 2 najmlodszych cyfrach
/*void PrintGrafH(const unsigned char lkod2,const unsigned char lkod1)
{
	SetData(&(PWD[_WS_TCO].Dt2),lkod2,lkod1);
	SetStatus(&(PWD[_WS_TCO].St2),tLED|tFOCUS,tALL,0,0);
	//SetData(&(PWD[_WS_PRM].Dt),0,nkod);
	//SetStatus(&(PWD[_WS_PRM].St),tDEC|tSDID|tFOCUS,tALL,_KZ_SRV,0);
}*/
void PrintGrafH(const unsigned char lkod2,const unsigned char lkod1)
{
	SetStatus(&(PWD[_WS_TCW].St2),tLED|tSDID|tFOCUS,tALL,0,0);
	SetData(&(PWD[_WS_TCW].Dt2),lkod2,lkod1);
	//if(!only) SetStatus(&(PWD[_WS_TCW].St),tWYGAS|tFOCUS,tALL,0,0);
	//UpdateWSW();
}