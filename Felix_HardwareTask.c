//****************************************************************************
// @Module        HardwareTask
// @Filename      
// @Project       common
//----------------------------------------------------------------------------
// @Controller    Infineon XE167F
//
// @Compiler      Tasking
//
// 
//
// @Description   This file contains functions and variables 
//                for controlling z-axis.
//
//	Besonderheit: beim Anfahren der Null-Lage wird eine mechanische Lose
//	berücksichtigt so dass definiert weiter in die Null-Lage
//	eingetaucht wird, bevor der Antrieb abbremst und die Null-Lage
//	verlässt (NUll-Punkt gefunden)
//----------------------------------------------------------------------------
// @Date          
//
//****************************************************************************

//****************************************************************************
// include files
//****************************************************************************
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <ctype.h>
#include "DevError.h"
#include "30-5015_Felix/XE167F/main.h"
#include "30-5015_Felix/XE167F/GPT2.h"
#include "30-5015_Felix/XE167F/CC2.h"
//#include "Shared/Stacker_Lifter_Allgemein/StacError.h"
#include "Cxconfig.h"
#include "DeviceConfig.h"
#include "Felix_Hardware.h"
#include "Felix_HardwareTask.h"
#include "Felix_Misc.h"
#include "Drv_config.h"
#include "Felix_UserMain.h"
#include "Felix_ASyncCtrl.h"
#include "MDriveCmd.h"
//****************************************************************************
// local definitions
//****************************************************************************
//#define US_HardwareSlot	  128//256		//	Größe des CUSTACK
#define TIMERBASE 0.015515F //millisekunden
//#define TIMESEC_03 15 // Basis = 20ms, 15 = 300ms = 0,3sec
#define TIMESEC_034 17 // Basis = 20ms, 17 = 340ms = 0,34sec
//#define TIMESEC_04 20 // Basis = 20ms, 20 = 400ms = 0,4sec
//#define TIMESEC_05 25 // Basis = 20ms, 25 = 500ms = 0,5sec
//#define TIMESEC_1 50 // Basis = 20ms, 50 = 1000ms = 1sec
//#define TIMESEC_15 75 // Basis = 20ms, 75 = 1500ms = 1,5sec
//#define TIMESEC_17 88 // Basis = 20ms, 88 = 1760ms = 1,7sec
#define TIMESEC_18 90 // Basis = 20ms, 90 = 1800ms = 1,8sec
#define TIMESEC_2 100 // Basis = 20ms, 100 = 2000ms = 2sec
#define TIMESEC_3 150 // Basis = 20ms, 150 = 3000ms = 3sec
#define PressKEY_RESET 0
#define NoPressKEY_START 1
#define PressKEY_2sec_FENSTER 2
#define NoPressKEY_3sec_SLOMO_AKTIVIERUNG 3
#define PressKEY_SLOMO_ACTIVE 4
#define PressZ_MOVE_WAIT_START 5
#define NoPressZ_MOVE_START 6

#define SLOW_BLINK_TIME 800//750
#define FAST_BLINK_TIME 250//200
#define SUPERFAST_BLINK_TIME 75

#define Z_SLEW 2000//4100//8300	//4500

enum BLINKTYPE
{
//	NoBlink		= 0
	NoBlinkOff	= 0
	,NoBlinkOn =  1
	,SlowBlink		= 2
	,FastBlink		= 3
	,SuperFastBlink	= 4
};

//****************************************************************************
// default settings
//****************************************************************************

//****************************************************************************
// external functions
//****************************************************************************

//****************************************************************************
// external data
//****************************************************************************
extern unsigned char HardwareSlot;
extern unsigned char MotorSlot;
extern unsigned short gDoppelTasteUI; 
//extern unsigned short gNrtl_LP;
//****************************************************************************
// internal functions
//****************************************************************************

//void HardwareTask(void);
//****************************************************************************
// public data
//****************************************************************************

//****************************************************************************
// private data
//****************************************************************************
HardwareCommandBlock *m_ptrHW;
//unsigned short m_GreenSlowBlink = 0;
//unsigned short m_YellowSlowBlink = 0;
unsigned short m_GreenBlink = NoBlinkOff;
unsigned short m_YellowBlink = NoBlinkOff;

unsigned short keyState = PressKEY_RESET;
unsigned short ZeitMessung = 0;
unsigned short BlinkErrorZeit = 0;
unsigned short m_ErrCnt = 0;
short m_ErrBlinkCnt = 0;
unsigned short BE_TogglePause;
unsigned short BE_ToggleLEDOn;

//unsigned short m_isSlomoActive = 0;
//unsigned short m_isNormalFahrenActive = 1;
//unsigned short m_SperrenBis_CmdContSec = 0;
//unsigned short m_DriveMode = DRV_MODE_NORMAL;

unsigned short m_ZDirToHome = 0;
unsigned short m_ZHandMoveEnable = 0;
unsigned short m_RedOnFlag = 0;
unsigned short m_Save_ErrorAxisHW[MAX_AXES];
unsigned short m_Save_ErrorAxisHWStall[MAX_AXES];
unsigned short m_muell_on = 0;
unsigned short m_ZHandMoveToHome[MAX_AXES];
//unsigned short DbgStep = 0;
//unsigned short EnableIntrEstopEvent = 1;
unsigned short ResetEstopCount = 0;
unsigned short GenerateEstopEvent = 1;
//****************************************************************************
// private functions
//****************************************************************************
void GreenPrepareTimer(unsigned short Time);
void GreenStartTimer(void);
void GreenClearTimer(void);
void YellowPrepareTimer(unsigned short Time);
void YellowStartTimer(void);
void YellowClearTimer(void);
void YellowSlowBlink(void);
void GreenSlowBlink(void);
void YellowFastBlink(void);
void GreenFastBlink(void);
void YellowSuperFastBlink(void);
void GreenSuperFastBlink(void);
void YellowOff(void);
void YellowOn(void);
void GreenOn(void);
void GreenOff(void);
void RedOn(void);
void RestoreGreen(unsigned short Blink);
unsigned short Z_MoveStart(void);
unsigned short Z_MoveStop(void);
unsigned short Z_MoveToEndPos(void);
//unsigned short Get_AxisID(unsigned short Mask);
//unsigned short Get_IdxFor_pIMSPara(unsigned short Mask);
void Z_ErrorStop(void);
void SaveZAxisError(void);
void RestoreZAxisError(void);
//#ifdef _CF_NRTL
//unsigned short FetTestEStop(unsigned short HostMode);
//unsigned short FetTestResetEStop(void);
unsigned short CheckMOSFETSwitch(void);
unsigned short CheckMOSFETSwitchLoop(void);
void SetInitError(unsigned short ErrorState);
//#endif
//void NormalFahrenAktiviert(void);
//void NormalFahrenDeaktiviert(void);
//****************************************************************************
// public functions
//****************************************************************************
/*void HardwareTask_vInit(void)
{
	int * ptr;
	unsigned char status;

	ptr = calloc(US_HardwareSlot, sizeof(int));
	ptr += US_HardwareSlot;
	status = K_Task_Create_Stack(PRI_HARDWARE, &HardwareSlot, HardwareTask, (void*)ptr);	// create task
	status = K_Task_Start(HardwareSlot);		  							// trigger task
} //  End of function HardwareTask_vInit
*/
unsigned short gRedOnFlag(void)
{
	return m_RedOnFlag;
}

/*unsigned short GetHardwareRevision(unsigned short *HW)
{
	HardwareCommandBlock Msg;

	Msg.eCommand = cGetHardwareRevision;
	K_Mesg_Send_Wait(MBX_HARDWARE, 0, &Msg);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	*HW = Msg.Parameter1;
	return Msg.Error;
}
*/
void LED_Red_On(void)
{
	HardwareCommandBlock Msg;

	Msg.eCommand = cLED_Red_On;
//	Msg.Parameter1 = ON;
	K_Mesg_Send_Wait(MBX_HARDWARE, 0, &Msg);
	m_RedOnFlag = 1;
}

void LED_Red_Off(void)
{
	HardwareCommandBlock Msg;

	Msg.eCommand = cLED_Red_Off;
//	Msg.Parameter1 = OFF;
	K_Mesg_Send_Wait(MBX_HARDWARE, 0, &Msg);
	m_RedOnFlag = 0;
}

void LED_Green_On(void)
{
	HardwareCommandBlock Msg;

	Msg.eCommand = cLED_Green_On;
//	Msg.Parameter1 = ON;
	K_Mesg_Send_Wait(MBX_HARDWARE, 0, &Msg);
}

void LED_Green_Off(void)
{
	HardwareCommandBlock Msg;

	Msg.eCommand = cLED_Green_Off;
//	Msg.Parameter1 = OFF;
	K_Mesg_Send_Wait(MBX_HARDWARE, 0, &Msg);
}
void LED_Green_SLowBlink(void)
{
	HardwareCommandBlock Msg;
	Msg.eCommand = cLED_Green_SlowBlink;
	K_Mesg_Send_Wait(MBX_HARDWARE, 0, &Msg);
}
void LED_Green_FastBlink(void)
{
	HardwareCommandBlock Msg;
	Msg.eCommand = cLED_Green_FastBlink;
	K_Mesg_Send_Wait(MBX_HARDWARE, 0, &Msg);
}
void LED_Green_SuperFastBlink(void)
{
	HardwareCommandBlock Msg;
	Msg.eCommand = cLED_Green_SuperFastBlink;
	K_Mesg_Send_Wait(MBX_HARDWARE, 0, &Msg);
}

void LED_RedGreen_Off(void)
{
	HardwareCommandBlock Msg;

	Msg.eCommand = cLED_RedGreen_Off;
//	Msg.Parameter1 = OFF;
	K_Mesg_Send_Wait(MBX_HARDWARE, 0, &Msg);
	m_RedOnFlag = 0;
}

void LED_Yellow_On(void)
{
	HardwareCommandBlock Msg;

	Msg.eCommand = cLED_Yellow_On;
//	Msg.Parameter1 = ON;
	K_Mesg_Send_Wait(MBX_HARDWARE, 0, &Msg);
}

void LED_Yellow_Off(void)
{
	HardwareCommandBlock Msg;

	Msg.eCommand = cLED_Yellow_Off;
//	Msg.Parameter1 = OFF;
	K_Mesg_Send_Wait(MBX_HARDWARE, 0, &Msg);

}
void LED_Yellow_SlowBlink(void)
{
	HardwareCommandBlock Msg;

	Msg.eCommand = cLED_Yellow_SlowBlink;
	K_Mesg_Send_Wait(MBX_HARDWARE, 0, &Msg);
}

void LED_Yellow_FastBlink(void)
{
	HardwareCommandBlock Msg;

	Msg.eCommand = cLED_Yellow_FastBlink;
	K_Mesg_Send_Wait(MBX_HARDWARE, 0, &Msg);
}
void LED_Yellow_SuperFastBlink(void)
{
	HardwareCommandBlock Msg;

	Msg.eCommand = cLED_Yellow_SuperFastBlink;
	K_Mesg_Send_Wait(MBX_HARDWARE, 0, &Msg);
}
void LED_SystemReady(void)
{
	HardwareCommandBlock Msg;

	Msg.eCommand = cLED_SystemReady;
	K_Mesg_Send_Wait(MBX_HARDWARE, 0, &Msg);
}
unsigned short HardwareEquipmentCtrl(unsigned short EquipmentCmd, unsigned short EquipmentAdr, unsigned short Control)
{	
	unsigned char status;
	HardwareCommandBlock Msg;

	//if(EquipmentAdr != 1) {return ERR_WRONG_PARAMETER;}
	Msg.eCommand = EquipmentCmd;
	Msg.Parameter1 = EquipmentAdr;
	Msg.Parameter2 = Control;
	status = K_Mesg_Send_Wait(MBX_HARDWARE, 0, &Msg);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	return Msg.Error;
}
//#ifdef _CF_NRTL
/*unsigned short MsgFetTestEStop(void)
{
	unsigned char status;
	HardwareCommandBlock Msg;

	Msg.eCommand = cFetTestEStop;
	status = K_Mesg_Send_Wait(MBX_HARDWARE, 0, &Msg);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	return Msg.Error;
}*/
unsigned short g_MsgCheckMOSFETSwitch(void)
{
	unsigned char status;
	HardwareCommandBlock Msg;

	Msg.eCommand = cCheckMOSFETSwitch;
	status = K_Mesg_Send_Wait(MBX_HARDWARE, 0, &Msg);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	return Msg.Error;
}
//#endif
unsigned short g_MsgCheckEstopState(unsigned short *State)
{
	unsigned char status;
	HardwareCommandBlock Msg;

	Msg.eCommand = cCheckEstopState;
	status = K_Mesg_Send_Wait(MBX_HARDWARE, 0, &Msg);
	if(status != K_OK)
	{
		return ERR_SYSTEM;
	}
	*State = Msg.Parameter1;
	return Msg.Error;
}
void HardwareTask(void)
{
	unsigned short event, i, error;
	unsigned short Save2 = NoBlinkOff;
	//static unsigned short offen = FALSE;
	static unsigned short EnableZeitmessung = 0;
	static unsigned short EnableBlinkError = 0;
	static unsigned short SystemReady = 0;
	//static unsigned short FlagYellowLEDOn = 0;
	//static unsigned short save_estop = 0;
	//static unsigned short save_extern_open = 0;
	//static unsigned short HaubeOffen = 0;
	static unsigned short EstopOffen = 0;
	//static unsigned short nur_einmal = 0;
//	static unsigned short Haube_nur_einmal = 0;
	static unsigned short ErlaubeTastenBedinung = 0;
	static long MuellTime = 0;
	static unsigned short MuellEstop = 0;
	static unsigned short m_ToolTestCtrl = 0;
	static unsigned short m_ToolTestState = 0;
//	static unsigned short ResetEstopCount = 0;

	//help = K_Mbox_Event_Set(MBX_HARDWARE, HardwareSlot, EVENT_HARDWARE_MBX);
	gDoppelTasteUI = 0;
	//gNrtl_LP = 0;
	i = 0;Save2 = 0;
	for(event = 0; event < 100;event++)
	{
		K_Task_Wait(2);
		if(!DOPPEL_TASTEN_UI)
		{
			i++;
		}
//#ifdef _CF_NRTL
		if(!NRTL_LP)
		{
			Save2++;
		}
//#endif
	}
	if(i > 90){gDoppelTasteUI = 1;}
//#ifdef _CF_NRTL
	if(Save2 > 90){SetNRTL_LP();/*gNrtl_LP = 1;*/}
//#endif
	Save2 = NoBlinkOff;
	SetTimeBlink(0);
//	CC2_vSetCCxReg_CC2_CC_21_GreenLED(0);
//	CC2_vSetCCxReg_CC2_CC_22_YellowLED(0);


xHW_LED_RedGreen_Off();
xHW_LED_Yellow_Off();
K_Task_Wait(1);
xHW_LED_Red_On();
xHW_LED_Yellow_On();
K_Task_Wait(200);
xHW_LED_Green_On();
K_Task_Wait(200);
xHW_LED_Yellow_Off();
xHW_LED_RedGreen_Off();

	do{
		event = K_Event_Wait(EVENT_HARDWARE_MBX, 500, 2);
		if(event & EVENT_HARDWARE_MBX)
		{
			m_ptrHW = K_Mesg_Get(MBX_HARDWARE);	//  Message holen
			switch(m_ptrHW->eCommand)
			{
				case cLED_SystemReady	:	K_Mesg_Ack_Sender();
											SystemReady = 1;
											break; 
				case cLED_Red_On		:	K_Mesg_Ack_Sender();GreenClearTimer();m_GreenBlink = NoBlinkOn;xHW_LED_Red_On();
											break; 
				case cLED_Green_On		:	K_Mesg_Ack_Sender();GreenOn();
										 	break; 
				default					:	m_ptrHW->eCommand = cHWNone;
											K_Mesg_Ack_Sender();
											break; 
			}
		}
	}while(!SystemReady);
	m_ErrCnt = 0;
	m_ErrBlinkCnt = 0;

	GreenOn();
	if(!g_DeviceConfig.ExternEstopInEnable	&& 	!g_DeviceConfig.InternEstopEnable /* && !FlagYellowLEDOn  && SystemReady*/)
	{
// ##newestop nur temporäer deaktiviert		YellowOn();
		if(isHostMode()){Save2 = SlowBlink;}else{Save2 = NoBlinkOn;}
	}
	if(isDeviceInitError()){EnableBlinkError = 1;BlinkErrorZeit = 0;BE_TogglePause = 0;}
	while(1)
	{
		if(g_isEstopFlag())
		{
			//DbgStep = 20;
			if(ESTOP)	// low active
			{
				ResetEstopCount++;
			}
			else
			{
				ResetEstopCount = 0;
			}
			if(ResetEstopCount >= 10)
			{
				ResetEstopCount = 0;
				//DbgStep = 21;
				GenerateEstopEvent = 1;
				K_Intrp_Event_Signal(3, 0, ESTOP_RESET_EVENT);
			}
		}
		else
		{

			if(!ESTOP && GenerateEstopEvent)	// low active
			{
				//dbPrint("EE\r");
				GenerateEstopEvent = 0;
				K_Intrp_Event_Signal(3, 0, ESTOP_EVENT);
			}
		}
		//DbgStep = 22;
		event = K_Event_Wait(EVENT_HARDWARE_MBX | TASTEN_EVENT | ESTOP_EVENT | ESTOP_RESET_EVENT | SLOMO_EVENT | SLOMO_RESET_EVENT, 20, 2);
		//DbgStep = 23;
		if(EnableZeitmessung){ZeitMessung++;}
		if(EnableBlinkError){BlinkErrorZeit++;}
		if(BlinkErrorZeit > TIMESEC_18 && BE_TogglePause)
		{
			// Pause zw. dem Blinken
			do{
				WDO();
				m_ErrBlinkCnt = g_errArray[m_ErrCnt++];
				if(m_ErrCnt >= (MAX_AXES + MAX_CNT_ISTATE)){m_ErrCnt = 0;}

			}while(!m_ErrBlinkCnt);
			BE_TogglePause = 0;
			BE_ToggleLEDOn = 1;
			BlinkErrorZeit = 0;
		}

		if(BlinkErrorZeit > TIMESEC_034 && BE_ToggleLEDOn && !BE_TogglePause)
		{
			// Blinken, LED ist an
			GreenClearTimer();m_GreenBlink = NoBlinkOn;xHW_LED_Red_On();
			m_ErrBlinkCnt--;
			BE_ToggleLEDOn = 0;
			BlinkErrorZeit = 0;
		}

		if(BlinkErrorZeit > TIMESEC_034 && !BE_ToggleLEDOn && !BE_TogglePause)
		{
			// Blinken, LED ist aus
			GreenOff();
			if(m_ErrBlinkCnt <= 0)
			{
				BE_TogglePause = 1;
			}
			BE_ToggleLEDOn = 1;
			BlinkErrorZeit = 0;
		}
		if(m_ToolTestCtrl)
		{
			if(!MUELL_IN)
			{ClearPortBit(bitPORT_MUELL_CTRL);m_ToolTestState = 0;}
			else
			{	if(!m_ToolTestState)
				{m_ToolTestState = 1;SetPortBit(bitPORT_MUELL_CTRL);}
			}
		}
		if(MuellTime || m_muell_on)
		{
			if(!EstopOffen)
			{
				if(MuellEstop)
				{
					SetPortBit(bitPORT_MUELL_CTRL);
					MuellEstop = 0;
				}
				if(--MuellTime == 0)
				{
					ClearPortBit(bitPORT_MUELL_CTRL);m_muell_on = 0;
				}
				if(!MUELL_IN)
				{
					ClearPortBit(bitPORT_MUELL_CTRL);
					MuellTime = 0;m_muell_on = 0;
				}
			}
			if(event & ESTOP_EVENT)
			{
				ClearPortBit(bitPORT_MUELL_CTRL);
				MuellEstop = 1;
			}

		}
		if(event & EVENT_HARDWARE_MBX)
		{
			//DbgStep = 1;
			m_ptrHW = K_Mesg_Get(MBX_HARDWARE);	// auf Message warten
			//DbgStep = 2;

			//ptr = K_Mesg_Wait( MBX_HARDWARE, 10);	// auf Message warten
			switch(m_ptrHW->eCommand)
			{
				case cLED_Red_On		:	m_ptrHW->Error = errNone;K_Mesg_Ack_Sender();RedOn();
											break; 
				case cLED_Green_On		:	m_ptrHW->Error = errNone;K_Mesg_Ack_Sender();GreenOn();
										 	break; 
				case cLED_Red_Off		:
				case cLED_Green_Off		:
				case cLED_RedGreen_Off	:	m_ptrHW->Error = errNone;
											K_Mesg_Ack_Sender();
											GreenOff();
										 	break;
				case cLED_Yellow_On		:	m_ptrHW->Error = errNone;
											K_Mesg_Ack_Sender();
											YellowOn();
											#ifdef _CF_DEBUG_PRINT
											if(g_TestFlag & TF_DBG_HW){dbPrint("Yellow_1\r");}
											#endif
											break; 
				case cLED_Yellow_Off	:	m_ptrHW->Error = errNone;
											K_Mesg_Ack_Sender();
											YellowOff();
											break; 
				case cLED_Green_SlowBlink:	
											m_ptrHW->Error = errNone;
										 	K_Mesg_Ack_Sender();
											GreenSlowBlink();
										 	break; 
				case cLED_Yellow_SlowBlink	:
											m_ptrHW->Error = errNone;
										 	K_Mesg_Ack_Sender();
											YellowSlowBlink();
											break;
				case cLED_Green_FastBlink:	m_ptrHW->Error = errNone;
										 	K_Mesg_Ack_Sender();
											GreenFastBlink();
										 	break; 
				case cLED_Yellow_FastBlink	:	m_ptrHW->Error = errNone;
										 	K_Mesg_Ack_Sender();
											YellowFastBlink();
										 	break; 
				case cLED_Green_SuperFastBlink:	m_ptrHW->Error = errNone;
										 	K_Mesg_Ack_Sender();
											GreenSuperFastBlink();
										 	break; 
				case cLED_Yellow_SuperFastBlink	:	m_ptrHW->Error = errNone;
										 	K_Mesg_Ack_Sender();
											YellowSuperFastBlink();
										 	break; 
				case cLED_SystemReady	:	m_ptrHW->Error = errNone;K_Mesg_Ack_Sender();
											SystemReady = 1;
											break; 
				case cToolTest			:	
											if(m_ptrHW->Parameter1 & BIT_E_MuellFoerderer) 
											{
												MuellTime = 0;m_muell_on = 0;
												if(m_ptrHW->Parameter2)
												{
													m_ToolTestCtrl = 1;	
													m_ToolTestState = 0;
													if(MUELL_IN)
													{
														SetPortBit(bitPORT_MUELL_CTRL);
														m_ToolTestState = 1;
													}
												}
												else
												{
													ClearPortBit(bitPORT_MUELL_CTRL);m_ToolTestCtrl = 0;m_ToolTestState = 0;
												}
												m_ptrHW->Error = errNone;
											}
											else{m_ptrHW->Error = ERR_VALUE_OUT_OF_RANGE;}
										 	K_Mesg_Ack_Sender();
											break;
				case cEquipmentCtrl	:		
											if(m_ptrHW->Parameter1 & BIT_E_MuellFoerderer) 
											{MuellTime = 0;
												if(MUELL_IN)
												{
													if(m_ptrHW->Parameter2)
													{SetPortBit(bitPORT_MUELL_CTRL);m_muell_on = 1;}
													else
													{ClearPortBit(bitPORT_MUELL_CTRL);m_muell_on = 0;}
													m_ptrHW->Error = errNone;
												}
												else
												{m_ptrHW->Error = ERR_NO_WASTE_BOX;}
											}
											else{m_ptrHW->Error = ERR_VALUE_OUT_OF_RANGE;}
										 	K_Mesg_Ack_Sender();
										 	break; 
				case cEquipmentTime	:		if(m_ptrHW->Parameter1 & BIT_E_MuellFoerderer)
											{
												m_ptrHW->Error = errNone;
												if(m_ptrHW->Parameter2 == 0)
												{
													ClearPortBit(bitPORT_MUELL_CTRL);
													MuellTime = 0;m_muell_on = 0;
												}
												else
												{
													if(MUELL_IN)
													{
														MuellTime = m_ptrHW->Parameter2 * (1000/20);
														SetPortBit(bitPORT_MUELL_CTRL);
													}
													else
													{
														m_ptrHW->Error = ERR_NO_WASTE_BOX;
														MuellTime = 0;m_muell_on = 0;
													}
												}
											}
											else{m_ptrHW->Error = ERR_VALUE_OUT_OF_RANGE;}
										 	K_Mesg_Ack_Sender();
										 	break;
				//#ifdef _CF_NRTL

				case cCheckMOSFETSwitch :	m_ptrHW->Error = CheckMOSFETSwitch();
											if(m_ptrHW->Error == ERR_SAFETY_MONITORING)
											{
												SetInitError(istate_MotorFETEstopL);SetDeviceInitError();
												EnableBlinkError = 1;BlinkErrorZeit = 0;BE_TogglePause = 0;
											}
											K_Mesg_Ack_Sender();
											break;
				//#endif
				case cCheckEstopState:		m_ptrHW->Error = errNone;
											m_ptrHW->Parameter1 = g_isEstopFlag();
											K_Mesg_Ack_Sender();
											break;
				default					:	m_ptrHW->eCommand = cHWNone;
											K_Mesg_Ack_Sender();
											break; 
			}
		}//end...if(event & EVENT_HARDWARE_MBX)

		if(event & (TASTEN_EVENT | ESTOP_EVENT | ESTOP_RESET_EVENT | SLOMO_EVENT | SLOMO_RESET_EVENT))
		{
			if(event & TASTEN_EVENT)
			{
				if(!isNRTL_LP())
				{
					if(!ErlaubeTastenBedinung)
					{
						if(TA_SERVICE || (TA2_SERVICE && gDoppelTasteUI));
						else{CLEAR_FW_ESTOP = 0;K_Task_Wait(1);CLEAR_FW_ESTOP = 1;}
					}
				}
			}
			if(event & ESTOP_EVENT)
			{
//{dbPrint("ESTOP Event\r");}
				#ifdef _CF_DEBUG_PRINT
				if(g_TestFlag & TF_DBG_HW){dbPrint("ESTOP Event\r");}
				#endif
				//DbgStep = 3;
				//EnableIntrEstopEvent = 0;
				//EstopCount = 0;
				YellowOn();//Haube_nur_einmal = 0;
				//DbgStep = 4;
				if(isNRTL_LP())
				{
					SetFwEStopLock();
					if(m_ZHandMoveEnable)
					{
						GreenOn();
						if(Z_MoveStop() != errNone){RedOn();}
						if(!isDeviceInitError()) RestoreGreen(Save2);
						keyState = PressKEY_RESET;
					}
					else
					{
						
						if(keyState == PressZ_MOVE_WAIT_START)
						{
							//DbgStep = 5;
							GreenOn();
							if(!isDeviceInitError()) RestoreGreen(Save2);
							keyState = PressKEY_RESET;
						}
					}
				}
				//SetDriveMode(DRV_MODE_ESTOP);
				//DbgStep = 6;
				EstopOffen = 1;
				K_Task_Wait(50);
				if(!isNRTL_LP())
				{
					if(!isHostMode())
					{
						ErlaubeTastenBedinung = 1;
					}
				}

			}
			if(!isNRTL_LP())
			{
				//-----------------------------------------------------------------------------------------
				if(ErlaubeTastenBedinung || isHostMode())
				{
					if(event & (ESTOP_RESET_EVENT | SLOMO_RESET_EVENT))
					{
						//ErlaubeTastenBedinung = 0;
						//YellowOff();
						#ifdef _CF_DEBUG_PRINT
						if(g_TestFlag & TF_DBG_HW){dbPrint("Reset ESTOP-SLOMO Event\r");}
						#endif
						if(!(event & SLOMO_EVENT))
						{
							ErlaubeTastenBedinung = 0;
							EstopOffen = 0;
							#ifdef _CF_DEBUG_PRINT
							if(g_TestFlag & TF_DBG_HW){dbPrint("!SLOMO Event\r");}
							#endif
							// ----------------------------------------------
							YellowOff();
							keyState = PressKEY_RESET;
							//SetDriveMode(DRV_MODE_NORMAL);
							if(isHostMode() && !isDeviceInitError()){GreenSlowBlink();}else{GreenOn();}
							g_ClearEstopFlag();
							if(!isHostMode())
							{
								g_ClearGenerateEStopError();
							}
							// ----------------------------------------------
						}
						if(event & SLOMO_EVENT)
						{
							ErlaubeTastenBedinung = 1;
							#ifdef _CF_DEBUG_PRINT
							if(g_TestFlag & TF_DBG_HW){dbPrint("SLOMO Event\r");}
							#endif
						}
					}
				}
				else
				{
					if(!ErlaubeTastenBedinung && !isHostMode())
					{
						if(event & ESTOP_RESET_EVENT)
						{
							YellowOff();g_ClearEstopFlag();g_ClearGenerateEStopError();
						}
					}
				}
				//-----------------------------------------------------------------------------------------
			}// end..if(!isNRTL_LP())
			else
			{
				if(event & ESTOP_RESET_EVENT)
				{
					//dbPrint("RR\r");
					ErlaubeTastenBedinung = 0;
					#ifdef _CF_DEBUG_PRINT
					if(g_TestFlag & TF_DBG_HW){dbPrint("Reset ESTOP Event\r");}
					#endif
					//DbgStep = 7;
					EstopOffen = 0;
					if(!isHostMode())
					{
						#ifdef _CF_485_DEBUG_PRINT
						if(g_TestFlag & TF_DBG_485){dbPrint("FET1");}
						#endif
						//DbgStep = 8;
						if(CheckMOSFETSwitch() != errNone)
						{
							//DbgStep = 9;
							SetInitError(istate_MotorFETEstopL);SetDeviceInitError();
							EnableBlinkError = 1;BlinkErrorZeit = 0;BE_TogglePause = 0;
							//DbgStep = 10;
						}
						//DbgStep = 50;
					}
					//DbgStep = 11;
					// ----------------------------------------------
					YellowOff();
					//DbgStep = 12;
					keyState = PressKEY_RESET;
					//SetDriveMode(DRV_MODE_NORMAL);
					if(isHostMode() && !isDeviceInitError())
					{
						GreenSlowBlink();
					}
					else{GreenOn();}
					//DbgStep = 12;
					g_ClearEstopFlag();
					if(!isHostMode())
					{
						g_ClearGenerateEStopError();
					}
					//DbgStep = 13;
					//EnableIntrEstopEvent = 1;
					// ----------------------------------------------
				}
				//-----------------------------------------------------------------------------------------
			}//end ...else

		}
if(ErlaubeTastenBedinung || !isHostMode())
{
		if((event & TASTEN_EVENT) && (g_DeviceConfig.ExternEstopInEnable ||	g_DeviceConfig.InternEstopEnable))
		{
			//DbgStep = 14;
			if(TA_SERVICE || (TA2_SERVICE && gDoppelTasteUI))// key high active
			{	// Start
				//DbgStep = 16;
				switch(keyState)
				{
					case PressKEY_RESET			:
												//DbgStep = 15;
												if(ErlaubeTastenBedinung)
												{
													YellowOn();
												}
												ZeitMessung = 0;
												//if(GetDriveMode() != DRV_MODE_ESTOP)
												/*if(!g_isEstopFlag())
												{*/
													EnableZeitmessung = 1;
												//}
												keyState = NoPressKEY_START;
												if(EstopOffen && isNRTL_LP())
												{
													keyState = PressKEY_RESET;
													EnableZeitmessung = 0;
												}
												#ifdef _CF_DEBUG_PRINT
												if(g_TestFlag & TF_DBG_HW){dbPrint("Yellow_4\r");}
												#endif
												break;
					case PressZ_MOVE_WAIT_START	:	// Z Handsteuerung starten
												#ifdef _CF_DEBUG_PRINT
												if(g_TestFlag & TF_DBG_HW){dbPrint("Press PressZ_MOVE_WAIT_START\r");}
												#endif
												keyState = NoPressZ_MOVE_START;
												//YellowOff();
												//if(Z_MoveStart() != errNone){RedOn();}
												error = Z_MoveStart();
												if(error != errNone)
												{
													if(error == (unsigned short)INFO)
													{
														GreenOn();
														if(!isDeviceInitError()) RestoreGreen(Save2);
														keyState = PressKEY_RESET;
													}
													else {RedOn();}
												}
												break;
					default:					break;

				}

			}//end...if(TA_SERVICE)
			else// Taste losgelassen
			{
				//DbgStep = 17;
				switch(keyState)
				{
					case NoPressKEY_START		:	// Zeitmessung, nach Taste betätigen, 2 sec Zeit für 3 sec druck 
												if(ZeitMessung >TIMESEC_3)
												{
													#ifdef _CF_DEBUG_PRINT
													if(g_TestFlag & TF_DBG_HW){dbPrint("Frei A NoPressKEY_START\r");}
													#endif
													keyState = PressZ_MOVE_WAIT_START;
													Save2 = m_GreenBlink;
													GreenSuperFastBlink();

												}
												else
												{
													keyState = PressKEY_RESET;
												}
												if(ErlaubeTastenBedinung){ZeitMessung = 0;EnableZeitmessung = 0;}
												break;
					case NoPressZ_MOVE_START	:	// Z Handsteuerung stoppen
													if(Z_MoveStop() != errNone){RedOn();}
													/*#ifndef _CF_NRTL
													YellowOn();
													#endif*/
													if(!isDeviceInitError()) RestoreGreen(Save2);
													keyState = PressKEY_RESET;
													#ifdef _CF_DEBUG_PRINT
													//if(g_TestFlag & TF_DBG_HW){dbPrint("Yellow_7\r");}
													if(g_TestFlag & TF_DBG_HW){dbPrint("Frei NoPressZ_MOVE_START\r");}
													#endif

												break;
					default:					break;
				}

			}//end...else, if(TA_SERVICE)

		}//end... if(event & TASTEN_EVENT)
}//end if(ErlaubeTastenBedinung)
		if((keyState == NoPressKEY_START) && (ZeitMessung >= TIMESEC_3) && (TA_SERVICE || (TA2_SERVICE && gDoppelTasteUI)))
		{
			//xxx YellowOff();
			Save2 = m_GreenBlink;
			//DbgStep = 18;
			GreenSuperFastBlink();
			do
			{
				WDO();
				K_Task_Wait(20);
			}while(TA_SERVICE || (TA2_SERVICE && gDoppelTasteUI));
			keyState = PressZ_MOVE_WAIT_START;
			ZeitMessung = 0;EnableZeitmessung = 0;
			//xxx YellowOn();
	
			#ifdef _CF_DEBUG_PRINT
			if(g_TestFlag & TF_DBG_HW){dbPrint("Yellow_9\r");}
			#endif
		}
		if(m_ZHandMoveEnable && m_ZDirToHome)
		{
			if(Z_MoveToEndPos() != errNone){RedOn();}
		}

	}//end...while(1);
}

/*void SetDriveMode(unsigned short mode)
{
	ESTOP_ALLOC
	m_DriveMode = mode;
	ESTOP_RELEASE
}*/

/*unsigned int GetDriveMode(void)
{unsigned short help;
	ESTOP_ALLOC
	help = m_DriveMode;
	ESTOP_RELEASE
	return help;
}*/


void SaveZAxisError(void)
{
	unsigned short i;
	//m_Save_ErrorAxisHW = g_GetAxes_ErrorAxisHW(ZAXES_ID);
	//m_Save_ErrorAxisHWStall = g_GetAxes_ErrorAxisHWStall(ZAXES_ID);
	for(i = 0; i < MAX_AXES;i++)
	{
		if(g_FelixAxes[i].Enable && g_FelixAxes[i].AxisWithBrake)
		{
			m_Save_ErrorAxisHW[i] = g_GetAxes_ErrorAxisHW(i);
			m_Save_ErrorAxisHWStall[i] = g_GetAxes_ErrorAxisHWStall(i);
		}
	}
}
void RestoreZAxisError(void)
{
	unsigned short i;
	//g_SetAxes_ErrorAxisHW(ZAXES_ID, m_Save_ErrorAxisHW);
	//g_SetAxes_ErrorAxisHWStall(ZAXES_ID, m_Save_ErrorAxisHWStall);
	for(i = 0; i < MAX_AXES;i++)
	{
		if(g_FelixAxes[i].Enable && g_FelixAxes[i].AxisWithBrake)
		{
			g_SetAxes_ErrorAxisHW(i, m_Save_ErrorAxisHW[i]);
			g_SetAxes_ErrorAxisHWStall(i, m_Save_ErrorAxisHWStall[i]);
		}
	}
}

unsigned short Z_MoveStart(void)
{
	unsigned short error;
//	unsigned short ID;
//	unsigned short idx;
	unsigned short i;
	long SlewValue = 0;
	unsigned short isHomeSwitchCnt = 0;
	unsigned short cntGetHomeSwitch = 0;
	unsigned short isHomeSwitch[MAX_AXES];
	short MoveSlewDirToHomeSwitch;
	#ifdef _CF_DEBUG_PRINT
	if(g_TestFlag & TF_DBG_HW_EVENT){dbPrint("flg Z Move ein\r");}
	#endif
	SaveZAxisError();

//	ID = Get_AxisID(eZ_Axis);
//	idx = Get_IdxFor_pIMSPara(eZ_Axis);
	#ifdef _CF_FELIX
	MoveSlewDirToHomeSwitch = FixZDrive.MoveSlewDirToHomeSwitch;
	#endif
	#ifdef _CF_PENTA_DISK
	MoveSlewDirToHomeSwitch = FixZ_E1_Drive.MoveSlewDirToHomeSwitch;
	#endif
	for(i = 0; i < MAX_AXES;i++)
	{
		isHomeSwitch[i] = 1;
		if(g_FelixAxes[i].Enable && g_FelixAxes[i].AxisWithBrake)
		{
			if((error = ImsCmd_MotorResetError(g_FelixAxes[i].AxisID)) != errNone) {RestoreZAxisError();return error;}
			if((error = ImsCmd_GetHomeSwitch(g_FelixAxes[i].AxisID, &isHomeSwitch[i])) != errNone) {RestoreZAxisError();return error;}
			cntGetHomeSwitch++;
			if(isHomeSwitch[i] == 1) {isHomeSwitchCnt++;}
		}
	}
	if(gDoppelTasteUI)
	{
		if(TA_SERVICE)
		{
			SlewValue = Z_SLEW * (long)MoveSlewDirToHomeSwitch * (long)-1;
			m_ZDirToHome = 0;
		}
		else
		{
			if(TA2_SERVICE)
			{
				if(isHomeSwitchCnt == cntGetHomeSwitch)
				{
					//return errNone;
					return (unsigned short)INFO;//kein Fehler aber es geht nicht weiter
				}
				SlewValue = Z_SLEW * (long)MoveSlewDirToHomeSwitch * (long)1;
				m_ZDirToHome = 1;
			}
		}
	}
	else
	{
		if(isHomeSwitchCnt == cntGetHomeSwitch)
		{
			SlewValue = Z_SLEW * (long)MoveSlewDirToHomeSwitch * (long)-1;
			m_ZDirToHome = 0;
		}
		else
		{
			SlewValue = Z_SLEW * (long)MoveSlewDirToHomeSwitch * (long)1;
			m_ZDirToHome = 1;
		}
	}
	for(i = 0; i < MAX_AXES;i++)
	{
		if(g_FelixAxes[i].Enable && g_FelixAxes[i].AxisWithBrake)
		{
			if((error = ImsCmd_SetHoldCurrent(g_FelixAxes[i].AxisID, pIMSChangedPara[i]->usHoldCurrent)) != errNone) {Z_ErrorStop();RestoreZAxisError();return error;}
		}
	}
	if((error = ImsCmd_SlewInitAll(eZ_Ex_Axis)) != errNone) {Z_ErrorStop();RestoreZAxisError();return error;}
	for(i = 0; i < MAX_AXES;i++)
	{
		if(g_FelixAxes[i].Enable && g_FelixAxes[i].AxisWithBrake)
		{
			if(m_ZDirToHome)
			{
				//STROM REDUZIERT Hochfahren
				if((error = ImsCmd_SetRunCurrent(g_FelixAxes[i].AxisID, 55)) != errNone) {Z_ErrorStop();RestoreZAxisError();return error;}
				if((error = ImsCmd_SetHoldCurrent(g_FelixAxes[i].AxisID, pIMSChangedPara[i]->usHoldCurrent)) != errNone) {Z_ErrorStop();RestoreZAxisError();return error;}
			}
			else
			{
				//STROM REDUZIERT  Runterfahren
				//if((error = ImsCmd_SetRunCurrent(ID, pIMSChangedPara[AXIS_ID_Z_DRIVE-1]->usRunCurrent)) != errNone) {Z_ErrorStop(ID);RestoreZAxisError();return error;}
				if((error = ImsCmd_SetRunCurrent(g_FelixAxes[i].AxisID, 20)) != errNone) {Z_ErrorStop();RestoreZAxisError();return error;}
				if((error = ImsCmd_SetHoldCurrent(g_FelixAxes[i].AxisID, pIMSChangedPara[i]->usHoldCurrent)) != errNone) {Z_ErrorStop();RestoreZAxisError();return error;}
			}
		}
	}
	for(i = 0; i < MAX_AXES;i++)
	{
		m_ZHandMoveToHome[i] = 0;
		if(g_FelixAxes[i].Enable && g_FelixAxes[i].AxisWithBrake)
		{
			if((m_ZDirToHome && (isHomeSwitch[i] == 0)) || !m_ZDirToHome)
			{	
				//#ifdef _CF_NRTL
				if(isNRTL_LP())
				{
					if(isFlagFwEStopLock())
					{
						#ifdef _CF_DEBUG_PRINT
						if(g_TestFlag & TF_DBG_FETRESET){dbPrint("RFL4");}
						#endif
						ResetFwEStopLock();
					}
				}
				//#endif
				if((error = ImsCmd_Hippie_Safety_ManuallyMode()) != errNone) {Z_ErrorStop();RestoreZAxisError();return error;}
				if((error = ImsCmd_SlewMoveID(g_FelixAxes[i].AxisID, SlewValue)) != errNone) {Z_ErrorStop();RestoreZAxisError();return error;}
				if(m_ZDirToHome){m_ZHandMoveToHome[i] = 1;}
			}
		}
	}

	m_ZHandMoveEnable = 1;
	SetFreeHandZMoveMode();
	return error;
}


unsigned short Z_MoveStop(void)
{
	unsigned short error;
	unsigned short i;
	#ifdef _CF_DEBUG_PRINT
	if(g_TestFlag & TF_DBG_HW_EVENT){dbPrint("flg Z Move aus\r");}
	#endif
	error = ImsCmd_SlewExitAll(eZ_Ex_Axis);
	m_ZHandMoveEnable = 0;
	ClearFreeHandZMoveMode();
	//if((error = ImsCmd_SlewExitAll(eZ_Ex_Axis)) != errNone)
	if(error != errNone)
	{
		Z_ErrorStop();// war kommentar
		RestoreZAxisError();
		//STROM NORMAL
		for(i = 0; i < MAX_AXES;i++)
		{
			if(g_FelixAxes[i].Enable && g_FelixAxes[i].AxisWithBrake)
			{
				ImsCmd_SetRunCurrent(g_FelixAxes[i].AxisID, pIMSChangedPara[i]->usRunCurrent);
			}
		}
		return error;
	}
	//STROM NORMAL
	for(i = 0; i < MAX_AXES;i++)
	{
		if(g_FelixAxes[i].Enable && g_FelixAxes[i].AxisWithBrake)
		{
			if((error = ImsCmd_SetRunCurrent(g_FelixAxes[i].AxisID, pIMSChangedPara[i]->usRunCurrent)) != errNone) {Z_ErrorStop();RestoreZAxisError();}
			if((error = ImsCmd_SetHoldCurrent(g_FelixAxes[i].AxisID, pIMSChangedPara[i]->usHoldCurrent)) != errNone) {Z_ErrorStop();RestoreZAxisError();return error;}
		}
	}
	return error;
	//KeyStateReset();
}
unsigned short Z_MoveToEndPos(void)
{
	unsigned short error = errNone;
	unsigned short i;
	unsigned short HomeSwitch = 1;
	long SlewValue;
	unsigned short HomeSwitchCount;
	unsigned short isHomeSwitch[MAX_AXES];
	unsigned short isHomeSwitchCnt = 0;
	unsigned short cntGetHomeSwitch = 0;
	short MoveSlewDirToHomeSwitch;

	#ifdef _CF_FELIX
	MoveSlewDirToHomeSwitch = FixZDrive.MoveSlewDirToHomeSwitch;
	#endif
	#ifdef _CF_PENTA_DISK
	MoveSlewDirToHomeSwitch = FixZ_E1_Drive.MoveSlewDirToHomeSwitch;
	#endif
	for(i = 0; i < MAX_AXES;i++)
	{
		if(g_FelixAxes[i].Enable && g_FelixAxes[i].AxisWithBrake && m_ZHandMoveToHome[i] && m_ZDirToHome)
		{
			if((error = ImsCmd_MotorResetError(g_FelixAxes[i].AxisID)) != errNone) {Z_ErrorStop();RestoreZAxisError();return error;}
			if((error = ImsCmd_GetHomeSwitch(g_FelixAxes[i].AxisID, &isHomeSwitch[i])) != errNone) {Z_ErrorStop();RestoreZAxisError();return error;}
			//if(isHomeSwitch[i])
			cntGetHomeSwitch++;
			if(isHomeSwitch[i] == 1) {isHomeSwitchCnt++;}
			if(isHomeSwitch[i])
			{
				if((error = ImsCmd_SlewMoveID(g_FelixAxes[i].AxisID, 0)) != errNone){Z_ErrorStop();RestoreZAxisError();return error;}
				m_ZHandMoveToHome[i] = 0;
			}
		}
	}
	if(gDoppelTasteUI)
	{
		HomeSwitchCount  = 0;
		for(i = 0; i < MAX_AXES;i++)
		{
			if(g_FelixAxes[i].Enable && g_FelixAxes[i].AxisWithBrake)
			{
				if(m_ZHandMoveToHome[i])
				{
					HomeSwitchCount++;
				}
			}
		}
		if(!HomeSwitchCount)
		{
			//#ifndef _CF_NRTL
			if(!isNRTL_LP())
			{
				return Z_MoveStop();
			}
			//#else
			else
			{
				GreenOn();
				error = Z_MoveStop();
				//if(!isDeviceInitError()) RestoreGreen(Save2);
				keyState = PressKEY_RESET;
			}
			//#endif
		}
		return errNone;
	}
	else
	{
		if(isHomeSwitchCnt == cntGetHomeSwitch)
		{
			SlewValue = Z_SLEW * (long)MoveSlewDirToHomeSwitch * (long)-1;
			m_ZDirToHome = 0;
//STROM REDUZIERT runterfahren
		//if((error = ImsCmd_SetRunCurrent(ID, 20)) != errNone) {Z_ErrorStop(ID);RestoreZAxisError();return error;}
			for(i = 0; i < MAX_AXES;i++)
			{
				if(g_FelixAxes[i].Enable && g_FelixAxes[i].AxisWithBrake)
				{
					//#ifdef _CF_NRTL
					if(isNRTL_LP())
					{
						if(isFlagFwEStopLock())
						{
							#ifdef _CF_DEBUG_PRINT
							if(g_TestFlag & TF_DBG_FETRESET){dbPrint("RFL5");}
							#endif
							ResetFwEStopLock();
						}
					}
					//#endif
					if((error = ImsCmd_Hippie_Safety_ManuallyMode()) != errNone) {Z_ErrorStop();RestoreZAxisError();return error;}
					if((error = ImsCmd_SlewMoveID(g_FelixAxes[i].AxisID, SlewValue)) != errNone){Z_ErrorStop();RestoreZAxisError();return error;}
				}
			}
			K_Task_Wait(100);
			for(i = 0; i < MAX_AXES;i++)
			{
				if(g_FelixAxes[i].Enable && g_FelixAxes[i].AxisWithBrake)
				{
					if((error = ImsCmd_SetRunCurrent(g_FelixAxes[i].AxisID, 20)) != errNone) {Z_ErrorStop();RestoreZAxisError();return error;}
					if((error = ImsCmd_SetHoldCurrent(g_FelixAxes[i].AxisID, pIMSChangedPara[i]->usRunCurrent)) != errNone) {Z_ErrorStop();RestoreZAxisError();return error;}
				}
			}

		}
	}
	return error;	
}
void Z_ErrorStop(void)
{
	unsigned short i;
	m_ZHandMoveEnable = 0;
	for(i = 0; i < MAX_AXES;i++)
	{
		if(g_FelixAxes[i].Enable && g_FelixAxes[i].AxisWithBrake)
		{
			ImsCmd_SlewMoveID(g_FelixAxes[i].AxisID, 0);
		}
	}
	ImsCmd_SetBreake();
	K_Task_Wait(10);
	for(i = 0; i < MAX_AXES;i++)
	{
		m_ZHandMoveToHome[i] = 0;
		if(g_FelixAxes[i].Enable && g_FelixAxes[i].AxisWithBrake)
		{
			ImsCmd_SetHoldCurrent(g_FelixAxes[i].AxisID, 0);
			ImsCmd_MotorResetError(g_FelixAxes[i].AxisID);
			//STROM NORMAL
			ImsCmd_SetRunCurrent(g_FelixAxes[i].AxisID, pIMSChangedPara[i]->usRunCurrent);
		}
	}
}

void RestoreGreen(unsigned short Blink)
{
	switch(Blink)
	{
		case NoBlinkOn	:	GreenOn();
							break;
		case SlowBlink	:	GreenSlowBlink();//GreenOn();
							break;
		case FastBlink	:	GreenFastBlink();
							break;
		case SuperFastBlink	:
							GreenSuperFastBlink();
							break;
		default			:	GreenOff();
							break;
	}
}

void GreenPrepareTimer(unsigned short Time)
{
	unsigned short help;
	help = 0xFFFF - (unsigned short)(Time / TIMERBASE);
	CC2_vLoadTmr(CC2_TIMER_7, help);
	CC2_vSetReload(CC2_TIMER_7, help);
	CC2_vSetCCxReg_CC2_CC_21_GreenLED(help);
}
void GreenStartTimer(void)
{
	ClearToggle_CC21();
	xHW_LED_Green_On();
	CC2_vStartTmr(CC2_TIMER_7);
}
void GreenClearTimer(void)
{
	//CC2_vClearTmr(CC2_TIMER_7);
	CC2_vStopTmr(CC2_TIMER_7);
}

void YellowPrepareTimer(unsigned short Time)
{
	unsigned short help;
	help = 0xFFFF - (unsigned short)(Time / TIMERBASE);
	CC2_vLoadTmr(CC2_TIMER_8, help);
	CC2_vSetReload(CC2_TIMER_8, help);
	CC2_vSetCCxReg_CC2_CC_22_YellowLED(help);
}

void YellowStartTimer(void)
{
	ClearToggle_CC22();
	xHW_LED_Yellow_On();
	CC2_vStartTmr(CC2_TIMER_8);
}

void YellowClearTimer(void)
{
//	CC2_vClearTmr(CC2_TIMER_8);
	CC2_vStopTmr(CC2_TIMER_8);
}

void YellowSlowBlink(void)
{
	YellowClearTimer();
	xHW_LED_Yellow_Off();
	YellowPrepareTimer(SLOW_BLINK_TIME);
	if(m_GreenBlink == SlowBlink)
	{
		GreenClearTimer();
		K_Task_Wait(2);
		GreenPrepareTimer(SLOW_BLINK_TIME);
		GreenStartTimer();
	}
	YellowStartTimer();
	m_YellowBlink = SlowBlink;
}
void GreenSlowBlink(void)
{

	GreenClearTimer();
	GreenPrepareTimer(SLOW_BLINK_TIME);
	if(m_YellowBlink == SlowBlink)
	{
		YellowClearTimer();
		K_Task_Wait(2);
		//xHW_LED_Yellow_Off();
		YellowPrepareTimer(SLOW_BLINK_TIME);
		YellowStartTimer();
	}
	GreenStartTimer();
	m_GreenBlink = SlowBlink;
}

void YellowFastBlink(void)
{
	YellowClearTimer();
	//xHW_LED_Yellow_Off();
	YellowPrepareTimer(FAST_BLINK_TIME);
	if(m_GreenBlink == FastBlink)
	{
		GreenClearTimer();
		K_Task_Wait(2);
		GreenPrepareTimer(FAST_BLINK_TIME);
		GreenStartTimer();
	}
	YellowStartTimer();
	m_YellowBlink = FastBlink;
}
void GreenFastBlink(void)
{
	GreenClearTimer();
	GreenPrepareTimer(FAST_BLINK_TIME);
	if(m_YellowBlink == FastBlink)
	{
		YellowClearTimer();
		K_Task_Wait(2);
		//xHW_LED_Yellow_Off();
		YellowPrepareTimer(FAST_BLINK_TIME);
		YellowStartTimer();
	}
	GreenStartTimer();
	m_GreenBlink = FastBlink;
}

void YellowSuperFastBlink(void)
{
	YellowClearTimer();
	//xHW_LED_Yellow_Off();
	YellowPrepareTimer(SUPERFAST_BLINK_TIME);
	if(m_GreenBlink == SuperFastBlink)
	{
		GreenClearTimer();
		K_Task_Wait(2);
		GreenPrepareTimer(SUPERFAST_BLINK_TIME);
		GreenStartTimer();
	}
	YellowStartTimer();
	m_YellowBlink = SuperFastBlink;

}
void GreenSuperFastBlink(void)
{
	GreenClearTimer();
	GreenPrepareTimer(SUPERFAST_BLINK_TIME);
	if(m_YellowBlink == SuperFastBlink)
	{
		YellowClearTimer();
		K_Task_Wait(2);
		//xHW_LED_Yellow_Off();
		YellowPrepareTimer(SUPERFAST_BLINK_TIME);
		YellowStartTimer();
	}
	GreenStartTimer();
	m_GreenBlink = SuperFastBlink;


}

void YellowOff(void)
{
	//CC2_vSetCCxReg_CC2_CC_22_YellowLED(0x0000);
	YellowClearTimer();
	xHW_LED_Yellow_Off();
	m_YellowBlink = NoBlinkOff;
}
void YellowOn(void)
{
	YellowClearTimer();
	xHW_LED_Yellow_On();
	m_YellowBlink = NoBlinkOn;
}
void GreenOn(void)
{
	GreenClearTimer();
	m_GreenBlink = NoBlinkOn;
	xHW_LED_Green_On();
}
void GreenOff(void)
{
	GreenClearTimer();
	m_GreenBlink = NoBlinkOff;
	xHW_LED_RedGreen_Off();
}

void RedOn(void)
{
	GreenClearTimer();
	m_GreenBlink = NoBlinkOn;
	xHW_LED_Red_On();
}

//#ifdef _CF_NRTL
unsigned short CheckMOSFETSwitch(void)
{
	unsigned short error;
	unsigned short i;

	for(i = 0; i < 6;i++)
	{
		error = CheckMOSFETSwitchLoop();
		if(error == errNone)
		{
			break;
		}
	}
	return error;
}
unsigned short CheckMOSFETSwitchLoop(void)
{
	unsigned short error = errNone;
	unsigned short i, a;//, Count;
	//SetFwEStopLock();
	a = 0;
	//DbgStep = 30;
//	K_Task_Wait(20);//20
	//DbgStep = 31;
	if(isFlagFwEStopLock())
	{
		//DbgStep = 32;
		for(i = 0; i < 6;i++)
		{
			if((UEW_FET1_24V || UEW_FET2_24V || UEW_FET3_48V || UEW_FET4_48V) && !ESTOP)
			//if(UEW_FET1_24V || UEW_FET2_24V || UEW_FET3_48V || UEW_FET4_48V)
			{
				a++;
				//DbgStep = 33;
				if(a == 5)
				{
					//DbgStep = 34;
					error = ERR_SAFETY_MONITORING;

					//SetErrorSafetyMonitoring();
				}
			}
			//DbgStep = 35;
			K_Task_Wait(10);//2);
			//DbgStep = 36;
			/*if(isSetBreakCheckMOSFET())
			{
				if(ESTOP) {ResetFwEStopLock();}
				ClearBreakCheckMOSFET();
				return errNone;
			}*/
		}
		//DbgStep = 37;
	}
	//DbgStep = 38;
	ResetFwEStopLock();
	WDO();
	//DbgStep = 39;
	K_Task_Wait(25);//25);
	//DbgStep = 40;
	a = 0;
	for(i = 0; i < 6;i++)
	{
		if((!UEW_FET1_24V || !UEW_FET2_24V || !UEW_FET3_48V || !UEW_FET4_48V) && ESTOP)
		{
			a++;
			//DbgStep = 41;
			if(a == 5)
			{
				//DbgStep = 42;
				error = ERR_SAFETY_MONITORING;
				//SetErrorSafetyMonitoring();
			}
		}
		//DbgStep = 43;
		K_Task_Wait(10);//2);
		//DbgStep = 44;
		/*if(isSetBreakCheckMOSFET())
		{
			ResetFwEStopLock();
			ClearBreakCheckMOSFET();
			return errNone;
		}*/

	}
	//DbgStep = 45;
	return error;
}

void SetInitError(unsigned short ErrorState)
{
	unsigned short i;
	for(i = 0; i < (MAX_AXES + MAX_CNT_ISTATE); i++)
	{
		WDO();
		if(g_errArray[i] != errNone)
		{
			if(g_errArray[i] == ErrorState)
			{
				return;
			}
		}
		else
		{
			g_errArray[i] = ErrorState;
			return;
		}
	}
	//nichts mehr frei im array
	g_errArray[0] = ErrorState;
}
//#endif

