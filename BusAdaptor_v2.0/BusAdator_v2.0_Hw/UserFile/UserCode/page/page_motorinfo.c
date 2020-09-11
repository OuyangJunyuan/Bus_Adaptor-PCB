/*
 * page_motorinfo.c
 *
 *  Created on: 2020年9月11日
 *      Author: ou
 */
/*
 * page_cpuinfo.c
 *
 *  Created on: 2020年9月11日
 *      Author: ou
 */
#include <page_manager.h>
#include "algorithm.h"
#include "math.h"
static lv_obj_t *appWindow;


static lv_obj_t *barmotor[8];

/* ---- 资源声明 ---- */
LV_FONT_DECLARE(LV_MY_FONT_WIFI);
LV_FONT_DECLARE(LV_MY_FONT_8);
#define MY_SYMBOL_WIFI_3 "\xEF\x87\xAB"
#define MY_SYMBOL_WIFI_2 "\xEF\x9A\xAB"
#define MY_SYMBOL_WIFI_1 "\xEF\x9A\xAA"
#define MY_SYMBOL_WIFI_0 "\xEF\x9A\xAC"

static void gui_creat(lv_obj_t *par) {
	for (int i = 0; i < 8; i++) {
		barmotor[i] = lv_bar_create(par, NULL);
		lv_obj_set_size(barmotor[i], 70, 10);
		lv_obj_align(barmotor[i], NULL, LV_ALIGN_CENTER, 0, -50 + i * 15);
		//lv_bar_set_anim_time(barmotor[i], 2000);
		lv_bar_set_value(barmotor[i], 100, LV_ANIM_ON);
		lv_obj_set_state(barmotor[i], LV_STATE_DISABLED);
	}
}
static void update_motor_info_gui() {
	for (int i = 0; i < 8; i++) {
		switch (Controller.Motor[0].mode) {
		case (UNDEFINED): {
			lv_obj_set_state(barmotor[0], LV_STATE_DISABLED);
			break;
		}
		case (SPEED): {
			lv_obj_set_state(barmotor[0], LV_STATE_DEFAULT);
			lv_bar_set_value(barmotor[0],
					fabs(Controller.Motor[0].rotor_vol / (10000) * 100),
					LV_ANIM_ON);
			break;
		}
		case (POSITION): {
			lv_obj_set_state(barmotor[0], LV_STATE_DEFAULT);
			lv_bar_set_value(barmotor[0],
					fabs(
							Controller.Motor[0].output_angle / (2 * 3.14159)
									* 100), LV_ANIM_ON);
			break;
		}
		default:
			;
		}
	}
}




static void loop(void) {
}

static void init(void) {
	gui_creat(appWindow);
}

static void Setup(void) {
	lv_obj_move_foreground(appWindow);
	update_motor_info_gui();
}
static void Exit(void) {
//    LV_OBJ_ADD_ANIM(contICON, x, lv_obj_get_width(contDisp) + ICON_Size, LV_ANIM_TIME_DEFAULT);
//    LV_OBJ_ADD_ANIM(contTitle, y, lv_obj_get_y(contTitle) + lv_obj_get_height(contTitle), LV_ANIM_TIME_DEFAULT);
//    LV_OBJ_ADD_ANIM(contSel, opa_scale, LV_OPA_TRANSP, LV_ANIM_TIME_DEFAULT);
//    PageDelay(LV_ANIM_TIME_DEFAULT);
	//lv_obj_clean(appWindow);
}
static void Event(int event, void *btn) {

}

void PageRegister_motor_info(uint8_t pageID) {
	appWindow = AppWindow_GetCont(pageID);
	PageRegister(pageID, init, Setup, loop, Exit, Event);
}

