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
#ifndef _FELIX_BARCODE_H_
#define _FELIX_BARCODE_H_
//****************************************************************************
// include files
//****************************************************************************

//****************************************************************************
// @Macros
//****************************************************************************

//****************************************************************************
// @Defines
//****************************************************************************
#define BC_BUFSIZE 50
#define BC_ERROR_CHAR 'E'
#define BC_INFO_CHAR 'I'


#define DEF_NO_READER 0
#define DEF_OPTICON 1
#define DEF_BCL8 2

#define CF_USED_BARCODE_READER DEF_BCL8

//****************************************************************************
// @Typedefs
//****************************************************************************
enum  BARC_DEVICE
{	BARCODE_READER_NO	// Reihenfolge entspricht der vereinbarten Kennung
	,BARCODE_READER_F30
	,BARCODE_READER_DS
	,BARCODE_READER_OPTICON
	,BARCODE_READER_BCL8
};


//****************************************************************************
// @Imported Global Variables
//****************************************************************************

//****************************************************************************
// @Global Variables
//****************************************************************************
extern char BCBuf[];
//****************************************************************************
// @External Prototypes
//****************************************************************************

//****************************************************************************
// Prototypen definitions
//****************************************************************************
extern unsigned short ASyncAnswerFromBarcode( char *pAnsw, unsigned short Timeout);
extern unsigned short AnswerFromBarcode( char *pAnsw, unsigned short Timeout);
extern unsigned short BarcodeReset(void);

extern void SetBarcodeConfig(unsigned short value);
extern unsigned short GetBarcodeConfig(void);
extern void SetBarcodeReader(unsigned short value);
extern unsigned short GetBarcodeReader(void);

//****************************************************************************
// @Prototypes Of Local Functions
//****************************************************************************

#endif
