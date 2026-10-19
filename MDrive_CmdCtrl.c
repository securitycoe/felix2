//****************************************************************************
// @Module        MDrive RS485 Ctrl
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
#include <limits.h>
//#include "Shared/standard/_doprint.h"

#include "DevCmds.h"
#include "DevError.h"
#include "30-5015_Felix/XE167F/main.h"
#include "30-5015_Felix/XE167F/GPT1.h"
#include "30-5015_Felix/XE167F/U0C1_RS485_COM1.h"
#include "DeviceConfig.h"
#include "MDrive_CmdCtrl.h"
#include "MDrive_BusCtrl.h"
#include "Felix_CmdCtrl.h"

#include "Drv_config.h"
#include "Felix_Misc.h"

//****************************************************************************
// @Macros
//****************************************************************************

//****************************************************************************
// local definitions
//****************************************************************************

// !!!!!!!! SL slew für shake ?????
// !!!!!!!! PM  (Position maintenance enable)
#define imsXXTest "XX"

const char imsSetEncoderMode[]= "EE=1";//Encoder ein
const char imsSetS1[]= "S1=1,1,0";		//Ausgabe S1 Homeposition
const char imsSetS1Invers[]= "S1=1,0,0";		//Ausgabe S1 Homeposition, für inverses Homing

const char imsSetS2ESTOPON[]= "S2=5,0,1";	//Ausgabe S2 Softstop, Alte Geräte FW kleiner 0.98 noch "S2=5,0,1"
//const char imsSetS2ESTOPOFF[]= "S2=0,0,0";		//Ausgabe S2 Softstop

const char imsSetESC_1[] = "ES=1";	//ES=1 default
const char imsSetESC_3[] = "ES=3";	//ES=1 default, alle Motor senden '#' bei esc, 
const char imsResetError[]= "ER 0";
//const char imsGetError[]= "PR ER";
const char imsResetStallFlag[]= "ST=0";
const char imsGetStallFlag[]= "PR ST";

const char imsSetStallFactor[]= "SF=";
const char imsGetStallFactor[]= "PR SF";

const char imsStallDetectON[]= "SM=0";
const char imsStallDetectOFF[]= "SM=1";

const char imsSetMotorWobbleWaitTime[]= "MT=";
const char imsGetMotorWobbleWaitTime[]= "PR MT";

const char imsSetHoldCurrentWaitTime[]= "HT=";
const char imsGetHoldCurrentWaitTime[]= "PR HT";

const char imsSetEncoderDeadBand[]= "DB=";
const char imsGetEncoderDeadBand[]= "PR DB";

const char imsSetCounterMotorSteps[]= "C1=";
const char imsGetCounterMotorSteps[]= "PR C1";

const char imsSetCounterEncoder[]= "C3=";
const char imsGetCounterEncoder[]= "PR C2";

const char imsSetHoldCurrent[]= "HC=";
const char imsGetHoldCurrent[]= "PR HC";
const char imsSetRunCurrent[]= "RC=";
const char imsGetRunCurrent[]= "PR RC";

const char imsSetDecToAcc[]= "D=A";		//Bremsrampe = Beschleunigung
const char imsSetStartSpeed[]= "VI=";	//Anfangsdrehzahl setzen
const char imsGetStartSpeed[]= "PR VI";

const char imsSetEndSpeed[]= "VM=";		//Enddrehzahl setzen
const char imsGetEndSpeed[]= "PR VM";		

const char imsSetAcceleration[]= "A=";			//Beschleunigung setzen
const char imsGetAcceleration[]= "PR A";

const char imsSetDeceleration[]= "D=";			//Verzögerung setzen
const char imsGetDeceleration[]= "PR D";



const char imsGetHomeSwitch[]= "PR I1";

const char imsGetEStopSwitch[]= "PR I2";

const char imsHomingSequence[]= "HM";

const char imsGetPosition[]= "PR P";
const char imsSetPosition[]= "P=";

//const char imsResetPosition[]= "P=0";	// Positionszähler auf 0 setzen (nach home positioning)

const char imsMoveAbsolutePos[]= "MA";		//Absolute Position anfahren
const char imsMoveRelativePos[]= "MR";		//Absolute Position anfahren

const char imsStopMotor[] = "SL 0";
const char imsMoveSlew[] = "SL";		

const char imsEnablePositionMaintenance[]= "PM=1";
const char imsDisablePositionMaintenance[]= "PM=0";
const char imsGetPositionMaintenance[]= "PR PM";





const char lpkGetSensor1[]= "gSensor 1";		

//****************************************************************************
// default settings
//****************************************************************************

//****************************************************************************
// external functions
//****************************************************************************
extern int _doprint( FILE *fp, const char *fmt, va_list ap );
extern unsigned short newBus_LPK_ReceivedResponse(const short ID);
//****************************************************************************
// external data
//****************************************************************************
extern const char imsGetError[];
extern const char imsGetMoveFlag[];
extern char m_IMSResponse[];
//****************************************************************************
// internal functions
//****************************************************************************

//****************************************************************************
// public data
//****************************************************************************

//****************************************************************************
// private data
//****************************************************************************

//unsigned short TestRepCmd = 0;
//****************************************************************************
// private functions
//****************************************************************************



//unsigned short IMS_SyncNoAlloc(void);
//unsigned short IMS_LowLewelTx(const short ID, const char *format, ...);
unsigned short IMS_LL_BufferReset(void);
//unsigned short IMS_LL_BufferResetNoAlloc(void);



//void SetErrorAxisHW(const short ID, const unsigned short error);
//void SetErrorAxisCyBio(const short ID, const unsigned short error);



//unsigned short IMS_LPK_LowLewelSend(const short ID, const char *format, ...);
//unsigned short IMS_LPK_LowLewelSendExe(const short ID, char *vaBuffer);

//unsigned short IMS_LPK_LowLewelReceive(const short ID);
//unsigned short IMS_LPK_LowLewelReceiveExe(const short ID);
unsigned short cmdLPKCheckError(unsigned short error);
unsigned short cmdLPKCheckErrorEstopContinue(unsigned short error, char *pszString);
//****************************************************************************
// public functions
//****************************************************************************
unsigned short IMS_Sync(void)
{
	return IMS_LL_BufferReset();
}
/*unsigned short IMS_SyncNoAlloc(void)
{
	return IMS_LL_BufferResetNoAlloc();
}*/

/*unsigned short IMS_TestCmd(const short ID)
{
	return Bus_Command_TxRx(ID, "XX");
}*/
//unsigned short IMS_OEM(const short ID, char *pszString)
unsigned short IMS_OEM(char *pszString)
{
	unsigned short error;
	error = Bus_Command_TxRx(OEM_CMD_DIRECT_ID, 0, "%s", pszString);
	if(error == errNone)
	{
		if(m_IMSResponse[0] != 0)
		{
			strcpy(pszString, m_IMSResponse);
			return errInfo;
		}
	}
	return error;
}

unsigned short IMS_DoorLifterOEM(char *pszString)
{
	unsigned short error;
	error = Bus_Command_TxRx(DOORLIFT_ID, 0, "%s", pszString);
	if(error == errNone)
	{
		if(m_IMSResponse[0] != 0)
		{
			strcpy(pszString, m_IMSResponse);
			return errInfo;
		}
	}
	return error;
}

//##############################################
unsigned short cmdLPKCheckError(unsigned short error)
{
	char * Response = m_IMSResponse;

	if(error == errNone)
	{
		if(m_IMSResponse[0] != 0)
		{
			if(strstr(m_IMSResponse, "E10"))
			{
				error = errNone;
			}
			else
			{
				if(strcmp(m_IMSResponse, "E65535") == 0)
				{
					error = (unsigned short)ERR_SAFETY_APPLIANCE;
				}
				else
				{
					if(strchr(m_IMSResponse, 'E'))
					{
						//08-2024 error = ERR_HEAD_LPK;
						++Response;
						error = atol(Response);
					}
				}
			}
		}
	}
	else
	{
		if(error == LPK_BUS_ERR_HEAD_NOT_CONNECTED){error = errNone;}
	}
	return error;
}

unsigned short cmdLPKCheckErrorEstopContinue(unsigned short error, char *pszString)
{
	char * Response = m_IMSResponse;

	if(error == errNone)
	{
		if(m_IMSResponse[0] != 0)
		{
			if(strstr(m_IMSResponse, "E10"))
			{
				error = errNone;
			}
			else
			{
				if(strcmp(m_IMSResponse, "E65535") == 0)
				{
					error = (unsigned short)ERR_SAFETY_APPLIANCE;
				}
				else
				{
					if(strchr(m_IMSResponse, 'E'))
					{
						++Response;
						error = atol(Response);
					}
					else
					{
						if(strchr(m_IMSResponse, RDY))
						{
							error = errNone;
						}
						else
						{
							strcpy(pszString, m_IMSResponse);
							error = errInfo;
							
						}
					}
				}
			}
		}
	}
	else
	{
		if(error == LPK_BUS_ERR_HEAD_NOT_CONNECTED){error = errNone;}
	}
	return error;
}
//################################################################

unsigned short IMS_cmdLPKSafety(const short ID, const char Adress, const unsigned short AdresseCyBiCtrl)
{
	unsigned short error;

	if(!isLPKConnect())
	{
		return errNone;
	}
//	error = Bus_Command_TxRx(LPKINTERN_ID, AdresseCyBiCtrl, "%c %s", Adress, _SAFETYPOS);
	error = Bus_Command_TxRx(ID, AdresseCyBiCtrl, "%c %s", Adress, _SAFETYPOS);
	return cmdLPKCheckError(error);
}
unsigned short IMS_cmdLPKRestore(const short ID, const char Adress, const unsigned short AdresseCyBiCtrl)
{
	unsigned short error;
	if(!isLPKConnect())
	{
		return errNone;
	}
//	error = Bus_Command_TxRx(LPKINTERN_ID, AdresseCyBiCtrl, "%c %s", Adress, _RESTOREPOS);
	error = Bus_Command_TxRx(ID, AdresseCyBiCtrl, "%c %s", Adress, _RESTOREPOS);
	return cmdLPKCheckError(error);
}
unsigned short IMS_cmdLPKContinue(const short ID, const char Adress, const unsigned short AdresseCyBiCtrl, char *pszString)
{
	unsigned short error;
	if(!isLPKConnect())
	{
		return errNone;
	}
//	error = Bus_Command_TxRx(LPKINTERN_ID, AdresseCyBiCtrl, "%c %s", Adress, _CONTINUEPOS);
	error = Bus_Command_TxRx(ID, AdresseCyBiCtrl, "%c %s", Adress, _CONTINUEPOS);
	return cmdLPKCheckErrorEstopContinue(error, pszString);
//08-2024	return cmdLPKCheckError(error);
}

unsigned short IMS_LPK(const unsigned short AdresseCyBiCtrl, char *pszString)
{
	unsigned short error;
	error = Bus_Command_TxRx(LPK_ID , AdresseCyBiCtrl, "%s", pszString);
	if(error == errNone)
	{
		if(m_IMSResponse[0] != 0)
		{
			if(strchr(m_IMSResponse, RDY))
			{
				return errNone;
			}
			strcpy(pszString, m_IMSResponse);
			return errInfo;
		}
	}
	return error;
}

unsigned short IMS_LPKSendParallelCmd(const unsigned short AdresseCyBiCtrl, char *pszString)
{
	unsigned short error;
	error = Bus_Command_TxRx(LPK_PARALLEL_CMD_ID, AdresseCyBiCtrl , "%s", pszString);
	return error;
}

unsigned short IMS_LPKReceiveParallelCmd( char *pszString)
{
	unsigned short error;
//	error = IMS_LPK_LowLewelReceive(LPK_ID);
	error = newBus_LPK_ReceivedResponse(LPK_PARALLEL_CMD_ID);
	if(error == errNone)
	{
		if(m_IMSResponse[0] != 0)
		{
			if(strchr(m_IMSResponse, RDY))
			{
				return errNone;
			}
			strcpy(pszString, m_IMSResponse);
			return errInfo;
		}
	}
	return error;
}

unsigned short IMS_cmdGetSensorMagazinInsert(const short ID, const char Adress, const unsigned short AdresseCyBiCtrl, unsigned short *value)
{
	unsigned short error;

//	error = Bus_Command_TxRx(LPKINTERN_ID, AdresseCyBiCtrl, "%c %s", Adress, _CONTINUEPOS);
	error = Bus_Command_TxRx(ID, AdresseCyBiCtrl, "%c %s", Adress, lpkGetSensor1);
	if(error == errNone)
	{
		error = ERR_HEAD_LPK;
		if(m_IMSResponse[0] != 0)
		{
			if(strstr(m_IMSResponse, "I0"))
			{
				*value = 0;
				error = errNone;
			}
			else
			{
				if(strstr(m_IMSResponse, "I0"))
				{
					*value = 1;
					error = errNone;
				}
			}
		}
	}
	return error;
}

unsigned short IMS_GetError(const short ID, unsigned short *value)
{
	unsigned short error;
	error = Bus_Command_TxRx(ID, 0, "%s", imsGetError);
	if(error == errNone)
	{
		*value = atoi(m_IMSResponse);
	}
	return error;
}
unsigned short IMS_ResetError(const short ID)
{
	return Bus_Command_TxRx(ID, 0, "%s", imsResetError);
}

unsigned short IMS_SetEncoderMode(const short ID)
{
	return Bus_Command_TxRx(ID, 0, "%s", imsSetEncoderMode);
}
unsigned short IMS_SetS1(const short ID)
{
	return Bus_Command_TxRx(ID, 0, "%s", imsSetS1);
}
unsigned short IMS_SetS1Invers(const short ID)
{
	return Bus_Command_TxRx(ID, 0, "%s", imsSetS1Invers);
}

unsigned short IMS_SetS2_EStopOn(const short ID)
{
	return Bus_Command_TxRx(ID, 0, "%s", imsSetS2ESTOPON);
}
/*unsigned short IMS_SetS2_EStopOff(const short ID)
{
	return Bus_Command_TxRx(ID, 0, "%s", imsSetS2ESTOPOFF);
}*/

unsigned short IMS_SetESC(const short ID)
{
	unsigned short error;
	if(ID == DOORLIFT_ID)
	{
		error =  Bus_Command_TxRx(ID, 0, "%s", imsSetESC_1);
	}
	else
	{
		error =  Bus_Command_TxRx(ID, 0, "%s", imsSetESC_3);
	}
	return error;
}

unsigned short IMS_StallDetectON(const short ID)
{
	return Bus_Command_TxRx(ID, 0, "%s", imsStallDetectON);
}
unsigned short IMS_StallDetectOFF(const short ID)
{
	return Bus_Command_TxRx(ID, 0, "%s", imsStallDetectOFF);
}


unsigned short IMS_GetStallFlag(const short ID, unsigned short *value)
{
	unsigned short error;
	error = Bus_Command_TxRx(ID, 0, "%s", imsGetStallFlag);
	if((error == errNone)||(error == ERR_OEM_HARDWARE))
	{
		*value = atoi(m_IMSResponse);
	}
	return error;
}
unsigned short IMS_ResetStallFlag(const short ID)
{
	return Bus_Command_TxRx(ID, 0, "%s", imsResetStallFlag);
}

unsigned short IMS_GetStallFactor(const short ID, unsigned short *value)
{
	unsigned short error;
	error = Bus_Command_TxRx(ID, 0, "%s", imsGetStallFactor);
	if(error == errNone)
	{
		*value = atoi(m_IMSResponse);
	}
	return error;
}
unsigned short IMS_SetStallFactor(const short ID, unsigned short value)
{
	return Bus_Command_TxRx(ID, 0, "%s%u", imsSetStallFactor, value);
}

unsigned short IMS_GetMotorWobbleWaitTime(const short ID, unsigned short *value)
{
	unsigned short error;
	error = Bus_Command_TxRx(ID, 0, "%s", imsGetMotorWobbleWaitTime);
	if(error == errNone)
	{
		*value = atoi(m_IMSResponse);
	}
	return error;
}
unsigned short IMS_SetMotorWobbleWaitTime(const short ID, unsigned short value)
{
	return Bus_Command_TxRx(ID, 0, "%s%u", imsSetMotorWobbleWaitTime, value);
}

unsigned short IMS_GetHoldCurrentWaitTime(const short ID, unsigned short *value)
{
	unsigned short error;
	error = Bus_Command_TxRx(ID, 0, "%s", imsGetHoldCurrentWaitTime);
	if(error == errNone)
	{
		*value = atoi(m_IMSResponse);
	}
	return error;
}
unsigned short IMS_SetHoldCurrentWaitTime(const short ID, unsigned short value)
{
	return Bus_Command_TxRx(ID, 0, "%s%u", imsSetHoldCurrentWaitTime, value);
}
unsigned short IMS_GetEncoderDeadBand(const short ID, unsigned short *value)
{
	unsigned short error;
	error = Bus_Command_TxRx(ID, 0, "%s", imsGetEncoderDeadBand);
	if(error == errNone)
	{
		*value = atoi(m_IMSResponse);
	}
	return error;
}
unsigned short IMS_SetEncoderDeadBand(const short ID, unsigned short value)
{
	return Bus_Command_TxRx(ID, 0, "%s%u", imsSetEncoderDeadBand, value);
}

unsigned short IMS_GetHoldCurrent(const short ID, unsigned short *value)
{
	unsigned short error;
	error = Bus_Command_TxRx(ID, 0, "%s", imsGetHoldCurrent);
	if(error == errNone)
	{
		*value = atoi(m_IMSResponse);
	}
	return error;
}
unsigned short IMS_SetHoldCurrent(const short ID, unsigned short value)
{
	return Bus_Command_TxRx(ID, 0, "%s%u", imsSetHoldCurrent, value);
}

unsigned short IMS_GetRunCurrent(const short ID, unsigned short *value)
{
	unsigned short error;
	error = Bus_Command_TxRx(ID, 0, "%s", imsGetRunCurrent);
	if(error == errNone)
	{
		*value = atoi(m_IMSResponse);
	}
	return error;
}
unsigned short IMS_SetRunCurrent(const short ID, unsigned short value)
{
	return Bus_Command_TxRx(ID, 0, "%s%u", imsSetRunCurrent, value);
}

unsigned short IMS_GetCounterMotorSteps(const short ID, long *value)
{
	unsigned short error;
	error = Bus_Command_TxRx(ID, 0, "%s", imsGetCounterMotorSteps);
	if(error == errNone)
	{
		*value = atol(m_IMSResponse);
	}
	return error;
}
unsigned short IMS_SetCounterMotorSteps(const short ID, long value)
{
	return Bus_Command_TxRx(ID, 0, "%s %ld", imsSetCounterMotorSteps, value);
}


unsigned short IMS_GetCounterEncoder(const short ID, long *value)
{
	unsigned short error;
	error = Bus_Command_TxRx(ID, 0, "%s", imsGetCounterEncoder);
	if(error == errNone)
	{
		*value = atol(m_IMSResponse);
	}
	return error;
}
unsigned short IMS_SetCounterEncoder(const short ID, long value)
{
	unsigned short error;
	error = Bus_Command_TxRx(ID, 0, "%s%ld", imsSetCounterEncoder, value);
	return error;
}
unsigned short IMS_SetDecToAcc(const short ID)
{
	return Bus_Command_TxRx(ID, 0, "%s", imsSetDecToAcc);
}


unsigned short IMS_GetStartSpeed(const short ID, long *value)
{
	unsigned short error;
	error = Bus_Command_TxRx(ID, 0, "%s", imsGetStartSpeed);
	if(error == errNone)
	{
		*value = atol(m_IMSResponse);
	}
	return error;
}
unsigned short IMS_SetStartSpeed(const short ID, long value)
{
	return Bus_Command_TxRx(ID, 0, "%s%ld", imsSetStartSpeed, value);
}


unsigned short IMS_GetEndSpeed(const short ID, long *value)
{
	unsigned short error;
	error = Bus_Command_TxRx(ID, 0, "%s", imsGetEndSpeed);
	if(error == errNone)
	{
		*value = atol(m_IMSResponse);
	}
	return error;
}
unsigned short IMS_SetEndSpeed(const short ID, long value)
{
	return Bus_Command_TxRx(ID, 0, "%s%ld", imsSetEndSpeed, value);
}

unsigned short IMS_GetAcceleration(const short ID, unsigned long *value)
{
	unsigned short error;
	error = Bus_Command_TxRx(ID, 0, "%s", imsGetAcceleration);
	if(error == errNone)
	{
		*value = atoi(m_IMSResponse);
	}
	return error;
}
unsigned short IMS_SetAcceleration(const short ID, unsigned long value)
{
	return Bus_Command_TxRx(ID, 0, "%s%lu", imsSetAcceleration, value);
}

unsigned short IMS_GetDeceleration(const short ID, unsigned long *value)
{
	unsigned short error;
	error = Bus_Command_TxRx(ID, 0, "%s", imsGetDeceleration);
	if(error == errNone)
	{
		*value = atoi(m_IMSResponse);
	}
	return error;
}
unsigned short IMS_SetDeceleration(const short ID, unsigned long value)
{
	return Bus_Command_TxRx(ID, 0, "%s%lu", imsSetDeceleration, value);
}

unsigned short IMS_SetSpeed(const short ID, float start, float end, float accel)
{
	unsigned short error;
	short idx;
	long lValue;
	unsigned long ulValue;

	idx = GetAxisIndex(ID);
	if(idx < 0)
	{
		return ERR_UNKNOWN_DRV;
	}
	//Startspeed
	if(start >= (float)0.0)
	{
		if(!CheckFloat(pIMSFixedPara[idx]-> fMinSpeed, pIMSFixedPara[idx]-> fMaxSpeed, start)){return ERR_WRONG_PARAMETER;}
		if(start > pIMSChangedPara[idx]->fEndSpeed){return ERR_WRONG_PARAMETER;}
		lValue = (long)(start * pIMSFixedPara[idx]->fMotImpProMM);
		if(!CheckLong(VI_RANGE_MIN, VI_RANGE_MAX, lValue)){return ERR_WRONG_PARAMETER;}
		error = IMS_SetStartSpeed(ID, lValue);
		if(error != errNone) {return error;}

	}
	//Endspeed
	if(!CheckFloat(pIMSFixedPara[idx]-> fMinSpeed, pIMSFixedPara[idx]-> fMaxSpeed, end)){return ERR_WRONG_PARAMETER;}
	if(end < pIMSChangedPara[idx]->fStartSpeed){return ERR_WRONG_PARAMETER;}
	lValue = (long)(end * pIMSFixedPara[idx]->fMotImpProMM);
	if(!CheckLong(VM_RANGE_MIN, VM_RANGE_MAX, lValue)){return ERR_WRONG_PARAMETER;}
	error = IMS_SetEndSpeed(ID, lValue);
	if(error != errNone) {return error;}
	//Beschleunigung
	if(!CheckFloat(pIMSFixedPara[idx]-> fMinAcceleration, pIMSFixedPara[idx]-> fMaxAcceleration, accel)){return ERR_WRONG_PARAMETER;}
	ulValue = (unsigned long)(accel * pIMSFixedPara[idx]->fMotImpProMM);
	if(!CheckULong(A_RANGE_MIN, A_RANGE_MAX, ulValue)){return ERR_WRONG_PARAMETER;}
	error = IMS_SetAcceleration( ID, ulValue);
	if(error != errNone) {return error;}
	if(error == errNone)
	{
		error = IMS_SetDeceleration( ID, (unsigned long)(accel * pIMSFixedPara[idx]->fMotImpProMM));
	}
	return error;
}

unsigned short IMS_SetSpeed_NoCalc(const short ID, long lStart, long lEnd, unsigned long ulAccel)
{
	unsigned short error;

	/*if(GetAxisIndex(ID) < 0)
	{
		return ERR_UNKNOWN_DRV;
	}*/
	//Startspeed
	if(lStart >= (long)0)
	{
		error = IMS_SetStartSpeed(ID, lStart);
		if(error != errNone) {return error;}
	}
	//Endspeed
	error = IMS_SetEndSpeed(ID, lEnd);
	if(error != errNone) {return error;}
	//Beschleunigung
	error = IMS_SetAcceleration( ID, ulAccel);
	if(error == errNone)
	{
		error = IMS_SetDeceleration( ID, ulAccel);
	}
	return error;
}

unsigned short IMS_GetSpeed_NoCalc(const short ID, long *lStart, long *lEnd, unsigned long *lAccel)
{
	unsigned short error;

	/*if(GetAxisIndex(ID) < 0)
	{
		return ERR_UNKNOWN_DRV;
	}*/
	//Startspeed
	error = IMS_GetStartSpeed(ID, lStart);
	if(error == errNone)
	{
		//Endspeed
		error = IMS_GetEndSpeed(ID, lEnd);
		if(error == errNone)
		{
			//Beschleunigung
			error = IMS_GetAcceleration(ID, lAccel);
		}
	}
	return error;
}

unsigned short IMS_GetSpeed(const short ID, float *start, float *end, float *accel)
{
	unsigned short error;
	short idx;
	long lValue;
	unsigned long ulValue;

	idx = GetAxisIndex(ID);
	if(idx < 0)
	{
		return ERR_UNKNOWN_DRV;
	}
	//Startspeed
	error = IMS_GetStartSpeed(ID, &lValue);
	if(error != errNone) {return error;}
	*start = (float)lValue / pIMSFixedPara[idx]->fMotImpProMM;

	//Endspeed
	error = IMS_GetEndSpeed(ID, &lValue);
	if(error != errNone) {return error;}
	*end = (float)lValue / pIMSFixedPara[idx]->fMotImpProMM;

	//Beschleunigung
	error = IMS_GetAcceleration(ID, &ulValue);
	if(error != errNone) {return error;}
	*accel = (float)ulValue / pIMSFixedPara[idx]->fMotImpProMM;

	return errNone;
}

unsigned short IMS_GetMoveFlag(const short ID, unsigned short *value)
{
	unsigned short error;
	error = Bus_Command_TxRx(ID, 0, "%s", imsGetMoveFlag);
	if((error == errNone)||(error == ERR_OEM_HARDWARE))
	{
		*value = atoi(m_IMSResponse);
	}
	return error;
}


unsigned short IMS_GetHomeSwitch(const short ID, unsigned short *value)
{
	unsigned short error;

	if((error = IMS_SetS1(ID)) != errNone){return error;}

	error = Bus_Command_TxRx(ID, 0, "%s", imsGetHomeSwitch);
	if(error == errNone)
	{
		*value = atoi(m_IMSResponse);
	}
	return error;
}


unsigned short IMS_GetMotorHomeSwitch(const short ID, unsigned short *value)
{
	unsigned short error;

	error = Bus_Command_TxRx(ID, 0, "%s", imsGetHomeSwitch);
	if(error == errNone)
	{
		*value = atoi(m_IMSResponse);
	}
	return error;
}

unsigned short IMS_GetEStopSwitch(const short ID, unsigned short *value)
{
	unsigned short error;
	error = Bus_Command_TxRx(ID, 0, "%s", imsGetEStopSwitch);
	if(error == errNone)
	{
		*value = atoi(m_IMSResponse);
	}
	return error;
}

unsigned short IMS_HomingSequence(const short ID, unsigned short value)
{
	unsigned short error;
	error = Bus_Command_TxRx(ID, 0, "%s %u", imsHomingSequence, value);
	return error;
}

unsigned short IMS_GetPosition(const short ID, long *value)
{
	unsigned short error;
	error = Bus_Command_TxRx(ID, 0, "%s", imsGetPosition);
	if(error == errNone)
	{
		*value = atol(m_IMSResponse);
	}
	return error;
}
unsigned short IMS_SetPosition(const short ID, long value)
{

	return Bus_Command_TxRx(ID, 0, "%s%ld", imsSetPosition, value);
}

/*unsigned short IMS_ResetPosition(const short ID)
{
	return Bus_Command_TxRx(ID, 0, "%s", imsResetPosition);
}*/

unsigned short IMS_MoveAbsolutePos(const short ID, long value)
{
	return Bus_Command_TxRx(ID, 0, "%s %ld", imsMoveAbsolutePos, value);
}


/*unsigned short IMS_StopMotorID(const short ID)
{
	return Bus_Command_TxRx(ID, 0, "%s", imsStopMotor);
}*/
unsigned short IMS_MoveHandTest(const short ID, long value)
{
	return Bus_Command_TxRx(ID, 0, "%s %ld", imsMoveAbsolutePos, value);
}

unsigned short IMS_SlewMove(const short ID, long value)
{
	return Bus_Command_TxRx(ID, 0, "%s %ld", imsMoveSlew, value);
}

/*unsigned short IMS_MoveRelativePos(const short ID, long value)
{
	return Bus_Command_TxRx(ID, 0, "%s %ld", imsMoveRelativePos, value);
}*/

unsigned short IMS_StopMotor(const short ID)
{
	#ifdef _CF_485_DEBUG_PRINT
	if(g_TestFlag & TF_DBG_485){dbPrint("[SL 0]");}
	#endif
	K_Task_Wait(10);
PRINT_ALLOC
	if(ID == DOORLIFT_ID)
	{
		fprintf(COM5_SLAVE_U1C0_OUT, "* SL 0\n");
		fflush(COM5_SLAVE_U1C0_OUT);
	}
	else
	{
		fprintf(COM1_RS485_U0C1_OUT, "* SL 0\n");
		fflush(COM1_RS485_U0C1_OUT);
	}
	K_Task_Wait(10);
PRINT_RELEASE
	return ERR_PROG_ABORT;
}

unsigned short IMS_ESC_StopMotor(void)
{
	IMS_StopMotor(FELIX_BASE_ID);
	#ifdef _CF_485_DEBUG_PRINT
	if(g_TestFlag & TF_DBG_485){dbPrint("[esc]");}
	#endif
	K_Task_Wait(10);
PRINT_ALLOC
//	fprintf(COM1_RS485_U0C1_OUT, "* SL 0\n");
//	fflush(COM1_RS485_U0C1_OUT);
//	K_Task_Wait(10);
	fputc('\033',COM1_RS485_U0C1_OUT);
	fflush(COM1_RS485_U0C1_OUT);
	K_Task_Wait(10);
  	K_Task_Wait(30);
	fputc('\n',COM1_RS485_U0C1_OUT);
	K_Task_Wait(30);
PRINT_RELEASE
	return ERR_PROG_ABORT;
}


unsigned short IMS_EnablePositionMaintenance(const short ID)
{
	return Bus_Command_TxRx(ID, 0, "%s", imsEnablePositionMaintenance);
}
unsigned short IMS_DisablePositionMaintenance(const short ID)
{
	return Bus_Command_TxRx(ID, 0, "%s", imsDisablePositionMaintenance);
}
unsigned short IMS_GetPositionMaintenance(const short ID, unsigned short *value)
{
	unsigned short error;
	error = Bus_Command_TxRx(ID, 0, "%s", imsGetPositionMaintenance);
	if(error == errNone)
	{
		*value = atoi(m_IMSResponse);
	}
	return error;
}

//****************************************************************************
// private functions
//****************************************************************************
unsigned short IMS_LL_BufferReset(void)
{
//	EnableOutRS485_BUS();
  	K_Task_Wait(10);
PRINT_ALLOC
	fputc('\n',COM1_RS485_U0C1_OUT);
	K_Task_Wait(10);
PRINT_RELEASE
//	DisableOutRS485_BUS();
	return errNone;
}

/*unsigned short IMS_LL_BufferResetNoAlloc(void)
{
//	EnableOutRS485_BUS();
  	K_Task_Wait(10);
	fputc('\n',COM1_RS485_U0C1_OUT);
	K_Task_Wait(10);
//	DisableOutRS485_BUS();
	return errNone;
}*/




/*
void SetErrorAxisCyBio(const short ID, const unsigned short error)
{
	unsigned short i;
	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if(ID == g_FelixAxes[i].AxisID)
		{
			#ifdef _CF_DEBUG_PRINT
			if(g_TestFlag & TF_DBG_485){dbPrint("SAC i:%u ID:%u E:%u\r", i, ID, error );}
			#endif
			g_SetAxes_ErrorAxisCyBio(i, error);
			g_SetAxes_ErrorFlag(i, 1);
			g_SetAxes_Running(i, STATE_ERROR);
		}
	}
}
*/



