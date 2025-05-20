/*IMZ.tp.h*/
/*IMZ-xx*/
#ifndef	_IMZ_TP_H
#define	_IMZ_TP_H
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
typedef struct 	{
				unsigned bFls:1;
				unsigned ClrIMZ:1;					//znacznik braku modulu MMY
				volatile unsigned char timIMZ;		//timer na potrzeby pulsacji wskazan
				unsigned char krok;
//-----------
				signed char CO;					//kolejne wielkosci wyswietlane na wyswietlaczach
				signed char CW;
				signed char PRZ;
				signed char CS;
				signed char ZW;
//-----------wyswietlacze----------------------------------
				unsigned char Config0;				//konfiguracja kolejnych ukladow SAA
				unsigned char Config1;
				unsigned char Config2;
				unsigned char Config3;
//-----------
				unsigned char Status0;				//szczegolowe parametry wyswietlania dla kolejnych ukladow SAA
				unsigned char Status1;
				unsigned char Status2;
				unsigned char Status3;
//-----------diody-----------------------------------------
				unsigned char WeWy0;				//slowo wysterowania kolejnych diod WE/WY
				unsigned char WeWy1;
				unsigned char WeWy2;
				unsigned char WeWy3;
//-----------
				unsigned char FStat0;				//bity pulsacji dla kolejnych diod
				unsigned char FStat1;
				unsigned char FStat2;
				unsigned char FStat3;
				}tpIMZ;

/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
#endif //_IMZ_TP_H
