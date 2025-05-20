/*isr.h*/

#ifndef	_ISR_H
#define	_ISR_H
//----------------------------------------------------------
//timer1.c
extern void ReloadTimer1(void);
//rtimer.c
extern void IncRTimer(void);
//port.c
extern void StabPort(void);
//mux.c
extern void SetResMux(void);
//wsw.c
extern void RefreshWSW(void);
//i2c_s.c
extern void I2CSlaveTR(void);
extern void FormatDataIMY(void);
//smgs_rs9b.c
extern void IntUSARTTR(void);
extern void IntUSARTRC(void);
extern void IncRSTimer(void);
extern void IncRSTimer2(void);
//pomiar.c
extern void IncPmrTimer(void);
//czcw.c
extern void IntCzCw(void);
//pwm.C
extern void IntPWM(void);
extern void IntPWM2(void);
//keypad.c
extern void IncKeyTimer(void);
//----------------------------------------------------------
#endif		/*_ISR*/
