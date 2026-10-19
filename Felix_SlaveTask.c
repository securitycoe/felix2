//****************************************************************************
// @Module        Motortask
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
#include "DeviceConfig.h"
#include "Cxconfig.h"

#include "MDrive_CmdCtrl.h"
#include "Felix_Misc.h"
#include "Drv_config.h"
#include "Felix_Misc.h"
#include "Felix_Slave.h"
#include "Felix_SlaveTask.h"
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
// internal functions
//****************************************************************************

//****************************************************************************
// public data
//****************************************************************************
SLAVE_CommandBlock g_SlaveCmd;
//****************************************************************************
// private data
//****************************************************************************
SLAVE_CommandBlock * m_slave_ptr;
unsigned short SlavePCCmd_StartFlag = 0;
unsigned short SlavePCCmd_DelayTime = 0;
unsigned short SlaveAdr;
char szSlaveStr[64];

//****************************************************************************
// private functions
//****************************************************************************
void SlaveCtrlTask(void);
//****************************************************************************
// public functions
//****************************************************************************


//###########################################################################################
//###########################################################################################
//###########################################################################################
void SlaveTask(void)
{
	unsigned short event;
	unsigned short error = errNone;
	unsigned short send_error = errNone;
	unsigned char UserBreak = 0;
	unsigned short EventWaitTime = 5;

	while(1)
	{
		event = K_Event_Wait(EVENT_SLAVE_MBX | EVENT_USER_BREAK, EventWaitTime, 2);
		if(event & EVENT_USER_BREAK)
		{
			SlavePCCmd_StartFlag = 0;
			UserBreak = 1;
		}

		if(event & EVENT_SLAVE_MBX)
		{
			//LED_3 = 1;
			m_slave_ptr = K_Mesg_Get(MBX_SLAVE);	// auf Message warten
			WDO();
			switch(m_slave_ptr->eCommand)
			{
				case eSlaveSend		:	m_slave_ptr->Error = errNone;
										UserBreak = 0;
										SlavePCCmd_DelayTime = m_slave_ptr->DelayTime / EventWaitTime;
										strcpy(szSlaveStr, (char *)m_slave_ptr->Param.cBuffer);
										SlaveAdr = m_slave_ptr->Adr - 10;
										SlavePCCmd_StartFlag = 1;

										/*if(m_slave_ptr->DelayTime > 0)
										{
											K_Task_Wait(m_slave_ptr->DelayTime);
										}
										m_slave_ptr->Error = SendToSlave(m_slave_ptr->Adr - 10, (char *)m_slave_ptr->Param.cBuffer, (char *)m_slave_ptr->Param.cBuffer);
										*/
										break; 
				case eSlaveReceive	:	
										if(UserBreak)
										{
											UserBreak = 0;
											error = ERR_PUMP_PROG_ABORT;
										}
										else
										{
											error = AnswerFromSlave(SlaveAdr, (char *)m_slave_ptr->Param.cBuffer);
										}
										m_slave_ptr->Error = error;
										break; 								
				default			  	:	m_slave_ptr->Error = ERR_CMD_NOT_IMPLEMENTED;
										break; 
			}//end...switch(m_ptr->eCommand)
			K_Mesg_Ack_Sender();
			//K_Task_Wait(1);//K_Task_Coop_Sched();//K_Task_Wake(UserMainSlot);
//			LED_3 = 0;
		}//end... if(event & EVENT_MOTOR_MBX)
		//LED_3 = 0;
		if(SlavePCCmd_StartFlag)
		{
			WDO();
			if(SlavePCCmd_DelayTime == 0)
			{
				SlavePCCmd_StartFlag = 0;
				//IMS_LPKSend((char *)m_ptr->Argument.cBuffer);
				SendToSlavePCCmd(SlaveAdr, szSlaveStr);
//				send_error = SendToSlave(SlaveAdr, szSlaveStr);
				
			}
			else
			{
				--SlavePCCmd_DelayTime;
			}
		}
	}//end...while(1)
}

//###############################################################

unsigned short g_Slave_Send(unsigned short DelayTime, unsigned short  Adr, char *sPtr)
{
	unsigned short error;

	g_SlaveCmd.Error = errNone;
	g_SlaveCmd.eCommand = eSlaveSend;
	g_SlaveCmd.DelayTime = DelayTime;
	g_SlaveCmd.Adr = Adr;
	strcpy((char *)g_SlaveCmd.Param.cBuffer, sPtr);

	error = K_Mesg_Send_Wait(MBX_SLAVE, 0, &g_SlaveCmd);
	if(error != K_OK)
	{
		return ERR_SYSTEM;
	}
	error = g_SlaveCmd.Error;
	return error;
}

unsigned short g_Slave_Answ(char *sPtr)
{
	unsigned short error;

	g_SlaveCmd.Error = errNone;
	g_SlaveCmd.eCommand = eSlaveReceive;
	error = K_Mesg_Send_Wait(MBX_SLAVE, 0, &g_SlaveCmd);
	if(error != K_OK)
	{
		return ERR_SYSTEM;
	}
	error = g_SlaveCmd.Error;

	if(error == errInfo)
	{
		//sprintf(g_acAnswerBuf,"%s\r", (char *)Msg.Argument.cBuffer);
		//sprintf(sPtr,"%s\r", (char *)Msg.Argument.cBuffer);
		strcpy(sPtr, (char *)g_SlaveCmd.Param.cBuffer);
		return errInfo;
	}
	return error;
}
