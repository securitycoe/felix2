//****************************************************************************
// @Module        Universal Serial Interface Channel (USIC) U2C0
// @Filename      U2C0_LAN_COM3.H
// @Project       30_5015_Felix.dav
//----------------------------------------------------------------------------
// @Controller    Infineon XE167F-96F66
//
// @Compiler      Tasking Classic
//
// @Codegenerator 2.2
//
// @Description   This file contains all function prototypes and macros for 
//                the U2C0 module.
//
//----------------------------------------------------------------------------
// @Date          07.03.2016 10:02:21
//
//****************************************************************************

// USER CODE BEGIN (U2C0_Header,1)

// USER CODE END

/*
******************************************************************************
@Note

Com3 LAN (zumHost)

******************************************************************************
*/


#ifndef _U2C0_LAN_COM3_H_
#define _U2C0_LAN_COM3_H_

//****************************************************************************
// @Project Includes
//****************************************************************************

// USER CODE BEGIN (U2C0_Header,2)
#include "main.h"
#include <stdio.h>
// USER CODE END


//****************************************************************************
// @Macros
//****************************************************************************

// USER CODE BEGIN (U2C0_Header,3)

// USER CODE END


//****************************************************************************
// @Defines
//****************************************************************************

// USER CODE BEGIN (U2C0_Header,4)

// USER CODE END


//****************************************************************************
// @Typedefs
//****************************************************************************

// USER CODE BEGIN (U2C0_Header,5)

// USER CODE END


//****************************************************************************
// @Imported Global Variables
//****************************************************************************

// USER CODE BEGIN (U2C0_Header,6)

// USER CODE END


//****************************************************************************
// @Global Variables
//****************************************************************************

// USER CODE BEGIN (U2C0_Header,7)
extern unsigned short g_UART3_Events;
extern unsigned char UART3_InpBuffer[];
//extern unsigned short g_UART0_CmdCnt;
// USER CODE END


//****************************************************************************
// @Prototypes Of Global Functions
//****************************************************************************

void U2C0_ASC_vInit(void);


// USER CODE BEGIN (U2C0_Header,8)

// USER CODE END


//****************************************************************************
// @Macro         U2C0_ASC_ubIsTxFIFOfull() 
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

#define U2C0_ASC_ubIsTxFIFOfull() (U2C0_TRBSRL & 0x1000)

//****************************************************************************
// @Interrupt Vectors
//****************************************************************************

#define U2C0_0INT    0x5C         // SR0 of U2C0


// USER CODE BEGIN (U2C0_Header,9)
int UART3_GetChar(void);
void UART3_ResetDataBufferAndErrorFlag(void);
int UART3_PutChar(char * base, size_t siz);
int Uart3_kbHit(void);
_bit getCTS_COM3_UART3(void);
void setRTS_COM3_UART3(unsigned short Value);

// USER CODE END


#endif  // ifndef _U2C0_LAN_COM3_H_
