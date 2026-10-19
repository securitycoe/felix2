//****************************************************************************
// @Module        General Purpose Timer Unit (GPT2)
// @Filename      GPT2.C
// @Project       30_5015_Felix.dav
//----------------------------------------------------------------------------
// @Controller    Infineon XE167F-96F66
//
// @Compiler      Tasking Classic
//
// @Codegenerator 2.2
//
// @Description   This file contains functions that use the GPT2 module.
//
//----------------------------------------------------------------------------
// @Date          07.03.2016 10:02:22
//
//****************************************************************************

// USER CODE BEGIN (GPT2_General,1)

// USER CODE END

/*
******************************************************************************
@Note

Timer 5 für Warten und Timeout Basis 1ms
Timer 6 Zeitverzögerung zum Einstellen des Haltestroms der Antriebe

******************************************************************************
*/


//****************************************************************************
// @Project Includes
//****************************************************************************

#include "MAIN.H"

// USER CODE BEGIN (GPT2_General,2)
#include  "GPT2.h"

// USER CODE END


//****************************************************************************
// @Macros
//****************************************************************************

// USER CODE BEGIN (GPT2_General,3)

// USER CODE END


//****************************************************************************
// @Defines
//****************************************************************************

// USER CODE BEGIN (GPT2_General,4)

// USER CODE END


//****************************************************************************
// @Typedefs
//****************************************************************************

// USER CODE BEGIN (GPT2_General,5)

// USER CODE END


//****************************************************************************
// @Imported Global Variables
//****************************************************************************

// USER CODE BEGIN (GPT2_General,6)

// USER CODE END


//****************************************************************************
// @Global Variables
//****************************************************************************

// USER CODE BEGIN (GPT2_General,7)
uword uwZDrvCurrentDelay;
unsigned short usT5_WaitTime0= 0;
unsigned short usT5_TimeOut= 0;
unsigned short usT5_BlinkTime= 0;
//unsigned short usT5_FlashTime= 0;
unsigned short usT6_ZDrive = 0;
 // USER CODE END


//****************************************************************************
// @External Prototypes
//****************************************************************************

// USER CODE BEGIN (GPT2_General,8)

// USER CODE END



//****************************************************************************
// @Prototypes Of Local Functions
//****************************************************************************

// USER CODE BEGIN (GPT2_General,9)

// USER CODE END


//****************************************************************************
// @Function      void GPT2_vInit(void) 
//
//----------------------------------------------------------------------------
// @Description   This is the initialization function of the GPT2 function 
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

void GPT2_vInit(void)
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
  ///  Configuration of Timer Block Prescaler 2:
  ///  -----------------------------------------------------------------------
  ///  - prescaler for timer block 2 is 2


  ///  -----------------------------------------------------------------------
  ///  Configuration of the GPT2 Core Timer 5:
  ///  -----------------------------------------------------------------------
  ///  - timer 5 works in timer mode
  ///  - prescaler factor is 2
  ///  - up/down control bit is set
  ///  - external up/down control is disabled
  ///  - timer 5 remote control is disabled

  GPT12E_T5CON   =  0x0080;      // load timer 5 control register
  GPT12E_T5      =  0x80E7;      // load timer 5 register

  ///  -----------------------------------------------------------------------
  ///  Configuration of the GPT2 Core Timer 6:
  ///  -----------------------------------------------------------------------
  ///  - timer 6 works in timer mode
  ///  - prescaler factor is 256
  ///  - up/down control bit is set
  ///  - external up/down control is disabled
  ///  - alternate output function T6OUT (P6.2) is disabled
  ///  - alternate output function T6OUT (P7.0) is disabled
  ///  - timer 6 output toggle latch (T6OTL) is set to 0
  ///  - timer 6 is not cleared on a capture

  GPT12E_T6CON   =  0x0887;      // load timer 6 control register
  GPT12E_T6      =  0xFBC4;      // load timer 6 register

  ///  -----------------------------------------------------------------------
  ///  Configuration of the GPT2 CAPREL:
  ///  -----------------------------------------------------------------------
  ///  - capture T5 into CAPREL is disabled
  ///  - capture trigger from pin CAPIN
  ///  - capure is disabled
  ///  - timer 5 is not cleared on a capture
  ///  - timer 5 is just captured without any correction

  GPT12E_T5CON  |=  0x0000;      // load timer 5 control register
  GPT12E_CAPREL  =  0x0000;      // load CAPREL register

  ///  -----------------------------------------------------------------------
  ///  Configuration of the used GPT2 Port Pins:
  ///  -----------------------------------------------------------------------


  ///  -----------------------------------------------------------------------
  ///  Configuration of the used GPT2 Interrupts:
  ///  -----------------------------------------------------------------------
  ///  timer 5 service request node configuration:
  ///  - timer 5 interrupt priority level (ILVL) = 5
  ///  - timer 5 interrupt group level (GLVL) = 0
  ///  - timer 5 group priority extension (GPX) = 0

  GPT12E_T5IC    =  0x0054;     

  ///  timer 6 service request node configuration:
  ///  - timer 6 interrupt priority level (ILVL) = 5
  ///  - timer 6 interrupt group level (GLVL) = 1
  ///  - timer 6 group priority extension (GPX) = 0

  GPT12E_T6IC    =  0x0055;     



  // USER CODE BEGIN (GPT2_Function,3)

  // USER CODE END

  GPT12E_T5CON_T5R  =  1;        // set timer 5 run bit

  GPT12E_T6CON_T6R  =  1;        // set timer 6 run bit

} //  End of function GPT2_viCAPREL

//****************************************************************************
// @Function      void GPT2_viTmr6(void) 
//
//----------------------------------------------------------------------------
// @Description   This is the interrupt service routine for the GPT2 timer 6. 
//                It is called up in the case of over or underflow of the 
//                timer 6 register.
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

// USER CODE BEGIN (Tmr6,1)
//#pragma noframe	//Achtung, geht nur wenn Interrupt keine Register ... ändert

// USER CODE END

_interrupt(T6INT)  void GPT2_viTmr6(void)
{
  // USER CODE BEGIN (Tmr6,2)
	GPT2_vLoadTmr(GPT2_TIMER_6, 0xFBC4);
#pragma asm
	ADD		_usT6_ZDrive,ONES
	SUBC	_usT6_ZDrive,ONES
#pragma endasm
   //GPT12E_T6=  0x64B4;		// load timer register
	K_OS_Intrp_Exit();
  // USER CODE END

} //  End of function GPT2_viTmr6


//****************************************************************************
// @Function      void GPT2_viTmr5(void) 
//
//----------------------------------------------------------------------------
// @Description   This is the interrupt service routine for the GPT2 timer 5. 
//                It is called up in the case of over or underflow of the 
//                timer 5 register.
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

// USER CODE BEGIN (Tmr5,1)
//#pragma noframe	//Achtung, geht nur wenn Interrupt keine Register ... ändert
// USER CODE END

_interrupt(T5INT)  void GPT2_viTmr5(void)
{
  // USER CODE BEGIN (Tmr5,2)
	GPT2_vLoadTmr(GPT2_TIMER_5, 0x80E7);
#pragma asm
	ADD		_usT5_WaitTime0,ONES
	SUBC	_usT5_WaitTime0,ONES
	ADD		_usT5_TimeOut,ONES
	SUBC	_usT5_TimeOut,ONES
	ADD		_usT5_BlinkTime,ONES
	SUBC	_usT5_BlinkTime,ONES
;	ADD		_usT5_FlashTime,ONES
;	SUBC	_usT5_FlashTime,ONES

#pragma endasm
	//_srvwdt();
	WDO();

	K_OS_Tick_Update();						// call the CMX tick function

	K_OS_Intrp_Exit();						// no call right CMX interrupt exit routine. MUST be
											// last instruction within interrupt's code

  // USER CODE END

} //  End of function GPT2_viTmr5




// USER CODE BEGIN (GPT2_General,10)
void SetTimeout(unsigned short Value)
{
	usT5_TimeOut = Value;
}
unsigned short GetTimeout(void)
{
	return usT5_TimeOut;
}

void SetTimeBlink(unsigned short Value)
{
	usT5_BlinkTime = Value;
}
unsigned short GetTimeBlink(void)
{
	return usT5_BlinkTime;
}
/*
void SetTimeFlash(unsigned short Value)
{
	usT5_FlashTime = Value;
}
unsigned short GetTimeFlash(void)
{
	return usT5_FlashTime;
}
*/

void SetTimeZDrive(unsigned short Value)
{
	usT6_ZDrive = Value;
}
unsigned short GetTimeZDrive(void)
{
	return usT6_ZDrive;
}


void Wait(unsigned short Value)
{
	usT5_WaitTime0 = Value;
	while(usT5_WaitTime0) {WDO();}
}
// USER CODE END

