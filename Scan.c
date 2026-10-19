/**************************************************************************
* Datei : scan.c
*
* Aufgabe : lexikalische Analyse
*
* Programmiersprache : Tasking C/C++ V6
*
* Projekt : 
*
* Autor : H.Köberich
* Firma : (c) 2000  CyBio Instruments GmbH
*         Göschwitzer Str. 40
*         07745 Jena
*
* letzte Änderung : 14.02.2002
*
**************************************************************************/

/**************************************************************************
* standard functions
**************************************************************************/
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DevError.h"
#include <limits.h>

#include "main.h"
#include "DeviceConfig.h"

#include "scan.h"
//#include "Shared/Stacker_Lifter_Allgemein/StacError.h"

#include "MyScanner.h"
//#include "KFA_CmdCtrl.h"


/**************************************************************************
* defines, structures, functions
**************************************************************************/
/* ******************************************************************************
 *  FUNCTION:		scanLongValues
 *
 *  PARAMETERS:
 *
 *  DESCRIPTION:
 *
 *  RETURNS: 
 *
 ******************************************************************************** */
//unsigned short scanLong(long * value, FILE *handle)
unsigned short scanLong(long * value)
{
	static char Buffer[20];
	unsigned short error;

	switch(scanner(Buffer))
	{
		case EOS	:
//		case EOF	:
		case Scan_EOF:
		case '\n'	:	error = ERR_NOT_ENOUGH_PARAMETER;//errASC_ERR;
						break;
		case BYTE	:
		case UBYTE	:
		case ICONST	:
		case UICONST:
		case LCONST	:
		case ULCONST:	*value = atol(Buffer);
						error = errNone;
						break;
		default		:	error = ERR_WRONG_PARAMETER;
						break;
	}
	return error;
}
//unsigned short scanULong(unsigned long * value, FILE *handle)
unsigned short scanULong(unsigned long * value)
{
	static char Buffer[20];
	unsigned short error;
	unsigned short id;
	id = scanner(Buffer);
	//switch(scanner(Buffer))
	switch(id)
	{
		case EOS	:
//		case EOF	:
		case Scan_EOF:
		case '\n'	:	error = ERR_NOT_ENOUGH_PARAMETER;//errASC_ERR;
						break;
		case BYTE	:
		case UBYTE	:
		case UICONST:
		case ULCONST:	*value = atol(Buffer);
						error = errNone;
						break;
		default		:	error = ERR_WRONG_PARAMETER;
						break;
	}
	return error;
}

/* ******************************************************************************
 *  FUNCTION:		scanUnsignedInt
 *
 *  PARAMETERS:
 *
 *  DESCRIPTION:
 *
 *  RETURNS: 
 *
 ******************************************************************************** */
//near unsigned int scanUnsignedInt(unsigned int * value, FILE *handle)
//unsigned short scanUShort(unsigned short * value, FILE *handle)
unsigned short scanUShort(unsigned short * value)
{
	static char Buffer[20];
	unsigned short error;
	unsigned short ScannerIdent;
	ScannerIdent = scanner(Buffer);
	switch(ScannerIdent)
	{
		case EOS	:
//		case EOF	:
		case Scan_EOF:
		case '\n'	:	error = ERR_NOT_ENOUGH_PARAMETER;//errASC_ERR;
						break;
		case BYTE	:
		case UBYTE	:
		case ICONST :
		case UICONST:	*value = (unsigned short)atol(Buffer);
						error = errNone;
						break;
		default		:	error = ERR_WRONG_PARAMETER;
						break;
	}
	return error;
}

/* ******************************************************************************
 *  FUNCTION:		scanSignedInt
 *
 *  PARAMETERS:
 *
 *  DESCRIPTION:
 *
 *  RETURNS: 
 *
 ******************************************************************************** */
//unsigned short scanShort(short * value, FILE *handle)
unsigned short scanShort(short * value)
{
	static char Buffer[20];
	unsigned short error;

	unsigned short ScannerIdent;
	ScannerIdent = scanner(Buffer);
//	printf("Scan %u\r", ScannerIdent);fflush(stdout);		/* Senden der Antwort */
	switch(ScannerIdent)
	{
		case EOS	:
//		case EOF	:
		case Scan_EOF:
		case '\n'	:	error = ERR_NOT_ENOUGH_PARAMETER;//errASC_ERR;
						break;
		case BYTE	:
		case UBYTE	:
		case UICONST:
		case ICONST :	*value = atoi(Buffer);
						error = errNone;
						break;
		default		:	error = ERR_WRONG_PARAMETER;
						break;
	}
	return error;
}

/* ******************************************************************************
 *  FUNCTION:		scanUnsignedByte
 *
 *  PARAMETERS:
 *
 *  DESCRIPTION:
 *
 *  RETURNS: 
 *
 ******************************************************************************** */
//unsigned short scanUChar(unsigned char * value, FILE *handle)
unsigned short scanUChar(unsigned char * value)
{
	static char Buffer[20];
	unsigned short error;

	switch(scanner(Buffer))
	{
		case EOS	:
//		case EOF	:
		case Scan_EOF:
		case '\n'	:	error = ERR_NOT_ENOUGH_PARAMETER;//errASC_ERR;
						break;
		case UBYTE	:	*value = (unsigned char)atoi(Buffer);
						error = errNone;
						break;
		default		:	error = ERR_WRONG_PARAMETER;
						break;
	}
	return error;
}

unsigned short scanChar(char * value)
{
	static char Buffer[20];
	unsigned short error;

	switch(scanner(Buffer))
	{
		case EOS	:
//		case EOF	:
		case Scan_EOF:
		case '\n'	:	error = ERR_NOT_ENOUGH_PARAMETER;//errASC_ERR;
						break;
		case BYTE	:	*value = (char)atoi(Buffer);
						error = errNone;
						break;
		default		:	error = ERR_WRONG_PARAMETER;
						break;
	}
	return error;
}

/* ******************************************************************************
 *  FUNCTION:		scanFloatValues
 *
 *  PARAMETERS:
 *
 *  DESCRIPTION:
 *
 *  RETURNS: 
 *
 ******************************************************************************** */
//unsigned short scanFloat(float * value, FILE *handle)
unsigned short scanFloat(float * value)
{
	unsigned short error;
	static char Buffer[20];

	switch(scanner(Buffer))
	{
		case EOS	:
//		case EOF	:
		case Scan_EOF:
		case '\n'	:	error = ERR_NOT_ENOUGH_PARAMETER;//errASC_ERR;
						break;
		case UBYTE	:
		case BYTE	:
		case UICONST:
		case ICONST	:
		case ULCONST:
		case LCONST	:
		case FCONST	:
						*value = atof(Buffer);
						error = errNone;
						break;
		default		:	error = ERR_WRONG_PARAMETER;
						break;
	}
	return error;
}
unsigned short scanDouble(double * value)
{
	unsigned short error;
	static char Buffer[20];

	switch(scanner(Buffer))
	{
		case EOS	:
//		case EOF	:
		case Scan_EOF:
		case '\n'	:	error = ERR_NOT_ENOUGH_PARAMETER;//errASC_ERR;
						break;
		case UBYTE	:
		case BYTE	:
		case UICONST:
		case ICONST	:
		case ULCONST:
		case LCONST	:
		case FCONST	:
						*value = atof(Buffer);
						error = errNone;
						break;
		default		:	error = ERR_WRONG_PARAMETER;
						break;
	}
	return error;
}

/* ******************************************************************************
 *  FUNCTION:		scanOnOff
 *
 *  PARAMETERS:
 *
 *  DESCRIPTION:
 *
 *  RETURNS: 
 *
 ******************************************************************************** */
//unsigned short scanOnOff(unsigned short * value, FILE *handle)
unsigned short scanOnOff(unsigned short * value)
{
	unsigned short error = errNone;
	char Buffer[20];

	switch(scanner(Buffer))
	{
		case EOS	:
//		case EOF	:
		case Scan_EOF:
		case '\n'	:	error = ERR_NOT_ENOUGH_PARAMETER;//errASC_ERR;
						break;
		case IDENT	:	if(strcmp(Buffer, "on") == 0) { *value = ON; break; }
						if(strcmp(Buffer, "off") == 0) {*value = OFF; break; }
		default		:	error = ERR_WRONG_PARAMETER;
						break;
	}
	return error;
}

unsigned short SetUShort(unsigned short(*acc)(unsigned short temp))
{
	static unsigned short value;

	unsigned short error = scanUShort(&value);
	if(error == errNone)
	{
		error = acc(value);
	}
	return error;
}
unsigned short SetShort(unsigned short(*acc)(short temp))
{
	static short value;

	unsigned short error = scanShort(&value);
	if(error == errNone)
	{
		error = acc(value);
	}
	return error;
}

unsigned short SetFloat(unsigned short(*acc)(float temp))
{
	static float value;

	unsigned short error = scanFloat(&value);
	if(error == errNone)
	{
		error = acc(value);
	}
	return error;
}
