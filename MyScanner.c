/**************************************************************************
* Datei : scanner.c
*
* Aufgabe : lexikalische Analyse
*
* Programmiersprache : Tasking C/C++ V6
*
* Projekt : 
*
* Autor : H.Köberich
* Firma : (c) 2000  CyBio AG
*         Göschwitzer Str. 40
*         07745 Jena
*
* letzte Änderung : 01.12.2004
*
**************************************************************************/

/**************************************************************************
* standard functions
**************************************************************************/
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "DevError.h"
#include <limits.h>
#include "DeviceConfig.h"
#include "MyScanner.h"
#include "MAIN.H"
#include "Felix_Misc.h"

//#include "Shared/Stacker_Lifter_Allgemein/StacError.h"
/**************************************************************************
* defines, structures, functions
**************************************************************************/
#define RESET	   0
#define EXP	       2

char m_szScannerBuf[ASCBUF];
char *m_pScannerBuf;
char m_szSaveScannerBuf[ASCBUF];
char m_szSaveReloadScannerBuf[ASCBUF];
//char m_szLPKSaveScannerBuf[ASCBUF];
//unsigned short m_LPKSaveAdr = 0;

char *	fgets( char * s, int n, FILE * fp );
void ResetSaveScannerBuffer(void);
void ResetSaveScannerBufferForReload(void);

void ResetSaveScannerBuffer(void)
{unsigned short i;

	for(i = 0; i < ASCBUF; i++)
	{
		WDO();			  
		m_szSaveScannerBuf[i] = 0;
	}
}
void ResetSaveScannerBufferForReload(void)
{unsigned short i;

	for(i = 0; i < ASCBUF; i++)
	{
		WDO();			  
		m_szSaveReloadScannerBuf[i] = 0;
	}
}

void ResetScannerBuffer(void)
{unsigned short i;
	m_pScannerBuf = m_szScannerBuf;
	for(i = 0; i < ASCBUF; i++)
	{
		WDO();			  
		//m_szScannerBuf[i] = 0;
		*m_pScannerBuf++ = 0;
	}
	m_pScannerBuf = m_szScannerBuf;
}

char *	fgets( char * s, int n, FILE * fp )
{
	register int c;
	register char *cs;

	c = !EOF;	/* initialise for n <= 0 */
	cs = s;
	while ( (--n > 0) && ((c = fgetc( fp )) != EOF) )
	{	/* read until end of file or until 'n-1' characters are read */
		*cs++ = (char) c;
		if ( c == '\n' )	/* Read until newline is found */
		{
			break;
		}
	}
	if ( (c == EOF) && (cs == s) )	/* end of file, and no characters have been read */
	{
		return NULL;
	}
	*cs = '\0';			/* append a '\0' character */
	return s;
}

unsigned short ZaehleZeichenImScanner(char zeichen)
{
unsigned short i;
unsigned short cnt = 0;
//char *save_ptr;
	//save_ptr = m_pScannerBuf;
	//m_pScannerBuf = m_szScannerBuf;
	for(i = 0; i < ASCBUF; i++)
	{
		WDO();			  
		if(m_szScannerBuf[i] == zeichen)
		{
			cnt++;
		}
		if(m_szScannerBuf[i] == 0)
		{
			//m_pScannerBuf = save_ptr;
			return cnt;
		}

	}
	//m_pScannerBuf = save_ptr;
	return cnt;
}
char *GetScannerPtr(void)
{
	return m_pScannerBuf; 
}
unsigned short GetStreamToScanner(FILE *stream)
{ 
	ResetScannerBuffer();
    if(fgets(m_szScannerBuf, ASCBUF, stream) != NULL)
	{
    	m_pScannerBuf=m_szScannerBuf;
		return errNone;
	}
	return errRxOverflow;;
}

void SaveScannerBuffer(void)
{
//	ResetSaveScannerBuffer();
	strcpy(m_szSaveScannerBuf, m_pScannerBuf);
//dbPrint("Sv1 :%s:\r", m_pScannerBuf);
//dbPrint("S:%s:\r", m_szSaveScannerBuf);
}

void SaveScannerBufferForReload(void)
{
	ResetSaveScannerBufferForReload();
	strcpy(m_szSaveReloadScannerBuf, m_szSaveScannerBuf);
//	dbPrint("SR:%s:%u\r", m_szSaveScannerBuf, value);
}
void ReloadScannerBuffer(void)
{
	ResetScannerBuffer();
	strcpy(m_pScannerBuf, m_szSaveReloadScannerBuf);
//dbPrint("R:%s:%u\r", m_szSaveReloadScannerBuf, value);
}

/*unsigned short SaveLPKScannerBuffer(unsigned short Adr)
{
	strcpy(m_szLPKSaveScannerBuf, m_pScannerBuf);
	m_LPKSaveAdr = Adr;
	return errNone;
}
unsigned short ReloadLPKScannerBuffer(unsigned short *Adr)
{
	strcpy(m_pScannerBuf, m_szLPKSaveScannerBuf);
	*Adr = m_LPKSaveAdr;
	return errNone;
}
*/
int GetC_ScannerBuf(void)
{
	unsigned short ReturnValue;
	//char szHelpBuffer[ASCBUF];
	if(m_pScannerBuf > m_szScannerBuf + ASCBUF)  {return EOF;}
	
	ReturnValue = *m_pScannerBuf++;
	
//	strcpy(szHelpBuffer, m_pScannerBuf);
//	strcpy(m_szScannerBuf, szHelpBuffer);
//	m_pScannerBuf = m_szScannerBuf;
	return ReturnValue;
}
int UngetC_ScannerBuf(int value)
{//char szValue[ASCBUF];
//	sprintf(szValue,"%c", value);
//	strcat(szValue, m_szScannerBuf);
//	strcpy(m_szScannerBuf, szValue);
//	m_pScannerBuf = m_szScannerBuf;
	--m_pScannerBuf;
	if(m_pScannerBuf < m_szScannerBuf)  {return EOF;}
	*m_pScannerBuf = value;
	return value;
	
}


/**************************************************************************
* Funktion : scanner
*
* Aufgabe : lexikalische Analyse
*
* Datei / Module : scan.c
*
* Autor : H.Köberich
*
* letzte Änderung : 14.02.2002
*
* Stand : Entwickeln  Testen  ok
*
**************************************************************************/
//unsigned short scanner(char * Buffer, FILE * handle )
unsigned short scanner(char * Buffer)
{
	int zeichen;
	unsigned short zustand = RESET;
	unsigned short bufptr = 0;
	double temp;

	while( 1 )
	{
/*
		// nicht an der schnittstelle versumpfen
		if(handle == stdin)
		{
			if(!Uart2_kbHit())
			{
				return Scan_EOF;
			}
		}
		if(handle == U0C1_IN)
		{
			if(!Uart1_kbHit())
			{
				return Scan_EOF;
			}
		}
		if(handle == U0C0_IN)
		{
			if(!Uart0_kbHit())
			{
				return Scan_EOF;
			}
		}
*/
		
//		zeichen = getc(handle);
		zeichen = GetC_ScannerBuf();
		//if(zeichen == 0) return Scan_EOF;	//	ASC-Fehler
		if(zeichen == EOF) 
		{
//printf("\rscanner EOF\r");fflush(stdout);
			return Scan_EOF;	//	ASC-Fehler
		}
//printf("\rscanner ch: %c %i\r", zeichen, zeichen);fflush(stdout);
		if(zeichen == '#') return HASH;			// Im Handling des Abbruch-Zeichens unterscheiden sich die Quellen Sanner.c und Scanner1.c
		if(zeichen == '%') return PERCENT;
		*(Buffer + bufptr++) = zeichen;

		switch( zustand )
		{
			case RESET :	if( isalpha( zeichen ) || (zeichen == '_') || (zeichen == '[') || (zeichen == ']'))
							{
								zustand = IDENT;
							}else
							{
								if( isdigit( zeichen ) || zeichen == '+' || zeichen == '-')
								{
									zustand = ICONST;
								}else
								{
									switch(zeichen)
									{
										case '"' :	bufptr--;
										case '.' :	zustand = zeichen;
													break;
										case '%' :
										case '\n':	return zeichen;
										default  :	bufptr = 0;
													break;
									}
								}
							}
							break;
			case IDENT :	switch(zeichen)
							{
								default  :	if( !isalnum( zeichen ) && ( zeichen != '_') && (zeichen != '[') && (zeichen != ']'))
											{
												*(Buffer+bufptr-1) = EOS;
												//ungetc((int)zeichen,handle);
												UngetC_ScannerBuf(zeichen);
												return IDENT;
											}
							}
							break;

			case '.'   :	switch(zeichen)
							{
								case 'e' :
								case 'E' :	zustand = zeichen;
											break;
								default  :	if(!isdigit(zeichen))
											{
												*(Buffer+bufptr-1) = EOS;
												//ungetc((int)zeichen,handle);
												UngetC_ScannerBuf(zeichen);
												return FCONST;
											}
											break;
							}
							break;

			case 'e'   :
			case 'E'   :	switch(zeichen)
							{
								case '-' :
								case '+' :	break;
								default  :	if(isdigit(zeichen))
											{
												zustand = EXP;
											}else
											{
												*(Buffer+bufptr-1) = EOS;
												//ungetc((int)zeichen,handle);
												UngetC_ScannerBuf(zeichen);
												return FCONST;
											}
											break;
							}
							break;

			case EXP   :	if(!isdigit( zeichen ))
							{
								*(Buffer+bufptr-1) = EOS;
								//ungetc((int)zeichen,handle);
								UngetC_ScannerBuf(zeichen);
								return FCONST;
							}
							break;

			case ICONST:	switch( zeichen )
							{
								case '.' :
								case 'e' :
								case 'E' :	zustand = zeichen;
											break;
								default  :	if(!isdigit(zeichen))
											{
												*(Buffer+bufptr-1) = EOS;
												//ungetc((int)zeichen, handle);
												UngetC_ScannerBuf(zeichen);
												temp = atof(Buffer);
												if((temp >= (float)CHAR_MIN) && (temp <= (float)CHAR_MAX))
												{
													zustand = BYTE;
												}else
												{
													if((temp > (float)CHAR_MAX) && (temp <= (float)UCHAR_MAX))
													{
														zustand = UBYTE;
													}else
													{
														if((temp >= (float)INT_MIN) && (temp <= (float)INT_MAX))
														{
															zustand = ICONST;
														}else
														{
															if((temp > (float)INT_MAX) && (temp <= (float)UINT_MAX))
															{
																zustand = UICONST;
															}else
															{
																if((temp >= (float)LONG_MIN) && (temp <= (float)LONG_MAX))
																{
																	zustand = LCONST;
																}else
																{
																	zustand = ((temp > (float)LONG_MAX) && (temp <= (float)ULONG_MAX)) ? ULCONST : NANR; 
																}
															}
														}
													}
												}
												return zustand;
											}
											break;
							}
							break;

			case '"'	:	switch(zeichen)
							{
								case '"' :
								case '\n':	*(Buffer + bufptr - 1) = EOS;
											return STRING;
								default  :	break;
							}
							break;

		}//switch(zustand)
	}//while
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
/*
unsigned int scanOnOff(unsigned int * value, FILE *handle)
{
	unsigned int error = Scan_errNone;
	char Buffer[20];

	switch(scanner(Buffer))
	{
		case Scan_errASC:	error = Scan_errASC;
							break;
		case '\n'		:	error = Scan_errNotEnoughParam;
							break;

		case IDENT		:	if(strcmp(Buffer, "on") == 0) { *value = ON; break; }
							if(strcmp(Buffer, "off") == 0) {*value = OFF; break; }

		default			:	error = Scan_errParamWrong;
							break;
	}
	return error;
}
*/
