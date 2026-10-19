//****************************************************************************
// @Module        Kommandointerpreter
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
#ifndef _FELIX_CMD_H_
#define _FELIX_CMD_H_

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
extern unsigned short m_EStopCmdState;
//****************************************************************************
// Prototypen definitions
//****************************************************************************
//void CmdExecution(unsigned char cmd, void (*fp)(unsigned short));
//extern unsigned short EStopCommandHandling(unsigned short *ptrContinueCmd, FILE *InHandle, FILE *OutHandle);
extern unsigned short EStopCommandHandling(unsigned short *ptrContinueCmd, FILE *InHandle, FILE *OutHandle, char *pszHippieAnswer);
extern unsigned short CommandFelix(FILE *InHandle, FILE *OutHandle);
extern unsigned short g_Cmd_ASync_ResetEstopMove(void);
//****************************************************************************
// @Prototypes Of Local Functions
//****************************************************************************

#endif
