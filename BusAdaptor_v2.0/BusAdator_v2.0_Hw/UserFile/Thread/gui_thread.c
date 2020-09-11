#include "gui_thread.h"

/* ---- 包含HAL.h ---- */
#include "usbd_cdc_if.h"
/* ---- 包含ROTS.h ---- */
#include "FreeRTOS.h"
#include "task.h"
/* ---- 包含GUI ---- */
#include "lvgl.h"
/* ---- 包含用户文件 ---- */
#include "decode.h"
#include "math.h"
#include "page_manager.h"
/* ---- 全局变量声明 ---- */
uint32_t cntst;

/* ---- 私有变量声明 ---- */



void cpu_info_send2usb(void)
{
	uint8_t cpu_info[CDC_DATA_HS_MAX_PACKET_SIZE];
	vTaskList((char *)&cpu_info);
	usb_printf("---------------------------------------------\r\n");
	usb_printf("任务名 任务状态 优先级 剩余栈 任务序号\r\n");
	usb_printf("%s", cpu_info);
	memset(cpu_info,0,CDC_DATA_HS_MAX_PACKET_SIZE); //信息缓冲区清零
	vTaskGetRunTimeStats((char *)&cpu_info);
	usb_printf("任务名 运行计数 使用率\r\n");
	usb_printf("%s", cpu_info);
	usb_printf("---------------------------------------------\r\n\n");
}





/* ---- 任务声明 ---- */
static TaskHandle_t gui_thread_Handle, gui_tick_thread_Handle;
static void gui_thread(void *argument);
static void gui_tick_thread(void *argument);
void gui_thread_creat(void) {
	AppWindows_init();
	BaseType_t xReturn = pdPASS;
	xReturn = xTaskCreate((TaskFunction_t) gui_tick_thread,
			(const char*) "gui_tick_thread", (uint16_t) 2048, (void*) NULL,
			(UBaseType_t) 1,
			(TaskHandle_t*) &gui_tick_thread_Handle);
	if (xReturn == pdPASS)
		;
	else
		while (1)
			;

	xReturn = xTaskCreate((TaskFunction_t) gui_thread,
			(const char*) "gui_thread", (uint16_t) 2048, (void*) NULL,
			(UBaseType_t) 2,
			(TaskHandle_t*) &gui_thread_Handle);
	if (xReturn == pdPASS)
		;
	else
		while (1)
			;
}


/* ---- 任务内容 ---- */

static void gui_thread(void *argument) {
	/* ---- 线程初始化 ---- */

	uint32_t cnt = 0;
	/* ---- 线程初始化 ---- */
	for (;;) {
		vTaskDelay(10);
		cnt++;
		if (cnt == 50) {
			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_11);
			cnt = 0;
		}
		AppWindows_running();
//    lv_label_set_text(labelwifi, MY_SYMBOL_WIFI_3);]
		//cpu_info_send2usb();
	}
}
static void gui_tick_thread(void *argument) {
	/* ---- 线程初始化 ---- */

	/* ---- 线程初始化 ---- */
	for (;;) {
		vTaskDelay(10);
	    //为lvgl提供心跳
	    lv_tick_inc(10);
	    //在任务中调用lv_task_handle()需要把任务对战开大一点，否则可能进入hard_fault。
	    lv_task_handler();
	}
}
