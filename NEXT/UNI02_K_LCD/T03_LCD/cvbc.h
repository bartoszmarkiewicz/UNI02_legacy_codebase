/*cvbc.h*/
#include "cvbc.tp.h"

#ifndef	_CVBC_H
#define	_CVBC_H
//----------------------------------------------------------
#define _RS_TOUT	120		//timeout dla odbioru [1/100sek]
#define _TS_TOUT	120		//timeout dla nadawania [1/100sek]
#define _UA_PAUSE	20		//minimalna przerwa miedzy transmisjami [1/100sek]

#define _NCYKLI_WR	5		//maksymalna liczba prob zapisu danej do CVBC
//#define	_NRS_BTY 16	//liczba przesy³anych danych (rozmiar ramki)
//#define	_NCH_BTY 10	//liczba danych z których liczone jest CHK
//----------------------------------------------------------
#define	_W_STR_RS 0x0D	//wartosc slowa startowego
#define	_W_ADR_SL 0x30	//wartosc adresu slave
#define	_W_STP_RS 0x0A	//wartosc slowa koncowego
//----------------------------------------------------------
//Message types SLAVE to MASTER
#define	_MSG_RA 0x10	//read acknowledge
#define	_MSG_WA 0x90	//write acknowledge
#define	_MSG_CM 0x20	//corrupted message
#define	_MSG_UD 0x60	//uncown DATA-ID

//Message types MASTER to SLAVE
#define _WR_CLASS5 	0x85	//ZAPIS,CLASS5
#define _WR_CLASS3 	0x83	//ZAPIS,CLASS3
#define _RD_CLASS5 	0x05	//ODCZYT,CLASS5
#define _RD_CLASS4 	0x04	//ODCZYT,CLASS4
#define _RD_CLASS1 	0x01	//ODCZYT,CLASS1
//---
#define _PCW_ID		0x05f4	//parametr PCW (DHW Setpoint)
#define _STAT_ID	0x0001	//parametr STATUS
#define _ERR_ID		0x0032	//parametr ERROR (Signed Actual Error)
#define _TCW_ID		0x0302	//parametr Sensor1
#define _RESET_ID	0x00AB	//procedura RESET
//---
#define _RESET_DT	0xAA55	//wartosc wyzwalajaca RESET
//----------------------------------------------------------
#define	_STR_RS 0	//indeks slowa STARTOWEGO
#define	_MSG_TP 1	//indes slowa MESSAGE
#define _ADR_SL 2	//indeks dresu slave
#define _DID_03 3	//indeks DATA-ID
#define _DID_02 4	//indeks DATA-ID
#define _DID_01 5	//indeks DATA-ID
#define _DID_00 6	//indeks DATA-ID
#define _DAT_03 7	//indeks DATA
#define _DAT_02 8	//indeks DATA
#define _DAT_01 9	//indeks DATA
#define _DAT_00 10	//indeks DATA
#define _CRC_03 11	//indeks CRC
#define _CRC_02 12	//indeks CRC
#define _CRC_01 13	//indeks CRC
#define _CRC_00 14	//indeks CRC
#define	_STP_RS 15	//indeks slowa KONCOWEGO
//----------------------------------------------------------
//ZETON
#define _SET_PCW 	0b0000000000000001
#define _GET_PCW 	0b0000000000000010
#define _SET_RES 	0b0000000000000100
#define _GET_ERR 	0b0000000000001000
#define _GET_STAT 	0b0000000000010000
#define _GET_TCW 	0b0000000000100000
//----------------------------------------------------------
//STATUS
//low byte
#define _H_FAULT_IND 			0b0000000000000001
#define _H_VALVE1_STAT			0b0000000000000010
#define _H_VALVE2_STAT			0b0000000000000100
#define _H_APS_STAT				0b0000000000001000
#define _H_FAN_STAT				0b0000000000010000
#define _H_CH_PUMP_STAT			0b0000000000100000
#define _H_DHW_PUMP_STAT		0b0000000001000000
#define _H_FROST_MODE			0b0000000010000000
//high byte
#define _H_CH_MODE 				0b0000000100000000
#define _H_DHW_MODE				0b0000001000000000
#define _H_TEST_MODE			0b0000010000000000
#define _H_FLAME_STAT			0b0000100000000000
#define _H_EXH_STAT				0b0001000000000000
#define _H_WATER_FL_STAT		0b0010000000000000
#define _H_CALIBRATION_START	0b0100000000000000
#define _H_CALIBRATION_MIN		0b1000000000000000




void ZtoNext(void);

unsigned int FOUR_ASCIItoHEX(unsigned char b3, unsigned char b2, unsigned char b1, unsigned char b0);
void HEXtoFOUR_ASCII(unsigned int h,unsigned char *a3, unsigned char *a2,unsigned char *a1, unsigned char *a0);
unsigned int LiczCRC(unsigned char *tb, unsigned char n);
void StartUSARTRC(const unsigned char nData);
void StopUSARTRC(void);
void StartUSARTTR(const unsigned char nData);
void StopUSARTTR(void);
unsigned char GetBufRUART(const unsigned char indeks);
void SetBufTUART(const unsigned char indeks,const unsigned char data);

unsigned int LiczCRC(unsigned char *tb, unsigned char n);
unsigned char CheckCHK(unsigned char a3, unsigned char a2, unsigned char a1, unsigned char a0, unsigned int h);
#endif		/*_CVBC_H*/