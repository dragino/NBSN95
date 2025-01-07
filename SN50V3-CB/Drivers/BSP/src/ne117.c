#include "ne117.h"
extern float tem_value;
extern uint16_t tem_store;
extern bool tdc_clock_log_flag;
extern bool tmp117_connect_status;

#define GPIO_PORT_TMP117_I2C	 GPIOB			
#define TMP117_I2C_SCL_PIN		 GPIO_PIN_6
#define TMP117_I2C_SDA_PIN		 GPIO_PIN_7

#define TMP117_I2C_SCL_1  HAL_GPIO_WritePin(GPIO_PORT_TMP117_I2C,TMP117_I2C_SCL_PIN,GPIO_PIN_SET)	
#define TMP117_I2C_SCL_0  HAL_GPIO_WritePin(GPIO_PORT_TMP117_I2C,TMP117_I2C_SCL_PIN,GPIO_PIN_RESET)			
	
#define TMP117_I2C_SDA_1  HAL_GPIO_WritePin(GPIO_PORT_TMP117_I2C,TMP117_I2C_SDA_PIN,GPIO_PIN_SET)		/* SDA = 1 */
#define TMP117_I2C_SDA_0  HAL_GPIO_WritePin(GPIO_PORT_TMP117_I2C, TMP117_I2C_SDA_PIN,GPIO_PIN_RESET)			/* SDA = 0 */
	
#define TMP117_I2C_SDA_READ()  HAL_GPIO_ReadPin(GPIO_PORT_TMP117_I2C, TMP117_I2C_SDA_PIN)	

void TMP117_I2C_GPIO_MODE_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  __HAL_RCC_GPIOB_CLK_ENABLE();
  GPIO_InitStruct.Pin = TMP117_I2C_SCL_PIN|TMP117_I2C_SDA_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;	
  HAL_GPIO_Init(GPIO_PORT_TMP117_I2C, &GPIO_InitStruct);
}

uint8_t TMP117_I2C_Write_Byte(uint8_t addr,uint8_t data)
{
    TMP117_I2C_Start();
    TMP117_I2C_SendByte((addr<<1)|0); 
    if(TMP117_I2C_WaitAck())          
    {
        TMP117_I2C_Stop();
        return 1;
    }

    TMP117_I2C_SendByte(data);        
    if(TMP117_I2C_WaitAck())         
    {
        TMP117_I2C_Stop();
        return 1;
    }
    TMP117_I2C_Stop();
    return 0;
}

uint8_t TMP117_I2C_Read_Byte(uint8_t addr)
{
    uint8_t res;

	  TMP117_I2C_Start();                
    TMP117_I2C_SendByte((addr<<1)|1); 
    TMP117_I2C_WaitAck();             
    res=TMP117_I2C_ReadByte(0);		
    TMP117_I2C_Stop();                 
    return res;  
} 

uint8_t TMP117_I2C_Write_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
{
    uint8_t i;
    TMP117_I2C_Start();
     TMP117_I2C_SendByte((addr<<1)|0); 
    if(TMP117_I2C_WaitAck())          
    {
        TMP117_I2C_Stop();
        return 1;
    }
     TMP117_I2C_SendByte(reg);        
    TMP117_I2C_WaitAck();             
    for(i=0;i<len;i++)
    {
         TMP117_I2C_SendByte(buf[i]); 
        if(TMP117_I2C_WaitAck())      
        {
            TMP117_I2C_Stop();
            return 1;
        }
    }
    TMP117_I2C_Stop();
    return 0;
} 

uint8_t TMP117_I2C_Read_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
{             
	  TMP117_I2C_Start();                
     TMP117_I2C_SendByte((addr<<1)|0); 
        if(TMP117_I2C_WaitAck())      
        {
            TMP117_I2C_Stop();
            return 1;
        }  

     TMP117_I2C_SendByte(reg);         
    TMP117_I2C_WaitAck(); 
			
	  TMP117_I2C_Start();                
     TMP117_I2C_SendByte((addr<<1)|1); 
    TMP117_I2C_WaitAck();
				
    while(len)
    {
        if(len==1)*buf=TMP117_I2C_ReadByte(0);
		else *buf=TMP117_I2C_ReadByte(1);		
		len--;
		buf++;  
    }
    TMP117_I2C_Stop();                 
    return 0;      
}

void TMP117_I2C_SDA_IN(void)
{
	static GPIO_InitTypeDef  GPIO_InitStruct;
	
	__HAL_RCC_GPIOB_CLK_ENABLE();
   
  GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Pin  = TMP117_I2C_SDA_PIN;

  HAL_GPIO_Init(GPIO_PORT_TMP117_I2C, &GPIO_InitStruct); 	
}

void TMP117_I2C_SDA_OUT(void)
{
	static GPIO_InitTypeDef  GPIO_InitStruct;
	
	__HAL_RCC_GPIOB_CLK_ENABLE();
   
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Pin  = TMP117_I2C_SDA_PIN;

  HAL_GPIO_Init(GPIO_PORT_TMP117_I2C, &GPIO_InitStruct); 	
}

void TMP117_I2C_Delay(void)
{
  volatile int i = 14;
    while (i)

    i--;
}
void TMP117_I2C_Start(void)
{
	TMP117_I2C_SDA_OUT();	
	TMP117_I2C_SDA_1;
	TMP117_I2C_SCL_1;
	TMP117_I2C_Delay();
	TMP117_I2C_SDA_0;
	TMP117_I2C_Delay();
	TMP117_I2C_SCL_0;
	TMP117_I2C_Delay();
}	
void TMP117_I2C_Stop(void)
{
	TMP117_I2C_SDA_OUT();
	TMP117_I2C_SCL_0;
	TMP117_I2C_SDA_0;
	TMP117_I2C_Delay();
	TMP117_I2C_SCL_1;
	TMP117_I2C_SDA_1;
	TMP117_I2C_Delay();
}
uint8_t TMP117_I2C_WaitAck(void)
{
	uint8_t ucErrTime=0;
  TMP117_I2C_SDA_IN();
	TMP117_I2C_SDA_1;	
	TMP117_I2C_Delay();
	TMP117_I2C_SCL_1;	
	TMP117_I2C_Delay();
	while (TMP117_I2C_SDA_READ())	
	{
			ucErrTime++;
		if(ucErrTime>250)
		{
			TMP117_I2C_Stop();
			return 1;
		}
	}

	TMP117_I2C_SCL_0;
	TMP117_I2C_Delay();
	return 0;
}
void TMP117_I2C_SendByte(uint8_t Byte)
{
	uint8_t i;
	TMP117_I2C_SDA_OUT();
	TMP117_I2C_SCL_0;
	for (i = 0; i < 8; i++)
	{		
		if (Byte & 0x80)
		{
			TMP117_I2C_SDA_1;
		}
		else
		{
			TMP117_I2C_SDA_0;
		}
		TMP117_I2C_Delay();
		TMP117_I2C_SCL_1;
		TMP117_I2C_Delay();	
		TMP117_I2C_Delay();
		TMP117_I2C_SCL_0;
//		TMP117_I2C_SDA_1; 
		Byte <<= 1;	
		TMP117_I2C_Delay();
	}
}
uint8_t TMP117_I2C_ReadByte(unsigned char ack)
{
	uint8_t i;
	uint8_t value;
  TMP117_I2C_SDA_IN();
	value = 0;
	for (i = 0; i < 8; i++)
	{
		value <<= 1;
		TMP117_I2C_SCL_0;
		TMP117_I2C_Delay();
		TMP117_I2C_Delay();
		TMP117_I2C_SCL_1;
		
		if (TMP117_I2C_SDA_READ())
		{
			value++;
		}
		TMP117_I2C_Delay();
	}
	
	if (!ack)
	{
			TMP117_I2C_NAck();
	}
	else
	{
			TMP117_I2C_Ack(); 
	}
		
	return value;
}	

void TMP117_I2C_Ack(void)
{
	TMP117_I2C_SCL_0;
	TMP117_I2C_SDA_OUT();
	TMP117_I2C_SDA_0;
	TMP117_I2C_Delay();
	TMP117_I2C_SCL_1;
	TMP117_I2C_Delay();
	TMP117_I2C_SCL_0;
}

void TMP117_I2C_NAck(void)
{
	TMP117_I2C_SCL_0;
	TMP117_I2C_SDA_OUT();
	TMP117_I2C_SDA_1;
	TMP117_I2C_Delay();
	TMP117_I2C_SCL_1;
	TMP117_I2C_Delay();
	TMP117_I2C_SCL_0;	
}

uint16_t TMP117_Set_Config(void)
{
	uint8_t data[2] = {0x0C, 0x20};
	uint8_t read_status=0,check_number=0;
	
	TMP117_I2C_GPIO_MODE_Config();
	
	do
	{
		read_status=1;
		check_number++;
	  if(TMP117_I2C_Write_Len(0x48,0x01,2,data)==1)
		{
			read_status=0;
						
			HAL_Delay(20);
		}
	}while(read_status==0&&check_number<4);
	
	return 0;
}

float get_tmp117_temp(void)
{
	short AD_code;
	float current_temp;
	uint8_t data[2] = {0x0C, 0x20};
	uint8_t read_status=0,check_number=0;
	
  do
	{
		read_status=1;
		check_number++;
	  if(TMP117_I2C_Write_Len(0x48,0x01,2,data)==1)
		{
			read_status=0;
						
			HAL_Delay(20);
		}
	}while(read_status==0&&check_number<4);
	
	if(read_status==1)
	{
		HAL_Delay(130);
	  check_number=0;
		do
		{
			read_status=1;
			check_number++;
			if(TMP117_I2C_Read_Len(0x48,0x00,2,data)==1)
			{
				read_status=0;
								
				HAL_Delay(20);
			}
		}while(read_status==0&&check_number<4);
	}
	
	if(read_status==1)
	{
		AD_code=(data[0]<<8)|data[1];
		
		if(AD_code <0)
		{
			current_temp=-(~AD_code+1)*0.0078125;
		}
		else 
			current_temp=AD_code*0.0078125;
		
		if(current_temp>150)
		{		
			current_temp=150;
		}
		else if(current_temp<-55)
		{		
			current_temp=-55;
		}
	}
	else
	{
		current_temp=327.67;
	}
	  tem_value=current_temp;
 		if(tdc_clock_log_flag==0)
	{
	user_main_printf("tem =%.2f C",current_temp);	
	}
	sensor.temSHT = (int)(current_temp*100);
 		if(tdc_clock_log_flag==1)
	{
	tem_store=(int)(current_temp*100);
	}	
	return current_temp;
}

void tmp117_connect(void)
{
	short AD_code;
	float current_temp;
	uint8_t data[2] = {0x0C, 0x20};
	uint8_t read_status=0,check_number=0;
	
  do
	{
		read_status=1;
		check_number++;
	  if(TMP117_I2C_Write_Len(0x48,0x01,2,data)==1)
		{
			read_status=0;
						
			HAL_Delay(20);
		}
	}while(read_status==0&&check_number<4);
	
	if(read_status==1)
	{
		HAL_Delay(130);
	  check_number=0;
		do
		{
			read_status=1;
			check_number++;
			if(TMP117_I2C_Read_Len(0x48,0x00,2,data)==1)
			{
				read_status=0;
								
				HAL_Delay(20);
			}
		}while(read_status==0&&check_number<4);
	}
	
	if(read_status==1)
	{
		tmp117_connect_status=1;
		AD_code=(data[0]<<8)|data[1];
		
		if(AD_code <0)
		{
			current_temp=-(~AD_code+1)*0.0078125;
		}
		else 
			current_temp=AD_code*0.0078125;
		
		if(current_temp>150)
		{
			tmp117_connect_status=0;			
		}
		else if(current_temp<-55)
		{
			tmp117_connect_status=0;			
		}
	}
	else
	{
  	tmp117_connect_status=0;
	}
}
void TMP117_soft_reset(void)
{
	uint8_t data[2] = {0x0C, 0x22};
	uint8_t read_status=0,check_number=0;
	
	do
	{
		read_status=1;
		check_number++;
	  if(TMP117_I2C_Write_Len(0x48,0x01,2,data)==1)
		{
			read_status=0;
			
			HAL_Delay(20);
		}
	}while(read_status==0&&check_number<4);
	
	HAL_Delay(20);
}
