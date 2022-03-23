#ifndef DEVICE_INCLUDE_KX224_1050_H_
#define DEVICE_INCLUDE_KX224_1050_H_

/************************************************************************************/
/*			Include 																*/
/************************************************************************************/
//		    Base
#include "main.h"
//			HAL
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_spi.h"
#include "stm32f1xx_hal_gpio.h"
// 		  FreeRTOS
#include "FreeRTOS.h"
#include "semphr.h"
//		   Common
#include "gpio_freertos.h"
#include "spi_freertos.h"

/************************************************************************************/
/*			Defines and def group													*/
/************************************************************************************/
// 			SPI	configurate
#define ADXL345_SPI_CPHA				SPI_PHASE_2EDGE	     // CPHA=1
#define ADXL345_SPI_CPOL				SPI_POLARITY_HIGH    // CPOL=1
#define ADXL345_SPI_DATASIZE			SPI_DATASIZE_8BIT
#define ADXL345_SPI_DIRECTION			SPI_DIRECTION_2LINES // 4-wire
#define ADXL345_SPI_BIT_ORDER			SPI_FIRSTBIT_MSB

//			Register tabel
#define	XHPL 							((uint8_t) 0x00)
#define	XHPH 							((uint8_t) 0x01)
#define	YHPL 							((uint8_t) 0x02)
#define	YHPH 							((uint8_t) 0x03)
#define	ZHPL 							((uint8_t) 0x04)
#define	ZHPH 							((uint8_t) 0x05)
#define	XOUTL							((uint8_t) 0x06)
#define	XOUTH 							((uint8_t) 0x07)
#define	YOUTL							((uint8_t) 0x08)
#define	YOUTH 							((uint8_t) 0x09)
#define	ZOUTL							((uint8_t) 0x0A)
#define	ZOUTH 							((uint8_t) 0x0B)
#define COTR 							((uint8_t) 0x0C)
#define WHO_AM_I						((uint8_t) 0x0F)
#define TSCP 							((uint8_t) 0x10)
#define TSPP 							((uint8_t) 0x11)
#define INS1 							((uint8_t) 0x12)
#define INS2 							((uint8_t) 0x13)
#define INS3 							((uint8_t) 0x14)
#define STAT 							((uint8_t) 0x15)
#define INT_REL							((uint8_t) 0x17)
#define CNTL1 							((uint8_t) 0x18)
#define CNTL2 							((uint8_t) 0x19)
#define CNTL3 							((uint8_t) 0x1A)
#define ODCNTL 							((uint8_t) 0x1B)
#define INC1 							((uint8_t) 0x1C)
#define INC2 							((uint8_t) 0x1D)
#define INC3 							((uint8_t) 0x1E)
#define INC4 							((uint8_t) 0x1F)
#define INC5 							((uint8_t) 0x20)
#define INC6 							((uint8_t) 0x21)
#define TILT_TIMER						((uint8_t) 0x22)
#define WUFC 							((uint8_t) 0x23)
#define TDTRC 							((uint8_t) 0x24)
#define TDTC 							((uint8_t) 0x25)
#define TTH 							((uint8_t) 0x26)
#define TTL 							((uint8_t) 0x27)
#define FTD 							((uint8_t) 0x28)
#define STD 							((uint8_t) 0x29)
#define TLT 							((uint8_t) 0x2A)
#define TWS 							((uint8_t) 0x2B)
#define FFTH 							((uint8_t) 0x2C)
#define FFC 							((uint8_t) 0x2D)
#define FFCNTL 							((uint8_t) 0x2E)
#define ATH 							((uint8_t) 0x30)
#define TILT_ANGLE_LL					((uint8_t) 0x32)
#define TILT_ANGLE_HL					((uint8_t) 0x33)
#define HYST_SET						((uint8_t) 0x34)
#define LP_CNTL							((uint8_t) 0x35)
#define BUF_CNTL1						((uint8_t) 0x3A)
#define BUF_CNTL2						((uint8_t) 0x3B)
#define BUF_STATUS1						((uint8_t) 0x3C)
#define BUF_STATUS2						((uint8_t) 0x3D)
#define BUF_CLEAR						((uint8_t) 0x3E)
#define BUF_READ						((uint8_t) 0x3F)
#define SELF_TEST						((uint8_t) 0x60)

//	kx224 WHO_AM_I content
#define KX224_WHO_AM_I					((uint8_t) 0x2B)

#define KX224_Self_Test_Activate 		((uint8_t) 0x2B)

#define KX224_SPI_READ					((uint8_t) 1<<7)

/************************************************************************************/
/*			 Settings Data type declarations										*/
/************************************************************************************/
typedef enum
{

	KX224_Standby_Mode					=	0b00000000,
	KX224_Operating_Mode				=	0b10000000,

}KX224_CNTL1_PC1_TypeDef;

typedef enum
{

	KX224_Low_Power_Mode				=	0b00000000,
	KX224_High_Resolution_Mode			=	0b01000000,

}KX224_CNTL1_RES_TypeDef;

typedef enum
{

	KX224_Disable_Interrupt_Data_Ready	=	0b00000000,
	KX224_Enable_Interrupt_Data_Ready	=	0b00100000,

}KX224_CNTL1_DRDYE_TypeDef;

typedef enum
{

	KX224_Range_8G						=	0b00000000,
	KX224_Range_16G						=	0b00001000,
	KX224_Range_32G						=	0b00010000,

}KX224_CNTL1_GSEL_TypeDef;

typedef enum
{

	KX224_Tap_Double_Tap_Disble			=	0b00000000,
	KX224_Tap_Double_Tap_Enable			=	0b00000100,

}KX224_CNTL1_TDTE_TypeDef;

typedef enum
{

	KX224_Wake_Up_Function_Disable		=	0b00000000,
	KX224_Wake_Up_Function_Enable		=	0b00000010,

}KX224_CNTL1_WUFE_TypeDef;

typedef enum
{

	KX224_Tilt_Position_Function_Disable=	0b00000000,
	KX224_Tilt_Position_Function_Enable	=	0b00000001,

}KX224_CNTL1_TPE_TypeDef;

typedef struct
{

	KX224_CNTL1_PC1_TypeDef				PC1;
	KX224_CNTL1_RES_TypeDef 			RES;
	KX224_CNTL1_DRDYE_TypeDef			DRDYE;
	KX224_CNTL1_GSEL_TypeDef			Range;
	KX224_CNTL1_TDTE_TypeDef			TDTE;
	KX224_CNTL1_WUFE_TypeDef			WUFE;
	KX224_CNTL1_TPE_TypeDef 			TPE;

}KX224_Settings_CNTL1_TypeDef;

typedef enum
{

	KX224_No_Action						=	0b00000000,
	KX224_Start_RAM_Reboot				=	0b10000000,

}KX224_CNTL2_SRST_TypeDef;

typedef enum
{

	KX224_No_Action						=	0b00000000,
	KX224_Command_Test_Control			=	0b01000000,

}KX224_CNTL2_COTC_TypeDef;

typedef enum
{

	KX224_No_Activate_Interupt			=	0b00000000,
	KX224_Activate_Interupt				=	0b00100000,

}KX224_CNTL2_LEM_TypeDef;

typedef enum
{

	KX224_No_Activate_Interupt			=	0b00000000,
	KX224_Activate_Interupt				=	0b00010000,

}KX224_CNTL2_RIM_TypeDef;

typedef enum
{

	KX224_No_Activate_Interupt			=	0b00000000,
	KX224_Activate_Interupt				=	0b00001000,

}KX224_CNTL2_DOM_TypeDef;

typedef enum
{

	KX224_No_Activate_Interupt			=	0b00000000,
	KX224_Activate_Interupt				=	0b00000100,

}KX224_CNTL2_UPM_TypeDef;

typedef enum
{

	KX224_No_Activate_Interupt			=	0b00000000,
	KX224_Activate_Interupt				=	0b00000010,

}KX224_CNTL2_FDM_TypeDef;

typedef enum
{

	KX224_No_Activate_Interupt			=	0b00000000,
	KX224_Activate_Interupt				=	0b00000001,

}KX224_CNTL2_FUM_TypeDef;

typedef struct
{

	KX224_CNTL2_SRST_TypeDef			SRST;
	KX224_CNTL2_COTC_TypeDef 			COTC;
	KX224_CNTL2_LEM_TypeDef				LEM;
	KX224_CNTL2_RIM_TypeDef				RIM;
	KX224_CNTL2_DOM_TypeDef				DOM;
	KX224_CNTL2_UPM_TypeDef				UPM;
	KX224_CNTL2_FDM_TypeDef				FDM;
	KX224_CNTL2_FUM_TypeDef				FUM;

}KX224_Settings_CNTL2_TypeDef;

//	Rate Update Data Tilt Angle
typedef enum
{

	KX224_CNTL3_Data_Rate_00001_563Hz 	=	0b00000000,	//     1.563Hz
	KX224_CNTL3_Data_Rate_00006_250Hz 	=	0b01000000,	//     6.250Hz
	KX224_CNTL3_Data_Rate_00012_500Hz 	=	0b10000000,	//    12.500Hz
	KX224_CNTL3_Data_Rate_00050_000Hz 	=	0b11000000,	//    50.000Hz

}KX224_CNTL3_Tilt_Pos_ODR_TypeDef;

//	Rate Update Data Tap Event
typedef enum
{

	KX224_CNTL3_Data_Rate_00012_5Hz 	=	0b00100000,	//    12.500Hz
	KX224_CNTL3_Data_Rate_00025_0Hz 	=	0b00101000,	//    25.000Hz
	KX224_CNTL3_Data_Rate_00050_0Hz 	=	0b00000000,	//    50.000Hz
	KX224_CNTL3_Data_Rate_00100_0Hz 	=	0b00001000,	//   100.000Hz
	KX224_CNTL3_Data_Rate_00200_0Hz 	=	0b00010000,	//   200.000Hz
	KX224_CNTL3_Data_Rate_00400_0Hz 	=	0b00011000,	//   400.000Hz
	KX224_CNTL3_Data_Rate_00800_0Hz 	=	0b00110000,	//   800.000Hz
	KX224_CNTL3_Data_Rate_01600_0Hz 	=	0b00111000,	//  1600.000Hz

}KX224_CNTL3_Tap_Event_ODR_TypeDef;

//	Rate Update Data Wake_Up
typedef enum
{

	KX224_CNTL3_Data_Rate_00000_781Hz 	=	0b00000000,	//     0.781Hz
	KX224_CNTL3_Data_Rate_00001_563Hz 	=	0b00000001,	//     1.563Hz
	KX224_CNTL3_Data_Rate_00003_125Hz 	=	0b00000010,	//     3.125Hz
	KX224_CNTL3_Data_Rate_00006_250Hz 	=	0b00000011,	//     6.250Hz
	KX224_CNTL3_Data_Rate_00012_500Hz 	=	0b00000100,	//    12.500Hz
	KX224_CNTL3_Data_Rate_00025_000Hz 	=	0b00000101,	//    25.000Hz
	KX224_CNTL3_Data_Rate_00050_000Hz 	=	0b00000110,	//    50.000Hz
	KX224_CNTL3_Data_Rate_00100_000Hz 	=	0b00000111,	//   100.000Hz

}KX224_CNTL3_Wake_Up_ODR_TypeDef;

typedef struct
{

	KX224_CNTL3_Tilt_Pos_ODR_TypeDef 	OTP;
	KX224_CNTL3_Tap_Event_ODR_TypeDef	OTDT;
	KX224_CNTL3_Wake_Up_ODR_TypeDef		OWUF;

}KX224_Settings_CNTL3_TypeDef;

typedef enum
{

	KX224_Low_Pass_Filter_Applied		=	0b00000000,
	KX224_Low_Pass_Filter_Not_Applied	=	0b10000000,

}KX224_ODCNTL_IIR_BYPASS_TypeDef;

typedef enum
{

	KX224_Low_Pass_Filter_Applied		=	0b00000000,
	KX224_Low_Pass_Filter_Not_Applied	=	0b01000000,

}KX224_ODCNTL_LPRO_TypeDef;

//	Rate Update Acceleration Output Data
typedef enum
{

	KX224_CNTL3_Data_Rate_00000_781Hz 	=	0b00001000,	//     0.781Hz
	KX224_CNTL3_Data_Rate_00001_563Hz 	=	0b00001001,	//     1.563Hz
	KX224_CNTL3_Data_Rate_00003_125Hz 	=	0b00001010,	//     3.125Hz
	KX224_CNTL3_Data_Rate_00006_250Hz 	=	0b00001011,	//     6.250Hz
	KX224_CNTL3_Data_Rate_00012_500Hz 	=	0b00000000,	//    12.500Hz
	KX224_CNTL3_Data_Rate_00025_000Hz 	=	0b00000001,	//    25.000Hz
	KX224_CNTL3_Data_Rate_00050_000Hz 	=	0b00000010,	//    50.000Hz
	KX224_CNTL3_Data_Rate_00100_000Hz 	=	0b00000011,	//   100.000Hz
	KX224_CNTL3_Data_Rate_00200_000Hz 	=	0b00000100,	//   200.000Hz
	KX224_CNTL3_Data_Rate_00400_000Hz 	=	0b00000101,	//   400.000Hz
	KX224_CNTL3_Data_Rate_00800_000Hz 	=	0b00000110,	//   800.000Hz
	KX224_CNTL3_Data_Rate_01600_000Hz 	=	0b00000111,	//  1600.000Hz
	KX224_CNTL3_Data_Rate_03200_000Hz 	=	0b00001100,	//  3200.000Hz
	KX224_CNTL3_Data_Rate_06400_000Hz 	=	0b00001101,	//  6400.000Hz
	KX224_CNTL3_Data_Rate_12800_000Hz 	=	0b00001110,	// 12800.000Hz
	KX224_CNTL3_Data_Rate_25600_000Hz 	=	0b00001111,	// 25600.000Hz

}KX224_ODCNTL_Out_Data_ODR_TypeDef;

typedef struct
{

	KX224_ODCNTL_IIR_BYPASS_TypeDef 	IIR_BYPASS;
	KX224_ODCNTL_LPRO_TypeDef			LPRO;
	KX224_ODCNTL_Out_Data_ODR_TypeDef	OSA;

}KX224_Settings_ODCNTL_TypeDef;

typedef enum
{

	KX224_Double_Tap_Disable			=	0b00000000,
	KX224_Double_Tap_Enable				=	0b00000010,

}KX224_TDTRC_DTRE_TypeDef;

typedef enum
{

	KX224_Single_Tap_Disable			=	0b00000000,
	KX224_Single_Tap_Enable				=	0b00000001,

}KX224_TDTRC_STRE_TypeDef;

typedef struct
{

	KX224_TDTRC_DTRE_TypeDef 			DTRE;
	KX224_TDTRC_STRE_TypeDef			STRE;

}KX224_Settings_TDTRC_TypeDef;

typedef enum
{

	KX224_Free_Fall_Engine_Disable		=	0b00000000,
	KX224_Free_Fall_Engine_Enable		=	0b10000000,

}KX224_FFCNTL_FFIE_TypeDef;

typedef enum
{

	KX224_Free_Fall_Interrupt_Latched	=	0b00000000,
	KX224_Free_Fall_Interrupt_UnLatched	=	0b01000000,

}KX224_FFCNTL_ULMODE_TypeDef;

typedef enum
{

	KX224_Debounce_Up_Down_Method		=	0b00000000,
	KX224_Debounce_Up_Reset_Method		=	0b00001000,

}KX224_FFCNTL_DCRM_TypeDef;

//	Rate Update Data Free Fall Engine
typedef enum
{

	KX224_FFCNTL_Data_Rate_00012_5Hz 	=	0b00000000,	//    12.500Hz
	KX224_FFCNTL_Data_Rate_00025_0Hz 	=	0b00000001,	//    25.000Hz
	KX224_FFCNTL_Data_Rate_00050_0Hz 	=	0b00000010,	//    50.000Hz
	KX224_FFCNTL_Data_Rate_00100_0Hz 	=	0b00000011,	//   100.000Hz
	KX224_FFCNTL_Data_Rate_00200_0Hz 	=	0b00000100,	//   200.000Hz
	KX224_FFCNTL_Data_Rate_00400_0Hz 	=	0b00000101,	//   400.000Hz
	KX224_FFCNTL_Data_Rate_00800_0Hz 	=	0b00000110,	//   800.000Hz
	KX224_FFCNTL_Data_Rate_01600_0Hz 	=	0b00000111,	//  1600.000Hz

}KX224_FFCNTL_OFFI_TypeDef;

typedef struct
{

	KX224_FFCNTL_FFIE_TypeDef 			FFIE;
	KX224_FFCNTL_ULMODE_TypeDef			ULMODE;
	KX224_FFCNTL_DCRM_TypeDef			DCRM;
	KX224_FFCNTL_OFFI_TypeDef 			OFFI;

}KX224_Settings_FFCNTL_TypeDef;

typedef enum
{

	KX224_LP_CNTL_No_Averaging 			=	0b00000000,
	KX224_LP_CNTL_002_Samples_Averaging =	0b00010000,
	KX224_LP_CNTL_004_Samples_Averaging	=	0b00100000,
	KX224_LP_CNTL_008_Samples_Averaging =	0b00110000,
	KX224_LP_CNTL_016_Samples_Averaging	=	0b01000000,
	KX224_LP_CNTL_032_Samples_Averaging =	0b01010000,
	KX224_LP_CNTL_064_Samples_Averaging =	0b01100000,
	KX224_LP_CNTL_128_Samples_Averaging	=	0b01110000,

}KX224_Settings_LP_CNTL_TypeDef;

typedef enum
{

	KX224_Buffer_Disable				=	0b00000000,
	KX224_Buffer_Enable					=	0b10000000,

}KX224_CNTLx_BUFE_TypeDef;

typedef enum
{

	KX224_Save_08bit_samples			=	0b00000000,
	KX224_Save_16bit_samples			=	0b01000000,

}KX224_CNTLx_BRES_TypeDef;

typedef enum
{

	KX224_Interrupt_Disable				=	0b00000000,
	KX224_Interrupt_Enable				=	0b00100000,

}KX224_CNTLx_BFIE_TypeDef;

typedef enum
{

	KX224_FIFO_Mode						=	0b00000000,
	KX224_Stream_Mode					=	0b00000001,
	KX224_Trigger_Mode					=	0b00000010,
	KX224_FILO_Mode						=	0b00000011,

}KX224_CNTLx_BUF_M_TypeDef;

typedef struct
{

	KX224_CNTLx_BUFE_TypeDef 			BUFE;
	KX224_CNTLx_BRES_TypeDef			BRES;
	KX224_CNTLx_BFIE_TypeDef			BFIE;
	KX224_CNTLx_BUF_M_TypeDef 			BUF_M;

}KX224_Settings_BUF_CNTLx_TypeDef;

/************************************************************************************/
/*			 Interrupt Data type declarations										*/
/************************************************************************************/

typedef enum
{

	KX224_50usec_or_10usec_Pulse_Width	=	0b00000000,	//	Then OSA in ODCNTL register > 1600, pulse width 10usec
	KX224_OSA_Period_Pulse_Width		=	0b01000000,
	KX224_2_OSA_Periods_Pulse_Width		=	0b10000000,
	KX224_4_OSA_Periods_Pulse_Width		=	0b11000000,

}KX224_INC1_PWSEL1_TypeDef;

typedef enum
{

	KX224_Interrupt_Disable				=	0b00000000,
	KX224_Interrupt_Enable				=	0b00100000,

}KX224_INC1_IEN1_TypeDef;

typedef enum
{

	KX224_Interrupt_Low_Polarity	=	0b00000000,
	KX224_Interrupt_High_Polarity	=	0b00010000,

}KX224_INC1_IEA1_TypeDef;

typedef enum
{

	KX224_Cleared_by_Redin_INT_REL		=	0b00000000,
	KX224_Transmit_One_Pulse			=	0b00001000,

}KX224_INC1_IEL1_TypeDef;

typedef enum
{

	KX224_Self_Test_Negative			=	0b00000000,
	KX224_Self_Test_Positive			=	0b00000010,

}KX224_INC1_STPOL_TypeDef;

typedef enum
{

	KX224_SPI_3Wire_Not_Used			=	0b00000000,
	KX224_SPI_3Wire_Used				=	0b00000001,

}KX224_INC1_SPI3E_TypeDef;

typedef struct
{

	KX224_INC1_PWSEL1_TypeDef 			PWSEL1;
	KX224_INC1_IEN1_TypeDef				IEN1;
	KX224_INC1_IEA1_TypeDef				IEA1;
	KX224_INC1_IEL1_TypeDef				IEL1;
	KX224_INC1_STPOL_TypeDef			STPOL;
	KX224_INC1_SPI3E_TypeDef			SPI3E;

}KX224_Settings_INC1_TypeDef;

typedef enum
{

	KX224_OR_Combination				=	0b00000000,
	KX224_AND_Combination				=	0b01000000,

}KX224_INC2_AOI_TypeDef;

typedef enum
{

	KX224_X_Negative_Disable			=	0b00000000,
	KX224_X_Negative_Enable				=	0b00100000,

}KX224_INC2_XNWU_TypeDef;

typedef enum
{

	KX224_X_Positive_Disable			=	0b00000000,
	KX224_X_Positive_Enable				=	0b00010000,

}KX224_INC2_XPWU_TypeDef;

typedef enum
{

	KX224_Y_Negative_Disable			=	0b00000000,
	KX224_Y_Negative_Enable				=	0b00001000,

}KX224_INC2_YNWU_TypeDef;

typedef enum
{

	KX224_Y_Positive_Disable			=	0b00000000,
	KX224_Y_Positive_Enable				=	0b00000100,

}KX224_INC2_YPWU_TypeDef;

typedef enum
{

	KX224_Z_Negative_Disable			=	0b00000000,
	KX224_Z_Negative_Enable				=	0b00000010,

}KX224_INC2_ZNWU_TypeDef;

typedef enum
{

	KX224_Z_Positive_Disable			=	0b00000000,
	KX224_Z_Positive_Enable				=	0b00000001,

}KX224_INC2_ZPWU_TypeDef;

typedef struct
{

	KX224_INC2_AOI_TypeDef 				AOI;
	KX224_INC2_XNWU_TypeDef				XNWU;
	KX224_INC2_XPWU_TypeDef				XPWU;
	KX224_INC2_YNWU_TypeDef				YNWU;
	KX224_INC2_YPWU_TypeDef				YPWU;
	KX224_INC2_ZNWU_TypeDef				ZNWU;
	KX224_INC2_ZPWU_TypeDef				ZPWU;

}KX224_Settings_INC2_TypeDef;

typedef enum
{

	KX224_Left_Tap_Disable				=	0b00000000,
	KX224_Left_Tap_Enable				=	0b00100000,

}KX224_INC3_TLEM_TypeDef;

typedef enum
{

	KX224_Right_Tap_Disable				=	0b00000000,
	KX224_Right_Tap_Enable				=	0b00010000,

}KX224_INC3_TRIM_TypeDef;

typedef enum
{

	KX224_Down_Tap_Disable				=	0b00000000,
	KX224_Down_Tap_Enable				=	0b00001000,

}KX224_INC3_TDOM_TypeDef;

typedef enum
{

	KX224_Up_Tap_Disable				=	0b00000000,
	KX224_Up_Tap_Enable					=	0b00000100,

}KX224_INC3_TUPM_TypeDef;

typedef enum
{

	KX224_Face_Down_Tap_Disable			=	0b00000000,
	KX224_Face_Down_Tap_Enable			=	0b00000010,

}KX224_INC3_TFDM_TypeDef;

typedef enum
{

	KX224_Face_Up_Tap_Disable			=	0b00000000,
	KX224_Face_Up_Tap_Enable			=	0b00000001,

}KX224_INC3_TFUM_TypeDef;

typedef struct
{

	KX224_INC3_TLEM_TypeDef				TLEM;
	KX224_INC3_TRIM_TypeDef				TRIM;
	KX224_INC3_TDOM_TypeDef				TDOM;
	KX224_INC3_TUPM_TypeDef				TUPM;
	KX224_INC3_TFDM_TypeDef				TFDM;
	KX224_INC3_TFUM_TypeDef				TFUM;

}KX224_Settings_INC3_TypeDef;

typedef enum
{

	KX224_Free_Fall_Interrupt_Pin_1		=	0b10000000,
	KX224_Buffer_Full_Interrupt_Pin_1	=	0b01000000,
	KX224_Watermark_Interrupt_Pin_1 	=	0b00100000,
	KX224_Data_Redy_Interrupt_Pin_1 	=	0b00010000,
	KX224_SorD_Tap_Interrupt_Pin_1 		=	0b00000100,
	KX224_Wake_Up_Interrupt_Pin_1 		=	0b00000010,
	KX224_Tilt_Position_Interrupt_Pin_1	=	0b00000001,

}KX224_Settings_INC4_TypeDef;

typedef enum
{

	KX224_50usec_or_10usec_Pulse_Width	=	0b00000000,	//	Then OSA in ODCNTL register > 1600, pulse width 10usec
	KX224_OSA_Period_Pulse_Width		=	0b01000000,
	KX224_2_OSA_Periods_Pulse_Width		=	0b10000000,
	KX224_4_OSA_Periods_Pulse_Width		=	0b11000000,

}KX224_INC5_PWSEL2_TypeDef;

typedef enum
{

	KX224_Interrupt_Disable				=	0b00000000,
	KX224_Interrupt_Enable				=	0b00100000,

}KX224_INC5_IEN2_TypeDef;

typedef enum
{

	KX224_Interrupt_Low_Polarity	=	0b00000000,
	KX224_Interrupt_High_Polarity	=	0b00010000,

}KX224_INC5_IEA2_TypeDef;

typedef enum
{

	KX224_Cleared_by_Redin_INT_REL		=	0b00000000,
	KX224_Transmit_One_Pulse			=	0b00001000,

}KX224_INC5_IEL2_TypeDef;

typedef enum
{

	KX224_Avto_Clear_Interrupt_Deactive	=	0b00000000,
	KX224_Avto_Clear_Interrupt_Active	=	0b00000010,

}KX224_INC5_ACLR2_TypeDef;

typedef enum
{

	KX224_Avto_Clear_Interrupt_Deactive	=	0b00000000,
	KX224_Avto_Clear_Interrupt_Active	=	0b00000001,

}KX224_INC5_ACLR1_TypeDef;

typedef struct
{

	KX224_INC5_PWSEL2_TypeDef 			PWSEL2;
	KX224_INC5_IEN2_TypeDef				IEN2;
	KX224_INC5_IEA2_TypeDef				IEA2;
	KX224_INC5_IEL2_TypeDef				IEL2;
	KX224_INC5_ACLR2_TypeDef			ACLR2;
	KX224_INC5_ACLR1_TypeDef			ACLR1;

}KX224_Settings_INC5_TypeDef;

typedef enum
{

	KX224_Free_Fall_Interrupt_Pin_2		=	0b10000000,
	KX224_Buffer_Full_Interrupt_Pin_2	=	0b01000000,
	KX224_Watermark_Interrupt_Pin_2 	=	0b00100000,
	KX224_Data_Redy_Interrupt_Pin_2 	=	0b00010000,
	KX224_SorD_Tap_Interrupt_Pin_2 		=	0b00000100,
	KX224_Wake_Up_Interrupt_Pin_2 		=	0b00000010,
	KX224_Tilt_Position_Interrupt_Pin_2	=	0b00000001,

}KX224_Settings_INC6_TypeDef;

/************************************************************************************/
/*			 All Settings Data type declarations									*/
/************************************************************************************/

typedef struct
{

	uint8_t 							Low_Limit_Timer_Tap_Event;
	uint8_t 							High_Limit_Timer_Tap_Event;

}KX224_Settings_FTD_TypeDef;

typedef struct
{

	uint8_t 							Low_Trashold_Level_Angle;
	uint8_t 							High_Trashold_Level_Angle;

}KX224_Settings_Tilt_Angle_TypeDef;

typedef struct
{

	KX224_Settings_CNTL1_TypeDef 		*CNTL1_TypeDef;
	KX224_Settings_CNTL2_TypeDef 		*CNTL2_TypeDef;
	KX224_Settings_CNTL3_TypeDef 		*CNTL3_TypeDef;
	KX224_Settings_ODCNTL_TypeDef 		*ODCNTL_TypeDef;

	KX224_Settings_INC1_TypeDef			*INC1_TypeDef;
	KX224_Settings_INC2_TypeDef			*INC2_TypeDef;
	KX224_Settings_INC3_TypeDef			*INC3_TypeDef;
	KX224_Settings_INC4_TypeDef			*INC4_TypeDef;
	KX224_Settings_INC5_TypeDef			*INC5_TypeDef;
	KX224_Settings_INC6_TypeDef			*INC6_TypeDef;

	uint8_t	 							Tilt_Timer;
	uint8_t 							Wake_Up_Timer;

	KX224_Settings_TDTRC_TypeDef 		*TDTRC_TypeDef;

	uint8_t 							High_Trashold_Lavel_Tap_Event;
	uint8_t 							Low_Trashold_Lavel_Tap_Event;

	uint8_t 							Timer_Between_Tap_DoubleTap;
	KX224_Settings_FTD_TypeDef			*FTD_TypeDef;
	uint8_t 							Limit_Timer_DoubleTap_Event;
	uint8_t 							Timer_Countains_Samples_Tap_Event;
	uint8_t 							Width_Time_Window;

	uint8_t								Trashold_Level_Free_Fall;
	uint8_t								Timer_Countains_Samples_Free_Fall;

	KX224_Settings_FFCNTL_TypeDef 		*FFCNTL_TypeDef;

	uint8_t 							Trashold_Level_Wake_Up;

	KX224_Settings_Tilt_Angle_TypeDef	*Tilt_Angle_TypeDef;

	uint8_t 							Hysteresis_Set;

	KX224_Settings_LP_CNTL_TypeDef 		*LP_CNTL_TypeDef;

	KX224_Settings_BUF_CNTLx_TypeDef	*BUF_CNTL_TypeDef;

}KX224_Settings_TypeDef;

/************************************************************************************/
/*			 		Status Data type declarations									*/
/************************************************************************************/

typedef enum
{

	KX224_Left_Position_Not_Active		=	0b00000000,
	KX224_Left_Position_Active			=	0b00100000,

}KX224_Status_LE_TypeDef;

typedef enum
{

	KX224_Right_Position_Not_Active		=	0b00000000,
	KX224_Right_Position_Active			=	0b00010000,

}KX224_Status_RI_TypeDef;

typedef enum
{

	KX224_Down_Position_Not_Active		=	0b00000000,
	KX224_Down_Position_Active			=	0b00001000,

}KX224_Status_DO_TypeDef;

typedef enum
{

	KX224_Up_Position_Not_Active		=	0b00000000,
	KX224_Up_Position_Active			=	0b00000100,

}KX224_Status_UP_TypeDef;

typedef enum
{

	KX224_Face_Down_Position_Not_Active	=	0b00000000,
	KX224_Face_Down_Position_Active		=	0b00000010,

}KX224_Status_FD_TypeDef;

typedef enum
{

	KX224_Face_Up_Position_Not_Active	=	0b00000000,
	KX224_Face_Up_Position_Active		=	0b00000001,

}KX224_Status_FU_TypeDef;

typedef struct
{

	KX224_Status_LE_TypeDef				LE;
	KX224_Status_RI_TypeDef				RI;
	KX224_Status_DO_TypeDef				DO;
	KX224_Status_UP_TypeDef				UP;
	KX224_Status_FD_TypeDef				FD;
	KX224_Status_FU_TypeDef				FU;

}KX224_Status_TSCP_TypeDef;

typedef struct
{

	KX224_Status_LE_TypeDef				LE;
	KX224_Status_RI_TypeDef				RI;
	KX224_Status_DO_TypeDef				DO;
	KX224_Status_UP_TypeDef				UP;
	KX224_Status_FD_TypeDef				FD;
	KX224_Status_FU_TypeDef				FU;

}KX224_Status_TSPP_TypeDef;

typedef enum
{

	KX224_Left_Tap_Not_Active			=	0b00000000,
	KX224_Left_Tap_Active				=	0b00100000,

}KX224_INS1_TLE_TypeDef;

typedef enum
{

	KX224_Right_Tap_Not_Active			=	0b00000000,
	KX224_Right_Tap_Active				=	0b00010000,

}KX224_INS1_TRI_TypeDef;

typedef enum
{

	KX224_Down_Tap_Not_Active			=	0b00000000,
	KX224_Down_Tap_Active				=	0b00001000,

}KX224_INS1_TDO_TypeDef;

typedef enum
{

	KX224_Up_Tap_Not_Active				=	0b00000000,
	KX224_Up_Tap_Active					=	0b00000100,

}KX224_INS1_TUP_TypeDef;

typedef enum
{

	KX224_Face_Down_Tap_Not_Active		=	0b00000000,
	KX224_Face_Down_Tap_Active			=	0b00000010,

}KX224_INS1_TFD_TypeDef;

typedef enum
{

	KX224_Face_Up_Tap_Not_Active		=	0b00000000,
	KX224_Face_Up_Tap_Active			=	0b00000001,

}KX224_INS1_TFU_TypeDef;

typedef struct
{

	KX224_INS1_TLE_TypeDef				TLE;
	KX224_INS1_TRI_TypeDef				TRI;
	KX224_INS1_TDO_TypeDef				TDO;
	KX224_INS1_TUP_TypeDef				TUP;
	KX224_INS1_TFD_TypeDef				TFD;
	KX224_INS1_TFU_TypeDef				TFU;

}KX224_Status_INS1_TypeDef;

typedef enum
{

	KX224_Free_Fall_Not_Active			=	0b00000000,
	KX224_Free_Fall_Active				=	0b10000000,

}KX224_Status_FFS_TypeDef;

typedef enum
{

	KX224_Buffer_Full_Not_Active		=	0b00000000,
	KX224_Buffer_Full_Active			=	0b01000000,

}KX224_Status_BFI_TypeDef;

typedef enum
{

	KX224_Watermark_Not_Active			=	0b00000000,
	KX224_Watermark_Active 				=	0b00100000,

}KX224_Status_WMI_TypeDef;

typedef enum
{

	KX224_Data_Redy_Not_Active			=	0b00000000,
	KX224_Data_Redy_Active 				=	0b00010000,

}KX224_Status_DRDY_TypeDef;

typedef enum
{

	KX224_No_Tap						=	0b00000000,
	KX224_Single_Tap					=	0b00000100,
	KX224_Double_Tap					=	0b00001000,
	KX224_Undefined_Tap					=	0b00001100,

}KX224_Status_TDTS_TypeDef;

typedef enum
{

	KX224_Wake_Up_Not_Active			=	0b00000000,
	KX224_Wake_Up_Active 				=	0b00000010,

}KX224_Status_WUFS_TypeDef;

typedef enum
{

	KX224_Tilt_Position_Not_Active		=	0b00000000,
	KX224_Tilt_Position_Active			=	0b00000001,

}KX224_Status_TPS_TypeDef;

typedef struct
{

	KX224_Status_FFS_TypeDef			FFS;
	KX224_Status_BFI_TypeDef			BFI;
	KX224_Status_WMI_TypeDef			WMI;
	KX224_Status_DRDY_TypeDef			DRDY;
	KX224_Status_TDTS_TypeDef			TDTS;
	KX224_Status_WUFS_TypeDef			WUFS;
	KX224_Status_TPS_TypeDef 			TPS;

}KX224_Status_INS2_TypeDef;

typedef enum
{

	KX224_X_Negative_Not_Active			=	0b00000000,
	KX224_X_Negative_Active				=	0b00100000,

}KX224_INS3_XNWU_TypeDef;

typedef enum
{

	KX224_X_Positive_Not_Active			=	0b00000000,
	KX224_X_Positive_Active				=	0b00010000,

}KX224_INS3_XPWU_TypeDef;

typedef enum
{

	KX224_Y_Negative_Not_Active			=	0b00000000,
	KX224_Y_Negative_Active				=	0b00001000,

}KX224_INS3_YNWU_TypeDef;

typedef enum
{

	KX224_Y_Positive_Not_Active			=	0b00000000,
	KX224_Y_Positive_Active				=	0b00000100,

}KX224_INS3_YPWU_TypeDef;

typedef enum
{

	KX224_Z_Negative_Not_Active			=	0b00000000,
	KX224_Z_Negative_Active				=	0b00000010,

}KX224_INS3_ZNWU_TypeDef;

typedef enum
{

	KX224_Z_Positive_Not_Active			=	0b00000000,
	KX224_Z_Positive_Active				=	0b00000001,

}KX224_INS3_ZPWU_TypeDef;

typedef struct
{

	KX224_INS3_XNWU_TypeDef				XNWU;
	KX224_INS3_XPWU_TypeDef				XPWU;
	KX224_INS3_YNWU_TypeDef				YNWU;
	KX224_INS3_YPWU_TypeDef				YPWU;
	KX224_INS3_ZNWU_TypeDef				ZNWU;
	KX224_INS3_ZPWU_TypeDef				ZPWU;

}KX224_Status_INS3_TypeDef;

typedef enum
{

	KX224_No_Interrupt_Event			=	0b00000000,
	KX224_Interrupt_Event				=	0b00010000,

}KX224_Status_STATUS_REG_TypeDef;

typedef enum
{

	KX224_Buffer_Not_Ready				=	0b00000000,
	KX224_Buffer_Ready					=	0b10000000,

}KX224_Status_Buf_Trig_TypeDef;

typedef struct
{
	uint8_t 							Comand_Test_Response;	 	//COTR
	uint8_t 							Who_AM_I;

	KX224_Status_TSCP_TypeDef			*TSCP_TypeDef;
	KX224_Status_TSPP_TypeDef			*TSPP_TypeDef;

	KX224_Status_INS1_TypeDef			*INS1_TypeDef;
	KX224_Status_INS2_TypeDef			*INS2_TypeDef;
	KX224_Status_INS3_TypeDef			*INS3_TypeDef;

	KX224_Status_STATUS_REG_TypeDef 	*STATUS_REG_TypeDef;

	uint16_t 							Number_Samples_in_Buffer; 	//BUF_CNTLx
	KX224_Status_Buf_Trig_TypeDef		Status_Buffer;

}KX224_Status_TypeDef;

/************************************************************************************/
/*			 		KX224 Data type declarations									*/
/************************************************************************************/

typedef struct
{

	uint16_t							XHP_Wake_Up;
	uint16_t							YHP_Wake_Up;
	uint16_t							ZHP_Wake_Up;
	uint16_t							XOUT;
	uint16_t							YOUT;
	uint16_t							ZOUT;

}KX224_Data_Accelerometer_TypeDef;

typedef struct
{

	KX224_Settings_TypeDef 				*Settings;
	KX224_Status_TypeDef				*Status;
	KX224_Data_Accelerometer_TypeDef	Data_Acceleration;
	/* SPI */
	spi_freertos_nss_t					RTOS_SPI;
	/* EXTI GPIO */
	gpio_freertos_t						INT1;
	gpio_freertos_t						INT2;
	/* Timeouts */
	TickType_t							Mutex_Timeout;
	TickType_t							Transfer_Timeout;

}KX224_TypeDef;

/* Status codes */
typedef enum
{
	KX224_OK							= 0x00U,	// Success
	KX224_ERR							= 0x01U,	// Error
	KX224_NODEV							= 0x02U,	// No such device
	KX224_BUSY							= 0x03U,	// Device is busy
	KX224_TIMEOUT						= 0x04U		// Timeout
}KX224_Status_Function;

/************************************************************************************/
/*			 	KX224 Middle Layer Function Declaration								*/
/************************************************************************************/
/*			 			Communication Function										*/
/************************************************************************************/

	/* Write the device register */
KX224_Status_Function KX224_Write_Reg(KX224_TypeDef* hkx224, uint8_t reg, uint8_t value);

	/* Read the device register */
KX224_Status_Function KX224_Read_Reg(KX224_TypeDef* hkx224, uint8_t reg);

/************************************************************************************/
/*			 				Status Function											*/
/************************************************************************************/

	//	Read COTR Register in Accelerometer
KX224_Status_Function KX224_Read_Command_Test_Responce(KX224_TypeDef* hkx224);

	//	Read WHO_AM_I Register in Accelerometer
KX224_Status_Function KX224_Read_Dev_ID(KX224_TypeDef* hkx224);

	//	Read TSCP Register in Accelerometer
KX224_Status_Function KX224_Read_Tilt_Status_Current(KX224_TypeDef* hkx224);

	//	Read TSPP Register in Accelerometer
KX224_Status_Function KX224_Read_Tilt_Status_Previous(KX224_TypeDef* hkx224);

	//	Read INS1 Register in Accelerometer
KX224_Status_Function KX224_Read_Status_Interrupt_Tap_Event_Axes(KX224_TypeDef* hkx224);

	//	Read INS2 Register in Accelerometer
KX224_Status_Function KX224_Read_Status_Interrupt_Types_Function(KX224_TypeDef* hkx224);

	//	Read INS3 Register in Accelerometer
KX224_Status_Function KX224_Read_Status_Interrupt_Motion(KX224_TypeDef* hkx224);

	//	Read STATUS_REG Register in Accelerometer
KX224_Status_Function KX224_Read_Status_Presence_Interrupt(KX224_TypeDef* hkx224);

	//	Read BUF_STATUSx Register in Accelerometer
KX224_Status_Function KX224_Status_Buf_State(KX224_TypeDef* hkx224);

/************************************************************************************/
/*			 		Control Mod Accelerometer Function								*/
/************************************************************************************/

	//	Read CNTL1 Register in Accelerometer
KX224_Status_Function KX224_Read_Control_Main_Feature(KX224_TypeDef* hkx224);

	//	Set P1 in CNTL1 struct in KX224_TypeDef
KX224_Settings_CNTL1_TypeDef KX224_Set_Measure_Active(KX224_TypeDef* hkx224);

	//	Set P1 in CNTL1 struct in KX224_TypeDef
KX224_Settings_CNTL1_TypeDef KX224_Set_Measure_DeActive(KX224_TypeDef* hkx224);

	//	Set Resolution in CNTL1 struct in KX224_TypeDef
KX224_Settings_CNTL1_TypeDef KX224_Set_Resolution(KX224_TypeDef* hkx224,
												  KX224_CNTL1_RES_TypeDef Res);

	//	Set Data Ready as an Interrupt in CNTL1 struct in KX224_TypeDef
KX224_Settings_CNTL1_TypeDef KX224_Set_Data_Ready_Interrupt(KX224_TypeDef* hkx224,
															KX224_CNTL1_DRDYE_TypeDef Info_Activ_Int_Data_Redy);

	//	Set Renge Measure Data in CNTL1 struct in KX224_TypeDef
KX224_Settings_CNTL1_TypeDef KX224_Set_Range(KX224_TypeDef* hkx224,
											 KX224_CNTL1_GSEL_TypeDef Range);

	//	Set Tap Event in CNTL1 struct in KX224_TypeDef
KX224_Settings_CNTL1_TypeDef KX224_Set_Range(KX224_TypeDef* hkx224,
											 KX224_CNTL1_TDTE_TypeDef Tap_Event);

	//	Set Wake Up Function in CNTL1 struct in KX224_TypeDef
KX224_Settings_CNTL1_TypeDef KX224_Set_Wake_Up_Function(KX224_TypeDef* hkx224,
											   	   	    KX224_CNTL1_WUFE_TypeDef WakeUp_Event);

	//	Set Tilt Pos Function in CNTL1 struct in KX224_TypeDef
KX224_Settings_CNTL1_TypeDef KX224_Set_Tilt_Pos_Function(KX224_TypeDef* hkx224,
														 KX224_CNTL1_TPE_TypeDef Tilt_Pos_Event);

	//	Write CNTL1 Register in Accelerometer
KX224_Status_Function KX224_Write_Control_Main_Feature(KX224_TypeDef* hkx224);

/************************************************************************************/
/************************************************************************************/
/************************************************************************************/

	//	Read CNTL2 Register in Accelerometer
KX224_Status_Function KX224_Read_Control_Additional_Feature(KX224_TypeDef* hkx224);

	//	Set SoftWare Reset in CNTL2 struct in KX224_TypeDef
KX224_Settings_CNTL2_TypeDef KX224_Set_Reboot_RAM_Active(KX224_TypeDef* hkx224);

	//	Set Command Test Control in CNTL2 struct in KX224_TypeDef
KX224_Settings_CNTL2_TypeDef KX224_Set_Command_Test_Active(KX224_TypeDef* hkx224);

	//	Set Left Tilt Pos as an Interrupt in CNTL2 struct in KX224_TypeDef
KX224_Settings_CNTL2_TypeDef KX224_Set_Left_Tilt_Pos_Interrupt(KX224_TypeDef* hkx224,
															   KX224_CNTL2_LEM_TypeDef State);

	//	Set Right Tilt Pos as an Interrupt in CNTL2 struct in KX224_TypeDef
KX224_Settings_CNTL2_TypeDef KX224_Set_Right_Tilt_Pos_Interrupt(KX224_TypeDef* hkx224,
														   	    KX224_CNTL2_RIM_TypeDef State);

	//	Set Down Tilt Pos as an Interrupt in CNTL2 struct in KX224_TypeDef
KX224_Settings_CNTL2_TypeDef KX224_Set_Down_Tilt_Pos_Interrupt(KX224_TypeDef* hkx224,
														   	   KX224_CNTL2_DOM_TypeDef State);

	//	Set Up Tilt Pos as an Interrupt in CNTL2 struct in KX224_TypeDef
KX224_Settings_CNTL2_TypeDef KX224_Set_Up_Tilt_Pos_Interrupt(KX224_TypeDef* hkx224,
														   	 KX224_CNTL2_UPM_TypeDef State);

	//	Set Face Down Tilt Pos as an Interrupt in CNTL2 struct in KX224_TypeDef
KX224_Settings_CNTL2_TypeDef KX224_Set_Face_Down_Tilt_Pos_Interrupt(KX224_TypeDef* hkx224,
														   	   	    KX224_CNTL2_FDM_TypeDef State);

	//	Set Face Up Tilt Pos as an Interrupt in CNTL2 struct in KX224_TypeDef
KX224_Settings_CNTL2_TypeDef KX224_Set_Face_Up_Tilt_Pos_Interrupt(KX224_TypeDef* hkx224,
																  KX224_CNTL2_FUM_TypeDef State);

	//	Write CNTL2 Register in Accelerometer
KX224_Status_Function KX224_Write_Control_Additional_Feature(KX224_TypeDef* hkx224);

/************************************************************************************/
/************************************************************************************/
/************************************************************************************/

	//	Read CNTL3 Register in Accelerometer
KX224_Status_Function KX224_Read_Control_Rete_Digital_Engines(KX224_TypeDef* hkx224);

	//	Set Tilt Rate in CNTL3 struct in KX224_TypeDef
KX224_Settings_CNTL3_TypeDef KX224_Set_Tilt_Rate(KX224_TypeDef* hkx224,
												 KX224_CNTL3_Tilt_Pos_ODR_TypeDef Tilt_Rate);

	//	Set Tap Event Rate in CNTL3 struct in KX224_TypeDef
KX224_Settings_CNTL3_TypeDef KX224_Set_Tap_Event_Rate(KX224_TypeDef* hkx224,
												 KX224_CNTL3_Tap_Event_ODR_TypeDef Tap_Rate);

//	Set Wake Up Function Rate in CNTL3 struct in KX224_TypeDef
KX224_Settings_CNTL3_TypeDef KX224_Set_WakeUp_Rate(KX224_TypeDef* hkx224,
													  KX224_CNTL3_Wake_Up_ODR_TypeDef WakeUp_Rate);

	//	Write CNTL3 Register in Accelerometer
KX224_Status_Function KX224_Write_Control_Rete_Digital_Engines(KX224_TypeDef* hkx224);

/************************************************************************************/
/************************************************************************************/
/************************************************************************************/

	//	Read ODCNTL Register in Accelerometer
KX224_Status_Function KX224_Read_Control_Out_Data_Rete(KX224_TypeDef* hkx224);

	//	Set IIR Prescaler Filter for Streamline Range in ODCNTL struct in KX224_TypeDef
KX224_Settings_ODCNTL_TypeDef KX224_Set_Prescaler_Filter_Active(KX224_TypeDef* hkx224);

	//	Set IIR Prescaler Filter for Streamline Range in ODCNTL struct in KX224_TypeDef
KX224_Settings_ODCNTL_TypeDef KX224_Set_Prescaler_Filter_DeActive(KX224_TypeDef* hkx224);

	//	Set Filter Cutoff in ODCNTL struct in KX224_TypeDef
KX224_Settings_ODCNTL_TypeDef KX224_Set_Prescaler_9(KX224_TypeDef* hkx224);

	//	Set Filter Cutoff in ODCNTL struct in KX224_TypeDef
KX224_Settings_ODCNTL_TypeDef KX224_Set_Prescaler_2(KX224_TypeDef* hkx224);

	//	Write ODCNTL Register in Accelerometer
KX224_Status_Function KX224_Write_Control_Out_Data_Rete(KX224_TypeDef* hkx224);

/************************************************************************************/
/*			 				Flush Function											*/
/************************************************************************************/


	//	Read INT_REL Register in Accelerometer
void KX224_Interrupt_Release(KX224_TypeDef* hkx224);

/************************************************************************************/
/*			 	KX224 Low Layer Function Declaration								*/
/************************************************************************************/

	/* Write registers through SPI */
KX224_Status_Function kx224_write_spi(KX224_TypeDef* hkx224,
		uint8_t addr, void* buf, size_t size);

	/* Read registers through SPI */
KX224_Status_Function kx224_read_spi(KX224_TypeDef* hkx224,
		uint8_t addr, void* buf, size_t size);

	/* Write registers through SPI using DMA */
KX224_Status_Function kx224_write_spi_dma(KX224_TypeDef* hkx224,
		uint8_t addr, void* buf, size_t size);

	/* Read registers through SPI using DMA */
KX224_Status_Function kx224_read_spi_dma(KX224_TypeDef* hkx224,
		uint8_t addr, void* buf, size_t size);

#endif
