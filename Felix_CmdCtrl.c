//****************************************************************************
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
/** include files **/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "DevError.h"
#include "main.h"
#include "DeviceConfig.h"
//#include "Shared/Stacker_Lifter_Allgemein/StacComm.h"
//#include "Shared/Stacker_Lifter_Allgemein/XYZComm.h"
//#include "Shared/Stacker_Lifter_Allgemein/StacError.h"
#include "Felix_CmdCtrl.h"
#include "Felix_Cmd.h"
#include "MyScanner.h"
#include "Scan.h"
#include "Felix_Hardware.h"
#include "LPK.h"
#include "MDriveCmd.h"
#include "Felix_Misc.h"
//#include "Felix_ASyncCtrl.h"
#include "Felix_Slave.h"
//#include "KFA_NT_Bus485.h"
//#include "KFA_CmdPipettor.h"
#include "Felix_HardwareTask.h"
#include "Drv_config.h"
#include "Felix_CmdParallel.h"

//****************************************************************************
// local definitions
//****************************************************************************

//****************************************************************************
// default settings
//****************************************************************************

//****************************************************************************
// external functions
//****************************************************************************
#ifdef _CF_DEBUG_POSERROR
void DebugPosErrReset(void);
#endif
//****************************************************************************
// external data
//****************************************************************************

//****************************************************************************
// internal functions
//****************************************************************************
void SendAnswerToHost(FILE *OutHandle);
void AnswerPrepare(unsigned short error);
void SendToHostAndResetUart(unsigned short error, FILE *InHandle, FILE *OutHandle);

//****************************************************************************
// public data
//****************************************************************************
char *g_pcCmdBuf, g_acAnswerBuf[ANSWER_BUF_LEN+1];
unsigned short g_uiErrorState= errNone;     //Speicherung der internen, externen Fehlernummer
//****************************************************************************
// private data
//****************************************************************************
char acCmdBuf[ASCBUF];
//****************************************************************************
// private functions
//****************************************************************************
void EmptyTheStream(FILE *handle)
{
	int zeichen;
	while(1)
	{
		WDO();
		zeichen = getc(handle);
		if(zeichen == EOF) break;
	}
}
/********************************************************************
 *  FUNCTION:    SendAnswerToHost
 *
 *  PARAMETERS:  none
 *
 *  DESCRIPTION: Senden der Antwort zum Host.
 *               Der Inhalt von "szResponseBuffer" wird zum Host gesendet.
 *
 *  RETURNS:    none
 ********************************************************************/
void SendAnswerToHost(FILE *OutHandle)
{
	//printf("%s",g_acAnswerBuf);fflush(stdout);		/* Senden der Antwort */
PRINT_ALLOC
	fprintf(OutHandle, "%s",g_acAnswerBuf);fflush(OutHandle);		/* Senden der Antwort */
PRINT_RELEASE
}

/********************************************************************
 *  FUNCTION:    AnswerPrepare
 *
 *  PARAMETERS:  unsigned char status
 *                   Es wird der Rückkehrcode von den 
 *                   ausgeführten Befehlsroutinen übergeben.
 *
 *  DESCRIPTION: Antwort zum Host vorbereiten.
 *
 *  RETURNS:     none
 ********************************************************************/
void AnswerPrepare(unsigned short error)
{char *pch;
	pch = g_acAnswerBuf;

	//SetVarBreakFlag(FALSE);
	if(error == errNone)
	{
		*pch++ = RDY;
		*pch = '\0';
		return;
	}
	if(error == errSync)
	{
		*pch++ = ACK;
		*pch = '\0';
		return;
	}
	if(error == errRxOverflow)
	{
		*pch++ = NACK;
		*pch = '\0';
		return;
	}
	
	if(error == errInfoLPK)
	{

		return;
	}
	if(error == errInfo) {return;}		// ZK ist bereits in g_acAnswerBuf
	sprintf(g_acAnswerBuf,"E%u\r", error);
}
/* FUNCTION: 	CmdCopy
   PARAMETERS:	Kommandobuffer, Anzahl, Filepointer
   DESCRIPTION:	Kopiert einen Kommandostring vom Eingabepuffer in den Kommandopuffer
   				Das Kommando muß vollständig sein, d.h. mit CR abschließen 
   RETURNS:		---	*/
/*void CmdCopy(FILE *stream)
{ 
    fgets(acCmdBuf, ASCBUF, stream);
    g_pcCmdBuf=acCmdBuf;
}*/
void SendToHostAndResetUart(unsigned short error, FILE *InHandle, FILE *OutHandle)
{
	AnswerPrepare(error);
	EmptyTheStream(InHandle);
	UART_ResetEvents(InHandle);
	UART_ResetDataBufferAndErrorFlag(InHandle);
	SendAnswerToHost(OutHandle);
}

//****************************************************************************
// public functions
//****************************************************************************
unsigned short UART_GetEvents(FILE *handle)
{
	if(handle == stdin)
	{
		return g_UART0_Events;
	}
	if(handle == COM1_RS485_U0C1_IN)
	{
		return g_UART1_Events;
	}
	if(handle == COM2_USB_U1C1_IN)
	{
		return g_UART2_Events;
	}
	if(handle == COM3_LAN_U2C0_IN)
	{
		return g_UART3_Events;
	}
	if(handle == COM4_BARC_U2C1_IN)
	{
		return g_UART4_Events;
	}
	if(handle == COM5_SLAVE_U1C0_IN)
	{
		return g_UART5_Events;
	}
	return g_UART2_Events;
}
void UART_ResetEvents(FILE *handle)
{
	if(handle == stdin)
	{
		g_UART0_Events=COM_EVENTS_RESET;
	}
	if(handle == COM1_RS485_U0C1_IN)
	{
		g_UART1_Events=COM_EVENTS_RESET;
	}
	if(handle == COM2_USB_U1C1_IN)
	{
		g_UART2_Events=COM_EVENTS_RESET;
	}
	if(handle == COM3_LAN_U2C0_IN)
	{
		g_UART3_Events=COM_EVENTS_RESET;
	}
	if(handle == COM4_BARC_U2C1_IN)
	{
		g_UART4_Events=COM_EVENTS_RESET;
	}
	if(handle == COM5_SLAVE_U1C0_IN)
	{
		g_UART5_Events=COM_EVENTS_RESET;
	}
}										

void UART_ResetDataBufferAndErrorFlag(FILE *handle)
{
	
	if(handle == stdin)
	{
		UART0_ResetDataBufferAndErrorFlag();
	}
	if(handle == COM1_RS485_U0C1_IN)
	{
		UART1_ResetDataBufferAndErrorFlag();
	}
	if(handle == COM2_USB_U1C1_IN)
	{
		UART2_ResetDataBufferAndErrorFlag();
	}
	if(handle == COM3_LAN_U2C0_IN)
	{
		UART3_ResetDataBufferAndErrorFlag();
	}
	if(handle == COM4_BARC_U2C1_IN)
	{
		UART4_ResetDataBufferAndErrorFlag();
	}
	if(handle == COM5_SLAVE_U1C0_IN)
	{
		UART5_ResetDataBufferAndErrorFlag();
	}
	UART_ResetEvents(handle);
}
/* FUNCTION: 	NextPara
   PARAMETERS:	keine
   DESCRIPTION:	Positioniert den Parameterzeiger zum nächsten Parameter
   RETURNS: 	void */
void NextPara(void)
{
  while(*g_pcCmdBuf) if(*g_pcCmdBuf==' ') break; else g_pcCmdBuf++;
  while(*g_pcCmdBuf) if(*g_pcCmdBuf!=' ') break; else g_pcCmdBuf++;
}

unsigned short CommandIdent(unsigned short ComEvents, FILE *handle)
{
	unsigned short error, dummy, State;
	short Adr;
	FILE *InHandle;
	FILE *OutHandle;
	char szHippieAnswer[64];

	InHandle = handle;
	OutHandle = stdout;
	if(handle == COM2_USB_U1C1_IN)
	{
		OutHandle = COM2_USB_U1C1_OUT;
	}
	if(handle == COM3_LAN_U2C0_IN)
	{
		OutHandle = COM3_LAN_U2C0_OUT;
	}
	error = errNone;
	error = GetStreamToScanner(handle);


	if(ComEvents & COM_EVENT_USER_BREAK)
	{
		putc(ACK, OutHandle);fflush(OutHandle);
		if(isSlavePresent())
		{
			SendBreakToSlave();
		}
		error = ERR_PROG_ABORT;
		#ifdef _CF_DEBUG_POSERROR
		DebugPosErrReset();
		#endif
	}
	if(ComEvents & COM_EVENT_SYNC)
	{
		if((error = ImsCmd_Sync()) == errNone)
		{
			SendToHostAndResetUart(errSync, InHandle, OutHandle);
			return errNone;
		}
	}
	if(ComEvents & COM_EVENT_OVERFLOW) {error = errRxOverflow;}

	if(error != errNone)
	{
		if(m_EStopCmdState)
		{
			m_EStopCmdState = ESTOPSTATE_RESET;
			//g_ClearEstopFlag();
			g_Cmd_ASync_ResetEstopMove();
		}
		SendToHostAndResetUart(error, InHandle, OutHandle);
		return error;
	}
	error = ERR_CMD_NOT_IMPLEMENTED;
	if(ComEvents & COM_EVENT_CR)
	{
		if((error = scanShort(&Adr)) == errNone)
		{
			putc(ACK, OutHandle);fflush(OutHandle);
//PRINT_ALLOC	fprintf(OutHandle, "ack %u %u",g_isGenerateEStopError(), ESTOP);fflush(OutHandle);PRINT_RELEASE
			//if(!isHostMode() && g_isGenerateEStopError() && ESTOP)
			if(!isHostMode() && g_isGenerateEStopError())
			{
				dummy = 40;
				do
				{
					WDO();
					K_Task_Wait(100);
					--dummy;
					if(g_MsgCheckEstopState(&State) != errNone)
					{
						SendToHostAndResetUart(error, InHandle, OutHandle);
						return error;
					}
				}while((State != ESTOP_OFF) && dummy);
				//}while((g_isGenerateEStopError() && ESTOP) && dummy);
				//if(!dummy) {return ERR_COVER_OPEN;}
//PRINT_ALLOC	fprintf(OutHandle, "%u %u %u",dummy, g_isGenerateEStopError(), ESTOP);fflush(OutHandle);PRINT_RELEASE
				//SetBreakCheckMOSFET();K_Task_Wait(1000);ClearBreakCheckMOSFET();
			}
			switch(Adr)
			{
				case ADR_HOST:	
								if(isEStopAnswerFromLPK() && (m_EStopCmdState != ESTOPSTATE_RESET))
								{
									m_EStopCmdState = ESTOPSTATE_RESET;
									g_Cmd_ASync_ResetEstopMove();
									error = ERR_CMD_NOT_PRACTICABLE;
								}
								else
								{error = CommandFelix(InHandle, OutHandle);}
								break;
				case ADR_LPK_2:
				case ADR_LPK_12:
				case ADR_LPK_22:
				case ADR_LPK_32:
				case ADR_LPK_42:
								if(isEStopAnswerFromLPK())
								{
								 	if((m_EStopCmdState == ESTOPSTATE_RESET))
									{
										m_EStopCmdState = ESTOPSTATE_WAIT;
									}
									error = EStopCommandHandling(&dummy, InHandle, OutHandle, &szHippieAnswer);
									if(error == errNone)
									{
										if(m_EStopCmdState == ESTOPSTATE_RESET)
										{
											if(LastLPKCommando("Pf"))
											{
												//ReloadLPKScannerBuffer(&Adr);
												error = CommandLPK(1, Adr, g_acAnswerBuf);
											}
											else
											{
												if(LastLPKCommando("gStatus"))
												{
													//ReloadLPKScannerBuffer(&Adr);
													error = CommandLPK(1, Adr, g_acAnswerBuf);
												}
											}

											//wenn last cmd PF dann pf hier wiederrholen	und warten bis felix fertig ist
										}
									}
									else
									{
										if(error == errInfo)
										{
											sprintf(g_acAnswerBuf, "%s\r", szHippieAnswer);
										}
										/* //08-2024 
										else
										{
											if(error == ERR_COVER_OPEN){error = LPK_ERR_CMD_NOT_PRACTICABLE;}
											else{error = LPK_ERR_EXT_DEVICE;}
										}*/
									}
								}
								else
								{
									if(!isEStopAnswerFromLPK() && (m_EStopCmdState != ESTOPSTATE_RESET))
									{
										m_EStopCmdState = ESTOPSTATE_RESET;
										g_Cmd_ASync_ResetEstopMove();
										error = LPK_ERR_CMD_NOT_PRACTICABLE;
									}
									else
									{
										//SaveLPKScannerBuffer(Adr);
										error = CommandLPK(0, Adr, g_acAnswerBuf);
										if(isEStopAnswerFromLPK() && (m_EStopCmdState == ESTOPSTATE_RESET))
										{
											m_EStopCmdState = ESTOPSTATE_WAIT;
										}
										else
										{
											if(g_acAnswerBuf[0] == 'E')
											{
												K_Task_Wait(200);
												m_EStopCmdState = ESTOPSTATE_RESET;
												//g_ClearEstopFlag();
												g_Cmd_ASync_ResetEstopMove();
											}
										}
										//hier oder in SetEStopAnswerFromLPK()
									 	//if((error == (unsigned short)ERR_SAFETY_APPLIANCE) && (m_EStopCmdState == ESTOPSTATE_RESET)){m_EStopCmdState = ESTOPSTATE_WAIT;}
									}
								}
								break;
				default:		
								error = CommandSlave(Adr, g_acAnswerBuf);
								break;
			}

		}
		else
		{
			if(m_EStopCmdState)
			{
				m_EStopCmdState = ESTOPSTATE_RESET;
				//g_ClearEstopFlag();
				g_Cmd_ASync_ResetEstopMove();
				//error = ERR_COVER_OPEN;
			}
		}
	}
	SendToHostAndResetUart(error, InHandle, OutHandle);
	return error;
}
