// @Module        Kommando-Interpreter
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

//#include <malloc.h>
#include "DevError.h"
//#include "Shared/Stacker_Lifter_Allgemein/StacComm.h"
//#include "Shared/Stacker_Lifter_Allgemein/XYZComm.h"
//#include "Shared/Stacker_Lifter_Allgemein/StacError.h"
#include "DevCmds.h"
#include "DeviceConfig.h"
#include "30-5015_Felix/XE167F/main.h"
#include "30-5015_Felix/XE167F/INT.h"
#include "30-5015_Felix/XE167F/CC2.h"
#include "30-5015_Felix/XE167F/U1C0_SLAVE_COM5.h"
#include "30-5015_Felix/XE167F/U2C1_RS232_COM4.h"
//#include "KFA_MISC.h"
#include "Felix_CmdCtrl.h"
#include "Felix_Cmd.h"


#include "MyScanner.h"
#include "Scan.h"
#include "Cxconfig.h"
#include "Felix_Hardware.h"
#include "Drv_config.h"
#include "MDrive_CmdCtrl.h"
#include "MDriveCmd.h"
#include "MotorExe.h"
#include "Felix_MotorTask.h"
#include "Felix_Misc.h"
#include "Felix_ASyncCtrl.h"
#include "Felix_FRAM.h"
#include "Felix_UserMain.h"
#include "Felix_HardwareTask.h"
#include "Felix_Slave.h"
#include "Felix_BarcodeCmd.h"
#include "LPK.h"
#include "Felix_SlaveTask.h"
#include "Felix_CmdParallel.h"
#include "Felix_ASync.h"
#include "Felix_DoorLifter.h"

extern unsigned char UserMainSlot;
extern unsigned char MotorSlot;
extern unsigned char ASyncCtrlSlot;
extern unsigned char HardwareSlot;
extern unsigned short gDoppelTasteUI;
//extern unsigned short gNrtl_LP;

#ifdef _CF_DEBUG_POSERROR
extern udtDebug DebugData[MAX_AXES];
extern void DebugDataClear(unsigned short Mask);
extern void DebugDataPrint(unsigned short all, unsigned short i_or_mask);
#endif
#define DEBUG_ERROR1 4001
#define DEBUG_ERROR2 4002
//****************************************************************************
// local definitions
//****************************************************************************
const char _SyncMovContinue[]	= "SyncMovContinue";
const char _SyncSpecialFunctionContinue[] = "SyncSpecFuncContinue";
//const char _GetPositionAdr[]	= "gPosAdr";
const char _SetToolConfig[]	= "sToolConfig";
const char _GetToolConfig[]	= "gToolConfig";
const char _SetDeviceConfig[]	= "sDeviceConfig";
const char _GetDeviceConfig[]	= "gDeviceConfig";

const char _cDowngradeFRAM[]	= "cDowngradeFRAM";
const char _cTestIO[]	= "cTestIO";
const char _cTestCom[]	= "cTestCom";
const char _cTestTool[]	= "cTestTool";
const char _SetAppSpd[]	= "sAppSpd";
const char _GetAppSpd[]	= "gAppSpd";
const char _GetDoorLiftState[] = "gDoorState";
const char _GetDoorLiftHomingState[] = "gDoorAdj";
const char _GetDoorLiftCloseState[] = "gDoorClose";

const char _DoorLiftOpen[] = "cDoorOpen";
const char _DoorLiftClose[] = "cDoorClose";
const char _DoorLiftOpenSync[] = "cDoorOpenS";
const char _DoorLiftCloseSync[] = "cDoorCloseS";

const char _GetDoorLiftPosition[] = "gDoorPos";
const char _DoorLiftAdj[] = "cDoorAdj";
const char _DoorLiftAdjust[] = "cDoorAdjust";
const char _GetDoorLiftSpd[] = "gDoorSpd";
const char _SetDoorLiftSpd[] = "sDoorSpd";
const char _GetDoorLiftAdjSpd[] = "gDoorAdjSpd";
const char _SetDoorLiftAdjSpd[] = "sDoorAdjSpd";

const char _GetDoorLiftMaxWay[] = "gDoorMaxWay";
const char _SetDoorLiftMaxWay[] = "sDoorMaxWay";
const char _GetDoorLiftOffset[] = "gDoorOffset";
const char _SetDoorLiftOffset[] = "sDoorOffset";

const char _GetDoorLiftOpenPos[] = "gDoorOpenPos";
const char _SetDoorLiftOpenPos[] = "sDoorOpenPos";
const char _GetDoorLiftClosePos[] = "gDoorClosePos";
const char _SetDoorLiftClosePos[] = "sDoorClosePos";


const char _GetDoorLiftStatus[] = "gDoorStatus";
const char _GetDoorLiftError[] = "gDoorError";	//! Fehler lesen
const char _ResetDoorLiftError[] = "cDoorError"; //! Fehler zurücksetzen

const char _GetDoorLiftOEMError[] = "gDoorOEMError"; //! Error der OEM Einheit lesen.
const char _ResetDoorLiftOEMError[] = "cDoorOEMError"; //! Error der OEM Einheit löschen.

const char _GetDoorLiftHWError[] = "gDoorHWError"; //! Fehler lesen
//const char _ResetDoorLiftHWError[] = "cDoorHWError"; //! Clear HardwareFehler
const char _DoorLiftOEM[] = "cDoorOEM"; //! Fehler lesen


//const char _SetDoorLifterCurrent[]	= "sDoorCur";				//! Den aktuellen Strom eines Antriebes einstellen
//const char _GetDoorLifterCurrent[]	= "gDoorCur";				//! Den aktuellen Strom eines Antriebes lesen
const char _SetDoorLifterDrvCurrent[]	= "sDoorDrvCur";			//! Fahrstrom einstellen
const char _GetDoorLifterDrvCurrent[]	= "gDoorDrvCur";		 	//! Fahrstrom lesen
const char _SetDoorLifterHoldCurrent[]	= "sDoorHoldCur";		//! Haltestrom einstellen
const char _GetDoorLifterHoldCurrent[]	= "gDoorHoldCur";		//! Haltestrom lesen

const char _SetDoorLifterDrvCurrentFRAM[]	= "sDoorDrvCurSave";			//! Fahrstrom einstellen
const char _GetDoorLifterDrvCurrentFRAM[]	= "gDoorDrvCurSave";		 	//! Fahrstrom lesen
const char _SetDoorLifterHoldCurrentFRAM[]	= "sDoorHoldCurSave";		//! Haltestrom einstellen
const char _GetDoorLifterHoldCurrentFRAM[]	= "gDoorHoldCurSave";		//! Haltestrom lesen
const char _GetDoorLiftSpdFRAM[] = "gDoorSpdSave";
const char _SetDoorLiftSpdFRAM[] = "sDoorSpdSave";
const char _GetDoorLiftAdjSpdFRAM[] = "gDoorAdjSpdSave";
const char _SetDoorLiftAdjSpdFRAM[] = "sDoorAdjSpdSave";

#ifdef _CF_DEBUG_POSERROR
const char _DebugPrint[]	= "cDebugPrint";
const char _DebugReset[]	= "cDebugReset";
const char _DebugClear[]	= "cDebugClear";
#endif
#define CSTR_INIT "i"        	// Umschalten auf Hostbetrieb
#define CSTR_QUIT "q"           // Umschalten auf manuelle Bedienung
#define CSTR_VERSION "v"		// Auslesen der Versionsnummer
#define CSTR_LAST_ANSWER "l"	// Wiederholtes Auslesen der letzten Antwort
#define CSTR_DRV_ADJ "j"		// Referenzfahrt
#define CSTR_SWITCH "T"			// Setzen/Rücksetzen von Bits
#define CSTR_READ_ABS "m"			// Lesen der aktuellen Position
#define CSTR_CMD_EXT "K"			/* Kommandoerweiterung */
	#define CSTR_READ_SENSOR "S"	/* Lesen des Zustandes eines Sensors */




//****************************************************************************
// default settings
//****************************************************************************

//****************************************************************************
// external functions
//****************************************************************************
extern unsigned short DataReset(void);
//****************************************************************************
// external data
//****************************************************************************
extern IMS_CommandBlock Msg;
//****************************************************************************
// internal functions
//****************************************************************************

//****************************************************************************
// public data
//****************************************************************************
unsigned short m_EnableMovCmd = FALSE;
unsigned short m_SynchronMoveEstop = FALSE;
unsigned short m_MaskSynchronMoveEstop = 0;
unsigned short m_MaskSynchronSpecialFunctionEstop = 0;
unsigned short m_FlagSynchronSpecialFunctionEstop = FALSE;
//****************************************************************************
// private data
//****************************************************************************
extern ASync_CommandBlock g_MsgASync;
char LongBuffer[CHARMEM_BLOCK_LENGTH+1];
#ifdef _CF_DEBUG_POSERROR
unsigned short DebugAsyncReposCnt = 0;
unsigned short DebugCmdMask;
unsigned short DebugCmdMode1536;
unsigned short DebugMoveError = 0;
#endif
unsigned short m_EStopCmdState = ESTOPSTATE_RESET;
unsigned short m_EStopCmdStateSkipped = ESTOPSTATE_RESET;

//****************************************************************************
// public functions
//****************************************************************************

//****************************************************************************
// private functions
//****************************************************************************
unsigned short CmdGetDeviceName(void);
unsigned short Cmd_Init(FILE *OutHandle);
unsigned short Cmd_Exit(void);
unsigned short Cmd_Homing(void);
unsigned short Cmd_HomingSyncron(void);
unsigned short Cmd_SwitchCtrl(void);
unsigned short Cmd_ResetError(void);
unsigned short Cmd_GetError(void);
unsigned short Cmd_ResetOEMError(void);
unsigned short Cmd_GetHWError(void);

unsigned short Cmd_OEM(void);
unsigned short Cmd_GetOEMError(void);
unsigned short Cmd_Drv_SetSpeed(void);
//unsigned short Cmd_Drv_GetSpeed(char *sPtr);
unsigned short Cmd_Drv_GetSpeed(void);
unsigned short Cmd_Drv_SetSonderSpeed(unsigned short Funktion);
//unsigned short Cmd_Drv_GetSonderSpeed(unsigned short Funktion, char *sPtr);
unsigned short Cmd_Drv_GetSonderSpeed(unsigned short Funktion);


//unsigned short Cmd_Drv_GetSpeedRange(char *sPtr);
unsigned short Cmd_Drv_GetSpeedRange(void);
unsigned short Cmd_Drv_GetMaxSpeedRange(void);
unsigned short Cmd_Drv_GetMaxWayRange(void);
unsigned short Cmd_Drv_SetMaxWayRange(void);
unsigned short Cmd_Drv_SetPosToMinMaxWay(unsigned short max);
unsigned short Cmd_Drv_InitSetPosMinMaxWay(void);


unsigned short Cmd_WriteCurrent(const udtMOTOR_CURRENT_Type CurrentType);
//unsigned short Cmd_ReadCurrent(const udtMOTOR_CURRENT_Type CurrentType, char *sPtr);
unsigned short Cmd_ReadCurrent(const udtMOTOR_CURRENT_Type CurrentType);

unsigned short Cmd_Drv_SetPrecision(void);
//unsigned short Cmd_Drv_GetPrecision(char *sPtr);
unsigned short Cmd_Drv_GetPrecision(void);
unsigned short Cmd_Drv_SetDeadBand(void);
//unsigned short Cmd_Drv_GetDeadBand(char *sPtr);
unsigned short Cmd_Drv_GetDeadBand(void);
unsigned short Cmd_Drv_SetStall(void);
//unsigned short Cmd_Drv_GetStall(char *sPtr);
unsigned short Cmd_Drv_GetStall(void);
unsigned short Cmd_Drv_SetWobbleTime(void);
//unsigned short Cmd_Drv_GetWobbleTime(char *sPtr);
unsigned short Cmd_Drv_GetWobbleTime(void);
unsigned short Cmd_Drv_SetHoldCurrentTime(void);
//unsigned short Cmd_Drv_GetHoldCurrentTime(char *sPtr);
unsigned short Cmd_Drv_GetHoldCurrentTime(void);

unsigned short Cmd_Drv_SetAccelWay(void);
//unsigned short Cmd_Drv_GetAccelWay(char *sPtr);
unsigned short Cmd_Drv_GetAccelWay(void);

unsigned short Cmd_ReadAbsolutePos(void);
//unsigned short Cmd_ReadMultipleAbsolutePos(unsigned short Index);


//unsigned short Cmd_MoveSyncron(unsigned short AbsolueElseRelative);
unsigned short Cmd_MoveSyncron(void);
//unsigned short Cmd_MoveASync(unsigned short AbsolueElseRelative);
unsigned short Cmd_MoveASync(void);
unsigned short Cmd_Teachen(void);
unsigned short Cmd_AShake(void);
unsigned short Cmd_SearchAdapter(unsigned short CmdSync);
unsigned short Cmd_Drv_GetStatus(void);
unsigned short Cmd_Drv_GetHomingStatus(void);
unsigned short Cmd_Drv_GetOffset(void);
unsigned short Cmd_Drv_SetOffset(void);
unsigned short Cmd_SetTestFlag(void);
unsigned short Cmd_GetTestFlag(void);
//unsigned short Cmd_DeviceTest(void);
unsigned short Cmd_FRAMReadVersion(void);
unsigned short Cmd_ReadSerialNo(void);
unsigned short Cmd_SetSerialNo(void);
unsigned short Cmd_GetFRAMCapacity(void);
unsigned short Cmd_GetUserDataCapacity(void);
unsigned short Cmd_GetUserData(void);
unsigned short Cmd_SetUserData(void);
unsigned short Cmd_EraseUserData(void);
unsigned short Cmd_FRAMDump(FILE *OutHandle);
//unsigned short Cmd_SetUserDataV2(void);
//unsigned short Cmd_SetExtInEStop(void);
unsigned short Cmd_GetExtInEStop(void);
//unsigned short Cmd_SetIntEStop(void);
unsigned short Cmd_GetIntEStop(void);
unsigned short Cmd_SetLockExtOutEStop(void);
unsigned short Cmd_GetLockExtOutEStop(void);
unsigned short Cmd_SetBreake(void);
unsigned short Cmd_ClearBreake(void);
unsigned short Cmd_WaitSynchronMoveEstop(void);
unsigned short Cmd_SlewInit(void);
unsigned short Cmd_SlewExit(void);
unsigned short Cmd_Slew(void);
//unsigned short Cmd_GetEStopPort(void);
unsigned short CmdGetVersion(void);
unsigned short Cmd_ReadSensor(void);
unsigned short Cmd_TestReadSensor(char *sPtr, FILE *InHandle);
unsigned short Cmd_SetPort(unsigned short isSetPort);
unsigned short Cmd_TestSetPort(unsigned short isSetPort);
unsigned short Cmd_ReadLimitSwitch(void);
unsigned short CmdIntern_Homing(unsigned short Mask);
unsigned short ConfirmHoming(unsigned short Mask);
#ifdef _CF_DEBUG_POSERROR
void DebugPosErrReset(void);
unsigned short DebugPositionTesten(unsigned short counter);
unsigned short Cmd_DebugPrint(FILE *OutHandle);
unsigned short Cmd_DebugReset(void);
unsigned short Cmd_DebugClear(void);
#endif
unsigned short Command_K_Extension(void);
unsigned short Cmd_SetSensorConfig(void);
unsigned short Cmd_GetSensorConfig(void);
unsigned short Cmd_SetPortConfig(void);
unsigned short Cmd_GetPortConfig(void);
unsigned short Cmd_SetEquipment(unsigned short Time);
unsigned short Cmd_SetEquipmentConfig(void);
unsigned short Cmd_GetEquipmentConfig(void);
unsigned short Cmd_SetDeviceConfig(void);
unsigned short Cmd_GetDeviceConfig(void);
unsigned short Cmd_DowngradeHelper(void);
unsigned short Cmd_TestPorts(void);
unsigned short CmdComCheck(FILE *InHandle, FILE *OutHandle);
unsigned short Cmd_ToolTest(void);
double ConvertAngle(double Value);
unsigned short Cmd_SetDeviceParameter(void);
unsigned short Cmd_GetDeviceParameter(void);

unsigned short Cmd_SetStallExeptionParameter(void);
unsigned short Cmd_GetStallExeptionParameter(void);
unsigned short Cmd_SetStallExeptionCounter(void);
unsigned short Cmd_GetStallExeptionCounter(void);

unsigned short Cmd_SetApplicationSpeed(void);
unsigned short Cmd_GetApplicationSpeed(void);




unsigned short ScanEStopCommand(void);
//unsigned short EStopHandlingCmd(unsigned short *ContinueSyncMove, FILE *InHandle, FILE *OutHandle);
unsigned short EStopHandlingCmd(unsigned short *ContinueSyncMove, FILE *InHandle, FILE *OutHandle, char *pszHippieAnswer);
unsigned short CheckCmdCanBeExecutedInEstop(void);

short ConvertMonth(const char* sptr);
void ConvertDate(const char *const date, char* dest_date);
unsigned short CmdGetVersionNumber(void);

unsigned short Cmd_ClearEstop(unsigned short NoRestore);
unsigned short Cmd_ClearEstopExe(unsigned short NoRestore);

//unsigned short Cmd_HandMove(unsigned short Start);
unsigned short TestForFirmware(void);
unsigned short Cmd_WaitSynchronSpecialFunctionMoveEstop(void);

unsigned short CheckCmdCanBeExecutedInEstop(void)
{
	unsigned short error = errNone;
	unsigned short ScannerIdent;
	char buf[25];

	ScannerIdent = scanner(buf);
		switch(ScannerIdent)
		{
			case IDENT  :
						if(
						(strcmp(buf, _OEM) == 0) || (strcmp(buf, CSTR_LAST_ANSWER) == 0)
						|| (strcmp(buf, _GetVersion) == 0) || (strcmp(buf, CSTR_VERSION) == 0)
						|| (strcmp(buf, _GetVersionNumber) == 0)
						|| (strcmp(buf, _SetTestFlag) == 0) || (strcmp(buf, _GetTestFlag) == 0)
						|| (strcmp(buf, _FormatFRAM) == 0) || (strcmp(buf, _GetFRAMVersion) == 0)
						|| (strcmp(buf, _GetFRAMDump) == 0) || (strcmp(buf, _GetFRAMCapacity) == 0)
						|| (strcmp(buf, _GetSerialNumber) == 0) || (strcmp(buf, _SetSerialNumber) == 0)
						|| (strcmp(buf, _gUserDataCapacity) == 0) || (strcmp(buf, _gUserData) == 0)
						|| (strcmp(buf, _sUserData) == 0) || (strcmp(buf, _eUserData) == 0)
						|| (strcmp(buf, _SetExtEStop) == 0) || (strcmp(buf, _GetExtEStop) == 0)
						|| (strcmp(buf, _SetIntEStop) == 0) || (strcmp(buf, _GetIntEStop) == 0)
						|| (strcmp(buf, _SetLockForwardEStop) == 0) || (strcmp(buf, _GetLockForwardEStop) == 0)
						|| (strcmp(buf, _GetDeviceName) == 0) || (strcmp(buf, _GetDeviceNameUSB) == 0)
						|| (strcmp(buf, _CmdTeach) == 0)
						|| (strcmp(buf, _CmdTeachInit) == 0) || (strcmp(buf, _CmdTeachExit) == 0)
						|| (strcmp(buf, _DeviceTest) == 0)
						|| (strcmp(buf, _GetOffset) == 0) || (strcmp(buf, _SetOffset) == 0)
						|| (strcmp(buf, "gEStopPort") == 0)
						|| (strcmp(buf, _GetMaxWay) == 0) || (strcmp(buf, _SetMaxWay) == 0)
						|| (strcmp(buf, _SetPosWayMax) == 0)
						|| (strcmp(buf, _SetPosWayMin) == 0) || (strcmp(buf, _CmdPosWayInit) == 0)
						|| (strcmp(buf, "sBreake") == 0) || (strcmp(buf, "cBreake") == 0)
						|| (strcmp(buf, "cBCJustage") == 0) || (strcmp(buf, _BarcodeReaderCmd) == 0)
						|| (strcmp(buf, _BarcodeReaderReset) == 0) || (strcmp(buf, _cDowngradeFRAM) == 0)
						|| (strcmp(buf, _SetSensorConfig) == 0) || (strcmp(buf, _GetSensorConfig) == 0)
						|| (strcmp(buf, _SetPortConfig) == 0) || (strcmp(buf, _GetPortConfig) == 0)
						|| (strcmp(buf, _SetToolConfig) == 0) || (strcmp(buf, _GetToolConfig) == 0)
						|| (strcmp(buf, _cTestIO) == 0) || (strcmp(buf, _cTestCom) == 0) || (strcmp(buf, _cTestTool) == 0)
						|| (strcmp(buf, _GetSensor) == 0) || (strcmp(buf, "gSensorTest") == 0)
						|| (strcmp(buf, "sPortTest") == 0) || (strcmp(buf, "cPortTest") == 0)
						|| (strcmp(buf, _SetPort) == 0) || (strcmp(buf, _ClearPort) == 0)
						|| (strcmp(buf, _SetDeviceParameter) == 0) || (strcmp(buf, _GetDeviceParameter) == 0)
						#ifdef _CF_DEBUG_POSERROR
						|| (strcmp(buf, _DebugPrint) == 0) || (strcmp(buf, _DebugReset) == 0)
						|| (strcmp(buf, _DebugClear) == 0)
						#endif
						|| (strcmp(buf, "cTestFw") == 0)
						|| (strcmp(buf, _CmdExit) == 0) || (strcmp(buf, CSTR_QUIT) == 0)
						|| (strcmp(buf, "sStallConfig") == 0) || (strcmp(buf, "gStallConfig") == 0)
						|| (strcmp(buf, "sStallCounter") == 0) || (strcmp(buf, "gStallCounter") == 0)
						|| (strcmp(buf, _ClearEstop) == 0) || (strcmp(buf, _GetPosition) == 0)
						|| (strcmp(buf, _SetAppSpd) == 0) || (strcmp(buf, _GetAppSpd) == 0)
						|| (strcmp(buf, _GetDoorLiftState) == 0) || (strcmp(buf, _GetDoorLiftStatus) == 0)
						|| (strcmp(buf, _DoorLiftOpen) == 0) || (strcmp(buf, _DoorLiftClose) == 0)
						|| (strcmp(buf, _DoorLiftOpenSync) == 0) || (strcmp(buf, _DoorLiftCloseSync) == 0)
						|| (strcmp(buf, _DoorLiftAdj) == 0) || (strcmp(buf, _DoorLiftAdjust) == 0)
						|| (strcmp(buf, _SetDoorLiftSpd) == 0) || (strcmp(buf, _GetDoorLiftSpd) == 0)
						|| (strcmp(buf, _SetDoorLiftAdjSpd) == 0) || (strcmp(buf, _GetDoorLiftAdjSpd) == 0)
						|| (strcmp(buf, _SetDoorLiftMaxWay) == 0) || (strcmp(buf, _GetDoorLiftMaxWay) == 0)
						|| (strcmp(buf, _SetDoorLiftOpenPos) == 0) || (strcmp(buf, _GetDoorLiftOpenPos) == 0)
						|| (strcmp(buf, _SetDoorLiftClosePos) == 0) || (strcmp(buf, _GetDoorLiftClosePos) == 0)
						|| (strcmp(buf, _SetDoorLiftOffset) == 0) || (strcmp(buf, _GetDoorLiftOffset) == 0)
						|| (strcmp(buf, _ResetDoorLiftError) == 0) || (strcmp(buf, _GetDoorLiftError) == 0)
						|| (strcmp(buf, _ResetDoorLiftOEMError) == 0) || (strcmp(buf, _GetDoorLiftOEMError) == 0)
						//|| (strcmp(buf, _ResetDoorLiftHWError) == 0)
						|| (strcmp(buf, _GetDoorLiftHWError) == 0)
						//|| (strcmp(buf, _SetDoorLifterCurrent) == 0) || (strcmp(buf, _GetDoorLifterCurrent) == 0)
						|| (strcmp(buf, _SetDoorLifterDrvCurrent) == 0) || (strcmp(buf, _GetDoorLifterDrvCurrent) == 0)
						|| (strcmp(buf, _SetDoorLifterHoldCurrent) == 0) || (strcmp(buf, _GetDoorLifterHoldCurrent) == 0)
						|| (strcmp(buf, _DoorLiftOEM) == 0) || (strcmp(buf, _GetDoorLiftPosition) == 0)

					)
					{
						ReloadScannerBuffer();
						WDO();
					}
					else
					{
						/*if((strcmp(buf, _CmdInit) == 0) || (strcmp(buf, CSTR_INIT) == 0)
							|| (strcmp(buf, _CmdExit) == 0) || (strcmp(buf, CSTR_QUIT) == 0))*/
						if((strcmp(buf, _CmdInit) == 0) || (strcmp(buf, CSTR_INIT) == 0))

						{
							if((error = g_Cmd_ASync_ResetEstopMove()) != errNone) {return error;}
						}
						return (unsigned short)ERR_SAFETY_APPLIANCE;
					}
				break;
			default : ReloadScannerBuffer();
						WDO();
						break;					    
		}
	return error;
}

unsigned short ScanEStopCommand(void)
{
	unsigned short error = ERR_COVER_OPEN;
	unsigned short ScannerIdent;
	char buf[25];

	ScannerIdent = scanner(buf);
	switch(ScannerIdent)
	{
		case HASH	:	error =  ERR_PROG_ABORT;
						break;
		case Scan_EOF	:	error = ERR_CMD_NOT_IMPLEMENTED;
							break;
		case IDENT  :
						
						if(strcmp(buf, _WAITSEC) == 0)
						{
							if(m_EStopCmdState == ESTOPSTATE_WAIT){error = errNone;}
						}
						if(strcmp(buf, _SAFETYPOS) == 0)
						{
							if(m_EStopCmdState == ESTOPSTATE_SAVETYPOS){error = errNone;}
						}
						if(strcmp(buf, _RESTOREPOS) == 0)
						{
							switch(m_EStopCmdState)
							{
								case ESTOPSTATE_RESTORE:
														error = errNone;
														break;
								case ESTOPSTATE_SAVETYPOS:
														m_EStopCmdState = ESTOPSTATE_RESTORE;
														m_EStopCmdStateSkipped = ESTOPSTATE_SAVETYPOS;
														error = errNone;
														break;
								default:				error = ERR_COVER_OPEN;
														break;
							}
						}
						if(strcmp(buf, _CONTSEC) == 0)
						{
							switch(m_EStopCmdState)
							{
								case ESTOPSTATE_CONTINUE:
														error = errNone;
														break;
								case ESTOPSTATE_SAVETYPOS:
														m_EStopCmdState = ESTOPSTATE_CONTINUE;
														m_EStopCmdStateSkipped = ESTOPSTATE_SAVETYPOS;
														error = errNone;
														break;
								case ESTOPSTATE_RESTORE:
														m_EStopCmdState = ESTOPSTATE_CONTINUE;
														m_EStopCmdStateSkipped = ESTOPSTATE_RESTORE;
														error = errNone;
														break;
/*								case ESTOPSTATE_WAIT:
														m_EStopCmdState = ESTOPSTATE_CONTINUE;
														m_EStopCmdStateSkipped = ESTOPSTATE_WAIT;
														error = errNone;
														break;*/

								default:				error = ERR_COVER_OPEN;
														break;
							}
						}
						break;
			default:		error = ERR_CMD_NOT_PRACTICABLE;
							break;
	}
	ReloadScannerBuffer();
	return error;
}

unsigned short EStopHandlingCmd(unsigned short *ContinueSyncMove, FILE *InHandle, FILE *OutHandle, char *pszHippieAnswer)
{
	unsigned short error = ERR_COVER_OPEN;
	unsigned short events;
	unsigned short ComEvents;
	unsigned short State;

	#ifdef _CF_ESTOP_DEBUG_PRINT
	if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("EHA_1\r");}
	#endif

	*ContinueSyncMove = 0;
	switch(m_EStopCmdState)
	{
		case ESTOPSTATE_WAIT:

		 						m_EStopCmdState = ESTOPSTATE_SAVETYPOS;
								// Warten auf das aufheben des EStops
								#ifdef _CF_ESTOP_DEBUG_PRINT
								if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("EHA_2\r");}
								#endif
								while(1)
								{
									WDO();
									if(g_MsgCheckEstopState(&State) != errNone)
									{
										#ifdef _CF_ESTOP_DEBUG_PRINT
										if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("EHA_3\r");}
										#endif
										return error;
									}
									else
									{
										if(State == ESTOP_OFF)
										{
											//m_EStopCmdState == ESTOPSTATE_SAVETYPOS;
											#ifdef _CF_ESTOP_DEBUG_PRINT
											if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("EHA_4\r");}
											#endif
											return errNone;
										}
									}
									/*if(gGetDriveMode() != DRV_MODE_ESTOP)
									{
										g_ClearEstopFlag();
										return errNone;
									}*/
									// Auf Schnittstellenereignisse reagieren
									events = K_Event_Wait(EVENT_MASTER_COM | EVENT_USER_BREAK, 22, 0 );	// löscht danach die angegebenen Events
									if(events)
									{
										K_Event_Reset(UserMainSlot, EVENT_MASTER_COM);
										error = errNone;
										if(events & EVENT_USER_BREAK)
										{
											K_Event_Reset(UserMainSlot, EVENT_USER_BREAK);
											putc(ACK, OutHandle);fflush(OutHandle);
											if(isSlavePresent())
											{
												SendBreakToSlave();
											}
											error = ERR_PROG_ABORT;
											#ifdef _CF_DEBUG_POSERROR
											DebugPosErrReset();
											#endif
											//putc(ACK, OutHandle);fflush(OutHandle);
										}
										if(events & EVENT_MASTER_COM)
										{
											#ifdef _CF_ESTOP_DEBUG_PRINT
											if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("EHA_5\r");}
											#endif

											ComEvents = UART_GetEvents(InHandle);
											if(ComEvents == COM_EVENT_CR)
											{
												error = ERR_COVER_OPEN;
											}
											if(ComEvents == COM_EVENT_SYNC)
											{
												//error = errSync;
												UART_ResetEvents(InHandle);
												EmptyTheStream(InHandle);
												UART_ResetDataBufferAndErrorFlag(InHandle);
												putc(ACK, OutHandle);fflush(OutHandle);
												//if((error = ImsCmd_Sync()) != errNone) {return error;}
												error = ImsCmd_Sync();
											}
											if(ComEvents == COM_EVENT_OVERFLOW)
											{
												error = errRxOverflow;
											}
										}

										if(error != errNone) break;
									}
									K_Task_Wait(100);
								}//end...while(1)
								#ifdef _CF_ESTOP_DEBUG_PRINT
								if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("EHA_6\r");}
								#endif
								//break;


								break;
		case ESTOPSTATE_SAVETYPOS:
								m_EStopCmdState = ESTOPSTATE_RESTORE;
								//#ifdef _CF_NRTL
								#ifdef _CF_ESTOP_DEBUG_PRINT
								if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("EHA_7\r");}
								#endif

								if(isNRTL_LP())
								{
									#ifdef _CF_ESTOP_DEBUG_PRINT
									if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("EHA_8\r");}
									#endif

									if((error = g_MsgCheckMOSFETSwitch()) == ERR_SAFETY_MONITORING)
									{
										//m_EnableMovCmd = FALSE;
										DebugPosErrReset();
									}
								}
								//#endif
								#ifdef _CF_ESTOP_DEBUG_PRINT
								if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("EHA_9\r");}
								#endif

								if(!g_isEstopFlag())
								{
									g_ClearGenerateEStopError();//NEU_20160726
									//08-2024 ClearEStopAnswerFromLPK();//NEU_20160726
								}
								error = g_ASync_EStopMoveSavetyPos();

								#ifdef _CF_ESTOP_DEBUG_PRINT
								if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("EHA_10\r");}
								#endif

								break;
		case ESTOPSTATE_RESTORE:
								m_EStopCmdState = ESTOPSTATE_CONTINUE;//ESTOPSTATE_RESTORE;
								//#ifdef _CF_NRTL
								/*if(isNRTL_LP())
								{
									if((error = g_MsgCheckMOSFETSwitch()) == ERR_SAFETY_MONITORING)
									{
										//m_EnableMovCmd = FALSE;
										DebugPosErrReset();
									}
								}*/
								//#endif
								#ifdef _CF_ESTOP_DEBUG_PRINT
								if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("EHA_11\r");}
								#endif

								if(m_EStopCmdStateSkipped == ESTOPSTATE_SAVETYPOS)
								{
									m_EStopCmdStateSkipped = ESTOPSTATE_RESET;
									if(isNRTL_LP())
									{
										#ifdef _CF_ESTOP_DEBUG_PRINT
										if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("EHA_12\r");}
										#endif

										if((error = g_MsgCheckMOSFETSwitch()) == ERR_SAFETY_MONITORING)
										{
											//m_EnableMovCmd = FALSE;
											DebugPosErrReset();
										}
									}
									#ifdef _CF_ESTOP_DEBUG_PRINT
									if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("EHA_13\r");}
									#endif
									if(!g_isEstopFlag())
									{
										g_ClearGenerateEStopError();//NEU_20160726
										//08-2024 ClearEStopAnswerFromLPK();//NEU_20160726
									}
									if((error = g_ASync_EStopMoveSavetyPos()) != errNone)
									{
										#ifdef _CF_ESTOP_DEBUG_PRINT
										if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("EHA_14\r");}
										#endif

										return error;
									}
									#ifdef _CF_ESTOP_DEBUG_PRINT
									if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("EHA_15\r");}
									#endif

								}
								error = g_ASync_EStopMoveRestorePos();
								break;
		case ESTOPSTATE_CONTINUE:
								m_EStopCmdState = ESTOPSTATE_RESET;
								if(g_isEstopFlag())
								{
									m_EStopCmdState = ESTOPSTATE_WAIT;
									return (unsigned short) ERR_SAFETY_APPLIANCE;
								}
								//#ifdef _CF_NRTL
								/*if(isNRTL_LP())
								{
									if((error = g_MsgCheckMOSFETSwitch()) == ERR_SAFETY_MONITORING)
									{
										//m_EnableMovCmd = FALSE;
										DebugPosErrReset();
									}
								}*/
								//#endif
								#ifdef _CF_ESTOP_DEBUG_PRINT
								if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("EHA_16\r");}
								#endif

								if(m_EStopCmdStateSkipped != ESTOPSTATE_RESET)
								{
									if(isNRTL_LP())
									{
										#ifdef _CF_ESTOP_DEBUG_PRINT
										if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("EHA_17\r");}
										#endif

										if((error = g_MsgCheckMOSFETSwitch()) == ERR_SAFETY_MONITORING)
										{
											//m_EnableMovCmd = FALSE;
											DebugPosErrReset();
										}
									}
									#ifdef _CF_ESTOP_DEBUG_PRINT
									if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("EHA_18\r");}
									#endif
									if(!g_isEstopFlag())
									{
										g_ClearGenerateEStopError();//NEU_20160726
										//08-2024 ClearEStopAnswerFromLPK();//NEU_20160726
									}
									if(m_EStopCmdStateSkipped == ESTOPSTATE_SAVETYPOS)
									{
										#ifdef _CF_ESTOP_DEBUG_PRINT
										if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("EHA_19\r");}
										#endif

										m_EStopCmdStateSkipped = ESTOPSTATE_RESET;
										if((error = g_ASync_EStopMoveSavetyPos()) != errNone)
										{
											#ifdef _CF_ESTOP_DEBUG_PRINT
											if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("EHA_20\r");}
											#endif
											return error;
										}
										if((error = g_ASync_EStopMoveRestorePos()) != errNone)
										{
											#ifdef _CF_ESTOP_DEBUG_PRINT
											if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("EHA_21\r");}
											#endif
											return error;
										}
									}
									if(m_EStopCmdStateSkipped == ESTOPSTATE_RESTORE)
									{
										m_EStopCmdStateSkipped = ESTOPSTATE_RESET;
										#ifdef _CF_ESTOP_DEBUG_PRINT
										if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("EHA_22\r");}
										#endif

										if((error = g_ASync_EStopMoveRestorePos()) != errNone)
										{
											#ifdef _CF_ESTOP_DEBUG_PRINT
											if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("EHA_23\r");}
											#endif

											return error;
										}
									}
								}
								else
								{
										#ifdef _CF_ESTOP_DEBUG_PRINT
										if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("EHA_24\r");}
										#endif
									if(!g_isEstopFlag())
									{
										g_ClearGenerateEStopError();
										//08-2024 ClearEStopAnswerFromLPK();
									}
								}
								//if((error = g_EStopMoveSavetyPos()) != errNone){return error;}
								//if((error = g_EStopMoveRestorePos()) != errNone){return error;}
								#ifdef _CF_ESTOP_DEBUG_PRINT
								if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("EHA_25\r");}
								#endif

								//if((error = g_ASync_EStopMoveContinue(pszHippieAnswer)) != errNone)
								error = g_ASync_EStopMoveContinue(pszHippieAnswer);
								if((error != errNone) && (error != errInfo))
								{
									#ifdef _CF_ESTOP_DEBUG_PRINT
									if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("EHA_25a\r");}
									#endif
									return error;
								}
								#ifdef _CF_ESTOP_DEBUG_PRINT
								if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("EHA_26\r");}
								#endif
								if(m_SynchronMoveEstop)
								{
									// EStop, fortsetzen synchrones Move Commando
									*ContinueSyncMove = 1;
									m_SynchronMoveEstop = FALSE;
									#ifdef _CF_ESTOP_DEBUG_PRINT
									if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("EHA_26a\r");}
									#endif

								}
								else
								{
									if(m_FlagSynchronSpecialFunctionEstop)
									{
										// EStop, fortsetzen synchrons Homing, synchrones SearchAdapter
										*ContinueSyncMove = 3;
										m_FlagSynchronSpecialFunctionEstop = FALSE;
										#ifdef _CF_ESTOP_DEBUG_PRINT
										if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("EHA_26b\r");}
										#endif
									}
									else
									{
										*ContinueSyncMove = 2;
										#ifdef _CF_ESTOP_DEBUG_PRINT
										if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("EHA_26c\r");}
										#endif
									}
								}
								//08-2024 error = errNone;
								break;
		default:				error = ERR_CMD_NOT_IMPLEMENTED;
								break;
	}
	#ifdef _CF_ESTOP_DEBUG_PRINT
	if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("EHA_27\r");}
	#endif

	return error;
}
unsigned short EStopCommandHandling(unsigned short *ptrContinueCmd, FILE *InHandle, FILE *OutHandle, char *pszHippieAnswer)
{
	unsigned short error = ERR_CMD_NOT_PRACTICABLE;

	#ifdef _CF_ESTOP_DEBUG_PRINT
	if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("ECH_1\r");}
	#endif
	if((error = ScanEStopCommand()) != errNone)
	{
		#ifdef _CF_ESTOP_DEBUG_PRINT
		if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("ECH_2a\r");}
		#endif

		m_EStopCmdState = ESTOPSTATE_RESET;
		g_Cmd_ASync_ResetEstopMove();

		#ifdef _CF_ESTOP_DEBUG_PRINT
		if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("ECH_2b\r");}
		#endif
		return error;
	}
	#ifdef _CF_ESTOP_DEBUG_PRINT
	if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("ECH_3\r");}
	#endif
	//if((error = EStopHandlingCmd(ptrContinueCmd, InHandle, OutHandle, pszHippieAnswer)) != errNone)
	error = EStopHandlingCmd(ptrContinueCmd, InHandle, OutHandle, pszHippieAnswer);
	if((error != errNone) && (error != errInfo))
	{
		#ifdef _CF_ESTOP_DEBUG_PRINT
		if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("ECH_4a\r");}
		#endif

		if(error != (unsigned short)ERR_SAFETY_APPLIANCE)
		{
			m_EStopCmdState = ESTOPSTATE_RESET;
			#ifdef _CF_ESTOP_DEBUG_PRINT
			if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("ECH_4b\r");}
			#endif
			ClearEStopAnswerFromLPK(); //08-2024
			g_Cmd_ASync_ResetEstopMove();
		}
		else
		{
			if(error == (unsigned short)ERR_SAFETY_APPLIANCE)
			{
				m_EStopCmdState = ESTOPSTATE_WAIT;
			}
			else
			{
				ClearEStopAnswerFromLPK(); //08-2024
			}

			#ifdef _CF_ESTOP_DEBUG_PRINT
			if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("ECH_4c\r");}
			#endif

		}
		//return error;
	}
	else
	{
	   if(m_EStopCmdState == ESTOPSTATE_RESET)
	   {
		   ClearEStopAnswerFromLPK(); //08-2024
	   }
	}
	return error;
}

unsigned short CommandFelix(FILE *InHandle, FILE *OutHandle)
{
	unsigned short ScannerIdent;
	char buf[25];
	unsigned short error = ERR_CMD_NOT_IMPLEMENTED;
//	unsigned short events;
//	unsigned short ComEvents;
	unsigned short ContinueCmd;
	char szHippieAnswer[64];

	WDO();
	SaveScannerBuffer();
	if((m_EStopCmdState == ESTOPSTATE_RESET) && g_isGenerateEStopError())
	{

		#ifdef _CF_ESTOP_DEBUG_PRINT
		if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("CF_1\r");}
		#endif

		SaveScannerBufferForReload();
		if((error = CheckCmdCanBeExecutedInEstop()) != errNone)
		{
			#ifdef _CF_ESTOP_DEBUG_PRINT
			if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("CF_2\r");}
			#endif

			if(error == (unsigned short)ERR_SAFETY_APPLIANCE){m_EStopCmdState = ESTOPSTATE_WAIT;}
			return error;
		}
	}
//	ScannerIdent = scanner(buf);

	ContinueCmd = 0;
	if(m_EStopCmdState != ESTOPSTATE_RESET)
	{
		#ifdef _CF_ESTOP_DEBUG_PRINT
		if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("CF_3\r");}
		#endif

		if((error = EStopCommandHandling(&ContinueCmd, InHandle, OutHandle, szHippieAnswer)) != errNone)
		{
			#ifdef _CF_ESTOP_DEBUG_PRINT
			if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("CF_4\r");}
			#endif
			return error;
		}
		if(!ContinueCmd)
		{
			#ifdef _CF_ESTOP_DEBUG_PRINT
			if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("CF_5\r");}
			#endif
			return error;
		}
	}
	if(ContinueCmd)
	{
		#ifdef _CF_ESTOP_DEBUG_PRINT
		if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("CF_6\r");}
		#endif

		if(ContinueCmd == 1)
		{
			// EStop, fortsetzen synchrones Move Commandoo
			ScannerIdent = IDENT;
			#ifdef _CF_ESTOP_DEBUG_PRINT
			if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("CF_6a\r");}
			#endif
			strcpy(buf, _SyncMovContinue);
		}
		if(ContinueCmd == 2)
		{
			#ifdef _CF_ESTOP_DEBUG_PRINT
			if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("CF_6b\r");}
			#endif

	   		ReloadScannerBuffer();
			ScannerIdent = scanner(buf);
		}
		if(ContinueCmd == 3)
		{	
			// EStop, fortsetzen synchrons Homing, synchrones SearchAdapter
			ScannerIdent = IDENT;
			#ifdef _CF_ESTOP_DEBUG_PRINT
			if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("CF_6c\r");}
			#endif
			strcpy(buf, _SyncSpecialFunctionContinue);
		}
	}
	else
	{
		#ifdef _CF_ESTOP_DEBUG_PRINT
		if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("CF_7\r");}
		#endif

		ScannerIdent = scanner(buf);
		#ifdef _CF_ESTOP_DEBUG_PRINT
		if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("CF_7b: %u\r", ScannerIdent);}
		#endif

	}
	switch(ScannerIdent)
	{

		case HASH	:	error = ERR_PROG_ABORT;
						#ifdef _CF_DEBUG_POSERROR
						DebugPosErrReset();
						#endif
						break;
		case PERCENT	:	error = errSync;
						break;
		case Scan_EOF	:	error = ERR_CMD_NOT_IMPLEMENTED;
						break;
		case IDENT  :
						error = ERR_CMD_NOT_IMPLEMENTED;

						if(strcmp(buf, _SyncMovContinue) == 0){error = Cmd_WaitSynchronMoveEstop();break;}
						if(strcmp(buf, _SyncSpecialFunctionContinue) == 0){error = Cmd_WaitSynchronSpecialFunctionMoveEstop();break;}

if(strcmp(buf, _PCLoad) == 0){error = Cmd_ParallelCmdLoad();break;}
if(strcmp(buf, _PCRun) == 0){error = Cmd_ParallelCmdRun();break;}
if(strcmp(buf, _PCClear) == 0){error = ParallelCmdClear();break;}

						if((strcmp(buf, _CmdInit) == 0) || (strcmp(buf, CSTR_INIT) == 0)){error = Cmd_Init(OutHandle);break;}
						if((strcmp(buf, _CmdExit) == 0) || (strcmp(buf, CSTR_QUIT) == 0)){error = Cmd_Exit();break;}
						if((strcmp(buf, _GetVersion) == 0) || (strcmp(buf, CSTR_VERSION) == 0)){K_Task_Wait(1);error = CmdGetVersion();break;}
						if(strcmp(buf, _GetVersionNumber) == 0){K_Task_Wait(1);error = CmdGetVersionNumber();break;}
						
						if((strcmp(buf, _GetDeviceName) == 0) || (strcmp(buf, _GetDeviceNameUSB) == 0)) {error = CmdGetDeviceName();break;}
						if(strcmp(buf, CSTR_LAST_ANSWER) == 0){K_Task_Wait(1);error = errInfo;break;}

						if(strcmp(buf, _GetSensor) == 0){error = Cmd_ReadSensor();break;}
						if(strcmp(buf, "gSensorTest") == 0){error = Cmd_TestReadSensor(g_acAnswerBuf, InHandle);break;}
						if(strcmp(buf, _SetPort) == 0){error = Cmd_SetPort(TRUE);break;}
						if(strcmp(buf, _ClearPort) == 0){error = Cmd_SetPort(FALSE);break;}
						if(strcmp(buf, "sPortTest") == 0){error = Cmd_TestSetPort(TRUE);break;}
						if(strcmp(buf, "cPortTest") == 0){error = Cmd_TestSetPort(FALSE);break;}

						
						if(strcmp(buf, _GetLimitSwitch) == 0){error = Cmd_ReadLimitSwitch();break;}

						if(strcmp(buf, CSTR_SWITCH) == 0){error = Cmd_SwitchCtrl();break;}

						if(strcmp(buf, _ResetError) == 0){error = Cmd_ResetError();break;}
						if(strcmp(buf, _GetError) == 0){error = Cmd_GetError();break;}

						if(strcmp(buf, _ResetHWError) == 0){error = Cmd_ResetOEMError();break;}
						if(strcmp(buf, _GetHWError) == 0){error = Cmd_GetHWError();break;}

						if(strcmp(buf, _OEMError) == 0){error = Cmd_GetOEMError();break;}
						if(strcmp(buf, _ResetOEMError) == 0){error = Cmd_ResetOEMError();break;}

						if(strcmp(buf, _SetDefSpeed) == 0){error = Cmd_Drv_SetSpeed();break;}
						//if(strcmp(buf, _GetDefSpeed) == 0){error = Cmd_Drv_GetSpeed(g_acAnswerBuf);break;}
						if(strcmp(buf, _GetDefSpeed) == 0){error = Cmd_Drv_GetSpeed();break;}
						if(strcmp(buf, _SetLowSpeed) == 0){error = Cmd_Drv_SetSonderSpeed(SLOMO);break;}
						//if(strcmp(buf, _GetLowSpeed) == 0){error = Cmd_Drv_GetSonderSpeed(SLOMO, g_acAnswerBuf);break;}
						if(strcmp(buf, _GetLowSpeed) == 0){error = Cmd_Drv_GetSonderSpeed(SLOMO);break;}
						if(strcmp(buf, _SetRefSpeed) == 0){error = Cmd_Drv_SetSonderSpeed(HOMING);break;}
						//if(strcmp(buf, _GetRefSpeed) == 0){error = Cmd_Drv_GetSonderSpeed(HOMING, g_acAnswerBuf);break;}
						if(strcmp(buf, _GetRefSpeed) == 0){error = Cmd_Drv_GetSonderSpeed(HOMING);break;}
						//if(strcmp(buf, _GetLimitsRev) == 0){error = Cmd_Drv_GetSpeedRange(g_acAnswerBuf);break;}
						if(strcmp(buf, _GetLimitsRev) == 0){error = Cmd_Drv_GetSpeedRange();break;}

						if(strcmp(buf, _GetMaxSpeed) == 0){error = Cmd_Drv_GetMaxSpeedRange();break;}
						if(strcmp(buf, _GetMaxWay) == 0){error = Cmd_Drv_GetMaxWayRange();break;}
						if(strcmp(buf, _SetMaxWay) == 0){error = Cmd_Drv_SetMaxWayRange();break;}

						if(strcmp(buf, _SetPosWayMin) == 0){error = Cmd_Drv_SetPosToMinMaxWay(0);break;}
						if(strcmp(buf, _SetPosWayMax) == 0){error = Cmd_Drv_SetPosToMinMaxWay(1);break;}
						if(strcmp(buf, _CmdPosWayInit) == 0){error = Cmd_Drv_InitSetPosMinMaxWay();break;}

						if(strcmp(buf, _SetCurrent) == 0){error = Cmd_WriteCurrent(eSET_CURRENT);break;}
						//if(strcmp(buf, _GetCurrent) == 0){error = Cmd_ReadCurrent(eGET_CURRENT, g_acAnswerBuf);break;}
						if(strcmp(buf, _GetCurrent) == 0){error = Cmd_ReadCurrent(eGET_CURRENT);break;}
						if(strcmp(buf, _SetDrvCurrent) == 0){error = Cmd_WriteCurrent(eSET_RUN_CURRENT);break;}
						//if(strcmp(buf, _GetDrvCurrent) == 0){error = Cmd_ReadCurrent(eGET_RUN_CURRENT, g_acAnswerBuf);break;}
						if(strcmp(buf, _GetDrvCurrent) == 0){error = Cmd_ReadCurrent(eGET_RUN_CURRENT);break;}
						if(strcmp(buf, _SetHoldCurrent) == 0){error = Cmd_WriteCurrent(eSET_HOLD_CURRENT);break;}
						//if(strcmp(buf, _GetHoldCurrent) == 0){error = Cmd_ReadCurrent(eGET_HOLD_CURRENT, g_acAnswerBuf);break;}
						if(strcmp(buf, _GetHoldCurrent) == 0){error = Cmd_ReadCurrent(eGET_HOLD_CURRENT);break;}


						if(strcmp(buf, _SetPrecision) == 0){error = Cmd_Drv_SetPrecision();break;}
						//if(strcmp(buf, _GetPrecision) == 0){error = Cmd_Drv_GetPrecision(g_acAnswerBuf);break;}
						if(strcmp(buf, _GetPrecision) == 0){error = Cmd_Drv_GetPrecision();break;}
						if(strcmp(buf, _SetDeadBand) == 0){error = Cmd_Drv_SetDeadBand();break;}
						//if(strcmp(buf, _GetDeadBand) == 0){error = Cmd_Drv_GetDeadBand(g_acAnswerBuf);break;}
						if(strcmp(buf, _GetDeadBand) == 0){error = Cmd_Drv_GetDeadBand();break;}
						if(strcmp(buf, _SetStall) == 0){error = Cmd_Drv_SetStall();break;}
						//if(strcmp(buf, _GetStall) == 0){error = Cmd_Drv_GetStall(g_acAnswerBuf);break;}
						if(strcmp(buf, _GetStall) == 0){error = Cmd_Drv_GetStall();break;}

						if(strcmp(buf, _SetMotorTime) == 0){error = Cmd_Drv_SetWobbleTime();break;}
						//if(strcmp(buf, _GetMotorTime) == 0){error = Cmd_Drv_GetWobbleTime(g_acAnswerBuf);break;}
						if(strcmp(buf, _GetMotorTime) == 0){error = Cmd_Drv_GetWobbleTime();break;}
						if(strcmp(buf, _SetHoldCurrentTime) == 0){error = Cmd_Drv_SetHoldCurrentTime();break;}
						//if(strcmp(buf, _GetHoldCurrentTime) == 0){error = Cmd_Drv_GetHoldCurrentTime(g_acAnswerBuf);break;}
						if(strcmp(buf, _GetHoldCurrentTime) == 0){error = Cmd_Drv_GetHoldCurrentTime();break;}

						if(strcmp(buf, _SetAccelWay) == 0){error = Cmd_Drv_SetAccelWay();break;}
						//if(strcmp(buf, _GetAccelWay) == 0){error = Cmd_Drv_GetAccelWay(g_acAnswerBuf);break;}
						if(strcmp(buf, _GetAccelWay) == 0){error = Cmd_Drv_GetAccelWay();break;}

						if(strcmp(buf, _GetPosition) == 0){error = Cmd_ReadAbsolutePos();break;}
						//if((strcmp(buf, CSTR_READ_ABS) == 0)||(strcmp(buf, _GetPositionAdr) == 0)){error = Cmd_ReadMultipleAbsolutePos(TRUE);break;}

						if((strcmp(buf, _CmdAdjust) == 0) || (strcmp(buf, CSTR_DRV_ADJ) == 0)){error = Cmd_Homing();break;}
						if(strcmp(buf, _CmdAdjustSync) == 0){error = Cmd_HomingSyncron();break;}

						//if(strcmp(buf, _CmdMoveTill) == 0){error = Cmd_MoveSyncron(1);break;}
						if(strcmp(buf, _CmdMoveTill) == 0){error = Cmd_MoveSyncron();break;}
						//if(strcmp(buf, _CmdMoveRelSync) == 0){error = Cmd_MoveSyncron(0);break;}
						//if(strcmp(buf, _CmdMovAbsAsync) == 0){error = Cmd_MoveASync(1);break;}
						if(strcmp(buf, _CmdMovAbsAsync) == 0){error = Cmd_MoveASync();break;}

						if(strcmp(buf, _CmdTeach) == 0){error = Cmd_Teachen();break;}
						if(strcmp(buf, _CmdTeachInit) == 0){error =errNone;break;}
						if(strcmp(buf, _CmdTeachExit) == 0){error =errNone;break;}
						
						//if(strcmp(buf, _CmdMovRelAsync) == 0){error = Cmd_MoveASync(0);break;}

						if(strcmp(buf, _CmdShake) == 0){error = Cmd_AShake();break;}
						if(strcmp(buf, _CmdSearchAdapter) == 0){error = Cmd_SearchAdapter(FALSE);break;}
						if(strcmp(buf, _CmdSearchAdapterWait) == 0){error = Cmd_SearchAdapter(TRUE);break;}

						if(strcmp(buf, _SetOffset) == 0){error = Cmd_Drv_SetOffset();break;}
						if(strcmp(buf, _GetOffset) == 0){error = Cmd_Drv_GetOffset();break;}
						if(strcmp(buf, _GetStatus) == 0){error = Cmd_Drv_GetStatus();break;}
						if(strcmp(buf, _GetAdjustStatus) == 0){error = Cmd_Drv_GetHomingStatus();break;}
						//if(strcmp(buf, CSTR_MOVE_ABS) == 0){error = Cmd_AbsMultipleFahren();break;}

						if(strcmp(buf, _OEM) == 0){error = Cmd_OEM();break;}
						if(strcmp(buf, _SetTestFlag) == 0){error = Cmd_SetTestFlag();break;}
						if(strcmp(buf, _GetTestFlag) == 0){error = Cmd_GetTestFlag();break;}
						//if(strcmp(buf, _DeviceTest) == 0){error = Cmd_DeviceTest();break;}


						if(strcmp(buf, _FormatFRAM) == 0){error = DataAndFRAMInit();break;}
						if(strcmp(buf, _ResetFRAM) == 0){error = DataReset();break;}

						if(strcmp(buf, _GetFRAMVersion) == 0){error = Cmd_FRAMReadVersion();break;}
						if(strcmp(buf, _GetFRAMDump) == 0){error = Cmd_FRAMDump(OutHandle);break;}
						if(strcmp(buf, _GetSerialNumber) == 0){error = Cmd_ReadSerialNo();break;}
						if(strcmp(buf, _SetSerialNumber) == 0){error = Cmd_SetSerialNo();break;}
						if(strcmp(buf, _GetFRAMCapacity) == 0){error = Cmd_GetFRAMCapacity();break;}

						if(strcmp(buf, _gUserDataCapacity) == 0){error = Cmd_GetUserDataCapacity();break;}
						if(strcmp(buf, _gUserData) == 0){error = Cmd_GetUserData();break;}
						if(strcmp(buf, _sUserData) == 0){error = Cmd_SetUserData();break;}
						//if(strcmp(buf, "sDataV2") == 0){error = Cmd_SetUserDataV2();break;}
						if(strcmp(buf, _eUserData) == 0){error = Cmd_EraseUserData();break;}

						//if(strcmp(buf, _SetExtEStop) == 0){error = Cmd_SetExtInEStop();break;}
						if(strcmp(buf, _GetExtEStop) == 0){error = Cmd_GetExtInEStop();break;}
						//if(strcmp(buf, _SetIntEStop) == 0){error = Cmd_SetIntEStop();break;}
						if(strcmp(buf, _GetIntEStop) == 0){error = Cmd_GetIntEStop();break;}
						if(strcmp(buf, _SetLockForwardEStop) == 0){error = Cmd_SetLockExtOutEStop();break;}
						if(strcmp(buf, _GetLockForwardEStop) == 0){error = Cmd_GetLockExtOutEStop();break;}

						if(strcmp(buf, _CmdSlew) == 0){error = Cmd_Slew();break;}
						if(strcmp(buf, _CmdSlewInit) == 0){error = Cmd_SlewInit();break;}
						if(strcmp(buf, _CmdSlewExit) == 0){error = Cmd_SlewExit();break;}

						if(strcmp(buf, "sBreake") == 0){error = Cmd_SetBreake();break;}
						if(strcmp(buf, "cBreake") == 0){error = Cmd_ClearBreake();break;}
						//if(strcmp(buf, "gEStopPort") == 0){error = Cmd_GetEStopPort();break;}

						// keine weiteren Parameter erforderlich, im Gegensatz zu früheren Readern
						if(strcmp(buf, _BarcodeReaderRead) == 0){error = Cmd_ReadBarcode(g_acAnswerBuf);break;}
						if(strcmp(buf, _BarcodeReaderReset) == 0){error = Cmd_BarcodeReset();break;}
						if(strcmp(buf, _BarcodeReaderCmd) == 0){error = Cmd_BarcodeSendCmdTo(g_acAnswerBuf);break;}
						if(strcmp(buf, "cBCJustage") == 0){error = Cmd_BarcodeReaderJustage(OutHandle);break;}
						//_BarcodeResetFlag
						if(strcmp(buf, _BarcodeResetFlags) == 0){error = ResetBarcodeDataForSlave(g_acAnswerBuf);break;}
/*						if(strcmp(buf, "sHandMv") == 0){error = Cmd_HandMove(TRUE);break;}
						if(strcmp(buf, "cHandMv") == 0){error = Cmd_HandMove(FALSE);break;}*/
						#ifdef _CF_DEBUG_POSERROR
						if(strcmp(buf, _DebugPrint) == 0){error = Cmd_DebugPrint(OutHandle);break;}
						if(strcmp(buf, _DebugReset) == 0){error = Cmd_DebugReset();break;}
						if(strcmp(buf, _DebugClear) == 0){error = Cmd_DebugClear();break;}
						#endif
						if(strcmp(buf, CSTR_CMD_EXT) == 0){error = Command_K_Extension();break;}

						if(strcmp(buf, _SetSensorConfig) == 0){error = Cmd_SetSensorConfig();break;}
						if(strcmp(buf, _GetSensorConfig) == 0){error = Cmd_GetSensorConfig();break;}
						if(strcmp(buf, _SetPortConfig) == 0){error = Cmd_SetPortConfig();break;}
						if(strcmp(buf, _GetPortConfig) == 0){error = Cmd_GetPortConfig();break;}
						if(strcmp(buf, _SetToolConfig) == 0){error = Cmd_SetEquipmentConfig();break;}
						if(strcmp(buf, _GetToolConfig) == 0){error = Cmd_GetEquipmentConfig();break;}

						if(strcmp(buf, _SetDeviceConfig) == 0){error = Cmd_SetDeviceConfig();break;}
						if(strcmp(buf, _GetDeviceConfig) == 0){error = Cmd_GetDeviceConfig();break;}

						if(strcmp(buf, _cDowngradeFRAM) == 0){error = Cmd_DowngradeHelper();break;}
						if(strcmp(buf, _cTestIO) == 0){error = Cmd_TestPorts();break;}
						if(strcmp(buf, _cTestCom) == 0){error = CmdComCheck(InHandle, OutHandle);break;}
						if(strcmp(buf, _cTestTool) == 0){error = Cmd_ToolTest();break;}
						if(strcmp(buf, "cTool") == 0){error = Cmd_SetEquipment(0);break;}
						if(strcmp(buf, "cToolTime") == 0){error = Cmd_SetEquipment(1);break;}
						if(strcmp(buf, _SetDeviceParameter) == 0){error = Cmd_SetDeviceParameter();break;}
						if(strcmp(buf, _GetDeviceParameter) == 0){error = Cmd_GetDeviceParameter();break;}

						if(strcmp(buf, "sStallConfig") == 0){error = Cmd_SetStallExeptionParameter();break;}
						if(strcmp(buf, "gStallConfig") == 0){error = Cmd_GetStallExeptionParameter();break;}
						if(strcmp(buf, "sStallCounter") == 0){error = Cmd_SetStallExeptionCounter();break;}
						if(strcmp(buf, "gStallCounter") == 0){error = Cmd_GetStallExeptionCounter();break;}

//						if(strcmp(buf, "sSearchAdapter") == 0){error = Cmd_SetSearchAdapterRelativeDeltaWay();break;}
//						if(strcmp(buf, "gSearchAdapter") == 0){error = Cmd_GetSearchAdapterRelativeDeltaWay();break;}

						if(strcmp(buf, "cTestFw") == 0){error = TestForFirmware();break;}
						if(strcmp(buf, _ClearEstop) == 0){error = Cmd_ClearEstop(0);break;}
						if(strcmp(buf, _SetAppSpd) == 0){error = Cmd_SetApplicationSpeed();break;}
						if(strcmp(buf, _GetAppSpd) == 0){error = Cmd_GetApplicationSpeed();break;}

						if(strcmp(buf, _GetDoorLiftState) == 0){error = Cmd_GetDoorLiftState();break;}
						if(strcmp(buf, _GetDoorLiftHomingState) == 0){error = Cmd_GetDoorLiftHomingState();break;}
						if(strcmp(buf, _GetDoorLiftCloseState) == 0){error = Cmd_GetDoorLiftCloseState();break;}
						if(strcmp(buf, _GetDoorLiftStatus) == 0){error = Cmd_GetDoorLiftStatus();break;}
						if(strcmp(buf, _GetDoorLiftPosition) == 0){error = Cmd_GetDoorAbsolutePosiion();break;}
						if(strcmp(buf, _DoorLiftOpen) == 0){error = Cmd_DoorLiftOpen(0);break;}
						if(strcmp(buf, _DoorLiftClose) == 0){error = Cmd_DoorLiftClose(0);break;}
						if(strcmp(buf, _DoorLiftOpenSync) == 0){error = Cmd_DoorLiftOpen(1);break;}
						if(strcmp(buf, _DoorLiftCloseSync) == 0){error = Cmd_DoorLiftClose(1);break;}
						if(strcmp(buf, _DoorLiftAdj) == 0){error = Cmd_DoorLiftAdj(0);break;}
						if(strcmp(buf, _DoorLiftAdjust) == 0){error = Cmd_DoorLiftAdj(1);break;}
						if(strcmp(buf, _SetDoorLiftSpd) == 0){error = Cmd_SetDoorLiftSpd(0);break;}
						if(strcmp(buf, _GetDoorLiftSpd) == 0){error = Cmd_GetDoorLiftSpd(0);break;}
						if(strcmp(buf, _SetDoorLiftAdjSpd) == 0){error = Cmd_SetDoorLiftAdjustSpd(0);break;}
						if(strcmp(buf, _GetDoorLiftAdjSpd) == 0){error = Cmd_GetDoorLiftAdjustSpd(0);break;}
						if(strcmp(buf, _SetDoorLiftMaxWay) == 0){error = Cmd_SetDoorLiftMaxWay();break;}
						if(strcmp(buf, _GetDoorLiftMaxWay) == 0){error = Cmd_GetDoorLiftMaxWay();break;}
						if(strcmp(buf, _SetDoorLiftOpenPos) == 0){error = Cmd_SetDoorLiftOpenPos();break;}
						if(strcmp(buf, _GetDoorLiftOpenPos) == 0){error = Cmd_GetDoorLiftOpenPos();break;}
						if(strcmp(buf, _SetDoorLiftClosePos) == 0){error = Cmd_SetDoorLiftClosePos();break;}
						if(strcmp(buf, _GetDoorLiftClosePos) == 0){error = Cmd_GetDoorLiftClosePos();break;}
						if(strcmp(buf, _SetDoorLiftOffset) == 0){error = Cmd_SetDoorLiftOffset();break;}
						if(strcmp(buf, _GetDoorLiftOffset) == 0){error = Cmd_GetDoorLiftOffset();break;}
						if(strcmp(buf, _ResetDoorLiftError) == 0){error = Cmd_ResetDoorLiftError();break;}
						if(strcmp(buf, _GetDoorLiftError) == 0){error = Cmd_GetDoorLiftError();break;}
						if(strcmp(buf, _ResetDoorLiftOEMError) == 0){error = Cmd_ResetDoorLiftOEMError();break;}
						if(strcmp(buf, _GetDoorLiftOEMError) == 0){error = Cmd_GetDoorLiftOEMError();break;}
						//if(strcmp(buf, _ResetDoorLiftHWError) == 0){error = Cmd_ResetDoorLiftHWError();break;}
						if(strcmp(buf, _GetDoorLiftHWError) == 0){error = Cmd_GetDoorLiftHWError();break;}
						//if(strcmp(buf, _SetDoorLifterCurrent) == 0){error = Cmd_DoorLifterWriteCurrent(eSET_CURRENT, 0);break;}
						//if(strcmp(buf, _GetDoorLifterCurrent) == 0){error = Cmd_DoorLifterReadCurrent(eGET_CURRENT, 0);break;}
						if(strcmp(buf, _SetDoorLifterDrvCurrent) == 0){error = Cmd_DoorLifterWriteCurrent(eSET_RUN_CURRENT, 0);break;}
						if(strcmp(buf, _GetDoorLifterDrvCurrent) == 0){error = Cmd_DoorLifterReadCurrent(eGET_RUN_CURRENT, 0);break;}
						if(strcmp(buf, _SetDoorLifterHoldCurrent) == 0){error = Cmd_DoorLifterWriteCurrent(eSET_HOLD_CURRENT, 0);break;}
						if(strcmp(buf, _GetDoorLifterHoldCurrent) == 0){error = Cmd_DoorLifterReadCurrent(eGET_HOLD_CURRENT, 0);break;}

						if(strcmp(buf, _SetDoorLiftSpdFRAM) == 0){error = Cmd_SetDoorLiftSpd(1);break;}
						if(strcmp(buf, _GetDoorLiftSpdFRAM) == 0){error = Cmd_GetDoorLiftSpd(1);break;}
						if(strcmp(buf, _SetDoorLiftAdjSpdFRAM) == 0){error = Cmd_SetDoorLiftAdjustSpd(1);break;}
						if(strcmp(buf, _GetDoorLiftAdjSpdFRAM) == 0){error = Cmd_GetDoorLiftAdjustSpd(1);break;}
						if(strcmp(buf, _SetDoorLifterDrvCurrentFRAM) == 0){error = Cmd_DoorLifterWriteCurrent(eSET_RUN_CURRENT, 1);break;}
						if(strcmp(buf, _GetDoorLifterDrvCurrentFRAM) == 0){error = Cmd_DoorLifterReadCurrent(eGET_RUN_CURRENT, 1);break;}
						if(strcmp(buf, _SetDoorLifterHoldCurrentFRAM) == 0){error = Cmd_DoorLifterWriteCurrent(eSET_HOLD_CURRENT, 1);break;}
						if(strcmp(buf, _GetDoorLifterHoldCurrentFRAM) == 0){error = Cmd_DoorLifterReadCurrent(eGET_HOLD_CURRENT, 1);break;}
						if(strcmp(buf, _DoorLiftOEM) == 0){error = Cmd_DoorLifterOEM();break;}

		default:		error = ERR_CMD_NOT_IMPLEMENTED;
						break;
	}
	#ifdef _CF_ESTOP_DEBUG_PRINT
	if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("CF_8: %u\r", error);}
	#endif

	if((error == (unsigned short)ERR_SAFETY_APPLIANCE) && (m_EStopCmdState == ESTOPSTATE_RESET))
	{
		#ifdef _CF_ESTOP_DEBUG_PRINT
		if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("CF_8b\r");}
		#endif
		SaveScannerBufferForReload();
		m_EStopCmdState = ESTOPSTATE_WAIT;
	}
	return error;
}
unsigned short Cmd_ClearEstopExe(unsigned short NoRestore)
{
	unsigned short error = errNone;
	unsigned short Option = 0;
	unsigned short State;
	unsigned short notEndless;
	char szHippieAnswer[64];

	#ifdef _CF_ESTOP_DEBUG_PRINT
	if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("CEE_1\r");}
	#endif
	if(!NoRestore)
	{
		if(scanUShort(&Option) != errNone)
		{
			#ifdef _CF_ESTOP_DEBUG_PRINT
			if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("CEE_2\r");}
			#endif
			Option = 1;
		}
	}
	else
	{
		Option = 2;
	}
	if(g_isGenerateEStopError())
	{
		#ifdef _CF_ESTOP_DEBUG_PRINT
		if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("CEE_3\r");}
		#endif

		notEndless = 100;
		do
		{
			WDO();
			K_Task_Wait(100);
			--notEndless;
			if(g_MsgCheckEstopState(&State) != errNone)
			{
				#ifdef _CF_ESTOP_DEBUG_PRINT
				if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("CEE_4\r");}
				#endif

				return error;
			}

		}while((State != ESTOP_OFF) && notEndless);
		#ifdef _CF_ESTOP_DEBUG_PRINT
		if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("CEE_5\r");}
		#endif

		if(!notEndless)
		{
			#ifdef _CF_ESTOP_DEBUG_PRINT
			if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("CEE_6\r");}
			#endif

			return ERR_COVER_OPEN;
		}

		if((State == ESTOP_OFF) && ESTOP)
		{
			#ifdef _CF_ESTOP_DEBUG_PRINT
			if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("CEE_7\r");}
			#endif

			if(isNRTL_LP())
			{
				#ifdef _CF_ESTOP_DEBUG_PRINT
				if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("CEE_NRTL_8\r");}
				#endif

				if((error = g_MsgCheckMOSFETSwitch()) == ERR_SAFETY_MONITORING)
				{
					//m_EnableMovCmd = FALSE;
					/*PRINT_ALLOC
					fprintf(stdout, "E5CEE_5 %u ",error);
					fflush(stdout);
					PRINT_RELEASE*/
					#ifdef _CF_ESTOP_DEBUG_PRINT
					if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("CEE_NRTL_9\r");}
					#endif
					DebugPosErrReset();
					#ifdef _CF_ESTOP_DEBUG_PRINT
					if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("CEE_NRTL_9a\r");}
					#endif

					return error;
				}
			}
			#ifdef _CF_ESTOP_DEBUG_PRINT
			if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("CEE_10\r");}
			#endif
			if(!g_isEstopFlag())
			{
			g_ClearGenerateEStopError();
			ClearEStopAnswerFromLPK();
			}
			#ifdef _CF_ESTOP_DEBUG_PRINT
			if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("CEE_11\r");}
			#endif

			if((error = g_ASync_ResetError(0, eAllMask)) != errNone)

			{
				#ifdef _CF_ESTOP_DEBUG_PRINT
				if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("CEE_12\r");}
				#endif
				return error;

			}
			#ifdef _CF_ESTOP_DEBUG_PRINT
			if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("CEE_13\r");}
			#endif

			if(Option == 1)
			{
				#ifdef _CF_ESTOP_DEBUG_PRINT
				if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("CES_14\r");}
				#endif
				if((error = g_ASync_EStopMoveSavetyPos()) != errNone)
				{
					#ifdef _CF_ESTOP_DEBUG_PRINT
					if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("CEE_15\r");}
					#endif
					return error;
				}
				#ifdef _CF_ESTOP_DEBUG_PRINT
				if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("CEE_16\r");}
				#endif

				if((error = g_ASync_EStopMoveRestorePos()) != errNone)
				{
					#ifdef _CF_ESTOP_DEBUG_PRINT
					if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("CEE_17\r");}
					#endif
					return error;
				}
				#ifdef _CF_ESTOP_DEBUG_PRINT
				if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("CEE_18\r");}
				#endif

				//if((error = g_ASync_EStopMoveContinue(&szHippieAnswer)) != errNone)
				error = g_ASync_EStopMoveContinue(szHippieAnswer);
				if((error != errNone) && (error != errInfo))
				{
					#ifdef _CF_ESTOP_DEBUG_PRINT
					if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("CEE_19\r");}
					#endif
					return error;
				}
			}
			if(Option == 0)
			{
				#ifdef _CF_ESTOP_DEBUG_PRINT
				if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("CEE_20\r");}
				#endif
				error = g_ASync_EStopRestoreCurrentAndSetPos();
				#ifdef _CF_ESTOP_DEBUG_PRINT
				if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("CEE_21\r");}
				#endif

			}
		}
		else
		{
			error = (unsigned short)ERR_SAFETY_APPLIANCE;
			#ifdef _CF_ESTOP_DEBUG_PRINT
			if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("CEE_22\r");}
			#endif


		}
	}
	#ifdef _CF_ESTOP_DEBUG_PRINT
	if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("CEE_23 %u\r", error);}
	#endif
	return error;
}

unsigned short Cmd_ClearEstop(unsigned short NoRestore)
{
	unsigned short error = errNone;
	error = Cmd_ClearEstopExe(NoRestore);
	if(error != (unsigned short)ERR_SAFETY_APPLIANCE)
	{
		#ifdef _CF_ESTOP_DEBUG_PRINT
		if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("CE_1\r");}
		#endif
		m_EStopCmdState = ESTOPSTATE_RESET;

		if(error != errNone)
		{
			#ifdef _CF_ESTOP_DEBUG_PRINT
			if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("CE_2\r");}
			#endif
			g_Cmd_ASync_ResetEstopMove();
			#ifdef _CF_ESTOP_DEBUG_PRINT
			if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("CE_3\r");}
			#endif

		}
	}
	else
	{
		//if(error == (unsigned short)ERR_SAFETY_APPLIANCE){m_EStopCmdState = ESTOPSTATE_WAIT;}
		#ifdef _CF_ESTOP_DEBUG_PRINT
		if(g_TestFlag & TF_DBG_ESTOP_CMD){dbPrint("CE_4\r");}
		#endif

	}
	return error;


}

unsigned short Command_K_Extension(void)
{
	unsigned short ScannerIdent;
	char buf[5];
	unsigned short error;
	error = ERR_CMD_NOT_IMPLEMENTED;
	ScannerIdent = scanner(buf);
	switch(ScannerIdent)
	{
		case IDENT  :
						if(strcmp(buf, CSTR_READ_SENSOR) == 0){error = Cmd_ReadSensor();break;}
		default:		error = ERR_CMD_NOT_IMPLEMENTED;
						break;
	}
	return error;
}

unsigned short Cmd_Init(FILE *OutHandle)
{
//	char answer[25];
//	char command[25];
	unsigned short error = errNone;
	unsigned short i, Option;
	char strHelp[5];
	if(isFreeHandZMoveMode())
	{
		return ERR_CMD_NOT_PRACTICABLE;
	}
	m_EnableMovCmd = TRUE;
	//LED_Green_On();
	LED_Green_SLowBlink();
	ClearMode1536();
	ParallelCmdClear();
	SetHostMode();
	if(GetLPKConnected())
	{
		SetLPKConnect();
	}
	else
	{
		ClearLPKConnect();
	}

error = g_Cmd_ASync_ResetEstopMove();

	if(g_TestFlag & TF_GENERAL)
	{
		BarcodeReaderShowTypAndVersion(OutHandle, TRUE);
	}
	if(scanUShort(&Option) != errNone)
	{
		Option = 0;
	}
	if(isDeviceInitError())
	{
		for(i = 0; i < (MAX_AXES + MAX_CNT_ISTATE); i++)
		{
			WDO();
			if(g_errArray[i] != errNone)
			{
				switch(g_errArray[i])
				{
					case istate_MotorInitAxisBase :	error = ERR_OEM_HARDWARE;
													break;
					case istate_MotorEStop :		error = ERR_HARDWARE;
													break;
					case istate_MotorJalousie :		error = ERR_SAFETY_MONITORING;//bis 4.0 ERR_HARDWARE
													break;
					//#ifdef _CF_NRTL
					case istate_MotorExternEstop :		error = ERR_SAFETY_MONITORING;
													break;
					case istate_MotorFETEstopL :		error = ERR_SAFETY_MONITORING;
													break;
					case istate_MotorFETEstopH :		error = ERR_SAFETY_MONITORING;
													break;
					//#endif
					case istate_FRAMStructVersion : error = errDataStructVersion;
													break;
					case istate_FRAMReadStructVersion	:
					case istate_FRAMReadDeviceConfig	:
					case istate_FRAMReadAxisOffset		:
					case istate_FRAMReadAxisMinMaxWay	:
					case istate_FRAMReadEstopCounter	:
					case istate_FRAMReadSensorConfig	:
					case istate_FRAMReadPortConfig		:
					case istate_FRAMReadEquipmentConfig	:
					case istate_FRAMReadEstopWait		:
					case istate_FRAMReadConfigStallExeption	:
					case istate_FRAMReadSearchAdapterRelativeDeltaWay:
					case istate_FRAMReadApplicationSpeed:
					case istate_FRAMReadCounterStallExeption: error = errCheckSum;
														break;
					case istate_FRAM_AutoChangeError_MinMaxWay		:
					case istate_FRAM_AutoChangeError_SensorConfig	:
					case istate_FRAM_AutoChangeError_PortConfig		:
					case istate_FRAM_AutoChangeError_EquipmentConfig:
					case istate_FRAM_AutoChangeError_StructVersion	:
					case istate_FRAM_AutoChangeError_EstopCounter	:
					case istate_FRAM_AutoChangeError_EstopWait		:
					case istate_FRAM_AutoChangeError_ConfigStallExeption	:
					case istate_FRAM_AutoChangeError_CounterStallExeption:
					case istate_FRAM_AutoChangeError_SearchAdapterRelativeDeltaWay:
					case istate_FRAM_AutoChangeError_ApplicationSpeed:
					case istate_FRAM_AutoChangeError_DoorLiftConfig	: error = errDataUpdate;
														break;
					case istate_ImsCmd_Sync : error = ERR_485_BUS_GENERIC;
													break;
					case istate_DoorLiftMotorInit : error = ERR_DRV1_COMM;
													break;
					default:					error = ERR_SYSTEM;
													break;
				}
			}
		}
	}
	if(error != errNone)
	{
		if(Option == 66)
		{
			sprintf(g_acAnswerBuf,"E%u ", error);
			strHelp[0]=0;
			for(i = 0; i < (MAX_AXES + MAX_CNT_ISTATE); i++)
			{
				WDO();
				if(g_errArray[i] != errNone)
				{
					sprintf(strHelp,"%u ", g_errArray[i]);
					strcat(g_acAnswerBuf,strHelp);
				}
			}
			g_acAnswerBuf[strlen(g_acAnswerBuf)-1]=0; // Letztes Leerzeichen ausblenden
		  	strcat(g_acAnswerBuf,"\r");
			return errInfo;
		}
	}
	else
	{
		if(gDoppelTasteUI)
		{
			if(g_EquipmentConfig.EquipmentConfig & BIT_E_MuellFoerderer)
			{
				if(!MUELL_IN)
				{
					error = ERR_NO_WASTE_BOX;
				}
			}
		}
	}
	return error;
}

unsigned short Cmd_Exit(void)
{
	unsigned short error = errNone;
	m_EnableMovCmd = FALSE;
	ClearHostMode();
	ClearMode1536();
	if(gDoppelTasteUI){PortQuitSetting();}
	LED_Green_On();
	g_Cmd_ASync_ResetEstopMove();
	return error;
}


// #######################################################################################################################
// #######################################################################################################################

unsigned short CmdGetDeviceName(void)
{
	sprintf(g_acAnswerBuf,"I%s\r", DeviceNameUSB);
	return errInfo;
}

unsigned short CmdGetVersion(void)
{
	//sprintf(g_acAnswerBuf,"I%s %s HW%u%u %s %s %s\r", DeviceName, FirmwareVersion, gDoppelTasteUI, gNrtl_LP, g_pStrCopyright, FW_Date, FW_Time);
	sprintf(g_acAnswerBuf,"I%s %s HW%u%u %s %s %s\r", DeviceName, FirmwareVersion, gDoppelTasteUI, isNRTL_LP(), g_pStrCopyright, FW_Date, FW_Time);
	return errInfo;
}

/// \brief             Konvertierung Monatstring in Short
/// \param         Pointer auf Monatsstring
/// \return         short
short ConvertMonth(const char* sptr)
{
	int i;
	const char* const MonthTab[]= {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
	for(i=0; i< 12; i++)
	{
		if(strncmp(sptr, MonthTab[i], 3) == 0)
		break;
	}
	return i+1;
}

/// \brief             Vorbereitung und Kommunikation mit Slave
/// \param         date      - Pointer auf zu konvertierendes Datum (z.B. "Sep 29 2015")
/// \param         dest_date - Pointer auf ZK-Buffer des konvertierten Datums (z.B. "2015-09-29")
void ConvertDate(const char *const date, char* dest_date)
{
	short year, month, day;
	char *cptr;
	month= ConvertMonth(date);
	cptr= strchr(date, ' '); cptr++;
	day= atoi(cptr);
	cptr= strchr(cptr, ' '); cptr++;
	year= atoi(cptr);
	sprintf(dest_date, "%04d-%02d-%02d", year, month, day);  
}

/// \brief             Kommando Lesen der erweiterten Versionsnummer (Versionsnummer + Timestamp)
/// \return         INFO_FLAG
unsigned short CmdGetVersionNumber(void)
{
	char tmp[12];
	ConvertDate(&FW_Date[0], tmp);
		//static const char *const m_paStrVersionDate =__DATE__;
		//static const char *const m_paStrVersionTime =__TIME__;
		//static const char *const m_paStrVersionsNo  = "V0.03";
	//ConvertDate(m_paStrVersionDate, tmp);
	//sprintf(g_acAnswerBuf,"I%s.%s %sT%s\r", m_paStrVersionsNo+1, m_paStrVersionsFix, tmp, m_paStrVersionTime);
	sprintf(g_acAnswerBuf,"I%s %sT%s\r", &FirmwareVersion[1], tmp, &FW_Time);
	return errInfo;
}

unsigned short Cmd_Drv_SetSpeed(void)
{
	unsigned short error = errNone;
	unsigned short Mask;
	unsigned short ID;
	unsigned short idx;
	unsigned short i;
	//float Start;
	float End;
	float Acc;
	// Startspeed ist optional
	if((error = scanUShort(&Mask)) != errNone) return error;
	if(!ConfirmMask(Mask)) return ERR_WRONG_PARAMETER;
	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if((ID = GetAxisIDFromMask(Mask, g_FelixAxes[i].AxisBit)) != 0)
		{
			idx = GetAxisIndex(ID);
			if(idx < 0)
			{
				return ERR_UNKNOWN_DRV;
			}
			//if((error = scanFloat(&Start)) != errNone) return error;//Start oder End
			if((error = scanFloat(&End)) != errNone) return error;// End oder Acceleration
			if((error = scanFloat(&Acc)) != errNone) return error;// End oder Acceleration
			/*if(scanFloat(&Acc) != errNone)
			{
				Acc = End;
				End = Start;
				Start = (float)-1.0;
			}*/

			if((error = MotorSetSpeedExe( ID, (float)-1.0, End, Acc)) != errNone) return error;
		}
	}
	return error;
}

//unsigned short Cmd_Drv_GetSpeed(char *sPtr)
unsigned short Cmd_Drv_GetSpeed(void)
{
	unsigned short error;
	unsigned short i;
	char strHelp[30];
	unsigned short ID;
	unsigned short Mask;
	udtSpeedParaDevice Speed;
	unsigned short Scr;
	unsigned short Start;

	if((error = scanUShort(&Mask)) != errNone) return error;
	if(!ConfirmMask(Mask)) return ERR_WRONG_PARAMETER;
	if(scanUShort(&Scr) != errNone)
	{
		Scr = 0;// von Firmware lesen, 1 = von Hardware lesen
	}
	if(scanUShort(&Start) != errNone)
	{
		Start = 0;// ohne Startspeed
	}
	sprintf(g_acAnswerBuf,"I"); strHelp[0]=0;

	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if((ID = GetAxisIDFromMask(Mask, g_FelixAxes[i].AxisBit)) != 0)
		{
			if((error = MotorGetSpeedExe( ID, &Speed, Scr)) != errNone) return error;
			if(Start)
			{
				sprintf(strHelp,"%3.1f %3.1f %3.1f ",Speed.Start, Speed.End, Speed.Acc);
			}
			else
			{
				sprintf(strHelp,"%3.1f %3.1f ",Speed.End, Speed.Acc);
			}
			strcat(g_acAnswerBuf,strHelp);
		}
	}
	g_acAnswerBuf[strlen(g_acAnswerBuf)-1]=0; // Letztes Leerzeichen ausblenden
  	strcat(g_acAnswerBuf,"\r");
	return errInfo;
}

unsigned short Cmd_Drv_SetSonderSpeed(unsigned short Funktion)
{
	unsigned short error = errNone;
	unsigned short Mask;
	unsigned short ID;
	unsigned short idx;
	unsigned short i;
	float End;
	float Acc;
	// Startspeed ist optional
	if((error = scanUShort(&Mask)) != errNone) return error;
	if(!ConfirmMask(Mask)) return ERR_WRONG_PARAMETER;
	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if((ID = GetAxisIDFromMask(Mask, g_FelixAxes[i].AxisBit)) != 0)
		{
			idx = GetAxisIndex(ID);
			if(idx < 0)
			{
				return ERR_UNKNOWN_DRV;
			}
			if((error = scanFloat(&End)) != errNone) return error;//Start oder End
			if((error = scanFloat(&Acc)) != errNone) return error;// End oder Acceleration
			if((error = MotorSetSonderSpeedExe( Funktion, ID, End, Acc)) != errNone) return error;
		}
	}
	return error;
}

//unsigned short Cmd_Drv_GetSonderSpeed(unsigned short Funktion, char *sPtr)
unsigned short Cmd_Drv_GetSonderSpeed(unsigned short Funktion)
{
	unsigned short error;
	unsigned short i;
	char strHelp[30];
	unsigned short ID;
	unsigned short Mask;
	udtSpeedParaDevice Speed;

	if((error = scanUShort(&Mask)) != errNone) return error;
	if(!ConfirmMask(Mask)) return ERR_WRONG_PARAMETER;

	sprintf(g_acAnswerBuf,"I"); strHelp[0]=0;

	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if((ID = GetAxisIDFromMask(Mask, g_FelixAxes[i].AxisBit)) != 0)
		{
			if((error = MotorGetSonderSpeedExe( Funktion, ID, &Speed)) != errNone) return error;
			sprintf(strHelp,"%4.1f %4.1f ",Speed.End, Speed.Acc);
			strcat(g_acAnswerBuf,strHelp);
		}
	}
	g_acAnswerBuf[strlen(g_acAnswerBuf)-1]=0; // Letztes Leerzeichen ausblenden
  	strcat(g_acAnswerBuf,"\r");
	return errInfo;
}


//unsigned short Cmd_Drv_GetSpeedRange(char *sPtr)
unsigned short Cmd_Drv_GetSpeedRange(void)
{
	unsigned short error;
	unsigned short i;
	char strHelp[100];
	unsigned short ID;
	unsigned short Mask;
	short idx;

	if((error = scanUShort(&Mask)) != errNone) return error;
	if(!ConfirmMask(Mask)) return ERR_WRONG_PARAMETER;

	sprintf(g_acAnswerBuf,"I"); strHelp[0]=0;

	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if((ID = GetAxisIDFromMask(Mask, g_FelixAxes[i].AxisBit)) != 0)
		{
			idx = GetAxisIndex(ID);
			if(idx < 0)
			{
				return ERR_UNKNOWN_DRV;
			}
			sprintf(strHelp,"%3.1f %3.1f %3.1f %3.1f %3.1f %3.1f %3.1f %3.1f ", pIMSFixedPara[idx]->fMinSpeed
			, pIMSFixedPara[idx]->fMaxSpeed, pIMSFixedPara[idx]->fMaxAcceleration, pIMSFixedPara[idx]->fMinAcceleration
			, pIMSFixedPara[idx]->fStartSpeed, pIMSFixedPara[idx]->fEndSpeed, pIMSFixedPara[idx]->fAcceleration, pIMSFixedPara[idx]->fHomingSpeed);
			strcat(g_acAnswerBuf,strHelp);
		}
	}
	g_acAnswerBuf[strlen(g_acAnswerBuf)-1]=0; // Letztes Leerzeichen ausblenden
  	strcat(g_acAnswerBuf,"\r");
	return errInfo;
}

unsigned short Cmd_Drv_GetMaxSpeedRange(void)
{
	unsigned short error;
	unsigned short i;
	char strHelp[100];
	unsigned short ID;
	unsigned short Mask;
	short idx;

	if((error = scanUShort(&Mask)) != errNone) return error;
	if(!ConfirmMask(Mask)) return ERR_WRONG_PARAMETER;

	sprintf(g_acAnswerBuf,"I"); strHelp[0]=0;

	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if((ID = GetAxisIDFromMask(Mask, g_FelixAxes[i].AxisBit)) != 0)
		{
			idx = GetAxisIndex(ID);
			if(idx < 0)
			{
				return ERR_UNKNOWN_DRV;
			}
			sprintf(strHelp,"%3.1f %3.1f ", pIMSFixedPara[idx]->fMaxSpeed, pIMSFixedPara[idx]->fMaxAcceleration);
			strcat(g_acAnswerBuf,strHelp);
		}
	}
	g_acAnswerBuf[strlen(g_acAnswerBuf)-1]=0; // Letztes Leerzeichen ausblenden
  	strcat(g_acAnswerBuf,"\r");
	return errInfo;
}

unsigned short Cmd_Drv_SetAccelWay(void)
{
	unsigned short error = errNone;
	unsigned short Mask;
	unsigned short ID;
	unsigned short IX;
	unsigned short idx;
	unsigned short i;
	float Way;
	float EndSpeed = 1;
	float StartSpeed, Accel;

	if((error = scanUShort(&Mask)) != errNone) return error;
	if(!ConfirmMask(Mask)) return ERR_WRONG_PARAMETER;
	if((error = scanUShort(&IX)) != errNone) return error;
	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if((ID = GetAxisIDFromMask(Mask, g_FelixAxes[i].AxisBit)) != 0)
		{
			idx = GetAxisIndex(ID);
			if(idx < 0)
			{
				return ERR_UNKNOWN_DRV;
			}
			if((error = scanFloat(&Way)) != errNone) return error;
			StartSpeed = pIMSChangedPara[idx]->fStartSpeed;
			switch(IX)
			{
				case 1:	//error = MotorSetWayAcceleration( ID, Way);
						EndSpeed = pIMSChangedPara[idx]->fEndSpeed;
						break;
				case 2:	//error = MotorSetWayHomingAcceleration( ID, Way);
						EndSpeed = pIMSChangedPara[idx]->fHomingSpeed;
						break;
				default:	error = ERR_WRONG_PARAMETER;
						break;
			}
			if(error != errNone) return error;
			Accel = ((EndSpeed * EndSpeed) - (StartSpeed * StartSpeed)) / (2 * Way);
			switch(IX)
			{
				case 1:	//if((error = MotorSetAcceleration( ID, Accel)) != errNone) return error;
						if((error = ImsCmd_SetSpeed( ID, (float)-1.0, pIMSChangedPara[idx]->fEndSpeed, Accel)) != errNone) return error;
						pIMSChangedPara[idx]->fAcceleration = Accel;
						break;
				case 2:	pIMSChangedPara[idx]->fAccelerationHoming = Accel;
						break;
				default:	error = ERR_WRONG_PARAMETER;
						break;
			}
		}
	}
	return error;
}
unsigned short Cmd_Drv_GetAccelWay(void)
{
	unsigned short error;
	unsigned short i;
	char strHelp[30];
	unsigned short ID;
	unsigned short idx;
	unsigned short Mask;
	unsigned short Scr;
	unsigned short IX;
	float Way;
	float EndSpeed = 1;
	float StartSpeed, Accel;


	if((error = scanUShort(&Mask)) != errNone) return error;
	if(!ConfirmMask(Mask)) return ERR_WRONG_PARAMETER;
	if((error = scanUShort(&IX)) != errNone) return error;
	if(scanUShort(&Scr) != errNone)
	{
		Scr = 0;// von Firmware lesen, 1 = von Hardware lesen
	}
	sprintf(g_acAnswerBuf,"I"); strHelp[0]=0;

	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if((ID = GetAxisIDFromMask(Mask, g_FelixAxes[i].AxisBit)) != 0)
		{
			idx = GetAxisIndex(ID);
			if(idx < 0)
			{
				return ERR_UNKNOWN_DRV;
			}
			StartSpeed = pIMSChangedPara[idx]->fStartSpeed;
			switch(IX)
			{
				case 1:	//error = MotorGetWayAcceleration( ID, &Way, Scr);
						if(Scr)
						{
							//if((error = MotorGetAcceleration( ID, &Accel)) != errNone) return error;
							if((error = ImsCmd_GetSpeed( ID, &StartSpeed, &EndSpeed, &Accel)) != errNone) return error;
						}
						else
						{
							Accel = pIMSChangedPara[idx]->fAcceleration;
							EndSpeed = pIMSChangedPara[idx]->fEndSpeed;
						}
						break;
				case 2:	//error = MotorGetWayHomingAcceleration( ID, &Way);
						EndSpeed = pIMSChangedPara[idx]->fHomingSpeed;
						Accel = pIMSChangedPara[idx]->fAccelerationHoming;
						break;
				default:	error = ERR_WRONG_PARAMETER;
						break;
			}
			if(error != errNone) return error;
			Way = ((EndSpeed * EndSpeed) - (StartSpeed * StartSpeed)) / (2 * Accel);
			sprintf(strHelp,"%3.2f ", Way);
			strcat(g_acAnswerBuf,strHelp);
		}
	}
	g_acAnswerBuf[strlen(g_acAnswerBuf)-1]=0; // Letztes Leerzeichen ausblenden
  	strcat(g_acAnswerBuf,"\r");
	return errInfo;
}

unsigned short Cmd_WriteCurrent(const udtMOTOR_CURRENT_Type CurrentType)
{
	unsigned short error = errNone;
	unsigned short Mask;
	unsigned short ID;
	unsigned short idx;
	unsigned short i;
	unsigned short value;

	if((error = scanUShort(&Mask)) != errNone) return error;
	if(!ConfirmMask(Mask)) return ERR_WRONG_PARAMETER;
	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if((ID = GetAxisIDFromMask(Mask, g_FelixAxes[i].AxisBit)) != 0)
		{
			idx = GetAxisIndex(ID);
			if(idx < 0)
			{
				return ERR_UNKNOWN_DRV;
			}
			if((error = scanUShort(&value)) != errNone) return error;
			switch(CurrentType)
			{
				case eSET_CURRENT:
									error = MotorSetHoldCurrent( ID, value);
									break;
				case eSET_RUN_CURRENT:
									error = MotorSetRunCurrent( ID, value);
									break;
				case eSET_HOLD_CURRENT:
									error = MotorSetHoldCurrent( ID, value);
									break;
				default:			error = ERR_WRONG_PARAMETER;
									break;
			}
			if(error != errNone) return error;
		}
	}
	return error;
}

//unsigned short Cmd_ReadCurrent(const udtMOTOR_CURRENT_Type CurrentType, char *sPtr)
unsigned short Cmd_ReadCurrent(const udtMOTOR_CURRENT_Type CurrentType)
{
	unsigned short error;
	unsigned short i;
	char strHelp[30];
	unsigned short ID;
	unsigned short Mask;
	unsigned short Scr;
	unsigned short value;

	if((error = scanUShort(&Mask)) != errNone) return error;
	if(!ConfirmMask(Mask)) return ERR_WRONG_PARAMETER;
	if(scanUShort(&Scr) != errNone)
	{
		Scr = 0;// von Firmware lesen, 1 = von Hardware lesen
	}
	sprintf(g_acAnswerBuf,"I"); strHelp[0]=0;

	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if((ID = GetAxisIDFromMask(Mask, g_FelixAxes[i].AxisBit)) != 0)
		{
			switch(CurrentType)
			{
				case eGET_CURRENT:
									error = MotorGetHoldCurrent( ID, &value, 0);
								break;
				case eGET_RUN_CURRENT:
									error = MotorGetRunCurrent( ID, &value, Scr);
								break;
				case eGET_HOLD_CURRENT:
									error = MotorGetHoldCurrent( ID, &value, Scr);
								break;
				default:			error = ERR_WRONG_PARAMETER;
								break;
			}
			if(error != errNone) return error;
			sprintf(strHelp,"%u ",value);
			strcat(g_acAnswerBuf,strHelp);
		}
	}
	g_acAnswerBuf[strlen(g_acAnswerBuf)-1]=0; // Letztes Leerzeichen ausblenden
  	strcat(g_acAnswerBuf,"\r");
	return errInfo;
}


unsigned short Cmd_Drv_SetPrecision(void)
{
	unsigned short error = errNone;
	unsigned short Mask;
	unsigned short ID;
	unsigned short idx;
	unsigned short i;
	float value;

	if((error = scanUShort(&Mask)) != errNone) return error;
	if(!ConfirmMask(Mask)) return ERR_WRONG_PARAMETER;
	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if((ID = GetAxisIDFromMask(Mask, g_FelixAxes[i].AxisBit)) != 0)
		{
			idx = GetAxisIndex(ID);
			if(idx < 0)
			{
				return ERR_UNKNOWN_DRV;
			}
			if((error = scanFloat(&value)) != errNone) return error;
			if((error = MotorSetPrecision( ID, value)) != errNone) return error;
		}
	}
	return error;
}

//unsigned short Cmd_Drv_GetPrecision(char *sPtr)
unsigned short Cmd_Drv_GetPrecision(void)
{
	unsigned short error = errNone;
	unsigned short i;
	char strHelp[20];
	unsigned short ID;
	unsigned short Mask;
	//float value;
	double dvalue;

	if((error = scanUShort(&Mask)) != errNone) return error;
	if(!ConfirmMask(Mask)) return ERR_WRONG_PARAMETER;

	sprintf(g_acAnswerBuf,"I"); strHelp[0]=0;

	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if((ID = GetAxisIDFromMask(Mask, g_FelixAxes[i].AxisBit)) != 0)
		{
			if((error = MotorGetPrecision( ID, &dvalue)) != errNone) return error;
			sprintf(strHelp,"%3.3f ",dvalue); strcat(g_acAnswerBuf,strHelp);
		}
	}
	g_acAnswerBuf[strlen(g_acAnswerBuf)-1]=0; // Letztes Leerzeichen ausblenden
  	strcat(g_acAnswerBuf,"\r");
	return errInfo;
}


unsigned short Cmd_Drv_SetDeadBand(void)
{
	unsigned short error = errNone;
	unsigned short Mask;
	unsigned short ID;
	unsigned short idx;
	unsigned short i;
	unsigned short value;

	if((error = scanUShort(&Mask)) != errNone) return error;
	if(!ConfirmMask(Mask)) return ERR_WRONG_PARAMETER;
	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if((ID = GetAxisIDFromMask(Mask, g_FelixAxes[i].AxisBit)) != 0)
		{
			idx = GetAxisIndex(ID);
			if(idx < 0)
			{
				return ERR_UNKNOWN_DRV;
			}
			if((error = scanUShort(&value)) != errNone) return error;
			if((error = MotorSetEncoderDeadBand( ID, value)) != errNone) return error;
		}
	}
	return error;
}

//unsigned short Cmd_Drv_GetDeadBand(char *sPtr)
unsigned short Cmd_Drv_GetDeadBand(void)
{
	unsigned short error = errNone;
	unsigned short i;
	char strHelp[20];
	unsigned short ID;
	unsigned short Mask;
	unsigned short value;

	if((error = scanUShort(&Mask)) != errNone) return error;
	if(!ConfirmMask(Mask)) return ERR_WRONG_PARAMETER;
	sprintf(g_acAnswerBuf,"I"); strHelp[0]=0;

	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if((ID = GetAxisIDFromMask(Mask, g_FelixAxes[i].AxisBit)) != 0)
		{
			if((error = MotorGetEncoderDeadBand( ID, &value)) != errNone) return error;
			sprintf(strHelp,"%u ",value); strcat(g_acAnswerBuf,strHelp);
		}
	}
	g_acAnswerBuf[strlen(g_acAnswerBuf)-1]=0; // Letztes Leerzeichen ausblenden
  	strcat(g_acAnswerBuf,"\r");
	return errInfo;
}


unsigned short Cmd_Drv_SetStall(void)
{
	unsigned short error = errNone;
	unsigned short Mask;
	unsigned short ID;
	unsigned short idx;
	unsigned short i;
	unsigned short value;

	if((error = scanUShort(&Mask)) != errNone) return error;
	if(!ConfirmMask(Mask)) return ERR_WRONG_PARAMETER;
	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if((ID = GetAxisIDFromMask(Mask, g_FelixAxes[i].AxisBit)) != 0)
		{
			idx = GetAxisIndex(ID);
			if(idx < 0)
			{
				return ERR_UNKNOWN_DRV;
			}
			if((error = scanUShort(&value)) != errNone) return error;
			if((error = MotorSetStallFactor( ID, value)) != errNone) return error;
		}
	}
	return error;
}

//unsigned short Cmd_Drv_GetStall(char *sPtr)
unsigned short Cmd_Drv_GetStall(void)
{
	unsigned short error = errNone;
	unsigned short i;
	char strHelp[20];
	unsigned short ID;
	unsigned short Mask;
	unsigned short Scr;
	unsigned short value;

	if((error = scanUShort(&Mask)) != errNone) return error;
	if(!ConfirmMask(Mask)) return ERR_WRONG_PARAMETER;
	if(scanUShort(&Scr) != errNone)
	{
		Scr = 0;// von Firmware lesen, 1 = von Hardware lesen
	}
	sprintf(g_acAnswerBuf,"I"); strHelp[0]=0;

	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if((ID = GetAxisIDFromMask(Mask, g_FelixAxes[i].AxisBit)) != 0)
		{
			if((error = MotorGetStallFactor( ID, &value, Scr)) != errNone) return error;
			sprintf(strHelp,"%u ",value); strcat(g_acAnswerBuf,strHelp);
		}
	}
	g_acAnswerBuf[strlen(g_acAnswerBuf)-1]=0; // Letztes Leerzeichen ausblenden
  	strcat(g_acAnswerBuf,"\r");
	return errInfo;
}

unsigned short Cmd_Drv_SetWobbleTime(void)
{
	unsigned short error = errNone;
	unsigned short Mask;
	unsigned short ID;
	unsigned short idx;
	unsigned short i;
	unsigned short value;

	if((error = scanUShort(&Mask)) != errNone) return error;
	if(!ConfirmMask(Mask)) return ERR_WRONG_PARAMETER;
	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if((ID = GetAxisIDFromMask(Mask, g_FelixAxes[i].AxisBit)) != 0)
		{
			idx = GetAxisIndex(ID);
			if(idx < 0)
			{
				return ERR_UNKNOWN_DRV;
			}
			if((error = scanUShort(&value)) != errNone) return error;
			if((error = MotorSetMotorWobbleWaitTime( ID, value)) != errNone) return error;
		}
	}
	return error;
}

//unsigned short Cmd_Drv_GetWobbleTime(char *sPtr)
unsigned short Cmd_Drv_GetWobbleTime(void)
{
	unsigned short error = errNone;
	unsigned short i;
	char strHelp[20];
	unsigned short ID;
	unsigned short Mask;
	unsigned short Scr;
	unsigned short value;

	if((error = scanUShort(&Mask)) != errNone) return error;
	if(!ConfirmMask(Mask)) return ERR_WRONG_PARAMETER;
	if(scanUShort(&Scr) != errNone)
	{
		Scr = 0;// von Firmware lesen, 1 = von Hardware lesen
	}
	sprintf(g_acAnswerBuf,"I"); strHelp[0]=0;

	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if((ID = GetAxisIDFromMask(Mask, g_FelixAxes[i].AxisBit)) != 0)
		{
			if((error = MotorGetMotorWobbleWaitTime( ID, &value, Scr)) != errNone) return error;
			sprintf(strHelp,"%u ",value); strcat(g_acAnswerBuf,strHelp);
		}
	}
	g_acAnswerBuf[strlen(g_acAnswerBuf)-1]=0; // Letztes Leerzeichen ausblenden
  	strcat(g_acAnswerBuf,"\r");
	return errInfo;
}

unsigned short Cmd_Drv_SetHoldCurrentTime(void)
{
	unsigned short error = errNone;
	unsigned short Mask;
	unsigned short ID;
	unsigned short idx;
	unsigned short i;
	unsigned short value;

	if((error = scanUShort(&Mask)) != errNone) return error;
	if(!ConfirmMask(Mask)) return ERR_WRONG_PARAMETER;
	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if((ID = GetAxisIDFromMask(Mask, g_FelixAxes[i].AxisBit)) != 0)
		{
			idx = GetAxisIndex(ID);
			if(idx < 0)
			{
				return ERR_UNKNOWN_DRV;
			}
			if((error = scanUShort(&value)) != errNone) return error;
			if((error = MotorSetHoldCurrentWaitTime( ID, value)) != errNone) return error;
		}
	}
	return error;
}
//unsigned short Cmd_Drv_GetHoldCurrentTime(char *sPtr)
unsigned short Cmd_Drv_GetHoldCurrentTime(void)
{
	unsigned short error = errNone;
	unsigned short i;
	char strHelp[20];
	unsigned short ID;
	unsigned short Mask;
	unsigned short Scr;
	unsigned short value;

	if((error = scanUShort(&Mask)) != errNone) return error;
	if(!ConfirmMask(Mask)) return ERR_WRONG_PARAMETER;
	if(scanUShort(&Scr) != errNone)
	{
		Scr = 0;// von Firmware lesen, 1 = von Hardware lesen
	}
	sprintf(g_acAnswerBuf,"I"); strHelp[0]=0;

	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if((ID = GetAxisIDFromMask(Mask, g_FelixAxes[i].AxisBit)) != 0)
		{
			if((error = MotorGetHoldCurrentWaitTime( ID, &value, Scr)) != errNone) return error;
			sprintf(strHelp,"%u ",value); strcat(g_acAnswerBuf,strHelp);
		}
	}
	g_acAnswerBuf[strlen(g_acAnswerBuf)-1]=0; // Letztes Leerzeichen ausblenden
  	strcat(g_acAnswerBuf,"\r");
	return errInfo;
}
double ConvertAngle(double Value)
{
	long lhelp;
	double fhelp_1, fhelp_2,current_angle;
	//sprintf(strHelp,"%3.2f ",(double)360.000 * modf((Value /(double)360.000), &fhelp_1)); strcat(g_acAnswerBuf,strHelp);
	if(fabs(Value) >= (double)1.00000)
	{
		lhelp = labs((long)Value);
		lhelp = lhelp % (long)360;
	}
	if(lhelp != 0)
	{
		fhelp_1 = Value / (double)360.000;
		fhelp_2 = fhelp_1 - (long)fhelp_1; // Normieren
		current_angle = fhelp_2 * (double)360.000;//aktuelle Pos.(Winkel)(kleiner 360 Grad)
		if(current_angle > (double)0.000) ;
		if(current_angle < (double)0.000) current_angle = current_angle + (double)360.000;
	}
	else
	{
		current_angle = (double)0.0;
	}
	return current_angle;
}
unsigned short Cmd_ReadAbsolutePos(void)
{
	unsigned short error = errNone;
	unsigned short i;
	char strHelp[20];
	unsigned short ID;
	unsigned short Mask;
	//float Value = 0;
	double Value = 0;

	if((error = scanUShort(&Mask)) != errNone) return error;
	if(!ConfirmMask(Mask)) return ERR_WRONG_PARAMETER;

	sprintf(g_acAnswerBuf,"I"); strHelp[0]=0;


	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if((ID = GetAxisIDFromMask(Mask, g_FelixAxes[i].AxisBit)) != 0)
		{
			if((error = MotorGetPosition( ID, &Value)) != errNone) return error;
			/*if(g_FelixAxes[i].AxisBit == eDisk_Axis)
			{
				//sprintf(strHelp,"%3.2f ",(double)360.000 * modf((Value /(double)360.000), &fhelp_1)); strcat(g_acAnswerBuf,strHelp);
				if(fabs(Value) >= (double)1.00000)
				{
					lhelp = labs((long)Value);
					lhelp = lhelp % (long)360;
				}
				if(lhelp != 0)
				{
					fhelp_1 = Value / (double)360.000;
					fhelp_2 = fhelp_1 - (long)fhelp_1; // Normieren
					current_angle = fhelp_2 * (double)360.000;//aktuelle Pos.(Winkel)(kleiner 360 Grad)
					if(current_angle > (double)0.000) ;
					if(current_angle < (double)0.000) current_angle = current_angle + (double)360.000;
				}
				else
				{
					current_angle = (double)0.0;
				}
				sprintf(strHelp,"%3.2f ",current_angle); strcat(g_acAnswerBuf,strHelp);
			}
			else
			{
				sprintf(strHelp,"%3.2f ",Value); strcat(g_acAnswerBuf,strHelp);
			}*/
			if(g_FelixAxes[i].AxisFullRotary == 1)
			{
				Value = ConvertAngle(Value);
			}
			sprintf(strHelp,"%3.2f ",Value); strcat(g_acAnswerBuf,strHelp);
		}
	}
	g_acAnswerBuf[strlen(g_acAnswerBuf)-1]=0; // Letztes Leerzeichen ausblenden
  	strcat(g_acAnswerBuf,"\r");
	return errInfo;
}

/*unsigned short Cmd_ReadMultipleAbsolutePos(unsigned short Index)
{
	unsigned short error = errNone;
	unsigned short error2 = errNone;
	unsigned short FirstParaSet = 0;
	char strHelp[20];
	unsigned short ID;
	//float Value = 0;
	double Value = 0;

	sprintf(g_acAnswerBuf,"I"); strHelp[0]=0;

	while(1)
	{
		if((error2 = scanUShort(&ID)) != errNone)
		{
			if(!FirstParaSet){return error2;}else{break;}
		}
		FirstParaSet = 1;
		if((error = MotorGetPosition( ID, &Value)) != errNone) return error;
		if(error == errNone)
		{
			if(Index)
			{
				sprintf(strHelp,"%u ",ID); strcat(g_acAnswerBuf,strHelp);
			}
			sprintf(strHelp,"%3.2f ",Value); strcat(g_acAnswerBuf,strHelp);
		}
	    if(strlen(g_acAnswerBuf) > ANSWER_BUF_LEN-12)
	    {
	    	error=ERR_WRONG_PARAMETER; // Verhinderung des Bufferüberlaufes	mit 12 bit Reserve
			break;
		}
	}//end...while(1)
	if(error == errNone)
	{
		g_acAnswerBuf[strlen(g_acAnswerBuf)-1]=0; // Letztes Leerzeichen ausblenden
	  	strcat(g_acAnswerBuf,"\r");
		error = errInfo;
	}
	return error;
}*/

unsigned short Cmd_Homing(void)
{
	unsigned short error = errNone;
	unsigned short Mask;

	if((error = scanUShort(&Mask)) != errNone) return error;
	if(!ConfirmMask(Mask)) return ERR_WRONG_PARAMETER;
	if(!m_EnableMovCmd) return ERR_CMD_NOT_PRACTICABLE;
	return CmdIntern_Homing(Mask);
}

unsigned short CmdIntern_Homing(unsigned short Mask)
{
	unsigned short error = errNone;

	g_MsgASync.Mask = Mask;
	g_MsgASync.Error = errNone;
	g_MsgASync.eCommand = eASyncAdjust;
	error = K_Mesg_Send_Wait(MBX_ASYNCCTRL, 0, &g_MsgASync);
	if(error != K_OK)
	{
		return ERR_SYSTEM;
	}
	return g_MsgASync.Error;
}

unsigned short Cmd_HomingSyncron(void)
{
	unsigned short error = errNone;
	unsigned short Mask;
	unsigned short value;
#ifdef _CF_DEBUG_PRINT
	unsigned short i;
	char strHelp[20];
	char strAnswer[50];
#endif


	if((error = scanUShort(&Mask)) != errNone) return error;
	if(!ConfirmMask(Mask)) return ERR_WRONG_PARAMETER;
	if(!m_EnableMovCmd) return ERR_CMD_NOT_PRACTICABLE;
	if((error = CmdIntern_Homing(Mask)) != errNone) return error;

	do{
		K_Task_Wait(50);
		WDO();
		if((error= g_ASync_GetAllRuningFlags(Mask, &value)) != errNone)
		{
			return error;
		}
		if(value == STATE_ESTOP)
		{
			m_MaskSynchronSpecialFunctionEstop = Mask;
			m_FlagSynchronSpecialFunctionEstop = TRUE;
			return (unsigned short)ERR_SAFETY_APPLIANCE;
		}
	#ifdef _CF_DEBUG_PRINT
	if(g_TestFlag & TF_DBG_ASYNC)
	{
		sprintf(strAnswer,"I"); strHelp[0]=0;
		for(i=0; i < MAX_AXES; i++)
		{
			WDO();
			if(GetAxisIDFromMask(Mask, g_FelixAxes[i].AxisBit) != 0)
			{
				//sprintf(strHelp,"%u ", g_GetAxes_Running(i));strcat(strAnswer,strHelp);//NEWERR
				sprintf(strHelp,"%u ", g_MsgASync.MoveData[i].Running);strcat(strAnswer,strHelp);//NEWERR
			}
		}
		dbPrint("GETSTATUS :%s\r", strAnswer);
	}
	#endif

	}while(value == STATE_BUSY);
	if((error = g_ASync_GetAllErrorFlags(Mask, &value)) != errNone) {return error;}
	// falls gewünsch Abfrage von gStatus und gError, dann g_ASync_ResetError entfernen
	if((error = g_ASync_ResetError(1,Mask)) != errNone) {return error;}
	return value;
}

unsigned short Cmd_MoveSyncron(void)
{
	unsigned short error = errNone;
	unsigned short Mask;
	unsigned short i;
	unsigned short ID;
	unsigned short idx;
	//float value;
	double value;
	unsigned short Mode1536 = 0;
//---------------------------------------------
#ifdef _CF_DEBUG_POSERROR
unsigned short a;
DebugPosErrReset();	
#endif
//---------------------------------------------
	if((error = scanUShort(&Mask)) != errNone) return error;
	if(!ConfirmMask(Mask)) return ERR_WRONG_PARAMETER;
	if(!m_EnableMovCmd) return ERR_CMD_NOT_PRACTICABLE;
	for(i=0; i < MAX_AXES; i++)
	{
		WDO();
		//if(GetAxisIDFromMask(Mask, g_FelixAxes[i].AxisBit) != 0)
		if((ID = GetAxisIDFromMask(Mask, g_FelixAxes[i].AxisBit)) != 0)
		{
			//if((error = scanFloat(&value)) != errNone) return error;
			if((error = scanDouble(&value)) != errNone) 
			{
				#ifdef _CF_DEBUG_POSERROR
				DebugPosErrReset();
				#endif
				return error;
			}
			idx = GetAxisIndex(ID);
			if(idx < 0)
			{
				#ifdef _CF_DEBUG_POSERROR
				DebugPosErrReset();
				#endif
				return ERR_UNKNOWN_DRV;
			}
			if(!CheckDouble(pIMSChangedPara[idx]->fMinWay - (double)0.005 , pIMSChangedPara[idx]->fMaxWay + (double)0.005, value))
			{
				#ifdef _CF_DEBUG_POSERROR
				DebugPosErrReset();
				#endif
				return ERR_WRONG_PARAMETER;
			}
			#ifdef _CF_DEBUG_PRINT
			if(g_TestFlag & TF_DBG_CMD){dbPrint("%3.3f ",value);}
			#endif
			g_MsgASync.MoveData[i].AbsolutePos = value;
			#ifdef _CF_DEBUG_POSERROR
			DebugData[i].CmdPos = value;
			DebugData[i].Running = 1;
			#endif
		}
	}
	if((error = scanUShort(&Mode1536)) != errNone)
	{
		Mode1536 = 0;
	}
	//else{if(Mode1536){Mode1536 = 1;}else{Mode1536 = 0;}}
	#ifdef _CF_DEBUG_POSERROR
	DebugCmdMask = Mask;
	DebugCmdMode1536 = Mode1536;
	a = 0;
	do{
		if((error = MotorMoveSyncronExe(Mask, Mode1536)) != errNone)
		{
			if(error == (unsigned short)ERR_SAFETY_APPLIANCE)
			{
				m_SynchronMoveEstop = TRUE;
				m_MaskSynchronMoveEstop = Mask;
			}
			else
			{
				DebugPosErrReset();
			}
			return error;
		}
		error = DebugPositionTesten(a);
		if(error == (DEBUG_ERROR1))
		{
			if(++a > 3)
			{
				DebugPosErrReset();
				return error;
			}
			DebugCmdMask = 0;
			for(i=0; i < MAX_AXES; i++)
			{
				WDO();
				if(DebugData[i].Running)
				{
					g_MsgASync.MoveData[i].AbsolutePos = DebugData[i].CmdPos;
					DebugCmdMask = DebugCmdMask | g_FelixAxes[i].AxisBit;
				}
			}
			Mask = DebugCmdMask;
			Mode1536 = DebugCmdMode1536;
		}
	}while(error == (DEBUG_ERROR1));
	DebugPosErrReset();
#else
	if((error = MotorMoveSyncronExe(Mask, Mode1536)) == (unsigned short)ERR_SAFETY_APPLIANCE)
	{
		m_SynchronMoveEstop = TRUE;
		m_MaskSynchronMoveEstop = Mask;
	}
#endif
	return error;
}

unsigned short Cmd_WaitSynchronMoveEstop(void)
{
	unsigned short error = errNone;
	unsigned short value;
#ifdef _CF_DEBUG_POSERROR
unsigned short i;
unsigned short a;
#endif
	do{
		K_Task_Wait(50);
		WDO();
		if((error= g_ASync_GetAllRuningFlags(m_MaskSynchronMoveEstop, &value)) != errNone)
		{
			/* 24.10.2019
			if(value == STATE_ESTOP)
			{
				m_SynchronMoveEstop = TRUE;
				return (unsigned short)ERR_SAFETY_APPLIANCE;
			}*/
			#ifdef _CF_DEBUG_POSERROR
			DebugPosErrReset();
			#endif
			return error;
		}
		if(value == STATE_ESTOP)
		{
			m_SynchronMoveEstop = TRUE;
			return (unsigned short)ERR_SAFETY_APPLIANCE;
		}
	//}while((value == STATE_BUSY)  || (value == STATE_BUSY_ESTOP));
	}while(value == STATE_BUSY);
	if((error = g_ASync_GetAllErrorFlags(m_MaskSynchronMoveEstop, &value)) != errNone)
	{
		#ifdef _CF_DEBUG_POSERROR
		DebugPosErrReset();
		#endif
		return error;
	}
	error = value;
//----------------------------------------
// hier position testen
#ifdef _CF_DEBUG_POSERROR
	if(error == errNone)
	{
		a =0;
		do{
			error = DebugPositionTesten(a);
			if(error == (DEBUG_ERROR1))
			{
				if(++a > 3)
				{
					DebugPosErrReset();
					return error;
				}
				DebugCmdMask = 0;
				for(i=0; i < MAX_AXES; i++)
				{
					WDO();
					if(DebugData[i].Running)
					{
						g_MsgASync.MoveData[i].AbsolutePos = DebugData[i].CmdPos;
						DebugCmdMask = DebugCmdMask | g_FelixAxes[i].AxisBit;
					}
				}
				//Mask = DebugCmdMask;
				//Mode1536 = DebugCmdMode1536;
				if((error = MotorMoveSyncronExe(DebugCmdMask, DebugCmdMode1536)) != errNone)
				{
					if(error == (unsigned short)ERR_SAFETY_APPLIANCE)
					{
						m_SynchronMoveEstop = TRUE;
						m_MaskSynchronMoveEstop = DebugCmdMask;
					}
					else
					{
						DebugPosErrReset();
					}
					return error;
				}
			}
	
		}while(error == (DEBUG_ERROR1));
	}
	DebugPosErrReset();
#endif
//----------------------------------------
	return error;
}

unsigned short Cmd_WaitSynchronSpecialFunctionMoveEstop(void)
{
	unsigned short error = errNone;
	unsigned short value;

	do{
		K_Task_Wait(50);
		WDO();
		if((error= g_ASync_GetAllRuningFlags(m_MaskSynchronSpecialFunctionEstop, &value)) != errNone)
		{
			return error;
		}
		if(value == STATE_ESTOP)
		{
			m_FlagSynchronSpecialFunctionEstop = TRUE;
			return (unsigned short)ERR_SAFETY_APPLIANCE;
		}
	}while(value == STATE_BUSY);
	if((error = g_ASync_GetAllErrorFlags(m_MaskSynchronSpecialFunctionEstop, &value)) != errNone)
	{
		return error;
	}
	// falls gewünsch Abfrage von gStatus und gError, dann g_ASync_ResetError entfernen
	if((error = g_ASync_ResetError(1,m_MaskSynchronSpecialFunctionEstop)) != errNone) {return error;}
	return value;
}

unsigned short Cmd_MoveASync(void)
{
	unsigned short error = errNone;
	unsigned short Mask;
	//unsigned short cnt;
	unsigned short i;
	unsigned short ID;
	unsigned short idx;
	//float value;
	double value;
	unsigned short Mode1536 = 0;
#ifdef _CF_DEBUG_POSERROR
	if(!DebugCmdMask){DebugPosErrReset();}
#endif
	if((error = scanUShort(&Mask)) != errNone) return error;
	if(!ConfirmMask(Mask)) return ERR_WRONG_PARAMETER;
	if(!m_EnableMovCmd) return ERR_CMD_NOT_PRACTICABLE;
	#ifdef _CF_DEBUG_PRINT
	if(g_TestFlag & TF_DBG_CMD){dbPrint("cMv ");}
	#endif
	for(i=0; i < MAX_AXES; i++)
	{
		WDO();
		if((ID = GetAxisIDFromMask(Mask, g_FelixAxes[i].AxisBit)) != 0)
		{
			
			//if((error = scanFloat(&value)) != errNone) return error;
			if((error = scanDouble(&value)) != errNone)
			{
				#ifdef _CF_DEBUG_POSERROR
				DebugPosErrReset();
				#endif
				return error;
			}
			idx = GetAxisIndex(ID);
			if(idx < 0)
			{
				#ifdef _CF_DEBUG_POSERROR
				DebugPosErrReset();
				#endif
				return ERR_UNKNOWN_DRV;
			}
			if(!CheckDouble(pIMSChangedPara[idx]->fMinWay - (double)0.005 , pIMSChangedPara[idx]->fMaxWay + (double)0.005, value))
			{
				#ifdef _CF_DEBUG_POSERROR
				DebugPosErrReset();
				#endif
				return ERR_WRONG_PARAMETER;
			}
			#ifdef _CF_DEBUG_PRINT
			if(g_TestFlag & TF_DBG_CMD){dbPrint("%3.3f ",value);}
			#endif
			g_MsgASync.MoveData[i].AbsolutePos = value;
			#ifdef _CF_DEBUG_POSERROR
			DebugData[i].CmdPos = value;
			DebugData[i].Running = 1;
			#endif
		}
	}
	#ifdef _CF_DEBUG_PRINT
	if(g_TestFlag & TF_DBG_CMD){dbPrint("\r");}
	#endif
	if((error = scanUShort(&Mode1536)) != errNone)
	{
		Mode1536 = 0;
	}
	#ifdef _CF_DEBUG_POSERROR
	DebugCmdMask = DebugCmdMask | Mask;
	DebugCmdMode1536 = Mode1536;
	error = MotorMoveASyncExe(Mask, Mode1536);
//----------------------------------------
	if((error != errNone) && (error != (unsigned short)ERR_SAFETY_APPLIANCE))
	{
		DebugPosErrReset();
	}

//----------------------------------------
	return error;
	#else
	return MotorMoveASyncExe(Mask, Mode1536);
	#endif
}

unsigned short Cmd_Drv_GetStatus(void)
{
	unsigned short error = errNone;
	unsigned short Mask;
	unsigned short i;
	char strHelp[20];
#ifdef _CF_DEBUG_POSERROR
	unsigned short value;
#endif

	if((error = scanUShort(&Mask)) != errNone) return error;
	if(!ConfirmMask(Mask)) return ERR_WRONG_PARAMETER;
	g_MsgASync.Mask = Mask;
	g_MsgASync.Error = errNone;
	g_MsgASync.eCommand = eASyncGetStatus;
	error = K_Mesg_Send_Wait(MBX_ASYNCCTRL, 0, &g_MsgASync);
	if(error != K_OK)
	{
		#ifdef _CF_DEBUG_POSERROR
		DebugPosErrReset();
		#endif
		return ERR_SYSTEM;
	}
	if(g_MsgASync.Error != errNone)
	{
		#ifdef _CF_DEBUG_POSERROR
		DebugPosErrReset();
		#endif
		return g_MsgASync.Error;
	}
	if(g_MsgASync.EStop)
	{
		return (unsigned short)ERR_SAFETY_APPLIANCE;
	}

	sprintf(g_acAnswerBuf,"I"); strHelp[0]=0;
	for(i=0; i < MAX_AXES; i++)
	{
		WDO();
		if(GetAxisIDFromMask(Mask, g_FelixAxes[i].AxisBit) != 0)
		{
			sprintf(strHelp,"%u ", g_MsgASync.MoveData[i].Running); strcat(g_acAnswerBuf,strHelp);//NEWERR
		}
	}
//----------------------------------------
#ifdef _CF_DEBUG_POSERROR
// hier position testen
	if(DebugCmdMask)// && (!isErrorSafetyMonitoring()))
	{
		DebugMoveError = 0;
		for(i=0; i < MAX_AXES; i++)
		{
			WDO();
			if(DebugData[i].Running)
			{
				if(g_MsgASync.MoveData[i].Running != STATE_BUSY)
				{
					if(g_MsgASync.MoveData[i].Running != STATE_READY)
					{
						DebugData[i].Running = 0;
						DebugMoveError = 1;
					}
					DebugCmdMask = DebugCmdMask & (~g_FelixAxes[i].AxisBit);
				}
			}
		}
		if(DebugMoveError)
		{
			DebugPosErrReset();
		}
		else
		{
			if(!DebugCmdMask)
			{

				if((error = g_ASync_GetAllErrorFlags(m_MaskSynchronMoveEstop, &value)) != errNone)
				{
					DebugPosErrReset();
					return error;
				}
				if(value != errNone)
				{
					DebugPosErrReset();
				}
				error = DebugPositionTesten(DebugAsyncReposCnt);
				if(error == (DEBUG_ERROR1))
				{
					if(++DebugAsyncReposCnt > 3)
					{
						DebugPosErrReset();
						return DEBUG_ERROR2;
					}
					for(i=0; i < MAX_AXES; i++)
					{
						WDO();
						if(DebugData[i].Running)
						{
							g_MsgASync.MoveData[i].AbsolutePos = DebugData[i].CmdPos;
							DebugCmdMask = DebugCmdMask | g_FelixAxes[i].AxisBit;
						}
					}
					error = MotorMoveASyncExe(DebugCmdMask, DebugCmdMode1536);
					if((error != errNone) && (error != (unsigned short)ERR_SAFETY_APPLIANCE))
					{
						DebugPosErrReset();
						return error;
					}
					sprintf(g_acAnswerBuf,"I"); strHelp[0]=0;
					for(i=0; i < MAX_AXES; i++)
					{
						WDO();
						if(GetAxisIDFromMask(Mask, g_FelixAxes[i].AxisBit) != 0)
						{
							//sprintf(strHelp,"%u ", DebugData[i].Running); strcat(g_acAnswerBuf,strHelp);
							sprintf(strHelp,"1 "); strcat(g_acAnswerBuf,strHelp);
						}
					}
				}
			}
		}
	}
#endif
//----------------------------------------
	g_acAnswerBuf[strlen(g_acAnswerBuf)-1]=0; // Letztes Leerzeichen ausblenden
  	strcat(g_acAnswerBuf,"\r");
	return errInfo;
}

unsigned short Cmd_Teachen(void)
{
	unsigned short error = errNone;
	unsigned short Mask;
	//unsigned short cnt;
	unsigned short i;
	unsigned short ID;
	unsigned short idx;
	//float value;
	double value;

	if((error = scanUShort(&Mask)) != errNone) return error;
	if(!ConfirmMask(Mask)) return ERR_WRONG_PARAMETER;
	#ifdef _CF_DEBUG_PRINT
	if(g_TestFlag & TF_DBG_INT_COM){dbPrint("\r");}
	#endif
	for(i=0; i < MAX_AXES; i++)
	{
		WDO();
		if((ID = GetAxisIDFromMask(Mask, g_FelixAxes[i].AxisBit)) != 0)
		{
			//if((error = scanFloat(&value)) != errNone) return error;
			if((error = scanDouble(&value)) != errNone) return error;
			idx = GetAxisIndex(ID);
			if(idx < 0)
			{
				return ERR_UNKNOWN_DRV;
			}
			if(!CheckDouble(pIMSChangedPara[idx]->fMinWay - (double)0.005 , pIMSChangedPara[idx]->fMaxWay + (double)0.005, value)){return ERR_WRONG_PARAMETER;}
			#ifdef _CF_DEBUG_PRINT
			if(g_TestFlag & TF_DBG_INT_COM){dbPrint("%3.3f ",value);}
			#endif
			g_MsgASync.MoveData[i].AbsolutePos = value;
		}
	}
	#ifdef _CF_DEBUG_PRINT
	if(g_TestFlag & TF_DBG_INT_COM){dbPrint("\r");}
	#endif
	//return MotorMoveASyncExe(Mask, AbsolueElseRelative);
	return MotorTeachExe(Mask);
}

unsigned short Cmd_AShake(void)
{
	unsigned short error = errNone;
	unsigned short i;
	unsigned short Mask;
	//float deflection;
	double deflection;
	unsigned short cycle;
	unsigned short state;

	if((error = scanUShort(&Mask)) != errNone) return error;
	if(!ConfirmMask(Mask)) return ERR_WRONG_PARAMETER;
	//if((error = scanFloat(&deflection)) != errNone) return error;
	if((error = scanDouble(&deflection)) != errNone) return error;
	if((error = scanUShort(&cycle)) != errNone) return error;
	if(GetAxisIDFromMask(Mask, eZ_Ex_Axis) != 0) return ERR_WRONG_PARAMETER;
	if(cycle == 0) return ERR_WRONG_PARAMETER;
	if(deflection == (double)0.0) return ERR_WRONG_PARAMETER;

	if((error= g_ASync_GetAllRuningFlags(Mask, &state)) != errNone){return error;}
	if(state == STATE_ESTOP){return (unsigned short)ERR_SAFETY_APPLIANCE;}
	if(state == STATE_BUSY){return ERR_DEVICE_BUSY;}

	if((error = g_ASync_ResetError(1,Mask)) != errNone) {return error;}
	for(i=0; i < MAX_AXES; i++)
	{
		WDO();
		if(GetAxisIDFromMask(Mask, g_FelixAxes[i].AxisBit) != 0)
		{
			g_MsgASync.MoveData[i].RelativeWay = deflection;
		}
	}
	g_MsgASync.Argument.usShort = cycle;

	error= g_ASync_StartShake(Mask);
	if(error != errNone){return error;}
	K_Task_Wait(10);
	if((error = g_ASync_GetAllErrorFlags(Mask, &state)) != errNone) {return error;}
	return state;

}

unsigned short Cmd_SearchAdapter(unsigned short CmdSync)
{
	unsigned short error = errNone;
	unsigned short value;
	unsigned short i;
	unsigned short Mask;
	unsigned short ID;
	unsigned short idx;
	double fZielPosition;
	double fRelativeWay;
	double fDeltaWay;
	unsigned short state;

	if((error = scanUShort(&Mask)) != errNone) return error;
	if(!ConfirmMask(Mask)) {return ERR_WRONG_PARAMETER;}
	//if(GetAxisIDFromMask(Mask, eXYMask) != 0) {return ERR_WRONG_PARAMETER;}
	if(Mask  & (~(unsigned short)eZ_Axis)) {return ERR_WRONG_PARAMETER;}
	if(!m_EnableMovCmd) return ERR_CMD_NOT_PRACTICABLE;
	if((error = scanDouble(&fZielPosition)) != errNone) return error;
	if((error = scanDouble(&fRelativeWay)) != errNone)
	{
		// hier default parameter setzen (aus FRAM)
		fRelativeWay = g_SearchAdapterRelativeDeltaWay.RelativeWay;
	}
	if((error = scanDouble(&fDeltaWay)) != errNone)
	{
		fDeltaWay = g_SearchAdapterRelativeDeltaWay.DeltaWay;
	}
	else
	{
		if(fRelativeWay < (double)0.0)
		{
			fDeltaWay = fabs(fDeltaWay) * (double)-1.0;
		}
		else
		{
			fDeltaWay = fabs(fDeltaWay);
		}
	}
	//if(fRelativeWay == (double)0.0) return ERR_WRONG_PARAMETER;
	if((error= g_ASync_GetAllRuningFlags(Mask, &state)) != errNone){return error;}
	if(state == STATE_ESTOP){return (unsigned short)ERR_SAFETY_APPLIANCE;}
	if(state == STATE_BUSY){return ERR_DEVICE_BUSY;}
	if((error = g_ASync_ResetError(1,Mask)) != errNone) {return error;}
	for(i=0; i < MAX_AXES; i++)
	{
		WDO();
		/*if(GetAxisIDFromMask(Mask, g_FelixAxes[i].AxisBit) != 0)
		{
			g_MsgASync.MoveData[i].AbsolutePos = fZielPosition;
			g_MsgASync.MoveData[i].RelativeWay = fRelativeWay;
			g_MsgASync.MoveData[i].DeltaWay = fDeltaWay;
		}*/
		if((ID = GetAxisIDFromMask(Mask, g_FelixAxes[i].AxisBit)) != 0)
		{
			idx = GetAxisIndex(ID);
			if(idx < 0)
			{
				return ERR_UNKNOWN_DRV;
			}
			if(!CheckDouble(pIMSChangedPara[idx]->fMinWay - (double)0.005 , pIMSChangedPara[idx]->fMaxWay + (double)0.005, fZielPosition))
			{
				return ERR_WRONG_PARAMETER;
			}
			if(!CheckDouble(pIMSChangedPara[idx]->fMinWay - (double)0.005 , pIMSChangedPara[idx]->fMaxWay + (double)0.005, fRelativeWay))
			{
				return ERR_WRONG_PARAMETER;
			}
			/*if(!CheckDouble(pIMSChangedPara[idx]->fMinWay - (double)0.005 , pIMSChangedPara[idx]->fMaxWay + (double)0.005, fZielPosition + fRelativeWay))
			{
				return ERR_WRONG_PARAMETER;
			}*/
			if(!CheckDouble((double)-10.0 - (double)0.005 , (double)10.0 + (double)0.005, fDeltaWay))
			{
				return ERR_WRONG_PARAMETER;
			}
			g_MsgASync.MoveData[i].AbsolutePos = fZielPosition;
			g_MsgASync.MoveData[i].RelativeWay = fRelativeWay;
			g_MsgASync.MoveData[i].DeltaWay = fDeltaWay;
		}

	}
	error= g_ASync_StartSearchAdapter(Mask);
	if(error != errNone)
	{
		return error;
	}
	if(CmdSync)
	{
		do{
			K_Task_Wait(50);
			WDO();
			if((error= g_ASync_GetAllRuningFlags(Mask, &value)) != errNone)
			{
				return error;
			}
			if(value == STATE_ESTOP)
			{
				m_MaskSynchronSpecialFunctionEstop = Mask;
				m_FlagSynchronSpecialFunctionEstop = TRUE;
				return (unsigned short)ERR_SAFETY_APPLIANCE;
			}
		}while(value == STATE_BUSY);
	}
	else
	{
		K_Task_Wait(10);
	}
	if((error = g_ASync_GetAllErrorFlags(Mask, &value)) != errNone)
	{
		return error;
	}
	if(CmdSync)
	{
		// falls gewünsch Abfrage von gStatus und gError, dann g_ASync_ResetError entfernen
		if((error = g_ASync_ResetError(1,Mask)) != errNone) {return error;}
	}
	return value;
}

unsigned short Cmd_Drv_GetHomingStatus(void)
{
	unsigned short error = errNone;
//	unsigned short ID;
	unsigned short Mask;
	unsigned short i;
	char strHelp[20];

	if((error = scanUShort(&Mask)) != errNone) return error;
	if(!ConfirmMask(Mask)) return ERR_WRONG_PARAMETER;


	g_MsgASync.Mask = Mask;
	g_MsgASync.Error = errNone;
	g_MsgASync.eCommand = eASyncGetAdjState;
	error = K_Mesg_Send_Wait(MBX_ASYNCCTRL, 0, &g_MsgASync);
	if(error != K_OK)
	{
		return ERR_SYSTEM;
	}
	if(g_MsgASync.Error != errNone){return g_MsgASync.Error;}
	sprintf(g_acAnswerBuf,"I"); strHelp[0]=0;
	for(i=0; i < MAX_AXES; i++)
	{
		WDO();
		if(GetAxisIDFromMask(Mask, g_FelixAxes[i].AxisBit) != 0)
		{
			sprintf(strHelp,"%u ", g_GetAxes_HomingFlag(i));strcat(g_acAnswerBuf,strHelp);
		}
	}
	g_acAnswerBuf[strlen(g_acAnswerBuf)-1]=0; // Letztes Leerzeichen ausblenden
  	strcat(g_acAnswerBuf,"\r");
	return errInfo;
}

unsigned short Cmd_GetError(void)
{
	unsigned short error = errNone;
	unsigned short Mask;
	unsigned short i;
	char strHelp[20];

	if((error = scanUShort(&Mask)) != errNone) return error;
	if(!ConfirmMask(Mask)) return ERR_WRONG_PARAMETER;
	//if((error = scanUShort(&Mask_Bit)) != errNone) return error;
	//if(!ConfirmMask(Mask_Bit)) return ERR_WRONG_PARAMETER;

	g_MsgASync.Mask = Mask;
	g_MsgASync.Error = errNone;
	g_MsgASync.eCommand = eASyncGetError;
	error = K_Mesg_Send_Wait(MBX_ASYNCCTRL, 0, &g_MsgASync);
	if(error != K_OK)
	{
		return ERR_SYSTEM;
	}
	if(g_MsgASync.Error != errNone){return g_MsgASync.Error;}
	sprintf(g_acAnswerBuf,"I"); strHelp[0]=0;

	for(i=0; i < MAX_AXES; i++)
	{
		WDO();
		if(GetAxisIDFromMask(Mask, g_FelixAxes[i].AxisBit) != 0)
		{
			/*2016.03.01 if(g_MsgASync.MoveData[i].ErrorFlag)
			{
				error = g_MsgASync.MoveData[i].Error;

			}*/
			if(g_GetAxes_ErrorFlag(i))
			{
				error = g_GetAxes_ErrorAxisCyBio(i);
			}
			else {error = errNone;}
			sprintf(strHelp,"%u ", error); strcat(g_acAnswerBuf,strHelp);
		}
	}
	g_acAnswerBuf[strlen(g_acAnswerBuf)-1]=0; // Letztes Leerzeichen ausblenden
  	strcat(g_acAnswerBuf,"\r");
	return errInfo;
}

unsigned short Cmd_ResetError(void)
{
	unsigned short error = errNone;
	unsigned short Mask;

	if((error = scanUShort(&Mask)) != errNone) return error;
	if(!ConfirmMask(Mask)) return ERR_WRONG_PARAMETER;
	return g_ASync_ResetError(0, Mask);
}

unsigned short Cmd_ResetOEMError(void)
{
	unsigned short error = errNone;
	unsigned short ID;
	unsigned short Mask;
	unsigned short i;

	if((error = scanUShort(&Mask)) != errNone) return error;
	if(!ConfirmMask(Mask)) return ERR_WRONG_PARAMETER;

	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if((ID = GetAxisIDFromMask(Mask, g_FelixAxes[i].AxisBit)) != 0)
		{
			if((error = ImsCmd_ResetError( ID)) != errNone) return error;
		}
	}
	return error;
}

unsigned short Cmd_GetOEMError(void)
{
	unsigned short error = errNone;
	unsigned short ID;
	unsigned short Mask;
	unsigned short value;	
	char strHelp[20];
	unsigned short i;

	if((error = scanUShort(&Mask)) != errNone) return error;
	if(!ConfirmMask(Mask)) return ERR_WRONG_PARAMETER;
	sprintf(g_acAnswerBuf,"I"); strHelp[0]=0;
	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if((ID = GetAxisIDFromMask(Mask, g_FelixAxes[i].AxisBit)) != 0)
		{
			if((error = ImsCmd_GetOEMError( ID, &value)) != errNone) return error;
			sprintf(strHelp,"%u ",value); strcat(g_acAnswerBuf,strHelp);
		}
	}
	g_acAnswerBuf[strlen(g_acAnswerBuf)-1]=0; // Letztes Leerzeichen ausblenden
  	strcat(g_acAnswerBuf,"\r");
	return errInfo;
}

unsigned short Cmd_GetHWError(void)
{
	unsigned short error = errNone;
	unsigned short ID;
	unsigned short Mask;
	unsigned short value;	
	char strHelp[20];
	unsigned short i;

	if((error = scanUShort(&Mask)) != errNone) return error;
	if(!ConfirmMask(Mask)) return ERR_WRONG_PARAMETER;
	sprintf(g_acAnswerBuf,"I"); strHelp[0]=0;
	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if((ID = GetAxisIDFromMask(Mask, g_FelixAxes[i].AxisBit)) != 0)
		{
			if((error = ImsCmd_GetHWError( ID, &value)) != errNone) return error;
			sprintf(strHelp,"%u ",value); strcat(g_acAnswerBuf,strHelp);
		}
	}
	g_acAnswerBuf[strlen(g_acAnswerBuf)-1]=0; // Letztes Leerzeichen ausblenden
  	strcat(g_acAnswerBuf,"\r");
	return errInfo;
}



unsigned short Cmd_Drv_GetOffset(void)
{
	unsigned short error = errNone;
	unsigned short i;
	char strHelp[20];
	unsigned short ID;
	unsigned short idx;
	unsigned short Mask;
	float Value = 0;

	if((error = scanUShort(&Mask)) != errNone) return error;
	if(!ConfirmMask(Mask)) return ERR_WRONG_PARAMETER;

	sprintf(g_acAnswerBuf,"I"); strHelp[0]=0;
	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if((ID = GetAxisIDFromMask(Mask, g_FelixAxes[i].AxisBit)) != 0)
		{
			idx = GetAxisIndex(ID);
			if(idx < 0)
			{
				return ERR_UNKNOWN_DRV;
			}
			sprintf(strHelp,"%3.2f ",pIMSChangedPara[idx]->fOffset); strcat(g_acAnswerBuf,strHelp);
		}
	}
	g_acAnswerBuf[strlen(g_acAnswerBuf)-1]=0; // Letztes Leerzeichen ausblenden
  	strcat(g_acAnswerBuf,"\r");
	return errInfo;
}


unsigned short Cmd_Drv_SetOffset(void)
{
	unsigned short error = errNone;
	unsigned short Mask;
	unsigned short ID;
	unsigned short idx;
	unsigned short i;
	float value = 0;

	if((error = scanUShort(&Mask)) != errNone) return error;
	if(!ConfirmMask(Mask)) return ERR_WRONG_PARAMETER;
	// alle offsets und minmax auslesen,, denn alle achsen werden in fram geschrieben
	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();

		g_AxisOffset[i].AxisOffsetFromHomingSwitch = pIMSChangedPara[i]->fOffset;
		g_AxisMinMaxWay[i].MinWay = pIMSChangedPara[i]->fMinWay;
		g_AxisMinMaxWay[i].MaxWay = pIMSChangedPara[i]->fMaxWay;
	}
	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if((ID = GetAxisIDFromMask(Mask, g_FelixAxes[i].AxisBit)) != 0)
		{
			idx = GetAxisIndex(ID);
			if(idx < 0)
			{
				return ERR_UNKNOWN_DRV;
			}
			if((error = scanFloat(&value)) != errNone) return error;
			//pIMSChangedPara[idx]->fOffset = value;
			g_AxisOffset[idx].AxisOffsetFromHomingSwitch = value;
		}
	}
	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if((ID = GetAxisIDFromMask(Mask, g_FelixAxes[i].AxisBit)) != 0)
		{
			idx = GetAxisIndex(ID);
			if(idx >= 0)
			{
				//
				// alter Offset -> pIMSChangedPara[idx]->fOffset
				// neuer Offset -> g_AxisOffset[idx].AxisOffsetFromHomingSwitch
				if(!g_FelixAxes[idx].AxisFullRotary)
				{
					g_AxisMinMaxWay[idx].MinWay = g_AxisMinMaxWay[idx].MinWay + pIMSChangedPara[idx]->fOffset - g_AxisOffset[idx].AxisOffsetFromHomingSwitch;
					g_AxisMinMaxWay[idx].MaxWay = g_AxisMinMaxWay[idx].MaxWay + pIMSChangedPara[idx]->fOffset - g_AxisOffset[idx].AxisOffsetFromHomingSwitch;
					pIMSChangedPara[idx]->fMinWay = g_AxisMinMaxWay[idx].MinWay;
					pIMSChangedPara[idx]->fMaxWay = g_AxisMinMaxWay[idx].MaxWay;
				}
				pIMSChangedPara[idx]->fOffset = g_AxisOffset[idx].AxisOffsetFromHomingSwitch;

			}

		}
	}
	error = FRAMSaveAxisOffset(g_AxisOffset);
	ID = FRAMSaveAxisMinMaxWay(g_AxisMinMaxWay);
	if(error != errNone) {return error;}
	if(ID != errNone) {return ID;}//hilfserror
	return errNone;
}

unsigned short Cmd_Drv_GetMaxWayRange(void)
{
	unsigned short error;
	unsigned short i;
	char strHelp[100];
	unsigned short ID;
	unsigned short Mask;
	short idx;

	if((error = scanUShort(&Mask)) != errNone) return error;
	if(!ConfirmMask(Mask)) return ERR_WRONG_PARAMETER;

	sprintf(g_acAnswerBuf,"I"); strHelp[0]=0;

	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if((ID = GetAxisIDFromMask(Mask, g_FelixAxes[i].AxisBit)) != 0)
		{
			idx = GetAxisIndex(ID);
			if(idx < 0)
			{
				return ERR_UNKNOWN_DRV;
			}
			//sprintf(strHelp,"%3.2f %3.2f ", pIMSFixedPara[idx]->fMinWay, pIMSFixedPara[idx]->fMaxWay);
			sprintf(strHelp,"%3.2f %3.2f ", pIMSChangedPara[idx]->fMinWay, pIMSChangedPara[idx]->fMaxWay);
			strcat(g_acAnswerBuf,strHelp);
		}
	}
	g_acAnswerBuf[strlen(g_acAnswerBuf)-1]=0; // Letztes Leerzeichen ausblenden
  	strcat(g_acAnswerBuf,"\r");
	return errInfo;
}

unsigned short Cmd_Drv_SetMaxWayRange(void)
{
	unsigned short error = errNone;
	unsigned short Mask;
	unsigned short ID;
	unsigned short idx;
	unsigned short i;
	double value = 0;

	if((error = scanUShort(&Mask)) != errNone) return error;
	if(!ConfirmMask(Mask)) return ERR_WRONG_PARAMETER;
	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		g_AxisMinMaxWay[i].MinWay = pIMSChangedPara[i]->fMinWay;
		g_AxisMinMaxWay[i].MaxWay = pIMSChangedPara[i]->fMaxWay;

	}
	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if((ID = GetAxisIDFromMask(Mask, g_FelixAxes[i].AxisBit)) != 0)
		{
			idx = GetAxisIndex(ID);
			if(idx < 0)
			{
				return ERR_UNKNOWN_DRV;
			}
			if((error = scanDouble(&value)) != errNone) return error;
			g_AxisMinMaxWay[idx].MinWay = value;
			if((error = scanDouble(&value)) != errNone) return error;
			g_AxisMinMaxWay[idx].MaxWay = value;
			if(g_AxisMinMaxWay[idx].MinWay > g_AxisMinMaxWay[idx].MaxWay) {return ERR_WRONG_PARAMETER;}
			if(g_FelixAxes[idx].AxisFullRotary){return ERR_VALUE_OUT_OF_RANGE;}
		}
	}
	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if((ID = GetAxisIDFromMask(Mask, g_FelixAxes[i].AxisBit)) != 0)
		{
			idx = GetAxisIndex(ID);
			if(idx >= 0)
			{
				if(g_FelixAxes[idx].AxisFullRotary){return ERR_VALUE_OUT_OF_RANGE;}
				pIMSChangedPara[idx]->fMinWay = g_AxisMinMaxWay[idx].MinWay;
				pIMSChangedPara[idx]->fMaxWay = g_AxisMinMaxWay[idx].MaxWay;
			}
		}
	}
	return FRAMSaveAxisMinMaxWay(g_AxisMinMaxWay);
}

unsigned short Cmd_Drv_SetPosToMinMaxWay(unsigned short max)
{
	unsigned short error = errNone;
	unsigned short Mask;
	unsigned short ID;
	unsigned short idx;
	unsigned short i;
	double value = 0;
//test ob antrieb Homing 
	if((error = scanUShort(&Mask)) != errNone) return error;
	if(!ConfirmMask(Mask)) return ERR_WRONG_PARAMETER;
	if(!ConfirmHoming(Mask)) return ERR_DRV_NOT_CALIBRATE;
	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		g_AxisMinMaxWay[i].MinWay = pIMSChangedPara[i]->fMinWay;
		g_AxisMinMaxWay[i].MaxWay = pIMSChangedPara[i]->fMaxWay;
	}
	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if((ID = GetAxisIDFromMask(Mask, g_FelixAxes[i].AxisBit)) != 0)
		{
			//test ob antrieb Homing einfügen
			idx = GetAxisIndex(ID);
			if(idx < 0)
			{
				return ERR_UNKNOWN_DRV;
			}
			if(g_FelixAxes[idx].AxisFullRotary){return ERR_VALUE_OUT_OF_RANGE;}
			if((error = MotorGetPosition( ID, &value)) != errNone) return error;
			if(max)
			{
				g_AxisMinMaxWay[idx].MaxWay = value;
			}
			else
			{
				g_AxisMinMaxWay[idx].MinWay = value;
			}
			if(g_AxisMinMaxWay[idx].MinWay > g_AxisMinMaxWay[idx].MaxWay) {return ERR_WRONG_PARAMETER;}
		}
	}
	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if((ID = GetAxisIDFromMask(Mask, g_FelixAxes[i].AxisBit)) != 0)
		{
			idx = GetAxisIndex(ID);
			if(idx >= 0)
			{
				if(g_FelixAxes[idx].AxisFullRotary){return ERR_VALUE_OUT_OF_RANGE;}
				if(max)
				{
					pIMSChangedPara[idx]->fMaxWay = g_AxisMinMaxWay[idx].MaxWay;
				}
				else
				{
					pIMSChangedPara[idx]->fMinWay = g_AxisMinMaxWay[idx].MinWay;
				}
			}
		}
	}
	return FRAMSaveAxisMinMaxWay(g_AxisMinMaxWay);
}

unsigned short Cmd_Drv_InitSetPosMinMaxWay(void)
{
	unsigned short error = errNone;
	unsigned short Mask;
	unsigned short ID;
	unsigned short idx;
	unsigned short i;

	if((error = scanUShort(&Mask)) != errNone) return error;
	if(!ConfirmMask(Mask)) return ERR_WRONG_PARAMETER;
	if(!ConfirmHoming(Mask)) return ERR_DRV_NOT_CALIBRATE;
	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if((ID = GetAxisIDFromMask(Mask, g_FelixAxes[i].AxisBit)) != 0)
		{
			//test ob antrieb Homing einfügen
			idx = GetAxisIndex(ID);
			if(idx < 0)
			{
				return ERR_UNKNOWN_DRV;
			}
			if((error = ImsCmd_StromlosJQV( ID)) != errNone) return error;
		}
	}
	return error;
}


//#####################################################################
//#####################################################################
unsigned short Cmd_OEM(void)
{

	IMS_CommandBlock *ptr;
	unsigned short error = errNone;
//	unsigned short ID;
//	unsigned short value;
//	unsigned short state;
	unsigned short cnt;
//	char Buffer[128];
//	char strHelp[128];
	char *pStart, *pEnd;

	Msg.eCommand = eImsOEM;
	ptr = &Msg;
//######################################
	cnt = ZaehleZeichenImScanner('"');
	if((cnt == 0 ) ||  (cnt % 2 )) return ERR_NOT_ENOUGH_PARAMETER;

	pStart = strchr(GetScannerPtr(), '"');
	pStart++;
	pEnd = strrchr(GetScannerPtr(), '"');
	*pEnd = '\0';
	strcpy((char *)Msg.Argument.cBuffer, pStart);
/*	
	//if(cnt % 2 ) ERR_NOT_ENOUGH_PARAMETER;
	// erstes '"' , Begin der OEM Zeichenfolge
	state = scanner( Buffer);
	if(state != STRING) return ERR_WRONG_PARAMETER;
	strcpy((char *)Msg.Argument.cBuffer, Buffer);
	// beliebiger '"' , Inhalt der der OEM Zeichenfolge
	//if(cnt > 2)
	cnt-=2;
	do
	{
		state = scanner( Buffer);
		if(state != STRING) return ERR_WRONG_PARAMETER;
		sprintf(strHelp,"""%s",Buffer);
		strcat((char *)Msg.Argument.cBuffer, strHelp);
		cnt--;
	}while(cnt);*/
//######################################
/*
	if((error = scanUShort(&Msg.AxisID)) != errNone) return error;
	
	state = scanner( Buffer);
//	if(state != STRING) return ERR_WRONG_PARAMETER;
	if(state != STRING) return ERR_WRONG_PARAMETER;
	//(char *)ptr->Argument.cBuffer
	strcpy((char *)Msg.Argument.cBuffer, Buffer);
*/	
	K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	error = Msg.Error;
	if(error == errInfo)
	{
		sprintf(g_acAnswerBuf,"I%s\r", (char *)Msg.Argument.cBuffer);
		return errInfo;
		
	}
/*	if(error == errNone)
	{
		sprintf(g_acAnswerBuf,"I%d\r", Msg.Argument.usShort);
		return errInfo;
	}*/
	return error;
}

unsigned short Cmd_SetTestFlag(void)
{
	unsigned short error;
	unsigned short value;
	if((error = scanUShort(&value)) != errNone) return error;
	if(value == TF_OFF)
	{
		g_TestFlag = TF_OFF;
	}
	else
	{
		g_TestFlag |= value;
	}
	
	return error;
}
unsigned short Cmd_GetTestFlag(void)
{
	sprintf(g_acAnswerBuf,"I%u\r", g_TestFlag);
	return errInfo;
}

/* FUNCTION: 	Cmd_FRAMReadVersion
   PARAMETERS:	---
   DESCRIPTION:	Kommando zum Lesen der internen EEPROM Struktur
   RETURNS: 	INFO / Fehlerkode */
unsigned short Cmd_FRAMReadVersion(void)
{
	unsigned short error;
	unsigned short vers;

	if((error = FRAMReadStructVersion(&vers)) == errNone)
	{
		sprintf(g_acAnswerBuf,"I%u\r",vers);
		error = errInfo;
	}
	return error;
}

/* FUNCTION: 	Cmd_ReadSerialNo
   PARAMETERS:	---
   DESCRIPTION:	Lesen der Gerätenummer
   RETURNS: 	INFO / Fehlerkode */
unsigned short Cmd_ReadSerialNo(void)
{
	unsigned short error;
	char *cPtr;

	SAVEMALLOC_ALLOC
	cPtr = malloc(sizeof(DEV_NUMBER_FRAM));
	SAVEMALLOC_RELEASE
	if((error = FRAMReadDevNumber(cPtr)) == errNone)
	{
		sprintf(g_acAnswerBuf,"I%s\r",cPtr);
		error = errInfo;
	}
	SAVEMALLOC_ALLOC
	free(cPtr);
	SAVEMALLOC_RELEASE
	return error;
}

/* FUNCTION: 	Cmd_SetSerialNo
   PARAMETERS:	---
   DESCRIPTION:	Einschreiben der Gerätenummer
   RETURNS: 	INFO / Fehlerkode */
unsigned short Cmd_SetSerialNo(void)
{
	int value;
	/*unsigned short len;
	NextPara(); if(!*g_pcCmdBuf) return IERR_NOT_ENOUGH_PARA;
	len= strlen(g_pcCmdBuf)-1; // Endezeichen vom Kommandostring ausblenden
  	*(g_pcCmdBuf+len)= 0;
  	return FRAMSetDevNumber(g_pcCmdBuf);*/

	//unsigned short cnt;
	//char *pStart, *pEnd;
	//char buffer[DEVICE_NUMBER_LEN];

//char *GetScannerPtr(void)

	/*cnt = ZaehleZeichenImScanner('"');
	if((cnt == 0 ) ||  (cnt % 2 )) return ERR_NOT_ENOUGH_PARAMETER;
	pStart = strchr(GetScannerPtr(), '"');
	pStart++;
	pEnd = strrchr(GetScannerPtr(), '"');
	*pEnd = '\0';

	//strcpy(buffer, pStart);
	//return FRAMSetDevNumber(buffer);
	return FRAMSetDevNumber(pStart);*/
	if((value = GetC_ScannerBuf()) != ' ') {UngetC_ScannerBuf(value);}
	return FRAMSetDevNumber(GetScannerPtr());
}


unsigned short Cmd_GetFRAMCapacity(void)
{
	unsigned short total_m;
	unsigned short used_m;
	unsigned short free_m;

	FRAM_GetMemoryStatus(&total_m, &used_m, &free_m);
	sprintf(g_acAnswerBuf,"Ifram memory in byte total %u used %u free %u\r",total_m, used_m, free_m);
	return errInfo;
}
unsigned short Cmd_GetUserDataCapacity(void)
{

	unsigned short datablock_count_m;
	unsigned short datablock_size_m;
	unsigned short value = 0;

	if(scanUShort(&value) != errNone) {value = 0;}else{value = 1;}
	FRAM_GetUserDataCapacity(&datablock_count_m, &datablock_size_m);
	if(value)
	{
		sprintf(g_acAnswerBuf,"Iuser data block total %u block size in byte %u\r",datablock_count_m, datablock_size_m);
	}
	else
	{
		sprintf(g_acAnswerBuf,"I%u %u\r",datablock_count_m, datablock_size_m);
	}
	return errInfo;
}

unsigned short Cmd_SetUserData(void)
{
	unsigned short error;
	unsigned short BlkCnt;
	//unsigned short cnt;
	//char *pStart, *pEnd;
	int value;

	if((error = scanUShort(&BlkCnt)) != errNone) return error;
	if(!CheckUShort(1, DATA_BLOCK_COUNT, BlkCnt)) return ERR_WRONG_PARAMETER;
	--BlkCnt;
/*	cnt = ZaehleZeichenImScanner('"');
	if((cnt == 0 ) ||  (cnt % 2 )) return ERR_NOT_ENOUGH_PARAMETER;
	pStart = strchr(GetScannerPtr(), '"');
	pStart++;
	pEnd = strrchr(GetScannerPtr(), '"');
	*pEnd = '\0';
	return FRAMSaveUserData(BlkCnt, pStart);*/
	if((value = GetC_ScannerBuf()) != ' ') {UngetC_ScannerBuf(value);}
	return FRAMSaveUserData(BlkCnt, GetScannerPtr());


}
/*unsigned short Cmd_SetUserDataV2(void)
{
	unsigned short error;
	unsigned short BlkCnt;
	//unsigned short cnt;
	//char *pStart, *pEnd;
	unsigned short ident;
	char Buffer[CHARMEM_BLOCK_LENGTH+3];
	char Buffer2[CHARMEM_BLOCK_LENGTH+3];

	if((error = scanUShort(&BlkCnt)) != errNone) return error;
	if(!CheckUShort(1, DATA_BLOCK_COUNT, BlkCnt)) return ERR_WRONG_PARAMETER;
	--BlkCnt;
	ident = scanner(Buffer);
	if(ident != STRING) return ERR_WRONG_PARAMETER;
	Buffer[CHARMEM_BLOCK_LENGTH+3-1] = 0;
	sprintf(Buffer2,"%s",Buffer);
	return FRAMSaveUserData(BlkCnt, Buffer2);
}*/

unsigned short Cmd_GetUserData(void)
{
	unsigned short error;
	unsigned short BlkCnt;
	//char *cPtr;

	if((error = scanUShort(&BlkCnt)) != errNone) return error;
	if(!CheckUShort(1, DATA_BLOCK_COUNT, BlkCnt)) return ERR_WRONG_PARAMETER;
	--BlkCnt;

	/*SAVEMALLOC_ALLOC
	//cPtr = (char*)malloc(sizeof(udtDataBlock));
	cPtr = (char*)malloc(CHARMEM_BLOCK_LENGTH+1);
	SAVEMALLOC_RELEASE*/


	if((error = FRAMReadUserData(BlkCnt, LongBuffer)) == errNone)
	{
		sprintf(g_acAnswerBuf,"I%s\r",LongBuffer);
		error = errInfo;
	}
	/*SAVEMALLOC_ALLOC
	free(cPtr);
	SAVEMALLOC_RELEASE*/
	return error;
}

unsigned short Cmd_EraseUserData(void)
{
	unsigned short error;
	unsigned short BlkCnt;

	if((error = scanUShort(&BlkCnt)) != errNone) return error;
	if(!CheckUShort(1, DATA_BLOCK_COUNT, BlkCnt)) return ERR_WRONG_PARAMETER;
	--BlkCnt;
	return FRAMEraseUserData(BlkCnt);
}

unsigned short Cmd_FRAMDump(FILE *OutHandle)
{
	unsigned short error;
	unsigned short StartAdr;
	unsigned short EndAdr;
	if((error = scanUShort(&StartAdr)) != errNone)
	{
		StartAdr = 0;
	}
	if((error = scanUShort(&EndAdr)) != errNone)
	{
		EndAdr = FRAM_SIZE;
	}
	if(!CheckUShort(0, FRAM_SIZE, StartAdr)) return ERR_WRONG_PARAMETER;
	if(!CheckUShort(StartAdr, FRAM_SIZE, EndAdr)) return ERR_WRONG_PARAMETER;
	return FRAMDump(StartAdr, EndAdr, OutHandle);
}

unsigned short Cmd_SetLockExtOutEStop(void)
{
	unsigned short error;
	unsigned short value;
	//udtDeviceConfig DeviceConfig;
	unsigned short tmp;

	if((error = scanOnOff(&value)) != errNone) return error;
	tmp = g_DeviceConfig.LockExternOutEstopEnable;
	g_DeviceConfig.LockExternOutEstopEnable = value;
	if((error = FRAMSaveDeviceConfig(&g_DeviceConfig)) == errNone)
	{
		SetLockEstopOut(g_DeviceConfig);
		//error = AllMotorInitS2();
		/*K_Event_Reset(UserMainSlot, SLOMO_EVENT | SLOMO_RESET_EVENT | ESTOP_EVENT | ESTOP_RESET_EVENT | TA_SERVICE_EVENT);
		K_Event_Reset(MotorSlot, SLOMO_EVENT | SLOMO_RESET_EVENT | ESTOP_EVENT | ESTOP_RESET_EVENT | TA_SERVICE_EVENT);
		K_Event_Reset(ASyncCtrlSlot, SLOMO_EVENT | SLOMO_RESET_EVENT | ESTOP_EVENT | ESTOP_RESET_EVENT | TA_SERVICE_EVENT);
		K_Event_Reset(HardwareSlot, SLOMO_EVENT | SLOMO_RESET_EVENT | ESTOP_EVENT | ESTOP_RESET_EVENT | TA_SERVICE_EVENT);*/

		K_Event_Reset(UserMainSlot, SLOMO_EVENT | SLOMO_RESET_EVENT | ESTOP_EVENT | ESTOP_RESET_EVENT);
		K_Event_Reset(MotorSlot, SLOMO_EVENT | SLOMO_RESET_EVENT | ESTOP_EVENT | ESTOP_RESET_EVENT);
		K_Event_Reset(ASyncCtrlSlot, SLOMO_EVENT | SLOMO_RESET_EVENT | ESTOP_EVENT | ESTOP_RESET_EVENT);
		K_Event_Reset(HardwareSlot, SLOMO_EVENT | SLOMO_RESET_EVENT | ESTOP_EVENT | ESTOP_RESET_EVENT);

		return error;
	}
	g_DeviceConfig.LockExternOutEstopEnable = tmp;
	return error;
}

unsigned short Cmd_GetLockExtOutEStop(void)
{
	if(g_DeviceConfig.LockExternOutEstopEnable)
	{
		sprintf(g_acAnswerBuf,"Ion\r");
	}
	else
	{
		sprintf(g_acAnswerBuf,"Ioff\r");
	}
	return errInfo;	
}

/*unsigned short Cmd_SetExtInEStop(void)
{
*	unsigned short error;
	unsigned short value;
	unsigned short tmp;*
return ERR_CMD_NOT_PRACTICABLE;
*
	if((error = scanOnOff(&value)) != errNone) return error;
	tmp = g_DeviceConfig.ExternEstopInEnable;
	g_DeviceConfig.ExternEstopInEnable = value;
	if((error = FRAMSaveDeviceConfig(&g_DeviceConfig)) == errNone)
	{
		SetEstopIn(g_DeviceConfig);
		//error = AllMotorInitS2();
		*K_Event_Reset(UserMainSlot, SLOMO_EVENT | SLOMO_RESET_EVENT | ESTOP_EVENT | ESTOP_RESET_EVENT | TA_SERVICE_EVENT);
		K_Event_Reset(MotorSlot, SLOMO_EVENT | SLOMO_RESET_EVENT | ESTOP_EVENT | ESTOP_RESET_EVENT | TA_SERVICE_EVENT);
		K_Event_Reset(ASyncCtrlSlot, SLOMO_EVENT | SLOMO_RESET_EVENT | ESTOP_EVENT | ESTOP_RESET_EVENT | TA_SERVICE_EVENT);
		K_Event_Reset(HardwareSlot, SLOMO_EVENT | SLOMO_RESET_EVENT | ESTOP_EVENT | ESTOP_RESET_EVENT | TA_SERVICE_EVENT);*

		K_Event_Reset(UserMainSlot, SLOMO_EVENT | SLOMO_RESET_EVENT | ESTOP_EVENT | ESTOP_RESET_EVENT);
		K_Event_Reset(MotorSlot, SLOMO_EVENT | SLOMO_RESET_EVENT | ESTOP_EVENT | ESTOP_RESET_EVENT);
		K_Event_Reset(ASyncCtrlSlot, SLOMO_EVENT | SLOMO_RESET_EVENT | ESTOP_EVENT | ESTOP_RESET_EVENT);
		K_Event_Reset(HardwareSlot, SLOMO_EVENT | SLOMO_RESET_EVENT | ESTOP_EVENT | ESTOP_RESET_EVENT);

		return error;
	}
	g_DeviceConfig.ExternEstopInEnable = tmp;
	return error;
*
}*/
unsigned short Cmd_GetExtInEStop(void)
{
	if(g_DeviceConfig.ExternEstopInEnable)
	{
		sprintf(g_acAnswerBuf,"Ion\r");
	}
	else
	{
		sprintf(g_acAnswerBuf,"Ioff\r");
	}
	return errInfo;	
}
/*unsigned short Cmd_SetIntEStop(void)
{
	unsigned short error;
	unsigned short value;
	//udtDeviceConfig DeviceConfig;
	unsigned short tmp;

	if((error = scanOnOff(&value)) != errNone) return error;
	tmp = g_DeviceConfig.InternEstopEnable;
	g_DeviceConfig.InternEstopEnable = value;
	if((error = FRAMSaveDeviceConfig(&g_DeviceConfig)) == errNone)
	{
		SetEstopIn(g_DeviceConfig);
		//error = AllMotorInitS2();
		K_Event_Reset(UserMainSlot, SLOMO_EVENT | SLOMO_RESET_EVENT | ESTOP_EVENT | ESTOP_RESET_EVENT);
		K_Event_Reset(MotorSlot, SLOMO_EVENT | SLOMO_RESET_EVENT | ESTOP_EVENT | ESTOP_RESET_EVENT);
		K_Event_Reset(ASyncCtrlSlot, SLOMO_EVENT | SLOMO_RESET_EVENT | ESTOP_EVENT | ESTOP_RESET_EVENT);
		K_Event_Reset(HardwareSlot, SLOMO_EVENT | SLOMO_RESET_EVENT | ESTOP_EVENT | ESTOP_RESET_EVENT);

		return error;
	}
	g_DeviceConfig.InternEstopEnable = tmp;
	return error;
}*/

unsigned short Cmd_GetIntEStop(void)
{
	if(g_DeviceConfig.InternEstopEnable)
	{
		sprintf(g_acAnswerBuf,"Ion\r");
	}
	else
	{
		sprintf(g_acAnswerBuf,"Ioff\r");
	}
	return errInfo;	

}

unsigned short Cmd_SetBreake(void)
{
	/*	Msg.eCommand = eZBreakeFixed;
	K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	return Msg.Error;
	*/
	return ImsCmd_SetBreake();

}
unsigned short Cmd_ClearBreake(void)
{
	/*Msg.eCommand = eZBreakeFree;
	K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	return Msg.Error;
	*/
	return ImsCmd_ClearBreake();
}

unsigned short Cmd_SlewInit(void)
{
	unsigned short error = errNone;
	//unsigned short ID;
	unsigned short Mask;
	//unsigned short i;

	if((error = scanUShort(&Mask)) != errNone) return error;
	if(!ConfirmMask(Mask)) return ERR_WRONG_PARAMETER;
	if((error = ImsCmd_SlewInitAll(Mask)) != errNone) return error;
	/*for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if((ID = GetAxisIDFromMask(Mask, g_FelixAxes[i].AxisBit)) != 0)
		{
	  		if((error = ImsCmd_SlewInitAll( Mask)) != errNone) return error;
		}
	}*/
	return error;
}
unsigned short Cmd_SlewExit(void)
{
	unsigned short error = errNone;
	//unsigned short ID;
	unsigned short Mask;
	//unsigned short i;

	if((error = scanUShort(&Mask)) != errNone) return error;
	if(!ConfirmMask(Mask)) return ERR_WRONG_PARAMETER;
	if((error = ImsCmd_SlewExitAll( Mask)) != errNone) return error;
	/*for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if((ID = GetAxisIDFromMask(Mask, g_FelixAxes[i].AxisBit)) != 0)
		{
			if((error = ImsCmd_SlewExitId( ID)) != errNone) return error;
		}
	}*/
	return error;
}

unsigned short Cmd_Slew(void)
{
	unsigned short error = errNone;
	unsigned short Mask;
	//unsigned short cnt;
	unsigned short i;
	unsigned short ID;
	unsigned short idx;
	long value;

	if((error = scanUShort(&Mask)) != errNone) return error;
	if(!ConfirmMask(Mask)) return ERR_WRONG_PARAMETER;
	#ifdef _CF_DEBUG_PRINT
	if(g_TestFlag & TF_DBG_INT_COM){dbPrint("\r");}
	#endif
	for(i=0; i < MAX_AXES; i++)
	{
		WDO();
		if((ID = GetAxisIDFromMask(Mask, g_FelixAxes[i].AxisBit)) != 0)
		{
			if((error = scanLong(&value)) != errNone) return error;
			idx = GetAxisIndex(ID);
			if(idx < 0)
			{
				return ERR_UNKNOWN_DRV;
			}
			if((error = ImsCmd_SlewMoveID( ID, value)) != errNone) return error;
		}
	}
	return error;
}

/*unsigned short Cmd_HandMove(unsigned short Start)
{
	unsigned short error = errNone;
	unsigned short Mask;
	unsigned short ID;
	unsigned short idx;
	unsigned short i;

	if((error = scanUShort(&Mask)) != errNone) return error;
	if(!ConfirmMask(Mask)) return ERR_WRONG_PARAMETER;
	error = ERR_WRONG_PARAMETER;
	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if((ID = GetAxisIDFromMask(Mask, g_FelixAxes[i].AxisBit)) != 0)
		{
			idx = GetAxisIndex(ID);
			if(idx < 0)
			{
				return ERR_UNKNOWN_DRV;
			}
			Msg.Argument.usShort = idx;
			error = errNone;
		}
	}
	if(error != errNone) return error;
	if(Start)
	{
		Msg.eCommand = eTestHandStart;
	}
	else
	{
		Msg.eCommand = eTestHandStop;
	}
	K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	return Msg.Error;
}*/

unsigned short Cmd_SetPort(unsigned short isSetPort)
{
	unsigned short error = errNone;
	unsigned short PortBit;
	if((error = scanUShort(&PortBit)) != errNone) return error;
	if(isSetPort)
	{
		SetPortBit(PortBit);
	}
	else
	{
		ClearPortBit(PortBit);
	}
	return error;
}

unsigned short Cmd_TestSetPort(unsigned short isSetPort)
{
	unsigned short error = errNone;
	unsigned short PortNr;
	unsigned short PortBit;
	if((error = scanUShort(&PortNr)) != errNone) return error;
	if(PortNr == 0)
	{
		if((error = scanUShort(&PortBit)) != errNone) return ERR_WRONG_PARAMETER;
		if(isSetPort)
		{
			TestSetPortBit(PortBit);
		}
		else
		{
			TestClearPortBit(PortBit);
		}
	}
	else
	{
		if((error = scanUShort(&PortBit)) != errNone) return ERR_WRONG_PARAMETER;
		if((PortNr == 93) || (PortNr == 94) || (PortNr == 95) || (PortNr == 96))
		{
			if(PortNr == 93){SetDesignLightCCU63_CC60_P93(PortBit);return error;}
			if(PortNr == 94){SetDesignLightCCU63_CC61_P94(PortBit);return error;}
			if(PortNr == 95){SetDesignLightCCU63_CC62_P95(PortBit);return error;}
			if(PortNr == 96){SetDesignLightCCU63_CC63_P96(PortBit);return error;}
			return error;
		}
		switch(PortNr)
		{
			case 1	:	if(PortBit && isSetPort){LED_Red_On();}
						if((!PortBit && isSetPort) || !isSetPort){LED_Red_Off();}
						break;
			case 2	:	if(PortBit && isSetPort){LED_Green_On();}
						if((!PortBit && isSetPort) || !isSetPort){LED_Green_Off();}
						break;
			case 3	:	LED_RedGreen_Off();
						break;
			case 4	:	if(PortBit && isSetPort){LED_Yellow_On();}
						if((!PortBit && isSetPort) || !isSetPort){LED_Yellow_Off();}
						break;
			case 5	:	LED_Green_SLowBlink();
						break;
			case 6	:	LED_Green_FastBlink();
						break;
			case 7	:	LED_Green_SuperFastBlink();
						break;
			case 8	:	LED_Yellow_SlowBlink();
						break;
			case 9	:	LED_Yellow_FastBlink();
						break;
			case 10	:	LED_Yellow_SuperFastBlink();
						break;
			case 11	:	xHW_LED_RedGreen_Off();
						break;
			case 12	:	xHW_LED_Red_On();
						break;
			case 13	:	xHW_LED_Green_On();
						break;
			case 14	:	xHW_LED_Yellow_On();
						break;
			case 15	:	xHW_LED_Yellow_Off();
						break;
			case 16	:	//P1_OUT_P2	// low active
						if(PortBit && isSetPort){SW_RESERVE = 1;}
						if((!PortBit && isSetPort) || !isSetPort){SW_RESERVE = 0;}
						break;
			case 17	:	//P1_OUT_P3	// low active
						if(PortBit && isSetPort){Z_BRAKE = 1;}
						if((!PortBit && isSetPort) || !isSetPort){Z_BRAKE = 0;}
						break;
			case 18	:	//P2_OUT_P0	// low active
						if(PortBit && isSetPort){RESET_LANPORT = 1;}
						if((!PortBit && isSetPort) || !isSetPort){RESET_LANPORT = 0;}
						break;
			case 19	:	//P9_OUT_P1	// hight active
						if(PortBit && isSetPort){LED_1 = 1;}
						if((!PortBit && isSetPort) || !isSetPort){LED_1 = 0;}
						break;
			case 20	:	//P9_OUT_P2	// hight active
						if(PortBit && isSetPort){LED_2 = 1;}
						if((!PortBit && isSetPort) || !isSetPort){LED_2 = 0;}
						break;
			case 21	:	//P9_OUT_P7	// hight active
						if(PortBit && isSetPort){LED_3 = 1;}
						if((!PortBit && isSetPort) || !isSetPort){LED_3 = 0;}
						break;
			case 22	:	//P0_OUT_P6	// hight is on
						if(PortBit && isSetPort){SET_VCC_OEM = 1;}
						if((!PortBit && isSetPort) || !isSetPort){SET_VCC_OEM = 0;}
						break;
			case 23	:	//P0_OUT_P7	// high is npn to vcc, low is pnp to gnd
						if(PortBit && isSetPort){SET_OEM_IN = 1;}
						if((!PortBit && isSetPort) || !isSetPort){SET_OEM_IN = 0;}
						break;
			case 24	:	//P10_OUT_P4	// hight is on
						if(PortBit && isSetPort){SET_PHOTOMOS = 1;}
						if((!PortBit && isSetPort) || !isSetPort){SET_PHOTOMOS = 0;}
						break;
			case 25	:	//P1_OUT_P1	// hight is on
						if(PortBit && isSetPort){SET_VCC_UVL = 1;}
						if((!PortBit && isSetPort) || !isSetPort){SET_VCC_UVL = 0;}
						break;
			case 26	:	//P1_OUT_P7	// high is npn to vcc, low is pnp to gnd
						if(PortBit && isSetPort){SET_UVL_IN = 1;}
						if((!PortBit && isSetPort) || !isSetPort){SET_UVL_IN = 0;}
						break;
			case 27	:	//P1_OUT_P4	// hight is on
						if(PortBit && isSetPort){SET_SW_UVL = 1;}
						if((!PortBit && isSetPort) || !isSetPort){SET_SW_UVL = 0;}
						break;
			case 28	:	//P10_OUT_P3	// high active
						if(PortBit && isSetPort){MUELL_CTRL = 1;}
						if((!PortBit && isSetPort) || !isSetPort){MUELL_CTRL = 0;}
						break;
//#ifndef _CF_NRTL
			case 29	:	//
						if(!isNRTL_LP())
						{
							if(PortBit && isSetPort){CLEAR_FW_ESTOP = 1;}
							if((!PortBit && isSetPort) || !isSetPort){CLEAR_FW_ESTOP = 0;}
						}
						else
						{error = ERR_WRONG_PARAMETER;}
						break;
			case 30	:	// P10_OUT_P9	// low active
						if(!isNRTL_LP())
						{
							if(PortBit && isSetPort){LOCK_ESTOP_EXTERN = 1;}
							if((!PortBit && isSetPort) || !isSetPort){LOCK_ESTOP_EXTERN = 0;}
						}
						else
						{error = ERR_WRONG_PARAMETER;}
						break;
			case 31	:	// P10_OUT_P10	// low active
						if(!isNRTL_LP())
						{
							if(PortBit && isSetPort){DISABLE_ESTOP_INTERN = 1;}
							if((!PortBit && isSetPort) || !isSetPort){DISABLE_ESTOP_INTERN = 0;}
						}
						else
						{error = ERR_WRONG_PARAMETER;}
						break;
			case 32	:	// P10_OUT_P11	// low active
						if(!isNRTL_LP())
						{
							if(PortBit && isSetPort){DISABLE_ESTOP_EXTERN = 1;}
							if((!PortBit && isSetPort) || !isSetPort){DISABLE_ESTOP_EXTERN = 0;}
						}
						else
						{error = ERR_WRONG_PARAMETER;}
						break;
			case 33	:	// P1_OUT_P0	// hight active
						if(!isNRTL_LP())
						{
							if(PortBit && isSetPort){SET_FF = 1;}
							if((!PortBit && isSetPort) || !isSetPort){SET_FF = 0;}
						}
						else
						{error = ERR_WRONG_PARAMETER;}
						break;
//#else
			case 34	:	// P2_OUT_P11
						if(isNRTL_LP())
						{
							if(PortBit && isSetPort){FW_LOCK_DN = 1;}
							if((!PortBit && isSetPort) || !isSetPort){FW_LOCK_DN = 0;}
						}
						else
						{error = ERR_WRONG_PARAMETER;}
						break;
			case 35	:	// P1_OUT_P0
						if(isNRTL_LP())
						{
							if(PortBit && isSetPort){FW_LOCK_UP = 1;}
							if((!PortBit && isSetPort) || !isSetPort){FW_LOCK_UP = 0;}
						}
						else
						{error = ERR_WRONG_PARAMETER;}
						break;
			case 36	:	xHW_LED_Reserve_On();
						break;
//#endif
					// 93 - 96 reserved
					default	:	error = ERR_WRONG_PARAMETER;
								break;
		}//end..switch
	}//ende...else
	return error;
}

unsigned short Cmd_TestReadSensor(char *sPtr, FILE *InHandle)
{
	unsigned short error = errNone;
	unsigned short SensorAdr;
	short Value = 0;
	unsigned short PrintSensors;
	if((error = scanUShort(&SensorAdr)) != errNone) return error;
	if(scanUShort(&PrintSensors) != errNone)
	{
		PrintSensors = 0;
	}
	switch(SensorAdr)
	{
		case 1:		//P5_IN_P14	// low active	
					Value = JALMON1;break;
		//#ifdef _CF_NRTL
		case 2:		//P5_IN_P15	// low active
					if(isNRTL_LP())
					{
						Value = ESTOP_EXT;
					}
					else
					{
						Value = JALMON2;
					}
					break;
		case 20:	if(isNRTL_LP())
					{
						//P10_IN_P10
						Value = UEW_FET1_24V;
					}
					else
					{
						error= ERR_WRONG_PARAMETER;
					}
					break;
		case 21:	if(isNRTL_LP())
					{
						//P10_IN_P11
						Value = UEW_FET2_24V;
					}
					else
					{
						error= ERR_WRONG_PARAMETER;
					}
					break;
		case 22:	if(isNRTL_LP())
					{
						//P10_IN_P8
						Value = UEW_FET3_48V;
					}
					else
					{
						error= ERR_WRONG_PARAMETER;
					}
					break;
		case 23:	if(isNRTL_LP())
					{
						//P10_IN_P9
						Value = UEW_FET4_48V;
					}
					else
					{
						error= ERR_WRONG_PARAMETER;
					}
					break;
		case 24:	if(isNRTL_LP())
					{
						//P6_IN_P2
						Value = IS_LP_201;						
					}
					else
					{
						error= ERR_WRONG_PARAMETER;
					}

					break;
		//#else
		/*case 2:		//P5_IN_P15	// low active
					Value = JALMON2;break;*/
		case 10:	
					if(isNRTL_LP())
					{
						error= ERR_WRONG_PARAMETER;
					}
					else
					{
						//P2_IN_P2	// hight active
						Value = SLO_MO_MODE;
					}
					break;
		//#endif
		case 3:		Value = (unsigned short)GetDilGroup();break;
		case 4:		Value = getCTS_COM2_UART2();break;
		case 5:		Value = getCTS_COM3_UART3();break;
		case 6:		Value = getCTS_COM4_UART4();break;
		case 7:		//P2_IN_P3	// hight active
					Value = TA_SERVICE;break;
		case 8:		//P2_IN_P5	// hight active
					Value = TA2_SERVICE;break;
		case 9:		//P2_IN_P1	// low active
					Value = ESTOP;break;

		case 11:	//P2_IN_P4	// low active
					Value = LEVEL_DETECT;break;
		case 12: 	Value = Get_Ex0Flag();Reset_Ex0Flag();break;
		case 13: 	Value = Get_Ex1Flag();Reset_Ex1Flag();break;
		case 14:	//P2_IN_P5
					Value = DOPPEL_TASTEN_UI;break;
		case 15:	//P6_IN_P2
					Value = NRTL_LP;break;
		default:error= ERR_WRONG_PARAMETER;break;
	}
	if(PrintSensors)
	{
		while(1){
			printf("\r##########################################################\r");
			//----------------------
			printf("Dil 1-1 : ");
			if(DIL1_1){printf("open - off\r");}else{printf("closed - on\r");}fflush(stdout);
			printf("Dil 1-2 : ");
			if(DIL1_2){printf("open - off\r");}else{printf("closed - on\r");}fflush(stdout);
			printf("Dil 1-3 : ");
			if(DIL1_3){printf("open - off\r");}else{printf("closed - on\r");}fflush(stdout);
			printf("Dil 1-4 : ");
			if(DIL1_4){printf("open - off\r");}else{printf("closed - on\r");}fflush(stdout);
			WDO();
			WDO();
			//----------------------
			printf("\rJalmon 1       : ");
			if(JALMON1){printf("high\r");}else{printf("low\r");}fflush(stdout);
			WDO();
			//#ifdef _CF_NRTL
			if(isNRTL_LP())
			{
				printf("\rESTOP_EXT      : ");
				if(ESTOP_EXT){printf("high\r");}else{printf("low\r");}fflush(stdout);
			}
			//#else
			{
				printf("\rJalmon 2       : ");
				if(JALMON2){printf("high\r");}else{printf("low\r");}fflush(stdout);
			}
			//#endif

			WDO();

			printf("\rCOM2 - CTS2    : ");
			if(getCTS_COM2_UART2()){printf("high\r");}else{printf("low\r");}fflush(stdout);
			WDO();
			printf("\rCOM3 - CTS3    : ");
			if(getCTS_COM3_UART3()){printf("high\r");}else{printf("low\r");}fflush(stdout);
			WDO();
			printf("\rCOM4 - CTS4    : ");
			if(getCTS_COM4_UART4()){printf("high\r");}else{printf("low\r");}fflush(stdout);
			WDO();

			printf("\rP2.1 EX0 ESTOP : ");
			if(Get_Ex0Flag()){printf("EDGE\r");Reset_Ex0Flag();}else{printf("no edge\r");}fflush(stdout);
			WDO();
			printf("\rP2.2 EX1 SLO_MO_MODE : ");
			if(Get_Ex1Flag()){printf("EDGE\r");Reset_Ex1Flag();}else{printf("no edge\r");}fflush(stdout);
			WDO();
			//----------------------
			K_Task_Wait(2000);
			//if(UART_GetEvents(InHandle) & EVENT_USER_BREAK) {error = ERR_PROG_ABORT;break;}
			if(UART_GetEvents(InHandle) & COM_EVENT_USER_BREAK) {error = ERR_PROG_ABORT;break;}
		}
	}
	else
	{
		if(error == errNone)
		{
			sprintf(sPtr,"I%d\r", Value);
			error = errInfo;
		}
	}
	return error;
}
//#####################################################################
//#####################################################################	

unsigned short Cmd_SwitchCtrl(void)
{
	unsigned short error = errNone;
	unsigned short Ports;
	unsigned short Bits;

	if((error = scanUShort(&Ports)) != errNone) return error;
	if((error = scanUShort(&Bits)) != errNone) return error;
	return Switchers(Ports, Bits);
}

unsigned short ConfirmHoming(unsigned short Mask)
{
	unsigned short ID;
	unsigned short i;

	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if((ID = GetAxisIDFromMask(Mask, g_FelixAxes[i].AxisBit)) != 0)
		{
			//test ob antrieb Homing einfügen
			if(g_GetAxes_HomingFlag(i) != FLG_HOMING_OK)
			{
				return FALSE;
			}
		}
	}
	return TRUE;
}

unsigned short Cmd_SetSensorConfig(void)
{
	unsigned short error;
	unsigned short SensorAdr, Value;

	if(!gDoppelTasteUI) return ERR_CMD_NOT_IMPLEMENTED;
	if((error = scanUShort(&SensorAdr)) != errNone) return error;
	if((error = scanUShort(&Value)) != errNone) return error;
	switch(SensorAdr)
	{
		case SENSOR_GET_OEM_IN1:g_SensorConfig.SetPort_OEM_IN = Value;
								SET_OEM_IN = Value;
								break;
		case SENSOR_GET_UVL_IN:	g_SensorConfig.SetPort_UVL_IN = Value;
								SET_UVL_IN = Value;
								break;
		default:				error = ERR_WRONG_PARAMETER;
								break;
	}
	if(error == errNone)
	{
		error = FRAMSaveSensorConfig(&g_SensorConfig);
	}
	return error;
}
unsigned short Cmd_GetSensorConfig(void)
{
	unsigned short error;
	unsigned short SensorAdr, Value;

	if(!gDoppelTasteUI) return ERR_CMD_NOT_IMPLEMENTED;
	if((error = scanUShort(&SensorAdr)) != errNone) return error;
	switch(SensorAdr)
	{
		case SENSOR_GET_OEM_IN1:Value = g_SensorConfig.SetPort_OEM_IN;
								break;
		case SENSOR_GET_UVL_IN:	Value = g_SensorConfig.SetPort_UVL_IN;
								break;
		default:				error = ERR_WRONG_PARAMETER;
								break;
	}
	if(error == errNone)
	{
		sprintf(g_acAnswerBuf,"I%u\r",Value);
		error = errInfo;
	}
	return error;
}
unsigned short Cmd_ReadSensor(void)
{
	unsigned short error = errNone;
	unsigned short SensorAdr;
	unsigned short Value = 0;

	if(!gDoppelTasteUI) return ERR_CMD_NOT_IMPLEMENTED;
	if((error = scanUShort(&SensorAdr)) != errNone) return error;
	if((error = ReadSensor(SensorAdr, &Value)) != errNone) return error;
	sprintf(g_acAnswerBuf,"I%u\r", Value);
	return errInfo;
}


unsigned short Cmd_ReadLimitSwitch(void)
{
	unsigned short error = errNone;
	unsigned short i;
	char strHelp[20];
	unsigned short ID;
	unsigned short Mask;
	unsigned short LimitSwitch;
	//float Value = 0;
	unsigned short Value = 0;

	if((error = scanUShort(&LimitSwitch)) != errNone) return error;
	if((error = scanUShort(&Mask)) != errNone) return error;
	if(!ConfirmMask(Mask)) return ERR_WRONG_PARAMETER;

	sprintf(g_acAnswerBuf,"I"); strHelp[0]=0;

	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if((ID = GetAxisIDFromMask(Mask, g_FelixAxes[i].AxisBit)) != 0)
		{
			if((error = ImsCmd_GetHomeSwitch( ID, &Value)) != errNone) return error;
			sprintf(strHelp,"%u ",Value); strcat(g_acAnswerBuf,strHelp);
		}
		else
		{
			return ERR_WRONG_PARAMETER;
		}
	}
	g_acAnswerBuf[strlen(g_acAnswerBuf)-1]=0; // Letztes Leerzeichen ausblenden
  	strcat(g_acAnswerBuf,"\r");
	return errInfo;
}

unsigned short Cmd_SetPortConfig(void)
{
	unsigned short error;
	unsigned short PortAdr, PowerOn, StateAfterQuit;

	if(!gDoppelTasteUI) return ERR_CMD_NOT_IMPLEMENTED;
	if((error = scanUShort(&PortAdr)) != errNone) return error;
	if((error = scanUShort(&PowerOn)) != errNone) return error;
	if((error = scanUShort(&StateAfterQuit)) != errNone) return error;
	switch(PortAdr)
	{
		case PORT_SET_PHOTOMOS:	g_PortConfig.SetPort_PHOTOMOS.PowerOn = PowerOn;
								g_PortConfig.SetPort_PHOTOMOS.StateAfterQuit = StateAfterQuit;
								SET_PHOTOMOS = PowerOn;
								break;
		case PORT_SET_VCC_OEM:	g_PortConfig.SetPort_VCC_OEM.PowerOn = PowerOn;
								g_PortConfig.SetPort_VCC_OEM.StateAfterQuit = StateAfterQuit;
								SET_VCC_OEM = PowerOn;
								break;
		case PORT_SET_SW_UVL:	g_PortConfig.SetPort_SW_UVL.PowerOn = PowerOn;
								g_PortConfig.SetPort_SW_UVL.StateAfterQuit = StateAfterQuit;
								SET_SW_UVL = PowerOn;
								break;
		case PORT_SET_VCC_UVL:	g_PortConfig.SetPort_VCC_UVL.PowerOn = PowerOn;
								g_PortConfig.SetPort_VCC_UVL.StateAfterQuit = StateAfterQuit;
								SET_VCC_UVL = PowerOn;
								break;
		case PORT_MUELL_CTRL:	g_PortConfig.SetPort_MUELL_CTRL.PowerOn = PowerOn;
								g_PortConfig.SetPort_MUELL_CTRL.StateAfterQuit = StateAfterQuit;
								MUELL_CTRL = PowerOn;
								break;
		case PORT_SET_OEM_IN:	g_PortConfig.SetPort_OEM_IN.PowerOn = PowerOn;
								g_PortConfig.SetPort_OEM_IN.StateAfterQuit = StateAfterQuit;
								SET_OEM_IN = PowerOn;
								break;
		case PORT_SET_UVL_IN:	g_PortConfig.SetPort_UVL_IN.PowerOn = PowerOn;
								g_PortConfig.SetPort_UVL_IN.StateAfterQuit = StateAfterQuit;
								SET_UVL_IN = PowerOn;
								break;
		case PORT_SW_RESERVE:	g_PortConfig.SetPort_SW_RESERVE.PowerOn = PowerOn;
								g_PortConfig.SetPort_SW_RESERVE.StateAfterQuit = StateAfterQuit;
								SW_RESERVE = PowerOn;
								break;
		default:				error = ERR_WRONG_PARAMETER;
								break;
	}
	if(error == errNone)
	{
		error = FRAMSavePortConfig(&g_PortConfig);
	}
	return error;
}
unsigned short Cmd_GetPortConfig(void)
{
	unsigned short error;
	unsigned short PortAdr, PowerOn, StateAfterQuit;

	if(!gDoppelTasteUI) return ERR_CMD_NOT_IMPLEMENTED;
	if((error = scanUShort(&PortAdr)) != errNone) return error;
	switch(PortAdr)
	{
		case PORT_SET_PHOTOMOS:	PowerOn = g_PortConfig.SetPort_PHOTOMOS.PowerOn;
								StateAfterQuit = g_PortConfig.SetPort_PHOTOMOS.StateAfterQuit;
								break;
		case PORT_SET_VCC_OEM:	PowerOn = g_PortConfig.SetPort_VCC_OEM.PowerOn;
								StateAfterQuit = g_PortConfig.SetPort_VCC_OEM.StateAfterQuit;
								break;
		case PORT_SET_SW_UVL:	PowerOn = g_PortConfig.SetPort_SW_UVL.PowerOn;
								StateAfterQuit = g_PortConfig.SetPort_SW_UVL.StateAfterQuit;
								break;
		case PORT_SET_VCC_UVL:	PowerOn = g_PortConfig.SetPort_VCC_UVL.PowerOn;
								StateAfterQuit = g_PortConfig.SetPort_VCC_UVL.StateAfterQuit;
								break;
		case PORT_MUELL_CTRL:	PowerOn = g_PortConfig.SetPort_MUELL_CTRL.PowerOn;
								StateAfterQuit = g_PortConfig.SetPort_MUELL_CTRL.StateAfterQuit;
								break;
		case PORT_SET_OEM_IN:	PowerOn = g_PortConfig.SetPort_OEM_IN.PowerOn;
								StateAfterQuit = g_PortConfig.SetPort_OEM_IN.StateAfterQuit;
								break;
		case PORT_SET_UVL_IN:	PowerOn = g_PortConfig.SetPort_UVL_IN.PowerOn;
								StateAfterQuit = g_PortConfig.SetPort_UVL_IN.StateAfterQuit;
								break;
		case PORT_SW_RESERVE:	PowerOn = g_PortConfig.SetPort_SW_RESERVE.PowerOn;
								StateAfterQuit = g_PortConfig.SetPort_SW_RESERVE.StateAfterQuit;
								break;
		default:				error = ERR_WRONG_PARAMETER;
								break;
	}
	if(error == errNone)
	{
		sprintf(g_acAnswerBuf,"I%u %u\r",PowerOn, StateAfterQuit);
		error = errInfo;
	}
	return error;
}
unsigned short Cmd_TestPorts(void)
{
	unsigned short error;
	unsigned short errorbit = 0x0000;
	unsigned short Value;
	unsigned short TestOn, PortTest;
	char strHelpIn1[15];
	char strHelpIn2[15];

	if(!gDoppelTasteUI) return ERR_CMD_NOT_IMPLEMENTED;
	if((error = scanUShort(&PortTest)) != errNone)
	{
		PortTest = 0x0003;TestOn = 2;
	}
	else
	{
		if((error = scanOnOff(&TestOn)) != errNone){TestOn = 2;}
	}
	if(!TestOn)
	{
		SensorAndPortPowerOnSetting();
		return errNone;
	}
	// Input as pull up
	if(PortTest & 0x0002){SET_OEM_IN = 1;}
	if(PortTest & 0x0001){SET_UVL_IN = 1;}

	// VCC_OEM and VCC_UVL einschalten,  Pruefmittel LED leuchtet
	//Switchers( bitPORT_SET_VCC_OEM | bitPORT_SET_VCC_UVL, bitPORT_SET_VCC_OEM | bitPORT_SET_VCC_UVL);
	if(PortTest & 0x0002){Switchers( bitPORT_SET_VCC_OEM, bitPORT_SET_VCC_OEM);}
	if(PortTest & 0x0001){Switchers( bitPORT_SET_VCC_UVL, bitPORT_SET_VCC_UVL);}

	// PhotoMOS und SW_UVL auschalten
	//Switchers(bitPORT_SET_PHOTOMOS | bitPORT_SET_SW_UVL, 0);
	if(PortTest & 0x0002){Switchers(bitPORT_SET_PHOTOMOS, 0);}
	if(PortTest & 0x0001){Switchers(bitPORT_SET_SW_UVL, 0);}
	K_Task_Wait(100);
	// Inputs OEM_IN und UVL_IN pruefen, low wird erwartet
	if(PortTest & 0x0002)
	{
		if((error = ReadSensor(SENSOR_GET_OEM_IN1, &Value)) != errNone) return error;
		if(Value){errorbit |= 0x0002;}
	}
	if(PortTest & 0x0001)
	{
		if((error = ReadSensor(SENSOR_GET_UVL_IN, &Value)) != errNone) return error;
		if(Value){errorbit |= 0x0001;}
	}
	// PhotoMOS und SW_UVL einchalten
	//Switchers(bitPORT_SET_PHOTOMOS | bitPORT_SET_SW_UVL, bitPORT_SET_PHOTOMOS | bitPORT_SET_SW_UVL);
	if(PortTest & 0x0002){Switchers(bitPORT_SET_PHOTOMOS, bitPORT_SET_PHOTOMOS);}
	if(PortTest & 0x0001){Switchers(bitPORT_SET_SW_UVL, bitPORT_SET_SW_UVL);}

	K_Task_Wait(100);
	// Inputs OEM_IN und UVL_IN pruefen, high wird erwartet
	if(PortTest & 0x0002)
	{
		if((error = ReadSensor(SENSOR_GET_OEM_IN1, &Value)) != errNone) return error;
		if(!Value){errorbit |= 0x0020;}
	}
	if(PortTest & 0x0001)
	{
		if((error = ReadSensor(SENSOR_GET_UVL_IN, &Value)) != errNone) return error;
		if(!Value){errorbit |= 0x0010;}
	}
	//sprintf(g_acAnswerBuf,"I");
	strHelpIn1[0]=0;strHelpIn2[0]=0;
	if(PortTest & 0x0002)
	{
		if(errorbit & (0x0002 | 0x0020))
		{
			sprintf(strHelpIn2,"ERROR %X", errorbit & (0x0002 | 0x0020));
		}
		else
		{
			sprintf(strHelpIn2,"OK");
		}
	}
	if(PortTest & 0x0001)
	{
		if(errorbit & (0x0001 | 0x0010))
		{
			sprintf(strHelpIn1,"ERROR %X", errorbit & (0x0001 | 0x0010));
		}
		else
		{
			sprintf(strHelpIn1,"OK");
		}
	}
	//sprintf(g_acAnswerBuf,"I IO1: %s  IO2: %s\r", strHelpIn1, strHelpIn2);
	sprintf(g_acAnswerBuf,"I ");
	if(PortTest & 0x0001)
	{
		strcat(g_acAnswerBuf,"IO1: ");
		strcat(g_acAnswerBuf, strHelpIn1);
		strcat(g_acAnswerBuf,"  ");
	}

	if(PortTest & 0x0002)
	{
		strcat(g_acAnswerBuf,"IO2: ");
		strcat(g_acAnswerBuf, strHelpIn2);
	
	}
	strcat(g_acAnswerBuf,"\r");
	if(TestOn == 2)
	{
		K_Task_Wait(2000);SensorAndPortPowerOnSetting();
	}
	//SET_OEM_IN = g_SensorConfig.SetPort_OEM_IN;
//	SET_UVL_IN = g_SensorConfig.SetPort_UVL_IN;
	return errInfo;
}
unsigned short Cmd_ToolTest(void)
{
	unsigned short error;
	unsigned short EquipmentAdrBit, EquipmentControlBit;
	if(!gDoppelTasteUI) {return ERR_CMD_NOT_IMPLEMENTED;}
	if((error = scanUShort(&EquipmentAdrBit)) != errNone) return error;
	if((error = scanUShort(&EquipmentControlBit)) != errNone) return error;
	/*if(((g_EquipmentConfig.EquipmentConfig & BIT_MuellFoerderer) &  (EquipmentAdrBit & BIT_MuellFoerderer)))
	{
		//if(!MUELL_IN) return ERR_NO_WASTE_BOX;
		error = HardwareEquipmentCtrl(cToolTest, BIT_MuellFoerderer, EquipmentControlBit);
	}

	else
	{error = ERR_VALUE_OUT_OF_RANGE;}
	return error;*/
	return HardwareEquipmentCtrl(cToolTest, BIT_E_MuellFoerderer, EquipmentControlBit);
}
unsigned short Cmd_SetEquipment(unsigned short Time)
{
	unsigned short error;
	unsigned short EquipmentAdrBit, EquipmentControlBit;

	if(!gDoppelTasteUI) return ERR_CMD_NOT_IMPLEMENTED;
	if((error = scanUShort(&EquipmentAdrBit)) != errNone) return error;
	if((error = scanUShort(&EquipmentControlBit)) != errNone) return error;
	if(!((g_EquipmentConfig.EquipmentConfig & BIT_E_MuellFoerderer) &  (EquipmentAdrBit & BIT_E_MuellFoerderer)))
	{return ERR_VALUE_OUT_OF_RANGE;}
	if(Time)
	{
		error = HardwareEquipmentCtrl(cEquipmentTime, EquipmentAdrBit, EquipmentControlBit);
	}
	else
	{
		error = HardwareEquipmentCtrl(cEquipmentCtrl, EquipmentAdrBit, EquipmentControlBit);
	}
	return error;	
}

unsigned short Cmd_SetEquipmentConfig(void)
{
	unsigned short error;
	unsigned short EquipmentAdrBit, EquipmentControlBit;

	if(!gDoppelTasteUI) return ERR_CMD_NOT_IMPLEMENTED;
	if((error = scanUShort(&EquipmentAdrBit)) != errNone) return error;
	if((error = scanUShort(&EquipmentControlBit)) != errNone) return error;
	if((EquipmentAdrBit > BIT_E_Equipment_MAX) || (EquipmentControlBit > BIT_E_Equipment_MAX)) return ERR_WRONG_PARAMETER;
	if(EquipmentAdrBit & BIT_E_MuellFoerderer)
	{
		//if(!gDoppelTasteUI) return ERR_CMD_NOT_IMPLEMENTED;
		if(EquipmentControlBit & BIT_E_MuellFoerderer)
		 //{g_EquipmentConfig.EquipmentConfig |= (EquipmentControlBit & BIT_MuellFoerderer);}
		 {g_EquipmentConfig.EquipmentConfig |= BIT_E_MuellFoerderer;}
		else
		 {g_EquipmentConfig.EquipmentConfig &= ~(unsigned short)BIT_E_MuellFoerderer;}
	}
	if(EquipmentAdrBit & BIT_E_DoorLifter)
	{
		if(EquipmentControlBit & BIT_E_DoorLifter)
		{
		 	g_EquipmentConfig.EquipmentConfig |= BIT_E_DoorLifter;
			DorrLift_U1C0_ASC_vInit();
		}
		else
		 {
		 	g_EquipmentConfig.EquipmentConfig &= ~(unsigned short)BIT_E_DoorLifter;
			Slave_U1C0_ASC_vInit();
		 }
	}

	return FRAMSaveEquipmentConfig(&g_EquipmentConfig);
}

unsigned short Cmd_GetEquipmentConfig(void)
{
	if(!gDoppelTasteUI) return ERR_CMD_NOT_IMPLEMENTED;
	sprintf(g_acAnswerBuf,"I%u\r",g_EquipmentConfig.EquipmentConfig);
	return errInfo;
}
unsigned short Cmd_SetDeviceConfig(void)
{
	unsigned short error;
	unsigned short DevConfigAdrBit, DevConfigControlBit;

	//if(!gDoppelTasteUI) return ERR_CMD_NOT_IMPLEMENTED;
	if((error = scanUShort(&DevConfigAdrBit)) != errNone) return error;
	if((error = scanUShort(&DevConfigControlBit)) != errNone) return error;
	if((DevConfigAdrBit > BIT_D_DeviceConfig_MAX) || (DevConfigControlBit > BIT_D_DeviceConfig_MAX)) return ERR_WRONG_PARAMETER;
	if(DevConfigAdrBit & BIT_D_MuellFoerderer)
	{
		if(DevConfigControlBit & BIT_D_MuellFoerderer)
		 {g_EquipmentConfig.DeviceConfig |= BIT_D_MuellFoerderer;}
		else
		 {g_EquipmentConfig.DeviceConfig &= ~(unsigned short)BIT_D_MuellFoerderer;}
	}
	if(DevConfigAdrBit & BIT_D_DoorLifter)
	{
		if(DevConfigControlBit & BIT_D_DoorLifter)
		 {
		 	g_EquipmentConfig.DeviceConfig |= BIT_D_DoorLifter;
			DorrLift_U1C0_ASC_vInit();
		 }
		else
		 {
		 	g_EquipmentConfig.DeviceConfig &= ~(unsigned short)BIT_D_DoorLifter;
			Slave_U1C0_ASC_vInit();
		 }
	}

	return FRAMSaveEquipmentConfig(&g_EquipmentConfig);
}
unsigned short Cmd_GetDeviceConfig(void)
{
	sprintf(g_acAnswerBuf,"I%u\r",g_EquipmentConfig.DeviceConfig);
	return errInfo;
}

unsigned short Cmd_SetDeviceParameter(void)
{
	unsigned short error;
	unsigned short DevPara;
	unsigned long All, Motion;
	unsigned short usValue;
	udtEstopCounter my_EstopCounter;
	udtEstopWait my_EstopWait;
	double fRelativeWay;
	double fDeltaWay;

	if((error = scanUShort(&DevPara)) != errNone) return error;
	switch(DevPara)
	{
		case 1:	
				if((error = scanULong(&All)) != errNone) return error;
				if((error = scanULong(&Motion)) != errNone) return error;
				ESTOP_VARIABLEN_SETGET_ALLOC
				g_EstopCounter.EstopAll = my_EstopCounter.EstopAll = All;
				g_EstopCounter.EstopByMotion = my_EstopCounter.EstopByMotion = Motion;
				ESTOP_VARIABLEN_SETGET_RELEASE
				error = FRAMSaveEstopCounter(&my_EstopCounter);
				break;
		case 2:	
				if((error = scanUShort(&usValue)) != errNone) return error;
				ESTOP_VARIABLEN_SETGET_ALLOC
				g_EstopWait.EstopWait = my_EstopWait.EstopWait = usValue;
				ESTOP_VARIABLEN_SETGET_RELEASE
				error = FRAMSaveEstopWait(&my_EstopWait);
				break;
		case 3:
				//if((error = scanUShort(&Mask)) != errNone) return error;
				if((error = scanDouble(&fRelativeWay)) != errNone) return error;
				if((error = scanDouble(&fDeltaWay)) != errNone) return error;
				if(fRelativeWay < (double)0.0)
				{
					fDeltaWay = fabs(fDeltaWay) * (double)-1.0;
				}
				else
				{
					fDeltaWay = fabs(fDeltaWay);
				}
				g_SearchAdapterRelativeDeltaWay.RelativeWay = fRelativeWay;
				g_SearchAdapterRelativeDeltaWay.DeltaWay = fDeltaWay;


				error = FRAMSaveSearchAdapterRelativeDeltaWay(&g_SearchAdapterRelativeDeltaWay);
				break;
		default:	error = ERR_WRONG_PARAMETER;
	}
	return error;
}

unsigned short Cmd_GetDeviceParameter(void)
{
	unsigned short error;
	unsigned short DevPara;
	unsigned long ulAll, ulMotion;
	unsigned short usValue;

	if((error = scanUShort(&DevPara)) != errNone) return error;
	switch(DevPara)
	{
		case 1:	
				ESTOP_VARIABLEN_SETGET_ALLOC
				ulAll = g_EstopCounter.EstopAll;
				ulMotion = g_EstopCounter.EstopByMotion;
				ESTOP_VARIABLEN_SETGET_RELEASE
				sprintf(g_acAnswerBuf,"I%lu %lu\r", ulAll, ulMotion);
				error = errInfo;
				break;
		case 2:	
				ESTOP_VARIABLEN_SETGET_ALLOC
				usValue = g_EstopWait.EstopWait;
				ESTOP_VARIABLEN_SETGET_RELEASE
				sprintf(g_acAnswerBuf,"I%u\r",usValue);
				error = errInfo;
				break;
		case 3:
				sprintf(g_acAnswerBuf,"I%3.2f %3.2f\r", g_SearchAdapterRelativeDeltaWay.RelativeWay, g_SearchAdapterRelativeDeltaWay.DeltaWay);
				error = errInfo;
				break;
		default:	error = ERR_WRONG_PARAMETER;
	}
	return error;
}
unsigned short Cmd_SetStallExeptionParameter(void)
{
	unsigned short error;
	unsigned short i;
	unsigned short ID;
	unsigned short Mask;
	short idx;
	unsigned short value;

	if((error = scanUShort(&Mask)) != errNone) return error;
	if(!ConfirmMask(Mask)) return ERR_WRONG_PARAMETER;
	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if((ID = GetAxisIDFromMask(Mask, g_FelixAxes[i].AxisBit)) != 0)
		{
			idx = GetAxisIndex(ID);
			if(idx < 0)
			{
				return ERR_UNKNOWN_DRV;
			}
			if((error = scanUShort(&value)) != errNone) return error;
			g_ConfigStallExeption[idx].Enable = value;
			if((error = scanUShort(&value)) != errNone) return error;
			g_ConfigStallExeption[idx].Up_DirA_Counter = value;
			if((error = scanUShort(&value)) != errNone) return error;
			g_ConfigStallExeption[idx].Down_DirB_Counter = value;
			if((error = scanUShort(&value)) != errNone) return error;
			g_ConfigStallExeption[idx].WaitTime = value;
		}
	}
	return FRAMSaveAxisConfigStallExeption(g_ConfigStallExeption);
}

unsigned short Cmd_GetStallExeptionParameter(void)
{
	unsigned short error;
	unsigned short i;
	char strHelp[100];
	unsigned short ID;
	unsigned short Mask;
	short idx;

	if((error = scanUShort(&Mask)) != errNone) return error;
	if(!ConfirmMask(Mask)) return ERR_WRONG_PARAMETER;

	sprintf(g_acAnswerBuf,"I"); strHelp[0]=0;

	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if((ID = GetAxisIDFromMask(Mask, g_FelixAxes[i].AxisBit)) != 0)
		{
			idx = GetAxisIndex(ID);
			if(idx < 0)
			{
				return ERR_UNKNOWN_DRV;
			}
			//sprintf(strHelp,"%3.2f %3.2f ", pIMSFixedPara[idx]->fMinWay, pIMSFixedPara[idx]->fMaxWay);
			sprintf(strHelp,"%u %u %u %u ", g_ConfigStallExeption[idx].Enable, g_ConfigStallExeption[idx].Up_DirA_Counter, g_ConfigStallExeption[idx].Down_DirB_Counter, g_ConfigStallExeption[idx].WaitTime);
			strcat(g_acAnswerBuf,strHelp);
		}
	}
	g_acAnswerBuf[strlen(g_acAnswerBuf)-1]=0; // Letztes Leerzeichen ausblenden
  	strcat(g_acAnswerBuf,"\r");
	return errInfo;
}
unsigned short Cmd_SetStallExeptionCounter(void)
{
	unsigned short error;
	unsigned short i;
	unsigned short ID;
	unsigned short Mask;
	short idx;
	unsigned long value;

	if((error = scanUShort(&Mask)) != errNone) return error;
	if(!ConfirmMask(Mask)) return ERR_WRONG_PARAMETER;
	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if((ID = GetAxisIDFromMask(Mask, g_FelixAxes[i].AxisBit)) != 0)
		{
			idx = GetAxisIndex(ID);
			if(idx < 0)
			{
				return ERR_UNKNOWN_DRV;
			}
			if((error = scanULong(&value)) != errNone) return error;
			g_CounterStallExeption[idx].ExeptionLifeTimeCounter = value;
		}
	}
	return FRAMSaveAxisCounterStallExeption(g_CounterStallExeption);

}
unsigned short Cmd_GetStallExeptionCounter(void)
{
	unsigned short error;
	unsigned short i;
	char strHelp[100];
	unsigned short ID;
	unsigned short Mask;
	short idx;

	if((error = scanUShort(&Mask)) != errNone) return error;
	if(!ConfirmMask(Mask)) return ERR_WRONG_PARAMETER;

	sprintf(g_acAnswerBuf,"I"); strHelp[0]=0;

	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if((ID = GetAxisIDFromMask(Mask, g_FelixAxes[i].AxisBit)) != 0)
		{
			idx = GetAxisIndex(ID);
			if(idx < 0)
			{
				return ERR_UNKNOWN_DRV;
			}
			//sprintf(strHelp,"%3.2f %3.2f ", pIMSFixedPara[idx]->fMinWay, pIMSFixedPara[idx]->fMaxWay);
			sprintf(strHelp,"%lu ", g_CounterStallExeption[idx].ExeptionLifeTimeCounter);
			strcat(g_acAnswerBuf,strHelp);
		}
	}
	g_acAnswerBuf[strlen(g_acAnswerBuf)-1]=0; // Letztes Leerzeichen ausblenden
  	strcat(g_acAnswerBuf,"\r");
	return errInfo;
}


unsigned short Cmd_SetApplicationSpeed(void)
{
	unsigned short error = errNone;
	unsigned short AppSpdID;
	unsigned short ID;
	unsigned short Mask;
	unsigned short i;
	short idx;
	float Spd;
	float Acc;

	if((error = scanUShort(&AppSpdID)) != errNone) return error;
	if(!CheckUShort(MIN_APPLICATION_SPD+1, MAX_APPLICATION_SPD, AppSpdID)){return ERR_WRONG_PARAMETER;}
	if((error = scanUShort(&Mask)) != errNone) return error;
	if(!ConfirmMask(Mask)) return ERR_WRONG_PARAMETER;
	AppSpdID--;
	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if((ID = GetAxisIDFromMask(Mask, g_FelixAxes[i].AxisBit)) != 0)
		{
			idx = GetAxisIndex(ID);
			if(idx < 0)
			{
				return ERR_UNKNOWN_DRV;
			}
			if((error = scanFloat(&Spd)) != errNone) return error;
			if((error = scanFloat(&Acc)) != errNone) return error;


			if(!CheckFloat(pIMSFixedPara[idx]-> fMinSpeed, pIMSFixedPara[idx]-> fMaxSpeed, Spd)){return ERR_WRONG_PARAMETER;}
			if(!CheckFloat(pIMSFixedPara[idx]-> fMinAcceleration, pIMSFixedPara[idx]-> fMaxAcceleration, Acc)){return ERR_WRONG_PARAMETER;}
			g_ApplicationSpd[AppSpdID].AppAxisSpd[idx].fSpd = Spd;
			g_ApplicationSpd[AppSpdID].AppAxisSpd[idx].fAcc = Acc;
			
		}
	}
	error = FRAMSaveApplicationSpdAxis(AppSpdID, g_ApplicationSpd[AppSpdID].AppAxisSpd);
	return error;
}

unsigned short Cmd_GetApplicationSpeed(void)
{
	unsigned short error;
	unsigned short AppSpdID;
	unsigned short ID;
	unsigned short Mask;
	unsigned short i;
	short idx;
	char strHelp[30];

	if((error = scanUShort(&AppSpdID)) != errNone) return error;
	if(!CheckUShort(MIN_APPLICATION_SPD+1, MAX_APPLICATION_SPD, AppSpdID)){return ERR_WRONG_PARAMETER;}
	if((error = scanUShort(&Mask)) != errNone) return error;
	if(!ConfirmMask(Mask)) return ERR_WRONG_PARAMETER;
	AppSpdID--;

			if((error = FRAMReadApplicationSpdAxis(AppSpdID, g_ApplicationSpd[AppSpdID].AppAxisSpd)) != errNone)
			{
				return error;
			}


	sprintf(g_acAnswerBuf,"I"); strHelp[0]=0;
	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if((ID = GetAxisIDFromMask(Mask, g_FelixAxes[i].AxisBit)) != 0)
		{
			idx = GetAxisIndex(ID);
			if(idx < 0)
			{
				return ERR_UNKNOWN_DRV;
			}
			sprintf(strHelp,"%3.1f %3.1f ", g_ApplicationSpd[AppSpdID].AppAxisSpd[idx].fSpd, g_ApplicationSpd[AppSpdID].AppAxisSpd[idx].fAcc);
			strcat(g_acAnswerBuf,strHelp);
		}
	}

	g_acAnswerBuf[strlen(g_acAnswerBuf)-1]=0; // Letztes Leerzeichen ausblenden
  	strcat(g_acAnswerBuf,"\r");
	return errInfo;
}

unsigned short Cmd_DowngradeHelper(void)
{
	unsigned short error;
	unsigned short Version;

	if((error = scanUShort(&Version)) != errNone) return error;
	FRAMSetStructVersion(1, Version);
	return error;
}

// Test der Schnittstellen COM OUT
unsigned short CmdComCheck(FILE *InHandle, FILE *OutHandle)
{
//	const char strTestComOut[]= {"COM OUT"};
	const char strTestComOut[]= {"COM5 OUT TxD5 RxD5"};
//	const char strTestComBarc[]= {"COM Barcode"};
	const char strTestComBarc[]= {"COM4 Barcode TxD4 RxD4"};
//	const char strTestComRTS485[]= {"COM RS485"};
	const char strTestComRTS485[]= {"COM1 RS485 Tx1 Rx1"};
	const char strTestComLAN[]= {"COM3 LAN TxD3 RxD3"};

	//const char strCom[]= {"Com"};
	const char strError[]= {"ERROR"};
	const char strOK[]= {"OK"};
	unsigned short i, j, err, ComTest, ComLoop;
	int ch;
	unsigned short error = errInfo;
	unsigned short events;
	unsigned short ComEvents;

	if((err = scanUShort(&ComTest)) == errNone)
	{
		switch(ComTest)
		{
			case 1: //COM1_RS485_U0C1_OUT
					ComTest = 0x0004;
					break;
			case 3: //COM3_LAN_U2C0_OUT
					ComTest = 0x0008;
					break;
			case 4: //COM4_BARC_U2C1_OUT
					ComTest = 0x0002;
					break;
			case 5: //COM5_SLAVE_U1C0_OUT
					ComTest = 0x0001;
					break;
			default: ComTest = 0x0007;
		}
	}
	else
	{
		ComTest = 0x0007;
	}
	if((err = scanUShort(&ComLoop)) != errNone){ComLoop = 0;}
do{
	sprintf(g_acAnswerBuf,"I ");
	// Test COM OUT Handshake-Signale, keine Handshake Leitungen vorhanden

	// Test COM OUT Schnittstelle
	if(ComTest & 0x0001)
	{
		err = FALSE;
		UART5_ResetDataBufferAndErrorFlag();
		for(i=0; i<10; i++)
		{
			fprintf(COM5_SLAVE_U1C0_OUT,"%s", strTestComOut);fflush(COM5_SLAVE_U1C0_OUT);
	   		K_Task_Wait(20);
			for(j = 0; j < strlen(strTestComOut); j++)
			{
				ch= UART5_GetChar();
				if(ch == EOF)
				{ err= TRUE; break;}
				else
				{
					if(strTestComOut[j] != ch)
					{ err= TRUE; break;}
				}
			}
		}
		UART5_ResetDataBufferAndErrorFlag();
		strcat(g_acAnswerBuf, strTestComOut);
		strcat(g_acAnswerBuf,": ");
		strcat(g_acAnswerBuf, err? strError: strOK);
		strcat(g_acAnswerBuf,"   ");
		//strcat(g_acAnswerBuf,"%s: %s   ", strTestComOut, err? strError: strOK);

	}
	// Test COM4 Barcode Handshake-Signale, keine Handshake Leitungen vorhanden
	// Test COM4 Barcode Schnittstelle
	if(ComTest & 0x0002)
	{
		err = FALSE;
		UART4_ResetDataBufferAndErrorFlag();
		for(i=0; i<10; i++)
		{
			fprintf(COM4_BARC_U2C1_OUT,"%s", strTestComBarc);fflush(COM4_BARC_U2C1_OUT);
	   		K_Task_Wait(20);
			for(j = 0; j < strlen(strTestComBarc); j++)
			{
				ch= UART4_GetChar();
				if(ch == EOF)
				{ err= TRUE; break;}
				else
				{
					if(strTestComBarc[j] != ch)
					{ err= TRUE; break;}
				}
			}
		}
		UART4_ResetDataBufferAndErrorFlag();
		strcat(g_acAnswerBuf, strTestComBarc);
		strcat(g_acAnswerBuf,": ");
		strcat(g_acAnswerBuf, err? strError: strOK);
		strcat(g_acAnswerBuf,"   ");
	}
	// Test COM1 RS485 Handshake-Signale, keine Handshake Leitungen vorhanden
	// Test COM1 RS485 Schnittstelle
	if(ComTest & 0x0004)
	{
		err = FALSE;
		UART1_ResetDataBufferAndErrorFlag();
		for(i=0; i<10; i++)
		{
			fprintf(COM1_RS485_U0C1_OUT,"%s", strTestComRTS485);fflush(COM1_RS485_U0C1_OUT);
	   		K_Task_Wait(20);
			for(j = 0; j < strlen(strTestComRTS485); j++)
			{
				ch= UART1_GetChar();
				if(ch == EOF)
				{ err= TRUE; break;}
				else
				{
					if(strTestComRTS485[j] != ch)
					{ err= TRUE; break;}
				}
			}
		}
		UART1_ResetDataBufferAndErrorFlag();
		strcat(g_acAnswerBuf, strTestComRTS485);
		strcat(g_acAnswerBuf,": ");
		strcat(g_acAnswerBuf, err? strError: strOK);
//strcat(g_acAnswerBuf,"   ");
	}
	// Test COM3 LAN Interface, RS232 Handshake-Signale, keine Handshake Leitungen vorhanden
	// Test COM3 LAN Interface, RS232 Schnittstelle
	if(ComTest & 0x0008)
	{
		err = FALSE;
		UART3_ResetDataBufferAndErrorFlag();
		for(i=0; i<10; i++)
		{
			fprintf(COM3_LAN_U2C0_OUT,"%s", strTestComLAN);fflush(COM3_LAN_U2C0_OUT);
	   		K_Task_Wait(20);
			for(j = 0; j < strlen(strTestComLAN); j++)
			{
				ch= UART3_GetChar();
				if(ch == EOF)
				{ err= TRUE; break;}
				else
				{
					if(strTestComLAN[j] != ch)
					{ err= TRUE; break;}
				}
			}
		}
		UART3_ResetDataBufferAndErrorFlag();
		strcat(g_acAnswerBuf, strTestComLAN);
		strcat(g_acAnswerBuf,": ");
		strcat(g_acAnswerBuf, err? strError: strOK);
	}

	strcat(g_acAnswerBuf,"\r");
	if(ComLoop)
	{
		PRINT_ALLOC
		fprintf(OutHandle, "%s",g_acAnswerBuf);fflush(OutHandle);		/* Senden der Antwort */
		PRINT_RELEASE
		if(UART_GetEvents(InHandle) & COM_EVENT_USER_BREAK) {error = ERR_PROG_ABORT;break;}
		//-------------------------------------------------------------------------
		// Auf Schnittstellenereignisse reagieren

		events = K_Event_Wait(EVENT_MASTER_COM | EVENT_USER_BREAK, 22, 0 );	// löscht danach die angegebenen Events
		if(events)
		{
			K_Event_Reset(UserMainSlot, EVENT_MASTER_COM);
			error = errNone;
			if(events & EVENT_USER_BREAK)
			{
				K_Event_Reset(UserMainSlot, EVENT_USER_BREAK);
				putc(ACK, OutHandle);fflush(OutHandle);
				error = ERR_PROG_ABORT;
			}
			if(events & EVENT_MASTER_COM)
			{
				ComEvents = UART_GetEvents(InHandle);
				if(ComEvents == COM_EVENT_CR)
				{
					error = ERR_PROG_ABORT;
				}
				if(ComEvents == COM_EVENT_SYNC)
				{
					UART_ResetEvents(InHandle);
					EmptyTheStream(InHandle);
					UART_ResetDataBufferAndErrorFlag(InHandle);
					putc(ACK, OutHandle);fflush(OutHandle);
					error = ImsCmd_Sync();
				}
				if(ComEvents == COM_EVENT_OVERFLOW)
				{
					error = errRxOverflow;
				}
			}
			if(error != errNone) break;
		}
		//-------------------------------------------------------------------------
	}

}while(ComLoop);
	return error;
}

#ifdef _CF_DEBUG_POSERROR
/*unsigned short DebugPositionTesten(unsigned short counter)
{
	unsigned short error = errNone;
	unsigned short i;
	double Value = 0;

	for(i=0; i < MAX_AXES; i++)
	{
		WDO();
		if(DebugData[i].Running)
		{
			if((error = MotorGetPosition( i + 1, &Value)) != errNone) return error;
			if(!CheckDouble(DebugData[i].CmdPos - (double)0.2, DebugData[i].CmdPos + (double)0.2, Value))
			{
				if(g_TestFlag & TF_DBG_CALC_STEPS)
				{
					dbPrint("\rCount:%u ActPos:%f\r", counter, Value);
					DebugDataPrint(0, i);
				}
				error = DEBUG_ERROR1;
			}
			else
			{
				DebugData[i].Running = 0;
			}
		}
	}
	return error;
}*/

unsigned short DebugPositionTesten(unsigned short counter)
{
	unsigned short error = errNone;
	unsigned short i;
	double Value = 0;
	//double fhelp_1, fhelp_2, current_angle;
	//long lhelp;
	unsigned short Kreis;

	for(i=0; i < MAX_AXES; i++)
	{
		WDO();
		if((DebugData[i].Running) &&	g_FelixAxes[i].Enable)
		{
			if((error = MotorGetPosition( i + 1, &Value)) != errNone) return error;

			/*if(g_FelixAxes[i].AxisBit == eDisk_Axis)
			{
				if(fabs(Value) >= (double)1.00000)
				{
					lhelp = labs((long)Value);
					lhelp = lhelp % (long)360;
				}
				if(lhelp != 0)
				{
					fhelp_1 = Value / (double)360.000;
					fhelp_2 = fhelp_1 - (long)fhelp_1; // Normieren
					current_angle = fhelp_2 * (double)360.000;//aktuelle Pos.(Winkel)(kleiner 360 Grad)
					if(current_angle > (double)0.000) ;
					if(current_angle < (double)0.000) current_angle = current_angle + (double)360.000;
				}
				else
				{
					current_angle = (double)0.0;

				}
				Value = current_angle;
			}*/
			if(g_FelixAxes[i].AxisFullRotary == 1)
			{
				Value = ConvertAngle(Value);
			}
			if(!CheckDouble(DebugData[i].CmdPos - (double)0.2, DebugData[i].CmdPos + (double)0.2, Value))
			{
				if(g_FelixAxes[i].AxisFullRotary == 1)
				{
					Kreis = 1;
					if(CheckDouble(DebugData[i].CmdPos - (double)0.2, DebugData[i].CmdPos + (double)0.2, (double)0.0000))
					{
						Kreis = 0;
				
					}
					if(CheckDouble(DebugData[i].CmdPos - (double)0.2, DebugData[i].CmdPos + (double)0.2, (double)360.0000))
					{
						Kreis = 360;
					}
					if((Kreis == 0) || (Kreis == 360))
					{
						//if(g_TestFlag & TF_DBG_CALC_STEPS)
						{
							//dbPrint("\rKreis: %u Count:%u ActPos:%f\r", Kreis, counter, Value);
							//DebugDataPrint(0, i);
						}

						DebugData[i].Running = 0;
					}
					else
					{
						if(g_TestFlag & TF_DBG_CALC_STEPS)
						{
							dbPrint("\rCount:%u ActPos:%f\r", counter, Value);
							DebugDataPrint(0, i);
						}
						error = DEBUG_ERROR1;
					}
				}
				else
				{
					if(g_TestFlag & TF_DBG_CALC_STEPS)
					{
						dbPrint("\rCount:%u ActPos:%f\r", counter, Value);
						DebugDataPrint(0, i);
					}
					error = DEBUG_ERROR1;
				}
			}
			else
			{
				DebugData[i].Running = 0;
			}
		}
	}
	return error;
}

void DebugPosErrReset(void)
{
	unsigned short i;
	for(i=0; i < MAX_AXES; i++)
	{
		WDO();
		DebugData[i].Running = 0;
	}
	DebugAsyncReposCnt = 0;
	DebugCmdMask = 0;

}

unsigned short Cmd_DebugPrint(FILE *OutHandle)
{
	unsigned short Mask;
	unsigned short i;
	if(scanUShort(&Mask) != errNone)
	{
		//Mask = eX_Axis | eFrontCarriage_Axis | eRearCarriage_Axis | eBottomCarriage | eZ_Axis;
		Mask = eAllMask;
	}
	if(!ConfirmMask(Mask)) return ERR_WRONG_PARAMETER;
	for(i=0; i < MAX_AXES; i++)
	{
		WDO();
		if(GetAxisIDFromMask(Mask, g_FelixAxes[i].AxisBit) != 0)
		{
			WDO();
			PRINT_ALLOC
			fprintf(OutHandle, "i:%u Err:%u uv:%f dduv:%f CmdPos:%f\r", i, DebugData[i].Error, pIMSFixedPara[i]->fMotImpProMM, DebugData[i].MotImpProMM, DebugData[i].CmdPos);fflush(OutHandle);
			WDO();
			fprintf(OutHandle, "DD Pos:%f dstep:%f lstep:%ld lStepWay:%ld MotStep:%ld\r", DebugData[i].AbsolutePos, DebugData[i].dSteps, DebugData[i].lSteps, DebugData[i].lStepWay, DebugData[i].lMotStepWay);fflush(OutHandle);
			WDO();
			fprintf(OutHandle, "MM dSteps:%f lstep:%f lStepWay:%f MotStep:%f\r", DebugData[i].mm_dSteps, DebugData[i].mm_lSteps, DebugData[i].mm_lStepWay, DebugData[i].mm_lMotStepWay);fflush(OutHandle);
			WDO();
			fprintf(OutHandle, "DD dd_dstep:%f dd_lstep:%f\r", DebugData[i].db_mm_dSteps, DebugData[i].db_mm_lSteps);fflush(OutHandle);
			PRINT_RELEASE
		}
	}
	return errNone;
}

unsigned short Cmd_DebugReset(void)
{
	DebugPosErrReset();
	return errNone;
}
unsigned short Cmd_DebugClear(void)
{
	unsigned short Mask;

	if(scanUShort(&Mask) != errNone)
	{
		//Mask = eX_Axis | eFrontCarriage_Axis | eRearCarriage_Axis | eBottomCarriage | eZ_Axis;
		Mask = eAllMask;
	}
	if(!ConfirmMask(Mask)) return ERR_WRONG_PARAMETER;
	DebugDataClear(Mask);
	return errNone;
}
#endif

unsigned short g_Cmd_ASync_ResetEstopMove(void)
{
	#ifdef _CF_DEBUG_POSERROR
	DebugPosErrReset();
	#endif
	m_EStopCmdState = ESTOPSTATE_RESET;
	ClearEStopAnswerFromLPK();
	if(!g_isEstopFlag()){g_ClearGenerateEStopError();}
	return g_ASync_ResetEstopMove();

}

unsigned short TestForFirmware(void)
{
	unsigned short error;

	error = m_Async_Hippie_Safety();
	return error;
}

