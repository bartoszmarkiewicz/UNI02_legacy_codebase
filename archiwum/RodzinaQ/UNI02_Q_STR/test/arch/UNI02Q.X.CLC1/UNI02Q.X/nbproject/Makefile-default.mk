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
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/UNI02Q.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/UNI02Q.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
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
SOURCEFILES_QUOTED_IF_SPACED=mcc_generated_files/i2c1_slave.c mcc_generated_files/ext_int.c mcc_generated_files/eusart1.c mcc_generated_files/interrupt_manager.c mcc_generated_files/mcc.c mcc_generated_files/pin_manager.c mcc_generated_files/pwm3.c mcc_generated_files/adcc.c mcc_generated_files/tmr1.c mcc_generated_files/memory.c mcc_generated_files/pwm1.c mcc_generated_files/pwm2.c mcc_generated_files/tmr2.c mcc_generated_files/tmr3.c mcc_generated_files/device_config.c mcc_generated_files/tmr5.c mcc_generated_files/tmr4.c mcc_generated_files/tmr6.c mcc_generated_files/tmr0.c mcc_generated_files/clc4.c mcc_generated_files/clc3.c main.c PRJ/GRZ02/i2c_proc.c PRJ/GRZ02/uart9b.c PRJ/GRZ02/i2c_s.c PRJ/GRZ02/isr.c PRJ/GRZ02/rtimer.c PRJ/GRZ02/port.c PRJ/GRZ02/ac.c PRJ/GRZ02/eeprom.c PRJ/GRZ02/keypad.c PRJ/GRZ02/konfig.c PRJ/GRZ02/pomiar.c PRJ/GRZ02/print.c PRJ/GRZ02/pwm.c PRJ/GRZ02/readdtal.c PRJ/GRZ02/wsw2.c PRJ/GRZ02/pwron.c PRJ/GRZ02/rgl.c PRJ/GRZ02/main.c PRJ/GRZ04/i2c_proc.c PRJ/GRZ04/uart9b.c PRJ/GRZ04/i2c_s.c PRJ/GRZ04/isr.c PRJ/GRZ04/rtimer.c PRJ/GRZ04/port.c PRJ/GRZ04/ac.c PRJ/GRZ04/eeprom.c PRJ/GRZ04/konfig.c PRJ/GRZ04/wsw2.c PRJ/GRZ04/print.c PRJ/GRZ04/pomiar.c PRJ/GRZ04/keypad.c PRJ/GRZ04/readdtal.c PRJ/GRZ04/pwm.c PRJ/GRZ04/tacho.c PRJ/GRZ04/pwron.c PRJ/GRZ04/rgl.c PRJ/GRZ04/main.c PRJ/UNI02_A_STR/i2c_proc.c PRJ/UNI02_A_STR/uart9b.c PRJ/UNI02_A_STR/i2c_s.c PRJ/UNI02_A_STR/isr.c PRJ/UNI02_A_STR/rtimer.c PRJ/UNI02_A_STR/port.c PRJ/UNI02_A_STR/ac.c PRJ/UNI02_A_STR/eeprom.c PRJ/UNI02_A_STR/smgs_rs9b.c PRJ/UNI02_A_STR/pomiar.c PRJ/UNI02_A_STR/konfig.c PRJ/UNI02_A_STR/pompa.c PRJ/UNI02_A_STR/pwm.c PRJ/UNI02_A_STR/rgl.c PRJ/UNI02_A_STR/print.c PRJ/UNI02_A_STR/pwron.c PRJ/UNI02_A_STR/main.c PRJ/UNI02_A_STR/history.c PRJ/UNI02_K_STR/i2c_proc.c PRJ/UNI02_K_STR/uart9b.c PRJ/UNI02_K_STR/i2c_s.c PRJ/UNI02_K_STR/isr.c PRJ/UNI02_K_STR/rtimer.c PRJ/UNI02_K_STR/port.c PRJ/UNI02_K_STR/ac.c PRJ/UNI02_K_STR/eeprom.c PRJ/UNI02_K_STR/smgs_rs9b.c PRJ/UNI02_K_STR/konfig.c PRJ/UNI02_K_STR/pomiar.c PRJ/UNI02_K_STR/tacho.c PRJ/UNI02_K_STR/vntregtch.c PRJ/UNI02_K_STR/pompa.c PRJ/UNI02_K_STR/pwm.c PRJ/UNI02_K_STR/rgl.c PRJ/UNI02_K_STR/history.c PRJ/UNI02_K_STR/print.c PRJ/UNI02_K_STR/pwron.c PRJ/UNI02_K_STR/main.c mcc_generated_files/clc5.c mcc_generated_files/clc1.c mcc_generated_files/clc2.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/mcc_generated_files/i2c1_slave.p1 ${OBJECTDIR}/mcc_generated_files/ext_int.p1 ${OBJECTDIR}/mcc_generated_files/eusart1.p1 ${OBJECTDIR}/mcc_generated_files/interrupt_manager.p1 ${OBJECTDIR}/mcc_generated_files/mcc.p1 ${OBJECTDIR}/mcc_generated_files/pin_manager.p1 ${OBJECTDIR}/mcc_generated_files/pwm3.p1 ${OBJECTDIR}/mcc_generated_files/adcc.p1 ${OBJECTDIR}/mcc_generated_files/tmr1.p1 ${OBJECTDIR}/mcc_generated_files/memory.p1 ${OBJECTDIR}/mcc_generated_files/pwm1.p1 ${OBJECTDIR}/mcc_generated_files/pwm2.p1 ${OBJECTDIR}/mcc_generated_files/tmr2.p1 ${OBJECTDIR}/mcc_generated_files/tmr3.p1 ${OBJECTDIR}/mcc_generated_files/device_config.p1 ${OBJECTDIR}/mcc_generated_files/tmr5.p1 ${OBJECTDIR}/mcc_generated_files/tmr4.p1 ${OBJECTDIR}/mcc_generated_files/tmr6.p1 ${OBJECTDIR}/mcc_generated_files/tmr0.p1 ${OBJECTDIR}/mcc_generated_files/clc4.p1 ${OBJECTDIR}/mcc_generated_files/clc3.p1 ${OBJECTDIR}/main.p1 ${OBJECTDIR}/PRJ/GRZ02/i2c_proc.p1 ${OBJECTDIR}/PRJ/GRZ02/uart9b.p1 ${OBJECTDIR}/PRJ/GRZ02/i2c_s.p1 ${OBJECTDIR}/PRJ/GRZ02/isr.p1 ${OBJECTDIR}/PRJ/GRZ02/rtimer.p1 ${OBJECTDIR}/PRJ/GRZ02/port.p1 ${OBJECTDIR}/PRJ/GRZ02/ac.p1 ${OBJECTDIR}/PRJ/GRZ02/eeprom.p1 ${OBJECTDIR}/PRJ/GRZ02/keypad.p1 ${OBJECTDIR}/PRJ/GRZ02/konfig.p1 ${OBJECTDIR}/PRJ/GRZ02/pomiar.p1 ${OBJECTDIR}/PRJ/GRZ02/print.p1 ${OBJECTDIR}/PRJ/GRZ02/pwm.p1 ${OBJECTDIR}/PRJ/GRZ02/readdtal.p1 ${OBJECTDIR}/PRJ/GRZ02/wsw2.p1 ${OBJECTDIR}/PRJ/GRZ02/pwron.p1 ${OBJECTDIR}/PRJ/GRZ02/rgl.p1 ${OBJECTDIR}/PRJ/GRZ02/main.p1 ${OBJECTDIR}/PRJ/GRZ04/i2c_proc.p1 ${OBJECTDIR}/PRJ/GRZ04/uart9b.p1 ${OBJECTDIR}/PRJ/GRZ04/i2c_s.p1 ${OBJECTDIR}/PRJ/GRZ04/isr.p1 ${OBJECTDIR}/PRJ/GRZ04/rtimer.p1 ${OBJECTDIR}/PRJ/GRZ04/port.p1 ${OBJECTDIR}/PRJ/GRZ04/ac.p1 ${OBJECTDIR}/PRJ/GRZ04/eeprom.p1 ${OBJECTDIR}/PRJ/GRZ04/konfig.p1 ${OBJECTDIR}/PRJ/GRZ04/wsw2.p1 ${OBJECTDIR}/PRJ/GRZ04/print.p1 ${OBJECTDIR}/PRJ/GRZ04/pomiar.p1 ${OBJECTDIR}/PRJ/GRZ04/keypad.p1 ${OBJECTDIR}/PRJ/GRZ04/readdtal.p1 ${OBJECTDIR}/PRJ/GRZ04/pwm.p1 ${OBJECTDIR}/PRJ/GRZ04/tacho.p1 ${OBJECTDIR}/PRJ/GRZ04/pwron.p1 ${OBJECTDIR}/PRJ/GRZ04/rgl.p1 ${OBJECTDIR}/PRJ/GRZ04/main.p1 ${OBJECTDIR}/PRJ/UNI02_A_STR/i2c_proc.p1 ${OBJECTDIR}/PRJ/UNI02_A_STR/uart9b.p1 ${OBJECTDIR}/PRJ/UNI02_A_STR/i2c_s.p1 ${OBJECTDIR}/PRJ/UNI02_A_STR/isr.p1 ${OBJECTDIR}/PRJ/UNI02_A_STR/rtimer.p1 ${OBJECTDIR}/PRJ/UNI02_A_STR/port.p1 ${OBJECTDIR}/PRJ/UNI02_A_STR/ac.p1 ${OBJECTDIR}/PRJ/UNI02_A_STR/eeprom.p1 ${OBJECTDIR}/PRJ/UNI02_A_STR/smgs_rs9b.p1 ${OBJECTDIR}/PRJ/UNI02_A_STR/pomiar.p1 ${OBJECTDIR}/PRJ/UNI02_A_STR/konfig.p1 ${OBJECTDIR}/PRJ/UNI02_A_STR/pompa.p1 ${OBJECTDIR}/PRJ/UNI02_A_STR/pwm.p1 ${OBJECTDIR}/PRJ/UNI02_A_STR/rgl.p1 ${OBJECTDIR}/PRJ/UNI02_A_STR/print.p1 ${OBJECTDIR}/PRJ/UNI02_A_STR/pwron.p1 ${OBJECTDIR}/PRJ/UNI02_A_STR/main.p1 ${OBJECTDIR}/PRJ/UNI02_A_STR/history.p1 ${OBJECTDIR}/PRJ/UNI02_K_STR/i2c_proc.p1 ${OBJECTDIR}/PRJ/UNI02_K_STR/uart9b.p1 ${OBJECTDIR}/PRJ/UNI02_K_STR/i2c_s.p1 ${OBJECTDIR}/PRJ/UNI02_K_STR/isr.p1 ${OBJECTDIR}/PRJ/UNI02_K_STR/rtimer.p1 ${OBJECTDIR}/PRJ/UNI02_K_STR/port.p1 ${OBJECTDIR}/PRJ/UNI02_K_STR/ac.p1 ${OBJECTDIR}/PRJ/UNI02_K_STR/eeprom.p1 ${OBJECTDIR}/PRJ/UNI02_K_STR/smgs_rs9b.p1 ${OBJECTDIR}/PRJ/UNI02_K_STR/konfig.p1 ${OBJECTDIR}/PRJ/UNI02_K_STR/pomiar.p1 ${OBJECTDIR}/PRJ/UNI02_K_STR/tacho.p1 ${OBJECTDIR}/PRJ/UNI02_K_STR/vntregtch.p1 ${OBJECTDIR}/PRJ/UNI02_K_STR/pompa.p1 ${OBJECTDIR}/PRJ/UNI02_K_STR/pwm.p1 ${OBJECTDIR}/PRJ/UNI02_K_STR/rgl.p1 ${OBJECTDIR}/PRJ/UNI02_K_STR/history.p1 ${OBJECTDIR}/PRJ/UNI02_K_STR/print.p1 ${OBJECTDIR}/PRJ/UNI02_K_STR/pwron.p1 ${OBJECTDIR}/PRJ/UNI02_K_STR/main.p1 ${OBJECTDIR}/mcc_generated_files/clc5.p1 ${OBJECTDIR}/mcc_generated_files/clc1.p1 ${OBJECTDIR}/mcc_generated_files/clc2.p1
POSSIBLE_DEPFILES=${OBJECTDIR}/mcc_generated_files/i2c1_slave.p1.d ${OBJECTDIR}/mcc_generated_files/ext_int.p1.d ${OBJECTDIR}/mcc_generated_files/eusart1.p1.d ${OBJECTDIR}/mcc_generated_files/interrupt_manager.p1.d ${OBJECTDIR}/mcc_generated_files/mcc.p1.d ${OBJECTDIR}/mcc_generated_files/pin_manager.p1.d ${OBJECTDIR}/mcc_generated_files/pwm3.p1.d ${OBJECTDIR}/mcc_generated_files/adcc.p1.d ${OBJECTDIR}/mcc_generated_files/tmr1.p1.d ${OBJECTDIR}/mcc_generated_files/memory.p1.d ${OBJECTDIR}/mcc_generated_files/pwm1.p1.d ${OBJECTDIR}/mcc_generated_files/pwm2.p1.d ${OBJECTDIR}/mcc_generated_files/tmr2.p1.d ${OBJECTDIR}/mcc_generated_files/tmr3.p1.d ${OBJECTDIR}/mcc_generated_files/device_config.p1.d ${OBJECTDIR}/mcc_generated_files/tmr5.p1.d ${OBJECTDIR}/mcc_generated_files/tmr4.p1.d ${OBJECTDIR}/mcc_generated_files/tmr6.p1.d ${OBJECTDIR}/mcc_generated_files/tmr0.p1.d ${OBJECTDIR}/mcc_generated_files/clc4.p1.d ${OBJECTDIR}/mcc_generated_files/clc3.p1.d ${OBJECTDIR}/main.p1.d ${OBJECTDIR}/PRJ/GRZ02/i2c_proc.p1.d ${OBJECTDIR}/PRJ/GRZ02/uart9b.p1.d ${OBJECTDIR}/PRJ/GRZ02/i2c_s.p1.d ${OBJECTDIR}/PRJ/GRZ02/isr.p1.d ${OBJECTDIR}/PRJ/GRZ02/rtimer.p1.d ${OBJECTDIR}/PRJ/GRZ02/port.p1.d ${OBJECTDIR}/PRJ/GRZ02/ac.p1.d ${OBJECTDIR}/PRJ/GRZ02/eeprom.p1.d ${OBJECTDIR}/PRJ/GRZ02/keypad.p1.d ${OBJECTDIR}/PRJ/GRZ02/konfig.p1.d ${OBJECTDIR}/PRJ/GRZ02/pomiar.p1.d ${OBJECTDIR}/PRJ/GRZ02/print.p1.d ${OBJECTDIR}/PRJ/GRZ02/pwm.p1.d ${OBJECTDIR}/PRJ/GRZ02/readdtal.p1.d ${OBJECTDIR}/PRJ/GRZ02/wsw2.p1.d ${OBJECTDIR}/PRJ/GRZ02/pwron.p1.d ${OBJECTDIR}/PRJ/GRZ02/rgl.p1.d ${OBJECTDIR}/PRJ/GRZ02/main.p1.d ${OBJECTDIR}/PRJ/GRZ04/i2c_proc.p1.d ${OBJECTDIR}/PRJ/GRZ04/uart9b.p1.d ${OBJECTDIR}/PRJ/GRZ04/i2c_s.p1.d ${OBJECTDIR}/PRJ/GRZ04/isr.p1.d ${OBJECTDIR}/PRJ/GRZ04/rtimer.p1.d ${OBJECTDIR}/PRJ/GRZ04/port.p1.d ${OBJECTDIR}/PRJ/GRZ04/ac.p1.d ${OBJECTDIR}/PRJ/GRZ04/eeprom.p1.d ${OBJECTDIR}/PRJ/GRZ04/konfig.p1.d ${OBJECTDIR}/PRJ/GRZ04/wsw2.p1.d ${OBJECTDIR}/PRJ/GRZ04/print.p1.d ${OBJECTDIR}/PRJ/GRZ04/pomiar.p1.d ${OBJECTDIR}/PRJ/GRZ04/keypad.p1.d ${OBJECTDIR}/PRJ/GRZ04/readdtal.p1.d ${OBJECTDIR}/PRJ/GRZ04/pwm.p1.d ${OBJECTDIR}/PRJ/GRZ04/tacho.p1.d ${OBJECTDIR}/PRJ/GRZ04/pwron.p1.d ${OBJECTDIR}/PRJ/GRZ04/rgl.p1.d ${OBJECTDIR}/PRJ/GRZ04/main.p1.d ${OBJECTDIR}/PRJ/UNI02_A_STR/i2c_proc.p1.d ${OBJECTDIR}/PRJ/UNI02_A_STR/uart9b.p1.d ${OBJECTDIR}/PRJ/UNI02_A_STR/i2c_s.p1.d ${OBJECTDIR}/PRJ/UNI02_A_STR/isr.p1.d ${OBJECTDIR}/PRJ/UNI02_A_STR/rtimer.p1.d ${OBJECTDIR}/PRJ/UNI02_A_STR/port.p1.d ${OBJECTDIR}/PRJ/UNI02_A_STR/ac.p1.d ${OBJECTDIR}/PRJ/UNI02_A_STR/eeprom.p1.d ${OBJECTDIR}/PRJ/UNI02_A_STR/smgs_rs9b.p1.d ${OBJECTDIR}/PRJ/UNI02_A_STR/pomiar.p1.d ${OBJECTDIR}/PRJ/UNI02_A_STR/konfig.p1.d ${OBJECTDIR}/PRJ/UNI02_A_STR/pompa.p1.d ${OBJECTDIR}/PRJ/UNI02_A_STR/pwm.p1.d ${OBJECTDIR}/PRJ/UNI02_A_STR/rgl.p1.d ${OBJECTDIR}/PRJ/UNI02_A_STR/print.p1.d ${OBJECTDIR}/PRJ/UNI02_A_STR/pwron.p1.d ${OBJECTDIR}/PRJ/UNI02_A_STR/main.p1.d ${OBJECTDIR}/PRJ/UNI02_A_STR/history.p1.d ${OBJECTDIR}/PRJ/UNI02_K_STR/i2c_proc.p1.d ${OBJECTDIR}/PRJ/UNI02_K_STR/uart9b.p1.d ${OBJECTDIR}/PRJ/UNI02_K_STR/i2c_s.p1.d ${OBJECTDIR}/PRJ/UNI02_K_STR/isr.p1.d ${OBJECTDIR}/PRJ/UNI02_K_STR/rtimer.p1.d ${OBJECTDIR}/PRJ/UNI02_K_STR/port.p1.d ${OBJECTDIR}/PRJ/UNI02_K_STR/ac.p1.d ${OBJECTDIR}/PRJ/UNI02_K_STR/eeprom.p1.d ${OBJECTDIR}/PRJ/UNI02_K_STR/smgs_rs9b.p1.d ${OBJECTDIR}/PRJ/UNI02_K_STR/konfig.p1.d ${OBJECTDIR}/PRJ/UNI02_K_STR/pomiar.p1.d ${OBJECTDIR}/PRJ/UNI02_K_STR/tacho.p1.d ${OBJECTDIR}/PRJ/UNI02_K_STR/vntregtch.p1.d ${OBJECTDIR}/PRJ/UNI02_K_STR/pompa.p1.d ${OBJECTDIR}/PRJ/UNI02_K_STR/pwm.p1.d ${OBJECTDIR}/PRJ/UNI02_K_STR/rgl.p1.d ${OBJECTDIR}/PRJ/UNI02_K_STR/history.p1.d ${OBJECTDIR}/PRJ/UNI02_K_STR/print.p1.d ${OBJECTDIR}/PRJ/UNI02_K_STR/pwron.p1.d ${OBJECTDIR}/PRJ/UNI02_K_STR/main.p1.d ${OBJECTDIR}/mcc_generated_files/clc5.p1.d ${OBJECTDIR}/mcc_generated_files/clc1.p1.d ${OBJECTDIR}/mcc_generated_files/clc2.p1.d

# Object Files
OBJECTFILES=${OBJECTDIR}/mcc_generated_files/i2c1_slave.p1 ${OBJECTDIR}/mcc_generated_files/ext_int.p1 ${OBJECTDIR}/mcc_generated_files/eusart1.p1 ${OBJECTDIR}/mcc_generated_files/interrupt_manager.p1 ${OBJECTDIR}/mcc_generated_files/mcc.p1 ${OBJECTDIR}/mcc_generated_files/pin_manager.p1 ${OBJECTDIR}/mcc_generated_files/pwm3.p1 ${OBJECTDIR}/mcc_generated_files/adcc.p1 ${OBJECTDIR}/mcc_generated_files/tmr1.p1 ${OBJECTDIR}/mcc_generated_files/memory.p1 ${OBJECTDIR}/mcc_generated_files/pwm1.p1 ${OBJECTDIR}/mcc_generated_files/pwm2.p1 ${OBJECTDIR}/mcc_generated_files/tmr2.p1 ${OBJECTDIR}/mcc_generated_files/tmr3.p1 ${OBJECTDIR}/mcc_generated_files/device_config.p1 ${OBJECTDIR}/mcc_generated_files/tmr5.p1 ${OBJECTDIR}/mcc_generated_files/tmr4.p1 ${OBJECTDIR}/mcc_generated_files/tmr6.p1 ${OBJECTDIR}/mcc_generated_files/tmr0.p1 ${OBJECTDIR}/mcc_generated_files/clc4.p1 ${OBJECTDIR}/mcc_generated_files/clc3.p1 ${OBJECTDIR}/main.p1 ${OBJECTDIR}/PRJ/GRZ02/i2c_proc.p1 ${OBJECTDIR}/PRJ/GRZ02/uart9b.p1 ${OBJECTDIR}/PRJ/GRZ02/i2c_s.p1 ${OBJECTDIR}/PRJ/GRZ02/isr.p1 ${OBJECTDIR}/PRJ/GRZ02/rtimer.p1 ${OBJECTDIR}/PRJ/GRZ02/port.p1 ${OBJECTDIR}/PRJ/GRZ02/ac.p1 ${OBJECTDIR}/PRJ/GRZ02/eeprom.p1 ${OBJECTDIR}/PRJ/GRZ02/keypad.p1 ${OBJECTDIR}/PRJ/GRZ02/konfig.p1 ${OBJECTDIR}/PRJ/GRZ02/pomiar.p1 ${OBJECTDIR}/PRJ/GRZ02/print.p1 ${OBJECTDIR}/PRJ/GRZ02/pwm.p1 ${OBJECTDIR}/PRJ/GRZ02/readdtal.p1 ${OBJECTDIR}/PRJ/GRZ02/wsw2.p1 ${OBJECTDIR}/PRJ/GRZ02/pwron.p1 ${OBJECTDIR}/PRJ/GRZ02/rgl.p1 ${OBJECTDIR}/PRJ/GRZ02/main.p1 ${OBJECTDIR}/PRJ/GRZ04/i2c_proc.p1 ${OBJECTDIR}/PRJ/GRZ04/uart9b.p1 ${OBJECTDIR}/PRJ/GRZ04/i2c_s.p1 ${OBJECTDIR}/PRJ/GRZ04/isr.p1 ${OBJECTDIR}/PRJ/GRZ04/rtimer.p1 ${OBJECTDIR}/PRJ/GRZ04/port.p1 ${OBJECTDIR}/PRJ/GRZ04/ac.p1 ${OBJECTDIR}/PRJ/GRZ04/eeprom.p1 ${OBJECTDIR}/PRJ/GRZ04/konfig.p1 ${OBJECTDIR}/PRJ/GRZ04/wsw2.p1 ${OBJECTDIR}/PRJ/GRZ04/print.p1 ${OBJECTDIR}/PRJ/GRZ04/pomiar.p1 ${OBJECTDIR}/PRJ/GRZ04/keypad.p1 ${OBJECTDIR}/PRJ/GRZ04/readdtal.p1 ${OBJECTDIR}/PRJ/GRZ04/pwm.p1 ${OBJECTDIR}/PRJ/GRZ04/tacho.p1 ${OBJECTDIR}/PRJ/GRZ04/pwron.p1 ${OBJECTDIR}/PRJ/GRZ04/rgl.p1 ${OBJECTDIR}/PRJ/GRZ04/main.p1 ${OBJECTDIR}/PRJ/UNI02_A_STR/i2c_proc.p1 ${OBJECTDIR}/PRJ/UNI02_A_STR/uart9b.p1 ${OBJECTDIR}/PRJ/UNI02_A_STR/i2c_s.p1 ${OBJECTDIR}/PRJ/UNI02_A_STR/isr.p1 ${OBJECTDIR}/PRJ/UNI02_A_STR/rtimer.p1 ${OBJECTDIR}/PRJ/UNI02_A_STR/port.p1 ${OBJECTDIR}/PRJ/UNI02_A_STR/ac.p1 ${OBJECTDIR}/PRJ/UNI02_A_STR/eeprom.p1 ${OBJECTDIR}/PRJ/UNI02_A_STR/smgs_rs9b.p1 ${OBJECTDIR}/PRJ/UNI02_A_STR/pomiar.p1 ${OBJECTDIR}/PRJ/UNI02_A_STR/konfig.p1 ${OBJECTDIR}/PRJ/UNI02_A_STR/pompa.p1 ${OBJECTDIR}/PRJ/UNI02_A_STR/pwm.p1 ${OBJECTDIR}/PRJ/UNI02_A_STR/rgl.p1 ${OBJECTDIR}/PRJ/UNI02_A_STR/print.p1 ${OBJECTDIR}/PRJ/UNI02_A_STR/pwron.p1 ${OBJECTDIR}/PRJ/UNI02_A_STR/main.p1 ${OBJECTDIR}/PRJ/UNI02_A_STR/history.p1 ${OBJECTDIR}/PRJ/UNI02_K_STR/i2c_proc.p1 ${OBJECTDIR}/PRJ/UNI02_K_STR/uart9b.p1 ${OBJECTDIR}/PRJ/UNI02_K_STR/i2c_s.p1 ${OBJECTDIR}/PRJ/UNI02_K_STR/isr.p1 ${OBJECTDIR}/PRJ/UNI02_K_STR/rtimer.p1 ${OBJECTDIR}/PRJ/UNI02_K_STR/port.p1 ${OBJECTDIR}/PRJ/UNI02_K_STR/ac.p1 ${OBJECTDIR}/PRJ/UNI02_K_STR/eeprom.p1 ${OBJECTDIR}/PRJ/UNI02_K_STR/smgs_rs9b.p1 ${OBJECTDIR}/PRJ/UNI02_K_STR/konfig.p1 ${OBJECTDIR}/PRJ/UNI02_K_STR/pomiar.p1 ${OBJECTDIR}/PRJ/UNI02_K_STR/tacho.p1 ${OBJECTDIR}/PRJ/UNI02_K_STR/vntregtch.p1 ${OBJECTDIR}/PRJ/UNI02_K_STR/pompa.p1 ${OBJECTDIR}/PRJ/UNI02_K_STR/pwm.p1 ${OBJECTDIR}/PRJ/UNI02_K_STR/rgl.p1 ${OBJECTDIR}/PRJ/UNI02_K_STR/history.p1 ${OBJECTDIR}/PRJ/UNI02_K_STR/print.p1 ${OBJECTDIR}/PRJ/UNI02_K_STR/pwron.p1 ${OBJECTDIR}/PRJ/UNI02_K_STR/main.p1 ${OBJECTDIR}/mcc_generated_files/clc5.p1 ${OBJECTDIR}/mcc_generated_files/clc1.p1 ${OBJECTDIR}/mcc_generated_files/clc2.p1

# Source Files
SOURCEFILES=mcc_generated_files/i2c1_slave.c mcc_generated_files/ext_int.c mcc_generated_files/eusart1.c mcc_generated_files/interrupt_manager.c mcc_generated_files/mcc.c mcc_generated_files/pin_manager.c mcc_generated_files/pwm3.c mcc_generated_files/adcc.c mcc_generated_files/tmr1.c mcc_generated_files/memory.c mcc_generated_files/pwm1.c mcc_generated_files/pwm2.c mcc_generated_files/tmr2.c mcc_generated_files/tmr3.c mcc_generated_files/device_config.c mcc_generated_files/tmr5.c mcc_generated_files/tmr4.c mcc_generated_files/tmr6.c mcc_generated_files/tmr0.c mcc_generated_files/clc4.c mcc_generated_files/clc3.c main.c PRJ/GRZ02/i2c_proc.c PRJ/GRZ02/uart9b.c PRJ/GRZ02/i2c_s.c PRJ/GRZ02/isr.c PRJ/GRZ02/rtimer.c PRJ/GRZ02/port.c PRJ/GRZ02/ac.c PRJ/GRZ02/eeprom.c PRJ/GRZ02/keypad.c PRJ/GRZ02/konfig.c PRJ/GRZ02/pomiar.c PRJ/GRZ02/print.c PRJ/GRZ02/pwm.c PRJ/GRZ02/readdtal.c PRJ/GRZ02/wsw2.c PRJ/GRZ02/pwron.c PRJ/GRZ02/rgl.c PRJ/GRZ02/main.c PRJ/GRZ04/i2c_proc.c PRJ/GRZ04/uart9b.c PRJ/GRZ04/i2c_s.c PRJ/GRZ04/isr.c PRJ/GRZ04/rtimer.c PRJ/GRZ04/port.c PRJ/GRZ04/ac.c PRJ/GRZ04/eeprom.c PRJ/GRZ04/konfig.c PRJ/GRZ04/wsw2.c PRJ/GRZ04/print.c PRJ/GRZ04/pomiar.c PRJ/GRZ04/keypad.c PRJ/GRZ04/readdtal.c PRJ/GRZ04/pwm.c PRJ/GRZ04/tacho.c PRJ/GRZ04/pwron.c PRJ/GRZ04/rgl.c PRJ/GRZ04/main.c PRJ/UNI02_A_STR/i2c_proc.c PRJ/UNI02_A_STR/uart9b.c PRJ/UNI02_A_STR/i2c_s.c PRJ/UNI02_A_STR/isr.c PRJ/UNI02_A_STR/rtimer.c PRJ/UNI02_A_STR/port.c PRJ/UNI02_A_STR/ac.c PRJ/UNI02_A_STR/eeprom.c PRJ/UNI02_A_STR/smgs_rs9b.c PRJ/UNI02_A_STR/pomiar.c PRJ/UNI02_A_STR/konfig.c PRJ/UNI02_A_STR/pompa.c PRJ/UNI02_A_STR/pwm.c PRJ/UNI02_A_STR/rgl.c PRJ/UNI02_A_STR/print.c PRJ/UNI02_A_STR/pwron.c PRJ/UNI02_A_STR/main.c PRJ/UNI02_A_STR/history.c PRJ/UNI02_K_STR/i2c_proc.c PRJ/UNI02_K_STR/uart9b.c PRJ/UNI02_K_STR/i2c_s.c PRJ/UNI02_K_STR/isr.c PRJ/UNI02_K_STR/rtimer.c PRJ/UNI02_K_STR/port.c PRJ/UNI02_K_STR/ac.c PRJ/UNI02_K_STR/eeprom.c PRJ/UNI02_K_STR/smgs_rs9b.c PRJ/UNI02_K_STR/konfig.c PRJ/UNI02_K_STR/pomiar.c PRJ/UNI02_K_STR/tacho.c PRJ/UNI02_K_STR/vntregtch.c PRJ/UNI02_K_STR/pompa.c PRJ/UNI02_K_STR/pwm.c PRJ/UNI02_K_STR/rgl.c PRJ/UNI02_K_STR/history.c PRJ/UNI02_K_STR/print.c PRJ/UNI02_K_STR/pwron.c PRJ/UNI02_K_STR/main.c mcc_generated_files/clc5.c mcc_generated_files/clc1.c mcc_generated_files/clc2.c



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
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/UNI02Q.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=18F47Q10
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/mcc_generated_files/i2c1_slave.p1: mcc_generated_files/i2c1_slave.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/i2c1_slave.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/i2c1_slave.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/i2c1_slave.p1 mcc_generated_files/i2c1_slave.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/i2c1_slave.d ${OBJECTDIR}/mcc_generated_files/i2c1_slave.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/i2c1_slave.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/ext_int.p1: mcc_generated_files/ext_int.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/ext_int.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/ext_int.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/ext_int.p1 mcc_generated_files/ext_int.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/ext_int.d ${OBJECTDIR}/mcc_generated_files/ext_int.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/ext_int.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/eusart1.p1: mcc_generated_files/eusart1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/eusart1.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/eusart1.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/eusart1.p1 mcc_generated_files/eusart1.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/eusart1.d ${OBJECTDIR}/mcc_generated_files/eusart1.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/eusart1.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/interrupt_manager.p1: mcc_generated_files/interrupt_manager.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/interrupt_manager.p1 mcc_generated_files/interrupt_manager.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.d ${OBJECTDIR}/mcc_generated_files/interrupt_manager.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/mcc.p1: mcc_generated_files/mcc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/mcc.p1 mcc_generated_files/mcc.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/mcc.d ${OBJECTDIR}/mcc_generated_files/mcc.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/mcc.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/pin_manager.p1: mcc_generated_files/pin_manager.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pin_manager.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pin_manager.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/pin_manager.p1 mcc_generated_files/pin_manager.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/pin_manager.d ${OBJECTDIR}/mcc_generated_files/pin_manager.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/pin_manager.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/pwm3.p1: mcc_generated_files/pwm3.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pwm3.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pwm3.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/pwm3.p1 mcc_generated_files/pwm3.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/pwm3.d ${OBJECTDIR}/mcc_generated_files/pwm3.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/pwm3.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/adcc.p1: mcc_generated_files/adcc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/adcc.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/adcc.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/adcc.p1 mcc_generated_files/adcc.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/adcc.d ${OBJECTDIR}/mcc_generated_files/adcc.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/adcc.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/tmr1.p1: mcc_generated_files/tmr1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr1.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr1.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/tmr1.p1 mcc_generated_files/tmr1.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/tmr1.d ${OBJECTDIR}/mcc_generated_files/tmr1.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/tmr1.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/memory.p1: mcc_generated_files/memory.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/memory.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/memory.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/memory.p1 mcc_generated_files/memory.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/memory.d ${OBJECTDIR}/mcc_generated_files/memory.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/memory.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/pwm1.p1: mcc_generated_files/pwm1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pwm1.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pwm1.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/pwm1.p1 mcc_generated_files/pwm1.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/pwm1.d ${OBJECTDIR}/mcc_generated_files/pwm1.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/pwm1.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/pwm2.p1: mcc_generated_files/pwm2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pwm2.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pwm2.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/pwm2.p1 mcc_generated_files/pwm2.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/pwm2.d ${OBJECTDIR}/mcc_generated_files/pwm2.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/pwm2.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/tmr2.p1: mcc_generated_files/tmr2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr2.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr2.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/tmr2.p1 mcc_generated_files/tmr2.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/tmr2.d ${OBJECTDIR}/mcc_generated_files/tmr2.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/tmr2.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/tmr3.p1: mcc_generated_files/tmr3.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr3.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr3.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/tmr3.p1 mcc_generated_files/tmr3.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/tmr3.d ${OBJECTDIR}/mcc_generated_files/tmr3.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/tmr3.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/device_config.p1: mcc_generated_files/device_config.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/device_config.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/device_config.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/device_config.p1 mcc_generated_files/device_config.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/device_config.d ${OBJECTDIR}/mcc_generated_files/device_config.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/device_config.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/tmr5.p1: mcc_generated_files/tmr5.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr5.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr5.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/tmr5.p1 mcc_generated_files/tmr5.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/tmr5.d ${OBJECTDIR}/mcc_generated_files/tmr5.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/tmr5.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/tmr4.p1: mcc_generated_files/tmr4.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr4.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr4.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/tmr4.p1 mcc_generated_files/tmr4.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/tmr4.d ${OBJECTDIR}/mcc_generated_files/tmr4.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/tmr4.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/tmr6.p1: mcc_generated_files/tmr6.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr6.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr6.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/tmr6.p1 mcc_generated_files/tmr6.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/tmr6.d ${OBJECTDIR}/mcc_generated_files/tmr6.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/tmr6.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/tmr0.p1: mcc_generated_files/tmr0.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr0.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr0.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/tmr0.p1 mcc_generated_files/tmr0.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/tmr0.d ${OBJECTDIR}/mcc_generated_files/tmr0.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/tmr0.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/clc4.p1: mcc_generated_files/clc4.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/clc4.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/clc4.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/clc4.p1 mcc_generated_files/clc4.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/clc4.d ${OBJECTDIR}/mcc_generated_files/clc4.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/clc4.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/clc3.p1: mcc_generated_files/clc3.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/clc3.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/clc3.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/clc3.p1 mcc_generated_files/clc3.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/clc3.d ${OBJECTDIR}/mcc_generated_files/clc3.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/clc3.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/main.p1: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.p1.d 
	@${RM} ${OBJECTDIR}/main.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/main.p1 main.c 
	@-${MV} ${OBJECTDIR}/main.d ${OBJECTDIR}/main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ02/i2c_proc.p1: PRJ/GRZ02/i2c_proc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ02" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/i2c_proc.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/i2c_proc.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ02/i2c_proc.p1 PRJ/GRZ02/i2c_proc.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ02/i2c_proc.d ${OBJECTDIR}/PRJ/GRZ02/i2c_proc.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ02/i2c_proc.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ02/uart9b.p1: PRJ/GRZ02/uart9b.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ02" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/uart9b.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/uart9b.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ02/uart9b.p1 PRJ/GRZ02/uart9b.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ02/uart9b.d ${OBJECTDIR}/PRJ/GRZ02/uart9b.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ02/uart9b.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ02/i2c_s.p1: PRJ/GRZ02/i2c_s.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ02" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/i2c_s.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/i2c_s.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ02/i2c_s.p1 PRJ/GRZ02/i2c_s.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ02/i2c_s.d ${OBJECTDIR}/PRJ/GRZ02/i2c_s.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ02/i2c_s.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ02/isr.p1: PRJ/GRZ02/isr.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ02" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/isr.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/isr.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ02/isr.p1 PRJ/GRZ02/isr.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ02/isr.d ${OBJECTDIR}/PRJ/GRZ02/isr.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ02/isr.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ02/rtimer.p1: PRJ/GRZ02/rtimer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ02" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/rtimer.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/rtimer.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ02/rtimer.p1 PRJ/GRZ02/rtimer.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ02/rtimer.d ${OBJECTDIR}/PRJ/GRZ02/rtimer.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ02/rtimer.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ02/port.p1: PRJ/GRZ02/port.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ02" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/port.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/port.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ02/port.p1 PRJ/GRZ02/port.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ02/port.d ${OBJECTDIR}/PRJ/GRZ02/port.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ02/port.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ02/ac.p1: PRJ/GRZ02/ac.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ02" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/ac.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/ac.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ02/ac.p1 PRJ/GRZ02/ac.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ02/ac.d ${OBJECTDIR}/PRJ/GRZ02/ac.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ02/ac.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ02/eeprom.p1: PRJ/GRZ02/eeprom.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ02" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/eeprom.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/eeprom.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ02/eeprom.p1 PRJ/GRZ02/eeprom.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ02/eeprom.d ${OBJECTDIR}/PRJ/GRZ02/eeprom.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ02/eeprom.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ02/keypad.p1: PRJ/GRZ02/keypad.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ02" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/keypad.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/keypad.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ02/keypad.p1 PRJ/GRZ02/keypad.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ02/keypad.d ${OBJECTDIR}/PRJ/GRZ02/keypad.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ02/keypad.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ02/konfig.p1: PRJ/GRZ02/konfig.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ02" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/konfig.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/konfig.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ02/konfig.p1 PRJ/GRZ02/konfig.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ02/konfig.d ${OBJECTDIR}/PRJ/GRZ02/konfig.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ02/konfig.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ02/pomiar.p1: PRJ/GRZ02/pomiar.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ02" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/pomiar.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/pomiar.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ02/pomiar.p1 PRJ/GRZ02/pomiar.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ02/pomiar.d ${OBJECTDIR}/PRJ/GRZ02/pomiar.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ02/pomiar.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ02/print.p1: PRJ/GRZ02/print.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ02" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/print.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/print.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ02/print.p1 PRJ/GRZ02/print.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ02/print.d ${OBJECTDIR}/PRJ/GRZ02/print.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ02/print.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ02/pwm.p1: PRJ/GRZ02/pwm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ02" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/pwm.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/pwm.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ02/pwm.p1 PRJ/GRZ02/pwm.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ02/pwm.d ${OBJECTDIR}/PRJ/GRZ02/pwm.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ02/pwm.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ02/readdtal.p1: PRJ/GRZ02/readdtal.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ02" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/readdtal.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/readdtal.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ02/readdtal.p1 PRJ/GRZ02/readdtal.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ02/readdtal.d ${OBJECTDIR}/PRJ/GRZ02/readdtal.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ02/readdtal.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ02/wsw2.p1: PRJ/GRZ02/wsw2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ02" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/wsw2.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/wsw2.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ02/wsw2.p1 PRJ/GRZ02/wsw2.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ02/wsw2.d ${OBJECTDIR}/PRJ/GRZ02/wsw2.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ02/wsw2.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ02/pwron.p1: PRJ/GRZ02/pwron.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ02" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/pwron.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/pwron.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ02/pwron.p1 PRJ/GRZ02/pwron.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ02/pwron.d ${OBJECTDIR}/PRJ/GRZ02/pwron.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ02/pwron.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ02/rgl.p1: PRJ/GRZ02/rgl.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ02" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/rgl.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/rgl.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ02/rgl.p1 PRJ/GRZ02/rgl.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ02/rgl.d ${OBJECTDIR}/PRJ/GRZ02/rgl.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ02/rgl.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ02/main.p1: PRJ/GRZ02/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ02" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/main.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/main.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ02/main.p1 PRJ/GRZ02/main.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ02/main.d ${OBJECTDIR}/PRJ/GRZ02/main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ02/main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ04/i2c_proc.p1: PRJ/GRZ04/i2c_proc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ04" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/i2c_proc.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/i2c_proc.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ04/i2c_proc.p1 PRJ/GRZ04/i2c_proc.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ04/i2c_proc.d ${OBJECTDIR}/PRJ/GRZ04/i2c_proc.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ04/i2c_proc.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ04/uart9b.p1: PRJ/GRZ04/uart9b.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ04" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/uart9b.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/uart9b.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ04/uart9b.p1 PRJ/GRZ04/uart9b.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ04/uart9b.d ${OBJECTDIR}/PRJ/GRZ04/uart9b.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ04/uart9b.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ04/i2c_s.p1: PRJ/GRZ04/i2c_s.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ04" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/i2c_s.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/i2c_s.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ04/i2c_s.p1 PRJ/GRZ04/i2c_s.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ04/i2c_s.d ${OBJECTDIR}/PRJ/GRZ04/i2c_s.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ04/i2c_s.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ04/isr.p1: PRJ/GRZ04/isr.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ04" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/isr.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/isr.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ04/isr.p1 PRJ/GRZ04/isr.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ04/isr.d ${OBJECTDIR}/PRJ/GRZ04/isr.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ04/isr.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ04/rtimer.p1: PRJ/GRZ04/rtimer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ04" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/rtimer.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/rtimer.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ04/rtimer.p1 PRJ/GRZ04/rtimer.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ04/rtimer.d ${OBJECTDIR}/PRJ/GRZ04/rtimer.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ04/rtimer.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ04/port.p1: PRJ/GRZ04/port.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ04" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/port.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/port.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ04/port.p1 PRJ/GRZ04/port.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ04/port.d ${OBJECTDIR}/PRJ/GRZ04/port.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ04/port.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ04/ac.p1: PRJ/GRZ04/ac.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ04" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/ac.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/ac.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ04/ac.p1 PRJ/GRZ04/ac.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ04/ac.d ${OBJECTDIR}/PRJ/GRZ04/ac.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ04/ac.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ04/eeprom.p1: PRJ/GRZ04/eeprom.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ04" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/eeprom.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/eeprom.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ04/eeprom.p1 PRJ/GRZ04/eeprom.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ04/eeprom.d ${OBJECTDIR}/PRJ/GRZ04/eeprom.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ04/eeprom.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ04/konfig.p1: PRJ/GRZ04/konfig.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ04" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/konfig.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/konfig.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ04/konfig.p1 PRJ/GRZ04/konfig.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ04/konfig.d ${OBJECTDIR}/PRJ/GRZ04/konfig.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ04/konfig.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ04/wsw2.p1: PRJ/GRZ04/wsw2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ04" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/wsw2.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/wsw2.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ04/wsw2.p1 PRJ/GRZ04/wsw2.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ04/wsw2.d ${OBJECTDIR}/PRJ/GRZ04/wsw2.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ04/wsw2.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ04/print.p1: PRJ/GRZ04/print.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ04" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/print.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/print.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ04/print.p1 PRJ/GRZ04/print.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ04/print.d ${OBJECTDIR}/PRJ/GRZ04/print.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ04/print.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ04/pomiar.p1: PRJ/GRZ04/pomiar.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ04" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/pomiar.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/pomiar.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ04/pomiar.p1 PRJ/GRZ04/pomiar.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ04/pomiar.d ${OBJECTDIR}/PRJ/GRZ04/pomiar.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ04/pomiar.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ04/keypad.p1: PRJ/GRZ04/keypad.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ04" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/keypad.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/keypad.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ04/keypad.p1 PRJ/GRZ04/keypad.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ04/keypad.d ${OBJECTDIR}/PRJ/GRZ04/keypad.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ04/keypad.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ04/readdtal.p1: PRJ/GRZ04/readdtal.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ04" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/readdtal.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/readdtal.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ04/readdtal.p1 PRJ/GRZ04/readdtal.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ04/readdtal.d ${OBJECTDIR}/PRJ/GRZ04/readdtal.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ04/readdtal.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ04/pwm.p1: PRJ/GRZ04/pwm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ04" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/pwm.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/pwm.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ04/pwm.p1 PRJ/GRZ04/pwm.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ04/pwm.d ${OBJECTDIR}/PRJ/GRZ04/pwm.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ04/pwm.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ04/tacho.p1: PRJ/GRZ04/tacho.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ04" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/tacho.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/tacho.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ04/tacho.p1 PRJ/GRZ04/tacho.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ04/tacho.d ${OBJECTDIR}/PRJ/GRZ04/tacho.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ04/tacho.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ04/pwron.p1: PRJ/GRZ04/pwron.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ04" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/pwron.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/pwron.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ04/pwron.p1 PRJ/GRZ04/pwron.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ04/pwron.d ${OBJECTDIR}/PRJ/GRZ04/pwron.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ04/pwron.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ04/rgl.p1: PRJ/GRZ04/rgl.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ04" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/rgl.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/rgl.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ04/rgl.p1 PRJ/GRZ04/rgl.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ04/rgl.d ${OBJECTDIR}/PRJ/GRZ04/rgl.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ04/rgl.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ04/main.p1: PRJ/GRZ04/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ04" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/main.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/main.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ04/main.p1 PRJ/GRZ04/main.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ04/main.d ${OBJECTDIR}/PRJ/GRZ04/main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ04/main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_A_STR/i2c_proc.p1: PRJ/UNI02_A_STR/i2c_proc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_A_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/i2c_proc.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/i2c_proc.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_A_STR/i2c_proc.p1 PRJ/UNI02_A_STR/i2c_proc.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_A_STR/i2c_proc.d ${OBJECTDIR}/PRJ/UNI02_A_STR/i2c_proc.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_A_STR/i2c_proc.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_A_STR/uart9b.p1: PRJ/UNI02_A_STR/uart9b.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_A_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/uart9b.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/uart9b.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_A_STR/uart9b.p1 PRJ/UNI02_A_STR/uart9b.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_A_STR/uart9b.d ${OBJECTDIR}/PRJ/UNI02_A_STR/uart9b.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_A_STR/uart9b.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_A_STR/i2c_s.p1: PRJ/UNI02_A_STR/i2c_s.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_A_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/i2c_s.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/i2c_s.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_A_STR/i2c_s.p1 PRJ/UNI02_A_STR/i2c_s.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_A_STR/i2c_s.d ${OBJECTDIR}/PRJ/UNI02_A_STR/i2c_s.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_A_STR/i2c_s.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_A_STR/isr.p1: PRJ/UNI02_A_STR/isr.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_A_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/isr.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/isr.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_A_STR/isr.p1 PRJ/UNI02_A_STR/isr.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_A_STR/isr.d ${OBJECTDIR}/PRJ/UNI02_A_STR/isr.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_A_STR/isr.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_A_STR/rtimer.p1: PRJ/UNI02_A_STR/rtimer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_A_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/rtimer.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/rtimer.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_A_STR/rtimer.p1 PRJ/UNI02_A_STR/rtimer.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_A_STR/rtimer.d ${OBJECTDIR}/PRJ/UNI02_A_STR/rtimer.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_A_STR/rtimer.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_A_STR/port.p1: PRJ/UNI02_A_STR/port.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_A_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/port.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/port.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_A_STR/port.p1 PRJ/UNI02_A_STR/port.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_A_STR/port.d ${OBJECTDIR}/PRJ/UNI02_A_STR/port.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_A_STR/port.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_A_STR/ac.p1: PRJ/UNI02_A_STR/ac.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_A_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/ac.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/ac.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_A_STR/ac.p1 PRJ/UNI02_A_STR/ac.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_A_STR/ac.d ${OBJECTDIR}/PRJ/UNI02_A_STR/ac.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_A_STR/ac.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_A_STR/eeprom.p1: PRJ/UNI02_A_STR/eeprom.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_A_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/eeprom.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/eeprom.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_A_STR/eeprom.p1 PRJ/UNI02_A_STR/eeprom.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_A_STR/eeprom.d ${OBJECTDIR}/PRJ/UNI02_A_STR/eeprom.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_A_STR/eeprom.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_A_STR/smgs_rs9b.p1: PRJ/UNI02_A_STR/smgs_rs9b.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_A_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/smgs_rs9b.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/smgs_rs9b.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_A_STR/smgs_rs9b.p1 PRJ/UNI02_A_STR/smgs_rs9b.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_A_STR/smgs_rs9b.d ${OBJECTDIR}/PRJ/UNI02_A_STR/smgs_rs9b.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_A_STR/smgs_rs9b.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_A_STR/pomiar.p1: PRJ/UNI02_A_STR/pomiar.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_A_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/pomiar.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/pomiar.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_A_STR/pomiar.p1 PRJ/UNI02_A_STR/pomiar.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_A_STR/pomiar.d ${OBJECTDIR}/PRJ/UNI02_A_STR/pomiar.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_A_STR/pomiar.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_A_STR/konfig.p1: PRJ/UNI02_A_STR/konfig.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_A_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/konfig.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/konfig.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_A_STR/konfig.p1 PRJ/UNI02_A_STR/konfig.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_A_STR/konfig.d ${OBJECTDIR}/PRJ/UNI02_A_STR/konfig.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_A_STR/konfig.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_A_STR/pompa.p1: PRJ/UNI02_A_STR/pompa.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_A_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/pompa.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/pompa.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_A_STR/pompa.p1 PRJ/UNI02_A_STR/pompa.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_A_STR/pompa.d ${OBJECTDIR}/PRJ/UNI02_A_STR/pompa.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_A_STR/pompa.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_A_STR/pwm.p1: PRJ/UNI02_A_STR/pwm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_A_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/pwm.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/pwm.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_A_STR/pwm.p1 PRJ/UNI02_A_STR/pwm.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_A_STR/pwm.d ${OBJECTDIR}/PRJ/UNI02_A_STR/pwm.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_A_STR/pwm.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_A_STR/rgl.p1: PRJ/UNI02_A_STR/rgl.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_A_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/rgl.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/rgl.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_A_STR/rgl.p1 PRJ/UNI02_A_STR/rgl.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_A_STR/rgl.d ${OBJECTDIR}/PRJ/UNI02_A_STR/rgl.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_A_STR/rgl.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_A_STR/print.p1: PRJ/UNI02_A_STR/print.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_A_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/print.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/print.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_A_STR/print.p1 PRJ/UNI02_A_STR/print.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_A_STR/print.d ${OBJECTDIR}/PRJ/UNI02_A_STR/print.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_A_STR/print.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_A_STR/pwron.p1: PRJ/UNI02_A_STR/pwron.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_A_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/pwron.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/pwron.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_A_STR/pwron.p1 PRJ/UNI02_A_STR/pwron.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_A_STR/pwron.d ${OBJECTDIR}/PRJ/UNI02_A_STR/pwron.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_A_STR/pwron.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_A_STR/main.p1: PRJ/UNI02_A_STR/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_A_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/main.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/main.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_A_STR/main.p1 PRJ/UNI02_A_STR/main.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_A_STR/main.d ${OBJECTDIR}/PRJ/UNI02_A_STR/main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_A_STR/main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_A_STR/history.p1: PRJ/UNI02_A_STR/history.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_A_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/history.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/history.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_A_STR/history.p1 PRJ/UNI02_A_STR/history.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_A_STR/history.d ${OBJECTDIR}/PRJ/UNI02_A_STR/history.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_A_STR/history.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_K_STR/i2c_proc.p1: PRJ/UNI02_K_STR/i2c_proc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_K_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/i2c_proc.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/i2c_proc.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_K_STR/i2c_proc.p1 PRJ/UNI02_K_STR/i2c_proc.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_K_STR/i2c_proc.d ${OBJECTDIR}/PRJ/UNI02_K_STR/i2c_proc.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_K_STR/i2c_proc.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_K_STR/uart9b.p1: PRJ/UNI02_K_STR/uart9b.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_K_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/uart9b.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/uart9b.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_K_STR/uart9b.p1 PRJ/UNI02_K_STR/uart9b.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_K_STR/uart9b.d ${OBJECTDIR}/PRJ/UNI02_K_STR/uart9b.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_K_STR/uart9b.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_K_STR/i2c_s.p1: PRJ/UNI02_K_STR/i2c_s.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_K_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/i2c_s.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/i2c_s.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_K_STR/i2c_s.p1 PRJ/UNI02_K_STR/i2c_s.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_K_STR/i2c_s.d ${OBJECTDIR}/PRJ/UNI02_K_STR/i2c_s.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_K_STR/i2c_s.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_K_STR/isr.p1: PRJ/UNI02_K_STR/isr.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_K_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/isr.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/isr.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_K_STR/isr.p1 PRJ/UNI02_K_STR/isr.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_K_STR/isr.d ${OBJECTDIR}/PRJ/UNI02_K_STR/isr.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_K_STR/isr.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_K_STR/rtimer.p1: PRJ/UNI02_K_STR/rtimer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_K_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/rtimer.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/rtimer.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_K_STR/rtimer.p1 PRJ/UNI02_K_STR/rtimer.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_K_STR/rtimer.d ${OBJECTDIR}/PRJ/UNI02_K_STR/rtimer.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_K_STR/rtimer.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_K_STR/port.p1: PRJ/UNI02_K_STR/port.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_K_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/port.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/port.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_K_STR/port.p1 PRJ/UNI02_K_STR/port.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_K_STR/port.d ${OBJECTDIR}/PRJ/UNI02_K_STR/port.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_K_STR/port.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_K_STR/ac.p1: PRJ/UNI02_K_STR/ac.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_K_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/ac.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/ac.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_K_STR/ac.p1 PRJ/UNI02_K_STR/ac.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_K_STR/ac.d ${OBJECTDIR}/PRJ/UNI02_K_STR/ac.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_K_STR/ac.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_K_STR/eeprom.p1: PRJ/UNI02_K_STR/eeprom.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_K_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/eeprom.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/eeprom.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_K_STR/eeprom.p1 PRJ/UNI02_K_STR/eeprom.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_K_STR/eeprom.d ${OBJECTDIR}/PRJ/UNI02_K_STR/eeprom.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_K_STR/eeprom.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_K_STR/smgs_rs9b.p1: PRJ/UNI02_K_STR/smgs_rs9b.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_K_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/smgs_rs9b.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/smgs_rs9b.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_K_STR/smgs_rs9b.p1 PRJ/UNI02_K_STR/smgs_rs9b.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_K_STR/smgs_rs9b.d ${OBJECTDIR}/PRJ/UNI02_K_STR/smgs_rs9b.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_K_STR/smgs_rs9b.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_K_STR/konfig.p1: PRJ/UNI02_K_STR/konfig.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_K_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/konfig.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/konfig.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_K_STR/konfig.p1 PRJ/UNI02_K_STR/konfig.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_K_STR/konfig.d ${OBJECTDIR}/PRJ/UNI02_K_STR/konfig.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_K_STR/konfig.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_K_STR/pomiar.p1: PRJ/UNI02_K_STR/pomiar.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_K_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/pomiar.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/pomiar.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_K_STR/pomiar.p1 PRJ/UNI02_K_STR/pomiar.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_K_STR/pomiar.d ${OBJECTDIR}/PRJ/UNI02_K_STR/pomiar.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_K_STR/pomiar.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_K_STR/tacho.p1: PRJ/UNI02_K_STR/tacho.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_K_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/tacho.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/tacho.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_K_STR/tacho.p1 PRJ/UNI02_K_STR/tacho.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_K_STR/tacho.d ${OBJECTDIR}/PRJ/UNI02_K_STR/tacho.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_K_STR/tacho.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_K_STR/vntregtch.p1: PRJ/UNI02_K_STR/vntregtch.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_K_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/vntregtch.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/vntregtch.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_K_STR/vntregtch.p1 PRJ/UNI02_K_STR/vntregtch.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_K_STR/vntregtch.d ${OBJECTDIR}/PRJ/UNI02_K_STR/vntregtch.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_K_STR/vntregtch.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_K_STR/pompa.p1: PRJ/UNI02_K_STR/pompa.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_K_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/pompa.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/pompa.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_K_STR/pompa.p1 PRJ/UNI02_K_STR/pompa.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_K_STR/pompa.d ${OBJECTDIR}/PRJ/UNI02_K_STR/pompa.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_K_STR/pompa.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_K_STR/pwm.p1: PRJ/UNI02_K_STR/pwm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_K_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/pwm.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/pwm.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_K_STR/pwm.p1 PRJ/UNI02_K_STR/pwm.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_K_STR/pwm.d ${OBJECTDIR}/PRJ/UNI02_K_STR/pwm.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_K_STR/pwm.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_K_STR/rgl.p1: PRJ/UNI02_K_STR/rgl.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_K_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/rgl.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/rgl.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_K_STR/rgl.p1 PRJ/UNI02_K_STR/rgl.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_K_STR/rgl.d ${OBJECTDIR}/PRJ/UNI02_K_STR/rgl.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_K_STR/rgl.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_K_STR/history.p1: PRJ/UNI02_K_STR/history.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_K_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/history.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/history.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_K_STR/history.p1 PRJ/UNI02_K_STR/history.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_K_STR/history.d ${OBJECTDIR}/PRJ/UNI02_K_STR/history.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_K_STR/history.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_K_STR/print.p1: PRJ/UNI02_K_STR/print.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_K_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/print.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/print.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_K_STR/print.p1 PRJ/UNI02_K_STR/print.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_K_STR/print.d ${OBJECTDIR}/PRJ/UNI02_K_STR/print.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_K_STR/print.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_K_STR/pwron.p1: PRJ/UNI02_K_STR/pwron.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_K_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/pwron.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/pwron.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_K_STR/pwron.p1 PRJ/UNI02_K_STR/pwron.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_K_STR/pwron.d ${OBJECTDIR}/PRJ/UNI02_K_STR/pwron.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_K_STR/pwron.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_K_STR/main.p1: PRJ/UNI02_K_STR/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_K_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/main.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/main.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_K_STR/main.p1 PRJ/UNI02_K_STR/main.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_K_STR/main.d ${OBJECTDIR}/PRJ/UNI02_K_STR/main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_K_STR/main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/clc5.p1: mcc_generated_files/clc5.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/clc5.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/clc5.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/clc5.p1 mcc_generated_files/clc5.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/clc5.d ${OBJECTDIR}/mcc_generated_files/clc5.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/clc5.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/clc1.p1: mcc_generated_files/clc1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/clc1.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/clc1.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/clc1.p1 mcc_generated_files/clc1.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/clc1.d ${OBJECTDIR}/mcc_generated_files/clc1.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/clc1.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/clc2.p1: mcc_generated_files/clc2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/clc2.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/clc2.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/clc2.p1 mcc_generated_files/clc2.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/clc2.d ${OBJECTDIR}/mcc_generated_files/clc2.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/clc2.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
else
${OBJECTDIR}/mcc_generated_files/i2c1_slave.p1: mcc_generated_files/i2c1_slave.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/i2c1_slave.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/i2c1_slave.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/i2c1_slave.p1 mcc_generated_files/i2c1_slave.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/i2c1_slave.d ${OBJECTDIR}/mcc_generated_files/i2c1_slave.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/i2c1_slave.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/ext_int.p1: mcc_generated_files/ext_int.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/ext_int.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/ext_int.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/ext_int.p1 mcc_generated_files/ext_int.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/ext_int.d ${OBJECTDIR}/mcc_generated_files/ext_int.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/ext_int.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/eusart1.p1: mcc_generated_files/eusart1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/eusart1.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/eusart1.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/eusart1.p1 mcc_generated_files/eusart1.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/eusart1.d ${OBJECTDIR}/mcc_generated_files/eusart1.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/eusart1.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/interrupt_manager.p1: mcc_generated_files/interrupt_manager.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/interrupt_manager.p1 mcc_generated_files/interrupt_manager.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.d ${OBJECTDIR}/mcc_generated_files/interrupt_manager.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/mcc.p1: mcc_generated_files/mcc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/mcc.p1 mcc_generated_files/mcc.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/mcc.d ${OBJECTDIR}/mcc_generated_files/mcc.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/mcc.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/pin_manager.p1: mcc_generated_files/pin_manager.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pin_manager.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pin_manager.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/pin_manager.p1 mcc_generated_files/pin_manager.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/pin_manager.d ${OBJECTDIR}/mcc_generated_files/pin_manager.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/pin_manager.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/pwm3.p1: mcc_generated_files/pwm3.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pwm3.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pwm3.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/pwm3.p1 mcc_generated_files/pwm3.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/pwm3.d ${OBJECTDIR}/mcc_generated_files/pwm3.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/pwm3.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/adcc.p1: mcc_generated_files/adcc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/adcc.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/adcc.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/adcc.p1 mcc_generated_files/adcc.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/adcc.d ${OBJECTDIR}/mcc_generated_files/adcc.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/adcc.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/tmr1.p1: mcc_generated_files/tmr1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr1.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr1.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/tmr1.p1 mcc_generated_files/tmr1.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/tmr1.d ${OBJECTDIR}/mcc_generated_files/tmr1.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/tmr1.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/memory.p1: mcc_generated_files/memory.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/memory.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/memory.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/memory.p1 mcc_generated_files/memory.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/memory.d ${OBJECTDIR}/mcc_generated_files/memory.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/memory.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/pwm1.p1: mcc_generated_files/pwm1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pwm1.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pwm1.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/pwm1.p1 mcc_generated_files/pwm1.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/pwm1.d ${OBJECTDIR}/mcc_generated_files/pwm1.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/pwm1.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/pwm2.p1: mcc_generated_files/pwm2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pwm2.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pwm2.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/pwm2.p1 mcc_generated_files/pwm2.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/pwm2.d ${OBJECTDIR}/mcc_generated_files/pwm2.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/pwm2.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/tmr2.p1: mcc_generated_files/tmr2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr2.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr2.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/tmr2.p1 mcc_generated_files/tmr2.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/tmr2.d ${OBJECTDIR}/mcc_generated_files/tmr2.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/tmr2.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/tmr3.p1: mcc_generated_files/tmr3.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr3.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr3.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/tmr3.p1 mcc_generated_files/tmr3.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/tmr3.d ${OBJECTDIR}/mcc_generated_files/tmr3.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/tmr3.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/device_config.p1: mcc_generated_files/device_config.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/device_config.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/device_config.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/device_config.p1 mcc_generated_files/device_config.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/device_config.d ${OBJECTDIR}/mcc_generated_files/device_config.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/device_config.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/tmr5.p1: mcc_generated_files/tmr5.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr5.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr5.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/tmr5.p1 mcc_generated_files/tmr5.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/tmr5.d ${OBJECTDIR}/mcc_generated_files/tmr5.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/tmr5.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/tmr4.p1: mcc_generated_files/tmr4.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr4.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr4.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/tmr4.p1 mcc_generated_files/tmr4.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/tmr4.d ${OBJECTDIR}/mcc_generated_files/tmr4.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/tmr4.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/tmr6.p1: mcc_generated_files/tmr6.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr6.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr6.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/tmr6.p1 mcc_generated_files/tmr6.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/tmr6.d ${OBJECTDIR}/mcc_generated_files/tmr6.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/tmr6.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/tmr0.p1: mcc_generated_files/tmr0.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr0.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr0.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/tmr0.p1 mcc_generated_files/tmr0.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/tmr0.d ${OBJECTDIR}/mcc_generated_files/tmr0.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/tmr0.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/clc4.p1: mcc_generated_files/clc4.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/clc4.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/clc4.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/clc4.p1 mcc_generated_files/clc4.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/clc4.d ${OBJECTDIR}/mcc_generated_files/clc4.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/clc4.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/clc3.p1: mcc_generated_files/clc3.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/clc3.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/clc3.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/clc3.p1 mcc_generated_files/clc3.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/clc3.d ${OBJECTDIR}/mcc_generated_files/clc3.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/clc3.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/main.p1: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.p1.d 
	@${RM} ${OBJECTDIR}/main.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/main.p1 main.c 
	@-${MV} ${OBJECTDIR}/main.d ${OBJECTDIR}/main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ02/i2c_proc.p1: PRJ/GRZ02/i2c_proc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ02" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/i2c_proc.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/i2c_proc.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ02/i2c_proc.p1 PRJ/GRZ02/i2c_proc.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ02/i2c_proc.d ${OBJECTDIR}/PRJ/GRZ02/i2c_proc.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ02/i2c_proc.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ02/uart9b.p1: PRJ/GRZ02/uart9b.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ02" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/uart9b.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/uart9b.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ02/uart9b.p1 PRJ/GRZ02/uart9b.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ02/uart9b.d ${OBJECTDIR}/PRJ/GRZ02/uart9b.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ02/uart9b.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ02/i2c_s.p1: PRJ/GRZ02/i2c_s.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ02" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/i2c_s.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/i2c_s.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ02/i2c_s.p1 PRJ/GRZ02/i2c_s.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ02/i2c_s.d ${OBJECTDIR}/PRJ/GRZ02/i2c_s.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ02/i2c_s.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ02/isr.p1: PRJ/GRZ02/isr.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ02" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/isr.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/isr.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ02/isr.p1 PRJ/GRZ02/isr.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ02/isr.d ${OBJECTDIR}/PRJ/GRZ02/isr.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ02/isr.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ02/rtimer.p1: PRJ/GRZ02/rtimer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ02" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/rtimer.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/rtimer.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ02/rtimer.p1 PRJ/GRZ02/rtimer.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ02/rtimer.d ${OBJECTDIR}/PRJ/GRZ02/rtimer.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ02/rtimer.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ02/port.p1: PRJ/GRZ02/port.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ02" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/port.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/port.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ02/port.p1 PRJ/GRZ02/port.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ02/port.d ${OBJECTDIR}/PRJ/GRZ02/port.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ02/port.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ02/ac.p1: PRJ/GRZ02/ac.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ02" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/ac.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/ac.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ02/ac.p1 PRJ/GRZ02/ac.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ02/ac.d ${OBJECTDIR}/PRJ/GRZ02/ac.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ02/ac.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ02/eeprom.p1: PRJ/GRZ02/eeprom.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ02" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/eeprom.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/eeprom.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ02/eeprom.p1 PRJ/GRZ02/eeprom.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ02/eeprom.d ${OBJECTDIR}/PRJ/GRZ02/eeprom.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ02/eeprom.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ02/keypad.p1: PRJ/GRZ02/keypad.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ02" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/keypad.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/keypad.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ02/keypad.p1 PRJ/GRZ02/keypad.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ02/keypad.d ${OBJECTDIR}/PRJ/GRZ02/keypad.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ02/keypad.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ02/konfig.p1: PRJ/GRZ02/konfig.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ02" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/konfig.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/konfig.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ02/konfig.p1 PRJ/GRZ02/konfig.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ02/konfig.d ${OBJECTDIR}/PRJ/GRZ02/konfig.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ02/konfig.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ02/pomiar.p1: PRJ/GRZ02/pomiar.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ02" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/pomiar.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/pomiar.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ02/pomiar.p1 PRJ/GRZ02/pomiar.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ02/pomiar.d ${OBJECTDIR}/PRJ/GRZ02/pomiar.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ02/pomiar.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ02/print.p1: PRJ/GRZ02/print.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ02" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/print.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/print.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ02/print.p1 PRJ/GRZ02/print.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ02/print.d ${OBJECTDIR}/PRJ/GRZ02/print.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ02/print.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ02/pwm.p1: PRJ/GRZ02/pwm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ02" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/pwm.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/pwm.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ02/pwm.p1 PRJ/GRZ02/pwm.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ02/pwm.d ${OBJECTDIR}/PRJ/GRZ02/pwm.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ02/pwm.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ02/readdtal.p1: PRJ/GRZ02/readdtal.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ02" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/readdtal.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/readdtal.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ02/readdtal.p1 PRJ/GRZ02/readdtal.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ02/readdtal.d ${OBJECTDIR}/PRJ/GRZ02/readdtal.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ02/readdtal.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ02/wsw2.p1: PRJ/GRZ02/wsw2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ02" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/wsw2.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/wsw2.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ02/wsw2.p1 PRJ/GRZ02/wsw2.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ02/wsw2.d ${OBJECTDIR}/PRJ/GRZ02/wsw2.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ02/wsw2.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ02/pwron.p1: PRJ/GRZ02/pwron.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ02" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/pwron.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/pwron.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ02/pwron.p1 PRJ/GRZ02/pwron.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ02/pwron.d ${OBJECTDIR}/PRJ/GRZ02/pwron.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ02/pwron.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ02/rgl.p1: PRJ/GRZ02/rgl.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ02" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/rgl.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/rgl.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ02/rgl.p1 PRJ/GRZ02/rgl.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ02/rgl.d ${OBJECTDIR}/PRJ/GRZ02/rgl.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ02/rgl.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ02/main.p1: PRJ/GRZ02/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ02" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/main.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ02/main.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ02/main.p1 PRJ/GRZ02/main.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ02/main.d ${OBJECTDIR}/PRJ/GRZ02/main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ02/main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ04/i2c_proc.p1: PRJ/GRZ04/i2c_proc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ04" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/i2c_proc.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/i2c_proc.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ04/i2c_proc.p1 PRJ/GRZ04/i2c_proc.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ04/i2c_proc.d ${OBJECTDIR}/PRJ/GRZ04/i2c_proc.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ04/i2c_proc.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ04/uart9b.p1: PRJ/GRZ04/uart9b.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ04" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/uart9b.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/uart9b.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ04/uart9b.p1 PRJ/GRZ04/uart9b.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ04/uart9b.d ${OBJECTDIR}/PRJ/GRZ04/uart9b.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ04/uart9b.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ04/i2c_s.p1: PRJ/GRZ04/i2c_s.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ04" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/i2c_s.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/i2c_s.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ04/i2c_s.p1 PRJ/GRZ04/i2c_s.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ04/i2c_s.d ${OBJECTDIR}/PRJ/GRZ04/i2c_s.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ04/i2c_s.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ04/isr.p1: PRJ/GRZ04/isr.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ04" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/isr.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/isr.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ04/isr.p1 PRJ/GRZ04/isr.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ04/isr.d ${OBJECTDIR}/PRJ/GRZ04/isr.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ04/isr.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ04/rtimer.p1: PRJ/GRZ04/rtimer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ04" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/rtimer.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/rtimer.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ04/rtimer.p1 PRJ/GRZ04/rtimer.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ04/rtimer.d ${OBJECTDIR}/PRJ/GRZ04/rtimer.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ04/rtimer.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ04/port.p1: PRJ/GRZ04/port.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ04" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/port.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/port.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ04/port.p1 PRJ/GRZ04/port.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ04/port.d ${OBJECTDIR}/PRJ/GRZ04/port.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ04/port.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ04/ac.p1: PRJ/GRZ04/ac.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ04" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/ac.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/ac.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ04/ac.p1 PRJ/GRZ04/ac.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ04/ac.d ${OBJECTDIR}/PRJ/GRZ04/ac.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ04/ac.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ04/eeprom.p1: PRJ/GRZ04/eeprom.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ04" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/eeprom.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/eeprom.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ04/eeprom.p1 PRJ/GRZ04/eeprom.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ04/eeprom.d ${OBJECTDIR}/PRJ/GRZ04/eeprom.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ04/eeprom.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ04/konfig.p1: PRJ/GRZ04/konfig.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ04" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/konfig.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/konfig.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ04/konfig.p1 PRJ/GRZ04/konfig.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ04/konfig.d ${OBJECTDIR}/PRJ/GRZ04/konfig.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ04/konfig.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ04/wsw2.p1: PRJ/GRZ04/wsw2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ04" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/wsw2.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/wsw2.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ04/wsw2.p1 PRJ/GRZ04/wsw2.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ04/wsw2.d ${OBJECTDIR}/PRJ/GRZ04/wsw2.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ04/wsw2.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ04/print.p1: PRJ/GRZ04/print.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ04" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/print.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/print.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ04/print.p1 PRJ/GRZ04/print.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ04/print.d ${OBJECTDIR}/PRJ/GRZ04/print.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ04/print.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ04/pomiar.p1: PRJ/GRZ04/pomiar.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ04" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/pomiar.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/pomiar.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ04/pomiar.p1 PRJ/GRZ04/pomiar.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ04/pomiar.d ${OBJECTDIR}/PRJ/GRZ04/pomiar.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ04/pomiar.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ04/keypad.p1: PRJ/GRZ04/keypad.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ04" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/keypad.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/keypad.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ04/keypad.p1 PRJ/GRZ04/keypad.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ04/keypad.d ${OBJECTDIR}/PRJ/GRZ04/keypad.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ04/keypad.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ04/readdtal.p1: PRJ/GRZ04/readdtal.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ04" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/readdtal.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/readdtal.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ04/readdtal.p1 PRJ/GRZ04/readdtal.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ04/readdtal.d ${OBJECTDIR}/PRJ/GRZ04/readdtal.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ04/readdtal.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ04/pwm.p1: PRJ/GRZ04/pwm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ04" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/pwm.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/pwm.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ04/pwm.p1 PRJ/GRZ04/pwm.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ04/pwm.d ${OBJECTDIR}/PRJ/GRZ04/pwm.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ04/pwm.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ04/tacho.p1: PRJ/GRZ04/tacho.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ04" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/tacho.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/tacho.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ04/tacho.p1 PRJ/GRZ04/tacho.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ04/tacho.d ${OBJECTDIR}/PRJ/GRZ04/tacho.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ04/tacho.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ04/pwron.p1: PRJ/GRZ04/pwron.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ04" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/pwron.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/pwron.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ04/pwron.p1 PRJ/GRZ04/pwron.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ04/pwron.d ${OBJECTDIR}/PRJ/GRZ04/pwron.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ04/pwron.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ04/rgl.p1: PRJ/GRZ04/rgl.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ04" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/rgl.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/rgl.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ04/rgl.p1 PRJ/GRZ04/rgl.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ04/rgl.d ${OBJECTDIR}/PRJ/GRZ04/rgl.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ04/rgl.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/GRZ04/main.p1: PRJ/GRZ04/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/GRZ04" 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/main.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/GRZ04/main.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/GRZ04/main.p1 PRJ/GRZ04/main.c 
	@-${MV} ${OBJECTDIR}/PRJ/GRZ04/main.d ${OBJECTDIR}/PRJ/GRZ04/main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/GRZ04/main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_A_STR/i2c_proc.p1: PRJ/UNI02_A_STR/i2c_proc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_A_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/i2c_proc.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/i2c_proc.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_A_STR/i2c_proc.p1 PRJ/UNI02_A_STR/i2c_proc.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_A_STR/i2c_proc.d ${OBJECTDIR}/PRJ/UNI02_A_STR/i2c_proc.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_A_STR/i2c_proc.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_A_STR/uart9b.p1: PRJ/UNI02_A_STR/uart9b.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_A_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/uart9b.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/uart9b.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_A_STR/uart9b.p1 PRJ/UNI02_A_STR/uart9b.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_A_STR/uart9b.d ${OBJECTDIR}/PRJ/UNI02_A_STR/uart9b.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_A_STR/uart9b.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_A_STR/i2c_s.p1: PRJ/UNI02_A_STR/i2c_s.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_A_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/i2c_s.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/i2c_s.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_A_STR/i2c_s.p1 PRJ/UNI02_A_STR/i2c_s.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_A_STR/i2c_s.d ${OBJECTDIR}/PRJ/UNI02_A_STR/i2c_s.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_A_STR/i2c_s.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_A_STR/isr.p1: PRJ/UNI02_A_STR/isr.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_A_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/isr.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/isr.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_A_STR/isr.p1 PRJ/UNI02_A_STR/isr.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_A_STR/isr.d ${OBJECTDIR}/PRJ/UNI02_A_STR/isr.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_A_STR/isr.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_A_STR/rtimer.p1: PRJ/UNI02_A_STR/rtimer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_A_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/rtimer.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/rtimer.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_A_STR/rtimer.p1 PRJ/UNI02_A_STR/rtimer.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_A_STR/rtimer.d ${OBJECTDIR}/PRJ/UNI02_A_STR/rtimer.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_A_STR/rtimer.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_A_STR/port.p1: PRJ/UNI02_A_STR/port.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_A_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/port.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/port.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_A_STR/port.p1 PRJ/UNI02_A_STR/port.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_A_STR/port.d ${OBJECTDIR}/PRJ/UNI02_A_STR/port.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_A_STR/port.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_A_STR/ac.p1: PRJ/UNI02_A_STR/ac.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_A_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/ac.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/ac.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_A_STR/ac.p1 PRJ/UNI02_A_STR/ac.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_A_STR/ac.d ${OBJECTDIR}/PRJ/UNI02_A_STR/ac.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_A_STR/ac.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_A_STR/eeprom.p1: PRJ/UNI02_A_STR/eeprom.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_A_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/eeprom.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/eeprom.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_A_STR/eeprom.p1 PRJ/UNI02_A_STR/eeprom.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_A_STR/eeprom.d ${OBJECTDIR}/PRJ/UNI02_A_STR/eeprom.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_A_STR/eeprom.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_A_STR/smgs_rs9b.p1: PRJ/UNI02_A_STR/smgs_rs9b.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_A_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/smgs_rs9b.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/smgs_rs9b.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_A_STR/smgs_rs9b.p1 PRJ/UNI02_A_STR/smgs_rs9b.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_A_STR/smgs_rs9b.d ${OBJECTDIR}/PRJ/UNI02_A_STR/smgs_rs9b.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_A_STR/smgs_rs9b.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_A_STR/pomiar.p1: PRJ/UNI02_A_STR/pomiar.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_A_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/pomiar.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/pomiar.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_A_STR/pomiar.p1 PRJ/UNI02_A_STR/pomiar.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_A_STR/pomiar.d ${OBJECTDIR}/PRJ/UNI02_A_STR/pomiar.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_A_STR/pomiar.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_A_STR/konfig.p1: PRJ/UNI02_A_STR/konfig.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_A_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/konfig.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/konfig.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_A_STR/konfig.p1 PRJ/UNI02_A_STR/konfig.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_A_STR/konfig.d ${OBJECTDIR}/PRJ/UNI02_A_STR/konfig.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_A_STR/konfig.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_A_STR/pompa.p1: PRJ/UNI02_A_STR/pompa.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_A_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/pompa.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/pompa.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_A_STR/pompa.p1 PRJ/UNI02_A_STR/pompa.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_A_STR/pompa.d ${OBJECTDIR}/PRJ/UNI02_A_STR/pompa.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_A_STR/pompa.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_A_STR/pwm.p1: PRJ/UNI02_A_STR/pwm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_A_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/pwm.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/pwm.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_A_STR/pwm.p1 PRJ/UNI02_A_STR/pwm.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_A_STR/pwm.d ${OBJECTDIR}/PRJ/UNI02_A_STR/pwm.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_A_STR/pwm.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_A_STR/rgl.p1: PRJ/UNI02_A_STR/rgl.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_A_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/rgl.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/rgl.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_A_STR/rgl.p1 PRJ/UNI02_A_STR/rgl.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_A_STR/rgl.d ${OBJECTDIR}/PRJ/UNI02_A_STR/rgl.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_A_STR/rgl.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_A_STR/print.p1: PRJ/UNI02_A_STR/print.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_A_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/print.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/print.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_A_STR/print.p1 PRJ/UNI02_A_STR/print.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_A_STR/print.d ${OBJECTDIR}/PRJ/UNI02_A_STR/print.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_A_STR/print.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_A_STR/pwron.p1: PRJ/UNI02_A_STR/pwron.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_A_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/pwron.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/pwron.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_A_STR/pwron.p1 PRJ/UNI02_A_STR/pwron.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_A_STR/pwron.d ${OBJECTDIR}/PRJ/UNI02_A_STR/pwron.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_A_STR/pwron.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_A_STR/main.p1: PRJ/UNI02_A_STR/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_A_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/main.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/main.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_A_STR/main.p1 PRJ/UNI02_A_STR/main.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_A_STR/main.d ${OBJECTDIR}/PRJ/UNI02_A_STR/main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_A_STR/main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_A_STR/history.p1: PRJ/UNI02_A_STR/history.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_A_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/history.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_A_STR/history.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_A_STR/history.p1 PRJ/UNI02_A_STR/history.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_A_STR/history.d ${OBJECTDIR}/PRJ/UNI02_A_STR/history.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_A_STR/history.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_K_STR/i2c_proc.p1: PRJ/UNI02_K_STR/i2c_proc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_K_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/i2c_proc.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/i2c_proc.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_K_STR/i2c_proc.p1 PRJ/UNI02_K_STR/i2c_proc.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_K_STR/i2c_proc.d ${OBJECTDIR}/PRJ/UNI02_K_STR/i2c_proc.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_K_STR/i2c_proc.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_K_STR/uart9b.p1: PRJ/UNI02_K_STR/uart9b.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_K_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/uart9b.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/uart9b.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_K_STR/uart9b.p1 PRJ/UNI02_K_STR/uart9b.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_K_STR/uart9b.d ${OBJECTDIR}/PRJ/UNI02_K_STR/uart9b.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_K_STR/uart9b.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_K_STR/i2c_s.p1: PRJ/UNI02_K_STR/i2c_s.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_K_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/i2c_s.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/i2c_s.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_K_STR/i2c_s.p1 PRJ/UNI02_K_STR/i2c_s.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_K_STR/i2c_s.d ${OBJECTDIR}/PRJ/UNI02_K_STR/i2c_s.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_K_STR/i2c_s.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_K_STR/isr.p1: PRJ/UNI02_K_STR/isr.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_K_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/isr.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/isr.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_K_STR/isr.p1 PRJ/UNI02_K_STR/isr.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_K_STR/isr.d ${OBJECTDIR}/PRJ/UNI02_K_STR/isr.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_K_STR/isr.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_K_STR/rtimer.p1: PRJ/UNI02_K_STR/rtimer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_K_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/rtimer.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/rtimer.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_K_STR/rtimer.p1 PRJ/UNI02_K_STR/rtimer.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_K_STR/rtimer.d ${OBJECTDIR}/PRJ/UNI02_K_STR/rtimer.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_K_STR/rtimer.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_K_STR/port.p1: PRJ/UNI02_K_STR/port.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_K_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/port.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/port.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_K_STR/port.p1 PRJ/UNI02_K_STR/port.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_K_STR/port.d ${OBJECTDIR}/PRJ/UNI02_K_STR/port.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_K_STR/port.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_K_STR/ac.p1: PRJ/UNI02_K_STR/ac.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_K_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/ac.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/ac.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_K_STR/ac.p1 PRJ/UNI02_K_STR/ac.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_K_STR/ac.d ${OBJECTDIR}/PRJ/UNI02_K_STR/ac.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_K_STR/ac.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_K_STR/eeprom.p1: PRJ/UNI02_K_STR/eeprom.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_K_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/eeprom.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/eeprom.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_K_STR/eeprom.p1 PRJ/UNI02_K_STR/eeprom.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_K_STR/eeprom.d ${OBJECTDIR}/PRJ/UNI02_K_STR/eeprom.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_K_STR/eeprom.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_K_STR/smgs_rs9b.p1: PRJ/UNI02_K_STR/smgs_rs9b.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_K_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/smgs_rs9b.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/smgs_rs9b.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_K_STR/smgs_rs9b.p1 PRJ/UNI02_K_STR/smgs_rs9b.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_K_STR/smgs_rs9b.d ${OBJECTDIR}/PRJ/UNI02_K_STR/smgs_rs9b.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_K_STR/smgs_rs9b.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_K_STR/konfig.p1: PRJ/UNI02_K_STR/konfig.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_K_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/konfig.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/konfig.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_K_STR/konfig.p1 PRJ/UNI02_K_STR/konfig.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_K_STR/konfig.d ${OBJECTDIR}/PRJ/UNI02_K_STR/konfig.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_K_STR/konfig.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_K_STR/pomiar.p1: PRJ/UNI02_K_STR/pomiar.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_K_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/pomiar.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/pomiar.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_K_STR/pomiar.p1 PRJ/UNI02_K_STR/pomiar.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_K_STR/pomiar.d ${OBJECTDIR}/PRJ/UNI02_K_STR/pomiar.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_K_STR/pomiar.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_K_STR/tacho.p1: PRJ/UNI02_K_STR/tacho.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_K_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/tacho.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/tacho.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_K_STR/tacho.p1 PRJ/UNI02_K_STR/tacho.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_K_STR/tacho.d ${OBJECTDIR}/PRJ/UNI02_K_STR/tacho.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_K_STR/tacho.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_K_STR/vntregtch.p1: PRJ/UNI02_K_STR/vntregtch.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_K_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/vntregtch.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/vntregtch.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_K_STR/vntregtch.p1 PRJ/UNI02_K_STR/vntregtch.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_K_STR/vntregtch.d ${OBJECTDIR}/PRJ/UNI02_K_STR/vntregtch.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_K_STR/vntregtch.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_K_STR/pompa.p1: PRJ/UNI02_K_STR/pompa.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_K_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/pompa.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/pompa.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_K_STR/pompa.p1 PRJ/UNI02_K_STR/pompa.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_K_STR/pompa.d ${OBJECTDIR}/PRJ/UNI02_K_STR/pompa.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_K_STR/pompa.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_K_STR/pwm.p1: PRJ/UNI02_K_STR/pwm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_K_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/pwm.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/pwm.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_K_STR/pwm.p1 PRJ/UNI02_K_STR/pwm.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_K_STR/pwm.d ${OBJECTDIR}/PRJ/UNI02_K_STR/pwm.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_K_STR/pwm.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_K_STR/rgl.p1: PRJ/UNI02_K_STR/rgl.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_K_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/rgl.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/rgl.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_K_STR/rgl.p1 PRJ/UNI02_K_STR/rgl.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_K_STR/rgl.d ${OBJECTDIR}/PRJ/UNI02_K_STR/rgl.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_K_STR/rgl.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_K_STR/history.p1: PRJ/UNI02_K_STR/history.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_K_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/history.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/history.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_K_STR/history.p1 PRJ/UNI02_K_STR/history.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_K_STR/history.d ${OBJECTDIR}/PRJ/UNI02_K_STR/history.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_K_STR/history.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_K_STR/print.p1: PRJ/UNI02_K_STR/print.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_K_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/print.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/print.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_K_STR/print.p1 PRJ/UNI02_K_STR/print.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_K_STR/print.d ${OBJECTDIR}/PRJ/UNI02_K_STR/print.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_K_STR/print.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_K_STR/pwron.p1: PRJ/UNI02_K_STR/pwron.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_K_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/pwron.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/pwron.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_K_STR/pwron.p1 PRJ/UNI02_K_STR/pwron.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_K_STR/pwron.d ${OBJECTDIR}/PRJ/UNI02_K_STR/pwron.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_K_STR/pwron.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/PRJ/UNI02_K_STR/main.p1: PRJ/UNI02_K_STR/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/PRJ/UNI02_K_STR" 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/main.p1.d 
	@${RM} ${OBJECTDIR}/PRJ/UNI02_K_STR/main.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/PRJ/UNI02_K_STR/main.p1 PRJ/UNI02_K_STR/main.c 
	@-${MV} ${OBJECTDIR}/PRJ/UNI02_K_STR/main.d ${OBJECTDIR}/PRJ/UNI02_K_STR/main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/PRJ/UNI02_K_STR/main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/clc5.p1: mcc_generated_files/clc5.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/clc5.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/clc5.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/clc5.p1 mcc_generated_files/clc5.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/clc5.d ${OBJECTDIR}/mcc_generated_files/clc5.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/clc5.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/clc1.p1: mcc_generated_files/clc1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/clc1.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/clc1.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/clc1.p1 mcc_generated_files/clc1.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/clc1.d ${OBJECTDIR}/mcc_generated_files/clc1.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/clc1.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/clc2.p1: mcc_generated_files/clc2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/clc2.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/clc2.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/clc2.p1 mcc_generated_files/clc2.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/clc2.d ${OBJECTDIR}/mcc_generated_files/clc2.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/clc2.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/UNI02Q.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) --chip=$(MP_PROCESSOR_OPTION) -G -mdist/${CND_CONF}/${IMAGE_TYPE}/UNI02Q.X.${IMAGE_TYPE}.map  -D__DEBUG=1  --debugger=none  -DXPRJ_default=$(CND_CONF)    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"        $(COMPARISON_BUILD) --memorysummary dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml -odist/${CND_CONF}/${IMAGE_TYPE}/UNI02Q.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}     
	@${RM} dist/${CND_CONF}/${IMAGE_TYPE}/UNI02Q.X.${IMAGE_TYPE}.hex 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/UNI02Q.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) --chip=$(MP_PROCESSOR_OPTION) -G -mdist/${CND_CONF}/${IMAGE_TYPE}/UNI02Q.X.${IMAGE_TYPE}.map  -DXPRJ_default=$(CND_CONF)    --double=24 --float=24 --emi=wordwrite --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=std -P -N255 --warn=-3 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     $(COMPARISON_BUILD) --memorysummary dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml -odist/${CND_CONF}/${IMAGE_TYPE}/UNI02Q.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}     
	
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
