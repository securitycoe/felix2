
// @Module        Main-Modul
// @Filename      UserMain.C
// @Project       Lifter
//----------------------------------------------------------------------------
// @Controller    Infineon XE167F-96F66
//
// @Compiler      Tasking Classic
//
// @Description   This file contains functions that use the user main module.
//
//****************************************************************************



//****************************************************************************
// @Project Includes
//****************************************************************************
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include "DevError.h"
#include "30-5015_Felix/XE167F/main.h"
//#include "Shared/Stacker_Lifter_Allgemein/StacError.h"
//#include "Shared/Stacker_Lifter_Allgemein/StacComm.h"
#include "DeviceConfig.h"
#include "SPI_XE167.h"
#include "FRAM_SPI_V1.H"
#include "Felix_Hardware.h"
#include "Felix_CmdCtrl.h"
#include "Felix_Cmd.h"
#include "Cxconfig.h"
#include "MDrive_CmdCtrl.h"
#include "MDriveCmd.h"
#include "Felix_Hardware.h"
#include "Drv_config.h"
#include "Felix_Misc.h"
#include "Felix_FRAM.h"
#include "Felix_UserMain.h"
#include "Felix_HardwareTask.h"
#include "Felix_CmdParallel.h"
#include "Felix_DoorLifter.h"
#include "LPK.h"


extern unsigned char UserMainSlot;
extern unsigned char MotorSlot;
extern unsigned short gDoppelTasteUI;
extern void EnableOutRS485_BUS(void);
#ifdef _CF_FELIX
const char DeviceNameUSB[] = "CyBi-FeliX";
const char DeviceName[] = "CyBi FeliX";
#endif
#ifdef _CF_PENTA_DISK
const char DeviceNameUSB[] = "CyBi-PentaDisk";
const char DeviceName[] = "CyBi-PentaDisk";
#endif
const char *const g_pStrCopyright = "Copyright by Analytik Jena AG last modification:";
const char FirmwareVersion[] = { "V04.64.00" };

const char FW_Date[] = { ""__DATE__"" };
const char FW_Time[] = { ""__TIME__"" };

unsigned short g_errArray[MAX_AXES + MAX_CNT_ISTATE];
unsigned short g_errCnt = 0;
unsigned short m_RedOn = 0;
//ubyte Array[16]= {0xAA,0xFF,0x01,0x02,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01};
//ubyte Dest[16];
//unsigned short gPCBTest = 1;

unsigned short DeviceInit(void);
unsigned short DeviceInitPCBTest(void);
unsigned short DataReset(void);
unsigned short DevInit_SetFRAMModify(unsigned short *ptrFRAM_Modify);
unsigned short FRAMVersion_AnhebenUndFehlendeDatenSpeichern(unsigned short FRAM_Modify);

void SetUILedBefore(unsigned short error);
void SetUILedAfter(unsigned short error);


void UserMain(void)	  
{
	unsigned short events, ComEvents;
	unsigned short error = errNone;
//	unsigned short testerror = errNone;
	unsigned short i;
	FILE *handle;
//g_TestFlag = TF_DBG_HW_EVENT | TF_DBG_EVENT | TF_DBG_INT_COM | TF_DBG_BREAKE | TF_DBG_HW | TF_DBG_CTRLCMD;
//g_TestFlag = TF_DBG_FRAM;
//g_TestFlag = TF_DBG_INT_COM | TF_DBG_EVENT | TF_DBG_485;
//g_TestFlag = TF_DBG_485 | TF_DBG_EVENT | TF_DBG_MOTTSK | TF_DBG_ASYNC | TF_DBG_HW | TF_DBG_BREAKE | TF_DBG_CMD | TF_DBG_INT_COM;
//g_TestFlag = TF_DBG_EVENT;
//g_TestFlag = TF_DBG_CALC_STEPS;
//g_TestFlag = TF_DBG_HW | TF_DBG_HW_EVENT | TF_DBG_485;
//g_TestFlag = TF_DBG_485 | TF_DBG_485_ERR;
g_TestFlag = 0;
//g_TestFlag = TF_DBG_485_ERR;
//g_TestFlag = TF_DBG_ESTOP_CMD;
	WDO();
	g_bBreakFlag = FALSE;
	K_Task_Wait(1000);
	EnableOutRS485_BUS();

	for(i = 0; i < (MAX_AXES + MAX_CNT_ISTATE); i++)
	{
		WDO();
		g_errArray[i] = errNone;
	}
#ifdef _CF_MAINPCB_JQV_FW_TYPE_002
//if(gPCBTest){
	DeviceInitPCBTest();
//}
#else
//else
//{
	if(DeviceInit() != errNone)
	{
		LED_Red_On();
		SetDeviceInitError();
	}
	else
	{
		LED_Green_On();
		ClearDeviceInitError();
	}
//}
#endif
	INT_vEnableInterrupt(CC2_CH24_INT);
	events = K_Event_Wait(ESTOP_EVENT, 50, 2);
	if(!(events & ESTOP_EVENT))
	{
		if(!ESTOP)	// low active
		{
			K_Event_Signal(3, 0, ESTOP_EVENT);
		}
		K_Task_Wait(10);
	}
	ClearMode1536();
	ParallelCmdClear();
	LED_SystemReady();

	if(GetLPKConnected())
	{
		SetLPKConnect();
	}
	else
	{
		ClearLPKConnect();
	}
while(1)
{
	WDO();

	events = K_Event_Wait(EVENT_MASTER_COM | EVENT_USER_BREAK, 10, 0 );	// löscht danach die angegebenen Events
	if(events)
	{
		if(events & EVENT_MASTER_COM)
		{
			K_Event_Reset(UserMainSlot, EVENT_MASTER_COM);
		}
		if(events & EVENT_USER_BREAK)
		{
			K_Event_Reset(UserMainSlot, EVENT_USER_BREAK);
		}
		K_Task_Wait(10);
		handle = stdin;
		ComEvents = UART_GetEvents(handle);
		if(ComEvents)
		{
			//printf("STDIN\r");fflush(stdout);
			SetUILedBefore(error);
			//dbPrintHost("STDIN\r");
			error = CommandIdent(ComEvents, handle);
			SetUILedAfter(error);
		}
		handle = COM2_USB_U1C1_IN;
		ComEvents = UART_GetEvents(handle);
		if(ComEvents)
		{
			//printf("COM2_USB_U1C1_IN\r");fflush(stdout);
			SetUILedBefore(error);
			//dbPrintHost("COM2_USB_U1C1_IN\r");
			error = CommandIdent(ComEvents, handle);
			SetUILedAfter(error);
		}
		handle = COM3_LAN_U2C0_IN;
		ComEvents = UART_GetEvents(handle);
		if(ComEvents)
		{
			//printf("COM2_USB_U1C1_IN\r");fflush(stdout);
			SetUILedBefore(error);
			//dbPrintHost("COM3_LAN_U2C0_IN\r");
			error = CommandIdent(ComEvents, handle);
			SetUILedAfter(error);
		}
	}//end...if(events)
}//end.. while(1)
}
void SetUILedBefore(unsigned short error)
{
	if((error!= errNone) && (error!= errInfo) && (error!= errInfoLPK) && (error!= (unsigned short)ERR_SAFETY_APPLIANCE) && (error!= errSync))
	{
		LED_RedGreen_Off();
		if(isHostMode()){LED_Green_SLowBlink();}else {LED_Green_On();}
	}

//HW_LED_Green_Busy();

}
void SetUILedAfter(unsigned short error)
{

	if((error!= errNone) && (error!= errInfo) && (error!= errInfoLPK) && (error!= (unsigned short)ERR_SAFETY_APPLIANCE) && (error!= errSync))
	{
		LED_Red_On();
	}
	else
	{
		if(gRedOnFlag())
		{
			LED_RedGreen_Off();
			if(isHostMode()){LED_Green_SLowBlink();}else {LED_Green_On();}
		}
	}
}
unsigned short DevInit_SetFRAMModify(unsigned short *ptrFRAM_Modify)
{
	unsigned short error;
	unsigned short Act_FramVersion;
	unsigned short FW_FramVersion;

	if((error = FRAMReadStructVersion(&Act_FramVersion)) == errNone)
	{
		if(Act_FramVersion != FRAM_STRUCT_VERSION)
		{
			FW_FramVersion = FRAM_STRUCT_VERSION;
			*ptrFRAM_Modify = 0;
			/*if((Act_FramVersion == 1) && (FW_FramVersion == 2))
			{
				FRAM_Modify = istate_FRAMAutomaticChange_V1_to_V2;
			}*/
			if((Act_FramVersion == 1) && (FW_FramVersion == 3))
			{
				*ptrFRAM_Modify = istate_FRAMAutomaticChange_V1_to_V3;
			}
			if((Act_FramVersion == 1) && (FW_FramVersion == 4))
			{
				*ptrFRAM_Modify = istate_FRAMAutomaticChange_V1_to_V4;
			}
			if((Act_FramVersion == 1) && (FW_FramVersion == 5))
			{
				*ptrFRAM_Modify = istate_FRAMAutomaticChange_V1_to_V5;
			}
			if((Act_FramVersion == 1) && (FW_FramVersion == 6))
			{
				*ptrFRAM_Modify = istate_FRAMAutomaticChange_V1_to_V6;
			}
			if((Act_FramVersion == 1) && (FW_FramVersion == 7))
			{
				*ptrFRAM_Modify = istate_FRAMAutomaticChange_V1_to_V7;
			}
			if((Act_FramVersion == 1) && (FW_FramVersion == 8))
			{
				*ptrFRAM_Modify = istate_FRAMAutomaticChange_V1_to_V8;
			}

			if((Act_FramVersion == 2) && (FW_FramVersion == 3))
			{
				*ptrFRAM_Modify = istate_FRAMAutomaticChange_V2_to_V3;
			}
			if((Act_FramVersion == 2) && (FW_FramVersion == 4))
			{
				*ptrFRAM_Modify = istate_FRAMAutomaticChange_V2_to_V4;
			}
			if((Act_FramVersion == 2) && (FW_FramVersion == 5))
			{
				*ptrFRAM_Modify = istate_FRAMAutomaticChange_V2_to_V5;
			}
			if((Act_FramVersion == 2) && (FW_FramVersion == 6))
			{
				*ptrFRAM_Modify = istate_FRAMAutomaticChange_V2_to_V6;
			}
			if((Act_FramVersion == 2) && (FW_FramVersion == 7))
			{
				*ptrFRAM_Modify = istate_FRAMAutomaticChange_V2_to_V7;
			}
			if((Act_FramVersion == 2) && (FW_FramVersion == 8))
			{
				*ptrFRAM_Modify = istate_FRAMAutomaticChange_V2_to_V8;
			}

			if((Act_FramVersion == 3) && (FW_FramVersion == 4))
			{	
				*ptrFRAM_Modify = istate_FRAMAutomaticChange_V3_to_V4;
			}
			if((Act_FramVersion == 3) && (FW_FramVersion == 5))
			{
				*ptrFRAM_Modify = istate_FRAMAutomaticChange_V3_to_V5;
			}
			if((Act_FramVersion == 3) && (FW_FramVersion == 6))
			{
				*ptrFRAM_Modify = istate_FRAMAutomaticChange_V3_to_V6;
			}
			if((Act_FramVersion == 3) && (FW_FramVersion == 7))
			{
				*ptrFRAM_Modify = istate_FRAMAutomaticChange_V3_to_V7;
			}
			if((Act_FramVersion == 3) && (FW_FramVersion == 8))
			{
				*ptrFRAM_Modify = istate_FRAMAutomaticChange_V3_to_V8;
			}

			if((Act_FramVersion == 4) && (FW_FramVersion == 5))
			{
				*ptrFRAM_Modify = istate_FRAMAutomaticChange_V4_to_V5;
			}
			if((Act_FramVersion == 4) && (FW_FramVersion == 6))
			{
				*ptrFRAM_Modify = istate_FRAMAutomaticChange_V4_to_V6;
			}
			if((Act_FramVersion == 4) && (FW_FramVersion == 7))
			{
				*ptrFRAM_Modify = istate_FRAMAutomaticChange_V4_to_V7;
			}
			if((Act_FramVersion == 4) && (FW_FramVersion == 8))
			{
				*ptrFRAM_Modify = istate_FRAMAutomaticChange_V4_to_V8;
			}

			if((Act_FramVersion == 5) && (FW_FramVersion == 6))
			{
				*ptrFRAM_Modify = istate_FRAMAutomaticChange_V5_to_V6;
			}
			if((Act_FramVersion == 5) && (FW_FramVersion == 7))
			{
				*ptrFRAM_Modify = istate_FRAMAutomaticChange_V5_to_V7;
			}
			if((Act_FramVersion == 5) && (FW_FramVersion == 8))
			{
				*ptrFRAM_Modify = istate_FRAMAutomaticChange_V5_to_V8;
			}

			if((Act_FramVersion == 6) && (FW_FramVersion == 7))
			{
				*ptrFRAM_Modify = istate_FRAMAutomaticChange_V6_to_V7;
			}
			if((Act_FramVersion == 6) && (FW_FramVersion == 8))
			{
				*ptrFRAM_Modify = istate_FRAMAutomaticChange_V6_to_V8;
			}

			if((Act_FramVersion == 7) && (FW_FramVersion == 8))
			{
				*ptrFRAM_Modify = istate_FRAMAutomaticChange_V7_to_V8;
			}
			//if(FRAM_Modify == 0){g_errArray[g_errCnt++] = errorReturn = istate_FRAMStructVersion;}
		}
		else
		{
			*ptrFRAM_Modify = istate_FRAMAutomaticChange_NoChange;
		}
	}
	return error;
}

unsigned short FRAMVersion_AnhebenUndFehlendeDatenSpeichern(unsigned short FRAM_Modify)
{
	unsigned short error = errNone;
	unsigned short iStateReturn = errNone;
	unsigned short Act_FramVersion;
	unsigned short i;

	if((FRAM_Modify == istate_FRAMAutomaticChange_V1_to_V2) 
		|| (FRAM_Modify == istate_FRAMAutomaticChange_V1_to_V3)|| (FRAM_Modify == istate_FRAMAutomaticChange_V2_to_V3)

		|| (FRAM_Modify == istate_FRAMAutomaticChange_V1_to_V4)|| (FRAM_Modify == istate_FRAMAutomaticChange_V2_to_V4)
		|| (FRAM_Modify == istate_FRAMAutomaticChange_V3_to_V4)

		|| (FRAM_Modify == istate_FRAMAutomaticChange_V1_to_V5) || (FRAM_Modify == istate_FRAMAutomaticChange_V2_to_V5)
		|| (FRAM_Modify == istate_FRAMAutomaticChange_V3_to_V5) || (FRAM_Modify == istate_FRAMAutomaticChange_V4_to_V5)

		|| (FRAM_Modify == istate_FRAMAutomaticChange_V1_to_V6) || (FRAM_Modify == istate_FRAMAutomaticChange_V2_to_V6)
		|| (FRAM_Modify == istate_FRAMAutomaticChange_V3_to_V6) || (FRAM_Modify == istate_FRAMAutomaticChange_V4_to_V6)
		|| (FRAM_Modify == istate_FRAMAutomaticChange_V5_to_V6)
		
		|| (FRAM_Modify == istate_FRAMAutomaticChange_V1_to_V7) || (FRAM_Modify == istate_FRAMAutomaticChange_V2_to_V7)
		|| (FRAM_Modify == istate_FRAMAutomaticChange_V3_to_V7) || (FRAM_Modify == istate_FRAMAutomaticChange_V4_to_V7)
		|| (FRAM_Modify == istate_FRAMAutomaticChange_V5_to_V7)	|| (FRAM_Modify == istate_FRAMAutomaticChange_V6_to_V7)

		|| (FRAM_Modify == istate_FRAMAutomaticChange_V1_to_V8) || (FRAM_Modify == istate_FRAMAutomaticChange_V2_to_V8)
		|| (FRAM_Modify == istate_FRAMAutomaticChange_V3_to_V8) || (FRAM_Modify == istate_FRAMAutomaticChange_V4_to_V8)
		|| (FRAM_Modify == istate_FRAMAutomaticChange_V5_to_V8)	|| (FRAM_Modify == istate_FRAMAutomaticChange_V6_to_V8)
		|| (FRAM_Modify == istate_FRAMAutomaticChange_V7_to_V8)
		)
	{
		FRAMSetStructVersion(0, 0);
		if((error = FRAMReadStructVersion(&Act_FramVersion)) == errNone)
		{
			if(Act_FramVersion == FRAM_STRUCT_VERSION)
			{
				switch(FRAM_Modify)
				{
					case istate_FRAMAutomaticChange_V1_to_V3:
					case istate_FRAMAutomaticChange_V1_to_V4:
					case istate_FRAMAutomaticChange_V1_to_V5:
					case istate_FRAMAutomaticChange_V1_to_V6:
					case istate_FRAMAutomaticChange_V1_to_V7:
					case istate_FRAMAutomaticChange_V1_to_V8:
															if((error = FRAMSaveAxisMinMaxWay(g_AxisMinMaxWay)) != errNone)
															{
																g_errArray[g_errCnt++] = iStateReturn = istate_FRAM_AutoChangeError_MinMaxWay;
															}

					case istate_FRAMAutomaticChange_V2_to_V3:
					case istate_FRAMAutomaticChange_V2_to_V4:
					case istate_FRAMAutomaticChange_V2_to_V5:
					case istate_FRAMAutomaticChange_V2_to_V6:
					case istate_FRAMAutomaticChange_V2_to_V7:
					case istate_FRAMAutomaticChange_V2_to_V8:
															if((error = FRAMSaveSensorConfig(&g_SensorConfig)) != errNone)
															{
																g_errArray[g_errCnt++] = iStateReturn = istate_FRAM_AutoChangeError_SensorConfig;
															}
															if((error = FRAMSavePortConfig(&g_PortConfig)) != errNone)
															{
																g_errArray[g_errCnt++] = iStateReturn = istate_FRAM_AutoChangeError_PortConfig;
															}
															if((error = FRAMSaveEquipmentConfig(&g_EquipmentConfig)) != errNone)
															{
																g_errArray[g_errCnt++] = iStateReturn = istate_FRAM_AutoChangeError_EquipmentConfig;
															}
					case istate_FRAMAutomaticChange_V3_to_V4:
					case istate_FRAMAutomaticChange_V3_to_V5:
					case istate_FRAMAutomaticChange_V3_to_V6:
					case istate_FRAMAutomaticChange_V3_to_V7:
					case istate_FRAMAutomaticChange_V3_to_V8:
															if((error = FRAMSaveEstopCounter(&g_EstopCounter)) != errNone)
															{
																g_errArray[g_errCnt++] = iStateReturn = istate_FRAM_AutoChangeError_EstopCounter;
															}
															if((error = FRAMSaveEstopWait(&g_EstopWait)) != errNone)
															{
																g_errArray[g_errCnt++] = iStateReturn = istate_FRAM_AutoChangeError_EstopWait;
															}
					case istate_FRAMAutomaticChange_V4_to_V5:
					case istate_FRAMAutomaticChange_V4_to_V6:
					case istate_FRAMAutomaticChange_V4_to_V7:
					case istate_FRAMAutomaticChange_V4_to_V8:
															if((error = FRAMSaveAxisConfigStallExeption(g_ConfigStallExeption)) != errNone)
															{
																g_errArray[g_errCnt++] = iStateReturn = istate_FRAM_AutoChangeError_ConfigStallExeption;
															}
															if((error = FRAMSaveAxisCounterStallExeption(g_CounterStallExeption)) != errNone)
															{
																g_errArray[g_errCnt++] = iStateReturn = istate_FRAM_AutoChangeError_CounterStallExeption;
															}
					case istate_FRAMAutomaticChange_V5_to_V6:
					case istate_FRAMAutomaticChange_V5_to_V7:
					case istate_FRAMAutomaticChange_V5_to_V8:
															if((error = FRAMSaveSearchAdapterRelativeDeltaWay(&g_SearchAdapterRelativeDeltaWay)) != errNone)
															{
																g_errArray[g_errCnt++] = iStateReturn = istate_FRAM_AutoChangeError_SearchAdapterRelativeDeltaWay;
															}
					case istate_FRAMAutomaticChange_V6_to_V7:
					case istate_FRAMAutomaticChange_V6_to_V8:
															for(i = 0;i < MAX_APPLICATION_SPD;i++)
															{
																if((error = FRAMSaveApplicationSpdAxis( i, g_ApplicationSpd[i].AppAxisSpd)) != errNone)
																{
																	g_errArray[g_errCnt++] = iStateReturn = istate_FRAM_AutoChangeError_ApplicationSpeed;
																}
															}
					case istate_FRAMAutomaticChange_V7_to_V8:
															if((error = FRAMSaveDoorLiftConfig(&g_DoorLiftConfig)) != errNone)
															{
																g_errArray[g_errCnt++] = iStateReturn = istate_FRAM_AutoChangeError_DoorLiftConfig;
															}

															break;
				}//end...switch
			}//end ...if(Act_FramVersion
			else
			{g_errArray[g_errCnt++] = iStateReturn = istate_FRAM_AutoChangeError_StructVersion;}
		}//ens...if((error = FRAMReadStructVersion
		else
		{g_errArray[g_errCnt++] = iStateReturn = istate_FRAM_AutoChangeError_StructVersion;}
	}
	return iStateReturn;
}
/* FUNCTION: 	DeviceInit
   PARAMETERS:	---
   DESCRIPTION:	Herstellen des Geräte-Anfangszustandes
   RETURNS: 	Integer */
unsigned short DeviceInit(void)
{ 
	unsigned short error = errNone;
	unsigned short errorReturn = errNone;
	unsigned short i, a;
	unsigned short ID;
	udtDeviceConfig tmpDeviceConfig;
//	unsigned short errCnt = 0;
	unsigned short errBus_CntNichtErhoehen = 0;
	unsigned short errEStop_CntNichtErhoehen = 0;
	unsigned short EStopSwitch;
	unsigned short EStopPort;
	unsigned short CntNichtErhoehen = 0;

	unsigned short FRAM_Modify = 0;
	InitSPI();
//  U0C1_COM1_RE_485= 0; // Enable reading RS485
	if((error = DevInit_SetFRAMModify(&FRAM_Modify)) == errNone)
	{
		if(FRAM_Modify == 0){g_errArray[g_errCnt++] = errorReturn = istate_FRAMStructVersion;}
	}
	else
	{
		g_errArray[g_errCnt++] = errorReturn = istate_FRAMReadStructVersion;
	}
	DataReset();
	if((error = FRAMReadDeviceConfig(&tmpDeviceConfig)) == errNone)
	{
		//g_DeviceConfig.ExternEstopInEnable = tmpDeviceConfig.ExternEstopInEnable;
		//g_DeviceConfig.InternEstopEnable = tmpDeviceConfig.InternEstopEnable;
		g_DeviceConfig.LockExternOutEstopEnable = tmpDeviceConfig.LockExternOutEstopEnable;
	}
	else
	{
		g_errArray[g_errCnt++] = errorReturn = istate_FRAMReadDeviceConfig;
	}
	SetEstopIn();//SetEstopIn(g_DeviceConfig);
	SetLockEstopOut(g_DeviceConfig);
	if((error = FRAMReadAxisOffset(g_AxisOffset)) == errNone)
	{
		for(i = 0; i < MAX_AXES; i++)
		{
			WDO();
			pIMSChangedPara[i]->fOffset = g_AxisOffset[i].AxisOffsetFromHomingSwitch;
		}
	}
	else
	{
		g_errArray[g_errCnt++] = errorReturn = istate_FRAMReadAxisOffset;
	}
	// ---------  FRAM Version anheben ----------
	if((error = FRAMVersion_AnhebenUndFehlendeDatenSpeichern(FRAM_Modify)) != errNone)
	{
		errorReturn = error;
	}
	/*else
	{*/
		if((error = FRAMReadAxisMinMaxWay(g_AxisMinMaxWay)) == errNone)
		{
			for(i = 0; i < MAX_AXES; i++)
			{
				WDO();
				pIMSChangedPara[i]->fMinWay = g_AxisMinMaxWay[i].MinWay;
				pIMSChangedPara[i]->fMaxWay = g_AxisMinMaxWay[i].MaxWay;
			}
		}
		else
		{g_errArray[g_errCnt++] = errorReturn = istate_FRAMReadAxisMinMaxWay;}

		if((error = FRAMReadSensorConfig(&g_SensorConfig)) != errNone)
		{
			g_errArray[g_errCnt++] = errorReturn = istate_FRAMReadSensorConfig;
		}
		if((error = FRAMReadPortConfig(&g_PortConfig)) != errNone)
		{
			g_errArray[g_errCnt++] = errorReturn = istate_FRAMReadPortConfig;
		}
		if((error = FRAMReadEquipmentConfig(&g_EquipmentConfig)) != errNone)
		{
			g_errArray[g_errCnt++] = errorReturn = istate_FRAMReadEquipmentConfig;
		}
		if((error = FRAMReadEstopCounter(&g_EstopCounter)) != errNone)
		{
			g_errArray[g_errCnt++] = errorReturn = istate_FRAMReadEstopCounter;
		}
		if((error = FRAMReadEstopWait(&g_EstopWait)) != errNone)
		{
			g_errArray[g_errCnt++] = errorReturn = istate_FRAMReadEstopWait;
		}
		if((error = FRAMReadAxisConfigStallExeption(g_ConfigStallExeption)) != errNone)
		{
			g_errArray[g_errCnt++] = errorReturn = istate_FRAMReadConfigStallExeption;
		}
		if((error = FRAMReadAxisCounterStallExeption(g_CounterStallExeption)) != errNone)
		{
			g_errArray[g_errCnt++] = errorReturn = istate_FRAMReadCounterStallExeption;
		}
		if((error = FRAMReadSearchAdapterRelativeDeltaWay(&g_SearchAdapterRelativeDeltaWay)) != errNone)
		{
			g_errArray[g_errCnt++] = errorReturn = istate_FRAMReadSearchAdapterRelativeDeltaWay;
		}
		CntNichtErhoehen = 0;
		for(i = 0;i < MAX_APPLICATION_SPD;i++)
		{
			if((error = FRAMReadApplicationSpdAxis(i, g_ApplicationSpd[i].AppAxisSpd)) != errNone)
			{
				if(!CntNichtErhoehen)
				{
					CntNichtErhoehen = 1;
					g_errArray[g_errCnt++] = errorReturn = istate_MotorInitAxisBase;
				}
			}
		}
		if((error = FRAMReadDoorLiftConfig(&g_DoorLiftConfig)) != errNone)
		{
			g_errArray[g_errCnt++] = errorReturn = istate_FRAM_AutoChangeError_DoorLiftConfig;
		}
		else
		{
			DoorLiftCopyDataConfigToChanged();
		}


	/*}*/
	if(gDoppelTasteUI){SensorAndPortPowerOnSetting();}
	K_Task_Wait(500);
	if((error = ImsCmd_Sync()) != errNone)
	{
		g_errArray[g_errCnt++] = errorReturn = istate_ImsCmd_Sync;
	}
	errBus_CntNichtErhoehen = 0;
	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if((ID = GetAxisIDFromMask(eAllMask, g_FelixAxes[i].AxisBit)) != 0)
		{
			if((error = MotorInit(ID)) != errNone)
			{
				//g_errArray[errCnt++] = errorReturn = istate_MotorInitAxisBase + ID;
				if(!errBus_CntNichtErhoehen)
				{
					errBus_CntNichtErhoehen = 1;
					g_errArray[g_errCnt++] = errorReturn = istate_MotorInitAxisBase;
				}
				//return error;
			}
		}
	}
	// ESTOP nur testenn wenn kein Fehler am bus oder IMS Motor
	if(!errBus_CntNichtErhoehen)
	{
		//ESTOP testen
		for(i = 0; i < MAX_AXES; i++)
		{
			WDO();
			if((ID = GetAxisIDFromMask(eAllMask, g_FelixAxes[i].AxisBit)) != 0)
			{
				if((error = ImsCmd_GetEStopSwitch(ID, &EStopSwitch)) != errNone)
				{
					// keine oder fehlerhafte Kommunikation
					if(!errBus_CntNichtErhoehen)
					{
						errBus_CntNichtErhoehen = 1;
						g_errArray[g_errCnt++] = errorReturn = istate_MotorInitAxisBase;
					}
				}
				else
				{
					//EStopSwitch H = ESTOP; L = kein ESTOP
					//ESTOP//P2_IN_P1//low active  L = ESTOP; H = kein ESTOP
					EStopPort = ESTOP;
					//if((EStopSwitch && ESTOP) || (!EStopSwitch && !ESTOP))
					if((EStopSwitch && EStopPort) || (!EStopSwitch && !EStopPort))
					{
						if(!errEStop_CntNichtErhoehen)
						{
							errEStop_CntNichtErhoehen = 1;
							g_errArray[g_errCnt++] = errorReturn = istate_MotorEStop;
						}
					}
				}//end...else
			}//end...if(ID
		}//end...for
	}//end...if(!errBus_...
	//#ifdef _CF_NRTL
	if(isNRTL_LP())
	{
		if(JALMON1)//ist H falls ungleich, else L
		{
			g_errArray[g_errCnt++] = errorReturn = istate_MotorJalousie;
		}
		if(ESTOP_EXT)//ist H falls ungleich, else L
		{
			g_errArray[g_errCnt++] = errorReturn = istate_MotorExternEstop;
		}
		i = 0;
		error = errNone;
//-----------------------------------------------
		SetFwEStopLock();
		for(i = 0; i < 75;i++)
		{
			K_Task_Wait(20);
			//if(ESTOP){return errNone;}
		}
		a = 0;
		K_Task_Wait(20);
		for(i = 0; i < 6;i++)
		{
			if(UEW_FET1_24V || UEW_FET2_24V || UEW_FET3_48V || UEW_FET4_48V)
			{
				a++;
				if(a == 3)
				{
					g_errArray[g_errCnt++] = errorReturn = error = istate_MotorFETEstopH;
				}
			}
			K_Task_Wait(2);
		}
		ResetFwEStopLock();
		K_Task_Wait(25);
		a = 0;
		if(ESTOP)	// low active, !ESTOP == estop
		{
			//Nur Test wenn kein ESTOP anliegt
			for(i = 0; i < 6;i++)
			{
				if((!UEW_FET1_24V || !UEW_FET2_24V || !UEW_FET3_48V || !UEW_FET4_48V) && (ESTOP))
				{
					a++;
					if(a == 3)
					{
						g_errArray[g_errCnt++] = errorReturn = error = istate_MotorFETEstopL;
					}
				}
				K_Task_Wait(2);
			}
		}
	}
	//#else
	else
	{
		if(JALMON1 != JALMON2)
		{
			g_errArray[g_errCnt++] = errorReturn = istate_MotorJalousie;
		}
	}
	//#endif
	if((g_EquipmentConfig.EquipmentConfig & BIT_E_DoorLifter) || (g_EquipmentConfig.DeviceConfig & BIT_D_DoorLifter))
	{
		DorrLift_U1C0_ASC_vInit();
		if((error = DoorLiftMotorInit()) != errNone)
		{
			g_errArray[g_errCnt++] = errorReturn = istate_DoorLiftMotorInit;
		}

	}
	return errorReturn;

}

unsigned short DeviceInitPCBTest(void)
{ 
	unsigned short error = errNone;
	unsigned short errorReturn = errNone;
	unsigned short i, a;
	unsigned short errBus_CntNichtErhoehen = 0;
	unsigned short errEStop_CntNichtErhoehen = 0;
	unsigned short CntNichtErhoehen = 0;

	unsigned short FRAM_Modify = 0;
	InitSPI();
//  U0C1_COM1_RE_485= 0; // Enable reading RS485
	DataReset();
	SetEstopIn();//SetEstopIn(g_DeviceConfig);
	SetLockEstopOut(g_DeviceConfig);
	if(gDoppelTasteUI){SensorAndPortPowerOnSetting();}
	K_Task_Wait(500);

	if(isNRTL_LP())
	{
		if(JALMON1)//ist H falls ungleich, else L
		{
			g_errArray[g_errCnt++] = errorReturn = istate_MotorJalousie;
		}
		if(ESTOP_EXT)//ist H falls ungleich, else L
		{
			g_errArray[g_errCnt++] = errorReturn = istate_MotorExternEstop;
		}
		i = 0;
		error = errNone;
//-----------------------------------------------
		SetFwEStopLock();
		for(i = 0; i < 75;i++)
		{
			K_Task_Wait(20);
			//if(ESTOP){return errNone;}
		}
		a = 0;
		K_Task_Wait(20);
		for(i = 0; i < 6;i++)
		{
			if(UEW_FET1_24V || UEW_FET2_24V || UEW_FET3_48V || UEW_FET4_48V)
			{
				a++;
				if(a == 3)
				{
					g_errArray[g_errCnt++] = errorReturn = error = istate_MotorFETEstopH;
				}
			}
			K_Task_Wait(2);
		}
		ResetFwEStopLock();
		K_Task_Wait(25);
		a = 0;
		if(ESTOP)	// low active, !ESTOP == estop
		{
			//Nur Test wenn kein ESTOP anliegt
			for(i = 0; i < 6;i++)
			{
				if((!UEW_FET1_24V || !UEW_FET2_24V || !UEW_FET3_48V || !UEW_FET4_48V) && (ESTOP))
				{
					a++;
					if(a == 3)
					{
						g_errArray[g_errCnt++] = errorReturn = error = istate_MotorFETEstopL;
					}
				}
				K_Task_Wait(2);
			}
		}
	}
	//#else
	else
	{
		if(JALMON1 != JALMON2)
		{
			g_errArray[g_errCnt++] = errorReturn = istate_MotorJalousie;
		}
	}
	return errorReturn;
}

/*void TaskStart(void)
{
	HardwareTask_vInit();
	K_Task_Create(PRI_USER_MAIN, &UserMainSlot, UserMain, C_TASK_STK_SIZE);
	K_Task_Start(UserMainSlot);		  									// trigger task

}*/
//void SetEstopIn(const udtDeviceConfig Config)
void SetEstopIn(void)
{
//#ifndef _CF_NRTL
	if(!isNRTL_LP())
	{
		DISABLE_ESTOP_INTERN = 0;// 1 = Intern disable; 0 = Intern enable
		DISABLE_ESTOP_EXTERN = 1;// 0 = extern disable; 1 = extern enable
	}
//#endif
/*	if(Config.ExternEstopInEnable	|| 	Config.InternEstopEnable)
	{
		g_EnableEstopEvent = 1;
	}
	else
	{
		//g_EnableEstopEvent = 0;
	}
	
	if(Config.InternEstopEnable)
	{
		DISABLE_ESTOP_INTERN = 0;// 1 = Intern disable; 0 = Intern enable
	}
	else
	{
		DISABLE_ESTOP_INTERN = 1;// 1 = Intern disable
	}

	if(Config.ExternEstopInEnable)
	{
		DISABLE_ESTOP_EXTERN = 1;// 0 = extern disable; 1 = extern enable
	}
	else
	{
		DISABLE_ESTOP_EXTERN = 0;// 0 = extern disable
	}
*/
}

void SetLockEstopOut(const udtDeviceConfig Config)
{
//#ifndef _CF_NRTL
	if(!isNRTL_LP())
	{
		if(Config.LockExternOutEstopEnable)
		{
			LOCK_ESTOP_EXTERN  = 0;// not lock estop for extern
		}
		else
		{
			LOCK_ESTOP_EXTERN  = 1;// lock estop for extern
		}
	}
//#endif
}

unsigned short DataReset(void)
{
	FelixDeviceConfigDefault();
	FelixAxesGenerate();
	FelixSensorAndPortGenerate();
	FelixEquipmentConfigGenerate();
	FelixEstopCounterGenerate();
	FelixEstopWaitGenerate();
	FelixSearchAdapterRelativeDeltaWayGenerate();
	AxisDataGenerateFromFixedData();
	FelixApplicationSpdGenerate();
	DoorLiftGenerate();

	return errNone;
}

unsigned short DataAndFRAMInit(void)
{
	unsigned short error;
	unsigned short i;
	unsigned short ID;
	udtDeviceConfig tmpDeviceConfig;

	DataReset();
	if((error = FRAMInit()) != errNone) {return error;}
	if((error = FRAMReadDeviceConfig(&tmpDeviceConfig)) != errNone){return error;}
	g_DeviceConfig.LockExternOutEstopEnable = tmpDeviceConfig.LockExternOutEstopEnable;
	SetLockEstopOut(g_DeviceConfig);
	if((error = FRAMReadAxisOffset(g_AxisOffset)) != errNone){return error;}
	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		pIMSChangedPara[i]->fOffset = g_AxisOffset[i].AxisOffsetFromHomingSwitch;
	}

	if((error = FRAMReadAxisMinMaxWay(g_AxisMinMaxWay)) != errNone){return error;}
	{
		for(i = 0; i < MAX_AXES; i++)
		{
			WDO();
			pIMSChangedPara[i]->fMinWay = g_AxisMinMaxWay[i].MinWay;
			pIMSChangedPara[i]->fMaxWay = g_AxisMinMaxWay[i].MaxWay;
		}
	}
	if((error = FRAMReadSensorConfig(&g_SensorConfig)) != errNone){return error;}
	if((error = FRAMReadPortConfig(&g_PortConfig)) != errNone){return error;}
	if((error = FRAMReadEquipmentConfig(&g_EquipmentConfig)) != errNone){return error;}
	if((error = FRAMReadEstopCounter(&g_EstopCounter)) != errNone){return error;}
	if((error = FRAMReadEstopWait(&g_EstopWait)) != errNone){return error;}
	if((error = FRAMReadAxisConfigStallExeption(g_ConfigStallExeption)) != errNone){return error;}
	if((error = FRAMReadAxisCounterStallExeption(g_CounterStallExeption)) != errNone){return error;}
	if((error = FRAMReadSearchAdapterRelativeDeltaWay(&g_SearchAdapterRelativeDeltaWay)) != errNone){return error;}
	if(gDoppelTasteUI){SensorAndPortPowerOnSetting();}

/*	entfernt in V04.63.00
	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if((ID = GetAxisIDFromMask((eAllMask), g_FelixAxes[i].AxisBit)) != 0)
		{
			if((error = MotorInit(ID)) != errNone){return error;}
		}
	}*/
	return error;
}



