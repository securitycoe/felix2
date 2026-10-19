//****************************************************************************
// @Module        MDrive RS485 Ctrl
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
//                for controlling 485 Bus.
//
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
//#include "Shared/standard/_doprint.h"

#include "DevCmds.h"
#include "DevError.h"
#include "30-5015_Felix/XE167F/main.h"
//#include "30-5015_Felix/XE167F/GPT1.h"
//#include "30-5015_Felix/XE167F/U0C1_RS485_COM1.h"
//#include "DeviceConfig.h"
//#include "MDrive_CmdCtrl.h"
#include "MDrive_BusCtrl.h"
#include "Felix_CmdCtrl.h"

#include "Drv_config.h"
//#include "Felix_Misc.h"
#include "Felix_DoorLifter.h"
//****************************************************************************
// @Macros
//****************************************************************************

//****************************************************************************
// local definitions
//****************************************************************************


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
extern char GetLPKBusAdresse(unsigned short Adresse);
//****************************************************************************
// external data
//****************************************************************************
extern unsigned char MotorSlot;
//****************************************************************************
// internal functions
//****************************************************************************

//****************************************************************************
// public data
//****************************************************************************
char m_IMSResponse[UARTBUF];
const char imsGetError[]= "PR ER";
const char imsGetMoveFlag[]= "PR MV";
const char strLF[]="LF";
const char strCR[]="CR";
//****************************************************************************
// private data
//****************************************************************************
int m_TxCharCnt;
#ifdef _CF_485_DEBUG_PRINT
unsigned long m_TestLPKTxCount = 0;
unsigned long m_TestLPKErrCount = 0;
#endif
//****************************************************************************
// private functions
//****************************************************************************
void SetErrorAxisHWForEver(const short ID, const unsigned short error);
void SetErrorAxis(const short ID, const unsigned short CyBioError, const unsigned short HWError);

void Reset_UART_Events_IMSResponse(const short ID);
unsigned replace_character(char* string, char from, char to);
void ResetIMSResponse(void);
char GetDeviceName(const short ID);

unsigned short newBus_LPK_General_Send_And_Receive(const short ID, const unsigned short AdresseCyBiCtrl, char *vaBuffer);
unsigned short newBus_IMS_SendWithReflection(const short ID, char *vaBuffer);
unsigned short newBus_IMS_GetError(const short ID, const unsigned short cntSend);
unsigned short IMS_LowLewelErrHndl(const short ID, const char *format, ...);
unsigned short newBus_LPK_SendWithoutReflection(char *vaBuffer);
#ifdef _CF_485_DEBUG_PRINT
void DbgPrintNotAllocSendChar(const short ID, int RxChar);
void DbgPrintNotAllocSendBuffer(const short ID, char *vaBuffer);
void DbgPrintNotAllocRxCharHex(const short ID, int RxChar);
unsigned short newBus_IMS_General_Send_And_Receive(const short ID, char *vaBuffer);
unsigned short newBus_IMS_ReceivedResponse(const short ID, char *vaBuffer);
#else
unsigned short newBus_IMS_General_Send_And_Receive(const short ID, char *vaBuffer);
unsigned short newBus_IMS_ReceivedResponse(const short ID);
#endif
//****************************************************************************
// public functions
//****************************************************************************


unsigned short newBus_LPK_SendWithoutReflection(char *vaBuffer)
{
	unsigned short error = errNone;

	m_TxCharCnt = fprintf(COM1_RS485_U0C1_OUT, vaBuffer);
	fflush(COM1_RS485_U0C1_OUT);
	return error;
}

unsigned short newBus_LPK_General_Send_And_Receive(const short ID, const unsigned short AdresseCyBiCtrl, char *vaBuffer)
{
	unsigned short error = errNone;
	short cntSend;
	short cntMax;

	LED_1 = 1;
#ifdef _CF_485_DEBUG_PRINT
++m_TestLPKTxCount;
#endif
	if(ID == LPK_MANUALLYMODE_INTERN_ID){cntMax = 1;}else{cntMax = 3;}
		for( cntSend = 0; cntSend < cntMax; cntSend++)
		{
			Reset_UART_Events_IMSResponse(ID);
			#ifdef _CF_485_DEBUG_PRINT
			if(g_TestFlag & TF_DBG_485){DbgPrintNotAllocSendBuffer(ID, vaBuffer);}
			#endif
			// Senden an LPK, hier muss alles gesendet werden, anschließend wird vom LPK die Reflexion gesendet.
			error = newBus_LPK_SendWithoutReflection(vaBuffer);
			if(ID == LPK_PARALLEL_CMD_ID)
			{
				// Parallel Kommond nur senden
				LED_1 = 0;
				//PRINT_RELEASE
  				return error;
			}
			else
			{
				error = newBus_LPK_ReceivedResponse(ID);
			}
			if(error == LPK_BUS_ERR_HEAD_NOT_CONNECTED)
			{
				K_Task_Wait(10);
			}
			else
			{
				if(error == ERR_485_DATATRANSFER)
				{
					K_Task_Wait(10);
					#ifdef _CF_485_DEBUG_PRINT
					if(g_TestFlag & (TF_DBG_485 | TF_DBG_485_ERR))
					{
						DbgPrintNotAllocSendBuffer(ID, vaBuffer);
						dbPrintNotAlloc(ID, "\rLPK RxCnt:%lu ErrCnt:%lu\r", m_TestLPKTxCount, ++m_TestLPKErrCount);
					}
					#endif
					sprintf(vaBuffer,"%c %s\n", GetLPKBusAdresse(AdresseCyBiCtrl), _GetLastAnswer);
					#ifdef _CF_485_DEBUG_PRINT
					if(g_TestFlag & (TF_DBG_485 | TF_DBG_485_ERR))
					{
						DbgPrintNotAllocSendBuffer(ID, vaBuffer);
					}
					#endif
				}
				else
				{
					if((error != LPK_BUS_ERR_HEAD_NOT_CONNECTED) && (error != ERR_485_DATATRANSFER))
					{
						break;
					}
				}
			}
		}//end for...
	LED_1 = 0;
//	PRINT_RELEASE
	return error;
}

#ifdef _CF_485_DEBUG_PRINT
unsigned short newBus_IMS_General_Send_And_Receive(const short ID, char *vaBuffer)
#else
unsigned short newBus_IMS_General_Send_And_Receive(const short ID, char *vaBuffer)
#endif
{
	unsigned short error;

	LED_1 = 1;
	Reset_UART_Events_IMSResponse(ID);
	#ifdef _CF_485_DEBUG_PRINT
		if(g_TestFlag & TF_DBG_485){DbgPrintNotAllocSendBuffer(ID, vaBuffer);}
	#endif
	// Senden an IMS, jedes Zeichen wird sofort reflektiert.
	if((error = newBus_IMS_SendWithReflection(ID, vaBuffer)) == errNone)
	{
		#ifdef _CF_485_DEBUG_PRINT
		error = newBus_IMS_ReceivedResponse(ID, vaBuffer);
		#else
		error = newBus_IMS_ReceivedResponse(ID);
		#endif
	}
	LED_1 = 0;
//	PRINT_RELEASE
	return error;
}

unsigned short Bus_Command_TxRx(const short ID, const unsigned short AdresseCyBiCtrl, const char *format, ...)
{
	unsigned short error = errNone;
	unsigned short cntSend = 0;
	char Adr;
	va_list ap;
	char vaBuffer[128];
	struct _iobuf	_strbuf;

	PRINT_ALLOC
//-------- Sendedaten vorbereiten -----------------------------------------
	va_start( ap, format );
	_strbuf._flag = _IOWR|_IOSTRG;
	_strbuf._base = vaBuffer;
	_strbuf._ptr  = vaBuffer;
	_strbuf._bufsiz= sizeof(vaBuffer);
	_strbuf._cnt  = INT_MAX;
	// LPK_ID ist -2, LPKINTERN_ID ist -3, OEM_CMD_DIRECT_ID ist -1
	// AxisID ist 1 bis so viele Achsen es gibt
	if(ID >= 0)
	{
		Adr = GetDeviceName(ID);
		if(Adr < 0)
		{
			PRINT_RELEASE
			return ERR_UNKNOWN_DRV;
		}
		fputc(Adr, &_strbuf);
		fputc(' ', &_strbuf);
	}
	_doprint( & _strbuf, format, ap );
	va_end( ap );
	if(ID == DOORLIFT_ID)
	{
		fputc('\r', &_strbuf);
	}
	else
	{
		fputc('\n', &_strbuf);
	}
	fputc('\0', &_strbuf);	/* terminate string with a NULL byte */
//-------------------------------------------------------------------------
	if((ID == LPK_ID) || (ID == LPKINTERN_ID) || (ID == LPK_PARALLEL_CMD_ID) || (ID == LPK_MANUALLYMODE_INTERN_ID))
	{
		error = newBus_LPK_General_Send_And_Receive(ID, AdresseCyBiCtrl, vaBuffer);
	}
	else
	{
		if(ID == DOORLIFT_ID)
		{
			if(!(g_EquipmentConfig.DeviceConfig & BIT_D_DoorLifter))
			{
				PRINT_RELEASE
				return ERR_UNKNOWN_DRV;
			}
		}
		for( cntSend = 0; cntSend < 3; cntSend++)
		{
			#ifdef _CF_485_DEBUG_PRINT
			error = newBus_IMS_General_Send_And_Receive(ID, vaBuffer);
			#else
			error = newBus_IMS_General_Send_And_Receive(ID, vaBuffer);
			#endif
			if((error != ERR_485_NO_ANSWER_FROM_DEV) && (error != ERR_485_BUS_GENERIC))
			{
				break;
			}
		}//end for...
	}
	PRINT_RELEASE
	if((error != errNone) && ((ID >= 0) || ID == DOORLIFT_ID))
	{
		if(error == ERR_OEM_HARDWARE)
		{
			
			K_Task_Wait(50);
			error = newBus_IMS_GetError(ID, cntSend);// Neu in 4.52.00
		}//end if(error == ERR_OEM_HARDWARE)....
		else
		{
			if(ID == DOORLIFT_ID)
			{
				SetDoorLift_ErrorAxisHW(OEM_GENERIC_ERROR+3);
				SetDoorLift_LastErrorAxisHWForEver(OEM_GENERIC_ERROR+3);
			}
			else {SetErrorAxis(ID, error, OEM_GENERIC_ERROR+3);}
		}
	}
	return error;
}

unsigned short newBus_IMS_SendWithReflection(const short ID, char *vaBuffer)
{
	unsigned short error = errNone;
	int RxChar;
	unsigned short Timeout;
	unsigned short i, a, cst;

	// das letze \n wird nicht reflektiert ????

	if(ID == DOORLIFT_ID)
	{
		replace_character(vaBuffer, '\r', '\0');
	}
	else
	{
		replace_character(vaBuffer, '\n', '\0');
	}

	m_TxCharCnt = strlen(vaBuffer);
	a = m_TxCharCnt;
	#ifdef _CF_485_DEBUG_PRINT
	if(g_TestFlag & TF_DBG_485)
	{
		if(!strstr(vaBuffer, imsGetMoveFlag)){dbPrintNotAlloc(ID, "EchoTxRx[");}
	}
	#endif
	for( i = 0; i < a; i++)
	{
		cst = 0;
		if(ID == DOORLIFT_ID)
		{
			fputc(vaBuffer[i],COM5_SLAVE_U1C0_OUT);
			fflush(COM5_SLAVE_U1C0_OUT);
		}
		else
		{
			fputc(vaBuffer[i],COM1_RS485_U0C1_OUT);
			fflush(COM1_RS485_U0C1_OUT);
		}
		#ifdef _CF_485_DEBUG_PRINT
		if(g_TestFlag & TF_DBG_485)
		{
			if(!strstr(vaBuffer, imsGetMoveFlag)){DbgPrintNotAllocSendChar(ID, vaBuffer[i]);}
		}
		#endif
		m_TxCharCnt--;
		Timeout = 60000; // !!! sind keine 3000ms, ist blöd programmiert, wieso auch immer
						// 30x = 600µs + 1ms = 1,6ms; 3000/30 = 100 -> 160ms
						// 1ms warten pro Zeichen ist wahrscheinlich zu langsam
		do
		{
			if(ID == DOORLIFT_ID)
			{
				RxChar = UART5_GetChar();
			}
			else
			{
				RxChar = UART1_GetChar();
			}

			switch(RxChar)
			{
				case EOF:
						if(--Timeout == 0)
						{
							error = ERR_485_NO_ANSWER_FROM_DEV;
						}
						else
						{
							//K_Task_Wait(1);
							WDO();
							ShortTimerIMS(20); // in µs !ACHTUNG BLOCKIERT hier das OS
							cst++;
							if(cst > 30)
							{
								// 600 µs
								cst = 0;
								K_Task_Wait(1);
							}
						}
						break;
				case '?':
						error = ERR_OEM_HARDWARE;
						Timeout = 0;
						#ifdef _CF_485_DEBUG_PRINT
						if(g_TestFlag & (TF_DBG_485 | TF_DBG_485_ERR)){dbPrintNotAlloc(ID, "%c\0",RxChar);}
						#endif
						break;
				default:
						if(RxChar == vaBuffer[i]){Timeout = 0;}
						#ifdef _CF_485_DEBUG_PRINT
						if(g_TestFlag & TF_DBG_485)
						{
							if(!strstr(vaBuffer, imsGetMoveFlag)){DbgPrintNotAllocSendChar(ID, RxChar);}
						}
						#endif
						break;

			}
		}while(Timeout);
		if(error != errNone)
		{
			//IMS_SyncNoAlloc();
			// das oben gelöschte \n senden
		  	K_Task_Wait(10);
			if(ID == DOORLIFT_ID)
			{
				fputc('\r',COM5_SLAVE_U1C0_OUT);fflush(COM5_SLAVE_U1C0_OUT);
			}
			else
			{
				fputc('\n',COM1_RS485_U0C1_OUT);fflush(COM1_RS485_U0C1_OUT);
			}
			K_Task_Wait(10);
			#ifdef _CF_485_DEBUG_PRINT
			if(g_TestFlag & TF_DBG_485)
			{
				if(!strstr(vaBuffer, imsGetMoveFlag)){dbPrintNotAlloc(ID, "]%s", strLF);}
			}
			#endif

			LED_1 = 0;
			return error;
		}
	}//end for(...
	// das oben gelöschte \n senden
	if(ID == DOORLIFT_ID)
	{
		fputc('\r',COM5_SLAVE_U1C0_OUT);fflush(COM5_SLAVE_U1C0_OUT);m_TxCharCnt++;
	}
	else
	{
		fputc('\n',COM1_RS485_U0C1_OUT);
		fflush(COM1_RS485_U0C1_OUT);
		m_TxCharCnt++;
	}
	#ifdef _CF_485_DEBUG_PRINT
	if(g_TestFlag & TF_DBG_485)
	{
		if(!strstr(vaBuffer, imsGetMoveFlag)){dbPrintNotAlloc(ID, "]%s", strLF);}
	}
	#endif
	return error;
}

unsigned short newBus_LPK_ReceivedResponse(const short ID)
{
	unsigned short error = ERR_485_BUS_GENERIC;
	unsigned short event;
	char * Response = m_IMSResponse;
	int RxChar;
	short notChangeTimeout = 0;
	unsigned short Timeout = 2000;
	#ifdef _CF_485_DEBUG_PRINT
	if(g_TestFlag & TF_DBG_485)
	{
		{dbPrintNotAlloc(ID, "\rRx<-[");}
	}
	#endif
	if(ID == LPK_MANUALLYMODE_INTERN_ID){Timeout = 600;}
	do
	{
		if((ID == LPK_ID) || (ID == LPKINTERN_ID))
		{
			event = K_Event_Wait(EVENT_USER_BREAK, 1, 0);
			if(event & EVENT_USER_BREAK) 
			{
				#ifdef _CF_485_DEBUG_PRINT
				if(g_TestFlag & (TF_DBG_485 | TF_DBG_485_ERR)){dbPrintNotAlloc(ID, "\r[* SL 0 ]");}
				#endif
				fprintf(COM1_RS485_U0C1_OUT, "* SL 0\n");
				fflush(COM1_RS485_U0C1_OUT);
				K_Task_Wait(10);
				#ifdef _CF_485_DEBUG_PRINT
				if(g_TestFlag & (TF_DBG_485 | TF_DBG_485_ERR)){dbPrintNotAlloc(ID, "\r[ESC]");}
				#endif
				fputc('\033',COM1_RS485_U0C1_OUT);
				fflush(COM1_RS485_U0C1_OUT);
				K_Event_Reset(MotorSlot, EVENT_USER_BREAK);
				error = ERR_PROG_ABORT;
				//error2 = ERR_PROG_ABORT;
				Timeout = 2000;
				notChangeTimeout = 1;
				//return error;
/*				hier  nach abbruch oder timeout/keine Antwort vom LPK verschwinden 
				wie ist die Fehlerauswertung, da es beim LPK nicht getError gibt sondern der Error aus der Antwort
				generiert wird*/
			}
		}
		else
		{
			K_Task_Wait(1);
		}
		RxChar = UART1_GetChar();
		switch(RxChar)
		{
			case EOF:
					//if(m_TxCharCnt > 0)
					//{
						if(--Timeout == 0)
						{
							error = LPK_BUS_ERR_HEAD_NOT_CONNECTED;
						}
					//}
					break;
			case '>':
					#ifdef _CF_485_DEBUG_PRINT
					if(g_TestFlag & TF_DBG_485){dbPrintNotAlloc(ID, "%c\0",RxChar);}
					#endif
					error = errNone;
					*Response++ = 0;
					Timeout = 0;
					break;
			default:
					if(m_TxCharCnt > 0)
					{
						m_TxCharCnt--;//Refelextion abzählen
					}
					else
					{
						if(!notChangeTimeout)
						{
							if(ID == LPK_MANUALLYMODE_INTERN_ID)
							{Timeout = 4000;}else{Timeout = 60000;}
							notChangeTimeout = 1;
						}
						if((RxChar != '\n') && (RxChar != '\r'))	// CR und LF ignorieren
						{
							if(!isprint(RxChar))
							{
								Timeout = 0;
								error = ERR_485_DATATRANSFER;
							}
							else
							{
								*Response++ = (char)RxChar;
							}
						}
					}
					/*if((ID == LPKINTERN_ID) || (ID == LPK_PARALLEL_CMD_ID)){Timeout = 1000;}
					else{
						if(m_TxCharCnt > 0)
						{
							//Timeout = 2000;
						}
						else
						{
							if(!notChangeTimeout){Timeout = 60000;notChangeTimeout = 1;}
						}
					}*/
					#ifdef _CF_485_DEBUG_PRINT
					if(g_TestFlag & (TF_DBG_485 | TF_DBG_485_ERR))
					{
						if(g_TestFlag & TF_DBG_485)
						{
							if(Timeout == 0){DbgPrintNotAllocRxCharHex(ID, RxChar);}
							else{DbgPrintNotAllocSendChar(ID, RxChar);}
						}
						else
						{
							if((Timeout == 0) && (g_TestFlag & TF_DBG_485_ERR))
							{
								dbPrintNotAlloc(ID, "\rRx<-[");DbgPrintNotAllocRxCharHex(ID, RxChar);dbPrintNotAlloc(ID, "]\0");
							}
						}
					}
					#endif
					break;
		}//end switch(RxChar....
	}while(Timeout);
	//if(error2 != errNone){error = error2;}
	#ifdef _CF_485_DEBUG_PRINT
	if(g_TestFlag & TF_DBG_485)
	{
		dbPrintNotAlloc(ID, "]\0");
	}
	if(g_TestFlag & (TF_DBG_485 | TF_DBG_485_ERR))
	{
		if(error){dbPrintNotAlloc(ID, "485Rx ID:%d E:%u\r", ID, error );}
	}
	#endif
	return error;
}

#ifdef _CF_485_DEBUG_PRINT
unsigned short newBus_IMS_ReceivedResponse(const short ID, char *vaBuffer)
#else
unsigned short newBus_IMS_ReceivedResponse(const short ID)
#endif
{
	unsigned short error = ERR_485_BUS_GENERIC;
	char * Response = m_IMSResponse;
	int RxChar;
	unsigned short Timeout = 1000;
	#ifdef _CF_485_DEBUG_PRINT
	if(g_TestFlag & TF_DBG_485)
	{
		if(!strstr(vaBuffer, imsGetMoveFlag)){dbPrintNotAlloc(ID, "\rRx<-[");}
	}
	#endif
	do
	{
		K_Task_Wait(1);
		if(ID == DOORLIFT_ID)
		{
			RxChar = UART5_GetChar();
		}
		else
		{
			RxChar = UART1_GetChar();
		}
		switch(RxChar)
		{
			case EOF:
					if(--Timeout == 0)
					{
						error = ERR_485_NO_ANSWER_FROM_DEV;
					}
					break;

			case '?':
					#ifdef _CF_485_DEBUG_PRINT
					if(g_TestFlag & (TF_DBG_485 | TF_DBG_485_ERR)){dbPrintNotAlloc(ID, "%c\0",RxChar);}
					#endif
					error = ERR_OEM_HARDWARE;
					*Response++ = 0;
					Timeout = 0;
					break;
			case '>':
					#ifdef _CF_485_DEBUG_PRINT
					if(g_TestFlag & TF_DBG_485)
					{
						if(strstr(vaBuffer, imsGetMoveFlag))
						{}
						else
						{
							dbPrintNotAlloc(ID, "%c\0",RxChar);
						}
					}
					#endif
					error = errNone;
					*Response++ = 0;
					Timeout = 0;
					break;
			default:
					if(m_TxCharCnt > 0)
					{
						m_TxCharCnt--;//Refelextion abzählen
					}
					else
					{		
						if((RxChar != '\n') && (RxChar != '\r'))	// CR und LF ignorieren
						{
							*Response++ = (char)RxChar;
						}
					}
					#ifdef _CF_485_DEBUG_PRINT
					if(g_TestFlag & TF_DBG_485)
					{
						if(strstr(vaBuffer, imsGetMoveFlag)){}
						else{DbgPrintNotAllocSendChar(ID, RxChar);}
					}
					#endif
					Timeout = 1000;
					break;
		}//end switch(RxChar....
	}while(Timeout);
	#ifdef _CF_485_DEBUG_PRINT
	if(g_TestFlag & TF_DBG_485)
	{
		if(strstr(vaBuffer, imsGetMoveFlag))
		{}
		else
		{
			dbPrintNotAlloc(ID, "]\0");
		}
	}
	if(g_TestFlag & (TF_DBG_485 | TF_DBG_485_ERR))
	{
		if(error){dbPrintNotAlloc(ID, "485Rx ID:%u E:%u\r", ID, error );}
	}
	#endif
	return error;
}

unsigned short newBus_IMS_GetError(const short ID, const unsigned short cntSend)
{
	unsigned short error;

	error = IMS_LowLewelErrHndl( ID, "%s", imsGetError);
	#ifdef _CF_485_DEBUG_PRINT
	if(g_TestFlag & (TF_DBG_485 | TF_DBG_485_ERR))
	{dbPrint("\rGetError adr: %c answ.: %u\r", GetDeviceName(ID), error);}
	#endif
	if(error == errNone)
	{
		error = atoi(m_IMSResponse);
		#ifdef _CF_485_DEBUG_PRINT
		if(g_TestFlag & (TF_DBG_485 | TF_DBG_485_ERR))
		{dbPrint("OEMReadError adr: %c error: %u\r", GetDeviceName(ID), error);}
		#endif
		if(error == 0)
		{
			error = ERR_485_DATATRANSFER;
			#ifdef _CF_485_DEBUG_PRINT
			if(g_TestFlag & (TF_DBG_485 | TF_DBG_485_ERR)){dbPrint("IL2 ID:%u E:%u\r", ID, error );}
			#endif
			if(ID == DOORLIFT_ID)
			{
				SetDoorLift_ErrorAxisHW(OEM_GENERIC_ERROR+1);
				SetDoorLift_LastErrorAxisHWForEver(OEM_GENERIC_ERROR+1);
			}
			else {SetErrorAxis(ID, error, OEM_GENERIC_ERROR+1);}
		}
		else
		{
			switch(error)
			{
				case 91:
							if(ID == DOORLIFT_ID){SetDoorLift_LastErrorAxisHWForEver(error);}
							else {SetErrorAxisHWForEver(ID, error);}
							// es kann vorkommen das gerade die Haube göffnet wird, wenn der Befehl losgeschickt wird
							error = errNone;
							break;
				case 85:	if(cntSend == 0)
							{
								if(ID == DOORLIFT_ID)
								{
									SetDoorLift_ErrorAxisHW(error);
									SetDoorLift_LastErrorAxisHWForEver(error);
								}
								else {SetErrorAxis(ID, ERR_STALL, error);}
								error = ERR_OEM_HARDWARE;
							}
							else
							{
								error = errNone;
							}
							break;
				case 86:
							if(ID == DOORLIFT_ID)
							{
								SetDoorLift_ErrorAxisHW(error);
								SetDoorLift_LastErrorAxisHWForEver(error);
								SetDoorLift_ErrorAxisHWStall(1);
							}
							else {SetErrorAxis(ID, ERR_STALL, error);}
							error = ERR_STALL;
							break;
				default:
							
							if(ID == DOORLIFT_ID)
							{
								SetDoorLift_ErrorAxisHW(error);
								SetDoorLift_LastErrorAxisHWForEver(error);
							}
							else {SetErrorAxis(ID, ERR_OEM_HARDWARE, error);}

							error = ERR_OEM_HARDWARE;
							#ifdef _CF_485_DEBUG_PRINT
							if(g_TestFlag & (TF_DBG_485 | TF_DBG_485_ERR)){dbPrint("IL3 ID:%u E:%u\r", ID, error );}
							#endif
							break;
			}//end switch...
		}
	}//end if(error == errNone)...
	else
	{
		error = ERR_485_DATATRANSFER;
		if(ID == DOORLIFT_ID)
		{
			SetDoorLift_ErrorAxisHW(OEM_GENERIC_ERROR+2);
			SetDoorLift_LastErrorAxisHWForEver(OEM_GENERIC_ERROR+2);
		}
		else {SetErrorAxis(ID, error, OEM_GENERIC_ERROR+2);}
		#ifdef _CF_485_DEBUG_PRINT
		if(g_TestFlag & (TF_DBG_485 | TF_DBG_485_ERR)){dbPrint("IL4 ID:%u E:%u\r", ID, error );}
		#endif
	}
	return error;
}

unsigned short IMS_LowLewelErrHndl(const short ID, const char *format, ...)
{
	unsigned short error;
	char Adr;
	va_list ap;
	char vaBuffer[32];
	struct _iobuf	_strbuf;

PRINT_ALLOC
	
	va_start( ap, format );
	_strbuf._flag = _IOWR|_IOSTRG;
	_strbuf._base = vaBuffer;
	_strbuf._ptr  = vaBuffer;
	_strbuf._bufsiz= sizeof(vaBuffer);
	_strbuf._cnt  = INT_MAX;
	if(ID >= 0)
	{
		Adr = GetDeviceName(ID);
		if(Adr < 0)
		{
			PRINT_RELEASE
			return ERR_UNKNOWN_DRV;
		}
		fputc(Adr, &_strbuf);
		fputc(' ', &_strbuf);
	}
	_doprint( & _strbuf, format, ap );
	va_end( ap );
	if(ID == DOORLIFT_ID)
	{
		fputc('\r', &_strbuf);
	}
	else
	{
		fputc('\n', &_strbuf);
	}
	fputc('\0', &_strbuf);	/* terminate string with a NULL byte */
	#ifdef _CF_485_DEBUG_PRINT
	error = newBus_IMS_General_Send_And_Receive(ID, vaBuffer);
	#else
	error = newBus_IMS_General_Send_And_Receive(ID, vaBuffer);
	#endif
PRINT_RELEASE
	return error;
}

//****************************************************************************
// private functions
//****************************************************************************
void Reset_UART_Events_IMSResponse(const short ID)
{

	if(ID == DOORLIFT_ID)
	{
		UART_ResetEvents(COM5_SLAVE_U1C0_IN);
		UART_ResetDataBufferAndErrorFlag(COM5_SLAVE_U1C0_IN);
	}
	else
	{
		UART_ResetEvents(COM1_RS485_U0C1_IN);
		UART_ResetDataBufferAndErrorFlag(COM1_RS485_U0C1_IN);
	}
	ResetIMSResponse();
}
/* Diese Funktion ersetzt in einer Zeichenkette ein Zeichen

  * durch ein anderes. Der Rückgabewert ist die Anzahl der

  * Ersetzungen */
unsigned replace_character(char* string, char from, char to)
{
  unsigned result = 0;
  if (!string) return 0;
  while (*string != '\0')
  {
    if (*string == from)
    {
      *string = to;
      result++;
    }
    string++;
  }
  return result;
}

void ResetIMSResponse(void)
{
	unsigned short a;
	for(a = 0; a < UARTBUF; a++) {WDO();m_IMSResponse[a] = 0;}

}

char GetDeviceName(const short ID)
{
/*
	unsigned char ucDeviceName = ID + '0';
	if(ucDeviceName > '9')			// 0 ... 9
	{
		ucDeviceName += 7;
		if(ucDeviceName > 'Z')		// A ... Z
		{
			ucDeviceName += 6;		// a ... z
		}
	}
	return ucDeviceName;
*/
	short i;
	if(ID == DOORLIFT_ID)
	{
		return 0;
	}
	else
	{
		for(i = 0; i < MAX_AXES; i++)
		{
			if(ID == g_FelixAxes[i].AxisID)
			{
				return g_FelixAxes[i].AxisAdr;
			}
		}
	}
	return -1;
}

void SetErrorAxis(const short ID, const unsigned short CyBioError, const unsigned short HWError)
{
	unsigned short i;
	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if(ID == g_FelixAxes[i].AxisID)
		{
			/*#ifdef _CF_485_DEBUG_PRINT
			if(g_TestFlag & (TF_DBG_485 | TF_DBG_485_ERR)){dbPrint("SAE i:%u ID:%u Ecy:%u Ehw:%u\r", i, ID, CyBioError, HWError);}
			#endif*/
			//g_SetAxes_ErrorAxisCyBio(i, CyBioError);
			g_SetAxes_ErrorAxisHW(i, HWError);
			g_SetAxes_LastErrorAxisHWForEver(i, HWError);
			//g_SetAxes_ErrorFlag(i, 1);
			//g_SetAxes_Running(i, STATE_ERROR);
			if(HWError == 86)
			{
				g_SetAxes_ErrorAxisHWStall(i, 1);
				//g_SetAxes_StallFlag(i, 1);
			}
		}
	}
}

void SetErrorAxisHWForEver(const short ID, const unsigned short error)
{
	unsigned short i;
	for(i = 0; i < MAX_AXES; i++)
	{
		WDO();
		if(ID == g_FelixAxes[i].AxisID)
		{
			#ifdef _CF_DEBUG_PRINT
			if(g_TestFlag & (TF_DBG_485 | TF_DBG_485_ERR)){dbPrint("SHWFE i:%u ID:%u E:%u\r", i, ID, error);}
			#endif
			g_SetAxes_LastErrorAxisHWForEver(i, error);
		}
	}
}

#ifdef _CF_485_DEBUG_PRINT
void DbgPrintNotAllocSendChar(const short ID, int RxChar)
{
	if(g_TestFlag & (TF_DBG_485 | TF_DBG_485_ERR))
	{
		if((RxChar != '\n') && (RxChar != '\r'))
		{
			dbPrintNotAlloc(ID, "%c\0",RxChar);
		}
		else
		{
			if(RxChar == '\n')
			{
				dbPrintNotAlloc(ID, "%s",strLF);
			}
			if(RxChar == '\r')
			{
				dbPrintNotAlloc(ID, "%s",strCR);
			}
		}
	}
}

void DbgPrintNotAllocSendBuffer(const short ID, char *vaBuffer)
{
	char bugBuffer[128];

	if(g_TestFlag & (TF_DBG_485 | TF_DBG_485_ERR))
	{
		if(strstr(vaBuffer, imsGetMoveFlag))
		{
			dbPrintNotAlloc(ID, ".");
		}
		else
		{
			strcpy(bugBuffer, vaBuffer);
			replace_character(bugBuffer, '\n', ' ');
			dbPrintNotAlloc(ID, "\rTx->[%s]",bugBuffer);
		}
	}
}

void DbgPrintNotAllocRxCharHex(const short ID, int RxChar)
{
	dbPrintNotAlloc(ID, "_0x%x_\0",RxChar);
}

#endif