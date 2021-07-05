#include "ultrasound.h"

void GPIO_ULT_INPUT_Init(void)	//ECHO
{
	GPIO_InitTypeDef GPIO_InitStruct={0};
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
	GPIO_InitStruct.Pin = GPIO_PIN_11;
	GPIO_InitStruct.Mode =GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;

  HAL_GPIO_Init( GPIOA, &GPIO_InitStruct);
}

void GPIO_ULT_OUTPUT_Init(void)	//TRIG
{
	GPIO_InitTypeDef GPIO_InitStruct={0};
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	GPIO_InitStruct.Pin = GPIO_PIN_12;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

  HAL_GPIO_Init( GPIOB,&GPIO_InitStruct);
}
/////////////////////////////////////////////////
void GPIO_ULT_INPUT_DeInit(void)
{
	GPIO_InitTypeDef GPIO_InitStruct={0};
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
	GPIO_InitStruct.Pin = GPIO_PIN_11;
	GPIO_InitStruct.Mode =GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;

  HAL_GPIO_Init( GPIOA, &GPIO_InitStruct);
}

void GPIO_ULT_OUTPUT_DeInit(void)
{
	GPIO_InitTypeDef GPIO_InitStruct={0};
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	GPIO_InitStruct.Pin = GPIO_PIN_12;
  GPIO_InitStruct.Mode  = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;

  HAL_GPIO_Init( GPIOB,&GPIO_InitStruct);
}
///////////////////////////////////////////
uint16_t ULT_distance(void)
{
	GPIO_ULT_INPUT_Init();
	GPIO_ULT_OUTPUT_Init();
	
	uint16_t distance = 0;
	uint8_t  ult_flags=0;
	
	if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11)==0)
	{
		 ult_flags=0;
	}
	else
	{
		 ult_flags=1;
	}
	
	if(ult_flags==0)
	{
		uint8_t error_num=0;
		while(error_num<10)
		{		
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET);
			HAL_Delay(1);	
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET);
			
			while(!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11));
			uint32_t time = HAL_GetTick();
			while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11));
			time= HAL_GetTick() - time;

			distance=(time*10000)/58;
			user_main_debug("time:%d",time);
			user_main_debug("distance:%d",distance);
			if((distance<240)||(distance>6000))
			{
				user_main_printf("Distance is out of range");
				error_num++;
				if(error_num==9)				
					break;
			}
			else
			{
				user_main_printf("Distance=%d mm",distance);
				return distance;
			}
		}
	}
	
	else
	{
		user_main_printf("ULT is not connect");
		return distance;
	}	
	
	return distance=0;
}


//uint16_t ULT_distance(void)
//{
//	GPIO_ULT_INPUT_Init();
//	GPIO_ULT_OUTPUT_Init();
//	MX_TIM2_Init();
//	
//	uint32_t time;
//	uint16_t distance = 0;
//	uint8_t  ult_flags=0;
//	
//	if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11)==0)
//	{
//		 ult_flags=0;
//	}
//	else
//	{
//		 ult_flags=1;
//	}
//	
//	if(ult_flags==0)
//	{
//		uint8_t error_num=0;
//		while(error_num<10)
//		{
//			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET);
//			HAL_Delay(1);	
//			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET);
//			
//			while(!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11));
//			__HAL_TIM_SET_COUNTER(&htim2,0);
//			__HAL_TIM_ENABLE(&htim2);
//			while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11));
//			time=__HAL_TIM_GetCounter(&htim2)+(int)((__HAL_TIM_GetCounter(&htim2))/33);

//			__HAL_TIM_DISABLE(&htim2);

//			distance=(170*(time*5/(float)1000)+5);

//			if((distance<240)||(distance>6000))
//			{
//				user_main_printf("Distance is out of range");
//				error_num++;
//				if(error_num==9)				
//					break;
//			}
//			else
//			{
//				user_main_printf("Distance=%d mm",distance);
//				return distance;
//			}
//		}
//	}
//	
//	else
//	{
//		user_main_printf("ULT is not connect");
//		return distance;
//	}	
//	
//	return distance=0;
//}
