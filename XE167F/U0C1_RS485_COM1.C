//****************************************************************************
// @Module        Universal Serial Interface Channel (USIC) U0C1
// @Filename      U0C1_RS485_COM1.C
// @Project       30_5015_Felix.dav
//----------------------------------------------------------------------------
// @Controller    Infineon XE167F-96F66
//
// @Compiler      Tasking Classic
//
// @Codegenerator 2.2
//
// @Description   This file contains functions that use the U0C1 module.
//
//----------------------------------------------------------------------------
// @Date          07.03.2016 10:02:17
//
//****************************************************************************

// USER CODE BEGIN (U0C1_General,1)

// USER CODE END

/*
******************************************************************************
@Note

Com1 XS300/301 RS485 (IMs Motore)

******************************************************************************
*/


//****************************************************************************
// @Project Includes
//****************************************************************************

#include "MAIN.H"

// USER CODE BEGIN (U0C1_General,2)
//#include "Shared/Stacker_Lifter_Allgemein/StacComm.h"
#include <string.h>
// USER CODE END


//****************************************************************************
// @Macros
//****************************************************************************

// USER CODE BEGIN (U0C1_General,3)
#define CheckAndWrap(HeadOrTail) (HeadOrTail = ((++HeadOrTail) >= (UART1_InpBuffer + UARTBUF)) ? UART1_InpBuffer : HeadOrTail);
// USER CODE END


//****************************************************************************
// @Defines
//****************************************************************************

// USER CODE BEGIN (U0C1_General,4)

// USER CODE END


//****************************************************************************
// @Typedefs
//****************************************************************************

// USER CODE BEGIN (U0C1_General,5)

// USER CODE END


//****************************************************************************
// @Imported Global Variables
//****************************************************************************

// USER CODE BEGIN (U0C1_General,6)

// USER CODE END


//****************************************************************************
// @Global Variables
//****************************************************************************

// USER CODE BEGIN (U0C1_General,7)
unsigned char UART1_InpBuffer[UARTBUF];
unsigned short g_UART1_SendByteCnt;
static unsigned char *UART1_Head= UART1_InpBuffer;		// head pointer for characters going into buffer.
static unsigned char *UART1_Tail= UART1_InpBuffer;		// tail pointer for characters going out of buffer.
//unsigned short g_UART1_CmdCnt= 0;

//unsigned char g_UART1_ucErrorFlag= FALSE;
extern unsigned char g_bBreakFlag;//= FALSE;

unsigned short g_UART1_Events= COM_EVENTS_RESET;

// USER CODE END


//****************************************************************************
// @External Prototypes
//****************************************************************************

// USER CODE BEGIN (U0C1_General,8)

// USER CODE END




//****************************************************************************
// @Prototypes Of Local Functions
//****************************************************************************

// USER CODE BEGIN (U0C1_General,9)
/* ******************************************************************************
 *	FUNCTION: 	SaveCharacter
 *	PARAMETERS:	zu speicherndes Zeichen
 *	DESCRIPTION:
 *	RETURNS: 	
 ******************************************************************************** */
static void SaveCharacter(unsigned char c)
{
	*UART1_Head = c;
	CheckAndWrap(UART1_Head);
}

/* ******************************************************************************
 *  FUNCTION:	UART0ResetDataBuffer
 *  PARAMETERS:	none
 *  DESCRIPTION:Setzt den Eingangspuffers Uart0 zürück
 *  RETURNS:	 
 ******************************************************************************** */
/*void UART1_ResetDataBufferAndErrorFlag(void)
{
	unsigned short a;
	_atomic(2);
	UART1_Tail = UART1_Head;
	*UART1_Head = 0;
//	g_UART1_ucErrorFlag=FALSE;
	g_UART1_Events = COM_EVENTS_RESET;
	for(a = 0; a < UARTBUF; a++) {WDO();UART1_InpBuffer[a] = 0;}
	UART1_Head = UART1_InpBuffer;
}
*/
void UART1_ResetDataBufferAndErrorFlag(void)
{
	unsigned short a;
//	UART1_Tail = UART1_Head;

//	g_UART1_ucErrorFlag=FALSE;
	g_UART1_Events = COM_EVENTS_RESET;
	
	for(a = 0; a < UARTBUF; a++) {WDO();UART1_InpBuffer[a] = 0;}
	UART1_Head = UART1_InpBuffer;
	*UART1_Head = 0;
	_atomic(2);
	UART1_Tail = UART1_Head;
	//chLastInpChar = 0;
}

// USER CODE END


//****************************************************************************
// @Function      void U0C1_ASC_vInit(void)
//
//----------------------------------------------------------------------------
// @Description   This is the initialization function of the U0C1 function
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

void U0C1_ASC_vInit(void)
{
  // USER CODE BEGIN (Init,2)

  // USER CODE END

  ///  -----------------------------------------------------------------------
  ///  U0C1 ASC is configured for Full-Duplex Operation Mode:
  ///  -----------------------------------------------------------------------

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U0C1 Fractional Divider:
  ///  -----------------------------------------------------------------------
  ///  - The Fractional divider is selected
  ///  - The step value STEP = 972

  U0C1_FDRL      =  0x83CC;      // load U0C1 fractional divider register

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U0C1 Baudrate Generator:
  ///  -----------------------------------------------------------------------
  ///  - The selected BaudRate is 38,400 kbaud
  ///  - The PreDivider for CTQ, PCTQ = 1
  ///  - The Denominator for CTQ, DCTQ = 15
  ///  - The Divider factor PDIV = 50

  U0C1_BRGL      =  0x3D00;      // load U0C1 load baud rate generator 
                                 // register L

  U0C1_BRGH      =  0x0032;      // load U0C1 load baud rate generator 
                                 // register H

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U0C1 Input Control Register 0:
  ///  -----------------------------------------------------------------------
  ///  - The data input DX0C is selected for pin - P10.14

  U0C1_DX0CR     =  0x0002;      // load U0C1 input control register(Data)

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U0C1 Interrupt Node Pointer Register:
  ///  -----------------------------------------------------------------------
  ///  - AINP pointer points to Interrupt node U0C1_0IC
  ///  - RINP pointer points to Interrupt node U0C1_0IC
  ///  - PINP pointer points to Interrupt node U0C1_1IC

  U0C1_INPRL     =  0x0010;      // load U0C1 Interrupt Node Pointer register 
                                 // L
  U0C1_INPRH     =  0x0001;      // load U0C1 Interrupt Node Pointer register 
                                 // H

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U0C1 Shift Control:
  ///  -----------------------------------------------------------------------
  ///  - Transmit/Receive LSB first is selected
  ///  - Transmission Mode (TRM) = 1
  ///  - Passive Data Level (PDL) = 1

  U0C1_SCTRL     =  0x0102;      // load U0C1 shift control register L

  ///  - Word Length (WLE) = 7
  ///  - Frame Length (FLE) = 7

  U0C1_SCTRH     =  0x0707;      // load U0C1 shift control register H

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U0C1 Transmit Control/Status Register:
  ///  -----------------------------------------------------------------------
  ///  - TBUF Data Enable (TDEN) = 1
  ///  - TBUF Data Single Shot Mode (TDSSM) = 1

  U0C1_TCSRL     =  0x0500;      // load U0C1 transmit control/status 
                                 // register L
  U0C1_TCSRH     =  0x0000;      // load U0C1 transmit control/status 
                                 // register H

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U0C1 Protocol Control Register:
  ///  -----------------------------------------------------------------------
  ///  - Sample Mode (SMD) = 1
  ///  - 1 Stop bit is selected
  ///  - Format error interrupt is enabled
  ///  - Frame finished interrupt is enabled
  ///  - Sample Point (SP) = 7
  ///  - Pulse Length (PL) = 0

  U0C1_PCRL      =  0x07C1;      // load U0C1 protocol control register L
  U0C1_PCRH      =  0x0000;      // load U0C1 protocol control register H

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U0C1 Protocol Status Register:
  ///  -----------------------------------------------------------------------
  ///  - TBIF is set to simplify polling

  U0C1_PSR      |=  0x2000;      // load U0C1 protocol status register


  ///  -----------------------------------------------------------------------
  ///  Configuration of the used U0C1 Interrupts:
  ///  -----------------------------------------------------------------------
  ///  U0C10IC service request node configuration:
  ///  - U0C10IC interrupt priority level (ILVL) = 9
  ///  - U0C10IC interrupt group level (GLVL) = 0
  ///  - U0C10IC group priority extension (GPX) = 0

  U0C1_0IC       =  0x0064;     

  ///  U0C11IC service request node configuration:
  ///  - U0C11IC interrupt priority level (ILVL) = 9
  ///  - U0C11IC interrupt group level (GLVL) = 1
  ///  - U0C11IC group priority extension (GPX) = 0

  U0C1_1IC       =  0x0065;     

  ///  U0C13IC service request node configuration:
  ///  - U0C13IC interrupt priority level (ILVL) = 3
  ///  - U0C13IC interrupt group level (GLVL) = 0
  ///  - U0C13IC group priority extension (GPX) = 0

  CC2_CC21IC     =  0x004C;     

  SCU_ISSR      |=  0x0300;      // load Interrupt Source Select Register

  ///  -----------------------------------------------------------------------
  ///  Configuration of U0C1 FIFO:
  ///  -----------------------------------------------------------------------
  ///  -----------------------------------------------------------------------
  ///  Configuration of U0C1 Transmitter Buffer Conrol Register:
  ///  -----------------------------------------------------------------------
  ///  - Transmit FIFO buffer contains 16 entries
  ///  - Transmit FIFO buffer starts at Data Pointer 32
  ///  - Limit for transmit FIFO interrupt generation is 0

  U0C1_TBCTRL    =  0x0020;      // load U0C1 transmitter buffer control 
                                 // register L
  U0C1_TBCTRH    =  0x0400;      // load U0C1 transmitter buffer control 
                                 // register H

  ///  -----------------------------------------------------------------------
  ///  Configuration of U0C1 Receiver Buffer Conrol Register:
  ///  -----------------------------------------------------------------------
  ///  - Receive FIFO buffer contains 16 entries
  ///  - Receive FIFO buffer starts at Data Pointer 48
  ///  - Limit for receive FIFO interrupt generation is 0
  ///  - Filling level mode is selected
  ///  - Standard Receive buffer interrupt is enabled

  U0C1_RBCTRL    =  0x0030;      // load U0C1 receive buffer control register 
                                 // L
  U0C1_RBCTRH    =  0x4400;      // load U0C1 receive buffer control register 
                                 // H

  ///  -----------------------------------------------------------------------
  ///  Configuration of the used U0C1 Port Pins:
  ///  -----------------------------------------------------------------------
  P10_IOCR14     =  0x0020;      // load port register for data input

  ///  - P10.15 is used for USIC0Channel1 Shift Data output(DOUT)

  P10_IOCR15 = 0x00A0;    //set direction register



  // USER CODE BEGIN (U0C1_Function,3)
//U0C1_PCRL      &=  0xFFFB;      // load U1C0 protocol control register L
  // USER CODE END



} //  End of function U0C1_ASC_vInit


//****************************************************************************
// @Function      void U0C1_ASC_vi0IC(void) 
//
//----------------------------------------------------------------------------
// @Description   This is the interrupt service routine for the Service 
//                Request Node U0C1_0IC.
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

_interrupt(U0C1_0INT)  void U0C1_ASC_vi0IC(void)
{

  // USER CODE BEGIN (ASC0IC,2)

  // USER CODE END

  if (U0C1_PSR & 0x8000)         // Alternate receive interrupt flag
  {

  // USER CODE BEGIN (ASC0IC,3)
//   	g_UART1_ucErrorFlag=TRUE;
//	g_UART1_Events |= EVENT_OVERFLOW_SL;
  // USER CODE END

  U0C1_PSCR   |= 0x8000;         // clear PSR_AIF
  }

  if (U0C1_PSR & 0x4000)         // Receive interrupt flag
  {

  // USER CODE BEGIN (ASC0IC,4)
   // Max. Laufzeit ohne Interruptframe ca. 1.8µs (555KHz) bei 66MHz
   SaveCharacter(U0C1_RBUF);
   /*
	vom KFA mit nanotec Master+slaves
	static char BreakWatch[3]= {0};
	char chInpChar = U0C1_RBUF;
	
//U0C1_IN00 = chInpChar;
	if(Bus485_Slave)
	{
		switch(chInpChar)
		{
			case 0		:	break; // Zeichen ist nicht protokollkonform, erscheint aber beim Reset des Host

			case '\r'   :  	//++g_UART1_CmdCnt;
							
							SaveCharacter('\r');
							//SaveCharacter('\0');		// CR vorhanden, Commando komplett
							if(strncmp(BreakWatch, "*S1",3)==0)
							{
								g_bBreakFlag= TRUE;
								//Drive_SetEvents(EVENT_USER_BREAK);
								g_UART1_Events |= COM_EVENT_USER_BREAK;
							}
							g_UART1_Events |= EVENT_TERM;
							break;
			case '\n'   :	break;
			//case '*'	:	
			default		:	BreakWatch[0]= BreakWatch[1];
							BreakWatch[1]= BreakWatch[2];
							BreakWatch[2]= chInpChar;
							SaveCharacter(chInpChar);
							if(chInpChar == '*') {g_UART1_Events |= BROADCAST_EVENT;}
							chInpChar = '0';
							break;
	   }
   }
   else
   {	//putchar(chInpChar);fflush(stdout);
		switch(chInpChar)
		{
//			case 0		:	break; // Zeichen ist nicht protokollkonform, erscheint aber beim Reset des Host

			case '\r'   :  	//++g_UART1_CmdCnt;
							SaveCharacter('\r');
							chInpChar = '0';
							g_UART1_Events |= EVENT_TERM;
							break;
			default		:	SaveCharacter(chInpChar);
							chInpChar = '0';
							break;

	   }
   }
   */
  // USER CODE END

  U0C1_PSCR   |= 0x4000;         // clear PSR_RIF
  }

  // USER CODE BEGIN (ASC0IC,15)
	K_OS_Intrp_Exit();						// no call right CMX interrupt exit routine. MUST be
											// last instruction within interrupt's code

  // USER CODE END

} //  End of function U0C1_ASC_vi0IC


//****************************************************************************
// @Function      void U0C1_ASC_vi1IC(void) 
//
//----------------------------------------------------------------------------
// @Description   This is the interrupt service routine for the Service 
//                Request Node U0C1_1IC.
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

// USER CODE BEGIN (ASC1IC,1)

// USER CODE END

_interrupt(U0C1_1INT)  void U0C1_ASC_vi1IC(void)
{

  // USER CODE BEGIN (ASC1IC,2)

  // USER CODE END

  if (U0C1_PSR & 0x0020)         // Format error interrupt flag in stop bit 0
  {

  // USER CODE BEGIN (ASC1IC,11)

  // USER CODE END

  U0C1_PSCR   |= 0x0020;         // clear PSR_ST5/FER0
  }

  if (U0C1_PSR & 0x0040)         // Format error interrupt flag in stop bit 1
  {

  // USER CODE BEGIN (ASC1IC,12)

  // USER CODE END

  U0C1_PSCR   |= 0x0040;         // clear PSR_ST6/FER1
  }

  if (U0C1_PSR & 0x0080)         // Rx frame finished interrupt flag
  {

  // USER CODE BEGIN (ASC1IC,13)

  // USER CODE END

  U0C1_PSCR   |= 0x0080;         // clear PSR_ST7/RFF
  }

  if (U0C1_PSR & 0x0100)         // Tx frame finished interrupt flag
  {

  // USER CODE BEGIN (ASC1IC,14)
#pragma asm
  ADD  _g_UART1_SendByteCnt,ONES
  SUBC _g_UART1_SendByteCnt,ONES
#pragma endasm

  // USER CODE END

  U0C1_PSCR   |= 0x0100;         // clear PSR_ST8/RFF
  }

  // USER CODE BEGIN (ASC1IC,15)
	K_OS_Intrp_Exit();						// no call right CMX interrupt exit routine. MUST be
											// last instruction within interrupt's code

  // USER CODE END

} //  End of function U0C1_ASC_vi1IC




// USER CODE BEGIN (U0C1_General,10)

/* ******************************************************************************
 *  FUNCTION:	UART1_GetChar
 *  PARAMETERS:	none
 *  DESCRIPTION:liest ein Zeichen aus dem stdin-buffer
 *  RETURNS:	- character from buffer
 *              - EOF wenn kein Zeichen eingegangen ist
 ******************************************************************************** */
int UART1_GetChar(void)
{
	register int ch;

	if(UART1_Tail != UART1_Head)
	{
		ch = *UART1_Tail;
		CheckAndWrap(UART1_Tail);
	}else
	{
		ch = EOF;
	}
	return(ch);
}
int Uart1_kbHit(void)
{
	if(UART1_Tail < UART1_Head)
	{
		return TRUE;

	}else
	{
		return FALSE;
	}
}

/* ******************************************************************************
 *  FUNCTION: UART1_PutChar
 *
 *  PARAMETERS:	Pointer auf ZK, Anzahl der Elemente
 *
 *  DESCRIPTION: schreibt Zeichen auf ASC1
 *
 *  RETURNS:  Anzahl der geschriebenen Zeichen	
 ******************************************************************************** */
int UART1_PutChar(char * base, size_t siz) 
{  unsigned int i;
   for(i=0; i<siz; i++)
   { 
       while(U0C1_ASC_ubIsTxFIFOfull())
       { 
       	  WDO();	/* falls Buffer voll ist, dann warten */
	   }
	   U0C1_IN00= *base;  base++;
   }
   return siz;	
}

/* FUNCTION: 	DisableOut485
   PARAMETERS:	---
   DESCRIPTION:	Hochohmig schalten des Treiberausgangs am 485-Bus (DE1)
				wenn alle Zeichen gesendet sind
				Nach Senden eines Frames wird ein Interrupt ausgelöst (PSR->TFF)
				und über g_UART1_SendByteCnt==0 wird das Ende des Sende-Strings erkannt
   RETURNS:		void */
void DisableOutRS485_BUS(void)
{
   while(g_UART1_SendByteCnt) WDO();  // Warten auf Senden aller Bytes
   ShortTimer(10); // in µs !ACHTUNG BLOCKIERT hier das OS
   U0C1_COM1_DE_485= 0;
	ShortTimer(10); // in µs !ACHTUNG BLOCKIERT hier das OS
}
void EnableOutRS485_BUS(void)
{
   U0C1_COM1_DE_485= 1;
}

// USER CODE END

