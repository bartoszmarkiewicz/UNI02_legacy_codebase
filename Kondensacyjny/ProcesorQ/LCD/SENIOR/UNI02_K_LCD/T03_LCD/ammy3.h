/*ammy3.h*/
#ifndef	_AMMY3_H
#define	_AMMY3_H
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//ODBIOR:
//adres slavepan,KOD,STAT,COmin,COmax,CWmin,CWmax,CSmin,CSmax,ZWmin,ZWmax
//,PRZmin,PRZmax, MPOB0, MPOB1, MPOT, CHK
//NADAWANIE
//adres master,ACK, KOD, STAT, CO, CW, CS, ZW, PRZ, TP0B0, TP0B1, TP0B2, 
//,CHK
typedef struct 	{
				unsigned char KOD;		//kod odbieranego urzadzenia
				unsigned char STAT;		//slowo statusowe
				signed char CO;		//temperatura CO
				signed char COmin;		//zakres
				signed char COmax;		//zakres		
				signed char CW;		//temperatura CW
				signed char CWmin;		//zakres
				signed char CWmax;		//zakres		
				signed char CS;		//cisnienie CS
				signed char CSmin;		//zakres
				signed char CSmax;		//zakres
				signed char ZW;		//typ modulatora
				signed char ZWmin;		//zakres
				signed char ZWmax;		//zakres
				signed char PRZ;		//przeplyw CW (*10)
				signed char PRZmin;		//zakres
				signed char PRZmax;		//zakres

				unsigned char MPOT;		//maska potencjometrow
				unsigned char MPOB0;	//maska stany pobudzen
				unsigned char MPOB1;	//maska stany pobudzen
				unsigned char MPOB2;	//maska stany pobudzen
				unsigned char TPOB0;	//stany pobudzen
				unsigned char TPOB1;	//stany pobudzen
				unsigned char TPOB2;	//stany pobudzen
				}tpAMMY3;
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//STAT
#define _stCLR		0

#define _bpPCO		0
#define _bpPCW		1
#define _bpPCS		2
#define _bpPZW		3
#define _bpPPR		4


//TPOB0
#define _bpE4		7
#define _bpE3		6
#define _bpE2		5
#define _bpE1		4
#define _bpE0		3
#define _bpZW		2
#define _bpCW		1
#define _bpRT		0
//TPOB1
#define _bpE12		7
#define _bpE11		6
#define _bpE10		5
#define _bpE9		4
#define _bpE8		3
#define _bpE7		2
#define _bpE6		1
#define _bpE5		0
//TPOB2

#endif //_AMMY3_H
