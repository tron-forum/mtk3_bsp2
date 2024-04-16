/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.0 BSP 2.0
 *
 *    Copyright (C) 2023-2024 by Ken Sakamura.
 *    This software is distributed under the T-License 2.1.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2024/03.
 *
 *----------------------------------------------------------------------
 */
#include <sys/machine.h>
#include <config_bsp/stm32_cube/config_bsp.h>

#ifdef MTKBSP_CPU_STM32G4
#if DEVCNF_USE_HAL_ADC

#include <stdlib.h>

#include <tk/tkernel.h>
#include <tk/device.h>

#include <sysdepend/stm32_cube/cpu_status.h>
#include <mtkernel/kernel/knlinc/tstdlib.h>
#include <mtkernel/device/common/drvif/msdrvif.h>
#include "../hal_adc_cnf.h"

/*
 *	hal_adc_stm32g4.c
 *	A/DC device driver (STM32G4)
*/
LOCAL const UW adc_cfg_ch[] = {
	ADC_CHANNEL_0, ADC_CHANNEL_1, ADC_CHANNEL_2, ADC_CHANNEL_3, ADC_CHANNEL_4, ADC_CHANNEL_5,
	ADC_CHANNEL_6, ADC_CHANNEL_7, ADC_CHANNEL_8, ADC_CHANNEL_9, ADC_CHANNEL_10, ADC_CHANNEL_11,
	ADC_CHANNEL_12, ADC_CHANNEL_13, ADC_CHANNEL_14, ADC_CHANNEL_15, ADC_CHANNEL_16, ADC_CHANNEL_17, 
};

EXPORT ER dev_adc_setch(ADC_HandleTypeDef *hadc, UW unit, W start)
{
	ADC_ChannelConfTypeDef	sConfig = {0};
	HAL_StatusTypeDef	hal_sts;

	if(start >= sizeof(adc_cfg_ch)/sizeof(UW)) return E_PAR;

	sConfig.Channel = adc_cfg_ch[start];
	if(unit == 0) {		// ADC1
		sConfig.Rank = ADC_REGULAR_RANK_1;
		sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
		sConfig.SingleDiff = ADC_SINGLE_ENDED;
		sConfig.OffsetNumber = ADC_OFFSET_NONE;
		sConfig.Offset = 0;
	}else if(unit == 1) {	// ADC2
		sConfig.Rank = ADC_REGULAR_RANK_1;
		sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
		sConfig.SingleDiff = ADC_SINGLE_ENDED;
		sConfig.OffsetNumber = ADC_OFFSET_NONE;
		sConfig.Offset = 0;
	} else {
		return E_IO;
	}

	hal_sts = HAL_ADC_ConfigChannel(hadc, &sConfig);

	return (hal_sts == HAL_OK)?E_OK:E_PAR;
}

#endif		/* DEVCNF_USE_HAL_ADC */
#endif		/* MTKBSP_CPU_STM32G4 */