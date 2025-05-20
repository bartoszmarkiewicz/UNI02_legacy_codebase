#include <xc.h>
#include "../../proj.tp.h"

#ifndef _TYP_PRJ
#error NIEOKRESLONY IDENTYFIKATOR PROJEKTU
#endif

#if _TYP_PRJ==2
#advisory _TYP_PRJ==2 
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
extern DataIN DtIN;
extern DtPMP PMP;
extern DataPFN PFN;
extern DataCSN DtCS;
extern DtPWM PWM;
extern DtKONF DtKNF;

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//--------------------------KOTLY ATMOSFERYCZNE----------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//Zapis danych przeznaczonych do wysylki do bufora
void inline SetTData(void)
{
    volatile static unsigned char dt;
    
    if(dt<99) dt++;
    else dt=0;
        
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
static void inline InBitSet(volatile unsigned char *data, const unsigned char nbit,const unsigned char bt)
{
	*data=(bt)?(*data|(1<<nbit)):(*data&(~(1<<nbit)));
}
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//Formatowanie danych na potrzeby modulu IMY-02
void inline FormatDataIMY(void)
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
	InBitSet(&DtI2C.ST_AD1,b_ERR_CO,InRdPrt(S_ERR_CO));
	InBitSet(&DtI2C.ST_AD1,b_ERR_CW,InRdPrt(S_ERR_CW));
	InBitSet(&DtI2C.ST_AD1,b_NEW_PCO,DtCO._newPCO);
	InBitSet(&DtI2C.ST_AD1,b_NEW_PCW,DtCW._newPCW);
	InBitSet(&DtI2C.ST_AD1,b_TERR_CSN,InRdPrt(S_ERR_CSN));
	InBitSet(&DtI2C.ST_AD1,b_TLO_CSN,DtCS._lo);
	InBitSet(&DtI2C.ST_AD1,b_THI_CSN,DtCS._hi);
	InBitSet(&DtI2C.ST_AD1,b_TWRK_CSN,InRdPrt(S_WRK_CSN));
	//ST_AD2
	InBitSet(&DtI2C.ST_AD2,b_SU_CO,DtCO._su_CO);
	InBitSet(&DtI2C.ST_AD2,b_AD_CO,InRdPrt(S_ADD_CO));
	InBitSet(&DtI2C.ST_AD2,b_ON_DZ,DtCO._on_DZ_CO);
	InBitSet(&DtI2C.ST_AD2,b_ON_POMP,DtCO._cold_CO||DtCO._hot_CO);
	InBitSet(&DtI2C.ST_AD2,b_SU_CW,DtCW._su_CW);
	InBitSet(&DtI2C.ST_AD2,b_AD_CW,DtCW._ad_CW);
	InBitSet(&DtI2C.ST_AD2,b_S_DTF,0);
    
    if(M._KEY7)         //konsola 7 przyciskow?
    {
        InBitSet(&DtI2C.ST_AD2,b_F_DTF,1);
        if(M._KKOM)     //nacisnieto przycisk specjalny?
        {
            InBitSet(&DtI2C.ST_AD3,b_F_LO_CSN,1);    
        }
        else
        {
            InBitSet(&DtI2C.ST_AD3,b_F_LO_CSN,0);
        }
    }
    else
    {
        InBitSet(&DtI2C.ST_AD2,b_F_DTF,0);  
        InBitSet(&DtI2C.ST_AD3,b_F_LO_CSN,InRdPrt(S_LO_CSN));        
    }
	//ST_AD3
	InBitSet(&DtI2C.ST_AD3,b_HOT_CO,DtCO._hot_CO);
	InBitSet(&DtI2C.ST_AD3,b_NO_CSN,DtCS._no);
	InBitSet(&DtI2C.ST_AD3,b_S_LO_CSN,InRdPrt(S_LO_CSN));
	InBitSet(&DtI2C.ST_AD3,b_BLOK_15S,M._BLOKUJ_15S);
	//PORTC
	InBitSet(&DtI2C.PORTC,b_PLOMIEN, _PLOMIEN_GetValue());
	InBitSet(&DtI2C.PORTC,b_ALARM,1);
	InBitSet(&DtI2C.PORTC,b_VNT, _VNT_GetValue());
	InBitSet(&DtI2C.PORTC,b_PRZEPLYW_CO, _PRZ_CO_GetValue());
	if(!M._TURBO)
		InBitSet(&DtI2C.PORTC,b_BRAK_CIAGU, _CIG_NC_GetValue());
	else
		InBitSet(&DtI2C.PORTC,b_BRAK_CIAGU, _CIG_NO_GetValue());
	InBitSet(&DtI2C.PORTC,b_MOD_ON, _MOD_ON_GetValue());	
	//PORTD
    switch(M.STRB)  //tryb produkcyjny
    {
        case 0:
            InBitSet(&DtI2C.PORTD,b_DTF,0);
            break;
        case 1:
            InBitSet(&DtI2C.PORTD,b_DTF,1);
            break; 
        case 2:
            InBitSet(&DtI2C.PORTD,b_DTF,0);
            break;  
        default:
            InBitSet(&DtI2C.PORTD,b_DTF,1);
            break;            
    }

	InBitSet(&DtI2C.PORTD,b_GRZEJ, _GRZEJ_GetValue());
	InBitSet(&DtI2C.PORTD,b_KCO_KCW,_KCO_CW_GetValue());
	InBitSet(&DtI2C.PORTD,b_POMPA,_POMPA_GetValue());		
	InBitSet(&DtI2C.PORTD,b_RESET_O,PFN._res);
	InBitSet(&DtI2C.PORTD,b_SW_CONTROL,PFN._off);
	InBitSet(&DtI2C.PORTD,b_ZIMA,PFN._zima);
	//ST_GLOBAL0
	if(DtKNF.tpkt!=_UNICO)
	{
        InBitSet(&DtI2C.ST_GLOBAL0,b_S_CW,(!M._AKTYWNE_SRW&&!M._pob_KCO)?InRdPrt(S_WCW):0);
	}
	else
	{
		if((M._pob_AL||M._pob_KCW||M._pob_DzCW)&&M._PRACA) InBitSet(&DtI2C.ST_GLOBAL0,b_S_CW,1);
		else
		{
			if(!M._AKTYWNE_SRW&&!M._pob_KCO)
            {
                if(!DtKNF.cwtm)     //(timera zasobnika||LIN_CW||OT_CW)
                {
                    InBitSet(&DtI2C.ST_GLOBAL0,b_S_CW,(InRdPrt(S_WCW)&&DtCW._wpz_CW)?1:0);
                }
                else                //timera zasobnika
                {
                    InBitSet(&DtI2C.ST_GLOBAL0,b_S_CW,(InRdPrt(S_TIM)&&DtCW._wpz_CW)?1:0);                    
                }
            }
			else
            {
				InBitSet(&DtI2C.ST_GLOBAL0,b_S_CW,0);
            }
		}
	}
	InBitSet(&DtI2C.ST_GLOBAL0,b_S_RT,InRdPrt(S_RT));
	InBitSet(&DtI2C.ST_GLOBAL0,b_S_ALARM,1);
	InBitSet(&DtI2C.ST_GLOBAL0,b_S_PRZ_CO,InRdPrt(S_PRZ_CO));
	if(!M._TURBO)		
		InBitSet(&DtI2C.ST_GLOBAL0,b_S_BRAK_CI,!InRdPrt(S_CIG_NC));
	else
		InBitSet(&DtI2C.ST_GLOBAL0,b_S_BRAK_CI,!InRdPrt(S_CIG_NO));
	InBitSet(&DtI2C.ST_GLOBAL0,b_S_MOD_ON,!InRdPrt(S_MOD_ON));
	InBitSet(&DtI2C.ST_GLOBAL0,b_S_RESET,PFN._res);//InRdPrt(S_RESET));
	InBitSet(&DtI2C.ST_GLOBAL0,b_S_PLOMIEN,!InRdPrt(S_PLM));
	//ST_GLOBAL2	
	InBitSet(&DtI2C.ST_GLOBAL2,b_OPDG,DtKNF.opdg);
	InBitSet(&DtI2C.ST_GLOBAL2,b_GP,DtKNF.rdgz);
    switch(M.STRB)      //tryb produkcyjny
    {
        case 0:
            InBitSet(&DtI2C.ST_GLOBAL2,b_OK_WRT,0);
            break;
        case 1:
            InBitSet(&DtI2C.ST_GLOBAL2,b_OK_WRT,0);
            break; 
        case 2:
            InBitSet(&DtI2C.ST_GLOBAL2,b_OK_WRT,1);
            break;  
        default:
            InBitSet(&DtI2C.ST_GLOBAL2,b_OK_WRT,1);
            break;            
    }
	InBitSet(&DtI2C.ST_GLOBAL2,b_S_ON_DZ,InRdPrt(S_DZ_CO));
	InBitSet(&DtI2C.ST_GLOBAL2,b_F_ON_DZ,InRdPrt(S_DZ_CO));	
	InBitSet(&DtI2C.ST_GLOBAL2,b_S_SWC,PFN._lato||PFN._zima);
	InBitSet(&DtI2C.ST_GLOBAL2,b_F_SWC,PFN._lato||PFN._zima);
	InBitSet(&DtI2C.ST_GLOBAL2,b_S_STB,!InRdPrt(S_STB));
	//ST_GLOBAL3
	InBitSet(&DtI2C.ST_GLOBAL3,b_ROZRUCH,M._ROZRUCH);
	InBitSet(&DtI2C.ST_GLOBAL3,b_WYLACZ,M._WYLACZ);
	InBitSet(&DtI2C.ST_GLOBAL3,b_RUSZ_CIAG,M._RUSZ_CIAG);
	InBitSet(&DtI2C.ST_GLOBAL3,b_RUSZ_POMPE,M._RUSZ_POMPE);
	InBitSet(&DtI2C.ST_GLOBAL3,b_CZEKAJ_NTC,M._RUSZ_NTC_CO);	
	InBitSet(&DtI2C.ST_GLOBAL3,b_RUSZ_PLOMIEN,M._RUSZ_PLOMIEN);
	InBitSet(&DtI2C.ST_GLOBAL3,b_RUSZ_STB,M._RUSZ_STB);
	InBitSet(&DtI2C.ST_GLOBAL3,b_BLOKUJ_3M,M._BLOKUJ_3M);
	//ST_GLOBAL4
	InBitSet(&DtI2C.ST_GLOBAL4,b_RUSZ_48,M._RUSZ_48);
	InBitSet(&DtI2C.ST_GLOBAL4,b_ROZRUCH_AWR,0);
	InBitSet(&DtI2C.ST_GLOBAL4,b_MODULUJ,M._MODULUJ);
	InBitSet(&DtI2C.ST_GLOBAL4,b_OST_STB,0);
	InBitSet(&DtI2C.ST_GLOBAL4,b_OST_PLM,M._OST_PL);	
	InBitSet(&DtI2C.ST_GLOBAL4,b_VWYBIEG,M._VWYBIEG);
    if(DtKNF.rdpm)
    {
        InBitSet(&DtI2C.ST_GLOBAL4,b_PPOMP,1);
    }
    else
    {
        InBitSet(&DtI2C.ST_GLOBAL4,b_PPOMP,0);
    }
	InBitSet(&DtI2C.ST_GLOBAL4,b_ZMKCOCW,M._ZMKCOCW);
	//TPRZ_CW
	DtI2C.TPRZ_CW=0;
	//ST0_WSW
	DtI2C.ST0_WSW=0;
	InBitSet(&DtI2C.ST0_WSW,b_MSG_A1,!M.ERR_BTY&&M._RUSZ_PLOMIEN);
	InBitSet(&DtI2C.ST0_WSW,b_MSG_A2,!M.ERR_BTY&&M._RUSZ_STB);
	InBitSet(&DtI2C.ST0_WSW,b_MSG_A3,!M.ERR_BTY&&M._RUSZ_CIAG&&M._prb_CIG);
	if(DtKNF.robg==_OTWARTY) InBitSet(&DtI2C.ST0_WSW,b_MSG_A5,!M.ERR_BTY&&M._RUSZ_POMPE&&M._prb_PRZ);

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
			InBitSet(&DtI2C.ST0_WSW,b_MSG_A4,1);
		break;
		case 0x05:
			if(DtKNF.robg==_OTWARTY) DtI2C.ERR_BTY=5;
			else InBitSet(&DtI2C.ST0_WSW,b_MSG_A5,1);
		break;
		case 0x06:
			DtI2C.ERR_BTY=6;
		break;
		case 0x07:
			InBitSet(&DtI2C.ST0_WSW,b_MSG_A7,1);
		break;
		case 0x08:
			InBitSet(&DtI2C.ST0_WSW,b_MSG_A8,1);
		break;
		case 0x09:
			InBitSet(&DtI2C.ST0_WSW,b_MSG_A9,1);
		break;
	}	
}
//-------------------------------------------------------------------------------
#endif