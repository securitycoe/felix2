//****************************************************************************
// @Module        Moror Config
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

#include "main.h"
#include "DeviceConfig.h"
//#include "Shared/Stacker_Lifter_Allgemein/StacError.h"
#include "Drv_config.h"
#include "MDrive_BusCtrl.h"
//****************************************************************************
// @Macros
//****************************************************************************

//****************************************************************************
// local definitions
//****************************************************************************

//---- FELIX -----------------
#ifdef _CF_FELIX
//#define VI_FRONT_CAR_STEP_PER_SEC 60
#define FRONT_CAR_WAY_PRO_REV 48.0F
#define FRONT_CAR_ENC_IMP_PRO_REV -2048.0F
#define FRONT_CAR_MOT_IMP_PRO_REV 2048.0F

//#define VI_REAR_CAR_STEP_PER_SEC 60
#define REAR_CAR_WAY_PRO_REV 48.0F
#define REAR_CAR_ENC_IMP_PRO_REV -2048.0F//2048.0F
#define REAR_CAR_MOT_IMP_PRO_REV 2048.0F

//#define VI_BOTTOM_CAR_STEP_PER_SEC 60
#define BOTTOM_CAR_WAY_PRO_REV 48.0F
#define BOTTOM_CAR_ENC_IMP_PRO_REV 2048.0F//-2048.0F
#define BOTTOM_CAR_MOT_IMP_PRO_REV 2048.0F

//#define VI_XDRIVE_STEP_PER_SEC 60
#define XDRIVE_WAY_PRO_REV 60.0F
#define XDRIVE_ENC_IMP_PRO_REV -2048.0F
#define XDRIVE_MOT_IMP_PRO_REV 2048.0F

#endif

//#define VI_ZDRIVE_CAR_STEP_PER_SEC 80
#define ZDRIVE_WAY_PRO_REV ((16.0F / 26.0F ) * 12.0F )		// Antrieb 16 Zähne, Achse 26 Zähne, Spindel 12mm Steigung
//Kugelumlaufspindel
//#define ZDRIVE_WAY_PRO_REV ((16.0F / 26.0F ) * 10.0F )		// Antrieb 16 Zähne, Achse 26 Zähne, Spindel 12mm Steigung
#define ZDRIVE_ENC_IMP_PRO_REV 2048.0F//-2048.0F
#define ZDRIVE_MOT_IMP_PRO_REV 2048.0F



//---- PENTA DISK -----------------
#ifdef _CF_PENTA_DISK
//#define VI_DISKDRIVE_STEP_PER_SEC 60
#define DISKDRIVE_WAY_PRO_REV 360.0F
//#define DISKDRIVE_ENC_IMP_PRO_REV (2048.0F * 13.49981625)	//Motor U = 40mm (16 Zähne bei T2,5); Scheibe U = 1079,9853
//#define DISKDRIVE_MOT_IMP_PRO_REV (2048.0F * 13.49981625)	//Motor U = 40mm (16 Zähne bei T2,5); Scheibe U = 1079,9853

//13.47358398
//13.47368164
//13.47371094
//13.47373047 bei 1000 Umdrehungen 375 steps zu weit
//13.47354736
#define DISKDRIVE_ENC_IMP_PRO_REV (-2048.0F * 13.48046875)	//Motor U = 40mm (16 Zähne bei T2,5); Scheibe U = 1079,9853
#define DISKDRIVE_MOT_IMP_PRO_REV (2048.0F * 13.48046875)	//Motor U = 40mm (16 Zähne bei T2,5); Scheibe U = 1079,9853
#endif

//****************************************************************************
// default settings
//****************************************************************************

//****************************************************************************
// external functions
//****************************************************************************

//****************************************************************************
// external data
//****************************************************************************

//****************************************************************************
// public data
//****************************************************************************
//float g_AxesWayArray[MAX_AXES];
//float *g_ptrAxesWayArray;
//unsigned short g_AxesStatusArray[MAX_AXES];
//unsigned short *g_ptrAxesStatusArray;



/*const unsigned short AxesArray[MAX_AXES]=
{
	eX_Axis
	,eFrontCarriage_Axis
	,eRearCarriage_Axis
	,eBottomCarriage
	,eZ_Axis
};*/
/*1 -3  -33.33   99  99
2 -18 -41.66  749  749
3 -18  41.66 -749 -749
4 -18 -41.66  749 749
5 0	0*/
#ifdef _CF_FELIX
const udtIMSAxisFixedParameter FixFrontCarriage = {
	(unsigned short) AXIS_ID_FRONT_CARRIAGE
	,(char) AXIS_ADR_FRONT_CARRIAGE		//Adress;
	,(unsigned short) eFrontCarriage_Axis			// MaskenBit
	,(float) -18.0				//Offset
	,(float) -135.0				// SetHomingPosition
	,(float) 1.0				// Differenz for slomo restore position handling
	// Geschwindigkeiten
	//,(float) VI_FRONT_CAR_STEP_PER_SEC / (FRONT_CAR_MOT_IMP_PRO_REV / FRONT_CAR_WAY_PRO_REV)			//[mm/s]
	,(float) 1.0			 	// initial speed [mm/s]
	,(float) 300.0//150.0//300.0			 	// Endspeed negative direction [mm/s]
	,(float) 300.0//150.0//300.0			 	// Endspeed positive direction [mm/s]
	,(float) 40.0					 	// Endspeed homing [mm/s]
,(float) 250.0//300.0					 	// Endspeed 1536 [mm/s]
	,(float) 500.0//700.0				// Acceleration negative direction [mm/s*s]
	,(float) 500.0//700.0				// Acceleration positive direction [mm/s*s]
	,(float) 200.0				// Acceleration homing [mm/s*s]
	,(float) 500.0				// Acceleration slew [mm/s*s]MAX_AXES
,(float) 250.0//300.0				// Acceleration 1536 [mm/s*s]

	// Grenzwerte
	,(float) 1.0				// min. Acceleration [mm/s*s]
	,(float) 1000.0				// max. Acceleration [mm/s*s]
	,(float) 300.0				// max. 1536 Acceleration [mm/s*s]
	,(float) 1.0				// min. Endspeed [mm/s*s]
	,(float) 500.0				// max. Endspeed [mm/s*s]
	,(float) 300.0				// max. 1536 Endspeed [mm/s*s]
	,(float) -135.7//-137.0				// min. drive range [mm]
	,(float) 7.6				// max. drive range [mm]

	// sonstiges
	,(float) 0.1				// Lose (mech. Spiel) [mm]
	,(float) 0.03//0.01//0.03				// Genauigkeit der Positionierung(Deadband) [mm]
	,(float) 0.03//0.01//0.03				// Genauigkeit der Positionierung 1536 (Deadband) [mm]
	,(unsigned short) 100		// 60 zulässige Diff. Steps zu Encoder [encoder_step]
	,(unsigned short) 500		// hold current wait time [millisecounds]
	,(unsigned short) 50			// wobbling wait time [millisecounds]
	,(unsigned short) 80			// wobbling wait time 1536[millisecounds]

	// Strom
	,(unsigned short) 90		//  run current [percent]
	,(unsigned short) 90		//  run current 1536 [percent]
	,(unsigned short) 40//15		// hold current [percent]
	,(unsigned short) 50//15		// hold current 1536 [percent]
	// Übersetzung
	,(float) FRONT_CAR_WAY_PRO_REV	// Weg pro Umdrehung in mm
	,(float) FRONT_CAR_ENC_IMP_PRO_REV	// EncoderAuflösung	0 -> Achse ohne Encoder
	,(float) (FRONT_CAR_MOT_IMP_PRO_REV / FRONT_CAR_WAY_PRO_REV)	// Motor-Impulse (µ-Step) pro mm	bzw. pro Umdr.
	,(float) (FRONT_CAR_ENC_IMP_PRO_REV / FRONT_CAR_WAY_PRO_REV)	// Encoder-Impulse pro mm
//	unsigned short usFullStepPerRev;	// Vollschritte pro Motorumdrehung
//	unsigned short usMicroStepPerFullStep;	//Anzahl der Mikroschritte pro Vollschritt
//	float fMicroStepProRev;			// Microschritte pro Umdrehung
	//void (*Direction)(unsigned short usDirection);
	,(unsigned short) HM_SLEW_POSITIVE_CREEP_NEGATIVE	// normales homing
	,(unsigned short) HM_SLEW_NEGATIVE_CREEP_POSITIVE	// inverses homig
	,(short) 1
	,(unsigned long) (500 * (FRONT_CAR_MOT_IMP_PRO_REV / FRONT_CAR_WAY_PRO_REV))
};

const udtIMSAxisFixedParameter FixRearCarriage = {
	(unsigned short) AXIS_ID_REAR_CARRIAGE
	,(char) AXIS_ADR_REAR_CARRIAGE			//Adress;
	,(unsigned short) eRearCarriage_Axis			// MaskenBit
	,(float) 18.0//-18.0				//Offset
	,(float) 135.0				// SetHomingPosition
	,(float) 1.0				// Differenz for slomo restore position handling
	// Geschwindigkeiten
	//,(float) VI_REAR_CAR_STEP_PER_SEC / (REAR_CAR_MOT_IMP_PRO_REV / REAR_CAR_WAY_PRO_REV)			//[mm/s]
	,(float) 1.0			 	// initial speed [mm/s]
	,(float) 300.0//150.0//300.0//150.0			 	// Endspeed negative direction [mm/s]
	,(float) 300.0//150.0//300.0//150.0			 	// Endspeed positive direction [mm/s]
	,(float) 40.0			 	// Endspeed homing [mm/s]
,(float) 250.0//300.0					 	// Endspeed 1536 [mm/s]
	,(float) 500.0//700.0//600.0				// Acceleration negative direction [mm/s*s]
	,(float) 500.0//700.0//600.0				// Acceleration positive direction [mm/s*s]
	,(float) 200.0				// Acceleration homing [mm/s*s]
	,(float) 500.0				// Acceleration slew [mm/s*s]MAX_AXES
,(float) 250.0//300.0				// Acceleration 1536 [mm/s*s]


	// Grenzwerte
	,(float) 1.0				// min. Acceleration [mm/s*s]
	,(float) 1000.0				// max. Acceleration [mm/s*s]
	,(float) 300.0				// max. 1536 Acceleration [mm/s*s]
	,(float) 1.0				// min. Endspeed [mm/s*s]
	,(float) 500.0				// max. Endspeed [mm/s*s]
	,(float) 300.0				// max. 1536 Endspeed [mm/s*s]
	,(float) -7.8				// min. drive range [mm]
	,(float) 135.7//137.0				// max. drive range [mm]

	// sonstiges
	,(float) 0.1				// Lose (mech. Spiel) [mm]
	,(float) 0.03//0.01//0.03				// Genauigkeit der Positionierung(Deadband) [mm]
	,(float) 0.03//0.01//0.03				// Genauigkeit der Positionierung 1536 (Deadband) [mm]
	,(unsigned short) 100		// 60 zulässige Diff. Steps zu Encoder [encoder_step]
	,(unsigned short) 500		// hold current wait time [millisecounds]
	,(unsigned short) 50			// wobbling wait time [millisecounds]
	,(unsigned short) 80			// wobbling wait time 1536[millisecounds]

	// Strom
	,(unsigned short) 90		//  run current [percent]
	,(unsigned short) 90		//  run current 1536 [percent]
	,(unsigned short) 40//15		// hold current [percent]
	,(unsigned short) 50//15		// hold current 1536 [percent]

	// Übersetzung
	,(float) REAR_CAR_WAY_PRO_REV	// Weg pro Umdrehung in mm
	,(float) REAR_CAR_ENC_IMP_PRO_REV	// EncoderAuflösung	0 -> Achse ohne Encoder
	,(float) (REAR_CAR_MOT_IMP_PRO_REV / REAR_CAR_WAY_PRO_REV)	// Motor-Impulse (µ-Step) pro mm	bzw. pro Umdr.
	,(float) (REAR_CAR_ENC_IMP_PRO_REV / REAR_CAR_WAY_PRO_REV)	// Encoder-Impulse pro mm
//	unsigned short usFullStepPerRev;	// Vollschritte pro Motorumdrehung
//	unsigned short usMicroStepPerFullStep;	//Anzahl der Mikroschritte pro Vollschritt
//	float fMicroStepProRev;			// Microschritte pro Umdrehung
	//void (*Direction)(unsigned short usDirection);
	,(unsigned short) HM_SLEW_NEGATIVE_CREEP_POSITIVE
	,(unsigned short) HM_SLEW_POSITIVE_CREEP_NEGATIVE	// inverses homig
	,(short) -1
	,(unsigned long) (500 * (REAR_CAR_MOT_IMP_PRO_REV / REAR_CAR_WAY_PRO_REV))
};

const udtIMSAxisFixedParameter FixBottomCarriage = {
	(unsigned short) AXIS_ID_BOTTOM_CARRIAGE
	,(char) AXIS_ADR_BOTTOM_CARRIAGE	//Adress;
	,(unsigned short) eBottomCarriage			// MaskenBit
	,(float) 18.0//-18.0				//Offset
	,(float) 135.0				// SetHomingPosition
	,(float) 1.0				// Differenz for slomo restore position handling
	// Geschwindigkeiten
	//,(float) VI_BOTTOM_CAR_STEP_PER_SEC / (BOTTOM_CAR_MOT_IMP_PRO_REV / BOTTOM_CAR_WAY_PRO_REV)			//[mm/s]
	,(float) 1.0			 	// initial speed [mm/s]
	,(float) 300.0//150.0	 	// Endspeed negative direction [mm/s]
	,(float) 300.0//150.0	 	// Endspeed positive direction [mm/s]
	,(float) 40.0			 	// Endspeed homing [mm/s]
,(float) 250.0//300.0					 	// Endspeed 1536 [mm/s]
	,(float) 500.0//700.0//600.0		// Acceleration negative direction [mm/s*s]
	,(float) 500.0//700.0//600.0		// Acceleration positive direction [mm/s*s]
	,(float) 200.0				// Acceleration homing [mm/s*s]
	,(float) 500.0				// Acceleration slew [mm/s*s]MAX_AXES
,(float) 250.0//300.0				// Acceleration 1536 [mm/s*s]

	// Grenzwerte
	,(float) 1.0				// min. Acceleration [mm/s*s]
	,(float) 1000.0				// max. Acceleration [mm/s*s]
	,(float) 300.0				// max. 1536 Acceleration [mm/s*s]
	,(float) 1.0				// min. Endspeed [mm/s*s]
	,(float) 500.0				// max. Endspeed [mm/s*s]
	,(float) 300.0				// max. 1536 Endspeed [mm/s*s]
	,(float) -5.2				// min. drive range [mm]
	,(float) 136.2//137.0				// max. drive range [mm]

	// sonstiges
	,(float) 0.1				// Lose (mech. Spiel) [mm]
	,(float) 0.03//0.01//0.03				// Genauigkeit der Positionierung(Deadband) [mm]
	,(float) 0.03//0.01//0.03				// Genauigkeit der Positionierung 1536 (Deadband) [mm]
	,(unsigned short) 100		// 60 zulässige Diff. Steps zu Encoder [encoder_step]
	,(unsigned short) 500		// hold current wait time [millisecounds]
	,(unsigned short) 50			// wobbling wait time [millisecounds]
	,(unsigned short) 80			// wobbling wait time 1536[millisecounds]

	// Strom
	,(unsigned short) 90		//  run current [percent]
	,(unsigned short) 90		//  run current 1536 [percent]
	,(unsigned short) 40//15		// hold current [percent]
	,(unsigned short) 50//15		// hold current 1536 [percent]
	// Übersetzung
	,(float) BOTTOM_CAR_WAY_PRO_REV	// Weg pro Umdrehung in mm
	,(float) BOTTOM_CAR_ENC_IMP_PRO_REV	// EncoderAuflösung	0 -> Achse ohne Encoder
	,(float) (BOTTOM_CAR_MOT_IMP_PRO_REV / BOTTOM_CAR_WAY_PRO_REV)	// Motor-Impulse (µ-Step) pro mm	bzw. pro Umdr.
	,(float) (BOTTOM_CAR_ENC_IMP_PRO_REV / BOTTOM_CAR_WAY_PRO_REV)	// Encoder-Impulse pro mm
//	unsigned short usFullStepPerRev;	// Vollschritte pro Motorumdrehung
//	unsigned short usMicroStepPerFullStep;	//Anzahl der Mikroschritte pro Vollschritt
//	float fMicroStepProRev;			// Microschritte pro Umdrehung
	//void (*Direction)(unsigned short usDirection);
	,(unsigned short) HM_SLEW_POSITIVE_CREEP_NEGATIVE
	,(unsigned short) HM_SLEW_NEGATIVE_CREEP_POSITIVE	// inverses homig
	,(short) 1
	,(unsigned long) (500 * (BOTTOM_CAR_MOT_IMP_PRO_REV / BOTTOM_CAR_WAY_PRO_REV))
};

const udtIMSAxisFixedParameter FixXDrive = {
	(unsigned short) AXIS_ID_X_DRIVE
	,(char) AXIS_ADR_X_DRIVE	//Adress;
	,(unsigned short) eX_Axis			// MaskenBit
	,(float) 0.0//-3.0				//Offset
	,(float) 0.0				// SetHomingPosition
	,(float) 1.0				// Differenz for slomo restore position handling
	// Geschwindigkeiten
	//,(float) VI_XDRIVE_STEP_PER_SEC / (XDRIVE_MOT_IMP_PRO_REV / XDRIVE_WAY_PRO_REV)			//[mm/s]
	,(float) 1.0			 	// initial speed [mm/s]
	,(float) 300.0//400.0			 	// Endspeed negative direction [mm/s]
	,(float) 300.0//400.0			 	// Endspeed positive direction [mm/s]
	,(float) 60.0			 	// Endspeed homing [mm/s]
,(float) 250.0//300.0					 	// Endspeed 1536 [mm/s]
	,(float) 300.0//350.0//700.0//1000.0		// Acceleration negative direction [mm/s*s]
	,(float) 300.0//350.0//700.0//1000.0		// Acceleration positive direction [mm/s*s]
	,(float) 600.0//400.0 		// Acceleration homing [mm/s*s]
	,(float) 500.0				// Acceleration slew [mm/s*s]
,(float) 250.0//300.0				// Acceleration 1536 [mm/s*s]

	// Grenzwerte
	,(float) 1.0				// min. Acceleration [mm/s*s]
	,(float) 1000.0				// max. Acceleration [mm/s*s]
	,(float) 300.0				// max. 1536 Acceleration [mm/s*s]
	,(float) 1.0				// min. Endspeed [mm/s*s]
	,(float) 400.0//500.0		// max. Endspeed [mm/s*s]
	,(float) 300.0				// max. 1536 Endspeed [mm/s*s]
	,(float) -5.5//-3.0	 		// min. drive range [mm]
	,(float) 397.4				// max. drive range [mm]

	// sonstiges
	,(float) 0.1				// Lose (mech. Spiel) [mm]
	,(float) 0.03//0.01//0.03				// Genauigkeit der Positionierung(Deadband) [mm]
	,(float) 0.03//0.01//0.03				// Genauigkeit der Positionierung 1536 (Deadband) [mm]
	,(unsigned short) 100		// 60 zulässige Diff. Steps zu Encoder [encoder_step]
	,(unsigned short) 500		// hold current wait time [millisecounds]
	,(unsigned short) 100//300			// wobbling wait time [millisecounds]
	,(unsigned short) 150			// wobbling wait time 1536[millisecounds]

	// Strom
	,(unsigned short) 100		//  run current [percent]
	,(unsigned short) 100		//  run current 1536 [percent]
	,(unsigned short) 65//30		// hold current [percent]
	,(unsigned short) 70//30		// hold current 1536 [percent]
	// Übersetzung
	,(float) XDRIVE_WAY_PRO_REV	// Weg pro Umdrehung in mm
	,(float) XDRIVE_ENC_IMP_PRO_REV	// EncoderAuflösung	0 -> Achse ohne Encoder
	,(float) (XDRIVE_MOT_IMP_PRO_REV / XDRIVE_WAY_PRO_REV)	// Motor-Impulse (µ-Step) pro mm	bzw. pro Umdr.
	,(float) (XDRIVE_ENC_IMP_PRO_REV / XDRIVE_WAY_PRO_REV)	// Encoder-Impulse pro mm
//	unsigned short usFullStepPerRev;	// Vollschritte pro Motorumdrehung
//	unsigned short usMicroStepPerFullStep;	//Anzahl der Mikroschritte pro Vollschritt
//	float fMicroStepProRev;			// Microschritte pro Umdrehung
	//void (*Direction)(unsigned short usDirection);
	,(unsigned short) HM_SLEW_POSITIVE_CREEP_NEGATIVE
	,(unsigned short) HM_SLEW_NEGATIVE_CREEP_POSITIVE	// inverses homig
	,(short) 1
	,(unsigned long) (300 * (XDRIVE_MOT_IMP_PRO_REV / XDRIVE_WAY_PRO_REV))
};

const udtIMSAxisFixedParameter FixZDrive = {
	(unsigned short) AXIS_ID_Z_DRIVE
	,(char) AXIS_ADR_Z_DRIVE	//Adress;
	,(unsigned short) eZ_Axis			// MaskenBit
	,(float) 0.0				//Offset
	,(float) 0.0				// SetHomingPosition
	,(float) 1.0				// Differenz for slomo restore position handling
	// Geschwindigkeiten
	//,(float) VI_ZDRIVE_STEP_PER_SEC / (ZDRIVE_MOT_IMP_PRO_REV / ZDRIVE_WAY_PRO_REV)			//[mm/s]
	,(float) 1.0			 	// initial speed [mm/s]
	,(float) 130.0//140.0	 	// Endspeed negative direction [mm/s]
	,(float) 130.0//140.0	 	// Endspeed positive direction [mm/s]
	,(float) 70.0		 	 	// Endspeed homing [mm/s]
,(float) 110.0			 	// Endspeed 1536 [mm/s]
	,(float) 500.0//700.0//1000.0		// Acceleration negative direction [mm/s*s]
	,(float) 500.0//700.0//1000.0		// Acceleration positive direction [mm/s*s]
	,(float) 500.0				// Acceleration homing [mm/s*s]
	,(float) 500.0				// Acceleration slew [mm/s*s]
,(float) 250.0				// Acceleration 1536 [mm/s*s]

	// Grenzwerte
	,(float) 1.0				// min. Acceleration [mm/s*s]
	,(float) 1000.0				// max. Acceleration [mm/s*s]
	,(float) 300.0				// max. 1536 Acceleration [mm/s*s]
	,(float) 1.0				// min. Endspeed [mm/s*s]
	,(float) 140.0				// max. Endspeed [mm/s*s]
	,(float) 100.0				// max. 1536 Endspeed [mm/s*s]
	,(float) -281.0//-260.0//-240.0		// min. drive range [mm] eröht da Z-Achse 15mm länger ist
	,(float) 0.1				// max. drive range [mm]

	// sonstiges
	,(float) 0.1				// Lose (mech. Spiel) [mm]
	,(float) 0.01//0.011//0.03				// Genauigkeit der Positionierung(Deadband) [mm]
	,(float) 0.01//0.011//0.03				// Genauigkeit der Positionierung 1536 (Deadband) [mm]
	,(unsigned short) 120		// 80 zulässige Diff. Steps zu Encoder [encoder_step]
	,(unsigned short) 500		// hold current wait time [millisecounds]
	,(unsigned short) 50			// wobbling wait time [millisecounds]
	,(unsigned short) 50			// wobbling wait time 1536[millisecounds]

	// Strom
	,(unsigned short) 100		//  run current [percent]
	,(unsigned short) 100		//  run current 1536 [percent]
	,(unsigned short) 85		// hold current [percent]
	,(unsigned short) 85		// hold current [percent]
	// Übersetzung
	,(float) ZDRIVE_WAY_PRO_REV	// Weg pro Umdrehung in mm
	,(float) ZDRIVE_ENC_IMP_PRO_REV	// EncoderAuflösung	0 -> Achse ohne Encoder
	,(float) (ZDRIVE_MOT_IMP_PRO_REV / ZDRIVE_WAY_PRO_REV)	// Motor-Impulse (µ-Step) pro mm	bzw. pro Umdr.
	,(float) (ZDRIVE_ENC_IMP_PRO_REV / ZDRIVE_WAY_PRO_REV)	// Encoder-Impulse pro mm
//	unsigned short usFullStepPerRev;	// Vollschritte pro Motorumdrehung
//	unsigned short usMicroStepPerFullStep;	//Anzahl der Mikroschritte pro Vollschritt
//	float fMicroStepProRev;			// Microschritte pro Umdrehung
	//void (*Direction)(unsigned short usDirection);
	,(unsigned short) HM_SLEW_POSITIVE_CREEP_NEGATIVE
	,(unsigned short) HM_SLEW_NEGATIVE_CREEP_POSITIVE	// inverses homig
	,(short) 1
	,(unsigned long) (500 * (XDRIVE_MOT_IMP_PRO_REV / XDRIVE_WAY_PRO_REV))
};
#endif

#ifdef _CF_PENTA_DISK
const udtIMSAxisFixedParameter FixDiskDrive = {
	(unsigned short) AXIS_ID_DISK_DRIVE
	,(char) AXIS_ADR_DISK_DRIVE	//Adress;
	,(unsigned short) eDisk_Axis			// MaskenBit
	,(float) 0.0//-3.0				//Offset
	,(float) 0.0				// SetHomingPosition
	,(float) 1.0				// Differenz for slomo restore position handling
	// Geschwindigkeiten
	//,(float) VI_XDRIVE_STEP_PER_SEC / (XDRIVE_MOT_IMP_PRO_REV / XDRIVE_WAY_PRO_REV)			//[mm/s]
	,(float) 1.0			 	// initial speed [mm/s]
	,(float) 150.0//400.0			 	// Endspeed negative direction [mm/s]
	,(float) 150.0//400.0			 	// Endspeed positive direction [mm/s]
	,(float) 30.0//40.0			 	// Endspeed homing [mm/s]
,(float) 50.0//300.0					 	// Endspeed 1536 [mm/s]
	,(float) 80.0//350.0//700.0//1000.0		// Acceleration negative direction [mm/s*s]
	,(float) 80.0//350.0//700.0//1000.0		// Acceleration positive direction [mm/s*s]
	,(float) 80.0//150.0 		// Acceleration homing [mm/s*s]
	,(float) 50.0				// Acceleration slew [mm/s*s]
,(float) 50.0//300.0				// Acceleration 1536 [mm/s*s]

	// Grenzwerte
	,(float) 1.0				// min. Acceleration [mm/s*s]
	,(float) 350.0				// max. Acceleration [mm/s*s]
	,(float) 120.0				// max. 1536 Acceleration [mm/s*s]
	,(float) 1.0				// min. Endspeed [mm/s*s]
	,(float) 350.0//500.0		// max. Endspeed [mm/s*s]
	,(float) 200.0				// max. 1536 Endspeed [mm/s*s]
	,(float) 0.0//-3.0	 		// min. drive range [mm]
	,(float) 359.994				// max. drive range [mm]

	// sonstiges
	,(float) 0.1				// Lose (mech. Spiel) [mm]
	,(float) 0.01//0.01//0.03				// Genauigkeit der Positionierung(Deadband) [mm]
	,(float) 0.01//0.01//0.03				// Genauigkeit der Positionierung 1536 (Deadband) [mm]
	,(unsigned short) 100		// 60 zulässige Diff. Steps zu Encoder [encoder_step]
	,(unsigned short) 500		// hold current wait time [millisecounds]
	,(unsigned short) 300//100//300			// wobbling wait time [millisecounds]
	,(unsigned short) 150			// wobbling wait time 1536[millisecounds]


	// Strom
	,(unsigned short) 100//100		//  run current [percent]
	,(unsigned short) 100		//  run current 1536 [percent]
	,(unsigned short) 50//65		// hold current [percent]
	,(unsigned short) 65//70		// hold current 1536 [percent]
	// Übersetzung
	,(float) DISKDRIVE_WAY_PRO_REV	// Weg pro Umdrehung in mm
	,(float) DISKDRIVE_ENC_IMP_PRO_REV	// EncoderAuflösung	0 -> Achse ohne Encoder
	,(float) (DISKDRIVE_MOT_IMP_PRO_REV / DISKDRIVE_WAY_PRO_REV)	// Motor-Impulse (µ-Step) pro mm	bzw. pro Umdr.
	,(float) (DISKDRIVE_ENC_IMP_PRO_REV / DISKDRIVE_WAY_PRO_REV)	// Encoder-Impulse pro mm
//	unsigned short usFullStepPerRev;	// Vollschritte pro Motorumdrehung
//	unsigned short usMicroStepPerFullStep;	//Anzahl der Mikroschritte pro Vollschritt
//	float fMicroStepProRev;			// Microschritte pro Umdrehung
	//void (*Direction)(unsigned short usDirection);
	,(unsigned short) HM_SLEW_POSITIVE_CREEP_NEGATIVE
	,(unsigned short) HM_SLEW_NEGATIVE_CREEP_POSITIVE	// inverses homig
	,(short) 1
};

const udtIMSAxisFixedParameter FixZ_E1_Drive = {
	(unsigned short) AXIS_ID_Z_E1_DRIVE
	,(char) AXIS_ADR_Z_E1_DRIVE	//Adress;
	,(unsigned short) eZ_E1_Axis			// MaskenBit
	,(float) 0.0				//Offset
	,(float) 0.0				// SetHomingPosition
	,(float) 1.0				// Differenz for slomo restore position handling
	// Geschwindigkeiten
	//,(float) VI_ZDRIVE_STEP_PER_SEC / (ZDRIVE_MOT_IMP_PRO_REV / ZDRIVE_WAY_PRO_REV)			//[mm/s]
	,(float) 1.0			 	// initial speed [mm/s]
	,(float) 130.0//140.0	 	// Endspeed negative direction [mm/s]
	,(float) 130.0//140.0	 	// Endspeed positive direction [mm/s]
	,(float) 70.0		 	 	// Endspeed homing [mm/s]
,(float) 110.0			 	// Endspeed 1536 [mm/s]
	,(float) 500.0//700.0//1000.0		// Acceleration negative direction [mm/s*s]
	,(float) 500.0//700.0//1000.0		// Acceleration positive direction [mm/s*s]
	,(float) 500.0				// Acceleration homing [mm/s*s]
	,(float) 500.0				// Acceleration slew [mm/s*s]
,(float) 250.0				// Acceleration 1536 [mm/s*s]

	// Grenzwerte
	,(float) 1.0				// min. Acceleration [mm/s*s]
	,(float) 1000.0				// max. Acceleration [mm/s*s]
	,(float) 300.0				// max. 1536 Acceleration [mm/s*s]
	,(float) 1.0				// min. Endspeed [mm/s*s]
	,(float) 140.0				// max. Endspeed [mm/s*s]
	,(float) 100.0				// max. 1536 Endspeed [mm/s*s]
	,(float) -281.0//-260.0//-240.0		// min. drive range [mm] eröht da Z-Achse 15mm länger ist
	,(float) 0.1				// max. drive range [mm]

	// sonstiges
	,(float) 0.1				// Lose (mech. Spiel) [mm]
	,(float) 0.01//0.011//0.03				// Genauigkeit der Positionierung(Deadband) [mm]
	,(float) 0.01//0.011//0.03				// Genauigkeit der Positionierung 1536 (Deadband) [mm]
	,(unsigned short) 120		// 80 zulässige Diff. Steps zu Encoder [encoder_step]
	,(unsigned short) 500		// hold current wait time [millisecounds]
	,(unsigned short) 50			// wobbling wait time [millisecounds]
	,(unsigned short) 50			// wobbling wait time 1536[millisecounds]

	// Strom
	,(unsigned short) 100		//  run current [percent]
	,(unsigned short) 100		//  run current 1536 [percent]
	,(unsigned short) 50		// hold current [percent]
	,(unsigned short) 50		// hold current [percent]
	// Übersetzung
	,(float) ZDRIVE_WAY_PRO_REV	// Weg pro Umdrehung in mm
	,(float) ZDRIVE_ENC_IMP_PRO_REV	// EncoderAuflösung	0 -> Achse ohne Encoder
	,(float) (ZDRIVE_MOT_IMP_PRO_REV / ZDRIVE_WAY_PRO_REV)	// Motor-Impulse (µ-Step) pro mm	bzw. pro Umdr.
	,(float) (ZDRIVE_ENC_IMP_PRO_REV / ZDRIVE_WAY_PRO_REV)	// Encoder-Impulse pro mm
//	unsigned short usFullStepPerRev;	// Vollschritte pro Motorumdrehung
//	unsigned short usMicroStepPerFullStep;	//Anzahl der Mikroschritte pro Vollschritt
//	float fMicroStepProRev;			// Microschritte pro Umdrehung
	//void (*Direction)(unsigned short usDirection);
	,(unsigned short) HM_SLEW_POSITIVE_CREEP_NEGATIVE
	,(unsigned short) HM_SLEW_NEGATIVE_CREEP_POSITIVE	// inverses homig
	,(short) 1
};
const udtIMSAxisFixedParameter FixZ_E2_Drive = {
	(unsigned short) AXIS_ID_Z_E2_DRIVE
	,(char) AXIS_ADR_Z_E2_DRIVE	//Adress;
	,(unsigned short) eZ_E2_Axis			// MaskenBit
	,(float) 0.0				//Offset
	,(float) 0.0				// SetHomingPosition
	,(float) 1.0				// Differenz for slomo restore position handling
	// Geschwindigkeiten
	//,(float) VI_ZDRIVE_STEP_PER_SEC / (ZDRIVE_MOT_IMP_PRO_REV / ZDRIVE_WAY_PRO_REV)			//[mm/s]
	,(float) 1.0			 	// initial speed [mm/s]
	,(float) 130.0//140.0	 	// Endspeed negative direction [mm/s]
	,(float) 130.0//140.0	 	// Endspeed positive direction [mm/s]
	,(float) 70.0		 	 	// Endspeed homing [mm/s]
,(float) 110.0			 	// Endspeed 1536 [mm/s]
	,(float) 500.0//700.0//1000.0		// Acceleration negative direction [mm/s*s]
	,(float) 500.0//700.0//1000.0		// Acceleration positive direction [mm/s*s]
	,(float) 500.0				// Acceleration homing [mm/s*s]
	,(float) 500.0				// Acceleration slew [mm/s*s]
,(float) 250.0				// Acceleration 1536 [mm/s*s]

	// Grenzwerte
	,(float) 1.0				// min. Acceleration [mm/s*s]
	,(float) 1000.0				// max. Acceleration [mm/s*s]
	,(float) 300.0				// max. 1536 Acceleration [mm/s*s]
	,(float) 1.0				// min. Endspeed [mm/s*s]
	,(float) 140.0				// max. Endspeed [mm/s*s]
	,(float) 100.0				// max. 1536 Endspeed [mm/s*s]
	,(float) -281.0//-260.0//-240.0		// min. drive range [mm] eröht da Z-Achse 15mm länger ist
	,(float) 0.1				// max. drive range [mm]

	// sonstiges
	,(float) 0.1				// Lose (mech. Spiel) [mm]
	,(float) 0.01//0.011//0.03				// Genauigkeit der Positionierung(Deadband) [mm]
	,(float) 0.01//0.011//0.03				// Genauigkeit der Positionierung 1536 (Deadband) [mm]
	,(unsigned short) 120		// 80 zulässige Diff. Steps zu Encoder [encoder_step]
	,(unsigned short) 500		// hold current wait time [millisecounds]
	,(unsigned short) 50			// wobbling wait time [millisecounds]
	,(unsigned short) 50			// wobbling wait time 1536[millisecounds]

	// Strom
	,(unsigned short) 100		//  run current [percent]
	,(unsigned short) 100		//  run current 1536 [percent]
	,(unsigned short) 50		// hold current [percent]
	,(unsigned short) 50		// hold current [percent]
	// Übersetzung
	,(float) ZDRIVE_WAY_PRO_REV	// Weg pro Umdrehung in mm
	,(float) ZDRIVE_ENC_IMP_PRO_REV	// EncoderAuflösung	0 -> Achse ohne Encoder
	,(float) (ZDRIVE_MOT_IMP_PRO_REV / ZDRIVE_WAY_PRO_REV)	// Motor-Impulse (µ-Step) pro mm	bzw. pro Umdr.
	,(float) (ZDRIVE_ENC_IMP_PRO_REV / ZDRIVE_WAY_PRO_REV)	// Encoder-Impulse pro mm
//	unsigned short usFullStepPerRev;	// Vollschritte pro Motorumdrehung
//	unsigned short usMicroStepPerFullStep;	//Anzahl der Mikroschritte pro Vollschritt
//	float fMicroStepProRev;			// Microschritte pro Umdrehung
	//void (*Direction)(unsigned short usDirection);
	,(unsigned short) HM_SLEW_POSITIVE_CREEP_NEGATIVE
	,(unsigned short) HM_SLEW_NEGATIVE_CREEP_POSITIVE	// inverses homig
	,(short) 1
};
const udtIMSAxisFixedParameter FixZ_E3_Drive = {
	(unsigned short) AXIS_ID_Z_E3_DRIVE
	,(char) AXIS_ADR_Z_E3_DRIVE	//Adress;
	,(unsigned short) eZ_E3_Axis			// MaskenBit
	,(float) 0.0				//Offset
	,(float) 0.0				// SetHomingPosition
	,(float) 1.0				// Differenz for slomo restore position handling
	// Geschwindigkeiten
	//,(float) VI_ZDRIVE_STEP_PER_SEC / (ZDRIVE_MOT_IMP_PRO_REV / ZDRIVE_WAY_PRO_REV)			//[mm/s]
	,(float) 1.0			 	// initial speed [mm/s]
	,(float) 130.0//140.0	 	// Endspeed negative direction [mm/s]
	,(float) 130.0//140.0	 	// Endspeed positive direction [mm/s]
	,(float) 70.0		 	 	// Endspeed homing [mm/s]
,(float) 110.0			 	// Endspeed 1536 [mm/s]
	,(float) 500.0//700.0//1000.0		// Acceleration negative direction [mm/s*s]
	,(float) 500.0//700.0//1000.0		// Acceleration positive direction [mm/s*s]
	,(float) 500.0				// Acceleration homing [mm/s*s]
	,(float) 500.0				// Acceleration slew [mm/s*s]
,(float) 250.0				// Acceleration 1536 [mm/s*s]

	// Grenzwerte
	,(float) 1.0				// min. Acceleration [mm/s*s]
	,(float) 1000.0				// max. Acceleration [mm/s*s]
	,(float) 300.0				// max. 1536 Acceleration [mm/s*s]
	,(float) 1.0				// min. Endspeed [mm/s*s]
	,(float) 140.0				// max. Endspeed [mm/s*s]
	,(float) 100.0				// max. 1536 Endspeed [mm/s*s]
	,(float) -281.0//-260.0//-240.0		// min. drive range [mm] eröht da Z-Achse 15mm länger ist
	,(float) 0.1				// max. drive range [mm]

	// sonstiges
	,(float) 0.1				// Lose (mech. Spiel) [mm]
	,(float) 0.01//0.011//0.03				// Genauigkeit der Positionierung(Deadband) [mm]
	,(float) 0.01//0.011//0.03				// Genauigkeit der Positionierung 1536 (Deadband) [mm]
	,(unsigned short) 120		// 80 zulässige Diff. Steps zu Encoder [encoder_step]
	,(unsigned short) 500		// hold current wait time [millisecounds]
	,(unsigned short) 50			// wobbling wait time [millisecounds]
	,(unsigned short) 50			// wobbling wait time 1536[millisecounds]

	// Strom
	,(unsigned short) 100		//  run current [percent]
	,(unsigned short) 100		//  run current 1536 [percent]
	,(unsigned short) 50		// hold current [percent]
	,(unsigned short) 50		// hold current [percent]
	// Übersetzung
	,(float) ZDRIVE_WAY_PRO_REV	// Weg pro Umdrehung in mm
	,(float) ZDRIVE_ENC_IMP_PRO_REV	// EncoderAuflösung	0 -> Achse ohne Encoder
	,(float) (ZDRIVE_MOT_IMP_PRO_REV / ZDRIVE_WAY_PRO_REV)	// Motor-Impulse (µ-Step) pro mm	bzw. pro Umdr.
	,(float) (ZDRIVE_ENC_IMP_PRO_REV / ZDRIVE_WAY_PRO_REV)	// Encoder-Impulse pro mm
//	unsigned short usFullStepPerRev;	// Vollschritte pro Motorumdrehung
//	unsigned short usMicroStepPerFullStep;	//Anzahl der Mikroschritte pro Vollschritt
//	float fMicroStepProRev;			// Microschritte pro Umdrehung
	//void (*Direction)(unsigned short usDirection);
	,(unsigned short) HM_SLEW_POSITIVE_CREEP_NEGATIVE
	,(unsigned short) HM_SLEW_NEGATIVE_CREEP_POSITIVE	// inverses homig
	,(short) 1
};
const udtIMSAxisFixedParameter FixZ_E4_Drive = {
	(unsigned short) AXIS_ID_Z_E4_DRIVE
	,(char) AXIS_ADR_Z_E4_DRIVE	//Adress;
	,(unsigned short) eZ_E4_Axis			// MaskenBit
	,(float) 0.0				//Offset
	,(float) 0.0				// SetHomingPosition
	,(float) 1.0				// Differenz for slomo restore position handling
	// Geschwindigkeiten
	//,(float) VI_ZDRIVE_STEP_PER_SEC / (ZDRIVE_MOT_IMP_PRO_REV / ZDRIVE_WAY_PRO_REV)			//[mm/s]
	,(float) 1.0			 	// initial speed [mm/s]
	,(float) 130.0//140.0	 	// Endspeed negative direction [mm/s]
	,(float) 130.0//140.0	 	// Endspeed positive direction [mm/s]
	,(float) 70.0		 	 	// Endspeed homing [mm/s]
,(float) 110.0			 	// Endspeed 1536 [mm/s]
	,(float) 500.0//700.0//1000.0		// Acceleration negative direction [mm/s*s]
	,(float) 500.0//700.0//1000.0		// Acceleration positive direction [mm/s*s]
	,(float) 500.0				// Acceleration homing [mm/s*s]
	,(float) 500.0				// Acceleration slew [mm/s*s]
,(float) 250.0				// Acceleration 1536 [mm/s*s]

	// Grenzwerte
	,(float) 1.0				// min. Acceleration [mm/s*s]
	,(float) 1000.0				// max. Acceleration [mm/s*s]
	,(float) 300.0				// max. 1536 Acceleration [mm/s*s]
	,(float) 1.0				// min. Endspeed [mm/s*s]
	,(float) 140.0				// max. Endspeed [mm/s*s]
	,(float) 100.0				// max. 1536 Endspeed [mm/s*s]
	,(float) -281.0//-260.0//-240.0		// min. drive range [mm] eröht da Z-Achse 15mm länger ist
	,(float) 0.1				// max. drive range [mm]

	// sonstiges
	,(float) 0.1				// Lose (mech. Spiel) [mm]
	,(float) 0.01//0.011//0.03				// Genauigkeit der Positionierung(Deadband) [mm]
	,(float) 0.01//0.011//0.03				// Genauigkeit der Positionierung 1536 (Deadband) [mm]
	,(unsigned short) 120		// 80 zulässige Diff. Steps zu Encoder [encoder_step]
	,(unsigned short) 500		// hold current wait time [millisecounds]
	,(unsigned short) 50			// wobbling wait time [millisecounds]
	,(unsigned short) 50			// wobbling wait time 1536[millisecounds]

	// Strom
	,(unsigned short) 100		//  run current [percent]
	,(unsigned short) 100		//  run current 1536 [percent]
	,(unsigned short) 50		// hold current [percent]
	,(unsigned short) 50		// hold current [percent]
	// Übersetzung
	,(float) ZDRIVE_WAY_PRO_REV	// Weg pro Umdrehung in mm
	,(float) ZDRIVE_ENC_IMP_PRO_REV	// EncoderAuflösung	0 -> Achse ohne Encoder
	,(float) (ZDRIVE_MOT_IMP_PRO_REV / ZDRIVE_WAY_PRO_REV)	// Motor-Impulse (µ-Step) pro mm	bzw. pro Umdr.
	,(float) (ZDRIVE_ENC_IMP_PRO_REV / ZDRIVE_WAY_PRO_REV)	// Encoder-Impulse pro mm
//	unsigned short usFullStepPerRev;	// Vollschritte pro Motorumdrehung
//	unsigned short usMicroStepPerFullStep;	//Anzahl der Mikroschritte pro Vollschritt
//	float fMicroStepProRev;			// Microschritte pro Umdrehung
	//void (*Direction)(unsigned short usDirection);
	,(unsigned short) HM_SLEW_POSITIVE_CREEP_NEGATIVE
	,(unsigned short) HM_SLEW_NEGATIVE_CREEP_POSITIVE	// inverses homig
	,(short) 1
};
const udtIMSAxisFixedParameter FixZ_E5_Drive = {
	(unsigned short) AXIS_ID_Z_E5_DRIVE
	,(char) AXIS_ADR_Z_E5_DRIVE	//Adress;
	,(unsigned short) eZ_E5_Axis			// MaskenBit
	,(float) 0.0				//Offset
	,(float) 0.0				// SetHomingPosition
	,(float) 1.0				// Differenz for slomo restore position handling
	// Geschwindigkeiten
	//,(float) VI_ZDRIVE_STEP_PER_SEC / (ZDRIVE_MOT_IMP_PRO_REV / ZDRIVE_WAY_PRO_REV)			//[mm/s]
	,(float) 1.0			 	// initial speed [mm/s]
	,(float) 130.0//140.0	 	// Endspeed negative direction [mm/s]
	,(float) 130.0//140.0	 	// Endspeed positive direction [mm/s]
	,(float) 70.0		 	 	// Endspeed homing [mm/s]
,(float) 110.0			 	// Endspeed 1536 [mm/s]
	,(float) 500.0//700.0//1000.0		// Acceleration negative direction [mm/s*s]
	,(float) 500.0//700.0//1000.0		// Acceleration positive direction [mm/s*s]
	,(float) 500.0				// Acceleration homing [mm/s*s]
	,(float) 500.0				// Acceleration slew [mm/s*s]
,(float) 250.0				// Acceleration 1536 [mm/s*s]

	// Grenzwerte
	,(float) 1.0				// min. Acceleration [mm/s*s]
	,(float) 1000.0				// max. Acceleration [mm/s*s]
	,(float) 300.0				// max. 1536 Acceleration [mm/s*s]
	,(float) 1.0				// min. Endspeed [mm/s*s]
	,(float) 140.0				// max. Endspeed [mm/s*s]
	,(float) 100.0				// max. 1536 Endspeed [mm/s*s]
	,(float) -281.0//-260.0//-240.0		// min. drive range [mm] eröht da Z-Achse 15mm länger ist
	,(float) 0.1				// max. drive range [mm]

	// sonstiges
	,(float) 0.1				// Lose (mech. Spiel) [mm]
	,(float) 0.01//0.011//0.03				// Genauigkeit der Positionierung(Deadband) [mm]
	,(float) 0.01//0.011//0.03				// Genauigkeit der Positionierung 1536 (Deadband) [mm]
	,(unsigned short) 120		// 80 zulässige Diff. Steps zu Encoder [encoder_step]
	,(unsigned short) 500		// hold current wait time [millisecounds]
	,(unsigned short) 50			// wobbling wait time [millisecounds]
	,(unsigned short) 50			// wobbling wait time 1536[millisecounds]

	// Strom
	,(unsigned short) 100		//  run current [percent]
	,(unsigned short) 100		//  run current 1536 [percent]
	,(unsigned short) 50		// hold current [percent]
	,(unsigned short) 50		// hold current [percent]
	// Übersetzung
	,(float) ZDRIVE_WAY_PRO_REV	// Weg pro Umdrehung in mm
	,(float) ZDRIVE_ENC_IMP_PRO_REV	// EncoderAuflösung	0 -> Achse ohne Encoder
	,(float) (ZDRIVE_MOT_IMP_PRO_REV / ZDRIVE_WAY_PRO_REV)	// Motor-Impulse (µ-Step) pro mm	bzw. pro Umdr.
	,(float) (ZDRIVE_ENC_IMP_PRO_REV / ZDRIVE_WAY_PRO_REV)	// Encoder-Impulse pro mm
//	unsigned short usFullStepPerRev;	// Vollschritte pro Motorumdrehung
//	unsigned short usMicroStepPerFullStep;	//Anzahl der Mikroschritte pro Vollschritt
//	float fMicroStepProRev;			// Microschritte pro Umdrehung
	//void (*Direction)(unsigned short usDirection);
	,(unsigned short) HM_SLEW_POSITIVE_CREEP_NEGATIVE
	,(unsigned short) HM_SLEW_NEGATIVE_CREEP_POSITIVE	// inverses homig
	,(short) 1
};
#endif
#ifdef _CF_FELIX
udtIMSAxisChangedParameter FrontCarriage;
udtIMSAxisChangedParameter RearCarriage;
udtIMSAxisChangedParameter BottomCarriage;
udtIMSAxisChangedParameter XDrive;
udtIMSAxisChangedParameter ZDrive;
#endif
#ifdef _CF_PENTA_DISK
udtIMSAxisChangedParameter Z_E1_Drive;
udtIMSAxisChangedParameter Z_E2_Drive;
udtIMSAxisChangedParameter Z_E3_Drive;
udtIMSAxisChangedParameter DiskDrive;
udtIMSAxisChangedParameter Z_E4_Drive;
udtIMSAxisChangedParameter Z_E5_Drive;
#endif

// Reihenfolge muss zu pIMSChangedPara gleich sein
const udtIMSAxisFixedParameter *pIMSFixedPara[MAX_AXES];
/*const udtIMSAxisFixedParameter *pIMSFixedPara[] ={
	&FixXDrive
	,&FixFrontCarriage
	,&FixRearCarriage
	,&FixBottomCarriage
	,&FixZDrive
};*/
// Reihenfolge muss zu pIMSFixedPara gleich sein
udtIMSAxisChangedParameter *pIMSChangedPara[MAX_AXES];
/*udtIMSAxisChangedParameter *pIMSChangedPara[] ={
	&XDrive
	,&FrontCarriage
	,&RearCarriage
	,&BottomCarriage
	,&ZDrive
};*/
// Reihenfolge muss zu pIMSFixedPara und pIMSChangedPara gleich sein
/*unsigned short AxesPresent[MAX_AXES] ={
// 0 = keine Achse vorhanden
 AXIS_ADR_X_DRIVE
 ,AXIS_ADR_FRONT_CARRIAGE
 ,AXIS_ADR_REAR_CARRIAGE
 ,AXIS_ADR_BOTTOM_CARRIAGE
 ,AXIS_ADR_Z_DRIVE
};*/
udtIMSAxes g_FelixAxes[MAX_AXES];
udtOffset g_AxisOffset[MAX_AXES + AXES_RESERVE];
udtOffset g_AxisOffsetDefault[MAX_AXES + AXES_RESERVE];
udtDeviceConfig g_DeviceConfig;
udtMinMaxWay g_AxisMinMaxWay[MAX_AXES + AXES_RESERVE];
udtSensorConfig g_SensorConfig;
udtPortConfig g_PortConfig;
udtEquipmentConfig g_EquipmentConfig;
udtEstopCounter g_EstopCounter;
udtEstopWait g_EstopWait;
udtConfigStallExeption g_ConfigStallExeption[MAX_AXES + AXES_RESERVE];
udtCounterStallExeption g_CounterStallExeption[MAX_AXES + AXES_RESERVE];
udtRelativeDeltaWay g_SearchAdapterRelativeDeltaWay;
udtApplicationIndexSpd g_ApplicationSpd[MAX_APPLICATION_SPD];

//****************************************************************************
// private data
//****************************************************************************

//****************************************************************************
// private functions
//****************************************************************************

//****************************************************************************
// public functions
//****************************************************************************
/*void aptest(void)
{unsigned short a;

	for(a=0;a<=4;a++)
	{
		printf("idx:%u adr:%u\r", a, pIMSFixedPara[a]->Adress);fflush(stdout);
	}
}
*/
// Aus maske AchsenID generieren
unsigned short GetAxisIDFromMask(unsigned short mask, unsigned short AxisBit)
{
	unsigned short cnt;
	for(cnt = 0; cnt < MAX_AXES; cnt++)
	{
		WDO();
		if((mask & AxisBit & g_FelixAxes[cnt].AxisBit) && g_FelixAxes[cnt].Enable)
		{
			return g_FelixAxes[cnt].AxisID;
		}
	}
	return 0;
}
// aus AchsenIndex index für datenzugriff generieren
short GetAxisIndex(const unsigned char ID)
{
	short cnt;
	short ReturnCnt = -1;

	if(ID == DOORLIFT_ID)
	{
		ReturnCnt = DOORLIFT_ID;
	}
	else
	{
		for(cnt = 0; cnt < MAX_AXES; cnt++)
		{
			if((ID == g_FelixAxes[cnt].AxisID) && g_FelixAxes[cnt].Enable)
			{
				//return cnt;
				ReturnCnt = cnt;
				break;
			}
		}
		//return -1;
	}
	return ReturnCnt;
}
#ifdef _CF_FELIX
void FelixAxesGenerate(void)
{

	g_FelixAxes[0].AxisID = FixXDrive.ID;
	g_FelixAxes[0].AxisAdr = FixXDrive.BusAdr;
	g_FelixAxes[0].AxisBit = FixXDrive.AxisBit;
	g_FelixAxes[0].ParaIDX = 0;
	g_FelixAxes[0].Enable = 1;	//0 = keine Achse vorhanden
	g_FelixAxes[0].AxisWithBrake = 0;
	g_FelixAxes[0].AxisFullRotary = 0;
	g_SetAxes_LastErrorAxisHWForEver(0, 0);
	g_SetAxes_ErrorAxisHW(0, 0);
	g_SetAxes_ErrorAxisCyBio(0, errNone);
	g_SetAxes_ErrorFlag(0, 0);
	g_SetAxes_HomingFlag(0, FLG_HOMING_FAILURE);
//	g_SetAxes_StallFlag(0, 0);
	g_SetAxes_Running(0, STATE_READY);
	g_SetAxes_ErrorAxisHWStall(0, 0);
	pIMSFixedPara[0] = &FixXDrive;
	pIMSChangedPara[0] = &XDrive;
	g_AxisOffset[0].AxisOffsetFromHomingSwitch = FixXDrive.fOffset;
	g_AxisOffsetDefault[0].AxisOffsetFromHomingSwitch = FixXDrive.fOffset;
	g_AxisMinMaxWay[0].MinWay = FixXDrive.fMinWay;
	g_AxisMinMaxWay[0].MaxWay = FixXDrive.fMaxWay;
	g_ConfigStallExeption[0].Enable = 0;
	g_ConfigStallExeption[0].Up_DirA_Counter = 1;
	g_ConfigStallExeption[0].Down_DirB_Counter = 1;
	g_ConfigStallExeption[0].WaitTime = 100;
	g_CounterStallExeption[0].ExeptionLifeTimeCounter = 0;

	g_FelixAxes[1].AxisID = FixFrontCarriage.ID;
	g_FelixAxes[1].AxisAdr = FixFrontCarriage.BusAdr;
	g_FelixAxes[1].AxisBit = FixFrontCarriage.AxisBit;
	g_FelixAxes[1].ParaIDX = 1;
	g_FelixAxes[1].Enable = 1;
	g_FelixAxes[1].AxisWithBrake = 0;
	g_FelixAxes[1].AxisFullRotary = 0;
	g_SetAxes_LastErrorAxisHWForEver(1, 0);
	g_SetAxes_ErrorAxisHW(1, 0);
	g_SetAxes_ErrorAxisCyBio(1, errNone);
	g_SetAxes_ErrorFlag(1, 0);
	g_SetAxes_HomingFlag(1, FLG_HOMING_FAILURE);
//	g_SetAxes_StallFlag(1, 0);
	g_SetAxes_Running(1, STATE_READY);
	g_SetAxes_ErrorAxisHWStall(1, 0);
	pIMSFixedPara[1] = &FixFrontCarriage;
	pIMSChangedPara[1] = &FrontCarriage;
	g_AxisOffset[1].AxisOffsetFromHomingSwitch = FixFrontCarriage.fOffset;
	g_AxisOffsetDefault[1].AxisOffsetFromHomingSwitch = FixFrontCarriage.fOffset;
	g_AxisMinMaxWay[1].MinWay = FixFrontCarriage.fMinWay;
	g_AxisMinMaxWay[1].MaxWay = FixFrontCarriage.fMaxWay;
	g_ConfigStallExeption[1].Enable = 0;
	g_ConfigStallExeption[1].Up_DirA_Counter = 1;
	g_ConfigStallExeption[1].Down_DirB_Counter = 1;
	g_ConfigStallExeption[1].WaitTime = 100;
	g_CounterStallExeption[1].ExeptionLifeTimeCounter = 0;

	g_FelixAxes[2].AxisID = FixRearCarriage.ID;
	g_FelixAxes[2].AxisAdr = FixRearCarriage.BusAdr;
	g_FelixAxes[2].AxisBit = FixRearCarriage.AxisBit;
	g_FelixAxes[2].ParaIDX = 2;
	g_FelixAxes[2].Enable = 1;
	g_FelixAxes[2].AxisWithBrake = 0;
	g_FelixAxes[2].AxisFullRotary = 0;
	g_SetAxes_LastErrorAxisHWForEver(2, 0);
	g_SetAxes_ErrorAxisHW(2, 0);
	g_SetAxes_ErrorAxisCyBio(2, errNone);
	g_SetAxes_ErrorFlag(2, 0);
	g_SetAxes_HomingFlag(2, FLG_HOMING_FAILURE);
//	g_SetAxes_StallFlag(2, 0);
	g_SetAxes_Running(2, STATE_READY);
	g_SetAxes_ErrorAxisHWStall(2, 0);
	pIMSFixedPara[2] = &FixRearCarriage;
	pIMSChangedPara[2] = &RearCarriage;
	g_AxisOffset[2].AxisOffsetFromHomingSwitch = FixRearCarriage.fOffset;
	g_AxisOffsetDefault[2].AxisOffsetFromHomingSwitch = FixRearCarriage.fOffset;
	g_AxisMinMaxWay[2].MinWay = FixRearCarriage.fMinWay;
	g_AxisMinMaxWay[2].MaxWay = FixRearCarriage.fMaxWay;
	g_ConfigStallExeption[2].Enable = 0;
	g_ConfigStallExeption[2].Up_DirA_Counter = 1;
	g_ConfigStallExeption[2].Down_DirB_Counter = 1;
	g_ConfigStallExeption[2].WaitTime = 100;
	g_CounterStallExeption[2].ExeptionLifeTimeCounter = 0;

	g_FelixAxes[3].AxisID = FixBottomCarriage.ID;
	g_FelixAxes[3].AxisAdr = FixBottomCarriage.BusAdr;
	g_FelixAxes[3].AxisBit = FixBottomCarriage.AxisBit;
	g_FelixAxes[3].ParaIDX = 3;
	g_FelixAxes[3].Enable = 1;
	g_FelixAxes[3].AxisWithBrake = 0;
	g_FelixAxes[3].AxisFullRotary = 0;
	g_SetAxes_LastErrorAxisHWForEver(3, 0);
	g_SetAxes_ErrorAxisHW(3, 0);
	g_SetAxes_ErrorAxisCyBio(3, errNone);
	g_SetAxes_ErrorFlag(3, 0);
	g_SetAxes_HomingFlag(3, FLG_HOMING_FAILURE);
//	g_SetAxes_StallFlag(3, 0);
	g_SetAxes_Running(3, STATE_READY);
	g_SetAxes_ErrorAxisHWStall(3, 0);
	pIMSFixedPara[3] = &FixBottomCarriage;
	pIMSChangedPara[3] = &BottomCarriage;
	g_AxisOffset[3].AxisOffsetFromHomingSwitch = FixBottomCarriage.fOffset;
	g_AxisOffsetDefault[3].AxisOffsetFromHomingSwitch = FixBottomCarriage.fOffset;
	g_AxisMinMaxWay[3].MinWay = FixBottomCarriage.fMinWay;
	g_AxisMinMaxWay[3].MaxWay = FixBottomCarriage.fMaxWay;
	g_ConfigStallExeption[3].Enable = 0;
	g_ConfigStallExeption[3].Up_DirA_Counter = 1;
	g_ConfigStallExeption[3].Down_DirB_Counter = 1;
	g_ConfigStallExeption[3].WaitTime = 100;
	g_CounterStallExeption[3].ExeptionLifeTimeCounter = 0;

	g_FelixAxes[4].AxisID = FixZDrive.ID;
	g_FelixAxes[4].AxisAdr = FixZDrive.BusAdr;
	g_FelixAxes[4].AxisBit = FixZDrive.AxisBit;
	g_FelixAxes[4].ParaIDX = 4;
	g_FelixAxes[4].Enable = 1;
	g_FelixAxes[4].AxisWithBrake = 1;
	g_FelixAxes[4].AxisFullRotary = 0;
	g_SetAxes_LastErrorAxisHWForEver(4, 0);
	g_SetAxes_ErrorAxisHW(4, 0);
	g_SetAxes_ErrorAxisCyBio(4, errNone);
	g_SetAxes_ErrorFlag(4, 0);
	g_SetAxes_HomingFlag(4, FLG_HOMING_FAILURE);
//	g_SetAxes_StallFlag(4, 0);
	g_SetAxes_Running(4, STATE_READY);
	g_SetAxes_ErrorAxisHWStall(4, 0);
	pIMSFixedPara[4] = &FixZDrive;
	pIMSChangedPara[4] = &ZDrive;
	g_AxisOffset[4].AxisOffsetFromHomingSwitch = FixZDrive.fOffset;
	g_AxisOffsetDefault[4].AxisOffsetFromHomingSwitch = FixZDrive.fOffset;
	g_AxisMinMaxWay[4].MinWay = FixZDrive.fMinWay;
	g_AxisMinMaxWay[4].MaxWay = FixZDrive.fMaxWay;
	g_ConfigStallExeption[4].Enable = 1;
	g_ConfigStallExeption[4].Up_DirA_Counter = 5;
	g_ConfigStallExeption[4].Down_DirB_Counter = 1;
	g_ConfigStallExeption[4].WaitTime = 100;
	g_CounterStallExeption[4].ExeptionLifeTimeCounter = 0;
}
#endif
#ifdef _CF_PENTA_DISK
void FelixAxesGenerate(void)
{
	g_FelixAxes[PARA_IDX_DISK_DRIVE].AxisID = FixDiskDrive.ID;
	g_FelixAxes[PARA_IDX_DISK_DRIVE].AxisAdr = FixDiskDrive.BusAdr;
	g_FelixAxes[PARA_IDX_DISK_DRIVE].AxisBit = FixDiskDrive.AxisBit;
	g_FelixAxes[PARA_IDX_DISK_DRIVE].ParaIDX = PARA_IDX_DISK_DRIVE;
	g_FelixAxes[PARA_IDX_DISK_DRIVE].Enable = 1;	//0 = keine Achse vorhanden
	g_FelixAxes[PARA_IDX_DISK_DRIVE].AxisWithBrake = 0;
	g_FelixAxes[PARA_IDX_DISK_DRIVE].AxisFullRotary = 1;
	g_SetAxes_LastErrorAxisHWForEver(0, 0);
	g_SetAxes_ErrorAxisHW(PARA_IDX_DISK_DRIVE, 0);
	g_SetAxes_ErrorAxisCyBio(PARA_IDX_DISK_DRIVE, errNone);
	g_SetAxes_ErrorFlag(PARA_IDX_DISK_DRIVE, 0);
	g_SetAxes_HomingFlag(PARA_IDX_DISK_DRIVE, FLG_HOMING_FAILURE);
//	g_SetAxes_StallFlag(0, 0);
	g_SetAxes_Running(PARA_IDX_DISK_DRIVE, STATE_READY);
	g_SetAxes_ErrorAxisHWStall(PARA_IDX_DISK_DRIVE, 0);
	pIMSFixedPara[PARA_IDX_DISK_DRIVE] = &FixDiskDrive;
	pIMSChangedPara[PARA_IDX_DISK_DRIVE] = &DiskDrive;
	g_AxisOffset[PARA_IDX_DISK_DRIVE].AxisOffsetFromHomingSwitch = FixDiskDrive.fOffset;
	g_AxisOffsetDefault[PARA_IDX_DISK_DRIVE].AxisOffsetFromHomingSwitch = FixDiskDrive.fOffset;
	g_AxisMinMaxWay[PARA_IDX_DISK_DRIVE].MinWay = FixDiskDrive.fMinWay;
	g_AxisMinMaxWay[PARA_IDX_DISK_DRIVE].MaxWay = FixDiskDrive.fMaxWay;
	g_ConfigStallExeption[PARA_IDX_DISK_DRIVE].Enable = 0;
	g_ConfigStallExeption[PARA_IDX_DISK_DRIVE].Up_DirA_Counter = 1;
	g_ConfigStallExeption[PARA_IDX_DISK_DRIVE].Down_DirB_Counter = 1;
	g_ConfigStallExeption[PARA_IDX_DISK_DRIVE].WaitTime = 100;
	g_CounterStallExeption[PARA_IDX_DISK_DRIVE].ExeptionLifeTimeCounter = 0;

	g_FelixAxes[PARA_IDX_Z_E1_DRIVE].AxisID = FixZ_E1_Drive.ID;
	g_FelixAxes[PARA_IDX_Z_E1_DRIVE].AxisAdr = FixZ_E1_Drive.BusAdr;
	g_FelixAxes[PARA_IDX_Z_E1_DRIVE].AxisBit = FixZ_E1_Drive.AxisBit;
	g_FelixAxes[PARA_IDX_Z_E1_DRIVE].ParaIDX = PARA_IDX_Z_E1_DRIVE;
	g_FelixAxes[PARA_IDX_Z_E1_DRIVE].Enable = 1;
	g_FelixAxes[PARA_IDX_Z_E1_DRIVE].AxisWithBrake = 1;
	g_FelixAxes[PARA_IDX_Z_E1_DRIVE].AxisFullRotary = 0;
	g_SetAxes_LastErrorAxisHWForEver(PARA_IDX_Z_E1_DRIVE, 0);
	g_SetAxes_ErrorAxisHW(PARA_IDX_Z_E1_DRIVE, 0);
	g_SetAxes_ErrorAxisCyBio(PARA_IDX_Z_E1_DRIVE, errNone);
	g_SetAxes_ErrorFlag(PARA_IDX_Z_E1_DRIVE, 0);
	g_SetAxes_HomingFlag(PARA_IDX_Z_E1_DRIVE, FLG_HOMING_FAILURE);
//	g_SetAxes_StallFlag(PARA_IDX_Z_E1_DRIVE, 0);
	g_SetAxes_Running(PARA_IDX_Z_E1_DRIVE, STATE_READY);
	g_SetAxes_ErrorAxisHWStall(PARA_IDX_Z_E1_DRIVE, 0);
	pIMSFixedPara[PARA_IDX_Z_E1_DRIVE] = &FixZ_E1_Drive;
	pIMSChangedPara[PARA_IDX_Z_E1_DRIVE] = &Z_E1_Drive;
	g_AxisOffset[PARA_IDX_Z_E1_DRIVE].AxisOffsetFromHomingSwitch = FixZ_E1_Drive.fOffset;
	g_AxisOffsetDefault[PARA_IDX_Z_E1_DRIVE].AxisOffsetFromHomingSwitch = FixZ_E1_Drive.fOffset;
	g_AxisMinMaxWay[PARA_IDX_Z_E1_DRIVE].MinWay = FixZ_E1_Drive.fMinWay;
	g_AxisMinMaxWay[PARA_IDX_Z_E1_DRIVE].MaxWay = FixZ_E1_Drive.fMaxWay;
	g_ConfigStallExeption[PARA_IDX_Z_E1_DRIVE].StallExeption.Enable = 1;
	g_ConfigStallExeption[PARA_IDX_Z_E1_DRIVE].Up_DirA_Counter = 5;
	g_ConfigStallExeption[PARA_IDX_Z_E1_DRIVE].Down_DirB_Counter = 1;
	g_ConfigStallExeption[PARA_IDX_Z_E1_DRIVE].WaitTime = 100;
	g_CounterStallExeption[PARA_IDX_Z_E1_DRIVE].ExeptionLifeTimeCounter = 0;

	g_FelixAxes[PARA_IDX_Z_E2_DRIVE].AxisID = FixZ_E2_Drive.ID;
	g_FelixAxes[PARA_IDX_Z_E2_DRIVE].AxisAdr = FixZ_E2_Drive.BusAdr;
	g_FelixAxes[PARA_IDX_Z_E2_DRIVE].AxisBit = FixZ_E2_Drive.AxisBit;
	g_FelixAxes[PARA_IDX_Z_E2_DRIVE].ParaIDX = PARA_IDX_Z_E2_DRIVE;
	g_FelixAxes[PARA_IDX_Z_E2_DRIVE].Enable = 1;
	g_FelixAxes[PARA_IDX_Z_E2_DRIVE].AxisWithBrake = 1;
	g_FelixAxes[PARA_IDX_Z_E2_DRIVE].AxisFullRotary = 0;
	g_SetAxes_LastErrorAxisHWForEver(PARA_IDX_Z_E2_DRIVE, 0);
	g_SetAxes_ErrorAxisHW(PARA_IDX_Z_E2_DRIVE, 0);
	g_SetAxes_ErrorAxisCyBio(PARA_IDX_Z_E2_DRIVE, errNone);
	g_SetAxes_ErrorFlag(PARA_IDX_Z_E2_DRIVE, 0);
	g_SetAxes_HomingFlag(PARA_IDX_Z_E2_DRIVE, FLG_HOMING_FAILURE);
//	g_SetAxes_StallFlag(PARA_IDX_Z_E2_DRIVE, 0);
	g_SetAxes_Running(PARA_IDX_Z_E2_DRIVE, STATE_READY);
	g_SetAxes_ErrorAxisHWStall(PARA_IDX_Z_E2_DRIVE, 0);
	pIMSFixedPara[PARA_IDX_Z_E2_DRIVE] = &FixZ_E2_Drive;
	pIMSChangedPara[PARA_IDX_Z_E2_DRIVE] = &Z_E2_Drive;
	g_AxisOffset[PARA_IDX_Z_E2_DRIVE].AxisOffsetFromHomingSwitch = FixZ_E2_Drive.fOffset;
	g_AxisOffsetDefault[PARA_IDX_Z_E2_DRIVE].AxisOffsetFromHomingSwitch = FixZ_E2_Drive.fOffset;
	g_AxisMinMaxWay[PARA_IDX_Z_E2_DRIVE].MinWay = FixZ_E2_Drive.fMinWay;
	g_AxisMinMaxWay[PARA_IDX_Z_E2_DRIVE].MaxWay = FixZ_E2_Drive.fMaxWay;
	g_ConfigStallExeption[PARA_IDX_Z_E2_DRIVE].StallExeption.Enable = 1;
	g_ConfigStallExeption[PARA_IDX_Z_E2_DRIVE].Up_DirA_Counter = 5;
	g_ConfigStallExeption[PARA_IDX_Z_E2_DRIVE].Down_DirB_Counter = 1;
	g_ConfigStallExeption[PARA_IDX_Z_E2_DRIVE].WaitTime = 100;
	g_CounterStallExeption[PARA_IDX_Z_E2_DRIVE].ExeptionLifeTimeCounter = 0;

	g_FelixAxes[PARA_IDX_Z_E3_DRIVE].AxisID = FixZ_E3_Drive.ID;
	g_FelixAxes[PARA_IDX_Z_E3_DRIVE].AxisAdr = FixZ_E3_Drive.BusAdr;
	g_FelixAxes[PARA_IDX_Z_E3_DRIVE].AxisBit = FixZ_E3_Drive.AxisBit;
	g_FelixAxes[PARA_IDX_Z_E3_DRIVE].ParaIDX = PARA_IDX_Z_E3_DRIVE;
	g_FelixAxes[PARA_IDX_Z_E3_DRIVE].Enable = 1;
	g_FelixAxes[PARA_IDX_Z_E3_DRIVE].AxisWithBrake = 1;
	g_FelixAxes[PARA_IDX_Z_E3_DRIVE].AxisFullRotary = 0;
	g_SetAxes_LastErrorAxisHWForEver(PARA_IDX_Z_E3_DRIVE, 0);
	g_SetAxes_ErrorAxisHW(PARA_IDX_Z_E3_DRIVE, 0);
	g_SetAxes_ErrorAxisCyBio(PARA_IDX_Z_E3_DRIVE, errNone);
	g_SetAxes_ErrorFlag(PARA_IDX_Z_E3_DRIVE, 0);
	g_SetAxes_HomingFlag(PARA_IDX_Z_E3_DRIVE, FLG_HOMING_FAILURE);
//	g_SetAxes_StallFlag(PARA_IDX_Z_E3_DRIVE, 0);
	g_SetAxes_Running(PARA_IDX_Z_E3_DRIVE, STATE_READY);
	g_SetAxes_ErrorAxisHWStall(PARA_IDX_Z_E3_DRIVE, 0);
	pIMSFixedPara[PARA_IDX_Z_E3_DRIVE] = &FixZ_E3_Drive;
	pIMSChangedPara[PARA_IDX_Z_E3_DRIVE] = &Z_E3_Drive;
	g_AxisOffset[PARA_IDX_Z_E3_DRIVE].AxisOffsetFromHomingSwitch = FixZ_E3_Drive.fOffset;
	g_AxisOffsetDefault[PARA_IDX_Z_E3_DRIVE].AxisOffsetFromHomingSwitch = FixZ_E3_Drive.fOffset;
	g_AxisMinMaxWay[PARA_IDX_Z_E3_DRIVE].MinWay = FixZ_E3_Drive.fMinWay;
	g_AxisMinMaxWay[PARA_IDX_Z_E3_DRIVE].MaxWay = FixZ_E3_Drive.fMaxWay;
	g_ConfigStallExeption[PARA_IDX_Z_E3_DRIVE].StallExeption.Enable = 1;
	g_ConfigStallExeption[PARA_IDX_Z_E3_DRIVE].Up_DirA_Counter = 5;
	g_ConfigStallExeption[PARA_IDX_Z_E3_DRIVE].Down_DirB_Counter = 1;
	g_ConfigStallExeption[PARA_IDX_Z_E3_DRIVE].WaitTime = 100;
	g_CounterStallExeption[PARA_IDX_Z_E3_DRIVE].ExeptionLifeTimeCounter = 0;

	g_FelixAxes[PARA_IDX_Z_E4_DRIVE].AxisID = FixZ_E4_Drive.ID;
	g_FelixAxes[PARA_IDX_Z_E4_DRIVE].AxisAdr = FixZ_E4_Drive.BusAdr;
	g_FelixAxes[PARA_IDX_Z_E4_DRIVE].AxisBit = FixZ_E4_Drive.AxisBit;
	g_FelixAxes[PARA_IDX_Z_E4_DRIVE].ParaIDX = PARA_IDX_Z_E4_DRIVE;
	g_FelixAxes[PARA_IDX_Z_E4_DRIVE].Enable = 0;
	g_FelixAxes[PARA_IDX_Z_E4_DRIVE].AxisWithBrake = 1;
	g_FelixAxes[PARA_IDX_Z_E4_DRIVE].AxisFullRotary = 0;
	g_SetAxes_LastErrorAxisHWForEver(PARA_IDX_Z_E4_DRIVE, 0);
	g_SetAxes_ErrorAxisHW(PARA_IDX_Z_E4_DRIVE, 0);
	g_SetAxes_ErrorAxisCyBio(PARA_IDX_Z_E4_DRIVE, errNone);
	g_SetAxes_ErrorFlag(PARA_IDX_Z_E4_DRIVE, 0);
	g_SetAxes_HomingFlag(PARA_IDX_Z_E4_DRIVE, FLG_HOMING_FAILURE);
//	g_SetAxes_StallFlag(PARA_IDX_Z_E4_DRIVE, 0);
	g_SetAxes_Running(PARA_IDX_Z_E4_DRIVE, STATE_READY);
	g_SetAxes_ErrorAxisHWStall(PARA_IDX_Z_E4_DRIVE, 0);
	pIMSFixedPara[PARA_IDX_Z_E4_DRIVE] = &FixZ_E4_Drive;
	pIMSChangedPara[PARA_IDX_Z_E4_DRIVE] = &Z_E4_Drive;
	g_AxisOffset[PARA_IDX_Z_E4_DRIVE].AxisOffsetFromHomingSwitch = FixZ_E4_Drive.fOffset;
	g_AxisOffsetDefault[PARA_IDX_Z_E4_DRIVE].AxisOffsetFromHomingSwitch = FixZ_E4_Drive.fOffset;
	g_AxisMinMaxWay[PARA_IDX_Z_E4_DRIVE].MinWay = FixZ_E4_Drive.fMinWay;
	g_AxisMinMaxWay[PARA_IDX_Z_E4_DRIVE].MaxWay = FixZ_E4_Drive.fMaxWay;
	g_ConfigStallExeption[PARA_IDX_Z_E4_DRIVE].StallExeption.Enable = 1;
	g_ConfigStallExeption[PARA_IDX_Z_E4_DRIVE].Up_DirA_Counter = 5;
	g_ConfigStallExeption[PARA_IDX_Z_E4_DRIVE].Down_DirB_Counter = 1;
	g_ConfigStallExeption[PARA_IDX_Z_E4_DRIVE].WaitTime = 100;
	g_CounterStallExeption[PARA_IDX_Z_E4_DRIVE].ExeptionLifeTimeCounter = 0;

	g_FelixAxes[PARA_IDX_Z_E5_DRIVE].AxisID = FixZ_E5_Drive.ID;
	g_FelixAxes[PARA_IDX_Z_E5_DRIVE].AxisAdr = FixZ_E5_Drive.BusAdr;
	g_FelixAxes[PARA_IDX_Z_E5_DRIVE].AxisBit = FixZ_E5_Drive.AxisBit;
	g_FelixAxes[PARA_IDX_Z_E5_DRIVE].ParaIDX = PARA_IDX_Z_E5_DRIVE;
	g_FelixAxes[PARA_IDX_Z_E5_DRIVE].Enable = 0;
	g_FelixAxes[PARA_IDX_Z_E5_DRIVE].AxisWithBrake = 1;
	g_FelixAxes[PARA_IDX_Z_E5_DRIVE].AxisFullRotary = 0;
	g_SetAxes_LastErrorAxisHWForEver(PARA_IDX_Z_E5_DRIVE, 0);
	g_SetAxes_ErrorAxisHW(PARA_IDX_Z_E5_DRIVE, 0);
	g_SetAxes_ErrorAxisCyBio(PARA_IDX_Z_E5_DRIVE, errNone);
	g_SetAxes_ErrorFlag(PARA_IDX_Z_E5_DRIVE, 0);
	g_SetAxes_HomingFlag(PARA_IDX_Z_E5_DRIVE, FLG_HOMING_FAILURE);
//	g_SetAxes_StallFlag(PARA_IDX_Z_E5_DRIVE, 0);
	g_SetAxes_Running(PARA_IDX_Z_E5_DRIVE, STATE_READY);
	g_SetAxes_ErrorAxisHWStall(PARA_IDX_Z_E5_DRIVE, 0);
	pIMSFixedPara[PARA_IDX_Z_E5_DRIVE] = &FixZ_E5_Drive;
	pIMSChangedPara[PARA_IDX_Z_E5_DRIVE] = &Z_E5_Drive;
	g_AxisOffset[PARA_IDX_Z_E5_DRIVE].AxisOffsetFromHomingSwitch = FixZ_E5_Drive.fOffset;
	g_AxisOffsetDefault[PARA_IDX_Z_E5_DRIVE].AxisOffsetFromHomingSwitch = FixZ_E5_Drive.fOffset;
	g_AxisMinMaxWay[PARA_IDX_Z_E5_DRIVE].MinWay = FixZ_E5_Drive.fMinWay;
	g_AxisMinMaxWay[PARA_IDX_Z_E5_DRIVE].MaxWay = FixZ_E5_Drive.fMaxWay;
	g_ConfigStallExeption[PARA_IDX_Z_E5_DRIVE].StallExeption.Enable = 1;
	g_ConfigStallExeption[PARA_IDX_Z_E5_DRIVE].Up_DirA_Counter = 5;
	g_ConfigStallExeption[PARA_IDX_Z_E5_DRIVE].Down_DirB_Counter = 1;
	g_ConfigStallExeption[PARA_IDX_Z_E5_DRIVE].WaitTime = 100;
	g_CounterStallExeption[PARA_IDX_Z_E5_DRIVE].ExeptionLifeTimeCounter = 0;
}
#endif

void FelixDeviceConfigDefault(void)
{
	g_DeviceConfig.ExternEstopInEnable = 1;// 1 is on, 0 is off
	g_DeviceConfig.InternEstopEnable = 1;// 1 is on, 0 is off
	g_DeviceConfig.LockExternOutEstopEnable = 1;// not lock estop for extern
}
void FelixSensorAndPortGenerate(void)
{
	g_PortConfig.SetPort_PHOTOMOS.PowerOn = 1;
	g_PortConfig.SetPort_PHOTOMOS.StateAfterQuit = 0;
	g_PortConfig.SetPort_VCC_OEM.PowerOn = 0;
	g_PortConfig.SetPort_VCC_OEM.StateAfterQuit = 0;
	g_PortConfig.SetPort_SW_UVL.PowerOn = 0;
	g_PortConfig.SetPort_SW_UVL.StateAfterQuit = 0;
	g_PortConfig.SetPort_VCC_UVL.PowerOn = 0;
	g_PortConfig.SetPort_VCC_UVL.StateAfterQuit = 0;
	g_PortConfig.SetPort_MUELL_CTRL.PowerOn = 1;
	g_PortConfig.SetPort_MUELL_CTRL.StateAfterQuit = 0;
	g_PortConfig.SetPort_OEM_IN.PowerOn = 0;
	g_PortConfig.SetPort_OEM_IN.StateAfterQuit = 0;
	g_PortConfig.SetPort_UVL_IN.PowerOn = 0;
	g_PortConfig.SetPort_UVL_IN.StateAfterQuit = 0;
	g_PortConfig.SetPort_SW_RESERVE.PowerOn = 0;
	g_PortConfig.SetPort_SW_RESERVE.StateAfterQuit = 0;
	g_SensorConfig.SetPort_OEM_IN = 0;
	g_SensorConfig.SetPort_UVL_IN = 0;

}
void FelixEquipmentConfigGenerate(void)
{
	g_EquipmentConfig.EquipmentConfig = 0x0000;
	g_EquipmentConfig.DeviceConfig = 0x0000;
}
void FelixEstopCounterGenerate(void)
{
	VARIABLEN_SETGET_ALLOC
	g_EstopCounter.EstopAll = 0;
	g_EstopCounter.EstopByMotion = 0;
	VARIABLEN_SETGET_RELEASE
}
void FelixEstopWaitGenerate(void)
{
	VARIABLEN_SETGET_ALLOC
	g_EstopWait.EstopWait = 1700;
	VARIABLEN_SETGET_RELEASE
}

void FelixSearchAdapterRelativeDeltaWayGenerate(void)
{
	VARIABLEN_SETGET_ALLOC
	g_SearchAdapterRelativeDeltaWay.RelativeWay = (double)-1.0;
	g_SearchAdapterRelativeDeltaWay.DeltaWay = (double)-0.2;
	VARIABLEN_SETGET_RELEASE
}

void FelixApplicationSpdGenerate(void)
{
	unsigned short idx;
	//unsigned short cntAx;
	VARIABLEN_SETGET_ALLOC
	for(idx = 0; idx < MAX_APPLICATION_SPD; idx++)
	{
		// Nicht Firmware Werte, sondern Composser Werte verwenden
		g_ApplicationSpd[idx].AppAxisSpd[AXIS_ID_X_DRIVE-1].fSpd = (float)300.0;
		g_ApplicationSpd[idx].AppAxisSpd[AXIS_ID_X_DRIVE-1].fAcc = (float)700.0;

		g_ApplicationSpd[idx].AppAxisSpd[AXIS_ID_FRONT_CARRIAGE-1].fSpd = (float)300.0;
		g_ApplicationSpd[idx].AppAxisSpd[AXIS_ID_FRONT_CARRIAGE-1].fAcc = (float)500.0;

		g_ApplicationSpd[idx].AppAxisSpd[AXIS_ID_REAR_CARRIAGE-1].fSpd = (float)300.0;
		g_ApplicationSpd[idx].AppAxisSpd[AXIS_ID_REAR_CARRIAGE-1].fAcc = (float)500.0;

		g_ApplicationSpd[idx].AppAxisSpd[AXIS_ID_BOTTOM_CARRIAGE-1].fSpd = (float)300.0;
		g_ApplicationSpd[idx].AppAxisSpd[AXIS_ID_BOTTOM_CARRIAGE-1].fAcc = (float)500.0;

		g_ApplicationSpd[idx].AppAxisSpd[AXIS_ID_Z_DRIVE-1].fSpd = (float)130.0;
		g_ApplicationSpd[idx].AppAxisSpd[AXIS_ID_Z_DRIVE-1].fAcc = (float)500.0;

		/*for(cntAx = 0; cntAx < MAX_AXES; cntAx++)
		{
			WDO();
			g_ApplicationSpd[idx].AppAxisSpd[cntAx].fSpd = pIMSFixedPara[cntAx]->fEndSpeed;
			g_ApplicationSpd[idx].AppAxisSpd[cntAx].fAcc = pIMSFixedPara[cntAx]->fAcceleration;
		}*/
	}
	g_ApplicationSpd[0].AppAxisSpd[AXIS_ID_Z_DRIVE-1].fSpd = (float)10.0;
	g_ApplicationSpd[0].AppAxisSpd[AXIS_ID_Z_DRIVE-1].fAcc = (float)40.0;
	VARIABLEN_SETGET_RELEASE
}
void AxisDataGenerateFromFixedData(void)
{
	unsigned short cnt;

	for(cnt = 0; cnt < MAX_AXES; cnt++)
	{
		WDO();
		CopyFullFixedToChangedPara(pIMSChangedPara[cnt], pIMSFixedPara[cnt]);
	}
}

void CopyFullFixedToChangedPara(udtIMSAxisChangedParameter *dst, const udtIMSAxisFixedParameter *src)
{
	dst->fOffset = src->fOffset;
	dst->fStartSpeed = src->fStartSpeed;
	dst->fEndSpeedNegative = src->fEndSpeedNegative;
	dst->fEndSpeed = src->fEndSpeed;
	dst->fHomingSpeed = src->fHomingSpeed;
	dst->f1536Speed = src->f1536Speed;
	
	dst->fAccelerationNegative = src->fAccelerationNegative;
	dst->fAcceleration = src->fAcceleration;
	dst->fAccelerationHoming = src->fAccelerationHoming;
	dst->fAccelerationSlew = src->fAccelerationSlew;
	dst->fAcceleration1536 = src->fAcceleration1536;
	
	dst->usRunCurrent = src->usRunCurrent;
	dst->usRunCurrent1536 = src->usRunCurrent1536;
	dst->usHoldCurrent = src->usHoldCurrent;
	dst->usHoldCurrent1536 = src->usHoldCurrent1536;
	dst->fPrecisionDeadband = src->fPrecisionDeadband;
	dst->fPrecisionDeadband1536 = src->fPrecisionDeadband1536;
	dst->usStallsteps = src->usStallsteps;
	dst->usHoldCurrentWaitTime = src->usHoldCurrentWaitTime;
	dst->usMotorWobbleWaitTime = src->usMotorWobbleWaitTime;
	dst->usMotorWobbleWaitTime1536 = src->usMotorWobbleWaitTime1536;
	dst->fMinWay = src->fMinWay;
	dst->fMaxWay = src->fMaxWay;
}

/*unsigned short CountMaskAxes(unsigned short Mask)
{
	unsigned short i;
	unsigned short cnt = 0;
	for(i=0; i<MAX_AXES; i++)
	{
		if(Mask & g_FelixAxes[i].AxisBit)
		{
			cnt++;
			WDO();
		}
	}
	return cnt;
}*/

void g_SetAxes_LastErrorAxisHWForEver(unsigned short i, unsigned short value)
{
	GLOBAL_AXES_DATA_ALLOC
	g_FelixAxes[i].LastErrorAxisHWForEver = value;
	GLOBAL_AXES_DATA_RELEASE
}
unsigned short g_GetAxes_LastErrorAxisHWForEver(unsigned short i)
{
	unsigned short tmp;
	GLOBAL_AXES_DATA_ALLOC
	tmp = g_FelixAxes[i].LastErrorAxisHWForEver;
	GLOBAL_AXES_DATA_RELEASE
	return tmp;
}

void g_SetAxes_ErrorAxisHW(unsigned short i, unsigned short value)
{
	GLOBAL_AXES_DATA_ALLOC
	g_FelixAxes[i].ErrorAxisHW = value;
	GLOBAL_AXES_DATA_RELEASE
}
unsigned short g_GetAxes_ErrorAxisHW(unsigned short i)
{
	unsigned short tmp;
	GLOBAL_AXES_DATA_ALLOC
	tmp = g_FelixAxes[i].ErrorAxisHW;
	GLOBAL_AXES_DATA_RELEASE
	return tmp;
}

void g_SetAxes_ErrorAxisCyBio(unsigned short i, unsigned short value)
{
	GLOBAL_AXES_DATA_ALLOC
	g_FelixAxes[i].ErrorAxisCyBio = value;
	GLOBAL_AXES_DATA_RELEASE
}
unsigned short g_GetAxes_ErrorAxisCyBio(unsigned short i)
{
	unsigned short tmp;
	GLOBAL_AXES_DATA_ALLOC
	tmp = g_FelixAxes[i].ErrorAxisCyBio;
	GLOBAL_AXES_DATA_RELEASE
	return tmp;
}


void g_SetAxes_ErrorFlag(unsigned short i, unsigned short value)
{
	GLOBAL_AXES_DATA_ALLOC
	g_FelixAxes[i].ErrorFlag = value;
	GLOBAL_AXES_DATA_RELEASE
}
unsigned short g_GetAxes_ErrorFlag(unsigned short i)
{
	unsigned short tmp;
	GLOBAL_AXES_DATA_ALLOC
	tmp = g_FelixAxes[i].ErrorFlag;
	GLOBAL_AXES_DATA_RELEASE
	return tmp;
}


/*void g_SetAxes_StallFlag(unsigned short i, unsigned short value)
{
	GLOBAL_AXES_DATA_ALLOC
	g_FelixAxes[i].StallFlag = value;
	GLOBAL_AXES_DATA_RELEASE
}
unsigned short g_GetAxes_StallFlag(unsigned short i)
{
	unsigned short tmp;
	GLOBAL_AXES_DATA_ALLOC
	tmp = g_FelixAxes[i].StallFlag;
	GLOBAL_AXES_DATA_RELEASE
	return tmp;
}
*/

void g_SetAxes_ErrorAxisHWStall(unsigned short i, unsigned short value)
{
	GLOBAL_AXES_DATA_ALLOC
	g_FelixAxes[i].ErrorAxisHWStall = value;
	GLOBAL_AXES_DATA_RELEASE
}
unsigned short g_GetAxes_ErrorAxisHWStall(unsigned short i)
{
	unsigned short tmp;
	GLOBAL_AXES_DATA_ALLOC
	tmp = g_FelixAxes[i].ErrorAxisHWStall;
	GLOBAL_AXES_DATA_RELEASE
	return tmp;
}


void g_SetAxes_Running(unsigned short i, unsigned short value)
{
	GLOBAL_AXES_DATA_ALLOC
	g_FelixAxes[i].Running = value;
	GLOBAL_AXES_DATA_RELEASE
}
unsigned short g_GetAxes_HomingFlag(unsigned short i)
{
	unsigned short tmp;
	GLOBAL_AXES_DATA_ALLOC
	tmp = g_FelixAxes[i].HomingFlag;
	GLOBAL_AXES_DATA_RELEASE
	return tmp;
}


void g_SetAxes_HomingFlag(unsigned short i, unsigned short value)
{
	GLOBAL_AXES_DATA_ALLOC
	g_FelixAxes[i].HomingFlag = value;
	GLOBAL_AXES_DATA_RELEASE
}
unsigned short g_GetAxes_Running(unsigned short i)
{
	unsigned short tmp;
	GLOBAL_AXES_DATA_ALLOC
	tmp = g_FelixAxes[i].Running;
	GLOBAL_AXES_DATA_RELEASE
	return tmp;
}
/**************************************************************************
   Anmerkungen zum Antriebssystem:
   Stromeinstellung am IC A3979 von Allegro
   Itrip=Vref/(8*RS) mit RS=0,12 Ohm und Vref_max=2,500V
   Itrip_max=2,6A
   Digitalpoti MCP41010
   Ua= Uref*(256-X)/256 mit X zw. 0 und 255 (digits), Uref= 2,500V
   daraus folgt:
   X= 256-(Itrip*8*RS*256/Uref)
***************************************************************************/

/* FUNCTION: 	CurrentCalc
   PARAMETERS:	Spitzenstrom in A
   DESCRIPTION:	Berechnung des Digitalwertes für das Poti zur Stromeinstellung
   RETURNS: 	digits */
/*unsigned char CurrentCalc(float curr)
{ int help;
  if(curr > MAX_CURR) curr= MAX_CURR;
  if(curr < MIN_CURR) curr= MIN_CURR;
  help= 256-(curr*8*RS*256/UREF);
  if(help < 0) help=0;
  if(help > 255) help=255;
  return help; 
}*/
