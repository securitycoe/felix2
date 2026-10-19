//****************************************************************************
// @Module        Universal Serial Interface Channel (USIC) U0C0
// @Filename      ASC0.C
// @Project       30_5015_Felix.dav
//----------------------------------------------------------------------------
// @Controller    Infineon XE167F-96F66
//
// @Compiler      Tasking Classic
//
// @Codegenerator 2.2
//
// @Description   This file contains functions that use the U0C0 module.
//
//----------------------------------------------------------------------------
// @Date          07.03.2016 10:02:16
//
//****************************************************************************

// USER CODE BEGIN (U0C0_General,1)

// USER CODE END

/*
******************************************************************************
@Note

Master Host und Programmierschnittstelle

******************************************************************************
*/


//****************************************************************************
// @Project Includes
//****************************************************************************

#include "MAIN.H"

// USER CODE BEGIN (U0C0_General,2)
//#include "Shared/Stacker_Lifter_Allgemein/StacComm.h"
#include <string.h>
// USER CODE END


//****************************************************************************
// @Macros
//****************************************************************************

// USER CODE BEGIN (U0C0_General,3)
#define CheckAndWrap(HeadOrTail) (HeadOrTail = ((++HeadOrTail) >= (UART0_InpBuffer + ASCBUF)) ? UART0_InpBuffer : HeadOrTail);
// USER CODE END


//****************************************************************************
// @Defines
//****************************************************************************

// USER CODE BEGIN (U0C0_General,4)

// USER CODE END


//****************************************************************************
// @Typedefs
//****************************************************************************

// USER CODE BEGIN (U0C0_General,5)

// USER CODE END


//****************************************************************************
// @Imported Global Variables
//****************************************************************************

// USER CODE BEGIN (U0C0_General,6)
extern unsigned char UserMainSlot;
// USER CODE END


//****************************************************************************
// @Global Variables
//****************************************************************************

// USER CODE BEGIN (U0C0_General,7)
unsigned char UART0_InpBuffer[ASCBUF];
static unsigned char *UART0_Head= UART0_InpBuffer;		// head pointer for characters going into buffer.
static unsigned char *UART0_Tail= UART0_InpBuffer;		// tail pointer for characters going out of buffer.
//unsigned char g_UART0_ucCmdFlag= FALSE;
//unsigned char g_UART0_ucErrorFlag= FALSE;
unsigned short g_UART0_Events= COM_EVENTS_RESET;
//unsigned short g_UART0_CmdCnt= 0;
// USER CODE END


//****************************************************************************
// @External Prototypes
//****************************************************************************

// USER CODE BEGIN (U0C0_General,8)

// USER CODE END




//****************************************************************************
// @Prototypes Of Local Functions
//****************************************************************************

// USER CODE BEGIN (U0C0_General,9)
/* ******************************************************************************
 *	FUNCTION: 	SaveCharacter
 *	PARAMETERS:	zu speicherndes Zeichen
 *	DESCRIPTION:
 *	RETURNS: 	
 ******************************************************************************** */
static void SaveCharacter(unsigned char c)
{
	*UART0_Head = c;
	CheckAndWrap(UART0_Head);
}

/* ******************************************************************************
 *  FUNCTION:	UART0ResetDataBuffer
 *  PARAMETERS:	none
 *  DESCRIPTION:Setzt den Eingangspuffers Uart0 zürück
 *  RETURNS:	 
 ******************************************************************************** */
/*void UART0_ResetDataBufferAndErrorFlag(void)
{
	unsigned short a;
	_atomic(2);
	UART0_Tail = UART0_Head;
	*UART0_Head = 0;
//	g_UART0_ucErrorFlag=FALSE;
	g_UART0_Events = COM_EVENTS_RESET;
	
	for(a = 0; a < ASCBUF; a++) {WDO();UART0_InpBuffer[a] = 0;}
	UART0_Head = UART0_InpBuffer;
}*/

void UART0_ResetDataBufferAndErrorFlag(void)
{
	unsigned short a;

	char ch = U0C0_RBUF;		//Dummy-OP
	_atomic(2);
	UART0_Tail = UART0_Head;

//	g_UART0_ucErrorFlag=FALSE;
	g_UART0_Events = COM_EVENTS_RESET;
	
	for(a = 0; a < ASCBUF; a++) {WDO();UART0_InpBuffer[a] = 0;}
	UART0_Head = UART0_InpBuffer;
	*UART0_Head = 0;
	_atomic(2);
	UART0_Tail = UART0_Head;
	U0C0_PSCR   |= 0x8000;         // clear PSR_AIF
	U0C0_PSCR   |= 0x4000;         // clear PSR_RIF
	//chLastInpChar = 0;
}

// USER CODE END


//****************************************************************************
// @Function      void ASC0_vInit(void)
//
//----------------------------------------------------------------------------
// @Description   This is the initialization function of the U0C0 function
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

void ASC0_vInit(void)
{
  // USER CODE BEGIN (Init,2)

  // USER CODE END

  ///  -----------------------------------------------------------------------
  ///  U0C0 ASC is configured for Full-Duplex Operation Mode:
  ///  -----------------------------------------------------------------------

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U0C0 Fractional Divider:
  ///  -----------------------------------------------------------------------
  ///  - The Fractional divider is selected
  ///  - The step value STEP = 953

  U0C0_FDRL      =  0x83B9;      // load U0C0 fractional divider register

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U0C0 Baudrate Generator:
  ///  -----------------------------------------------------------------------
  ///  - The selected BaudRate is 19,200 kbaud
  ///  - The PreDivider for CTQ, PCTQ = 1
  ///  - The Denominator for CTQ, DCTQ = 15
  ///  - The Divider factor PDIV = 99

  U0C0_BRGL      =  0x3D00;      // load U0C0 load baud rate generator 
                                 // register L

  U0C0_BRGH      =  0x0063;      // load U0C0 load baud rate generator 
                                 // register H

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U0C0 Input Control Register 0:
  ///  -----------------------------------------------------------------------
  ///  - The data input DX0D is selected for pin - P7.4

  U0C0_DX0CR     =  0x0003;      // load U0C0 input control register(Data)

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U0C0 Interrupt Node Pointer Register:
  ///  -----------------------------------------------------------------------
  ///  - AINP pointer points to Interrupt node U0C0_0IC
  ///  - RINP pointer points to Interrupt node U0C0_0IC

  U0C0_INPRL     =  0x0000;      // load U0C0 Interrupt Node Pointer register 
                                 // L
  U0C0_INPRH     =  0x0000;      // load U0C0 Interrupt Node Pointer register 
                                 // H

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U0C0 Shift Control:
  ///  -----------------------------------------------------------------------
  ///  - Transmit/Receive LSB first is selected
  ///  - Transmission Mode (TRM) = 1
  ///  - Passive Data Level (PDL) = 1

  U0C0_SCTRL     =  0x0102;      // load U0C0 shift control register L

  ///  - Word Length (WLE) = 7
  ///  - Frame Length (FLE) = 7

  U0C0_SCTRH     =  0x0707;      // load U0C0 shift control register H

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U0C0 Transmit Control/Status Register:
  ///  -----------------------------------------------------------------------
  ///  - TBUF Data Enable (TDEN) = 1
  ///  - TBUF Data Single Shot Mode (TDSSM) = 1

  U0C0_TCSRL     =  0x0500;      // load U0C0 transmit control/status 
                                 // register L
  U0C0_TCSRH     =  0x0000;      // load U0C0 transmit control/status 
                                 // register H

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U0C0 Protocol Control Register:
  ///  -----------------------------------------------------------------------
  ///  - Sample Mode (SMD) = 1
  ///  - 2 Stop bits are selected
  ///  - Sample Point (SP) = 9
  ///  - Pulse Length (PL) = 0

  U0C0_PCRL      =  0x0903;      // load U0C0 protocol control register L
  U0C0_PCRH      =  0x0000;      // load U0C0 protocol control register H

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U0C0 Protocol Status Register:
  ///  -----------------------------------------------------------------------
  ///  - TBIF is set to simplify polling

  U0C0_PSR      |=  0x2000;      // load U0C0 protocol status register


  ///  -----------------------------------------------------------------------
  ///  Configuration of the used U0C0 Interrupts:
  ///  -----------------------------------------------------------------------
  ///  U0C00IC service request node configuration:
  ///  - U0C00IC interrupt priority level (ILVL) = 10
  ///  - U0C00IC interrupt group level (GLVL) = 0
  ///  - U0C00IC group priority extension (GPX) = 0

  U0C0_0IC       =  0x0068;     

  ///  -----------------------------------------------------------------------
  ///  Configuration of U0C0 FIFO:
  ///  -----------------------------------------------------------------------
  ///  -----------------------------------------------------------------------
  ///  Configuration of U0C0 Transmitter Buffer Conrol Register:
  ///  -----------------------------------------------------------------------
  ///  - Transmit FIFO buffer contains 16 entries
  ///  - Transmit FIFO buffer starts at Data Pointer 16
  ///  - Limit for transmit FIFO interrupt generation is 0

  U0C0_TBCTRL    =  0x0010;      // load U0C0 transmitter buffer control 
                                 // register L
  U0C0_TBCTRH    =  0x0400;      // load U0C0 transmitter buffer control 
                                 // register H

  ///  -----------------------------------------------------------------------
  ///  Configuration of U0C0 Receiver Buffer Conrol Register:
  ///  -----------------------------------------------------------------------
  ///  - Receive FIFO buffer contains 16 entries
  ///  - Receive FIFO buffer starts at Data Pointer 0
  ///  - Limit for receive FIFO interrupt generation is 0
  ///  - Filling level mode is selected
  ///  - Standard Receive buffer interrupt is enabled

  U0C0_RBCTRL    =  0x0000;      // load U0C0 receive buffer control register 
                                 // L
  U0C0_RBCTRH    =  0x4400;      // load U0C0 receive buffer control register 
                                 // H

  ///  -----------------------------------------------------------------------
  ///  Configuration of the used U0C0 Port Pins:
  ///  -----------------------------------------------------------------------
  P7_IOCR04      =  0x0020;      // load port register for data input

  ///  - P7.3 is used for USIC0Channel0 Shift Data output(DOUT)

  P7_IOCR03 = 0x00B0;    //set direction register



  // USER CODE BEGIN (U0C0_Function,3)
	//DSTP0 = (int)(&S0TBUF);
	DSTP0 = (int)&U0C0_RBUF0;
  // USER CODE END



} //  End of function ASC0_vInit


//****************************************************************************
// @Function      void U0C0_ASC_vi0IC(void) 
//
//----------------------------------------------------------------------------
// @Description   This is the interrupt service routine for the Service 
//                Request Node U0C0_0IC.
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

_interrupt(U0C0_0INT)  void U0C0_ASC_vi0IC(void)
{

  // USER CODE BEGIN (ASC0IC,2)

  // USER CODE END

  if (U0C0_PSR & 0x8000)         // Alternate receive interrupt flag
  {

  // USER CODE BEGIN (ASC0IC,3)
  	// wird durch Paritätsfehler ausgelöst
  	char ch;
  	//U0C0_TRBSCR= 0x4000;			//clear FIFO-Buffer
  	ch = U0C0_RBUF;
	U0C0_PSCR   |= 0x4000;         // clear PSR_RIF
   	//g_UART0_ucErrorFlag=TRUE;
	g_UART0_Events |= COM_EVENT_OVERFLOW;
	K_Intrp_Event_Signal(0, UserMainSlot, EVENT_MASTER_COM);
  // USER CODE END

  U0C0_PSCR   |= 0x8000;         // clear PSR_AIF
  }

  if (U0C0_PSR & 0x4000)         // Receive interrupt flag
  {

  // USER CODE BEGIN (ASC0IC,4)
  static char BreakWatch[3]= {0};
	static char chLastInpChar = 0;
	char chInpChar = U0C0_RBUF;
//U0C0_IN00 = chInpChar;
	switch(chInpChar)
	{
		case '#'	:	if(chLastInpChar == chInpChar)
						{
							g_bBreakFlag = TRUE;
							//Drive_SetEvents(EVENT_USER_BREAK);
							g_UART0_Events |= COM_EVENT_USER_BREAK;
							//SaveCharacter(C_BREAK);		// damit BREAK in UserMain erkannt werden kann
							//SaveCharacter('\r');		// für 'buffered input' erforderlich
							chLastInpChar = 0;
							K_Intrp_Event_Signal(3, 0, EVENT_USER_BREAK);
							//g_UART0_ucCmdFlag=TRUE;
						}
						else
						{
							chLastInpChar = '#';
						}
						break;
 
		case '%'	:	//SaveCharacter('%');
						//SaveCharacter('\r');		// für 'buffered input' erforderlich
						chLastInpChar = '0';
						//g_UART0_ucCmdFlag=TRUE;
						g_UART0_Events |= COM_EVENT_SYNC;
						K_Intrp_Event_Signal(0, UserMainSlot, EVENT_MASTER_COM);
						
						break;
		case '\r'   :	//++g_UART0_CmdCnt;
						//SaveCharacter('\r');		// CR vorhanden, Commando komplett
						SaveCharacter('\0');		// CR vorhanden, Commando komplett
						chLastInpChar = '0';
						//g_UART0_ucCmdFlag=TRUE;
						g_UART0_Events |= COM_EVENT_CR;
						K_Intrp_Event_Signal(0, UserMainSlot, EVENT_MASTER_COM);
						break;
		case '\n'   :	break;
		default		:	SaveCharacter(chInpChar);
						chLastInpChar = '0';
						break;
/*
für test protokoll des 485 Bus
		case 0		:	break; // Zeichen ist nicht protokollkonform, erscheint aber beim Reset des Host

		case '\r'   :  	//++g_UART1_CmdCnt;
						SaveCharacter('\r');
						//SaveCharacter('\0');		// CR vorhanden, Commando komplett
						if(strncmp(BreakWatch, "*S1",3)==0)
						{
							g_bBreakFlag= TRUE;
							g_UART0_Events |= EVENT_USER_BREAK;
							Drive_SetEvents(EVENT_USER_BREAK);

						}
						g_UART0_Events |= EVENT_TERM;
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
*/
	}
  // USER CODE END

  U0C0_PSCR   |= 0x4000;         // clear PSR_RIF
  }

  // USER CODE BEGIN (ASC0IC,15)
	K_OS_Intrp_Exit();						// no call right CMX interrupt exit routine. MUST be
											// last instruction within interrupt's code

  // USER CODE END

} //  End of function U0C0_ASC_vi0IC




// USER CODE BEGIN (U0C0_General,10)
/* ******************************************************************************
 *  FUNCTION:	UART0_GetChar
 *  PARAMETERS:	none
 *  DESCRIPTION:liest ein Zeichen aus dem stdin-buffer
 *  RETURNS:	- character from stdin-buffer
 *              - EOF wenn kein Zeichen eingegangen ist
 ******************************************************************************** */
int UART0_GetChar(void)
{
	register int ch;

	if(UART0_Tail != UART0_Head)
	{
		ch = *UART0_Tail;
		CheckAndWrap(UART0_Tail);
	}else
	{
		ch = EOF;
	}
	return(ch);
}
int Uart0_kbHit(void)
{
	if(UART0_Tail < UART0_Head)
	{
		return TRUE;

	}else
	{
		return FALSE;
	}
}

/* ******************************************************************************
 *  FUNCTION: UART0_PutChar
 *
 *  PARAMETERS:	Pointer auf ZK, Anzahl der Elemente
 *
 *  DESCRIPTION: schreibt Zeichen auf ASC0
 *
 *  RETURNS:  Anzahl der geschriebenen Zeichen	
 ******************************************************************************** */
int UART0_PutChar(char * base, size_t siz) 
{  unsigned int i;
   for(i=0; i<siz; i++)
   { 
       while(U0C0_ASC_ubIsTxFIFOfull())
       { 
       	  WDO();	/* falls Buffer voll ist, dann warten */
	   }
	   U0C0_IN00= *base;  base++;
   }
   return siz;	
}

unsigned char IsUserBreak(void)
{
   return g_bBreakFlag;
}

// USER CODE END

