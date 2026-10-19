//****************************************************************************
// @Module        DoorLifter
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
#ifndef _DOORLIFTER_H_
#define _DOORLIFTER_H_
//****************************************************************************
// include files
//****************************************************************************
#include "DeviceConfig.h"
//****************************************************************************
// @Macros
//****************************************************************************

//****************************************************************************
// @Defines
//****************************************************************************
#ifdef _CF_FELIX
	#define AXIS_ID_DOORLIFT_DRIVE 6
	#define AXIS_ADR_DOORLIFT_DRIVE 'D'

#endif

#define DOORLIFT_OPEN 0
#define DOORLIFT_CLOSED 1
//****************************************************************************
// @Typedefs
//****************************************************************************

typedef struct
{
	unsigned short ID;
	char BusAdr;
	unsigned short AxisBit;
	float fOffset;
	float fSetHomingPosition;
//	double fDiffRestoreActivate;
	// Geschwindigkeiten
	float fStartSpeed;			//[mm/s]
	float fEndSpeed;		 	// Endspeed positive direction [mm/s]
	float fHomingStartSpeed;			//[mm/s]
	float fHomingSpeed;			// Endspeed homing [mm/s]
	float fAcceleration;	// Acceleration positive direction [mm/s*s]
	float fAccelerationHoming;	// Acceleration homing [mm/s*s]
	// Grenzwerte
	float fMinAcceleration;		// min. Acceleration [mm/s*s]
	float fMaxAcceleration;		// max. Acceleration [mm/s*s]
	float fMinSpeed;			// min. Endspeed [mm/s*s]
	float fMaxSpeed;			// max. Endspeed [mm/s*s]
	double fMinWay;				// min. drive range [mm]
	double fMaxWay;				// max. drive range [mm]

	// sonstiges
	float fMechLose;						// Lose (mech. Spiel) [mm]
	double fPrecisionDeadband;				// Genauigkeit der Positionierung [mm]
	unsigned short usStallsteps;			// zulässige Diff. Steps zu Encoder [encoder_step]
	unsigned short usHoldCurrentWaitTime;	// hold current wait time [millisecounds]
	unsigned short usMotorWobbleWaitTime;	// wobbling wait time [millisecounds]

	// Strom
	unsigned short usRunCurrent;	//  run current [percent]
	unsigned short usHoldCurrent;	// hold current [percent]

	// Übersetzung
	float fWayProRev;			// Weg pro Umdrehung in mm
	float fIgrImpulseProRev;	// EncoderAuflösung	0 -> Achse ohne Encoder
	double fMotImpProMM;			// Motor-Impulse (µ-Step) pro mm	bzw. pro Umdr.
	double fENCImpProMM;			// Encoder-Impulse pro mm
//	unsinged short usFullStepPerRev;	// Vollschritte pro Motorumdrehung
//	unsigned short usMicroStepPerFullStep;	//Anzahl der Mikroschritte pro Vollschritt
//	float fMicroStepProRev;			// Microschritte pro Umdrehung
	//void (*Direction)(unsigned short usDirection);
	unsigned short HomeToSwitchMode;	//1. Speed: Specifies the direction and speed that the axis will
										//move until the switch is activated (VM).
										//2. Creep: Specifies the direction and speed that the axis will
										//move off the switch until it becomes inactive again (VI).
										//When HM is executed, the axis moves at VM in the direction
										//specified by the sign of speed. It then creeps off of the switch at VI
										//in the direction specified by the sign of creep. Motion is stopped
										//as soon as the switch becomes deactivated. Note that Speed and
										//Creep is set by the VM and VI commands.
										//Range 1 - 4
										//Modes
										// 1 Slew at VM in the minus direction and Creep at VI in the plus direction.
										// 2 Slew at VM in the minus direction and Creep at VI in the minus direction.
										// 3 Slew at VM in the plus direction and Creep at VI in the minus direction.
										// 4 Slew at VM in the plus direction and Creep at VI in the plus direction.
	unsigned short InversHomeToSwitchMode;	// wieHomeToSwitchMode nur invers
	double fDoorClose;
	double fDoorOpen;
}udtIMSDoorLiftAxisFixedParameter;


typedef struct
{
	float fOffset;
	// Geschwindigkeiten
	float fStartSpeed;			//[mm/s]
	float fEndSpeed;		 	// Endspeed [mm/s]
	float fHomingStartSpeed;			//[mm/s]
	float fHomingSpeed;			// Endspeed homing [mm/s]
	float fAcceleration;	// Acceleration [mm/s*s]
	float fAccelerationHoming;	// Acceleration homing [mm/s*s]

	// sonstiges
	double fPrecisionDeadband;				// Genauigkeit der Positionierung [mm]

	unsigned short usStallsteps;						// zulässige Diff. Steps zu Encoder [encoder_step]
	unsigned short usHoldCurrentWaitTime;	// hold current wait time [millisecounds]
	unsigned short usMotorWobbleWaitTime;	// wobbling wait time [millisecounds]

	// Strom
	unsigned short usRunCurrent;	//  run current [percent]
	unsigned short usHoldCurrent;	// hold current [percent]
	double fMinWay;
	double fMaxWay;
	double fDoorClose;
	double fDoorOpen;


}udtIMSDoorLiftAxisChangedParameter;

typedef struct
{
	float fOffset;
	double fMinWay;
	double fMaxWay;
	double fDoorClose;
	double fDoorOpen;
	float fStartSpeed;			//[mm/s]
	float fEndSpeed;		 	// Endspeed [mm/s]
	float fAcceleration;	// Acceleration [mm/s*s]
	float fHomingStartSpeed;			//[mm/s]
	float fHomingSpeed;			// Endspeed homing [mm/s]
	float fAccelerationHoming;	// Acceleration homing [mm/s*s]
	unsigned short usRunCurrent;	//  run current [percent]
	unsigned short usHoldCurrent;	// hold current [percent]
}udtIMSDoorLiftConfig;

typedef struct
{
	unsigned short AxisID;
	char AxisAdr;
	unsigned short AxisBit;
	unsigned short ParaIDX;
//	unsigned short Enable;
//	unsigned short AxisWithBrake;
//	unsigned short AxisFullRotary;
//	unsigned short AxisStallHandling;
	unsigned short LastErrorAxisHWForEver;
	unsigned short ErrorAxisHW;
	unsigned short ErrorAxisHWStall;
	unsigned short ErrorAxisCyBio;
	unsigned short ErrorFlag;
	unsigned short HomingFlag;
	unsigned short Running;
	unsigned short Closed;
}udtIMSDoorLiftAxes;

//****************************************************************************
// @Imported Global Variables
//****************************************************************************

//****************************************************************************
// @Global Variables
//****************************************************************************
#ifdef _CF_FELIX
//extern const udtIMSAxisFixedParameter FixDoorLifter;
//extern const udtIMSAxisFixedParameter *pIMSDoorLifFixedPara;
extern udtIMSDoorLiftAxisChangedParameter DoorLifter;
//extern udtIMSDoorLiftAxisChangedParameter *pIMSDoorLifChangedPara;
extern udtIMSDoorLiftAxes g_DoorLiftAxes;
extern udtIMSDoorLiftConfig g_DoorLiftConfig;
#endif

//****************************************************************************
// @External Prototypes
//****************************************************************************

//****************************************************************************
// Prototypen definitions
//****************************************************************************
void DorrLift_U1C0_ASC_vInit(void);
void Slave_U1C0_ASC_vInit(void);
void DoorLiftGenerate(void);
void DoorLiftCopyDataConfigToChanged(void);

void SetDoorLift_LastErrorAxisHWForEver(unsigned short value);
unsigned short GetDoorLift_LastErrorAxisHWForEver(void);

void SetDoorLift_ErrorAxisHW(unsigned short value);
unsigned short GetDoorLift_ErrorAxisHW(void);

void SetDoorLift_ErrorAxisCyBio(unsigned short value);
unsigned short GetDoorLift_ErrorAxisCyBio(void);

void SetDoorLift_ErrorFlag(unsigned short value);
unsigned short GetDoorLift_ErrorFlag(void);

void SetDoorLift_ErrorAxisHWStall(unsigned short value);
unsigned short GetDoorLift_ErrorAxisHWStall(void);

void SetDoorLift_Running(unsigned short value);
unsigned short GetDoorLift_Running(void);

void SetDoorLift_HomingFlag( unsigned short value);
unsigned short GetDoorLift_HomingFlag(void);

//void SetDoorLift_Closed( unsigned short value);
//unsigned short GetDoorLift_Closed(void);


unsigned short Cmd_GetDoorLiftState(void);
unsigned short Cmd_GetDoorLiftHomingState(void);
unsigned short Cmd_GetDoorLiftCloseState(void);


unsigned short Cmd_GetDoorLiftStatus(void);

unsigned short Cmd_DoorLiftOpen(unsigned short Synchron);
unsigned short Cmd_DoorLiftClose(unsigned short Synchron);

unsigned short Cmd_DoorLiftAdj(unsigned short Synchron);

unsigned short Cmd_GetDoorAbsolutePosiion(void);

unsigned short Cmd_SetDoorLiftSpd(unsigned short WriteFram);
unsigned short Cmd_GetDoorLiftSpd(unsigned short ReadFram);

unsigned short Cmd_SetDoorLiftAdjustSpd(unsigned short WriteFram);
unsigned short Cmd_GetDoorLiftAdjustSpd(unsigned short ReadFram);

unsigned short Cmd_SetDoorLiftMaxWay(void);
unsigned short Cmd_GetDoorLiftMaxWay(void);

unsigned short Cmd_GetDoorLiftOpenPos(void);
unsigned short Cmd_SetDoorLiftOpenPos(void);
unsigned short Cmd_GetDoorLiftClosePos(void);
unsigned short Cmd_SetDoorLiftClosePos(void);



unsigned short Cmd_SetDoorLiftOffset(void);
unsigned short Cmd_GetDoorLiftOffset(void);

unsigned short Cmd_ResetDoorLiftError(void);
unsigned short Cmd_GetDoorLiftError(void);

unsigned short Cmd_ResetDoorLiftOEMError(void);
unsigned short Cmd_GetDoorLiftOEMError(void);

//unsigned short Cmd_ResetDoorLiftHWError(void);
unsigned short Cmd_GetDoorLiftHWError(void);

unsigned short Cmd_DoorLifterWriteCurrent(const udtMOTOR_CURRENT_Type CurrentType, unsigned short WriteFram);
unsigned short Cmd_DoorLifterReadCurrent(const udtMOTOR_CURRENT_Type CurrentType, unsigned short ReadFram);


unsigned short Cmd_SetDoorLiftSpd(unsigned short WriteFram);
unsigned short Cmd_GetDoorLiftSpd(unsigned short ReadFram);
unsigned short Cmd_SetDoorLiftAdjustSpd(unsigned short WriteFram);
unsigned short Cmd_GetDoorLiftAdjustSpd(unsigned short ReadFram);
unsigned short Cmd_DoorLifterWriteCurrent(const udtMOTOR_CURRENT_Type CurrentType, unsigned short WriteFram);
unsigned short Cmd_DoorLifterReadCurrent(const udtMOTOR_CURRENT_Type CurrentType, unsigned short ReadFram);

unsigned short DoorLiftMotorInit(void);

unsigned short DoorLiftErrorHandling(unsigned short extError);

unsigned short Cmd_DoorLifterOEM(void);
//****************************************************************************
// @Prototypes Of Local Functions
//****************************************************************************
//****************************************************************************
// @Prototypes Of Global Functions
//****************************************************************************

#endif