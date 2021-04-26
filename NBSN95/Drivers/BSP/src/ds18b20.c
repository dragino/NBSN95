#include "ds18b20.h"

void DS18B20_delay(uint16_t time)
{
	uint8_t i;

  while(time)
  {    
    for (i = 0; i < 4; i++);
    time--;
  }
}

void DS18B20_Mode_IPU(uint8_t num)
{
  GPIO_InitTypeDef GPIO_InitStruct;
	if(num==1)
	{
		__HAL_RCC_GPIOB_CLK_ENABLE();
		GPIO_InitStruct.Pin   = GPIO_PIN_3;
		GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull  = GPIO_PULLUP;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct); 
	}
  else if(num==2)
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();
		GPIO_InitStruct.Pin   = GPIO_PIN_9;
		GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull  = GPIO_PULLUP;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); 		
	}
	else if(num==3)
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();
		GPIO_InitStruct.Pin   = GPIO_PIN_10;
		GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull  = GPIO_PULLUP;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); 		
	}
}

void DS18B20_Mode_Out_PP(uint8_t num)
{
  GPIO_InitTypeDef GPIO_InitStruct;
	if(num==1)
	{	
		__HAL_RCC_GPIOB_CLK_ENABLE();
		GPIO_InitStruct.Pin = GPIO_PIN_3;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);   
  }
	else if(num==2)
	{	
		__HAL_RCC_GPIOA_CLK_ENABLE();
		GPIO_InitStruct.Pin = GPIO_PIN_9;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);   
  }
	else if(num==3)
	{	
		__HAL_RCC_GPIOA_CLK_ENABLE();
		GPIO_InitStruct.Pin = GPIO_PIN_10;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); 
  }	
}

void DS18B20_IoDeInit(uint8_t num)
{
	if(num==1)
	{
		GPIO_InitTypeDef GPIO_InitStruct;
		__HAL_RCC_GPIOB_CLK_ENABLE();

		GPIO_InitStruct.Pin = GPIO_PIN_3;
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	}
	else if(num==2)
	{
		GPIO_InitTypeDef GPIO_InitStruct;
		__HAL_RCC_GPIOA_CLK_ENABLE();

		GPIO_InitStruct.Pin = GPIO_PIN_9;
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);		
	}
	else if(num==3)
	{
		GPIO_InitTypeDef GPIO_InitStruct;
		__HAL_RCC_GPIOA_CLK_ENABLE();

		GPIO_InitStruct.Pin = GPIO_PIN_10;
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);		
	}
}

void DS18B20_Rst(uint8_t num)
{
	if(num==1)
	{
		DS18B20_Mode_Out_PP(1);		
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_RESET);
		DS18B20_delay(750);		
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_SET);					 
		DS18B20_delay(15);
	}
	else if(num==2)
	{
		DS18B20_Mode_Out_PP(2);	
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,GPIO_PIN_RESET);
		DS18B20_delay(750);	
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,GPIO_PIN_SET);				 
		DS18B20_delay(15);
	}
	else if(num==3)
	{
		DS18B20_Mode_Out_PP(3);		
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10,GPIO_PIN_RESET);
		DS18B20_delay(750);		
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10,GPIO_PIN_SET);				 
		DS18B20_delay(15);
	}
}

uint8_t DS18B20_Presence(uint8_t num)
{
		uint8_t pulse_time = 0;

		if(num==1)
		{        
			DS18B20_Mode_IPU(1);

		while( HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3) && pulse_time<100 )
		{
				pulse_time++;
				DS18B20_delay(1);
		}        

		if( pulse_time >=100 )
		{
			return 1;
		}
		else
			pulse_time = 0;
		
		
		while( !HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3) && pulse_time<240 )
		{
			pulse_time++;
			DS18B20_delay(1);
		}        
		if( pulse_time >=240 )
		{
			return 1;
		}
		else			
			return 0;
		}
		else if(num==2)
		{        
			DS18B20_Mode_IPU(2);

		while( HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_9) && pulse_time<100 )
		{
			pulse_time++;
			DS18B20_delay(1);
		}        

		if( pulse_time >=100 )
		{
			return 1;
		}
		else
			pulse_time = 0;
		
		
		while( !HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_9) && pulse_time<240 )
		{
			pulse_time++;
			DS18B20_delay(1);
		}        
		if( pulse_time >=240 )
		{
			return 1;
		}
		else			
			return 0;
		}
		else if(num==3)
		{        
			DS18B20_Mode_IPU(3);

			while( HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_10) && pulse_time<100 )
			{
				pulse_time++;
				DS18B20_delay(1);
			}        

			if( pulse_time >=100 )
			{
				return 1;
			}
			else
				pulse_time = 0;
		
		
			while( !HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_10) && pulse_time<240 )
			{
				pulse_time++;
				DS18B20_delay(1);
			}        
			if( pulse_time >=240 )
			{
				return 1;
			}
			else			
				return 0;
		}
		
		return 0;
}

uint8_t DS18B20_ReadBit(uint8_t num)
{
	uint8_t dat;

 if(num==1)
	{               
		DS18B20_Mode_Out_PP(1);

		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_RESET);
		DS18B20_delay(10);
		DS18B20_Mode_IPU(1);
		
		if( HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3)==SET)
			dat = 1;
		else
			dat = 0;
	}    
	else if(num==2)
	{               
		DS18B20_Mode_Out_PP(2);

		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,GPIO_PIN_RESET);
		DS18B20_delay(10);
		DS18B20_Mode_IPU(2);
		
		if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_9)==SET)
			dat = 1;
		else
			dat = 0;
	}  
	else if(num==3)
	{               
		DS18B20_Mode_Out_PP(3);

		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10,GPIO_PIN_RESET);
		DS18B20_delay(10);
		DS18B20_Mode_IPU(3);
		
		if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_10)==SET)
			dat = 1;
		else
			dat = 0;
	}  

	DS18B20_delay(45);
	
	return dat;
}

uint8_t DS18B20_ReadByte(uint8_t num)
{
	uint8_t i, j, dat = 0;        
	
	for(i=0; i<8; i++) 
	{
		j = DS18B20_ReadBit(num); 	               								
		dat = (dat) | (j<<i);
	}
	
	return dat;
}

void DS18B20_WriteBit(uint8_t dat,uint8_t num)  
{  
	if(num==1)
	{
		DS18B20_Mode_Out_PP(1);  
		if (dat)  
		{  
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_RESET);
			DS18B20_delay(8);  
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_SET);  
			DS18B20_delay(58);  
		}  
		else  
		{  
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_RESET); 
			DS18B20_delay(70);  
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_SET);  
			DS18B20_delay(2);  
		} 
	}
	else if(num==2)
	{
		DS18B20_Mode_Out_PP(2);  
		if (dat)  
		{  
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,GPIO_PIN_RESET); 
			DS18B20_delay(8);  
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,GPIO_PIN_SET);
			DS18B20_delay(58);  
		}  
		else  
		{  
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,GPIO_PIN_RESET); 
			DS18B20_delay(70);  
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,GPIO_PIN_SET);  
			DS18B20_delay(2);  
		} 
	}	
	else if(num==3)
	{
		DS18B20_Mode_Out_PP(3);  
		if (dat)  
		{  
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10,GPIO_PIN_RESET);
			DS18B20_delay(8);  
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10,GPIO_PIN_SET);  
			DS18B20_delay(58);  
		}  
		else  
		{  
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10,GPIO_PIN_RESET); 
			DS18B20_delay(70);  
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10,GPIO_PIN_SET);  
			DS18B20_delay(2);  
		} 
	}				
}

void DS18B20_WriteByte(uint8_t dat,uint8_t num)
{
	uint8_t i, testb;
	if(num==1)
	{
		DS18B20_Mode_Out_PP(1);

		for( i=0; i<8; i++ )
		{
			testb = dat&0x01;
			dat = dat>>1;                
		 
			if (testb)
			{                        
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_RESET);
				/* 1us < ???? < 15us */
				DS18B20_delay(8);
				
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_SET);
				DS18B20_delay(58);
			}                
			else
			{                        
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_RESET);
				/* 60us < Tx 0 < 120us */
				DS18B20_delay(70);
				
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_SET);                
				/* 1us < Trec(????) < ???*/
				DS18B20_delay(2);
			}
		}
	 }
	else if(num==2)
	{
		DS18B20_Mode_Out_PP(2);

		for( i=0; i<8; i++ )
		{
			testb = dat&0x01;
			dat = dat>>1;                
		 
			if (testb)
			{                        
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,GPIO_PIN_RESET);
				/* 1us < ???? < 15us */
				DS18B20_delay(8);
				
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,GPIO_PIN_SET);
				DS18B20_delay(58);
			}                
			else
			{                        
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,GPIO_PIN_RESET);
				/* 60us < Tx 0 < 120us */
				DS18B20_delay(70);
				
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,GPIO_PIN_SET);                
				/* 1us < Trec(????) < ???*/
				DS18B20_delay(2);
			}
		 }
	}
	else if(num==3)
	{
		DS18B20_Mode_Out_PP(3);

		for( i=0; i<8; i++ )
		{
			testb = dat&0x01;
			dat = dat>>1;                
		 
			if (testb)
			{                        
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10,GPIO_PIN_RESET);
				/* 1us < ???? < 15us */
				DS18B20_delay(8);
				
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10,GPIO_PIN_SET);
				DS18B20_delay(58);
			}                
			else
			{                        
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10,GPIO_PIN_RESET);
				/* 60us < Tx 0 < 120us */
				DS18B20_delay(70);
				
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10,GPIO_PIN_SET);               
				/* 1us < Trec(????) < ???*/
				DS18B20_delay(2);
			}
		 }
	}
}

void DS18B20_SkipRom(uint8_t num)
{
		DS18B20_Rst(num);                   
		DS18B20_Presence(num);                 
		DS18B20_WriteByte(0XCC,num);       
}

float DS18B20_GetTemp_SkipRom (uint8_t num)
{
	uint8_t tpmsb, tplsb;
	short s_tem;
	float f_tem;
	
	
	DS18B20_SkipRom(num);
	DS18B20_WriteByte(0X44,num);                                
	
	HAL_Delay(750);

	DS18B20_SkipRom (num);
	DS18B20_WriteByte(0XBE,num);                                
	
	tplsb = DS18B20_ReadByte(num);                 
	tpmsb = DS18B20_ReadByte(num); 
	
	if((tpmsb==5)&&(tplsb==80))     //1360= 00000101 01010000,tpmsb=00000101=5,tplsb=01010000=80;  
	{
		DS18B20_SkipRom(num);
		DS18B20_WriteByte(0X44,num);                                
		
		HAL_Delay(750);

		DS18B20_SkipRom (num);
		DS18B20_WriteByte(0XBE,num); 
			
		tplsb = DS18B20_ReadByte(num);                 
		tpmsb = DS18B20_ReadByte(num);
	}
	
	s_tem = tpmsb<<8;
	s_tem = s_tem | tplsb;
	
	if( s_tem < 0 )                
			f_tem = (~s_tem+1) * -0.0625;        
	else
			f_tem = s_tem * 0.0625;
	
	user_main_printf("DS18B20(%d) temp is %.2f ",num,f_tem);
	return f_tem;         
}
