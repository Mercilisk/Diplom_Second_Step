
/* FreeRTOS */
#include "stm32f1xx_hal.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "stm32f1xx_hal_uart.h"

//#include "dma.h"
#include "uart_freertos.h"

/* UART FreeRTOS list item structure */
struct uart_rtos_list
{
	uart_freertos_t			*uart_rtos;
	struct uart_rtos_list		*next;
};
/* Root of UART FreeRTOS list */
static struct uart_rtos_list *uart_rtos_list_begin = NULL;

/* Append the item at the end of UART FreeRTOS list */
static struct uart_rtos_list* uart_rtos_list_append(uart_freertos_t *uart_rtos)
{
	/* if list is empty, create and return the begin of list */
	if(uart_rtos_list_begin == NULL)
	{
		uart_rtos_list_begin = (struct uart_rtos_list*)
			pvPortMalloc(sizeof(struct uart_rtos_list));
		uart_rtos_list_begin->uart_rtos = uart_rtos;
		uart_rtos_list_begin->next = NULL;
		return uart_rtos_list_begin;
	}
	/* else find end of list */
	struct uart_rtos_list *item = uart_rtos_list_begin;
	while(item->next != NULL)
		item = item->next;
	/* and create and append item at the end of list */
	item->next = (struct uart_rtos_list*)
		pvPortMalloc(sizeof(struct uart_rtos_list));
	item->next->uart_rtos = uart_rtos;
	item->next->next = NULL;
	return item->next;
}

/* Delete the item from UART FreeRTOS list */
static struct uart_rtos_list* uart_rtos_list_delete
	(uart_freertos_t *uart_rtos)
{
	struct uart_rtos_list *item = uart_rtos_list_begin, *tmp;
	/* find list item */
	while(item != NULL)
	{
		/* if item found, delete and replace next item pointer */
		if(item->next->uart_rtos == uart_rtos)
		{
			tmp = item->next;
			item->next = tmp->next;
			vPortFree(tmp);
			break;
		}
		else
			item = item->next;
	}
	return item;
}

/* Find item in SPI FreeRTOS list with mached SPI_HandleTypeDef */
static struct uart_rtos_list* uart_rtos_list_find_item
	(UART_HandleTypeDef* huart)
{

	struct uart_rtos_list *item = uart_rtos_list_begin;
	while(item != NULL)
	{
		if(item->uart_rtos->huart == huart)
			break;
		else
			item = item->next;
	}
	return item;
}

/* Initialize UART with FreeRTOS mutexes and semaphores */
uart_freertos_status uart_freertos_init(uart_freertos_t* uart_rtos)
{
	/* find hspi into list */
	if(uart_rtos_list_find_item(uart_rtos->huart) != NULL)
		return UART_FREERTOS_EXIST;
	/* if hspi not found, create semaphores and mutexes */
	uart_rtos->tx_mutex = xSemaphoreCreateMutex();
	uart_rtos->rx_mutex = xSemaphoreCreateMutex();
	uart_rtos->tx_complete = xSemaphoreCreateBinary();
	uart_rtos->rx_complete = xSemaphoreCreateBinary();

	/* register spi_freertos_base into list */
	uart_rtos_list_append(uart_rtos);
	return UART_FREERTOS_OK;
}

/* Deinitialize UART with FreeRTOS mutexes and semaphores */
void uart_freertos_deinit(uart_freertos_t* uart_rtos)
{
	/* remove semaphores, mutexes */
	vSemaphoreDelete(uart_rtos->tx_mutex);
	vSemaphoreDelete(uart_rtos->rx_mutex);
	vSemaphoreDelete(uart_rtos->tx_complete);
	vSemaphoreDelete(uart_rtos->rx_complete);
	/* unregister spi_freertos_base from list */
	uart_rtos_list_delete(uart_rtos);
}

/* Parse HAL status */
static inline uart_freertos_status parse_hal_status(HAL_StatusTypeDef status)
{
  switch(status)
  {
  case HAL_ERROR:
    return UART_FREERTOS_ERR;
  case HAL_BUSY:
    return UART_FREERTOS_BUSY;
  case HAL_TIMEOUT:
    return UART_FREERTOS_TIMEOUT;
  default:
    return UART_FREERTOS_OK;
  }
}

/* Transmit data through UART whithout interupts */
uart_freertos_status uart_freertos_tx (uart_freertos_t* uart, const void* data, size_t data_size, TickType_t mutex_timeout, uint32_t transfer_timeout)
{
	uart_freertos_status rtn;

	if(xSemaphoreTake(uart->tx_mutex, mutex_timeout) == pdFALSE)
	{
		return UART_FREERTOS_BUSY;
	}

	rtn = parse_hal_status (HAL_UART_Transmit(uart->huart,(void*) data, data_size, transfer_timeout));

	/* Give back UART mutex */
	xSemaphoreGive(uart->tx_mutex);

	return rtn;
}

/* Recieve data through UART whithout interupts */
uart_freertos_status_t uart_freertos_rx (uart_freertos_t* uart, const void* data, size_t data_size, TickType_t mutex_timeout, uint32_t transfer_timeout)
{
	uart_freertos_status_t rtn;
	rtn.status = UART_FREERTOS_OK;
	rtn.rx_size = 0;

	if(xSemaphoreTake(uart->rx_mutex, mutex_timeout) == pdFALSE)
	{
		rtn.status = UART_FREERTOS_BUSY;
		return rtn;
	}
	rtn.status = parse_hal_status (HAL_UART_Receive(uart->huart,(void*) data, data_size, transfer_timeout));
	rtn.rx_size = data_size - uart->huart->RxXferCount;
	/* Give back UART mutex */
	xSemaphoreGive(uart->rx_mutex);

	return rtn;
}


/* Transmit data through UART with interupts */
uart_freertos_status uart_freertos_tx_it (uart_freertos_t* uart, const void* data, size_t data_size, TickType_t mutex_timeout, TickType_t transfer_timeout)
{
	uart_freertos_status rtn = UART_FREERTOS_OK;

	if(xSemaphoreTake(uart->tx_mutex, mutex_timeout) == pdFALSE)
	{
		rtn = UART_FREERTOS_BUSY;
		goto exit;
	}

	rtn =  parse_hal_status ( HAL_UART_Transmit_IT(uart->huart,(void*) data, data_size));

	if ((rtn == UART_FREERTOS_ERR) || (rtn == UART_FREERTOS_BUSY) ) goto end_of_transaction;

	/* Waiting for tx complete */
	if(xSemaphoreTake(uart->tx_complete,transfer_timeout) == pdFALSE)
	{
		rtn = UART_FREERTOS_TIMEOUT;
		goto end_of_transaction;
	}

	end_of_transaction:

	/* Give back UART mutex */
	xSemaphoreGive(uart->tx_mutex);

	exit:
	return rtn;
}

/* Recieve data through UART with interupts */
uart_freertos_status_t uart_freertos_rx_it (uart_freertos_t* uart, const void* data, size_t data_size,
		TickType_t mutex_timeout, TickType_t transfer_timeout)
{
	uart_freertos_status_t rtn;
	rtn.status = UART_FREERTOS_OK;
	rtn.rx_size = 0;

	if(xSemaphoreTake(uart->rx_mutex, mutex_timeout) == pdFALSE)
	{
		rtn.status = UART_FREERTOS_BUSY;
		goto exit;
	}

	rtn.status =  parse_hal_status ( HAL_UART_Receive_IT(uart->huart,(void*) data, data_size));

	if ((rtn.status == UART_FREERTOS_ERR) || (rtn.status == UART_FREERTOS_BUSY) ) goto end_of_transaction;

	/* Waiting for tx complete */
	if(xSemaphoreTake(uart->rx_complete,transfer_timeout) == pdFALSE)
	{
		rtn.status = UART_FREERTOS_TIMEOUT;
		rtn.rx_size = data_size -uart->huart->RxXferCount;
		HAL_UART_AbortReceive_IT(uart->huart);
	}
	else
	{
		rtn.rx_size = data_size -uart->huart->RxXferCount;
	}

	end_of_transaction:

	/* Give back UART mutex */
	xSemaphoreGive(uart->rx_mutex);

	exit:
	return rtn;
}


/* Transmit data through UART whithout interupts */
uart_freertos_status uart_freertos_tx_dma (uart_freertos_t* uart, const void* data, size_t data_size, TickType_t mutex_timeout, TickType_t transfer_timeout)
{
	uart_freertos_status rtn = UART_FREERTOS_OK;

	if(xSemaphoreTake(uart->tx_mutex, mutex_timeout) == pdFALSE)
	{
		rtn = UART_FREERTOS_BUSY;
		goto exit;
	}

	rtn =  parse_hal_status ( HAL_UART_Transmit_DMA(uart->huart,(void*) data, data_size));

	if ((rtn == UART_FREERTOS_ERR) || (rtn == UART_FREERTOS_BUSY) ) goto end_of_transaction;

	/* Waiting for tx complete */
	if(xSemaphoreTake(uart->tx_complete,transfer_timeout) == pdFALSE)
	{
		rtn = UART_FREERTOS_TIMEOUT;
		goto end_of_transaction;
	}

	end_of_transaction:

	/* Give back UART mutex */
	xSemaphoreGive(uart->tx_mutex);

	exit:
	return rtn;
}

/* Recieve data through UART whithout interupts */
uart_freertos_status_t uart_freertos_rx_dma (uart_freertos_t* uart, const void* data, size_t data_size,
		TickType_t mutex_timeout, TickType_t transfer_timeout)
{
	uart_freertos_status_t rtn;
	rtn.status = UART_FREERTOS_OK;
	rtn.rx_size = 0;

	if(xSemaphoreTake(uart->rx_mutex, mutex_timeout) == pdFALSE)
	{
		rtn.status = UART_FREERTOS_BUSY;
		goto exit;
	}

	rtn.status =  parse_hal_status ( HAL_UART_Receive_DMA(uart->huart,(void*) data, data_size));

	if ((rtn.status == UART_FREERTOS_ERR) || (rtn.status == UART_FREERTOS_BUSY) ) goto end_of_transaction;

	/* Waiting for tx complete */
	if(xSemaphoreTake(uart->rx_complete,transfer_timeout) == pdFALSE)
	{
		rtn.status = UART_FREERTOS_TIMEOUT;
		HAL_UART_AbortReceive_IT(uart->huart);
	}

	end_of_transaction:
	rtn.rx_size = data_size -__HAL_DMA_GET_COUNTER(uart->huart->hdmarx);
	/* Give back UART mutex */
	xSemaphoreGive(uart->rx_mutex);

	exit:
	return rtn;
}

/* Recieve data through UART whithout interupts */
uart_freertos_status_t uart_freertos_rx_dma_idle (uart_freertos_t* uart, const void* data, size_t data_size,
		TickType_t mutex_timeout,TickType_t expectation_timeout, TickType_t idle_timeout)
{
	uart_freertos_status_t rtn ;
	rtn.status = UART_FREERTOS_OK;
	rtn.rx_size = 0;
	TickType_t timeout;

	if(xSemaphoreTake(uart->rx_mutex, mutex_timeout) == pdFALSE)
	{
		rtn.status = UART_FREERTOS_BUSY;
		goto exit;
	}

	/* Turn IDLE interrupt*/
	SET_BIT(uart->huart->Instance->CR1,USART_CR1_IDLEIE);
	rtn.status =  parse_hal_status ( HAL_UART_Receive_DMA(uart->huart,(void*) data, data_size));

	if ((rtn.status == UART_FREERTOS_ERR) || (rtn.status == UART_FREERTOS_BUSY) ) goto end_of_transaction;

	/* Waiting for tx complete */

	timeout = expectation_timeout;

	do
	{
		rtn.rx_size = data_size -__HAL_DMA_GET_COUNTER(uart->huart->hdmarx);	//	current count rx bytes

		if (xSemaphoreTake(uart->rx_complete,timeout) == pdFALSE)
		{
			if( __HAL_DMA_GET_COUNTER(uart->huart->hdmarx) == data_size)	// if receive not start
			{
				rtn.status = UART_FREERTOS_TIMEOUT;
				goto end_of_transaction;
			}
			else if( data_size -__HAL_DMA_GET_COUNTER(uart->huart->hdmarx) == rtn.rx_size)	// if IDLE timeout stop
			{
				rtn.status = UART_FREERTOS_IDLE;
				goto end_of_transaction;
			}
		}
		else
		{
			if(__HAL_DMA_GET_COUNTER(uart->huart->hdmarx) == 0) // buffer is empty, It is DMA interrupt
			{
				rtn.status = UART_FREERTOS_OK;
				goto end_of_transaction;
			}
		}
		timeout = idle_timeout;	//
	}while ((data_size - __HAL_DMA_GET_COUNTER(uart->huart->hdmarx)) - rtn.rx_size ); // if we have receive data do again

	end_of_transaction:

	HAL_UART_AbortReceive_IT(uart->huart);
	rtn.rx_size = data_size - __HAL_DMA_GET_COUNTER(uart->huart->hdmarx);	// size of rx data

	CLEAR_BIT(uart->huart->Instance->CR1,USART_CR1_IDLEIE); // turn off IDLE interrupt

	/* Give back UART mutex */
	xSemaphoreGive(uart->rx_mutex);

	exit:
	return rtn;
}

/* USART RX complete inperrupt */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	struct uart_rtos_list *item = uart_rtos_list_find_item(huart);
	if(item == NULL) return;
	xSemaphoreGiveFromISR(item->uart_rtos->rx_complete,	&xHigherPriorityTaskWoken);
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

/* USART TX complete inperrupt */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	struct uart_rtos_list *item = uart_rtos_list_find_item(huart);
	if(item == NULL) return;
	xSemaphoreGiveFromISR(item->uart_rtos->tx_complete,	&xHigherPriorityTaskWoken);
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void uart_freertos_rx_idle_callback(UART_HandleTypeDef *huart)
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	struct uart_rtos_list *item = uart_rtos_list_find_item(huart);
	if(item == NULL) return;
	READ_REG(huart->Instance->DR);
	xSemaphoreGiveFromISR(item->uart_rtos->rx_complete,	&xHigherPriorityTaskWoken);
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}




