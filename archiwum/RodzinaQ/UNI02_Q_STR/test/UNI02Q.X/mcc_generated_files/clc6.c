 /**
   CLC6 Generated Driver File
 
   @Company
     Microchip Technology Inc.
 
   @File Name
     clc6.c
 
   @Summary
     This is the generated driver implementation file for the CLC6 driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs
 
   @Description
     This source file provides implementations for driver APIs for CLC6.
     Generation Information :
         Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.78.1
         Device            :  PIC18F47Q10
         Driver Version    :  2.11
     The generated drivers are tested against the following:
         Compiler          :  XC8 2.10 and above or later
         MPLAB             :  MPLAB X 5.30
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
 
 /**
   Section: Included Files
 */

#include <xc.h>
#include "clc6.h"

/**
  Section: CLC6 APIs
*/

void CLC6_Initialize(void)
{
    // Set the CLC6 to the options selected in the User Interface

    // LC6G1POL not_inverted; LC6G2POL inverted; LC6G3POL not_inverted; LC6G4POL not_inverted; LC6POL not_inverted; 
    CLC6POL = 0x02;
    // LC6D1S T0_overflow; 
    CLC6SEL0 = 0x11;
    // LC6D2S TMR6_OUT; 
    CLC6SEL1 = 0x17;
    // LC6D3S CLC5_OUT; 
    CLC6SEL2 = 0x25;
    // LC6D4S CLC5_OUT; 
    CLC6SEL3 = 0x25;
    // LC6G1D3N disabled; LC6G1D2N disabled; LC6G1D4N disabled; LC6G1D1T enabled; LC6G1D3T disabled; LC6G1D2T disabled; LC6G1D4T disabled; LC6G1D1N disabled; 
    CLC6GLS0 = 0x02;
    // LC6G2D2N disabled; LC6G2D1N disabled; LC6G2D4N disabled; LC6G2D3N enabled; LC6G2D2T enabled; LC6G2D1T disabled; LC6G2D4T disabled; LC6G2D3T disabled; 
    CLC6GLS1 = 0x18;
    // LC6G3D1N disabled; LC6G3D2N disabled; LC6G3D3N disabled; LC6G3D4N disabled; LC6G3D1T disabled; LC6G3D2T enabled; LC6G3D3T disabled; LC6G3D4T disabled; 
    CLC6GLS2 = 0x08;
    // LC6G4D1N disabled; LC6G4D2N disabled; LC6G4D3N disabled; LC6G4D4N disabled; LC6G4D1T disabled; LC6G4D2T disabled; LC6G4D3T disabled; LC6G4D4T disabled; 
    CLC6GLS3 = 0x00;
    // LC6EN enabled; INTN disabled; INTP disabled; MODE 2-input D flip-flop with R; 
    CLC6CON = 0x85;

}


bool CLC6_OutputStatusGet(void)
{
    return(CLC6CONbits.LC6OUT);
}
/**
 End of File
*/
