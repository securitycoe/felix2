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
//#include "Felix_ASyncSearchAdapter.h"
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
#define SEARCHADAPTER_NONE 0
#define SEARCHADAPTER_MOV_START 1
#define SEARCHADAPTER_MOV_WAIT 2
#define SEARCHADAPTER_GET_SWITCH 3

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
unsigned short m_SearchAdapterActive = 0;
unsigned short m_SearchAdapterMask = 0;
//****************************************************************************
// private data
//****************************************************************************
//unsigned short m_SearchAdapterCounter = 0;
unsigned short m_SearchAdapter_State = SEARCHADAPTER_NONE;
unsigned short m_SearchAdapterActiveMask;
unsigned short m_SearchAdapterWaitMask = 0;
unsigned short m_SearchAdapterGetErrorMask = 0;

udtASyncAxesSearchAdapter m_SearchAdapterData[MAX_AXES];




//****************************************************************************
// private functions
//****************************************************************************
//****************************************************************************
// public functions
//****************************************************************************
unsigned short ASync_SearchAdapterData(ASync_CommandBlock * ptr);
void SetSearchAdapterCtrlVar(unsigned short Mask);
void ResetSearchAdapterCtrlVar(void);
unsigned short m_ASync_MoveSearchAdapterAll(unsigned short Mask);

unsigned short ASync_SearchAdapter(void);
unsigned short ASync_SearchAdapterExe(void);

unsigned short ASync_SearchAdapterData(ASync_CommandBlock * ptr)
{
	unsigned short error = ERR_WRONG_PARAMETER;
	unsigned short i;
	unsigned short ID;
	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if((ID = m_GetAxisIDFromMask(ptr->Mask, i)) != 0)
		{
			error = errNone;
			m_SearchAdapterData[i].RelativeWayStep = RoundToLong(ptr->MoveData[i].RelativeWay * pIMSFixedPara[i]->fMotImpProMM);
			m_SearchAdapterData[i].ZielPosBasicStep = RoundToLong(ptr->MoveData[i].AbsolutePos * pIMSFixedPara[i]->fMotImpProMM);
			m_SearchAdapterData[i].ZielPosMaximalStep = m_SearchAdapterData[i].ZielPosBasicStep + m_SearchAdapterData[i].RelativeWayStep;
			m_SearchAdapterData[i].ZielPosAktuellStep = m_SearchAdapterData[i].ZielPosBasicStep;
			m_SearchAdapterData[i].DeltaStep = RoundToLong(ptr->MoveData[i].DeltaWay * pIMSFixedPara[i]->fMotImpProMM);
//			m_SearchAdapterData[i].DeltaStep = RoundToLong((double)0.1 * pIMSFixedPara[i]->fMotImpProMM);
			m_SearchAdapterData[i].SwitchContact = 0;
			m_SearchAdapterData[i].MaxPosFlag = 0;
			//m_SearchAdapterData[i].Counter = m_SearchAdapterData[i].RelativeWayStep / m_SearchAdapterData[i].DeltaStep;
		}
	}
	return error;
}
void SetSearchAdapterCtrlVar(unsigned short Mask)
{
	m_SearchAdapterActive = 1;
	m_SearchAdapterMask = Mask;

	SetRunStatusBusy(m_SearchAdapterMask);

	m_SearchAdapterActiveMask = m_SearchAdapterMask;
	m_SearchAdapterGetErrorMask = m_SearchAdapterMask;
	m_SearchAdapterWaitMask = m_SearchAdapterMask;

	//m_SearchAdapterCounter = ;
	m_SearchAdapter_State = SEARCHADAPTER_MOV_START;
}

void ResetSearchAdapterCtrlVar(void)
{
	m_SearchAdapter_State = SEARCHADAPTER_NONE;
	m_SearchAdapterActive = 0;
}



unsigned short ASync_SearchAdapter(void)
{
	unsigned short error;

	if((error = ASync_SearchAdapterExe()) != errNone)
	{
		if(error == (unsigned short)ERR_SAFETY_APPLIANCE)
		{
			return (unsigned short)ERR_SAFETY_APPLIANCE;
		}
		m_AsyncImsCmd_StopMotor();
		m_SearchAdapterActive = 0;
		m_SearchAdapter_State = SEARCHADAPTER_NONE;
		SetRunStatusRdyErr(m_SearchAdapterMask);
	}
	return error;
}

unsigned short ASync_SearchAdapterExe(void)
{
	unsigned short error = errNone;
	//unsigned short helpmask;
	unsigned short Running;
	unsigned short i, ID;
	unsigned short Switch = 0;
	long actNewStep, maxZielStep;

	if(m_SearchAdapter_State == SEARCHADAPTER_MOV_START)
	{
		if(m_SearchAdapterActiveMask  & eZ_Axis)
		{
			if((error= m_ASync_MoveSearchAdapterAll(m_SearchAdapterActiveMask & eZ_Axis)) != errNone) {return error;}
			m_SearchAdapterWaitMask = m_SearchAdapterActiveMask;
			m_SearchAdapterGetErrorMask = m_SearchAdapterActiveMask;
			m_SearchAdapter_State = SEARCHADAPTER_MOV_WAIT;
		}
		else
		{
			return ERR_WRONG_PARAMETER;
			//m_SearchAdapter_State = SEARCHADAPTER_NONE;
		}
	}
	if(m_SearchAdapter_State == SEARCHADAPTER_MOV_WAIT)
	{
		Running = ASync_GetRunningAllNew(&m_SearchAdapterWaitMask);
		if(Running == STATE_ESTOP){return (unsigned short)ERR_SAFETY_APPLIANCE;}
		if(Running == STATE_ERROR)
		{
			return ERR_MOVE;
		}
		if(Running == STATE_READY)
		{
			m_SearchAdapter_State = SEARCHADAPTER_GET_SWITCH;
		}
	}
	if(m_SearchAdapter_State == SEARCHADAPTER_GET_SWITCH)
	{
		for(i = 0; i < MAX_AXES; i++)
		{
			WDO();
			if((ID = m_GetAxisIDFromMask(m_SearchAdapterActiveMask, i)) != 0)
			{
				// Pentadisk: m_Async_LPKGetSensorSwitchMagzInsert, entsprechend einer der Z-Achsedie die dazugehörige KopfAdresse oder ID mitgeben
				if((error = m_Async_LPKGetSensorSwitchMagazinInsert(&Switch)) != errNone)
				{
					return error;
				}
				if(Switch)
				{
					m_SearchAdapterActiveMask = m_SearchAdapterActiveMask & ~g_FelixAxes[i].AxisBit;
					m_SearchAdapterData[i].SwitchContact = 1;
				}
				else
				{
					if(m_SearchAdapterData[i].MaxPosFlag)
					{
						m_SearchAdapterActiveMask = m_SearchAdapterActiveMask & ~g_FelixAxes[i].AxisBit;
					}
					else
					{
						m_SearchAdapterData[i].ZielPosAktuellStep += m_SearchAdapterData[i].DeltaStep;
						//if(m_SearchAdapterData[i].ZielPosAktuellStep > m_SearchAdapterData[i].ZielPosMaximalStep)
						actNewStep = labs(m_SearchAdapterData[i].ZielPosAktuellStep);
						maxZielStep = labs(m_SearchAdapterData[i].ZielPosMaximalStep);
						if(m_SearchAdapterData[i].RelativeWayStep < 0)
						{
							if(actNewStep > maxZielStep)
							{
								m_SearchAdapterData[i].ZielPosAktuellStep = m_SearchAdapterData[i].ZielPosMaximalStep;
								m_SearchAdapterData[i].MaxPosFlag = 1;
							}
						}
						else
						{
							if(actNewStep < maxZielStep)
							{
								m_SearchAdapterData[i].ZielPosAktuellStep = m_SearchAdapterData[i].ZielPosMaximalStep;
								m_SearchAdapterData[i].MaxPosFlag = 1;
							}
						}
					}
				} // end else if(Switch)
			}
		}// end for...

		if(m_SearchAdapterActiveMask == 0)
		{
			m_SearchAdapter_State = SEARCHADAPTER_NONE;
			m_SearchAdapterActive = 0;
			for(i = 0; i < MAX_AXES; i++)
			{
				WDO();
				if((ID = m_GetAxisIDFromMask(m_SearchAdapterMask, i)) != 0)
				{
					if(!m_SearchAdapterData[i].SwitchContact)
					{
						g_SetAxes_ErrorAxisCyBio(i, ERR_SENSOR_SEARCH_FAILED);
						g_SetAxes_ErrorFlag(i, 1);
					}
				}
			}
			//SearchAdapter_RestoreMoveDriveParaAll(m_SearchAdapterMask);
			SetRunStatusRdyErr(m_SearchAdapterMask);
		}
		else
		{
			m_SearchAdapter_State = SEARCHADAPTER_MOV_START;
		}
	}
	return error;	
}

unsigned short m_ASync_MoveSearchAdapterAll(unsigned short Mask)
{
	unsigned char status;
	unsigned short ID;
	unsigned short i;

	m_MsgASyncMot.Error = errNone;
	m_MsgASyncMot.eCommand = eImsStartMultipleMoveAbsolute;

	for(i=0; i < MAX_AXES; i++)
	{
		if((ID = GetAxisIDFromMask(Mask, g_FelixAxes[i].AxisBit)) != 0)
		{
			/*value = m_SearchAdapterData[i].ZielPosAktuellStep;
			if(pIMSFixedPara[i]->fIgrImpulseProRev < 0.0)
			{
				value*= -1;
			}
			m_MsgASyncMot.MoveData[i].StepWay = value;*/
			m_MsgASyncMot.MoveData[i].ID = ID;
			m_MsgASyncMot.MoveData[i].StartEnable = 1;
			m_MsgASyncMot.MoveData[i].StepWay = m_SearchAdapterData[i].ZielPosAktuellStep;
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
