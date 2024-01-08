/**********************BST-M51实验开发板例程************************
*  平台：BST-M51 + Keil U4 + STC89C52
*  名称：声音模块检测
*  公司：深圳市亚博软件开发有限公司       
*  日期：2015-7
*  晶振:11.0592MHZ
*  实验效果：有声音时，检测到，并让led1亮
*************************************************/
#include<reg52.h>

sbit led1 = P1^0;//RED
sbit led2 = P1^1;//RED
sbit led3 = P1^2;//GREEN
sbit led4 = P1^3;//GREEN
sbit led5 = P1^4;//YELLOW
sbit led6 = P1^5;//YELLOW
sbit led7 = P1^6;//BLUE
sbit led8 = P1^7;//BLUE

sbit voice = P2^0;

unsigned long level = 0;

void timer0_init()
{
	TMOD = 0x01; //定时器0选择工作方式1
    TH0 = 0xDC;	 //设置初始值	10ms
    TL0 = 0x00; 
    EA = 1;			 //打开总中断
    ET0 = 1;		 //打开定时器0中断
    TR0 = 1;		 //启动定时器0
}

void main()
{
	P1 = 0xff;//熄灭所有led灯
	timer0_init();
	while(1)	//根据不同等级判断led灯亮
	{
		if(voice == 0)
		{
			led1 = voice;
		}
		if((led1 == 0)&&(level >= 1))
		{
			 led2 = 0;
		}
		if((led2 == 0)&&(level >= 2))
		{
			 led3 = 0;
		}
		if((led3 == 0)&&(level >= 3))
		{
			 led4 = 0;
		}
		if((led4 == 0)&&(level >= 4))
		{
			 led5 = 0;
		}
		if((led5 == 0)&&(level >= 5))
		{
			 led6 = 0;
		}
		if((led6 == 0)&&(level >= 6))
		{
			 led7 = 0;
		}
		if((led7 == 0)&&(level >= 7))
		{
			 led8 = 0;
		}
		if(level == 0)
		{
			P1 = 0xff;
		}
	}
}

void timer0() interrupt 1
{
    TH0 = 0xDC;	 //设置初始值  10ms
    TL0 = 0x00; 
	if(voice == 0) //每过10ms检测一次，如果还是有声音则上升等级
		level++;
	else
		level = 0;
}