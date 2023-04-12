#include "battery_read.h"

uint16_t HW_AdcReadChannel( uint32_t Channel )
{

  ADC_ChannelConfTypeDef adcConf;
  uint16_t adcData = 0;
  
	/* wait the the Vrefint used by adc is set */
	while (__HAL_PWR_GET_FLAG(PWR_FLAG_VREFINTRDY) == 0) {};
		
	__HAL_RCC_ADC1_CLK_ENABLE();
	
	/*calibrate ADC if any calibraiton hardware*/
	HAL_ADCEx_Calibration_Start(&hadc, ADC_SINGLE_ENDED );
	
	/* Deselects all channels*/
	adcConf.Channel = ADC_CHANNEL_MASK;
	adcConf.Rank = ADC_RANK_NONE; 
	HAL_ADC_ConfigChannel( &hadc, &adcConf);
		
	/* configure adc channel */
	adcConf.Channel = Channel;
	adcConf.Rank = ADC_RANK_CHANNEL_NUMBER;
	HAL_ADC_ConfigChannel( &hadc, &adcConf);

	/* Start the conversion process */
	HAL_ADC_Start( &hadc);
		
	/* Wait for the end of conversion */
	HAL_ADC_PollForConversion( &hadc, HAL_MAX_DELAY );
		
	/* Get the converted value of regular channel */
	adcData = HAL_ADC_GetValue ( &hadc);

	__HAL_ADC_DISABLE( &hadc);

	__HAL_RCC_ADC1_CLK_DISABLE();
	
  return adcData;
}


uint16_t getVoltage(void)
{
	uint16_t measuredLevel = HW_AdcReadChannel( ADC_CHANNEL_VREFINT );
	uint16_t batteryLevel_mV= (( (uint32_t) VDDA_VREFINT_CAL * (*VREFINT_CAL ) )/ measuredLevel);
	user_main_printf("remaining battery =%d mv",batteryLevel_mV);
	return batteryLevel_mV;
}

uint16_t ADCModel(uint32_t channel)
{
//	uint16_t measuredLevel = HW_AdcReadChannel( channel );
//	float adc_mV= measuredLevel*sensor.batteryLevel_mV/4096;
//	HAL_Delay(100);
//	if(channel == ADC_CHANNEL_0)
//		user_main_printf("adc_mV(1):%.2f",adc_mV);
//	else if(channel == ADC_CHANNEL_1)
//		user_main_printf("adc_mV(2):%.2f",adc_mV);
//	else if(channel == ADC_CHANNEL_4)
//		user_main_printf("adc_mV(3):%.2f",adc_mV);

//	uint16_t adc_mV_=adc_mV;
//	return adc_mV_;
	
	uint16_t buff[10]={0};
	for(uint8_t i=0;i<6;i++)
	{
		uint16_t measuredLevel = HW_AdcReadChannel( channel );
		float adc_mV= measuredLevel*sensor.batteryLevel_mV/4096;
		buff[i] = adc_mV;
		HAL_Delay(10);
	}

	stob(buff,6);

	float adc_mV_average = (buff[1]+buff[2]+buff[3]+buff[4])/4;
	if(channel == ADC_CHANNEL_0)
		user_main_printf("adc_mV(1):%.2f",adc_mV_average);
	else if(channel == ADC_CHANNEL_1)
		user_main_printf("adc_mV(2):%.2f",adc_mV_average);
	else if(channel == ADC_CHANNEL_4)
		user_main_printf("adc_mV(3):%.2f",adc_mV_average);

	uint16_t adc_mV_= adc_mV_average;
	return adc_mV_;
}

void stob(uint16_t* buf,int len)
{
	int t=0;
	for(int i=0;i<len;i++)
	{
		for(int j=i+1;j<len;j++)
		{
			if(buf[i]>buf[j])
			{
				t = buf[i];
				buf[i] = buf[j];
				buf[j] = t;
			}
		}
	}
}
