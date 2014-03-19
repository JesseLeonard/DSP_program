/* DSP Controller Project
 * Energy Conversion and Integration Group
 * Center for Advanced Power Systems
 * Florida State University
 * ******************************************************************************
 * 
 * Filename: main.c
 *  
 * Last Modified: September 1, 2011
 * 	
 * Author: Troy Bevis
 * 
 * ******************************************************************************
 * Purpose:
 *		This file is the main file project file.  It initializes all DSP hardware
 * 		and runs the control code.  All headers needed are referenced from ECI_API.h
 * 		Insert custom control code in the section labeled 'Control Code'.  Functions 
 * 		can be added as needed.  Please use the API functions provided in ECI_API for
 * 		accessing peripheral hardware.
 * *****************************************************************************
 */

//#define rk1b2b
//#define rk2b2b
//#define b2b
#define npc

/*
 * Revisions
 *
 * Program: Closed Loop Inverter Control, designed by Dr. Amin; Sep 27, 2011 
 * 
 * 8/27/2013 - Jesse Leonard
 * Replaced AFE control with decoupled id and iq PI controls found in the PLECS demo
 * "SpaceVectorControl.plecs" although with an inverse dq->abc transform at the end to
 * use a traditional PWM modulator instead of SVM to reduce development time to testing.
 * Also am using the PLL diagram from the same PLECS demo.
 *
 * 9/10/2013 - Jesse Leonard
 * NLDL_revisit_v1
 * Starting from NLDL_revisit_v0 that had a well running AFE closed-loop control with PFC.
 * Now adding closed-loop INV control using a PI loop on vd and vq of the INV output voltage.
 * This revision should be a complete AFE+INV controller (although the INV output voltage
 * PI controller doesn't provide a very stiff voltage for the NPC under diode-rectifier operation
 * so there is some voltage distortion due to the nonlinear load current.
 *
 * 9/23/2013 - Jesse Leonard
 * AFE Testing 1kVA
 * This code is for testing the AFE up to its originally designed specifications of:
 * Output power: 1.3kW
 * Input power: 1.5kVA
 * Output voltage: 200Vdc
 * Output current: 6.5Adc
 * Input voltage: 58V_l-n (100V_l-l)
 * Input current: 8.6A
 * Output capacitance: 760uF
 * Input inductance: 0.64mH
 * 
 * 11/7/2013 - Jesse Leonard
 * AFE Testing 208V input
 * Starting with AFEtesting_1kVA_archive_11-7-2013.zip
 * Changing input voltage scaling and DC voltage scaling
 * Input voltage VTs changed to 3*8.25k = 24.75k resistors
 * DC voltage VT changed to 4*8.25k+2*3k = 39k
 * DC voltage reference changed to 350V
 * disabling INV PWMs for now.
 * va = 0.1705*(GetAIN_B0()-2048);
 * Vdc = 0.2687*(GetAIN_B5()-2048);
 *
 * 12/12/2013 - Jesse Leonard
 * Changed RTDS scaling for 220V/5V RTDS output.
 * (1.5V/2048)*(6.3V/1.5)*(220/5) = 0.1354
 * This scale factor returns the value inside RTDS,
 * in this case a reference voltage.
 *
 * 1/7/2014 - Jesse Leonard
 * 5kVA INV output testing with NHResearch 3ph load
 * RTDS rack 9 in use today, changing to 170Vpk l-n reference, open loop
 *
 * 2/13/2014 - Jesse Leonard
 * Received NHResearch 9200/4960 bidirectional DC load that can sink 10kW.
 * Today I'm working on NLDL rack 2 to bring it up to 208V status using an
 * MTE Corp. 1.2mH input inductor that has a high current rating to limit
 * heating.
 *
 *
 */
 
 


#include <ECI_API.h>
#include <cmath>

#define PI 3.14159

void SetAll_AO(float32 *V)
{
    Uint16 DacVal[4];
    static Uint16 DacAddys[] = { DAC1, DAC2, DAC3, DAC4 };
    static Uint16 DacSettings[] = {0x30, 0x30, 0x30, 0x20};

    int i = 0;

    for(i = 0; i < 4; i++)
    {
        DacVal[i] = (Uint16) (255*V[i])/3;              //Convert for [0 3] to [0 255].
        if(DacVal[i] > 255) DacVal[i] = 255;                //For error checking, Vout = [0 3].

        if(i != 0)
        {
            //This function does not return until the I2C is finished transmitting and the bus is free.
            while(I2caRegs.I2CMDR.bit.STP == 1);
            while(I2caRegs.I2CSTR.bit.BB == 1);
        }
                                        //We will send 3 bytes to DAC.
        //Send first set of bytes
        I2caRegs.I2CDXR = DacAddys[i];                          //We are sending data to DAC1.
        I2caRegs.I2CDXR = (DacVal[i] >> 4) | (DacSettings[i]);  //First byte: [ PD1 | PD0 |CLR_L|LDAC_L| D7 | D6 | D5 | D4 ]
        I2caRegs.I2CDXR = (DacVal[i] & 0xF) << 4;               //Second byte:[ D3 | D2 | D1 | D0 | 0 | 0 | 0 | 0 ]

        I2caRegs.I2CCNT = 3;
        I2caRegs.I2CMDR.bit.TRX = 0x1;                          //Transmit mode.
        I2caRegs.I2CMDR.bit.FREE = 0x1;                         //Free run during breakpoint.
        I2caRegs.I2CMDR.bit.MST = 0x1;                          //Master mode.
        I2caRegs.I2CMDR.bit.STP = 0x1;                          //Stop bit.
        I2caRegs.I2CMDR.bit.STT = 0x1;                          //Start transmission.

        if(i == 0)
        {
            //This function does not return until the I2C is finished transmitting and the bus is free.
            while(I2caRegs.I2CMDR.bit.STP == 1);
            while(I2caRegs.I2CSTR.bit.BB == 1);
        }
    }
}

//////////////////////////////////Beginning of Jesse's added variables 8/27/2013//////////////////////////

volatile float32 T = 0.00005;   //sample time = 1/10k = 0.0001 for 10kHz ISR (and fsw)
int AFEenable = 0;
int INVenable = 0;

//debugging variables
volatile float32 vabuff[167];
volatile float32 vbbuff[167];
volatile float32 vcbuff[167];
volatile float32 vdcbuff[167];
volatile float32 iabuff[167];
volatile float32 ibbuff[167];
volatile float32 icbuff[167];
int buffidx = 0;

//INV voltage references from RTDS
volatile float32 viaref_rtds;
volatile float32 vibref_rtds;
volatile float32 vicref_rtds;

//variables for input
volatile float32 Vdc;
volatile float32 vab, vbc;
volatile float32 ia, ib, ic;
volatile float32 va, vb, vc;
volatile float32 vrd;
volatile float32 vrq;
volatile float32 ird;
volatile float32 irq;

//PLL variables
volatile float32 theta_vin = 0;
volatile float32 theta_vinn1 = 0;
volatile float32 omega_pll = 0;
volatile float32 omega_plln1 = 0;
volatile float32 ki_pll = 500;
volatile float32 kp_pll = 10;
volatile float32 vrqn1 = 0;

//for Vdc PI control, output is idref
volatile float32 Vdcref = 360; // ***********set Vdc reference here**************

volatile float32 irdref = 0;  //input current, i, of rectifier, r, for d axis
volatile float32 irdrefn1 = 0; //idref one delay in the past
volatile float32 e_vdc = 0;
volatile float32 e_vdcn1 = 0;

volatile float32 ki_vdc = 10;
volatile float32 kp_vdc = 0.2;

//for id, iq PI control
volatile float32 irqref = 0.0;  //input current, i, of rectifier, r, for q axis

volatile float32 u_ird = 0;
volatile float32 u_irq = 0;
volatile float32 u_irdn1 = 0;
volatile float32 u_irqn1 = 0;
volatile float32 e_ird = 0;
volatile float32 e_irq = 0;
volatile float32 e_irdn1 = 0;
volatile float32 e_irqn1 = 0;
volatile float32 vrdref = 0;
volatile float32 vrqref = 0;
volatile float32 vraref = 0;
volatile float32 vrbref = 0;
volatile float32 vrcref = 0;

volatile float32 ki_ird = 50;
volatile float32 ki_irq = 50;
volatile float32 kp_ird = 5;
volatile float32 kp_irq = 5;

volatile float32 L=0.0012; //input inductor value for decoupling

volatile float32 dra,drb,drc; //rectifier pwm duty cycles
volatile float32 time = 0;

//////////////////////////////////END OF Jesse's added variables 8/27/2013//////////////////////////

// INV closed loop variables - JPL 9/10/2013
volatile float32 via = 0;
volatile float32 vib = 0;
volatile float32 vic = 0;

volatile float32 theta_vout = 0;
volatile float32 w_inv = 377;

volatile float32 dia = 0;
volatile float32 dib = 0;
volatile float32 dic = 0;
volatile float32 viaref = 0;
volatile float32 vibref = 0;
volatile float32 vicref = 0;
volatile float32 vid = 0;
volatile float32 viq = 0;
volatile float32 vidref = 170;  // OUTPUT VOLTAGE Vd REF FOR INV HERE *******
volatile float32 viqref = 0;
volatile float32 kp_vid = 0.1;
volatile float32 kp_viq = 0.1;
volatile float32 ki_vid = 10;
volatile float32 ki_viq = 10;

volatile float32 u_vid = 0;    //for output voltage PI of INV
volatile float32 u_viq = 0;    //for output voltage PI of INV
volatile float32 u_vidn1 = 0;  //for output voltage PI of INV
volatile float32 u_viqn1 = 0;  //for output voltage PI of INV
volatile float32 e_vid = 0;    //for output voltage PI of INV
volatile float32 e_viq = 0;    //for output voltage PI of INV
volatile float32 e_vidn1 = 0;  //for output voltage PI of INV
volatile float32 e_viqn1 = 0;  //for output voltage PI of INV
//end of variables added 9/10/13





volatile Uint16 t = 0x0;
volatile float32 A = 0;
volatile float32 B = 0;
volatile float32 C = 0;

volatile float32 Ph = 0;
volatile float32 Phn1 = 0;
volatile float32 EPh = 0;
volatile float32 Vp = 0;
volatile float32 UVpn1 = 0;
volatile float32 UVpsn1 = 0;
volatile float32 UVpcn1 = 0;
volatile float32 Vr = 0;
volatile float32 UVrn1 = 0;
volatile float32 EVrn1 = 0;
volatile float32 UrAn = 7440.5;
volatile float32 UrBn = 7440.5;
volatile float32 UrCn = 7440.5;

volatile float32 Vi = 0;
volatile float32 EVi1n1 = 0;
volatile float32 EVi1n2 = 0;
volatile float32 UVi1n1 = 0;
volatile float32 UVi1n2 = 0;
volatile float32 EVi2n1 = 0;
volatile float32 EVi2n2 = 0;
volatile float32 UVi2n1 = 0;
volatile float32 UVi2n2 = 0;
volatile float32 UiAn = 7440.5;
volatile float32 UiBn = 7440.5;
volatile float32 UiCn = 7440.5;
//volatile float32 Vdcref = 70 ;
volatile float32 Vpi = 14.3; //28.57738 ;

/////////////////////////////////////////ISR///////////////////////////////////////////
//Timer interrupt.  The frequency is linked to the PWM 1 interrupt
/////////////////////////////////////////ISR///////////////////////////////////////////

//#ifdef b2b
interrupt void timer_isr(void)
{

	// Clear INT flag for this timer
	EPwm1Regs.ETCLR.bit.INT = 1;

	SetDO_10(); //set output, square wave should be at 5k for 10kHz ISR (toggling is at 10k)





	float32 Vab, Vbc, Va, Vb, Vc, Vr1, Vr2, Ir1, Ir2, EVp, UVp, EVps, UVps, EVpc, UVpc, EVr, UVr, Ur1, Ur2, wd;

	float32 Vi1, Vi2, Ii1, Ii2, Vi2f, Vi1f, EVi1, EVi2, UVi1, UVi2, Ui1, Ui2, Ui3;

//	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
//	PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
	StartADC();

/////////////////////////////////////////REC///////////////////////////////////////////


	////////////////////////////////////////////////////////////////////////
	//input current and DC link voltage measurements
	////////////////////////////////////////////////////////////////////////

	ia = 0.01723*(GetAIN_B2()-2048); //  0.01723 = 1/[1/1000*178.5*0.2382*2048/1.5]
	ib = 0.01723*(GetAIN_B3()-2048); //  0.01723 = 1/[1/1000*178.5*0.2382*2048/1.5]
	ic = 0.01723*(GetAIN_B4()-2048); //  0.01723 = 1/[1/1000*178.5*0.2382*2048/1.5]

	Vdc = 0.2687*(GetAIN_B5()-2048); // 0.2687 = 1/[1/39k*2.5*178.5*0.2382*2048/1.5]

	////////////////////////////////////////////////////////////////////////
	//input voltage L-L --> L-N
	////////////////////////////////////////////////////////////////////////

//	vab = 0.1705*(GetAIN_B0()-2048); //  0.1705 = 1/[1/24.75k*2.5*178.5*0.2382*2048/1.5]
//	vbc = 0.1705*(GetAIN_B1()-2048); //  0.1705 = 1/[1/24.75k*2.5*178.5*0.2382*2048/1.5]

//	va = 0.333333 * ( 2*vab+vbc);
//	vb = 0.333333 * ( vbc-vab);
//	vc = 0.333333 * ( -vab-2*vbc);

	va = 0.1705*(GetAIN_B0()-2048);
	vb = 0.1705*(GetAIN_B1()-2048);
	vc = 0.1705*(GetAIN_B6()-2048);

	////////////////////////////////////////////////////////////////////////
	//PLL (includes abc->dq for input voltages once the phase is locked on)
	////////////////////////////////////////////////////////////////////////
	vrd = 0.666667*(va*cos(theta_vin) + vb*cos(theta_vin-2.0944) + vc*cos(theta_vin+2.0944)) ;
	vrq = 0.666667*(-va*sin(theta_vin) - vb*sin(theta_vin-2.0944) - vc*sin(theta_vin+2.0944)) ;


	//PLECS PLL

	omega_pll = omega_plln1+(ki_pll*T-kp_pll)*vrqn1+kp_pll*vrq; //PI control

	theta_vin = theta_vinn1+omega_plln1*T; //self-resetting integrator for omega to find theta
	if (theta_vin > 6.28319) {theta_vin = theta_vin-6.28319;} //reset integrator at 2pi
	theta_vinn1 = theta_vin; //update delayed variable

	vrqn1 = vrq; //update delayed variable
	//if (omega_pll > 502.0) {omega_pll = 502.0;}
	//if (omega_pll < -502.0) {omega_pll = -502.0;}
	omega_plln1 = omega_pll; //update delayed variable

	////////////////////////////////////////////////////////////////////////
	//abc->dq transform for input current
	////////////////////////////////////////////////////////////////////////
	ird = 0.666667*(ia*cos(theta_vin) + ib*cos(theta_vin-2.0944) + ic*cos(theta_vin+2.0944)) ;
	irq = 0.666667*(-ia*sin(theta_vin) - ib*sin(theta_vin-2.0944) - ic*sin(theta_vin+2.0944)) ;


//if AFE is enabled from CANbus control, perform Vdc, ird, irq PI loops, else reset the loops
if(AFEenable == 1)
{
	////////////////////////////////////////////////////////////////////
	// Vdc PI control
	////////////////////////////////////////////////////////////////////
	e_vdc = Vdcref-Vdc;

	irdref = irdrefn1+(ki_vdc*T-kp_vdc)*e_vdcn1+kp_vdc*e_vdc; //id reference from Vdc PI control

	irdrefn1 = irdref; //update delayed variable
	e_vdcn1 = e_vdc; //update delayed variable

	////////////////////////////////////////////////////////////////////
	// id, iq PI control, note iqref set to 0 in variable declarations
	////////////////////////////////////////////////////////////////////
	//Vd* PI
	e_ird = irdref-ird;  //error = id*-id
	u_ird = u_irdn1+(ki_ird*T-kp_ird)*e_irdn1+kp_ird*e_ird; //PI control
	vrdref = u_ird-irq*2*PI*60*L; //add decoupling term
	vrdref = vrd-vrdref;

	e_irdn1 = e_ird; //update delayed variable
	u_irdn1 = u_ird; //update delayed variable


	//Vq* PI
	e_irq = irqref-irq; //error = iq*-iq
	u_irq = u_irqn1+(ki_irq*T-kp_irq)*e_irqn1+kp_irq*e_irq; //PI control
	vrqref = u_irq+ird*2*PI*60*L; //add decoupling term
	vrqref = vrq-vrqref;

	e_irqn1 = e_irq; //update delayed variable
	u_irqn1 = u_irq; //update delayed variable
}
else
{
	e_vdc = 0;
	irdref = 0;
	irdrefn1 = 0;
	e_vdcn1 = 0;

	e_ird = 0;
	u_ird = 0;
	vrdref = 0;
	e_irdn1 = 0;
	u_irdn1 = 0;

	e_irq = 0;
	u_irq = 0;
	vrqref = 0;
	e_irqn1 = 0;
	u_irqn1 = 0;
}


	////////////////////////////////////////////////////////////////////////
	//dq->abc inverse transform for vd, vq references
	////////////////////////////////////////////////////////////////////////
	vraref = vrdref*cos(theta_vin) - vrqref*sin(theta_vin);
	vrbref = vrdref*cos(theta_vin-2.0944) - vrqref*sin(theta_vin-2.0944);
	vrcref = vrdref*cos(theta_vin+2.0944) - vrqref*sin(theta_vin+2.0944);

	// TEST CODE FOR BENCHTOP TESTING OF UPDOWN PWM
//	Vdc = 200;
//	vraref = 75*cos(theta_vout);
//	vrbref = 75*cos(theta_vout-2.0944);
//	vrcref = 75*cos(theta_vout+2.0944);


	//PWM
	dra = 0.5*(vraref/(Vdc/2))+0.5; //scale by Vdc then shrink+shift for [-1 1] modulation to [0 1]
	drb = 0.5*(vrbref/(Vdc/2))+0.5; //scale by Vdc then shrink+shift for [-1 1] modulation to [0 1]
	drc = 0.5*(vrcref/(Vdc/2))+0.5; //scale by Vdc then shrink+shift for [-1 1] modulation to [0 1]

	//set PWM duty out
	SetPWM_Rau(dra*PWM_PD);  //dra is [0 1], i.e. percentage of PWM_PD, the clock cycles of PWM period
	SetPWM_Rbu(drb*PWM_PD);  //dra is [0 1], i.e. percentage of PWM_PD, the clock cycles of PWM period
	SetPWM_Rcu(drc*PWM_PD);  //dra is [0 1], i.e. percentage of PWM_PD, the clock cycles of PWM period

/////////////////////////////////////////END OF REC CODE///////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////
//                                  INV
///////////////////////////////////////////////////////////////////////////////////////

	// 9/10/13
	// Inverter controller changed to PI for vd and vq of INV output - Jesse 9/10/13

	////////////////////////////////////////////////////////////////////////
	//RTDS voltage references
	////////////////////////////////////////////////////////////////////////
	viaref_rtds = 0.1354*(GetAIN_B7()-2048);  //scale factor depends on scaling for GTAO too
	vibref_rtds = 0.1354*(GetAIN_A5()-2048);  //scale factor depends on scaling for GTAO too
	vicref_rtds = 0.1354*(GetAIN_A7()-2048);  //scale factor depends on scaling for GTAO too


	theta_vout = theta_vout + w_inv*T;
	//t_inv = t_inv + T;
	if (theta_vout > 6.28319)
		{theta_vout = theta_vout - 6.28319;
	//	 t_inv = 0;
		 }

	////////////////////////////////////////////////////////////////////////
	//output voltage measurement across LC filter capacitors
	////////////////////////////////////////////////////////////////////////
	via = 0.1705*(GetAIN_A0()-2048); // 0.1705 = 1/[1/24.75k*2.5*178.5*0.2382*2048/1.5]
	vib = 0.1705*(GetAIN_A1()-2048); // 0.1705 = 1/[1/24.75k*2.5*178.5*0.2382*2048/1.5]
	vic = 0.1705*(GetAIN_A6()-2048); // 0.1705 = 1/[1/24.75k*2.5*178.5*0.2382*2048/1.5]

	////////////////////////////////////////////////////////////////////////
	//measured voltage abc-->dq
	////////////////////////////////////////////////////////////////////////
	vid = 0.666667*(via*cos(theta_vout) + vib*cos(theta_vout-2.0944) + vic*cos(theta_vout+2.0944)) ;
	viq = 0.666667*(-via*sin(theta_vout) - vib*sin(theta_vout-2.0944) - vic*sin(theta_vout+2.0944)) ;


//if INV is enabled from CANbus control, perform Vd, Vq PI loops, else reset the loops
if(INVenable == 1)
{
	////////////////////////////////////////////////////////////////////////
	//ramp INV output voltage
	////////////////////////////////////////////////////////////////////////
	if(vidref<170)
	{vidref = vidref + 0.00283;}
	else
	{vidref = 170;}

	////////////////////////////////////////////////////////////////////////
	//output voltage dq PI loops
	////////////////////////////////////////////////////////////////////////
	//Vd* PI
	e_vid = vidref-vid;  //error = id*-id
	u_vid = u_vidn1+(ki_vid*T-kp_vid)*e_vidn1+kp_vid*e_vid; //PI control

	e_vidn1 = e_vid; //update delayed variable
	u_vidn1 = u_vid; //update delayed variable


	//Vq* PI
	e_viq = viqref-viq; //error = iq*-iq
	u_viq = u_viqn1+(ki_viq*T-kp_viq)*e_viqn1+kp_viq*e_viq; //PI control

	e_viqn1 = e_viq; //update delayed variable
	u_viqn1 = u_viq; //update delayed variable
}
else
{
	//for ramp
	vidref = 10;

	e_vid = 0;
	u_vid = 0;
	e_vidn1 = 0;
	u_vidn1 = 0;

	e_viq = 0;
	u_viq = 0;
	e_viqn1 = 0;
	u_viqn1 = 0;
}

	////////////////////////////////////////////////////////////////////////
	//dq->abc inverse transform for vd, vq references
	////////////////////////////////////////////////////////////////////////
	/* closed loop references */
	viaref = u_vid*cos(theta_vout) - u_viq*sin(theta_vout);
	vibref = u_vid*cos(theta_vout-2.0944) - u_viq*sin(theta_vout-2.0944);
	vicref = u_vid*cos(theta_vout+2.0944) - u_viq*sin(theta_vout+2.0944);

	/* open loop references */
//	viaref = 170*cos(theta_vout);// - viqref*sin(theta_vout);
//	vibref = 170*cos(theta_vout-2.0944);// - viqref*sin(theta_vout-2.0944);
//	vicref = 170*cos(theta_vout+2.0944);// - viqref*sin(theta_vout+2.0944);

	/* rtds open loop references */
//	viaref = viaref_rtds;
//	vibref = vibref_rtds;
//	vicref = vicref_rtds;

	//PWM
	dia = 0.5*(viaref/(Vdc/2))+0.5; //scale by Vdc then shrink+shift for [-1 1] modulation to [0 1]
	dib = 0.5*(vibref/(Vdc/2))+0.5; //scale by Vdc then shrink+shift for [-1 1] modulation to [0 1]
	dic = 0.5*(vicref/(Vdc/2))+0.5; //scale by Vdc then shrink+shift for [-1 1] modulation to [0 1]


	//set PWM duty out
	SetPWM_Iau(dia*PWM_PD);  //dia is [0 1], i.e. percentage of PWM_PD, the clock cycles of PWM period
	SetPWM_Ibu(dib*PWM_PD);  //dia is [0 1], i.e. percentage of PWM_PD, the clock cycles of PWM period
	SetPWM_Icu(dic*PWM_PD);  //dia is [0 1], i.e. percentage of PWM_PD, the clock cycles of PWM period

/////////////////////////////////////////END OF INV CODE///////////////////////////////////////////





	//debugging, storage buffers to view in CodeComposer debugger graphs

	vabuff[buffidx] = va; //GetAIN_A0()-2048;
	vbbuff[buffidx] = vb; //GetAIN_A1()-2048;
	vcbuff[buffidx] = vc; //GetAIN_A6()-2048;
	vdcbuff[buffidx] = Vdc;
	iabuff[buffidx] = ia;
	ibbuff[buffidx] = ib;
	icbuff[buffidx] = ic;

	buffidx++;
	if(buffidx > 167) buffidx = 0;

	// Acknowledge this interrupt to receive more interrupts from group 3
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;

	ClearDO_10(); //clear output, square wave should be at 5k for 10kHz ISR (toggling is at 10k)
	return;
}
//#endif



/////////////////////////////////////////ISR///////////////////////////////////////////
//Timer interrupt.  The frequency is linked to the PWM 1 interrupt
/////////////////////////////////////////ISR///////////////////////////////////////////

void main(void)
{

	float32 V[4] = {0, 0, 0, 0};
	DSP_init();
//	EnablePWM_I();
//	EnablePWM_R();
	struct ECAN_REGS ECanaShadow;
    InitECanGpio();
    InitECan();

    //Setup mailbox 1 for AFE PWM enable Message ID = 0x10000000
    //read byte 0 for 1 or 0
#ifdef rk1b2b
    ECanaMboxes.MBOX1.MSGID.all = 0x10000000; // message Identifier
	ECanaMboxes.MBOX1.MSGID.bit.IDE = 1; //extended identifier
	//set mailbox as receive
	ECanaShadow.CANMD.all = ECanaRegs.CANMD.all;
	ECanaShadow.CANMD.bit.MD1 = 1;  //mailbox direction to receive
	ECanaRegs.CANMD.all = ECanaShadow.CANMD.all;
	//enable mailbox
	ECanaShadow.CANME.all = ECanaRegs.CANME.all;
	ECanaShadow.CANME.bit.ME1 = 1;
	ECanaRegs.CANME.all = ECanaShadow.CANME.all;

    //Setup mailbox 2 for INV PWM enable Message ID = 0x10000001
    //read byte 0 for 1 or 0
	ECanaMboxes.MBOX2.MSGID.all = 0x10000001; // message Identifier
	ECanaMboxes.MBOX2.MSGID.bit.IDE = 1; //extended identifier
	//set mailbox as receive
	ECanaShadow.CANMD.all = ECanaRegs.CANMD.all;
	ECanaShadow.CANMD.bit.MD2 = 1;  //mailbox direction to receive
	ECanaRegs.CANMD.all = ECanaShadow.CANMD.all;
	//enable mailbox
	ECanaShadow.CANME.all = ECanaRegs.CANME.all;
	ECanaShadow.CANME.bit.ME2 = 1;
	ECanaRegs.CANME.all = ECanaShadow.CANME.all;
#endif

	//change message IDs for rack 2 b2b PWM enables
#ifdef rk2b2b
    ECanaMboxes.MBOX1.MSGID.all = 0x10000002; // message Identifier
	ECanaMboxes.MBOX1.MSGID.bit.IDE = 1; //extended identifier
	//set mailbox as receive
	ECanaShadow.CANMD.all = ECanaRegs.CANMD.all;
	ECanaShadow.CANMD.bit.MD1 = 1;  //mailbox direction to receive
	ECanaRegs.CANMD.all = ECanaShadow.CANMD.all;
	//enable mailbox
	ECanaShadow.CANME.all = ECanaRegs.CANME.all;
	ECanaShadow.CANME.bit.ME1 = 1;
	ECanaRegs.CANME.all = ECanaShadow.CANME.all;

    //Setup mailbox 2 for INV PWM enable Message ID = 0x10000001
    //read byte 0 for 1 or 0
	ECanaMboxes.MBOX2.MSGID.all = 0x10000003; // message Identifier
	ECanaMboxes.MBOX2.MSGID.bit.IDE = 1; //extended identifier
	//set mailbox as receive
	ECanaShadow.CANMD.all = ECanaRegs.CANMD.all;
	ECanaShadow.CANMD.bit.MD2 = 1;  //mailbox direction to receive
	ECanaRegs.CANMD.all = ECanaShadow.CANMD.all;
	//enable mailbox
	ECanaShadow.CANME.all = ECanaRegs.CANME.all;
	ECanaShadow.CANME.bit.ME2 = 1;
	ECanaRegs.CANME.all = ECanaShadow.CANME.all;
#endif

	StartTimer();
	while(1)
	{

#ifdef b2b
		////////////////////////////////////////////
		//AFE enable signal from CANbus
		////////////////////////////////////////////
		if(ECanaRegs.CANRMP.bit.RMP1 == 1)  //valid new data in MBX1?
		{
			AFEenable = ECanaMboxes.MBOX1.MDL.byte.BYTE0; //read message
			ECanaShadow.CANRMP.bit.RMP1 = 1;  //clear status flag RMP1
			ECanaRegs.CANME.all = ECanaShadow.CANME.all;
		}

		if(AFEenable == 1)
			{EnablePWM_R();}
		else
			{DisablePWM_R();}

		////////////////////////////////////////////
		//INV enable signal from CANbus
		////////////////////////////////////////////
		if(ECanaRegs.CANRMP.bit.RMP2 == 1)  //valid new data in MBX2?
		{
			INVenable = ECanaMboxes.MBOX2.MDL.byte.BYTE0; //read message
			ECanaShadow.CANRMP.bit.RMP2 = 1;  //clear status flag RMP1
			ECanaRegs.CANME.all = ECanaShadow.CANME.all;
		}

		if(INVenable == 1)
			{EnablePWM_I();}
		else
			{DisablePWM_I();}

		//DAC outputs for b2b converters

		V[0] = 0;
		V[1] = GetAIN_A0()*0.00073242 ; //(3/4096)/0.051703046561388 ; //VaINV
		V[2] = GetAIN_A1()*0.00073242 ; //(3/4096)/0.011693505697301 ; //VbINV
		V[3] = GetAIN_A6()*0.00073242 ; //(3/4096)/0.124087311747332 ; //VcINV

		SetAll_AO(V);

		V[0] = 3;
		V[1] = GetAIN_A2()*0.00073242 ; //(3/4096)/0.051703046561388 ; //IoINVa  0.0931
		V[2] = GetAIN_A3()*0.00073242 ; //(3/4096)/0.008617174426898 ; //IoINVb  0.006153
		V[3] = GetAIN_A4()*0.00073242 ; //(3/4096)/0.006152662540805 ; //IoINVc  0.006153

		SetAll_AO(V);
#endif

		//DAC outputs for NPC
#ifdef npc
		V[0] = 0;
		V[1] = GetAIN_B0()*0.00073242 ; //(3/4096)/0.051703046561388 ; //VabNPC
		V[2] = GetAIN_B2()*0.00073242 ; //(3/4096)/0.011693505697301 ; //IaNPC
		V[3] = GetAIN_A0()*0.00073242 ; //(3/4096)/0.124087311747332 ; //Vdc1NPC

		SetAll_AO(V);

		V[0] = 3;
		V[1] = GetAIN_A1()*0.00073242 ; //(3/4096)/0.051703046561388 ; //Vdc2  0.0931
		V[2] = GetAIN_A2()*0.00073242 ; //(3/4096)/0.008617174426898 ; //Io+NPC  0.006153
		V[3] = GetAIN_A3()*0.00073242 ; //(3/4096)/0.006152662540805 ; //Io-NPC  0.006153

		SetAll_AO(V);
#endif

	}
}						





