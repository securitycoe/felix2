//****************************************************************************
// @Module        Interrupt Controller (INT)
// @Filename      INT.C
// @Project       30_5015_Felix.dav
//----------------------------------------------------------------------------
// @Controller    Infineon XE167F-96F66
//
// @Compiler      Tasking Classic
//
// @Codegenerator 2.2
//
// @Description   This file contains functions that use the INT module.
//
//----------------------------------------------------------------------------
// @Date          13.06.2022 10:15:38
//
//****************************************************************************

// USER CODE BEGIN (INT_General,1)

// USER CODE END



//****************************************************************************
// @Project Includes
//****************************************************************************

#include "MAIN.H"

// USER CODE BEGIN (INT_General,2)

// USER CODE END


//****************************************************************************
// @Macros
//****************************************************************************

// USER CODE BEGIN (INT_General,3)

// USER CODE END


//****************************************************************************
// @Defines
//****************************************************************************

// USER CODE BEGIN (INT_General,4)

// USER CODE END


//****************************************************************************
// @Typedefs
//****************************************************************************

// USER CODE BEGIN (INT_General,5)

// USER CODE END


//****************************************************************************
// @Imported Global Variables
//****************************************************************************

// USER CODE BEGIN (INT_General,6)

// USER CODE END


//****************************************************************************
// @Global Variables
//****************************************************************************

// USER CODE BEGIN (INT_General,7)
unsigned short Ex0Flag = FALSE;
unsigned short Ex1Flag = FALSE;

// USER CODE END


//****************************************************************************
// @External Prototypes
//****************************************************************************

// USER CODE BEGIN (INT_General,8)

// USER CODE END




//****************************************************************************
// @Prototypes Of Local Functions
//****************************************************************************

// USER CODE BEGIN (INT_General,9)

// USER CODE END


//****************************************************************************
// @Function      void INT_vInit(void) 
//
//----------------------------------------------------------------------------
// @Description   This is the initialization function of the INT function 
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
// @Date          13.06.2022
//
//****************************************************************************

// USER CODE BEGIN (Init,1)

// USER CODE END

void INT_vInit(void)
{
  // USER CODE BEGIN (Init,2)

  // USER CODE END

  ///  -----------------------------------------------------------------------
  ///  Configuration of the Input and output Channel 0:
  ///  -----------------------------------------------------------------------
  ///  - input from pin P2.1 (ERU_0A0)
  ///  - interrupt edge is not selected
  ///  - Interrupt node is not selected


  ///  -----------------------------------------------------------------------
  ///  Configuration of the Input and output Channel 1:
  ///  -----------------------------------------------------------------------
  ///  - input from pin P2.2 (ERU_1A0)
  ///  - interrupt on both rising and falling edge
  ///  - CC2_CC25IC is selected as interrupt node.


  SCU_EXICON1   |= 0x001D;       // load the external Input control register 1
  SCU_EXOCON1   |= 0x0010;       // load the external output trigger control 
                                 // 1 register

  ///  -----------------------------------------------------------------------
  ///  Configuration of used interrupts :
  ///  -----------------------------------------------------------------------
  ///  CC2 Ch16INT service request node configuration:
  ///  - CC2 Ch16INT interrupt priority level (ILVL) = 11
  ///  - CC2 Ch16INT interrupt group level (GLVL) = 2
  ///  - CC2 Ch16INT group priority extension (GPX) = 0

  CC2_CC16IC     =  0x006E;     

  ///  CC2 Ch17INT service request node configuration:
  ///  - CC2 Ch17INT interrupt priority level (ILVL) = 11
  ///  - CC2 Ch17INT interrupt group level (GLVL) = 3
  ///  - CC2 Ch17INT group priority extension (GPX) = 0

  CC2_CC17IC     =  0x006F;     

  ///  CC2 Ch18INT service request node configuration:
  ///  - CC2 Ch18INT interrupt priority level (ILVL) = 10
  ///  - CC2 Ch18INT interrupt group level (GLVL) = 3
  ///  - CC2 Ch18INT group priority extension (GPX) = 0

  CC2_CC18IC     =  0x006B;     

  ///  CC2 Ch25INT  service request node configuration:
  ///  - CC2 Ch25INT  interrupt priority level (ILVL) = 11
  ///  - CC2 Ch25INT  interrupt group level (GLVL) = 1
  ///  - CC2 Ch25INT  group priority extension (GPX) = 0

  CC2_CC25IC     =  0x006D;     

  SCU_ISSR      |=  0x0300;      // Load Interrupt Source Select Register



  // USER CODE BEGIN (INT_Function,3)

  // USER CODE END

} //  End of function INT_vInit


//****************************************************************************
// @Function      void INT_viEx1(void) 
//
//----------------------------------------------------------------------------
// @Description   This is the interrupt service routine for the interrupt 
//                input from external request unit (ERU_1A0). It is called 
//                when the configured event is captured.
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
// @Date          13.06.2022
//
//****************************************************************************

// USER CODE BEGIN (Ex1,1)

// USER CODE END

_interrupt(CC2_CC25INT)  void INT_viEx1(void)
{
  // USER CODE BEGIN (Ex1,4)
#ifndef _CF_NRTL
	if(P2_IN_P2)	// hight active
	{
		//dbPrint(">>>>> P2_IN_P2 SLOMO : high, EVENT\r");
		//if(g_EnableEstopEvent)K_Intrp_Event_Signal(3, 0, SLOMO_EVENT);
		K_Intrp_Event_Signal(3, 0, SLOMO_EVENT);
	}
	else
	{
		//dbPrint(">>>>> P2_IN_P2 SLOMO : low, RESET EVENT\r");
		//if(g_EnableEstopEvent)K_Intrp_Event_Signal(3, 0, SLOMO_RESET_EVENT);
		K_Intrp_Event_Signal(3, 0, SLOMO_RESET_EVENT);
	}
//	K_Intrp_Event_Signal(3, 0, SLOMO_EVENT);
	Ex1Flag = TRUE;
#endif
	K_OS_Intrp_Exit();
  // USER CODE END

} //  End of function INT_viEx1




// USER CODE BEGIN (INT_General,10)
void Reset_Ex0Flag(void)
{
	Ex0Flag = FALSE;
}
unsigned short Get_Ex0Flag(void)
{
	return Ex0Flag;
}
void Reset_Ex1Flag(void)
{
	Ex1Flag = FALSE;
}
unsigned short Get_Ex1Flag(void)
{
	return Ex1Flag;
}

// USER CODE END

