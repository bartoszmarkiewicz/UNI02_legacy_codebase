
	; HI-TECH PICC-18 STD COMPILER (Microchip PIC micro) V9.51PL3
	; Copyright (C) 1984-2009 HI-TECH Software
	;Serial no. HCPIC18-904077389

	; Auto-generated runtime startup code for final link stage.

	;
	; Compiler options:
	;
	; -oUNISTR.cof -mUNISTR.map --summary=default --output=default bit.obj \
	; isr.obj main.obj osc.obj pwron.obj rtimer.obj timer1.obj ascii.obj \
	; chkcitt.obj cvbc.obj port.obj uart9c.obj --chip=18F63J90 -P \
	; --runtime=default --opt=default -D__DEBUG=1 -g --asmlist \
	; --errformat=Error   [%n] %f; %l.%c %s --msgformat=Advisory[%n] %s \
	; --warnformat=Warning [%n] %f; %l.%c %s
	;


	GLOBAL	_main,start,_exit
	FNROOT	_main
	FNCONF	param,?a,?

	pic18cxx	equ	1

	psect	jflash_buffer,class=CODE,reloc=0x400,space=0
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
	dw 0xFBBF

; User config settings for word 2
;
	psect config
	org 2
	dw 0xF77D

; User config settings for word 3
;
	psect config
	org 4
	dw 0xFFFF
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
; fardata psect  - 0 bytes to load

; farbss psect  - 0 bytes to clear

; clearing bss psect - 55 bytes to clear
GLOBAL	__Lbss
	lfsr	0,__LRAM
	movlw	55	; loop counter
clear_bss_psect:
	clrf	postinc0	; clear, increment FSR0
	decf	wreg	; decrement loop counter
	bnz	clear_bss_psect

; clearing rbss psect - 69 bytes to clear
GLOBAL	__Lrbss
	lfsr	0,__Lrbss
	movlw	69	; loop counter
clear_rbss_psect:
	clrf	postinc0	; clear, increment FSR0
	decf	wreg	; decrement loop counter
	bnz	clear_rbss_psect

; clearing rbit psect - 0 bytes to clear

; clearing bigbss psect - 69 bytes to clear
GLOBAL	__Lbigbss
	lfsr	0,__Lbigbss
	movlw	69	; loop counter
clear_bigbss_psect:
	clrf	postinc0	; clear, increment FSR0
	decf	wreg	; decrement loop counter
	bnz	clear_bigbss_psect

; bigdata psect - 9 bytes to load
GLOBAL	__Lbigdata,__Libigdata
	lfsr	0,__Lbigdata
	; load TBLPTR registers with __Libigdata (40Ah)
	movlw	low (__Libigdata)
	movwf	tblptrl
	movlw	low (__Libigdata/100h)
	movwf	tblptrh
	lfsr	1,9	; loop variable
; Copy the ROM data image to destination in RAM
copy_data:
	tblrd	*+
	movff	tablat,postinc0
	movf	postdec1,w	;decrement loop variable
	movf	fsr1l,w
	bnz	copy_data
	movf	fsr1h,w
	bnz	copy_data

; data psect - 0 bytes to load

; rdata psect - 0 bytes to load

	PSECT	end_init
	goto	_main		;go do the main stuff

	END	start
