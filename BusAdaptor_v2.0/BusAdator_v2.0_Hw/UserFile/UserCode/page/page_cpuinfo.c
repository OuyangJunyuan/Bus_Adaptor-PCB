/*
 * page_cpuinfo.c
 *
 *  Created on: 2020年9月11日
 *      Author: ou
 */
#include <page_manager.h>

static lv_obj_t *appWindow;




static void init(void) {

}

static void Setup(void) {
	lv_obj_move_foreground(appWindow);
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

void PageRegister_cpu_info(uint8_t pageID) {
	appWindow = AppWindow_GetCont(pageID);
	PageRegister(pageID, init, Setup, NULL, Exit, Event);
}
