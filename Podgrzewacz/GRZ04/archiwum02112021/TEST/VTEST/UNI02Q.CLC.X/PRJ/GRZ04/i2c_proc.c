#include <xc.h>
#include "../../proj.tp.h"
 
#ifndef _TYP_PRJ
#error NIEOKRESLONY IDENTYFIKATOR PROJEKTU
#endif

#if _TYP_PRJ==4
#advisory _TYP_PRJ==4
#include "../../mcc_generated_files\mcc.h"

#include "global.h"
#include "main.h"
#include "global.ex.h"

#include "dtimy.h"

extern volatile DataI2C DtI2C;
extern volatile bfI2C BufI2C;
extern tpMAIN M;
extern DataCO DtCO;
extern DataCW DtCW;
extern DataZW DtZW;
extern DataPFN PFN;
extern DataCSN DtCS;
extern DtPWM PWM;
extern DtKONF DtKNF;

extern unsigned char pw1,pw2;
extern unsigned char xxy;
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//--------------------------PODGRZEWACZ GRZ04----------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//Zapis danych przeznaczonych do wysylki do bufora
void inline SetTData(void)
{
    volatile static unsigned char dt;
    
    if(dt<99) dt++;
    else dt=0;
    
	BufI2C.Bf[0]=DtI2C.kod;
	BufI2C.Bf[1]=pw2;//DtI2C.PCO;//M.LICZNIK_CIG3;//DtI2C.PCO;//M.LICZNIK_CIG3;
	BufI2C.Bf[2]=pw1;//DtI2C.CO;//M.LICZNIK_CIG;//DtI2C.CO;//M.LICZNIK_CIG;
	BufI2C.Bf[3]=0;//dt;//DtI2C.PCW;//RTS(_RTRSCI2)/10;//DtI2C.PCW;//RTS(_RTRSCI2)/10;
	BufI2C.Bf[4]=DtI2C.CW;//RTS(_RTRSCI)/10;//DtI2C.CW;//RTS(_RTRSCI)/10;
    if(M._ROZRUCH)
    {
        BufI2C.Bf[5]=10;    
    }
    else
    {
        BufI2C.Bf[5]=0;
    }
    /*BufI2C.Bf[1]=CLCDATAbits.MLC4OUT;//M.LICZNIK_CIG3;//DtI2C.PCO;//M.LICZNIK_CIG3;
	BufI2C.Bf[2]=CLCDATAbits.MLC3OUT;//M.LICZNIK_CIG;//DtI2C.CO;//M.LICZNIK_CIG;
	BufI2C.Bf[3]=CLCDATAbits.MLC2OUT;;//CLCDATAbits.MCL1OUT;//RTS(_RTRSCI2)/10;//DtI2C.PCW;//RTS(_RTRSCI2)/10;
	BufI2C.Bf[4]=0;//CLCDATAbits.MLC7OUT;//RTS(_RTRSCI)/10;//DtI2C.CW;//RTS(_RTRSCI)/10;
	BufI2C.Bf[5]=RD1_GetValue();//CLCDATAbits.MLC8OUT;//M._ZAPLON;//prpwm;////DtCW.ddDCW;//DtI2C.CS;    
    */
    /*
    BufI2C.Bf[1]=DtI2C.PCO;//M.LICZNIK_CIG3;//DtI2C.PCO;//M.LICZNIK_CIG3;
	BufI2C.Bf[2]=DtI2C.CO;//M.LICZNIK_CIG;//DtI2C.CO;//M.LICZNIK_CIG;
	BufI2C.Bf[3]=DtI2C.PCW;//RTS(_RTRSCI2)/10;//DtI2C.PCW;//RTS(_RTRSCI2)/10;
	BufI2C.Bf[4]=DtI2C.CW;//RTS(_RTRSCI)/10;//DtI2C.CW;//RTS(_RTRSCI)/10;
	BufI2C.Bf[5]=DtI2C.CS;//M._ZAPLON;//prpwm;////DtCW.ddDCW;//DtI2C.CS;
    */
    /*BufI2C.Bf[1]=InRdPrt(S_CIG2);//DtI2C.PCO;//M.LICZNIK_CIG3;//DtI2C.PCO;//M.LICZNIK_CIG3;
	BufI2C.Bf[2]=InRdPrt(S_CIG_NO);//DtI2C.CO;//M.LICZNIK_CIG;//DtI2C.CO;//M.LICZNIK_CIG;
	BufI2C.Bf[3]=DtI2C.PCW;//RTS(_RTRSCI2)/10;//DtI2C.PCW;//RTS(_RTRSCI2)/10;
	BufI2C.Bf[4]=DtI2C.CW;//RTS(_RTRSCI)/10;//DtI2C.CW;//RTS(_RTRSCI)/10;
	BufI2C.Bf[5]=_VNT_GetValue();//DtI2C.CS;//M._ZAPLON;//prpwm;////DtCW.ddDCW;//DtI2C.CS;
    */
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
	BufI2C.Bf[18]=DtI2C.VVNT;
	BufI2C.Bf[19]=DtI2C.ST0_WSW;
	BufI2C.Bf[20]=DtI2C.ERR_BTY;
}
//-------------------------------------------------------------------------------
//Operacje wykonywane po odebraniu danych
void inline ToWork(void)
{

}
//-------------------------------------------------------------------------------
//Przygotowanie na odbior danych
void inline SetRData(void)
{
	BufI2C.indeks=0;
}
//-------------------------------------------------------------------------------
static inline void InBitSet(volatile unsigned char *data, const unsigned char nbit,const unsigned char bt)
{
	*data=(bt)?(*data|(1<<nbit)):(*data&(~(1<<nbit)));
}
//-------------------------------------------------------------------------------
//Formatowanie danych na potrzeby modulu IMY-02
void inline FormatDataIMY(void)
{
    //!UWAGA: W MIEJSCU POMIARU M.VVNT UMIESCIC PONIZSZY WPIS
	//if(M.VVNT) M.I2CVNT=(unsigned char)((float)M.VVNT*0.7+5.0/7);
	//else M.I2CVNT=0;
    
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
    //VVNT
    DtI2C.VVNT=M.I2CVNT;
	//Moc startowa
	DtI2C.STR_PWM=PWM.MSTART;
	//Moc maksymalna
	DtI2C.MAX_PWM=PWM.MMAXCO;
	//Wypelnienia modulatora
	DtI2C.BUF_PWM=PWM.BufPWM;

    //ST_AD1
    InBitSet(&DtI2C.ST_AD1, b_ERR_CO,0);
    InBitSet(&DtI2C.ST_AD1, b_ERR_CW, InRdPrt(S_ERR_CW));
    InBitSet(&DtI2C.ST_AD1, b_NEW_PCO,0);
    InBitSet(&DtI2C.ST_AD1, b_NEW_PCW, DtCW._newPCW);
    InBitSet(&DtI2C.ST_AD1, b_TERR_CSN,0);
    InBitSet(&DtI2C.ST_AD1, b_TLO_CSN, 0);
    InBitSet(&DtI2C.ST_AD1, b_THI_CSN, 0);
    InBitSet(&DtI2C.ST_AD1, b_TWRK_CSN,1);
    //ST_AD2
    InBitSet(&DtI2C.ST_AD2, b_SU_CO, 0);
    InBitSet(&DtI2C.ST_AD2, b_AD_CO, 0);
    InBitSet(&DtI2C.ST_AD2, b_ON_DZ, 0);
    InBitSet(&DtI2C.ST_AD2, b_ON_POMP,0);
    InBitSet(&DtI2C.ST_AD2, b_SU_CW, DtCW._su_CW);
    InBitSet(&DtI2C.ST_AD2, b_AD_CW, DtCW._ad_CW);
    InBitSet(&DtI2C.ST_AD2, b_S_DTF, 0);
    InBitSet(&DtI2C.ST_AD2, b_F_DTF, 0);
    //ST_AD3
    InBitSet(&DtI2C.ST_AD3, b_HOT_CO,0);
    InBitSet(&DtI2C.ST_AD3, b_NO_CSN,0);
    InBitSet(&DtI2C.ST_AD3, b_F_LO_CSN,0);
    InBitSet(&DtI2C.ST_AD3, b_S_LO_CSN,0);
    InBitSet(&DtI2C.ST_AD3, b_BLOK_15S,0);
    //PORTC
    InBitSet(&DtI2C.PORTC, b_PLOMIEN, _PLOMIEN_GetValue());
    InBitSet(&DtI2C.PORTC, b_ALARM, 1);
    InBitSet(&DtI2C.PORTC, b_VNT, _VNT_GetValue());
    InBitSet(&DtI2C.PORTC, b_PRZEPLYW_CO,0);
    if(!M._TURBO)
        InBitSet(&DtI2C.PORTC, b_BRAK_CIAGU, _CIG_NC_GetValue());
    else
        InBitSet(&DtI2C.PORTC, b_BRAK_CIAGU, _CIG_NO_GetValue());
    InBitSet(&DtI2C.PORTC, b_MOD_ON, _MOD_ON_GetValue());
    //PORTD
    InBitSet(&DtI2C.PORTD, b_DTF, 0);
    InBitSet(&DtI2C.PORTD, b_GRZEJ, _GRZEJ_GetValue());
    InBitSet(&DtI2C.PORTD, b_KCO_KCW,0);
    InBitSet(&DtI2C.PORTD, b_POMPA,0);
    InBitSet(&DtI2C.PORTD, b_RESET_O,0);
    InBitSet(&DtI2C.PORTD, b_SW_CONTROL,1);
    InBitSet(&DtI2C.PORTD, b_ZIMA,0);
    //ST_GLOBAL0
	InBitSet(&DtI2C.ST_GLOBAL0,b_S_CW,InRdPrt(S_WCW));
    InBitSet(&DtI2C.ST_GLOBAL0, b_S_RT,0);
    InBitSet(&DtI2C.ST_GLOBAL0, b_S_ALARM, 1);
    InBitSet(&DtI2C.ST_GLOBAL0, b_S_PRZ_CO,0);
    if(!M._TURBO)
        InBitSet(&DtI2C.ST_GLOBAL0, b_S_BRAK_CI, !InRdPrt(S_CIG_NC));
    else
        InBitSet(&DtI2C.ST_GLOBAL0, b_S_BRAK_CI, !InRdPrt(S_CIG_NO));
    InBitSet(&DtI2C.ST_GLOBAL0, b_S_MOD_ON, !InRdPrt(S_MOD_ON));
    InBitSet(&DtI2C.ST_GLOBAL0, b_S_RESET,0); 
    InBitSet(&DtI2C.ST_GLOBAL0, b_S_PLOMIEN, !InRdPrt(S_PLM));
    //ST_GLOBAL2
	InBitSet(&DtI2C.ST_GLOBAL2,b_OPDG,0);
	InBitSet(&DtI2C.ST_GLOBAL2,b_GP,DtKNF.rdgz);
	InBitSet(&DtI2C.ST_GLOBAL2,b_OK_WRT,0);
	InBitSet(&DtI2C.ST_GLOBAL2,b_S_ON_DZ,0);
	InBitSet(&DtI2C.ST_GLOBAL2,b_F_ON_DZ,0);	
	InBitSet(&DtI2C.ST_GLOBAL2,b_S_SWC,1);
	InBitSet(&DtI2C.ST_GLOBAL2,b_F_SWC,1);
	InBitSet(&DtI2C.ST_GLOBAL2,b_S_STB,!InRdPrt(S_STB));
    //ST_GLOBAL3
 	InBitSet(&DtI2C.ST_GLOBAL3,b_ROZRUCH,M._ROZRUCH);
	InBitSet(&DtI2C.ST_GLOBAL3,b_WYLACZ,0);
	InBitSet(&DtI2C.ST_GLOBAL3,b_RUSZ_CIAG,M._RUSZ_CIAG);
	InBitSet(&DtI2C.ST_GLOBAL3,b_RUSZ_POMPE,0);
	InBitSet(&DtI2C.ST_GLOBAL3,b_CZEKAJ_NTC,0);	
	InBitSet(&DtI2C.ST_GLOBAL3,b_RUSZ_PLOMIEN,M._RUSZ_PLOMIEN);
	InBitSet(&DtI2C.ST_GLOBAL3,b_RUSZ_STB,M._RUSZ_STB);
	InBitSet(&DtI2C.ST_GLOBAL3,b_BLOKUJ_3M,0);
    //ST_GLOBAL4
	InBitSet(&DtI2C.ST_GLOBAL4,b_RUSZ_48,0);
	InBitSet(&DtI2C.ST_GLOBAL4,b_ROZRUCH_AWR,0);
	InBitSet(&DtI2C.ST_GLOBAL4,b_MODULUJ,M._MODULUJ);
	InBitSet(&DtI2C.ST_GLOBAL4,b_OST_STB,0);
	InBitSet(&DtI2C.ST_GLOBAL4,b_OST_PLM,M._OST_PL);	
	InBitSet(&DtI2C.ST_GLOBAL4,b_VWYBIEG,M._VWYBIEG);
	InBitSet(&DtI2C.ST_GLOBAL4,b_PPOMP,0);
	InBitSet(&DtI2C.ST_GLOBAL4,b_ZMKCOCW,0);
 
	//TPRZ_CW
	DtI2C.TPRZ_CW=0;
	//ST0_WSW
	DtI2C.ST0_WSW=0;
	InBitSet(&DtI2C.ST0_WSW,b_MSG_A1,!M.ERR_BTY&&M._RUSZ_PLOMIEN);
	InBitSet(&DtI2C.ST0_WSW,b_MSG_A2,!M.ERR_BTY&&M._RUSZ_STB);
	InBitSet(&DtI2C.ST0_WSW,b_MSG_A3,!M.ERR_BTY&&M._RUSZ_CIAG&&M._prb_CIG);
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
			InBitSet(&DtI2C.ST0_WSW,b_MSG_A4,1);
		break;
		case 0x05:
			InBitSet(&DtI2C.ST0_WSW,b_MSG_A5,1);
		break;
        case 0x06:
            DtI2C.ERR_BTY = 6;
            break;
        case 0x07:
            InBitSet(&DtI2C.ST0_WSW, b_MSG_A7, 1);
            break;
        case 0x08:
            InBitSet(&DtI2C.ST0_WSW, b_MSG_A8, 1);
            break;
        case 0x09:
            InBitSet(&DtI2C.ST0_WSW, b_MSG_A9, 1);
            break;
    }
}
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
#endif