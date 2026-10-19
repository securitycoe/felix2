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
#include "Felix_BCL8.h"
#include "Felix_ASyncCtrl.h"

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
extern unsigned char UserMainSlot;
//****************************************************************************
// public data
//****************************************************************************

//****************************************************************************
// private data
//****************************************************************************

//****************************************************************************
// private functions
//****************************************************************************

//****************************************************************************
// public functions
//****************************************************************************

unsigned short Cmd_ReadBarcode(char *sPtr)
{
	unsigned short error;
	unsigned short Para;
	if(GetBarcodeReader() == BARCODE_READER_NO) return ERR_BARC_DEV_NOT_ANSWER;
	//Para 1
	if((error = scanUShort(&Para)) == errNone)
	{
		return errNone;
	}
	#if(CF_USED_BARCODE_READER == DEF_OPTICON)
	return OpticonReadBarcode(sPtr);
	#endif

	#if(CF_USED_BARCODE_READER == DEF_BCL8)
	return BCL8_ReadBarcode(sPtr);
	#endif
	#if(CF_USED_BARCODE_READER == DEF_NO_READER)
	return ERR_BARC_DEV_NOT_ANSWER;
	#endif
}

unsigned short Cmd_BarcodeReaderJustage(FILE *OutHandle)
{
	unsigned short error;
	unsigned short HostOutput, onoff;
	if((error = scanOnOff(&onoff)) != errNone) return error;
	if(onoff)
	{
		if((error = scanUShort(&HostOutput)) != errNone)
		{
			HostOutput = 0;
		}
		switch(HostOutput)
		{
			case 0:	//keine Ausgabe
					break;
			case 1://Ausgabe Host
					if(OutHandle == stdout)
					{
						HostOutput = 1;
					}
					if(OutHandle == COM2_USB_U1C1_OUT)
					{
						HostOutput = 2;
					}
					if(OutHandle == COM3_LAN_U2C0_OUT)
					{
						HostOutput = 3;
					}
					break;
			case 2:	//Ausgabe Slave
					HostOutput = 4;
					break;
			default: return ERR_WRONG_PARAMETER;

		}
		/*if(HostOutput == 1)
		{
			if(OutHandle == stdout)
			{
				HostOutput = 1;
			}
			if(OutHandle == COM2_USB_U1C1_OUT)
			{
				HostOutput = 2;
			}
			if(OutHandle == COM3_LAN_U2C0_OUT)
			{
				HostOutput = 3;
			}
		}*/
	}

	#if(CF_USED_BARCODE_READER == DEF_BCL8)

	
	return g_ASync_BarcodeJustage( onoff, HostOutput);
	//return BCL8_ReaderJustage(OutHandle);
	#else
	return ERR_BARC_DEV_NOT_ANSWER;
	#endif
	
}
/* FUNCTION: 	BarcodeSendCmdTo
   PARAMETERS:	Pointer auf Kommandostring
   DESCRIPTION:	Senden des Kommandostrings zum Reader 
   RETURNS: 	CB_READY */

unsigned short Cmd_BarcodeSendCmdTo(char *sPtr)
{
	unsigned short error;
	char *pStart, *pEnd;

	pStart = GetScannerPtr();
	if(*pStart == ' '){++pStart;}
	pEnd = strrchr(GetScannerPtr(), '\n');
	*pEnd = '\0';

	if((GetBarcodeReader() != BARCODE_READER_OPTICON) && (GetBarcodeReader() != BARCODE_READER_BCL8)) return ERR_BARC_DEV_NOT_ANSWER;
	BarcodeReset();
	
	if(GetBarcodeReader() == BARCODE_READER_OPTICON) 
	{
		PRINT_ALLOC
		fprintf(COM4_BARC_U2C1_OUT,"%s\r", pStart);fflush(COM4_BARC_U2C1_OUT);
		PRINT_RELEASE
	}
	if(GetBarcodeReader() == BARCODE_READER_BCL8) 
	{
		PRINT_ALLOC
		fprintf(COM4_BARC_U2C1_OUT,"%c%s\r\n", chSTX, pStart);fflush(COM4_BARC_U2C1_OUT);
		PRINT_RELEASE
		if((error= AnswerFromBarcode(BCBuf, OPTICON_BARCODE_TIME_OUT)) == errNone)
		{
			if(BCBuf[0] == chSTX)
			{
				BCBuf[0] = 'I';
				strcpy(sPtr, &BCBuf[0]);
				return errInfo;
			}
		}
		else
		{
			return error;
		}
	}
	return errNone;

}


unsigned short Cmd_BarcodeReset(void)
{
	#if(CF_USED_BARCODE_READER == DEF_OPTICON)
	BarcodeReset();
	return errNone
	#endif

	#if(CF_USED_BARCODE_READER == DEF_BCL8)
	return BCL8_Reset();
	#endif
	#if(CF_USED_BARCODE_READER == DEF_NO_READER)
	return ERR_BARC_DEV_NOT_ANSWER;
	#endif
}

/* FUNCTION: 	BarcodeReaderShowTypAndVersion
   PARAMETERS:	Anzeigeoption
   DESCRIPTION:	Lesen und Anzeigen der Firmwareversion
   				(nicht portokollkompatibel!)
   RETURNS: 	--- */

unsigned short BarcodeReaderShowTypAndVersion(FILE *OutHandle, unsigned short flg)
{
	unsigned short error;
	enum BARC_DEVICE ident;

	ident= BARCODE_READER_NO;
	BarcodeReset();
#if(CF_USED_BARCODE_READER == DEF_OPTICON)
	PRINT_ALLOC
	fprintf(COM4_BARC_U2C1_OUT,"%s", OptiGetSoftwareVersion);fflush(COM4_BARC_U2C1_OUT);
	PRINT_RELEASE
	if((error= AnswerFromBarcode(BCBuf, OPTICON_BARCODE_TIME_OUT)) == errNone)
    {
    	if(flg)
    	{
			PRINT_ALLOC
			fprintf(OutHandle, "Barcode reader:\r Opticon\r");fflush(OutHandle);		/* Senden der Antwort */
			fprintf(OutHandle, " Firmware %s",BCBuf);fflush(OutHandle);		/* Senden der Antwort */
			PRINT_RELEASE
		}
		ident= BARCODE_READER_OPTICON;
	}
	else
	{	if(flg)
			PRINT_ALLOC
			fprintf(OutHandle, "No Barcode was found\r");fflush(OutHandle);		/* Senden der Antwort */
			PRINT_RELEASE
    }
#endif
#if(CF_USED_BARCODE_READER == DEF_BCL8)
	PRINT_ALLOC
	fprintf(COM4_BARC_U2C1_OUT,"%s", aszBCL8_STX_Firmware);fflush(COM4_BARC_U2C1_OUT);
	PRINT_RELEASE

	if((error= AnswerFromBarcode(BCBuf, BCL8_BARCODE_READ_TIME_OUT)) == errNone)
    {
    	if(flg)
    	{
			if(BCBuf[0] == chSTX)
			{
				BCBuf[0] = ' ';
			}
			PRINT_ALLOC
			fprintf(OutHandle, "Barcode reader:\r BCL8\r");fflush(OutHandle);		/* Senden der Antwort */
			fprintf(OutHandle, " Firmware %s",BCBuf);fflush(OutHandle);		/* Senden der Antwort */
			PRINT_RELEASE
		}
		ident= BARCODE_READER_OPTICON;
	}
	else
	{	if(flg)
			PRINT_ALLOC
			fprintf(OutHandle, "No Barcode was found\r");fflush(OutHandle);		/* Senden der Antwort */
			PRINT_RELEASE
    }

#endif
    return ident;
}


/* *******************************************************************
 *  FUNCTION:    TestBarcodeReaderType
 *
 *  PARAMETERS:  none
 *
 *  DESCRIPTION: Ermitteln des Barcodereadertypes
 *
 *  RETURNS:     READY   = i.O.
 *               Fehlernummer X = Kein Reader erkannt, oder Problem
 ******************************************************************* */
 /*
unsigned char TestBarcodeReaderType(void)
{
#if(CF_USED_BARCODE_READER == DEF_DL3)
unsigned char i, status;
#endif
#if(CF_USED_BARCODE_READER == DEF_NOT_READER)
	SetBarcodeReader(DL3);
	return READY;
#endif
	SetBarcodeReader(BARCODE_READER_NO);
	if(!GetBarcodeConfig()) return BARC_DEV_NOT_ANSWER; // Barcode nicht konfiguriert
#if(CF_USED_BARCODE_READER == DEF_BCL8)
	BarcodeComInit();
	reader_end_code='\r';

	// Warten auf Antwort nach PowerOn
	if(BCL8_WaitReset()) return READY;
	if(BCL8_Reset() == READY) return READY;

#endif
//evtl. Barcodeflag auf FALSE setzen, wenn das Programm an dieser Stelle verlassen wird
	SetBarcodeReader(BARCODE_READER_NO);
	return BARC_DEV_NOT_ANSWER;
} 
*/