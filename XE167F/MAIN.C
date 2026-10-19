//****************************************************************************
// @Module        Project Settings
// @Filename      MAIN.C
// @Project       30_5015_Felix.dav
//----------------------------------------------------------------------------
// @Controller    Infineon XE167F-96F66
//
// @Compiler      Tasking Classic
//
// @Codegenerator 2.2
//
// @Description   This file contains the project initialization function.
//
//----------------------------------------------------------------------------
// @Date          07.03.2016 10:02:14
//
//****************************************************************************

// USER CODE BEGIN (MAIN_General,1)

// USER CODE END



//****************************************************************************
// @Project Includes
//****************************************************************************

#include "MAIN.H"

// USER CODE BEGIN (MAIN_General,2)
#include <assert.h>
#include "Felix_Hardware.h"
#include "Felix_HardwareTask.h"
#include "Felix_MotorTask.h"
#include "Felix_ASyncCtrl.h"
#include "Felix_SlaveTask.h"

// USER CODE END


//****************************************************************************
// @Macros
//****************************************************************************

// USER CODE BEGIN (MAIN_General,3)

// USER CODE END


//****************************************************************************
// @Defines
//****************************************************************************

// USER CODE BEGIN (MAIN_General,4)
#define US_MOTOR		640//Besser 448 //	Größe des CUSTACK
#define US_ASYNCCTRL	320//Besser 256	//	Größe des CUSTACK
#define US_HARDWARE		384//512
#define US_MAIN		 	512//Besser 480	//	Größe des CUSTACK
#define US_SLAVE 	 	128		//	Größe des CUSTACK
// USER CODE END


//****************************************************************************
// @Typedefs
//****************************************************************************

// USER CODE BEGIN (MAIN_General,5)

// USER CODE END


//****************************************************************************
// @Imported Global Variables
//****************************************************************************

// USER CODE BEGIN (MAIN_General,6)

// USER CODE END


//****************************************************************************
// @Global Variables
//****************************************************************************

// USER CODE BEGIN (MAIN_General,7)

unsigned char UserMainSlot;
unsigned char MotorSlot;
unsigned char ASyncCtrlSlot;
unsigned char HardwareSlot;
unsigned char SlaveSlot;
unsigned char g_bBreakFlag= FALSE;
unsigned char g_ucTestFlag= FALSE;
//unsigned short m_DriveEvents = RESET_EVENTS;
//FILE *g_InHandle;
//FILE *g_OutHandle;
unsigned short Bus485_Slave = FALSE;

//unsigned short g_EnableEstopEvent = 1;

// USER CODE END


//****************************************************************************
// @External Prototypes
//****************************************************************************

// USER CODE BEGIN (MAIN_General,8)

// USER CODE END


//****************************************************************************
// @Prototypes Of Local Functions
//****************************************************************************

// USER CODE BEGIN (MAIN_General,9)

// USER CODE END


//****************************************************************************
// @Function      void MAIN_vInit(void) 
//
//----------------------------------------------------------------------------
// @Description   This function initializes the microcontroller.
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

void MAIN_vInit(void)
{
  // USER CODE BEGIN (Init,2)
	unsigned short i = 0;
	unsigned short nrtl_lp = 0;
  // USER CODE END

  //   globally disable interrupts
  PSW_IEN        =  0;          


  ///  -----------------------------------------------------------------------
  ///  Configuration of the System Clock:
  ///  -----------------------------------------------------------------------
  ///  - VCO clock used, input clock is connected
  ///  - input frequency is 8,00 MHz
  ///  - system clock is 66.00 MHz

  MAIN_vUnlockProtecReg();     // unlock write security

  MAIN_vChangeFreq();          // load PLL control register

  //   -----------------------------------------------------------------------
  //   Initialization of the Peripherals:
  //   -----------------------------------------------------------------------

  //   initializes the Parallel Ports
  IO_vInit();

  //   initializes the General Purpose Timer Unit (GPT1)
  GPT1_vInit();

  //   initializes the General Purpose Timer Unit (GPT2)
  GPT2_vInit();

  //   initializes the Capture / Compare Unit 2 (CAPCOM2)
  CC2_vInit();

  //   initializes the Capture / Compare Unit 63 (CCU63)
  CCU63_vInit();

  //   initializes the Real Time Clock (RTC)
  RTC_vInit();

  //   initializes the Interrupt Controller (INT)
  INT_vInit();

  //   initializes the Watchdog Timer (WDT)
  WDT_vInit();

  //   initializes the USIC0 Module
  USIC0_vInit();

  //   initializes the USIC1 Module
  USIC1_vInit();

  //   initializes the USIC2 Module
  USIC2_vInit();


  //   -----------------------------------------------------------------------
  //   Initialization of the Bank Select registers:
  //   -----------------------------------------------------------------------


  // USER CODE BEGIN (Init,3)
	for(i = 0; i < 60;i++)
	{

		if(!P6_IN_P2)
		{
			nrtl_lp++;
		}

	}

	if(nrtl_lp > 50)
	{
		IO_vInit_NRTL();
	}
	else
	{
		IO_vInit_noNRTL();
	}


  // USER CODE END

  MAIN_vLockProtecReg();       // lock write security

  //   globally enable interrupts
  PSW_IEN        =  1;          

} //  End of function MAIN_vInit


//****************************************************************************
// @Function      void MAIN_vUnlockProtecReg(void) 
//
//----------------------------------------------------------------------------
// @Description   This function makes it possible to write one protected 
//                register.
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

// USER CODE BEGIN (UnlockProtecReg,1)

// USER CODE END

void MAIN_vUnlockProtecReg(void)
{
  uword uwPASSWORD;

    SCU_SLC = 0xAAAA;                   // command 0
    SCU_SLC = 0x5554;                   // command 1

    uwPASSWORD = SCU_SLS & 0x00FF;
    uwPASSWORD = (~uwPASSWORD) & 0x00FF;

    SCU_SLC = 0x9600 | uwPASSWORD;      // command 2
    SCU_SLC = 0x0000;                   // command 3

} //  End of function MAIN_vUnlockProtecReg


//****************************************************************************
// @Function      void MAIN_vLockProtecReg(void) 
//
//----------------------------------------------------------------------------
// @Description   This function makes it possible to lock one protected 
//                register.
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

// USER CODE BEGIN (LockProtecReg,1)

// USER CODE END

void MAIN_vLockProtecReg(void)
{
  uword uwPASSWORD;

    SCU_SLC = 0xAAAA;                   // command 0
    SCU_SLC = 0x5554;                   // command 1

    uwPASSWORD = SCU_SLS & 0x00FF;
    uwPASSWORD = (~uwPASSWORD) & 0x00FF;

    SCU_SLC = 0x9600 | uwPASSWORD;      // command 2
    SCU_SLC = 0x1800;                   // command 3; new PASSWOR is 0x00

    uwPASSWORD = SCU_SLS & 0x00FF;
    uwPASSWORD = (~uwPASSWORD) & 0x00FF;
    SCU_SLC = 0x8E00 | uwPASSWORD;      // command 4

} //  End of function MAIN_vLockProtecReg


//****************************************************************************
// @Function      void MAIN_vChangeFreq(void) 
//
//----------------------------------------------------------------------------
// @Description   This function is used to select the external crystal and
//                configure the system frequency to 80Mhz/66Mhz.
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

// USER CODE BEGIN (ChangeFreq,1)

// USER CODE END

void MAIN_vChangeFreq(void)
{
  SCS_SwitchToHighPrecBandgap();

  //For application and internal application resets, the complete PLL configuration could be avoided
  //The entry from application resets and internal application reset is covered in the following differentiation
  //in int/ext clock in lock/unlocked state.

  if  ((SCU_PLLSTAT & 0x0004) == 0x0004)             // fR derived from Internal clock
        {
            //Normal startup state during boot and the clock
            //has to be in the next step configured on the external crystal
            //use XTAL/VCO, count XTAL clock

            SCS_StartXtalOsc(1);                     // Starts the crystal oscillator
            SCS_SwitchSystemClock(1);                // System clock is increased to target speed (80/66 MHz)

        }

  else                                               // fR derived from external crystal clock
        {
            if ((SCU_PLLSTAT & 0x1009) == 0x1009)    // fR derived from external crystal clock + VCO is locked
            {
               //usually after an application reset where clock need not be configured again.
               //check K2/P/N values and decide whether these values have to be adapted based on application needs.
               NOP();
               //usually the PLL losss of Lock TRAP schould be enabled here.
            }
            else                                     //fR derived from external crystal clock + VCO is not locked

            {
               //estimate the K1 value and the current frequency
               //reduce K2/P/N values in steps so that the frequency
               //jumps is limited to 20MHz or factor of 5 whichever is minimum
               NOP();
            }


        }

} //  End of function MAIN_vChangeFreq


//****************************************************************************
// @Function      void main(void) 
//
//----------------------------------------------------------------------------
// @Description   This is the main function.
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

// USER CODE BEGIN (Main,1)
extern void UserMain(void);
//extern void TaskStart(void);
// USER CODE END

void main(void)
{
  // USER CODE BEGIN (Main,2)
	//int i;
	byte ret;

	word16 *pUserStack, *pMotorStack, *pASyncCtrlStack, *pHardwareStack, *pSlaveStack;

	pMotorStack = (word16 *)calloc(US_MOTOR, sizeof(int));
    assert(pMotorStack);
    pMotorStack += US_MOTOR;

	pASyncCtrlStack = (word16 *)calloc(US_ASYNCCTRL, sizeof(int));
    assert(pASyncCtrlStack);
    pASyncCtrlStack += US_ASYNCCTRL;

	pHardwareStack = (word16 *)calloc(US_HARDWARE, sizeof(int));
    assert(pHardwareStack);
    pHardwareStack += US_HARDWARE;

	pSlaveStack = (word16 *)calloc(US_SLAVE, sizeof(int));
    assert(pSlaveStack);
    pSlaveStack += US_SLAVE;

	pUserStack = (word16 *)calloc(US_MAIN, sizeof(int));
	assert(pUserStack);
	pUserStack += US_MAIN;

	K_OS_Init();			// initialize ram and things
  // USER CODE END

  MAIN_vInit();

  // USER CODE BEGIN (Main,3)
INT_vDisableInterrupt(CC2_CH24_INT);  
GPT1_vClearTmr_GPT1_TIMER_4();
	//UserMain();
/*	for(i=1;i<=2;i++)
	{
	LED_1=1;
	Wait(100);
	LED_1=0;
	LED_2=1;
	Wait(100);
	LED_2=0;
	LED_3=1;
	Wait(100);
	LED_3=0;
	LED_2=1;
	Wait(100);
	LED_2=0;
	LED_1=1;
	Wait(100);
	LED_1=0;
	Wait(100);
	}
	Wait(200);*/
	

	//HardwareTask_vInit();
	//MotorTask_vInit();

//	K_Task_Create(PRI_USER_MAIN, &UserMainSlot, UserMain, C_TASK_STK_SIZE);
//	K_Task_Start(UserMainSlot);		  									// trigger task

	ret= K_Task_Create_Stack(PRI_USER_MAIN, &UserMainSlot, UserMain, pUserStack);
	//ret= K_Task_Create(PRI_USER_MAIN, &UserMainSlot, UserMain, 128);
	ret= K_Task_Start(UserMainSlot);                                                                        // trigger task
                
	ret= K_Task_Create_Stack(PRI_MOTOR, &MotorSlot, MotorTask, pMotorStack);
	//ret= K_Task_Create(PRI_PST, &PstSlot, PstTask, 128);
	//ret= K_Mbox_Event_Set(MBX_MOTOR, MotorSlot, EVENT_MOTOR_MBX);
	//ret= K_Mbox_Event_Set(MBX_ASYNCMOTOR, MotorSlot, EVENT_ASYNC_MOTOR_MBX);
	ret= K_Task_Start(MotorSlot);                                                                      // trigger task


	ret= K_Task_Create_Stack(PRI_ASYNCCTRL, &ASyncCtrlSlot, ASyncCtrlTask, pASyncCtrlStack);
	//ret= K_Task_Create(PRI_PST, &PstSlot, PstTask, 128);
	ret= K_Mbox_Event_Set(MBX_ASYNCCTRL, ASyncCtrlSlot, EVENT_ASYNCCTRL_MBX);
	ret= K_Task_Start(ASyncCtrlSlot);                                                                      // trigger task

	ret= K_Task_Create_Stack(PRI_HARDWARE, &HardwareSlot, HardwareTask, pHardwareStack);
	//ret= K_Task_Create(PRI_PST, &PstSlot, PstTask, 128);
	ret= K_Mbox_Event_Set(MBX_HARDWARE, HardwareSlot, EVENT_HARDWARE_MBX);
	ret= K_Task_Start(HardwareSlot);                                                                      // trigger task

	ret= K_Task_Create_Stack(PRI_SLAVE, &SlaveSlot, SlaveTask, pSlaveStack);
	//ret= K_Task_Create(PRI_PST, &PstSlot, PstTask, 128);
	ret= K_Mbox_Event_Set(MBX_SLAVE, SlaveSlot, EVENT_SLAVE_MBX);
	ret= K_Task_Start(SlaveSlot);                                                                      // trigger task
  
    assert(ret == K_OK);

	K_OS_Start();								// enter CMX RTOS

  // USER CODE END

  while(1)
  {

   // USER CODE BEGIN (Main,4)

   // USER CODE END

  }

} //  End of function main



// USER CODE BEGIN (MAIN_General,10)

// USER CODE END

