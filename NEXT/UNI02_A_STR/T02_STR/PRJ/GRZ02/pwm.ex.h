#include "pwm.tp.h"

void InitDtaPWM(void);
void ModParam(const unsigned char tpk,const unsigned char tpg);
void WritePWM(const unsigned char tpwm);
void WriteOPWM(unsigned char tpwm);
void MocStart(unsigned char nst);
void MocMaksCW(unsigned char nst);
void inline IntPWM(void);



