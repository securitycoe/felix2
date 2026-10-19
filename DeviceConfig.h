
/*******************************************************************
 *
 *    DESCRIPTION:	Hier sollten alle dateiübergreifende Informationen stehen
 *
 *    AUTHOR:
 *
 *    HISTORY:
 *
 *******************************************************************/

#ifndef _DEVICE_CONFIG_H_
#define _DEVICE_CONFIG_H_

//****************************************************************************
// include files
//****************************************************************************

//****************************************************************************
// @Macros
//****************************************************************************

//****************************************************************************
// @Defines
//****************************************************************************
//#define _CF_MAINPCB_JQV_FW_TYPE_002
//#define _CF_DEBUG_PRINT
//#define _CF_485_DEBUG_PRINT
//#define _CF_485_HOST_OUTPUT_DEBUG_PRINT // SLAVE oder HOST auswählnen
#define _CF_485_SLAVE_OUTPUT_DEBUG_PRINT // SLAVE oder HOST auswählnen
//#define _CF_ESTOP_DEBUG_PRINT
#define _CF_DEBUG_POSERROR

#define _CF_FELIX 1
//#define _CF_PENTA_DISK 2
//#define _CF_NRTL 3

/* Datenprotokoll */
 #define C_SYNC '%'
 #define ACK  '+'
 #define NACK '-'
 #define RDY '!'
 #define ERRORD 'E'
 #define INFO_CHAR 'I'
 #define BREAK '#'
 #define CR 0x0d
// #define EOF 0x00
 #define ESC 0x1b	  

#define LPK_USER_BREAK 20
#define LPK_ERR_CMD_NOT_PRACTICABLE 24
#define LPK_BUS_ERR_HEAD_NOT_CONNECTED 27
//#define LPK_ERR_EXT_DEVICE 31

#define ADR_HOST 1
#define ADR_LPK_2 2
#define ADR_LPK_12 12
#define ADR_LPK_22 22
#define ADR_LPK_32 32
#define ADR_LPK_42 42
//#define BUS_ADR_StacAB 11
//#define BUS_ADR_CarZHead 21
//#define BUS_ADR_LPK 22
//#define BUS_ADR_CarZ_as_Well 2

#define ANSWER_BUF_LEN 288//256

#define errNone 0
#define errSafetyAppliance 65535
#define errInfo errSafetyAppliance - 1
#define errSync errSafetyAppliance - 2
#define errRxOverflow errSafetyAppliance - 3
#define errWeiterleitung errSafetyAppliance - 4
#define errInfoLPK errSafetyAppliance - 5

#define COUNT_READ_KEY 10

#ifndef OFF
 #define OFF 0
#endif

#ifndef ON
 #define ON 1
#endif

#define ESTOP_OFF 0
// ------------------------------------ NEU ---------------------------------------
// Peripherie - Output - Pin
#define LED_1			P9_OUT_P1	// hight active
#define LED_2			P9_OUT_P2	// hight active
#define LED_3			P9_OUT_P7	// hight active

#define SW_RESERVE		P1_OUT_P2	// low active
#define Z_BRAKE			P1_OUT_P3	// low active

#define RESET_LANPORT	P2_OUT_P0	// low active

#define DOPPEL_TASTEN_UI P2_IN_P5
#define NRTL_LP P6_IN_P2

#define JALMON1			P5_IN_P14	// low active

//#ifdef _CF_NRTL
	#define UEW_FET1_24V	P10_IN_P10
	#define UEW_FET2_24V	P10_IN_P11
	#define UEW_FET3_48V	P10_IN_P8
	#define UEW_FET4_48V	P10_IN_P9
	#define FW_LOCK_DN		P2_OUT_P11
	#define FW_LOCK_UP		P1_OUT_P0
	#define IS_LP_201		P6_IN_P2
	#define ESTOP_EXT		P5_IN_P15	// low active
//#else
	#define CLEAR_FW_ESTOP	P10_OUT_P8	// low active
	#define LOCK_ESTOP_EXTERN		P10_OUT_P9	// low active
	#define DISABLE_ESTOP_INTERN	P10_OUT_P10	// low active
	#define DISABLE_ESTOP_EXTERN	P10_OUT_P11	// low active
	#define SET_FF 			P1_OUT_P0	// hight active
	#define JALMON2			P5_IN_P15	// low active
	#define SLO_MO_MODE		P2_IN_P2	// hight active
//#endif
// Peripherie - Input - Pin
#define DIL1_1			P8_IN_P0
#define DIL1_2			P8_IN_P1
#define DIL1_3			P8_IN_P2
#define DIL1_4			P8_IN_P3


#define ESTOP			P2_IN_P1	// low active

#define TA_SERVICE		P2_IN_P3	// hight active
#define LEVEL_DETECT	P2_IN_P4	// low active
#define TA2_SERVICE		P2_IN_P5	// hight active

#define SET_VCC_OEM		P0_OUT_P6	// hight is on
#define SET_OEM_IN		P0_OUT_P7	// high is npn to vcc, low is pnp to gnd
#define SET_PHOTOMOS	P10_OUT_P4	// hight is on
#define GET_OEM_IN		P2_IN_P6	// high or low, see SET_OEM_IN

#define SET_VCC_UVL		P1_OUT_P1	// hight is on
#define SET_UVL_IN		P1_OUT_P7	// high is npn to vcc, low is pnp to gnd
#define SET_SW_UVL		P1_OUT_P4	// hight is on
#define GET_UVL_IN		P2_IN_P10	// high or low, see SET_UVL_IN

#define MUELL_IN		P8_IN_P6	// high active
#define MUELL_CTRL		P10_OUT_P3	// high active

// ------------------------------------ ALT ---------------------------------------
// MotorControl - Steuerung
/*
#define M1_POTI_CS 		P0_OUT_P4
#define M1_A1_MS1		P9_OUT_P0
//#define M1_A1_MS2		P9_OUT_P1
//#define M1_A1_RST		P9_OUT_P2
#define M1_A1_ENABLE	P9_OUT_P3
#define M1_A1_DIR		P9_OUT_P4
#define M1_A1_SLEEP		P10_OUT_P7

#define M2_POTI_CS 		P0_OUT_P5
#define M2_A2_ENABLE	P9_OUT_P5
#define M2_A2_DIR		P9_OUT_P6
//#define M2_A2_RST		P9_OUT_P7
#define M2_A2_MS1		P10_OUT_P4
#define M2_A2_MS2		P10_OUT_P5
#define M2_A2_SLEEP		P10_OUT_P6

// MotorControl - Endlagen
#define M1_ZERO_SENS	P11_IN_P0
#define M1_PREZERO_SENS	P11_IN_P1

#define M2_ZERO_SENS	P11_IN_P3
#define M2_PREZERO_SENS	P11_IN_P4

// MotorControl - Handling - optionale Sensoren
#define M1_OPTIONAL_SENS	P11_IN_P2
#define M2_OPTIONAL_SENS	P11_IN_P5


#define SENS_X6			P2_IN_P3

// Peripherie - Input - Function
#define DOOR_OPEN		SENS_X6

// Peripherie - Output - Pin
// switcher
#define SWITCH1_X9		P0_OUT_P6
#define SWITCH2_X8		P1_OUT_P3
#define SWITCH3_X7		P1_OUT_P1
#define SWITCH4_X7		P1_OUT_P2
//extern IF, XS19

#define XS19_7			P2_OUT_P4
#define XS19_11			P2_OUT_P5
#define XS19_9			P2_OUT_P6
#define LED_6			P2_OUT_P7
#define LED_5			P2_OUT_P8
#define LED_4			P2_OUT_P10

// Peripherie - Output - Function
#define M1_BRAKE		SWITCH1_X9
#define M2_BRAKE		SWITCH1_X8

#define STAC_MAG_A		SWITCH1_X8
#define STAC_MAG_B		SWITCH1_X9
*/
//****************************************************************************
// @Typedefs
//****************************************************************************
/*enum TESTSENSOR
{
	TEST_SENSOR_JALMON1		= 1
	,TEST_SENSOR_JALMON2	= 2
	,TEST_SENSOR_DILL1		= 3

	,TEST_SENSOR_CTS2_UART2	= 4
	,TEST_SENSOR_CTS3_UART3	= 5
	,TEST_SENSOR_CTS4_UART4	= 6
	,TEST_TA_SERVICE		= 7
	,TEST_TA2_SERVICE		= 8
	,TEST_ESTOP				= 9
	,TEST_SLO_MO_MODE		= 10
	,TEST_LEVEL_DETECT		= 11
	,TEST_SENSOR_EX0		= 12
	,TEST_SENSOR_EX1		= 13

};*/

enum SENSOR
{
	SENSOR_GET_OEM_IN1		= 1
	,SENSOR_GET_UVL_IN		= 2
	,SENSOR_MUELL_IN		= 3
	,SENSOR_Z_AXIS_ZERO		= 20
};
/*enum nrPORT
{
	PORT_Z_BRAKE				= 1
	,PORT_SW_RESERVE			= 2

	,PORT_RESET_LANPORT			= 3
	,PORT_CLEAR_FW_ESTOP		= 4
	,PORT_LOCK_ESTOP_EXTERN		= 5
	,PORT_DISABLE_ESTOP_INTERN	= 6
	,PORT_DISABLE_ESTOP_EXTERN	= 7

	,PORT_LED_1					= 8
	,PORT_LED_2					= 9
	,PORT_LED_3					= 10

	,PORT_U0C1_COM1_RE_485		= 11
	,PORT_U0C1_COM1_DE_485		= 12
	,PORT_U1C1_COM2_RTS2		= 13
	,PORT_U2C0_COM3_RTS3		= 14
	,PORT_U2C1_COM4_RTS4		= 15
	,PORT_FRAM_CS				= 16
	,PORT_CCU63_CC60_P93		= 20
	,PORT_CCU63_CC61_P94		= 21
	,PORT_CCU63_CC62_P95		= 22
	,PORT_CCU63_CC63_P96		= 23
};*/


enum bitPORT
{
	//bitPORT_Z_BRAKE   				= 0x0001
	bitPORT_SET_PHOTOMOS	= 0x0001
	,PORT_SET_PHOTOMOS		= 1
	,bitPORT_SET_VCC_OEM   	= 0x0002
	,PORT_SET_VCC_OEM   	= 2
	,bitPORT_SET_SW_UVL		= 0x0004
	,PORT_SET_SW_UVL		= 3
	,bitPORT_SET_VCC_UVL   	= 0x0008
	,PORT_SET_VCC_UVL   	= 4
	,bitPORT_MUELL_CTRL		= 0x0010
	,PORT_MUELL_CTRL		= 5
	,bitPORT_SET_OEM_IN		= 0x0020
	,PORT_SET_OEM_IN		= 6
	,bitPORT_SET_UVL_IN		= 0x0040
	,PORT_SET_UVL_IN		= 7
	,bitPORT_SW_RESERVE		= 0x0080
	,PORT_SW_RESERVE		= 8
	,bitPORT_MAX			= 0x0080
};


enum bitEquipment
{
	BIT_E_MuellFoerderer	= 0x0001
	,BIT_E_DoorLifter	    = 0x0002
	,BIT_E_Equipment_MAX	= 0x0003
};
enum bitDeviceConfig
{
	BIT_D_MuellFoerderer	= 0x0001
	,BIT_D_DoorLifter	    = 0x0002
	,BIT_D_DeviceConfig_MAX	= 0x0003
};


enum bitPORTTest
{
	Test_bitPORT_RESET_LANPORT			= 0x0001
	,Test_bitPORT_CLEAR_FW_ESTOP		= 0x0002
	,Test_bitPORT_LOCK_ESTOP_EXTERN	 	= 0x0004
	,Test_bitPORT_DISABLE_ESTOP_INTERN	= 0x0008
	,Test_bitPORT_DISABLE_ESTOP_EXTERN	= 0x0010
	,Test_bitPORT_LED_1					= 0x0020
	,Test_bitPORT_LED_2					= 0x0040
	,Test_bitPORT_LED_3					= 0x0080
	,Test_bitPORT_U0C1_COM1_RE_485	   	= 0x0100
	,Test_bitPORT_U0C1_COM1_DE_485	   	= 0x0200
	,Test_bitPORT_U1C1_COM2_RTS2		= 0x0400
	,Test_bitPORT_U2C0_COM3_RTS3		= 0x0800
	,Test_bitPORT_U2C1_COM4_RTS4		= 0x1000
	,Test_bitPORT_FRAM_CS		 		= 0x2000
	,Test_bitPORT_SET_FF		 		= 0x4000
};


/*typedef enum
{
	eKFA_RESET_DRIVE		=	0x0000
	,eKFA_H_DRIVE			=	0x0001
	,eKFA_HEAD_Z_DRIVE		=	0x0002
	,eKFA_Z_HEIZ_DRIVE		=	0x0004
	,eKFA_Z_MAGADAP_DRIVE	=	0x0008
	,eKFA_STAC_A_DRIVE		=	0x0010
	,eKFA_STAC_B_DRIVE		=	0x0020
}udtKFA_DRIVE_Type;*/

typedef enum
{
	 eDil1None		= 0x0000
	,eDil1_1		= 0x0001
	,eDil1_2		= 0x0002
	,eDil1_3		= 0x0004
	,eDil1_4		= 0x0008
}udtDilGroup;

enum ERR_ISTATE
{
	istate_MotorInitAxisBase = 1

	,istate_MotorEStop = 2
	,istate_MotorJalousie = 3
//#ifdef _CF_NRTL
	,istate_MotorExternEstop = 4
	,istate_MotorFETEstopL = 5
	,istate_MotorFETEstopH = 6
//#endif
	,istate_DoorLiftMotorInit = 7
	,istate_FRAMReadAxisMinMaxWay = 9
	,istate_FRAMStructVersion	= 10
	,istate_FRAMReadStructVersion = 11
	,istate_FRAMReadDeviceConfig = 12
	,istate_FRAMReadAxisOffset	= 13
	,istate_ImsCmd_Sync	= 14
	,istate_FRAMReadSensorConfig = 15
	,istate_FRAMReadPortConfig = 16
	,istate_FRAMReadEquipmentConfig = 17
	,istate_FRAMReadEstopCounter = 18
	,istate_FRAMReadEstopWait = 19
	,istate_FRAMReadConfigStallExeption = 20
	,istate_FRAMReadCounterStallExeption = 21
	,istate_FRAMReadSearchAdapterRelativeDeltaWay = 22
	,istate_FRAMReadApplicationSpeed = 23

	,istate_FRAM_AutoChangeError_MinMaxWay = 100
	,istate_FRAM_AutoChangeError_StructVersion = 101
	,istate_FRAM_AutoChangeError_SensorConfig = 102
	,istate_FRAM_AutoChangeError_PortConfig = 103
	,istate_FRAM_AutoChangeError_EquipmentConfig = 104
	,istate_FRAM_AutoChangeError_EstopCounter = 105
	,istate_FRAM_AutoChangeError_EstopWait = 106
	,istate_FRAM_AutoChangeError_ConfigStallExeption = 107
	,istate_FRAM_AutoChangeError_CounterStallExeption = 108
	,istate_FRAM_AutoChangeError_SearchAdapterRelativeDeltaWay = 109
	,istate_FRAM_AutoChangeError_ApplicationSpeed = 110
	,istate_FRAM_AutoChangeError_DoorLiftConfig = 111

	,MAX_CNT_ISTATE	= 26

	,istate_FRAMAutomaticChange_NoChange = 549
	,istate_FRAMAutomaticChange_V1_to_V2 = 550
	,istate_FRAMAutomaticChange_V1_to_V3
	,istate_FRAMAutomaticChange_V1_to_V4
	,istate_FRAMAutomaticChange_V1_to_V5
	,istate_FRAMAutomaticChange_V1_to_V6
	,istate_FRAMAutomaticChange_V1_to_V7
	,istate_FRAMAutomaticChange_V1_to_V8

	,istate_FRAMAutomaticChange_V2_to_V3
	,istate_FRAMAutomaticChange_V2_to_V4
	,istate_FRAMAutomaticChange_V2_to_V5
	,istate_FRAMAutomaticChange_V2_to_V6
	,istate_FRAMAutomaticChange_V2_to_V7
	,istate_FRAMAutomaticChange_V2_to_V8

	,istate_FRAMAutomaticChange_V3_to_V4
	,istate_FRAMAutomaticChange_V3_to_V5
	,istate_FRAMAutomaticChange_V3_to_V6
	,istate_FRAMAutomaticChange_V3_to_V7
	,istate_FRAMAutomaticChange_V3_to_V8

	,istate_FRAMAutomaticChange_V4_to_V5
	,istate_FRAMAutomaticChange_V4_to_V6
	,istate_FRAMAutomaticChange_V4_to_V7
	,istate_FRAMAutomaticChange_V4_to_V8

	,istate_FRAMAutomaticChange_V5_to_V6
	,istate_FRAMAutomaticChange_V5_to_V7
	,istate_FRAMAutomaticChange_V5_to_V8

	,istate_FRAMAutomaticChange_V6_to_V7
	,istate_FRAMAutomaticChange_V6_to_V8

	,istate_FRAMAutomaticChange_V7_to_V8
};

//****************************************************************************
// @Imported Global Variables
//****************************************************************************

//****************************************************************************
// @Global Variables
//****************************************************************************

//****************************************************************************
// @External Prototypes
//****************************************************************************

//****************************************************************************
// Prototypen definitions
//****************************************************************************
//****************************************************************************
// @Prototypes Of Local Functions
//****************************************************************************



 // Konstanten für die SM-Steuerung



const struct DEV_CONFIG 
			{
 				const char *name;			// Gerätename
 				unsigned char rt_cfg;				// Konfiguration RT-Antrieb (true, false)
				float Height;
			};
	
 #define DRV_CONFIG_CNT 10					// mit ausreichend Reserve für zukünftige Erweiterungen
 struct DRV_CONFIG 	{
 						unsigned char index;			// Index des Antriebes (siehe StacComm.h)
 						unsigned char adr;				// Adresse des Antriebes
						unsigned char cfg;				// Antrieb aktiviert
						float offset;			// Offset zum Nullpunkt
					};


// Schnittstellenhandles
//COM0 U0C0 HOST stdin ASC0
//#define HOST_COM0_U0C0_IN &_iob[0]
//COM0 U0C0 HOST stdout ASC0
//#define HOST_COM0_U0C0_OUT &_iob[1]

//COM1 U0C1 RS485 MainBus
#define COM1_RS485_U0C1_IN &_iob[3]
//COM1 U0C1 RS485 MainBus
#define COM1_RS485_U0C1_OUT &_iob[4]

//COM2 U1C1 RS232 USB
#define COM2_USB_U1C1_IN &_iob[5]
//COM2 U1C1 RS232 USB
#define COM2_USB_U1C1_OUT &_iob[6]

//COM3 U2C0 RS232 LAN
#define COM3_LAN_U2C0_IN &_iob[7]
//COM3 U2C0 RS232 LAN
#define COM3_LAN_U2C0_OUT &_iob[7]

//COM4 U2C1 RS232 Barcode
#define COM4_BARC_U2C1_IN &_iob[8]
//COM4 U2C1 RS232 Barcode
#define COM4_BARC_U2C1_OUT &_iob[8]

//COM5 U1C0 RS232 Slave
#define COM5_SLAVE_U1C0_IN &_iob[9]
//COM5 U1C0 RS232 Slave
#define COM5_SLAVE_U1C0_OUT &_iob[9]


// FRAM Auswahl	von RAMTRON
//#define FRAM25040	// nur dieser Typ muss beim Einsatz deklariert werden, da Addressierung abweicht

#define SPI_CLK_DELAY 1	 // Zeit für die Taktgenerierung, abhängig von CPU-Frequenz, Verwendung in SPI_PORT_PINS.C
//#define FRAM_ENABLE	IO_P0_3= 0;
//#define FRAM_DISABLE IO_P0_3= 1;
#endif
