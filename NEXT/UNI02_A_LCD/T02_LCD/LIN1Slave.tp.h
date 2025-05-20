/*LIN1Slave.tp.h*/
//typy danych dla pliku LIN1Slave.c
//----------------------------------------------------------
#ifndef	_LIN1SLAV_TPH
#define	_LIN1SLAV_TPH
//----------------------------------------------------------
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
}BTY_VAL1;

volatile bit UAD1tf_RFData;				//znacznik odczytu danych
volatile bit UAD1tf_RFBrSn;                              //znacznik oczekiwania na BREAK SYNCH
volatile bit UAD1tf_RFEnd;				//znacznik konca odbioru
volatile bit UAD1tf_RFExt;				//znacznik anulowania odbioru
volatile bit UAD1tf_TFData;				//znacznik transmisji danych ramki
volatile bit UAD1tf_TFEnd;				//znacznik konca transmisji danych ramki
typedef struct	{
                    /*unsigned _RFData:1;				//znacznik odczytu danych
                    unsigned _RFBrSn:1;                              //znacznik oczekiwania na BREAK SYNCH
                    unsigned _RFEnd:1;				//znacznik konca odbioru
                    unsigned _RFExt:1;				//znacznik anulowania odbioru
                    unsigned _TFData:1;				//znacznik transmisji danych ramki
                    unsigned _TFEnd:1;				//znacznik konca transmisji danych ramki
                    */
                    unsigned char Rindeks;                          //indeks roboczy midulu UART
                    unsigned char Tindeks;                          //indeks roboczy transmisji midulu UART
                    unsigned char RNData;                           //liczba danych do odebrania
                    unsigned char TNData;                           //liczba danych do transmisji
                    unsigned int tmFR;                               //timer ramki
                    unsigned int tmB;                               //timer bajtu
				}UARTData1;


typedef union   {
                    unsigned int war;
                    struct //__PACKED
                    {
                        unsigned char byte0;
                        unsigned char byte1;
                    };
                    struct //__PACKED
                    {
                        unsigned int b0:1;
                        unsigned int b1:1;
                        unsigned int b2:1;
                        unsigned int b3:1;
                        unsigned int b4:1;
                        unsigned int b5:1;
                        unsigned int b6:1;
                        unsigned int b7:1;
                        unsigned int b8:1;
                        unsigned int b9:1;
                        unsigned int b10:1;
                        unsigned int b11:1;
                        unsigned int b12:1;
                        unsigned int b13:1;
                        unsigned int b14:1;
                        unsigned int b15:1;
                     };
}WRD_VAL1;

typedef union {
                    unsigned int wrd;
                    struct //__PACKED
                    {
                        //unsigned int No_Response:1;
                        unsigned int ID_PR:1;       //parrity error
                        unsigned int CRC:1;         //crc error
                        unsigned int BT:1;          //bit error
                        unsigned int NAD:1;         //nad error
                        unsigned int PCI_TYPE:1;    //uncown pci type error
                        unsigned int PCI_LEN:1;     //wrong pci length error
                        unsigned int BUF:1;         //buf error
                        unsigned int RBTOUT:1;      //timeout dla bajtu
                        unsigned int RFTOUT:1;      //timeout dla ramki odbieranej
                        unsigned int LEN:1;         //wrong TFF length
                        unsigned int TFTOUT:1;      //timeout dla ramki wysylanej
                        unsigned int TRTOUT:1;      //timeout dla zapytania/odpowiedzi
                        unsigned int TADTA:1;       //tomeout dla przetwarzania danych
                        unsigned int USID:1;        //nieznany bajt potwierdzenia
                        unsigned int UPIF:1;        //nieznany bajt PIF
                        unsigned int b15:1;
                    };
}LIN_ERROR_FLAGS1;
//----------------------------------------------
volatile bit LIN1f_NDTA;                                //znacznik nowej paczki danych do wysylki
volatile bit LIN1f_Busy;                                //zajetosc LIN1
volatile bit LIN1f_FTrans;                              //aktywna faza zapytania
volatile bit LIN1f_FResp;                               //aktywna faza odpowiedzi
volatile bit LIN1f_FAdta;                               //przetwarzanie danych
volatile bit LIN1f_ERRR;                                 //transmisja/odbior z bledem
volatile bit LIN1f_ACK;                                 //poteirdzenie odbioru
volatile bit LIN1f_SCS;                                 //udany transfer (z potwierdzeniem lub bez)
volatile bit LIN1f_FIN;                                 //znacznik zakonczenia transmisji
volatile bit LIN1f_CHK;                                 //znacznik poprawnosci CHK
typedef struct {
                    /*unsigned _NDTA:1;                                //znacznik nowej paczki danych do wysylki

                    unsigned _Busy:1;                                //zajetosc LIN1
                    unsigned _FTrans:1;                              //aktywna faza zapytania
                    unsigned _FResp:1;                               //aktywna faza odpowiedzi
                    unsigned _FAdta:1;                               //przetwarzanie danych
                    unsigned _ERRR:1;                                 //transmisja/odbior z bledem
                    unsigned _ACK:1;                                 //poteirdzenie odbioru
                    unsigned _SCS:1;                                 //udany transfer (z potwierdzeniem lub bez)
                    unsigned _FIN:1;                                 //znacznik zakonczenia transmisji
                    unsigned _CHK:1;                                 //znacznik poprawnosci CHK
*/
                    unsigned char State;                            //stan interfejsu LIN (MCP2003)
                    unsigned int tmAd;                              //timer oczekiwania na przetworzenie danych   
                    unsigned int tmZw;                              //timer minimalnej zwloki przed wysylka odpowiedzi                    
}LIN_STAT1;
//----------------------------------------------
volatile bit FRL1f_NoResp;           //znacznik czy oczekiwana jest odpowiedz
volatile bit FRL1f_NextFr;
volatile bit FRL1f_ZeroAdr;
//**************************
volatile bit FRL1f_Auto;
//**************************
//**************************
volatile bit FTL1f_Auto;
//**************************
volatile bit FRL1f_RSFrame;
volatile bit FRL1f_RSMultiFrame;

volatile bit FTL1f_NoResp;           //znacznik czy oczekiwana jest odpowiedz

typedef struct {
                    /*unsigned _NoResp:1;           //znacznik czy oczekiwana jest odpowiedz
                    unsigned _NextFr:1;
                    unsigned _ZeroAdr:1;    
                    */
                    //dane aktualizowane w locie
                    BTY_VAL1 PIF;         //protected idetifier (header)

                    unsigned char NAD;           //address
                    unsigned char PCI;           //type+length(SID+DATA)
                    unsigned char PCI_TYPE;      //typ ramki
                    unsigned char PCI_LENGTH;    //rozmiar ramki
                    unsigned char SID;           //service identifier
                    unsigned char CSID;          //service identifier dla TCF
                    unsigned char IDN;           //identyfikator dla paczki WRD1,WRD2
                    unsigned char PFR;           //licznik powtorzen polecenia
                    unsigned char RND;           //liczba bajtow danych ramki                    
                    unsigned char RS_KOM;           //komenda paczki RS
                    unsigned char RS_ADR;           //adres paczki RS
                    unsigned char RS_IDN;           //idn paczki RS

                    unsigned char MSTIDO;       //identyfikator nadajnika (MASTER LIN2)                    
                    
                    WRD_VAL1 WRD0;               //pierwsze dwa bajty
                    WRD_VAL1 WRD1;               //drugie dwa bajty
                    WRD_VAL1 WRD2;               //ostatnie dwa bajty

                    WRD_VAL1 PCI_FCOUNT;         //indeks dla TFF i TFC
                    WRD_VAL1 START;              //pierwsze dwa bajty
                    WRD_VAL1 STOP;               //drugie dwa bajty
                    WRD_VAL1 FFPAR;                //parametr przesylany w multikomendzie
                    unsigned int bPCI_FCOUNT;
                    unsigned int imax;
                    unsigned char *tab;
}FRAME_LIN1;

typedef struct	{
                //parametry wysylki dla komendy cyklicznej
                union   {
                    unsigned char PAR0;
                    struct //__PACKED
                    {
                                unsigned _KSK_POB_CO:1;       //znacznik pobudzenia CO
                                unsigned _KSK_POB_CW:1;       //znacznik pobudzenia CW

                                unsigned _EN_POB_CO:1;        //znacznik aktywnosci pobudzenia CO 
                                unsigned _EN_POB_CW:1;        //znacznik aktywnosci pobudzenia CW              
                                unsigned _TRB_KSK:1;          //znacznik pracy w trybie kaskady
                                unsigned _NST_PCO:1;          //znacznik ze Par1 = nowa wartosc nastawy PCO
                                unsigned _TRB_STR:1;          //znacznik pracy w trybie sterowania z managera strefy lub regulatora RT
                                unsigned _DPGD:1;             //zdalna deaktywacja regulatora pogodowego
                    };
                };
                union   {
                    unsigned char PAR1;
                    struct //__PACKED
                    {
                                unsigned _p1b0:1;      
                                unsigned _p1b1:1;       
                                unsigned _p1b2:1;    
                                unsigned _p1b3:1;                
                                unsigned _p1b4:1;
                                unsigned _p1b5:1;
                                unsigned _p1b6:1;
                                unsigned _p1b7:1;
                    };
                };
}CMSG_PAR;
//----------------------------------------------------------
#endif /*_LIN1SLAV_TPH*/
