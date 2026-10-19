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
#include "Felix_BCL8.h"

//****************************************************************************
// @Macros
//****************************************************************************

//****************************************************************************
// local definitions
//****************************************************************************
#define BCL8_RESET_TIME_OUT 8000//2000  /* 10s, Basis 5ms*/

//****************************************************************************
// default settings
//****************************************************************************

//****************************************************************************
// external functions
//****************************************************************************

//****************************************************************************
// external data
//****************************************************************************
extern unsigned char UserMainSlot;
//****************************************************************************
// public data
//****************************************************************************

//****************************************************************************
// private data
//****************************************************************************
#if(CF_USED_BARCODE_READER == DEF_BCL8)
unsigned char ucBarcAnswerFlag;	/* ist bei Armin barc_flag */

const char chSTX = '\002';
//const unsigned char aszBCL8_Reset[]={'H','\r','\0'};
const char aszBCL8_STX_Reset[]="\002H\r\n";
//const unsigned char aszBCL8_Firmware[]={'V','\r','\0'};
const char aszBCL8_STX_Firmware[]="\002V\r\n";
//const unsigned char aszBCL8_DecoderOn[]={'+','\r','\0'};
const char aszBCL8_STX_DecoderOn[]="\002+\r\n";
//const unsigned char aszBCL8_DecoderOff[]={'-','\r','\0'};
const char aszBCL8_STX_DecoderOff[]="\002-\r\n";
//const unsigned char aszBCL8_JustageModusOn[]={'J','P','+','\r','\0'};
const char aszBCL8_STX_JustageModusOn[]="\002JP+\r\n";
//const unsigned char aszBCL8_JustageModusOff[]={'J','P','-','\r','\0'};
const char aszBCL8_STX_JustageModusOff[]="\002JP-\r\n";
#endif
unsigned short noReadCounter = 0;
//****************************************************************************
// private functions
//****************************************************************************

//****************************************************************************
// public functions
//****************************************************************************
#if(CF_USED_BARCODE_READER == DEF_BCL8)
unsigned short BCL8_ReadBarcode(char *sPtr)
{
	unsigned short error, a, error2;
for(a = 0; a < 3; a++)
{
	BarcodeReset();
	K_Event_Reset(UserMainSlot, EVENT_BARC_COM);
	PRINT_ALLOC
	fprintf(COM4_BARC_U2C1_OUT,"%s", aszBCL8_STX_DecoderOn);fflush(COM4_BARC_U2C1_OUT);
	PRINT_RELEASE
	if((error = AnswerFromBarcode(BCBuf, BCL8_BARCODE_READ_TIME_OUT)) == errNone)
	{
		/*if(BCBuf[0] == '?')
		{return ERR_BAR_CODE_NO_LABEL;//ERR_BARC_NO_DECODE
		}strcpy(sPtr, &BCBuf[0]);return errInfo;*/
		if(BCBuf[0] == chSTX)
		{
			BCBuf[0] = 'I';
			strcpy(sPtr, &BCBuf[0]);
			return errInfo;
		}
	}
	// in defaulteinstellungen der readers, lesen bis barcode erkannt
	PRINT_ALLOC
	fprintf(COM4_BARC_U2C1_OUT,"%s", aszBCL8_STX_DecoderOff);fflush(COM4_BARC_U2C1_OUT);
	PRINT_RELEASE
	// Antwort von decode off abwarten
	error2 = AnswerFromBarcode(BCBuf, BCL8_BARCODE_READ_TIME_OUT);
	if(error != errRxOverflow)
	{
		if(error == ERR_BARC_DEV_NOT_ANSWER)
		{
			return ERR_BAR_CODE_NO_LABEL;
		}
		else
		{
			return error;
		}
	}
}
return ERR_BARC_COMMUNICATION;
}
void BCL8_StartReaderJustage(void)
{
	BarcodeReset();
	PRINT_ALLOC
	fprintf(COM4_BARC_U2C1_OUT,"%s", aszBCL8_STX_JustageModusOn);fflush(COM4_BARC_U2C1_OUT);
	PRINT_RELEASE
}
void BCL8_StopReaderJustage(void)
{
	PRINT_ALLOC
	fprintf(COM4_BARC_U2C1_OUT,"%s", aszBCL8_STX_JustageModusOff);fflush(COM4_BARC_U2C1_OUT);
	PRINT_RELEASE
}
unsigned short BCL8_ReadReaderJustage(unsigned short Output)
{
	unsigned short error;
	unsigned short scan;
	char *pBarcode;
	FILE *OutHandle;

	switch(Output)
	{
		case 0:	OutHandle = COM5_SLAVE_U1C0_OUT;
				break;
		case 1:	OutHandle = stdout;
				break;
		case 2:	OutHandle = COM2_USB_U1C1_OUT;
				break;
		case 3:OutHandle = COM3_LAN_U2C0_OUT;
				break;
		case 4:	OutHandle = COM5_SLAVE_U1C0_OUT;
				break;
		default: return ERR_WRONG_PARAMETER; 
	}
//	while(1)
//	{
		if((error = ASyncAnswerFromBarcode(BCBuf, 10)) == errNone)
		{
			if(BCBuf[0] == chSTX)
			{
				pBarcode = strchr(BCBuf, '_');
				if(!pBarcode)
				{
					PRINT_ALLOC
					fprintf(COM4_BARC_U2C1_OUT,"%s", aszBCL8_STX_JustageModusOff);fflush(COM4_BARC_U2C1_OUT);
					PRINT_RELEASE
					return ERR_BARC_COMMUNICATION;
				}
				*pBarcode = '\0';
				pBarcode++;
				scan = atoi(&BCBuf[1]);
				if(Output)
				{
					PRINT_ALLOC
					fprintf(OutHandle, "IScan:%u/100 %s", scan, pBarcode);fflush(OutHandle);		/* Senden der Antwort */
					PRINT_RELEASE
					noReadCounter = 0;
				}
			}
			BarcodeReset();
			error = errNone;
		}
		else
		{
			if(error == ERR_BARC_DEV_NOT_ANSWER)
			{
				if(++noReadCounter > 25)
				{
					noReadCounter = 0;
					if(Output)
					{
						PRINT_ALLOC
						fprintf(OutHandle, "IScan:0/100 ?\r");fflush(OutHandle);
						PRINT_RELEASE
					}
				}
				error = errNone;
			
			}
			else
			{
				if(Output)
				{
					PRINT_ALLOC
					fprintf(COM4_BARC_U2C1_OUT,"%s", aszBCL8_STX_JustageModusOff);fflush(COM4_BARC_U2C1_OUT);
					PRINT_RELEASE
				}
				return error;
			}
	
		}
//	}
	return error;

}

unsigned short BCL8_ReaderJustage(FILE *OutHandle)
{
	unsigned short error;
	unsigned short scan;
	char *pBarcode;
	BarcodeReset();
	PRINT_ALLOC
	fprintf(COM4_BARC_U2C1_OUT,"%s", aszBCL8_STX_JustageModusOn);fflush(COM4_BARC_U2C1_OUT);
	PRINT_RELEASE

	while(1)
	{
		if((error = AnswerFromBarcode(BCBuf, BCL8_BARCODE_READ_TIME_OUT)) == errNone)
		{
			if(BCBuf[0] == chSTX)
			{
				pBarcode = strchr(BCBuf, '_');
				if(!pBarcode)
				{
					PRINT_ALLOC
					fprintf(COM4_BARC_U2C1_OUT,"%s", aszBCL8_STX_JustageModusOff);fflush(COM4_BARC_U2C1_OUT);
					PRINT_RELEASE
					return ERR_BARC_COMMUNICATION;
				}
				*pBarcode = '\0';
				pBarcode++;
				scan = atoi(&BCBuf[1]);
				PRINT_ALLOC
				fprintf(OutHandle, "IScan:%u/100 %s", scan, pBarcode);fflush(OutHandle);		/* Senden der Antwort */
				PRINT_RELEASE
			}
			BarcodeReset();
		}
		else
		{
			if(error == ERR_BARC_DEV_NOT_ANSWER)
			{
				PRINT_ALLOC
				fprintf(OutHandle, "IScan:0/100 ?\r");fflush(OutHandle);		/* Senden der Antwort */
				PRINT_RELEASE
			
			}
			else
			{
				PRINT_ALLOC
				fprintf(COM4_BARC_U2C1_OUT,"%s", aszBCL8_STX_JustageModusOff);fflush(COM4_BARC_U2C1_OUT);
				PRINT_RELEASE
				return error;
			}
	
		}
	}
}

unsigned short BCL8_Reset(void)
{
	unsigned short error, a;
for(a = 0; a < 3; a++)
{
	BarcodeReset();
	PRINT_ALLOC
	fprintf(COM4_BARC_U2C1_OUT,"%s", aszBCL8_STX_Reset);fflush(COM4_BARC_U2C1_OUT);
	PRINT_RELEASE
	if((error = AnswerFromBarcode(BCBuf, BCL8_RESET_TIME_OUT)) == errNone)
	{
		if(BCBuf[0] == chSTX)
		{
			if(BCBuf[1] == 'S')
			{
				BarcodeReset();
				SetBarcodeReader(BARCODE_READER_BCL8);
				return errNone;
			}
		}
	}
}
return ERR_BARC_DEV_NOT_ANSWER;
}


#endif

