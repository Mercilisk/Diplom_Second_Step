#include "main.h"
#include "stm32f1xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "cmsis_os.h"

#include "uart_freertos.h"
#include "cobs.h"
#include "uart_cobs_service.h"

size_t uart_cobs_send(uart_cobs_service_t* h, void* data, size_t size,
	TickType_t timeout)
{
	if(h->input_queue == NULL)
		return 0;
	uart_cobs_frame_t frame;
	frame.data = data;
	frame.size = size;
	if(xQueueSend(h->input_queue, &frame, timeout) == pdFALSE)
		return 0;
	else
		return size;
}

size_t uart_cobs_recv(uart_cobs_service_t* h, void** data, TickType_t timeout)
{
	if(h->output_queue == NULL)
		return 0;
	uart_cobs_frame_t frame = {.data = NULL, .size = 0};
	xQueueReceive(h->output_queue, &frame, timeout);
	*data = frame.data;
	return frame.size;
}

void uart_cobs_service_rx_task(void const * argument)
{
	uart_cobs_service_t* h = (uart_cobs_service_t *) argument;
	h->output_queue = xQueueCreate(h->queue_depth, sizeof(uart_cobs_frame_t));
	/* Frame buffer */
	uint8_t* framebuffer = pvPortMalloc(h->queue_depth*h->max_frame_size);
	if(!framebuffer) Error_Handler();
	/* Buffer for COBS */
	size_t cobs_buffer_size = h->max_frame_size + h->max_frame_size/254 + 2;
	uint8_t *buf = pvPortMalloc(cobs_buffer_size);
	if(!buf) Error_Handler();
	uart_freertos_status_t status = {0};
	size_t size = 0;
	/* Data frame handler */
	uart_cobs_frame_t frame = {.data = NULL, .size = 0};
	frame.data = (void *) framebuffer;
	while(1)
	{
		size = 0;
		do
		{
			switch(h->mode)
			{
			case UART_COBS_POLLING:
				status = uart_freertos_rx(h->huart, &(buf[size]),
					sizeof(uint8_t), portMAX_DELAY, HAL_MAX_DELAY);
				switch(status.status)
				{
				case UART_FREERTOS_OK:
					size++;
				default:
					break;
				}
				break;
			case UART_COBS_INTERRUPT:
				status = uart_freertos_rx_it(h->huart, &(buf[size]),
					sizeof(uint8_t), portMAX_DELAY, portMAX_DELAY);
				switch(status.status)
				{
				case UART_FREERTOS_OK:
					size++;
				default:
					break;
				}
				break;
			case UART_COBS_DMA:
				status = uart_freertos_rx_dma_idle(h->huart, &(buf[size]),
					cobs_buffer_size-size, portMAX_DELAY, portMAX_DELAY, 1);
				switch(status.status)
				{
				case UART_FREERTOS_OK:
				case UART_FREERTOS_IDLE:
					size += status.rx_size;
				default:
					break;
				}
				break;
			default:
				break;
			}
			if(size >= cobs_buffer_size) size = 0;
		} while(buf[size-1] != 0x00);
		size--;
		frame.size = cobs_decode(buf, size, frame.data);
		xQueueSend(h->output_queue, &frame, portMAX_DELAY);
		frame.data += h->max_frame_size;
		if(frame.data-((void *)framebuffer) >= h->queue_depth*h->max_frame_size)
			frame.data = (void *) framebuffer;
	}
}

void uart_cobs_service_tx_task(void const * argument)
{
	uart_cobs_service_t* h = (uart_cobs_service_t *) argument;
	h->input_queue = xQueueCreate(h->queue_depth, sizeof(uart_cobs_frame_t));
	/* Data frame handler */
	uart_cobs_frame_t frame = {.data = NULL, .size = 0};
	/* Buffer for COBS */
	size_t cobs_buffer_size = h->max_frame_size + h->max_frame_size/254 + 2;
	uint8_t *buf = pvPortMalloc(cobs_buffer_size);
	if(!buf) Error_Handler();
	size_t size = 0;
	while(1)
	{
		xQueueReceive(h->input_queue, &frame, portMAX_DELAY);
		size = cobs_encode((uint8_t *) frame.data, frame.size, buf);
		buf[size++] = 0;
		switch(h->mode)
		{
		case UART_COBS_POLLING:
			uart_freertos_tx(h->huart, buf, size,
				portMAX_DELAY, HAL_MAX_DELAY);
			break;
		case UART_COBS_INTERRUPT:
			uart_freertos_tx_it(h->huart, buf, size,
				portMAX_DELAY, portMAX_DELAY);
			break;
		case UART_COBS_DMA:
			uart_freertos_tx_dma(h->huart, buf, size,
				portMAX_DELAY, portMAX_DELAY);
			break;
		default:
			break;
		}
	}
}

osThreadId uart_cobs_service_rx_create(char *name, osPriority priority,
	uint32_t instances, uint32_t stack_size, uart_cobs_service_t* h)
{

	/* create treads */
	osThreadDef_t thread = {
		.name		= name,
		.pthread	= uart_cobs_service_rx_task,
		.tpriority	= priority,
		.instances	= instances,
		.stacksize	= stack_size
	};

	return osThreadCreate(&thread, (void *) h);
}

osThreadId uart_cobs_service_tx_create(char *name, osPriority priority,
	uint32_t instances, uint32_t stack_size, uart_cobs_service_t* h)
{

	/* create treads */
	osThreadDef_t thread = {
		.name		= name,
		.pthread	= uart_cobs_service_tx_task,
		.tpriority	= priority,
		.instances	= instances,
		.stacksize	= stack_size
	};

	return osThreadCreate(&thread, (void *) h);
}
