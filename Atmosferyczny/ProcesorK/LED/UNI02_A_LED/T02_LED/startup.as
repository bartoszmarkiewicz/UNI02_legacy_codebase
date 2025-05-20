
	; Microchip MPLAB XC8 C Compiler V1.12
	; Copyright (C) 1984-2012 HI-TECH Software

	; Auto-generated runtime startup code for final link stage.

	;
	; Compiler options:
	;
	; -oUNISTR.cof -mUNISTR.map --summary=default --output=default ac.p1 \
	; bit.p1 i2c_s.p1 isr.p1 main.p1 osc.p1 pomiar.p1 port.p1 print.p1 \
	; pwm.p1 pwron.p1 rgl.p1 rtimer.p1 timer1.p1 konfig.p1 uart9b.p1 \
	; pompa.p1 wsw.p1 keypad.p1 readdtal.p1 konsola.p1 --chip=18F46K20 -P \
	; --runtime=default --opt=default -N-1 -D__DEBUG=1 -g --asmlist \
	; --errformat=Error   [%n] %f; %l.%c %s --msgformat=Advisory[%n] %s \
	; --warnformat=Warning [%n] %f; %l.%c %s
	;


	processor	18F46K20

	GLOBAL	_main,start
	FNROOT	_main

	pic18cxx	equ	1

	psect	config,class=CONFIG,delta=1
	psect	idloc,class=IDLOC,delta=1
	psect	const,class=CODE,delta=1,reloc=2
	psect	smallconst,class=SMALLCONST,delta=1,reloc=2
	psect	mediumconst,class=MEDIUMCONST,delta=1,reloc=2
	psect	rbss,class=COMRAM,space=1
	psect	bss,class=RAM,space=1
	psect	rdata,class=COMRAM,space=1
	psect	irdata,class=CODE,space=0,reloc=2
	psect	bss,class=RAM,space=1
	psect	data,class=RAM,space=1
	psect	idata,class=CODE,space=0,reloc=2
	psect	nvrram,class=COMRAM,space=1
	psect	nvbit,class=COMRAM,bit,space=1
	psect	temp,ovrld,class=COMRAM,space=1
	psect	struct,ovrld,class=COMRAM,space=1
	psect	rbit,class=COMRAM,bit,space=1
	psect	bigbss,class=BIGRAM,space=1
	psect	bigdata,class=BIGRAM,space=1
	psect	ibigdata,class=CODE,space=0,reloc=2
	psect	farbss,class=FARRAM,space=0,reloc=2,delta=1
	psect	fardata,class=FARRAM,space=0,reloc=2,delta=1
	psect	ifardata,class=CODE,space=0,reloc=2,delta=1

	psect	reset_vec,class=CODE,delta=1,reloc=2
	psect	powerup,class=CODE,delta=1,reloc=2
	psect	intcode,class=CODE,delta=1,reloc=2
	psect	intcode_body,class=CODE,delta=1,reloc=2
	psect	intcodelo,class=CODE,delta=1,reloc=2
	psect	intret,class=CODE,delta=1,reloc=2
	psect	intentry,class=CODE,delta=1,reloc=2

	psect	intsave_regs,class=BIGRAM,space=1
	psect	init,class=CODE,delta=1,reloc=2
	psect	text,class=CODE,delta=1,reloc=2
GLOBAL	intlevel0,intlevel1,intlevel2
intlevel0:
intlevel1:
intlevel2:
GLOBAL	intlevel3
intlevel3:
	psect	end_init,class=CODE,delta=1,reloc=2
	psect	clrtext,class=CODE,delta=1,reloc=2

	psect	eeprom_data,class=EEDATA,delta=1
	psect	smallconst
	GLOBAL	__smallconst
__smallconst:
	psect	mediumconst
	GLOBAL	__mediumconst
__mediumconst:
wreg	EQU	0FE8h
fsr0l	EQU	0FE9h
fsr0h	EQU	0FEAh
fsr1l	EQU	0FE1h
fsr1h	EQU	0FE2h
fsr2l	EQU	0FD9h
fsr2h	EQU	0FDAh
postinc0	EQU	0FEEh
postdec0	EQU	0FEDh
postinc1	EQU	0FE6h
postdec1	EQU	0FE5h
postinc2	EQU	0FDEh
postdec2	EQU	0FDDh
tblptrl	EQU	0FF6h
tblptrh	EQU	0FF7h
tblptru	EQU	0FF8h
tablat		EQU	0FF5h

	PSECT	ramtop,class=RAM
	GLOBAL	__S1			; top of RAM usage
	GLOBAL	__ramtop
	GLOBAL	__LRAM,__HRAM
__ramtop:

	psect	reset_vec
reset_vec:
	; No powerup routine
	global start

; jump to start
	goto start
	GLOBAL __accesstop
__accesstop EQU 96


	psect	init
start:
	psect	end_init
	global start_initialization
	goto start_initialization	;jump to C runtime clear & initialization

; Padding undefined space
	psect	config,class=CONFIG,delta=1
		org 0x0
		db 0xFF

; Config register CONFIG1H @ 0x300001
;	Internal/External Oscillator Switchover bit
;	IESO = ON, Oscillator Switchover mode enabled
;	Oscillator Selection bits
;	FOSC = INTIO67, Internal oscillator block, port function on RA6 and RA7
;	Fail-Safe Clock Monitor Enable bit
;	FCMEN = ON, Fail-Safe Clock Monitor enabled

	psect	config,class=CONFIG,delta=1
		org 0x1
		db 0xC8

; Config register CONFIG2L @ 0x300002
;	Brown-out Reset Enable bits
;	BOREN = 0x3, unprogrammed default
;	Brown Out Reset Voltage bits
;	BORV = 27, VBOR set to 2.7 V nominal
;	Power-up Timer Enable bit
;	PWRT = 0x1, unprogrammed default

	psect	config,class=CONFIG,delta=1
		org 0x2
		db 0xF

; Config register CONFIG2H @ 0x300003
;	Watchdog Timer Postscale Select bits
;	WDTPS = 128, 1:128
;	Watchdog Timer Enable bit
;	WDTEN = 0x1, unprogrammed default

	psect	config,class=CONFIG,delta=1
		org 0x3
		db 0xF

; Padding undefined space
	psect	config,class=CONFIG,delta=1
		org 0x4
		db 0xFF

; Config register CONFIG3H @ 0x300005
;	CCP2 MUX bit
;	CCP2MX = 0x1, unprogrammed default
;	PORTB A/D Enable bit
;	PBADEN = OFF, PORTB<4:0> pins are configured as digital I/O on Reset
;	Low-Power Timer1 Oscillator Enable bit
;	LPT1OSC = 0x0, unprogrammed default
;	MCLR Pin Enable bit
;	MCLRE = OFF, RE3 input pin enabled; MCLR disabled
;	HFINTOSC Fast Start-up
;	HFOFST = 0x1, unprogrammed default

	psect	config,class=CONFIG,delta=1
		org 0x5
		db 0x9

; Config register CONFIG4L @ 0x300006
;	Background Debugger Enable bit
;	DEBUG = 0x1, unprogrammed default
;	Stack Full/Underflow Reset Enable bit
;	STVREN = 0x1, unprogrammed default
;	Extended Instruction Set Enable bit
;	XINST = ON, Instruction set extension and Indexed Addressing mode enabled
;	Single-Supply ICSP Enable bit
;	LVP = OFF, Single-Supply ICSP disabled

	psect	config,class=CONFIG,delta=1
		org 0x6
		db 0xC1

; Padding undefined space
	psect	config,class=CONFIG,delta=1
		org 0x7
		db 0xFF

; Config register CONFIG5L @ 0x300008
;	Code Protection Block 0
;	CP0 = ON, Block 0 (000800-003FFFh) code-protected
;	Code Protection Block 1
;	CP1 = ON, Block 1 (004000-007FFFh) code-protected
;	Code Protection Block 2
;	CP2 = ON, Block 2 (008000-00BFFFh) code-protected
;	Code Protection Block 3
;	CP3 = ON, Block 3 (00C000-00FFFFh) code-protected

	psect	config,class=CONFIG,delta=1
		org 0x8
		db 0x0

; Config register CONFIG5H @ 0x300009
;	unspecified using default value

	psect	config,class=CONFIG,delta=1
		org 0x9
		db 0xC0

; Config register CONFIG6L @ 0x30000A
;	Write Protection Block 0
;	WRT0 = ON, Block 0 (000800-003FFFh) write-protected
;	Write Protection Block 1
;	WRT1 = ON, Block 1 (004000-007FFFh) write-protected
;	Write Protection Block 2
;	WRT2 = ON, Block 2 (008000-00BFFFh) write-protected
;	Write Protection Block 3
;	WRT3 = ON, Block 3 (00C000h-00FFFFh) write-protected

	psect	config,class=CONFIG,delta=1
		org 0xA
		db 0x0

; Config register CONFIG6H @ 0x30000B
;	Boot Block Write Protection bit
;	WRTB = 0x1, unprogrammed default
;	Configuration Register Write Protection bit
;	WRTC = ON, Configuration registers (300000-3000FFh) write-protected
;	Data EEPROM Write Protection bit
;	WRTD = 0x1, unprogrammed default

	psect	config,class=CONFIG,delta=1
		org 0xB
		db 0xC0


psect comram,class=COMRAM,space=1
psect abs1,class=ABS1,space=1
psect bigram,class=BIGRAM,space=1
psect ram,class=RAM,space=1
psect bank0,class=BANK0,space=1
psect bank1,class=BANK1,space=1
psect bank2,class=BANK2,space=1
psect bank3,class=BANK3,space=1
psect bank4,class=BANK4,space=1
psect bank5,class=BANK5,space=1
psect bank6,class=BANK6,space=1
psect bank7,class=BANK7,space=1
psect bank8,class=BANK8,space=1
psect bank9,class=BANK9,space=1
psect bank10,class=BANK10,space=1
psect bank11,class=BANK11,space=1
psect bank12,class=BANK12,space=1
psect bank13,class=BANK13,space=1
psect bank14,class=BANK14,space=1
psect bank15,class=BANK15,space=1
psect sfr,class=SFR,space=1


	end	start
