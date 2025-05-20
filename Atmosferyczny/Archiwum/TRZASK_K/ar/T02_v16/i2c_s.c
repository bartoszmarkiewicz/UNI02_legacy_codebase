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
#include "uart9b.h"			//parametry lokalne
#include "pompa.tp.h"

extern UARTData UADt;
static DataI2C	DtI2C;
static bfI2C	BufI2C;

extern tpMAIN M;
extern DataCO DtCO;
extern DataCW DtCW;
extern DataZW DtZW;
extern DataIN DtIN;
extern DtPMP PMP;
extern DataPFN PFN;
extern DataCSN DtCS;
extern DtPWM PWM;
extern DtKONF DtKNF;
extern DtRgl rgl;
extern tpRSDTA1 RSDT1;

extern unsigned char x;
extern unsigned char xxy,zzz,vvv,mmm;
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

    SEN=1;                  //Stretch enable bit
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
	SSPEN=1;				//enables the serial port
}


//-------------------------------------------------------------------------------
//Procedura transferu/odbioru danych
//UWAGA: umiescic w przerwaniu od SSPIF.
volatile static unsigned char dt;
/*void I2CSlaveTR(void)
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
			ReceptDataI2C();
			return;
		}
		//Transmisja danych
		if(DtI2C.krok==1) 
		{
			TransmitDataI2C();
			return;
		}
	}
	//Koniec transmisji/odbioru
	CKP=1;
	DtI2C.krok=0;
	BufI2C.indeks=0;
}*/
void I2CSlaveTR(void)
{
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
        dt = SSPBUF;
	}
	else
	{ 
		if(!DA)							//adres czy dane?
		{	
			//ADRES
			if(RW) 						//transmisja danych
			{
				//Przygotowanie do transmisji danych
				DtI2C.krok=1;
				dt=SSPBUF;
				SetTData();
				BufI2C.Bf[_NBDATA-1]=LiczCHK(_NBDATA);
				BufI2C.indeks=0;
			}
			else 
			{
				//Przygotowanie do odbioru danych
				DtI2C.krok=0;
				dt=SSPBUF;
				SetRData();
                CKP = 1;    //uwolnij zegar
				return;
			}
		}
		//DATA
		//Odbior danych
		if(DtI2C.krok==0) 
		{
			ReceptDataI2C();
            CKP = 1;    //uwolnij zegar
			return;
		}
		//Transmisja danych
		if(DtI2C.krok==1) 
		{
			TransmitDataI2C();
            CKP = 1;    //uwolnij zegar
			return;
		}
	}
	//Koniec transmisji/odbioru
	dt=SSPBUF;
	DtI2C.krok=0;
	BufI2C.indeks=0;
    CKP = 1;    //uwolnij zegar
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
/*
void TransmitDataI2C(void)
{
	if(RW)				//ACK lub bit STOP?
	{
		if(BufI2C.indeks<_NBSIZE) SSPBUF=BufI2C.Bf[BufI2C.indeks];
		else SSPBUF=BufI2C.Bf[_NBSIZE-1];
		CKP=1;
		if(BufI2C.indeks<_NBDATA-1) BufI2C.indeks++;	
	}
	else
	{
		CKP=1;
		BufI2C.indeks=0;		
	}
}*/
void TransmitDataI2C(void)
{
	if(RW)				//ACK lub bit STOP?
	{
		if(BufI2C.indeks<_NBSIZE) SSPBUF=BufI2C.Bf[BufI2C.indeks];
		else SSPBUF=BufI2C.Bf[_NBSIZE-1];
		if(BufI2C.indeks<_NBDATA-1) BufI2C.indeks++;	
	}
	else
	{
		BufI2C.indeks=0;		
	}
}
//-------------------------------------------------------------------------------
//Odbiera nadeslane dane
/*
void ReceptDataI2C(void)
{
	if(BufI2C.indeks<_NRCDT)
	{
		BufI2C.Bf[BufI2C.indeks]=SSPBUF;
		BufI2C.indeks++;
		if(BufI2C.indeks==_NRCDT-1)
		if(LiczCHK(_NRCDT)==BufI2C.Bf[_NRCDT-1]) ToWork();
	}
}*/
void ReceptDataI2C(void)
{
	dt = SSPBUF;
	if(BufI2C.indeks<_NRCDT)
	{
		BufI2C.Bf[BufI2C.indeks]=dt;
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
    //unsigned char pg;
    //float mod;
	//unsigned int p;
	//p=(rgl.tmp_pwm-PWM.MDMOD)*100;
	//p=p/(PWM.MGMOD-PWM.MDMOD);
	BufI2C.Bf[0]=DtI2C.kod;
	BufI2C.Bf[1]=DtI2C.PCO;//mmm%100;
	BufI2C.Bf[2]=DtI2C.CO;//mmm/100;
	BufI2C.Bf[3]=DtI2C.PCW;//DtIN.IN;//DtI2C.PCW;
	BufI2C.Bf[4]=DtI2C.CW;//PMP.APROC;//DtKNF.rdpm;//DtI2C.CW;
	BufI2C.Bf[5]=DtI2C.CS;
	BufI2C.Bf[6]=DtIN.IN;//DtI2C.STR_PWM;   (dla b_PPOMP=1)
	BufI2C.Bf[7]=PMP.APROC;//DtI2C.MAX_PWM; (dla b_PPOMP=1)
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
	BufI2C.Bf[18]=DtI2C.TPRZ_CW;
    //pg=RSDT1.outPG;
    //if(pg>99) pg=99;
	//if(_GRZEJ) mod=(float)pg*0.7;//+4.5/7;
	//else mod=0;
    
	//BufI2C.Bf[18]=(unsigned char)mod;//DtI2C.TPRZ_CW;    
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
void FormatDataIMY(void)
{
	//Kod sterownika
	//DtI2C.kod=RSDT1.outRDZ0;
	switch(DtKNF.tpkt)		//dla danego rodzaju kotla
	{
		case _UNICO:		//unico - monotermiczny
			if(!M._TURBO) DtI2C.kod=0xA0;
			else DtI2C.kod=0xA5;
			break;
		case _BITERM:		//bitermiczny (miniterm)
			if(!M._TURBO) DtI2C.kod=0xA2;
			else DtI2C.kod=0xA7;	
			break;
		case _BITERM2:		//bitermiczny (maksiterm)
			if(!M._TURBO) DtI2C.kod=0xA4;
			else DtI2C.kod=0xA9;	
			break;
		case _MONOHB:		//monotermiczny z hydroblokiem
			if(!M._TURBO) DtI2C.kod=0xAA;
			else DtI2C.kod=0xAB;
			break;
		default:			//domyslny monotermiczny
			if(!M._TURBO) DtI2C.kod=0xAA;
			else DtI2C.kod=0xAB;	//inny domyslny
	}
	//PCO
    if(M._ROZRUCH||!M._PRACA)
    {
        DtI2C.PCO=DtCO.PPCO;
    }
    else
    {
        if(!M.STRB) //to nie jest tryb produkcyjny?
        {
            DtI2C.PCO=M.CONTSETP;
        }
        else
        {
            DtI2C.PCO=DtCO.PPCO;
        }
    }
	/*if(M._AKTYWNE_KNF) DtI2C.PCO=DtCO.PPCO;
	else
	if(M._AKTYWNE_PGD) DtI2C.PCO=DtCO.PPCO;
	else DtI2C.PCO=DtCO.PPCO;*/
	//CO
	DtI2C.CO=DtCO.CO;
	//PCW
	if(M._AKTYWNE_KNF) DtI2C.PCW=DtCW.PPCW;
	else
	if(M._pob_AL) DtI2C.PCW=DtCW.PPCW;
	else DtI2C.PCW=DtCW.PPCW;
	//CW
	DtI2C.CW=DtCW.CW;
	//CS
	DtI2C.CS=DtCS.CS;
	//Moc startowa
	DtI2C.STR_PWM=PWM.MSTART;
	//Moc maksymalna
	DtI2C.MAX_PWM=PWM.MMAXCO;
	//Wypelnienia modulatora
	DtI2C.BUF_PWM=PWM.BufPWM;
	//ST_AD1
	BitSet(&DtI2C.ST_AD1,b_ERR_CO,RdPrt(S_ERR_CO));
	BitSet(&DtI2C.ST_AD1,b_ERR_CW,DtCW._err_CW);
	BitSet(&DtI2C.ST_AD1,b_NEW_PCO,DtCO._newPCO);
	BitSet(&DtI2C.ST_AD1,b_NEW_PCW,DtCW._newPCW);
	BitSet(&DtI2C.ST_AD1,b_TERR_CSN,RdPrt(S_ERR_CSN));
	BitSet(&DtI2C.ST_AD1,b_TLO_CSN,DtCS._lo);
	BitSet(&DtI2C.ST_AD1,b_THI_CSN,DtCS._hi);
	BitSet(&DtI2C.ST_AD1,b_TWRK_CSN,RdPrt(S_WRK_CSN));
	//ST_AD2
	BitSet(&DtI2C.ST_AD2,b_SU_CO,DtCO._su_CO);
	BitSet(&DtI2C.ST_AD2,b_AD_CO,RdPrt(S_ADD_CO));
	BitSet(&DtI2C.ST_AD2,b_ON_DZ,DtCO._on_DZ_CO);
	BitSet(&DtI2C.ST_AD2,b_ON_POMP,DtCO._cold_CO||DtCO._hot_CO);
	BitSet(&DtI2C.ST_AD2,b_SU_CW,DtCW._su_CW);
	BitSet(&DtI2C.ST_AD2,b_AD_CW,DtCW._ad_CW);
	BitSet(&DtI2C.ST_AD2,b_S_DTF,0);//RdPrt(S_DTF));
    //BitSet(&DtI2C.ST_AD2,b_F_DTF,RdPrt(S_DTF));    
    if(M._KEY7)         //konsola 7 przyciskow?
    {
        BitSet(&DtI2C.ST_AD2,b_F_DTF,1);
        if(M._KKOM)     //nacisnieto przycisk specjalny?
        {
            BitSet(&DtI2C.ST_AD3,b_F_LO_CSN,1);    
        }
        else
        {
            BitSet(&DtI2C.ST_AD3,b_F_LO_CSN,0);
        }
    }
    else
    {
        BitSet(&DtI2C.ST_AD2,b_F_DTF,0);  
        BitSet(&DtI2C.ST_AD3,b_F_LO_CSN,RdPrt(S_LO_CSN));        
    }
	//ST_AD3
	BitSet(&DtI2C.ST_AD3,b_HOT_CO,DtCO._hot_CO);
	BitSet(&DtI2C.ST_AD3,b_NO_CSN,DtCS._no);
	//BitSet(&DtI2C.ST_AD3,b_F_LO_CSN,RdPrt(S_LO_CSN));
	BitSet(&DtI2C.ST_AD3,b_S_LO_CSN,RdPrt(S_LO_CSN));
	BitSet(&DtI2C.ST_AD3,b_BLOK_15S,M._BLOKUJ_15S);
	//PORTC
	BitSet(&DtI2C.PORTC,b_PLOMIEN,_PLOMIEN);
	BitSet(&DtI2C.PORTC,b_ALARM,1);
	BitSet(&DtI2C.PORTC,b_VNT,_VNT);
	BitSet(&DtI2C.PORTC,b_PRZEPLYW_CO,_PRZ_CO);
	if(!M._TURBO)
		BitSet(&DtI2C.PORTC,b_BRAK_CIAGU,_CIG_NC);
	else
		BitSet(&DtI2C.PORTC,b_BRAK_CIAGU,_CIG_NO);
	BitSet(&DtI2C.PORTC,b_MOD_ON,_MOD_ON);	
	//PORTD
    switch(M.STRB)  //tryb produkcyjny
    {
        case 0:
            BitSet(&DtI2C.PORTD,b_DTF,0);
            break;
        case 1:
            BitSet(&DtI2C.PORTD,b_DTF,1);
            break; 
        case 2:
            BitSet(&DtI2C.PORTD,b_DTF,0);
            break;  
        default:
            BitSet(&DtI2C.PORTD,b_DTF,1);
            break;            
    }

	BitSet(&DtI2C.PORTD,b_GRZEJ,_GRZEJ);
	BitSet(&DtI2C.PORTD,b_KCO_KCW,_KCO_CW);
	BitSet(&DtI2C.PORTD,b_POMPA,_POMPA);		
	BitSet(&DtI2C.PORTD,b_RESET_O,PFN._res);//0);
	BitSet(&DtI2C.PORTD,b_SW_CONTROL,PFN._off);//PFN._lato||PFN._zima);
	BitSet(&DtI2C.PORTD,b_ZIMA,PFN._zima);
	//ST_GLOBAL0
	if(DtKNF.tpkt!=_UNICO)
	{
        BitSet(&DtI2C.ST_GLOBAL0,b_S_CW,(!M._AKTYWNE_SRW&&!M._pob_KCO)?RdPrt(S_WCW):0);
	}
	else
	{
		if((M._pob_AL||M._pob_KCW||M._pob_DzCW)&&M._PRACA) BitSet(&DtI2C.ST_GLOBAL0,b_S_CW,1);
		else
		{
			if(!M._AKTYWNE_SRW&&!M._pob_KCO)
            {
                if(!DtKNF.cwtm)     //(timera zasobnika||LIN_CW||OT_CW)
                {
                    BitSet(&DtI2C.ST_GLOBAL0,b_S_CW,(RdPrt(S_WCW)&&DtCW._wpz_CW)?1:0);
                }
                else                //timera zasobnika
                {
                    BitSet(&DtI2C.ST_GLOBAL0,b_S_CW,(RdPrt(S_TIM)&&DtCW._wpz_CW)?1:0);                    
                }
            }
			else
            {
				BitSet(&DtI2C.ST_GLOBAL0,b_S_CW,0);
            }
		}
	}
	BitSet(&DtI2C.ST_GLOBAL0,b_S_RT,RdPrt(S_RT));
	BitSet(&DtI2C.ST_GLOBAL0,b_S_ALARM,1);
	BitSet(&DtI2C.ST_GLOBAL0,b_S_PRZ_CO,RdPrt(S_PRZ_CO));
	if(!M._TURBO)		
		BitSet(&DtI2C.ST_GLOBAL0,b_S_BRAK_CI,!RdPrt(S_CIG_NC));
	else
		BitSet(&DtI2C.ST_GLOBAL0,b_S_BRAK_CI,!RdPrt(S_CIG_NO));
	BitSet(&DtI2C.ST_GLOBAL0,b_S_MOD_ON,!RdPrt(S_MOD_ON));
	BitSet(&DtI2C.ST_GLOBAL0,b_S_RESET,PFN._res);//RdPrt(S_RESET));
	BitSet(&DtI2C.ST_GLOBAL0,b_S_PLOMIEN,!RdPrt(S_PLM));
	//ST_GLOBAL2	
	BitSet(&DtI2C.ST_GLOBAL2,b_OPDG,DtKNF.opdg);
	BitSet(&DtI2C.ST_GLOBAL2,b_GP,DtKNF.rdgz);
    switch(M.STRB)      //tryb produkcyjny
    {
        case 0:
            BitSet(&DtI2C.ST_GLOBAL2,b_OK_WRT,0);
            break;
        case 1:
            BitSet(&DtI2C.ST_GLOBAL2,b_OK_WRT,0);
            break; 
        case 2:
            BitSet(&DtI2C.ST_GLOBAL2,b_OK_WRT,1);
            break;  
        default:
            BitSet(&DtI2C.ST_GLOBAL2,b_OK_WRT,1);
            break;            
    }
	BitSet(&DtI2C.ST_GLOBAL2,b_S_ON_DZ,RdPrt(S_DZ_CO));
	BitSet(&DtI2C.ST_GLOBAL2,b_F_ON_DZ,RdPrt(S_DZ_CO));	
	BitSet(&DtI2C.ST_GLOBAL2,b_S_SWC,PFN._lato||PFN._zima);
	BitSet(&DtI2C.ST_GLOBAL2,b_F_SWC,PFN._lato||PFN._zima);
	BitSet(&DtI2C.ST_GLOBAL2,b_S_STB,!RdPrt(S_STB));
	//ST_GLOBAL3
	BitSet(&DtI2C.ST_GLOBAL3,b_ROZRUCH,M._ROZRUCH);
	BitSet(&DtI2C.ST_GLOBAL3,b_WYLACZ,M._WYLACZ);
	BitSet(&DtI2C.ST_GLOBAL3,b_RUSZ_CIAG,M._RUSZ_CIAG);
	BitSet(&DtI2C.ST_GLOBAL3,b_RUSZ_POMPE,M._RUSZ_POMPE);
	BitSet(&DtI2C.ST_GLOBAL3,b_CZEKAJ_NTC,M._RUSZ_NTC_CO);	
	BitSet(&DtI2C.ST_GLOBAL3,b_RUSZ_PLOMIEN,M._RUSZ_PLOMIEN);
	BitSet(&DtI2C.ST_GLOBAL3,b_RUSZ_STB,M._RUSZ_STB);
	BitSet(&DtI2C.ST_GLOBAL3,b_BLOKUJ_3M,M._BLOKUJ_3M);
	//ST_GLOBAL4
	BitSet(&DtI2C.ST_GLOBAL4,b_RUSZ_48,M._RUSZ_48);
	BitSet(&DtI2C.ST_GLOBAL4,b_ROZRUCH_AWR,0);
	BitSet(&DtI2C.ST_GLOBAL4,b_MODULUJ,M._MODULUJ);
	BitSet(&DtI2C.ST_GLOBAL4,b_OST_STB,0);
	BitSet(&DtI2C.ST_GLOBAL4,b_OST_PLM,M._OST_PL);	
	BitSet(&DtI2C.ST_GLOBAL4,b_VWYBIEG,M._VWYBIEG);
    if(DtKNF.rdpm)
    {
        BitSet(&DtI2C.ST_GLOBAL4,b_PPOMP,1);
    }
    else
    {
        BitSet(&DtI2C.ST_GLOBAL4,b_PPOMP,0);
    }
	BitSet(&DtI2C.ST_GLOBAL4,b_ZMKCOCW,M._ZMKCOCW);
	//TPRZ_CW
	DtI2C.TPRZ_CW=0;
	//ST0_WSW
	DtI2C.ST0_WSW=0;
	BitSet(&DtI2C.ST0_WSW,b_MSG_A1,!M.ERR_BTY&&M._RUSZ_PLOMIEN);
	BitSet(&DtI2C.ST0_WSW,b_MSG_A2,!M.ERR_BTY&&M._RUSZ_STB);
	BitSet(&DtI2C.ST0_WSW,b_MSG_A3,!M.ERR_BTY&&M._RUSZ_CIAG&&M._prb_CIG);
	if(DtKNF.robg==_OTWARTY) BitSet(&DtI2C.ST0_WSW,b_MSG_A5,!M.ERR_BTY&&M._RUSZ_POMPE&&M._prb_PRZ);

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
			if(DtKNF.robg==_OTWARTY) DtI2C.ERR_BTY=5;
			else BitSet(&DtI2C.ST0_WSW,b_MSG_A5,1);
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
//-------------------------------------------------------------------------------
