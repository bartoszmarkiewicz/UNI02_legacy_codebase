/*eeprom.c*/
//Funkcje zapisu/odczytu daneych do/z pamieci EEPROM
/*---------------------------------------------------------------------------------*/
#include <pic18.h>
#include <htc.h>
#include "eeprom.h"
/*---------------------------------------------------------------------------------*/
//---------------------------------------------------
//Zapis danej do pamieci EEPROM pod wskazany adres
void WriteEEPROM(unsigned int adr,unsigned char data)
{
	eeprom_write(adr,data);
}
//---------------------------------------------------
//Odczyt danej z pamieci EEPROM pod wskazanym adresem
unsigned char ReadEEPROM(unsigned int adr)
{
	unsigned char data;
	data=eeprom_read(adr);
	return data;
}
//---------------------------------------------------
