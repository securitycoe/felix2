//****************************************************************************
// @Module        USIC2
// @Filename      USIC2.C
// @Project       30_5015_Felix.dav
//----------------------------------------------------------------------------
// @Controller    Infineon XE167F-96F66
//
// @Compiler      Tasking Classic
//
// @Codegenerator 2.2
//
// @Description   This file contains functions that use the USIC2 module.
//
//----------------------------------------------------------------------------
// @Date          07.03.2016 10:02:22
//
//****************************************************************************

// USER CODE BEGIN (USIC2_General,1)

// USER CODE END

//****************************************************************************
// @Project Includes
//****************************************************************************

#include "MAIN.H"

// USER CODE BEGIN (USIC2_General,2)

// USER CODE END


//****************************************************************************
// @Macros
//****************************************************************************

// USER CODE BEGIN (USIC2_General,3)

// USER CODE END


//****************************************************************************
// @Defines
//****************************************************************************

// USER CODE BEGIN (USIC2_General,4)

// USER CODE END


//****************************************************************************
// @Typedefs
//****************************************************************************

// USER CODE BEGIN (USIC2_General,5)

// USER CODE END


//****************************************************************************
// @Imported Global Variables
//****************************************************************************

// USER CODE BEGIN (USIC2_General,6)

// USER CODE END


//****************************************************************************
// @Global Variables
//****************************************************************************

// USER CODE BEGIN (USIC2_General,7)

// USER CODE END


//****************************************************************************
// @External Prototypes
//****************************************************************************

// USER CODE BEGIN (USIC2_General,8)

// USER CODE END


//****************************************************************************
// @Prototypes Of Local Functions
//****************************************************************************

// USER CODE BEGIN (USIC2_General,9)

// USER CODE END


//****************************************************************************
// @Function      void USIC2_vInit(void) 
//
//----------------------------------------------------------------------------
// @Description   This function initializes USIC2 Module
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

void USIC2_vInit(void)
{
  // USER CODE BEGIN (Init,2)

  // USER CODE END

  //   -----------------------------------------------------------------------
  //   Initialization of USIC Peripherals:
  //   -----------------------------------------------------------------------

  ///  - USIC2 module clock = 66,00 MHz

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U2C0 Kernel State Configuration:
  ///  -----------------------------------------------------------------------

  U2C0_KSCFG     =  0x0003;      // load U2C0 kernel state configuration 
                                 // register

  _nop();  // one cycle delay 

  _nop();  // one cycle delay 

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U2C0 Channel Control Register (Mode & Interrupts):
  ///  -----------------------------------------------------------------------
  /// - The USIC channel is disabled
  /// - The parity generation is disabled

  U2C0_CCR       =  0x0000;      // load U0C0 channel control register

  //   initializes the Universal Serial Interface Channel (USIC) U2C0

  U2C0_ASC_vInit();

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U2C0 Channel Control Register (Mode & Interrupts):
  ///  -----------------------------------------------------------------------
  ///  - ASC (SCI, UART) Protocol is selected 
  ///  - Odd parity is selected
  ///  - Receive interrupt is enabled
  ///  - Alternate receive interrupt is enabled

  U2C0_CCR       =  0xC302;      // load U2C0 channel control register

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U2C1 Kernel State Configuration:
  ///  -----------------------------------------------------------------------

  U2C1_KSCFG     =  0x0003;      // load U2C1 kernel state configuration 
                                 // register

  _nop();  // one cycle delay 

  _nop();  // one cycle delay 

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U2C1 Channel Control Register (Mode & Interrupts):
  ///  -----------------------------------------------------------------------
  /// - The USIC channel is disabled
  /// - The parity generation is disabled

  U2C1_CCR       =  0x0000;      // load U0C0 channel control register

  //   initializes the Universal Serial Interface Channel (USIC) U2C1

  U2C1_ASC_vInit();

  ///  -----------------------------------------------------------------------
  ///  Configuration of the U2C1 Channel Control Register (Mode & Interrupts):
  ///  -----------------------------------------------------------------------
  ///  - ASC (SCI, UART) Protocol is selected 
  ///  - The parity generation is disabled
  ///  - Receive interrupt is enabled
  ///  - Alternate receive interrupt is enabled

  U2C1_CCR       =  0xC002;      // load U2C1 channel control register


  // USER CODE BEGIN (Init,3)

  // USER CODE END

} //  End of function USIC2_vInit




// USER CODE BEGIN (USIC2_General,10)

// USER CODE END

