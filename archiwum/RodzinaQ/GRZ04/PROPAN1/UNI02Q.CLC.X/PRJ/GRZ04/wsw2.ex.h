#include "wsw2.h"

void InitDtaWsw(void);
void inline RefreshWSW(void);

void WswOn(void);
void WswOff(void);
void Swsdid(unsigned char dtd);
void Rwsdid(unsigned char dtd);
void SetWbtyBit(unsigned char nrbty,  unsigned char nrbit, const unsigned char bt);
void SetData(WPack *wData,const unsigned char wdt1,const unsigned char wdt0);
void SetPoz(const unsigned char nrkol,const unsigned char nrpoz);
void ResetPoz(const unsigned char nrkol,const unsigned char nrpoz);
void ResetKol(const unsigned char nrkol);
void RestartKol(const unsigned char nrkol);
void AllKolToFirstPoz(void);
void SetStatus(WPack *wData,const unsigned int wst,const unsigned int mask);
void Swbdid(WPack *wData,unsigned char dtd);
void Rwbdid(WPack *wData,unsigned char dtd);


