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
//#include <malloc.h>
#include "DevError.h"
//#include "Shared/Stacker_Lifter_Allgemein/StacComm.h"
//#include "Shared/Stacker_Lifter_Allgemein/XYZComm.h"
//#include "Shared/Stacker_Lifter_Allgemein/StacError.h"
#include "DevCmds.h"
#include "DeviceConfig.h"
#include "30-5015_Felix/XE167F/main.h"
#include "MyScanner.h"
#include "Scan.h"
#include "Cxconfig.h"
#include "Felix_CmdCtrl.h"
#include "Felix_Barcode.h"
#include "Felix_Opticon.h"

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
#if(CF_USED_BARCODE_READER == DEF_OPTICON)
const char OptiReadBarcode[]={0x1b,'Z','\r','\0'};
const char OptiBeepGood[]={0x1b,'B','\r','\0'};
const char OptiGetASCIIprintable[]={0x1b,'Z','A','\r','\0'};
const char OptiGetSoftwareVersion[]={0x1b,'Z','1','\r','\0'};
unsigned char StrOpticon[] = "Opticon";
#endif

//****************************************************************************
// private functions
//****************************************************************************

//****************************************************************************
// public functions
//****************************************************************************
#if(CF_USED_BARCODE_READER == DEF_OPTICON)
unsigned short OpticonReadBarcode(char *sPtr)
{
	unsigned short error, a;
for(a = 0; a < 3; a++)
{
	BarcodeReset();
	PRINT_ALLOC
	fprintf(COM4_BARC_U2C1_OUT,"%s", OptiReadBarcode);fflush(COM4_BARC_U2C1_OUT);
	PRINT_RELEASE
	if((error = AnswerFromBarcode(BCBuf, OPTICON_BARCODE_TIME_OUT)) == errNone)
	{
		if(BCBuf[0] == BC_INFO_CHAR)
		{
		
	  		strcpy(sPtr, &BCBuf[1]);
			return errInfo;
		}
		else if(BCBuf[0] == BC_ERROR_CHAR)
		{
			error = (unsigned short)atoi(&BCBuf[1]);
		}
		else
		{
			error = ERR_BARC_COMMUNICATION;
		}
		return error;
	}
	if(error != errRxOverflow)
	{
		return error;
	}
}
return ERR_BARC_COMMUNICATION;

}
#endif
