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
OUTPUT_SUFFIX=cof
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/T03.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/T03.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
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
SOURCEFILES_QUOTED_IF_SPACED=../ac.c ../bit.c ../i2c_s.c ../isr.c ../main.c ../osc.c ../pomiar.c ../port.c ../print.c ../pwm.c ../pwron.c ../rgl.c ../rtimer.c ../timer1.c ../konfig.c ../smgs_rs9b.c ../uart9b.c ../timer3.c ../tacho.c ../vntregtch.c ../pompa.c ../history.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1472/ac.obj ${OBJECTDIR}/_ext/1472/bit.obj ${OBJECTDIR}/_ext/1472/i2c_s.obj ${OBJECTDIR}/_ext/1472/isr.obj ${OBJECTDIR}/_ext/1472/main.obj ${OBJECTDIR}/_ext/1472/osc.obj ${OBJECTDIR}/_ext/1472/pomiar.obj ${OBJECTDIR}/_ext/1472/port.obj ${OBJECTDIR}/_ext/1472/print.obj ${OBJECTDIR}/_ext/1472/pwm.obj ${OBJECTDIR}/_ext/1472/pwron.obj ${OBJECTDIR}/_ext/1472/rgl.obj ${OBJECTDIR}/_ext/1472/rtimer.obj ${OBJECTDIR}/_ext/1472/timer1.obj ${OBJECTDIR}/_ext/1472/konfig.obj ${OBJECTDIR}/_ext/1472/smgs_rs9b.obj ${OBJECTDIR}/_ext/1472/uart9b.obj ${OBJECTDIR}/_ext/1472/timer3.obj ${OBJECTDIR}/_ext/1472/tacho.obj ${OBJECTDIR}/_ext/1472/vntregtch.obj ${OBJECTDIR}/_ext/1472/pompa.obj ${OBJECTDIR}/_ext/1472/history.obj
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1472/ac.obj.d ${OBJECTDIR}/_ext/1472/bit.obj.d ${OBJECTDIR}/_ext/1472/i2c_s.obj.d ${OBJECTDIR}/_ext/1472/isr.obj.d ${OBJECTDIR}/_ext/1472/main.obj.d ${OBJECTDIR}/_ext/1472/osc.obj.d ${OBJECTDIR}/_ext/1472/pomiar.obj.d ${OBJECTDIR}/_ext/1472/port.obj.d ${OBJECTDIR}/_ext/1472/print.obj.d ${OBJECTDIR}/_ext/1472/pwm.obj.d ${OBJECTDIR}/_ext/1472/pwron.obj.d ${OBJECTDIR}/_ext/1472/rgl.obj.d ${OBJECTDIR}/_ext/1472/rtimer.obj.d ${OBJECTDIR}/_ext/1472/timer1.obj.d ${OBJECTDIR}/_ext/1472/konfig.obj.d ${OBJECTDIR}/_ext/1472/smgs_rs9b.obj.d ${OBJECTDIR}/_ext/1472/uart9b.obj.d ${OBJECTDIR}/_ext/1472/timer3.obj.d ${OBJECTDIR}/_ext/1472/tacho.obj.d ${OBJECTDIR}/_ext/1472/vntregtch.obj.d ${OBJECTDIR}/_ext/1472/pompa.obj.d ${OBJECTDIR}/_ext/1472/history.obj.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1472/ac.obj ${OBJECTDIR}/_ext/1472/bit.obj ${OBJECTDIR}/_ext/1472/i2c_s.obj ${OBJECTDIR}/_ext/1472/isr.obj ${OBJECTDIR}/_ext/1472/main.obj ${OBJECTDIR}/_ext/1472/osc.obj ${OBJECTDIR}/_ext/1472/pomiar.obj ${OBJECTDIR}/_ext/1472/port.obj ${OBJECTDIR}/_ext/1472/print.obj ${OBJECTDIR}/_ext/1472/pwm.obj ${OBJECTDIR}/_ext/1472/pwron.obj ${OBJECTDIR}/_ext/1472/rgl.obj ${OBJECTDIR}/_ext/1472/rtimer.obj ${OBJECTDIR}/_ext/1472/timer1.obj ${OBJECTDIR}/_ext/1472/konfig.obj ${OBJECTDIR}/_ext/1472/smgs_rs9b.obj ${OBJECTDIR}/_ext/1472/uart9b.obj ${OBJECTDIR}/_ext/1472/timer3.obj ${OBJECTDIR}/_ext/1472/tacho.obj ${OBJECTDIR}/_ext/1472/vntregtch.obj ${OBJECTDIR}/_ext/1472/pompa.obj ${OBJECTDIR}/_ext/1472/history.obj

# Source Files
SOURCEFILES=../ac.c ../bit.c ../i2c_s.c ../isr.c ../main.c ../osc.c ../pomiar.c ../port.c ../print.c ../pwm.c ../pwron.c ../rgl.c ../rtimer.c ../timer1.c ../konfig.c ../smgs_rs9b.c ../uart9b.c ../timer3.c ../tacho.c ../vntregtch.c ../pompa.c ../history.c


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
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/T03.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=18F46K20
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1472/ac.obj: ../ac.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	${MP_CC} -C ../ac.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -D__DEBUG  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep -C ../ac.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -D__DEBUG  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/_ext/1472/ac.obj: > ${OBJECTDIR}/_ext/1472/ac.obj.d
	@cat ${OBJECTDIR}/_ext/1472/ac.dep >> ${OBJECTDIR}/_ext/1472/ac.obj.d
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/ac.obj.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	 
	
${OBJECTDIR}/_ext/1472/bit.obj: ../bit.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	${MP_CC} -C ../bit.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -D__DEBUG  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep -C ../bit.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -D__DEBUG  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/_ext/1472/bit.obj: > ${OBJECTDIR}/_ext/1472/bit.obj.d
	@cat ${OBJECTDIR}/_ext/1472/bit.dep >> ${OBJECTDIR}/_ext/1472/bit.obj.d
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/bit.obj.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	 
	
${OBJECTDIR}/_ext/1472/i2c_s.obj: ../i2c_s.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	${MP_CC} -C ../i2c_s.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -D__DEBUG  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep -C ../i2c_s.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -D__DEBUG  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/_ext/1472/i2c_s.obj: > ${OBJECTDIR}/_ext/1472/i2c_s.obj.d
	@cat ${OBJECTDIR}/_ext/1472/i2c_s.dep >> ${OBJECTDIR}/_ext/1472/i2c_s.obj.d
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/i2c_s.obj.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	 
	
${OBJECTDIR}/_ext/1472/isr.obj: ../isr.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	${MP_CC} -C ../isr.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -D__DEBUG  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep -C ../isr.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -D__DEBUG  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/_ext/1472/isr.obj: > ${OBJECTDIR}/_ext/1472/isr.obj.d
	@cat ${OBJECTDIR}/_ext/1472/isr.dep >> ${OBJECTDIR}/_ext/1472/isr.obj.d
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/isr.obj.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	 
	
${OBJECTDIR}/_ext/1472/main.obj: ../main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	${MP_CC} -C ../main.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -D__DEBUG  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep -C ../main.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -D__DEBUG  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/_ext/1472/main.obj: > ${OBJECTDIR}/_ext/1472/main.obj.d
	@cat ${OBJECTDIR}/_ext/1472/main.dep >> ${OBJECTDIR}/_ext/1472/main.obj.d
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/main.obj.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	 
	
${OBJECTDIR}/_ext/1472/osc.obj: ../osc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	${MP_CC} -C ../osc.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -D__DEBUG  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep -C ../osc.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -D__DEBUG  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/_ext/1472/osc.obj: > ${OBJECTDIR}/_ext/1472/osc.obj.d
	@cat ${OBJECTDIR}/_ext/1472/osc.dep >> ${OBJECTDIR}/_ext/1472/osc.obj.d
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/osc.obj.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	 
	
${OBJECTDIR}/_ext/1472/pomiar.obj: ../pomiar.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	${MP_CC} -C ../pomiar.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -D__DEBUG  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep -C ../pomiar.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -D__DEBUG  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/_ext/1472/pomiar.obj: > ${OBJECTDIR}/_ext/1472/pomiar.obj.d
	@cat ${OBJECTDIR}/_ext/1472/pomiar.dep >> ${OBJECTDIR}/_ext/1472/pomiar.obj.d
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/pomiar.obj.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	 
	
${OBJECTDIR}/_ext/1472/port.obj: ../port.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	${MP_CC} -C ../port.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -D__DEBUG  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep -C ../port.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -D__DEBUG  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/_ext/1472/port.obj: > ${OBJECTDIR}/_ext/1472/port.obj.d
	@cat ${OBJECTDIR}/_ext/1472/port.dep >> ${OBJECTDIR}/_ext/1472/port.obj.d
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/port.obj.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	 
	
${OBJECTDIR}/_ext/1472/print.obj: ../print.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	${MP_CC} -C ../print.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -D__DEBUG  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep -C ../print.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -D__DEBUG  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/_ext/1472/print.obj: > ${OBJECTDIR}/_ext/1472/print.obj.d
	@cat ${OBJECTDIR}/_ext/1472/print.dep >> ${OBJECTDIR}/_ext/1472/print.obj.d
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/print.obj.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	 
	
${OBJECTDIR}/_ext/1472/pwm.obj: ../pwm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	${MP_CC} -C ../pwm.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -D__DEBUG  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep -C ../pwm.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -D__DEBUG  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/_ext/1472/pwm.obj: > ${OBJECTDIR}/_ext/1472/pwm.obj.d
	@cat ${OBJECTDIR}/_ext/1472/pwm.dep >> ${OBJECTDIR}/_ext/1472/pwm.obj.d
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/pwm.obj.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	 
	
${OBJECTDIR}/_ext/1472/pwron.obj: ../pwron.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	${MP_CC} -C ../pwron.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -D__DEBUG  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep -C ../pwron.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -D__DEBUG  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/_ext/1472/pwron.obj: > ${OBJECTDIR}/_ext/1472/pwron.obj.d
	@cat ${OBJECTDIR}/_ext/1472/pwron.dep >> ${OBJECTDIR}/_ext/1472/pwron.obj.d
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/pwron.obj.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	 
	
${OBJECTDIR}/_ext/1472/rgl.obj: ../rgl.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	${MP_CC} -C ../rgl.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -D__DEBUG  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep -C ../rgl.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -D__DEBUG  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/_ext/1472/rgl.obj: > ${OBJECTDIR}/_ext/1472/rgl.obj.d
	@cat ${OBJECTDIR}/_ext/1472/rgl.dep >> ${OBJECTDIR}/_ext/1472/rgl.obj.d
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/rgl.obj.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	 
	
${OBJECTDIR}/_ext/1472/rtimer.obj: ../rtimer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	${MP_CC} -C ../rtimer.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -D__DEBUG  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep -C ../rtimer.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -D__DEBUG  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/_ext/1472/rtimer.obj: > ${OBJECTDIR}/_ext/1472/rtimer.obj.d
	@cat ${OBJECTDIR}/_ext/1472/rtimer.dep >> ${OBJECTDIR}/_ext/1472/rtimer.obj.d
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/rtimer.obj.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	 
	
${OBJECTDIR}/_ext/1472/timer1.obj: ../timer1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	${MP_CC} -C ../timer1.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -D__DEBUG  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep -C ../timer1.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -D__DEBUG  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/_ext/1472/timer1.obj: > ${OBJECTDIR}/_ext/1472/timer1.obj.d
	@cat ${OBJECTDIR}/_ext/1472/timer1.dep >> ${OBJECTDIR}/_ext/1472/timer1.obj.d
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/timer1.obj.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	 
	
${OBJECTDIR}/_ext/1472/konfig.obj: ../konfig.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	${MP_CC} -C ../konfig.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -D__DEBUG  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep -C ../konfig.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -D__DEBUG  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/_ext/1472/konfig.obj: > ${OBJECTDIR}/_ext/1472/konfig.obj.d
	@cat ${OBJECTDIR}/_ext/1472/konfig.dep >> ${OBJECTDIR}/_ext/1472/konfig.obj.d
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/konfig.obj.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	 
	
${OBJECTDIR}/_ext/1472/smgs_rs9b.obj: ../smgs_rs9b.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	${MP_CC} -C ../smgs_rs9b.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -D__DEBUG  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep -C ../smgs_rs9b.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -D__DEBUG  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/_ext/1472/smgs_rs9b.obj: > ${OBJECTDIR}/_ext/1472/smgs_rs9b.obj.d
	@cat ${OBJECTDIR}/_ext/1472/smgs_rs9b.dep >> ${OBJECTDIR}/_ext/1472/smgs_rs9b.obj.d
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/smgs_rs9b.obj.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	 
	
${OBJECTDIR}/_ext/1472/uart9b.obj: ../uart9b.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	${MP_CC} -C ../uart9b.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -D__DEBUG  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep -C ../uart9b.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -D__DEBUG  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/_ext/1472/uart9b.obj: > ${OBJECTDIR}/_ext/1472/uart9b.obj.d
	@cat ${OBJECTDIR}/_ext/1472/uart9b.dep >> ${OBJECTDIR}/_ext/1472/uart9b.obj.d
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/uart9b.obj.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	 
	
${OBJECTDIR}/_ext/1472/timer3.obj: ../timer3.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	${MP_CC} -C ../timer3.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -D__DEBUG  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep -C ../timer3.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -D__DEBUG  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/_ext/1472/timer3.obj: > ${OBJECTDIR}/_ext/1472/timer3.obj.d
	@cat ${OBJECTDIR}/_ext/1472/timer3.dep >> ${OBJECTDIR}/_ext/1472/timer3.obj.d
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/timer3.obj.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	 
	
${OBJECTDIR}/_ext/1472/tacho.obj: ../tacho.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	${MP_CC} -C ../tacho.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -D__DEBUG  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep -C ../tacho.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -D__DEBUG  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/_ext/1472/tacho.obj: > ${OBJECTDIR}/_ext/1472/tacho.obj.d
	@cat ${OBJECTDIR}/_ext/1472/tacho.dep >> ${OBJECTDIR}/_ext/1472/tacho.obj.d
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/tacho.obj.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	 
	
${OBJECTDIR}/_ext/1472/vntregtch.obj: ../vntregtch.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	${MP_CC} -C ../vntregtch.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -D__DEBUG  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep -C ../vntregtch.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -D__DEBUG  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/_ext/1472/vntregtch.obj: > ${OBJECTDIR}/_ext/1472/vntregtch.obj.d
	@cat ${OBJECTDIR}/_ext/1472/vntregtch.dep >> ${OBJECTDIR}/_ext/1472/vntregtch.obj.d
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/vntregtch.obj.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	 
	
${OBJECTDIR}/_ext/1472/pompa.obj: ../pompa.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	${MP_CC} -C ../pompa.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -D__DEBUG  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep -C ../pompa.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -D__DEBUG  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/_ext/1472/pompa.obj: > ${OBJECTDIR}/_ext/1472/pompa.obj.d
	@cat ${OBJECTDIR}/_ext/1472/pompa.dep >> ${OBJECTDIR}/_ext/1472/pompa.obj.d
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/pompa.obj.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	 
	
${OBJECTDIR}/_ext/1472/history.obj: ../history.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	${MP_CC} -C ../history.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -D__DEBUG  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep -C ../history.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -D__DEBUG  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/_ext/1472/history.obj: > ${OBJECTDIR}/_ext/1472/history.obj.d
	@cat ${OBJECTDIR}/_ext/1472/history.dep >> ${OBJECTDIR}/_ext/1472/history.obj.d
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/history.obj.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	 
	
else
${OBJECTDIR}/_ext/1472/ac.obj: ../ac.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	${MP_CC} -C ../ac.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep -C ../ac.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/_ext/1472/ac.obj: > ${OBJECTDIR}/_ext/1472/ac.obj.d
	@cat ${OBJECTDIR}/_ext/1472/ac.dep >> ${OBJECTDIR}/_ext/1472/ac.obj.d
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/ac.obj.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	 
	
${OBJECTDIR}/_ext/1472/bit.obj: ../bit.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	${MP_CC} -C ../bit.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep -C ../bit.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/_ext/1472/bit.obj: > ${OBJECTDIR}/_ext/1472/bit.obj.d
	@cat ${OBJECTDIR}/_ext/1472/bit.dep >> ${OBJECTDIR}/_ext/1472/bit.obj.d
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/bit.obj.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	 
	
${OBJECTDIR}/_ext/1472/i2c_s.obj: ../i2c_s.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	${MP_CC} -C ../i2c_s.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep -C ../i2c_s.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/_ext/1472/i2c_s.obj: > ${OBJECTDIR}/_ext/1472/i2c_s.obj.d
	@cat ${OBJECTDIR}/_ext/1472/i2c_s.dep >> ${OBJECTDIR}/_ext/1472/i2c_s.obj.d
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/i2c_s.obj.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	 
	
${OBJECTDIR}/_ext/1472/isr.obj: ../isr.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	${MP_CC} -C ../isr.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep -C ../isr.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/_ext/1472/isr.obj: > ${OBJECTDIR}/_ext/1472/isr.obj.d
	@cat ${OBJECTDIR}/_ext/1472/isr.dep >> ${OBJECTDIR}/_ext/1472/isr.obj.d
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/isr.obj.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	 
	
${OBJECTDIR}/_ext/1472/main.obj: ../main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	${MP_CC} -C ../main.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep -C ../main.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/_ext/1472/main.obj: > ${OBJECTDIR}/_ext/1472/main.obj.d
	@cat ${OBJECTDIR}/_ext/1472/main.dep >> ${OBJECTDIR}/_ext/1472/main.obj.d
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/main.obj.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	 
	
${OBJECTDIR}/_ext/1472/osc.obj: ../osc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	${MP_CC} -C ../osc.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep -C ../osc.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/_ext/1472/osc.obj: > ${OBJECTDIR}/_ext/1472/osc.obj.d
	@cat ${OBJECTDIR}/_ext/1472/osc.dep >> ${OBJECTDIR}/_ext/1472/osc.obj.d
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/osc.obj.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	 
	
${OBJECTDIR}/_ext/1472/pomiar.obj: ../pomiar.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	${MP_CC} -C ../pomiar.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep -C ../pomiar.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/_ext/1472/pomiar.obj: > ${OBJECTDIR}/_ext/1472/pomiar.obj.d
	@cat ${OBJECTDIR}/_ext/1472/pomiar.dep >> ${OBJECTDIR}/_ext/1472/pomiar.obj.d
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/pomiar.obj.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	 
	
${OBJECTDIR}/_ext/1472/port.obj: ../port.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	${MP_CC} -C ../port.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep -C ../port.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/_ext/1472/port.obj: > ${OBJECTDIR}/_ext/1472/port.obj.d
	@cat ${OBJECTDIR}/_ext/1472/port.dep >> ${OBJECTDIR}/_ext/1472/port.obj.d
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/port.obj.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	 
	
${OBJECTDIR}/_ext/1472/print.obj: ../print.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	${MP_CC} -C ../print.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep -C ../print.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/_ext/1472/print.obj: > ${OBJECTDIR}/_ext/1472/print.obj.d
	@cat ${OBJECTDIR}/_ext/1472/print.dep >> ${OBJECTDIR}/_ext/1472/print.obj.d
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/print.obj.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	 
	
${OBJECTDIR}/_ext/1472/pwm.obj: ../pwm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	${MP_CC} -C ../pwm.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep -C ../pwm.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/_ext/1472/pwm.obj: > ${OBJECTDIR}/_ext/1472/pwm.obj.d
	@cat ${OBJECTDIR}/_ext/1472/pwm.dep >> ${OBJECTDIR}/_ext/1472/pwm.obj.d
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/pwm.obj.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	 
	
${OBJECTDIR}/_ext/1472/pwron.obj: ../pwron.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	${MP_CC} -C ../pwron.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep -C ../pwron.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/_ext/1472/pwron.obj: > ${OBJECTDIR}/_ext/1472/pwron.obj.d
	@cat ${OBJECTDIR}/_ext/1472/pwron.dep >> ${OBJECTDIR}/_ext/1472/pwron.obj.d
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/pwron.obj.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	 
	
${OBJECTDIR}/_ext/1472/rgl.obj: ../rgl.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	${MP_CC} -C ../rgl.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep -C ../rgl.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/_ext/1472/rgl.obj: > ${OBJECTDIR}/_ext/1472/rgl.obj.d
	@cat ${OBJECTDIR}/_ext/1472/rgl.dep >> ${OBJECTDIR}/_ext/1472/rgl.obj.d
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/rgl.obj.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	 
	
${OBJECTDIR}/_ext/1472/rtimer.obj: ../rtimer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	${MP_CC} -C ../rtimer.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep -C ../rtimer.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/_ext/1472/rtimer.obj: > ${OBJECTDIR}/_ext/1472/rtimer.obj.d
	@cat ${OBJECTDIR}/_ext/1472/rtimer.dep >> ${OBJECTDIR}/_ext/1472/rtimer.obj.d
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/rtimer.obj.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	 
	
${OBJECTDIR}/_ext/1472/timer1.obj: ../timer1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	${MP_CC} -C ../timer1.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep -C ../timer1.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/_ext/1472/timer1.obj: > ${OBJECTDIR}/_ext/1472/timer1.obj.d
	@cat ${OBJECTDIR}/_ext/1472/timer1.dep >> ${OBJECTDIR}/_ext/1472/timer1.obj.d
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/timer1.obj.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	 
	
${OBJECTDIR}/_ext/1472/konfig.obj: ../konfig.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	${MP_CC} -C ../konfig.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep -C ../konfig.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/_ext/1472/konfig.obj: > ${OBJECTDIR}/_ext/1472/konfig.obj.d
	@cat ${OBJECTDIR}/_ext/1472/konfig.dep >> ${OBJECTDIR}/_ext/1472/konfig.obj.d
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/konfig.obj.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	 
	
${OBJECTDIR}/_ext/1472/smgs_rs9b.obj: ../smgs_rs9b.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	${MP_CC} -C ../smgs_rs9b.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep -C ../smgs_rs9b.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/_ext/1472/smgs_rs9b.obj: > ${OBJECTDIR}/_ext/1472/smgs_rs9b.obj.d
	@cat ${OBJECTDIR}/_ext/1472/smgs_rs9b.dep >> ${OBJECTDIR}/_ext/1472/smgs_rs9b.obj.d
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/smgs_rs9b.obj.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	 
	
${OBJECTDIR}/_ext/1472/uart9b.obj: ../uart9b.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	${MP_CC} -C ../uart9b.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep -C ../uart9b.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/_ext/1472/uart9b.obj: > ${OBJECTDIR}/_ext/1472/uart9b.obj.d
	@cat ${OBJECTDIR}/_ext/1472/uart9b.dep >> ${OBJECTDIR}/_ext/1472/uart9b.obj.d
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/uart9b.obj.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	 
	
${OBJECTDIR}/_ext/1472/timer3.obj: ../timer3.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	${MP_CC} -C ../timer3.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep -C ../timer3.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/_ext/1472/timer3.obj: > ${OBJECTDIR}/_ext/1472/timer3.obj.d
	@cat ${OBJECTDIR}/_ext/1472/timer3.dep >> ${OBJECTDIR}/_ext/1472/timer3.obj.d
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/timer3.obj.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	 
	
${OBJECTDIR}/_ext/1472/tacho.obj: ../tacho.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	${MP_CC} -C ../tacho.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep -C ../tacho.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/_ext/1472/tacho.obj: > ${OBJECTDIR}/_ext/1472/tacho.obj.d
	@cat ${OBJECTDIR}/_ext/1472/tacho.dep >> ${OBJECTDIR}/_ext/1472/tacho.obj.d
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/tacho.obj.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	 
	
${OBJECTDIR}/_ext/1472/vntregtch.obj: ../vntregtch.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	${MP_CC} -C ../vntregtch.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep -C ../vntregtch.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/_ext/1472/vntregtch.obj: > ${OBJECTDIR}/_ext/1472/vntregtch.obj.d
	@cat ${OBJECTDIR}/_ext/1472/vntregtch.dep >> ${OBJECTDIR}/_ext/1472/vntregtch.obj.d
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/vntregtch.obj.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	 
	
${OBJECTDIR}/_ext/1472/pompa.obj: ../pompa.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	${MP_CC} -C ../pompa.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep -C ../pompa.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/_ext/1472/pompa.obj: > ${OBJECTDIR}/_ext/1472/pompa.obj.d
	@cat ${OBJECTDIR}/_ext/1472/pompa.dep >> ${OBJECTDIR}/_ext/1472/pompa.obj.d
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/pompa.obj.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	 
	
${OBJECTDIR}/_ext/1472/history.obj: ../history.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	${MP_CC} -C ../history.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep -C ../history.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 --warn=0  -N31 -I".." -I"."  --outdir="${OBJECTDIR}\_ext\1472" --cp=16 -Blarge --double=24   -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/_ext/1472/history.obj: > ${OBJECTDIR}/_ext/1472/history.obj.d
	@cat ${OBJECTDIR}/_ext/1472/history.dep >> ${OBJECTDIR}/_ext/1472/history.obj.d
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/history.obj.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/T03.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_LD} $(MP_EXTRA_LD_PRE) -odist/${CND_CONF}/${IMAGE_TYPE}/T03.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  -mdist/${CND_CONF}/${IMAGE_TYPE}/T03.X.${IMAGE_TYPE}.map --summary=default,-psect,-class,+mem,-hex --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 -D__DEBUG  -N31 -I".." -I"." --warn=0 --cp=16 -Blarge --double=24    -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s" ${OBJECTFILES_QUOTED_IF_SPACED}    
	${RM} dist/${CND_CONF}/${IMAGE_TYPE}/T03.X.${IMAGE_TYPE}.hex
else
dist/${CND_CONF}/${IMAGE_TYPE}/T03.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_LD} $(MP_EXTRA_LD_PRE) -odist/${CND_CONF}/${IMAGE_TYPE}/T03.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  -mdist/${CND_CONF}/${IMAGE_TYPE}/T03.X.${IMAGE_TYPE}.map --summary=default,-psect,-class,+mem,-hex --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,-download,+ramtest,+clib,-checksum --opt=default,+asm,-asmfile,9 -N31 -I".." -I"." --warn=0 --cp=16 -Blarge --double=24    -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s" ${OBJECTFILES_QUOTED_IF_SPACED}    
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
