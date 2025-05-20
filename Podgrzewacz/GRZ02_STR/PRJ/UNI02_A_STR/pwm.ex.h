#include "pwm.tp.h"

void InitDtaPWM(void);
void inline IntPWM(void);
void inline IntPWM2(void);

void ModParam(const unsigned char tpk,const unsigned char tpg);
void WritePWM(const unsigned char tpwm);
void WritePWM2(const unsigned char tpwm);
void WriteOPWM(unsigned char tpwm);
unsigned char ModToProc(unsigned char wrt);
unsigned char MocProc(unsigned char nst);
void MocStart(unsigned char nst);
void MocMaksCO(unsigned char nst);
void MocMaksCW(unsigned char nst);



