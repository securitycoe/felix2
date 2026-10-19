//****************************************************************************
// @Module        Capture / Compare Unit 63 (CCU63)
// @Filename      CCU63.C
// @Project       30_5015_Felix.dav
//----------------------------------------------------------------------------
// @Controller    Infineon XE167F-96F66
//
// @Compiler      Tasking Classic
//
// @Codegenerator 2.2
//
// @Description   This file contains functions that use the CCU63 module.
//
//----------------------------------------------------------------------------
// @Date          07.03.2016 10:02:22
//
//****************************************************************************

// USER CODE BEGIN (CCU63_General,1)

// USER CODE END

/*
******************************************************************************
@Note

T13  wird zur PWM-Steuerung der Leuchtdioden für die Designelemente verwendet.
Periodendauer ca. 1ms, Auflösung ca. 3,8µs


******************************************************************************
*/


//****************************************************************************
// @Project Includes
//****************************************************************************

#include "MAIN.H"

// USER CODE BEGIN (CCU63_General,2)

// USER CODE END


//****************************************************************************
// @Macros
//****************************************************************************

// USER CODE BEGIN (CCU63_General,3)

// USER CODE END


//****************************************************************************
// @Defines
//****************************************************************************

// USER CODE BEGIN (CCU63_General,4)

// USER CODE END


//****************************************************************************
// @Typedefs
//****************************************************************************

// USER CODE BEGIN (CCU63_General,5)

// USER CODE END


//****************************************************************************
// @Imported Global Variables
//****************************************************************************

// USER CODE BEGIN (CCU63_General,6)

// USER CODE END


//****************************************************************************
// @Global Variables
//****************************************************************************

// USER CODE BEGIN (CCU63_General,7)

// USER CODE END


//****************************************************************************
// @External Prototypes
//****************************************************************************

// USER CODE BEGIN (CCU63_General,8)

// USER CODE END


//****************************************************************************
// @Prototypes Of Local Functions
//****************************************************************************

// USER CODE BEGIN (CCU63_General,9)

// USER CODE END


//****************************************************************************
// @Function      void CCU63_vInit(void) 
//
//----------------------------------------------------------------------------
// @Description   This is the initialization function of the CCU63 function 
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

void CCU63_vInit(void)
{
  // USER CODE BEGIN (Init,2)

  // USER CODE END

  ///  -----------------------------------------------------------------------
  ///  Configuration of KERNEL REGISTERS :
  ///  -----------------------------------------------------------------------
  ///  - CCU63 Module is enabled.
  ///  - The CCU63 module clock = 66,000 MHz. 
  ///  - T12 is enabled.
  ///  - T13 is enabled.
  ///  - MCM is enabled.

  CCU63_KSCFG    =  0x0003;      // Kernel State Configuration Register


  _nop();    //No operation function as Delay
  _nop();    //No operation function as Delay
  _nop();    //No operation function as Delay
  _nop();    //No operation function as Delay

  ///  -----------------------------------------------------------------------
  ///  Configuration of CCU63 Timer 12:
  ///  -----------------------------------------------------------------------
  ///  - Timer 12 Input clock factor (T12CLK) is 0
  ///  - prescaler factor is 1
  ///  - Timer 12 run bit is set
  ///  - Single shot mode is disabled
  ///  - Timer 12 works in edge aligned mode
  ///  - Interrupt on period match is disabled
  ///  - Interrupt on one match is disabled
  ///  - No External run selection is selected.
  ///  - Timer mode is selected.
  ///  - 

  CCU63_T12PR    =  0x0A11;      // load CCU63 T12 period register

  ///  -----------------------------------------------------------------------
  ///  Configuration of T13HR Signal:
  ///  -----------------------------------------------------------------------
  ///  - Signal T13HRA is selected as Input

  ///  -----------------------------------------------------------------------
  ///  Configuration of CCU63 Timer 13:
  ///  -----------------------------------------------------------------------
  ///  - Timer 13 Input Clock factor (T13CLK) is 0
  ///  - prescaler factor is 1
  ///  - Timer 13 run bit is set
  ///  - No External run selection is selected.
  ///  - Timer mode is selected.
  ///  - 
  ///  - Single shot mode is disabled
  ///  - Interrupt on period match is disabled
  ///  - Interrupt on compare match is disabled

  CCU63_T13PR    =  0x0A11;      // load CCU63 T13 period register

  CCU63_TCTR0    =  0x0808;      // load CCU63 timer control register 0
  CCU63_TCTR2    =  0x0020;      // load CCU63 timer control register 2

  ///  -----------------------------------------------------------------------
  ///  Configuration of Multi Channel Mode:
  ///  -----------------------------------------------------------------------
  ///  - Multi channel mode is enabled

  ///  - switching selelction:
  ///  - Transfer on T13 period match (while counting up)

  ///  - Switching synchronization:
  ///  - Synchronisation on T13 period match



  ///  -----------------------------------------------------------------------
  ///  Configuration of CCU63 Channel 0:
  ///  -----------------------------------------------------------------------
  ///  - Compare mode 1 is selected
  ///  - T12 modulation for output CC60 is disabled
  ///  - T13 modulation for output CC60 is disabled
  ///  - T12 modulation for output COUT60 is enabled
  ///  - T13 modulation for output COUT60 is disabled
  ///  - The output COUT60 can deliver the PWM generated by T12 or T13
  ///  - The trap functionality of the pin COUT60 is disabled
  ///  - The compare output COUT60 drives passive level while CC60ST is '0'
  ///  - The passive level of the output COUT60 is '1'
  ///  - Dead time generation is disabled

  ///  - Generation interrupt on flag ICC60R is disabled
  ///  - Generation interrupt on flag ICC60F is disabled

  CCU63_CC60SR   =  0x0000;      // Load CCU63 capture/compare shadow 
                                 // register for channel 0

  ///  -----------------------------------------------------------------------
  ///  Configuration of CCU63 Channel 1:
  ///  -----------------------------------------------------------------------
  ///  - Compare mode 1 is selected
  ///  - T12 modulation for output CC61 is disabled
  ///  - T13 modulation for output CC61 is disabled
  ///  - T12 modulation for output COUT61 is enabled
  ///  - T13 modulation for output COUT61 is disabled
  ///  - The output COUT61 can deliver the PWM generated by T12 or T13
  ///  - The trap functionality of the pin COUT61 is disabled
  ///  - The compare output COUT61 drives passive level while CC61ST is '0'
  ///  - The passive level of the output COUT61 is '0'
  ///  - Dead time generation is disabled

  ///  - Generation interrupt on flag ICC61R is disabled
  ///  - Generation interrupt on flag ICC61F is disabled

  CCU63_CC61SR   =  0x0000;      // Load CCU63 capture/compare shadow 
                                 // register for channel 1

  ///  -----------------------------------------------------------------------
  ///  Configuration of CCU63 Channel 2:
  ///  -----------------------------------------------------------------------
  ///  - Compare mode 1 is selected
  ///  - T12 modulation for output CC62 is disabled
  ///  - T13 modulation for output CC62 is disabled
  ///  - T12 modulation for output COUT62 is enabled
  ///  - T13 modulation for output COUT62 is disabled
  ///  - The output COUT62 can deliver the PWM generated by T12 or T13
  ///  - The trap functionality of the pin COUT62 is disabled
  ///  - The compare output COUT62 drives passive level while CC62ST is '0'
  ///  - The passive level of the output COUT62 is '1'
  ///  - Dead time generation is disabled

  ///  - Generation interrupt on flag ICC62R is disabled
  ///  - Generation interrupt on flag ICC62F is disabled

  CCU63_CC62SR   =  0x0000;      // Load CCU63 capture/compare shadow 
                                 // register for channel 2

  ///  -----------------------------------------------------------------------
  ///  Configuration of CCU63 Channel 3:
  ///  -----------------------------------------------------------------------
  ///  - T13 modulation for output COUT63 is enabled
  ///  - The trap functionality of the pin COUT63 is disabled
  ///  - The compare output COUT63 drives passive level while CC63ST is '0'
  ///  - The passive level of the output COUT63 is '0'
  ///  - T13 output is not inverted


  CCU63_CC63SR   =  0x0000;      // load CCU63 capture/compare shadow 
                                 // register for channel 3


  CCU63_T12DTC   =  0x0001;      // load CCU63 dead time control register for 
                                 // timer T12

  CCU63_T12MSEL  =  0x0111;      // load CCU63 T12 campture/compare mode 
                                 // select register

  CCU63_PSLR     =  0x0022;      // load CCU63 passive state level register

  CCU63_MODCTR   =  0x80AA;      // load CCU63 modulation control register

  CCU63_MCMOUTS  =  0x002A;      // load CCU63 multi channel mode output 
                                 // control register

  CCU63_MCMCTR   =  0x0012;      // load CCU63 multi channel mode control 
                                 // register

  ///  -----------------------------------------------------------------------
  ///  Configuration of CCU63 trap control:
  ///  -----------------------------------------------------------------------
  ///  - Trap can only be generated by SW by setting the bit TRPF
  ///  - The trap state is left when a zero-match of T12 (while counting up) 
  ///    is detected (synchronization to T12)
  ///  - Bit TRPF is automatically cleared by HW (according to TRPPEN, TRPM0 
  ///    and TRPM1)
  ///  - Trap interrupt is disabled


  ///  -----------------------------------------------------------------------
  ///  Configuration of the used CCU63 Channel Port Pins:
  ///  -----------------------------------------------------------------------
  ///  - P9.3 is used for CCU63 output(COUT60)
  ///  - P9.4 is used for CCU63 output(COUT61)
  ///  - P9.5 is used for CCU63 output(COUT62)
  ///  - P9.6 is used for  CCU63 output(COUT63)

  P9_IOCR03 = 0x0090;    //set direction register
  P9_IOCR04 = 0x0090;    //set direction register
  P9_IOCR05 = 0x0090;    //set direction register
  P9_IOCR06 = 0x0090;    //set direction register

  CCU63_PISELL   =  0x3FC0;      // Load CCU63 Port Input Selection register

  ///  -----------------------------------------------------------------------
  ///  Configuration of the used CCU63 Channels Interrupts:
  ///  -----------------------------------------------------------------------

  ///  -----------------------------------------------------------------------
  ///  Configuration of T12, T13 ---- CCU63_TCTR4 Register:
  ///  -----------------------------------------------------------------------
  ///  - Enable shadow transfer of T12 and T13
  ///  - Timer 12 run bit is set
  ///  - Timer 13 run bit is set
  CCU63_TCTR4    =  0x4242;      // load CCU63 timer control register 4


  // USER CODE BEGIN (NodeI3,3)

  // USER CODE END

} //  End of function CCU63_vInit




// USER CODE BEGIN (CCU63_General,10)

// USER CODE END

