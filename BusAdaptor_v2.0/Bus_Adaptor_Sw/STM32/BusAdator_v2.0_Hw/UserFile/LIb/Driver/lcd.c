#include "lcd.h"
/* --- pin defined ---*/
  //PE0   LEDA
  //PE1   RESET
  //PE3   RS
  //PC13  CS
  //PE2   SCK
  //PE6   MOSI
#define OLED_SPI   &hspi4
#define OLED_SCLK_Clr() HAL_GPIO_WritePin(GPIOE,GPIO_PIN_2,GPIO_PIN_RESET)//SCL
#define OLED_SCLK_Set() HAL_GPIO_WritePin(GPIOE,GPIO_PIN_2,GPIO_PIN_SET)

#define OLED_SDIN_Clr() HAL_GPIO_WritePin(GPIOE,GPIO_PIN_6,GPIO_PIN_RESET)//DIN
#define OLED_SDIN_Set() HAL_GPIO_WritePin(GPIOE,GPIO_PIN_6,GPIO_PIN_SET)

#define OLED_RST_Clr() HAL_GPIO_WritePin(GPIOE,GPIO_PIN_1,GPIO_PIN_RESET)//RES
#define OLED_RST_Set() HAL_GPIO_WritePin(GPIOE,GPIO_PIN_1,GPIO_PIN_SET)

#define OLED_DC_Clr() HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,GPIO_PIN_RESET)//DC
#define OLED_DC_Set() HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,GPIO_PIN_SET)
 		     
#define OLED_CS_Clr()  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET)//CS
#define OLED_CS_Set()  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET)

#define OLED_BLK_Clr()  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_0,GPIO_PIN_RESET)//BLK
#define OLED_BLK_Set()  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_0,GPIO_PIN_SET)




#define OLED_CMD  0	
#define OLED_DATA 1	


/* --- define value ---*/
uint16_t BACK_COLOR;  

void LCD_WR_DATA8(uint8_t dat)
{
	OLED_DC_Set();
	OLED_CS_Clr();
  HAL_SPI_Transmit(OLED_SPI,&dat,1,0xffff);
  OLED_CS_Set();	
}


void LCD_WR_DATA16(uint16_t dat)
{
  uint8_t dath=dat>>8; 
	OLED_DC_Set();
	OLED_CS_Clr();
  HAL_SPI_Transmit(OLED_SPI,&dath,1,0xffff);
  OLED_CS_Set();	
  
	OLED_CS_Clr();
  dath=dat;
  HAL_SPI_Transmit(OLED_SPI,&dath,1,0xffff);
  OLED_CS_Set();	
}


void LCD_WR_REG(uint8_t dat)
{
	OLED_DC_Clr();
	OLED_CS_Clr();
  HAL_SPI_Transmit(OLED_SPI,&dat,1,0xffff);
  OLED_CS_Set();	
}


/* --- x1,x2起始和结束列地址;y1，y2起始和结束行地址 ---*/
void LCD_Address_Set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
{
	if(USE_HORIZONTAL==0)
	{
		LCD_WR_REG(0x2a);
		LCD_WR_DATA16(x1+26);
		LCD_WR_DATA16(x2+26);
		LCD_WR_REG(0x2b);
		LCD_WR_DATA16(y1+1);
		LCD_WR_DATA16(y2+1);
		LCD_WR_REG(0x2c);
	}
	else if(USE_HORIZONTAL==2)
	{
		LCD_WR_REG(0x2a);
		LCD_WR_DATA16(x1+1);
		LCD_WR_DATA16(x2+1);
		LCD_WR_REG(0x2b);
		LCD_WR_DATA16(y1+26);
		LCD_WR_DATA16(y2+26);
		LCD_WR_REG(0x2c);
	}
}


void Lcd_Init(void)
{

	OLED_RST_Clr();
	HAL_Delay(200);
	OLED_RST_Set();
	HAL_Delay(20);
	OLED_BLK_Set();
	
  LCD_WR_REG(0x11); 
  HAL_Delay(100);

  LCD_WR_REG(0x21); 

  LCD_WR_REG(0xB1); 
  LCD_WR_DATA8(0x05);
  LCD_WR_DATA8(0x3A);
  LCD_WR_DATA8(0x3A);

  LCD_WR_REG(0xB2);
  LCD_WR_DATA8(0x05);
  LCD_WR_DATA8(0x3A);
  LCD_WR_DATA8(0x3A);

  LCD_WR_REG(0xB3); 
  LCD_WR_DATA8(0x05);  
  LCD_WR_DATA8(0x3A);
  LCD_WR_DATA8(0x3A);
  LCD_WR_DATA8(0x05);
  LCD_WR_DATA8(0x3A);
  LCD_WR_DATA8(0x3A);

  LCD_WR_REG(0xB4);
  LCD_WR_DATA8(0x03);

  LCD_WR_REG(0xC0);
  LCD_WR_DATA8(0x62);
  LCD_WR_DATA8(0x02);
  LCD_WR_DATA8(0x04);

  LCD_WR_REG(0xC1);
  LCD_WR_DATA8(0xC0);

  LCD_WR_REG(0xC2);
  LCD_WR_DATA8(0x0D);
  LCD_WR_DATA8(0x00);

  LCD_WR_REG(0xC3);
  LCD_WR_DATA8(0x8D);
  LCD_WR_DATA8(0x6A);   

  LCD_WR_REG(0xC4);
  LCD_WR_DATA8(0x8D); 
  LCD_WR_DATA8(0xEE); 

  LCD_WR_REG(0xC5);  /*VCOM*/
  LCD_WR_DATA8(0x0E);    

  LCD_WR_REG(0xE0);
  LCD_WR_DATA8(0x10);
  LCD_WR_DATA8(0x0E);
  LCD_WR_DATA8(0x02);
  LCD_WR_DATA8(0x03);
  LCD_WR_DATA8(0x0E);
  LCD_WR_DATA8(0x07);
  LCD_WR_DATA8(0x02);
  LCD_WR_DATA8(0x07);
  LCD_WR_DATA8(0x0A);
  LCD_WR_DATA8(0x12);
  LCD_WR_DATA8(0x27);
  LCD_WR_DATA8(0x37);
  LCD_WR_DATA8(0x00);
  LCD_WR_DATA8(0x0D);
  LCD_WR_DATA8(0x0E);
  LCD_WR_DATA8(0x10);

  LCD_WR_REG(0xE1);
  LCD_WR_DATA8(0x10);
  LCD_WR_DATA8(0x0E);
  LCD_WR_DATA8(0x03);
  LCD_WR_DATA8(0x03);
  LCD_WR_DATA8(0x0F);
  LCD_WR_DATA8(0x06);
  LCD_WR_DATA8(0x02);
  LCD_WR_DATA8(0x08);
  LCD_WR_DATA8(0x0A);
  LCD_WR_DATA8(0x13);
  LCD_WR_DATA8(0x26);
  LCD_WR_DATA8(0x36);
  LCD_WR_DATA8(0x00);
  LCD_WR_DATA8(0x0D);
  LCD_WR_DATA8(0x0E);
  LCD_WR_DATA8(0x10);

  LCD_WR_REG(0x3A); 
  LCD_WR_DATA8(0x05);

  LCD_WR_REG(0x36);
  if(USE_HORIZONTAL==0)LCD_WR_DATA8(0x08);
  else if(USE_HORIZONTAL==1)LCD_WR_DATA8(0xC8);
  else if(USE_HORIZONTAL==2)LCD_WR_DATA8(0x78);
  else LCD_WR_DATA8(0xA8);

  LCD_WR_REG(0x29); 
} 



void LCD_Clear(uint16_t Color)
{
	uint16_t i,j;  	
	LCD_Address_Set(0,0,LCD_W-1,LCD_H-1);
  for(i=0;i<LCD_W;i++)
  {
    for (j=0;j<LCD_H;j++)
      {
        LCD_WR_DATA16(Color);
      }
  }
}

void LCD_DrawPoint(uint16_t x,uint16_t y,uint16_t color)
{
	LCD_Address_Set(x,y,x,y);
	LCD_WR_DATA16(color);
} 






void LCD_Fill(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color)
{          
	uint16_t i,j; 
	LCD_Address_Set(xsta,ysta,xend,yend);      
	for(i=ysta;i<=yend;i++)
	{													   	 	
		for(j=xsta;j<=xend;j++)
      LCD_COLOR_NEXT(color);
	} 					  	    
}

