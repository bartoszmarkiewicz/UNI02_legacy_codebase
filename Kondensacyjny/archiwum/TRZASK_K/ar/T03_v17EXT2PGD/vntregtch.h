/*vntregtch.h*/
#ifndef	_VNTTCH_H
#define	_VNTTCH_H
//----------------------------------------------------------
#include "vntregtch.tp.h"		//typy danych
//----------------------------------------------------------
	//void FPmVNT(unsigned char doc);
	//void RDtV(void);
	void ClrDtV(void);
	void SetNstVNT(unsigned char doc);
	bit Vrosnie(const unsigned char t);
	bit Vmaleje(const unsigned char t);
	bit Vstoi(void);
	bit Vnizsza(const unsigned char t);
	bit Vwyzsza(const unsigned char t);
	bit Vrowna(void);	
	extern bit VTrosnacy(const unsigned char t);
	extern bit VTmalejacy(const unsigned char t);
	extern bit VTzerowy(void);

	extern void StartRTdS(const unsigned char Index);
	extern unsigned char RTdS(const unsigned char Index);
	extern void StartRTcS(const unsigned char Index);
	extern unsigned char RTcS(const unsigned char Index);
	extern void WritePWM(const unsigned char tpwm);
	extern void WritePWM2(const unsigned char tpwm);
	extern unsigned char ProcToPmpPwm(unsigned char prc);

	extern void ModParam(void);
	extern void PrintErr(const unsigned char kod, const unsigned char pls);
	extern void ErrPTG(void);

	extern void WriteEEVNT0(unsigned char v,unsigned char p);
	extern void WriteEEVNT1(unsigned char v,unsigned char p);
	extern void WriteEEVNT2(unsigned char v,unsigned char p);
	extern void WriteEEVNT3(unsigned char v,unsigned char p);
	extern void WriteEEVNT4(unsigned char v,unsigned char p);
	extern void WriteEEVNT5(unsigned char v,unsigned char p);
	extern void WriteEEVNT6(unsigned char v,unsigned char p);
	extern void WriteEEVNT7(unsigned char v,unsigned char p);

	extern unsigned char ReadEEVNT0_V();
	extern unsigned char ReadEEVNT1_V();
	extern unsigned char ReadEEVNT2_V();
	extern unsigned char ReadEEVNT3_V();
	extern unsigned char ReadEEVNT4_V();
	extern unsigned char ReadEEVNT5_V();
	extern unsigned char ReadEEVNT6_V();
	extern unsigned char ReadEEVNT7_V();

	extern unsigned char ReadEEVNT0_P();
	extern unsigned char ReadEEVNT1_P();
	extern unsigned char ReadEEVNT2_P();
	extern unsigned char ReadEEVNT3_P();
	extern unsigned char ReadEEVNT4_P();
	extern unsigned char ReadEEVNT5_P();
	extern unsigned char ReadEEVNT6_P();
	extern unsigned char ReadEEVNT7_P();

	extern void WriteEEPFN(void);
	extern void EnKonf(void);
	extern void DsKonf(void);
	extern void OdczytDanychKalibracji();
	extern bit GetBit2(const unsigned char data,const unsigned char nbit);
	extern void BitSet2(unsigned char *data, const unsigned char nbit,const unsigned char bt);
    
    extern void ClrTachoDta(void);
    extern void ToWriteESTAT(void);
    
    extern void PomALL(void);
	//extern unsigned char ReadEEMDMOD(void);
	//extern unsigned char ReadEEMSTART(void);
	//extern unsigned char ReadEEMAXCO(void);
	//extern unsigned char ReadEEMAXCW(void);

//----------------------------------------------------------
//----------------------------------------------------------
#endif		/*_VNTTCH_H*/