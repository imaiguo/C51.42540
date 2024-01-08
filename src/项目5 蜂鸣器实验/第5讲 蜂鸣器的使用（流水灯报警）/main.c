/***************************************************
*实验名：流水灯报警
*实验效果：程序烧录进去后 蜂鸣器配合流水灯发出声音
*************************************************/
#include<reg52.h>
#include<intrins.h>

#define on 0
#define off 1
sbit fm = P2^3;

void delay(unsigned int xms)				
{
	unsigned int i,j;
	for(i=xms;i>0;i--)		      //i=xms即延时约xms毫秒
		for(j=112;j>0;j--);
}

void main()
{
	P1 = 0xfe;//1111 1110
	while(1)
	{
		delay(100);
		P1 =_crol_(P1,1);/*将P1循环左移1位*/
		fm = on;
		delay(100);
		fm = off;
	}
}