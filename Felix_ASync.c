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
#include "MDrive_BusCtrl.h"
#include "MDrive_CmdCtrl.h"
#include "MDriveCmd.h"
#include "Felix_Misc.h"
#include "Felix_HardwareTask.h"
#include "Felix_EStopHandling.h"
#include "Felix_ASync.h"
//#include "Felix_Hardware.h"


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
//IMS_CommandBlock MsgMot;
IMS_CommandBlock m_MsgASyncMot;




//****************************************************************************
// private data
//****************************************************************************


//****************************************************************************
// private functions
//****************************************************************************

//****************************************************************************
// public functions
//****************************************************************************
//für zukünftiges RePositionieren
unsigned short m_Async_ImsCmdMoveMask(unsigned short Cmd, unsigned short Mask)
{
	unsigned char status;
	unsigned short ID;
	unsigned short i;

	m_MsgASyncMot.Error = errNone;
	m_MsgASyncMot.eCommand = Cmd;
	m_MsgASyncMot.Mask = Mask;
	for(i=0; i < MAX_AXES; i++)
	{
		if((ID = m_GetAxisIDFromMask(Mask, i)) != 0)
		{
			m_MsgASyncMot.MoveData[i].ID = ID;
			m_MsgASyncMot.MoveData[i].StartEnable = 1;
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

unsigned short m_Async_ImsCmdID(unsigned short Cmd, unsigned short ID)
{
	unsigned char status;

	m_MsgASyncMot.Error = errNone;
	m_MsgASyncMot.eCommand = Cmd;
	m_MsgASyncMot.AxisID = ID;

	status = K_Mesg_Send_Wait(MBX_ASYNCMOTOR, 0, &m_MsgASyncMot);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	return m_MsgASyncMot.Error;
}

unsigned short m_AsyncImsCmd_StopMotor(void)
{

	unsigned char status;
	//m_MsgASyncMot.AxisID = mask;
	m_MsgASyncMot.Error = errNone;
	m_MsgASyncMot.eCommand = eImsStopMotor;

	status = K_Mesg_Send_Wait(MBX_ASYNCMOTOR, 0, &m_MsgASyncMot);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	return m_MsgASyncMot.Error;
}

unsigned short m_AsyncImsCmd_SetStallFactor(const unsigned short ID, unsigned short value)
{

	unsigned char status;

	if(value > SF_RANGE_MAX){value = SF_RANGE_MAX;}
	m_MsgASyncMot.AxisID = ID;
	m_MsgASyncMot.Error = errNone;
	m_MsgASyncMot.eCommand = eImsSetStallFactor;
	m_MsgASyncMot.Argument.usShort = value;

	status = K_Mesg_Send_Wait(MBX_MOTOR, 0, &m_MsgASyncMot);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	return m_MsgASyncMot.Error;
}

unsigned short m_AsyncImsCmd_SetHoldCurrent(const unsigned short ID, unsigned short value)
{

	unsigned char status;

	if(!CheckUShort(HC_RANGE_MIN, HC_RANGE_MAX, value)){return ERR_WRONG_PARAMETER;}
	m_MsgASyncMot.AxisID = ID;
	m_MsgASyncMot.Error = errNone;
	m_MsgASyncMot.eCommand = eImsSetHoldCurrent;
	m_MsgASyncMot.Argument.usShort = value;

	status = K_Mesg_Send_Wait(MBX_ASYNCMOTOR, 0, &m_MsgASyncMot);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	return m_MsgASyncMot.Error;
}

unsigned short m_AsyncImsCmd_GetPosition(const unsigned short ID, long *value)
{

	unsigned char status;

	m_MsgASyncMot.AxisID = ID;
	m_MsgASyncMot.Error = errNone;
	m_MsgASyncMot.eCommand = eImsGetPosition;

	status = K_Mesg_Send_Wait(MBX_ASYNCMOTOR, 0, &m_MsgASyncMot);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	*value = m_MsgASyncMot.Argument.lLong;
	return m_MsgASyncMot.Error;
}

/*
unsigned short m_AsyncImsCmd_ResetPosition(const unsigned short ID)
{

	unsigned char status;

	m_MsgASyncMot.AxisID = ID;
	m_MsgASyncMot.Error = errNone;
	m_MsgASyncMot.eCommand = eImsResetPosition;

	status = K_Mesg_Send_Wait(MBX_ASYNCMOTOR, 0, &m_MsgASyncMot);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	return m_MsgASyncMot.Error;
}

unsigned short m_AsyncImsCmd_SetDecToAcc(const unsigned short ID)
{

	unsigned char status;

	m_MsgASyncMot.AxisID = ID;
	m_MsgASyncMot.Error = errNone;
	m_MsgASyncMot.eCommand = eImsSetDecToAcc;

	status = K_Mesg_Send_Wait(MBX_ASYNCMOTOR, 0, &m_MsgASyncMot);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	return m_MsgASyncMot.Error;
}
*/

/*unsigned short m_AsyncImsCmd_HomingEstop(unsigned short value)
{

	unsigned char status;

	MsgMot.Error = errNone;
	MsgMot.eCommand = eImsHomingEstop;
	MsgMot.Argument.usShort = value;

	status = K_Mesg_Send_Wait(MBX_MOTOR, 0, &MsgMot);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	return MsgMot.Error;
}*/

unsigned short m_AsyncImsCmd_SetSpeed(const unsigned short ID, float start, float end, float accel)
{

	unsigned char status;

	m_MsgASyncMot.AxisID = ID;
	m_MsgASyncMot.Error = errNone;
	m_MsgASyncMot.eCommand = eImsSetSpeed;
	m_MsgASyncMot.Argument.fFloat = start;
	m_MsgASyncMot.ArgumentB.fFloatB = end;
	m_MsgASyncMot.ArgumentC.fFloatC = accel;
	status = K_Mesg_Send_Wait(MBX_ASYNCMOTOR, 0, &m_MsgASyncMot);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	return m_MsgASyncMot.Error;
}

unsigned short m_AsyncImsCmd_SetPosition(const unsigned short ID, long value)
{

	unsigned char status;

	m_MsgASyncMot.AxisID = ID;
	m_MsgASyncMot.Error = errNone;
	m_MsgASyncMot.eCommand = eImsSetPosition;
	m_MsgASyncMot.Argument.lLong = value;

	status = K_Mesg_Send_Wait(MBX_ASYNCMOTOR, 0, &m_MsgASyncMot);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	return m_MsgASyncMot.Error;
}

/*unsigned short m_AsyncImsCmd_ResetError(const unsigned short ID)
{

	unsigned char status;

	MsgMot.AxisID = ID;
	MsgMot.Error = errNone;
	MsgMot.eCommand = eImsResetError;

	status = K_Mesg_Send_Wait(MBX_ASYNCMOTOR, 0, &MsgMot);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	return MsgMot.Error;
}*/
/*unsigned short m_AsyncImsCmd_ResetStallFlag(const unsigned short ID)
{

	unsigned char status;

	MsgMot.AxisID = ID;
	MsgMot.Error = errNone;
	MsgMot.eCommand = eImsResetStallFlag;

	status = K_Mesg_Send_Wait(MBX_ASYNCMOTOR, 0, &MsgMot);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	return MsgMot.Error;
}*/

// ################################################################################
// ################################################################################
// ################################################################################

//unsigned short m_GetAxisIDFromMask(unsigned short mask, unsigned short AxisBit)
unsigned short m_GetAxisIDFromMask(unsigned short mask, unsigned short i)
{
	if((mask & g_FelixAxes[i].AxisBit) && g_FelixAxes[i].Enable)
	{
		return g_FelixAxes[i].AxisID;
	}
/*
	unsigned short cnt;
	for(cnt = 0; cnt < MAX_AXES; cnt++)
	{
		WDO();
		if((mask & AxisBit & g_FelixAxes[cnt].AxisBit) && g_FelixAxes[cnt].Enable)
		{
			return g_FelixAxes[cnt].AxisID;
		}
	}
*/
	return 0;
}

unsigned short m_CheckMaskBit(unsigned short mask, unsigned short i)
{
	if((mask & g_FelixAxes[i].AxisBit) && g_FelixAxes[i].Enable)
	{
		return 1;
	}
	return 0;
}

unsigned short ASync_ResetError(unsigned short mask)
{
	unsigned short error = errNone;
	unsigned short i;
	unsigned short ID;

	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if((ID = m_GetAxisIDFromMask(mask, i)) != 0)
		{
			m_MsgASyncMot.AxisID = ID;
			m_MsgASyncMot.Error = errNone;
			m_MsgASyncMot.eCommand = eImsResetError;
			error = K_Mesg_Send_Wait(MBX_ASYNCMOTOR, 0, &m_MsgASyncMot);
			if(error != K_OK)
			{
				return ERR_SYSTEM;
			}
			/*if(m_MsgASyncMot.Error == errNone)
			{
				//kann auch wegfallen
				g_SetAxes_ErrorAxisHW(i, 0);
				g_SetAxes_ErrorAxisCyBio(i, errNone);
				g_SetAxes_ErrorFlag(i, 0);
				//g_SetAxes_StallFlag(i, 0);
				//g_SetAxes_Running(i, STATE_READY);
				if(g_GetAxes_Running(i) == STATE_ERROR){g_SetAxes_Running(i, STATE_READY);}
			}*/
			//kann auch wegfallen
			g_SetAxes_ErrorAxisHW(i, 0);
			g_SetAxes_ErrorAxisCyBio(i, errNone);
			g_SetAxes_ErrorFlag(i, 0);
			//g_SetAxes_StallFlag(i, 0);
			//g_SetAxes_Running(i, STATE_READY);
			if(g_GetAxes_Running(i) != STATE_BUSY){g_SetAxes_Running(i, STATE_READY);}
		}
	}
	return error;
}


unsigned short ASync_ResetHWError(unsigned short mask)
{
	unsigned short error = errNone;
	unsigned short i;
	//unsigned short ID;

	/*for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		g_SetAxes_ErrorAxisHWStall(i, 0);
		g_SetAxes_ErrorAxisHW(i, 0);
	}*/
	for(i=0; i < MAX_AXES; i++)
	{
		WDO();
		if(m_CheckMaskBit(mask, i) != 0)
		{
			g_SetAxes_ErrorAxisHWStall(i, 0);
			g_SetAxes_ErrorAxisHW(i, 0);
		}
	}

	return error;
}

void SetFlagHoming(unsigned short mask, unsigned short flag)
{
	unsigned short i;
	for(i=0; i < MAX_AXES; i++)
	{
		WDO();
		if(m_CheckMaskBit(mask, i) != 0)
		{
			//m_MoveData[i].HomingFlag = flag;//NEWERR
			g_SetAxes_HomingFlag(i, flag);
		}
	}
}	

void SetRunStatusRdyErr(unsigned short mask)
{
	unsigned short i;
	for(i=0; i < MAX_AXES; i++)
	{
		WDO();
		if(m_CheckMaskBit(mask, i))
		{
			/*NEWERR
			if(m_MoveData[i].ErrorFlag){m_MoveData[i].Running = STATE_ERROR;}
			else{m_MoveData[i].Running = STATE_READY;}
			*/
			if(g_GetAxes_ErrorFlag(i)){g_SetAxes_Running(i, STATE_ERROR);}
			else{g_SetAxes_Running(i, STATE_READY);}
		}
	}
}
void SetRunStatusBusy(unsigned short mask)
{
	unsigned short i;
	for(i=0; i < MAX_AXES; i++)
	{
		WDO();
		if(m_CheckMaskBit(mask, i) != 0)
		{
			//m_MoveData[i].Running = STATE_BUSY;//NEWERR
			g_SetAxes_Running(i, STATE_BUSY);
		}
	}
}

unsigned short ASync_GetRunningAllNew(unsigned short *mask)
{
	unsigned short error = errNone;
	unsigned short i;
//	unsigned short Counter;
	unsigned short ErrorFlag = 0;
//	unsigned short EStopCounter = 0;
	unsigned short MaskRunAxes, MaskErrorAxes;
// HINWEIS
// STATE_READY erst wenn alle alle Antriebe fertig sind
// wegen, Klappe auf und Antriebe verschieben, dann müssen diese Positionieren.
// somit kein Paralleler Betrieb von Mov, Schake und Homing möglich
// soll das möglich sein, mußß einen verknüpfung von ASync_GetRunningAllNew und ASync_GetRunningAllOld erfolgen
// hierzu muß der Motortask abgefragt werden, ob er in einem Positioniermodus ist nachdem die Klappe 
// offen war und die Antriebe verschoben wurden.
// ASync_GetRunningAllNew wenn im Positioniermodus nachdem die Klappe offen war
// ansonsten die Vorgehensweise von ASync_GetRunningAllOld  verwenden und nur die *mask auswerten.
	m_MsgASyncMot.eCommand = eImsGetStatus;
	error = K_Mesg_Send_Wait(MBX_ASYNCMOTOR, 0, &m_MsgASyncMot);
	if(error != K_OK)
	{
		m_MsgASyncMot.Error = ERR_SYSTEM;
		return STATE_BUSY;
	}
	if(m_MsgASyncMot.EStop)
	{
		return STATE_ESTOP;
	}
	MaskRunAxes = m_MsgASyncMot.MaskRunAxes;// m_MsgASyncMot.Argument.usShort ->>ist eine Maske mit fahrenden Motoren 
	MaskErrorAxes = m_MsgASyncMot.MaskErrorAxes;// m_MsgASyncMot.Argument.usShort ->>ist eine Maske mit fahrenden Motoren 
	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if((*mask & g_FelixAxes[i].AxisBit) && g_FelixAxes[i].Enable)
		{
			if(!(MaskRunAxes & *mask & g_FelixAxes[i].AxisBit))
			{
				g_SetAxes_Running(i, STATE_READY);
				#ifdef _CF_DEBUG_PRINT
				if(g_TestFlag & TF_DBG_ASYNC){dbPrint("STATE_READY i:%u\r", i);}
				#endif


				if(MaskErrorAxes & *mask & g_FelixAxes[i].AxisBit)
				{
					ErrorFlag = 1;
					#ifdef _CF_DEBUG_PRINT
					if(g_TestFlag & TF_DBG_INT_COM){dbPrint("ASGRA i:%u E:%u EF:%u\r", i, g_GetAxes_ErrorAxisCyBio(i), g_GetAxes_ErrorFlag(i));}
					#endif

					g_SetAxes_ErrorFlag(i, 1);
					g_SetAxes_Running(i, STATE_ERROR);
	
				}
				*mask = *mask & (~g_FelixAxes[i].AxisBit);
			}
		}
	}
	if(MaskRunAxes & *mask)
	{
		return STATE_BUSY;
	}
	if(ErrorFlag)
	{
		return STATE_ERROR;
	}
	return STATE_READY;
}

unsigned short m_AsyncImsCmd_SetNormalModeAll(unsigned short mask)
{
	unsigned char status;

	m_MsgASyncMot.Error = errNone;
	m_MsgASyncMot.eCommand = eImsSetMode_Mask;
	m_MsgASyncMot.Mask = mask;
	status = K_Mesg_Send_Wait(MBX_ASYNCMOTOR, 0, &m_MsgASyncMot);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	return m_MsgASyncMot.Error;
}

unsigned short m_AsyncImsCmd_GetHomeSwitchID(const unsigned short ID, unsigned short *value)
{
	unsigned char status;

	m_MsgASyncMot.AxisID = ID;
	m_MsgASyncMot.Error = errNone;
	m_MsgASyncMot.eCommand = eImsGetHomeSwitch;

	status = K_Mesg_Send_Wait(MBX_ASYNCMOTOR, 0, &m_MsgASyncMot);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	*value = m_MsgASyncMot.Argument.usShort;
	return m_MsgASyncMot.Error;
}

unsigned short m_AsyncImsCmd_GetHomeSwitchMask(const unsigned short mask, unsigned short *value)
{
	unsigned short error = errNone;
	unsigned short i;
	unsigned short ID;

	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if((ID = m_GetAxisIDFromMask(mask, i)) != 0)
		{
			return m_AsyncImsCmd_GetHomeSwitchID(ID, value);
		}
	}
	return ERR_UNKNOWN_DRV;
}

/*
unsigned short m_AsyncImsCmd_ImsCmd_SetS1(const unsigned short ID)
{

	unsigned char status;

	Msg.AxisID = ID;
	Msg.Error = errNone;
	Msg.eCommand = eImsSetS1;

	status = K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	return Msg.Error;
}
unsigned short m_AsyncImsCmd_ImsCmd_SetS1Invers(const unsigned short ID)
{

	unsigned char status;

	Msg.AxisID = ID;
	Msg.Error = errNone;
	Msg.eCommand = eImsSetS1Invers;

	status = K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	return Msg.Error;
}

*/

unsigned short CheckDriveHomingOk(unsigned short mask)
{
	unsigned short i;
//----->prüfen ob m_extern_ptr.MoveData[i].Running == STATE_BUSY
//----->beachte estop

	for(i=0; i < MAX_AXES; i++)
	{
		if(m_CheckMaskBit(mask, i))
		{
			if(g_GetAxes_HomingFlag(i) != FLG_HOMING_OK)
			{
				return FALSE;
			}
		}
	}
	return TRUE;
}

unsigned short m_Async_Hippie_Safety(void)
{
	unsigned char status;
#ifdef _CF_DEBUG_PRINT
if(g_TestFlag & TF_DBG_FETRESET){dbPrint("SAFETYPOS ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\r");}
#endif

	m_MsgASyncMot.Error = errNone;
	m_MsgASyncMot.eCommand = eImsHippieSafety;
	m_MsgASyncMot.Argument.cChar = AXIS_ADR_LPK_DEF;
//	m_MsgASyncMot.AxisID = 2;//Missbraucht für AdresseCyBiCtrl
	m_MsgASyncMot.LpkID = LPKINTERN_ID;
	m_MsgASyncMot.AdresseCyBiCtrl = 2;
	status = K_Mesg_Send_Wait(MBX_ASYNCMOTOR, 0, &m_MsgASyncMot);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	return m_MsgASyncMot.Error;
}
unsigned short m_Async_Hippie_Restore(void)
{
	unsigned char status;
#ifdef _CF_DEBUG_PRINT
if(g_TestFlag & TF_DBG_FETRESET){dbPrint("RESTORE +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\r");}
#endif

	m_MsgASyncMot.Error = errNone;
	m_MsgASyncMot.eCommand = eImsHippieRestore;
	m_MsgASyncMot.Argument.cChar = AXIS_ADR_LPK_DEF;
//	m_MsgASyncMot.AxisID = 2;//Missbraucht für AdresseCyBiCtrl
	m_MsgASyncMot.LpkID = LPKINTERN_ID;
	m_MsgASyncMot.AdresseCyBiCtrl = 2;
	status = K_Mesg_Send_Wait(MBX_ASYNCMOTOR, 0, &m_MsgASyncMot);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	return m_MsgASyncMot.Error;

}

unsigned short m_Async_Hippie_Continue(char *pszAnswerString)
{
	unsigned char status;
#ifdef _CF_DEBUG_PRINT
if(g_TestFlag & TF_DBG_FETRESET){dbPrint("CONTINUE +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\r");}
#endif

	m_MsgASyncMot.Error = errNone;
	m_MsgASyncMot.eCommand = eImsHippieContinue;
	m_MsgASyncMot.Argument.cChar = AXIS_ADR_LPK_DEF;
//	m_MsgASyncMot.AxisID = 2;//Missbraucht für AdresseCyBiCtrl
	m_MsgASyncMot.LpkID = LPKINTERN_ID;
	m_MsgASyncMot.AdresseCyBiCtrl = 2;
	status = K_Mesg_Send_Wait(MBX_ASYNCMOTOR, 0, &m_MsgASyncMot);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	strcpy(pszAnswerString, (char *)m_MsgASyncMot.Argument.cBuffer);
	return m_MsgASyncMot.Error;
}

unsigned short m_Async_LPKGetSensorSwitchMagazinInsert(unsigned short *value)
{
	unsigned char status;

	m_MsgASyncMot.Error = errNone;
	m_MsgASyncMot.eCommand = eLPKGetSensorSwitchMagzInsert;
	m_MsgASyncMot.LpkID = LPKINTERN_ID;
	m_MsgASyncMot.Argument.cChar = AXIS_ADR_LPK_DEF;
	m_MsgASyncMot.AdresseCyBiCtrl = 2;
	status = K_Mesg_Send_Wait(MBX_ASYNCMOTOR, 0, &m_MsgASyncMot);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	*value = m_MsgASyncMot.Argument.usShort;
	return m_MsgASyncMot.Error;
}
