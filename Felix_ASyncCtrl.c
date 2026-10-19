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
#include "DeviceConfig.h"
#include "Cxconfig.h"
#include "Felix_ASync.h"
#include "Felix_ASyncHoming.h"
#include "Felix_ASyncShake.h"
#include "Felix_ASyncMove.h"
#include "Felix_ASyncRePos.h"
#include "Felix_ASyncCtrl.h"

#include "Felix_Hardware.h"
#include "Drv_config.h"
#include "MDrive_CmdCtrl.h"
#include "MDriveCmd.h"
#include "Felix_Misc.h"
#include "Felix_HardwareTask.h"
#include "Felix_EStopHandling.h"
#include "Felix_BCL8.h"
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
extern unsigned short ASync_SearchAdapterData(ASync_CommandBlock * ptr);
extern void SetSearchAdapterCtrlVar(unsigned short Mask);
extern void ResetSearchAdapterCtrlVar(void);
extern unsigned short ASync_SearchAdapter(void);
//****************************************************************************
// external data
//****************************************************************************
extern unsigned short m_SearchAdapterActive;
extern unsigned short m_SearchAdapterMask;
//****************************************************************************
// internal functions
//****************************************************************************

//****************************************************************************
// public data
//****************************************************************************
ASync_CommandBlock g_MsgASync;
//****************************************************************************
// private data
//****************************************************************************
ASync_CommandBlock * m_extern_ptr;
//udtASyncAxesMoveData m_MoveData[MAX_AXES];

//unsigned short m_MoveAsyncMask = 0;
//unsigned short m_MoveSyncMask = 0;

//unsigned short m_AdjStatusRead = 0;
unsigned short m_MvStatusRead = 0;
//unsigned short m_ShakeStatusRead = 0;

unsigned short m_EstopContinue = 0;
unsigned short m_EstopNotActive = 1;

udtAsyncSaveMoveToPos m_AsyncSaveMoveToPos;
//unsigned short m_EnableEStopContinueDrive = 0;
//unsigned short m_WaitForEStopClose = 0;
/*double dbg_double;
double dbg_doublelong;
double dbg_doublestep;
double dbg_doubleneg;*/
#ifdef _CF_DEBUG_POSERROR
udtDebug DebugData[MAX_AXES];
#endif
#define DEBUG_ERROR 4000

//****************************************************************************
// private functions
//****************************************************************************
//void InitASyncStatus(void);
//unsigned short m_GetAxisIDFromMask(unsigned short mask, unsigned short AxisBit);
unsigned short m_ResetMaskBit(unsigned short mask, unsigned short Axis);
unsigned short CheckDriveReadyForUse(unsigned short mask);

//unsigned short ASync_GetRunningAllOld(unsigned short *mask);
unsigned short ASync_MoveAbsoluteAll(unsigned short mask, unsigned short Mode1536);
unsigned short ASync_MoveAbsoluteStart(unsigned short Mask, unsigned short Mode1536);
unsigned short m_AsyncImsCmd_ResetEStopMov(void);
#ifdef _CF_DEBUG_POSERROR
void DebugDataPrint_i(unsigned short i);
void DebugDataPrint(unsigned short all, unsigned short i_or_mask);
void DebugDataClear(unsigned short Mask);
#endif

void SaveMoveParameter(unsigned short Mask, unsigned short Mode1536);
//****************************************************************************
// public functions
//****************************************************************************
/*void InitASyncStatus(void)
{
	unsigned short i;
	for(i=0; i < MAX_AXES; i++)
	{
		WDO();
		//m_MoveData[i].Runing = STATE_READY;
		//m_MoveData[i].HomingFlag = FLG_HOMING_FAILURE;//NEWERR
		//m_MoveData[i].ErrorFlag = 0;//NEWERR
		//m_MoveData[i].StallFlag = 0;//NEWERR
//		m_MoveData[i].StartEnable = 0;
	}
}*/

unsigned short CheckDriveReadyForUse(unsigned short mask)
{
	unsigned short i;
//----->prüfen ob m_extern_ptr.MoveData[i].Running == STATE_BUSY
//----->beachte estop

	for(i=0; i < MAX_AXES; i++)
	{
		if(m_CheckMaskBit(mask, i) != 0)
		{
			//if((m_MoveData[i].Running == STATE_BUSY) || (m_MoveData[i].Running == STATE_BUSY_ESTOP))
			//if(m_MoveData[i].Running == STATE_BUSY)//NEWERR
			if(g_GetAxes_Running(i) == STATE_BUSY)
			{
				return FALSE;
			}
		}
	}
	return TRUE;
}


unsigned short m_ResetMaskBit(unsigned short mask, unsigned short Axis)
{
	return mask & ~Axis;
}
/*
unsigned short ASync_GetRunningAllOld(unsigned short *mask)
{
	unsigned short error = errNone;
//*mask = 0;//no compiler warning
	unsigned short i;
	unsigned short ID;

	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if((ID = m_GetAxisIDFromMask(*mask, i)) != 0)
		{
			if(m_MsgASyncMot.MoveData[i].Running == STATE_READY)
			{
				#ifdef _CF_DEBUG_PRINT
				if(g_TestFlag & TF_DBG_INT_COM){dbPrint("ASGRA R i:%u\r", i);}
				#endif
				*mask = m_ResetMaskBit(*mask, i);
			}
		 	if(m_MsgASyncMot.MoveData[i].Running == STATE_ERROR)
			{
				*mask = m_ResetMaskBit(*mask, i);
				m_MoveData[i].Error = m_MsgASyncMot.MoveData[i].Error;
				m_MoveData[i].ErrorFlag = 1;
				error = m_MsgASyncMot.MoveData[i].Error;
				#ifdef _CF_DEBUG_PRINT
				if(g_TestFlag & TF_DBG_INT_COM){dbPrint("ASGRA i:%u E:%u EF:%u\r", i,  m_MoveData[i].Error, m_MoveData[i].ErrorFlag);}
				#endif
			}
			*if((m_MsgASyncMot.MoveData[i].Running == STATE_BUSY) || (m_MsgASyncMot.MoveData[i].Running == STATE_BUSY_ESTOP))
			{
				m_MoveData[i].Running = m_MsgASyncMot.MoveData[i].Running;
			}*
		}
	}

	return error;
}
*/
void SaveMoveParameter(unsigned short Mask, unsigned short Mode1536)
{
	unsigned short i;
	m_AsyncSaveMoveToPos.Mask = Mask;
	m_AsyncSaveMoveToPos.Mode1536 = Mode1536;
	for(i=0; i < MAX_AXES; i++)
	{
		if(m_CheckMaskBit(Mask, i))
		{
			m_AsyncSaveMoveToPos.AbsolutePos[i] = m_extern_ptr->MoveData[i].AbsolutePos;
		}
	}
}

unsigned short ASync_MoveAbsoluteAll(unsigned short mask, unsigned short Mode1536)
{
//	unsigned short error;
	
	SaveMoveParameter(mask, Mode1536);
	return ASync_MoveAbsoluteStart(mask, Mode1536);
}
unsigned short ASync_MoveAbsoluteStart(unsigned short Mask, unsigned short Mode1536)
{
	unsigned char status;
	unsigned short ID;
	unsigned short i;
	long lSteps;
	double dSteps;
	#ifdef _CF_PENTA_DISK
	double IstGrad, fhelp_1, fhelp_2, current_angle, diff_angle, MoveTo;
	#endif
#ifdef _CF_DEBUG_POSERROR
	long lStepWay;
	unsigned short debugerror = 0;
#endif

	m_MsgASyncMot.Error = errNone;
	m_MsgASyncMot.eCommand = eImsStartMultipleMoveAbsolute;
	m_MsgASyncMot.Mask = Mask;
	m_MsgASyncMot.Argument.usShort = Mode1536;

	for(i=0; i < MAX_AXES; i++)
	{
		if((ID = m_GetAxisIDFromMask(Mask, i)) != 0)
		{
			#ifdef _CF_PENTA_DISK
			if(g_FelixAxes[i].AxisBit == eDisk_Axis)
			{
				if((status = m_AsyncImsCmd_GetPosition(ID, &lSteps)) != errNone){return status;}
				m_MsgASyncMot.Error = errNone;
				m_MsgASyncMot.eCommand = eImsStartMultipleMoveAbsolute;
				m_MsgASyncMot.Mask = Mask;
				m_MsgASyncMot.Argument.usShort = Mode1536;


				IstGrad = lSteps / pIMSFixedPara[i]->fENCImpProMM;
				fhelp_1 = IstGrad / (double)360.000;
				fhelp_2 = fhelp_1 - (long)fhelp_1; // Normieren
				current_angle = fhelp_2 * (double)360.000;//aktuelle Pos.(Winkel)(kleiner 360 Grad)
				//differenz Winkel, Fahrweg bestimmen
				diff_angle = m_AsyncSaveMoveToPos.AbsolutePos[i] - current_angle;
				if(diff_angle > (double)360.000)
				{
					fhelp_1 = diff_angle / (double)360.000;
					fhelp_2 = fhelp_1 - (long)fhelp_1; // Normieren
					
					diff_angle = (double)360.000 * fhelp_2;
				}

				if(fabs(diff_angle) > (double)180.000)
				{
					if(diff_angle > (double)0.000) diff_angle = diff_angle - (double)360.000;
					else diff_angle = diff_angle + (double)360.000;
				}
				MoveTo = IstGrad + diff_angle;
//dbPrint("\rIstAbs: %f  IstAngle: %f  Diff: %f  MoveTo: %f\r", IstGrad, current_angle, diff_angle, MoveTo);
	
			}
			#endif
//-------------------------------------------
			#ifdef _CF_DEBUG_POSERROR
			#ifdef _CF_PENTA_DISK
			if(g_FelixAxes[i].AxisBit == eDisk_Axis)
			{
				DebugData[i].AbsolutePos = MoveTo;
			}
			else
			{
				DebugData[i].AbsolutePos = m_AsyncSaveMoveToPos.AbsolutePos[i];
			}
			#endif
			#ifdef _CF_FELIX
			DebugData[i].AbsolutePos = m_AsyncSaveMoveToPos.AbsolutePos[i];
			#endif

			DebugData[i].MotImpProMM = pIMSFixedPara[i]->fMotImpProMM;
			//dSteps =  m_AsyncSaveMoveToPos.AbsolutePos[i] * pIMSFixedPara[i]->fMotImpProMM;
			dSteps =  DebugData[i].AbsolutePos * DebugData[i].MotImpProMM;
			DebugData[i].dSteps = dSteps;

			lSteps = RoundToLong(dSteps);
			DebugData[i].lSteps = lSteps;
			
			if(pIMSFixedPara[i]->fIgrImpulseProRev < 0.0)
			{
				//value*= -1;
				lStepWay = lSteps * -1;
			}
			else
			{
				lStepWay = lSteps;
			}
			DebugData[i].lStepWay = lStepWay;
			m_MsgASyncMot.MoveData[i].ID = ID;
			m_MsgASyncMot.MoveData[i].StartEnable = 1;
			//m_MsgASyncMot.MoveData[i].StepWay = lSteps;
			m_MsgASyncMot.MoveData[i].StepWay = lStepWay;
			DebugData[i].lMotStepWay = m_MsgASyncMot.MoveData[i].StepWay;
			DebugData[i].mm_dSteps = dSteps / pIMSFixedPara[i]->fMotImpProMM;
			DebugData[i].mm_lSteps = (double)lSteps / pIMSFixedPara[i]->fMotImpProMM;
			DebugData[i].mm_lStepWay = (double)lStepWay / pIMSFixedPara[i]->fMotImpProMM;
			DebugData[i].mm_lMotStepWay = (double)m_MsgASyncMot.MoveData[i].StepWay / pIMSFixedPara[i]->fMotImpProMM;
			DebugData[i].db_mm_dSteps = DebugData[i].dSteps / DebugData[i].MotImpProMM;
			DebugData[i].db_mm_lSteps = (double)DebugData[i].lSteps / DebugData[i].MotImpProMM;
			if(pIMSFixedPara[i]->fIgrImpulseProRev < 0.0)
			{
				DebugData[i].mm_lStepWay = DebugData[i].mm_lStepWay * (double)-1.0;
				DebugData[i].mm_lMotStepWay = DebugData[i].mm_lMotStepWay * (double)-1.0;
			}
			//if(Mode1536)
			DebugData[i].Error = 0;
			//if(!CheckDouble(m_AsyncSaveMoveToPos.AbsolutePos[i] - pIMSChangedPara[idx]->fPrecisionDeadband,double max,dbg_double)
			if(!CheckDouble(m_AsyncSaveMoveToPos.AbsolutePos[i] - (double)0.1, m_AsyncSaveMoveToPos.AbsolutePos[i] + (double)0.1, DebugData[i].mm_dSteps))
			{
				DebugData[i].Error|=0x0001;
				debugerror = 1;
			}
			if(!CheckDouble(m_AsyncSaveMoveToPos.AbsolutePos[i] - (double)0.1, m_AsyncSaveMoveToPos.AbsolutePos[i] + (double)0.1, DebugData[i].mm_lSteps))
			{
				DebugData[i].Error|=0x0002;
				debugerror = 1;
			}
			if(!CheckDouble(m_AsyncSaveMoveToPos.AbsolutePos[i] - (double)0.1, m_AsyncSaveMoveToPos.AbsolutePos[i] + (double)0.1, DebugData[i].mm_lStepWay))
			{
				DebugData[i].Error|=0x0004;
				debugerror = 1;
			}

			if(!CheckDouble(m_AsyncSaveMoveToPos.AbsolutePos[i] - (double)0.1, m_AsyncSaveMoveToPos.AbsolutePos[i] + (double)0.1, DebugData[i].mm_lMotStepWay))
			{
				DebugData[i].Error|=0x0008;
				debugerror = 1;
			}
			if(!CheckDouble(DebugData[i].AbsolutePos - (double)0.1, DebugData[i].AbsolutePos + (double)0.1, DebugData[i].db_mm_dSteps))
			{
				DebugData[i].Error|=0x0010;
				debugerror = 1;
			}
			if(!CheckDouble(DebugData[i].AbsolutePos - (double)0.1, DebugData[i].AbsolutePos + (double)0.1, DebugData[i].db_mm_lSteps))
			{
				DebugData[i].Error|=0x0020;
				debugerror = 1;
			}
			if(debugerror)
			{
				if(g_TestFlag & TF_DBG_CALC_STEPS)
				{
					dbPrint("\rPosCalc i:%u dstep:%f lstep:%ld lStepWay:%ld \r", i, dSteps, lSteps, lStepWay);
					DebugDataPrint_i(i);
				}
			}
			#else
			#ifdef _CF_PENTA_DISK
			if(g_FelixAxes[i].AxisBit == eDisk_Axis)
			{
				dSteps =  MoveTo * pIMSFixedPara[i]->fMotImpProMM;
			}
			else
			{
				dSteps =  m_AsyncSaveMoveToPos.AbsolutePos[i] * pIMSFixedPara[i]->fMotImpProMM;
			}
			#endif
			#ifdef _CF_FELIX
			dSteps =  m_AsyncSaveMoveToPos.AbsolutePos[i] * pIMSFixedPara[i]->fMotImpProMM;
			#endif
			lSteps = RoundToLong(dSteps);
			if(pIMSFixedPara[i]->fIgrImpulseProRev < 0.0)
			{
				lSteps*= -1;
			}
			m_MsgASyncMot.MoveData[i].ID = ID;
			m_MsgASyncMot.MoveData[i].StartEnable = 1;
			m_MsgASyncMot.MoveData[i].StepWay = lSteps;
			#endif
//-------------------------------------------
		}
		else{m_MsgASyncMot.MoveData[i].StartEnable = 0;}
	}
#ifdef _CF_DEBUG_POSERROR
debugerror = 0;
if(debugerror)
{
	return DEBUG_ERROR;
}
#endif
	status = K_Mesg_Send_Wait(MBX_ASYNCMOTOR, 0, &m_MsgASyncMot);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	return m_MsgASyncMot.Error;
}

#ifdef _CF_DEBUG_POSERROR
void DebugDataPrint(unsigned short all, unsigned short i_or_mask)
{
	unsigned short i;
	if(all)
	{
		for(i=0; i < MAX_AXES; i++)
		{
			WDO();
			if(GetAxisIDFromMask(i_or_mask, g_FelixAxes[i].AxisBit) != 0)
			DebugDataPrint_i(i);
		}
	}
	else
	{
		DebugDataPrint_i(i_or_mask);
	}
}
void DebugDataPrint_i(unsigned short i)
{
WDO();
dbPrint("i:%u Err:%u uv:%f dduv:%f\r",i, DebugData[i].Error, pIMSFixedPara[i]->fMotImpProMM, DebugData[i].MotImpProMM);
WDO();
dbPrint("Pos:%f CmdPos:%f MotStep:%ld\r", m_AsyncSaveMoveToPos.AbsolutePos[i], DebugData[i].CmdPos, m_MsgASyncMot.MoveData[i].StepWay);
WDO();
dbPrint("DD Pos:%f dstep:%f lstep:%ld lStepWay:%ld MotStep:%ld\r", DebugData[i].AbsolutePos, DebugData[i].dSteps, DebugData[i].lSteps, DebugData[i].lStepWay, DebugData[i].lMotStepWay);
WDO();
dbPrint("MM dSteps:%f lstep:%f lStepWay:%f MotStep:%f\r", DebugData[i].mm_dSteps, DebugData[i].mm_lSteps, DebugData[i].mm_lStepWay, DebugData[i].mm_lMotStepWay);
WDO();
dbPrint("DD dd_dstep:%f dd_lstep:%f\r", DebugData[i].db_mm_dSteps, DebugData[i].db_mm_lSteps);
}
void DebugDataClear(unsigned short Mask)
{
	unsigned short i;
	for(i=0; i < MAX_AXES; i++)
	{
		WDO();
		if(GetAxisIDFromMask(Mask, g_FelixAxes[i].AxisBit) != 0)
		{
			DebugData[i].AbsolutePos = (double)0.0;
			DebugData[i].MotImpProMM = (double)0.0;
			DebugData[i].dSteps = (double)0.0;
			DebugData[i].lSteps = (long)0;
			DebugData[i].lStepWay = (long)0;
			DebugData[i].lMotStepWay = (long)0;
			DebugData[i].mm_dSteps = (double)0.0;
			DebugData[i].mm_lSteps = (double)0.0;
			DebugData[i].mm_lStepWay = (double)0.0;
			DebugData[i].mm_lMotStepWay = (double)0.0;
			DebugData[i].db_mm_dSteps = (double)0.0;
			DebugData[i].db_mm_lSteps = (double)0.0;
			DebugData[i].CmdPos = (double)0.0;
			DebugData[i].Running = 0;
			DebugData[i].Error =0;
		}
	}
}
#endif

unsigned short m_AsyncImsCmd_ResetEStopMov(void)
{

	unsigned char status;

//	m_MsgASyncMot.AxisID = ID;

	m_MsgASyncMot.Error = errNone;
	m_MsgASyncMot.eCommand = eResetEStopMov;

	status = K_Mesg_Send_Wait(MBX_ASYNCMOTOR, 0, &m_MsgASyncMot);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	return m_MsgASyncMot.Error;
}

//###########################################################################################
//###########################################################################################
//###########################################################################################
void ASyncCtrlTask(void)
{
	unsigned short event;
	unsigned short error = errNone;
	//unsigned char status;
	unsigned short EventWaitTime;
	unsigned short BarcodeJustage = 0;
	unsigned short BarcodeOutput = 1;
	unsigned short i;
#ifdef _CF_DEBUG_POSERROR
DebugDataClear(eAllMask);
#endif
	//InitASyncStatus();

//	K_Task_Wait(1000);
	while(1)
	{
		//-------------------------------------------------------------
		/*m_MsgASyncMot.eCommand = eImsGetStatus;
		
		status = K_Mesg_Send_Wait(MBX_ASYNCMOTOR, 0, &m_MsgASyncMot);
		if(status != K_OK){m_MsgASyncMot.Error = ERR_SYSTEM;error = ERR_SYSTEM;m_AdjStatusRead = 0;}
		else{m_AdjStatusRead = 1;
		*for(i=0; i < MAX_AXES; i++)
		{
			m_MoveData[i].Running = m_MsgASyncMot.MoveData[i].Running;
			if(m_CheckMaskBit(m_AdjustMask | m_ShakeMask, i) != 0)
			{
				//für extern immer erstmal auf busy setzen, wegen komplexen kommandos homing und shake
				if(m_MoveData[i].Running != STATE_ERROR)
				{
					m_MoveData[i].Running = STATE_BUSY;
				}
			}
		}*
		}*/
		//-------------------------------------------------------------
		if(m_AdjustActive || m_MovActive || m_ShakeActive || m_SearchAdapterActive){EventWaitTime = 10;}
		else{EventWaitTime = 40;}
		event = K_Event_Wait(EVENT_ASYNCCTRL_MBX, EventWaitTime, 2);
		if(event & EVENT_ASYNCCTRL_MBX)
		{
			LED_3 = 1;
			m_extern_ptr = K_Mesg_Get(MBX_ASYNCCTRL);	// auf Message warten
			WDO();
			switch(m_extern_ptr->eCommand)
			{
				case eASyncAdjust		:	m_extern_ptr->Error = errNone;
											if(m_AdjustActive)
											{
												m_extern_ptr->Error = ERR_DEVICE_BUSY;
											}
											else
											{
												if(CheckDriveReadyForUse(m_extern_ptr->Mask))
												{
													m_extern_ptr->Error = SetHomingCtrlVar(m_extern_ptr->Mask);
												}
												else
												{
													m_extern_ptr->Error = ERR_DEVICE_BUSY;
												}
											}
											break; 
				case eASyncShake		:	if(CheckDriveReadyForUse(m_extern_ptr->Mask) && !m_ShakeActive)
											{
												if(CheckDriveHomingOk(m_extern_ptr->Mask))
												{
													if((m_extern_ptr->Error = ASync_ShakeDataAll(m_extern_ptr)) == errNone)
													{
														SetShakeCtrlVar(m_extern_ptr->Mask, m_extern_ptr->Argument.usShort);
													}
													else
													{
														Shake_RestoreMoveDriveParaAll(m_extern_ptr->Mask);
													}
												}
												else
												{
													m_extern_ptr->Error = ERR_DRV_NOT_CALIBRATE;
													//!!!! eventuell noch die achsen error setzen
												}
											}
											else
											{
												m_extern_ptr->Error = ERR_DEVICE_BUSY;
											}
											break;
				case eSearchAdapter		:	if(CheckDriveReadyForUse(m_extern_ptr->Mask))
											{
												if(CheckDriveHomingOk(m_extern_ptr->Mask))
												{
													if((m_extern_ptr->Error = ASync_SearchAdapterData(m_extern_ptr)) == errNone)
													{
														SetSearchAdapterCtrlVar(m_extern_ptr->Mask);
													}
													/*else
													{
														SearchAdapter_RestoreMoveDriveParaAll(m_extern_ptr->Mask);
													}*/
												}
												else
												{
													m_extern_ptr->Error = ERR_DRV_NOT_CALIBRATE;
													//!!!! eventuell noch die achsen error setzen
												}
											}
											else
											{
												m_extern_ptr->Error = ERR_DEVICE_BUSY;
											}
											break;


				case eASyncMv		:		if(CheckDriveReadyForUse(m_extern_ptr->Mask))
											{
												if(CheckDriveHomingOk(m_extern_ptr->Mask))
												{
													m_extern_ptr->Error = ASync_MoveAbsoluteAll(m_extern_ptr->Mask, m_extern_ptr->Argument.usShort);
													if(m_extern_ptr->Error != errNone){m_AsyncImsCmd_StopMotor();}
													else
													{
														//m_MovActive = 1;m_MovMask = m_extern_ptr->Mask;SetRunStatusBusy(m_MovMask);m_MovGetErrorMask = m_MovMask;m_MovWaitMask = m_MovMask;
														SetMoveCtrlVar(m_extern_ptr->Mask);
													}
												}
												else
												{
													m_extern_ptr->Error = ERR_DRV_NOT_CALIBRATE;
													//!!!! eventuell noch die achsen error setzen
												}
											}
											else
											{
												m_extern_ptr->Error = ERR_DEVICE_BUSY;
											}
											break;
				case eASyncTeach	:		if(CheckDriveHomingOk(m_extern_ptr->Mask))
											{
												m_extern_ptr->Error = ASync_MoveAbsoluteAll(m_extern_ptr->Mask, 1);
												if(m_extern_ptr->Error != errNone){m_AsyncImsCmd_StopMotor();}
												else
												{
													//m_MovActive = 1;m_MovMask = m_extern_ptr->Mask;SetRunStatusBusy(m_MovMask);m_MovGetErrorMask = m_MovMask;m_MovWaitMask = m_MovMask;
													SetMoveCtrlVar(m_extern_ptr->Mask);
												}
											}
											else
											{
												m_extern_ptr->Error = ERR_DRV_NOT_CALIBRATE;
												//!!!! eventuell noch die achsen error setzen
											}
										 	break;
				case eASyncGetError		:	
											for(i=0; i < MAX_AXES; i++)
											{
												WDO();
												if(m_CheckMaskBit(m_extern_ptr->Mask, i) != 0)
												{
													// 2016.03.01 m_extern_ptr->MoveData[i].ErrorFlag = g_GetAxes_ErrorFlag(i);
													error = errNone;
													if(g_GetAxes_ErrorFlag(i))
													{
														//m_extern_ptr->MoveData[i].Error = g_GetAxes_ErrorAxisCyBio(i);
														error = g_GetAxes_ErrorAxisCyBio(i);
														if(error == errNone)
														{
															error = ERR_MOVE;
														}
													}
													m_extern_ptr->MoveData[i].Error = error;
													#ifdef _CF_DEBUG_PRINT
													//if(g_TestFlag & TF_DBG_INT_COM){dbPrint("ASGE i:%u E:%u EF:%u SF:%u\r", i, g_GetAxes_ErrorAxisCyBio(i), g_GetAxes_ErrorFlag(i), g_GetAxes_StallFlag(i));}
													if(g_TestFlag & TF_DBG_INT_COM){dbPrint("ASGE i:%u E:%u EF:%u\r", i, g_GetAxes_ErrorAxisCyBio(i), g_GetAxes_ErrorFlag(i));}
													#endif
												}
											}
											m_extern_ptr->Error = errNone;
											break;
				case eASyncGetStatus   	:	
											for(i=0; i < MAX_AXES; i++)
											{
												WDO();
												if(m_ShakeActive || m_AdjustActive || m_SearchAdapterActive)
												{
													if(m_ShakeActive)
													{
														if(m_CheckMaskBit(m_ShakeMask, i) != 0)
														m_extern_ptr->MoveData[i].Running = STATE_BUSY;
													}
													if(m_AdjustActive)
													{
														if(m_CheckMaskBit(m_AdjustMask, i) != 0)
														{m_extern_ptr->MoveData[i].Running = STATE_BUSY;}
													}
													if(m_SearchAdapterActive)
													{
														if(m_CheckMaskBit(m_SearchAdapterMask, i) != 0)
														m_extern_ptr->MoveData[i].Running = STATE_BUSY;
													}
												}
												else
												{
													if(m_CheckMaskBit(m_extern_ptr->Mask, i) != 0)
													{
														//m_extern_ptr->MoveData[i].Running = m_MoveData[i].Running;//NEWERR
														m_extern_ptr->MoveData[i].Running = g_GetAxes_Running(i);
													}
												}
											}
											//m_extern_ptr->EStop = g_isEstopFlag();
											m_extern_ptr->EStop = g_isGenerateEStopError();
											m_extern_ptr->Error = errNone;
											break;
				case eASyncGetAdjState:		
											for(i=0; i < MAX_AXES; i++)
											{
												WDO();
												if(m_CheckMaskBit(m_extern_ptr->Mask, i) != 0)
												{
													//m_extern_ptr->MoveData[i].HomingFlag = m_MoveData[i].HomingFlag;//NEWERR
													//m_extern_ptr->MoveData[i].HomingFlag = g_GetAxes_HomingFlag(i);
												}
											}
											m_extern_ptr->Error = errNone;
											break;
				case eASyncClearError		:
											m_extern_ptr->Error = ASync_ResetError(m_extern_ptr->Mask);
											break;
				case eASyncResetEStopMov:  	
											m_EstopNotActive = 1;
											m_EstopContinue = 0;
											ResetHomingCtrlVar();
											ResetShakeCtrlVar();
											ResetSearchAdapterCtrlVar();
											for(i=0; i < MAX_AXES; i++)
											{
												WDO();
												SetRunStatusRdyErr( g_FelixAxes[i].AxisBit);
											}
											m_extern_ptr->Error = m_AsyncImsCmd_ResetEStopMov();
											break;
				case eASyncEStopMoveContinue:
											m_extern_ptr->Error = errNone;
											/*m_extern_ptr->Error = m_Async_Hippie_Continue();
											if(m_extern_ptr->Error == errNone)
											{
												if(CheckDriveHomingOk(eAllMask))
												{
													m_EstopContinue = 1;
													m_EstopNotActive = 1;
													if(m_MovActive)
													{
														m_extern_ptr->Error = ASync_MoveAbsoluteStart(m_AsyncSaveMoveToPos.Mask, m_AsyncSaveMoveToPos.Mode1536);
														if(m_extern_ptr->Error != errNone){m_AsyncImsCmd_StopMotor();}
														else
														{
															SetMoveCtrlVar(m_extern_ptr->Mask);
														}
													}
												}
											}*/
											if(m_AdjustActive)
											{
												m_EstopContinue = 1;
												m_EstopNotActive = 1;
											}
											else
											{
												if(CheckDriveHomingOk(eAllMask))
												{
													//08-2024 m_extern_ptr->Error = m_Async_Hippie_Continue();
													//08-2024if(m_extern_ptr->Error == errNone)
													//08-2024{
														//m_EstopContinue = 1;
														m_EstopNotActive = 1;
														if(m_MovActive)
														{
															m_extern_ptr->Error = ASync_EStopContinue();
															if(m_extern_ptr->Error == errNone)
															{
																m_extern_ptr->Error = ASync_MoveAbsoluteStart(m_AsyncSaveMoveToPos.Mask, m_AsyncSaveMoveToPos.Mode1536);
																	if(m_extern_ptr->Error != errNone){m_AsyncImsCmd_StopMotor();}
																else
																{
																	SetMoveCtrlVar(m_extern_ptr->Mask);
																}
															}
														}
														/*if(m_ShakeActive || m_SearchAdapterActive)
														{
														}*/
													//08-2024}
												}
											}
										break;
				//08-2024
				case eASyncEStopHippieMoveContinue:
													m_extern_ptr->Error = m_Async_Hippie_Continue((char *)m_extern_ptr->Argument.cBuffer);
/*													if((m_extern_ptr->Error == errNone)||(m_extern_ptr->Error == errInfo))
													{
!!HIER
													}*/

										break;
				case eASyncEStopMoveSavetyPos: 	
											if(CheckDriveHomingOk(eAllMask))
											{
												m_extern_ptr->Error = ASync_EStopMoveSavetyPos();
											}
											else
											{
												m_extern_ptr->Error = m_Async_Hippie_Safety();
											}
										break;
				case eASyncEStopMoveRestorePos:
											m_extern_ptr->Error = errNone;
											if(CheckDriveHomingOk(eAllMask))
											{
												m_extern_ptr->Error = ASync_EStopMoveRestorePos();
											}
											else
											{
												m_extern_ptr->Error = m_Async_Hippie_Restore();
											}
										break;
				case eASyncEStopRestoreCurrentAndSetPos:
											if(CheckDriveHomingOk(eAllMask))
											{
												m_extern_ptr->Error = ASync_EStopMoveSavetyPos();
											}
											else
											{
												m_extern_ptr->Error = m_Async_Hippie_Safety();
											}
										break;

				case ASyncBarcodeJustage:
										BarcodeJustage = m_extern_ptr->Argument.usShort;
										m_extern_ptr->Error = errNone;
										if(BarcodeJustage)
										{
											BarcodeOutput =  m_extern_ptr->Mask;
											BCL8_StartReaderJustage();
											m_extern_ptr->Error = BCL8_ReadReaderJustage(BarcodeOutput);
											if(m_extern_ptr->Error != errNone)
											{
												BarcodeJustage = 0;
												BCL8_StopReaderJustage();
											}
										}
										else
										{
											BCL8_StopReaderJustage();
										}
										m_extern_ptr->Error = errNone;
										break;

				default					:	m_extern_ptr->Error = ERR_CMD_NOT_IMPLEMENTED;
											break; 
			}//end...switch(m_ptr->eCommand)
			K_Mesg_Ack_Sender();
			K_Task_Wait(1);//K_Task_Coop_Sched();//K_Task_Wake(UserMainSlot);
			LED_3 = 0;
		}//end... if(event & EVENT_MOTOR_MBX)
		if(BarcodeJustage)
		{
			if(BCL8_ReadReaderJustage(BarcodeOutput) != errNone)
			{BarcodeJustage = 0;}
		}
		if(m_AdjustActive || m_MovActive || m_ShakeActive || m_SearchAdapterActive)
		{	LED_3 = 1;
		}
		if(m_AdjustActive && m_EstopNotActive)
		{
			if((error = ASync_Adjust(&m_EstopContinue)) != errNone)
			{
				if(error == (unsigned short)ERR_SAFETY_APPLIANCE)
				{
					m_EstopNotActive = 0;
				}
			}
		}
		if(m_MovActive && m_EstopNotActive)
		{
			if((error = ASync_Mov()) != errNone)
			{
				if(error == (unsigned short)ERR_SAFETY_APPLIANCE)
				{
					m_EstopNotActive = 0;
				}
				else
				{
					m_MovActive = 0;
				}
			}
		}
		if(m_ShakeActive && m_EstopNotActive)
		{
			if((error = ASync_Shake()) != errNone)
			{
				if(error == (unsigned short)ERR_SAFETY_APPLIANCE)
				{
					m_EstopNotActive = 0;
				}
				else
				{
					/*m_AsyncImsCmd_StopMotor();
					m_ShakeActive = 0;
					SetRunStatusRdyErr(m_ShakeMask);*/
					Shake_RestoreMoveDriveParaAll(m_ShakeMask);
				}
			}
		}
		if(m_SearchAdapterActive && m_EstopNotActive)
		{
			if((error = ASync_SearchAdapter()) != errNone)
			{
				if(error == (unsigned short)ERR_SAFETY_APPLIANCE)
				{
					m_EstopNotActive = 0;
				}
				else
				{
					/*m_AsyncImsCmd_StopMotor();
					m_SearchAdapterActive = 0;
					SetRunStatusRdyErr(m_SearchAdapterMask);*/
				}
			}
		}

		LED_3 = 0;
/*
------------------------------------------------------
nicht vergessen: alles wieder null setzen
erledigt m_async_ptr->AdjustActive = 0;
erledigt	unsigned short		AdjustActive;
	unsigned short		ShakeActive;
unsigned short m_AdjustMask = 0;
unsigned short m_ShakeMask = 0;
unsigned short m_MoveAsyncMask = 0;
unsigned short m_MoveSyncMask = 0;
------------------------------------------------------
*/
	}//end...while(1)
}
//###############################################################

unsigned short g_ASync_GetAllRuningFlags(unsigned short Mask, unsigned short *value)
{
	unsigned char error;
	unsigned short i;
	g_MsgASync.Mask = Mask;
	g_MsgASync.Error = errNone;
	g_MsgASync.eCommand = eASyncGetStatus;
	error = K_Mesg_Send_Wait(MBX_ASYNCCTRL, 0, &g_MsgASync);
	if(error != K_OK)
	{
		return ERR_SYSTEM;
	}
	if(g_MsgASync.Error != errNone)
	{
		return g_MsgASync.Error;
	}
	if(g_MsgASync.EStop)
	{
		*value = STATE_ESTOP;
		//return (unsigned short)ERR_SAFETY_APPLIANCE;24.10.2019
		return errNone;
	}
	*value = STATE_READY;
	for(i=0; i < MAX_AXES; i++)
	{
		if(GetAxisIDFromMask(Mask, g_FelixAxes[i].AxisBit) != 0)
		{
			if(g_MsgASync.MoveData[i].Running == STATE_BUSY)//NEWERR
			{
				*value = STATE_BUSY;
				break;
			}
		}
	}
	//return Msg.Error;
	return errNone;
}


unsigned short g_ASync_GetAllErrorFlags(unsigned short Mask, unsigned short *value)
{
	unsigned char error;
	unsigned short i;
	g_MsgASync.Mask = Mask;
	g_MsgASync.Error = errNone;
	g_MsgASync.eCommand = eASyncGetError;
	error = K_Mesg_Send_Wait(MBX_ASYNCCTRL, 0, &g_MsgASync);
	if(error != K_OK)
	{
		return ERR_SYSTEM;
	}
	if(g_MsgASync.Error != errNone){return g_MsgASync.Error;}

	*value = errNone;
	for(i=0; i < MAX_AXES; i++)
	{
		if(GetAxisIDFromMask(Mask, g_FelixAxes[i].AxisBit) != 0)
		{
			/*2016.03.01 if(g_MsgASync.MoveData[i].ErrorFlag)
			{

				*value = g_MsgASync.MoveData[i].Error;
				break;
			}*/
			if(g_GetAxes_ErrorFlag(i))
			{
				*value = g_GetAxes_ErrorAxisCyBio(i);
				break;
			}
		}
	}
	//return Msg.Error;
	return errNone;
}
unsigned short g_ASync_StartMultipleMoveAbsolute(unsigned short Mask, unsigned short Mode1536)
{
	unsigned char error;

	g_MsgASync.Error = errNone;
	g_MsgASync.eCommand = eASyncMv;
	g_MsgASync.Mask = Mask;
	g_MsgASync.Argument.usShort = Mode1536;

	error = K_Mesg_Send_Wait(MBX_ASYNCCTRL, 0, &g_MsgASync);
	if(error != K_OK)
	{
		return ERR_SYSTEM;
	}
	return g_MsgASync.Error;
}

unsigned short g_ASync_StartMultipleMoveTeach(unsigned short Mask)
{
	unsigned char error;
	//unsigned short ID;
	//short idx;
	//unsigned short i;
	//long value;

	g_MsgASync.Error = errNone;
	g_MsgASync.eCommand = eASyncTeach;
	g_MsgASync.Mask = Mask;
	error = K_Mesg_Send_Wait(MBX_ASYNCCTRL, 0, &g_MsgASync);
	if(error != K_OK)
	{
		return ERR_SYSTEM;
	}
	return g_MsgASync.Error;
}

unsigned short g_ASync_StartShake(unsigned short Mask)
{
	unsigned char error;

	g_MsgASync.Error = errNone;
	g_MsgASync.eCommand = eASyncShake;
	g_MsgASync.Mask = Mask;
	error = K_Mesg_Send_Wait(MBX_ASYNCCTRL, 0, &g_MsgASync);
	if(error != K_OK)
	{
		return ERR_SYSTEM;
	}
	return g_MsgASync.Error;
}

unsigned short g_ASync_StartSearchAdapter(unsigned short Mask)
{
	unsigned char error;

	g_MsgASync.Error = errNone;
	g_MsgASync.eCommand = eSearchAdapter;
	g_MsgASync.Mask = Mask;
	error = K_Mesg_Send_Wait(MBX_ASYNCCTRL, 0, &g_MsgASync);
	if(error != K_OK)
	{
		return ERR_SYSTEM;
	}
	return g_MsgASync.Error;
}

unsigned short g_ASync_ResetError(unsigned short AbfrageFehler, unsigned short Mask)
{
	unsigned char error;
	unsigned short i;
	unsigned short macheReset = 1;
//	if(AbfrageFehler == 0){return errNone;} 
	if(AbfrageFehler)
	{
		macheReset = 0;
		for(i=0; i < MAX_AXES; i++)
		{
			if(GetAxisIDFromMask(Mask, g_FelixAxes[i].AxisBit) != 0)
			{
				if(g_GetAxes_ErrorFlag(i) || (g_GetAxes_Running(i) == STATE_ERROR))
				{
					macheReset = 1;
				}
			}
		}
	}
	if(macheReset)
	{
		g_MsgASync.Mask = Mask;
		g_MsgASync.Error = errNone;
		g_MsgASync.eCommand = eASyncClearError;
		error = K_Mesg_Send_Wait(MBX_ASYNCCTRL, 0, &g_MsgASync);
		if(error != K_OK)
		{
			return ERR_SYSTEM;
		}
		return g_MsgASync.Error;
	}
	return errNone;
}

unsigned short g_ASync_BarcodeJustage(unsigned short onoff, unsigned short output)
{
	unsigned char error;

	g_MsgASync.Error = errNone;
	g_MsgASync.eCommand = ASyncBarcodeJustage;
	g_MsgASync.Argument.usShort = onoff;
	g_MsgASync.Mask = output;

	error = K_Mesg_Send_Wait(MBX_ASYNCCTRL, 0, &g_MsgASync);
	if(error != K_OK)
	{
		return ERR_SYSTEM;
	}
	return g_MsgASync.Error;
}
unsigned short g_ASync_ResetEstopMove(void)
{
	unsigned char error;

	g_MsgASync.Error = errNone;
	g_MsgASync.eCommand = eASyncResetEStopMov;
//	g_MsgASync.Mask = Mask;

	error = K_Mesg_Send_Wait(MBX_ASYNCCTRL, 0, &g_MsgASync);
	if(error != K_OK)
	{
		return ERR_SYSTEM;
	}
	return g_MsgASync.Error;
}
									   									   
unsigned short g_ASync_EStopMoveContinue(char *pszHippieAnswer)
{
	unsigned char errorCMX;
	unsigned short Error = errNone;
	unsigned short ErrorHippie;
//	char szHippieAnswer[64];

	g_MsgASync.Error = errNone;
	g_MsgASync.eCommand = eASyncEStopHippieMoveContinue;
	errorCMX = K_Mesg_Send_Wait(MBX_ASYNCCTRL, 0, &g_MsgASync);
	if(errorCMX != K_OK)
	{
		Error = ERR_SYSTEM;
	}
	else
	{
		ErrorHippie = g_MsgASync.Error;
		//if((g_MsgASync.Error == errNone) || (g_MsgASync.Error == errInfo))
		if((ErrorHippie == errNone) || (ErrorHippie == errInfo))
		{
			//ErrorHippie = g_MsgASync.Error;
			//strcpy(&szHippieAnswer[0], (char *)g_MsgASync.Argument.cBuffer);
			strcpy(pszHippieAnswer, (char *)g_MsgASync.Argument.cBuffer);
	//!!HIER

			g_MsgASync.Error = errNone;
			g_MsgASync.eCommand = eASyncEStopMoveContinue;

			errorCMX = K_Mesg_Send_Wait(MBX_ASYNCCTRL, 0, &g_MsgASync);
			if(errorCMX != K_OK)
			{
				Error = ERR_SYSTEM;
			}
			else
			{
				Error = g_MsgASync.Error;//Error vom Felix
			}
		}
		//!!Hier Auswahl Hippie oder Felix Error
		if(Error == errNone)
		{
			// Felix keon Error
			if(isEStopAnswerFromLPK())
			{
				Error = ErrorHippie;
			}
		}
	}
	return Error;
	//return g_MsgASync.Error;
}
unsigned short g_ASync_EStopMoveSavetyPos(void)
{
	unsigned char error;

	g_MsgASync.Error = errNone;
	g_MsgASync.eCommand = eASyncEStopMoveSavetyPos;

	error = K_Mesg_Send_Wait(MBX_ASYNCCTRL, 0, &g_MsgASync);
	if(error != K_OK)
	{
		return ERR_SYSTEM;
	}
	return g_MsgASync.Error;
}
unsigned short g_ASync_EStopMoveRestorePos(void)
{
	unsigned char error;

	g_MsgASync.Error = errNone;
	g_MsgASync.eCommand = eASyncEStopMoveRestorePos;

	error = K_Mesg_Send_Wait(MBX_ASYNCCTRL, 0, &g_MsgASync);
	if(error != K_OK)
	{
		return ERR_SYSTEM;
	}
	return g_MsgASync.Error;
}


unsigned short g_ASync_EStopRestoreCurrentAndSetPos(void)
{
	unsigned char error;

	g_MsgASync.Error = errNone;
	g_MsgASync.eCommand = eASyncEStopRestoreCurrentAndSetPos;

	error = K_Mesg_Send_Wait(MBX_ASYNCCTRL, 0, &g_MsgASync);
	if(error != K_OK)
	{
		return ERR_SYSTEM;
	}
	return g_MsgASync.Error;
}
