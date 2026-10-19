//****************************************************************************
// @Module        Universal Serial Interface Channel (USIC) U1C0
// @Filename      U1C0_SLAVE_COM5.H
// @Project       30_5015_Felix.dav
//----------------------------------------------------------------------------
// @Controller    Infineon XE167F-96F66
//
// @Compiler      Tasking Classic
//
// @Codegenerator 2.2
//
// @Description   This file contains all function prototypes and macros for 
//                the U1C0 module.
//
//----------------------------------------------------------------------------
// @Date          07.03.2016 10:02:18
//
//****************************************************************************

// USER CODE BEGIN (U1C0_Header,1)

// USER CODE END

/*
******************************************************************************
@Note

Com5 Slave Weiterleitung.

******************************************************************************
*/


#ifndef _U1C0_SLAVE_COM5_H_
#define _U1C0_SLAVE_COM5_H_

//****************************************************************************
// @Project Includes
//****************************************************************************

// USER CODE BEGIN (U1C0_Header,2)
#include "main.h"
#include <stdio.h>
// USER CODE END


//****************************************************************************
// @Macros
//****************************************************************************

// USER CODE BEGIN (U1C0_Header,3)

// USER CODE END


//****************************************************************************
// @Defines
//****************************************************************************

// USER CODE BEGIN (U1C0_Header,4)

// USER CODE END


//****************************************************************************
// @Typedefs
//****************************************************************************

// USER CODE BEGIN (U1C0_Header,5)

// USER CODE END


//****************************************************************************
// @Imported Global Variables
//****************************************************************************

// USER CODE BEGIN (U1C0_Header,6)

// USER CODE END


//****************************************************************************
// @Global Variables
//****************************************************************************

// USER CODE BEGIN (U1C0_Header,7)
extern unsigned short g_UART5_Events;
extern char UART5_InpBuffer[];
//extern unsigned short g_UART0_CmdCnt;
// USER CODE END


//****************************************************************************
// @Prototypes Of Global Functions
//****************************************************************************

void U1C0_ASC_vInit(void);


// USER CODE BEGIN (U1C0_Header,8)

// USER CODE END


//****************************************************************************
// @Macro         U1C0_ASC_ubIsTxFIFOfull() 
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

#define U1C0_ASC_ubIsTxFIFOfull() (U1C0_TRBSRL & 0x1000)

//****************************************************************************
// @Interrupt Vectors
//****************************************************************************

#define U1C0_0INT    0x56         // SR0 of U1C0


// USER CODE BEGIN (U1C0_Header,9)
int UART5_GetChar(void);
void UART5_ResetDataBufferAndErrorFlag(void);
int UART5_PutChar(char * base, size_t siz);
int Uart5_kbHit(void);

// USER CODE END


#endif  // ifndef _U1C0_SLAVE_COM5_H_
