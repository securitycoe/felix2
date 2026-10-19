//****************************************************************************
// @Module        Universal Serial Interface Channel (USIC) U2C0
// @Filename      U2C0_LAN_COM3.C
// @Project       30_5015_Felix.dav
//----------------------------------------------------------------------------
// @Controller    Infineon XE167F-96F66
//
// @Compiler      Tasking Classic
//
// @Codegenerator 2.2
//
// @Description   This file contains functions that use the U2C0 module.
//
//----------------------------------------------------------------------------
// @Date          07.03.2016 10:02:21
//
//****************************************************************************

// USER CODE BEGIN (U2C0_General,1)

// USER CODE END

/*
******************************************************************************
@Note

Com3 LAN (zumHost)

******************************************************************************
*/


//****************************************************************************
// @Project Includes
//****************************************************************************

#include "MAIN.H"

// USER CODE BEGIN (U2C0_General,2)
//#include "Shared/Stacker_Lifter_Allgemein/StacComm.h"
#include <string.h>
// USER CODE END


//****************************************************************************
// @Macros
//****************************************************************************

// USER CODE BEGIN (U2C0_General,3)
#define CheckAndWrap(HeadOrTail) (HeadOrTail = ((++HeadOrTail) >= (UART3_InpBuffer + UARTBUF)) ? UART3_InpBuffer : HeadOrTail);
// USER CODE END


//****************************************************************************
// @Defines
//****************************************************************************

// USER CODE BEGIN (U2C0_General,4)

// USER CODE END


//****************************************************************************
// @Typedefs
//****************************************************************************

// USER CODE BEGIN (U2C0_General,5)

// USER CODE END


//****************************************************************************
// @Imported Global Variables
//****************************************************************************

// USER CODE BEGIN (U2C0_General,6)
extern unsigned char UserMainSlot;
// USER CODE END


//****************************************************************************
// @Global Variables
//****************************************************************************

// USER CODE BEGIN (U2C0_General,7)
unsigned char UART3_InpBuffer[UARTBUF];
static unsigned char *UART3_Head= UART3_InpBuffer;		// head pointer for characters going into buffer.
static unsigned char *UART3_Tail= UART3_InpBuffer;		// tail pointer for characters going out of buffer.
//unsigned char g_UART3_ucCmdFlag= FALSE;
//unsigned char g_UART3_ucErrorFlag= FALSE;
unsigned short g_UART3_Events= COM_EVENTS_RESET;
//unsigned short g_UART3_CmdCnt= 0;

// USER CODE END


//****************************************************************************
// @External Prototypes
//****************************************************************************

// USER CODE BEGIN (U2C0_General,8)

// USER CODE END




//****************************************************************************
// @Prototypes Of Local Functions
//****************************************************************************

// USER CODE BEGIN (U2C0_General,9)
/* ******************************************************************************
 *	FUNCTION: 	SaveCharacter
 *	PARAMETERS:	zu speicherndes Zeichen
 *	DESCRIPTION:
 *	RETURNS: 	
 ******************************************************************************** */
static void SaveCharacter(unsigned char c)
{
	*UART3_Head = c;
	CheckAndWrap(UART3_Head);
}

/* ******************************************************************************
 *  FUNCTION:	UART3ResetDataBuffer
 *  PARAMETERS:	none
 *  DESCRIPTION:Setzt den Eingangspuffers Uart0 zürück
 *  RETURNS:	 
 ******************************************************************************** */
/*void UART3_ResetDataBufferAndErrorFlag(void)
{
	unsigned short a;
	_atomic(2);
	UART3_Tail = UART3_Head;
	*UART3_Head = 0;
//	g_UART3_ucErrorFlag=FALSE;
	g_UART3_Events = COM_EVENTS_RESET;
	
	for(a = 0; a < UARTBUF; a++) {WDO();UART3_InpBuffer[a] = 0;}
	UART3_Head = UART3_InpBuffer;
}*/

void UART3_ResetDataBufferAndErrorFlag(void)
{
	unsigned short a;
	char ch = U2C0_RBUF;		//Dummy-OP
	_atomic(2);
	UART3_Tail = UART3_Head;

//	g_UART3_ucErrorFlag=FALSE;
	g_UART3_Events = COM_EVENTS_RESET;
	
	for(a = 0; a < UARTBUF; a++) {WDO();UART3_InpBuffer[a] = 0;}
	UART3_Head = UART3_InpBuffer;
	*UART3_Head = 0;
	_atomic(2);
	UART3_Tail = UART3_Head;
	U2C0_PSCR   |= 0x8000;         // clear PSR_AIF
	U2C0_PSCR   |= 0x4000;         // clear PSR_RIF

	//chLastInpChar = 0;
}

// USER CODE END


//****************************************************************************
// @Function      void U2C0_ASC_vInit(void)
//
//----------------------------------------------------------------------------
// @Description   This is the initialization function of the U2C0 function
//                library. It is assumed that the SFRs used by this library
//                are in reset state.
//
//----------------------------------------------------------------------------
// @Returnvalue   None
//
//----------------------------------------------------------------------------
// @Parameters    None
//
//----------------------------------------------------------------------------
// @Date          07.03.2016
//
//****************************************************************************

// USER CODE BEGIN (Init,1)

// USER CODE END

void U2C0_ASC_vInit(void)
{
  // USER CODE BEGIN (Init,2)

  // USER CODE END

  ///  -----------------------------------------------------------------------
  ///  U2C0 ASC is configured for Full-Duplex Operation Mode:
  ///  -----------------------------------------------------------------------

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U2C0 Fractional Divider:
  ///  -----------------------------------------------------------------------
  ///  - The Fractional divider is selected
  ///  - The step value STEP = 953

  U2C0_FDRL      =  0x83B9;      // load U2C0 fractional divider register

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U2C0 Baudrate Generator:
  ///  -----------------------------------------------------------------------
  ///  - The selected BaudRate is 19,200 kbaud
  ///  - The PreDivider for CTQ, PCTQ = 1
  ///  - The Denominator for CTQ, DCTQ = 15
  ///  - The Divider factor PDIV = 99

  U2C0_BRGL      =  0x3D00;      // load U2C0 load baud rate generator 
                                 // register L

  U2C0_BRGH      =  0x0063;      // load U2C0 load baud rate generator 
                                 // register H

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U2C0 Input Control Register 0:
  ///  -----------------------------------------------------------------------
  ///  - The data input DX0C is selected for pin - P1.5

  U2C0_DX0CR     =  0x0002;      // load U2C0 input control register(Data)

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U2C0 Interrupt Node Pointer Register:
  ///  -----------------------------------------------------------------------
  ///  - AINP pointer points to Interrupt node U2C0_0IC
  ///  - RINP pointer points to Interrupt node U2C0_0IC

  U2C0_INPRL     =  0x0000;      // load U2C0 Interrupt Node Pointer register 
                                 // L
  U2C0_INPRH     =  0x0000;      // load U2C0 Interrupt Node Pointer register 
                                 // H

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U2C0 Shift Control:
  ///  -----------------------------------------------------------------------
  ///  - Transmit/Receive LSB first is selected
  ///  - Transmission Mode (TRM) = 1
  ///  - Passive Data Level (PDL) = 1

  U2C0_SCTRL     =  0x0102;      // load U2C0 shift control register L

  ///  - Word Length (WLE) = 7
  ///  - Frame Length (FLE) = 7

  U2C0_SCTRH     =  0x0707;      // load U2C0 shift control register H

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U2C0 Transmit Control/Status Register:
  ///  -----------------------------------------------------------------------
  ///  - TBUF Data Enable (TDEN) = 1
  ///  - TBUF Data Single Shot Mode (TDSSM) = 1

  U2C0_TCSRL     =  0x0500;      // load U2C0 transmit control/status 
                                 // register L
  U2C0_TCSRH     =  0x0000;      // load U2C0 transmit control/status 
                                 // register H

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U2C0 Protocol Control Register:
  ///  -----------------------------------------------------------------------
  ///  - Sample Mode (SMD) = 1
  ///  - 2 Stop bits are selected
  ///  - Sample Point (SP) = 9
  ///  - Pulse Length (PL) = 0

  U2C0_PCRL      =  0x0903;      // load U2C0 protocol control register L
  U2C0_PCRH      =  0x0000;      // load U2C0 protocol control register H

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U2C0 Protocol Status Register:
  ///  -----------------------------------------------------------------------
  ///  - TBIF is set to simplify polling

  U2C0_PSR      |=  0x2000;      // load U2C0 protocol status register


  ///  -----------------------------------------------------------------------
  ///  Configuration of the used U2C0 Interrupts:
  ///  -----------------------------------------------------------------------
  ///  U2C00IC service request node configuration:
  ///  - U2C00IC interrupt priority level (ILVL) = 10
  ///  - U2C00IC interrupt group level (GLVL) = 2
  ///  - U2C00IC group priority extension (GPX) = 0

  U2C0_0IC       =  0x006A;     

  ///  -----------------------------------------------------------------------
  ///  Configuration of U2C0 FIFO:
  ///  -----------------------------------------------------------------------
  ///  -----------------------------------------------------------------------
  ///  Configuration of U2C0 Transmitter Buffer Conrol Register:
  ///  -----------------------------------------------------------------------
  ///  - Transmit FIFO buffer contains 16 entries
  ///  - Transmit FIFO buffer starts at Data Pointer 32
  ///  - Limit for transmit FIFO interrupt generation is 0

  U2C0_TBCTRL    =  0x0020;      // load U2C0 transmitter buffer control 
                                 // register L
  U2C0_TBCTRH    =  0x0400;      // load U2C0 transmitter buffer control 
                                 // register H

  ///  -----------------------------------------------------------------------
  ///  Configuration of U2C0 Receiver Buffer Conrol Register:
  ///  -----------------------------------------------------------------------
  ///  - Receive FIFO buffer contains 16 entries
  ///  - Receive FIFO buffer starts at Data Pointer 48
  ///  - Limit for receive FIFO interrupt generation is 0
  ///  - Filling level mode is selected
  ///  - Standard Receive buffer interrupt is enabled

  U2C0_RBCTRL    =  0x0030;      // load U2C0 receive buffer control register 
                                 // L
  U2C0_RBCTRH    =  0x4400;      // load U2C0 receive buffer control register 
                                 // H

  ///  -----------------------------------------------------------------------
  ///  Configuration of the used U2C0 Port Pins:
  ///  -----------------------------------------------------------------------
  P1_IOCR05      =  0x0020;      // load port register for data input

  ///  - P1.6 is used for USIC2 Channel0 Shift Data output(DOUT)

  P1_IOCR06 = 0x00B0;    //set direction register



  // USER CODE BEGIN (U2C0_Function,3)

  // USER CODE END



} //  End of function U2C0_ASC_vInit


//****************************************************************************
// @Function      void U2C0_ASC_vi0IC(void) 
//
//----------------------------------------------------------------------------
// @Description   This is the interrupt service routine for the Service 
//                Request Node U2C0_0IC.
//                Please note that you have to add application specific code 
//                to this function.
//
//----------------------------------------------------------------------------
// @Returnvalue   None
//
//----------------------------------------------------------------------------
// @Parameters    None
//
//----------------------------------------------------------------------------
// @Date          07.03.2016
//
//****************************************************************************

// USER CODE BEGIN (ASC0IC,1)

// USER CODE END

_interrupt(U2C0_0INT)  void U2C0_ASC_vi0IC(void)
{

  // USER CODE BEGIN (ASC0IC,2)

  // USER CODE END

  if (U2C0_PSR & 0x8000)         // Alternate receive interrupt flag
  {

  // USER CODE BEGIN (ASC0IC,3)
  	// wird durch Paritätsfehler ausgelöst
  	char ch;
  	//U2C0_TRBSCR= 0x4000;			//clear FIFO-Buffer
  	ch = U2C0_RBUF;
	U2C0_PSCR   |= 0x4000;         // clear PSR_RIF

   	//g_UART3_ucErrorFlag=TRUE;
	g_UART3_Events |= COM_EVENT_OVERFLOW;
	K_Intrp_Event_Signal(0, UserMainSlot, EVENT_MASTER_COM);
  // USER CODE END

  U2C0_PSCR   |= 0x8000;         // clear PSR_AIF
  }

  if (U2C0_PSR & 0x4000)         // Receive interrupt flag
  {

  // USER CODE BEGIN (ASC0IC,4)
	static char chLastInpChar = 0;
	char chInpChar = U2C0_RBUF;
//U2C0_IN00 = chInpChar;
	switch(chInpChar)
	{
		case '#'	:	if(chLastInpChar == chInpChar)
						{
							g_bBreakFlag = TRUE;
							//Drive_SetEvents(EVENT_USER_BREAK);
							g_UART3_Events |= COM_EVENT_USER_BREAK;
							//SaveCharacter(C_BREAK);		// damit BREAK in UserMain erkannt werden kann
							//SaveCharacter('\r');		// für 'buffered input' erforderlich
							chLastInpChar = 0;
							K_Intrp_Event_Signal(3, 0, EVENT_USER_BREAK);
							//g_UART3_ucCmdFlag=TRUE;
						}
						else
						{
							chLastInpChar = '#';
						}
						break;
 
		case '%'	:	//SaveCharacter('%');
						//SaveCharacter('\r');		// für 'buffered input' erforderlich
						chLastInpChar = '0';
						//g_UART3_ucCmdFlag=TRUE;
						g_UART3_Events |= COM_EVENT_SYNC;
						K_Intrp_Event_Signal(0, UserMainSlot, EVENT_MASTER_COM);
						break;
		case '\r'   :	//++g_UART3_CmdCnt;
						SaveCharacter('\0');		// CR vorhanden, Commando komplett
						//SaveCharacter('\0');		// CR vorhanden, Commando komplett
						chLastInpChar = '0';
						//g_UART3_ucCmdFlag=TRUE;
						g_UART3_Events |= COM_EVENT_CR;
						K_Intrp_Event_Signal(0, UserMainSlot, EVENT_MASTER_COM);
						break;
		case '\n'   :	break;
		default		:	SaveCharacter(chInpChar);
						chLastInpChar = '0';
						break;
	}

  // USER CODE END

  U2C0_PSCR   |= 0x4000;         // clear PSR_RIF
  }

  // USER CODE BEGIN (ASC0IC,15)
	K_OS_Intrp_Exit();						// no call right CMX interrupt exit routine. MUST be
											// last instruction within interrupt's code

  // USER CODE END

} //  End of function U2C0_ASC_vi0IC




// USER CODE BEGIN (U2C0_General,10)
/* ******************************************************************************
 *  FUNCTION:	UART3_GetChar
 *  PARAMETERS:	none
 *  DESCRIPTION:liest ein Zeichen aus dem stdin-buffer
 *  RETURNS:	- character from stdin-buffer
 *              - EOF wenn kein Zeichen eingegangen ist
 ******************************************************************************** */
int UART3_GetChar(void)
{
	register int Char;

	//if(UART3_Tail != UART3_Head)
	if(UART3_Tail < UART3_Head)
	{
		Char = *UART3_Tail;
		CheckAndWrap(UART3_Tail);
	}else
	{
		Char = EOF;
	}
	return Char;
}
int Uart3_kbHit(void)
{
	if(UART3_Tail < UART3_Head)
	{
		return TRUE;

	}else
	{
		return FALSE;
	}
}

/* ******************************************************************************
 *  FUNCTION: UART3_PutChar
 *
 *  PARAMETERS:	Pointer auf ZK, Anzahl der Elemente
 *
 *  DESCRIPTION: schreibt Zeichen auf ASC0
 *
 *  RETURNS:  Anzahl der geschriebenen Zeichen	
 ******************************************************************************** */
int UART3_PutChar(char * base, size_t siz) 
{  unsigned int i;
   for(i=0; i<siz; i++)
   { 
       while(U2C0_ASC_ubIsTxFIFOfull())
       { 
       	  WDO();	/* falls Buffer voll ist, dann warten */
	   }
	   U2C0_IN00= *base;  base++;
   }
   return siz;	
}

_bit getCTS_COM3_UART3(void)
{
	return U2C0_COM3_CTS3;
}

void setRTS_COM3_UART3(unsigned short Value)
{
	// hight active
	U2C0_COM3_RTS3 = Value;
}

// USER CODE END

