
	; HI-TECH PICC-18 STD COMPILER (Microchip PIC micro) V9.51PL3
	; Copyright (C) 1984-2009 HI-TECH Software
	;Serial no. HCPIC18-904077389

	; Auto-generated runtime startup code for final link stage.

	;
	; Compiler options:
	;
	; -oUNISTR.cof -mUNISTR.map --summary=default --output=default ac.obj \
	; bit.obj i2c_s.obj isr.obj main.obj osc.obj pomiar.obj port.obj \
	; print.obj pwm.obj pwron.obj rgl.obj rtimer.obj timer1.obj konfig.obj \
	; smgs_rs9b.obj uart9b.obj pompa.obj --chip=18F46K20 -P \
	; --runtime=default --opt=default -D__DEBUG=1 -g --asmlist \
	; --errformat=Error   [%n] %f; %l.%c %s --msgformat=Advisory[%n] %s \
	; --warnformat=Warning [%n] %f; %l.%c %s
	;


	GLOBAL	_main,start,_exit
	FNROOT	_main
	FNCONF	param,?a,?

	pic18cxx	equ	1

	psect	config,class=CONFIG,delta=1
	psect	idloc,class=IDLOC,delta=1
	psect	eeprom_data,class=EEDATA,delta=1
	psect	const,class=CODE,delta=1,reloc=2
	psect	rbss,class=COMRAM,space=1
	psect	bss,class=RAM,space=1
	psect	rdata,class=COMRAM,space=1
	psect	irdata,class=CODE,space=0,reloc=2
	psect	bss,class=RAM,space=1
	psect	data,class=RAM,space=1
	psect	idata,class=CODE,space=0,reloc=2
	psect	nvram,class=NVRAM,space=1
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
	psect	intcodelo,class=CODE,delta=1,reloc=2
	psect	intret,class=CODE,delta=1,reloc=2
	psect	intentry,class=CODE,delta=1,reloc=2

	psect	intsave_regs,class=BIGRAM,space=1
	psect	lowtext,class=CODE,delta=1,reloc=2,limit=0ffffh
	psect	init,class=CODE,delta=1,reloc=2
	psect	text,class=CODE,delta=1,reloc=2
global	intlevel0,intlevel1
intlevel0:
intlevel1:
	psect	end_init,class=CODE,delta=1,reloc=2
	psect	clrtext,class=CODE,delta=1,reloc=2


; User config settings for word 1
;
	psect config
	org 0
	dw 0xF8FF

; User config settings for word 2
;
	psect config
	org 2
	dw 0xEFEF

; User config settings for word 3
;
	psect config
	org 4
	dw 0x79FF

; User config settings for word 4
;
	psect config
	org 6
	dw 0xFFBB

; User config settings for word 5
;
	psect config
	org 8
	dw 0xFFF0

; User config settings for word 6
;
	psect config
	org 10
	dw 0xDFF0
	psect	param,class=RAM,space=1
wreg	EQU	0FE8h
fsr0l	EQU	0FE9h
fsr0h	EQU	0FEAh
fsr1l	EQU	0FE1h
fsr1h	EQU	0FE2h
fsr2l	EQU	0FD9h
fsr2h	EQU	0FDAh
indf0	EQU	0FEFh
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
prodl		EQU	0FF3h

	PSECT	ramtop,class=RAM
	global	__S1			; top of RAM usage
	global	__ramtop
	global	__LRAM,__HRAM
__ramtop:

	PSECT	reset_vec
	goto	start

	PSECT	init
start:
_exit:
	clrf	tblptru ; bootloader safety check

; fardata psect  - 0 bytes to load

; farbss psect  - 0 bytes to clear

; clearing bss, rbit psect - 427 bytes to clear
GLOBAL	__Lrbit
	lfsr	0,(__Lrbit)/8
	lfsr	1,427	; loop variable
; Clear these memory locations
clear_ram:
	clrf	postinc0	;clear, increment FSR0
	movf	postdec1,w	;decrement loop variable
	movf	fsr1l,w
	bnz	clear_ram
	movf	fsr1h,w
	bnz	clear_ram

; clearing rbss psect - 25 bytes to clear
GLOBAL	__Lrbss
	lfsr	0,__Lrbss
	movlw	25	; loop counter
clear_rbss_psect:
	clrf	postinc0	; clear, increment FSR0
	decf	wreg	; decrement loop counter
	bnz	clear_rbss_psect

; clearing bigbss psect - 156 bytes to clear
GLOBAL	__Lbigbss
	lfsr	0,__Lbigbss
	movlw	156	; loop counter
clear_bigbss_psect:
	clrf	postinc0	; clear, increment FSR0
	decf	wreg	; decrement loop counter
	bnz	clear_bigbss_psect

; bigdata psect - 0 bytes to load

; data psect - 0 bytes to load

; rdata psect - 0 bytes to load

	clrf	tblptru	; Upper address may be non-zero, maintain clear in cp16 mode
	PSECT	end_init
	goto	_main		;go do the main stuff

	END	start
