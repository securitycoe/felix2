//****************************************************************************
// @Module        Felix FRAM
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

//****************************************************************************
// include files
//****************************************************************************
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <ctype.h>
#include "DevError.h"
#include "30-5015_Felix/XE167F/main.h"
#include "Cxconfig.h"
#include "DeviceConfig.h"
//#include "Drv_config.h"
#include "Felix_FRAM.h"
#include "FRAM_SPI_V1.H"
#include "CRC16.h"
#include "Felix_Misc.h"
#include "Felix_DoorLifter.h"
//****************************************************************************
// @Macros
//****************************************************************************

//****************************************************************************
// local definitions
//****************************************************************************

//****************************************************************************
// default settings
//****************************************************************************

//****************************************************************************
// external functions
//****************************************************************************

//****************************************************************************
// external data
//****************************************************************************
//****************************************************************************
// public data
//****************************************************************************

//****************************************************************************
// private data
//****************************************************************************
	static char FramTranslate[256]={
	'.','.','.','.','.','.','.','.',	'.','.','.','.','.','.','.','.',
	'.','.','.','.','.','.','.','.',	'.','.','.','.','.','.','.','.',
	' ','!','\"','#','$','%','&','\'',	'(',')','*','+',',','-','.','/',
	'0','1','2','3','4','5','6','7',	'8','9',':',';','<','=','>','?',
	'@','A','B','C','D','E','F','G',	'H','I','J','K','L','M','N','O',
	'P','Q','R','S','T','U','V','W',	'X','Y','Z','[','\\',']','^','_',
	'\'','a','b','c','d','e','f','g',	'h','i','j','k','l','m','n','o',
	'p','q','r','s','t','u','v','w',	'x','y','z','{','|','}','~','.',
	'.','.','.','.','.','.','.','.',	'.','.','.','.','.','.','.','.',
	'.','.','.','.','.','.','.','.',	'.','.','.','.','.','.','.','.',
	'.','.','.','.','.','.','.','.',	'.','.','.','.','.','.','.','.',
	'.','.','.','.','.','.','.','.',	'.','.','.','.','.','.','.','.',
	'.','.','.','.','.','.','.','.',	'.','.','.','.','.','.','.','.',
	'.','.','.','.','.','.','.','.',	'.','.','.','.','.','.','.','.',
	'.','.','.','.','.','.','.','.',	'.','.','.','.','.','.','.','.',
	'.','.','.','.','.','.','.','.',	'.','.','.','.','.','.','.','.'

	};

//****************************************************************************
// private functions
//****************************************************************************
unsigned int MyCalcCRC16(const unsigned char * src, const unsigned int len);
void FRAMClearAll(void);


/* FUNCTION: 	FRAMClearAll
   PARAMETERS:	---
   DESCRIPTION:	Löschen des gesamten FRAM's (0x00)
   RETURNS: 	--- */
void FRAMClearAll(void)
{
	unsigned short i;
	unsigned char pattern = 0x00;
FRAM_ALLOC
	for(i=0; i < FRAM_SIZE; i++)
	{
		WDO();
		WriteEEPROM(i, 1, &pattern);
	} 
FRAM_RELEASE
}

unsigned int MyCalcCRC16(const unsigned char * src, const unsigned int len)
{
	#ifdef _CF_DEBUG_PRINT
	if(g_TestFlag & TF_DBG_FRAM){dbPrint("CRC16 src:\"%s\"len:%u\r", src, len);}
	#endif
	return CalcCRC16( src, len);
}
//****************************************************************************
// public functions
//****************************************************************************
/*unsigned short FRAMSaveAxisOffsetV2(const udtOffset *offset)
{ 
  unsigned short crc;
  //OFFSET_FRAM tmp;
  struct FRAM *ptr;
  unsigned int len, len2;
  len =  sizeof(*offset);
   len2 =  sizeof(ptr->OffsetFRAM.Offset);
printf("offW len:%u len2:%u\r", len, len2);fflush(stdout);
  //WriteEEPROM(offsetof(struct FRAM, OffsetFRAM.Offset), sizeof(*offset),(unsigned char*) offset);
  WriteEEPROM(offsetof(struct FRAM, OffsetFRAM.Offset), sizeof(ptr->OffsetFRAM.Offset),(unsigned char*) offset);
  
  crc = CalcCRC16((unsigned char*)offset, len);
  //crc = CalcCRC16((unsigned char*)offset, sizeof(*offset));
  WriteEEPROM(offsetof(struct FRAM, OffsetFRAM.CRC), sizeof(ptr->OffsetFRAM.CRC), (unsigned char*) &crc);
  return errNone;
}
unsigned short FRAMReadAxisOffsetV2(udtOffset *offset)
{
  unsigned short error = errNone;
  unsigned int crc;
  //OFFSET_FRAM tmp;
  struct FRAM *ptr;
  unsigned int len, len2;
	len =  sizeof(*offset);
	len2 =  sizeof(ptr->OffsetFRAM.Offset);
printf("offR len:%u len2:%u\r", len, len2);fflush(stdout);
  ReadEEPROM(offsetof(struct FRAM, OffsetFRAM.Offset), sizeof(ptr->OffsetFRAM.Offset),(unsigned char*)offset);
  //ReadEEPROM(offsetof(struct FRAM, OffsetFRAM.CRC), sizeof(tmp.CRC),(unsigned char*)&crc);
  ReadEEPROM(offsetof(struct FRAM, OffsetFRAM.CRC), sizeof(ptr->OffsetFRAM.CRC),(unsigned char*)&crc);
  if(CalcCRC16((unsigned char*)offset,sizeof(*offset)) != crc) error = errCheckSum;
  return error;
}
*/
unsigned short FRAMInit(void)
{  	unsigned short hPtr; // dummy
   	unsigned short error = errNone;
	unsigned short i;
	char Buffer[3] = "";
	Buffer[0] = '\0';
	FRAMClearAll();
	FRAMSetStructVersion(0, 0);
	FRAMSaveDeviceConfig(&g_DeviceConfig);
	FRAMSetDevNumber(&Buffer[0]);
	for(i = 0;i < DATA_BLOCK_COUNT;i++)
	{
		FRAMSaveUserData(i, &Buffer[0]);
	}
//	DefaultOffsetGenerateFromFixedData();
	FRAMSaveAxisOffset(g_AxisOffsetDefault);
	FRAMSaveAxisMinMaxWay(g_AxisMinMaxWay);
	FRAMSaveSensorConfig(&g_SensorConfig);
	FRAMSavePortConfig(&g_PortConfig);
	FRAMSaveEquipmentConfig(&g_EquipmentConfig);
	FRAMSaveEstopCounter(&g_EstopCounter);
	FRAMSaveEstopWait(&g_EstopWait);
	FRAMSaveAxisConfigStallExeption(g_ConfigStallExeption);
	FRAMSaveAxisCounterStallExeption(g_CounterStallExeption);
	FRAMSaveSearchAdapterRelativeDeltaWay(&g_SearchAdapterRelativeDeltaWay);
	FRAMSaveDoorLiftConfig(&g_DoorLiftConfig);
//	DefaultCurrentGenerateFromFixedData();
//	FRAMSaveCurrent(AxisCurrrentDefault);

	for(i = 0;i < MAX_APPLICATION_SPD;i++)
	{
		FRAMSaveApplicationSpdAxis( i, g_ApplicationSpd[i].AppAxisSpd);
	}


	return FRAMReadStructVersion(&hPtr);
}

unsigned short FRAMSaveUserData(const unsigned short DataBlock, const char *ptrChar)
{ 
	unsigned short error = ERR_WRONG_PARAMETER;
	unsigned short strSize;
	unsigned short crc;
	//DATA_FRAM tmp;
	strSize = strlen(ptrChar);
	#ifdef _CF_DEBUG_PRINT
	if(g_TestFlag & TF_DBG_FRAM){dbPrint("wUserData Block:%u Size:%u\r",DataBlock, strSize);}
	#endif
	//if(strSize <= (sizeof(tmp.Data[DataBlock].szCharBlock)-1))
FRAM_ALLOC
	if(strSize <= (CHARMEM_BLOCK_LENGTH+1-1))
	{
		WriteEEPROM(offsetof(struct FRAM, DataFRAM.Data[DataBlock].szCharBlock), strlen(ptrChar)+1,(unsigned char*) ptrChar);  // Ende ZK (Null) mit speichern!
		if(strSize == 0) strSize = 1;
		crc = MyCalcCRC16((unsigned char*)ptrChar,strSize);
		WriteEEPROM(offsetof(struct FRAM, DataFRAM.Data[DataBlock].CRC), sizeof(crc), (unsigned char*)&crc);
		error = errNone;
	}
	//free(ptr);
FRAM_RELEASE
	return error;  
}
unsigned short FRAMReadUserData(const unsigned short DataBlock, char *ptrChar)
{
	unsigned short error = errNone;
	char *sPtr;
	unsigned int crc, siz;
	//DATA_FRAM tmp;

FRAM_ALLOC
	sPtr = ptrChar;
	//ReadEEPROM(offsetof(struct FRAM, DataFRAM.Data[DataBlock].szCharBlock), sizeof(tmp.Data[DataBlock].szCharBlock),(unsigned char*)ptrChar);
	ReadEEPROM(offsetof(struct FRAM, DataFRAM.Data[DataBlock].szCharBlock), CHARMEM_BLOCK_LENGTH+1,(unsigned char*)ptrChar);
	//sPtr = ptrChar;
	//ReadEEPROM(offsetof(struct FRAM, DataFRAM.Data[DataBlock].CRC), sizeof(tmp.Data[DataBlock].CRC),(unsigned char*)&crc);
	ReadEEPROM(offsetof(struct FRAM, DataFRAM.Data[DataBlock].CRC), sizeof(unsigned short),(unsigned char*)&crc);
	siz = strlen(sPtr);
	#ifdef _CF_DEBUG_PRINT
	if(g_TestFlag & TF_DBG_FRAM){dbPrint("rUserData Block:%u Size:%u\r",DataBlock, siz);}
	#endif
	if(siz == 0) siz = 1;

	if(MyCalcCRC16((unsigned char*)ptrChar, siz) != crc) error = errCheckSum;
FRAM_RELEASE
	return error;	    
}
/*#######################
void ReadEEPROM(uword address, uword numBytes, ubyte *dst)
{	EEPROM_BUF_TYPE	fram;
	ubyte TxCount;

	fram.cmd = READ;
	fram.addr_h = (ubyte) (address >> 8);
	fram.addr_l = (ubyte) (address);

	FRAM_CS= ENABLE;
	#ifdef FRAM25040
		if(fram.addr_h > 0) fram.cmd |=	0x08;
		fram.addr_h= fram.addr_l;	// Kommando und Adresse wird in einem Rutsch gesendet, deshalb Umsortierung
		TxCount= 2;
	#else
		TxCount= 3;
	#endif
	Tx_Rx_SPI(TxCount, &fram.cmd, numBytes, dst);
	FRAM_CS= DISABLE;
}
#######################*/


unsigned short FRAMEraseUserData(const unsigned short DataBlock)
{ 
	char Buffer[3] = "";
	Buffer[0] = '\0';
	return FRAMSaveUserData(DataBlock, &Buffer[0]);
}


unsigned short FRAMSaveAxisOffset(const udtOffset *offset)
{ 
	unsigned short crc;
	//struct FRAM *ptr = (struct FRAM*) calloc(1, sizeof(struct FRAM));
	struct FRAM *ptr;
	//unsigned int len;
	//if(ptr == NULL) return ERR_MEMORY_ALLOCATE;
	//len =  sizeof(ptr->OffsetFRAM.Offset);
	#ifdef _CF_DEBUG_PRINT
	if(g_TestFlag & TF_DBG_FRAM){dbPrint("wOffset Size:%u\r",sizeof(ptr->OffsetFRAM.Offset));}
	#endif
FRAM_ALLOC
	WriteEEPROM(offsetof(struct FRAM, OffsetFRAM.Offset), sizeof(ptr->OffsetFRAM.Offset),(unsigned char*) offset);
	crc = MyCalcCRC16((unsigned char*)offset, sizeof(ptr->OffsetFRAM.Offset));
	//crc = MyCalcCRC16((unsigned char*)offset, sizeof(*offset));
	WriteEEPROM(offsetof(struct FRAM, OffsetFRAM.CRC), sizeof(ptr->OffsetFRAM.CRC), (unsigned char*) &crc);
	//free(ptr);
FRAM_RELEASE
	return errNone;
}
unsigned short FRAMReadAxisOffset(udtOffset *offset)
{
	unsigned short error = errNone;
	unsigned int crc;
	//struct FRAM *ptr = (struct FRAM*) calloc(1, sizeof(struct FRAM));
	struct FRAM *ptr;
	//if(ptr == NULL) return ERR_MEMORY_ALLOCATE;
	#ifdef _CF_DEBUG_PRINT
	if(g_TestFlag & TF_DBG_FRAM){dbPrint("rOffset Size:%u\r",sizeof(ptr->OffsetFRAM.Offset));}
	#endif
FRAM_ALLOC
	//ReadEEPROM(offsetof(struct FRAM, OffsetFRAM.Offset), sizeof(ptr->OffsetFRAM.Offset),(unsigned char*)&AxisOffset);
	ReadEEPROM(offsetof(struct FRAM, OffsetFRAM.Offset), sizeof(ptr->OffsetFRAM.Offset),(unsigned char*)offset);
	ReadEEPROM(offsetof(struct FRAM, OffsetFRAM.CRC), sizeof(ptr->OffsetFRAM.CRC),(unsigned char*)&crc);
	//if(MyCalcCRC16((unsigned char*)offset,sizeof(*offset)) != crc) error = errCheckSum;
	if(MyCalcCRC16((unsigned char*)offset,sizeof(ptr->OffsetFRAM.Offset)) != crc) error = errCheckSum;
	//free(ptr);
FRAM_RELEASE
	return error;
}

unsigned short FRAMSaveAxisMinMaxWay(const udtMinMaxWay *minmax)
{ 
	unsigned short crc;
	//struct FRAM *ptr = (struct FRAM*) calloc(1, sizeof(struct FRAM));
	struct FRAM *ptr;
	//unsigned int len;
	//if(ptr == NULL) return ERR_MEMORY_ALLOCATE;
	//len =  sizeof(ptr->OffsetFRAM.Offset);
	#ifdef _CF_DEBUG_PRINT
	if(g_TestFlag & TF_DBG_FRAM){dbPrint("wMinMax Size:%u\r",sizeof(ptr->MinMaxWayFRAM.MinMaxWay));}
	#endif
FRAM_ALLOC
	WriteEEPROM(offsetof(struct FRAM, MinMaxWayFRAM.MinMaxWay), sizeof(ptr->MinMaxWayFRAM.MinMaxWay),(unsigned char*) minmax);
	crc = MyCalcCRC16((unsigned char*)minmax, sizeof(ptr->MinMaxWayFRAM.MinMaxWay));
	//crc = MyCalcCRC16((unsigned char*)offset, sizeof(*offset));
	WriteEEPROM(offsetof(struct FRAM, MinMaxWayFRAM.CRC), sizeof(ptr->MinMaxWayFRAM.CRC), (unsigned char*) &crc);
	//free(ptr);
FRAM_RELEASE
	return errNone;
}
unsigned short FRAMReadAxisMinMaxWay(udtMinMaxWay *minmax)
{
	unsigned short error = errNone;
	unsigned int crc;
	//struct FRAM *ptr = (struct FRAM*) calloc(1, sizeof(struct FRAM));
	struct FRAM *ptr;
	//if(ptr == NULL) return ERR_MEMORY_ALLOCATE;
	#ifdef _CF_DEBUG_PRINT
	if(g_TestFlag & TF_DBG_FRAM){dbPrint("rMinMax Size:%u\r",sizeof(ptr->MinMaxWayFRAM.MinMaxWay));}
	#endif
FRAM_ALLOC
	//ReadEEPROM(offsetof(struct FRAM, OffsetFRAM.Offset), sizeof(ptr->OffsetFRAM.Offset),(unsigned char*)&AxisOffset);
	ReadEEPROM(offsetof(struct FRAM, MinMaxWayFRAM.MinMaxWay), sizeof(ptr->MinMaxWayFRAM.MinMaxWay),(unsigned char*)minmax);
	ReadEEPROM(offsetof(struct FRAM, MinMaxWayFRAM.CRC), sizeof(ptr->MinMaxWayFRAM.CRC),(unsigned char*)&crc);
	//if(MyCalcCRC16((unsigned char*)offset,sizeof(*offset)) != crc) error = errCheckSum;
	if(MyCalcCRC16((unsigned char*)minmax,sizeof(ptr->MinMaxWayFRAM.MinMaxWay)) != crc) error = errCheckSum;
	//free(ptr);
FRAM_RELEASE
	return error;
}

/* FUNCTION: 	FRAMSetStructVersion - Serviceroutine
   PARAMETERS:	---
   DESCRIPTION:	Setzen der Strukturversionsnummer mit CRC
   RETURNS: 	--- */
void FRAMSetStructVersion(unsigned short DowngradeHelper, unsigned short Version)
{
	unsigned short vers;
	unsigned short crc;
	//struct FRAM * ptr = (struct FRAM *) calloc(1, sizeof(struct FRAM));
	struct FRAM *ptr;
FRAM_ALLOC
	if(DowngradeHelper)
	{
		vers = Version;
	}
	else
	{
		vers = FRAM_STRUCT_VERSION;
	}
	#ifdef _CF_DEBUG_PRINT
	if(g_TestFlag & TF_DBG_FRAM){dbPrint("wFRAMVer Size:%u\r",sizeof(ptr->StructVersionNo.StructVers));}
	#endif
	WriteEEPROM(offsetof(struct FRAM, StructVersionNo.StructVers), sizeof(ptr->StructVersionNo.StructVers),(unsigned char*) &vers);
	//crc = MyCalcCRC16((unsigned char*)&vers, sizeof(vers));
	crc = MyCalcCRC16((unsigned char*)&vers, sizeof(ptr->StructVersionNo.StructVers));
	WriteEEPROM(offsetof(struct FRAM, StructVersionNo.CRC), sizeof(ptr->StructVersionNo.CRC), (unsigned char*) &crc);
FRAM_RELEASE
	//free(ptr);
}

/* FUNCTION: 	FRAMReadStructVersion - Serviceroutine
   PARAMETERS:	Pointer zum Speichern der gelesenen Strukturversionsnummer
   DESCRIPTION:	Lesen der Strukturversionsnummer
   RETURNS: 	CB_READY / IERR_EEPROM_MEMORY (Aktion fehlgeschlagen) */
unsigned short FRAMReadStructVersion(unsigned short *sptr)
{
	unsigned short error;
	unsigned short crc;
	//struct FRAM * ptr = (struct FRAM *) calloc(1, sizeof(struct FRAM));
	struct FRAM * ptr;
FRAM_ALLOC
	error = errNone;
	#ifdef _CF_DEBUG_PRINT
	if(g_TestFlag & TF_DBG_FRAM){dbPrint("rFRAMVer Size:%u\r",sizeof(ptr->StructVersionNo.StructVers));}
	#endif
	ReadEEPROM(offsetof(struct FRAM, StructVersionNo.StructVers), sizeof(ptr->StructVersionNo.StructVers),(unsigned char*)sptr);
	ReadEEPROM(offsetof(struct FRAM, StructVersionNo.CRC), sizeof(ptr->StructVersionNo.CRC),(unsigned char*)&crc);
	if(MyCalcCRC16((unsigned char*)sptr,sizeof(ptr->StructVersionNo.StructVers)) != crc) error = errCheckSum;
	//free(ptr);
FRAM_RELEASE
	return error;  
}

unsigned short FRAMSetDevNumber(char *sptr)
{
	unsigned short error;
	unsigned short siz, crc;
	DEV_NUMBER_FRAM tmp;
	//struct FRAM* ptr = (struct FRAM *) calloc(1, sizeof(struct FRAM));
FRAM_ALLOC
	siz = strlen(sptr);
	error = ERR_WRONG_PARAMETER;
	#ifdef _CF_DEBUG_PRINT
	if(g_TestFlag & TF_DBG_FRAM){dbPrint("wDevNo Size:%u\r",siz);}
	#endif
	if(siz < (sizeof(tmp.DevNo)-1))
	{ 
		WriteEEPROM(offsetof(struct FRAM, DevNumber.DevNo), strlen(sptr)+1,(unsigned char*) sptr);  // Ende ZK (Null) mit speichern!
		//WriteEEPROM(offsetof(struct FRAM, DevNumber.DevNo), strlen(sptr),(unsigned char*) sptr);
		if(siz == 0) siz = 1;
		crc = MyCalcCRC16((unsigned char*)sptr,siz);
		WriteEEPROM(offsetof(struct FRAM, DevNumber.CRC), sizeof(crc), (unsigned char*)&crc);
		error = errNone;
	}
	//free(ptr);
FRAM_RELEASE
	return error;  
}

/* FUNCTION: 	FRAMReadDevNumber - Serviceroutine
   PARAMETERS:	Pointer auf Speicher für die zu speichernden Kopfnummer
   DESCRIPTION:	Lesen der Gerätenummer mit CRC-Check (nur über die Gerätenummer)
   RETURNS: 	CB_READY / IERR_EEPROM_MEMORY */
unsigned short FRAMReadDevNumber(char *sptr)
{
	unsigned short error = errNone;
	char *sPtr;
	unsigned int crc, crcCalc, siz;
	DEV_NUMBER_FRAM tmp;
	//struct FRAM * ptr = (struct FRAM *) calloc(1, sizeof(struct FRAM));
FRAM_ALLOC
	sPtr = sptr;
	ReadEEPROM(offsetof(struct FRAM, DevNumber.DevNo), sizeof(tmp.DevNo),(unsigned char*)sptr);
	ReadEEPROM(offsetof(struct FRAM, DevNumber.CRC), sizeof(tmp.CRC),(unsigned char*)&crc);
	siz = strlen(sPtr);
	#ifdef _CF_DEBUG_PRINT
	if(g_TestFlag & TF_DBG_FRAM){dbPrint("rDevNo Size:%u\r",siz);}
	#endif
	if(siz == 0) siz = 1;
	crcCalc = MyCalcCRC16((unsigned char*)sptr, siz);
//	if(MyCalcCRC16((unsigned char*)sptr, strlen(sPtr)) != crc) error = errCheckSum;
	if(crcCalc != crc) error = errCheckSum;
	//free(ptr);
FRAM_RELEASE
	return error;	    
}


unsigned short FRAMSaveApplicationSpdAxis(const unsigned short Index, const udtApplicationAxisSpd *AppAxisSpd)
{ 
	unsigned short crc;
	struct FRAM *ptr;
//	unsigned short testsiz, testoff;
FRAM_ALLOC
	WriteEEPROM(offsetof(struct FRAM, ApplicationSpdFRAM.ApplicationSpd[Index].AppAxisSpd), sizeof(ptr->ApplicationSpdFRAM.ApplicationSpd[Index].AppAxisSpd),(unsigned char*) AppAxisSpd);
	crc = MyCalcCRC16((unsigned char*)AppAxisSpd, sizeof(ptr->ApplicationSpdFRAM.ApplicationSpd[Index].AppAxisSpd));
	//crc = MyCalcCRC16((unsigned char*)offset, sizeof(*offset));
	WriteEEPROM(offsetof(struct FRAM, ApplicationSpdFRAM.ApplicationSpd[Index].CRC), sizeof(ptr->ApplicationSpdFRAM.ApplicationSpd[Index].CRC), (unsigned char*) &crc);
	//free(ptr);
FRAM_RELEASE

/*PRINT_ALLOC
testsiz = sizeof(ptr->ApplicationSpdFRAM.ApplicationSpd[Index].AppAxisSpd);
testoff = offsetof(struct FRAM, ApplicationSpdFRAM.ApplicationSpd[Index].AppAxisSpd);
	fprintf(stdout, "ID:%u offs:%u siz:%u crc:%u\r", Index, testoff, testsiz, crc);fflush(stdout);
PRINT_RELEASE
*/
	return errNone;
}
unsigned short FRAMReadApplicationSpdAxis(const unsigned short Index, udtApplicationAxisSpd *AppAxisSpd)
{
	unsigned short error = errNone;
	unsigned int crc;
	struct FRAM *ptr;
//	unsigned short testsiz, testoff, mycrc;

FRAM_ALLOC
	//ReadEEPROM(offsetof(struct FRAM, OffsetFRAM.Offset), sizeof(ptr->OffsetFRAM.Offset),(unsigned char*)&AxisOffset);
	ReadEEPROM(offsetof(struct FRAM, ApplicationSpdFRAM.ApplicationSpd[Index].AppAxisSpd), sizeof(ptr->ApplicationSpdFRAM.ApplicationSpd[Index].AppAxisSpd),(unsigned char*)AppAxisSpd);
	ReadEEPROM(offsetof(struct FRAM, ApplicationSpdFRAM.ApplicationSpd[Index].CRC), sizeof(ptr->ApplicationSpdFRAM.ApplicationSpd[Index].CRC),(unsigned char*)&crc);
	//if(MyCalcCRC16((unsigned char*)offset,sizeof(*offset)) != crc) error = errCheckSum;
	if(MyCalcCRC16((unsigned char*)AppAxisSpd, sizeof(ptr->ApplicationSpdFRAM.ApplicationSpd[Index].AppAxisSpd)) != crc) error = errCheckSum;
	//free(ptr);
FRAM_RELEASE
/*PRINT_ALLOC
mycrc = MyCalcCRC16((unsigned char*)AppAxisSpd, sizeof(ptr->ApplicationSpdFRAM.ApplicationSpd[Index].AppAxisSpd));
testsiz = sizeof(ptr->ApplicationSpdFRAM.ApplicationSpd[Index].AppAxisSpd);
testoff = offsetof(struct FRAM, ApplicationSpdFRAM.ApplicationSpd[Index].AppAxisSpd);
	fprintf(stdout, "ID:%u offs:%u siz:%u crc:%u mycrc:%u\r", Index, testoff, testsiz, crc, mycrc);fflush(stdout);
PRINT_RELEASE*/

	return error;
}

/*
//##############################################################
unsigned short xytFRAMSaveUserData(const unsigned short DataBlock, const char *ptrChar)
{ 
	unsigned short error = ERR_WRONG_PARAMETER;
	unsigned short strSize;
	unsigned short crc;
	//DATA_FRAM tmp;
	strSize = strlen(ptrChar);
FRAM_ALLOC
	if(strSize <= (CHARMEM_BLOCK_LENGTH+1-1))
	{
		WriteEEPROM(offsetof(struct FRAM, DataFRAM.Data[DataBlock].szCharBlock), strlen(ptrChar)+1,(unsigned char*) ptrChar);  // Ende ZK (Null) mit speichern!
		if(strSize == 0) strSize = 1;
		crc = MyCalcCRC16((unsigned char*)ptrChar,strSize);
		WriteEEPROM(offsetof(struct FRAM, DataFRAM.Data[DataBlock].CRC), sizeof(crc), (unsigned char*)&crc);
		error = errNone;
	}
	//free(ptr);
FRAM_RELEASE
	return error;  
}
unsigned short xytFRAMReadUserData(const unsigned short DataBlock, char *ptrChar)
{
	unsigned short error = errNone;
	char *sPtr;
	unsigned int crc, siz;
	//DATA_FRAM tmp;

FRAM_ALLOC
	sPtr = ptrChar;
	//ReadEEPROM(offsetof(struct FRAM, DataFRAM.Data[DataBlock].szCharBlock), sizeof(tmp.Data[DataBlock].szCharBlock),(unsigned char*)ptrChar);
	ReadEEPROM(offsetof(struct FRAM, DataFRAM.Data[DataBlock].szCharBlock), CHARMEM_BLOCK_LENGTH+1,(unsigned char*)ptrChar);
	//sPtr = ptrChar;
	//ReadEEPROM(offsetof(struct FRAM, DataFRAM.Data[DataBlock].CRC), sizeof(tmp.Data[DataBlock].CRC),(unsigned char*)&crc);
	ReadEEPROM(offsetof(struct FRAM, DataFRAM.Data[DataBlock].CRC), sizeof(unsigned short),(unsigned char*)&crc);
	siz = strlen(sPtr);
	#ifdef _CF_DEBUG_PRINT
	if(g_TestFlag & TF_DBG_FRAM){dbPrint("rUserData Block:%u Size:%u\r",DataBlock, siz);}
	#endif
	if(siz == 0) siz = 1;

	if(MyCalcCRC16((unsigned char*)ptrChar, siz) != crc) error = errCheckSum;
FRAM_RELEASE
	return error;	    
}
//###########################################################
*/
unsigned short FRAMDump(unsigned short StartAdress, unsigned short EndAdress, FILE *OutHandle)
{
	unsigned short b;
	unsigned char Byte;
	char ReadCharArray[8];
	unsigned short address;
	//unsigned short framsize = FRAM_SIZE;


	//for(address = 0; address < framsize; address+=sizeof(ReadCharArray))
	StartAdress = ((unsigned short)(StartAdress/sizeof(ReadCharArray))) * sizeof(ReadCharArray);
	for(address = StartAdress; address < EndAdress; address+=sizeof(ReadCharArray))
	{
		PRINT_ALLOC
		fprintf(OutHandle, "\r%04x  %04u  ", address, address);fflush(OutHandle);
		PRINT_RELEASE
		FRAM_ALLOC
		ReadEEPROM(address, sizeof(ReadCharArray), (unsigned char*)ReadCharArray);
		FRAM_RELEASE
 		//if(ReadEEPROM(address, sizeof(ReadCharArray), (unsigned char*)ReadCharArray))
		{
			
			for( b = 0;b < sizeof(ReadCharArray); b++)
			{
				Byte = ReadCharArray[b];
				PRINT_ALLOC
				fprintf(OutHandle, "  %02x", (unsigned short)Byte);fflush(OutHandle);
				PRINT_RELEASE
			}
			PRINT_ALLOC
			fprintf(OutHandle, "  ");fflush(OutHandle);
			PRINT_RELEASE

			for( b = 0;b < sizeof(ReadCharArray); b++)
			{
				PRINT_ALLOC
				fprintf(OutHandle, "%c", FramTranslate[ReadCharArray[b]]);fflush(OutHandle);
				PRINT_RELEASE

				
			}
		}
		/*else
		{
			PRINT_ALLOC
			fprintf(OutHandle, "error read\r");fflush(OutHandle);
			PRINT_RELEASE
			break;

		}*/

	}
	PRINT_ALLOC
	fprintf(OutHandle, "\r");fflush(OutHandle);
	PRINT_RELEASE
	return errNone;
}



void FRAM_GetMemoryStatus(unsigned short *total_m, unsigned short *used_m, unsigned short *free_m)
{
	*total_m = FRAM_SIZE;
	*used_m = sizeof(struct FRAM);
	*free_m = *total_m - *used_m;
}

void FRAM_GetUserDataCapacity(unsigned short *datablock_count_m, unsigned short *datablock_size_m)
{
	*datablock_count_m = DATA_BLOCK_COUNT;
	*datablock_size_m = CHARMEM_BLOCK_LENGTH;

}


unsigned short FRAMSaveDeviceConfig(const udtDeviceConfig *Config)
{
	unsigned short crc;
	struct FRAM *ptr;
	#ifdef _CF_DEBUG_PRINT
	if(g_TestFlag & TF_DBG_FRAM){dbPrint("wDevConfig Size:%u\r",sizeof(ptr->DeviceConfigFRAM.DeviceConfig));}
	#endif
FRAM_ALLOC
	WriteEEPROM(offsetof(struct FRAM, DeviceConfigFRAM.DeviceConfig), sizeof(ptr->DeviceConfigFRAM.DeviceConfig),(unsigned char*) Config);
	crc = MyCalcCRC16((unsigned char*)Config, sizeof(ptr->DeviceConfigFRAM.DeviceConfig));
	WriteEEPROM(offsetof(struct FRAM, DeviceConfigFRAM.CRC), sizeof(ptr->DeviceConfigFRAM.CRC), (unsigned char*) &crc);
FRAM_RELEASE
	return errNone;
}

unsigned short FRAMReadDeviceConfig(udtDeviceConfig *Config)
{
	unsigned short error = errNone;
	unsigned int crc;
	struct FRAM *ptr;
	#ifdef _CF_DEBUG_PRINT
	if(g_TestFlag & TF_DBG_FRAM){dbPrint("rDevConfig Size:%u\r",sizeof(ptr->DeviceConfigFRAM.DeviceConfig));}
	#endif
FRAM_ALLOC
	ReadEEPROM(offsetof(struct FRAM, DeviceConfigFRAM.DeviceConfig), sizeof(ptr->DeviceConfigFRAM.DeviceConfig),(unsigned char*)Config);
	ReadEEPROM(offsetof(struct FRAM, DeviceConfigFRAM.CRC), sizeof(ptr->DeviceConfigFRAM.CRC),(unsigned char*)&crc);
	if(MyCalcCRC16((unsigned char*)Config,sizeof(ptr->DeviceConfigFRAM.DeviceConfig)) != crc) error = errCheckSum;
	//free(ptr);
FRAM_RELEASE
	return error;
}

unsigned short FRAMSaveSensorConfig(const udtSensorConfig *Config)
{
	unsigned short crc;
	struct FRAM *ptr;
	#ifdef _CF_DEBUG_PRINT
	if(g_TestFlag & TF_DBG_FRAM){dbPrint("wSensConfig Size:%u\r",sizeof(ptr->SensorConfigFRAM.SensorConfig));}
	#endif
FRAM_ALLOC
	WriteEEPROM(offsetof(struct FRAM, SensorConfigFRAM.SensorConfig), sizeof(ptr->SensorConfigFRAM.SensorConfig),(unsigned char*) Config);
	crc = MyCalcCRC16((unsigned char*)Config, sizeof(ptr->SensorConfigFRAM.SensorConfig));
	WriteEEPROM(offsetof(struct FRAM, SensorConfigFRAM.CRC), sizeof(ptr->SensorConfigFRAM.CRC), (unsigned char*) &crc);
FRAM_RELEASE
	return errNone;
}

unsigned short FRAMReadSensorConfig(udtSensorConfig *Config)
{
	unsigned short error = errNone;
	unsigned int crc;
	struct FRAM *ptr;
	#ifdef _CF_DEBUG_PRINT
	if(g_TestFlag & TF_DBG_FRAM){dbPrint("rSensConfig Size:%u\r",sizeof(ptr->SensorConfigFRAM.SensorConfig));}
	#endif
FRAM_ALLOC
	ReadEEPROM(offsetof(struct FRAM, SensorConfigFRAM.SensorConfig), sizeof(ptr->SensorConfigFRAM.SensorConfig),(unsigned char*)Config);
	ReadEEPROM(offsetof(struct FRAM, SensorConfigFRAM.CRC), sizeof(ptr->SensorConfigFRAM.CRC),(unsigned char*)&crc);
	if(MyCalcCRC16((unsigned char*)Config,sizeof(ptr->SensorConfigFRAM.SensorConfig)) != crc) error = errCheckSum;
	//free(ptr);
FRAM_RELEASE
	return error;
}

unsigned short FRAMSavePortConfig(const udtPortConfig *Config)
{
	unsigned short crc;
	struct FRAM *ptr;
	#ifdef _CF_DEBUG_PRINT
	if(g_TestFlag & TF_DBG_FRAM){dbPrint("wPortConfig Size:%u\r",sizeof(ptr->PortConfigFRAM.PortConfig));}
	#endif
FRAM_ALLOC
	WriteEEPROM(offsetof(struct FRAM, PortConfigFRAM.PortConfig), sizeof(ptr->PortConfigFRAM.PortConfig),(unsigned char*) Config);
	crc = MyCalcCRC16((unsigned char*)Config, sizeof(ptr->PortConfigFRAM.PortConfig));
	WriteEEPROM(offsetof(struct FRAM, PortConfigFRAM.CRC), sizeof(ptr->PortConfigFRAM.CRC), (unsigned char*) &crc);
FRAM_RELEASE
	return errNone;
}

unsigned short FRAMReadPortConfig(udtPortConfig *Config)
{
	unsigned short error = errNone;
	unsigned int crc;
	struct FRAM *ptr;
	#ifdef _CF_DEBUG_PRINT
	if(g_TestFlag & TF_DBG_FRAM){dbPrint("rPortConfig Size:%u\r",sizeof(ptr->PortConfigFRAM.PortConfig));}
	#endif
FRAM_ALLOC
	ReadEEPROM(offsetof(struct FRAM, PortConfigFRAM.PortConfig), sizeof(ptr->PortConfigFRAM.PortConfig),(unsigned char*)Config);
	ReadEEPROM(offsetof(struct FRAM, PortConfigFRAM.CRC), sizeof(ptr->PortConfigFRAM.CRC),(unsigned char*)&crc);
	if(MyCalcCRC16((unsigned char*)Config,sizeof(ptr->PortConfigFRAM.PortConfig)) != crc) error = errCheckSum;
	//free(ptr);
FRAM_RELEASE
	return error;
}

unsigned short FRAMSaveEquipmentConfig(const udtEquipmentConfig *Config)
{
	unsigned short crc;
	struct FRAM *ptr;
	#ifdef _CF_DEBUG_PRINT
	if(g_TestFlag & TF_DBG_FRAM){dbPrint("wEquipmentConfig Size:%u\r",sizeof(ptr->EquipmentConfigFRAM.EquipmentConfig));}
	#endif
FRAM_ALLOC
	WriteEEPROM(offsetof(struct FRAM, EquipmentConfigFRAM.EquipmentConfig), sizeof(ptr->EquipmentConfigFRAM.EquipmentConfig),(unsigned char*) Config);
	crc = MyCalcCRC16((unsigned char*)Config, sizeof(ptr->EquipmentConfigFRAM.EquipmentConfig));
	WriteEEPROM(offsetof(struct FRAM, EquipmentConfigFRAM.CRC), sizeof(ptr->EquipmentConfigFRAM.CRC), (unsigned char*) &crc);
FRAM_RELEASE
	return errNone;
}

unsigned short FRAMReadEquipmentConfig(udtEquipmentConfig *Config)
{
	unsigned short error = errNone;
	unsigned int crc;
	struct FRAM *ptr;
	#ifdef _CF_DEBUG_PRINT
	if(g_TestFlag & TF_DBG_FRAM){dbPrint("rEquipmentConfig Size:%u\r",sizeof(ptr->EquipmentConfigFRAM.EquipmentConfig));}
	#endif
FRAM_ALLOC
	ReadEEPROM(offsetof(struct FRAM, EquipmentConfigFRAM.EquipmentConfig), sizeof(ptr->EquipmentConfigFRAM.EquipmentConfig),(unsigned char*)Config);
	ReadEEPROM(offsetof(struct FRAM, EquipmentConfigFRAM.CRC), sizeof(ptr->EquipmentConfigFRAM.CRC),(unsigned char*)&crc);
	if(MyCalcCRC16((unsigned char*)Config,sizeof(ptr->EquipmentConfigFRAM.EquipmentConfig)) != crc) error = errCheckSum;
	//free(ptr);
FRAM_RELEASE
	return error;
}

unsigned short FRAMSaveEstopCounter(const udtEstopCounter *Config)
{
	unsigned short crc;
	struct FRAM *ptr;
	#ifdef _CF_DEBUG_PRINT
	if(g_TestFlag & TF_DBG_FRAM){dbPrint("wEstopCounter Size:%u\r",sizeof(ptr->EstopCounterFRAM.EstopCounter));}
	#endif
FRAM_ALLOC
	WriteEEPROM(offsetof(struct FRAM, EstopCounterFRAM.EstopCounter), sizeof(ptr->EstopCounterFRAM.EstopCounter),(unsigned char*) Config);
	crc = MyCalcCRC16((unsigned char*)Config, sizeof(ptr->EstopCounterFRAM.EstopCounter));
	WriteEEPROM(offsetof(struct FRAM, EstopCounterFRAM.CRC), sizeof(ptr->EstopCounterFRAM.CRC), (unsigned char*) &crc);
FRAM_RELEASE
	return errNone;
}

unsigned short FRAMReadEstopCounter(udtEstopCounter *Config)
{
	unsigned short error = errNone;
	unsigned int crc;
	struct FRAM *ptr;
	#ifdef _CF_DEBUG_PRINT
	if(g_TestFlag & TF_DBG_FRAM){dbPrint("rEstopCounter Size:%u\r",sizeof(ptr->EstopCounterFRAM.EstopCounter));}
	#endif
FRAM_ALLOC
	ReadEEPROM(offsetof(struct FRAM, EstopCounterFRAM.EstopCounter), sizeof(ptr->EstopCounterFRAM.EstopCounter),(unsigned char*)Config);
	ReadEEPROM(offsetof(struct FRAM, EstopCounterFRAM.CRC), sizeof(ptr->EstopCounterFRAM.CRC),(unsigned char*)&crc);
	if(MyCalcCRC16((unsigned char*)Config,sizeof(ptr->EstopCounterFRAM.EstopCounter)) != crc) error = errCheckSum;
	//free(ptr);
FRAM_RELEASE
	return error;
}

unsigned short FRAMSaveEstopWait(const udtEstopWait *Config)
{
	unsigned short crc;
	struct FRAM *ptr;
	#ifdef _CF_DEBUG_PRINT
	if(g_TestFlag & TF_DBG_FRAM){dbPrint("wEstopWait Size:%u\r",sizeof(ptr->EstopWaitFRAM.EstopWait));}
	#endif
FRAM_ALLOC
	WriteEEPROM(offsetof(struct FRAM, EstopWaitFRAM.EstopWait), sizeof(ptr->EstopWaitFRAM.EstopWait),(unsigned char*) Config);
	crc = MyCalcCRC16((unsigned char*)Config, sizeof(ptr->EstopWaitFRAM.EstopWait));
	WriteEEPROM(offsetof(struct FRAM, EstopWaitFRAM.CRC), sizeof(ptr->EstopWaitFRAM.CRC), (unsigned char*) &crc);
FRAM_RELEASE
	return errNone;
}

unsigned short FRAMReadEstopWait(udtEstopWait *Config)
{
	unsigned short error = errNone;
	unsigned int crc;
	struct FRAM *ptr;
	#ifdef _CF_DEBUG_PRINT
	if(g_TestFlag & TF_DBG_FRAM){dbPrint("rEstopWait Size:%u\r",sizeof(ptr->EstopCounterFRAM.EstopCounter));}
	#endif
FRAM_ALLOC
	ReadEEPROM(offsetof(struct FRAM, EstopWaitFRAM.EstopWait), sizeof(ptr->EstopWaitFRAM.EstopWait),(unsigned char*)Config);
	ReadEEPROM(offsetof(struct FRAM, EstopWaitFRAM.CRC), sizeof(ptr->EstopWaitFRAM.CRC),(unsigned char*)&crc);
	if(MyCalcCRC16((unsigned char*)Config,sizeof(ptr->EstopWaitFRAM.EstopWait)) != crc) error = errCheckSum;
	//free(ptr);
FRAM_RELEASE
	return error;
}

unsigned short FRAMSaveAxisConfigStallExeption(const udtConfigStallExeption *CnfgStallExep)
{ 
	unsigned short crc;
	//struct FRAM *ptr = (struct FRAM*) calloc(1, sizeof(struct FRAM));
	struct FRAM *ptr;
	//unsigned int len;
	//if(ptr == NULL) return ERR_MEMORY_ALLOCATE;
	//len =  sizeof(ptr->OffsetFRAM.Offset);
	#ifdef _CF_DEBUG_PRINT
	if(g_TestFlag & TF_DBG_FRAM){dbPrint("wStallExep Size:%u\r",sizeof(ptr->ConfigStallExeptionFRAM.ConfigStallExeption));}
	#endif
FRAM_ALLOC
	WriteEEPROM(offsetof(struct FRAM, ConfigStallExeptionFRAM.ConfigStallExeption), sizeof(ptr->ConfigStallExeptionFRAM.ConfigStallExeption),(unsigned char*) CnfgStallExep);
	crc = MyCalcCRC16((unsigned char*)CnfgStallExep, sizeof(ptr->ConfigStallExeptionFRAM.ConfigStallExeption));
	//crc = MyCalcCRC16((unsigned char*)offset, sizeof(*offset));
	WriteEEPROM(offsetof(struct FRAM, ConfigStallExeptionFRAM.CRC), sizeof(ptr->ConfigStallExeptionFRAM.CRC), (unsigned char*) &crc);
	//free(ptr);
FRAM_RELEASE
	return errNone;
}
unsigned short FRAMReadAxisConfigStallExeption(udtConfigStallExeption *CnfgStallExep)
{
	unsigned short error = errNone;
	unsigned int crc;
	//struct FRAM *ptr = (struct FRAM*) calloc(1, sizeof(struct FRAM));
	struct FRAM *ptr;
	//if(ptr == NULL) return ERR_MEMORY_ALLOCATE;
	#ifdef _CF_DEBUG_PRINT
	if(g_TestFlag & TF_DBG_FRAM){dbPrint("rStallExep Size:%u\r",sizeof(ptr->ConfigStallExeptionFRAM.ConfigStallExeption));}
	#endif
FRAM_ALLOC
	//ReadEEPROM(offsetof(struct FRAM, OffsetFRAM.Offset), sizeof(ptr->OffsetFRAM.Offset),(unsigned char*)&AxisOffset);
	ReadEEPROM(offsetof(struct FRAM, ConfigStallExeptionFRAM.ConfigStallExeption), sizeof(ptr->ConfigStallExeptionFRAM.ConfigStallExeption),(unsigned char*)CnfgStallExep);
	ReadEEPROM(offsetof(struct FRAM, ConfigStallExeptionFRAM.CRC), sizeof(ptr->ConfigStallExeptionFRAM.CRC),(unsigned char*)&crc);
	//if(MyCalcCRC16((unsigned char*)offset,sizeof(*offset)) != crc) error = errCheckSum;
	if(MyCalcCRC16((unsigned char*)CnfgStallExep,sizeof(ptr->ConfigStallExeptionFRAM.ConfigStallExeption)) != crc) error = errCheckSum;
	//free(ptr);
FRAM_RELEASE
	return error;
}



unsigned short FRAMSaveAxisCounterStallExeption(const udtCounterStallExeption *CntStallExep)
{ 
	unsigned short crc;
	//struct FRAM *ptr = (struct FRAM*) calloc(1, sizeof(struct FRAM));
	struct FRAM *ptr;
	//unsigned int len;
	//if(ptr == NULL) return ERR_MEMORY_ALLOCATE;
	//len =  sizeof(ptr->OffsetFRAM.Offset);
	#ifdef _CF_DEBUG_PRINT
	if(g_TestFlag & TF_DBG_FRAM){dbPrint("wStallExep Size:%u\r",sizeof(ptr->ConfigStallExeptionFRAM.ConfigStallExeption));}
	#endif
FRAM_ALLOC
	WriteEEPROM(offsetof(struct FRAM, CounterStallExeptionFRAM.CounterStallExeption), sizeof(ptr->CounterStallExeptionFRAM.CounterStallExeption),(unsigned char*) CntStallExep);
	crc = MyCalcCRC16((unsigned char*)CntStallExep, sizeof(ptr->CounterStallExeptionFRAM.CounterStallExeption));
	//crc = MyCalcCRC16((unsigned char*)offset, sizeof(*offset));
	WriteEEPROM(offsetof(struct FRAM, CounterStallExeptionFRAM.CRC), sizeof(ptr->CounterStallExeptionFRAM.CRC), (unsigned char*) &crc);
	//free(ptr);
FRAM_RELEASE
	return errNone;
}
unsigned short FRAMReadAxisCounterStallExeption(udtCounterStallExeption *CntStallExep)
{
	unsigned short error = errNone;
	unsigned int crc;
	//struct FRAM *ptr = (struct FRAM*) calloc(1, sizeof(struct FRAM));
	struct FRAM *ptr;
	//if(ptr == NULL) return ERR_MEMORY_ALLOCATE;
	#ifdef _CF_DEBUG_PRINT
	if(g_TestFlag & TF_DBG_FRAM){dbPrint("rStallExep Size:%u\r",sizeof(ptr->CounterStallExeptionFRAM.CounterStallExeption));}
	#endif
FRAM_ALLOC
	//ReadEEPROM(offsetof(struct FRAM, OffsetFRAM.Offset), sizeof(ptr->OffsetFRAM.Offset),(unsigned char*)&AxisOffset);
	ReadEEPROM(offsetof(struct FRAM, CounterStallExeptionFRAM.CounterStallExeption), sizeof(ptr->CounterStallExeptionFRAM.CounterStallExeption),(unsigned char*)CntStallExep);
	ReadEEPROM(offsetof(struct FRAM, CounterStallExeptionFRAM.CRC), sizeof(ptr->CounterStallExeptionFRAM.CRC),(unsigned char*)&crc);
	//if(MyCalcCRC16((unsigned char*)offset,sizeof(*offset)) != crc) error = errCheckSum;
	if(MyCalcCRC16((unsigned char*)CntStallExep,sizeof(ptr->CounterStallExeptionFRAM.CounterStallExeption)) != crc) error = errCheckSum;
	//free(ptr);
FRAM_RELEASE
	return error;
}


unsigned short FRAMSaveSearchAdapterRelativeDeltaWay(const udtRelativeDeltaWay *Config)
{
	unsigned short crc;
	struct FRAM *ptr;
	#ifdef _CF_DEBUG_PRINT
	if(g_TestFlag & TF_DBG_FRAM){dbPrint("wSearchAdapterRelativeDeltaWay Size:%u\r",sizeof(ptr->SearchAdapterRelativeDeltaWayFRAM.SearchAdapterRelativeDeltaWay));}
	#endif
FRAM_ALLOC
	WriteEEPROM(offsetof(struct FRAM, SearchAdapterRelativeDeltaWayFRAM.SearchAdapterRelativeDeltaWay), sizeof(ptr->SearchAdapterRelativeDeltaWayFRAM.SearchAdapterRelativeDeltaWay),(unsigned char*) Config);
	crc = MyCalcCRC16((unsigned char*)Config, sizeof(ptr->SearchAdapterRelativeDeltaWayFRAM.SearchAdapterRelativeDeltaWay));
	WriteEEPROM(offsetof(struct FRAM, SearchAdapterRelativeDeltaWayFRAM.CRC), sizeof(ptr->SearchAdapterRelativeDeltaWayFRAM.CRC), (unsigned char*) &crc);
FRAM_RELEASE
	return errNone;
}

unsigned short FRAMReadSearchAdapterRelativeDeltaWay(udtRelativeDeltaWay *Config)
{
	unsigned short error = errNone;
	unsigned int crc;
	struct FRAM *ptr;
	#ifdef _CF_DEBUG_PRINT
	if(g_TestFlag & TF_DBG_FRAM){dbPrint("rSearchAdapterRelativeDeltaWay Size:%u\r",sizeof(ptr->SearchAdapterRelativeDeltaWayFRAM.SearchAdapterRelativeDeltaWay));}
	#endif
FRAM_ALLOC
	ReadEEPROM(offsetof(struct FRAM, SearchAdapterRelativeDeltaWayFRAM.SearchAdapterRelativeDeltaWay), sizeof(ptr->SearchAdapterRelativeDeltaWayFRAM.SearchAdapterRelativeDeltaWay),(unsigned char*)Config);
	ReadEEPROM(offsetof(struct FRAM, SearchAdapterRelativeDeltaWayFRAM.CRC), sizeof(ptr->SearchAdapterRelativeDeltaWayFRAM.CRC),(unsigned char*)&crc);
	if(MyCalcCRC16((unsigned char*)Config,sizeof(ptr->SearchAdapterRelativeDeltaWayFRAM.SearchAdapterRelativeDeltaWay)) != crc) error = errCheckSum;
	//free(ptr);
FRAM_RELEASE
	return error;
}

unsigned short FRAMSaveDoorLiftConfig(const udtIMSDoorLiftConfig *Config)
{
	unsigned short crc;
	struct FRAM *ptr;
	#ifdef _CF_DEBUG_PRINT
	if(g_TestFlag & TF_DBG_FRAM){dbPrint("wIMSDoorLiftConfig Size:%u\r",sizeof(ptr->IMSDoorLiftAxisFRAM.IMSDoorLiftConfig));}
	#endif
FRAM_ALLOC
	WriteEEPROM(offsetof(struct FRAM, IMSDoorLiftAxisFRAM.IMSDoorLiftConfig), sizeof(ptr->IMSDoorLiftAxisFRAM.IMSDoorLiftConfig),(unsigned char*) Config);
	crc = MyCalcCRC16((unsigned char*)Config, sizeof(ptr->IMSDoorLiftAxisFRAM.IMSDoorLiftConfig));
	WriteEEPROM(offsetof(struct FRAM, IMSDoorLiftAxisFRAM.CRC), sizeof(ptr->IMSDoorLiftAxisFRAM.CRC), (unsigned char*) &crc);
FRAM_RELEASE
	return errNone;
}

unsigned short FRAMReadDoorLiftConfig(udtIMSDoorLiftConfig *Config)
{
	unsigned short error = errNone;
	unsigned int crc;
	struct FRAM *ptr;
	#ifdef _CF_DEBUG_PRINT
	if(g_TestFlag & TF_DBG_FRAM){dbPrint("rSearchIMSDoorLiftConfig Size:%u\r",sizeof(ptr->IMSDoorLiftAxisFRAM.IMSDoorLiftConfig));}
	#endif
FRAM_ALLOC
	ReadEEPROM(offsetof(struct FRAM, IMSDoorLiftAxisFRAM.IMSDoorLiftConfig), sizeof(ptr->IMSDoorLiftAxisFRAM.IMSDoorLiftConfig),(unsigned char*)Config);
	ReadEEPROM(offsetof(struct FRAM, IMSDoorLiftAxisFRAM.CRC), sizeof(ptr->IMSDoorLiftAxisFRAM.CRC),(unsigned char*)&crc);
	if(MyCalcCRC16((unsigned char*)Config,sizeof(ptr->IMSDoorLiftAxisFRAM.IMSDoorLiftConfig)) != crc) error = errCheckSum;
	//free(ptr);
FRAM_RELEASE
	return error;
}
