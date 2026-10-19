//****************************************************************************
// @Module        Universal Serial Interface Channel (USIC) U0C0
// @Filename      ASC0.H
// @Project       30_5015_Felix.dav
//----------------------------------------------------------------------------
// @Controller    Infineon XE167F-96F66
//
// @Compiler      Tasking Classic
//
// @Codegenerator 2.2
//
// @Description   This file contains all function prototypes and macros for 
//                the U0C0 module.
//
//----------------------------------------------------------------------------
// @Date          07.03.2016 10:02:16
//
//****************************************************************************

// USER CODE BEGIN (U0C0_Header,1)

// USER CODE END

/*
******************************************************************************
@Note

Master Host und Programmierschnittstelle

******************************************************************************
*/


#ifndef _ASC0_H_
#define _ASC0_H_

//****************************************************************************
// @Project Includes
//****************************************************************************

// USER CODE BEGIN (U0C0_Header,2)
#include "main.h"
#include <stdio.h>
// USER CODE END


//****************************************************************************
// @Macros
//****************************************************************************

// USER CODE BEGIN (U0C0_Header,3)

// USER CODE END


//****************************************************************************
// @Defines
//****************************************************************************

// USER CODE BEGIN (U0C0_Header,4)

// USER CODE END


//****************************************************************************
// @Typedefs
//****************************************************************************

// USER CODE BEGIN (U0C0_Header,5)

// USER CODE END


//****************************************************************************
// @Imported Global Variables
//****************************************************************************

// USER CODE BEGIN (U0C0_Header,6)
//extern unsigned char g_UART0_ucCmdFlag;
//extern unsigned char g_UART0_ucErrorFlag;
extern unsigned char g_bBreakFlag;
extern unsigned short g_UART0_Events;
// USER CODE END


//****************************************************************************
// @Global Variables
//****************************************************************************

// USER CODE BEGIN (U0C0_Header,7)
extern unsigned short g_UART0_Events;
extern unsigned char UART0_InpBuffer[];
//extern unsigned short g_UART0_CmdCnt;
// USER CODE END


//****************************************************************************
// @Prototypes Of Global Functions
//****************************************************************************

void ASC0_vInit(void);


// USER CODE BEGIN (U0C0_Header,8)

// USER CODE END


//****************************************************************************
// @Macro         U0C0_ASC_ubIsTxFIFOfull() 
//
//----------------------------------------------------------------------------
// @Description   This macro is used to get the status whether the transmit 
//                buffer is full or not.
//
//----------------------------------------------------------------------------
// @Returnvalue   1 if transmit buffer is full, else 0
//
//----------------------------------------------------------------------------
// @Parameters    None
//
//----------------------------------------------------------------------------
// @Date          07.03.2016
//
//****************************************************************************

#define U0C0_ASC_ubIsTxFIFOfull() (U0C0_TRBSRL & 0x1000)

//****************************************************************************
// @Interrupt Vectors
//****************************************************************************

#define U0C0_0INT    0x50         // SR0 of U0C0


// USER CODE BEGIN (U0C0_Header,9)
int UART0_GetChar(void);
void UART0_ResetDataBufferAndErrorFlag(void);
int UART0_PutChar(char * base, size_t siz);
int Uart0_kbHit(void);
// USER CODE END


#endif  // ifndef _ASC0_H_
