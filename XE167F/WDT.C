//****************************************************************************
// @Module        Watchdog Timer (WDT)
// @Filename      WDT.C
// @Project       30_5015_Felix.dav
//----------------------------------------------------------------------------
// @Controller    Infineon XE167F-96F66
//
// @Compiler      Tasking Classic
//
// @Codegenerator 2.2
//
// @Description   This file contains functions that use the WDT module.
//
//----------------------------------------------------------------------------
// @Date          07.03.2016 10:02:23
//
//****************************************************************************

// USER CODE BEGIN (WDT_General,1)

// USER CODE END



//****************************************************************************
// @Project Includes
//****************************************************************************

#include "MAIN.H"

// USER CODE BEGIN (WDT_General,2)

// USER CODE END


//****************************************************************************
// @Macros
//****************************************************************************

// USER CODE BEGIN (WDT_General,3)

// USER CODE END


//****************************************************************************
// @Defines
//****************************************************************************

// USER CODE BEGIN (WDT_General,4)

// USER CODE END


//****************************************************************************
// @Typedefs
//****************************************************************************

// USER CODE BEGIN (WDT_General,5)

// USER CODE END


//****************************************************************************
// @Imported Global Variables
//****************************************************************************

// USER CODE BEGIN (WDT_General,6)

// USER CODE END


//****************************************************************************
// @Global Variables
//****************************************************************************

// USER CODE BEGIN (WDT_General,7)

// USER CODE END


//****************************************************************************
// @External Prototypes
//****************************************************************************

// USER CODE BEGIN (WDT_General,8)

// USER CODE END



//****************************************************************************
// @Prototypes Of Local Functions
//****************************************************************************

// USER CODE BEGIN (WDT_General,9)

// USER CODE END


//****************************************************************************
// @Function      void WDT_vInit(void) 
//
//----------------------------------------------------------------------------
// @Description   This is the initialization function of the WDT function 
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

void WDT_vInit(void)
{
  // USER CODE BEGIN (Init,2)

  // USER CODE END

  ///  - DISWDT executable only until End Of Init
  ///  - the input frequency is Fin / 16384
  ///  - period in normal watchdog mode =  19,859 ms
  ///  - period in time-out mode =  992,970 usec

  CPUCON1       |=  0x0000;      // CPU Control Register 1
  SCU_WDTREL     =  0xFFB0;      // WDT Reload Register
  SCU_WDTCS     |=  0x0000;      // WDT Control and Status Register

  SCU_INTDIS     =  0x0100;      // Interrupt Disable Register
  SCU_INTNP1     =  0x0000;      // Interrupt Node Pointer 1 Register


  // USER CODE BEGIN (Init,3)

  // USER CODE END

} //  End of function WDT_vInit




// USER CODE BEGIN (WDT_General,10)

// USER CODE END

