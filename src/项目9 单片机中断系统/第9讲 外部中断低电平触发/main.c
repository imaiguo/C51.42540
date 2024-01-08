/******************************************
*实验名：外部中断实验（低电平触发）
*实验效果：给P3.2低电平触发中断，让led灯
*		   在触发前后四位状态调换
******************************************/
#include<reg52.h>

void delay(unsigned int xms)				
{
	unsigned int i,j;
	for(i=xms;i>0;i--)		      //i=xms即延时约xms毫秒
		for(j=112;j>0;j--);
}

void main()
{
	P1 = 0x0f;// 0000 1111 下面四个灯亮 上面四个灯灭
	EX0 = 1;//INT0中断允许
	EA = 1;//全局中断打开
	IT0 = 0;//触发方式为低电平触发
	while(1);
}

void low() interrupt 0
{
	P1 = ~P1;
	delay(200);
}