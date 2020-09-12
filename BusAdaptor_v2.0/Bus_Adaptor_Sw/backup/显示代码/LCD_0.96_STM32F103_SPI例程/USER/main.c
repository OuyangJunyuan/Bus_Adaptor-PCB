//////////////////////////////////////////////////////////////////////////////////	
//
//  文 件 名   : main.c
//  版 本 号   : v2.0
//  作    者   : 
//  生成日期   : 2018-10-31
//  最近修改   : 
//  功能描述   : LCD SPI接口演示例程(STM32F103系列)
//              说明: 
//              ----------------------------------------------------------------
//              GND     电源地
//              VCC     3.3v电源
//              D0   PB13（SCL）
//              D1   PB15（SDA） 
//              CS   PB12 
//              RES  PB11
//              DC   PB10S
//							BLK     PB1 控制背光
//              ----------------------------------------------------------------
// 修改历史   :
// 日    期   : 
// 作    者   : 
// 修改内容   : 创建文件
//版权所有，盗版必究。
//Copyright(C) 
//All rights reserved
//******************************************************************************/

#include "delay.h"
#include "sys.h"
#include "lcd.h"


int main(void)
 {	
	 u8 i,m;
	 float t=0;
	 delay_init();	    	 //延时函数初始化	  
	 NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	 Lcd_Init();			//初始化OLED  
	 LCD_Clear(WHITE);
	 BACK_COLOR=WHITE;
	 LED_ON;
	 while(1)
	 {
		  LCD_ShowString(0,60,"0.96 TFT SPI",RED);
			LCD_ShowNum1(104,60,t,5,RED);//显示小数
			t+=0.01;
			delay_ms(1000);
		  LCD_Clear(WHITE);

			LCD_ShowString(10,0,"LCD_W:",RED);	LCD_ShowNum(70,0,LCD_W,3,RED);
			LCD_ShowString(10,20,"LCD_H:",RED);LCD_ShowNum(70,20,LCD_H,2,RED);
			for(i=0;i<4;i++)
			{
				LCD_ShowPicture(i*40,40,39+i*40,79);
			}
			delay_ms(1000);
		  LCD_Clear(WHITE);
   }
}