/*ammy1.h*/
#ifndef	_AMMY1_H
#define	_AMMY1_H
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
typedef struct 	{
				unsigned char KOD;		//kod odbieranego urzadzenia
				unsigned char STAT;		//slowo statusowe
				unsigned char AC_VMMY3;
				unsigned char AC_VMMY5;
				unsigned char AC_VMMY18;
				unsigned char AC_VCS;
				unsigned char AC_IMOD;
				unsigned int IMOD;				//wartosc pradu modulatora
				unsigned char IMODH;			//starszy bajt wartosci pradu
				unsigned char VMMY3;			//wartosc napiecia 5V MMY
				unsigned char VMMY5;			//wartosc napiecia 5V MMY			
				unsigned char VMMY18;			//wartosc napiecia 18V MMY
				unsigned char VRT;				//wartosc HEX napiecia VRT MMY	
				unsigned char VPLM;				//wartosc HEX napiecia VRT MMY	
				unsigned char VCS;				//wartosc HEX napiecia VCS MMY	

				unsigned char TPOM0;	//stany zmierzone
				unsigned char TPOM1;	//stany zmierzone
				unsigned char TPOM2;	//stany zmierzone
				unsigned char TPOM3;	//stany zmierzone
				}tpAMMY1;
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//STAT
#define _stCLR		0
//TPOM0
#define _V3			0
#define _V5			1
#define _V18		2
#define _VRT		3
#define _pPLM		4
#define _pPLM2		5
#define _pIMOD		6
#define _VCS		7

//TPOM1
#define _HiVRT		0
#define _LoVRT		1
#define _HiVMMY18	2
#define _LoVMMY18	3
#define _LoVMMY5	4
#define _HiVMMY5	5
#define _LoVMMY3	6
#define _HiVMMY3	7

//TPOM2
#define _pPOMPA		0
#define _pPMP_CO	1
#define _pPMP_CW	2
#define _pVNT		3
#define _pZGZ		4
#define _pALARM		5
#define _pGEN		6
#define _pFAZA		7

//TPOM3
#define _HiIMOD		0
#define _LoIMOD		1
#define _HiVCS		2
#define _LoVCS		3


#endif //_AMMY1_H
