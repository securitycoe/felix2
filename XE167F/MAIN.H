//****************************************************************************
// @Module        Project Settings
// @Filename      MAIN.H
// @Project       30_5015_Felix.dav
//----------------------------------------------------------------------------
// @Controller    Infineon XE167F-96F66
//
// @Compiler      Tasking Classic
//
// @Codegenerator 2.2
//
// @Description   This file contains all function prototypes and macros for 
//                the MAIN module.
//
//----------------------------------------------------------------------------
// @Date          07.03.2016 10:02:14
//
//****************************************************************************

// USER CODE BEGIN (MAIN_Header,1)

// USER CODE END



#ifndef _MAIN_H_
#define _MAIN_H_

//****************************************************************************
// @Project Includes
//****************************************************************************

// USER CODE BEGIN (MAIN_Header,2)
#include "Cxconfig.h"
#include "Drv_config.h"
#include "DeviceConfig.h"
// USER CODE END


//****************************************************************************
// @Macros
//****************************************************************************

// USER CODE BEGIN (MAIN_Header,3)

// USER CODE END


//****************************************************************************
// @Defines
//****************************************************************************
#define TASKING_CLASSIC

// USER CODE BEGIN (MAIN_Header,4)
#ifndef FALSE
 #define FALSE 0
#endif

#ifndef TRUE
 #define TRUE  (!FALSE)
#endif

#ifndef	ASCBUF
 #define ASCBUF	 288
#endif

#ifndef	UARTBUF
 #define UARTBUF	 288//128
#endif

enum COM_EVENTS
{
	COM_EVENTS_RESET			= 0x0000
	,COM_EVENT_CR				= 0x0001
	,COM_EVENT_SYNC				= 0x0002
	,COM_EVENT_OVERFLOW			= 0x0004
	,COM_EVENT_BROADCAST		= 0x0008
	,COM_EVENT_USER_BREAK		= 0x0010
//	,COM_EVENT_SAFETYAPPLIANCE	= 0x0020
};

// USER CODE END


//****************************************************************************
// @Typedefs
//****************************************************************************

// USER CODE BEGIN (MAIN_Header,5)

// USER CODE END


//****************************************************************************
// @Imported Global Variables
//****************************************************************************

// USER CODE BEGIN (MAIN_Header,6)

// USER CODE END


//****************************************************************************
// @Global Variables
//****************************************************************************

// USER CODE BEGIN (MAIN_Header,7)
//extern unsigned short g_EnableEstopEvent;
// USER CODE END


//****************************************************************************
// @Prototypes Of Global Functions
//****************************************************************************


void MAIN_vUnlockProtecReg(void);

void MAIN_vLockProtecReg(void);

void MAIN_vChangeFreq(void);

// USER CODE BEGIN (MAIN_Header,8)

// USER CODE END


//****************************************************************************
// @Interrupt Vectors
//****************************************************************************

// USER CODE BEGIN (MAIN_Header,9)

// USER CODE END


//****************************************************************************
// @Project Includes
//****************************************************************************


#include  "XE16xREGS.H"
#include  "SCS.H"

#include  "IO.H"
#include  "GPT1.H"
#include  "GPT2.H"
#include  "CC2.H"
#include  "CCU63.H"
#include  "RTC.H"
#include  "INT.H"
#include  "WDT.H"
#include  "USIC0.H"
#include  "USIC1.H"
#include  "USIC2.H"

// USER CODE BEGIN (MAIN_Header,10)
#include "ASC0.H"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "undefsXE16xREGS.h"
#include <regxe167f.h>

#include "../DeviceConfig.h"
#include "../Felix_Misc.h"
#include "../Felix_Hardware.h"

#define U0C1_COM1_RE_485 		P8_OUT_P4
#define U0C1_COM1_DE_485 		P8_OUT_P5

#define U1C1_COM2_RTS2		P3_OUT_P3	// hight active
#define U1C1_COM2_CTS2		P3_IN_P0	// hight active

#define U2C0_COM3_RTS3		P3_OUT_P4	// hight active
#define U2C0_COM3_CTS3		P3_IN_P5	// hight active

#define U2C1_COM4_RTS4		P3_OUT_P2	// hight active
#define U2C1_COM4_CTS4		P3_IN_P1	// hight active

#define SPI_MTSR	P0_OUT_P0
#define SPI_MRST	P0_IN_P1
#define SPI_SCLK	P0_OUT_P2
#define FRAM_CS		P0_OUT_P3


// ########################## neu definitionen für KFA ##################

void main(void);
extern unsigned char g_bBreakFlag;
extern unsigned char g_ucTestFlag;
extern FILE *g_InHandle;
extern FILE *g_OutHandle;
extern unsigned short Bus485_Slave;
// USER CODE END


#endif  // ifndef _MAIN_H_
