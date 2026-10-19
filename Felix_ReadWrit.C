/*******************************************************************
 *
 *    DESCRIPTION: Communikations-Interface
 *
 *    AUTHOR:
 *
 *    HISTORY:
 *
 *******************************************************************/
#include "30-5015_Felix/XE167F/main.h"
#include "30-5015_Felix/XE167F/ASC0.h"
#include "30-5015_Felix/XE167F/U0C1_RS485_COM1.H"
#include "30-5015_Felix/XE167F/U1C0_SLAVE_COM5.H"
#include "30-5015_Felix/XE167F/U1C1_USB_COM2.H"
#include "30-5015_Felix/XE167F/U2C0_LAN_COM3.H"
#include "30-5015_Felix/XE167F/U2C1_RS232_COM4.H"
/*
COM0 U0C0 HOST stdin ASC0
int UART0_GetChar(void);
int UART0_PutChar(char * base, size_t siz);
IN  0
OUT 1
ERR 2
---------------------------------------------
COM1 U0C1 RS485 MainBus
int UART1_GetChar(void);
int UART1_PutChar(char * base, size_t siz);
IN  3
OUT 4
---------------------------------------------
COM2 U1C1 RS232 USB
int UART2_GetChar(void);
int UART2_PutChar(char * base, size_t siz);
IN  5
OUT 6
---------------------------------------------
COM3 U2C0 RS232 LAN
int UART3_GetChar(void);
int UART3_PutChar(char * base, size_t siz);
IN  7
OUT 7
---------------------------------------------
COM4 U2C1 RS232 Barcode
int UART4_GetChar(void);
int UART4_PutChar(char * base, size_t siz);
IN  8
OUT 8
---------------------------------------------
COM5 U1C0 RS232 Slave
int UART5_GetChar(void);
int UART5_PutChar(char * base, size_t siz);
IN  9
OUT 9
---------------------------------------------
*/
int _ioread( int fd )
{
        /* At this point, it is known that 'fin' is a file opened for   */
        /* reading, and is not in error yet.                            */
        /* This routine should read the input for the given filehandle  */
        /* (in the standard version only 'stdin') from the required     */
        /* hardware.                                                    */
        /* Whenever an error occurs, 'EOF' should be returned           */
        /* and fin->_flag should be set to the proper error value       */
        /* i.e.         _IOEOF on end of FILE                           */
        /*              _IOERR on any other error                       */
        /* Note that when a read is done on 'stdin', this routine       */
        /* should do the scanning, and whenever neccessary, do the      */
        /* echoing to stdout                                            */

  int c = EOF;
  if ( fd == 0 )
  {
     /* Assume terminal sending either CR or CRLF. Allow
        only LF as valid delimiter, so translate CR into LF
        and ignore received LF's
     */
     //do	/* blocking read from serial channel 0 */
	 { c = UART0_GetChar();			/* without echo */
	   if ( c == 0x0a )		/* LF */
			c = EOF;		/* ignore it */
	   else if(c == 0x0d)	/* CR */
	   		c = 0x0a;		/* turn into LF */
	 } //while ( c < 0 );

	 //} while ( c == 0 );
	 /* when reading 'stdin', echo to 'stdout'
	    performing LF ==> CR-LF translation
	 */
	 //_iowrite( c, 1 );
  }

  if ( fd == 3 )
  {
     /* Assume terminal sending either CR or CRLF. Allow
        only LF as valid delimiter, so translate CR into LF
        and ignore received LF's
     */
     //do	/* blocking read from serial channel 0 */
	 //{
	 c = UART1_GetChar();			/* without echo */
	   if ( c == 0x0a )		/* LF */
			c = EOF;		/* ignore it */
	   else if(c == 0x0d)	/* CR */
	   		c = 0x0a;		/* turn into LF */
	 //} while ( c < 0 );
  }

  if ( fd == 5 )
  {
     /* Assume terminal sending either CR or CRLF. Allow
        only LF as valid delimiter, so translate CR into LF
        and ignore received LF's
     */
     //do	/* blocking read from serial channel 0 */
	 //{
	 c = UART2_GetChar();			/* without echo */
	   if ( c == 0x0a )		/* LF */
			c = EOF;		/* ignore it */
	   else if(c == 0x0d)	/* CR */
	   		c = 0x0a;		/* turn into LF */
	 //} while ( c < 0 );
  }

  if ( fd == 7 )
  {
     /* Assume terminal sending either CR or CRLF. Allow
        only LF as valid delimiter, so translate CR into LF
        and ignore received LF's
     */
     //do	/* blocking read from serial channel 0 */
	 //{
	 c = UART3_GetChar();			/* without echo */
	   if ( c == 0x0a )		/* LF */
			c = EOF;		/* ignore it */
	   else if(c == 0x0d)	/* CR */
	   		c = 0x0a;		/* turn into LF */
	 //} while ( c < 0 );
  }

  if ( fd == 8 )
  {
     /* Assume terminal sending either CR or CRLF. Allow
        only LF as valid delimiter, so translate CR into LF
        and ignore received LF's
     */
     //do	/* blocking read from serial channel 0 */
	 //{
	 c = UART4_GetChar();			/* without echo */
	   if ( c == 0x0a )		/* LF */
			c = EOF;		/* ignore it */
	   else if(c == 0x0d)	/* CR */
	   		c = 0x0a;		/* turn into LF */
	 //} while ( c < 0 );
  }
  if ( fd == 9 )
  {
     /* Assume terminal sending either CR or CRLF. Allow
        only LF as valid delimiter, so translate CR into LF
        and ignore received LF's
     */
     //do	/* blocking read from serial channel 0 */
	 //{
	 c = UART5_GetChar();			/* without echo */
	   if ( c == 0x0a )		/* LF */
			c = EOF;		/* ignore it */
	   else if(c == 0x0d)	/* CR */
	   		c = 0x0a;		/* turn into LF */
	 //} while ( c < 0 );
  }
  return( c );	/* return read character on succes */
}

/* ******************************************************************************
 *  FUNCTION: _iowrite
 *  PARAMETERS:
 *  DESCRIPTION:
 *  RETURNS: 
 ******************************************************************************** */
/*
int	_iowrite( int c, int fd )
//int	_iowrite(char * base, size_t size )
{
        // At this point, it is known that 'fout' is a file opened for 
        // writing, and is not in error yet.                            
        // This routine should write the output for the given filehandle
        // (in the standard version only 'stdout' or 'stderr')          
        // to the required hardware.                                    
        // Whenever an error occurs, 'EOF' should be returned           
        // and fout->_flag should be set to the proper error value      
        // i.e.         _IOEOF on end of FILE                           
        //              _IOERR on any other error                       

	if ( fd == 1  ||  fd == 2 )
	{
		if ( c == '\n' )
			putch( '\r' );
		putch( c );
	}
      return( c );    // return written character on success //
}
*/
/*************************************************************************
*                                                                        *
*  DESCRIPTION :  Reads a block of characters from the given stream      *
*                 returns nr of read characters                          *
*                                                                        *
*************************************************************************/
size_t _read( int fd, char * base, size_t size )
{
	int	c;
	size_t	cnt = 0;

	// At this point, it is known that 'fin' is a file opened for	//
	// reading, and is not in error yet.							//
	// This routine should read the input for the given filehandle  //
	// from the required hardware.									//
	// when nothing is read, '0' should be returned, meaning EOF    //
	// is found                                                     //
	// On any other error, the routine should return non zero       //
	// Note that when a read is done on 'stdin', this routine       //
	// should do the scanning, and whenever neccessary, do the      //
	// echoing to stdout                                            //

	for( ; size; size--,cnt++ )
	{
		// _ioread will echo to stdout when neccessary //
		if( (c = _ioread( fd )) == EOF )
			break;	// Nothing more read //
		*base++ = c;
	}

	return( cnt );
}
/*************************************************************************
*                                                                        *
*  DESCRIPTION :  Write a block of data to the given file                *
*                 As a working example, this routine just calls the low  *
*                 level routine to write one character, but a faster     *
*                 implementation is often possible.                      *
*                 returns 0 on success, nonzero on error                 *
*                                                                        *
*  This routine should be customised.                                    *
*                                                                        *
*************************************************************************/
/*
COM0 U0C0 HOST stdin ASC0
int UART0_GetChar(void);
int UART0_PutChar(char * base, size_t siz);
IN  0
OUT 1
ERR 2
---------------------------------------------
COM1 U0C1 RS485 MainBus
int UART1_GetChar(void);
int UART1_PutChar(char * base, size_t siz);
IN  3
OUT 4
---------------------------------------------
COM2 U1C1 RS232 USB
int UART2_GetChar(void);
int UART2_PutChar(char * base, size_t siz);
IN  5
OUT 6
---------------------------------------------
COM3 U2C0 RS232 LAN
int UART3_GetChar(void);
int UART3_PutChar(char * base, size_t siz);
IN  7
OUT 7
---------------------------------------------
COM4 U2C1 RS232 Barcode
int UART4_GetChar(void);
int UART4_PutChar(char * base, size_t siz);
IN  8
OUT 8
---------------------------------------------
COM5 U1C0 RS232 Slave
int UART5_GetChar(void);
int UART5_PutChar(char * base, size_t siz);
IN  9
OUT 9
---------------------------------------------
*/
size_t _write( int fd, char * base, size_t size )
{
	size_t err = 0;

	switch(fd)
	{
		case 1:
		case 2:	err = UART0_PutChar(base, size);//COM0 U0C0 HOST stdin ASC0
				break;
		case 3:	err = UART0_PutChar(base, size);//COM0 U0C0 HOST stdin ASC0
				//falsch 3 ist read
				break;
		case 4:	err= UART1_PutChar(base, size);//COM1 U0C1 RS485 MainBus
				break;
		case 6:	err= UART2_PutChar(base, size);//COM2 U1C1 RS232 USB
				break;
		case 7:	err= UART3_PutChar(base, size);//COM3 U2C0 RS232 LAN
				break;
		case 8:	err= UART4_PutChar(base, size);//COM4 U2C1 RS232 Barcode
				break;	
		case 9:	err= UART5_PutChar(base, size);//COM5 U1C0 RS232 Slave
				break;	
	}
	return err;
}
