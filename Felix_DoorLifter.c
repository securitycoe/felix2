//****************************************************************************
// @Module        Moror Config
// @Filename      
// @Project       common
//----------------------------------------------------------------------------
// @Controller    Infineon XE167F
//
// @Compiler      Tasking
//
// 
//
// @Description   This file contains functions and variables 
//                for controlling z-axis.
//
//	Besonderheit: beim Anfahren der Null-Lage wird eine mechanische Lose
//	berücksichtigt so dass definiert weiter in die Null-Lage
//	eingetaucht wird, bevor der Antrieb abbremst und die Null-Lage
//	verlässt (NUll-Punkt gefunden)
//----------------------------------------------------------------------------
// @Date          
//
//****************************************************************************

//****************************************************************************
// include files
//****************************************************************************
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <ctype.h>
#include <math.h>

#include "DevError.h"
#include "main.h"
#include "DeviceConfig.h"
#include "Drv_config.h"
#include "MDriveCmd.h"
#include "MDrive_CmdCtrl.h"
#include "Felix_DoorLifter.h"
#include "Scan.h"
#include "MyScanner.h"
#include "MotorExe.h"
#include "Felix_FRAM.h"
//****************************************************************************
// @Macros
//****************************************************************************

//****************************************************************************
// local definitions
//****************************************************************************


#ifdef _CF_FELIX
//neue Getriebeübersetzung, für Doorlifter mit RS232 und Steuerung durch den Felix
#define DOORLIFT_WAY_PRO_REV 46.15724591F	 // Getreibe Motor Z1 = 10 Zähne, Gerätewelle Z2 = 26 Zähne
											// Riemenantrieb: Radius R3 = 19.10mm, Z3 = 40 Zähne
											// way_pro_rev = 2*PI *R3 *Z1/Z2

//alt  Getriebeübersetzung, für Doorlifter mit RS485 und Steuerung durch Composer-Plugin
//#define DOORLIFT_WAY_PRO_REV 73.8440F 	//lt. Konstruktion 73.85159346
											// Getreibe Motor Z1 = 16 Zähne, Gerätewelle Z2 = 26 Zähne
											// Riemenantrieb: Radius R3 = 19.10mm, Z3 = 40 Zähne
											// way_pro_rev = 2*PI *R3 *Z1/Z2

#define DOORLIFT_ENC_IMP_PRO_REV 2048.0F
#define DOORLIFT_MOT_IMP_PRO_REV 2048.0F
#endif


#define DOOR_HOMING_RESET_STATE 0
#define DOOR_HOMING_RUN_STATE 1
#define DOOR_HOMING_OFFSET_STATE 2

#define DOOR_MOVE_RESET_STATE 0
#define DOOR_MOVE_OPEN_STATE 1
#define DOOR_MOVE_CLOSED_STATE 2

//****************************************************************************
// default settings
//****************************************************************************

//****************************************************************************
// external functions
//****************************************************************************
extern unsigned short Cmd_ClearEstop(unsigned short NoRestore);
extern unsigned short CheckDriveHomingOk(unsigned short mask);
//****************************************************************************
// external data
//****************************************************************************
extern char *g_pcCmdBuf, g_acAnswerBuf[];
extern IMS_CommandBlock Msg;
//****************************************************************************
// public data
//****************************************************************************
#ifdef _CF_FELIX
const udtIMSDoorLiftAxisFixedParameter FixDoorLifter = {
	(unsigned short) AXIS_ID_DOORLIFT_DRIVE
	,(char) AXIS_ADR_DOORLIFT_DRIVE		//Adress;
	,(unsigned short) eDoorLift_Axis			// MaskenBit
	,(float) -5.0				//Offset
	,(float) 0.0				// SetHomingPosition
	// Geschwindigkeiten
	,(float) 1.45			 	// initial speed [mm/s]
	,(float) 300.0			 	// Endspeed positive direction [mm/s]
	,(float) 1.45			 	// initial speed homing [mm/s]
	,(float) 10.82 			 	// Endspeed homing [mm/s]
	,(float) 500.0				// Acceleration positive direction [mm/s*s]
	,(float) 18.03				// Acceleration homing [mm/s*s]


	// Grenzwerte
	,(float) 1.0				// min. Acceleration [mm/s*s]
	,(float) 1000.0				// max. Acceleration [mm/s*s]
	,(float) 1.0				// min. Endspeed [mm/s*s]
	,(float) 500.0				// max. Endspeed [mm/s*s]
	,(float) 0.0	//-5.0		// min. drive range [mm]
	,(float) 350.0				// max. drive range [mm]

	// sonstiges
	,(float) 0.1				// Lose (mech. Spiel) [mm]
	,(float) 0.2//0.01//0.03				// Genauigkeit der Positionierung(Deadband) [mm]
	,(unsigned short) 30		// 60 zulässige Diff. Steps zu Encoder [encoder_step]
	,(unsigned short) 500		// hold current wait time [millisecounds]
	,(unsigned short) 50			// wobbling wait time [millisecounds]

	// Strom
	,(unsigned short) 100		//  run current [percent]
	,(unsigned short) 80		// hold current [percent]

	// Übersetzung
	,(float) DOORLIFT_WAY_PRO_REV	// Weg pro Umdrehung in mm
	,(float) DOORLIFT_ENC_IMP_PRO_REV	// EncoderAuflösung	0 -> Achse ohne Encoder
	,(float) (DOORLIFT_MOT_IMP_PRO_REV / DOORLIFT_WAY_PRO_REV)	// Motor-Impulse (µ-Step) pro mm	bzw. pro Umdr.
	,(float) (DOORLIFT_ENC_IMP_PRO_REV / DOORLIFT_WAY_PRO_REV)	// Encoder-Impulse pro mm
	,(unsigned short) HM_SLEW_NEGATIVE_CREEP_POSITIVE	// normales homing
	,(unsigned short) HM_SLEW_POSITIVE_CREEP_NEGATIVE	// inverses homig
	//open , close
	,(float) 0.0				// position door closed [mm]
	,(float) 340.0				// position door open [mm]

};

#endif

#ifdef _CF_FELIX
/*udtIMSAxisChangedParameter FrontCarriage;
udtIMSAxisChangedParameter *pIMSChangedPara[MAX_AXES];
pIMSChangedPara[1] = &FrontCarriage;*/

//const udtIMSAxisFixedParameter *pIMSDoorLifFixedPara;

udtIMSDoorLiftAxisChangedParameter ChangedDoorLifter;
//udtIMSDoorLiftAxisChangedParameter *pIMSDoorLifChangedPara;

udtIMSDoorLiftAxes g_DoorLiftAxes;
udtIMSDoorLiftConfig g_DoorLiftConfig;
#endif

//****************************************************************************
// private data
//****************************************************************************
unsigned short DoorAdjustState = DOOR_HOMING_RESET_STATE;
unsigned short DoorMoveState = DOOR_MOVE_RESET_STATE;
//****************************************************************************
// private functions
//****************************************************************************
//****************************************************************************
// public functions
//****************************************************************************
void DorrLift_U1C0_ASC_vInit(void)
{
  // USER CODE BEGIN (Init,2)

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U1C0 Fractional Divider:
  ///  -----------------------------------------------------------------------
  ///  - The Fractional divider is selected
  ///  - The step value STEP = 477

  U1C0_FDRL      =  0x81DD;      // load U1C0 fractional divider register

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U1C0 Protocol Control Register:
  ///  -----------------------------------------------------------------------
  ///  - Sample Mode (SMD) = 1
  ///  - 1 Stop bit is selected
  ///  - Sample Point (SP) = 7
  ///  - Pulse Length (PL) = 0

  U1C0_PCRL      =  0x0701;      // load U1C0 protocol control register L
  U1C0_PCRH      =  0x0000;      // load U1C0 protocol control register H


// ACHTUNG DAVE setzt U1C0_SCTRL und U1C0_PCRL auf Werte das die Schnittstelle nicht mehr funktioniert
// hier sind die richtigen Werte
//	U1C0_SCTRL     =  0x0102;
//	U1C0_PCRL      =  0x0903;
/*
  // USER CODE END

  ///  -----------------------------------------------------------------------
  ///  U1C0 ASC is configured for Full-Duplex Operation Mode:
  ///  -----------------------------------------------------------------------

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U1C0 Fractional Divider:
  ///  -----------------------------------------------------------------------
  ///  - The Fractional divider is selected
  ///  - The step value STEP = 477

  U1C0_FDRL      =  0x81DD;      // load U1C0 fractional divider register

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U1C0 Baudrate Generator:
  ///  -----------------------------------------------------------------------
  ///  - The selected BaudRate is 9,600 kbaud
  ///  - The PreDivider for CTQ, PCTQ = 1
  ///  - The Denominator for CTQ, DCTQ = 15
  ///  - The Divider factor PDIV = 99

  U1C0_BRGL      =  0x3D00;      // load U1C0 load baud rate generator 
                                 // register L

  U1C0_BRGH      =  0x0063;      // load U1C0 load baud rate generator 
                                 // register H

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U1C0 Input Control Register 0:
  ///  -----------------------------------------------------------------------
  ///  - The data input DX0D is selected for pin - P10.13

  U1C0_DX0CR     =  0x0003;      // load U1C0 input control register(Data)

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U1C0 Interrupt Node Pointer Register:
  ///  -----------------------------------------------------------------------
  ///  - AINP pointer points to Interrupt node U1C0_0IC
  ///  - RINP pointer points to Interrupt node U1C0_0IC

  U1C0_INPRL     =  0x0000;      // load U1C0 Interrupt Node Pointer register 
                                 // L
  U1C0_INPRH     =  0x0000;      // load U1C0 Interrupt Node Pointer register 
                                 // H

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U1C0 Shift Control:
  ///  -----------------------------------------------------------------------
  ///  - Transmit/Receive MSB first is selected
  ///  - Transmission Mode (TRM) = 1
  ///  - Passive Data Level (PDL) = 1

  U1C0_SCTRL     =  0x0102;      // load U1C0 shift control register L

  ///  - Word Length (WLE) = 7
  ///  - Frame Length (FLE) = 7

  U1C0_SCTRH     =  0x0707;      // load U1C0 shift control register H

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U1C0 Transmit Control/Status Register:
  ///  -----------------------------------------------------------------------
  ///  - TBUF Data Enable (TDEN) = 1
  ///  - TBUF Data Single Shot Mode (TDSSM) = 1

  U1C0_TCSRL     =  0x0500;      // load U1C0 transmit control/status 
                                 // register L
  U1C0_TCSRH     =  0x0000;      // load U1C0 transmit control/status 
                                 // register H

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U1C0 Protocol Control Register:
  ///  -----------------------------------------------------------------------
  ///  - Sample Mode (SMD) = 1
  ///  - 1 Stop bit is selected
  ///  - Sample Point (SP) = 7
  ///  - Pulse Length (PL) = 0

  U1C0_PCRL      =  0x0705;      // load U1C0 protocol control register L
  U1C0_PCRH      =  0x0000;      // load U1C0 protocol control register H

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U1C0 Protocol Status Register:
  ///  -----------------------------------------------------------------------
  ///  - TBIF is set to simplify polling

  U1C0_PSR      |=  0x2000;      // load U1C0 protocol status register


  ///  -----------------------------------------------------------------------
  ///  Configuration of the used U1C0 Interrupts:
  ///  -----------------------------------------------------------------------
  ///  U1C00IC service request node configuration:
  ///  - U1C00IC interrupt priority level (ILVL) = 8
  ///  - U1C00IC interrupt group level (GLVL) = 0
  ///  - U1C00IC group priority extension (GPX) = 0

  U1C0_0IC       =  0x0060;     

  ///  U1C03IC service request node configuration:
  ///  - U1C03IC interrupt priority level (ILVL) = 3
  ///  - U1C03IC interrupt group level (GLVL) = 1
  ///  - U1C03IC group priority extension (GPX) = 0

  CC2_CC22IC     =  0x004D;     

  SCU_ISSR      |=  0x0300;      // load Interrupt Source Select Register

  ///  -----------------------------------------------------------------------
  ///  Configuration of U1C0 FIFO:
  ///  -----------------------------------------------------------------------
  ///  -----------------------------------------------------------------------
  ///  Configuration of U1C0 Transmitter Buffer Conrol Register:
  ///  -----------------------------------------------------------------------
  ///  - Transmit FIFO buffer contains 16 entries
  ///  - Transmit FIFO buffer starts at Data Pointer 32
  ///  - Limit for transmit FIFO interrupt generation is 0

  U1C0_TBCTRL    =  0x0020;      // load U1C0 transmitter buffer control 
                                 // register L
  U1C0_TBCTRH    =  0x0400;      // load U1C0 transmitter buffer control 
                                 // register H

  ///  -----------------------------------------------------------------------
  ///  Configuration of U1C0 Receiver Buffer Conrol Register:
  ///  -----------------------------------------------------------------------
  ///  - Receive FIFO buffer contains 16 entries
  ///  - Receive FIFO buffer starts at Data Pointer 48
  ///  - Limit for receive FIFO interrupt generation is 0
  ///  - Filling level mode is selected
  ///  - Standard Receive buffer interrupt is enabled

  U1C0_RBCTRL    =  0x0030;      // load U1C0 receive buffer control register 
                                 // L
  U1C0_RBCTRH    =  0x4400;      // load U1C0 receive buffer control register 
                                 // H

  ///  -----------------------------------------------------------------------
  ///  Configuration of the used U1C0 Port Pins:
  ///  -----------------------------------------------------------------------
  P10_IOCR13     =  0x0020;      // load port register for data input

  ///  - P10.12 is used for USIC1Channel0 Shift Data output(DOUT)

  P10_IOCR12 = 0x0090;    //set direction register



  // USER CODE BEGIN (U1C0_Function,3)
// ACHTUNG DAVE setzt U1C0_SCTRL und U1C0_PCRL auf Werte das die Schnittstelle nicht mehr funktioniert
// hier sind die richtigen Werte
//	U1C0_SCTRL     =  0x0102;
//	U1C0_PCRL      =  0x0903;
*/
  U1C0_CCR       =  0xC002;      // load U1C0 channel control register
  // USER CODE END


} //  End of function DorrLift_U1C0_ASC_vInit

void Slave_U1C0_ASC_vInit(void)
{
  // USER CODE BEGIN (Init,2)
  ///  -----------------------------------------------------------------------
  ///  Configuration of the U1C0 Fractional Divider:
  ///  -----------------------------------------------------------------------
  ///  - The Fractional divider is selected
  ///  - The step value STEP = 953

  U1C0_FDRL      =  0x83B9;      // load U1C0 fractional divider register

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U1C0 Protocol Control Register:
  ///  -----------------------------------------------------------------------
  ///  - Sample Mode (SMD) = 1
  ///  - 2 Stop bits are selected
  ///  - Sample Point (SP) = 9
  ///  - Pulse Length (PL) = 0

  U1C0_PCRL      =  0x0903;      // load U1C0 protocol control register L
  U1C0_PCRH      =  0x0000;      // load U1C0 protocol control register H


// ACHTUNG DAVE setzt U1C0_SCTRL und U1C0_PCRL auf Werte das die Schnittstelle nicht mehr funktioniert
// hier sind die richtigen Werte
//	U1C0_SCTRL     =  0x0102;
//	U1C0_PCRL      =  0x0903;

  // USER CODE END
/*
  ///  U1C0 ASC is configured for Full-Duplex Operation Mode:
  ///  -----------------------------------------------------------------------

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U1C0 Fractional Divider:
  ///  -----------------------------------------------------------------------
  ///  - The Fractional divider is selected
  ///  - The step value STEP = 953

  U1C0_FDRL      =  0x83B9;      // load U1C0 fractional divider register

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U1C0 Baudrate Generator:
  ///  -----------------------------------------------------------------------
  ///  - The selected BaudRate is 19,200 kbaud
  ///  - The PreDivider for CTQ, PCTQ = 1
  ///  - The Denominator for CTQ, DCTQ = 15
  ///  - The Divider factor PDIV = 99

  U1C0_BRGL      =  0x3D00;      // load U1C0 load baud rate generator 
                                 // register L

  U1C0_BRGH      =  0x0063;      // load U1C0 load baud rate generator 
                                 // register H

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U1C0 Input Control Register 0:
  ///  -----------------------------------------------------------------------
  ///  - The data input DX0D is selected for pin - P10.13

  U1C0_DX0CR     =  0x0003;      // load U1C0 input control register(Data)

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U1C0 Interrupt Node Pointer Register:
  ///  -----------------------------------------------------------------------
  ///  - AINP pointer points to Interrupt node U1C0_0IC
  ///  - RINP pointer points to Interrupt node U1C0_0IC

  U1C0_INPRL     =  0x0000;      // load U1C0 Interrupt Node Pointer register 
                                 // L
  U1C0_INPRH     =  0x0000;      // load U1C0 Interrupt Node Pointer register 
                                 // H

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U1C0 Shift Control:
  ///  -----------------------------------------------------------------------
  ///  - Transmit/Receive MSB first is selected
  ///  - Transmission Mode (TRM) = 1
  ///  - Passive Data Level (PDL) = 1

  U1C0_SCTRL     =  0x0102;      // load U1C0 shift control register L

  ///  - Word Length (WLE) = 7
  ///  - Frame Length (FLE) = 7

  U1C0_SCTRH     =  0x0707;      // load U1C0 shift control register H

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U1C0 Transmit Control/Status Register:
  ///  -----------------------------------------------------------------------
  ///  - TBUF Data Enable (TDEN) = 1
  ///  - TBUF Data Single Shot Mode (TDSSM) = 1

  U1C0_TCSRL     =  0x0500;      // load U1C0 transmit control/status 
                                 // register L
  U1C0_TCSRH     =  0x0000;      // load U1C0 transmit control/status 
                                 // register H

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U1C0 Protocol Control Register:
  ///  -----------------------------------------------------------------------
  ///  - Sample Mode (SMD) = 1
  ///  - 2 Stop bits are selected
  ///  - Sample Point (SP) = 9
  ///  - Pulse Length (PL) = 0

  U1C0_PCRL      =  0x0903;      // load U1C0 protocol control register L
  U1C0_PCRH      =  0x0000;      // load U1C0 protocol control register H

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U1C0 Protocol Status Register:
  ///  -----------------------------------------------------------------------
  ///  - TBIF is set to simplify polling

  U1C0_PSR      |=  0x2000;      // load U1C0 protocol status register


  ///  -----------------------------------------------------------------------
  ///  Configuration of the used U1C0 Interrupts:
  ///  -----------------------------------------------------------------------
  ///  U1C00IC service request node configuration:
  ///  - U1C00IC interrupt priority level (ILVL) = 8
  ///  - U1C00IC interrupt group level (GLVL) = 0
  ///  - U1C00IC group priority extension (GPX) = 0

  U1C0_0IC       =  0x0060;     

  ///  U1C03IC service request node configuration:
  ///  - U1C03IC interrupt priority level (ILVL) = 3
  ///  - U1C03IC interrupt group level (GLVL) = 1
  ///  - U1C03IC group priority extension (GPX) = 0

  CC2_CC22IC     =  0x004D;     

  SCU_ISSR      |=  0x0300;      // load Interrupt Source Select Register

  ///  -----------------------------------------------------------------------
  ///  Configuration of U1C0 FIFO:
  ///  -----------------------------------------------------------------------
  ///  -----------------------------------------------------------------------
  ///  Configuration of U1C0 Transmitter Buffer Conrol Register:
  ///  -----------------------------------------------------------------------
  ///  - Transmit FIFO buffer contains 16 entries
  ///  - Transmit FIFO buffer starts at Data Pointer 32
  ///  - Limit for transmit FIFO interrupt generation is 0

  U1C0_TBCTRL    =  0x0020;      // load U1C0 transmitter buffer control 
                                 // register L
  U1C0_TBCTRH    =  0x0400;      // load U1C0 transmitter buffer control 
                                 // register H

  ///  -----------------------------------------------------------------------
  ///  Configuration of U1C0 Receiver Buffer Conrol Register:
  ///  -----------------------------------------------------------------------
  ///  - Receive FIFO buffer contains 16 entries
  ///  - Receive FIFO buffer starts at Data Pointer 48
  ///  - Limit for receive FIFO interrupt generation is 0
  ///  - Filling level mode is selected
  ///  - Standard Receive buffer interrupt is enabled

  U1C0_RBCTRL    =  0x0030;      // load U1C0 receive buffer control register 
                                 // L
  U1C0_RBCTRH    =  0x4400;      // load U1C0 receive buffer control register 
                                 // H

  ///  -----------------------------------------------------------------------
  ///  Configuration of the used U1C0 Port Pins:
  ///  -----------------------------------------------------------------------
  P10_IOCR13     =  0x0020;      // load port register for data input

  ///  - P10.12 is used for USIC1Channel0 Shift Data output(DOUT)

  P10_IOCR12 = 0x0090;    //set direction register


*/
  // USER CODE BEGIN (U1C0_Function,3)
// ACHTUNG DAVE setzt U1C0_SCTRL und U1C0_PCRL auf Werte das die Schnittstelle nicht mehr funktioniert
// hier sind die richtigen Werte
//	U1C0_SCTRL     =  0x0102;
//	U1C0_PCRL      =  0x0903;

  // USER CODE END
}
void DoorLiftGenerate(void)
{
	g_DoorLiftAxes.AxisID = FixDoorLifter.ID;
	g_DoorLiftAxes.AxisAdr = FixDoorLifter.BusAdr;
	g_DoorLiftAxes.AxisBit = FixDoorLifter.AxisBit;
	g_DoorLiftAxes.ParaIDX = 0;
//	g_DoorLiftAxes.Enable = 1;	//0 = keine Achse vorhanden
//	g_DoorLiftAxes.AxisWithBrake = 0;
//	g_DoorLiftAxes.AxisFullRotary = 0;
	SetDoorLift_LastErrorAxisHWForEver(0);
	SetDoorLift_ErrorAxisHW(0);
	SetDoorLift_ErrorAxisCyBio(errNone);
	SetDoorLift_ErrorFlag(0);
	SetDoorLift_HomingFlag(FLG_HOMING_FAILURE);
//	SetDoorLift_Closed(DOORLIFT_OPEN);
//	g_SetAxes_StallFlag(0, 0);

	//SetDoorLift_Running(STATE_READY);
	SetDoorLift_ErrorAxisHWStall(0);

	ChangedDoorLifter.fOffset = FixDoorLifter.fOffset;
g_DoorLiftConfig.fOffset = FixDoorLifter.fOffset;

	ChangedDoorLifter.fStartSpeed = FixDoorLifter.fStartSpeed;

	ChangedDoorLifter.fEndSpeed = FixDoorLifter.fEndSpeed;
g_DoorLiftConfig.fEndSpeed = FixDoorLifter.fEndSpeed;

	ChangedDoorLifter.fAcceleration = FixDoorLifter.fAcceleration;
g_DoorLiftConfig.fAcceleration = FixDoorLifter.fAcceleration;

	ChangedDoorLifter.fHomingSpeed = FixDoorLifter.fHomingSpeed;
g_DoorLiftConfig.fHomingSpeed = FixDoorLifter.fHomingSpeed;

	ChangedDoorLifter.fAccelerationHoming = FixDoorLifter.fAccelerationHoming;
g_DoorLiftConfig.fAccelerationHoming = FixDoorLifter.fAccelerationHoming;

	ChangedDoorLifter.fPrecisionDeadband = FixDoorLifter.fPrecisionDeadband;

	ChangedDoorLifter.usStallsteps = FixDoorLifter.usStallsteps;
	ChangedDoorLifter.usHoldCurrentWaitTime = FixDoorLifter.usHoldCurrentWaitTime;
	ChangedDoorLifter.usMotorWobbleWaitTime = FixDoorLifter.usMotorWobbleWaitTime;
		
	ChangedDoorLifter.usRunCurrent = FixDoorLifter.usRunCurrent;
g_DoorLiftConfig.usRunCurrent = FixDoorLifter.usRunCurrent;

	ChangedDoorLifter.usHoldCurrent = FixDoorLifter.usHoldCurrent;
g_DoorLiftConfig.usHoldCurrent = FixDoorLifter.usHoldCurrent;

	ChangedDoorLifter.fMinWay = FixDoorLifter.fMinWay;
g_DoorLiftConfig.fMinWay  = FixDoorLifter.fMinWay;
	ChangedDoorLifter.fMaxWay = FixDoorLifter.fMaxWay;
g_DoorLiftConfig.fMaxWay  = FixDoorLifter.fMaxWay;


	ChangedDoorLifter.fDoorClose = FixDoorLifter.fDoorClose;
g_DoorLiftConfig.fDoorClose  = FixDoorLifter.fDoorClose;
	ChangedDoorLifter.fDoorOpen = FixDoorLifter.fDoorOpen;
g_DoorLiftConfig.fDoorOpen  = FixDoorLifter.fDoorOpen;

	//pIMSFixedPara[0] = &FixXDrive;
	//pIMSDoorLifChangedPara = &DoorLifter;

//	g_AxisOffset[0].AxisOffsetFromHomingSwitch = FixXDrive.fOffset;
//	g_AxisOffsetDefault[0].AxisOffsetFromHomingSwitch = FixXDrive.fOffset;
/*	g_ConfigStallExeption[0].Enable = 0;
	g_ConfigStallExeption[0].Up_DirA_Counter = 1;
	g_ConfigStallExeption[0].Down_DirB_Counter = 1;
	g_ConfigStallExeption[0].WaitTime = 100;
	g_CounterStallExeption[0].ExeptionLifeTimeCounter = 0;*/
}

unsigned short DoorLiftMotorInit(void)
{
	unsigned short error;
	unsigned short db;
	double db_double;

	long lStartSpd;
	long lEndSpd;
	unsigned long ulAcc;
	unsigned short i;

	for(i = 0; i < 3; i++)
	{
		if((error = ImsCmd_MotorResetError(DOORLIFT_ID)) == errNone) break;
	}
	if(error != errNone) {return error;}
	if((error = ImsCmd_ResetMotorStallFlag(DOORLIFT_ID)) != errNone) {return error;}

	if((error = ImsCmd_SetS1(DOORLIFT_ID)) != errNone) {return error;}

	if((error = ImsCmd_SetESC(DOORLIFT_ID)) != errNone) {return error;}
	if((error = ImsCmd_SetEncoderMode(DOORLIFT_ID)) != errNone) {return error;}
//K_Task_Wait(10);
	if((error = ImsCmd_StallDetectON(DOORLIFT_ID)) != errNone) {return error;}
//K_Task_Wait(10);
	if((error = ImsCmd_SetStallFactor(DOORLIFT_ID, ChangedDoorLifter.usStallsteps)) != errNone) {return error;}
//K_Task_Wait(10);
	//if((error = ImsCmd_SetEncoderDeadBand(ID, 1)) != errNone) {return error;}
	//oder MotorSetPrecision
	//if((error = MotorSetPrecision(DOORLIFT_ID, ChangedDoorLifter.fPrecisionDeadband)) != errNone) {return error;}
	db_double = fabs(FixDoorLifter.fENCImpProMM  * ChangedDoorLifter.fPrecisionDeadband);
	db = RoundToLong(db_double);
	if(db < DB_RANGE_MIN)
	{

		db = DB_RANGE_MIN;
	}
	error = ImsCmd_SetEncoderDeadBand( DOORLIFT_ID, db);
	if(error == errNone)
	{
		ChangedDoorLifter.fPrecisionDeadband = fabs((double)db / FixDoorLifter.fENCImpProMM);
	}
	if((error = ImsCmd_DisablePositionMaintenance(DOORLIFT_ID)) != errNone) {return error;}
//K_Task_Wait(10);
	if((error = ImsCmd_Motor_SetHoldCurrent(DOORLIFT_ID, ChangedDoorLifter.usHoldCurrent)) != errNone) {return error;}
//K_Task_Wait(10);
	if((error = ImsCmd_SetRunCurrent(DOORLIFT_ID, ChangedDoorLifter.usRunCurrent)) != errNone) {return error;}
//K_Task_Wait(10);
	if((error = ImsCmd_SetMotorWobbleWaitTime(DOORLIFT_ID, ChangedDoorLifter.usMotorWobbleWaitTime)) != errNone) {return error;}
//K_Task_Wait(10);
	if((error = ImsCmd_SetHoldCurrentWaitTime(DOORLIFT_ID, ChangedDoorLifter.usHoldCurrentWaitTime)) != errNone) {return error;}
	if((error = ImsCmd_Motor_ResetPosition(DOORLIFT_ID)) != errNone) {return error;}
//K_Task_Wait(10);
//normal
	//return MotorSetSpeedNormal(ID);
	lStartSpd = (long)(ChangedDoorLifter.fStartSpeed * FixDoorLifter.fMotImpProMM);
	lEndSpd = (long)(ChangedDoorLifter.fEndSpeed * FixDoorLifter.fMotImpProMM);
	ulAcc = (unsigned long)(ChangedDoorLifter.fAcceleration * FixDoorLifter.fMotImpProMM);
	if((error= ImsCmd_Motor_SetSpeed(DOORLIFT_ID, lStartSpd, lEndSpd, ulAcc)) != errNone) {return error;}
	return error;
}

void DoorLift_IMS_StopMotor(void)
{
	IMS_StopMotor(DOORLIFT_ID);
			
}

void DoorLiftCopyDataConfigToChanged(void)
{
	ChangedDoorLifter.fOffset = g_DoorLiftConfig.fOffset;
	ChangedDoorLifter.fMinWay = g_DoorLiftConfig.fMinWay;
	ChangedDoorLifter.fMaxWay = g_DoorLiftConfig.fMaxWay;
	ChangedDoorLifter.fDoorClose = g_DoorLiftConfig.fDoorClose;
	ChangedDoorLifter.fDoorOpen = g_DoorLiftConfig.fDoorOpen;

	ChangedDoorLifter.fEndSpeed = g_DoorLiftConfig.fEndSpeed;
	ChangedDoorLifter.fAcceleration = g_DoorLiftConfig.fAcceleration;
	ChangedDoorLifter.fHomingSpeed = g_DoorLiftConfig.fHomingSpeed;
	ChangedDoorLifter.fAccelerationHoming = g_DoorLiftConfig.fAccelerationHoming;
	ChangedDoorLifter.usRunCurrent = g_DoorLiftConfig.usRunCurrent;
	ChangedDoorLifter.usHoldCurrent = g_DoorLiftConfig.usHoldCurrent;
}
void SetDoorLift_ErrorAxisHWStall(unsigned short value)
{
	GLOBAL_AXES_DATA_ALLOC
	g_DoorLiftAxes.ErrorAxisHWStall = value;
	GLOBAL_AXES_DATA_RELEASE
}
unsigned short GetDoorLift_ErrorAxisHWStall(void)
{
	unsigned short tmp;
	GLOBAL_AXES_DATA_ALLOC
	tmp = g_DoorLiftAxes.ErrorAxisHWStall;
	GLOBAL_AXES_DATA_RELEASE
	return tmp;
}

void SetDoorLift_LastErrorAxisHWForEver(unsigned short value)
{
	GLOBAL_AXES_DATA_ALLOC
	g_DoorLiftAxes.LastErrorAxisHWForEver = value;
	GLOBAL_AXES_DATA_RELEASE
}
unsigned short GetDoorLift_LastErrorAxisHWForEver(void)
{
	unsigned short tmp;
	GLOBAL_AXES_DATA_ALLOC
	tmp = g_DoorLiftAxes.LastErrorAxisHWForEver;
	GLOBAL_AXES_DATA_RELEASE
	return tmp;
}

void SetDoorLift_ErrorAxisHW(unsigned short value)
{
	GLOBAL_AXES_DATA_ALLOC
	g_DoorLiftAxes.ErrorAxisHW = value;
	GLOBAL_AXES_DATA_RELEASE
}
unsigned short GetDoorLift_ErrorAxisHW(void)
{
	unsigned short tmp;
	GLOBAL_AXES_DATA_ALLOC
	tmp = g_DoorLiftAxes.ErrorAxisHW;
	GLOBAL_AXES_DATA_RELEASE
	return tmp;
}

void SetDoorLift_ErrorAxisCyBio(unsigned short value)
{
	GLOBAL_AXES_DATA_ALLOC
	g_DoorLiftAxes.ErrorAxisCyBio = value;
	GLOBAL_AXES_DATA_RELEASE
}
unsigned short GetDoorLift_ErrorAxisCyBio(void)
{
	unsigned short tmp;
	GLOBAL_AXES_DATA_ALLOC
	tmp = g_DoorLiftAxes.ErrorAxisCyBio;
	GLOBAL_AXES_DATA_RELEASE
	return tmp;
}


void SetDoorLift_ErrorFlag(unsigned short value)
{
	GLOBAL_AXES_DATA_ALLOC
	g_DoorLiftAxes.ErrorFlag = value;
	GLOBAL_AXES_DATA_RELEASE
}
unsigned short GetDoorLift_ErrorFlag(void)
{
	unsigned short tmp;
	GLOBAL_AXES_DATA_ALLOC
	tmp = g_DoorLiftAxes.ErrorFlag;
	GLOBAL_AXES_DATA_RELEASE
	return tmp;
}

void SetDoorLift_HomingFlag( unsigned short value)
{
	GLOBAL_AXES_DATA_ALLOC
	g_DoorLiftAxes.HomingFlag = value;
	GLOBAL_AXES_DATA_RELEASE
}
unsigned short GetDoorLift_HomingFlag(void)
{
	unsigned short tmp;
	GLOBAL_AXES_DATA_ALLOC
	tmp = g_DoorLiftAxes.HomingFlag;
	GLOBAL_AXES_DATA_RELEASE
	return tmp;
}

void SetDoorLift_Running(unsigned short value)
{
	GLOBAL_AXES_DATA_ALLOC
	g_DoorLiftAxes.Running = value;
	GLOBAL_AXES_DATA_RELEASE
}

unsigned short GetDoorLift_Running(void)
{
	unsigned short tmp;
	GLOBAL_AXES_DATA_ALLOC
	tmp = g_DoorLiftAxes.Running;
	GLOBAL_AXES_DATA_RELEASE
	return tmp;
}


/*void SetDoorLift_Closed( unsigned short value)
{
	GLOBAL_AXES_DATA_ALLOC
	g_DoorLiftAxes.Closed = value;
	GLOBAL_AXES_DATA_RELEASE
}*/

/*unsigned short GetDoorLift_Closed(void)
{
	unsigned short tmp;
	GLOBAL_AXES_DATA_ALLOC
	tmp = g_DoorLiftAxes.Closed;
	GLOBAL_AXES_DATA_RELEASE
	return tmp;
}*/

unsigned short Cmd_GetDoorLiftState(void)
{
	unsigned short DoorConfig = 0;
	unsigned short error;
	unsigned short HomeSwitch;

	if(g_EquipmentConfig.DeviceConfig & BIT_D_DoorLifter)
	{
		DoorConfig = 1;
	}
	if(DoorConfig)
	{
		if((error= ImsCmd_GetMotorHomeSwitch(DOORLIFT_ID, &HomeSwitch)) != errNone)
		{
			error = DoorLiftErrorHandling(error);
		}
		else
		{
			sprintf(g_acAnswerBuf,"I%u %u %u\r", DoorConfig, HomeSwitch, GetDoorLift_HomingFlag());
			error = errInfo;
		}
	}
	else
	{
		sprintf(g_acAnswerBuf,"I%u %u %u\r", DoorConfig, 0, GetDoorLift_HomingFlag());
		//sprintf(g_acAnswerBuf,"I%u %u %u\r", DoorConfig, GetDoorLift_Closed(), GetDoorLift_HomingFlag());
		error = errInfo;
	}
	return error;
}

unsigned short Cmd_GetDoorLiftHomingState(void)
{
	sprintf(g_acAnswerBuf,"I%u\r", GetDoorLift_HomingFlag());
	return errInfo;
}
unsigned short Cmd_GetDoorLiftCloseState(void)
{
	unsigned short error;
	unsigned short HomeSwitch;

//	sprintf(g_acAnswerBuf,"I%u\r", GetDoorLift_Closed());
	if((error= ImsCmd_GetMotorHomeSwitch(DOORLIFT_ID, &HomeSwitch)) != errNone)
	{
		error = DoorLiftErrorHandling(error);
	}
	else
	{
		sprintf(g_acAnswerBuf,"I%u\r", HomeSwitch);
		error = errInfo;
	}
	return error;
}

//####################################################################################
//####################################################################################
//####################################################################################
unsigned short DoorLiftErrorHandling(unsigned short extError)
{
	SetDoorLift_ErrorFlag(1);
	if(GetDoorLift_ErrorAxisHWStall())
	{
		//g_SetAxes_StallFlag(i, 1);
		SetDoorLift_ErrorAxisCyBio(ERR_STALL);
	}
	else
	{
		SetDoorLift_ErrorAxisCyBio(extError);
	}
	SetDoorLift_Running(STATE_ERROR);
	return GetDoorLift_ErrorAxisCyBio();
}

unsigned short Cmd_GetDoorLiftStatus(void)
{
	unsigned short error;
	unsigned short MoveFlag;
	long lStartSpd;
	long lEndSpd;
	unsigned long ulAcc;
	long lSteps;
	unsigned short HomeSwitch;
	unsigned short NoRestore = 0;

if(GetDoorLift_ErrorFlag())
{
	MoveFlag = STATE_ERROR;	
}
else
{
	error = ImsCmd_GetMoveFlag(DOORLIFT_ID, &MoveFlag);
	if(error != errNone)
	{
		//DoorLift_IMS_StopMotor();
		error = DoorLiftErrorHandling(error);
		DoorAdjustState = DOOR_HOMING_RESET_STATE;
		DoorMoveState = DOOR_MOVE_RESET_STATE;
	}
	else
	{

		if(DoorAdjustState != DOOR_HOMING_RESET_STATE)
		{
			if(DoorAdjustState == DOOR_HOMING_RUN_STATE)
			{
				if(!MoveFlag)
				{
					if((error = ImsCmd_Motor_ResetPosition(DOORLIFT_ID)) != errNone)
					{
						error = DoorLiftErrorHandling(error);
					}
					else
					{
						lSteps = (long)(ChangedDoorLifter.fOffset * FixDoorLifter.fMotImpProMM);
						if((error = ImsCmd_MoveAbsolutePos(DOORLIFT_ID, lSteps)) != errNone)
						{
							error =  DoorLiftErrorHandling(error);
							//return m_ErrorHandling(i, error);
						}
						else
						{
							DoorAdjustState = DOOR_HOMING_OFFSET_STATE;
							MoveFlag = 1;
						}
					}
				}
			}//end ...if(DoorAdjustState == DOOR_HOMING_RUN_STATE)
			else
			{
				if(DoorAdjustState == DOOR_HOMING_OFFSET_STATE)
				{
					if(!MoveFlag)
					{
						if((error = ImsCmd_Motor_ResetPosition(DOORLIFT_ID)) != errNone)
						{
							error = DoorLiftErrorHandling(error);
						}
						else
						{
							lStartSpd  = (long)(ChangedDoorLifter.fStartSpeed * FixDoorLifter.fMotImpProMM);
							//if(!CheckLong(VM_RANGE_MIN, VM_RANGE_MAX, lStartSpd)){return ERR_WRONG_PARAMETER;}
							lEndSpd = (long)(ChangedDoorLifter.fEndSpeed * FixDoorLifter.fMotImpProMM);
							//if(!CheckLong(VM_RANGE_MIN, VM_RANGE_MAX, lEndSpd)){return ERR_WRONG_PARAMETER;}
							//Beschleunigung
							ulAcc = (unsigned long)(ChangedDoorLifter.fAcceleration * FixDoorLifter.fMotImpProMM);
							//if(!CheckULong(A_RANGE_MIN, A_RANGE_MAX, ulAcc)){return ERR_WRONG_PARAMETER;}
							if((error= ImsCmd_Motor_SetSpeed(DOORLIFT_ID, -1, lEndSpd, ulAcc)) != errNone)
							{
								error = DoorLiftErrorHandling(error);
							}
							else
							{
								SetDoorLift_HomingFlag(1);
								if((error= ImsCmd_GetMotorHomeSwitch(DOORLIFT_ID, &HomeSwitch)) != errNone)
								{
									error = DoorLiftErrorHandling(error);
								}
								else
								{
									/*if(HomeSwitch)
									{
										SetDoorLift_Closed(DOORLIFT_CLOSED);
									}*/
									if(!HomeSwitch)
									{
										error = DoorLiftErrorHandling(ERR_DOOR_OPEN);
									}
									DoorAdjustState = DOOR_HOMING_RESET_STATE;
								}
							}
						}
					}
				}//end ...if(DoorAdjustState == DOOR_HOMING_OFFSET_STATE)
			}
		}//end...if(DoorAdjustState != DOOR_HOMING_RESET_STATE)
		else
		{
			if(DoorMoveState != DOOR_MOVE_RESET_STATE)
			{
				if(!MoveFlag)
				{
					if((error= ImsCmd_GetMotorHomeSwitch(DOORLIFT_ID, &HomeSwitch)) != errNone)
					{
						error = DoorLiftErrorHandling(error);
					}
					else
					{

						if(DoorMoveState == DOOR_MOVE_CLOSED_STATE)
						{
							if(!HomeSwitch)
							{
								error = DoorLiftErrorHandling(ERR_DOOR_OPEN);
							}
							else
							{
								K_Task_Wait(150);
								if(!isHostMode()){NoRestore = 1;}
								if(!isHostMode())
								{
									NoRestore = 1;
								}
								else
								{
									if(!CheckDriveHomingOk(eAllMask))
									{
										NoRestore = 1;
									}
								}
								error = Cmd_ClearEstop(NoRestore);
							}
						}
						else
						{
							if(HomeSwitch)
							{
								error = DoorLiftErrorHandling(ERR_DOOR_CLOSED);
							}
						}
						/*if(HomeSwitch)
						{
							SetDoorLift_Closed(DOORLIFT_CLOSED);
						}
						else
						{
							SetDoorLift_Closed(DOORLIFT_OPEN);
						}*/
					}
					/*if(DoorMoveState == DOOR_MOVE_CLOSED_STATE)
					{
						SetDoorLift_Closed(DOORLIFT_CLOSED);
					}
					else
					{
						SetDoorLift_Closed(DOORLIFT_OPEN);
						// ClearEstopError
					}*/
					DoorMoveState = DOOR_MOVE_RESET_STATE;
				}
			}
			/*else
			{
				Alles fertig und 0, nur GetErrorFlag abfragen undsenden	
			}*/
		}

		if(error != errNone)
		{
			DoorAdjustState = DOOR_HOMING_RESET_STATE;
			DoorMoveState = DOOR_MOVE_RESET_STATE;
		}
	}
	if(error != errNone)
	{
		MoveFlag = STATE_ERROR;
	} 
}
sprintf(g_acAnswerBuf, "I%u\r", MoveFlag);
error = errInfo;
return error;
}

unsigned short Cmd_DoorLiftOpen(unsigned short Synchron)
{
	unsigned short error = errNone;
	unsigned short MoveFlag;
	//unsigned short i;
	double Value;
	long lSteps;
	unsigned short HomeSwitch;

	if((error = scanDouble(&Value)) == errNone)
	{
		if(!CheckDouble(ChangedDoorLifter.fMinWay - (double)0.005 , ChangedDoorLifter.fMaxWay + (double)0.005, Value))
		{
			return ERR_WRONG_PARAMETER;
		}
		lSteps = (long)(Value * FixDoorLifter.fMotImpProMM);
	}
	else
	{
		lSteps = (long)(ChangedDoorLifter.fDoorOpen * FixDoorLifter.fMotImpProMM);
	}
	if(GetDoorLift_ErrorFlag())
	{
		if((error = Cmd_ResetDoorLiftError()) != errNone) {return error;}
	}
	/*
	for(i = 0; i < 3; i++)
	{
		if((error = ImsCmd_MotorResetError(DOORLIFT_ID)) == errNone) break;
	}
	if(error != errNone) {return error;}
	if((error = ImsCmd_ResetMotorStallFlag(DOORLIFT_ID)) != errNone) {return error;}
	*/

	if((error = ImsCmd_MoveAbsolutePos(DOORLIFT_ID, lSteps)) != errNone)
	{
		return DoorLiftErrorHandling(error);
		//return m_ErrorHandling(i, error);
	}
	if(Synchron)
	{
		do
		{
			K_Task_Wait(100);
			error = ImsCmd_GetMoveFlag(DOORLIFT_ID, &MoveFlag);
			if(error != errNone)
			{
				//DoorLift_IMS_StopMotor();
				error = DoorLiftErrorHandling(error);
				MoveFlag = 0;
			}
		}while(MoveFlag);
		if(error == errNone)
		{
			if((error= ImsCmd_GetMotorHomeSwitch(DOORLIFT_ID, &HomeSwitch)) != errNone)
			{
				error = DoorLiftErrorHandling(error);
			}
			else
			{
				if(HomeSwitch)
				{
					error = DoorLiftErrorHandling(ERR_DOOR_CLOSED);
				}
			}
			//SetDoorLift_Running(0);
			//SetDoorLift_Closed(DOORLIFT_OPEN);
		}
	}
	else
	{
		DoorMoveState = DOOR_MOVE_OPEN_STATE;
	}
	return error;
}
unsigned short Cmd_DoorLiftClose(unsigned short Synchron)
{
	unsigned short error = errNone;
	unsigned short MoveFlag;
	//unsigned short i;
	double Value;
	long lSteps;
	unsigned short HomeSwitch;
	unsigned short NoRestore = 0;

	if((error = scanDouble(&Value)) == errNone)
	{
		if(!CheckDouble(ChangedDoorLifter.fMinWay - (double)0.005 , ChangedDoorLifter.fMaxWay + (double)0.005, Value))
		{
			return ERR_WRONG_PARAMETER;
		}
		lSteps = (long)(Value * FixDoorLifter.fMotImpProMM);
	}
	else
	{
		lSteps = (long)(ChangedDoorLifter.fDoorClose * FixDoorLifter.fMotImpProMM);
	}

	if(GetDoorLift_ErrorFlag())
	{
		if((error = Cmd_ResetDoorLiftError()) != errNone) {return error;}
	}
	/*
	for(i = 0; i < 3; i++)
	{
		if((error = ImsCmd_MotorResetError(DOORLIFT_ID)) == errNone) break;
	}
	if(error != errNone) {return error;}
	if((error = ImsCmd_ResetMotorStallFlag(DOORLIFT_ID)) != errNone) {return error;}
	*/

	if((error = ImsCmd_MoveAbsolutePos(DOORLIFT_ID, lSteps)) != errNone)
	{
		return DoorLiftErrorHandling(error);
		//return m_ErrorHandling(i, error);
	}
	if(Synchron)
	{
		do
		{
			K_Task_Wait(100);
			error = ImsCmd_GetMoveFlag(DOORLIFT_ID, &MoveFlag);
			if(error != errNone)
			{
				//DoorLift_IMS_StopMotor();
				error = DoorLiftErrorHandling(error);
				MoveFlag = 0;
			}
		}while(MoveFlag);
		if(error == errNone)
		{
			if((error= ImsCmd_GetMotorHomeSwitch(DOORLIFT_ID, &HomeSwitch)) != errNone)
			{
				error = DoorLiftErrorHandling(error);
			}
			else
			{
				if(!HomeSwitch)
				{
					error = DoorLiftErrorHandling(ERR_DOOR_OPEN);

				}
				else
				{	
					K_Task_Wait(150);
					if(!isHostMode())
					{
						NoRestore = 1;
					}
					else
					{
						if(!CheckDriveHomingOk(eAllMask))
						{
							NoRestore = 1;
						}
					}
					error = Cmd_ClearEstop(NoRestore);
				}
			}
			//SetDoorLift_Running(0);
			//SetDoorLift_Closed(DOORLIFT_CLOSED);
		}
	}
	else
	{
		DoorMoveState = DOOR_MOVE_CLOSED_STATE;
	}
	return error;
}

unsigned short Cmd_DoorLiftAdj(unsigned short Synchron)
{
	unsigned short error = errNone;
	long lStartSpd;
	long lEndSpd;
	unsigned long ulAcc;
	unsigned short MoveFlag;
	long lSteps;
	//unsigned short i;

	SetDoorLift_HomingFlag(0);
	DoorAdjustState = DOOR_HOMING_RESET_STATE;

	if((error = Cmd_ResetDoorLiftError()) != errNone) {return error;}
	/*
	for(i = 0; i < 3; i++)
	{
		if((error = ImsCmd_MotorResetError(DOORLIFT_ID)) == errNone) break;
	}
	if(error != errNone) {return error;}
	if((error = ImsCmd_ResetMotorStallFlag(DOORLIFT_ID)) != errNone) {return error;}
	*/


	lStartSpd  = (long)(ChangedDoorLifter.fHomingStartSpeed * FixDoorLifter.fMotImpProMM);
	//if(!CheckLong(VM_RANGE_MIN, VM_RANGE_MAX, lStartSpd)){return ERR_WRONG_PARAMETER;}
	lEndSpd = (long)(ChangedDoorLifter.fHomingSpeed * FixDoorLifter.fMotImpProMM);
	//if(!CheckLong(VM_RANGE_MIN, VM_RANGE_MAX, lEndSpd)){return ERR_WRONG_PARAMETER;}
	//Beschleunigung
	ulAcc = (unsigned long)(ChangedDoorLifter.fAccelerationHoming * FixDoorLifter.fMotImpProMM);
	//if(!CheckULong(A_RANGE_MIN, A_RANGE_MAX, ulAcc)){return ERR_WRONG_PARAMETER;}
	if((error= ImsCmd_Motor_SetSpeed(DOORLIFT_ID, -1, lEndSpd, ulAcc)) != errNone) {return DoorLiftErrorHandling(error);}
	if((error= ImsCmd_HomingSequence(DOORLIFT_ID, FixDoorLifter.HomeToSwitchMode)) != errNone) {return DoorLiftErrorHandling(error);}
	//SetDoorLift_Running(1);
	if(Synchron)
	{
		do
		{
			K_Task_Wait(100);
			error = ImsCmd_GetMoveFlag(DOORLIFT_ID, &MoveFlag);
			if(error != errNone)
			{
				//DoorLift_IMS_StopMotor();
				error = DoorLiftErrorHandling(error);
				MoveFlag = 0;
			}
		}while(MoveFlag);
		if(error == errNone)
		{
			if((error = ImsCmd_Motor_ResetPosition(DOORLIFT_ID)) != errNone)
			{
				return DoorLiftErrorHandling(error);
			}
			lSteps = (long)(ChangedDoorLifter.fOffset * FixDoorLifter.fMotImpProMM);
			if((error = ImsCmd_MoveAbsolutePos(DOORLIFT_ID, lSteps)) != errNone)
			{
				return DoorLiftErrorHandling(error);
				//return m_ErrorHandling(i, error);
			}
			do
			{
				K_Task_Wait(100);
				error = ImsCmd_GetMoveFlag(DOORLIFT_ID, &MoveFlag);
				if(error != errNone)
				{
					//DoorLift_IMS_StopMotor();
					error = DoorLiftErrorHandling(error);
					MoveFlag = 0;
				}
			}while(MoveFlag);
		}
		if(error == errNone)
		{
			//SetDoorLift_Running(0);
			if((error = ImsCmd_Motor_ResetPosition(DOORLIFT_ID)) != errNone)
			{
				return DoorLiftErrorHandling(error);
			}
			lStartSpd  = (long)(ChangedDoorLifter.fStartSpeed * FixDoorLifter.fMotImpProMM);
			//if(!CheckLong(VM_RANGE_MIN, VM_RANGE_MAX, lStartSpd)){return ERR_WRONG_PARAMETER;}
			lEndSpd = (long)(ChangedDoorLifter.fEndSpeed * FixDoorLifter.fMotImpProMM);
			//if(!CheckLong(VM_RANGE_MIN, VM_RANGE_MAX, lEndSpd)){return ERR_WRONG_PARAMETER;}
			//Beschleunigung
			ulAcc = (unsigned long)(ChangedDoorLifter.fAcceleration * FixDoorLifter.fMotImpProMM);
			//if(!CheckULong(A_RANGE_MIN, A_RANGE_MAX, ulAcc)){return ERR_WRONG_PARAMETER;}
			if((error= ImsCmd_Motor_SetSpeed(DOORLIFT_ID, -1, lEndSpd, ulAcc)) != errNone) {return DoorLiftErrorHandling(error);}
			SetDoorLift_HomingFlag(1);
//			SetDoorLift_Closed(DOORLIFT_CLOSED);
		}
	}
	else
	{
		DoorAdjustState = DOOR_HOMING_RUN_STATE;
	}
	return error;
}

unsigned short Cmd_GetDoorAbsolutePosiion(void)
{
	unsigned short error = errNone;
	long lSteps;
	double AbsPosition;

	error = ImsCmd_GetPosition( DOORLIFT_ID, &lSteps);
	if(error == errNone)
	{
		AbsPosition = (double)lSteps / FixDoorLifter.fMotImpProMM;
		sprintf(g_acAnswerBuf, "I%3.2f\r", AbsPosition);
		error = errInfo;

	}
	return error;
}

unsigned short Cmd_SetDoorLiftSpd(unsigned short WriteFram)
{
	unsigned short error = errNone;
	//float fStartSpd;
	float fEndSpd;
	float fAcc;
	//long lStartSpd;
	long lEndSpd;
	unsigned long ulAcc;

	//if((error = scanFloat(&fStartSpd)) != errNone) return error;//Start oder End
	if((error = scanFloat(&fEndSpd)) != errNone) return error;// End oder Acceleration
	if((error = scanFloat(&fAcc)) != errNone) return error;// End oder Acceleration

	/*if(fStartSpd >= (float)0.0)
	{
		if(!CheckFloat(FixDoorLifter.fMinSpeed, FixDoorLifter.fMaxSpeed, fStartSpd)){return ERR_WRONG_PARAMETER;}
		if(fStartSpd > ChangedDoorLifter.fEndSpeed){return ERR_WRONG_PARAMETER;}
		lStartSpd = (long)(fStartSpd * FixDoorLifter.fMotImpProMM);
		if(!CheckLong(VI_RANGE_MIN, VI_RANGE_MAX, lStartSpd)){return ERR_WRONG_PARAMETER;}
	}*/
	//Endspeed
	if(!CheckFloat(FixDoorLifter.fMinSpeed, FixDoorLifter.fMaxSpeed, fEndSpd)){return ERR_WRONG_PARAMETER;}
	if(fEndSpd < ChangedDoorLifter.fStartSpeed){return ERR_WRONG_PARAMETER;}
	lEndSpd = (long)(fEndSpd * FixDoorLifter.fMotImpProMM);
	if(!CheckLong(VM_RANGE_MIN, VM_RANGE_MAX, lEndSpd)){return ERR_WRONG_PARAMETER;}

	//Beschleunigung
	if(!CheckFloat(FixDoorLifter.fMinAcceleration, FixDoorLifter.fMaxAcceleration, fAcc)){return ERR_WRONG_PARAMETER;}
	ulAcc = (unsigned long)(fAcc * FixDoorLifter.fMotImpProMM);
	if(!CheckULong(A_RANGE_MIN, A_RANGE_MAX, ulAcc)){return ERR_WRONG_PARAMETER;}
	if((error= ImsCmd_Motor_SetSpeed(DOORLIFT_ID, -1, lEndSpd, ulAcc)) != errNone) return DoorLiftErrorHandling(error);

	/*if(fStartSpd >= (float)0.0)
	{
		ChangedDoorLifter.fStartSpeed = fStartSpd;
	}*/
	ChangedDoorLifter.fEndSpeed = fEndSpd;
	ChangedDoorLifter.fAcceleration = fAcc;
	if(WriteFram)
	{
		/*if(fStartSpd >= (float)0.0)
		{
			g_DoorLiftConfig.fStartSpeed = ChangedDoorLifter.fStartSpeed;
		}*/
		g_DoorLiftConfig.fEndSpeed = ChangedDoorLifter.fEndSpeed;
		g_DoorLiftConfig.fAcceleration = ChangedDoorLifter.fAcceleration;
		error = FRAMSaveDoorLiftConfig(&g_DoorLiftConfig);
	}
	return error;
}

unsigned short Cmd_GetDoorLiftSpd(unsigned short ReadFram)
{
	unsigned short error;
//	unsigned short ID;
	udtSpeedParaDevice SpeedDev;
	udtSpeedParaMotor SpeedMot;
	unsigned short Scr;
	unsigned short Start;


	if(scanUShort(&Scr) != errNone)
	{
		Scr = 0;// von Firmware lesen, 1 = von Hardware lesen
	}
	if(scanUShort(&Start) != errNone)
	{
		Start = 0;// ohne Startspeed
	}
	if(ReadFram)
	{
		SpeedDev.Start = g_DoorLiftConfig.fStartSpeed;
		SpeedDev.End = g_DoorLiftConfig.fEndSpeed;
		SpeedDev.Acc = g_DoorLiftConfig.fAcceleration;
	}
	else
	{
		if(Scr)
		{
			error = ImsCmd_Motor_GetSpeed(DOORLIFT_ID, &SpeedMot.lStart, &SpeedMot.lEnd, &SpeedMot.ulAcc);
			if(error != errNone) return DoorLiftErrorHandling(error);
			SpeedDev.Start = (float)SpeedMot.lStart / FixDoorLifter.fMotImpProMM;
			SpeedDev.End = (float)SpeedMot.lEnd / FixDoorLifter.fMotImpProMM;
			SpeedDev.Acc = (float)SpeedMot.ulAcc / FixDoorLifter.fMotImpProMM;
		}
		else
		{
			SpeedDev.Start = ChangedDoorLifter.fStartSpeed;
			SpeedDev.End = ChangedDoorLifter.fEndSpeed;
			SpeedDev.Acc = ChangedDoorLifter.fAcceleration;
		}
	}
	if(Start)
	{
		sprintf(g_acAnswerBuf, "I%3.2f %3.2f %3.2f\r",SpeedDev.Start, SpeedDev.End, SpeedDev.Acc);
	}
	else
	{
		sprintf(g_acAnswerBuf, "I%3.2f %3.2f\r",SpeedDev.End, SpeedDev.Acc);
	}
	return errInfo;
}

unsigned short Cmd_SetDoorLiftAdjustSpd(unsigned short WriteFram)
{
	unsigned short error = errNone;
	//float fStartSpd;
	float fHomingSpd;
	float fAcc;
	//long lStartSpd;
	long lHomingSpd;
	unsigned long ulAcc;

	//if((error = scanFloat(&fStartSpd)) != errNone) return error;//Start oder End
	if((error = scanFloat(&fHomingSpd)) != errNone) return error;// End oder Acceleration
	if((error = scanFloat(&fAcc)) != errNone) return error;// End oder Acceleration

	/*if(fStartSpd >= (float)0.0)
	{
		if(!CheckFloat(FixDoorLifter.fMinSpeed, FixDoorLifter.fMaxSpeed, fStartSpd)){return ERR_WRONG_PARAMETER;}
		if(fStartSpd > ChangedDoorLifter.fEndSpeed){return ERR_WRONG_PARAMETER;}
		lStartSpd = (long)(fStartSpd * FixDoorLifter.fMotImpProMM);
		if(!CheckLong(VI_RANGE_MIN, VI_RANGE_MAX, lStartSpd)){return ERR_WRONG_PARAMETER;}
	}*/
	//Endspeed
	if(!CheckFloat(FixDoorLifter.fMinSpeed, FixDoorLifter.fMaxSpeed, fHomingSpd)){return ERR_WRONG_PARAMETER;}
	if(fHomingSpd < ChangedDoorLifter.fStartSpeed){return ERR_WRONG_PARAMETER;}
	lHomingSpd = (long)(fHomingSpd * FixDoorLifter.fMotImpProMM);
	//if(!CheckLong(VM_RANGE_MIN, VM_RANGE_MAX, lHomingSpd)){return ERR_WRONG_PARAMETER;}

	//Beschleunigung
	if(!CheckFloat(FixDoorLifter.fMinAcceleration, FixDoorLifter.fMaxAcceleration, fAcc)){return ERR_WRONG_PARAMETER;}
	ulAcc = (unsigned long)(fAcc * FixDoorLifter.fMotImpProMM);
	if(!CheckULong(A_RANGE_MIN, A_RANGE_MAX, ulAcc)){return ERR_WRONG_PARAMETER;}
	//if((error= ImsCmd_Motor_SetSpeed(DOORLIFT_ID, -1, lEndSpd, ulAcc)) != errNone) return error;

	/*if(fStartSpd >= (float)0.0)
	{
		ChangedDoorLifter.fHomingStartSpeed = fStartSpd;
	}*/
	ChangedDoorLifter.fHomingSpeed = fHomingSpd;
	ChangedDoorLifter.fAccelerationHoming = fAcc;
	if(WriteFram)
	{
		/*if(fStartSpd >= (float)0.0)
		{
			g_DoorLiftConfig.fHomingStartSpeed = ChangedDoorLifter.fHomingStartSpeed;
		}*/
		g_DoorLiftConfig.fHomingSpeed = ChangedDoorLifter.fHomingSpeed;
		g_DoorLiftConfig.fAccelerationHoming = ChangedDoorLifter.fAccelerationHoming;
		error = FRAMSaveDoorLiftConfig(&g_DoorLiftConfig);
	}

	return error;
}

unsigned short Cmd_GetDoorLiftAdjustSpd(unsigned short ReadFram)
{
	unsigned short error;
//	unsigned short ID;
	udtSpeedParaDevice SpeedDev;
	udtSpeedParaMotor SpeedMot;
	unsigned short Start;


	if(scanUShort(&Start) != errNone)
	{
		Start = 0;// ohne Startspeed
	}
	if(ReadFram)
	{
		SpeedDev.Start = g_DoorLiftConfig.fHomingStartSpeed;
		SpeedDev.End = g_DoorLiftConfig.fHomingSpeed;
		SpeedDev.Acc = g_DoorLiftConfig.fAccelerationHoming;
	}
	else
	{
		if(Start)
		{
			error = ImsCmd_Motor_GetSpeed(DOORLIFT_ID, &SpeedMot.lStart, &SpeedMot.lEnd, &SpeedMot.ulAcc);
			if(error != errNone) return DoorLiftErrorHandling(error);
			SpeedDev.Start = (float)SpeedMot.lStart / FixDoorLifter.fMotImpProMM;
			//SpeedDev.End = (float)SpeedMot.lEnd / FixDoorLifter.fMotImpProMM;
			//SpeedDev.Acc = (float)SpeedMot.ulAcc / FixDoorLifter.fMotImpProMM;
		}
		else
		{
			SpeedDev.Start = ChangedDoorLifter.fStartSpeed;
		}
		SpeedDev.End = ChangedDoorLifter.fHomingSpeed;
		SpeedDev.Acc = ChangedDoorLifter.fAccelerationHoming;
	}
	if(Start)
	{
		sprintf(g_acAnswerBuf, "I%3.2f %3.2f %3.2f\r",SpeedDev.Start, SpeedDev.End, SpeedDev.Acc);
	}
	else
	{
		sprintf(g_acAnswerBuf, "I%3.2f %3.2f\r",SpeedDev.End, SpeedDev.Acc);
	}
	return errInfo;
}

unsigned short Cmd_GetDoorLiftMaxWay(void)
{
	sprintf(g_acAnswerBuf,"I%3.2f %3.2f\r", ChangedDoorLifter.fMinWay, ChangedDoorLifter.fMaxWay);
	return errInfo;
}

unsigned short Cmd_SetDoorLiftMaxWay(void)
{
	unsigned short error = errNone;
	double tmp_MinWay;
	double tmp_MaxWay;
	double value;

	if((error = scanDouble(&value)) != errNone) return error;
	tmp_MinWay = value;
	if((error = scanDouble(&value)) != errNone) return error;
	tmp_MaxWay = value;
	if(tmp_MinWay > tmp_MaxWay) {return ERR_WRONG_PARAMETER;}
	ChangedDoorLifter.fMinWay = tmp_MinWay;
	ChangedDoorLifter.fMaxWay = tmp_MaxWay;
	g_DoorLiftConfig.fMinWay = tmp_MinWay;
	g_DoorLiftConfig.fMaxWay = tmp_MaxWay;
	return FRAMSaveDoorLiftConfig(&g_DoorLiftConfig);
}

unsigned short Cmd_GetDoorLiftOpenPos(void)
{
	sprintf(g_acAnswerBuf,"I%3.2f\r", ChangedDoorLifter.fDoorOpen);
	return errInfo;
}

unsigned short Cmd_SetDoorLiftOpenPos(void)
{
	unsigned short error = errNone;
	double tmp_Pos;
	double value;

	if((error = scanDouble(&value)) != errNone) return error;
	tmp_Pos = value;
	if(tmp_Pos > ChangedDoorLifter.fMaxWay) {return ERR_WRONG_PARAMETER;}
	ChangedDoorLifter.fDoorOpen = tmp_Pos;
	g_DoorLiftConfig.fDoorOpen = tmp_Pos;
	return FRAMSaveDoorLiftConfig(&g_DoorLiftConfig);
}
unsigned short Cmd_GetDoorLiftClosePos(void)
{
	sprintf(g_acAnswerBuf,"I%3.2f\r", ChangedDoorLifter.fDoorClose);
	return errInfo;
}

unsigned short Cmd_SetDoorLiftClosePos(void)
{
	unsigned short error = errNone;
	double tmp_Pos;
	double value;

	if((error = scanDouble(&value)) != errNone) return error;
	tmp_Pos = value;
	if(tmp_Pos < ChangedDoorLifter.fMinWay) {return ERR_WRONG_PARAMETER;}
	ChangedDoorLifter.fDoorClose = tmp_Pos;
	g_DoorLiftConfig.fDoorClose = tmp_Pos;
	return FRAMSaveDoorLiftConfig(&g_DoorLiftConfig);
}

unsigned short Cmd_SetDoorLiftOffset(void)
{
	unsigned short error = errNone;
//	double tmp_MinWay;
//	double tmp_MaxWay;
	float tmp_Offset;
	float value;

	if((error = scanFloat(&value)) != errNone) return error;
	tmp_Offset = value;
/*	tmp_MinWay = ChangedDoorLifter.fMinWay;
	tmp_MaxWay = ChangedDoorLifter.fMaxWay;
	tmp_MinWay = ChangedDoorLifter.fMinWay + ChangedDoorLifter.fOffset - tmp_Offset;
	tmp_MaxWay = ChangedDoorLifter.fMaxWay + ChangedDoorLifter.fOffset - tmp_Offset;
	ChangedDoorLifter.fMinWay = tmp_MinWay;
	ChangedDoorLifter.fMaxWay = tmp_MaxWay;*/
	ChangedDoorLifter.fOffset = tmp_Offset;
/*	g_DoorLiftConfig.fMinWay = tmp_MinWay;
	g_DoorLiftConfig.fMaxWay = tmp_MaxWay;*/
	g_DoorLiftConfig.fOffset = tmp_Offset;
	return FRAMSaveDoorLiftConfig(&g_DoorLiftConfig);
}

unsigned short Cmd_GetDoorLiftOffset(void)
{
	sprintf(g_acAnswerBuf,"I%3.2f\r", ChangedDoorLifter.fOffset);
	return errInfo;
}

unsigned short Cmd_GetDoorLiftError(void)
{
	unsigned short error = errNone;
	if(GetDoorLift_ErrorFlag())
	{
		error = GetDoorLift_ErrorAxisCyBio();
	}
	else {error = errNone;}
	sprintf(g_acAnswerBuf,"I%u\r", error);
	return errInfo;
}

unsigned short Cmd_ResetDoorLiftError(void)
{
	unsigned short error;
	unsigned short i;

	SetDoorLift_ErrorAxisCyBio(errNone);
	SetDoorLift_ErrorAxisHW(0);
	SetDoorLift_ErrorFlag(0);
	SetDoorLift_Running(STATE_READY);
	SetDoorLift_ErrorAxisHWStall(0);
	//SetDoorLift_ErrorAxisHWStall(0);

	for(i = 0; i < 3; i++)
	{
		if((error = ImsCmd_MotorResetError(DOORLIFT_ID)) == errNone) break;
	}
	if(error != errNone)
	{
		error = DoorLiftErrorHandling(error);
	}
	else
	{
		error = ImsCmd_ResetMotorStallFlag(DOORLIFT_ID);
		if(error != errNone)
		{
			error = DoorLiftErrorHandling(error);
		}

	}
	return error;
}

unsigned short Cmd_GetDoorLiftOEMError(void)
{
	unsigned short error;
	unsigned short usValue;

	//usValue = GetDoorLift_ErrorAxisHW();
	if((error = ImsCmd_GetOEMError(DOORLIFT_ID, &usValue)) == errNone)
	{
		sprintf(g_acAnswerBuf,"I%u\r", usValue);
		error = errInfo;
	}
	else
	{
		error = DoorLiftErrorHandling(error);
	}
	return error;
}

unsigned short Cmd_ResetDoorLiftOEMError(void)
{
	unsigned short error;

	error = ImsCmd_MotorResetError(DOORLIFT_ID);
	if(error != errNone)
	{
		error = DoorLiftErrorHandling(error);
	}
	else
	{
		error = ImsCmd_ResetMotorStallFlag(DOORLIFT_ID);
		if(error != errNone)
		{
			error = DoorLiftErrorHandling(error);
		}
	}
	return error;
}

unsigned short Cmd_GetDoorLiftHWError(void)
{
	sprintf(g_acAnswerBuf,"I%u\r", GetDoorLift_LastErrorAxisHWForEver());
	return errInfo;
}

/*unsigned short Cmd_ResetDoorLiftHWError(void)
{
	unsigned short error;

	error = ImsCmd_MotorResetError(DOORLIFT_ID);
	if(error != errNone)
	{
		error = DoorLiftErrorHandling(error);
	}
	else
	{
		error = ImsCmd_ResetMotorStallFlag(DOORLIFT_ID);
		if(error != errNone)
		{
			error = DoorLiftErrorHandling(error);
		}
	}

	return error;
//	SetDoorLift_LastErrorAxisHWForEver(0);
//	return errNone;
}*/

unsigned short Cmd_DoorLifterWriteCurrent(const udtMOTOR_CURRENT_Type CurrentType, unsigned short WriteFram)
{
	unsigned short error = errNone;
	//unsigned short WriteFram = 1;
	unsigned short Current;
	unsigned short oldCurrent;

	if((error = scanUShort(&Current)) != errNone) return error;
	/*if(scanUShort(&WriteFram) != errNone)
	{
		WriteFram = 0;// ohne speichern
	}*/
	switch(CurrentType)
	{
		case eSET_HOLD_CURRENT:	oldCurrent = ChangedDoorLifter.usHoldCurrent;
								ChangedDoorLifter.usHoldCurrent = Current;
								error = ImsCmd_Motor_SetHoldCurrent( DOORLIFT_ID, Current);
								if(error != errNone)
								{
									ChangedDoorLifter.usHoldCurrent = oldCurrent;
									error = DoorLiftErrorHandling(error);
								}
								break;

		/*case eSET_CURRENT:		error = ImsCmd_SetRunCurrent( DOORLIFT_ID, Current);
								if(error != errNone)
								{
									error = DoorLiftErrorHandling(error);
								}
								WriteFram = 0;// ohne speichern
								break;*/

		case eSET_RUN_CURRENT:  oldCurrent = ChangedDoorLifter.usRunCurrent;
								ChangedDoorLifter.usRunCurrent = Current;
								//error = IMS_SetRunCurrent( DOORLIFT_ID, Current);
								error = ImsCmd_SetRunCurrent( DOORLIFT_ID, Current);
								if(error != errNone)
								{
									ChangedDoorLifter.usRunCurrent = oldCurrent;
									error = DoorLiftErrorHandling(error);
								}
								break;
		default:			error = ERR_WRONG_PARAMETER;
							break;
	}
	if(error == errNone)
	{
		if(WriteFram)
		{

			g_DoorLiftConfig.usHoldCurrent = ChangedDoorLifter.usHoldCurrent;
			g_DoorLiftConfig.usRunCurrent = ChangedDoorLifter.usRunCurrent;
			error = FRAMSaveDoorLiftConfig(&g_DoorLiftConfig);
		}
	}
	return error;
}

unsigned short Cmd_DoorLifterReadCurrent(const udtMOTOR_CURRENT_Type CurrentType, unsigned short ReadFram)
{
	unsigned short error = errNone;
	unsigned short Scr = 1;
	unsigned short Current;


	if(scanUShort(&Scr) != errNone)
	{
		Scr = 0;// von Firmware lesen, 1 = von Hardware lesen
	}
	switch(CurrentType)
	{
		/*case eGET_CURRENT:
								error = ImsCmd_GetRunCurrent( DOORLIFT_ID, &Current);
								if(error != errNone)
								{
									error = DoorLiftErrorHandling(error);
								}
								break;*/
		case eGET_RUN_CURRENT:
								if(ReadFram)
								{
									Current = g_DoorLiftConfig.usRunCurrent;
								}
								else
								{
									if(Scr)
									{
										//error = IMS_GetRunCurrent( DOORLIFT_ID, &Current);
										error = ImsCmd_GetRunCurrent( DOORLIFT_ID, &Current);
										if(error != errNone)
										{
											error = DoorLiftErrorHandling(error);
										}
									}
									else
									{
										Current = ChangedDoorLifter.usRunCurrent;
									}
								}
								break;
		case eGET_HOLD_CURRENT:
								if(ReadFram)
								{
									Current = g_DoorLiftConfig.usHoldCurrent;
								}
								else
								{
									if(Scr)
									{
										error = ImsCmd_GetHoldCurrent( DOORLIFT_ID, &Current);
										if(error != errNone)
										{
											error = DoorLiftErrorHandling(error);
										}
									}
									else
									{
										Current = ChangedDoorLifter.usHoldCurrent;
									}
								}
								break;
		default:				error = ERR_WRONG_PARAMETER;
								break;
	}
	if(error == errNone)
	{
		sprintf(g_acAnswerBuf,"I%u\r", Current);
		error = errInfo;
	}
	return error;
}


unsigned short Cmd_DoorLifterOEM(void)
{

	IMS_CommandBlock *ptr;
	unsigned short error = errNone;
	unsigned short cnt;
	char *pStart, *pEnd;

	Msg.eCommand = eImsDoorLifterOEM;
	ptr = &Msg;

	cnt = ZaehleZeichenImScanner('"');
	if((cnt == 0 ) ||  (cnt % 2 )) return ERR_NOT_ENOUGH_PARAMETER;

	pStart = strchr(GetScannerPtr(), '"');
	pStart++;
	pEnd = strrchr(GetScannerPtr(), '"');
	*pEnd = '\0';
	strcpy((char *)Msg.Argument.cBuffer, pStart);
	K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	error = Msg.Error;
	if(error == errInfo)
	{
		sprintf(g_acAnswerBuf,"I%s\r", (char *)Msg.Argument.cBuffer);
		return errInfo;
		
	}
	return error;
}

