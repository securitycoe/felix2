//****************************************************************************
// @Module        Watchdog Timer (WDT)
// @Filename      WDT.H
// @Project       30_5015_Felix.dav
//----------------------------------------------------------------------------
// @Controller    Infineon XE167F-96F66
//
// @Compiler      Tasking Classic
//
// @Codegenerator 2.2
//
// @Description   This file contains all function prototypes and macros for 
//                the WDT module.
//
//----------------------------------------------------------------------------
// @Date          07.03.2016 10:02:23
//
//****************************************************************************

// USER CODE BEGIN (WDT_Header,1)

// USER CODE END



#ifndef _WDT_H_
#define _WDT_H_

//****************************************************************************
// @Project Includes
//****************************************************************************

// USER CODE BEGIN (WDT_Header,2)

// USER CODE END


//****************************************************************************
// @Macros
//****************************************************************************

// USER CODE BEGIN (WDT_Header,3)

// USER CODE END


//****************************************************************************
// @Defines
//****************************************************************************

// USER CODE BEGIN (WDT_Header,4)

// USER CODE END


//****************************************************************************
// @Typedefs
//****************************************************************************

// USER CODE BEGIN (WDT_Header,5)

// USER CODE END


//****************************************************************************
// @Imported Global Variables
//****************************************************************************

// USER CODE BEGIN (WDT_Header,6)

// USER CODE END


//****************************************************************************
// @Global Variables
//****************************************************************************

// USER CODE BEGIN (WDT_Header,7)

// USER CODE END


//****************************************************************************
// @Prototypes Of Global Functions
//****************************************************************************

void WDT_vInit(void);
void WDT_vServiceWDT(void);


// USER CODE BEGIN (WDT_Header,8)

// USER CODE END


//****************************************************************************
// @Function      void WDT_vServiceWDT(void) 
//
//----------------------------------------------------------------------------
// @Description   This function service the watchdog timer. If the watchdog 
//                timer is used in an application, it has to be regularly 
//                serviced to prevent it from overflowing. 
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

#define  WDT_vServiceWDT()  _srvwdt()

// USER CODE BEGIN (WDT_Header,9)
#define WDO() WDT_vServiceWDT()
// USER CODE END



// USER CODE BEGIN (WDT_Header,10)

// USER CODE END



// USER CODE BEGIN (WDT_Header,11)

// USER CODE END



// USER CODE BEGIN (WDT_Header,12)

// USER CODE END



// USER CODE BEGIN (WDT_Header,13)

// USER CODE END



// USER CODE BEGIN (WDT_Header,14)

// USER CODE END


#endif  // ifndef _WDT_H_
