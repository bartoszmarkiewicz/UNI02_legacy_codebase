/*eeprom.h*/
#ifndef	_EEPROM_H
#define	_EEPROM_H

//adresy danych przechowywanych w pamieci nieulotnej eeprom
#define _ADRCOMIN 0x0000		//min CO
#define _ADRCOMAX 0x0001		//max CO
#define _ADRCWMIN 0x0002		//min CW
#define _ADRCWMAX 0x0003		//max CW
#define _ADRZWMIN 0x0004		//min ZW
#define _ADRZWMAX 0x0005		//max ZW
#define _ADRCSMIN 0x0006		//min CS
#define _ADRCSMAX 0x0007		//max CS
#define _ADRPRMIN 0x0008		//min PRZ
#define _ADRPRMAX 0x0009		//max PRZ
#define _ADRTP0B0 0x000A		//key0
#define _ADRTP0B1 0x000B		//key1
#define _ADRMP0B0 0x000C		//maska key0
#define _ADRMP0B1 0x000D		//maska key1
#define _ADRMP0T  0x000E		//maska potencjometrow

//wartosci domyslne danych w pamieci (pierwsze programowanie)
#define _DTACOMIN	40			//min CO
#define _DTACOMAX	80			//max CO
#define _DTACWMIN	35			//min CW
#define _DTACWMAX	60			//max CW
#define _DTAZWMIN	-5			//min ZW
#define _DTAZWMAX	40			//max ZW
#define _DTACSMIN	0			//min CS
#define _DTACSMAX	40			//max CS
#define _DTAPRMIN	0			//min PRZ
#define _DTAPRMAX	40			//max PRZ
#define _DTATP0B0	0x00			//key0
#define _DTATP0B1	0x00			//key1
#define _DTAMP0B0	0xFF		//key0
#define _DTAMP0B1	0xFF		//key1
#define _DTAMP0T	0xFF		//maska potencjometrow

//----------
//Adres 0x0000
//Kod awaryjny, parametry konfiguracyjne kotla
__EEPROM_DATA(_DTACOMIN,_DTACOMAX,_DTACWMIN,_DTACWMAX,_DTAZWMIN,_DTAZWMAX,_DTACSMIN,_DTACSMAX);	
__EEPROM_DATA(_DTAPRMIN,_DTAPRMAX,_DTATP0B0,_DTATP0B1,_DTAMP0B0,_DTAMP0B1,_DTAMP0T,00);	
//Adres 0x0010
//----------------------------------------------------------
#endif		/*_EEPROM_H*/
