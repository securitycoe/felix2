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
#ifndef _ASYNCCTRL_H_
#define _ASYNCCTRL_H_

//****************************************************************************
// include files
//****************************************************************************

//****************************************************************************
// @Macros
//****************************************************************************

//****************************************************************************
// @Defines
//****************************************************************************


//****************************************************************************
// @Typedefs
//****************************************************************************
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
//extern void MotorTask_vInit(void);
extern void ASyncCtrlTask(void);
extern unsigned short g_ASync_GetAllRuningFlags(unsigned short Mask, unsigned short *value);
extern unsigned short g_ASync_GetAllErrorFlags(unsigned short Mask, unsigned short *value);
extern unsigned short g_ASync_ResetError(unsigned short AbfrageFehler, unsigned short Mask);
extern unsigned short g_ASync_StartMultipleMoveAbsolute(unsigned short Mask, unsigned short Mode1536);
extern unsigned short g_ASync_StartMultipleMoveTeach(unsigned short Mask);
extern unsigned short g_ASync_StartShake(unsigned short Mask);
extern unsigned short g_ASync_StartSearchAdapter(unsigned short Mask);
extern unsigned short m_GetAxisIDFromMask(unsigned short mask, unsigned short AxisBit);
extern void SetFlagHoming(unsigned short mask, unsigned short flag);
extern unsigned short g_ASync_BarcodeJustage(unsigned short onoff, unsigned short output);
extern unsigned short g_ASync_ResetEstopMove(void);
extern unsigned short g_ASync_EStopMoveSavetyPos(void);
extern unsigned short g_ASync_EStopMoveRestorePos(void);
//extern unsigned short g_ASync_EStopMoveContinue(void);
extern unsigned short g_ASync_EStopMoveContinue(char *pszHippieAnswer);
extern unsigned short g_ASync_EStopRestoreCurrentAndSetPos(void);
//****************************************************************************
// @Prototypes Of Local Functions
//****************************************************************************

#endif
