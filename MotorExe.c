//****************************************************************************
// @Module        Moror Config
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
#include "Cxconfig.h"
#include "DeviceConfig.h"
#include "Drv_config.h"

#include "Felix_Hardware.h"
#include "MDrive_CmdCtrl.h"
#include "MDriveCmd.h"
#include "MotorExe.h"
#include "Felix_ASyncCtrl.h"
#include "Felix_Misc.h"
#include "Felix_MotorTask.h"
#include "Felix_HardwareTask.h"
//****************************************************************************
// @Macros
//****************************************************************************

//****************************************************************************
// local definitions
//****************************************************************************

//****************************************************************************
// default settings
//****************************************************************************

//****************************************************************************
// external functions
//****************************************************************************

//****************************************************************************
// external data
//****************************************************************************
extern IMS_CommandBlock Msg;
extern ASync_CommandBlock g_MsgASync;
//****************************************************************************
// public data
//****************************************************************************

//****************************************************************************
// private data
//****************************************************************************
#ifdef _CF_DEBUG_PRINT
void DebugPrintPosition(unsigned short mask);
#endif
//unsigned short GetAxesMoveFlag(unsigned short Mask);
//unsigned short PrepareAxesAndWayParameter(unsigned short Mask);
//****************************************************************************
// private functions
//****************************************************************************
//****************************************************************************
// public functions
//****************************************************************************

unsigned short MotorMoveSyncronExe(unsigned short Mask, unsigned short Mode1536)
{
	unsigned short error = errNone;
	//unsigned short i;
	//unsigned short ID;
	unsigned short value;
	//unsigned short ESTOP_flag;
	//unsigned short SLOMO_flag;

	if((Mask = ConfirmMask(Mask)) == 0){return ERR_WRONG_PARAMETER;}
	//if((error = ImsCmd_GetAllRuningFlags(Mask, &value)) != errNone) return error;
	if((error= g_ASync_GetAllRuningFlags(Mask, &value)) != errNone){return error;}
	if(value == STATE_ESTOP){return (unsigned short)ERR_SAFETY_APPLIANCE;}
	if(value == STATE_BUSY){return ERR_DEVICE_BUSY;}
	//if(value != STATE_READY){return ERR_DEVICE_BUSY;}
	if((error = g_ASync_ResetError(1, Mask)) != errNone) {return error;}
	#ifdef _CF_DEBUG_PRINT
	if(g_TestFlag & TF_DBG_INT_COM){DebugPrintPosition(Mask);}
	#endif
	error= g_ASync_StartMultipleMoveAbsolute(Mask, Mode1536);
	if(error != errNone){return error;}
	do{
		K_Task_Wait(50);
		WDO();
		if((error= g_ASync_GetAllRuningFlags(Mask, &value)) != errNone)
		{
			return error;
		}
		if(value == STATE_ESTOP)
		{
			return (unsigned short)ERR_SAFETY_APPLIANCE;
		}
	//}while((value == STATE_BUSY)  || (value == STATE_BUSY_ESTOP));
	}while(value == STATE_BUSY);
	if((error = g_ASync_GetAllErrorFlags(Mask, &value)) != errNone)
	{
		return error;
	}
	#ifdef _CF_DEBUG_PRINT
	if(g_TestFlag & TF_DBG_INT_COM){DebugPrintPosition(Mask);}
	#endif
	return value;
}


unsigned short MotorMoveASyncExe(unsigned short Mask, unsigned short Mode1536)
{
	unsigned short error = errNone;
	//unsigned short i;
	//unsigned short ID;
	unsigned short value;

	Mask = ConfirmMask(Mask);
	if((error= g_ASync_GetAllRuningFlags(Mask, &value)) != errNone){return error;}
	if(value == STATE_ESTOP){return (unsigned short)ERR_SAFETY_APPLIANCE;}
	if(value == STATE_BUSY){return ERR_DEVICE_BUSY;}
	//if(value != STATE_READY){return ERR_DEVICE_BUSY;}
	if((error = g_ASync_ResetError(1, Mask)) != errNone) {return error;}
/*	if((error = g_ASync_GetAllErrorFlags(Mask, &value)) != errNone) {return error;}
	if(value) {return value;}*/
	#ifdef _CF_DEBUG_PRINT
	if(g_TestFlag & TF_DBG_INT_COM){DebugPrintPosition(Mask);}
	#endif
	error= g_ASync_StartMultipleMoveAbsolute(Mask, Mode1536);
	if(error != errNone){return error;}
	K_Task_Wait(10);
	if((error = g_ASync_GetAllErrorFlags(Mask, &value)) != errNone) {return error;}
	return value;

}
unsigned short MotorTeachExe(unsigned short Mask)
{
	unsigned short error = errNone;
	error= g_ASync_StartMultipleMoveTeach(Mask);
	return error;
}
#ifdef _CF_DEBUG_PRINT
void DebugPrintPosition(unsigned short mask)

{
	unsigned short i;
	unsigned short ID;
	double Value = 0;

	dbPrint("\r");
	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if((ID = GetAxisIDFromMask(mask, g_FelixAxes[i].AxisBit)) != 0)
		{
			MotorGetPosition( ID, &Value);
			dbPrint("%3.3f ",Value);
		}
	}
	dbPrint("\r");

}
#endif
unsigned short MotorSetSpeedExe(unsigned short  ID, float Start, float End, float Acc)
{
	unsigned short error = errNone;
	short idx;

	idx = GetAxisIndex(ID);
	if(idx < 0)
	{
		return ERR_UNKNOWN_DRV;
	}
	/*if(Start >= (float)0.0)
	{

		if((error= MotorSetStartSpeed( ID, Start)) != errNone) return error;
	}
	if((error= MotorSetEndSpeed( ID, End)) != errNone) return error;
	if((error= MotorSetAcceleration( ID, Acc)) != errNone) return error;
	*/
	if((error= ImsCmd_SetSpeed(ID, Start, End, Acc)) != errNone) return error;
	if(Start >= (float)0.0)
	{
		pIMSChangedPara[idx]->fStartSpeed = Start;
	}
	pIMSChangedPara[idx]->fEndSpeed = End;
	pIMSChangedPara[idx]->fAcceleration = Acc;
	return error;
}

unsigned short MotorSetSonderSpeedExe(unsigned short funktion, unsigned short ID, float End, float Acc)
{
	unsigned short error = errNone;
	short idx;

	idx = GetAxisIndex(ID);
	if(idx < 0)
	{
		return ERR_UNKNOWN_DRV;
	}
	switch(funktion)
	{
		case HOMING:	pIMSChangedPara[idx]->fHomingSpeed = End;
						pIMSChangedPara[idx]->fAccelerationHoming = Acc;
						break;
		default:		error = ERR_WRONG_PARAMETER;
						break;
	}

	return error;
}

unsigned short MotorGetSpeedExe(const unsigned short ID, udtSpeedParaDevice *Speed, unsigned short Scr)
{
	unsigned short error = errNone;
	short idx;
	idx = GetAxisIndex(ID);
	if(idx < 0)
	{
		return ERR_UNKNOWN_DRV;
	}
	if(Scr)
	{
		/*if((error= MotorGetStartSpeed( ID, &Speed->Start)) != errNone) return error;
		if((error= MotorGetEndSpeed( ID, &Speed->End)) != errNone) return error;
		if((error= MotorGetAcceleration( ID, &Speed->Acc)) != errNone) return error;*/
		error = ImsCmd_GetSpeed(ID, &Speed->Start, &Speed->End, &Speed->Acc);
	}
	else
	{
		Speed->Start = pIMSChangedPara[idx]->fStartSpeed;
		Speed->End = pIMSChangedPara[idx]->fEndSpeed;
		Speed->Acc = pIMSChangedPara[idx]->fAcceleration;
	}
	return error;

}

unsigned short MotorGetSonderSpeedExe(unsigned short funktion, unsigned short ID, udtSpeedParaDevice *Speed)
{
	unsigned short error = errNone;
	short idx;
	idx = GetAxisIndex(ID);
	if(idx < 0)
	{
		return ERR_UNKNOWN_DRV;
	}
	switch(funktion)
	{
		case HOMING:	Speed->End = pIMSChangedPara[idx]->fHomingSpeed;
						Speed->Acc = pIMSChangedPara[idx]->fAccelerationHoming;
						break;
		default:		error = ERR_WRONG_PARAMETER;
						break;
	}

	return error;

}
