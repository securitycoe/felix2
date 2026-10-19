//****************************************************************************
// @Module        Schrittmotor-Controller
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
#ifndef _MOTOREXE_H_
#define _MOTOREXE_H_
//****************************************************************************
// include files
//****************************************************************************

//****************************************************************************
// @Macros
//****************************************************************************

//****************************************************************************
// @Defines
//****************************************************************************
#define HOMING 1
#define SLOMO 2

//****************************************************************************
// @Typedefs
//****************************************************************************
typedef struct
{
	float Start;
	float End;
	float Acc;
}udtSpeedParaDevice;

typedef struct
{
	long lStart;
	long lEnd;
	unsigned long ulAcc;
}udtSpeedParaMotor;

//****************************************************************************
// @Imported Global Variables
//****************************************************************************

//****************************************************************************
// @Global Variables
//****************************************************************************

//****************************************************************************
// @External Prototypes
//****************************************************************************

//****************************************************************************
// Prototypen definitions
//****************************************************************************

//extern unsigned short MotorHomingExe(unsigned short Mask);
//extern unsigned short MotorMoveSyncronExe(unsigned short Mask, unsigned short AbsolueElseRelative);
extern unsigned short MotorMoveSyncronExe(unsigned short Mask, unsigned short Mode1536);
//extern unsigned short MotorMoveASyncExe(unsigned short Mask, unsigned short AbsolueElseRelative);
extern unsigned short MotorMoveASyncExe(unsigned short Mask, unsigned short Mode1536);
extern unsigned short MotorTeachExe(unsigned short Mask);
extern unsigned short MotorSetSpeedExe(unsigned short  ID, float Start, float End, float Acc);
extern unsigned short MotorSetSonderSpeedExe(unsigned short funktion, unsigned short ID, float End, float Acc);
extern unsigned short MotorGetSpeedExe(const unsigned short ID, udtSpeedParaDevice *Speed, unsigned short Scr);
extern unsigned short MotorGetSonderSpeedExe(unsigned short funktion, unsigned short ID, udtSpeedParaDevice *Speed);

//****************************************************************************
// @Prototypes Of Local Functions
//****************************************************************************

#endif
