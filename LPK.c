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
#include "DeviceConfig.h"
#include "Drv_config.h"
#include "MDrive_CmdCtrl.h"
#include "MDriveCmd.h"
#include "MyScanner.h"
#include "Scan.h"
#include "Cxconfig.h"
#include "Felix_Misc.h"

#include "LPK.h"

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
extern IMS_CommandBlock Msg;
extern char *g_pcCmdBuf, g_acAnswerBuf[];
//****************************************************************************
// public data
//****************************************************************************

//****************************************************************************
// private data
//****************************************************************************
char cLPKLastCmdBuffer[128];
char cLPKAnswerBuf[ANSWER_BUF_LEN+1];
//****************************************************************************
// private functions
//****************************************************************************
char GetLPKBusAdresse(unsigned short Adresse);
//****************************************************************************
// public functions
//****************************************************************************
unsigned short LastLPKCommando(const char *pszCmd)
{
	if(strstr(cLPKLastCmdBuffer, pszCmd))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

char GetLPKBusAdresse(unsigned short Adresse)
{
	char BusAdresse;
	switch(Adresse)
	{
		//---- PENTA DISK -----------------
		#ifdef _CF_PENTA_DISK
		case ADR_LPK_12:	BusAdresse = AXIS_ADR_LPK_12;
							break;
		case ADR_LPK_22:	BusAdresse = AXIS_ADR_LPK_22;
							break;
		case ADR_LPK_32:	BusAdresse = AXIS_ADR_LPK_32;
							break;
		case ADR_LPK_42:	BusAdresse = AXIS_ADR_LPK_42;
							break;
		#endif
		default:			BusAdresse = AXIS_ADR_LPK_DEF;
	}
	return BusAdresse;
}

unsigned short GetLPKConnected(void)
{
	unsigned short error = errNone;
	unsigned short Connect;

	strcpy(cLPKLastCmdBuffer, "H v");
	error = CommandLPK(1, 2, cLPKAnswerBuf);
	if(error == LPK_BUS_ERR_HEAD_NOT_CONNECTED)
	{
		Connect = FALSE;
	}
	else
	{
		Connect = TRUE;
	}
	return Connect;


}
unsigned short CommandLPK(unsigned short RepeatCmd, unsigned short Adresse, char *sPtr)
{
	unsigned short error = errNone;
	char *pStart, *pEnd;
	
	if(!RepeatCmd)
	{
		pStart = GetScannerPtr();
		if(*pStart == ' '){++pStart;}
		pEnd = strrchr(GetScannerPtr(), '\n');
		*pEnd = '\0';
		sprintf((char *)Msg.Argument.cBuffer,"%c %s", GetLPKBusAdresse(Adresse), pStart);
		strcpy(cLPKLastCmdBuffer, (char *)Msg.Argument.cBuffer);
	}
	else
	{
		strcpy((char *)Msg.Argument.cBuffer, cLPKLastCmdBuffer);
	}
	Msg.eCommand = eLPK;
//	Msg.AxisID  = Adresse;//Missbraucht für AdresseCyBiCtrl
	Msg.AdresseCyBiCtrl = Adresse;
	K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	error = Msg.Error;
	if(error == errInfo)
	{
		sprintf(sPtr,"%s\r", (char *)Msg.Argument.cBuffer);
		if(Msg.Argument.cBuffer[0] != 0)
		{
			if(strstr((char *)Msg.Argument.cBuffer, "E65535"))
			{
				SetEStopAnswerFromLPK();
			}
		}
		return errInfoLPK;
	}
	return error;
}

unsigned short LPK_Send_PCCmd(unsigned short DelayTime, unsigned short Adresse, char *sPtr)
{
	unsigned short error = errNone;

	sprintf((char *)Msg.Argument.cBuffer,"%c %s", GetLPKBusAdresse(Adresse), sPtr);
	Msg.eCommand = eLPKPCCmdSend;
//	Msg.AxisID  = Adresse;//Missbraucht für AdresseCyBiCtrl
	Msg.AdresseCyBiCtrl = Adresse;
	Msg.Mask = DelayTime;//Missbraucht für Time
	error = K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	if(error != K_OK)
	{
		return ERR_SYSTEM;
	}
	error = Msg.Error;
	return error;
}

unsigned short LPK_ReceiveAnswer_PCCmd(char *sPtr)
{
	unsigned short error = errNone;

	Msg.eCommand = eLPKPCCmdReceive;
	error = K_Mesg_Send_Wait(MBX_MOTOR, 0, &Msg);
	if(error != K_OK)
	{
		return ERR_SYSTEM;
	}
	error = Msg.Error;
	if(error == errInfo)
	{
		//sprintf(g_acAnswerBuf,"%s\r", (char *)Msg.Argument.cBuffer);
		sprintf(sPtr,"%s\r", (char *)Msg.Argument.cBuffer);
		return errInfoLPK;
	}
	return error;
}
