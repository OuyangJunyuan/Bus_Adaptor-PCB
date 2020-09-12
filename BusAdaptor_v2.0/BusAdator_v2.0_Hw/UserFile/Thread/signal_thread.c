#include "signal_thread.h"

/* ---- 包含HAL ---- */
#include "usbd_cdc_if.h"
/* ---- 包含ROTS ---- */
#include "FreeRTOS.h"
#include "task.h"
/* ---- 包含GUI ---- */
#include "lvgl.h"
/* ---- 包含用户文件 ---- */
#include "decode.h"
#include "ringbuf.h"
/* ---- 全局变量声明 ---- */

/* ---- 私有变量声明 ---- */

/* ---- 任务声明 ---- */
static TaskHandle_t signal_thread_Handle;
TaskHandle_t can_process_packet_thread_Handle;
TaskHandle_t usb_process_packet_thread_Handle;

static void signal_thread(void *argument);
static void can_process_packet_thread(void *argument);
static void usb_process_packet_thread(void *argument);

void signal_thread_creat(void) {
	BaseType_t xReturn = pdPASS;
	xReturn = xTaskCreate((TaskFunction_t) signal_thread,
			(const char*) "signal", (uint16_t) 1024, (void*) NULL,
			(UBaseType_t) 5,
			(TaskHandle_t*) &signal_thread_Handle);
	if (xReturn == pdPASS)
		;
	else
		while (1)
			;

	xReturn = xTaskCreate((TaskFunction_t) can_process_packet_thread,
			(const char*) "can", (uint16_t) 1024,
			(void*) NULL, (UBaseType_t) 4, //Priority
			(TaskHandle_t*) &can_process_packet_thread_Handle);
	if (xReturn == pdPASS)
		;
	else
		while (1)
			;

	xReturn = xTaskCreate((TaskFunction_t) usb_process_packet_thread,
			(const char*) "usb", (uint16_t) 1024,
			(void*) NULL, (UBaseType_t) 3, //Priority
			(TaskHandle_t*) &usb_process_packet_thread_Handle);
	if (xReturn == pdPASS)
		;
	else
		while (1)
			;
}

extern Chassis_t Controller;
/* ---- 任务内容 ---- */
static void signal_thread(void *argument) {
	/* ---- 线程初始化 ---- */

	static portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	/* ---- 线程初始化 ---- */
	for (;;) {
		vTaskDelayUntil(&xLastWakeTime, 2);
		//HAL_IWDG_Refresh(&hiwdg);
		//CDC_Transmit_HS("1234567890", 10);
		for (int i = 0; i < 8; i++) {
			switch (Controller.Motor[i].mode) {
			case POSITION: {
				positionServoRun(&Controller.Motor[i]);
				break;
			}
			case SPEED: {
				speedServoRun(&Controller.Motor[i]);
				break;
			}
			default: {
				Controller.Motor[i].speed_pid.output = 0;
			}
			}
		}
		CanTransmit_motor();
	}
}

static void can_process_packet_thread(void *argument) {
	/* ---- 线程初始化 ---- */
	Can_Init_User();
	CanRxFrame_t *read_ptr;
	/* ---- 线程初始化 ---- */
	for (;;) {
		ulTaskNotifyTake(pdTRUE,1);
		while (RingBuf_Read(&CanRxRingBuf, (void**) &read_ptr) != READ_END) {
			CanDataPreDecode(read_ptr);
		}
	}
}

static void usb_process_packet_thread(void *argument) {
	/* ---- 线程初始化 ---- */
	USBFrame *read_ptr;
	/* ---- 线程初始化 ---- */
	for (;;) {
		ulTaskNotifyTake(pdTRUE,1);
		while (RingBuf_Read(&USBRxRingBuf, (void**) &read_ptr) != READ_END) {
			if (read_ptr->data[0] == 'B' && read_ptr->data[1] == 'A') {
				//'B''A'0 3 0 0 _ _ _ _ _ _ _ _
				rosCommondDecoder(&read_ptr->data[6], read_ptr->data[5]);
			}
		}
	}
}
