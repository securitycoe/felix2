//****************************************************************************
// @Module        Universal Serial Interface Channel (USIC) U2C1
// @Filename      U2C1_RS232_COM4.H
// @Project       30_5015_Felix.dav
//----------------------------------------------------------------------------
// @Controller    Infineon XE167F-96F66
//
// @Compiler      Tasking Classic
//
// @Codegenerator 2.2
//
// @Description   This file contains all function prototypes and macros for 
//                the U2C1 module.
//
//----------------------------------------------------------------------------
// @Date          07.03.2016 10:02:22
//
//****************************************************************************

// USER CODE BEGIN (U2C1_Header,1)

// USER CODE END

/*
******************************************************************************
@Note

Com4 RS232 Barcode

******************************************************************************
*/


#ifndef _U2C1_RS232_COM4_H_
#define _U2C1_RS232_COM4_H_

//****************************************************************************
// @Project Includes
//****************************************************************************

// USER CODE BEGIN (U2C1_Header,2)
#include "main.h"
#include <stdio.h>
// USER CODE END


//****************************************************************************
// @Macros
//****************************************************************************

// USER CODE BEGIN (U2C1_Header,3)

// USER CODE END


//****************************************************************************
// @Defines
//****************************************************************************

// USER CODE BEGIN (U2C1_Header,4)

// USER CODE END


//****************************************************************************
// @Typedefs
//****************************************************************************

// USER CODE BEGIN (U2C1_Header,5)

// USER CODE END


//****************************************************************************
// @Imported Global Variables
//****************************************************************************

// USER CODE BEGIN (U2C1_Header,6)

// USER CODE END


//****************************************************************************
// @Global Variables
//****************************************************************************

// USER CODE BEGIN (U2C1_Header,7)
extern unsigned short g_UART4_Events;
extern unsigned char UART4_InpBuffer[];
//extern unsigned short g_UART0_CmdCnt;
// USER CODE END


//****************************************************************************
// @Prototypes Of Global Functions
//****************************************************************************

void U2C1_ASC_vInit(void);


// USER CODE BEGIN (U2C1_Header,8)

// USER CODE END


//****************************************************************************
// @Macro         U2C1_ASC_ubIsTxFIFOfull() 
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

#define U2C1_ASC_ubIsTxFIFOfull() (U2C1_TRBSRL & 0x1000)

//****************************************************************************
// @Interrupt Vectors
//****************************************************************************

#define U2C1_0INT    0x5F         // SR0 of U2C1


// USER CODE BEGIN (U2C1_Header,9)
int UART4_GetChar(void);
void UART4_ResetDataBufferAndErrorFlag(void);
int UART4_PutChar(char * base, size_t siz);
int Uart4_kbHit(void);
_bit getCTS_COM4_UART4(void);
void setRTS_COM4_UART4(unsigned short Value);

// USER CODE END


#endif  // ifndef _U2C1_RS232_COM4_H_
