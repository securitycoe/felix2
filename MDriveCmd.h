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
#ifndef _MDRIVECMD_H_
#define _MDRIVECMD_H_
//****************************************************************************
// include files
//****************************************************************************

//****************************************************************************
// @Macros
//****************************************************************************

//****************************************************************************
// @Defines
//****************************************************************************

//****************************************************************************
// @Typedefs
//****************************************************************************


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

//############################################################################################################
//########### germeinsamme Nutzung ###########################################################################
//############################################################################################################
extern unsigned short ImsCmd_GetHomeSwitch(const short ID, unsigned short *value);
extern unsigned short ImsCmd_GetMotorHomeSwitch(const short ID, unsigned short *value);

//############################################################################################################
//########### nur FelixCmd ###################################################################################
//############################################################################################################
extern unsigned short ImsCmd_ResetError(const short ID);
extern unsigned short ImsCmd_GetHWError(const short ID, unsigned short *value);
extern unsigned short ImsCmd_GetOEMError(const short ID, unsigned short *value);
unsigned short ImsCmdInitMotorTaskVariable(const unsigned short i);
//unsigned short ImsCmd_SetEStopRestorePosition(unsigned short value);
unsigned short ImsCmd_SetEncoderMode(const short ID);
unsigned short ImsCmd_SetS1(const short ID);
unsigned short ImsCmd_SetS2EStopOn(const short ID);
//unsigned short ImsCmd_SetS2EStopOff(const short ID);
unsigned short ImsCmd_SetESC(const short ID);
unsigned short ImsCmd_SetDecToAcc(const short ID);
unsigned short ImsCmd_StallDetectON(const short ID);
unsigned short ImsCmd_StallDetectOFF(const short ID);

//############################################################################################################
//########### nur HardwareTask ###################################################################################
//############################################################################################################
extern unsigned short ImsCmd_MotorResetError(const short ID);


//extern unsigned short CheckMask(unsigned short mask);


//extern unsigned short ResetMaskBit(unsigned short mask, unsigned short Axis);
extern unsigned short ConfirmMask(unsigned short mask);


//extern unsigned short ImsCmd_GetError(const short ID, unsigned short *value);


//extern unsigned short ImsCmd_ResetStallFlag(const short ID);
extern unsigned short ImsCmd_ResetMotorStallFlag(const short ID);
//extern unsigned short ImsCmd_GetStallFlag(const short ID, unsigned short *value);
extern unsigned short ImsCmd_SetStallFactor(const short ID, unsigned short value);
extern unsigned short ImsCmd_GetStallFactor(const short ID, unsigned short *value);
extern unsigned short ImsCmd_SetMotorWobbleWaitTime(const short ID, unsigned short value);
extern unsigned short ImsCmd_GetMotorWobbleWaitTime(const short ID, unsigned short *value);
extern unsigned short ImsCmd_SetHoldCurrentWaitTime(const short ID, unsigned short value);
extern unsigned short ImsCmd_GetHoldCurrentWaitTime(const short ID, unsigned short *value);
extern unsigned short ImsCmd_SetEncoderDeadBand(const short ID, unsigned short value);
extern unsigned short ImsCmd_GetEncoderDeadBand(const short ID, unsigned short *value);
extern unsigned short ImsCmd_SetHoldCurrent(const short ID, unsigned short value);
extern unsigned short ImsCmd_GetHoldCurrent(const short ID, unsigned short *value);
extern unsigned short ImsCmd_StromlosJQV(const short ID);
extern unsigned short ImsCmd_SetRunCurrent(const short ID, unsigned short value);
extern unsigned short ImsCmd_GetRunCurrent(const short ID, unsigned short *value);
extern unsigned short ImsCmd_SetCounterMotorSteps(const short ID, long value);
extern unsigned short ImsCmd_GetCounterMotorSteps(const short ID, long *value);
//extern unsigned short ImsCmd_SetCounterEncoder(const short ID, long value);
extern unsigned short ImsCmd_GetCounterEncoder(const short ID, long *value);
extern unsigned short ImsCmd_GetMoveFlag(const short ID, unsigned short *value);
extern unsigned short ImsCmd_HomingSequence(const short ID, unsigned short value);

extern unsigned short ImsCmd_SetSpeed(const short ID, float start, float end, float accel);
extern unsigned short ImsCmd_GetSpeed(const short ID, float *start, float *end, float *accel);

//extern unsigned short ImsCmd_SetStartSpeed(const short ID, long value);
//extern unsigned short ImsCmd_GetStartSpeed(const short ID, long *value);
//extern unsigned short ImsCmd_SetEndSpeed(const short ID, long value);
//extern unsigned short ImsCmd_GetEndSpeed(const short ID, long *value);
//extern unsigned short ImsCmd_SetAcceleration(const short ID, unsigned long value);
//extern unsigned short ImsCmd_GetAcceleration(const short ID, unsigned long *value);
//extern unsigned short ImsCmd_SetDeceleration(const short ID, unsigned long value);
//extern unsigned short ImsCmd_GetDeceleration(const short ID, unsigned long *value);
extern unsigned short ImsCmd_ResetPosition(const short ID);
extern unsigned short ImsCmd_GetPosition(const short ID, long *value);
extern unsigned short ImsCmd_SetPosition(const short ID, long value);
extern unsigned short ImsCmd_MoveAbsolutePos(const short ID, long value);
//extern unsigned short ImsCmd_MoveRelativePos(const short ID, long value);
//extern unsigned short ImsCmd_PrepareWay(const short ID, long value);
//extern unsigned short ImsCmd_ResetPrepare(void);
//extern unsigned short ImsCmd_StartMultipleMoveAbsolute(unsigned short Mask);
//extern unsigned short ImsCmd_StartMultipleMoveReleative(void);
extern unsigned short ImsCmd_Sync(void);
//extern unsigned short ImsCmd_WaitMove(unsigned short *value);

//extern unsigned short ImsCmd_EnableEStopRestorePosition(void);
//extern unsigned short ImsCmd_DisableEStopRestorePosition(void);

extern unsigned short ImsCmd_GetEStopSwitch(const short ID, unsigned short *value);
extern unsigned short ImsCmd_SlewMoveID(const short ID, long value);
extern unsigned short ImsCmd_SlewInitAll(const unsigned short Mask);
extern unsigned short ImsCmd_SlewExitAll(const unsigned short Mask);

extern unsigned short ImsCmd_SetBreake(void);
extern unsigned short ImsCmd_ClearBreake(void);

unsigned short ImsCmd_Motor_SetSpeed(const short ID, long lStart, long lEnd, unsigned short ulAccel);
unsigned short ImsCmd_Motor_GetSpeed(const short ID, long *lStart, long *lEnd, unsigned long *ulAccel);
unsigned short ImsCmd_Motor_ResetPosition(const short ID);
unsigned short ImsCmd_Motor_SetPosition(const short ID, long value);
unsigned short ImsCmd_Motor_SetHoldCurrent(const short ID, unsigned short value);


unsigned short ImsCmd_EnablePositionMaintenance(const short ID);
unsigned short ImsCmd_DisablePositionMaintenance(const short ID);
unsigned short ImsCmd_GetGetPositionMaintenance(const short ID, unsigned short *value);

//extern unsigned short AllMotorInitS2(void);
// ############################################ SPEED ####################################################################
// ############################################ SPEED ####################################################################
//extern unsigned short MotorSetStartSpeed(const short ID, float Speed);
//extern unsigned short MotorGetStartSpeed(const short ID, float *Speed);
//extern unsigned short MotorSetEndSpeed(const short ID, float Speed);
//extern unsigned short MotorGetEndSpeed(const short ID, float *Speed);
// ############################################# ACCELERATION ###################################################################
// ############################################# ACCELERATION ###################################################################
//extern unsigned short MotorSetAcceleration(const short ID, float value);
//extern unsigned short MotorGetAcceleration(const short ID, float *value);

//extern unsigned short MotorSetWayAcceleration(const short ID, float value);
//extern unsigned short MotorGetWayAcceleration(const short ID, float *value, unsigned short Scr);
//extern unsigned short MotorSetWayHomingAcceleration(const short ID, float value);
//extern unsigned short MotorGetWayHomingAcceleration(const short ID, float *value);
//extern unsigned short MotorSetWaySloMoAcceleration(const short ID, float value);
//extern unsigned short MotorGetWaySloMoAcceleration(const short ID, float *value);

// ############################################# STROM ###################################################################
// ############################################# STROM ###################################################################
extern unsigned short MotorSetHoldCurrent(const short ID, unsigned short value);
extern unsigned short MotorGetHoldCurrent(const short ID, unsigned short *value, unsigned short Scr);
extern unsigned short MotorSetRunCurrent(const short ID, unsigned short value);
extern unsigned short MotorGetRunCurrent(const short ID, unsigned short *value, unsigned short Scr);
// ################################################## Genauigkeit, Stall, Time ##############################################
// ################################################## Genauigkeit, Stall, Time ##############################################
extern unsigned short MotorSetStallFactor(const short ID, unsigned short value);
extern unsigned short MotorGetStallFactor(const short ID, unsigned short *value, unsigned short Scr);
extern unsigned short MotorSetEncoderDeadBand(const short ID, unsigned short value);
extern unsigned short MotorGetEncoderDeadBand(const short ID, unsigned short *value);
extern unsigned short MotorSetPrecision(const short ID, double value);
extern unsigned short MotorGetPrecision(const short ID, double *value);
extern unsigned short MotorSetHoldCurrentWaitTime(const short ID, unsigned short value);
extern unsigned short MotorGetHoldCurrentWaitTime(const short ID, unsigned short *value, unsigned short Scr);
extern unsigned short MotorSetMotorWobbleWaitTime(const short ID, unsigned short value);
extern unsigned short MotorGetMotorWobbleWaitTime(const short ID, unsigned short *value, unsigned short Scr);


//extern unsigned short MotorResetError(const short ID);
//extern unsigned short MotorGetError(const short ID, unsigned short *value);
extern unsigned short MotorGetPosition(const short ID, double *value);
//extern unsigned short MotorGetPosition(const short ID, float *value);
//extern unsigned short MotorSetPosition(const short ID, float value);
//extern unsigned short MotorResetPosition(const short ID);
//extern unsigned short MotorGetMoveFlag(const short ID, unsigned short *value);
//extern unsigned short MotorGetStallFlag(const short ID, unsigned short *value);
//extern unsigned short MotorResetStallFlag(const short ID);


extern unsigned short MotorInit(unsigned short ID);
//extern unsigned short MotorSetSpeedNormal(unsigned short ID);
//extern unsigned short MotorSetSpeedHoming(unsigned short ID);
//extern unsigned short MotorSetSpeedSloMo(unsigned short ID);

extern unsigned short ImsCmd_Hippie_Safety_ManuallyMode(void);

// ############################################# MOVE ###################################################################
// ############################################# MOVE ###################################################################

//extern unsigned short MotorHoming(const short ID);
//extern unsigned short ImsCmd_GetAllRuningFlags(unsigned short Mask, unsigned short *value);
//extern unsigned short ImsCmd_GetAllErrorFlags(unsigned short Mask, unsigned short *value);
//****************************************************************************
// @Prototypes Of Local Functions
//****************************************************************************
#endif