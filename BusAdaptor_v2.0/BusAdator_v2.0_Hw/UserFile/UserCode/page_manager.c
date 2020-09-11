/*
 * page_manager.c
 *
 *  Created on: 2020年9月11日
 *      Author: ou
 */
#include <page_manager.h>
#include "page_manager_config.h"


/***************************
 * 类型定义
 * *************************/
typedef void (*CallbackFunction_t)(void);
typedef void (*EventFunction_t)(int event, void*btn) ;
typedef struct {
	lv_obj_t * obj;
	struct{
		CallbackFunction_t InitCallback;
		CallbackFunction_t SetupCallback;
		CallbackFunction_t LoopCallback;
		CallbackFunction_t ExitCallback;
		EventFunction_t EventCallback;
	}fp;
} PageList_TypeDef;

typedef struct {
	PageList_TypeDef PageList[PAGE_MAX];
} Page_Manager_t;

Page_Manager_t appWindows;

/***************************
 * 资源声明
 * *************************/
LV_FONT_DECLARE(LV_MY_FONT_WIFI);
LV_FONT_DECLARE(LV_MY_FONT_8);
#define MY_SYMBOL_WIFI_3 "\xEF\x87\xAB"
#define MY_SYMBOL_WIFI_2 "\xEF\x9A\xAB"
#define MY_SYMBOL_WIFI_1 "\xEF\x9A\xAA"
#define MY_SYMBOL_WIFI_0 "\xEF\x9A\xAC"


/***************************
 * 私有函数定义
 * *************************/
static lv_obj_t *labelbettery, *labelwifi, *labelusb;
static void AppWindow_CreatBg(void) {
//    LV_IMG_DECLARE(IMG_Background);
//    lv_obj_t * imgBg = lv_img_create(lv_scr_act(), NULL);
//    lv_img_set_src(imgBg, &IMG_Background);
//    lv_obj_align(imgBg, NULL, LV_ALIGN_CENTER, 0, 0);
}


static void AppWindow_BarCrt(void)
{
	labelbettery = lv_label_create(lv_scr_act(), NULL);
	lv_label_set_text(labelbettery, LV_SYMBOL_BATTERY_FULL);
	lv_obj_align(labelbettery, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 0);
	//  lv_label_set_text_fmt(labelbettery,"#ffffff %s",LV_SYMBOL_BATTERY_2);

	labelwifi = lv_label_create(lv_scr_act(), NULL);
	lv_obj_set_style_local_text_font(labelwifi, LV_LABEL_PART_MAIN,
			LV_STATE_DEFAULT, &LV_MY_FONT_WIFI);
	lv_obj_align(labelwifi, NULL, LV_ALIGN_IN_TOP_MID, -7, 0);

	labelusb = lv_label_create(lv_scr_act(), NULL);
	lv_label_set_text(labelusb, LV_SYMBOL_USB);
	lv_obj_align(labelusb, NULL, LV_ALIGN_IN_TOP_LEFT, 4, 0);
}

/***************************
 * 公共函数定义
 * *************************/
void PageRegister(int ID,void (*init)(void),void (*setup)(void),void (*loop)(void),void (*exit)(void),void (*event)(int event,void *btn)){
	appWindows.PageList[ID].fp.InitCallback=init;
	appWindows.PageList[ID].fp.SetupCallback=setup;
	appWindows.PageList[ID].fp.LoopCallback=loop;
	appWindows.PageList[ID].fp.ExitCallback=exit;
	appWindows.PageList[ID].fp.EventCallback=event;
}

int16_t StatusBar_GetHeight(void) {return (int16_t) TOP_BAR_HEIGHT;}
lv_obj_t* AppWindow_GetCont(uint8_t pageID) {return (pageID < PAGE_MAX) ? appWindows.PageList[pageID].obj : NULL;}
lv_coord_t AppWindow_GetHeight(void) {return (lv_obj_get_height(lv_scr_act()) - StatusBar_GetHeight());}
lv_coord_t AppWindow_GetWidth(void) {return (lv_obj_get_width(lv_scr_act()));}











void AppWindows_running(void)
{
	appWindows.PageList[0].fp.SetupCallback();
}




void AppWindows_init() {
	AppWindow_BarCrt();
	AppWindow_CreatBg();
	lv_obj_t *par = lv_scr_act();
	for (int i = 0; i < PAGE_MAX; i++) {
		lv_obj_t *cont = lv_cont_create(par, NULL);
		lv_obj_set_size(cont, AppWindow_GetWidth(), AppWindow_GetHeight());
		lv_obj_align(cont, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);
		lv_cont_set_fit(cont, LV_FIT_NONE);

//		appWindow_Grp[i].style = lv_style_transp;
//		lv_cont_set_style(cont, LV_CONT_STYLE_MAIN, &appWindow_Grp[i].style);
		appWindows.PageList[i].obj = cont;
	}
	PAGE_INIT();
	for (int i = 0; i < PAGE_MAX; i++) {
		appWindows.PageList[i].fp.InitCallback();
	}
}





