/*keypad.c*/
//Funkcje obslugi przycisków p³ytki interfejsu
// (!) Bazuje na funkcjach z plikow: rtimer.c
/*---------------------------------------------------------------------------------*/
//#include <pic18.h>
#include <xc.h>
#include "global.h"			//parametry globalne
#include "konsola.h"			//parametry lokalne
#include "main.tp.h"
#include "pomiar.tp.h"
#include "print.h"

extern volatile unsigned char x,y;
/*---------------------------------------------------------------------------------*/
extern tpMAIN M;
extern DataPFN PFN;

#if _SRVTRB==1						//aktywny tryb serwisowy kompilacji?
unsigned char mypwm=_MG2_MOD;
#endif
extern volatile StPin xxky[];
extern volatile StPin ky[];

extern volatile DataCO DtCO;
extern volatile DataCW DtCW;

//Zapis nastawy PCW po ustalonym czasie do EEPROM
void WriteParam(unsigned char *zapis)
{
		if(*zapis&&RTdS(_RTPCW)>=_TWRTPCW)
		{
			*zapis=0;
			StartRTdS(_RTPCW);
			//WriteEEPCW();
		}
}
/*---------------------------------------------------------------------------------*/
//Inkrementacja/Dekrementacja parametru
//gdzie:
//param- adres modyfikowanego parametru (16bit)
//zmiana - znacznik ze wartosc parametru ulegla zmianie (0- brak zmianay, 1 - tylko podglad min/max, 2 - wartosc zmniejszona, 3 - wartosc zwiekszona)
//zapis - znacznik sugestji/aktywacji zapisu parametru do pamieci
//min- minimalna wartosc jaka moze przyjac parametr
//max- maksymalna wartosc jaka moze przyjac parametr
//rckl - po osiagnieciu max+1 nastepuje przeladowanie wartoscia min i odwrotnie po osiagnieciu min-1 nastepuje przeladowanie wartoscia max
//fst - aktywacja funkcji przyspieszajacej inkrementacji/dekrementacji
#if _KEY_BBOARD==0
//void ReadParam(unsigned int *param,unsigned char *zmiana,unsigned char *zapis,unsigned int min, unsigned int max, unsigned char rckl, unsigned char fst)
////void ReadParam(volatile unsigned int *param,unsigned char *zmiana,unsigned char *zapis,unsigned int min, unsigned int max, unsigned char rckl, unsigned char fst)
//{
//	static unsigned char pnst;
//	unsigned char stp,ovi,ovd;
//
//#if _SRVTRB==1						//aktywny tryb serwisowy kompilacji?
///*	*param=max;
//	*zmiana=0;
//	*zapis=0;
//	if(!RdPrt(S_KDW))
//	{
//		ReadDataXONLI(RdPrt(S_KUP),&mypwm,_MG2_MOD,1,1);
//	}
//	if(!RdPrt(S_KUP))
//	{
//		if(PWM._ENOSC)
//		{
//			if(mypwm<PWM.MDMOD) mypwm=PWM.MDMOD;
//			ReadDataXONLD(RdPrt(S_KDW),&mypwm,PWM.MDMOD,1,1);
//		}
//		else
//		{
//			ReadDataXONLD(RdPrt(S_KDW),&mypwm,0,1,1);
//		}
//	}*/
//#else
//	ovi=0;
//	ovd=0;
//	if(*param<min) *param=min;
//	if(*param>max) *param=max;
//	if((max-min)>99) stp=5;
//	else stp=1;
//	if(!(RdPrt(S_KDW)&&RdPrt(S_KUP))) 					//nie nacisnieto jednoczesnie obu przyciskow?
//	{
//		if(!RdPrt(S_KDW))
//		{
//			ReadDataXONLID(RdPrt(S_KUP),&xxky[SS_KUP],param,&ovi,1,min,max,1,stp,fst);
//			if(rckl&&ovi) *param=min;
//			if(!rckl&&RdPrt(S_KUP)&&(*param==max)) *zmiana=1;	//wymuszony wskaz nastawy dla maksimum
//		}
//		else
//		if(!RdPrt(S_KUP))
//		{
//			ReadDataXONLID(RdPrt(S_KDW),&xxky[SS_KDW],param,&ovd,0,min,max,1,stp,fst);
//			if(rckl&&ovd) *param=max;
//			if(!rckl&&RdPrt(S_KDW)&&(*param==min)) *zmiana=1;	//wymuszony wskaz nastawy dla minimum
//		}
//		if(!RdPrt(S_KDW)&&!RdPrt(S_KUP)) //oba przyciski wycisniete
//		{
//			if(RTdS(_RTPCW)>_TFLPCW)	*zmiana=0;	 //podtrzymanie stanu znacznika modyfikacji przez czas _TWRTPCW-1
//		}
//
//		if(*param!=pnst)								//nastapila zmiana nastawy?
//		{
//			if(rckl&&ovi) *zmiana=3;
//			else
//			if(rckl&&ovd) *zmiana=2;
//			else *zmiana=(pnst<*param)?3:2;				//znacznik zmiany nastawy
//			pnst=*param;
//			StartRTdS(_RTPCW);							//zegar podtrzymania znacznika modyfikacji nastawy
//			*zapis=1;									//sugestia zapisu dla nowej nastawy (uwaga po zapisie nalezy wyzerowac)
//		}
//	}else *zmiana=0;									//nacisnieto dwa przyciski jednoczesnie
//#endif								
//}
//Inkrementacja parametru dla interfejsu z jednym przyciskiem
void ReadParamONE(signed int *param,unsigned char *zmiana,unsigned char *zapis,signed int min, signed int max, unsigned char rckl, unsigned char fst)  //kkk
{
	static signed int pnst;        //kkk
	unsigned char stp,ovi,ovd;
    signed int zkr=0;        //kkk
    
#if _SRVTRB==1						//aktywny tryb serwisowy kompilacji?
/*	*param=max;
	*zmiana=0;
	*zapis=0;
	if(!RdPrt(S_KDW))
	{
		ReadDataXONLI(RdPrt(S_KUP),&mypwm,_MG2_MOD,1,1);
	}
	if(!RdPrt(S_KUP))
	{
		if(PWM._ENOSC)
		{
			if(mypwm<PWM.MDMOD) mypwm=PWM.MDMOD;
			ReadDataXONLD(RdPrt(S_KDW),&mypwm,PWM.MDMOD,1,1);
		}
		else
		{
			ReadDataXONLD(RdPrt(S_KDW),&mypwm,0,1,1);
		}
	}*/
#else
	ovi=0;
	ovd=0;
	if(*param<min) *param=min;
	if(*param>max) *param=max;
    zkr=(max-min);        //kkk
    if(zkr>=0)        //kkk
    {
        if(zkr>99) stp=5;
        else stp=1;
    }
    else
    {
        if((-1*zkr)>99) stp=5;
        else stp=1;        
    }   

	if(!RdPrt(S_RES)) 					//nie nacisnieto przycisku RESET?
	{
        ReadDataXONLID(RdPrt(S_BAR),&xxky[SS_BAR],param,&ovi,1,min,max,1,stp,fst);
        if(rckl&&ovi) *param=min;
        if(!rckl&&RdPrt(S_BAR)&&(*param==max)) *zmiana=1;	//wymuszony wskaz nastawy dla maksimum

		if(!RdPrt(S_BAR)) //przycisk wycisniety?
		{
			if(RTdS(_RTPCW)>_TFLPCW)	*zmiana=0;	 //podtrzymanie stanu znacznika modyfikacji przez czas _TWRTPCW-1
		}

		if(*param!=pnst)								//nastapila zmiana nastawy?
		{
			if(rckl&&ovi) *zmiana=3;
			else
			if(rckl&&ovd) *zmiana=2;
			else *zmiana=(pnst<*param)?3:2;				//znacznik zmiany nastawy
			pnst=*param;
			StartRTdS(_RTPCW);							//zegar podtrzymania znacznika modyfikacji nastawy
			*zapis=1;									//sugestia zapisu dla nowej nastawy (uwaga po zapisie nalezy wyzerowac)
		}
	}else *zmiana=0;									//nacisnieto dwa przyciski jednoczesnie
#endif								
}
//void ReadParamONE(unsigned int *param,unsigned char *zmiana,unsigned char *zapis,unsigned int min, unsigned int max, unsigned char rckl, unsigned char fst)
//{
//	static unsigned char pnst;
//	unsigned char stp,ovi,ovd;
//
//#if _SRVTRB==1						//aktywny tryb serwisowy kompilacji?
///*	*param=max;
//	*zmiana=0;
//	*zapis=0;
//	if(!RdPrt(S_KDW))
//	{
//		ReadDataXONLI(RdPrt(S_KUP),&mypwm,_MG2_MOD,1,1);
//	}
//	if(!RdPrt(S_KUP))
//	{
//		if(PWM._ENOSC)
//		{
//			if(mypwm<PWM.MDMOD) mypwm=PWM.MDMOD;
//			ReadDataXONLD(RdPrt(S_KDW),&mypwm,PWM.MDMOD,1,1);
//		}
//		else
//		{
//			ReadDataXONLD(RdPrt(S_KDW),&mypwm,0,1,1);
//		}
//	}*/
//#else
//	ovi=0;
//	ovd=0;
//	if(*param<min) *param=min;
//	if(*param>max) *param=max;
//	if((max-min)>99) stp=5;
//	else stp=1;
//	if(!RdPrt(S_RES)) 					//nie nacisnieto przycisku RESET?
//	{
//        ReadDataXONLID(RdPrt(S_BAR),&xxky[SS_BAR],param,&ovi,1,min,max,1,stp,fst);
//        if(rckl&&ovi) *param=min;
//        if(!rckl&&RdPrt(S_BAR)&&(*param==max)) *zmiana=1;	//wymuszony wskaz nastawy dla maksimum
//
//		if(!RdPrt(S_BAR)) //przycisk wycisniety?
//		{
//			if(RTdS(_RTPCW)>_TFLPCW)	*zmiana=0;	 //podtrzymanie stanu znacznika modyfikacji przez czas _TWRTPCW-1
//		}
//
//		if(*param!=pnst)								//nastapila zmiana nastawy?
//		{
//			if(rckl&&ovi) *zmiana=3;
//			else
//			if(rckl&&ovd) *zmiana=2;
//			else *zmiana=(pnst<*param)?3:2;				//znacznik zmiany nastawy
//			pnst=*param;
//			StartRTdS(_RTPCW);							//zegar podtrzymania znacznika modyfikacji nastawy
//			*zapis=1;									//sugestia zapisu dla nowej nastawy (uwaga po zapisie nalezy wyzerowac)
//		}
//	}else *zmiana=0;									//nacisnieto dwa przyciski jednoczesnie
//#endif								
//}
#endif
#if _KEY_BBOARD==1
void ReadParamCW(unsigned int *param,unsigned char *zmiana,unsigned char *zapis,unsigned int min, unsigned int max, unsigned char rckl, unsigned char fst)
{
	static unsigned char pnst;
	unsigned char stp,ovi,ovd;

#if _SRVTRB==1						//aktywny tryb serwisowy kompilacji?
/*	*param=max;
	*zmiana=0;
	*zapis=0;
	if(!RdPrt(S_KDW_CW))
	{
		ReadDataXONLI(RdPrt(S_KUP_CW),&mypwm,_MG2_MOD,1,1);
	}
	if(!RdPrt(S_KUP_CW))
	{
		if(PWM._ENOSC)
		{
			if(mypwm<PWM.MDMOD) mypwm=PWM.MDMOD;
			ReadDataXONLD(RdPrt(S_KDW_CW),&mypwm,PWM.MDMOD,1,1);
		}
		else
		{
			ReadDataXONLD(RdPrt(S_KDW_CW),&mypwm,0,1,1);
		}
	}*/
#else
	ovi=0;
	ovd=0;
	if(*param<min) *param=min;
	if(*param>max) *param=max;
	if((max-min)>99) stp=5;
	else stp=1;
	if(!(RdPrt(S_KDW_CW)&&RdPrt(S_KUP_CW))) 					//nie nacisnieto jednoczesnie obu przyciskow?
	{
		if(!RdPrt(S_KDW_CW))
		{
			ReadDataXONLID(RdPrt(S_KUP_CW),&xxky[SS_KUP_CW],param,&ovi,1,min,max,1,stp,fst);
			if(rckl&&ovi) *param=min;
			if(!rckl&&RdPrt(S_KUP_CW)&&(*param==max)) *zmiana=1;	//wymuszony wskaz nastawy dla maksimum
		}
		else
		if(!RdPrt(S_KUP_CW))
		{
			ReadDataXONLID(RdPrt(S_KDW_CW),&xxky[SS_KDW_CW],param,&ovd,0,min,max,1,stp,fst);
			if(rckl&&ovd) *param=max;
			if(!rckl&&RdPrt(S_KDW_CW)&&(*param==min)) *zmiana=1;	//wymuszony wskaz nastawy dla minimum
		}
		if(!RdPrt(S_KDW_CW)&&!RdPrt(S_KUP_CW)) //oba przyciski wycisniete
		{
			if(RTdS(_RTPCW)>_TFLPCW)	*zmiana=0;	 //podtrzymanie stanu znacznika modyfikacji przez czas _TWRTPCW-1
		}

		if(*param!=pnst)								//nastapila zmiana nastawy?
		{
			if(rckl&&ovi) *zmiana=3;
			else
			if(rckl&&ovd) *zmiana=2;
			else *zmiana=(pnst<*param)?3:2;				//znacznik zmiany nastawy
			pnst=*param;
			StartRTdS(_RTPCW);							//zegar podtrzymania znacznika modyfikacji nastawy
			*zapis=1;									//sugestia zapisu dla nowej nastawy (uwaga po zapisie nalezy wyzerowac)
		}
	}else *zmiana=0;									//nacisnieto dwa przyciski jednoczesnie
#endif								
}
void ReadParamCO(unsigned int *param,unsigned char *zmiana,unsigned char *zapis,unsigned int min, unsigned int max, unsigned char rckl, unsigned char fst)
{
	static unsigned char pnst;
	unsigned char stp,ovi,ovd;

#if _SRVTRB==1						//aktywny tryb serwisowy kompilacji?
/*	*param=max;
	*zmiana=0;
	*zapis=0;
	if(!RdPrt(S_KDW_CO))
	{
		ReadDataXONLI(RdPrt(S_KUP_CO),&mypwm,_MG2_MOD,1,1);
	}
	if(!RdPrt(S_KUP_CO))
	{
		if(PWM._ENOSC)
		{
			if(mypwm<PWM.MDMOD) mypwm=PWM.MDMOD;
			ReadDataXONLD(RdPrt(S_KDW_CO),&mypwm,PWM.MDMOD,1,1);
		}
		else
		{
			ReadDataXONLD(RdPrt(S_KDW_CO),&mypwm,0,1,1);
		}
	}*/
#else
	ovi=0;
	ovd=0;
	if(*param<min) *param=min;
	if(*param>max) *param=max;
	if((max-min)>99) stp=5;
	else stp=1;
	if(!(RdPrt(S_KDW_CO)&&RdPrt(S_KUP_CO))) 					//nie nacisnieto jednoczesnie obu przyciskow?
	{
		if(!RdPrt(S_KDW_CO))
		{
			ReadDataXONLID(RdPrt(S_KUP_CO),&xxky[SS_KUP_CO],param,&ovi,1,min,max,1,stp,fst);
			if(rckl&&ovi) *param=min;
			if(!rckl&&RdPrt(S_KUP_CO)&&(*param==max)) *zmiana=1;	//wymuszony wskaz nastawy dla maksimum
		}
		else
		if(!RdPrt(S_KUP_CO))
		{
			ReadDataXONLID(RdPrt(S_KDW_CO),&xxky[SS_KDW_CO],param,&ovd,0,min,max,1,stp,fst);
			if(rckl&&ovd) *param=max;
			if(!rckl&&RdPrt(S_KDW_CO)&&(*param==min)) *zmiana=1;	//wymuszony wskaz nastawy dla minimum
		}
		if(!RdPrt(S_KDW_CO)&&!RdPrt(S_KUP_CO)) //oba przyciski wycisniete
		{
			if(RTdS(_RTPCW)>_TFLPCW)	*zmiana=0;	 //podtrzymanie stanu znacznika modyfikacji przez czas _TWRTPCW-1
		}

		if(*param!=pnst)								//nastapila zmiana nastawy?
		{
			if(rckl&&ovi) *zmiana=3;
			else
			if(rckl&&ovd) *zmiana=2;
			else *zmiana=(pnst<*param)?3:2;				//znacznik zmiany nastawy
			pnst=*param;
			StartRTdS(_RTPCW);							//zegar podtrzymania znacznika modyfikacji nastawy
			*zapis=1;									//sugestia zapisu dla nowej nastawy (uwaga po zapisie nalezy wyzerowac)
		}
	}else *zmiana=0;									//nacisnieto dwa przyciski jednoczesnie
#endif								
}
#endif
/*---------------------------------------------------------------------------------*/
#define _FOFF 0
#define _ZIMA 1
#define _LATO 0
#if _KEY_BBOARD==0
void TrybPracy(void)
{
	//static bit bf_oaleg;
	volatile unsigned char key,key2,EnEco,tm;
	if(!PFNf_fsetLaddr&&!PFNf_aktywne_KNF&&!PFNf_aktywne_LKNF&&!PFNf_opgres)        //kkk
	{


    //--------------------------PRZELACZNIK FUNKCJI------------------------------
//#if _FOFF==1
//        PFNf_off=1;
//        PFNf_popFN=0;
//        PFNf_lato=0;
//        PFNf_zima=1;    
//#else
//#if _ZIMA==1
//        PFNf_off=0;
//        PFNf_popFN=0;
//        PFNf_lato=0;
//        PFNf_zima=1;  
//#else
//        PFNf_off=0;
//        PFNf_popFN=1;
//        PFNf_lato=1;
//        PFNf_zima=0;        
//#endif
//#endif
//        if(DtCW._pozZRO&&DtCO._pozZRO)
//        {
//            PFNf_off=1;
//        }
//        else
//        {
//            PFNf_off=0;
//            if(!DtCO._pozZRO)
//            {
//                PFNf_lato=0;
//                PFNf_zima=1;                
//            }
//            else
//            {
//                PFNf_lato=1;
//                PFNf_zima=0;                    
//            }
//        }
        
        
//        //przytrzymano przycisk RESET (wylaczenie/zalaczenie sterownika)
//        if(!RdPrt(S_SET))
//        {
//            key=PresKeyEX(RdPrt(S_OFF),&ky[SS_OFF]);
//            if(key!=_KDFST&&key>=_TKOFF)
//            {	
//                WaitToLowEx(&ky[SS_OFF]);
//                if(PFN.vUNI02>=9)
//                {
//                    if(PFNf_off)            //powrot ze stanu off?
//                    {
//                        if(PFNf_popFN)      //zapamietany, poprzedni stan przelacznika = LATO?
//                        {
//                            PFNf_lato=1;
//                            PFNf_zima=0;                        
//                        }
//                        else
//                        {
//                            PFNf_lato=0;
//                            PFNf_zima=1;                        
//                        }
//                    }
//                }
//                PFNf_off=!PFNf_off;
//                PFNf_set_nco=0;
//                PFNf_set_ncw=0;
//                PFNf_set_eco=0;
//                PFNf_str_nco=0;
//                PFNf_str_ncw=0;
//                PFNf_prn_res=0;
//                PFNf_set_fnserw=0;
//                PFNf_fnserw=0;
//                PFNf_set_aleg=0;
//                PFNf_aleg=0;
//                ResBufKeyEx(&ky[SS_KUP]);
//                ResBufKeyEx(&ky[SS_KDW]);
//                ResBufKeyEx(&ky[SS_SET]);
//            }
//        }
//---------------------
        if(!PFNf_off&&!PFNf_lato&&!PFNf_zima)		//stan nieustalony?
        {
            PFNf_lato=0;
            PFNf_zima=1;
        }
//        //przytrzymano przycisk SET (zmiana trybu pracy)
//        if(!RdPrt(S_OFF)&&!RdPrt(S_KUP)&&!RdPrt(S_KDW)&&!PFNf_off&&!PFNf_fnserw&&!PFNf_aleg&&!PFNf_prODP)
//        {
//            key=PresKeyEX(RdPrt(S_SET),&ky[SS_SET]);
//            if(key!=_KDFST&&key>=_TKSET)
//            {	
//                WaitToLowEx(&ky[SS_SET]);	
//                PFNf_lato=!PFNf_lato;
//                PFNf_zima=!PFNf_lato;
//                PFNf_set_nco=0;
//                PFNf_set_ncw=0;
//                PFNf_set_eco=0;
//                PFNf_fnserw=0;				//przerwanie funkcji serwisowej
//                PFNf_aleg=0;				//przerwanie funkcji antylegionella
//                PFNf_prn_res=0;
//            }
//        }
//--------------------------NASTAWY------------------------------
        //nacisnieto przycisk SET (wybor nastawy CO, funkcji serwisowej, antylegionella)
        if(PFNf_oaleg) PFNf_set_aleg=0;
        if(PFNf_ofnserw) PFNf_set_fnserw=0; 

//        if(!RdPrt(S_OFF)&&!RdPrt(S_KUP)&&!RdPrt(S_KDW)&&!PFNf_off&&!PFNf_fnserw&&!PFNf_aleg&&!PFNf_prODP)
//        {
//            key=PresKeyEX(RdPrt(S_SET),&ky[SS_SET]);
//            if(key==_KDFST)
//            {
//                ResBufKeyEx(&ky[SS_SET]);		//resetuj podtrzymanie
//                PFNf_b_set_nco=0;
//                PFNf_b_set_ncw=0;
//                PFNf_b_set_eco=0;
//                PFNf_set_ncw=0;
//                PFNf_set_eco=0;
//                PFNf_prn_res=0;
//                if(PFNf_set_nco)
//                {
//                    if(PFNf_newPCO) 			//nastawa ulegla zmianie (potraktuj klawisz SET jako potwierdzenie
//                    {
//                        PFNf_newPCO=0;
//                        PFNf_set_nco=0;
//                        PFNf_b_set_nco=0;
//                        PFNf_set_ncw=0;
//                        PFNf_b_set_ncw=0;
//                        PFNf_set_eco=0;
//                        PFNf_b_set_eco=0;
//                        PFNf_set_fnserw=0;
//                        PFNf_set_aleg=0;
//                        SetPrt(S_KDW,0);
//                        SetPrt(S_KUP,0);
//                        SetPrt(S_SET,0);
//                        ResBufKeyEx(&ky[SS_KDW]);		//resetuj podtrzymanie
//                        ResBufKeyEx(&ky[SS_KUP]);		//resetuj podtrzymanie
//                    }
//                    else
//                    if(PFNf_zima&&!PFNf_obgCW&&!PFNf_set_fnserw) {PFNf_set_nco=0;PFNf_set_fnserw=1;PFNf_fns_max=1;}
//                    else
//                    if(PFNf_EnAleg&&!PFNf_set_aleg) {PFNf_set_nco=0;PFNf_set_aleg=1;}
//                    else
//                    {
//                        PFNf_newPCO=0;
//                        PFNf_set_nco=0;
//                        PFNf_b_set_nco=0;
//                        PFNf_set_ncw=0;
//                        PFNf_b_set_ncw=0;
//                        PFNf_set_eco=0;
//                        PFNf_b_set_eco=0;
//                        PFNf_set_fnserw=0;
//                        PFNf_set_aleg=0;
//                        SetPrt(S_KDW,0);
//                        SetPrt(S_KUP,0);
//                        SetPrt(S_SET,0);
//                        ResBufKeyEx(&ky[SS_KDW]);		//resetuj podtrzymanie
//                        ResBufKeyEx(&ky[SS_KUP]);		//resetuj podtrzymanie
//                    }
//                }
//                else
//                if(PFNf_set_fnserw&&!PFNf_ferr&&!PFNf_fmsg&&!PFNf_prODP)
//                {
//                    if(PFNf_EnAleg&&!PFNf_set_aleg) {PFNf_set_fnserw=0;PFNf_set_aleg=1;}
//                    else
//                    {
//                        PFNf_newPCO=0;
//                        PFNf_set_nco=0;
//                        PFNf_b_set_nco=0;
//                        PFNf_set_ncw=0;
//                        PFNf_b_set_ncw=0;
//                        PFNf_set_eco=0;
//                        PFNf_b_set_eco=0;
//                        PFNf_set_fnserw=0;
//                        PFNf_set_aleg=0;
//                    }
//                }
//                else
//                if(PFNf_set_aleg&&!PFNf_ferr&&!PFNf_fmsg&&!PFNf_prODP)
//                {					
//                    PFNf_newPCO=0;
//                    PFNf_set_nco=0;
//                    PFNf_b_set_nco=0;
//                    PFNf_set_ncw=0;
//                    PFNf_b_set_ncw=0;
//                    PFNf_set_eco=0;
//                    PFNf_b_set_eco=0;
//                    PFNf_set_fnserw=0;
//                    PFNf_set_aleg=0;
//                }					
//                else
//                {	
//                    PFNf_set_fnserw=0;				
//                    PFNf_set_aleg=0;
//                    PFNf_set_nco=1;
//                }					
//                StartRTdS(_RTSET);
//                if(PFNf_set_nco)
//                {
//                    PFNf_b_set_nco=0;
//                    RestartKol2(_KL_NCWCO);
//                    RestartKol(_KL_NCWCO);
//                }
//            }
//        }
//        if(RTdS(_RTSET)>_TSETNST)				//uplynal czas podtrzymania wybranego parametru?
//        {
//            PFNf_set_nco=0;
//            PFNf_b_set_nco=0;
//            PFNf_set_ncw=0;
//            PFNf_b_set_ncw=0;
//            PFNf_set_eco=0;
//            PFNf_b_set_eco=0;
//            PFNf_set_fnserw=0;
//            PFNf_set_aleg=0;
//        } 
//
//        if(!RdPrt(S_OFF)&&!RdPrt(S_KDW)&&!PFNf_off&&!PFNf_fnserw&&!PFNf_aleg)
//        {
//            if(PFNf_set_fnserw)
//            {
//                key=PresKeyEX(RdPrt(S_KUP),&ky[SS_KUP]);
//                if(key!=_KDFST&&key>=_TKALG)
//                {	
//                    WaitToLowEx(&ky[SS_KUP]);
//                    PFNf_fnserw=1;
//                    PFNf_prn_res=0;
//                    PFNf_set_nco=0;
//                    PFNf_set_ncw=0;
//                    PFNf_set_eco=0;
//                    PFNf_b_ofnserw=0;
//                    StartRTdS(_RTFSP);
//                }
//            }
//            if(PFNf_set_aleg)
//            {
//                key=PresKeyEX(RdPrt(S_KUP),&ky[SS_KUP]);
//                if(key!=_KDFST&&key>=_TKSRV)
//                {	
//                    WaitToLowEx(&ky[SS_KUP]);
//                    PFNf_aleg=1;
//                    PFNf_set_nco=0;
//                    PFNf_set_ncw=0;
//                    PFNf_set_eco=0;
//                    PFNf_b_oaleg=0;
//                    StartRTdS(_RTFSP);
//                }
//            }
//        }
              
        //nastawa ECO
        if(!RdPrt(S_BAR)&&!PFNf_off&&PFNf_zima&&!PFNf_fnserw&&!PFNf_aleg&&!PFNf_set_fnserw&&!PFNf_set_aleg&&!PFNf_set_nco&&!PFNf_set_ncw&&!PFNf_set_eco&&!PFNf_prODP)
        {
            key=PresKeyEX(RdPrt(S_RES),&ky[SS_RES]);
            EnEco=(PFNf_ofzima&&PFNf_pmpPWM&&PFNf_trbECO&&!PFNf_errIN&&!PFNf_obgCW)?1:0; 
            if(EnEco&&key!=_KDFST&&key>=_TKECO)    //przytrzymanie przycisku RESET?
            {	
                WaitToLowEx(&ky[SS_RES]);
                //ResBufKeyEx(&ky[SS_RES]);		//resetuj podtrzymanie
                ResBufKeyEx(&ky[SS_BAR]);		//resetuj podtrzymanie
                RestartKol2(_KL_NCWCO);
                RestartKol(_KL_NCWCO);
                PFNf_b_set_eco=0;
                PFNf_prn_res=0;
                PFNf_prn_bar=0;
                PFNf_set_eco=1;
                PFNf_mod_eco=0;
                StartRTdS(_RTSET);                 
            }
        }        
//---------------------
        if(RTdS(_RTSET)>_TSETNST)				//uplynal czas podtrzymania wybranego parametru?
        {
            PFNf_set_nco=0;
            PFNf_b_set_nco=0;
            PFNf_set_ncw=0;
            PFNf_b_set_ncw=0;
            PFNf_set_eco=0;
            PFNf_b_set_eco=0;
            PFNf_set_fnserw=0;
            PFNf_set_aleg=0;
            PFNf_newPCO=0;
            PFNf_bnewPCO=0;
            PFNf_newPCW=0;
            PFNf_bnewPCW=0; 
            if(PFNf_FLIN)
            {
                PFNf_hi_ncw=1;
                PFNf_hi_nco=1;
            }
        }
//---------------------       
        //funkcja serwisowa, antylegionella
        if(!RdPrt(S_RES)&&!PFNf_off&&!PFNf_fnserw&&!PFNf_aleg&&!PFNf_prODP)
        {
            key=PresKeyEX(RdPrt(S_BAR),&ky[SS_BAR]);
            if(key!=_KDFST&&key>=_TKALSR)    //przytrzymanie przycisku BAR?
            {	
                WaitToLowEx(&ky[SS_BAR]);
                if(PFNf_zima)   //aktywacja funkcji serwisowej
                {
                    WaitToLowEx(&ky[SS_BAR]);
                    PFNf_fnserw=1;
                    PFNf_fns_max=1;
                    PFNf_prn_res=0;
                    PFNf_prn_bar=0;                    
                    PFNf_set_nco=0;
                    PFNf_set_ncw=0;
                    PFNf_set_eco=0;
                    PFNf_newPCW=0;
                    PFNf_bnewPCW=0;      
                    PFNf_newPCO=0;
                    PFNf_bnewPCO=0;                    
                    PFNf_b_ofnserw=0;
                    StartRTdS(_RTFSP);                    
                }
                else    //aktywacja funkcji antylegionella
                {
                    if(PFNf_EnAleg)
                    {
                        WaitToLowEx(&ky[SS_BAR]);
                        PFNf_aleg=1;
                        PFNf_set_nco=0;
                        PFNf_set_ncw=0;
                        PFNf_set_eco=0;
                        PFNf_b_oaleg=0;
                        PFNf_newPCW=0;
                        PFNf_bnewPCW=0;      
                        PFNf_newPCO=0;
                        PFNf_bnewPCO=0;                        
                        StartRTdS(_RTFSP);
                    }
                }
            }
        }
//---------------------
        if(!PFNf_off&&(PFNf_fnserw||PFNf_ofnserw))					//aktywna funkcja serwisowa?
        {
            if(PresKeyEX(RdPrt(S_BAR),&ky[SS_BAR])==100)	//nacisnieto przycisk BAR?
            {
                //ResBufKeyEx(&ky[SS_BAR]);		//resetuj podtrzymanie
                PFNf_fns_max=!PFNf_fns_max;
            }
        }        
        if(RTdS(_RTFSP)>_TAFSP)				//uplynal maksymalny czas na aktywacje funkcji serwisowej/antylegionella?
        {
            if(PFNf_aleg&&!PFNf_oaleg) PFNf_aleg=0;
            if(PFNf_fnserw&&!PFNf_ofnserw) PFNf_fnserw=0;
        }
        
        
//---------------------
        //zmiana nastawy CW nastawnikiem
        //if(!RdPrt(S_OFF)&&!RdPrt(S_SET)&&!RdPrt(S_KOM)&&!PFNf_off&&!PFNf_fnserw&&!PFNf_aleg&&!PFNf_set_fnserw&&!PFNf_set_aleg&&!PFNf_set_nco&&!PFNf_set_ncw&&!PFNf_set_eco&&!PFNf_prODP)
        if(!RdPrt(S_BAR)&&!RdPrt(S_RES)&&!PFNf_off&&!PFNf_fnserw&&!PFNf_aleg&&!PFNf_set_fnserw&&!PFNf_set_aleg&&!PFNf_set_ncw&&!PFNf_set_eco)//&&!PFNf_prODP)            
        {
            if(DtCW._newPCW&&PFN.PCW!=DtCW.PCW)		
            {
                DtCW._newPCW=0;
                RestartKol2(_KL_NCWCO);
                RestartKol(_KL_NCWCO);
                PFNf_b_set_ncw=0;
                PFNf_prn_res=0;
                PFNf_set_ncw=1;
                PFNf_mod_ncw=0;
                PFNf_set_nco=0;
                PFNf_newPCW=0;
                PFNf_bnewPCW=0;      
                PFNf_newPCO=0;
                PFNf_bnewPCO=0;                
                StartRTdS(_RTSET);
            }
        }
        else
        {
            DtCW._newPCW=0;
        }
//---------------------
        //zmiana nastawy CO nastawnikiem
        //if(!RdPrt(S_OFF)&&!RdPrt(S_SET)&&!RdPrt(S_KOM)&&!PFNf_off&&!PFNf_fnserw&&!PFNf_aleg&&!PFNf_set_fnserw&&!PFNf_set_aleg&&!PFNf_set_nco&&!PFNf_set_ncw&&!PFNf_set_eco&&!PFNf_prODP)
        if(!RdPrt(S_BAR)&&!RdPrt(S_RES)&&!PFNf_off&&!PFNf_fnserw&&!PFNf_aleg&&!PFNf_set_fnserw&&!PFNf_set_aleg&&!PFNf_set_nco&&!PFNf_set_eco)//&&!PFNf_prODP)            
        {
            if(DtCO._newPCO&&PFN.PCO!=DtCO.PCO)		
            {
                DtCO._newPCO=0;
                RestartKol2(_KL_NCWCO);
                RestartKol(_KL_NCWCO);
                PFNf_b_set_nco=0;
                PFNf_prn_res=0;
                PFNf_set_nco=1;
                PFNf_set_ncw=0;
                PFNf_newPCW=0;
                PFNf_bnewPCW=0;      
                PFNf_newPCO=0;
                PFNf_bnewPCO=0;                
                StartRTdS(_RTSET);
            }
        }  
        else
        {
            DtCO._newPCO=0;  
        }
//---------------------
        //nacisnieto przycisk UP lub DW (zmiana nastawy CW lub nastawy ECO)
//        if(!RdPrt(S_OFF)&&!RdPrt(S_SET)&&!PFNf_off&&!PFNf_fnserw&&!PFNf_aleg&&!PFNf_set_fnserw&&!PFNf_set_aleg&&!PFNf_set_nco&&!PFNf_set_ncw&&!PFNf_set_eco&&!PFNf_prODP)
//        {
//            key=PresKeyEX(RdPrt(S_KDW),&ky[SS_KDW]);
//            key2=PresKeyEX(RdPrt(S_KUP),&ky[SS_KUP]);
//
//            //EnEco=(!PFNf_pgd&&PFNf_ofzima&&PFNf_pmpPWM&&PFNf_trbECO&&!PFNf_errIN&&!PFNf_obgCW)?1:0;
//            EnEco=(PFNf_ofzima&&PFNf_pmpPWM&&PFNf_trbECO&&!PFNf_errIN&&!PFNf_obgCW)?1:0;            
//
//            if(key==_KDFST||(!EnEco&&key>=_TKSET)||key2==_KDFST||(!EnEco&&key2>=_TKSET))		
//            {
//                ResBufKeyEx(&ky[SS_KDW]);		//resetuj podtrzymanie
//                ResBufKeyEx(&ky[SS_KUP]);		//resetuj podtrzymanie
//                RestartKol2(_KL_NCWCO);
//                RestartKol(_KL_NCWCO);
//                PFNf_b_set_ncw=0;
//                PFNf_prn_res=0;
//                PFNf_set_ncw=1;
//                PFNf_mod_ncw=0;
//                StartRTdS(_RTSET);
//            }
//            else
//            if((EnEco&&key>=(_TKSET*2))||(EnEco&&key2>=(_TKSET*2)))		
//            {
//                ResBufKeyEx(&ky[SS_KDW]);		//resetuj podtrzymanie
//                ResBufKeyEx(&ky[SS_KUP]);		//resetuj podtrzymanie
//                RestartKol2(_KL_NCWCO);
//                RestartKol(_KL_NCWCO);
//                PFNf_b_set_eco=0;
//                PFNf_prn_res=0;
//                PFNf_set_eco=1;
//                PFNf_mod_eco=0;
//                StartRTdS(_RTSET);
//            }
//        }
//        else
//        {
//            if(!PFNf_off&&PFNf_prODP)
//            {
//                WaitToLowEx(&ky[SS_KDW]);  
//                WaitToLowEx(&ky[SS_KUP]);
//            }
//        }
//

//--------------------------FUNKCJE SPECJALNE------------------------------
        if(!PFNf_cwIMP)
        {
            tm=_TPCS;
        }
        else
        {
            tm=_TPCS2;
        }
        //wskaz parametrow
        if(RTdS(_RTPCS)>tm)									//uplynal czas podtrzymania wskazu cisnienia?
        {
            PFNf_prn_res=0;
        }

        if(!PFNf_prn_res)
        {
            //PFNf_prn_cs=0;
            PFNf_prn_in=0;
            PFNf_prn_pp=0;
            PFNf_prn_pg=0;
            PFNf_prn_fl=0;
        }
        else
        {
            if(!PFNf_cwIMP)     //stykowy czujnik przeplywu CW?
            {            
                if(RTdS(_RTPCS)<(tm/3))
                {
                    //PFNf_prn_cs=1;			
                    PFNf_prn_in=1;
                    PFNf_prn_pp=0;
                    PFNf_prn_pg=0;
                }
                else
                if((RTdS(_RTPCS)>=(tm/3))&&(RTdS(_RTPCS)<(2*tm/3)))		//nadszedl czas podtrzymania wskazu temp wlotowej?
                {
                    //PFNf_prn_cs=0;
                    PFNf_prn_in=0;
                    PFNf_prn_pp=1;
                    PFNf_prn_pg=0;
                }
                else
                if((RTdS(_RTPCS)>=((2*tm)/3)))
                {
                    PFNf_prn_in=0;
                    PFNf_prn_pp=0;
                    PFNf_prn_pg=1;
                }		
            }
            else    //impulsowy czujnik przeplywu CW
            {
                if(RTdS(_RTPCS)<(tm/4))
                {
                    //PFNf_prn_cs=1;			
                    PFNf_prn_in=1;
                    PFNf_prn_pp=0;
                    PFNf_prn_pg=0;
                    PFNf_prn_fl=0;                    
                }
                else
                if((RTdS(_RTPCS)>=(tm/4))&&(RTdS(_RTPCS)<(2*tm/4)))		//nadszedl czas podtrzymania wskazu temp wlotowej?
                {
                    //PFNf_prn_cs=0;
                    PFNf_prn_in=0;
                    PFNf_prn_pp=1;
                    PFNf_prn_pg=0;
                    PFNf_prn_fl=0;                     
                }
                else
                if((RTdS(_RTPCS)>=(2*tm/4))&&(RTdS(_RTPCS)<(3*tm/4)))		//nadszedl czas podtrzymania wskazu temp wlotowej?
                {
                    //PFNf_prn_cs=0;
                    PFNf_prn_in=0;
                    PFNf_prn_pp=0;
                    PFNf_prn_pg=1;
                    PFNf_prn_fl=0;                     
                }
                else                    
                if((RTdS(_RTPCS)>=((3*tm)/4)))
                {
                    PFNf_prn_in=0;
                    PFNf_prn_pp=0;
                    PFNf_prn_pg=0;
                    PFNf_prn_fl=1;                     
                }                
            }
        }  
        
        //wskaz cisnienia
        if(RTdS(_RTPBAR)>_TPCS/3)									//uplynal czas podtrzymania wskazu cisnienia?
        {
            PFNf_prn_bar=0;
        }
	
        if(!PFNf_prn_bar)
        {
            PFNf_prn_cs=0;
        }
        else
        {
            if(RTdS(_RTPBAR)<_TPCS/3)
            {
                PFNf_prn_cs=1;			
            }
            else
            {
                PFNf_prn_cs=0;
            }
        }

//--------------------------RESET------------------------------
        //nacisnieto przycisk RESET
//        key=PresKeyEX(RdPrt(S_OFF),&ky[SS_OFF]);
//        if(key==_KDFST)
//        {
//            if(PFNf_set_nco||PFNf_set_ncw||PFNf_set_eco||PFNf_str_nco||PFNf_str_ncw||PFNf_prn_res||PFNf_set_fnserw||PFNf_fnserw||PFNf_set_aleg||PFNf_aleg||PFNf_prODP)
//            {
//                WaitToLowEx(&ky[SS_OFF]);
//                PFNf_set_nco=0;
//                PFNf_set_ncw=0;
//                PFNf_str_nco=0;
//                PFNf_str_ncw=0;
//                PFNf_mod_ncw=0;
//                PFNf_set_eco=0;
//                PFNf_mod_eco=0;
//                PFNf_prn_res=0;
//                PFNf_set_fnserw=0;
//                PFNf_fnserw=0;
//                PFNf_set_aleg=0;
//                PFNf_aleg=0;
//            }
//            else
//            if(!PFNf_off&&!PFNf_ferr&&!PFNf_fmsg&&!PFNf_prODP)
//            {
//                WaitToLowEx(&ky[SS_OFF]);
//                if(!PFNf_errCS||(PFNf_fnoCS&&!PFNf_errIN))
//                {
//                    PFNf_prn_res=1;
//                }
//                StartRTdS(_RTPCS);
//            }
//        }
        
        //nacisnieto przycisk RESET
        key=PresKeyEX(RdPrt(S_RES),&ky[SS_RES]);
        if(key==_KDFST)
        {
            if(PFNf_set_nco||PFNf_set_ncw||PFNf_set_eco||PFNf_str_nco||PFNf_str_ncw||PFNf_prn_res||PFNf_prn_bar||PFNf_set_fnserw||PFNf_fnserw||PFNf_set_aleg||PFNf_aleg||PFNf_prODP)
            {
                WaitToLowEx(&ky[SS_RES]);
                PFNf_set_nco=0;
                PFNf_set_ncw=0;
                PFNf_str_nco=0;
                PFNf_str_ncw=0;
                PFNf_mod_ncw=0;
                PFNf_set_eco=0;
                PFNf_mod_eco=0;
                PFNf_prn_res=0;
                PFNf_prn_bar=0;                
                PFNf_set_fnserw=0;
                PFNf_fnserw=0;
                PFNf_set_aleg=0;
                PFNf_aleg=0;
                PFNf_newPCO=0;
                PFNf_bnewPCO=0;
                PFNf_newPCW=0;
                PFNf_bnewPCW=0;                 
            }
            else
            if(!PFNf_off&&!PFNf_ferr&&!PFNf_fmsg&&!PFNf_prODP)
            {
                WaitToLowEx(&ky[SS_RES]);
                //if(!PFNf_errCS||(PFNf_fnoCS&&!PFNf_errIN))
                {
                    PFNf_prn_res=1;
                    PFNf_prn_bar=0;
                }
                StartRTdS(_RTPCS);
            }
        }     
        //nacisnieto przycisk BAR
        key=PresKeyEX(RdPrt(S_BAR),&ky[SS_BAR]);
        if(key==_KDFST)
        {
            if(PFNf_set_nco||PFNf_set_ncw||PFNf_set_eco||PFNf_str_nco||PFNf_str_ncw||PFNf_prn_res||PFNf_prn_bar||PFNf_set_fnserw||PFNf_fnserw||PFNf_set_aleg||PFNf_aleg||PFNf_prODP)
            {
                WaitToLowEx(&ky[SS_BAR]);
                PFNf_prn_bar=0;                
            }
            else           
            if(!PFNf_off&&!PFNf_ferr&&!PFNf_fmsg&&!PFNf_prODP)
            {
                WaitToLowEx(&ky[SS_BAR]);
                if(!PFNf_errCS&&!PFNf_fnoCS)
                {
                    PFNf_prn_res=0;
                    PFNf_prn_bar=1;
                }
                StartRTdS(_RTPBAR);
            }
        }        
	}
	else
	{
		PFNf_set_nco=0;
		PFNf_set_ncw=0;
		PFNf_str_nco=0;
		PFNf_str_ncw=0;
		PFNf_set_eco=0;
		PFNf_prn_res=0;
        PFNf_prn_bar=0;
		PFNf_set_fnserw=0;
		PFNf_fnserw=0;
		PFNf_set_aleg=0;
		PFNf_aleg=0;
        PFNf_newPCO=0;
        PFNf_bnewPCO=0;
        PFNf_newPCW=0;
        PFNf_bnewPCW=0;         
	}
    
    if(!PFNf_opgres) PFNf_LINres=0;
    if(!PFNf_fsetLaddr) //nieaktywny tryb przypisania adresu LIN?
    {
        //stan przycisku RESET
        //if(RdPrt(S_OFF)||PFNf_LINres)
        if(RdPrt(S_RES)||PFNf_LINres)
        {
                PFNf_res=1;
        }
        else PFNf_res=0;
    }
}
#endif
/*---------------------------------------------------------------------------------*/
#if _KEY_BBOARD==1
void TrybPracy(void)
{
	//static bit bf_oaleg;
	volatile unsigned char key,key2,EnEco,tm;
	if(!PFNf_fsetLaddr&&!PFNf_aktywne_KNF&&!PFNf_aktywne_LKNF&&!PFNf_opgres)
	{

    //--------------------------PRZELACZNIK FUNKCJI------------------------------
        //przytrzymano przycisk RESET (wylaczenie/zalaczenie sterownika)
        if(!RdPrt(S_SET)&&!RdPrt(S_KOM))
        {
            key=PresKeyEX(RdPrt(S_OFF),&ky[SS_OFF]);
            if(key!=_KDFST&&key>=_TKOFF)
            {	
                WaitToLowEx(&ky[SS_OFF]);
                if(PFN.vUNI02>=9)
                {
                    if(PFNf_off)            //powrot ze stanu off?
                    {
                        if(PFNf_popFN)      //zapamietany, poprzedni stan przelacznika = LATO?
                        {
                            PFNf_lato=1;
                            PFNf_zima=0;                        
                        }
                        else
                        {
                            PFNf_lato=0;
                            PFNf_zima=1;                        
                        }
                    }
                }
                PFNf_off=!PFNf_off;
                PFNf_set_nco=0;
                PFNf_set_ncw=0;
                PFNf_set_eco=0;
                PFNf_str_nco=0;
                PFNf_str_ncw=0;
                PFNf_prn_res=0;
                PFNf_set_fnserw=0;
                PFNf_fnserw=0;
                PFNf_set_aleg=0;
                PFNf_aleg=0;
                ResBufKeyEx(&ky[SS_KUP_CW]);
                ResBufKeyEx(&ky[SS_KDW_CW]);
                ResBufKeyEx(&ky[SS_KUP_CO]);
                ResBufKeyEx(&ky[SS_KDW_CO]);                
                ResBufKeyEx(&ky[SS_SET]);
                ResBufKeyEx(&ky[SS_KOM]);                
            }
        }
//---------------------
        if(!PFNf_off&&!PFNf_lato&&!PFNf_zima)		//stan nieustalony?
        {
            PFNf_lato=0;
            PFNf_zima=1;
        }
        //przytrzymano przycisk SET (zmiana trybu pracy)
        if(!RdPrt(S_OFF)&&!RdPrt(S_KOM)&&!RdPrt(S_KUP_CW)&&!RdPrt(S_KDW_CW)&&!RdPrt(S_KUP_CO)&&!RdPrt(S_KDW_CO)&&!PFNf_off&&!PFNf_fnserw&&!PFNf_aleg&&!PFNf_prODP)
        {
            key=PresKeyEX(RdPrt(S_SET),&ky[SS_SET]);
            if(key!=_KDFST&&key>=_TKSET)
            {	
                WaitToLowEx(&ky[SS_SET]);	
                PFNf_lato=!PFNf_lato;
                PFNf_zima=!PFNf_lato;
                PFNf_set_nco=0;
                PFNf_set_ncw=0;
                PFNf_set_eco=0;
                PFNf_fnserw=0;				//przerwanie funkcji serwisowej
                PFNf_aleg=0;				//przerwanie funkcji antylegionella
                PFNf_prn_res=0;
            }
        }
//--------------------------NASTAWY------------------------------
        //nacisnieto przycisk KOM (wybor funkcji serwisowej, antylegionella)
        if(PFNf_oaleg) PFNf_set_aleg=0;
        if(PFNf_ofnserw) PFNf_set_fnserw=0; 

        if(!RdPrt(S_SET)&&!RdPrt(S_OFF)&&!RdPrt(S_KUP_CW)&&!RdPrt(S_KDW_CW)&&!RdPrt(S_KUP_CO)&&!RdPrt(S_KDW_CO)&&!PFNf_off&&!PFNf_fnserw&&!PFNf_aleg&&!PFNf_prODP)
        {
            key=PresKeyEX(RdPrt(S_KOM),&ky[SS_KOM]);
            if(((PFNf_set_fnserw||PFNf_set_aleg)&&(key==_KDFST))||(!PFNf_set_fnserw&&!PFNf_set_aleg&&(key!=_KDFST&&key>=_TKSET)))
            {
                if(PFNf_set_fnserw||PFNf_set_aleg)
                {
                    ResBufKeyEx(&ky[SS_KOM]);		//resetuj podtrzymanie
                }
                else
                {
                    WaitToLowEx(&ky[SS_KOM]);
                }
                PFNf_b_set_nco=0;
                PFNf_b_set_ncw=0;
                PFNf_b_set_eco=0;
                PFNf_set_ncw=0;
                PFNf_set_eco=0;
                PFNf_prn_res=0;
                if(PFNf_zima&&!PFNf_obgCW&&!PFNf_set_fnserw&&!PFNf_set_aleg&&!PFNf_ferr&&!PFNf_fmsg&&!PFNf_prODP)
                {
                    PFNf_set_nco=0;
                    PFNf_set_fnserw=1;
                    PFNf_fns_max=1;
                    StartRTdS(_RTSET);
                }
                else
                if(PFNf_set_fnserw&&!PFNf_ferr&&!PFNf_fmsg&&!PFNf_prODP)
                {
                    if(PFNf_EnAleg&&!PFNf_set_aleg)
                    {
                        PFNf_set_fnserw=0;
                        PFNf_set_aleg=1;
                        StartRTdS(_RTSET);
                    }
                    else
                    {
                        PFNf_newPCO=0;
                        PFNf_set_nco=0;
                        PFNf_b_set_nco=0;
                        PFNf_set_ncw=0;
                        PFNf_b_set_ncw=0;
                        PFNf_set_eco=0;
                        PFNf_b_set_eco=0;
                        PFNf_set_fnserw=0;
                        PFNf_set_aleg=0;
                    }
                }
                else
                if(!PFNf_zima&&PFNf_EnAleg&&!PFNf_set_aleg&&!PFNf_ferr&&!PFNf_fmsg&&!PFNf_prODP)    
                {
                    PFNf_set_fnserw=0;
                    PFNf_set_aleg=1;
                    StartRTdS(_RTSET);                    
                }
                else
                {
                    PFNf_newPCO=0;
                    PFNf_set_nco=0;
                    PFNf_b_set_nco=0;
                    PFNf_set_ncw=0;
                    PFNf_b_set_ncw=0;
                    PFNf_set_eco=0;
                    PFNf_b_set_eco=0;
                    PFNf_set_fnserw=0;
                    PFNf_set_aleg=0;                    
                }
            }
        }
        if(RTdS(_RTSET)>_TSETNST)				//uplynal czas podtrzymania wybranego parametru?
        {
            PFNf_set_nco=0;
            PFNf_b_set_nco=0;
            PFNf_set_ncw=0;
            PFNf_b_set_ncw=0;
            PFNf_set_eco=0;
            PFNf_b_set_eco=0;
            PFNf_set_fnserw=0;
            PFNf_set_aleg=0;
        } 
        //aktywacja funkcji serwisowej lub antylegionella przyciskiem +
        if(!RdPrt(S_SET)&&!RdPrt(S_OFF)&&!RdPrt(S_KDW_CW)&&!RdPrt(S_KDW_CO)&&!PFNf_off&&!PFNf_fnserw&&!PFNf_aleg)
        {
            if(PFNf_set_fnserw)
            {
                key=PresKeyEX(RdPrt(S_KUP_CO),&ky[SS_KUP_CO]);
                if(key!=_KDFST&&key>=_TKALG)
                {	
                    WaitToLowEx(&ky[SS_KUP_CO]);
                    PFNf_fnserw=1;
                    PFNf_prn_res=0;
                    PFNf_set_nco=0;
                    PFNf_set_ncw=0;
                    PFNf_set_eco=0;
                    PFNf_b_ofnserw=0;
                    StartRTdS(_RTFSP);
                }
            }
            if(PFNf_set_aleg)
            {
                key=PresKeyEX(RdPrt(S_KUP_CW),&ky[SS_KUP_CW]);
                if(key!=_KDFST&&key>=_TKSRV)
                {	
                    WaitToLowEx(&ky[SS_KUP_CW]);
                    PFNf_aleg=1;
                    PFNf_set_nco=0;
                    PFNf_set_ncw=0;
                    PFNf_set_eco=0;
                    PFNf_b_oaleg=0;
                    StartRTdS(_RTFSP);
                }
            }
        }
        if(RTdS(_RTFSP)>_TAFSP)				//uplynal maksymalny czas na aktywacje funkcji serwisowej/antylegionella?
        {
            if(PFNf_aleg&&!PFNf_oaleg) PFNf_aleg=0;
            if(PFNf_fnserw&&!PFNf_ofnserw) PFNf_fnserw=0;
        }
//---------------------
        //nacisnieto przycisk UP lub DW (zmiana nastawy CW)
        //if(!RdPrt(S_OFF)&&!RdPrt(S_SET)&&!RdPrt(S_KOM)&&!PFNf_off&&!PFNf_fnserw&&!PFNf_aleg&&!PFNf_set_fnserw&&!PFNf_set_aleg&&!PFNf_set_nco&&!PFNf_set_ncw&&!PFNf_set_eco&&!PFNf_prODP)
        if(!RdPrt(S_KUP_CO)&&!RdPrt(S_KDW_CO)&&!RdPrt(S_OFF)&&!RdPrt(S_SET)&&!RdPrt(S_KOM)&&!PFNf_off&&!PFNf_fnserw&&!PFNf_aleg&&!PFNf_set_fnserw&&!PFNf_set_aleg&&!PFNf_set_ncw&&!PFNf_prODP)            
        {
            key=PresKeyEX(RdPrt(S_KDW_CW),&ky[SS_KDW_CW]);
            key2=PresKeyEX(RdPrt(S_KUP_CW),&ky[SS_KUP_CW]);

            if(key==_KDFST||(key>=_TKSET)||key2==_KDFST||(key2>=_TKSET))		
            {
                ResBufKeyEx(&ky[SS_KDW_CW]);		//resetuj podtrzymanie
                ResBufKeyEx(&ky[SS_KUP_CW]);		//resetuj podtrzymanie
                RestartKol2(_KL_NCWCO);
                RestartKol(_KL_NCWCO);
                PFNf_b_set_ncw=0;
                PFNf_prn_res=0;
                PFNf_set_ncw=1;
                PFNf_mod_ncw=0;
                StartRTdS(_RTSET);
            }
        }
//---------------------
        //nacisnieto przycisk UP lub DW (zmiana nastawy CO lub nastawy ECO)
        //if(!RdPrt(S_OFF)&&!RdPrt(S_SET)&&!RdPrt(S_KOM)&&!PFNf_off&&!PFNf_fnserw&&!PFNf_aleg&&!PFNf_set_fnserw&&!PFNf_set_aleg&&!PFNf_set_nco&&!PFNf_set_ncw&&!PFNf_set_eco&&!PFNf_prODP)
        if(!RdPrt(S_KUP_CW)&&!RdPrt(S_KDW_CW)&&!RdPrt(S_OFF)&&!RdPrt(S_SET)&&!RdPrt(S_KOM)&&!PFNf_off&&!PFNf_fnserw&&!PFNf_aleg&&!PFNf_set_fnserw&&!PFNf_set_aleg&&!PFNf_set_nco&&!PFNf_set_eco&&!PFNf_prODP)            
        {
            key=PresKeyEX(RdPrt(S_KDW_CO),&ky[SS_KDW_CO]);
            key2=PresKeyEX(RdPrt(S_KUP_CO),&ky[SS_KUP_CO]);

            EnEco=(PFNf_ofzima&&PFNf_pmpPWM&&PFNf_trbECO&&!PFNf_errIN&&!PFNf_obgCW)?1:0;            

            if(key==_KDFST||(!EnEco&&key>=_TKSET)||key2==_KDFST||(!EnEco&&key2>=_TKSET))		
            {
                ResBufKeyEx(&ky[SS_KDW_CO]);		//resetuj podtrzymanie
                ResBufKeyEx(&ky[SS_KUP_CO]);		//resetuj podtrzymanie
                RestartKol2(_KL_NCWCO);
                RestartKol(_KL_NCWCO);
                PFNf_b_set_nco=0;
                PFNf_prn_res=0;
                PFNf_set_nco=1;
                StartRTdS(_RTSET);
            }
            else
            if((EnEco&&key>=(_TKSET*2))||(EnEco&&key2>=(_TKSET*2)))		
            {
                ResBufKeyEx(&ky[SS_KDW_CO]);		//resetuj podtrzymanie
                ResBufKeyEx(&ky[SS_KUP_CO]);		//resetuj podtrzymanie
                RestartKol2(_KL_NCWCO);
                RestartKol(_KL_NCWCO);
                PFNf_b_set_eco=0;
                PFNf_prn_res=0;
                PFNf_set_eco=1;
                PFNf_mod_eco=0;
                StartRTdS(_RTSET);
            }
        }
        else
        {
            if(!PFNf_off&&PFNf_prODP)
            {
                WaitToLowEx(&ky[SS_KDW_CO]);  
                WaitToLowEx(&ky[SS_KUP_CO]);
            }
        }
//--------------------------FUNKCJE SPECJALNE------------------------------

        tm=_TPCS;

        if(RTdS(_RTPCS)>tm)									//uplynal czas podtrzymania wskazu cisnienia?
        {
            PFNf_prn_res=0;
        }
		
        if(!PFNf_prn_res)
        {
            PFNf_prn_cs=0;
            PFNf_prn_in=0;
            PFNf_prn_pp=0;
            PFNf_prn_pg=0;
        }
        else
        {
            if(RTdS(_RTPCS)<(tm/4))
            {
                PFNf_prn_cs=1;			
                PFNf_prn_in=0;
                PFNf_prn_pp=0;
                PFNf_prn_pg=0;
            }
            else
            if((RTdS(_RTPCS)>=(tm/4))&&(RTdS(_RTPCS)<(tm/2)))		//nadszedl czas podtrzymania wskazu temp wlotowej?
            {
                PFNf_prn_cs=0;
                PFNf_prn_in=1;
                PFNf_prn_pp=0;
                PFNf_prn_pg=0;
            }
            else
            if((RTdS(_RTPCS)>=(tm/2))&&(RTdS(_RTPCS)<((3*tm)/4)))
            {
                PFNf_prn_cs=0;
                PFNf_prn_in=0;
                PFNf_prn_pp=1;
                PFNf_prn_pg=0;
            }
            else
            if((RTdS(_RTPCS)>=((3*tm)/4)))
            {
                PFNf_prn_cs=0;
                PFNf_prn_in=0;
                PFNf_prn_pp=0;
                PFNf_prn_pg=1;
            }				
        }      
//---------------------
        if(!PFNf_off&&(PFNf_fnserw||PFNf_ofnserw))					//aktywna funkcja serwisowa?
        {
            if(PresKeyEX(RdPrt(S_KUP_CO),&ky[SS_KUP_CO])>0)	//nacisnieto przycisk +?
            {
                ResBufKeyEx(&ky[SS_KUP_CO]);		//resetuj podtrzymanie
                PFNf_fns_max=1;
            }
            if(PresKeyEX(RdPrt(S_KDW_CO),&ky[SS_KDW_CO])>0)	//nacisnieto przycisk -?
            {
                ResBufKeyEx(&ky[SS_KDW_CO]);		//resetuj podtrzymanie
                PFNf_fns_max=0;
            }
        }
//--------------------------RESET------------------------------
        //nacisnieto przycisk RESET
        key=PresKeyEX(RdPrt(S_OFF),&ky[SS_OFF]);
        if(key==_KDFST)
        {
            if(PFNf_set_nco||PFNf_set_ncw||PFNf_set_eco||PFNf_str_nco||PFNf_str_ncw||PFNf_prn_res||PFNf_set_fnserw||PFNf_fnserw||PFNf_set_aleg||PFNf_aleg||PFNf_prODP)
            {
                WaitToLowEx(&ky[SS_OFF]);
                PFNf_set_nco=0;
                PFNf_set_ncw=0;
                PFNf_str_nco=0;
                PFNf_str_ncw=0;
                PFNf_mod_ncw=0;
                PFNf_set_eco=0;
                PFNf_mod_eco=0;
                PFNf_prn_res=0;
                PFNf_set_fnserw=0;
                PFNf_fnserw=0;
                PFNf_set_aleg=0;
                PFNf_aleg=0;
            }
            else
            if(!PFNf_off&&!PFNf_ferr&&!PFNf_fmsg&&!PFNf_prODP)
            {
                WaitToLowEx(&ky[SS_OFF]);
                if(!PFNf_errCS||(PFNf_fnoCS&&!PFNf_errIN))
                {
                    PFNf_prn_res=1;
                }
                StartRTdS(_RTPCS);
            }
        }
	}
	else
	{
		PFNf_set_nco=0;
		PFNf_set_ncw=0;
		PFNf_str_nco=0;
		PFNf_str_ncw=0;
		PFNf_set_eco=0;
		PFNf_prn_res=0;
		PFNf_set_fnserw=0;
		PFNf_fnserw=0;
		PFNf_set_aleg=0;
		PFNf_aleg=0;
	}
    
    if(!PFNf_opgres) PFNf_LINres=0;
    if(!PFNf_fsetLaddr) //nieaktywny tryb przypisania adresu LIN?
    {
        //stan przycisku RESET
        if(RdPrt(S_OFF)||PFNf_LINres)
        {
                PFNf_res=1;
        }
        else PFNf_res=0;
    }
}
#endif




