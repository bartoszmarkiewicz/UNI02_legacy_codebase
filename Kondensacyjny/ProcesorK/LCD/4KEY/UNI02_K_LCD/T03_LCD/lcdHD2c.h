/*lcdHD2c.h*/
/*Wyswietlacz LCD firmy TESTER typ GZ0401M9*/
#ifndef	_LCDHD2_H
#define	_LCDHD2_H
//#include "lcdHD1.tp.h"		//typy danych

//Mapa polaczen LCD - PIC 
//S2->SEG21
//S3->SEG22
//S4->SEG23
//S5->SEG24
//S6->SEG25
//S7->SEG2
//S8->SEG3
//S9->SEG4
//S10->SEG5
//S11->SEG6
//S12->SEG7
//COM0->COM3
//COM1->COM2
//COM2->COM1
//COM3->COM0

/*
//cyfry wyswietlacza (pierwsza wersja) 

//najmlodsza cyfra
#define _LCD_1A		S22C0	//S3
#define _LCD_1B		S22C1	//S3
#define _LCD_1C		S22C2	//S3	
#define _LCD_1D		S22C3	//S3
#define _LCD_1E		S23C3	//S4
#define _LCD_1F		S23C1	//S4
#define _LCD_1G		S23C2	//S4

#define _LCD_2A		S24C0	//S5
#define _LCD_2B		S24C1	//S5
#define _LCD_2C		S24C2	//S5			
#define _LCD_2D		S24C3	//S5
#define _LCD_2E		S25C3	//S6
#define _LCD_2F		S25C1	//S6
#define _LCD_2G		S25C2	//S6

#define _LCD_3A		S03C0	//S8
#define _LCD_3B		S03C1	//S8
#define _LCD_3C		S03C2	//S8
#define _LCD_3D		S03C3	//S8
#define _LCD_3E		S04C3	//S9
#define _LCD_3F		S04C1	//S9
#define _LCD_3G		S04C2	//S9

//najstarsza cyfra
#define _LCD_4A		S05C0	//S10
#define _LCD_4B		S05C1	//S10
#define _LCD_4C		S05C2	//S10
#define _LCD_4D		S05C3	//S10
#define _LCD_4E		S06C3	//S11
#define _LCD_4F		S06C1	//S11
#define _LCD_4G		S06C2	//S11



	
//kropka
#define _LCD_P1		S21C3	//S2

//regulacja pogodowa
#define _LCD_PGD	S20C2	//S1

//plomien CO
#define _LCD_PLC	S20C3	//S1

//plomien CW
#define _LCD_PLW	S07C3	//S12

//kreski
#define _LCD_KRS	S20C1	//S1

//kran
#define _LCD_KRN	S07C0	//S12

//kaloryfer
#define _LCD_KAL	S20C0	//S1

//max CO
#define _LCD_MXC	S25C0	//S6

//min C0
#define _LCD_MNC	S23C0	//S4

//max CW
#define _LCD_MXW	S06C0	//S11

//min CW
#define _LCD_MNW	S04C0	//S9

//bar
#define _LCD_BAR	S21C2	//S2

//temp CO
#define _LCD_TCO	S21C1	//S2

//temp CW
#define _LCD_TCW	S02C0	//S7

//RESET
#define _LCD_RES	S02C2	//S7

//zasilanie
#define _LCD_OFF	S02C1	//S7

//serwis, konfiguracja
#define _LCD_SRV	S07C2	//S12
*/

//cyfry wyswietlacza (druga wersja)

//najmlodsza cyfra
#define _LCD_1A		S23C0	//S3
#define _LCD_1B		S23C1	//S3
#define _LCD_1C		S23C2	//S3	
#define _LCD_1D		S23C3	//S3
#define _LCD_1E		S22C3	//S4
#define _LCD_1F		S22C1	//S4
#define _LCD_1G		S22C2	//S4

#define _LCD_2A		S21C0	//S5
#define _LCD_2B		S21C1	//S5
#define _LCD_2C		S21C2	//S5			
#define _LCD_2D		S21C3	//S5
#define _LCD_2E		S20C3	//S6
#define _LCD_2F		S20C1	//S6
#define _LCD_2G		S20C2	//S6

#define _LCD_3A		S06C0	//S8
#define _LCD_3B		S06C1	//S8
#define _LCD_3C		S06C2	//S8
#define _LCD_3D		S06C3	//S8
#define _LCD_3E		S05C3	//S9
#define _LCD_3F		S05C1	//S9
#define _LCD_3G		S05C2	//S9

//najstarsza cyfra
#define _LCD_4A		S04C0	//S10
#define _LCD_4B		S04C1	//S10
#define _LCD_4C		S04C2	//S10
#define _LCD_4D		S04C3	//S10
#define _LCD_4E		S03C3	//S11
#define _LCD_4F		S03C1	//S11
#define _LCD_4G		S03C2	//S11



	
//kropka
#define _LCD_P1		S24C3	//S2

//regulacja pogodowa
#define _LCD_PGD	S25C2	//S1

//plomien CO
#define _LCD_PLC	S25C3	//S1

//plomien CW
#define _LCD_PLW	S02C3	//S12

//kreski
#define _LCD_KRS	S25C1	//S1

//kran
#define _LCD_KRN	S02C0	//S12

//kaloryfer
#define _LCD_KAL	S25C0	//S1

//max CO
#define _LCD_MXC	S20C0	//S6

//min C0
#define _LCD_MNC	S22C0	//S4

//max CW
#define _LCD_MXW	S03C0	//S11

//min CW
#define _LCD_MNW	S05C0	//S9

//bar
#define _LCD_BAR	S24C2	//S2

//temp CO
#define _LCD_TCO	S24C1	//S2

//temp CW
#define _LCD_TCW	S07C0	//S7

//RESET
#define _LCD_RES	S07C2	//S7

//zasilanie
#define _LCD_OFF	S07C1	//S7

//serwis, konfiguracja
#define _LCD_SRV	S02C2	//S12
//----------------------------------------------------------
void FormatLcdData(void);
//----------------------------------------------------------
#endif /*_LCDHD2_H*/