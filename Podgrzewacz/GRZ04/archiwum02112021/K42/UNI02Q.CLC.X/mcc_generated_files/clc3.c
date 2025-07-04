 /**
   CLC3 Generated Driver File
 
   @Company
     Microchip Technology Inc.
 
   @File Name
     clc3.c
 
   @Summary
     This is the generated driver implementation file for the CLC3 driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs
 
   @Description
     This source file provides implementations for driver APIs for CLC3.
     Generation Information :
         Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.4
         Device            :  PIC18F47K42
         Driver Version    :  2.11
     The generated drivers are tested against the following:
         Compiler          :  XC8 2.20 and above or later
         MPLAB             :  MPLAB X 5.40
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
#include "clc3.h"

/**
  Section: CLC3 APIs
*/

void CLC3_Initialize(void)
{
    // Set the CLC3 to the options selected in the User Interface

    // LC3G1POL not_inverted; LC3G2POL not_inverted; LC3G3POL not_inverted; LC3G4POL not_inverted; LC3POL not_inverted; 
    CLC3POL = 0x00;
    // LC3D1S T0_overflow; 
    CLC3SEL0 = 0x0C;
    // LC3D2S T0_overflow; 
    CLC3SEL1 = 0x0C;
    // LC3D3S T0_overflow; 
    CLC3SEL2 = 0x0C;
    // LC3D4S T0_overflow; 
    CLC3SEL3 = 0x0C;
    // LC3G1D3N disabled; LC3G1D2N disabled; LC3G1D4N disabled; LC3G1D1T disabled; LC3G1D3T disabled; LC3G1D2T disabled; LC3G1D4T disabled; LC3G1D1N disabled; 
    CLC3GLS0 = 0x00;
    // LC3G2D2N disabled; LC3G2D1N disabled; LC3G2D4N disabled; LC3G2D3N disabled; LC3G2D2T disabled; LC3G2D1T disabled; LC3G2D4T disabled; LC3G2D3T disabled; 
    CLC3GLS1 = 0x00;
    // LC3G3D1N disabled; LC3G3D2N disabled; LC3G3D3N disabled; LC3G3D4N disabled; LC3G3D1T disabled; LC3G3D2T disabled; LC3G3D3T disabled; LC3G3D4T disabled; 
    CLC3GLS2 = 0x00;
    // LC3G4D1N disabled; LC3G4D2N disabled; LC3G4D3N disabled; LC3G4D4N disabled; LC3G4D1T disabled; LC3G4D2T disabled; LC3G4D3T disabled; LC3G4D4T disabled; 
    CLC3GLS3 = 0x00;
    // LC3EN enabled; INTN disabled; INTP disabled; MODE AND-OR; 
    CLC3CON = 0x80;

}


bool CLC3_OutputStatusGet(void)
{
    return(CLC3CONbits.LC3OUT);
}
/**
 End of File
*/
