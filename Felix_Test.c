//****************************************************************************
// @Module        Felix FRAM
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


#include "DevelpomentBase/DevError.h"
#include "DeviceConfig.h"
#include "Cxconfig.h"
#include "Felix_Cmd.h"
#include "MDrive_CmdCtrl.h"
#include "Felix_ASyncCtrl.h"
#include "MotorExe.h"
#include "30-5015_Felix/XE167F/main.h"
#include "Felix_HardwareTask.h"
#include "MDriveCmd.h"
#include "Felix_Misc.h"
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
extern ASync_CommandBlock g_MsgASync;
//****************************************************************************
// public data
//****************************************************************************

//****************************************************************************
// private data
//****************************************************************************

//****************************************************************************
// private functions
//****************************************************************************
//unsigned short CmdTestSynchronHoming(unsigned short Mask);
//unsigned short TestMotorMoveSyncronExe(unsigned short Mask);
//unsigned short TestWaitSynchronMoveEstop(unsigned short Mask);

/*unsigned short CmdTestSynchronHoming(unsigned short Mask)
{
	unsigned short error;
	unsigned short value;
	if((error = CmdIntern_Homing(Mask)) != errNone)
	{
		return error;
	}

	do{
		K_Task_Wait(50);
		WDO();
		if((error= g_ASync_GetAllRuningFlags(Mask, &value)) != errNone)
		{
			return error;
		}
		if(GetDriveMode() == DRV_MODE_ESTOP)
		{
			//return errSafetyAppliance;
			do{
				K_Task_Wait(50);
				WDO();
			}while(GetEStopHalt()==0);
			SetEStopHalt(0);
			error = TestWaitSynchronMoveEstop(Mask);
		}
	}while((value == STATE_BUSY)  || (value == STATE_BUSY_ESTOP));
	if((error = g_ASync_GetAllErrorFlags(Mask, &value)) != errNone) {return error;}
	return value;
}


unsigned short TestMotorMoveSyncronExe(unsigned short Mask)
{
	unsigned short error = errNone;
	unsigned short value;
	//unsigned short HelpMask;
	error = MotorMoveSyncronExe(Mask,0);
	if(error == errSafetyAppliance)
	{
		//HelpMask = Mask;
		// wait for estop anhalten
		do{
			K_Task_Wait(50);
			WDO();
		}while(GetEStopHalt()==0);
		SetEStopHalt(0);
		error = TestWaitSynchronMoveEstop(Mask);
	}
	if(error == errNone)
	{
		if((error = g_ASync_GetAllErrorFlags(Mask, &value)) != errNone) {return error;}
		error = value;
	}
	return error;
}


unsigned short TestWaitSynchronMoveEstop(unsigned short Mask)
{
	unsigned short error = errNone;
	unsigned short value;
	unsigned short sperre = 1;
	do{
		K_Task_Wait(50);
		WDO();
		if(GetDriveMode() != DRV_MODE_ESTOP)
		{
			ResetEstopWarOffen();
			if(sperre)
			{
				sperre = 0;
				error = ImsCmd_EnableEStopRestorePosition();
			}
		}
		else
		{
			sperre = 1;
		}
		if((error= g_ASync_GetAllRuningFlags(Mask, &value)) != errNone){ return error;}
	}while((value == STATE_BUSY)  || (value == STATE_BUSY_ESTOP));
	if((error = g_ASync_GetAllErrorFlags(Mask, &value)) != errNone) {return error;}
//	if(g_TestFlag & TF_DBG_INT_COM){DebugPrintPosition(m_MaskSynchronMoveEstop);}
	return value;
}
*/
//****************************************************************************
// public functions
//****************************************************************************
/*
unsigned short Cmd_TestXZ(void)
{
	unsigned short error = errNone;
	unsigned short Mask = 0;
	Mask = eX_Axis | eZ_Axis;
	if((error = CmdTestSynchronHoming(eX_Axis | eFrontCarriage_Axis | eRearCarriage_Axis | eBottomCarriage | eZ_Axis)) != errNone) return error;
	while(1)
	{
		g_MsgASync.MoveData[0].AbsolutePos = 338.0;//X
		g_MsgASync.MoveData[4].AbsolutePos = -140.0;//Z
		if((error = TestMotorMoveSyncronExe(Mask)) != errNone) return error;
		if(!GetFlagDeviceTest(DevTestXZ))
		{
			if(DIL1_1) return errNone;
		}
		g_MsgASync.MoveData[0].AbsolutePos = 0.0;//X
		g_MsgASync.MoveData[4].AbsolutePos = 0.0;//Z
		if((error = TestMotorMoveSyncronExe(Mask)) != errNone) return error;
		if(!GetFlagDeviceTest(DevTestXZ))
		{
			if(DIL1_1) return errNone;
		}
	}
}

unsigned short Cmd_TestZ(void)
{
	unsigned short error = errNone;
//	unsigned short ID;
	unsigned short Mask = 0;
	float F_Mitte = 0.0;
	float F_Home = -135.0;
	float R_Mitte = 0.0;
	float R_Home = 135.0;
	float B_Mitte = 0.0;
	float B_Home = 135.0;

	Mask = eX_Axis | eFrontCarriage_Axis | eRearCarriage_Axis | eBottomCarriage | eZ_Axis;
	if((error = CmdTestSynchronHoming(eX_Axis | eFrontCarriage_Axis | eRearCarriage_Axis | eBottomCarriage | eZ_Axis)) != errNone) return error;
	g_MsgASync.MoveData[0].AbsolutePos = 180.0;//X
	g_MsgASync.MoveData[1].AbsolutePos = F_Home;//F
	g_MsgASync.MoveData[2].AbsolutePos = R_Home;//R
	g_MsgASync.MoveData[3].AbsolutePos = B_Home;//B
	g_MsgASync.MoveData[4].AbsolutePos = 0.0;//Z
	if((error = TestMotorMoveSyncronExe(Mask)) != errNone) return error;
//	if((error = MotorSetSpeedExe( FixZDrive.ID, (float)-1.0, (float)260.0, (float)1000.0)) != errNone) return error;
	Mask = eZ_Axis ;
	while(1)
	{
		g_MsgASync.MoveData[0].AbsolutePos = -200.0;//Z
		if((error = TestMotorMoveSyncronExe(Mask)) != errNone) return error;
		if(!GetFlagDeviceTest(DevTestZ))
		{
			if(DIL1_3) return errNone;
		}
		g_MsgASync.MoveData[0].AbsolutePos = 0.0;//Z
		if((error = TestMotorMoveSyncronExe(Mask)) != errNone) return error;
		if(!GetFlagDeviceTest(DevTestZ))
		{
			if(DIL1_3) return errNone;
		}
	}
}

unsigned short Cmd_TestCar(void)
{
	unsigned short error = errNone;
//	unsigned short ID;
	unsigned short Mask = 0;
	float F_Mitte = 0.0;
	float F_Home = -135.0;
	float R_Mitte = 0.0;
	float R_Home = 135.0;
	float B_Mitte = 0.0;
	float B_Home = 135.0;

	Mask = eFrontCarriage_Axis | eRearCarriage_Axis | eBottomCarriage;
	if((error = CmdTestSynchronHoming(eX_Axis | eFrontCarriage_Axis | eRearCarriage_Axis | eBottomCarriage | eZ_Axis)) != errNone) return error;
	while(1)
	{
		g_MsgASync.MoveData[1].AbsolutePos = F_Mitte;//F
		g_MsgASync.MoveData[2].AbsolutePos = R_Home;//R
		g_MsgASync.MoveData[3].AbsolutePos = B_Home;//B
		if((error = TestMotorMoveSyncronExe(Mask)) != errNone) return error;
		if(!GetFlagDeviceTest(DevTestCAR))
		{
			if(DIL1_2) return errNone;
		}
		g_MsgASync.MoveData[1].AbsolutePos = F_Home;//F
		g_MsgASync.MoveData[2].AbsolutePos = R_Mitte;//R
		g_MsgASync.MoveData[3].AbsolutePos = B_Mitte;//B
		if((error = TestMotorMoveSyncronExe(Mask)) != errNone) return error;
		if(!GetFlagDeviceTest(DevTestCAR))
		{
			if(DIL1_2) return errNone;
		}
	}
}

unsigned short Cmd_TestAll(void)
{
	unsigned short error = errNone;
//	unsigned short ID;
	unsigned short Mask = 0;
	short i;
	float x_pos, z_hoch, z_runter;
	float F_Mitte = 0.0;
	float F_Home = -135.0;
	float R_Mitte = 0.0;
	float R_Home = 135.0;
	float B_Mitte = 0.0;
	float B_Home = 135.0;

//	float Value = 0;
	Mask = eX_Axis | eFrontCarriage_Axis | eRearCarriage_Axis | eBottomCarriage | eZ_Axis;
	if((error = CmdTestSynchronHoming(eX_Axis | eFrontCarriage_Axis | eRearCarriage_Axis | eBottomCarriage | eZ_Axis)) != errNone) return error;
*
1 sSpd 16 140 1000
1 sSpd 16 250 2000
1 sSpd 16 220 2000
1 sSpd 16 300 3000
  *

	while(1)
	{
		g_MsgASync.MoveData[0].AbsolutePos = 145.0;//X
		g_MsgASync.MoveData[1].AbsolutePos = F_Home;//F
		g_MsgASync.MoveData[2].AbsolutePos = R_Mitte;//R
		g_MsgASync.MoveData[3].AbsolutePos = B_Mitte;//B
		g_MsgASync.MoveData[4].AbsolutePos = -65.0;//Z
		if((error = MotorSetSpeedExe( FixZDrive.ID, (float)-1.0, (float)300.0, (float)3000.0)) != errNone) return error;
		if((error = TestMotorMoveSyncronExe(Mask)) != errNone) return error;
		if(!GetFlagDeviceTest(DevTestALL)){if(DIL1_1 & DIL1_2) return errNone;}
		g_MsgASync.MoveData[0].AbsolutePos = 290.0;//X
		g_MsgASync.MoveData[1].AbsolutePos = F_Mitte;//F
		g_MsgASync.MoveData[2].AbsolutePos = R_Home;//R
		g_MsgASync.MoveData[3].AbsolutePos = B_Home;//B
		g_MsgASync.MoveData[4].AbsolutePos = 0.0;//Z
		if((error = MotorSetSpeedExe( FixZDrive.ID, (float)-1.0, (float)140.0, (float)1000.0)) != errNone) return error;
		if((error = TestMotorMoveSyncronExe(Mask)) != errNone) return error;
		if(!GetFlagDeviceTest(DevTestALL)){if(DIL1_1 & DIL1_2) return errNone;}
		g_MsgASync.MoveData[0].AbsolutePos = 0.0;//X
		g_MsgASync.MoveData[1].AbsolutePos = F_Home;//F
		g_MsgASync.MoveData[2].AbsolutePos = R_Home;//R
		g_MsgASync.MoveData[3].AbsolutePos = B_Mitte;//B
		g_MsgASync.MoveData[4].AbsolutePos = 0.0;//Z
		if((error = TestMotorMoveSyncronExe(Mask)) != errNone) return error;
		if(!GetFlagDeviceTest(DevTestALL)){if(DIL1_1 & DIL1_2) return errNone;}
		g_MsgASync.MoveData[0].AbsolutePos = 340.0;//X
		g_MsgASync.MoveData[1].AbsolutePos = F_Home;//F
		g_MsgASync.MoveData[2].AbsolutePos = R_Home;//R
		g_MsgASync.MoveData[3].AbsolutePos = B_Mitte;//B
		g_MsgASync.MoveData[4].AbsolutePos = -185.0;//Z
		if((error = MotorSetSpeedExe( FixZDrive.ID, (float)-1.0, (float)300.0, (float)3000.0)) != errNone) return error;
		if((error = TestMotorMoveSyncronExe(Mask)) != errNone) return error;
		if(!GetFlagDeviceTest(DevTestALL)){if(DIL1_1 & DIL1_2) return errNone;}
		g_MsgASync.MoveData[0].AbsolutePos = 170.0;//X
		g_MsgASync.MoveData[1].AbsolutePos = F_Home;//F
		g_MsgASync.MoveData[2].AbsolutePos = R_Home;//R
		g_MsgASync.MoveData[3].AbsolutePos = B_Home;//B
		g_MsgASync.MoveData[4].AbsolutePos = -110.0;//Z
		if((error = MotorSetSpeedExe( FixZDrive.ID, (float)-1.0, (float)140.0, (float)1000.0)) != errNone) return error;
		if((error = TestMotorMoveSyncronExe(Mask)) != errNone) return error;
		if(!GetFlagDeviceTest(DevTestALL)){if(DIL1_1 & DIL1_2) return errNone;}
		g_MsgASync.MoveData[0].AbsolutePos = 0.0;//X
		g_MsgASync.MoveData[1].AbsolutePos = F_Home;//F
		g_MsgASync.MoveData[2].AbsolutePos = R_Home;//R
		g_MsgASync.MoveData[3].AbsolutePos = B_Mitte;//B
		g_MsgASync.MoveData[4].AbsolutePos = -185.0;//Z
		if((error = MotorSetSpeedExe( FixZDrive.ID, (float)-1.0, (float)300.0, (float)3000.0)) != errNone) return error;
		if((error = TestMotorMoveSyncronExe(Mask)) != errNone) return error;
		if(!GetFlagDeviceTest(DevTestALL)){if(DIL1_1 & DIL1_2) return errNone;}
		g_MsgASync.MoveData[0].AbsolutePos = 340.0;//X
		g_MsgASync.MoveData[1].AbsolutePos = F_Home;//F
		g_MsgASync.MoveData[2].AbsolutePos = R_Home;//R
		g_MsgASync.MoveData[3].AbsolutePos = B_Home;//B
		g_MsgASync.MoveData[4].AbsolutePos = 0.0;//Z
		if((error = MotorSetSpeedExe( FixZDrive.ID, (float)-1.0, (float)140.0, (float)1000.0)) != errNone) return error;
		if((error = TestMotorMoveSyncronExe(Mask)) != errNone) return error;
		if(!GetFlagDeviceTest(DevTestALL)){if(DIL1_1 & DIL1_2) return errNone;}

		x_pos = 145.0;
		z_hoch = -85.0;
		z_runter = -100.0;
		g_MsgASync.MoveData[0].AbsolutePos = x_pos;//X
		g_MsgASync.MoveData[1].AbsolutePos = F_Home;//F
		g_MsgASync.MoveData[2].AbsolutePos = R_Home;//R
		g_MsgASync.MoveData[3].AbsolutePos = B_Home;//B
		g_MsgASync.MoveData[4].AbsolutePos = z_hoch;//Z

		for(i=0;i<12;i++)
		{
			g_MsgASync.MoveData[0].AbsolutePos = x_pos + (i*9.0);//X
			if((error = MotorSetSpeedExe( FixZDrive.ID, (float)-1.0, (float)300.0, (float)3000.0)) != errNone) return error;
			if((error = TestMotorMoveSyncronExe(Mask)) != errNone) return error;
			g_MsgASync.MoveData[4].AbsolutePos = z_runter;//Z
			if((error = TestMotorMoveSyncronExe(Mask)) != errNone) return error;
			if(!GetFlagDeviceTest(DevTestALL)){if(DIL1_1 & DIL1_2) return errNone;}
			g_MsgASync.MoveData[0].AbsolutePos = x_pos + (i*9.0);//X
			g_MsgASync.MoveData[4].AbsolutePos = z_hoch;//Z
			if((error = MotorSetSpeedExe( FixZDrive.ID, (float)-1.0, (float)140.0, (float)1000.0)) != errNone) return error;
			if((error = TestMotorMoveSyncronExe(Mask)) != errNone) return error;
			if(!GetFlagDeviceTest(DevTestALL)){if(DIL1_1 & DIL1_2) return errNone;}
		}
		g_MsgASync.MoveData[4].AbsolutePos = 0.0;//Z
		if((error = MotorSetSpeedExe( FixZDrive.ID, (float)-1.0, (float)140.0, (float)1000.0)) != errNone) return error;
		if((error = TestMotorMoveSyncronExe(Mask)) != errNone) return error;

		for(i=0;i<8;i++)
		{
			g_MsgASync.MoveData[0].AbsolutePos = 0.0;//X
			g_MsgASync.MoveData[1].AbsolutePos = F_Home;//F
			g_MsgASync.MoveData[2].AbsolutePos = R_Home;//R
			g_MsgASync.MoveData[3].AbsolutePos = 135.0;//B
			g_MsgASync.MoveData[4].AbsolutePos = 0.0;//Z
			if((error = TestMotorMoveSyncronExe(Mask)) != errNone) return error;
			if(!GetFlagDeviceTest(DevTestALL)){if(DIL1_1 & DIL1_2) return errNone;}
			g_MsgASync.MoveData[0].AbsolutePos = 0.0;//X
			g_MsgASync.MoveData[1].AbsolutePos = F_Mitte;//F
			g_MsgASync.MoveData[2].AbsolutePos = R_Home;//R
			g_MsgASync.MoveData[3].AbsolutePos = B_Home;//B
			g_MsgASync.MoveData[4].AbsolutePos = 0.0;//Z
			if((error = TestMotorMoveSyncronExe(Mask)) != errNone) return error;
			if(!GetFlagDeviceTest(DevTestALL)){if(DIL1_1 & DIL1_2) return errNone;}
			g_MsgASync.MoveData[0].AbsolutePos = 0.0;//X
			g_MsgASync.MoveData[1].AbsolutePos = F_Home;//F
			g_MsgASync.MoveData[2].AbsolutePos = R_Mitte;//R
			g_MsgASync.MoveData[3].AbsolutePos = B_Mitte;//B
			g_MsgASync.MoveData[4].AbsolutePos = 0.0;//Z
			if((error = TestMotorMoveSyncronExe(Mask)) != errNone) return error;
			if(!GetFlagDeviceTest(DevTestALL)){if(DIL1_1 & DIL1_2) return errNone;}
			g_MsgASync.MoveData[0].AbsolutePos = 0.0;//X
			g_MsgASync.MoveData[1].AbsolutePos = F_Mitte;//F
			g_MsgASync.MoveData[2].AbsolutePos = R_Home;//R
			g_MsgASync.MoveData[3].AbsolutePos = B_Home;//B
			g_MsgASync.MoveData[4].AbsolutePos = 0.0;//Z
			if((error = TestMotorMoveSyncronExe(Mask)) != errNone) return error;
			if(!GetFlagDeviceTest(DevTestALL)){if(DIL1_1 & DIL1_2) return errNone;}
			g_MsgASync.MoveData[0].AbsolutePos = 0.0;//X
			g_MsgASync.MoveData[1].AbsolutePos = F_Home;//F
			g_MsgASync.MoveData[2].AbsolutePos = R_Mitte;//R
			g_MsgASync.MoveData[3].AbsolutePos = B_Mitte;//B
			g_MsgASync.MoveData[4].AbsolutePos = 0.0;//Z
			if((error = TestMotorMoveSyncronExe(Mask)) != errNone) return error;
			if(!GetFlagDeviceTest(DevTestALL)){if(DIL1_1 & DIL1_2) return errNone;}
			g_MsgASync.MoveData[0].AbsolutePos = 0.0;//X
			g_MsgASync.MoveData[1].AbsolutePos = F_Home;//F
			g_MsgASync.MoveData[2].AbsolutePos = R_Home;//R
			g_MsgASync.MoveData[3].AbsolutePos = B_Home;//B
			g_MsgASync.MoveData[4].AbsolutePos = 0.0;//Z
			if((error = TestMotorMoveSyncronExe(Mask)) != errNone) return error;
			if(!GetFlagDeviceTest(DevTestALL)){if(DIL1_1 & DIL1_2) return errNone;}
		}
	}
}
*/