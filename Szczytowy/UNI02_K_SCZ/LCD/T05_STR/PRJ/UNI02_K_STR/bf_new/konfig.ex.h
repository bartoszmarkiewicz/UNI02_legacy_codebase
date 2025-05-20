#include "konfig.tp.h"
void MKonfiguracja(void);
void RdEEParam(void);

void SetPFN(void);
void SetMinMaxPCO(void);
void SetMinMaxPGD(void);
void ChkPCO(void);
void ChkPGD(void);



unsigned char ReadEEPCW(void);
void WriteEEPCW(void);
unsigned char ReadEEPCO(void);
void WriteEEPCO(void);
unsigned char ReadEEMXCO(void);
void WriteEEMXCO(void);
unsigned char ReadEEPPGD(void);
void WriteEEPPGD(void);
unsigned char ReadEEECO(void);
void WriteEEECO(void);
void WriteEEPFN(void);
unsigned char ReadEEPFN(void);
unsigned char ReadEEPopPFN(void);
void WriteEEALIN(unsigned char d);
unsigned char ReadEEALIN(void);
void ClearEETA(void);

void WriteEEVNT0(unsigned char v,unsigned char p);
unsigned char ReadEEVNT0_V();
unsigned char  ReadEEVNT0_P();
void WriteEEVNT1(unsigned char v,unsigned char p);
unsigned char ReadEEVNT1_V();
unsigned char ReadEEVNT1_P();
void WriteEEVNT2(unsigned char v,unsigned char p);
unsigned char ReadEEVNT2_V();
unsigned char ReadEEVNT2_P();
void WriteEEVNT3(unsigned char v,unsigned char p);
unsigned char ReadEEVNT3_V();
unsigned char ReadEEVNT3_P();
void WriteEEVNT4(unsigned char v,unsigned char p);
unsigned char ReadEEVNT4_V();
unsigned char ReadEEVNT4_P();
void WriteEEVNT5(unsigned char v,unsigned char p);
unsigned char ReadEEVNT5_V();
unsigned char ReadEEVNT5_P();
void WriteEEVNT6(unsigned char v,unsigned char p);
unsigned char ReadEEVNT6_V();
unsigned char ReadEEVNT6_P();
void WriteEEVNT7(unsigned char v,unsigned char p);
unsigned char ReadEEVNT7_V();
unsigned char ReadEEVNT7_P();

void Write2EEVNT0(unsigned int v,unsigned int p);
unsigned int Read2EEVNT0_V();
unsigned int  Read2EEVNT0_P();
void Write2EEVNT1(unsigned int v,unsigned int p);
unsigned int Read2EEVNT1_V();
unsigned int Read2EEVNT1_P();
void Write2EEVNT2(unsigned int v,unsigned int p);
unsigned int Read2EEVNT2_V();
unsigned int Read2EEVNT2_P();
void Write2EEVNT3(unsigned int v,unsigned int p);
unsigned int Read2EEVNT3_V();
unsigned int Read2EEVNT3_P();
void Write2EEVNT4(unsigned int v,unsigned int p);
unsigned int Read2EEVNT4_V();
unsigned int Read2EEVNT4_P();
void Write2EEVNT5(unsigned int v,unsigned int p);
unsigned int Read2EEVNT5_V();
unsigned int Read2EEVNT5_P();
void Write2EEVNT6(unsigned int v,unsigned int p);
unsigned int Read2EEVNT6_V();
unsigned int Read2EEVNT6_P();
void Write2EEVNT7(unsigned int v,unsigned int p);
unsigned int Read2EEVNT7_V();
unsigned int Read2EEVNT7_P();

void Write2EEVNT8(unsigned int v,unsigned int p);
unsigned int Read2EEVNT8_V();
unsigned int Read2EEVNT8_P();
void Write2EEVNT9(unsigned int v,unsigned int p);
unsigned int Read2EEVNT9_V();
unsigned int Read2EEVNT9_P();

unsigned char ReadBtyHNPZ(void);
unsigned char ReadBtyHFLG(void);
unsigned char ReadBtyHREF(const unsigned char ind);
unsigned char ReadBtyEEHIST(const unsigned char ind);
unsigned char ReadBtyNZSTAT(const unsigned char ind);
void WriteBtyNZSTAT(const unsigned char ind, const unsigned char dt);
unsigned char ReadBtySTAT(const unsigned char nr, const unsigned char ind);
void WriteBtySTAT(const unsigned char nr, const unsigned char ind, const unsigned char dt);
void WriteEESTAT(const unsigned char nr);
void CopyEESTAT(const unsigned char nr0,const unsigned char nr1);

void FDataToBtyF88(const float dt,volatile unsigned char *lo,volatile unsigned char *hi);
float BtyF88ToFData(const unsigned char lo, const unsigned char hi);

unsigned int GetRGDZ(void);
void SetRGDZ(const unsigned int dt);

void EnKonf(void);