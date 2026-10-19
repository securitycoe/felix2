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
#include <math.h>

#include "DevError.h"
//#include "Shared/Stacker_Lifter_Allgemein/StacError.h"
#include "DevCmds.h"
#include "30-5015_Felix/XE167F/main.h"
#include "Cxconfig.h"
//#include "DeviceConfig.h"
#include "Drv_config.h"
//#include "Felix_Hardware.h"
#include "MDrive_CmdCtrl.h"
#include "MDrive_BusCtrl.h"
#include "Felix_CmdCtrl.h"
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

//****************************************************************************
// public data
//****************************************************************************

//****************************************************************************
// private data
//****************************************************************************
IMS_CommandBlock Msg;
//****************************************************************************
// private functions
//****************************************************************************
//unsigned short MotorInitS2(unsigned short ID);


unsigned short ImsCmdInitMotorTaskVariable(const unsigned short i)
{
	unsigned char status;

	Msg.Error = errNone;
	Msg.AxisID = i;
	Msg.eCommand = eImsInitMotorTaskVariable;

	status = K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	return Msg.Error;
}



//****************************************************************************
// public functions
//****************************************************************************

//############################################################################################################
//########### germeinsamme Nutzung ###########################################################################
//############################################################################################################
unsigned short ImsCmd_GetHomeSwitch(const short ID, unsigned short *value)
{

	unsigned char status;

	MOTOR_MBX_ALLOC
	if(GetAxisIndex(ID) < 0)
	{
		MOTOR_MBX_RELEASE
		return ERR_UNKNOWN_DRV;
	}
	Msg.AxisID = ID;
	//Msg.Error = errNone;
	Msg.eCommand = eImsGetHomeSwitch;

	status = K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	if(status != K_OK)
	{
		MOTOR_MBX_RELEASE
		return ERR_SYSTEM;
	}
	*value = Msg.Argument.usShort;
	MOTOR_MBX_RELEASE
	return Msg.Error;
}
unsigned short ImsCmd_GetMotorHomeSwitch(const short ID, unsigned short *value)
{

	unsigned char status;

	Msg.AxisID = ID;
	//Msg.Error = errNone;
	Msg.eCommand = eImsGetMotorHomeSwitch;

	status = K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	*value = Msg.Argument.usShort;
	return Msg.Error;
}

//############################################################################################################
//########### nur FelixCmd ###################################################################################
//############################################################################################################

/*unsigned short CheckMask(unsigned short mask)
{
	if(mask & (eX_Axis | eFrontCarriage_Axis | eRearCarriage_Axis | eBottomCarriage| eZ_Axis))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}*/

unsigned short ConfirmMask(unsigned short mask)
{
		return mask & eAllMask;
}

/*unsigned short ResetMaskBit(unsigned short mask, unsigned short Axis)
{
	return mask & ~Axis;
}*/

/*unsigned short MotorInitS2(unsigned short ID)
{
	//unsigned short error = errNone;

return ImsCmd_SetS2EStopOn(ID);
*	if(g_EnableEstopEvent)
	{
		if((error = ImsCmd_SetS2EStopOn(ID)) != errNone) {return error;}
	}
	else
	{
		//if((error = ImsCmd_SetS2EStopOff(ID)) != errNone) {return error;}
	}
	return error;
	*
}*/

unsigned short MotorInit(unsigned short ID)
{
	unsigned short error;
	unsigned short idx;
	idx = GetAxisIndex(ID);
	if(idx < 0)
	{
		return ERR_UNKNOWN_DRV;
	}
	if((error = ImsCmd_ResetError(ID)) != errNone) return error;
	if((error = ImsCmd_SetS1(ID)) != errNone) {return error;}
	//if((error = MotorInitS2(ID)) != errNone) {return error;}
	if((error = ImsCmd_SetS2EStopOn(ID)) != errNone) {return error;}
//K_Task_Wait(10);
	if((error = ImsCmd_SetESC(ID)) != errNone) {return error;}
	if((error = ImsCmd_SetEncoderMode(ID)) != errNone) {return error;}
//K_Task_Wait(10);
	if((error = ImsCmd_StallDetectON(ID)) != errNone) {return error;}
//K_Task_Wait(10);
	if((error = ImsCmd_SetStallFactor(ID, pIMSChangedPara[idx]->usStallsteps)) != errNone) {return error;}
//K_Task_Wait(10);
	//if((error = ImsCmd_SetEncoderDeadBand(ID, 1)) != errNone) {return error;}
	//oder MotorSetPrecision
	if((error = MotorSetPrecision(ID, pIMSChangedPara[idx]->fPrecisionDeadband)) != errNone) {return error;}
//K_Task_Wait(10);
//	if((error = ImsCmd_SetHoldCurrent(ID, pIMSChangedPara[idx]->usHoldCurrent)) != errNone) {return error;}
	if((error = ImsCmd_SetHoldCurrent(ID, 0)) != errNone) {return error;}
//K_Task_Wait(10);
	if((error = ImsCmd_SetRunCurrent(ID, pIMSChangedPara[idx]->usRunCurrent)) != errNone) {return error;}
//K_Task_Wait(10);
	if((error = ImsCmd_SetMotorWobbleWaitTime(ID, pIMSChangedPara[idx]->usMotorWobbleWaitTime)) != errNone) {return error;}
//K_Task_Wait(10);
	if((error = ImsCmd_SetHoldCurrentWaitTime(ID, pIMSChangedPara[idx]->usHoldCurrentWaitTime)) != errNone) {return error;}
	if((error = ImsCmd_ResetPosition(ID)) != errNone) {return error;}
//K_Task_Wait(10);
	//if((error = MotorSetStartSpeed(ID, pIMSChangedPara[idx]->fStartSpeed)) != errNone) {return error;}
//K_Task_Wait(10);
//normal
	//return MotorSetSpeedNormal(ID);
	if((error = ImsCmd_SetSpeed(ID, pIMSChangedPara[idx]->fStartSpeed, pIMSChangedPara[idx]->fEndSpeed, pIMSChangedPara[idx]->fAcceleration)) != errNone) {return error;}
	error = ImsCmdInitMotorTaskVariable(idx);
	return error;
}
/*unsigned short AllMotorInitS2(void)
{
	unsigned short error = errNone;
	unsigned short ID;
	unsigned short i;

	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if((ID = GetAxisIDFromMask((eX_Axis | eFrontCarriage_Axis | eRearCarriage_Axis | eBottomCarriage | eZ_Axis), g_FelixAxes[i].AxisBit)) != 0)
		{
			if((error = MotorInitS2(ID)) != errNone) {return error;}
		}
	}
	return error;
}*/



// ############################################ SPEED ####################################################################
// ############################################ SPEED ####################################################################

// ############################################# ACCELERATION ###################################################################
// ############################################# ACCELERATION ###################################################################





// ################################################## Strom ##############################################
// ################################################## Strom ##############################################

unsigned short MotorSetHoldCurrent(const short ID, unsigned short value)
{
	unsigned short error;
	unsigned short oldCurrent;
	short idx;
	idx = GetAxisIndex(ID);
	if(idx < 0)
	{
		return ERR_UNKNOWN_DRV;
	}
	//if(!CheckUShort(HC_RANGE_MIN, HC_RANGE_MAX, value)){return ERR_WRONG_PARAMETER;}
	oldCurrent = pIMSChangedPara[idx]->usHoldCurrent; // intern wird  pIMSChangedPara[idx]->usHoldCurrent; verwendet
	pIMSChangedPara[idx]->usHoldCurrent = value;
	error = ImsCmd_SetHoldCurrent( ID, value);
	if(error != errNone)
	{
		pIMSChangedPara[idx]->usHoldCurrent = oldCurrent;
	}
	return error;
}

unsigned short MotorGetHoldCurrent(const short ID, unsigned short *value, unsigned short Scr)
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
		error = ImsCmd_GetHoldCurrent( ID, value);
	}
	else
	{
		*value = pIMSChangedPara[idx]->usHoldCurrent;
	}
	return error;
}
unsigned short MotorSetRunCurrent(const short ID, unsigned short value)
{
	unsigned short error;
	short idx;
	idx = GetAxisIndex(ID);
	if(idx < 0)
	{
		return ERR_UNKNOWN_DRV;
	}
	//if(!CheckUShort(RC_RANGE_MIN, RC_RANGE_MAX, value)){return ERR_WRONG_PARAMETER;}
	error = IMS_SetRunCurrent( ID, value);
	if(error == errNone)
	{
		pIMSChangedPara[idx]->usRunCurrent = value;
	}
	return error;
}

unsigned short MotorGetRunCurrent(const short ID, unsigned short *value, unsigned short Scr)
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
		error = IMS_GetRunCurrent( ID, value);
	}
	else
	{
		*value = pIMSChangedPara[idx]->usRunCurrent;
	}
	return error;
}
// ################################################## Genauigkeit, Stall, Time ##############################################
// ################################################## Genauigkeit, Stall, Time ##############################################
unsigned short MotorSetStallFactor(const short ID, unsigned short value)
{
	unsigned short error;
	short idx;
	idx = GetAxisIndex(ID);
	if(idx < 0)
	{
		return ERR_UNKNOWN_DRV;
	}
	error = ImsCmd_SetStallFactor( ID, value);
	if(error == errNone)
	{
		pIMSChangedPara[idx]->usStallsteps = value;
	}
	return error;
}

unsigned short MotorGetStallFactor(const short ID, unsigned short *value, unsigned short Scr)
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
		error = ImsCmd_GetStallFactor( ID, value);
	}
	else
	{
		*value = pIMSChangedPara[idx]->usStallsteps;
	}
	return error;
}

unsigned short MotorSetEncoderDeadBand(const short ID, unsigned short value)
{
	unsigned short error;
	short idx;
	idx = GetAxisIndex(ID);
	if(idx < 0)
	{
		return ERR_UNKNOWN_DRV;
	}
	error = ImsCmd_SetEncoderDeadBand( ID, value);
	return error;
}

unsigned short MotorGetEncoderDeadBand(const short ID, unsigned short *value)
{
	unsigned short error = errNone;
	short idx;
	idx = GetAxisIndex(ID);
	if(idx < 0)
	{
		return ERR_UNKNOWN_DRV;
	}
	error = ImsCmd_GetEncoderDeadBand( ID, value);
	return error;
}
unsigned short MotorSetPrecision(const short ID, double value)
{
	unsigned short error;
	short idx;
	unsigned short db;
	double db_double;
	idx = GetAxisIndex(ID);
	if(idx < 0)
	{
		return ERR_UNKNOWN_DRV;
	}
	//db = (unsigned short)fabs(pIMSFixedPara[idx]->fENCImpProMM  * value);
	db_double = fabs(pIMSFixedPara[idx]->fENCImpProMM  * value);
	db = RoundToLong(db_double);
	if(db < DB_RANGE_MIN)
	{

		db = 0;//db = DB_RANGE_MIN;
	}
	error = ImsCmd_SetEncoderDeadBand( ID, db);
	if(error == errNone)
	{
		//pIMSChangedPara[idx]->fPrecisionDeadband = value;
		//db = db + 1;// 1 = minimale Präzision	die ein Encoderstep hat, db -> die zulässige Encoderabweichung bei der Positionierung
		pIMSChangedPara[idx]->fPrecisionDeadband = fabs((double)db / pIMSFixedPara[idx]->fENCImpProMM);
	}
	return error;
}

unsigned short MotorGetPrecision(const short ID, double *value)
{
	unsigned short error = errNone;
	short idx;
	unsigned short db;
	idx = GetAxisIndex(ID);
	if(idx < 0)
	{
		return ERR_UNKNOWN_DRV;
	}
	/*if(Scr)
	{
		error = ImsCmd_GetEncoderDeadBand( ID, &db);
		*value = fabs(pIMSFixedPara[idx]->fIgrImpulseProRev / pIMSFixedPara[idx]->fWayProRev);
	}
	else
	{
		*value = pIMSChangedPara[idx]->fPrecisionDeadband;
	}*/

	//*value = pIMSChangedPara[idx]->fPrecisionDeadband;
	error = ImsCmd_GetEncoderDeadBand( ID, &db);
	if(error == errNone)
	{
		//db = db + 1;// 1 = minimale Präzision	die ein Encoderstep hat, db -> die zulässige Encoderabweichung bei der Positionierung
		*value = fabs((double)db / pIMSFixedPara[idx]->fENCImpProMM);
	}

	return error;
}

unsigned short MotorSetHoldCurrentWaitTime(const short ID, unsigned short value)
{
	unsigned short error;
	short idx;
	idx = GetAxisIndex(ID);
	if(idx < 0)
	{
		return ERR_UNKNOWN_DRV;
	}
	error = ImsCmd_SetHoldCurrentWaitTime( ID, value);
	if(error == errNone)
	{
		pIMSChangedPara[idx]->usHoldCurrentWaitTime = value;
	}
	return error;
}

unsigned short MotorGetHoldCurrentWaitTime(const short ID, unsigned short *value, unsigned short Scr)
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
		error = ImsCmd_GetHoldCurrentWaitTime( ID, value);
	}
	else
	{
		*value = pIMSChangedPara[idx]->usHoldCurrentWaitTime;
	}
	return error;
}

unsigned short MotorSetMotorWobbleWaitTime(const short ID, unsigned short value)
{
	unsigned short error;
	short idx;
	idx = GetAxisIndex(ID);
	if(idx < 0)
	{
		return ERR_UNKNOWN_DRV;
	}
	error = ImsCmd_SetMotorWobbleWaitTime( ID, value);
	if(error == errNone)
	{
		pIMSChangedPara[idx]->usMotorWobbleWaitTime = value;
	}
	return error;
}

unsigned short MotorGetMotorWobbleWaitTime(const short ID, unsigned short *value, unsigned short Scr)
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
		error = ImsCmd_GetMotorWobbleWaitTime( ID, value);
	}
	else
	{
		*value = pIMSChangedPara[idx]->usMotorWobbleWaitTime;
	}
	return error;
}
// ################################################## Pos und Flags ##############################################
// ################################################## Pos und Flags ##############################################

//unsigned short MotorGetPosition(const short ID, float *value)
unsigned short MotorGetPosition(const short ID, double *value)
{
	unsigned short error;
	long lvalue;
	short idx;
	idx = GetAxisIndex(ID);
	if(idx < 0)
	{
		return ERR_UNKNOWN_DRV;
	}
	error = ImsCmd_GetPosition( ID, &lvalue);
	if(error == errNone)
	{
		//*value = (float)lvalue / pIMSFixedPara[idx]->fENCImpProMM;
		*value = (double)lvalue / pIMSFixedPara[idx]->fENCImpProMM;
	}
	return error;
}


/*unsigned short MotorGetStallFlag(const short ID, unsigned short *value)
{
	short idx;
	idx = GetAxisIndex(ID);
	if(idx < 0)
	{
		return ERR_UNKNOWN_DRV;
	}
 	return ImsCmd_GetStallFlag( ID, value);
}*/
// ################################################## Move ##############################################
// ################################################## Move ##############################################


unsigned short ImsCmd_ResetError(const short ID)
{

	unsigned char status;

	Msg.AxisID = ID;
	Msg.Error = errNone;
	Msg.eCommand = eImsResetError;

	status = K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	return Msg.Error;
}
unsigned short ImsCmd_MotorResetError(const short ID)
{

	unsigned char status;

	Msg.AxisID = ID;
	Msg.Error = errNone;
	Msg.eCommand = eImsMotorResetError;

	status = K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	return Msg.Error;
}

unsigned short ImsCmd_GetHWError(const short ID, unsigned short *value)
{

	unsigned char status;

	if(GetAxisIndex(ID) < 0) return ERR_UNKNOWN_DRV;

	Msg.AxisID = ID;
	Msg.Error = errNone;
	Msg.eCommand = eImsGetHWError;

	status = K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	*value = Msg.Argument.usShort;
	return Msg.Error;
}
unsigned short ImsCmd_GetOEMError(const short ID, unsigned short *value)
{

	unsigned char status;

//DoorLifter	if(GetAxisIndex(ID) < 0) return ERR_UNKNOWN_DRV;

	Msg.AxisID = ID;
	Msg.Error = errNone;
	Msg.eCommand = eImsGetOEMError;

	status = K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	*value = Msg.Argument.usShort;
	return Msg.Error;
}

unsigned short ImsCmd_SetEncoderMode(const short ID)
{

	unsigned char status;

	Msg.AxisID = ID;
	Msg.Error = errNone;
	Msg.eCommand = eImsSetEncoderMode;

	status = K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	return Msg.Error;
}

unsigned short ImsCmd_SetS1(const short ID)
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
unsigned short ImsCmd_SetS2EStopOn(const short ID)
{

	unsigned char status;

	Msg.AxisID = ID;
	Msg.Error = errNone;
	Msg.eCommand = eImsSetS2_EStopOn;

	status = K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	return Msg.Error;
}
/*unsigned short ImsCmd_SetS2EStopOff(const short ID)
{

	unsigned char status;

	Msg.AxisID = ID;
	Msg.Error = errNone;
	Msg.eCommand = eImsSetS2_EStopOff;

	status = K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	return Msg.Error;
}*/

unsigned short ImsCmd_SetESC(const short ID)
{

	unsigned char status;

	Msg.AxisID = ID;
	Msg.Error = errNone;
	Msg.eCommand = eImsSetESC;

	status = K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	return Msg.Error;
}

unsigned short ImsCmd_SetDecToAcc(const short ID)
{

	unsigned char status;

	Msg.AxisID = ID;
	Msg.Error = errNone;
	Msg.eCommand = eImsSetDecToAcc;

	status = K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	return Msg.Error;
}
unsigned short ImsCmd_StallDetectON(const short ID)
{

	unsigned char status;

	Msg.AxisID = ID;
	Msg.Error = errNone;
	Msg.eCommand = eImsStallDetectON;

	status = K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	return Msg.Error;
}
unsigned short ImsCmd_StallDetectOFF(const short ID)
{

	unsigned char status;

	Msg.AxisID = ID;
	Msg.Error = errNone;
	Msg.eCommand = eImsStallDetectOFF;

	status = K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	return Msg.Error;
}

unsigned short ImsCmd_ResetMotorStallFlag(const short ID)
{

	unsigned char status;

	Msg.AxisID = ID;
	Msg.Error = errNone;
	Msg.eCommand = eImsResetMotorStallFlag;

	status = K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	return Msg.Error;
}

/*unsigned short ImsCmd_ResetStallFlag(const short ID)
{

	unsigned char status;

	Msg.AxisID = ID;
	Msg.Error = errNone;
	Msg.eCommand = eImsResetStallFlag;

	status = K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	return Msg.Error;
}*/

/*unsigned short ImsCmd_GetStallFlag(const short ID, unsigned short *value)
{

	unsigned char status;

	Msg.AxisID = ID;
	Msg.Error = errNone;
	Msg.eCommand = eImsGetStallFlag;

	status = K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	*value = Msg.Argument.usShort;
	return Msg.Error;
}*/

unsigned short ImsCmd_SetStallFactor(const short ID, unsigned short value)
{

	unsigned char status;

	if(!CheckUShort(SF_RANGE_MIN, SF_RANGE_MAX, value)){return ERR_WRONG_PARAMETER;}
	Msg.AxisID = ID;
	Msg.Error = errNone;
	Msg.eCommand = eImsSetStallFactor;
	Msg.Argument.usShort = value;

	status = K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	return Msg.Error;
}

unsigned short ImsCmd_GetStallFactor(const short ID, unsigned short *value)
{

	unsigned char status;

	Msg.AxisID = ID;
	Msg.Error = errNone;
	Msg.eCommand = eImsGetStallFactor;

	status = K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	*value = Msg.Argument.usShort;

	return Msg.Error;
}

unsigned short ImsCmd_SetMotorWobbleWaitTime(const short ID, unsigned short value)
{

	unsigned char status;

	if(!CheckUShort(MT_RANGE_MIN, MT_RANGE_MAX, value)){return ERR_WRONG_PARAMETER;}
	Msg.AxisID = ID;
	Msg.Error = errNone;
	Msg.eCommand = eImsSetMotorWobbleWaitTime;
	Msg.Argument.usShort = value;

	status = K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	return Msg.Error;
}

unsigned short ImsCmd_GetMotorWobbleWaitTime(const short ID, unsigned short *value)
{

	unsigned char status;

	Msg.AxisID = ID;
	Msg.Error = errNone;
	Msg.eCommand = eImsGetMotorWobbleWaitTime;

	status = K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	*value = Msg.Argument.usShort;
	return Msg.Error;
}

unsigned short ImsCmd_SetHoldCurrentWaitTime(const short ID, unsigned short value)
{

	unsigned char status;

	if(!CheckUShort(HT_RANGE_MIN, HT_RANGE_MAX, value)){return ERR_WRONG_PARAMETER;}
	if(value == 1){ value = 2;}//lt. MDIPlus_software-Manual.pdf
	Msg.AxisID = ID;
	Msg.Error = errNone;
	Msg.eCommand = eImsSetHoldCurrentWaitTime;
	Msg.Argument.usShort = value;

	status = K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	return Msg.Error;
}

unsigned short ImsCmd_GetHoldCurrentWaitTime(const short ID, unsigned short *value)
{

	unsigned char status;

	Msg.AxisID = ID;
	Msg.Error = errNone;
	Msg.eCommand = eImsGetHoldCurrentWaitTime;

	status = K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	*value = Msg.Argument.usShort;
	return Msg.Error;
}

unsigned short ImsCmd_SetEncoderDeadBand(const short ID, unsigned short value)
{

	unsigned char status;

	if(!CheckUShort(0, DB_RANGE_MAX, value)){return ERR_WRONG_PARAMETER;}
	Msg.AxisID = ID;
	Msg.Error = errNone;
	Msg.eCommand = eImsSetEncoderDeadBand;
	Msg.Argument.usShort = value;

	status = K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	return Msg.Error;
}

unsigned short ImsCmd_GetEncoderDeadBand(const short ID, unsigned short *value)
{

	unsigned char status;

	Msg.AxisID = ID;
	Msg.Error = errNone;
	Msg.eCommand = eImsGetEncoderDeadBand;

	status = K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	*value = Msg.Argument.usShort;
	return Msg.Error;
}
unsigned short ImsCmd_SetHoldCurrent(const short ID, unsigned short value)
{

	unsigned char status;

	if(!CheckUShort(HC_RANGE_MIN, HC_RANGE_MAX, value)){return ERR_WRONG_PARAMETER;}
	Msg.AxisID = ID;
	Msg.Error = errNone;
	Msg.eCommand = eImsSetHoldCurrent;
	Msg.Argument.usShort = value;

	status = K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	return Msg.Error;
}

unsigned short ImsCmd_Motor_SetHoldCurrent(const short ID, unsigned short value)
{

	unsigned char status;

	if(!CheckUShort(HC_RANGE_MIN, HC_RANGE_MAX, value)){return ERR_WRONG_PARAMETER;}
	Msg.AxisID = ID;
	Msg.Error = errNone;
	Msg.eCommand = eImsSetMotorHoldCurrent;
	Msg.Argument.usShort = value;

	status = K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	return Msg.Error;
}

unsigned short ImsCmd_GetHoldCurrent(const short ID, unsigned short *value)
{

	unsigned char status;

	Msg.AxisID = ID;
	Msg.Error = errNone;
	Msg.eCommand = eImsGetHoldCurrent;

	status = K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	*value = Msg.Argument.usShort;
	return Msg.Error;
}

unsigned short ImsCmd_StromlosJQV(const short ID)
{

	unsigned char status;
	short idx;
	idx = GetAxisIndex(ID);
	if(idx < 0)
	{
		return ERR_UNKNOWN_DRV;
	}

	Msg.AxisID = ID;
	Msg.Error = errNone;
	Msg.eCommand = eImsStromlosJQV;

	status = K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	return Msg.Error;
}

unsigned short ImsCmd_SetRunCurrent(const short ID, unsigned short value)
{

	unsigned char status;

	if(!CheckUShort(RC_RANGE_MIN, RC_RANGE_MAX, value)){return ERR_WRONG_PARAMETER;}
	Msg.AxisID = ID;
	Msg.Error = errNone;
	Msg.eCommand = eImsSetRunCurrent;
	Msg.Argument.usShort = value;

	status = K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	return Msg.Error;
}

unsigned short ImsCmd_GetRunCurrent(const short ID, unsigned short *value)
{

	unsigned char status;

	Msg.AxisID = ID;
	Msg.Error = errNone;
	Msg.eCommand = eImsGetRunCurrent;

	status = K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	*value = Msg.Argument.usShort;
	return Msg.Error;
}

unsigned short ImsCmd_SetCounterMotorSteps(const short ID, long value)
{

	unsigned char status;

	if(!CheckLong(C1_RANGE_MIN, C1_RANGE_MAX, value)){return ERR_WRONG_PARAMETER;}
	Msg.AxisID = ID;
	Msg.eCommand = eImsSetCounterMotorSteps;
	Msg.Argument.lLong = value;

	status = K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	return Msg.Error;
}

unsigned short ImsCmd_GetCounterMotorSteps(const short ID, long *value)
{

	unsigned char status;

	Msg.AxisID = ID;
	Msg.eCommand = eImsGetCounterMotorSteps;

	status = K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	*value = Msg.Argument.lLong;
	return Msg.Error;
}
/*unsigned short ImsCmd_SetCounterEncoder(const short ID, long value)
{

	IMS_CommandBlock *ptr;
	unsigned char status;

	if(!CheckLong(C2_RANGE_MIN, C2_RANGE_MAX, value)){return ERR_WRONG_PARAMETER;}
	Msg.AxisID = ID;
	Msg.eCommand = eImsSetCounterEncoder;
	Msg.Argument.lLong = value;

	status = K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	return Msg.Error;
}*/

unsigned short ImsCmd_GetCounterEncoder(const short ID, long *value)
{

	unsigned char status;

	Msg.AxisID = ID;
	Msg.Error = errNone;
	Msg.eCommand = eImsGetCounterEncoder;

	status = K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	*value = Msg.Argument.lLong;
	return Msg.Error;
}

unsigned short ImsCmd_GetMoveFlag(const short ID, unsigned short *value)
{

	unsigned char status;

	Msg.AxisID = ID;
	Msg.Error = errNone;
	Msg.eCommand = eImsGetMoveFlag;

	status = K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	*value = Msg.Argument.usShort;
	return Msg.Error;
}



unsigned short ImsCmd_HomingSequence(const short ID, unsigned short value)
{

	unsigned char status;

	if(!CheckUShort(HM_RANGE_MIN, HM_RANGE_MAX, value)){return ERR_WRONG_PARAMETER;}
	Msg.AxisID = ID;
	Msg.Error = errNone;
	Msg.eCommand = eImsHomingSequence;
	Msg.Argument.usShort = value;

	status = K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	return Msg.Error;
}

unsigned short ImsCmd_SetSpeed(const short ID, float start, float end, float accel)
{

	unsigned char status;

	Msg.AxisID = ID;
	Msg.Error = errNone;
	Msg.eCommand = eImsSetSpeed;
	Msg.Argument.fFloat = start;
	Msg.ArgumentB.fFloatB = end;
	Msg.ArgumentC.fFloatC = accel;
	status = K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	return Msg.Error;
}
unsigned short ImsCmd_GetSpeed(const short ID, float *start, float *end, float *accel)
{

	unsigned char status;

	Msg.AxisID = ID;
	Msg.Error = errNone;
	Msg.eCommand = eImsGetSpeed;
	status = K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	*start = Msg.Argument.fFloat;
	*end = Msg.ArgumentB.fFloatB;
	*accel = Msg.ArgumentC.fFloatC;
	return Msg.Error;

}

unsigned short ImsCmd_ResetPosition(const short ID)
{

	unsigned char status;

	Msg.AxisID = ID;
	Msg.Error = errNone;
	Msg.eCommand = eImsResetPosition;

	status = K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	return Msg.Error;
}
unsigned short ImsCmd_GetPosition(const short ID, long *value)
{

	unsigned char status;

	Msg.AxisID = ID;
	Msg.Error = errNone;
	Msg.eCommand = eImsGetPosition;

	status = K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	*value = Msg.Argument.lLong;
	return Msg.Error;
}
unsigned short ImsCmd_SetPosition(const short ID, long value)
{

	unsigned char status;

	Msg.AxisID = ID;
	Msg.Error = errNone;
	Msg.eCommand = eImsSetPosition;
	Msg.Argument.lLong = value;

	status = K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	return Msg.Error;
}

unsigned short ImsCmd_Sync(void)
{
	unsigned char status;

	Msg.Error = errNone;
	Msg.eCommand = eSync;

	status = K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	return Msg.Error;
}
/*
unsigned short ImsCmd_EnableEStopRestorePosition(void)
{
	return ImsCmd_SetEStopRestorePosition(1);
}
unsigned short ImsCmd_DisableEStopRestorePosition(void)
{
	return ImsCmd_SetEStopRestorePosition(0);
}

unsigned short ImsCmd_SetEStopRestorePosition(unsigned short value)
{

	unsigned char status;

	Msg.Error = errNone;
	Msg.eCommand = eImsEnableEStopRestorePosition;
	Msg.Argument.usShort = value;

	status = K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	return Msg.Error;
}
*/

unsigned short ImsCmd_SetBreake(void)
{
	Msg.eCommand = eZBreakeFixed;
	K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	return Msg.Error;
}
unsigned short ImsCmd_ClearBreake(void)
{
	Msg.eCommand = eZBreakeFree;
	K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	return Msg.Error;
}


unsigned short ImsCmd_GetEStopSwitch(const short ID, unsigned short *value)
{

	unsigned char status;

	if(GetAxisIndex(ID) < 0) return ERR_UNKNOWN_DRV;

	Msg.AxisID = ID;
	Msg.Error = errNone;
	Msg.eCommand = eImsGetEStopSwitch;

	status = K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	*value = Msg.Argument.usShort;
	return Msg.Error;
}

unsigned short ImsCmd_SlewMoveID(const short ID, long value)
{

	unsigned char status;

	Msg.AxisID = ID;
	Msg.Error = errNone;
	Msg.eCommand = eImsSlewMoveID;
	Msg.Argument.lLong = value;

	status = K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	return Msg.Error;
}
unsigned short ImsCmd_SlewInitAll(const unsigned short Mask)
{

	unsigned char status;

	Msg.AxisID = Mask;
	Msg.Error = errNone;
	Msg.eCommand = eImsSlewStartAll;

	status = K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	return Msg.Error;
}
unsigned short ImsCmd_SlewExitAll(const unsigned short Mask)
{

	unsigned char status;

	Msg.AxisID = Mask;
	Msg.Error = errNone;
	Msg.eCommand = eImsSlewEndAll;

	status = K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	return Msg.Error;
}

	
unsigned short ImsCmd_Hippie_Safety_ManuallyMode(void)
{
	unsigned char status;

#ifdef _CF_DEBUG_PRINT
if(g_TestFlag & TF_DBG_FETRESET){dbPrint("safetypos ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\r");}
#endif

	Msg.Error = errNone;
	Msg.eCommand = eImsHippieSafety;
	Msg.Argument.cChar = AXIS_ADR_LPK_DEF;
//	Msg.AxisID = 2;//Missbraucht für AdresseCyBiCtrl;
	Msg.LpkID = LPK_MANUALLYMODE_INTERN_ID;
	Msg.AdresseCyBiCtrl = 2;
	status = K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	return Msg.Error;
}


unsigned short ImsCmd_Motor_SetSpeed(const short ID, long lStart, long lEnd, unsigned short ulAccel)
{

	unsigned char status;

	Msg.AxisID = ID;
	Msg.Error = errNone;
	Msg.eCommand = eImsSetMotorSpeed;
	Msg.Argument.lLong = lStart;
	Msg.ArgumentB.lLongB = lEnd;
	Msg.ArgumentC.ulLongC = ulAccel;
	status = K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	return Msg.Error;
}
unsigned short ImsCmd_Motor_GetSpeed(const short ID, long *lStart, long *lEnd, unsigned long *ulAccel)
{

	unsigned char status;

	Msg.AxisID = ID;
	Msg.Error = errNone;
	Msg.eCommand = eImsGetMotorSpeed;
	status = K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	*lStart = Msg.Argument.lLong;
	*lEnd = Msg.ArgumentB.lLongB;
	*ulAccel = Msg.ArgumentC.ulLongC;
	return Msg.Error;
}

unsigned short ImsCmd_Motor_ResetPosition(const short ID)
{

	unsigned char status;

	Msg.AxisID = ID;
	Msg.Error = errNone;
	Msg.eCommand = eImsMotorResetPosition;

	status = K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	return Msg.Error;
}
unsigned short ImsCmd_Motor_SetPosition(const short ID, long value)
{

	unsigned char status;

	Msg.AxisID = ID;
	Msg.Error = errNone;
	Msg.eCommand = eImsMotorSetPosition;
	Msg.Argument.lLong = value;

	status = K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	return Msg.Error;
}

unsigned short ImsCmd_EnablePositionMaintenance(const short ID)
{

	unsigned char status;

	Msg.AxisID = ID;
	Msg.Error = errNone;
	Msg.eCommand = eImsEnablePositionMaintenance;

	status = K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	return Msg.Error;
}
unsigned short ImsCmd_DisablePositionMaintenance(const short ID)
{

	unsigned char status;

	Msg.AxisID = ID;
	Msg.Error = errNone;
	Msg.eCommand = eImsDisablePositionMaintenance;

	status = K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	return Msg.Error;
}
unsigned short ImsCmd_GetGetPositionMaintenance(const short ID, unsigned short *value)
{

	unsigned char status;

	Msg.AxisID = ID;
	Msg.Error = errNone;
	Msg.eCommand = eImsGetGetPositionMaintenance;

	status = K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	*value = Msg.Argument.usShort;
	return Msg.Error;
}

unsigned short ImsCmd_MoveAbsolutePos(const short ID, long value)
{

	unsigned char status;

	Msg.AxisID = ID;
	Msg.Error = errNone;
	Msg.eCommand = eImsMoveAbsolutePos;
	Msg.Argument.lLong = value;

	status = K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	return Msg.Error;
}










