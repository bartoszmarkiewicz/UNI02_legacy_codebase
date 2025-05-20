/*otInterpreter.c*/
//Interpretacja odebranej ramki OT
/*---------------------------------------------------------------------------------*/
//#include <pic18.h>
//#include <htc.h>
#include <xc.h>
#include "global.h"			//parametry globalne
#include "main.tp.h"			//parametry globalne
#include "otInterpreter.h"				//parametry lokalne

extern volatile tpOTDta OT;
volatile tpOTMaster OTMaster;
extern volatile DataPFN PFN;
extern volatile tpRSDTA1 RSDT1;
extern volatile unsigned int db;
extern volatile bit MGSDf_NO_ODB;

volatile unsigned char xxy,zzz,vvv;
#if (_EXT_BOARD>=3)
/*---------------------------------------------------------------------------------*/
//Interpretacja odebranej ramki, 
void OTInterpreter(void)
{
#if _TEST_IOT==0
    if(OT.ToACK)                                     //odebrano paczke danych, przejdz do interpretacji   
    {
        //if(MGSDf_NO_ODB||PFN.vUNI02<0x09||(PFNf_LIN_KSK&&PFNf_FLIN)||(PFNf_LIN_STR&&PFNf_FLIN))    //brak polaczenia z UNI-02 lub wersja oprogramowania UNI-02<09 lub aktywny interfejs LIN w trybie sterowania?  
        if(MGSDf_NO_ODB||PFN.vUNI02<0x09||(!PFNf_LIN_BPSS&&PFNf_LIN_KSK&&PFNf_FLIN)||(!PFNf_LIN_BPSS&&PFNf_LIN_STR&&PFNf_FLIN)) //+++
        {
            OTMaster.Ref.war=0;
            OTMaster.Status._DHW_enable=0;
            OTMaster.Status._CH_enable=0;            
            OT.ACKType=_OT_NO_RESP;
            PFN.IPCO=0;     //jjj
            PFN.IPCW=0;
            PFNf_new_IPCO=0;
            PFNf_new_IPCW=0;                       
        }
        else
        {
            switch(OT.RFrame.msg_type)
            {
                case _READ_OT_DATA:
                {
                    BitSet3(&RSDT1.inINSTAT,_FOT,1);        //odswiez znacznik komunikacji OT dla ukladu UNI-02                
                    PFNf_FOT=1;
                    StartRTdS(_RTFOT);                
                    OT.ACKType=OTMSGReadData();
                    break;
                }
                case _WRITE_OT_DATA:
                {
                    BitSet3(&RSDT1.inINSTAT,_FOT,1);        //odswiez znacznik komunikacji OT dla ukladu UNI-02                
                    PFNf_FOT=1;
                    StartRTdS(_RTFOT);
                    OT.ACKType=OTMSGWriteData();
                    break;
                }  
                default:
                {
                    OT.ACKType=_OT_NO_RESP;
                    break;
                }
            }
        }
        if(OT.ACKType!=0) OT.ToACK=0;               //gotowa odpowiedz
    } 
#else
    //TEST OT
    volatile unsigned int dt=0; //jjj
    volatile unsigned char dt2=0;    
    
    BitSet3(&RSDT1.inINSTAT,_FOT,1);        //odswiez znacznik komunikacji OT dla ukladu UNI-02                
    PFNf_FOT=1;
    StartRTdS(_RTFOT);    
    //-----------------------------------------------------------
    //STATUS    
    OTMaster.Ref._Status=1;
    OTMaster.Status._CH_enable=1;   //pobudzenie RT
    OTMaster.Status._DHW_enable=1;  //timer zasobnika
    PFNf_OT_DPGD=1;                 //dezaktywacja regulatora pogodowego
    //-----------------------------------------------------------
    //SETPOINT
    OTMaster.Ref._Control_Setpoint=1;
    OTMaster.Control_Setpoint=47;   //wartosc SETP
    if(OTMaster.Control_Setpoint<0)
    {
        OTMaster.Control_Setpoint=0;
    }            
    dt2=(unsigned char) OTMaster.Control_Setpoint; 
    if((dt2>=PFN.SETPmin)&&(dt2<=PFN.SETPmax))   //dolna granica =minPCO, gorna granica =PCO    //jjj
    {
        if(!PFNf_LIN_BPSS)
        {
            PFN.INSETP=dt2;
            PFN.IINSETP=dt2;
        }
        else
        {
            PFN.IINSETP=dt2;
        }
    }
    else
    if(dt2<PFN.SETPmin)    
    {
        if(!PFNf_LIN_BPSS)
        {
            PFN.INSETP=PFN.SETPmin;
            PFN.IINSETP=PFN.SETPmin;
        }
        else
        {
            PFN.IINSETP=PFN.SETPmin;
        }
    }
    else
    if(dt2>PFN.SETPmax)
    {
        if(!PFNf_LIN_BPSS)
        {
            PFN.INSETP=PFN.SETPmax;
            PFN.IINSETP=PFN.SETPmax;
        }
        else
        {
            PFN.IINSETP=PFN.SETPmax;
        }
    }     
    //-----------------------------------------------------------
    //PCW
    OTMaster.Ref._DHW_Setpoint=1;            
    OTMaster.DHW_Setpoint=51;   //wartosc PCW
    if(OTMaster.DHW_Setpoint<0)
    {
        OTMaster.DHW_Setpoint=0;
    }            
    dt=(unsigned int)OTMaster.DHW_Setpoint; //jjj
    if((dt>=PFN.minPCW)&&(dt<=PFN.maxPCW))    //jjj
    {              
        if(!PFNf_LIN_BPSS)
        {          
            PFNf_new_IPCW=0;
            PFN.PCW=dt;
            PFN.IPCW=dt;
        }
        else
        {
            if(dt!=PFN.IPCW)
            {
                PFNf_new_IPCW=1;
            }
            PFN.IPCW=dt;
        }
    } 
    else
    if(dt<PFN.minPCW)
    {
        if(!PFNf_LIN_BPSS)
        {            
            PFNf_new_IPCW=0;
            PFN.PCW=PFN.minPCW;
            PFN.IPCW=PFN.minPCW;
        }
        else
        {
            if(PFN.minPCW!=PFN.IPCW)
            {
                PFNf_new_IPCW=1;
            }
            PFN.IPCW=PFN.minPCW;                    
        }              
    }
    else
    if(dt>PFN.maxPCW)
    {
        if(!PFNf_LIN_BPSS)
        {    
            PFNf_new_IPCW=0;
            PFN.PCW=PFN.maxPCW;
            PFN.IPCW=PFN.maxPCW;
        }
        else
        {
            if(PFN.maxPCW!=PFN.IPCW)
            {
                PFNf_new_IPCW=1;
            }
            PFN.IPCW=PFN.maxPCW;                     
        }
    }  
    //-----------------------------------------------------------
    //PCO
    OTMaster.Ref._maxCH_Setpoint=1;            
    OTMaster.maxCH_Setpoint=58;   //wartosc PCO
    if(OTMaster.maxCH_Setpoint<0)
    {
        OTMaster.maxCH_Setpoint=0;
    }
    dt=(unsigned int)OTMaster.maxCH_Setpoint;   //jjj
    if(!PFNf_pgd)       //przy nieaktywnym regulatorze zezwol na modyfikacje maxCH
    {
        if((dt>=PFN.minPCO)&&(dt<=PFN.maxPCO))    //jjj
        {
            if(!PFNf_LIN_BPSS)
            {        
                PFNf_new_IPCO=0;
                PFN.PCO=dt;
                PFN.IPCO=dt;
            }
            else
            {
                if(dt!=PFN.IPCO)
                {
                    PFNf_new_IPCO=1;
                }
                PFN.IPCO=dt;                        
            }
        }
        else
        if(dt<PFN.minPCO)    
        {
            if(!PFNf_LIN_BPSS)
            {                    
                PFNf_new_IPCO=0;
                PFN.PCO=PFN.minPCO;
                PFN.IPCO=PFN.minPCO;
            }
            else
            {
                if(PFN.minPCO!=PFN.IPCO)
                {
                    PFNf_new_IPCO=1;
                }
                PFN.IPCO=PFN.minPCO;                        
            }
        }
        else
        if(dt>PFN.maxPCO)
        {
            if(!PFNf_LIN_BPSS)
            {                    
                PFNf_new_IPCO=0;
                PFN.PCO=PFN.maxPCO;    
                PFN.IPCO=PFN.maxPCO;
            }
            else
            {
                if(PFN.maxPCO!=PFN.IPCO)
                {
                    PFNf_new_IPCO=1;
                }
                PFN.IPCO=PFN.maxPCO;                        
            }
        }  
    }    
#endif
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
void SetUkownAckRespData(void)
{
    OT.TFrame.msg_type=_UNKOWN_ACK_OT_DATA;
    OT.TFrame.data_id=OT.RFrame.data_id;
    OT.TFrame.data1=OT.RFrame.data1;
    OT.TFrame.data0=OT.RFrame.data0;    
}
void SetReadAckRespData(void)
{
    OT.TFrame.msg_type=_READ_ACK_OT_DATA;
    OT.TFrame.data_id=OT.RFrame.data_id;   
}
void SetWriteAckRespData(void)
{
    OT.TFrame.msg_type=_WRITE_ACK_OT_DATA;
    OT.TFrame.data_id=OT.RFrame.data_id;
    OT.TFrame.word0.war=OT.RFrame.word0.war;    
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
//Interpretacja odebranej ramki z poleceniem odczytu
unsigned char did[50]={0};
unsigned char OTMSGReadData(void)
{
    static unsigned char ind=0;//,a=0;
    
    if(ind<49)ind++;
    else ind=0;
    //a++;
    did[ind]=OT.RFrame.data_id;
    //db=a;
    switch(OT.RFrame.data_id)
    {
        //-----------------------------------------
        //Class 1 : Control and Status Information
        case 0:     //Master and slave status flags
        {
            //_WOUTX=1;
            SetReadAckRespData();
            //------------------------------
            //byte1 - Master status
            /*bit: description [ clear/0, set/1]
            0: CH enable [ CH is disabled, CH is enabled]           //pobudzenie RT
            1: DHW enable [ DHW is disabled, DHW is enabled]        //timer zasobnika
            2: Cooling enable [ Cooling is disabled, Cooling is enabled]
            3: OTC active [OTC not active, OTC is active]
            4: CH2 enable [CH2 is disabled, CH2 is enabled]
            5: Summer/winter mode [winter mode active, summer mode active]
            6: DHW blocking [DHW unblocked, DHW blocked]
            7: reserved
             */
            OTMaster.Ref._Status=1;
            OTMaster.Status.war=OT.RFrame.byte1.war;
            OT.TFrame.byte1.war=OT.RFrame.byte1.war;
            
            //--------------------------------
            //byte0 - Slave status
            /*bit: description [ clear/0, set/1]
            0: fault indication [ no fault, fault ]
            1: CH mode [CH not active, CH active]
            2: DHW mode [ DHW not active, DHW active]
            3: Flame status [ flame off, flame on ]
            4: Cooling status [ cooling mode not active, cooling mode active ]
            5: CH2 mode [CH2 not active, CH2 active]
            6: diagnostic/service indication [no diagnostic/service, diagnostic/service event]
            7: Electricity production [off, on]
             */            
            OT.TFrame.byte0.war=0;
            if(PFNf_ferr||PFNf_fmsg) OT.TFrame.byte0.b0=1;          //fault indication=1
            else OT.TFrame.byte0.b0=0;                              //fault indication=0
            if(PFNf_obgCO) OT.TFrame.byte0.b1=1;                    //CH active
            else OT.TFrame.byte0.b1=0;                              //CH not active
            if(PFNf_obgCW) OT.TFrame.byte0.b2=1;                    //DHW active  
            else OT.TFrame.byte0.b2=0;                              //DHW not active 
            if(PFNf_plmCW||PFNf_plmCO) OT.TFrame.byte0.b3=1;        //flame status=1
            else OT.TFrame.byte0.b3=0;                              //flame status=0
            //--------------------------------
            return _OT_TO_RESP;            
        }
        case 5:     //Application-specific fault flags
        {
            SetReadAckRespData();      
            
            OT.TFrame.byte1.war=0;
            
            if(PFNf_ferr) OT.TFrame.byte1.b0=1;                 //Service request [service not req?d, service required]
            else OT.TFrame.byte1.b0=0;
            if((PFNf_fmsg||PFNf_ferr)&&((PFN.KERR==9)||(PFN.KAWR==9))&&(PFN.CS<=6))
            {
                OT.TFrame.byte1.b2=1;                           //Low water press [no WP fault, water pressure fault]
            }
            else OT.TFrame.byte1.b2=0;
            if((PFNf_fmsg||PFNf_ferr)&&((PFN.KERR==1)||(PFN.KAWR==1)))
            {
                OT.TFrame.byte1.b3=1;                           //Gas/flame fault [ no G/F fault, gas/flame fault ]
            }
            else OT.TFrame.byte1.b3=0;         
            if((PFNf_fmsg||PFNf_ferr)&&((PFN.KERR==3)||(PFN.KAWR==3)))
            {
                OT.TFrame.byte1.b4=1;                           //Air press fault [ no AP fault, air pressure fault ]
            }
            else OT.TFrame.byte1.b4=0;   
            if((PFNf_fmsg||PFNf_ferr)&&((PFN.KERR==2)||(PFN.KAWR==2)))
            {
                OT.TFrame.byte1.b5=1;                           //Water over-temp [ no OvT fault, over-temperat. Fault]
            }
            else OT.TFrame.byte1.b5=0;      
            if(PFNf_fmsg||PFNf_ferr) 
            {
                if(PFNf_ferr) OT.TFrame.byte0.war=PFN.KERR;     //An OEM-specific fault/error code
                else OT.TFrame.byte0.war=PFN.KAWR;
            }
            else OT.TFrame.byte0.war=0;
            return _OT_TO_RESP;            
        }

        case 70:    //Status ventilation heat-recovery
        case 72:    //Application-specific fault flags ventilation /heat-recovery   
        case 73:    //OEM diagnostic code ventilation / heat-recovery            
        case 101:   //Solar Storage mode and status 
        case 115:   //OEM diagnostic code            
        {
            SetUkownAckRespData();
            return _OT_TO_RESP;
        }
        //--------------------------------------------
        //Class 2 : Configuration Information
        case 3:     //Slave configuration
        {
            SetReadAckRespData();  
            OT.TFrame.byte1.war=0;
            OT.TFrame.byte1.b0=1;   //DHW present [ dhw not present, dhw is present ]
            //if(!PFNf_EnAleg) OT.TFrame.byte1.b0=0;   //DHW present [ dhw not present, dhw is present ]
            //else OT.TFrame.byte1.b0=1;            
            OT.TFrame.byte1.b1=0;   //Control type [ modulating, on/off ]
            OT.TFrame.byte1.b2=0;   //Cooling config [ cooling not supported,cooling supported]  
            if(!PFNf_EnAleg) OT.TFrame.byte1.b3=0;   //DHW config [instantaneous or not-specified,storage tank]
            else OT.TFrame.byte1.b3=1;
            OT.TFrame.byte1.b4=1;   //Master low-off&pump control function [allowed,not allowed]
            OT.TFrame.byte1.b5=0;   //CH2 present [CH2 not present, CH2 present]            
            OT.TFrame.byte1.b6=0;   //Remote water filling function [available or unknown,not available]. Unknown for applications with protocol version 2.2 or older.           
            OT.TFrame.byte1.b7=1;   //Heat/cool mode control [Heat/cool mode switching can be done by master, Heat/cool mode switching is done by slave]  
            
            OT.TFrame.byte0.war=0;  //MemberID code of the slave
            return _OT_TO_RESP;            
        }
        case 125:     //OpenTherm version Slave
        {
            SetReadAckRespData();
            
            OT.TFrame.word0.war=DataToF88((float)_OT_VER); //The implemented version of the OpenTherm Protocol Specification in the ventilation / heatrecovery system.          
            return _OT_TO_RESP;            
        } 
        case 127:   //Slave product version number and type
        {
            SetReadAckRespData();
            OT.TFrame.byte1.war=0;    //produkt type
            OT.TFrame.byte0.war=0;    //product version          
            return _OT_TO_RESP;            
        }
        case 74:    //OpenTherm version ventilation / heat-recovery 
        case 75:    //Configuration ventilation / heat-recovery       
        case 76:    //Ventilation / heatrecovery product version number and type            
        case 103:   //Solar Storage configuration  
        case 104:   //Solar Storage product version number and type
        {
            SetUkownAckRespData();
            return _OT_TO_RESP;
        } 
        //-----------------------------------------
        //Class 3 : Remote Request
        //-----------------------------------------
        //Class 4 : Sensor and Informational Data
        case 17:     //Relative Modulation Level.
        {
            SetReadAckRespData();
            
            OT.TFrame.word0.war=DataToF88(PFN.PG);      //Percent modulation between min and max modulation levels. i.e.
                                                    //0% = Minimum modulation level
                                                    //100% = Maximum modulation level
            return _OT_TO_RESP;            
        }  
        case 18:     //CH water pressure.
        {
            SetReadAckRespData();
            
            OT.TFrame.word0.war=DataToF88((float)PFN.CS/10.0);   //Water pressure of the boiler CH circuit (bar)
            return _OT_TO_RESP;            
        } 
        /*case 19:     //Water flow rate 
        {
            SetReadAckRespData();
            
            OT.TFrame.word0.war=0;                      //Water flow rate through the DHW circuit (l/min)
            return _OT_TO_RESP;            
        }*/        
        case 25:     //Boiler water temp.
        {
            SetReadAckRespData();
            
            OT.TFrame.word0.war=DataToF88(PFN.CO);      //Flow water temperature from boiler (°C)
            return _OT_TO_RESP;            
        }
        case 26:     //DHW temperature
        {
            SetReadAckRespData();
            
            OT.TFrame.word0.war=DataToF88(PFN.CW);      // Domestic hot water temperature (°C)           
            return _OT_TO_RESP;            
        }   
        case 27:   //Outside temperature
        {
            if(PFNf_tmpZW)      //aktywny czujnik zewnetrzny?
            {
                SetReadAckRespData();
                if(!PFNf_pgd||(PFNf_pgd&&!PFNf_pgdTP))
                {
                    PFNf_OT_DPGD=1;
                }
                else
                {
                    PFNf_OT_DPGD=0;
                }
                OT.TFrame.word0.war=DataToF88(PFN.ZW);      // Outside temperature (°C)
                return _OT_TO_RESP;
            }
            else
            {
                SetUkownAckRespData();
                PFNf_OT_DPGD=0;
                return _OT_TO_RESP;                
            }
        }            
        case 28:     //Return water temperature
        {
            SetReadAckRespData();
            
            OT.TFrame.word0.war=DataToF88(PFN.IN);      // Domestic hot water temperature (°C)
            return _OT_TO_RESP;            
        }        
        case 35:     //Boiler fan speed
        {
            SetReadAckRespData();
            
            OT.TFrame.data1=(unsigned char) (PFN.VNT*100)/60;   //Actual boiler fan speed Setpoint in Hz (RPM/60)
            OT.TFrame.data0=(unsigned char) (PFN.VNT*100)/60;   //Actual boiler fan speed in Hz (RPM/60)           
            return _OT_TO_RESP;            
        }      
        case 19:   //Water flow rate
        case 20:   //Day of Week & Time of Day        
        case 21:   //Date
        case 22:   //Year            

        case 29:   //Solar storage temperature        
        case 30:   //Solar collector temperature        
        case 31:   //Flow temperature CH2
        case 32:   //DHW2 temperature        
        case 33:   //Exhaust temperature (temp. spalin)        
        case 34:   //Boiler heat exchanger temperature     
        case 36:   //Flame current        
        case 37:   //TrCH2        
        case 38:   //Relative Humidity
        case 77:   //Relative ventilation        
        case 78:   //Relative humidity        
        case 79:   //CO2 level 
        case 80:   //Supply inlet temperature
        case 81:   //Supply outlet temperature        
        case 82:   //Exhaust inlet temperature        
        case 83:   //Exhaust outlet temperature  
        case 84:   //Actual exhaust fan speed        
        case 85:   //Actual inlet fan speed           
        case 109:  //Electricity producer starts  
        case 110:  //Electricity producer hours        
        case 111:  //Electricity production      
        case 112:  //Cumulative Electricity production        
        case 113:  //Number of un-successful burner starts  
        case 114:  //Number of times flame signal was too low        
        case 116:  //Successful Burner starts        
        case 117:  //CH pump starts        
        case 118:  //DHW pump/valve starts  
        case 119:  //DHW burner starts        
        case 120:  //Burner operation hours            
        case 121:  //CH pump operation hours        
        case 122:  //DHW pump/valve operation hours  
        case 123:  //DHW burner operation hours                     
        {
            SetUkownAckRespData();
            return _OT_TO_RESP;
        }  
        //-----------------------------------------
        //Class 5 : Pre-Defined Remote Boiler Parameters
        case 6:     //Remote-parameter transfer-enable flags
        {
            SetReadAckRespData();
            
            OT.TFrame.byte1.war=0;  //Remote-parameter transfer-enable flags
            OT.TFrame.byte1.b0=1;   //0: DHW Setpoint [ transfer disabled, transfer enabled ]
            OT.TFrame.byte1.b1=1;   //max CHsetpoint [ transfer disabled, transfer enabled ]   
            OT.TFrame.byte0.war=0;  //Remote-parameter transfer-enable flags
            OT.TFrame.byte0.b0=1;   //DHW Setpoint [ read-only, read/write ]
            OT.TFrame.byte0.b1=1;   //max CHsetpoint [ read-only, read/write ]            
            return _OT_TO_RESP;            
        }
        case 48:     //DHWsetp upp-bound, low-bound
        {
            SetReadAckRespData();
            
            OT.TFrame.data1=PFN.maxPCW;   //DHW Setpoint upper & lower bounds for adjustment (°C)
            OT.TFrame.data0=PFN.minPCW;   //DHW Setpoint upper & lower bounds for adjustment (°C)              
            return _OT_TO_RESP;            
        }   
        case 49:     //max CHsetp upp-bound, low-bound
        {
            SetReadAckRespData();
            if(!PFNf_pgd)
            {
                OT.TFrame.data1=PFN.maxPCO;   //Max CH water Setpoint upper & lower bounds for adjustment (°C)
                OT.TFrame.data0=PFN.minPCO;   //Max CH water Setpoint upper & lower bounds for adjustment (°C)
            }
            else
            {
                OT.TFrame.data1=PFN.SETPmax;
                OT.TFrame.data0=PFN.SETPmin;
            }
            return _OT_TO_RESP;            
        } 
        case 56:     //DHW Setpoint
        {
            SetReadAckRespData();
            
            OT.TFrame.word0.war=DataToF88(PFN.PCW);      // Domestic hot water temperature Setpoint (°C)
            return _OT_TO_RESP;            
        }        
        case 57:     //max CH water Setpoint
        {
            SetReadAckRespData();
            if(!PFNf_pgd)
            {            
                OT.TFrame.word0.war=DataToF88(PFN.PCO);          //Maximum allowable CH water Setpoint (°C)
            }
            else
            {
                OT.TFrame.word0.war=DataToF88(PFN.SETPmax);      //Maximum allowable CH water Setpoint (°C)                
            }
            return _OT_TO_RESP;            
        }               
        case 86:  //Remote-parameter transfer-enable flags ventilation / heat-recovery 
        case 87:  //Nominal ventilation value            
        {
            SetUkownAckRespData();
            return _OT_TO_RESP;
        } 
        //-----------------------------------------
        //Class 6 : Transparent Slave Parameters
        /*case 10:  //Number of TSP's 
        {
            SetReadAckRespData();
            
            OT.TFrame.word0.war=0;      // Number of transparent-slave-parameter supported by the slave device.
            return _OT_TO_RESP;            
        }*/
        case 10:  //Number of TSP's        
        case 11:  //TSP index no, value 
        case 88:  //Number of TSP?s ventilation / heat-recovery 
        case 89:  //TSP index no. ventilation / heat-recovery     
        case 105:  //Number of TSP?s Solar Storage 
        case 106:  //TSP index no. SolarStorage            
        {
            SetUkownAckRespData();            
            return _OT_TO_RESP;
        }  
        //-----------------------------------------
        //Class 7 : Fault History Data      
        /*case 12:  //Size of Fault Buffer 
        {
            SetReadAckRespData();
            
            OT.TFrame.word0.war=0;      // The size of the fault history buffer..
            return _OT_TO_RESP;            
        }*/  
        case 12:  //Size of Fault Buffer        
        case 13:  //FHB-entry index no 
        case 90:  //Size of Fault Buffer ventilation / heat-recovery 
        case 91:  //FHB-entry index no. ventilation / heat-recovery     
        case 107:  //Size of Fault Buffer Solar Storage 
        case 108:  //FHB-entry index no.Solar Storage            
        {
            SetUkownAckRespData();            
            return _OT_TO_RESP;
        }  
        //-----------------------------------------
        //Class 8 : Control of Special Applications
        /*case 9:  //Remote Override Room Setpoint    
        {
            SetReadAckRespData();
            
            OT.TFrame.word0.war=0;      // 0= No override 1..30= Remote override room Setpoint
            return _OT_TO_RESP;            
        }     
        case 99:  //Remote Override Operating Mode Heating
        {
            SetReadAckRespData();
            
            OT.TFrame.byte1.war=0;      //Remote Override Operating Mode DHW            
            OT.TFrame.byte0.war=0;      //Remote Override Operating Mode Heating
            return _OT_TO_RESP;            
        }            
        case 100:  //Remote Override Room Setpoint function         
        {
            SetReadAckRespData();
            
            OT.TFrame.byte1.war=0;   //reserved
            OT.TFrame.byte0.war=0;   //Remote Override Room Setpoint function
            OT.TFrame.byte0.b0=0;//Manual change priority [disable overruling remote Setpoint by manual Setpoint change, enable overruling remote Setpoint by manual Setpoint change ]
            OT.TFrame.byte0.b1=0;//Program change priority [disable overruling remote Setpoint by program Setpoint change, enable overruling remote Setpoint by program Setpoint change ]            
            return _OT_TO_RESP;            
        }*/
        case 9:  //Remote Override Room Setpoint
        case 99:  //Remote Override Operating Mode Heating 
        case 100:  //Remote Override Room Setpoint function   
        case 7:  //Cooling control signal    
        case 14:  //Maximum relative modulation level setting 
        case 15:  //Maximum boiler capacity & Minimum modulation level  
        {
            SetUkownAckRespData();            
            return _OT_TO_RESP;
        } 
        default:
        {
            SetUkownAckRespData();            
            return _OT_TO_RESP;            
        }
    }
}
/*---------------------------------------------------------------------------------*/
//Interpretacja odebranej ramki z poleceniem zapisu
unsigned char OTMSGWriteData(void)
{
    volatile unsigned int dt=0; //jjj
    volatile unsigned char dt2=0;   
    
    if(PFN.STRB==2)     //dla odbioru produkcyjnego ignoruj polecenia zapisu
    {
        SetWriteAckRespData();
        return _OT_TO_RESP;        
    }
    switch(OT.RFrame.data_id)
    {
        //-----------------------------------------
        //Class 1 : Control and Status Information
        case 1:     //Control Setpoint f8.8
        {
            SetWriteAckRespData(); 
            OTMaster.Ref._Control_Setpoint=1;
            OTMaster.Control_Setpoint=F88ToData((signed int) OT.RFrame.word0.war);
            if(OTMaster.Control_Setpoint<0)
            {
                OTMaster.Control_Setpoint=0;
            }            
            dt2=(unsigned char) OTMaster.Control_Setpoint;  //jjj
            if((dt2>=PFN.SETPmin)&&(dt2<=PFN.SETPmax))   //dolna granica =minPCO, gorna granica =PCO    //jjj
            {
                if(!PFNf_LIN_BPSS)
                {
                    PFN.INSETP=dt2;
                    PFN.IINSETP=dt2;
                }
                else
                {
                    PFN.IINSETP=dt2;
                }
            }
            else
            if(dt2<PFN.SETPmin)    
            {
                if(!PFNf_LIN_BPSS)
                {
                    PFN.INSETP=PFN.SETPmin;
                    PFN.IINSETP=PFN.SETPmin;
                }
                else
                {
                    PFN.IINSETP=PFN.SETPmin;
                }
            }
            else
            if(dt2>PFN.SETPmax)
            {
                if(!PFNf_LIN_BPSS)
                {
                    PFN.INSETP=PFN.SETPmax;
                    PFN.IINSETP=PFN.SETPmin;
                }
                else
                {
                    PFN.IINSETP=PFN.SETPmax;
                }
            }                     
            return _OT_TO_RESP;
        }
        case 8:     //Control Setpoint2 f8.8        
        case 71:    //Control Setpoint ventilation / heat-recovery          
        {
            SetUkownAckRespData();
            return _OT_TO_RESP;
        }
        //--------------------------------------------
        //Class 2 : Configuration Information
        case 2:       //Master configuration 
        {
            SetWriteAckRespData();            
            OTMaster.Ref._MemberID=1;   
            OTMaster.MemberID=OT.RFrame.data0;   
            OTMaster.Ref._Config=1;            
            OTMaster.Config.war=OT.RFrame.data1;            
            return _OT_TO_RESP;  
        }        
        case 124:     //OpenTherm version Master
        {
            SetWriteAckRespData();
            OTMaster.Ref._OTVer=1;      
            OTMaster.OTVer=OT.RFrame.word0.war;
            return _OT_TO_RESP;            
        } 
        case 126:     //Master product version number and type
        {
            SetWriteAckRespData();
            OTMaster.Ref._ProductVer=1;            
            OTMaster.ProductVer=OT.RFrame.data0;    
            OTMaster.Ref._ProductType=1;            
            OTMaster.ProductType=OT.RFrame.data1;            
            return _OT_TO_RESP;            
        }                
        //-----------------------------------------
        //Class 3 : Remote Request
        case 4:     //Request-Code
        {
            switch(OT.RFrame.data1)
            {
                case 0: //Back to Normal oparation mode
                case 1: //?BLOR?= Boiler Lock-out Reset request
                case 2: //?CHWF?=CH water filling request
                case 3: //Service mode maximum power request(for instance for CO2 measurement during Chimney Sweep Function )
                case 4: //Service mode minimum power request(CO2 measurement)
                case 5: //Service mode spark test request (no gas)
                case 6: //Service mode fan maximum speed request(no flame)
                case 7: //Service mode fan to minimum speed request (no flame)
                case 8: //Service mode 3-way valve to CH request(no pump, no flame)
                case 9: //Service mode 3-way valve to DHW request(no pump, no flame)
                case 10://Request to reset service request flag
                case 11://Service test 1. This is a OEM specific test.
                case 12://Automatic hydronic air purge
                default:    //odrzucenie polecenia
                    OT.TFrame.msg_type=_UNKOWN_ACK_OT_DATA;
                    OT.TFrame.data1=OT.RFrame.data1;
                    OT.TFrame.data0=0;  //0..127 refused; 128..255 accepted
                    return _OT_TO_RESP;                       
                    
            }
        }            
        //-----------------------------------------
        //Class 4 : Sensor and Informational Data          
        case 16:   //Room Setpoint        
        {
            SetWriteAckRespData();
            OTMaster.Ref._Room_Setpoint=1;            
            OTMaster.Room_Setpoint=F88ToData((signed int) OT.RFrame.word0.war);            
            return _OT_TO_RESP;            
        }
        case 24:   //Room temperature 
        {
            SetWriteAckRespData();
            OTMaster.Ref._Room_Temperature=1;            
            OTMaster.Room_Temperature=F88ToData((signed int) OT.RFrame.word0.war);            
            return _OT_TO_RESP;            
        }            
        case 20:   //Day of Week & Time of Day        
        case 21:   //Date
        case 22:   //Year        
        case 23:   //Room Setpoint CH2
        case 27:    //Outside temperature
        case 37:   //TrCH2        
        case 38:   //Relative Humidity      
        case 78:   //Relative humidity        
        case 79:   //CO2 level 
        case 98:   //Type of sensor and battery        
        case 109:  //Electricity producer starts  
        case 110:  //Electricity producer hours            
        case 112:  //Cumulative Electricity production        
        case 113:  //Number of un-successful burner starts  
        case 114:  //Number of times flame signal was too low        
        case 116:  //Successful Burner starts        
        case 117:  //CH pump starts        
        case 118:  //DHW pump/valve starts  
        case 119:  //DHW burner starts        
        case 120:  //Burner operation hours            
        case 121:  //CH pump operation hours        
        case 122:  //DHW pump/valve operation hours       
        case 123:  //DHW burner operation hours            
        {
            SetUkownAckRespData();
            return _OT_TO_RESP;
        }  
        //-----------------------------------------
        //Class 5 : Pre-Defined Remote Boiler Parameters 
        case 56:     //DHW Setpoint
        {
            SetWriteAckRespData();
            OTMaster.Ref._DHW_Setpoint=1;            
            OTMaster.DHW_Setpoint=F88ToData((signed int) OT.RFrame.word0.war);
            if(OTMaster.DHW_Setpoint<0)
            {
                OTMaster.DHW_Setpoint=0;
            }            
            dt=(unsigned int)OTMaster.DHW_Setpoint; //jjj
            if((dt>=PFN.minPCW)&&(dt<=PFN.maxPCW))    //jjj
            {              
                if(!PFNf_LIN_BPSS)
                {          
                    PFNf_new_IPCW=0;
                    PFN.PCW=dt;
                    PFN.IPCW=dt;
                }
                else
                {
                    if(dt!=PFN.IPCW)
                    {
                        PFNf_new_IPCW=1;
                    }
                    PFN.IPCW=dt;
                }
            } 
            else
            if(dt<PFN.minPCW)
            {
                if(!PFNf_LIN_BPSS)
                {            
                    PFNf_new_IPCW=0;
                    PFN.PCW=PFN.minPCW;
                    PFN.IPCW=PFN.minPCW;
                }
                else
                {
                    if(PFN.minPCW!=PFN.IPCW)
                    {
                        PFNf_new_IPCW=1;
                    }
                    PFN.IPCW=PFN.minPCW;                    
                }              
            }
            else
            if(dt>PFN.maxPCW)
            {
                if(!PFNf_LIN_BPSS)
                {    
                    PFNf_new_IPCW=0;
                    PFN.PCW=PFN.maxPCW;
                    PFN.IPCW=PFN.maxPCW;
                }
                else
                {
                    if(PFN.maxPCW!=PFN.IPCW)
                    {
                        PFNf_new_IPCW=1;
                    }
                    PFN.IPCW=PFN.maxPCW;                     
                }
            }  
            return _OT_TO_RESP; 
        }        
        case 57:     //max CH water Setpoint
        {                       
            OTMaster.Ref._maxCH_Setpoint=1;            
            OTMaster.maxCH_Setpoint=F88ToData((signed int) OT.RFrame.word0.war);
            if(OTMaster.maxCH_Setpoint<0)
            {
                OTMaster.maxCH_Setpoint=0;
            }
            dt=(unsigned int)OTMaster.maxCH_Setpoint;   //jjj
            if(!PFNf_pgd)       //przy nieaktywnym regulatorze zezwol na modyfikacje maxCH
            {
                SetWriteAckRespData();
                if((dt>=PFN.minPCO)&&(dt<=PFN.maxPCO))    //jjj
                {
                    if(!PFNf_LIN_BPSS)
                    {        
                        PFNf_new_IPCO=0;
                        PFN.PCO=dt;
                        PFN.IPCO=dt;
                    }
                    else
                    {
                        if(dt!=PFN.IPCO)
                        {
                            PFNf_new_IPCO=1;
                        }
                        PFN.IPCO=dt;                        
                    }
                }
                else
                if(dt<PFN.minPCO)    
                {
                    if(!PFNf_LIN_BPSS)
                    {                    
                        PFNf_new_IPCO=0;
                        PFN.PCO=PFN.minPCO;
                        PFN.IPCO=PFN.minPCO;
                    }
                    else
                    {
                        if(PFN.minPCO!=PFN.IPCO)
                        {
                            PFNf_new_IPCO=1;
                        }
                        PFN.IPCO=PFN.minPCO;                        
                    }
                }
                else
                if(dt>PFN.maxPCO)
                {
                    if(!PFNf_LIN_BPSS)
                    {                    
                        PFNf_new_IPCO=0;
                        PFN.PCO=PFN.maxPCO;    
                        PFN.IPCO=PFN.maxPCO;
                    }
                    else
                    {
                        if(PFN.maxPCO!=PFN.IPCO)
                        {
                            PFNf_new_IPCO=1;
                        }
                        PFN.IPCO=PFN.maxPCO;                        
                    }
                }  
            }
            else                //przy aktywnym reg. pogodowym to regulator definiuje maxCH
            {
                SetUkownAckRespData();  
            }           
            return _OT_TO_RESP;            
        }               
        case 87:  //Nominal ventilation value            
        {
            SetUkownAckRespData();
            return _OT_TO_RESP;
        } 
        //-----------------------------------------
        //Class 6 : Transparent Slave Parameters
        case 11:  //TSP index no, value  
        case 89:  //TSP index no. ventilation / heat-recovery     
        case 106:  //TSP index no. SolarStorage            
        {
            SetUkownAckRespData();            
            return _OT_TO_RESP;
        }  
        //-----------------------------------------
        //Class 7 : Fault History Data      
        //-----------------------------------------
        //Class 8 : Control of Special Applications
        case 7:  //Cooling control signal    
        case 14:  //Maximum relative modulation level setting   
        case 99:  //Remote Override Operating Mode Heating            
        {
            SetUkownAckRespData();            
            return _OT_TO_RESP;
        }   
        default:
        {
            SetUkownAckRespData();            
            return _OT_TO_RESP;            
        }        
    }    
}
/*---------------------------------------------------------------------------------*/
signed int DataToF88(float dt)
{
    return (signed int)(dt*256);
}
float F88ToData(signed int dt)
{
    return (float)(dt/256);
}
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
#endif //(_EXT_BOARD>=3)