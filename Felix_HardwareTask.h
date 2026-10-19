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
#ifndef _HARDWARETASK_H_
#define _HARDWARETASK_H_

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
#define DRV_MODE_ESTOP 0
#define DRV_MODE_NORMAL 1
//****************************************************************************
// @Typedefs
//****************************************************************************
//enum HardwareCommandCode
typedef enum
{
	cHWNone = 0
	,cLED_Red_On
	,cLED_Red_Off
	,cLED_Green_On
	,cLED_Green_Off
//	,cLED_Green_Blink
	,cLED_Green_SlowBlink
	,cLED_Green_FastBlink
	,cLED_Green_SuperFastBlink
	,cLED_RedGreen_Off
	,cLED_Yellow_On
	,cLED_Yellow_Off
//	,cLED_Yellow_Blink
	,cLED_Yellow_SlowBlink
	,cLED_Yellow_FastBlink
	,cLED_Yellow_SuperFastBlink
	,cLED_SystemReady
	,cEquipmentCtrl
	,cEquipmentTime
	,cToolTest
//	,cFetTestEStop
	,cCheckMOSFETSwitch
	,cCheckEstopState

}udtHardwareCommandCode;
//};
typedef	struct
{
	//unsigned short Cmd;
	udtHardwareCommandCode eCommand;
	unsigned short Error;
	unsigned short Parameter1;
	unsigned short Parameter2;
} HardwareCommandBlock;

//****************************************************************************
// @Imported Global Variables
//****************************************************************************

//****************************************************************************
// @Global Variables
//****************************************************************************
//extern unsigned char HardwareSlot;
//****************************************************************************
// @External Prototypes
//****************************************************************************

//****************************************************************************
// Prototypen definitions
//****************************************************************************
//extern void HardwareTask_vInit(void);
extern void HardwareTask(void);

extern void LED_Red_On(void);
extern void LED_Red_Off(void);

extern void LED_Green_On(void);
extern void LED_Green_Off(void);
extern void LED_Green_SLowBlink(void);
extern void LED_Green_FastBlink(void);
extern void LED_Green_SuperFastBlink(void);

extern void LED_RedGreen_Off(void);

extern void LED_Yellow_On(void);
extern void LED_Yellow_Off(void);
extern void LED_Yellow_SlowBlink(void);
extern void LED_Yellow_FastBlink(void);
extern void LED_Yellow_SuperFastBlink(void);
extern void LED_SystemReady(void);

//extern void SetDriveMode(unsigned short mode);
//extern unsigned int GetDriveMode(void);


extern unsigned short gRedOnFlag(void);
extern unsigned short HardwareEquipmentCtrl(unsigned short EquipmentCmd, unsigned short EquipmentAdr, unsigned short Control);
//#ifdef _CF_NRTL
//extern unsigned short MsgFetTestEStop(void);
extern unsigned short g_MsgCheckMOSFETSwitch(void);
//#endif

extern unsigned short g_MsgCheckEstopState(unsigned short *State);
//****************************************************************************
// @Prototypes Of Local Functions
//****************************************************************************

#endif
