//****************************************************************************
// @Module        Real Time Clock (RTC)
// @Filename      RTC.C
// @Project       30_5015_Felix.dav
//----------------------------------------------------------------------------
// @Controller    Infineon XE167F-96F66
//
// @Compiler      Tasking Classic
//
// @Codegenerator 2.2
//
// @Description   This file contains functions that use the RTC module.
//
//----------------------------------------------------------------------------
// @Date          07.03.2016 10:02:23
//
//****************************************************************************

// USER CODE BEGIN (RTC_General,1)

// USER CODE END

/*
******************************************************************************
@Note

Minutenzähler für die Zwangssetzen des Liftermotors auf Haltestroms

******************************************************************************
*/


//****************************************************************************
// @Project Includes
//****************************************************************************

#include "MAIN.H"

// USER CODE BEGIN (RTC_General,2)

// USER CODE END


//****************************************************************************
// @Macros
//****************************************************************************

// USER CODE BEGIN (RTC_General,3)

// USER CODE END


//****************************************************************************
// @Defines
//****************************************************************************

// USER CODE BEGIN (RTC_General,4)

// USER CODE END


//****************************************************************************
// @Typedefs
//****************************************************************************

// USER CODE BEGIN (RTC_General,5)

// USER CODE END


//****************************************************************************
// @Imported Global Variables
//****************************************************************************

// USER CODE BEGIN (RTC_General,6)

// USER CODE END


//****************************************************************************
// @Global Variables
//****************************************************************************

// USER CODE BEGIN (RTC_General,7)

// USER CODE END


//****************************************************************************
// @External Prototypes
//****************************************************************************

// USER CODE BEGIN (RTC_General,8)

// USER CODE END


//****************************************************************************
// @Prototypes Of Local Functions
//****************************************************************************

// USER CODE BEGIN (RTC_General,9)

// USER CODE END


//****************************************************************************
// @Function      void RTC_vInit(void) 
//
//----------------------------------------------------------------------------
// @Description   This is the initialization function of the RTC function 
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

void RTC_vInit(void)
{
  // USER CODE BEGIN (Init,2)

  // USER CODE END

  ///  -----------------------------------------------------------------------
  ///  Configuration of the RTC RTC_KSCCFG Register:
  ///  -----------------------------------------------------------------------

  RTC_KSCCFG     =  0x0003;      // Module Enable

  _nop();  // one cycle delay 

  _nop();  // one cycle delay 


  SCU_RTCCLKCON  =  0x0005;      // RTC working in synchronous mode with osc. 
                                 // freq. as a input freq.

  ///  -----------------------------------------------------------------------
  ///  Configuration of the RTC Counter:
  ///  -----------------------------------------------------------------------
  ///  - RTC input frequency is 250,000 kHz
  ///  - the input clock is divided by factor 32
  ///  - the input clock is divided by factor 8
  ///  - counter is not started after initialization

  while((RTC_CON & 0x8000) == 0x0000);      // wait until access is possible
  RTC_CON        =  0x0002;      // load RTC control register

  ///  - overflow period on T14:  1,0000 [s]
  ///  - overflow period on CNT0: 1,0000 [min]
  ///  - overflow period on CNT1: 1,0000 [h]
  ///  - overflow period on CNT2: 1,0000 [days]
  ///  - overflow period on CNT3: 1,0139 [years]

  RTC_T14REL     =  0x85EE;      // load T14 count/reload register
  RTC_RTCL       =  0x13C4;      // load RTC low register
  RTC_RELL       =  0x13C4;      // load RTC reload low register
  RTC_RTCH       =  0xA4E8;      // load RTC high register
  RTC_RELH       =  0xA4E8;      // load RTC reload high register

  ///  -----------------------------------------------------------------------
  ///  Configuration of the used RTC Interrupts:
  ///  -----------------------------------------------------------------------
  ///  RTC service request node configuration:
  ///  - RTC interrupt priority level (ILVL) = 4
  ///  - RTC interrupt group level (GLVL) = 0
  ///  - RTC group priority extension (GPX) = 0

  RTC_IC         =  0x0050;     

  RTC_ISNC       =  0x0004;      // load RTC interrupt sub node register



  // USER CODE BEGIN (Init,3)

  // USER CODE END

} //  End of function RTC_vInit


//****************************************************************************
// @Function      void RTC_vRTCRun(void) 
//
//----------------------------------------------------------------------------
// @Description   This function starts the RTC timer.
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

// USER CODE BEGIN (RTCRun,1)

// USER CODE END

void RTC_vRTCRun(void)
{

  RTC_CON_RUN = 1;

} //  End of function RTC_vRTCRun


//****************************************************************************
// @Function      void RTC_vRTCStop(void) 
//
//----------------------------------------------------------------------------
// @Description   This function stops the RTC timer.
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

// USER CODE BEGIN (RTCStop,1)

// USER CODE END

void RTC_vRTCStop(void)
{

  RTC_CON_RUN = 0;

} //  End of function RTC_vRTCStop


//****************************************************************************
// @Function      void RTC_viRTC(void) 
//
//----------------------------------------------------------------------------
// @Description   This is the RTC interrupt service routine. It is called up 
//                when:
//                 - counter T14 overflows and is reloaded
//                 - counter CNT0 overflows and is reloaded
//                 - counter CNT1 overflows and is reloaded
//                 - counter CNT2 overflows and is reloaded
//                 - counter CNT3 overflows and is reloaded
//                This interrupt request may eg. be used to provide a system 
//                time tick independent of the CPU frequency, or to wake up 
//                regularly from idle mode.
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

// USER CODE BEGIN (RTC,1)

// USER CODE END

_interrupt(RTCINT)  void RTC_viRTC(void)
{

  // USER CODE BEGIN (RTC,2)

  // USER CODE END

  if(RTC_ISNC_CNT0IR)            // if counter CNT0 overflow
  {

    // USER CODE BEGIN (RTC,4)

    // USER CODE END

    RTC_ISNC_CNT0IR = 0;

  }


  // USER CODE BEGIN (RTC,8)

  // USER CODE END

} //  End of function RTC_viRTC


//****************************************************************************
// @Function      void RTC_vSetTime(ulong ulRegValue, ubyte ubMode) 
//
//----------------------------------------------------------------------------
// @Description   This function loads the RTC reload register (RTC_RELH and 
//                RTC_RELL) with the forwarded value. If the parameter ubMode 
//                is '1' the RTC timer (RTC_RTCH and RTC_RTCL) are reloaded 
//                too.
//
//----------------------------------------------------------------------------
// @Returnvalue   None
//
//----------------------------------------------------------------------------
// @Parameters    ulRegValue: 
//                RTCL and RTCH load value
// @Parameters    ubMode: 
//                1 if RTCH and RTCL should be reloaded, 0 else
//
//----------------------------------------------------------------------------
// @Date          07.03.2016
//
//****************************************************************************

// USER CODE BEGIN (SetTime,1)

// USER CODE END

void RTC_vSetTime(ulong ulRegValue, ubyte ubMode)
{
  ulong tmp;

  tmp  = ulRegValue % 65536;
  RTC_RELH = (ulRegValue >> 16);
  RTC_RELL = tmp;

  if(ubMode)
  {
    RTC_RTCH = RTC_RELH;
    RTC_RTCL = RTC_RELL;
  }
} //  End of function RTC_vSetTime




// USER CODE BEGIN (RTC_General,10)

// USER CODE END

