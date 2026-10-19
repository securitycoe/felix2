//****************************************************************************
// @Module        Universal Serial Interface Channel (USIC) U0C1
// @Filename      U0C1_RS485_COM1.H
// @Project       30_5015_Felix.dav
//----------------------------------------------------------------------------
// @Controller    Infineon XE167F-96F66
//
// @Compiler      Tasking Classic
//
// @Codegenerator 2.2
//
// @Description   This file contains all function prototypes and macros for 
//                the U0C1 module.
//
//----------------------------------------------------------------------------
// @Date          07.03.2016 10:02:17
//
//****************************************************************************

// USER CODE BEGIN (U0C1_Header,1)

// USER CODE END

/*
******************************************************************************
@Note

Com1 XS300/301 RS485 (IMs Motore)

******************************************************************************
*/


#ifndef _U0C1_RS485_COM1_H_
#define _U0C1_RS485_COM1_H_

//****************************************************************************
// @Project Includes
//****************************************************************************

// USER CODE BEGIN (U0C1_Header,2)

// USER CODE END


//****************************************************************************
// @Macros
//****************************************************************************

// USER CODE BEGIN (U0C1_Header,3)

// USER CODE END


//****************************************************************************
// @Defines
//****************************************************************************

// USER CODE BEGIN (U0C1_Header,4)

// USER CODE END


//****************************************************************************
// @Typedefs
//****************************************************************************

// USER CODE BEGIN (U0C1_Header,5)

// USER CODE END


//****************************************************************************
// @Imported Global Variables
//****************************************************************************

// USER CODE BEGIN (U0C1_Header,6)

// USER CODE END


//****************************************************************************
// @Global Variables
//****************************************************************************

// USER CODE BEGIN (U0C1_Header,7)
//extern unsigned short g_UART1_CmdCnt;
extern unsigned short g_UART1_SendByteCnt;
extern unsigned short g_UART1_Events;
// USER CODE END


//****************************************************************************
// @Prototypes Of Global Functions
//****************************************************************************

void U0C1_ASC_vInit(void);


// USER CODE BEGIN (U0C1_Header,8)

// USER CODE END


//****************************************************************************
// @Macro         U0C1_ASC_ubIsTxFIFOfull() 
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

#define U0C1_ASC_ubIsTxFIFOfull() (U0C1_TRBSRL & 0x1000)

//****************************************************************************
// @Interrupt Vectors
//****************************************************************************

#define U0C1_0INT    0x53         // SR0 of U0C1
#define U0C1_1INT    0x54         // SR1 of U0C1


// USER CODE BEGIN (U0C1_Header,9)
int UART1_GetChar(void);
int UART1_PutChar(char * base, size_t siz);
void UART1_ResetDataBufferAndErrorFlag(void);
int Uart1_kbHit(void);
void DisableOutRS485_BUS(void);
void EnableOutRS485_BUS(void);

// USER CODE END


#endif  // ifndef _U0C1_RS485_COM1_H_
