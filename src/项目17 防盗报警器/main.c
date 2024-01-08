/****************BST-M51实验开发板例程*************
*  平台：BST-M51 + Keil U4 + STC89C52
*  名称：防盗报警实验
*  公司：深圳市亚博软件开发有限公司       
*  日期：2015-7：
*  实验效果：拾起传感器时，发出蜂鸣声报警，并让灯闪烁
*************************************************/
#include<reg52.h>

sbit switch0 = P2^1;
sbit beep=P2^3;

void delay()		   //大概0.5ms左右的延时
{
	unsigned char a;
	for(a=450;a>0;a--)
	{
	}
}
void main()
{
	P1 = 0xff;
	while(1)
	{
		if(!switch0) //蜂鸣器以及led报警
		{
			unsigned int m;
			for(m=800;m>0;m--)	  //持续时间0.5ms*800
			{
				beep=~beep;
	
				delay();		 //2000HZ的信号。
			}
			P1=~P1;
			for(m=500;m>0;m--)	 //持续时间0.5ms*2*500
			{
				beep=~beep;	
				delay();
				delay();		//1000HZ的信号。
			}
			P1=~P1;	
		}
	}
}