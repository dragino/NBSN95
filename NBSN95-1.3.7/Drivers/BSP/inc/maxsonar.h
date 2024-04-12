#ifndef __MAXSONAR_H
#define __MAXSONAR_H

#include "common.h"

void MAXSONAR_Rest(void);
int MAXSONAR_Data_processing(void);
void MAXSONAR_getData();
void MAXSONAR_Data_Read(void);
void MAXSONAR_Init(void (*uart_init)(UART_HandleTypeDef* uartHandle),void (*txs_init)(void));
void MAXSONAR_DeInit(void(*uart_disable)(UART_HandleTypeDef* uartHandle),void (*txs_deinit)(void));
void MAXSONAR_Check_Sum(void);
//uint8_t MAXSONAR_Connection_Test(UART_HandleTypeDef *UartHandle );
#endif
