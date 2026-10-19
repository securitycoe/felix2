//****************************************************************************
// @Module        Universal Serial Interface Channel (USIC) U1C1
// @Filename      U1C1_USB_COM2.H
// @Project       30_5015_Felix.dav
//----------------------------------------------------------------------------
// @Controller    Infineon XE167F-96F66
//
// @Compiler      Tasking Classic
//
// @Codegenerator 2.2
//
// @Description   This file contains all function prototypes and macros for 
//                the U1C1 module.
//
//----------------------------------------------------------------------------
// @Date          07.03.2016 10:02:19
//
//****************************************************************************

// USER CODE BEGIN (U1C1_Header,1)

// USER CODE END

/*
******************************************************************************
@Note

Com2 USB Schnittstelle (zum Host)

******************************************************************************
*/


#ifndef _U1C1_USB_COM2_H_
#define _U1C1_USB_COM2_H_

//****************************************************************************
// @Project Includes
//****************************************************************************

// USER CODE BEGIN (U1C1_Header,2)
#include "main.h"
#include <stdio.h>
// USER CODE END


//****************************************************************************
// @Macros
//****************************************************************************

// USER CODE BEGIN (U1C1_Header,3)

// USER CODE END


//****************************************************************************
// @Defines
//****************************************************************************

// USER CODE BEGIN (U1C1_Header,4)

// USER CODE END


//****************************************************************************
// @Typedefs
//****************************************************************************

// USER CODE BEGIN (U1C1_Header,5)

// USER CODE END


//****************************************************************************
// @Imported Global Variables
//****************************************************************************

// USER CODE BEGIN (U1C1_Header,6)

// USER CODE END


//****************************************************************************
// @Global Variables
//****************************************************************************

// USER CODE BEGIN (U1C1_Header,7)
//extern unsigned char g_UART2_ucCmdFlag;
//extern unsigned char g_UART2_ucErrorFlag;
extern unsigned short g_UART2_Events;
extern unsigned char UART2_InpBuffer[];
//extern unsigned short g_UART2_CmdCnt;
// USER CODE END


//****************************************************************************
// @Prototypes Of Global Functions
//****************************************************************************

void U1C1_ASC_vInit(void);


// USER CODE BEGIN (U1C1_Header,8)

// USER CODE END


//****************************************************************************
// @Macro         U1C1_ASC_ubIsTxFIFOfull() 
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

#define U1C1_ASC_ubIsTxFIFOfull() (U1C1_TRBSRL & 0x1000)

//****************************************************************************
// @Interrupt Vectors
//****************************************************************************

#define U1C1_0INT    0x59         // SR0 of U1C1


// USER CODE BEGIN (U1C1_Header,9)
int UART2_GetChar(void);
int UART2_PutChar(char * base, size_t siz);
void UART2_ResetDataBufferAndErrorFlag(void);
int Uart2_kbHit(void);
_bit getCTS_COM2_UART2(void);
void setRTS_COM2_UART2(unsigned short Value);

// USER CODE END


#endif  // ifndef _U1C1_USB_COM2_H_
