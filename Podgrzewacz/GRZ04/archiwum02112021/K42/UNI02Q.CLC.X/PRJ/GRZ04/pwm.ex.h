#include "pwm.tp.h"

void InitDtaPWM(void);
void ModParam(const unsigned char tpk,const unsigned char tpg);
void WritePWM(const unsigned char tpwm);
void WriteOPWM(unsigned char tpwm);
void MocStart(unsigned char nst);
void MocMaksCW(unsigned char nst);
void inline IntPWM(void);

void inline IntPWM6(void);
unsigned char MocModToVNT();
void ProcToVNT(const unsigned char pv);
void SterownikVNT(void);
void WritePWM3H(const unsigned int tpwm);

unsigned char MocMod(void);
unsigned char VNTtoPPWM(unsigned char dvnt);
unsigned char MPPWMtoVNT(unsigned char dpwm);
unsigned int MProcToVNT(void);
unsigned int SetVNT(unsigned char pv);
unsigned char ProcToMOD(unsigned char prc);
unsigned char ProcToMOD2(unsigned char prc);
unsigned char GetModSTR(unsigned char ln, unsigned char pwr);
unsigned char GetVntSTR(unsigned char ln, unsigned char pwr);
unsigned char VNTtoVPROC(void);