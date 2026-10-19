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
#include "Felix_ASyncHoming.h"
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
unsigned short m_AdjustActive = 0;
unsigned short m_Adjust_State = 0;
//****************************************************************************
// private data
//****************************************************************************
unsigned short m_AdjustMask = 0;
unsigned short m_AdjWaitMask = 0;
unsigned short m_AdjGetErrorMask = 0;

//****************************************************************************
// private functions
//****************************************************************************
unsigned short m_ASync_AdjustExe(void);
//unsigned short m_Adjust_InitAll(unsigned short mask);
//unsigned short m_Adjust_InitStep(unsigned short ID, unsigned short i);
unsigned short m_Adjust_HomingAll(unsigned short mask, unsigned short MaskInversMode);
unsigned short m_Adjust_HomingStep(unsigned short mask, unsigned short MaskInversMode);
unsigned short m_Adjust_ResetPositionAll(unsigned short mask);
unsigned short m_Adjust_NormalSpeedAll(unsigned short mask);
unsigned short m_Adjust_NormalSpeedStep(unsigned short ID, unsigned short i);
unsigned short m_Adjust_StartOffsetMoveAll(unsigned short mask);
unsigned short m_Adjust_StartOffsetMoveStep(unsigned short Mask);
unsigned short m_Adjust_SetKoordinatensystemAll(unsigned short mask);
unsigned short m_Adjust_StartHomingPositionAll(unsigned short mask);
unsigned short m_Adjust_StartHomingPositionStep(unsigned short Mask);
unsigned short m_Adjust_SetS1(udtIMS_CMD_IDENT eCommand, unsigned short mask);
void ResetHomingCtrlVar(void);
unsigned short SetHomingVar(unsigned short Mask);
//****************************************************************************
// public functions
//****************************************************************************

/*unsigned short m_Adjust_InitAll(unsigned short mask)
{
*	unsigned short error = errNone;
	unsigned short i;
	unsigned short ID;
*
return errNone;
*	//Strom setzen
	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if((ID = m_GetAxisIDFromMask(mask, i)) != 0)
		{

			if((error = m_Adjust_InitStep(ID, i)) != errNone)
			{
				SetErrorForBit(i,error);
				return error;
			}
		}
	}
	return error;*
}*/


/*unsigned short m_Adjust_InitStep(unsigned short ID, unsigned short i)
{
	unsigned short error;
	if((error = m_AsyncImsCmd_ResetError( ID)) != errNone) return error;
	if((error = m_AsyncImsCmd_ResetStallFlag( ID)) != errNone) return error;
	//if((error = m_AsyncImsCmd_SetHoldCurrent( ID, pIMSChangedPara[i]->usHoldCurrent)) != errNone) return error;
	//HomingSpeed
*	if((error = m_AsyncImsCmd_SetDecToAcc(ID)) != errNone) {return error;}
	if(GetSloMo())
	{
		if((error = m_AsyncImsCmd_SetSpeed(ID, (float)-1.0, pIMSChangedPara[i]->fSloMoSpeed, pIMSChangedPara[i]->fAccelerationSloMo)) != errNone)
		{
			return error;
		}
	}
	else
	{
		if((error = m_AsyncImsCmd_SetSpeed(ID, (float)-1.0, pIMSChangedPara[i]->fHomingSpeed, pIMSChangedPara[i]->fAccelerationHoming)) != errNone)
		{
			return error;
		}
	}*
	return error;
}*/


unsigned short m_Adjust_HomingAll(unsigned short mask, unsigned short MaskInversMode)
{
	unsigned short error = errNone;

	if((error = m_Adjust_HomingStep(mask, MaskInversMode)) != errNone)
	{
		//SetErrorForMask(mask, error);
	}
	return error;
}
unsigned short m_Adjust_HomingStep(unsigned short mask, unsigned short MaskInversMode)
{
	unsigned short error;
	unsigned short i;
	unsigned short ID;

	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if((ID = m_GetAxisIDFromMask(mask, i)) != 0)
		{
			m_MsgASyncMot.MoveData[i].ID = ID;
			m_MsgASyncMot.MoveData[i].StartEnable = 1;
			if(g_FelixAxes[i].AxisBit & MaskInversMode)
			{
				m_MsgASyncMot.MoveData[i].HomeToSwitchMode = pIMSFixedPara[i]->InversHomeToSwitchMode;
				m_MsgASyncMot.MoveData[i].InversHomingMode = 1;
			}
			else
			{
				m_MsgASyncMot.MoveData[i].HomeToSwitchMode = pIMSFixedPara[i]->HomeToSwitchMode;
				m_MsgASyncMot.MoveData[i].InversHomingMode = 0;
			}
		}
		else
		{
			m_MsgASyncMot.MoveData[i].StartEnable = 0;
		}
	}
	m_MsgASyncMot.Error = errNone;
	m_MsgASyncMot.eCommand = eImsHomingMultiSequence;
	error = K_Mesg_Send_Wait(MBX_ASYNCMOTOR, 0, &m_MsgASyncMot);
	if(error != K_OK)
	{
		return ERR_SYSTEM;
	}
	return m_MsgASyncMot.Error;
}

unsigned short m_Adjust_ResetPositionAll(unsigned short mask)
{
	unsigned short error = errNone;
	unsigned short i;
	unsigned short ID;
	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if((ID = m_GetAxisIDFromMask(mask, i)) != 0)
		{
			//if((error = m_AsyncImsCmd_ResetPosition(eImsResetPosition, ID)) != errNone)
			if((error = m_Async_ImsCmdID(eImsResetPosition, ID)) != errNone)
			{
				//SetErrorForBit(i,error);
				return error;
			}
		}
	}
	return error;
}

unsigned short m_Adjust_NormalSpeedAll(unsigned short mask)
{
	unsigned short error = errNone;
	unsigned short i;
	unsigned short ID;
	//Strom setzen
	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if((ID = m_GetAxisIDFromMask(mask, i)) != 0)
		{

			if((error = m_Adjust_NormalSpeedStep(ID, i)) != errNone)
			{
				//SetErrorForBit(i,error);
				return error;
			}
		}
	}
	return error;
}

unsigned short m_Adjust_NormalSpeedStep(unsigned short ID, unsigned short i)
{
	unsigned short error;
//	if((error = m_AsyncImsCmd_ResetError( ID)) != errNone) return error;
//	if((error = m_AsyncImsCmd_ResetStallFlag( ID)) != errNone) return error;
	//if((error = m_AsyncImsCmd_SetHoldCurrent( ID, pIMSChangedPara[i]->usHoldCurrent)) != errNone) return error;
	//HomingSpeed
	if((error = m_Async_ImsCmdID(eImsSetDecToAcc, ID)) != errNone)
	//if((error = m_AsyncImsCmd_SetDecToAcc(ID)) != errNone) {return error;}
	if((error = m_AsyncImsCmd_SetSpeed(ID, (float)-1.0, pIMSChangedPara[i]->fEndSpeed, pIMSChangedPara[i]->fAcceleration)) != errNone)
	{
		return error;
	}
	return error;
}

unsigned short m_Adjust_StartOffsetMoveAll(unsigned short mask)
{
	unsigned short error;

	if((error = m_Adjust_StartOffsetMoveStep( mask)) != errNone)
	{
		//SetErrorForMask(mask, error);
	}
	return error;

}
unsigned short m_Adjust_StartOffsetMoveStep(unsigned short Mask)
{
	unsigned char status;
	unsigned short ID;
	unsigned short i;
	long value;
	double dvalue;

	m_MsgASyncMot.Error = errNone;
	m_MsgASyncMot.eCommand = eImsAdjustStartMove;

	for(i=0; i < MAX_AXES; i++)
	{
		if((ID = GetAxisIDFromMask(Mask, g_FelixAxes[i].AxisBit)) != 0)
		{
			//value =  pIMSChangedPara[i]->fOffset * pIMSFixedPara[i]->fMotImpProMM;
			dvalue =  pIMSChangedPara[i]->fOffset * pIMSFixedPara[i]->fMotImpProMM;
			//dvalue =  m_extern_ptr->MoveData[i].AbsolutePos * pIMSFixedPara[i]->fMotImpProMM;
			value = RoundToLong(dvalue);
			if(pIMSFixedPara[i]->fIgrImpulseProRev < 0.0)
			{
				value*= -1;
			}
			m_MsgASyncMot.MoveData[i].ID = ID;
			m_MsgASyncMot.MoveData[i].StartEnable = 1;
			m_MsgASyncMot.MoveData[i].StepWay = value;
		}
		else
		{
			m_MsgASyncMot.MoveData[i].StartEnable = 0;
		}
	}
	status = K_Mesg_Send_Wait(MBX_ASYNCMOTOR, 0, &m_MsgASyncMot);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	return m_MsgASyncMot.Error;
}

unsigned short m_Adjust_SetKoordinatensystemAll(unsigned short mask)
{
	unsigned short error = errNone;
	unsigned short i;
	unsigned short ID;
	long lvalue;
	double dvalue;
	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if((ID = m_GetAxisIDFromMask(mask, i)) != 0)
		{
			//lvalue = pIMSFixedPara[i]->fSetHomingPosition * pIMSFixedPara[i]->fENCImpProMM;
			dvalue = pIMSFixedPara[i]->fSetHomingPosition * pIMSFixedPara[i]->fENCImpProMM;
			lvalue = RoundToLong(dvalue);
			if((error = m_AsyncImsCmd_SetPosition( ID, lvalue)) != errNone)
			{
				//SetErrorForBit(i,error);
				return error;
			}
			/*if((error = m_AsyncImsCmd_ResetError( ID)) != errNone)
			{
				SetErrorForBit(i,error);
				return error;
			}*/
			/*if((error = m_AsyncImsCmd_ResetStallFlag( ID)) != errNone)
			{
				SetErrorForBit(i,error);
				return error;
			}*/
		}
	}
	return error;
}

unsigned short m_Adjust_StartHomingPositionAll(unsigned short mask)
{
	unsigned short error;

	if((error = m_Adjust_StartHomingPositionStep( mask)) != errNone)
	{
		//SetErrorForMask(mask, error);
	}
	return error;

}
unsigned short m_Adjust_StartHomingPositionStep(unsigned short Mask)
{
	unsigned char status;
	unsigned short ID;
	unsigned short i;
	long value;
	double dvalue;

	m_MsgASyncMot.Error = errNone;
	m_MsgASyncMot.eCommand = eImsAdjustStartMove;

	for(i=0; i < MAX_AXES; i++)
	{
		if((ID = GetAxisIDFromMask(Mask, g_FelixAxes[i].AxisBit)) != 0)
		{
			//value =  pIMSFixedPara[i]->fSetHomingPosition * pIMSFixedPara[i]->fMotImpProMM;
			dvalue =  pIMSFixedPara[i]->fSetHomingPosition * pIMSFixedPara[i]->fMotImpProMM;
			//dvalue =  m_extern_ptr->MoveData[i].AbsolutePos * pIMSFixedPara[i]->fMotImpProMM;
			value = RoundToLong(dvalue);
			if(pIMSFixedPara[i]->fIgrImpulseProRev < 0.0)
			{
				value*= -1;
			}
			m_MsgASyncMot.MoveData[i].ID = ID;
			m_MsgASyncMot.MoveData[i].StartEnable = 1;
			m_MsgASyncMot.MoveData[i].StepWay = value;
		}
		else
		{
			m_MsgASyncMot.MoveData[i].StartEnable = 0;
		}
	}
	status = K_Mesg_Send_Wait(MBX_ASYNCMOTOR, 0, &m_MsgASyncMot);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	return m_MsgASyncMot.Error;
}
//------------------------
unsigned short m_Adjust_SetS1(udtIMS_CMD_IDENT eCommand, unsigned short mask)
{
	unsigned short error = errNone;
	unsigned short i;
	unsigned short ID;
	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if((ID = m_GetAxisIDFromMask(mask, i)) != 0)
		{
			if((error = m_Async_ImsCmdID(eCommand, ID)) != errNone)
			{
				return error;
			}
		}
	}
	return error;
}


unsigned short m_ASync_AdjustExe(void)
{
	unsigned short error, i, ID;
	unsigned short xy_mask, zz_mask, help;
	unsigned short Running;
	unsigned short HomeSwitch;
static unsigned short xy_mask_invers;
static unsigned short zz_mask_invers;
	//---------------------------------------------------------------------------------------------------------
	/*if(m_Adjust_State == ADJ_STEP_Z_HOMING_START)
	{
		if((error = m_AsyncImsCmd_GetHomeSwitchMask(eZ_Axis, &HomeSwitch)) != errNone) {return error;}
		if(HomeSwitch){m_Adjust_State = ADJ_STEP_Z_HOMING_INVERS_START;}
		else{m_Adjust_State = ADJ_STEP_Z_HOMING_NORMAL_START;}
	}
	//---------------------------------------------------------------------------------------------------------
	if(m_Adjust_State == ADJ_STEP_Z_HOMING_INVERS_START)
	{
		#ifdef _CF_DEBUG_PRINT
		if(g_TestFlag & TF_DBG_ASYNC){dbPrint("Start ADJ_STEP_Z_HOMING\r");}
		#endif
		// Z in Nulllage fahren starten
		if((error= m_Adjust_HomingAll(eZ_Axis, eZ_Axis)) != errNone)
		{
			#ifdef _CF_DEBUG_PRINT
			if(g_TestFlag & TF_DBG_ASYNC){dbPrint("Start ADJ_STEP_Z_HOMING ErrB\r");}
			#endif
			return error;
		}
		m_AdjWaitMask = eZ_Axis;
		m_AdjGetErrorMask = eZ_Axis;
		m_Adjust_State = ADJ_STEP_Z_HOMING_INVERS_WAIT;
		//m_AdjStatusRead = 0;
	}*/
	help = eXYMask;
	zz_mask = m_AdjustMask & ~help;

	if(m_Adjust_State == ADJ_STEP_Z_HOMING_START)
	{
		//help = eXYMask;
		//zz_mask = m_AdjustMask & ~help;
		HomeSwitch = 0;
		zz_mask_invers = 0;
		for(i = 0; i < MAX_AXES; i++)
		{
			WDO();
			if((ID = m_GetAxisIDFromMask(zz_mask, i)) != 0)
			{
				if((error = m_AsyncImsCmd_GetHomeSwitchID(ID, &HomeSwitch)) != errNone) {return error;}
				{
					if(HomeSwitch)
					{
						zz_mask_invers = zz_mask_invers | g_FelixAxes[i].AxisBit;
					}
				}
			}
		}
		if(zz_mask)
		{
			#ifdef _CF_DEBUG_PRINT
			if(g_TestFlag & TF_DBG_ASYNC){dbPrint("Start ADJ_STEP_Z_HOMING_INVERS\r");}
			#endif
			// Z in Nulllage fahren starten
			if((error= m_Adjust_HomingAll(zz_mask, zz_mask_invers)) != errNone)
			{
				#ifdef _CF_DEBUG_PRINT
				if(g_TestFlag & TF_DBG_ASYNC){dbPrint("Start ADJ_STEP_Z_HOMING_INVERS ErrB\r");}
				#endif
				return error;
			}
			m_AdjWaitMask = zz_mask;
			m_AdjGetErrorMask = zz_mask;
			m_Adjust_State = ADJ_STEP_Z_HOMING_INVERS_WAIT;
			//m_AdjStatusRead = 0;
		}
		else
		{m_Adjust_State = ADJ_STEP_XY_MV_OUT_ZERO_START;}
		//{m_Adjust_State = ADJ_STEP_Z_MV_OFFSET_START;}
	}
	if(m_Adjust_State == ADJ_STEP_Z_HOMING_INVERS_WAIT)
	{
		#ifdef _CF_DEBUG_PRINT
		if(g_TestFlag & TF_DBG_ASYNC){dbPrint("Wait ADJ_STEP_Z_HOMING_INVERS\r");}
		#endif
		// Z in Nulllage fahren, Homing Befehl warten
		Running = ASync_GetRunningAllNew(&m_AdjWaitMask);
		if(Running == STATE_ESTOP){return (unsigned short)ERR_SAFETY_APPLIANCE;}
		if(Running == STATE_ERROR)
		{
			#ifdef _CF_DEBUG_PRINT
			if(g_TestFlag & TF_DBG_ASYNC){dbPrint("Wait ADJ_STEP_Z_HOMING_INVERS ErrA\r");}
			#endif
			return ERR_HOME_POS;
		}
		//if(m_AdjWaitMask == 0)
		if(Running == STATE_READY)
		{
			m_Adjust_State = ADJ_STEP_Z_HOMING_NORMAL_START;
		}
	}
	//---------------------------------------------------------------------------------------------------------
	if(m_Adjust_State == ADJ_STEP_Z_HOMING_NORMAL_START)
	{
		#ifdef _CF_DEBUG_PRINT
		if(g_TestFlag & TF_DBG_ASYNC){dbPrint("Start ADJ_STEP_Z_HOMING_NORMAL\r");}
		#endif
		// Z in Nulllage fahren starten
		if(zz_mask_invers)
		{
			if((error= m_Adjust_HomingAll(zz_mask_invers, 0)) != errNone)
			{
				#ifdef _CF_DEBUG_PRINT
				if(g_TestFlag & TF_DBG_ASYNC){dbPrint("Start ADJ_STEP_Z_HOMING_NORMAL ErrB\r");}
				#endif
				return error;
			}
			m_AdjWaitMask = zz_mask_invers;
			m_AdjGetErrorMask = zz_mask_invers;
			m_Adjust_State = ADJ_STEP_Z_HOMING_NORMAL_WAIT;
			//m_AdjStatusRead = 0;
		}
		else
		{
			//m_Adjust_State = ADJ_STEP_XY_MV_OUT_ZERO_START; neu_z_move
			m_Adjust_State = ADJ_STEP_Z_MV_OFFSET_START;
		}
	}
	if(m_Adjust_State == ADJ_STEP_Z_HOMING_NORMAL_WAIT)
	{
		#ifdef _CF_DEBUG_PRINT
		if(g_TestFlag & TF_DBG_ASYNC){dbPrint("Wait ADJ_STEP_Z_HOMING_NORMAL\r");}
		#endif
		// Z in Nulllage fahren, Homing Befehl warten
		Running = ASync_GetRunningAllNew(&m_AdjWaitMask);
		if(Running == STATE_ESTOP){return (unsigned short)ERR_SAFETY_APPLIANCE;}
		if(Running == STATE_ERROR)
		{
			#ifdef _CF_DEBUG_PRINT
			if(g_TestFlag & TF_DBG_ASYNC){dbPrint("Wait ADJ_STEP_Z_HOMING_NORMAL ErrA\r");}
			#endif
			return ERR_HOME_POS;
		}
		//if(m_AdjWaitMask == 0)
		if(Running == STATE_READY)
		{
			//m_Adjust_State = ADJ_STEP_XY_MV_OUT_ZERO_START;  neu_z_move
			m_Adjust_State = ADJ_STEP_Z_MV_OFFSET_START;
		}
	}
// hier offset z fahren start ####################################################################
// hier offset z fahren start ####################################################################
// hier offset z fahren start ####################################################################
	if(m_Adjust_State == ADJ_STEP_Z_MV_OFFSET_START)
	{
		#ifdef _CF_DEBUG_PRINT
		if(g_TestFlag & TF_DBG_ASYNC){dbPrint("Start ADJ_STEP_Z_MV_OFFSET\r");}
		#endif
		// z Offset fahren starten
		if((error= m_Adjust_ResetPositionAll(zz_mask)) != errNone)
		{
			#ifdef _CF_DEBUG_PRINT
			if(g_TestFlag & TF_DBG_ASYNC){dbPrint("Start ADJ_STEP_Z_MV_OFFSET ErrA\r");}
			#endif
			return error;
		}
		/*if((error= m_Adjust_NormalSpeedAll(zz_mask)) != errNone)
		{
			#ifdef _CF_DEBUG_PRINT
			if(g_TestFlag & TF_DBG_ASYNC){dbPrint("Start ADJ_STEP_XY_MV_OFFSET ErrB\r");}
			#endif
			return error;
		}*/
		if((error= m_Adjust_StartOffsetMoveAll(zz_mask)) != errNone)
		{
			#ifdef _CF_DEBUG_PRINT
			if(g_TestFlag & TF_DBG_ASYNC){dbPrint("Start ADJ_STEP_Z_MV_OFFSET ErrC\r");}
			#endif
			return error;
		}
		m_AdjWaitMask = zz_mask;
		m_AdjGetErrorMask = zz_mask;
		m_Adjust_State = ADJ_STEP_Z_MV_OFFSET_WAIT;
		//m_AdjStatusRead = 0;
	}
	if(m_Adjust_State == ADJ_STEP_Z_MV_OFFSET_WAIT)
	{
		#ifdef _CF_DEBUG_PRINT
		if(g_TestFlag & TF_DBG_ASYNC){dbPrint("Wait ADJ_STEP_Z_MV_OFFSET\r");}
		#endif
		// z,  x, y1, y2, y3 Offset fahren warten
		Running = ASync_GetRunningAllNew(&m_AdjWaitMask);
		if(Running == STATE_ESTOP){return (unsigned short)ERR_SAFETY_APPLIANCE;}
		if(Running == STATE_ERROR)
		{
			#ifdef _CF_DEBUG_PRINT
			if(g_TestFlag & TF_DBG_ASYNC){dbPrint("Wait ADJ_STEP_Z_MV_OFFSET ErrA\r");}
			#endif
			return ERR_HOME_POS;
		}
		//if(m_AdjWaitMask == 0)
		if(Running == STATE_READY)
		{
			K_Task_Wait(100);
			if((error= m_Adjust_SetKoordinatensystemAll(m_AdjustMask)) != errNone) {return error;}
			m_Adjust_State = ADJ_STEP_Z_SET_COORDINATE_START;
		}
	}
	//---------------------------------------------------------------------------------------------------------
	if(m_Adjust_State == ADJ_STEP_Z_SET_COORDINATE_START)
	{
		#ifdef _CF_DEBUG_PRINT
		if(g_TestFlag & TF_DBG_ASYNC){dbPrint("Start ADJ_STEP_SET_COORDINATE_SYS\r");}
		#endif
		// z  HomingPosition anfahren, nötig für estop
		if((error= m_Adjust_StartHomingPositionAll(zz_mask)) != errNone)
		{
			#ifdef _CF_DEBUG_PRINT
			if(g_TestFlag & TF_DBG_ASYNC){dbPrint("Start ADJ_STEP_SET_COORDINATE_SYS ErrA\r");}
			#endif
			return error;
		}
		m_AdjWaitMask = zz_mask;
		m_AdjGetErrorMask = zz_mask;
		m_Adjust_State = ADJ_STEP_Z_SET_COORDINATE_WAIT;
		//m_AdjStatusRead = 0;
	}

	if(m_Adjust_State == ADJ_STEP_Z_SET_COORDINATE_WAIT)
	{
		#ifdef _CF_DEBUG_PRINT
		if(g_TestFlag & TF_DBG_ASYNC){dbPrint("Wait ADJ_STEP_SET_COORDINATE_SYS\r");}
		#endif
		// z,  x, y1, y2, y3  fahren warten
		Running = ASync_GetRunningAllNew(&m_AdjWaitMask);
		if(Running == STATE_ESTOP){return (unsigned short)ERR_SAFETY_APPLIANCE;}
		if(Running == STATE_ERROR)
		{
			#ifdef _CF_DEBUG_PRINT
			if(g_TestFlag & TF_DBG_ASYNC){dbPrint("Wait ADJ_STEP_SET_COORDINATE_SYS ErrB\r");}
			#endif
			return ERR_HOME_POS;
		}
		//if(m_AdjWaitMask == 0)
		if(Running == STATE_READY)
		{
			if((error= m_Adjust_NormalSpeedAll(m_AdjustMask)) != errNone)
			{
				#ifdef _CF_DEBUG_PRINT
				if(g_TestFlag & TF_DBG_ASYNC){dbPrint("Start ADJ_STEP_SET_COORDINATE_WAIT ErrC\r");}
				#endif
				return error;
			}
			if((error= m_AsyncImsCmd_SetNormalModeAll(m_AdjustMask)) != errNone)
			{
				/*#ifdef _CF_DEBUG_PRINT
				if(g_TestFlag & TF_DBG_ASYNC){dbPrint("Start ADJ_STEP_SET_COORDINATE_SYS ErrA\r");}
				#endif*/
				return error;
			}
			SetFlagHoming(zz_mask, FLG_HOMING_OK);
			SetRunStatusRdyErr(zz_mask);
			m_Adjust_State = ADJ_STEP_XY_MV_OUT_ZERO_START;
		}
	}

// hier offset z fahren ende ####################################################################
// hier offset z fahren ende ####################################################################
// hier offset z fahren ende ####################################################################
	help = eZ_Ex_Axis;
	xy_mask = m_AdjustMask & ~help;
	//---------------------------------------------------------------------------------------------------------
	if(m_Adjust_State == ADJ_STEP_XY_MV_OUT_ZERO_START)
	{
		// x, y1, y2, y3 aus Nulllage rausfahren starten
		//help = eZ_Ex_Axis;
		//xy_mask = m_AdjustMask & ~help;
		HomeSwitch = 0;
		xy_mask_invers = 0;

		for(i = 0; i < MAX_AXES; i++)
		{
			WDO();
			if((ID = m_GetAxisIDFromMask(xy_mask, i)) != 0)
			{
				if((error = m_AsyncImsCmd_GetHomeSwitchID(ID, &HomeSwitch)) != errNone) {return error;}
				{
					if(HomeSwitch)
					{
						xy_mask_invers = xy_mask_invers | g_FelixAxes[i].AxisBit;
					}
				}
			}
		}
		if(xy_mask)
		{
			/*if((error= m_Adjust_InitAll(xy_mask)) != errNone)
			{
				#ifdef _CF_DEBUG_PRINT
				if(g_TestFlag & TF_DBG_ASYNC){dbPrint("Start ADJ_STEP_XY_MV_OUT_ZERO ErrA\r");}
				#endif
				return error;
			}*/
			if((error= m_Adjust_HomingAll(xy_mask, xy_mask_invers)) != errNone)
			{
				#ifdef _CF_DEBUG_PRINT
				if(g_TestFlag & TF_DBG_ASYNC){dbPrint("Start ADJ_STEP_XY_MV_OUT_ZERO ErrB\r");}
				#endif
				return error;
			}
			m_AdjWaitMask = xy_mask;
			m_AdjGetErrorMask = xy_mask;
			m_Adjust_State = ADJ_STEP_XY_MV_OUT_ZERO_WAIT;
			//m_AdjStatusRead = 0;
		}
		else
		{
			m_Adjust_State = ADJ_STEP_XY_MV_OFFSET_START;//m_Adjust_State = ADJ_NONE;
		}
	}
	if(m_Adjust_State == ADJ_STEP_XY_MV_OUT_ZERO_WAIT)
	{
		// x, y1, y2, y3 aus Nulllage rausfahren warten
		#ifdef _CF_DEBUG_PRINT
		if(g_TestFlag & TF_DBG_ASYNC){dbPrint("Wait ADJ_STEP_XY_MV_OUT_ZERO\r");}
		#endif
		// x, y1, y2, y3 Homing Befehl. warten
		Running = ASync_GetRunningAllNew(&m_AdjWaitMask);
		if(Running == STATE_ESTOP){return (unsigned short)ERR_SAFETY_APPLIANCE;}
		if(Running == STATE_ERROR)
		{
			#ifdef _CF_DEBUG_PRINT
			if(g_TestFlag & TF_DBG_ASYNC){dbPrint("Wait ADJ_STEP_XY_MV_OUT_ZERO ErrA\r");}
			#endif
			return ERR_HOME_POS;
		}
		//if(m_AdjWaitMask == 0)
		if(Running == STATE_READY)
		{
			//m_AsyncImsCmd_HomingEstop(0);
			m_Adjust_State = ADJ_STEP_XY_HOMING_START;
		}
	}
	//---------------------------------------------------------------------------------------------------------
	if(m_Adjust_State == ADJ_STEP_XY_HOMING_START)
	{
		#ifdef _CF_DEBUG_PRINT
		if(g_TestFlag & TF_DBG_ASYNC){dbPrint("Start ADJ_STEP_XY_HOMING\r");}
		#endif
		// x, y1, y2, y3 Homing Befehl starten
		if(xy_mask_invers)
		{
			if((error= m_Adjust_HomingAll(xy_mask_invers, 0)) != errNone)
			{
				#ifdef _CF_DEBUG_PRINT
				if(g_TestFlag & TF_DBG_ASYNC){dbPrint("Start ADJ_STEP_XY_HOMING ErrB\r");}
				#endif
				return error;
			}
			m_AdjWaitMask = xy_mask_invers;
			m_AdjGetErrorMask = xy_mask_invers;
	  		m_Adjust_State = ADJ_STEP_XY_HOMING_WAIT;
			//m_AdjStatusRead = 0;
		}
		else
		{
			m_Adjust_State = ADJ_STEP_XY_MV_OFFSET_START;//m_Adjust_State = ADJ_NONE;
		}
	}
	if(m_Adjust_State == ADJ_STEP_XY_HOMING_WAIT)
	{
		#ifdef _CF_DEBUG_PRINT
		if(g_TestFlag & TF_DBG_ASYNC){dbPrint("Wait ADJ_STEP_XY_HOMING\r");}
		#endif
		// x, y1, y2, y3 Homing Befehl. warten
		Running = ASync_GetRunningAllNew(&m_AdjWaitMask);
		if(Running == STATE_ESTOP){return (unsigned short)ERR_SAFETY_APPLIANCE;}
		if(Running == STATE_ERROR)
		{
			#ifdef _CF_DEBUG_PRINT
			if(g_TestFlag & TF_DBG_ASYNC){dbPrint("Wait ADJ_STEP_XY_HOMING ErrA\r");}
			#endif
			return ERR_HOME_POS;
		}
		if(Running == STATE_READY)
		{
			m_Adjust_State = ADJ_STEP_XY_MV_OFFSET_START;
		}
	}
	//---------------------------------------------------------------------------------------------------------
	if(m_Adjust_State == ADJ_STEP_XY_MV_OFFSET_START)
	{
		#ifdef _CF_DEBUG_PRINT
		if(g_TestFlag & TF_DBG_ASYNC){dbPrint("Start ADJ_STEP_XY_MV_OFFSET\r");}
		#endif
		// z,  x, y1, y2, y3 Offset fahren starten
		//if((error= m_Adjust_ResetPositionAll(m_AdjustMask)) != errNone) neu_z_move
		if((error= m_Adjust_ResetPositionAll(xy_mask)) != errNone)
		{
			#ifdef _CF_DEBUG_PRINT
			if(g_TestFlag & TF_DBG_ASYNC){dbPrint("Start ADJ_STEP_XY_MV_OFFSET ErrA\r");}
			#endif
			return error;
		}
		/*if((error= m_Adjust_NormalSpeedAll(m_AdjustMask)) != errNone)
		{
			#ifdef _CF_DEBUG_PRINT
			if(g_TestFlag & TF_DBG_ASYNC){dbPrint("Start ADJ_STEP_XY_MV_OFFSET ErrB\r");}
			#endif
			return error;
		}*/
		//if((error= m_Adjust_StartOffsetMoveAll(m_AdjustMask)) != errNone) neu_z_move
		if((error= m_Adjust_StartOffsetMoveAll(xy_mask)) != errNone)
		{
			#ifdef _CF_DEBUG_PRINT
			if(g_TestFlag & TF_DBG_ASYNC){dbPrint("Start ADJ_STEP_XY_MV_OFFSET ErrC\r");}
			#endif
			return error;
		}
		//m_AdjWaitMask = m_AdjustMask;  neu_z_move
		//m_AdjGetErrorMask = m_AdjustMask;  neu_z_move
		m_AdjWaitMask = xy_mask;
		m_AdjGetErrorMask = xy_mask;
		m_Adjust_State = ADJ_STEP_XY_MV_OFFSET_WAIT;
		//m_AdjStatusRead = 0;
	}
	if(m_Adjust_State == ADJ_STEP_XY_MV_OFFSET_WAIT)
	{
		#ifdef _CF_DEBUG_PRINT
		if(g_TestFlag & TF_DBG_ASYNC){dbPrint("Wait ADJ_STEP_XY_MV_OFFSET\r");}
		#endif
		// z,  x, y1, y2, y3 Offset fahren warten
		Running = ASync_GetRunningAllNew(&m_AdjWaitMask);
		if(Running == STATE_ESTOP){return (unsigned short)ERR_SAFETY_APPLIANCE;}
		if(Running == STATE_ERROR)
		{
			#ifdef _CF_DEBUG_PRINT
			if(g_TestFlag & TF_DBG_ASYNC){dbPrint("Wait ADJ_STEP_XY_MV_OFFSET ErrA\r");}
			#endif
			return ERR_HOME_POS;
		}
		//if(m_AdjWaitMask == 0)
		if(Running == STATE_READY)
		{
			K_Task_Wait(100);
			//if((error= m_Adjust_SetKoordinatensystemAll(m_AdjustMask)) != errNone) {return error;}  neu_z_move
			if((error= m_Adjust_SetKoordinatensystemAll(xy_mask)) != errNone) {return error;}
			m_Adjust_State = ADJ_STEP_SET_COORDINATE_START;
		}
	}
	//---------------------------------------------------------------------------------------------------------
	if(m_Adjust_State == ADJ_STEP_SET_COORDINATE_START)
	{
		#ifdef _CF_DEBUG_PRINT
		if(g_TestFlag & TF_DBG_ASYNC){dbPrint("Start ADJ_STEP_SET_COORDINATE_SYS\r");}
		#endif
		// z,  x, y1, y2, y3 HomingPosition anfahren, nötig für estop
		//if((error= m_Adjust_StartHomingPositionAll(m_AdjustMask)) != errNone) neu_z_move
		if((error= m_Adjust_StartHomingPositionAll(xy_mask)) != errNone)
		{
			#ifdef _CF_DEBUG_PRINT
			if(g_TestFlag & TF_DBG_ASYNC){dbPrint("Start ADJ_STEP_SET_COORDINATE_SYS ErrA\r");}
			#endif
			return error;
		}
		//m_AdjWaitMask = m_AdjustMask;  neu_z_move
		//m_AdjGetErrorMask = m_AdjustMask;  neu_z_move
		m_AdjWaitMask = xy_mask;
		m_AdjGetErrorMask = xy_mask;
		m_Adjust_State = ADJ_STEP_SET_COORDINATE_WAIT;
		//m_AdjStatusRead = 0;
	}

	if(m_Adjust_State == ADJ_STEP_SET_COORDINATE_WAIT)
	{
		#ifdef _CF_DEBUG_PRINT
		if(g_TestFlag & TF_DBG_ASYNC){dbPrint("Wait ADJ_STEP_SET_COORDINATE_SYS\r");}
		#endif
		// z,  x, y1, y2, y3  fahren warten
		Running = ASync_GetRunningAllNew(&m_AdjWaitMask);
		if(Running == STATE_ESTOP){return (unsigned short)ERR_SAFETY_APPLIANCE;}
		if(Running == STATE_ERROR)
		{
			#ifdef _CF_DEBUG_PRINT
			if(g_TestFlag & TF_DBG_ASYNC){dbPrint("Wait ADJ_STEP_SET_COORDINATE_SYS ErrB\r");}
			#endif
			return ERR_HOME_POS;
		}
		//if(m_AdjWaitMask == 0)
		if(Running == STATE_READY)
		{
			//if((error= m_Adjust_NormalSpeedAll(m_AdjustMask)) != errNone)  neu_z_move
			if((error= m_Adjust_NormalSpeedAll(xy_mask)) != errNone)
			{
				#ifdef _CF_DEBUG_PRINT
				if(g_TestFlag & TF_DBG_ASYNC){dbPrint("Start ADJ_STEP_SET_COORDINATE_WAIT ErrC\r");}
				#endif
				return error;
			}
			//if((error= m_AsyncImsCmd_SetNormalModeAll(m_AdjustMask)) != errNone)  neu_z_move
			if((error= m_AsyncImsCmd_SetNormalModeAll(xy_mask)) != errNone)
			{
				/*#ifdef _CF_DEBUG_PRINT
				if(g_TestFlag & TF_DBG_ASYNC){dbPrint("Start ADJ_STEP_SET_COORDINATE_SYS ErrA\r");}
				#endif*/
				return error;
			}
			//SetFlagHoming(m_AdjustMask, FLG_HOMING_OK); neu_z_move
			//SetRunStatusRdyErr(m_AdjustMask); neu_z_move
			SetFlagHoming(xy_mask, FLG_HOMING_OK);
			SetRunStatusRdyErr(xy_mask);

			m_Adjust_State = ADJ_NONE;
			m_AdjustActive = 0;
			#ifdef _CF_DEBUG_PRINT
			if(g_TestFlag & TF_DBG_ASYNC){dbPrint("HOMING READY\r");}
			#endif
		}
	}
	return errNone;
}

unsigned short ASync_Adjust(unsigned short *ContinueEstop)
{
	unsigned short error;

	if(*ContinueEstop)
	{
		*ContinueEstop = 0;
		if((error = SetHomingVar(m_AdjustMask)) != errNone)
		{
			ResetHomingCtrlVar();
			SetRunStatusRdyErr(m_AdjustMask);
			return error;
		}
	}
	if((error = m_ASync_AdjustExe()) != errNone)
	{
		if(error == (unsigned short)ERR_SAFETY_APPLIANCE)
		{
			return (unsigned short)ERR_SAFETY_APPLIANCE;
		}
		m_AsyncImsCmd_StopMotor();
		ResetHomingCtrlVar();
		SetRunStatusRdyErr(m_AdjustMask);
	}
	return error;
}

unsigned short SetHomingCtrlVar(unsigned short Mask)
{
	m_AdjustMask = Mask;
	return SetHomingVar(m_AdjustMask);
}

unsigned short SetHomingVar(unsigned short Mask)
{
	unsigned short error;
	if((error = ASync_ResetError(Mask)) != errNone){return error;}
	if((error = ASync_ResetHWError(Mask)) != errNone){return error;}
	SetRunStatusBusy(Mask);
	SetFlagHoming(Mask, FLG_HOMING_FAILURE);
	m_AdjustActive = 1;
	SetAdjustState();
	return error;
}

void SetAdjustState(void)
{
	if(m_AdjustMask & eZ_Ex_Axis)
	{
		m_Adjust_State = ADJ_STEP_Z_HOMING_START;
	}
	else
	{
		m_Adjust_State = ADJ_STEP_XY_MV_OUT_ZERO_START;
	}
}

void ResetHomingCtrlVar(void)
{
	m_Adjust_State = ADJ_NONE;
	m_AdjustActive = 0;
}

