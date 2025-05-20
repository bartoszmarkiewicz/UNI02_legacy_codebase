#include "keypad.tp.h"		//typy danych

void SetBufKey1(void);
void ResBufKey1(void);
unsigned char PresKey1(unsigned char k);

void SetBufKey2(void);
void ResBufKey2(void);
unsigned char PresKey2(unsigned char k);

unsigned char DoublePresKey(unsigned char k);
unsigned char ExPresKey(unsigned char k,unsigned char dTim,unsigned char pTim);
unsigned char ExPresKey2(unsigned char k,unsigned char dTim,unsigned char pTim);
unsigned char ExPresKey2B(unsigned char k,unsigned char dTim,unsigned char pTim);