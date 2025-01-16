## INA226 Driver Class
## Written by Noah Tanner
## Husky Robotics, 2024

### Intro
This driver class enables usage of the INA226 device. It will be very helpful to read through the INA226 datasheet, included here, to understand and get relevant current information from the device. 

### Usage
1. Add the ina226.c and ina226.h files into your PSoC creator 4.4 project. 
2. Add a I2C (SCB mode) [v4.0] block in your TopDesign.cysch
    a. Configure the mode of the block to be Master
3. Add a UART (SCB mode)  block in your TopDesign.cysch
    a. When you run the main program, you can see UART output by using TeraTerm or Putty. You can see what COM device your devboard is connected to in Device Manager, and the Baudrate
        is set in the UART block in your TopDesign.cysch.
4. In your main, write the following line: I2C_Start() to start your I2C component, and UART_Start() to start your UART component. Note: if your TopDesign I2C or UART block block is named something differently than I2C and UART like I have here, you will need to change what is before the _ in that command. Ex. if the I2C block in the TopDesign is called INA226_I2C, it would be INA226_I2C_Start().
5. Under Design Wide Resources, assign the correct pins for I2C SCA and SCL as well as the UART. This should all be based on your hardware setup.
6. You can run the provided example.c file to try and see if you can interact with an INA226. You will need to correctly configure the 7 bit addr of the device if the A0 and A1 pins are connected to anything other than ground. See the datasheet for addressing.
7. Once you are able to connect and read some current values, it probably won't make any sense what you are reading. That is because you need to determing the CURRENT_LSB value and set the configuration register.
    7a. First, determine the expected current you should see across your shunt resistor. Use V=IR here where R is your shunt resistor value and V is the voltage measured at the IN+ side.
    7b. To get the CURRENT_LSB value, which is the maximum current you expect to see across the shunt resistor, take the value you got in 7a and increase it by 25-50%. That is your Imax. Apply this equation now:

        CURRENT_LSB = Imax/2^15
    
    7c. Now, the value to be written to te calibration register can be found with the following equaton.

        CAL = 00.512/CURRENT_LSB * Rshunt

    7d. Convert the CAL value to hex and then write it to the INA using the driver class. I already did this in the example file in line 33 where the cal reg value was written as 0x56CE. 
    7e. Now you can make sense of what you are reading. The CURRENT_LSB value is effectively a resolution for how much each bit will change. In the example file, I was using a CURRENT_LSB vlaue of 2.288 uA/bit. When you read a value with the getCurrent() function, you can do the following to convert it to a value in A.

        Register Value = ReadCurrent/Resolution
    
    Rearranging that, you can get a value for your read current

8. Read through the INA226.C file to see each command to interact with the INA226 device and add them into your main file! 


### Note
If you are having trouble seeing if you are connected to the INA correctly. Download the Bridge Control Panel app and after connecting to your device on there, you can click "list" in the bottom left corner and it will show you all the avaiable I2C connections and their correspodning 8 and 7 bit addresses!  

### Contact
If you encounter errors, let me know and I can fix them. noahetanner@gmail.com. 