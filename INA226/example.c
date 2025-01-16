/* ======================================================
 *
 * Copyright I went insane making this Co., Noah Tanner
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF I went insane making this Co.
 *
 * ======================================================
*/
#include "project.h"
#include "ina226.h"
#include <stdio.h>

int main(void)
{
    CyGlobalIntEnable;                  // Enable global interrupts
    I2C_Start();                        // start the I2C component
    UART_Start();                       // start the UART, really helpful for debugging!
    
    uint8 reg = 0xFF;                   // Device ID register, already defined in ina226.h though 
    uint32 ina_addr = 0x40;             // 7 bit addr, YOU MAY NEED TO CHANGE THIS DEPENDING ON HOW YOUR INA IS WIRED.
    
    uint16 deviceID;                    // initialize the returned device ID
    uint16 currentVal;                  // initialize the returned current value
    
    char buffer[64];                    // buffer for print strings over UART
    UART_UartPutString("Start");
    UART_UartPutString("\r\n");
    
    // write to calibration register ( IMPLEMENT CALIBRTION FN)
    writeReg16(ina_addr, CAL_REG, 0x56CE);          // write the calibration register value
    uint16 cal_val = readReg16(ina_addr, CAL_REG);
    sprintf(buffer, "Calibration Value to Reg 0x%X: 0x%04X\r\n", CAL_REG, cal_val);
    UART_UartPutString(buffer);
    
    
    for(;;)
    {   
        
        deviceID = readReg16(ina_addr, ID_REG);
        sprintf(buffer, "Read value from register 0x%X: 0x%04X\r\n", ID_REG, deviceID);
        UART_UartPutString(buffer);
        
        currentVal = getCurrent(ina_addr);
        sprintf(buffer, "Current from register 0x%X: 0x%04X\r\n", CUR_REG, currentVal);
        UART_UartPutString(buffer);
        CyDelay(5000);
    }
}

/* [] END OF FILE */
