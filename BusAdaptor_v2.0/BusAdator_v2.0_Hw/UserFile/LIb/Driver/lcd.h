#ifndef __LCD_H
#define __LCD_H		

#include "spi.h"


#define USE_HORIZONTAL 0  

#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
#define LCD_W 80
#define LCD_H 160

#else
#define LCD_W 160
#define LCD_H 80
#endif



/* --- color fomat(rgb565) ---*/
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE           	 0x001F  
#define BRED             0XF81F
#define GRED 			       0XFFE0
#define GBLUE			       0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			     0XBC40 
#define BRRED 			     0XFC07 
#define GRAY  			     0X8430 
#define DARKBLUE      	 0X01CF	
#define LIGHTBLUE      	 0X7D7C	
#define GRAYBLUE       	 0X5458
#define LIGHTGREEN     	 0X841F 
#define LGRAY 			     0XC618 
#define LGRAYBLUE        0XA651 
#define LBBLUE           0X2B12 

extern  uint16_t BACK_COLOR;   

void LCD_WR_DATA8(uint8_t dat);
void LCD_WR_DATA16(uint16_t dat);
void LCD_WR_REG(uint8_t dat);
void LCD_Address_Set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);

void Lcd_Init(void);
void LCD_Clear(uint16_t Color);
void LCD_DrawPoint(uint16_t x,uint16_t y,uint16_t color);
void LCD_ShowString(uint16_t x,uint16_t y,const uint8_t *p,uint16_t color);
#define LCD_COLOR_NEXT(__COLOR) LCD_WR_DATA16(__COLOR)		 
#endif  
	 
	 



