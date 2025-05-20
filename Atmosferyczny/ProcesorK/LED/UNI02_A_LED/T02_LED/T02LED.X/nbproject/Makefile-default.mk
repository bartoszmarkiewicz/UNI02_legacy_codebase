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
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/T02LED.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/T02LED.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=
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
SOURCEFILES_QUOTED_IF_SPACED=../ac.c ../bit.c ../i2c_s.c ../isr.c ../main.c ../osc.c ../pomiar.c ../port.c ../print.c ../pwm.c ../pwron.c ../rgl.c ../rtimer.c ../timer1.c ../uart9b.c ../pompa.c ../wsw.c ../keypad.c ../readdtal.c ../konsola.c ../konfig2.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1472/ac.p1 ${OBJECTDIR}/_ext/1472/bit.p1 ${OBJECTDIR}/_ext/1472/i2c_s.p1 ${OBJECTDIR}/_ext/1472/isr.p1 ${OBJECTDIR}/_ext/1472/main.p1 ${OBJECTDIR}/_ext/1472/osc.p1 ${OBJECTDIR}/_ext/1472/pomiar.p1 ${OBJECTDIR}/_ext/1472/port.p1 ${OBJECTDIR}/_ext/1472/print.p1 ${OBJECTDIR}/_ext/1472/pwm.p1 ${OBJECTDIR}/_ext/1472/pwron.p1 ${OBJECTDIR}/_ext/1472/rgl.p1 ${OBJECTDIR}/_ext/1472/rtimer.p1 ${OBJECTDIR}/_ext/1472/timer1.p1 ${OBJECTDIR}/_ext/1472/uart9b.p1 ${OBJECTDIR}/_ext/1472/pompa.p1 ${OBJECTDIR}/_ext/1472/wsw.p1 ${OBJECTDIR}/_ext/1472/keypad.p1 ${OBJECTDIR}/_ext/1472/readdtal.p1 ${OBJECTDIR}/_ext/1472/konsola.p1 ${OBJECTDIR}/_ext/1472/konfig2.p1
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1472/ac.p1.d ${OBJECTDIR}/_ext/1472/bit.p1.d ${OBJECTDIR}/_ext/1472/i2c_s.p1.d ${OBJECTDIR}/_ext/1472/isr.p1.d ${OBJECTDIR}/_ext/1472/main.p1.d ${OBJECTDIR}/_ext/1472/osc.p1.d ${OBJECTDIR}/_ext/1472/pomiar.p1.d ${OBJECTDIR}/_ext/1472/port.p1.d ${OBJECTDIR}/_ext/1472/print.p1.d ${OBJECTDIR}/_ext/1472/pwm.p1.d ${OBJECTDIR}/_ext/1472/pwron.p1.d ${OBJECTDIR}/_ext/1472/rgl.p1.d ${OBJECTDIR}/_ext/1472/rtimer.p1.d ${OBJECTDIR}/_ext/1472/timer1.p1.d ${OBJECTDIR}/_ext/1472/uart9b.p1.d ${OBJECTDIR}/_ext/1472/pompa.p1.d ${OBJECTDIR}/_ext/1472/wsw.p1.d ${OBJECTDIR}/_ext/1472/keypad.p1.d ${OBJECTDIR}/_ext/1472/readdtal.p1.d ${OBJECTDIR}/_ext/1472/konsola.p1.d ${OBJECTDIR}/_ext/1472/konfig2.p1.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1472/ac.p1 ${OBJECTDIR}/_ext/1472/bit.p1 ${OBJECTDIR}/_ext/1472/i2c_s.p1 ${OBJECTDIR}/_ext/1472/isr.p1 ${OBJECTDIR}/_ext/1472/main.p1 ${OBJECTDIR}/_ext/1472/osc.p1 ${OBJECTDIR}/_ext/1472/pomiar.p1 ${OBJECTDIR}/_ext/1472/port.p1 ${OBJECTDIR}/_ext/1472/print.p1 ${OBJECTDIR}/_ext/1472/pwm.p1 ${OBJECTDIR}/_ext/1472/pwron.p1 ${OBJECTDIR}/_ext/1472/rgl.p1 ${OBJECTDIR}/_ext/1472/rtimer.p1 ${OBJECTDIR}/_ext/1472/timer1.p1 ${OBJECTDIR}/_ext/1472/uart9b.p1 ${OBJECTDIR}/_ext/1472/pompa.p1 ${OBJECTDIR}/_ext/1472/wsw.p1 ${OBJECTDIR}/_ext/1472/keypad.p1 ${OBJECTDIR}/_ext/1472/readdtal.p1 ${OBJECTDIR}/_ext/1472/konsola.p1 ${OBJECTDIR}/_ext/1472/konfig2.p1

# Source Files
SOURCEFILES=../ac.c ../bit.c ../i2c_s.c ../isr.c ../main.c ../osc.c ../pomiar.c ../port.c ../print.c ../pwm.c ../pwron.c ../rgl.c ../rtimer.c ../timer1.c ../uart9b.c ../pompa.c ../wsw.c ../keypad.c ../readdtal.c ../konsola.c ../konfig2.c


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
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/T02LED.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=18F46K20
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1472/ac.p1: ../ac.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/ac.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1472/ac.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=realice  --double=32 --float=32 --emi=wordwrite --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=std -P -N255 -I".." --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1472/ac.p1  ../ac.c 
	@-${MV} ${OBJECTDIR}/_ext/1472/ac.d ${OBJECTDIR}/_ext/1472/ac.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1472/ac.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/bit.p1: ../bit.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/bit.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1472/bit.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=realice  --double=32 --float=32 --emi=wordwrite --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=std -P -N255 -I".." --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1472/bit.p1  ../bit.c 
	@-${MV} ${OBJECTDIR}/_ext/1472/bit.d ${OBJECTDIR}/_ext/1472/bit.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1472/bit.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/i2c_s.p1: ../i2c_s.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/i2c_s.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1472/i2c_s.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=realice  --double=32 --float=32 --emi=wordwrite --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=std -P -N255 -I".." --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1472/i2c_s.p1  ../i2c_s.c 
	@-${MV} ${OBJECTDIR}/_ext/1472/i2c_s.d ${OBJECTDIR}/_ext/1472/i2c_s.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1472/i2c_s.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/isr.p1: ../isr.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/isr.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1472/isr.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=realice  --double=32 --float=32 --emi=wordwrite --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=std -P -N255 -I".." --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1472/isr.p1  ../isr.c 
	@-${MV} ${OBJECTDIR}/_ext/1472/isr.d ${OBJECTDIR}/_ext/1472/isr.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1472/isr.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/main.p1: ../main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/main.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=realice  --double=32 --float=32 --emi=wordwrite --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=std -P -N255 -I".." --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1472/main.p1  ../main.c 
	@-${MV} ${OBJECTDIR}/_ext/1472/main.d ${OBJECTDIR}/_ext/1472/main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1472/main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/osc.p1: ../osc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/osc.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1472/osc.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=realice  --double=32 --float=32 --emi=wordwrite --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=std -P -N255 -I".." --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1472/osc.p1  ../osc.c 
	@-${MV} ${OBJECTDIR}/_ext/1472/osc.d ${OBJECTDIR}/_ext/1472/osc.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1472/osc.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/pomiar.p1: ../pomiar.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/pomiar.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1472/pomiar.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=realice  --double=32 --float=32 --emi=wordwrite --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=std -P -N255 -I".." --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1472/pomiar.p1  ../pomiar.c 
	@-${MV} ${OBJECTDIR}/_ext/1472/pomiar.d ${OBJECTDIR}/_ext/1472/pomiar.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1472/pomiar.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/port.p1: ../port.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/port.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1472/port.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=realice  --double=32 --float=32 --emi=wordwrite --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=std -P -N255 -I".." --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1472/port.p1  ../port.c 
	@-${MV} ${OBJECTDIR}/_ext/1472/port.d ${OBJECTDIR}/_ext/1472/port.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1472/port.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/print.p1: ../print.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/print.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1472/print.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=realice  --double=32 --float=32 --emi=wordwrite --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=std -P -N255 -I".." --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1472/print.p1  ../print.c 
	@-${MV} ${OBJECTDIR}/_ext/1472/print.d ${OBJECTDIR}/_ext/1472/print.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1472/print.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/pwm.p1: ../pwm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/pwm.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1472/pwm.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=realice  --double=32 --float=32 --emi=wordwrite --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=std -P -N255 -I".." --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1472/pwm.p1  ../pwm.c 
	@-${MV} ${OBJECTDIR}/_ext/1472/pwm.d ${OBJECTDIR}/_ext/1472/pwm.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1472/pwm.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/pwron.p1: ../pwron.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/pwron.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1472/pwron.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=realice  --double=32 --float=32 --emi=wordwrite --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=std -P -N255 -I".." --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1472/pwron.p1  ../pwron.c 
	@-${MV} ${OBJECTDIR}/_ext/1472/pwron.d ${OBJECTDIR}/_ext/1472/pwron.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1472/pwron.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/rgl.p1: ../rgl.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/rgl.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1472/rgl.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=realice  --double=32 --float=32 --emi=wordwrite --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=std -P -N255 -I".." --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1472/rgl.p1  ../rgl.c 
	@-${MV} ${OBJECTDIR}/_ext/1472/rgl.d ${OBJECTDIR}/_ext/1472/rgl.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1472/rgl.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/rtimer.p1: ../rtimer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/rtimer.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1472/rtimer.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=realice  --double=32 --float=32 --emi=wordwrite --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=std -P -N255 -I".." --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1472/rtimer.p1  ../rtimer.c 
	@-${MV} ${OBJECTDIR}/_ext/1472/rtimer.d ${OBJECTDIR}/_ext/1472/rtimer.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1472/rtimer.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/timer1.p1: ../timer1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/timer1.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1472/timer1.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=realice  --double=32 --float=32 --emi=wordwrite --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=std -P -N255 -I".." --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1472/timer1.p1  ../timer1.c 
	@-${MV} ${OBJECTDIR}/_ext/1472/timer1.d ${OBJECTDIR}/_ext/1472/timer1.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1472/timer1.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/uart9b.p1: ../uart9b.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/uart9b.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1472/uart9b.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=realice  --double=32 --float=32 --emi=wordwrite --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=std -P -N255 -I".." --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1472/uart9b.p1  ../uart9b.c 
	@-${MV} ${OBJECTDIR}/_ext/1472/uart9b.d ${OBJECTDIR}/_ext/1472/uart9b.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1472/uart9b.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/pompa.p1: ../pompa.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/pompa.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1472/pompa.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=realice  --double=32 --float=32 --emi=wordwrite --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=std -P -N255 -I".." --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1472/pompa.p1  ../pompa.c 
	@-${MV} ${OBJECTDIR}/_ext/1472/pompa.d ${OBJECTDIR}/_ext/1472/pompa.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1472/pompa.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/wsw.p1: ../wsw.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/wsw.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1472/wsw.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=realice  --double=32 --float=32 --emi=wordwrite --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=std -P -N255 -I".." --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1472/wsw.p1  ../wsw.c 
	@-${MV} ${OBJECTDIR}/_ext/1472/wsw.d ${OBJECTDIR}/_ext/1472/wsw.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1472/wsw.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/keypad.p1: ../keypad.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/keypad.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1472/keypad.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=realice  --double=32 --float=32 --emi=wordwrite --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=std -P -N255 -I".." --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1472/keypad.p1  ../keypad.c 
	@-${MV} ${OBJECTDIR}/_ext/1472/keypad.d ${OBJECTDIR}/_ext/1472/keypad.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1472/keypad.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/readdtal.p1: ../readdtal.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/readdtal.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1472/readdtal.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=realice  --double=32 --float=32 --emi=wordwrite --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=std -P -N255 -I".." --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1472/readdtal.p1  ../readdtal.c 
	@-${MV} ${OBJECTDIR}/_ext/1472/readdtal.d ${OBJECTDIR}/_ext/1472/readdtal.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1472/readdtal.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/konsola.p1: ../konsola.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/konsola.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1472/konsola.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=realice  --double=32 --float=32 --emi=wordwrite --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=std -P -N255 -I".." --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1472/konsola.p1  ../konsola.c 
	@-${MV} ${OBJECTDIR}/_ext/1472/konsola.d ${OBJECTDIR}/_ext/1472/konsola.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1472/konsola.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/konfig2.p1: ../konfig2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/konfig2.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1472/konfig2.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=realice  --double=32 --float=32 --emi=wordwrite --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=std -P -N255 -I".." --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1472/konfig2.p1  ../konfig2.c 
	@-${MV} ${OBJECTDIR}/_ext/1472/konfig2.d ${OBJECTDIR}/_ext/1472/konfig2.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1472/konfig2.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
else
${OBJECTDIR}/_ext/1472/ac.p1: ../ac.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/ac.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1472/ac.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=32 --float=32 --emi=wordwrite --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=std -P -N255 -I".." --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1472/ac.p1  ../ac.c 
	@-${MV} ${OBJECTDIR}/_ext/1472/ac.d ${OBJECTDIR}/_ext/1472/ac.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1472/ac.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/bit.p1: ../bit.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/bit.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1472/bit.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=32 --float=32 --emi=wordwrite --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=std -P -N255 -I".." --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1472/bit.p1  ../bit.c 
	@-${MV} ${OBJECTDIR}/_ext/1472/bit.d ${OBJECTDIR}/_ext/1472/bit.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1472/bit.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/i2c_s.p1: ../i2c_s.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/i2c_s.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1472/i2c_s.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=32 --float=32 --emi=wordwrite --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=std -P -N255 -I".." --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1472/i2c_s.p1  ../i2c_s.c 
	@-${MV} ${OBJECTDIR}/_ext/1472/i2c_s.d ${OBJECTDIR}/_ext/1472/i2c_s.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1472/i2c_s.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/isr.p1: ../isr.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/isr.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1472/isr.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=32 --float=32 --emi=wordwrite --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=std -P -N255 -I".." --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1472/isr.p1  ../isr.c 
	@-${MV} ${OBJECTDIR}/_ext/1472/isr.d ${OBJECTDIR}/_ext/1472/isr.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1472/isr.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/main.p1: ../main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/main.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=32 --float=32 --emi=wordwrite --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=std -P -N255 -I".." --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1472/main.p1  ../main.c 
	@-${MV} ${OBJECTDIR}/_ext/1472/main.d ${OBJECTDIR}/_ext/1472/main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1472/main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/osc.p1: ../osc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/osc.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1472/osc.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=32 --float=32 --emi=wordwrite --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=std -P -N255 -I".." --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1472/osc.p1  ../osc.c 
	@-${MV} ${OBJECTDIR}/_ext/1472/osc.d ${OBJECTDIR}/_ext/1472/osc.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1472/osc.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/pomiar.p1: ../pomiar.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/pomiar.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1472/pomiar.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=32 --float=32 --emi=wordwrite --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=std -P -N255 -I".." --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1472/pomiar.p1  ../pomiar.c 
	@-${MV} ${OBJECTDIR}/_ext/1472/pomiar.d ${OBJECTDIR}/_ext/1472/pomiar.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1472/pomiar.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/port.p1: ../port.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/port.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1472/port.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=32 --float=32 --emi=wordwrite --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=std -P -N255 -I".." --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1472/port.p1  ../port.c 
	@-${MV} ${OBJECTDIR}/_ext/1472/port.d ${OBJECTDIR}/_ext/1472/port.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1472/port.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/print.p1: ../print.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/print.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1472/print.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=32 --float=32 --emi=wordwrite --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=std -P -N255 -I".." --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1472/print.p1  ../print.c 
	@-${MV} ${OBJECTDIR}/_ext/1472/print.d ${OBJECTDIR}/_ext/1472/print.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1472/print.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/pwm.p1: ../pwm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/pwm.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1472/pwm.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=32 --float=32 --emi=wordwrite --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=std -P -N255 -I".." --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1472/pwm.p1  ../pwm.c 
	@-${MV} ${OBJECTDIR}/_ext/1472/pwm.d ${OBJECTDIR}/_ext/1472/pwm.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1472/pwm.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/pwron.p1: ../pwron.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/pwron.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1472/pwron.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=32 --float=32 --emi=wordwrite --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=std -P -N255 -I".." --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1472/pwron.p1  ../pwron.c 
	@-${MV} ${OBJECTDIR}/_ext/1472/pwron.d ${OBJECTDIR}/_ext/1472/pwron.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1472/pwron.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/rgl.p1: ../rgl.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/rgl.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1472/rgl.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=32 --float=32 --emi=wordwrite --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=std -P -N255 -I".." --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1472/rgl.p1  ../rgl.c 
	@-${MV} ${OBJECTDIR}/_ext/1472/rgl.d ${OBJECTDIR}/_ext/1472/rgl.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1472/rgl.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/rtimer.p1: ../rtimer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/rtimer.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1472/rtimer.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=32 --float=32 --emi=wordwrite --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=std -P -N255 -I".." --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1472/rtimer.p1  ../rtimer.c 
	@-${MV} ${OBJECTDIR}/_ext/1472/rtimer.d ${OBJECTDIR}/_ext/1472/rtimer.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1472/rtimer.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/timer1.p1: ../timer1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/timer1.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1472/timer1.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=32 --float=32 --emi=wordwrite --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=std -P -N255 -I".." --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1472/timer1.p1  ../timer1.c 
	@-${MV} ${OBJECTDIR}/_ext/1472/timer1.d ${OBJECTDIR}/_ext/1472/timer1.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1472/timer1.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/uart9b.p1: ../uart9b.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/uart9b.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1472/uart9b.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=32 --float=32 --emi=wordwrite --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=std -P -N255 -I".." --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1472/uart9b.p1  ../uart9b.c 
	@-${MV} ${OBJECTDIR}/_ext/1472/uart9b.d ${OBJECTDIR}/_ext/1472/uart9b.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1472/uart9b.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/pompa.p1: ../pompa.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/pompa.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1472/pompa.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=32 --float=32 --emi=wordwrite --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=std -P -N255 -I".." --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1472/pompa.p1  ../pompa.c 
	@-${MV} ${OBJECTDIR}/_ext/1472/pompa.d ${OBJECTDIR}/_ext/1472/pompa.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1472/pompa.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/wsw.p1: ../wsw.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/wsw.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1472/wsw.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=32 --float=32 --emi=wordwrite --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=std -P -N255 -I".." --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1472/wsw.p1  ../wsw.c 
	@-${MV} ${OBJECTDIR}/_ext/1472/wsw.d ${OBJECTDIR}/_ext/1472/wsw.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1472/wsw.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/keypad.p1: ../keypad.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/keypad.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1472/keypad.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=32 --float=32 --emi=wordwrite --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=std -P -N255 -I".." --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1472/keypad.p1  ../keypad.c 
	@-${MV} ${OBJECTDIR}/_ext/1472/keypad.d ${OBJECTDIR}/_ext/1472/keypad.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1472/keypad.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/readdtal.p1: ../readdtal.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/readdtal.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1472/readdtal.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=32 --float=32 --emi=wordwrite --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=std -P -N255 -I".." --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1472/readdtal.p1  ../readdtal.c 
	@-${MV} ${OBJECTDIR}/_ext/1472/readdtal.d ${OBJECTDIR}/_ext/1472/readdtal.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1472/readdtal.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/konsola.p1: ../konsola.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/konsola.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1472/konsola.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=32 --float=32 --emi=wordwrite --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=std -P -N255 -I".." --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1472/konsola.p1  ../konsola.c 
	@-${MV} ${OBJECTDIR}/_ext/1472/konsola.d ${OBJECTDIR}/_ext/1472/konsola.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1472/konsola.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1472/konfig2.p1: ../konfig2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/konfig2.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1472/konfig2.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=32 --float=32 --emi=wordwrite --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=std -P -N255 -I".." --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/_ext/1472/konfig2.p1  ../konfig2.c 
	@-${MV} ${OBJECTDIR}/_ext/1472/konfig2.d ${OBJECTDIR}/_ext/1472/konfig2.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1472/konfig2.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/T02LED.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) --chip=$(MP_PROCESSOR_OPTION) -G -mdist/${CND_CONF}/${IMAGE_TYPE}/T02LED.X.${IMAGE_TYPE}.map  -D__DEBUG=1 --debugger=realice  --double=32 --float=32 --emi=wordwrite --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=std -P -N255 -I".." --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     --rom=default,-fd30-ffff --ram=default,-ef4-eff,-f9c-f9c,-fd4-fd4,-fdb-fdf,-fe3-fe7,-feb-fef,-ffd-fff  $(COMPARISON_BUILD) --memorysummary dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml -odist/${CND_CONF}/${IMAGE_TYPE}/T02LED.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}     
	@${RM} dist/${CND_CONF}/${IMAGE_TYPE}/T02LED.X.${IMAGE_TYPE}.hex 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/T02LED.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) --chip=$(MP_PROCESSOR_OPTION) -G -mdist/${CND_CONF}/${IMAGE_TYPE}/T02LED.X.${IMAGE_TYPE}.map  --double=32 --float=32 --emi=wordwrite --opt=default,+asm,-asmfile,-speed,+space,-debug --addrqual=ignore --mode=std -P -N255 -I".." --warn=-1 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,-config,+clib,+plib --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     $(COMPARISON_BUILD) --memorysummary dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml -odist/${CND_CONF}/${IMAGE_TYPE}/T02LED.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}     
	
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
