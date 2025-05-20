/*eeprom.c*/
//Funkcje zapisu/odczytu daneych do/z pamieci EEPROM
/*---------------------------------------------------------------------------------*/
#include <xc.h>
#include <stdbool.h>
#include <stdint.h>

#include "../../proj.tp.h"

#ifndef _TYP_PRJ
#error NIEOKRESLONY IDENTYFIKATOR PROJEKTU
#endif

#if _TYP_PRJ==3
#advisory _TYP_PRJ==3 
#include "../../mcc_generated_files\mcc.h"

/*---------------------------------------------------------------------------------*/
//---------------------------------------------------
//Zapis danej do pamieci EEPROM pod wskazany adres
/*void inline WriteEEPROM(unsigned int adr,unsigned char data)
{
	DATAEE_WriteByte(adr,data);
}*/
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Zapis danej w pamieci EEPROM po sprawdzeniu warunku roznicy miedzy dana do zapisu a dana aktualnie zapisana w EEPROM
void if_WriteEEPROM(unsigned int adr, unsigned char dta)
{
    if(DATAEE_ReadByte(adr)!=dta) 
    {
        DATAEE_WriteByte(adr,dta);
    }
}
//---------------------------------------------------
//Odczyt danej z pamieci EEPROM pod wskazanym adresem
unsigned char ReadEEPROM(unsigned int adr)
{
	volatile unsigned char dta;
    
	dta=DATAEE_ReadByte(adr);
	return dta;
}
//---------------------------------------------------
#endif