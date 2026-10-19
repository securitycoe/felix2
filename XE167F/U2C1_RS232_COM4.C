//****************************************************************************
// @Module        Universal Serial Interface Channel (USIC) U2C1
// @Filename      U2C1_RS232_COM4.C
// @Project       30_5015_Felix.dav
//----------------------------------------------------------------------------
// @Controller    Infineon XE167F-96F66
//
// @Compiler      Tasking Classic
//
// @Codegenerator 2.2
//
// @Description   This file contains functions that use the U2C1 module.
//
//----------------------------------------------------------------------------
// @Date          07.03.2016 10:02:22
//
//****************************************************************************

// USER CODE BEGIN (U2C1_General,1)

// USER CODE END

/*
******************************************************************************
@Note

Com4 RS232 Barcode

******************************************************************************
*/


//****************************************************************************
// @Project Includes
//****************************************************************************

#include "MAIN.H"

// USER CODE BEGIN (U2C1_General,2)
//#include "Shared/Stacker_Lifter_Allgemein/StacComm.h"
#include <string.h>
// USER CODE END


//****************************************************************************
// @Macros
//****************************************************************************

// USER CODE BEGIN (U2C1_General,3)
#define CheckAndWrap(HeadOrTail) (HeadOrTail = ((++HeadOrTail) >= (UART4_InpBuffer + UARTBUF)) ? UART4_InpBuffer : HeadOrTail);
// USER CODE END


//****************************************************************************
// @Defines
//****************************************************************************

// USER CODE BEGIN (U2C1_General,4)

// USER CODE END


//****************************************************************************
// @Typedefs
//****************************************************************************

// USER CODE BEGIN (U2C1_General,5)

// USER CODE END


//****************************************************************************
// @Imported Global Variables
//****************************************************************************

// USER CODE BEGIN (U2C1_General,6)
extern unsigned char UserMainSlot;
extern unsigned char ASyncCtrlSlot;
// USER CODE END


//****************************************************************************
// @Global Variables
//****************************************************************************

// USER CODE BEGIN (U2C1_General,7)
unsigned char UART4_InpBuffer[UARTBUF];
static unsigned char *UART4_Head= UART4_InpBuffer;		// head pointer for characters going into buffer.
static unsigned char *UART4_Tail= UART4_InpBuffer;		// tail pointer for characters going out of buffer.
//unsigned char g_UART4_ucCmdFlag= FALSE;
//unsigned char g_UART4_ucErrorFlag= FALSE;
unsigned short g_UART4_Events= COM_EVENTS_RESET;
//unsigned short g_UART4_CmdCnt= 0;

// USER CODE END


//****************************************************************************
// @External Prototypes
//****************************************************************************

// USER CODE BEGIN (U2C1_General,8)

// USER CODE END




//****************************************************************************
// @Prototypes Of Local Functions
//****************************************************************************

// USER CODE BEGIN (U2C1_General,9)
/* ******************************************************************************
 *	FUNCTION: 	SaveCharacter
 *	PARAMETERS:	zu speicherndes Zeichen
 *	DESCRIPTION:
 *	RETURNS: 	
 ******************************************************************************** */
static void SaveCharacter(unsigned char c)
{
	*UART4_Head = c;
	CheckAndWrap(UART4_Head);
}

/* ******************************************************************************
 *  FUNCTION:	UART4ResetDataBuffer
 *  PARAMETERS:	none
 *  DESCRIPTION:Setzt den Eingangspuffers Uart0 zürück
 *  RETURNS:	 
 ******************************************************************************** */
/*void UART4_ResetDataBufferAndErrorFlag(void)
{
	unsigned short a;
	_atomic(2);
	UART4_Tail = UART4_Head;
	*UART4_Head = 0;
//	g_UART4_ucErrorFlag=FALSE;
	g_UART4_Events = COM_EVENTS_RESET;
	
	for(a = 0; a < UARTBUF; a++) {WDO();UART4_InpBuffer[a] = 0;}
	UART4_Head = UART4_InpBuffer;
}*/

void UART4_ResetDataBufferAndErrorFlag(void)
{
	unsigned short a;
//	UART4_Tail = UART4_Head;

//	g_UART4_ucErrorFlag=FALSE;
	g_UART4_Events = COM_EVENTS_RESET;
	
	for(a = 0; a < UARTBUF; a++) {WDO();UART4_InpBuffer[a] = 0;}
	UART4_Head = UART4_InpBuffer;
	*UART4_Head = 0;
	_atomic(2);
	UART4_Tail = UART4_Head;
	//chLastInpChar = 0;
}

// USER CODE END


//****************************************************************************
// @Function      void U2C1_ASC_vInit(void)
//
//----------------------------------------------------------------------------
// @Description   This is the initialization function of the U2C1 function
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

void U2C1_ASC_vInit(void)
{
  // USER CODE BEGIN (Init,2)

  // USER CODE END

  ///  -----------------------------------------------------------------------
  ///  U2C1 ASC is configured for Full-Duplex Operation Mode:
  ///  -----------------------------------------------------------------------

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U2C1 Fractional Divider:
  ///  -----------------------------------------------------------------------
  ///  - The Fractional divider is selected
  ///  - The step value STEP = 477

  U2C1_FDRL      =  0x81DD;      // load U2C1 fractional divider register

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U2C1 Baudrate Generator:
  ///  -----------------------------------------------------------------------
  ///  - The selected BaudRate is 9,600 kbaud
  ///  - The PreDivider for CTQ, PCTQ = 1
  ///  - The Denominator for CTQ, DCTQ = 15
  ///  - The Divider factor PDIV = 99

  U2C1_BRGL      =  0x3D00;      // load U2C1 load baud rate generator 
                                 // register L

  U2C1_BRGH      =  0x0063;      // load U2C1 load baud rate generator 
                                 // register H

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U2C1 Input Control Register 0:
  ///  -----------------------------------------------------------------------
  ///  - The data input DX0B is selected for pin - P3.7

  U2C1_DX0CR     =  0x0001;      // load U2C1 input control register(Data)

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U2C1 Interrupt Node Pointer Register:
  ///  -----------------------------------------------------------------------
  ///  - AINP pointer points to Interrupt node U2C1_0IC
  ///  - RINP pointer points to Interrupt node U2C1_0IC

  U2C1_INPRL     =  0x0000;      // load U2C1 Interrupt Node Pointer register 
                                 // L
  U2C1_INPRH     =  0x0000;      // load U2C1 Interrupt Node Pointer register 
                                 // H

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U2C1 Shift Control:
  ///  -----------------------------------------------------------------------
  ///  - Transmit/Receive LSB first is selected
  ///  - Transmission Mode (TRM) = 1
  ///  - Passive Data Level (PDL) = 1

  U2C1_SCTRL     =  0x0102;      // load U2C1 shift control register L

  ///  - Word Length (WLE) = 7
  ///  - Frame Length (FLE) = 7

  U2C1_SCTRH     =  0x0707;      // load U2C1 shift control register H

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U2C1 Transmit Control/Status Register:
  ///  -----------------------------------------------------------------------
  ///  - TBUF Data Enable (TDEN) = 1
  ///  - TBUF Data Single Shot Mode (TDSSM) = 1

  U2C1_TCSRL     =  0x0500;      // load U2C1 transmit control/status 
                                 // register L
  U2C1_TCSRH     =  0x0000;      // load U2C1 transmit control/status 
                                 // register H

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U2C1 Protocol Control Register:
  ///  -----------------------------------------------------------------------
  ///  - Sample Mode (SMD) = 1
  ///  - 1 Stop bit is selected
  ///  - Sample Point (SP) = 7
  ///  - Pulse Length (PL) = 0

  U2C1_PCRL      =  0x0701;      // load U2C1 protocol control register L
  U2C1_PCRH      =  0x0000;      // load U2C1 protocol control register H

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U2C1 Protocol Status Register:
  ///  -----------------------------------------------------------------------
  ///  - TBIF is set to simplify polling

  U2C1_PSR      |=  0x2000;      // load U2C1 protocol status register


  ///  -----------------------------------------------------------------------
  ///  Configuration of the used U2C1 Interrupts:
  ///  -----------------------------------------------------------------------
  ///  U2C10IC service request node configuration:
  ///  - U2C10IC interrupt priority level (ILVL) = 8
  ///  - U2C10IC interrupt group level (GLVL) = 1
  ///  - U2C10IC group priority extension (GPX) = 0

  U2C1_0IC       =  0x0061;     

  ///  -----------------------------------------------------------------------
  ///  Configuration of U2C1 FIFO:
  ///  -----------------------------------------------------------------------
  ///  -----------------------------------------------------------------------
  ///  Configuration of U2C1 Transmitter Buffer Conrol Register:
  ///  -----------------------------------------------------------------------
  ///  - Transmit FIFO buffer contains 16 entries
  ///  - Transmit FIFO buffer starts at Data Pointer 16
  ///  - Limit for transmit FIFO interrupt generation is 0

  U2C1_TBCTRL    =  0x0010;      // load U2C1 transmitter buffer control 
                                 // register L
  U2C1_TBCTRH    =  0x0400;      // load U2C1 transmitter buffer control 
                                 // register H

  ///  -----------------------------------------------------------------------
  ///  Configuration of U2C1 Receiver Buffer Conrol Register:
  ///  -----------------------------------------------------------------------
  ///  - Receive FIFO buffer contains 16 entries
  ///  - Receive FIFO buffer starts at Data Pointer 0
  ///  - Limit for receive FIFO interrupt generation is 0
  ///  - Filling level mode is selected
  ///  - Standard Receive buffer interrupt is enabled

  U2C1_RBCTRL    =  0x0000;      // load U2C1 receive buffer control register 
                                 // L
  U2C1_RBCTRH    =  0x4400;      // load U2C1 receive buffer control register 
                                 // H

  ///  -----------------------------------------------------------------------
  ///  Configuration of the used U2C1 Port Pins:
  ///  -----------------------------------------------------------------------
  P3_IOCR07      =  0x0020;      // load port register for data input

  ///  - P3.6 is used for USIC2Channel1 Shift Data output(DOUT)

  P3_IOCR06 = 0x0090;    //set direction register



  // USER CODE BEGIN (U2C1_Function,3)

  // USER CODE END



} //  End of function U2C1_ASC_vInit


//****************************************************************************
// @Function      void U2C1_ASC_vi0IC(void) 
//
//----------------------------------------------------------------------------
// @Description   This is the interrupt service routine for the Service 
//                Request Node U2C1_0IC.
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

_interrupt(U2C1_0INT)  void U2C1_ASC_vi0IC(void)
{

  // USER CODE BEGIN (ASC0IC,2)

  // USER CODE END

  if (U2C1_PSR & 0x8000)         // Alternate receive interrupt flag
  {

  // USER CODE BEGIN (ASC0IC,3)
  	// wird durch Paritätsfehler ausgelöst
  	char ch;
  	//U0C0_TRBSCR= 0x4000;			//clear FIFO-Buffer
  	ch = U2C1_RBUF;
	U2C1_PSCR   |= 0x4000;         // clear PSR_RIF
   	//g_UART5_ucErrorFlag=TRUE;
	g_UART4_Events |= COM_EVENT_OVERFLOW;
	K_Intrp_Event_Signal(0, UserMainSlot, EVENT_BARC_COM);
	K_Intrp_Event_Signal(0, ASyncCtrlSlot, EVENT_BARC_COM);

  // USER CODE END

  U2C1_PSCR   |= 0x8000;         // clear PSR_AIF
  }

  if (U2C1_PSR & 0x4000)         // Receive interrupt flag
  {

  // USER CODE BEGIN (ASC0IC,4)
	static char chLastInpChar = 0;
	char chInpChar = U2C1_RBUF;
//U2C1_IN00 = chInpChar;
	switch(chInpChar)
	{
/*		case RDY	:
						SaveCharacter(chInpChar);
						chLastInpChar = '0';
						//g_UART5_ucCmdFlag=TRUE;
						g_UART4_Events |= COM_EVENT_CR;
						K_Intrp_Event_Signal(0, UserMainSlot, EVENT_BARC_COM);
						break;*/

		case '\n'   :	//++g_UART5_CmdCnt;
						//SaveCharacter('\r');		// CR vorhanden, Commando komplett
						SaveCharacter('\0');		// CR vorhanden, Commando komplett
						chLastInpChar = '0';
						//g_UART5_ucCmdFlag=TRUE;
						g_UART4_Events |= COM_EVENT_CR;
						K_Intrp_Event_Signal(0, UserMainSlot, EVENT_BARC_COM);
						K_Intrp_Event_Signal(0, ASyncCtrlSlot, EVENT_BARC_COM);
						break;
		case '\r'   :	
						break;
		default		:	SaveCharacter(chInpChar);
						chLastInpChar = '0';
						break;
	}

  // USER CODE END

  U2C1_PSCR   |= 0x4000;         // clear PSR_RIF
  }

  // USER CODE BEGIN (ASC0IC,15)
	K_OS_Intrp_Exit();						// no call right CMX interrupt exit routine. MUST be
											// last instruction within interrupt's code

  // USER CODE END

} //  End of function U2C1_ASC_vi0IC




// USER CODE BEGIN (U2C1_General,10)
/* ******************************************************************************
 *  FUNCTION:	UART4_GetChar
 *  PARAMETERS:	none
 *  DESCRIPTION:liest ein Zeichen aus dem stdin-buffer
 *  RETURNS:	- character from stdin-buffer
 *              - EOF wenn kein Zeichen eingegangen ist
 ******************************************************************************** */
int UART4_GetChar(void)
{
	register int Char;

	//if(UART4_Tail != UART4_Head)
	if(UART4_Tail < UART4_Head)
	{
		Char = *UART4_Tail;
		CheckAndWrap(UART4_Tail);
	}else
	{
		Char = EOF;
	}
	return Char;
}
int Uart4_kbHit(void)
{
	if(UART4_Tail < UART4_Head)
	{
		return TRUE;

	}else
	{
		return FALSE;
	}
}

/* ******************************************************************************
 *  FUNCTION: UART4_PutChar
 *
 *  PARAMETERS:	Pointer auf ZK, Anzahl der Elemente
 *
 *  DESCRIPTION: schreibt Zeichen auf ASC0
 *
 *  RETURNS:  Anzahl der geschriebenen Zeichen	
 ******************************************************************************** */
int UART4_PutChar(char * base, size_t siz) 
{  unsigned int i;
   for(i=0; i<siz; i++)
   { 
       while(U2C1_ASC_ubIsTxFIFOfull())
       { 
       	  WDO();	/* falls Buffer voll ist, dann warten */
	   }
	   U2C1_IN00= *base;  base++;
   }
   return siz;	
}

_bit getCTS_COM4_UART4(void)
{
	return U2C1_COM4_CTS4;
}

void setRTS_COM4_UART4(unsigned short Value)
{
	// hight active
	U2C1_COM4_RTS4 = Value;
}

// USER CODE END

