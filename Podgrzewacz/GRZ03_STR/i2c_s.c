/*i2c_s.c*/
/*I2C Slave Mode*/
//-------------------------------------------------------------------------------
#include <pic18.h>
#include "global.h"			//parametry globalne
#include "i2c_s.h"			//parametry lokalne
#include "dtimy.h"			
#include "main.tp.h"
#include "pomiar.h"	
#include "pwm.h"
#include "konfig.h"
#include "rgl.h"
#include "vntregtch.tp.h"

static DataI2C	DtI2C;
static bfI2C	BufI2C;

extern KALVnt Kvnt[10];
extern tpMAIN M;
extern DataCO DtCO;
extern DataCW DtCW;
extern DataZW DtZW;
extern DataPFN PFN;
extern DataCSN DtCS;
extern DtPWM PWM;
extern DtKONF DtKNF;
extern DtRgl rgl;

extern unsigned char bfrs;
extern unsigned char x,y,t,ty;
extern unsigned char rosnie,stoi;
//-------------------------------------------------------------------------------
//Inicjalizacja modulu I2C
void InitI2C(void)
{
	//TRISC
	TRISC3=1;				//SDA i SCA konfiguracja jako wejscia
	TRISC4=1;

	//SSPADD
	SSPADD=_I2CSPADD;		
	
	//SSPCON1
	SSPM0=0;
	SSPM1=1;
	SSPM2=1;
	SSPM3=0;				//I2C Slave Mode, 7 bit address
	SSPEN=1;				//enables the serial port
	CKP=1;					//Release clock	

	//SSPSTAT
	SMP=1;					//standart speed mode
	CKE=0;					//data transmited on rising edge

	//PIE
	SSPIE=1;				//serial interrupt enable bit
	//BCLIE=1;				//bus collision interrupt enable bit
	
	//IPR
	//high prioriy interrupt				
	SSPIP=1;				
	
	BufI2C.indeks=0;
	DtI2C.error=0;
	DtI2C.krok=0;
}


//-------------------------------------------------------------------------------
//Procedura transferu/odbioru danych
//UWAGA: umiescic w przerwaniu od SSPIF.
void I2CSlaveTR(void)
{
	unsigned char dt;
	
	if(WCOL)							//nadpisanie danej przy zapisie?
	{ 
		WCOL=0;
		DtI2C.error=_ErWCOL;

	}
	else
	if(SSPOV) 							//nadpisanie danej przy odbiorze?
	{
		SSPOV=0;
		DtI2C.error=_ErSSPOV;
	}
	else
	{ 
		if(!DA)							//adres czy dane?
		{	
			//ADRES
			if(RW) 
			{
				//Przygotowanie do transmisji danych
				DtI2C.krok=1;
				SetTData();
				BufI2C.Bf[_NBDATA-1]=LiczCHK(_NBDATA);
				BufI2C.indeks=0;
				dt=SSPBUF;
			}
			else 
			{
				//Przygotowanie do odbioru danych
				DtI2C.krok=0;
				SetRData();
				dt=SSPBUF;
				return;
			}
		}
		//DATA
		//Odbior danych
		if(DtI2C.krok==0) 
		{
			ReceptData();
			return;
		}
		//Transmisja danych
		if(DtI2C.krok==1) 
		{
			TransmitData();
			return;
		}
	}
	//Koniec transmisji/odbioru
	CKP=1;
	DtI2C.krok=0;
	BufI2C.indeks=0;
}
//-------------------------------------------------------------------------------
//Oblicza CHK i umieszcza go w ostatnim bajcie bufora danych
//NBTI2C - liczba danych bez adresu z CHK
unsigned char LiczCHK(const unsigned char NBTI2C)
{
	unsigned char i;
	unsigned char chk=0;
	
	for(i=0;i<NBTI2C-1;i++) chk+=(BufI2C.Bf[i]+i)^0xAA;
	return chk;		
}
//-------------------------------------------------------------------------------
//Wysyla dane do odbiornika
void TransmitData(void)
{
	if(RW)				//ACK lub bit STOP?
	{
		SSPBUF=BufI2C.Bf[BufI2C.indeks];
		CKP=1;
		if(BufI2C.indeks<_NBDATA-1) BufI2C.indeks++;	
	}
	else
	{
		CKP=1;
		BufI2C.indeks=0;		
	}
}
//-------------------------------------------------------------------------------
//Odbiera nadeslane dane
void ReceptData(void)
{
	if(BufI2C.indeks<_NRCDT)
	{
		BufI2C.Bf[BufI2C.indeks]=SSPBUF;
		BufI2C.indeks++;
		if(BufI2C.indeks==_NRCDT-1)
		if(LiczCHK(_NRCDT)==BufI2C.Bf[_NRCDT-1]) ToWork();
	}
}
//-------------------------------------------------------------------------------
//Operacje wykonywane po odebraniu danych
void ToWork(void)
{

}

//-------------------------------------------------------------------------------
//Zapis danych przeznaczonych do wysylki do bufora
void SetTData(void)
{
	unsigned char a,b;
	unsigned int prpwm;
	float vprz;
    
	//prpwm=(PWM.BufPWM-PWM.MDMOD)*100;
	//prpwm/=(PWM.MGMOD-PWM.MDMOD);
	if(prpwm>99) prpwm=99;
	/*if(PWM.BufPWM>=200)
	{
 		a=PWM.BufPWM-200;
		b=2;
	}
	else
	if(PWM.BufPWM>=100)
	{
 		a=PWM.BufPWM-100;
		b=1;
	}
	else
	{
 		a=PWM.BufPWM;
		b=0;
	}*/
	BufI2C.Bf[0]=DtI2C.kod;
	BufI2C.Bf[1]=(rgl.fz)?1:0;//Kvnt[7].dvnt%100;//M.rVVNT%100;//Kvnt[7].dvnt%100;//DtI2C.PCO;//(M._min_SRW)?1:0;//DtI2C.PCO;//M.LICZNIK_CIG3;//DtI2C.PCO;//M.LICZNIK_CIG3;
	BufI2C.Bf[2]=0;//Kvnt[7].dvnt/100;//M.rVVNT/100;////Kvnt[7].dvnt/100;//DtI2C.CO;//PWM.APROC;//DtI2C.CO;//M.LICZNIK_CIG;//DtI2C.CO;//M.LICZNIK_CIG;
	BufI2C.Bf[3]=DtI2C.PCW;//Kvnt[7].dpwm%100;//DtI2C.PCW;//RTS(_RTRSCI2)/10;//DtI2C.PCW;//RTS(_RTRSCI2)/10;
	BufI2C.Bf[4]=DtI2C.CW;//Kvnt[7].dpwm/100;//DtI2C.CW;//RTS(_RTRSCI)/10;//DtI2C.CW;//RTS(_RTRSCI)/10;
	BufI2C.Bf[5]=0;//DtI2C.CS;//M._ZAPLON;//prpwm;////DtCW.ddDCW;//DtI2C.CS;
	BufI2C.Bf[6]=DtI2C.STR_PWM;
	BufI2C.Bf[7]=DtI2C.MAX_PWM;
	BufI2C.Bf[8]=DtI2C.BUF_PWM;
	BufI2C.Bf[9]=DtI2C.ST_AD1;
	BufI2C.Bf[10]=DtI2C.ST_AD2;
	BufI2C.Bf[11]=DtI2C.ST_AD3;
	BufI2C.Bf[12]=DtI2C.PORTC;
	BufI2C.Bf[13]=DtI2C.PORTD;
	BufI2C.Bf[14]=DtI2C.ST_GLOBAL0;
	BufI2C.Bf[15]=DtI2C.ST_GLOBAL2;
	BufI2C.Bf[16]=DtI2C.ST_GLOBAL3;
	BufI2C.Bf[17]=DtI2C.ST_GLOBAL4;
	if(M.VVNT) vprz=(float)M.VVNT*0.7+5.0/7;
	else vprz=0;
	BufI2C.Bf[18]=(unsigned char)vprz;//DtI2C.TPRZ_CW;    
	//BufI2C.Bf[18]=(rgl.fz)?2*7:7;//DtI2C.TPRZ_CW;
	BufI2C.Bf[19]=DtI2C.ST0_WSW;
	BufI2C.Bf[20]=DtI2C.ERR_BTY;
}
//-------------------------------------------------------------------------------
//Przygotowanie na odbior danych
void SetRData(void)
{
	BufI2C.indeks=0;
}
//-------------------------------------------------------------------------------
//Formatowanie danych na potrzeby modulu IMY-02
/*
void FormatDataIMY(void)
{
	//Kod sterownika
	if(!M._TURBO) DtI2C.kod=0xE0;
	else DtI2C.kod=0xE1;
	//PCO
	DtI2C.PCO=0;
	//CO
	DtI2C.CO=0;
	//PCW
	if(M._AKTYWNE_KNF) DtI2C.PCW=DtCW.PPCW;
	else DtI2C.PCW=DtCW.PCW;
	//CW
	DtI2C.CW=DtCW.CW;
	//CS
	DtI2C.CS=0;
	//Moc startowa
	DtI2C.STR_PWM=PWM.MSTART;
	//Moc maksymalna
	DtI2C.MAX_PWM=PWM.MMAXCO;
	//Wypelnienia modulatora
	DtI2C.BUF_PWM=PWM.BufPWM;
	//ST_AD1
	BitSet(&DtI2C.ST_AD1,b_ERR_CO,0);
	BitSet(&DtI2C.ST_AD1,b_ERR_CW,DtCW._err_CW);
	BitSet(&DtI2C.ST_AD1,b_NEW_PCO,0);
	BitSet(&DtI2C.ST_AD1,b_NEW_PCW,DtCW._newPCW);
	BitSet(&DtI2C.ST_AD1,b_TERR_CSN,0);
	BitSet(&DtI2C.ST_AD1,b_TLO_CSN,0);
	BitSet(&DtI2C.ST_AD1,b_THI_CSN,0);
	BitSet(&DtI2C.ST_AD1,b_TWRK_CSN,1);
	//ST_AD2
	BitSet(&DtI2C.ST_AD2,b_SU_CO,0);
	BitSet(&DtI2C.ST_AD2,b_AD_CO,0);
	BitSet(&DtI2C.ST_AD2,b_ON_DZ,0);
	BitSet(&DtI2C.ST_AD2,b_ON_POMP,0);
	BitSet(&DtI2C.ST_AD2,b_SU_CW,DtCW._su_CW);
	BitSet(&DtI2C.ST_AD2,b_AD_CW,DtCW._ad_CW);
	BitSet(&DtI2C.ST_AD2,b_S_DTF,0);
	BitSet(&DtI2C.ST_AD2,b_F_DTF,0);	
	//ST_AD3
	BitSet(&DtI2C.ST_AD3,b_HOT_CO,0);
	BitSet(&DtI2C.ST_AD3,b_NO_CSN,0);
	BitSet(&DtI2C.ST_AD3,b_F_LO_CSN,0);
	BitSet(&DtI2C.ST_AD3,b_S_LO_CSN,0);
	BitSet(&DtI2C.ST_AD3,b_BLOK_15S,0);
	//PORTC
	BitSet(&DtI2C.PORTC,b_PLOMIEN,_PLOMIEN);
	BitSet(&DtI2C.PORTC,b_ALARM,1);
	BitSet(&DtI2C.PORTC,b_VNT,_VNT);
	BitSet(&DtI2C.PORTC,b_PRZEPLYW_CO,0);
	BitSet(&DtI2C.PORTC,b_BRAK_CIAGU,_CIG_NO);
	BitSet(&DtI2C.PORTC,b_MOD_ON,0);	
	//PORTD
	BitSet(&DtI2C.PORTD,b_DTF,0);
	BitSet(&DtI2C.PORTD,b_GRZEJ,_GRZEJ);
	BitSet(&DtI2C.PORTD,b_KCO_KCW,0);
	BitSet(&DtI2C.PORTD,b_POMPA,0);		
	BitSet(&DtI2C.PORTD,b_RESET_O,0);
	BitSet(&DtI2C.PORTD,b_SW_CONTROL,1);
	BitSet(&DtI2C.PORTD,b_ZIMA,0);
	//ST_GLOBAL0
	BitSet(&DtI2C.ST_GLOBAL0,b_S_CW,RdPrt(S_WCW));
	BitSet(&DtI2C.ST_GLOBAL0,b_S_RT,0);
	BitSet(&DtI2C.ST_GLOBAL0,b_S_ALARM,1);
	BitSet(&DtI2C.ST_GLOBAL0,b_S_PRZ_CO,0);
	if(!M._TURBO)		
		BitSet(&DtI2C.ST_GLOBAL0,b_S_BRAK_CI,!RdPrt(S_CIG_NC));
	else
		BitSet(&DtI2C.ST_GLOBAL0,b_S_BRAK_CI,!RdPrt(S_CIG_NO));
	BitSet(&DtI2C.ST_GLOBAL0,b_S_MOD_ON,0);
	BitSet(&DtI2C.ST_GLOBAL0,b_S_RESET,0);
	BitSet(&DtI2C.ST_GLOBAL0,b_S_PLOMIEN,!RdPrt(S_PLM));
	//ST_GLOBAL2	
	BitSet(&DtI2C.ST_GLOBAL2,b_OPDG,0);
	BitSet(&DtI2C.ST_GLOBAL2,b_GP,DtKNF.rdgz);
	BitSet(&DtI2C.ST_GLOBAL2,b_OK_WRT,0);
	BitSet(&DtI2C.ST_GLOBAL2,b_S_ON_DZ,0);
	BitSet(&DtI2C.ST_GLOBAL2,b_F_ON_DZ,0);	
	BitSet(&DtI2C.ST_GLOBAL2,b_S_SWC,1);
	BitSet(&DtI2C.ST_GLOBAL2,b_F_SWC,1);
	BitSet(&DtI2C.ST_GLOBAL2,b_S_STB,!RdPrt(S_STB));
	//ST_GLOBAL3
	BitSet(&DtI2C.ST_GLOBAL3,b_ROZRUCH,M._ROZRUCH);
	BitSet(&DtI2C.ST_GLOBAL3,b_WYLACZ,0);
	BitSet(&DtI2C.ST_GLOBAL3,b_RUSZ_CIAG,M._RUSZ_CIAG);
	BitSet(&DtI2C.ST_GLOBAL3,b_RUSZ_POMPE,0);
	BitSet(&DtI2C.ST_GLOBAL3,b_CZEKAJ_NTC,0);	
	BitSet(&DtI2C.ST_GLOBAL3,b_RUSZ_PLOMIEN,M._RUSZ_PLOMIEN);
	BitSet(&DtI2C.ST_GLOBAL3,b_RUSZ_STB,M._RUSZ_STB);
	BitSet(&DtI2C.ST_GLOBAL3,b_BLOKUJ_3M,0);
	//ST_GLOBAL4
	BitSet(&DtI2C.ST_GLOBAL4,b_RUSZ_48,0);
	BitSet(&DtI2C.ST_GLOBAL4,b_ROZRUCH_AWR,0);
	BitSet(&DtI2C.ST_GLOBAL4,b_MODULUJ,M._MODULUJ);
	BitSet(&DtI2C.ST_GLOBAL4,b_OST_STB,0);
	BitSet(&DtI2C.ST_GLOBAL4,b_OST_PLM,M._OST_PL);	
	BitSet(&DtI2C.ST_GLOBAL4,b_VWYBIEG,M._VWYBIEG);
	BitSet(&DtI2C.ST_GLOBAL4,b_PPOMP,0);
	BitSet(&DtI2C.ST_GLOBAL4,b_ZMKCOCW,0);
	//TPRZ_CW
	DtI2C.TPRZ_CW=0;
	//ST0_WSW
	DtI2C.ST0_WSW=0;
	BitSet(&DtI2C.ST0_WSW,b_MSG_A1,!M.ERR_BTY&&M._RUSZ_PLOMIEN);
	BitSet(&DtI2C.ST0_WSW,b_MSG_A2,!M.ERR_BTY&&M._RUSZ_STB);
	BitSet(&DtI2C.ST0_WSW,b_MSG_A3,!M.ERR_BTY&&M._RUSZ_CIAG&&M._prb_CIG);
	switch(M.ERR_BTY)
	{
		case 0x01:
			DtI2C.ERR_BTY=1;
		break;
		case 0x02:
			DtI2C.ERR_BTY=2;
		break;
		case 0x03:
			DtI2C.ERR_BTY=3;
		break;
		case 0x04:
			BitSet(&DtI2C.ST0_WSW,b_MSG_A4,1);
		break;
		case 0x05:
			BitSet(&DtI2C.ST0_WSW,b_MSG_A5,1);
		break;
		case 0x06:
			DtI2C.ERR_BTY=6;
		break;
		case 0x07:
			BitSet(&DtI2C.ST0_WSW,b_MSG_A7,1);
		break;
		case 0x08:
			BitSet(&DtI2C.ST0_WSW,b_MSG_A8,1);
		break;
		case 0x09:
			BitSet(&DtI2C.ST0_WSW,b_MSG_A9,1);
		break;
	}		
}
*/
void FormatDataIMY(void)
{
	//Kod sterownika
	if(!M._TURBO) DtI2C.kod=0xE0;
	else DtI2C.kod=0xE1;
	//PCO
	DtI2C.PCO=0;
	//CO
	DtI2C.CO=0;
	//PCW
	if(M._AKTYWNE_KNF) DtI2C.PCW=DtCW.PPCW;
	else DtI2C.PCW=DtCW.PCW;
	//CW
	DtI2C.CW=DtCW.CW;
	//CS
	DtI2C.CS=0;
	//Moc startowa
	DtI2C.STR_PWM=PWM.MSTART;
	//Moc maksymalna
	DtI2C.MAX_PWM=PWM.MMAXCO;
	//Wypelnienia modulatora
	DtI2C.BUF_PWM=PWM.BufPWM;

    //ST_AD1
    BitSet(&DtI2C.ST_AD1, b_ERR_CO,0);
    BitSet(&DtI2C.ST_AD1, b_ERR_CW, DtCW._err_CW);
    BitSet(&DtI2C.ST_AD1, b_NEW_PCO,0);
    BitSet(&DtI2C.ST_AD1, b_NEW_PCW, DtCW._newPCW);
    BitSet(&DtI2C.ST_AD1, b_TERR_CSN,0);
    BitSet(&DtI2C.ST_AD1, b_TLO_CSN, 0);
    BitSet(&DtI2C.ST_AD1, b_THI_CSN, 0);
    BitSet(&DtI2C.ST_AD1, b_TWRK_CSN,1);
    //ST_AD2
    BitSet(&DtI2C.ST_AD2, b_SU_CO, 0);
    BitSet(&DtI2C.ST_AD2, b_AD_CO, 0);
    BitSet(&DtI2C.ST_AD2, b_ON_DZ, 0);
    BitSet(&DtI2C.ST_AD2, b_ON_POMP,0);
    BitSet(&DtI2C.ST_AD2, b_SU_CW, DtCW._su_CW);
    BitSet(&DtI2C.ST_AD2, b_AD_CW, DtCW._ad_CW);
    BitSet(&DtI2C.ST_AD2, b_S_DTF, 0);
    BitSet(&DtI2C.ST_AD2, b_F_DTF, 0);
    //ST_AD3
    BitSet(&DtI2C.ST_AD3, b_HOT_CO,0);
    BitSet(&DtI2C.ST_AD3, b_NO_CSN,0);
    BitSet(&DtI2C.ST_AD3, b_F_LO_CSN,0);
    BitSet(&DtI2C.ST_AD3, b_S_LO_CSN,0);
    BitSet(&DtI2C.ST_AD3, b_BLOK_15S,0);
    //PORTC
    BitSet(&DtI2C.PORTC, b_PLOMIEN, _PLOMIEN);
    BitSet(&DtI2C.PORTC, b_ALARM, 1);
    BitSet(&DtI2C.PORTC, b_VNT, _VNT);
    BitSet(&DtI2C.PORTC, b_PRZEPLYW_CO,0);
    if(!M._TURBO)
        BitSet(&DtI2C.PORTC, b_BRAK_CIAGU, _CIG_NC);
    else
        BitSet(&DtI2C.PORTC, b_BRAK_CIAGU, _CIG_NO);
    BitSet(&DtI2C.PORTC, b_MOD_ON, _MOD_ON);
    //PORTD
    BitSet(&DtI2C.PORTD, b_DTF, 0);
    BitSet(&DtI2C.PORTD, b_GRZEJ, _GRZEJ);
    BitSet(&DtI2C.PORTD, b_KCO_KCW,0);
    BitSet(&DtI2C.PORTD, b_POMPA,0);
    BitSet(&DtI2C.PORTD, b_RESET_O,0); //0);
    BitSet(&DtI2C.PORTD, b_SW_CONTROL,1); //PFN._lato||PFN._zima);
    BitSet(&DtI2C.PORTD, b_ZIMA,0);
    //ST_GLOBAL0
	BitSet(&DtI2C.ST_GLOBAL0,b_S_CW,RdPrt(S_WCW));
    BitSet(&DtI2C.ST_GLOBAL0, b_S_RT,0);
    BitSet(&DtI2C.ST_GLOBAL0, b_S_ALARM, 1);
    BitSet(&DtI2C.ST_GLOBAL0, b_S_PRZ_CO,0);
    if(!M._TURBO)
        BitSet(&DtI2C.ST_GLOBAL0, b_S_BRAK_CI, !RdPrt(S_CIG_NC));
    else
        BitSet(&DtI2C.ST_GLOBAL0, b_S_BRAK_CI, !RdPrt(S_CIG_NO));
    BitSet(&DtI2C.ST_GLOBAL0, b_S_MOD_ON, !RdPrt(S_MOD_ON));
    BitSet(&DtI2C.ST_GLOBAL0, b_S_RESET,0); 
    BitSet(&DtI2C.ST_GLOBAL0, b_S_PLOMIEN, !RdPrt(S_PLM));
    //ST_GLOBAL2
	BitSet(&DtI2C.ST_GLOBAL2,b_OPDG,0);
	BitSet(&DtI2C.ST_GLOBAL2,b_GP,DtKNF.rdgz);
	BitSet(&DtI2C.ST_GLOBAL2,b_OK_WRT,0);
	BitSet(&DtI2C.ST_GLOBAL2,b_S_ON_DZ,0);
	BitSet(&DtI2C.ST_GLOBAL2,b_F_ON_DZ,0);	
	BitSet(&DtI2C.ST_GLOBAL2,b_S_SWC,1);
	BitSet(&DtI2C.ST_GLOBAL2,b_F_SWC,1);
	BitSet(&DtI2C.ST_GLOBAL2,b_S_STB,!RdPrt(S_STB));
    //ST_GLOBAL3
 	BitSet(&DtI2C.ST_GLOBAL3,b_ROZRUCH,M._ROZRUCH);
	BitSet(&DtI2C.ST_GLOBAL3,b_WYLACZ,0);
	BitSet(&DtI2C.ST_GLOBAL3,b_RUSZ_CIAG,M._RUSZ_CIAG);
	BitSet(&DtI2C.ST_GLOBAL3,b_RUSZ_POMPE,0);
	BitSet(&DtI2C.ST_GLOBAL3,b_CZEKAJ_NTC,0);	
	BitSet(&DtI2C.ST_GLOBAL3,b_RUSZ_PLOMIEN,M._RUSZ_PLOMIEN);
	BitSet(&DtI2C.ST_GLOBAL3,b_RUSZ_STB,M._RUSZ_STB);
	BitSet(&DtI2C.ST_GLOBAL3,b_BLOKUJ_3M,0);
    //ST_GLOBAL4
	BitSet(&DtI2C.ST_GLOBAL4,b_RUSZ_48,0);
	BitSet(&DtI2C.ST_GLOBAL4,b_ROZRUCH_AWR,0);
	BitSet(&DtI2C.ST_GLOBAL4,b_MODULUJ,M._MODULUJ);
	BitSet(&DtI2C.ST_GLOBAL4,b_OST_STB,0);
	BitSet(&DtI2C.ST_GLOBAL4,b_OST_PLM,M._OST_PL);	
	BitSet(&DtI2C.ST_GLOBAL4,b_VWYBIEG,M._VWYBIEG);
	BitSet(&DtI2C.ST_GLOBAL4,b_PPOMP,0);
	BitSet(&DtI2C.ST_GLOBAL4,b_ZMKCOCW,0);
 
	//TPRZ_CW
	DtI2C.TPRZ_CW=0;
	//ST0_WSW
	DtI2C.ST0_WSW=0;
	BitSet(&DtI2C.ST0_WSW,b_MSG_A1,!M.ERR_BTY&&M._RUSZ_PLOMIEN);
	BitSet(&DtI2C.ST0_WSW,b_MSG_A2,!M.ERR_BTY&&M._RUSZ_STB);
	BitSet(&DtI2C.ST0_WSW,b_MSG_A3,!M.ERR_BTY&&M._RUSZ_CIAG&&M._prb_CIG);
    switch(M.ERR_BTY)
    {
        case 0x01:
            DtI2C.ERR_BTY = 1;
            break;
        case 0x02:
            DtI2C.ERR_BTY = 2;
            break;
        case 0x03:
            DtI2C.ERR_BTY = 3;
            break;
 		case 0x04:
			BitSet(&DtI2C.ST0_WSW,b_MSG_A4,1);
		break;
		case 0x05:
			BitSet(&DtI2C.ST0_WSW,b_MSG_A5,1);
		break;
        case 0x06:
            DtI2C.ERR_BTY = 6;
            break;
        case 0x07:
            BitSet(&DtI2C.ST0_WSW, b_MSG_A7, 1);
            break;
        case 0x08:
            BitSet(&DtI2C.ST0_WSW, b_MSG_A8, 1);
            break;
        case 0x09:
            BitSet(&DtI2C.ST0_WSW, b_MSG_A9, 1);
            break;
    }	
}
//-------------------------------------------------------------------------------
