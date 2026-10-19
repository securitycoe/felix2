//****************************************************************************
// @Module        Parallel Ports
// @Filename      IO.C
// @Project       30_5015_Felix.dav
//----------------------------------------------------------------------------
// @Controller    Infineon XE167F-96F66
//
// @Compiler      Tasking Classic
//
// @Codegenerator 2.2
//
// @Description   This file contains functions that use the IO module.
//
//----------------------------------------------------------------------------
// @Date          07.03.2016 10:02:15
//
//****************************************************************************

// USER CODE BEGIN (IO_General,1)

// USER CODE END

/*
******************************************************************************
@Note

Notes for Port P0:
P0.6 VCC_OEM


******************************************************************************
*/

/*
******************************************************************************
@Note

Notes for Port P1:
P1.0 SET_FF
P1.1 VCC_UVL

P1.4 SW_UVL
P1.7 SET_UVL_IN


******************************************************************************
*/

/*
******************************************************************************
@Note

Notes for Port P2:
P2.6 OEM_IN
P2.10 UVL_IN


******************************************************************************
*/

/*
******************************************************************************
@Note

Notes for Port P8:
P8.6 MIn


******************************************************************************
*/

/*
******************************************************************************
@Note

Notes for Port P10:
P10.3 MCtrl
P10.4 PHOTO_MOS


******************************************************************************
*/

/*
******************************************************************************
@Note

Notes for Port P11:
Parameter zu spezifizieren ist nicht sinnvoll, da automatisch noch der Prefix IN hinzugefügt wird

******************************************************************************
*/



//****************************************************************************
// @Project Includes
//****************************************************************************

#include "MAIN.H"

// USER CODE BEGIN (IO_General,2)

// USER CODE END


//****************************************************************************
// @Macros
//****************************************************************************

// USER CODE BEGIN (IO_General,3)

// USER CODE END


//****************************************************************************
// @Defines
//****************************************************************************

// USER CODE BEGIN (IO_General,4)

// USER CODE END


//****************************************************************************
// @Typedefs
//****************************************************************************

// USER CODE BEGIN (IO_General,5)

// USER CODE END


//****************************************************************************
// @Imported Global Variables
//****************************************************************************

// USER CODE BEGIN (IO_General,6)

// USER CODE END


//****************************************************************************
// @Global Variables
//****************************************************************************

// USER CODE BEGIN (IO_General,7)

// USER CODE END


//****************************************************************************
// @External Prototypes
//****************************************************************************

// USER CODE BEGIN (IO_General,8)

// USER CODE END


//****************************************************************************
// @Prototypes Of Local Functions
//****************************************************************************

// USER CODE BEGIN (IO_General,9)

// USER CODE END


//****************************************************************************
// @Function      void IO_vInit(void) 
//
//----------------------------------------------------------------------------
// @Description   This is the initialization function of the IO function 
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

// USER CODE BEGIN (IO_Function,1)

// USER CODE END

void IO_vInit(void)
{
  // USER CODE BEGIN (IO_Function,2)

  // USER CODE END


  ///  -----------------------------------------------------------------------
  ///  Configuration of Port P0:
  ///  -----------------------------------------------------------------------
  ///  P0.0 is used as general purpose output
  ///  - push/pull output is selected
  ///  - the pin status is high level
  ///  P0.1 is used as general input
  ///  - pull device is tristate 
  ///  P0.2 is used as general purpose output
  ///  - push/pull output is selected
  ///  - the pin status is high level
  ///  P0.3 is used as general purpose output
  ///  - push/pull output is selected
  ///  - the pin status is low level
  ///  P0.6 is used as general purpose output
  ///  - push/pull output is selected
  ///  - the pin status is low level
  ///  P0.7 is used as general purpose output
  ///  - push/pull output is selected
  ///  - the pin status is low level

  ///  P0.0 - P0.3 output driver characteristic: strong driver
  ///  P0.4 - P0.7 output driver characteristic: strong driver

  ///  P0.0 - P0.3 output edge characteristic: sharp edge mode
  ///  P0.4 - P0.7 output edge characteristic: sharp edge mode

  P0_OUT         =  0x0005;      // load data output register
  P0_IOCR00      =  0x0080;      // load port control register 0
  P0_IOCR02      =  0x0080;      // load port control register 2
  P0_IOCR03      =  0x0080;      // load port control register 3
  P0_IOCR06      =  0x0080;      // load port control register 6
  P0_IOCR07      =  0x0080;      // load port control register 7

  ///  -----------------------------------------------------------------------
  ///  Configuration of Port P1:
  ///  -----------------------------------------------------------------------
  ///  P1.0 is used as general purpose output
  ///  - push/pull output is selected
  ///  - the pin status is low level
  ///  P1.1 is used as general purpose output
  ///  - push/pull output is selected
  ///  - the pin status is low level
  ///  P1.2 is used as general purpose output
  ///  - push/pull output is selected
  ///  - the pin status is high level
  ///  P1.3 is used as general purpose output
  ///  - push/pull output is selected
  ///  - the pin status is high level
  ///  P1.4 is used as general purpose output
  ///  - push/pull output is selected
  ///  - the pin status is low level
  ///  P1.5 is used as alternate input for the USIC2 Channel0 Shift Data 
  ///  input(DXoC)
  ///  P1.6 is used as alternate output for the USIC2 Channel0 Shift Data 
  ///  output(DOUT)
  ///  - push/pull output is selected
  ///  P1.7 is used as general purpose output
  ///  - push/pull output is selected
  ///  - the pin status is low level

  ///  P1.0 - P1.3 output driver characteristic: strong driver
  ///  P1.4 - P1.7 output driver characteristic: strong driver

  ///  P1.0 - P1.3 output edge characteristic: sharp edge mode
  ///  P1.4 - P1.7 output edge characteristic: sharp edge mode

  P1_OUT         =  0x000C;      // load data output register
  P1_IOCR00      =  0x0080;      // load port control register 0
  P1_IOCR01      =  0x0080;      // load port control register 1
  P1_IOCR02      =  0x0080;      // load port control register 2
  P1_IOCR03      =  0x0080;      // load port control register 3
  P1_IOCR04      =  0x0080;      // load port control register 4
  P1_IOCR07      =  0x0080;      // load port control register 7

  ///  -----------------------------------------------------------------------
  ///  Configuration of Port P2:
  ///  -----------------------------------------------------------------------
  ///  P2.0 is used as general purpose output
  ///  - open drain output is selected
  ///  - the pin status is high level
  ///  P2.1 is used as alternate input for the SCU_EX0AINA input
  ///  P2.2 is used as alternate input for the SCU_EX1AINA input
  ///  P2.3 is used as alternate input for the CAPCOM2 input(CC2_16)
  ///  P2.4 is used as alternate input for the CAPCOM2 input(CC2_17)
  ///  P2.5 is used as alternate input for the CAPCOM2 input(CC2_18)
  ///  P2.6 is used as general input
  ///  - pull device is tristate 
  ///  P2.10 is used as general input
  ///  - pull device is tristate 

  ///  P2.0 - P2.3 output driver characteristic: strong driver
  ///  P2.4 - P2.7 output driver characteristic: strong driver
  ///  P2.8 - P2.12 output driver characteristic: strong driver

  ///  P2.0 - P2.3 output edge characteristic: sharp edge mode
  ///  P2.4 - P2.7 output edge characteristic: sharp edge mode
  ///  P2.8 - P2.12 output edge characteristic: sharp edge mode

  P2_OUT         =  0x0001;      // load data output register
  P2_IOCR00      =  0x00C0;      // load port control register 0

  ///  -----------------------------------------------------------------------
  ///  Configuration of Port P3:
  ///  -----------------------------------------------------------------------
  ///  P3.0 is used as general input
  ///  - pull device is tristate 
  ///  P3.1 is used as general input
  ///  - pull device is tristate 
  ///  P3.2 is used as general purpose output
  ///  - open drain output is selected
  ///  - the pin status is low level
  ///  P3.3 is used as general purpose output
  ///  - push/pull output is selected
  ///  - the pin status is low level
  ///  P3.4 is used as general purpose output
  ///  - push/pull output is selected
  ///  - the pin status is low level
  ///  P3.5 is used as general input
  ///  - pull device is tristate 
  ///  P3.6 is used as alternate output for the USIC2Channel1 Shift Data 
  ///  output(DOUT)
  ///  - push/pull output is selected
  ///  P3.7 is used as alternate input for the USIC2Channel1 Shift Data 
  ///  input(DX0B)

  ///  P3.0 - P3.3 output driver characteristic: strong driver
  ///  P3.4 - P3.7 output driver characteristic: strong driver

  ///  P3.0 - P3.3 output edge characteristic: sharp edge mode
  ///  P3.4 - P3.7 output edge characteristic: sharp edge mode

  P3_IOCR02      =  0x00C0;      // load port control register 2
  P3_IOCR03      =  0x0080;      // load port control register 3
  P3_IOCR04      =  0x0080;      // load port control register 4

  ///  -----------------------------------------------------------------------
  ///  Configuration of Port P4:
  ///  -----------------------------------------------------------------------
  ///  - no pin of port P4 is used


  ///  -----------------------------------------------------------------------
  ///  Configuration of Port P5:
  ///  -----------------------------------------------------------------------
  ///  P5.14 is used as general input
  ///  P5.15 is used as general input


  ///  -----------------------------------------------------------------------
  ///  Configuration of Port P6:
  ///  -----------------------------------------------------------------------
  ///  P6.0 is used as alternate input for the USIC1 Channel1 shift Data 
  ///  input(DX0E)
  ///  P6.1 is used as alternate output for the USIC1 Channel1 shift Data 
  ///  output(DOUT)
  ///  - push/pull output is selected
  ///  P6.2 is used as general input
  ///  - pull-up device is assigned

  ///  P6.0 - P6.3 output driver characteristic: strong driver

  ///  P6.0 - P6.3 output edge characteristic: sharp edge mode

  P6_IOCR02      =  0x0020;      // load port control register 2

  ///  -----------------------------------------------------------------------
  ///  Configuration of Port P7:
  ///  -----------------------------------------------------------------------
  ///  P7.3 is used as alternate output for the USIC0Channel0 Shift Data 
  ///  output(DOUT)
  ///  - push/pull output is selected
  ///  P7.4 is used as alternate input for the USIC0Channel0 Shift Data 
  ///  input(DX0D)

  ///  P7.0 - P7.3 output driver characteristic: strong driver
  ///  P7.4 - P7.4 output driver characteristic: strong driver

  ///  P7.0 - P7.3 output edge characteristic: sharp edge mode
  ///  P7.4 - P7.4 output edge characteristic: sharp edge mode


  ///  -----------------------------------------------------------------------
  ///  Configuration of Port P8:
  ///  -----------------------------------------------------------------------
  ///  P8.0 is used as general input
  ///  - pull device is tristate 
  ///  P8.1 is used as general input
  ///  - pull device is tristate 
  ///  P8.2 is used as general input
  ///  - pull device is tristate 
  ///  P8.3 is used as general input
  ///  - pull device is tristate 
  ///  P8.4 is used as general purpose output
  ///  - push/pull output is selected
  ///  - the pin status is low level
  ///  P8.5 is used as general purpose output
  ///  - push/pull output is selected
  ///  - the pin status is low level
  ///  P8.6 is used as general input
  ///  - pull device is tristate 

  ///  P8.0 - P8.3 output driver characteristic: strong driver
  ///  P8.4 - P8.6 output driver characteristic: strong driver

  ///  P8.0 - P8.3 output edge characteristic: sharp edge mode
  ///  P8.4 - P8.6 output edge characteristic: sharp edge mode

  P8_IOCR04      =  0x0080;      // load port control register 4
  P8_IOCR05      =  0x0080;      // load port control register 5

  ///  -----------------------------------------------------------------------
  ///  Configuration of Port P9:
  ///  -----------------------------------------------------------------------
  ///  P9.1 is used as general purpose output
  ///  - push/pull output is selected
  ///  - the pin status is low level
  ///  P9.2 is used as general purpose output
  ///  - push/pull output is selected
  ///  - the pin status is low level
  ///  P9.3 is used as alternate output for the CCU63 output(COUT60)
  ///  - push/pull output is selected
  ///  P9.4 is used as alternate output for the CCU63 output(COUT61)
  ///  - push/pull output is selected
  ///  P9.5 is used as alternate output for the CCU63 output(COUT62)
  ///  - push/pull output is selected
  ///  P9.6 is used as alternate output for the  CCU63 output(COUT63)
  ///  - push/pull output is selected
  ///  P9.7 is used as general purpose output
  ///  - push/pull output is selected
  ///  - the pin status is low level

  ///  P9.0 - P9.3 output driver characteristic: strong driver
  ///  P9.4 - P9.7 output driver characteristic: strong driver

  ///  P9.0 - P9.3 output edge characteristic: sharp edge mode
  ///  P9.4 - P9.7 output edge characteristic: sharp edge mode

  P9_IOCR01      =  0x0080;      // load port control register 1
  P9_IOCR02      =  0x0080;      // load port control register 2
  P9_IOCR07      =  0x0080;      // load port control register 7

  ///  -----------------------------------------------------------------------
  ///  Configuration of Port P10:
  ///  -----------------------------------------------------------------------
  ///  P10.3 is used as general purpose output
  ///  - push/pull output is selected
  ///  - the pin status is high level
  ///  P10.4 is used as general purpose output
  ///  - push/pull output is selected
  ///  - the pin status is high level
  ///  P10.12 is used as alternate output for the USIC1Channel0 Shift Data 
  ///  output(DOUT)
  ///  - push/pull output is selected
  ///  P10.13 is used as alternate input for the USIC1Channel0 Shift Data 
  ///  input(DX0D)
  ///  P10.14 is used as alternate input for the USIC0Channel1 Shift Data 
  ///  input(DX0C)
  ///  P10.15 is used as alternate output for the USIC0Channel1 Shift Data 
  ///  output(DOUT)
  ///  - push/pull output is selected

  ///  P10.0 - P10.3 output driver characteristic: strong driver
  ///  P10.4 - P10.7 output driver characteristic: strong driver
  ///  P10.8 - P10.11 output driver characteristic: strong driver
  ///  P10.12 - P10.15 output driver characteristic: strong driver

  ///  P10.0 - P10.3 output edge characteristic: sharp edge mode
  ///  P10.4 - P10.7 output edge characteristic: sharp edge mode
  ///  P10.8 - P10.11 output edge characteristic: sharp edge mode
  ///  P10.12 - P10.15 output edge characteristic: sharp edge mode

  P10_OUT        =  0x0018;      // load data output register
  P10_IOCR03     =  0x0080;      // load port control register 3
  P10_IOCR04     =  0x0080;      // load port control register 4

  ///  -----------------------------------------------------------------------
  ///  Configuration of Port P11:
  ///  -----------------------------------------------------------------------
  ///  - no pin of port P11 is used


  ///  -----------------------------------------------------------------------
  ///  Configuration of Port P15:
  ///  -----------------------------------------------------------------------
  ///  - no pin of port P15 is used
  ///  - Port15 Data register P15(Read only)



  // USER CODE BEGIN (IO_Function,3)

  // USER CODE END

} //  End of function IO_vInit




// USER CODE BEGIN (IO_General,10)
void IO_vInit_noNRTL(void)
{

  ///  -----------------------------------------------------------------------
  ///  Configuration of Port P0:
  ///  -----------------------------------------------------------------------
  ///  P0.0 is used as general purpose output
  ///  - push/pull output is selected
  ///  - the pin status is high level
  ///  P0.1 is used as general input
  ///  - pull device is tristate 
  ///  P0.2 is used as general purpose output
  ///  - push/pull output is selected
  ///  - the pin status is high level
  ///  P0.3 is used as general purpose output
  ///  - push/pull output is selected
  ///  - the pin status is low level
  ///  P0.6 is used as general purpose output
  ///  - push/pull output is selected
  ///  - the pin status is low level
  ///  P0.7 is used as general purpose output
  ///  - push/pull output is selected
  ///  - the pin status is low level

  ///  P0.0 - P0.3 output driver characteristic: strong driver
  ///  P0.4 - P0.7 output driver characteristic: strong driver

  ///  P0.0 - P0.3 output edge characteristic: sharp edge mode
  ///  P0.4 - P0.7 output edge characteristic: sharp edge mode

  P0_OUT         =  0x0005;      // load data output register
  P0_IOCR00      =  0x0080;      // load port control register 0
  P0_IOCR02      =  0x0080;      // load port control register 2
  P0_IOCR03      =  0x0080;      // load port control register 3
  P0_IOCR06      =  0x0080;      // load port control register 6
  P0_IOCR07      =  0x0080;      // load port control register 7

  ///  -----------------------------------------------------------------------
  ///  Configuration of Port P1:
  ///  -----------------------------------------------------------------------
  ///  P1.0 is used as general purpose output
  ///  - push/pull output is selected
  ///  - the pin status is low level
  ///  P1.1 is used as general purpose output
  ///  - push/pull output is selected
  ///  - the pin status is low level
  ///  P1.2 is used as general purpose output
  ///  - push/pull output is selected
  ///  - the pin status is high level
  ///  P1.3 is used as general purpose output
  ///  - push/pull output is selected
  ///  - the pin status is high level
  ///  P1.4 is used as general purpose output
  ///  - push/pull output is selected
  ///  - the pin status is low level
  ///  P1.5 is used as alternate input for the USIC2 Channel0 Shift Data 
  ///  input(DXoC)
  ///  P1.6 is used as alternate output for the USIC2 Channel0 Shift Data 
  ///  output(DOUT)
  ///  - push/pull output is selected
  ///  P1.7 is used as general purpose output
  ///  - push/pull output is selected
  ///  - the pin status is low level

  ///  P1.0 - P1.3 output driver characteristic: strong driver
  ///  P1.4 - P1.7 output driver characteristic: strong driver

  ///  P1.0 - P1.3 output edge characteristic: sharp edge mode
  ///  P1.4 - P1.7 output edge characteristic: sharp edge mode

  P1_OUT         =  0x000C;      // load data output register
  P1_IOCR00      =  0x0080;      // load port control register 0
  P1_IOCR01      =  0x0080;      // load port control register 1
  P1_IOCR02      =  0x0080;      // load port control register 2
  P1_IOCR03      =  0x0080;      // load port control register 3
  P1_IOCR04      =  0x0080;      // load port control register 4
  P1_IOCR07      =  0x0080;      // load port control register 7

  ///  -----------------------------------------------------------------------
  ///  Configuration of Port P2:
  ///  -----------------------------------------------------------------------
  ///  P2.0 is used as general purpose output
  ///  - open drain output is selected
  ///  - the pin status is high level
  ///  P2.1 is used as alternate input for the SCU_EX0AINA input
  ///  P2.2 is used as alternate input for the SCU_EX1AINA input
  ///  P2.3 is used as alternate input for the CAPCOM2 input(CC2_16)
  ///  P2.4 is used as alternate input for the CAPCOM2 input(CC2_17)
  ///  P2.5 is used as alternate input for the CAPCOM2 input(CC2_18)
  ///  P2.6 is used as general input
  ///  - pull device is tristate 
  ///  P2.10 is used as general input
  ///  - pull device is tristate 

  ///  P2.0 - P2.3 output driver characteristic: strong driver
  ///  P2.4 - P2.7 output driver characteristic: strong driver
  ///  P2.8 - P2.12 output driver characteristic: strong driver

  ///  P2.0 - P2.3 output edge characteristic: sharp edge mode
  ///  P2.4 - P2.7 output edge characteristic: sharp edge mode
  ///  P2.8 - P2.12 output edge characteristic: sharp edge mode

  P2_OUT         =  0x0001;      // load data output register
  P2_IOCR00      =  0x00C0;      // load port control register 0

  ///  -----------------------------------------------------------------------
  ///  Configuration of Port P3:
  ///  -----------------------------------------------------------------------
  ///  P3.0 is used as general input
  ///  - pull device is tristate 
  ///  P3.1 is used as general input
  ///  - pull device is tristate 
  ///  P3.2 is used as general purpose output
  ///  - open drain output is selected
  ///  - the pin status is low level
  ///  P3.3 is used as general purpose output
  ///  - push/pull output is selected
  ///  - the pin status is low level
  ///  P3.4 is used as general purpose output
  ///  - push/pull output is selected
  ///  - the pin status is low level
  ///  P3.5 is used as general input
  ///  - pull device is tristate 
  ///  P3.6 is used as alternate output for the USIC2Channel1 Shift Data 
  ///  output(DOUT)
  ///  - push/pull output is selected
  ///  P3.7 is used as alternate input for the USIC2Channel1 Shift Data 
  ///  input(DX0B)

  ///  P3.0 - P3.3 output driver characteristic: strong driver
  ///  P3.4 - P3.7 output driver characteristic: strong driver

  ///  P3.0 - P3.3 output edge characteristic: sharp edge mode
  ///  P3.4 - P3.7 output edge characteristic: sharp edge mode

  P3_IOCR02      =  0x00C0;      // load port control register 2
  P3_IOCR03      =  0x0080;      // load port control register 3
  P3_IOCR04      =  0x0080;      // load port control register 4

  ///  -----------------------------------------------------------------------
  ///  Configuration of Port P4:
  ///  -----------------------------------------------------------------------
  ///  - no pin of port P4 is used


  ///  -----------------------------------------------------------------------
  ///  Configuration of Port P5:
  ///  -----------------------------------------------------------------------
  ///  P5.14 is used as general input
  ///  P5.15 is used as general input


  ///  -----------------------------------------------------------------------
  ///  Configuration of Port P6:
  ///  -----------------------------------------------------------------------
  ///  P6.0 is used as alternate input for the USIC1 Channel1 shift Data 
  ///  input(DX0E)
  ///  P6.1 is used as alternate output for the USIC1 Channel1 shift Data 
  ///  output(DOUT)
  ///  - push/pull output is selected
  ///  P6.2 is used as general input
  ///  - pull-up device is assigned

  ///  P6.0 - P6.3 output driver characteristic: strong driver

  ///  P6.0 - P6.3 output edge characteristic: sharp edge mode

  P6_IOCR02      =  0x0020;      // load port control register 2

  ///  -----------------------------------------------------------------------
  ///  Configuration of Port P7:
  ///  -----------------------------------------------------------------------
  ///  P7.3 is used as alternate output for the USIC0Channel0 Shift Data 
  ///  output(DOUT)
  ///  - push/pull output is selected
  ///  P7.4 is used as alternate input for the USIC0Channel0 Shift Data 
  ///  input(DX0D)

  ///  P7.0 - P7.3 output driver characteristic: strong driver
  ///  P7.4 - P7.4 output driver characteristic: strong driver

  ///  P7.0 - P7.3 output edge characteristic: sharp edge mode
  ///  P7.4 - P7.4 output edge characteristic: sharp edge mode


  ///  -----------------------------------------------------------------------
  ///  Configuration of Port P8:
  ///  -----------------------------------------------------------------------
  ///  P8.0 is used as general input
  ///  - pull device is tristate 
  ///  P8.1 is used as general input
  ///  - pull device is tristate 
  ///  P8.2 is used as general input
  ///  - pull device is tristate 
  ///  P8.3 is used as general input
  ///  - pull device is tristate 
  ///  P8.4 is used as general purpose output
  ///  - push/pull output is selected
  ///  - the pin status is low level
  ///  P8.5 is used as general purpose output
  ///  - push/pull output is selected
  ///  - the pin status is low level
  ///  P8.6 is used as general input
  ///  - pull device is tristate 

  ///  P8.0 - P8.3 output driver characteristic: strong driver
  ///  P8.4 - P8.6 output driver characteristic: strong driver

  ///  P8.0 - P8.3 output edge characteristic: sharp edge mode
  ///  P8.4 - P8.6 output edge characteristic: sharp edge mode

  P8_IOCR04      =  0x0080;      // load port control register 4
  P8_IOCR05      =  0x0080;      // load port control register 5

  ///  -----------------------------------------------------------------------
  ///  Configuration of Port P9:
  ///  -----------------------------------------------------------------------
  ///  P9.1 is used as general purpose output
  ///  - push/pull output is selected
  ///  - the pin status is low level
  ///  P9.2 is used as general purpose output
  ///  - push/pull output is selected
  ///  - the pin status is low level
  ///  P9.3 is used as alternate output for the CCU63 output(COUT60)
  ///  - push/pull output is selected
  ///  P9.4 is used as alternate output for the CCU63 output(COUT61)
  ///  - push/pull output is selected
  ///  P9.5 is used as alternate output for the CCU63 output(COUT62)
  ///  - push/pull output is selected
  ///  P9.6 is used as alternate output for the  CCU63 output(COUT63)
  ///  - push/pull output is selected
  ///  P9.7 is used as general purpose output
  ///  - push/pull output is selected
  ///  - the pin status is low level

  ///  P9.0 - P9.3 output driver characteristic: strong driver
  ///  P9.4 - P9.7 output driver characteristic: strong driver

  ///  P9.0 - P9.3 output edge characteristic: sharp edge mode
  ///  P9.4 - P9.7 output edge characteristic: sharp edge mode

  P9_IOCR01      =  0x0080;      // load port control register 1
  P9_IOCR02      =  0x0080;      // load port control register 2
  P9_IOCR07      =  0x0080;      // load port control register 7

  ///  -----------------------------------------------------------------------
  ///  Configuration of Port P10:
  ///  -----------------------------------------------------------------------
  ///  P10.3 is used as general purpose output
  ///  - push/pull output is selected
  ///  - the pin status is high level
  ///  P10.4 is used as general purpose output
  ///  - push/pull output is selected
  ///  - the pin status is high level
  ///  P10.8 is used as general purpose output
  ///  - open drain output is selected
  ///  - the pin status is high level
  ///  P10.9 is used as general purpose output
  ///  - open drain output is selected
  ///  - the pin status is low level
  ///  P10.10 is used as general purpose output
  ///  - push/pull output is selected
  ///  - the pin status is high level
  ///  P10.11 is used as general purpose output
  ///  - push/pull output is selected
  ///  - the pin status is low level
  ///  P10.12 is used as alternate output for the USIC1Channel0 Shift Data 
  ///  output(DOUT)
  ///  - push/pull output is selected
  ///  P10.13 is used as alternate input for the USIC1Channel0 Shift Data 
  ///  input(DX0D)
  ///  P10.14 is used as alternate input for the USIC0Channel1 Shift Data 
  ///  input(DX0C)
  ///  P10.15 is used as alternate output for the USIC0Channel1 Shift Data 
  ///  output(DOUT)
  ///  - push/pull output is selected

  ///  P10.0 - P10.3 output driver characteristic: strong driver
  ///  P10.4 - P10.7 output driver characteristic: strong driver
  ///  P10.8 - P10.11 output driver characteristic: strong driver
  ///  P10.12 - P10.15 output driver characteristic: strong driver

  ///  P10.0 - P10.3 output edge characteristic: sharp edge mode
  ///  P10.4 - P10.7 output edge characteristic: sharp edge mode
  ///  P10.8 - P10.11 output edge characteristic: sharp edge mode
  ///  P10.12 - P10.15 output edge characteristic: sharp edge mode

  P10_OUT        =  0x0518;      // load data output register
  P10_IOCR03     =  0x0080;      // load port control register 3
  P10_IOCR04     =  0x0080;      // load port control register 4
  P10_IOCR08     =  0x00C0;      // load port control register 8
  P10_IOCR09     =  0x00C0;      // load port control register 9
  P10_IOCR10     =  0x0080;      // load port control register 10
  P10_IOCR11     =  0x0080;      // load port control register 11

  ///  -----------------------------------------------------------------------
  ///  Configuration of Port P11:
  ///  -----------------------------------------------------------------------
  ///  - no pin of port P11 is used


  ///  -----------------------------------------------------------------------
  ///  Configuration of Port P15:
  ///  -----------------------------------------------------------------------
  ///  - no pin of port P15 is used
  ///  - Port15 Data register P15(Read only)


} //  End of function IO_vInit_noNRTL

void IO_vInit_NRTL(void)
{

  ///  -----------------------------------------------------------------------
  ///  Configuration of Port P0:
  ///  -----------------------------------------------------------------------
  ///  P0.0 is used as general purpose output
  ///  - push/pull output is selected
  ///  - the pin status is high level
  ///  P0.1 is used as general input
  ///  - pull device is tristate 
  ///  P0.2 is used as general purpose output
  ///  - push/pull output is selected
  ///  - the pin status is high level
  ///  P0.3 is used as general purpose output
  ///  - push/pull output is selected
  ///  - the pin status is low level
  ///  P0.6 is used as general purpose output
  ///  - push/pull output is selected
  ///  - the pin status is low level
  ///  P0.7 is used as general purpose output
  ///  - push/pull output is selected
  ///  - the pin status is low level

  ///  P0.0 - P0.3 output driver characteristic: strong driver
  ///  P0.4 - P0.7 output driver characteristic: strong driver

  ///  P0.0 - P0.3 output edge characteristic: sharp edge mode
  ///  P0.4 - P0.7 output edge characteristic: sharp edge mode

  P0_OUT         =  0x0005;      // load data output register
  P0_IOCR00      =  0x0080;      // load port control register 0
  P0_IOCR02      =  0x0080;      // load port control register 2
  P0_IOCR03      =  0x0080;      // load port control register 3
  P0_IOCR06      =  0x0080;      // load port control register 6
  P0_IOCR07      =  0x0080;      // load port control register 7

  ///  -----------------------------------------------------------------------
  ///  Configuration of Port P1:
  ///  -----------------------------------------------------------------------
  ///  P1.0 is used as general purpose output
  ///  - push/pull output is selected
  ///  - the pin status is high level
  ///  P1.1 is used as general purpose output
  ///  - push/pull output is selected
  ///  - the pin status is low level
  ///  P1.2 is used as general purpose output
  ///  - push/pull output is selected
  ///  - the pin status is high level
  ///  P1.3 is used as general purpose output
  ///  - push/pull output is selected
  ///  - the pin status is high level
  ///  P1.4 is used as general purpose output
  ///  - push/pull output is selected
  ///  - the pin status is low level
  ///  P1.5 is used as alternate input for the USIC2 Channel0 Shift Data 
  ///  input(DXoC)
  ///  P1.6 is used as alternate output for the USIC2 Channel0 Shift Data 
  ///  output(DOUT)
  ///  - push/pull output is selected
  ///  P1.7 is used as general purpose output
  ///  - push/pull output is selected
  ///  - the pin status is low level

  ///  P1.0 - P1.3 output driver characteristic: strong driver
  ///  P1.4 - P1.7 output driver characteristic: strong driver

  ///  P1.0 - P1.3 output edge characteristic: sharp edge mode
  ///  P1.4 - P1.7 output edge characteristic: sharp edge mode

  P1_OUT         =  0x000D;      // load data output register
  P1_IOCR00      =  0x0080;      // load port control register 0
  P1_IOCR01      =  0x0080;      // load port control register 1
  P1_IOCR02      =  0x0080;      // load port control register 2
  P1_IOCR03      =  0x0080;      // load port control register 3
  P1_IOCR04      =  0x0080;      // load port control register 4
  P1_IOCR07      =  0x0080;      // load port control register 7

  ///  -----------------------------------------------------------------------
  ///  Configuration of Port P2:
  ///  -----------------------------------------------------------------------
  ///  P2.0 is used as general purpose output
  ///  - open drain output is selected
  ///  - the pin status is high level
  ///  P2.1 is used as alternate input for the SCU_EX0AINA input
  ///  P2.2 is used as alternate input for the SCU_EX1AINA input
  ///  P2.3 is used as alternate input for the CAPCOM2 input(CC2_16)
  ///  P2.4 is used as alternate input for the CAPCOM2 input(CC2_17)
  ///  P2.5 is used as alternate input for the CAPCOM2 input(CC2_18)
  ///  P2.6 is used as general input
  ///  - pull device is tristate 
  ///  P2.10 is used as general input
  ///  - pull device is tristate 
  ///  P2.11 is used as general purpose output
  ///  - push/pull output is selected
  ///  - the pin status is high level

  ///  P2.0 - P2.3 output driver characteristic: strong driver
  ///  P2.4 - P2.7 output driver characteristic: strong driver
  ///  P2.8 - P2.12 output driver characteristic: strong driver

  ///  P2.0 - P2.3 output edge characteristic: sharp edge mode
  ///  P2.4 - P2.7 output edge characteristic: sharp edge mode
  ///  P2.8 - P2.12 output edge characteristic: sharp edge mode

  P2_OUT         =  0x0801;      // load data output register
  P2_IOCR00      =  0x00C0;      // load port control register 0
  P2_IOCR11      =  0x0080;      // load port control register 11

  ///  -----------------------------------------------------------------------
  ///  Configuration of Port P3:
  ///  -----------------------------------------------------------------------
  ///  P3.0 is used as general input
  ///  - pull device is tristate 
  ///  P3.1 is used as general input
  ///  - pull device is tristate 
  ///  P3.2 is used as general purpose output
  ///  - open drain output is selected
  ///  - the pin status is low level
  ///  P3.3 is used as general purpose output
  ///  - push/pull output is selected
  ///  - the pin status is low level
  ///  P3.4 is used as general purpose output
  ///  - push/pull output is selected
  ///  - the pin status is low level
  ///  P3.5 is used as general input
  ///  - pull device is tristate 
  ///  P3.6 is used as alternate output for the USIC2Channel1 Shift Data 
  ///  output(DOUT)
  ///  - push/pull output is selected
  ///  P3.7 is used as alternate input for the USIC2Channel1 Shift Data 
  ///  input(DX0B)

  ///  P3.0 - P3.3 output driver characteristic: strong driver
  ///  P3.4 - P3.7 output driver characteristic: strong driver

  ///  P3.0 - P3.3 output edge characteristic: sharp edge mode
  ///  P3.4 - P3.7 output edge characteristic: sharp edge mode

  P3_IOCR02      =  0x00C0;      // load port control register 2
  P3_IOCR03      =  0x0080;      // load port control register 3
  P3_IOCR04      =  0x0080;      // load port control register 4

  ///  -----------------------------------------------------------------------
  ///  Configuration of Port P4:
  ///  -----------------------------------------------------------------------
  ///  - no pin of port P4 is used


  ///  -----------------------------------------------------------------------
  ///  Configuration of Port P5:
  ///  -----------------------------------------------------------------------
  ///  P5.14 is used as general input
  ///  P5.15 is used as general input


  ///  -----------------------------------------------------------------------
  ///  Configuration of Port P6:
  ///  -----------------------------------------------------------------------
  ///  P6.0 is used as alternate input for the USIC1 Channel1 shift Data 
  ///  input(DX0E)
  ///  P6.1 is used as alternate output for the USIC1 Channel1 shift Data 
  ///  output(DOUT)
  ///  - push/pull output is selected
  ///  P6.2 is used as general input
  ///  - pull-up device is assigned

  ///  P6.0 - P6.3 output driver characteristic: strong driver

  ///  P6.0 - P6.3 output edge characteristic: sharp edge mode

  P6_IOCR02      =  0x0020;      // load port control register 2

  ///  -----------------------------------------------------------------------
  ///  Configuration of Port P7:
  ///  -----------------------------------------------------------------------
  ///  P7.3 is used as alternate output for the USIC0Channel0 Shift Data 
  ///  output(DOUT)
  ///  - push/pull output is selected
  ///  P7.4 is used as alternate input for the USIC0Channel0 Shift Data 
  ///  input(DX0D)

  ///  P7.0 - P7.3 output driver characteristic: strong driver
  ///  P7.4 - P7.4 output driver characteristic: strong driver

  ///  P7.0 - P7.3 output edge characteristic: sharp edge mode
  ///  P7.4 - P7.4 output edge characteristic: sharp edge mode


  ///  -----------------------------------------------------------------------
  ///  Configuration of Port P8:
  ///  -----------------------------------------------------------------------
  ///  P8.0 is used as general input
  ///  - pull device is tristate 
  ///  P8.1 is used as general input
  ///  - pull device is tristate 
  ///  P8.2 is used as general input
  ///  - pull device is tristate 
  ///  P8.3 is used as general input
  ///  - pull device is tristate 
  ///  P8.4 is used as general purpose output
  ///  - push/pull output is selected
  ///  - the pin status is low level
  ///  P8.5 is used as general purpose output
  ///  - push/pull output is selected
  ///  - the pin status is low level
  ///  P8.6 is used as general input
  ///  - pull device is tristate 

  ///  P8.0 - P8.3 output driver characteristic: strong driver
  ///  P8.4 - P8.6 output driver characteristic: strong driver

  ///  P8.0 - P8.3 output edge characteristic: sharp edge mode
  ///  P8.4 - P8.6 output edge characteristic: sharp edge mode

  P8_IOCR04      =  0x0080;      // load port control register 4
  P8_IOCR05      =  0x0080;      // load port control register 5

  ///  -----------------------------------------------------------------------
  ///  Configuration of Port P9:
  ///  -----------------------------------------------------------------------
  ///  P9.1 is used as general purpose output
  ///  - push/pull output is selected
  ///  - the pin status is low level
  ///  P9.2 is used as general purpose output
  ///  - push/pull output is selected
  ///  - the pin status is low level
  ///  P9.3 is used as alternate output for the CCU63 output(COUT60)
  ///  - push/pull output is selected
  ///  P9.4 is used as alternate output for the CCU63 output(COUT61)
  ///  - push/pull output is selected
  ///  P9.5 is used as alternate output for the CCU63 output(COUT62)
  ///  - push/pull output is selected
  ///  P9.6 is used as alternate output for the  CCU63 output(COUT63)
  ///  - push/pull output is selected
  ///  P9.7 is used as general purpose output
  ///  - push/pull output is selected
  ///  - the pin status is low level

  ///  P9.0 - P9.3 output driver characteristic: strong driver
  ///  P9.4 - P9.7 output driver characteristic: strong driver

  ///  P9.0 - P9.3 output edge characteristic: sharp edge mode
  ///  P9.4 - P9.7 output edge characteristic: sharp edge mode

  P9_IOCR01      =  0x0080;      // load port control register 1
  P9_IOCR02      =  0x0080;      // load port control register 2
  P9_IOCR07      =  0x0080;      // load port control register 7

  ///  -----------------------------------------------------------------------
  ///  Configuration of Port P10:
  ///  -----------------------------------------------------------------------
  ///  P10.3 is used as general purpose output
  ///  - push/pull output is selected
  ///  - the pin status is high level
  ///  P10.4 is used as general purpose output
  ///  - push/pull output is selected
  ///  - the pin status is high level
  ///  P10.8 is used as general input
  ///  - pull device is tristate 
  ///  P10.9 is used as general input
  ///  - pull device is tristate 
  ///  P10.10 is used as general input
  ///  - pull device is tristate 
  ///  P10.11 is used as general input
  ///  - pull device is tristate 
  ///  P10.12 is used as alternate output for the USIC1Channel0 Shift Data 
  ///  output(DOUT)
  ///  - push/pull output is selected
  ///  P10.13 is used as alternate input for the USIC1Channel0 Shift Data 
  ///  input(DX0D)
  ///  P10.14 is used as alternate input for the USIC0Channel1 Shift Data 
  ///  input(DX0C)
  ///  P10.15 is used as alternate output for the USIC0Channel1 Shift Data 
  ///  output(DOUT)
  ///  - push/pull output is selected

  ///  P10.0 - P10.3 output driver characteristic: strong driver
  ///  P10.4 - P10.7 output driver characteristic: strong driver
  ///  P10.8 - P10.11 output driver characteristic: strong driver
  ///  P10.12 - P10.15 output driver characteristic: strong driver

  ///  P10.0 - P10.3 output edge characteristic: sharp edge mode
  ///  P10.4 - P10.7 output edge characteristic: sharp edge mode
  ///  P10.8 - P10.11 output edge characteristic: sharp edge mode
  ///  P10.12 - P10.15 output edge characteristic: sharp edge mode

  P10_OUT        =  0x0018;      // load data output register
  P10_IOCR03     =  0x0080;      // load port control register 3
  P10_IOCR04     =  0x0080;      // load port control register 4

  ///  -----------------------------------------------------------------------
  ///  Configuration of Port P11:
  ///  -----------------------------------------------------------------------
  ///  - no pin of port P11 is used


  ///  -----------------------------------------------------------------------
  ///  Configuration of Port P15:
  ///  -----------------------------------------------------------------------
  ///  - no pin of port P15 is used
  ///  - Port15 Data register P15(Read only)


} //  End of function IO_vInit_NRTL

// USER CODE END

