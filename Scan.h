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

#ifndef __SCAN_H__
#define __SCAN_H__

/**************************************************************************
* defines
**************************************************************************/

/**************************************************************************
* globals
**************************************************************************/

/**************************************************************************
* imports
**************************************************************************/
/**************************************************************************
* Prototypen
**************************************************************************/

//unsigned short scanLong(long * value, FILE *handle);
//unsigned short scanULong(unsigned long * value, FILE *handle);
//unsigned short scanUShort(unsigned short * value, FILE *handle);
//unsigned short scanShort(short * value, FILE *handle);
//unsigned short scanUChar(unsigned char * value, FILE *handle);
//unsigned short scanFloat(float * value, FILE *handle);
//unsigned short scanOnOff(unsigned short * value, FILE *handle);
unsigned short scanLong(long * value);
unsigned short scanULong(unsigned long * value);
unsigned short scanUShort(unsigned short * value);
unsigned short scanShort(short * value);
unsigned short scanUChar(unsigned char * value);
unsigned short scanChar(char * value);
unsigned short scanFloat(float * value);
unsigned short scanDouble(double * value);
unsigned short scanOnOff(unsigned short * value);

unsigned short SetUShort(unsigned short(*acc)(unsigned short temp));
unsigned short SetShort(unsigned short(*acc)(short temp));
unsigned short SetFloat(unsigned short(*acc)(float temp));
/**************************************************************************
* globals
**************************************************************************/

/**************************************************************************
* imports
**************************************************************************/

/**************************************************************************
* Prototypen
**************************************************************************/



#endif