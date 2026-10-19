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
#ifndef _HARDWARE_H_
#define _HARDWARE_H_

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

//****************************************************************************
// @Typedefs
//****************************************************************************
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
extern udtDilGroup GetDilGroup(void);
//extern void ResetPort(void);
//extern unsigned short SetPortAdr(unsigned short Adr);
//extern unsigned short ClearPortAdr(unsigned short Value);

extern void SetPortBit(unsigned short Value);
extern void ClearPortBit(unsigned short Value);
extern void TestSetPortBit(unsigned short Value);
extern void TestClearPortBit(unsigned short Value);

extern unsigned short Switchers(unsigned short Ports, unsigned short Bits);
extern unsigned short ReadSensor(unsigned short Sensor, unsigned short *Value);

extern void Z_Breake_Free(void);
extern void Z_Breake_Fixed(void);

extern void SetDesignLightCCU63_CC60_P93(unsigned short val);
extern void SetDesignLightCCU63_CC61_P94(unsigned short val);
extern void SetDesignLightCCU63_CC62_P95(unsigned short val);
extern void SetDesignLightCCU63_CC63_P96(unsigned short val);

extern void xHW_LED_Red_On(void);
extern void xHW_LED_Green_On(void);
extern void xHW_LED_RedGreen_Off(void);
extern void xHW_LED_Yellow_On(void);
extern void xHW_LED_Reserve_On(void);
extern void xHW_LED_Yellow_Off(void);

extern void SensorAndPortPowerOnSetting(void);
extern void PortQuitSetting(void);

//#ifdef _CF_NRTL
extern void ResetFwEStopLock(void);
extern void SetFwEStopLock(void);
//#endif
//****************************************************************************
// @Prototypes Of Local Functions
//****************************************************************************
#endif
