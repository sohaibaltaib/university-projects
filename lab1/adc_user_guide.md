# adc_user_guide.md is a documentation file thay illustrates and explains how to use the configuration register and register map.


1-  The configuration register is a 16-bit register that allows the user to configure the operation of the ADC. 

2-  The register map lists the various registers in the ADC and their addresses.

3-  To use the configuration register, you need to write the desired configuration value to the register.

4-  The configuration register can be accessed using the Avalon Memory Mapped interface or the JTAG interface.

5-  If using the Avalon Memory Mapped interface, you need to map the register to an address in the address space.

6-  To do this, add an Avalon MM address span to the project and assign it an appropriate base address.

7- In the hps_adc.vhd file, instantiate an Avalon MM slave interface and connect it to the configuration register.

8- In the software, write the configuration value to the mapped address to configure the ADC.

9- If using the JTAG interface, you need to use Platform Designer to access the configuration register.

10- In Platform Designer, select the JTAG interface and connect to the DE10-Nano board.

11- Load the hps_adc.vhd file and set the configuration register address to the appropriate value.

12- Write the desired configuration value to the register using System Console.

13- The register map can be used to read and write other registers in the ADC as well.

14- To use the register map, you need to map the registers to addresses in the address space and connect them to an Avalon MM interface or JTAG interface.

15- In the software or System Console, you can read and write the registers using the mapped addresses.

![table](https://user-images.githubusercontent.com/55866933/219979545-af0e158a-44a9-4f7c-a8de-a3d3e4061f56.png)

![table1](https://user-images.githubusercontent.com/55866933/219979743-d1287030-0d38-491a-8a70-615960d43f7a.png)
