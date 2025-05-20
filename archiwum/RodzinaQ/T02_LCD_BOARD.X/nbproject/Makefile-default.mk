#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/T02_LCD_BOARD.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/T02_LCD_BOARD.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=--mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS

else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=bit.c i2c_s.c isr.c keypad.c kolejka.c konfig3.c konsola.c lled.c main.c osc.c port.c print.c pwm2.c pwron.c readdtal.c rtimer.c timer1.c wsw.c timer3.c auart9b.c amgs_rs9b.c uart9b.c mgs_rs9b.c LIN1SInterpreter.c LIN1Slave.c ac.c ot.c pomiar.c timer0.c otInterpreter.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/bit.p1 ${OBJECTDIR}/i2c_s.p1 ${OBJECTDIR}/isr.p1 ${OBJECTDIR}/keypad.p1 ${OBJECTDIR}/kolejka.p1 ${OBJECTDIR}/konfig3.p1 ${OBJECTDIR}/konsola.p1 ${OBJECTDIR}/lled.p1 ${OBJECTDIR}/main.p1 ${OBJECTDIR}/osc.p1 ${OBJECTDIR}/port.p1 ${OBJECTDIR}/print.p1 ${OBJECTDIR}/pwm2.p1 ${OBJECTDIR}/pwron.p1 ${OBJECTDIR}/readdtal.p1 ${OBJECTDIR}/rtimer.p1 ${OBJECTDIR}/timer1.p1 ${OBJECTDIR}/wsw.p1 ${OBJECTDIR}/timer3.p1 ${OBJECTDIR}/auart9b.p1 ${OBJECTDIR}/amgs_rs9b.p1 ${OBJECTDIR}/uart9b.p1 ${OBJECTDIR}/mgs_rs9b.p1 ${OBJECTDIR}/LIN1SInterpreter.p1 ${OBJECTDIR}/LIN1Slave.p1 ${OBJECTDIR}/ac.p1 ${OBJECTDIR}/ot.p1 ${OBJECTDIR}/pomiar.p1 ${OBJECTDIR}/timer0.p1 ${OBJECTDIR}/otInterpreter.p1
POSSIBLE_DEPFILES=${OBJECTDIR}/bit.p1.d ${OBJECTDIR}/i2c_s.p1.d ${OBJECTDIR}/isr.p1.d ${OBJECTDIR}/keypad.p1.d ${OBJECTDIR}/kolejka.p1.d ${OBJECTDIR}/konfig3.p1.d ${OBJECTDIR}/konsola.p1.d ${OBJECTDIR}/lled.p1.d ${OBJECTDIR}/main.p1.d ${OBJECTDIR}/osc.p1.d ${OBJECTDIR}/port.p1.d ${OBJECTDIR}/print.p1.d ${OBJECTDIR}/pwm2.p1.d ${OBJECTDIR}/pwron.p1.d ${OBJECTDIR}/readdtal.p1.d ${OBJECTDIR}/rtimer.p1.d ${OBJECTDIR}/timer1.p1.d ${OBJECTDIR}/wsw.p1.d ${OBJECTDIR}/timer3.p1.d ${OBJECTDIR}/auart9b.p1.d ${OBJECTDIR}/amgs_rs9b.p1.d ${OBJECTDIR}/uart9b.p1.d ${OBJECTDIR}/mgs_rs9b.p1.d ${OBJECTDIR}/LIN1SInterpreter.p1.d ${OBJECTDIR}/LIN1Slave.p1.d ${OBJECTDIR}/ac.p1.d ${OBJECTDIR}/ot.p1.d ${OBJECTDIR}/pomiar.p1.d ${OBJECTDIR}/timer0.p1.d ${OBJECTDIR}/otInterpreter.p1.d

# Object Files
OBJECTFILES=${OBJECTDIR}/bit.p1 ${OBJECTDIR}/i2c_s.p1 ${OBJECTDIR}/isr.p1 ${OBJECTDIR}/keypad.p1 ${OBJECTDIR}/kolejka.p1 ${OBJECTDIR}/konfig3.p1 ${OBJECTDIR}/konsola.p1 ${OBJECTDIR}/lled.p1 ${OBJECTDIR}/main.p1 ${OBJECTDIR}/osc.p1 ${OBJECTDIR}/port.p1 ${OBJECTDIR}/print.p1 ${OBJECTDIR}/pwm2.p1 ${OBJECTDIR}/pwron.p1 ${OBJECTDIR}/readdtal.p1 ${OBJECTDIR}/rtimer.p1 ${OBJECTDIR}/timer1.p1 ${OBJECTDIR}/wsw.p1 ${OBJECTDIR}/timer3.p1 ${OBJECTDIR}/auart9b.p1 ${OBJECTDIR}/amgs_rs9b.p1 ${OBJECTDIR}/uart9b.p1 ${OBJECTDIR}/mgs_rs9b.p1 ${OBJECTDIR}/LIN1SInterpreter.p1 ${OBJECTDIR}/LIN1Slave.p1 ${OBJECTDIR}/ac.p1 ${OBJECTDIR}/ot.p1 ${OBJECTDIR}/pomiar.p1 ${OBJECTDIR}/timer0.p1 ${OBJECTDIR}/otInterpreter.p1

# Source Files
SOURCEFILES=bit.c i2c_s.c isr.c keypad.c kolejka.c konfig3.c konsola.c lled.c main.c osc.c port.c print.c pwm2.c pwron.c readdtal.c rtimer.c timer1.c wsw.c timer3.c auart9b.c amgs_rs9b.c uart9b.c mgs_rs9b.c LIN1SInterpreter.c LIN1Slave.c ac.c ot.c pomiar.c timer0.c otInterpreter.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/T02_LCD_BOARD.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=18F66J90
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/bit.p1: bit.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/bit.p1.d 
	@${RM} ${OBJECTDIR}/bit.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"." --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/bit.p1 bit.c 
	@-${MV} ${OBJECTDIR}/bit.d ${OBJECTDIR}/bit.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/bit.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/i2c_s.p1: i2c_s.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/i2c_s.p1.d 
	@${RM} ${OBJECTDIR}/i2c_s.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"." --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/i2c_s.p1 i2c_s.c 
	@-${MV} ${OBJECTDIR}/i2c_s.d ${OBJECTDIR}/i2c_s.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/i2c_s.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/isr.p1: isr.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/isr.p1.d 
	@${RM} ${OBJECTDIR}/isr.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"." --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/isr.p1 isr.c 
	@-${MV} ${OBJECTDIR}/isr.d ${OBJECTDIR}/isr.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/isr.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/keypad.p1: keypad.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/keypad.p1.d 
	@${RM} ${OBJECTDIR}/keypad.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"." --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/keypad.p1 keypad.c 
	@-${MV} ${OBJECTDIR}/keypad.d ${OBJECTDIR}/keypad.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/keypad.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/kolejka.p1: kolejka.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/kolejka.p1.d 
	@${RM} ${OBJECTDIR}/kolejka.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"." --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/kolejka.p1 kolejka.c 
	@-${MV} ${OBJECTDIR}/kolejka.d ${OBJECTDIR}/kolejka.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/kolejka.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/konfig3.p1: konfig3.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/konfig3.p1.d 
	@${RM} ${OBJECTDIR}/konfig3.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"." --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/konfig3.p1 konfig3.c 
	@-${MV} ${OBJECTDIR}/konfig3.d ${OBJECTDIR}/konfig3.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/konfig3.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/konsola.p1: konsola.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/konsola.p1.d 
	@${RM} ${OBJECTDIR}/konsola.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"." --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/konsola.p1 konsola.c 
	@-${MV} ${OBJECTDIR}/konsola.d ${OBJECTDIR}/konsola.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/konsola.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lled.p1: lled.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/lled.p1.d 
	@${RM} ${OBJECTDIR}/lled.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"." --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/lled.p1 lled.c 
	@-${MV} ${OBJECTDIR}/lled.d ${OBJECTDIR}/lled.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lled.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/main.p1: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.p1.d 
	@${RM} ${OBJECTDIR}/main.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"." --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/main.p1 main.c 
	@-${MV} ${OBJECTDIR}/main.d ${OBJECTDIR}/main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/osc.p1: osc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/osc.p1.d 
	@${RM} ${OBJECTDIR}/osc.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"." --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/osc.p1 osc.c 
	@-${MV} ${OBJECTDIR}/osc.d ${OBJECTDIR}/osc.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/osc.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/port.p1: port.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/port.p1.d 
	@${RM} ${OBJECTDIR}/port.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"." --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/port.p1 port.c 
	@-${MV} ${OBJECTDIR}/port.d ${OBJECTDIR}/port.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/port.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/print.p1: print.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/print.p1.d 
	@${RM} ${OBJECTDIR}/print.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"." --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/print.p1 print.c 
	@-${MV} ${OBJECTDIR}/print.d ${OBJECTDIR}/print.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/print.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/pwm2.p1: pwm2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/pwm2.p1.d 
	@${RM} ${OBJECTDIR}/pwm2.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"." --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/pwm2.p1 pwm2.c 
	@-${MV} ${OBJECTDIR}/pwm2.d ${OBJECTDIR}/pwm2.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/pwm2.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/pwron.p1: pwron.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/pwron.p1.d 
	@${RM} ${OBJECTDIR}/pwron.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"." --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/pwron.p1 pwron.c 
	@-${MV} ${OBJECTDIR}/pwron.d ${OBJECTDIR}/pwron.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/pwron.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/readdtal.p1: readdtal.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/readdtal.p1.d 
	@${RM} ${OBJECTDIR}/readdtal.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"." --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/readdtal.p1 readdtal.c 
	@-${MV} ${OBJECTDIR}/readdtal.d ${OBJECTDIR}/readdtal.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/readdtal.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/rtimer.p1: rtimer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/rtimer.p1.d 
	@${RM} ${OBJECTDIR}/rtimer.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"." --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/rtimer.p1 rtimer.c 
	@-${MV} ${OBJECTDIR}/rtimer.d ${OBJECTDIR}/rtimer.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/rtimer.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/timer1.p1: timer1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/timer1.p1.d 
	@${RM} ${OBJECTDIR}/timer1.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"." --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/timer1.p1 timer1.c 
	@-${MV} ${OBJECTDIR}/timer1.d ${OBJECTDIR}/timer1.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/timer1.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/wsw.p1: wsw.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/wsw.p1.d 
	@${RM} ${OBJECTDIR}/wsw.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"." --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/wsw.p1 wsw.c 
	@-${MV} ${OBJECTDIR}/wsw.d ${OBJECTDIR}/wsw.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/wsw.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/timer3.p1: timer3.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/timer3.p1.d 
	@${RM} ${OBJECTDIR}/timer3.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"." --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/timer3.p1 timer3.c 
	@-${MV} ${OBJECTDIR}/timer3.d ${OBJECTDIR}/timer3.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/timer3.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/auart9b.p1: auart9b.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/auart9b.p1.d 
	@${RM} ${OBJECTDIR}/auart9b.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"." --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/auart9b.p1 auart9b.c 
	@-${MV} ${OBJECTDIR}/auart9b.d ${OBJECTDIR}/auart9b.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/auart9b.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/amgs_rs9b.p1: amgs_rs9b.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/amgs_rs9b.p1.d 
	@${RM} ${OBJECTDIR}/amgs_rs9b.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"." --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/amgs_rs9b.p1 amgs_rs9b.c 
	@-${MV} ${OBJECTDIR}/amgs_rs9b.d ${OBJECTDIR}/amgs_rs9b.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/amgs_rs9b.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/uart9b.p1: uart9b.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/uart9b.p1.d 
	@${RM} ${OBJECTDIR}/uart9b.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"." --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/uart9b.p1 uart9b.c 
	@-${MV} ${OBJECTDIR}/uart9b.d ${OBJECTDIR}/uart9b.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/uart9b.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mgs_rs9b.p1: mgs_rs9b.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/mgs_rs9b.p1.d 
	@${RM} ${OBJECTDIR}/mgs_rs9b.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"." --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mgs_rs9b.p1 mgs_rs9b.c 
	@-${MV} ${OBJECTDIR}/mgs_rs9b.d ${OBJECTDIR}/mgs_rs9b.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mgs_rs9b.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/LIN1SInterpreter.p1: LIN1SInterpreter.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/LIN1SInterpreter.p1.d 
	@${RM} ${OBJECTDIR}/LIN1SInterpreter.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"." --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/LIN1SInterpreter.p1 LIN1SInterpreter.c 
	@-${MV} ${OBJECTDIR}/LIN1SInterpreter.d ${OBJECTDIR}/LIN1SInterpreter.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/LIN1SInterpreter.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/LIN1Slave.p1: LIN1Slave.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/LIN1Slave.p1.d 
	@${RM} ${OBJECTDIR}/LIN1Slave.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"." --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/LIN1Slave.p1 LIN1Slave.c 
	@-${MV} ${OBJECTDIR}/LIN1Slave.d ${OBJECTDIR}/LIN1Slave.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/LIN1Slave.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/ac.p1: ac.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ac.p1.d 
	@${RM} ${OBJECTDIR}/ac.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"." --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/ac.p1 ac.c 
	@-${MV} ${OBJECTDIR}/ac.d ${OBJECTDIR}/ac.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/ac.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/ot.p1: ot.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ot.p1.d 
	@${RM} ${OBJECTDIR}/ot.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"." --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/ot.p1 ot.c 
	@-${MV} ${OBJECTDIR}/ot.d ${OBJECTDIR}/ot.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/ot.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/pomiar.p1: pomiar.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/pomiar.p1.d 
	@${RM} ${OBJECTDIR}/pomiar.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"." --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/pomiar.p1 pomiar.c 
	@-${MV} ${OBJECTDIR}/pomiar.d ${OBJECTDIR}/pomiar.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/pomiar.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/timer0.p1: timer0.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/timer0.p1.d 
	@${RM} ${OBJECTDIR}/timer0.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"." --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/timer0.p1 timer0.c 
	@-${MV} ${OBJECTDIR}/timer0.d ${OBJECTDIR}/timer0.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/timer0.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/otInterpreter.p1: otInterpreter.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/otInterpreter.p1.d 
	@${RM} ${OBJECTDIR}/otInterpreter.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"." --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/otInterpreter.p1 otInterpreter.c 
	@-${MV} ${OBJECTDIR}/otInterpreter.d ${OBJECTDIR}/otInterpreter.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/otInterpreter.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
else
${OBJECTDIR}/bit.p1: bit.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/bit.p1.d 
	@${RM} ${OBJECTDIR}/bit.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"." --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/bit.p1 bit.c 
	@-${MV} ${OBJECTDIR}/bit.d ${OBJECTDIR}/bit.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/bit.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/i2c_s.p1: i2c_s.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/i2c_s.p1.d 
	@${RM} ${OBJECTDIR}/i2c_s.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"." --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/i2c_s.p1 i2c_s.c 
	@-${MV} ${OBJECTDIR}/i2c_s.d ${OBJECTDIR}/i2c_s.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/i2c_s.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/isr.p1: isr.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/isr.p1.d 
	@${RM} ${OBJECTDIR}/isr.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"." --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/isr.p1 isr.c 
	@-${MV} ${OBJECTDIR}/isr.d ${OBJECTDIR}/isr.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/isr.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/keypad.p1: keypad.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/keypad.p1.d 
	@${RM} ${OBJECTDIR}/keypad.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"." --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/keypad.p1 keypad.c 
	@-${MV} ${OBJECTDIR}/keypad.d ${OBJECTDIR}/keypad.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/keypad.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/kolejka.p1: kolejka.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/kolejka.p1.d 
	@${RM} ${OBJECTDIR}/kolejka.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"." --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/kolejka.p1 kolejka.c 
	@-${MV} ${OBJECTDIR}/kolejka.d ${OBJECTDIR}/kolejka.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/kolejka.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/konfig3.p1: konfig3.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/konfig3.p1.d 
	@${RM} ${OBJECTDIR}/konfig3.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"." --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/konfig3.p1 konfig3.c 
	@-${MV} ${OBJECTDIR}/konfig3.d ${OBJECTDIR}/konfig3.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/konfig3.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/konsola.p1: konsola.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/konsola.p1.d 
	@${RM} ${OBJECTDIR}/konsola.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"." --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/konsola.p1 konsola.c 
	@-${MV} ${OBJECTDIR}/konsola.d ${OBJECTDIR}/konsola.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/konsola.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/lled.p1: lled.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/lled.p1.d 
	@${RM} ${OBJECTDIR}/lled.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"." --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/lled.p1 lled.c 
	@-${MV} ${OBJECTDIR}/lled.d ${OBJECTDIR}/lled.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/lled.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/main.p1: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.p1.d 
	@${RM} ${OBJECTDIR}/main.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"." --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/main.p1 main.c 
	@-${MV} ${OBJECTDIR}/main.d ${OBJECTDIR}/main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/osc.p1: osc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/osc.p1.d 
	@${RM} ${OBJECTDIR}/osc.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"." --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/osc.p1 osc.c 
	@-${MV} ${OBJECTDIR}/osc.d ${OBJECTDIR}/osc.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/osc.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/port.p1: port.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/port.p1.d 
	@${RM} ${OBJECTDIR}/port.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"." --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/port.p1 port.c 
	@-${MV} ${OBJECTDIR}/port.d ${OBJECTDIR}/port.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/port.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/print.p1: print.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/print.p1.d 
	@${RM} ${OBJECTDIR}/print.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"." --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/print.p1 print.c 
	@-${MV} ${OBJECTDIR}/print.d ${OBJECTDIR}/print.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/print.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/pwm2.p1: pwm2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/pwm2.p1.d 
	@${RM} ${OBJECTDIR}/pwm2.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"." --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/pwm2.p1 pwm2.c 
	@-${MV} ${OBJECTDIR}/pwm2.d ${OBJECTDIR}/pwm2.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/pwm2.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/pwron.p1: pwron.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/pwron.p1.d 
	@${RM} ${OBJECTDIR}/pwron.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"." --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/pwron.p1 pwron.c 
	@-${MV} ${OBJECTDIR}/pwron.d ${OBJECTDIR}/pwron.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/pwron.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/readdtal.p1: readdtal.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/readdtal.p1.d 
	@${RM} ${OBJECTDIR}/readdtal.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"." --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/readdtal.p1 readdtal.c 
	@-${MV} ${OBJECTDIR}/readdtal.d ${OBJECTDIR}/readdtal.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/readdtal.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/rtimer.p1: rtimer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/rtimer.p1.d 
	@${RM} ${OBJECTDIR}/rtimer.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"." --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/rtimer.p1 rtimer.c 
	@-${MV} ${OBJECTDIR}/rtimer.d ${OBJECTDIR}/rtimer.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/rtimer.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/timer1.p1: timer1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/timer1.p1.d 
	@${RM} ${OBJECTDIR}/timer1.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"." --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/timer1.p1 timer1.c 
	@-${MV} ${OBJECTDIR}/timer1.d ${OBJECTDIR}/timer1.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/timer1.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/wsw.p1: wsw.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/wsw.p1.d 
	@${RM} ${OBJECTDIR}/wsw.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"." --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/wsw.p1 wsw.c 
	@-${MV} ${OBJECTDIR}/wsw.d ${OBJECTDIR}/wsw.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/wsw.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/timer3.p1: timer3.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/timer3.p1.d 
	@${RM} ${OBJECTDIR}/timer3.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"." --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/timer3.p1 timer3.c 
	@-${MV} ${OBJECTDIR}/timer3.d ${OBJECTDIR}/timer3.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/timer3.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/auart9b.p1: auart9b.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/auart9b.p1.d 
	@${RM} ${OBJECTDIR}/auart9b.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"." --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/auart9b.p1 auart9b.c 
	@-${MV} ${OBJECTDIR}/auart9b.d ${OBJECTDIR}/auart9b.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/auart9b.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/amgs_rs9b.p1: amgs_rs9b.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/amgs_rs9b.p1.d 
	@${RM} ${OBJECTDIR}/amgs_rs9b.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"." --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/amgs_rs9b.p1 amgs_rs9b.c 
	@-${MV} ${OBJECTDIR}/amgs_rs9b.d ${OBJECTDIR}/amgs_rs9b.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/amgs_rs9b.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/uart9b.p1: uart9b.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/uart9b.p1.d 
	@${RM} ${OBJECTDIR}/uart9b.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"." --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/uart9b.p1 uart9b.c 
	@-${MV} ${OBJECTDIR}/uart9b.d ${OBJECTDIR}/uart9b.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/uart9b.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mgs_rs9b.p1: mgs_rs9b.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/mgs_rs9b.p1.d 
	@${RM} ${OBJECTDIR}/mgs_rs9b.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"." --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mgs_rs9b.p1 mgs_rs9b.c 
	@-${MV} ${OBJECTDIR}/mgs_rs9b.d ${OBJECTDIR}/mgs_rs9b.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mgs_rs9b.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/LIN1SInterpreter.p1: LIN1SInterpreter.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/LIN1SInterpreter.p1.d 
	@${RM} ${OBJECTDIR}/LIN1SInterpreter.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"." --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/LIN1SInterpreter.p1 LIN1SInterpreter.c 
	@-${MV} ${OBJECTDIR}/LIN1SInterpreter.d ${OBJECTDIR}/LIN1SInterpreter.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/LIN1SInterpreter.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/LIN1Slave.p1: LIN1Slave.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/LIN1Slave.p1.d 
	@${RM} ${OBJECTDIR}/LIN1Slave.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"." --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/LIN1Slave.p1 LIN1Slave.c 
	@-${MV} ${OBJECTDIR}/LIN1Slave.d ${OBJECTDIR}/LIN1Slave.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/LIN1Slave.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/ac.p1: ac.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ac.p1.d 
	@${RM} ${OBJECTDIR}/ac.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"." --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/ac.p1 ac.c 
	@-${MV} ${OBJECTDIR}/ac.d ${OBJECTDIR}/ac.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/ac.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/ot.p1: ot.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ot.p1.d 
	@${RM} ${OBJECTDIR}/ot.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"." --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/ot.p1 ot.c 
	@-${MV} ${OBJECTDIR}/ot.d ${OBJECTDIR}/ot.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/ot.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/pomiar.p1: pomiar.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/pomiar.p1.d 
	@${RM} ${OBJECTDIR}/pomiar.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"." --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/pomiar.p1 pomiar.c 
	@-${MV} ${OBJECTDIR}/pomiar.d ${OBJECTDIR}/pomiar.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/pomiar.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/timer0.p1: timer0.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/timer0.p1.d 
	@${RM} ${OBJECTDIR}/timer0.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"." --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/timer0.p1 timer0.c 
	@-${MV} ${OBJECTDIR}/timer0.d ${OBJECTDIR}/timer0.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/timer0.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/otInterpreter.p1: otInterpreter.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/otInterpreter.p1.d 
	@${RM} ${OBJECTDIR}/otInterpreter.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"." --warn=0 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/otInterpreter.p1 otInterpreter.c 
	@-${MV} ${OBJECTDIR}/otInterpreter.d ${OBJECTDIR}/otInterpreter.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/otInterpreter.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/T02_LCD_BOARD.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) --chip=$(MP_PROCESSOR_OPTION) -G -mdist/${CND_CONF}/${IMAGE_TYPE}/T02_LCD_BOARD.X.${IMAGE_TYPE}.map  -D__DEBUG=1  --debugger=none  -DXPRJ_default=$(CND_CONF)  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"." --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,-plib --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"        $(COMPARISON_BUILD) --memorysummary dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml -odist/${CND_CONF}/${IMAGE_TYPE}/T02_LCD_BOARD.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}     
	@${RM} dist/${CND_CONF}/${IMAGE_TYPE}/T02_LCD_BOARD.X.${IMAGE_TYPE}.hex 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/T02_LCD_BOARD.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) --chip=$(MP_PROCESSOR_OPTION) -G -mdist/${CND_CONF}/${IMAGE_TYPE}/T02_LCD_BOARD.X.${IMAGE_TYPE}.map  -DXPRJ_default=$(CND_CONF)  --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"." --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,-plib --output=-mcof,+elf --stack=compiled "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     $(COMPARISON_BUILD) --memorysummary dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml -odist/${CND_CONF}/${IMAGE_TYPE}/T02_LCD_BOARD.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}     
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
