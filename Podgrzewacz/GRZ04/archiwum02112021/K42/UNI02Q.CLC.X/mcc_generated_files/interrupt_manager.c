/**
  Generated Interrupt Manager Source File

  @Company:
    Microchip Technology Inc.

  @File Name:
    interrupt_manager.c

  @Summary:
    This is the Interrupt Manager file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description:
    This header file provides implementations for global interrupt handling.
    For individual peripheral handlers please see the peripheral driver for
    all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.4
        Device            :  PIC18F47K42
        Driver Version    :  2.03
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.20 and above or later
        MPLAB 	          :  MPLAB X 5.40
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

#include "interrupt_manager.h"
#include "mcc.h"

void  INTERRUPT_Initialize (void)
{
    // Disable Interrupt Priority Vectors (16CXXX Compatibility Mode)
    INTCON0bits.IPEN = 0;
}

void __interrupt() INTERRUPT_InterruptManager (void)
{
    // interrupt handler
    if(PIE1bits.INT0IE == 1 && PIR1bits.INT0IF == 1)
    {
        INT0_ISR();
    }
    else if(PIE3bits.U1TXIE == 1 && PIR3bits.U1TXIF == 1)
    {
        UART1_TxInterruptHandler();
    }
    else if(PIE3bits.U1RXIE == 1 && PIR3bits.U1RXIF == 1)
    {
        UART1_RxInterruptHandler();
    }
    else if(PIE3bits.I2C1EIE == 1 && PIR3bits.I2C1EIF == 1)
    {
        I2C1_InterruptHandler();
    }
    else if(PIE2bits.I2C1RXIE == 1 && PIR2bits.I2C1RXIF == 1)
    {
        I2C1_InterruptHandler();
    }
    else if(PIE3bits.I2C1IE == 1 && PIR3bits.I2C1IF == 1)
    {
        I2C1_InterruptHandler();
    }
    else if(PIE3bits.I2C1TXIE == 1 && PIR3bits.I2C1TXIF == 1)
    {
        I2C1_InterruptHandler();
    }
    else if(PIE8bits.TMR5IE == 1 && PIR8bits.TMR5IF == 1)
    {
        TMR5_ISR();
    }
    else if(PIE7bits.TMR4IE == 1 && PIR7bits.TMR4IF == 1)
    {
        TMR4_ISR();
    }
    else if(PIE6bits.TMR3IE == 1 && PIR6bits.TMR3IF == 1)
    {
        TMR3_ISR();
    }
    else if(PIE4bits.TMR2IE == 1 && PIR4bits.TMR2IF == 1)
    {
        TMR2_ISR();
    }
    else if(PIE4bits.TMR1IE == 1 && PIR4bits.TMR1IF == 1)
    {
        TMR1_ISR();
    }
    else
    {
        //Unhandled Interrupt
    }
}
/**
 End of File
*/
