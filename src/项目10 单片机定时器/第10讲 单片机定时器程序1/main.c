/*************************************************
*实验名：定时器实验
*实验效果：第一个led灯以定时器0设定的时间周期闪烁
*************************************************/
#include<reg52.h>

sbit led = P1^0;

void timer0_init()
{
	TMOD = 0x01; //定时器0选择工作方式1
    TH0 = 0x00;	 //设置初始值
    TL0 = 0x00; 
    EA = 1;			 //打开总中断
    ET0 = 1;		 //打开定时器0中断
    TR0 = 1;		 //启动定时器0
}

void main()
{
	led = 1;
	timer0_init();
	while(1);
}

void timer0() interrupt 1
{
    TH0 = 0x00;	 //设置初始值
    TL0 = 0x00; 
	led = ~led;
}