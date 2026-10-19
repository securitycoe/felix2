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
#ifndef _DRV_CONFIG_H_
#define _DRV_CONFIG_H_
//****************************************************************************
// include files
//****************************************************************************
#include "DeviceConfig.h"
//****************************************************************************
// @Macros
//****************************************************************************

//****************************************************************************
// @Defines
//****************************************************************************
#define FELIX_BASE_ID 0
#define OEM_CMD_DIRECT_ID -1
#define LPK_ID -2
#define LPKINTERN_ID -3
#define LPK_PARALLEL_CMD_ID -4
#define LPK_MANUALLYMODE_INTERN_ID -5
#define DOORLIFT_ID -6

/*
#define MAX_CURR 2.6F		// in Ampere
#define ZERO_CURR 0
#define MIN_CURR ZERO_CURR

#define MOT_RUN 0
#define MOT_READY 1
#define MOT_STALL 2
#define MOT_HARDSTOP 3

#define CLOCKWISE_POSITIV TRUE
//#define COUNTERCLOCKWISE 0


#define REDUCE 1
#define ADJUST 2

#define CPY_FULL 0x0001
#define CPY_REV 0x0002
#define CPY_ADJ 0x0004
#define CPY_SPD 0x0008
#define CPY_FULL_CURR 0x0010
*/

#define HM_SLEW_NEGATIVE_CREEP_POSITIVE 1	// 1 Slew at VM in the minus direction and Creep at VI in the plus direction.
//#define HM_SLEW_NEGATIVE_CREEP_NEGATIVE 2	// 2 Slew at VM in the minus direction and Creep at VI in the minus direction.
#define HM_SLEW_POSITIVE_CREEP_NEGATIVE 3	// 3 Slew at VM in the plus direction and Creep at VI in the minus direction.
//#define HM_SLEW_POSITIVE_CREEP_POSITIVE 4	// 4 Slew at VM in the plus direction and Creep at VI in the plus direction.

//#define AXIS_ADR_LPK 'H'
#define AXIS_ADR_LPK_DEF 'H'
#define AXIS_ADR_LPK_12 'I'
#define AXIS_ADR_LPK_22 'J'
#define AXIS_ADR_LPK_32 'K'
#define AXIS_ADR_LPK_42 'L'

#define MIN_APPLICATION_SPD 0
#define MAX_APPLICATION_SPD 4

#ifdef _CF_FELIX
	#define AXIS_ID_X_DRIVE 1
	#define AXIS_ADR_X_DRIVE 'X'

	#define AXIS_ID_FRONT_CARRIAGE 2
	#define AXIS_ADR_FRONT_CARRIAGE 'F'

	#define AXIS_ID_REAR_CARRIAGE 3
	#define AXIS_ADR_REAR_CARRIAGE 'R'

	#define AXIS_ID_BOTTOM_CARRIAGE 4
	#define AXIS_ADR_BOTTOM_CARRIAGE 'B'

	#define AXIS_ID_Z_DRIVE 5
	#define AXIS_ADR_Z_DRIVE 'Z'

	#define MAX_AXES 5
	#define Z_AXES 1
//	#define ZAXES_ID MAX_AXES-1
	#define AXES_RESERVE 0//3

	#define AXIS_ID_DOORLIFT_DRIVE 6
	#define AXIS_ADR_DOORLIFT_DRIVE 'D'

#endif

//---- PENTA DISK -----------------
#ifdef _CF_PENTA_DISK
	#define PARA_IDX_Z_E1_DRIVE 0
	#define AXIS_ID_Z_E1_DRIVE 1
	#define AXIS_ADR_Z_E1_DRIVE 'A'

	#define PARA_IDX_Z_E2_DRIVE 1
	#define AXIS_ID_Z_E2_DRIVE 2
	#define AXIS_ADR_Z_E2_DRIVE 'B'

	#define PARA_IDX_Z_E3_DRIVE 2
	#define AXIS_ID_Z_E3_DRIVE 3
	#define AXIS_ADR_Z_E3_DRIVE 'C'

	#define PARA_IDX_Z_E4_DRIVE 3
	#define AXIS_ID_Z_E4_DRIVE 4
	#define AXIS_ADR_Z_E4_DRIVE 'D'

	#define PARA_IDX_Z_E5_DRIVE 4
	#define AXIS_ID_Z_E5_DRIVE 5
	#define AXIS_ADR_Z_E5_DRIVE 'E'

	#define PARA_IDX_DISK_DRIVE 5
	#define AXIS_ID_DISK_DRIVE 6
	#define AXIS_ADR_DISK_DRIVE 'F'

	#define MAX_AXES 6
	#define Z_AXES 5
	#define AXES_RESERVE 0//3
#endif

//Stallfactor
#define SF_RANGE_MIN 0
#define SF_RANGE_MAX 65000
//Motorwobbletime (Motor setting delay time)
#define MT_RANGE_MIN 0
#define MT_RANGE_MAX 65000
//Hold current delay time
#define HT_RANGE_MIN 0
#define HT_RANGE_MAX 65535
//encoder deadband
#define DB_RANGE_MIN 1
#define DB_RANGE_MAX 65000
//motor hold current
#define HC_RANGE_MIN 0
#define HC_RANGE_MAX 100
//motor run current
#define RC_RANGE_MIN 1
#define RC_RANGE_MAX 100
//homing, home to home switch
#define HM_RANGE_MIN 1
#define HM_RANGE_MAX 4
//counter C1 positions counter steps
#define C1_RANGE_MIN -2147483648
#define C1_RANGE_MAX +2147483647
//counter C2 positions counter encoder
#define C1_RANGE_MIN -2147483648
#define C1_RANGE_MAX +2147483647
//initial velocity
#define VI_RANGE_MIN -200000
#define VI_RANGE_MAX 200000
//maximum velocity
#define VM_RANGE_MIN -200000
#define VM_RANGE_MAX 200000
// acceleration
#define A_RANGE_MIN 91
#define A_RANGE_MAX 61035160
// acceleration
#define D_RANGE_MIN 91
#define D_RANGE_MAX 61035160


#define STATE_READY 0
#define STATE_BUSY 1
#define STATE_ESTOP 2
#define STATE_ERROR 3

#define FLG_HOMING_FAILURE 0
#define FLG_HOMING_OK 1


#define ESTOPSTATE_RESET 0
#define ESTOPSTATE_WAIT 1
#define ESTOPSTATE_SAVETYPOS 2
#define ESTOPSTATE_RESTORE 3
#define ESTOPSTATE_CONTINUE 4

//****************************************************************************
// @Typedefs
//****************************************************************************
#ifdef _CF_FELIX
enum AxesMask
{
	eX_Axis					= 0x0001
	,eFrontCarriage_Axis 	= 0x0002
	,eRearCarriage_Axis		= 0x0004
	,eBottomCarriage	 	= 0x0008
	,eZ_Axis				= 0x0010
	,eAllMask				= eX_Axis | eFrontCarriage_Axis | eRearCarriage_Axis | eBottomCarriage | eZ_Axis 
	,eZ_Ex_Axis				= eZ_Axis
	,eXYMask				= eX_Axis | eFrontCarriage_Axis | eRearCarriage_Axis | eBottomCarriage
	,eDoorLift_Axis			= 0x1000
};
#endif
//---- PENTA DISK -----------------
#ifdef _CF_PENTA_DISK
enum AxesMask
{
	eZ_E1_Axis				= 0x0001
	,eZ_E2_Axis				= 0x0002
	,eZ_E3_Axis				= 0x0004
	,eZ_E4_Axis				= 0x0008
	,eZ_E5_Axis				= 0x0010
	,eDisk_Axis				= 0x0020
	,eAllMask				= eZ_E1_Axis | eZ_E2_Axis | eZ_E3_Axis | eZ_E4_Axis | eZ_E5_Axis |  eDisk_Axis
	,eZ_Ex_Axis				= eZ_E1_Axis | eZ_E2_Axis | eZ_E3_Axis | eZ_E4_Axis | eZ_E5_Axis
	,eXYMask				= eDisk_Axis
};
#endif


typedef enum
{
	 eGET_CURRENT
	,eSET_CURRENT

	,eGET_RUN_CURRENT
	,eSET_RUN_CURRENT

	,eSET_HOLD_CURRENT
	,eGET_HOLD_CURRENT
//	,eGET_ACCELERATION_CURRENT
//	,eSET_ACCELERATION_CURRENT

	,eGET_ZERO_CURRENT
	,eSET_ZERO_CURRENT
}udtMOTOR_CURRENT_Type;

typedef struct
{
	unsigned short ID;
	char BusAdr;
	unsigned short AxisBit;
	float fOffset;
	float fSetHomingPosition;
	double fDiffRestoreActivate;
	// Geschwindigkeiten
	float fStartSpeed;			//[mm/s]
	float fEndSpeedNegative; 	// ?????? wird nicht verwendet, Endspeed negative direction [mm/s]
	float fEndSpeed;		 	// Endspeed positive direction [mm/s]
	float fHomingSpeed;			// Endspeed homing [mm/s]
	float f1536Speed;			// Endspeed 1536 Mode [mm/s]
	float fAccelerationNegative;// ?????? wird nicht verwendet, Acceleration negative direction [mm/s*s]
	float fAcceleration;	// Acceleration positive direction [mm/s*s]
	float fAccelerationHoming;	// Acceleration homing [mm/s*s]
	float fAccelerationSlew;	// Acceleration Slewmode [mm/s*s]
	float fAcceleration1536;	// Acceleration 1536 Mode [mm/s*s]

	// Grenzwerte
	float fMinAcceleration;		// min. Acceleration [mm/s*s]
	float fMaxAcceleration;		// max. Acceleration [mm/s*s]
	float fMax1536Acceleration;	 // ?????? wird nicht verwendet, max. 1536 Acceleration [mm/s*s]
	float fMinSpeed;			// min. Endspeed [mm/s*s]
	float fMaxSpeed;			// max. Endspeed [mm/s*s]
	float fMax1536Speed;		// ?????? wird nicht verwendet, max. 1536 Endspeed [mm/s*s]
	double fMinWay;				// min. drive range [mm]
	double fMaxWay;				// max. drive range [mm]

	// sonstiges
	float fMechLose;						// Lose (mech. Spiel) [mm]
	double fPrecisionDeadband;				// Genauigkeit der Positionierung [mm]
	double fPrecisionDeadband1536;			// Genauigkeit der 1536 Positionierung [mm]
	unsigned short usStallsteps;			// zulässige Diff. Steps zu Encoder [encoder_step]
	unsigned short usHoldCurrentWaitTime;	// hold current wait time [millisecounds]
	unsigned short usMotorWobbleWaitTime;	// wobbling wait time [millisecounds]
	unsigned short usMotorWobbleWaitTime1536;// 1536 wobbling wait time [millisecounds]

	// Strom
	unsigned short usRunCurrent;	//  run current [percent]
	unsigned short usRunCurrent1536;//  1536 run current [percent]
	unsigned short usHoldCurrent;	// hold current [percent]
	unsigned short usHoldCurrent1536;// 1536 hold current [percent]

	// Übersetzung
	float fWayProRev;			// Weg pro Umdrehung in mm
	float fIgrImpulseProRev;	// EncoderAuflösung	0 -> Achse ohne Encoder
	double fMotImpProMM;			// Motor-Impulse (µ-Step) pro mm	bzw. pro Umdr.
	double fENCImpProMM;			// Encoder-Impulse pro mm
//	unsinged short usFullStepPerRev;	// Vollschritte pro Motorumdrehung
//	unsigned short usMicroStepPerFullStep;	//Anzahl der Mikroschritte pro Vollschritt
//	float fMicroStepProRev;			// Microschritte pro Umdrehung
	//void (*Direction)(unsigned short usDirection);
	unsigned short HomeToSwitchMode;	//1. Speed: Specifies the direction and speed that the axis will
										//move until the switch is activated (VM).
										//2. Creep: Specifies the direction and speed that the axis will
										//move off the switch until it becomes inactive again (VI).
										//When HM is executed, the axis moves at VM in the direction
										//specified by the sign of speed. It then creeps off of the switch at VI
										//in the direction specified by the sign of creep. Motion is stopped
										//as soon as the switch becomes deactivated. Note that Speed and
										//Creep is set by the VM and VI commands.
										//Range 1 - 4
										//Modes
										// 1 Slew at VM in the minus direction and Creep at VI in the plus direction.
										// 2 Slew at VM in the minus direction and Creep at VI in the minus direction.
										// 3 Slew at VM in the plus direction and Creep at VI in the minus direction.
										// 4 Slew at VM in the plus direction and Creep at VI in the plus direction.
	unsigned short InversHomeToSwitchMode;	// wieHomeToSwitchMode nur invers
	short MoveSlewDirToHomeSwitch;		// negativer oder positiver Wert für Befehl SL, wenn Achse zu Homeposition fährt
	unsigned long ulDeceleration;		//?????? wird nicht verwendet
}udtIMSAxisFixedParameter;



// diese struchtur im FRAM ABSPEICHERN
typedef struct
{
	float fOffset;
	// Geschwindigkeiten
	float fStartSpeed;			//[mm/s]
	float fEndSpeedNegative; 	// ?????? wird nicht verwendet, Endspeed negative direction [mm/s]
	float fEndSpeed;		 	// Endspeed positive direction [mm/s]
	float fHomingSpeed;			// Endspeed homing [mm/s]
	float f1536Speed;			// Endspeed 1536 Mode [mm/s]
	float fAccelerationNegative;	// ?????? wird nicht verwendet, Acceleration negative direction [mm/s*s]
	float fAcceleration;	// Acceleration positive direction [mm/s*s]
	float fAccelerationHoming;	// Acceleration homing [mm/s*s]
	float fAccelerationSlew;	// Acceleration Slewmode [mm/s*s]
	float fAcceleration1536;	// Acceleration 1536 Mode [mm/s*s]

	// sonstiges
	double fPrecisionDeadband;				// Genauigkeit der Positionierung [mm]
	double fPrecisionDeadband1536;			// Genauigkeit der 1536 Positionierung [mm]
	unsigned short usStallsteps;						// zulässige Diff. Steps zu Encoder [encoder_step]
	unsigned short usHoldCurrentWaitTime;	// hold current wait time [millisecounds]
	unsigned short usMotorWobbleWaitTime;	// wobbling wait time [millisecounds]
	unsigned short usMotorWobbleWaitTime1536;// 1536 wobbling wait time [millisecounds]
	// Strom
	unsigned short usRunCurrent;	//  run current [percent]
	unsigned short usRunCurrent1536;//  1536 run current [percent]
	unsigned short usHoldCurrent;	// hold current [percent]
	unsigned short usHoldCurrent1536;// 1536 hold current [percent]
	double fMinWay;
	double fMaxWay;

}udtIMSAxisChangedParameter;

typedef struct
{
   //	unsigned short Error;
//	unsigned short StartEnable;
//	unsigned short StartHomingEnable;
//	unsigned short EStopStartHomingEnable;
//	unsigned short isHomingRuningActive;
//	unsigned short isHomingOkFlag;
//	unsigned short globalMoveRuningState;
//	unsigned short internMoveRuningState;
//	unsigned short EStopRuning;
//	unsigned short Toleranz;
	//char BusAdr;
//	unsigned short ID;
	long Way;
	//long LoadWay;
	unsigned short HomeToSwitchMode;
//	unsigned short InversHomingMode;
	long SaveStopPos;
//	unsigned short setMode1536;
	unsigned short getSpeedMode;
	unsigned short ModeHoldCurrent;
}udtIMSAxesPrepare;

typedef struct
{
	unsigned short AxisID;
	char AxisAdr;
	unsigned short AxisBit;
	unsigned short ParaIDX;
	unsigned short Enable;
	unsigned short AxisWithBrake;
	unsigned short AxisFullRotary;
	unsigned short AxisStallHandling; // ?????? wird nicht verwendet,
//	unsigned short Error;
//	unsigned short Error2;
	unsigned short LastErrorAxisHWForEver;
	unsigned short ErrorAxisHW;
	unsigned short ErrorAxisHWStall;
	unsigned short ErrorAxisCyBio;
	unsigned short ErrorFlag;
	unsigned short HomingFlag;
//	unsigned short StallFlag;
	unsigned short Running;
}udtIMSAxes;

typedef struct
{
	float AxisOffsetFromHomingSwitch;
}udtOffset;

typedef struct
{
	double MinWay;
	double MaxWay;
}udtMinMaxWay;

typedef struct
{
	float fSpd;
	float fAcc;
}udtApplicationAxisSpd;

typedef struct
{
	udtApplicationAxisSpd AppAxisSpd[MAX_AXES + AXES_RESERVE];
	unsigned short CRC;
}udtApplicationIndexSpd;

typedef struct
{
	double RelativeWay;
	double DeltaWay;
}udtRelativeDeltaWay;

typedef struct
{
  unsigned char InternEstopEnable;
  unsigned char ExternEstopInEnable;
  unsigned char LockExternOutEstopEnable;
}udtDeviceConfig;

typedef struct
{
	unsigned short SetPort_OEM_IN;//1 = pullup, 0 = pulldown
	unsigned short SetPort_UVL_IN;//1 = pullup, 0 = pulldown
}udtSensorConfig;

typedef struct
{
	unsigned short PowerOn;
	unsigned short StateAfterQuit;
}udtPortData;
typedef struct
{
	unsigned short EquipmentConfig;
	unsigned short DeviceConfig;
}udtEquipmentConfig;
typedef struct
{
	unsigned long EstopAll;
	unsigned long EstopByMotion;
}udtEstopCounter;
typedef struct
{
	unsigned short EstopWait;
}udtEstopWait;

typedef struct
{
	udtPortData SetPort_PHOTOMOS;
	udtPortData SetPort_VCC_OEM;
	udtPortData SetPort_SW_UVL;
	udtPortData SetPort_VCC_UVL;
	udtPortData SetPort_MUELL_CTRL;
	udtPortData SetPort_OEM_IN;
	udtPortData SetPort_UVL_IN;
	udtPortData SetPort_SW_RESERVE;
}udtPortConfig;

typedef struct
{
	double AbsolutePos;
	double MotImpProMM;
	double dSteps;
	long lSteps;
	long lStepWay;
	long lMotStepWay;
	double mm_dSteps;
	double mm_lSteps;
	double mm_lStepWay;
	double mm_lMotStepWay;
	double db_mm_dSteps;
	double db_mm_lSteps;
	double CmdPos;
	unsigned short Running;
	unsigned short Error;
}udtDebug;

typedef struct
{
	unsigned short Enable;
	unsigned short Up_DirA_Counter;
	unsigned short Down_DirB_Counter;
	unsigned short WaitTime;
}udtConfigStallExeption;

typedef struct
{
	unsigned long ExeptionLifeTimeCounter;
}udtCounterStallExeption;


/*typedef struct
{
  	const udtIMSAxisFixedParameter *ptr;
}pudtIMSAxisFixedParameter;
so ist die definition
pudtIMSAxisFixedParameter IMSFixedPara[5] ={
	&FixXDrive
	,&FixFrontCarriage
	,&FixRearCarriage
	,&FixBottomCarriage
	,&FixZDrive
};
so der zugriff
printf("idx:%u adr:%u\r", a, IMSFixedPara[a].ptr->Adress);fflush(stdout);

typedef struct
{
	udtIMSAxisChangedParameter *ptr;
}pudtIMSAxisChangedParameter;*/

//****************************************************************************
// @Imported Global Variables
//****************************************************************************

//****************************************************************************
// @Global Variables
//****************************************************************************
#ifdef _CF_FELIX
extern const udtIMSAxisFixedParameter FixFrontCarriage;
extern const udtIMSAxisFixedParameter FixRearCarriage;
extern const udtIMSAxisFixedParameter FixBottomCarriage;
extern const udtIMSAxisFixedParameter FixXDrive;
extern const udtIMSAxisFixedParameter FixZDrive;

extern udtIMSAxisChangedParameter FrontCarriage;
extern udtIMSAxisChangedParameter RearCarriage;
extern udtIMSAxisChangedParameter BottomCarriage;
extern udtIMSAxisChangedParameter XDrive;
extern udtIMSAxisChangedParameter ZDrive;
#endif
//---- PENTA DISK -----------------
#ifdef _CF_PENTA_DISK
extern const udtIMSAxisFixedParameter FixZ_E1_Drive;
extern const udtIMSAxisFixedParameter FixZ_E2_Drive;
extern const udtIMSAxisFixedParameter FixZ_E3_Drive;
extern const udtIMSAxisFixedParameter FixZ_E4_Drive;
extern const udtIMSAxisFixedParameter FixZ_E5_Drive;
extern const udtIMSAxisFixedParameter FixDiskDrive;

extern udtIMSAxisChangedParameter Z_E1_Drive;
extern udtIMSAxisChangedParameter Z_E2_Drive;
extern udtIMSAxisChangedParameter Z_E3_Drive;
extern udtIMSAxisChangedParameter DiskDrive;
extern udtIMSAxisChangedParameter Z_E4_Drive;
extern udtIMSAxisChangedParameter Z_E5_Drive;
#endif
extern const udtIMSAxisFixedParameter *pIMSFixedPara[];

extern udtIMSAxisChangedParameter *pIMSChangedPara[];

//extern const unsigned short g_AxesBits[];
//extern float g_AxesWayArray[];
//extern float *g_ptrAxesWayArray;

//extern unsigned short g_AxesStatusArray[MAX_AXES];
//extern unsigned short *g_ptrAxesStatusArray;
extern udtIMSAxes g_FelixAxes[];

extern udtOffset g_AxisOffset[];
extern udtOffset g_AxisOffsetDefault[];
extern udtDeviceConfig g_DeviceConfig;
extern udtMinMaxWay g_AxisMinMaxWay[];
extern udtRelativeDeltaWay g_SearchAdapterRelativeDeltaWay;
extern udtSensorConfig g_SensorConfig;
extern udtPortConfig g_PortConfig;
extern udtEquipmentConfig g_EquipmentConfig;
extern udtEstopCounter g_EstopCounter;
extern udtEstopWait g_EstopWait;
extern udtConfigStallExeption g_ConfigStallExeption[];
extern udtCounterStallExeption g_CounterStallExeption[];
extern udtApplicationIndexSpd g_ApplicationSpd[];
//****************************************************************************
// @External Prototypes
//****************************************************************************

//****************************************************************************
// Prototypen definitions
//****************************************************************************
extern void CopyFullFixedToChangedPara(udtIMSAxisChangedParameter *dst, const udtIMSAxisFixedParameter *src);
//extern unsigned int CheckAxisSpeedPara(udtIMSAxisChangedParameter *para, const udtIMSAxisFixedParameter *limit);
extern short GetAxisIndex(const unsigned char ID);
extern void AxisDataGenerateFromFixedData(void);
extern unsigned short GetAxisIDFromMask(unsigned short mask, unsigned short Axis);
extern void FelixAxesGenerate(void);
//extern unsigned short CountMaskAxes(unsigned short Mask);
extern void FelixDeviceConfigDefault(void);
//unsigned char CurrentCalc(float curr);
extern void g_SetAxes_ErrorAxisHW(unsigned short i, unsigned short value);
extern unsigned short g_GetAxes_ErrorAxisHW(unsigned short i);

extern void g_SetAxes_ErrorAxisCyBio(unsigned short i, unsigned short value);
extern unsigned short g_GetAxes_ErrorAxisCyBio(unsigned short i);

extern void g_SetAxes_ErrorFlag(unsigned short i, unsigned short value);
extern unsigned short g_GetAxes_ErrorFlag(unsigned short i);

//extern void g_SetAxes_StallFlag(unsigned short i, unsigned short value);
//extern unsigned short g_GetAxes_StallFlag(unsigned short i);

extern void g_SetAxes_ErrorAxisHWStall(unsigned short i, unsigned short value);
extern unsigned short g_GetAxes_ErrorAxisHWStall(unsigned short i);

extern void g_SetAxes_HomingFlag(unsigned short i, unsigned short value);
extern unsigned short g_GetAxes_HomingFlag(unsigned short i);

extern void g_SetAxes_Running(unsigned short i, unsigned short value);
extern unsigned short g_GetAxes_Running(unsigned short i);

extern void g_SetAxes_LastErrorAxisHWForEver(unsigned short i, unsigned short value);
extern unsigned short g_GetAxes_LastErrorAxisHWForEver(unsigned short i);

extern void FelixSensorAndPortGenerate(void);
extern void FelixEquipmentConfigGenerate(void);
extern void FelixEstopCounterGenerate(void);
extern void FelixEstopWaitGenerate(void);
extern void FelixSearchAdapterRelativeDeltaWayGenerate(void);
void FelixApplicationSpdGenerate(void);
//****************************************************************************
// @Prototypes Of Local Functions
//****************************************************************************
#endif