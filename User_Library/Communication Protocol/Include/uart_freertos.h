#ifndef UART_FREERTOS_H
#define UART_FREERTOS_H
#ifdef __cplusplus
 extern "C" {
#endif

/*----------------------------------------------------------------------
  Includes
----------------------------------------------------------------------*/

#include <stdint.h>
#include <stddef.h>

/* HAL */
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_uart.h"
/* FreeRTOS */
#include "FreeRTOS.h"
#include "semphr.h"

/*----------------------------------------------------------------------
  Exemple for uses interupts DMA IDLE
----------------------------------------------------------------------*/

#if FALSE
/* Necessary add this code to stm32fxxx_it.c  to USARTX_IRQHandler*/

 	if((READ_BIT(huartx.Instance->SR,USART_SR_IDLE) == USART_SR_IDLE))
	{
		uart_freertos_rx_idle_callback(&huartx);
		return;
	}


/* EXEMPLE for uart2*/
			/**
			* @brief This function handles USART2 global interrupt.
			*/

			void USART2_IRQHandler(void)
			{
  	  	  	  /* USER CODE BEGIN USART2_IRQn 0 */
				if((READ_BIT(huart2.Instance->SR,USART_SR_IDLE) == USART_SR_IDLE))
				{
					uart_freertos_rx_idle_callback(&huart2);
					return;
				}
  	  	  	  /* USER CODE END USART2_IRQn 0 */
  			HAL_UART_IRQHandler(&huart2);
  	  	  	  /* USER CODE BEGIN USART2_IRQn 1 */

  	  	  	  /* USER CODE END USART2_IRQn 1 */
			}
#endif

/*----------------------------------------------------------------------
  Defines
----------------------------------------------------------------------*/

/*----------------------------------------------------------------------
  Data type declarations
----------------------------------------------------------------------*/

/* Status codes */
typedef enum
{
	UART_FREERTOS_OK		= 0x00U,	// Success
	UART_FREERTOS_ERR		= 0x01U,	// Error
	UART_FREERTOS_NODEV		= 0x02U,	// No such device
	UART_FREERTOS_BUSY		= 0x03U,	// Device is busy
	UART_FREERTOS_TIMEOUT	= 0x04U,	// Timeout
	UART_FREERTOS_EXIST		= 0x05U,	// UART already exists
	UART_FREERTOS_IDLE		= 0x06U		// IDLE Timeout
} uart_freertos_status;

/* UART FreeRTOS structures */
typedef struct __packed
{
	/* UART interface */
	UART_HandleTypeDef		*huart;
	/* UART mutual exceptions */
	SemaphoreHandle_t		rx_mutex;
	SemaphoreHandle_t		tx_mutex;
	/* UART transfer complete semaphores */
	SemaphoreHandle_t		rx_complete;
	SemaphoreHandle_t		tx_complete;

} uart_freertos_t;

typedef struct __packed
{
	uint16_t rx_size;
	uart_freertos_status status;
}uart_freertos_status_t;

uart_freertos_status uart_freertos_init(uart_freertos_t* uart_rtos);

void uart_freertos_deinit(uart_freertos_t* uart_rtos);

uart_freertos_status uart_freertos_tx (uart_freertos_t* uart, const void* data, size_t data_size,
		TickType_t mutex_timeout, uint32_t transfer_timeout);

uart_freertos_status_t uart_freertos_rx (uart_freertos_t* uart, const void* data, size_t data_size,
		TickType_t mutex_timeout, uint32_t transfer_timeout);

uart_freertos_status uart_freertos_tx_it (uart_freertos_t* uart, const void* data, size_t data_size,
		TickType_t mutex_timeout, TickType_t transfer_timeout);

uart_freertos_status_t uart_freertos_rx_it (uart_freertos_t* uart, const void* data, size_t data_size,
		TickType_t mutex_timeout, TickType_t transfer_timeout);

uart_freertos_status uart_freertos_tx_dma (uart_freertos_t* uart, const void* data, size_t data_size,
		TickType_t mutex_timeout, TickType_t transfer_timeout);
uart_freertos_status_t uart_freertos_rx_dma (uart_freertos_t* uart, const void* data, size_t data_size,
		TickType_t mutex_timeout, TickType_t transfer_timeout);

uart_freertos_status_t uart_freertos_rx_dma_idle (uart_freertos_t* uart, const void* data, size_t data_size,
		TickType_t mutex_timeout,TickType_t expectation_timeout, TickType_t idle_timeout);

void uart_freertos_rx_idle_callback(UART_HandleTypeDef *huart);

#ifdef __cplusplus
}
#endif
#endif /* UART_FREERTOS_H */





