//****************************************************************************
// @Module        Capture / Compare Unit 2 (CAPCOM2)
// @Filename      CC2.H
// @Project       30_5015_Felix.dav
//----------------------------------------------------------------------------
// @Controller    Infineon XE167F-96F66
//
// @Compiler      Tasking Classic
//
// @Codegenerator 2.2
//
// @Description   This file contains all function prototypes and macros for 
//                the CC2 module.
//
//----------------------------------------------------------------------------
// @Date          07.03.2016 10:02:22
//
//****************************************************************************

// USER CODE BEGIN (CC2_Header,1)

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


#ifndef _CC2_H_
#define _CC2_H_

//****************************************************************************
// @Project Includes
//****************************************************************************

// USER CODE BEGIN (CC2_Header,2)

// USER CODE END


//****************************************************************************
// @Macros
//****************************************************************************

// USER CODE BEGIN (CC2_Header,3)

// USER CODE END


//****************************************************************************
// @Defines
//****************************************************************************

//   This parameter identifies CAPCOM2 timer 7
#define CC2_TIMER_7 7

//   This parameter identifies CAPCOM2 timer 8
#define CC2_TIMER_8 8

//   This parameter identifies CAPCOM2 channel 16
#define CC2_CC_16 16

//   This parameter identifies CAPCOM2 channel 17
#define CC2_CC_17 17

//   This parameter identifies CAPCOM2 channel 18
#define CC2_CC_18 18

//   This parameter identifies CAPCOM2 channel 21
#define CC2_CC_21_GreenLED 21

//   This parameter identifies CAPCOM2 channel 22
#define CC2_CC_22_YellowLED 22


// USER CODE BEGIN (CC2_Header,4)

// USER CODE END


//****************************************************************************
// @Typedefs
//****************************************************************************

// USER CODE BEGIN (CC2_Header,5)

// USER CODE END


//****************************************************************************
// @Imported Global Variables
//****************************************************************************

// USER CODE BEGIN (CC2_Header,6)

// USER CODE END


//****************************************************************************
// @Global Variables
//****************************************************************************

// USER CODE BEGIN (CC2_Header,7)

// USER CODE END


//****************************************************************************
// @Prototypes Of Global Functions
//****************************************************************************

void CC2_vInit(void);


// USER CODE BEGIN (CC2_Header,8)
void ClearToggle_CC21(void);
void ClearToggle_CC22(void);

// USER CODE END


//****************************************************************************
// @Macro         CC2_vStartTmr(TimerNr) 
//
//----------------------------------------------------------------------------
// @Description   This macro starts the selected CAPCOM2 timer. The timer 
//                continues counting from where it stopped. 
//                
//                The following definitions for TimerNr are available: 
//                CC2_TIMER_7
//                CC2_TIMER_8
//                (see @Defines in header file)
//
//----------------------------------------------------------------------------
// @Returnvalue   None
//
//----------------------------------------------------------------------------
// @Parameters    TimerNr: 
//                Name of the CAPCOM2 timer to be started
//
//----------------------------------------------------------------------------
// @Date          07.03.2016
//
//****************************************************************************

#define CC2_vStartTmr(TimerNr) CC2_vStartTmr_##TimerNr()
#define CC2_vStartTmr_CC2_TIMER_7() CC2_T78CON_T7R = 1
#define CC2_vStartTmr_CC2_TIMER_8() CC2_T78CON_T8R = 1


//****************************************************************************
// @Macro         CC2_vStopTmr(TimerNr) 
//
//----------------------------------------------------------------------------
// @Description   This macro stops the selected CAPCOM2 timer. The content of 
//                the timer register remains unchanged. 
//                
//                The following definitions for TimerNr are available: 
//                CC2_TIMER_7
//                CC2_TIMER_8
//                (see @Defines in header file)
//
//----------------------------------------------------------------------------
// @Returnvalue   None
//
//----------------------------------------------------------------------------
// @Parameters    TimerNr: 
//                Name of the CAPCOM2 timer to be stopped
//
//----------------------------------------------------------------------------
// @Date          07.03.2016
//
//****************************************************************************

#define CC2_vStopTmr(TimerNr) CC2_vStopTmr_##TimerNr()
#define CC2_vStopTmr_CC2_TIMER_7() CC2_T78CON_T7R = 0
#define CC2_vStopTmr_CC2_TIMER_8() CC2_T78CON_T8R = 0


//****************************************************************************
// @Macro         CC2_vClearTmr(TimerNr) 
//
//----------------------------------------------------------------------------
// @Description   This macro stops the selected CAPCOM2 timer and sets the 
//                timer register to 0. 
//                
//                The following definitions for TimerNr are available: 
//                CC2_TIMER_7
//                CC2_TIMER_8
//                (see @Defines in header file)
//
//----------------------------------------------------------------------------
// @Returnvalue   None
//
//----------------------------------------------------------------------------
// @Parameters    TimerNr: 
//                Name of the CAPCOM2 timer to be cleared
//
//----------------------------------------------------------------------------
// @Date          07.03.2016
//
//****************************************************************************

#define CC2_vClearTmr(TimerNr) CC2_vClearTmr_##TimerNr()
#define CC2_vClearTmr_CC2_TIMER_7() CC2_T78CON_T7R = 0; CC2_T7 = 0x0000
#define CC2_vClearTmr_CC2_TIMER_8() CC2_T78CON_T8R = 0; CC2_T8 = 0x0000


//****************************************************************************
// @Macro         CC2_vLoadTmr(TimerNr, Value) 
//
//----------------------------------------------------------------------------
// @Description   This macro loads the timer register of the selected CAPCOM2 
//                timer with the requested value. The timer need not be 
//                stopped for this happen as a CPU access to the timer 
//                register has a higher priority than a timer increment or a 
//                reload. 
//                
//                The following definitions for TimerNr are available: 
//                CC2_TIMER_7
//                CC2_TIMER_8
//                (see @Defines in header file)
//
//----------------------------------------------------------------------------
// @Returnvalue   None
//
//----------------------------------------------------------------------------
// @Parameters    TimerNr: 
//                Name of the CAPCOM2 timer to be loaded
// @Parameters    Value: 
//                16-bit Value with which the CAPCOM2 timer is to be loaded
//
//----------------------------------------------------------------------------
// @Date          07.03.2016
//
//****************************************************************************

#define CC2_vLoadTmr(TimerNr, Value) CC2_vLoadTmr_##TimerNr(Value)
#define CC2_vLoadTmr_CC2_TIMER_7(Value) CC2_T7 = Value
#define CC2_vLoadTmr_CC2_TIMER_8(Value) CC2_T8 = Value


//****************************************************************************
// @Macro         CC2_vSetReload(TimerNr, Value) 
//
//----------------------------------------------------------------------------
// @Description   This macro loads the reload register of the selected 
//                CAPCOM2 timer.
//                
//                The following definitions for TimerNr are available: 
//                CC2_TIMER_7
//                CC2_TIMER_8
//                (see @Defines in header file)
//
//----------------------------------------------------------------------------
// @Returnvalue   None
//
//----------------------------------------------------------------------------
// @Parameters    TimerNr: 
//                Name of the CAPCOM2 timer to be used
// @Parameters    Value: 
//                16-bit Value with which the CAPCOM2 timer reload register 
//                is to be loaded
//
//----------------------------------------------------------------------------
// @Date          07.03.2016
//
//****************************************************************************

#define CC2_vSetReload(TimerNr, Value) CC2_vSetReload_##TimerNr(Value)
#define CC2_vSetReload_CC2_TIMER_7(Value) CC2_T7REL = Value
#define CC2_vSetReload_CC2_TIMER_8(Value) CC2_T8REL = Value


//****************************************************************************
// @Macro         CC2_vSetCCxReg(RegName, Value) 
//
//----------------------------------------------------------------------------
// @Description   This macro loads the selected CAPCOM2 register with the 
//                parameter Value. 
//                
//                The following definitions are available for RegName (only 
//                for enabled CAPCOM2 channels): 
//                CC2_CC_16 .. CC2_CC_31
//                (see @Defines in header file)
//
//----------------------------------------------------------------------------
// @Returnvalue   None
//
//----------------------------------------------------------------------------
// @Parameters    RegName: 
//                Name of the CAPCOM2 channel to be loaded
// @Parameters    Value: 
//                16-bit Value with which the CAPCOM2 register is to be loaded
//
//----------------------------------------------------------------------------
// @Date          07.03.2016
//
//****************************************************************************

#define CC2_vSetCCxReg(RegName, Value) CC2_vSetCCxReg_##RegName(Value)
#define CC2_vSetCCxReg_CC2_CC_16(Value) CC2_CC16 = Value
#define CC2_vSetCCxReg_CC2_CC_17(Value) CC2_CC17 = Value
#define CC2_vSetCCxReg_CC2_CC_18(Value) CC2_CC18 = Value
#define CC2_vSetCCxReg_CC2_CC_19(Value) CC2_CC19 = Value
#define CC2_vSetCCxReg_CC2_CC_20(Value) CC2_CC20 = Value
#define CC2_vSetCCxReg_CC2_CC_21_GreenLED(Value) CC2_CC21 = Value
#define CC2_vSetCCxReg_CC2_CC_22_YellowLED(Value) CC2_CC22 = Value
#define CC2_vSetCCxReg_CC2_CC_23(Value) CC2_CC23 = Value
#define CC2_vSetCCxReg_CC2_CC_24(Value) CC2_CC24 = Value
#define CC2_vSetCCxReg_CC2_CC_25(Value) CC2_CC25 = Value
#define CC2_vSetCCxReg_CC2_CC_26(Value) CC2_CC26 = Value
#define CC2_vSetCCxReg_CC2_CC_27(Value) CC2_CC27 = Value
#define CC2_vSetCCxReg_CC2_CC_28(Value) CC2_CC28 = Value
#define CC2_vSetCCxReg_CC2_CC_29(Value) CC2_CC29 = Value
#define CC2_vSetCCxReg_CC2_CC_30(Value) CC2_CC30 = Value
#define CC2_vSetCCxReg_CC2_CC_31(Value) CC2_CC31 = Value


//****************************************************************************
// @Interrupt Vectors
//****************************************************************************

#define CC2_T7INT    0x26
#define CC2_T8INT    0x27
#define CC2_CC16INT    0x10
#define CC2_CC17INT    0x11
#define CC2_CC18INT    0x12
#define CC2_CC21INT    0x15
#define CC2_CC22INT    0x16


// USER CODE BEGIN (CC2_Header,8)
void ClearToggle_CC21(void);
void ClearToggle_CC22(void);

// USER CODE END


#endif  // ifndef _CC2_H_
