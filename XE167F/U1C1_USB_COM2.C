//****************************************************************************
// @Module        Universal Serial Interface Channel (USIC) U1C1
// @Filename      U1C1_USB_COM2.C
// @Project       30_5015_Felix.dav
//----------------------------------------------------------------------------
// @Controller    Infineon XE167F-96F66
//
// @Compiler      Tasking Classic
//
// @Codegenerator 2.2
//
// @Description   This file contains functions that use the U1C1 module.
//
//----------------------------------------------------------------------------
// @Date          07.03.2016 10:02:19
//
//****************************************************************************

// USER CODE BEGIN (U1C1_General,1)

// USER CODE END

/*
******************************************************************************
@Note

Com2 USB Schnittstelle (zum Host)

******************************************************************************
*/


//****************************************************************************
// @Project Includes
//****************************************************************************

#include "MAIN.H"

// USER CODE BEGIN (U1C1_General,2)
//#include "Shared/Stacker_Lifter_Allgemein/StacComm.h"
#include <string.h>
// USER CODE END


//****************************************************************************
// @Macros
//****************************************************************************

// USER CODE BEGIN (U1C1_General,3)
#define CheckAndWrap(HeadOrTail) (HeadOrTail = ((++HeadOrTail) >= (UART2_InpBuffer + UARTBUF)) ? UART2_InpBuffer : HeadOrTail);
// USER CODE END


//****************************************************************************
// @Defines
//****************************************************************************

// USER CODE BEGIN (U1C1_General,4)

// USER CODE END


//****************************************************************************
// @Typedefs
//****************************************************************************

// USER CODE BEGIN (U1C1_General,5)

// USER CODE END


//****************************************************************************
// @Imported Global Variables
//****************************************************************************

// USER CODE BEGIN (U1C1_General,6)
extern unsigned char UserMainSlot;
// USER CODE END


//****************************************************************************
// @Global Variables
//****************************************************************************

// USER CODE BEGIN (U1C1_General,7)
unsigned char UART2_InpBuffer[UARTBUF];
static unsigned char *UART2_Head= UART2_InpBuffer;		// head pointer for characters going into buffer.
static unsigned char *UART2_Tail= UART2_InpBuffer;		// tail pointer for characters going out of buffer.
//unsigned char g_UART2_ucCmdFlag= FALSE;
//unsigned char g_UART2_ucErrorFlag= FALSE;
unsigned short g_UART2_Events= COM_EVENTS_RESET;
//unsigned short g_UART2_CmdCnt= 0;
// USER CODE END


//****************************************************************************
// @External Prototypes
//****************************************************************************

// USER CODE BEGIN (U1C1_General,8)

// USER CODE END




//****************************************************************************
// @Prototypes Of Local Functions
//****************************************************************************

// USER CODE BEGIN (U1C1_General,9)
/* ******************************************************************************
 *	FUNCTION: 	SaveCharacter
 *	PARAMETERS:	zu speicherndes Zeichen
 *	DESCRIPTION:
 *	RETURNS: 	
 ******************************************************************************** */
static void SaveCharacter(unsigned char c)
{
	*UART2_Head = c;
	CheckAndWrap(UART2_Head);
}

/* ******************************************************************************
 *  FUNCTION:	UART2ResetDataBuffer
 *  PARAMETERS:	none
 *  DESCRIPTION:Setzt den Eingangspuffers Uart2 zürück
 *  RETURNS:	 
 ******************************************************************************** */
/*void UART2_ResetDataBufferAndErrorFlag(void)
{
	unsigned short a;
	_atomic(2);
	UART2_Tail = UART2_Head;
	*UART2_Head = 0;
//	g_UART2_ucErrorFlag=FALSE;
	g_UART2_Events = COM_EVENTS_RESET;
	
	for(a = 0; a < UARTBUF; a++) {WDO();UART2_InpBuffer[a] = 0;}
	UART2_Head = UART2_InpBuffer;
	//chLastInpChar = 0;
}*/
void UART2_ResetDataBufferAndErrorFlag(void)
{
	unsigned short a;
	char ch = U1C1_RBUF;		//Dummy-OP
	_atomic(2);
	UART2_Tail = UART2_Head;

//	g_UART2_ucErrorFlag=FALSE;
	g_UART2_Events = COM_EVENTS_RESET;
	
	for(a = 0; a < UARTBUF; a++) {WDO();UART2_InpBuffer[a] = 0;}
	UART2_Head = UART2_InpBuffer;
	*UART2_Head = 0;
	_atomic(2);
	UART2_Tail = UART2_Head;
	U1C1_PSCR   |= 0x8000;         // clear PSR_AIF
	U1C1_PSCR   |= 0x4000;         // clear PSR_RIF

}

// USER CODE END


//****************************************************************************
// @Function      void U1C1_ASC_vInit(void)
//
//----------------------------------------------------------------------------
// @Description   This is the initialization function of the U1C1 function
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

void U1C1_ASC_vInit(void)
{
  // USER CODE BEGIN (Init,2)

  // USER CODE END

  ///  -----------------------------------------------------------------------
  ///  U1C1 ASC is configured for Full-Duplex Operation Mode:
  ///  -----------------------------------------------------------------------

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U1C1 Fractional Divider:
  ///  -----------------------------------------------------------------------
  ///  - The Fractional divider is selected
  ///  - The step value STEP = 953

  U1C1_FDRL      =  0x83B9;      // load U1C1 fractional divider register

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U1C1 Baudrate Generator:
  ///  -----------------------------------------------------------------------
  ///  - The selected BaudRate is 19,200 kbaud
  ///  - The PreDivider for CTQ, PCTQ = 1
  ///  - The Denominator for CTQ, DCTQ = 15
  ///  - The Divider factor PDIV = 99

  U1C1_BRGL      =  0x3D00;      // load U1C1 load baud rate generator 
                                 // register L

  U1C1_BRGH      =  0x0063;      // load U1C1 load baud rate generator 
                                 // register H

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U1C1 Input Control Register 0:
  ///  -----------------------------------------------------------------------
  ///  - The data input DX0E is selected for pin - P6.0

  U1C1_DX0CR     =  0x0004;      // load U1C1 input control register(Data)

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U1C1 Interrupt Node Pointer Register:
  ///  -----------------------------------------------------------------------
  ///  - AINP pointer points to Interrupt node U1C1_0IC
  ///  - RINP pointer points to Interrupt node U1C1_0IC

  U1C1_INPRL     =  0x0000;      // load U1C1 Interrupt Node Pointer register 
                                 // L
  U1C1_INPRH     =  0x0000;      // load U1C1 Interrupt Node Pointer register 
                                 // H

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U1C1 Shift Control:
  ///  -----------------------------------------------------------------------
  ///  - Transmit/Receive LSB first is selected
  ///  - Transmission Mode (TRM) = 1
  ///  - Passive Data Level (PDL) = 1

  U1C1_SCTRL     =  0x0102;      // load U1C1 shift control register L

  ///  - Word Length (WLE) = 7
  ///  - Frame Length (FLE) = 7

  U1C1_SCTRH     =  0x0707;      // load U1C1 shift control register H

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U1C1 Transmit Control/Status Register:
  ///  -----------------------------------------------------------------------
  ///  - TBUF Data Enable (TDEN) = 1
  ///  - TBUF Data Single Shot Mode (TDSSM) = 1

  U1C1_TCSRL     =  0x0500;      // load U1C1 transmit control/status 
                                 // register L
  U1C1_TCSRH     =  0x0000;      // load U1C1 transmit control/status 
                                 // register H

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U1C1 Protocol Control Register:
  ///  -----------------------------------------------------------------------
  ///  - Sample Mode (SMD) = 1
  ///  - 2 Stop bits are selected
  ///  - Sample Point (SP) = 9
  ///  - Pulse Length (PL) = 0

  U1C1_PCRL      =  0x0903;      // load U1C1 protocol control register L
  U1C1_PCRH      =  0x0000;      // load U1C1 protocol control register H

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U1C1 Protocol Status Register:
  ///  -----------------------------------------------------------------------
  ///  - TBIF is set to simplify polling

  U1C1_PSR      |=  0x2000;      // load U1C1 protocol status register


  ///  -----------------------------------------------------------------------
  ///  Configuration of the used U1C1 Interrupts:
  ///  -----------------------------------------------------------------------
  ///  U1C10IC service request node configuration:
  ///  - U1C10IC interrupt priority level (ILVL) = 10
  ///  - U1C10IC interrupt group level (GLVL) = 1
  ///  - U1C10IC group priority extension (GPX) = 0

  U1C1_0IC       =  0x0069;     

  ///  -----------------------------------------------------------------------
  ///  Configuration of U1C1 FIFO:
  ///  -----------------------------------------------------------------------
  ///  -----------------------------------------------------------------------
  ///  Configuration of U1C1 Transmitter Buffer Conrol Register:
  ///  -----------------------------------------------------------------------
  ///  - Transmit FIFO buffer contains 16 entries
  ///  - Transmit FIFO buffer starts at Data Pointer 16
  ///  - Limit for transmit FIFO interrupt generation is 0

  U1C1_TBCTRL    =  0x0010;      // load U1C1 transmitter buffer control 
                                 // register L
  U1C1_TBCTRH    =  0x0400;      // load U1C1 transmitter buffer control 
                                 // register H

  ///  -----------------------------------------------------------------------
  ///  Configuration of U1C1 Receiver Buffer Conrol Register:
  ///  -----------------------------------------------------------------------
  ///  - Receive FIFO buffer contains 16 entries
  ///  - Receive FIFO buffer starts at Data Pointer 0
  ///  - Limit for receive FIFO interrupt generation is 0
  ///  - Filling level mode is selected
  ///  - Standard Receive buffer interrupt is enabled

  U1C1_RBCTRL    =  0x0000;      // load U1C1 receive buffer control register 
                                 // L
  U1C1_RBCTRH    =  0x4400;      // load U1C1 receive buffer control register 
                                 // H

  ///  -----------------------------------------------------------------------
  ///  Configuration of the used U1C1 Port Pins:
  ///  -----------------------------------------------------------------------
  P6_IOCR00      =  0x0020;      // load port register for data input

  ///  - P6.1 is used for USIC1 Channel1 shift Data output(DOUT)

  P6_IOCR01 = 0x00B0;    //set direction register



  // USER CODE BEGIN (U1C1_Function,3)

  // USER CODE END



} //  End of function U1C1_ASC_vInit


//****************************************************************************
// @Function      void U1C1_ASC_vi0IC(void) 
//
//----------------------------------------------------------------------------
// @Description   This is the interrupt service routine for the Service 
//                Request Node U1C1_0IC.
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

_interrupt(U1C1_0INT)  void U1C1_ASC_vi0IC(void)
{

  // USER CODE BEGIN (ASC0IC,2)

  // USER CODE END

  if (U1C1_PSR & 0x8000)         // Alternate receive interrupt flag
  {

  // USER CODE BEGIN (ASC0IC,3)
  	// wird durch Paritätsfehler ausgelöst
  	char ch;
  	//U1C1_TRBSCR= 0x4000;			//clear FIFO-Buffer
  	ch = U1C1_RBUF;
	U1C1_PSCR   |= 0x4000;         // clear PSR_RIF
//   	g_UART2_ucErrorFlag=TRUE;
	g_UART2_Events |= COM_EVENT_OVERFLOW;
	K_Intrp_Event_Signal(0, UserMainSlot, EVENT_MASTER_COM);
  // USER CODE END

  U1C1_PSCR   |= 0x8000;         // clear PSR_AIF
  }

  if (U1C1_PSR & 0x4000)         // Receive interrupt flag
  {

  // USER CODE BEGIN (ASC0IC,4)
	static char chLastInpChar = 0;
	char chInpChar = U1C1_RBUF;
//U1C1_IN00 = chInpChar;
	switch(chInpChar)
	{
		case '#'	:	if(chLastInpChar == chInpChar)
						{
							g_bBreakFlag = TRUE;
							//Drive_SetEvents(EVENT_USER_BREAK);
							g_UART2_Events |= COM_EVENT_USER_BREAK;
							//SaveCharacter(C_BREAK);		// damit BREAK in UserMain erkannt werden kann
							//SaveCharacter('\r');		// für 'buffered input' erforderlich
							chLastInpChar = 0;
							K_Intrp_Event_Signal(3, 0, EVENT_USER_BREAK);
							//g_UART2_ucCmdFlag=TRUE;
						}
						else
						{
							chLastInpChar = '#';
						}
						break;

		case '%'	:	//SaveCharacter('%');
						//SaveCharacter('\r');		// für 'buffered input' erforderlich
						chLastInpChar = '0';
						//g_UART2_ucCmdFlag=TRUE;
						g_UART2_Events |= COM_EVENT_SYNC;
						K_Intrp_Event_Signal(0, UserMainSlot, EVENT_MASTER_COM);
						break;
		case '\r'   :	//++g_UART2_CmdCnt;
						//SaveCharacter('\0');		// CR vorhanden, Commando komplett
				   		SaveCharacter('\0');		// CR vorhanden, Commando komplett
						chLastInpChar = '0';
						//g_UART2_ucCmdFlag=TRUE;
						g_UART2_Events |= COM_EVENT_CR;
						K_Intrp_Event_Signal(0, UserMainSlot, EVENT_MASTER_COM);
						break;
		case '\n'   :	break;
	
		default		:	SaveCharacter(chInpChar);
						chLastInpChar = '0';
						break;
	}

  // USER CODE END

  U1C1_PSCR   |= 0x4000;         // clear PSR_RIF
  }

  // USER CODE BEGIN (ASC0IC,15)
	K_OS_Intrp_Exit();						// no call right CMX interrupt exit routine. MUST be
											// last instruction within interrupt's code

  // USER CODE END

} //  End of function U1C1_ASC_vi0IC




// USER CODE BEGIN (U1C1_General,10)
/* ******************************************************************************
 *  FUNCTION:	UART2_GetChar
 *  PARAMETERS:	none
 *  DESCRIPTION:liest ein Zeichen aus dem stdin-buffer
 *  RETURNS:	- character from stdin-buffer
 *              - EOF wenn kein Zeichen eingegangen ist
 ******************************************************************************** */
int UART2_GetChar(void)
{
	register int Char = 0;

	//if(UART2_Tail != UART2_Head)
	if(UART2_Tail < UART2_Head)
	{
		Char = *UART2_Tail;
		CheckAndWrap(UART2_Tail);
	}else
	{
		Char = EOF;
	}
	return Char;
}
int Uart2_kbHit(void)
{
	if(UART2_Tail < UART2_Head)
	{
		return TRUE;

	}else
	{
		return FALSE;
	}
}

/* ******************************************************************************
 *  FUNCTION: UART2_PutChar
 *
 *  PARAMETERS:	Pointer auf ZK, Anzahl der Elemente
 *
 *  DESCRIPTION: schreibt Zeichen auf ASC2
 *
 *  RETURNS:  Anzahl der geschriebenen Zeichen	
 ******************************************************************************** */
int UART2_PutChar(char * base, size_t siz) 
{  unsigned int i;
   for(i=0; i<siz; i++)
   { 
       while(U1C1_ASC_ubIsTxFIFOfull())
       { 
       	  WDO();	/* falls Buffer voll ist, dann warten */
	   }
	   U1C1_IN00= *base;  base++;
   }
   return siz;	
}

_bit getCTS_COM2_UART2(void)
{
	return U1C1_COM2_CTS2;
}

void setRTS_COM2_UART2(unsigned short Value)
{
	// hight active
	U1C1_COM2_RTS2 = Value;
}

// USER CODE END

