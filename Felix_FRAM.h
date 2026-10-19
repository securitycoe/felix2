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
#ifndef _FELIXFRAM_H_
#define _FELIXFRAM_H_
//****************************************************************************
// include files
//****************************************************************************
#include "Drv_config.h"
#include "Felix_DoorLifter.h"
//****************************************************************************
// @Macros
//****************************************************************************

//****************************************************************************
// @Defines
//****************************************************************************
//#define FRAM_SIZE 512 // in Bytes, 512 x 8bit
#define FRAM_SIZE 8192 // in Bytes, 8192 x 8bit
#define FRAM_STRUCT_VERSION 8


#define DEVICE_NAME_SIZE 32	// nicht änderbar ohne neue FRAM-Struktur
#define DEVICE_NUMBER_SIZE 32	// nicht änderbar ohne neue FRAM-Struktur

#define CHARMEM_BLOCK_LENGTH 256
#define DATA_BLOCK_COUNT 16
//****************************************************************************
// @Typedefs
//****************************************************************************
/*typedef struct
{ 
  unsigned short adr[ADR_MAX];
  unsigned short CRC;
} DRIVE_ADR;*/

/*typedef struct
{ 
  unsigned short com[6];
  unsigned short CRC;
} COM_ADR;*/

/*typedef struct
{
	float CorrDiff[4]; // Abweichungen der Magnethöhen bezogen auf die Bodebplatte
	unsigned short CRC;
} CORR_HEIGHTS;*/

/*typedef struct
{ 
  int Pos;
  unsigned short CRC;
} CARR_START_POS;*/

/*typedef struct
{ 
  int Flag;
  unsigned short CRC;
} GET_PUT_OPT;*/

typedef struct
{ 
  char DevNo[DEVICE_NUMBER_SIZE];
  unsigned short CRC;
}DEV_NUMBER_FRAM;

typedef struct
{ 
  unsigned short StructVers;
  unsigned short CRC;
}STRUCT_VERSION;


/*typedef struct
{
	float fOffset;
	// Geschwindigkeiten
	float fStartSpeed;			//[mm/s]
	float fEndSpeedNegative; 	// Endspeed negative direction [mm/s]
	float fEndSpeed;		 	// Endspeed positive direction [mm/s]
	float fAccelerationNegative;	// Acceleration negative direction [mm/s*s]
	float fAcceleration;	// Acceleration positive direction [mm/s*s]
}udtNomalSpeed;*/
/*typedef struct
{
	float fHomingSpeed;			// Endspeed homing [mm/s]
	float fAccelerationHoming;	// Acceleration homing [mm/s*s]
}udtHomingSpeed;*/
/*typedef struct
{
	float fSloMoSpeed;			// Endspeed Slowmotionmode [mm/s]
	float fAccelerationSloMo;	// Acceleration Slowmotionmode [mm/s*s]
}udtSloMoSpeed;*/
typedef struct
{
	unsigned short usRunCurrent;	//  run current [percent]
	unsigned short usHoldCurrent;	// hold current [percent]
}udtCurrent;
/*typedef struct
{
	float fPrecisionDeadband;				// Genauigkeit der Positionierung [mm]
	unsigned short usStallsteps;						// zulässige Diff. Steps zu Encoder [encoder_step]
	unsigned short usHoldCurrentWaitTime;	// hold current wait time [millisecounds]
	unsigned short usMotorWobbleWaitTime;	// wobbling wait time [millisecounds]
}udtDrivePara;*/


typedef struct
{ 
  udtOffset Offset[MAX_AXES + AXES_RESERVE];
  unsigned short CRC;
}OFFSET_FRAM;

/*typedef struct
{ 
  udtNomalSpeed NormalSpeed[MAX_AXES + AXES_RESERVE];
  unsigned short CRC;
}NORMAL_SPEED_FRAM;*/

/*typedef struct
{ 
  udtHomingSpeed HomingSpeed[MAX_AXES + AXES_RESERVE];
  unsigned short CRC;
}HOMING_SPEED_FRAM;*/

/*typedef struct
{ 
  udtSloMoSpeed SloMoSpeed[MAX_AXES + AXES_RESERVE];
  unsigned short CRC;
}SLOMO_SPEED_FRAM;*/

typedef struct
{ 
  udtCurrent Current[MAX_AXES + AXES_RESERVE];
  unsigned short CRC;
}CURRENT_FRAM;


typedef struct
{
	char szCharBlock[CHARMEM_BLOCK_LENGTH+1];
	unsigned short CRC;
}udtDataBlock;

typedef struct
{ 
	udtDataBlock Data[DATA_BLOCK_COUNT];
}DATA_FRAM;

typedef struct
{ 
	udtDeviceConfig DeviceConfig;
	unsigned short CRC;
}CONFIG_FRAM;

typedef struct
{ 
  udtMinMaxWay MinMaxWay[MAX_AXES + AXES_RESERVE];
  unsigned short CRC;
}MINMAXWAY_FRAM;

typedef struct
{ 
  udtSensorConfig SensorConfig;
  unsigned short CRC;
}CONFIG_SENSOR;

typedef struct
{ 
  udtPortConfig PortConfig;
  unsigned short CRC;
}CONFIG_PORT;

typedef struct
{ 
  udtEquipmentConfig EquipmentConfig;
  unsigned short CRC;
}CONFIG_EQUIPMENT;

typedef struct
{ 
  udtEstopCounter EstopCounter;
  unsigned short CRC;
}ESTOPCOUNTER_FRAM;

typedef struct
{ 
  udtEstopWait EstopWait;
  unsigned short CRC;
}ESTOPWAIT_FRAM;

typedef struct
{ 
  udtConfigStallExeption ConfigStallExeption[MAX_AXES + AXES_RESERVE];
  unsigned short CRC;
}CONFIGSTALLEXEPTION_FRAM;

typedef struct
{ 
  udtCounterStallExeption CounterStallExeption[MAX_AXES + AXES_RESERVE];
  unsigned short CRC;
}COUNTERSTALLEXEPTION_FRAM;

typedef struct
{ 
  udtRelativeDeltaWay SearchAdapterRelativeDeltaWay;
  unsigned short CRC;
}SEARCHADAPTERRELATIVEDELTAWAY_FRAM;

typedef struct
{ 
  udtApplicationIndexSpd ApplicationSpd[MAX_APPLICATION_SPD];
}APPLICATIONSPD_FRAM;

typedef struct
{
	udtIMSDoorLiftConfig IMSDoorLiftConfig;
	unsigned short CRC;
}IMSDOORLIFTAXIS_FRAM;
/*typedef struct
{ 
  udtDrivePara DrivePara[MAX_AXES + AXES_RESERVE];
  unsigned short CRC;
}DRIVE_PARA_FRAM;*/

/*#define DRV_CONFIG_CNT 10					// mit ausreichend Reserve für zukünftige Erweiterungen
struct DRV_CONFIG 	{
 						unsigned char index;			// Index des Antriebes (siehe StacComm.h)
 						unsigned char adr;				// Adresse des Antriebes
						unsigned char cfg;				// Antrieb aktiviert
						float offset;			// Offset zum Nullpunkt
					};
struct DRV_CONFIG DrvConfig[DRV_CONFIG_CNT];
typedef struct
{ 
  struct DRV_CONFIG DrvCfg[DRV_CONFIG_CNT];
  unsigned short CRC;
} DRIVE_CONFIG_FRAM;
*/
struct FRAM	
{
	STRUCT_VERSION StructVersionNo;
	DEV_NUMBER_FRAM DevNumber;			// Gerätenummer	in Srukturversion 1
	DATA_FRAM DataFRAM;
	CONFIG_FRAM DeviceConfigFRAM;
	OFFSET_FRAM OffsetFRAM;
	MINMAXWAY_FRAM MinMaxWayFRAM;
	CONFIG_SENSOR SensorConfigFRAM;
	CONFIG_PORT PortConfigFRAM;
	CONFIG_EQUIPMENT EquipmentConfigFRAM;
	ESTOPCOUNTER_FRAM EstopCounterFRAM;
	ESTOPWAIT_FRAM EstopWaitFRAM;
	CONFIGSTALLEXEPTION_FRAM ConfigStallExeptionFRAM;
	COUNTERSTALLEXEPTION_FRAM CounterStallExeptionFRAM;
	SEARCHADAPTERRELATIVEDELTAWAY_FRAM SearchAdapterRelativeDeltaWayFRAM;
	APPLICATIONSPD_FRAM ApplicationSpdFRAM;
	IMSDOORLIFTAXIS_FRAM IMSDoorLiftAxisFRAM;
//	CURRENT_FRAM CurrentRAM;

   //DRIVE_ADR DriveADR;
//	DRIVE_CONFIG_FRAM DrvConfigFRAM;

   //COM_ADR COMAdr;
   //CORR_HEIGHTS DevHeightCorr;
   //CARR_START_POS CarStartPosition;
   //GET_PUT_OPT EnableGetPutOptimize;
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
extern unsigned short FRAMInit(void);
extern unsigned short FRAMDump(unsigned short StartAdress, unsigned short EndAdress, FILE *OutHandle);
extern unsigned short FRAMReadStructVersion(unsigned short *sptr);
extern unsigned short FRAMReadDevNumber(char *sptr);
extern unsigned short FRAMSetDevNumber(char *sptr);
extern void FRAM_GetMemoryStatus(unsigned short *total_m, unsigned short *used_m, unsigned short *free_m);
extern void FRAM_GetUserDataCapacity(unsigned short *datablock_count_m, unsigned short *datablock_size_m);
extern unsigned short FRAMSaveUserData(const unsigned short DataBlock, const char *ptrChar);
extern unsigned short FRAMReadUserData(const unsigned short DataBlock, char *ptrChar);
extern unsigned short FRAMEraseUserData(const unsigned short DataBlock);
extern unsigned short FRAMSaveDeviceConfig(const udtDeviceConfig *Config);
extern unsigned short FRAMReadDeviceConfig(udtDeviceConfig *Config);
extern unsigned short FRAMSaveSensorConfig(const udtSensorConfig *Config);
extern unsigned short FRAMReadSensorConfig(udtSensorConfig *Config);
extern unsigned short FRAMSavePortConfig(const udtPortConfig *Config);
extern unsigned short FRAMReadPortConfig(udtPortConfig *Config);
extern unsigned short FRAMSaveEquipmentConfig(const udtEquipmentConfig *Config);
extern unsigned short FRAMReadEquipmentConfig(udtEquipmentConfig *Config);
extern unsigned short FRAMSaveEstopCounter(const udtEstopCounter *Config);
extern unsigned short FRAMReadEstopCounter(udtEstopCounter *Config);
extern unsigned short FRAMSaveEstopWait(const udtEstopWait *Config);
extern unsigned short FRAMReadEstopWait(udtEstopWait *Config);
extern unsigned short FRAMSaveAxisConfigStallExeption(const udtConfigStallExeption *CnfgStallExep);
extern unsigned short FRAMReadAxisConfigStallExeption(udtConfigStallExeption *CnfgStallExep);
extern unsigned short FRAMSaveAxisCounterStallExeption(const udtCounterStallExeption *CntStallExep);
extern unsigned short FRAMReadAxisCounterStallExeption(udtCounterStallExeption *CntStallExep);
extern unsigned short FRAMSaveSearchAdapterRelativeDeltaWay(const udtRelativeDeltaWay *Config);
extern unsigned short FRAMReadSearchAdapterRelativeDeltaWay(udtRelativeDeltaWay *Config);

void FRAMSetStructVersion(unsigned short DowngradeHelper, unsigned short Version);

unsigned short FRAMSaveAxisOffset(const udtOffset *offset);
unsigned short FRAMReadAxisOffset(udtOffset *offset);
unsigned short FRAMSaveAxisMinMaxWay(const udtMinMaxWay *minmax);
unsigned short FRAMReadAxisMinMaxWay(udtMinMaxWay *minmax);

unsigned short FRAMSaveApplicationSpdAxis(const unsigned short Index, const udtApplicationAxisSpd *AppAxisSpd);
unsigned short FRAMReadApplicationSpdAxis(const unsigned short Index, udtApplicationAxisSpd *AppAxisSpd);

unsigned short FRAMSaveDoorLiftConfig(const udtIMSDoorLiftConfig *Config);
unsigned short FRAMReadDoorLiftConfig(udtIMSDoorLiftConfig *Config);

//****************************************************************************
// @Prototypes Of Local Functions
//****************************************************************************
void FRAMClearAll(void);


//unsigned short FRAMSaveAxisOffsetV2(const udtOffset *offset);
//unsigned short FRAMReadAxisOffsetV2(udtOffset *offset);



#endif

