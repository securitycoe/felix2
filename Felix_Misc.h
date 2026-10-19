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
#ifndef _FELIXMISC_H_
#define _FELIXMISC_H_
//****************************************************************************
// include files
//****************************************************************************

//****************************************************************************
// @Macros
//****************************************************************************

//****************************************************************************
// @Defines
//****************************************************************************
#define TF_OFF			0x0000
#define TF_GENERAL		0x0001
#define TF_DBG_INT_COM	0x0002
#define TF_DBG_EVENT 	0x0004
#define TF_DBG_FRAM 	0x0008
#define TF_DBG_ASYNC	0x0010
#define TF_DBG_HW		0x0020
#define TF_DBG_HW_EVENT 0x0040
#define TF_DBG_BREAKE	0x0080
#define TF_DBG_CTRLCMD	0x0100
#define TF_DBG_485		0x0200
#define TF_DBG_485_ERR	0x0400
#define TF_DBG_MOTTSK	0x0800
#define TF_DBG_ESTOP_CMD	0x1000
#define TF_DBG_CMD		0x2000
#define TF_DBG_CALC_STEPS 0x4000
#define TF_DBG_FETRESET 0x8000

/*#define DevTestOFF	0x0000
#define DevTestXZ	0x0001
#define DevTestZ	0x0002
#define DevTestCAR	0x0004
#define DevTestALL	0x0008
*/
//****************************************************************************
// @Typedefs
//****************************************************************************


//****************************************************************************
// @Imported Global Variables
//****************************************************************************

//****************************************************************************
// @Global Variables
//****************************************************************************
extern unsigned short g_TestFlag;
//****************************************************************************
// @External Prototypes
//****************************************************************************

//****************************************************************************
// Prototypen definitions
//****************************************************************************
extern void dbPrint( const char *format, ...);
extern void dbPrintNotAlloc(const short ID, const char *format, ...);
//extern void dbPrintHost( const char *format, ...);
extern unsigned short CheckChar(char min,char max,char ist);
extern unsigned short CheckUChar(unsigned char min,unsigned char max,unsigned char ist);
extern unsigned short CheckShort(short min,short max,short ist);
extern unsigned short CheckUShort(unsigned short min,unsigned short max,unsigned short ist);
extern unsigned short CheckFloat(float min,float max,float ist);
extern unsigned short CheckDouble(double min,double max,double ist);
extern unsigned short CheckULong(unsigned long min,unsigned long max,unsigned long ist);
extern unsigned short CheckLong(long min,long max,long ist);


extern void SetNRTL_LP(void);
extern unsigned short isNRTL_LP(void);

extern void SetHostMode(void);
extern void ClearHostMode(void);
extern unsigned short isHostMode(void);
extern void SetDeviceInitError(void);
extern void ClearDeviceInitError(void);
extern unsigned short isDeviceInitError(void);
extern void SetFreeHandZMoveMode(void);
extern void ClearFreeHandZMoveMode(void);
extern unsigned short isFreeHandZMoveMode(void);

extern void g_SetEstopFlag(void);
extern void g_ClearEstopFlag(void);
extern unsigned short g_isEstopFlag(void);

extern void g_SetGenerateEStopError(void);
extern void g_ClearGenerateEStopError(void);
extern unsigned short g_isGenerateEStopError(void);



extern void SetMode1536(void);
extern void ClearMode1536(void);
extern unsigned short isMode1536(void);
//#ifdef _CF_NRTL
//extern void SetErrorSafetyMonitoring(void);
//extern unsigned short isErrorSafetyMonitoring(void);
extern void SetFlagFwEStopLock(void);
extern void ClearFlagFwEStopLock(void);
extern unsigned short isFlagFwEStopLock(void);
//#endif

extern void SetEStopAnswerFromLPK(void);
extern void ClearEStopAnswerFromLPK(void);
extern unsigned short isEStopAnswerFromLPK(void);

void SetLPKConnect(void);
void ClearLPKConnect(void);
unsigned short isLPKConnect(void);

/*extern void SetBreakCheckMOSFET(void);
extern void ClearBreakCheckMOSFET(void);
extern unsigned short isSetBreakCheckMOSFET(void);*/

extern long RoundToLong(const double d);
//****************************************************************************
// @Prototypes Of Local Functions
//****************************************************************************

#endif
