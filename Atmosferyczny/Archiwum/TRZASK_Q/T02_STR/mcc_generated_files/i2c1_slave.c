/**
  I2C1 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    i2c1.c

  @Summary
    This is the generated driver implementation file for the I2C1 driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides implementations for driver APIs for I2C1.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.78.1
        Device            :  PIC18F47Q10
        Driver Version    :  1.0.0
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

#include "i2c1_slave.h"
#include <xc.h>

#define I2C1_SLAVE_ADDRESS      102 
#define I2C1_SLAVE_MASK         127

/**
 Section: Global Variables
 */
typedef enum 
{
    I2C1_ADDR, 
    I2C1_TX, 
    I2C1_RX
} i2c1_state_t;

static void I2C1_Isr(void);
static void I2C1_SlaveDefRdInterruptHandler(void);
static void I2C1_SlaveDefWrInterruptHandler(void);
static void I2C1_SlaveDefAddrInterruptHandler(void);
static void I2C1_SlaveDefWrColInterruptHandler(void);
static void I2C1_SlaveDefBusColInterruptHandler(void);

static void I2C1_SlaveRdCallBack(void);
static void I2C1_SlaveWrCallBack(void);
static void I2C1_SlaveAddrCallBack(void);
static void I2C1_SlaveWrColCallBack(void);
static void I2C1_SlaveBusColCallBack(void);

static inline bool I2C1_SlaveOpen();
static inline void I2C1_SlaveClose();
static inline void I2C1_SlaveSetSlaveAddr(uint8_t slaveAddr);
static inline void I2C1_SlaveSetSlaveMask(uint8_t maskAddr);
static inline void I2C1_SlaveEnableIrq(void);
static inline bool I2C1_SlaveIsAddr(void);
static inline bool I2C1_SlaveIsRead(void);
static inline void I2C1_SlaveClearBuff(void);
static inline void I2C1_SlaveClearIrq(void);
static inline void I2C1_SlaveReleaseClock(void);
static inline bool I2C1_SlaveIsWriteCollision(void);
static inline bool I2C1_SlaveIsTxBufEmpty(void);
static inline bool I2C1_SlaveIsData(void);
static inline void I2C1_SlaveRestart(void);
static inline bool I2C1_SlaveIsRxBufFull(void);
static inline void I2C1_SlaveSendTxData(uint8_t data);
static inline uint8_t I2C1_SlaveGetRxData(void);
static inline uint8_t I2C1_SlaveGetAddr(void);
static inline void I2C1_SlaveSendAck(void);
static inline void I2C1_SlaveSendNack(void);


static volatile i2c1_state_t i2c1State = I2C1_ADDR;
static volatile i2c1_state_t i2c1NextState = I2C1_ADDR;
volatile uint8_t i2c1WrData;
volatile uint8_t i2c1RdData;
volatile uint8_t i2c1SlaveAddr;

void I2C1_Initialize()
{
    SSP1STAT  = 0x80;
    SSP1CON1 |= 0x16;       //0b00010110
    SSP1CON2  = 0x00;
    SSP1CON1bits.SSPEN = 0;
    I2C1_Open();
}
//void I2CSlaveTR(void);
void I2C1_Open() 
{
    I2C1_SlaveOpen();
    I2C1_SlaveSetSlaveAddr(I2C1_SLAVE_ADDRESS << 1);
    I2C1_SlaveSetSlaveMask(I2C1_SLAVE_MASK);
    I2C1_SlaveSetIsrHandler(I2C1_Isr);
    //I2C1_SlaveSetIsrHandler(I2CSlaveTR);        //dzialajaca procedura (tradycyjna)
    I2C1_SlaveSetBusColIntHandler(I2C1_SlaveDefBusColInterruptHandler);
    I2C1_SlaveSetWriteIntHandler(I2C1_SlaveDefWrInterruptHandler);
    I2C1_SlaveSetReadIntHandler(I2C1_SlaveDefRdInterruptHandler);
    I2C1_SlaveSetAddrIntHandler(I2C1_SlaveDefAddrInterruptHandler);
    I2C1_SlaveSetWrColIntHandler(I2C1_SlaveDefWrColInterruptHandler);
    I2C1_SlaveEnableIrq();    
}

void I2C1_Close() 
{
    I2C1_SlaveClose();
}

uint8_t I2C1_Read()
{
   return I2C1_SlaveGetRxData();
}

void I2C1_Write(uint8_t data)
{
    I2C1_SlaveSendTxData(data);
}

void I2C1_Enable()
{
    I2C1_Initialize();
}

void I2C1_SendAck()
{
    I2C1_SlaveSendAck();
}

void I2C1_SendNack()
{
    I2C1_SlaveSendNack();
}

static void I2C1_Isr() 
{ 
    I2C1_SlaveClearIrq();

    // read SSPBUF to clear BF
    i2c1RdData = I2C1_SlaveGetRxData();

    if(I2C1_SlaveIsRead())
    {
        i2c1State = I2C1_TX;
    }
    else
    {
        i2c1State = I2C1_RX;
    }
    
    switch(i2c1State)
    {
        case I2C1_TX:
                        
            if(!I2C1_SlaveIsWriteCollision())
            {
                I2C1_SlaveWrCallBack();
            }
            else
            {
                I2C1_SlaveWrColCallBack();
                I2C1_SlaveRestart();
            }
            i2c1NextState = I2C1_ADDR;
            break;
           
        case I2C1_RX:
            if (I2C1_SlaveIsData()) 
            {
                if(I2C1_SlaveIsRxBufFull())
                { 
                    I2C1_SlaveRdCallBack();
                }
            }
            else
            {
                I2C1_SlaveAddrCallBack();
                i2c1NextState = I2C1_ADDR;
            }
            break;
        default:  
            break;
    }
    i2c1State = i2c1NextState;
    I2C1_SlaveReleaseClock();
}

// Common Event Interrupt Handlers
void I2C1_SlaveSetIsrHandler(interruptHandler handler)
{
    MSSP1_InterruptHandler = handler;
}

// Read Event Interrupt Handlers
void I2C1_SlaveSetReadIntHandler(interruptHandler handler) {
    I2C1_SlaveRdInterruptHandler = handler;
}

static void I2C1_SlaveRdCallBack() {
    // Add your custom callback code here  
    if (I2C1_SlaveRdInterruptHandler) 
    {
        I2C1_SlaveRdInterruptHandler();
    } 
}

static void I2C1_SlaveDefRdInterruptHandler() {
    i2c1RdData = I2C1_SlaveGetRxData();
}

// Write Event Interrupt Handlers
void I2C1_SlaveSetWriteIntHandler(interruptHandler handler) {
    I2C1_SlaveWrInterruptHandler = handler;
}

static void I2C1_SlaveWrCallBack() {
    // Add your custom callback code here
    if (I2C1_SlaveWrInterruptHandler) 
    {
        I2C1_SlaveWrInterruptHandler();
    }  
}

static void I2C1_SlaveDefWrInterruptHandler() {
    I2C1_SlaveSendTxData(i2c1WrData);
}

// ADDRESS Event Interrupt Handlers
void I2C1_SlaveSetAddrIntHandler(interruptHandler handler){
    I2C1_SlaveAddrInterruptHandler = handler;
}

static void I2C1_SlaveAddrCallBack() {
    // Add your custom callback code here
    if (I2C1_SlaveAddrInterruptHandler) {
        I2C1_SlaveAddrInterruptHandler();
    }
}

static void I2C1_SlaveDefAddrInterruptHandler() {
    i2c1SlaveAddr = I2C1_SlaveGetAddr();
}

// Write Collision Event Interrupt Handlers
void I2C1_SlaveSetWrColIntHandler(interruptHandler handler){
    I2C1_SlaveWrColInterruptHandler = handler;
}

static void  I2C1_SlaveWrColCallBack() {
    // Add your custom callback code here
    if ( I2C1_SlaveWrColInterruptHandler) 
    {
         I2C1_SlaveWrColInterruptHandler();
    }
}

static void I2C1_SlaveDefWrColInterruptHandler() {
}

// Bus Collision Event Interrupt Handlers
void I2C1_SlaveSetBusColIntHandler(interruptHandler handler){
    I2C1_SlaveBusColInterruptHandler = handler;
}

static void  I2C1_SlaveBusColCallBack() {
    // Add your custom callback code here
    if ( I2C1_SlaveBusColInterruptHandler) 
    {
         I2C1_SlaveBusColInterruptHandler();
    }
}

static void I2C1_SlaveDefBusColInterruptHandler() {
}

static inline bool I2C1_SlaveOpen()
{
    if(!SSP1CON1bits.SSPEN)
    {      
        SSP1STAT  = 0x80;
        SSP1CON1 |= 0x16;
        SSP1CON2  = 0x00;
        SSP1CON1bits.SSPEN = 1;
        return true;
    }
    return false;
}

static inline void I2C1_SlaveClose()
{
    SSP1STAT  = 0x80;
    SSP1CON1 |= 0x16;
    SSP1CON2  = 0x00;
    SSP1CON1bits.SSPEN = 0;
}

static inline void I2C1_SlaveSetSlaveAddr(uint8_t slaveAddr)
{
    SSP1ADD = slaveAddr;
}

static inline void I2C1_SlaveSetSlaveMask(uint8_t maskAddr)
{
    SSP1MSK = maskAddr;
}

static inline void I2C1_SlaveEnableIrq()
{
    PIE3bits.SSP1IE = 1;
}

static inline bool I2C1_SlaveIsAddr()
{
    return !(SSP1STATbits.D_nA);
}

static inline bool I2C1_SlaveIsRead()
{
    return (SSP1STATbits.R_nW);
}

static inline void I2C1_SlaveClearIrq()
{
    PIR3bits.SSP1IF = 0;
}

static inline void I2C1_SlaveReleaseClock()
{
    SSP1CON1bits.CKP = 1;
}

static inline bool I2C1_SlaveIsWriteCollision()
{
    return SSP1CON1bits.WCOL;
}

static inline bool I2C1_SlaveIsData()
{
    return SSP1STATbits.D_nA;
}

static inline void I2C1_SlaveRestart(void)
{
    SSP1CON2bits.RSEN = 1;
}

static inline bool I2C1_SlaveIsTxBufEmpty()
{
    return !SSP1STATbits.BF;
}

static inline bool I2C1_SlaveIsRxBufFull()
{
    return SSP1STATbits.BF;
}

static inline void I2C1_SlaveSendTxData(uint8_t data)
{
    SSP1BUF = data;
}

static inline uint8_t I2C1_SlaveGetRxData()
{
    return SSP1BUF;
}

static inline uint8_t I2C1_SlaveGetAddr()
{
    return SSP1ADD;
}

static inline void I2C1_SlaveSendAck()
{
    SSP1CON2bits.ACKDT = 0;
    SSP1CON2bits.ACKEN = 1;
}

static inline void I2C1_SlaveSendNack()
{
    SSP1CON2bits.ACKDT = 1;
    SSP1CON2bits.ACKEN = 1;
}
