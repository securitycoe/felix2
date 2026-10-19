//****************************************************************************
// @Module        Capture / Compare Unit 63 (CCU63)
// @Filename      CCU63.H
// @Project       30_5015_Felix.dav
//----------------------------------------------------------------------------
// @Controller    Infineon XE167F-96F66
//
// @Compiler      Tasking Classic
//
// @Codegenerator 2.2
//
// @Description   This file contains all function prototypes and macros for 
//                the CCU63 module.
//
//----------------------------------------------------------------------------
// @Date          07.03.2016 10:02:22
//
//****************************************************************************

// USER CODE BEGIN (CCU63_Header,1)

// USER CODE END

/*
******************************************************************************
@Note

T13  wird zur PWM-Steuerung der Leuchtdioden für die Designelemente verwendet.
Periodendauer ca. 1ms, Auflösung ca. 3,8µs


******************************************************************************
*/


#ifndef _CCU63_H_
#define _CCU63_H_

//****************************************************************************
// @Project Includes
//****************************************************************************

// USER CODE BEGIN (CCU63_Header,2)

// USER CODE END


//****************************************************************************
// @Macros
//****************************************************************************

// USER CODE BEGIN (CCU63_Header,3)

// USER CODE END


//****************************************************************************
// @Defines
//****************************************************************************

//   This parameter identifies CCU63 timer 12
#define CCU63_TIMER_12 12

//   This parameter identifies CCU63 timer 13
#define CCU63_TIMER_13 13

//   This parameter identifies CCU63 channel 0
#define CCU63_CHANNEL_0 0

//   This parameter identifies CCU63 channel 1
#define CCU63_CHANNEL_1 1

//   This parameter identifies CCU63 channel 2
#define CCU63_CHANNEL_2 2

//   This parameter identifies CCU63 channel 3
#define CCU63_CHANNEL_3 3


// USER CODE BEGIN (CCU63_Header,4)

// USER CODE END


//****************************************************************************
// @Typedefs
//****************************************************************************

// USER CODE BEGIN (CCU63_Header,5)

// USER CODE END


//****************************************************************************
// @Imported Global Variables
//****************************************************************************

// USER CODE BEGIN (CCU63_Header,6)

// USER CODE END


//****************************************************************************
// @Global Variables
//****************************************************************************

// USER CODE BEGIN (CCU63_Header,7)

// USER CODE END


//****************************************************************************
// @Prototypes Of Global Functions
//****************************************************************************

void CCU63_vInit(void);


// USER CODE BEGIN (CCU63_Header,8)

// USER CODE END


//****************************************************************************
// @Macro         CCU63_vStartTmr(TimerNr) 
//
//----------------------------------------------------------------------------
// @Description   This macro starts the selected CCU63 timer. The timer 
//                continues counting from where it stopped. 
//                
//                The following definitions for TimerNr are available: 
//                CCU63_TIMER_12
//                CCU63_TIMER_13
//
//----------------------------------------------------------------------------
// @Returnvalue   None
//
//----------------------------------------------------------------------------
// @Parameters    TimerNr: 
//                Name of the CCU63 timer to be started
//
//----------------------------------------------------------------------------
// @Date          07.03.2016
//
//****************************************************************************

#define CCU63_vStartTmr(TimerNr) CCU63_vStartTmr_##TimerNr()
#define CCU63_vStartTmr_CCU63_TIMER_12() CCU63_TCTR4 |= 0x0002
#define CCU63_vStartTmr_CCU63_TIMER_13() CCU63_TCTR4 |= 0x0200


//****************************************************************************
// @Macro         CCU63_vStopTmr(TimerNr) 
//
//----------------------------------------------------------------------------
// @Description   This macro stops the selected CCU63 timer. The content of 
//                the timer register remains unchanged. 
//                
//                The following definitions for TimerNr are available: 
//                CCU63_TIMER_12
//                CCU63_TIMER_13
//
//----------------------------------------------------------------------------
// @Returnvalue   None
//
//----------------------------------------------------------------------------
// @Parameters    TimerNr: 
//                Name of the CCU63 timer to be stopped
//
//----------------------------------------------------------------------------
// @Date          07.03.2016
//
//****************************************************************************

#define CCU63_vStopTmr(TimerNr) CCU63_vStopTmr_##TimerNr()
#define CCU63_vStopTmr_CCU63_TIMER_12() CCU63_TCTR4 |= 0x0001
#define CCU63_vStopTmr_CCU63_TIMER_13() CCU63_TCTR4 |= 0x0100


//****************************************************************************
// @Macro         CCU63_vResetTmr(TimerNr) 
//
//----------------------------------------------------------------------------
// @Description   This macro sets the selected CCU63 timer counter register 
//                to zero. Timer is not stopped.
//                
//                The following definitions for TimerNr are available: 
//                CCU63_TIMER_12
//                CCU63_TIMER_13
//
//----------------------------------------------------------------------------
// @Returnvalue   None
//
//----------------------------------------------------------------------------
// @Parameters    TimerNr: 
//                Name of the CCU63 timer to be cleared
//
//----------------------------------------------------------------------------
// @Date          07.03.2016
//
//****************************************************************************

#define CCU63_vResetTmr(TimerNr) CCU63_vResetTmr_##TimerNr()
#define CCU63_vResetTmr_CCU63_TIMER_12() CCU63_TCTR4 |= 0x0004
#define CCU63_vResetTmr_CCU63_TIMER_13() CCU63_TCTR4 |= 0x0400


//****************************************************************************
// @Macro         CCU63_vSetTmrPeriod(TimerNr, Value) 
//
//----------------------------------------------------------------------------
// @Description   This macro loads the period register of the selected CCU63 
//                timer.
//                
//                Note: Call the macro CCU63_vEnableShadowTransfer() to 
//                enable the shadow transfer.
//                The following definitions for TimerNr are available: 
//                CCU63_TIMER_12
//                CCU63_TIMER_13
//
//----------------------------------------------------------------------------
// @Returnvalue   None
//
//----------------------------------------------------------------------------
// @Parameters    TimerNr: 
//                Name of the CCU63 timer to be used
// @Parameters    Value: 
//                16-bit Value with which the CCU63 timer reload register is 
//                to be loaded
//
//----------------------------------------------------------------------------
// @Date          07.03.2016
//
//****************************************************************************

#define CCU63_vSetTmrPeriod(TimerNr, Value) CCU63_vSetTmrPeriod_##TimerNr(Value)
#define CCU63_vSetTmrPeriod_CCU63_TIMER_12(Value) CCU63_T12PR = Value
#define CCU63_vSetTmrPeriod_CCU63_TIMER_13(Value) CCU63_T13PR = Value


//****************************************************************************
// @Macro         CCU63_uwGetTmrCounter(TimerNr) 
//
//----------------------------------------------------------------------------
// @Description   This macro returns the contens of the selected CCU63 timer 
//                counter register.
//                
//                The following definitions for TimerNr are available: 
//                CCU63_TIMER_12
//                CCU63_TIMER_13
//
//----------------------------------------------------------------------------
// @Returnvalue   16-bit counter value
//
//----------------------------------------------------------------------------
// @Parameters    TimerNr: 
//                Name of the CCU63 timer to be used
//
//----------------------------------------------------------------------------
// @Date          07.03.2016
//
//****************************************************************************

#define CCU63_uwGetTmrCounter(TimerNr) CCU63_uwGetTmrCounter_##TimerNr()
#define CCU63_uwGetTmrCounter_CCU63_TIMER_12() CCU63_T12
#define CCU63_uwGetTmrCounter_CCU63_TIMER_13() CCU63_T13


//****************************************************************************
// @Macro         CCU63_uwSetTmrCounter(TimerNr, Value) 
//
//----------------------------------------------------------------------------
// @Description   This macro loads the selected CCU63 timer counter register 
//                with the parameter Value . It is possible to use this macro 
//                only if the selected timer is stopped.
//                
//                The following definitions for TimerNr are available: 
//                CCU63_TIMER_12
//                CCU63_TIMER_13
//
//----------------------------------------------------------------------------
// @Returnvalue   None
//
//----------------------------------------------------------------------------
// @Parameters    TimerNr: 
//                Name of the CCU63 timer to be used
// @Parameters    Value: 
//                16-bit value with which the CCU63 counter register is to be 
//                loaded
//
//----------------------------------------------------------------------------
// @Date          07.03.2016
//
//****************************************************************************

#define CCU63_uwSetTmrCounter(TimerNr, Value) CCU63_uwSetTmrCounter_##TimerNr(Value)
#define CCU63_uwSetTmrCounter_CCU63_TIMER_12(Value) CCU63_T12 = Value
#define CCU63_uwSetTmrCounter_CCU63_TIMER_13(Value) CCU63_T13 = Value


//****************************************************************************
// @Macro         CCU63_vEnableShadowTransfer(TimerNr) 
//
//----------------------------------------------------------------------------
// @Description   This macro enables the transfer from the shadow registers 
//                to the actual CCU63 registers.
//                
//                The following definitions for TimerNr are available: 
//                CCU63_TIMER_12
//                CCU63_TIMER_13
//
//----------------------------------------------------------------------------
// @Returnvalue   None
//
//----------------------------------------------------------------------------
// @Parameters    TimerNr: 
//                Name of the CCU63 timer to be used
//
//----------------------------------------------------------------------------
// @Date          07.03.2016
//
//****************************************************************************

#define CCU63_vEnableShadowTransfer(TimerNr) CCU63_vEnableShadowTransfer_##TimerNr()
#define CCU63_vEnableShadowTransfer_CCU63_TIMER_12() CCU63_TCTR4 |= 0x0040
#define CCU63_vEnableShadowTransfer_CCU63_TIMER_13() CCU63_TCTR4 |= 0x4000


//****************************************************************************
// @Macro         CCU63_vLoadChannelShadowRegister(ChName, Value) 
//
//----------------------------------------------------------------------------
// @Description   This macro loads the selected CCU63 channel shadow register 
//                with the parameter Value. 
//                
//                The following definitions are available for RegName: 
//                CCU63_CHANNEL_0 .. CCU63_CHANNEL_3 (see @Defines in header 
//                file)
//
//----------------------------------------------------------------------------
// @Returnvalue   None
//
//----------------------------------------------------------------------------
// @Parameters    ChName: 
//                Name of the CCU63 channel to be loaded
// @Parameters    Value: 
//                16-bit Value with which the CCU63 shadow register is to be 
//                loaded
//
//----------------------------------------------------------------------------
// @Date          07.03.2016
//
//****************************************************************************

#define CCU63_vLoadChannelShadowRegister(ChName, Value) CCU63_vLoadChannelShadowRegister_##ChName(Value)
#define CCU63_vLoadChannelShadowRegister_CCU63_CHANNEL_0(Value) CCU63_CC60SR = Value
#define CCU63_vLoadChannelShadowRegister_CCU63_CHANNEL_1(Value) CCU63_CC61SR = Value
#define CCU63_vLoadChannelShadowRegister_CCU63_CHANNEL_2(Value) CCU63_CC62SR = Value
#define CCU63_vLoadChannelShadowRegister_CCU63_CHANNEL_3(Value) CCU63_CC63SR = Value


//****************************************************************************
// @Macro         CCU63_uwGetChannelShadowRegister(ChName) 
//
//----------------------------------------------------------------------------
// @Description   This macro returns the content of the CCU63 capture/compare 
//                shadow register of the selected channel.
//                
//                The following definitions are available for RegName: 
//                CCU63_CHANNEL_0 .. CCU63_CHANNEL_3 (see @Defines in header 
//                file)
//
//----------------------------------------------------------------------------
// @Returnvalue   Current 16-bit value of the CCU63 shadow register
//
//----------------------------------------------------------------------------
// @Parameters    ChName: 
//                Name of the CCU63 channel to be readed
//
//----------------------------------------------------------------------------
// @Date          07.03.2016
//
//****************************************************************************

#define CCU63_uwGetChannelShadowRegister(ChName) CCU63_uwGetChannelShadowRegister_##ChName()
#define CCU63_uwGetChannelShadowRegister_CCU63_CHANNEL_0() CCU63_CC60SR
#define CCU63_uwGetChannelShadowRegister_CCU63_CHANNEL_1() CCU63_CC61SR
#define CCU63_uwGetChannelShadowRegister_CCU63_CHANNEL_2() CCU63_CC62SR
#define CCU63_uwGetChannelShadowRegister_CCU63_CHANNEL_3() CCU63_CC63SR


//****************************************************************************
// @Macro         CCU63_uwGetChannelRegister(ChName) 
//
//----------------------------------------------------------------------------
// @Description   This macro returns the content of the CCU63 capture/compare 
//                register of the selected channel.
//                
//                The following definitions are available for RegName: 
//                CCU63_CHANNEL_0 .. CCU63_CHANNEL_3 (see @Defines in header 
//                file)
//
//----------------------------------------------------------------------------
// @Returnvalue   Current 16-bit value of the CCU63 register
//
//----------------------------------------------------------------------------
// @Parameters    ChName: 
//                Name of the CCU63 channel to be readed
//
//----------------------------------------------------------------------------
// @Date          07.03.2016
//
//****************************************************************************

#define CCU63_uwGetChannelRegister(ChName) CCU63_uwGetChannelRegister_##ChName()
#define CCU63_uwGetChannelRegister_CCU63_CHANNEL_0() CCU63_CC60R
#define CCU63_uwGetChannelRegister_CCU63_CHANNEL_1() CCU63_CC61R
#define CCU63_uwGetChannelRegister_CCU63_CHANNEL_2() CCU63_CC62R
#define CCU63_uwGetChannelRegister_CCU63_CHANNEL_3() CCU63_CC63R


//****************************************************************************
// @Interrupt Vectors
//****************************************************************************



// USER CODE BEGIN (CCU63_Header,9)

// USER CODE END


#endif  // ifndef _CCU63_H_
