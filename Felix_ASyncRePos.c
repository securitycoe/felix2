//****************************************************************************
// @Module        Motortask
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
#include "DeviceConfig.h"
#include "Cxconfig.h"
#include "Drv_config.h"
#include "MDrive_CmdCtrl.h"
#include "MDriveCmd.h"
#include "Felix_Misc.h"
#include "Felix_Hardware.h"
#include "Felix_HardwareTask.h"
#include "Felix_EStopHandling.h"
#include "Felix_ASync.h"
#include "Felix_ASyncRePos.h"

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
//extern unsigned short m_maskErrorAxes;
//****************************************************************************
// internal functions
//****************************************************************************

//****************************************************************************
// public data
//****************************************************************************

//****************************************************************************
// private data
//****************************************************************************

//****************************************************************************
// private functions
//****************************************************************************

unsigned short m_Async_EStopRestorCurrentAndGetPos(unsigned short *result);
unsigned short m_Async_EStopRestoreCurrentAndSetPos(void);
unsigned short m_Async_EStopSafetyPos(unsigned short mask);
unsigned short m_Async_EStopRestorePos(unsigned short mask);

unsigned short m_Async_EStopResetError(unsigned short mask);

unsigned short m_Async_EStopResetError(unsigned short mask)
{
	unsigned short error;
/*	unsigned short i;

	for(i=0; i < MAX_AXES; i++)
	{
		WDO();
		if(m_CheckMaskBit(mask, i) != 0)
		{
			m_maskErrorAxes &= ~g_FelixAxes[i].AxisBit;
		}
	}*/
	if((error= ASync_ResetError(mask)) != errNone) { return error;}
	if((error= ASync_ResetHWError(mask)) != errNone) { return error;}
	return error;
}

unsigned short m_Async_EStopRestorCurrentAndGetPos(unsigned short *result)
{
	unsigned char status;

	m_MsgASyncMot.Error = errNone;
	m_MsgASyncMot.eCommand = eImsEStopRestoreCurrentAndGetPos;
	status = K_Mesg_Send_Wait(MBX_ASYNCMOTOR, 0, &m_MsgASyncMot);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	*result = m_MsgASyncMot.Argument.usShort;
	return m_MsgASyncMot.Error;
}


unsigned short m_Async_EStopRestoreCurrentAndSetPos(void)
{
	unsigned char status;

	m_MsgASyncMot.Error = errNone;
	m_MsgASyncMot.eCommand = eImsEStopRestoreCurrentAndSetPos;
	status = K_Mesg_Send_Wait(MBX_ASYNCMOTOR, 0, &m_MsgASyncMot);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	return m_MsgASyncMot.Error;
}


unsigned short m_Async_EStopSafetyPos(unsigned short mask)
{
	unsigned char status;
	unsigned short Running;
	unsigned short i;
	unsigned short error;

	if((error= m_Async_EStopResetError(mask)) != errNone) { return error;}

	m_MsgASyncMot.Error = errNone;
	m_MsgASyncMot.eCommand = eImsEStopSafetyPos;
	m_MsgASyncMot.Mask = mask;

	for(i = 0; i < MAX_AXES; i++)
	{
		if(m_CheckMaskBit(mask & eZ_Ex_Axis, i))
		{
			m_MsgASyncMot.MoveData[i].StepWay = (long)0;// in Steps
		}
	}

	status = K_Mesg_Send_Wait(MBX_ASYNCMOTOR, 0, &m_MsgASyncMot);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	if(m_MsgASyncMot.Error != errNone){return m_MsgASyncMot.Error;}
	while(1)
	{
		Running = ASync_GetRunningAllNew(&mask);
		if(Running == STATE_ESTOP){return (unsigned short)ERR_SAFETY_APPLIANCE;}
		if(Running == STATE_ERROR)
		{
			return ERR_MOVE;
		}
		if(Running == STATE_READY)
		{
			return errNone;
		}
	}
}

unsigned short m_Async_EStopRestorePos(unsigned short mask)
{
	unsigned char status;
	unsigned short Running;

	unsigned short error;

	if((error= m_Async_EStopResetError(mask)) != errNone) { return error;}

	m_MsgASyncMot.Error = errNone;
	m_MsgASyncMot.eCommand = eImsEStopRestorePos;
	m_MsgASyncMot.Mask = mask;
	status = K_Mesg_Send_Wait(MBX_ASYNCMOTOR, 0, &m_MsgASyncMot);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	if(m_MsgASyncMot.Error != errNone)
	{
#ifdef _CF_ESTOP_DEBUG_PRINT
if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("mA_EStopRestorePos %u\r", m_MsgASyncMot.Error);}
#endif

		return m_MsgASyncMot.Error;
	}
	while(1)
	{
		Running = ASync_GetRunningAllNew(&mask);
		if(Running == STATE_ESTOP){return (unsigned short)ERR_SAFETY_APPLIANCE;}
		if(Running == STATE_ERROR)
		{
			return ERR_MOVE;
		}
		if(Running == STATE_READY)
		{
			return errNone;
		}
	}
}
//****************************************************************************
// public functions
//****************************************************************************
unsigned short ASync_EStopMoveSavetyPos(void)
{
	unsigned short error;
	unsigned short Verschoben;
	
	// Bestromen und Posditionsvergleich
	if((error= m_Async_EStopRestorCurrentAndGetPos(&Verschoben)) != errNone) 
	{
#ifdef _CF_ESTOP_DEBUG_PRINT
if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("mA_EStopRestorCurrentAndGetPos %u\r", error);}
#endif

		return error;
	}
	if(Verschoben)
	{
		// Hippie nach oben fahren
#ifdef _CF_DEBUG_PRINT
if(g_TestFlag & TF_DBG_FETRESET){dbPrint("SAFETYPOS\r");}
#endif

		if((error= m_Async_Hippie_Safety()) != errNone) {return error;}
		// Z Achsen nach oben fahren
		if((error= m_Async_EStopSafetyPos(eZ_Ex_Axis)) != errNone) 
		{
#ifdef _CF_ESTOP_DEBUG_PRINT
if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("mA_EStopSafetyPos %u\r", error);}
#endif

			return error;
		}
	}
	return error;
}

unsigned short ASync_EStopMoveRestorePos(void)
{
	unsigned short error;
	// XY Achsen repositionieren
	if((error= m_Async_EStopRestorePos(eXYMask)) != errNone) 
	{
#ifdef _CF_ESTOP_DEBUG_PRINT
if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("mA_EStopRestorePosXY %u\r", error);}
#endif

		return error;
	}
	// Z Achsen repositionieren
	if((error= m_Async_EStopRestorePos(eZ_Ex_Axis)) != errNone) 
	{
#ifdef _CF_ESTOP_DEBUG_PRINT
if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("mA_EStopRestorePosZ %u\r", error);}
#endif

		return error;
	}
	// Hippie repositionieren
	if((error= m_Async_Hippie_Restore()) != errNone) 
	{
		return error;
	}
	return error;
}

unsigned short ASync_EStopRestoreCurrentAndSetPos(void)
{
	unsigned short error;
	
	// Bestromen und Posditionsvergleich
	if((error= m_Async_EStopRestoreCurrentAndSetPos()) != errNone) 
	{
#ifdef _CF_ESTOP_DEBUG_PRINT
if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("mA_EStopRestoreCurrentAndSetPos %u\r", error);}
#endif

		return error;
	}
	return error;
}


unsigned short ASync_EStopContinue(void)
{
	unsigned short error;

	if((error= m_Async_EStopResetError(eAllMask)) != errNone) { return error;}
	return error;
}

