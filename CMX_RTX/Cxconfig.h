/*********************************************************************
 The following will be included when the cmx_init.c program gets
 compiled. These are set the maximums for a variety of things. This way
 CMX can pre-allocate the needed memory to accomplish the user
 needs. This is so no memory allocation functions are needed by
 CMX, and no delays are introduced to obtain the needed memory during
 run time.

 The user is free to make a copy of this file for each application
 program, so each application program will have their own "cxconfig.h"
 file. Also the user should probally copy the cmx_init.c file to a "user
 name file", for each application program. This way the user does not
 have to change "this" file each time a different application program
 is worked with. Just remember to make sure you change the 
 #include <cxconfig.h> line in the user named ("cmx_init.c") file
 to include the proper user named "cxconfig.h" file.

 Also if any of these defines ARE changed, then the user must be
 sure to compile the cmx_init.c file (or equalivent user file)
 so these NEW values take effect.
*********************************************************************/
#ifndef __CXCONFIG_H__
#define __CXCONFIG_H__

//****************************************************************************
// @Project Includes
//****************************************************************************
#include "cxfuncs.h"

#ifdef __cplusplus
//! export for (M$) C++
extern "C" {
#endif

//****************************************************************************
// @Typedefs
//****************************************************************************

// priority of Tasks
enum TASK_RPRIORITIES
{
	 PRI_USER_MAIN = 1
	 ,PRI_MOTOR
	 ,PRI_ASYNCCTRL
	 ,PRI_HARDWARE
	 ,PRI_SLAVE
//	,PRI_STEPPER_MOTOR
//	,PRI_KEYMONITOR
//	,PRI_HANDLING
//	,PRI_HARDWARE
//	,PRI_LED_DRIVER1
//	,PRI_LED_DRIVER2
//	,PRI_MANUAL_CONTROL
//	,PRI_KEY_CONTROL
//	,PRI_SAFETY
	,PRI_END
};
// semaphores
/*
enum CMX_SEMAPHORES
{
	 SEM_UART0_OUT = 0		// COM 0 Output
	,SEM_UART1_OUT			// COM 1 Output
	,SEM_UART2_OUT			// COM 2 Output
	,SEM_UART3_OUT			// COM 3 Output
	,SEM_UART4_OUT			// COM 4 Output
	,SEM_UART5_OUT			// COM 5 Output
	,SEM_COUNT_END
};
*/
// mailboxes
enum CMX_MBX
{
//	 MBX_Z_AXIS = 0
	MBX_MOTOR = 1
	,MBX_ASYNCCTRL
	,MBX_ASYNCMOTOR
	,MBX_HARDWARE
	,MBX_SLAVE
};

// queues

enum CMX_QUEUS
{
	QUEUS_COUNT_END
};

// events
enum CMX_EVENTS
{
	RESET_EVENTS			= 0x0000
	,EVENT_MOTOR_MBX		= 0x0001
	,EVENT_USER_BREAK		= 0x0002
	,ESTOP_EVENT			= 0x0004
	,ESTOP_RESET_EVENT		= 0x0008
	,SLOMO_EVENT			= 0x0010
	,SLOMO_RESET_EVENT		= 0x0020
	,TASTEN_EVENT			= 0x0040

	,EVENT_MASTER_COM  		= 0x0080
	,EVENT_SLAVE_COM  		= 0x0100
	,EVENT_BARC_COM  		= 0x0200

	,EVENT_ASYNC_MOTOR_MBX	= 0x0400
	,EVENT_ASYNCCTRL_MBX	= 0x0800
	,EVENT_HARDWARE_MBX		= 0x1000
	,EVENT_SLAVE_MBX		= 0x2000

//	,EVENT_TERM_MA	  	= 0x0008
//	,EVENT_SYNC_MA	  	= 0x0010
//	,EVENT_OVERFLOW_MA	= 0x0020

//	,EVENT_SYNC_SL	  	= 0x0080

//	,EVENT_OVERFLOW_SL	= 0x0100
	//,TA_SERVICE_EVENT	= 0x0400// wird nicht verwendet 
//	,TASTEN_EVENT = 0x8000

//	,ABORT_KEY_EVENT	= 0x0040
//	,BROADCAST_EVENT	= 0x0080
//	,EVENT_DISPLAY_MBX				= 0x0004
//	,EVENT_KEY_RUN_PIPET	   		= 0x0008
//	,EVENT_KEY_RUN_PIPET_MOVE  		= 0x0010
//	,STOP_EVENTS			= EVENT_USER_BREAK | ESTOP_EVENT
//	,COM_MASTER_EVENTS	= EVENT_TERM_MA | EVENT_SYNC_MA | EVENT_OVERFLOW_MA
//	,COM_SLAVE_EVENTS	= EVENT_TERM_SL | EVENT_SYNC_SL | EVENT_OVERFLOW_SL
//	,COM_SLAVE_EVENTS	= EVENT_TERM_SL
};


// ressources
enum CMX_RESOURCES
{
	 PRINT_RESOURCE = 0
//	,SCAN_RESOURCE
	,PORT_RESOURCE
//	,SLOMO_RESOURCE
	,ESTOP_RESOURCE
//	,ESTOP_STOP_RESOURCE
//	,ESTOP_INTR_RESOURCE
	,GLOBAL_AXES_DATA_RESOURCE
	,VARIABLEN_SETGET_RESOURCE
	,FRAM_RESOURCE
	,ESTOP_VARIABLEN_SETGET_RESOURCE
	,MOTOR_MBX_RESOURCE
//	,DEVICE_RESOURCE
	,malloc_RESOURCE
//	,FILE_RESOURCE
//	,STANDBY_RESOURCE
//	,STEPPERMOTOR_RESOURCE
//	,ZAXISCHECK_RESOURCE
//	,fmalloc_RESOURCE
//	,C_MAX_RESOURCES
//	,RESOURCE_COUNT_END
};

// timers
enum CMX_TIMERS
{
	TIMER_COUNT_END
//	,C_MAX_CYCLIC_TIMERS
};
/*
// modi
enum CMX_TIMERS_MODI : byte
{
	doJob_TIMER_MODUS = 0		// acts on the specified task
};
*/

//****************************************************************************
// @Macros
//****************************************************************************
/*
// Macros for using Port-CANopen on CMX
#define CO_NEW_RX_MSG()			K_Intrp_Task_Start(CANopenSlot);	// trigger task
#define CO_COM_PART_ALLOC()		K_Resource_Wait(CO_COM_RESOURCE, 0);
#define CO_COM_PART_RELEASE()	K_Resource_Release(CO_COM_RESOURCE);
#define CO_APPL_PART_ALLOC()	K_Resource_Wait(CO_APPL_RESOURCE, 0);
#define CO_APPL_PART_RELEASE()	K_Resource_Release(CO_APPL_RESOURCE);
#define CO_WATCH_DOG			K_Task_Coop_Sched();
*/
#define __CXDEFINE_H_

#define TASK_COOP_SCHED()		K_Task_Coop_Sched();

#define SAVEMALLOC_ALLOC		K_Resource_Wait(malloc_RESOURCE, 0);
#define SAVEMALLOC_RELEASE	K_Resource_Release(malloc_RESOURCE);

//#define SAVE_FMALLOC_ALLOC()	K_Resource_Wait(fmalloc_RESOURCE, 0);
//#define SAVE_FMALLOC_RELEASE()	K_Resource_Release(fmalloc_RESOURCE);

#define PRINT_ALLOC				K_Resource_Wait(PRINT_RESOURCE, 0);
#define PRINT_RELEASE			K_Resource_Release(PRINT_RESOURCE);

//#define SCAN_ALLOC				K_Resource_Wait(SCAN_RESOURCE, 0);
//#define SCAN_RELEASE			K_Resource_Release(SCAN_RESOURCE);
//FELIX
#define	PORT_ALLOC				K_Resource_Wait(PORT_RESOURCE, 0);
#define	PORT_RELEASE			K_Resource_Release(PORT_RESOURCE);

//#define SLOMO_ALLOC				K_Resource_Wait(SLOMO_RESOURCE, 0);
//#define SLOMO_RELEASE			K_Resource_Release(SLOMO_RESOURCE);
#define ESTOP_ALLOC				K_Resource_Wait(ESTOP_RESOURCE, 0);
#define ESTOP_RELEASE			K_Resource_Release(ESTOP_RESOURCE);

//#define ESTOP_STOP_ALLOC			K_Resource_Wait(ESTOP_STOP_RESOURCE, 0);
//#define ESTOP_STOP_RELEASE			K_Resource_Release(ESTOP_STOP_RESOURCE);

//#define ESTOP_INTR_ALLOC			K_Resource_Wait(ESTOP_INTR_RESOURCE, 0);
//#define ESTOP_INTR_RELEASE			K_Resource_Release(ESTOP_INTR_RESOURCE);

#define GLOBAL_AXES_DATA_ALLOC		K_Resource_Wait(GLOBAL_AXES_DATA_RESOURCE, 0);
#define GLOBAL_AXES_DATA_RELEASE 	K_Resource_Release(GLOBAL_AXES_DATA_RESOURCE);

#define VARIABLEN_SETGET_ALLOC		K_Resource_Wait(VARIABLEN_SETGET_RESOURCE, 0);
#define VARIABLEN_SETGET_RELEASE 	K_Resource_Release(VARIABLEN_SETGET_RESOURCE);

#define	FRAM_ALLOC				K_Resource_Wait(FRAM_RESOURCE, 0);
#define	FRAM_RELEASE			K_Resource_Release(FRAM_RESOURCE);

#define ESTOP_VARIABLEN_SETGET_ALLOC		K_Resource_Wait(ESTOP_VARIABLEN_SETGET_RESOURCE, 0);
#define ESTOP_VARIABLEN_SETGET_RELEASE 	K_Resource_Release(ESTOP_VARIABLEN_SETGET_RESOURCE);

#define	MOTOR_MBX_ALLOC				K_Resource_Wait(MOTOR_MBX_RESOURCE, 0);
#define	MOTOR_MBX_RELEASE			K_Resource_Release(MOTOR_MBX_RESOURCE);

//#define DEVICE_LOCK				K_Resource_Get(DEVICE_RESOURCE)
//#define DEVICE_UNLOCK			K_Resource_Release(DEVICE_RESOURCE)

//#define READ_WRITE_COOP_SCHED	K_Task_Coop_Sched();
//#define UART_COOP_SCHED			K_Task_Coop_Sched();

//#define FILE_ALLOC				K_Resource_Wait(FILE_RESOURCE, 0);
//#define FILE_RELEASE			K_Resource_Release(FILE_RESOURCE);

//#define STANDBY_ALLOC			K_Resource_Wait(STANDBY_RESOURCE, 0);
//#define STANDBY_RELEASE			K_Resource_Release(STANDBY_RESOURCE);

//#define STEPPERMOTOR_ALLOC		//K_Resource_Wait(STEPPERMOTOR_RESOURCE, 0);
//#define STEPPERMOTOR_RELEASE	//K_Resource_Release(STEPPERMOTOR_RESOURCE);

//#define ZAXISCHECK_ALLOC			K_Resource_Wait(ZAXISCHECK_RESOURCE, 0);
//#define ZAXISCHECK_RELEASE			K_Resource_Release(ZAXISCHECK_RESOURCE);

//****************************************************************************
// @Defines
//****************************************************************************
//#define NDEBUG				// bei Inbetriebnahme auskommentieren

#define C_MAX_TASKS		 	 7					/* Maximum number of tasks for this program.*/
#define C_MAX_RESOURCES 	9	//RESOURCE_COUNT_END	/* Maximum number of resources.*/
#define C_MAX_CYCLIC_TIMERS	 2/*TIMER_COUNT_END*/	/* Maximum number of cyclic timers.*/
#define C_MAX_MESSAGES		 32					/* Maximum number of messages.*/
#define C_MAX_QUEUES		 1/*QUEUS_COUNT_END*/	/* Maximum number of queues.*/
#define C_MAX_MAILBOXES		 6/*MBX_COUNT_END*/		/* Maximum number of mailboxes.*/
#define C_MAX_SEMAPHORES	 1/*SEM_COUNT_END*/		/* Maximum number of semaphores.*/
#define C_TASK_STK_SIZE		 4096	//4096 /*2048*/	/*1024*/		/* Maximum stack size of ALL tasks stacks*/
															/* added together, does NOT include timer task*/
#define C_INTERRUPT_SIZE	/*256*/ 512	/* Size of interrupt stack. The CMX timer task*/
										/* K_I_Scheduler, and K_I_Intrp_Pipe_Out function use this.*/
#define C_RTC_SCALE			1			/* number of timer interrupt ticks, before CMX */
										/* tick. All time counters based on this.*/
#define C_TSLICE_SCALE	 	4			/* The number of timer interrupt ticks, before*/
										/* forcing task to do time slice task switch.*/

#define C_PIPE_SIZE			255			/* the size of the interrupt pipe. up to 255 big.*/

#define CMXBUG_ENABLE		0		/* ENABLE or DISABLE CMXBug(TM) inclusion.
									Set to 1 to ENABLE, set to 0 (zero) to DISABLE.
									The use still must link in cmxbug.obj, for the 
									CMXBug code to be functional */

#define CMXTRACKER_ENABLE	0 	/* ENABLE or DISABLE CMXTracker(TM) inclusion.
									Set to 1 to ENABLE, set to 0 (zero) to DISABLE */

#define CMXTRACKER_TICK_UPDATE 0//1
									/* Set to 1 to have SYSTEM TICK update
										CMXTracker 'track_tick_count' */

#define CMXTRACKER_SIZE 4096
								 	/* SIZE IN BYTES of the buffer to hold the 
									CMXTracker COMPRESSED information. */

#define CMX_RAM_INIT		1		/* ENABLE or DISABLE the initialization of CMX RAM
									variables. Usually done by Compiler start up code
									in most cases. If not, MUST be ENABLED. Normally
									compiler code will set GLOBAL variables that are
									initialized and set non-initialize variables to
									0 (zero). Set to 1 to ENABLE CMX code to initialize
									it's variables, set to 0 (zero) to DISABLE */


//****************************************************************************
// @Imported Global Variables
//****************************************************************************

//****************************************************************************
// @Global Variables
//****************************************************************************

//****************************************************************************
// @Prototypes Of Global Functions
//****************************************************************************
#ifdef __cplusplus
//! export for (M$) C++
};
#endif


#endif
