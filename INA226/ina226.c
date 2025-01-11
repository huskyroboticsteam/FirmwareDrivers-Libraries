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
#include <project.h>
#include <stdio.h>
#include <ina226.h>
#include <stdint.h>

/*******************************************************************************
* Function Name: whoAmiI
********************************************************************************
*
* Returns the device Die ID Register contents to ensure that I2C communication 
* is setup correctly. Likely the first function you will want to run to make 
* sure your INA226 is correctly setup.
*
* \param slaveAddr
* Right justified 7-bit Slave address (valid range 8 to 120). This is dependent
* on your hardware configuration and how you setup A0 and A1
* 
* \return Read_Buf
* Read_Buf is a 2 element array that contains the MSB (Read_Buf[0]), and the
* LSB (Read_Buf[0]) of the Die ID register. It should return 0x2260, which can 
* also display as 8800 in decimal.
* 
*******************************************************************************/
    uint8 whoAmI(uint8 slaveAddr) {
        uint8 deviceId = readReg16(slaveAddr, ID_REG);
        return deviceId;
    }

/*******************************************************************************
* Function Name: writeReg16
********************************************************************************
*
* writes an input data buffer into a specific register on a specific INA226
*
* \param slaveAddr
* Right justified 7-bit Slave address (valid range 8 to 120). This is dependent
* on your hardware configuration and how you setup A0 and A1
*
* \param reg 
* Register on the INA226 that you want to read. See the INA226 datasheets for more
* details on register values and contents.
*
* \param val
* value that is to be input into the INA226
* 
* \return Read_Buf
* Read_Buf is a 2 element array that contains the MSB (Read_Buf[0]), and the
* LSB (Read_Buf[0]) of the register that was read
* 
*******************************************************************************/
    uint8 writeReg16(uint8 slaveAddr, uint8 reg, uint16 val) {
        
        uint8 write_Buf[3];             // define a 3 item array to be written
        write_Buf[0] = reg;             // register to write data to
        write_Buf[1] = (val >> 8);      // MSB of val
        write_Buf[2] = (val & 0xFF);    // LSB of val
        
        uint8 errStatus = I2C_I2CMasterWriteBuf(slaveAddr, write_Buf, 3,  I2C_I2C_MODE_COMPLETE_XFER);
        while((I2C_I2CMasterStatus() & I2C_I2C_MSTAT_WR_CMPLT) == 0){}
        
        return errStatus;
    }    
    
/*******************************************************************************
* Function Name: readReg16
********************************************************************************
*
* Reads a 16 bit register on the INA226 and returns the contents
*
* \param slaveAddr
* Right justified 7-bit Slave address (valid range 8 to 120). This is dependent
* on your hardware configuration and how you setup A0 and A1
*
*
* \param reg 
* Register on the INA226 that you want to read. See the INA226 datasheets for more
* details on register values and contents.
* 
* \return Read_Buf
* Read_Buf is a 2 element array that contains the MSB (Read_Buf[0]), and the
* LSB (Read_Buf[0]) of the register that was read
* 
*******************************************************************************/
    uint16 readReg16(uint8 slaveAddr, uint8 reg) {
        char buffer[64];
  
        uint8 Write_Buf[1] = {0};
        Write_Buf[0]=reg;
        
        uint8 Read_Buf[2] = {0}; // 2 byte register, 16 bit
        
        I2C_I2CMasterWriteBuf(slaveAddr, (uint8 *)Write_Buf, 1,  I2C_I2C_MODE_NO_STOP);
        while((I2C_I2CMasterStatus() & I2C_I2C_MSTAT_WR_CMPLT) == 0){
        }
        
        I2C_I2CMasterReadBuf(slaveAddr, (uint8 *)Read_Buf, 2, I2C_I2C_MODE_REPEAT_START);
        while((I2C_I2CMasterStatus() & I2C_I2C_MSTAT_RD_CMPLT) == 0) {}
        
        uint16 result = (Read_Buf[0] << 8) | Read_Buf[1];
        
        UART_UartPutString("Read Successful\r\n");
        sprintf(buffer, "Raw Read_Buf: 0x%02X 0x%02X\r\n", Read_Buf[0], Read_Buf[1]);
        UART_UartPutString(buffer);
        sprintf(buffer, "Result in Hex: 0x%04X\r\n", result);
        UART_UartPutString(buffer);
        UART_UartPutString("\r\n'");
        
        return result;
    }
    
/*******************************************************************************
* Function Name: reset
********************************************************************************
*
* Resets the specified INA226
*
* \param slaveAddr
* Right justified 7-bit Slave address (valid range 8 to 120). This is dependent
* on your hardware configuration and how you setup A0 and A1
*
* \return errSatus
* Value of error status from I2C API function. Shouldn't matter unless you are 
* troubleshooting, then this value would be very, very helpful!
* 
*******************************************************************************/
    uint8 reset(uint8 slaveAddr) {
        uint8 errStatus = writeReg16(slaveAddr, CONFIG_REG, RESET);
        return errStatus;
    }

/*******************************************************************************
* Function Name: getCurrent
********************************************************************************
*
* Returns the current register, calculated by multiplaying the decimal value
* in the shunt voltage register with the decimal equivalent value of the
* calibration register
*
* \param slaveAddr
* Right justified 7-bit Slave address (valid range 8 to 120). This is dependent
* on your hardware configuration and how you setup A0 and A1
* 
* \return current
* value of current register. if averaging is enabled, it will display the
* averaged value
*
*******************************************************************************/
    uint16 getCurrent(uint8 slaveAddr) {
        uint16 current = readReg16(slaveAddr, CUR_REG); // unsigned result
        
        return current;
    }
    
/*******************************************************************************
* Function Name: getBusVoltage
********************************************************************************
*
* Returns the most recent bus voltage reading
*
* \param slaveAddr
* Right justified 7-bit Slave address (valid range 8 to 120). This is dependent
* on your hardware configuration and how you setup A0 and A1
* 
* \return busVoltage
* most recent bus voltage. Note that if averaging is enabled, it displays the
* most recent value
*
*******************************************************************************/
    uint8 getBusVoltage(uint8 slaveAddr) {
        uint16 busVoltage = readReg16(slaveAddr, BUS_REG); // unsigned result
        
        return busVoltage; // type case to int16 to allow for negatives
    }
    
/*******************************************************************************
* Function Name: setCalibration
********************************************************************************
*
* Programs the calibration register for the INA226 based on the equation (1) in
* the datasheet. The register that is programmed sets the full scale range and 
* lsb of the current and power measurements and acts as an overall system
* calibration. Page 15 in the datasheet explains the math behind the 
* calibration.
*
* \param rShunt
* shunt resistor value that is in the circuit
*
* \param currentLSB
* a convenient, selected value for the LSB for the current register. 

*
*******************************************************************************/
    uint8 setCalibration(uint8 slaveAddr, uint8 rShunt, uint8 currentLSB) {
        uint8  cal = (0.00512 / (currentLSB * rShunt)); // datasheet eqn 1
        
        uint8 errStatus = writeReg16(slaveAddr, CAL_REG, cal);
        
        return errStatus;
    }

/*******************************************************************************
* Function Name: getShuntVoltage
*
* Returns the current shunt voltage reading, Vshunt. A negative number is 
* represented in two's complement fomrat. ( MSB='1' means negative Vshunt )
*
* \param slaveAddr
* Right justified 7-bit Slave address (valid range 8 to 120). This is dependent
* on your hardware configuration and how you setup A0 and A1
*   
* \return shuntVoltage
* 16 bit value of the current shunt voltage represented in 2's complement form
*
********************************************************************************
*
*******************************************************************************/
    uint8 getShuntVoltage(uint8 slaveAddr) {
        uint16 shuntVoltage = readReg16(slaveAddr, SHUNT_REG); // unsigned result
        
        return (int16)shuntVoltage; // type case to int16 to allow for negatives
    }

/*******************************************************************************
* Function Name: setEnable
********************************************************************************
*
* Programs the mast/enable register if control over the Alert pin is desired. If
* multiple functions are enabled, the highest significant bit position (D15-D11)
* takes priority and responds to the alert limit register
*
* \param slaveAddr
* Right justified 7-bit Slave address (valid range 8 to 120). This is dependent
* on your hardware configuration and how you setup A0 and A1
*
* \param val
* value to choose when an alert is enabled:
*       Bit 15 - Shunt Voltage > Alert Limit Register
*       Bit 14 - Shunt Voltage < Alert Limit Register
*       Bit 13 - Bus Voltage > Alert Limit Register
*       Bit 12 - Bus Voltage < Alert Limit Register
*       Bit 11 - Power > Alert Limit Register
*       Bit 10 - on Conversion Ready Flag (bit 3) high
*       Bit 4 - Alert function flag, see datasheet
*       Bit 3 - Conversion ready flag, see datasheet
*       Bit 2 - Math overflow, sets this bit high
*       Bit 1 - Alert polarity. 1 = inverted (active-high open collecter)),
*               0 = normal (active-low open collector) (default)
*       Bit 0 - Alert Latch. Configures latching. 1 = latch enabled, 
*               0 = latch disabled, transparent. See datasheet once again
*               learn to lie for that datasheet, all 40 pages of it...
*
*******************************************************************************/
    uint8 setEnable(uint8 slaveAddr, uint8 val) {
        
        uint8 errStatus = writeReg16(slaveAddr, ENBL_REG, val);
        
        return errStatus;
    }
/* [] END OF FILE */
