//****************************************************************************
// @Module        Hardware
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

//****************************************************************************
// include files
//****************************************************************************
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <ctype.h>
#include "DevError.h"
#include "30-5015_Felix/XE167F/main.h"
#include "30-5015_Felix/XE167F/GPT2.h"
//#include "Shared/Stacker_Lifter_Allgemein/StacError.h"
#include "Cxconfig.h"
#include "DeviceConfig.h"
#include "Drv_Config.h"
#include "Felix_Misc.h"
#include "Felix_Hardware.h"
				   
//****************************************************************************
// local definitions
//****************************************************************************
//#define US_HardwareSlot	  128//256		//	Größe des CUSTACK
//#define LED_GREEN_PWM 0
//#define LED_RED_PWM 100

#define LED_ON_PWM 100
//#define LED_GREEN_0N_20PWM 20
#define LED_OFF_PWM 0

//****************************************************************************
// default settings
//****************************************************************************
//****************************************************************************
// external functions
//****************************************************************************

//****************************************************************************
// external data
//****************************************************************************

//****************************************************************************
// internal functions
//****************************************************************************
void WritePort(unsigned short Value);
void WritePortTest(unsigned short Value);
//void SetPWM_YellowLED(short Brightness, short Light);
//void SetPWM_RedGreenLED(short Brightness, short Light);
//void SetRedGreenLight(short value);
//void SetRedGreenBrightness(short value);
//void SetYellowLight(short value);
//void SetYellowBrightness(short value);

//****************************************************************************
// public data
//****************************************************************************
//unsigned char HardwareSlot;																									oo
unsigned short gDoppelTasteUI;
//unsigned short gNrtl_LP;
//****************************************************************************
// private data
//****************************************************************************
unsigned short m_Ports = 0;
unsigned short m_TestPorts = 0;

//short m_RedGreenBrightness = 0;
//short m_RedGreenLight = 0;
//short m_YellowBrightness = 0;
//short m_YellowLight = 0;
//****************************************************************************
// private functions
//****************************************************************************

void SetDesignLightCCU63_CC60_P93(unsigned short val)
{
	long help= (long)val*2578/100;
	CCU63_CC60SR= help;
	CCU63_TCTR4= CCU63_TCTR4|0x0040; // Enable Shadow-Register-Transfer
//printf("\rCC60: %i\r", help);fflush(stdout);
}
void SetDesignLightCCU63_CC61_P94(unsigned short val)
{
	long help= (long)val*2578/100;
	CCU63_CC61SR= help;
	CCU63_TCTR4= CCU63_TCTR4|0x0040; // Enable Shadow-Register-Transfer
//printf("\rCC61: %i\r", help);fflush(stdout);
}

void SetDesignLightCCU63_CC62_P95(unsigned short val)
{
	//Helligkeit
	int help= (long)val*2578/100;
	CCU63_CC62SR= help;
	CCU63_TCTR4= CCU63_TCTR4|0x0040; // Enable Shadow-Register-Transfer
//printf("\rCC62: %i\r", help);fflush(stdout);
}

void SetDesignLightCCU63_CC63_P96(unsigned short val)
{
	//Enable
	int help= (long)val*2578/100;
	CCU63_CC63SR= help;
	CCU63_TCTR4= CCU63_TCTR4|0x4000; // Enable Shadow-Register-Transfer
//printf("\rCC63: %i\r", help);fflush(stdout);
}

/*void SetPWM_YellowLED(short Brightness, short Light)
{
	long help_Brightness;
//	long help_Light;

	help_Brightness= Light;
	help_Brightness= (long)Brightness*2578/100;
//	help_Light= (long)Light*help_Brightness/100;
	CCU63_CC62SR= help_Brightness;
//	CCU63_CC63SR= help_Light;
	CCU63_TCTR4= CCU63_TCTR4|0x0040; // Enable Shadow-Register-Transfer
	m_YellowBrightness = Brightness;
//	m_YellowLight = Light;
}*/

/*void SetPWM_RedGreenLED(short Brightness, short Light)
{
	long help_Brightness;
	long help_Light;

	help_Brightness= (long)Brightness*2578/100;
	help_Light= (long)Light*help_Brightness/100;
	CCU63_CC60SR= help_Brightness;
	CCU63_CC61SR= help_Light;
	CCU63_TCTR4= CCU63_TCTR4|0x0040; // Enable Shadow-Register-Transfer
	m_RedGreenBrightness = Brightness;
	m_RedGreenLight = Light;
}*/

/*void SetYellowLight(short value)
{
	SetPWM_YellowLED(m_YellowBrightness, value);
	m_YellowLight = value;

}
void SetYellowBrightness(short value)
{
	SetPWM_YellowLED(value, m_YellowLight);
	m_YellowBrightness = value;
}*/

/*void SetRedGreenLight(short value)
{
	SetPWM_RedGreenLED(m_RedGreenBrightness, value);
	m_RedGreenLight = value;
}
void SetRedGreenBrightness(short value)
{
	SetPWM_RedGreenLED(value, m_RedGreenLight);
	m_RedGreenBrightness = value;
}*/


void xHW_LED_Red_On(void)
{
	//SetRedGreenLight(LED_RED_PWM);
	//SetRedGreenBrightness(LED_ON_PWM);
	// neu
	if(gDoppelTasteUI)
	{
		// für LP's 30-5015-101, 30-5015-102, 30-5015-103
		SetDesignLightCCU63_CC61_P94(LED_OFF_PWM);//LED4, Helligkeit
	}
	else
	{
		// für LP's 30-5015-001, 30-5015-002, 30-5015-003
		SetDesignLightCCU63_CC61_P94(LED_ON_PWM);//Helligkeit
	}
	SetDesignLightCCU63_CC60_P93(LED_ON_PWM);//LED4_EN, Enable
}

void xHW_LED_Green_On(void)
{
	//SetRedGreenLight(LED_GREEN_PWM);
	//SetRedGreenBrightness(LED_ON_PWM);
	// neu
	if(gDoppelTasteUI)
	{
		// für LP's 30-5015-101, 30-5015-102, 30-5015-103
		SetDesignLightCCU63_CC61_P94(LED_ON_PWM);//LED4, Helligkeit
	}
	else
	{
		// für LP's 30-5015-001, 30-5015-002, 30-5015-003
		SetDesignLightCCU63_CC61_P94(LED_OFF_PWM);//Helligkeit
	}
	SetDesignLightCCU63_CC60_P93(LED_ON_PWM);//LED4_EN, Enable

}

void xHW_LED_RedGreen_Off(void)
{
	//SetRedGreenBrightness(LED_OFF_PWM);
	// neu
	SetDesignLightCCU63_CC60_P93(LED_OFF_PWM);//LED4_EN, Enable
}

void xHW_LED_Yellow_On(void)
{
	//SetYellowLight(LED_ON_PWM);
	//SetYellowBrightness(LED_ON_PWM);
	// neu
	SetDesignLightCCU63_CC63_P96(LED_ON_PWM);//LED5, Helligkeit
	SetDesignLightCCU63_CC62_P95(LED_ON_PWM);//LED5_EN, Enable
}

void xHW_LED_Reserve_On(void)
{
	//SetYellowLight(LED_ON_PWM);
	//SetYellowBrightness(LED_ON_PWM);
	// neu
	SetDesignLightCCU63_CC63_P96(LED_OFF_PWM);//LED5, Helligkeit
	SetDesignLightCCU63_CC62_P95(LED_ON_PWM);//LED5_EN, Enable
}

void xHW_LED_Yellow_Off(void)
{
	//SetYellowBrightness(LED_OFF_PWM);
	// neu
	SetDesignLightCCU63_CC62_P95(LED_OFF_PWM);//LED5_EN, Enable    25.03.2023
	//SetDesignLightCCU63_CC63_P96(LED_OFF_PWM);//LED5, Helligkeit 25.03.2023
}

void WritePortTest(unsigned short Value)
{
//	PORT_ALLOC
	if(Value & Test_bitPORT_RESET_LANPORT)
	{
		if(m_TestPorts & Test_bitPORT_RESET_LANPORT)
		{RESET_LANPORT = 0;}else{RESET_LANPORT  = 1;}
	}
//#ifndef _CF_NRTL
	if(!isNRTL_LP())
	{
		if(Value & Test_bitPORT_CLEAR_FW_ESTOP)
		{
			if(m_TestPorts & Test_bitPORT_CLEAR_FW_ESTOP)
			{CLEAR_FW_ESTOP = 0;}else{CLEAR_FW_ESTOP  = 1;}
		}
		if(Value & Test_bitPORT_LOCK_ESTOP_EXTERN)
		{
			if(m_TestPorts & Test_bitPORT_LOCK_ESTOP_EXTERN)
			{LOCK_ESTOP_EXTERN = 0;}else {LOCK_ESTOP_EXTERN  = 1;}
		}
		if(Value & Test_bitPORT_DISABLE_ESTOP_INTERN)
		{
			if(m_TestPorts & Test_bitPORT_DISABLE_ESTOP_INTERN)
			{DISABLE_ESTOP_INTERN = 1;}else{DISABLE_ESTOP_INTERN  = 0;}
		}
		if(Value & Test_bitPORT_DISABLE_ESTOP_EXTERN)
		{
			if(m_TestPorts & Test_bitPORT_DISABLE_ESTOP_EXTERN)
			{DISABLE_ESTOP_EXTERN = 0;}else{DISABLE_ESTOP_EXTERN  = 1;}
		}
	}
//#endif
	if(Value & Test_bitPORT_LED_1)
	{
		if(m_TestPorts & Test_bitPORT_LED_1)
		{LED_1 = 1;}else{LED_1  = 0;}
	}
	if(Value & Test_bitPORT_LED_2)
	{
		if(m_TestPorts & Test_bitPORT_LED_2)
		{LED_2 = 1;}else{LED_2  = 0;}
	}
	if(Value & Test_bitPORT_LED_3)
	{
		if(m_TestPorts & Test_bitPORT_LED_3)
		{LED_3 = 1;}else{LED_3  = 0;}
	}
	if(Value & Test_bitPORT_U0C1_COM1_RE_485)
	{
		if(m_TestPorts & Test_bitPORT_U0C1_COM1_RE_485)
		{U0C1_COM1_RE_485 = 1;}else{U0C1_COM1_RE_485  = 0;}
	}
	if(Value & Test_bitPORT_U0C1_COM1_DE_485)
	{
		if(m_TestPorts & Test_bitPORT_U0C1_COM1_DE_485)
		{U0C1_COM1_DE_485 = 1;}else{U0C1_COM1_DE_485  = 0;}
	}
	if(Value & Test_bitPORT_U1C1_COM2_RTS2)
	{
		if(m_TestPorts & Test_bitPORT_U1C1_COM2_RTS2)
		{U1C1_COM2_RTS2 = 1;}else{U1C1_COM2_RTS2  = 0;}
	}
	if(Value & Test_bitPORT_U2C0_COM3_RTS3)
	{
		if(m_TestPorts & Test_bitPORT_U2C0_COM3_RTS3)
		{U2C0_COM3_RTS3 = 1;}else{U2C0_COM3_RTS3  = 0;}
	}
	if(Value & Test_bitPORT_U2C1_COM4_RTS4)
	{
		if(m_TestPorts & Test_bitPORT_U2C1_COM4_RTS4)
		{U2C1_COM4_RTS4 = 1;}else{U2C1_COM4_RTS4  = 0;}
	}
	if(Value & Test_bitPORT_FRAM_CS)
	{
		if(m_TestPorts & Test_bitPORT_FRAM_CS)
		{FRAM_CS = 1;}else{FRAM_CS  = 0;}
	}
//#ifndef _CF_NRTL
	if(!isNRTL_LP())
	{
		if(Value & Test_bitPORT_SET_FF)
		{
			if(m_TestPorts & Test_bitPORT_SET_FF)
			{SET_FF = 1;}else{SET_FF  = 0;}
		}
	}
//#endif
//	PORT_RELEASE
}

void WritePort(unsigned short Value)
{
//	PORT_ALLOC
	if(Value & bitPORT_SW_RESERVE)
	{
		if(m_Ports & bitPORT_SW_RESERVE)
		{SW_RESERVE = 0;}else{SW_RESERVE  = 1;}
	}
	/*if(Value & bitPORT_Z_BRAKE)
	{
		if(m_Ports & bitPORT_Z_BRAKE)
		{Z_BRAKE = 0;}else{Z_BRAKE  = 1;}
	}*/
	if(Value & bitPORT_SET_PHOTOMOS)
	{
		if(m_Ports & bitPORT_SET_PHOTOMOS)
		{SET_PHOTOMOS = 0;}
		else
		{SET_PHOTOMOS  = 1;}
	}
	if(Value & bitPORT_SET_VCC_OEM)
	{
		if(m_Ports & bitPORT_SET_VCC_OEM)
		{SET_VCC_OEM = 1;}
		else
		{SET_VCC_OEM  = 0;}
	}
	if(Value & bitPORT_SET_VCC_UVL)
	{
		if(m_Ports & bitPORT_SET_VCC_UVL)
		{SET_VCC_UVL = 1;}
		else
		{SET_VCC_UVL  = 0;}
	}
	if(Value & bitPORT_SET_SW_UVL)
	{
		if(m_Ports & bitPORT_SET_SW_UVL)
		{SET_SW_UVL = 1;}
		else
		{SET_SW_UVL  = 0;}
	}
	if(Value & bitPORT_MUELL_CTRL)
	{
		if(m_Ports & bitPORT_MUELL_CTRL)
		{MUELL_CTRL = 0;}
		else
		{MUELL_CTRL  = 1;}
	}
	if(Value & bitPORT_SET_OEM_IN)
	{
		if(m_Ports & bitPORT_SET_OEM_IN)
		{SET_OEM_IN = 1;}
		else
		{SET_OEM_IN  = 0;}
	}
	if(Value & bitPORT_SET_UVL_IN)
	{
		if(m_Ports & bitPORT_SET_UVL_IN)
		{SET_UVL_IN = 1;}
		else
		{SET_UVL_IN  = 0;}
	}
//	PORT_RELEASE
}

//****************************************************************************
// public functions
//****************************************************************************
/*void ResetPort(void)
{
	PORT_ALLOC
	m_Ports = 0;
	WritePort(0);
	PORT_RELEASE
}*/
void SetPortBit(unsigned short Value)
{
	PORT_ALLOC
	m_Ports |= Value;
	WritePort(Value);
	PORT_RELEASE
}

void ClearPortBit(unsigned short Value)
{
	PORT_ALLOC
	m_Ports &= ~Value;
	WritePort(Value);
	PORT_RELEASE
}
void TestSetPortBit(unsigned short Value)
{
	//TESTPORT_ALLOC
	m_TestPorts |= Value;
	WritePortTest(Value);
	//TESTPORT_RELEASE
}

void TestClearPortBit(unsigned short Value)
{
	//TESTPORT_ALLOC
	m_TestPorts &= ~Value;
	WritePortTest(Value);
	//TESTPORT_RELEASE
}


unsigned short Switchers(unsigned short Ports, unsigned short Bits)
{
	if((Ports > bitPORT_MAX) || (Bits > bitPORT_MAX)) return ERR_WRONG_PARAMETER;
	if(!gDoppelTasteUI) return ERR_CMD_NOT_IMPLEMENTED;
	if(Ports & bitPORT_SET_PHOTOMOS)
	{
		if(Bits & bitPORT_SET_PHOTOMOS)
		 {SetPortBit(bitPORT_SET_PHOTOMOS);}
		else
		 {ClearPortBit(bitPORT_SET_PHOTOMOS);}
	}
	if(Ports & bitPORT_SET_VCC_OEM)
	{
		if(Bits & bitPORT_SET_VCC_OEM)
		 {SetPortBit(bitPORT_SET_VCC_OEM);}
		else
		 {ClearPortBit(bitPORT_SET_VCC_OEM);}
	}
	if(Ports & bitPORT_SET_SW_UVL)
	{
		if(Bits & bitPORT_SET_SW_UVL)
		 {SetPortBit(bitPORT_SET_SW_UVL);}
		else
		 {ClearPortBit(bitPORT_SET_SW_UVL);}
	}
	if(Ports & bitPORT_SET_VCC_UVL)
	{
		if(Bits & bitPORT_SET_VCC_UVL)
		 {SetPortBit(bitPORT_SET_VCC_UVL);}
		else
		 {ClearPortBit(bitPORT_SET_VCC_UVL);}
	}
	if(Ports & bitPORT_MUELL_CTRL)
	{
		if(Bits & bitPORT_MUELL_CTRL)
		 {SetPortBit(bitPORT_MUELL_CTRL);}
		else
		 {ClearPortBit(bitPORT_MUELL_CTRL);}
	}
	if(Ports & bitPORT_SET_OEM_IN)
	{
		if(Bits & bitPORT_SET_OEM_IN)
		 {SetPortBit(bitPORT_SET_OEM_IN);}
		else
		 {ClearPortBit(bitPORT_SET_OEM_IN);}
	}
	if(Ports & bitPORT_SET_UVL_IN)
	{
		if(Bits & bitPORT_SET_UVL_IN)
		 {SetPortBit(bitPORT_SET_UVL_IN);}
		else
		 {ClearPortBit(bitPORT_SET_UVL_IN);}
	}
	if(Ports & bitPORT_SW_RESERVE)
	{
		if(Bits & bitPORT_SW_RESERVE)
		 {SetPortBit(bitPORT_SW_RESERVE);}
		else
		 {ClearPortBit(bitPORT_SW_RESERVE);}
	}
	return errNone;
}

unsigned short ReadSensor(unsigned short Sensor, unsigned short *Value)
{
	unsigned short error = errNone;
	if(!gDoppelTasteUI) return ERR_CMD_NOT_IMPLEMENTED;
	switch(Sensor)
	{
		case SENSOR_GET_OEM_IN1:	*Value= GET_OEM_IN;
									break;
		case SENSOR_GET_UVL_IN:		*Value= GET_UVL_IN;
									break;
		case SENSOR_MUELL_IN:   	*Value= MUELL_IN;
									break;
		default: 	error= ERR_WRONG_PARAMETER;
	}
	return error;
}

void SensorAndPortPowerOnSetting(void)
{
	SET_OEM_IN = g_SensorConfig.SetPort_OEM_IN;
	SET_UVL_IN = g_SensorConfig.SetPort_UVL_IN;
	SET_PHOTOMOS = g_PortConfig.SetPort_PHOTOMOS.PowerOn;
	SET_VCC_OEM = g_PortConfig.SetPort_VCC_OEM.PowerOn;
	SET_SW_UVL = g_PortConfig.SetPort_SW_UVL.PowerOn;
	SET_VCC_UVL = g_PortConfig.SetPort_VCC_UVL.PowerOn;
	MUELL_CTRL = g_PortConfig.SetPort_MUELL_CTRL.PowerOn;
	//SET_OEM_IN = g_PortConfig.SetPort_OEM_IN.PowerOn;
	//SET_UVL_IN = g_PortConfig.SetPort_UVL_IN.PowerOn;
	SW_RESERVE = g_PortConfig.SetPort_SW_RESERVE.PowerOn;
}
	
void PortQuitSetting(void)
{
	if(g_PortConfig.SetPort_PHOTOMOS.StateAfterQuit)
	{
		SET_PHOTOMOS = g_PortConfig.SetPort_PHOTOMOS.PowerOn;
	}
	if(g_PortConfig.SetPort_VCC_OEM.StateAfterQuit)
	{
		SET_VCC_OEM = g_PortConfig.SetPort_VCC_OEM.PowerOn;
	}
	if(g_PortConfig.SetPort_SW_UVL.StateAfterQuit)
	{
		SET_SW_UVL = g_PortConfig.SetPort_SW_UVL.PowerOn;
	}
	if(g_PortConfig.SetPort_VCC_UVL.StateAfterQuit)
	{
		SET_VCC_UVL = g_PortConfig.SetPort_VCC_UVL.PowerOn;
	}
	if(g_PortConfig.SetPort_MUELL_CTRL.StateAfterQuit)
	{
		MUELL_CTRL = g_PortConfig.SetPort_MUELL_CTRL.PowerOn;
	}
	if(g_PortConfig.SetPort_SW_RESERVE.StateAfterQuit)
	{
		SW_RESERVE = g_PortConfig.SetPort_SW_RESERVE.PowerOn;
	}
}

/*unsigned short SetPortAdr(unsigned short Adr)
{
	unsigned short error = errNone;
	switch(Adr)
	{
		case PORT_Z_BRAKE				:	SetPortBit(bitPORT_Z_BRAKE);
											break;
		case PORT_SW_RESERVE			:	SetPortBit(bitPORT_SW_RESERVE);
											break;
		case PORT_RESET_LANPORT			:	SetPortBit(bitPORT_RESET_LANPORT);
											break;
		case PORT_CLEAR_FW_ESTOP		:	SetPortBit(bitPORT_CLEAR_FW_ESTOP);
											break;
		case PORT_LOCK_ESTOP_EXTERN		:	SetPortBit(bitPORT_LOCK_ESTOP_EXTERN);
											break;
		case PORT_DISABLE_ESTOP_INTERN	:	SetPortBit(bitPORT_DISABLE_ESTOP_INTERN);
											break;
		case PORT_DISABLE_ESTOP_EXTERN	:	SetPortBit(bitPORT_DISABLE_ESTOP_EXTERN);
											break;
		case PORT_LED_1					:	SetPortBit(bitPORT_LED_1);
											break;
		case PORT_LED_2					:	SetPortBit(bitPORT_LED_2);
											break;
		case PORT_LED_3					:	SetPortBit(bitPORT_LED_3);
											break;
		case PORT_U0C1_COM1_RE_485		:	SetPortBit(bitPORT_U0C1_COM1_RE_485);
											break;
		case PORT_U0C1_COM1_DE_485		:	SetPortBit(bitPORT_U0C1_COM1_DE_485);
											break;
		case PORT_U1C1_COM2_RTS2		:	SetPortBit(bitPORT_U1C1_COM2_RTS2);
											break;
		case PORT_U2C0_COM3_RTS3		:	SetPortBit(bitPORT_U2C0_COM3_RTS3);
											break;
		case PORT_U2C1_COM4_RTS4		:	SetPortBit(bitPORT_U2C1_COM4_RTS4);
											break;
		case PORT_FRAM_CS				:	FRAM_CS = 1;
											break;
		 default						:	error = ERR_WRONG_PARAMETER;
											break;
	}
	return error;
}

unsigned short ClearPortAdr(unsigned short Adr)
{
	unsigned short error = errNone;
	switch(Adr)
	{
		case PORT_Z_BRAKE				:	ClearPortBit(bitPORT_Z_BRAKE);
											break;
		case PORT_SW_RESERVE			:	ClearPortBit(bitPORT_SW_RESERVE);
											break;
		case PORT_RESET_LANPORT			:	ClearPortBit(bitPORT_RESET_LANPORT);
											break;
		case PORT_CLEAR_FW_ESTOP		:	ClearPortBit(bitPORT_CLEAR_FW_ESTOP);
											break;
		case PORT_LOCK_ESTOP_EXTERN	  	:	ClearPortBit(bitPORT_LOCK_ESTOP_EXTERN);
											break;
		case PORT_DISABLE_ESTOP_INTERN	:	ClearPortBit(bitPORT_DISABLE_ESTOP_INTERN);
											break;
		case PORT_DISABLE_ESTOP_EXTERN	:	ClearPortBit(bitPORT_DISABLE_ESTOP_EXTERN);
											break;
		case PORT_LED_1					:	ClearPortBit(bitPORT_LED_1);
											break;
		case PORT_LED_2					:	ClearPortBit(bitPORT_LED_2);
											break;
		case PORT_LED_3					:	ClearPortBit(bitPORT_LED_3);
											break;
		case PORT_U0C1_COM1_RE_485		:	ClearPortBit(bitPORT_U0C1_COM1_RE_485);
											break;
		case PORT_U0C1_COM1_DE_485		:	ClearPortBit(bitPORT_U0C1_COM1_DE_485);
											break;
		case PORT_U1C1_COM2_RTS2		:	ClearPortBit(bitPORT_U1C1_COM2_RTS2);
											break;
		case PORT_U2C0_COM3_RTS3		:	ClearPortBit(bitPORT_U2C0_COM3_RTS3);
											break;
		case PORT_U2C1_COM4_RTS4		:	ClearPortBit(bitPORT_U2C1_COM4_RTS4);
											break;
		case PORT_FRAM_CS				:	FRAM_CS = 0;
											break;
		 default						:	error = ERR_WRONG_PARAMETER;
											break;
	}
	return error;
}*/

void Z_Breake_Free(void)
{
	//SetPortBit(bitPORT_Z_BRAKE);
	Z_BRAKE = 0;
	K_Task_Wait(10);
}

void Z_Breake_Fixed(void)
{
	//ClearPortBit(bitPORT_Z_BRAKE);
	Z_BRAKE  = 1;
	K_Task_Wait(10);
}

udtDilGroup GetDilGroup(void)
{
	unsigned short i;
	unsigned short Dil11 = 0;
	unsigned short Dil12 = 0;
	unsigned short Dil13 = 0;
	unsigned short Dil14 = 0;
	udtDilGroup ReturnValue = eDil1None;
	for(i = 1; i <= COUNT_READ_KEY; i++)
	{
		WDO();
		K_Task_Wait(2);
		if(!DIL1_1) {Dil11++;}
		if(!DIL1_2) {Dil12++;}
		if(!DIL1_3) {Dil13++;}
		if(!DIL1_4) {Dil14++;}
	}
	if(Dil11 >= COUNT_READ_KEY){ReturnValue |= eDil1_1;}
	if(Dil12 >= COUNT_READ_KEY){ReturnValue |= eDil1_2;}
	if(Dil13 >= COUNT_READ_KEY){ReturnValue |= eDil1_3;}
	if(Dil14 >= COUNT_READ_KEY){ReturnValue |= eDil1_4;}

	return ReturnValue;
}

//#ifdef _CF_NRTL
void ResetFwEStopLock(void)
{
	ESTOP_ALLOC
	FW_LOCK_UP = 1;
	K_Task_Wait(40);
	FW_LOCK_DN = 1;	// Einbruch
	ClearFlagFwEStopLock();
/*	K_Task_Wait(10);//Laden
	FW_LOCK_DN = 0;
	K_Task_Wait(50);
	FW_LOCK_DN = 1;
*/
	ESTOP_RELEASE
}
void SetFwEStopLock(void)
{
	ESTOP_ALLOC
	FW_LOCK_UP = 0;FW_LOCK_DN = 0;
	SetFlagFwEStopLock();
	ESTOP_RELEASE
}
//#endif
