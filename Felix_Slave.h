//****************************************************************************
// @Module        Slave
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
#ifndef _FELIX_SLAVE_H_
#define _FELIX_SLAVE_H_

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
extern unsigned short CommandSlave(unsigned short Adr, char *pAnsw);
extern unsigned short SendToSlave(unsigned short Adr, char *pSend, char *pAnsw);
extern unsigned short isSlavePresent(void);
extern void SendBreakToSlave(void);
extern unsigned short ResetBarcodeDataForSlave(char *pAnsw);
extern unsigned short AnswerFromSlave(unsigned short Adr, char *pAnsw);
extern unsigned short SendToSlavePCCmd(unsigned short Adr, char *pSend);
//****************************************************************************
// @Prototypes Of Local Functions
//****************************************************************************

#endif
