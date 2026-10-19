/**************************************************************************
* Datei : scan.h
*
* Programmiersprache : Tasking C/C++ V7
*
* Aufgabe : lexikalische Analyse
*
* Autor : H.Köberich
* Firma : (c) 2000  Cybio Instruments GmbH
*         Göschwitzer Str. 40
*         07745 Jena
*
* letzte Änderung : 14.02.2002
*
**************************************************************************/

#ifndef __SCANNER_H__
#define __SCANNER_H__

//****************************************************************************
// @Project Includes
//****************************************************************************
#include "Libs/standard/stdio.h"
/**************************************************************************
* defines
**************************************************************************/
//typedef enum ScannerReturnCode
//{
//#define Scan_errNone 0

#define Scan_EOF 1

//#define SCAN_WRONG_PARAMETER 2

//#define SCAN_NOT_ENOUGH_PARAMETER 3

#define NANR 4			// not a number

#define LINEFEED '\n'

//#define 	DOT			= '.',

#define HASH '#'

#define PERCENT '%'

#define STRING '"'

#define BYTE 0x0100

#define UBYTE 0x0200

#define ICONST 0x0400

#define UICONST 0x0800

#define LCONST 0x1000

#define ULCONST 0x2000

#define IDENT 0x4000

#define FCONST 0x8000

//	FCONST		= 0x4000,

//	IDENT		= 0x8000,

//	UINTCONST	= UBYTE | UICONST,
	
//	INTCONST	= UINTCONST | BYTE | ICONST,

//	LONGCONST	= INTCONST | LCONST | ULCONST,

//	FLOATCONST	= LONGCONST | FCONST

//}ScannerReturnCode;

#define	EOS		0	


/**************************************************************************
* globals
**************************************************************************/
extern char m_szScannerBuf[];
extern char *m_pScannerBuf;

/**************************************************************************
* imports
**************************************************************************/

/**************************************************************************
* Prototypen
**************************************************************************/
#ifdef __cplusplus
//! export for (M$) C++
extern "C" {
#endif
void ResetScannerBuffer(void);
unsigned short GetStreamToScanner(FILE *stream);
int UngetC_ScannerBuf(int value);
int GetC_ScannerBuf(void);
unsigned short scanner(char * Buffer);
//unsigned short scanner (char * Buffer, FILE * handle );
//unsigned int scanNumericValue(ScannerReturnCode what, void * value, FILE *handle);
unsigned short ZaehleZeichenImScanner(char zeichen);
char *GetScannerPtr(void);
void SaveScannerBuffer(void);
void SaveScannerBufferForReload(void);
void ReloadScannerBuffer(void);
//unsigned short ReloadLPKScannerBuffer(unsigned short *Adr);
//unsigned short SaveLPKScannerBuffer(unsigned short Adr);


#ifdef __cplusplus
//! export for (M$) C++
};
#endif
#endif	// __SCANNER_H__

