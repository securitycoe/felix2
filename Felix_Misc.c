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
#include <limits.h>
#include "DeviceConfig.h"
#include "Drv_config.h"
#include "30-5015_Felix/XE167F/main.h"
#include "Cxconfig.h"
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
extern int _doprint( FILE *fp, const char *fmt, va_list ap );
//****************************************************************************
// external data
//****************************************************************************

//****************************************************************************
// public data
//****************************************************************************
unsigned short g_TestFlag = 0;
//****************************************************************************
// private data
//****************************************************************************
//unsigned short m_DeviceTest = DevTestOFF;
//unsigned short m_EStopHalt = 0;
//unsigned short m_EStopINTR = 0;
unsigned short g_Hostmode = FALSE;
unsigned short g_NRTL_LP = 0;
unsigned short g_DeviceInitError;
unsigned short m_FreeHandZMoveMode;
unsigned short g_Mode1536 = FALSE;
//#ifdef _CF_NRTL
//unsigned short g_ErrorSafetyMonitoring = FALSE;
unsigned short g_FlagFwEStopLock = 0;
//#endif

unsigned short g_EstopFlag = FALSE;
unsigned short g_GenerateEStopErrorFlag = FALSE;
unsigned short g_EStopAnswerFromLPK = FALSE;
unsigned short g_LPKConnect = FALSE;

unsigned short g_BreakCheckMOSFET = FALSE;

#ifdef _CF_485_HOST_OUTPUT_DEBUG_PRINT
short g_Debug_ID = DOORLIFT_ID;
#endif
#ifdef _CF_485_SLAVE_OUTPUT_DEBUG_PRINT
short g_Debug_ID = FELIX_BASE_ID;
#endif
//****************************************************************************
// private functions
//****************************************************************************

//****************************************************************************
// public functions
//****************************************************************************
void dbPrint( const char *format, ...)
{
	va_list ap;
	char dbBuffer[128];
	char prBuffer[128];
	struct _iobuf	_strbuf;
	unsigned short i;

	PRINT_ALLOC
	for(i =0; i<128;i++)
	{
		dbBuffer[i]= '\0';
	}
	va_start( ap, format );
	_strbuf._flag = _IOWR|_IOSTRG;
	_strbuf._base = dbBuffer;
	_strbuf._ptr  = dbBuffer;
	_strbuf._bufsiz= sizeof(dbBuffer);
	_strbuf._cnt  = INT_MAX;
	_doprint( & _strbuf, format, ap );
	va_end( ap );
	strcpy(prBuffer, dbBuffer);
	if(g_Debug_ID == DOORLIFT_ID)
	{
		fprintf(stdout, prBuffer);fflush(stdout);
	}
	else
	{
		fprintf(COM5_SLAVE_U1C0_OUT, prBuffer);fflush(COM5_SLAVE_U1C0_OUT);
	}
	PRINT_RELEASE
}

void dbPrintNotAlloc(const short ID, const char *format, ...)
{
	va_list ap;
	char dbiBuffer[128];
	char priBuffer[128];
	struct _iobuf	_strbuf;
	unsigned short i;

	for(i =0; i<128;i++)
	{
		dbiBuffer[i]= '\0';
	}
	va_start( ap, format );
	_strbuf._flag = _IOWR|_IOSTRG;
	_strbuf._base = dbiBuffer;
	_strbuf._ptr  = dbiBuffer;
	_strbuf._bufsiz= sizeof(dbiBuffer);
	_strbuf._cnt  = INT_MAX;
	_doprint( & _strbuf, format, ap );
	va_end( ap );
	strcpy(priBuffer, dbiBuffer);
	if(ID == DOORLIFT_ID)
	{
		fprintf(stdout, priBuffer);fflush(stdout);
	}
	else
	{
		fprintf(COM5_SLAVE_U1C0_OUT, priBuffer);fflush(COM5_SLAVE_U1C0_OUT);
	}
}
/*void dbPrintHost( const char *format, ...)
{
	va_list ap;
	char dbBuffer[128];
	char prBuffer[128];
	struct _iobuf	_strbuf;
	unsigned short i;

		PRINT_ALLOC
		for(i =0; i<128;i++)
		{
			dbBuffer[i]= '\0';
		}
		va_start( ap, format );
		_strbuf._flag = _IOWR|_IOSTRG;
		_strbuf._base = dbBuffer;
		_strbuf._ptr  = dbBuffer;
		_strbuf._bufsiz= sizeof(dbBuffer);
		_strbuf._cnt  = INT_MAX;
		_doprint( & _strbuf, format, ap );
		va_end( ap );
		strcpy(prBuffer, dbBuffer);
//		fprintf(stdout, prBuffer);fflush(stdout);
		fprintf(COM5_SLAVE_U1C0_OUT, prBuffer);fflush(COM5_SLAVE_U1C0_OUT);
		PRINT_RELEASE
}
*/
/********************************************************************
 *  FUNCTION:    CheckChar
 *
 *  PARAMETERS:  char min
 *                   minimal erlaubter Wert
 *               char max
 *                   maximal erlaubter Wert
 *               char ist
 *                   Istwert
 *
 *  DESCRIPTION: Prüfen ob der Istwert im erlaubten Bereich
 *
 *  RETURNS:     TRUE  = i.O.
 *               FALSE = Istwert nicht im Bereich
 ********************************************************************/
unsigned short CheckChar(char min,char max,char ist)
{
	if((ist < min) || (ist > max)) return FALSE;
	return TRUE;
}

/********************************************************************
 *  FUNCTION:    CheckUChar
 *
 *  PARAMETERS:  char unsigned min
 *                   minimal erlaubter Wert
 *               char unsigned max
 *                   maximal erlaubter Wert
 *               char unsigned ist
 *                   Istwert
 *
 *  DESCRIPTION: Prüfen ob der Istwert im erlaubten Bereich
 *
 *  RETURNS:     TRUE  = i.O.
 *               FALSE = Istwert nicht im Bereich
 ********************************************************************/
unsigned short CheckUChar(unsigned char min,unsigned char max,unsigned char ist)
{
	if((ist < min) || (ist > max)) return FALSE;
	return TRUE;
}


/********************************************************************
 *  FUNCTION:    CheckInt
 *
 *  PARAMETERS:  int min
 *                   minimal erlaubter Wert
 *               int max
 *                   maximal erlaubter Wert
 *               int ist
 *                   Istwert
 *

 *  DESCRIPTION: Prüfen ob der Istwert im erlaubten Bereich
 *
 *  RETURNS:     TRUE  = i.O.
 *               FALSE = Istwert nicht im Bereich
 ********************************************************************/
unsigned short CheckShort(short min,short max,short ist)
{
	if((ist < min) || (ist > max)) return FALSE;
	return TRUE;
}


/********************************************************************
 *  FUNCTION:    CheckUInt
 *
 *  PARAMETERS:  int unsigned min
 *                   minimal erlaubter Wert
 *               int unsigned max
 *                   maximal erlaubter Wert
 *               int unsigned ist
 *                   Istwert
 *
 *  DESCRIPTION: Prüfen ob der Istwert im erlaubten Bereich
 *
 *  RETURNS:     TRUE  = i.O.
 *               FALSE = Istwert nicht im Bereich
 ********************************************************************/
unsigned short CheckUShort(unsigned short min,unsigned short max,unsigned short ist)
{
	if((ist < min) || (ist > max)) return FALSE;
	return TRUE;
}


/********************************************************************
 *  FUNCTION:    CheckFloat
 *
 *  PARAMETERS:  float min
 *                   minimal erlaubter Wert
 *               float max
 *                   maximal erlaubter Wert
 *               float ist
 *                   Istwert
 *
 *  DESCRIPTION: Prüfen ob der Istwert im erlaubten Bereich
 *
 *  RETURNS:     TRUE  = i.O.
 *               FALSE = Istwert nicht im Bereich
 ********************************************************************/
unsigned short CheckFloat(float min,float max,float ist)
{
	if((ist < min) || (ist > max)) return FALSE;
	return TRUE;
}
unsigned short CheckDouble(double min,double max,double ist)
{
	if((ist < min) || (ist > max)) return FALSE;
	return TRUE;
}

/********************************************************************
 *  FUNCTION:    CheckUInt
 *
 *  PARAMETERS:  int unsigned min
 *                   minimal erlaubter Wert
 *               int unsigned max
 *                   maximal erlaubter Wert
 *               int unsigned ist
 *                   Istwert
 *
 *  DESCRIPTION: Prüfen ob der Istwert im erlaubten Bereich
 *
 *  RETURNS:     TRUE  = i.O.
 *               FALSE = Istwert nicht im Bereich
 ********************************************************************/
unsigned short CheckULong(unsigned long min,unsigned long max,unsigned long ist)
{
	if((ist < min) || (ist > max)) return FALSE;
	return TRUE;
}

/********************************************************************
 *  FUNCTION:    CheckUInt
 *
 *  PARAMETERS:  int unsigned min
 *                   minimal erlaubter Wert
 *               int unsigned max
 *                   maximal erlaubter Wert
 *               int unsigned ist
 *                   Istwert
 *
 *  DESCRIPTION: Prüfen ob der Istwert im erlaubten Bereich
 *
 *  RETURNS:     TRUE  = i.O.
 *               FALSE = Istwert nicht im Bereich
 ********************************************************************/
unsigned short CheckLong(long min,long max,long ist)
{
	if((ist < min) || (ist > max)) return FALSE;
	return TRUE;
}


void SetNRTL_LP(void)
{
	VARIABLEN_SETGET_ALLOC
	g_NRTL_LP = 1;
	VARIABLEN_SETGET_RELEASE
}

unsigned short isNRTL_LP(void)
{
	unsigned short help;
	VARIABLEN_SETGET_ALLOC
	help = g_NRTL_LP;
	VARIABLEN_SETGET_RELEASE
	return help;
}


void SetHostMode(void)
{
	VARIABLEN_SETGET_ALLOC
	g_Hostmode = TRUE;
	VARIABLEN_SETGET_RELEASE
}
void ClearHostMode(void)
{
	VARIABLEN_SETGET_ALLOC
	g_Hostmode = FALSE;
	VARIABLEN_SETGET_RELEASE
}
unsigned short isHostMode(void)
{
	unsigned short help;
	VARIABLEN_SETGET_ALLOC
	help = g_Hostmode;
	VARIABLEN_SETGET_RELEASE
	return help;
}

void SetDeviceInitError(void)
{
	VARIABLEN_SETGET_ALLOC
	g_DeviceInitError = TRUE;
	VARIABLEN_SETGET_RELEASE
}
void ClearDeviceInitError(void)
{
	VARIABLEN_SETGET_ALLOC
	g_DeviceInitError = FALSE;
	VARIABLEN_SETGET_RELEASE
}

unsigned short isDeviceInitError(void)
{
	unsigned short help;
	VARIABLEN_SETGET_ALLOC
	help = g_DeviceInitError;
	VARIABLEN_SETGET_RELEASE
	return help;
}

void SetFreeHandZMoveMode(void)
{
	VARIABLEN_SETGET_ALLOC
	m_FreeHandZMoveMode = TRUE;
	VARIABLEN_SETGET_RELEASE
}
void ClearFreeHandZMoveMode(void)
{
	VARIABLEN_SETGET_ALLOC
	m_FreeHandZMoveMode = FALSE;
	VARIABLEN_SETGET_RELEASE
}
unsigned short isFreeHandZMoveMode(void)
{
	unsigned short help;
	VARIABLEN_SETGET_ALLOC
	help = m_FreeHandZMoveMode;
	VARIABLEN_SETGET_RELEASE
	return help;
}

void g_SetEstopFlag(void)
{
	ESTOP_ALLOC
	g_EstopFlag = TRUE;
	ESTOP_RELEASE
}
void g_ClearEstopFlag(void)
{
	ESTOP_ALLOC
	g_EstopFlag = FALSE;
	ESTOP_RELEASE
}
unsigned short g_isEstopFlag(void)
{
	unsigned short help;
	ESTOP_ALLOC
	help = g_EstopFlag;
	ESTOP_RELEASE
	return help;
}

void g_SetGenerateEStopError(void)
{
	ESTOP_ALLOC
	g_GenerateEStopErrorFlag = TRUE;
	ESTOP_RELEASE
}
void g_ClearGenerateEStopError(void)
{
	ESTOP_ALLOC
	g_GenerateEStopErrorFlag = FALSE;
	ESTOP_RELEASE
}
unsigned short g_isGenerateEStopError(void)
{
	unsigned short help;
	ESTOP_ALLOC
	help = g_GenerateEStopErrorFlag;
	ESTOP_RELEASE
	return help;
}




void SetMode1536(void)
{
	VARIABLEN_SETGET_ALLOC
	g_Mode1536 = TRUE;
	VARIABLEN_SETGET_RELEASE
}
void ClearMode1536(void)
{
	VARIABLEN_SETGET_ALLOC
	g_Mode1536 = FALSE;
	VARIABLEN_SETGET_RELEASE
}
unsigned short isMode1536(void)
{
	unsigned short help;
	VARIABLEN_SETGET_ALLOC
	help = g_Mode1536;
	VARIABLEN_SETGET_RELEASE
	return help;
}
//#ifdef _CF_NRTL
/*void SetErrorSafetyMonitoring(void)
{
	VARIABLEN_SETGET_ALLOC
	g_ErrorSafetyMonitoring = TRUE;
	VARIABLEN_SETGET_RELEASE
}*/
/*unsigned short isErrorSafetyMonitoring(void)
{
	unsigned short help;
	VARIABLEN_SETGET_ALLOC
	help = g_ErrorSafetyMonitoring;
	VARIABLEN_SETGET_RELEASE
	return help;
}*/

void SetFlagFwEStopLock(void)
{
	//VARIABLEN_SETGET_ALLOC
	g_FlagFwEStopLock = 1;
	//VARIABLEN_SETGET_RELEASE
}
void ClearFlagFwEStopLock(void)
{
	//VARIABLEN_SETGET_ALLOC
	g_FlagFwEStopLock = 0;
	//VARIABLEN_SETGET_RELEASE
}
unsigned short isFlagFwEStopLock(void)
{
	unsigned short help;
	//VARIABLEN_SETGET_ALLOC
	help = g_FlagFwEStopLock;
	//VARIABLEN_SETGET_RELEASE
	return help;
}

//#endif

void SetEStopAnswerFromLPK(void)
{
	VARIABLEN_SETGET_ALLOC
	g_EStopAnswerFromLPK = TRUE;
	VARIABLEN_SETGET_RELEASE
}
void ClearEStopAnswerFromLPK(void)
{
	VARIABLEN_SETGET_ALLOC
	g_EStopAnswerFromLPK = FALSE;
	VARIABLEN_SETGET_RELEASE
}
unsigned short isEStopAnswerFromLPK(void)
{
	unsigned short help;
	VARIABLEN_SETGET_ALLOC
	help = g_EStopAnswerFromLPK;
	VARIABLEN_SETGET_RELEASE
	return help;
}

void SetLPKConnect(void)
{
	VARIABLEN_SETGET_ALLOC
	g_LPKConnect = TRUE;
	VARIABLEN_SETGET_RELEASE
}
void ClearLPKConnect(void)
{
	VARIABLEN_SETGET_ALLOC
	g_LPKConnect = FALSE;
	VARIABLEN_SETGET_RELEASE
}
unsigned short isLPKConnect(void)
{
	unsigned short help;
	VARIABLEN_SETGET_ALLOC
	help = g_LPKConnect;
	VARIABLEN_SETGET_RELEASE
	return help;
}


/*void SetBreakCheckMOSFET(void)
{
	VARIABLEN_SETGET_ALLOC
	g_BreakCheckMOSFET = TRUE;
	VARIABLEN_SETGET_RELEASE
}
void ClearBreakCheckMOSFET(void)
{
	VARIABLEN_SETGET_ALLOC
	g_BreakCheckMOSFET = FALSE;
	VARIABLEN_SETGET_RELEASE
}
unsigned short isSetBreakCheckMOSFET(void)
{
	unsigned short help;
	VARIABLEN_SETGET_ALLOC
	help = g_BreakCheckMOSFET;
	VARIABLEN_SETGET_RELEASE
	return help;
}
*/


long RoundToLong(const double d)
{
	return d<0?d-.5:d+.5;
	//if(d > 0) return (long)(d+0.5); else return (long)(d-0.5);
}

/*
long d2l(double d)
{
  return d<0?d-.5:d+.5;
}
*/

/*double Round(double Zahl, unsigned int Stellen)
{
    Zahl *= pow(10, Stellen);
    if (Zahl >= 0)
        Zahl = floor(Zahl + 0.5)
    else
        Zahl = ceil(Zahl - 0.5);
    Zahl /= pow(10, Stellen);
    return Zahl;
}
// 2. Änderung: Einführen der Potenzgesetze
double Round(double Zahl, int Stellen)
{
    Zahl *= pow( 10, Stellen);
    if (Zahl >= 0)
        Zahl = floor(Zahl + 0.5)
    else
        Zahl = ceil(Zahl - 0.5);
    Zahl *= pow(10, -Stellen);
    return Zahl;
}
// 3. Änderung: Rundung überarbeitet
double Round(double Zahl, int Stellen)
{
    Zahl *= pow( 10, Stellen);
    Zahl = floor(Zahl + 0.5)
    Zahl *= pow(10, -Stellen);
    return Zahl;
}
// 4. Änderung: Zusammenfassen
double Round(double Zahl, int Stellen)
{
    return floor(Zahl * pow( 10, Stellen) + 0.5) * pow(10, -Stellen);
}
// 5. Änderung: Laufzeitverhalten verbessern
double Round(double Zahl, int Stellen)
{
    double v[] = { 1, 10, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7, 1e8 };  // mgl. verlängern
    return floor(Zahl * v[Stellen] + 0.5) / v[Stellen];
}
*/