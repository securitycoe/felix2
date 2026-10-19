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
#include "Felix_ASync.h"
#include "Felix_ASyncMove.h"
#include "Felix_Hardware.h"
#include "Drv_config.h"
#include "MDrive_CmdCtrl.h"
#include "MDriveCmd.h"
#include "Felix_Misc.h"
#include "Felix_HardwareTask.h"
#include "Felix_EStopHandling.h"

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

//****************************************************************************
// internal functions
//****************************************************************************

//****************************************************************************
// public data
//****************************************************************************
unsigned short m_MovActive = 0;
//****************************************************************************
// private data
//****************************************************************************
unsigned short m_MovMask = 0;
//unsigned short m_MovWaitMask = 0;
//unsigned short m_MovGetErrorMask = 0;

//****************************************************************************
// private functions
//***************************************************************************

//unsigned short m_ASync_MovExe(void);
//****************************************************************************
// public functions
//****************************************************************************
unsigned short ASync_Mov(void)
{
	unsigned short Running;
//	#ifdef _CF_DEBUG_PRINT
//	if(g_TestFlag & TF_DBG_ASYNC){dbPrint("AM\r");}
//	#endif
//	Running = ASync_GetRunningAllNew(&m_MovWaitMask);
	Running = ASync_GetRunningAllNew(&m_MovMask);
	if(Running == STATE_ESTOP){return (unsigned short)ERR_SAFETY_APPLIANCE;}
	if(Running == STATE_ERROR)
	{
		m_MovActive = 0;
		m_AsyncImsCmd_StopMotor();//neu
		SetRunStatusRdyErr(m_MovMask);//neu
		return ERR_MOVE;
	}
	if(Running == STATE_READY)
	{
//		//	#ifdef _CF_DEBUG_PRINT
//		if(g_TestFlag & TF_DBG_ASYNC){dbPrint("AM3\r");}
//		#endif
		m_MovActive = 0;
		SetRunStatusRdyErr(m_MovMask);
	}
	return errNone;
}


void SetMoveCtrlVar(unsigned short Mask)
{
	m_MovActive = 1;
	m_MovMask = m_MovMask | Mask;
	SetRunStatusBusy(m_MovMask);
	//m_MovWaitMask = Mask;
	//m_MovGetErrorMask = m_MovMask;
	//m_MovWaitMask = m_MovMask;
}
