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
#ifndef _USERMAIN_H_
#define _USERMAIN_H_

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
extern const char FirmwareVersion[];
extern const char FW_Date[];
extern const char FW_Time[];
extern const char DeviceName[];
extern const char *const g_pStrCopyright;
extern const char DeviceNameUSB[];
extern unsigned short g_errArray[];
//****************************************************************************
// @External Prototypes
//****************************************************************************

//****************************************************************************
// Prototypen definitions
//****************************************************************************
//extern void TaskStart(void);
//extern void SetEstopIn(const udtDeviceConfig Config);
extern void SetEstopIn(void);
extern void SetLockEstopOut(const udtDeviceConfig Config);
extern unsigned short DataAndFRAMInit(void);
//****************************************************************************
// @Prototypes Of Local Functions
//****************************************************************************

#endif
