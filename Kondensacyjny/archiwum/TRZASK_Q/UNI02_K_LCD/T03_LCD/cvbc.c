/*cvbc.c*/
#include <pic18.h>
#include "global.h"			//parametry globalne
#include "cvbc.h"			//parametry globalne
#include "uart9c.h"			//parametry lokalne

extern UARTData UADt;

tpCVBC CVBC;
unsigned char dCVBC[_NRS_BTY];


//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//Timer na potrzeby transmisji (umiescic w przerwaniu od zegara TMR1)
void IncRSTimer(void)
{
	if(CVBC.timRS<255) CVBC.timRS++;
}
//-------------------------------------------------------------------------------
//Restart zegara 
void StartRSTim(void)
{
	CVBC.timRS=0;
}
//-------------------------------------------------------------------------------
//Odczyt stanu zegara 
unsigned char RSTim(void)
{
	return CVBC.timRS;
}
/*---------------------------------------------------------------------------------*/
//-------------------------------------------------------------------------------
//Timer na potrzeby transmisji (umiescic w przerwaniu od zegara TMR1)
void IncRSTimer2(void)
{
	if(CVBC.timRS2<255) CVBC.timRS2++;
}
//-------------------------------------------------------------------------------
//Restart zegara 
void StartRSTim2(void)
{
	CVBC.timRS2=0;
}
//-------------------------------------------------------------------------------
//Odczyt stanu zegara 
unsigned char RSTim2(void)
{
	return CVBC.timRS2;
}
/*---------------------------------------------------------------------------------*/
//-----------------------------
//Czyszczenie tablicy danych (zerowanie ramki)
void CLRdCVBC(void)
{
	unsigned char i;
	for(i=0;i<_NRS_BTY;i++) dCVBC[i]=0;
}
//uzupelnienie tablicy danych z bufora RS
void CopyRSto_dCVBC(void)
{
	unsigned char i;
	for(i=0;i<_NRS_BTY;i++) dCVBC[i]=GetBufRUART(i);
}


//-----------------------------
//Aktualizacja flag statusowych modulu CVBC (PO POPRAWNEJ TRANSMISJI DANYCH)
void UpdateStatCVBC(void)
{
	CVBC.H_FAULT_Indication=(CVBC.HSTAT&_H_FAULT_IND)?1:0;
	CVBC.H_VALVE1_Status=(CVBC.HSTAT&_H_VALVE1_STAT)?1:0;
	CVBC.H_VALVE2_Status=(CVBC.HSTAT&_H_VALVE2_STAT)?1:0;
	CVBC.H_APS_Status=(CVBC.HSTAT&_H_APS_STAT)?1:0;
	CVBC.H_FAN_Status=(CVBC.HSTAT&_H_FAN_STAT)?1:0;
	CVBC.H_CH_PUMP_Status=(CVBC.HSTAT&_H_CH_PUMP_STAT)?1:0;
	CVBC.H_DHW_PUMP_Status=(CVBC.HSTAT&_H_DHW_PUMP_STAT)?1:0;
	CVBC.H_FROST_Mode=(CVBC.HSTAT&_H_FROST_MODE)?1:0;
	CVBC.H_CH_Mode=(CVBC.HSTAT&_H_CH_MODE)?1:0;
	CVBC.H_DHW_Mode=(CVBC.HSTAT&_H_DHW_MODE)?1:0;
	CVBC.H_TEST_Mode=(CVBC.HSTAT&_H_TEST_MODE)?1:0;
	CVBC.H_FLAME_Status=(CVBC.HSTAT&_H_FLAME_STAT)?1:0;
	CVBC.H_EXH_Status=(CVBC.HSTAT&_H_EXH_STAT)?1:0;
	CVBC.H_WATER_FL_Status=(CVBC.HSTAT&_H_WATER_FL_STAT)?1:0;
	CVBC.H_CALIBRATION_Start=(CVBC.HSTAT&_H_CALIBRATION_START)?1:0;
	CVBC.H_CALIBRATION_Min=(CVBC.HSTAT&_H_CALIBRATION_MIN)?1:0;
}
//-----------------------------
//Decyzja o odczycie/zapisie parametru do modu³u CVBC
//Kod Message jao MSS, DATA-ID jako DID HEX(16bit), DATA jako DTA HEX(16bit)
void StartRWtoCVBS(unsigned char MSS,unsigned int DID,unsigned int DTA)
{
	CLRdCVBC();
	dCVBC[_STR_RS]=_W_STR_RS;
	dCVBC[_MSG_TP]=MSS;
	dCVBC[_ADR_SL]=_W_ADR_SL;
	HEXtoFOUR_ASCII(DID,dCVBC+_DID_03,dCVBC+_DID_02,dCVBC+_DID_01,dCVBC+_DID_00);
	HEXtoFOUR_ASCII(DTA,dCVBC+_DAT_03,dCVBC+_DAT_02,dCVBC+_DAT_01,dCVBC+_DAT_00);								//konwersja danej DTA na 4 bajty ASCII
	HEXtoFOUR_ASCII(LiczCRC(dCVBC+_MSG_TP,_NCH_BTY),dCVBC+_CRC_03,dCVBC+_CRC_02,dCVBC+_CRC_01,dCVBC+_CRC_00);	//oblicz CRC
	dCVBC[_STP_RS]=_W_STP_RS;
	CVBC.HNL=0;

	CVBC.HErr_COM=0;
	CVBC.HErr_CHK=0;
	CVBC.HErr_ERR=0;
	CVBC.HErr_MSG=0;
	CVBC.HErr_DID=0;

	CVBC.HRead_OK=0;
	CVBC.HWrite_OK=0;

	CVBC.TRANS_DATA=1;	//wyzwolenie procedury transmisji danych
}
//-----------------------------
//Algorytm transmisji danych z potwierdzeniem (modu³ CVBC)
void Trans_Data_CVBC(void)
{
	switch(CVBC.HNL)
	{
		case 0:
		{
			StartUSARTTR(_NRS_BTY);			//rozpocznij transmisje
			if(!UADt.TData)					//udalo sie rozpocznac transmisje?
			{
				StopUSARTTR();				//zatrzymaj transmije
				UADt.TErrTRN=1;				//ustaw znacznik bledu transmisji
				StartRSTim();				
				CVBC.HNL=3;					//zwloka 200m sek
				return;
			}
			StartRSTim();
			CVBC.HNL=1;
		}
		break;
		case 1:
		{
			if(UADt.TData)					//trwa transmisja?
			{
				if(RSTim()>=_TS_TOUT)		//przekroczono dozwolony limit czasu na transmisje?
				{
					StopUSARTTR();
					UADt.TErrTim=1;			//ustaw znacznik przekroczenia dozwolonego czasu na transmisje
					CVBC.HNL=4;				//analiza danych
				}
				return;
			}
			else							//przygotuj sie do odbioru
			{
				CLRdCVBC();					//wyczysc bufor danych przed odbiorem
				StartUSARTRC(_NRS_BTY);		//rozpocznij odbior
				if(!UADt.RData)				//udalo sie rozpocznac odbior?
				{
					
					StopUSARTRC();
					UADt.RErrREC=1;				//ustaw znacznik bledu transmisji
					StartRSTim();				
					CVBC.HNL=3;					//zwloka 200m sek
					return;
				}
				StartRSTim();
				CVBC.HNL=2;
			}
		}
		break;				
		case 2:
		{
			if(UADt.RData)					//trwa odbior?
			{
				if(RSTim()>=_RS_TOUT)		//przekroczono dozwolony limit czasu na odbior?
				{
					StopUSARTRC();
					UADt.RErrTim=1;			//ustaw znacznik przekroczenia dozwolonego czasu na odbior
					CVBC.HNL=4;				//analiza danych
				}
				return;
			}
			else							//przygotuj sie do odbioru
			{
				StartRSTim();
				CVBC.HNL=4;
			}
		}
		break;
		case 3:
		{
			if(RSTim()<_UA_PAUSE) return;	//zwloka 200m sek
		}
		break;
		case 4:
		{
			if(UADt.TErrTRN||UADt.TErrTim||UADt.RErrREC||UADt.RErrTim) //blad transmisji?
			{
				CVBC.HErr_COM=1;
				CVBC.HNL=0;
				CVBC.TRANS_DATA=0;
				return;
			}
			CopyRSto_dCVBC();	//zapisanie danych w tablicy dCVBC
			if(!CheckCHK(dCVBC[_CRC_03],dCVBC[_CRC_02],dCVBC[_CRC_01],dCVBC[_CRC_00],LiczCRC(dCVBC+_MSG_TP,_NCH_BTY))) //blad CHK?
			{
				CVBC.HErr_CHK=1;
				CVBC.HNL=0;
				CVBC.TRANS_DATA=0;
				return;				
			}
			//analiza danych
			CVBC.HMSG=dCVBC[_MSG_TP];
			switch(CVBC.HMSG&0xf0)
			{
				case _MSG_RA:	//odczyt danych z potwierdzeniem
				{
					CVBC.HDID=FOUR_ASCIItoHEX(dCVBC[_DID_03],dCVBC[_DID_02],dCVBC[_DID_01],dCVBC[_DID_00]);
					CVBC.HDTA=FOUR_ASCIItoHEX(dCVBC[_DAT_03],dCVBC[_DAT_02],dCVBC[_DAT_01],dCVBC[_DAT_00]);
					CVBC.HRead_OK=1;
					CVBC.HNL=0;
					CVBC.TRANS_DATA=0;
					return;	
				}
				case _MSG_WA:	//zapis danych z potwierdzeniem
				{
					CVBC.HWrite_OK=1;
					CVBC.HNL=0;
					CVBC.TRANS_DATA=0;
					return;			
				}
				case _MSG_CM:	//nieprawidlowa wartosc message
				{
					CVBC.HErr_MSG=1;
					CVBC.HNL=0;
					CVBC.TRANS_DATA=0;
					return;					
				}
				case _MSG_UD:	//nieprawidlowe DATA-ID
				{
					CVBC.HErr_DID=1;
					CVBC.HNL=0;
					CVBC.TRANS_DATA=0;
					return;								
				}
				default:		//niezrozumialy komunikat
				{
					CVBC.HErr_ERR=1;
					CVBC.HNL=0;
					CVBC.TRANS_DATA=0;
					return;	
				}
			}
		}
	}
}
//-----------------------------
//Odczyt danej z modulu CVBC (master to slave)
//gdy funkcja zwraca 0 parametr data zostal zaktualizowany o wartosc z modulu CVBC
unsigned char ReadDataFromCVBS(unsigned char MSG, unsigned int ID, unsigned int *data)
{
	switch(CVBC.SEP)
	{
		case 0:
		{
			CVBC.licznik=0;
			StartRWtoCVBS(MSG&0x0f,ID,0);			//wyzwolenie transmisji danych
			CVBC.SEP=1;
		}
		case 1:
		{
			if(CVBC.TRANS_DATA) return 1;			//oczeiwanie na zaknczenie transmisji
			if(CVBC.HRead_OK&&(CVBC.HDID==ID))		//zapisano dana z powodzeniem?
			{
				*data=CVBC.HDTA;					//zapamietaj dana
				CVBC.SEP=0;
				CVBC.NO_CVBC=0;
				return 0;							//deaktywuj wywolanie procedury
			}
			CVBC.licznik++;							
			if(CVBC.licznik>=_NCYKLI_WR)			//dokonano maksymalna liczbe prob zapisu?
			{
				CVBC.SEP=0;
				CVBC.NO_CVBC=1;
				ZtoNext(); 							//przekaz zeton dalej
				return 1;							//zachowaj wywolanie procedury
			}
			StartRWtoCVBS(MSG&0x0f,ID,0);			//ponow probe odczytu
			return 1;								//zachowaj wywolanie procedury		
		}
	}
	return 1;
}
//-----------------------------
//Zapis danej do modulu CVBC (master to slave)
//gdy funkcja zwraca 0 otrzymujemy potwierdzenie prawidlowego zapisu danej do CVBC
unsigned char WriteDataToCVBS(unsigned char MSG, unsigned int ID, unsigned int DT)
{
	switch(CVBC.SEP)
	{
		case 0:
		{
			CVBC.licznik=0;
			StartRWtoCVBS(MSG|0x80,ID,DT);				//wyzwolenie transmisji danych
			CVBC.SEP=1;
		}
		case 1:
		{
			if(CVBC.TRANS_DATA) return 1;			//oczeiwanie na zaonczenie transmisji
			if(CVBC.HWrite_OK)						//zapisano dana z powodzeniem?
			{
				CVBC.SEP=0;
				CVBC.NO_CVBC=0;
				return 0;							//deaktywuj wywolanie procedury
			}
			CVBC.licznik++;							
			if(CVBC.licznik>=_NCYKLI_WR)			//dokonano maksymalna liczbe prob zapisu?
			{
				CVBC.SEP=0;
				CVBC.NO_CVBC=1;
				ZtoNext(); 							//przekaz zeton dalej
				return 1;							//zachowaj wywolanie procedury
			}
			StartRWtoCVBS(MSG|0x80,ID,DT);  				//ponow probe zapisu
			return 1;								//zachowaj wywolanie procedury		
		}
	}
	return 1;
}
//-----------------------------
//Zapis wartosci nastawy PCW do modulu CVBC
void WritePCWtoCVBS(unsigned char PCW)
{
	switch(CVBC.SEP)
	{
		case 0:
		{
			CVBC.licznik=0;
			StartRWtoCVBS(_WR_CLASS5,_PCW_ID,PCW);
			CVBC.SEP=1;
		}
		case 1:
		{
			if(CVBC.TRANS_DATA) return;			//oczeiwanie na zaonczenie transmisji
			if(CVBC.HWrite_OK)					//zapisano dana z powodzeniem?
			{
				CVBC.active=CVBC.active&(~_SET_PCW); //deaktywuj wywolanie procedury
				CVBC.SEP=0;
				CVBC.NO_CVBC=0;
				return;
			}
			CVBC.licznik++;							
			if(CVBC.licznik>=_NCYKLI_WR)			//dokonano maksymalna liczbe prob zapisu?
			{
				CVBC.SEP=0;
				CVBC.NO_CVBC=1;
				ZtoNext(); 							//przekaz zeton dalej zachowujac wywolanie procedury
				return;
			}
			StartRWtoCVBS(_WR_CLASS5,_PCW_ID,PCW);  	//ponow probe zapisu
			return;			
		}
	}
}
//-----------------------------
//Wywolanie procedury RESET w module CVBC
void WriteRESETtoCVBS(void)
{
	switch(CVBC.SEP)
	{
		case 0:
		{
			CVBC.licznik=0;
			StartRWtoCVBS(_WR_CLASS3,_RESET_ID,_RESET_DT);
			CVBC.SEP=1;
		}
		case 1:
		{
			if(CVBC.TRANS_DATA) return;			//oczeiwanie na zaonczenie transmisji
			if(CVBC.HWrite_OK)					//zapisano dana z powodzeniem?
			{
				CVBC.active=CVBC.active&(~_SET_RES); //deaktywuj wywolanie procedury
				CVBC.SEP=0;
				CVBC.NO_CVBC=0;
				return;
			}
			CVBC.licznik++;							
			if(CVBC.licznik>=_NCYKLI_WR)			//dokonano maksymalna liczbe prob zapisu?
			{
				CVBC.SEP=0;
				CVBC.NO_CVBC=1;
				ZtoNext(); 							//przekaz zeton dalej zachowujac wywolanie procedury
				return;
			}
			StartRWtoCVBS(_WR_CLASS3,_RESET_ID,_RESET_DT);  	//ponow probe zapisu
			return;			
		}
	}
}
//-----------------------------
//Odczytaj wartosci nastawy PCW z modulu CVBC
void ReadPCWfromCVBS(void)
{
	switch(CVBC.SEP)
	{
		case 0:
		{
			CVBC.licznik=0;
			StartRWtoCVBS(_RD_CLASS5,_PCW_ID,0);
			CVBC.SEP=1;
		}
		case 1:
		{
			if(CVBC.TRANS_DATA) return;			//oczeiwanie na zaonczenie transmisji
			
			if(CVBC.HRead_OK&&(CVBC.HDID==_PCW_ID))	//odczytano dana z powodzeniem?
			{
				CVBC.active=CVBC.active&(~_GET_PCW); //deaktywuj wywolanie procedury
				CVBC.HPCW=CVBC.HDTA;			//zapamietaj wartosc PCW
				CVBC.SEP=0;
				CVBC.NO_CVBC=0;
				return;							//odczyt z powodzeniem
			}
			CVBC.licznik++;							
			if(CVBC.licznik>=_NCYKLI_WR)			//dokonano maksymalna liczbe prob zapisu?
			{
				CVBC.SEP=0;
				CVBC.NO_CVBC=1;
				ZtoNext(); 							//przekaz zeton dalej zachowujac wywolanie procedury
				return;
			}
			StartRWtoCVBS(_RD_CLASS5,_PCW_ID,0);  	//ponow probe odczytu
			return;			
		}
	}
}
//-----------------------------
//Odczytaj wartosci STATUS z modulu CVBC
void ReadSTATfromCVBS(void)
{
	switch(CVBC.SEP)
	{
		case 0:
		{
			CVBC.licznik=0;
			StartRWtoCVBS(_RD_CLASS1,_STAT_ID,0);
			CVBC.SEP=1;
		}
		case 1:
		{
			if(CVBC.TRANS_DATA) return;			//oczeiwanie na zaonczenie transmisji
			
			if(CVBC.HRead_OK&&(CVBC.HDID==_STAT_ID))	//odczytano dana z powodzeniem?
			{
				CVBC.active=CVBC.active&(~_GET_STAT); //deaktywuj wywolanie procedury
				CVBC.HSTAT=CVBC.HDTA;			//zapamietaj wartosc STATUS
				UpdateStatCVBC();				//aktualizuj stan flag statusowych mod. CVBC
				CVBC.SEP=0;
				CVBC.NO_CVBC=0;
				return;							//odczyt z powodzeniem
			}
			CVBC.licznik++;							
			if(CVBC.licznik>=_NCYKLI_WR)			//dokonano maksymalna liczbe prob zapisu?
			{
				CVBC.SEP=0;
				CVBC.NO_CVBC=1;
				ZtoNext(); 							//przekaz zeton dalej zachowujac wywolanie procedury
				return;
			}
			StartRWtoCVBS(_RD_CLASS1,_STAT_ID,0);  	//ponow probe odczytu
			return;			
		}
	}
}
//-----------------------------
//Odczytaj wartosci ERROR z modulu CVBC
void ReadERRfromCVBS(void)
{
	switch(CVBC.SEP)
	{
		case 0:
		{
			CVBC.licznik=0;
			StartRWtoCVBS(_RD_CLASS1,_ERR_ID,0);
			CVBC.SEP=1;
		}
		case 1:
		{
			if(CVBC.TRANS_DATA) return;			//oczeiwanie na zaonczenie transmisji
			
			if(CVBC.HRead_OK&&(CVBC.HDID==_ERR_ID))	//odczytano dana z powodzeniem?
			{
				CVBC.active=CVBC.active&(~_GET_ERR); //deaktywuj wywolanie procedury
				CVBC.HERR=CVBC.HDTA;			//zapamietaj wartosc STATUS
				CVBC.SEP=0;
				CVBC.NO_CVBC=0;
				return;							//odczyt z powodzeniem
			}
			CVBC.licznik++;							
			if(CVBC.licznik>=_NCYKLI_WR)			//dokonano maksymalna liczbe prob zapisu?
			{
				CVBC.SEP=0;
				CVBC.NO_CVBC=1;
				ZtoNext(); 							//przekaz zeton dalej zachowujac wywolanie procedury
				return;
			}
			StartRWtoCVBS(_RD_CLASS1,_ERR_ID,0);  	//ponow probe odczytu
			return;			
		}
	}
}
//-----------------------------
//Odczytaj wartosci TCW z modulu CVBC
void ReadTCWfromCVBS(void)
{
	switch(CVBC.SEP)
	{
		case 0:
		{
			CVBC.licznik=0;
			StartRWtoCVBS(_RD_CLASS4,_TCW_ID,0);
			CVBC.SEP=1;
		}
		case 1:
		{
			if(CVBC.TRANS_DATA) return;			//oczeiwanie na zaonczenie transmisji
			
			if(CVBC.HRead_OK&&(CVBC.HDID==_TCW_ID))	//odczytano dana z powodzeniem?
			{
				CVBC.active=CVBC.active&(~_GET_TCW); //deaktywuj wywolanie procedury
				CVBC.HTCW=CVBC.HDTA;			//zapamietaj wartosc STATUS
				CVBC.SEP=0;
				CVBC.NO_CVBC=0;
				return;							//odczyt z powodzeniem
			}
			CVBC.licznik++;							
			if(CVBC.licznik>=_NCYKLI_WR)			//dokonano maksymalna liczbe prob zapisu?
			{
				CVBC.SEP=0;
				CVBC.NO_CVBC=1;
				ZtoNext(); 							//przekaz zeton dalej zachowujac wywolanie procedury
				return;
			}
			StartRWtoCVBS(_RD_CLASS4,_TCW_ID,0);  	//ponow probe odczytu
			return;			
		}
	}
}
/*---------------------------------------------------------------------------------*/
//-------------------------------------------------------------------------------
//Przesuniecie zetonu
//zeton jest przekazywany przez aktywowana procedure bez jej deaktywacji w przypadku niepowodzenia
//lub po deaktywacji procedury
void ZtoNext(void)
{
	CVBC.zeton=CVBC.zeton<<1;
	if(!CVBC.zeton) CVBC.zeton=0x0001;
}
//-----------------------------
//Ustawienie zetonu w pozycji START
void ZtoStart(void)
{
	CVBC.zeton=0x0001;
}
//-----------------------------
//Realizacja drogi zetonu
void Ring(void)
{
	switch(CVBC.zeton)
	{
		case _SET_PCW:								//zeton przyznany
		{
			if(CVBC.active&_SET_PCW) return;		//procedura aktywowana?
			ZtoNext();
			return;
		}
		case _GET_PCW:								//zeton przyznany
		{
			if(CVBC.active&_GET_PCW) return;		//procedura aktywowana?
			ZtoNext();
			return;
		}
		case _SET_RES:								//zeton przyznany
		{
			if(CVBC.active&_SET_RES) return;		//procedura aktywowana?
			ZtoNext();
			return;
		}
		case _GET_ERR:								//zeton przyznany
		{
			if(CVBC.active&_GET_ERR) return;		//procedura aktywowana?
			ZtoNext();
			return;
		}
		case _GET_STAT:								//zeton przyznany
		{
			if(CVBC.active&_GET_STAT) return;		//procedura aktywowana?
			ZtoNext();
			return;
		}
		case _GET_TCW:								//zeton przyznany
		{
			if(CVBC.active&_GET_TCW) return;		//procedura aktywowana?
			ZtoNext();
			return;
		}
		default:
			ZtoStart();								//restart zetonu
	}
}
//-----------------------------
//Wywolanie kolejnych transmisji wg przyznanego zetonu
//PROCEDURE UMIESCIC W PETLI GLOWNEJ PROGRAMU
void SendMessageToCVBC(void)
{
	if(CVBC.TRANS_DATA) Trans_Data_CVBC();			//realizuj transmisje do modulu CVBC

	if(CVBC.zeton==_SET_PCW)						//zeton przyznany?		
	{
		if(CVBC.active&_SET_PCW)					//procedura aktywowana?
		{
			WritePCWtoCVBS(0);
		}
	}
	if(CVBC.zeton==_GET_PCW)						//zeton przyznany?		
	{
		if(CVBC.active&_GET_PCW)					//procedura aktywowana?
		{
			ReadPCWfromCVBS();
		}
	}
	if(CVBC.zeton==_SET_RES)						//zeton przyznany?		
	{
		if(CVBC.active&_SET_RES)					//procedura aktywowana?
		{
			WriteRESETtoCVBS();			
		}
	}
	if(CVBC.zeton==_GET_ERR)						//zeton przyznany?		
	{
		if(CVBC.active&_GET_ERR)					//procedura aktywowana?
		{
			ReadERRfromCVBS();
		}
	}
	if(CVBC.zeton==_GET_STAT)						//zeton przyznany?		
	{
		if(CVBC.active&_GET_STAT)					//procedura aktywowana?
		{
			ReadSTATfromCVBS();
		}
	}
	if(CVBC.zeton==_GET_TCW)						//zeton przyznany?		
	{
		if(CVBC.active&_GET_TCW)					//procedura aktywowana?
		{
			ReadTCWfromCVBS();
		}
	}
	Ring();											//steruj ruchem zetonu
}
//-----------------------------
//Wywolanie kolejnych transmisji wg przyznanego zetonu
//PROCEDURE UMIESCIC W PETLI GLOWNEJ PROGRAMU
void SendMessageToCVBC2(void)
{
	unsigned int data;

	if(CVBC.TRANS_DATA) Trans_Data_CVBC();			//realizuj transmisje do modulu CVBC

	if(CVBC.zeton==_SET_PCW)						//zeton przyznany?		
	{
		if(CVBC.active&_SET_PCW)					//procedura aktywowana?
		{
			if(!WriteDataToCVBS(_WR_CLASS5,_PCW_ID,data)) //transmisja zakonczona powodzeniem?
			{
				CVBC.active=CVBC.active&(~_SET_PCW); //deaktywuj wywolanie procedury
			}
		}
	}
	if(CVBC.zeton==_GET_PCW)						//zeton przyznany?		
	{
		if(CVBC.active&_GET_PCW)					//procedura aktywowana?
		{
			if(!ReadDataFromCVBS(_RD_CLASS5,_PCW_ID,&data))	//transmisja zakonczona powodzeniem?
			{
				CVBC.active=CVBC.active&(~_GET_PCW); //deaktywuj wywolanie procedury
				CVBC.HPCW=data;				
			}
		}
	}
	if(CVBC.zeton==_SET_RES)						//zeton przyznany?		
	{
		if(CVBC.active&_SET_RES)					//procedura aktywowana?
		{
			if(!WriteDataToCVBS(_WR_CLASS3,_RESET_ID,_RESET_DT)) //transmisja zakonczona powodzeniem?
			{
				CVBC.active=CVBC.active&(~_SET_RES); //deaktywuj wywolanie procedury
			}
		}
	}
	if(CVBC.zeton==_GET_ERR)						//zeton przyznany?		
	{
		if(CVBC.active&_GET_ERR)					//procedura aktywowana?
		{
			if(!ReadDataFromCVBS(_RD_CLASS1,_ERR_ID,&data))	//transmisja zakonczona powodzeniem?
			{
				CVBC.active=CVBC.active&(~_GET_ERR); //deaktywuj wywolanie procedury
				CVBC.HERR=data;
			}
		}
	}
	if(CVBC.zeton==_GET_STAT)						//zeton przyznany?		
	{
		if(CVBC.active&_GET_STAT)					//procedura aktywowana?
		{
			if(!ReadDataFromCVBS(_RD_CLASS1,_STAT_ID,&data))	//transmisja zakonczona powodzeniem?
			{
				CVBC.active=CVBC.active&(~_GET_STAT); //deaktywuj wywolanie procedury
				CVBC.HSTAT=data;
				UpdateStatCVBC();					//zaktualizuj flagi stanu
			}
		}
	}
	if(CVBC.zeton==_GET_TCW)						//zeton przyznany?		
	{
		if(CVBC.active&_GET_TCW)					//procedura aktywowana?
		{
			if(!ReadDataFromCVBS(_RD_CLASS4,_TCW_ID,&data))	//transmisja zakonczona powodzeniem?
			{
				CVBC.active=CVBC.active&(~_GET_TCW); //deaktywuj wywolanie procedury
				CVBC.HTCW=data;
			}
		}
	}
	Ring();											//steruj ruchem zetonu
}
//-----------------------------
