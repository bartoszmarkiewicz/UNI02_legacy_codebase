/*readdta.c*/
//Funkcja odczytu danej z keypadu
// (!) Bazuje na funkcjach z plikow: rtimer.c /RTS(1)/, lcd8.c, keypad.c
/*---------------------------------------------------------------------------------*/
#include <pic18.h>
#include <stdio.h>
#include "global.h"			//parametry globalne
#include "readdtal.h"		//parametry lokalne
#include "pomiar.h"
extern DataPFN PFN;
/*---------------------------------------------------------------------------------*/
//Odczyt danej z keypadu
//dt - wskaznik do modyfikowanego parametru
//min - minimalna wartosc jaka moze przyjac parametr
//max - maksymalna wartosc jaka moze przyjac parametr
//step - krok z jakim odbywa sie inkrementacja/dekrementacja
//np. akceptacja=ReadData(&x,1,3,1,wr);
#if (_ENRDT)
unsigned char ReadData(unsigned char k,unsigned char *dt,unsigned char min,unsigned char max,const unsigned char step,unsigned char wrn)
{
	static unsigned char bd;
	static bit dtmo=0,okvi=0;
	
	if(!dtmo) 						//pierwsze uruchomienie procedury
	{
		bd=*dt;
		dtmo=1;
		okvi=0;
	}
	if(!okvi)						//akceptacja danej?
	{
		if(bd<min) bd=min;
		if(ExPresKey(k,_RDTOFS,_RDKRFS)) 
		{
			bd=bd+step;
		}
		if(bd>max) bd=min;
		PrintCO(bd);
	
		if(wrn) okvi=1;				//spelniony warunek wyjscia?
	}
	else
	{
		dtmo=0;
		okvi=0;
		*dt=bd;
		return 1;					//zwroc znacznik potwierdzenia
	}	
	return 0x00;
}
#endif
/*---------------------------------------------------------------------------------*/
//Odczyt danej z keypadu /wersja dla danych o duzych wartosciach/
//dt - wskaznik do modyfikowanego parametru
//min - minimalna wartosc jaka moze przyjac parametr
//max - maksymalna wartosc jaka moze przyjac parametr
//step - krok z jakim odbywa sie inkrementacja/dekrementacja przy pojedynczym nacisnieciu przycisku
//step2 - krok z jakim odbywa sie inkrementacja/dekrementacja przy wcisnietym przycisku
//np. akceptacja=ReadDataX(&x,1,100,1,5,wr);
#if (_ENRDTX)
unsigned char ReadDataX(unsigned char k,unsigned char *dt,unsigned char min,unsigned char max,const unsigned char step,const unsigned char step2,unsigned char wrn)
{
	static bit dtmo=0,okvi=0;
	static unsigned char bd,iu;
	static unsigned char ktu;
	char key=0;
	
	if(!dtmo) 						//pierwsze uruchomienie procedury
	{
		iu=0;
		ktu=0;
		dtmo=1;
		okvi=0;
		bd=*dt;
	}
	if(!okvi)						//akceptacja danej?
	{
		if(bd<min) bd=min;
		if(key=ExPresKey2(k,_RDTOFSX,ktu)) 
		{
			if(key==1)
			{
				iu=0;
				ktu=5;
				bd=bd+step;
			}
			if(key==2) 
			{
				if (iu<(_RDKRFSX-1)*_VKRFSX) iu++;
				ktu=_RDKRFSX-iu/_VKRFSX;
				bd=bd+step2;
			}
		}
		else 
		{
			iu=0;
			ktu=5;
		}
		if(bd>max) bd=min;		
		PrintCO(bd);

		if(wrn) okvi=1;				//spelniony warunek wyjscia?
	}
	else
	{
		dtmo=0;
		okvi=0;
		*dt=bd;
		return 1;							//zwroc znacznik potwierdzenia
	}
	return 0x00;
}
#endif
/*---------------------------------------------------------------------------------*/
//Odczyt danej z keypadu (modyfikacja zmiennej dt "w locie")
//dt - wskaznik do modyfikowanego parametru
//min - minimalna wartosc jaka moze przyjac parametr
//max - maksymalna wartosc jaka moze przyjac parametr
//step - krok z jakim odbywa sie inkrementacja/dekrementacja
//np. akceptacja=ReadData(&x,1,100,1,wr);
#if (_ENRDTONL)
unsigned char ReadDataONL(unsigned char k,unsigned char *dt,unsigned char min,unsigned char max,const unsigned char step,unsigned char wrn)
{
	if(*dt<min) *dt=min;
	if(ExPresKey(k,_RDTOFSNL,_RDKRFSNL)) 
	{
		if(*dt>=max) *dt=min;
		else
		if(*dt+step<=max) *dt=*dt+step;
	}
	if(*dt>max) *dt=min;
	if(wrn) return 1;			//spelniony warunek wyjscia?
	return 0x00;
}
#endif
/*---------------------------------------------------------------------------------*/
//Odczyt danej z keypadu (modyfikacja zmiennej dt "w locie")
//dt - wskaznik do modyfikowanego parametru
//min - minimalna wartosc jaka moze przyjac parametr
//max - maksymalna wartosc jaka moze przyjac parametr
//step - krok z jakim odbywa sie inkrementacja/dekrementacja przy pojedynczym nacisnieciu przycisku
//step2 - krok z jakim odbywa sie inkrementacja/dekrementacja przy wcisnietym przycisku
//np. akceptacja=ReadDataXONL(&x,1,100,1,5,wr);
#if (_ENRDTOXL)
unsigned char ReadDataXONL(unsigned char k,unsigned char *dt,unsigned char min,unsigned char max,const unsigned char step,const unsigned char step2,unsigned char wrn)
{
	static bit dtmo=0;
	static unsigned char ktu,iu;
	char key=0;

	if(!dtmo) 						//pierwsze uruchomienie procedury
	{
		dtmo=1;
		ktu=0;
		iu=0;
	}
	if(*dt<min) *dt=min;
	if(key=ExPresKey2(k,_RDTOFSXL,ktu)) 
	{
		if(key==1)
		{
			iu=0;
			ktu=5;
			if(*dt>=max) *dt=min;
			else
			if(*dt+step<=max) *dt=*dt+step;
		}
		if(key==2) 
		{
			if (iu<=(_RDKRFSXL-1)*_VKRFSXL) iu++;
			ktu=_RDKRFSXL-iu/_VKRFSXL;				//czas miedzy kolejnymi impulsami inkrementacji
			if(*dt>=max) *dt=min;
			else
			if(*dt+step2<=max) *dt=*dt+step2;
		}
	}
	else 
	{
		iu=0;
		ktu=5;
	}
	if(*dt>max) *dt=min;
	if(wrn)					//spelniony warunek wyjscia?
	{
		dtmo=0;
		return 1;			//OK key?
	}
	return 0x00;
}
#endif
