/*
 *----------------------------------------------------------------------
 *    Device Driver for μT-Kernel 3.0
 *
 *    Copyright (C) 2020-2024 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2024/04.
 *
 *----------------------------------------------------------------------
 */


/*
 *	adc_hal_xmc7200.c
 *	A/D converter device driver
 *	System dependent processing for XMC7200
 */
#include <sys/machine.h>
#include <config_bsp/xmc_mtb/config_bsp.h>

#include <cyhal.h>
#include <cybsp.h>
#include <cy_sysclk.h>
#include "adc_cnf_sysdep.h"

#ifdef MTKBSP_MODUSTOOLBOX
#if DEVCNF_USE_HAL_ADC

#define CH_VBG		(16U)
#define CH_CAL		(17U)

const cy_en_sar2_pin_address_t pin_address[] = {
	CY_SAR2_PIN_ADDRESS_AN20,	/* A0  = SAR1_AN20 (P14.0) */
	CY_SAR2_PIN_ADDRESS_AN21,	/* A1  = SAR1_AN21 (P14.1) */
	CY_SAR2_PIN_ADDRESS_AN22,	/* A2  = SAR1_AN22 (P14.2) */
	CY_SAR2_PIN_ADDRESS_AN23,	/* A3  = SAR1_AN23 (P14.3) */
	CY_SAR2_PIN_ADDRESS_AN24,	/* A4  = SAR1_AN24 (P14.4) */
	CY_SAR2_PIN_ADDRESS_AN25,	/* A5  = SAR1_AN25 (P14.5) */
	CY_SAR2_PIN_ADDRESS_AN26,	/* A6  = SAR1_AN26 (P14.6) */
	CY_SAR2_PIN_ADDRESS_AN27,	/* A7  = SAR1_AN27 (P14.7) */
	CY_SAR2_PIN_ADDRESS_AN4,	/* A8  = SAR1_AN4  (P12.0) */
	CY_SAR2_PIN_ADDRESS_AN5,	/* A9  = SAR1_AN5  (P12.1) */
	CY_SAR2_PIN_ADDRESS_AN6,	/* A10 = SAR1_AN6  (P12.2) */
	CY_SAR2_PIN_ADDRESS_AN7,	/* A11 = SAR1_AN7  (P12.3) */
	CY_SAR2_PIN_ADDRESS_AN8,	/* A12 = SAR1_AN8  (P12.4) */
	CY_SAR2_PIN_ADDRESS_AN9,	/* A13 = SAR1_AN9  (P12.5) */
	CY_SAR2_PIN_ADDRESS_AN10,	/* A14 = SAR1_AN10 (P12.6) */
	CY_SAR2_PIN_ADDRESS_AN11,	/* A15 = SAR1_AN11 (P12.7) */
	CY_SAR2_PIN_ADDRESS_VBG,	/* VBG */
};

/* Scenario: SAR2 block 1 with it's clock source is configured, channel 17 is
 * not used. */
/* Calibration initial setting. */
cy_stc_sar2_channel_config_t calibration_channel_config = 
{
	.channelHwEnable = true,
	.triggerSelection = CY_SAR2_TRIGGER_OFF,
	.channelPriority = 0U,
	.preenptionType = CY_SAR2_PREEMPTION_ABORT_RESUME,
	.isGroupEnd = true,
	.pinAddress = 0U,
	.portAddress = 0U,
	.extMuxEnable = false,
	.extMuxSelect = 0U,
	.preconditionMode = CY_SAR2_PRECONDITION_MODE_OFF,
	.overlapDiagMode = CY_SAR2_OVERLAP_DIAG_MODE_OFF,
	.sampleTime = DEVCONF_SAR1_SMPTIME,
	.postProcessingMode = CY_SAR2_POST_PROCESSING_MODE_NONE,
	.resultAlignment = CY_SAR2_RESULT_ALIGNMENT_RIGHT,
	.signExtention = CY_SAR2_SIGN_EXTENTION_UNSIGNED,
	.averageCount = 1U,
	.rightShift = 0U,
	.positiveReload = 0U,
	.negativeReload = 0U,
	.rangeDetectionMode = CY_SAR2_RANGE_DETECTION_MODE_INSIDE_RANGE,
	.rangeDetectionLoThreshold = 0U,
	.rangeDetectionHiThreshold = 4095U,
};

static cy_stc_sar2_analog_calibration_conifg_t calibrationConfig =
{
	.offset = 127ul,/* Offset Calibration start setting */
	.gain   = 15ul,/* Gain Calibration start setting */
};

/*******************************************************************************
* Function Name: getAdcValue
********************************************************************************
*
* Issues software trigger for the channel 2, waits for SAR2 block to complete
* it's calculation and returns calculated value.
*
* \return
* Measurement value
*
*******************************************************************************/
static uint16_t getAdcValue(void)
{
	/* Trigger ADC. */
	Cy_SAR2_Channel_SoftwareTrigger(PASS0_SAR1, CH_CAL);
	/* Wait for group of single channel to complete. */
	while (CY_SAR2_INT_GRP_DONE != Cy_SAR2_Channel_GetInterruptStatus(PASS0_SAR1, CH_CAL));
	Cy_SAR2_Channel_ClearInterrupt(PASS0_SAR1, CH_CAL, CY_SAR2_INT_GRP_DONE);
	/* Get the result(s) */
	return (Cy_SAR2_Channel_GetResult(PASS0_SAR1, CH_CAL, NULL));
}
/*******************************************************************************
* Function Name: offset_Calibration
********************************************************************************
*
* Performs SAR block offset calibration
*
* \return
* Boolean value if calibration was successful.
*
*******************************************************************************/
static bool offset_Calibration(void)
{
	/* Start ADC conversion and get the ADC value */
	int16_t result;
	int16_t offset_VREFH = 0U;
	int16_t offset_VREFL = 0U;
	int32_t value = 0U;
	calibration_channel_config.pinAddress = CY_SAR2_PIN_ADDRESS_VREF_L;
	Cy_SAR2_SetAnalogCalibrationValue(PASS0_SAR1, &calibrationConfig);
	/* Initialize channel. */
	Cy_SAR2_Channel_Init(PASS0_SAR1, CH_CAL, &calibration_channel_config);
	
	result = getAdcValue();
	if(result > 0)
	{
		/* Decrement the Value of Offset Compensation by "1" */
		for(offset_VREFL = 127; offset_VREFL >= -128; offset_VREFL -= 1)
		{
			/* Set "offset_VREFL" to offset register value */
			calibrationConfig.offset = offset_VREFL;
			Cy_SAR2_SetAnalogCalibrationValue(PASS0_SAR1, &calibrationConfig);
			/* Get ADC conversion value. */
			result = getAdcValue();
			/* Does the ADC conversion value equal to 0? */
			if(result == 0)
			{
				break;
			}
			/* If offset has reached lower bound. */
			if(offset_VREFL == -128)
			{
				/* Report error. */
				return false;
			}
		}
	}
	else
	{
		/* End of Offset Adjustment with Abnormality. */
		return false;
	}
	/* Set VREFH Voltage to Input */
	calibration_channel_config.pinAddress = CY_SAR2_PIN_ADDRESS_VREF_H;
	/* Set Offset Compensation to "-128" and init channel. */
	calibrationConfig.offset = -128;
	Cy_SAR2_SetAnalogCalibrationValue(PASS0_SAR1, &calibrationConfig);
	Cy_SAR2_Channel_Init(PASS0_SAR1, CH_CAL, &calibration_channel_config);
	/* Start ADC Conversion and get the A/D value. */
	result = getAdcValue();
	/* Is the ADC Conversion value lower than 0xFFF */
	if(result < 0xFFF)
	{
		/* Increment the Value of Offset Compensation by "1". */
		for(offset_VREFH = -128; offset_VREFH <= 127; offset_VREFH += 1)
		{
			/* Set "offset_VREFH" to offset register value. */
			calibrationConfig.offset = offset_VREFH;
			Cy_SAR2_SetAnalogCalibrationValue(PASS0_SAR1, &calibrationConfig);
			/* Get ADC Conversion value. */
			result = getAdcValue();
			/* Does the ADC Conversion value Equal to "4095"? */
			if(result == 0xFFF)
			{
				break;
			}
			/* Does the value of offset compensation equal to "127"? */
			if(offset_VREFH == 127)
			{
				/* Offset adjustment failed - report error. */
				return false;
			}
		}
	}
	else
	{
		/* Offset adjustment failed - report error. */
		return false;
	}
	/* Does "((offset_VREFH+offset_VREFL)/2 + 2)" greater than "125" ?*/
	value = ((offset_VREFH+offset_VREFL)/2 + 2);
	if(value > 125)
	{
		/* Offset adjustment failed - report error. */
		return false;
	}
	/* Set the value of offset compensation to ((offset_VREFH+offset_VREFL)/2 + 2) */
	calibrationConfig.offset = ((offset_VREFH+offset_VREFL)/2 + 2);

	return true;
}

/* Scenario: SAR2 block 1 is left after offset calibration intact, global
 * structure is still available with offset calibration results. */
/*******************************************************************************
* Function Name: gain_Calibration
********************************************************************************
*
* Performs SAR block gain calibration
*
* \return
* Boolean value if calibration was successful.
*
*******************************************************************************/
static bool gain_Calibration(void)
{
	int16_t gain;
	int16_t result = 0U;
	/* Set Gain Correction setting to "15". */
	calibrationConfig.gain = 15;
	/* Set VREFL Voltage to Input and init the channel. */
	calibration_channel_config.pinAddress = CY_SAR2_PIN_ADDRESS_VREF_L;
	Cy_SAR2_Channel_Init(PASS0_SAR1, CH_CAL, &calibration_channel_config);
	/* Apply the initial calibration settings. */
	Cy_SAR2_SetAnalogCalibrationValue(PASS0_SAR1, &calibrationConfig);
	/* Start ADC value Conversion */
	result = getAdcValue();
	if(result > 0)
	{
		for(gain = 15; gain >=-14; gain -= 1)
		{
			/* Set gain to register */
			calibrationConfig.gain = gain;
			Cy_SAR2_SetAnalogCalibrationValue(PASS0_SAR1, &calibrationConfig);
			result = getAdcValue();
			if(result == 0)
			{
				/* Set gain compensation to "gain - 1" */
				calibrationConfig.gain = gain - 1;
				Cy_SAR2_SetAnalogCalibrationValue(PASS0_SAR1, &calibrationConfig);
				break;
			}
			if(gain == -14)
			{
				/* Gain adjustment failed - report error. */
				return false;
			}
		}
	}
	return true;
}

/*----------------------------------------------------------------------
 * Interrupt handler
 */
void adc_hal_clear_interrupt( void )
{
	/* Clear Interrupt. */
	Cy_SAR2_Channel_ClearInterrupt(PASS0_SAR1, CH_VBG, CY_SAR2_INT_GRP_DONE);
}

/*----------------------------------------------------------------------
 * Start A/D convert 
 */
void adc_hal_start_convert( void )
{
	Cy_SAR2_Channel_SoftwareTrigger(PASS0_SAR1, 0);
}

/*----------------------------------------------------------------------
 * Get A/D convert Result.
 */
unsigned int adc_hal_get_result( unsigned int ch)
{
	return Cy_SAR2_Channel_GetResult(PASS0_SAR1, ch, NULL);
}

/*----------------------------------------------------------------------
 * A/DC open
 */
int adc_hal_open( void )
{
	/* Do offset calibration. */
	if (offset_Calibration() != true) {
		return -1;
	}
	
	/* Do gain calibration. */
	if (gain_Calibration() != true) {
		return -1;
	}
	
	/* Finalize the calibration channel. */
	Cy_SAR2_Channel_DeInit(PASS0_SAR1, CH_CAL);

	return 0;
}

/*----------------------------------------------------------------------
 * A/DC close
 */
void adc_hal_close()
{
	/* Disable ADC */
	Cy_SAR2_DeInit(PASS0_SAR1);
}

/*----------------------------------------------------------------------
 * A/DC Clock initialization
 */
void adc_hal_init_clock( void )
{
	uint8_t div;
	
	/* Scenario:  Use one of the PeriClk dividers with a divider value of 50 to get a 4 Mhz SAR clock. */
	/* Assign divider number 0 of the the 8-bit dividers to the SAR2, block 0. */
	Cy_SysClk_PeriphAssignDivider(PCLK_PASS0_CLOCK_SAR1, CY_SYSCLK_DIV_8_BIT, 0U);
	/* Disable assigned divider. */
	Cy_SysClk_PeriPclkDisableDivider((1U << PERI_PCLK_GR_NUM_Pos), CY_SYSCLK_DIV_8_BIT, 0U);
	
	div = (Cy_SysClk_ClkHfGetFrequency(2) / 4000000) - 1;
	/* Divide the PeriClk by 50, which is a input value of 49. */
	Cy_SysClk_PeriPclkSetDivider((1U << PERI_PCLK_GR_NUM_Pos), CY_SYSCLK_DIV_8_BIT, 0U, div);
	/* Enable the divider. */
	Cy_SysClk_PeriPclkEnableDivider((1U << PERI_PCLK_GR_NUM_Pos), CY_SYSCLK_DIV_8_BIT, 0U);
	
	return;
}

/*----------------------------------------------------------------------
 * Device initialization
 */
void adc_hal_init( void )
{
	int i;
	
	/* Initialize Port */
	
	Cy_GPIO_Pin_Init(CYBSP_A0_PORT, CYBSP_A0_PIN, &CYBSP_A0_config);
	Cy_GPIO_Pin_Init(CYBSP_A1_PORT, CYBSP_A1_PIN, &CYBSP_A1_config);
	Cy_GPIO_Pin_Init(CYBSP_A2_PORT, CYBSP_A2_PIN, &CYBSP_A2_config);
	Cy_GPIO_Pin_Init(CYBSP_A3_PORT, CYBSP_A3_PIN, &CYBSP_A3_config);
	Cy_GPIO_Pin_Init(CYBSP_A4_PORT, CYBSP_A4_PIN, &CYBSP_A4_config);
	Cy_GPIO_Pin_Init(CYBSP_A5_PORT, CYBSP_A5_PIN, &CYBSP_A5_config);
	Cy_GPIO_Pin_Init(CYBSP_A6_PORT, CYBSP_A6_PIN, &CYBSP_A6_config);
	Cy_GPIO_Pin_Init(CYBSP_A7_PORT, CYBSP_A7_PIN, &CYBSP_A7_config);
	Cy_GPIO_Pin_Init(CYBSP_A8_PORT, CYBSP_A8_PIN, &CYBSP_A8_config);
	Cy_GPIO_Pin_Init(CYBSP_A9_PORT, CYBSP_A9_PIN, &CYBSP_A9_config);
	Cy_GPIO_Pin_Init(CYBSP_A10_PORT, CYBSP_A10_PIN, &CYBSP_A10_config);
	Cy_GPIO_Pin_Init(CYBSP_A11_PORT, CYBSP_A11_PIN, &CYBSP_A11_config);
	Cy_GPIO_Pin_Init(CYBSP_A12_PORT, CYBSP_A12_PIN, &CYBSP_A12_config);
	Cy_GPIO_Pin_Init(CYBSP_A13_PORT, CYBSP_A13_PIN, &CYBSP_A13_config);
	Cy_GPIO_Pin_Init(CYBSP_A14_PORT, CYBSP_A14_PIN, &CYBSP_A14_config);
	Cy_GPIO_Pin_Init(CYBSP_A15_PORT, CYBSP_A15_PIN, &CYBSP_A15_config);
	
	cy_stc_sar2_channel_config_t channel_config = {
		.channelHwEnable = true,
		.triggerSelection = CY_SAR2_TRIGGER_OFF,
		.channelPriority = 0U,
		.preenptionType = CY_SAR2_PREEMPTION_FINISH_RESUME,
		.isGroupEnd = false,
		.pinAddress = CY_SAR2_PIN_ADDRESS_AN20,
		.portAddress = CY_SAR2_PORT_ADDRESS_SARMUX1,
		.extMuxEnable = true,
		.extMuxSelect = 0U,
		.preconditionMode = CY_SAR2_PRECONDITION_MODE_OFF,
		.overlapDiagMode = CY_SAR2_OVERLAP_DIAG_MODE_OFF,
		.sampleTime = DEVCONF_SAR1_SMPTIME,
		.postProcessingMode = CY_SAR2_POST_PROCESSING_MODE_NONE,
		.resultAlignment = CY_SAR2_RESULT_ALIGNMENT_RIGHT,
		.signExtention = CY_SAR2_SIGN_EXTENTION_UNSIGNED,
		.averageCount = 1U,
		.rightShift = 0U,
		.rangeDetectionMode = CY_SAR2_RANGE_DETECTION_MODE_INSIDE_RANGE,
		.rangeDetectionLoThreshold = 0U,
		.rangeDetectionHiThreshold = 65535U,
	};
	cy_stc_sar2_config_t sar2_config = {
		.preconditionTime = 0U,
		.powerupTime = 0U,
		.enableIdlePowerDown = false,
		.msbStretchMode = CY_SAR2_MSB_STRETCH_MODE_1CYCLE,
		.enableHalfLsbConv = false,
		.sarMuxEnable = true,
		.adcEnable = true,
		.sarIpEnable = true,
		.channelConfig = {0},
	};
	
	/* Scenario: SAR2 ADC initialization */
	/* Initialize the SAR1 module */
	Cy_SAR2_Init(PASS0_SAR1, &sar2_config);
	/* Set ePASS MMIO reference buffer mode for bangap voltage */
	Cy_SAR2_SetReferenceBufferMode(PASS0_EPASS_MMIO, CY_SAR2_REF_BUF_MODE_ON);
	
	/* Initialize A/DC channels. */
	for (i = 0; i < (sizeof(pin_address) / sizeof(cy_en_sar2_pin_address_t)); i++) {
		channel_config.pinAddress = pin_address[i];
		if (i + 1 >= (sizeof(pin_address) / sizeof(cy_en_sar2_pin_address_t))) {
			channel_config.isGroupEnd = true;
			channel_config.interruptMask = CY_SAR2_INT_GRP_DONE;
		}
		Cy_SAR2_Channel_Init(PASS0_SAR1, i, &channel_config);
	}

	return;
}

#endif		/* DEVCNF_USE_HAL_ADC */
#endif		/* MTKBSP_MODUSTOOLBOX */
