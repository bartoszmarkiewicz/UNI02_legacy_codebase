/*lcdHD2.h*/
/*Wyswietlacz LCD firmy TESTER typ GZ0401M9*/
#ifndef	_LCDHD2_H
#define	_LCDHD2_H
//#include "lcdHD1.tp.h"		//typy danych

//Mapa polaczen LCD - PIC 

//cyfry wyswietlacza 
//najmlodsza cyfra
#define _LCD_1A		S03C3	//S2->SEG14
#define _LCD_1B		S03C2	//S2->SEG14
#define _LCD_1C		S03C1	//S2->SEG14		
#define _LCD_1D		S03C0	//S2->SEG14
#define _LCD_1E		S04C0	//S3->SEG12
#define _LCD_1F		S04C2	//S3->SEG12
#define _LCD_1G		S04C1	//S3->SEG12

#define _LCD_2A		S05C3	//S4->SEG29	
#define _LCD_2B		S05C2	//S4->SEG29
#define _LCD_2C		S05C1	//S4->SEG29			
#define _LCD_2D		S05C0	//S4->SEG29
#define _LCD_2E		S06C0	//S5->SEG11
#define _LCD_2F		S06C2	//S5->SEG11
#define _LCD_2G		S06C1	//S5->SEG11

#define _LCD_3A		S08C3	//S6->SEG10
#define _LCD_3B		S08C2	//S6->SEG10
#define _LCD_3C		S08C1	//S6->SEG10
#define _LCD_3D		S08C0	//S6->SEG10
#define _LCD_3E		S09C0	//S7->SEG9
#define _LCD_3F		S09C2	//S7->SEG9
#define _LCD_3G		S09C1	//S7->SEG9 
//najstarsza cyfra
#define _LCD_4A		S10C3	//S8->SEG8
#define _LCD_4B		S10C2	//S8->SEG8
#define _LCD_4C		S10C1	//S8->SEG8
#define _LCD_4D		S10C0	//S8->SEG8
#define _LCD_4E		S30C0	//S9->SEG30
#define _LCD_4F		S30C2	//S9->SEG30
#define _LCD_4G		S30C1	//S9->SEG30


	
//kropka
#define _LCD_P1		S02C0

//regulacja pogodowa
#define _LCD_PGD	S01C0

//plomien CO
#define _LCD_PLC	S01C0

//plomien CW
#define _LCD_PLW	S12C0

//kreski
#define _LCD_KRS	S01C2

//kran
#define _LCD_KRN	S12C3

//kaloryfer
#define _LCD_KAL	S01C3

//max CO
#define _LCD_MXC	S06C3

//min C0
#define _LCD_MNC	S04C3

//max CW
#define _LCD_MXW	S11C3

//min CW
#define _LCD_MNW	S09C3

//bar
#define _LCD_BAR	S02C1

//temp CO
#define _LCD_TCO	S02C2

//temp CW
#define _LCD_TCW	S07C3

//RESET
#define _LCD_RES	S07C1

//zasilanie
#define _LCD_OFF	S07C2

//serwis, konfiguracja
#define _LCD_VNT1	S12C1


//----------------------------------------------------------
void FormatLcdData(void);
//----------------------------------------------------------
#endif /*_LCDHD2_H*/