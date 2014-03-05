/* DSP Controller Project
 * Energy Conversion and Integration Group
 * Center for Advanced Power Systems
 * Florida State University
 * ******************************************************************************
 * 
 * Filename: ECI_API.c
 * 
 * Last Modified: August 24, 2011
 * 	
 * Author: Troy Bevis
 * 
 * Version: 1.5
 * 
 * ******************************************************************************
 * Purpose:
 * 		This file defines the application program interface for using hardware 
 * 		peripherals on the signal conditioning board with the DSP.  It provides
 * 		the API via wrapper functions.
 *		
 * *****************************************************************************
 * Revisions
 *
 * 8/27/2013 - Jesse Leonard
 * Changed the PWM_PD and TIMER_0_PD from 14881 to 15000 for a 10kHz fsw and 10kHz ISR timer, respectively
 *
 * 9/25/2013 - Jesse Leonard
 * Changed PWM_PD to 7500 for 20kHz fsw, ISR still at 10kHz
 *
 */
 

#ifndef ECI_API_H
#define ECI_API_H 

#include <DSP2833x_Device.h>     	// DSP2833x Headerfile Include File
#include <DSP2833x_Examples.h>  	// DSP2833x Examples Include File
#include <lut_def.h>				// Sin Lookup table

/********************************************************************************************/
//MODULATION, SPI, and DEBUG FLAGS.  MAKE SURE SETTINGS ARE CORRECT.
#define SPI_FLAG 0					//Flag for SPI(SPI_FLAG = 1) vs. DIO(SPI_FLAG = 0) Use.
#define MODULATION 1				//Flag for PWM(MODULATION = 1) or DO(MODULATION = 0) 
									//	on the Rxx & Ixx connections.
#define DEBUG_MODE 1				//Flag to compile and run in debug mode.
									//(DEBUG_MODE = 1 is enabled), controls debug flags. 
/********************************************************************************************/

//Constant Definitions:
#if(MODULATION)
	#define GPAMUX_1 0x55555555  		//Defines register value for GPAMUX1 when PWM is used
	#define GPAMUX_2 0x0000000F	 		//Defines register value for GPAMUX2 when PWM is used.
#else
	#define GPAMUX_1 0x00000000			//Defines register value for GPAMUX1 when Digital Output is used.
	#define GPAMUX_2 0x00000000			//Defines register value for GPAMUX2 when Digital Output is used.
#endif

	#define GPBMUX_1 0x00000005  			//Defines register value for GPBMUX1.

#if(SPI)
	#define GPBMUX_2 0x00055000			//Defines register value for GPBMUX2 when using SPI.
#else
	#define GPBMUX_2 0x00000000  		//Defines register value for GPBMUX2 when not using SPI.
#endif

#define PWM_PD 3750		 		//Defines count variable for switching at 10,080. /9.6 kHz/ w/ 150 MHZ SYSCLK.
#define TIMER_0_PD 15000			//Defines timer 0 period. (14881 = ~10,080 Hz).
#define DEAD_BAND 150				//Defines Dead-band for rising and falling edge (150*1/150Mhz = 1us)
#define DAC_ADDRESS 0x0C			//I2C address of the DAC.
#define DAC1 0x01					//For use with DAC, this is the internal address of DAC1.
#define DAC2 0x02					//DAC2.
#define DAC3 0x04					//DAC3.
#define DAC4 0x08					//DAC4.

//Prototype for timer_isr function, this is needed for initialization.
interrupt void timer_isr();

/***********************************************************/
//API Function Definitions
/***********************************************************/

/*Digital Output Wrapper Functions*/

//Digital Output Channel 0 (GPIO51)
inline void SetDO_0() 		{GpioDataRegs.GPBSET.bit.GPIO51 = 1; 	}
inline void ClearDO_0()		{GpioDataRegs.GPBCLEAR.bit.GPIO51 = 1; 	}
inline void ToggleDO_0()	{GpioDataRegs.GPBTOGGLE.bit.GPIO51 = 1; }
inline void PSetDO_0(int p)	{GpioDataRegs.GPBDAT.bit.GPIO51 = p;	}

//Digital Output Channel 1 (GPIO53)
inline void SetDO_1() 		{GpioDataRegs.GPBSET.bit.GPIO53 = 1; 	}
inline void ClearDO_1()		{GpioDataRegs.GPBCLEAR.bit.GPIO53 = 1; 	}
inline void ToggleDO_1()	{GpioDataRegs.GPBTOGGLE.bit.GPIO53 = 1; }
inline void PSetDO_1(int p) {GpioDataRegs.GPBDAT.bit.GPIO53 = p; 	}

//Digital Output Channel 2 (GPIO59)
inline void SetDO_2() 		{GpioDataRegs.GPBSET.bit.GPIO59 = 1; 	}
inline void ClearDO_2()		{GpioDataRegs.GPBCLEAR.bit.GPIO59 = 1; 	}
inline void ToggleDO_2()	{GpioDataRegs.GPBTOGGLE.bit.GPIO59 = 1; }
inline void PSetDO_2(int p) {GpioDataRegs.GPBDAT.bit.GPIO59 = p; 	}

//Digital Output Channel 3 (GPIO39)
inline void SetDO_3() 		{GpioDataRegs.GPBSET.bit.GPIO39 = 1; 	}
inline void ClearDO_3()		{GpioDataRegs.GPBCLEAR.bit.GPIO39 = 1; 	}
inline void ToggleDO_3()	{GpioDataRegs.GPBTOGGLE.bit.GPIO39 = 1; }
inline void PSetDO_3(int p) {GpioDataRegs.GPBDAT.bit.GPIO39 = p; 	}

//Digital Output Channel 4 (GPIO36)
inline void SetDO_4() 		{GpioDataRegs.GPBSET.bit.GPIO36 = 1; 	}
inline void ClearDO_4()		{GpioDataRegs.GPBCLEAR.bit.GPIO36 = 1; 	}
inline void ToggleDO_4()	{GpioDataRegs.GPBTOGGLE.bit.GPIO36 = 1; }
inline void PSetDO_4(int p) {GpioDataRegs.GPBDAT.bit.GPIO36 = p; 	}

//Digital Output Channel 5 (GPIO63)
inline void SetDO_5() 		{GpioDataRegs.GPBSET.bit.GPIO63 = 1; 	}
inline void ClearDO_5()		{GpioDataRegs.GPBCLEAR.bit.GPIO63 = 1; 	}
inline void ToggleDO_5()	{GpioDataRegs.GPBTOGGLE.bit.GPIO63 = 1; }
inline void PSetDO_5(int p) {GpioDataRegs.GPBDAT.bit.GPIO63 = p; 	}

//Digital Output Channel 6 (GPIO56)
inline void SetDO_6() 		{GpioDataRegs.GPBSET.bit.GPIO56 = 1; 	}
inline void ClearDO_6()		{GpioDataRegs.GPBCLEAR.bit.GPIO56 = 1; 	}
inline void ToggleDO_6()	{GpioDataRegs.GPBTOGGLE.bit.GPIO56 = 1; }
inline void PSetDO_6(int p) {GpioDataRegs.GPBDAT.bit.GPIO56 = p; 	}

//Digital Output Channel 7 (GPIO57)
inline void SetDO_7() 		{GpioDataRegs.GPBSET.bit.GPIO57 = 1; 	}
inline void ClearDO_7()		{GpioDataRegs.GPBCLEAR.bit.GPIO57 = 1; 	}
inline void ToggleDO_7()	{GpioDataRegs.GPBTOGGLE.bit.GPIO57 = 1; }
inline void PSetDO_7(int p) {GpioDataRegs.GPBDAT.bit.GPIO57 = p; 	}

//Digital Output Channel 8 (GPIO58)
inline void SetDO_8() 		{GpioDataRegs.GPBSET.bit.GPIO58 = 1; 	}
inline void ClearDO_8()		{GpioDataRegs.GPBCLEAR.bit.GPIO58 = 1; 	}
inline void ToggleDO_8()	{GpioDataRegs.GPBTOGGLE.bit.GPIO58 = 1; }
inline void PSetDO_8(int p) {GpioDataRegs.GPBDAT.bit.GPIO58 = p; 	}

//Digital Output Channel 9 (GPIO50)
inline void SetDO_9() 		{GpioDataRegs.GPBSET.bit.GPIO50 = 1; 	}
inline void ClearDO_9()		{GpioDataRegs.GPBCLEAR.bit.GPIO50 = 1; 	}
inline void ToggleDO_9()	{GpioDataRegs.GPBTOGGLE.bit.GPIO50 = 1; }
inline void PSetDO_9(int p) {GpioDataRegs.GPBDAT.bit.GPIO50 = p; 	}

//Digital Output Channel 10 (GPIO52)
inline void SetDO_10() 	 	{GpioDataRegs.GPBSET.bit.GPIO52 = 1; 	}
inline void ClearDO_10() 	{GpioDataRegs.GPBCLEAR.bit.GPIO52 = 1;	}
inline void ToggleDO_10()	{GpioDataRegs.GPBTOGGLE.bit.GPIO52 = 1;	}
inline void PSetDO_10(int p){GpioDataRegs.GPBDAT.bit.GPIO52 = p; 	}

//Digital Output - All Channels: 
//		Parameter passed in sets Digital Outputs to DO[11:0]
void SetDO_Vec(Uint16 DO)
{
	PSetDO_0(DO & 0x0001);					//Set DO_0
	PSetDO_1( (DO >> 1) & 0x0001 );			//Set DO_1
	PSetDO_2( (DO >> 2) & 0x0001 );			//Set DO_2
	PSetDO_3( (DO >> 3) & 0x0001 );			//Set DO_3
	PSetDO_4( (DO >> 4) & 0x0001 );			//Set DO_4
	PSetDO_5( (DO >> 5) & 0x0001 );			//Set DO_5
	
	//If SPI is enabled, this will do nothing.
	PSetDO_6( (DO >> 6) & 0x0001 );			//Set DO_6
	PSetDO_7( (DO >> 7) & 0x0001 );			//Set DO_7
	
	PSetDO_8( (DO >> 8) & 0x0001 );			//Set DO_8
	PSetDO_9( (DO >> 9) & 0x0001 );			//Set DO_9
	PSetDO_10( (DO >> 10) & 0x0001 );		//Set DO_10
}

/*Digital Input Wrapper Functions*/
inline int GetDI_0()	{return GpioDataRegs.GPADAT.bit.GPIO24;	}	//Digital Input Channel 0 (GPIO24)
inline int GetDI_1()	{return GpioDataRegs.GPADAT.bit.GPIO25;	}	//Digital Input Channel 1 (GPIO25)
inline int GetDI_2()	{return GpioDataRegs.GPADAT.bit.GPIO26;	}	//Digital Input Channel 2 (GPIO26)
inline int GetDI_3()	{return GpioDataRegs.GPADAT.bit.GPIO27;	}	//Digital Input Channel 3 (GPIO27)
inline int GetDI_4()	{return GpioDataRegs.GPBDAT.bit.GPIO62;	}	//Digital Input Channel 4 (GPIO62)
inline int GetDI_5()	{return GpioDataRegs.GPBDAT.bit.GPIO60;	}	//Digital Input Channel 5 (GPIO60)
inline int GetDI_6()	{return GpioDataRegs.GPBDAT.bit.GPIO54;	}	//Digital Input Channel 6 (GPIO54)
inline int GetDI_7()	{return GpioDataRegs.GPBDAT.bit.GPIO55;	}	//Digital Input Channel 7 (GPIO55)
inline int GetDI_8()	{return GpioDataRegs.GPBDAT.bit.GPIO48;	}	//Digital Input Channel 8 (GPIO48)
inline int GetDI_9()	{return GpioDataRegs.GPBDAT.bit.GPIO61;	}	//Digital Input Channel 9 (GPIO61)
inline int GetDI_10()	{return GpioDataRegs.GPBDAT.bit.GPIO49;	}	//Digital Input Channel 10 (GPIO49)

/*GetDI_Vec()*/
//Returns all 11 digital inputs as a single 16-bit  number, where bit
//10 corresponds to digital input 11.
Uint16 GetDI_Vec()
{
	Uint16 DI = 0x0000;
				  
	//Loop not written to save computation time.
	DI = DI | (GetDI_0() << 0);
	DI = DI | (GetDI_1() << 1);
	DI = DI | (GetDI_2() << 2);
	DI = DI | (GetDI_3() << 3);
	DI = DI | (GetDI_4() << 4);
	DI = DI | (GetDI_5() << 5);
	DI = DI | (GetDI_6() << 6);
	DI = DI | (GetDI_7() << 7);
	DI = DI | (GetDI_8() << 8);
	DI = DI | (GetDI_9() << 9);
	DI = DI | (GetDI_10() << 10);	
	
	return DI;	
}

/*Digital Output Read-Back Functions*/
inline int GetDO_0()	{return GpioDataRegs.GPBDAT.bit.GPIO51;	}	//Digital Input Channel 0 (GPIO51)
inline int GetDO_1()	{return GpioDataRegs.GPBDAT.bit.GPIO53;	}	//Digital Input Channel 1 (GPIO53)
inline int GetDO_2()	{return GpioDataRegs.GPBDAT.bit.GPIO59;	}	//Digital Input Channel 2 (GPIO59)
inline int GetDO_3()	{return GpioDataRegs.GPBDAT.bit.GPIO39;	}	//Digital Input Channel 3 (GPIO39)
inline int GetDO_4()	{return GpioDataRegs.GPBDAT.bit.GPIO36;	}	//Digital Input Channel 4 (GPIO36)
inline int GetDO_5()	{return GpioDataRegs.GPBDAT.bit.GPIO63;	}	//Digital Input Channel 5 (GPIO63)
inline int GetDO_6()	{return GpioDataRegs.GPBDAT.bit.GPIO56;	}	//Digital Input Channel 6 (GPIO56)
inline int GetDO_7()	{return GpioDataRegs.GPBDAT.bit.GPIO57;	}	//Digital Input Channel 7 (GPIO57)
inline int GetDO_8()	{return GpioDataRegs.GPBDAT.bit.GPIO58;	}	//Digital Input Channel 8 (GPIO58)
inline int GetDO_9()	{return GpioDataRegs.GPBDAT.bit.GPIO50;	}	//Digital Input Channel 9 (GPIO50)
inline int GetDO_10()	{return GpioDataRegs.GPBDAT.bit.GPIO52;	}	//Digital Input Channel 10 (GPIO52)

/**********************************************************************************************************/
//IGBT PWM/DO Functions 

/*RST_L Functions: Sends pulse to GPIO 18-23 to reset PWMA/B 0-11*/
//COMMENT
void RST_Ra()
{
	GpioDataRegs.GPACLEAR.bit.GPIO18 = 1;
	DELAY_US(1);
	GpioDataRegs.GPASET.bit.GPIO18 = 1;
}

//COMMENT
void RST_Rb()
{
	GpioDataRegs.GPACLEAR.bit.GPIO19 = 1;
	DELAY_US(1);
	GpioDataRegs.GPASET.bit.GPIO19 = 1;
}

//COMENT
void RST_Rc()
{
	GpioDataRegs.GPACLEAR.bit.GPIO20 = 1;
	DELAY_US(1);
	GpioDataRegs.GPASET.bit.GPIO20 = 1;
}		

//COMMENT.
void RST_Ia()
{
	GpioDataRegs.GPACLEAR.bit.GPIO21 = 1;
	DELAY_US(1);
	GpioDataRegs.GPASET.bit.GPIO21 = 1;
}

//COMMENT.
void RST_Ib()
{
	GpioDataRegs.GPACLEAR.bit.GPIO22 = 1;
	DELAY_US(1);
	GpioDataRegs.GPASET.bit.GPIO22 = 1;
}

//COMMENT.
void RST_Ic()
{
	GpioDataRegs.GPACLEAR.bit.GPIO23 = 1;
	DELAY_US(1);
	GpioDataRegs.GPASET.bit.GPIO23 = 1;
}

/*PWM Enable & Disable Functions*/
inline void EnablePWM_R() {GpioDataRegs.GPBCLEAR.bit.GPIO34 = 1;}   //Enables PWM A, Active Low.
inline void EnablePWM_I() {GpioDataRegs.GPBCLEAR.bit.GPIO35 = 1;}   //Enables PWM B, Active Low.
inline void DisablePWM_R(){GpioDataRegs.GPBSET.bit.GPIO34 = 1;	}  	//Disables PWM A, Active Low.
inline void DisablePWM_I(){GpioDataRegs.GPBSET.bit.GPIO35 = 1;	}  	//Disables PWM B, Active Low.

//Note: can't have PWM functions if PWM module isn't used.
#if(MODULATION)
	/*PWM Set Duty Cycle Functions*/
	void SetPWM_Rau(Uint16 D)	{EPwm1Regs.CMPA.half.CMPA = (Uint16)(D);	}	//PWM1A
	void SetPWM_Rad(Uint16 D)	{EPwm1Regs.CMPB = (Uint16)(D);				}	//PWM1B
	void SetPWM_Rbu(Uint16 D)	{EPwm2Regs.CMPA.half.CMPA = (Uint16)(D);	}	//PWM2A
	void SetPWM_Rbd(Uint16 D)	{EPwm2Regs.CMPB = (Uint16)(D);				}	//PWM2B
	void SetPWM_Rcu(Uint16 D)	{EPwm3Regs.CMPA.half.CMPA = (Uint16)(D); 	}	//PWM3A
	void SetPWM_Rcd(Uint16 D)	{EPwm3Regs.CMPB = (Uint16)(D);				}	//PWM3B	
	void SetPWM_Iau(Uint16 D)	{EPwm4Regs.CMPA.half.CMPA = (Uint16)(D);	}	//PWM4A
	void SetPWM_Iad(Uint16 D)	{EPwm4Regs.CMPB = (Uint16)(D);				}	//PWM4B
	void SetPWM_Ibu(Uint16 D)	{EPwm5Regs.CMPA.half.CMPA = (Uint16)(D);	}	//PWM5A
	void SetPWM_Ibd(Uint16 D)	{EPwm5Regs.CMPB = (Uint16)(D);				}	//PWM5B
	void SetPWM_Icu(Uint16 D)	{EPwm6Regs.CMPA.half.CMPA = (Uint16)(D);	}	//PWM6A
	void SetPWM_Icd(Uint16 D)	{EPwm6Regs.CMPB = (Uint16)(D);				}	//PWM6B

#else
	//If PWM is not used, the ports are configured as digital output and use the four functions.
	
	//IGBT Rau
	inline void SetRau() 		{GpioDataRegs.GPASET.bit.GPIO0 = 1; 	}
	inline void ClearRau()		{GpioDataRegs.GPACLEAR.bit.GPIO0 = 1; 	}
	inline void ToggleRau()		{GpioDataRegs.GPATOGGLE.bit.GPIO0 = 1; 	}
	inline void PSetRau(int p) 	{GpioDataRegs.GPADAT.bit.GPIO0 = p; 	}
	
	//IGBT Rad
	inline void SetRad() 		{GpioDataRegs.GPASET.bit.GPIO1 = 1; 	}
	inline void ClearRad()		{GpioDataRegs.GPACLEAR.bit.GPIO1 = 1; 	}
	inline void ToggleRad()		{GpioDataRegs.GPATOGGLE.bit.GPIO1 = 1; 	}
	inline void PSetRad(int p) 	{GpioDataRegs.GPADAT.bit.GPIO1 = p; 	}
	
	//IGBT Rbu
	inline void SetRbu() 		{GpioDataRegs.GPASET.bit.GPIO2 = 1; 	}
	inline void ClearRbu()		{GpioDataRegs.GPACLEAR.bit.GPIO2 = 1; 	}
	inline void ToggleRbu()		{GpioDataRegs.GPATOGGLE.bit.GPIO2 = 1; 	}
	inline void PSetRbu(int p) 	{GpioDataRegs.GPADAT.bit.GPIO2 = p; 	}
	
	//IGBT Rbd
	inline void SetRbd() 		{GpioDataRegs.GPASET.bit.GPIO3 = 1; 	}
	inline void ClearRbd()		{GpioDataRegs.GPACLEAR.bit.GPIO3 = 1; 	}
	inline void ToggleRbd()		{GpioDataRegs.GPATOGGLE.bit.GPIO3 = 1; 	}
	inline void PSetRbd(int p) 	{GpioDataRegs.GPADAT.bit.GPIO3 = p; 	}
	
	//IGBT Rcu
	inline void SetRcu() 		{GpioDataRegs.GPASET.bit.GPIO4 = 1; 	}
	inline void ClearRcu()		{GpioDataRegs.GPACLEAR.bit.GPIO4 = 1; 	}
	inline void ToggleRcu()		{GpioDataRegs.GPATOGGLE.bit.GPIO4 = 1; 	}
	inline void PSetRcu(int p) 	{GpioDataRegs.GPADAT.bit.GPIO4 = p; 	}
	
	//IGBT Rcd
	inline void SetRcd() 		{GpioDataRegs.GPASET.bit.GPIO5 = 1; 	}
	inline void ClearRcd()		{GpioDataRegs.GPACLEAR.bit.GPIO5 = 1; 	}
	inline void ToggleRcd()		{GpioDataRegs.GPATOGGLE.bit.GPIO5 = 1; 	}
	inline void PSetRcd(int p) 	{GpioDataRegs.GPADAT.bit.GPIO5 = p; 	}
	
	//IGBT Iau
	inline void SetIau() 		{GpioDataRegs.GPASET.bit.GPIO6 = 1; 	}
	inline void ClearIau()		{GpioDataRegs.GPACLEAR.bit.GPIO6 = 1; 	}
	inline void ToggleIau()		{GpioDataRegs.GPATOGGLE.bit.GPIO6 = 1; 	}
	inline void PSetIau(int p) 	{GpioDataRegs.GPADAT.bit.GPIO6 = p; 	}
	
	//IGBT Iad
	inline void SetIad() 		{GpioDataRegs.GPASET.bit.GPIO7 = 1; 	}
	inline void ClearIad()		{GpioDataRegs.GPACLEAR.bit.GPIO7 = 1; 	}
	inline void ToggleIad()		{GpioDataRegs.GPATOGGLE.bit.GPIO7 = 1; 	}
	inline void PSetIad(int p) 	{GpioDataRegs.GPADAT.bit.GPIO7 = p; 	}
	
	//IGBT Ibu
	inline void SetIbu() 		{GpioDataRegs.GPASET.bit.GPIO8 = 1; 	}
	inline void ClearIbu()		{GpioDataRegs.GPACLEAR.bit.GPIO8 = 1; 	}
	inline void ToggleIbu()		{GpioDataRegs.GPATOGGLE.bit.GPIO8 = 1; 	}
	inline void PSetIbu(int p) 	{GpioDataRegs.GPADAT.bit.GPIO8 = p; 	}
	
	//IGBT Ibd
	inline void SetIbd() 		{GpioDataRegs.GPASET.bit.GPIO9 = 1; 	}
	inline void ClearIbd()		{GpioDataRegs.GPACLEAR.bit.GPIO9 = 1; 	}
	inline void ToggleIbd()		{GpioDataRegs.GPATOGGLE.bit.GPIO9 = 1; 	}
	inline void PSetIbd(int p) 	{GpioDataRegs.GPADAT.bit.GPIO9 = p; 	}
	
	//IGBT Icu
	inline void SetIcu() 		{GpioDataRegs.GPASET.bit.GPIO10 = 1; 	}
	inline void ClearIcu()		{GpioDataRegs.GPACLEAR.bit.GPIO10 = 1; 	}
	inline void ToggleIcu()		{GpioDataRegs.GPATOGGLE.bit.GPIO10 = 1; }
	inline void PSetIcu(int p) 	{GpioDataRegs.GPADAT.bit.GPIO10 = p; 	}
	
	//IGBT Icd
	inline void SetIcd() 		{GpioDataRegs.GPASET.bit.GPIO11 = 1; 	}
	inline void ClearIcd()		{GpioDataRegs.GPACLEAR.bit.GPIO11 = 1; 	}
	inline void ToggleIcd()		{GpioDataRegs.GPATOGGLE.bit.GPIO11 = 1; }
	inline void PSetIcd(int p) 	{GpioDataRegs.GPADAT.bit.GPIO11 = p; 	}

#endif		//End alternative IGBT control.

#ifdef npc
	/*PWM Set Duty Cycle Functions*/
	void SetPWM_Na1(Uint16 D)	{EPwm1Regs.CMPA.half.CMPA = (Uint16)(D);	}	//PWM1A
	void SetPWM_Na3(Uint16 D)	{EPwm1Regs.CMPB = (Uint16)(D);				}	//PWM1B
	void SetPWM_Na2(Uint16 D)	{EPwm2Regs.CMPA.half.CMPA = (Uint16)(D);	}	//PWM2A
	void SetPWM_Na4(Uint16 D)	{EPwm2Regs.CMPB = (Uint16)(D);				}	//PWM2B
	void SetPWM_Nb1(Uint16 D)	{EPwm3Regs.CMPA.half.CMPA = (Uint16)(D); 	}	//PWM3A
	void SetPWM_Nb3(Uint16 D)	{EPwm3Regs.CMPB = (Uint16)(D);				}	//PWM3B
	void SetPWM_Nb2(Uint16 D)	{EPwm4Regs.CMPA.half.CMPA = (Uint16)(D);	}	//PWM4A
	void SetPWM_Nb4(Uint16 D)	{EPwm4Regs.CMPB = (Uint16)(D);				}	//PWM4B
	void SetPWM_Nc1(Uint16 D)	{EPwm5Regs.CMPA.half.CMPA = (Uint16)(D);	}	//PWM5A
	void SetPWM_Nc3(Uint16 D)	{EPwm5Regs.CMPB = (Uint16)(D);				}	//PWM5B
	void SetPWM_Nc2(Uint16 D)	{EPwm6Regs.CMPA.half.CMPA = (Uint16)(D);	}	//PWM6A
	void SetPWM_Nc4(Uint16 D)	{EPwm6Regs.CMPB = (Uint16)(D);				}	//PWM6B
#endif
/*****************************************************************************************************/
/*ADC GET FUNCTIONS*/

//Start ADC Sequence. This starts an ADC sequence.
//	Note: There is a 2 us delay inserted here to let the ADC sequence finish.  This is
//			to ensure that the adc values that are read are up to date. This can
//			be removed for faster code as long as the delay is accounted for elsewhere.
inline void StartADC()
{
	AdcRegs.ADCTRL2.bit.SOC_SEQ1 = 1;  		//Start adc conversion.
	DELAY_US(2);							//2us delay for conversion to finsih.  	
}
//GetAIN_Vec(): Pass in a 16-element array by reference, this function fills in with AIN0-16.
//MUST BE float32 OR THIS WILL CAUSE PROBLEMS.
void GetAIN_Vec(float32 ADC_VEC[])
{
	if(ADC_VEC != 0) //Make sure it's initialized.
	{
		ADC_VEC[0] = (AdcRegs.ADCRESULT0 >> 4);		//Right Align, scale properly. AINA0
		ADC_VEC[1] = (AdcRegs.ADCRESULT1 >> 4);		//AINA1
		ADC_VEC[2] = (AdcRegs.ADCRESULT2 >> 4);		//AINA2
		ADC_VEC[3] = (AdcRegs.ADCRESULT3 >> 4);		//AINA3
		ADC_VEC[4] = (AdcRegs.ADCRESULT4 >> 4);		//AINA4
		ADC_VEC[5] = (AdcRegs.ADCRESULT5 >> 4);		//AINA5
		ADC_VEC[6] = (AdcRegs.ADCRESULT6 >> 4);		//AINA6
		ADC_VEC[7] = (AdcRegs.ADCRESULT7 >> 4);		//AINA7
		ADC_VEC[8] = (AdcRegs.ADCRESULT8 >> 4);		//AINB0
		ADC_VEC[9] = (AdcRegs.ADCRESULT9 >> 4);  	//AINB1
		ADC_VEC[10] = (AdcRegs.ADCRESULT10 >> 4);		//AINB2
		ADC_VEC[11] = (AdcRegs.ADCRESULT11 >> 4);		//AINB3
		ADC_VEC[12] = (AdcRegs.ADCRESULT12 >> 4);		//AINB4
		ADC_VEC[13] = (AdcRegs.ADCRESULT13 >> 4);		//AINB5
		ADC_VEC[14] = (AdcRegs.ADCRESULT14 >> 4);		//AINB6
		ADC_VEC[15] = (AdcRegs.ADCRESULT15 >> 4);		//AINB7
	}	
}

/*Individual ADC Reads.  Must Right Align and Scale.*/
inline float32 GetAIN_A0()	{return ((AdcRegs.ADCRESULT0 >> 4));}	//Read ADC Channel A0
inline float32 GetAIN_A1()	{return ((AdcRegs.ADCRESULT1 >> 4));}	//Read ADC Channel A1
inline float32 GetAIN_A2()	{return ((AdcRegs.ADCRESULT2 >> 4));}	//Read ADC Channel A2
inline float32 GetAIN_A3()	{return ((AdcRegs.ADCRESULT3 >> 4));}	//Read ADC Channel A3
inline float32 GetAIN_A4()	{return ((AdcRegs.ADCRESULT4 >> 4));}	//Read ADC Channel A4
inline float32 GetAIN_A5()	{return ((AdcRegs.ADCRESULT5 >> 4));}	//Read ADC Channel A5
inline float32 GetAIN_A6()	{return ((AdcRegs.ADCRESULT6 >> 4));}	//Read ADC Channel A6
inline float32 GetAIN_A7()	{return ((AdcRegs.ADCRESULT7 >> 4));}	//Read ADC Channel A7
inline float32 GetAIN_B0()	{return ((AdcRegs.ADCRESULT8 >> 4));}	//Read ADC Channel B0
inline float32 GetAIN_B1()	{return ((AdcRegs.ADCRESULT9 >> 4));}	//Read ADC Channel B1
inline float32 GetAIN_B2()	{return ((AdcRegs.ADCRESULT10 >> 4));}	//Read ADC Channel B2
inline float32 GetAIN_B3()	{return ((AdcRegs.ADCRESULT11 >> 4));}	//Read ADC Channel B3
inline float32 GetAIN_B4()	{return ((AdcRegs.ADCRESULT12 >> 4));}	//Read ADC Channel B4
inline float32 GetAIN_B5()	{return ((AdcRegs.ADCRESULT13 >> 4));}	//Read ADC Channel B5
inline float32 GetAIN_B6()	{return ((AdcRegs.ADCRESULT14 >> 4));}	//Read ADC Channel B6
inline float32 GetAIN_B7()	{return ((AdcRegs.ADCRESULT15 >> 4));}	//Read ADC Channel B7

//ADC ISR: Since the Get_AINxx() returns the values, the ISR just resets the ADC for the next sequence.
interrupt void adc_isr(void)
{
	//Reinitialize for next ADC sequence.
	AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;					//Reset Sequencer
	AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;					//Reset ADC interrupt enable.
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;				//Acknowledge the interrupt to PIE.
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;				//Acknowledge the interrupt to PIE.
	return;
}

/********************************************************************************************************/
//Timer start & stop functions.
inline void StartTimer()	{CpuTimer0Regs.TCR.bit.TSS = 0;}	//Start CPU Timer.
inline void StopTimer()		{CpuTimer0Regs.TCR.bit.TSS = 1;}	//Stop CPU Timer.

/********************************************************************************************************/
//DSP_init - Initializes all DSP hardware and peripherals.
void DSP_init()
{
	//Initialize PLL, WatchDog, and enable Peripheral Clocks
	InitSysCtrl();
	
	//Clear interrupts and initialize PIE vector table:
	DINT;
	InitPieCtrl();
	IER = 0x0000;
	IFR = 0x0000;
	InitPieVectTable();
	
	//Initialize all peripherals specific to board:
	EALLOW;
	
	//Initialize GPAMUX1: (Sets Gpio 0-11 for PWM & Gpio 12-15 for TZx if PWM is used.
	//						Gpio32&33 are for I2C)
	GpioCtrlRegs.GPAMUX1.all = GPAMUX_1;			
	GpioCtrlRegs.GPAMUX2.all = GPAMUX_2;
	GpioCtrlRegs.GPBMUX1.all = GPBMUX_1;
	GpioCtrlRegs.GPBMUX2.all = GPBMUX_2;
	GpioCtrlRegs.GPAMUX2.bit.GPIO30 = 1;	// CANA_RX added 11/26/2013 JPL
	GpioCtrlRegs.GPAMUX2.bit.GPIO31 = 1;	// CANA_TX added 11/26/2013 JPL
	
	/* Enable internal pull-up for the selected pins */

	//For I2C:
	GpioCtrlRegs.GPBPUD.bit.GPIO32 = 0;    // Enable pull-up for GPIO32 (SDAA)
	GpioCtrlRegs.GPBPUD.bit.GPIO33 = 0;	   // Enable pull-up for GPIO33 (SCLA)
	/* Set qualification for selected pins to asynch only */
	GpioCtrlRegs.GPBQSEL1.bit.GPIO32 = 3;  // Asynch input GPIO32 (SDAA)
    GpioCtrlRegs.GPBQSEL1.bit.GPIO33 = 3;  // Asynch input GPIO33 (SCLA)
	
	
	//*****************************************************************************
	/*Digital I/O Setup.
	 * Digital Outputs: GPIO 18-23, 34-36, 39, 50-53, 56-59, 63.
	 * Digital Inputs:  GPIO 24-27, 48-49, 54-55, 60-62.
	 */
	
	//Default Values.  We set these before changing to outputs.

#if(!MODULATION)	//If PWM isn't used, default value of R/Ixx is 0.
	GpioDataRegs.GPACLEAR.bit.GPIO0 = 0;			//Rau
	GpioDataRegs.GPACLEAR.bit.GPIO1 = 0;			//Rad
	GpioDataRegs.GPACLEAR.bit.GPIO2 = 0;			//Rbu
	GpioDataRegs.GPACLEAR.bit.GPIO3 = 0;			//Rbd
	GpioDataRegs.GPACLEAR.bit.GPIO4 = 0;			//Rcu
	GpioDataRegs.GPACLEAR.bit.GPIO5 = 0;			//Rcd
	GpioDataRegs.GPACLEAR.bit.GPIO6 = 0;			//Iau
	GpioDataRegs.GPACLEAR.bit.GPIO7 = 0;			//Iad
	GpioDataRegs.GPACLEAR.bit.GPIO8 = 0;			//Ibu
	GpioDataRegs.GPACLEAR.bit.GPIO9 = 0;			//Ibd
	GpioDataRegs.GPACLEAR.bit.GPIO10 = 0;			//Icu
	GpioDataRegs.GPACLEAR.bit.GPIO11 = 0;			//Icd
#endif
	
	GpioDataRegs.GPASET.bit.GPIO18 = 1;				//RST_Ra
	GpioDataRegs.GPASET.bit.GPIO19 = 1;				//RST_Rb
	GpioDataRegs.GPASET.bit.GPIO20 = 1;				//RST_Rc
	GpioDataRegs.GPASET.bit.GPIO21 = 1;				//RST_Ia
	GpioDataRegs.GPASET.bit.GPIO22 = 1;				//RST_Ib
	GpioDataRegs.GPASET.bit.GPIO23 = 1;				//RST_Ic
	GpioDataRegs.GPBSET.bit.GPIO34 = 1;				//Enable_PWMA()
	GpioDataRegs.GPBSET.bit.GPIO35 = 1;				//Enable_PWMB()
	GpioDataRegs.GPBCLEAR.bit.GPIO51 = 1;			//DO_0;
	GpioDataRegs.GPBCLEAR.bit.GPIO53 = 1;			//DO_1;
	GpioDataRegs.GPBCLEAR.bit.GPIO59 = 1;			//DO_2;
	GpioDataRegs.GPBCLEAR.bit.GPIO39 = 1;			//DO_3;
	GpioDataRegs.GPBCLEAR.bit.GPIO36 = 1;			//DO_4;
	GpioDataRegs.GPBCLEAR.bit.GPIO63 = 1;			//DO_5;
	GpioDataRegs.GPBCLEAR.bit.GPIO56 = 1;			//DO_6;
	GpioDataRegs.GPBCLEAR.bit.GPIO57 = 1;			//DO_7;
	GpioDataRegs.GPBCLEAR.bit.GPIO58 = 1;			//DO_8;
	GpioDataRegs.GPBCLEAR.bit.GPIO50 = 1;			//DO_9;
	GpioDataRegs.GPBCLEAR.bit.GPIO52 = 1;			//DO_10;
	
	//Set Direction Register: 0->input & 1->output. 
	//Note: Default is '0' so we only change '1'.

	GpioCtrlRegs.GPADIR.bit.GPIO18 = 1;				//RST_L - PWM Ra
	GpioCtrlRegs.GPADIR.bit.GPIO19 = 1;				//RST_L - PWM Rb
	GpioCtrlRegs.GPADIR.bit.GPIO20 = 1;				//RST_L - PWM Ru
	GpioCtrlRegs.GPADIR.bit.GPIO21 = 1;				//RST_L - PWM Ia
	GpioCtrlRegs.GPADIR.bit.GPIO22 = 1;				//RST_L - PWM Ib
	GpioCtrlRegs.GPADIR.bit.GPIO23 = 1;				//RST_L - PWM Ic
	
	GpioCtrlRegs.GPBDIR.bit.GPIO34 = 1;				//EN_L - PWM Rxx
	GpioCtrlRegs.GPBDIR.bit.GPIO35 = 1;				//EN_L - PWM Ixx
	GpioCtrlRegs.GPBDIR.bit.GPIO36 = 1;				//DO_4
	GpioCtrlRegs.GPBDIR.bit.GPIO39 = 1;				//DO_3
	
	GpioCtrlRegs.GPBDIR.bit.GPIO50 = 1;				//DO_9
	GpioCtrlRegs.GPBDIR.bit.GPIO51 = 1;				//DO_0
	GpioCtrlRegs.GPBDIR.bit.GPIO52 = 1;				//DO_10
	GpioCtrlRegs.GPBDIR.bit.GPIO53 = 1;				//DO_1
	
	GpioCtrlRegs.GPBDIR.bit.GPIO58 = 1;				//DO_8
	GpioCtrlRegs.GPBDIR.bit.GPIO59 = 1;				//DO_2
	GpioCtrlRegs.GPBDIR.bit.GPIO63 = 1;				//DO_5
	
	//If SPI is not used, we set 56/57 to output (54 & 55 are input by default.) 
#if(!SPI)
		GpioCtrlRegs.GPBDIR.bit.GPIO56 = 1;			//DO_6
		GpioCtrlRegs.GPBDIR.bit.GPIO57 = 1;			//DO_7
#endif
		
	//Configure Pull Up Resistors:
	//Note: Pull Up Resistors are enabled by default, except for PWM.
	
#if(MODULATION)
	//****************************************************************
	/*ePWM Setup.  ePWM A/B 1-6 are used.
	 * PWM Constant Definitions are in 'DSP2833x_EPwm_defines.h'
	 * TI Manual for this section as of 2/27/2014: "TMS320x2833x,
	 * 2823x Enhanced Pulse Width Modulator (ePWM) Module"
	 * Literature Number SPRUG04A October 2008-Revised 2009
	 * http://www.ti.com/lit/ug/sprug04a/sprug04a.pdf
	 * 
	 * PWM Settings:
	 * 			Symmetrical PWM using TB_COUNT_UPDOWN
	 * 			Counter counts up to PWM_PD then back down
	 * 			so period is actually 2*PWM_PD.
	 * 			PWM_PD set at top of ECI_API.h
	 *
	 * 			2*PWM_PD = (150 MHz / fsw)
	 * 			ex 1: for fsw = 10 kHz, PWM_PD = 7500
	 * 			ex 2: for fsw = 20 kHz, PWM_PD = 3750
	 *
	 * 
	 * Clock enable in init_sys_control().
	 * Trap zones TZ1-6 are used as well.
	 * All PWMs are synced.
	 * 
	*/
	//First, we disable global clock sync (PWM clock already enabled).
	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;

	//Then set all PWM(1-6) modules up.
	//*********PWM 1*********//
	EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;     // Select INT on Zero event
	EPwm1Regs.ETSEL.bit.INTEN = 1;  			  // Enable INT
	EPwm1Regs.ETPS.bit.INTPRD = ET_1ST;           // Generate INT on 1st event

	EPwm1Regs.TBPRD = PWM_PD;						//Set Switching Frequency.
	EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;		//Counter will count up.
	EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;			//Phase Loading Enabled for PWM1.
	EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;		//Sync output is enabled, sent when cnt -> 0.
	EPwm1Regs.TBCTL.bit.PRDLD = TB_SHADOW;			//Shadowing enabled, D update on counter = 0.
	EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;		//TBCLK = SYSCLK
	
	EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;		//Enable Shadow register for CMPA.
	EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;		//Enable Shadow register for CMPB.
	
	EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;	//Loads CMPA on cnt = 0;
	EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;	//Loads CMPB when cnt = 0
	
	EPwm1Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;	//Enables Dead-band.
	EPwm1Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;		//Dead-band is active high complementary.
	
	EPwm1Regs.DBFED = DEAD_BAND;					//Falling Edge Dead-Band.
	EPwm1Regs.DBRED = DEAD_BAND;					//Rising Edge Dead-Band.
	
	EPwm1Regs.AQCTLA.bit.CAU = AQ_CLEAR;				//Sets PWMA output when cnt = 0;
	EPwm1Regs.AQCTLA.bit.CAD = AQ_SET;			//Clears PWMA output when cnt = CMPA and cnt is incrementing.
	//EPwm1Regs.AQCTLB.bit.ZRO = AQ_SET;			//Commented because in complemented mode.
	//EPwm1Regs.AQCTLB.bit.CBU = AQ_CLEAR;			//Commented because in complemented mode.
	
	EPwm1Regs.TBCTR = 0;							//Clear PWM1 Counter. 
	EPwm1Regs.CMPA.half.CMPA = 0;					//Initial Duty Cycle = 0.	
	//EPwm1Regs.CMPB = 0;							//Initial Duty Cycle (B) (Commented due to config).
	
	
	//*********PWM 2*********//		
	EPwm2Regs.TBPRD = PWM_PD;						//Set Switching Frequency.
	EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;		//Counter will count up.
	EPwm2Regs.TBCTL.bit.PHSEN = TB_ENABLE;			//Phase Loading Enabled for PWM1.
	EPwm2Regs.TBPHS.half.TBPHS = 0;					//Phase register, it is in phase.
	EPwm2Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;		//Sync output is enabled, sent when SYNC_IN is input..
	EPwm2Regs.TBCTL.bit.PRDLD = TB_SHADOW;			//Shadowing enabled, D update on counter = 0.
	EPwm2Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;		//TBCLK = SYSCLK
	
	EPwm2Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;		//Enable Shadow register for CMPA.
	EPwm2Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;		//Enable Shadow register for CMPB.

	EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;	//Loads CMPA on cnt = 0;
	EPwm2Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;	//Loads CMPB when cnt = 0
	
	EPwm2Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;	//Enables Dead-band.
	EPwm2Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;		//Dead-band is active high complementary.
	
	EPwm2Regs.DBFED = DEAD_BAND;					//Falling Edge Dead-Band.
	EPwm2Regs.DBRED = DEAD_BAND;					//Rising Edge Dead-Band.
	
	EPwm2Regs.AQCTLA.bit.CAU = AQ_CLEAR;				//Sets PWMA output when cnt = 0;
	EPwm2Regs.AQCTLA.bit.CAD = AQ_SET;			//Clears PWMA output when cnt = CMPA and cnt is incrementing.
	//EPwm1Regs.AQCTLB.bit.ZRO = AQ_SET;			//Commented because in complemented mode.
	//EPwm1Regs.AQCTLB.bit.CBU = AQ_CLEAR;			//Commented because in complemented mode.
	
	EPwm2Regs.TBCTR = 0;							//Clear PWM1 Counter. 
	EPwm2Regs.CMPA.half.CMPA = 0;					//Initial Duty Cycle = 0.	
	//EPwm1Regs.CMPB = 0;							//Initial Duty Cycle (B) (Commented due to config).
	
	//*********PWM 3*********//		
	EPwm3Regs.TBPRD = PWM_PD;						//Set Switching Frequency.
	EPwm3Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;		//Counter will count up.
	EPwm3Regs.TBCTL.bit.PHSEN = TB_ENABLE;			//Phase Loading Enabled for PWM1.
	EPwm3Regs.TBPHS.half.TBPHS = 0;					//Phase register, it is in phase.
	EPwm3Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;		//Sync output is enabled, sent when SYNC_IN is input..
	EPwm3Regs.TBCTL.bit.PRDLD = TB_SHADOW;			//Shadowing enabled, D update on counter = 0.
	EPwm3Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;		//TBCLK = SYSCLK
	
	EPwm3Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;		//Enable Shadow register for CMPA.
	EPwm3Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;		//Enable Shadow register for CMPB.

	EPwm3Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;	//Loads CMPA on cnt = 0;
	EPwm3Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;	//Loads CMPB when cnt = 0
	
	EPwm3Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;	//Enables Dead-band.
	EPwm3Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;		//Dead-band is active high complementary.
	
	EPwm3Regs.DBFED = DEAD_BAND;					//Falling Edge Dead-Band.
	EPwm3Regs.DBRED = DEAD_BAND;					//Rising Edge Dead-Band.
	
	EPwm3Regs.AQCTLA.bit.CAU = AQ_CLEAR;				//Sets PWMA output when cnt = 0;
	EPwm3Regs.AQCTLA.bit.CAD = AQ_SET;			//Clears PWMA output when cnt = CMPA and cnt is incrementing.
	//EPwm1Regs.AQCTLB.bit.ZRO = AQ_SET;			//Commented because in complemented mode.
	//EPwm1Regs.AQCTLB.bit.CBU = AQ_CLEAR;			//Commented because in complemented mode.
	
	EPwm3Regs.TBCTR = 0;							//Clear PWM1 Counter. 
	EPwm3Regs.CMPA.half.CMPA = 0;					//Initial Duty Cycle = 0.	
	//EPwm1Regs.CMPB = 0;							//Initial Duty Cycle (B) (Commented due to config).
	
	//*********PWM 4*********//		
	EPwm4Regs.TBPRD = PWM_PD;						//Set Switching Frequency.
	EPwm4Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;	//Counter will count up.
	EPwm4Regs.TBCTL.bit.PHSEN = TB_ENABLE;			//Phase Loading Enabled for PWM1.
	EPwm4Regs.TBPHS.half.TBPHS = 0;					//Phase register, it is in phase.
	EPwm4Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;		//Sync output is enabled, sent when SYNC_IN is input..
	EPwm4Regs.TBCTL.bit.PRDLD = TB_SHADOW;			//Shadowing enabled, D update on counter = 0.
	EPwm4Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;		//TBCLK = SYSCLK
	
	EPwm4Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;		//Enable Shadow register for CMPA.
	EPwm4Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;		//Enable Shadow register for CMPB.

	EPwm4Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;	//Loads CMPA on cnt = 0;
	EPwm4Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;	//Loads CMPB when cnt = 0
	
	EPwm4Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;	//Enables Dead-band.
	EPwm4Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;		//Dead-band is active high complementary.
	
	EPwm4Regs.DBFED = DEAD_BAND;					//Falling Edge Dead-Band.
	EPwm4Regs.DBRED = DEAD_BAND;					//Rising Edge Dead-Band.
	
	EPwm4Regs.AQCTLA.bit.CAU = AQ_CLEAR;			//Sets PWMA output when cnt = 0;
	EPwm4Regs.AQCTLA.bit.CAD = AQ_SET;				//Clears PWMA output when cnt = CMPA and cnt is incrementing.
	//EPwm1Regs.AQCTLB.bit.ZRO = AQ_SET;			//Commented because in complemented mode.
	//EPwm1Regs.AQCTLB.bit.CBU = AQ_CLEAR;			//Commented because in complemented mode.
	
	EPwm4Regs.TBCTR = 0;							//Clear PWM1 Counter. 
	EPwm4Regs.CMPA.half.CMPA = 0;					//Initial Duty Cycle = 0.	
	//EPwm1Regs.CMPB = 0;							//Initial Duty Cycle (B) (Commented due to config).
	
	//*********PWM 5*********//		
	EPwm5Regs.TBPRD = PWM_PD;						//Set Switching Frequency.
	EPwm5Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;	//Counter will count up.
	EPwm5Regs.TBCTL.bit.PHSEN = TB_ENABLE;			//Phase Loading Enabled for PWM1.
	EPwm5Regs.TBPHS.half.TBPHS = 0;					//Phase register, it is in phase.
	EPwm5Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;		//Sync output is enabled, sent when SYNC_IN is input..
	EPwm5Regs.TBCTL.bit.PRDLD = TB_SHADOW;			//Shadowing enabled, D update on counter = 0.
	EPwm5Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;		//TBCLK = SYSCLK
	
	EPwm5Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;		//Enable Shadow register for CMPA.
	EPwm5Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;		//Enable Shadow register for CMPB.

	EPwm5Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;	//Loads CMPA on cnt = 0;
	EPwm5Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;	//Loads CMPB when cnt = 0
	
	EPwm5Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;	//Enables Dead-band.
	EPwm5Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;		//Dead-band is active high complementary.
	
	EPwm5Regs.DBFED = DEAD_BAND;					//Falling Edge Dead-Band.
	EPwm5Regs.DBRED = DEAD_BAND;					//Rising Edge Dead-Band.
	
	EPwm5Regs.AQCTLA.bit.CAU = AQ_CLEAR;			//Sets PWMA output when cnt = 0;
	EPwm5Regs.AQCTLA.bit.CAD = AQ_SET;				//Clears PWMA output when cnt = CMPA and cnt is incrementing.
	//EPwm1Regs.AQCTLB.bit.ZRO = AQ_SET;			//Commented because in complemented mode.
	//EPwm1Regs.AQCTLB.bit.CBU = AQ_CLEAR;			//Commented because in complemented mode.
	
	EPwm5Regs.TBCTR = 0;							//Clear PWM1 Counter. 
	EPwm5Regs.CMPA.half.CMPA = 0;					//Initial Duty Cycle = 0.	
	//EPwm1Regs.CMPB = 0;							//Initial Duty Cycle (B) (Commented due to config).
	
	//*********PWM 6*********//		
	EPwm6Regs.TBPRD = PWM_PD;						//Set Switching Frequency.
	EPwm6Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;	//Counter will count up.
	EPwm6Regs.TBCTL.bit.PHSEN = TB_ENABLE;			//Phase Loading Enabled for PWM1.
	EPwm6Regs.TBPHS.half.TBPHS = 0;					//Phase register, it is in phase.
	EPwm6Regs.TBCTL.bit.SYNCOSEL = TB_DISABLE;		//Sync output is disabled on PWM6.
	EPwm6Regs.TBCTL.bit.PRDLD = TB_SHADOW;			//Shadowing enabled, D update on counter = 0.
	EPwm6Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;		//TBCLK = SYSCLK
	
	EPwm6Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;		//Enable Shadow register for CMPA.
	EPwm6Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;		//Enable Shadow register for CMPB.

	EPwm6Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;	//Loads CMPA on cnt = 0;
	EPwm6Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;	//Loads CMPB when cnt = 0
	
	EPwm6Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;	//Enables Dead-band.
	EPwm6Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;		//Dead-band is active high complementary.
	
	EPwm6Regs.DBFED = DEAD_BAND;					//Falling Edge Dead-Band.
	EPwm6Regs.DBRED = DEAD_BAND;					//Rising Edge Dead-Band.
	
	EPwm6Regs.AQCTLA.bit.CAU = AQ_CLEAR;			//Sets PWMA output when cnt = 0;
	EPwm6Regs.AQCTLA.bit.CAD = AQ_SET;				//Clears PWMA output when cnt = CMPA and cnt is incrementing.
	//EPwm1Regs.AQCTLB.bit.ZRO = AQ_SET;			//Commented because in complemented mode.
	//EPwm1Regs.AQCTLB.bit.CBU = AQ_CLEAR;			//Commented because in complemented mode.
	
	EPwm6Regs.TBCTR = 0;							//Clear PWM1 Counter. 
	EPwm6Regs.CMPA.half.CMPA = 0;					//Initial Duty Cycle = 0.	
	//EPwm1Regs.CMPB = 0;							//Initial Duty Cycle (B) (Commented due to config).
	
	//Then enable global clock syncing.
	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;

#endif //Don't define these if not using PWM.

	//********************************************************
	//ADC Module Set UP:
	//	-Cascaded Sequential Mode. 
	//	-CSP is 1, therefore the ADC clock is HSPCLK/2 = 12.5 MHz.
	//	-S/H is 1 ADC Clock cycle (80 ns)
	//
	//	-At 12.5 Msps, 16 channels/sequence, sample rate can be up to 781,250/second/channel.
	//
	//	-ADC interrupt is given in adc_isr().
	//	-ADC clock is enabled already.
	
   #if (CPU_FRQ_150MHZ)     // Default - 150 MHz SYSCLKOUT
     #define ADC_MODCLK 0x3 // HSPCLK = SYSCLKOUT/2*ADC_MODCLK2 = 150/(2*3)   = 25.0 MHz
   #endif
   #if (CPU_FRQ_100MHZ)
     #define ADC_MODCLK 0x2 // HSPCLK = SYSCLKOUT/2*ADC_MODCLK2 = 100/(2*2)   = 25.0 MHz
   #endif
   
	PieVectTable.ADCINT = &adc_isr;					//Assign ISR
	AdcRegs.ADCTRL3.all = 0x00E0; 					//Power up everything.
	DELAY_US(100000);					    		//Need at least 5ms delay.  giving 10ms.

	PieCtrlRegs.PIEIER1.bit.INTx6 = 0x1;			//PIE Interrupt Enable
	//IER |= M_INT1; 								//Enable CPU Interrupt 1 (PIE Set 1).
	IER |= M_INT3;									// Enable CPU INT3 which is connected to EPWM1-6 INT:
	EINT;		   									//Enable Global interrupt INTM
	ERTM;		   									//Enable Global reatime interrupt DBGM.
	
	AdcRegs.ADCTRL3.bit.ADCCLKPS = 0x0;				//Scaling is 1-1.
	AdcRegs.ADCTRL1.bit.CPS = 0x1;					//CPS scaling is 2.
	AdcRegs.ADCTRL1.bit.ACQ_PS = 0x0;				//Sample-Hold Pulse is 1 ADCCLK (80ns)
	
	//Set up conversions.
	AdcRegs.ADCMAXCONV.bit.MAX_CONV1 = 0xF;			//16 conversions per sequence.
	AdcRegs.ADCMAXCONV.bit.MAX_CONV2 = 0x0;			//Read 1 ADC Result.
	
	AdcRegs.ADCTRL1.bit.CONT_RUN = 0x0;				//Continuous conversion is off.  MUST RESET in ADC ISR.
	AdcRegs.ADCTRL1.bit.SEQ_CASC = 0x1;				//Sequential Simultaneous mode.  SEQ is 16-states.
	
	AdcRegs.ADCREFSEL.bit.REF_SEL = 0x00;			//Internal Reference.
	
	AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0;			//ADC Result 0 is ADCINA0;
	AdcRegs.ADCCHSELSEQ1.bit.CONV01 = 1;			//ADC Result 1 is ADCINA1;
	AdcRegs.ADCCHSELSEQ1.bit.CONV02 = 2;			//ADC Result 2 is ADCINA2;
	AdcRegs.ADCCHSELSEQ1.bit.CONV03 = 3;			//ADC Result 3 is ADCINA3;
	AdcRegs.ADCCHSELSEQ2.bit.CONV04 = 4;			//ADC Result 4 is ADCINA4;
	AdcRegs.ADCCHSELSEQ2.bit.CONV05 = 5;			//ADC Result 5 is ADCINA5;
	AdcRegs.ADCCHSELSEQ2.bit.CONV06 = 6;			//ADC Result 6 is ADCINA6;
	AdcRegs.ADCCHSELSEQ2.bit.CONV07 = 7;			//ADC Result 7 is ADCINA7;
	AdcRegs.ADCCHSELSEQ3.bit.CONV08 = 8;			//ADC Result 8 is ADCINA8;
	AdcRegs.ADCCHSELSEQ3.bit.CONV09 = 9;			//ADC Result 9 is ADCINA9;
	AdcRegs.ADCCHSELSEQ3.bit.CONV10 = 10;			//ADC Result 10 is ADCINA10;
	AdcRegs.ADCCHSELSEQ3.bit.CONV11 = 11;			//ADC Result 11 is ADCINA11;
	AdcRegs.ADCCHSELSEQ4.bit.CONV12 = 12;			//ADC Result 12 is ADCINA12;
	AdcRegs.ADCCHSELSEQ4.bit.CONV13 = 13;			//ADC Result 13 is ADCINA13;
	AdcRegs.ADCCHSELSEQ4.bit.CONV14 = 14;			//ADC Result 14 is ADCINA14;
	AdcRegs.ADCCHSELSEQ4.bit.CONV15 = 15;			//ADC Result 15 is ADCINA15;
							
	AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 = 1;			//Enable interrupt request.	
	
	
	/*************************************
	 * CPU Timer Setup
	 * 
	 * We use CPU Timer 0 as the main timer set up.
	 * Frequency is 9.6 Khz (TIMER_0_PD, same as PWM_PD).
	 *
	 * 11/13/2013 Jesse Leonard
	 * Main timer_isr timer changed to PWM 1 interrupt, this is done with
	 * PieVectTable.EPWM1_INT = &timer_isr
	 * The main.c was modified as well to have
	 * interrupt void timer_isr(void)
     *  {
     *  // Clear INT flag for this timer
	 *  EPwm1Regs.ETCLR.bit.INT = 1;
     *  .
     *  .
     *  .
	 * // Acknowledge this interrupt to receive more interrupts from group 3
	 * PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
	 * ...
	 *
	 *
	 *
	 */
	CpuTimer0Regs.PRD.half.LSW = TIMER_0_PD;			//9.6 KHz
	CpuTimer0Regs.TCR.bit.TIE = 0x1;					//Enable Timer Interrupt.				
	CpuTimer0Regs.TCR.bit.FREE = 0x0;					//Causes timer to stop during debug halt.
	CpuTimer0Regs.TCR.bit.SOFT = 0x0;					//Debug operation.
	CpuTimer0Regs.TCR.bit.TRB = 0x1;					//Loads timer register with count.
	CpuTimer0Regs.TCR.bit.TSS = 0x1;					//Timer starts of halted.
	CpuTimer0Regs.TPR.all = 0x0;						//Prescaler is 0.
	//PieVectTable.TINT0 = &timer_isr;					//Assign timer_isr function to the PIE interrupt vector.
	PieVectTable.EPWM1_INT = &timer_isr;				//Assign timer_isr function to EPWM1 interrupt
	//PieCtrlRegs.PIEIER1.bit.INTx7 = 1;				//Enable TINT0, group 1 int 7.
    PieCtrlRegs.PIEIER3.bit.INTx1 = 1;    				// Enable EPWM INTn in the PIE: Group 3 interrupt 1

	EDIS;
	
	/***********************************
	 * I2C Setup for DAC.
	 * We run in FIFO mode, no interrupts.
	 */
   	I2caRegs.I2CSAR = DAC_ADDRESS;		// Slave address - DAC
    I2caRegs.I2CPSC.all = 14;   			// Prescaler - need 7-12 Mhz on module clk (150/15 = 10MHz)
   	I2caRegs.I2CCLKL = 15;					// SCL High Counter -> Together SCL freq = 312.5 KHz.
   	I2caRegs.I2CCLKH = 7;					// SCL Low Counter -> 
   	I2caRegs.I2CIER.all = 0x0000;			// No interrupts
   	I2caRegs.I2CMDR.bit.FREE = 0x1;			//Run Free! (during debug break)
   	I2caRegs.I2CMDR.bit.MST = 0x1;			//Master Mode
   	I2caRegs.I2CMDR.bit.TRX = 0x1;			// Transmitter mode.
   	I2caRegs.I2CMDR.bit.XA = 0x0;			//7-bit address
   	I2caRegs.I2CFFTX.bit.I2CFFEN = 0x1; 	//Enable FIFO.
   	I2caRegs.I2CFFTX.bit.TXFFRST = 0x1;		//Enable FIFO transmit.
   	I2caRegs.I2CFFRX.bit.RXFFRST = 0x1;
   	I2caRegs.I2CMDR.bit.IRS = 0x1;			//enable!
   											// Take I2C out of reset
   											// Stop I2C when suspended
  
   I2caRegs.I2CFFTX.bit.TXFFINTCLR = 0x1;	//Reset the interrupt, even though we're not using it.
}
#endif  // end of ECI_API definition


