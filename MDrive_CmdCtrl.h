//****************************************************************************
// @Module        Schrittmotor-Controller
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
#ifndef _MDRIVE_CMDCTRL_H_
#define _MDRIVE_CMDCTRL_H_

//****************************************************************************
// include files
//****************************************************************************
#include "Drv_config.h"
//****************************************************************************
// @Macros
//****************************************************************************

//****************************************************************************
// @Defines
//****************************************************************************


//****************************************************************************
// @Typedefs
//****************************************************************************
typedef enum
//enum udtIMS_CMD_IDENT
{
	eImsNone
//,eImsXX

	,eImsMotorResetError
	,eImsResetError
	,eImsGetHWError
	,eImsOEM
	,eImsGetOEMError
	,eImsSetEncoderMode
	,eImsSetS1
	,eImsSetS1Invers
	,eImsSetS2_EStopOn
//	,eImsSetS2_EStopOff
	,eImsSetESC

	,eImsStallDetectON
	,eImsStallDetectOFF


//	,eImsResetStallFlag
	,eImsGetStallFlag
	,eImsResetMotorStallFlag

	,eImsSetStallFactor
	,eImsGetStallFactor

	,eImsSetMotorWobbleWaitTime
	,eImsGetMotorWobbleWaitTime

	,eImsSetHoldCurrentWaitTime
	,eImsGetHoldCurrentWaitTime

	,eImsSetEncoderDeadBand
	,eImsGetEncoderDeadBand

	,eImsSetCounterMotorSteps
	,eImsGetCounterMotorSteps
	,eImsGetCounterEncoder
	,eImsSetCounterEncoder

	,eImsSetHoldCurrent
	,eImsSetMotorHoldCurrent
	,eImsGetHoldCurrent
	,eImsSetRunCurrent
	,eImsGetRunCurrent

	,eImsSetDecToAcc
//	,eImsSetStartSpeed
//	,eImsGetStartSpeed

//	,eImsSetEndSpeed
//	,eImsGetEndSpeed

//	,eImsSetAcceleration
//	,eImsGetAcceleration
//	,eImsSetDeceleration
//	,eImsGetDeceleration
	,eImsSetSpeed
	,eImsGetSpeed
	,eImsSetHomingSpeed
	,eImsGetHomingSpeed
	,eImsSetSloMoSpeed
	,eImsGetSloMoSpeed
	,eImsGetMoveFlag
	,eImsHomingSequence
	,eImsHomingMultiSequence
	,eImsGetPosition
	,eImsSetPosition
	,eImsResetPosition
	,eImsMotorResetPosition
	,eImsMotorSetPosition
	,eImsMoveAbsolutePos
//	,eImsMoveRelativePos
//	,eImsPrepareWay
//	,eImsResetPrepare
	,eImsStartMultipleMoveAbsolute
//	,eImsStartMultipleMoveReleative
	,eImsGetHomeSwitch
	,eImsGetMotorHomeSwitch
	,eImsGetEStopSwitch
//	,eWaitMove
	,eLPK
	,eLPKPCCmdSend
	,eLPKPCCmdReceive
	,eLPKGetSensorSwitchMagzInsert
	,eSync
//	,eImsEnableEStopRestorePosition
	,eImsGetStatus

	,eImsAdjustStartMove
	,eImsStopMotor
//	,eImsHomingEstop
	,eZBreakeFixed
	,eZBreakeFree
//	,eTestHandStart
//	,eTestHandStop
	,eResetEStopMov
	,eImsSlewStartAll
	,eImsSlewMoveID
	,eImsSlewEndAll
	,eImsSetMode_Mask
	,eImsInitMotorTaskVariable

	,eImsEStopRestoreCurrentAndGetPos
	,eImsEStopSafetyPos
	,eImsEStopRestorePos
	,eImsEStopRestoreCurrentAndSetPos

	,eImsHippieSafety
	,eImsHippieRestore
	,eImsHippieContinue

	,eSlaveSend
	,eSlaveReceive
	,eImsStromlosJQV
	,eImsSetMotorSpeed
	,eImsGetMotorSpeed

	,eImsEnablePositionMaintenance
	,eImsDisablePositionMaintenance
	,eImsGetGetPositionMaintenance
	,eImsDoorLifterOEM

}udtIMS_CMD_IDENT;


typedef enum
{
	eASyncNone
	,eASyncAdjust
	,eASyncShake
	,eASyncMv
	,eASyncGetStatus
	,eASyncGetError
	,eASyncClearError
	,eASyncGetAdjState
	,eASyncTeach
	,eASyncResetEStopMov
	,eASyncEStopMoveContinue
	,eASyncEStopHippieMoveContinue
	,eASyncEStopMoveSavetyPos
	,eASyncEStopMoveRestorePos
	,eASyncEStopRestoreCurrentAndSetPos
	,ASyncBarcodeJustage
	,eSearchAdapter
}udtASYNC_CMD_IDENT;

typedef struct
{
	unsigned short Mask;
	unsigned short Mode1536;
	double AbsolutePos[MAX_AXES];
}udtAsyncSaveMoveToPos;


typedef struct
{
	unsigned short StartEnable;
	unsigned short ID;
	long StepWay;
	double AbsolutePos;
	double RelativeWay;
	unsigned short HomeToSwitchMode;
	unsigned short InversHomingMode;
//	unsigned short Error;
//	unsigned short ErrorFlag;
//	unsigned short HomingFlag;
//	unsigned short StallFlag;
//	unsigned short Running;
unsigned short MoveRuningState;
}udtIMSAxesMoveData;

typedef struct
{
	unsigned short StartEnable;
	unsigned short ID;
	long StepWay;
	double AbsolutePos;
	double RelativeWay;
	double DeltaWay;
	unsigned short Error;
//2016.03.01	unsigned short ErrorFlag;
//	unsigned short HomingFlag;
//	unsigned short StallFlag;
	unsigned short Running;
}udtASyncAxesMoveData;

typedef struct
{
	unsigned short StartEnable;
	long RestorePos;
	long Auslenkung1;
	long Auslenkung2;
}udtASyncAxesShakeData;

typedef struct
{
	unsigned short StartEnable;
	unsigned short SwitchContact;
	unsigned short MaxPosFlag;
	long ZielPosBasicStep;
	long RelativeWayStep;
	long ZielPosMaximalStep;
	long ZielPosAktuellStep;
	long DeltaStep;
}udtASyncAxesSearchAdapter;

typedef struct
{
	udtIMS_CMD_IDENT	eCommand;
	//unsigned short	eCommand;
	unsigned short		Error;
	unsigned short		AxisID;
	unsigned short		Mask;
	unsigned short		MaskRunAxes;
	unsigned short		MaskErrorAxes;
	unsigned short		EStop;
	short 				LpkID;
	unsigned short		AdresseCyBiCtrl;
	union Argument
	{
		char			cChar;
		short			sShort;
		unsigned short	usShort;
		long			lLong;
		unsigned long	ulLong;
		float			fFloat;
		char			cBuffer[128];
	}Argument;
	union ArgumentB
	{
		long			lLongB;
		unsigned long	ulLongB;
		float			fFloatB;
	}ArgumentB;
	union ArgumentC
	{
		long			lLongC;
		unsigned long	ulLongC;
		float			fFloatC;
	}ArgumentC;
	udtIMSAxesMoveData MoveData[MAX_AXES];
}IMS_CommandBlock;


typedef struct
{
	udtASYNC_CMD_IDENT	eCommand;
	//unsigned short	eCommand;
	unsigned short		Error;// ist CyBioError
	unsigned short		Mask;
	unsigned short		EStop;
//	unsigned short		AdjustActive;
//	unsigned short		ShakeActive;
	union ArgumentAS
	{
		char			cChar;
		short			sShort;
		unsigned short	usShort;
		long			lLong;
		unsigned long	ulLong;
		float			fFloat;
		char			cBuffer[64];
	}Argument;
	udtASyncAxesMoveData MoveData[MAX_AXES];
}ASync_CommandBlock;

typedef struct
{
	udtIMS_CMD_IDENT	eCommand;
	//unsigned short	eCommand;
	unsigned short		Error;
	unsigned short		Adr;
	unsigned short		DelayTime;
	union Param
	{
		char			cChar;
		short			sShort;
		unsigned short	usShort;
		long			lLong;
		unsigned long	ulLong;
		float			fFloat;
		char			cBuffer[64];
	}Param;
}SLAVE_CommandBlock;


enum AxesState
{
	eSTATE_NONE				= 0x0000
	,eSTATE_RUN			 	= 0x0001
	,eSTATE_ESTOP			= 0x0002
	,eSTATE_SLOMO		 	= 0x0004

};

//****************************************************************************
// @Imported Global Variables
//****************************************************************************

//****************************************************************************
// @Global Variables
//****************************************************************************

//****************************************************************************
// @External Prototypes
//****************************************************************************

//****************************************************************************
// Prototypen definitions
//****************************************************************************
extern unsigned short IMS_GetError(const short ID, unsigned short *value);
extern unsigned short IMS_ResetError(const short ID);
//extern unsigned short IMS_OEM(const short ID, char *pszString);
extern unsigned short IMS_OEM(char *pszString);
extern unsigned short IMS_DoorLifterOEM(char *pszString);
//extern unsigned short IMS_TestCmd(const short ID);
extern unsigned short IMS_SetEncoderMode(const short ID);
extern unsigned short IMS_SetS1(const short ID);
extern unsigned short IMS_SetS1Invers(const short ID);
extern unsigned short IMS_SetS2_EStopOn(const short ID);
//extern unsigned short IMS_SetS2_EStopOff(const short ID);
extern unsigned short IMS_SetESC(const short ID);
extern unsigned short IMS_StallDetectON(const short ID);
extern unsigned short IMS_StallDetectOFF(const short ID);
extern unsigned short IMS_GetStallFlag(const short ID, unsigned short *value);
extern unsigned short IMS_ResetStallFlag(const short ID);
extern unsigned short IMS_GetStallFactor(const short ID, unsigned short *value);
extern unsigned short IMS_SetStallFactor(const short ID, unsigned short value);
extern unsigned short IMS_GetMotorWobbleWaitTime(const short ID, unsigned short *value);
extern unsigned short IMS_SetMotorWobbleWaitTime(const short ID, unsigned short value);
extern unsigned short IMS_GetHoldCurrentWaitTime(const short ID, unsigned short *value);
extern unsigned short IMS_SetHoldCurrentWaitTime(const short ID, unsigned short value);
extern unsigned short IMS_GetEncoderDeadBand(const short ID, unsigned short *value);
extern unsigned short IMS_SetEncoderDeadBand(const short ID, unsigned short value);
extern unsigned short IMS_GetHoldCurrent(const short ID, unsigned short *value);
extern unsigned short IMS_SetHoldCurrent(const short ID, unsigned short value);
extern unsigned short IMS_GetRunCurrent(const short ID, unsigned short *value);
extern unsigned short IMS_SetRunCurrent(const short ID, unsigned short value);
extern unsigned short IMS_GetCounterMotorSteps(const short ID, long *value);
extern unsigned short IMS_SetCounterMotorSteps(const short ID, long value);
extern unsigned short IMS_GetCounterEncoder(const short ID, long *value);
extern unsigned short IMS_SetCounterEncoder(const short ID, long value);
extern unsigned short IMS_SetDecToAcc(const short ID);
extern unsigned short IMS_GetStartSpeed(const short ID, long *value);
extern unsigned short IMS_SetStartSpeed(const short ID, long value);
extern unsigned short IMS_GetEndSpeed(const short ID, long *value);
extern unsigned short IMS_SetEndSpeed(const short ID, long value);
extern unsigned short IMS_GetAcceleration(const short ID, unsigned long *value);
extern unsigned short IMS_SetAcceleration(const short ID, unsigned long value);
extern unsigned short IMS_GetDeceleration(const short ID, unsigned long *value);
extern unsigned short IMS_SetDeceleration(const short ID, unsigned long value);
extern unsigned short IMS_GetMoveFlag(const short ID, unsigned short *value);
extern unsigned short IMS_GetHomeSwitch(const short ID, unsigned short *value);
extern unsigned short IMS_GetMotorHomeSwitch(const short ID, unsigned short *value);
extern unsigned short IMS_GetEStopSwitch(const short ID, unsigned short *value);
extern unsigned short IMS_HomingSequence(const short ID, unsigned short value);
extern unsigned short IMS_GetPosition(const short ID, long *value);
extern unsigned short IMS_SetPosition(const short ID, long value);
//extern unsigned short IMS_ResetPosition(const short ID);
extern unsigned short IMS_MoveAbsolutePos(const short ID, long value);
//extern unsigned short IMS_MoveRelativePos(const short ID, long value);
extern unsigned short IMS_LPK(const unsigned short AdresseCyBiCtrl, char *pszString);
extern unsigned short IMS_Sync(void);
extern unsigned short IMS_ESC_StopMotor(void);
extern unsigned short IMS_SetSpeed(const short ID, float start, float end, float accel);
extern unsigned short IMS_GetSpeed(const short ID, float *start, float *end, float *accel);
unsigned short IMS_SetSpeed_NoCalc(const short ID, long lStart, long lEnd, unsigned long ulAccel);
unsigned short IMS_GetSpeed_NoCalc(const short ID, long *lStart, long *lEnd, unsigned long *lAccel);

extern unsigned short IMS_StopMotor(const short ID);

//extern unsigned short IMS_StopMotorID(const short ID);
extern unsigned short IMS_MoveHandTest(const short ID, long value);
extern unsigned short IMS_SlewMove(const short ID, long value);

extern unsigned short IMS_LPKSendParallelCmd(const unsigned short AdresseCyBiCtrl, char *pszString);
extern unsigned short IMS_LPKReceiveParallelCmd( char *pszString);

extern unsigned short IMS_cmdLPKSafety(const short ID, const char Adress, const unsigned short AdresseCyBiCtrl);
extern unsigned short IMS_cmdLPKRestore(const short ID, const char Adress, const unsigned short AdresseCyBiCtrl);
//extern unsigned short IMS_cmdLPKContinue(const short ID, const char Adress, const unsigned short AdresseCyBiCtrl);
extern unsigned short IMS_cmdLPKContinue(const short ID, const char Adress, const unsigned short AdresseCyBiCtrl, char *pszString);
extern unsigned short IMS_cmdGetSensorMagazinInsert(const short ID, const char Adress, const unsigned short AdresseCyBiCtrl, unsigned short *value);

unsigned short IMS_EnablePositionMaintenance(const short ID);
unsigned short IMS_DisablePositionMaintenance(const short ID);
unsigned short IMS_GetPositionMaintenance(const short ID, unsigned short *value);


//****************************************************************************
// @Prototypes Of Local Functions
//****************************************************************************

#endif
