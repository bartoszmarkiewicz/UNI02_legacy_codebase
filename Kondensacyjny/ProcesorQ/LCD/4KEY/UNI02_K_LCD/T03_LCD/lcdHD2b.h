/*lcdHD2.h*/
/*Wyswietlacz LCD firmy TESTER typ GZ0401M9*/
#ifndef	_LCDHD2_H
#define	_LCDHD2_H
//#include "lcdHD1.tp.h"		//typy danych

//Mapa polaczen LCD - PIC 
/*
s1=>s12
s2=>s11
s3=>s10
s4=>s9
s5=>s8
s6=>s7
s16=>s1
s17=>s2
s19=>s3
s20=>s4
s21=>s5
s22=>s6
*/
//cyfry wyswietlacza 
//najstarsza cyfra
#define _LCD_4A		S12C3	//S2->SEG14
#define _LCD_4B		S12C2	//S2->SEG14
#define _LCD_4C		S12C1	//S2->SEG14		
#define _LCD_4D		S12C0	//S2->SEG14
#define _LCD_4E		S14C0	//S3->SEG12
#define _LCD_4F		S14C2	//S3->SEG12
#define _LCD_4G		S14C1	//S3->SEG12

#define _LCD_3A		S11C3	//S4->SEG29	
#define _LCD_3B		S11C2	//S4->SEG29
#define _LCD_3C		S11C1	//S4->SEG29			
#define _LCD_3D		S11C0	//S4->SEG29
#define _LCD_3E		S29C0	//S5->SEG11
#define _LCD_3F		S29C2	//S5->SEG11
#define _LCD_3G		S29C1	//S5->SEG11

#define _LCD_2A		S24C3	//S6->SEG10
#define _LCD_2B		S24C2	//S6->SEG10
#define _LCD_2C		S24C1	//S6->SEG10
#define _LCD_2D		S24C0	//S6->SEG10
#define _LCD_2E		S25C0	//S7->SEG9
#define _LCD_2F		S25C2	//S7->SEG9
#define _LCD_2G		S25C1	//S7->SEG9 
//najmlodsza cyfra
#define _LCD_1A		S22C3	//S8->SEG8
#define _LCD_1B		S22C2	//S8->SEG8
#define _LCD_1C		S22C1	//S8->SEG8
#define _LCD_1D		S22C0	//S8->SEG8
#define _LCD_1E		S23C0	//S9->SEG30
#define _LCD_1F		S23C2	//S9->SEG30
#define _LCD_1G		S23C1	//S9->SEG30


	
//kropka
#define _LCD_P1		S20C0

//regulacja pogodowa
#define _LCD_PGD	S19C1

//plomien CO
#define _LCD_PLC	S19C0

//plomien CW
#define _LCD_PLW	S15C0

//kreski
#define _LCD_KRS	S19C2

//kran
#define _LCD_KRN	S15C3

//kaloryfer
#define _LCD_KAL	S19C3

//max CO
#define _LCD_MXC	S25C3

//min C0
#define _LCD_MNC	S23C3

//max CW
#define _LCD_MXW	S14C3

//min CW
#define _LCD_MNW	S29C3

//bar
#define _LCD_BAR	S20C1

//temp CO
#define _LCD_TCO	S20C2

//temp CW
#define _LCD_TCW	S10C3

//RESET
#define _LCD_RES	S10C1

//zasilanie
#define _LCD_OFF	S10C2

//serwis, konfiguracja
#define _LCD_SRV	S15C1

//----------------------------------------------------------
void FormatLcdData(void);
//----------------------------------------------------------
#endif /*_LCDHD2_H*/