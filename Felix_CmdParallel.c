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
#include "Felix_CmdCtrl.h"
//#include "Felix_Cmd.h"
#include "MyScanner.h"
#include "Scan.h"
#include "Cxconfig.h"
/*#include "Felix_Hardware.h"
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
*/
#include "LPK.h"
#include "Felix_SlaveTask.h"


//****************************************************************************
// local definitions
//****************************************************************************
#define FREE_ADR 0
#define CHARMEM_PCCMD_LENGTH 90
#define MAX_PCCMD 2
typedef struct
{
	unsigned short DelayTime;
	short Adr;
	char szCharBlock[CHARMEM_PCCMD_LENGTH];
	
}udtPCCmdBlock;
udtPCCmdBlock g_PCCmdData[MAX_PCCMD];
unsigned short g_PCDataClear = 1;

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
// internal functions
//****************************************************************************

//****************************************************************************
// public data
//****************************************************************************

//****************************************************************************
// private data
//****************************************************************************

//****************************************************************************
// public functions
//****************************************************************************

//****************************************************************************
// private functions
//****************************************************************************

unsigned short Cmd_ParallelCmdLoad(void);
//unsigned short Cmd_ParallelCmdRun(char *sPtr);
unsigned short Cmd_ParallelCmdRun(void);
unsigned short ParallelCmdClear(void);



unsigned short Cmd_ParallelCmdLoad(void)
{
	unsigned short error = errNone;
	unsigned short i, siz;
	unsigned short adr_gefunden = 0;
	int value;
	short Adr;
	unsigned short DelayTime;
	if((error = scanUShort(&DelayTime)) != errNone) return error;
	if((error = scanShort(&Adr)) != errNone) {return error;}
	if(Adr <= FREE_ADR)
	{
		return ERR_WRONG_PARAMETER;
	}
	if((value = GetC_ScannerBuf()) != ' ') {UngetC_ScannerBuf(value);}
	siz = strlen(GetScannerPtr());
	if(siz > CHARMEM_PCCMD_LENGTH)
	{
		return ERR_WRONG_PARAMETER;		
	}
	// Adresse vorhanden ?, wenn ja, überschreiben	
	for(i = 0; i < MAX_PCCMD; i++)
	{
		WDO();
		if(g_PCCmdData[i].Adr == Adr)
		{
			strcpy((char *)g_PCCmdData[i].szCharBlock, GetScannerPtr());
			g_PCCmdData[i].DelayTime = DelayTime;
			adr_gefunden = 1;
			g_PCDataClear = 1;
			break;
		}
	}
	if(!adr_gefunden)
	{
		// position nicht gefunden, neue position anlegen
		for(i = 0; i < MAX_PCCMD; i++)
		{
			WDO();
			if(g_PCCmdData[i].Adr == FREE_ADR)
			{
				g_PCCmdData[i].Adr = Adr;
				g_PCCmdData[i].DelayTime = DelayTime;
				strcpy((char *)g_PCCmdData[i].szCharBlock, GetScannerPtr());
				adr_gefunden = 1;
				g_PCDataClear = 1;
				break;
			}
		}
	}
	if(!adr_gefunden){return DISK_FULL;}
	return error;
}
//unsigned short Cmd_ParallelCmdRun(char *sPtr)
unsigned short Cmd_ParallelCmdRun(void)
{
	unsigned short error_lpk = errNone;
	unsigned short error_slave = errNone;
	unsigned short SendToLPk = 0;
	unsigned short SendToSlave = 0;
	unsigned short i;
	char strHelp[90];

	if(g_PCCmdData[0].Adr == FREE_ADR){ParallelCmdClear();return ERR_WRONG_PARAMETER;}
	if(g_PCCmdData[0].Adr == g_PCCmdData[1].Adr){ParallelCmdClear();return ERR_WRONG_PARAMETER;}
	g_acAnswerBuf[0] = 0;strHelp[0]=0;
// Empfangen
	for(i = 0; i < MAX_PCCMD; i++)
	{
		WDO();
		//if(g_PCCmdData[i].Adr == 2){error_lpk = LPK_Send_PCCmd(g_PCCmdData[i].DelayTime, (char *)g_PCCmdData[i].szCharBlock);
		if((g_PCCmdData[i].Adr == ADR_LPK_2) || (g_PCCmdData[i].Adr == ADR_LPK_12) || (g_PCCmdData[i].Adr == ADR_LPK_22)
			 || (g_PCCmdData[i].Adr == ADR_LPK_32) || (g_PCCmdData[i].Adr == ADR_LPK_42))
		{
			error_lpk = LPK_Send_PCCmd(g_PCCmdData[i].DelayTime, g_PCCmdData[i].Adr, (char *)g_PCCmdData[i].szCharBlock);
			SendToLPk = 1;
		}
		else
		{
			if(g_PCCmdData[i].Adr > 10)
			{
				error_slave	= g_Slave_Send(g_PCCmdData[i].DelayTime, g_PCCmdData[i].Adr, (char *)g_PCCmdData[i].szCharBlock);
				SendToSlave = 1;
			}
			else
			{
				if(i == 0)
				{
					ParallelCmdClear();return ERR_WRONG_PARAMETER;
				}
			}
		}
	}
// Empfangen
	for(i = 0; i < MAX_PCCMD; i++)
	{
		WDO();
		//if((g_PCCmdData[i].Adr == 2) && SendToLPk)
		if(((g_PCCmdData[i].Adr == ADR_LPK_2) || (g_PCCmdData[i].Adr == ADR_LPK_12) || (g_PCCmdData[i].Adr == ADR_LPK_22)
			 || (g_PCCmdData[i].Adr == ADR_LPK_32) || (g_PCCmdData[i].Adr == ADR_LPK_42))&& SendToLPk)
		{
			if(error_lpk == errNone)
			{
				if(g_PCCmdData[i].DelayTime != 0){K_Task_Wait(g_PCCmdData[i].DelayTime);}
				error_lpk = LPK_ReceiveAnswer_PCCmd((char *)g_PCCmdData[i].szCharBlock);
				if((error_lpk == errInfo) || (error_lpk == errInfoLPK))
				{
					sprintf(strHelp,"%s",(char *)g_PCCmdData[i].szCharBlock);
					strHelp[strlen(strHelp)-1]='\n'; // CR ausblenden
					strcat(g_acAnswerBuf,strHelp);
				}
				else
				{
						if(error_lpk == errNone)
						{
							sprintf(strHelp,"!\n");strcat(g_acAnswerBuf,strHelp);
						}
						else
						{
							sprintf(strHelp,"E%u\n",error_lpk);strcat(g_acAnswerBuf,strHelp);

						}
				}
			}
			else
			{
				sprintf(strHelp,"E%u ",error_lpk);
				strcat(g_acAnswerBuf,strHelp);
			}
		}
		else
		{
			if((g_PCCmdData[i].Adr > 10) && SendToSlave)
			{
				if(error_slave == errNone)
				{
					if(g_PCCmdData[i].DelayTime != 0){K_Task_Wait(g_PCCmdData[i].DelayTime);}
					error_slave	= g_Slave_Answ((char *)g_PCCmdData[i].szCharBlock);
					if(error_slave == errInfo)
					{
						sprintf(strHelp,"%s",(char *)g_PCCmdData[i].szCharBlock);
						strHelp[strlen(strHelp)-1]='\n'; // CR ausblenden
						strcat(g_acAnswerBuf,strHelp);

					}
					else
					{
						if(error_slave == errNone)
						{
							sprintf(strHelp,"!\n");strcat(g_acAnswerBuf,strHelp);
						}
						else
						{
							sprintf(strHelp,"E%u\n",error_slave);strcat(g_acAnswerBuf,strHelp);
						}
					}
				}
				else
				{
					sprintf(strHelp,"E%u\n",error_slave);strcat(g_acAnswerBuf,strHelp);
				}
			}
		}
	}
	//g_acAnswerBuf[strlen(g_acAnswerBuf)-1]=0; // Letztes Leerzeichen ausblenden
  	strcat(g_acAnswerBuf,"\r");
	ParallelCmdClear();
	return errInfo;

/*								 
	time = g_PCCmdData[0].DelayTime;
	error = LPK_Send_PCCmd(g_PCCmdData[0].DelayTime, (char *)g_PCCmdData[0].szCharBlock);

	error_slave = SendToSlave(g_PCCmdData[1].Adr - 10, (char *)g_PCCmdData[1].szCharBlock, m_PCCmdResponse1);
	

	if(time != 0)
	{
		K_Task_Wait(time);
	}
	error = LPK_ReceiveAnswer_PCCmd(m_PCCmdResponse0);
	if(error == errInfoLPK)
	{
		sprintf(sPtr,"%s\r", (char *)m_PCCmdResponse0);
		error = errInfo;
	}
	if(error_slave == errInfo)
	{
		sprintf(sPtr,"%s\r", (char *)m_PCCmdResponse1);
		error = errInfo;
	}

	return error;  */
}
unsigned short ParallelCmdClear(void)
{
	unsigned short i, a;
	if(g_PCDataClear)
	{
		for(i = 0; i < MAX_PCCMD; i++)
		{
			WDO();
			g_PCCmdData[i].Adr = FREE_ADR;
			g_PCCmdData[i].DelayTime = 0;
			for(a = 0; a < CHARMEM_PCCMD_LENGTH; a++)
			{
				WDO();
				g_PCCmdData[i].szCharBlock[a] = 0;
			}
		}
		g_PCDataClear = 0;
	}
	return errNone;
}
