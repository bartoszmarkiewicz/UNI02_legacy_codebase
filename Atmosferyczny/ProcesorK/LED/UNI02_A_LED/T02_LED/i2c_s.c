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
#include "konfig2.h"
#include "rgl.h"
#include "uart9b.h"			//parametry lokalne
#include "pompa.tp.h"
extern UARTData UADt;
volatile static DataI2C DtI2C;
volatile static bfI2C BufI2C;

extern volatile tpMAIN M;
extern volatile DataCO DtCO;
extern volatile DataCW DtCW;
extern volatile DataZW DtZW;
extern volatile DataCSN DtCS;
extern volatile DtPWM PWM;
extern volatile DtKONF DtKNF;
extern volatile DtRgl rgl;
//extern tpRSDTA1 RSDT1;
extern volatile DataIN DtIN;
extern volatile DtPMP PMP;
extern volatile DataPFN PFN;

extern volatile unsigned char x,xj;
//extern unsigned char xxy,zzz,vvv,mmm;
//-------------------------------------------------------------------------------
//Inicjalizacja modulu I2C

void InitI2C(void)
{
    //TRISC
    TRISC3 = 1; //SDA i SCA konfiguracja jako wejscia
    TRISC4 = 1;

    //SSPADD
    SSPADD = _I2CSPADD;

    //SSPCON1
    SSPM0 = 0;
    SSPM1 = 1;
    SSPM2 = 1;
    SSPM3 = 0; //I2C Slave Mode, 7 bit address
    SSPEN = 1; //enables the serial port
    CKP = 1; //Release clock

    //SSPSTAT
    SMP = 1; //standart speed mode
    CKE = 0; //data transmited on rising edge

    //PIE
    SSPIE = 1; //serial interrupt enable bit
    //BCLIE=1;				//bus collision interrupt enable bit

    //IPR
    //high prioriy interrupt
    SSPIP = 1;

    BufI2C.indeks = 0;
    DtI2C.error = 0;
    DtI2Cf_krok = 0;
}


//-------------------------------------------------------------------------------
//Procedura transferu/odbioru danych
//UWAGA: umiescic w przerwaniu od SSPIF.
volatile static unsigned char dt;
/*void I2CSlaveTR(void)
{
    unsigned char dt;

    if(WCOL) //nadpisanie danej przy zapisie?
    {
        WCOL = 0;
        DtI2C.error = _ErWCOL;

    }
    else
        if(SSPOV) //nadpisanie danej przy odbiorze?
    {
        SSPOV = 0;
        DtI2C.error = _ErSSPOV;
    }
    else
    {
        if(!DA) //adres czy dane?
        {
            //ADRES
            if(RW)
            {
                //Przygotowanie do transmisji danych
                DtI2Cf_krok = 1;
                SetTData();
                BufI2C.Bf[_NBDATA - 1] = LiczCHK(_NBDATA);
                BufI2C.indeks = 0;
                dt = SSPBUF;
            }
            else
            {
                //Przygotowanie do odbioru danych
                DtI2Cf_krok = 0;
                SetRData();
                dt = SSPBUF;
                return;
            }
        }
        //DATA
        //Odbior danych
        if(DtI2Cf_krok == 0)
        {
            ReceptDataI2C();
            return;
        }
        //Transmisja danych
        if(DtI2Cf_krok == 1)
        {
            TransmitDataI2C();
            return;
        }
    }
    //Koniec transmisji/odbioru
    CKP = 1;
    DtI2Cf_krok = 0;
    BufI2C.indeks = 0;
}*/
void I2CSlaveTR(void)
{
    //unsigned char dt;
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
				DtI2Cf_krok=1;
				dt=SSPBUF;
				SetTData();
				BufI2C.Bf[_NBDATA-1]=LiczCHK(_NBDATA);
				BufI2C.indeks=0;
			}
			else 
			{
				//Przygotowanie do odbioru danych
				DtI2Cf_krok=0;
				dt=SSPBUF;
				SetRData();
                CKP = 1;    //uwolnij zegar
				return;
			}
		}
		//DATA
		//Odbior danych
		if(DtI2Cf_krok==0) 
		{
			ReceptDataI2C();
            CKP = 1;    //uwolnij zegar
			return;
		}
		//Transmisja danych
		if(DtI2Cf_krok==1) 
		{
			TransmitDataI2C();
            CKP = 1;    //uwolnij zegar
			return;
		}
	}
	//Koniec transmisji/odbioru
	dt=SSPBUF;
	DtI2Cf_krok=0;
	BufI2C.indeks=0;
    CKP = 1;    //uwolnij zegar
}
//-------------------------------------------------------------------------------
//Oblicza CHK i umieszcza go w ostatnim bajcie bufora danych
//NBTI2C - liczba danych bez adresu z CHK

unsigned char LiczCHK(const unsigned char NBTI2C)
{
    unsigned char i;
    unsigned char chk = 0;

    for(i = 0; i < NBTI2C - 1; i++) chk += (BufI2C.Bf[i] + i)^0xAA;
    return chk;
}
//-------------------------------------------------------------------------------
//Wysyla dane do odbiornika
/*
void TransmitDataI2C(void)
{
    if(RW) //ACK lub bit STOP?
    {
        if(BufI2C.indeks < _NBSIZE) SSPBUF = BufI2C.Bf[BufI2C.indeks];
        else SSPBUF = BufI2C.Bf[_NBSIZE - 1];
        CKP = 1;
        if(BufI2C.indeks < _NBDATA - 1) BufI2C.indeks++;
    }
    else
    {
        CKP = 1;
        BufI2C.indeks = 0;
    }
}
 */
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
    if(BufI2C.indeks < _NRCDT)
    {
        BufI2C.Bf[BufI2C.indeks] = SSPBUF;
        BufI2C.indeks++;
        if(BufI2C.indeks == _NRCDT - 1)
            if(LiczCHK(_NRCDT) == BufI2C.Bf[_NRCDT - 1]) ToWork();
    }
}
*/
void ReceptDataI2C(void)
{
    //unsigned char dt;
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
    //unsigned int p;
    //p=(rgl.tmp_pwm-PWM.MDMOD)*100;
    //p=p/(PWM.MGMOD-PWM.MDMOD);
    BufI2C.Bf[0] = DtI2C.kod;
    BufI2C.Bf[1] = DtI2C.PCO;//mmm%100;
    BufI2C.Bf[2] = DtI2C.CO;//mmm/100;
    BufI2C.Bf[3] = DtI2C.PCW;
    BufI2C.Bf[4] = DtI2C.CW;
    BufI2C.Bf[5] = DtI2C.CS;
    BufI2C.Bf[6] = DtI2C.STR_PWM;
    BufI2C.Bf[7] = DtI2C.MAX_PWM;
    BufI2C.Bf[8] = DtI2C.BUF_PWM;
    BufI2C.Bf[9] = DtI2C.ST_AD1;
    BufI2C.Bf[10] = DtI2C.ST_AD2;
    BufI2C.Bf[11] = DtI2C.ST_AD3;
    BufI2C.Bf[12] = DtI2C.PORTC;
    BufI2C.Bf[13] = DtI2C.PORTD;
    BufI2C.Bf[14] = DtI2C.ST_GLOBAL0;
    BufI2C.Bf[15] = DtI2C.ST_GLOBAL2;
    BufI2C.Bf[16] = DtI2C.ST_GLOBAL3;
    BufI2C.Bf[17] = DtI2C.ST_GLOBAL4;
    BufI2C.Bf[18] = DtI2C.TPRZ_CW;
    BufI2C.Bf[19] = DtI2C.ST0_WSW;
    BufI2C.Bf[20] = DtI2C.ERR_BTY;
}
//-------------------------------------------------------------------------------
//Przygotowanie na odbior danych

void SetRData(void)
{
    BufI2C.indeks = 0;
}
//-------------------------------------------------------------------------------
//Formatowanie danych na potrzeby modulu IMY-02
/*
void FormatDataIMY(void)
{
    //Kod sterownika
    //DtI2C.kod=RSDT1.outRDZ0;
    switch(DtKNF.tpkt) //dla danego rodzaju kotla
    {
        case _UNICO: //unico - monotermiczny
            if(!Mf_TURBO) DtI2C.kod = 0xA0;
            else DtI2C.kod = 0xA5;
            break;
        case _BITERM: //bitermiczny (miniterm)
            if(!Mf_TURBO) DtI2C.kod = 0xA2;
            else DtI2C.kod = 0xA7;
            break;
        case _BITERM2: //bitermiczny (maksiterm)
            if(!Mf_TURBO) DtI2C.kod = 0xA4;
            else DtI2C.kod = 0xA9;
            break;
        case _MONOHB: //monotermiczny z hydroblokiem
            if(!Mf_TURBO) DtI2C.kod = 0xAA;
            else DtI2C.kod = 0xAB;
            break;
        default: //domyslny monotermiczny
            if(!Mf_TURBO) DtI2C.kod = 0xAA;
            else DtI2C.kod = 0xAB; //inny domyslny
    }
    //PCO
    if(Mf_AKTYWNE_KNF) DtI2C.PCO = DtCO.PPCO;
    else
        if(Mf_AKTYWNE_PGD) DtI2C.PCO = DtCO.PPCO;
    else DtI2C.PCO = DtCO.PPCO;
    //CO
    DtI2C.CO = DtCO.CO;
    //PCW
    if(Mf_AKTYWNE_KNF) DtI2C.PCW = DtCW.PPCW;
    else
        if(Mf_pob_AL) DtI2C.PCW = DtCW.PPCW;
    else DtI2C.PCW = DtCW.PPCW;
    //CW
    DtI2C.CW = DtCW.CW;
    //CS
    DtI2C.CS = DtCS.CS;
    //Moc startowa
    DtI2C.STR_PWM = PWM.MSTART;
    //Moc maksymalna
    DtI2C.MAX_PWM = PWM.MMAXCO;
    //Wypelnienia modulatora
    DtI2C.BUF_PWM = PWM.BufPWM;
    //ST_AD1
    BitSet3(&DtI2C.ST_AD1, b_ERR_CO, RdPrt(S_ERR_CO));
    BitSet3(&DtI2C.ST_AD1, b_ERR_CW, DtCWf_err_CW);
    BitSet3(&DtI2C.ST_AD1, b_NEW_PCO, DtCOf_newPCO);
    BitSet3(&DtI2C.ST_AD1, b_NEW_PCW, DtCWf_newPCW);
    BitSet3(&DtI2C.ST_AD1, b_TERR_CSN, RdPrt(S_ERR_CSN));
    BitSet3(&DtI2C.ST_AD1, b_TLO_CSN, DtCSf_lo);
    BitSet3(&DtI2C.ST_AD1, b_THI_CSN, DtCSf_hi);
    BitSet3(&DtI2C.ST_AD1, b_TWRK_CSN, RdPrt(S_WRK_CSN));
    //ST_AD2
    BitSet3(&DtI2C.ST_AD2, b_SU_CO, DtCOf_su_CO);
    BitSet3(&DtI2C.ST_AD2, b_AD_CO, RdPrt(S_ADD_CO));
    BitSet3(&DtI2C.ST_AD2, b_ON_DZ, DtCOf_on_DZ_CO);
    BitSet3(&DtI2C.ST_AD2, b_ON_POMP, DtCOf_cold_CO || DtCOf_hot_CO);
    BitSet3(&DtI2C.ST_AD2, b_SU_CW, DtCWf_su_CW);
    BitSet3(&DtI2C.ST_AD2, b_AD_CW, DtCWf_ad_CW);
    BitSet3(&DtI2C.ST_AD2, b_S_DTF, RdPrt(S_DTF));
    BitSet3(&DtI2C.ST_AD2, b_F_DTF, RdPrt(S_DTF));
    //ST_AD3
    BitSet3(&DtI2C.ST_AD3, b_HOT_CO, DtCOf_hot_CO);
    BitSet3(&DtI2C.ST_AD3, b_NO_CSN, DtCSf_no);
    BitSet3(&DtI2C.ST_AD3, b_F_LO_CSN, RdPrt(S_LO_CSN));
    BitSet3(&DtI2C.ST_AD3, b_S_LO_CSN, RdPrt(S_LO_CSN));
    BitSet3(&DtI2C.ST_AD3, b_BLOK_15S, Mf_BLOKUJ_15S);
    //PORTC
    BitSet3(&DtI2C.PORTC, b_PLOMIEN, _PLOMIEN);
    BitSet3(&DtI2C.PORTC, b_ALARM, 1);
    BitSet3(&DtI2C.PORTC, b_VNT, _VNT);
    BitSet3(&DtI2C.PORTC, b_PRZEPLYW_CO, _PRZ_CO);
    if(!Mf_TURBO)
        BitSet3(&DtI2C.PORTC, b_BRAK_CIAGU, _CIG_NC);
    else
        BitSet3(&DtI2C.PORTC, b_BRAK_CIAGU, _CIG_NO);
    BitSet3(&DtI2C.PORTC, b_MOD_ON, _MOD_ON);
    //PORTD
    BitSet3(&DtI2C.PORTD, b_DTF, 0);
    BitSet3(&DtI2C.PORTD, b_GRZEJ, _GRZEJ);
    BitSet3(&DtI2C.PORTD, b_KCO_KCW, _KCO_CW);
    BitSet3(&DtI2C.PORTD, b_POMPA, _POMPA);
    BitSet3(&DtI2C.PORTD, b_RESET_O, PFNf_res); //0);
    BitSet3(&DtI2C.PORTD, b_SW_CONTROL, PFNf_off); //PFNf_lato||PFNf_zima);
    BitSet3(&DtI2C.PORTD, b_ZIMA, PFNf_zima);
    //ST_GLOBAL0
    if(DtKNF.tpkt != _UNICO)
    {
        BitSet3(&DtI2C.ST_GLOBAL0, b_S_CW, (!Mf_AKTYWNE_SRW && !Mf_pob_KCO) ? RdPrt(S_WCW) : 0);
    }
    else
    {
        if((Mf_pob_AL || Mf_pob_KCW || Mf_pob_DzCW) && Mf_PRACA) BitSet3(&DtI2C.ST_GLOBAL0, b_S_CW, 1);
        else
        {
            if(!Mf_AKTYWNE_SRW&&!Mf_pob_KCO)
                    BitSet3(&DtI2C.ST_GLOBAL0,b_S_CW,(RdPrt(S_WCW)&&DtCWf_wpz_CW)?1:0);
            else
                    BitSet3(&DtI2C.ST_GLOBAL0,b_S_CW,0);
        }
    }
    BitSet3(&DtI2C.ST_GLOBAL0, b_S_RT, RdPrt(S_RT));
    BitSet3(&DtI2C.ST_GLOBAL0, b_S_ALARM, 1);
    BitSet3(&DtI2C.ST_GLOBAL0, b_S_PRZ_CO, RdPrt(S_PRZ_CO));
    if(!Mf_TURBO)
        BitSet3(&DtI2C.ST_GLOBAL0, b_S_BRAK_CI, !RdPrt(S_CIG_NC));
    else
        BitSet3(&DtI2C.ST_GLOBAL0, b_S_BRAK_CI, !RdPrt(S_CIG_NO));
    BitSet3(&DtI2C.ST_GLOBAL0, b_S_MOD_ON, !RdPrt(S_MOD_ON));
    BitSet3(&DtI2C.ST_GLOBAL0, b_S_RESET, PFNf_res); //RdPrt(S_RESET));
    BitSet3(&DtI2C.ST_GLOBAL0, b_S_PLOMIEN, !RdPrt(S_PLM));
    //ST_GLOBAL2
    BitSet3(&DtI2C.ST_GLOBAL2, b_OPDG, DtKNF.opdg);
    BitSet3(&DtI2C.ST_GLOBAL2, b_GP, DtKNF.rdgz);
    BitSet3(&DtI2C.ST_GLOBAL2, b_OK_WRT, 0);
    BitSet3(&DtI2C.ST_GLOBAL2, b_S_ON_DZ, RdPrt(S_DZ_CO));
    BitSet3(&DtI2C.ST_GLOBAL2, b_F_ON_DZ, RdPrt(S_DZ_CO));
    BitSet3(&DtI2C.ST_GLOBAL2, b_S_SWC, !PFNf_off);//PFNf_lato || PFNf_zima);
    BitSet3(&DtI2C.ST_GLOBAL2, b_F_SWC, !PFNf_off);//PFNf_lato || PFNf_zima);
    BitSet3(&DtI2C.ST_GLOBAL2, b_S_STB, !RdPrt(S_STB));
    //ST_GLOBAL3
    BitSet3(&DtI2C.ST_GLOBAL3, b_ROZRUCH, Mf_ROZRUCH);
    BitSet3(&DtI2C.ST_GLOBAL3, b_WYLACZ, Mf_WYLACZ);
    BitSet3(&DtI2C.ST_GLOBAL3, b_RUSZ_CIAG, Mf_RUSZ_CIAG);
    BitSet3(&DtI2C.ST_GLOBAL3, b_RUSZ_POMPE, Mf_RUSZ_POMPE);
    BitSet3(&DtI2C.ST_GLOBAL3, b_CZEKAJ_NTC, Mf_RUSZ_NTC_CO);
    BitSet3(&DtI2C.ST_GLOBAL3, b_RUSZ_PLOMIEN, Mf_RUSZ_PLOMIEN);
    BitSet3(&DtI2C.ST_GLOBAL3, b_RUSZ_STB, Mf_RUSZ_STB);
    BitSet3(&DtI2C.ST_GLOBAL3, b_BLOKUJ_3M, Mf_BLOKUJ_3M);
    //ST_GLOBAL4
    BitSet3(&DtI2C.ST_GLOBAL4, b_RUSZ_48, Mf_RUSZ_48);
    BitSet3(&DtI2C.ST_GLOBAL4, b_ROZRUCH_AWR, 0);
    BitSet3(&DtI2C.ST_GLOBAL4, b_MODULUJ, Mf_MODULUJ);
    BitSet3(&DtI2C.ST_GLOBAL4, b_OST_STB, 0);
    BitSet3(&DtI2C.ST_GLOBAL4, b_OST_PLM, Mf_OST_PL);
    BitSet3(&DtI2C.ST_GLOBAL4, b_VWYBIEG, Mf_VWYBIEG);
    BitSet3(&DtI2C.ST_GLOBAL4, b_PPOMP, 0);
    BitSet3(&DtI2C.ST_GLOBAL4, b_ZMKCOCW, Mf_ZMKCOCW);
    //TPRZ_CW
    DtI2C.TPRZ_CW = 0;
    //ST0_WSW
    DtI2C.ST0_WSW = 0;
    BitSet3(&DtI2C.ST0_WSW, b_MSG_A1, !M.ERR_BTY && Mf_RUSZ_PLOMIEN);
    BitSet3(&DtI2C.ST0_WSW, b_MSG_A2, !M.ERR_BTY && Mf_RUSZ_STB);
    BitSet3(&DtI2C.ST0_WSW, b_MSG_A3, !M.ERR_BTY && Mf_RUSZ_CIAG && Mf_prb_CIG);
    if(DtKNF.robg == _OTWARTY) BitSet3(&DtI2C.ST0_WSW, b_MSG_A5, !M.ERR_BTY && Mf_RUSZ_POMPE && Mf_prb_PRZ);

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
            BitSet3(&DtI2C.ST0_WSW, b_MSG_A4, 1);
            break;
        case 0x05:
            if(DtKNF.robg == _OTWARTY) DtI2C.ERR_BTY = 5;
            else BitSet3(&DtI2C.ST0_WSW, b_MSG_A5, 1);
            break;
        case 0x06:
            DtI2C.ERR_BTY = 6;
            break;
        case 0x07:
            BitSet3(&DtI2C.ST0_WSW, b_MSG_A7, 1);
            break;
        case 0x08:
            BitSet3(&DtI2C.ST0_WSW, b_MSG_A8, 1);
            break;
        case 0x09:
            BitSet3(&DtI2C.ST0_WSW, b_MSG_A9, 1);
            break;
    }
}
 */
void FormatDataIMY(void)
{
	//Kod sterownika
	//DtI2C.kod=RSDT1.outRDZ0;
	switch(DtKNF.tpkt)		//dla danego rodzaju kotla
	{
		case _UNICO:		//unico - monotermiczny
			if(!Mf_TURBO) DtI2C.kod=0xA0;
			else DtI2C.kod=0xA5;
			break;
		case _BITERM:		//bitermiczny (miniterm)
			if(!Mf_TURBO) DtI2C.kod=0xA2;
			else DtI2C.kod=0xA7;
			break;
		case _BITERM2:		//bitermiczny (maksiterm)
			if(!Mf_TURBO) DtI2C.kod=0xA4;
			else DtI2C.kod=0xA9;
			break;
		case _MONOHB:		//monotermiczny z hydroblokiem
			if(!Mf_TURBO) DtI2C.kod=0xAA;
			else DtI2C.kod=0xAB;
			break;
		default:			//domyslny monotermiczny
			if(!Mf_TURBO) DtI2C.kod=0xAA;
			else DtI2C.kod=0xAB;	//inny domyslny
	}
	//PCO
	if(Mf_AKTYWNE_KNF) DtI2C.PCO=DtCO.PPCO;
	else
	if(Mf_AKTYWNE_PGD) DtI2C.PCO=DtCO.PPCO;
	else DtI2C.PCO=DtCO.PPCO;
	//CO
	DtI2C.CO=DtCO.CO;
	//PCW
	if(Mf_AKTYWNE_KNF) DtI2C.PCW=DtCW.PPCW;
	else
	if(Mf_pob_AL) DtI2C.PCW=DtCW.PPCW;
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
	BitSet3(&DtI2C.ST_AD1,b_ERR_CO,RdPrt(S_ERR_CO));
	BitSet3(&DtI2C.ST_AD1,b_ERR_CW,DtCWf_err_CW);
	BitSet3(&DtI2C.ST_AD1,b_NEW_PCO,DtCOf_newPCO);
	BitSet3(&DtI2C.ST_AD1,b_NEW_PCW,DtCWf_newPCW);
	BitSet3(&DtI2C.ST_AD1,b_TERR_CSN,RdPrt(S_ERR_CSN));
	BitSet3(&DtI2C.ST_AD1,b_TLO_CSN,DtCSf_lo);
	BitSet3(&DtI2C.ST_AD1,b_THI_CSN,DtCSf_hi);
	BitSet3(&DtI2C.ST_AD1,b_TWRK_CSN,RdPrt(S_WRK_CSN));
	//ST_AD2
	BitSet3(&DtI2C.ST_AD2,b_SU_CO,DtCOf_su_CO);
	BitSet3(&DtI2C.ST_AD2,b_AD_CO,RdPrt(S_ADD_CO));
	BitSet3(&DtI2C.ST_AD2,b_ON_DZ,DtCOf_on_DZ_CO);
	BitSet3(&DtI2C.ST_AD2,b_ON_POMP,DtCOf_cold_CO||DtCOf_hot_CO);
	BitSet3(&DtI2C.ST_AD2,b_SU_CW,DtCWf_su_CW);
	BitSet3(&DtI2C.ST_AD2,b_AD_CW,DtCWf_ad_CW);
	BitSet3(&DtI2C.ST_AD2,b_S_DTF,RdPrt(S_DTF));
	BitSet3(&DtI2C.ST_AD2,b_F_DTF,RdPrt(S_DTF));
	//ST_AD3
	BitSet3(&DtI2C.ST_AD3,b_HOT_CO,DtCOf_hot_CO);
	BitSet3(&DtI2C.ST_AD3,b_NO_CSN,DtCSf_no);
	BitSet3(&DtI2C.ST_AD3,b_F_LO_CSN,RdPrt(S_LO_CSN));
	BitSet3(&DtI2C.ST_AD3,b_S_LO_CSN,RdPrt(S_LO_CSN));
	BitSet3(&DtI2C.ST_AD3,b_BLOK_15S,Mf_BLOKUJ_15S);
	//PORTC
	BitSet3(&DtI2C.PORTC,b_PLOMIEN,_PLOMIEN);
	BitSet3(&DtI2C.PORTC,b_ALARM,1);
	BitSet3(&DtI2C.PORTC,b_VNT,_VNT);
	BitSet3(&DtI2C.PORTC,b_PRZEPLYW_CO,_PRZ_CO);
	if(!Mf_TURBO)
		BitSet3(&DtI2C.PORTC,b_BRAK_CIAGU,_CIG_NC);
	else
		BitSet3(&DtI2C.PORTC,b_BRAK_CIAGU,_CIG_NO);
	BitSet3(&DtI2C.PORTC,b_MOD_ON,_MOD_ON);
	//PORTD
	BitSet3(&DtI2C.PORTD,b_DTF,0);
	BitSet3(&DtI2C.PORTD,b_GRZEJ,_GRZEJ);
	BitSet3(&DtI2C.PORTD,b_KCO_KCW,_KCO_CW);
	BitSet3(&DtI2C.PORTD,b_POMPA,_POMPA);
	BitSet3(&DtI2C.PORTD,b_RESET_O,PFNf_res);//0);
	BitSet3(&DtI2C.PORTD,b_SW_CONTROL,!PFNf_off);//PFNf_lato||PFNf_zima);
	BitSet3(&DtI2C.PORTD,b_ZIMA,(!PFNf_off&&PFNf_zima));
	//ST_GLOBAL0
	if(DtKNF.tpkt!=_UNICO)
	{
		BitSet3(&DtI2C.ST_GLOBAL0,b_S_CW,(!Mf_AKTYWNE_SRW&&!Mf_pob_KCO)?RdPrt(S_WCW):0);
	}
	else
	{
		if((Mf_pob_AL||Mf_pob_KCW||Mf_pob_DzCW)&&Mf_PRACA) BitSet3(&DtI2C.ST_GLOBAL0,b_S_CW,1);
		else
		{
			if(!Mf_AKTYWNE_SRW&&!Mf_pob_KCO)
				BitSet3(&DtI2C.ST_GLOBAL0,b_S_CW,(RdPrt(S_WCW)&&DtCWf_wpz_CW)?1:0);
			else
				BitSet3(&DtI2C.ST_GLOBAL0,b_S_CW,0);
		}
	}
	BitSet3(&DtI2C.ST_GLOBAL0,b_S_RT,RdPrt(S_RT));
	BitSet3(&DtI2C.ST_GLOBAL0,b_S_ALARM,1);
	BitSet3(&DtI2C.ST_GLOBAL0,b_S_PRZ_CO,RdPrt(S_PRZ_CO));
	if(!Mf_TURBO)
		BitSet3(&DtI2C.ST_GLOBAL0,b_S_BRAK_CI,!RdPrt(S_CIG_NC));
	else
		BitSet3(&DtI2C.ST_GLOBAL0,b_S_BRAK_CI,!RdPrt(S_CIG_NO));
	BitSet3(&DtI2C.ST_GLOBAL0,b_S_MOD_ON,!RdPrt(S_MOD_ON));
	BitSet3(&DtI2C.ST_GLOBAL0,b_S_RESET,PFNf_res);//RdPrt(S_RESET));
	BitSet3(&DtI2C.ST_GLOBAL0,b_S_PLOMIEN,!RdPrt(S_PLM));
	//ST_GLOBAL2
	BitSet3(&DtI2C.ST_GLOBAL2,b_OPDG,DtKNF.opdg);
	BitSet3(&DtI2C.ST_GLOBAL2,b_GP,DtKNF.rdgz);
	BitSet3(&DtI2C.ST_GLOBAL2,b_OK_WRT,0);
	BitSet3(&DtI2C.ST_GLOBAL2,b_S_ON_DZ,RdPrt(S_DZ_CO));
	BitSet3(&DtI2C.ST_GLOBAL2,b_F_ON_DZ,RdPrt(S_DZ_CO));
	BitSet3(&DtI2C.ST_GLOBAL2,b_S_SWC,!PFNf_off);//PFNf_lato||PFNf_zima);
	BitSet3(&DtI2C.ST_GLOBAL2,b_F_SWC,!PFNf_off);//PFNf_lato||PFNf_zima);
	BitSet3(&DtI2C.ST_GLOBAL2,b_S_STB,!RdPrt(S_STB));
	//ST_GLOBAL3
	BitSet3(&DtI2C.ST_GLOBAL3,b_ROZRUCH,Mf_ROZRUCH);
	BitSet3(&DtI2C.ST_GLOBAL3,b_WYLACZ,Mf_WYLACZ);
	BitSet3(&DtI2C.ST_GLOBAL3,b_RUSZ_CIAG,Mf_RUSZ_CIAG);
	BitSet3(&DtI2C.ST_GLOBAL3,b_RUSZ_POMPE,Mf_RUSZ_POMPE);
	BitSet3(&DtI2C.ST_GLOBAL3,b_CZEKAJ_NTC,Mf_RUSZ_NTC_CO);
	BitSet3(&DtI2C.ST_GLOBAL3,b_RUSZ_PLOMIEN,Mf_RUSZ_PLOMIEN);
	BitSet3(&DtI2C.ST_GLOBAL3,b_RUSZ_STB,Mf_RUSZ_STB);
	BitSet3(&DtI2C.ST_GLOBAL3,b_BLOKUJ_3M,Mf_BLOKUJ_3M);
	//ST_GLOBAL4
	BitSet3(&DtI2C.ST_GLOBAL4,b_RUSZ_48,Mf_RUSZ_48);
	BitSet3(&DtI2C.ST_GLOBAL4,b_ROZRUCH_AWR,0);
	BitSet3(&DtI2C.ST_GLOBAL4,b_MODULUJ,Mf_MODULUJ);
	BitSet3(&DtI2C.ST_GLOBAL4,b_OST_STB,0);
	BitSet3(&DtI2C.ST_GLOBAL4,b_OST_PLM,Mf_OST_PL);
	BitSet3(&DtI2C.ST_GLOBAL4,b_VWYBIEG,Mf_VWYBIEG);
	BitSet3(&DtI2C.ST_GLOBAL4,b_PPOMP,0);
	BitSet3(&DtI2C.ST_GLOBAL4,b_ZMKCOCW,Mf_ZMKCOCW);
	//TPRZ_CW
	DtI2C.TPRZ_CW=0;
	//ST0_WSW
	DtI2C.ST0_WSW=0;
	BitSet3(&DtI2C.ST0_WSW,b_MSG_A1,!M.ERR_BTY&&Mf_RUSZ_PLOMIEN);
	BitSet3(&DtI2C.ST0_WSW,b_MSG_A2,!M.ERR_BTY&&Mf_RUSZ_STB);
	BitSet3(&DtI2C.ST0_WSW,b_MSG_A3,!M.ERR_BTY&&Mf_RUSZ_CIAG&&Mf_prb_CIG);
	if(DtKNF.robg==_OTWARTY) BitSet3(&DtI2C.ST0_WSW,b_MSG_A5,!M.ERR_BTY&&Mf_RUSZ_POMPE&&Mf_prb_PRZ);

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
			BitSet3(&DtI2C.ST0_WSW,b_MSG_A4,1);
		break;
		case 0x05:
			if(DtKNF.robg==_OTWARTY) DtI2C.ERR_BTY=5;
			else BitSet3(&DtI2C.ST0_WSW,b_MSG_A5,1);
		break;
		case 0x06:
			DtI2C.ERR_BTY=6;
		break;
		case 0x07:
			BitSet3(&DtI2C.ST0_WSW,b_MSG_A7,1);
		break;
		case 0x08:
			BitSet3(&DtI2C.ST0_WSW,b_MSG_A8,1);
		break;
		case 0x09:
			BitSet3(&DtI2C.ST0_WSW,b_MSG_A9,1);
		break;
	}
}
//-------------------------------------------------------------------------------
