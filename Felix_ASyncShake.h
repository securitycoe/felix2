//****************************************************************************
// @Module        Controller
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
#ifndef _ASYNCSHAKE_H_
#define _ASYNCSHAKE_H_

//****************************************************************************
// include files
//****************************************************************************

//****************************************************************************
// @Macros
//****************************************************************************

//****************************************************************************
// @Defines
//****************************************************************************
#define SHAKE_NONE 0
#define SHAKE_STEP1_X_POS1_START 21
#define SHAKE_STEP1_X_POS1_WAIT 22
#define SHAKE_STEP2_Y_POS1_START 23
#define SHAKE_STEP2_Y_POS1_WAIT 24
#define SHAKE_STEP3_X_POS2_START 25
#define SHAKE_STEP3_X_POS2_WAIT 26
#define SHAKE_STEP4_Y_POS2_START 27
#define SHAKE_STEP4_Y_POS2_WAIT 28
#define SHAKE_STEP5_XY_RESTORE_START 29
#define SHAKE_STEP5_XY_RESTORE_WAIT 30


//****************************************************************************
// @Typedefs
//****************************************************************************
//****************************************************************************
// @Imported Global Variables
//****************************************************************************

//****************************************************************************
// @Global Variables
//****************************************************************************
extern unsigned short m_ShakeActive;
extern unsigned short m_ShakeMask;
//****************************************************************************
// @External Prototypes
//****************************************************************************

//****************************************************************************
// Prototypen definitions
//****************************************************************************
extern void ResetShakeCtrlVar(void);
extern void SetShakeCtrlVar(unsigned short Mask, unsigned short Count);
extern unsigned short ASync_ShakeDataAll(ASync_CommandBlock * ptr);
extern unsigned short ASync_Shake(void);
extern unsigned short Shake_RestoreMoveDriveParaAll(unsigned short mask);
//****************************************************************************
// @Prototypes Of Local Functions
//****************************************************************************

#endif