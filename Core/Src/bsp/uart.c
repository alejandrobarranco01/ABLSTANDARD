#include "bsp/uart.h"

#include "main.h"

#include <string.h>

#define TX_MESSAGE_MAX_LEN (256U)   ///< Each message can be at most 255 bytes (+ null terminator)
#define TX_MESSAGE_QUEUE_SIZE (10U) ///< There can be 10 messages in the queue

static uint8_t message_buffer[TX_MESSAGE_QUEUE_SIZE][TX_MESSAGE_MAX_LEN]; ///< Queue of message buffers

static volatile uint8_t head = 0;    ///< Index of the next message to be transmitted
static volatile uint8_t current = 0; ///< Index of the next message to be written

int uart_send(UART_HandleTypeDef* huartx, uint8_t* message)
{
    if (huartx == NULL || message == NULL)
    {
        return -1;
    }

    uint16_t size = (uint16_t) strlen((char*) message);

    if (size == 0 || size >= TX_MESSAGE_MAX_LEN || (current - head) >= (uint8_t) (TX_MESSAGE_QUEUE_SIZE))
    {
        return -1;
    }

    memcpy(message_buffer[current], message, size + 1);

    if (current == head)
    {
        HAL_UART_Transmit_DMA(huartx, message_buffer[head % TX_MESSAGE_QUEUE_SIZE], size);
    }

    current++;

    return 0;
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef* huart)
{
    head++;

    if (head == current)
    {
        head = 0;
        current = 0;
    }
    else
    {
        uint16_t size = (uint16_t) strlen((char*) message_buffer[head % TX_MESSAGE_QUEUE_SIZE]);
        HAL_UART_Transmit_DMA(huart, message_buffer[head % TX_MESSAGE_QUEUE_SIZE], size + 1);
    }
}
