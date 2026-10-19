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
#ifndef _ASYNCREPOS_H_
#define _ASYNCREPOS_H_

//****************************************************************************
// include files
//****************************************************************************

//****************************************************************************
// @Macros
//****************************************************************************

//****************************************************************************
// @Defines
//****************************************************************************
/*#define RESPOS_NONE 0
#define RESPOS_START 41
#define RESPOS_REPOS_ALL_START 42
#define RESPOS_REPOS_ALL_WAIT 43
#define RESPOS_REPOS_Z_HOMEPOS_START 44
#define RESPOS_REPOS_Z_HOMEPOS_WAIT 45
#define RESPOS_REPOS_XY_START 46
#define RESPOS_REPOS_XY_WAIT 47
#define RESPOS_REPOS_Z_START 47
#define RESPOS_REPOS_Z_WAIT 49
#define RESPOS_MOVE_CONTINUE_START 50
#define RESPOS_MOVE_CONTINUE_WAIT 51
*/

//****************************************************************************
// @Typedefs
//****************************************************************************
//****************************************************************************
// @Imported Global Variables
//****************************************************************************

//****************************************************************************
// @Global Variables
//****************************************************************************
/* nicht löschen, für neue variante
extern unsigned short m_ResPosActive;
extern unsigned short m_ResPos_State;
*/
//****************************************************************************
// @External Prototypes
//****************************************************************************

//****************************************************************************
// Prototypen definitions
//****************************************************************************
/* 		/* nicht löschen, für neue variante
extern unsigned short ASync_MoveRestore(void);
*/
extern unsigned short ASync_EStopMoveSavetyPos(void);
extern unsigned short ASync_EStopMoveRestorePos(void);
extern unsigned short ASync_EStopRestoreCurrentAndSetPos(void);
extern unsigned short ASync_EStopContinue(void);
//****************************************************************************
// @Prototypes Of Local Functions
//****************************************************************************

#endif