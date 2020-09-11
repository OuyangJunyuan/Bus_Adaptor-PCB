#ifndef __SIGNAL_THREAD__
#define __SIGNAL_THREAD__


void signal_thread_creat(void);
#define LED_PORT GPIOD
#define LEDR_PIN GPIO_PIN_1
#define LEDG_PIN GPIO_PIN_11
#define LEDB_PIN GPIO_PIN_4

#define LED_R_ON    HAL_GPIO_WritePin(LED_PORT,LEDR_PIN,GPIO_PIN_RESET)
#define LED_R_OFF   HAL_GPIO_WritePin(LED_PORT,LEDR_PIN,GPIO_PIN_SET)
#define LED_R_TOG   HAL_GPIO_TogglePin(LED_PORT,LEDR_PIN)

#define LED_G_ON    HAL_GPIO_WritePin(LED_PORT,LEDG_PIN,GPIO_PIN_RESET)
#define LED_G_OFF   HAL_GPIO_WritePin(LED_PORT,LEDG_PIN,GPIO_PIN_SET)
#define LED_G_TOG   HAL_GPIO_TogglePin(LED_PORT,LEDG_PIN)

#define LED_B_ON    HAL_GPIO_WritePin(LED_PORT,LEDB_PIN,GPIO_PIN_RESET)
#define LED_B_OFF   HAL_GPIO_WritePin(LED_PORT,LEDB_PIN,GPIO_PIN_SET)
#define LED_B_TOG   HAL_GPIO_TogglePin(LED_PORT,LEDB_PIN)
#endif