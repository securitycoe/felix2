//****************************************************************************
// @Module        Universal Serial Interface Channel (USIC) U1C0
// @Filename      U1C0_SLAVE_COM5.C
// @Project       30_5015_Felix.dav
//----------------------------------------------------------------------------
// @Controller    Infineon XE167F-96F66
//
// @Compiler      Tasking Classic
//
// @Codegenerator 2.2
//
// @Description   This file contains functions that use the U1C0 module.
//
//----------------------------------------------------------------------------
// @Date          07.03.2016 10:02:18
//
//****************************************************************************

// USER CODE BEGIN (U1C0_General,1)

// USER CODE END

/*
******************************************************************************
@Note

Com5 Slave Weiterleitung.

******************************************************************************
*/


//****************************************************************************
// @Project Includes
//****************************************************************************

#include "MAIN.H"

// USER CODE BEGIN (U1C0_General,2)
//#include "Shared/Stacker_Lifter_Allgemein/StacComm.h"
#include <string.h>
// USER CODE END


//****************************************************************************
// @Macros
//****************************************************************************

// USER CODE BEGIN (U1C0_General,3)
#define CheckAndWrap(HeadOrTail) (HeadOrTail = ((++HeadOrTail) >= (UART5_InpBuffer + UARTBUF)) ? UART5_InpBuffer : HeadOrTail);
// USER CODE END


//****************************************************************************
// @Defines
//****************************************************************************

// USER CODE BEGIN (U1C0_General,4)

// USER CODE END


//****************************************************************************
// @Typedefs
//****************************************************************************

// USER CODE BEGIN (U1C0_General,5)

// USER CODE END


//****************************************************************************
// @Imported Global Variables
//****************************************************************************

// USER CODE BEGIN (U1C0_General,6)
extern unsigned char UserMainSlot;
extern unsigned char SlaveSlot;
// USER CODE END


//****************************************************************************
// @Global Variables
//****************************************************************************

// USER CODE BEGIN (U1C0_General,7)
char UART5_InpBuffer[UARTBUF];
static char *UART5_Head= UART5_InpBuffer;		// head pointer for characters going into buffer.
static char *UART5_Tail= UART5_InpBuffer;		// tail pointer for characters going out of buffer.
//unsigned char g_UART5_ucCmdFlag= FALSE;
//unsigned char g_UART5_ucErrorFlag= FALSE;
unsigned short g_UART5_Events= COM_EVENTS_RESET;
//unsigned short g_UART5_CmdCnt= 0;

// USER CODE END


//****************************************************************************
// @External Prototypes
//****************************************************************************

// USER CODE BEGIN (U1C0_General,8)

// USER CODE END




//****************************************************************************
// @Prototypes Of Local Functions
//****************************************************************************

// USER CODE BEGIN (U1C0_General,9)
/* ******************************************************************************
 *	FUNCTION: 	SaveCharacter
 *	PARAMETERS:	zu speicherndes Zeichen
 *	DESCRIPTION:
 *	RETURNS: 	
 ******************************************************************************** */
static void SaveCharacter(unsigned char c)
{
	*UART5_Head = c;
	CheckAndWrap(UART5_Head);
}

/* ******************************************************************************
 *  FUNCTION:	UART5ResetDataBuffer
 *  PARAMETERS:	none
 *  DESCRIPTION:Setzt den Eingangspuffers Uart0 zürück
 *  RETURNS:	 
 ******************************************************************************** */
/*void UART5_ResetDataBufferAndErrorFlag(void)
{
	unsigned short a;
	_atomic(2);
	UART5_Tail = UART5_Head;
	*UART5_Head = 0;
//	g_UART5_ucErrorFlag=FALSE;
	g_UART5_Events = COM_EVENTS_RESET;
	
	for(a = 0; a < UARTBUF; a++) {WDO();UART5_InpBuffer[a] = 0;}
	UART5_Head = UART5_InpBuffer;
}*/

void UART5_ResetDataBufferAndErrorFlag(void)
{
	unsigned short a;
//	UART5_Tail = UART5_Head;

//	g_UART5_ucErrorFlag=FALSE;
	g_UART5_Events = COM_EVENTS_RESET;
	
	for(a = 0; a < UARTBUF; a++) {WDO();UART5_InpBuffer[a] = 0;}
	UART5_Head = UART5_InpBuffer;
	*UART5_Head = 0;
	_atomic(2);
	UART5_Tail = UART5_Head;
	//chLastInpChar = 0;
}

// USER CODE END


//****************************************************************************
// @Function      void U1C0_ASC_vInit(void)
//
//----------------------------------------------------------------------------
// @Description   This is the initialization function of the U1C0 function
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

void U1C0_ASC_vInit(void)
{
  // USER CODE BEGIN (Init,2)

// ACHTUNG DAVE setzt U1C0_SCTRL und U1C0_PCRL auf Werte das die Schnittstelle nicht mehr funktioniert
// hier sind die richtigen Werte
//	U1C0_SCTRL     =  0x0102;
//	U1C0_PCRL      =  0x0903;

  // USER CODE END

  ///  -----------------------------------------------------------------------
  ///  U1C0 ASC is configured for Full-Duplex Operation Mode:
  ///  -----------------------------------------------------------------------

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U1C0 Fractional Divider:
  ///  -----------------------------------------------------------------------
  ///  - The Fractional divider is selected
  ///  - The step value STEP = 953

  U1C0_FDRL      =  0x83B9;      // load U1C0 fractional divider register
//  U1C0_FDRL      =  0x81DD;      // load U1C0 fractional divider register

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U1C0 Baudrate Generator:
  ///  -----------------------------------------------------------------------
  ///  - The selected BaudRate is 19,200 kbaud
  ///  - The PreDivider for CTQ, PCTQ = 1
  ///  - The Denominator for CTQ, DCTQ = 15
  ///  - The Divider factor PDIV = 99

  U1C0_BRGL      =  0x3D00;      // load U1C0 load baud rate generator 
                                 // register L

  U1C0_BRGH      =  0x0063;      // load U1C0 load baud rate generator 
                                 // register H

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U1C0 Input Control Register 0:
  ///  -----------------------------------------------------------------------
  ///  - The data input DX0D is selected for pin - P10.13

  U1C0_DX0CR     =  0x0003;      // load U1C0 input control register(Data)

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U1C0 Interrupt Node Pointer Register:
  ///  -----------------------------------------------------------------------
  ///  - AINP pointer points to Interrupt node U1C0_0IC
  ///  - RINP pointer points to Interrupt node U1C0_0IC

  U1C0_INPRL     =  0x0000;      // load U1C0 Interrupt Node Pointer register 
                                 // L
  U1C0_INPRH     =  0x0000;      // load U1C0 Interrupt Node Pointer register 
                                 // H

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U1C0 Shift Control:
  ///  -----------------------------------------------------------------------
  ///  - Transmit/Receive MSB first is selected
  ///  - Transmission Mode (TRM) = 1
  ///  - Passive Data Level (PDL) = 1

  U1C0_SCTRL     =  0x0102;      // load U1C0 shift control register L

  ///  - Word Length (WLE) = 7
  ///  - Frame Length (FLE) = 7

  U1C0_SCTRH     =  0x0707;      // load U1C0 shift control register H

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U1C0 Transmit Control/Status Register:
  ///  -----------------------------------------------------------------------
  ///  - TBUF Data Enable (TDEN) = 1
  ///  - TBUF Data Single Shot Mode (TDSSM) = 1

  U1C0_TCSRL     =  0x0500;      // load U1C0 transmit control/status 
                                 // register L
  U1C0_TCSRH     =  0x0000;      // load U1C0 transmit control/status 
                                 // register H

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U1C0 Protocol Control Register:
  ///  -----------------------------------------------------------------------
  ///  - Sample Mode (SMD) = 1
  ///  - 2 Stop bits are selected
  ///  - Sample Point (SP) = 9
  ///  - Pulse Length (PL) = 0

//  U1C0_PCRL      =  0x0901;      // load U1C0 protocol control register L
  U1C0_PCRL      =  0x0903;      // load U1C0 protocol control register L
//  U1C0_PCRL      =  0x0701;      // load U1C0 protocol control register L
  U1C0_PCRH      =  0x0000;      // load U1C0 protocol control register H

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U1C0 Protocol Status Register:
  ///  -----------------------------------------------------------------------
  ///  - TBIF is set to simplify polling

  U1C0_PSR      |=  0x2000;      // load U1C0 protocol status register


  ///  -----------------------------------------------------------------------
  ///  Configuration of the used U1C0 Interrupts:
  ///  -----------------------------------------------------------------------
  ///  U1C00IC service request node configuration:
  ///  - U1C00IC interrupt priority level (ILVL) = 8
  ///  - U1C00IC interrupt group level (GLVL) = 0
  ///  - U1C00IC group priority extension (GPX) = 0

  U1C0_0IC       =  0x0060;     

  ///  U1C03IC service request node configuration:
  ///  - U1C03IC interrupt priority level (ILVL) = 3
  ///  - U1C03IC interrupt group level (GLVL) = 1
  ///  - U1C03IC group priority extension (GPX) = 0

  CC2_CC22IC     =  0x004D;     

  SCU_ISSR      |=  0x0300;      // load Interrupt Source Select Register

  ///  -----------------------------------------------------------------------
  ///  Configuration of U1C0 FIFO:
  ///  -----------------------------------------------------------------------
  ///  -----------------------------------------------------------------------
  ///  Configuration of U1C0 Transmitter Buffer Conrol Register:
  ///  -----------------------------------------------------------------------
  ///  - Transmit FIFO buffer contains 16 entries
  ///  - Transmit FIFO buffer starts at Data Pointer 32
  ///  - Limit for transmit FIFO interrupt generation is 0

  U1C0_TBCTRL    =  0x0020;      // load U1C0 transmitter buffer control 
                                 // register L
  U1C0_TBCTRH    =  0x0400;      // load U1C0 transmitter buffer control 
                                 // register H

  ///  -----------------------------------------------------------------------
  ///  Configuration of U1C0 Receiver Buffer Conrol Register:
  ///  -----------------------------------------------------------------------
  ///  - Receive FIFO buffer contains 16 entries
  ///  - Receive FIFO buffer starts at Data Pointer 48
  ///  - Limit for receive FIFO interrupt generation is 0
  ///  - Filling level mode is selected
  ///  - Standard Receive buffer interrupt is enabled

  U1C0_RBCTRL    =  0x0030;      // load U1C0 receive buffer control register 
                                 // L
  U1C0_RBCTRH    =  0x4400;      // load U1C0 receive buffer control register 
                                 // H

  ///  -----------------------------------------------------------------------
  ///  Configuration of the used U1C0 Port Pins:
  ///  -----------------------------------------------------------------------
  P10_IOCR13     =  0x0020;      // load port register for data input

  ///  - P10.12 is used for USIC1Channel0 Shift Data output(DOUT)

  P10_IOCR12 = 0x0090;    //set direction register



  // USER CODE BEGIN (U1C0_Function,3)
// ACHTUNG DAVE setzt U1C0_SCTRL und U1C0_PCRL auf Werte das die Schnittstelle nicht mehr funktioniert
// hier sind die richtigen Werte
//	U1C0_SCTRL     =  0x0102;
//	U1C0_PCRL      =  0x0903;

  // USER CODE END



} //  End of function U1C0_ASC_vInit


//****************************************************************************
// @Function      void U1C0_ASC_vi0IC(void) 
//
//----------------------------------------------------------------------------
// @Description   This is the interrupt service routine for the Service 
//                Request Node U1C0_0IC.
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

_interrupt(U1C0_0INT)  void U1C0_ASC_vi0IC(void)
{

  // USER CODE BEGIN (ASC0IC,2)

  // USER CODE END

  if (U1C0_PSR & 0x8000)         // Alternate receive interrupt flag
  {

  // USER CODE BEGIN (ASC0IC,3)
  	// wird durch Paritätsfehler ausgelöst

//   NICHT DoorLift
  	char ch;
  	//U0C0_TRBSCR= 0x4000;			//clear FIFO-Buffer
  	ch = U1C0_RBUF;
	U1C0_PSCR   |= 0x4000;         // clear PSR_RIF
   	//g_UART5_ucErrorFlag=TRUE;
	g_UART5_Events |= COM_EVENT_OVERFLOW;
	K_Intrp_Event_Signal(0, UserMainSlot, EVENT_SLAVE_COM);
	K_Intrp_Event_Signal(0, SlaveSlot, EVENT_SLAVE_COM);
  // USER CODE END

  U1C0_PSCR   |= 0x8000;         // clear PSR_AIF
  }

  if (U1C0_PSR & 0x4000)         // Receive interrupt flag
  {

  // USER CODE BEGIN (ASC0IC,4)
  if((g_EquipmentConfig.DeviceConfig & BIT_D_DoorLifter))
  {
  	SaveCharacter(U1C0_RBUF);
  }
  else
  {
  // NICHT DoorLift
	static char chLastInpChar = 0;
	char chInpChar = U1C0_RBUF;
//U1C0_IN00 = chInpChar;
	switch(chInpChar)
	{
		case RDY	:
						SaveCharacter(chInpChar);
						chLastInpChar = '0';
						//g_UART5_ucCmdFlag=TRUE;
						g_UART5_Events |= COM_EVENT_CR;
						K_Intrp_Event_Signal(0, UserMainSlot , EVENT_SLAVE_COM);
						K_Intrp_Event_Signal(0, SlaveSlot, EVENT_SLAVE_COM);
						break;

		case '\r'   :	//++g_UART5_CmdCnt;
						//SaveCharacter('\r');		// CR vorhanden, Commando komplett
						SaveCharacter('\0');		// CR vorhanden, Commando komplett
						chLastInpChar = '0';
						//g_UART5_ucCmdFlag=TRUE;
						g_UART5_Events |= COM_EVENT_CR;
						K_Intrp_Event_Signal(0, UserMainSlot, EVENT_SLAVE_COM);
						K_Intrp_Event_Signal(0, SlaveSlot, EVENT_SLAVE_COM);
						break;
		case '\n'   :	
						break;
		default		:	SaveCharacter(chInpChar);
						chLastInpChar = '0';
						break;
	}
  }
  // USER CODE END

  U1C0_PSCR   |= 0x4000;         // clear PSR_RIF
  }

  // USER CODE BEGIN (ASC0IC,15)
	K_OS_Intrp_Exit();						// no call right CMX interrupt exit routine. MUST be
											// last instruction within interrupt's code
  // USER CODE END

} //  End of function U1C0_ASC_vi0IC




// USER CODE BEGIN (U1C0_General,10)
/* ******************************************************************************
 *  FUNCTION:	UART5_GetChar
 *  PARAMETERS:	none
 *  DESCRIPTION:liest ein Zeichen aus dem stdin-buffer
 *  RETURNS:	- character from stdin-buffer
 *              - EOF wenn kein Zeichen eingegangen ist
 ******************************************************************************** */
int UART5_GetChar(void)
{
	register int Char;

	//if(UART5_Tail != UART5_Head)
	if(UART5_Tail < UART5_Head)
	{
		Char = *UART5_Tail;
		CheckAndWrap(UART5_Tail);
	}else
	{
		Char = EOF;
	}
	return Char;
}
int Uart5_kbHit(void)
{
	if(UART5_Tail < UART5_Head)
	{
		return TRUE;

	}else
	{
		return FALSE;
	}
}

/* ******************************************************************************
 *  FUNCTION: UART5_PutChar
 *
 *  PARAMETERS:	Pointer auf ZK, Anzahl der Elemente
 *
 *  DESCRIPTION: schreibt Zeichen auf ASC0
 *
 *  RETURNS:  Anzahl der geschriebenen Zeichen	
 ******************************************************************************** */
int UART5_PutChar(char * base, size_t siz) 
{  unsigned int i;
   for(i=0; i<siz; i++)
   { 
       while(U1C0_ASC_ubIsTxFIFOfull())
       { 
       	  WDO();	/* falls Buffer voll ist, dann warten */
	   }
	   U1C0_IN00= *base;  base++;
   }
   return siz;	
}

// USER CODE END

