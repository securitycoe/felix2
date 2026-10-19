

/**************************************************************************
**                                                                        *
**  FILE        :  _iob.c                                                 *
**                                                                        *
**  DESCRIPTION :  Source file with _iob structure definitions            *
**                 The streams 'stdin', 'stdout' and 'stderr' are         *
**                 already opened and ready to use.                       *
**                                                                        *
**  Copyright 1996-2004 Altium BV                                         *
**                                                                        *
**************************************************************************/
#include "main.h"
#include <stdio.h>

#ifndef	STDIN_BUF
#define	STDIN_BUF	0
#endif

#ifndef	STDOUT_BUF
#define	STDOUT_BUF	0
#endif

#if	STDIN_BUF > 0
static	char	stdin_buf[STDIN_BUF];
#endif

#if	STDOUT_BUF > 0
static	char	stdout_buf[STDOUT_BUF];
#endif

FILE	_iob[_NFILE] =
{
	/* stdin COM0 U0C0 HOST stdin ASC0 */
#if	STDIN_BUF > 0
	{ stdin_buf,  stdin_buf,  0, STDIN_BUF,  _IORD + _IOLBF, 0 },
#else
	{ NULL,       NULL,       0, EOF,        _IORD + _IONBF, 0 },
#endif
	/* stdout COM0 U0C0 HOST stdin ASC0 */
#if	STDOUT_BUF > 0
	{ stdout_buf, stdout_buf, 0, STDOUT_BUF, _IOWR + _IOLBF, 1 },
#else
	{ NULL,       NULL,       0, EOF,        _IOWR + _IONBF, 1 },
#endif
	/* stderr */
	{ NULL,       NULL,       0, EOF,        _IOWR + _IONBF, 2 },

	/* COM1 U0C1 RS485 MainBus */
	{ NULL,       NULL,       0, EOF,        _IORD + _IONBF, 3 },
	/* COM1 U0C1 RS485 MainBus */
	{ NULL,       NULL,       0, EOF,        _IOWR + _IONBF, 4 },

	/* COM2 U1C1 RS232 USB */
	{ NULL,       NULL,       0, EOF,        _IORD + _IONBF, 5 },
	/* COM2 U1C1 RS232 USB */
	{ NULL,       NULL,       0, EOF,        _IOWR + _IONBF, 6 },

	/* COM3 U2C0 RS232 LAN */
	{ NULL,       NULL,       0, EOF,        _IORD + _IOWR +_IONBF, 7 },

	/* COM4 U2C1 RS232 Barcode */
	{ NULL,       NULL,       0, EOF,        _IORD + _IOWR + _IONBF, 8 },

	/* COM5 U1C0 RS232 Slave */
	{ NULL,       NULL,       0, EOF,        _IORD + _IOWR + _IONBF, 9 }

};
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

/*
 * Buffered input/output
 * ---------------------
 *
 * When a stream is unbuffered (_IONBF), the following conditions hold:
 *   _ptr    == NULL
 *   _base   == NULL
 *   _cnt    == 0
 *   _bufsiz is used as the "ungetc" buffer: EOF means empty
 *
 * When a stream is buffered, (_IOLBF or _IOFBF), the following holds:
 *   _base   is a valid buffer address
 *   _ptr    is a pointer into the buffer
 *   _bufsiz > 0
 *
 * After a buffered stream is opened and after a file positioning
 * operation, the following is true:
 *   _cnt    == 0
 *   _base   is a valid buffer address
 *   _ptr    == _base
 *
 * When reading from a buffered stream when _cnt == 0, the buffer will
 * be filled by _filbuf(). The _cnt member will be set to a negative
 * number to indicate that the buffer is currently used for input.
 * When (_cnt <= -1), the number of unread characters in the input
 * buffer is (-_cnt-1).
 *
 * When writing to a buffered stream for the first time (_cnt == 0),
 * _flsbuf() will write the character into the buffer and initialize
 * _cnt with (_bufsiz-1). As long as (_cnt > 0), output characters
 * are written into the buffer by putc() and _cnt is decremented for
 * each character. When a character is written when (_cnt == 0),
 * _flsbuf() will first write the buffer contents to the file and
 * than reinitialize _cnt. When (_cnt > 0), the stream is currently
 * used in output mode. When (_cnt == 0), it is either in output mode
 * or or it is unbuffered.
 *
 * On every file positioning operation, fflush() is called to write
 * the buffer to the file when the stream is in output mode, or to
 * discard the input buffer when the stream is in input mode.
 * Discarding the input buffer involves seeking back to the file
 * position of the first unread character. Although the ANSI-C
 * standard says that you may not switch from input to output mode
 * or vice versa, without an intervening file positioning operation,
 * this situation is easy to detect, and so _filbuf() and _flsbuf()
 * will call fflush() whenever such a switch is detected.
 */

/*
 * Close all open files. This function will be called from exit().
 */
void _doclose( void )
{
	int    i;

	for ( i = 0; i < _NFILE; i++ )
	{
		if ( _iob[i]._flag )
		{
			fclose( &_iob[i] );
		}
	}
}
