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
#include <math.h>

#include "DevError.h"
#include "30-5015_Felix/XE167F/main.h"
#include "DeviceConfig.h"
#include "Cxconfig.h"
#include "Felix_Hardware.h"
#include "Drv_config.h"
#include "MDrive_CmdCtrl.h"
#include "MDriveCmd.h"
#include "Felix_Misc.h"
#include "Felix_FRAM.h"
#include "Felix_HardwareTask.h"
#include "Felix_MotorTask.h"
#include "MDrive_BusCtrl.h"
//****************************************************************************
// @Macros
//****************************************************************************

//****************************************************************************
// local definitions
//****************************************************************************
#define CF_Z_STALL_REPEAT 1 
#define Z_BREAKE_TIME 1200	//20= 10sec, 1200= 5min;

#define MOTOR_TASK_EVENT_WAIT 5

#define Z_BREAKE_TIME 1200	//20= 10sec, 1200= 5min;
#define SPD_UNKNOWN 0
#define SPD_NORMAL 2
#define SPD_HOMING 3
#define SPD_1536 4
#define SPD_SLEW 5

#define HOMING_START	0x0001
#define HOMING_ACTIVE	0x0002
#define MOVING_START	0x0004
#define MOVING_ACTIVE	0x0008

#define NORMALMODE 0
#define ESTOP_STOPWAIT 1	//EStop aufgetreten, warten bis antriebe stehen
#define USERBREAKMODE 2
#define ERRORMODE 3

//****************************************************************************
// default settings
//****************************************************************************

//****************************************************************************
// external functions
//****************************************************************************

//****************************************************************************
// external data
//****************************************************************************
extern unsigned char UserMainSlot;
extern unsigned char MotorSlot;

//****************************************************************************
// internal functions
//****************************************************************************

//****************************************************************************
// public data
//****************************************************************************

//****************************************************************************
// private data
//****************************************************************************
IMS_CommandBlock * m_ptr;
udtIMSAxesPrepare m_AxesMovData[MAX_AXES];

#ifdef _CF_FELIX
long m_F_oldPosStep =0;
long m_R_oldPosStep =0;
#endif

//unsigned short m_WhatIsToDo  = 0x0000;
unsigned short m_maskStartHomingAxes = 0x0000;
//unsigned short m_maskRunHomingAxes = 0x0000;
unsigned short m_maskSetMode1536 = 0x0000;
unsigned short m_maskInversHomingMode  = 0x0000;

unsigned short m_maskStartMoveAxes = 0x0000;
unsigned short m_maskRunAxes = 0x0000;
unsigned short m_maskErrorAxes = 0x0000;

unsigned short m_maskHomingRunAxes = 0x0000;


unsigned short PCCmd_StartFlag = 0;
unsigned short PCCmd_DelayTime = 0;
unsigned short PCCmd_Adr = 2;
char szLpkcmd[64];

//	unsigned short m_ContinueEStop = 0;
	unsigned short m_EnableSavePosition = 0;
	short m_Mode1536Count = 0;
//	unsigned short m_LastMode1536 = 0;
//	unsigned short m_ChangeMode1536 = 0;
//	unsigned short m_EnableEStopContinueDrive = 0;
//	unsigned short m_SpeedState = SPD_UNKNOWN;
	unsigned short m_Enable_Z_Stromlos = 1;
	unsigned short m_Z_StromlosFlag = 1;
	unsigned short m_StromlosActive = 0;
	unsigned short m_isSaveEstopPosition = 0;

unsigned short m_EstopFlag = 0;

unsigned short CntStallError[MAX_AXES];
long StallLastPosition[MAX_AXES];
unsigned short g_FlagStallError = 0;
//****************************************************************************
// private functions
//****************************************************************************
unsigned short m_IsZ_Ex_Axis(unsigned short AxisID);

unsigned short m_ErrorHandling(unsigned short ex_i, unsigned short ex_error);
unsigned short m_ErrorHandlingCore(unsigned short ex_i, unsigned short ex_error);
unsigned short m_StateMachine(void);
unsigned short m_HomingStarten(void);
unsigned short m_HomingSchleife(short i);
unsigned short m_AntriebeStarten(unsigned short StartMode);
unsigned short m_WartenAufBewegungsEnde(unsigned short Stopmode);
unsigned short m_SaveEstopPosition(void);
void m_MT_UserBreak(void);

unsigned short StromlosAll(void);
unsigned short StromlosAxisWithBrakeAll(void);
unsigned short SetHoldCurrentAll(void);
void varSetNormalHoldCurrentIx(unsigned short i);
void varSet1536HoldCurrentIx(unsigned short i);
unsigned short SetHoldCurrentIx(unsigned short i);
unsigned short SetActHoldCurrentIx(unsigned short i, unsigned short HoldCurrent);
unsigned short Set1536RunCurrentIx(unsigned short i);
unsigned short SetRunCurrentAll(void);
unsigned short SetRunCurrentIx(unsigned short i);
unsigned short SetNormalSpeedAll(void);
unsigned short SetNormalSpeedIx(unsigned short i);
//unsigned short SetHomingSpeedAll(void);
unsigned short SetHomingSpeedIx(unsigned short i);
unsigned short Set1536SpeedIx(unsigned short i);
unsigned short SetNormalDeadBandIx(unsigned short i);
unsigned short Set1536DeadBandIx(unsigned short i);
unsigned short SetDeadBand(const unsigned short i, double value);
unsigned short SetNormalMotorWobbleWaitTimeIx(unsigned short i);
unsigned short Set1536MotorWobbleWaitTimeIx(unsigned short i);
unsigned short SetMode1536Ix(unsigned short i);
unsigned short SetModeNormalIx(unsigned short i);
unsigned short SetModeHomingIx(unsigned short i);
unsigned short SetModeSlewIx(unsigned short i);
unsigned short SetModeMask(unsigned short Mask);
unsigned short SetAktuellePositionAll(void);
unsigned short SetAktuellePositionID(unsigned short ID);
unsigned short SlewStartAll(unsigned short Mask);
unsigned short SlewStartID(unsigned short ID, unsigned short i);
unsigned short SlewEndAll(unsigned short Mask);
unsigned short SlewEndID(unsigned short ID, unsigned short i);
unsigned short Slew_ErrorStopAll(unsigned short Mask);
unsigned short Slew_ErrorStopID(unsigned short ID);
unsigned short SetSlewSpeedIx(unsigned short i);
unsigned short Slew_ErrorAll(unsigned short Mask);
unsigned short Slew_Error(unsigned short i);
//unsigned short SetSlewSpeedAll(void);
	void ResetVarEstopMov(void);

void ErrorLED(void);

unsigned short m_EStopRestorCurrentAndPos(unsigned short *Vergleich);
unsigned short m_SetHoldCurrentAndPos(void);
unsigned short m_PositionsVergleichSaveVsAktuell(unsigned short *result);

//****************************************************************************
// public functions
//****************************************************************************
/*void MotorTask_vInit(void)
{
	int * ptr;
	unsigned char status;

	ptr = calloc(US_MotorSlot, sizeof(int));
	ptr += US_MotorSlot;
	status = K_Task_Create_Stack(PRI_MOTOR, &MotorSlot, MotorTask, (void*)ptr);	// create task
	status = K_Task_Start(MotorSlot);		  							// trigger task
} //  End of function HardwareTask_vInit
*/



void MotorTask(void)
{
	unsigned short error = errNone;
	unsigned short event;
	unsigned short usValue;
	long lValue, lValueB;
	unsigned long ulValueC;
	float fValue, fValueB, fValueC;
	unsigned short i, n;
	unsigned short UserBreak = 0;
	i= K_Mbox_Event_Set(MBX_MOTOR, MotorSlot, EVENT_MOTOR_MBX);
	i= K_Mbox_Event_Set(MBX_ASYNCMOTOR, MotorSlot, EVENT_ASYNC_MOTOR_MBX);
#ifdef _CF_FELIX
//	m_AxesMovData[0].ID = FixXDrive.ID;
	//m_AxesMovData[0].BusAdr = FixXDrive.BusAdr;
//	m_AxesMovData[1].ID = FixFrontCarriage.ID;
	//m_AxesMovData[1].BusAdr = FixFrontCarriage.BusAdr;
//	m_AxesMovData[2].ID = FixRearCarriage.ID;
	//m_AxesMovData[2].BusAdr = FixRearCarriage.BusAdr;								   
//	m_AxesMovData[3].ID = FixBottomCarriage.ID;
	//m_AxesMovData[3].BusAdr = FixBottomCarriage.BusAdr;
//	m_AxesMovData[4].ID = FixZDrive.ID;
	//m_AxesMovData[4].BusAdr = FixZDrive.BusAdr;
#endif
#ifdef _CF_PENTA_DISK
//	m_AxesMovData[PARA_IDX_DISK_DRIVE].ID = FixDiskDrive.ID;
	//m_AxesMovData[0].BusAdr = FixDiskDrive.BusAdr;
//	m_AxesMovData[PARA_IDX_Z_E1_DRIVE].ID = FixZ_E1_Drive.ID;
	//m_AxesMovData[1].BusAdr = FixZ_E1_Drive.BusAdr;
//	m_AxesMovData[PARA_IDX_Z_E2_DRIVE].ID = FixZ_E2_Drive.ID;
	//m_AxesMovData[2].BusAdr = FixZ_E2_Drive.BusAdr;								   
//	m_AxesMovData[PARA_IDX_Z_E3_DRIVE].ID = FixZ_E3_Drive.ID;
	//m_AxesMovData[3].BusAdr = FixZ_E3_Drive.BusAdr;
//	m_AxesMovData[PARA_IDX_Z_E4_DRIVE].ID = FixZ_E4_Drive.ID;
	//m_AxesMovData[4].BusAdr = FixZ_E4_Drive.BusAdr;
//	m_AxesMovData[PARA_IDX_Z_E5_DRIVE].ID = FixZ_E5_Drive.ID;
	//m_AxesMovData[4].BusAdr = FixZ_E5_Drive.BusAdr;
#endif
	for(i = 0; i < MAX_AXES;i++)
	{
//		m_AxesMovData[i].StartEnable = 0;
//		m_AxesMovData[i].StartHomingEnable = 0;
//		m_AxesMovData[i].EStopStartHomingEnable = 0;
//		m_AxesMovData[i].globalMoveRuningState = STATE_READY;
//		m_AxesMovData[i].internMoveRuningState = STATE_READY;
//		m_AxesMovData[i].isHomingRuningActive = 0;
//		m_EStopRestoreStatusHelperForHost[i] = 1;
		m_AxesMovData[i].getSpeedMode = SPD_UNKNOWN;
		CntStallError[i] = 0;
//		m_AxesMovData[i].setMode1536= 0;

	}
	//m_ContinueEStop = 0;

	while(1)
	{

	event = K_Event_Wait(EVENT_MOTOR_MBX | EVENT_ASYNC_MOTOR_MBX | EVENT_USER_BREAK, 5, 0);
	if(event & EVENT_USER_BREAK)
	{
		K_Event_Reset(MotorSlot, EVENT_USER_BREAK);
		PCCmd_StartFlag = 0;
		UserBreak = 1;
		m_MT_UserBreak();
	}

	if((event & EVENT_MOTOR_MBX) || (event & EVENT_ASYNC_MOTOR_MBX))
	{
		LED_2 = 1;
		if(event & EVENT_MOTOR_MBX)
		{
			m_ptr = K_Mesg_Get(MBX_MOTOR);	// auf Message warten
			K_Event_Reset(MotorSlot, EVENT_MOTOR_MBX);

		}
		else
		{
			m_ptr = K_Mesg_Get(MBX_ASYNCMOTOR);	// auf Message warten
			K_Event_Reset(MotorSlot, EVENT_ASYNC_MOTOR_MBX);
		}
		WDO();
		switch(m_ptr->eCommand)
		{
			case eZBreakeFixed		:	Z_Breake_Fixed();
										m_ptr->Error = errNone;
										break;
			case eZBreakeFree		:	Z_Breake_Free();
										m_ptr->Error = errNone;
										break;
			case eImsGetOEMError	:	m_ptr->Error = IMS_GetError(m_ptr->AxisID, &usValue);
										m_ptr->Argument.usShort = usValue;
										break; 
			case eImsGetHWError		:	m_ptr->Error = ERR_UNKNOWN_DRV;
										for(i = 0; i < MAX_AXES;i++)
										{
											if(m_ptr->AxisID == g_FelixAxes[i].AxisID)
											{
												//m_ptr->Argument.usShort = g_GetAxes_ErrorAxisHW(i);
												m_ptr->Argument.usShort = g_GetAxes_LastErrorAxisHWForEver(i);
												m_ptr->Error = errNone;
											}
										}
										break; 
			case eImsMotorResetError:	m_ptr->Error = IMS_ResetError(m_ptr->AxisID);
										break;
			case eImsResetError		:	error = IMS_ResetError(m_ptr->AxisID);
										if(error != errNone)
										{
											m_ptr->Error  = error;
											break;
										}
										//!! unbeding das break weglassen
			//case eImsResetStallFlag	:	
										error = IMS_ResetStallFlag(m_ptr->AxisID);
										m_ptr->Error  = error;
										if(error == errNone)
										{
											for(i = 0; i < MAX_AXES;i++)
											{
												if(m_ptr->AxisID == g_FelixAxes[i].AxisID)
												{
													g_SetAxes_ErrorAxisHW(i, 0);
													g_SetAxes_ErrorAxisCyBio(i, errNone);
													g_SetAxes_ErrorFlag(i, 0);
													m_maskErrorAxes &= ~g_FelixAxes[i].AxisBit;
													if(g_GetAxes_Running(i) == STATE_ERROR){g_SetAxes_Running(i, STATE_READY);}
													//m_AxesMovData[i].globalMoveRuningState = STATE_READY;
													//m_AxesMovData[i].internMoveRuningState = STATE_READY;//NEU
												}
											}
										}
										break; 
			case eImsResetMotorStallFlag: error = IMS_ResetStallFlag(m_ptr->AxisID);
										m_ptr->Error  = error;
										break;
			case eImsOEM :				m_ptr->Error = IMS_OEM((char *)m_ptr->Argument.cBuffer);
										break; 
			case eImsDoorLifterOEM :	m_ptr->Error = IMS_DoorLifterOEM((char *)m_ptr->Argument.cBuffer);
										break; 
			/*case eImsXX		:			m_ptr->Error = IMS_TestCmd(m_ptr->AxisID);
										break; */
			case eImsSetEncoderMode	:	m_ptr->Error = IMS_SetEncoderMode(m_ptr->AxisID);
										break; 
			case eImsSetS1			:	m_ptr->Error = IMS_SetS1(m_ptr->AxisID);
										break;
			case eImsSetS1Invers	:	m_ptr->Error = IMS_SetS1Invers(m_ptr->AxisID);
										break;

			case eImsSetS2_EStopOn	:	m_ptr->Error = IMS_SetS2_EStopOn(m_ptr->AxisID);
										break; 
			/*case eImsSetS2_EStopOff	:	m_ptr->Error = IMS_SetS2_EStopOff(m_ptr->AxisID);
										break;*/
			case eImsSetESC			:	m_ptr->Error = IMS_SetESC(m_ptr->AxisID);
										break; 
			case eImsStallDetectON	:	m_ptr->Error = IMS_StallDetectON(m_ptr->AxisID);
										break; 
			case eImsStallDetectOFF	:	m_ptr->Error = IMS_StallDetectOFF(m_ptr->AxisID);
										break; 
			case eImsGetStallFlag	:	m_ptr->Error = IMS_GetStallFlag(m_ptr->AxisID, &usValue);
										m_ptr->Argument.usShort = usValue;
										break; 
			case eImsGetStallFactor		:	m_ptr->Error = IMS_GetStallFactor(m_ptr->AxisID, &usValue);
										m_ptr->Argument.usShort = usValue;
										break; 
			case eImsSetStallFactor		:	m_ptr->Error = IMS_SetStallFactor(m_ptr->AxisID, m_ptr->Argument.usShort);
										break; 
			case eImsGetMotorWobbleWaitTime:m_ptr->Error = IMS_GetMotorWobbleWaitTime(m_ptr->AxisID, &usValue);
											m_ptr->Argument.usShort = usValue;
											break; 
			case eImsSetMotorWobbleWaitTime:m_ptr->Error = IMS_SetMotorWobbleWaitTime(m_ptr->AxisID, m_ptr->Argument.usShort);
											break; 
			case eImsGetHoldCurrentWaitTime:m_ptr->Error = IMS_GetHoldCurrentWaitTime(m_ptr->AxisID, &usValue);
											m_ptr->Argument.usShort = usValue;
											break; 
			case eImsSetHoldCurrentWaitTime:m_ptr->Error = IMS_SetHoldCurrentWaitTime(m_ptr->AxisID, m_ptr->Argument.usShort);
											break; 
			case eImsGetEncoderDeadBand	:	m_ptr->Error = IMS_GetEncoderDeadBand(m_ptr->AxisID, &usValue);
											m_ptr->Argument.usShort = usValue;
											break; 
			case eImsSetEncoderDeadBand	:	m_ptr->Error = IMS_SetEncoderDeadBand(m_ptr->AxisID, m_ptr->Argument.usShort);
											break; 
			case eImsGetHoldCurrent	:	m_ptr->Error = IMS_GetHoldCurrent(m_ptr->AxisID, &usValue);
										m_ptr->Argument.usShort = usValue;
										break; 
			case eImsSetHoldCurrent	:	m_ptr->Error = IMS_SetHoldCurrent(m_ptr->AxisID, m_ptr->Argument.usShort);
										if(error == errNone)
										{
											for(i = 0; i < MAX_AXES;i++)
											{
												if(m_ptr->AxisID == g_FelixAxes[i].AxisID)
												{
													varSetNormalHoldCurrentIx(i);
												}
											}
										}
										break; 
			case eImsSetMotorHoldCurrent	:	m_ptr->Error = IMS_SetHoldCurrent(m_ptr->AxisID, m_ptr->Argument.usShort);
										break; 
			case eImsInitMotorTaskVariable:	
										varSetNormalHoldCurrentIx(m_ptr->AxisID);
										m_ptr->Error = errNone;
										break;
			case eImsGetRunCurrent	:	m_ptr->Error = IMS_GetRunCurrent(m_ptr->AxisID, &usValue);
										m_ptr->Argument.usShort = usValue;
										break; 
			case eImsSetRunCurrent	:	m_ptr->Error = IMS_SetRunCurrent(m_ptr->AxisID, m_ptr->Argument.usShort);
										break;
			case eImsStromlosJQV	:	m_ptr->Error = IMS_SetHoldCurrent(m_ptr->AxisID, 0);
										if((m_ptr->Error == errNone) && m_IsZ_Ex_Axis(m_ptr->AxisID))
										{
											Z_Breake_Free();//anziehen der bremse über timer verhindern
											
										}
										break;
		
			case eImsGetCounterMotorSteps:	m_ptr->Error = IMS_GetCounterMotorSteps(m_ptr->AxisID, &lValue);
											m_ptr->Argument.lLong = lValue;
											break;
			case eImsSetCounterMotorSteps:	m_ptr->Error = IMS_SetCounterMotorSteps(m_ptr->AxisID, m_ptr->Argument.lLong);
											//K_Mesg_Ack_Sender();
											break; 
			case eImsGetCounterEncoder:	m_ptr->Error = IMS_GetCounterEncoder(m_ptr->AxisID, &lValue);
										m_ptr->Argument.lLong = lValue;
										break; 
			case eImsSetCounterEncoder:	m_ptr->Error = IMS_SetCounterEncoder(m_ptr->AxisID, m_ptr->Argument.lLong);
										K_Mesg_Ack_Sender();
										break;
			case eImsSetDecToAcc		:	m_ptr->Error = IMS_SetDecToAcc(m_ptr->AxisID);
										break;
			case eImsSetSpeed		:	m_ptr->Error = IMS_SetSpeed(m_ptr->AxisID, m_ptr->Argument.fFloat, m_ptr->ArgumentB.fFloatB, m_ptr->ArgumentC.fFloatC);
										break;
			case eImsGetSpeed		:	m_ptr->Error = IMS_GetSpeed(m_ptr->AxisID, &fValue, &fValueB, &fValueC);
										m_ptr->Argument.fFloat = fValue;
										m_ptr->ArgumentB.fFloatB = fValueB;
										m_ptr->ArgumentC.fFloatC = fValueC;
										break;
			case eImsSetMotorSpeed	:	m_ptr->Error = IMS_SetSpeed_NoCalc(m_ptr->AxisID, m_ptr->Argument.lLong, m_ptr->ArgumentB.lLongB, m_ptr->ArgumentC.ulLongC);
										break;
			case eImsGetMotorSpeed	:	m_ptr->Error = IMS_GetSpeed_NoCalc(m_ptr->AxisID, &lValue, &lValueB, &ulValueC);
										m_ptr->Argument.lLong = lValue;
										m_ptr->ArgumentB.lLongB = lValueB;
										m_ptr->ArgumentC.ulLongC = ulValueC;
										break;


			case eImsStopMotor		:	m_ptr->Error = IMS_StopMotor(FELIX_BASE_ID);
										break;
			case eImsGetMoveFlag	:	m_ptr->Error = IMS_GetMoveFlag(m_ptr->AxisID, &usValue);
										m_ptr->Argument.usShort = usValue;
										break; 
			case eImsGetHomeSwitch	:	
										m_ptr->Error = IMS_GetHomeSwitch(m_ptr->AxisID, &usValue);
										m_ptr->Argument.usShort = usValue;
										break;
			case eImsGetMotorHomeSwitch	:	
										m_ptr->Error = IMS_GetMotorHomeSwitch(m_ptr->AxisID, &usValue);
										m_ptr->Argument.usShort = usValue;
										break; 

			case eImsGetEStopSwitch	:	m_ptr->Error = IMS_GetEStopSwitch(m_ptr->AxisID, &usValue);
										m_ptr->Argument.usShort = usValue;
										break; 

			case eImsGetPosition  	:	m_ptr->Error = IMS_GetPosition(m_ptr->AxisID, &lValue);
										m_ptr->Argument.lLong = lValue;
										break; 
			case eImsMotorResetPosition	:	m_ptr->Argument.lLong = 0;
			case eImsMotorSetPosition	:	m_ptr->Error = IMS_SetPosition(m_ptr->AxisID, m_ptr->Argument.lLong);
										if(m_ptr->Error == errNone)
										{
											K_Task_Wait(10);
											m_ptr->Error = IMS_ResetError(m_ptr->AxisID);
											if(m_ptr->Error == errNone)
											{
												m_ptr->Error = IMS_ResetStallFlag(m_ptr->AxisID);
											}
										}
										break;
			case eImsResetPosition	:	m_ptr->Argument.lLong = 0;
			case eImsSetPosition	:	m_ptr->Error = IMS_SetPosition(m_ptr->AxisID, m_ptr->Argument.lLong);
										if(m_ptr->Error == errNone)
										{
											K_Task_Wait(10);
											m_ptr->Error = IMS_ResetError(m_ptr->AxisID);
											if(m_ptr->Error == errNone)
											{
												m_ptr->Error = IMS_ResetStallFlag(m_ptr->AxisID);
											}
										}
										for(i = 0; i < MAX_AXES;i++)
										{
											if(g_FelixAxes[i].AxisID == m_ptr->AxisID)
											{
												m_AxesMovData[i].Way = m_ptr->Argument.lLong;
												m_AxesMovData[i].SaveStopPos = m_ptr->Argument.lLong;
												break;
											}
										}
										break; 
			case eImsHomingSequence:	m_ptr->Error = IMS_HomingSequence(m_ptr->AxisID, m_ptr->Argument.usShort);
										break;
			case eImsHomingMultiSequence:
										m_maskStartHomingAxes = 0;
										m_maskInversHomingMode = 0;
										for(i = 0; i < MAX_AXES;i++)
										{
											for(n = 0; n < MAX_AXES;n++)
											{
												if(g_FelixAxes[i].AxisID == m_ptr->MoveData[n].ID)
												{
													if(m_ptr->MoveData[n].StartEnable)//entfernen nur mit id oder maske
													{
														m_maskStartHomingAxes |= g_FelixAxes[n].AxisBit;
														if(m_ptr->MoveData[n].InversHomingMode)
														{
															m_maskInversHomingMode |= g_FelixAxes[n].AxisBit;
														}
														m_AxesMovData[i].HomeToSwitchMode = m_ptr->MoveData[n].HomeToSwitchMode;
													}
													break;
												}
											}
										}
										if(!m_maskStartHomingAxes)
										{
											m_ptr->Error = ERR_UNKNOWN_DRV;
										}
										else
										{
											m_EnableSavePosition = 0;
											m_ptr->Error = errNone;
										}
										break;
			case eImsMoveAbsolutePos:	m_ptr->Error = IMS_MoveAbsolutePos(m_ptr->AxisID, m_ptr->Argument.lLong);
										break;
										
			case eImsStartMultipleMoveAbsolute:
										#ifdef _CF_DEBUG_PRINT
										if(g_TestFlag & TF_DBG_EVENT){dbPrint("\r ################# New Move Command ############# \r");}
										#endif
										/*if(m_ptr->Argument.usShort != m_LastMode1536)
										{
											m_LastMode1536 = m_ptr->Argument.usShort;
											//m_ChangeMode1536 = 1;
										}*/
			case eImsAdjustStartMove:
										m_maskStartMoveAxes = 0;
										for(i = 0; i < MAX_AXES;i++)
										{
											for(n = 0; n < MAX_AXES;n++)
											{
												if(g_FelixAxes[i].AxisID == m_ptr->MoveData[n].ID)
												{
													if(m_ptr->MoveData[n].StartEnable)
													{
														m_maskStartMoveAxes |= g_FelixAxes[n].AxisBit;
														m_AxesMovData[i].Way = m_ptr->MoveData[n].StepWay;
														//m_AxesMovData[i].setMode1536 = m_ptr->Argument.usShort;
														if(m_ptr->Argument.usShort)
														{
															m_maskSetMode1536 |= g_FelixAxes[n].AxisBit;
														}
														else
														{
															m_maskSetMode1536 &= ~g_FelixAxes[n].AxisBit;
														}
													}
													break;
												}
											}
										}
										if(!m_maskStartMoveAxes)
										{
											m_ptr->Error = ERR_UNKNOWN_DRV;
										}
										else
										{
											m_EnableSavePosition = 1;
											m_ptr->Error = errNone;
										}
										break;
			case eLPK				:	m_ptr->Error = IMS_LPK(m_ptr->AdresseCyBiCtrl, (char *)m_ptr->Argument.cBuffer);
										break;
			case eLPKPCCmdSend		:	
										UserBreak = 0;
										PCCmd_DelayTime = m_ptr->Mask;
										strcpy(szLpkcmd, (char *)m_ptr->Argument.cBuffer);
										PCCmd_Adr = m_ptr->AdresseCyBiCtrl;//PCCmd_Adr = m_ptr->AxisID;
										if(PCCmd_DelayTime == 0)
										{
											//m_ptr->Error = IMS_LPKSend((char *)m_ptr->Argument.cBuffer);
											m_ptr->Error = IMS_LPKSendParallelCmd(PCCmd_Adr, szLpkcmd);
										}
										else
										{
											PCCmd_DelayTime = PCCmd_DelayTime / 11;
											PCCmd_StartFlag = 1;
											m_ptr->Error = errNone;
										}
										
										break;
			case eLPKPCCmdReceive	:	
										if(UserBreak)
										{
											UserBreak = 0;
											error = LPK_USER_BREAK;
										}
										else
										{
											error = IMS_LPKReceiveParallelCmd((char *)m_ptr->Argument.cBuffer);
										}
										m_ptr->Error = error;
										break;
			case eLPKGetSensorSwitchMagzInsert	:	
										m_ptr->Error = IMS_cmdGetSensorMagazinInsert(m_ptr->LpkID, m_ptr->Argument.cChar, m_ptr->AdresseCyBiCtrl, &usValue);
										m_ptr->Argument.usShort = usValue;
										break;
			case eSync				:	m_ptr->Error = IMS_Sync();
										break;
			//case eImsEnableEStopRestorePosition: 
										//m_EnableEStopContinueDrive = m_ptr->Argument.usShort;
										//break;
			case eImsGetStatus:			m_ptr->Error = errNone;
										m_ptr->MaskRunAxes = m_maskRunAxes;
										m_ptr->MaskErrorAxes = m_maskErrorAxes;
										//m_ptr->Argument.usShort = m_EstopFlag;
										m_EstopFlag = 0;
										//m_ptr->EStop = g_isEstopFlag();
										m_ptr->EStop = g_isGenerateEStopError();
										break;
			case eResetEStopMov		:	
										ResetVarEstopMov();
										m_ptr->Error = errNone;
										break;
			case eImsSetMode_Mask	:	m_ptr->Error = SetModeMask(m_ptr->Mask);
										break;
			case eImsSlewMoveID		:	
										m_ptr->Error = IMS_SlewMove(m_ptr->AxisID, m_ptr->Argument.lLong);
										break;
			case eImsSlewStartAll		:	m_ptr->Error = SlewStartAll(m_ptr->AxisID);
										break;
			case eImsSlewEndAll		:	m_ptr->Error = SlewEndAll(m_ptr->AxisID);
										break;
			case eImsEnablePositionMaintenance	:	m_ptr->Error = IMS_EnablePositionMaintenance(m_ptr->AxisID);
										break; 
			case eImsDisablePositionMaintenance	:	m_ptr->Error = IMS_DisablePositionMaintenance(m_ptr->AxisID);
										break; 
			case eImsGetGetPositionMaintenance	:	m_ptr->Error = IMS_GetPositionMaintenance(m_ptr->AxisID, &usValue);
										m_ptr->Argument.usShort = usValue;
										break; 
			case eImsEStopRestoreCurrentAndGetPos:
										m_ptr->Error = m_EStopRestorCurrentAndPos(&usValue);
										m_ptr->Argument.usShort = usValue;
										break;
			case eImsEStopRestoreCurrentAndSetPos:
										m_ptr->Error = m_SetHoldCurrentAndPos();
										break;
			case eImsEStopSafetyPos:
										m_maskStartMoveAxes = 0;
										for(i = 0; i < MAX_AXES;i++)
										{
											// ACHTUNG Nur für Z Achsen
											if((m_ptr->Mask & g_FelixAxes[i].AxisBit) && g_FelixAxes[i].Enable)
											{
												m_AxesMovData[i].Way = m_ptr->MoveData[i].StepWay;
												m_maskStartMoveAxes |= g_FelixAxes[i].AxisBit;
												/*if(m_ptr->Argument.usShort)
												{
													m_maskSetMode1536 |= g_FelixAxes[i].AxisBit;
												}
												else
												{
													m_maskSetMode1536 &= ~g_FelixAxes[i].AxisBit;
												}*/
											}
										}
										if(!m_maskStartMoveAxes)
										{
											m_ptr->Error = ERR_UNKNOWN_DRV;
										}
										else
										{
											m_EnableSavePosition = 0;
											m_ptr->Error = errNone;
										}
										break;
			case eImsEStopRestorePos:
										m_maskStartMoveAxes = 0;
										m_ptr->Error = errNone;
										if(m_isSaveEstopPosition)
										{
											for(i = 0; i < MAX_AXES;i++)
											{
												// ACHTUNG Nur für Z Achsen
												if((m_ptr->Mask & g_FelixAxes[i].AxisBit) && g_FelixAxes[i].Enable)
												{
													m_AxesMovData[i].Way = m_AxesMovData[i].SaveStopPos;
													m_maskStartMoveAxes |= g_FelixAxes[i].AxisBit;
													/*if(m_ptr->Argument.usShort){m__maskSetMode1536 |= g_FelixAxes[i].AxisBit;}
													else{m_maskSetMode1536 &= ~g_FelixAxes[i].AxisBit;}*/
												}
											}
											if(!m_maskStartMoveAxes)
											{
												//m_ptr->Error = ERR_UNKNOWN_DRV;
												m_ptr->Error = errNone;
											}
											else
											{
												m_EnableSavePosition = 0;
												m_ptr->Error = errNone;
											}
										}
										break;
			case eImsHippieSafety:		m_ptr->Error = IMS_cmdLPKSafety(m_ptr->LpkID, m_ptr->Argument.cChar, m_ptr->AdresseCyBiCtrl);
										break;
			case eImsHippieRestore:		m_ptr->Error = IMS_cmdLPKRestore(m_ptr->LpkID, m_ptr->Argument.cChar, m_ptr->AdresseCyBiCtrl);
										break;
			case eImsHippieContinue:	//m_ptr->Error = IMS_cmdLPKContinue(m_ptr->LpkID, m_ptr->Argument.cChar, m_ptr->AdresseCyBiCtrl);
										m_ptr->Error = IMS_cmdLPKContinue(m_ptr->LpkID, m_ptr->Argument.cChar, m_ptr->AdresseCyBiCtrl, (char *)m_ptr->Argument.cBuffer);
										break;
			default					:	//m_ptr->eCommand = eImsNone;
										m_ptr->Error = ERR_CMD_NOT_IMPLEMENTED;
										break; 
		}//end...switch(m_ptr->eCommand)
		K_Mesg_Ack_Sender();
		LED_2 = 0;
	}//end...if(Event & EVENT_MOTOR_MBX)
		if(PCCmd_StartFlag)
		{
			WDO();
			if(PCCmd_DelayTime == 0)
			{
				PCCmd_StartFlag = 0;
				IMS_LPKSendParallelCmd(PCCmd_Adr, szLpkcmd);
			
			}
			else
			{
				--PCCmd_DelayTime;
			}
		}
		m_StateMachine();
	}//end...while(1)
}

//****************************************************************************
// private functions
//****************************************************************************

void ResetVarEstopMov(void)
{
	//m_WhatIsToDo  = 0x0000;
	m_maskStartHomingAxes = 0x0000;
//	m_maskRunHomingAxes = 0x0000;
	m_maskStartMoveAxes = 0x0000;
	m_maskRunAxes = 0x0000;

	//m_ContinueEStop = 0;
	m_EnableSavePosition = 0;
	//m_EnableEStopContinueDrive = 0;
	m_isSaveEstopPosition = 0;
}

unsigned short m_IsZ_Ex_Axis(unsigned short AxisID)
{
//#################### ändern ###############
	#ifdef _CF_FELIX
	if(AxisID == FixZDrive.ID)
	#endif
	#ifdef _CF_PENTA_DISK
	if((AxisID == FixZ_E1_Drive.ID)|(AxisID == FixZ_E2_Drive.ID)
		|(AxisID == FixZ_E3_Drive.ID)|(AxisID == FixZ_E4_Drive.ID)|(AxisID == FixZ_E5_Drive.ID))
	#endif
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

unsigned short m_ErrorHandling(unsigned short ex_i, unsigned short ex_error)
{
	unsigned short error = errNone;
	unsigned short i;

	IMS_StopMotor(FELIX_BASE_ID);
	i = 0;
	while(m_maskRunAxes)
	{
		K_Task_Wait(10);
		i++;
		error = m_WartenAufBewegungsEnde(ERRORMODE);
		if(error != errNone) break;
		if(i > 300)
		{
			error = ERR_SYSTEM;
			break;
		}
	}
	if(error != errNone) {K_Task_Wait(1000);}
	return m_ErrorHandlingCore( ex_i, ex_error);
}

unsigned short m_ErrorHandlingCore(unsigned short ex_i, unsigned short ex_error)
{
	m_maskStartHomingAxes = 0x0000;
	m_maskStartMoveAxes = 0x0000;
	m_maskRunAxes = 0x0000;
	m_EnableSavePosition = 0;
	m_isSaveEstopPosition = 0;

	g_SetAxes_ErrorFlag(ex_i, 1);
	m_maskErrorAxes |= g_FelixAxes[ex_i].AxisBit;
	if(g_GetAxes_ErrorAxisHWStall(ex_i))
	{
		//g_SetAxes_StallFlag(i, 1);
		g_SetAxes_ErrorAxisCyBio(ex_i, ERR_STALL);
	}
	else
	{
		g_SetAxes_ErrorAxisCyBio(ex_i, ex_error);
	}
	g_SetAxes_Running(ex_i, STATE_ERROR);
	return ex_error;
}
unsigned short m_StateMachine(void)
{
	unsigned short error = errNone;
	unsigned short errorsave = errNone;
	unsigned short event;
//	unsigned short help;
	unsigned short i;
//	unsigned short Vergleich;
//	#ifdef _CF_NRTL
  	unsigned short Count;
//	#endif
	unsigned short EstopInMotion;
	udtEstopCounter my_EstopCounter;

	if((GetTimeZDrive()==0) && (!m_Z_StromlosFlag) && m_Enable_Z_Stromlos)
	{
		#ifdef _CF_DEBUG_PRINT
		if(g_TestFlag & TF_DBG_BREAKE){dbPrint("Z GetTime 1\r");}
		#endif
		if(!m_maskRunAxes)
		{
			if((error = StromlosAxisWithBrakeAll()) != errNone){ErrorLED();return error;}
		}
	}
	if(m_maskStartHomingAxes)
	{
		#ifdef _CF_DEBUG_PRINT
		if(g_TestFlag & TF_DBG_EVENT){dbPrint("Homing Normal\r");}
		#endif
		//m_EnableEStopContinueDrive = 0;
		if((error = m_HomingStarten()) != errNone){ErrorLED();return error;}
	}
	if(m_maskStartMoveAxes)
	{
		#ifdef _CF_DEBUG_PRINT
		if(g_TestFlag & TF_DBG_EVENT){dbPrint("Move Normal\r");}
		#endif
		for(i = 0; i < MAX_AXES;i++)
		{
			if((m_maskStartMoveAxes & g_FelixAxes[i].AxisBit) && g_FelixAxes[i].Enable)
			{
				if((m_maskSetMode1536 & g_FelixAxes[i].AxisBit) && (m_AxesMovData[i].getSpeedMode != SPD_1536))
				{
					//1536er setzen
					//m_SpeedState = SPD_UNKNOWN;
					if((error = SetMode1536Ix(i)) != errNone){m_ErrorHandling(i, error);ErrorLED();return error;}
					//m_SpeedState = SPD_1536;
					m_Mode1536Count++;
				}
				if(!(m_maskSetMode1536 & g_FelixAxes[i].AxisBit) && (m_AxesMovData[i].getSpeedMode != SPD_NORMAL))
				{
					//Normal setzen
					//m_SpeedState = SPD_UNKNOWN;
					if((error = SetModeNormalIx(i)) != errNone){m_ErrorHandling(i, error);ErrorLED();return error;}
					//m_SpeedState = SPD_NORMAL;
					m_Mode1536Count--;
				}
				
			}//end...if((m_maskStartMoveAxes ...
		}//end...for(
		//in Antrieb starten verlegen
		if((error = m_SetHoldCurrentAndPos()) != errNone){ErrorLED();return error;}
		if((error = m_AntriebeStarten(NORMALMODE)) != errNone){ErrorLED();return error;}
	}
	if((error = m_WartenAufBewegungsEnde(NORMALMODE)) != errNone)
	{
		ErrorLED();return error;
	}


//	event = K_Event_Wait(ESTOP_EVENT | ESTOP_RESET_EVENT, 5, 2 );
	event = K_Event_Wait(ESTOP_EVENT, 5, 2 );
// A
	if((event & ESTOP_EVENT) && (!g_isEstopFlag()))
	{//ESTOP ausgelöst
//dbPrint("A ESTOP Event\r");
		#ifdef _CF_DEBUG_PRINT
		if(g_TestFlag & (TF_DBG_EVENT | TF_DBG_485)){dbPrint("A ESTOP Event\r");}
		#endif
		IMS_StopMotor(FELIX_BASE_ID);
		g_SetEstopFlag();
		g_SetGenerateEStopError();

		EstopInMotion = 0;
		errorsave = errNone;
		m_EstopFlag = 1;
		//#ifdef _CF_NRTL
		if(isNRTL_LP())
		{
			ESTOP_VARIABLEN_SETGET_ALLOC
			Count = g_EstopWait.EstopWait / 10;
			if(Count == 0){Count++;}
			ESTOP_VARIABLEN_SETGET_RELEASE
			for(i = 0; i < Count;i++)
			{
				K_Task_Wait(10);
			}
			//K_Task_Wait(1700);
			for(i = 0; i < MAX_AXES;i++)
			{
				if(g_FelixAxes[i].Enable)
				{
					IMS_ResetError(g_FelixAxes[i].AxisID);
					IMS_ResetStallFlag(g_FelixAxes[i].AxisID);
				}
			}
		}
		//#endif
		while(m_maskRunAxes)
		{
			K_Task_Wait(1);
			if((error = m_WartenAufBewegungsEnde(ESTOP_STOPWAIT)) != errNone)
			{
				ErrorLED();
				errorsave = error;
			}//return error;}
		}
		if(m_EnableSavePosition)
		{
			if((error = m_SaveEstopPosition()) != errNone){ErrorLED();errorsave = error;}//return error;}
			m_EnableSavePosition = 0;
			#ifdef _CF_DEBUG_PRINT
			if(g_TestFlag & TF_DBG_EVENT){dbPrint("A ESTOP SavePosition\r");}
			#endif
		}
		#ifdef _CF_DEBUG_PRINT
		if(g_TestFlag & TF_DBG_EVENT){dbPrint("A ESTOP Event STROMLOS\r");}
		#endif
		//m_EnableEStopContinueDrive = 0;//###neu
		m_StromlosActive = 1;
		if((error = StromlosAll()) != errNone){ErrorLED();errorsave = error;}//return error;}
		ESTOP_VARIABLEN_SETGET_ALLOC
		g_EstopCounter.EstopAll++;
		if(EstopInMotion){g_EstopCounter.EstopByMotion++;}
		my_EstopCounter.EstopAll = g_EstopCounter.EstopAll;
		my_EstopCounter.EstopByMotion = g_EstopCounter.EstopByMotion;
		ESTOP_VARIABLEN_SETGET_RELEASE
		FRAMSaveEstopCounter(&my_EstopCounter);
		if(errorsave != errNone){ErrorLED();return errorsave;}
	}
/*	if(event & ESTOP_RESET_EVENT)
	{
		g_ClearEstopFlag();
		if(!isHostMode())
		{
			g_ClearGenerateEStopError();
		}
	}*/
// C DRIVE kein ESTOP mehr
return error;
}




unsigned short m_HomingStarten(void)
{
	unsigned short error = errNone;
	short i;
	//m_WhatIsToDo &= ~HOMING_START;
	m_StromlosActive = 0;
	//m_SpeedState = SPD_UNKNOWN;

	//#ifdef _CF_NRTL
	if(isNRTL_LP())
	{
		if(isFlagFwEStopLock())
		{
			ResetFwEStopLock();
		}
	}
	//#endif
	for(i = 0; i < MAX_AXES;i++)
	{
		if((m_maskStartHomingAxes & g_FelixAxes[i].AxisBit) && g_FelixAxes[i].Enable)
		{
			if(m_maskSetMode1536 & g_FelixAxes[i].AxisBit)
			{
				m_maskSetMode1536 &= ~g_FelixAxes[i].AxisBit;
				m_Mode1536Count--;
			}
			if((error = SetModeHomingIx(i)) != errNone){return m_ErrorHandling(i, error);}
		}
	}
	//m_SpeedState = SPD_HOMING;
	#ifdef _CF_FELIX
	if((m_maskStartHomingAxes & eFrontCarriage_Axis)&&(m_maskStartHomingAxes & eRearCarriage_Axis)
		&& (m_maskInversHomingMode & eFrontCarriage_Axis) && (!(m_maskInversHomingMode & eRearCarriage_Axis)))
	{
		for(i = MAX_AXES-1; i >= 0; i--)
		{
			if((error = m_HomingSchleife(i)) != errNone){return m_ErrorHandling(i, error);}
		}
	}
	else
	{
		for(i = 0; i < MAX_AXES;i++)
		{
			if((error = m_HomingSchleife(i)) != errNone){return m_ErrorHandling(i, error);}
		}
	}
	#endif
	#ifdef _CF_PENTA_DISK
	for(i = 0; i < MAX_AXES;i++)
	{
		if((error = m_HomingSchleife(i)) != errNone){return m_ErrorHandling(i, error);}
	}
	#endif
	return error;
}

unsigned short m_HomingSchleife(short i)
{
	unsigned short error = errNone;

	if((m_maskStartHomingAxes & g_FelixAxes[i].AxisBit) && g_FelixAxes[i].Enable)
	{
		if((error = SetHoldCurrentIx(i)) != errNone){return error;}
		if((error = SetAktuellePositionID(g_FelixAxes[i].AxisID)) != errNone){return error;}
		// -------------------------------------------------------------
		// in oberer Schicht ausführen
	g_SetAxes_ErrorAxisHWStall(i, 0);
	g_SetAxes_ErrorAxisHW(i, 0);
	g_SetAxes_ErrorAxisCyBio(i, errNone);
	g_SetAxes_ErrorFlag(i, 0);
	m_maskErrorAxes &= ~g_FelixAxes[i].AxisBit;
	g_SetAxes_HomingFlag(i, FLG_HOMING_FAILURE);
		// -------------------------------------------------------------
		CntStallError[i] = 0;
		g_FlagStallError = 0;
		if(m_maskInversHomingMode & g_FelixAxes[i].AxisBit)
		{
			if((error = IMS_SetS1Invers(g_FelixAxes[i].AxisID)) != errNone){return error;}
		}
		else
		{
			if((error = IMS_SetS1(g_FelixAxes[i].AxisID)) != errNone){return error;}
		}
		if((error = IMS_HomingSequence(g_FelixAxes[i].AxisID, m_AxesMovData[i].HomeToSwitchMode)) != errNone){return error;}

		//m_WhatIsToDo |= HOMING_ACTIVE;
		m_maskStartHomingAxes &= ~g_FelixAxes[i].AxisBit;
		//m_maskRunHomingAxes |= g_FelixAxes[i].AxisBit;
		m_maskRunAxes |= g_FelixAxes[i].AxisBit;
		#ifdef CF_Z_STALL_REPEAT
		m_maskHomingRunAxes |= g_FelixAxes[i].AxisBit;
		#endif
	}
	return error;
}

unsigned short m_AntriebeStarten(unsigned short StartMode)
{
	unsigned short error = errNone;	
	unsigned short i;
	#ifdef _CF_FELIX
	unsigned short Move_First_Rear_then_Front = 0;
	unsigned short RearNichtZweimalStarten = 0;
	long m_F_AktPos = 0;
	long m_R_AktPos = 0;
	#endif
	long lValue;
i = StartMode;// for compiler
	//#ifdef _CF_NRTL
	if(isNRTL_LP())
	{
		if(isFlagFwEStopLock())
		{
			#ifdef _CF_DEBUG_PRINT
			if(g_TestFlag & TF_DBG_FETRESET){dbPrint("RFL0");}
			#endif
			ResetFwEStopLock();
			if((error = SetHoldCurrentAll()) != errNone){return error;}
			K_Task_Wait(200);
			if((error = SetAktuellePositionAll()) != errNone){return error;}
		}
	}
	//#endif

	#ifdef _CF_FELIX
	if((m_maskStartMoveAxes & eFrontCarriage_Axis)&&(m_maskStartMoveAxes & eRearCarriage_Axis))
	{
		if((error= IMS_GetPosition(AXIS_ID_FRONT_CARRIAGE, &lValue)) != errNone){return m_ErrorHandling(0, error);}
		else{m_F_AktPos = lValue;}
		if((error= IMS_GetPosition(AXIS_ID_REAR_CARRIAGE, &lValue)) != errNone){return m_ErrorHandling(0, error);}
		else{m_R_AktPos = lValue;}
		if((m_AxesMovData[AXIS_ID_REAR_CARRIAGE -1].Way < m_R_AktPos)
			&& (m_AxesMovData[AXIS_ID_FRONT_CARRIAGE -1].Way < m_F_AktPos))
		{
			Move_First_Rear_then_Front = 1;
		}
	}
	#endif
	for(i = 0; i < MAX_AXES;i++)
	{
		#ifdef _CF_FELIX
		if(Move_First_Rear_then_Front && (g_FelixAxes[i].AxisBit & eFrontCarriage_Axis))
		{
			++i;
		}
		if(RearNichtZweimalStarten && (g_FelixAxes[i].AxisBit & eRearCarriage_Axis))
		{
			++i;
			RearNichtZweimalStarten = 0;
		}
		#endif
		if((m_maskStartMoveAxes & g_FelixAxes[i].AxisBit) && g_FelixAxes[i].Enable)
		{
			if(g_GetAxes_ErrorAxisHWStall(i) || (g_GetAxes_ErrorAxisHW(i) == 86))
			{
				if((error = SetAktuellePositionID(g_FelixAxes[i].AxisID)) != errNone){return m_ErrorHandling(i, error);}
				g_SetAxes_ErrorAxisHWStall(i, 0);
				g_SetAxes_ErrorAxisHW(i, 0);
				//g_SetAxes_ErrorAxisCyBio(i, errNone);
				g_SetAxes_ErrorFlag(i, 0);
				//g_SetAxes_StallFlag(i, 0);
				m_maskErrorAxes &= ~g_FelixAxes[i].AxisBit;
			}
			if(m_IsZ_Ex_Axis(g_FelixAxes[i].AxisID) && m_Z_StromlosFlag)
			{
				if((error = SetHoldCurrentIx(i)) != errNone){return m_ErrorHandling(i, error);}
				if((error = SetAktuellePositionID(g_FelixAxes[i].AxisID)) != errNone){return m_ErrorHandling(i, error);}
			}
			/*if(m_isSaveEstopPosition && m_UsedSaveStopPos)
			{
				if((error = IMS_MoveAbsolutePos(g_FelixAxes[i].AxisID, m_AxesMovData[i].SaveStopPos)) != errNone){return ErrorHandling(i, error);}
			}
			else
			{*/
			#ifdef CF_Z_STALL_REPEAT
			if(g_ConfigStallExeption[i].Enable)
			{
				CntStallError[i] = 0;g_FlagStallError = 0;
				if((error= IMS_GetPosition(g_FelixAxes[i].AxisID, &StallLastPosition[i])) != errNone){return m_ErrorHandling(i, error);}
			}
			#endif
			if((error = IMS_MoveAbsolutePos(g_FelixAxes[i].AxisID, m_AxesMovData[i].Way)) != errNone)
			{
				return m_ErrorHandling(i, error);
			}
			//}
			//m_WhatIsToDo |= MOVING_ACTIVE;
			
			m_maskStartMoveAxes &= ~g_FelixAxes[i].AxisBit;
			m_maskRunAxes |= g_FelixAxes[i].AxisBit;
		}
		#ifdef _CF_FELIX
		if(Move_First_Rear_then_Front && (g_FelixAxes[i].AxisBit & eRearCarriage_Axis))
		{
			i=0;
			Move_First_Rear_then_Front = 0;
			RearNichtZweimalStarten = 1;
		}
		#endif
	}
	return error;
}

unsigned short m_WartenAufBewegungsEnde(unsigned short Stopmode)
{
	unsigned short error = errNone;
	unsigned short usValue;
	unsigned short i, a;
	unsigned short AxisError = 0;
	unsigned short cntUpDown = 0;
i =Stopmode;// für compiler

	// ############ Achsen sind in Bewegung ################
	if(m_maskRunAxes)
	{
		for(i = 0; i < MAX_AXES;i++)
		{
			K_Task_Wait(1);
			if((m_maskRunAxes & g_FelixAxes[i].AxisBit) && g_FelixAxes[i].Enable)
			{
				error = IMS_GetMoveFlag(g_FelixAxes[i].AxisID, &usValue);
				#ifdef CF_Z_STALL_REPEAT
				if(error != errNone)
				{
					AxisError = 1;
					if((g_ConfigStallExeption[i].Enable && !(m_maskHomingRunAxes & g_FelixAxes[i].AxisBit)) && (g_GetAxes_ErrorAxisHWStall(i) || (g_GetAxes_ErrorAxisHW(i) == 86)))
					{
						if((Stopmode == NORMALMODE) || (Stopmode == ESTOP_STOPWAIT))
						{
							AxisError = 0;
							if(StallLastPosition[i] > m_AxesMovData[i].Way)
							{
								//nach unten fahren
								cntUpDown = g_ConfigStallExeption[i].Down_DirB_Counter;
							}
							else
							{
								//nach oben fahren
								cntUpDown = g_ConfigStallExeption[i].Up_DirA_Counter;
							}
							g_FlagStallError = 1;
							if(CntStallError[i] == 0)
							{
								g_CounterStallExeption[i].ExeptionLifeTimeCounter += 1;
							}
							CntStallError[i] += 1;

							if(CntStallError[i] > cntUpDown)
							{
								IMS_StopMotor(FELIX_BASE_ID);
								g_FlagStallError = 0;FRAMSaveAxisCounterStallExeption(g_CounterStallExeption);
								return m_ErrorHandlingCore(i, error);
							}
							K_Task_Wait(g_ConfigStallExeption[i].WaitTime);
							/*if((error = SetAktuellePositionID(g_FelixAxes[i].AxisID)) != errNone)
							{
								IMS_StopMotor(FELIX_BASE_ID);
								g_FlagStallError = 0;
								FRAMSaveAxisCounterStallExeption(g_CounterStallExeption);
								return m_ErrorHandlingCore(i, error);
							}*/
							for(a = 4; a >= 0; a--)
							{
								if((error = SetAktuellePositionID(g_FelixAxes[i].AxisID)) != errNone)
								{
									if(!a)
									{
										IMS_StopMotor(FELIX_BASE_ID);
										g_FlagStallError = 0;
										FRAMSaveAxisCounterStallExeption(g_CounterStallExeption);
										return m_ErrorHandlingCore(i, error);
									}
									else
									{
										K_Task_Wait(g_ConfigStallExeption[i].WaitTime);
										if((error = IMS_ResetError(g_FelixAxes[i].AxisID)) != errNone)
										{
											IMS_StopMotor(FELIX_BASE_ID);
											g_FlagStallError = 0;
											FRAMSaveAxisCounterStallExeption(g_CounterStallExeption);
											return m_ErrorHandlingCore(i, error);
										}
										if((error = IMS_ResetStallFlag(g_FelixAxes[i].AxisID)) != errNone)
										{
											IMS_StopMotor(FELIX_BASE_ID);
											g_FlagStallError = 0;
											FRAMSaveAxisCounterStallExeption(g_CounterStallExeption);
											return m_ErrorHandlingCore(i, error);
										}
									}
								}
								else
								{
									break;
								}

							}
							g_SetAxes_ErrorAxisHWStall(i, 0);
							g_SetAxes_ErrorAxisHW(i, 0);
							//g_SetAxes_ErrorAxisCyBio(i, errNone);g_SetAxes_ErrorFlag(i, 0);
							if((error = IMS_MoveAbsolutePos(g_FelixAxes[i].AxisID, m_AxesMovData[i].Way)) != errNone)
							{
								IMS_StopMotor(FELIX_BASE_ID);
								g_FlagStallError = 0;
								FRAMSaveAxisCounterStallExeption(g_CounterStallExeption);
								return m_ErrorHandlingCore(i, error);
							}
						}
					}
					if(AxisError){IMS_StopMotor(FELIX_BASE_ID);m_maskHomingRunAxes = 0;return m_ErrorHandlingCore(i, error);}
				}
				#else
				if(error != errNone)
				{
					
					IMS_StopMotor(FELIX_BASE_ID);//NEWERR
					#ifdef _CF_DEBUG_PRINT
					if(g_TestFlag & TF_DBG_EVENT){dbPrint("WAB Error i:%u E:%u\r", i,  g_GetAxes_ErrorAxisHW(i));}
					#endif
					return m_ErrorHandlingCore(i, error);
				}
				#endif
				if(usValue==0)
				{
					m_maskRunAxes &= ~g_FelixAxes[i].AxisBit;
					m_maskHomingRunAxes &= ~g_FelixAxes[i].AxisBit;
					if(m_IsZ_Ex_Axis(g_FelixAxes[i].AxisID))
					{
						#ifdef _CF_DEBUG_PRINT
						if(g_TestFlag & TF_DBG_BREAKE){dbPrint("Z SetTime 1\r");}
						#endif
						SetTimeZDrive(Z_BREAKE_TIME);
					}
					if(!m_maskRunAxes && g_FlagStallError)
					{
						g_FlagStallError = 0;
						FRAMSaveAxisCounterStallExeption(g_CounterStallExeption);
					}
				}
			}
		}//end...for(i = 0; i < MAX_AXES;i++)
	}
	return errNone;	
}

unsigned short m_SetHoldCurrentAndPos(void)
{
	unsigned short error = errNone;
	if(m_StromlosActive)
	{
		//in Antrieb starten verlegen
		#ifdef _CF_DEBUG_PRINT
		if(g_TestFlag & TF_DBG_EVENT){dbPrint("Move SetHoldCurrent\r");}
		#endif
		if((error = SetHoldCurrentAll()) != errNone){return error;}
		K_Task_Wait(200);
		if((error = SetAktuellePositionAll()) != errNone){return error;}
		m_StromlosActive = 0;
	}
	return error;
}


unsigned short m_EStopRestorCurrentAndPos(unsigned short *Vergleich)
{
	unsigned short error = errNone;
	//unsigned short Vergleich;
	if((error = m_SetHoldCurrentAndPos()) != errNone){ErrorLED();return error;}
	if((error = m_PositionsVergleichSaveVsAktuell(Vergleich)) != errNone){ErrorLED();return error;}
	return error;
}

unsigned short m_SaveEstopPosition(void)
{
	unsigned short error;
	unsigned short errorsave = errNone;
	unsigned short i;
	long lValue;

	for(i = 0; i < MAX_AXES;i++)
	{
		if(g_FelixAxes[i].Enable)
		{
			if((error = IMS_GetPosition(g_FelixAxes[i].AxisID, &lValue)) != errNone){errorsave = m_ErrorHandling(i, error);}
			m_AxesMovData[i].SaveStopPos = lValue;
		}
	}//end...for(i = 0; i < MAX_AXES;i++)
	if(errorsave == errNone){m_isSaveEstopPosition = 1;}else {m_isSaveEstopPosition = 0;}
	return errorsave;
}

unsigned short m_PositionsVergleichSaveVsAktuell(unsigned short *result)
{
	unsigned short error;
	long ActPos;
	long Diff;
	long actdiff;
	unsigned short i;
	for(i = 0; i < MAX_AXES;i++)
	{
		if(g_FelixAxes[i].Enable)
		{
			if((error = IMS_GetPosition(g_FelixAxes[i].AxisID, &ActPos)) != errNone)
			{*result = FALSE;return m_ErrorHandling(i, error);}
			Diff = (long)(pIMSFixedPara[i]->fDiffRestoreActivate * pIMSFixedPara[i]->fENCImpProMM);
			Diff = labs(Diff);
			actdiff = ActPos - m_AxesMovData[i].SaveStopPos;
			actdiff = labs(actdiff);
			if(actdiff > Diff)
			{
				*result = TRUE;
				return errNone;
			}
		}
	}//end...for(i = 0; i < MAX_AXES;i++)
	*result = FALSE;
	return errNone;
}

void m_MT_UserBreak(void)
{
	unsigned short error = errNone;
	unsigned short i;

	IMS_ESC_StopMotor();
	i = 0;
	while(m_maskRunAxes)
	{
		K_Task_Wait(10);
		i++;
		error = m_WartenAufBewegungsEnde(USERBREAKMODE);
		if(error != errNone) break;
		if(i > 300)
		{
			error = ERR_SYSTEM;
			break;
		}
	}
	if(error != errNone) {K_Task_Wait(1000);}
	for(i = 0; i < MAX_AXES;i++)
	{
		WDO();
		g_SetAxes_ErrorAxisCyBio(i, ERR_PROG_ABORT);
		/* 2016.03.01 g_SetAxes_ErrorFlag(i, 0);
		m_maskErrorAxes &= ~g_FelixAxes[i].AxisBit;
		*/
		g_SetAxes_ErrorFlag(i, 1);
		m_maskErrorAxes |= g_FelixAxes[i].AxisBit;

		g_SetAxes_Running(i, STATE_ERROR);

	}
	ErrorLED();
}


unsigned short StromlosAll(void)
{	   
	unsigned short error;
	unsigned short errorsave = errNone;
	unsigned short i;
	Z_Breake_Fixed();
	m_Z_StromlosFlag = 1;
	for(i = 0; i < MAX_AXES;i++)
	{
		if(g_FelixAxes[i].Enable)
		{
			if((error = IMS_SetHoldCurrent(g_FelixAxes[i].AxisID, 0)) != errNone){errorsave = m_ErrorHandling(i, error);}
		}

	}//end...for(i = 0; i < MAX_AXES;i++)
	return errorsave;
}

unsigned short StromlosAxisWithBrakeAll(void)
{
	unsigned short error = errNone;
	unsigned short i;
	Z_Breake_Fixed();
	m_Z_StromlosFlag = 1;
	for(i = 0; i < MAX_AXES;i++)
	{
		if(g_FelixAxes[i].Enable && g_FelixAxes[i].AxisWithBrake)
		{
			if((error = IMS_SetHoldCurrent(g_FelixAxes[i].AxisID, 0)) != errNone){return m_ErrorHandling(i, error);}
			K_Task_Wait(10);
		}
	}
	return error;
}
unsigned short SetHoldCurrentAll(void)
{	   
	unsigned short error = errNone;
	unsigned short i;

	for(i = 0; i < MAX_AXES;i++)
	{
		if(g_FelixAxes[i].Enable)
		{
			if((error= SetHoldCurrentIx(i)) != errNone){return m_ErrorHandling(i, error);}
		}
	}
	return error;
}
void varSetNormalHoldCurrentIx(unsigned short i)
{
	m_AxesMovData[i].ModeHoldCurrent = pIMSChangedPara[g_FelixAxes[i].ParaIDX]->usHoldCurrent;
}

void varSet1536HoldCurrentIx(unsigned short i)
{
	m_AxesMovData[i].ModeHoldCurrent = pIMSChangedPara[g_FelixAxes[i].ParaIDX]->usHoldCurrent1536;
}

unsigned short SetHoldCurrentIx(unsigned short i)
{
	return SetActHoldCurrentIx( i, m_AxesMovData[i].ModeHoldCurrent);
}


unsigned short SetActHoldCurrentIx(unsigned short i, unsigned short HoldCurrent)
{
	unsigned short error = errNone;
	unsigned short a;
	if(m_IsZ_Ex_Axis(g_FelixAxes[i].AxisID))
	{
		if(m_Z_StromlosFlag)
		{
			for(a = 0; a < MAX_AXES;a++)
			{
				if(g_FelixAxes[a].Enable && g_FelixAxes[a].AxisWithBrake)
				{
					if((error = IMS_SetHoldCurrent(g_FelixAxes[a].AxisID, HoldCurrent)) != errNone){return error;}
					K_Task_Wait(10);
					if((error = IMS_ResetError(g_FelixAxes[a].AxisID)) != errNone){return error;}
					if((error = IMS_ResetStallFlag(g_FelixAxes[a].AxisID)) != errNone){return error;}
					if((error = SetAktuellePositionID(g_FelixAxes[a].AxisID)) != errNone){return error;}
				}
			}
		}
		else
		{
			if((error = IMS_SetHoldCurrent(g_FelixAxes[i].AxisID, HoldCurrent)) != errNone){return error;}
			K_Task_Wait(10);
			if((error = IMS_ResetError(g_FelixAxes[i].AxisID)) != errNone){return error;}
			if((error = IMS_ResetStallFlag(g_FelixAxes[i].AxisID)) != errNone){return error;}
		}
		m_Z_StromlosFlag = 0;
		Z_Breake_Free();
		#ifdef _CF_DEBUG_PRINT
		if(g_TestFlag & TF_DBG_BREAKE){dbPrint("Z breake free 3\r");}
		#endif
		SetTimeZDrive(Z_BREAKE_TIME);
	}
	else
	{
		if((error = IMS_SetHoldCurrent(g_FelixAxes[i].AxisID, HoldCurrent)) != errNone){return error;}
		K_Task_Wait(10);
		if((error = IMS_ResetError(g_FelixAxes[i].AxisID)) != errNone){return error;}
		if((error = IMS_ResetStallFlag(g_FelixAxes[i].AxisID)) != errNone){return error;}
	}
	return error;
}

unsigned short Set1536RunCurrentIx(unsigned short i)
{
	unsigned short error;
	error = IMS_SetRunCurrent(g_FelixAxes[i].AxisID, pIMSChangedPara[g_FelixAxes[i].ParaIDX]->usRunCurrent1536);
	return error;
}

unsigned short SetRunCurrentAll(void)
{
	unsigned short error = errNone;
	unsigned short i;

	for(i = 0; i < MAX_AXES;i++)
	{
		if(g_FelixAxes[i].Enable)
		{
			if((error= SetRunCurrentIx(i)) != errNone){return m_ErrorHandling(i, error);}
		}
	}//end...for(i = 0; i < MAX_AXES;i++)
	return error;
}
unsigned short SetRunCurrentIx(unsigned short i)
{
	unsigned short error;
	error= IMS_SetRunCurrent(g_FelixAxes[i].AxisID, pIMSChangedPara[g_FelixAxes[i].ParaIDX]->usRunCurrent);
	return error;
}

unsigned short SetNormalSpeedAll(void)
{
	unsigned short error = errNone;
	unsigned short i;

	for(i = 0; i < MAX_AXES;i++)
	{
		if(g_FelixAxes[i].Enable)
		{
			if((error= SetNormalSpeedIx(i)) != errNone){return m_ErrorHandling(i, error);}
		}
	}
	return error;
}
unsigned short SetNormalSpeedIx(unsigned short i)
{
	unsigned short error;

	if((error= IMS_SetDecToAcc(g_FelixAxes[i].AxisID)) != errNone){return error;}
	return IMS_SetSpeed(g_FelixAxes[i].AxisID, (float)-1.0, pIMSChangedPara[g_FelixAxes[i].ParaIDX]->fEndSpeed, pIMSChangedPara[g_FelixAxes[i].ParaIDX]->fAcceleration);
}
/*unsigned short SetHomingSpeedAll(void)
{
	unsigned short error;
	unsigned short errorsave = errNone;
	unsigned short i;

	for(i = 0; i < MAX_AXES;i++)
	{
		if(g_FelixAxes[i].Enable)
		{
			if((error= SetHomingSpeedIx(i)) != errNone){errorsave = m_ErrorHandling(i, error);}
		}
	}
	return errorsave;
}*/
unsigned short SetHomingSpeedIx(unsigned short i)
{
	unsigned short error;

	if((error= IMS_SetDecToAcc(g_FelixAxes[i].AxisID)) != errNone){return error;}
	return IMS_SetSpeed(g_FelixAxes[i].AxisID, (float)-1.0, pIMSChangedPara[g_FelixAxes[i].ParaIDX]->fHomingSpeed, pIMSChangedPara[g_FelixAxes[i].ParaIDX]->fAccelerationHoming);
}
unsigned short Set1536SpeedIx(unsigned short i)
{
	unsigned short error;

	if((error= IMS_SetDecToAcc(g_FelixAxes[i].AxisID)) != errNone){return error;}
	return IMS_SetSpeed(g_FelixAxes[i].AxisID, (float)-1.0, pIMSChangedPara[g_FelixAxes[i].ParaIDX]->f1536Speed, pIMSChangedPara[g_FelixAxes[i].ParaIDX]->fAcceleration1536);
}

unsigned short SetNormalDeadBandIx(unsigned short i)
{
	return SetDeadBand( i, pIMSChangedPara[g_FelixAxes[i].ParaIDX]->fPrecisionDeadband);
}

unsigned short Set1536DeadBandIx(unsigned short i)
{
	return SetDeadBand( i, pIMSChangedPara[g_FelixAxes[i].ParaIDX]->fPrecisionDeadband1536);
}
unsigned short SetDeadBand(const unsigned short i, double value)
{
	unsigned short db;
	double db_double;

	db_double = fabs(pIMSFixedPara[i]->fENCImpProMM  * value);
	db = RoundToLong(db_double);
	if(db < DB_RANGE_MIN)
	{
		db = 0;//db = DB_RANGE_MIN;
	}
	return IMS_SetEncoderDeadBand(g_FelixAxes[i].AxisID, db);
}

unsigned short SetNormalMotorWobbleWaitTimeIx(unsigned short i)
{
	return IMS_SetMotorWobbleWaitTime(g_FelixAxes[i].AxisID, pIMSChangedPara[g_FelixAxes[i].ParaIDX]->usMotorWobbleWaitTime);
}

unsigned short Set1536MotorWobbleWaitTimeIx(unsigned short i)
{
	return IMS_SetMotorWobbleWaitTime(g_FelixAxes[i].AxisID, pIMSChangedPara[g_FelixAxes[i].ParaIDX]->usMotorWobbleWaitTime1536);
}

unsigned short SetMode1536Ix(unsigned short i)
{
	unsigned short error;

 	if((pIMSChangedPara[g_FelixAxes[i].ParaIDX]->fEndSpeed < pIMSChangedPara[g_FelixAxes[i].ParaIDX]->f1536Speed) &&
 	(pIMSChangedPara[g_FelixAxes[i].ParaIDX]->fAcceleration < pIMSChangedPara[g_FelixAxes[i].ParaIDX]->fAcceleration1536))
	{
		return SetModeNormalIx(i);
	}
	else
	{
		if((error= Set1536DeadBandIx(i)) != errNone){return error;}
		if((error= Set1536MotorWobbleWaitTimeIx(i)) != errNone){return error;}
		varSet1536HoldCurrentIx(i);
		if((error= Set1536RunCurrentIx(i)) != errNone){return error;}
		if((error= Set1536SpeedIx(i)) != errNone){return error;}
		m_AxesMovData[i].getSpeedMode = SPD_1536;
	}
	return error;
}
unsigned short SetModeNormalIx(unsigned short i)
{
	unsigned short error;
	if((error= SetNormalDeadBandIx(i)) != errNone){return error;}
	if((error= SetNormalMotorWobbleWaitTimeIx(i)) != errNone){return error;}
	varSetNormalHoldCurrentIx(i);
	if((error= SetRunCurrentIx(i)) != errNone){return error;}
	if((error= SetNormalSpeedIx(i)) != errNone){return error;}
	m_AxesMovData[i].getSpeedMode = SPD_NORMAL;
	return error;
}
unsigned short SetModeHomingIx(unsigned short i)
{
	unsigned short error;

	if((error= SetNormalDeadBandIx(i)) != errNone){return error;}
	if((error= SetNormalMotorWobbleWaitTimeIx(i)) != errNone){return error;}
	varSetNormalHoldCurrentIx(i);
	if((error= SetRunCurrentIx(i)) != errNone){return error;}
	if((error= SetHomingSpeedIx(i)) != errNone){return error;}
	m_AxesMovData[i].getSpeedMode = SPD_HOMING;
	return error;
}

unsigned short SetModeSlewIx(unsigned short i)
{
	unsigned short error;
	#ifdef _CF_DEBUG_PRINT
	if(g_TestFlag & TF_DBG_MOTTSK)
	{
		dbPrint("SetModeSlewIx: %u \r",i);
	}
	#endif
	if((error= SetNormalDeadBandIx(i)) != errNone){return error;}
	if((error= SetNormalMotorWobbleWaitTimeIx(i)) != errNone){return error;}
	varSetNormalHoldCurrentIx(i);
	if((error= SetRunCurrentIx(i)) != errNone){return error;}
	if((error= SetHomingSpeedIx(i)) != errNone){return error;}
	m_AxesMovData[i].getSpeedMode = SPD_HOMING;
	return error;
}

unsigned short SetModeMask(unsigned short Mask)
{
	unsigned short error = errNone;
	unsigned short i;
	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if((Mask & g_FelixAxes[i].AxisBit) && g_FelixAxes[i].Enable)
		{
			m_maskSetMode1536 &= ~g_FelixAxes[i].AxisBit;
			if((error= SetModeNormalIx(i)) != errNone){return m_ErrorHandling(i, error);}
		}
	}
	return error;
}


unsigned short SetAktuellePositionAll(void)
{
	unsigned short error;
	unsigned short errorsave = errNone;
	unsigned short i;

	for(i = 0; i < MAX_AXES;i++)
	{
		if(g_FelixAxes[i].Enable)
		{
			if((error= SetAktuellePositionID(g_FelixAxes[i].AxisID)) != errNone) {errorsave = m_ErrorHandling(i, error);}
		}
	}//end...for(i = 0; i < MAX_AXES;i++)

	return errorsave;
}
unsigned short SetAktuellePositionID(unsigned short ID)
{
	unsigned short error;
	long lValue;

	if((error= IMS_GetPosition(ID, &lValue)) == errNone)
	{
		#ifdef _CF_FELIX
		switch(ID)
		{
			case AXIS_ID_FRONT_CARRIAGE:m_F_oldPosStep = lValue;
										break;
			case AXIS_ID_REAR_CARRIAGE:m_R_oldPosStep = lValue;
										break;
			default:break;
		}
		#endif
		if((error = IMS_SetPosition(ID, lValue)) != errNone){return error;}
		K_Task_Wait(5);
		if((error = IMS_ResetError(ID)) != errNone){return error;}
		if((error = IMS_ResetStallFlag(ID)) != errNone){return error;}
	}
	return error;
}


unsigned short SlewStartAll(unsigned short Mask)
{
	unsigned short error = ERR_UNKNOWN_DRV;
	unsigned short i;

	//Strom setzen
	//m_SpeedState = SPD_UNKNOWN;
	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if((Mask & g_FelixAxes[i].AxisBit) && g_FelixAxes[i].Enable)
		{
			if((error = SlewStartID(g_FelixAxes[i].AxisID, i)) != errNone)
			{
				Slew_ErrorAll(Mask);
				//m_SpeedState = SPD_UNKNOWN;
				return error;
			}
		}
	}
	//m_SpeedState = SPD_SLEW;
	return error;
}

unsigned short SlewStartID(unsigned short ID, unsigned short i)
{
	unsigned short error;
	if((error= SetHoldCurrentIx(i)) != errNone){return error;}
	//if(ID == FixZDrive.ID)
	if(m_IsZ_Ex_Axis(ID))
	{
		m_Enable_Z_Stromlos = 0;
	}
	//if((error= SetSlewSpeedIx(i)) != errNone){return error;}
	//if((error= SetSloMoRunCurrentIx(i)) != errNone){return error;}
	if((error= SetModeSlewIx(i)) != errNone){return error;}
	return SetAktuellePositionID(g_FelixAxes[i].AxisID);
}

unsigned short SlewEndAll(unsigned short Mask)
{
	unsigned short error = ERR_UNKNOWN_DRV;
	unsigned short i;

	//Strom setzen
	//m_SpeedState = SPD_UNKNOWN;
	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if((Mask & g_FelixAxes[i].AxisBit) && g_FelixAxes[i].Enable)
		{

			if((error = SlewEndID(g_FelixAxes[i].AxisID, i)) != errNone)
			{
				Slew_ErrorStopAll(Mask);
				//m_SpeedState = SPD_UNKNOWN;
				return error;
			}
		}
	}
	return error;

}

unsigned short SlewEndID(unsigned short ID, unsigned short i)
{
	unsigned short error;
	unsigned short errorSave = errNone;
	unsigned short MoveFlg;
	//if(ID == FixZDrive.ID)
	if(m_IsZ_Ex_Axis(ID))
	{
		m_Enable_Z_Stromlos = 1;
	}
	if((error= IMS_SlewMove(ID, 0)) != errNone)
	{
		Slew_ErrorStopID(ID);
		return error;
	}
	MoveFlg = 0;
	do{
		WDO();
		if((error = IMS_GetMoveFlag(ID, &MoveFlg)) != errNone)
		{
			errorSave = error;
			MoveFlg = 0;
		}
	}while(MoveFlg);
	K_Task_Wait(10);
//	if(GetDriveMode() == DRV_MODE_NORMAL)
//	{
		if(m_maskSetMode1536 & g_FelixAxes[i].AxisBit)
		{
			if((error = SetMode1536Ix(i)) != errNone) {errorSave = error;}
		}
		else
		{
			if((error = SetModeNormalIx(i)) != errNone) {errorSave = error;}
		}
		//m_SpeedState = SPD_NORMAL;
//	}
	return errorSave;
}

unsigned short Slew_ErrorStopAll(unsigned short Mask)
{
	unsigned short i;

	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if((Mask & g_FelixAxes[i].AxisBit) && g_FelixAxes[i].Enable)
		{
			Slew_ErrorStopID(g_FelixAxes[i].AxisID);
		}
	}
	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if((Mask & g_FelixAxes[i].AxisBit) && g_FelixAxes[i].Enable)
		{
			Slew_Error( i);
		}
	}
	return errNone;
}

unsigned short Slew_ErrorStopID(unsigned short ID)
{
	unsigned short error = errNone;
	unsigned short MoveFlg;

	if((error = IMS_SlewMove(ID, 0)) != errNone){return error;}
	MoveFlg = 0;
	do{
		WDO();
		if((error = IMS_GetMoveFlag(ID, &MoveFlg)) != errNone)
		{
			MoveFlg = 0;
		}
	}while(MoveFlg);
	K_Task_Wait(10);
	return StromlosAxisWithBrakeAll();
}
unsigned short SetSlewSpeedIx(unsigned short i)
{
	unsigned short error;

	if((error= IMS_SetDecToAcc(g_FelixAxes[i].AxisID)) != errNone){return error;}
	return IMS_SetSpeed(g_FelixAxes[i].AxisID, (float)-1.0, pIMSChangedPara[g_FelixAxes[i].ParaIDX]->fEndSpeed, pIMSChangedPara[g_FelixAxes[i].ParaIDX]->fAccelerationSlew);
}

unsigned short Slew_ErrorAll(unsigned short Mask)
{
	unsigned short error = errNone;
	unsigned short i;

	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if((Mask & g_FelixAxes[i].AxisBit) && g_FelixAxes[i].Enable)
		{
			Slew_Error(i);
		}
	}
	return error;
}
unsigned short Slew_Error(unsigned short i)
{
	unsigned short error = errNone;

	if((error = StromlosAxisWithBrakeAll()) != errNone){return error;}
//	if(GetDriveMode() == DRV_MODE_NORMAL)
//	{
		if(m_maskSetMode1536 & g_FelixAxes[i].AxisBit)
		{
			if((error = SetMode1536Ix(i)) != errNone) {return error;}
		}
		else
		{
			if((error = SetModeNormalIx(i)) != errNone) {return error;}
		}
//	}
	return error;
}

/*unsigned short SetSlewSpeedAll(void)
{
	unsigned short error;
	unsigned short i;

	for(i = 0; i < MAX_AXES;i++)
	{
		if((error= SetSlewSpeedIx(i)) != errNone){return error;}

	}//end...for(i = 0; i < MAX_AXES;i++)
	return error;
}*/

void ErrorLED(void)
{
	LED_Red_On();
}
