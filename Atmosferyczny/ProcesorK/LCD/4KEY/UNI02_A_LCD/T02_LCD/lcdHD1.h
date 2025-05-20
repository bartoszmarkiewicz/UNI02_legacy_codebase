/*lcdHD1.h*/
/*Wyswietlacz typ WDNP265-ATL-#00*/
#ifndef	_LCDHD1_H
#define	_LCDHD1_H

//Mapa polaczen LCD - PIC 

//cyfry wyswietlacza 
//najmlodsza cyfra
#define _LCD_1A		S14C0	//S2->SEG14
#define _LCD_1B		S14C1	//S2->SEG14
#define _LCD_1C		S14C2	//S2->SEG14		
#define _LCD_1D		S14C3	//S2->SEG14
#define _LCD_1E		S12C2	//S3->SEG12
#define _LCD_1F		S12C0	//S3->SEG12
#define _LCD_1G		S12C1	//S3->SEG12

#define _LCD_2A		S29C0	//S4->SEG29	
#define _LCD_2B		S29C1	//S4->SEG29
#define _LCD_2C		S29C2	//S4->SEG29			
#define _LCD_2D		S29C3	//S4->SEG29
#define _LCD_2E		S11C2	//S5->SEG11
#define _LCD_2F		S11C0	//S5->SEG11
#define _LCD_2G		S11C1	//S5->SEG11

#define _LCD_3A		S10C0	//S6->SEG10
#define _LCD_3B		S10C1	//S6->SEG10
#define _LCD_3C		S10C2	//S6->SEG10
#define _LCD_3D		S10C3	//S6->SEG10
#define _LCD_3E		S09C2	//S7->SEG9
#define _LCD_3F		S09C0	//S7->SEG9
#define _LCD_3G		S09C1	//S7->SEG9 
//najstarsza cyfra
#define _LCD_4A		S08C0	//S8->SEG8
#define _LCD_4B		S08C1	//S8->SEG8
#define _LCD_4C		S08C2	//S8->SEG8
#define _LCD_4D		S08C3	//S8->SEG8
#define _LCD_4E		S30C2	//S9->SEG30
#define _LCD_4F		S30C0	//S9->SEG30
#define _LCD_4G		S30C1	//S9->SEG30

//linia skali 24
#define _LCD_SKL	S21C0

//bar kreski 
#define _LCD_T1		S02C0	
#define _LCD_T2		S02C1	
#define _LCD_T3		S02C2	
#define _LCD_T4		S02C3	
#define _LCD_T5		S03C3	
#define _LCD_T6		S03C2	
#define _LCD_T7		S03C1	
#define _LCD_T8		S03C0	

#define _LCD_T9		S04C0	
#define _LCD_T10	S04C1	
#define _LCD_T11	S04C2	
#define _LCD_T12	S04C3	
#define _LCD_T13	S05C3	
#define _LCD_T14	S05C2	
#define _LCD_T15	S05C1	
#define _LCD_T16	S05C0

#define _LCD_T17	S06C0	
#define _LCD_T18	S06C1	
#define _LCD_T19	S06C2	
#define _LCD_T20	S06C3	
#define _LCD_T21	S23C3	
#define _LCD_T22	S23C2	
#define _LCD_T23	S23C1	
#define _LCD_T24	S23C0

#define _LCD_T25	S24C0	
#define _LCD_T26	S24C1	
#define _LCD_T27	S24C2	
#define _LCD_T28	S24C3	
#define _LCD_T29	S25C3	
#define _LCD_T30	S25C2	
#define _LCD_T31	S25C1	
#define _LCD_T32	S25C0

#define _LCD_T33	S26C0	
#define _LCD_T34	S26C1	
#define _LCD_T35	S26C2	
#define _LCD_T36	S26C3	
#define _LCD_T37	S00C3	
#define _LCD_T38	S00C2	
#define _LCD_T39	S00C1	
#define _LCD_T40	S00C0

#define _LCD_T41	S01C0	
#define _LCD_T42	S02C1	
#define _LCD_T43	S03C2	
#define _LCD_T44	S04C3

//kwadraty z cyframi (dni)
#define _LCD_D1		S21C3	
#define _LCD_D2		S21C2	
#define _LCD_D3		S21C1	
#define _LCD_D4		S22C0	
#define _LCD_D5		S22C1	
#define _LCD_D6		S22C2	
#define _LCD_D7		S22C3
	
//dwie kropki
#define _LCD_COL	S11C3

//kropka
#define _LCD_P1		S12C3

//domek
#define _LCD_DOM	S20C3

//termometr
#define _LCD_TERM	S20C1

//stopnie C
#define _LCD_C		S20C2

//kran
#define _LCD_KRN	S20C0

//kaloryfer
#define _LCD_KAL	S19C0

//strzalka z kwadratem
#define _LCD_ARR	S19C1

//dlon
#define _LCD_HND	S19C2

//sniezka
#define _LCD_SN		S19C3

//ksiezyc
#define _LCD_MN		S15C3

//slonce
#define _LCD_SUN	S15C2

//zegar
#define _LCD_CLK	S15C1

//kran w czarnym kole
#define _LCD_BKR	S15C0

//symbol trzech fal
#define _LCD_3FL	S07C0

//pozycje wentylatora
#define _LCD_VNT1	S30C3
#define _LCD_VNT2	S07C3
#define _LCD_VNT3	S09C3

//male slonce
#define _LCD_sSUN	S07C1

//maly ksiezyc
#define _LCD_sMN	S07C2

//----------------------------------------------------------
void FormatLcdData(void);
//----------------------------------------------------------
#endif /*_LCDHD1_H*/