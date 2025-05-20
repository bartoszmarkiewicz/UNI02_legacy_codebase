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
//i2c_m.c
extern void IncI2CTimer(void);
extern void CpSSPIF(void);
extern void CpBCLIF(void);
//imy.c
extern void IncIMYTimer(void);
//imz.c
extern void IncIMZTimer(void);
//buzzer.c
extern void IncBuzzTimer(void);
//main.c
extern void IncOscWU(void);
//uart.c
extern void IntUSARTTR(void);
extern void IntUSARTRC(void);
//smgs_rs.c
//extern void IncRSTimer(void);
//extern void IncRSTimer2(void);
//lcdHD1.c
extern void UpdateWSW(void);
//i2c_s.c
extern void I2CSlaveTR(void);
extern void FormatDataIMY(void);
//keypad.c
extern void IncKeyTimer(void);
//mgs_rs9b.c
extern void IncRSTimer(void);
//kolejka.c
void IncRingTimer(void);
//pwm2.c
void IntPWM2(void);
//timer3.c
extern void ReloadTimer3(void);

//LIN2Master.c, LIN2Slave.c
extern void IntLIN1Tim(void);
extern void IntUSART1STR(void);
extern void IntUSART1SRC(void);

extern void IncKSKTimer(void);

//timer0.c
extern void IntTimer0(void);
extern void ReloadTMR0(volatile unsigned int tm);
//ot.c
extern void IntMillis(void);
extern void IntOTTimeout(void);
extern void IntINT0(void);
extern void IntINT1(void);
extern void IntOTDeamon(void);

//----------------------------------------------------------
#endif		/*_ISR*/
