## INA226 Driver Class
## Written by Noah Tanner
## Husky Robotics, 2024

### Intro
This driver class enables usage of the INA226 device. It will be very helpful to read through the INA226 datasheet, included here, to understand and get relevant current information from the device. 

### Usage
1. Add the ina226.c and ina226.h files into your PSoC creator 4.4 project. 
2. Add a I2C (SCB mode) [v4.0] block in your TopDesign.cysch
    a. Configure the mode of the block to be Master
3. In your main, write the following line: I2C_Start() to start your I2C component. Note: if your TopDesign block is named something differently than I2C, you will need to change what is before the _ in that command. Ex. if the block in the TopDesign is called Hello, it would be Hello_Start().
4. Under Design Wide Resources, assign the correct pins for I2C SCA and SCL. This should all be based on your hardware setup.
5. Read through the INA226.C file to see each command to interact with the INA226 device and add them into your main file! 

Note: The INA226 must be calibrated in order to get non-trivial data from it. These files only allow you to communicate with it, but you will need to see the datasheet under section 6.5 Programming to calculate what values to write to the registers so that you can get good data.

### Contact
If you encounter errors, let me know and I can fix them. noahetanner@gmail.com. 