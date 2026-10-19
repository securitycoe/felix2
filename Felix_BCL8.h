//****************************************************************************
// @Module        Schrittmotor-Controller
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
#ifndef _FELIXBCL8_H_
#define _FELIXBCL8_H_
//****************************************************************************
// include files
//****************************************************************************

//****************************************************************************
// @Macros
//****************************************************************************

//****************************************************************************
// @Defines
//****************************************************************************
#define BCL8_BARCODE_READ_TIME_OUT 2000 /* 2sec */
//****************************************************************************
// @Typedefs
//****************************************************************************


//****************************************************************************
// @Imported Global Variables
//****************************************************************************

//****************************************************************************
// @Global Variables
//****************************************************************************
#if(CF_USED_BARCODE_READER == DEF_BCL8)
//extern const unsigned char aszBCL8_Firmware[];
extern const char aszBCL8_STX_Firmware[];
extern const char chSTX;
#endif
//****************************************************************************
// @External Prototypes
//****************************************************************************

//****************************************************************************
// Prototypen definitions
//****************************************************************************
#if(CF_USED_BARCODE_READER == DEF_BCL8)
extern unsigned short BCL8_ReadBarcode(char *sPtr);
extern unsigned short BCL8_ReaderJustage(FILE *OutHandle);
extern unsigned short BCL8_Reset(void);
extern void BCL8_StartReaderJustage(void);
extern void BCL8_StopReaderJustage(void);
extern unsigned short BCL8_ReadReaderJustage(unsigned short Output);

#endif
//****************************************************************************
// @Prototypes Of Local Functions
//****************************************************************************

#endif
