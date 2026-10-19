//****************************************************************************
// @Module        Capture / Compare Unit 2 (CAPCOM2)
// @Filename      CC2.C
// @Project       30_5015_Felix.dav
//----------------------------------------------------------------------------
// @Controller    Infineon XE167F-96F66
//
// @Compiler      Tasking Classic
//
// @Codegenerator 2.2
//
// @Description   This file contains functions that use the CC2 module.
//
//----------------------------------------------------------------------------
// @Date          07.03.2016 10:02:22
//
//****************************************************************************

// USER CODE BEGIN (CC2_General,1)

// USER CODE END

/*
******************************************************************************
@Note

T7 generiert Ansteuer-Frequenz des A3979 auf LP (Ausgang P4.4)
CC28 erzeugt Impulslänge für P4.4

T8 generiert Ansteuer-Frequenz für externe Ansteuerung (Ausgang P4.5)
CC29 erzeugt Impulslänge für P4.5

******************************************************************************
*/


//****************************************************************************
// @Project Includes
//****************************************************************************

#include "MAIN.H"

// USER CODE BEGIN (CC2_General,2)
#include "Felix_Hardware.h"
#include "Felix_Misc.h"
// USER CODE END


//****************************************************************************
// @Macros
//****************************************************************************

// USER CODE BEGIN (CC2_General,3)

// USER CODE END


//****************************************************************************
// @Defines
//****************************************************************************

// USER CODE BEGIN (CC2_General,4)

// USER CODE END


//****************************************************************************
// @Typedefs
//****************************************************************************

// USER CODE BEGIN (CC2_General,5)

// USER CODE END


//****************************************************************************
// @Imported Global Variables
//****************************************************************************

// USER CODE BEGIN (CC2_General,6)

// USER CODE END


//****************************************************************************
// @Global Variables
//****************************************************************************

// USER CODE BEGIN (CC2_General,7)
unsigned short toggle_CC21 = 1;
unsigned short toggle_CC22 = 1;
// USER CODE END


//****************************************************************************
// @External Prototypes
//****************************************************************************

// USER CODE BEGIN (CC2_General,8)

// USER CODE END


//****************************************************************************
// @Prototypes Of Local Functions
//****************************************************************************

// USER CODE BEGIN (CC2_General,9)

// USER CODE END


//****************************************************************************
// @Function      void CC2_vInit(void) 
//
//----------------------------------------------------------------------------
// @Description   This is the initialization function of the CAPCOM2 function 
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

void CC2_vInit(void)
{
  // USER CODE BEGIN (Init,2)

  // USER CODE END

  ///  -----------------------------------------------------------------------
  ///  Configuration of  Kernel state configuration Control:
  ///  -----------------------------------------------------------------------
  ///  - the Peripheral is supplied with clock signal for CAPCOM2 unit
  ///  - the CC2 module clock = 66,00 MHz

  CC2_KSCCFG     =  0x0003;      // load CAPCOM2 Kernel state configuration 
                                 // register


  _nop();  // one cycle delay 

  _nop();  // one cycle delay 

  ///  -----------------------------------------------------------------------
  ///  Configuration of CAPCOM2 Control:
  ///  -----------------------------------------------------------------------
  ///  - the contents of the CC2_OUT register is visble at output port
  ///  - the contents of the port register is changed by the CAPCOM2 unit

  ///  -----------------------------------------------------------------------
  ///  Configuration of CAPCOM2 Timer 7:
  ///  -----------------------------------------------------------------------
  ///  - timer 7 works in timer mode
  ///  - prescaler factor is 1024
  ///  - timer 7 run bit is reset

  ///  -----------------------------------------------------------------------
  ///  Configuration of CAPCOM2 Timer 8:
  ///  -----------------------------------------------------------------------
  ///  - timer 8 works in timer mode
  ///  - prescaler factor is 1024
  ///  - timer 8 run bit is reset


  CC2_T78CON     =  0x0707;      // load CAPCOM2 timer 7 and timer 8 control 
                                 // register

  CC2_T7         =  0xFD7B;      // load CAPCOM2 timer 7 register

  CC2_T7REL      =  0xFD7B;      // load CAPCOM2 timer 7 reload register

  CC2_T8         =  0xFD7B;      // load CAPCOM2 timer 8 register

  CC2_T8REL      =  0xFD7B;      // load CAPCOM2 timer 8 reload register

  ///  -----------------------------------------------------------------------
  ///  Configuration of the used CAPCOM2 Timer Port Pins:
  ///  -----------------------------------------------------------------------


  ///  -----------------------------------------------------------------------
  ///  Configuration of the used CAPCOM2 Timer Interrupts:
  ///  -----------------------------------------------------------------------
  ///  Tmr7 service request node configuration:
  ///  - Tmr7 interrupt priority level (ILVL) = 4
  ///  - Tmr7 interrupt group level (GLVL) = 1
  ///  - Tmr7 group priority extension (GPX) = 0

  CC2_T7IC       =  0x0051;     


  ///  Tmr8 service request node configuration:
  ///  - Tmr8 interrupt priority level (ILVL) = 4
  ///  - Tmr8 interrupt group level (GLVL) = 2
  ///  - Tmr8 group priority extension (GPX) = 0

  CC2_T8IC       =  0x0052;     


  ///  -----------------------------------------------------------------------
  ///  Configuration of the used CAPCOM2 Channel 16:
  ///  -----------------------------------------------------------------------
  ///  - capture on positive & negative transition (both edges) at pin CC16IO 
  ///    (P2.3)
  ///  - CC16 allocated to CAPCOM2 timer 7
  ///  - single event mode is disabled


  ///  -----------------------------------------------------------------------
  ///  Configuration of the used CAPCOM2 Channel 17:
  ///  -----------------------------------------------------------------------
  ///  - capture on negative transition (falling edge) at pin CC17IO (P2.4)
  ///  - CC17 allocated to CAPCOM2 timer 7
  ///  - single event mode is disabled


  ///  -----------------------------------------------------------------------
  ///  Configuration of the used CAPCOM2 Channel 18:
  ///  -----------------------------------------------------------------------
  ///  - capture on positive & negative transition (both edges) at pin CC18IO 
  ///    (P2.5)
  ///  - CC18 allocated to CAPCOM2 timer 7
  ///  - single event mode is disabled


  ///  -----------------------------------------------------------------------
  ///  Configuration of the used CAPCOM2 Channel 19:
  ///  -----------------------------------------------------------------------
  ///  - channel 19 is disabled

  ///  -----------------------------------------------------------------------
  ///  Configuration of the used CAPCOM2 Channel 20:
  ///  -----------------------------------------------------------------------
  ///  - channel 20 is disabled

  CC2_CC20       =  0xFFF0;      // load CAPCOM2 channel 20 register

  ///  -----------------------------------------------------------------------
  ///  Configuration of the used CAPCOM2 Channel 21:
  ///  -----------------------------------------------------------------------
  ///  - compare mode 2:  interrupt only
  ///  - CC21 allocated to CAPCOM2 timer 7
  ///  - single event mode is disabled


  ///  -----------------------------------------------------------------------
  ///  Configuration of the used CAPCOM2 Channel 22:
  ///  -----------------------------------------------------------------------
  ///  - compare mode 2:  interrupt only
  ///  - CC22 allocated to CAPCOM2 timer 8
  ///  - single event mode is disabled


  ///  -----------------------------------------------------------------------
  ///  Configuration of the used CAPCOM2 Channel 23:
  ///  -----------------------------------------------------------------------
  ///  - channel 23 is disabled

  ///  -----------------------------------------------------------------------
  ///  Configuration of the used CAPCOM2 Channel 24:
  ///  -----------------------------------------------------------------------
  ///  - channel 24 is disabled

  ///  -----------------------------------------------------------------------
  ///  Configuration of the used CAPCOM2 Channel 25:
  ///  -----------------------------------------------------------------------
  ///  - channel 25 is disabled

  ///  -----------------------------------------------------------------------
  ///  Configuration of the used CAPCOM2 Channel 26:
  ///  -----------------------------------------------------------------------
  ///  - channel 26 is disabled

  ///  -----------------------------------------------------------------------
  ///  Configuration of the used CAPCOM2 Channel 27:
  ///  -----------------------------------------------------------------------
  ///  - channel 27 is disabled

  ///  -----------------------------------------------------------------------
  ///  Configuration of the used CAPCOM2 Channel 28:
  ///  -----------------------------------------------------------------------
  ///  - channel 28 is disabled

  CC2_CC28       =  0xFFF2;      // load CAPCOM2 channel 28 register

  ///  -----------------------------------------------------------------------
  ///  Configuration of the used CAPCOM2 Channel 29:
  ///  -----------------------------------------------------------------------
  ///  - channel 29 is disabled

  CC2_CC29       =  0xFFF2;      // load CAPCOM2 channel 29 register

  ///  -----------------------------------------------------------------------
  ///  Configuration of the used CAPCOM2 Channel 30:
  ///  -----------------------------------------------------------------------
  ///  - channel 30 is disabled

  ///  -----------------------------------------------------------------------
  ///  Configuration of the used CAPCOM2 Channel 31:
  ///  -----------------------------------------------------------------------
  ///  - channel 31 is disabled

  CC2_M4         =  0x0323;      // load CAPCOM2 mode register 4
  CC2_M5         =  0x0E60;      // load CAPCOM2 mode register 5

  ///  -----------------------------------------------------------------------
  ///  Configuration of the used CAPCOM2 Channel Port Pins:
  ///  -----------------------------------------------------------------------
  ///  - P2.3 is used for CAPCOM2 input(CC2_16)
  ///  - P2.4 is used for CAPCOM2 input(CC2_17)
  ///  - P2.5 is used for CAPCOM2 input(CC2_18)


  ///  -----------------------------------------------------------------------
  ///  Configuration of the used CAPCOM2 Channels Interrupts:
  ///  -----------------------------------------------------------------------
  ///  CC16 service request node configuration:
  ///  - CC16 interrupt priority level (ILVL) = 11
  ///  - CC16 interrupt group level (GLVL) = 2
  ///  - CC16 group priority extension (GPX) = 0

  CC2_CC16IC     =  0x006E;     


  ///  CC17 service request node configuration:
  ///  - CC17 interrupt priority level (ILVL) = 11
  ///  - CC17 interrupt group level (GLVL) = 3
  ///  - CC17 group priority extension (GPX) = 0

  CC2_CC17IC     =  0x006F;     


  ///  CC18 service request node configuration:
  ///  - CC18 interrupt priority level (ILVL) = 10
  ///  - CC18 interrupt group level (GLVL) = 3
  ///  - CC18 group priority extension (GPX) = 0

  CC2_CC18IC     =  0x006B;     


  ///  CC21 service request node configuration:
  ///  - CC21 interrupt priority level (ILVL) = 3
  ///  - CC21 interrupt group level (GLVL) = 0
  ///  - CC21 group priority extension (GPX) = 0

  CC2_CC21IC     =  0x004C;     


  ///  CC22 service request node configuration:
  ///  - CC22 interrupt priority level (ILVL) = 3
  ///  - CC22 interrupt group level (GLVL) = 1
  ///  - CC22 group priority extension (GPX) = 0

  CC2_CC22IC     =  0x004D;     




  // USER CODE BEGIN (CC22,3)

  // USER CODE END

} //  End of function CC2_viCC22


//****************************************************************************
// @Function      void CC2_viTmr7(void) 
//
//----------------------------------------------------------------------------
// @Description   This is the interrupt service routine for the CAPCOM2 timer 
//                7. It is called when overflow of the timer 7 register 
//                occurs.
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

// USER CODE BEGIN (Tmr7,1)

// USER CODE END

_interrupt(CC2_T7INT)  void CC2_viTmr7(void)
{
  // USER CODE BEGIN (Tmr7,2)

	//CC2_vLoadTmr_CC2_TIMER_7(CC2_T7REL);
	CC2_T7 = CC2_T7REL;
	//CC2_vLoadTmr(CC2_TIMER_7, CC2_T7REL);
	K_OS_Intrp_Exit();
  // USER CODE END

} //  End of function CC2_viTmr7


//****************************************************************************
// @Function      void CC2_viTmr8(void) 
//
//----------------------------------------------------------------------------
// @Description   This is the interrupt service routine for the CAPCOM2 timer 
//                8. It is called when overflow of the timer 8 register 
//                occurs.
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

// USER CODE BEGIN (Tmr8,1)

// USER CODE END

_interrupt(CC2_T8INT)  void CC2_viTmr8(void)
{
  // USER CODE BEGIN (Tmr8,2)
  CC2_T8 = CC2_T8REL;
	K_OS_Intrp_Exit();
  // USER CODE END

} //  End of function CC2_viTmr8


//****************************************************************************
// @Function      void CC2_viCC16(void) 
//
//----------------------------------------------------------------------------
// @Description   This is the interrupt service routine for the CAPCOM2 
//                register CC16. If the content of the corresponding compare 
//                timer (configurable) equals the content of the 
//                capture/compare register CC16 or if a capture event occurs 
//                at the associated port pin, the interrupt request flag is 
//                set and an interrupt is triggered (only if enabled).
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

// USER CODE BEGIN (CC16,1)

// USER CODE END

_interrupt(CC2_CC16INT)  void CC2_viCC16(void)
{
  // USER CODE BEGIN (CC16,2)
	//if(g_EnableEstopEvent) K_Intrp_Event_Signal(3, 0, TA_SERVICE_EVENT);
	K_Intrp_Event_Signal(3, 0, TASTEN_EVENT);

/*	if(P2_IN_P3)	// hight active
	{
		dbPrint(">>>>> P2_IN_P3 : high, EVENT\r");
		//K_Intrp_Event_Signal(3, 0, TASTE_GEDRUECKT_EVENT);
	}
	else
	{
		dbPrint(">>>>> P2_IN_P3 : low, RESET EVENT\r");
		//K_Intrp_Event_Signal(3, 0, TASTE_LOSGELASSEN_EVENT);
	}
*/
	K_OS_Intrp_Exit();
  // USER CODE END

} //  End of function CC2_viCC16


//****************************************************************************
// @Function      void CC2_viCC17(void) 
//
//----------------------------------------------------------------------------
// @Description   This is the interrupt service routine for the CAPCOM2 
//                register CC17. If the content of the corresponding compare 
//                timer (configurable) equals the content of the 
//                capture/compare register CC17 or if a capture event occurs 
//                at the associated port pin, the interrupt request flag is 
//                set and an interrupt is triggered (only if enabled).
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

// USER CODE BEGIN (CC17,1)

// USER CODE END

_interrupt(CC2_CC17INT)  void CC2_viCC17(void)
{
  // USER CODE BEGIN (CC17,2)
	K_OS_Intrp_Exit();
  // USER CODE END

} //  End of function CC2_viCC17


//****************************************************************************
// @Function      void CC2_viCC18(void) 
//
//----------------------------------------------------------------------------
// @Description   This is the interrupt service routine for the CAPCOM2 
//                register CC18. If the content of the corresponding compare 
//                timer (configurable) equals the content of the 
//                capture/compare register CC18 or if a capture event occurs 
//                at the associated port pin, the interrupt request flag is 
//                set and an interrupt is triggered (only if enabled).
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

// USER CODE BEGIN (CC18,1)

// USER CODE END

_interrupt(CC2_CC18INT)  void CC2_viCC18(void)
{
  // USER CODE BEGIN (CC18,2)
	K_Intrp_Event_Signal(3, 0, TASTEN_EVENT);
	K_OS_Intrp_Exit();
  // USER CODE END

} //  End of function CC2_viCC18


//****************************************************************************
// @Function      void CC2_viCC21(void) 
//
//----------------------------------------------------------------------------
// @Description   This is the interrupt service routine for the CAPCOM2 
//                register CC21. If the content of the corresponding compare 
//                timer (configurable) equals the content of the 
//                capture/compare register CC21 or if a capture event occurs 
//                at the associated port pin, the interrupt request flag is 
//                set and an interrupt is triggered (only if enabled).
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

// USER CODE BEGIN (CC21,1)

// USER CODE END

_interrupt(CC2_CC21INT)  void CC2_viCC21(void)
{
  // USER CODE BEGIN (CC21,2)
	if(toggle_CC21)
	{
		xHW_LED_Green_On();
	}
	else
	{
		xHW_LED_RedGreen_Off();
	}
	toggle_CC21= !toggle_CC21;
	K_OS_Intrp_Exit();
  // USER CODE END

} //  End of function CC2_viCC21


//****************************************************************************
// @Function      void CC2_viCC22(void) 
//
//----------------------------------------------------------------------------
// @Description   This is the interrupt service routine for the CAPCOM2 
//                register CC22. If the content of the corresponding compare 
//                timer (configurable) equals the content of the 
//                capture/compare register CC22 or if a capture event occurs 
//                at the associated port pin, the interrupt request flag is 
//                set and an interrupt is triggered (only if enabled).
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

// USER CODE BEGIN (CC22,1)

// USER CODE END

_interrupt(CC2_CC22INT)  void CC2_viCC22(void)
{
  // USER CODE BEGIN (CC22,2)
	if(toggle_CC22)
	{
		xHW_LED_Yellow_On();
	}
	else
	{
		xHW_LED_Yellow_Off();
	}
	toggle_CC22= !toggle_CC22;
	K_OS_Intrp_Exit();
  // USER CODE END

} //  End of function CC2_viCC22




// USER CODE BEGIN (CC2_General,10)
void SetToggle_CC21(void)
{
	toggle_CC21 = 1;
}
void SetToggle_CC22(void)
{
	toggle_CC22 = 1;
}
void ClearToggle_CC21(void)
{
	toggle_CC21 = 0;
}
void ClearToggle_CC22(void)
{
	toggle_CC22 = 0;
}

// USER CODE END

