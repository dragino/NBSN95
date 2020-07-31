#include "weight.h"

uint32_t HX711_Buffer=0;
uint32_t Weight_Maopi=0;
int32_t Weight_Shiwu=0;

void WEIGHT_SCK_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct={0};
  WEIGHT_SCK_CLK_ENABLE();
	//HX711_SCK
	GPIO_InitStruct.Pin = WEIGHT_SCK_PIN;				 
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; 		 
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;		
	HW_GPIO_Init(WEIGHT_SCK_PORT,WEIGHT_SCK_PIN,&GPIO_InitStruct);	
	HX711_SCK_0; 
}

void WEIGHT_DOUT_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct={0};
	WEIGHT_DOUT_CLK_ENABLE();
	//HX711_DOUT
	GPIO_InitStruct.Pin = WEIGHT_DOUT_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	HW_GPIO_Init(WEIGHT_DOUT_PORT,WEIGHT_DOUT_PIN,&GPIO_InitStruct);	
}


uint32_t HX711_Read(void)	
{
	uint32_t count; 
	uint8_t i; 
	HX711_SCK_0;  
	count=0; 
	while(HAL_GPIO_ReadPin(WEIGHT_DOUT_PORT,WEIGHT_DOUT_PIN)!=GPIO_PIN_RESET); 
	for(i=0;i<24;i++)
	{ 
	  	HX711_SCK_1; 	
	  	count=count<<1; 
			HX711_SCK_0; 
	  	if(HAL_GPIO_ReadPin(WEIGHT_DOUT_PORT,WEIGHT_DOUT_PIN)==GPIO_PIN_SET)
			count++;	
	} 
 	HX711_SCK_1; 
  count=count^0x800000;
	HX711_SCK_0; 
	return(count);
}

void Get_Maopi(void)
{
	Weight_Maopi = HX711_Read();
	user_main_debug("Weight_Maopi %d \r\n",Weight_Maopi);	
} 

int32_t Get_Weight(void)
{
	HX711_Buffer = HX711_Read();
	if(HX711_Buffer != Weight_Maopi)			
	{
		Weight_Shiwu = HX711_Buffer - Weight_Maopi;			
	
		Weight_Shiwu = (int32_t)((float)Weight_Shiwu/sensor.GapValue);
		if((Weight_Shiwu<-5000)||(Weight_Shiwu>5000))
		{
			Weight_Shiwu =0;
		}
	}
	else
	{
		Weight_Shiwu =0;
	}
//	user_main_printf("Weight is %d g\r\n",Weight_Shiwu);
	return Weight_Shiwu;
}

void HW_GPIO_Init( GPIO_TypeDef* port, uint16_t GPIO_Pin, GPIO_InitTypeDef* initStruct)
{

  RCC_GPIO_CLK_ENABLE(  (uint32_t) port);

  initStruct->Pin = GPIO_Pin ;

  HAL_GPIO_Init( port, initStruct );
}
