/*main.h*/
#ifndef	_HEEPROM_H
#define	_HEEPROM_H
#include "proj.tp.h"

#ifndef _TYP_PRJ
#error NIEOKRESLONY IDENTYFIKATOR PROJEKTU
#endif

#if _TYP_PRJ==1
//#advisory _TYP_PRJ==1
#include "PRJ\UNI02_K_STR\eeprom.h"
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//--------------------------KOTLY ATMOSFERYCZNE----------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
#elif _TYP_PRJ==2
//#advisory _TYP_PRJ==2
#include "PRJ\UNI02_A_STR\eeprom.h"
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//--------------------------PODGRZEWACZ GRZ02----------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
#elif _TYP_PRJ==3
//#advisory _TYP_PRJ==3
#include "PRJ\GRZ02\eeprom.h"
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//--------------------------PODGRZEWACZ GRZ04----------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
#elif _TYP_PRJ==4
//#advisory _TYP_PRJ==4
#include "PRJ\GRZ04\eeprom.h"
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
#else
#error NIEPRAWIDLOWY IDENTYFIKATOR PROJEKTU
#endif
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//----------------------------------------------------------
#endif		/*_HEEPROM_H*/
