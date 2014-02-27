**************************************************
DSP Project 

Version 1.6

Created: June 23, 2011
Last Modified: September 1, 2011

Author: Troy Bevis

Energy Conversion and Integration Group, 2011.
FSU - Center for Advanced Power Systems
**************************************************

This document is a README for the DSP Controller project.  This project is intended
for all ECI DSP work that uses the DSP interface board.  The code composer project
is pre-built and ready for use on the DSP board.  Simply import the archive into 
code composer, insert the custom controller code into main.c, and download to 
the board. For further assistance, see the DSP User Guide, and the DSP Developer's Guide.


****Version History & Change Log****
------------------------------------
9/1/2011 -- v1.6 -- Troy Bevis
Added sine wave lookup table in header file lut_def.h.  It is has 168 samples and represents
a 60 Hz sine wave when sampled at 10,080 samples / second.

Changed frequency of timer and pwm switching.  The frequency is now 10,080 Hz.

8/24/2011 -- v1.5 -- Troy Bevis 

Found bug in SetAOx(): If negative number passed, it wasn't corrected and a large number
was passed to the I2C registers (>255).  Corrected by moving check to after conversion.
8/23/2011 -- Troy Bevis

Changed the linker command file to reflect programming to FLASH.  Added the file
'DSP2833xxx_SectionCopy_nonBios.asm' file, which copies program & data from flash to
RAM.  Also changed 'DSP2833x_CodeStartBranch.asm' to jump to SectionCopy instead of 
_c_int00.  

Changed the F28335.cmd linker command file name to F28335_ECI.cmd to reflect 
customization.

Added DEBUG_MODE flag in ECI_API.h for future use.

8/16/2011 -- Troy Bevis

Changed EnablePWM_A/B to EnablePWM_I/R to reflect board usage.
Finished Digital-to-Analog Converter I2C interface coding.  
Added functions:
	-SetAOx, sets AO1/2/3/4 to [0-3]V.  
	-DisablePWM_I and DisablePWM_R, disables PWM.	
	
7/29/2011 -- Troy Bevis

Added preprocessor control over the use of PWM or Digital Output on IGBT Connections.
	-If MODULATION = 1, the PWM is used and has the SetPWM_x() functions.
	-If MODULATION = 0, the IGBT R/Ixx connections are digital outputs and controlled with set/clear/toggle/Pset(),
		like the DI and DO functions.
		
	
7/28/2011 -- Troy Bevis

Added CPU timer initialization and interface.  Only using timer 0 for now.
	-Added StartTimer() & StopTimer().
	-Added initial version of timer_isr(), the timer interrupt service routine.
	

7/27/2011 -- Troy Bevis

Added enables for PWM, and PWM settings & wrapper functions:
	-Changed DSP_init(), added settings for PWM.
	-Added SetPWM_xxx(), to changed Duty Cycle for PWM_xxx, where xxx is Rau/Rad, etc...
	-Added EnablePWM_A() and EnablePWM_B(), active low PWM enables.

Added ADC initialization and ADC wrapper functions:
	-Added StartADC(), which starts a ADC SEQ.
	-Added GetAIN_Vec(), which takes in an array by reference and fills array with ADC values.
	-Added GetAIN_xx(), xx = A1,B2,etc.
	-Added adc_isr(), the ISR for the adc.
	
Note:	
*During testing, tried to use 500-element array of float32 for a buffer inside main.  
This caused the program to malfunction and jump to incorrect memory addresses.  The 
problem was corrected by making the buffer array a global variable.

*Remember that integer division is important for SetPWM_xxx.


7/1/2011 -- Troy Bevis

Added digital input and digital output functions.
	-Digital Input: GetDI_x() & GetDI_Vec()
	-Digital Output: SetDO_x(), ClearDO_x(), ToggleDO_x(), & PSetDO_x(int);


6/23/2011 -- Troy Bevis

Initial project creation.
		Added ECI_API.h.
			-Added DSP_init() function intializes:
				-interrupts
				-gpio
			


		