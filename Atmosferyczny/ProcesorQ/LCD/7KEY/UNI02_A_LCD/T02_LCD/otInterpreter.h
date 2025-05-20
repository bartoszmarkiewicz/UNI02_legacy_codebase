/*otInterpreter.h*/
#ifndef	_OTI_H
#define	_OTI_H
#include "ot.h"

#define _OT_VER 4.0
//----------------------------------------------------------
//MASTER TO SLAVE Message type
#define _READ_OT_DATA           0
#define _WRITE_OT_DATA          1
#define _INVALID_OT_DATA        2
#define _UNCOWN_OT_DATA         3
//----------------------------------------------------------
//SLAVE TO MASTER Message type
#define _READ_ACK_OT_DATA       4
#define _WRITE_ACK_OT_DATA      5
#define _INVALID_ACK_OT_DATA    6
#define _UNKOWN_ACK_OT_DATA     7
//----------------------------------------------------------
unsigned char OTMSGReadData(void);
unsigned char OTMSGWriteData(void);
signed int DataToF88(float dt);
float F88ToData(signed int dt);
extern void BitSet2(unsigned char *data, const unsigned char nbit,const unsigned char bt);
extern void BitSet3(volatile unsigned char *data, const unsigned char nbit,const unsigned char bt);
extern void StartRTdS(const unsigned char Index);
//----------------------------------------------------------
//----------------------------------------------------------
#endif		/*_OTI_H*/
