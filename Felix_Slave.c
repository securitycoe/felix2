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

#include "DevError.h"
#include "30-5015_Felix/XE167F/main.h"
#include "30-5015_Felix/XE167F/U1C0_SLAVE_COM5.h"
#include "MyScanner.h"
#include "Scan.h"
#include "Cxconfig.h"
#include "DeviceConfig.h"
//#include "Shared/Stacker_Lifter_Allgemein/StacError.h"
#include "Drv_config.h"
#include "Felix_CmdCtrl.h"
#include "Felix_Slave.h"
//****************************************************************************
// @Macros
//****************************************************************************

//****************************************************************************
// local definitions
//****************************************************************************
char CmdResetAllBarcodes[]= "F";
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
unsigned short m_first_comm_to_slave = TRUE; // Zum Slave hat noch keine Kommunikation stattgefunden
unsigned short m_SlavePresent = FALSE;
//****************************************************************************
// private functions
//****************************************************************************
unsigned short SyncSlave(void);

void SlaveComErrHandler(unsigned short Adr);
unsigned short CheckSlave(void);
//****************************************************************************
// public functions
//****************************************************************************

unsigned short CheckSlave(void)
{
	if(m_first_comm_to_slave)
	{
		m_SlavePresent = SyncSlave();
		if(m_SlavePresent)
		{
			m_first_comm_to_slave= FALSE;	
		}
	}
	return m_SlavePresent;
}


unsigned short ResetBarcodeDataForSlave(char *pAnsw)
{
	unsigned short error;
	if(CheckSlave())
	{
		error = SendToSlave( 1, CmdResetAllBarcodes, pAnsw);
	}
	else
	{
		//error = ERR_NO_ANSWER_FROM_DEV;
		error = ERR_DEV_NOT_ANSWER;
	}
	return error;
	
}
unsigned short CommandSlave(unsigned short Adr, char *pAnsw)
{
	unsigned short error = errNone;
	unsigned short ScannerIdent;
	char buf[25];
	char *pStart, *pEnd;

	pStart = GetScannerPtr();
	if(*pStart == ' '){++pStart;}
	pEnd = strrchr(GetScannerPtr(), '\n');
	*pEnd = '\0';


	//SaveScannerBuffer();
	ScannerIdent = scanner(buf);
	switch(ScannerIdent)
	{

		case HASH		:	error = ERR_PROG_ABORT;
							break;
		case PERCENT	:	error = errSync;
							break;
		case Scan_EOF	:	error = ERR_CMD_NOT_IMPLEMENTED;
							break;
		default			:
							if(Adr > 10)
							{
								Adr -= 10;
								if(CheckSlave())
								{
									error = SendToSlave(Adr, pStart, pAnsw);
								}
								else
								{
									//error = ERR_NO_ANSWER_FROM_DEV;
									error = ERR_DEV_NOT_ANSWER;
								}
							}
							else
							{
								error = ERR_UNKNOWN_ADDRESS;
							}
							break;
	}
	return error;
}

unsigned short SyncSlave(void)
{
	unsigned short i;
	short a;
	int c;	
	for(i=0; i<4; i++)
	{
	    putc(C_SYNC, COM5_SLAVE_U1C0_OUT);fflush(COM5_SLAVE_U1C0_OUT);/* Senden von SYNC */
		a = 10;
		do
	    {
			K_Task_Wait(100);
	    	WDO();
		  	c = UART5_GetChar();
		  	if(c == ACK)
			{	
				return TRUE;
			}
			a--;
	    }
	    while(a > 0);
	}
	return FALSE;
}

unsigned short SendToSlave(unsigned short Adr, char *pSend, char *pAnsw)
{
	unsigned short error;
	unsigned short i;
	for(i=0; i < 2; i++)
	{
		UART5_ResetDataBufferAndErrorFlag();
		PRINT_ALLOC
		fprintf(COM5_SLAVE_U1C0_OUT,"%u %s\r", Adr, pSend);fflush(COM5_SLAVE_U1C0_OUT);
		PRINT_RELEASE
		error = AnswerFromSlave(Adr, pAnsw);
		if(error != errRxOverflow)
		{
			return error;
		}
		K_Task_Wait(100);
		UART5_ResetDataBufferAndErrorFlag();
		if(!SyncSlave())
		{
			return ERR_DEV_NOT_ANSWER;
		}
	}
	return ERR_DEV_NOT_ANSWER;
} 

unsigned short SendToSlavePCCmd(unsigned short Adr, char *pSend)
{
	//unsigned short error;
	UART5_ResetDataBufferAndErrorFlag();
	PRINT_ALLOC
	fprintf(COM5_SLAVE_U1C0_OUT,"%u %s\r", Adr, pSend);fflush(COM5_SLAVE_U1C0_OUT);
	PRINT_RELEASE
	return errNone;
} 

unsigned short AnswerFromSlave(unsigned short Adr, char *pAnsw)
{
	unsigned short events;
	unsigned short ComEvents;
	unsigned short i, a;
	char *pPtr;
//	K_Task_Wait(20);
	for(i=0; i < 3; i++)
	{
		a = 1;
		do
		{
			WDO();
			events = K_Event_Wait(EVENT_SLAVE_COM | EVENT_USER_BREAK, 30, 0 );	// löscht nicht die angegebenen Events
			if(events)
			{
				if(events & EVENT_USER_BREAK)
				{
					K_Event_Reset(UserMainSlot, EVENT_USER_BREAK);
					SendBreakToSlave();
					K_Task_Wait(20); // ausreichend Zeit für evtl. noch zu sendende Daten!
					sprintf(pAnsw,"+E%u\r", ERR_PUMP_PROG_ABORT);
					// beim Programmmabbruch eines laufenden Befehles wird als Antwort auf das Break noch ein ACK benötigt
					return errInfo;
				}
				if(events & EVENT_SLAVE_COM)
				{
					K_Event_Reset(UserMainSlot, EVENT_SLAVE_COM);
					ComEvents = UART_GetEvents(COM5_SLAVE_U1C0_IN);
					if(ComEvents == COM_EVENT_CR)
					{
						if((UART5_InpBuffer[0] == ACK) || (UART5_InpBuffer[0] == NACK))
						{
							pPtr = UART5_InpBuffer;
							pPtr++;
						}
						else
						{
							pPtr = UART5_InpBuffer;
						}
						if(strchr(pPtr, RDY))
						{
							return errNone;
						}
						sprintf(pAnsw,"%s\r", pPtr);
						return errInfo;
					}
					if(ComEvents == COM_EVENT_OVERFLOW)
					{
						a = 0;
					}
				}
			}
			else
			{
				if(UART5_InpBuffer[0] == NACK)
				{
					return errRxOverflow;
				}
			}
		}while(a);
		if(UART5_InpBuffer[0] == NACK)
		{
			return errRxOverflow;
		}
		else
		{
			SlaveComErrHandler(Adr);	// nochmalige Abfrage der letzten Antwort 
		}
	}//end...for
	//return ERR_NO_ANSWER_FROM_DEV;
	return ERR_DEV_NOT_ANSWER;
}

void SlaveComErrHandler(unsigned short Adr)
{
	K_Task_Wait(100); // ausreichend Zeit für evtl. noch zu sendende Daten!
	UART5_ResetDataBufferAndErrorFlag();
	if(Adr > 10)
	{
		do
		{
			WDO();
			K_Task_Wait(1);
			Adr -= 10;

		}while(Adr >10);
	}
	PRINT_ALLOC
	fprintf(COM5_SLAVE_U1C0_OUT,"%u l\r", Adr);fflush(COM5_SLAVE_U1C0_OUT);// Antwort nochmals senden
	PRINT_RELEASE
}

/*EVENT_SLAVE_COM
SaveScannerBuffer();
ReloadScannerBuffer();
ScannerIdent = scanner(buf);

	switch(ScannerIdent)
						if((strcmp(buf, _CmdInit) == 0) || (strcmp(buf, CSTR_INIT) == 0)){error = Cmd_Init();break;}
*/

/*  WaitForReadyPumps wird auch als internes Kommando verwendet und sollte es nur gesendet werden, wenn auch wirklich ein Stacker
    angeschlossen ist, unabhängig von der Konfiguration, ähnlich wie beim Vorgänger */
/*ubyte WaitForReadyPumps(void)
{	ubyte state;
    char Buf[15];
 	if(IsSlavePresent()) 	
	{
		state= SendToStack(CmdWaitForReadyPumps, Buf);
		if((state == INFO_FLAG)&&(Buf[0] == RDY))
			state= CB_READY;
		else
			state= atoi(&Buf[1]);
	}
	else
		state= CB_READY;
	return state;
}
*/

unsigned short isSlavePresent(void)
{
	return m_SlavePresent;
}

void SendBreakToSlave(void)
{
	PRINT_ALLOC
	fprintf(COM5_SLAVE_U1C0_OUT,"##");fflush(COM5_SLAVE_U1C0_OUT);
	PRINT_RELEASE
}