#include "maxsonar.h"

uint8_t receive_data_ult[210] = {0};
static char data[100]={0};

void MAXSONAR_Rest(void)
{
	memset(receive_data_ult,0,sizeof(receive_data_ult));
	memset(data,0,sizeof(data));
}

void MAXSONAR_Init(void  (*uart_init)(UART_HandleTypeDef* uartHandle),void (*txs_init)(void))
{
	ULT_Rest();
	if(uart_init != NULL)
		uart_init(&huart1);
	if(txs_init!=NULL)
		txs_init();
}

void MAXSONAR_DeInit(void(*uart_disable)(UART_HandleTypeDef* uartHandle),void (*txs_deinit)(void))
{
	if(uart_disable != NULL)
		uart_disable(&huart1);	
	if(txs_deinit!=NULL)
		txs_deinit();
}

//uint8_t MAXSONAR_Connection_Test(UART_HandleTypeDef *UartHandle)
//{
//	uint8_t result = 0;
//	HAL_GPIO_WritePin(Power_5v_GPIO_Port, Power_5v_Pin, GPIO_PIN_SET);
//	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_SET);
//	HAL_UART_Receive(&huart1,receive_data_ult,100,1000);
//	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(Power_5v_GPIO_Port, Power_5v_Pin, GPIO_PIN_RESET);
//	if(strlen((char*)receive_data_ult) !=0)
//		result=1;
//	
//	return result;
//}

void MAXSONAR_getData()
{
	HAL_UART_Receive(&huart1,receive_data_ult,100,1000);
}

int MAXSONAR_Data_processing(void)
{
	char buff[10]={0};
	ULT_Check_Sum();
	int distance[5]={0};
	uint16_t distanceSum=0;
	for(uint8_t i=0;i<4;i++)
	{
		memset(buff,0,10);
		memcpy(buff,&data[6*i+1],4);
		distance[i] = atoi(buff);		
	}
	distanceSum = (distance[0]+distance[1]+distance[2]+distance[3])/4;
	printf("distanceSum:%d\r\n",distanceSum);
	return distanceSum;
}

void MAXSONAR_Data_Read(void)
{	
//	printf("receive_data_ult:%s\r\n",&receive_data_ult[1]);
}

void MAXSONAR_Check_Sum(void)
{
	uint8_t i=0;
	for(int a=0;a<200;a++)
	{
		if(receive_data_ult[a]=='R'&&(receive_data_ult[a+1]>='0' && receive_data_ult[a+1]<='9'))
		{
			memcpy(data+strlen(data),&receive_data_ult[a],6);
			i++;
			if(i==4)
				break;
		}
	}
//	printf("data:%s\r\n",data);
}