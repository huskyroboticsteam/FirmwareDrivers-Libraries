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

#include <stdint.h>
#include <project.h>

// Registers
#define CONFIG_REG  (0x00)  // R/W
#define SHUNT_REG   (0x01)  // R
#define BUS_REG     (0x02)  // R
#define PWR_REG     (0x03)  // R
#define CUR_REG     (0x04)  // R
#define CAL_REG     (0x05)  // R/W
#define ENBL_REG    (0x06)  // R/W
#define ALRT_REG    (0x07)  // R/W
#define MANID_REG   (0xFE)  // R
#define ID_REG      (0xFF)  // R

// values
#define RESET       0b1000000000000000
#define TIMEOUT     20


// functions
uint8 whoAmI(uint8 slaveAddr);
uint8 writeReg16(uint8 slaveAddr, uint8 reg, uint16 val);
uint16 readReg16(uint8 slaveAddr, uint8 regAddr);
uint8 reset(uint8 slaveAddr);
uint16 getCurrent(uint8 slaveAddr);
uint8 getBusVoltage(uint8 slaveAddr);
uint8 setCalibration(uint8 slaveAddr, uint8 rShunt, uint8 currentLSB);
uint8 getShuntVoltage(uint8 slaveAddr);
uint8 setEnable(uint8 slaveAddr, uint8 val);

/* [] END OF FILE */
