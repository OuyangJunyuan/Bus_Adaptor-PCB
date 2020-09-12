//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//测试硬件：STC51
//QQ：2534656669
//创建日期:2015/9/10
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 
//All rights reserved
//********************************************************************************
//V1.1修改说明
//加入横屏显示代码，直接在初始化中切换横竖屏模式
//////////////////////////////////////////////////////////////////////////////////
#include<reg51.h>
#include<absacc.h>
#include<intrins.h>
#include<string.h>
#define uchar unsigned char
#define uint unsigned int


//测试硬件平台：STC12C5A60S2/STC12LE5A60S2
//主频：12MHZ
//单片机IO推挽输出设置定义
//#define USE_LANDSCAPE//测试程序横竖屏切换，横屏请开启此宏，竖屏请屏蔽
#define MCU_STC12//如果您使用的单片机不是STC12系列(如STC89C52)请屏蔽此宏定义
#ifdef MCU_STC12
sfr P3M1  = 0xB1;	//P3M1.n,P3M0.n 	=00--->Standard,	01--->push-pull
sfr P3M0  = 0xB2;	//					=10--->pure input,	11--->open drain
#endif


//---------------------------液晶屏接线说明-------------------------------------//
//接线前请参考液晶屏说明书第10页引脚定义
sbit bl        =P1^5;//接模块BL引脚，背光可以采用IO控制或者PWM控制，也可以直接接到高电平常亮
sbit scl       =P1^0;//接模块CLK引脚,接裸屏Pin9_SCL
sbit sda       =P1^1;//接模块DIN/MOSI引脚，接裸屏Pin8_SDA
sbit rs        =P1^3;//接模块D/C引脚，接裸屏Pin7_A0
sbit cs        =P1^4;//接模块CE引脚，接裸屏Pin12_CS
sbit reset     =P1^2;//接模块RST引脚，接裸屏Pin6_RES
//---------------------------End of液晶屏接线---------------------------------//

//定义常用颜色
#define RED  		0xf800
#define GREEN		0x07e0
#define BLUE 		0x001f
#define WHITE		0xffff
#define BLACK		0x0000
#define YELLOW  0xFFE0
#define GRAY0   0xEF7D   
#define GRAY1   0x8410      	
#define GRAY2   0x4208   

code unsigned char gImage_qq[];





void delay_ms(uint time)
{
 uint i,j;
  for(i=0;i<time;i++)
   for(j=0;j<250;j++);
}


//向SPI总线传输一个8位数据
void  SPI_WriteData(uchar Data)
{
	unsigned char i=0;
	for(i=8;i>0;i--)
	{
		if(Data&0x80)	
		sda=1; //输出数据
		else sda=0;
		scl=0;
		scl=1;
		Data<<=1;
	}
}
//向液晶屏写一个8位指令
void  Lcd_WriteIndex(uchar Data)
{
		
		cs=0;
		rs=0;
		SPI_WriteData(Data); 		
		cs=1;
}
//向液晶屏写一个8位数据
void  Lcd_WriteData(uchar Data)
{	
		unsigned char i=0;
		cs=0;
		rs=1;
		SPI_WriteData(Data); 	
		cs=1;
}
//向液晶屏写一个16位数据
void  LCD_WriteData_16Bit(unsigned int Data)
{
	unsigned char i=0;
	cs=0;
	rs=1;
	SPI_WriteData(Data>>8); 	//写入高8位数据
	SPI_WriteData(Data); 			//写入低8位数据
	cs=1;

}

void Reset()
{
    reset=0;
    delay_ms(100);
    reset=1;
    delay_ms(100);
}
//////////////////////////////////////////////////////////////////////////////////////////////
//液晶屏初始化 for S6D02A1
void lcd_initial()
{	Reset();//Reset before LCD Init.
		
	//LCD Init For 1.44Inch LCD Panel with ST7735R.
	Lcd_WriteIndex(0x11);//Sleep exit 
	delay_ms (120);
	
Lcd_WriteIndex(0x21); 

Lcd_WriteIndex(0xB1); 
Lcd_WriteData(0x05);
Lcd_WriteData(0x3A);
Lcd_WriteData(0x3A);

Lcd_WriteIndex(0xB2);
Lcd_WriteData(0x05);
Lcd_WriteData(0x3A);
Lcd_WriteData(0x3A);

Lcd_WriteIndex(0xB3); 
Lcd_WriteData(0x05);  
Lcd_WriteData(0x3A);
Lcd_WriteData(0x3A);
Lcd_WriteData(0x05);
Lcd_WriteData(0x3A);
Lcd_WriteData(0x3A);

Lcd_WriteIndex(0xB4);
Lcd_WriteData(0x03);

Lcd_WriteIndex(0xC0);
Lcd_WriteData(0x62);
Lcd_WriteData(0x02);
Lcd_WriteData(0x04);

Lcd_WriteIndex(0xC1);
Lcd_WriteData(0xC0);

Lcd_WriteIndex(0xC2);
Lcd_WriteData(0x0D);
Lcd_WriteData(0x00);

Lcd_WriteIndex(0xC3);
Lcd_WriteData(0x8D);
Lcd_WriteData(0x6A);   

Lcd_WriteIndex(0xC4);
Lcd_WriteData(0x8D); 
Lcd_WriteData(0xEE); 

Lcd_WriteIndex(0xC5);  /*VCOM*/
Lcd_WriteData(0x0E);    

Lcd_WriteIndex(0xE0);
Lcd_WriteData(0x10);
Lcd_WriteData(0x0E);
Lcd_WriteData(0x02);
Lcd_WriteData(0x03);
Lcd_WriteData(0x0E);
Lcd_WriteData(0x07);
Lcd_WriteData(0x02);
Lcd_WriteData(0x07);
Lcd_WriteData(0x0A);
Lcd_WriteData(0x12);
Lcd_WriteData(0x27);
Lcd_WriteData(0x37);
Lcd_WriteData(0x00);
Lcd_WriteData(0x0D);
Lcd_WriteData(0x0E);
Lcd_WriteData(0x10);

Lcd_WriteIndex(0xE1);
Lcd_WriteData(0x10);
Lcd_WriteData(0x0E);
Lcd_WriteData(0x03);
Lcd_WriteData(0x03);
Lcd_WriteData(0x0F);
Lcd_WriteData(0x06);
Lcd_WriteData(0x02);
Lcd_WriteData(0x08);
Lcd_WriteData(0x0A);
Lcd_WriteData(0x13);
Lcd_WriteData(0x26);
Lcd_WriteData(0x36);
Lcd_WriteData(0x00);
Lcd_WriteData(0x0D);
Lcd_WriteData(0x0E);
Lcd_WriteData(0x10);

Lcd_WriteIndex(0x3A); 
Lcd_WriteData(0x05);

Lcd_WriteIndex(0x36);
Lcd_WriteData(0xC8);

Lcd_WriteIndex(0x29); 

}




/*************************************************
函数名：LCD_Set_Region
功能：设置lcd显示区域，在此区域写点数据自动换行
入口参数：xy起点和终点
返回值：无
*************************************************/
void Lcd_SetRegion(unsigned int x_start,unsigned int y_start,unsigned int x_end,unsigned int y_end)
{	

	Lcd_WriteIndex(0x2a);
	Lcd_WriteData(0x00);
	Lcd_WriteData(x_start+0x1A);
	Lcd_WriteData(0x00);
	Lcd_WriteData(x_end+0x1A);

	Lcd_WriteIndex(0x2b);
	Lcd_WriteData(0x00);
	Lcd_WriteData(y_start+1);
	Lcd_WriteData(0x00);
	Lcd_WriteData(y_end+1);	
	Lcd_WriteIndex(0x2c);
}


void dsp_single_colour(int color)
{
 	uchar i,j;
	Lcd_SetRegion(0,0,80-1,160-1);
 	for (i=0;i<160;i++)
    	for (j=0;j<80;j++)
        	LCD_WriteData_16Bit(color);
}



//取模方式 水平扫描 从左到右 低位在前
void showimage(const unsigned char *p) //显示40*40 QQ图片
{
  	int i,j,k; 
	unsigned char picH,picL;
	dsp_single_colour(WHITE); //清屏  
	
	for(k=0;k<4;k++)
	{
	   	for(j=0;j<2;j++)
		{	
			Lcd_SetRegion(40*j,40*k,40*j+39,40*k+39);		//坐标设置
		    for(i=0;i<40*40;i++)
			 {	
			 	picL=*(p+i*2);	//数据低位在前
				picH=*(p+i*2+1);				
				LCD_WriteData_16Bit(picH<<8|picL);  						
			 }	
		 }
	}		
}
void main(void)
{
#ifdef MCU_STC12
	P3M1 &= ~(1<<2),	P3M0 |=  (1<<2);	//P3.2 set as push-pull output mode
#endif
	lcd_initial();
	bl=1;
  while(1)
  {
   
	showimage(gImage_qq); 
	delay_ms(1500);	
    }
 }

//16位BMP 40X40 QQ图像取模数据
//Image2LCD取模选项设置
//水平扫描
//16位
//40X40
//不包含图像头数据
//自左至右
//自顶至底
//低位在前
};



