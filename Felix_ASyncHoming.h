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
#ifndef _ASYNCHOMING_H_
#define _ASYNCHOMING_H_

//****************************************************************************
// include files
//****************************************************************************

//****************************************************************************
// @Macros
//****************************************************************************

//****************************************************************************
// @Defines
//****************************************************************************
#define ADJ_NONE 0
/*
#define ADJ_STEP_Z_HOMING_START 1
#define ADJ_STEP_Z_HOMING_WAIT 2
#define ADJ_STEP_XY_MV_OUT_ZERO_START 3
#define ADJ_STEP_XY_MV_OUT_ZERO_WAIT 4

#define ADJ_STEP_XY_HOMING_START 5
#define ADJ_STEP_XY_HOMING_WAIT 6
#define ADJ_STEP_XY_MV_OFFSET_START 7
#define ADJ_STEP_XY_MV_OFFSET_WAIT 8
#define ADJ_STEP_SET_COORDINATE_START 9
#define ADJ_STEP_SET_COORDINATE_WAIT 10
*/
#define ADJ_STEP_Z_HOMING_START 1
#define ADJ_STEP_Z_HOMING_INVERS_START 2
#define ADJ_STEP_Z_HOMING_INVERS_WAIT 3
#define ADJ_STEP_Z_HOMING_NORMAL_START 4
#define ADJ_STEP_Z_HOMING_NORMAL_WAIT 5

#define ADJ_STEP_Z_MV_OFFSET_START 6
#define ADJ_STEP_Z_MV_OFFSET_WAIT 7
#define ADJ_STEP_Z_SET_COORDINATE_START 8
#define ADJ_STEP_Z_SET_COORDINATE_WAIT 9

#define ADJ_STEP_XY_MV_OUT_ZERO_START 10
#define ADJ_STEP_XY_MV_OUT_ZERO_WAIT 11

#define ADJ_STEP_XY_HOMING_START 12
#define ADJ_STEP_XY_HOMING_WAIT 13
#define ADJ_STEP_XY_MV_OFFSET_START 14
#define ADJ_STEP_XY_MV_OFFSET_WAIT 15
#define ADJ_STEP_SET_COORDINATE_START 16
#define ADJ_STEP_SET_COORDINATE_WAIT 17
//****************************************************************************
// @Typedefs
//****************************************************************************
//****************************************************************************
// @Imported Global Variables
//****************************************************************************

//****************************************************************************
// @Global Variables
//****************************************************************************
extern unsigned short m_AdjustActive;
extern unsigned short m_Adjust_State;
extern unsigned short m_AdjustMask;
//****************************************************************************
// @External Prototypes
//****************************************************************************

//****************************************************************************
// Prototypen definitions
//****************************************************************************
extern unsigned short ASync_Adjust(unsigned short *ContinueEstop);
extern unsigned short SetHomingCtrlVar(unsigned short Mask);
extern unsigned short SetHomingVar(unsigned short Mask);
extern void SetAdjustState(void);
extern void ResetHomingCtrlVar(void);
//****************************************************************************
// @Prototypes Of Local Functions
//****************************************************************************

#endif