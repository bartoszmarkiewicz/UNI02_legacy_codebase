#include "vntregtch.tp.h"

unsigned char KalibracjaVNT(void);
void SetDefFan(const unsigned char tp);
void OdczytDanychKalibracji();
unsigned char VNTToProc(unsigned char wrt);
unsigned char CalkVNTtoPWM(unsigned char nst);
unsigned char CalkVNTtoPopPWM(unsigned char nst,unsigned char pop);
unsigned int CalkVNTtoPWMH(unsigned int nst);
unsigned int CalkVNTtoPopPWMH(unsigned char nst,unsigned char pop);
unsigned char CalkTimStr(unsigned char vstr);
unsigned char ProcToVNT(unsigned char wrt);
unsigned char DPWMtoVNT(unsigned char dpwm);