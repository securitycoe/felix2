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
#ifndef _ASYNC_H_
#define _ASYNC_H_

//****************************************************************************
// include files
//****************************************************************************
#include "MDrive_CmdCtrl.h"
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
extern IMS_CommandBlock m_MsgASyncMot;
//****************************************************************************
// @External Prototypes
//****************************************************************************

//****************************************************************************
// Prototypen definitions
//****************************************************************************
//extern unsigned short m_Async_ImsCmdMoveMask(unsigned short Cmd, unsigned short Mask);
extern unsigned short m_Async_ImsCmdID(unsigned short Cmd, unsigned short ID);
extern unsigned short m_AsyncImsCmd_SetStallFactor(const unsigned short ID, unsigned short value);
extern unsigned short m_AsyncImsCmd_SetHoldCurrent(const unsigned short ID, unsigned short value);
extern unsigned short m_AsyncImsCmd_StopMotor(void);
extern unsigned short m_AsyncImsCmd_GetPosition(const unsigned short ID, long *value);
//extern unsigned short m_AsyncImsCmd_ResetPosition(const unsigned short ID);
//extern unsigned short m_AsyncImsCmd_SetDecToAcc(const unsigned short ID);
extern unsigned short m_AsyncImsCmd_SetSpeed(const unsigned short ID, float start, float end, float accel);
extern unsigned short m_AsyncImsCmd_SetPosition(const unsigned short ID, long value);

extern unsigned short m_GetAxisIDFromMask(unsigned short mask, unsigned short AxisBit);
extern unsigned short m_CheckMaskBit(unsigned short mask, unsigned short i);
extern void SetFlagHoming(unsigned short mask, unsigned short flag);

extern unsigned short ASync_GetRunningAllNew(unsigned short *mask);

extern void SetRunStatusRdyErr(unsigned short mask);
extern void SetRunStatusBusy(unsigned short mask);
//extern void SetErrorForMask(unsigned short mask, unsigned short error);
//extern void SetErrorForBit(unsigned short i, unsigned short error);

extern unsigned short ASync_ResetError(unsigned short mask);
extern unsigned short ASync_ResetHWError(unsigned short mask);

//extern unsigned short m_AsyncImsCmd_ResetError(const unsigned short ID);
//extern unsigned short m_AsyncImsCmd_ResetStallFlag(const unsigned short ID);
//extern unsigned short m_AsyncImsCmd_HomingEstop(unsigned short value);
extern unsigned short m_AsyncImsCmd_SetNormalModeAll(unsigned short mask);
extern unsigned short m_AsyncImsCmd_GetHomeSwitchID(const unsigned short ID, unsigned short *value);
extern unsigned short m_AsyncImsCmd_GetHomeSwitchMask(const unsigned short mask, unsigned short *value);

extern unsigned short m_Async_LPKGetSensorSwitchMagazinInsert(unsigned short *value);

extern unsigned short CheckDriveHomingOk(unsigned short mask);
extern unsigned short m_Async_Hippie_Safety(void);
extern unsigned short m_Async_Hippie_Restore(void);
//extern unsigned short m_Async_Hippie_Continue(void);
extern unsigned short m_Async_Hippie_Continue(char *pszAnswerString);

//****************************************************************************
// @Prototypes Of Local Functions
//****************************************************************************

#endif
