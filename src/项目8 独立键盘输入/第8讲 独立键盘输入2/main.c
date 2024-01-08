/*************************************************
*实验名：独立键盘输入
*实验效果：按下第1个独立按键K1 松开按键后 
           第一个led灯改变状态
*************************************************/
#include<reg52.h>

sbit led1 = P1^0;
sbit k1 = P3^4;

void delay(unsigned int xms)				
{
	unsigned int i,j;
	for(i=xms;i>0;i--)		      //i=xms即延时约xms毫秒
		for(j=112;j>0;j--);
}

void main()
{
	P1 = 0xff;//熄灭所有led灯
	while(1)
	{
		if(k1 == 0)//判断是否有按下按键的信号
		{
			delay(10);//延时10ms 消抖
			if(k1 == 0)//再次判断按键是否被按下
			{
				while(k1 == 0);//直到按键判断松开
				led1 = ~led1;//松开后执行程序 翻转led灯
			}
		}
	}
}
