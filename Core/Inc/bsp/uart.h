#ifndef UART_H
#define UART_H

#include "main.h"

#include <stdint.h>

int uart_send(UART_HandleTypeDef* huartx, uint8_t* message);
void HAL_UART_TxCpltCallback(UART_HandleTypeDef* huart);

#endif // UART_H
