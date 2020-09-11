/*
 * page_manager_config.h
 *
 *  Created on: 2020年9月11日
 *      Author: ou
 */

#ifndef PAGE_MANAGER_CONFIG_H_
#define PAGE_MANAGER_CONFIG_H_

#include "page_manager.h"
/* user config  */
#define TOP_BAR_HEIGHT 15
#define PAGE_MAX 2

#define PAGE_0 motor_info
#define PAGE_1 cpu_info


#define PAGE_INIT() do{PAGE(0);PAGE(1);}while(0)



#endif /* PAGE_MANAGER_CONFIG_H_ */
