/*isr.h*/

#ifndef	_ISR_H
#define	_ISR_H
//----------------------------------------------------------
//timer1.c
extern void ReloadTimer1(void);
//timer3.c
extern void ReloadTimer3(void);
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
//uart.c
extern void IntUSARTTR(void);
extern void IntUSARTRC(void);
//mgs_rs.c
extern void IncRSTimer(void);
//pomiar.c
extern void IncPmrTimer(void);
//czcw.c
extern void IntCzCw(void);
//pwm.C
extern void IntPWM(void);
//tacho.c
extern void IntTacho(void);
extern unsigned char IntINT0(void);
extern void IncTimTacho(void);
//----------------------------------------------------------
#endif		/*_ISR*/
