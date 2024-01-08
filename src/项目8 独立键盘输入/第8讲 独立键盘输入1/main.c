/*************************************************
*实验名：独立键盘输入
*实验效果：按下第1个独立按键K1 2个红色的led灯亮
		   按下第2个独立按键K1 2个绿色的led灯亮
		   按下第3个独立按键K1 2个黄色的led灯亮
		   按下第4个独立按键K1 2个蓝色的led灯亮
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

sbit k1 = P3^4;
sbit k2 = P3^5;
sbit k3 = P3^6;
sbit k4 = P3^7;

void main()
{
	P1 = 0xff;//熄灭所有led灯
	while(1)
	{
	led1 = k1;
	led2 = k1;
	/*	if(k1 == 0)
		{
			led1 = 0;
			led2 = 0;
		}
		else
		{
			led1 = 1;
			led2 = 1;
		}
	*/
	led3 = k2;
	led4 = k2;
	led5 = k3;	
	led6 = k3;
	led7 = k4;
	led8 = k4;
	}
}