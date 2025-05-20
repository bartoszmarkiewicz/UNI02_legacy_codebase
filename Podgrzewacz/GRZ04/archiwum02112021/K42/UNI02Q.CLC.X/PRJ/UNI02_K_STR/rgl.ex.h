#include "rgl.tp.h"
#include "global.h"

void Moduluj(void);
void RestartMod(void);

#if _HIRESVNT==0
unsigned char ProcToMod(unsigned char prvnt);
#else
unsigned int ProcToMod(unsigned char prvnt);
unsigned int ProcToRegMod(unsigned char ref, unsigned char prvnt);

#endif
bit Trosnie(const unsigned char t);
bit Tmaleje(const unsigned char t);
bit Tstoi(void);
bit Tnizsza(const unsigned char t);
bit Twyzsza(const unsigned char t);
bit Trowna(void);
bit TFrosnie(const unsigned char t);
bit TFmaleje(const unsigned char t);
bit TFstoi(void);

