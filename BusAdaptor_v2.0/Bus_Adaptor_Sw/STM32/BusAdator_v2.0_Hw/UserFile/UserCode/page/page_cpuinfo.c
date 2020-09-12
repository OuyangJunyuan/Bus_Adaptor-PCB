/*
 * page_cpuinfo.c
 *
 *  Created on: 2020年9月11日
 *      Author: ou
 */
#include "page_manager_config.h"
#include "page_manager.h"
#include "FreeRTOS.h"
#include "task.h"

static lv_obj_t *appWindow;
static lv_obj_t *infolabel;
static uint32_t cnt;

/* ---- 资源声明 ---- */
LV_FONT_DECLARE(LV_MY_FONT_WIFI);
LV_FONT_DECLARE(LV_MY_FONT_8);
#define MY_SYMBOL_WIFI_3 "\xEF\x87\xAB"
#define MY_SYMBOL_WIFI_2 "\xEF\x9A\xAB"
#define MY_SYMBOL_WIFI_1 "\xEF\x9A\xAA"
#define MY_SYMBOL_WIFI_0 "\xEF\x9A\xAC"


void process_info(uint8_t *buf)
{
	for(uint16_t i=1;buf[i]!='\0';i++){
		if(buf[i]=='\n' && buf[i-1]=='\r')
		{
			buf[i-1]=' ';
		}
	}
}
uint8_t cpu_info[300];
void print_cpu_info(void) {
	memset(cpu_info,0,sizeof(cpu_info));
	if(cnt%2==0)
	{
		vTaskList((char *)&cpu_info);
		process_info(cpu_info);
//		lv_label_set_text_fmt(infolabel,"---------------------------------------------\n"
//				"task						s  p stack n\n%s"
//				"---------------------------------------------\n",cpu_info);
	}else
	{
		vTaskGetRunTimeStats((char *)&cpu_info);
		process_info(cpu_info);
		lv_label_set_text_fmt(infolabel,"---------------------------------------------\n"
				"task					  counts 	  cpu \n%s"
				"---------------------------------------------\n",cpu_info);
	}

}

static void gui_creat(lv_obj_t *par){
	infolabel = lv_label_create(par, NULL);
	lv_obj_set_style_local_text_font(infolabel,LV_LABEL_PART_MAIN,LV_STATE_DEFAULT,&LV_MY_FONT_8);
	lv_obj_set_size(infolabel, LCD_H, LCD_W-TOP_BAR_HEIGHT);
	lv_obj_align(infolabel, par, LV_ALIGN_IN_TOP_LEFT, 10, 0);
	lv_label_set_text_fmt(infolabel,"%d",cnt);
}

static void init() {
	cnt=0;
	gui_creat(appWindow);
}

static void Setup(void) {
	cnt++;
	lv_obj_move_foreground(appWindow);
}
static void Exit(void) {

}

static void Event(int event, void *btn) {

}
static void loop(void) {
	print_cpu_info();
}
void PageRegister_cpu_info(uint8_t pageID) {
	appWindow = AppWindow_GetCont(pageID);
	PageRegister(pageID, init, Setup, loop, Exit, Event);
}
