/*
 * page_manager.h
 *
 *  Created on: 2020年9月11日
 *      Author: ou
 */

#ifndef PAGE_MANAGER_H_
#define PAGE_MANAGER_H_
#include "lvgl.h"



void AppWindows_init();
void AppWindows_running();

lv_obj_t* AppWindow_GetCont(uint8_t pageID);
void PageRegister(int ID,void (*init)(void),void (*setup)(void),void (*loop)(void),void (*exit)(void),void (*event)(int event,void *btn));


//用户通过 PAGE_N  pagename 来定义页名
//一下通过 N来得到相关变量名字
#define INIT_BY_PAGENAME(NAME) PageRegister_##NAME
#define _INIT(NAME) INIT_BY_PAGENAME(NAME)
#define PAGENAME(IDX) PAGE_##IDX
#define _PAGE(IDX) _INIT(PAGENAME(IDX))

#define PAGE(N)\
do{\
    extern void _PAGE(N)(uint8_t pageID);\
    _PAGE(N)(N);\
}while(0);
#endif /* PAGE_MANAGER_H_ */
