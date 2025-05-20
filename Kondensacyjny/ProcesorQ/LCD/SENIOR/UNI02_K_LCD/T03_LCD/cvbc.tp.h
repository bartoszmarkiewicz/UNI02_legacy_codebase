/*cvbc.tp.h*/
//----------------------------------------------------------
#ifndef	_CVBC_TPH
#define	_CVBC_TPH
//----------------------------------------------------------
typedef struct 	{
				unsigned TRANS_DATA:1;
				//------------------------
				unsigned SetPCW:1;
				unsigned GetPCW:1;
				unsigned GetSTAT:1;
				unsigned GetERR:1;
				unsigned GetTCW:1;
				unsigned int HPCW;
				unsigned int HERR;
				unsigned int HTCW;
				unsigned int HSTAT;
	
	
				//--------------------------------
				unsigned NO_CVBC:1;
				unsigned char HNL;
				unsigned char SEP;
				unsigned char licznik;
				unsigned int zeton;
				unsigned int active;
				unsigned char timRS;
				unsigned char timRS2;
				//--------------------------------
				unsigned char HMSG;
				unsigned int HDID;
				unsigned int HDTA;	

				unsigned HErr_ERR:1;
				unsigned HErr_CHK:1;
				unsigned HErr_DID:1;
				unsigned HErr_MSG:1;
				unsigned HErr_COM:1;
				unsigned HRead_OK:1;
				unsigned HWrite_OK:1;
				//--------------------------------
				//STATUS (high byte)
				unsigned H_CH_Mode:1;			//0
				unsigned H_DHW_Mode:1;
				unsigned H_TEST_Mode:1;
				unsigned H_FLAME_Status:1;
				unsigned H_EXH_Status:1;
				unsigned H_WATER_FL_Status:1;
				unsigned H_CALIBRATION_Start:1;
				unsigned H_CALIBRATION_Min:1;	//7
				//STATUS (low byte)
				unsigned H_FAULT_Indication:1;	//0
				unsigned H_VALVE1_Status:1;
				unsigned H_VALVE2_Status:1;
				unsigned H_APS_Status:1;
				unsigned H_FAN_Status:1;
				unsigned H_CH_PUMP_Status:1;
				unsigned H_DHW_PUMP_Status:1;
				unsigned H_FROST_Mode:1;		//7
				}tpCVBC;
//----------------------------------------------------------
#endif /*_CVBC_TPH*/