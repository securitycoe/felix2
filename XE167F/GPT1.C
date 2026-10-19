//****************************************************************************
// @Module        General Purpose Timer Unit (GPT1)
// @Filename      GPT1.C
// @Project       30_5015_Felix.dav
//----------------------------------------------------------------------------
// @Controller    Infineon XE167F-96F66
//
// @Compiler      Tasking Classic
//
// @Codegenerator 2.2
//
// @Description   This file contains functions that use the GPT1 module.
//
//----------------------------------------------------------------------------
// @Date          07.03.2016 10:02:22
//
//****************************************************************************

// USER CODE BEGIN (GPT1_General,1)

// USER CODE END



//****************************************************************************
// @Project Includes
//****************************************************************************

#include "MAIN.H"

// USER CODE BEGIN (GPT1_General,2)

// USER CODE END


//****************************************************************************
// @Macros
//****************************************************************************

// USER CODE BEGIN (GPT1_General,3)

// USER CODE END


//****************************************************************************
// @Defines
//****************************************************************************

// USER CODE BEGIN (GPT1_General,4)

// USER CODE END


//****************************************************************************
// @Typedefs
//****************************************************************************

// USER CODE BEGIN (GPT1_General,5)

// USER CODE END


//****************************************************************************
// @Imported Global Variables
//****************************************************************************

// USER CODE BEGIN (GPT1_General,6)

// USER CODE END


//****************************************************************************
// @Global Variables
//****************************************************************************

// USER CODE BEGIN (GPT1_General,7)
uword uwWaitFlg;
uword uwT4WaitFlg;
// USER CODE END


//****************************************************************************
// @External Prototypes
//****************************************************************************

// USER CODE BEGIN (GPT1_General,8)

// USER CODE END


//****************************************************************************
// @Prototypes Of Local Functions
//****************************************************************************

// USER CODE BEGIN (GPT1_General,9)

// USER CODE END


//****************************************************************************
// @Function      void GPT1_vInit(void) 
//
//----------------------------------------------------------------------------
// @Description   This is the initialization function of the GPT1 function 
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

void GPT1_vInit(void)
{
  // USER CODE BEGIN (Init,2)

  // USER CODE END
  ///  -----------------------------------------------------------------------
  ///  Configuration of Timer Block Prescaler 1:
  ///  -----------------------------------------------------------------------
  GPT12E_KSCCFG  =  0x0003;      // Module Enable

  _nop();  // one cycle delay 

  _nop();  // one cycle delay 



  ///  -----------------------------------------------------------------------
  ///  Configuration of Timer Block Prescaler 1:
  ///  -----------------------------------------------------------------------
  ///  - prescaler for timer block 1 is 8


  ///  -----------------------------------------------------------------------
  ///  Configuration of the GPT1 Core Timer 3:
  ///  -----------------------------------------------------------------------
  ///  - timer 3 works in timer mode
  ///  - external up/down control is disabled
  ///  - prescaler factor is 8
  ///  - up/down control bit is set
  ///  - alternate output function T3OUT (P7.0) is disabled
  ///  - timer 3 output toggle latch (T3OTL) is set to 0
  ///  - timer 3 run bit is reset

  GPT12E_T3CON   =  0x0080;      // load timer 3 control register
  GPT12E_T3      =  0x0008;      // load timer 3 register
  ///  - prescaler for timer block 1 is 8

  ///  -----------------------------------------------------------------------
  ///  Configuration of the GPT1 Auxiliary Timer 2:
  ///  -----------------------------------------------------------------------
  ///  - timer 2 works in timer mode
  ///  - external up/down control is disabled
  ///  - prescaler factor is 64
  ///  - up/down control bit is reset
  ///  - timer 2 run bit is reset

  GPT12E_T2CON   =  0x0003;      // load timer 2 control register
  GPT12E_T2      =  0x0000;      // load timer 2 register
  ///  - prescaler for timer block 1 is 8

  ///  -----------------------------------------------------------------------
  ///  Configuration of the GPT1 Auxiliary Timer 4:
  ///  -----------------------------------------------------------------------
  ///  - timer 4 works in timer mode
  ///  - external up/down control is disabled
  ///  - prescaler factor is 8
  ///  - up/down control bit is set
  ///  - timer 4 run bit is reset

  GPT12E_T4CON   =  0x0080;      // load timer 4 control register
  GPT12E_T4      =  0x0008;      // load timer 4 register
  ///  - prescaler for timer block 1 is 8

  ///  -----------------------------------------------------------------------
  ///  Configuration of the used GPT1 Port Pins:
  ///  -----------------------------------------------------------------------


  ///  -----------------------------------------------------------------------
  ///  Configuration of the used GPT1 Interrupts:
  ///  -----------------------------------------------------------------------
  ///  timer 3 service request node configuration:
  ///  - timer 3 interrupt priority level (ILVL) = 6
  ///  - timer 3 interrupt group level (GLVL) = 0
  ///  - timer 3 group priority extension (GPX) = 0

  GPT12E_T3IC    =  0x0058;     

  ///  timer 4 service request node configuration:
  ///  - timer 4 interrupt priority level (ILVL) = 2
  ///  - timer 4 interrupt group level (GLVL) = 0
  ///  - timer 4 group priority extension (GPX) = 0

  GPT12E_T4IC    =  0x0048;     


  // USER CODE BEGIN (GPT1_Function,3)

  // USER CODE END

} //  End of function GPT1_viTmr4


//****************************************************************************
// @Function      void GPT1_viTmr3(void) 
//
//----------------------------------------------------------------------------
// @Description   This is the interrupt service routine for the GPT1 timer 3. 
//                It is called up in the case of over or underflow of the 
//                timer 3 register.
//                If the incremental interface mode is selected it is called 
//                up if count edge or count direction was detected.
//                
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

// USER CODE BEGIN (Tmr3,1)
//#pragma noframe	//Achtung, geht nur wenn Interrupt keine Register ... ändert

// USER CODE END

_interrupt(T3INT)  void GPT1_viTmr3(void)
{
  // USER CODE BEGIN (Tmr3,2)
//;ONES=0xffff (-1), falls ein Überlauf kommt, wird dieser mit SUBC korrigiert
#pragma asm
  ADD  _uwWaitFlg,ONES	   
  SUBC _uwWaitFlg,ONES
#pragma endasm

  // USER CODE END


  // USER CODE BEGIN (Tmr3,5)
	K_OS_Intrp_Exit();
  // USER CODE END

} //  End of function GPT1_viTmr3


//****************************************************************************
// @Function      void GPT1_viTmr4(void) 
//
//----------------------------------------------------------------------------
// @Description   This is the interrupt service routine for the GPT1 timer 4. 
//                It is called up in the case of over or underflow of the 
//                timer 4 register.
//                If the incremental interface mode is selected and the 
//                interrupt for this mode is not disabled it is called up if 
//                count edge or count direction was detected.
//                
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

// USER CODE BEGIN (Tmr4,1)

// USER CODE END

_interrupt(T4INT)  void GPT1_viTmr4(void)
{
  // USER CODE BEGIN (Tmr4,2)
#pragma asm
  ADD  _uwT4WaitFlg,ONES	   
  SUBC _uwT4WaitFlg,ONES
#pragma endasm

  // USER CODE END


  // USER CODE BEGIN (Tmr4,5)
	K_OS_Intrp_Exit();
  // USER CODE END

} //  End of function GPT1_viTmr4




// USER CODE BEGIN (GPT1_General,10)
void ShortTimer(uword time)
{
	uwWaitFlg= TRUE;
	GPT12E_T3=  0x0005 * time; //load timer 3 register, 1µs
	T3R=  1;        	// set timer 3 run bit
	while(uwWaitFlg) WDO();
	T3R= 0;
}

void ShortTimerIMS(uword time)
{
	uwT4WaitFlg= TRUE;
	GPT12E_T4=  0x0005 * time; //load timer 4 register, 1µs
	T4R=  1;        	// set timer 4 run bit
	while(uwT4WaitFlg) WDO();
	T4R= 0;
}

// USER CODE END

