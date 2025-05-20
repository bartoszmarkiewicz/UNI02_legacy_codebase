/*rgl.c*/
/*---------------------------------------------------------------------------------*/
#include <pic18.h>
#include "global.h"				//parametry globalne
#include "nrgl.h"				//parametry lokalne
#include "pomiar.tp.h"				//parametry lokalne
#include "pwm.h"				//parametry lokalne
#include "main.h"				//parametry lokalne

extern tpMAIN M;
extern DataCO DtCO;
extern DataCW DtCW;
extern DataOPT DtOPT;
extern DtPWM PWM;
DtRgl rgl;

//wartosci PWM dla roznych rodzajow gazu, przy odpowiednich stopniach 
//otwarcia zaworu gazowego
const unsigned char pbd1[2][6]={	
								//gaz ziemny
								MS_1200,	//85%
								MS_1000,	//70%
								MS_800, 	//60%
								MS_600, 	//40%
								MS_400, 	//30%
								MS_250, 	//20%
								//gaz plynny
								MS_P2500,	//85%
								MS_P2100,	//70%
								MS_P1700,	//60%
								MS_P1200,	//40%
								MS_P900,	//30%
								MS_P600		//20%
								};
//wartosci PWM dla roznych rodzajow gazu, przy modulacji prop od 
//temp. CO przy grzaniu CW								
const unsigned char prm1[2][3]={
								//gaz ziemny
								_MG_MOD-1*((_MG_MOD-_MD_MOD)/7),
								_MG_MOD-2*((_MG_MOD-_MD_MOD)/7),
								_MG_MOD-3*((_MG_MOD-_MD_MOD)/7),
								//gaz plynny
								_MG_PMOD-1*((_MG_PMOD-_MD_PMOD)/7),
								_MG_PMOD-2*((_MG_PMOD-_MD_PMOD)/7),
								_MG_PMOD-3*((_MG_PMOD-_MD_PMOD)/7)
								};
//------------------------------------------------------------
//------------------------------------------------------------
//rozpoczêcie pracy w obiegu CO
void RozpoczeciePracyCO(void)
{
	switch(M.TPSTR)
	{
		case _INWEST:
			StartCO();
			break;	
		case _MMAXP:
			StartCO2();
			break;
		case _UNICO:
			StartCO1F();
			break	
	}
}
void OdczytDanychCO(void)
{
	rgl.x_t=DtCO.DCO;
	rgl.x_dt=DtCO.dDCO;	
}
void DecyzjaMiedzyfazowaCO()
{
	//warunek przejscia do FAZA2
	if(!rgl.fz)						//aktualnie faza 1?
	if((!Tnizsza(5)&&Tmaleje(0))||(!Tnizsza(2)&&Tstoi())) 		//warunek przejscia do fazy 2
	{
		rgl.fz=1;					//ustaw znacznik FAZA2
		rgl.srp=PWM.BufPWM;
		rgl.xmod=PWM.BufPWM;
		if(rgl.xmod>PWM.MGMOD)
		{
			rgl.srp=PWM.MGMOD;
			rgl.xmod=PWM.MGMOD;
		}
		return;
	}
	//warunek przejscia do FAZA1
	if(rgl.fz)						//aktualnie faza 2?
	{
		if(Tnizsza(10))				//warunek przejscia do fazy 1				
		{
			rgl.fz=0;				//ustaw znacznik FAZA1
			rgl.xmod=PWM.BufPWM;
			rgl.srp=0;
			rgl.ytmp=0;	
		}
	}	
}
void ObiegCO(void)
{
	RozpoczeciePracyCO();
	OdczytDanychCO();
	DecyzjaMiedzyfazowaCO();
	if(!rgl.fz) FazaDochodzeniaCO();
	else FazaStabilizacjiCO();
	ReakcjaSuperwizyjnaCO();
	KontrolaOpenT();
}

