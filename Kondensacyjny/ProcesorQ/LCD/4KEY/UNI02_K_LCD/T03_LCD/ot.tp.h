/*ot.tp.h*/
#ifndef	_OT_TP_H
#define	_OT_TP_H
//----------------------------------------------------------
	volatile bit PFNf_firstEdge;		//pierwsze zbocze
typedef union   {
                    unsigned char war;
                    struct //__PACKED
                    {
                        unsigned b0:1;
                        unsigned b1:1;
                        unsigned b2:1;
                        unsigned b3:1;
                        unsigned b4:1;
                        unsigned b5:1;
                        unsigned b6:1;
                        unsigned b7:1;
                     };
}BTY8_VAL; 
typedef union   {
                    unsigned char war;
                    struct //__PACKED
                    {
                        unsigned spare:4;
                        unsigned msg_tp:3;
                        unsigned parity:1;
                     };
}BTYOT1_VAL;
typedef union   {
                    unsigned char data_id;
                    struct //__PACKED
                    {
                        unsigned b0:1;
                        unsigned b1:1;
                        unsigned b2:1;
                        unsigned b3:1;
                        unsigned b4:1;
                        unsigned b5:1;
                        unsigned b6:1;
                        unsigned b7:1;
                     };
}BTYOT2_VAL;
typedef union   {
                    unsigned int war;
                    struct //__PACKED
                    {
                        BTY8_VAL byte0;  
                        BTY8_VAL byte1; 
                    };                    
                    struct //__PACKED
                    {
                        unsigned b0:1;
                        unsigned b1:1;
                        unsigned b2:1;
                        unsigned b3:1;
                        unsigned b4:1;
                        unsigned b5:1;
                        unsigned b6:1;
                        unsigned b7:1;
                        unsigned b8:1;
                        unsigned b9:1;
                        unsigned b10:1;
                        unsigned b11:1;
                        unsigned b12:1;
                        unsigned b13:1;
                        unsigned b14:1;
                        unsigned b15:1;                        
                     };
}BTY16_VAL;
typedef union   {
                    long unsigned int war;
                    struct //__PACKED
                    {
                        BTY8_VAL byte0;
                        BTY8_VAL byte1;
                        BTY8_VAL byte2;
                        BTY8_VAL byte3;                        
                    };     
                    struct //__PACKED
                    {
                        BTY16_VAL word0;  
                        BTY16_VAL word1; 
                    };  
                    struct //__PACKED
                    {
                        unsigned data0:8;
                        unsigned data1:8;
                        unsigned data_id:8;
                        unsigned spare:4;
                        unsigned msg_type:3;
                        unsigned parity:1;                        
                    };                    
}BTY32OT_VAL;    
    
typedef struct	{
       
    //IntMillis,WaitOTFrame
    volatile unsigned long int timeLow;   
    volatile unsigned long millis;    //0


    volatile unsigned int tdr;//0
    volatile unsigned int tmo;//0
    volatile unsigned char tptm;//0
    
    //MeasureOTPeriod
    volatile unsigned char cnt1;//0
    volatile unsigned char cnt2;//0
    volatile unsigned int sum;//0
    volatile unsigned int T2;    
    volatile long unsigned int otPeriod;    
    volatile unsigned int prelTMR0;     //period    
    volatile unsigned int preload;     //period
    volatile unsigned int preload2;    //period/2    
    volatile unsigned int preload4;    //period/4
    //ReadOTFrame
    volatile unsigned char cnt;//0
    volatile unsigned char parityOdd;//0
    volatile unsigned int transitionCount;//0    
    volatile unsigned char previousTransitionCount;//0
    volatile long unsigned int tempFrame;//0    
    volatile unsigned long data1;//0
    //volatile unsigned int data2;
    volatile signed int pop;//0
    volatile unsigned int offsetL;
    //volatile unsigned int offsetH;  
    volatile unsigned int offset2;    
    volatile unsigned char errorMsg;   
    volatile unsigned char nRERR;

    volatile unsigned char ToACK;    
    volatile unsigned char ACKType;   
    
    BTY32OT_VAL RFrame;
    BTY32OT_VAL TFrame;
    
}tpOTDta;    



typedef union   {
                    unsigned int war;  
                    struct //__PACKED
                    {
                        unsigned _MemberID:1;                        
                        unsigned _Status:1;
                        unsigned _Config:1;
                        unsigned _OTVer:1;
                        unsigned _ProductType:1;
                        unsigned _ProductVer:1;     
                        unsigned _Control_Setpoint:1;
                        unsigned _Room_Setpoint:1; 
                        unsigned _Room_Temperature:1;     
                        unsigned _DHW_Setpoint:1;
                        unsigned _maxCH_Setpoint:1; 
                        unsigned _Maximum_Mod_Level:1;   
                        unsigned _b12:1; 
                        unsigned _b13:1;     
                        unsigned _b14:1;
                        unsigned _b15:1;                     
                    };                    
}BTY_MOTREF_VAL;  
typedef union   {
                    unsigned char war;  
                    struct //__PACKED
                    {
                        unsigned _CH_enable:1;      //pobudzenie CW                 
                        unsigned _DHW_enable:1;     //pobudzenie CO
                        unsigned _Cooling_enable:1;
                        unsigned _OTC_active:1;     //regulator pogodowy
                        unsigned _CH2_enable:1;
                        unsigned _Summer_winter_mode:1;     
                        unsigned _DHW_blocking:1;
                        unsigned _reserved:1;                      
                    };                    
}BTY_MOTSTAT_VAL; 
typedef union   {
                    unsigned char war;  
                    struct //__PACKED
                    {
                        unsigned _Smart_Power:1;                        
                        unsigned _reserved:7;                      
                    };                    
}BTY_MOTCONF_VAL;
typedef struct	{
    BTY_MOTREF_VAL Ref;                          //bity znacznika aktualizacji danej pobranej z MASTER_OT
    
    volatile int OTVer; //f8.8 
    volatile unsigned char MemberID; 
    volatile unsigned char ProductType;     
    volatile unsigned char ProductVer;     
    BTY_MOTSTAT_VAL Status;                            
    BTY_MOTCONF_VAL Config;                        
    volatile float Control_Setpoint;            //sugerowana temp PCO
    volatile float Room_Setpoint;               //Nastawa temp w pomieszczeniu
    volatile float Room_Temperature;            //Temp w pomieszczeniu 
    volatile float DHW_Setpoint;                //Nastawa temp PCW
    volatile float maxCH_Setpoint;              //Nastawa temp PCO  
    volatile float Maximum_Mod_Level;           //Zdalny wzgledny maksymalny poziom modulacji [%]
    
}tpOTMaster;
//----------------------------------------------------------
#endif		/*_OT_TP_H*/
