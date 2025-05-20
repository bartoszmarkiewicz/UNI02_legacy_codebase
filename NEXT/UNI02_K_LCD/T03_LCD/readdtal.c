/*readdta.c*/
//Funkcja odczytu danej z keypadu
// (!) Bazuje na funkcjach z plikow:  lcd8.c, keypad.c
/*---------------------------------------------------------------------------------*/
//include <pic18.h>
#include <xc.h>
#include <stdio.h>
#include "global.h"			//parametry globalne
#include "readdtal.h"		//parametry lokalne

extern volatile StPin xxky[];
extern volatile unsigned char x;
/*---------------------------------------------------------------------------------*/
//Odczyt danej z keypadu z potwierdzeniem
//dt - wskaznik do modyfikowanego parametru
//min - minimalna wartosc jaka moze przyjac parametr
//max - maksymalna wartosc jaka moze przyjac parametr
//step - krok z jakim odbywa sie inkrementacja/dekrementacja
//np. akceptacja=ReadData(&x,1,3,1,wr);
#if (_ENRDT)
unsigned char ReadData(unsigned char k,volatile StPin *ky,unsigned char *dt,unsigned char min,unsigned char max,const unsigned char step,unsigned char wrn)
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
		if(ExPresKey(k,ky,_RDTOFS,_RDKRFS)) 
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
//Odczyt danej z keypadu z potwierdzeniem /regulowany krok - wersja dla danych o duzych wartosciach/
//dt - wskaznik do modyfikowanego parametru
//min - minimalna wartosc jaka moze przyjac parametr
//max - maksymalna wartosc jaka moze przyjac parametr
//step - krok z jakim odbywa sie inkrementacja/dekrementacja przy pojedynczym nacisnieciu przycisku
//step2 - krok z jakim odbywa sie inkrementacja/dekrementacja przy wcisnietym przycisku
//np. akceptacja=ReadDataX(&x,1,100,1,5,wr);
#if (_ENRDTX)
unsigned char ReadDataX(unsigned char k,volatile StPin *ky,unsigned char *dt,unsigned char min,unsigned char max,const unsigned char step,const unsigned char step2,unsigned char wrn)
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
		if(key=ExxPresKey1(k,ky,_RDTOFSX,ktu)) 
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
unsigned char ReadDataONL(unsigned char k,volatile StPin *ky,unsigned char *dt,unsigned char min,unsigned char max,const unsigned char step,unsigned char wrn)
{
	if(*dt<min) *dt=min;
	if(ExPresKey(k,ky,_RDTOFSNL,_RDKRFSNL)) 
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
//Odczyt danej z keypadu (modyfikacja zmiennej dt "w locie") /regulowany krok - wersja dla danych o duzych wartosciach/
//dt - wskaznik do modyfikowanego parametru
//min - minimalna wartosc jaka moze przyjac parametr
//max - maksymalna wartosc jaka moze przyjac parametr
//step - krok z jakim odbywa sie inkrementacja przy pojedynczym nacisnieciu przycisku
//step2 - krok z jakim odbywa sie inkrementacja przy wcisnietym przycisku
//np. akceptacja=ReadDataXONL(&x,1,100,1,5,wr);
#if (_ENRDTOXL)
unsigned char ReadDataXONL(unsigned char k,volatile StPin *ky,unsigned char *dt,unsigned char min,unsigned char max,const unsigned char step,const unsigned char step2,unsigned char wrn)
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
	if(key=ExxPresKey(k,ky,_RDTOFSXL,ktu)) 
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
/*---------------------------------------------------------------------------------*/
//Odczyt danej z keypadu (modyfikacja zmiennej dt "w locie") /regulowany krok - wersja dla danych o duzych wartosciach/
//dt - wskaznik do modyfikowanego parametru
//ovf - dla 1 inkrementacja/dekrementacja na okraglo
//min - minimalna wartosc jaka moze przyjac parametr
//max - maksymalna wartosc jaka moze przyjac parametr
//step - krok z jakim odbywa sie inkrementacja przy pojedynczym nacisnieciu przycisku
//step2 - krok z jakim docelowo odbywa sie inkrementacja przy wcisnietym przycisku (dla fst=1)
//fst - aktywacja funkcji zwiekszania czestotliwosci kroków oraz zwiekszania ich amplitudy z poziomu step do step2 przy wcisnietym przycisku
//ReadDataXONLI(RdPrt(S_KUP),&xxky[S_KUP],&param,&ov,max,1,stp,1);
//ReadDataXONLD(RdPrt(S_KDW),&xxky[S_KDW],&param,&ov,min,1,stp,1);
#if (_ENRDTOXLI!=0)
//INKREMENTACJA
unsigned char ReadDataXONLI(unsigned char k,volatile StPin *ky,unsigned int *dt,unsigned char *ovf,unsigned int max,const unsigned char step,const unsigned char step2,const unsigned char fst)
{
	static bit dtmo=0;
	static unsigned char ktu,iu,pp,es2;
	char key=0;

	if(!dtmo) 						//pierwsze uruchomienie procedury
	{
		dtmo=1;
		ktu=0;
		iu=0;
		es2=0;
		pp=step;
	}
//-------
	if(key=ExxPresKey(k,ky,_RDTOFSXLI,ktu)) 
	{
		if(key==1||((key==2)&&!fst))
		{
			iu=0;
			ktu=5;
			es2=0;
			pp=step;
			if((*dt+pp)>max) *ovf=1;
			else *ovf=0;
			if(*dt+pp<=max) *dt=*dt+pp;
			else *dt=max;
	
		}
		if(key==2&&fst) 
		{
			if (iu<=(_RDKRFSXLI-1)*_VKRFSXLI) iu++;
			ktu=_RDKRFSXLI-iu/_VKRFSXLI;				//czas miedzy kolejnymi impulsami inkrementacji
			if (iu>(_RDKRFSXLI-1)*_VKRFSXLI)			//przytrzymany przycisk przez dluzszy czas?
			{
				if(es2)
				{
					if(pp<step2) pp++;
				}
				else pp=step;
				if((*dt%10)==0) es2=1;
			}
			else
			{
				es2=0;
				pp=step;
			}
			if((*dt+pp)>max) *ovf=1;
			else *ovf=0;
			if(*dt+pp<=max) *dt=*dt+pp;
			else *dt=max;
		}
		if(*dt>max) *dt=max;
		return 1;
	}
	else 
	{
		iu=0;
		ktu=5;
		es2=0;
		pp=step;
	}
	return 0x00;
}
#endif
#if (_ENRDTOXLD!=0)
//DEKREMENTACJA
unsigned char ReadDataXONLD(unsigned char k,volatile StPin *ky,unsigned int *dt,unsigned char *ovf, unsigned int min,const unsigned char step,const unsigned char step2,const unsigned char fst)
{
	static bit dtmo=0;
	static unsigned char ktu,iu,pp,es2;
	char key=0;

	if(!dtmo) 						//pierwsze uruchomienie procedury
	{
		dtmo=1;
		ktu=0;
		iu=0;
		es2=0;
		pp=step;
	}
//-------
	if(key=ExxPresKey(k,ky,_RDTOFSXLD,ktu)) 
	{
		if(key==1||((key==2)&&!fst))
		{
			iu=0;
			ktu=5;
			es2=0;
			pp=step;
			if(!*dt||((*dt-pp)<min)) *ovf=1;
			else *ovf=0;
			if(*dt&&((*dt-pp)>min)) *dt=*dt-pp;
			else *dt=min;
		}
		if(key==2&&fst)  
		{
			if (iu<=(_RDKRFSXLD-1)*_VKRFSXLD) iu++;
			ktu=_RDKRFSXLD-iu/_VKRFSXLD;				//czas miedzy kolejnymi impulsami inkrementacji
			if (iu>(_RDKRFSXLD-1)*_VKRFSXLD)
			{
				if(es2)
				{
					if(pp<step2) pp++;
				}
				else pp=step;
				if((*dt%10)==0) es2=1;
			}
			else
			{
				es2=0;
				pp=step;
			}
			if(!*dt||((*dt-pp)<min)) *ovf=1;
			else *ovf=0;
			if(*dt&&((*dt-pp)>min)) *dt=*dt-pp;
			else *dt=min;
		}
		if(*dt<min) *dt=min;
		return 1;
	}
	else 
	{
		iu=0;
		ktu=5;
		es2=0;
		pp=step;
	}
	return 0x00;
}
#endif
/*---------------------------------------------------------------------------------*/
//Odczyt danej z keypadu (modyfikacja zmiennej dt "w locie") /regulowany krok - wersja dla danych o duzych wartosciach/
//dt - wskaznik do modyfikowanego parametru
//min - minimalna wartosc jaka moze przyjac parametr
//max - maksymalna wartosc jaka moze przyjac parametr
//ovf - dla 1 inkrementacja/dekrementacja na okraglo
//tid - dla 1 inkrementacja dla 0 dekrementacja
//step - krok z jakim odbywa sie inkrementacja przy pojedynczym nacisnieciu przycisku
//step2 - krok z jakim docelowo odbywa sie inkrementacja przy wcisnietym przycisku (dla fst=1)
//fst - aktywacja funkcji zwiekszania czestotliwosci kroków oraz zwiekszania ich amplitudy z poziomu step do step2 przy wcisnietym przycisku
//ReadDataXONLID(RdPrt(S_KDW),&xxky[S_KDW],param,&ov,0,min,max,1,stp,fst);
#if (_ENRDTOXLID!=0)
unsigned char ReadDataXONLID(unsigned char k,volatile StPin *ky,unsigned int *dt,unsigned char *ovf, unsigned char tid, unsigned int min, unsigned int max, const unsigned char step,const unsigned char step2,const unsigned char fst)
{
	static bit dtmo=0;
	static unsigned char ktu,iu,pp,es2;
	char key=0;

	if(!dtmo) 						//pierwsze uruchomienie procedury
	{
		dtmo=1;
		ktu=0;
		iu=0;
		es2=0;
		pp=step;
	}
//-------
	if(key=ExxPresKey(k,ky,_RDTOFSXLID,ktu)) 
	{
		if(key==1||((key==2)&&!fst))
		{
			iu=0;
			ktu=5;
			es2=0;
			pp=step;
			if(!tid)
			{
				if(!*dt||((*dt-pp)<min)) *ovf=1;
				else *ovf=0;
				if(*dt&&((*dt-pp)>min)) *dt=*dt-pp;
				else *dt=min;
			}
			else
			{
				if((*dt+pp)>max) *ovf=1;
				else *ovf=0;
				if(*dt+pp<=max) *dt=*dt+pp;
				else *dt=max;
			}
		}
		if(key==2&&fst)  
		{
			if (iu<=(_RDKRFSXLID-1)*_VKRFSXLID) iu++;
			ktu=_RDKRFSXLID-iu/_VKRFSXLID;				//czas miedzy kolejnymi impulsami inkrementacji
			if (iu>(_RDKRFSXLID-1)*_VKRFSXLID)
			{
				if(es2)
				{
					if(pp<step2) pp++;
				}
				else pp=step;
				if((*dt%10)==0) es2=1;
			}
			else
			{
				es2=0;
				pp=step;
			}
			if(!tid)
			{
				if(!*dt||((*dt-pp)<min)) *ovf=1;
				else *ovf=0;
				if(*dt&&((*dt-pp)>min)) *dt=*dt-pp;
				else *dt=min;
			}
			else
			{
				if((*dt+pp)>max) *ovf=1;
				else *ovf=0;
				if(*dt+pp<=max) *dt=*dt+pp;
				else *dt=max;
			}

		}
		if(*dt<min) *dt=min;
		if(*dt>max) *dt=max;
		return 1;
	}
	else 
	{
		iu=0;
		ktu=5;
		es2=0;
		pp=step;
	}
	return 0x00;
}
#endif