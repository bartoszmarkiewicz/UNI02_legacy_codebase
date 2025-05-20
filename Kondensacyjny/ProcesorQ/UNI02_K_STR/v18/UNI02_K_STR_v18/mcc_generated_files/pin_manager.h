/**
  @Generated Pin Manager Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

  @Summary:
    This is the Pin Manager file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides APIs for driver for .
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.78.1
        Device            :  PIC18F47Q10
        Driver Version    :  2.11
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.10 and above
        MPLAB 	          :  MPLAB X 5.30	
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

/**
  Section: Included Files
*/

#include <xc.h>

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set COTemp aliases
#define COTemp_TRIS                 TRISAbits.TRISA0
#define COTemp_LAT                  LATAbits.LATA0
#define COTemp_PORT                 PORTAbits.RA0
#define COTemp_WPU                  WPUAbits.WPUA0
#define COTemp_OD                   ODCONAbits.ODCA0
#define COTemp_ANS                  ANSELAbits.ANSELA0
#define COTemp_SetHigh()            do { LATAbits.LATA0 = 1; } while(0)
#define COTemp_SetLow()             do { LATAbits.LATA0 = 0; } while(0)
#define COTemp_Toggle()             do { LATAbits.LATA0 = ~LATAbits.LATA0; } while(0)
#define COTemp_GetValue()           PORTAbits.RA0
#define COTemp_SetDigitalInput()    do { TRISAbits.TRISA0 = 1; } while(0)
#define COTemp_SetDigitalOutput()   do { TRISAbits.TRISA0 = 0; } while(0)
#define COTemp_SetPullup()          do { WPUAbits.WPUA0 = 1; } while(0)
#define COTemp_ResetPullup()        do { WPUAbits.WPUA0 = 0; } while(0)
#define COTemp_SetPushPull()        do { ODCONAbits.ODCA0 = 0; } while(0)
#define COTemp_SetOpenDrain()       do { ODCONAbits.ODCA0 = 1; } while(0)
#define COTemp_SetAnalogMode()      do { ANSELAbits.ANSELA0 = 1; } while(0)
#define COTemp_SetDigitalMode()     do { ANSELAbits.ANSELA0 = 0; } while(0)

// get/set CWTemp aliases
#define CWTemp_TRIS                 TRISAbits.TRISA1
#define CWTemp_LAT                  LATAbits.LATA1
#define CWTemp_PORT                 PORTAbits.RA1
#define CWTemp_WPU                  WPUAbits.WPUA1
#define CWTemp_OD                   ODCONAbits.ODCA1
#define CWTemp_ANS                  ANSELAbits.ANSELA1
#define CWTemp_SetHigh()            do { LATAbits.LATA1 = 1; } while(0)
#define CWTemp_SetLow()             do { LATAbits.LATA1 = 0; } while(0)
#define CWTemp_Toggle()             do { LATAbits.LATA1 = ~LATAbits.LATA1; } while(0)
#define CWTemp_GetValue()           PORTAbits.RA1
#define CWTemp_SetDigitalInput()    do { TRISAbits.TRISA1 = 1; } while(0)
#define CWTemp_SetDigitalOutput()   do { TRISAbits.TRISA1 = 0; } while(0)
#define CWTemp_SetPullup()          do { WPUAbits.WPUA1 = 1; } while(0)
#define CWTemp_ResetPullup()        do { WPUAbits.WPUA1 = 0; } while(0)
#define CWTemp_SetPushPull()        do { ODCONAbits.ODCA1 = 0; } while(0)
#define CWTemp_SetOpenDrain()       do { ODCONAbits.ODCA1 = 1; } while(0)
#define CWTemp_SetAnalogMode()      do { ANSELAbits.ANSELA1 = 1; } while(0)
#define CWTemp_SetDigitalMode()     do { ANSELAbits.ANSELA1 = 0; } while(0)

// get/set ZWTemp aliases
#define ZWTemp_TRIS                 TRISAbits.TRISA2
#define ZWTemp_LAT                  LATAbits.LATA2
#define ZWTemp_PORT                 PORTAbits.RA2
#define ZWTemp_WPU                  WPUAbits.WPUA2
#define ZWTemp_OD                   ODCONAbits.ODCA2
#define ZWTemp_ANS                  ANSELAbits.ANSELA2
#define ZWTemp_SetHigh()            do { LATAbits.LATA2 = 1; } while(0)
#define ZWTemp_SetLow()             do { LATAbits.LATA2 = 0; } while(0)
#define ZWTemp_Toggle()             do { LATAbits.LATA2 = ~LATAbits.LATA2; } while(0)
#define ZWTemp_GetValue()           PORTAbits.RA2
#define ZWTemp_SetDigitalInput()    do { TRISAbits.TRISA2 = 1; } while(0)
#define ZWTemp_SetDigitalOutput()   do { TRISAbits.TRISA2 = 0; } while(0)
#define ZWTemp_SetPullup()          do { WPUAbits.WPUA2 = 1; } while(0)
#define ZWTemp_ResetPullup()        do { WPUAbits.WPUA2 = 0; } while(0)
#define ZWTemp_SetPushPull()        do { ODCONAbits.ODCA2 = 0; } while(0)
#define ZWTemp_SetOpenDrain()       do { ODCONAbits.ODCA2 = 1; } while(0)
#define ZWTemp_SetAnalogMode()      do { ANSELAbits.ANSELA2 = 1; } while(0)
#define ZWTemp_SetDigitalMode()     do { ANSELAbits.ANSELA2 = 0; } while(0)

// get/set CisnCO aliases
#define CisnCO_TRIS                 TRISAbits.TRISA3
#define CisnCO_LAT                  LATAbits.LATA3
#define CisnCO_PORT                 PORTAbits.RA3
#define CisnCO_WPU                  WPUAbits.WPUA3
#define CisnCO_OD                   ODCONAbits.ODCA3
#define CisnCO_ANS                  ANSELAbits.ANSELA3
#define CisnCO_SetHigh()            do { LATAbits.LATA3 = 1; } while(0)
#define CisnCO_SetLow()             do { LATAbits.LATA3 = 0; } while(0)
#define CisnCO_Toggle()             do { LATAbits.LATA3 = ~LATAbits.LATA3; } while(0)
#define CisnCO_GetValue()           PORTAbits.RA3
#define CisnCO_SetDigitalInput()    do { TRISAbits.TRISA3 = 1; } while(0)
#define CisnCO_SetDigitalOutput()   do { TRISAbits.TRISA3 = 0; } while(0)
#define CisnCO_SetPullup()          do { WPUAbits.WPUA3 = 1; } while(0)
#define CisnCO_ResetPullup()        do { WPUAbits.WPUA3 = 0; } while(0)
#define CisnCO_SetPushPull()        do { ODCONAbits.ODCA3 = 0; } while(0)
#define CisnCO_SetOpenDrain()       do { ODCONAbits.ODCA3 = 1; } while(0)
#define CisnCO_SetAnalogMode()      do { ANSELAbits.ANSELA3 = 1; } while(0)
#define CisnCO_SetDigitalMode()     do { ANSELAbits.ANSELA3 = 0; } while(0)

// get/set _PRZ_CW aliases
#define _PRZ_CW_TRIS                 TRISAbits.TRISA4
#define _PRZ_CW_LAT                  LATAbits.LATA4
#define _PRZ_CW_PORT                 PORTAbits.RA4
#define _PRZ_CW_WPU                  WPUAbits.WPUA4
#define _PRZ_CW_OD                   ODCONAbits.ODCA4
#define _PRZ_CW_ANS                  ANSELAbits.ANSELA4
#define _PRZ_CW_SetHigh()            do { LATAbits.LATA4 = 1; } while(0)
#define _PRZ_CW_SetLow()             do { LATAbits.LATA4 = 0; } while(0)
#define _PRZ_CW_Toggle()             do { LATAbits.LATA4 = ~LATAbits.LATA4; } while(0)
#define _PRZ_CW_GetValue()           PORTAbits.RA4
#define _PRZ_CW_SetDigitalInput()    do { TRISAbits.TRISA4 = 1; } while(0)
#define _PRZ_CW_SetDigitalOutput()   do { TRISAbits.TRISA4 = 0; } while(0)
#define _PRZ_CW_SetPullup()          do { WPUAbits.WPUA4 = 1; } while(0)
#define _PRZ_CW_ResetPullup()        do { WPUAbits.WPUA4 = 0; } while(0)
#define _PRZ_CW_SetPushPull()        do { ODCONAbits.ODCA4 = 0; } while(0)
#define _PRZ_CW_SetOpenDrain()       do { ODCONAbits.ODCA4 = 1; } while(0)
#define _PRZ_CW_SetAnalogMode()      do { ANSELAbits.ANSELA4 = 1; } while(0)
#define _PRZ_CW_SetDigitalMode()     do { ANSELAbits.ANSELA4 = 0; } while(0)

// get/set INTemp aliases
#define INTemp_TRIS                 TRISAbits.TRISA5
#define INTemp_LAT                  LATAbits.LATA5
#define INTemp_PORT                 PORTAbits.RA5
#define INTemp_WPU                  WPUAbits.WPUA5
#define INTemp_OD                   ODCONAbits.ODCA5
#define INTemp_ANS                  ANSELAbits.ANSELA5
#define INTemp_SetHigh()            do { LATAbits.LATA5 = 1; } while(0)
#define INTemp_SetLow()             do { LATAbits.LATA5 = 0; } while(0)
#define INTemp_Toggle()             do { LATAbits.LATA5 = ~LATAbits.LATA5; } while(0)
#define INTemp_GetValue()           PORTAbits.RA5
#define INTemp_SetDigitalInput()    do { TRISAbits.TRISA5 = 1; } while(0)
#define INTemp_SetDigitalOutput()   do { TRISAbits.TRISA5 = 0; } while(0)
#define INTemp_SetPullup()          do { WPUAbits.WPUA5 = 1; } while(0)
#define INTemp_ResetPullup()        do { WPUAbits.WPUA5 = 0; } while(0)
#define INTemp_SetPushPull()        do { ODCONAbits.ODCA5 = 0; } while(0)
#define INTemp_SetOpenDrain()       do { ODCONAbits.ODCA5 = 1; } while(0)
#define INTemp_SetAnalogMode()      do { ANSELAbits.ANSELA5 = 1; } while(0)
#define INTemp_SetDigitalMode()     do { ANSELAbits.ANSELA5 = 0; } while(0)

// get/set _CIG_NO aliases
#define _CIG_NO_TRIS                 TRISAbits.TRISA6
#define _CIG_NO_LAT                  LATAbits.LATA6
#define _CIG_NO_PORT                 PORTAbits.RA6
#define _CIG_NO_WPU                  WPUAbits.WPUA6
#define _CIG_NO_OD                   ODCONAbits.ODCA6
#define _CIG_NO_ANS                  ANSELAbits.ANSELA6
#define _CIG_NO_SetHigh()            do { LATAbits.LATA6 = 1; } while(0)
#define _CIG_NO_SetLow()             do { LATAbits.LATA6 = 0; } while(0)
#define _CIG_NO_Toggle()             do { LATAbits.LATA6 = ~LATAbits.LATA6; } while(0)
#define _CIG_NO_GetValue()           PORTAbits.RA6
#define _CIG_NO_SetDigitalInput()    do { TRISAbits.TRISA6 = 1; } while(0)
#define _CIG_NO_SetDigitalOutput()   do { TRISAbits.TRISA6 = 0; } while(0)
#define _CIG_NO_SetPullup()          do { WPUAbits.WPUA6 = 1; } while(0)
#define _CIG_NO_ResetPullup()        do { WPUAbits.WPUA6 = 0; } while(0)
#define _CIG_NO_SetPushPull()        do { ODCONAbits.ODCA6 = 0; } while(0)
#define _CIG_NO_SetOpenDrain()       do { ODCONAbits.ODCA6 = 1; } while(0)
#define _CIG_NO_SetAnalogMode()      do { ANSELAbits.ANSELA6 = 1; } while(0)
#define _CIG_NO_SetDigitalMode()     do { ANSELAbits.ANSELA6 = 0; } while(0)

// get/set _STB aliases
#define _STB_TRIS                 TRISAbits.TRISA7
#define _STB_LAT                  LATAbits.LATA7
#define _STB_PORT                 PORTAbits.RA7
#define _STB_WPU                  WPUAbits.WPUA7
#define _STB_OD                   ODCONAbits.ODCA7
#define _STB_ANS                  ANSELAbits.ANSELA7
#define _STB_SetHigh()            do { LATAbits.LATA7 = 1; } while(0)
#define _STB_SetLow()             do { LATAbits.LATA7 = 0; } while(0)
#define _STB_Toggle()             do { LATAbits.LATA7 = ~LATAbits.LATA7; } while(0)
#define _STB_GetValue()           PORTAbits.RA7
#define _STB_SetDigitalInput()    do { TRISAbits.TRISA7 = 1; } while(0)
#define _STB_SetDigitalOutput()   do { TRISAbits.TRISA7 = 0; } while(0)
#define _STB_SetPullup()          do { WPUAbits.WPUA7 = 1; } while(0)
#define _STB_ResetPullup()        do { WPUAbits.WPUA7 = 0; } while(0)
#define _STB_SetPushPull()        do { ODCONAbits.ODCA7 = 0; } while(0)
#define _STB_SetOpenDrain()       do { ODCONAbits.ODCA7 = 1; } while(0)
#define _STB_SetAnalogMode()      do { ANSELAbits.ANSELA7 = 1; } while(0)
#define _STB_SetDigitalMode()     do { ANSELAbits.ANSELA7 = 0; } while(0)

// get/set RB0 procedures
#define RB0_SetHigh()            do { LATBbits.LATB0 = 1; } while(0)
#define RB0_SetLow()             do { LATBbits.LATB0 = 0; } while(0)
#define RB0_Toggle()             do { LATBbits.LATB0 = ~LATBbits.LATB0; } while(0)
#define RB0_GetValue()              PORTBbits.RB0
#define RB0_SetDigitalInput()    do { TRISBbits.TRISB0 = 1; } while(0)
#define RB0_SetDigitalOutput()   do { TRISBbits.TRISB0 = 0; } while(0)
#define RB0_SetPullup()             do { WPUBbits.WPUB0 = 1; } while(0)
#define RB0_ResetPullup()           do { WPUBbits.WPUB0 = 0; } while(0)
#define RB0_SetAnalogMode()         do { ANSELBbits.ANSELB0 = 1; } while(0)
#define RB0_SetDigitalMode()        do { ANSELBbits.ANSELB0 = 0; } while(0)

// get/set _KY1 aliases
#define _KY1_TRIS                 TRISBbits.TRISB1
#define _KY1_LAT                  LATBbits.LATB1
#define _KY1_PORT                 PORTBbits.RB1
#define _KY1_WPU                  WPUBbits.WPUB1
#define _KY1_OD                   ODCONBbits.ODCB1
#define _KY1_ANS                  ANSELBbits.ANSELB1
#define _KY1_SetHigh()            do { LATBbits.LATB1 = 1; } while(0)
#define _KY1_SetLow()             do { LATBbits.LATB1 = 0; } while(0)
#define _KY1_Toggle()             do { LATBbits.LATB1 = ~LATBbits.LATB1; } while(0)
#define _KY1_GetValue()           PORTBbits.RB1
#define _KY1_SetDigitalInput()    do { TRISBbits.TRISB1 = 1; } while(0)
#define _KY1_SetDigitalOutput()   do { TRISBbits.TRISB1 = 0; } while(0)
#define _KY1_SetPullup()          do { WPUBbits.WPUB1 = 1; } while(0)
#define _KY1_ResetPullup()        do { WPUBbits.WPUB1 = 0; } while(0)
#define _KY1_SetPushPull()        do { ODCONBbits.ODCB1 = 0; } while(0)
#define _KY1_SetOpenDrain()       do { ODCONBbits.ODCB1 = 1; } while(0)
#define _KY1_SetAnalogMode()      do { ANSELBbits.ANSELB1 = 1; } while(0)
#define _KY1_SetDigitalMode()     do { ANSELBbits.ANSELB1 = 0; } while(0)

// get/set _KY2 aliases
#define _KY2_TRIS                 TRISBbits.TRISB2
#define _KY2_LAT                  LATBbits.LATB2
#define _KY2_PORT                 PORTBbits.RB2
#define _KY2_WPU                  WPUBbits.WPUB2
#define _KY2_OD                   ODCONBbits.ODCB2
#define _KY2_ANS                  ANSELBbits.ANSELB2
#define _KY2_SetHigh()            do { LATBbits.LATB2 = 1; } while(0)
#define _KY2_SetLow()             do { LATBbits.LATB2 = 0; } while(0)
#define _KY2_Toggle()             do { LATBbits.LATB2 = ~LATBbits.LATB2; } while(0)
#define _KY2_GetValue()           PORTBbits.RB2
#define _KY2_SetDigitalInput()    do { TRISBbits.TRISB2 = 1; } while(0)
#define _KY2_SetDigitalOutput()   do { TRISBbits.TRISB2 = 0; } while(0)
#define _KY2_SetPullup()          do { WPUBbits.WPUB2 = 1; } while(0)
#define _KY2_ResetPullup()        do { WPUBbits.WPUB2 = 0; } while(0)
#define _KY2_SetPushPull()        do { ODCONBbits.ODCB2 = 0; } while(0)
#define _KY2_SetOpenDrain()       do { ODCONBbits.ODCB2 = 1; } while(0)
#define _KY2_SetAnalogMode()      do { ANSELBbits.ANSELB2 = 1; } while(0)
#define _KY2_SetDigitalMode()     do { ANSELBbits.ANSELB2 = 0; } while(0)

// get/set _KY3 aliases
#define _KY3_TRIS                 TRISBbits.TRISB3
#define _KY3_LAT                  LATBbits.LATB3
#define _KY3_PORT                 PORTBbits.RB3
#define _KY3_WPU                  WPUBbits.WPUB3
#define _KY3_OD                   ODCONBbits.ODCB3
#define _KY3_ANS                  ANSELBbits.ANSELB3
#define _KY3_SetHigh()            do { LATBbits.LATB3 = 1; } while(0)
#define _KY3_SetLow()             do { LATBbits.LATB3 = 0; } while(0)
#define _KY3_Toggle()             do { LATBbits.LATB3 = ~LATBbits.LATB3; } while(0)
#define _KY3_GetValue()           PORTBbits.RB3
#define _KY3_SetDigitalInput()    do { TRISBbits.TRISB3 = 1; } while(0)
#define _KY3_SetDigitalOutput()   do { TRISBbits.TRISB3 = 0; } while(0)
#define _KY3_SetPullup()          do { WPUBbits.WPUB3 = 1; } while(0)
#define _KY3_ResetPullup()        do { WPUBbits.WPUB3 = 0; } while(0)
#define _KY3_SetPushPull()        do { ODCONBbits.ODCB3 = 0; } while(0)
#define _KY3_SetOpenDrain()       do { ODCONBbits.ODCB3 = 1; } while(0)
#define _KY3_SetAnalogMode()      do { ANSELBbits.ANSELB3 = 1; } while(0)
#define _KY3_SetDigitalMode()     do { ANSELBbits.ANSELB3 = 0; } while(0)

// get/set _KY4 aliases
#define _KY4_TRIS                 TRISBbits.TRISB4
#define _KY4_LAT                  LATBbits.LATB4
#define _KY4_PORT                 PORTBbits.RB4
#define _KY4_WPU                  WPUBbits.WPUB4
#define _KY4_OD                   ODCONBbits.ODCB4
#define _KY4_ANS                  ANSELBbits.ANSELB4
#define _KY4_SetHigh()            do { LATBbits.LATB4 = 1; } while(0)
#define _KY4_SetLow()             do { LATBbits.LATB4 = 0; } while(0)
#define _KY4_Toggle()             do { LATBbits.LATB4 = ~LATBbits.LATB4; } while(0)
#define _KY4_GetValue()           PORTBbits.RB4
#define _KY4_SetDigitalInput()    do { TRISBbits.TRISB4 = 1; } while(0)
#define _KY4_SetDigitalOutput()   do { TRISBbits.TRISB4 = 0; } while(0)
#define _KY4_SetPullup()          do { WPUBbits.WPUB4 = 1; } while(0)
#define _KY4_ResetPullup()        do { WPUBbits.WPUB4 = 0; } while(0)
#define _KY4_SetPushPull()        do { ODCONBbits.ODCB4 = 0; } while(0)
#define _KY4_SetOpenDrain()       do { ODCONBbits.ODCB4 = 1; } while(0)
#define _KY4_SetAnalogMode()      do { ANSELBbits.ANSELB4 = 1; } while(0)
#define _KY4_SetDigitalMode()     do { ANSELBbits.ANSELB4 = 0; } while(0)

// get/set _PRZ_CO aliases
#define _PRZ_CO_TRIS                 TRISBbits.TRISB5
#define _PRZ_CO_LAT                  LATBbits.LATB5
#define _PRZ_CO_PORT                 PORTBbits.RB5
#define _PRZ_CO_WPU                  WPUBbits.WPUB5
#define _PRZ_CO_OD                   ODCONBbits.ODCB5
#define _PRZ_CO_ANS                  ANSELBbits.ANSELB5
#define _PRZ_CO_SetHigh()            do { LATBbits.LATB5 = 1; } while(0)
#define _PRZ_CO_SetLow()             do { LATBbits.LATB5 = 0; } while(0)
#define _PRZ_CO_Toggle()             do { LATBbits.LATB5 = ~LATBbits.LATB5; } while(0)
#define _PRZ_CO_GetValue()           PORTBbits.RB5
#define _PRZ_CO_SetDigitalInput()    do { TRISBbits.TRISB5 = 1; } while(0)
#define _PRZ_CO_SetDigitalOutput()   do { TRISBbits.TRISB5 = 0; } while(0)
#define _PRZ_CO_SetPullup()          do { WPUBbits.WPUB5 = 1; } while(0)
#define _PRZ_CO_ResetPullup()        do { WPUBbits.WPUB5 = 0; } while(0)
#define _PRZ_CO_SetPushPull()        do { ODCONBbits.ODCB5 = 0; } while(0)
#define _PRZ_CO_SetOpenDrain()       do { ODCONBbits.ODCB5 = 1; } while(0)
#define _PRZ_CO_SetAnalogMode()      do { ANSELBbits.ANSELB5 = 1; } while(0)
#define _PRZ_CO_SetDigitalMode()     do { ANSELBbits.ANSELB5 = 0; } while(0)

// get/set _KNF aliases
#define _KNF_TRIS                 TRISBbits.TRISB6
#define _KNF_LAT                  LATBbits.LATB6
#define _KNF_PORT                 PORTBbits.RB6
#define _KNF_WPU                  WPUBbits.WPUB6
#define _KNF_OD                   ODCONBbits.ODCB6
#define _KNF_ANS                  ANSELBbits.ANSELB6
#define _KNF_SetHigh()            do { LATBbits.LATB6 = 1; } while(0)
#define _KNF_SetLow()             do { LATBbits.LATB6 = 0; } while(0)
#define _KNF_Toggle()             do { LATBbits.LATB6 = ~LATBbits.LATB6; } while(0)
#define _KNF_GetValue()           PORTBbits.RB6
#define _KNF_SetDigitalInput()    do { TRISBbits.TRISB6 = 1; } while(0)
#define _KNF_SetDigitalOutput()   do { TRISBbits.TRISB6 = 0; } while(0)
#define _KNF_SetPullup()          do { WPUBbits.WPUB6 = 1; } while(0)
#define _KNF_ResetPullup()        do { WPUBbits.WPUB6 = 0; } while(0)
#define _KNF_SetPushPull()        do { ODCONBbits.ODCB6 = 0; } while(0)
#define _KNF_SetOpenDrain()       do { ODCONBbits.ODCB6 = 1; } while(0)
#define _KNF_SetAnalogMode()      do { ANSELBbits.ANSELB6 = 1; } while(0)
#define _KNF_SetDigitalMode()     do { ANSELBbits.ANSELB6 = 0; } while(0)

// get/set IO_RB7 aliases
#define IO_RB7_TRIS                 TRISBbits.TRISB7
#define IO_RB7_LAT                  LATBbits.LATB7
#define IO_RB7_PORT                 PORTBbits.RB7
#define IO_RB7_WPU                  WPUBbits.WPUB7
#define IO_RB7_OD                   ODCONBbits.ODCB7
#define IO_RB7_ANS                  ANSELBbits.ANSELB7
#define IO_RB7_SetHigh()            do { LATBbits.LATB7 = 1; } while(0)
#define IO_RB7_SetLow()             do { LATBbits.LATB7 = 0; } while(0)
#define IO_RB7_Toggle()             do { LATBbits.LATB7 = ~LATBbits.LATB7; } while(0)
#define IO_RB7_GetValue()           PORTBbits.RB7
#define IO_RB7_SetDigitalInput()    do { TRISBbits.TRISB7 = 1; } while(0)
#define IO_RB7_SetDigitalOutput()   do { TRISBbits.TRISB7 = 0; } while(0)
#define IO_RB7_SetPullup()          do { WPUBbits.WPUB7 = 1; } while(0)
#define IO_RB7_ResetPullup()        do { WPUBbits.WPUB7 = 0; } while(0)
#define IO_RB7_SetPushPull()        do { ODCONBbits.ODCB7 = 0; } while(0)
#define IO_RB7_SetOpenDrain()       do { ODCONBbits.ODCB7 = 1; } while(0)
#define IO_RB7_SetAnalogMode()      do { ANSELBbits.ANSELB7 = 1; } while(0)
#define IO_RB7_SetDigitalMode()     do { ANSELBbits.ANSELB7 = 0; } while(0)

// get/set _CIG_NC aliases
#define _CIG_NC_TRIS                 TRISCbits.TRISC0
#define _CIG_NC_LAT                  LATCbits.LATC0
#define _CIG_NC_PORT                 PORTCbits.RC0
#define _CIG_NC_WPU                  WPUCbits.WPUC0
#define _CIG_NC_OD                   ODCONCbits.ODCC0
#define _CIG_NC_ANS                  ANSELCbits.ANSELC0
#define _CIG_NC_SetHigh()            do { LATCbits.LATC0 = 1; } while(0)
#define _CIG_NC_SetLow()             do { LATCbits.LATC0 = 0; } while(0)
#define _CIG_NC_Toggle()             do { LATCbits.LATC0 = ~LATCbits.LATC0; } while(0)
#define _CIG_NC_GetValue()           PORTCbits.RC0
#define _CIG_NC_SetDigitalInput()    do { TRISCbits.TRISC0 = 1; } while(0)
#define _CIG_NC_SetDigitalOutput()   do { TRISCbits.TRISC0 = 0; } while(0)
#define _CIG_NC_SetPullup()          do { WPUCbits.WPUC0 = 1; } while(0)
#define _CIG_NC_ResetPullup()        do { WPUCbits.WPUC0 = 0; } while(0)
#define _CIG_NC_SetPushPull()        do { ODCONCbits.ODCC0 = 0; } while(0)
#define _CIG_NC_SetOpenDrain()       do { ODCONCbits.ODCC0 = 1; } while(0)
#define _CIG_NC_SetAnalogMode()      do { ANSELCbits.ANSELC0 = 1; } while(0)
#define _CIG_NC_SetDigitalMode()     do { ANSELCbits.ANSELC0 = 0; } while(0)

// get/set _PWM_PMP aliases
#define _PWM_PMP_TRIS                 TRISCbits.TRISC1
#define _PWM_PMP_LAT                  LATCbits.LATC1
#define _PWM_PMP_PORT                 PORTCbits.RC1
#define _PWM_PMP_WPU                  WPUCbits.WPUC1
#define _PWM_PMP_OD                   ODCONCbits.ODCC1
#define _PWM_PMP_ANS                  ANSELCbits.ANSELC1
#define _PWM_PMP_SetHigh()            do { LATCbits.LATC1 = 1; } while(0)
#define _PWM_PMP_SetLow()             do { LATCbits.LATC1 = 0; } while(0)
#define _PWM_PMP_Toggle()             do { LATCbits.LATC1 = ~LATCbits.LATC1; } while(0)
#define _PWM_PMP_GetValue()           PORTCbits.RC1
#define _PWM_PMP_SetDigitalInput()    do { TRISCbits.TRISC1 = 1; } while(0)
#define _PWM_PMP_SetDigitalOutput()   do { TRISCbits.TRISC1 = 0; } while(0)
#define _PWM_PMP_SetPullup()          do { WPUCbits.WPUC1 = 1; } while(0)
#define _PWM_PMP_ResetPullup()        do { WPUCbits.WPUC1 = 0; } while(0)
#define _PWM_PMP_SetPushPull()        do { ODCONCbits.ODCC1 = 0; } while(0)
#define _PWM_PMP_SetOpenDrain()       do { ODCONCbits.ODCC1 = 1; } while(0)
#define _PWM_PMP_SetAnalogMode()      do { ANSELCbits.ANSELC1 = 1; } while(0)
#define _PWM_PMP_SetDigitalMode()     do { ANSELCbits.ANSELC1 = 0; } while(0)

// get/set _PWM_MOD aliases
#define _PWM_MOD_TRIS                 TRISCbits.TRISC2
#define _PWM_MOD_LAT                  LATCbits.LATC2
#define _PWM_MOD_PORT                 PORTCbits.RC2
#define _PWM_MOD_WPU                  WPUCbits.WPUC2
#define _PWM_MOD_OD                   ODCONCbits.ODCC2
#define _PWM_MOD_ANS                  ANSELCbits.ANSELC2
#define _PWM_MOD_SetHigh()            do { LATCbits.LATC2 = 1; } while(0)
#define _PWM_MOD_SetLow()             do { LATCbits.LATC2 = 0; } while(0)
#define _PWM_MOD_Toggle()             do { LATCbits.LATC2 = ~LATCbits.LATC2; } while(0)
#define _PWM_MOD_GetValue()           PORTCbits.RC2
#define _PWM_MOD_SetDigitalInput()    do { TRISCbits.TRISC2 = 1; } while(0)
#define _PWM_MOD_SetDigitalOutput()   do { TRISCbits.TRISC2 = 0; } while(0)
#define _PWM_MOD_SetPullup()          do { WPUCbits.WPUC2 = 1; } while(0)
#define _PWM_MOD_ResetPullup()        do { WPUCbits.WPUC2 = 0; } while(0)
#define _PWM_MOD_SetPushPull()        do { ODCONCbits.ODCC2 = 0; } while(0)
#define _PWM_MOD_SetOpenDrain()       do { ODCONCbits.ODCC2 = 1; } while(0)
#define _PWM_MOD_SetAnalogMode()      do { ANSELCbits.ANSELC2 = 1; } while(0)
#define _PWM_MOD_SetDigitalMode()     do { ANSELCbits.ANSELC2 = 0; } while(0)

// get/set RC3 procedures
#define RC3_SetHigh()            do { LATCbits.LATC3 = 1; } while(0)
#define RC3_SetLow()             do { LATCbits.LATC3 = 0; } while(0)
#define RC3_Toggle()             do { LATCbits.LATC3 = ~LATCbits.LATC3; } while(0)
#define RC3_GetValue()              PORTCbits.RC3
#define RC3_SetDigitalInput()    do { TRISCbits.TRISC3 = 1; } while(0)
#define RC3_SetDigitalOutput()   do { TRISCbits.TRISC3 = 0; } while(0)
#define RC3_SetPullup()             do { WPUCbits.WPUC3 = 1; } while(0)
#define RC3_ResetPullup()           do { WPUCbits.WPUC3 = 0; } while(0)
#define RC3_SetAnalogMode()         do { ANSELCbits.ANSELC3 = 1; } while(0)
#define RC3_SetDigitalMode()        do { ANSELCbits.ANSELC3 = 0; } while(0)

// get/set RC4 procedures
#define RC4_SetHigh()            do { LATCbits.LATC4 = 1; } while(0)
#define RC4_SetLow()             do { LATCbits.LATC4 = 0; } while(0)
#define RC4_Toggle()             do { LATCbits.LATC4 = ~LATCbits.LATC4; } while(0)
#define RC4_GetValue()              PORTCbits.RC4
#define RC4_SetDigitalInput()    do { TRISCbits.TRISC4 = 1; } while(0)
#define RC4_SetDigitalOutput()   do { TRISCbits.TRISC4 = 0; } while(0)
#define RC4_SetPullup()             do { WPUCbits.WPUC4 = 1; } while(0)
#define RC4_ResetPullup()           do { WPUCbits.WPUC4 = 0; } while(0)
#define RC4_SetAnalogMode()         do { ANSELCbits.ANSELC4 = 1; } while(0)
#define RC4_SetDigitalMode()        do { ANSELCbits.ANSELC4 = 0; } while(0)

// get/set _MOD_ON aliases
#define _MOD_ON_TRIS                 TRISCbits.TRISC5
#define _MOD_ON_LAT                  LATCbits.LATC5
#define _MOD_ON_PORT                 PORTCbits.RC5
#define _MOD_ON_WPU                  WPUCbits.WPUC5
#define _MOD_ON_OD                   ODCONCbits.ODCC5
#define _MOD_ON_ANS                  ANSELCbits.ANSELC5
#define _MOD_ON_SetHigh()            do { LATCbits.LATC5 = 1; } while(0)
#define _MOD_ON_SetLow()             do { LATCbits.LATC5 = 0; } while(0)
#define _MOD_ON_Toggle()             do { LATCbits.LATC5 = ~LATCbits.LATC5; } while(0)
#define _MOD_ON_GetValue()           PORTCbits.RC5
#define _MOD_ON_SetDigitalInput()    do { TRISCbits.TRISC5 = 1; } while(0)
#define _MOD_ON_SetDigitalOutput()   do { TRISCbits.TRISC5 = 0; } while(0)
#define _MOD_ON_SetPullup()          do { WPUCbits.WPUC5 = 1; } while(0)
#define _MOD_ON_ResetPullup()        do { WPUCbits.WPUC5 = 0; } while(0)
#define _MOD_ON_SetPushPull()        do { ODCONCbits.ODCC5 = 0; } while(0)
#define _MOD_ON_SetOpenDrain()       do { ODCONCbits.ODCC5 = 1; } while(0)
#define _MOD_ON_SetAnalogMode()      do { ANSELCbits.ANSELC5 = 1; } while(0)
#define _MOD_ON_SetDigitalMode()     do { ANSELCbits.ANSELC5 = 0; } while(0)

// get/set RC6 procedures
#define RC6_SetHigh()            do { LATCbits.LATC6 = 1; } while(0)
#define RC6_SetLow()             do { LATCbits.LATC6 = 0; } while(0)
#define RC6_Toggle()             do { LATCbits.LATC6 = ~LATCbits.LATC6; } while(0)
#define RC6_GetValue()              PORTCbits.RC6
#define RC6_SetDigitalInput()    do { TRISCbits.TRISC6 = 1; } while(0)
#define RC6_SetDigitalOutput()   do { TRISCbits.TRISC6 = 0; } while(0)
#define RC6_SetPullup()             do { WPUCbits.WPUC6 = 1; } while(0)
#define RC6_ResetPullup()           do { WPUCbits.WPUC6 = 0; } while(0)
#define RC6_SetAnalogMode()         do { ANSELCbits.ANSELC6 = 1; } while(0)
#define RC6_SetDigitalMode()        do { ANSELCbits.ANSELC6 = 0; } while(0)

// get/set RC7 procedures
#define RC7_SetHigh()            do { LATCbits.LATC7 = 1; } while(0)
#define RC7_SetLow()             do { LATCbits.LATC7 = 0; } while(0)
#define RC7_Toggle()             do { LATCbits.LATC7 = ~LATCbits.LATC7; } while(0)
#define RC7_GetValue()              PORTCbits.RC7
#define RC7_SetDigitalInput()    do { TRISCbits.TRISC7 = 1; } while(0)
#define RC7_SetDigitalOutput()   do { TRISCbits.TRISC7 = 0; } while(0)
#define RC7_SetPullup()             do { WPUCbits.WPUC7 = 1; } while(0)
#define RC7_ResetPullup()           do { WPUCbits.WPUC7 = 0; } while(0)
#define RC7_SetAnalogMode()         do { ANSELCbits.ANSELC7 = 1; } while(0)
#define RC7_SetDigitalMode()        do { ANSELCbits.ANSELC7 = 0; } while(0)

// get/set _GRZEJ aliases
#define _GRZEJ_TRIS                 TRISDbits.TRISD0
#define _GRZEJ_LAT                  LATDbits.LATD0
#define _GRZEJ_PORT                 PORTDbits.RD0
#define _GRZEJ_WPU                  WPUDbits.WPUD0
#define _GRZEJ_OD                   ODCONDbits.ODCD0
#define _GRZEJ_ANS                  ANSELDbits.ANSELD0
#define _GRZEJ_SetHigh()            do { LATDbits.LATD0 = 1; } while(0)
#define _GRZEJ_SetLow()             do { LATDbits.LATD0 = 0; } while(0)
#define _GRZEJ_Toggle()             do { LATDbits.LATD0 = ~LATDbits.LATD0; } while(0)
#define _GRZEJ_GetValue()           PORTDbits.RD0
#define _GRZEJ_SetDigitalInput()    do { TRISDbits.TRISD0 = 1; } while(0)
#define _GRZEJ_SetDigitalOutput()   do { TRISDbits.TRISD0 = 0; } while(0)
#define _GRZEJ_SetPullup()          do { WPUDbits.WPUD0 = 1; } while(0)
#define _GRZEJ_ResetPullup()        do { WPUDbits.WPUD0 = 0; } while(0)
#define _GRZEJ_SetPushPull()        do { ODCONDbits.ODCD0 = 0; } while(0)
#define _GRZEJ_SetOpenDrain()       do { ODCONDbits.ODCD0 = 1; } while(0)
#define _GRZEJ_SetAnalogMode()      do { ANSELDbits.ANSELD0 = 1; } while(0)
#define _GRZEJ_SetDigitalMode()     do { ANSELDbits.ANSELD0 = 0; } while(0)

// get/set _KCO_CW aliases
#define _KCO_CW_TRIS                 TRISDbits.TRISD1
#define _KCO_CW_LAT                  LATDbits.LATD1
#define _KCO_CW_PORT                 PORTDbits.RD1
#define _KCO_CW_WPU                  WPUDbits.WPUD1
#define _KCO_CW_OD                   ODCONDbits.ODCD1
#define _KCO_CW_ANS                  ANSELDbits.ANSELD1
#define _KCO_CW_SetHigh()            do { LATDbits.LATD1 = 1; } while(0)
#define _KCO_CW_SetLow()             do { LATDbits.LATD1 = 0; } while(0)
#define _KCO_CW_Toggle()             do { LATDbits.LATD1 = ~LATDbits.LATD1; } while(0)
#define _KCO_CW_GetValue()           PORTDbits.RD1
#define _KCO_CW_SetDigitalInput()    do { TRISDbits.TRISD1 = 1; } while(0)
#define _KCO_CW_SetDigitalOutput()   do { TRISDbits.TRISD1 = 0; } while(0)
#define _KCO_CW_SetPullup()          do { WPUDbits.WPUD1 = 1; } while(0)
#define _KCO_CW_ResetPullup()        do { WPUDbits.WPUD1 = 0; } while(0)
#define _KCO_CW_SetPushPull()        do { ODCONDbits.ODCD1 = 0; } while(0)
#define _KCO_CW_SetOpenDrain()       do { ODCONDbits.ODCD1 = 1; } while(0)
#define _KCO_CW_SetAnalogMode()      do { ANSELDbits.ANSELD1 = 1; } while(0)
#define _KCO_CW_SetDigitalMode()     do { ANSELDbits.ANSELD1 = 0; } while(0)

// get/set _POMPA aliases
#define _POMPA_TRIS                 TRISDbits.TRISD2
#define _POMPA_LAT                  LATDbits.LATD2
#define _POMPA_PORT                 PORTDbits.RD2
#define _POMPA_WPU                  WPUDbits.WPUD2
#define _POMPA_OD                   ODCONDbits.ODCD2
#define _POMPA_ANS                  ANSELDbits.ANSELD2
#define _POMPA_SetHigh()            do { LATDbits.LATD2 = 1; } while(0)
#define _POMPA_SetLow()             do { LATDbits.LATD2 = 0; } while(0)
#define _POMPA_Toggle()             do { LATDbits.LATD2 = ~LATDbits.LATD2; } while(0)
#define _POMPA_GetValue()           PORTDbits.RD2
#define _POMPA_SetDigitalInput()    do { TRISDbits.TRISD2 = 1; } while(0)
#define _POMPA_SetDigitalOutput()   do { TRISDbits.TRISD2 = 0; } while(0)
#define _POMPA_SetPullup()          do { WPUDbits.WPUD2 = 1; } while(0)
#define _POMPA_ResetPullup()        do { WPUDbits.WPUD2 = 0; } while(0)
#define _POMPA_SetPushPull()        do { ODCONDbits.ODCD2 = 0; } while(0)
#define _POMPA_SetOpenDrain()       do { ODCONDbits.ODCD2 = 1; } while(0)
#define _POMPA_SetAnalogMode()      do { ANSELDbits.ANSELD2 = 1; } while(0)
#define _POMPA_SetDigitalMode()     do { ANSELDbits.ANSELD2 = 0; } while(0)

// get/set _VNT aliases
#define _VNT_TRIS                 TRISDbits.TRISD3
#define _VNT_LAT                  LATDbits.LATD3
#define _VNT_PORT                 PORTDbits.RD3
#define _VNT_WPU                  WPUDbits.WPUD3
#define _VNT_OD                   ODCONDbits.ODCD3
#define _VNT_ANS                  ANSELDbits.ANSELD3
#define _VNT_SetHigh()            do { LATDbits.LATD3 = 1; } while(0)
#define _VNT_SetLow()             do { LATDbits.LATD3 = 0; } while(0)
#define _VNT_Toggle()             do { LATDbits.LATD3 = ~LATDbits.LATD3; } while(0)
#define _VNT_GetValue()           PORTDbits.RD3
#define _VNT_SetDigitalInput()    do { TRISDbits.TRISD3 = 1; } while(0)
#define _VNT_SetDigitalOutput()   do { TRISDbits.TRISD3 = 0; } while(0)
#define _VNT_SetPullup()          do { WPUDbits.WPUD3 = 1; } while(0)
#define _VNT_ResetPullup()        do { WPUDbits.WPUD3 = 0; } while(0)
#define _VNT_SetPushPull()        do { ODCONDbits.ODCD3 = 0; } while(0)
#define _VNT_SetOpenDrain()       do { ODCONDbits.ODCD3 = 1; } while(0)
#define _VNT_SetAnalogMode()      do { ANSELDbits.ANSELD3 = 1; } while(0)
#define _VNT_SetDigitalMode()     do { ANSELDbits.ANSELD3 = 0; } while(0)

// get/set _PLOMIEN aliases
#define _PLOMIEN_TRIS                 TRISDbits.TRISD4
#define _PLOMIEN_LAT                  LATDbits.LATD4
#define _PLOMIEN_PORT                 PORTDbits.RD4
#define _PLOMIEN_WPU                  WPUDbits.WPUD4
#define _PLOMIEN_OD                   ODCONDbits.ODCD4
#define _PLOMIEN_ANS                  ANSELDbits.ANSELD4
#define _PLOMIEN_SetHigh()            do { LATDbits.LATD4 = 1; } while(0)
#define _PLOMIEN_SetLow()             do { LATDbits.LATD4 = 0; } while(0)
#define _PLOMIEN_Toggle()             do { LATDbits.LATD4 = ~LATDbits.LATD4; } while(0)
#define _PLOMIEN_GetValue()           PORTDbits.RD4
#define _PLOMIEN_SetDigitalInput()    do { TRISDbits.TRISD4 = 1; } while(0)
#define _PLOMIEN_SetDigitalOutput()   do { TRISDbits.TRISD4 = 0; } while(0)
#define _PLOMIEN_SetPullup()          do { WPUDbits.WPUD4 = 1; } while(0)
#define _PLOMIEN_ResetPullup()        do { WPUDbits.WPUD4 = 0; } while(0)
#define _PLOMIEN_SetPushPull()        do { ODCONDbits.ODCD4 = 0; } while(0)
#define _PLOMIEN_SetOpenDrain()       do { ODCONDbits.ODCD4 = 1; } while(0)
#define _PLOMIEN_SetAnalogMode()      do { ANSELDbits.ANSELD4 = 1; } while(0)
#define _PLOMIEN_SetDigitalMode()     do { ANSELDbits.ANSELD4 = 0; } while(0)

// get/set _RT aliases
#define _RT_TRIS                 TRISDbits.TRISD5
#define _RT_LAT                  LATDbits.LATD5
#define _RT_PORT                 PORTDbits.RD5
#define _RT_WPU                  WPUDbits.WPUD5
#define _RT_OD                   ODCONDbits.ODCD5
#define _RT_ANS                  ANSELDbits.ANSELD5
#define _RT_SetHigh()            do { LATDbits.LATD5 = 1; } while(0)
#define _RT_SetLow()             do { LATDbits.LATD5 = 0; } while(0)
#define _RT_Toggle()             do { LATDbits.LATD5 = ~LATDbits.LATD5; } while(0)
#define _RT_GetValue()           PORTDbits.RD5
#define _RT_SetDigitalInput()    do { TRISDbits.TRISD5 = 1; } while(0)
#define _RT_SetDigitalOutput()   do { TRISDbits.TRISD5 = 0; } while(0)
#define _RT_SetPullup()          do { WPUDbits.WPUD5 = 1; } while(0)
#define _RT_ResetPullup()        do { WPUDbits.WPUD5 = 0; } while(0)
#define _RT_SetPushPull()        do { ODCONDbits.ODCD5 = 0; } while(0)
#define _RT_SetOpenDrain()       do { ODCONDbits.ODCD5 = 1; } while(0)
#define _RT_SetAnalogMode()      do { ANSELDbits.ANSELD5 = 1; } while(0)
#define _RT_SetDigitalMode()     do { ANSELDbits.ANSELD5 = 0; } while(0)

// get/set IO_RD6 aliases
#define IO_RD6_TRIS                 TRISDbits.TRISD6
#define IO_RD6_LAT                  LATDbits.LATD6
#define IO_RD6_PORT                 PORTDbits.RD6
#define IO_RD6_WPU                  WPUDbits.WPUD6
#define IO_RD6_OD                   ODCONDbits.ODCD6
#define IO_RD6_ANS                  ANSELDbits.ANSELD6
#define IO_RD6_SetHigh()            do { LATDbits.LATD6 = 1; } while(0)
#define IO_RD6_SetLow()             do { LATDbits.LATD6 = 0; } while(0)
#define IO_RD6_Toggle()             do { LATDbits.LATD6 = ~LATDbits.LATD6; } while(0)
#define IO_RD6_GetValue()           PORTDbits.RD6
#define IO_RD6_SetDigitalInput()    do { TRISDbits.TRISD6 = 1; } while(0)
#define IO_RD6_SetDigitalOutput()   do { TRISDbits.TRISD6 = 0; } while(0)
#define IO_RD6_SetPullup()          do { WPUDbits.WPUD6 = 1; } while(0)
#define IO_RD6_ResetPullup()        do { WPUDbits.WPUD6 = 0; } while(0)
#define IO_RD6_SetPushPull()        do { ODCONDbits.ODCD6 = 0; } while(0)
#define IO_RD6_SetOpenDrain()       do { ODCONDbits.ODCD6 = 1; } while(0)
#define IO_RD6_SetAnalogMode()      do { ANSELDbits.ANSELD6 = 1; } while(0)
#define IO_RD6_SetDigitalMode()     do { ANSELDbits.ANSELD6 = 0; } while(0)

// get/set _FFPWM aliases
#define _FFPWM_TRIS                 TRISDbits.TRISD7
#define _FFPWM_LAT                  LATDbits.LATD7
#define _FFPWM_PORT                 PORTDbits.RD7
#define _FFPWM_WPU                  WPUDbits.WPUD7
#define _FFPWM_OD                   ODCONDbits.ODCD7
#define _FFPWM_ANS                  ANSELDbits.ANSELD7
#define _FFPWM_SetHigh()            do { LATDbits.LATD7 = 1; } while(0)
#define _FFPWM_SetLow()             do { LATDbits.LATD7 = 0; } while(0)
#define _FFPWM_Toggle()             do { LATDbits.LATD7 = ~LATDbits.LATD7; } while(0)
#define _FFPWM_GetValue()           PORTDbits.RD7
#define _FFPWM_SetDigitalInput()    do { TRISDbits.TRISD7 = 1; } while(0)
#define _FFPWM_SetDigitalOutput()   do { TRISDbits.TRISD7 = 0; } while(0)
#define _FFPWM_SetPullup()          do { WPUDbits.WPUD7 = 1; } while(0)
#define _FFPWM_ResetPullup()        do { WPUDbits.WPUD7 = 0; } while(0)
#define _FFPWM_SetPushPull()        do { ODCONDbits.ODCD7 = 0; } while(0)
#define _FFPWM_SetOpenDrain()       do { ODCONDbits.ODCD7 = 1; } while(0)
#define _FFPWM_SetAnalogMode()      do { ANSELDbits.ANSELD7 = 1; } while(0)
#define _FFPWM_SetDigitalMode()     do { ANSELDbits.ANSELD7 = 0; } while(0)

// get/set _ST aliases
#define _ST_TRIS                 TRISEbits.TRISE0
#define _ST_LAT                  LATEbits.LATE0
#define _ST_PORT                 PORTEbits.RE0
#define _ST_WPU                  WPUEbits.WPUE0
#define _ST_OD                   ODCONEbits.ODCE0
#define _ST_ANS                  ANSELEbits.ANSELE0
#define _ST_SetHigh()            do { LATEbits.LATE0 = 1; } while(0)
#define _ST_SetLow()             do { LATEbits.LATE0 = 0; } while(0)
#define _ST_Toggle()             do { LATEbits.LATE0 = ~LATEbits.LATE0; } while(0)
#define _ST_GetValue()           PORTEbits.RE0
#define _ST_SetDigitalInput()    do { TRISEbits.TRISE0 = 1; } while(0)
#define _ST_SetDigitalOutput()   do { TRISEbits.TRISE0 = 0; } while(0)
#define _ST_SetPullup()          do { WPUEbits.WPUE0 = 1; } while(0)
#define _ST_ResetPullup()        do { WPUEbits.WPUE0 = 0; } while(0)
#define _ST_SetPushPull()        do { ODCONEbits.ODCE0 = 0; } while(0)
#define _ST_SetOpenDrain()       do { ODCONEbits.ODCE0 = 1; } while(0)
#define _ST_SetAnalogMode()      do { ANSELEbits.ANSELE0 = 1; } while(0)
#define _ST_SetDigitalMode()     do { ANSELEbits.ANSELE0 = 0; } while(0)

// get/set _DT aliases
#define _DT_TRIS                 TRISEbits.TRISE1
#define _DT_LAT                  LATEbits.LATE1
#define _DT_PORT                 PORTEbits.RE1
#define _DT_WPU                  WPUEbits.WPUE1
#define _DT_OD                   ODCONEbits.ODCE1
#define _DT_ANS                  ANSELEbits.ANSELE1
#define _DT_SetHigh()            do { LATEbits.LATE1 = 1; } while(0)
#define _DT_SetLow()             do { LATEbits.LATE1 = 0; } while(0)
#define _DT_Toggle()             do { LATEbits.LATE1 = ~LATEbits.LATE1; } while(0)
#define _DT_GetValue()           PORTEbits.RE1
#define _DT_SetDigitalInput()    do { TRISEbits.TRISE1 = 1; } while(0)
#define _DT_SetDigitalOutput()   do { TRISEbits.TRISE1 = 0; } while(0)
#define _DT_SetPullup()          do { WPUEbits.WPUE1 = 1; } while(0)
#define _DT_ResetPullup()        do { WPUEbits.WPUE1 = 0; } while(0)
#define _DT_SetPushPull()        do { ODCONEbits.ODCE1 = 0; } while(0)
#define _DT_SetOpenDrain()       do { ODCONEbits.ODCE1 = 1; } while(0)
#define _DT_SetAnalogMode()      do { ANSELEbits.ANSELE1 = 1; } while(0)
#define _DT_SetDigitalMode()     do { ANSELEbits.ANSELE1 = 0; } while(0)

// get/set _CL aliases
#define _CL_TRIS                 TRISEbits.TRISE2
#define _CL_LAT                  LATEbits.LATE2
#define _CL_PORT                 PORTEbits.RE2
#define _CL_WPU                  WPUEbits.WPUE2
#define _CL_OD                   ODCONEbits.ODCE2
#define _CL_ANS                  ANSELEbits.ANSELE2
#define _CL_SetHigh()            do { LATEbits.LATE2 = 1; } while(0)
#define _CL_SetLow()             do { LATEbits.LATE2 = 0; } while(0)
#define _CL_Toggle()             do { LATEbits.LATE2 = ~LATEbits.LATE2; } while(0)
#define _CL_GetValue()           PORTEbits.RE2
#define _CL_SetDigitalInput()    do { TRISEbits.TRISE2 = 1; } while(0)
#define _CL_SetDigitalOutput()   do { TRISEbits.TRISE2 = 0; } while(0)
#define _CL_SetPullup()          do { WPUEbits.WPUE2 = 1; } while(0)
#define _CL_ResetPullup()        do { WPUEbits.WPUE2 = 0; } while(0)
#define _CL_SetPushPull()        do { ODCONEbits.ODCE2 = 0; } while(0)
#define _CL_SetOpenDrain()       do { ODCONEbits.ODCE2 = 1; } while(0)
#define _CL_SetAnalogMode()      do { ANSELEbits.ANSELE2 = 1; } while(0)
#define _CL_SetDigitalMode()     do { ANSELEbits.ANSELE2 = 0; } while(0)

// get/set IO_RE3 aliases
#define IO_RE3_PORT                 PORTEbits.RE3
#define IO_RE3_WPU                  WPUEbits.WPUE3
#define IO_RE3_GetValue()           PORTEbits.RE3
#define IO_RE3_SetPullup()          do { WPUEbits.WPUE3 = 1; } while(0)
#define IO_RE3_ResetPullup()        do { WPUEbits.WPUE3 = 0; } while(0)

/**
   @Param
    none
   @Returns
    none
   @Description
    GPIO and peripheral I/O initialization
   @Example
    PIN_MANAGER_Initialize();
 */
void PIN_MANAGER_Initialize (void);

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handling routine
 * @Example
    PIN_MANAGER_IOC();
 */
void PIN_MANAGER_IOC(void);


/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handler for the IOCBF0 pin functionality
 * @Example
    IOCBF0_ISR();
 */
void IOCBF0_ISR(void);

/**
  @Summary
    Interrupt Handler Setter for IOCBF0 pin interrupt-on-change functionality

  @Description
    Allows selecting an interrupt handler for IOCBF0 at application runtime
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    InterruptHandler function pointer.

  @Example
    PIN_MANAGER_Initialize();
    IOCBF0_SetInterruptHandler(MyInterruptHandler);

*/
void IOCBF0_SetInterruptHandler(void (* InterruptHandler)(void));

/**
  @Summary
    Dynamic Interrupt Handler for IOCBF0 pin

  @Description
    This is a dynamic interrupt handler to be used together with the IOCBF0_SetInterruptHandler() method.
    This handler is called every time the IOCBF0 ISR is executed and allows any function to be registered at runtime.
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    None.

  @Example
    PIN_MANAGER_Initialize();
    IOCBF0_SetInterruptHandler(IOCBF0_InterruptHandler);

*/
extern void (*IOCBF0_InterruptHandler)(void);

/**
  @Summary
    Default Interrupt Handler for IOCBF0 pin

  @Description
    This is a predefined interrupt handler to be used together with the IOCBF0_SetInterruptHandler() method.
    This handler is called every time the IOCBF0 ISR is executed. 
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    None.

  @Example
    PIN_MANAGER_Initialize();
    IOCBF0_SetInterruptHandler(IOCBF0_DefaultInterruptHandler);

*/
void IOCBF0_DefaultInterruptHandler(void);



#endif // PIN_MANAGER_H
/**
 End of File
*/