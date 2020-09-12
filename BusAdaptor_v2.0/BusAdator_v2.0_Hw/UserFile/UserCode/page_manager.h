/*
 * page_manager.h
 *
 *  Created on: 2020年9月11日
 *      Author: ou
 */

#ifndef PAGE_MANAGER_H_
#define PAGE_MANAGER_H_
#include "lvgl.h"

/***************************
 * macro define
 * *************************/
#define INIT_BY_PAGENAME(NAME) PageRegister_##NAME
#define _INIT(NAME) INIT_BY_PAGENAME(NAME)
#define PAGENAME(IDX) PAGE_##IDX
#define _PAGE(IDX) _INIT(PAGENAME(IDX))

#define PAGE(N)\
do{\
    extern void _PAGE(N)(uint8_t pageID);\
    _PAGE(N)(N);\
}while(0);



/***************************
 * function extern
 * *************************/
int16_t StatusBar_GetHeight(void);
lv_obj_t* AppWindow_GetCont(uint8_t pageID);
lv_coord_t AppWindow_GetHeight(void) ;
lv_coord_t AppWindow_GetWidth(void) ;
void PageRegister(int ID,void (*init)(void),void (*setup)(void),void (*loop)(void),void (*exit)(void),void (*event)(int event,void *btn));
void AppWindows_Init();
void AppWindows_Running();
void AppWindows_ChangeTo(uint8_t pageid);
void AppWindows_Event(int event, void* param);
#endif /* PAGE_MANAGER_H_ */
