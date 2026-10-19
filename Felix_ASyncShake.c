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
#include "Felix_ASyncShake.h"
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
unsigned short m_ShakeActive = 0;
unsigned short m_ShakeMask = 0;
//****************************************************************************
// private data
//****************************************************************************
unsigned short m_ShakeCounter = 0;
unsigned short m_Shake_State = SHAKE_NONE;
unsigned short m_ShakeWaitMask = 0;
unsigned short m_ShakeGetErrorMask = 0;

udtASyncAxesShakeData m_ShakeData[MAX_AXES];

//****************************************************************************
// private functions
//****************************************************************************
unsigned short m_ASync_MoveShakeAll(unsigned short mask, unsigned short ShakeStep);
unsigned short m_ASync_MoveShakeStep(unsigned short Mask, unsigned short ShakeStep);
unsigned short m_ASync_ShakeDataStep(unsigned short ID, unsigned short i, double Relative);

unsigned short ASync_ShakeExe(void);

unsigned short m_ASync_MoveShakeAll(unsigned short mask, unsigned short ShakeStep)
{
	unsigned short error;

	if((error = m_ASync_MoveShakeStep(mask, ShakeStep)) != errNone)
	{
		//SetErrorForMask(mask, error);
	}
	return error;
}
unsigned short m_ASync_MoveShakeStep(unsigned short Mask, unsigned short ShakeStep)
{
	unsigned char status;
	unsigned short ID;
	unsigned short i;
	long value;

	m_MsgASyncMot.Error = errNone;
	m_MsgASyncMot.eCommand = eImsStartMultipleMoveAbsolute;
	m_MsgASyncMot.Mask = Mask;
	for(i=0; i < MAX_AXES; i++)
	{
		if((ID = GetAxisIDFromMask(Mask, g_FelixAxes[i].AxisBit)) != 0)
		{
			//value =  m_MoveData[i].AbsolutePos * pIMSFixedPara[i]->fMotImpProMM;
			m_MsgASyncMot.MoveData[i].ID = ID;
			m_MsgASyncMot.MoveData[i].StartEnable = 1;
			switch(ShakeStep)
			{
				case SHAKE_STEP1_X_POS1_START: value = m_ShakeData[i].Auslenkung1;
										break;
				case SHAKE_STEP2_Y_POS1_START: value = m_ShakeData[i].Auslenkung1;
										break;
				case SHAKE_STEP3_X_POS2_START: value = m_ShakeData[i].Auslenkung2;
										break;
				case SHAKE_STEP4_Y_POS2_START: value = m_ShakeData[i].Auslenkung2;
										break;
				case SHAKE_STEP5_XY_RESTORE_START: value = m_ShakeData[i].RestorePos;
										break;
				default:				value = m_ShakeData[i].RestorePos;
										break;
			}
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

//****************************************************************************
// public functions
//****************************************************************************
void SetShakeCtrlVar(unsigned short Mask, unsigned short Count)
{
	m_ShakeActive = 1;
	m_ShakeMask = Mask;
	SetRunStatusBusy(m_ShakeMask);
	m_ShakeGetErrorMask = m_ShakeMask;
	m_ShakeWaitMask = m_ShakeMask;
	m_ShakeCounter = Count;
	m_Shake_State = SHAKE_STEP1_X_POS1_START;
}

void ResetShakeCtrlVar(void)
{
	m_Shake_State = SHAKE_NONE;
	m_ShakeActive = 0;
}

unsigned short Shake_RestoreMoveDriveParaAll(unsigned short mask)
{
	unsigned short error = errNone;
	unsigned short i;
	unsigned short ID;
	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if((ID = m_GetAxisIDFromMask(mask, i)) != 0)
		{
			if((error = m_AsyncImsCmd_SetStallFactor( ID,  pIMSChangedPara[i]->usStallsteps)) != errNone)
			{
				//SetErrorForBit(i,error);
				return error;
			}
			if((error = m_AsyncImsCmd_SetHoldCurrent( ID, pIMSChangedPara[i]->usHoldCurrent)) != errNone)
			{
				//SetErrorForBit(i,error);
				return error;
			}

		}
	}
	return error;
}

//unsigned short ASync_ShakeDataAll(unsigned short mask)
unsigned short ASync_ShakeDataAll(ASync_CommandBlock * ptr)
{
	unsigned short error = errNone;
	unsigned short i;
	unsigned short ID;
	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if((ID = m_GetAxisIDFromMask(ptr->Mask, i)) != 0)
		{
			
			if((error = m_ASync_ShakeDataStep(ID, i, ptr->MoveData[i].RelativeWay)) != errNone)
			{
				//SetErrorForBit(i,error);
				#ifdef _CF_DEBUG_PRINT
				if(g_TestFlag & TF_DBG_INT_COM){dbPrint("ASSDA i:%u E:%u EF:%u\r", i, g_GetAxes_ErrorAxisCyBio(i), g_GetAxes_ErrorFlag(i));}
				#endif
				return error;
			}
		}
	}
	return error;
}


unsigned short m_ASync_ShakeDataStep(unsigned short ID, unsigned short i, double Relative)
{

	unsigned short error;
	long AktPosSteps;
	long auslenkung;
	//float AbsPosMM, ZielPosMM1, ZielPosMM2, Relative;
	double AbsPosMM, ZielPosMM1, ZielPosMM2;//, Relative;
	//, min, max;
	if((error = m_AsyncImsCmd_GetPosition(ID, &AktPosSteps)) != errNone){return error;}
	//Relative = m_extern_ptr->MoveData[i].RelativeWay;
	auslenkung = RoundToLong(Relative * pIMSFixedPara[i]->fMotImpProMM);
	if(pIMSFixedPara[i]->fIgrImpulseProRev < 0.0)
	{
		auslenkung*= -1;
		Relative*= (double)-1.0;
	}
	AbsPosMM = (double)AktPosSteps / pIMSFixedPara[i]->fENCImpProMM;
	//ZielPosMM1 = AbsPosMM + m_extern_ptr->MoveData[i].RelativeWay;
	ZielPosMM1 = AbsPosMM + Relative;
/*	min = pIMSFixedPara[i]->fMinWay - pIMSFixedPara[i]->fPrecisionDeadband - pIMSFixedPara[i]->fPrecisionDeadband;
	max = pIMSFixedPara[i]->fMaxWay + pIMSFixedPara[i]->fPrecisionDeadband + pIMSFixedPara[i]->fPrecisionDeadband;
printf("min: %3.3f  max: %3.3f\r", min, max);fflush(stdout);
printf("Ist: %3.3f Ziel1: %3.3f \r", AbsPosMM, ZielPosMM);fflush(stdout);		
	if(!CheckFloat(min, max, ZielPosMM)){return ERR_VALUE_OUT_OF_RANGE;}
	ZielPosMM = AbsPosMM - m_extern_ptr->MoveData[i].RelativeWay;
printf("Ist: %3.3f Ziel2: %3.3f \r", AbsPosMM, ZielPosMM);fflush(stdout);			
	if(!CheckFloat(min, max, ZielPosMM)){return ERR_VALUE_OUT_OF_RANGE;}	*/

	/*if(ZielPosMM1 < pIMSFixedPara[i]->fMinWay) {ZielPosMM1 = pIMSFixedPara[i]->fMinWay;}
	if(ZielPosMM1 > pIMSFixedPara[i]->fMaxWay) {ZielPosMM1 = pIMSFixedPara[i]->fMaxWay;}*/
	if(ZielPosMM1 < pIMSChangedPara[i]->fMinWay) {ZielPosMM1 = pIMSChangedPara[i]->fMinWay;}
	if(ZielPosMM1 > pIMSChangedPara[i]->fMaxWay) {ZielPosMM1 = pIMSChangedPara[i]->fMaxWay;}

	//ZielPosMM2 = AbsPosMM - m_extern_ptr->MoveData[i].RelativeWay;
	ZielPosMM2 = AbsPosMM - Relative;
	/*if(ZielPosMM2 < pIMSFixedPara[i]->fMinWay) {ZielPosMM2 = pIMSFixedPara[i]->fMinWay;}
	if(ZielPosMM2 > pIMSFixedPara[i]->fMaxWay) {ZielPosMM2 = pIMSFixedPara[i]->fMaxWay;}*/
	if(ZielPosMM2 < pIMSChangedPara[i]->fMinWay) {ZielPosMM2 = pIMSChangedPara[i]->fMinWay;}
	if(ZielPosMM2 > pIMSChangedPara[i]->fMaxWay) {ZielPosMM2 = pIMSChangedPara[i]->fMaxWay;}
	
	m_ShakeData[i].RestorePos = AktPosSteps;
	/*m_ShakeData[i].Auslenkung1 = AktPosSteps + auslenkung;
	m_ShakeData[i].Auslenkung2 = AktPosSteps - auslenkung;*/
	m_ShakeData[i].Auslenkung1 = RoundToLong(ZielPosMM1 * pIMSFixedPara[i]->fMotImpProMM);
	m_ShakeData[i].Auslenkung2 = RoundToLong(ZielPosMM2 * pIMSFixedPara[i]->fMotImpProMM);
	if(pIMSFixedPara[i]->fIgrImpulseProRev < 0.0)
	{
		m_ShakeData[i].Auslenkung1*= -1;
		m_ShakeData[i].Auslenkung2*= -1;
	}

	if((error = m_AsyncImsCmd_SetStallFactor( ID,  labs(auslenkung) + pIMSChangedPara[i]->usStallsteps)) != errNone){return error;}
	if((error = m_AsyncImsCmd_SetHoldCurrent( ID, pIMSChangedPara[i]->usRunCurrent)) != errNone){return error;}
	return error;
}

unsigned short ASync_Shake(void)
{
	unsigned short error;

	if((error = ASync_ShakeExe()) != errNone)
	{
		if(error == (unsigned short)ERR_SAFETY_APPLIANCE)
		{
			return (unsigned short)ERR_SAFETY_APPLIANCE;
		}
		m_AsyncImsCmd_StopMotor();
		ResetShakeCtrlVar();
		SetRunStatusRdyErr(m_ShakeMask);
		//Shake_RestoreMoveDriveParaAll(m_ShakeMask);
	}
	//Shake_RestoreMoveDriveParaAll(m_ShakeMask);24.10.2019
	return error;
}
unsigned short ASync_ShakeExe(void)
{
	unsigned short error;
	unsigned short helpmask;
	unsigned short Running;
	// 1. shake move X, hin
	if(m_Shake_State == SHAKE_STEP1_X_POS1_START)
	{
		#ifdef _CF_FELIX
		if(m_ShakeMask  & eX_Axis)
		{
			if((error= m_ASync_MoveShakeAll(eX_Axis, SHAKE_STEP1_X_POS1_START)) != errNone) {return error;}
			m_ShakeWaitMask = eX_Axis;
			m_ShakeGetErrorMask = eX_Axis;
			//m_ShakeStatusRead = 0;
			m_Shake_State = SHAKE_STEP1_X_POS1_WAIT;
		}
		#endif
		//---- PENTA DISK -----------------
		#ifdef _CF_PENTA_DISK
		if(m_ShakeMask  & eDisk_Axis)
		{
			if((error= m_ASync_MoveShakeAll(eDisk_Axis, SHAKE_STEP1_X_POS1_START)) != errNone) {return error;}
			m_ShakeWaitMask = eDisk_Axis;
			m_ShakeGetErrorMask = eDisk_Axis;
			//m_ShakeStatusRead = 0;
			m_Shake_State = SHAKE_STEP1_X_POS1_WAIT;
		}
		#endif

		else
		{
			m_Shake_State = SHAKE_STEP2_Y_POS1_START;
		}
	}
	if(m_Shake_State == SHAKE_STEP1_X_POS1_WAIT)
	{
		Running = ASync_GetRunningAllNew(&m_ShakeWaitMask);
		if(Running == STATE_ESTOP){return (unsigned short)ERR_SAFETY_APPLIANCE;}
		if(Running == STATE_ERROR){return ERR_MOVE;}
		//if(m_ShakeWaitMask == 0)
		if(Running == STATE_READY)
		{
			m_Shake_State = SHAKE_STEP2_Y_POS1_START;
		}
	}
	// 2. shake move y, hin
	if(m_Shake_State == SHAKE_STEP2_Y_POS1_START)
	{
		#ifdef _CF_FELIX
		if(m_ShakeMask  & (eFrontCarriage_Axis | eRearCarriage_Axis | eBottomCarriage))
		{
			helpmask = m_ShakeMask  & (eFrontCarriage_Axis | eRearCarriage_Axis | eBottomCarriage);
			if((error= m_ASync_MoveShakeAll(helpmask, SHAKE_STEP2_Y_POS1_START)) != errNone) {return error;}
			m_ShakeWaitMask = helpmask;
			m_ShakeGetErrorMask = helpmask;
			//m_ShakeStatusRead = 0;
			m_Shake_State = SHAKE_STEP2_Y_POS1_WAIT;
		}
		else
		{
			m_Shake_State = SHAKE_STEP3_X_POS2_START;
		}
		#endif
		//---- PENTA DISK -----------------
		#ifdef _CF_PENTA_DISK
		m_Shake_State = SHAKE_STEP3_X_POS2_START;
		#endif
	}
	if(m_Shake_State == SHAKE_STEP2_Y_POS1_WAIT)
	{
		Running = ASync_GetRunningAllNew(&m_ShakeWaitMask);
		if(Running == STATE_ESTOP){return (unsigned short)ERR_SAFETY_APPLIANCE;}
		if(Running == STATE_ERROR){return ERR_MOVE;}
		//if(m_ShakeWaitMask == 0)
		if(Running == STATE_READY)
		{
			m_Shake_State = SHAKE_STEP3_X_POS2_START;
		}
	}
	// 3. shake move X, zurück
	if(m_Shake_State == SHAKE_STEP3_X_POS2_START)
	{
		#ifdef _CF_FELIX		
		if(m_ShakeMask  & eX_Axis)
		{
			if((error= m_ASync_MoveShakeAll(eX_Axis, SHAKE_STEP3_X_POS2_START)) != errNone) {return error;}
			m_ShakeWaitMask = eX_Axis;
			m_ShakeGetErrorMask = eX_Axis;
			//m_ShakeStatusRead = 0;
			m_Shake_State = SHAKE_STEP3_X_POS2_WAIT;
		}
		#endif
		//---- PENTA DISK -----------------
		#ifdef _CF_PENTA_DISK
		if(m_ShakeMask  & eDisk_Axis)
		{
			if((error= m_ASync_MoveShakeAll(eDisk_Axis, SHAKE_STEP3_X_POS2_START)) != errNone) {return error;}
			m_ShakeWaitMask = eDisk_Axis;
			m_ShakeGetErrorMask = eDisk_Axis;
			//m_ShakeStatusRead = 0;
			m_Shake_State = SHAKE_STEP3_X_POS2_WAIT;
		}
		#endif
		else
		{
			m_Shake_State = SHAKE_STEP4_Y_POS2_START;
		}
	}
	if(m_Shake_State == SHAKE_STEP3_X_POS2_WAIT)
	{
		Running = ASync_GetRunningAllNew(&m_ShakeWaitMask);
		if(Running == STATE_ESTOP){return (unsigned short)ERR_SAFETY_APPLIANCE;}
		if(Running == STATE_ERROR){return ERR_MOVE;}
		//if(m_ShakeWaitMask == 0)
		if(Running == STATE_READY)
		{
			m_Shake_State = SHAKE_STEP4_Y_POS2_START;
		}
	}
	// 4. shake move Y, zurück
	if(m_Shake_State == SHAKE_STEP4_Y_POS2_START)
	{
		#ifdef _CF_FELIX		
		if(m_ShakeMask  & (eFrontCarriage_Axis | eRearCarriage_Axis | eBottomCarriage))
		{
			helpmask = m_ShakeMask  & (eFrontCarriage_Axis | eRearCarriage_Axis | eBottomCarriage);
			if((error= m_ASync_MoveShakeAll(helpmask, SHAKE_STEP4_Y_POS2_START)) != errNone) {return error;}
			m_ShakeWaitMask = helpmask;
			m_ShakeGetErrorMask = helpmask;
			//m_ShakeStatusRead = 0;
			m_Shake_State = SHAKE_STEP4_Y_POS2_WAIT;
		}
		else
		{
			//if(m_ShakeCounter != 0){m_ShakeStartCounter = SHAKE_STEP1_X_POS1;m_ShakeWaitCounter = SHAKE_NONE;}
			//else{m_ShakeStartCounter = SHAKE_STEP5_XY_RESTORE;m_ShakeWaitCounter = SHAKE_NONE;}
			m_Shake_State = SHAKE_STEP5_XY_RESTORE_START;
		}
		#endif
		//---- PENTA DISK -----------------
		#ifdef _CF_PENTA_DISK
		m_Shake_State = SHAKE_STEP5_XY_RESTORE_START;
		#endif
	}
	if(m_Shake_State == SHAKE_STEP4_Y_POS2_WAIT)
	{
		Running = ASync_GetRunningAllNew(&m_ShakeWaitMask);
		if(Running == STATE_ESTOP){return (unsigned short)ERR_SAFETY_APPLIANCE;}
		if(Running == STATE_ERROR){return ERR_MOVE;}
		//if(m_ShakeWaitMask == 0)
		if(Running == STATE_READY)
		{
			m_Shake_State = SHAKE_STEP5_XY_RESTORE_START;
		}
	}
	// 5. restore pre shake position
	if(m_Shake_State == SHAKE_STEP5_XY_RESTORE_START)
	{
		if(--m_ShakeCounter == 0)
		{
			if((error= m_ASync_MoveShakeAll(m_ShakeMask, SHAKE_STEP5_XY_RESTORE_START)) != errNone) {return error;}
			m_ShakeWaitMask = m_ShakeMask;
			m_ShakeGetErrorMask = m_ShakeMask;
			//m_ShakeStatusRead = 0;
			m_Shake_State = SHAKE_STEP5_XY_RESTORE_WAIT;
		}
		else
		{
			m_Shake_State = SHAKE_STEP1_X_POS1_START;
		}
	}
	if(m_Shake_State == SHAKE_STEP5_XY_RESTORE_WAIT)
	{
		Running = ASync_GetRunningAllNew(&m_ShakeWaitMask);
		if(Running == STATE_ESTOP){return (unsigned short)ERR_SAFETY_APPLIANCE;}
		if(Running == STATE_ERROR){return ERR_MOVE;}
		//if(m_ShakeWaitMask == 0)
		if(Running == STATE_READY)
		{
			m_Shake_State = SHAKE_NONE;
			m_ShakeActive = 0;
			//Shake_RestoreMoveDriveParaAll(m_ShakeMask);
			SetRunStatusRdyErr(m_ShakeMask);
		}
	}
	return errNone;	
}
