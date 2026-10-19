//****************************************************************************
// @Module        Kommandointerpreter
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
#ifndef _FELIX_CMDCTRL_H_
#define _FELIX_CMDCTRL_H_

//****************************************************************************
// include files
//****************************************************************************

//****************************************************************************
// @Macros
//****************************************************************************

//****************************************************************************
// @Defines
//****************************************************************************

//****************************************************************************
// @Typedefs
//****************************************************************************

//****************************************************************************
// @Imported Global Variables
//****************************************************************************

//****************************************************************************
// @Global Variables
//****************************************************************************
extern char *g_pcCmdBuf, g_acAnswerBuf[];
//****************************************************************************
// @External Prototypes
//****************************************************************************

//****************************************************************************
// Prototypen definitions
//****************************************************************************
void NextPara(void);
//unsigned short UART_GetCmdCnt(FILE *handle);
unsigned short UART_GetEvents(FILE *handle);
void UART_ResetEvents(FILE *handle);
//unsigned short UART_GetErrorFlag(void);
void UART_ResetDataBufferAndErrorFlag(FILE *handle);
//unsigned short UART_GetCmdFlag(void);
//void UART_ResetCmdFlag(void);

//void CmdCtrlExec(void);
/*void Done(unsigned short state);*/
//void CmdExec(void);
unsigned short CommandIdent(unsigned short ComEvents, FILE *handle);
//unsigned short CommandIdentRS485_BUS_(unsigned short events, FILE *handle);
//unsigned short Cmd_Slave(unsigned short Adresse, char *pCmd, char *answer);
void EmptyTheStream(FILE *handle);
//****************************************************************************
// @Prototypes Of Local Functions
//****************************************************************************

#endif
