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
extern unsigned char ASyncCtrlSlot;
//****************************************************************************
// public data
//****************************************************************************
char BCBuf[BC_BUFSIZE];
//****************************************************************************
// private data
//****************************************************************************
unsigned short m_barcode_reader = BARCODE_READER_BCL8;
unsigned short m_barcode_config = TRUE;
//****************************************************************************
// private functions
//****************************************************************************

//****************************************************************************
// public functions
//****************************************************************************
/* FUNCTION: 	BarcodeReset
   PARAMETERS:	---
   DESCRIPTION:	Reset Barcodeinterface
   RETURNS: 	CB_READY */
unsigned short BarcodeReset(void)
{
	UART4_ResetDataBufferAndErrorFlag();
	BCBuf[0]= 0;
	return errNone;
}


unsigned short AnswerFromBarcode( char *pAnsw, unsigned short Timeout)
{
	unsigned short events;
	unsigned short ComEvents;
	//unsigned short i, a;
	unsigned char *pPtr;
	K_Task_Wait(20);
		//a = 1;
		//do
		//{
			WDO();
			events = K_Event_Wait(EVENT_BARC_COM | EVENT_USER_BREAK, Timeout, 2 );	// löscht nicht die angegebenen Events
			if(events)
			{
				if(events & EVENT_USER_BREAK)
				{
					K_Event_Reset(UserMainSlot, EVENT_USER_BREAK);

					K_Task_Wait(20); // ausreichend Zeit für evtl. noch zu sendende Daten!
					sprintf(pAnsw,"+E%u\r", ERR_PUMP_PROG_ABORT);
					// beim Programmmabbruch eines laufenden Befehles wird als Antwort auf das Break noch ein ACK benötigt
					return ERR_PUMP_PROG_ABORT;
				}
				if(events & EVENT_BARC_COM)
				{
					K_Event_Reset(UserMainSlot, EVENT_BARC_COM);
					K_Task_Wait(20);
					ComEvents = UART_GetEvents(COM4_BARC_U2C1_IN);
					if(ComEvents == COM_EVENT_CR)
					{
						pPtr = UART4_InpBuffer;
						sprintf(pAnsw,"%s\r", pPtr);
						return errNone;
					}
					if(ComEvents == COM_EVENT_OVERFLOW)
					{
						//a = 0;
						return errRxOverflow;
					}
				}
				return ERR_BARC_COMMUNICATION;
			}
			else
			{
				return ERR_BARC_DEV_NOT_ANSWER;
			}
		//}while(a);
	//return ERR_NO_ANSWER_FROM_DEV;
	//return ERR_BARC_DEV_NOT_ANSWER;
}

unsigned short ASyncAnswerFromBarcode( char *pAnsw, unsigned short Timeout)
{
	unsigned short events;
	unsigned short ComEvents;
	//unsigned short i, a;
	unsigned char *pPtr;
	//K_Task_Wait(20);
		//a = 1;
		//do
		//{
			WDO();
			events = K_Event_Wait(EVENT_BARC_COM | EVENT_USER_BREAK, Timeout, 2 );	// löscht nicht die angegebenen Events
			if(events)
			{
				if(events & EVENT_USER_BREAK)
				{
					K_Event_Reset(ASyncCtrlSlot, EVENT_USER_BREAK);

					K_Task_Wait(20); // ausreichend Zeit für evtl. noch zu sendende Daten!
					sprintf(pAnsw,"+E%u\r", ERR_PUMP_PROG_ABORT);
					// beim Programmmabbruch eines laufenden Befehles wird als Antwort auf das Break noch ein ACK benötigt
					return ERR_PUMP_PROG_ABORT;
				}
				if(events & EVENT_BARC_COM)
				{
					K_Event_Reset(ASyncCtrlSlot, EVENT_BARC_COM);
					K_Task_Wait(20);
					ComEvents = UART_GetEvents(COM4_BARC_U2C1_IN);
					if(ComEvents == COM_EVENT_CR)
					{
						pPtr = UART4_InpBuffer;
						sprintf(pAnsw,"%s\r", pPtr);
						return errNone;
					}
					if(ComEvents == COM_EVENT_OVERFLOW)
					{
						//a = 0;
						return errRxOverflow;
					}
				}
				return ERR_BARC_COMMUNICATION;
			}
			else
			{
				return ERR_BARC_DEV_NOT_ANSWER;
			}
		//}while(a);
	//return ERR_NO_ANSWER_FROM_DEV;
	//return ERR_BARC_DEV_NOT_ANSWER;
}


/********************************************************************
 *  FUNCTION:    SetBarcodeConfig
 *
 *  PARAMETERS:  unsigned char value
 *                  TRUE = Barcodeleser vorhanden,
 *                  FALSE = kein Barcodeleser vorhanden
 *
 *  DESCRIPTION:	Setzen der globalen Variable barcode_config
 *
 *  RETURNS:		none
 ********************************************************************/
void SetBarcodeConfig(unsigned short value)
{
	m_barcode_config = value;
}


/********************************************************************
 *  FUNCTION:    GetBarcodeConfig
 *
 *  PARAMETERS:  none
 *
 *  DESCRIPTION: Auslesen der Variable "barcode_config"
 *
 *  RETURNS:     Inhalt der Variable "barcode_config"
 ********************************************************************/
unsigned short GetBarcodeConfig(void)
{
	return m_barcode_config;
}


/* *******************************************************************
 *  FUNCTION:    SetBarcodeReader
 *
 *  PARAMETERS:  unsigned char reader_type
 *                   Der Barcodereadertyp
 *
 *  DESCRIPTION: Die Variable "barcode_reader" mit dem 
 *               Barcodereadertyp beschreiben.
 *
 *  RETURNS:     none
 ******************************************************************* */
void SetBarcodeReader(unsigned short value)
{
	m_barcode_reader = value;
}


/* *******************************************************************
 *  FUNCTION:    GetBarcodeReader
 *
 *  PARAMETERS:  none
 *
 *  DESCRIPTION: Die Variable "barcode_reader" mit dem 
 *               Barcodereadertyp auslesen
 *
 *  RETURNS:     Inhalt der Variable "barcode_reader"
 ******************************************************************* */
unsigned short GetBarcodeReader(void)
{
 return m_barcode_reader;
}

