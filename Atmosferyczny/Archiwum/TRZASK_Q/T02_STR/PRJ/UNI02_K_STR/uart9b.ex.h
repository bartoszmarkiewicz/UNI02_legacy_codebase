#include "uart9b.tp.h"

void InitDtaUART1(void);
void inline IntUSARTTR(void);
void inline IntUSARTRC(void);
void StartUSARTRC(void);
void StartUSARTTR(void);
void StopUSARTTR(void);
void ClrRSRBuf(void);
void ClrRSTBuf(void);
void SetRSTBuf(const unsigned char indeks,const unsigned char data);
unsigned char GetRSTBuf(const unsigned char indeks);
unsigned char GetRSRBuf(const unsigned char indeks);
unsigned char LiczRSCHK(const unsigned char *bf,const unsigned char nData);
void SetRSCHK(unsigned char nData);
unsigned char VerifyRSACK(void);
unsigned char VerifyRSCHK(unsigned char nData);
unsigned char GetRSCHK(void);
unsigned char SetNDatCHK(const unsigned char Data);
void SetExNDatCHK(const unsigned char ndt);
unsigned char GetNDat(void);
unsigned char GetExNDat(void);

inline void UART1_SetContinuousReceive_Ex(bool dt);
inline void UART1_SetContinuousReceive_Ex2(bool dt);
inline bool UART1_TransmitStat_Ex();
inline bool UART1_ReceiveStat_Ex();
inline bool UART1_IsTXIE_Ex();
